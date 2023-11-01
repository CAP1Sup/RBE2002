#pragma once

#include <Romi32U4.h>

#define IR_PIN A0
#define IR_TABLE_SIZE 2

#define ADC_MAX_VALUE 1023.0f

enum IR_INTERP_VALUES { VOLTAGE = 0, DIST = 1 };

class IRSensor {
 private:
  // In format {voltage, distance in cm}
  // MUST BE IN DESCENDING ORDER
  // First and last value should be doubled... minor hack to prevent
  // interpolation beyond the range of the table
  float interpTable[IR_TABLE_SIZE][2] = {{0, 0}, {0, 0}};

  /**
   * @brief Linearly interpolates the input voltage to a distance from the IR
   * sensor
   *
   * @param voltage voltage from the IR sensor
   * @return float distance in [cm]
   */
  float voltageToDist(float voltage);

 public:
  /**
   * @brief Initializes the IR sensor
   *
   */
  void init();

  /**
   * @brief Reads the raw voltage from the IR sensor
   *
   * @return uint16_t raw voltage
   */
  float getRawVoltage();

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