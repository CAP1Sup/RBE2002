#pragma once

#include <Romi32U4.h>

#define COUNTS_PER_REV 1440 // how many counts equate to one wheel rotation?
#define WHEEL_DIA 70        // what is the radius of a Romi wheel in (mm)
#define WHEEL_CIRCUM (float)PI *WHEEL_DIA // circumference of wheel (mm)
#define POS_COUNT_TOL 10 // how close to target position (counts)

class Encoders {
private:
  // Speed tracking
  uint32_t prevLeftTime = 0;
  uint32_t prevRightTime = 0;
  int16_t prevLeftCount = 0;
  int16_t prevRightCount = 0;

  // Position tracking
  int16_t desiredLeftCount = 0;
  int16_t desiredRightCount = 0;

  Romi32U4Encoders encoders;

public:
  /**
   * @brief Get the current count of the left wheel
   *
   * @return int16_t count of the left wheel
   */
  int16_t getLeftCount();

  /**
   * @brief Returns the current speed of the left wheel in mm/s. MAKE SURE TO
   * LEAVE DELAY BETWEEN CALLS
   *
   * @return float speed in mm/s
   */
  float getLeftSpeed();

  /**
   * @brief Set the desired count for the left wheel
   *
   * @param count The desired count for the left wheel
   */
  void setDesiredLeftCount(int16_t count);

  /**
   * @brief Get the desired count for the left wheel
   *
   * @return int16_t The desired count for the left wheel
   */
  int16_t getDesiredLeftCount();

  /**
   * @brief Set the desired distance for the left wheel
   *
   * @param distance The desired distance for the left wheel
   */
  void setDesiredLeftDist(float distance);

  /**
   * @brief Returns if the left wheel is at the desired position
   *
   * @return Is the left wheel at the desired position?
   */
  bool isLeftAtPos();

  /**
   * @brief Get the current count of the right wheel
   *
   * @return int16_t count of the right wheel
   */
  int16_t getRightCount();

  /**
   * @brief Returns the current speed of the right wheel in mm/s. MAKE SURE TO
   * LEAVE DELAY BETWEEN CALLS
   *
   * @return float speed in mm/s
   */
  float getRightSpeed();

  /**
   * @brief Set the desired count for the right wheel
   *
   * @param count The desired count for the right wheel
   */
  void setDesiredRightCount(int16_t count);

  /**
   * @brief Set the desired distance for the right wheel
   *
   * @param distance The desired distance for the right wheel
   */
  void setDesiredRightDist(float distance);

  /**
   * @brief Returns if the right wheel is at the desired position
   *
   * @return Is the right wheel at the desired position?
   */
  bool isRightAtPos();

  /**
   * @brief Prints the current speed of the left and right wheels in mm/s
   *
   * @return float speed in mm/s
   */
  void printSpeeds();

  /**
   * @brief Resets the encoder counts to 0
   *
   */
  void reset();
};

#include "Encoders.cpp"