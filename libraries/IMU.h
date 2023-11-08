#pragma once

#include <Romi32U4.h>

#include "MedianFilter.h"

class IMU {
 private:
  // IMU
  LSM6 imu;

  // Minimum time between acceleration readings
  // Used to automatically update the IMU
  // Both are in ms
  uint32_t lastUpdate = 0;
  uint32_t minUpdatePeriod = 0;

  /**
   * @brief Updates the IMU and filters if enough time has passed
   *
   */
  void updateIfNeeded();

 public:
  /**
   * @brief Initializes the IMU and filters
   *
   * @param maxUpdateRate The maximum update rate of the IMU (in Hz)
   * @param filterSize The size of the median filter (must be <= 255)
   */
  void init(uint16_t maxUpdateRate, uint8_t filterSize);

  /**
   * @brief Returns if the robot is being picked up
   *
   * @param threshold The threshold for being picked up (in mgs)
   * @return Is the robot being picked up?
   */
  bool beingPickedUp(float threshold);

  /**
   * @brief Returns if the robot is colliding with something
   *
   * @param threshold The threshold for collision (in mgs)
   * @return Is the robot colliding with something?
   */
  bool hadCollision(float threshold);

  /**
   * @brief Prints the acceleration of the robot
   *
   */
  void printAccel();
};

#include "IMU.cpp"