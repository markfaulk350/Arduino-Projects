#include <FastLED.h>

#define LED_PIN 7  // Arduino Uno digital pin connecting to WS2812 data-in
#define NUM_LEDS 3 // # of WS2812 leds on strip

// int commits[] = {0, 1, 5};

// 1st (235, 234, 240)
// 2nd (155, 226, 167)
// 3rd (84, 205, 101)
// 4th (56, 156, 68)
// 5th (33, 96, 42)

CRGB led[NUM_LEDS];

void setup()
{
  // FastLED.addLeds<WS2812, LED_PIN, GRB>(led, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  led[0] = CRGB(255, 0, 0);
    FastLED.show();
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    led[i] = CRGB(255, 0, 0);
    FastLED.show();
    delay(100);
  }

  for (int i = 0; i < 3; i++)
  {
    led[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(100);
  }

  // led[0] = CRGB(235, 234, 240);
  // FastLED.show();
  // delay(500);

  // led[1] = CRGB(155, 226, 167);
  // FastLED.show();
  // delay(500);

  // led[2] = CRGB(84, 205, 101);
  // FastLED.show();
  // delay(500);

  // led[0] = CRGB(56, 156, 68);
  // FastLED.show();
  // delay(500);

  // led[1] = CRGB(33, 96, 42);
  // FastLED.show();
  // delay(500);

  // led[2] = CRGB(235, 234, 240);
  // FastLED.show();
  // delay(500);
}