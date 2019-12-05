#include "alus.h"
#include "powerup.h"
#include "seina.h"
#include "media.h"
#include "helpers.h"

static void luo_alus(struct alus *a, float leveys, float pituus, struct paikka *p,
	      float suunta, int nopeus, struct SDL_Color *v);

static void coll_update(struct alus *a, struct paikka *p)
{
	a->coll_max.x = MAX(a->coll_max.x, p->x);
	a->coll_max.y = MAX(a->coll_max.y, p->y);
	a->coll_min.x = MIN(a->coll_min.x, p->x);
	a->coll_min.y = MIN(a->coll_min.y, p->y);
}


static void alusta_oma_alus(struct areena *a)
{
	struct SDL_Color v = ALUS_OLETUS_VARI;
	struct paikka p;

	p.x = a->leveys/2;
	p.y = a->korkeus/2;

	luo_alus(&a->alukset[0], ALUS_OLETUS_LEVEYS, ALUS_OLETUS_PITUUS, &p,
		 ALUS_OLETUS_SUUNTA, ALUS_OLETUS_NOPEUS, &v);
	a->alukset[0].oma = 1;
}

void loppu_punaa(struct areena *ar)
{
	struct alus *oma = &ar->alukset[0];
	oma->vri.r = 150;
	oma->vri.g = 0;
	oma->vri.b = 0;
	ar->stop = 1;
	Mix_PlayChannel( -1, ar->s.crash, 0);
}

static void arvo_alus(struct areena *a, int index)
{
	struct SDL_Color v = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	struct paikka p;
	float lev, pit, suunta;
	int nop;
	float koko = (float) (rand() % 20);
	struct alus *al = &a->alukset[index];

	if (index >= ALUKSET_MAX) {
		SDL_Log("Ouch! Bad index %d\n", index);
		return;
	}
uus:
	p.x = 1 + (rand() % a->leveys) - 2;
	p.y = 1 + (rand() % a->korkeus) - 2;
	lev = 5 + koko;
	pit = 10 + koko*2;
	suunta = (float)(rand() % 360);
	nop = rand() % NOP_MAX;

	luo_alus(al, lev, pit, &p, suunta, nop, &v);

	if (o_iholla(&a->alukset[0], al))
		goto uus;
}

int luo_alukset(struct areena *a)
{
	int i;

	a->alusten_maara = 20;

	alusta_oma_alus(a);

	for (i = 1; i < a->alusten_maara; i++)
		arvo_alus(a, i);

	return 0;
}

void lisaa_alus(struct areena *a)
{
	if (a->alusten_maara == ALUKSET_MAX)
		return;
	arvo_alus(a,a->alusten_maara);
	a->alusten_maara++;
	Mix_PlayChannel( -1, a->s.new_ship, 0 );
}

void alus_laske_nurkat(struct alus *a)
{
	float change_x;
	float change_y;
	float pit, pit_nurkka;
	float lev;
	float angle = a->suunta * M_PI / 180.0f;

	float tangle;

	/* Paikat ennen kiertoa */
	pit = a->pituus/2.0;
	lev = a->leveys/2.0;
	pit_nurkka = sqrtf(pit*pit + lev*lev);

	tangle = atan(a->leveys/a->pituus);

	/* Kierrä suunnan mukaan */

	change_y = sinf(angle) * pit;
	change_x = cosf(angle) * pit;

	a->etunurkka.x = a->p.x + change_x;
	a->etunurkka.y = a->p.y + change_y;

	a->coll_max.x = a->coll_min.x = a->etunurkka.x;
	a->coll_max.y = a->coll_min.y = a->etunurkka.y;

	change_y = sin(M_PI + tangle + angle) * pit_nurkka;
	change_x = cos(M_PI + tangle + angle) * pit_nurkka;

	a->oik_takanurkka.x = a->p.x + change_x;
	a->oik_takanurkka.y = a->p.y + change_y;

	coll_update(a, &a->oik_takanurkka);

	change_y = sin(M_PI - tangle + angle) * pit_nurkka;
	change_x = cos(M_PI - tangle + angle) * pit_nurkka;

	a->vas_takanurkka.x = a->p.x + change_x;
	a->vas_takanurkka.y = a->p.y + change_y;

	coll_update(a, &a->vas_takanurkka);
}

void piirra_alus(struct areena *ar, struct alus *a)
{
	struct seina s;
	struct SDL_Color *v;
	struct SDL_Color v_nopee = VARI_NOPPEE;
	struct SDL_Color v_upee = VARI_UPPEE;
	struct SDL_Color v_kuolematon = VARI_KUOLEMATON;
	struct SDL_Color v_haamu = VARI_HAAMU;
	struct SDL_Color v_rikkova = VARI_RIKKOVA;
	struct SDL_Color v_jaassa = VARI_JAASSA;


	v = &a->vri;

	if (oonko_uppee(a))
		v = &v_upee;
	if (oonko_noppee(a))
 		v = &v_nopee;
	if (oonko_haamu(a))
		v = &v_haamu;
	if (oonko_rikkova(a))
		v = &v_rikkova;
	if (oonko_kuolematon(a))
		v = &v_kuolematon;
	if (oonko_jaassa(a))
		v = &v_jaassa;


	alusta_seina(&s, &a->vas_takanurkka, &a->oik_takanurkka, v);
	s.piirra(ar, &s);
	alusta_seina(&s, &a->oik_takanurkka, &a->etunurkka, v);
	s.piirra(ar, &s);
	alusta_seina(&s, &a->etunurkka, &a->vas_takanurkka, v);
	s.piirra(ar, &s);
/*
	SDL_SetRenderDrawColor(renderer, 0,255,0,SDL_ALPHA_OPAQUE);
	SDL_RenderDrawPoint(renderer, a->p.x+1, a->p.y+1);
	SDL_RenderDrawPoint(renderer, a->p.x+1, a->p.y);
	SDL_RenderDrawPoint(renderer, a->p.x+1, a->p.y-1);
	SDL_RenderDrawPoint(renderer, a->p.x-1, a->p.y);
	SDL_RenderDrawPoint(renderer, a->p.x-1, a->p.y+1);
	SDL_RenderDrawPoint(renderer, a->p.x-1, a->p.y-1);
*/
}

static void luo_alus(struct alus *a, float leveys, float pituus, struct paikka *p,
	      float suunta, int nopeus, struct SDL_Color *v)
{
	memset(a, 0, sizeof(*a));
	a->oma = 0;
	a->leveys = leveys;
	a->pituus = pituus;
	a->suunta = suunta;
	a->p = *p;
	a->nopeus = nopeus;
	a->vri = *v;
	a->piirra = piirra_alus;
	alus_laske_nurkat(a);
}

void pysayta_alus(struct alus *a)
{
	a->vri.r = 255;
	a->vri.g = 0;
	a->vri.b = 0;
	a->nopeus = 0;
}





