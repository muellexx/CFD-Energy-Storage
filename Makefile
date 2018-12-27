# ------------------------------------------------
# Generic Makefile
#
# Author: alex.muellex@gmail.com
# Date  : 2018-12-27
#
# ------------------------------------------------

# Project Name (Generate executable with this name)
TARGET = FOCFD

CC=g++
# Compiling flags
CFLAGS=-Wall -fexceptions -O3 -fopenmp -Iinclude

# Directories
SRCDIR = src
OBJDIR = obj/Release
BINDIR = bin/Release

SRC      := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJ      := $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET)

.Phony: clean

$(BINDIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $(OBJ) -s -lgomp -pthread

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -rf $(OBJDIR)/*.o $(BINDIR)/$(TARGET)
	@echo "Executable and Objects removed!"
