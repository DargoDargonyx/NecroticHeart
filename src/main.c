/**
 * @file main.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "engine/game.h"
#include "util/config.h"
#include "util/helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int check_args(int argc, char** argv) {
    if (argc > 2) {
        printf(PRINT_WARNING "Usage: %s <print_configs: bool>\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (check_args(argc, argv)) return 1;
   
	if (init_game()) {
		printf(PRINT_ERROR "Could not initialize the game\n");
		quit_game(EXIT_SUCCESS);
	}

	// Seed random generator
	srand((unsigned int) time(NULL));

	// Handle config information
	ConfigManager* config_manager = get_config_manager();
    if (argv[1] || config_manager->dev_opts->print_configs) print_configs();

	// Start game
	if (run_game_loop()) printf(PRINT_WARNING "Game loop ended early\n");


	return 0;
}
