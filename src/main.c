/**
 * @file main.c
 * @author DargoDargonyx
 * @date 07/18/2026
 */

#define GAME_NAME "NecroticHeart"

#include "util/file.h"
#include <stdio.h>

int main(int argc, char** argv) {
    int error = 0;

    error = read_config();
    if (!error) printf("Compiled project successfully!\n");
    return error;
}
