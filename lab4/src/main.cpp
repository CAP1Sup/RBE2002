#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>

#include "DDPoseEstimator.h"

// Op mode
#define QUESTION_NUM 1

// General settings
#define SAME_SPEED_THRESHOLD 3   // mm/s
#define POSE_UPDATE_INTERVAL 50  // ms

// Constants
#define MM_TO_CM 0.1

// Questions
#if QUESTION_NUM == 1
enum ROBOT_STATE { IDLE, DRIVE_AWAY, TURN_AROUND, DRIVE_BACK };
#define DRIVE_TIME 10000  // ms
#define DRIVE_SPEED 100   // mm/s
#define TURN_SPEED 60     // deg/s
#define TURN_ANGLE 180    // deg

#elif QUESTION_NUM == 2
enum ROBOT_STATE { IDLE, DRIVE_STRAIGHT, SLOW_TURN };
#define DRIVE_TIME 10000  // ms
#define DRIVE_SPEED 100   // mm/s
#define TURN_TIME 10000   // ms
#define TURN_SPEED 9      // deg/s

#elif QUESTION_NUM == 3
enum ROBOT_STATE { IDLE, SLOW_CIRCLE };
#define DRIVE_TIME 30000  // ms
#define DRIVE_SPEED 50    // mm/s
#define TURN_SPEED \
  (DRIVE_TIME / 1000 / 360)  // deg/s (1 rotation in DRIVE_TIME)
#endif

// Initial state will always be IDLE
ROBOT_STATE state = IDLE;  // initial state: IDLE

DDPoseEstimator poseEstimator(POSE_UPDATE_INTERVAL, SAME_SPEED_THRESHOLD);
Romi32U4ButtonA buttonA;
Chassis chassis;

uint32_t lastStateChange = 0;

void setup() { chassis.init(); }

void loop() {
#if QUESTION_NUM == 1
  switch (state) {
    case IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        state = DRIVE_AWAY;
        delay(1000);  // allow time to move hand away from button
        poseEstimator.reset();

        // Write whatever motion commands you want here
        chassis.setWheelSpeeds(DRIVE_SPEED * MM_TO_CM, DRIVE_SPEED * MM_TO_CM);
        lastStateChange = millis();
      } else {
        chassis.idle();
      }
      break;

    case DRIVE_AWAY:
      if (millis() - lastStateChange >= DRIVE_TIME) {
        state = TURN_AROUND;
        chassis.turnFor(TURN_ANGLE, TURN_SPEED);
      }
      break;

    case TURN_AROUND:
      if (chassis.checkMotionComplete()) {
        state = DRIVE_BACK;
        chassis.setWheelSpeeds(DRIVE_SPEED * MM_TO_CM, DRIVE_SPEED * MM_TO_CM);
        lastStateChange = millis();
      }
      break;

    case DRIVE_BACK:
      if (millis() - lastStateChange >= DRIVE_TIME) {
        state = IDLE;
        chassis.idle();
      }
      break;
  }

#elif QUESTION_NUM == 2
  switch (state) {
    case IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        state = DRIVE_STRAIGHT;
        delay(1000);  // allow time to move hand away from button
        poseEstimator.reset();

        // Write whatever motion commands you want here
        chassis.setWheelSpeeds(DRIVE_SPEED * MM_TO_CM, DRIVE_SPEED * MM_TO_CM);
        lastStateChange = millis();
      } else {
        chassis.idle();
      }
      break;

    case DRIVE_STRAIGHT:
      if (millis() - lastStateChange >= DRIVE_TIME) {
        state = SLOW_TURN;
        chassis.setTwist(DRIVE_SPEED * MM_TO_CM, TURN_SPEED);
        lastStateChange = millis();
      }
      break;

    case SLOW_TURN:
      if (millis() - lastStateChange >= TURN_TIME) {
        state = IDLE;
        chassis.idle();
      }
      break;
  }

#elif QUESTION_NUM == 3
  switch (state) {
    case IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        state = SLOW_CIRCLE;
        delay(1000);  // allow time to move hand away from button
        poseEstimator.reset();

        // Write whatever motion commands you want here
        chassis.setTwist(DRIVE_SPEED * MM_TO_CM, TURN_SPEED);
        lastStateChange = millis();
      } else {
        chassis.idle();
      }
      break;

    case SLOW_CIRCLE:
      if (millis() - lastStateChange >= DRIVE_TIME) {
        state = IDLE;
        chassis.idle();
      }
      break;
  }
#endif

  // Always update the pose estimator!
  poseEstimator.update(chassis.getLeftWheelSpeed() * MM_TO_CM,
                       chassis.getRightWheelSpeed() * MM_TO_CM);

  // Print the pose data (for the lab plots)
  poseEstimator.printPose();
}