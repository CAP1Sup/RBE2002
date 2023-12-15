/**
 * @file Maze.h
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "Arduino.h"
#include "Node.h"

const int MAX_WIDTH = 6; // Define maximum dimensions for the maze (nodes)
const int MAX_HEIGHT = 3;

class Maze {
public:
  /**
   * @brief Construct a new Maze object
   * Default constructor
   *
   */
  Maze(){};
  /**
   * @brief Construct a new Maze object
   *
   * @param width  Max Width
   * @param height Max Height
   */
  Maze(int width, int height);

  /**
   * @brief print the maze
   *
   */
  void printMaze();

  /**
   * @brief Set the Wall object
   *
   * @param x x coordinate
   * @param y y coordinate
   * @param up up wall
   * @param right right wall
   * @param down down wall
   * @param left  left wall
   */
  void setWall(int x, int y, bool up, bool right, bool down, bool left);

  /**
   * @brief Get the Node object
   *
   * @param x x coordinate
   * @param y y coordinate
   * @return Node*
   */
  Node *getNode(int x, int y);

  /**
   * @brief Print Wall Layout
   *
   * @param x x coordinate
   * @param y y coordinate
   */
  void printWall(int x, int y);

  /**
   * @brief Create virtual environment walls
   *
   */
  void checkWall();

  /**
   * @brief Get the Node Child object
   *
   * @param x x coordinate
   * @param y y coordinate
   * @return Node*
   */
  Node *getNodeChild(int x, int y);

  /**
   * @brief reset parents of all nodes
   *
   */
  void resetParents();

private:
  Node grid[MAX_WIDTH][MAX_HEIGHT]; // Grid of nodes
  int width;
  int height;
};