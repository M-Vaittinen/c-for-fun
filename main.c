#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "paikka.h"
#include "seina.h"
#include "alus.h"
#include "media.h"
#include "areena.h"
#include "common.h"
#include "hiscore.h"

#define WINDOW_X (640*2)
#define WINDOW_Y (480*2)
#define MOVE_THRESH 10
#define MOVE_JUMP_THRESH 6000
#define LOOPS_TO_SHOW_PUP_TEXT 150

#define PISTEET_NAKYY_KIERROSTA 75
#define PISTEKOON_PIENENNYS_ALUSSA 150
#define PISTEKOON_MUUTOS_STEPPI 10
#define PISTEKOKO_SAMANA_LOOPIT 5

//5 ms
#define LOOP_DELAY_US 5000
#define NOP_MAX 36000*2

int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct vari *v);

bool mun_pupit(struct alus *a, int tyyppi);

#define oonko_noppee(a) mun_pupit((a), PUP_SPEED)
#define oonko_uppee(a) mun_pupit((a), PUP_COOL)
#define oonko_kuolematon(a) mun_pupit((a), PUP_IMMORTAL)
#define oonko_rikkova(a) mun_pupit((a), PUP_DESTROY)
#define oonko_jaassa(a) mun_pupit((a), PUP_FREEZE)
#define oonko_haamu(a) mun_pupit((a), PUP_PASS_WALLS)

#define MIN(a,b) ((a)<=(b))?(a):(b)
#define MAX(a,b) ((a)>=(b))?(a):(b)

int music_init(struct sounds *s)
{
	int ret;

	memset(s,0,sizeof(*s));

	//Initialize SDL_mixer
	if( (ret = Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ))
		return ret;    

	/* Taustamusiikki */
	s->music = Mix_LoadMUS( "/home/mvaittin/.kolomiosnd/Blazer Rail.wav" );
	/* pisteet 500 */
	s->points = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/glass_breaking_2.wav" );
	/* törmäys */
	s->crash = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/kolmiopelihavio.wav" );
	/* Spawnaus */
	s->new_ship = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/bottle_pop_2.wav" );
	s->pupaanet[PUP_SPEED] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/NOPEUS.wav");
	s->pupaanet[PUP_COOL] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/UPPEE.wav");
	s->pupaanet[PUP_IMMORTAL] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/tiktok.wav");
	s->pupaanet[PUP_DESTROY] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/nauru.wav");
	s->pupaanet[PUP_FREEZE] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/");
	s->pupaanet[PUP_PASS_WALLS] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/WARP.wav");
	
	return !(s->music && s->new_ship && s->crash && s->points);
}

bool isin_kolmio(struct paikka *a, struct paikka *b, struct paikka *c,
		struct paikka *p)
{
	long long int fAB = ((long long int)(p->y-a->y)) *
			    ((long long int)(b->x-a->x)) -
			    ((long long int)(p->x-a->x)) *
			    ((long long int)(b->y-a->y));
	long long int fCA = ((long long int)(p->y-c->y)) *
			    ((long long int)(a->x-c->x)) -
			    ((long long int)(p->x-c->x)) *
			    ((long long int)(a->y-c->y));
	long long int fBC = ((long long int)(p->y-b->y)) *
			    ((long long int)(c->x-b->x)) -
			    ((long long int)(p->x-b->x)) *
			    ((long long int)(c->y-b->y));

	return ((fAB*fBC > 0) && (fBC*fCA > 0));
}



void draw_text(struct piirrin *pr, const char *text, struct paikka *p, int w, int h, struct SDL_Color *v)
{
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	SDL_Rect Message_rect;

	SDL_SetRenderDrawColor(pr->renderer, v->r, v->g, v->b, v->a);
	surfaceMessage = TTF_RenderText_Blended(pr->font, text, *v);
	if (!surfaceMessage)
		SDL_Log("Surface ei surffannu %s\n", SDL_GetError());

	Message = SDL_CreateTextureFromSurface(pr->renderer, surfaceMessage);
	if (!Message)
		SDL_Log("Viesti ei pullottunu\n");

	SDL_FreeSurface(surfaceMessage);

	SDL_SetTextureBlendMode(Message, SDL_BLENDMODE_BLEND);
	Message_rect.x = p->x;
	Message_rect.y = p->y;
	Message_rect.w = w;
	Message_rect.h = h;

	if (SDL_RenderCopy(pr->renderer, Message, NULL, &Message_rect)) {
		SDL_Log("Copy ei renderöityny\n");
	}
	SDL_DestroyTexture(Message);

}

int orientation(struct paikka *p, struct paikka *q, struct paikka *r) 
{ 
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	int val = (q->y - p->y) * (r->x - q->x) - 
		      (q->x - p->x) * (r->y - q->y); 
  
	if (val == 0)
	return 0;  // colinear 
  
	return (val > 0)? 1: 2; // clock or counterclock wise 
} 
 
// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(struct paikka *p, struct paikka *q, struct paikka *r) 
{ 
	if (q->x <= MAX(p->x, r->x) && q->x >= MIN(p->x, r->x) && 
		q->y <= MAX(p->y, r->y) && q->y >= MIN(p->y, r->y)) 
	   return true; 
  
	return false; 
} 
 
// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(struct paikka *p1, struct paikka *q1, struct paikka *p2, struct paikka *q2) 
{ 
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2); 
	int o2 = orientation(p1, q1, q2); 
	int o3 = orientation(p2, q2, p1); 
	int o4 = orientation(p2, q2, q1); 
  
	// General case 
	if (o1 != o2 && o3 != o4) 
		return true; 
  
	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
	 // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
	return false; // Doesn't fall in any of the above cases 
} 

int o_iholla(struct alus *a, struct alus *a2)
{
	return ((a->coll_min.x <= a2->coll_max.x) &&
		(a->coll_max.x >= a2->coll_min.x) &&
		(a->coll_min.y <= a2->coll_max.y ) &&
		(a->coll_max.y >= a2->coll_min.y));
}

int tormasi(struct alus *oma, struct alus *a)
{
	return (doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka,
			    &a->vas_takanurkka, &a->oik_takanurkka) ||
		doIntersect(&oma->oik_takanurkka, &oma->etunurkka,
			    &a->vas_takanurkka, &a->oik_takanurkka) ||
		doIntersect(&oma->etunurkka, &oma->vas_takanurkka,
			    &a->vas_takanurkka, &a->oik_takanurkka) ||

		doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka,
			    &a->oik_takanurkka, &a->etunurkka) ||
		doIntersect(&oma->oik_takanurkka, &oma->etunurkka,
			    &a->oik_takanurkka, &a->etunurkka) ||
		doIntersect(&oma->etunurkka, &oma->vas_takanurkka,
			    &a->oik_takanurkka, &a->etunurkka) ||

		doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka,
			    &a->etunurkka, &a->oik_takanurkka) ||
		doIntersect(&oma->oik_takanurkka, &oma->etunurkka,
			    &a->etunurkka, &a->oik_takanurkka) ||
		doIntersect(&oma->etunurkka, &oma->vas_takanurkka,
			    &a->etunurkka,  &a->oik_takanurkka));
}

void coll_update(struct alus *a, struct paikka *p)
{
	a->coll_max.x = MAX(a->coll_max.x, p->x);
	a->coll_max.y = MAX(a->coll_max.y, p->y);
	a->coll_min.x = MIN(a->coll_min.x, p->x);
	a->coll_min.y = MIN(a->coll_min.y, p->y);
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

void piirra_alus(SDL_Renderer* renderer, struct alus *a)
{
	struct seina s;
	struct vari *v;
	struct vari v_nopee = VARI_NOPPEE;
	struct vari v_upee = VARI_UPPEE;
	struct vari v_kuolematon = VARI_KUOLEMATON;
	struct vari v_haamu = VARI_HAAMU;
	struct vari v_rikkova = VARI_RIKKOVA;
	struct vari v_jaassa = VARI_JAASSA;


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


	alusta_seina(&s, &a->vas_takanurkka, &a->oik_takanurkka, v /* &v */);
	s.piirra(renderer, &s);
	alusta_seina(&s, &a->oik_takanurkka, &a->etunurkka, v);
	s.piirra(renderer, &s);
	alusta_seina(&s, &a->etunurkka, &a->vas_takanurkka, v);
	s.piirra(renderer, &s);
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

void piirra_seina(SDL_Renderer* renderer, struct seina *s)
{
	SDL_SetRenderDrawColor(renderer, s->vri.r, s->vri.g, s->vri.b, s->vri.alpha);
	if (SDL_RenderDrawLine(renderer, s->alku.x, s->alku.y, s->loppu.x, s->loppu.y)) {
		SDL_Log("Unable to draw line: %s", SDL_GetError()); 
	}
}

int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct vari *v)
{
	s->alku = *alku;
	s->loppu = *loppu;
	s->piirra = piirra_seina;
	s->vri = *v;

	return 0;
}

int alusta_seinat(struct areena *a)
{
	int i;
	struct vari v = {
		.r = 255,
		.g = 255,
		.b = 255,
		.alpha = SDL_ALPHA_OPAQUE,
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

static void DrawCircle(SDL_Renderer * renderer, struct paikka *centre, int32_t radius, struct vari *v)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

	SDL_SetRenderDrawColor(renderer, v->r, v->g, v->b, v->alpha);

   while (x >= y)
   {
	  //  Each of the following renders an octant of the circle
	  SDL_RenderDrawPoint(renderer, centre->x + x, centre->y - y);
	  SDL_RenderDrawPoint(renderer, centre->x + x, centre->y + y);
	  SDL_RenderDrawPoint(renderer, centre->x - x, centre->y - y);
	  SDL_RenderDrawPoint(renderer, centre->x - x, centre->y + y);
	  SDL_RenderDrawPoint(renderer, centre->x + y, centre->y - x);
	  SDL_RenderDrawPoint(renderer, centre->x + y, centre->y + x);
	  SDL_RenderDrawPoint(renderer, centre->x - y, centre->y - x);
	  SDL_RenderDrawPoint(renderer, centre->x - y, centre->y + x);

	  if (error <= 0)
	  {
		 ++y;
		 error += ty;
		 ty += 2;
	  }

	  if (error > 0)
	  {
		 --x;
		 tx += 2;
		 error += (tx - diameter);
	  }
   }
}
void piirra_pup(SDL_Renderer* renderer, struct powerup* pup)
{
	/* TODO: lisaa vari */
	DrawCircle(renderer, &pup->p, pup->koko, &pup->vri);
}

static void putsaa_pupit(struct areena *ar)
{
	int i;

	for (i = 0; i < MAX_PUPS; i++)
		ar->pups[i].expire = 0;
	ar->active_pups = 0;
}

static void hanskaa_pupit(struct areena *ar)
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
		ar->pups[i].piirra(ar->p.renderer, &ar->pups[i]);
	}
}

void piirra_puppiteksti(struct areena *ar, struct puppi *p)
{
	struct SDL_Color v;

	if (p->piirretty) {
		int kokokerroin = LOOPS_TO_SHOW_PUP_TEXT;
		if (p->piirretty == LOOPS_TO_SHOW_PUP_TEXT) {
			snprintf(p->teksti, PUPPITXT_MAX, "+%u %s", p->lisapisteet, p->teksti_alustava);
			p->teksti[PUPPITXT_MAX-1] = 0;
		}
		v.r = p->vri.r;
		v.g = p->vri.g;
		v.b = p->vri.b;
		v.a = p->vri.alpha;

		kokokerroin -= (p->piirretty /5 * 5);
		draw_text(&ar->p, p->teksti, &p->p, 120 + kokokerroin, 50 + kokokerroin, &v);
		p->piirretty--;
	}
}

/* Tää ei toimi jos puskuri on täys */
#define jokaselle_saadulle_powerupille(a,p) \
for ((p) = &(a)->pups.pbuf[(a)->pups.first]; (a)->pups.first != (a)->pups.last && (p) != &(a)->pups.pbuf[(a)->pups.last + 1]; (p)++)

int piirra_areena(struct areena *a)
{
	int i;
	struct puppi *p;

	for (i = 0; i < a->seinien_maara; i++)
		a->seinat[i].piirra(a->p.renderer, &a->seinat[i]);

	jokaselle_saadulle_powerupille(&a->alukset[0], p)
		if (p->piirretty) {
			p->piirra(a, p);
		}

	for (i = 0; i < a->alusten_maara; i++)
		a->alukset[i].piirra(a->p.renderer, &a->alukset[i]);

	hanskaa_pupit(a);

	if (a->stop) {
		char pisteet[255];
		SDL_Color valk = {255, 255, 255, SDL_ALPHA_OPAQUE/2 };
		struct paikka p = { .x = a->leveys/2 - 50,
				    .y = a->korkeus/2 -50, };

		snprintf(pisteet, 255, "%u", a->pisteet);
		SDL_Log("Pisteita palajo? %s\n", pisteet);

		draw_text(&a->p, pisteet, &p, 200, 200, &valk);
		SDL_RenderPresent(a->p.renderer);
		//sleep(3);
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
	a->leveys = WINDOW_X-2;
	a->korkeus = WINDOW_Y-2;
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

void luo_alus(struct alus *a, float leveys, float pituus, struct paikka *p,
	      float suunta, int nopeus, struct vari *v)
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

void arvo_alus(struct areena *a, int index)
{
	struct vari v = { 255, 255, 255, SDL_ALPHA_OPAQUE };
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

#define ALUS_OLETUS_PITUUS 40
#define ALUS_OLETUS_LEVEYS 25
//#define ALUS_OLETUS_VARI { 51, 245, 255, SDL_ALPHA_OPAQUE }
#define ALUS_OLETUS_VARI { 61, 245, 255, SDL_ALPHA_OPAQUE }
#define ALUS_OLETUS_SUUNTA 0
#define ALUS_OLETUS_NOPEUS NOP_MAX

void loppu_punaa(struct areena *ar)
{
	struct alus *oma = &ar->alukset[0];
	oma->vri.r = 150;
	oma->vri.g = 0;
	oma->vri.b = 0;
	ar->stop = 1;
	Mix_PlayChannel( -1, ar->s.crash, 0);
}

bool nurkka_ympyrassa(struct paikka *nurkka, struct paikka *kp, int sade)
{
	return ((nurkka->x <= kp->x + sade && nurkka->x >= kp->x - sade) &&
		(nurkka->y <= kp->y + sade && nurkka->y >= kp->y - sade));
}

bool pup_napattu(struct alus *a, struct powerup *pup)
{
	return (nurkka_ympyrassa(&a->etunurkka, &pup->p, pup->koko) ||
		nurkka_ympyrassa(&a->vas_takanurkka, &pup->p, pup->koko) ||
		nurkka_ympyrassa(&a->oik_takanurkka, &pup->p, pup->koko));
}

void lisaa_puptieto(struct alus *a, struct powerup *pup)
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
	uusi->piirra = piirra_puppiteksti;
	uusi->piirretty = LOOPS_TO_SHOW_PUP_TEXT;
	uusi->lisapisteet = pup->nappauspisteet;
	uusi->teksti_alustava = puppi_txt_arr[pup->tyyppi];
	uusi->p = pup->p;
	uusi->vri = pup->vri;
	a->pups.last++;

	uusi->tyyppi = pup->tyyppi;
	uusi->expire = time(NULL) + POWERUP_VAIKUTUSAIKA;
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

void pup_pisteet(struct areena *ar, struct powerup *pup)
{
	ar->pisteet += pup->nappauspisteet;
}

void soita_puppinappaus(struct areena *ar, struct powerup *pup)
{
	Mix_Chunk * snd = ar->s.pupaanet[pup->tyyppi];

	if (snd)
		Mix_PlayChannel( -1, snd, 0 );
}

void kato_pupit(struct areena *ar, struct alus *a)
{
	int i;

	if (!ar->active_pups)
		return;

	for (i = 0; i < MAX_PUPS; i++)
		if (ar->pups[i].expire)
			if (pup_napattu(a, &ar->pups[i])) {
				lisaa_puptieto(a, &ar->pups[i]);
				ar->pups[i].expire = 0;
				ar->active_pups--;
				pup_pisteet(ar, &ar->pups[i]);
				soita_puppinappaus(ar, &ar->pups[i]);
			}

	return;
}

bool mun_pupit(struct alus *a, int tyyppi)
{
	uint8_t i;
	if (tyyppi >= PUP_TYYPIT)
		return false;

	for (i = a->pups.first; i != a->pups.last; i++)
		if (a->pups.pbuf[i].tyyppi == tyyppi)
			return true;
	return false;
}

void pysayta_alus(struct alus *a)
{
	a->vri.r = 255;
	a->vri.g = 0;
	a->vri.b = 0;
	a->nopeus = 0;
}

void uusi_paikka(struct areena *ar, struct alus *a)
{
	int nop_x, nop_y;
	float angle;
	int noppeus;
	struct alus *oma = &ar->alukset[0];

	if (oonko_jaassa(a))
		noppeus = 0;
	else if (oonko_noppee(a))
		noppeus = NOP_MAX;
	else
		noppeus = a->nopeus;

	if (!noppeus)
		goto paikka_paivitetty;

	angle = a->suunta * M_PI / 180.0f;
	nop_y = sinf(angle) * noppeus;
	nop_x = cosf(angle) * noppeus;

	a->p_delta.x += nop_x;
	while (a->p_delta.x > NOP_MAX) {
		a->p.x++;
		a->p_delta.x -= NOP_MAX;
	}
	while (a->p_delta.x < -NOP_MAX) {
		a->p.x--;
		a->p_delta.x += NOP_MAX;
	}

	a->p_delta.y += nop_y;
	while (a->p_delta.y > NOP_MAX) {
		a->p.y++;
		a->p_delta.y -= NOP_MAX;
	}
	while (a->p_delta.y < -NOP_MAX) {
		a->p.y--;
		a->p_delta.y += NOP_MAX;
	}

	if ( a->p.x <= 0 ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.x = ar->leveys;
			goto x_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		a->p.x=1;
		a->p_delta.x=0;

		if (a->suunta > 90 && a->suunta < 270) {
			if ( a->suunta <= 180.0)
				a->suunta = 90.0 - (a->suunta - 90.0);
			else
				a->suunta = 270.0 - a->suunta + 270.0;
		}
	}
	if ( a->p.x >= ar->leveys ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.x = 0;
			goto x_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		a->p.x=ar->leveys-1;
		a->p_delta.x=0;

		if (a->suunta > 270 || a->suunta < 90) {

			if( a->suunta <= 90.0)
				a->suunta = 90.0 - a->suunta + 90.0;
			else
				a->suunta = 270.0 - (a->suunta - 270);
		}
	}
x_paivitetty:
	if ( a->p.y <= 0 ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.y = ar->korkeus;
			goto paikka_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		if (a->p.y <= 0) {
			a->p.y = 1;
			a->p_delta.y = 0;
		}

		if ((a->suunta <= 270 && a->suunta >= 180) )
			a->suunta = 180 - (a->suunta - 180);
		else if (a->suunta > 270)
			a->suunta = 360.0 - a->suunta;
	}
	if (a->p.y >= ar->korkeus) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.y = 0;
			goto paikka_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}
		if (a->p.y >= ar->korkeus) {
			a->p.y = ar->korkeus - 1;
			a->p_delta.y = 0;
		}

		if ( a->suunta <= 180 )
			a->suunta = 360.0 - a->suunta;


	}
paikka_paivitetty:

	alus_laske_nurkat(a);

	if (a->oma)
	{
		poista_vanhat_pupit(a);
		kato_pupit(ar, a);
	}

	if (a->oma)
		return;

	if (!oonko_kuolematon(oma))
		if ( o_iholla(oma,a))
			if ( tormasi(oma, a)) {
				if (oonko_rikkova(oma))
						pysayta_alus(a);
					else
					loppu_punaa(ar);
			}
}

void alusta_oma_alus(struct areena *a)
{
	struct vari v = ALUS_OLETUS_VARI;
	struct paikka p;

	p.x = a->leveys/2;
	p.y = a->korkeus/2;

	luo_alus(&a->alukset[0], ALUS_OLETUS_LEVEYS, ALUS_OLETUS_PITUUS, &p,
		 ALUS_OLETUS_SUUNTA, ALUS_OLETUS_NOPEUS, &v);
	a->alukset[0].oma = 1;
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

void uudet_paikat(struct areena *a)
{
	int i;

	for (i = 0; i < a->alusten_maara && !a->stop; i++)
		uusi_paikka(a, &a->alukset[i]);
}

void togglefullscreen(struct areena *a, SDL_Window* window, SDL_Renderer* renderer)
{
	Uint32 flags = SDL_GetWindowFlags(window);

	flags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
	SDL_SetWindowFullscreen(window, flags | SDL_WINDOWPOS_CENTERED);

	if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP))
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, a->leveys, a->korkeus);
	}
	else
	{
		SDL_SetWindowSize(window, a->leveys, a->korkeus);
	}
}

void test_display(struct areena *a, SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, a->leveys, a->korkeus);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void get_input(struct areena *a)
{
	struct alus *oma = &a->alukset[0];
	float fix = 0;
	const Uint8 *state;
	struct paikka hiiri = {0,0};;

	/* Is this needed anymore? */
	SDL_PumpEvents();
	state = SDL_GetKeyboardState(NULL);

	if(state) {
		if (state[SDL_SCANCODE_UP])
			oma->nopeus+=100;
		if (state[SDL_SCANCODE_DOWN])
			oma->nopeus-=100;
	}

	if (oma->nopeus < 0)
		oma->nopeus = 0;
	if (oma->nopeus > NOP_MAX)
		oma->nopeus = NOP_MAX; 

	if (SDL_GetMouseState(&hiiri.x,&hiiri.y) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		a->realstop = 1;
		return;
	}

	if ((!oonko_kuolematon(oma)) && isin_kolmio(&oma->vas_takanurkka, &oma->oik_takanurkka,
	    &oma->etunurkka, &hiiri)) {
		SDL_Log("Törmäsit hiireen!\n");
		loppu_punaa(a);
	}

	hiiri.x-=oma->p.x;
	hiiri.y-=oma->p.y;

	if (hiiri.x < 0)
		fix = -180.0;

	if (!hiiri.y) {
		if (hiiri.x<0)
			oma->suunta = 180;
		else if (!hiiri.x)
			;
		else
			oma->suunta = 0;
	} else if (!hiiri.x) {
		if (hiiri.y<0)
			oma->suunta = 90;
		else
			oma->suunta = 270;
	} else
		oma->suunta = 180.0/M_PI * atan((float)-hiiri.y/(float)-hiiri.x);
	oma->suunta += fix;
	if (oma->suunta < 0)
		oma->suunta += 360;
}
void valipisteet(struct areena *ar)
{
	static struct SDL_Color v = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	static char pisteet[255];
	struct paikka p = { .x = ar->leveys/2 - 100, .y = ar->korkeus/2 -100, };
	static int loop = 0;

	struct SDL_Color v_table[] = {
		{ 255, 255, 255, SDL_ALPHA_OPAQUE },
		{ 0, 255, 255, SDL_ALPHA_OPAQUE },
		{ 255, 0, 255, SDL_ALPHA_OPAQUE },
		{ 255, 255, 0, SDL_ALPHA_OPAQUE },
		{ 0, 0, 255, SDL_ALPHA_OPAQUE },
		{ 255, 0, 0, SDL_ALPHA_OPAQUE },
		{ 0, 255, 0, SDL_ALPHA_OPAQUE },
	};

	if (ar->valipisteet_kierros == 0) {
		snprintf(pisteet, 255, "%u", ar->pisteet);
		ar->valipisteet_kierros = PISTEET_NAKYY_KIERROSTA;
		ar->valipisteet_kokomuutos = PISTEKOON_PIENENNYS_ALUSSA;
		
		loop = 0;
		v = v_table[((ar->pisteet/50)-1)%7];
		Mix_PlayChannel( -1, ar->s.points, 0 );
	} else {
		ar->valipisteet_kierros--;
		if (!((loop++)%PISTEKOKO_SAMANA_LOOPIT))
			ar->valipisteet_kokomuutos -= PISTEKOON_MUUTOS_STEPPI;
	}

	v.a -= 100;

	draw_text(&ar->p, pisteet, &p, 200-ar->valipisteet_kokomuutos, 200-ar->valipisteet_kokomuutos, &v);	
}
#define PELAAJIA 5
static const char *nimi[PELAAJIA] = { "Muru", "Jasper", "Joona", "Iivari", "Mestari-Isi" };

const char * alkuruutu(struct areena *a)
{
	struct paikka p[PELAAJIA] = {
	};
	struct SDL_Color v = { 51, 221, 255, SDL_ALPHA_OPAQUE };
	uint32_t state;
	int korkeus = a->korkeus/PELAAJIA;
	int y,i;

	p[0].x = a->leveys/2 -300,
	p[0].y = a->korkeus/4 -5,		 

	draw_text(&a->p, "Aloita Peli", &p[0], 500, 100, &v);
	p[0].x = a->leveys/2 - 300;
	p[0].y = (a->korkeus/4)*3 -100;
	v.r = 225;
	v.g = 255;
	v.b = 255;

	draw_text(&a->p, "Paina Nayttoa", &p[0], 500, 100, &v);

	v.r = 0;
	v.g = 225;
	v.b = 0;
	v.a = SDL_ALPHA_OPAQUE/2;

	for (i = 0; i < PELAAJIA; i++) {
		p[i].x = a->leveys/2 -300,
		p[i].y = i*korkeus + korkeus/2;

		draw_text(&a->p, nimi[i], &p[i], 500, korkeus, &v);
		SDL_RenderDrawLine(a->p.renderer, 0, p[i].y + korkeus/2, a->leveys, p[i].y + korkeus/2);
	}

	SDL_RenderPresent(a->p.renderer);


	for (;;) {
		SDL_PumpEvents();
		if ((state = SDL_GetMouseState(NULL,&y)) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (i = 0; i < PELAAJIA; i++) 
				if (y < (i+1)*korkeus)
					return nimi[i];
			continue;
		}
		if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			a->realstop = 1;
			return NULL;
		}
		usleep(10000);
	}
}

static int arvo_powerup(struct areena *ar)
{
	if (ar->active_pups < MAX_PUPS) {
		unsigned long long chance = rand() %UUDEN_PUPIN_TSAANNSSI;

		if (!chance) {
			int i;
			struct vari v = {255, 0, 200, SDL_ALPHA_OPAQUE};
			struct powerup *p = &ar->pups[0];

			for (i = 0; i < MAX_PUPS && p->expire; i++, p++);

			if (i == MAX_PUPS) {
				SDL_Log("Viiiirrhheita Koodissa\n");
				return -1;
			}
			ar->active_pups++;
			/* Arpa suosi, tehdään uus poweruppi */
			p->koko = PUPS_KOKO;
			p->p.x = p->koko + (rand() % ar->leveys) - 2*p->koko;
			p->p.y = p->koko + (rand() % ar->korkeus) - 2*p->koko;
			p->tyyppi = rand() % PUP_TYYPIT;
			p->nappauspisteet = nappauspisteet[p->tyyppi];
			p->vri = v;
			p->piirra = piirra_pup,
			p->expire = time(NULL) + 5;
		}
	}
	return 0;
}

int main(int arc, char *argv[])
{
	int ok, i;
	const char *nimi;
	static struct areena a;
	SDL_Window* window = NULL;

	srand(time(NULL));

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	if (TTF_Init()) {
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(WINDOW_X, WINDOW_Y, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &a.p.renderer)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		goto err_out;
	}
	test_display(&a, window, a.p.renderer);
	SDL_SetRenderDrawBlendMode(a.p.renderer, SDL_BLENDMODE_BLEND);

	ok = luo_areena(&a);
	if (ok)
		 goto err_out;

	a.p.font = TTF_OpenFont("/usr/share/fonts/liberation/LiberationMono-Regular.ttf", 24);
	if (!a.p.font) {
		SDL_Log("Fontti ei auennu %s\n",SDL_GetError());
		goto err_out;
	}
	if (music_init(&a.s))
		goto out_font;

	Mix_PlayMusic( a.s.music, -1 );
uusiksi:
	a.pisteet = 0;
	putsaa_pupit(&a);
	ok = luo_alukset(&a);
	a.stop = 0;
	a.valipisteet_kierros = 0;
	if (ok)
		goto out_font;

	nimi = alkuruutu(&a);


	for (i = 0; 1 ; i++) {
		a.pisteet ++;
		SDL_SetRenderDrawColor(a.p.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(a.p.renderer);
		uudet_paikat(&a);
		if (i && !(i%100)) {
			lisaa_alus(&a);
			arvo_powerup(&a);
		}
		//if (i && !(i%500))
		if (a.pisteet && !(a.pisteet%500) && !a.valipisteet_kierros)
			valipisteet(&a);
		else if (a.valipisteet_kierros)
			valipisteet(&a);
		if (a.piirra(&a))
		{
			update_fivebests(&a, nimi);
			goto uusiksi;
		}
		usleep(LOOP_DELAY_US);
		get_input(&a);
		if (a.realstop)
			break;
		SDL_RenderPresent(a.p.renderer);
	}

	Mix_FreeChunk( a.s.new_ship );
	Mix_FreeChunk( a.s.crash );
	Mix_FreeChunk( a.s.points );
	Mix_FreeMusic( a.s.music );
	Mix_CloseAudio();

out_font:
	TTF_CloseFont( a.p.font );	
	TTF_Quit();
	SDL_Quit();

	return 0;

err_out:
	SDL_Quit();
	return -1;

}
