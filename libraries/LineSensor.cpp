#include "LineSensor.h"

// Constructor implementation
LineSensor::LineSensor(uint8_t leftPin, uint8_t rightPin) {
  this->leftPin = leftPin;
  this->rightPin = rightPin;
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