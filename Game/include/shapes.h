#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "body.h"
#include "color.h"
#include "polygon.h"

/**
 * Makes a sector
 *
 * @param radius the radius of the sector to be drawn
 * @param center the center of the sector.
 * @param angle how much of the sector should be filled in
 * @param mass the mass of the object to be drawn
 * @param gap asks whether they want a closed or open sector
 * @param color the color of the sector
 * @return a body
 */
list_t *create_sector(double radius, vector_t *center, double angle, bool gap);

/**
 * Makes an oval
 *
 * @param width width of the oval
 * @param height height of the oval
 * @param center the center of the sector.
 * @return a body
 */
list_t *create_oval(double width, double height, vector_t *center);

/**
 * Makes a rectangle
 *
 * @param center center of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param mass the mass of the object to be drawn
 * @param color the color of the sector
 * @return list_t*
 */
list_t *create_four_sided_shape(vector_t center, double width, double height);

/**
 * Makes variations of star objects
 *
 * @param outer_radius the outter radius of the star
 * @param inner_radius the inner radius of the star
 * @param number_corners number of corners that the star has
 * @param center the center of the star
 * @param mass the mass of the star
 * @param color the color of the star
 * @return a star body
 */
list_t *create_star(double outer_radius, double inner_radius,
                    size_t number_corners, vector_t center);

#endif // #ifndef __SHAPES_H__