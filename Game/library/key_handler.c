#include "key_handler.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const vector_t WINDOW_CENTER = (vector_t){500, 250};
const double SCENE_TO_WINDOW = 2;

vector_t get_mouse() {
  int x_position, y_position;
  SDL_GetMouseState(&x_position, &y_position);
  return (vector_t){x_position, y_position};
}

void fob_move(character_t *player, double fob) {
  assert(fob == 1 || fob == -1);
  character_set_velocity(
      player, (vector_t){body_get_velocity(character_get_body(player)).x,
                         fob * USER_SPEED});
}

void lor_move(character_t *player, double lor) {
  assert(lor == -1 || lor == 1);
  character_set_velocity(
      player, (vector_t){lor * USER_SPEED,
                         body_get_velocity(character_get_body(player)).y});
}

double get_rotation_character(character_t *player) {
  vector_t user_center = body_get_centroid(character_get_body(player));
  vector_t mouse_pos = get_mouse();
  vector_t mouse_scaled = vec_subtract(mouse_pos, WINDOW_CENTER);
  mouse_scaled = vec_add(user_center, mouse_scaled);
  vector_t direction = vec_subtract(mouse_scaled, user_center);
  if (direction.x != 0) {
    double angle = atan2(-direction.y, direction.x);
    return angle;
  }
  return 0.0;
}

double get_rotation_body(body_t *player) {
  vector_t user_center = body_get_centroid(player);
  vector_t mouse_pos = get_mouse();
  vector_t mouse_scaled = vec_subtract(mouse_pos, WINDOW_CENTER);
  mouse_scaled = vec_add(user_center, mouse_scaled);
  vector_t direction = vec_subtract(mouse_scaled, user_center);
  if (direction.x != 0) {
    double angle = atan2(-direction.y, direction.x);
    return angle;
  }
  return 0.0;
}