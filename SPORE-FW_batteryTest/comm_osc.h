#ifndef COMM_OSC_H
#define COMM_OSC_H

#include "OSCMessage.h"
#include "src/lib/TI_ota/TI_OTA.h"
TI_OTA ota;

void setBrightness(OSCMessage &msg, int addressOffset) {
  if (msg.isFloat(0)) {
    sensorState.brightness = msg.getFloat(0);
  }
  else {
    Serial.printf("[OSC] brightness not float\n");
    return;
  }
  if (SERIAL_DEBUG) Serial.printf("[OSC] setBrightness: %s\n", String(sensorState.brightness).c_str());
}

void setMode(OSCMessage &msg, int addressOffset) {
  if (msg.isInt(0)) {
    sensorState.mode = msg.getInt(0);
  }
  else {
    Serial.printf("[OSC] mode not int\n");
    return;
  }
  if (SERIAL_DEBUG) Serial.printf("[OSC] setMode: %s\n", String(sensorState.mode).c_str());
}

#endif /* COMM_OSC_H */
