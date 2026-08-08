/**
 * @file config.h
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#ifndef CONFIG_H

typedef enum { ENGLISH } PLAYER_LANG_OPTS;

typedef struct {
    int print_configs;
} DevOptsConfig;

typedef struct {
    char* username;
    char* language;
} PlayerInfoConfig;

typedef struct {
    DevOptsConfig* dev_opts;
    PlayerInfoConfig* player_info;
} ConfigCont;

// General config
int seed_configs();
ConfigCont* init_config();
int destroy_config(ConfigCont*);

// Dev opts config
DevOptsConfig* init_dev_opts_config();
int destroy_dev_opts_config(DevOptsConfig*);
void print_configs(ConfigCont*);

// Player info config
PlayerInfoConfig* init_player_info_config();
int destroy_player_info_config(PlayerInfoConfig*);

#endif // CONFIG_H
