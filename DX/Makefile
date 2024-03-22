# Compiler and linker configurations
CC = cc
CFLAGS = -Iinc -Wall
LDFLAGS =

# Define directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = inc

# Target executable name
TARGET = $(BIN_DIR)/DX

# Find all .c files in src directory, and convert them to .o files in obj directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files to create target executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target for cleaning up the project
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
