#include "Arduino.h"
#include "Maze.h"

struct Tag {
  uint8_t id;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  float angle;
};

const uint16_t xColBounds[] = {105, 210, 320, 433, 540};
const uint16_t yRowBounds[] = {155, 278};

class MQTT {
 private:
  char input;
  uint8_t index = 0;
  char cArray[80];

 public:
  void init();
  bool isMessageAvailable();
  Tag readTagMessage();
  Node toNode(Tag tag);
  void sendMessage(const String& topic, const String& message);
};