#include "SonarSensor.h"

#include <Romi32U4.h>

void SonarSensor::init(uint8_t trigPin, uint8_t echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

uint32_t SonarSensor::getDuration() {
  // Pulse the trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo
  return pulseIn(echoPin, HIGH, 1000000);
}

float SonarSensor::getDistance() {
  // assignment 1.2
  // read out and calibrate your sonar sensor, to convert readouts to distance
  // in [cm]
  // Convert to distance
  return (getDuration() * SPEED_OF_SOUND_CM_PER_US / 2.0f);
}

void SonarSensor::printDistance() { Serial.println(getDistance()); }