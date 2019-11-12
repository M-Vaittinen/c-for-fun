CC = gcc

all:
	$(CC) -Wall -ggdb main.c -o ampu -lSDL2 -lm

clean:
	rm -rf ampu
