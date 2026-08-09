/**
 * @file render.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "engine/widget.h"

// Scenes
int render_current_scene();

// Widgets
int render_widgets(WidgetCont*);
int render_button(Button*);
int render_info_tag(InfoTag*);
