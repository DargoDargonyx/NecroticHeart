/**
 * @file file.c
 * @author DargoDargonyx
 * @date 07/19/2026
 */

#include "util/file.h"
#include "external/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

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

int read_config() {
    char* player_info_json_str = read_json(PLAYER_INFO_PATH);
    if (!player_info_json_str) return 1;

    cJSON* player_info_json = cJSON_Parse(player_info_json_str);
    free(player_info_json_str);
    if (!player_info_json) {
        printf("ERROR: Could not parse player information JSON file\n");
        return 1;
    }

    cJSON* username = cJSON_GetObjectItemCaseSensitive(player_info_json, "Username");
    cJSON* language = cJSON_GetObjectItemCaseSensitive(player_info_json, "Language");

    if (cJSON_IsString(username)) {
        printf("Config Username was read as {%s}\n", username->valuestring);
    } else {
        printf("ERROR: Could not find the player username in config file\n");
        return 1;
    }

    if (cJSON_IsString(language)) {
        printf("Config Language was read as {%s}\n", language->valuestring);
    } else {
        printf("ERROR: Could not find the player language in config file\n");
        return 1;
    }

    cJSON_Delete(player_info_json);
    return 0;
}

int write_config(const char* filename) { return 0; }
