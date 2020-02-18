#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 21

CRGB led[NUM_LEDS];

int commits[21] = {0, 1, 0, 0, 3, 1, 22, 5, 7, 2, 5, 0, 0, 50, 12, 8, 1, 0, 4, 34, 0};

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {

    if (commits[i] == 0) {
      led[i] = CRGB(0, 0, 0);
      FastLED.show();
    } else if (commits[i] >= 1 && commits[i] < 5) {
      led[i] = CRGB(0, 20, 0);
      FastLED.show();
    } else if (commits[i] >= 5 && commits[i] < 10) {
      led[i] = CRGB(0, 80, 0);
      FastLED.show();
    } else if (commits[i] >= 10 && commits[i] < 20) {
      led[i] = CRGB(0, 255, 0);
      FastLED.show();
    } else if (commits[i] >= 20 && commits[i] < 100) {
      led[i] = CRGB(255, 255, 0);
      FastLED.show();
    } else {
      led[i] = CRGB(0, 0, 100);
      FastLED.show();
    }
  }

  //// Red
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(255, 0, 0);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// Orange
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(255, 128, 0);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //
  //// Yellow
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(255, 255, 0);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// Green
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(0, 255, 0);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// Blue
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(0, 0, 255);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// Violet
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(153, 0, 153);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// Pink
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(255, 51, 255);
  //  FastLED.show();
  //  delay(50);
  //}
  //
  //// White
  //for(int i = 0; i < NUM_LEDS; i++){
  //  led[i] = CRGB(255,255, 255);
  //  FastLED.show();
  //  delay(5);
  //}

}