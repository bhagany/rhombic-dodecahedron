#ifndef __RHOMBIC_DODECAHEDRON_SNAKE__
#define __RHOMBIC_DODECAHEDRON_SNAKE__

#include "defs.h"

struct snake {
  int16_t led;
  bool newVertex;
  int8_t edgeDirection;
  uint8_t hue;
};

struct snake initializeSnake() {
  struct snake snek;
  snek.led = random(NUM_LEDS);
  snek.newVertex = false;
  const int8_t edgeChoices[] = {-1, 1};
  snek.edgeDirection = edgeChoices[random(1)];
  snek.hue = random(255);

  return snek;
}

void snekPlz(struct snake *snek, CRGB *leds) {
  leds[snek->led] = CHSV(snek->hue, 255, 255);
  FastLED.show();

  // I have no idea what this is, except that it uses interrupts
  EVERY_N_MILLISECONDS(30) {
    if (!snek->newVertex
        && (snek->led % LEDS_PER_EDGE == 0
            || snek->led % LEDS_PER_EDGE == LEDS_PER_EDGE - 1)) {
      // We have encountered a vertex
      bool foundVertex = false;
      int16_t otherVertexLeds[MAX_VERTEX_DEGREE - 1];
      uint8_t numValidValues = 0;

      // Find which vertex we hit
      for (uint8_t v=0; v<NUM_VERTICES; v++) {
        for (uint8_t l=0; l<MAX_VERTEX_DEGREE; l++) {
          if (snek->led == vertexLeds[v][l]) {
            foundVertex = true;
            break;
          }
        }
        if (foundVertex) {
          uint8_t idx = 0;

          // Gather the other LEDs that are adjacent to this vertex
          // to choose between
          for (uint8_t l=0; l<MAX_VERTEX_DEGREE; l++) {
            if (vertexLeds[v][l] != snek->led) {
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
      snek->led = otherVertexLeds[random(numValidValues)];
      if (snek->led % LEDS_PER_EDGE == 0) {
        // we're at the beginning of an edge, and need to count up to travel along it
        snek->edgeDirection = 1;
      } else if (snek->led % LEDS_PER_EDGE == LEDS_PER_EDGE - 1) {
        // we're at the end of an edge, and need to count down to travel along it
        snek->edgeDirection = -1;
      }
      // we are still at a vertex, but we are leaving it; this flag ensures
      // that we travel along the chosen edge instead of going back and choosing
      // a new edge
      snek->newVertex = true;
    } else {
      // we're not at a vertex, so continue along the edge
      snek->led += snek->edgeDirection;
      // ensure that the next vertex encounter results in an edge choice
      snek->newVertex = false;
    }
    snek->hue++;
    fadeToBlackBy(leds, NUM_LEDS, 16);
  }
}

#endif
