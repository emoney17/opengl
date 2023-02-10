CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all: tutorial

tutorial: triangle matrix cube

triangle:
	$(CC) -o triangle.o $(CFLAGS) $(LIBS) ./triangle.cpp
matrix:
	$(CC) -o matrix.o $(CFLAGS) $(LIBS) ./matrix.cpp
cube:
	$(CC) -o cube.o $(CFLAGS) $(LIBS) ./texture_cube/cube.cpp
clean:
	$(RM) *.o
