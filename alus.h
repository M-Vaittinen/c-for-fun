#ifndef _ALUS_H_
#define _ALUS_H_

#include "paikka.h"

#define MAX_ACTIVE_PUPS 255
/* kauanko poweruppi vaikuttaa sekunteina */
#define POWERUP_VAIKUTUSAIKA 5
#define PUPPITXT_MAX 255
struct puppi;
struct areena;

struct puppi {
	int tyyppi;
	time_t expire;
	int piirretty;
	int lisapisteet;
	const char *teksti_alustava;
	char teksti[PUPPITXT_MAX];
	struct paikka p;
	struct vari vri;
	void (*piirra) (struct areena *a, struct puppi *p);
};

struct puppipuskuri {
	uint8_t first;
	uint8_t last;

	struct puppi pbuf[MAX_ACTIVE_PUPS];
};

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
	bool rikki;

	struct puppipuskuri pups;
	
	void (*piirra) (struct areena*, struct alus*);
};


#endif
