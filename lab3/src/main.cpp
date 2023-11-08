#include <Arduino.h>
#include <Chassis.h>
#include <IMU.h>
#include <MedianFilter.h>
#include <Romi32U4.h>

#define COLLISION_THRESHOLD 100  // mg
#define PICKUP_THRESHOLD 1500    // mg
#define DRIVE_SPEED 100          // mm/s
#define REVERSE_TIME 1000        // ms
#define TURN_ANGLE 90            // deg
#define TURN_SPEED 90            // deg/s

// Robot states
enum ROBOT_STATE { IDLE, DRIVE, REVERSE, TURN };
ROBOT_STATE robotState = IDLE;  // initial state: IDLE

// Sensors
IMU imu;
Romi32U4ButtonA buttonA;

// Chassis
Chassis chassis;

void setup() {
  // IMU setup
  imu.init(100, 5);
}

void loop() {
  switch (robotState) {
    case IDLE: {
      // Time to start driving
      if (buttonA.getSingleDebouncedRelease()) {
        robotState = DRIVE;
        chassis.stop();
      } else {
        // Stop by default
        chassis.stop();
      }
      break;
    }

    case DRIVE: {
      if (imu.hadCollision(COLLISION_THRESHOLD)) {
        robotState = REVERSE;
        chassis.stop();
      } else if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
        robotState = IDLE;
        chassis.stop();
      } else {
        // Drive forward by default
        chassis.setTargetSpeeds(DRIVE_SPEED, DRIVE_SPEED);
      }
      break;
    }

    case REVERSE: {
      // Reverse for 1 second
      chassis.setTargetSpeeds(-DRIVE_SPEED, -DRIVE_SPEED);
      uint32_t startTime = millis();
      while (millis() - startTime < 1000) {
        if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
          robotState = IDLE;
          chassis.stop();
          break;
        }
      }
      break;
    }

    case TURN: {
      chassis.pointTurn(90, 90);
      while (!chassis.isMotionComplete()) {
        if (imu.beingPickedUp(PICKUP_THRESHOLD)) {
          robotState = IDLE;
          chassis.stop();
          break;
        }
      }
      robotState = DRIVE;
      break;
    }
  }
  Serial.println(robotState);
}
