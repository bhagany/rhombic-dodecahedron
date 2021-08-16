#define FASTLED_INTERNAL  // suppress annoying compiler messages from FastLED
#include <FastLED.h>
#define LED_PIN           13
#define NUM_LEDS         288
#define NUM_VERTICES      14
#define MAX_VERTEX_DEGREE  4
#define NUM_EDGES         24
#define LEDS_PER_EDGE     12
#define LED_BUFFER_SIZE  144
#define TRAIL_LENGTH      12

// for accessing edges global
#define START              0
#define END                1
#define HUE                2
#define SATURATION         3
#define VALUE              4
#define DIRECTION          5
#define EDGE_COUNTER       6
#define TRAIL_COUNTER      7

CRGB leds[NUM_LEDS];

// Each array contains the leds adjacent to each vertex in the
// dodecahedron, which is dependent on the way in which the
// data lines are wired up. -1 is "empty", which indicates that
// these vertices are degree 3, rather than the maximum degree
// of 4.
const int16_t vertexLeds[NUM_VERTICES][MAX_VERTEX_DEGREE] = {
  {0, 47, 48, -1},  // 0
  {11, 12, 143, 144},  // 1
  {23, 24, 108, -1},  // 2
  {35, 36, 83, 84},  // 3
  {59, 60, 167, 168},  // 4
  {71, 72, 275, -1},  // 5
  {95, 96, 263, -1},  // 6
  {107, 119, 120, 228},  // 7
  {131, 132, 215, -1},  // 8
  {155, 156, 203, -1},  // 9
  {179, 180, 287, -1},  // 10
  {191, 192, 204, 216},  // 11
  {227, 239, 240, -1},  // 12
  {251, 252, 264, 276},  // 13
};
int16_t edges[NUM_EDGES][8] = {  // start, end, hue, saturation, value, direction, edgecounter, trailcounter
  {0, 1, 0, 0, 0, 0, 0, 0},    // edge a
  {1, 2, 0, 0, 0, 0, 0, 0},    // edge b
  {2, 3, 0, 0, 0, 0, 0, 0},    // edge c
  {3, 0, 0, 0, 0, 0, 0, 0},    // edge d
  {0, 4, 0, 0, 0, 0, 0, 0},    // edge e
  {4, 5, 0, 0, 0, 0, 0, 0},    // edge f
  {5, 3, 0, 0, 0, 0, 0, 0},    // edge g
  {3, 6, 0, 0, 0, 0, 0, 0},    // edge h
  {6, 7, 0, 0, 0, 0, 0, 0},    // edge i
  {2, 7, 0, 0, 0, 0, 0, 0},    // edge j
  {7, 8, 0, 0, 0, 0, 0, 0},    // edge k
  {8, 1, 0, 0, 0, 0, 0, 0},    // edge l
  {1, 9, 0, 0, 0, 0, 0, 0},    // edge m
  {9, 4, 0, 0, 0, 0, 0, 0},    // edge n
  {4, 10, 0, 0, 0, 0, 0, 0},   // edge o
  {10, 11, 0, 0, 0, 0, 0, 0},  // edge p
  {11, 9, 0, 0, 0, 0, 0, 0},   // edge q
  {11, 8, 0, 0, 0, 0, 0, 0},   // edge r
  {11, 12, 0, 0, 0, 0, 0, 0},  // edge s
  {7, 12, 0, 0, 0, 0, 0, 0},   // edge t
  {12, 13, 0, 0, 0, 0, 0, 0},  // edge u
  {13, 6, 0, 0, 0, 0, 0, 0},   // edge v
  {13, 5, 0, 0, 0, 0, 0, 0},   // edge w
  {13, 10, 0, 0, 0, 0, 0, 0},  // edge x
};

void fireEdge(uint8_t idx, uint8_t hue, uint8_t saturation, uint8_t value, uint8_t trailCounter, int8_t dir) {
  edges[idx][HUE] = hue;
  edges[idx][SATURATION] = saturation;
  edges[idx][VALUE] = value;
  edges[idx][DIRECTION] = dir;
  edges[idx][EDGE_COUNTER] = 0;
  edges[idx][TRAIL_COUNTER] = trailCounter;
}

void fireVertex(uint8_t vertex, uint8_t hue, uint8_t saturation, uint8_t value, uint8_t trailCounter, uint8_t sourceEdge) {
  for (uint8_t i=0; i<NUM_EDGES; i++) {
    if (i != sourceEdge && (edges[i][START] == vertex || edges[i][END] == vertex)) {
      int8_t dir;
      if (edges[i][START] == vertex) {
        dir = 1;
      } else {
        dir = -1;
      }
      fireEdge(i, hue, saturation, value, trailCounter, dir);
    }
  }
}

void fireNewVertex() {
  uint8_t vertex = random(NUM_VERTICES);
  uint8_t hue = random(255);
  uint8_t saturationOffset = random(196);
  uint8_t saturation = 255 - saturationOffset;
  uint8_t valueOffset = random(64);
  uint8_t value = 255 - valueOffset;
  fireVertex(vertex, hue, saturation, value, TRAIL_LENGTH, NULL);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  FastLED.setMaxPowerInMilliWatts(900);
  randomSeed(analogRead(A0));
  FastLED.clear();
}

void loop() {
  EVERY_N_MILLISECONDS(100) {
    bool fire = random(5) == 0;
    if (fire) {
      fireNewVertex();
    }
  }

  EVERY_N_MILLISECONDS(20) {
    for (uint8_t i=0; i<NUM_EDGES; i++) {
      if (edges[i][TRAIL_COUNTER] > 0) {
        uint16_t led;
        if (edges[i][DIRECTION] == 1) {
          led = (i * LEDS_PER_EDGE) + edges[i][EDGE_COUNTER];
        } else if (edges[i][DIRECTION] == -1) {
          led = ((i + 1) * LEDS_PER_EDGE) - edges[i][EDGE_COUNTER] - 1;
        }
        uint8_t value = map(TRAIL_LENGTH - edges[i][TRAIL_COUNTER], 0, TRAIL_LENGTH, 48, edges[i][VALUE]);
        leds[led] = CHSV(edges[i][HUE], edges[i][SATURATION], value);
        Serial.println(led);
        edges[i][TRAIL_COUNTER]--;
        edges[i][EDGE_COUNTER]++;
        //if (edges[i][TRAIL_COUNTER] > 0 && edges[i][EDGE_COUNTER] == LEDS_PER_EDGE) {
        //  Serial.print("Finished edge ");
        //  Serial.print(i);
        //  Serial.print(", firing vertex ");
        //  if (edges[i][DIRECTION] == 1) {
        //    Serial.println(edges[i][END]);
        //    fireVertex(edges[i][END], edges[i][HUE], edges[i][SATURATION], value, edges[i][TRAIL_COUNTER], i);
        //  } else if (edges[i][DIRECTION] == -1) {
        //    Serial.println(edges[i][START]);
        //    fireVertex(edges[i][START], edges[i][HUE], edges[i][SATURATION], edges[i][VALUE], edges[i][TRAIL_COUNTER], i);
        //  }
        //}
      }
    }
    fadeToBlackBy(leds, NUM_LEDS, 16);
    FastLED.show();
  }
}
