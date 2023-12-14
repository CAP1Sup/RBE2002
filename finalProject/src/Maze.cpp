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
  grid[x][y].setWalls(up, right, down, left);
}

Node *Maze::getNode(int x, int y) { return &grid[x][y]; }

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

void Maze::checkWall() {
  // Check for walls
  setWall(0, 0, true, false, false, false);
  setWall(0, 1, false, true, false, false);
  setWall(1, 1, true, false, false, false);
  setWall(0, 2, false, false, false, false);
  setWall(1, 2, false, false, true, false);
}
