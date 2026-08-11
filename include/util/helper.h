/**
 * @file helper.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef HELPER_H
#define HELPER_H

#include "util/ansi.h"


// Helper ANSI print output macros

#define PRINT_ERROR ANSI_BOLD ANSI_RED "[ERROR] " ANSI_RESET
#define PRINT_WARNING ANSI_BOLD ANSI_YELLOW "[WARNING] " ANSI_RESET
#define PRINT_INFO ANSI_BOLD ANSI_CYAN "[INFO]" ANSI_RESET

// Size and position structs

typedef struct {
    int w;
    int h;
} IntSize;

typedef struct {
    float w;
    float h;
} FloatSize;

typedef struct {
    int x;
    int y;
} IntPos;

typedef struct {
    float x;
    float y;
} FloatPos;

typedef struct {
    float x;
    float y;
    float z;
} FloatPos3;

#endif // HELPER_H
