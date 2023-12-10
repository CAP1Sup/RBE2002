#pragma once

#include "main.h"

// Only compile this class if the ZOMBIE macro is defined
#ifdef ZOMBIE

#include <Arduino.h>
#include <Chassis.h>
#include <IRdecoder.h>
#include <PIDcontroller.h>
#include <Rangefinder.h>
#include <Romi32U4.h>

#include "IRSensor.h"
#include "LineSensor.h"
#include "SonarSensor.h"

// Range finder pins
#define TRIG_PIN 8
#define ECHO_PIN 7
#define LEFT_LINE_PIN A0
#define RIGHT_LINE_PIN A1
#define LEFT_IR_PIN A4
#define RIGHT_IR_PIN A3

// Assuming necessary libraries for MQTT, networking, and sensor input are
// included
#define WALL_DIS_THRESHOLD 30.0  // mm
#define SEEKING_FWD_SPEED 25     // in/s
#define TURN_SPEED 150           // deg/s
#define SEARCH_EFFORT 80         // Motor effort
#define IN_CH 2.54               // Inch to Centimeter conversion
#define LINE_P 0.2               // Line P value

class Zombie {
 public:
  // Constructor with sensor, controller, and chassis pointers
  Zombie();

  // Function to run the zombie Romi
  void run();

  // Function to receive target coordinates from MQTT server
  void receiveTargetCoordinates(float x, float y);

  // Function to follow a line
  void followLine();

  // Function to detect survivor's position (left, right, stationary)
  void detectSurvivorPosition();

  // Function to pursue the survivor
  void pursueSurvivor();

  // Function to move to the last known survivor's location
  void moveToLastKnownLocation();

  bool getOnLastKnownPosition();

  int getIntersectionCount();

  void stop();

  bool survivorFound();

  bool survivorInfected();

  void printAllSensor();

 private:
  // Robot state
  enum RobotState { IDLE, SEEKING, CHASING, STOP };
  RobotState state = IDLE;

  // Private member variables
  float lastKnownX;
  float lastKnownY;
  // Private member objects
  float currentX;
  float currentY;

  Romi32U4ButtonA buttonA;

  SonarSensor sonar;
  LineSensor lineSensor = LineSensor(LEFT_LINE_PIN, RIGHT_LINE_PIN);
  IRSensor irSensorLeft;
  IRSensor irSensorRight;

  float currentTheta;

  bool isOnLastKnownPosition = false;

  int intersectionCount = 0;

  typedef enum { STRAIGHT, LEFT, RIGHT } turnDirection;

  // Private member functions
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  float getLeftLineValue();

  float getRightLineValue();

  bool onIntersection();

  turnDirection getTurnDirection();
};

#endif