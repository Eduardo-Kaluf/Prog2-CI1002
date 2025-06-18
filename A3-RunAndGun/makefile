# Compiler to use
CC = gcc

# Name of the final executable
TARGET = A3

# Directory to store compiled object files (.o)
OBJ_DIR = obj

# Automatically find all .c source files in the current directory
SRC = $(wildcard *.c)

# Generate the list of object file names from the source file list
# This replaces each ".c" with ".o" and puts it in the OBJ_DIR
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

# Compiler flags:
# -Wall -Wextra: Enable all common and extra warnings (good practice)
# `pkg-config`: A tool that provides the correct compiler flags for Allegro
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

# Linker flags:
# `pkg-config`: Provides the correct library linking flags for Allegro
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

# The default target, called when you just run `make`
all: $(TARGET)

# Rule to link the final executable
# This depends on all the object files being created first
$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Pattern rule to compile a .c file into a .o file
# This runs for each source file
# $< is the name of the source file (e.g., main.c)
# $@ is the name of the target (e.g., obj/main.o)
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Target to clean up the project directory
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(TARGET)

