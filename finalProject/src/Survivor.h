#pragma once

#include "main.h"

#ifdef SURVIVOR

#include <Arduino.h>
#include <Chassis.h>
#include <IRdecoder.h>
#include <PIDcontroller.h>
#include <Rangefinder.h>
#include <Romi32U4.h>

#include "IRSensor.h"
#include "LineSensor.h"
#include "SonarSensor.h"

#define IR_PIN A0
#define TRIG_PIN 8
#define ECHO_PIN 7

class Survivor {
 public:
  Survivor();
  void run();

 private:
  // Wall following constants
  const float DIST_PID_UPDATE_INTERVAL = 50;  // ms
  const float TARGET_DISTANCE = 30.0f;        // mm
  const float MAX_PID_OUTPUT = 50.0f;         // mm/s
  const float DRIVE_SPEED = 75.0f;            // mm/s
  const float CHASED_TIME_INTERVAL = 5000;    // ms

  // Robot state
  enum RobotState { IDLE, WANDERING, CHASED };
  RobotState robot_state = IDLE;

  SonarSensor sonar;
  IRSensor IR;
  Romi32U4ButtonA buttonA;
  PIDController wallFollowPID = PIDController(4.0f, 0.0f, 0.0f);
  PIDController chasedWallFollowPID = PIDController(3.0f, 1.0f, 0.0f);
  uint32_t lastPIDUpdate = 0;
  uint32_t chasedTime = 0;
  uint32_t updateTime = 0;
};
#endif