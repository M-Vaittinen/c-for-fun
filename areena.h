#ifndef _AREENA_H_
#define _AREENA_H_

#include <SDL.h>
#include <stdbool.h>
#include "media.h"
#include "seina.h"
#include "alus.h"
#include "common.h"
#include "powerup.h"
#include "server_data.h"

struct areena_server_data;

struct areena {
	struct timespec prev_server_update_c;
	struct piirrin p;
	struct sounds s;
	unsigned pisteet;
	int stop;
	int realstop;
	int leveys;
	int leveys_offset;
	int korkeus;
	int korkeus_offset;
	/*
	 * Let's keep the walls and wall amount in the client side only.
	 * It is currently fixed to 4 walls with constant arena size anyways.
	 * Keeping it all in client allows us to keep the data and functions
	 * in same constant struct.
	 */
	int seinien_maara;
	struct seina *seinat;
	int alusten_maara;
	struct alus alukset[ALUKSET_MAX];
	int active_pups;
	struct powerup pups[MAX_PUPS];
	int (*piirra) (struct areena*);
};

int piirra_areena(struct areena *a);
int luo_areena(struct areena *a);
int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct SDL_Color *v);
void lisaa_rikkopisteet(struct areena *ar, struct alus *oma);
int arvo_powerup(struct areena *ar);
void update_areena_by_serverdata(struct areena *a, struct areena_server_data *sd,
				 int id);

#endif
