/**
 * @file Node.cpp
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Node.h"

#include <Arduino.h>

Node::Node() : x(0), y(0), parent(nullptr) {}
Node::Node(uint8_t x, uint8_t y) : x(x), y(y), parent(nullptr) {}

bool Node::isValid() const {
  return x >= 0 &&
         y >= 0;  // Adjust the condition based on your coordinate system
}

bool Node::isEqual(const Node &other) {
  return this->x == other.x && this->y == other.y;
}
bool Node::isEqual(Node &other) {
  return this->x == other.x && this->y == other.y;
}

void Node::setWallUp(bool wall) {
  if (wall) {
    walls |= UP_MASK;
  } else {
    walls &= ~UP_MASK;
  }
}

void Node::setWallRight(bool wall) {
  if (wall) {
    walls |= RIGHT_MASK;
  } else {
    walls &= ~RIGHT_MASK;
  }
}

void Node::setWallDown(bool wall) {
  if (wall) {
    walls |= DOWN_MASK;
  } else {
    walls &= ~DOWN_MASK;
  }
}

void Node::setWallLeft(bool wall) {
  if (wall) {
    walls |= LEFT_MASK;
  } else {
    walls &= ~LEFT_MASK;
  }
}

void Node::setWalls(bool north, bool east, bool south, bool west) {
  walls = 0;
  walls |= north ? UP_MASK : 0;
  walls |= east ? RIGHT_MASK : 0;
  walls |= south ? DOWN_MASK : 0;
  walls |= west ? LEFT_MASK : 0;
}

void Node::printWall() {
  Serial.print("Node (");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(") ");
  Serial.print("Walls: ");
  Serial.print("Up: ");
  Serial.print(getWallUp());
  Serial.print(" Right: ");
  Serial.print(getWallRight());
  Serial.print(" Down: ");
  Serial.print(getWallDown());
  Serial.print(" Left: ");
  Serial.print(getWallLeft());
  Serial.println();
}

int Node::getX() const { return x; }
int Node::getY() const { return y; }

bool Node::getWallDown() const { return walls & DOWN_MASK ? true : false; }
bool Node::getWallUp() const { return walls & UP_MASK ? true : false; }
bool Node::getWallLeft() const { return walls & LEFT_MASK ? true : false; }
bool Node::getWallRight() const { return walls & RIGHT_MASK ? true : false; }