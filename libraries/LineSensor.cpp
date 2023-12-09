#include "LineSensor.h"

// Constructor implementation
LineSensor::LineSensor(){};

LineSensor::getLeftLineValue() {
  leftLineValue = analogRead(L_LINE_PIN);
  return analogRead(L_LINE_PIN);
}

LineSensor::getRightLineValue() {
  rightLineValue = analogRead(R_LINE_PIN);
  return analogRead(R_LINE_PIN);
}

LineSensor::onCross() {
  if (leftLineValue > WHT_THRES && rightLineValue > WHT_THRES) {
    return true;
  } else {
    return false;
  }
}