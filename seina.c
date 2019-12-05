#include "seina.h"
#include "areena.h"

void piirra_seina(struct areena *a, struct seina *s)
{
	SDL_SetRenderDrawColor(a->p.renderer, s->vri.r, s->vri.g, s->vri.b, s->vri.a);
	if (SDL_RenderDrawLine(a->p.renderer, s->alku.x + a->leveys_offset, s->alku.y + a->korkeus_offset, s->loppu.x + a->leveys_offset, s->loppu.y + a->korkeus_offset)) {
		SDL_Log("Unable to draw line: %s", SDL_GetError()); 
	}
}

int alusta_seina(struct seina *s, struct paikka *alku, struct paikka *loppu, struct SDL_Color *v)
{
	s->alku = *alku;
	s->loppu = *loppu;
	s->piirra = piirra_seina;
	s->vri = *v;

	return 0;
}


