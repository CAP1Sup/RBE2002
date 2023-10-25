#include <Romi32U4.h>
#include "chassis.h"

float RomiChassis::getLeftSpeed(void)
{
  // !!! ATTENTION !!!
  // Assignment 1
  uint32_t currentTime = millis();
  int16_t count_left = encoders.getCountsLeft();
  float speed = (C_WHEEL / N_WHEEL) * ((count_left - prev_count_left) / ((currentTime - previousLeftTime) / 1000));
  prev_count_left = count_left;
  previousLeftTime = currentTime;
  return speed;
}

float RomiChassis::getRightSpeed(void)
{
  // !!! ATTENTION !!!
  // Assignment 1
  uint32_t currentTime = millis();
  int16_t count_right = encoders.getCountsRight();
  float speed = (C_WHEEL / N_WHEEL) * ((count_right - prev_count_right) / ((currentTime - previousRightTime) / 1000));
  prev_count_right = count_right;
  previousRightTime = currentTime;
  return speed;
}

float RomiChassis::getLeftEffort(void)
{
  return OCR1B;
}

float RomiChassis::getRightEffort(void)
{
  return OCR1A;
}

void RomiChassis::setDriveEffort(int left, int right)
{
  motors.setEfforts(left, right);
}

void RomiChassis::updateMotorEffortPI(int target_speed_left, int target_speed_right)
{
  // !!! ATTENTION !!!
  // Assignment 2
  {
    float error_Left = target_speed_left - getLeftSpeed();
    float error_Right = target_speed_right - getRightSpeed();

    cum_E_left += error_Left;
    cum_E_right += error_Right;

    motors.setEfforts(Kp * error_Left + Ki * cum_E_left, Kp * error_Right + Ki * cum_E_right);
    printToSerial(getLeftSpeed(), getLeftEffort(), getRightSpeed(), getRightEffort());
  }
}

void RomiChassis::printToSerial(float a, float b, float c, float d)
{
  // !!! ATTENTION !!!
  // USE this function for assignment 3!
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.print(c);
  Serial.print(",");
  Serial.print(d);
  Serial.println();
}

void RomiChassis::updateMotorPID(void)
{
  uint32_t now = millis();
  if (now - lastPIDUpdate >= PID_UPDATE_INTERVAL)
  {
    updateMotorEffortPI(target_left, target_right);
    lastPIDUpdate = now;
  }
}

void RomiChassis::beginDriving(float left, float right, uint32_t duration)
{
  target_left = left;
  target_right = right;
  start_time = millis();
  lastPIDUpdate = start_time;
  end_time = start_time + duration; // fails at rollover
  cum_E_left = 0;
  cum_E_right = 0;
}

bool RomiChassis::isDriveComplete(void)
{
  return millis() >= end_time;
}

void RomiChassis::stop(void)
{
  target_left = target_right = 0;
  motors.setEfforts(0, 0);
}