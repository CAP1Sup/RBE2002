#include "zombieRomi.h"

zombieRomi::zombieRomi(Chassis *chassis, Rangefinder *rangefinder) {
  // Constructor body

  this->rangefinder = rangefinder;
  this->chassis = chassis;

  irSensorLeft.init(LEFT_IR_PIN);
  irSensorRight.init(RIGHT_IR_PIN);
  rangefinder->init();
  chassis->init();
  lineSensor.init();
}

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
  chassis->setTwist(SEEKING_FWD_SPEED * IN_CH, LINE_P * difference);
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
  return 0; // sonarSensor.getDistance();
}

float zombieRomi::getIRLeftDistance() {
  // Logic to get distance from left IR sensor
  return 0; // irSensorLeft.getDistance();
}

float zombieRomi::getIRRightDistance() {
  // Logic to get distance from right IR sensor
  return 0; // irSensorRight.getDistance();
}

// Additional implementation details as needed...
void zombieRomi::moveToLastKnownLocation() {
  // bool isOnIntersection = onIntersection();
  // bool wallAhead = getSonarDistance() < 30.0f;
  // bool wallLeft = getIRLeftDistance() < WALL_DIS_THRESHOLD;
  // bool wallRight = getIRRightDistance() < WALL_DIS_THRESHOLD;
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
  chassis->setTwist(0.0f, 0.0f);
}

bool zombieRomi::survivorFound() {
  // Logic to determine if the survivor has been found
  return false;
}

bool zombieRomi::survivorInfected() {
  // Logic to determine if the survivor is infected
  return false;
}