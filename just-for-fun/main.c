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

#include "kepi.h"

#define OPTSTRING "s:v?"
#define KEPI 0xabba

static struct option long_options[] =
{
    {"janoja" , required_argument, 0, 'j'},
    {"kepi", no_argument, 0, 'k'},
    {"version",  no_argument, 0, 'v'},
    {"help",  no_argument, 0, 'h'},
    {0,0,0,0}
};

static void print_version()
{
	printf("version 0\n");
}

static int parse_args(int argc, char *argv[], int *seinia)
{
	int index;
	int c;
	int ret = 1;

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
			ret = -1;
		case 'h':
			print_version();
			printf("Usage: %s [-j janojen-maara ]\n", argv[0]);
			return ret;
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
			return KEPI;
		}
	}

	return 0;
}

#define MAX_SEINA 2048

struct seina {
	int alku_x;
	int alku_y;
	int loppu_x;
	int loppu_y;
};

static void jarjesta(int *alku, int *loppu)
{
	int tmp;

	if (*alku > *loppu) {
		tmp = *alku;
		*alku = *loppu;
		*loppu = tmp;
	}
}

static void arvo_pistepari(int *alku, int *loppu, int max)
{
	*alku = (rand() % max);
uus_alku:
	*loppu = (rand() % max);

	if (*alku == *loppu)
		goto uus_alku;

	jarjesta(alku, loppu);
}

static void arvo_alkuseina(struct seina *s, int leveys, int korkeus)
{
	s[0].alku_x = 1;
	s[0].loppu_x = leveys - 1;
	arvo_pistepari(&s[0].alku_y, &s[0].loppu_y, korkeus);
}

static int seinan_mitta(struct seina *s)
{
	int mitta_x, mitta_y;

	mitta_x = s->loppu_x - s->alku_x;
	mitta_y = s->loppu_y - s->alku_y;

	return (int)sqrt((double)mitta_x*mitta_x + (double)mitta_y * mitta_y);

}

static void keskipiste(int x1, int y1, int x2, int y2, int *kpx, int *kpy)
{
	*kpx = (x2 - x1)/2 + x1;
	*kpy = (y2 - y1)/2 + y1;
}
/*
void uus_pitse(int etaisyys, int kpx, int kpy, double sin_angle, double cos_angle, int *uus_x, int *uus_y)
{
}
*/


static int jaa_seina(struct seina s, struct seina *a, struct seina *b, int leveys, int korkeus)
{
	int pituus, etaisyys, kpx, kpy, uus_x, uus_y, px, py;
//	double sin_angle, cos_angle;

	px = s.loppu_x - s.alku_x;
	py = s.loppu_y - s.alku_y;

	pituus = seinan_mitta(&s);
	if (!pituus) {
		printf("mitta tyhja\n");
		return -1;
	}

	keskipiste(s.alku_x, s.alku_y, s.loppu_x, s.loppu_y, &kpx, &kpy);
	etaisyys = pituus/2;
	if (!etaisyys) {
		printf("etaisyytta ei ole\n");
		return -1;
	}

	//sin_angle = ((double)(s->loppu_y - s->alku_y)) / (double)pituus;
	//cos_angle = ((double)(s->loppu_x - s->alku_x)) / (double)pituus;
	//uus_pitse(etaisyys, kpx, kpy, sin_angle, cos_angle, &uus_x, &uus_y);

	uus_x = kpx - py/3;
	if (uus_x >= leveys || uus_x <= 0)
		uus_x = kpx + py/3;

	if (uus_x <= 0)
		uus_x = 1;
	else if (uus_x >= leveys)
		uus_x = leveys - 1;

	uus_y = kpy + px/3;
	if (uus_y >= korkeus || uus_y <= 0)
		uus_y = kpy - px/3;

	if (uus_y <= 0)
		uus_y = 1;
	else if (uus_y >= korkeus)
		uus_y = korkeus - 1;

	a->alku_x = s.alku_x;
	a->alku_y = s.alku_y;

	b->loppu_x = s.loppu_x;
	b->loppu_y = s.loppu_y;

	a->loppu_x = b->alku_x = uus_x;
	a->loppu_y = b->alku_y = uus_y;

	return 0;
}

static int laske_uudet_seinat(struct seina *s, int seinamaara, int leveys, int korkeus)
{
	int i;

	for (i = seinamaara - 1; i >= 0; i--) {
		int uusi1 = i * 2;
		int uusi2 = uusi1 + 1;

		if (jaa_seina(s[i], &s[uusi1], &s[uusi2], leveys, korkeus))
			return -1;
	}

	return 0;
}

static void piirra_seinat(SDL_Renderer* renderer, struct seina **st, int seinatauluja, int seinia, int leveys, int korkeus)
{
	int i, j;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, 0, 0, 0, korkeus - 1);
	SDL_RenderDrawLine(renderer, 0, 0, leveys - 1, 0);
	SDL_RenderDrawLine(renderer, 0, korkeus -1, leveys -1 , korkeus - 1);
	SDL_RenderDrawLine(renderer, leveys - 1, korkeus - 1, leveys - 1, 0);

	for (j = 0; j < seinatauluja; j++) {
		struct seina *s = st[j];
	for (i = 0; i < seinia; i++) {
		SDL_Rect r;

		r.x =  s[i].alku_x - 1;
		r.y = s[i].alku_y - 1;
		r.w = 3;
		r.h = 3;

		if (!(i%2))
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);

		SDL_RenderDrawRect(renderer, &r);

		r.x = s[i].loppu_x;
		r.y = s[i].loppu_y;
		SDL_RenderDrawRect(renderer, &r);
		SDL_RenderDrawLine(renderer, s[i].alku_x, s[i].alku_y, s[i].loppu_x, s[i].loppu_y);
	}
	}

	SDL_RenderPresent(renderer);
}

int main(int arc, char *argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer;
	int leveys, korkeus, i, j, ret;
	int seinamaara = 0;
	int alkuseinia = 1;
	struct seina **seinataulukot;
	bool kepi = false;

	srand(time(NULL));

	ret = parse_args(arc, argv, &alkuseinia);
	/*
	 * Return value KEPI is special and indicates we should do kehapiste
	 * stuff
	 */
	if (ret == KEPI) {
		kepi = true;
	} else if (ret) {
		if (ret < 0)
			return -1;
		return 0;
	}

	/*
	 * We don't need these tables for KEPI.
	 * Yep. These ifs are ugly. It'd be nicer to move the 'seina'-stuff in
	 * own file and just keep the generic SDL init and arg parsing here &&
	 * depending on the args just proceed with correct program.
	 *
	 * Well, I don't have the time or energy to split this now so I just
	 * leave it as is and do these ifs. I won't move the allocation after
	 * SDL init (which would also leave us with clearer code as then we
	 * could branch-out for KEPI after SDL init but before the alloc)
	 * because these allocations may fail if -j has a large value. It does
	 * not really matter but something in me says that it's nicer to get the
	 * alloc failure right at the beginning and not to bother with the
	 * SDL inits if we don't have the memory.
	 */
	if (!kepi) {
		seinataulukot = calloc(alkuseinia, sizeof(struct seina *));
		if (!seinataulukot)
			return -ENOMEM;

		for (i = 0; i < alkuseinia; i++) {
			seinataulukot[i] = calloc(MAX_SEINA, sizeof(struct seina));
			if (!seinataulukot[i])
				return -ENOMEM;
		}
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
	if (kepi)
		return the_kepi(renderer, leveys, korkeus);


	while (1) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		for (j = 0; j < alkuseinia; j++)
			arvo_alkuseina(seinataulukot[j], leveys, korkeus);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);

		for (seinamaara = 1; seinamaara <= MAX_SEINA; seinamaara <<= 1) {
			SDL_Event event;

			piirra_seinat(renderer, &seinataulukot[0], alkuseinia, seinamaara, leveys, korkeus);

			if (seinamaara == MAX_SEINA)
				break;
			for (j = 0; j < alkuseinia; j++)
				if (laske_uudet_seinat(&seinataulukot[j][0], seinamaara, leveys, korkeus))
				break;
			sleep(1);
			while( SDL_PollEvent( &event )) {
				if (event.type == SDL_KEYDOWN) {
					SDL_Quit();
					return 0;
				}

			}
		}
		sleep(2);
	}
	SDL_Quit();

	return 0;
}
