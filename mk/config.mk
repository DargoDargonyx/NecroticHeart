CC := gcc

BIN_DIR  := bin
DIST_DIR := dist
INC_DIR  := include
SRC_DIR  := src

SRC_ENGINE_DIR   := $(SRC_DIR)/engine
SRC_EXTERNAL_DIR := $(SRC_DIR)/external
SRC_UTIL_DIR     := $(SRC_DIR)/util
SRC_WINDOW_DIR   := $(SRC_DIR)/window

BASE_CFLAGS := -Wall -Wextra -I $(INC_DIR)
MODE_CFLAGS ?= -02 -DNDEBUG

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS   := $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image -lSDL2_mixer
