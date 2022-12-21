#ifndef __KEY_H__
#define __KEY_H__

#include "character.h"
#include "scene.h"
#include "sdl_wrapper.h"
/**
 * @brief gets the current x,y coordinates of the mouse position.
 *
 * @return vector coordinate of the of the mouse.
 */
vector_t get_mouse();

/**
 * @brief sets the character's velocity along its relative y axix;
 *
 * @param player the player's body;
 * @param fob takes in 1 for f or -1 for b to indicate whether the character
 * is moving forwads or back wards.
 */
void fob_move(character_t *player, double fob);

/**
 * @brief sets the character's velocity along its relative x axix;
 *
 * @param player the player's body;
 * @param lor takes in 1 for r or -1 for l to indicate whether the character
 * is moving to the right or lefct or back wards;
 */
void lor_move(character_t *player, double lor);

/**
 * @brief gets the angle between the direction the character is facing and the
 * mouse location so that we can use it to rotate the character to face the
 * mouse.
 *
 * @param player the player;
 * @return double angle between character and mouse;
 */
double get_rotation_character(character_t *player);

/**
 * @brief gets the angle between the direction a body is facing and the
 * mouse location so that we can use it to rotate the body to face the
 * mouse.
 *
 * @param player the player;
 * @return double angle between player and mouse;
 */
double get_rotation_body(body_t *player);

#endif
