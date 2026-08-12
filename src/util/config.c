/**
 * @file config.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "util/config.h"
#include "util/file.h"

#include <stdio.h>
#include <stdlib.h>


static ConfigManager* config_manager = NULL;

// General Config

int init_config_manager(void) {
	if (config_manager) {
		printf(PRINT_WARNING "Attempting to initialize the config manager when " 
				"one has already been initialized\n");
		return 0;
	}
	config_manager = malloc(sizeof(ConfigManager));

	config_manager->dev_opts = init_dev_opts_config();
	if (!config_manager->dev_opts) 
		printf(PRINT_WARNING "Loaded an empty dev opts config\n");

	config_manager->display = init_display_config();
	if (!config_manager->display)
		printf(PRINT_WARNING "Loaded an empty display config\n");

	config_manager->player_info = init_player_info_config();
	if (!config_manager->player_info)
		printf(PRINT_WARNING "Loaded an empty player info config\n");

	if (read_config())  {
		printf(PRINT_ERROR "Unable to read config files\n");
		return 1;
	}

	return 0;
}

void destroy_config_manager(void) {
	if (config_manager->dev_opts) destroy_dev_opts_config(config_manager->dev_opts);
	if (config_manager->display) destroy_display_config(config_manager->display);
	if (config_manager->player_info) destroy_player_info_config(config_manager->player_info);

	free(config_manager);
	config_manager = NULL;
}

ConfigManager* get_config_manager(void) { return config_manager; }

// Dev opts config

DevOptsConfig* init_dev_opts_config(void) {
	DevOptsConfig* dev_opts = malloc(sizeof(DevOptsConfig));
	return dev_opts;
}

void destroy_dev_opts_config(DevOptsConfig* dev_opts) {
	free(dev_opts);
}

int print_configs(void) {
	if (!config_manager->player_info) {
		printf(PRINT_ERROR "Unable to print configs when the config manager is null\n");
		return 1;
	}

	if (config_manager->dev_opts) {
		printf("============ Dev Opts Config ===========\n"
			   "print_configs: %s\n"
			   "========================================\n",
			   config_manager->dev_opts->print_configs ? "enabled" : "disabled");
	} else {
		printf(PRINT_WARNING "Unable to print dev opts config\n");
	}

	if (config_manager->display) {
		printf("============ Display Config ============\n"
			   "Fullscreen: %s\n"
			   "Resolution: %dx%d\n"
			   "========================================\n",
			   config_manager->display->fullscreen ? "enabled" : "disabled",
			   config_manager->display->resolution.w,
			   config_manager->display->resolution.h);
	} else {
		printf(PRINT_WARNING "Unable to print display config\n");
	}

	if (config_manager->player_info) {
		printf("========== Player Info Config ==========\n"
			   "Username: %s\n"
			   "Language: %s\n"
			   "========================================\n",
			   config_manager->player_info->username, 
			   config_manager->player_info->language);
	} else {
		printf(PRINT_WARNING "Unable to print player info config\n");
	}

	return 0;
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
	if (!player_info) {
		printf(PRINT_WARNING "Attempting to destroy a null player info config\n");
		return;
	}

	if (player_info->username) free(player_info->username);
	else printf(PRINT_WARNING "Attempting to free a null player info config field\n");

	if (player_info->language) free(player_info->language);
	else printf(PRINT_WARNING "Attempting to free a null player info config field\n");

	free(player_info);
}
