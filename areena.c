#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "areena.h"
#include "helpers.h"
#include "piirrettavat_tekstit.h"


static const char *rikkopts_str = "100";

static int alusta_seinat(struct areena *a)
{
	int i;
	struct SDL_Color v = {
		.r = 255,
		.g = 255,
		.b = 255,
		.a = SDL_ALPHA_OPAQUE,
	};
	struct paikka alut[] = {
		{ .x = 0, .y =  0},
		{ .x = a->leveys, .y =  0 },
		{ .x = a->leveys, .y = a->korkeus },
		{ .x = 0, .y = a->korkeus },
	};
	struct paikka loput[] = {
		{ .x = a->leveys, .y = 0 },
		{ .x = a->leveys, .y = a->korkeus },
		{ .x = 0, .y = a->korkeus },
		{ .x = 0, .y = 0 },
	};

	if (a->seinien_maara != 4) {
		printf("outo seinamaara %d\n", a->seinien_maara);
		return -EINVAL;
	}

	for (i = 0; i < 4; i++)
		alusta_seina(&a->seinat[i], &alut[i], &loput[i], &v);

	return 0;
}


int piirra_areena(struct areena *a)
{
	int i;

	for (i = 0; i < a->seinien_maara; i++)
		a->seinat[i].piirra(a, &a->seinat[i]);

	for (i = 0; i < a->alusten_maara; i++)
		a->alukset[i].piirra(a, &a->alukset[i]);

	hanskaa_pupit(a);
	piirra_tekstit(a);

	if (a->stop) {
		char pisteet[255];
		SDL_Color valk = {255, 255, 255, SDL_ALPHA_OPAQUE/2 };
		struct paikka p = { .x = a->leveys/2 - 50,
				    .y = a->korkeus/2 -50, };

		snprintf(pisteet, 255, "%u", a->pisteet);
		SDL_Log("Pisteita palajo? %s\n", pisteet);

		draw_text(a, pisteet, &p, 200, 200, &valk);
		SDL_RenderPresent(a->p.renderer);
		return -1;
	}
	return 0;
}

int luo_areena(struct areena *a)
{
	int ok;

	memset(&a->pups[0], 0, sizeof(a->pups));
	a->active_pups = 0;
	a->stop = 0;
	a->realstop = 0;
	a->seinien_maara = 4;
	a->seinat = calloc(a->seinien_maara, sizeof(*a->seinat));
	if (!a->seinat) {
		printf("Muisti loppu\n");
		return -ENOMEM;
	}
	ok = alusta_seinat(a);
	if (ok) {
		printf("Seinen alustus män mönkään\n");
		return -1;
	}
	a->piirra = piirra_areena;
	return 0;
}


void lisaa_rikkopisteet(struct areena *ar, struct alus *oma)
{
	struct pirrettava_teksti *pt = varaa_piirrospaikka();

	ar->pisteet += 100;
	if (!pt) {
		SDL_Log("Piirrospooli täys\n");
		return;
	}
	else
		SDL_Log("Lisataan rikkopts %p\n", rikkopts_str );

	pt->teksti = rikkopts_str;
	SDL_Log("Added str %s %p\n",pt->teksti, pt->teksti);
	pt->nakyvilla_kierros = 20;
	pt->p = oma->p;
	pt->leveys = ar->leveys/20;
	pt->korkeus=ar->korkeus/20;
	pt->kokomuutos_kierroksia = 20;
	pt->kokomuutos_x_kierros = 0;
	pt->kokomuutos_y_kierros = 0;
	pt->v.r = 255;
	pt->v.g = 255;
	pt->v.b = 255;
	pt->v.a = 255;
}

int arvo_powerup(struct areena *ar)
{
	if (ar->active_pups < MAX_PUPS) {
		unsigned long long chance = rand() %UUDEN_PUPIN_TSAANNSSI;

		if (!chance) {
			/* Arpa suosi, tehdään uus poweruppi */

			int i;
			struct powerup *p = &ar->pups[0];
			struct paikka pa;

			for (i = 0; i < MAX_PUPS && p->expire; i++, p++);

			if (i == MAX_PUPS) {
				SDL_Log("Viiiirrhheita Koodissa\n");
				return -1;
			}
			ar->active_pups++;
			pa.x = PUPS_KOKO + (rand() % ar->leveys) - 2*PUPS_KOKO;
			pa.y = PUPS_KOKO + (rand() % ar->korkeus) - 2*PUPS_KOKO;
			create_random_powerup_to_place(p, PUPS_KOKO, &pa);
		}
	}
	return 0;
}

void copy_server_pup_to_client(struct powerup_server_data *s, struct powerup *c)
{
	s->koko = c->koko;
	s->p = c->p;
	s->tyyppi = c->tyyppi;
	s->nappauspisteet = c->nappauspisteet;
}

void copy_server_alus_to_client(struct alus_server_data *sa, struct alus *ca)
{
	ca->id = sa->id;
	ca->p = sa->p;
	ca->p_delta = sa->p_delta;
	ca->coll_min = sa->coll_min;
	ca->coll_max = sa->coll_max;
	ca->suunta = sa->suunta;
	ca->nopeus = sa->nopeus;
	ca->pituus = sa->pituus;
	ca->leveys = sa->leveys;
	ca->rikki = sa->rikki;
}

void update_areena_by_serverdata(struct areena *a, struct areena_server_data *sd,
				 int id)
{
	int my_points, i;

	if (a->prev_server_update_c.tv_sec == sd->last_server_update_c.tv_sec &&
	    a->prev_server_update_c.tv_nsec == sd->last_server_update_c.tv_nsec)
		return;

	if (!sd->initialized) {
		printf("Uninitialized server data\n");
		return;
	}
	/* TODO: Get client side time */
	if (id == 0)
		my_points = sd->pisteet_id1;
	else
		my_points = sd->pisteet_id2;

	a->pisteet = my_points;
	a->alusten_maara = sd->alusten_maara;

	if (id == 0) {
		copy_server_alus_to_client(&sd->alukset[0], &a->alukset[0]);
		copy_server_alus_to_client(&sd->alukset[1], &a->alukset[1]);
	} else {
		copy_server_alus_to_client(&sd->alukset[1], &a->alukset[0]);
		copy_server_alus_to_client(&sd->alukset[0], &a->alukset[1]);
	}
	for (i = 2; i < sd->alusten_maara && i < ALUKSET_MAX; i++)
		copy_server_alus_to_client(&sd->alukset[i], &a->alukset[i]);

	a->active_pups = sd->active_pups;
	for (i = 0; i < a->active_pups && i < MAX_PUPS; i++)
		copy_server_pup_to_client(&sd->pups[i], & a->pups[i]);
}

