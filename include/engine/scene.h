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
	PLAY_SCENE,
	INVENTORY_SCENE
} SceneType;

typedef struct {
	SceneType type;
	WidgetCont* widget_cont;
} Scene;

int init_scenes(void);
void destroy_scenes(void);
Scene* get_current_scene(void);

// Start menu scene

int init_start_menu_scene(void);
void destroy_start_menu_scene(void);
Scene* get_start_menu_scene(void);

#endif // SCENE_H
