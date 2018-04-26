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


void setSendRate(OSCMessage &msg, int addressOffset) {
  if (msg.isFloat(0)) {
    sensorState.sendRate = msg.getFloat(0);
  }
  else {
    Serial.printf("[OSC] sendRate not float\n");
    return;
  }
  if (SERIAL_DEBUG) Serial.printf("[OSC] setSendRate: %s\n", String(sensorState.sendRate).c_str());
}


void changeServer(OSCMessage &msg, int addressOffset) {
  String _newServer = "";
  uint8_t octets[4];
  for (int i = 0; i < 4; i++) {
    if (msg.isInt(i)) {
      octets[i] = msg.getInt(i);
      _newServer+=octets[i];
      if (i < 3) _newServer+='.';
      serverIP[i] = octets[i];
    }
    else {
      Serial.printf("[OSC] changeServer ip @ octet %u not int\n", i);
      return;
    }
  }
  webSocket.disconnect();
  webSocket.begin(_newServer, wsPort, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  
  if (SERIAL_DEBUG) Serial.printf("[OSC] changeServer ws: %s, osc: %s\n", _newServer.c_str(), serverIP.toString().c_str());
}
#endif /* COMM_OSC_H */
