// #pragma once
// #include "survivor.h"

// // Wall following constants
// #define DIST_PID_UPDATE_INTERVAL 50 // ms
// #define TARGET_DISTANCE 30.0f       // mm
// #define MAX_PID_OUTPUT 50.0f        // mm/s
// #define DRIVE_SPEED 75.0f           // mm/s
// #define CHASED_TIME_INTERVAL 5000   // ms

// Chassis chassis;
// SonarSensor sonar;
// IRSensor IR;
// //PIDController wallFollowPID(4.0f, 0.0f, 0.0f);
// //PIDController chasedWallFollowPID(3.0f, 1.0f, 0.0f);
// uint32_t lastPIDUpdate = 0;
// uint32_t chasedTime = 0;
// uint32_t updateTime = 0;

// void loop2() {
//   // switch (robot_state) {
//   // case IDLE: {
//   //   if (buttonA.getSingleDebouncedRelease()) {
//   //     robot_state = LOST;
//   //     wallFollowPID.resetSum();
//   //     IR.resetDistAvg();
//   //   }
//   //   break;
//   // }
//   // case LOST: {
//   //   // Get the average distance
//   //   // Also updates the average distance if needed
//   //   float avgDist = IR.getAvgDistance();

//   //   // Calculate the wall distance PID output if needed
//   //   if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
//   //     lastPIDUpdate = millis();
//   //     float pidOut = wallFollowPID.calcEffort(TARGET_DISTANCE - avgDist);
//   //     Serial.print("Avg dist: ");
//   //     Serial.println(avgDist);

//   //     // Constrain the output
//   //     pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

//   //     // Set the target speeds
//   //     chassis.setWheelSpeeds(DRIVE_SPEED + pidOut, DRIVE_SPEED - pidOut);
//   //   }

//   //   /*if (tag.id == 0 || tag.id == 1 || tag.id == 2 || tag.id == 3) {
//   //     robot_state = CHASED;
//   //     chasedTime = millis();
//   //     wallFollowPID.resetSum();
//   //     IR.resetDistAvg();
//   //   }*/

//   //   if (buttonA.getSingleDebouncedRelease()) {
//   //     chassis.idle();
//   //     robot_state = IDLE;
//   //   }
//   //   break;
//   // }
//   // case CHASED: {

//   //   float avgDist = IR.getAvgDistance();

//   //   // Calculate the wall distance PID output if needed
//   //   if (millis() - lastPIDUpdate >= DIST_PID_UPDATE_INTERVAL) {
//   //     lastPIDUpdate = millis();
//   //     float pidOut = chasedWallFollowPID.calcEffort(TARGET_DISTANCE -
//   //     avgDist); Serial.print("Avg dist: "); Serial.println(avgDist);

//   //     // Constrain the output
//   //     pidOut = constrain(pidOut, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);

//   //     // Set the target speeds
//   //     chassis.setWheelSpeeds((DRIVE_SPEED + pidOut) * 1.5f,
//   //                            (DRIVE_SPEED - pidOut) * 1.5f);
//   //   }

//   //   if (millis() - updateTime >= CHASED_TIME_INTERVAL) {
//   //     robot_state = CHASED;
//   //     wallFollowPID.resetSum();
//   //     IR.resetDistAvg();
//   //   }

//   //   /*if (tag.w >= 100) {
//   //     robot_state = IDLE;
//   //     chassis.idle();
//   //     wallFollowPID.resetSum();
//   //     IR.resetDistAvg();
//   //   }*/
//   // }
//   // }
// }