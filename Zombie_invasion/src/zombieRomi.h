#pragma once
#include "../../libraries/IRSensor.h"
#include "../../libraries/LineSensor.h"
#include "../../libraries/SonarSensor.h"
#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>
#include <Wire.h>

// Assuming necessary libraries for MQTT, networking, and sensor input are
// included
#define WALL_DIS_THRESHOLD 30.0 // mm
#define SEEKING_FWD_SPEED 25    // in/s
#define TURN_SPEED 150          // deg/s
#define SEARCH_EFFORT 80        // Motor effort
#define IN_CH 2.54              // Inch to Centimeter conversion
#define LINE_P 0.2              // Line P value

class zombieRomi {
public:
  // Constructor with sensor, controller, and chassis pointers
  zombieRomi();

  /* zombieRomi(Chassis *chassis, LineSensor *lineSensor, IRSensor
   *irSensorLeft, IRSensor *irSensorRight, SonarSensor *sonarSensor); */

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

private:
  // Private member variables
  float lastKnownX;
  float lastKnownY;
  // Private member objects
  Chassis chassis;
  LineSensor lineSensor;
  IRSensor irSensorLeft;
  IRSensor irSensorRight;
  SonarSensor sonarSensor;

  /* Chassis *chassis;
  LineSensor *lineSensor;
  IRSensor *irSensorLeft;
  IRSensor *irSensorRight;
  SonarSensor *sonarSensor; */

  // Private member variables
  float currentX;
  float currentY;
  float currentTheta;

  bool isOnLastKnownPosition = false;

  int intersectionCount = 0;

  typedef enum { STRAIGHT, LEFT, RIGHT } turnDirection;

  // Private member functions
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  bool onIntersection();

  turnDirection getTurnDirection();
};
