#ifndef __COLOR_H__
#define __COLOR_H__

#include <SDL2/SDL.h>

/**
 * A color to display on the screen.
 * The color is represented by its red, green, and blue components.
 * Each component must be between 0 (black) and 1 (white).
 */
typedef struct {
  float r;
  float g;
  float b;
  float a;
} rgb_color_t;

/**
 * @brief Returns a random color
 *
 * @return rgb_color_t representing a random color of r, g, b style
 * where each value is between 0 and 1
 */
rgb_color_t random_color();

/**
 * @brief Takes in a number represting a color in the rainbow or black or white
 *  1: RED
 *  2: ORANGE
 *  3: YELLOW
 *  4: GREEN
 *  5: BLUE
 *  6: INDIGO
 *  7: VIOLET
 *  8: WHITE
 *  9 and above: DEFAULT TO BLACK
 *
 * @param num representing the color you want
 * @return rgb_color_t
 */
rgb_color_t switch_color(int num);

/**
 * @brief converts an rgb_color_t to an SDL_Color data type
 *
 * @param color thats converted
 * @return SDL_Color thats returned
 */
SDL_Color rgb_to_SDL(rgb_color_t color);
#endif // #ifndef __COLOR_H__
