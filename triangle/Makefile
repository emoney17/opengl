CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o triangle.o $(CFLAGS) $(LIBS) ./triangle.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
