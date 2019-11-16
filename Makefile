CC = gcc

all:
	$(CC) -Wall -ggdb `sdl2-config --cflags` main.c -o ampu -lSDL2 -lm -lSDL2_ttf
install:
	cp ampu /usr/bin/.
#	cp ampu.desktop ~/.local/share/applications/.

clean:
	rm -rf ampu
