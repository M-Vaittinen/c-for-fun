#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "paikka.h" 
#define PUPS_KOKO 8

enum pup_tyypit {
	PUP_SPEED = 0,
	PUP_COOL,
	PUP_IMMORTAL,
	PUP_DESTROY,
	PUP_FREEZE,
	PUP_PASS_WALLS,
	PUP_TYYPIT,
};

int nappauspisteet[PUP_TYYPIT] = {
	[PUP_SPEED] = 200,
	[PUP_COOL] = 50,
	[PUP_IMMORTAL] = 10,
	[PUP_DESTROY] = 10,
	[PUP_FREEZE] = 500,
	[PUP_PASS_WALLS] = 10,
};

static const char *puppi_txt_arr[PUP_TYYPIT] = {
	[PUP_SPEED] = "Kiire!",
	[PUP_COOL] = "Rahaa!",
	[PUP_IMMORTAL] = "Haamu",
	[PUP_DESTROY] = "+Powers!",
	[PUP_FREEZE] = "Paikka!",
	[PUP_PASS_WALLS] = "Rajaton",
};

struct powerup {
	int koko;
	struct paikka p;
	int tyyppi;
	int nappauspisteet;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct powerup*);
	time_t expire;
};

#endif

