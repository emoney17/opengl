CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o textured_cube.o $(CFLAGS) $(LIBS) ./textured_cube.cpp ./shaders.cpp
clean:
	$(RM) *.o
