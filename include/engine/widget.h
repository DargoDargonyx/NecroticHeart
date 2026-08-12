/**
 * @file widget.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef WIDGET_H
#define WIDGET_H

#include "util/helper.h"

#include <SDL2/SDL.h>


// Helper functions
int is_mouse_over_screen_pixels(SDL_Rect);

// General widgets
#define INIT_WIDGET_CONT_CAP 31

typedef enum { BUTTON, INFO_TAG } WidgetType;
typedef struct {
	WidgetType type;
} Widget;

void destroy_widget(Widget*);

// Widget container
typedef struct {
	Widget** widgets;
	int count;
	int cap;
} WidgetCont;

WidgetCont* create_widget_cont(void);
void destroy_widget_cont(WidgetCont*);
int add_widget_to_cont(WidgetCont*, Widget*);
int remove_widget_from_cont(WidgetCont*, Widget*);

// Collective widgets
int update_widgets(WidgetCont*, float);
void handle_widget_events(WidgetCont*, SDL_Event*);

// Buttons
#define BTN_PIXEL_SCALE_OFFSET 2

typedef enum { LONG_TRANSPARENT_BTN } ButtonType;
typedef enum { BTN_IDLE, BTN_HOVER, BTN_PRESSED } ButtonState;
typedef void (*ButtonCallback)(void);
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

	ButtonCallback on_click;
} Button;

Button* create_button(ButtonType, IntSize, IntPos, const char*, 
		SDL_Color, int, float, float, ButtonCallback);
void destroy_button(Button*);
int update_button(Button*, float);
void handle_button_event(Button*, SDL_Event*);

// Info tag
typedef struct {
	Widget base;
	SDL_Rect sdl_rect;
} InfoTag;

InfoTag* create_info_tag(IntSize, IntPos);
void destroy_info_tag(InfoTag*);
int update_info_tag(InfoTag*, float);

#endif // WIDGET_H
