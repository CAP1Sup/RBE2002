#include "IMU.h"

#include <LSM6.h>
#include <Romi32U4.h>

// Filters
// Initialize with a size of 5 by default
// Will be overwritten in init()
MedianFilter<int16_t> XAccelFilter(5);
MedianFilter<int16_t> YAccelFilter(5);
MedianFilter<int16_t> ZAccelFilter(5);

void IMU::init(uint16_t maxUpdateRate, uint8_t filterSize) {
  Wire.begin();
  if (!imu.init()) {
    while (true) {
      Serial.println("Failed to detect the LSM6.");
      delay(100);
    }
  }

  // Possibly redundant?
  imu.setFullScaleAcc(imu.ACC_FS2);
  imu.enableDefault();
  imu.setAccDataOutputRate(imu.ODR833);
  imu.setGyroDataOutputRate(imu.ODR833);

  // Initialize the median filters
  XAccelFilter = MedianFilter<int16_t>(filterSize);
  YAccelFilter = MedianFilter<int16_t>(filterSize);
  ZAccelFilter = MedianFilter<int16_t>(filterSize);

  // Calculate the minimum update period (in ms)
  minUpdatePeriod = 1000 / maxUpdateRate;

  // Call the update function to fill the filter
  for (int i = 0; i < filterSize; i++) {
    updateAccel();
  }

  // Check if the Z axis is inverted
  if (ZAccelFilter.getMedian() > 0) {
    flipZAccel = -1;
  }

  // Save the biases
  xAccelBias = XAccelFilter.getMedian();
  yAccelBias = YAccelFilter.getMedian();
  zAccelBias = 1000.0 / imu.mg + (flipZAccel * ZAccelFilter.getMedian());

  // Re-initialize the median filters
  XAccelFilter = MedianFilter<int16_t>(filterSize);
  YAccelFilter = MedianFilter<int16_t>(filterSize);
  ZAccelFilter = MedianFilter<int16_t>(filterSize);
}

void IMU::updateIfNeeded() {
  if (millis() - lastUpdate > minUpdatePeriod) {
    updateAccel();
    lastUpdate = millis();
  }
}

void IMU::updateAccel() {
  imu.readAcc();
  XAccelFilter.addValue(imu.a.x - xAccelBias);
  YAccelFilter.addValue(imu.a.y - yAccelBias);
  ZAccelFilter.addValue(flipZAccel * imu.a.z - zAccelBias);
}

bool IMU::beingPickedUp(float threshold) {
  updateIfNeeded();
  return (ZAccelFilter.getMedian() * imu.mg > threshold);
}

bool IMU::hadCollision(float threshold) {
  updateIfNeeded();
  float aX = XAccelFilter.getMedian() * imu.mg;
  float aY = YAccelFilter.getMedian() * imu.mg;
  return ((abs(aX) > threshold) || (abs(aY) > threshold));
}

void IMU::printAccel() {
  //updateIfNeeded();
  Serial.print("T: ");
  Serial.print(millis());
  Serial.print(" Ax: ");
  Serial.print(XAccelFilter.getMedian() * imu.mg);
  Serial.print(" Ay: ");
  Serial.print(YAccelFilter.getMedian() * imu.mg);
  Serial.print(" Az: ");
  Serial.println(ZAccelFilter.getMedian() * imu.mg);
}