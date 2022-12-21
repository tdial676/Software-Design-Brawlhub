#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "collision.h"
#include "forces.h"
#include "scene.h"
#include "weapon.h"

/**
 * A character inside the brawlhub game
 * Has a particular combat style, inner body, and sprite representation
 *
 */
typedef struct character character_t;

/**
 * The maximum speed multiplier for a character
 *
 */
extern const double MAX_SPEED_MULT;

/**
 * The maximum healing factor/regeneration for a character
 *
 */
extern const double MAX_REGENERATION;

/**
 * The maximum damage multiplier for a character
 *
 */
extern const double MAX_DAMAGE_MULT;

/**
 * Represents the standard upgrade increase for
 * character upgrades
 *
 */
extern const double UPGRADE_AMOUNT;

/**
 * Represents the standard Character speed
 *
 */
extern const double USER_SPEED;

/**
 * Initializes and returns a character pointer
 *
 * @param scene to add character body to
 * @param vertices used to make the character body
 * @param char_info info to make character body
 * @param style in which character fights
 * @return character_t* (a new character pointer)
 */
character_t *character_init(scene_t *scene, list_t *vertices,
                            computer_info_t *char_info, style_info_t style);

/**
 * Frees the character and the character's weapon
 *
 * @param character to free
 */
void character_free(character_t *character);

/**
 * Returns the character's body
 *
 * @param character to get body of
 * @return body_t* that is the body pointer for the character
 */
body_t *character_get_body(character_t *character);

/**
 * Returns the direction that the character is facing as a vector relative to
 * the unit circle
 *
 * @param character whose orientation vector is attained
 * @return vector_t indicating direction the 'character' is facing
 */
vector_t character_vector_facing(character_t *character);

/**
 * Shoots a bullet in the direction the 'character' is facing
 * by adding a bullet body to 'scene' if the character's weapon is not on
 * cooldown and has ammo or decrements the cooldown timer on the character's
 * weapon
 *
 * @param scene that bullet is added to
 * @param character who is shooting bullet
 */
void character_shoot(scene_t *scene, character_t *character, double mass);

/**
 * Changes a character's body's orientation in the plane.
 * The body is rotated about its center of mass.
 *
 * @param character to rotate
 * @param angle the body's new angle in radians. Positive is counterclockwise.
 */
void character_set_rotation(character_t *character, double angle);

/**
 * Processes damage done to character on a tick
 *
 * @param character whose damage is processed
 * @param enemy_dmg_mult dmg multiplier of the enemy wave
 */
void character_process_damage(character_t *character, double enemy_dmg_mult);

/**
 * Returns the character's current health
 *
 * @param character whose health is returned
 * @return double representing amount of health character has remaining
 */
double character_get_health(character_t *character);

/**
 * Returns whether character is alive or dead
 *
 * @param character whose status is being retrieved
 * @return true if character is alive
 * @return false if character is dead
 */
bool character_is_alive(character_t *character);

/**
 * sets the velocity of the character's body to 'velocity'
 *
 * @param character whose velocity is changed
 * @param velocity that the character's velocity is set to
 */
void character_set_velocity(character_t *character, vector_t velocity);

/**
 * Heals the character by an amount based on their healing factor multiplied by
 * 25
 *
 * @param character who is healed
 */
void character_heal(character_t *character);

/**
 * Lowers the cooldown timer on current weapon of 'character' by
 * 'cooldown_decrement'
 *
 * @param character whose weapon cooldown is lowered
 * @param cooldown_decrement amount of time from tick that the cooldown timer is
 * decremented by
 */
void character_cooldown_weapon(character_t *character,
                               double cooldown_decrement);

/**
 * Switches the weapon of the character between primary and secondary
 *
 * @param character whose weapon is switched
 */
void character_switch_weapon(character_t *character);

/**
 * reloads the character's current weapon
 *
 * @param character whose weapon is reloaded
 */
void character_reload(character_t *character);

/**
 * Lowers the reload timer on current weapon of 'character' by
 * 'reload_decrement'
 *
 * @param character whose weapon reload time is lowered
 * @param reload_decrement amount of time from tick that the reload timer is
 * decremented by
 */
void character_decrement_reload(character_t *character,
                                double reload_decrement);

/**
 * Returns if the character's current weapon is reloading
 *
 * @param character whose weapon status is checked
 * @return true if current weapon is reloading
 * @return false if current weapon is not reloading
 */
bool character_is_reloading_curr(character_t *character);

/**
 * Returns if the character's current weapon is on cooldown
 *
 * @param character whose weapon status is checked
 * @return true if current weapon is on cooldown
 * @return false if current weapon is not on cooldown
 */
bool character_is_on_cooldown(character_t *character);

/**
 * Returns the character's damage multiplier to be used for processing
 * damage on computers
 *
 * @param character whose damage multiplier is returned
 * @return double representing character damage multiplier
 */
double character_get_dmg_multiplier(character_t *character);

/**
 * Returns the combat style of a character
 *
 * @param player whose combat style is returned
 * @return style_info_t (an enum representing a combat style)
 */
style_info_t character_style(character_t *player);

/**
 * @brief Create a shield object as one of the powerups around a gunman
 * character
 *
 * @param scene that shield is added to
 * @param player who shield is built around
 */
void create_shield(scene_t *scene, character_t *player);

/**
 * @brief Returns the weapon a character is currently holding
 *
 * @param player whose currently held weapon is returned
 * @return weapon_t* representing a weapon object pointer
 */
weapon_t *character_weapon(character_t *player);

/**
 * @brief Increases the character's speed based on the
 *        character's upgrade factor
 *
 * @param character whose speed is increased
 */
void character_increase_speed(character_t *character);

/**
 * @brief Increases the character's damage based on the
 *        character's upgrade factor
 *
 * @param character whose damage is increased
 */
void character_increase_damage(character_t *character);

/**
 * @brief Increases the character's regeneration based on the
 *        character's upgrade factor
 *
 * @param character whose regeneration is increased
 */
void character_increase_healing(character_t *character);

/**
 * @brief Increases the character's max health based on the
 *        character's upgrade factor
 *
 * @param character whose max health is increased
 */
void character_increase_health(character_t *character);

/**
 * @brief Increases the character's upgrade factor by one;
 *
 * @param character whose upgrade factor is increased
 */
void character_increase_upgrade_factor(character_t *character);

/**
 * @brief Returns the character's current speed multiplier
 *
 * @param character whose speed multiplier is returned
 * @return double representing a character's speed multiplier
 */
double character_get_speed_multiplier(character_t *character);

/**
 * @brief Returns the character's current speed multiplier
 *
 * @param character whose speed multiplier is returned
 * @return double representing a character's speed multiplier
 */
double character_get_upgrade_multiplier(character_t *character);

/**
 * @brief Returns the character's current regeneration
 *
 * @param character whose regeneration is returned
 * @return double representing a character's regeneration factor
 */
double character_get_healing_factor(character_t *character);

/**
 * @brief Returns the current max health of a character
 *
 * @param character whose max health is returned
 * @return double representing a character's current max health
 */
double character_get_max_health(character_t *character);
#endif