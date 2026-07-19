UTIL_SRC := $(wildcard $(SRC_UTIL_DIR)/*.c)
UTIL_OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(UTIL_SRC))

$(BIN_DIR)/util/%.o: $(SRC_UTIL_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(BASE_CFLAGS) -c $< -o $@
