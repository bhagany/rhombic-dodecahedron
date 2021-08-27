#include <Arduino.h>
#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>
#include "snake.h"
#define LED_PIN           13

CRGB leds[NUM_LEDS];

struct snake snek1;
struct snake snek2;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  FastLED.setMaxPowerInMilliWatts(900);
  randomSeed(analogRead(A0));
  snek1 = initializeSnake();
  snek2 = initializeSnake();
}

void loop() {
  snekPlz(&snek1, leds);
  snekPlz(&snek2, leds);
}
