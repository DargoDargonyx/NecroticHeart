/**
 * @file tiles.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "engine/tiles.h"
#include "window/display.h"
#include "external/cJSON.h"
#include "util/file.h"

#include <SDL2/SDL_image.h>


static TilesheetManager* tilesheet_manager = NULL;
static TileDefinitions* tile_definitions = NULL;

// Tiles

int get_tile_id_from_name(const char* name) {
	if (strcmp(name, "dirt") == 0) return 0;
	else if (strcmp(name, "grass") == 0) return 1;
	else if (strcmp(name, "rock_border") == 0) return 2;
	else return -1;
}

TileType get_tile_type(Tile* map_tiles, IntSize map_size, IntPos tile_pos) {
    if (tile_pos.x < 0 
			|| tile_pos.x >= map_size.w  
			|| tile_pos.y < 0 
			|| tile_pos.y >= map_size.h) {

        return TILE_VOID;
    }

    return map_tiles[tile_pos.y * map_size.w + tile_pos.x].type;
}

int choose_tile_sprite(Tile* map_tiles, IntSize map_size, IntPos tile_pos) {
    Tile* tile = &map_tiles[tile_pos.y * map_size.w + tile_pos.x];
	TileDefinitions* tile_definitions = get_tile_definitions();
    TileDefinition* definition = &tile_definitions->definitions[tile->type];

    for (int i = 0; i < definition->rule_count; i++) {
        TileRule* rule = &definition->rules[i];
        if (tile_rule_matches(map_tiles, map_size, tile_pos, rule)) {
			return choose_random_tile_sprite(rule->sprites, rule->sprite_count);
		}
    }

    return choose_random_tile_sprite(definition->sprites, definition->sprite_count);
}

int choose_random_tile_sprite(int* sprites, int sprite_count) {
    if (sprite_count == 0) return -1;
    int index = rand() % sprite_count;

    return sprites[index];
}

// Tilesheets

Tilesheet* create_dirt_tilesheet(void) {
	Tilesheet* tilesheet = malloc(sizeof(Tilesheet));
	tilesheet->size = (IntSize) { .w = 3, .h = 3 };

	GameWindow* game_window = get_game_window();
	tilesheet->sdl_texture = IMG_LoadTexture(
		game_window->sdl_renderer,
		DIRT_TILESHEET_PATH
	);
	if (!tilesheet->sdl_texture)
		printf(PRINT_ERROR "Failed to load the dirt tilesheet sdl texture\n");

	return tilesheet;
}

Tilesheet* create_grass_tilesheet(void) {
	Tilesheet* tilesheet = malloc(sizeof(Tilesheet));
	tilesheet->size = (IntSize) { .w = 3, .h = 3 };

	GameWindow* game_window = get_game_window();
	tilesheet->sdl_texture = IMG_LoadTexture(
		game_window->sdl_renderer,
		GRASS_TILESHEET_PATH
	);
	if (!tilesheet->sdl_texture)
		printf(PRINT_ERROR "Failed to load the grass tilesheet sdl texture\n");

	return tilesheet;
}

Tilesheet* create_rock_border_tilesheet(void) {
	Tilesheet* tilesheet = malloc(sizeof(Tilesheet));
	tilesheet->size = (IntSize) { .w = 3, .h = 3 };
	
	GameWindow* game_window = get_game_window();
	tilesheet->sdl_texture = IMG_LoadTexture(
		game_window->sdl_renderer,
		ROCK_BORDER_TILESHEET_PATH
	);
	if (!tilesheet->sdl_texture)
		printf(PRINT_ERROR "Failed to load the rock border tilesheet sdl texture\n");

	return tilesheet;
}

void destroy_tilesheet(Tilesheet* self) {
	if (!self) {
		printf(PRINT_WARNING "Attempting to destroy a null tilesheet\n");
		return;
	}

	SDL_DestroyTexture(self->sdl_texture);
	free(self);
}

Tilesheet* get_tilesheet(TilesheetName name) {
	if (!tilesheet_manager) {
		printf(PRINT_ERROR "Attempted to retrieve a tilesheet when the tilesheet "
				"manager is null\n");
		return NULL;
	}

	switch (name) {
		case GRASS_TILESHEET: 
			return tilesheet_manager->tilesheets[GRASS_TILE_INDEX];
		case DIRT_TILESHEET: 
			return tilesheet_manager->tilesheets[DIRT_TILE_INDEX];
		case ROCK_BORDER_TILESHEET: 
			return tilesheet_manager->tilesheets[ROCK_BORDER_TILE_INDEX];
		default:
			printf(PRINT_ERROR "Could not grab a tilesheet using an unknown name\n");
			return NULL;
	}
}

// Tilesheet manager

int init_tilesheet_manager(void) {
	if (tilesheet_manager) {
		printf(PRINT_WARNING "Attempting to initialize the tilesheet manager "
				"when one has already been initialized\n");
		return 0;
	}
	tilesheet_manager = malloc(sizeof(TilesheetManager));
	tilesheet_manager->tilesheets = calloc(TILESHEET_COUNT, sizeof(Tilesheet));
	tilesheet_manager->tilesheet_count = TILESHEET_COUNT;
	
	tilesheet_manager->tilesheets[GRASS_TILE_INDEX] = create_grass_tilesheet();
	if (!tilesheet_manager->tilesheets[GRASS_TILE_INDEX]) {
		printf("Failed to load the grass tilesheet\n");
		return 1;
	}
	tilesheet_manager->tilesheets[DIRT_TILE_INDEX] = create_dirt_tilesheet();	
	if (!tilesheet_manager->tilesheets[DIRT_TILE_INDEX]) {
		printf("Failed to load the dirt tilesheet\n");
		return 1;
	}
	tilesheet_manager->tilesheets[ROCK_BORDER_TILE_INDEX] = create_rock_border_tilesheet();	
	if (!tilesheet_manager->tilesheets[ROCK_BORDER_TILE_INDEX]) {
		printf("Failed to load the rock border tilesheet\n");
		return 1;
	}

	return 0;
}

void destroy_tilesheet_manager(void) {
	if (!tilesheet_manager) {
		printf(PRINT_WARNING "Attempting to destroy a null tilesheet manager\n");
		return;
	}

	if (tilesheet_manager->tilesheets) {
		for (int i = 0; i < tilesheet_manager->tilesheet_count; i++) {
			destroy_tilesheet(tilesheet_manager->tilesheets[i]);
		}
		free(tilesheet_manager->tilesheets);
	}

	free(tilesheet_manager);
	tilesheet_manager = NULL;
}

TilesheetManager* get_tilesheet_manager(void) { return tilesheet_manager; }

// Tiling requirements

int tile_requirement_matches(TileType current_type, TileType neighbor_type, 
		TileRequirement requirement) {
    
	switch (requirement.type) {
        case REQUIRE_ANY:	return 1;
        case REQUIRE_SELF:	return neighbor_type == current_type;
        case REQUIRE_OTHER: return neighbor_type != current_type && neighbor_type != TILE_VOID;
        case REQUIRE_EDGE:	return neighbor_type == TILE_VOID;
        case REQUIRE_TYPE:	return neighbor_type == requirement.tile_type;
    }

    return 0;
}

TileRequirement parse_tile_requirement(cJSON* json) {
    TileRequirement result = {
        .type = REQUIRE_ANY,
        .tile_type = -1
    };

    if (!json) return result;
    if (!cJSON_IsString(json)) return result;

    const char* value = json->valuestring;

    if (strcmp(value, "any") == 0) {
        result.type = REQUIRE_ANY;
    } else if (strcmp(value, "self") == 0) {
        result.type = REQUIRE_SELF;
    } else if (strcmp(value, "other") == 0) {
        result.type = REQUIRE_OTHER;
    } else if (strcmp(value, "edge") == 0) {
		result.type = REQUIRE_EDGE;
	} else {
        int id = get_tile_id_from_name(value);

        if (id >= 0) {
            result.type = REQUIRE_TYPE;
            result.tile_type = id;
        }
    }

    return result;
}

// Tiling rules

int tile_rule_matches(Tile* map_tiles, IntSize map_size, 
		IntPos tile_pos, TileRule* rule) {
    
	int current = get_tile_type(map_tiles, map_size, tile_pos);
    
	int north = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_y_to_pos(tile_pos, -1)
	);
	int north_east = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_x_to_pos(add_int_y_to_pos(tile_pos, -1), 1)
	);
    int east = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_x_to_pos(tile_pos, 1)
	);
    int south_east = get_tile_type(
		map_tiles, map_size, 
		add_int_x_to_pos(add_int_y_to_pos(tile_pos, 1), 1)
	);
    int south = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_y_to_pos(tile_pos, 1)
	);
    int south_west = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_x_to_pos(add_int_y_to_pos(tile_pos, 1), -1)
	);
    int west = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_x_to_pos(tile_pos, -1)
	);
    int north_west = get_tile_type(
		map_tiles, 
		map_size, 
		add_int_x_to_pos(add_int_y_to_pos(tile_pos, -1), -1)
	);

    return tile_requirement_matches(current, north, rule->north)
		&& tile_requirement_matches(current, north_east, rule->north_east)
		&& tile_requirement_matches(current, east, rule->east) 
		&& tile_requirement_matches(current, south_east, rule->south_east) 
		&& tile_requirement_matches(current, south, rule->south) 
		&& tile_requirement_matches(current, south_west, rule->south_west) 
		&& tile_requirement_matches(current, west, rule->west)
		&& tile_requirement_matches(current, north_west, rule->north_west);
}

TileRule parse_tile_rule(cJSON* json) {
    TileRule rule = {0};
	cJSON* when = cJSON_GetObjectItem(json, "when");

	rule.north = parse_tile_requirement(cJSON_GetObjectItem(when, "N"));
	rule.north_east = parse_tile_requirement(cJSON_GetObjectItem(when, "NE"));
    rule.east = parse_tile_requirement(cJSON_GetObjectItem(when, "E"));
    rule.south_east = parse_tile_requirement(cJSON_GetObjectItem(when, "SE"));
    rule.south = parse_tile_requirement(cJSON_GetObjectItem(when, "S"));
    rule.south_west = parse_tile_requirement(cJSON_GetObjectItem(when, "SW"));
    rule.west = parse_tile_requirement(cJSON_GetObjectItem(when, "W"));
    rule.north_west = parse_tile_requirement(cJSON_GetObjectItem(when, "NW"));

    cJSON* sprites = cJSON_GetObjectItem(json, "sprites");
    rule.sprite_count = cJSON_GetArraySize(sprites);
    rule.sprites = calloc(rule.sprite_count, sizeof(int));

    for (int i = 0; i < rule.sprite_count; i++)
        rule.sprites[i] = cJSON_GetArrayItem(sprites, i)->valueint;

    return rule;
}

// Tile definitions

int init_tile_definitions(void) {
	if (tile_definitions) {
		printf(PRINT_WARNING "Attempting to initialize the tile definitions when "
				"they have already been initialized\n");
		return 0;
	}
	tile_definitions = malloc(sizeof(TileDefinitions));
	tile_definitions->definition_count = TILESHEET_COUNT;
	tile_definitions->definitions = calloc(TILESHEET_COUNT, sizeof(TileDefinition));
		
	if (read_tile_definitions()) {
		printf(PRINT_ERROR "Failed to read tile definitions while initializing them\n");
		return 1;
	}

	if (init_tilesheet_manager()) {
		printf(PRINT_ERROR "Failed to initialize tilesheet manager\n");
		return 1;
	}

	tile_definitions->definitions[DIRT_TILE_INDEX].tilesheet = 
		tilesheet_manager->tilesheets[DIRT_TILE_INDEX];
	tile_definitions->definitions[GRASS_TILE_INDEX].tilesheet =
		tilesheet_manager->tilesheets[GRASS_TILE_INDEX];
	tile_definitions->definitions[ROCK_BORDER_TILE_INDEX].tilesheet = 
		tilesheet_manager->tilesheets[ROCK_BORDER_TILE_INDEX];

	return 0;
}

void destroy_tile_definitions(void) {
	if (!tile_definitions) {
		printf(PRINT_WARNING "Attempting to destroy a null tile definitions container\n");
		return;
	}

	if (tile_definitions->definitions) free(tile_definitions->definitions);
	
	free(tile_definitions);
	tile_definitions = NULL;
}

TileDefinitions* get_tile_definitions(void) { return tile_definitions; }

int read_tile_definitions(void) {
	if (!tile_definitions) {
		printf(PRINT_ERROR "Could not read tile definitions with a null tile "
				"definitions container\n");
		return 1;
	}

	if (read_tile_definition(GRASS_TILE_INDEX, GRASS_TILE_RULE_PATH)) {
		printf(PRINT_ERROR "Could not read the grass tile definition\n");
		return 1;
	}

	if (read_tile_definition(DIRT_TILE_INDEX, DIRT_TILE_RULE_PATH)) {
		printf(PRINT_ERROR "Could not read the dirt tile definition\n");
		return 1;
	}

	if (read_tile_definition(ROCK_BORDER_TILE_INDEX, ROCK_BORDER_TILE_RULE_PATH)) {
		printf(PRINT_ERROR "Could not read the rock border tile definition\n");
		return 1;
	}

	return 0;
}

int read_tile_definition(int tile_index, const char* tile_rule_path) {
	cJSON* tile_json = load_tile_definition(tile_rule_path);
	if (!tile_json) {
		printf(PRINT_ERROR "Could not load the grass tile definition file\n");
		return 1;
	}
	
	TileDefinition* tile = &tile_definitions->definitions[tile_index];

	cJSON* name = cJSON_GetObjectItem(tile_json, "name");
	cJSON* id = cJSON_GetObjectItem(tile_json, "id");
	cJSON* layer = cJSON_GetObjectItem(tile_json, "layer");
	tile->name = strdup(name->valuestring);
	tile->id = id->valueint;
	tile->layer = layer->valueint;
	
	cJSON* sprites = cJSON_GetObjectItem(tile_json, "sprites");
	tile->sprite_count = cJSON_GetArraySize(sprites);
	tile->sprites = malloc(sizeof(int) * tile->sprite_count);
	
	for (int j = 0; j < tile->sprite_count; j++) {
		cJSON* sprite = cJSON_GetArrayItem(sprites, j);
		tile->sprites[j] = sprite->valueint;
	}

	cJSON* rules = cJSON_GetObjectItem(tile_json, "rules");
	if (rules && cJSON_IsArray(rules)) {
		tile->rule_count = cJSON_GetArraySize(rules);
		tile->rules = calloc(tile->rule_count, sizeof(TileRule));

		for (int j = 0; j < tile->rule_count; j++) {
			cJSON* rule_json = cJSON_GetArrayItem(rules, j);
			tile->rules[j] = parse_tile_rule(rule_json);
		}
	}

	return 0;
}
