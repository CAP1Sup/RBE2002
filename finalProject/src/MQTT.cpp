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

Tag MQTT::readMessage() {
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