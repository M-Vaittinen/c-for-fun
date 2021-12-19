#ifndef _PUPPIPUSKURI_H
#define _PUPPIPUSKURI_H

#define MAX_ACTIVE_PUPS 255

struct puppi {
	int tyyppi;
	time_t expire;
};

struct puppipuskuri {
	uint8_t first;
	uint8_t last;

	struct puppi pbuf[MAX_ACTIVE_PUPS];
};



#endif
