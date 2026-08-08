/**
 * @file config.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "util/config.h"

#include <stdio.h>
#include <stdlib.h>

// General Config

ConfigCont* init_config() {
    ConfigCont* config_cont = malloc(sizeof(ConfigCont));

    config_cont->dev_opts = init_dev_opts_config();
    if (!config_cont->dev_opts) printf("ERROR: Loaded an empty dev opts config\n");

    config_cont->player_info = init_player_info_config();
    if (!config_cont->player_info) printf("ERROR: Loaded an empty player info config\n");

    return config_cont;
}

int destroy_config(ConfigCont* config_cont) {
    int err;

    err = destroy_dev_opts_config(config_cont->dev_opts);
    if (err) return err;
    err = destroy_player_info_config(config_cont->player_info);
    if (err) return err;

    free(config_cont);
    return err;
}

// Dev opts config

DevOptsConfig* init_dev_opts_config() {
    DevOptsConfig* dev_opts = malloc(sizeof(DevOptsConfig));
    return dev_opts;
}

int destroy_dev_opts_config(DevOptsConfig* dev_opts) {
    free(dev_opts);
    return 0;
}

void print_configs(ConfigCont* config_cont) {
    printf("========== Player Info Config ==========\n"
           "Username: %s\n"
           "Language: %s\n"
           "========================================\n",
           config_cont->player_info->username, config_cont->player_info->language);
}

// Player info config

PlayerInfoConfig* init_player_info_config() {
    PlayerInfoConfig* player_info = malloc(sizeof(PlayerInfoConfig));
    player_info->username = NULL;
    player_info->language = NULL;

    return player_info;
}

int destroy_player_info_config(PlayerInfoConfig* player_info) {
    free(player_info->username);
    free(player_info->language);
    free(player_info);

    return 0;
}
