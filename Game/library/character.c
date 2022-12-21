#include "character.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const rgb_color_t SHIELD_COLOR = (rgb_color_t){1, 0, 1, 0.31};
const double SHIELD_RADIUS = 50;
const double SHIELD_ELASTICITY = 1;
const double MAX_SPEED_MULT = 1.5;
const double MAX_REGENERATION = 3.0;
const double MAX_DAMAGE_MULT = 4.0;
const double HEAL_AMOUNT = 50;
const double UPGRADE_AMOUNT = 0.1;
const double USER_SPEED = 250;

typedef struct character {
  body_t *char_body;
  style_info_t char_style;
  weapon_t *curr_weapon;
  weapon_t *weapon1;
  weapon_t *weapon2;
  double health;
  double max_health;
  double damage_multiplier;
  double speed_multiplier;
  double healing_factor;
  double upgrade_factor;
} character_t;

double set_mass(style_info_t style) {
  switch (style) {
  case BRUTE:
    return 300;
  case GUNMAN:
    return 60;
  case SNIPER:
    return 100;
  default:
    return 60;
  }
}

weapon_t *set_primary(style_info_t style) {
  weapon_t *primary = NULL;
  switch (style) {
  case BRUTE:
    primary = weapon_init(SHOTGUN);
    break;
  case GUNMAN:
    primary = weapon_init(ASSAULT_RIFLE);
    break;
  case SNIPER:
    primary = weapon_init(SNIPER_RIFLE);
    break;
  default:
    primary = weapon_init(PISTOL);
    break;
  }
  return primary;
}

weapon_t *set_secondary(style_info_t style) {
  weapon_t *secondary = NULL;
  switch (style) {
  case BRUTE:
    secondary = weapon_init(PISTOL);
    break;
  case GUNMAN:
    secondary = weapon_init(SHOTGUN);
    break;
  case SNIPER:
    secondary = weapon_init(PISTOL);
    break;
  default:
    secondary = weapon_init(PISTOL);
    break;
  }
  return secondary;
}

double set_health(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 400;
  case BRUTE:
    return 1000;
  case SNIPER:
    return 500;
  default:
    return 250;
  }
}

double set_damage_multiplier(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 1.2;
  case BRUTE:
    return 1.5;
  case SNIPER:
    return 2.2;
  default:
    return 1.0;
  }
}

double set_speed_multiplier(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 1.0;
  case BRUTE:
    return 0.5;
  case SNIPER:
    return 0.8;
  default:
    return 0.8;
  }
}

double set_healing_factor(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 0.8;
  case BRUTE:
    return 1.1;
  case SNIPER:
    return 1.3;
  default:
    return 1.0;
  }
}

character_t *character_init(scene_t *scene, list_t *vertices,
                            computer_info_t *char_info, style_info_t style) {
  character_t *character = malloc(sizeof(character_t));
  body_t *char_body =
      body_init_with_info(vertices, set_mass(style), INTERNAL_BODY_COLOR,
                          char_info, (free_func_t)free);
  character->char_body = char_body;
  character->char_style = style;
  character->weapon1 = set_primary(style);
  character->weapon2 = set_secondary(style);
  character->curr_weapon = character->weapon1;
  character->health = set_health(style);
  character->max_health = set_health(style);
  character->damage_multiplier = set_damage_multiplier(style);
  character->speed_multiplier = set_speed_multiplier(style);
  character->healing_factor = set_healing_factor(style);
  character->upgrade_factor = 1.0;
  scene_add_body(scene, char_body);
  return character;
}

void character_free(character_t *character) {
  free(character->weapon1);
  free(character->weapon2);
  free(character);
}

body_t *character_get_body(character_t *character) {
  return character->char_body;
}

vector_t character_vector_facing(character_t *character) {
  return vec_rotate((vector_t){1, 0}, body_get_rotation(character->char_body));
}

void character_shoot(scene_t *scene, character_t *character, double mass) {
  weapon_shoot(
      scene, character->curr_weapon,
      vec_get_unit_vector(character_vector_facing(character), VEC_ZERO),
      character->char_body, body_get_rotation(character->char_body), mass);
}

void character_set_rotation(character_t *character, double angle) {
  body_set_rotation(character->char_body, angle);
}

void character_process_damage(character_t *character, double enemy_dmg_mult) {
  double damage = body_damage_collisions(character->char_body);
  character->health -= enemy_dmg_mult * damage;
  body_add_damage(character->char_body, -damage);
}

double character_get_health(character_t *character) {
  return character->health;
}

bool character_is_alive(character_t *character) {
  return character->health > 0.0;
}

void character_set_velocity(character_t *character, vector_t velocity) {
  body_set_velocity(character->char_body,
                    vec_multiply(character->speed_multiplier, velocity));
}

void character_heal(character_t *character) {
  character->health += (character->healing_factor * HEAL_AMOUNT);
  character->health = character->health > character->max_health
                          ? character->max_health
                          : character->health;
}

void character_cooldown_weapon(character_t *character,
                               double cooldown_decrement) {
  weapon_cooldown(character->weapon1, cooldown_decrement);
  weapon_cooldown(character->weapon2, cooldown_decrement);
}

void character_switch_weapon(character_t *character) {
  if (character->curr_weapon == character->weapon1) {
    character->curr_weapon = character->weapon2;

  } else if (character->curr_weapon == character->weapon2) {
    character->curr_weapon = character->weapon1;
  }
}

void character_reload(character_t *character) {
  weapon_reload(character->curr_weapon);
}

void character_decrement_reload(character_t *character,
                                double reload_decrement) {
  weapon_decrement_reload(character->curr_weapon, reload_decrement);
}

bool character_is_reloading_curr(character_t *character) {
  return weapon_is_reloading(character->curr_weapon);
}

bool character_is_on_cooldown(character_t *character) {
  return !weapon_off_cooldown(character->curr_weapon);
}

void character_increase_speed(character_t *character) {
  character->speed_multiplier += (character->upgrade_factor * UPGRADE_AMOUNT);
  character->speed_multiplier = character->speed_multiplier > MAX_SPEED_MULT
                                    ? MAX_SPEED_MULT
                                    : character->speed_multiplier;
}

void character_increase_healing(character_t *character) {
  character->healing_factor += (character->upgrade_factor * UPGRADE_AMOUNT);
  character->healing_factor = character->healing_factor > MAX_REGENERATION
                                  ? MAX_REGENERATION
                                  : character->healing_factor;
}

void character_increase_damage(character_t *character) {
  character->damage_multiplier += (character->upgrade_factor * UPGRADE_AMOUNT);
  character->damage_multiplier =
      character->damage_multiplier > 4.0 ? 4.0 : character->damage_multiplier;
}

void character_increase_health(character_t *character) {
  character->max_health += (character->upgrade_factor * HEAL_AMOUNT);
}

void character_increase_upgrade_factor(character_t *character) {
  character->upgrade_factor++;
}

double character_get_dmg_multiplier(character_t *character) {
  return character->damage_multiplier;
}

double character_get_speed_multiplier(character_t *character) {
  return character->speed_multiplier;
}

double character_get_upgrade_multiplier(character_t *character) {
  return character->upgrade_factor;
}

double character_get_healing_factor(character_t *character) {
  return character->healing_factor;
}

style_info_t character_style(character_t *player) { return player->char_style; }

void create_shield(scene_t *scene, character_t *player) {
  vector_t *center = malloc(sizeof(vector_t));
  *center = body_get_centroid(player->char_body);
  list_t *vertices = create_sector(SHIELD_RADIUS, center, 2 * M_PI, false);
  computer_info_t *shield_info = malloc(sizeof(computer_info_t));
  *shield_info = SHIELD;
  body_t *shield =
      body_init_with_info(vertices, INFINITY, SHIELD_COLOR, shield_info, free);
  scene_add_body(scene, shield);
  list_t *enemies = scene_bodies_with_comp_info(scene, ENEMY);
  for (size_t i = 0; i < list_size(enemies); i++) {
    body_t *enemy = list_get(enemies, i);
    create_physics_collision(scene, SHIELD_ELASTICITY, enemy, shield);
  }
  list_free(enemies);
}

weapon_t *character_weapon(character_t *player) { return player->curr_weapon; }

double character_get_max_health(character_t *character) {
  return character->max_health;
}