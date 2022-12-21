#include "sdl_wrapper.h"
#include "character.h"
#include "computer.h"
#include "info_types.h"
#include "key_handler.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const char WINDOW_TITLE[] = "CS 3";
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
const double MS_PER_S = 1e3;
const double FONT_SIZE = 70;
const double COMP_SIZE = 30;
const double USER_SIZE = 30;
const int SPRITE_SIZE_FACTOR = 3;

/**
 * The coordinate at the center of the screen.
 */
vector_t center;
/**
 * The top right coordinate of the map.
 */
vector_t max;
/**
 * The bottom left coordinate of the map.
 */
vector_t min;
/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;
/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
uint32_t key_start_timestamp;
/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

Mix_Chunk *sound_effect = NULL;

void sdl_set_max(vector_t new_max) { max = new_max; }

void sdl_set_min(vector_t new_min) { min = new_min; }

/** Computes the center of the window in pixel coordinates */
vector_t get_window_center(void) {
  int *width = malloc(sizeof(*width)), *height = malloc(sizeof(*height));
  assert(width != NULL);
  assert(height != NULL);
  SDL_GetWindowSize(window, width, height);
  vector_t dimensions = {.x = *width, .y = *height};
  free(width);
  free(height);
  return vec_multiply(0.5, dimensions);
}

/**
 * Computes the scaling factor between scene coordinates and pixel coordinates.
 * The scene is scaled by the same factor in the x and y dimensions,
 * chosen to maximize the size of the scene while keeping it in the window.
 */
double get_scene_scale(vector_t window_center) {
  // Scale scene so it fits entirely in the window
  double x_scale = window_center.x / max_diff.x,
         y_scale = window_center.y / max_diff.y;
  return x_scale < y_scale ? x_scale : y_scale;
}

/** Maps a scene coordinate to a window coordinate */
vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
  // Scale scene coordinates by the scaling factor
  // and map the center of the scene to the center of the window
  vector_t scene_center_offset = vec_subtract(scene_pos, center);
  double scale = get_scene_scale(window_center);
  vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
  vector_t pixel = {.x = round(window_center.x + pixel_center_offset.x),
                    // Flip y axis since positive y is down on the screen
                    .y = round(window_center.y - pixel_center_offset.y)};
  return pixel;
}

/**
 * Converts an SDL key code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_keycode(SDL_Keycode key) {
  switch (key) {
  case SDLK_LEFT:
    return LEFT_ARROW;
  case SDLK_UP:
    return UP_ARROW;
  case SDLK_RIGHT:
    return RIGHT_ARROW;
  case SDLK_DOWN:
    return DOWN_ARROW;
  default:
    // Only process 7-bit ASCII characters
    return key == (SDL_Keycode)(char)key ? key : '\0';
  }
}

void sdl_set_center(vector_t new_center) { center = new_center; }

void sdl_init(vector_t min, vector_t max) {
  // Check parameters
  assert(min.x < max.x);
  assert(min.y < max.y);

  center = vec_multiply(0.5, vec_add(min, max));
  max_diff = vec_subtract(max, center);
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  TTF_Init();
  int check_audio = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  assert(check_audio != -1);
  Mix_AllocateChannels(250);
  Mix_Chunk *background_sound1 = Mix_LoadWAV("assets/HIM.wav");
  assert(background_sound1 != NULL);
  Mix_Chunk *background_sound2 =
      Mix_LoadWAV("assets/Undertale-Megalovania.wav");
  assert(background_sound2 != NULL);
  Mix_PlayChannel(0, background_sound1, -1);
  Mix_PlayChannel(1, background_sound2, -1);
  Mix_Pause(1);
  Mix_Volume(-1, 96);
  Mix_Volume(0, 128);
  Mix_Volume(1, 128);
}

void sdl_change_background_music(void) {
  if (Mix_Paused(0)) {
    Mix_Pause(1);
    Mix_Resume(0);
  } else if (Mix_Paused(1)) {
    Mix_Pause(0);
    Mix_Resume(1);
  }
}

void sdl_play_sound(char *file_name) {
  Mix_Chunk *current_sound = NULL;
  if (sound_effect != NULL) {
    current_sound = sound_effect;
  }
  sound_effect = Mix_LoadWAV(file_name);
  Mix_PlayChannel(-1, sound_effect, 0);
  if (current_sound != NULL) {
    Mix_FreeChunk(current_sound);
    current_sound = NULL;
  }
}

bool sdl_is_done(state_t *current) {
  SDL_Event *event = malloc(sizeof(*event));
  assert(event != NULL);
  while (SDL_PollEvent(event)) {
    switch (event->type) {

    // mouse press and shoot//
    case SDL_MOUSEMOTION: {
      if (key_handler == NULL) {
        break;
      }
      key_event_type_t type = MOUSE_MOTION;
      key_handler('.', type, 0, current);
      break;
    }
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN: {
      if (key_handler == NULL) {
        break;
      }
      key_event_type_t type =
          event->type == SDL_MOUSEBUTTONDOWN ? MOUSE_PRESSED : MOUSE_RELEASED;
      key_handler('.', type, 0, current);
      break;
    }
      // MOUSE EVENTS END//

    case SDL_QUIT:
      free(event);
      return true;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      // Skip the keypress if no handler is configured
      // or an unrecognized key was pressed
      if (key_handler == NULL)
        break;
      char key = get_keycode(event->key.keysym.sym);
      if (key == '\0')
        break;

      uint32_t timestamp = event->key.timestamp;
      if (!event->key.repeat) {
        key_start_timestamp = timestamp;
      }
      key_event_type_t type =
          event->type == SDL_KEYDOWN ? KEY_PRESSED : KEY_RELEASED;
      double held_time = (timestamp - key_start_timestamp) / MS_PER_S;
      key_handler(key, type, held_time, current);
      break;
    }
  }
  free(event);
  return false;
}

void sdl_clear(void) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
}

void sdl_draw_polygon(list_t *points, rgb_color_t color) {
  // Check parameters
  size_t n = list_size(points);
  assert(n >= 3);
  assert(0 <= color.r && color.r <= 1);
  assert(0 <= color.g && color.g <= 1);
  assert(0 <= color.b && color.b <= 1);
  assert(0 <= color.a && color.a <= 1);

  vector_t window_center = get_window_center();

  // Convert each vertex to a point on screen
  int16_t *x_points = malloc(sizeof(*x_points) * n),
          *y_points = malloc(sizeof(*y_points) * n);
  assert(x_points != NULL);
  assert(y_points != NULL);
  for (size_t i = 0; i < n; i++) {
    vector_t *vertex = list_get(points, i);
    vector_t pixel = get_window_position(*vertex, window_center);
    x_points[i] = pixel.x;
    y_points[i] = pixel.y;
  }

  // Draw polygon with the given color
  filledPolygonRGBA(renderer, x_points, y_points, n, color.r * 255,
                    color.g * 255, color.b * 255, color.a * 255);
  free(x_points);
  free(y_points);
}

void sdl_show(void) {
  // Draw boundary lines
  vector_t window_center = get_window_center();
  vector_t max_pixel = get_window_position(max, window_center),
           min_pixel = get_window_position(min, window_center);
  SDL_Rect *boundary = malloc(sizeof(*boundary));
  boundary->x = min_pixel.x;
  boundary->y = max_pixel.y;
  boundary->w = max_pixel.x - min_pixel.x;
  boundary->h = min_pixel.y - max_pixel.y;
  SDL_SetRenderDrawColor(renderer, 0, 1, 1, 255);
  SDL_RenderDrawRect(renderer, boundary);
  free(boundary);

  SDL_RenderPresent(renderer);
}

void sdl_on_key(key_handler_t handler) { key_handler = handler; }

double time_since_last_tick(void) {
  clock_t now = clock();
  double difference = last_clock
                          ? (double)(now - last_clock) / CLOCKS_PER_SEC
                          : 0.0; // return 0 the first time this is called
  last_clock = now;
  return difference;
}

// print handling
typedef struct SDL_print_object {
  print_handler_t print_handler;
  void *message;
  double font_size;
  char *font_file;
  SDL_Rect rectangle_dms;
  SDL_Color color;
  free_func_t message_freer;
} SDL_print_object_t;

SDL_print_object_t *
SDL_print_object_init(print_handler_t print_handler, void *message,
                      double font_size, char *font_file, SDL_Rect rectangle_dms,
                      SDL_Color color, free_func_t message_freer) {
  SDL_print_object_t *output = malloc(sizeof(SDL_print_object_t));
  output->print_handler = print_handler;
  output->message = message;
  output->font_size = font_size;
  output->font_file = font_file;
  output->rectangle_dms = rectangle_dms;
  output->color = color;
  output->message_freer = message_freer;
  return output;
}

void SDL_print_object_free(SDL_print_object_t *print_object) {
  if (print_object->message_freer != NULL) {
    print_object->message_freer(print_object->message);
  }
  free(print_object);
}

void sdl_render_scene(scene_t *scene, list_t *print_objects,
                      character_t *character, list_t *comps, bool start) {
  sdl_clear();
  // Texture list is to make sure all the textures are destroyed.
  list_t *texture_list =
      list_init(list_size(print_objects), (free_func_t)SDL_DestroyTexture);

  size_t body_count = scene_bodies(scene);
  for (size_t i = 0; i < body_count; i++) {
    body_t *body = scene_get_body(scene, i);
    list_t *shape = body_get_shape(body);
    sdl_draw_polygon(shape, body_get_color(body));
    list_free(shape);
  }
  // Renders Text
  if (print_objects != NULL) {
    for (size_t i = 0; i < list_size(print_objects); i++) {
      SDL_print_object_t *print_object = list_get(print_objects, i);
      list_add(texture_list,
               print_object->print_handler(
                   print_object->message, print_object->font_size,
                   print_object->font_file, print_object->rectangle_dms,
                   print_object->color));
    }
  }
  // Renders Sprites
  list_t *img_textures =
      list_init(list_size(print_objects), (free_func_t)list_free);
  if (start) {
    list_add(img_textures, show_image(NULL, 1, 1, NULL, NULL, start));
  }
  if (character != NULL) {
    weapon_info_t weapon_type = weapon_get_type(character_weapon(character));
    style_info_t style = character_style(character);
    list_add(img_textures, show_image(character_get_body(character), style,
                                      weapon_type, character, NULL, start));
  }
  if (comps != NULL) {
    for (size_t i = 0; i < list_size(comps); i++) {
      computer_t *comp = list_get(comps, i);
      weapon_info_t weapon_type = weapon_get_type(computer_get_weapon(comp));
      style_info_t style = computer_get_style(comp);
      list_add(img_textures, show_image(get_comp_body(comp), style, weapon_type,
                                        character, comp, start));
    }
  }
  sdl_show();
  list_free(texture_list);
  list_free(img_textures);
}

// Print handling continued
SDL_Texture *SDL_print_text(char *string, double font_size, char *font_file,
                            SDL_Rect rectangle_dms, SDL_Color color) {
  TTF_Font *font_type = TTF_OpenFont(font_file, font_size);
  assert(font_type != NULL);
  SDL_Surface *mssg_surface = TTF_RenderText_Solid(font_type, string, color);
  SDL_Texture *mssg = SDL_CreateTextureFromSurface(renderer, mssg_surface);
  SDL_RenderCopy(renderer, mssg, NULL, &rectangle_dms);
  SDL_FreeSurface(mssg_surface);
  TTF_CloseFont(font_type);
  return mssg;
}

// Image Handling
list_t *show_image(body_t *body, style_info_t style, weapon_info_t weapon_type,
                   character_t *character, computer_t *comp, bool start) {
  int w;
  int h;
  list_t *textures = list_init(5, (free_func_t)SDL_DestroyTexture);
  if (!start) {
    SDL_Texture *img = NULL;
    double angle = 0;
    switch (style) {
    case GUNMAN: {
      if (weapon_type == ASSAULT_RIFLE) {
        img = IMG_LoadTexture(renderer, "assets/gunman2.png");
      } else {
        img = IMG_LoadTexture(renderer, "assets/gunman1.png");
      }
      break;
    }
    case SNIPER: {
      if (weapon_type == PISTOL) {
        img = IMG_LoadTexture(renderer, "assets/sniper1.png");
      } else {
        img = IMG_LoadTexture(renderer, "assets/sniper2.png");
      }
      break;
    }
    case BRUTE: {
      if (weapon_type == PISTOL) {
        img = IMG_LoadTexture(renderer, "assets/brute1.png");
      } else {
        img = IMG_LoadTexture(renderer, "assets/brute2.png");
      }
      break;
    }
    default: {
      img = IMG_LoadTexture(renderer, "assets/henchmen.png");
      break;
    }
    }
    SDL_QueryTexture(img, NULL, NULL, &w, &h);
    SDL_Rect location;
    location.w = w / SPRITE_SIZE_FACTOR;
    location.h = h / SPRITE_SIZE_FACTOR;
    // Character Location
    if (*((computer_info_t *)(body_get_info(body))) == CHARACTER) {
      angle = (get_rotation_body(body) - (M_PI / 2)) * (-180.0 / M_PI);
      location.x = (WINDOW_WIDTH / 2) - USER_SIZE;
      location.y = (WINDOW_HEIGHT / 2) - USER_SIZE;
    } else { // Computer Location
      vector_t distance = computer_direction(comp, character);
      angle = (body_get_rotation(body) - (M_PI / 2)) * (-180.0 / M_PI);
      location.x = ((WINDOW_WIDTH / 2) - distance.x - COMP_SIZE);
      location.y = ((WINDOW_HEIGHT / 2) + distance.y - COMP_SIZE);
    }
    SDL_RenderCopyEx(renderer, img, NULL, &location, angle, NULL,
                     SDL_FLIP_NONE);
    list_add(textures, img);
    return textures;
  } else {
    SDL_Texture *img1 = IMG_LoadTexture(renderer, "assets/gunman1.png");
    SDL_Texture *img2 = IMG_LoadTexture(renderer, "assets/brute2.png");
    SDL_Texture *img3 = IMG_LoadTexture(renderer, "assets/sniper2.png");
    SDL_QueryTexture(img1, NULL, NULL, &w, &h);
    SDL_Rect location_1;
    location_1.w = w / SPRITE_SIZE_FACTOR;
    location_1.h = h / SPRITE_SIZE_FACTOR;
    location_1.x = 450;
    location_1.y = 400;
    SDL_Rect location_2;
    location_2.w = w / SPRITE_SIZE_FACTOR;
    location_2.h = h / SPRITE_SIZE_FACTOR;
    location_2.x = 200;
    location_2.y = 400;
    SDL_Rect location_3;
    location_3.w = w / SPRITE_SIZE_FACTOR;
    location_3.h = h / SPRITE_SIZE_FACTOR;
    location_3.x = 700;
    location_3.y = 400;
    SDL_RenderCopyEx(renderer, img1, NULL, &location_1, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, img2, NULL, &location_2, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, img3, NULL, &location_3, 0, NULL, SDL_FLIP_NONE);
    list_add(textures, img1);
    list_add(textures, img2);
    list_add(textures, img3);
    return textures;
  }
}
