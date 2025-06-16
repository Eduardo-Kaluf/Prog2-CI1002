CC = gcc

TARGET = A3

SRC = main.c

OBJ_DIR = obj

OBJ = $(OBJ_DIR)/main.o 

CFLAGS = -Wall -Wextra -o $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)