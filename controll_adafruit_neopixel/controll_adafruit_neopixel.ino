#include <Adafruit_NeoPixel.h>

#define DATA_PIN 7
#define NUM_LEDS 21

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    strip.begin();
    strip.clear();
    strip.show();
}

void loop()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 150, 0));
        strip.show();
        delay(50);
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
        delay(50);
    }
}