# Compiler and Linker
CC := cc

# Directories
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin

# Flags, Libraries and Includes
CFLAGS := -Wall -I$(INC_DIR)
LDFLAGS :=
LIBS :=

# Source and Object Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(wildcard $(INC_DIR)/*.h)

# Target Executable
TARGET := $(BIN_DIR)/DR

# Default make
all: $(TARGET)

# Link Object Files to Create Binary
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile Source Files into Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up Objects, Binaries
clean:
	$(RM) $(OBJS) $(TARGET)

# Phony Targets
.PHONY: all clean
