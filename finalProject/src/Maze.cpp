/**
 * @file Maze.cpp
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Maze.h"

Maze::Maze(int width, int height) : width(width), height(height) {
  // Initialize all cells to false (no wall)
  for (int x = 0; x < MAX_WIDTH; x++) {
    for (int y = 0; y < MAX_HEIGHT; y++) {
      grid[x][y] = Node(x, y);
    }
  }
}

void Maze::setWall(int x, int y, bool up, bool right, bool down, bool left) {
  if (y < MAX_HEIGHT && up) {
    grid[x][y + 1].setWallDown(up);
  }
  if (y > 0 && down) {
    grid[x][y - 1].setWallUp(down);
  }
  if (x > 0 && left) {
    grid[x - 1][y].setWallRight(left);
  }
  if (x < MAX_WIDTH && right) {
    grid[x + 1][y].setWallLeft(right);
  }
  grid[x][y].setWalls(up, right, down, left);
}

Node *Maze::getNode(int x, int y) { return &grid[x][y]; }

Node *Maze::getNodeChild(int x, int y) { return grid[x][y].child; }

void Maze::printMaze() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Serial.print("(");
      Serial.print(grid[x][y].getX());
      Serial.print(",");
      Serial.print(grid[x][y].getY());
      Serial.print(") ");
    }
    Serial.println();
  }
}

void Maze::printWall(int x, int y) { grid[x][y].printWall(); }

void Maze::checkWall() {
  // Check for walls
  setWall(0, 0, false, true, false, true);
  setWall(0, 1, true, false, false, true);
  setWall(1, 1, false, false, false, false);
  setWall(1, 2, true, true, false, false);
  setWall(0, 2, true, false, true, true);
}
