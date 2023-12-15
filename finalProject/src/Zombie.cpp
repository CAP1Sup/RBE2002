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
  pathLength = 2; // set to 2 to start path
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
  // Serial.print("Tag count: ");
  // Serial.println(camera.getTagCount());
  // if (camera.getTagCount()) {
  //  AprilTagDatum tag;
  //  if (camera.readTag(tag)) {
  /* mqtt.sendMessage("tag" + String(tag.id) + "/str",
                    String(tag.cx) + "|" + String(tag.cy) + "|" +
                        String(tag.w) + "|" + String(tag.h) + "|" +
                        String(tag.rot) + "\n");*/
  /*if (tag.id == SURVIVOR_SIDE_TAG_ID) {
    // Check if the survivor is infected
    if (tag.w > MAX_SURVIVOR_TAG_WIDTH) {
      // Survivor is infected
      chassis.idle();
      state = STOP;
    }
  }*/
  //}
  //}

  switch (state) {
  case DEBUG_PATHFIND:
    delay(3000);
    if (!pathUpdated) {
      Serial.println(F("Getting path"));
      defaultMaze.checkWall();
      getPath();
    }
    break;
  case IDLE:
    // Serial.println("Idle");
    if (buttonA.getSingleDebouncedRelease()) {
      Serial.println("Button A pressed");
      state = SEEKING;
    }
    break;
  case SEEKING:
    // Serial.println("Seeking");
    if (onIntersection()) {
      stop();
      recordIntersection();
      if (currentIntersection_X == lastClosestIntersectionIndex_X &&
          currentIntersection_Y == lastClosestIntersectionIndex_Y) {
        state = STOP;
        break;
      }
      Serial.println("Current Path Index: " + String(currentIntersection_X) +
                     ", " + String(currentIntersection_Y));
      if (!pathUpdated) { // Precalcualte path
        Serial.println("Getting path");
        defaultMaze.resetParents();
        getPath();
      }
      chassis.driveFor(3, SEEKING_FWD_SPEED, true);
      // This is the current intersection index
      headingDirection nextHeading = getTurnDirection();
      Serial.println("Next Path Index: " + String(nextNode_X) + ", " +
                     String(nextNode_Y));
      Serial.println("Check Path: " + String(path[0].x) + ", " +
                     String(path[0].y));

      int turnAngle = nextHeading - currentHeading;
      // Serial.println("Turn Angle: " + String(turnAngle));
      // Serial.println();
      if (!pathFound || onTarget()) {
        stop();
      } else if (turnAngle != 0) {
        chassis.driveFor(5, SEEKING_FWD_SPEED, true);
        chassis.turnFor(turnAngle, SEEKING_TURN_SPEED, true);
        currentHeading = nextHeading;
      }
    } else {
      followLine();
    }
    break;
  case STOP:
    Serial.println("Stop");
    stop();
    break;
  }
}

// Follow a line
void Zombie::followLine() {
  // Calculate the difference between readings from the two line sensors
  int difference =
      lineSensor.getRightLineValue() - lineSensor.getLeftLineValue();
  getSonarDistance();
  getIRLeftDistance();
  getIRRightDistance();
  chassis.setTwist(SEEKING_FWD_SPEED * IN_CH, LINE_P * difference);
  delay(10);
}

bool Zombie::onTarget() {
  // Logic to determine if the robot is on target
  return lastClosestIntersectionIndex_X == currentIntersection_X &&
         lastClosestIntersectionIndex_Y == currentIntersection_Y;
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

Zombie::headingDirection Zombie::getTurnDirection() {
  // Logic to determine which direction to turn
  headingDirection nextHeading;
  Node *child =
      defaultMaze.getNodeChild(currentIntersection_X, currentIntersection_Y);
  nextNode_X = child->getX();
  nextNode_Y = child->getY();
  if (nextNode_Y != currentIntersection_Y) {
    nextHeading = nextNode_Y > currentIntersection_Y ? headingDirection::UP
                                                     : headingDirection::DOWN;
  } else {
    nextHeading = nextNode_X > currentIntersection_X ? headingDirection::RIGHT
                                                     : headingDirection::LEFT;
  }
  return nextHeading;
}

void Zombie::stop() {
  // Logic to stop the robot
  chassis.setTwist(0.0f, 0.0f);
}

float Zombie::getLeftLineValue() { return lineSensor.getLeftLineValue(); }

float Zombie::getRightLineValue() { return lineSensor.getRightLineValue(); }

void Zombie::recordIntersection() {
  bool isOnIntersection = onIntersection();
  bool wallAhead = getSonarDistance() < WALL_SONAR_DIS_THRESHOLD;
  bool wallLeft = getIRLeftDistance() < WALL_IR_DIS_THRESHOLD;
  bool wallRight = getIRRightDistance() < WALL_IR_DIS_THRESHOLD;

  Serial.println(("wallAhead: " + String(wallAhead)));
  Serial.println("wallLeft: " + String(wallLeft));
  Serial.println("wallRight: " + String(wallRight));
  updateIntersectionIndex();
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
  defaultMaze.printWall(currentIntersection_X, currentIntersection_Y);
  pathUpdated = false;
  intersectionCount++;
}

void Zombie::getPath() {
  // Logic to get path
  pathLength = 0;
  pathFound = mazeSolver.findPath(
      defaultMaze.getNode(currentIntersection_X, currentIntersection_Y),
      defaultMaze.getNode(survivorNode.x, survivorNode.y), path, pathLength);

  Serial.print(F("Path found: "));
  Serial.println(pathFound ? F("Yes") : F("No"));

  mazeSolver.printPath(path, pathLength);
  pathUpdated = true;
}

void Zombie::updateIntersectionIndex() {
  switch (currentHeading) {
  case UP:
    currentIntersection_Y = currentIntersection_Y + 1;
    break;
  case DOWN:
    currentIntersection_Y = currentIntersection_Y - 1;
    break;
  case LEFT:
    currentIntersection_X = currentIntersection_X - 1;
    break;
  case RIGHT:
    currentIntersection_X = currentIntersection_X + 1;
    break;
  }
  currentPathIndex++;
} // Needs Implementation

void Zombie::printMaze() { defaultMaze.printMaze(); }

#endif