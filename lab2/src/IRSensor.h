#pragma once

#include <Romi32U4.h>

#define IR_PIN A0
#define IR_TABLE_SIZE 37

#define ADC_MAX_VALUE 1023.0f

enum IR_INTERP_VALUES { VOLTAGE = 0, DIST = 1 };

class IRSensor {
private:
  // In format {voltage, distance in cm}
  // MUST BE IN DESCENDING ORDER
  // First and last value should be doubled... minor hack to prevent
  // interpolation beyond the range of the table
  // after 74 cm away, voltage picks back up, data is not useful
  float interpTable[IR_TABLE_SIZE][2] = {
      {340, 2},         {625, 4},        {625, 6},  {570, 8},  {480, 10},
      {410, 12},        {355, 14},       {312, 16}, {281, 18}, {262, 20},
      {243, 22},        {224, 24},       {210, 26}, {196, 28}, {192, 30},
      {181, 32},        {172, 34},       {165, 36}, {157, 38}, {150, 40},
      {144, 42},        {136, 44},       {128, 46}, {120, 48}, {114, 50},
      {104, 52},        {99, 54},        {92, 56},  {87, 58},  {81, 60},
      {75, 62},         {70, 64},        {67, 66},  {60, 68},  {0.30791788, 70},
      {0.26881721, 72}, {0.24926686, 74}};

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