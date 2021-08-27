#ifndef __RHOMBIC_DODECAHEDRON_FIREWORKS__
#define __RHOMBIC_DODECAHEDRON_FIREWORKS__

#include "defs.h"

#define TRAIL_LENGTH      12

// for accessing edges and edge_info globals
#define START              0
#define END                1
#define HUE                0
#define SATURATION         1
#define VALUE              2
#define DIRECTION          3
#define EDGE_COUNTER       4
#define TRAIL_COUNTER      5

// hue, saturation, value, direction, edgecounter, trailcounter
int16_t edge_info[NUM_EDGES][6] = {{0}};

void fireEdge(uint8_t idx, uint8_t hue, uint8_t saturation, uint8_t value, uint8_t trailCounter, int8_t dir) {
  edge_info[idx][HUE] = hue;
  edge_info[idx][SATURATION] = saturation;
  edge_info[idx][VALUE] = value;
  edge_info[idx][DIRECTION] = dir;
  edge_info[idx][EDGE_COUNTER] = 0;
  edge_info[idx][TRAIL_COUNTER] = trailCounter;
}

void fireVertex(uint8_t vertex, uint8_t hue, uint8_t saturation, uint8_t value, uint8_t trailCounter, int8_t sourceEdge) {
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
  fireVertex(vertex, hue, saturation, value, TRAIL_LENGTH, NULL_EDGE);
}

void fireworks(CRGB *leds) {
  EVERY_N_MILLISECONDS(100) {
    bool fire = random(5) == 0;
    if (fire) {
      fireNewVertex();
    }
  }

  EVERY_N_MILLISECONDS(20) {
    for (uint8_t i=0; i<NUM_EDGES; i++) {
      if (edge_info[i][TRAIL_COUNTER] > 0) {
        uint16_t led;
        if (edge_info[i][DIRECTION] == 1) {
          led = (i * LEDS_PER_EDGE) + edge_info[i][EDGE_COUNTER];
        } else if (edge_info[i][DIRECTION] == -1) {
          led = ((i + 1) * LEDS_PER_EDGE) - edge_info[i][EDGE_COUNTER] - 1;
        }
        uint8_t value = map(TRAIL_LENGTH - edge_info[i][TRAIL_COUNTER], 0, TRAIL_LENGTH, 48, edge_info[i][VALUE]);
        leds[led] = CHSV(edge_info[i][HUE], edge_info[i][SATURATION], value);
        edge_info[i][TRAIL_COUNTER]--;
        edge_info[i][EDGE_COUNTER]++;
        //if (edge_info[i][TRAIL_COUNTER] > 0 && edge_info[i][EDGE_COUNTER] == LEDS_PER_EDGE) {
        //  Serial.print("Finished edge ");
        //  Serial.print(i);
        //  Serial.print(", firing vertex ");
        //  if (edge_info[i][DIRECTION] == 1) {
        //    Serial.println(edges[i][END]);
        //    fireVertex(edges[i][END], edge_info[i][HUE], edge_info[i][SATURATION], value, edge_info[i][TRAIL_COUNTER], i);
        //  } else if (edge_info[i][DIRECTION] == -1) {
        //    Serial.println(edges[i][START]);
        //    fireVertex(edges[i][START], edge_info[i][HUE], edge_info[i][SATURATION], edge_info[i][VALUE], edge_info[i][TRAIL_COUNTER], i);
        //  }
        //}
      }
    }
    fadeToBlackBy(leds, NUM_LEDS, 16);
    FastLED.show();
  }
}

#endif
