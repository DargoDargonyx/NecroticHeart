/**
 * @file game.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "engine/game.h"
#include "window/display.h"
#include "window/render.h"
#include "util/font.h"

#include <SDL2/SDL_ttf.h>

int init_game(void) {
	SDL_Init(SDL_INIT_VIDEO);
    
	if (init_game_window(GAME_NAME)) {
		printf(PRINT_ERROR "Could not initialize game window\n");
        quit_game();
		return 1;
    }

	TTF_Init();
	if (init_global_fonts()) {
		printf(PRINT_ERROR "Could not initialize global fonts\n");
		quit_game();
		return 1;
	}

	if (init_scenes()) {
		printf(PRINT_ERROR "Could not initialize game scenes\n");
		quit_game();
		return 1;
	}

	return 0;
}

void quit_game(void) {
	if (get_current_scene()) destroy_scenes();
	if (get_global_fonts()) destroy_global_fonts();
	if (get_game_window()) destroy_game_window();

    SDL_Quit();
	exit(EXIT_SUCCESS);
}

int run_game_loop(void) {
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not access the game window when starting the game loop\n");
		return 1;
	}

	int err = 0;
    SDL_Event event;
	Uint64 last;
	while (game_window->running) {
		Uint64 now = SDL_GetPerformanceCounter();
		float dt = (float) (now - last) / (float) SDL_GetPerformanceFrequency();
		last = now;
	
		Scene* current_scene = get_current_scene();

		// Events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) game_window->running = 0;

			handle_widget_events(current_scene->widget_cont, &event);
		}

		// Update
		if (update_widgets(current_scene->widget_cont, dt)) {
			game_window->running = 0;
			printf(PRINT_ERROR "Could not update the current game scene\n");
			err = 1;
		}

		// Render
		SDL_SetRenderDrawColor(game_window->sdl_renderer, 30, 30, 30, 255);
		SDL_RenderClear(game_window->sdl_renderer);
		
		if (render_current_scene()) {
			game_window->running = 0;
			printf(PRINT_ERROR "Could not render the current game scene\n");
			err = 1;
		}
		SDL_RenderPresent(game_window->sdl_renderer);
	}

	return err;
}
