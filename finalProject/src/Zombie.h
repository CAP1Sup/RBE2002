/**
 * @file Zombie.h
 * @author Kang Zhang (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "main.h"

// Only compile this class if the ZOMBIE macro is defined
#ifdef ZOMBIE

#include <Chassis.h>
#include <PIDcontroller.h>
#include <Rangefinder.h>
#include <Romi32U4.h>

#include "IRSensor.h"
#include "LineSensor.h"
#include "MQTT.h"
#include "MazeSolver.h"
#include "SonarSensor.h"

// Range finder pins
#define TRIG_PIN 12
#define ECHO_PIN 11
#define LEFT_LINE_PIN A0
#define RIGHT_LINE_PIN A2
#define LEFT_IR_PIN A4
#define RIGHT_IR_PIN A3

// Assuming necessary libraries for MQTT, networking, and sensor input are
// included
#define WALL_IR_DIS_THRESHOLD 30.0     // mm
#define WALL_SONAR_DIS_THRESHOLD 20.0  // cm
#define THETA_THESHOLD 10              // deg
#define LINE_THRESHOLD 50              // 0-1023
#define SEEKING_FWD_SPEED 12           // in/s
#define SEEKING_TURN_SPEED 100         // deg/s
#define TURN_SPEED 150                 // deg/s
#define SEARCH_EFFORT 80               // Motor effort

#define IN_CH 2.54  // Inch to Centimeter conversion
#define LINE_P 0.1  // Line P value

class Zombie {
 public:
  // Function to initialize the zombie Romi
  void init();

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

  void stop();

  bool survivorFound();

  bool survivorInfected();

  void printAllSensor();

  void readMQTT();

  void getPath();  // Make this private

 private:
  // Robot state
  enum RobotState { IDLE, SEEKING, CHASING, STOP, DEBUG };
  RobotState state = DEBUG;

  // Heading direction
  typedef enum { UP = 90, DOWN = 270, LEFT = 180, RIGHT = 0 } headingDirection;
  headingDirection currentHeading = UP;

  // Private member variables
  float lastKnownX = 1.0;
  float lastKnownY = 1.0;
  // Private member objects
  float currentX = 0.0;
  float currentY = 0.0;

  uint8_t lastClosestIntersectionIndex_X = 5;
  uint8_t lastClosestIntersectionIndex_Y = 2;

  int currentIntersection_X = 0;
  int currentIntersection_Y = 0;

  Node path[MAX_HEIGHT * MAX_WIDTH];
  int pathLength = 0;
  bool pathFound = false;
  bool pathUpdated = false;
  int currentPathIndex = 0;

  // float intersectionPoints[3][6] = {
  //     {(0, 0), (22.5, 0), (52.9, 0), (88.2, 0), 0, 0},
  //     {(1, 56.1), (22.5, 55.6), (52.9, 56.1), (88.2, 56.1), (1, 56.1),
  //      (0, 56.1)},
  //     {(0, 0), (22.5, 0), (52.9, 0), (88.2, 0), (0, 0), (0, 0)}};

  Romi32U4ButtonA buttonA;
  SonarSensor sonar;
  LineSensor lineSensor = LineSensor(LEFT_LINE_PIN, RIGHT_LINE_PIN);
  IRSensor irSensorLeft;
  IRSensor irSensorRight;

  Maze maze;
  MazeSolver mazeSolver;
  MQTT mqtt;

  float currentTheta = 0;

  bool isOnLastKnownPosition = false;

  uint8_t intersectionCount = 0;

  // Private member functions
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  float getLeftLineValue();

  float getRightLineValue();

  bool onIntersection();

  void recordIntersection();

  headingDirection getTurnDirection();

  uint8_t getIntersectionCount();

  void followPath();

  void closestIntersection();

  void updateIntersectionIndex();

  void turnOnIntersection(headingDirection nextHeading);

  void printMaze();
};

#endif