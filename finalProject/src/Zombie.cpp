/**
 * @file Zombie.cpp
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Zombie.h"

#ifdef ZOMBIE

Chassis chassis;
Rangefinder rangefinder(ECHO_PIN, TRIG_PIN);
extern Maze defaultMaze;
// SonarSensor sonar;

void Zombie::init() {
  // Constructor body
  irSensorLeft.init(LEFT_IR_PIN);
  irSensorRight.init(RIGHT_IR_PIN);
  rangefinder.init();
  lineSensor = LineSensor(LEFT_LINE_PIN, RIGHT_LINE_PIN);
  lineSensor.init();
  lineSensor.setThreshold(LINE_THRESHOLD);
  chassis.init();
  rangefinder.init();
  mqtt.init();

  // Camera setup
  Wire.begin();
  Wire.setClock(100000ul);
}

void Zombie::run() {
  if (mqtt.isMessageAvailable()) {
    Tag tag = mqtt.readTagMessage();
    if (tag.id == ZOMBIE_TOP_TAG_ID) {
      // TODO: Update the zombie's position?
    } else if (tag.id == SURVIVOR_TOP_TAG_ID) {
      // Update the survivor's position
      survivorNode = mqtt.toNode(tag);
    }
  }

  // Upload any tags from the camera to the MQTT server
  if (camera.getTagCount()) {
    AprilTagDatum tag;
    camera.readTag(tag);
    mqtt.sendMessage("tag" + String(tag.id) + "/str",
                     String(tag.cx) + "|" + String(tag.cy) + "|" +
                         String(tag.w) + "|" + String(tag.h) + "|" +
                         String(tag.rot) + "\n");
  }

  switch (state) {
    case DEBUG:
      delay(2000);
      if (!pathUpdated) {  // Precalculated path
        delay(1000);
        Serial.println("Getting path");
        getPath();
      }

      if (onIntersection()) {
        recordIntersection();
        while (onIntersection()) {
          chassis.driveFor(10, SEEKING_FWD_SPEED, true);
        }
        printMaze();
      } else {
        followLine();
      }

      // state = IDLE;
      break;
    case IDLE:
      Serial.println("Idle");
      if (!pathUpdated) {  // Precalculate path
        getPath();
      }
      if (buttonA.getSingleDebouncedRelease()) {
        state = SEEKING;
      }
      break;
    case SEEKING:
      receiveTargetCoordinates(0, 0);  // Add time delay here
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
  // pathUpdated = false;
}

// Follow a line
void Zombie::followLine() {
  // Calculate the difference between readings from the two line sensors
  int difference =
      lineSensor.getRightLineValue() - lineSensor.getLeftLineValue();
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
  return rangefinder.getDistance();
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

  if (onIntersection()) {
    recordIntersection();
    getPath();
    updateIntersectionIndex();
  }
}

uint8_t Zombie::getIntersectionCount() {
  // Logic to get intersection count
  return intersectionCount;
}

Zombie::headingDirection Zombie::getTurnDirection() {
  // Logic to determine which direction to turn
  headingDirection nextHeading;
  if (!path[currentPathIndex].y == currentIntersection_Y) {
    nextHeading = path[currentPathIndex].y > currentIntersection_Y
                      ? headingDirection::UP
                      : headingDirection::DOWN;
  } else {
    nextHeading = path[currentPathIndex].x > currentIntersection_X
                      ? headingDirection::RIGHT
                      : headingDirection::LEFT;
  }
  return nextHeading;
}

void Zombie::stop() {
  // Logic to stop the robot
  chassis.setTwist(0.0f, 0.0f);
}

bool Zombie::survivorFound() {  // IMPLEMENT THIS
  // Logic to determine if the survivor has been found
  return false;
}

bool Zombie::survivorInfected() {  // IMPLEMENT THIS
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
  if (currentHeading == UP) {
    if (isOnIntersection) {
      defaultMaze.setWall(currentIntersection_X, currentIntersection_Y,
                          wallAhead, wallLeft, false, wallRight);
    }
  } else if (currentHeading == LEFT) {
    if (isOnIntersection) {
      defaultMaze.setWall(currentIntersection_X, currentIntersection_Y,
                          wallLeft, false, wallRight, wallAhead);
    }
  } else if (currentHeading == DOWN) {
    if (isOnIntersection) {
      defaultMaze.setWall(currentIntersection_X, currentIntersection_Y, false,
                          wallRight, wallAhead, wallLeft);
    }
  } else if (currentHeading == RIGHT) {
    if (isOnIntersection) {
      defaultMaze.setWall(currentIntersection_X, currentIntersection_Y,
                          wallRight, wallAhead, wallLeft, false);
    }
  }
  pathUpdated = false;
  intersectionCount++;
}

void Zombie::getPath() {
  // Logic to get path
  pathLength = 0;
  pathFound = mazeSolver.findPath(
      defaultMaze.getNode(currentIntersection_X, currentIntersection_Y),
      defaultMaze.getNode(lastClosestIntersectionIndex_X,
                          lastClosestIntersectionIndex_Y),
      path, pathLength);
  Serial.print("Path found: ");
  Serial.println(pathFound ? "Yes" : "No");
  defaultMaze.printMaze();
  Serial.println();
  mazeSolver.printPath(path, pathLength);
  pathUpdated = true;
}

void Zombie::followPath() {
  if (pathUpdated && currentPathIndex < pathLength) {
    if (onIntersection()) {
      updateIntersectionIndex();
      chassis.driveFor(10, SEEKING_FWD_SPEED, true);
      headingDirection nextHeading = getTurnDirection();
      int turnAngle = nextHeading - currentHeading;
      chassis.turnFor(turnAngle, SEEKING_TURN_SPEED, true);
    } else {
      followLine();
    }
  }
}  // Needs  Implementation

void Zombie::closestIntersection() {}  // Needs Implementation

void Zombie::updateIntersectionIndex() {
  switch (currentHeading) {
    case UP:
      currentIntersection_Y = path[currentPathIndex].y + 1;
      break;
    case DOWN:
      currentIntersection_Y = path[currentPathIndex].y - 1;
      break;
    case LEFT:
      currentIntersection_X = path[currentPathIndex].x - 1;
      break;
    case RIGHT:
      currentIntersection_X = path[currentPathIndex].x + 1;
      break;
  }
}  // Needs Implementation

void Zombie::printMaze() { defaultMaze.printMaze(); }

#endif