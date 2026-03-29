# Ensure compiler is gcc
CC = gcc

# Compiler flags
CFLAGS = -ansi -pedantic -Wall -Werror

# Project files
SOURCES = main.c helper.c lexer.c parser.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = run

# Create executable
# $@ refers to $(TARGET)
# $^ refers to $(OBJECTS)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm *.o