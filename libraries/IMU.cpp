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
  imu.setAccDataOutputRate(imu.ODR166k);
  imu.setGyroDataOutputRate(imu.ODR166k);

  // Initialize the median filters
  XAccelFilter = MedianFilter<int16_t>(filterSize);
  YAccelFilter = MedianFilter<int16_t>(filterSize);
  ZAccelFilter = MedianFilter<int16_t>(filterSize);

  // Calculate the minimum update period (in ms)
  minUpdatePeriod = 1000 / maxUpdateRate;
}

void IMU::updateIfNeeded() {
  if (millis() - lastUpdate > minUpdatePeriod) {
    imu.readAcc();
    XAccelFilter.addValue(imu.a.x);
    YAccelFilter.addValue(imu.a.y);
    ZAccelFilter.addValue(imu.a.z);
    lastUpdate = millis();
  }
}

bool IMU::beingPickedUp(float threshold) {
  updateIfNeeded();
  float aZ = ZAccelFilter.getMedian() * imu.mg;
  return (aZ > threshold);
}

bool IMU::hadCollision(float threshold) {
  updateIfNeeded();
  float aX = XAccelFilter.getMedian() * imu.mg;
  float aY = YAccelFilter.getMedian() * imu.mg;
  return ((abs(aX) > threshold) || (abs(aY) > threshold));
}

void IMU::printAccel() {
  updateIfNeeded();
  Serial.print("T: ");
  Serial.print(millis());
  Serial.print(" Ax: ");
  Serial.print(XAccelFilter.getMedian());
  Serial.print(" Ay: ");
  Serial.print(YAccelFilter.getMedian());
  Serial.print(" Az: ");
  Serial.println(ZAccelFilter.getMedian());
}