#include "polygon.h"
#include <math.h>
#include <stdlib.h>

typedef struct polygon {
  vector_t velocity;
  list_t *shape;
  float R;
  float G;
  float B;
} polygon_t;

polygon_t *polygon_init() {
  polygon_t *output = malloc(sizeof(polygon_t));
  return output;
};

void polygon_free(polygon_t *polygon) {
  list_free(polygon->shape);
  free(polygon);
};

vector_t polygon_get_velocity(polygon_t *polygon) { return polygon->velocity; }

list_t *polygon_get_vertices(polygon_t *polygon) { return polygon->shape; }

float polygon_get_R(polygon_t *polygon) { return polygon->R; }

float polygon_get_G(polygon_t *polygon) { return polygon->G; }

float polygon_get_B(polygon_t *polygon) { return polygon->B; }

void polygon_set_velocity(polygon_t *polygon, vector_t velocity) {
  polygon->velocity = velocity;
}

void *polygon_set_vertices(polygon_t *polygon, list_t *shape) {
  return polygon->shape = shape;
}

void polygon_set_R(polygon_t *polygon, float R) { polygon->R = R; }

void polygon_set_G(polygon_t *polygon, float G) { polygon->G = G; }

void polygon_set_B(polygon_t *polygon, float B) { polygon->B = B; }

void polygon_change_velocity(polygon_t *polygon, vector_t delta_v) {
  polygon->velocity = vec_add(polygon->velocity, delta_v);
}

vector_t *void_to_vector(list_t *polygon, size_t index) {
  return (vector_t *)list_get(polygon, index);
}

double polygon_area(list_t *polygon) {
  size_t n_sides = list_size(polygon);
  double area = 0;
  for (size_t i = 0; i < n_sides; i++) {
    area += vec_cross(*void_to_vector(polygon, i),
                      *void_to_vector(polygon, (i + 1) % n_sides));
  }
  return 0.5 * area;
};

vector_t polygon_centroid(list_t *polygon) {
  size_t n_sides = list_size(polygon);
  double factor = 6 * polygon_area(polygon);
  double center_x = 0;
  double center_y = 0;
  for (size_t i = 0; i < n_sides; i++) {
    vector_t *v1 = void_to_vector(polygon, i);
    vector_t *v2 = void_to_vector(polygon, (i + 1) % n_sides);
    center_x += vec_cross(*v1, *v2) * (v1->x + v2->x);
    center_y += vec_cross(*v1, *v2) * (v1->y + v2->y);
  }
  vector_t polyvecent = {center_x / factor, center_y / factor};
  return polyvecent;
};

void polygon_translate(list_t *polygon, vector_t translation) {
  for (size_t i = 0; i < list_size(polygon); i++) {
    *void_to_vector(polygon, i) =
        vec_add(*void_to_vector(polygon, i), translation);
  }
};

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *vertex = void_to_vector(polygon, i);
    *void_to_vector(polygon, i) =
        vec_add(vec_rotate(vec_subtract(*vertex, point), angle), point);
  }
};