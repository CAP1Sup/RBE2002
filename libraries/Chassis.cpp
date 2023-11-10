#include "Chassis.h"

#include <Encoders.h> // For COUNTS_PER_REV and WHEEL_CIRCUM
#include <Romi32U4.h>

int16_t Chassis::getLeftEffort() { return lastLeftEffort; }

int16_t Chassis::getRightEffort() { return lastRightEffort; }

void Chassis::resetEncoderCount() { reset(); }

void Chassis::setDriveEffort(int16_t left, int16_t right) {
  lastLeftEffort = left;
  lastRightEffort = right;
  motors.setEfforts(left, right);
}

void Chassis::updateMotorEffort(uint32_t deltaMs) {
  if (trackMove) {
    // Check if the move is complete
    if (isMotionComplete()) {
      // Stop the motors
      stop();
      return;
    }
  }
  float leftError = targetSpeedLeft - getLeftSpeed();
  float rightError = targetSpeedRight - getRightSpeed();
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

void Chassis::resetDrivePID() {
  // Use to prevent jerk when starting
  leftPID.reset();
  rightPID.reset();
  reset();

  // Set the target speeds to 0
  // Forces caller to use setTargetSpeeds() or drive()
  targetSpeedLeft = 0;
  targetSpeedRight = 0;

  // Update the last PID time
  lastPIDUpdate = millis();

  // Turn off the motors
  // Forces caller to call updateMotorPID() or updateMotorEffort()
  motors.setEfforts(0, 0);

  // Disable move tracking
  trackMove = false;
}

void Chassis::drive(float speed, float distance) {
  // Save the target speeds
  // Ignore the sign of the speed and use the distance's sign
  speed = abs(speed) * sgn(distance);
  setTargetSpeeds(speed, speed);

  // Calculate the new desired counts
  setDesiredLeftDist(distance);
  setDesiredRightDist(distance);

  // Enable move tracking
  trackMove = true;
}

void Chassis::setTargetSpeeds(float leftSpeed, float rightSpeed) {
  targetSpeedLeft = leftSpeed;
  targetSpeedRight = rightSpeed;

  // Disable move tracking (by default, can be re-enabled by calling function)
  trackMove = false;
}

void Chassis::pointTurn(float angle, float speed) {
  // Convert from deg/s to mm/s
  float turnSpeed = speed / 360.0f * BASE_DIA * PI;

  // If the angle is negative, turn right
  if (angle < 0) {
    turnSpeed = -turnSpeed;
  }

  // Calculate the desired encoder counts
  int16_t counts = (angle / 360.0f) * COUNTS_PER_REV * (BASE_DIA / 2.0f) /
                   (WHEEL_DIA / 2.0f);

  // Set the desired counts
  setDesiredLeftCount(counts);
  setDesiredRightCount(-counts);

  trackMove = true;

  // Begin driving
  setTargetSpeeds(turnSpeed, -turnSpeed);
}

void Chassis::swingTurn(float angle, float speed, TURN_DIR pivotWheel) {
  // Convert from deg/s to mm/s
  float turnSpeed = speed / 360.0f * 2.0f * BASE_DIA * PI;

  // If the angle is negative, turn right
  if (angle < 0) {
    turnSpeed = -turnSpeed;
  }

  // Calculate the desired encoder counts
  int16_t counts =
      (angle / 360.0f) * COUNTS_PER_REV * BASE_DIA / (WHEEL_DIA / 2.0f);

  // Set the desired counts
  if (pivotWheel == RIGHT) {
    setDesiredLeftCount(counts);
    setDesiredRightCount(0);
    setTargetSpeeds(turnSpeed, 0);
  } else {
    setDesiredRightCount(counts);
    setDesiredLeftCount(0);
    setTargetSpeeds(0, turnSpeed);
  }
  trackMove = true;
}

bool Chassis::isMotionComplete() { return isLeftAtPos() && isRightAtPos(); }

void Chassis::stop() { setDriveEffort(0, 0); }