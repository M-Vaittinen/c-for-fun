#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


#include "seina.h"

struct seina {
	int alku_x;
	int alku_y;
	int loppu_x;
	int loppu_y;
};

#define MAX_SEINA 2048

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

static int jaa_seina(struct seina s, struct seina *a, struct seina *b, int leveys, int korkeus)
{
	int pituus, etaisyys, kpx, kpy, uus_x, uus_y, px, py;

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

int linesplit(SDL_Renderer* renderer, int leveys, int korkeus, int alkuseinia)
{
	int seinamaara = 0;
	int i, j;
	struct seina **seinataulukot;

	seinataulukot = calloc(alkuseinia, sizeof(struct seina *));
	if (!seinataulukot)
		return -ENOMEM;

	for (i = 0; i < alkuseinia; i++) {
		seinataulukot[i] = calloc(MAX_SEINA, sizeof(struct seina));
		if (!seinataulukot[i])
			return -ENOMEM;
	}

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
			//sleep(1);
			SDL_Delay(1000);
			while( SDL_PollEvent( &event )) {
				if (event.type == SDL_KEYDOWN) {
			//		SDL_Quit();
					return 0;
				}

			}
		}
		sleep(2);
	}
}
