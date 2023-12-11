#pragma once
#include <Romi32U4.h>

#define IR_TABLE_SIZE 37
#define IR_AVG_SAMPLES 10
#define IR_MIN_SAMPLE_PERIOD 10

#define ADC_MAX_VALUE 1023.0f

enum IR_INTERP_VALUES { VOLTAGE = 0, DIST = 1 };

class IRSensor {
 private:
  // In format {voltage, distance in cm}
  // MUST BE IN DESCENDING ORDER
  // First and last value should be doubled... minor hack to prevent
  // interpolation beyond the range of the table
  // after 74 cm away, voltage picks back up, data is not useful
  float interpTable[IR_TABLE_SIZE][2] = {{625 * 5.0f / 1023.0f, 6},
                                         {570 * 5.0f / 1023.0f, 8},
                                         {480 * 5.0f / 1023.0f, 10},
                                         {410 * 5.0f / 1023.0f, 12},
                                         {355 * 5.0f / 1023.0f, 14},
                                         {312 * 5.0f / 1023.0f, 16},
                                         {281 * 5.0f / 1023.0f, 18},
                                         {262 * 5.0f / 1023.0f, 20},
                                         {243 * 5.0f / 1023.0f, 22},
                                         {224 * 5.0f / 1023.0f, 24},
                                         {210 * 5.0f / 1023.0f, 26},
                                         {196 * 5.0f / 1023.0f, 28},
                                         {192 * 5.0f / 1023.0f, 30},
                                         {181 * 5.0f / 1023.0f, 32},
                                         {172 * 5.0f / 1023.0f, 34},
                                         {165 * 5.0f / 1023.0f, 36},
                                         {157 * 5.0f / 1023.0f, 38},
                                         {150 * 5.0f / 1023.0f, 40},
                                         {144 * 5.0f / 1023.0f, 42},
                                         {136 * 5.0f / 1023.0f, 44},
                                         {128 * 5.0f / 1023.0f, 46},
                                         {120 * 5.0f / 1023.0f, 48},
                                         {114 * 5.0f / 1023.0f, 50},
                                         {104 * 5.0f / 1023.0f, 52},
                                         {99 * 5.0f / 1023.0f, 54},
                                         {92 * 5.0f / 1023.0f, 56},
                                         {87 * 5.0f / 1023.0f, 58},
                                         {81 * 5.0f / 1023.0f, 60},
                                         {75 * 5.0f / 1023.0f, 62},
                                         {70 * 5.0f / 1023.0f, 64},
                                         {67 * 5.0f / 1023.0f, 66},
                                         {60 * 5.0f / 1023.0f, 68},
                                         {0.30791788, 70},
                                         {0.26881721, 72},
                                         {0.24926686, 74}};

  float distValues[IR_AVG_SAMPLES];
  float distValueSum = 0;
  uint8_t distIndex = 0;
  uint8_t totalDistValues = 0;
  uint32_t lastDistSampleTime = 0;

  uint8_t pin;

  /**
   * @brief Linearly interpolates the input voltage to a distance from the
   * IR sensor
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
  void init(uint8_t pin);

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
   * @brief Calculates the average distance measured by the IR sensor
   *
   */
  float getAvgDistance();

  /**
   * @brief Resets the distance averaging
   *
   */
  void resetDistAvg();

  /**
   * @brief Prints the distance measured by the IR sensor
   *
   */
  void printDistance();
};

#include "IRSensor.cpp"