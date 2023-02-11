CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o texturecube.o $(CFLAGS) $(LIBS) ./texturecube.cpp ./shaders.cpp
clean:
	$(RM) *.o
