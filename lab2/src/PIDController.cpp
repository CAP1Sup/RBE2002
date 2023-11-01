#include "PIDController.h"

PIDController::PIDController(float kp, float ki, float kd) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->prevError = 0;
  this->integral = 0;
  this->prevTime = 0;
}

float PIDController::calculate(float error) {
  uint32_t now = millis();
  float dt = (now - prevTime) / 1000.0f;
  prevTime = now;
  return calculate(error, dt);
}

float PIDController::calculate(float error, float dt) {
  integral += error * dt;
  float derivative = (error - prevError) / dt;
  prevError = error;
  prevTime = millis();
  return kp * error + ki * integral + kd * derivative;
}

void PIDController::reset() {
  prevError = 0;
  integral = 0;
  prevTime = millis();
}