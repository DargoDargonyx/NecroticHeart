/**
 * @file game.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "window/display.h"
#include "window/render.h"

int update_current_scene(float dt) {
	Scene* current_scene = get_current_scene();
	if (update_widgets(current_scene->widget_cont, dt)) return 1;
	return 0;
}

int run_game_loop() {
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

		// Events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) game_window->running = 0;
		}

		// Update
		if (update_current_scene(dt)) {
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
