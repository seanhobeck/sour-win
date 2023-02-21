CXX = g++
SOURCES = src/main.cpp
CXX_FLAGS = -Wall -Wreorder -g -fPIC -shared 
LINKER = -ldl -lSDL2 -lSDL2_image -lSDL2_mixer -lz -lGL -lX11 -Lenet

all:
	$(CXX) $(SOURCES) $(CXX_FLAGS)$(LINKER) -o sour.so