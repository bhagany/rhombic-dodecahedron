#include <Arduino.h>
#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>
#define LED_PIN           13
#define NUM_LEDS         288
#define NUM_VERTICES      14
#define MAX_VERTEX_DEGREE  4
#define LEDS_PER_EDGE     12

CRGB leds[NUM_LEDS];

// Each array contains the leds adjacent to each vertex in the
// dodecahedron, which is dependent on the way in which the
// data lines are wired up. -1 is "empty", which indicates that
// these vertices are degree 3, rather than the maximum degree
// of 4.
const int16_t vertexLeds[NUM_VERTICES][MAX_VERTEX_DEGREE] = {
  {0, 47, 48, -1},
  {11, 12, 143, 144},
  {23, 24, 108, -1},
  {35, 36, 83, 84},
  {59, 60, 167, 168},
  {71, 72, 275, -1},
  {95, 96, 263, -1},
  {107, 119, 120, 228},
  {131, 132, 215, -1},
  {155, 156, 203, -1},
  {179, 180, 287, -1},
  {191, 192, 204, 216},
  {227, 239, 240, -1},
  {251, 252, 264, 276},
};
uint16_t led;
bool newVertex;
int8_t edgeDirection;
uint8_t hue = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  FastLED.setMaxPowerInMilliWatts(900);
  randomSeed(analogRead(A0));
  led = random(NUM_LEDS);
  newVertex = false;
  const int8_t edgeChoices[] = {-1, 1};
  edgeDirection = edgeChoices[random(1)];
}

void loop() {
  leds[led] = CHSV(hue, 255, 255);
  FastLED.show();

  // I have no idea what this is, except that it uses interrupts
  EVERY_N_MILLISECONDS(30) {
    if (!newVertex
        && (led % LEDS_PER_EDGE == 0
            || led % LEDS_PER_EDGE == LEDS_PER_EDGE - 1)) {
      // We have encountered a vertex
      bool foundVertex = false;
      int16_t otherVertexLeds[MAX_VERTEX_DEGREE - 1];
      uint8_t numValidValues = 0;

      // Find which vertex we hit
      for (uint8_t v=0; v<NUM_VERTICES; v++) {
        for (uint8_t l=0; l<MAX_VERTEX_DEGREE; l++) {
          if (led == vertexLeds[v][l]) {
            foundVertex = true;
            break;
          }
        }
        if (foundVertex) {
          uint8_t idx = 0;

          // Gather the other LEDs that are adjacent to this vertex
          // to choose between
          for (uint8_t l=0; l<MAX_VERTEX_DEGREE; l++) {
            if (vertexLeds[v][l] != led) {
              otherVertexLeds[idx] = vertexLeds[v][l];
              idx++;
              // Ignore "empty" entries
              if (vertexLeds[v][l] > 0) {
                numValidValues++;
              }
            }
          }
          break;
        }
      }
      led = otherVertexLeds[random(numValidValues)];
      if (led % LEDS_PER_EDGE == 0) {
        // we're at the beginning of an edge, and need to count up to travel along it
        edgeDirection = 1;
      } else if (led % LEDS_PER_EDGE == LEDS_PER_EDGE - 1) {
        // we're at the end of an edge, and need to count down to travel along it
        edgeDirection = -1;
      }
      // we are still at a vertex, but we are leaving it; this flag ensures
      // that we travel along the chosen edge instead of going back and choosing
      // a new edge
      newVertex = true;
    } else {
      // we're not at a vertex, so continue along the edge
      led += edgeDirection;
      // ensure that the next vertex encounter results in an edge choice
      newVertex = false;
    }
    hue++;
    fadeToBlackBy(leds, NUM_LEDS, 16);
  }
}
