#ifndef __RHOMBIC_DODECAHEDRON_DEFS__
#define __RHOMBIC_DODECAHEDRON_DEFS__

#include <Arduino.h>
#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>

#define NUM_LEDS         288
#define NUM_VERTICES      14
#define MAX_VERTEX_DEGREE  4
#define NUM_EDGES         24
#define LEDS_PER_EDGE     12
#define NULL_LED          -1
#define NULL_EDGE         -1

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

const int8_t edges[NUM_EDGES][2] = {  // start vertex, end vertex
  {0, 1},    // edge a
  {1, 2},    // edge b
  {2, 3},    // edge c
  {3, 0},    // edge d
  {0, 4},    // edge e
  {4, 5},    // edge f
  {5, 3},    // edge g
  {3, 6},    // edge h
  {6, 7},    // edge i
  {2, 7},    // edge j
  {7, 8},    // edge k
  {8, 1},    // edge l
  {1, 9},    // edge m
  {9, 4},    // edge n
  {4, 10},   // edge o
  {10, 11},  // edge p
  {11, 9},   // edge q
  {11, 8},   // edge r
  {11, 12},  // edge s
  {7, 12},   // edge t
  {12, 13},  // edge u
  {13, 6},   // edge v
  {13, 5},   // edge w
  {13, 10},  // edge x
};

#endif
