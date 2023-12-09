#pragma once
#include <Romi32U4.h>

#define L_LINE_PIN A2 // Left line pin location
#define R_LINE_PIN A3 // right line pin location
#define WHT_THRES 500 // Value when sensor sees black

class LineSensor {
  // Type definitions for turning direction
  typedef enum { LEFT = 1, RIGHT = -1 } TURN_DIRECTION;
  int leftLineValue, rightLineValue;
  bool onLine;

public:
  LineSensor();
  int getLeftLineValue();
  int getRightLineValue();
  bool onCross();
};
