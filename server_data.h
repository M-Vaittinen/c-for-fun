#ifndef _SERVER_DATA_H
#define _SERVER_DATA_H

#include <stdbool.h>
#include "common.h"
#include "client.h"
#include "nurkat.h"
#include "paikka.h"
#include "puppipuskuri.h"

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
	struct paikka p_delta;
	/* TODO: Check where these are used */
	struct paikka coll_min;
	struct paikka coll_max;
	struct nurkat corners;
	float suunta;
	int nopeus;
	float pituus;
	float leveys;
	bool rikki;

	struct puppipuskuri pups;
};

struct powerup_server_data {
	int koko;
	struct paikka p;
	int tyyppi;
	int nappauspisteet;
	time_t expire;

};

struct areena_server_data {
	struct timespec last_server_update_c; /* Time when client got this data */
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
	bool player1_lost;
	bool player2_lost;
	int alusten_maara;
	struct alus_server_data alukset[ALUKSET_MAX];
	int active_pups;
	struct powerup_server_data pups[MAX_PUPS];
	/* These should not need to be sent */
	int leveys;
	int korkeus;
};

void client_get_serverdata(struct client *c, struct areena_server_data *d);

#endif
