/**
 * @file file.c
 * @author DargoDargonyx
 * @date 08/08/2026
 */

#include "util/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper functions

int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

char* read_json(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("ERROR: Could not open file {%s}\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) goto end;

    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';
end:
    fclose(file);
    return buffer;
}

int write_json(cJSON* json, const char* filename) {
    FILE* file = fopen(filename, "w");

    char* json_string = cJSON_Print(json);

    if (json_string == NULL) {
        printf("ERROR: Could not write an empty json object to a file\n");
        return 1;
    }

    if (file == NULL) {
        printf("ERROR: Could not write to the file {%s}\n", filename);
        free(json_string);
        return 1;
    }

    fputs(json_string, file);
    fclose(file);

    free(json_string);
    return 0;
}

// General config

int seed_configs() {
    int err;

    err = seed_dev_opts_config();
    if (err) return err;
    err = seed_player_info_config();

    return err;
}

int read_config(ConfigCont* config_cont) {
    int err;

    err = read_dev_opts_config(config_cont->dev_opts);
    if (err) return err;
    err = read_player_info_config(config_cont->player_info);

    return err;
}

int write_config(ConfigCont* config_cont) {
    int err;

    err = write_dev_opts_config(config_cont->dev_opts);
    if (err) return err;
    err = write_player_info_config(config_cont->player_info);

    return err;
}

// Dev opts config

int seed_dev_opts_config() {
    if (file_exists(DEV_OPTS_CONFIG_PATH)) return 0;

    cJSON* json = cJSON_CreateObject();
    cJSON_AddBoolToObject(json, "print_configs", 0);

    int err = write_json(json, DEV_OPTS_CONFIG_PATH);
    cJSON_Delete(json);
    return err;
}

int read_dev_opts_config(DevOptsConfig* dev_opts) {
    char* json_str = read_json(DEV_OPTS_CONFIG_PATH);
    if (!json_str) return 1;

    cJSON* json = cJSON_Parse(json_str);
    free(json_str);
    if (!json) {
        printf("ERROR: Could not parse the json file {%s}\n", DEV_OPTS_CONFIG_PATH);
        return 1;
    }

    cJSON* print_configs = cJSON_GetObjectItemCaseSensitive(json, "print_configs");

    if (cJSON_IsBool(print_configs)) {
        dev_opts->print_configs = cJSON_IsTrue(print_configs);
    } else {
        printf("ERROR: Could not find a \"Username\" field in the json file {%s}\n",
               PLAYER_INFO_CONFIG_PATH);
        cJSON_Delete(json);
        return 1;
    }

    cJSON_Delete(json);
    return 0;
}

int write_dev_opts_config(DevOptsConfig* dev_opts) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddBoolToObject(json, "print_configs", dev_opts->print_configs);

    int err = write_json(json, DEV_OPTS_CONFIG_PATH);
    cJSON_Delete(json);
    return err;
}

// Player info config

int seed_player_info_config() {
    if (file_exists(PLAYER_INFO_CONFIG_PATH)) return 0;

    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "Username", "Default");
    cJSON_AddStringToObject(json, "Language", "Default");

    int err = write_json(json, PLAYER_INFO_CONFIG_PATH);
    cJSON_Delete(json);
    return err;
}

int read_player_info_config(PlayerInfoConfig* player_info) {
    char* json_str = read_json(PLAYER_INFO_CONFIG_PATH);
    if (!json_str) return 1;

    cJSON* json = cJSON_Parse(json_str);
    free(json_str);
    if (!json) {
        printf("ERROR: Could not parse the json file {%s}\n", PLAYER_INFO_CONFIG_PATH);
        return 1;
    }

    cJSON* username = cJSON_GetObjectItemCaseSensitive(json, "Username");
    cJSON* language = cJSON_GetObjectItemCaseSensitive(json, "Language");

    if (cJSON_IsString(username)) {
        player_info->username = strdup(username->valuestring);
    } else {
        printf("ERROR: Could not find a \"Username\" field in the json file {%s}\n",
               PLAYER_INFO_CONFIG_PATH);
        cJSON_Delete(json);
        return 1;
    }

    if (cJSON_IsString(language)) {
        player_info->language = strdup(language->valuestring);
    } else {
        printf("ERROR: Could not find a \"Language\" field in the json file {%s}\n",
               PLAYER_INFO_CONFIG_PATH);
        cJSON_Delete(json);
        return 1;
    }

    cJSON_Delete(json);
    return 0;
}

int write_player_info_config(PlayerInfoConfig* player_info) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "Username", player_info->username);
    cJSON_AddStringToObject(json, "Language", player_info->language);

    int err = write_json(json, PLAYER_INFO_CONFIG_PATH);
    cJSON_Delete(json);
    return err;
}
