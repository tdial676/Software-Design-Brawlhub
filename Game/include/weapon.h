#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "collision.h"
#include "forces.h"
#include "scene.h"
#include "shapes.h"
#include <stdbool.h>

/**
 * Represents one of the weapon types implemented
 * for brawlhub depending on a weapon_info_t
 *
 */
typedef struct weapon weapon_t;

/**
 * Creates a weapon pointer of 'type'
 *
 * @param type that denotes weapon type
 * @return weapon_t* pointer of weapon_t type
 */
weapon_t *weapon_init(weapon_info_t type);

/**
 * Returns whether or not a weapon has ammo
 *
 * @param weapon whose status is checked
 * @return true if weapon has ammo
 * @return false if weapon has no ammo
 */
bool weapon_has_ammo(weapon_t *weapon);

/**
 * Returns amount of ammo currently in 'weapon'
 *
 * @param weapon whose ammo is checked
 * @return double representing the amount of ammo in 'weapon'
 */
double weapon_ammo(weapon_t *weapon);

/**
 * Returns whether a weapon is off cooldown
 *
 * @param weapon whose status is checked
 * @return true if weapon is off cooldown
 * @return false if weapon is not off cooldown
 */
bool weapon_off_cooldown(weapon_t *weapon);

/**
 * Returns weapon's current cooldown timer
 *
 * @param weapon whose cooldown timer is checked
 * @return double representing the status of weapon's cooldown
 */
double weapon_cooldown_timer(weapon_t *weapon);

/**
 * Decrements the weapon cooldown by 'timer_decrement' which is
 * value representing tick
 *
 * @param weapon that is on cooldown
 * @param timer_decrement amount of time from tick that the cooldown timer is
 * decremented by
 */
void weapon_cooldown(weapon_t *weapon, double timer_decrement);

/**
 * Returns if 'weapon' is reloading
 *
 * @param weapon whose status is checked
 * @return true if weapon is reloading
 * @return false if weapon is not reloading
 */
bool weapon_is_reloading(weapon_t *weapon);

/**
 * Reloads the weapon if the reload timer is up
 *
 * @param weapon that is being reloaded
 */
void weapon_reload(weapon_t *weapon);

/**
 * Decrements weapon reload timer by 'timer_decrement'
 *
 * @param weapon whose reload timer is decremented
 * @param timer_decrement amount to decrement reload timer by
 */
void weapon_decrement_reload(weapon_t *weapon, double timer_decrement);

/**
 * Returns weapon's current reload timer
 *
 * @param weapon whose reload timer is checked
 * @return double representing the status of weapon's cooldown
 */
double weapon_reload_timer(weapon_t *weapon);

/**
 * Shoots a bullet in 'dir' from 'weapon' of 'shooter' and adds it to the
 * 'scene' if 'weapon' has ammo, is off cooldown, and is not reloading
 *
 * @param scene that bullet is added
 * @param weapon that is being shot
 * @param dir that the bullet is travelling in
 * @param shooter body that is shooting the bullet
 * @param orientation of bullet relative to a unit circle angle
 */
void weapon_shoot(scene_t *scene, weapon_t *weapon, vector_t dir,
                  body_t *shooter, double orientation, double mass);

/**
 * @brief Returns the type of weapon
 *
 * @param weapon whose type is returned
 * @return weapon_info_t an enum representing the type of weapon
 */
weapon_info_t weapon_get_type(weapon_t *weapon);
#endif