# Makefile for ParallelFileCompressor

# Compiler settings - Can change cc to gcc for C compilation
CC = gcc
CFLAGS = -Wall -g

# Define the directories.
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Define source, object, and binary files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/compressor

# Default target
all: $(EXECUTABLE)

# Link objects to binary
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files to objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Create directories
$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Help target to display makefile rules
help:
	@echo "Available rules:"
	@echo "all      - Builds the compressor binary"
	@echo "clean    - Removes object and binary directories"
