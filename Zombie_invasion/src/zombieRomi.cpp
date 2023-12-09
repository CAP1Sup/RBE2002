#include "zombieRomi.h"

// Constructor implementation
ZombieRomi::ZombieRomi() {
  // Constructor body
  chassis.init();
  irSensorLeft.init();
  irSensorRight.init();
  sonarSensor.init();
  // TODO: Should this be here?
  // lineSensor.init();

  // Initialize member variables, set up sensors, networking, etc.
}

// Receive target coordinates from MQTT server
void ZombieRomi::receiveTargetCoordinates(float x, float y) {
  // Implementation for receiving coordinates
  lastKnownX = x;
  lastKnownY = y;
  isOnLastKnownPosition = false;
}

// Follow a line
void zombieRomi::followLine() {
  // Calculate the difference between readings from the two line sensors
  int difference =
      lineSensor->getLeftLineValue() - lineSensor->getRightLineValue();
  chassis->setTwist(SEEKING_FWD_SPEED * IN_CH, LINE_P * difference);
}

// Detect survivor's position
void ZombieRomi::detectSurvivorPosition() {
  // Implementation for detecting survivor's position
}

// Pursue the survivor
void ZombieRomi::pursueSurvivor() {
  // Logic to pursue survivor
}

bool zombieRomi::getOnLastKnownPosition() {
  // Logic to determine if the robot is on target
  return isOnLastKnownPosition;
}

// Move to the last known survivor's location
void ZombieRomi::moveToLastKnownLocation() {
  // Logic to move to last known location
}

bool ZombieRomi::onIntersection() {
  // Logic to determine if the robot is on an intersection
  return lineSensor.onCross();
}

float ZombieRomi::getSonarDistance() {
  // Logic to get distance from sonar sensor
  return sonarSensor.getDistance();
}

float ZombieRomi::getIRLeftDistance() {
  // Logic to get distance from left IR sensor
  return irSensorLeft.getDistance();
}

float ZombieRomi::getIRRightDistance() {
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
