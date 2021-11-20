CC = gcc
SRC = main.c points.c paikka.c alus.c powerup.c seina.c areena.c tekstit.c helpers.c server.c client.c server_data.c server_areena.c
HDR = alus.h areena.h common.h helpers.h hiscore.h media.h paikka.h piirrettavat_tekstit.h powerup.h seina.h server.h client.h server_data.h server_areena.h

all: $(SRC) $(HDR)
	$(CC) -Wall -ggdb `sdl2-config --cflags` $(SRC) -o ampu -lSDL2 -lm -lSDL2_ttf -lSDL2_mixer -lpthread
	cp snd/* /home/mvaittin/.kolomiosnd/.
install:
	cp ampu /usr/bin/.
#	cp -r snd ~/.kolomiosnd
#	cp ampu.desktop ~/.local/share/applications/.

clean:
	rm -rf ampu
