/**
 * @file game.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "engine/game.h"
#include "util/file.h"
#include "util/font.h"
#include "util/config.h"
#include "window/display.h"
#include "window/render.h"
#include "engine/scene.h"

#include <SDL2/SDL_ttf.h>


int init_game(void) {
	// Config files
	if (seed_configs()) {
		printf(PRINT_ERROR "Could not seed configuration files\n");
		return 1;
	}
    if (init_config_manager()) {
		printf(PRINT_ERROR "Could not initialize config manager\n");
		return 1;
	}

	// Game window
	SDL_Init(SDL_INIT_VIDEO);
	if (init_game_window(GAME_NAME)) {
		printf(PRINT_ERROR "Could not initialize game window\n");
        quit_game(EXIT_FAILURE);
		return 1;
    }

	// Fonts
	TTF_Init();
	if (init_global_fonts()) {
		printf(PRINT_ERROR "Could not initialize global fonts\n");
		quit_game(EXIT_FAILURE);
		return 1;
	}

	// Scenes
	if (init_scene_manager()) {
		printf(PRINT_ERROR "Could not initialize game scenes\n");
		quit_game(EXIT_FAILURE);
		return 1;
	}

	return 0;
}

void quit_game(int error_code) {
	if (get_scene_manager()) destroy_scene_manager();
	if (get_global_fonts()) destroy_global_fonts();
	if (get_game_window()) destroy_game_window();
	if (get_config_manager()) destroy_config_manager();

	TTF_Quit();
    SDL_Quit();
	exit(error_code);
}

int run_game_loop(void) {
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR 
				"Could not access the game window when starting the game loop\n");
		quit_game(EXIT_FAILURE);
	}

	SceneManager* scene_manager = get_scene_manager();
	if (!scene_manager) {
		printf(PRINT_ERROR 
				"Could not access the scene manager when starting the game loop\n");
		quit_game(EXIT_FAILURE);
	}

	int err = 0;
    SDL_Event event;
	Uint64 last = 0;
	while (game_window->running) {
		Uint64 now = SDL_GetPerformanceCounter();
		float dt = (float) (now - last) / (float) SDL_GetPerformanceFrequency();
		last = now;
	
		// Events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) game_window->running = 0;

			handle_widget_events(scene_manager->current_scene->widget_cont, &event);
		}

		// Update
		if (update_widgets(scene_manager->current_scene->widget_cont, dt)) {
			game_window->running = 0;
			printf(PRINT_ERROR "Unable to update the current game scene\n");
			quit_game(EXIT_FAILURE);
		}
		if (scene_manager->scene_switch_requested) apply_scene_switch_request();

		// Render
		SDL_SetRenderDrawColor(game_window->sdl_renderer, 30, 30, 30, 255);
		SDL_RenderClear(game_window->sdl_renderer);
		
		if (render_widgets(scene_manager->current_scene->widget_cont)) {
			game_window->running = 0;
			printf(PRINT_ERROR "Unable to render the current game scene\n");
			quit_game(EXIT_FAILURE);
		}
		SDL_RenderPresent(game_window->sdl_renderer);
	}

	return err;
}
