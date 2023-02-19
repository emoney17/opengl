CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o camera.o $(CFLAGS) $(LIBS) ./camera.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
