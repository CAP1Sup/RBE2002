#pragma once

#include <Romi32U4.h>

#define IR_PIN A0
#define IR_TABLE_SIZE 2

class IRSensor {
 private:
  // In format {adc, distance in cm}
  // MUST BE IN ASCENDING ORDER
  float interpTable[IR_TABLE_SIZE][2] = {{0, 0}, {0, 0}};
  float adcToDist(uint16_t adc);

 public:
  /**
   * @brief Initializes the IR sensor
   *
   */
  void init();

  /**
   * @brief Reads the raw ADC value from the IR sensor
   *
   * @return uint16_t raw ADC value
   */
  uint16_t getRawADC();

  /**
   * @brief Calculates the distance measured by the IR sensor
   *
   * @return float distance in [cm]
   */
  float getDistance();

  /**
   * @brief Prints the distance measured by the IR sensor
   *
   */
  void printDistance();
};