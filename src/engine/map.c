/**
 * @file map.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "engine/map.h"
#include "engine/tiles.h"
#include "util/helper.h"
#include "util/file.h"

#include <stdlib.h>
#include <stdio.h>


static MapManager* map_manager = NULL;

// Maps

void destroy_map(Map* self) {
	if (!self) {
		printf(PRINT_WARNING "Attempting to destroy a null map\n");
	}

	free(self);
}

// Map manager

int init_map_manager(void) {
	if (map_manager) {
		printf(PRINT_WARNING "Attempting to initialize a map manager that "
				"has already been initialized\n");
		return 0;
	}
	map_manager = malloc(sizeof(MapManager));
	map_manager->current_map = load_map(MAP_TESTING_YARD);

	if (!map_manager->current_map) {
		printf(PRINT_ERROR "Failed to load the map while initializing the map manager\n");
		return 1;
	}

	return 0;
}

void destroy_map_manager(void) {
	if (!map_manager) {
		printf("Attempting to destroy a null map manager\n");
		return;
	}

	free(map_manager);
	map_manager = NULL;
}

MapManager* get_map_manager(void) { return map_manager; }

Map* load_map(MapName name) {
	const char* filename;
	switch(name) {
		case MAP_TESTING_YARD: 
			filename = TESTING_YARD_MAP_PATH;
			break;
		default:
			printf(PRINT_ERROR "Could not load a map of an unknown name\n");
			return NULL;
	}

    char* json_text = read_json(filename);
    if (!json_text) {
		printf(PRINT_ERROR "Failed to parse json file when loading a map\n");
		return NULL;
	}

    cJSON* map_json = cJSON_Parse(json_text);
    free(json_text);
    if (!map_json) {
		printf(PRINT_ERROR "Failed to initialize json objects when loading a map\n");
		return NULL;
	}

    cJSON* width_json = cJSON_GetObjectItem(map_json, "width");
    cJSON* height_json = cJSON_GetObjectItem(map_json, "height");
    cJSON* tiles_json = cJSON_GetObjectItem(map_json, "tiles");

    if (!cJSON_IsNumber(width_json) 
			|| !cJSON_IsNumber(height_json) 
			|| !cJSON_IsArray(tiles_json)) {
        
		cJSON_Delete(map_json);
        return NULL;
    }

	Map* map = malloc(sizeof(Map));
    map->world_size.w = width_json->valueint;
    map->world_size.h = height_json->valueint;
    int count = map->world_size.w * map->world_size.h;
    map->tiles = calloc(count, sizeof(Tile));

    if (!map->tiles) {
        cJSON_Delete(map_json);
        return NULL;
    }

	for (int i = 0; i < count; i++) {
		cJSON* value = cJSON_GetArrayItem(tiles_json, i);
		if (!cJSON_IsNumber(value)) continue;
		map->tiles[i].type = value->valueint;
	}

	for (int y = 0; y < map->world_size.h; y++) {
		for (int x = 0; x < map->world_size.w; x++) {
			Tile* tile = &map->tiles[y * map->world_size.w + x];
			
			tile->sprite_id = choose_tile_sprite(
				map->tiles, 
				map->world_size, 
				(IntPos) { .x = x, .y = y }
			);

			if (tile->sprite_id == -1) {
				printf(PRINT_ERROR "Failed to find a matching sprite id for a tile "
						"with rules\n");
				
				cJSON_Delete(map_json);
				return NULL;
			}
		}
	}

    cJSON_Delete(map_json);
    return map;
}
