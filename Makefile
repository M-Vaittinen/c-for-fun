CC = gcc

all:
	$(CC) -Wall -ggdb main.c -o ampu -lSDL2 -lm
install:
	cp ampu /usr/bin/.
#	cp ampu.desktop ~/.local/share/applications/.

clean:
	rm -rf ampu
