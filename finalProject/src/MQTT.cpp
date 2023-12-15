#include "MQTT.h"

void MQTT::init() {
  Serial1.begin(115200);
  memset(cArray, 0, sizeof(cArray));
}

bool MQTT::isMessageAvailable() {
  do {
    if (Serial1.available() > 0) {
      input = Serial1.read();
      cArray[index] = input;
      index++;
    } else {
      return false;
    }
  } while (input != '\n');
  return true;
}

Tag MQTT::readTagMessage() {
  // Create a new Tag to store the data
  Tag tag;

  // Create a new string to store the message
  String message = String(cArray);

  // Harvest the id from the message
  tag.id =
      atoi(message.substring(message.indexOf("tag") + 3, message.indexOf("str"))
               .c_str());

  // Get the string of data
  String data = message.substring(message.indexOf("str") + 4);

  // Harvest the tag values from the data
  tag.x = atoi(data.substring(0, data.indexOf("|")).c_str());
  data = data.substring(data.indexOf("|") + 1);
  tag.y = atoi(data.substring(0, data.indexOf("|")).c_str());
  data = data.substring(data.indexOf("|") + 1);
  tag.w = atoi(data.substring(0, data.indexOf("|")).c_str());
  data = data.substring(data.indexOf("|") + 1);
  tag.h = atoi(data.substring(0, data.indexOf("|")).c_str());
  data = data.substring(data.indexOf("|") + 1);
  tag.angle = atof(data.c_str());

  // Clear the array
  index = 0;
  memset(cArray, 0, sizeof(cArray));

  // Return the tag
  return tag;
}

Node MQTT::toNode(Tag tag) {
  uint8_t col = 0;
  uint8_t row = 0;

  // Find the column
  for (uint8_t i = 0; i < sizeof(xColBounds) / sizeof(xColBounds[0]); i++) {
    if (tag.x > xColBounds[i]) {
      col++;
    }
  }

  // Flip the column
  // X = 0 is on the right side of the map
  // Subtract from the length minus 1 because the array is indexed from 0
  col = sizeof(xColBounds) / sizeof(xColBounds[0]) - col - 1;

  // Find the row
  for (uint8_t i = 0; i < sizeof(yRowBounds) / sizeof(yRowBounds[0]); i++) {
    if (tag.y > yRowBounds[i]) {
      row++;
    }
  }

  return Node(col, row);
}