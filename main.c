#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define WINDOW_X (640*2)
#define WINDOW_Y (480*2)
#define MOVE_THRESH 10
#define MOVE_JUMP_THRESH 6000

#define ALUKSET_MAX 255
#define LOOP_DELAY_US 5000 //5 ms
#define NOP_MAX 36000*2

struct paikka {
	int x;
	int y;
};

struct vari {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char alpha;
};

struct seina {
	struct paikka alku;
	struct paikka loppu;
	int flags;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct seina*);
};

struct alus {
	int oma;
	struct paikka p;
	struct paikka coll_min;
	struct paikka coll_max;
	struct paikka p_delta;
	struct paikka vas_takanurkka;
	struct paikka oik_takanurkka;
	struct paikka etunurkka;
	float suunta;
	int nopeus;
	float pituus;
	float leveys;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct alus*);
};

struct areena {
	int stop;
	int leveys;
	int korkeus;
	int seinien_maara;
	struct seina *seinat;
	int alusten_maara;
	struct alus alukset[ALUKSET_MAX];
	void (*piirra) (SDL_Renderer*, struct areena*);
};

int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct vari *v);

#define MIN(a,b) ((a)<=(b))?(a):(b)
#define MAX(a,b) ((a)>=(b))?(a):(b)

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

	//SDL_Log("Kulma %f rad\n", angle);
	//SDL_Log("sin(%f) = %f\n", angle, sin(angle));
	//SDL_Log("cos(%f) = %f\n", angle, cos(angle));
	/* Paikat ennen kiertoa */
	pit = a->pituus/2.0;
	lev = a->leveys/2.0;
	pit_nurkka = sqrtf(pit*pit + lev*lev);

	tangle = atan(a->leveys/a->pituus);

	/* Kierrä suunnan mukaan */

	change_y = sinf(angle) * pit;
	//SDL_Log("Y change %f, len %f\n", change_y, lev);
	change_x = cosf(angle) * pit;
	//SDL_Log("X change %f, len %f\n", change_x, pit);

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
//	struct paikka vas_takanurkka, oik_takanurkka, etunurkka;
	struct seina s;
//	struct vari v = {255, 0, 0, SDL_ALPHA_OPAQUE};

	alusta_seina(&s, &a->vas_takanurkka, &a->oik_takanurkka, &a->vri /* &v */);
	s.piirra(renderer, &s);
	alusta_seina(&s, &a->oik_takanurkka, &a->etunurkka, &a->vri);
	s.piirra(renderer, &s);
	alusta_seina(&s, &a->etunurkka, &a->vas_takanurkka, &a->vri);
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

void piirra_areena(SDL_Renderer* renderer, struct areena *a)
{
	int i;

	for (i = 0; i < a->seinien_maara; i++)
		a->seinat[i].piirra(renderer, &a->seinat[i]);

	for (i = 0; i < a->alusten_maara; i++)
		a->alukset[i].piirra(renderer, &a->alukset[i]);
	SDL_RenderPresent(renderer);

	if (a->stop) {
		sleep(5);
		exit(0);	
	}
}

int luo_areena(struct areena *a)
{
	int ok;

	a->stop = 0;
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
	a->oma = 0;
	a->leveys = leveys;
	a->pituus = pituus;
	a->suunta = suunta;
	a->p = *p;
	a->nopeus = nopeus;
	a->vri = *v;
	a->piirra = piirra_alus;
}

void arvo_alus(struct areena *a, int index)
{
	struct vari v = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	struct paikka p;
	float lev, pit, suunta;
	int nop;
	float koko = (float) (rand() % 20);

	p.x = 1 + rand() % a->leveys - 2;
	p.y = 1 + rand() % a->korkeus - 2;
	lev = 5 + koko;
	pit = 10 + koko*2;
	suunta = (float)(rand() % 360);
//	nop = 1.0/(double)(rand() % 100);
	nop = rand() % NOP_MAX;

	luo_alus(&a->alukset[index], lev, pit, &p, suunta, nop, &v);
/*	a->alukset[index].p.x = rand() % a->leveys; 
	a->alukset[index].p.y = rand() % a->korkeus;
	a->alukset[index].suunta = (float)(rand() % 360);
	a->alukset[index].suunta = (float) i;
	a->alukset[index].nopeus = rand() % 5;
	i+=90;
*/
}

int orientation(struct paikka *p, struct paikka *q, struct paikka *r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q->y - p->y) * (r->x - q->x) - 
              (q->x - p->x) * (r->y - q->y); 
  
    if (val == 0) return 0;  // colinear 
  
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
 

#define ALUS_OLETUS_PITUUS 30
#define ALUS_OLETUS_LEVEYS 15
#define ALUS_OLETUS_VARI { 0, 255, 0, SDL_ALPHA_OPAQUE }
#define ALUS_OLETUS_SUUNTA 0
#define ALUS_OLETUS_NOPEUS 0

void uusi_paikka(struct areena *ar, struct alus *a)
{
	int nop_x, nop_y;
	float angle;
	struct paikka p = a->p;
	struct paikka pd = a->p_delta;
	struct alus *oma = &ar->alukset[0];

	if (!a->nopeus)
		return;
	if (0) {
uuziaan:
		a->p = p;
		a->p_delta = pd;
	}
	angle = a->suunta * M_PI / 180.0f;
	nop_y = sinf(angle) * a->nopeus;
	nop_x = cosf(angle) * a->nopeus;
	//SDL_Log("Nopeus %d, Suunta %f, nop_x %d, nop_y %d\n",
	//	a->nopeus, a->suunta, nop_x, nop_y);

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

	//SDL_Log("Raakapaikka %d, %d\n", a->p.x, a->p.y);

	if ( a->p.x <= 0 ) {
		if (a->oma) {
			printf("Törmäsit seinään\n");
			
			oma->vri.r = 255;
			oma->vri.g = 0;
			oma->vri.b = 0;
			ar->stop = 1;
		}
		//SDL_Log("Paikka %d,%d - x<0\n", a->p.x, a->p.y);
		//SDL_Log("Suunta %f\n", a->suunta);
		if ( a->suunta <= 180.0)
			a->suunta = 90.0 - (a->suunta - 90.0);
		else
			a->suunta = 270.0 - a->suunta + 270.0;
		//SDL_Log("Uusi suunta %f\n", a->suunta);
		goto uuziaan;
	}
	if ( a->p.x >= ar->leveys ) {
		if (a->oma) {
			printf("Törmäsit seinään\n");
			oma->vri.r = 255;
			oma->vri.g = 0;
			oma->vri.b = 0;
			ar->stop = 1;
		}
		//SDL_Log("Paikka %d,%d - x>%d\n", a->p.x, a->p.y, ar->leveys);
		//SDL_Log("Suunta %f\n", a->suunta);
		if( a->suunta <= 90.0)
			a->suunta = 90.0 - a->suunta + 90.0;
		else
			a->suunta = 270.0 - (a->suunta - 270);
		//SDL_Log("Uusi suunta %f\n", a->suunta);
		goto uuziaan;
	}
	if ( a->p.y >= ar->korkeus || a->p.y <= 0 ) {
		if (a->oma) {
			printf("Törmäsit seinään\n");
			oma->vri.r = 255;
			oma->vri.g = 0;
			oma->vri.b = 0;
			ar->stop = 1;
		}
		//SDL_Log("Paikka %d,%d - y<0 || y>%d\n", a->p.x, a->p.y, ar->korkeus);
		//SDL_Log("Suunta %f\n", a->suunta);
		a->suunta = 360.0 - a->suunta;
		//SDL_Log("Uusi suunta %f\n", a->suunta);
		goto uuziaan;
	}

	alus_laske_nurkat(a);

	if (a->oma)
		return;

	if ( (oma->coll_min.x <= a->coll_max.x) &&
	     (oma->coll_max.x >= a->coll_min.x) &&
	     (oma->coll_min.y <= a->coll_max.y ) &&
	     (oma->coll_max.y >= a->coll_min.y)) {

		if (doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka, &a->vas_takanurkka, &a->oik_takanurkka) ||
		    doIntersect(&oma->oik_takanurkka, &oma->etunurkka, &a->vas_takanurkka, &a->oik_takanurkka) ||
		    doIntersect(&oma->etunurkka, &oma->vas_takanurkka, &a->vas_takanurkka, &a->oik_takanurkka) ||
		    
		    doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka, &a->oik_takanurkka, &a->etunurkka) ||
		    doIntersect(&oma->oik_takanurkka, &oma->etunurkka, &a->oik_takanurkka, &a->etunurkka) ||
		    doIntersect(&oma->etunurkka, &oma->vas_takanurkka, &a->oik_takanurkka, &a->etunurkka) ||

		    doIntersect(&oma->vas_takanurkka, &oma->oik_takanurkka, &a->etunurkka, &a->oik_takanurkka) ||
		    doIntersect(&oma->oik_takanurkka, &oma->etunurkka, &a->etunurkka, &a->oik_takanurkka) ||
		    doIntersect(&oma->etunurkka, &oma->vas_takanurkka, &a->etunurkka,  &a->oik_takanurkka))
		{

			oma->vri.r = a->vri.r = 255;
			oma->vri.g = a->vri.g = 0;
			oma->vri.b = a->vri.b = 0;

			ar->stop = 1;
		}
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

	a->alusten_maara = 100;
//	a->alusten_maara = 20;

	alusta_oma_alus(a);

	for (i = 1; i < a->alusten_maara; i++)
		arvo_alus(a, i);

	return 0;
}

void uudet_paikat(struct areena *a)
{
	int i;

	for (i = 0; i < a->alusten_maara; i++)
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
	SDL_DisplayMode m;

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetCurrentDisplayMode(0, &m);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, a->leveys, a->korkeus);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

int get_input(struct areena *a)
{
	int x = 0,y = 0;
	struct alus *oma = &a->alukset[0];
	float fix = 0;
	const Uint8 *state;

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

	if (SDL_GetMouseState(&x,&y) & SDL_BUTTON(SDL_BUTTON_LEFT))
		return -1;

	SDL_Log("Hiirulainen ny %d,%d\n",x,y);

	x-=oma->p.x;
	y-=oma->p.y;

	if (x < 0)
		fix = -180.0;

	SDL_Log("Hiirulainen mun paatista %d, %d\n",x,y);

	if (!y) {
		if (x<0)
			oma->suunta = 180;
		else if (!x)
			;
		else
			oma->suunta = 0;
	} else if (!x) {
			if (y<0)
				oma->suunta = 90;
			else
				oma->suunta = 270;
	} else
		oma->suunta = 180.0/M_PI * atan((float)-y/(float)-x);
	oma->suunta += fix;
	if (oma->suunta < 0)
		oma->suunta += 360;
	SDL_Log("OmaSuunta %f\n", oma->suunta);
//	SDL_Log("Oma suunta muutettu, uusi suunta %f\n", oma->suunta);
	return 0;
}

int main(int arc, char *argv[])
{
	int ok, i;
	struct areena a;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	srand(time(NULL));

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(WINDOW_X, WINDOW_Y, 0, &window, &renderer)) {
//	if (SDL_CreateWindowAndRenderer(WINDOW_X, WINDOW_Y, SDL_WINDOWPOS_CENTERED, &window, &renderer)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		goto err_out;
	}

	//_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	ok = luo_areena(&a);
	if (ok)
		return -1;

	ok = luo_alukset(&a);
	if (ok)
		return -1;

	//togglefullscreen(&a, window, renderer);
//	test_display(&a, window, renderer);
//	SDL_PumpEvents();

	for (i = 0; 1 ; i++) {
	//	hae_napinpainallukset();
	//	laske_alusten_paikat();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		uudet_paikat(&a);
		a.piirra(renderer, &a);
		usleep(LOOP_DELAY_US);
		if (get_input(&a))
			break;
	}

	SDL_Quit();

	return 0;

err_out:
	SDL_Quit();
	return -1;

}
