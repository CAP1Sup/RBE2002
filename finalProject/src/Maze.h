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

  void printMaze();

  void setWall(int x, int y, bool up, bool right, bool down, bool left);

  Node *getNode(int x, int y);

  void printWall(int x, int y);

  void checkWall();

  Node *getNodeChild(int x, int y);

private:
  Node grid[MAX_WIDTH][MAX_HEIGHT];
  int width;
  int height;
};