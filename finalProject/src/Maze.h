const int NUM_INTERSECTIONS_X = 6;
const int NUM_INTERSECTIONS_Y = 3;

struct Intersection {
  bool wallLeft;
  bool wallRight;
  bool wallFront;
};

class Maze {
public:
  Maze();

  void setWalls(int intersectionX, int intersectionY, bool left, bool right,
                bool front);

  Intersection getIntersectionInfo(int intersectionX, int intersectionY);

private:
  Intersection intersections[NUM_INTERSECTIONS_X][NUM_INTERSECTIONS_Y];
};