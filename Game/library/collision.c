#include "collision.h"
#include "list.h"
#include "polygon.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

list_t *get_axes(list_t *shape) {
  list_t *axes = list_init(list_size(shape), (free_func_t)free);
  for (size_t i = 0; i < list_size(shape); i++) {
    vector_t *vertex1 = (vector_t *)list_get(shape, i);
    vector_t *vertex2 =
        (vector_t *)list_get(shape, (i + 1 == list_size(shape) ? 0 : i + 1));
    vector_t edge = vec_subtract(*vertex1, *vertex2);
    double norm_x = edge.y;
    double norm_y = -edge.x;
    vector_t *axis = malloc(sizeof(vector_t));
    double magnitude = sqrt((norm_x * norm_x) + (norm_y * norm_y));
    *axis = vec_multiply(1 / magnitude, (vector_t){norm_x, norm_y});
    list_add(axes, axis);
  }
  return axes;
}

vector_t get_projection(list_t *shape, vector_t *axis) {
  double min = vec_dot(*axis, *(vector_t *)list_get(shape, 0));
  double max = min;
  for (size_t i = 1; i < list_size(shape); i++) {
    double dot = vec_dot(*axis, *(vector_t *)list_get(shape, i));
    if (dot < min) {
      min = dot;
    } else if (dot > max) {
      max = dot;
    }
  }
  vector_t projection = (vector_t){min, max};
  return projection;
}

bool detected_overlap(vector_t projection1, vector_t projection2) {
  if (projection1.y < projection2.x || projection2.y < projection1.x) {
    return false;
  }
  return true;
}

double get_overlap(vector_t projection1, vector_t projection2) {
  return projection1.y >= projection2.x ? projection1.y - projection2.x
                                        : projection2.y - projection1.x;
}

collision_info_t check_axes(list_t *axes, list_t *shape1, list_t *shape2,
                            double *overlap, vector_t *smallest_axis) {
  for (size_t i = 0; i < list_size(axes); i++) {
    vector_t *axis = list_get(axes, i);
    vector_t projection1 = get_projection(shape1, axis);
    vector_t projection2 = get_projection(shape2, axis);
    if (!detected_overlap(projection1, projection2)) {
      return (collision_info_t){.collided = false, .axis = VEC_ZERO};
    } else {
      double o = get_overlap(projection1, projection2);
      if (o < *overlap) {
        *overlap = o;
        *smallest_axis = vec_multiply(*overlap, *axis);
      }
    }
  }
  return (collision_info_t){.collided = true, .axis = *smallest_axis};
}

collision_info_t find_collision(list_t *shape1, list_t *shape2) {
  list_t *axes1 = get_axes(shape1);
  list_t *axes2 = get_axes(shape2);
  double *overlap = malloc(sizeof(double));
  *overlap = INFINITY;
  vector_t *smallest_axis = malloc(sizeof(vector_t));
  collision_info_t collided1 =
      check_axes(axes1, shape1, shape2, overlap, smallest_axis);
  collision_info_t collided2 =
      !collided1.collided
          ? collided1
          : check_axes(axes2, shape1, shape2, overlap, smallest_axis);
  collision_info_t collision_info =
      (collision_info_t){.collided = collided1.collided && collided2.collided,
                         .axis = *smallest_axis};
  free(smallest_axis);
  free(overlap);
  list_free(axes1);
  list_free(axes2);
  list_free(shape1);
  list_free(shape2);
  return collision_info;
}
