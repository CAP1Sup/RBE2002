/**
 * @file LineSensor.h
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <Arduino.h>

class LineSensor {
  uint8_t leftPin = A0;
  uint8_t rightPin = A1;

  // Type definitions for turning direction
  bool onLine;
  int THRESHOLD = 0;

public:
  LineSensor(uint8_t line1, uint8_t line2);
  void init();
  void setThreshold(int threshold);
  int getLeftLineValue();
  int getRightLineValue();
  bool onCross();
};

// #include "LineSensor.cpp"