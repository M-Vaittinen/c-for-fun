#ifndef _POWERUP_H_
#define _POWERUP_H_

#include <stdbool.h>
#include "paikka.h"
#include "puppipuskuri.h"
#include "alus.h"
#define PUPS_KOKO 8
#include "server_data.h"

struct areena;

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

bool mun_pupit(struct puppipuskuri *pups, int tyyppi);
void hanskaa_pupit(struct areena *ar);
void putsaa_pupit(struct areena *ar);
void poista_vanhat_pupit(struct alus *a);
//void kato_pupit(struct areena *ar, struct alus *a);
/* TODO: Clean this up. Parameters are not clever and design is leaked */
void kato_pupit(int *active_pups, struct puppipuskuri *pups, unsigned int *pts, struct areena *ar);
int create_random_powerup_to_place(struct powerup *p, int koko, struct paikka *pa);
int create_powerup_to_place(struct powerup *p, int koko, struct paikka *pa,
			    int tyyppi);

#define oonko_noppee(pups) mun_pupit((pups), PUP_SPEED)
#define oonko_uppee(pups) mun_pupit((pups), PUP_COOL)
#define oonko_kuolematon(pups) mun_pupit((pups), PUP_IMMORTAL)
#define oonko_rikkova(pups) mun_pupit((pups), PUP_DESTROY)
#define oonko_jaassa(pups) mun_pupit((pups), PUP_FREEZE)
#define oonko_haamu(pups) mun_pupit((pups), PUP_PASS_WALLS)

#endif

