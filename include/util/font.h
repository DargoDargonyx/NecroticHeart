/**
 * @file font.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL_ttf.h>

#define GLOBAL_FONT_COUNT 1
#define GLOBAL_FONT_START_MENU_BTN 0

typedef struct {
	TTF_Font** fonts;
} GlobalFonts;

int init_global_fonts();
void destroy_global_fonts();
GlobalFonts* get_global_fonts();

#endif // FONT_H
