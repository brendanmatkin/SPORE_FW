#ifndef COMM_WS_H
#define COMM_WS_H

#include <ArduinoJson.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;
extern const bool SERIAL_DEBUG;



/*--------- make JSON object from serial websocket data -----------*/
bool deserializeJSON(uint8_t * json) {
  StaticJsonBuffer<512> jsonBuffer;     // actually only needs to be about 240 bytes, extra reserved for future use (or just because I have the space..). (http://arduinojson.org/assistant/)
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    if (SERIAL_DEBUG) Serial.printf("[ws] parseObject() failed\n");
    return false;
  }
  else {
    if (SERIAL_DEBUG) Serial.printf("[ws] parseObject() success!\n");
        
  }
  return root.success();
}


  
/*--------- make serialized JSON to send over websockets -----------*/
const uint16_t jsonSendSize = 256;
void serializeJSON_connected(char * json) {
    StaticJsonBuffer<jsonSendSize> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
//    root["type"] = MSG_TYPE_CONNECT_INFO;
//    // JsonObject& data = root.createNestedObject("data");
//    // data["id"] = address;
//    // data["firmwareVersion"] = FW_VERSION;
//    root["id"] = address;
//    root["firmwareVersion"] = FW_VERSION;
      root["id"] = address;
      root["battery"] = sensorState.voltage;
    //if (SERIAL_DEBUG) Serial.println(json);
    root.printTo(json, jsonSendSize);
    if (SERIAL_DEBUG) root.printTo(Serial); Serial.println();
}


/*---------- Websocket Event ------------*/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_ERROR:
      if (SERIAL_DEBUG) Serial.printf("[ws] ERROR!\n");
      break;
    case WStype_DISCONNECTED:
      if (SERIAL_DEBUG) Serial.printf("[ws] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      if (SERIAL_DEBUG) Serial.printf("[ws] Connected to url: %s; replying\n", payload);
      char connJSON[jsonSendSize];
      serializeJSON_connected(connJSON);
      webSocket.sendTXT(connJSON);    // send to server
    }
      break;
    case WStype_TEXT: {
        if (SERIAL_DEBUG) Serial.printf("[ws] got text: %s\n", payload);
        // deserializeJSON(payload);
        char connJSON[jsonSendSize];
        serializeJSON_connected(connJSON);
        webSocket.sendTXT(connJSON);    // send to server
      }
      break;
    case WStype_BIN:
      if (SERIAL_DEBUG) Serial.printf("[ws] got binary (length): %zu\n", length);
      hexdump(payload, length);
      break;
    default:
      break;
  }
}

#endif /* COMM_WS_H */
