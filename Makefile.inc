# Common included file for all makefiles in the project

CC       	= gcc
CXX         	= g++
INCLFLAGS = -IC:\Libraries\i686-w64-mingw32\include\SDL2
CFLAGS   	= -g -Wall -Wno-unused-variable 
CXXFLAGS   	= $(INCLFLAGS) -g -std=c++17 -Wall -Wno-unused-variable
#CFLAGS   	= -O2 -Wall -Werror
#CXXFLAGS   	= -O2 -Wall -Werror
AR		= ar
MAKE        	= make
