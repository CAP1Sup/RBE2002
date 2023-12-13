#pragma once

#include "Arduino.h"
#include "stdint.h"

class PID {
 public:
  /**
   * @brief Constructs a new PID controller
   *
   * @param kp Proportional gain
   * @param ki Integral gain
   * @param kd Derivative gain
   */
  PID(float kp, float ki, float kd);

  /**
   * @brief Calculates the control effort
   *
   * @param error The error to use in the PID controller
   * @return float The control effort
   */
  float calculate(float error);

  /**
   * @brief Calculates the control effort
   *
   * @param error The error to use in the PID controller
   * @param dt The time step (s)
   * @return float The control effort
   */
  float calculate(float error, float dt);

  /**
   * @brief Resets the PID controller
   *
   */
  void reset();

 private:
  float kp;
  float ki;
  float kd;
  float prevError;
  float integral;
  uint32_t prevTime;
};