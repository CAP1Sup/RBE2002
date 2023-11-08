#include <Arduino.h>
#include <Chassis.h>
#include <IMU.h>
#include <MedianFilter.h>
#include <Romi32U4.h>

// Op modes
// #define PRINT_ENCODER_COUNTS
#define ACCEL_TESTING
// #define STATE_MACHINE

#if defined(PRINT_ENCODER_COUNTS) && defined(STATE_MACHINE)
#error "Only one of PRINT_ENCODER_COUNTS or STATE_MACHINE can be defined"
#endif

#define IMU_UPDATE_RATE 50      // Hz
#define MEDIAN_READINGS 15      // number of readings to use for median filter
#define COLLISION_THRESHOLD 100 // mg
#define PICKUP_THRESHOLD 1500   // mg
#define DRIVE_SPEED 100         // mm/s
#define REVERSE_DIST 100        // mm
#define TURN_ANGLE 90           // deg
#define TURN_SPEED 90           // deg/s

// Robot states
enum ROBOT_STATE { IDLE, DRIVE, REVERSE, TURN };
ROBOT_STATE robotState = IDLE; // initial state: IDLE

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
  chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
#elif defined(STATE_MACHINE)
  imu.init(IMU_UPDATE_RATE, MEDIAN_READINGS);
#endif
}

void loop() {
#ifdef PRINT_ENCODER_COUNTS
  // Print encoder counts
  Serial.print("L: ");
  Serial.print(chassis.getLeftCount());
  Serial.print(" R: ");
  Serial.println(chassis.getRightCount());
  delay(100);
#elif defined(ACCEL_TESTING)
  chassis.updateMotorPID();
  imu.printAccel();
  delay(10);

#elif defined(STATE_MACHINE)
  switch (robotState) {
  case IDLE: {
    // Time to start driving
    if (buttonA.getSingleDebouncedRelease()) {
      robotState = DRIVE;
      chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
    } else {
      // Stop by default
      chassis.stop();
    }
    break;
  }

  case DRIVE: {
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      robotState = IDLE;
      chassis.stop();
    } else if (imu.hadCollision(COLLISION_THRESHOLD)) {
      robotState = REVERSE;
      chassis.resetDrivePID();
      chassis.drive(DRIVE_SPEED, REVERSE_DIST);
    }
    break;
  }

  case REVERSE: {
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      robotState = IDLE;
      chassis.stop();
    } else if (chassis.isMotionComplete()) {
      robotState = TURN;
      chassis.resetDrivePID();
      chassis.pointTurn(TURN_ANGLE, TURN_SPEED);
    }
    break;
  }

  case TURN: {
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      robotState = IDLE;
      chassis.stop();
    } else if (chassis.isMotionComplete()) {
      robotState = DRIVE;
      chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
    }
    break;
  }
  }
#endif
}
