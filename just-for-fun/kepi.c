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

#include "piste.h"
#include "jana.h"
#include "kepi.h"

#define MAX_KEHAP 600

//static SDL_Renderer* grdr;

void arvo_kolmio(SDL_Renderer* grdr, struct piste *k, int max_x, int max_y)
{
	int i;
	struct piste tmp[3] = {{ 0, 0 }};

	for (i = 0; i < 3; i++) {
		int j = 0;

		k[i].x = (rand() % max_x);
		k[i].y = (rand() % max_y);
		while (j < i) {
			if (k[i].x == k[j].x && k[i].y == k[j].y) {
				i--;
				break;
			}
			j++;
		}
	}

	if (k[0].x < k[1].x) {
		tmp[0] = k[0];
		if (k[1].x < k[2].x) {
			tmp[1] = k[1];
			tmp[2] = k[2];
		} else {
			tmp[2] = k[1];
			if (k[2].x < k[0].x) {
				tmp[0] = k[2];
				tmp[1] = k[0];
			} else {
				tmp[1] = k[2];
			}
		}
	} else {
		tmp[0] = k[1];
		if (k[0].x < k[2].x) {
			tmp[1] = k[0];
			tmp[2] = k[2];
		} else {
			tmp[2] = k[0];
			if (k[2].x < k[1].x) {
				tmp[0] = k[2];
				tmp[1] = k[1];
			} else {
				tmp[1] =  k[2];
			}
		}

	}

	k[0] = tmp[0];
	k[1] = tmp[1];
	k[2] = tmp[2];

	SDL_RenderPresent(grdr);
	SDL_SetRenderDrawColor(grdr, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(grdr, k[0].x, k[0].y, k[1].x, k[1].y);
	SDL_RenderDrawLine(grdr, k[1].x, k[1].y, k[2].x, k[2].y);
	SDL_RenderDrawLine(grdr, k[2].x, k[2].y, k[0].x, k[0].y);
	SDL_SetRenderDrawColor(grdr, 255, 0, 255, SDL_ALPHA_OPAQUE);
}

int laske_pts(struct piste kylki1, struct piste kylki2, int kokopit,
	      int num_vali, struct piste **pisteet)
{
	struct piste *p;
	int i;

	*pisteet = calloc(sizeof(struct piste), num_vali + 1);
	if (!*pisteet)
		return -ENOMEM;

	p = *pisteet;

	for (i = 0; i < num_vali; i++) {
		p[i].x = (double)i * (kylki2.x - kylki1.x) / (double)num_vali + kylki1.x;
		p[i].y = (double)i * (kylki2.y - kylki1.y) / (double)num_vali + kylki1.y;
	}
	p[num_vali] = kylki2;

	return 0;
}

void piirra_pts(SDL_Renderer* renderer, struct piste *p, int lkm)
{
	int i;

	for (i = 0; i < lkm; i++) {
		SDL_Rect r;

		r.x = p[i].x;
		r.y = p[i].y;
		r.w = 3;
		r.h = 3;

		SDL_RenderDrawRect(renderer, &r);
	}
}
void oota()
{
	SDL_Event event;
	while( 1 ) {
		SDL_PollEvent( &event );
		if (event.type == SDL_KEYDOWN) {
			SDL_Quit();
			break;
		}
	}
}

int the_kepi(SDL_Renderer* renderer, int leveys, int korkeus)
{
	int num_kehap, i, tmp;
	struct piste kolmio[3];
	int len[3], len_keha;
	int num_valipts[3];
	struct piste *pisteet[3] = { NULL };
	SDL_Event event;
	//int ret;

//	while (1) {

	len_keha = 0;
	tmp = 0;
	if (pisteet[0])
		for (i = 0; i < 3; i++)
			free(pisteet[i]);

	memset(&kolmio[0], 0, sizeof(kolmio));
	num_kehap = 0;
	memset(&len, 0, sizeof(len));
	memset(&num_valipts,0 ,sizeof(num_valipts));;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
		
	//SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	SDL_Delay(1000);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);

	arvo_kolmio(renderer, &kolmio[0], leveys, korkeus);
	num_kehap = (9 + rand() % MAX_KEHAP);

	for (i = 0; i < 3; i++) {
		struct jana tmp;

		tmp.alku = kolmio[i];
		tmp.loppu = kolmio[(i + 1) % 3];
		len[i] = jana_pituus(&tmp);
		len_keha += len[i];
	}
	for (i = 0; i < 3; i++) {
		/* Bah. We should not do even distribution but the same amount of pts for every wall */
		num_valipts[i] = num_kehap / 3;
		tmp += num_valipts[i];
	}
	num_kehap = tmp;

	for (i = 0; i < 3; i++) {
		laske_pts(kolmio[i], kolmio[(i + 1) % 3], len[i], num_valipts[i], /* kulmakerroin[i],*/ &pisteet[i]);
		piirra_pts(renderer, pisteet[i], num_valipts[i] + 1);
	}
	/* Just lets see what this does */
	SDL_RenderPresent(renderer);

	for (i = 0; i < num_valipts[0]; i++) {
		struct piste *alkup, *loppup;
//		struct timespec tm = {.tv_sec = 0, .tv_nsec = 10000000 };

		alkup = &pisteet[0][i];
		loppup = &pisteet[1][i];

		SDL_RenderDrawLine(renderer, alkup->x, alkup->y, loppup->x, loppup->y);

		alkup = &pisteet[1][i];
		loppup = &pisteet[2][i];

		SDL_RenderDrawLine(renderer, alkup->x, alkup->y, loppup->x, loppup->y);

		alkup = &pisteet[2][i];
		loppup = &pisteet[0][i];

		SDL_RenderDrawLine(renderer, alkup->x, alkup->y, loppup->x, loppup->y);
		SDL_RenderPresent(renderer);
//		nanosleep(&tm, NULL);
		SDL_Delay(10);
	}

/*
	SDL_PollEvent( &event );
	if (event.type == SDL_KEYDOWN) {
		SDL_Quit();
		break;
	}
*/
	oota();
//	}

	return 0;
}
