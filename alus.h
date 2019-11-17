#ifndef _ALUS_H_
#define _ALUS_H_

#include "paikka.h"

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


#endif
