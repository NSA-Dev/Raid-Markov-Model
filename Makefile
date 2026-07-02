# Compiler and Flags
CC       := gcc
CFLAGS   := -O3 -Wall -Wextra -std=c99
LIBS     := -lm

# Target Executable Name
TARGET   := raid_sim

# Directories
SRC_DIR  := src
OBJ_DIR  := obj

# Source and Object Files
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default Rule
all: $(TARGET)

# Link the Final Executable
$(TARGET): $(OBJS)
	@echo "Linking final executable: $@"
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compile Source Files to Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Create the Object Directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean Build Artifacts
clean:
	@echo "Cleaning up build files..."
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony Targets
.PHONY: all clean