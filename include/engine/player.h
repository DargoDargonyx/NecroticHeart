/**
 * @file player.h
 * @author DargoDargonyx
 * @date 08/13/2026
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "util/helper.h"

#include <SDL2/SDL.h>


#define PLAYER_RENDERING_LAYER 3

typedef struct {
	IntSize pixel_size;
	FloatPos world_pos;

	SDL_Texture* sprite;
} Player;

int init_player(void);
void destroy_player(void);
Player* get_player(void);

#endif // PLAYER_H
