#pragma once

#include <Romi32U4.h>

#include "Encoders.h"
#include "PIDController.h"

#define PID_UPDATE_INTERVAL \
  50                     // how often do we update the motor target speeds? (ms)
#define BASE_DIA 147.0f  // distance between wheels (mm)

// Motor PID constants
#define PID_KP 0.5f
#define PID_KI 5.0f
#define PID_KD 0.0f

typedef enum { LEFT = 1, RIGHT = -1 } TURN_DIR;

class Chassis : public Encoders {
 private:
  // Basic variables
  float targetSpeedLeft = 0;
  float targetSpeedRight = 0;
  uint32_t lastPIDUpdate = 0;
  int16_t lastLeftEffort = 0;
  int16_t lastRightEffort = 0;

  // Move tracking
  bool trackMove = false;
  int16_t desiredLeftCount = 0;
  int16_t desiredRightCount = 0;

  // Objects
  PIDController leftPID = PIDController(PID_KP, PID_KI, PID_KD);
  PIDController rightPID = PIDController(PID_KP, PID_KI, PID_KD);
  Romi32U4Motors motors;  // No need to init, automatically done on first call
                          // to setEfforts()

  template <typename T>
  int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

 public:
  /**
   * @brief Returns the current effort of the left wheel
   *
   * @return int16_t effort
   */
  int16_t getLeftEffort();

  /**
   * @brief Get the current count of the left wheel's encoder
   *
   * @return int16_t count of the left wheel
   */
  int16_t getLeftCount();

  /**
   * @brief Returns the current effort of the right wheel
   *
   * @return int16_t effort
   */
  int16_t getRightEffort();

  /**
   * @brief Get the current count of the right wheel's encoder
   *
   * @return int16_t count of the right wheel
   */
  int16_t getRightCount();

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
   * @brief Prepares the robot for driving. Should be called before using
   * setTargetSpeeds() for the first time
   *
   */
  void resetDrivePID();

  /**
   * @brief Begins driving the robot at the specified speeds. Should be called
   * after resetDrivePID().
   *
   * @param speed Target speed for the wheels in mm/s. Sign doesn't matter
   * @param distance Distance to drive in mm. Negatives will make robot drive
   * backwards
   */
  void drive(float speed, float distance);

  /**
   * @brief Sets the target speeds for the left and right wheels
   *
   * @param leftSpeed Target speed for the left wheel in mm/s
   * @param rightSpeed Target speed for the right wheel in mm/s
   */
  void setTargetSpeeds(float leftSpeed, float rightSpeed);

  /**
   * @brief Turns the robot in place
   *
   * @param angle Degrees to turn (left/CCW is positive, right/CW is negative)
   * @param speed Speed of the turn in deg/s
   */
  void pointTurn(float angle, float speed);

  /**
   * @brief
   *
   * @param angle Degrees to turn (left/CCW is positive, right/CW is negative)
   * @param speed Speed of the turn in deg/s
   * @param pivotWheel Which wheel to pivot around
   */
  void swingTurn(float angle, float speed, TURN_DIR pivotWheel);

  /**
   * @brief Returns if the robot is done moving
   *
   * @return Is the robot done moving?
   */
  bool isMotionComplete();

  /**
   * @brief Stops the robot
   *
   */
  void stop();
};

extern Chassis chassis;

#include "Chassis.cpp"