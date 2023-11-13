#include <Arduino.h>
#include <Chassis.h>
#include <IMU.h>
#include <MedianFilter.h>
#include <Romi32U4.h>

// Op modes
// #define PRINT_ENCODER_COUNTS
// #define ACCEL_TESTING
#define STATE_MACHINE

#if defined(PRINT_ENCODER_COUNTS) && defined(ACCEL_TESTING) && \
    defined(STATE_MACHINE)
#error "Only one of the modes can be defined"
#endif

#define IMU_UPDATE_RATE 50      // Hz
#define MEDIAN_READINGS 5       // number of readings to use for median filter
#define COLLISION_THRESHOLD 75  // mg
#define PICKUP_THRESHOLD 1300   // mg
#define DRIVE_SPEED 200         // mm/s
#define ACCEL_TIME 250          // ms
#define REVERSE_SPEED 50        // mm/s
#define REVERSE_DIST 50         // mm
#define TURN_ANGLE 90           // deg
#define TURN_SPEED 30           // deg/s

// Robot states
enum ROBOT_STATE { IDLE, DRIVE, REVERSE, TURN };
ROBOT_STATE robotState = IDLE;  // initial state: IDLE
uint32_t accelStartTime = 0;

// Sensors
IMU imu;
Romi32U4ButtonA buttonA;

// Chassis
Chassis chassis;

void setup() {
// IMU setup
#ifdef PRINT_ENCODER_COUNTS
  Serial.begin(9600);
#elif defined(ACCEL_TESTING)
  imu.init(IMU_UPDATE_RATE, MEDIAN_READINGS);
  chassis.resetDrivePID();
  // chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
#elif defined(STATE_MACHINE)
  imu.init(IMU_UPDATE_RATE, MEDIAN_READINGS);
  chassis.init();
#endif
}

void loop() {
#ifdef PRINT_ENCODER_COUNTS
  // Print encoder counts
  Serial.print("L: ");
  Serial.print(chassis.getLeftEncoderCount());
  Serial.print(" R: ");
  Serial.println(chassis.getRightEncoderCount());
  delay(100);
#elif defined(ACCEL_TESTING)
  chassis.updateMotorPID();
  imu.updateIfNeeded();
  imu.printAccel();
  delay(10);

#elif defined(STATE_MACHINE)
  switch (robotState) {
    case IDLE: {
      // Time to start driving
      if (buttonA.getSingleDebouncedRelease()) {
        robotState = DRIVE;
        imu.purgeFilters();
        chassis.setWheelSpeeds(DRIVE_SPEED / 10.0f, DRIVE_SPEED / 10.0f);
        accelStartTime = millis();
      } else {
        // Stop by default
        chassis.idle();
      }
      break;
    }

    case DRIVE: {
      // imu.printAccel();
      if (millis() - accelStartTime > ACCEL_TIME) {
        if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
          Serial.println("Being picked up!");
          robotState = IDLE;
          chassis.idle();
        } else if (imu.hadCollision(COLLISION_THRESHOLD)) {
          robotState = REVERSE;
          Serial.println("Collision!");
          chassis.driveFor(-REVERSE_DIST / 10.0f, -REVERSE_SPEED / 10.0f);
        }
      }

      break;
    }

    case REVERSE: {
      if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
        robotState = IDLE;
        Serial.print("Picked Up!");
        chassis.idle();
      } else if (chassis.checkMotionComplete()) {
        robotState = TURN;
        Serial.print("Turning!");
        chassis.turnFor(-TURN_ANGLE, TURN_SPEED);
      }
      break;
    }

    case TURN: {
      if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
        robotState = IDLE;
        Serial.print("Picked up from turning");
        chassis.idle();
      } else if (chassis.checkMotionComplete()) {
        robotState = DRIVE;
        Serial.print("Go to drive, ");
        imu.purgeFilters();
        chassis.setWheelSpeeds(DRIVE_SPEED / 10.0f, DRIVE_SPEED / 10.0f);
        accelStartTime = millis();
      }
      break;
    }
  }

#endif
}
