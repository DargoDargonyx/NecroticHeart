/**
 * @file display.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "util/helper.h"
#include "engine/scene.h"

#include <SDL2/SDL.h>

#define WINDOW_RENDER_LOGICAL_SIZE_X 1920
#define WINDOW_RENDER_LOGICAL_SIZE_Y 1080

typedef struct {
    IntSize resolution;
    SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;
	
	SceneType current_scene;
    int running;
} GameWindow;

GameWindow* get_game_window();
int init_game_window(const char*);
void destroy_game_window();

#endif // DISPLAY_H
