#include <math.h>
#include <stdlib.h>

#include "jana.h"

int jana_pituus(struct jana *j)
{
	int p_x, p_y;

	pituuskomponentit(j, &p_x, &p_y);

	return (int)sqrt((double)(p_x * p_x + p_y * p_y));
}


