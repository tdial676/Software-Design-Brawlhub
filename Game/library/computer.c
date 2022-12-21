#include "computer.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const double BOSS_HEALTH_MULTIPLIER = 5.0;
const double BOSS_DAMAGE_RESISTANCE = 0.5;
const double ENEMY_SPEED = 125;
const double COMP_BULLET_MASS = 150;

typedef struct computer {
  computer_info_t target;
  body_t *comp_body;
  double health;
  bool is_boss;
  weapon_t *weapon;
  style_info_t comp_style;
  double speed_multiplier;
  double xp_offered;
} computer_t;

double set_xp_offered(style_info_t style) {
  switch (style) {
  case BRUTE:
    return 10.0;
  case HENCHMAN:
    return 3.0;
  case SNIPER:
    return 5.0;
  case GUNMAN:
    return 8.0;
  }
}

weapon_t *set_weapon(style_info_t style) {
  weapon_t *comp_weapon = NULL;
  switch (style) {
  case BRUTE:
    comp_weapon = weapon_init(SHOTGUN);
    break;
  case GUNMAN:
    comp_weapon = weapon_init(ASSAULT_RIFLE);
    break;
  case SNIPER:
    comp_weapon = weapon_init(SNIPER_RIFLE);
    break;
  case HENCHMAN:
    comp_weapon = weapon_init(PISTOL);
    break;
  }
  return comp_weapon;
}

double set_computer_health(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 80;
  case BRUTE:
    return 160;
  case SNIPER:
    return 120;
  case HENCHMAN:
    return 40;
  }
}

double set_computer_speed_multiplier(style_info_t style) {
  switch (style) {
  case GUNMAN:
    return 1.2;
  case BRUTE:
    return 0.5;
  case SNIPER:
    return 0.8;
  case HENCHMAN:
    return 1.0;
  }
}

double set_computer_mass(style_info_t style) {
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

computer_t *computer_init(scene_t *scene, bool is_boss, style_info_t style,
                          computer_info_t *type_of_comp, list_t *vertices) {
  computer_t *ai = malloc(sizeof(computer_t));
  body_t *comp_body =
      body_init_with_info(vertices, set_computer_mass(style),
                          INTERNAL_BODY_COLOR, type_of_comp, (free_func_t)free);
  ai->comp_body = comp_body;
  ai->is_boss = is_boss;
  if (is_boss) {
    ai->health = BOSS_HEALTH_MULTIPLIER * set_computer_health(style);
  } else {
    ai->health = set_computer_health(style);
  }
  ai->weapon = set_weapon(style);
  ai->comp_style = style;
  ai->target = CHARACTER;
  ai->speed_multiplier = set_computer_speed_multiplier(style);
  ai->xp_offered = set_xp_offered(style);
  scene_add_body(scene, comp_body);
  return ai;
}

bool is_boss(computer_t *boss) { return boss->is_boss; }

weapon_t *computer_get_weapon(computer_t *ai) { return ai->weapon; }

style_info_t computer_get_style(computer_t *ai) { return ai->comp_style; }

body_t *get_comp_body(computer_t *ai) { return ai->comp_body; }

void computer_process_damage(computer_t *ai, double character_multiplier) {
  if (is_boss(ai)) {
    double damage = body_damage_collisions(ai->comp_body);
    ai->health -= (character_multiplier * BOSS_DAMAGE_RESISTANCE * damage);
    body_add_damage(ai->comp_body, -damage);
  } else {
    double damage = body_damage_collisions(ai->comp_body);
    ai->health -= (character_multiplier * damage);
    body_add_damage(ai->comp_body, -damage);
  }
}

double computer_health(computer_t *ai) { return ai->health; }

vector_t computer_vector_facing(computer_t *ai) {
  return vec_rotate((vector_t){1, 0}, body_get_rotation(ai->comp_body));
}

void computer_shoot(scene_t *scene, computer_t *ai) {
  weapon_shoot(scene, ai->weapon,
               vec_get_unit_vector(computer_vector_facing(ai), VEC_ZERO),
               ai->comp_body, body_get_rotation(ai->comp_body),
               COMP_BULLET_MASS);
}

double computer_distance_to(computer_t *ai, character_t *enemy) {
  vector_t vec_dist = vec_subtract(body_get_centroid(character_get_body(enemy)),
                                   body_get_centroid(get_comp_body(ai)));
  double scalar_dist = vec_scalar(vec_dist);
  return scalar_dist;
}

void computer_set_velocity(computer_t *ai, vector_t direction) {
  body_set_velocity(
      ai->comp_body,
      vec_multiply(ENEMY_SPEED * ai->speed_multiplier, direction));
}

double computer_get_health(computer_t *ai) { return ai->health; }

bool computer_is_alive(computer_t *ai) { return ai->health > 0.0; }

void computer_cooldown_weapon(computer_t *ai, double cooldown_decrement) {
  weapon_cooldown(ai->weapon, cooldown_decrement);
}

void computer_reload(computer_t *ai) { weapon_reload(ai->weapon); }

void computer_decrement_reload(computer_t *ai, double reload_decrement) {
  weapon_decrement_reload(ai->weapon, reload_decrement);
}

bool computer_is_reloading_curr(computer_t *ai) {
  return weapon_is_reloading(ai->weapon);
}

bool computer_is_on_cooldown(computer_t *ai) {
  return !weapon_off_cooldown(ai->weapon);
}

bool computer_weapon_ammo(computer_t *ai) {
  weapon_t *weapon = computer_get_weapon(ai);
  return weapon_has_ammo(weapon);
}

void computer_free(computer_t *ai) {
  free(ai->weapon);
  free(ai);
}

void computer_rotate(computer_t *ai, character_t *character) {
  vector_t ai_center = body_get_centroid(get_comp_body(ai));
  vector_t user_center = body_get_centroid(character_get_body(character));
  vector_t direction = vec_subtract(user_center, ai_center);
  double angle = atan2(direction.y, direction.x);
  body_set_rotation(ai->comp_body, angle);
}

vector_t computer_direction(computer_t *ai, character_t *character) {
  vector_t ai_center = body_get_centroid(get_comp_body(ai));
  vector_t user_center = body_get_centroid(character_get_body(character));
  vector_t direction = vec_subtract(user_center, ai_center);
  return direction;
}

double computer_get_xp(computer_t *ai) { return ai->xp_offered; }

char *comp_switch_file(style_info_t style) {
  char *file = malloc(100 * sizeof(char));
  switch (style) {
  case BRUTE: {
    file = "assets/brute2.png";
    return file;
  }
  case SNIPER: {
    file = "assets/sniper2.png";
    return file;
  }
  case GUNMAN: {
    file = "assets/gunman2.png";
    return file;
  }
  case HENCHMAN: {
    file = "assets/henchmen.png";
    return file;
  }
  }
}