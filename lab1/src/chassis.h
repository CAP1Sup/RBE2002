#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>

#define COUNTS_PER_REV 1440  // how many counts equate to one wheel rotation?
#define WHEEL_DIA 70         // what is the radius of a Romi wheel in (mm)
#define WHEEL_CIRCUM (float)PI* WHEEL_DIA  // circumference of wheel (mm)
#define PID_UPDATE_INTERVAL 50  // how often do we update the motor effort? (ms)

class RomiChassis {
 private:
  // Romi specific constants:
  //  !!! ATTENTION !!!
  // declare variables for PI controller
  float targetSpeedLeft = 0;
  float targetSpeedRight = 0;
  float Kp = 1.35f;
  float Ki = 12.0;
  float cumErrorLeft = 0;  // accumulated errors
  float cumErrorRight = 0;

  // encoder and motor objects
  Romi32U4Encoders encoders;
  Romi32U4Motors motors;

  // declare variables for for keeping track of counts and conversion to
  // velocities
  uint32_t endTime = 0;
  uint32_t lastPIDUpdate = 0;
  int prevLeftCount = 0;
  int prevRightCount = 0;
  uint32_t prevLeftTime = 0;
  uint32_t prevRightTime = 0;
  int16_t lastLeftEffort = 0;
  int16_t lastRightEffort = 0;

 public:
  float getLeftSpeed();
  float getRightSpeed();
  int16_t getLeftEffort();
  int16_t getRightEffort();

  void setDriveEffort(int a, int b);
  void updateMotorEffortPI(uint32_t deltaMs);

  void updateMotorPID();
  void printToSerial(float a, float b, float c, float d);

  void beginDriving(float, float, uint32_t);
  bool isDriveComplete();
  void stop();
};

extern RomiChassis chassis;

#endif