/**
 * @file widget.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "engine/widget.h"
#include "window/display.h"

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

WidgetCont* create_widget_cont() {
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

	btn->base.type = BUTTON;
	btn->state = BTN_IDLE;

	btn->sdl_rect.x = pos.x;
	btn->sdl_rect.y = pos.y;
	btn->sdl_rect.w = size.w;
	btn->sdl_rect.h = size.h;

	switch (type) {
		case LONG_TRANSPARENT_BTN:	
			btn->background = IMG_LoadTexture(
				game_window->sdl_renderer,
				"assets/ui/long_transparent_button.png"
			);
			break;
		default:
			printf(PRINT_ERROR "Could not identify the type of button to be ceated\n");
			break;
	}

	btn->text = text;
	btn->text_color = text_color;
	btn->font_num = font_num;

	return btn;
}

void destroy_button(Button* btn) {
	if (btn->background) {
		SDL_DestroyTexture(btn->background);
		btn->background = NULL;
	}

	free(btn);
}

// Info tag @TODO

InfoTag* create_info_tag(IntSize size, IntPos pos) {}

void destroy_info_tag(InfoTag* info_tag) {}
