#include <Arduino.h>
#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>
#include "snake.h"
#include "fireworks.h"

#define LED_PIN           13

CRGB leds[NUM_LEDS];

struct snake snek1;
struct snake snek2;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  FastLED.setMaxPowerInMilliWatts(1000);
  randomSeed(analogRead(A0));
  snek1 = initializeSnake();
  snek2 = initializeSnake();
}

uint8_t mode = 0;

void loop() {
  switch (mode) {
    case 0:
      snekPlz(&snek1, leds);
      snekPlz(&snek2, leds);
      break;
    case 1:
      fireworks(leds);
      break;
    default:
      mode = 0;
  }

  EVERY_N_SECONDS(10) {
    mode++;
  }
}
