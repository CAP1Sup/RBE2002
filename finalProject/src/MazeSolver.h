/**
 * @file MazeSolver.h
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "Maze.h"
#include <Arduino.h>
#include <math.h>

const int MAX_NODES = MAX_HEIGHT * MAX_WIDTH;
const int MAX_NEIGHBORS = 3; // Maximum number of neighbors

class MazeSolver {
public:
  /**
   * @brief Construct a new Maze Solver object
   * Default constructor
   *
   */
  MazeSolver();
  /**
   * @brief Construct a new Maze Solver object
   *
   * @param maze
   */
  MazeSolver(const Maze &maze);
  /**
   * @brief Find the path from start to goal
   *
   * @param start Starting Node
   * @param goal Ending Node (Goal)
   * @param path Store nodes in path array
   * @param pathLength Length of the path
   * @return true
   * @return false
   */
  bool findPath(Node *start, Node *goal, Node path[], int &pathLength);
  /**
   * @brief Print the path
   *
   * @param path Array of nodes
   * @param pathLength Length of the path
   */
  void printPath(Node path[], int pathLength);

private:
  const Maze &maze;                      // Reference to the maze
  Node goalNode;                         // Store the goal node
  Node *openList[MAX_NODES] = {nullptr}; // Make all null Pointers to start
  int openListSize;                      // Number of elements in the open list

  /**
   * @brief Add a node to the open list
   *
   * @param node Node to be added
   */
  void addToOpenList(Node *node);
  /**
   * @brief Pop the best node from the open list
   *
   * @param goal Goal node
   * @return Node*
   */
  Node *popBestNode(const Node &goal);
  /**
   * @brief Get the Neighbors object
   *
   * @param node Node
   * @param neighbors Array of neighbors of node
   * @return int
   */

  int getNeighbors(const Node &node, Node *neighbors[]);

  /**
   * @brief Reconstruct the path
   *
   * @param goalNode Goal node
   * @param path Array of nodes
   * @param pathLength Length of the path
   */
  void reconstructPath(Node &goalNode, Node path[], int &pathLength);
  /**
   * @brief Calculate the heuristic
   *
   * @param a node
   * @param b node
   * @return float
   */
  int calculateHeuristic(const Node &a, const Node &b);
};