/**
 * @file game.c
 * @author DargoDargonyx
 * @date 08/09/2026
 */

#include "window/display.h"
#include "window/render.h"

int update_current_scene() {
	Scene* current_scene = get_current_scene();
	if (update_widgets(current_scene->widget_cont)) return 1;
	return 0;
}

int run_game_loop() {
	int err = 0;
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

		if (update_current_scene()) {
			game_window->running = 0;
			printf(PRINT_ERROR "Could not update the current game scene\n");
			err = 1;
		}

		if (render_current_scene()) {
			game_window->running = 0;
			printf(PRINT_ERROR "Could not render the current game scene\n");
			err = 1;
		}

		SDL_RenderPresent(game_window->sdl_renderer);
	}

	return err;
}
