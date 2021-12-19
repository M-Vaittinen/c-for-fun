#include "helpers.h"

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

static int orientation(struct paikka *p, struct paikka *q, struct paikka *r)
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
static bool onSegment(struct paikka *p, struct paikka *q, struct paikka *r)
{
	if (q->x <= MAX(p->x, r->x) && q->x >= MIN(p->x, r->x) &&
		q->y <= MAX(p->y, r->y) && q->y >= MIN(p->y, r->y))
	   return true;

	return false;
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
static bool doIntersect(struct paikka *p1, struct paikka *q1, struct paikka *p2, struct paikka *q2)
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

void DrawCircle(SDL_Renderer * renderer, struct paikka *centre, int32_t radius, struct SDL_Color *v)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

	SDL_SetRenderDrawColor(renderer, v->r, v->g, v->b, v->a);

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


bool nurkka_ympyrassa(struct paikka *nurkka, struct paikka *kp, int sade)
{
	return ((nurkka->x <= kp->x + sade && nurkka->x >= kp->x - sade) &&
		(nurkka->y <= kp->y + sade && nurkka->y >= kp->y - sade));
}

void draw_text(struct areena *a, const char *text, struct paikka *p, int w, int h, struct SDL_Color *v)
{
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	SDL_Rect Message_rect;

	SDL_SetRenderDrawColor(a->p.renderer, v->r, v->g, v->b, v->a);
	surfaceMessage = TTF_RenderText_Blended(a->p.font, text, *v);
	if (!surfaceMessage)
		SDL_Log("Surface ei surffannu %s\n", SDL_GetError());

	Message = SDL_CreateTextureFromSurface(a->p.renderer, surfaceMessage);
	if (!Message)
		SDL_Log("Viesti ei pullottunu\n");

	SDL_FreeSurface(surfaceMessage);

	SDL_SetTextureBlendMode(Message, SDL_BLENDMODE_BLEND);
	Message_rect.x = p->x + a->leveys_offset;
	Message_rect.y = p->y + a->korkeus_offset;
	Message_rect.w = w;
	Message_rect.h = h;

	if (SDL_RenderCopy(a->p.renderer, Message, NULL, &Message_rect)) {
		SDL_Log("Copy ei renderöityny\n");
	}
	SDL_DestroyTexture(Message);
}

int o_iholla(struct alus *a, struct alus *a2)
{
	return ((a->coll_min.x <= a2->coll_max.x) &&
		(a->coll_max.x >= a2->coll_min.x) &&
		(a->coll_min.y <= a2->coll_max.y ) &&
		(a->coll_max.y >= a2->coll_min.y));
}

int tormasi(struct nurkat *oma, struct nurkat *a)
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

