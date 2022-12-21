#ifndef __INFO_TYPES_H__
#define __INFO_TYPES_H__

/**
 * @brief Represents the internal body color for character's and computers
 * MATCHES FLOOR OF SCENE
 *
 */
static const rgb_color_t INTERNAL_BODY_COLOR =
    (rgb_color_t){0.1961, 0.8039, 0.1961, 1.0};

/**
 * @brief weapon info used for building weapon_t pointers
 *
 */
typedef enum {
  PISTOL = 1,        // REPRESENTS PISTOL WEAPON TYPE
  SHOTGUN = 2,       // REPRESENTS SHOTGUN WEAPON TYPE
  ASSAULT_RIFLE = 3, // REPRESENTS ASSAULT RIFLE WEAPON TYPE
  SNIPER_RIFLE = 4,  // REPRESENTS SNIPER RIFLE WEAPON TYPE
} weapon_info_t;

/**
 * @brief body info for bullets added to scene; also sets their damage
 *
 */
typedef enum {
  PISTOL_BULLET = 15,  // ENUM REPRESENTING DAMAGE AMOUNT FOR PISTOL BULLETS
  SHOTGUN_BULLET = 20, // ENUM REPRESENTING DAMAGE AMOUNT FOR SHOTGUN BULLETS
  ASSAULT_RIFLE_BULLET =
      25, // ENUM REPRESENTING DAMAGE AMOUNT FOR ASSAULT RIFLE BULLETS
  SNIPER_BULLET = 50, // ENUM REPRESENTING DAMAGE AMOUNT FOR SNIPER BULLETS
} bullet_info_t;

/**
 * @brief combat styles for enemies and computers
 *
 */
typedef enum {
  BRUTE = 1,    // BRUTE COMBAT TYPE FOR CHARACTER AND COMPUTER
  GUNMAN = 2,   // GUNMAN COMBAT TYPE FOR CHARACTER AND COMPUTER
  SNIPER = 3,   // SNIPER COMBAT TYPE FOR CHARACTER AND COMPUTER
  HENCHMAN = 4, // PISTOL USER ENEMY/COMPUTER
} style_info_t;

/**
 * Represent the body info for different elements of BRAWLHUB
 *
 */
typedef enum {
  ENEMY = 1,      // REPRESENTS ENEMY TYPE
  CHARACTER = 2,  // REPRESENTS CHARACTER TYPE
  OBSTACLE = 3,   // REPRESENTS OBSTACLE TYPE
  FLOOR = 4,      // REPRESENTS FLOOR OF GAME SCENE
  BACKGROUND = 5, // REPRESENTS THE BACKGROUND OF GAME SCENE
  SHIELD = 6,     // REPRESENTS SHIELD TYPE
} computer_info_t;

/**
 * Represent enemy dmg multipliers
 *
 */
typedef enum {
  ENEMY_WAVE1 = 1,
  ENEMY_WAVE2 = 2,
  ENEMY_WAVE3 = 3,
  ENEMY_WAVE4 = 4,
} wave_info_t;

#endif