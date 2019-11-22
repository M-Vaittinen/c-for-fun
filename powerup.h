#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "paikka.h" 
#define PUPS_KOKO 5

enum pup_tyypit {
	PUP_SPEED = 0,
	PUP_COOL,
	PUP_IMMORTAL,
	PUP_DESTROY,
	PUP_FREEZE,
	PUP_PASS_WALLS,
	PUP_TYYPIT,
};

struct powerup {
	int koko;
	struct paikka p;
	int tyyppi;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct powerup*);
	time_t expire;
};

#endif

