#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <SDL.h>
#include "paikka.h"
#include "areena.h"

#define MIN(a,b) ((a)<=(b))?(a):(b)
#define MAX(a,b) ((a)>=(b))?(a):(b)

bool nurkka_ympyrassa(struct paikka *nurkka, struct paikka *kp, int sade);
void draw_text(struct areena *a, const char *text, struct paikka *p, int w, int h, struct SDL_Color *v);
void DrawCircle(SDL_Renderer * renderer, struct paikka *centre, int32_t radius, struct SDL_Color *v);
int o_iholla(struct alus *a, struct alus *a2);
int tormasi(struct alus *oma, struct alus *a);
bool isin_kolmio(struct paikka *a, struct paikka *b, struct paikka *c,
		struct paikka *p);

#endif // _HELPERS_H_
