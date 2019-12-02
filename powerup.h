#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "paikka.h" 
#define PUPS_KOKO 8

struct areena;

enum pup_tyypit {
	PUP_SPEED = 0,
	PUP_COOL,
	PUP_IMMORTAL,
	PUP_DESTROY,
	PUP_FREEZE,
	PUP_PASS_WALLS,
	PUP_TYYPIT,
};

static int nappauspisteet[PUP_TYYPIT] __attribute__((unused)) = {
	[PUP_SPEED] = 200,
	[PUP_COOL] = 50,
	[PUP_IMMORTAL] = 10,
	[PUP_DESTROY] = 10,
	[PUP_FREEZE] = 500,
	[PUP_PASS_WALLS] = 10,
/*	[PUP_SPEED] = 20000,
	[PUP_COOL] = 500000,
	[PUP_IMMORTAL] = 1000000,
	[PUP_DESTROY] = 100000,
	[PUP_FREEZE] = 5000000,
	[PUP_PASS_WALLS] = 100000,
*/
};

static const char *puppi_txt_arr[PUP_TYYPIT] __attribute__((unused)) = {
	[PUP_SPEED] = "+200 Kiire!",
	[PUP_COOL] = "+50 Rahaa!",
	[PUP_IMMORTAL] = "+10 Haamu",
	[PUP_DESTROY] = "+10 +Powers!",
	[PUP_FREEZE] = "+500 Paikka!",
	[PUP_PASS_WALLS] = "+10 Rajaton",
};

struct powerup {
	int koko;
	struct paikka p;
	int tyyppi;
	int nappauspisteet;
	struct SDL_Color vri;
	void (*piirra) (struct areena *, struct powerup*);
	time_t expire;
};

#endif

