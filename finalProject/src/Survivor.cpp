#include "survivor.h"

#ifdef SURVIVOR

#include <Arduino.h>
#include <Chassis.h>
#include <IRdecoder.h>
#include <PIDcontroller.h>
#include <Rangefinder.h>
#include <Romi32U4.h>

#include "IRSensor.h"
#include "LineSensor.h"
#include "SonarSensor.h"

Chassis chassis;
Rangefinder rangefinder(ECHO_PIN, TRIG_PIN);

void Survivor::init() {
  IR.init(IR_PIN);
  chassis.init();
  rangefinder.init();
}

void Survivor::run() {
  switch (state) {
    case IDLE: {
      if (buttonA.getSingleDebouncedRelease()) {
        state = WANDERING;
        wallFollowPID.reset();
        IR.resetDistAvg();
      }
      break;
    }
    case WANDERING: {
      // Get the average distance
      // Also updates the average distance if needed
      float avgDist = IR.getAvgDistance();

      // Calculate the wall distance PID output if needed
      if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
        lastPIDUpdate = millis();
        float pidOut = wallFollowPID.calculate(TARGET_DISTANCE - avgDist);
        Serial.print(F("Avg dist: "));
        Serial.println(avgDist);

        // Constrain the output
        pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

        // Set the target speeds
        if (!turning) {
          if (avgDist > NO_WALL_THRESHOLD && !turning) {
            chassis.driveFor(NO_WALL_FORWARD_DIST * MM_TO_CM,
                             DRIVE_SPEED * MM_TO_CM, true);
            chassis.turnFor(LEFT_TURN_ANGLE, TURN_SPEED);
            turning = true;
          } else {
            chassis.setWheelSpeeds((DRIVE_SPEED + pidOut) * MM_TO_CM,
                                   (DRIVE_SPEED - pidOut) * MM_TO_CM);
          }
        }
      }

      // Serial.print(F("Distance: "));
      // Serial.println(rangefinder.getDistance());

      // Check if the robot has a wall in front of it
      if (rangefinder.getDistance() < WALL_DISTANCE * MM_TO_CM && !turning) {
        // Turn away from the wall
        chassis.turnFor(RIGHT_TURN_ANGLE, TURN_SPEED);
        turning = true;
      }

      // Check if the robot is being chased
      /*
      getMQTTMessages();
      if (tag.id == 0 || tag.id == 1 || tag.id == 2 || tag.id == 3) {
        state = CHASED;
        chasedTime = millis();
        chasedWallFollowPID.reset();
        IR.resetDistAvg();
      }*/

      if (chassis.checkMotionComplete()) {
        turning = false;
        IR.resetDistAvg();
      }

      if (buttonA.getSingleDebouncedRelease()) {
        chassis.idle();
        state = IDLE;
      }
      break;
    }
    case CHASED: {
      float avgDist = IR.getAvgDistance();

      // Calculate the wall distance PID output if needed
      if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
        lastPIDUpdate = millis();
        float pidOut = chasedWallFollowPID.calculate(TARGET_DISTANCE - avgDist);
        Serial.print("Avg dist: ");
        Serial.println(avgDist);

        // Constrain the output
        pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

        // Set the target speeds
        chassis.setWheelSpeeds((DRIVE_SPEED + pidOut) * 1.5f * MM_TO_CM,
                               (DRIVE_SPEED - pidOut) * 1.5f * MM_TO_CM);
      }

      if (millis() - updateTime >= CHASED_TIME_INTERVAL) {
        state = WANDERING;
        wallFollowPID.reset();
        IR.resetDistAvg();
      }

      /*if (tag.w >= 100) {
        state = IDLE;
        chassis.idle();
        wallFollowPID.resetSum();
        IR.resetDistAvg();
      }*/
    }
  }
}
#endif