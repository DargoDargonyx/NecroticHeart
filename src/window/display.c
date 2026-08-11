/**
 * @file display.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "window/display.h"
#include "util/config.h"


static GameWindow* game_window = NULL;

// Game display window

int init_game_window(const char* window_name) {
    if (game_window) return 0;
    game_window = malloc(sizeof(GameWindow));

	ConfigCont* config = get_config();

	if (config->display->fullscreen) {
		game_window->sdl_window = SDL_CreateWindow(
			window_name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_RENDER_LOGICAL_SIZE_X,
			WINDOW_RENDER_LOGICAL_SIZE_Y,
			SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	} else {
		game_window->resolution = config->display->resolution;
		game_window->sdl_window = SDL_CreateWindow(
			window_name, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			game_window->resolution.w, 
			game_window->resolution.h, 
			SDL_WINDOW_SHOWN
		);
	}

	if (!game_window->sdl_window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

	game_window->sdl_renderer = SDL_CreateRenderer(
		game_window->sdl_window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	SDL_RenderSetLogicalSize(
		game_window->sdl_renderer, 
		WINDOW_RENDER_LOGICAL_SIZE_X,
		WINDOW_RENDER_LOGICAL_SIZE_Y
	);

	game_window->current_scene = START_MENU_SCENE;
	game_window->running = 1;
	return 0;
}

void destroy_game_window(void) {
	if (!game_window) {
		printf(PRINT_WARNING "Could not destroy a null game window struct\n");
		return;
	}

	if (game_window->sdl_window) SDL_DestroyWindow(game_window->sdl_window);
    if (game_window->sdl_renderer) SDL_DestroyRenderer(game_window->sdl_renderer);
		
	free(game_window);
	game_window = NULL;
}

GameWindow* get_game_window(void) { return game_window; }
