#include "IRSensor.h"

#include <Romi32U4.h>

void IRSensor::init() { pinMode(IR_PIN, INPUT); }

float IRSensor::getRawVoltage() {
  return (5.0f * analogRead(IR_PIN)) / ADC_MAX_VALUE;
}

float IRSensor::voltageToDist(float voltage) {
  // Loop through the table to find the correct range
  for (int i = 1; i < IR_TABLE_SIZE; i++) {
    if (voltage > interpTable[i][VOLTAGE]) {
      // Found the range, interpolate
      float voltage1 = interpTable[i - 1][VOLTAGE];
      float voltage2 = interpTable[i][VOLTAGE];
      float dist1 = interpTable[i - 1][DIST];
      float dist2 = interpTable[i][DIST];
      return dist1 +
             (dist2 - dist1) * (voltage - voltage1) / (voltage2 - voltage1);
    }
  }

  // If we made it this far, return -1 to indicate an error
  return -1;
}

float IRSensor::getDistance() {
  // assignment 1.1
  // read out and calibrate your IR sensor, to convert readouts to distance in
  // [cm]
  return voltageToDist(getRawVoltage());
}

void IRSensor::printDistance() { Serial.println(getDistance()); }