/**
 * @file display.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "util/helper.h"

#include <SDL2/SDL.h>


// Game display window
typedef struct {
    IntSize resolution;
    SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;
	
    int running;
} GameWindow;

GameWindow* get_game_window(void);
int init_game_window(const char*);
void destroy_game_window(void);

int is_hovering_over_pos(IntSize, IntPos);

#endif // DISPLAY_H
