#ifndef _SERVER_DATA_H
#define _SERVER_DATA_H

#include <stdbool.h>
#include "common.h"
#include "paikka.h"

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


struct alus_server_data {
	int id;
	int oma;
	struct paikka p;
	/* TODO: Check where these are used */
	struct paikka coll_min;
	struct paikka coll_max;
	float suunta;
	int nopeus;
	float pituus;
	float leveys;
	bool rikki;
};

struct powerup_server_data {
	int koko;
	struct paikka p;
	int tyyppi;
	int nappauspisteet;

};

struct areena_server_data {
	struct timespec last_server_update; /* Client should ignore these times */
	/*
	 * TODO: Should we put the client update timestamps directly in correct
	 * 'alus' struct ?
	 */
	struct timespec last_client1_update; /* TODO: Split server specifics out */
	struct timespec last_client2_update; /* TODO: Split server specifics out */
	bool initialized;
        unsigned pisteet_id1;
	unsigned pisteet_id2;
	int alusten_maara;
	struct alus_server_data alukset[ALUKSET_MAX];
	int active_pups;
	struct powerup_server_data pups[MAX_PUPS];
};

#endif
