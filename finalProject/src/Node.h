#pragma once

#include <stdint.h>

class Node {
public:
  uint8_t x, y;
  float cost;      // Cost to reach this node
  float heuristic; // Heuristic cost from this node to goal
  Node *parent;    // Parent node in the path
  // Node *neighbors[4]; // Parent node in the path

  Node();
  Node(uint8_t x, uint8_t y);

  float totalCost() const;

  bool operator==(const Node &other) const;

  struct Comparator {
    bool operator()(const Node *a, const Node *b);
  };
  bool isValid() const; // Check if the node has valid coordinates

  bool isEqual(Node &other);
  bool isEqual(const Node &other);
};