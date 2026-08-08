/**
 * @file main.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#define GAME_NAME "NecroticHeart"

#include "util/file.h"
#include <stdio.h>

int check_args(int argc, char** argv) {
    if (argc > 2) {
        printf("Usage: %s <print_configs: bool>\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    int err = check_args(argc, argv);
    err = seed_configs();
    if (err) return err;

    ConfigCont* config_cont = init_config();
    err = read_config(config_cont);
    if (err) return err;

    if (argv[1] || config_cont->dev_opts->print_configs) print_configs(config_cont);

    err = destroy_config(config_cont);
    return err;
}
