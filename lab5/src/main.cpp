#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>
#include <Wire.h>
#include <openmv.h>

OpenMV camera;

const uint16_t redLEDPin = 13; // red LED connected to pin 13
uint32_t lastUpdateTime = 0;
bool isLEDOn = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin();
  Wire.setClock(100000ul);

  Serial.println(F("Welcome."));
}

void blinkLED(uint8_t pin, uint8_t frequency) {
  if (millis() - lastUpdateTime > (1000 / (2 * frequency))) {
    isLEDOn = !isLEDOn;
    digitalWrite(pin, isLEDOn);
    lastUpdateTime = millis();
  }
}

uint8_t findAprilTags() {
  uint8_t tagCount = camera.getTagCount();
  if (tagCount) {
    Serial.println(tagCount);
    AprilTagDatum tag;
    if (camera.readTag(tag)) {
      Serial.print(F("Tag [cx="));
      Serial.print(tag.cx);
      Serial.print(F(", cy="));
      Serial.print(tag.cy);
      Serial.print(F(", w="));
      Serial.print(tag.w);
      Serial.print(F(", h="));
      Serial.print(tag.h);
      Serial.print(F(", id="));
      Serial.print(tag.id);
      Serial.print(F(", rot="));
      Serial.print(tag.rot);
      Serial.println(F("]"));

      // Check for Tag25h9 with ID 4
      if (tag.family == "Tag25h9" && tag.id == 4) {
        // Follow the tag (add your code for following the tag here)
      } else {
        // Blink red LED at 1 Hz for other Tag25h9 tags
        blinkLED(redLEDPin, 1);
      }

      // Check for Tag36h11 or Tag16h5
      if (tag.family == "Tag36h11" || tag.family == "Tag16h5") {
        // Blink red LED at 10 Hz for Tag36h11 or Tag16h5 tags
        blinkLED(redLEDPin, 10);
      }
    }
  }

  return tagCount;
}

void loop() {
  delay(1); // calm things down for a msec...
  findAprilTags();
}