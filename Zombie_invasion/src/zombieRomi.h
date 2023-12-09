#pragma once
#include "include.h"

// Assuming necessary libraries for MQTT, networking, and sensor input are
// included

class zombieRomi {
public:
  // Constructor with sensor, controller, and chassis pointers
  zombieRomi(IRSensor *irl, IRSensor *irr, SonarSensor *sonar,
             LineSensor *lineSensor, Chassis *chassis);

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
  IRSensor *irSensorLeft;
  IRSensor *irSensorRight;
  SonarSensor *sonarSensor;
  Chassis *chassis;
  LineSensor *lineSensor;

  // Private member functions
  float getSonarDistance();

  float getIRLeftDistance();

  float getIRRightDistance();

  bool onIntersection();
};
