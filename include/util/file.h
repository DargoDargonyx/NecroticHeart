/**
 * @file file.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef FILE_H
#define FILE_H

#include "external/cJSON.h"


// File paths
#define DEV_OPTS_CONFIG_PATH "../data/config/dev_opts.json"
#define DISPLAY_CONFIG_PATH "../data/config/display.json"
#define PLAYER_INFO_CONFIG_PATH "../data/config/player_info.json"

// Helper functions
int file_exists(const char*);
char* read_json(const char*);
int write_json(cJSON*, const char*);

// General config
int seed_configs(void);
int read_config(void);
int write_config(void);

// Dev opts config
int seed_dev_opts_config(void);
int read_dev_opts_config(void);
int write_dev_opts_config(void);

// Display config
int seed_display_config(void);
int read_display_config(void);
int write_display_config(void);

// Player info config
int seed_player_info_config(void);
int read_player_info_config(void);
int write_player_info_config(void);

// Tile definitions
cJSON* load_tile_definition(const char*);


#endif // FILE_H
