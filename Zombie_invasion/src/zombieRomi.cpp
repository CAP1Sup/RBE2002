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
}

// Update the zombie's current target
void ZombieRomi::updateTarget() {
  // Logic to update target
}

// Follow a line
void ZombieRomi::followLine() {
  // Line following logic
}

// Detect survivor's position
void ZombieRomi::detectSurvivorPosition() {
  // Implementation for detecting survivor's position
}

// Pursue the survivor
void ZombieRomi::pursueSurvivor() {
  // Logic to pursue survivor
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