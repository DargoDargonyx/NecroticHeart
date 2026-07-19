WINDOW_SRC := $(wildcard $(SRC_WINDOW_DIR)/*.c)
WINDOW_OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(WINDOW_SRC))

$(BIN_DIR)/window/%.o: $(SRC_WINDOW_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(BASE_CFLAGS) $(SDL_CFLAGS) -c $< -o $@
