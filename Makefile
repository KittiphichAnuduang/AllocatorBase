# ================================
# AllocatorBase v1.0
# ================================

# Compiler and Flags
CC       := gcc
STD      := -std=c11
WARN     := -Wall -Wextra -Wpedantic
INCLUDE  := -I.
SRC      := main.c AllocatorBase.c VM.c DebugTool.c
OBJ      := $(SRC:.c=.o)
TARGET   := alloc_demo

# Color Codes
RESET := \033[0m
BOLD  := \033[1m
GREEN := \033[32m
YELLOW:= \033[33m
BLUE  := \033[34m
RED   := \033[31m
CYAN  := \033[36m
WHITE := \033[37m

# Build Configurations
CFLAGS_debug   := $(STD) $(WARN) $(INCLUDE) -O0 -g -Wno-unused-parameter -Wno-unused-variable
CFLAGS_release := $(STD) $(WARN) $(INCLUDE) -O2 -Werror

# Makefile Targets
.PHONY: all clean debug release run

# Default target
all: release

# Debug build
debug:
	@printf "$(BOLD)$(YELLOW)========== Building DEBUG Mode ==========$(RESET)\n"
	$(CC) $(CFLAGS_debug) -c main.c -o main.o
	$(CC) $(CFLAGS_debug) -c AllocatorBase.c -o AllocatorBase.o
	$(CC) $(CFLAGS_debug) -c VM.c -o VM.o
	$(CC) $(CFLAGS_debug) -c DebugTool.c -o DebugTool.o
	@printf "$(CYAN)[LINK]$(RESET) $(TARGET)"
	$(CC) $(CFLAGS_debug) -o $(TARGET) $(OBJ)
	@printf "$(GREEN)Build successful!$(RESET)\n"

# Release build
release:
	@printf "$(BOLD)$(BLUE)========== Building RELEASE Mode ==========$(RESET)\n"
	$(CC) $(CFLAGS_release) -c main.c -o main.o
	$(CC) $(CFLAGS_release) -c AllocatorBase.c -o AllocatorBase.o
	$(CC) $(CFLAGS_release) -c VM.c -o VM.o
	$(CC) $(CFLAGS_release) -c DebugTool.c -o DebugTool.o
	@printf "$(CYAN)[LINK]$(RESET) $(TARGET)"
	$(CC) $(CFLAGS_release) -o $(TARGET) $(OBJ)
	@printf "$(GREEN)Build successful!$(RESET)\n"

# Run the program
run:
	@printf "$(WHITE)[RUN]$(RESET) ./$(TARGET)"
	@./$(TARGET)

# Clean build artifacts
clean:
	@printf "$(RED)[CLEAN]$(RESET) "
	rm -f $(OBJ) $(TARGET)
