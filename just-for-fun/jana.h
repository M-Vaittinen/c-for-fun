#ifndef __JANA_H
#define __JANA_H

#include <math.h>
#include <stdbool.h>
#include "piste.h"

struct jana {
	struct piste alku;
	struct piste loppu;
};

int jana_pituus(struct jana *j);

static inline void pituuskomponentit(struct jana *j, int *px, int *py)
{
	*px = abs(j->loppu.x - j->alku.x);
	*py = labs(j->loppu.y - j->alku.y);
}

static inline double jana_kulmakerroin(struct jana *j)
{
	int p_x, p_y;

	pituuskomponentit(j, &p_x, &p_y);

	return (double)-p_y/(double)p_x;
}

#endif
