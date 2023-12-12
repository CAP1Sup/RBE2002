#include "Node.h"
Node::Node() : x(0), y(0), cost(0), heuristic(0), parent(nullptr) {}
Node::Node(int x, int y) : x(x), y(y), cost(0), heuristic(0), parent(nullptr) {}

float Node::totalCost() const { return cost + heuristic; }

bool Node::operator==(const Node &other) const {
  return x == other.x && y == other.y;
}

bool Node::Comparator::operator()(const Node *a, const Node *b) {
  return a->totalCost() > b->totalCost();
}