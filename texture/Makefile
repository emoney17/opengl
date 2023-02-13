CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o texture.o $(CFLAGS) $(LIBS) ./texture.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
