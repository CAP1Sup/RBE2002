#include "chassis.h"

#include <Romi32U4.h>

/**
 * @brief Returns the current speed of the left wheel in mm/s
 *
 * @return float speed in mm/s
 */
float RomiChassis::getLeftSpeed() {
  // !!! ATTENTION !!!
  // Assignment 1
  uint32_t time = millis();
  int16_t count = encoders.getCountsLeft();
  float speed = (((WHEEL_CIRCUM / COUNTS_PER_REV) * (float)(count - prevLeftCount)) * 1000.0) /
                (time - prevLeftTime);
  prevLeftCount = count;
  prevLeftTime = time;
  return speed;
}

/**
 * @brief Returns the current speed of the right wheel in mm/s
 *
 * @return float speed in mm/s
 */
float RomiChassis::getRightSpeed() {
  // !!! ATTENTION !!!
  // Assignment 1
  uint32_t time = millis();
  int16_t count = encoders.getCountsRight();
  float speed = ((WHEEL_CIRCUM / COUNTS_PER_REV) * (count - prevRightCount)) /
                (float)(time - prevRightTime) * 1000.0;
  prevRightCount = count;
  prevRightTime = time;
  return speed;
}

/**
 * @brief Returns the current effort of the left wheel
 *
 * @return int16_t effort
 */
int16_t RomiChassis::getLeftEffort() { return lastLeftEffort; }

/**
 * @brief Returns the current effort of the right wheel
 *
 * @return int16_t effort
 */
int16_t RomiChassis::getRightEffort() { return lastRightEffort; }

/**
 * @brief Sets the effort of the left and right motors
 *
 * @param left Left motor effort (-300 to 300)
 * @param right Right motor effort
 */
void RomiChassis::setDriveEffort(int left, int right) {
  lastLeftEffort = left;
  lastRightEffort = right;
  motors.setEfforts(left, right);
 // printToSerial(getLeftSpeed(), getLeftEffort(), getRightSpeed(), getRightEffort());
}

/**
 * @brief Updates the motor effort using a PI controller
 *
 * @param deltaMs Time since the last update in milliseconds
 */
void RomiChassis::updateMotorEffortPI(uint32_t deltaMs) {
  // !!! ATTENTION !!!
  // Assignment 2
  float leftSpeed = getLeftSpeed();
  float rightSpeed = getRightSpeed();
  float leftError = targetSpeedLeft - leftSpeed;
  float rightError = targetSpeedRight - rightSpeed;

  cumErrorLeft += leftError * deltaMs / 1000;
  cumErrorRight += rightError * deltaMs / 1000;

  setDriveEffort(Kp * leftError + Ki * cumErrorLeft,
                 Kp * rightError + Ki * cumErrorRight);
  printToSerial(leftSpeed, getLeftEffort(), rightSpeed, getRightEffort());
}

/**
 * @brief A helper function for printing comma separated values to serial
 *
 * @param a First value
 * @param b Second value
 * @param c Third value
 * @param d Fourth value
 */
void RomiChassis::printToSerial(float a, float b, float c, float d) {
  // !!! ATTENTION !!!
  // Use this function for assignment 3!
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.print(c);
  Serial.print(",");
  Serial.print(d);
  Serial.println();
}

/**
 * @brief Updates the motor effort using a PI controller
 *
 */
void RomiChassis::updateMotorPID() {
  uint32_t now = millis();
  if (now - lastPIDUpdate >= PID_UPDATE_INTERVAL) {
    updateMotorEffortPI(now - lastPIDUpdate);
    lastPIDUpdate = now;
  }
}

/**
 * @brief Begins driving the robot at the specified speeds for the specified
 *
 * @param leftSpeed Target speed for the left wheel in mm/s
 * @param rightSpeed Target speed for the right wheel in mm/s
 * @param duration How long to drive for in milliseconds
 */
void RomiChassis::beginDriving(float leftSpeed, float rightSpeed,
                               uint32_t duration) {
  targetSpeedLeft = leftSpeed;
  targetSpeedRight = rightSpeed;
  uint32_t time = millis();
  lastPIDUpdate = time;
  endTime = time + duration;  // fails at rollover

  // Use to prevent jerk when starting
  cumErrorLeft = 0;
  cumErrorRight = 0;
  prevLeftCount = encoders.getCountsLeft();
  prevRightCount = encoders.getCountsRight();
}

/**
 * @brief Returns true if the robot has finished driving
 *
 */
bool RomiChassis::isDriveComplete() { return millis() >= endTime; }

/**
 * @brief Stops the robot
 *
 */
void RomiChassis::stop() { setDriveEffort(0, 0); }