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
#include <Rangefinder.h>
#include <Romi32U4.h>
#include <openmv.h>

#include "IRSensor.h"
#include "LineSensor.h"
#include "MQTT.h"
#include "MazeSolver.h"

// Range finder pins
#define TRIG_PIN 12
#define ECHO_PIN 11
#define LEFT_LINE_PIN A0
#define RIGHT_LINE_PIN A2
#define LEFT_IR_PIN A4
#define RIGHT_IR_PIN A3

// Assuming necessary libraries for MQTT, networking, and sensor input are
// included
#define WALL_IR_DIS_THRESHOLD 20.0    // mm
#define WALL_SONAR_DIS_THRESHOLD 24.0 // cm
#define THETA_THESHOLD 10             // deg
#define LINE_THRESHOLD 100            // 0-1023
#define SEEKING_FWD_SPEED 12          // in/s
#define SEEKING_TURN_SPEED 100        // deg/s
#define TURN_SPEED 150                // deg/s
#define SEARCH_EFFORT 80              // Motor effort

#define IN_CH 2.54 // Inch to Centimeter conversion
#define LINE_P 0.1 // Line P value

class Zombie {
public:
  // Function to initialize the zombie Romi
  /**
   * @brief init the zombie Romi
   *
   */
  void init();

  /**
   * @brief runs zombie Romi
   *
   */
  void run();

  /**
   * @brief follow line function
   *
   */
  void followLine();

  /**
   * @brief Check if on target
   *
   * @return true
   * @return false
   */
  bool onTarget();

  /**
   * @brief Stops the Romi
   *
   */
  void stop();

  /**
   * @brief Gets sensor readings
   *
   */
  void printAllSensor();

  /**
   * @brief Get the Path object
   *
   */
  void getPath(); // Make this private

private:
  // Robot state
  enum RobotState { IDLE, SEEKING, STOP, DEBUG_PATHFIND };
  RobotState state = IDLE;

  typedef enum { UP = 0, DOWN = 180, LEFT = -90, RIGHT = 90 } headingDirection;
  headingDirection currentHeading = UP;

  uint8_t lastClosestIntersectionIndex_X = 5; // target node
  uint8_t lastClosestIntersectionIndex_Y = 2; // target node

  int currentIntersection_X = 0; // current node
  int currentIntersection_Y = 0; //  current node

  Node path[MAX_HEIGHT * MAX_WIDTH]; // path to target node
  int pathLength = 0;                // length of path to target node
  bool pathFound = false;            // flag for path found
  bool pathUpdated = false;          // flag for path updated
  int currentPathIndex = 0;          // current index in path

  int nextNode_X = 0; // next node
  int nextNode_Y = 0; // next node

  Romi32U4ButtonA buttonA;
  LineSensor lineSensor = LineSensor(LEFT_LINE_PIN, RIGHT_LINE_PIN);
  IRSensor irSensorLeft;
  IRSensor irSensorRight;

  Maze maze;
  MazeSolver mazeSolver;
  Node survivorNode = Node(5, 2);
  MQTT mqtt;
  OpenMV camera;

  float currentTheta = 0;

  bool isOnLastKnownPosition = false;

  uint8_t intersectionCount = 0;

  // Private member functions

  // get Sensor Values
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  float getLeftLineValue();

  float getRightLineValue();

  // Check if on intersection
  bool onIntersection();

  // Update current intersection index and state of robot
  void recordIntersection();

  // Gets the next heading direction for the robot
  headingDirection getTurnDirection();

  // Update the current heading direction
  void updateIntersectionIndex();

  void printMaze();
};

#endif