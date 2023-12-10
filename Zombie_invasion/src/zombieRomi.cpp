#include "zombieRomi.h"

// Constructor implementation
zombieRomi::zombieRomi() {
  // Constructor body
  chassis.init();
  irSensorLeft.init(A0);
  irSensorRight.init(A1);
  sonarSensor.init();

  // Initialize member variables, set up sensors, networking, etc.
}
/*
zombieRomi::zombieRomi(Chassis *chassis, LineSensor *lineSensor,
                       IRSensor *irSensorLeft, IRSensor *irSensorRight,
                       SonarSensor *sonarSensor) {
  // Constructor body
  this.chassis = chassis;
  this.lineSensor = lineSensor;
  this.irSensorLeft = irSensorLeft;
  this.irSensorRight = irSensorRight;
  this.sonarSensor = sonarSensor;

  // Initialize member variables, set up sensors, networking, etc.
} */

// Receive target coordinates from MQTT server
void zombieRomi::receiveTargetCoordinates(float x, float y) {
  // Implementation for receiving coordinates
  lastKnownX = x;
  lastKnownY = y;
  isOnLastKnownPosition = false;
}

// Follow a line
void zombieRomi::followLine() {
  // Calculate the difference between readings from the two line sensors
  int difference =
      lineSensor.getLeftLineValue() - lineSensor.getRightLineValue();
  chassis.setTwist(SEEKING_FWD_SPEED * IN_CH, LINE_P * difference);
}

// Detect survivor's position
void zombieRomi::detectSurvivorPosition() {
  // Implementation for detecting survivor's position
}

// Pursue the survivor
void zombieRomi::pursueSurvivor() {
  // Logic to pursue survivor
}

bool zombieRomi::getOnLastKnownPosition() {
  // Logic to determine if the robot is on target
  return isOnLastKnownPosition;
}

bool zombieRomi::onIntersection() {
  // Logic to determine if the robot is on an intersection
  return lineSensor.onCross();
}

float zombieRomi::getSonarDistance() {
  // Logic to get distance from sonar sensor
  return sonarSensor.getDistance();
}

float zombieRomi::getIRLeftDistance() {
  // Logic to get distance from left IR sensor
  return irSensorLeft.getDistance();
}

float zombieRomi::getIRRightDistance() {
  // Logic to get distance from right IR sensor
  return irSensorRight.getDistance();
}

// Additional implementation details as needed...
void zombieRomi::moveToLastKnownLocation() {
  bool isOnIntersection = onIntersection();
  bool wallAhead = getSonarDistance() < 30.0f;
  bool wallLeft = getIRLeftDistance() < WALL_DIS_THRESHOLD;
  bool wallRight = getIRRightDistance() < WALL_DIS_THRESHOLD;
  bool onTargetX = abs(currentX - lastKnownX) > 0.5f;
  bool onTargetY = abs(currentY - lastKnownY) > 0.5f;

  if (onTargetX && onTargetY) {
    isOnLastKnownPosition = true;
  }
}

int zombieRomi::getIntersectionCount() {
  // Logic to get intersection count
  return intersectionCount;
}

zombieRomi::turnDirection zombieRomi::getTurnDirection() {
  // Logic to determine which direction to turn
  return zombieRomi::turnDirection::STRAIGHT;
}

void zombieRomi::stop() {
  // Logic to stop the robot
  chassis.setTwist(0.0f, 0.0f);
}

bool zombieRomi::survivorFound() {
  // Logic to determine if the survivor has been found
  return false;
}

bool zombieRomi::survivorInfected() {
  // Logic to determine if the survivor is infected
  return false;
}