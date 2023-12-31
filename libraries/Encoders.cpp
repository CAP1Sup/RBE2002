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
  desiredLeftCount = ((distance / 219.8) * 1440);
}

int16_t Encoders::getDesiredLeftCount() { return desiredLeftCount; }

bool Encoders::isLeftAtPos() {
  int32_t error = desiredLeftCount - encoders.getCountsLeft();
  error = error < 0 ? -error : error;


  return abs((desiredLeftCount)-abs(encoders.getCountsLeft())) < 10;
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
  desiredRightCount = (distance / 219.8 * 1440);
}

bool Encoders::isRightAtPos() {
  // Serial.println(encoders.getCountsRight());
  // Serial.println(".....");
  // Serial.println(abs((desiredRightCount) - (encoders.getCountsRight())));
  return abs((desiredRightCount) - (encoders.getCountsRight())) < POS_COUNT_TOL;
}

void Encoders::printSpeeds() {
  Serial.print("Velocity of left wheel: ");
  Serial.print(getLeftSpeed());
  Serial.print('\t');
  Serial.print("Velocity of right wheel: ");
  Serial.println(getRightSpeed());
}

void Encoders::resetEncoderCounts() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}