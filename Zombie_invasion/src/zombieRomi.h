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

class ZombieRomi {
public:
  // Constructor with sensor, controller, and chassis pointers
  ZombieRomi();

  // Function to receive target coordinates from MQTT server
  void receiveTargetCoordinates(float x, float y);

  // Function to update the zombie's current target
  void updateTarget();

  // Function to follow a line
  void followLine();

  // Function to detect survivor's position (left, right, stationary)
  void detectSurvivorPosition();

  // Function to pursue the survivor
  void pursueSurvivor();

  // Function to move to the last known survivor's location
  void moveToLastKnownLocation();

private:
  // Private member variables
  float lastKnownX;
  float lastKnownY;

  typedef enum { STRAIGHT = 0, LEFT = -1, RIGHT = 1 } survivorPosition;

  // Private member objects
  IRSensor irSensorLeft;
  IRSensor irSensorRight;
  SonarSensor sonarSensor;
  Chassis chassis;
  LineSensor lineSensor;

  // Private member functions
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  bool onIntersection();
};
