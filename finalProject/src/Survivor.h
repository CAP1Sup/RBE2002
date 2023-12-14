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
#include "PID.h"
#include "SonarSensor.h"

// Op settings
// Wall following constants
#define DIST_PID_UPDATE_INTERVAL 50  // ms
#define TARGET_DISTANCE 22.5f        // mm
#define NO_WALL_THRESHOLD 45.0f      // mm
#define NO_WALL_FORWARD_DIST 400.0f  // mm
#define MAX_PID_OUTPUT 50.0f         // mm/s
#define DRIVE_SPEED 150.0f           // mm/s
#define CHASED_TIME_INTERVAL 5000    // ms
#define RIGHT_TURN_ANGLE -80.0f      // deg
#define LEFT_TURN_ANGLE 80.0f        // deg
#define TURN_SPEED 135.0f            // deg/s
#define WALL_DISTANCE 125.0f         // mm

// Pins
#define IR_PIN A0
#define TRIG_PIN 12
#define ECHO_PIN 3

class Survivor {
 public:
  void init();
  void run();

 private:
  // Robot state
  enum RobotState { IDLE, WANDERING, CHASED };
  RobotState state = IDLE;

  IRSensor IR;
  Romi32U4ButtonA buttonA;
  PID wallFollowPID = PID(4.0f, 0.0f, 0.0f);
  PID chasedWallFollowPID = PID(3.0f, 1.0f, 0.0f);
  uint32_t lastPIDUpdate = 0;
  uint32_t chasedTime = 0;
  uint32_t updateTime = 0;
  bool turning = false;
};
#endif