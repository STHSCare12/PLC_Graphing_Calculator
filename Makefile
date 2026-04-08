# Ensure compiler is gcc
CC = gcc
STD = c99

# Compiler flags
CFLAGS = -pedantic -Wall -Werror

# Project files
# .c files
SRC_DIR = .
SOURCES = $(filter-out $(SRC_DIR)/parser.c, $(wildcard $(SRC_DIR)/*.c)) helper_functions/pbPlots/pbPlots.c helper_functions/pbPlots/supportLib.c

# .o files
$(OBJECTS)
OBJECTS = $(SOURCES:.c=.o)
TARGET = calculator

# Create executable
# $@ refers to $(TARGET)
# $^ refers to $(OBJECTS)
$(TARGET): $(OBJECTS)
	$(CC) -std=$(STD) $(CFLAGS) -o $@ $^ -lm

.PHONY: clean

clean:
	rm *.o