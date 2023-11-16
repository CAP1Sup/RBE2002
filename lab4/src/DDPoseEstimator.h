#pragma once

#include <Romi32U4.h>

#define M_TO_MM 1000

class DDPoseEstimator {
 private:
  float x, y, theta = 0;
  uint32_t lastPoseUpdate = 0;  // ms

  // FORMERLY KNOWN AS 'l'.. change for the lab
  // This equals 142.875 mm... Lewin's library uses 147 mm for the wheel width
  const float wheelWidth = 0.142875 * M_TO_MM;

  float sameSpeedThreshold = 0;
  uint32_t poseUpdateInterval = 0;

 public:
  struct pose {
    float X;
    float Y;
    float THETA;
  };

  /**
   * @brief Creates a new pose estimator
   *
   * @param poseUpdateInterval The interval at which the pose estimator should
   * update the pose (in ms)
   * @param sameSpeedThreshold The threshold for the robot to be considered
   * moving straight or point turning. Same unit as wheel speeds
   */
  DDPoseEstimator(uint32_t poseUpdateInterval, float sameSpeedThreshold);

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