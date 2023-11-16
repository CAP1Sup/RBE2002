#include "DDPoseEstimator.h"

#define POSE_UPDATE_INTERVAL 50  // ms

void DDPoseEstimator::reset() {
  x = 0;
  y = 0;
  theta = 0;
}

void DDPoseEstimator::update(float leftSpeed, float rightSpeed) {
  uint32_t currentTime = millis();
  if (currentTime - lastPoseUpdate >= POSE_UPDATE_INTERVAL) {
    // Check if the robot is moving straight or turning
    if (abs(leftSpeed - rightSpeed) <=
        SAME_SPEED_THRESHOLD) {  // moving straight

      // Calculate the average speed
      float avgSpeed = (leftSpeed + rightSpeed) / 2;

      // Convert to X and Y components
      float xSpeed = avgSpeed * cos(theta);
      float ySpeed = avgSpeed * sin(theta);

      // Add the estimated displacements to the current pose
      x += xSpeed * (currentTime - lastPoseUpdate) / 1000.0f;
      y += ySpeed * (currentTime - lastPoseUpdate) / 1000.0f;
      // No change in theta

    } else if (abs(leftSpeed + rightSpeed) <=
               SAME_SPEED_THRESHOLD) {  // point turning

      // Calculate the angular speed of the robot
      float omega = (rightSpeed - leftSpeed) / wheelWidth;

      // Add the change in theta to the current pose
      theta += omega * (currentTime - lastPoseUpdate) / 1000.0f;
      // No change in x or y

    } else {  // swing turning

      // Calculate ICC parameters
      float R = (wheelWidth / 2) *
                ((leftSpeed + rightSpeed) / (rightSpeed - leftSpeed));
      float omega = (rightSpeed - leftSpeed) / wheelWidth;

      // Add the change in pose with the old theta first
      x += -R * sin(theta);
      y += R * cos(theta);

      // Now change the theta by the omega times the time interval
      theta += omega * (currentTime - lastPoseUpdate);

      // Now add the change in pose with the new theta
      x += R * sin(theta);
      y += -R * cos(theta);
    }

    // Update the last pose update time
    lastPoseUpdate = currentTime;
  }
}

DDPoseEstimator::pose DDPoseEstimator::getPose() { return {x, y, theta}; }

void DDPoseEstimator::printPose() {
  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);
  Serial.print('\t');
  Serial.println(theta);
}