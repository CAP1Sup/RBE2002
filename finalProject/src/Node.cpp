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
         y >= 0; // Adjust the condition based on your coordinate system
}

bool Node::isEqual(const Node &other) {
  return this->x == other.x && this->y == other.y;
}
bool Node::isEqual(Node &other) {
  return this->x == other.x && this->y == other.y;
}

void Node::setWalls(bool north, bool east, bool south, bool west) {
  walls.up = north;
  walls.right = east;
  walls.down = south;
  walls.left = west;
}

int Node::getX() const { return x; }
int Node::getY() const { return y; }