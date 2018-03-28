#include <NeoPixelBus.h>

const uint16_t PixelCount = 16;
NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

#define BRIGHTNESS      0.5f
#define ALL_16_LEDS     true


void setup() {
  strip.Begin();
  strip.Show();
}


unsigned long period = (int)(1000.0f/60.0f);
float hue;

void loop() {
  for (uint16_t i = 0; i < strip.PixelCount(); i++) {
    // All 16 LEDs:
    if (ALL_16_LEDS) {
      RgbColor color = HsbColor(hue/1000.0f, 1.0f, BRIGHTNESS);
      //strip.SetPixelColor(i, color);
      strip.SetPixelColor(i, colorGamma.Correct(color));
    }
    // Half LEDs (4 per side): 
    else {
      if (i == 0 || i == 2 || i == 5 || i == 7 || i == 8 || i == 10 || i == 13 || i == 15) {
        RgbColor color = HsbColor(hue/1000.0f, 1.0f, BRIGHTNESS);
        //strip.SetPixelColor(i, color);
        strip.SetPixelColor(i, colorGamma.Correct(color));
      }
      else {
        strip.SetPixelColor(i, RgbColor(0));
      }
    }
  }
  strip.Show();
  
  static unsigned long lastUpdate;
  if (millis() - lastUpdate > period) {
    lastUpdate = millis();
    hue = (hue==1000.0f) ? 0.0f : (hue+=1.0f);
  }
}

