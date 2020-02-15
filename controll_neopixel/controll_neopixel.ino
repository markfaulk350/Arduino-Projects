#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 3

CRGB led[NUM_LEDS];

void setup() {
  // FastLED.addLeds<WS2812, LED_PIN, GRB>(led, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
}

void loop() {
  led[0] = CRGB(100, 0, 0);
  FastLED.show();
  delay(500);

  led[1] = CRGB(0, 100, 0);
  FastLED.show();
  delay(500);

  led[2] = CRGB(0, 0, 100);
  FastLED.show();
  delay(500);

  led[0] = CRGB(0, 255, 0);
  FastLED.show();
  delay(500);

  led[1] = CRGB(0, 0, 255);
  FastLED.show();
  delay(500);

  led[2] = CRGB(255, 0, 0);
  FastLED.show();
  delay(500);
}