#ifndef _AREENA_H_
#define _AREENA_H_

#include "media.h"
#include "seina.h"
#include "alus.h"
#include "common.h"

struct areena {
	struct piirrin p;
	struct sounds s;
	unsigned pisteet;
	unsigned valipisteet;
	int stop;
	int realstop;
	int leveys;
	int korkeus;
	int seinien_maara;
	struct seina *seinat;
	int alusten_maara;
	struct alus alukset[ALUKSET_MAX];
	int (*piirra) (struct areena*);
};



#endif
