#pragma once

#include <Romi32U4.h>

#include "Encoders.h"
#include "PIDController.h"

#define PID_UPDATE_INTERVAL 50  // how often do we update the motor effort? (ms)

// Robot selection
#define CURRENT_ROBOT 1  // 1-4

#if (CURRENT_ROBOT == 1)
#define PID_KP 1.15f
#define PID_KI 12.5f
#define PID_KD 0.0

#elif (CURRENT_ROBOT == 2)
#define PID_KP 1.15f
#define PID_KI 12.5f
#define PID_KD 0.0

#elif (CURRENT_ROBOT == 3)
#define PID_KP 1.15f
#define PID_KI 12.5f
#define PID_KD 0.0

#elif (CURRENT_ROBOT == 4)
#define PID_KP 1.15f
#define PID_KI 12.5f
#define PID_KD 0.0

#endif

class Chassis {
 private:
  // Basic variables
  float targetSpeedLeft = 0;
  float targetSpeedRight = 0;
  uint32_t endTime = 0;
  uint32_t lastPIDUpdate = 0;
  int16_t lastLeftEffort = 0;
  int16_t lastRightEffort = 0;

  // Objects
  PIDController leftPID = PIDController(PID_KP, PID_KI, PID_KD);
  PIDController rightPID = PIDController(PID_KP, PID_KI, PID_KD);
  Encoders encoders;
  Romi32U4Motors motors;  // No need to init, automatically done on first call
                          // to setEfforts()

 public:
  /**
   * @brief Returns the current effort of the left wheel
   *
   * @return int16_t effort
   */
  int16_t getLeftEffort();

  /**
   * @brief Returns the current effort of the right wheel
   *
   * @return int16_t effort
   */
  int16_t getRightEffort();

  /**
   * @brief Sets the effort of the left and right motors
   *
   * @param left Left motor effort (-300 to 300)
   * @param right Right motor effort
   */
  void setDriveEffort(int16_t leftEffort, int16_t rightEffort);

  /**
   * @brief Updates the motor effort using the PID controller
   *
   * @param deltaMs Time since the last update in milliseconds
   */
  void updateMotorEffort(uint32_t deltaMs);

  /**
   * @brief Updates the motor effort using a PID controller if enough time has
   * passed
   *
   */
  void updateMotorPID();

  /**
   * @brief A helper function for printing comma separated values to serial
   *
   * @param a First value
   * @param b Second value
   * @param c Third value
   * @param d Fourth value
   */
  void printToSerial(float a, float b, float c, float d);

  /**
   * @brief Prepares the robot for driving
   *
   */
  void beginDriving();

  /**
   * @brief Begins driving the robot at the specified speeds for the specified
   *
   * @param leftSpeed Target speed for the left wheel in mm/s
   * @param rightSpeed Target speed for the right wheel in mm/s
   * @param duration How long to drive for in milliseconds
   */
  void beginDriving(float, float, uint32_t);

  /**
   * @brief Sets the target speeds for the left and right wheels
   *
   * @param leftSpeed Target speed for the left wheel in mm/s
   * @param rightSpeed Target speed for the right wheel in mm/s
   */
  void setTargetSpeeds(float, float);

  /**
   * @brief Returns true if the robot has finished driving. ONly works if the
   * drive was started with a duration (by calling beginDriving(float, float,
   * uint32_t))
   *
   */
  bool isDriveComplete();

  /**
   * @brief Stops the robot
   *
   */
  void stop();
};

extern Chassis chassis;

#include "Chassis.cpp"