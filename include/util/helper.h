/**
 * @file helper.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#ifndef HELPER_H
#define HELPER_H

#include "util/ansi.h"


// Helper ANSI print output macros

#define PRINT_ERROR ANSI_BOLD ANSI_RED "[ERROR] " ANSI_RESET
#define PRINT_WARNING ANSI_BOLD ANSI_YELLOW "[WARNING] " ANSI_RESET
#define PRINT_INFO ANSI_BOLD ANSI_CYAN "[INFO]" ANSI_RESET

// Integer size

typedef struct {
    int w;
    int h;
} IntSize;

static inline IntSize add_int_w_to_size(IntSize size, int n) { 
	return (IntSize) { .w = size.w + n, .h = size.h };
}

static inline IntSize add_int_h_to_size(IntSize size, int n) {
	return (IntSize) { .w = size.w, .h = size.h + n };
}

// Float size

typedef struct {
    float w;
    float h;
} FloatSize;

static inline FloatSize add_float_w_to_size(FloatSize size, float f) {
	return (FloatSize) { .w = size.w + f, .h = size.h };
}

static inline FloatSize add_float_h_to_size(FloatSize size, float f) {
	return (FloatSize) { .w = size.w, .h = size.h + f };
}

// Integer pos

typedef struct {
    int x;
    int y;
} IntPos;

static inline IntPos add_int_x_to_pos(IntPos pos, int n) { 
	return (IntPos) { .x = pos.x + n, .y = pos.y };
}

static inline IntPos add_int_y_to_pos(IntPos pos, int n) {
	return (IntPos) { .x = pos.x, .y = pos.y + n };
}

// Float pos

typedef struct {
    float x;
    float y;
} FloatPos;

static inline FloatPos add_float_x_to_pos(FloatPos pos, float f) {
	return (FloatPos) { .x = pos.x + f, .y = pos.y };
}

static inline FloatPos add_float_y_to_pos(FloatPos pos, float f) {
	return (FloatPos) { .x = pos.x, .y = pos.y + f };
}

// Float pos 3D

typedef struct {
    float x;
    float y;
    float z;
} FloatPos3;

static inline FloatPos3 add_float_x_to_pos3(FloatPos3 pos, float f) {
	return (FloatPos3) { .x = pos.x + f, .y = pos.y, .z = pos.z };
}

static inline FloatPos3 add_float_y_to_pos3(FloatPos3 pos, float f) {
	return (FloatPos3) { .x = pos.x, .y = pos.y + f, .z = pos.z };
}

static inline FloatPos3 add_float_z_to_pos3(FloatPos3 pos, float f) {
	return (FloatPos3) { .x = pos.x, .y = pos.y, .z = pos.z + f };
}


#endif // HELPER_H
