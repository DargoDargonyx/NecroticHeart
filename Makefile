include mk/config.mk
include mk/engine.mk
include mk/external.mk
include mk/util.mk
include mk/window.mk

MAIN_SRC := $(SRC_DIR)/main.c
MAIN_OBJ := $(BIN_DIR)/main.o

OBJ := $(MAIN_OBJ) $(ENGINE_OBJ) $(EXTERNAL_OBJ) \
	   $(UTIL_OBJ) $(WINDOW_OBJ)

TARGET := $(DIST_DIR)/game

.PHONY: copy_libs clean debug release run

all: $(TARGET)

$(BIN_DIR)/main.o: $(MAIN_SRC)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(BASE_CFLAGS) $(SDL_CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@echo "Linking files..."
	@mkdir -p $(DIST_DIR)
	@$(CC) $^ -o $@ $(SDL_LIBS)
	@echo "Project linked."

copy_libs:
	@echo "Copying SDL libraries..."
	@mkdir -p $(DIST_DIR)/lib
	ldd $(TARGET) | awk `{print $$3}` | grep -E '^/' | \
	grep -vE 'libc\.so|libm\.so|ld-linux' | sort -u | \
	while read lib; do
		@cp -n $$lib $(DIST_DIR)/lib/; \
	done

clean:
	@echo "Cleaning targets..."
	@rm -rf $(BIN_DIR) $(DIST_DIR)
	@echo "Targets cleaned."

debug:
	$(MAKE) clean MODE_CFLAGS="-g -00"

release:
	$(MAKE) clean MODE_CFLAGS="-02 -DNDEBUG"
	strip $(TARGET)

run: all
	@cd $(DIST_DIR) && ./game $(ARGS)
