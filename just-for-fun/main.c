#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "fun.h"
#include "circle.h"
#include "kepi.h"
#include "seina.h"

#define OPTSTRING "ahj:ks:vy?"
#define KAIKKI (KEPI| (YMPY << 16))
#define KEPI 0xabba
#define YMPY 0xbabe

static struct option long_options[] =
{
    {"all",	no_argument, 0, 'a'},
    {"janoja" , required_argument, 0, 'j'},
    {"kepi", no_argument, 0, 'k'},
    {"version",  no_argument, 0, 'v'},
    {"help",  no_argument, 0, 'h'},
    {"ympyra",  no_argument, 0, 'y'},
    {0,0,0,0}
};

static void print_help(char *prog)
{
	printf("Usage: %s [-k -v -h -y] [-j number of lines]\n", prog);
	printf("-k --kepi	Launch drawing with triangles instead of the\n		default one with line splitting\n");
	printf("-v --version	print version and exit\n");
	printf("-h --help	print this help and exit\n");
	printf("-j --janoja	when -k is not given the number of lines to\n		start with can be specified using -j\n");
	printf("-y --ympyra	Launch drawing circles instead of the\n		default one with line splitting\n");
	printf("-a --all	Käy läpi kaikki\n");
}

static void print_version()
{
	printf("version 0\n");
}

static int parse_args(int argc, char *argv[], int *seinia)
{
	int index;
	int c;
	int helpret = 1, ret = 0;

	while(-1 != (c = getopt_long(argc, argv, OPTSTRING, long_options,
				     &index))) {
		switch(c)
		{
		case ':':
			return -1;
		case 'v':
			print_version();
			return 1;
		case '?':
			printf("Unknown parameter\n");
			helpret = -1;
		case 'h':
			print_version();
			print_help(argv[0]);
			return helpret;
        	case 'j':
		{
			char *test;

			if (!optarg) {
				printf ("-j puuttuva janojen maara\n");
				return -1;
			}
			*seinia = strtol(optarg, &test, 0);
			if (!test || (test[0] && test[0] != '\n') || *seinia < 0) {
				printf("Janojen maaran pitaa olla positiivinen luku\n");
				return -1;
			}
			break;
		}
		case 'k':
			if (ret == YMPY || ret == KAIKKI) {
				printf("-k, -y ja -a ovat toisensa poissulkevia\n");
				return -1;
			}
			ret = KEPI;
			break;
		case 'y':
			if (ret == KEPI || ret == KAIKKI) {
				printf("-k, -y ja -a ovat toisensa poissulkevia\n");
				return -1;
			}
			ret = YMPY;
			break;
		case 'a':
			if (ret == YMPY || ret == KEPI) {
				printf("-k, -y ja -a ovat toisensa poissulkevia\n");
				return -1;
			}
			ret = KAIKKI;
			break;
		}
	}

	return ret;
}

int main(int arc, char *argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer;
	int leveys, korkeus, ret, alkuseinia = 1;
	bool kepi = false;
	bool ympy = false;
	bool kaikki = false;

	srand(time(NULL));

	ret = parse_args(arc, argv, &alkuseinia);
	/*
	 * Return value KEPI is special and indicates we should do kehapiste
	 * stuff
	 */
	if (ret == KEPI) {
		kepi = true;
	} else if (ret == YMPY) {
		ympy = true;
	} else if (ret == KAIKKI) {
		kaikki = true;
	} else if (ret) {
		if (ret < 0)
			return -1;
		return 0;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	if (TTF_Init()) {
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return -1;
	}

	SDL_GetWindowSize(window, &leveys, &korkeus);
	leveys -= 2;
	korkeus -= 2;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	/*
	 * Decide which decoration to display
	 */
	if (kaikki) {
		while (1) {
		/* TODO - add param to exit and to not quit SDL */
			ret = the_kepi(renderer, leveys, korkeus, true);
			if (ret)
				goto out;
			ret = circle(renderer, leveys, korkeus, alkuseinia, true);
			if (ret)
				goto out;
			ret = linesplit(renderer, leveys, korkeus, alkuseinia, true);
			if (ret)
				goto out;
		}
	} else if (kepi)
		ret = the_kepi(renderer, leveys, korkeus, false);
	else if (ympy)
		ret = circle(renderer, leveys, korkeus, alkuseinia, false);
	else
		ret = linesplit(renderer, leveys, korkeus, alkuseinia, false);

out:
	if (ret == ALL_OK)
		ret = 0;

	SDL_Quit();

	return ret;
}
