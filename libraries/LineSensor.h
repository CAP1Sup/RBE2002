#pragma once
#include <Arduino.h>

#define WHT_THRES 500  // Value when sensor sees black

class LineSensor {
  uint8_t leftPin = 1;
  uint8_t rightPin = 1;

  // Type definitions for turning direction
  bool onLine;

 public:
  LineSensor(uint8_t line1, uint8_t line2);
  void init();
  int getLeftLineValue();
  int getRightLineValue();
  bool onCross();
};

#include "LineSensor.cpp"