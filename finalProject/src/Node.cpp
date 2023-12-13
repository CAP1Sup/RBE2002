#include "Node.h"
#include <Arduino.h>
Node::Node() : x(0), y(0), cost(0), heuristic(0), parent(nullptr) {}
Node::Node(uint8_t x, uint8_t y)
    : x(x), y(y), cost(0), heuristic(0), parent(nullptr) {}

float Node::totalCost() const { return cost + heuristic; }

bool Node::operator==(const Node &other) const {
  return x == other.x && y == other.y;
}

bool Node::Comparator::operator()(const Node *a, const Node *b) {
  return a->totalCost() > b->totalCost();
}

bool Node::isValid() const {
  return x >= 0 &&
         y >= 0; // Adjust the condition based on your coordinate system
}

bool Node::isEqual(const Node &other) { return x == other.x && y == other.y; }
bool Node::isEqual(Node &other) {
  // Serial.println("Node 1 (this): x = " + String(x) + ", y = " + String(y));
  // Serial.println("Node 2 (other): x = " + String(other.x) +
  //                ", y = " + String(other.y));

  bool isEqual = this->x == other.x && this->y == other.y;
  // Serial.println("isEqual: " + String(isEqual ? 1 : 0));

  return isEqual;
}