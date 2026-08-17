/**
 * @file display.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "window/display.h"
#include "util/config.h"


static GameWindow* game_window = NULL;

// Game display window

int init_game_window(const char* window_name) {
    if (game_window) {
		printf(PRINT_WARNING "Attempting to initialize the game window when it has "
				"already been initialized\n");
		return 0;
	}
	game_window = malloc(sizeof(GameWindow));

	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not initialize the game window with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->display) {
		printf(PRINT_ERROR "Could not initialize the game window with a null "
				"display field in the config manager\n");
		return 1;
	}

	if (config_manager->display->fullscreen) {
		game_window->sdl_window = SDL_CreateWindow(
			window_name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			0,
			0,
			SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	} else {
		game_window->resolution = config_manager->display->resolution;
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
		printf(PRINT_ERROR "SDL_CreateWindow failed: %s\n", SDL_GetError());
		return 1;
	}

	game_window->sdl_renderer = SDL_CreateRenderer(
		game_window->sdl_window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	game_window->running = 1;
	return 0;
}

void destroy_game_window(void) {
	if (!game_window) {
		printf(PRINT_WARNING "Attempting to destroy a null game window\n");
		return;
	}

	if (game_window->sdl_window) SDL_DestroyWindow(game_window->sdl_window);
    if (game_window->sdl_renderer) SDL_DestroyRenderer(game_window->sdl_renderer);
		
	free(game_window);
	game_window = NULL;
}

GameWindow* get_game_window(void) { return game_window; }
