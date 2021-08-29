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
  {0, 1},    // edge a 1
  {1, 2},    // edge b 2
  {2, 3},    // edge c 3
  {3, 0},    // edge d 4
  {0, 4},    // edge e 5
  {4, 5},    // edge f 6
  {5, 3},    // edge g 7
  {3, 6},    // edge h 8
  {6, 7},    // edge i 9
  {2, 7},    // edge j 10
  {7, 8},    // edge k 11
  {8, 1},    // edge l 12
  {1, 9},    // edge m 13
  {9, 4},    // edge n 14
  {4, 10},   // edge o 15
  {10, 11},  // edge p 16
  {11, 9},   // edge q 17
  {11, 8},   // edge r 18
  {11, 12},  // edge s 19
  {7, 12},   // edge t 20
  {12, 13},  // edge u 21
  {13, 6},   // edge v 22
  {13, 5},   // edge w 23
  {13, 10},  // edge x 24
};

// okay so here's the scheme --
// each edge maps onto two other sets of edges, one at the beginning of the edge,
// and another at the end.  these two sets are the inner arrays. the numbers in those
// arrays are encoded so that abs(num) - 1 is the edge index, and the sign of the number
// indicates whether this is the beginning or end of that edge.  0 is the null edge.
const int8_t incidentEdges[NUM_EDGES][2][MAX_VERTEX_DEGREE - 1] = {
  {{-4, 5, 0}, {2, -12, 13}},      // edge a
  {{-1, -12, 13}, {3, 10, 0}},     // edge b
  {{-2, 10, 0}, {4, -7, 8}},       // edge c
  {{-3, -7, 8}, {1, 5, 0}},        // edge d
  {{1, -4, 0}, {6, -14, 15}},      // edge e
  {{-5, -14, 15}, {7, -23, 0}},    // edge f
  {{-6, -23, 0}, {-3, 4, 8}},      // edge g
  {{-3, 4, -7}, {9, -22, 0}},      // edge h
  {{-8, -22, 0}, {-10, 11, 20}},   // edge i
  {{-2, 3, 0}, {-9, 11, 20}},      // edge j
  {{-9, -10, 20}, {12, -18, 0}},   // edge k
  {{-11, -18, 0}, {-1, 2, -12}},   // edge l
  {{-1, 2, 13}, {14, -17, 0}},     // edge m
  {{-13, -17, 0}, {-5, 6, 15}},    // edge n
  {{-5, 6, -14}, {16, -24, 0}},    // edge o
  {{-15, -24, 0}, {17, 18, 19}},   // edge p
  {{-16, 18, 19}, {-13, 14, 0}},   // edge q
  {{-16, 17, 19}, {-11, 12, 0}},   // edge r
  {{-16, 17, 18}, {-20, 21, 0}},   // edge s
  {{-9, -10, 11}, {-19, 21, 0}},   // edge t
  {{-19, -20, 0}, {22, 23, 24}},   // edge u
  {{-21, 23, 24}, {-8, 9, 0}},     // edge v
  {{-21, 22, 24}, {-6, 7, 0}},     // edge w
  {{-21, 22, 23}, {-15, 16, 0}},   // edge w
};

#endif
