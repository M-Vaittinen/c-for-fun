#include <time.h>
#include <errno.h>
#include "areena.h"
#include "powerup.h"
#include "common.h"
#include "piirrettavat_tekstit.h"
#include "helpers.h"

/* Montako kierrosta powerupin teksti näkyy */
#define LOOPS_TO_SHOW_PUP_TEXT 150

void piirra_pup(struct areena *a, struct powerup* pup)
{
	struct paikka p;

	p.x = pup->p.x + a->leveys_offset;
	p.y = pup->p.y + a->korkeus_offset;
	DrawCircle(a->p.renderer, &p, pup->koko, &pup->vri);
}

void putsaa_pupit(struct areena *ar)
{
	int i;

	for (i = 0; i < MAX_PUPS; i++)
		ar->pups[i].expire = 0;
	ar->active_pups = 0;
}

int create_powerup_to_place(struct powerup *p, int koko, struct paikka *pa,
			    int tyyppi)
{
	struct SDL_Color v = {255, 0, 200, SDL_ALPHA_OPAQUE};

	if (tyyppi < 0 || tyyppi >= PUP_TYYPIT)
		return -EINVAL;
	p->koko = koko;
	p->p = *pa;
	p->tyyppi = tyyppi;
	p->nappauspisteet = nappauspisteet[p->tyyppi];
	p->vri = v;
	p->piirra = piirra_pup;
	p->expire = time(NULL) + 5;

	return 0;
}

int create_random_powerup_to_place(struct powerup *p, int koko, struct paikka *pa)
{
	int tyyppi;

	tyyppi =  rand() % PUP_TYYPIT;
	return create_powerup_to_place(p, koko, pa, tyyppi);
}

bool pup_napattu(struct alus *a, struct powerup *pup)
{
	return (nurkka_ympyrassa(&a->corners.etunurkka, &pup->p, pup->koko) ||
		nurkka_ympyrassa(&a->corners.vas_takanurkka, &pup->p, pup->koko) ||
		nurkka_ympyrassa(&a->corners.oik_takanurkka, &pup->p, pup->koko));
}

void lisaa_puptxt_piirtoon(struct powerup *pup)
{
	struct pirrettava_teksti *pt = varaa_piirrospaikka();

	if (!pt) {
		SDL_Log("Piirrospooli täys\n");
		return;
	}
	else
		SDL_Log("Lisataan puptxt\n");

	pt->teksti = puppi_txt_arr[pup->tyyppi];
	pt->nakyvilla_kierros = LOOPS_TO_SHOW_PUP_TEXT;
	pt->p = pup->p;
	pt->leveys = 120;
	pt->korkeus = 50;
	pt->kokomuutos_kierroksia = 5;
	pt->kokomuutos_x_kierros = 6;
	pt->kokomuutos_y_kierros = 3;
	pt->v = pup->vri;
}

void poista_vanhat_pupit(struct alus *a)
{
	time_t aika = time(NULL);
	uint8_t i;

	for (i = a->pups.first; i != a->pups.last; i++) {
		if (a->pups.pbuf[i].expire < aika)
			a->pups.first++;
		else
			break;
	}
}

void pup_pisteet(int *pts, struct powerup *pup)
{
	*pts += pup->nappauspisteet;
}

void soita_puppinappaus(struct areena *ar, struct powerup *pup)
{
	Mix_Chunk * snd = ar->s.pupaanet[pup->tyyppi];

	if (snd)
		Mix_PlayChannel( -1, snd, 0 );
}
/*
static void lisaa_puptieto(struct alus *a, struct powerup *pup)
{
	struct puppi *uusi;

	if (a->pups.last-a->pups.first == sizeof(a->pups.pbuf)-1) {
		SDL_Log("puppivarasto taynna (first = %d, last = %d\n",
			a->pups.first, a->pups.last);
		return;
	}
	if (a->pups.last == 255)
		SDL_Log("Puppipuskuri ympari\n");

	if (a->pups.first == 255)
		SDL_Log("Puppipuskuri ympari 2\n");

	uusi = &a->pups.pbuf[a->pups.last];
	a->pups.last++;

	uusi->tyyppi = pup->tyyppi;
	uusi->expire = time(NULL) + POWERUP_VAIKUTUSAIKA;
}
*/

void kato_pupit(int *active_pups, struct puppipuskuri *pups, unsigned int *pts, struct areena *ar)
{
	//int i;

	if (!*active_pups)
		return;
#if 0
	for (i = 0; i < MAX_PUPS; i++)
		if (pups[i].expire)
			if (pup_napattu(a, &pups[i])) {
				lisaa_puptieto(a, &pups[i]);
				lisaa_puptxt_piirtoon(&pups[i]);
				pups[i].expire = 0;
				*active_pups -= 1;
				/* TODO: Increase points somewhere outside ?
				 * Now points are only increased at cloent side
				 * and will be overwritten by server data
				 * Points hould be increased at server side, right?
				 * OTOH - we could leave points to be fully
				 * handled at client side. Server does not store
				 * points now so cheating does not really matter
				 */
				if (pts)
					pup_pisteet(pts, &>pups[i]);
				if (ar)
					soita_puppinappaus(ar, &>pups[i]);
			}

	return;
#endif
}

//bool mun_pupit(struct alus *a, int tyyppi)
bool mun_pupit(struct puppipuskuri *pups, int tyyppi)
{
	uint8_t i;
	if (tyyppi >= PUP_TYYPIT)
		return false;

	for (i = pups->first; i != pups->last; i++)
		if (pups->pbuf[i].tyyppi == tyyppi)
			return true;
	return false;
}

void hanskaa_pupit(struct areena *ar)
{
	int i;
	time_t now = time(NULL);

	if (!ar->active_pups)
		return;

	for (i = 0; i < MAX_PUPS; i++) {
		if ( ar->pups[i].expire < now) {
			if (ar->pups[i].expire)
				ar->active_pups--;
			ar->pups[i].expire = 0;
			continue;
		}
		ar->pups[i].piirra(ar, &ar->pups[i]);
	}
}

