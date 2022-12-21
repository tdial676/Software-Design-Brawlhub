#include "color.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

const int RGB_FACTOR = 1000;

rgb_color_t random_color() {
  float R = ((float)(rand() % RGB_FACTOR)) / RGB_FACTOR;
  float G = ((float)(rand() % RGB_FACTOR)) / RGB_FACTOR;
  float B = ((float)(rand() % RGB_FACTOR)) / RGB_FACTOR;
  return (rgb_color_t){R, G, B, 1.0};
}

rgb_color_t switch_color(int num) {
  switch (num) {
  case 1: {
    return (rgb_color_t){1.0, 0.0, 0.0, 1.0};
  }
  case 2: {
    return (rgb_color_t){1.0, 0.5, 0.0, 1.0};
  }
  case 3: {
    return (rgb_color_t){1.0, 1.0, 0.0, 1.0};
  }
  case 4: {
    return (rgb_color_t){0.0, 1.0, 0.0, 1.0};
  }
  case 5: {
    return (rgb_color_t){0.0, 0.0, 1.0, 1.0};
  }
  case 6: {
    return (rgb_color_t){0.5, 0.0, 1.0, 1.0};
  }
  case 7: {
    return (rgb_color_t){0.5, 0.0, 1.0, 1.0};
  }
  case 8: { // WHITE
    return (rgb_color_t){1.0, 1.0, 1.0, 1.0};
  }
  default: { // BLACK
    return (rgb_color_t){0.0, 0.0, 0.0, 1.0};
  }
  }
}

SDL_Color rgb_to_SDL(rgb_color_t color) {
  return (SDL_Color){.r = color.r * 255,
                     .g = color.g * 255,
                     .b = color.b * 255,
                     .a = color.a * 255};
}