/**
 * @file render.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "engine/widget.h"
#include "engine/map.h"


// Tiles
int render_tile(Tile*);

// Map
int render_current_map(void);
int render_map(Map*);
int render_map_layer(MapLayer*, IntSize);

// Widgets
int render_widgets(WidgetCont*);
int render_button(Button*);
int render_info_tag(InfoTag*);
