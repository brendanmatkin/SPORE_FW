#ifndef TI_OTA_H
#define TI_OTA_H

#include <ArduinoOTA.h>
#include "Arduino.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define OTA_SERIAL_DEBUG true

struct TI_OTA {

  bool OTA_onboardLED = true;
  //void begin(const char**);
  void begin(String*);
  void loop();
  int checkServer(String __fwUrlBase, String __fwName, const int __FW_VERSION);
private:
  const char* deviceName;

};


#endif /* TI_OTA_H */
