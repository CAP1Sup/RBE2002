#include "LineSensor.h"

// Constructor implementation
LineSensor::LineSensor(uint8_t line1, uint8_t line2) {
  leftPin = line1;
  rightPin = line2;
};

void LineSensor::init() {
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
}

int LineSensor::getLeftLineValue() { return analogRead(leftPin); }

int LineSensor::getRightLineValue() { return analogRead(rightPin); }

bool LineSensor::onCross() {
  if (getLeftLineValue() > WHT_THRES && getRightLineValue() > WHT_THRES) {
    return true;
  } else {
    return false;
  }
}