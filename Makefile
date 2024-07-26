# Makefile for KTH's code.

include Makefile.inc

C_SOURCES = src/extra/coldet/*.c
SOURCES = src/*.cpp src/extra/*.cpp src/extra/coldet/*.cpp src/extra/pathfinder/*.cpp

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard $(SOURCES))) $(patsubst %.c, %.o, $(wildcard $(C_SOURCES)))
DEPENDS = $(patsubst %.cpp, %.d, $(wildcard $(SOURCES)))

THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

SDL_LIB = -lSDL2 
GLUT_LIB = -w
BASS_LIB = -lbass


ifeq  ($(OS),Windows_NT)
LIBS = $(SDL_LIB) $(GLUT_LIB) $(BASS_LIB) -Wl,-subsystem,windows
else
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIBS = $(SDL_LIB) $(GLUT_LIB) $(BASS_LIB)
endif
ifeq ($(UNAME), Darwin)
LIBS = $(SDL_LIB) $(GLUT_LIB) $(BASS_LIB) -framework OpenGL -framework Cocoa -framework IOKit
endif
endif

all:	main

main:	$(DEPENDS) 
	$(CXX) $(CXXFLAGS) -o $@  $(LIBS)

run:
	./main

clean:
	rm -f $(OBJECTS) $(DEPENDS) main *.pyc


