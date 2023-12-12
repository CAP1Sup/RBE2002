#pragma once
#include <Romi32U4.h>

#define IR_TABLE_SIZE 37
#define IR_AVG_SAMPLES 10
#define IR_MIN_SAMPLE_PERIOD 10

#define ADC_MAX_VALUE 1023.0f

enum IR_INTERP_VALUES { VOLTAGE = 0, DIST = 1 };

const static float PROGMEM voltages[IR_TABLE_SIZE] = {625 * 5.0f / 1023.0f,
                                                      570 * 5.0f / 1023.0f,
                                                      480 * 5.0f / 1023.0f,
                                                      410 * 5.0f / 1023.0f,
                                                      355 * 5.0f / 1023.0f,
                                                      312 * 5.0f / 1023.0f,
                                                      281 * 5.0f / 1023.0f,
                                                      262 * 5.0f / 1023.0f,
                                                      243 * 5.0f / 1023.0f,
                                                      224 * 5.0f / 1023.0f,
                                                      210 * 5.0f / 1023.0f,
                                                      196 * 5.0f / 1023.0f,
                                                      192 * 5.0f / 1023.0f,
                                                      181 * 5.0f / 1023.0f,
                                                      172 * 5.0f / 1023.0f,
                                                      165 * 5.0f / 1023.0f,
                                                      157 * 5.0f / 1023.0f,
                                                      150 * 5.0f / 1023.0f,
                                                      144 * 5.0f / 1023.0f,
                                                      136 * 5.0f / 1023.0f,
                                                      128 * 5.0f / 1023.0f,
                                                      120 * 5.0f / 1023.0f,
                                                      114 * 5.0f / 1023.0f,
                                                      104 * 5.0f / 1023.0f,
                                                      99 * 5.0f / 1023.0f,
                                                      92 * 5.0f / 1023.0f,
                                                      87 * 5.0f / 1023.0f,
                                                      81 * 5.0f / 1023.0f,
                                                      75 * 5.0f / 1023.0f,
                                                      70 * 5.0f / 1023.0f,
                                                      67 * 5.0f / 1023.0f,
                                                      60 * 5.0f / 1023.0f,
                                                      0.30791788,
                                                      0.26881721,
                                                      0.24926686};

const static uint8_t PROGMEM distances[IR_TABLE_SIZE] = {
    6,  8,  10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
    42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74};

class IRSensor {
private:
  // In format {voltage, distance in cm}
  // MUST BE IN DESCENDING ORDER
  // First and last value should be doubled... minor hack to prevent
  // interpolation beyond the range of the table
  // after 74 cm away, voltage picks back up, data is not useful

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
