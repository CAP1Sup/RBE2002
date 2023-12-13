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
      grid[x][y] = false;
    }
  }
}

bool Maze::isWall(int x, int y) const {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return grid[x][y];
  }
  return false; // Out of bounds
}

void Maze::setWall(int x, int y, bool hasWall) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    grid[x][y] = hasWall;
  }
}

int Maze::getWidth() const { return MAX_WIDTH; }

int Maze::getHeight() const { return MAX_HEIGHT; }
