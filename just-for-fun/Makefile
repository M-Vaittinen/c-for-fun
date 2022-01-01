CC=gcc
SRC=main.c jana.c  kepi.c seina.c circle.c
CFLAGS= -Wall `sdl2-config --cflags`
CFLAGS= -ggdb -Wall `sdl2-config --cflags`

all: compile

compile: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o seina -lSDL2 -lm -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -lpthread

