#include "Arduino.h"

struct Tag {
  uint8_t id;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  float angle;
};

class MQTT {
 private:
  char input;
  uint8_t index = 0;
  char cArray[80];

 public:
  void init();
  bool isMessageAvailable();
  Tag readMessage();
};