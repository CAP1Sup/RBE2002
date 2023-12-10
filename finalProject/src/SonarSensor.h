#pragma once

#include <Romi32U4.h>

#define SPEED_OF_SOUND_CM_PER_US 0.0343f

class SonarSensor {
 public:
  /**
   * @brief Initializes the sonar sensor
   *
   */
  void init(uint8_t trigPin, uint8_t echoPin);

  /**
   * @brief Measures the duration of the echo pulse
   *
   * @return uint32_t duration in [us]
   */
  uint32_t getDuration();

  /**
   * @brief Calculates the distance measured by the sonar sensor
   *
   * @return float distance in [cm]
   */
  float getDistance();

  /**
   * @brief Prints the distance measured by the sonar sensor
   *
   */
  void printDistance();

 private:
  uint8_t trigPin;
  uint8_t echoPin;
};

// #include "SonarSensor.cpp"
