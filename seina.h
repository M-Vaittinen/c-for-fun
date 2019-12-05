#ifndef _SEINA_H_
#define _SEINA_H_

#include "paikka.h"
#include "media.h"

struct areena;

struct seina {
	struct paikka alku;
	struct paikka loppu;
	int flags;
	struct SDL_Color vri;
	void (*piirra) (struct areena*, struct seina*);
};

int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct SDL_Color *v);


#endif
