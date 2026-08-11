/**
 * @file scene.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "engine/scene.h"
#include "window/display.h"
#include "engine/game.h"
#include "util/font.h"

#include <stdlib.h>


static Scene* start_menu_scene = NULL;
static Scene* settings_menu_scene = NULL;
static Scene* play_scene = NULL;

// General scenes

int init_scenes(void) {
	if (init_start_menu_scene()) return 1;
	if (init_settings_menu_scene()) return 1;
	if (init_play_scene()) return 1;

	return 0;
}

void destroy_scenes(void) {
	destroy_start_menu_scene();	
}

Scene* get_current_scene(void) {
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR 
			   "Could not access the game window when grabbing the current scene\n");
		return NULL;
	}

	switch (game_window->current_scene) {
		case START_MENU_SCENE: return get_start_menu_scene();
		case SETTINGS_MENU_SCENE: return get_settings_menu_scene();
		case PLAY_SCENE: return get_play_scene();
		case INVENTORY_SCENE: // @TODO
		default:
			printf(PRINT_ERROR 
				   "Could not grab the current scene because its type is unknown\n");
			return NULL;
	}
}

void destroy_current_scene(void) {
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR 
			   "Could not access the game window when destroying the current scene\n");
		return;
	}

	switch (game_window->current_scene) {
		case START_MENU_SCENE: destroy_start_menu_scene();
		case SETTINGS_MENU_SCENE: destroy_settings_menu_scene();
		case PLAY_SCENE: destroy_play_scene();
		case INVENTORY_SCENE: //@TODO
		default:
			printf(PRINT_ERROR 
				   "Could not destroy the current scene because its type is unknown\n");
	}

}

// Start menu scene

int init_start_menu_scene(void) {
	if (start_menu_scene) return 1;

	start_menu_scene = malloc(sizeof(Scene));
	start_menu_scene->type = START_MENU_SCENE;
	start_menu_scene->widget_cont = create_widget_cont();

	SDL_Color white = {255, 255, 255, 255};
	
	// Menu buttons
	Button* start_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) {.w = 650, .h = 130},
		(IntPos) {.x = 130, .y = 390},
		"Start",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		switch_scene_to_start
	);
	if (add_widget_to_cont(start_menu_scene->widget_cont, (Widget*) start_btn)) 
		return 1;

	Button* settings_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) {.w = 650, .h = 130},
		(IntPos) {.x = 130, .y = 585},
		"Settings",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		switch_scene_to_settings
	);
	if (add_widget_to_cont(start_menu_scene->widget_cont, (Widget*) settings_btn)) 
		return 1;

	Button* quit_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) {.w = 650, .h = 130},
		(IntPos) {.x = 130, .y = 780},
		"Quit",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		quit_game
	);
	if (add_widget_to_cont(start_menu_scene->widget_cont, (Widget*) quit_btn))
		return 1;

	return 0;
}

void destroy_start_menu_scene(void) { 
	if (!start_menu_scene) return;
	if (start_menu_scene->widget_cont) 
		destroy_widget_cont(start_menu_scene->widget_cont);

	free(start_menu_scene); 
	start_menu_scene = NULL;
}

Scene* get_start_menu_scene(void) { return start_menu_scene; }

void switch_scene_to_start(void) {

}

// Settings meny scene

int init_settings_menu_scene(void) {}

void destroy_settings_menu_scene(void) {}

Scene* get_settings_menu_scene(void) { return settings_menu_scene; }

void switch_scene_to_settings(void) {}

// Play scene

int init_play_scene(void) {}

void destroy_play_scene(void) {}

Scene* get_play_scene(void) { return play_scene; }

void switch_scene_to_play(void) {}
