#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>

Chassis chassis;

uint8_t driveTime = 2;  // seconds
uint32_t startDriveTime = 0;

enum ROBOT_STATE { IDLE, DRIVING };
ROBOT_STATE state = IDLE;

Romi32U4ButtonA buttonA;

void setup() { Serial.begin(9600); }

void loop() {
  switch (state) {
    case IDLE:
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.resetDrivePID();
        chassis.setTargetSpeeds(10, 10);
        startDriveTime = millis();
        state = DRIVING;
      }
      break;

    case DRIVING:
      chassis.updateMotorPID();
      if (millis() - startDriveTime >= driveTime * 1000) {
        chassis.stop();
        state = IDLE;
        driveTime += 2;
      }
      if (buttonA.getSingleDebouncedRelease()) {
        chassis.stop();
        state = IDLE;
      }
  }
}