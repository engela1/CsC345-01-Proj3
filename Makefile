# Compiler and linker
CC = gcc
CFLAGS = -Wall -std=c99 -g  # Enable warnings and debugging symbols
LDFLAGS =  # Add linker flags here if needed

# The name of the executable
TARGET = proj3

# Source files
SRC = main.c

# Object files (derived from source files)
OBJ = $(SRC:.c=.o)

# Default target: build the executable
all: $(TARGET)

# Rule to create the executable from object files
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

# Rule to compile .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files (object files and executable)
clean:
	rm -f $(OBJ) $(TARGET)

# Mark targets that aren't files
.PHONY: all clean