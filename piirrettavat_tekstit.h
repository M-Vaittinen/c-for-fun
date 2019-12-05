#ifndef _PIIRRETTAVAT_TEKSTIT_H
#define _PIIRRETTAVAT_TEKSTIT_H
#include <SDL.h>
#include "paikka.h"
#include "areena.h"

struct pirrettava_teksti {
	/* If we need dynamic text we can add here arrauy later */
	const char *teksti;
	struct paikka p;
	int nakyvilla_kierros;
	int leveys;
	int korkeus;
	int kokomuutos_kierroksia;
	int kokomuutos_x_kierros;
	int kokomuutos_y_kierros;
	struct SDL_Color v;
};

struct pirrettava_teksti *varaa_piirrospaikka();
void piirra_tekstit(struct areena *a);

#endif
