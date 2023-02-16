CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o coords.o $(CFLAGS) $(LIBS) ./coords.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
