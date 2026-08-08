/**
 * @file game.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "window/display.h"

int run_game_loop() {
    SDL_Event event;
	GameWindow* game_window = get_game_window();

	while (game_window->running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				game_window->running = 0;
			}
		}

		SDL_SetRenderDrawColor(game_window->sdl_renderer, 30, 30, 30, 255);
		SDL_RenderClear(game_window->sdl_renderer);
		
		SDL_RenderPresent(game_window->sdl_renderer);
	}

	return 0;
}
