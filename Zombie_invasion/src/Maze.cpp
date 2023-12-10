#include "Maze.h"
Maze::Maze() {
  // Initialize all intersections to have no walls
  for (int x = 0; x < NUM_INTERSECTIONS_X; ++x) {
    for (int y = 0; y < NUM_INTERSECTIONS_Y; ++y) {
      intersections[x][y] = {false, false, false};
    }
  }
}

void Maze::setWalls(int intersectionX, int intersectionY, bool left, bool right,
                    bool front) {
  if (intersectionX >= 0 && intersectionX < NUM_INTERSECTIONS_X &&
      intersectionY >= 0 && intersectionY < NUM_INTERSECTIONS_Y) {
    intersections[intersectionX][intersectionY].wallLeft = left;
    intersections[intersectionX][intersectionY].wallRight = right;
    intersections[intersectionX][intersectionY].wallFront = front;
  }
}

Intersection Maze::getIntersectionInfo(int intersectionX, int intersectionY) {
  if (intersectionX >= 0 && intersectionX < NUM_INTERSECTIONS_X &&
      intersectionY >= 0 && intersectionY < NUM_INTERSECTIONS_Y) {
    return intersections[intersectionX][intersectionY];
  }
  return {false, false, false}; // Return default if out of bounds
}
