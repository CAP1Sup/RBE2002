#include "Zombie.h"

#ifdef ZOMBIE

Chassis chassis;
Rangefinder rangefinder(ECHO_PIN, TRIG_PIN);
// SonarSensor sonar;

Zombie::Zombie() : mazeSolver(maze) {
  // Constructor body
  irSensorLeft.init(LEFT_IR_PIN);
  irSensorRight.init(RIGHT_IR_PIN);
  rangefinder.init();
  lineSensor = LineSensor(LEFT_LINE_PIN, RIGHT_LINE_PIN);
  lineSensor.init();
  lineSensor.setThreshold(LINE_THRESHOLD);
  chassis.init();
  maze = Maze(MAX_WIDTH, MAX_HEIGHT);
}

void Zombie::run() {
  printAllSensor();
  switch (state) {
  case DEBUG:
    // Debugging code
    recordIntersection();
    break;
  case IDLE:
    if (buttonA.getSingleDebouncedRelease()) {
      state = SEEKING;
    }
    break;
  case SEEKING:
    receiveTargetCoordinates(0, 0); // Add time delay here
    moveToLastKnownLocation();
    if (survivorFound()) {
      state = CHASING;
    }
    break;
  case CHASING:
    pursueSurvivor();
    if (survivorInfected()) {
      state = STOP;
    } else if (!survivorFound()) {
      state = SEEKING;
    }
    break;
  case STOP:
    stop();
    break;
  }
}

// Receive target coordinates from MQTT server
void Zombie::receiveTargetCoordinates(float x, float y) {
  // Implementation for receiving coordinates
  lastKnownX = x;
  lastKnownY = y;
  isOnLastKnownPosition = false;
}

// Follow a line
void Zombie::followLine() {
  // Calculate the difference between readings from the two line sensors
  int difference =
      lineSensor.getLeftLineValue() - lineSensor.getRightLineValue();
  chassis.setTwist(SEEKING_FWD_SPEED * IN_CH, LINE_P * difference);
}

// Detect survivor's position
void Zombie::detectSurvivorPosition() {
  // Implementation for detecting survivor's position
}

// Pursue the survivor
void Zombie::pursueSurvivor() {
  // Logic to pursue survivor
}

bool Zombie::getOnLastKnownPosition() {
  // Logic to determine if the robot is on target
  return isOnLastKnownPosition;
}

bool Zombie::onIntersection() {
  // Logic to determine if the robot is on an intersection
  return lineSensor.onCross();
}

float Zombie::getSonarDistance() {
  // Logic to get distance from sonar sensor
  return rangefinder.getDistance() * 10;
}

float Zombie::getIRLeftDistance() {
  // Logic to get distance from left IR sensor
  return irSensorLeft.getAvgDistance();
}

float Zombie::getIRRightDistance() {
  // Logic to get distance from right IR sensor
  return irSensorRight.getAvgDistance();
}

void Zombie::printAllSensor() {
  Serial.println("Sonar: " + String(getSonarDistance()));
  Serial.println("IR Left: " + String(getIRLeftDistance()));
  Serial.println("IR Right: " + String(getIRRightDistance()));
  Serial.println("Line Left: " + String(getLeftLineValue()));
  Serial.println("Line Right: " + String(getRightLineValue()));

  // Logic to print all sensor readings
}

// Additional implementation details as needed...
void Zombie::moveToLastKnownLocation() {

  bool onTargetX = abs(currentX - lastKnownX) > 0.5f;
  bool onTargetY = abs(currentY - lastKnownY) > 0.5f;

  if (onTargetX && onTargetY) {
    isOnLastKnownPosition = true;
  }
}

int Zombie::getIntersectionCount() {
  // Logic to get intersection count
  return intersectionCount;
}

Zombie::turnDirection Zombie::getTurnDirection() {
  // Logic to determine which direction to turn
  return Zombie::turnDirection::STRAIGHT;
}

void Zombie::stop() {
  // Logic to stop the robot
  chassis.setTwist(0.0f, 0.0f);
}

bool Zombie::survivorFound() { // IMPLEMENT THIS
  // Logic to determine if the survivor has been found
  return false;
}

bool Zombie::survivorInfected() { // IMPLEMENT THIS
  // Logic to determine if the survivor is infected
  return false;
}

float Zombie::getLeftLineValue() { return lineSensor.getLeftLineValue(); }

float Zombie::getRightLineValue() { return lineSensor.getRightLineValue(); }

void Zombie::recordIntersection() {
  bool isOnIntersection = onIntersection();
  bool wallAhead = getSonarDistance() < WALL_SONAR_DIS_THRESHOLD;
  bool wallLeft = getIRLeftDistance() < WALL_IR_DIS_THRESHOLD;
  bool wallRight = getIRRightDistance() < WALL_IR_DIS_THRESHOLD;

  Serial.println("isOnIntersection: " + String(isOnIntersection));
  Serial.println("wallAhead: " + String(wallAhead));
  Serial.println("wallLeft: " + String(wallLeft));
  Serial.println("wallRight: " + String(wallRight));

  if (fabs(0 - currentTheta) < THETA_THESHOLD) {
    if (isOnIntersection) {
      if (wallAhead) {
        maze.setWall(currentXIndex, currentYIndex, true);
      }
      if (wallLeft) {
        maze.setWall(currentXIndex - 1, currentYIndex, true);
      }
      if (wallRight) {
        maze.setWall(currentXIndex + 1, currentYIndex, true);
      }
    }
  } else if (fabs(90 - currentTheta) < THETA_THESHOLD) {
    if (isOnIntersection) {
      if (wallAhead) {
        maze.setWall(currentXIndex, currentYIndex, true);
      }
      if (wallLeft) {
        maze.setWall(currentXIndex, currentYIndex - 1, true);
      }
      if (wallRight) {
        maze.setWall(currentXIndex, currentYIndex + 1, true);
      }
    }
  } else if (fabs(180 - currentTheta) < THETA_THESHOLD) {
    if (isOnIntersection) {
      if (wallAhead) {
        maze.setWall(currentXIndex, currentYIndex, true);
      }
      if (wallLeft) {
        maze.setWall(currentXIndex + 1, currentYIndex, true);
      }
      if (wallRight) {
        maze.setWall(currentXIndex - 1, currentYIndex, true);
      }
    }
  } else if (fabs(270 - currentTheta) < THETA_THESHOLD) {
    if (isOnIntersection) {
      if (wallAhead) {
        maze.setWall(currentXIndex, currentYIndex, true);
      }
      if (wallLeft) {
        maze.setWall(currentXIndex, currentYIndex + 1, true);
      }
      if (wallRight) {
        maze.setWall(currentXIndex, currentYIndex - 1, true);
      }
    }
  }

  intersectionCount++;
}

void Zombie::getIntersectionCoordinates() {
  // Logic to get intersection coordinates
  currentX = 0;
  currentY = 0;
  currentXIndex = 0;
  currentYIndex = 0; // IMPLEMENT
}

#endif