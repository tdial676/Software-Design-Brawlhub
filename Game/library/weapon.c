#include "weapon.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct weapon {
  weapon_info_t type;
  bullet_info_t bullet_type;
  double ammo;
  double shot_cooldown;
  double reload_time;
  bool is_reloading;
} weapon_t;

const double PISTOL_COOLDOWN = 1.0;
const double SHOTGUN_COOLDOWN = 2.0;
const double ASSAULT_COOLDOWN = 1.0;
const double SNIPER_COOLDOWN = 3.0;

const double PISTOL_RELOAD = 2.0;
const double SHOTGUN_RELOAD = 5.0;
const double ASSAULT_RELOAD = 3.0;
const double SNIPER_RELOAD = 7.0;

const double PISTOL_AMMO = 10.0;
const double SHOTGUN_AMMO = 6.0;
const double ASSAULT_AMMO = 30.0;
const double SNIPER_AMMO = 6.0;

const double BULLET_SPEED = 1000;
const double BULLET_MASS = 150;
const double BULLET_LENGTH = 7;
const double BULLET_WIDTH = 3;

const double PISTOL_DAMAGE = 10;
const double SHOTGUN_DAMAGE = 6;
const double ASSAULT_DAMAGE = 7;
const double SNIPER_DAMAGE = 45;

const double PISTOL_DRAG = 400;
const double SHOTGUN_DRAG = 1100;
const double ASSAULT_DRAG = 250;
const double SNIPER_DRAG = 175;

const double SHOTGUN_SPREAD = M_PI / 30;

rgb_color_t set_bullet_color(bullet_info_t type) {
  switch (type) {
  case PISTOL_BULLET: {
    return switch_color(9);
  }
  case SHOTGUN_BULLET: {
    return switch_color(1);
  }
  case ASSAULT_RIFLE_BULLET: {
    return switch_color(7);
  }
  case SNIPER_BULLET: {
    return switch_color(5);
  }
  }
}

double set_bullet_drag(bullet_info_t type) {
  switch (type) {
  case PISTOL_BULLET: {
    return PISTOL_DRAG;
  }
  case SHOTGUN_BULLET: {
    return SHOTGUN_DRAG;
  }
  case ASSAULT_RIFLE_BULLET: {
    return ASSAULT_DRAG;
  }
  case SNIPER_BULLET: {
    return SNIPER_DRAG;
  }
  }
}

weapon_t *weapon_init(weapon_info_t type) {
  weapon_t *weapon = malloc(sizeof(weapon_t));
  weapon->type = type;
  switch (type) {
  case PISTOL: {
    weapon->bullet_type = PISTOL_BULLET;
    weapon->ammo = PISTOL_AMMO;
    break;
  }
  case SHOTGUN: {
    weapon->bullet_type = SHOTGUN_BULLET;
    weapon->ammo = SHOTGUN_AMMO;
    break;
  }
  case ASSAULT_RIFLE: {
    weapon->bullet_type = ASSAULT_RIFLE_BULLET;
    weapon->ammo = ASSAULT_AMMO;
    break;
  }
  case SNIPER_RIFLE: {
    weapon->bullet_type = SNIPER_BULLET;
    weapon->ammo = SNIPER_AMMO;
    break;
  }
  }
  weapon->shot_cooldown = 0.0;
  weapon->reload_time = 0.0;
  weapon->is_reloading = false;
  return weapon;
}

bool weapon_has_ammo(weapon_t *weapon) { return weapon->ammo > 0; }

double weapon_ammo(weapon_t *weapon) { return weapon->ammo; }

double get_weapon_ammo(weapon_info_t type) {
  switch (type) {
  case PISTOL: {
    return PISTOL_AMMO;
  }
  case SHOTGUN: {
    return SHOTGUN_AMMO;
  }
  case ASSAULT_RIFLE: {
    return ASSAULT_AMMO;
  }
  case SNIPER_RIFLE: {
    return SNIPER_AMMO;
  }
  }
  return 0;
}

bool weapon_off_cooldown(weapon_t *weapon) {
  return weapon->shot_cooldown <= 0.0;
}

double weapon_cooldown_timer(weapon_t *weapon) { return weapon->shot_cooldown; }

void weapon_cooldown(weapon_t *weapon, double timer_decrement) {
  weapon->shot_cooldown -= timer_decrement;
  weapon->shot_cooldown = weapon->shot_cooldown < 0 ? 0 : weapon->shot_cooldown;
}

double get_weapon_cooldown(weapon_info_t type) {
  switch (type) {
  case PISTOL: {
    return PISTOL_COOLDOWN;
  }
  case SHOTGUN: {
    return SHOTGUN_COOLDOWN;
  }
  case ASSAULT_RIFLE: {
    return ASSAULT_COOLDOWN;
  }
  case SNIPER_RIFLE: {
    return SNIPER_COOLDOWN;
  }
  }
  return 0;
}

double get_weapon_reload(weapon_info_t type) {
  switch (type) {
  case PISTOL: {
    return PISTOL_RELOAD;
  }
  case SHOTGUN: {
    return SHOTGUN_RELOAD;
  }
  case ASSAULT_RIFLE: {
    return ASSAULT_RELOAD;
  }
  case SNIPER_RIFLE: {
    return SNIPER_RELOAD;
  }
  }
}

bool weapon_is_reloading(weapon_t *weapon) { return weapon->is_reloading; }

void weapon_reload(weapon_t *weapon) {
  if (weapon_has_ammo(weapon) || !weapon_is_reloading(weapon)) {
    weapon->ammo = 0.0;
    weapon->reload_time = get_weapon_reload(weapon->type);
    weapon->is_reloading = true;
  }
  if (weapon->reload_time <= 0) {
    weapon->ammo = get_weapon_ammo(weapon->type);
    weapon->reload_time = 0.0;
    weapon->is_reloading = false;
  }
}

void weapon_decrement_reload(weapon_t *weapon, double timer_decrement) {
  weapon->reload_time -= timer_decrement;
  weapon->reload_time = weapon->reload_time < 0 ? 0 : weapon->reload_time;
}

double weapon_reload_timer(weapon_t *weapon) { return weapon->reload_time; }

void shotgun_shoot(scene_t *scene, weapon_t *weapon, vector_t dir,
                   body_t *shooter, double mass) {
  computer_info_t *shooter_info = body_get_info(shooter);
  list_t *obstacles = scene_bodies_with_comp_info(scene, OBSTACLE);
  list_t *enemies = scene_bodies_with_comp_info(scene, ENEMY);
  list_t *characters = scene_bodies_with_comp_info(scene, CHARACTER);
  list_t *shields = scene_bodies_with_comp_info(scene, SHIELD);
  for (size_t i = 1; i <= (size_t)SHOTGUN_AMMO; i++) {
    bullet_info_t *bullet_info = malloc(sizeof(bullet_info_t));
    *bullet_info = weapon->bullet_type;
    vector_t *bullet_center = malloc(sizeof(vector_t));
    *bullet_center = body_get_centroid(shooter);
    body_t *bullet = body_init_with_info(
        create_sector(BULLET_WIDTH, bullet_center, 2 * M_PI, false), mass * 2,
        set_bullet_color(weapon->bullet_type), bullet_info, free);
    if (i % 2) {
      body_set_velocity(
          bullet,
          vec_multiply(BULLET_SPEED, vec_rotate(dir, SHOTGUN_SPREAD * i)));
    } else {
      body_set_velocity(
          bullet,
          vec_multiply(BULLET_SPEED, vec_rotate(dir, -SHOTGUN_SPREAD * i)));
    }
    scene_add_body(scene, bullet);
    create_drag(scene, set_bullet_drag(weapon->bullet_type), bullet);
    for (size_t i = 0; i < list_size(obstacles); i++) {
      body_t *obstacle = list_get(obstacles, i);
      create_solo_destructive_collision(scene, obstacle, bullet);
    }
    if (*shooter_info == CHARACTER) {
      for (size_t i = 0; i < list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        create_damaging_collision(scene, enemy, bullet);
      }
    } else if (*shooter_info == ENEMY) {
      for (size_t i = 0; i < list_size(characters); i++) {
        body_t *character = list_get(characters, i);
        create_damaging_collision(scene, character, bullet);
      }
      for (size_t i = 0; i < list_size(shields); i++) {
        body_t *shield = list_get(shields, i);
        create_damaging_collision(scene, shield, bullet);
      }
    }
  }
  list_free(shields);
  list_free(obstacles);
  list_free(enemies);
  list_free(characters);
}

void weapon_shoot(scene_t *scene, weapon_t *weapon, vector_t dir,
                  body_t *shooter, double orientation,
                  double mass) { // added mass
  if ((!weapon_has_ammo(weapon) || !weapon_off_cooldown(weapon) ||
       weapon_is_reloading(weapon)) &&
      mass <= BULLET_MASS) {
    return;
  }
  if (weapon->type == SHOTGUN) {
    // SHOT SPREAD POWERUP
    if (mass > BULLET_MASS) {
      shotgun_shoot(scene, weapon, vec_negate(dir), shooter, mass);
      shotgun_shoot(scene, weapon, vec_rotate(dir, M_PI / 2), shooter, mass);
      shotgun_shoot(scene, weapon, vec_rotate(dir, -M_PI / 2), shooter, mass);
    }
    shotgun_shoot(scene, weapon, dir, shooter, mass);
  } else {
    vector_t center = body_get_centroid(shooter);
    computer_info_t *shooter_info = body_get_info(shooter);
    bullet_info_t *bullet_info = malloc(sizeof(bullet_info_t));
    *bullet_info = weapon->bullet_type;
    body_t *bullet = body_init_with_info(
        create_four_sided_shape(center, BULLET_LENGTH, BULLET_WIDTH), mass,
        set_bullet_color(weapon->bullet_type), bullet_info, free);
    body_set_rotation(bullet, orientation);
    body_set_velocity(bullet, vec_multiply(BULLET_SPEED, dir));
    scene_add_body(scene, bullet);
    create_drag(scene, set_bullet_drag(weapon->bullet_type), bullet);
    list_t *obstacles = scene_bodies_with_comp_info(scene, OBSTACLE);
    for (size_t i = 0; i < list_size(obstacles); i++) {
      body_t *obstacle = list_get(obstacles, i);
      create_solo_destructive_collision(scene, obstacle, bullet);
    }
    if (*shooter_info == CHARACTER) {
      list_t *enemies = scene_bodies_with_comp_info(scene, ENEMY);
      for (size_t i = 0; i < list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        if (mass > BULLET_MASS) {
          // Homing bullet that draws enemies
          body_set_velocity(bullet, vec_multiply(400, dir));
          create_newtonian_gravity(scene, 100000, bullet, enemy);
        }
        create_damaging_collision(scene, enemy, bullet);
      }
      list_free(enemies);
    } else if (*shooter_info == ENEMY) {
      list_t *characters = scene_bodies_with_comp_info(scene, CHARACTER);
      for (size_t i = 0; i < list_size(characters); i++) {
        body_t *character = list_get(characters, i);
        create_damaging_collision(scene, character, bullet);
      }
      list_free(characters);
      list_t *shields = scene_bodies_with_comp_info(scene, SHIELD);
      for (size_t i = 0; i < list_size(shields); i++) {
        body_t *shield = list_get(shields, i);
        create_damaging_collision(scene, shield, bullet);
      }
      list_free(shields);
    }
    list_free(obstacles);
  }
  if (mass <= BULLET_MASS) {
    weapon->ammo--;
    weapon->shot_cooldown = get_weapon_cooldown(weapon->type);
  }
}

weapon_info_t weapon_get_type(weapon_t *weapon) { return weapon->type; }
