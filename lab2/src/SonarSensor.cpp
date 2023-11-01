#include "SonarSensor.h"

#include <Romi32U4.h>

void SonarSensor::init() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float SonarSensor::getDistance() {
  // assignment 1.2
  // read out and calibrate your sonar sensor, to convert readouts to distance
  // in [cm]
  // TODO: Write this

  // Pulse the trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo
  uint32_t duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance
  return (duration * SPEED_OF_SOUND_CM_PER_US / 2.0f);
}

void SonarSensor::printDistance() { Serial.println(getDistance()); }