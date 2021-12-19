#ifndef _ALUS_H_
#define _ALUS_H_

#include <SDL.h>
#include <stdbool.h>
#include "nurkat.h"
#include "paikka.h"
#include "puppipuskuri.h"
#include "server_data.h"

/* kauanko poweruppi vaikuttaa sekunteina */
#define POWERUP_VAIKUTUSAIKA 5
#define PUPPITXT_MAX 255

#define ALUS_OLETUS_PITUUS 40
#define ALUS_OLETUS_LEVEYS 25
#define ALUS_OLETUS_VARI { 61, 245, 255, SDL_ALPHA_OPAQUE }
#define ALUS_OLETUS_SUUNTA 0
#define ALUS_OLETUS_NOPEUS NOP_MAX

struct areena;

struct alus {
	int id; /* Alus ID at sever data */
	int oma;
	struct paikka p;
	struct paikka coll_min;
	struct paikka coll_max;
	struct paikka p_delta;
	struct nurkat corners;
	float suunta;
	int nopeus;
	float pituus;
	float leveys;
	struct SDL_Color vri;
	bool rikki;

	struct puppipuskuri pups;

	int (*update_position) (struct alus *, struct areena_server_data *asd);
	void (*piirra) (struct areena*, struct alus*);
};

int luo_alukset(struct areena *a);
void loppu_punaa(struct areena *ar);
void pysayta_alus(struct alus *a);
void lisaa_alus(struct areena *a);
void alus_laske_nurkat(struct alus *a);

#endif
