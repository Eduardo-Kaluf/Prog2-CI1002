# Compiler to use
CC = gcc

# Name of the final executable
TARGET = A3

# Directory to store compiled object files (.o) and dependency files (.d)
OBJ_DIR = obj

# Automatically find all .c source files in the current directory
SRC = $(wildcard *.c)

# Generate the list of object file names from the source file list
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

# Generate a list of dependency file names (e.g., obj/main.d)
DEPS = $(OBJ:.o=.d)

# Compiler flags:
# -g: Adds debugging information for use with GDB
# -std=c17: Enforces the C17 language standard
# -Wall -Wextra: Enable all common and extra warnings
# -MMD -MP: Crucial flags to generate dependency files for header tracking
# `pkg-config`: A tool that provides the correct compiler flags for Allegro
CFLAGS = -g -std=c17 -Wall -Wextra -MMD -MP $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

# Linker flags:
# `pkg-config`: Provides the correct library linking flags for Allegro
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

# The default target, called when you just run `make`
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Include the dependency files if they exist
-include $(DEPS)

# Pattern rule to compile a .c file into a .o file
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Target to clean up the project directory
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(TARGET)
