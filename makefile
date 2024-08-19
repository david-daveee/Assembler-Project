# Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468

# Main file
TARGET = assembler

# Compiler and flags
CC = gcc
CFLAGS = -std=c90 -Wall -Wextra -pedantic

# source files
SRCS = assembler.c assisting_functions.c first_pass.c macro.c second_pass.c fixed_tables.c

# Objective files
OBJS = $(SRCS:.c=.o)

# Headers files
HEADERS = prototypes.h structures.h

# Default rule
all: $(TARGET)

# Rule for creating an executable file
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule for creating object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for cleaning up a project
clean:
	rm -f $(OBJS) $(TARGET)

# Rule for recompilation
rebuild: clean all

