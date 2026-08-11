/**
 * @file config.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "util/config.h"

#include <stdio.h>
#include <stdlib.h>


static ConfigCont* config_cont = NULL;

// General Config

int init_config(void) {
	if (config_cont) return 1;
    config_cont = malloc(sizeof(ConfigCont));

    config_cont->dev_opts = init_dev_opts_config();
    if (!config_cont->dev_opts) {
		printf(PRINT_WARNING "Loaded an empty dev opts config\n");
		return 1;
	}

    config_cont->display = init_display_config();
    if (!config_cont->display) {
		printf(PRINT_WARNING "Loaded an empty display config\n");
		return 1;
	}

    config_cont->player_info = init_player_info_config();
    if (!config_cont->player_info) {
		printf(PRINT_WARNING "Loaded an empty player info config\n");
		return 1;
	}

	return 0;
}

void destroy_config(void) {
    destroy_dev_opts_config(config_cont->dev_opts);
    destroy_display_config(config_cont->display);
    destroy_player_info_config(config_cont->player_info);

    free(config_cont);
	config_cont = NULL;
}

ConfigCont* get_config(void) {
	return config_cont;
}

// Dev opts config

DevOptsConfig* init_dev_opts_config(void) {
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

DisplayConfig* init_display_config(void) {
	DisplayConfig* display = malloc(sizeof(DisplayConfig));
	return display;
}

void destroy_display_config(DisplayConfig* display) {
	free(display);
}

// Player info config

PlayerInfoConfig* init_player_info_config(void) {
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
