#include <Arduino.h>
#include <Romi32U4.h>

#include "chassis.h"

RomiChassis chassis;

uint8_t driveTime = 2;  // seconds

enum ROBOT_STATE { ROBOT_IDLE, ROBOT_DRIVING };
ROBOT_STATE state = ROBOT_IDLE;

Romi32U4ButtonA buttonA;

void setup() { Serial.begin(115200); }

void loop() {
  switch (state) {
    case ROBOT_IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.beginDriving(
            10, 10,
            driveTime * 1000);  // contains your program that the robot
                                // executes when pushbutton A is pressed
        state = ROBOT_DRIVING;
      }
      break;

    case ROBOT_DRIVING:
      chassis.updateMotorPID();
      if (chassis.isDriveComplete()) {
        chassis.stop();
        state = ROBOT_IDLE;
        driveTime += 2;
      }
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.stop();
        state = ROBOT_IDLE;
      }
  }
}