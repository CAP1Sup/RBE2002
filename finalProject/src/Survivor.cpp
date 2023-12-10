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

Survivor::Survivor() {
  IR.init(IR_PIN);
  sonar.init(TRIG_PIN, ECHO_PIN);
  chassis.init();
}

void Survivor::run() {
  switch (robot_state) {
    case IDLE: {
      if (buttonA.getSingleDebouncedRelease()) {
        robot_state = WANDERING;
        wallFollowPID.resetSum();
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
        float pidOut = wallFollowPID.calcEffort(TARGET_DISTANCE - avgDist);
        Serial.print("Avg dist: ");
        Serial.println(avgDist);

        // Constrain the output
        pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

        // Set the target speeds
        chassis.setWheelSpeeds(DRIVE_SPEED + pidOut, DRIVE_SPEED - pidOut);
      }

      // Check if the robot is being chased
      /*
      getMQTTMessages();
      if (tag.id == 0 || tag.id == 1 || tag.id == 2 || tag.id == 3) {
        robot_state = CHASED;
        chasedTime = millis();
        wallFollowPID.resetSum();
        IR.resetDistAvg();
      }*/

      if (buttonA.getSingleDebouncedRelease()) {
        chassis.idle();
        robot_state = IDLE;
      }
      break;
    }
    case CHASED: {
      float avgDist = IR.getAvgDistance();

      // Calculate the wall distance PID output if needed
      if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
        lastPIDUpdate = millis();
        float pidOut =
            chasedWallFollowPID.calcEffort(TARGET_DISTANCE - avgDist);
        Serial.print("Avg dist: ");
        Serial.println(avgDist);

        // Constrain the output
        pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

        // Set the target speeds
        chassis.setWheelSpeeds((DRIVE_SPEED + pidOut) * 1.5f,
                               (DRIVE_SPEED - pidOut) * 1.5f);
      }

      if (millis() - updateTime >= CHASED_TIME_INTERVAL) {
        robot_state = CHASED;
        wallFollowPID.resetSum();
        IR.resetDistAvg();
      }

      /*if (tag.w >= 100) {
        robot_state = IDLE;
        chassis.idle();
        wallFollowPID.resetSum();
        IR.resetDistAvg();
      }*/
    }
  }
}
#endif