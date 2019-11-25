#ifndef _AREENA_H_
#define _AREENA_H_

#include <SDL.h>
#include "media.h"
#include "seina.h"
#include "alus.h"
#include "common.h"
#include "powerup.h"

struct areena {
	struct piirrin p;
	struct sounds s;
	unsigned pisteet;
	unsigned valipisteet_kierros;
	unsigned valipisteet_kokomuutos;
	int stop;
	int realstop;
	int leveys;
	int leveys_offset;
	int korkeus;
	int korkeus_offset;
	int seinien_maara;
	struct seina *seinat;
	int alusten_maara;
	struct alus alukset[ALUKSET_MAX];
	int active_pups;
	struct powerup pups[MAX_PUPS];
	int (*piirra) (struct areena*);
};



#endif
