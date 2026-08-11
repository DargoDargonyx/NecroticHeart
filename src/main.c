/**
 * @file main.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "util/file.h"
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
    if (check_args(argc, argv)) return 1;
    
	// Config files
	if (seed_configs()) return 1;
    if (init_config()) return 1;
	ConfigCont* config = get_config();
	if (read_config(config)) {
		printf(PRINT_ERROR "Could not read configuration files\n");
		goto end;
	}

    if (argv[1] || config->dev_opts->print_configs) print_configs(config);

	// Game
	if (init_game()) {
		printf(PRINT_ERROR "Could not initialize the game\n");
		goto end;
	}
	if (run_game_loop()) printf(PRINT_WARNING "Game loop ended early\n");

end:
	quit_game();
    destroy_config();
	return 0;
}
