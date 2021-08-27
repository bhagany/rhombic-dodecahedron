#include <Arduino.h>
#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>

#define NUM_LEDS         288
#define NUM_VERTICES      14
#define MAX_VERTEX_DEGREE  4
#define LEDS_PER_EDGE     12
#define NULL_LED          -1

// Each array contains the leds adjacent to each vertex in the
// dodecahedron, which is dependent on the way in which the
// data lines are wired up. Vertices of degree 3 have null entries
const int16_t vertexLeds[NUM_VERTICES][MAX_VERTEX_DEGREE] = {
  {0, 47, 48, NULL_LED},
  {11, 12, 143, 144},
  {23, 24, 108, NULL_LED},
  {35, 36, 83, 84},
  {59, 60, 167, 168},
  {71, 72, 275, NULL_LED},
  {95, 96, 263, NULL_LED},
  {107, 119, 120, 228},
  {131, 132, 215, NULL_LED},
  {155, 156, 203, NULL_LED},
  {179, 180, 287, NULL_LED},
  {191, 192, 204, 216},
  {227, 239, 240, NULL_LED},
  {251, 252, 264, 276},
};
