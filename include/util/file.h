/**
 * @file file.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#ifndef FILE_H

#include "external/cJSON.h"
#include "util/config.h"

#define DEV_OPTS_CONFIG_PATH "../data/config/dev_opts.json"
#define PLAYER_INFO_CONFIG_PATH "../data/config/player_info.json"

// Helper functions
int file_exists(const char*);
char* read_json(const char*);
int write_json(cJSON*, const char*);

// General config
int read_config(ConfigCont*);
int write_config(ConfigCont*);

// Dev opts config
int seed_dev_opts_config();
int read_dev_opts_config(DevOptsConfig*);
int write_dev_opts_config(DevOptsConfig*);

// Player info config
int seed_player_info_config();
int read_player_info_config(PlayerInfoConfig*);
int write_player_info_config(PlayerInfoConfig*);

#endif // FILE_H
