CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -Iinclude
SRCS = src/main.c src/functions.c src/auxiliares.c
TARGET = vaxlogistics

all: $(TARGET)

$(TARGET): $(SRCS)
	@echo "Compiling $(TARGET)..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
	@echo "Compilation successful: ./$(TARGET)"

clean:
	@rm -f $(TARGET)
	@echo "Cleaned build artifacts."

.PHONY: all clean
