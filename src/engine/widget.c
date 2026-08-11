/**
 * @file widget.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "engine/widget.h"
#include "window/display.h"
#include "util/font.h"

#include <SDL2/SDL_image.h>
#include <stdlib.h>


// General widgets

void destroy_widget(Widget* widget) {
	switch (widget->type) {
		case BUTTON:
			destroy_button((Button*) widget);
			break;
		case INFO_TAG:
			destroy_info_tag((InfoTag*) widget);
			break;
		default:
			break;
	}
}

WidgetCont* create_widget_cont(void) {
	WidgetCont* cont = malloc(sizeof(WidgetCont));
	cont->widgets = calloc(INIT_WIDGET_CONT_CAP, sizeof(Widget*));
	cont->count = 0;
	cont->cap = INIT_WIDGET_CONT_CAP;

	return cont;
}

void destroy_widget_cont(WidgetCont* cont) {
	if (!cont) return;

	for (int i = 0; i < cont->count; i++) {
		destroy_widget(cont->widgets[i]);
		cont->widgets[i] = NULL;
	}
	free(cont->widgets);
	free(cont);
}

int update_widgets(WidgetCont* cont, float dt) {
	int err = 0;

	for (int i = 0; i < cont->count; i++) {
		switch (cont->widgets[i]->type) {
			case BUTTON:
				err = update_button((Button*) cont->widgets[i], dt);
				break;
			case INFO_TAG:
				// @TODO
				break;
			default:
				printf(PRINT_ERROR "Could not update a widget with an unknown type\n");
				return 1;
		}
	}

	return err;
}

int add_widget_to_cont(WidgetCont* cont, Widget* widget) {
	if (!cont) {
		printf(PRINT_ERROR "Could not add a widget to an empty widget container\n");
		return 1;
	}
	if (!widget) {
		printf(PRINT_ERROR "Could not add a null widget to a widget container\n");
		return 1;
	}

	if (cont->count == cont->cap) {
		WidgetCont* temp = malloc(sizeof(WidgetCont));
		temp->widgets = calloc(cont->cap * 2 + 1, sizeof(Widget*));
		temp->cap = cont->cap * 2 + 1;
		temp->count = cont->count;
		
		for (int i = 0; cont->cap; i++) temp->widgets[i] = cont->widgets[i];
		destroy_widget_cont(cont);
		cont = temp;
	}

	cont->widgets[cont->count++] = widget;
	return 0;
}

int remove_widget_from_cont(WidgetCont* cont, Widget* widget) {
	if (!cont) {
		printf(PRINT_ERROR "Could not remove a widget from an empty widget container\n");
		return 1;
	}
	if (!widget) {
		printf(PRINT_ERROR "Could not remove a null widget from a widget container\n");
		return 1;
	}
	
	if (cont->widgets[cont->count - 1] == widget) {
		cont->widgets[cont->count--] = NULL;
	}

	int removed = 0;
	for (int i = 0; i < cont->count; i++) {
		if (cont->widgets[i] == widget) removed = 1;
		if (removed) cont->widgets[i] = cont->widgets[i + 1];

		if (i == cont->count - 1 && !removed) {
			printf(PRINT_ERROR "Could not remove a widget from a widget container that"
					" doesn't contain it\n");
			return 1;
		}
	}

	return 0;
}

// Buttons

Button* create_button(ButtonType type, IntSize size, IntPos pos, 
		const char* text, SDL_Color text_color, int font_num) {
	
	Button* btn = malloc(sizeof(Button));
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not access the game window while creating a button\n");
		goto end;
	}
	GlobalFonts* global_fonts = get_global_fonts();
	if (!global_fonts) {
		printf(PRINT_ERROR "Could not access the global fonts while creating a button\n");
		goto end;
	}

	btn->base.type = BUTTON;
	btn->state = BTN_IDLE;

	btn->sdl_rect.x = pos.x;
	btn->sdl_rect.y = pos.y;
	btn->sdl_rect.w = size.w;
	btn->sdl_rect.h = size.h;

	// Grabbing the sprite asset to render
	switch (type) {
		case LONG_TRANSPARENT_BTN:	
			btn->sdl_background_texture = IMG_LoadTexture(
				game_window->sdl_renderer,
				"assets/ui/long_transparent_btn.png"
			);
			break;
		default:
			printf(PRINT_ERROR "Could not identify the type of button to be ceated\n");
			goto end;
			break;
	}

	btn->text = text;
	btn->text_color = text_color;
	btn->font_num = font_num;
	btn->press_offset = 10;
	btn->press_animation_speed = 80.0f;

	// SDL textures
	SDL_Surface* surface = TTF_RenderUTF8_Blended(
		global_fonts->fonts[btn->font_num], 
		btn->text, 
		btn->text_color
	);

	btn->sdl_text_texture = SDL_CreateTextureFromSurface(
		game_window->sdl_renderer, 
		surface
	);

	if (!btn->sdl_text_texture) printf(PRINT_ERROR "Button text texture loading failed\n");
	SDL_FreeSurface(surface);
end:
	return btn;
}

void destroy_button(Button* btn) {
	if (btn->sdl_background_texture) {
		SDL_DestroyTexture(btn->sdl_background_texture);
		btn->sdl_background_texture = NULL;
	}
	
	if (btn->sdl_text_texture) {
		SDL_DestroyTexture(btn->sdl_text_texture);
		btn->sdl_text_texture = NULL;
	}

	free(btn);
}

int update_button(Button* btn, float dt) {
	GameWindow* game_window = get_game_window();
	if (!game_window) {
		printf(PRINT_ERROR "Could not access the game window while updating a button\n");
		return 1;
	}

	// Hover detection
	int mouse_x, mouse_y;
    Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	float logical_mouse_x, logical_mouse_y;
	SDL_RenderWindowToLogical(
		game_window->sdl_renderer,
		mouse_x,
		mouse_y,
		&logical_mouse_x,
		&logical_mouse_y
	);

	int hovering = logical_mouse_x >= btn->sdl_rect.x 
		&& logical_mouse_x < btn->sdl_rect.x + btn->sdl_rect.w
		&& logical_mouse_y >= btn->sdl_rect.y
		&& logical_mouse_y < btn->sdl_rect.y + btn->sdl_rect.h;

    if (!hovering) btn->state = BTN_IDLE;
    else if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) btn->state = BTN_PRESSED;
    else btn->state = BTN_HOVER;

	// Button press animation
	float target_offset = btn->state == BTN_PRESSED ? 4.0f : 0.0f;

    if (btn->press_offset < target_offset) {
        btn->press_offset += btn->press_animation_speed * dt;
        if (btn->press_offset > target_offset) btn->press_offset = target_offset;
    } else if (btn->press_offset > target_offset) {
        btn->press_offset -= btn->press_animation_speed * dt;
        if (btn->press_offset < target_offset) btn->press_offset = target_offset;
    }

	return 0;
}

// Info tag @TODO

InfoTag* create_info_tag(IntSize size, IntPos pos) {}

void destroy_info_tag(InfoTag* info_tag) {}
