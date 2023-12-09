#include "zombieRomi.h"

// Constructor implementation
zombieRomi::zombieRomi(IRSensor *irl, IRSensor *irr, SonarSensor *sonar,
                       LineSensor *lineSensor, Chassis *chassis)
    : irSensorLeft(irl), irSensorRight(irr), sonarSensor(sonar),
      lineSensor(lineSensor), chassis(chassis) {
  // Constructor body

  // Initialize member variables, set up sensors, networking, etc.
}

// Receive target coordinates from MQTT server
void zombieRomi::receiveTargetCoordinates(float x, float y) {
  // Implementation for receiving coordinates
  lastKnownX, lastKnownY = x, y;
}

// Update the zombie's current target
void zombieRomi::updateTarget() {
  // Logic to update target
}

// Follow a line
void zombieRomi::followLine() {
  // Line following logic
}

// Detect survivor's position
void zombieRomi::detectSurvivorPosition() {
  // Implementation for detecting survivor's position
}

// Pursue the survivor
void zombieRomi::pursueSurvivor() {
  // Logic to pursue survivor
}

// Move to the last known survivor's location
void zombieRomi::moveToLastKnownLocation() {
  // Logic to move to last known location
}

bool zombieRomi::onIntersection() {
  // Logic to determine if the robot is on an intersection
  lineSensor->onCross();
}

float zombieRomi::getSonarDistance() {
  // Logic to get distance from sonar sensor
  sonarSensor->getDistance();
}

float zombieRomi::getIRLeftDistance() {
  // Logic to get distance from left IR sensor
  irSensorLeft->getDistance();
}

float zombieRomi::getIRRightDistance() {
  // Logic to get distance from right IR sensor
  irSensorRight->getDistance();
}

// Additional implementation details as needed...