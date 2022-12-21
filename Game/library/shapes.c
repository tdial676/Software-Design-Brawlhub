#include "shapes.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

const size_t GRANULARITY_1 = 20;
const size_t QUADRILATERAL = 4;

list_t *create_sector(double radius, vector_t *center, double angle, bool gap) {
  assert(angle > 0);
  assert(radius > 0);
  list_t *vertices = list_init(GRANULARITY_1 + 1, (free_func_t)free);
  if (gap == true) {
    list_add(vertices, center);
  }
  vector_t radius_vector = {center->x + radius, center->y};
  for (size_t i = 0; i < GRANULARITY_1; i++) {
    vector_t *added_point =
        malloc(sizeof(vector_t)); // Need these two lines since list_add will
                                  // only accept vector pointers, not vectors
    *added_point = vec_add(vec_rotate(vec_subtract(radius_vector, *center),
                                      i * angle / GRANULARITY_1),
                           *center);
    list_add(vertices, added_point);
  }
  if (gap == false) {
    free(center);
  }
  return vertices;
}

list_t *create_oval(double width, double height, vector_t *center) {
  assert(width > 0);
  assert(height > 0);
  double c = width / height;
  list_t *oval = create_sector(height, center, 2 * M_PI, false);
  for (size_t i = 0; i < list_size(oval); i++) {
    vector_t *v = (vector_t *)list_get(oval, i);
    v->x = center->x + (v->x - center->x) * c;
  }
  return oval;
}

list_t *create_four_sided_shape(vector_t center, double width, double height) {
  assert(width > 0);
  assert(height > 0);
  list_t *vertices = list_init(QUADRILATERAL, free);
  vector_t *v = malloc(sizeof(*v));
  double half_w = width / 2;
  double half_h = height / 2;
  *v = (vector_t){center.x + half_w, center.y + half_h};
  list_add(vertices, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){center.x - half_w, center.y + half_h};
  list_add(vertices, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){center.x - half_w, center.y - half_h};
  list_add(vertices, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){center.x + half_w, center.y - half_h};
  list_add(vertices, v);
  return vertices;
}

list_t *create_star(double outer_radius, double inner_radius,
                    size_t number_corners, vector_t center) {
  assert(outer_radius > 0);
  assert(inner_radius > 0);
  list_t *vertices = list_init(2 * number_corners, (free_func_t)free);
  vector_t start_long = {center.x, center.y + outer_radius};
  vector_t start_short = vec_add(
      vec_rotate(
          vec_subtract((vector_t){center.x, center.y + inner_radius}, center),
          M_PI / number_corners),
      center);
  // Forming the polygon using our values
  for (size_t i = 0; i < number_corners; i++) {
    vector_t *long_pointer = malloc(sizeof(vector_t));
    vector_t *short_pointer = malloc(sizeof(vector_t));
    *long_pointer = vec_add(vec_rotate(vec_subtract(start_long, center),
                                       2 * M_PI * i / number_corners),
                            center);
    *short_pointer = vec_add(vec_rotate(vec_subtract(start_short, center),
                                        2 * M_PI * i / number_corners),
                             center);
    list_add(vertices, long_pointer);
    list_add(vertices, short_pointer);
  }
  return vertices;
}