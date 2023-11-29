#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4.h>
#include <Wire.h>
#include <openmv.h>

#define PRINT_TAG_DATA

#define TARGET_WIDTH 25     // px
#define FRAME_WIDTH 160     // px
#define DRIVE_KP 2.0f       // (cm/s)/px
#define TURN_KP 2.0f        // (deg/s)/px
#define DRIVE_THRESHOLD 0.5 // cm/s
#define TURN_THRESHOLD 1    // deg/s
#define SEARCH_RATE 20      // deg/s
#define HALF_FRAME 80

OpenMV camera;
Chassis chassis;

const uint16_t redLEDPin = 13; // red LED connected to pin 13
uint32_t lastUpdateTime = 0;
bool isLEDOn = false;

void setup() {
  chassis.init();

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
void loop() {
  uint8_t tagCount = camera.getTagCount();
  if (tagCount) {
    Serial.println(tagCount);
    AprilTagDatum tag;
    if (camera.readTag(tag)) {
#ifdef PRINT_TAG_DATA
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
#endif
      // Check for ID 4
      if (tag.id == 3) {
        // Follow the tag
        float driveEffort = DRIVE_KP * (float(TARGET_WIDTH) - tag.w);
        float turnEffort = TURN_KP * ((float(FRAME_WIDTH) / 2) - tag.cx);
  
        //float turnEffort = turnError * TURN_KP;
        if (abs(driveEffort) < DRIVE_THRESHOLD) {
          driveEffort = 0;
        }
        if (abs(turnEffort) < TURN_THRESHOLD) {
          turnEffort = 0;
    
        }
  

        Serial.print("drive effort: ");
        Serial.println(driveEffort);
        chassis.setTwist(driveEffort, turnEffort);
      } else {
        // Blink red LED at 1 Hz for other tags
        Serial.print("here");
        blinkLED(redLEDPin, 1);
        chassis.idle();
      }
    }
  } else {
    Serial.println(F("No tags found."));
    chassis.setTwist(0, SEARCH_RATE);
  }
  delay(50);
}