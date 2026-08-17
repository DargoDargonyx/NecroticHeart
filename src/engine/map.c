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

    if (self->layers) {
        for (int i = 0; i < self->layer_count; i++) {
            free(self->layers[i].name);
            free(self->layers[i].tiles);
        }
        free(self->layers);
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
    cJSON* layers_json = cJSON_GetObjectItem(map_json, "layers");

    if (!cJSON_IsNumber(width_json) 
			|| !cJSON_IsNumber(height_json) 
			|| !cJSON_IsArray(layers_json)) {
        
		cJSON_Delete(map_json);
        return NULL;
    }

	Map* map = malloc(sizeof(Map));
    map->size.w = width_json->valueint;
    map->size.h = height_json->valueint;
	int tile_count = map->size.w * map->size.h;

	map->layer_count = cJSON_GetArraySize(layers_json);
    map->layers = calloc(map->layer_count, sizeof(MapLayer));
    if (!map->layers) {
        cJSON_Delete(map_json);
        return NULL;
    }

	// Load each layer
    for (int layer_index = 0;
         layer_index < map->layer_count;
         layer_index++) {

        cJSON* layer_json = cJSON_GetArrayItem(layers_json, layer_index);
        cJSON* name_json = cJSON_GetObjectItem(layer_json, "name");
        cJSON* tiles_json = cJSON_GetObjectItem(layer_json, "tiles");

        if (!cJSON_IsString(name_json) || !cJSON_IsArray(tiles_json)) {
            printf(PRINT_ERROR "Invalid map layer\n");

            cJSON_Delete(map_json);
            destroy_map(map);
            return NULL;
        }

        MapLayer* layer = &map->layers[layer_index];

        layer->name = strdup(name_json->valuestring);
        layer->tiles = calloc(tile_count, sizeof(Tile));

        if (!layer->tiles) {
            printf(PRINT_ERROR "Failed to allocate tiles for layer %s\n", 
					layer->name);

            cJSON_Delete(map_json);
            destroy_map(map);
            return NULL;
        }

        // Load tiles
        for (int i = 0; i < tile_count; i++) {
            cJSON* value = cJSON_GetArrayItem(tiles_json, i);

            if (!cJSON_IsNumber(value)) {
                layer->tiles[i].type = TILE_EMPTY;
                continue;
            }

            layer->tiles[i].type = value->valueint;

            int x = i % map->size.w;
            int y = i / map->size.w;

            layer->tiles[i].world_pos = (FloatPos) { .x = x, .y = y };
        }
    }
    cJSON_Delete(map_json);
    
	for (int layer_index = 0; layer_index < map->layer_count; layer_index++) {
		MapLayer* layer = &map->layers[layer_index];

		for (int y = 0; y < map->size.h; y++) {
			for (int x = 0; x < map->size.w; x++) {
				Tile* tile = &layer->tiles[y * map->size.w + x];

				if (tile->type == TILE_EMPTY) {
					tile->sprite_id = -1;
					continue;
				}

				tile->sprite_id = choose_tile_sprite(
					layer->tiles,
					map->size,
					(IntPos) { .x = x, .y = y }
				);

				if (tile->sprite_id < 0) {
					printf(PRINT_ERROR "Failed to choose sprite for "
							"tile at (%d, %d)\n", x, y);
				}
			}
		}
	}

	return map;
}
