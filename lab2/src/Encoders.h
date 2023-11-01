#pragma once

#include <Romi32U4.h>

#define COUNTS_PER_REV 1440  // how many counts equate to one wheel rotation?
#define WHEEL_DIA 70         // what is the radius of a Romi wheel in (mm)
#define WHEEL_CIRCUM (float)PI* WHEEL_DIA  // circumference of wheel (mm)

class Encoders {
 private:
  uint32_t prevLeftTime = 0;
  uint32_t prevRightTime = 0;

  Romi32U4Encoders encoders;

 public:
  /**
   * @brief Returns the current speed of the left wheel in mm/s
   *
   * @return float speed in mm/s
   */
  float getLeftSpeed();

  /**
   * @brief Returns the current speed of the right wheel in mm/s
   *
   * @return float speed in mm/s
   */
  float getRightSpeed();

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