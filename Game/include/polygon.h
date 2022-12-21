#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "list.h"
#include "vector.h"

typedef struct polygon polygon_t;

polygon_t *polygon_init();

void polygon_free(polygon_t *polygon);

vector_t polygon_get_velocity(polygon_t *polygon);

list_t *polygon_get_vertices(polygon_t *polygon);

float polygon_get_R(polygon_t *polygon);

float polygon_get_G(polygon_t *polygon);

float polygon_get_B(polygon_t *polygon);

void polygon_set_velocity(polygon_t *polygon, vector_t velocity);

void *polygon_set_vertices(polygon_t *polygon, list_t *shape);

void polygon_set_R(polygon_t *polygon, float R);

void polygon_set_G(polygon_t *polygon, float G);

void polygon_set_B(polygon_t *polygon, float B);

void polygon_change_velocity(polygon_t *polygon, vector_t delta_v);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(list_t *polygon);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
vector_t polygon_centroid(list_t *polygon);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(list_t *polygon, vector_t translation);

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(list_t *polygon, double angle, vector_t point);

#endif // #ifndef __POLYGON_H__
