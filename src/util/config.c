/**
 * @file config.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "util/config.h"

#include <stdio.h>
#include <stdlib.h>

static ConfigCont* config = NULL;

// General Config

int init_config() {
	if (config) return 1;

    config = malloc(sizeof(ConfigCont));

    config->dev_opts = init_dev_opts_config();
    if (!config->dev_opts) {
		printf(PRINT_WARNING "Loaded an empty dev opts config\n");
		return 1;
	}

    config->display = init_display_config();
    if (!config->display) {
		printf(PRINT_WARNING "ERROR: Loaded an empty display config\n");
		return 1;
	}

    config->player_info = init_player_info_config();
    if (!config->player_info) {
		printf(PRINT_WARNING "ERROR: Loaded an empty player info config\n");
		return 1;
	}

	return 0;
}

void destroy_config() {
    destroy_dev_opts_config(config->dev_opts);
    destroy_display_config(config->display);
    destroy_player_info_config(config->player_info);

    free(config);
	config = NULL;
}

ConfigCont* get_config() {
	return config;
}

// Dev opts config

DevOptsConfig* init_dev_opts_config() {
    DevOptsConfig* dev_opts = malloc(sizeof(DevOptsConfig));
    return dev_opts;
}

void destroy_dev_opts_config(DevOptsConfig* dev_opts) {
    free(dev_opts);
}

void print_configs(ConfigCont* config_cont) {
    printf("========== Player Info Config ==========\n"
           "Username: %s\n"
           "Language: %s\n"
           "========================================\n",
           config_cont->player_info->username, config_cont->player_info->language);
}

// Display config

DisplayConfig* init_display_config() {
	DisplayConfig* display = malloc(sizeof(DisplayConfig));
	return display;
}

void destroy_display_config(DisplayConfig* display) {
	free(display);
}

// Player info config

PlayerInfoConfig* init_player_info_config() {
    PlayerInfoConfig* player_info = malloc(sizeof(PlayerInfoConfig));
    player_info->username = NULL;
    player_info->language = NULL;

    return player_info;
}

void destroy_player_info_config(PlayerInfoConfig* player_info) {
    free(player_info->username);
    free(player_info->language);
    free(player_info);
}
