/**
 * @file camera.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "window/camera.h"
#include "window/display.h"

#include <stdlib.h>
#include <stdio.h>


static Camera* play_camera = NULL;

// Play camera

int init_play_camera(void) {
	if (play_camera) {
		printf(PRINT_WARNING "Attempting to initialize the play camera when "
				"it has already been initialized\n");
		return 0;
	}

	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not initialize the play scene with a "
				"null game window\n");
		return 1;
	}

	play_camera = malloc(sizeof(Camera));
    SDL_GetRendererOutputSize(
        game_window->sdl_renderer,
        &play_camera->pixel_size.w,
        &play_camera->pixel_size.h
    );
	play_camera->pixel_pos = (IntPos) { .x = 0, .y = 0 };

	return 0;
}

void destroy_play_camera(void) {
	if (!play_camera) {
		printf(PRINT_WARNING "Attempting to destroy a null play camera\n");
		return;
	}

	free(play_camera);
	play_camera = NULL;
}

Camera* get_play_camera(void) { return play_camera; }
