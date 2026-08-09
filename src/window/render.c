/**
 * @file render.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "window/render.h"
#include "window/display.h"
#include "util/font.h"


int render_current_scene() {
	GameWindow* game_window = get_game_window();
	Scene* current_scene;

	switch (game_window->current_scene) {
		case START_MENU_SCENE:
			current_scene = get_start_menu_scene();
			break;
		case SETTINGS_MENU_SCENE:
			// @TODO
			break;
		case PLAY_SCENE:
			// @TODO
			break;
		case INVENTORY_SCENE:
			// @TODO
			break;
		default:
			printf(PRINT_ERROR "Could not render a scene of unknown type\n");
			return 1;
	}

	if (render_widgets(current_scene->widget_cont)) return 1;
	return 0;
}

int render_widgets(WidgetCont* cont) {
	if (!cont) {
		printf(PRINT_WARNING "Could not render widgets for an empty widget container\n");
		return 1;
	}

	for (int i = 0; i < cont->count; i++) {
		switch (cont->widgets[i]->type) {
			case BUTTON:
				render_button((Button*) cont->widgets[i]);
				break;
			case INFO_TAG:
				render_info_tag((InfoTag*) cont->widgets[i]);
				break;
			default:
				printf(PRINT_ERROR "Could not render a widget of unknown type\n");
				return 1;
		}
	}
	return 0;
}

int render_button(Button* btn) {
	GameWindow* game_window = get_game_window();
	GlobalFonts* global_fonts = get_global_fonts();

	SDL_RenderCopy(
		game_window->sdl_renderer, 
		btn->background, 
		NULL, 
		&btn->sdl_rect
	);

	SDL_Surface* surface = TTF_RenderUTF8_Blended(
		global_fonts->fonts[btn->font_num], 
		btn->text, 
		btn->text_color
	);

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(
		game_window->sdl_renderer, 
		surface
	);

	if (!text_texture) { 
		printf(PRINT_ERROR "Button text texture loading failed\n");
		return 1;
	}


	int text_w;
	int text_h;
	SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
	SDL_Rect text_rect = {
		.x = btn->sdl_rect.x + (btn->sdl_rect.w - text_w) / 2,
        .y = btn->sdl_rect.y + (btn->sdl_rect.h - text_h) / 2,
        .w = text_w,
        .h = text_h
	};

	SDL_RenderCopy(game_window->sdl_renderer, text_texture, NULL, &text_rect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(surface);
	return 0;
}

// @TODO
int render_info_tag(InfoTag* info_tag) { return 0; }
