#ifndef __RHOMBIC_DODECAHEDRON_FLOOD_VERTEX__
#define __RHOMBIC_DODECAHEDRON_FLOOD_VERTEX__

#include "defs.h"

// for accessing edges and fillProgress globals
#define START              0
#define END                1
#define FV_DIRECTION       0
#define FV_EDGE_COUNTER    1


// direction, edgecounter
int16_t fillProgress[NUM_EDGES][2] = {{1, LEDS_PER_EDGE}};

void fillFromVertex(CRGB *leds) {
  uint8_t vertex;
  uint8_t hue;
  uint8_t saturation;
  uint8_t value;
  EVERY_N_MILLISECONDS(50) {
    bool restart = true;
    for (uint8_t i=0; i<NUM_EDGES; i++) {
      if (fillProgress[i][FV_EDGE_COUNTER] < LEDS_PER_EDGE) {
        restart = false;
      }
    }
    if (restart) {
      vertex = random(NUM_VERTICES);
      hue = random(255);
      uint8_t saturationOffset = random(196);
      saturation = 255 - saturationOffset;
      uint8_t valueOffset = random(64);
      value = 255 - valueOffset;
      for (uint8_t i=0; i<NUM_EDGES; i++) {
        if (edges[i][START] == vertex) {
          fillProgress[i][FV_DIRECTION] = 1;
          fillProgress[i][FV_EDGE_COUNTER] = 0;
        } else if (edges[i][END] == vertex) {
          fillProgress[i][FV_DIRECTION] = -1;
          fillProgress[i][FV_EDGE_COUNTER] = 0;
        } else {
          fillProgress[i][FV_EDGE_COUNTER] = -1;
        }
      }
    }
    for (uint8_t i=0; i<NUM_EDGES; i++) {
      if (fillProgress[i][FV_EDGE_COUNTER] > -1 && fillProgress[i][FV_EDGE_COUNTER] < LEDS_PER_EDGE) {
        uint16_t led;
        if (fillProgress[i][FV_DIRECTION] == 1) {
          led = (i * LEDS_PER_EDGE) + fillProgress[i][FV_EDGE_COUNTER];
        } else if (fillProgress[i][FV_DIRECTION] == -1) {
          led = ((i + 1) * LEDS_PER_EDGE) - fillProgress[i][FV_EDGE_COUNTER] - 1;
        }
        leds[led] = CHSV(hue, saturation, value);
        FastLED.show();
        fillProgress[i][FV_EDGE_COUNTER]++;
        if (fillProgress[i][FV_EDGE_COUNTER] == LEDS_PER_EDGE) {
          uint8_t incidenceIndex = max(0, fillProgress[i][FV_DIRECTION]);
          for (uint8_t j=0; j<MAX_VERTEX_DEGREE - 1; j++) {
            int8_t edgeish = incidentEdges[i][incidenceIndex][j];
            if (edgeish == 0) {
              continue;
            }
            uint8_t absEdgeish = abs(edgeish);
            uint8_t nextEdge = absEdgeish - 1;
            if (fillProgress[nextEdge][FV_EDGE_COUNTER] < 0) {
              int8_t nextDirection = edgeish / absEdgeish;
              fillProgress[nextEdge][FV_DIRECTION] = nextDirection;
              fillProgress[nextEdge][FV_EDGE_COUNTER] = 0;
            }
          }
        }
      }
    }
  }
}

#endif
