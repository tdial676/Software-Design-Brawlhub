#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "body.h"
#include "character.h"
#include "collision.h"
#include "scene.h"
#include "weapon.h"

/**
 * The struct representing the computer object. A computer
 * within our game will have a specific target, a certain amount
 * of xp, a speed multiplier, boss checker bool, and specific
 * associated health.
 *
 */
typedef struct computer computer_t;

/**
 * A specific multiplier to increase a boss computers health
 */
extern const double BOSS_HEALTH_MULTIPLIER;

/**
 * A constant representing a limiting factor to the
 * amount of damage a boss will take from a bullet.
 */
extern const double BOSS_DAMAGE_RESISTANCE;

/**
 * Double representing the scalar speed of
 * an enemy
 */
extern const double ENEMY_SPEED;

/**
 * Constant representing the standard bullet
 * mass for a computer's weapon
 */
extern const double STANDARD_BULLET_MASS;

/**
 * @brief Initializes a computer within a specific scene and a set
 * of various arguments that denotes the computer's characteristics.
 *
 * @param scene specific scene where the computer is initialized
 * @param is_boss bool denoting whether the computer is to be initialized as a
 * boss
 * @param style specific type of enemy that will be spawned into scene.
 * @param type_of_comp will be set to ENEMY for enemy AI.
 * @param vertices vertices of the computer that will be used for generation
 * @return computer_t*
 */
computer_t *computer_init(scene_t *scene, bool is_boss, style_info_t style,
                          computer_info_t *type_of_comp, list_t *vertices);

/**
 * @brief Checks whether an enemy is a boss
 * or not
 *
 * @param boss a potential boss computer
 * @return true if boss
 * @return false no boss
 */
bool is_boss(computer_t *boss);

/**
 * @brief Measures the distance between
 * a computer and character within the scene
 *
 * @param ai the specific computer
 * @param enemy the user within the scene
 * @return double
 */
double computer_distance_to(computer_t *ai, character_t *enemy);

/**
 * @brief Processes the damage on a specific computer
 * based on a specifc multiplier and the amount
 * of collisions
 *
 * @param ai specific computer's damage being processed
 * @param character_multiplier the damage multiplier
 */
void computer_process_damage(computer_t *ai, double character_multiplier);

/**
 * @brief Returns the computer's health
 *
 * @param ai the specific computer
 * @return double health
 */
double computer_health(computer_t *ai);

/**
 * @brief Returns the style of a specific computer
 *
 * @param ai the specific computer
 * @return style_info_t style
 */
style_info_t computer_get_style(computer_t *ai);

/**
 * @brief Returns the direction of the computer
 *
 * @param ai the specific computer
 * @return vector_t
 */
vector_t computer_vector_facing(computer_t *ai);

/**
 * @brief Returns the resultant computer's
 * body
 *
 * @param ai comouter that has the body
 * @return body_t*
 */
body_t *get_comp_body(computer_t *ai);

/**
 * @brief Invokes the weapon_shoot method
 * in order to start shooting the character
 *
 * @param scene specific scene that the computer is in
 * @param ai computer needing to shoot
 */
void computer_shoot(scene_t *scene, computer_t *ai);

/**
 * @brief Returns the health of the computer
 *
 * @param ai specific computer
 * @return double health
 */
double computer_get_health(computer_t *ai);

/**
 * @brief Checks whether a specific computer within the scene
 * is still alive or not
 *
 * @param ai the specific computer
 * @return true if alive
 * @return false is not alive
 */
bool computer_is_alive(computer_t *ai);

/**
 * @brief Sets the velocity of the computer within a
 * specific direction
 *
 * @param ai the specific computer being changed
 * @param direction vector representing the direction of computer
 */
void computer_set_velocity(computer_t *ai, vector_t direction);

/**
 * @brief An associated cooldown method for weapons that the computer owns.
 * Constantly lowers the cooldown timer by cooldown_decrement.
 *
 * @param ai the specific computer under cooldown
 * @param cooldown_decrement amount of time it takes to decrement the cooldown
 * timer
 */
void computer_cooldown_weapon(computer_t *ai, double cooldown_decrement);

/**
 * @brief Associated method used for reloading the weapon
 * associated with the computer
 *
 * @param ai specific computer reloading
 */
void computer_reload(computer_t *ai);

/**
 * Constantly decrements the reload timer by reload_decrement
 *
 * @param ai specific computer reloading
 * @param reload_decrement amount of time needed to decrement
 */
void computer_decrement_reload(computer_t *ai, double reload_decrement);

/**
 * Checks whether a specific computer needs to relaod the weapon
 *
 * @param ai the specific computer reloading
 * @return true if needing to reload
 * @return false if not needing to reload
 */
bool computer_is_reloading_curr(computer_t *ai);

/**
 * Checks whether the computer's weapon is
 * still on cooldown
 *
 * @param ai the specific computer on cooldown
 * @return true if on cooldown
 * @return false if not on cooldown
 */
bool computer_is_on_cooldown(computer_t *ai);

/**
 * Checks whether the computer's weapon ammo is
 * full or not
 *
 * @param ai the specific computer
 * @return true if still full
 * @return false if not full
 */
bool computer_weapon_ammo(computer_t *ai);

/**
 * Rotates the computer within the direction of the centroid
 * of the user within the scene
 *
 * @param ai the specific computer being rotated
 * @param character the user in the scene
 */
void computer_rotate(computer_t *ai, character_t *character);

/**
 * Gets the vector within the direction of the computer
 *
 * @param ai the specific computer
 * @param character the user
 * @return vector_t the vector direction to the computer
 */
vector_t computer_direction(computer_t *ai, character_t *character);

/**
 * Freer method of computer
 *
 * @param ai computer to be freed
 */
void computer_free(computer_t *ai);

/**
 * @brief the xp that this enemy offered object
 *
 * @param style the enemy type
 * @return the amount of xp that enemy offers
 */
double set_xp_offered(style_info_t style);

/**
 * @brief Returns the xp an enemy offers;
 *
 * @param ai an enemy
 * @return Returns the xp an enemy offers as a double;
 */
double computer_get_xp(computer_t *ai);

/**
 * @brief gets the associated png filename with the respective computer
 * object within the scene based on the stule
 *
 * @param style specific style of the computer
 * @return char* the associated png filename
 */

char *comp_switch_file(style_info_t style);

/**
 * @brief Returns the specific weapon associated with the
 * computer
 *
 * @param ai the computer that wants to get the weapon
 * @return weapon_t* the weapon
 */

weapon_t *computer_get_weapon(computer_t *ai);

#endif
