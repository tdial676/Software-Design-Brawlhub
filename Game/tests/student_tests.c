// TODO: IMPLEMENT YOUR TESTS IN THIS FILE
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "body.h"
#include "forces.h"
#include "test_util.h"

const int MASS = 10;
const double G = 10.0;
const int RADI = 10;
const double K = 2.0;
const int ANGLE = 2 * M_PI;
double GAMMA = 1.0;
const int GRANULARITY = 100;
const double dt = 1.0;
const rgb_color_t COLOR = {1, 0.8, 0};

body_t *make_sector(size_t radius, vector_t *center, float angle, double mass) {
  list_t *vertices = list_init(GRANULARITY + 1, (free_func_t)free);
  vector_t radius_vector = {center->x + radius, center->y};
  for (size_t i = 0; i < GRANULARITY; i++) {
    vector_t *added_point =
        malloc(sizeof(vector_t)); // Need these two lines since list_add will
                                  // only accept vector pointers, not vectors
    *added_point = vec_add(vec_rotate(vec_subtract(radius_vector, *center),
                                      i * angle / GRANULARITY),
                           *center);
    list_add(vertices, added_point);
  }
  return body_init(vertices, mass, COLOR);
}

// calculate velocity change due to drag.
double calculate_velocity_change(double dt, double mass) {
  double velocity = 10 * exp(-dt / mass);
  return velocity;
}

// calculate momentum
double calculate_momentum(double mass, double velocity) {
  return (mass * velocity);
}

// calculates change in spring velocity
double calculate_spring_velocity(double k, double mass, double dt) {
  double amp = 20.0;
  double freq = sqrt(k / mass);
  return (amp * freq * -sin(freq * dt));
}

double calculate_spring_position(double k, double mass, double dt) {
  double amp = 20.0;
  double freq = sqrt(k / mass);
  return 100 + (amp * cos(freq * dt)); // position with respect to anchor!!!
}

int main() {
  // Makes five shapes to test
  vector_t *cent1 = malloc(sizeof(*cent1));
  *cent1 = (vector_t){10, 10};
  vector_t *cent2 = malloc(sizeof(*cent2));
  *cent2 = (vector_t){30, 30};
  vector_t *cent3 = malloc(sizeof(*cent3));
  *cent3 = (vector_t){50, 50};
  vector_t *cent4 = malloc(sizeof(*cent4));
  *cent4 = (vector_t){100, 100};
  vector_t *cent5 = malloc(sizeof(*cent5));
  *cent5 = (vector_t){120, 100};
  body_t *shape1 = make_sector(RADI, cent1, RADI, MASS);
  body_t *shape2 = make_sector(RADI, cent2, RADI, MASS);
  body_t *shape3 = make_sector(RADI, cent3, RADI, MASS);
  body_t *shape4 = make_sector(RADI, cent4, RADI, INFINITY);
  body_t *shape5 = make_sector(RADI, cent5, RADI, MASS);

  // creates a scene with bodies
  scene_t *frame = scene_init();
  body_set_velocity(shape1, (vector_t){10, 0});
  body_set_velocity(shape2, (vector_t){0, 5});
  body_set_velocity(shape3, (vector_t){0, 6});
  body_set_velocity(shape5, (vector_t){0, 0});
  scene_add_body(frame, shape1);
  scene_add_body(frame, shape2);
  scene_add_body(frame, shape3);
  scene_add_body(frame, shape4);
  scene_add_body(frame, shape5);

  // Applies forces on bodies
  create_drag(frame, GAMMA, shape1);
  create_newtonian_gravity(frame, G, shape2, shape3);
  create_spring(frame, K, shape4, shape5);

  // updates scene and tests for dambing and gravity through conservation of
  // momentum
  double initial_momentum1 =
      calculate_momentum(body_get_mass(shape2), body_get_velocity(shape2).y);
  double initial_momentum2 =
      calculate_momentum(body_get_mass(shape3), body_get_velocity(shape3).y);
  double initial_momentum = initial_momentum1 + initial_momentum2;
  scene_tick(frame, dt);
  double final_momentum1 =
      calculate_momentum(body_get_mass(shape2), body_get_velocity(shape2).y);
  double final_momentum2 =
      calculate_momentum(body_get_mass(shape3), body_get_velocity(shape3).y);
  double final_momentum = final_momentum1 + final_momentum2;
  assert(round(calculate_velocity_change(1, body_get_mass(shape1))) ==
         round(body_get_velocity(shape1).x));
  printf("Test 1 passed! \n");
  assert(initial_momentum == final_momentum);
  printf("Test 2 passed! \n");
  assert(round(calculate_spring_velocity(K, body_get_mass(shape5), dt)) ==
         round(body_get_velocity(shape5).x));
  printf("Test 3 passed! \n");
  assert(round(calculate_spring_position(K, body_get_mass(shape5), dt)) ==
         round(body_get_centroid(shape5).x));
  printf("Test 4 passed! \n");

  // tests momentum conservation and change in velocity over times
  for (int i = 0; i < 7; i++) {
    double initial_momentum1 =
        calculate_momentum(body_get_mass(shape2), body_get_velocity(shape2).y);
    double initial_momentum2 =
        calculate_momentum(body_get_mass(shape3), body_get_velocity(shape3).y);
    double initial_momentum = initial_momentum1 + initial_momentum2;
    scene_tick(frame, dt);
    double final_momentum1 =
        calculate_momentum(body_get_mass(shape2), body_get_velocity(shape2).y);
    double final_momentum2 =
        calculate_momentum(body_get_mass(shape3), body_get_velocity(shape3).y);
    double final_momentum = final_momentum1 + final_momentum2;
    assert(round(calculate_velocity_change(i + 2, body_get_mass(shape1))) ==
           round(body_get_velocity(shape1).x));
    assert(round(initial_momentum) == round(final_momentum));
  }
  printf("Test 5 passed!\n");

  // frees everything
  scene_free(frame);
  free(cent1);
  free(cent2);
  free(cent3);
  free(cent4);
  free(cent5);
}
