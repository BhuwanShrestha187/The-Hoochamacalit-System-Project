# Define the compiler
CC := cc

# Define any compile-time flags
CFLAGS := -Wall -Iinc

# Define the directory for object files
OBJDIR := obj

# Define the source directory
SRCDIR := src

# Define the binary (output) directory
BINDIR := bin

# Define source files
SOURCES := $(wildcard $(SRCDIR)/*.c)

# Define object files
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Define the output file
TARGET := $(BINDIR)/DC

# Default target
all: $(TARGET)

# Link the target with all object files
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets
.PHONY: all clean
