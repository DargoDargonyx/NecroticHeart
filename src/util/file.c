/**
 * @file file.c
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#include "util/file.h"
#include "util/helper.h"
#include "util/config.h"

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
		printf(PRINT_ERROR "Could not open file {%s}\n", filename);
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
		printf(PRINT_ERROR "Could not write an empty json object to a file\n");
		return 1;
	}

	if (file == NULL) {
		printf(PRINT_ERROR "Could not write to the file {%s}\n", filename);
		free(json_string);
		return 1;
	}

	fputs(json_string, file);
	fclose(file);

	free(json_string);
	return 0;
}

// General config

int seed_configs(void) {
	if (seed_dev_opts_config()) {
		printf(PRINT_ERROR "Could not seed the dev opts config file\n");
		return 1;
	}
	if (seed_display_config()) {
		printf(PRINT_ERROR "Could not seed the display config file\n");
		return 1;
	}
	if (seed_player_info_config()) {
		printf(PRINT_ERROR "Could not seed the player info config file\n");	
		return 1;
	}

	return 0;
}

int read_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not read the config files with a null config manager\n");
		return 1;
	}

	if (read_dev_opts_config()) {
		printf(PRINT_ERROR "Could not read the dev opts config file\n");
		return 1;
	}
	if (read_display_config()) {
		printf(PRINT_ERROR "Could not read the display config file\n");
		return 1;
	}
	if (read_player_info_config()) {
		printf(PRINT_ERROR "Could not read the player info config\n");
		return 1;
	}

	return 0;
}

int write_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (config_manager) {
		printf(PRINT_ERROR "Could not write to the config files with a null config manager\n");
		return 1;
	}

	if (write_dev_opts_config()) {
		printf(PRINT_ERROR "Could not write to the dev opts config file\n");
		return 1;
	}
	if (write_display_config()) {
		printf(PRINT_ERROR "Could not write to the display config file\n");
		return 1;
	}
	if (write_player_info_config()) {
		printf(PRINT_ERROR "Could not write to the player info config file\n");
		return 1;
	}

	return 0;
}

// Dev opts config

int seed_dev_opts_config(void) {
	if (file_exists(DEV_OPTS_CONFIG_PATH)) return 0;

	cJSON* json = cJSON_CreateObject();
	cJSON_AddBoolToObject(json, "print_configs", 0);

	int err = write_json(json, DEV_OPTS_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

int read_dev_opts_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not read the dev opts config file with a null " 
				"config manager\n");
		return 1;
	}
	if (!config_manager->dev_opts) {
		printf(PRINT_ERROR "Could not read the dev opts config file with a null "
				"dev opts field in the config manager\n");
		return 1;
	}

	char* json_str = read_json(DEV_OPTS_CONFIG_PATH);
	if (!json_str) return 1;

	cJSON* json = cJSON_Parse(json_str);
	free(json_str);
	if (!json) {
		printf(PRINT_ERROR "Could not parse the json file {%s}\n", DEV_OPTS_CONFIG_PATH);
		return 1;
	}

	cJSON* print_configs = cJSON_GetObjectItemCaseSensitive(json, "print_configs");

	if (cJSON_IsBool(print_configs)) {
		config_manager->dev_opts->print_configs = print_configs->valueint;
	} else {
		printf(PRINT_ERROR "Could not find a proper \"print_configs\" field in " 
				"the json file {%s}\n", PLAYER_INFO_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	cJSON_Delete(json);
	return 0;
}

int write_dev_opts_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not write to the dev opts config file with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->dev_opts) {
		printf(PRINT_ERROR "Could not write to the dev opts config file with a null "
				"dev opts field in the config manager\n");
		return 1;
	}

	cJSON* json = cJSON_CreateObject();
	cJSON_AddBoolToObject(json, "print_configs", config_manager->dev_opts->print_configs);

	int err = write_json(json, DEV_OPTS_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

// Display config

int seed_display_config(void) {
	if (file_exists(DISPLAY_CONFIG_PATH)) return 0;

	cJSON* json = cJSON_CreateObject();
	cJSON_AddBoolToObject(json, "fullscreen", 0);
	cJSON_AddNumberToObject(json, "resolution_x", 800);
	cJSON_AddNumberToObject(json, "resolution_y", 600);

	int err = write_json(json, DISPLAY_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

int read_display_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not read the display config file with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->display) {
		printf(PRINT_ERROR "Could not read the display config file with a null "
				"display field in the config manager\n");
		return 1;
	}

	char* json_str = read_json(DISPLAY_CONFIG_PATH);
	if (!json_str) return 1;

	cJSON* json = cJSON_Parse(json_str);
	free(json_str);
	if (!json) {
		printf(PRINT_ERROR "Could not parse the json file {%s}\n", DISPLAY_CONFIG_PATH);
		return 1;
	}

	cJSON* fullscreen = cJSON_GetObjectItemCaseSensitive(json, "fullscreen");
	if (cJSON_IsBool(fullscreen)) {
		config_manager->display->fullscreen = fullscreen->valueint;
    } else {
		printf(PRINT_ERROR "Could not find a proper \"fullscreen\" field in the json file {%s}\n",
				DISPLAY_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	cJSON* resolution_x = cJSON_GetObjectItemCaseSensitive(json, "resolution_x");
	if (!cJSON_IsNumber(resolution_x)) {
		printf(PRINT_ERROR "Could not find a proper \"resolution_x\" field in the json file {%s}\n", 
				DISPLAY_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	cJSON* resolution_y = cJSON_GetObjectItemCaseSensitive(json, "resolution_y");
	if (!cJSON_IsNumber(resolution_y)) {
		printf(PRINT_ERROR "Could not find a proper \"resolution_y\" field in the json file {%s}\n",
				DISPLAY_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	config_manager->display->resolution = (IntSize) {
		.w = resolution_x->valueint, 
		.h = resolution_y->valueint
	};

	cJSON_Delete(json);
	return 0;
}

int write_display_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not write to the display config file with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->display) {
		printf(PRINT_ERROR "Could not write to the display config file with a null "
				"display field in the config manager\n");
		return 1;
	}

	cJSON* json = cJSON_CreateObject();
	cJSON_AddBoolToObject(json, "fullscreen", config_manager->display->fullscreen);
	cJSON_AddNumberToObject(json, "resolution_x", config_manager->display->resolution.w);
	cJSON_AddNumberToObject(json, "resolution_y", config_manager->display->resolution.h);

	int err = write_json(json, DISPLAY_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

// Player info config

int seed_player_info_config(void) {
	if (file_exists(PLAYER_INFO_CONFIG_PATH)) return 0;

	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Username", "Default");
	cJSON_AddStringToObject(json, "Language", "Default");

	int err = write_json(json, PLAYER_INFO_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

int read_player_info_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not read from the player info config file with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->player_info) {
		printf(PRINT_ERROR "Could not read the player info config file with a null "
				"player info field in the config manager\n");
		return 1;
	}

	char* json_str = read_json(PLAYER_INFO_CONFIG_PATH);
	if (!json_str) return 1;

	cJSON* json = cJSON_Parse(json_str);
	free(json_str);
	if (!json) {
		printf(PRINT_ERROR "Could not parse the json file {%s}\n", PLAYER_INFO_CONFIG_PATH);
		return 1;
	}

	cJSON* username = cJSON_GetObjectItemCaseSensitive(json, "Username");
	cJSON* language = cJSON_GetObjectItemCaseSensitive(json, "Language");

	if (cJSON_IsString(username)) {
		config_manager->player_info->username = strdup(username->valuestring);
	} else {
		printf(PRINT_ERROR "Could not find a proper \"Username\" field in the json file {%s}\n",
				PLAYER_INFO_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	if (cJSON_IsString(language)) {
		config_manager->player_info->language = strdup(language->valuestring);
	} else {
		printf(PRINT_ERROR "Could not find a proper \"Language\" field in the json file {%s}\n",
				PLAYER_INFO_CONFIG_PATH);
		cJSON_Delete(json);
		return 1;
	}

	cJSON_Delete(json);
	return 0;
}

int write_player_info_config(void) {
	ConfigManager* config_manager = get_config_manager();
	if (!config_manager) {
		printf(PRINT_ERROR "Could not write to the player info config file with a null "
				"config manager\n");
		return 1;
	}
	if (!config_manager->player_info) {
		printf(PRINT_ERROR "Could not write to the player info config file with a null "
				"player info field in the config manager\n");
		return 1;
	}

	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "Username", config_manager->player_info->username);
	cJSON_AddStringToObject(json, "Language", config_manager->player_info->language);

	int err = write_json(json, PLAYER_INFO_CONFIG_PATH);
	cJSON_Delete(json);
	return err;
}

// Tile definitions

cJSON* load_tile_definition(const char* filepath) {
	char* json_text = read_json(filepath);
	cJSON* tile_json = cJSON_Parse(json_text);

	if (!tile_json) {
		printf(PRINT_ERROR "Could not load a tile definition json file\n");
		free(json_text);
		return NULL;
	}

	free(json_text);
	return tile_json;
}
