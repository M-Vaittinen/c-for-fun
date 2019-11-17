#ifndef _SEINA_H_
#define _SEINA_H_

#include "paikka.h"
#include "media.h"

struct seina {
	struct paikka alku;
	struct paikka loppu;
	int flags;
	struct vari vri;
	void (*piirra) (SDL_Renderer*, struct seina*);
};



#endif
