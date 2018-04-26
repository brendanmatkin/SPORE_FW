// esp8266 2.4.1-rc2

#include "settings.h"
#include "comm_ws.h"
#include "comm_osc.h"
#include "FS.h"
#include <NeoPixelBus.h>


#define LED_BUILTIN   2                 // not correctly mapped for ESP-12x
#define BOOTLOAD_PIN  0                 // BOOTLOAD button

const uint16_t PixelCount = 16;                                   // DO NOT CHANGE!!
NeoGamma<NeoGammaTableMethod> colorGamma;                         // for any fade animations, best to correct gamma
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, 3);

WiFiUDP udp;
WiFiUDP udpSend;                                // this is to keep the multicast subscription alive I guess? Doesn't work without it anyway...

uint32_t sendTimer;                             // osc send timer;
unsigned long period = (int)(1000.0f / 60.0f);
float hue;



void setup() {
  Serial.begin(115200);
  Serial.printf("\n");
  delay(500);
  Serial.printf("\n\n\nSPORE\n");
  Serial.printf("FW: %u%s   ", FW_VERSION, FW_PHASE);
  Serial.printf("HW: %s%s\n", HW_VERSION, HW_PHASE);
  Serial.printf("(c)2017 Tangible Interaction Design Inc.\n\n");    // pick open source license
  Serial.flush();

  /* begin SPIFFS */
  //  Serial.print("Mounting file system...");
  //  if (!SPIFFS.begin()) {
  //    Serial.println("Failed to mount file system");
  //    return;
  //  }
  //  Serial.println(" mounted.");

  /* init LED PIN */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // low is on

  networkInit();                            // connect to wifi, init osc and websockets, print status.

  //EEPROM.begin(512);
  //EEPROM.get(addr, val);

  /* LEDs */
  delay(100);
  strip.Begin();
  strip.Show();

  sendTimer = millis();
  yield();
}



void loop() {

  if (ENABLE_WEBSOCKET) webSocket.loop();
  static unsigned long pingTimer;
  if (millis() - pingTimer > pingInterval) {
    if (ENABLE_WEBSOCKET) webSocket.sendPing();                    // ping server to recognize a dropped TCP connection quickly
    udpSend.beginPacketMulticast(mIP, mPort, WiFi.localIP());      // keep local multicast subscription alive.. maybe a bug? this fixes it.
    udpSend.write('0');
    udpSend.endPacket();
    pingTimer = millis();
  }


  /**** HARDWARE check for firmware update (server): ****/
  static unsigned long buttonTimer;
  if (digitalRead(BOOTLOAD_PIN) == HIGH) {
    buttonTimer = millis();         // reset timer
  }
  else if (digitalRead(BOOTLOAD_PIN) == LOW && millis() - buttonTimer >= fwCheckButtonTime) {
    digitalWrite(LED_BUILTIN, LOW);
//    ota.checkServer(fwUrlBase, fwName, FW_VERSION);       // (( Server OTA ))
//    otaTimer = millis();
    if (SERIAL_DEBUG) Serial.println("[OTA] check fw (button)");
  }
  digitalWrite(LED_BUILTIN, HIGH);


  /* LED CONTROL/Animation: */
  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    // All 16 LEDs:
    switch (sensorState.mode) {
      case 1: {
          RgbColor color = HsbColor(hue / 1000.0f, 1.0f, sensorState.brightness);
          //strip.SetPixelColor(i, color);
          strip.SetPixelColor(i, colorGamma.Correct(color));
          break;
        }
      case 2: {
          if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15) {
            RgbColor color = HsbColor(hue / 1000.0f, 1.0f, sensorState.brightness);
            //strip.SetPixelColor(i, color);
            strip.SetPixelColor(i, colorGamma.Correct(color));
          }
          else {
            strip.SetPixelColor(i, RgbColor(0));
          }
          break;
        }
      default:
        break;
    }
  }
  strip.Show();
  static unsigned long lastUpdate;
  if (millis() - lastUpdate > period) {
    lastUpdate = millis();
    hue = (hue == 1000.0f) ? 0.0f : (hue += 1.0f);
  }


  /* battery voltage: */
  float vRaw = analogRead(A0) / 1023.0f * 4.03f;  // ideally *4.2f. 4.03 is the max measurable by current voltage divider. May vary with resistor tolerance..
  float coef = 0.1;
  sensorState.voltage = vRaw * coef + (1.0f - coef) * sensorState.voltage;
  //Serial.println(sensorState.voltage);


  /* OSC: */
  if (WiFi.isConnected()) {
    /**** OSC out ****/
    if (millis() - sendTimer > (1000.0f/sensorState.sendRate)) {
      //String _oscAddress = oscAddress + "/dist";
      String _oscAddress = oscAddress;
      OSCMessage msg(_oscAddress.c_str());
        msg.add((int)address);
        msg.add(sensorState.voltage);
        // uint16_t bytesSent = msg.bytes();
        udp.beginPacket(serverIP, remotePort_OSC);
        msg.send(udp);
        bool oscSent = udp.endPacket();
        msg.empty();
      sendTimer = millis();
      if (SERIAL_DEBUG && !oscSent) {
        Serial.printf("[OSC] Socket error on write: ");
        Serial.print(serverIP); Serial.printf(":%u\n", remotePort_OSC);
      }
    }
    /**** OSC in ****/
    OSCMessage inMsg;
    int udpSize = udp.parsePacket();
    if (udpSize > 0) {
      //if (SERIAL_DEBUG) Serial.printf("[UDP] Got UDP: %i\n", udpSize);      // DELETE ME
      while (udpSize--) {
        inMsg.fill(udp.read());
      }
      if (!inMsg.hasError()) {
        //inMsg.route("/sendRate", setSendRate);
        inMsg.route("/brightness", setBrightness);
        inMsg.route("/mode", setMode);
      }
    }
  }

  yield();
}





