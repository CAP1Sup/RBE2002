#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>

Chassis chassis;

uint8_t driveTime = 2;  // seconds
uint32_t startDriveTime = 0;

enum ROBOT_STATE { ROBOT_IDLE, ROBOT_DRIVING };
ROBOT_STATE state = ROBOT_IDLE;

Romi32U4ButtonA buttonA;

void setup() { Serial.begin(115200); }

void loop() {
  switch (state) {
    case ROBOT_IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.resetDrivePID();
        chassis.setTargetSpeeds(10, 10);
        startDriveTime = millis();
        state = ROBOT_DRIVING;
      }
      break;

    case ROBOT_DRIVING:
      chassis.updateMotorPID();
      if (millis() - startDriveTime >= driveTime * 1000) {
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