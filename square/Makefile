CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o square.o $(CFLAGS) $(LIBS) ./square.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
