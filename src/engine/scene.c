/**
 * @file scene.c
 * @author DargoDargonyx
 * @date 08/09/2026
 */

#include "engine/scene.h"
#include "util/font.h"
#include "window/display.h"

#include <stdlib.h>


static Scene* start_menu_scene = NULL;

int init_scenes(void) {
	if (init_start_menu_scene()) return 1;

	return 0;
}

void destroy_scenes(void) {
	destroy_start_menu_scene();	
}

Scene* get_current_scene(void) {
	GameWindow* game_window = get_game_window();

	switch (game_window->current_scene) {
		case START_MENU_SCENE: return get_start_menu_scene();
		case SETTINGS_MENU_SCENE:
			// @TODO
		case PLAY_SCENE:
			// @TODO
		case INVENTORY_SCENE:
			// @TODO
		default:
			printf(PRINT_ERROR "Could not render a scene of unknown type\n");
			return NULL;
	}
}

// Start menu scene

int init_start_menu_scene(void) {
	if (start_menu_scene) return 1;

	start_menu_scene = malloc(sizeof(Scene));
	start_menu_scene->type = START_MENU_SCENE;
	start_menu_scene->widget_cont = create_widget_cont();

	// Buttons
	SDL_Color white = {255, 255, 255, 255};

	Button* start_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) {650, 130},
		(IntPos) {130, 390},
		"Start",
		white,
		GLOBAL_FONT_START_MENU_BTN
	);

	if (add_widget_to_cont(start_menu_scene->widget_cont, (Widget*) start_btn)) 
		return 1;

	Button* settings_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) {650, 130},
		(IntPos) {130, 585},
		"Settings",
		white,
		GLOBAL_FONT_START_MENU_BTN
	);
	
	if (add_widget_to_cont(start_menu_scene->widget_cont, (Widget*) settings_btn)) 
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
