#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUDP.h>
#include <EEPROM.h>


const bool SERIAL_DEBUG = true;               // enabling this will disable the LEDDAR and vice versa
const bool ENABLE_WEBSOCKET = false;          // 


/* -- firmware settings -- */
#define HW_VERSION "0.0.3"
#define HW_PHASE   ""
const int FW_VERSION = 100;                   // 0.1.00, convention: 1.2.10 = 1210, 0.5.9 = 509, no leading 0s or it is interpreted as octal.. learned that the hard way!
#define FW_PHASE   "-alpha"
float fwCheckButtonTime = 2000.0f;            // how long to hold button down.
//const char* fwUrlBase = "http://10.0.3.100/";
//const char* fwName = "field_fw";
//bool sendFirmwareVersion = false;             // send firmware via OSC to 8888 for testing


/* -- network settings -- */
uint8_t address = 99;                         // set by DIPs (if you see 99 then it's wrong!)
String deviceName = "SPORE_";                 //

const char* ssid = "Tangible";
const char* password = "nosotros";

//IPAddress staticIP(10, 0, 3, address);        // static IP  - use staticIP[index] to get & set individual octets
//IPAddress gateway(10, 0, 3, 254);             // gateway (for static)
//IPAddress subnet(255, 255, 255, 0);           // subnet (for static)
IPAddress serverIP(10,0,1,100);               // mbp-bm
uint16_t localPort_OSC = 8889;                // OSC listen port
uint16_t remotePort_OSC = 5005;               // OSC send port (this sends to the server now) // was 8888
IPAddress mIP(239, 0, 1, 1);                  // multicast ip address
uint16_t mPort = 7777;                        // multicast port
uint16_t wsPort = 8080;                       // websocket port

String oscAddress = "/battery";               // osc address to send batt voltage
int battInterval = 1000;                      // batt osc send interval
int pingInterval = 60000;                     // websocket ping server interval (server is also pinging me every 30 seconds. this is to help detect disconnect in case of server crash etc.)


struct SensorState {
  uint8_t mode = 1;                           // 1 = all 16 leds; 2 = 4 on each side; 
  float brightness = 0.1f;                    // temporarily just modifying LED brightness in HSB color.
  //uint8_t numLEDs = 16;                     // current valid numbers are 8 and 16
  float sendRate = 20.0f;
  uint32_t timeout = 5;                       // TODO timeout before
  //float easing = 1.0f;                      // coefficient
  float voltage = 3.7f;                       // init to typical.. 
} sensorState;



#endif /* SETTINGS_H */
