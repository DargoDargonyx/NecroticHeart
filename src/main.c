/**
 * @file main.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#define GAME_NAME "NecroticHeart"

#include "util/file.h"
#include "util/config.h"
#include "util/font.h"
#include "window/display.h"
#include "engine/game.h"

#include <stdio.h>

int check_args(int argc, char** argv) {
    if (argc > 2) {
        printf(PRINT_WARNING "Usage: %s <print_configs: bool>\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    int err = check_args(argc, argv);
    err = seed_configs();
    if (err) return err;

    if (init_config()) return 1;
	ConfigCont* config = get_config();
    err = read_config(config);
    if (err) return err;

    if (argv[1] || config->dev_opts->print_configs) print_configs(config);

    SDL_Init(SDL_INIT_VIDEO);
    if (init_game_window(GAME_NAME)) {
		printf(PRINT_ERROR "Could not initialize game window\n");

        SDL_Quit();
		destroy_config();
        return 1;
    }

	TTF_Init();
	if (init_global_fonts()) {
		printf(PRINT_ERROR "Could not initialize global fonts\n");

		destroy_game_window();
		SDL_Quit();
		destroy_config();
		return 1;
	}

	if (init_scenes()) {
		printf(PRINT_ERROR "Could not initialize game scenes\n");

		destroy_global_fonts();
		destroy_game_window();
		SDL_Quit();
		destroy_config();
		return 1;
	}

	err = run_game_loop();
   
	destroy_scenes();
	destroy_global_fonts();
	destroy_game_window();
    SDL_Quit();
    destroy_config();
    
	return err;
}
