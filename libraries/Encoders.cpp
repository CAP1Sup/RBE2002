#include "Encoders.h"

#include <Romi32U4.h>

int16_t Encoders::getLeftCount() { return encoders.getCountsLeft(); }

float Encoders::getLeftSpeed() {
  uint32_t time = millis();
  int16_t currentCount = encoders.getCountsLeft();
  float speed = (((WHEEL_CIRCUM / COUNTS_PER_REV) *
                  (float)(currentCount - prevLeftCount)) *
                 1000.0) /
                (time - prevLeftTime);
  prevLeftCount = currentCount;
  prevLeftTime = time;
  return speed;
}

void Encoders::setDesiredLeftCount(int16_t count) { desiredLeftCount = count; }

void Encoders::setDesiredLeftDist(float distance) {
  Serial.print("Setting desired left distance to: ");
  Serial.println(distance);
  Serial.print("Wheel circumference: ");
  Serial.println(WHEEL_CIRCUM);
  Serial.print("Counts per rev: ");
  Serial.print(COUNTS_PER_REV);
  desiredLeftCount = (int32_t)((distance / WHEEL_CIRCUM) * COUNTS_PER_REV);
  Serial.print("Desired left count: ");
  Serial.println(desiredLeftCount);
}

int16_t Encoders::getDesiredLeftCount() { return desiredLeftCount; }

bool Encoders::isLeftAtPos() {
  int32_t error = desiredLeftCount - encoders.getCountsLeft();
  error = error < 0 ? -error : error;

  Serial.print("Error: ");
  Serial.println(desiredLeftCount);
  // Serial.println(abs(desiredLeftCount - encoders.getCountsLeft()));

  return abs(desiredLeftCount - encoders.getCountsLeft()) < POS_COUNT_TOL;
}

int16_t Encoders::getRightCount() { return encoders.getCountsRight(); }

float Encoders::getRightSpeed() {
  uint32_t time = millis();
  int16_t currentCount = encoders.getCountsRight();
  float speed = (((WHEEL_CIRCUM / COUNTS_PER_REV) *
                  (float)(currentCount - prevRightCount)) *
                 1000.0) /
                (time - prevRightTime);
  prevRightCount = currentCount;
  prevRightTime = time;
  return speed;
}

void Encoders::setDesiredRightCount(int16_t count) {
  desiredRightCount = count;
}

void Encoders::setDesiredRightDist(float distance) {
  desiredRightCount = (int16_t)(distance / WHEEL_CIRCUM * COUNTS_PER_REV);
}

bool Encoders::isRightAtPos() {
  return abs(desiredRightCount - encoders.getCountsRight()) < POS_COUNT_TOL;
}

void Encoders::printSpeeds() {
  Serial.print("Velocity of left wheel: ");
  Serial.print(getLeftSpeed());
  Serial.print('\t');
  Serial.print("Velocity of right wheel: ");
  Serial.println(getRightSpeed());
}

void Encoders::reset() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}