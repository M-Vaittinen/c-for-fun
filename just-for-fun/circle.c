#include <errno.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>

#include "fun.h"
#include "circle.h"
#include "piste.h"

#define SADE_MAX 500
#define SADE_MIN 100

struct ympyra {
	struct piste kp;
	int sade;
	char r;
	char g;
	char b;
};

void arvo_ympyran_varit(struct ympyra *y)
{
uus:
	y->r = rand() % 255;
	y->g = rand() % 255;
	y->b = rand() % 255;

	if (y->r < 20 && y->g < 20 && y->b < 20)
		goto uus;
}

void arvo_ympyra(struct ympyra *y, int leveys, int korkeus)
{
	int max_y, min, max_x;

	y->sade = (rand() % (SADE_MAX - SADE_MIN)) + SADE_MIN;

	min = y->sade * 2 + 1;
	max_y = korkeus - min;
	max_x = leveys - min;
	y->kp.x = (rand() % (max_x - min)) + min;
	y->kp.y = (rand() % (max_y - min)) + min;
	arvo_ympyran_varit(y);
}

void piirra_ympyra(SDL_Renderer* renderer, struct ympyra *y)
{
	circleRGBA(renderer, y->kp.x, y->kp.y, y->sade, y->r, y->g, y->b, SDL_ALPHA_OPAQUE);
}

void kehaympyra(struct ympyra *y, struct ympyra *y2, int degree)
{
	int matka = y->sade;
	struct piste *p = &y->kp;

	y2->kp.x = p->x + (sin(degree)*matka);
	y2->kp.y = p->y + (cos(degree)*matka);

	arvo_ympyran_varit(y2);
}

void piirra_kehaympyrat(SDL_Renderer* renderer, struct ympyra *y,
			struct ympyra *y2, int split)
{
	int i;

	/* Piira keskiympyra */
	piirra_ympyra(renderer, y);

	/* Piirra kehaympyrat splittin asti */
	for (i = 0; i < split; i++) {
		piirra_ympyra(renderer, &y2[i]);
	}
}

void calc_kehaympyrat(struct ympyra *y, struct ympyra *y2, int deg, int sp)
{
	int i;

	for (i = 0; i < sp; i++)
		kehaympyra(y, &y2[i], i * deg);
}

int circle(SDL_Renderer* renderer, int leveys, int korkeus, int kertaa, bool all)
{
	struct ympyra *y, **y2;
	int *splits, *degree, i, j;

	y = calloc(kertaa, sizeof(*y));
	y2 = calloc(kertaa, sizeof(*y2));
	splits = calloc(kertaa, sizeof(*splits));
	degree = calloc(kertaa, sizeof(*degree));
	if (!y || !y2 || !splits || !degree)
		return -ENOMEM;

	for (j = 0; j < kertaa; j++) {

		arvo_ympyra(&y[j], leveys, korkeus);

		/* We divide the circle to 15 ... 360 pieces */
		splits[j] = rand() % (360 - 15);
		splits[j] += 15;

		/* But we want to ensure 360/split is even */
		degree[j] = 360 / splits[j];
		splits[j] = 360 / degree[j];

		y2[j] = calloc(splits[j], sizeof(**y2));
		if (!y2[j])
			return -ENOMEM;

		y2[j][0].sade = rand() % SADE_MAX;
		if ( y2[j][0].sade < 20)
			y2[j][0].sade = 20;

		for(i = 1; i < splits[j]; i++)
			y2[j][i].sade = y2[j][0].sade;

		calc_kehaympyrat(&y[j], y2[j], degree[j], splits[j]);
	}

	for ( i = 0; i < kertaa; i++) {
		int loops_for_me;

		for (loops_for_me = 0; loops_for_me < splits[i]; loops_for_me++) {
			SDL_Event event;

			for (j = 0; j < i; j++)
				piirra_kehaympyrat(renderer, &y[j], y2[j], splits[j]);

			SDL_PollEvent( &event );
			if (event.type == SDL_KEYDOWN)
				return ALL_OK;

			piirra_kehaympyrat(renderer, &y[i], y2[i], loops_for_me);
			SDL_Delay(50);
			SDL_RenderPresent(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);
		}

	}
	for (i = 0; i < kertaa; i++)
		free(y2[i]);
	free(y);
	free(y2);
	free(splits);
	free(degree);

	if (!all)
		SDL_Delay(5000);

	return 0;
}
