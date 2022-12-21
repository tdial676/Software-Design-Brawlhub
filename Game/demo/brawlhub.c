#include "character.h"
#include "computer.h"
#include "forces.h"
#include "key_handler.h"
#include "polygon.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const size_t SCREEN_WIDTH = 1000;
const size_t SCREEN_HEIGHT = 500;
const double SPACING_BOUNDS = 50;
const double ELASTICITY = 0;

const size_t ENEMY_WIDTH = 30;
const size_t ENEMY_HEIGHT = 30;

const size_t OBSTACLE_WIDTH = 50;
const size_t OBSTACLE_HEIGHT = 50;
const double MIN_OBSTACLE_DISTANCE = 100.0;

const size_t FREE_RANGE_WIDTH = 800;
const size_t FREE_RANGE_HEIGHT = 600;

const size_t MAX_WIDTH = 2000;
const size_t MAX_HEIGHT = 1000;

const vector_t INIT_CENTER =
    (vector_t){.x = MAX_WIDTH / 2, .y = MAX_HEIGHT / 2};

const double USER_WIDTH = 30;
const double USER_HEIGHT = 30;
const double SHOOTER_TIME = 0.5;

const double DRAG_FACTOR = 300.0;
const double BULLET_DELETE_SPEED = 250.0;
const double SPECIAL_BULLET_DELETE_SPEED = 20.0;

const size_t STRING_BUFFER = 100;
const double SPEED_DMG_COST = 25;
const double REGEN_COST = 75;
const double HEALTH_COST = 50;

const rgb_color_t START_COLOR = (rgb_color_t){0.7529, 0.7529, 0.7529, 1.0};
const SDL_Color TEXT_COLOR = (SDL_Color){153, 0, 0, 255};
const char *FONT = "assets/Lato-Black.ttf";

const size_t MAX_WAVE = 20;
const double BRUTE_SHOOT_DISTANCE = 150.0;
const double SNIPER_SHOOT_DISTANCE = 450.0;
const double GUNMAN_SHOOT_DISTANCE = 350.0;
const double HENCHMAN_SHOOT_DISTANCE = 200.0;

const double GUNMAN_POWERUP_COST = 12.5;
const double SNIPER_POWERUP_COST = 7.0;
const double BRUTE_POWERUP_COST = 10.0;

const double STANDARD_BULLET_MASS = 150.0;
const double SPECIAL_BULLET_MASS = 15100.0;

const double SHIELD_HEALTH = 500.0;

const double STANDARD_RACKS_PER_KILL = 10.0;
const double BOSS_BONUS_RACKS = 300.0;
const double BOSS_BONUS_XP = 150.0;
const size_t BOSS_OCCURENCE = 5;

typedef enum {
  START_SCENE = 1,
  GAME_SCENE = 2,
  PAUSE_SCENE = 3
} current_scene_t;

typedef struct state {
  scene_t *game_scene;
  scene_t *start_scene;
  scene_t *pause_scene;
  list_t *computers;
  list_t *obstacles;
  character_t *user;
  style_info_t user_style;
  current_scene_t current_scene;
  body_t *pause_background;
  body_t *start_background;
  size_t wave_count;
  wave_info_t wave_dmg_multiplier;
  double current_xp;
  double character_heal_timer;
  double racks;
} state_t;

bool exit_out_of_game(state_t *state) {
  if (!character_is_alive(state->user) || state->wave_count > MAX_WAVE) {
    scene_t *old_game = state->game_scene;
    list_t *enemies = state->computers;
    list_t *obstacles = state->obstacles;
    state->computers = list_init(5, (free_func_t)computer_free);
    scene_free(old_game);
    list_free(enemies);
    list_free(obstacles);
    vector_t start_center = body_get_centroid(state->start_background);
    sdl_set_center(start_center);
    sdl_set_max((vector_t){.x = start_center.x + SCREEN_WIDTH / 2,
                           .y = start_center.y + SCREEN_HEIGHT / 2});
    sdl_set_min((vector_t){.x = start_center.x - SCREEN_WIDTH / 2,
                           .y = start_center.y - SCREEN_HEIGHT / 2});
    state->current_scene = START_SCENE;
    return true;
  }
  return false;
}

style_info_t get_boss(size_t wave_count) {
  switch (wave_count) {
  case 5: {
    return HENCHMAN;
  }
  case 10: {
    return SNIPER;
  }
  case 15: {
    return GUNMAN;
  }
  case 20: {
    return BRUTE;
  }
  }
  return HENCHMAN;
}

char *get_weapon_sound(weapon_info_t weapon_type) {
  char *file_name = malloc(STRING_BUFFER * sizeof(char));
  switch (weapon_type) {
  case PISTOL: {
    file_name = "assets/PISTOL.wav";
    break;
  }
  case SHOTGUN: {
    file_name = "assets/SHOTGUN.wav";
    break;
  }
  case ASSAULT_RIFLE: {
    file_name = "assets/AR.wav";
    break;
  }
  case SNIPER_RIFLE:
    file_name = "assets/SNIPER.wav";
    break;
  }
  return file_name;
}

char *get_reload_sound(weapon_info_t weapon_type) {
  char *file_name = malloc(STRING_BUFFER * sizeof(char));
  switch (weapon_type) {
  case PISTOL: {
    file_name = "assets/PISTOL_RELOAD.wav";
    break;
  }
  case SHOTGUN: {
    file_name = "assets/SHOTGUN_RELOAD.wav";
    break;
  }
  case ASSAULT_RIFLE: {
    file_name = "assets/AR_RELOAD.wav";
    break;
  }
  case SNIPER_RIFLE:
    file_name = "assets/SNIPER_RELOAD.wav";
    break;
  }
  return file_name;
}

void add_enemies_to_scene(state_t *state, bool is_boss, size_t init_size) {
  for (size_t i = 0; i < init_size;
       i++) { // placeholder code, will refactor this
    vector_t rand_center = {
        (rand() % ((MAX_WIDTH - ENEMY_WIDTH) - ENEMY_WIDTH + 1)) + ENEMY_WIDTH,
        (rand() % ((MAX_HEIGHT - ENEMY_HEIGHT) - ENEMY_HEIGHT + 1)) +
            ENEMY_HEIGHT}; // placeholder, will add function that does
                           // not spawn enemies too close
    list_t *vertices =
        create_four_sided_shape(rand_center, ENEMY_WIDTH, ENEMY_HEIGHT);
    computer_info_t *enemy_info = malloc(sizeof(computer_info_t));
    *enemy_info = ENEMY;
    style_info_t enemy_style =
        !is_boss ? (rand() + i) % 4 + 1 : get_boss(state->wave_count);
    computer_t *enemy =
        computer_init(state->game_scene, is_boss, enemy_style, enemy_info,
                      vertices); // CHANGE
    list_add(state->computers, enemy);
    for (size_t j = 0; j < list_size(state->obstacles); j++) {
      create_physics_collision(state->game_scene, ELASTICITY,
                               get_comp_body(enemy),
                               list_get(state->obstacles, j));
    }
  }
}

double max_enemy_to_distance(style_info_t style) {
  double max_distance;
  switch (style) {
  case BRUTE:
    max_distance = BRUTE_SHOOT_DISTANCE;
    break;
  case GUNMAN:
    max_distance = GUNMAN_SHOOT_DISTANCE;
    break;
  case SNIPER:
    max_distance = SNIPER_SHOOT_DISTANCE;
    break;
  case HENCHMAN:
    max_distance = HENCHMAN_SHOOT_DISTANCE;
    break;
  }
  return max_distance;
}

void create_boundaries(scene_t *game_scene, body_t *user) {
  // we add two tiles of infinite mass to the sides of the the demo and create a
  // physics collision with the ball.
  computer_info_t *info_left = malloc(sizeof(computer_info_t));
  *info_left = OBSTACLE;
  body_t *left_boundary =
      body_init_with_info(create_four_sided_shape((vector_t){0, MAX_HEIGHT / 2},
                                                  SPACING_BOUNDS, MAX_HEIGHT),
                          INFINITY, switch_color(9), info_left, free);
  computer_info_t *info_right = malloc(sizeof(computer_info_t));
  *info_right = OBSTACLE;
  body_t *right_boundary = body_init_with_info(
      create_four_sided_shape((vector_t){MAX_WIDTH, MAX_HEIGHT / 2},
                              SPACING_BOUNDS, MAX_HEIGHT),
      INFINITY, switch_color(9), info_right, free);
  computer_info_t *info_top = malloc(sizeof(computer_info_t));
  *info_top = OBSTACLE;
  body_t *top_boundary = body_init_with_info(
      create_four_sided_shape((vector_t){MAX_WIDTH / 2, MAX_HEIGHT}, MAX_WIDTH,
                              SPACING_BOUNDS),
      INFINITY, switch_color(9), info_top, free);
  computer_info_t *info_bottom = malloc(sizeof(computer_info_t));
  *info_bottom = OBSTACLE;
  body_t *bottom_boundary =
      body_init_with_info(create_four_sided_shape((vector_t){MAX_WIDTH / 2, 0},
                                                  MAX_WIDTH, SPACING_BOUNDS),
                          INFINITY, switch_color(9), info_bottom, free);
  scene_add_body(game_scene, left_boundary);
  scene_add_body(game_scene, right_boundary);
  scene_add_body(game_scene, top_boundary);
  scene_add_body(game_scene, bottom_boundary);
  create_physics_collision(game_scene, ELASTICITY, user, left_boundary);
  create_physics_collision(game_scene, ELASTICITY, user, right_boundary);
  create_physics_collision(game_scene, ELASTICITY, user, top_boundary);
  create_physics_collision(game_scene, ELASTICITY, user, bottom_boundary);
}

list_t *obstacle_get_corner_centroid(size_t corner) {
  assert(corner >= 0 && corner < 4);
  list_t *centroids = list_init(2, (free_func_t)free);
  vector_t *center1 = malloc(sizeof(vector_t));
  vector_t *center2 = malloc(sizeof(vector_t));
  switch (corner) {
  case 0: {
    *center1 = (vector_t){300, 825};
    *center2 = (vector_t){225, 750};
    list_add(centroids, center1);
    list_add(centroids, center2);
    break;
  }
  case 1: {
    *center1 = (vector_t){1700, 825};
    *center2 = (vector_t){1775, 750};
    list_add(centroids, center1);
    list_add(centroids, center2);
    break;
  }
  case 2: {
    *center1 = (vector_t){300, 175};
    *center2 = (vector_t){225, 250};
    list_add(centroids, center1);
    list_add(centroids, center2);
    break;
  }
  case 3: {
    *center1 = (vector_t){1700, 175};
    *center2 = (vector_t){1775, 250};
    list_add(centroids, center1);
    list_add(centroids, center2);
    break;
  }
  }
  return centroids;
}

void build_scene_plus(state_t *state, vector_t center) {
  computer_info_t *vertical_info = malloc(sizeof(computer_info_t));
  *vertical_info = OBSTACLE;
  computer_info_t *horizontal_info = malloc(sizeof(computer_info_t));
  *horizontal_info = OBSTACLE;
  body_t *vertical =
      body_init_with_info(create_four_sided_shape(center, 50, 150), INFINITY,
                          switch_color(9), vertical_info, free);
  body_t *horizontal =
      body_init_with_info(create_four_sided_shape(center, 150, 50), INFINITY,
                          switch_color(9), horizontal_info, free);
  scene_add_body(state->game_scene, vertical);
  scene_add_body(state->game_scene, horizontal);
  list_add(state->obstacles, vertical);
  list_add(state->obstacles, horizontal);
}

void add_obstacles(state_t *state, body_t *user) {
  size_t num_obstacles = 14; // abstract later
  // NULL freer below since scene frees bodies
  state->obstacles = list_init(num_obstacles, NULL);
  // BUILD CORNER BLOCKS
  for (size_t i = 0; i < 4; i++) {
    list_t *centers = obstacle_get_corner_centroid(i);
    for (size_t i = 0; i < list_size(centers); i++) {
      computer_info_t *corner_info = malloc(sizeof(computer_info_t));
      *corner_info = OBSTACLE;
      vector_t *center = (vector_t *)list_get(centers, i);
      body_t *obstacle;
      if (i == 0) {
        obstacle =
            body_init_with_info(create_four_sided_shape(*center, 200, 50),
                                INFINITY, switch_color(9), corner_info, free);
      } else {
        obstacle =
            body_init_with_info(create_four_sided_shape(*center, 50, 200),
                                INFINITY, switch_color(9), corner_info, free);
      }
      scene_add_body(state->game_scene, obstacle);
      list_add(state->obstacles, obstacle);
    }
    list_free(centers);
  }
  // BUILD PLUSES
  build_scene_plus(state, (vector_t){550, 500});
  build_scene_plus(state, (vector_t){1450, 500});
  // BUILD TOP AND BOTTOM BOXES
  computer_info_t *top_info = malloc(sizeof(computer_info_t));
  *top_info = OBSTACLE;
  computer_info_t *bottom_info = malloc(sizeof(computer_info_t));
  *bottom_info = OBSTACLE;
  body_t *top = body_init_with_info(
      create_four_sided_shape((vector_t){1000, 800}, 200, 200), INFINITY,
      switch_color(9), top_info, free);
  body_t *bottom = body_init_with_info(
      create_four_sided_shape((vector_t){1000, 200}, 200, 200), INFINITY,
      switch_color(9), bottom_info, free);
  scene_add_body(state->game_scene, top);
  scene_add_body(state->game_scene, bottom);
  list_add(state->obstacles, top);
  list_add(state->obstacles, bottom);
  for (size_t i = 0; i < list_size(state->obstacles); i++) {
    create_physics_collision(state->game_scene, ELASTICITY, user,
                             list_get(state->obstacles, i));
  }
}

void game_scene_init(state_t *output) {
  output->game_scene = scene_init();
  computer_info_t *background_info = malloc(sizeof(computer_info_t));
  *background_info = BACKGROUND;
  body_t *background = body_init_with_info(
      create_four_sided_shape(INIT_CENTER, MAX_WIDTH + SCREEN_WIDTH,
                              MAX_HEIGHT + SCREEN_HEIGHT),
      1.0, switch_color(9), background_info, free);
  scene_add_body(output->game_scene, background);
  computer_info_t *floor_info = malloc(sizeof(computer_info_t));
  *floor_info = FLOOR;
  body_t *floor = body_init_with_info(
      create_four_sided_shape(INIT_CENTER, MAX_WIDTH, MAX_HEIGHT), 1.0,
      INTERNAL_BODY_COLOR, floor_info, free);
  scene_add_body(output->game_scene, floor);
  computer_info_t *comp_info = malloc(sizeof(computer_info_t));
  *comp_info = CHARACTER;
  output->user = character_init(
      output->game_scene,
      create_four_sided_shape(INIT_CENTER, USER_WIDTH, USER_HEIGHT), comp_info,
      output->user_style); // CHARACTER
  create_drag(output->game_scene, DRAG_FACTOR,
              character_get_body(output->user));
  create_boundaries(output->game_scene, character_get_body(output->user));
  add_obstacles(output, character_get_body(output->user));
  output->wave_count = 0;
  output->wave_dmg_multiplier = 1;
  output->current_xp = 0;
  output->racks = 0;
}

void pause_scene_init(state_t *output) {
  scene_t *pause_scene = scene_init();
  list_t *background_points = create_four_sided_shape(
      vec_multiply(0.5, (vector_t)INIT_CENTER), SCREEN_WIDTH, SCREEN_HEIGHT);
  body_t *background = body_init(background_points, INFINITY, START_COLOR);
  scene_add_body(pause_scene, background);
  output->pause_scene = pause_scene;
  output->pause_background = background;
}

void start_scene_init(state_t *output) {
  scene_t *start_scene = scene_init();
  list_t *background_points = create_four_sided_shape(
      vec_multiply(0.5, (vector_t)INIT_CENTER), SCREEN_WIDTH, SCREEN_HEIGHT);
  body_t *background = body_init(background_points, INFINITY, START_COLOR);
  scene_add_body(start_scene, background);
  output->start_scene = start_scene;
  output->start_background = background;
}

void keep_within_boundaries(character_t *user) {
  body_t *body = character_get_body(user);
  vector_t centroid = body_get_centroid(body);
  if (centroid.x - USER_WIDTH < 0) {
    body_set_centroid(body,
                      (vector_t){USER_WIDTH + SPACING_BOUNDS, centroid.y});
    character_set_velocity(user, VEC_ZERO);
  } else if (centroid.x + USER_WIDTH > MAX_WIDTH) {
    body_set_centroid(
        body, (vector_t){MAX_WIDTH - USER_WIDTH - SPACING_BOUNDS, centroid.y});
    character_set_velocity(user, VEC_ZERO);
  } else if (centroid.y - USER_HEIGHT < 0) {
    body_set_centroid(body,
                      (vector_t){centroid.x, USER_HEIGHT + SPACING_BOUNDS});
    character_set_velocity(user, VEC_ZERO);
  } else if (centroid.y + USER_HEIGHT > MAX_HEIGHT) {
    body_set_centroid(body, (vector_t){centroid.x, MAX_HEIGHT - USER_HEIGHT -
                                                       SPACING_BOUNDS});
    character_set_velocity(user, VEC_ZERO);
  }
}

void computer_keep_within_boundaries(computer_t *ai) {
  body_t *body = get_comp_body(ai);
  vector_t centroid = body_get_centroid(body);
  if (centroid.x - ENEMY_WIDTH < 0) {
    body_set_centroid(body,
                      (vector_t){ENEMY_WIDTH + SPACING_BOUNDS, centroid.y});
    computer_set_velocity(ai, VEC_ZERO);
  } else if (centroid.x + ENEMY_WIDTH > MAX_WIDTH) {
    body_set_centroid(
        body, (vector_t){MAX_WIDTH - ENEMY_WIDTH - SPACING_BOUNDS, centroid.y});
    computer_set_velocity(ai, VEC_ZERO);
  } else if (centroid.y - ENEMY_HEIGHT < 0) {
    body_set_centroid(body,
                      (vector_t){centroid.x, ENEMY_HEIGHT + SPACING_BOUNDS});
    computer_set_velocity(ai, VEC_ZERO);
  } else if (centroid.y + ENEMY_HEIGHT > MAX_HEIGHT) {
    body_set_centroid(body, (vector_t){centroid.x, MAX_HEIGHT - ENEMY_HEIGHT -
                                                       SPACING_BOUNDS});
    computer_set_velocity(ai, VEC_ZERO);
  }
}

// KEY STUFF START
void game_key(char key, key_event_type_t type, double held_time,
              state_t *state) {
  character_t *user = state->user;
  body_t *player = character_get_body(user);
  assert(player != NULL);
  if (type == KEY_PRESSED) {
    switch (key) {
    case 'w': {
      fob_move(user, 1);
      break;
    }
    case 's': {
      fob_move(user, -1);
      break;
    }
    case 'a': {
      lor_move(user, -1);
      break;
    }
    case 'd': {
      lor_move(user, 1);
      break;
    }
    case 'r': {
      character_reload(state->user);
      sdl_play_sound(
          get_reload_sound(weapon_get_type(character_weapon(state->user))));
      break;
    }
    case 'q': {
      character_switch_weapon(state->user);
      break;
    }
    case 't': {
      style_info_t style = character_style(state->user);
      switch (style) {
      case GUNMAN:
        if (state->current_xp >= GUNMAN_POWERUP_COST) {
          create_shield(state->game_scene, state->user);
          state->current_xp -= GUNMAN_POWERUP_COST;
        }
        break;
      case SNIPER:
        if (state->current_xp >= SNIPER_POWERUP_COST) {
          weapon_info_t weapon_type =
              weapon_get_type(character_weapon(state->user));
          if (weapon_type != SNIPER_RIFLE) {
            character_switch_weapon(state->user);
          }
          character_shoot(state->game_scene, state->user,
                          SPECIAL_BULLET_MASS); // MAGICIZE
          if (weapon_type != SNIPER_RIFLE) {
            character_switch_weapon(state->user);
          }
          state->current_xp -= SNIPER_POWERUP_COST;
        }
        break;
      case BRUTE:
        if (state->current_xp >= BRUTE_POWERUP_COST) {
          weapon_info_t weapon_type =
              weapon_get_type(character_weapon(state->user));
          if (weapon_type != SHOTGUN) {
            character_switch_weapon(state->user);
          }
          character_shoot(state->game_scene, state->user, SPECIAL_BULLET_MASS);
          if (weapon_type != SHOTGUN) {
            character_switch_weapon(state->user);
          }
          state->current_xp -= BRUTE_POWERUP_COST;
        }
        break;
      default:
        break;
      }
      break;
    }
    case 'p': {
      character_set_velocity(state->user,
                             VEC_ZERO); // SET PLAYER VELOCITY TO ZERO
      // SET BULLET VELOCITIES TO ZERO
      list_t *pistol_bullets =
          scene_bodies_with_bullet_info(state->game_scene, PISTOL_BULLET);
      list_t *shotgun_bullets =
          scene_bodies_with_bullet_info(state->game_scene, SHOTGUN_BULLET);
      list_t *assault_bullets = scene_bodies_with_bullet_info(
          state->game_scene, ASSAULT_RIFLE_BULLET);
      list_t *sniper_bullets =
          scene_bodies_with_bullet_info(state->game_scene, SNIPER_BULLET);
      for (size_t i = 0; i < list_size(pistol_bullets); i++) {
        body_t *bullet = list_get(pistol_bullets, i);
        body_set_velocity(bullet, VEC_ZERO);
      }
      for (size_t i = 0; i < list_size(shotgun_bullets); i++) {
        body_t *bullet = list_get(shotgun_bullets, i);
        body_set_velocity(bullet, VEC_ZERO);
      }
      for (size_t i = 0; i < list_size(assault_bullets); i++) {
        body_t *bullet = list_get(assault_bullets, i);
        body_set_velocity(bullet, VEC_ZERO);
      }
      for (size_t i = 0; i < list_size(sniper_bullets); i++) {
        body_t *bullet = list_get(sniper_bullets, i);
        body_set_velocity(bullet, VEC_ZERO);
      }
      list_free(pistol_bullets);
      list_free(shotgun_bullets);
      list_free(assault_bullets);
      list_free(sniper_bullets);
      // SET COMPUTER VELOCITIES TO ZERO
      for (size_t i = 0; i < list_size(state->computers); i++) {
        computer_t *ai = list_get(state->computers, i);
        computer_set_velocity(ai, VEC_ZERO);
      }
      // SET WINDOW CENTER TO BACKGROUND FOR PAUSE SCREEN
      vector_t pause_center = body_get_centroid(state->pause_background);
      sdl_set_center(pause_center);
      sdl_set_max((vector_t){.x = pause_center.x + SCREEN_WIDTH / 2,
                             .y = pause_center.y + SCREEN_HEIGHT / 2});
      sdl_set_min((vector_t){.x = pause_center.x - SCREEN_WIDTH / 2,
                             .y = pause_center.y - SCREEN_HEIGHT / 2});
      state->current_scene = PAUSE_SCENE;
      break;
    }
    }
  } else if (type == MOUSE_PRESSED) {
    if (!weapon_has_ammo(character_weapon(state->user)) ||
        character_is_reloading_curr(state->user) ||
        character_is_on_cooldown(state->user)) {
      sdl_play_sound("assets/EMPTY_CLIP.wav");
    } else {
      character_shoot(state->game_scene, state->user, STANDARD_BULLET_MASS);
      sdl_play_sound(
          get_weapon_sound(weapon_get_type(character_weapon(state->user))));
    }
  } else if (type == MOUSE_MOTION) {
    double angle = get_rotation_character(state->user);
    character_set_rotation(state->user, angle);
  }
  // KEY STUFF END
}

void pause_key(char key, key_event_type_t type, double held_time,
               state_t *state) {
  if (type == KEY_PRESSED) {
    switch (key) {
    case SDLK_SPACE: { // press space to resume playing
      state->current_scene = GAME_SCENE;
      break;
    }
    case SDLK_ESCAPE: { // press esc to reset
      scene_t *old_game = state->game_scene;
      list_t *enemies = state->computers;
      list_t *obstacles = state->obstacles;
      state->computers = list_init(5, (free_func_t)computer_free);
      scene_free(old_game);
      list_free(enemies);
      list_free(obstacles);
      vector_t start_center = body_get_centroid(state->start_background);
      sdl_set_center(start_center);
      sdl_set_max((vector_t){.x = start_center.x + SCREEN_WIDTH / 2,
                             .y = start_center.y + SCREEN_HEIGHT / 2});
      sdl_set_min((vector_t){.x = start_center.x - SCREEN_WIDTH / 2,
                             .y = start_center.y - SCREEN_HEIGHT / 2});
      state->current_scene = START_SCENE;
      break;
    }
    case 'g': {
      if (state->racks >= SPEED_DMG_COST &&
          character_get_speed_multiplier(state->user) < MAX_SPEED_MULT) {
        character_increase_speed(state->user);
        state->racks -= SPEED_DMG_COST;
      }
      break;
    }
    case 'h': {
      if (state->racks >= SPEED_DMG_COST &&
          character_get_dmg_multiplier(state->user) < MAX_DAMAGE_MULT) {
        character_increase_damage(state->user);
        state->racks -= SPEED_DMG_COST;
      }
      break;
    }
    case 'u': {
      if (state->racks >= REGEN_COST &&
          character_get_healing_factor(state->user) < MAX_REGENERATION) {
        character_increase_healing(state->user);
        state->racks -= REGEN_COST;
      }
      break;
    }
    case 'y': {
      if (state->racks >= HEALTH_COST) {
        character_increase_health(state->user);
        state->racks -= HEALTH_COST;
      }
      break;
    }
    }
  }
}

void start_key(char key, key_event_type_t type, double held_time,
               state_t *state) {
  if (type == KEY_PRESSED) {
    switch (key) {
    case SDLK_1: {
      state->user_style = BRUTE;
      game_scene_init(state);
      state->current_scene = GAME_SCENE;
      break;
    }
    case SDLK_2: {
      state->user_style = GUNMAN;
      game_scene_init(state);
      state->current_scene = GAME_SCENE;
      break;
    }
    case SDLK_3: {
      state->user_style = SNIPER;
      game_scene_init(state);
      state->current_scene = GAME_SCENE;
      break;
    }
    }
  }
}

void process_bullet_life(state_t *current) {
  list_t *pistol_bullets =
      scene_bodies_with_bullet_info(current->game_scene, PISTOL_BULLET);
  list_t *shotgun_bullets =
      scene_bodies_with_bullet_info(current->game_scene, SHOTGUN_BULLET);
  list_t *assault_bullets =
      scene_bodies_with_bullet_info(current->game_scene, ASSAULT_RIFLE_BULLET);
  list_t *sniper_bullets =
      scene_bodies_with_bullet_info(current->game_scene, SNIPER_BULLET);
  for (size_t i = 0; i < list_size(pistol_bullets); i++) {
    body_t *bullet = list_get(pistol_bullets, i);
    vector_t bullet_velocity = body_get_velocity(bullet);
    double magnitude = vec_scalar(bullet_velocity);
    if (magnitude < BULLET_DELETE_SPEED) {
      body_remove(bullet);
    }
  }
  for (size_t i = 0; i < list_size(shotgun_bullets); i++) {
    body_t *bullet = list_get(shotgun_bullets, i);
    vector_t bullet_velocity = body_get_velocity(bullet);
    double magnitude = vec_scalar(bullet_velocity);
    if (magnitude < BULLET_DELETE_SPEED) {
      body_remove(bullet);
    }
  }
  for (size_t i = 0; i < list_size(assault_bullets); i++) {
    body_t *bullet = list_get(assault_bullets, i);
    vector_t bullet_velocity = body_get_velocity(bullet);
    double magnitude = vec_scalar(bullet_velocity);
    if (magnitude < BULLET_DELETE_SPEED) {
      body_remove(bullet);
    }
  }
  for (size_t i = 0; i < list_size(sniper_bullets); i++) {
    body_t *bullet = list_get(sniper_bullets, i);
    vector_t bullet_velocity = body_get_velocity(bullet);
    double magnitude = vec_scalar(bullet_velocity);
    if (magnitude < BULLET_DELETE_SPEED &&
        body_get_mass(bullet) <= STANDARD_BULLET_MASS) {
      body_remove(bullet);
    } else if (magnitude < SPECIAL_BULLET_DELETE_SPEED) {
      body_remove(bullet);
    }
  }
  list_free(pistol_bullets);
  list_free(shotgun_bullets);
  list_free(assault_bullets);
  list_free(sniper_bullets);
}

void process_damages(state_t *current) {
  character_process_damage(current->user,
                           (double)(int)current->wave_dmg_multiplier);
  for (size_t i = 0; i < list_size(current->computers); i++) {
    computer_t *ai = list_get(current->computers, i);
    computer_process_damage(ai, character_get_dmg_multiplier(current->user));
    if (!computer_is_alive(ai)) {
      current->current_xp += computer_get_xp(ai);
      current->racks +=
          (STANDARD_RACKS_PER_KILL * (int)current->wave_dmg_multiplier);
      if (is_boss(ai)) {
        current->racks +=
            (BOSS_BONUS_RACKS * (int)current->wave_dmg_multiplier);
        current->current_xp += BOSS_BONUS_XP;
      }
      body_remove(get_comp_body(ai));
      list_remove(current->computers, i);
      computer_free(ai);
      i--;
    }
  }
  list_t *shields = scene_bodies_with_comp_info(current->game_scene, SHIELD);
  for (size_t i = 0; i < list_size(shields); i++) {
    body_t *shield = list_get(shields, i);
    double shield_damage = body_damage_collisions(shield);
    if (shield_damage > SHIELD_HEALTH) {
      body_remove(shield);
    }
  }
  list_free(shields);
}

void process_gameplay(state_t *current, double tick_time) {
  if (character_is_on_cooldown(current->user)) {
    character_cooldown_weapon(current->user, tick_time);
  }
  if (character_is_reloading_curr(current->user)) {
    character_decrement_reload(current->user, tick_time);
    character_reload(current->user);
  }
  if (isEmpty(current->computers)) {
    current->wave_count++;
    size_t total_enemies =
        (current->wave_count) + (5 % current->wave_dmg_multiplier);
    if (current->wave_count % BOSS_OCCURENCE == 0) {
      sdl_change_background_music();
      add_enemies_to_scene(current, true, 1);
    } else {
      if (current->wave_count % BOSS_OCCURENCE == 1 &&
          current->wave_count != 1) {
        current->wave_dmg_multiplier++;
        character_increase_upgrade_factor(current->user);
        sdl_change_background_music();
      }
      add_enemies_to_scene(current, false, total_enemies);
    }
  }
  for (size_t i = 0; i < list_size(current->computers); i++) {
    computer_t *ai = list_get(current->computers, i);
    computer_keep_within_boundaries(ai);
    computer_rotate(ai, current->user);
    style_info_t style = computer_get_style(ai);
    double max_distance = max_enemy_to_distance(style);
    double distance = computer_distance_to(ai, current->user);
    if (distance <= max_distance) {
      computer_set_velocity(ai, VEC_ZERO);
      if (computer_weapon_ammo(ai) && !computer_is_on_cooldown(ai) &&
          !computer_is_reloading_curr(ai)) {
        sdl_play_sound(
            get_weapon_sound(weapon_get_type(computer_get_weapon(ai))));
      }
      computer_shoot(current->game_scene, ai);
    } else {
      computer_set_velocity(
          ai,
          vec_get_unit_vector(computer_direction(ai, current->user), VEC_ZERO));
    }
    if (computer_is_on_cooldown(ai)) {
      computer_cooldown_weapon(ai, tick_time);
    }
    if (!computer_weapon_ammo(ai)) {
      computer_reload(ai);
      sdl_play_sound(
          get_reload_sound(weapon_get_type(computer_get_weapon(ai))));
    }
    if (computer_is_reloading_curr(ai)) {
      computer_decrement_reload(ai, tick_time);
      computer_reload(ai);
    }
  }
}

void orient_screen(state_t *current) {
  keep_within_boundaries(current->user);
  body_t *user_body = character_get_body(current->user);
  vector_t user_center = body_get_centroid(user_body);
  sdl_on_key((key_handler_t)game_key);
  sdl_set_center(user_center);
  sdl_set_max((vector_t){.x = user_center.x + SCREEN_WIDTH / 2,
                         .y = user_center.y + SCREEN_HEIGHT / 2});
  sdl_set_min((vector_t){.x = user_center.x - SCREEN_WIDTH / 2,
                         .y = user_center.y - SCREEN_HEIGHT / 2});
}

void print_controls(list_t *print_objects_list) {
  char *font_file = "assets/Lato-Black.ttf";
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "CONTROLS:", FONT_SIZE, font_file,
                                 (SDL_Rect){800, 20, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text, "P: PAUSE",
                                 FONT_SIZE, font_file,
                                 (SDL_Rect){750, 40, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "WASD: MOVEMENT", FONT_SIZE, font_file,
                                 (SDL_Rect){750, 60, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "MOUSE: AIM AND SHOOT", FONT_SIZE, font_file,
                                 (SDL_Rect){750, 80, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text, "R: RELOAD",
                                 FONT_SIZE, font_file,
                                 (SDL_Rect){860, 40, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Q: SWITCHES WEAPON", FONT_SIZE, font_file,
                                 (SDL_Rect){860, 60, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "T: ACTIVATES POWERUP", FONT_SIZE, font_file,
                                 (SDL_Rect){860, 80, 100, 20},
                                 rgb_to_SDL(switch_color(8)), NULL));
}

void print_game_scene(state_t *current, list_t *game_print_objects_list) {
  char *font_file = "assets/Lato-Black.ttf";
  char *health = malloc(STRING_BUFFER * sizeof(char));
  sprintf(health, "HEALTH: %0.2f", character_get_health(current->user));
  char *xp = malloc(STRING_BUFFER * sizeof(char));
  sprintf(xp, "EXP: %0.2f", current->current_xp);
  char *ammo = malloc(STRING_BUFFER * sizeof(char));
  sprintf(ammo, "AMMO: %0.2f", weapon_ammo(character_weapon(current->user)));
  char *racks = malloc(STRING_BUFFER * sizeof(char));
  sprintf(racks, "RACKS: %0.2f", current->racks);
  char *wave = malloc(STRING_BUFFER * sizeof(char));
  sprintf(wave, "WAVE %02zu", current->wave_count);
  char *enemies = malloc(STRING_BUFFER * sizeof(char));
  sprintf(enemies, "Enemies Remaining: %02zu", list_size(current->computers));
  list_add(game_print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text, "BRAWLHUB",
                                 FONT_SIZE, font_file,
                                 (SDL_Rect){0, 0, 200, 100}, TEXT_COLOR, NULL));
  list_add(game_print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, health, FONT_SIZE, font_file,
               (SDL_Rect){895, 410, 100, 30}, TEXT_COLOR, free));
  list_add(game_print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text, xp, FONT_SIZE,
                                 font_file, (SDL_Rect){895, 440, 100, 30},
                                 TEXT_COLOR, free));
  list_add(game_print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, ammo, FONT_SIZE, font_file,
               (SDL_Rect){895, 470, 100, 30}, TEXT_COLOR, free));
  list_add(game_print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, racks, FONT_SIZE, font_file,
               (SDL_Rect){0, 100, 100, 30}, TEXT_COLOR, free));
  list_add(game_print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, enemies, FONT_SIZE, font_file,
               (SDL_Rect){0, 370, 150, 50}, TEXT_COLOR, free));
  list_add(game_print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, wave, FONT_SIZE, font_file,
               (SDL_Rect){0, 420, 150, 70}, TEXT_COLOR, free));
}

void print_pause_scene(state_t *current, list_t *print_objects_list) {
  char *font_file = "assets/Lato-Black.ttf";
  char *speed_mult = malloc(STRING_BUFFER * sizeof(char));
  sprintf(speed_mult, "Current Speed Multiplier: %0.2f",
          character_get_speed_multiplier(current->user));
  char *damage_mult = malloc(STRING_BUFFER * sizeof(char));
  sprintf(damage_mult, "Current Damage Multiplier: %0.2f",
          character_get_dmg_multiplier(current->user));
  char *racks = malloc(STRING_BUFFER * sizeof(char));
  sprintf(racks, "Racks: %0.2f", current->racks);
  char *regen_mult = malloc(STRING_BUFFER * sizeof(char));
  sprintf(regen_mult, "Current Regeneration Rate: %0.2f",
          character_get_healing_factor(current->user));
  char *max_health = malloc(STRING_BUFFER * sizeof(char));
  sprintf(max_health, "Current Max Health is: %0.2f",
          character_get_max_health(current->user));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, "PAUSE", FONT_SIZE, font_file,
               (SDL_Rect){300, 5, 400, 100}, TEXT_COLOR, NULL));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text,
               "Press SPACEBAR to return to current game", FONT_SIZE, font_file,
               (SDL_Rect){300, 105, 400, 25}, TEXT_COLOR, NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Press ESCAPE to start a new game", FONT_SIZE,
                                 font_file, (SDL_Rect){300, 125, 400, 25},
                                 TEXT_COLOR, NULL));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, racks, FONT_SIZE, font_file,
               (SDL_Rect){300, 150, 300, 50}, TEXT_COLOR, free));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, speed_mult, FONT_SIZE,
               font_file, (SDL_Rect){50, 225, 300, 25}, TEXT_COLOR, free));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text,
               "Press G to increase your speed multiplier for 25 racks",
               FONT_SIZE, font_file, (SDL_Rect){50, 250, 400, 25}, TEXT_COLOR,
               NULL));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, damage_mult, FONT_SIZE,
               font_file, (SDL_Rect){500, 225, 300, 25}, TEXT_COLOR, free));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text,
               "Press H to increase your damage multiplier for 25 racks",
               FONT_SIZE, font_file, (SDL_Rect){500, 250, 400, 25}, TEXT_COLOR,
               NULL));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, regen_mult, FONT_SIZE,
               font_file, (SDL_Rect){50, 300, 300, 25}, TEXT_COLOR, free));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text,
               "Press U to increase your regeneration rate for 75 racks",
               FONT_SIZE, font_file, (SDL_Rect){50, 330, 400, 25}, TEXT_COLOR,
               NULL));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, max_health, FONT_SIZE,
               font_file, (SDL_Rect){500, 300, 400, 25}, TEXT_COLOR, free));
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text,
               "Press Y to increase your max health for 50 racks", FONT_SIZE,
               font_file, (SDL_Rect){500, 330, 400, 25}, TEXT_COLOR, NULL));
}

void print_start_scene(list_t *print_objects_list) {
  char *font_file = "assets/Lato-Black.ttf";
  list_add(print_objects_list,
           SDL_print_object_init(
               (print_handler_t)SDL_print_text, "BRAWLHUB", FONT_SIZE,
               font_file, (SDL_Rect){300, 5, 400, 100}, TEXT_COLOR, NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Press 1 to play as a Brute", FONT_SIZE,
                                 font_file, (SDL_Rect){150, 105, 200, 50},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Base Health: 1000", FONT_SIZE, font_file,
                                 (SDL_Rect){150, 155, 200, 50},
                                 rgb_to_SDL(switch_color(1)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Primary Weapon: Shotgun", FONT_SIZE,
                                 font_file, (SDL_Rect){150, 205, 200, 50},
                                 rgb_to_SDL(switch_color(1)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Secondary Weapon: Pistol", FONT_SIZE,
                                 font_file, (SDL_Rect){150, 255, 200, 50},
                                 rgb_to_SDL(switch_color(1)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Damage Multiplier: 1.5", FONT_SIZE, font_file,
                                 (SDL_Rect){150, 305, 200, 50},
                                 rgb_to_SDL(switch_color(1)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Speed Multiplier: 0.5", FONT_SIZE, font_file,
                                 (SDL_Rect){150, 355, 200, 50},
                                 rgb_to_SDL(switch_color(1)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Press 2 to play as a Gunman", FONT_SIZE,
                                 font_file, (SDL_Rect){400, 105, 200, 50},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Base Health: 400", FONT_SIZE, font_file,
                                 (SDL_Rect){400, 155, 200, 50},
                                 rgb_to_SDL(switch_color(7)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Primary Weapon: Assault Rifle", FONT_SIZE,
                                 font_file, (SDL_Rect){400, 205, 200, 50},
                                 rgb_to_SDL(switch_color(7)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Secondary Weapon: Shotgun", FONT_SIZE,
                                 font_file, (SDL_Rect){400, 255, 200, 50},
                                 rgb_to_SDL(switch_color(7)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Damage Multiplier: 1.2", FONT_SIZE, font_file,
                                 (SDL_Rect){400, 305, 200, 50},
                                 rgb_to_SDL(switch_color(7)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Speed Multiplier: 1.0", FONT_SIZE, font_file,
                                 (SDL_Rect){400, 355, 200, 50},
                                 rgb_to_SDL(switch_color(7)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Press 3 to play as a Sniper", FONT_SIZE,
                                 font_file, (SDL_Rect){650, 105, 200, 50},
                                 rgb_to_SDL(switch_color(8)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Base Health: 500", FONT_SIZE, font_file,
                                 (SDL_Rect){650, 155, 200, 50},
                                 rgb_to_SDL(switch_color(5)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Primary Weapon: Sniper Rifle", FONT_SIZE,
                                 font_file, (SDL_Rect){650, 205, 200, 50},
                                 rgb_to_SDL(switch_color(5)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Secondary Weapon: Pistol", FONT_SIZE,
                                 font_file, (SDL_Rect){650, 255, 200, 50},
                                 rgb_to_SDL(switch_color(5)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Damage Multiplier: 2.2", FONT_SIZE, font_file,
                                 (SDL_Rect){650, 305, 200, 50},
                                 rgb_to_SDL(switch_color(5)), NULL));
  list_add(print_objects_list,
           SDL_print_object_init((print_handler_t)SDL_print_text,
                                 "Speed Multiplier: 0.8", FONT_SIZE, font_file,
                                 (SDL_Rect){650, 355, 200, 50},
                                 rgb_to_SDL(switch_color(5)), NULL));
}

state_t *emscripten_init() {
  sdl_init(VEC_ZERO, (vector_t){SCREEN_WIDTH, SCREEN_HEIGHT});
  state_t *output = malloc(sizeof(state_t));
  start_scene_init(output);
  pause_scene_init(output);
  output->computers = list_init(5, (free_func_t)computer_free);
  vector_t start_center = body_get_centroid(output->start_background);
  sdl_set_center(start_center);
  sdl_set_max((vector_t){.x = start_center.x + SCREEN_WIDTH / 2,
                         .y = start_center.y + SCREEN_HEIGHT / 2});
  sdl_set_min((vector_t){.x = start_center.x - SCREEN_WIDTH / 2,
                         .y = start_center.y - SCREEN_HEIGHT / 2});
  output->current_scene = START_SCENE;
  return output;
}

void emscripten_main(state_t *current) {
  double time = time_since_last_tick(); // Should tick even while paused so
                                        // enemies don't "teleport"
  switch (current->current_scene) {
  case GAME_SCENE: {
    if (!exit_out_of_game(current)) { // check for character death
      current->character_heal_timer += time;
      if (current->character_heal_timer >= 5 &&
          character_is_alive(current->user)) {
        character_heal(current->user);
        current->character_heal_timer = 0;
      }
      orient_screen(current);
      process_gameplay(current, time);
      process_damages(current);
      process_bullet_life(current);
      scene_tick(current->game_scene, time);
      // PRINTING STATS AND TITLE
      list_t *game_print_objects_list =
          list_init(6, (free_func_t)SDL_print_object_free);
      print_game_scene(current, game_print_objects_list);
      // RENDER SCENE
      sdl_render_scene(current->game_scene, game_print_objects_list,
                       current->user, current->computers, false);
      list_free(game_print_objects_list);
    }
    break;
  }

  case PAUSE_SCENE: { // check order of lines here
    sdl_on_key((key_handler_t)pause_key);
    list_t *print_objects_list =
        list_init(10, (free_func_t)SDL_print_object_free);
    print_pause_scene(current, print_objects_list);
    print_controls(print_objects_list);
    sdl_render_scene(current->pause_scene, print_objects_list, NULL, NULL,
                     false);
    list_free(print_objects_list);
    break;
  }

  case START_SCENE: {
    sdl_on_key((key_handler_t)start_key);
    list_t *print_objects_list =
        list_init(3, (free_func_t)SDL_print_object_free);
    print_start_scene(print_objects_list);
    print_controls(print_objects_list);
    sdl_render_scene(current->start_scene, print_objects_list, NULL, NULL,
                     true);
    list_free(print_objects_list);
    break;
  }
  }
}

void emscripten_free(state_t *current) {
  scene_free(current->game_scene);
  scene_free(current->pause_scene);
  scene_free(current->start_scene);
  list_free(current->computers);
  list_free(current->obstacles);
  character_free(current->user);
  free(current);
}
