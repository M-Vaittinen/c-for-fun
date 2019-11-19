#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "paikka.h" 

struct powerup {
	int koko;
	struct paikka p;
	int tyyppi;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct powerup*);
};

#endif

