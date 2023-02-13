CC = g++
CFLAGS= -Wall -Wextra
LIBS = -lGL -lglfw -lGLEW -lm

all:
	$(CC) -o transform.o $(CFLAGS) $(LIBS) ./transform.cpp ../common/read_shaders.cpp ../common/init.cpp
clean:
	$(RM) *.o
