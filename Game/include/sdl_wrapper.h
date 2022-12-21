#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include "character.h"
#include "color.h"
#include "computer.h"
#include "list.h"
#include "scene.h"
#include "state.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Font size used for printing text on screen
 *
 */
extern const double FONT_SIZE;

// Values passed to a key handler when the given arrow key is pressed
typedef enum {
  LEFT_ARROW = 1,
  UP_ARROW = 2,
  RIGHT_ARROW = 3,
  DOWN_ARROW = 4
} arrow_key_t;

/**
 * The possible types of key events.
 * Enum types in C are much more primitive than in Java; this is equivalent to:
 * typedef unsigned int KeyEventType;
 * #define KEY_PRESSED 0
 * #define KEY_RELEASED 1
 */
typedef enum {
  KEY_PRESSED,
  KEY_RELEASED,
  MOUSE_MOTION,
  MOUSE_PRESSED,
  MOUSE_RELEASED
} key_event_type_t;

/**
 * A keypress handler.
 * When a key is pressed or released, the handler is passed its char value.
 * Most keys are passed as their char value, e.g. 'a', '1', or '\r'.
 * Arrow keys have the special values listed above.
 *
 * @param key a character indicating which key was pressed
 * @param type the type of key event (KEY_PRESSED or KEY_RELEASED)
 * @param held_time if a press event, the time the key has been held in seconds
 */
typedef void (*key_handler_t)(char key, key_event_type_t type, double held_time,
                              void *current);

// Add docstrings
typedef SDL_Texture *(*print_handler_t)(void *message, double font_size,
                                        char *font_file, SDL_Rect rectangle_dms,
                                        SDL_Color color);

/**
 * @brief print object struct
 *
 */
typedef struct SDL_print_object SDL_print_object_t;

/**
 * @brief Initializes a print objcet
 *
 * @param print_handler a function that handles text printing
 * @param message message to be printed
 * @param font_size message font size
 * @param font_file type of font
 * @param rectangle_dms location of text
 * @param color color of the text
 * @param message_freer frees the message
 * @return SDL_print_object_t that was initialized
 */
SDL_print_object_t *
SDL_print_object_init(print_handler_t print_handler, void *message,
                      double font_size, char *font_file, SDL_Rect rectangle_dms,
                      SDL_Color color, free_func_t message_freer);

/**
 * @brief Frees SDL print objects
 *
 * @param print_object Object to free.
 */
void SDL_print_object_free(SDL_print_object_t *print_object);

/**
 * @brief renders text to screen with given dimmesions
 *
 * @param string The string to render.
 * @param font_size the size of the text to be rendered.
 * @param font_file the font type of the text to be rendered.
 * @param rectangle_dms location of text to be rendered.
 * @param color color of the text to be rendered.
 * @return SDL_Texture of the rendered text so that it can be destroyed later
 * on.
 */
SDL_Texture *SDL_print_text(char *string, double font_size, char *font_file,
                            SDL_Rect rectangle_dms, SDL_Color color);

/**
 * Initializes the SDL window and renderer.
 * Must be called once before any of the other SDL functions.
 *
 * @param min the x and y coordinates of the bottom left of the scene
 * @param max the x and y coordinates of the top right of the scene
 */
void sdl_init(vector_t min, vector_t max);

/**
 * @brief changes the background music
 *
 */
void sdl_change_background_music(void);

/**
 * @brief plays the sound file passed in
 *
 * @param file_name music file to play
 */
void sdl_play_sound(char *file_name);

/**
 * Sets the center to the SDL window
 *
 * @param new_center new_center that everything gets built around
 * */
void sdl_set_center(vector_t new_center);

/**
 * Sets the top right of the SDL window
 *
 * @param new_center new_center that everything gets built around
 * */
void sdl_set_max(vector_t v);

/**
 * Sets the bottom left of the SDL window
 *
 * @param new_center new_center that everything gets built around
 * */
void sdl_set_min(vector_t v);

/**
 * Processes all SDL events and returns whether the window has been closed.
 * This function must be called in order to handle keypresses.
 *
 * @return true if the window was closed, false otherwise
 */
bool sdl_is_done(state_t *current);

/**
 * Clears the screen. Should be called before drawing polygons in each frame.
 */
void sdl_clear(void);

/**
 * Draws a polygon from the given list of vertices and a color.
 *
 * @param points the list of vertices of the polygon
 * @param color the color used to fill in the polygon
 */
void sdl_draw_polygon(list_t *points, rgb_color_t color);

/**
 * Displays the rendered frame on the SDL window.
 * Must be called after drawing the polygons in order to show them.
 */
void sdl_show(void);

/**
 * Draws all bodies in a scene.
 * This internally calls sdl_clear(), sdl_draw_polygon(), and sdl_show(),
 * so those functions should not be called directly.
 *
 * @param scene the scene to draw
 */
void sdl_render_scene(scene_t *scene, list_t *print_objects,
                      character_t *character, list_t *comps, bool start);

/**
 * Registers a function to be called every time a key is pressed.
 * Overwrites any existing handler.
 *
 * Example:
 * ```
 * void on_key(char key, key_event_type_t type, double held_time) {
 *     if (type == KEY_PRESSED) {
 *         switch (key) {
 *             case 'a':
 *                 printf("A pressed\n");
 *                 break;
 *             case UP_ARROW:
 *                 printf("UP pressed\n");
 *                 break;
 *         }
 *     }
 * }
 * int main(void) {
 *     sdl_on_key(on_key);
 *     while (!sdl_is_done());
 * }
 * ```
 *
 * @param handler the function to call with each key press
 */
void sdl_on_key(key_handler_t handler);

/**
 * Gets the amount of time that has passed since the last time
 * this function was called, in seconds.
 *
 * @return the number of seconds that have elapsed
 */
double time_since_last_tick(void);

/**
 * @brief Renders different sprites based on info types.
 *
 * @param body The body that the sprite is associated with.
 * @param style The computer/character type to render
 * @param weapon_type the weapon type being used by said computer/character.
 * @param character The character sprite to render
 * @param comp The computer sprite to render
 * @return list of SDL_Textures of the sprites rendered.
 */
list_t *show_image(body_t *body, style_info_t style, weapon_info_t weapon_type,
                   character_t *character, computer_t *comp, bool start);

#endif // #ifndef __SDL_WRAPPER_H__
