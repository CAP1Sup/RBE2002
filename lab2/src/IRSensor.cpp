#include "IRSensor.h"

#include <Romi32U4.h>

void IRSensor::init() { pinMode(IR_PIN, INPUT); }

float IRSensor::adcToDist(uint16_t adc) {
  // Loop through the table to find the correct range
  for (int i = 0; i < IR_TABLE_SIZE; i++) {
    if (adc < interpTable[i][0]) {
      // Found the range, interpolate
      float adc1 = interpTable[i - 1][0];
      float adc2 = interpTable[i][0];
      float dist1 = interpTable[i - 1][1];
      float dist2 = interpTable[i][1];
      return dist1 + (dist2 - dist1) * (adc - adc1) / (adc2 - adc1);
    }
  }

  // If we made it this far, return -1 to indicate an error
  return -1;
}

float IRSensor::getDistance() {
  // assignment 1.1
  // read out and calibrate your IR sensor, to convert readouts to distance in
  // [cm]
  // TODO: Write this
  return analogRead(IR_PIN);
}

void IRSensor::printDistance() { Serial.println(getDistance()); }