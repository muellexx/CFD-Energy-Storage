config ?= debug

CC=g++
CFLAGS=-Wall -fexceptions -O3 -fopenmp

ODIR=obj/Release

_OBJ=energy.o file.o init.o main.o mms.o state.o struct.o temp.o timer.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

.Phony: all clean hellomake

all: FoCFD

$(ODIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

FoCFD: $(OBJ)
	@mkdir -p bin/Release
	$(CC) -o bin/Release/$@ $^ -s -lgomp -pthread

clean:
	rm -r obj/Release/* bin/Release/*
