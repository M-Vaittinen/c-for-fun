CC = gcc

all:
	$(CC) -Wall -ggdb `sdl2-config --cflags` main.c points.c -o ampu -lSDL2 -lm -lSDL2_ttf -lSDL2_mixer
	cp snd/* /home/mvaittin/.kolomiosnd/.
install:
	cp ampu /usr/bin/.
#	cp -r snd ~/.kolomiosnd
#	cp ampu.desktop ~/.local/share/applications/.

clean:
	rm -rf ampu
