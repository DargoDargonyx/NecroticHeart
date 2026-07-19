ENGINE_SRC := $(wildcard $(SRC_ENGINE_DIR)/*.c)
ENGINE_OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(ENGINE_SRC))

$(BIN_DIR)/engine/%.o: $(SRC_ENGINE_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(BASE_CFLAGS) -c $< -o $@
