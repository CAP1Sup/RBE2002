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
  Node openList[MAX_NODES];
  int openListSize;
  bool visited[MAX_WIDTH][MAX_HEIGHT];
  const Maze &maze;

  void addToOpenList(const Node &node);
  Node popBestNode();
  int getNeighbors(const Node &node, Node neighbors[]);
  float calculateHeuristic(const Node &a, const Node &b);
  float edgeCost(const Node &a, const Node &b);
  void reconstructPath(Node *node, Node path[], int &pathLength);
};