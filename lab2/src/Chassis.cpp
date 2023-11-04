#include "Chassis.h"

#include <Romi32U4.h>

int16_t Chassis::getLeftEffort() { return lastLeftEffort; }

int16_t Chassis::getRightEffort() { return lastRightEffort; }

void Chassis::setDriveEffort(int16_t left, int16_t right) {
  lastLeftEffort = left;
  lastRightEffort = right;
  motors.setEfforts(left, right);
}

void Chassis::updateMotorEffort(uint32_t deltaMs) {
  float leftError = targetSpeedLeft - encoders.getLeftSpeed();
  float rightError = targetSpeedRight - encoders.getRightSpeed();
  setDriveEffort(leftPID.calculate(leftError, deltaMs / 1000.0f),
                 rightPID.calculate(rightError, deltaMs / 1000.0f));
}

void Chassis::updateMotorPID() {
  uint32_t now = millis();
  if (now - lastPIDUpdate >= PID_UPDATE_INTERVAL) {
    updateMotorEffort(now - lastPIDUpdate);
    lastPIDUpdate = now;
  }
}

void Chassis::printToSerial(float a, float b, float c, float d) {
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.print(c);
  Serial.print(",");
  Serial.print(d);
  Serial.println();
}

void Chassis::beginDriving() { beginDriving(0, 0, 0); }

void Chassis::beginDriving(float leftSpeed, float rightSpeed,
                           uint32_t duration) {
  targetSpeedLeft = leftSpeed;
  targetSpeedRight = rightSpeed;
  uint32_t time = millis();
  lastPIDUpdate = time;
  endTime = time + duration; // fails at rollover

  // Use to prevent jerk when starting
  leftPID.reset();
  rightPID.reset();
  encoders.reset();

  // Turn off the motors (just in case)
  motors.setEfforts(0, 0);
}

void Chassis::setTargetSpeeds(float leftSpeed, float rightSpeed) {
  targetSpeedLeft = leftSpeed;
  targetSpeedRight = rightSpeed;
}

bool Chassis::isDriveComplete() { return millis() >= endTime; }

void Chassis::stop() { setDriveEffort(0, 0); }