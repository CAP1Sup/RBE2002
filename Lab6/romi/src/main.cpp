#include <Arduino.h>
#include <IMU.h>
#include <IRSensor.h>
#include <SonarSensor.h>

#define IMU_UPDATE_RATE 50  // Hz
#define MEDIAN_READINGS 5   // number of readings to use for median filter
#define MQTT_UPDATE_INTERVAL 1000  // ms

// Variables
uint32_t lastMQTTUpdate = 0;

// Objects
IRSensor irSensor;
SonarSensor usSensor;
IMU imu;

/**
 * sendMessage creates a string of the form
 *      topic:message
 * which is what the corresponding ESP32 code expects.
 * */
void sendMessage(const String& topic, const String& message) {
  Serial1.println(topic + String(':') + message);
}

void setup() {
  irSensor.init();
  usSensor.init();
  imu.init(IMU_UPDATE_RATE, MEDIAN_READINGS);
  Serial1.begin(115200);

  // Set internal pullup on RX1 to avoid spurious signals
  digitalWrite(0, HIGH);
}

void loop() {
  // Update the MQTT data if needed
  if (millis() - lastMQTTUpdate >= MQTT_UPDATE_INTERVAL) {
    lastMQTTUpdate = millis();
    sendMessage("irDist", String(irSensor.getDistance()));
    sendMessage("usDist", String(usSensor.getDistance()));
    sendMessage("imuZ", String(imu.getZAccel()));
  }

  // Update the IMU (if needed)
  imu.updateIfNeeded();
}