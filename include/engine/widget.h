/**
 * @file widget.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef WIDGET_H
#define WIDGET_H

#include "util/helper.h"

#include <SDL2/SDL.h>

// General widgets

#define INIT_WIDGET_CONT_CAP 31

typedef enum { BUTTON, INFO_TAG } WidgetType;

typedef struct {
	WidgetType type;
} Widget;

void destroy_widget(Widget*);

typedef struct {
	Widget** widgets;
	int count;
	int cap;
} WidgetCont;

WidgetCont* create_widget_cont();
void destroy_widget_cont(WidgetCont*);
int update_widgets(WidgetCont*, float);

int add_widget_to_cont(WidgetCont*, Widget*);
int remove_widget_from_cont(WidgetCont*, Widget*);

// Buttons

#define BTN_PIXEL_SCALE_OFFSET 2

typedef enum { LONG_TRANSPARENT_BTN } ButtonType;
typedef enum { BTN_IDLE, BTN_HOVER, BTN_PRESSED } ButtonState;

typedef struct {
	Widget base;
	ButtonState state;
	SDL_Rect sdl_rect;

	const char* text;
	SDL_Color text_color;
	int font_num;	
	
	SDL_Texture* sdl_background_texture;
	SDL_Texture* sdl_text_texture;
	
	float press_offset;
	float press_animation_speed;
} Button;

Button* create_button(ButtonType, IntSize, IntPos, const char*, SDL_Color, int);
void destroy_button(Button*);
int update_button(Button*, float);

// Info tag

typedef struct {
	Widget base;
	SDL_Rect sdl_rect;
} InfoTag;

InfoTag* create_info_tag(IntSize, IntPos);
void destroy_info_tag(InfoTag*);

#endif // WIDGET_H
