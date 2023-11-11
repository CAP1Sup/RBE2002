#include <Arduino.h>
#include <Chassis.h>
#include <IMU.h>
#include <MedianFilter.h>
#include <Romi32U4.h>

// Op modes
// #define PRINT_ENCODER_COUNTS
//#define ACCEL_TESTING
#define STATE_MACHINE

#if defined(PRINT_ENCODER_COUNTS) && defined(STATE_MACHINE)
#error "Only one of PRINT_ENCODER_COUNTS or STATE_MACHINE can be defined"
#endif

#define IMU_UPDATE_RATE 100    // Hz
#define MEDIAN_READINGS 50     // number of readings to use for median filter
#define COLLISION_THRESHOLD 30 // mg
#define PICKUP_THRESHOLD 7500  // mg
#define DRIVE_SPEED 100        // mm/s
#define REVERSE_DIST 20        // mm
#define TURN_ANGLE 90          // deg
#define TURN_SPEED 40          // deg/s

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
  chassis.resetDrivePID();
  chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
#elif defined(STATE_MACHINE)
  imu.init(IMU_UPDATE_RATE, MEDIAN_READINGS);
  chassis.resetDrivePID();
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
      chassis.resetDrivePID();
      chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
    } else {
      // Stop by default
      chassis.stop();
    }
    break;
  }

  case DRIVE: {
    chassis.updateMotorPID();
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      Serial.print("Being picked up");
      robotState = IDLE;
      chassis.stop();
    } else if (imu.hadCollision(COLLISION_THRESHOLD)) {
      robotState = REVERSE;
      Serial.print("Collision");
      chassis.resetDrivePID();
      chassis.drive(DRIVE_SPEED, (-1) * REVERSE_DIST);
    }
    break;
  }

  case REVERSE: {
    chassis.updateMotorPID();
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      robotState = IDLE;
      Serial.print("in idle");
      chassis.stop();
    } else if (chassis.isMotionComplete()) {
      robotState = TURN;
      Serial.print("in turn");
      chassis.resetDrivePID();
      chassis.pointTurn(TURN_ANGLE, TURN_SPEED);
    }
    break;
  }

  case TURN: {
    chassis.updateMotorPID();
    if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
      robotState = IDLE;
      Serial.print("in idle from turn");
      chassis.stop();
    } else if (chassis.isMotionComplete()) {
      robotState = DRIVE;
      Serial.print("in drive");
      chassis.resetDrivePID();
      chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
    }
    break;
  }
  }
    imu.printAccel();
#endif
}
