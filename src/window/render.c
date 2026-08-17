/**
 * @file render.c
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#include "window/render.h"
#include "window/display.h"
#include "window/camera.h"
#include "engine/map.h"


// Tile

int render_tile(Tile* tile) {
	if (!tile) {
		printf(PRINT_WARNING "Could not render a null tile\n");
		return 0;
	}

	TileDefinitions* tile_definitions = get_tile_definitions();
	if (!tile_definitions) {
		printf(PRINT_ERROR "Could not render a tile when the tile "
				"definitions are null\n");
		return 1;
	}
	
	TileDefinition* tile_definition = &tile_definitions->definitions[tile->type];
    int sprite_x = tile->sprite_id % tile_definition->tilesheet->size.w;
    int sprite_y = tile->sprite_id / tile_definition->tilesheet->size.w;

	Camera* play_camera = get_play_camera();
	if (!play_camera) {
		printf(PRINT_ERROR "Could not render a tile when the play "
				"camera is null\n");
		return 1;
	}

    SDL_Rect src = {
        .x = sprite_x * TILE_PIXEL_SIZE,
        .y = sprite_y * TILE_PIXEL_SIZE,
        .w = TILE_PIXEL_SIZE,
        .h = TILE_PIXEL_SIZE
    };
    SDL_Rect dst = {
        .x = tile->world_pos.x * TILE_PIXEL_SIZE - play_camera->pixel_pos.x,
        .y = tile->world_pos.y * TILE_PIXEL_SIZE - play_camera->pixel_pos.y,
        .w = TILE_PIXEL_SIZE,
        .h = TILE_PIXEL_SIZE
    };

    GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not access the game window while trying to "
				"render a tile\n");
		return 1;
	}

	SDL_RenderCopy(
        game_window->sdl_renderer,
        tile_definition->tilesheet->sdl_texture,
        &src,
        &dst
    );

	return 0;
}

// Map

int render_current_map(void) {
	MapManager* map_manager = get_map_manager();
	if (!map_manager) {
		printf(PRINT_ERROR "Could not render the play scene because the map "
				"manager is null\n");
		return 1;
	}

	if (!map_manager->current_map) {
		printf(PRINT_WARNING "Could not render the current map because it's null\n");
		return 0;
	}

	return render_map(map_manager->current_map);
}

int render_map(Map* map) {
	if (!map) {
		printf(PRINT_WARNING "Could not render a null map\n");
		return 0;
	}

	for (int layer = 0; layer < map->layer_count; layer++) {
		if (render_map_layer(&map->layers[layer], map->size)) {
			printf(PRINT_ERROR "Failed to render a layer of the current map\n");
			return 1;
		}
	}

	return 0;
}

int render_map_layer(MapLayer* layer, IntSize map_size) {
    Camera* play_camera = get_play_camera();
	if (!play_camera) {
		printf(PRINT_ERROR "Could not render a map with a null play camera\n");
		return 1;
	}

	int start_x = play_camera->pixel_pos.x / TILE_PIXEL_SIZE;
    int start_y = play_camera->pixel_pos.y / TILE_PIXEL_SIZE;
    int end_x = (play_camera->pixel_pos.x + play_camera->pixel_size.w) 
		/ TILE_PIXEL_SIZE + 1;
    int end_y = (play_camera->pixel_pos.y + play_camera->pixel_size.h) 
		/ TILE_PIXEL_SIZE + 1;

    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x > map_size.w) end_x = map_size.w;
    if (end_y > map_size.h) end_y = map_size.h;

	for (int y = start_y; y < end_y; y++) {
        for (int x = start_x; x < end_x; x++) {
            Tile* tile = &layer->tiles[y * map_size.w + x];
            if (tile->type == TILE_EMPTY) continue;
            if (render_tile(tile)) {
				printf(
					PRINT_WARNING "Failed to render a tile at world pos: (%f, %f)\n",
					tile->world_pos.x, 
					tile->world_pos.y
				);
			}
        }
    }

    return 0;
}

// Widgets

int render_widgets(WidgetCont* cont) {
	if (!cont) {
		printf(PRINT_WARNING "Could not render widgets for an empty widget container\n");
		return 0;
	}

	for (int i = 0; i < cont->count; i++) {
		switch (cont->widgets[i]->type) {
			case BUTTON:
				if (render_button((Button*) cont->widgets[i])) return 1;
				break;
			case INFO_TAG:
				if (render_info_tag((InfoTag*) cont->widgets[i])) return 1;
				break;
			default:
				printf(PRINT_ERROR "Could not render a widget of unknown type\n");
				return 1;
		}
	}

	return 0;
}

int render_button(Button* btn) {
	if (!btn) {
		printf(PRINT_WARNING "Could not render a null button widget\n");
		return 0;
	}

	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not access the game window while rendering a button\n");
		return 1;
	}

	int spritesheet_offset = btn->state == BTN_IDLE ? 0 : 1;
	SDL_Rect dst = btn->sdl_rect;
	dst.y += (int) btn->press_offset;

    SDL_Rect src = {
        .x = 0,
        .y = spritesheet_offset * btn->sdl_rect.h * BTN_PIXEL_SCALE_OFFSET,
        .w = btn->sdl_rect.w * BTN_PIXEL_SCALE_OFFSET,
        .h = btn->sdl_rect.h * BTN_PIXEL_SCALE_OFFSET
    };
    SDL_RenderCopy(
        game_window->sdl_renderer,
        btn->sdl_background_texture,
        &src,
        &dst
    );

	int text_w, text_h;
	SDL_QueryTexture(btn->sdl_text_texture, NULL, NULL, &text_w, &text_h);
	SDL_Rect text_rect = {
		.x = btn->sdl_rect.x + (btn->sdl_rect.w - text_w) / 2,
        .y = btn->sdl_rect.y + (int) btn->press_offset + (btn->sdl_rect.h - text_h) / 2,
        .w = text_w,
        .h = text_h
	};
	SDL_RenderCopy(
		game_window->sdl_renderer, 
		btn->sdl_text_texture, 
		NULL, 
		&text_rect
	);

	return 0;
}

// @TODO
int render_info_tag(InfoTag* info_tag) { 
	if (!info_tag) {
		printf(PRINT_WARNING "Could not render a null info tag widget\n");
		return 0;
	}

	return 0; 
}
