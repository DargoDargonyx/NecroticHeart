/**
 * @file scene.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "engine/scene.h"
#include "engine/game.h"
#include "engine/map.h"
#include "util/font.h"

#include <stdlib.h>


static SceneManager* scene_manager = NULL;

// Start menu scene

int init_start_menu_scene(void) {
	Scene* scene = malloc(sizeof(Scene));
	scene->type = START_MENU_SCENE;
	scene->widget_cont = create_widget_cont();
	scene->destroy = destroy_start_menu_scene;

	SDL_Color white = {255, 255, 255, 255};
	
	// Menu buttons
	Button* play_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) { .w = 650, .h = 130 },
		(IntPos) { .x = 130, .y = 390 },
		"Play",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		request_switch_to_play_scene
	);
	if (add_widget_to_cont(scene->widget_cont, (Widget*) play_btn)) 
		return 1;

	Button* settings_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) { .w = 650, .h = 130 },
		(IntPos) { .x = 130, .y = 585 },
		"Settings",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		request_switch_to_settings_scene
	);
	if (add_widget_to_cont(scene->widget_cont, (Widget*) settings_btn)) 
		return 1;

	Button* quit_btn = create_button(
		LONG_TRANSPARENT_BTN,
		(IntSize) { .w = 650, .h = 130 },
		(IntPos) { .x = 130, .y = 780 },
		"Quit",
		white,
		GLOBAL_FONT_START_MENU_BTN,
		20.0f,
		100.0f,
		exit_game_successfully
	);
	if (add_widget_to_cont(scene->widget_cont, (Widget*) quit_btn))
		return 1;

	scene_manager->current_scene = scene;
	return 0;
}

void destroy_start_menu_scene(Scene* self) { 
	if (!self) {
		printf(PRINT_WARNING "Attempted to destroy a null scene\n");
		return;
	}

	if (self->widget_cont) destroy_widget_cont(self->widget_cont);
	free(self); 
}

void request_switch_to_start_scene(void) {
	if (scene_manager->current_scene_type == START_MENU_SCENE) {
		printf(PRINT_WARNING "Attempting to switch scene to start menu when " 
				"already on the start menu scene\n");
		return;
	}

	scene_manager->requested_scene_type = START_MENU_SCENE;
	scene_manager->scene_switch_requested = 1;
}

// Settings menu scene

int init_settings_menu_scene(void) {
	Scene* scene = malloc(sizeof(Scene));
	scene->type = SETTINGS_MENU_SCENE;
	scene->widget_cont = create_widget_cont();
	scene->destroy = destroy_settings_menu_scene;

	scene_manager->current_scene = scene;
	return 0;
}

void destroy_settings_menu_scene(Scene* self) {
	if (!self) {
		printf(PRINT_WARNING "Attempted to destroy a null scene\n");
		return;
	}

	if (self->widget_cont) destroy_widget_cont(self->widget_cont);
	free(self); 
}

void request_switch_to_settings_scene(void) {
	if (scene_manager->current_scene_type == SETTINGS_MENU_SCENE) {
		printf(PRINT_WARNING "Attempting to switch scene to settings menu when " 
				"already on the settings menu scene\n");
		return;
	}

	scene_manager->requested_scene_type = SETTINGS_MENU_SCENE;
	scene_manager->scene_switch_requested = 1;
}

// Play scene

int init_play_scene(void) {
	Scene* scene = malloc(sizeof(Scene));
	scene->type = PLAY_SCENE;
	scene->widget_cont = create_widget_cont();
	scene->destroy = destroy_play_scene;

	if (init_tile_definitions()) {
		printf(PRINT_ERROR "Failed to initialize the tile definitions\n");
		return 1;
	}

	if (init_tilesheet_manager()) {
		printf(PRINT_ERROR "Failed to initialize the tilesheet manager\n");
		return 1;
	}

	if (init_map_manager()) {
		printf(PRINT_ERROR "Failed to initialize the map manager\n");
		return 1;
	}

	scene_manager->current_scene = scene;
	return 0;
}

void destroy_play_scene(Scene* self) {
	if (!self) {
		printf(PRINT_WARNING "Attempted to destroy a null scene\n");
		return;
	}

	if (self->widget_cont) destroy_widget_cont(self->widget_cont);
	
	if (get_tile_definitions())	destroy_tile_definitions();
	if (get_tilesheet_manager()) destroy_tilesheet_manager();
	if (get_map_manager()) destroy_map_manager();

	free(self);
}

void request_switch_to_play_scene(void) {
	if (scene_manager->current_scene_type == PLAY_SCENE) {
		printf(PRINT_WARNING "Attempting to switch scene to play when " 
				"already on the play scene\n");
		return;
	}

	scene_manager->requested_scene_type = PLAY_SCENE;
	scene_manager->scene_switch_requested = 1;
}

// Scene manager

int init_scene_manager(void) {
	if (scene_manager) {
		printf(PRINT_WARNING "Attempted to initialize the scene manager when one "
				"has already been initialized\n");

		return 0;
	}

	scene_manager = malloc(sizeof(SceneManager));
	scene_manager->current_scene_type = START_MENU_SCENE;
	scene_manager->current_scene = NULL;
	return init_current_scene();
}

void destroy_scene_manager(void) {
	if (!scene_manager) {
		printf(PRINT_WARNING "Attempting to destroy a null scene manager\n");
		return;
	}

	if (scene_manager->current_scene) {
		scene_manager->current_scene->destroy(scene_manager->current_scene);
		scene_manager->current_scene = NULL;
	}
	
	free(scene_manager);
	scene_manager = NULL;
}

SceneManager* get_scene_manager(void) { return scene_manager; }

int init_current_scene(void) {
	if (!scene_manager) {
		printf(PRINT_ERROR 
				"Could not initialize current scene when scene manager is null\n");
		return 1;
	}
	if (scene_manager->current_scene) {
		printf(PRINT_ERROR "Could not initialize the current scene because the "
				"scene manager already has a current scene\n");
		return 1;
	}

	switch (scene_manager->current_scene_type) {
		case START_MENU_SCENE:	  return init_start_menu_scene();
		case SETTINGS_MENU_SCENE: return init_settings_menu_scene();
		case PLAY_SCENE:		  return init_play_scene();
		default:
			printf(PRINT_ERROR "Could not initialize the current scene because its " 
					"type is unknown\n");
			return 1;
	}

	return 0;
}

int apply_scene_switch_request(void) {
	scene_manager->current_scene_type = scene_manager->requested_scene_type;
	scene_manager->scene_switch_requested = 0;
	scene_manager->current_scene->destroy(scene_manager->current_scene);
	scene_manager->current_scene = NULL;

	return init_current_scene();
}
