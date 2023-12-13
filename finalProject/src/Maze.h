#pragma once

const int MAX_WIDTH = 6; // Define maximum dimensions for the maze
const int MAX_HEIGHT = 3;

class Maze {
public:
  Maze(){};
  Maze(int width, int height);

  bool isWall(int x, int y) const;
  void setWall(int x, int y, bool hasWall);

  int getWidth() const;
  int getHeight() const;

  int getX();
  int getY();

private:
  bool grid[MAX_WIDTH][MAX_HEIGHT];
  int width;
  int height;
};