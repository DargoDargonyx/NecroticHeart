/**
 * @file scene.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef SCENE_H
#define SCENE_H

#include "engine/widget.h"

// General scenes
typedef enum {
	START_MENU_SCENE,
	SETTINGS_MENU_SCENE,
	PLAY_SCENE
} SceneType;

typedef struct Scene Scene;
struct Scene {
	SceneType type;
	WidgetCont* widget_cont;
	void (*destroy)(Scene*);
};

// Start menu scene
int init_start_menu_scene(void);
void destroy_start_menu_scene(Scene*);
void request_switch_to_start_scene(void);

// Settings menu scene
int init_settings_menu_scene(void);
void destroy_settings_menu_scene(Scene*);
void request_switch_to_settings_scene(void);

// Play scene
int init_play_scene(void);
void destroy_play_scene(Scene*);
void request_switch_to_play_scene(void);

// Scene manager
typedef struct {
	SceneType current_scene_type;
	Scene* current_scene;

	int scene_switch_requested;
	SceneType requested_scene_type;
} SceneManager;

int init_scene_manager(void);
void destroy_scene_manager(void);
SceneManager* get_scene_manager(void);
int init_current_scene(void);
int apply_scene_switch_request(void);

#endif // SCENE_H
