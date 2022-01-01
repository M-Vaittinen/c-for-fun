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

static SDL_Renderer* grdr;

void arvo_kolmio(struct piste *k, int max_x, int max_y)
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

	SDL_SetRenderDrawColor(grdr, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(grdr);
	SDL_SetRenderDrawColor(grdr, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(grdr, k[0].x, k[0].y, k[1].x, k[1].y);
	SDL_RenderDrawLine(grdr, k[1].x, k[1].y, k[2].x, k[2].y);
	SDL_RenderDrawLine(grdr, k[2].x, k[2].y, k[0].x, k[0].y);
	SDL_SetRenderDrawColor(grdr, 255, 0, 255, SDL_ALPHA_OPAQUE);
//	SDL_RenderPresent(grdr);

//	sleep(1);

	printf("alkukolmio (%d, %d), (%d, %d), (%d, %d)\n", k[0].x, k[0].y,
							    k[1].x, k[1].y,
							    k[2].x, k[2].y);
}
/*
int laske_seuraava(double kulmakerroin, int pit, struct piste *p)
{
	struct piste *uusi = p + 1;
	long double tmp_x;

	tmp_x = sqrtl( ((long double)(pit * pit) / (kulmakerroin * kulmakerroin + 1.0)));
	uusi->x = (int)tmp_x + p->x;
	uusi->y = kulmakerroin * tmp_x + p->y;

	return 0;
}
*/
int laske_pts(struct piste kylki1, struct piste kylki2, int kokopit, int num_vali, double kulmakerroin, struct piste **pisteet)
{
	struct piste *p;
	int i;
	double valip;

	*pisteet = calloc(sizeof(struct piste), num_vali + 1);
	if (!*pisteet)
		return -ENOMEM;

	p = *pisteet;

	for (i = 0; i < num_vali; i++) {
		p[i].x = (double)i * (kylki2.x - kylki1.x) / (double)num_vali + kylki1.x;
		p[i].y = (double)i * (kylki2.y - kylki1.y) / (double)num_vali + kylki1.y;
	}
	p[num_vali] = kylki2;


	//*p = kylki1;

//	for (i = 0; i < num_vali; i++)
	//	laske_seuraava(kulmakerroin, kokopit/num_vali, &p[i]);

	return 0;
}

void piirra_pts(SDL_Renderer* renderer, struct piste *p, int lkm)
{
	int i;

//	SDL_SetRenderDrawColor(renderer, 255, 0 , 255, SDL_ALPHA_OPAQUE);

	printf("pisteita %d\n", lkm);
	for (i = 0; i < lkm; i++) {
		SDL_Rect r;

		r.x = p[i].x;
		r.y = p[i].y;
		r.w = 3;
		r.h = 3;

	//	printf("piste[%d]  (%d,%d)\n", i, r.x, r.y);

		SDL_RenderDrawRect(renderer, &r);
	}
//	sleep(1);
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
	int num_kehap, i, tmp = 0;
	struct piste kolmio[3];
	int len[3], len_keha = 0;
	int num_valipts[3];
	struct piste *pisteet[3];
	double kulmakerroin[3];
	int valistys;

	grdr = renderer;

	arvo_kolmio(&kolmio[0], leveys, korkeus);
	num_kehap = (9 + rand() % MAX_KEHAP);

	for (i = 0; i < 3; i++) {
		struct jana tmp;

		tmp.alku = kolmio[i];
		tmp.loppu = kolmio[(i + 1) % 3];
		len[i] = jana_pituus(&tmp);
		kulmakerroin[i] = jana_kulmakerroin(&tmp);
		len_keha += len[i];
	}
	for (i = 0; i < 3; i++) {
		/* Bah. We should not do even distribution but the same amount of pts for every wall */
		//num_valipts[i] = len[i] * num_kehap / len_keha;
		num_valipts[i] = num_kehap / 3;
		tmp += num_valipts[i];
	}
	num_kehap = tmp;

	for (i = 0; i < 3; i++) {
		printf("vali %d jako %d pts\n", i, num_valipts[i]);
		laske_pts(kolmio[i], kolmio[(i + 1) % 3], len[i], num_valipts[i], kulmakerroin[i], &pisteet[i]);
		piirra_pts(renderer, pisteet[i], num_valipts[i] + 1);
	}
	SDL_RenderPresent(renderer);

	/* Just lets see what this does */

	for (i = 0; i < num_valipts[0]; i++) {
		struct piste *alkup, *loppup;
		struct timespec tm = {.tv_sec = 0, .tv_nsec = 10000000 };

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
		nanosleep(&tm, NULL);
	}


	oota();

	return 0;
}
