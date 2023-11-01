#include "SonarSensor.h"

#include <Romi32U4.h>

void SonarSensor::init() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

uint32_t SonarSensor::getDuration() {
  // Pulse the trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo
  return pulseIn(ECHO_PIN, HIGH);
}

float SonarSensor::getDistance() {
  // assignment 1.2
  // read out and calibrate your sonar sensor, to convert readouts to distance
  // in [cm]
  // Convert to distance
  return (getDuration() * SPEED_OF_SOUND_CM_PER_US / 2.0f);
}

void SonarSensor::printDistance() { Serial.println(getDistance()); }