#include "body.h"
#include "polygon.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct body {
  list_t *shape;
  double mass;
  rgb_color_t color;
  vector_t centroid;
  vector_t velocity;
  vector_t impulse;
  vector_t force;
  void *info;
  free_func_t info_freer;
  bool is_removed;
  double damage_collisions;
  double angle_facing;
} body_t;

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color,
                            void *info, free_func_t info_freer) {
  assert(mass >= 0);
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);
  body->shape = shape;
  body->mass = mass;
  body->color = color;
  body->centroid = polygon_centroid(shape);
  body->velocity = VEC_ZERO;
  body->impulse = VEC_ZERO;
  body->force = VEC_ZERO;
  body->info = info;
  body->info_freer = info_freer;
  body->is_removed = false;
  body->damage_collisions = 0;
  body->angle_facing = 0;
  return body;
}

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  body_t *body = body_init_with_info(shape, mass, color, NULL, NULL);
  return body;
}

void body_free(body_t *body) {
  if (body->info_freer != NULL) {
    body->info_freer(body->info);
  }
  list_free(body->shape);
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *current_shape = list_init(list_size(body->shape), (free_func_t)free);
  for (size_t i = 0; i < list_size(body->shape); i++) {
    vector_t *point = malloc(sizeof(vector_t));
    *point = *((vector_t *)list_get(body->shape, i));
    list_add(current_shape, point);
  }
  return current_shape;
}

vector_t body_get_centroid(body_t *body) { return body->centroid; }

vector_t body_get_velocity(body_t *body) { return body->velocity; }

rgb_color_t body_get_color(body_t *body) { return body->color; }

double body_get_rotation(body_t *body) { return body->angle_facing; }

void body_set_centroid(body_t *body, vector_t x) {
  polygon_translate(body->shape, vec_subtract(x, body->centroid));
  body->centroid = polygon_centroid(body->shape);
}

void body_set_velocity(body_t *body, vector_t v) { body->velocity = v; }

void body_set_rotation(body_t *body, double angle) {
  polygon_rotate(body->shape, -(body->angle_facing), body->centroid);
  polygon_rotate(body->shape, angle, body->centroid);
  body->angle_facing = angle;
}

void body_add_force(body_t *body, vector_t force) {
  body->force = vec_add(body->force, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulse = vec_add(body->impulse, impulse);
}

double body_get_mass(body_t *body) { return body->mass; }

void body_tick(body_t *body, double dt) {
  vector_t old_v = body->velocity;
  // initialize change in velocity to 0
  vector_t dv = vec_multiply((1.0 / body->mass), body->impulse);
  // add change due to impulse
  // add change due to acceleration
  dv = vec_add(dv, vec_multiply((dt / body->mass), body->force));
  // dv = vec_add(dv, vec_multiply(dt, acceleration));
  // add change in velocity
  vector_t new_v = vec_add(dv, body->velocity);
  body->velocity = new_v;
  vector_t averageV = vec_multiply(0.5, vec_add(old_v, new_v));
  body_set_centroid(body, vec_add(body->centroid, vec_multiply(dt, averageV)));
  body->impulse = VEC_ZERO;
  body->force = VEC_ZERO;
}

void *body_get_info(body_t *body) { return body->info; }

void body_remove(body_t *body) { body->is_removed = true; }

bool body_is_removed(body_t *body) { return body->is_removed == true; }

double body_damage_collisions(body_t *body) { return body->damage_collisions; }

void body_add_damage(body_t *body, double damage) {
  body->damage_collisions += damage;
}