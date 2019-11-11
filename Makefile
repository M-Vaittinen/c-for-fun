CC = gcc

all:
	$(CC) -Wall main.c -o ampu -lSDL2 -lm

clean:
	rm -rf ampu
