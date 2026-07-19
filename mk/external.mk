EXTERNAL_SRC := $(wildcard $(SRC_EXTERNAL_DIR)/*.c)
EXTERNAL_OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(EXTERNAL_SRC))

$(BIN_DIR)/external/%.o: $(SRC_EXTERNAL_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(BASE_CFLAGS) -c $< -o $@
