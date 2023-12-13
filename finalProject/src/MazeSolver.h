#pragma once
#include "Maze.h"
#include "Node.h"
#include <Arduino.h>
#include <math.h>

const int MAX_NODES = MAX_HEIGHT * MAX_WIDTH;
const int MAX_NEIGHBORS = 4; // Maximum number of neighbors

class MazeSolver {
public:
  MazeSolver();
  MazeSolver(const Maze &maze);
  bool findPath(Node start, Node goal, Node path[], int &pathLength);

private:
  const Maze &maze;
  Node goalNode; // Store the goal node
  Node *openList[MAX_NODES];
  int openListSize;

  void addToOpenList(Node *node);
  Node *popBestNode(const Node &goal);
  int getNeighbors(const Node &node, Node neighbors[]);
  void reconstructPath(Node *current, Node path[], int &pathLength);
  float calculateHeuristic(const Node &a, const Node &b);
};