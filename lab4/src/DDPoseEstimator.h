#pragma once

#include <Romi32U4.h>

#define SAME_SPEED_THRESHOLD 3  // units/s... in this case, mm
#define M_TO_MM 1000

class DDPoseEstimator {
 private:
  float x, y, theta = 0;
  uint32_t lastPoseUpdate = 0;  // ms

  // FORMERLY KNOWN AS 'l'.. change for the lab
  // This equals 142.875 mm... Lewin's library uses 147 mm for the wheel width
  const float wheelWidth = 0.142875 * M_TO_MM;

 public:
  struct pose {
    float X;
    float Y;
    float THETA;
  };

  /**
   * @brief Reset the pose estimator
   *
   */
  void reset();

  /**
   * @brief Update the pose estimator. Should be called often!
   *
   * @param target_speed_left
   * @param target_speed_right
   */
  void update(float, float);

  /**
   * @brief Get the estimated pose of the robot
   *
   * @return pose {X, Y, THETA}
   */
  pose getPose();

  /**
   * @brief Print the estimated pose of the robot
   *
   */
  void printPose();
};