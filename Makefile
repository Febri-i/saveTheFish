OBJS = *.cpp

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -I./includes
OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./bin/$(OBJ_NAME)

