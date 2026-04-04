# Ensure compiler is gcc
CC = gcc

# Compiler flags
CFLAGS = -ansi -pedantic -Wall -Werror

# Project files
# SOURCES = main.c helper.c lexer.c parser.c
SOURCES = main.c helper.c lexer.c ast.c evaluator.c dualUtils.c graph.c helper_functions/pbPlots/pbPlots.c helper_functions/pbPlots/supportLib.c
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