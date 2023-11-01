#include "Encoders.h"

#include <Romi32U4.h>

float Encoders::getLeftSpeed() {
  uint32_t time = millis();
  float speed = (((WHEEL_CIRCUM / COUNTS_PER_REV) *
                  (float)(encoders.getCountsAndResetLeft())) *
                 1000.0) /
                (time - prevLeftTime);
  prevLeftTime = time;
  return speed;
}

float Encoders::getRightSpeed() {
  uint32_t time = millis();
  float speed = (((WHEEL_CIRCUM / COUNTS_PER_REV) *
                  (float)(encoders.getCountsAndResetRight())) *
                 1000.0) /
                (time - prevLeftTime);
  prevRightTime = time;
  return speed;
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