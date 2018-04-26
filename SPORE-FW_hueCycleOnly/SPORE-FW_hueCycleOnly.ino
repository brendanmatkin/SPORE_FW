#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include "FastLED.h"

#define NUM_LEDS 16
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

void setup() {
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall(int amt) { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(amt); } }

unsigned long period = (int)1000.0/60.0;
void loop() {
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
//    if (i % 2 == 0) leds[i] = CHSV(hue, 255, 255);
//    else            leds[i] = CRGB::Black;
//    if (i == 0 || i == 2 || i == 5 || i == 7) {
//      leds[i] = CHSV(hue, 255, 255);
//    }
//    else {
//      leds[i] = CRGB(1, 1, 1);
//    }
    //FastLED.show(); 
    //fadeall(240);
    //delay(10);
  }
  static unsigned long lastUpdate;
  if (millis() - lastUpdate > period) {
    lastUpdate = millis();
    hue++;
  }
  FastLED.show(); 
  yield();
}
