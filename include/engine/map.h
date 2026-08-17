/**
 * @file map.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#ifndef MAP_H
#define MAP_H

#include "util/helper.h"
#include "engine/tiles.h"


// Maps
#define TESTING_YARD_MAP_PATH "../assets/maps/testing_yard.json"

typedef enum {
	MAP_TESTING_YARD
} MapName;

typedef struct {
	MapName name;
	IntSize size;
	Tile* tiles;
} Map;

Map* load_map(MapName);
void destroy_map(Map*);

// Map manager
typedef struct {
	Map* current_map;

	int switch_map_request;
	MapName requested_map_name;
} MapManager;

int init_map_manager(void);
void destroy_map_manager(void);
MapManager* get_map_manager(void);


#endif // MAP_H
