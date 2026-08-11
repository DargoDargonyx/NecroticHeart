/**
 * @file font.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "util/font.h"
#include "util/helper.h"


static GlobalFonts* global_fonts = NULL;

int init_global_fonts(void) {
	if (global_fonts) return 1;

	global_fonts = malloc(sizeof(GlobalFonts));
	global_fonts->fonts = calloc(GLOBAL_FONT_COUNT, sizeof(TTF_Font*));
	global_fonts->fonts[GLOBAL_FONT_START_MENU_BTN] = TTF_OpenFont(
		"assets/fonts/JetBrainsMono-Regular.ttf", 
		48
	);

	if (!global_fonts->fonts[GLOBAL_FONT_START_MENU_BTN]) {
		printf(PRINT_ERROR "Could not load the start menu button font\n");
		return 1;
	}

	return 0;
}

void destroy_global_fonts(void) {
	if (!global_fonts) {
		printf(PRINT_WARNING "Could not destroy a null global font struct pointer\n");
		return;
	}

	for (int i = 0; i < GLOBAL_FONT_COUNT; i++) TTF_CloseFont(global_fonts->fonts[i]);
	free(global_fonts->fonts);
	free(global_fonts);
	global_fonts = NULL;
}

GlobalFonts* get_global_fonts() { return global_fonts; }
