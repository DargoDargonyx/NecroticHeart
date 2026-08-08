/**
 * @file config.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "util/helper.h"

typedef enum { ENGLISH } PLAYER_LANG_OPTS;

typedef struct {
    int print_configs;
} DevOptsConfig;

typedef struct {
	int fullscreen;
	IntSize resolution;
} DisplayConfig;

typedef struct {
    char* username;
    char* language;
} PlayerInfoConfig;

typedef struct {
    DevOptsConfig* dev_opts;
	DisplayConfig* display;
    PlayerInfoConfig* player_info;
} ConfigCont;

// General config
int init_config();
void destroy_config();
ConfigCont* get_config();

// Dev opts config
DevOptsConfig* init_dev_opts_config();
void destroy_dev_opts_config(DevOptsConfig*);
void print_configs(ConfigCont*);

// Display config
DisplayConfig* init_display_config();
void destroy_display_config(DisplayConfig*);

// Player info config
PlayerInfoConfig* init_player_info_config();
void destroy_player_info_config(PlayerInfoConfig*);

#endif // CONFIG_H
