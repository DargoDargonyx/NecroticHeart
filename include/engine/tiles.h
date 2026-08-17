/**
 * @file tiles.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#ifndef TILES_H
#define TILES_H

#include "util/helper.h"
#include "external/cJSON.h"

#include <SDL2/SDL.h>


// Tiles
#define TILE_PIXEL_SIZE 64

typedef enum {
	TILE_DIRT,
	TILE_GRASS,
	TILE_ROCK_BORDER,
	TILE_VOID
} TileType;

typedef struct {
	TileType type;
	FloatPos world_pos;
	int layer;

	int sprite_id;
} Tile;

int get_tile_id_from_name(const char*);
TileType get_tile_type(Tile*, IntSize, IntPos);
int choose_tile_sprite(Tile*, IntSize, IntPos);
int choose_random_tile_sprite(int*, int);

// Tilesheets
#define DIRT_TILESHEET_PATH			"../assets/tiles/dirt_tileset.png"
#define GRASS_TILESHEET_PATH		"../assets/tiles/grass_tileset.png"
#define ROCK_BORDER_TILESHEET_PATH	"../assets/tiles/rock_border_tileset.png"

#define TILESHEET_COUNT 3
#define DIRT_TILE_INDEX			0
#define GRASS_TILE_INDEX		1
#define ROCK_BORDER_TILE_INDEX	2

typedef enum {
	DIRT_TILESHEET,
	GRASS_TILESHEET,
	ROCK_BORDER_TILESHEET
} TilesheetName;

typedef struct {
	SDL_Texture* sdl_texture;
	IntSize size;
} Tilesheet;

Tilesheet* create_dirt_tilesheet(void);
Tilesheet* create_grass_tilesheet(void);
Tilesheet* create_rock_border_tilesheet(void);

void destroy_tilesheet(Tilesheet*);
Tilesheet* get_tilesheet(TilesheetName);

// Tilesheet manager
typedef struct {
	int tilesheet_count;
	Tilesheet** tilesheets;
} TilesheetManager;

int init_tilesheet_manager(void);
void destroy_tilesheet_manager(void);
TilesheetManager* get_tilesheet_manager(void);

// Tile requirements
typedef enum {
	REQUIRE_ANY,
	REQUIRE_SELF,
	REQUIRE_OTHER,
	REQUIRE_EDGE,
	REQUIRE_TYPE
} TileRequirementType;

typedef struct {
	TileRequirementType type;
	TileType tile_type;
} TileRequirement;

TileRequirement parse_tile_requirement(cJSON*);

// Tiling rules
#define DIRT_TILE_RULE_PATH			"../assets/tile_rules/dirt.json"
#define GRASS_TILE_RULE_PATH		"../assets/tile_rules/grass.json"
#define ROCK_BORDER_TILE_RULE_PATH	"../assets/tile_rules/rock_border.json"

typedef struct {
	TileRequirement north;
	TileRequirement north_east;
	TileRequirement east;
	TileRequirement south_east;
	TileRequirement south;
	TileRequirement south_west;
	TileRequirement west;
	TileRequirement north_west;

	int* sprites;
	int sprite_count;
} TileRule;

TileRule parse_tile_rule(cJSON*);
int tile_rule_matches(Tile*, IntSize, IntPos, TileRule*);

// Tile definitions
typedef struct {
	char* name;
	int id;
	int layer;

	int* sprites;
	int sprite_count;

	TileRule* rules;
	int rule_count;

	Tilesheet* tilesheet;
} TileDefinition;

typedef struct {
	TileDefinition* definitions;
	int definition_count;
} TileDefinitions;

int init_tile_definitions(void);
void destroy_tile_definitions(void);
TileDefinitions* get_tile_definitions(void);

int read_tile_definitions(void);
int read_tile_definition(int, const char*);


#endif // TILES_H
