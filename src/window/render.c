/**
 * @file render.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "window/render.h"
#include "window/display.h"


int render_current_scene() {
	Scene* current_scene = get_current_scene();
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
int render_info_tag(InfoTag* info_tag) { return 0; }
