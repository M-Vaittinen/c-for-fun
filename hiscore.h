#ifndef _HISCORE_H_
#define _HISCORE_H_

#include "areena.h"
#define NAME_MAX 200

void talleta_pisteet(struct areena *ar, const char *name);
void read_scores();

struct hiscore {
	struct hiscore *head;
	struct hiscore *next;
	struct hiscore *prev;
	char name[NAME_MAX];
	unsigned int points;
};

#define joka_huippupisteelle(_head,_h) \
        for ((_h)=(_head)->head->next; (_h); (_h)=(_h)->next)

//struct hiscore * etsi_nimella(struct hiscore *head, const char *name);
unsigned int hae_pisteet(const char *name);

#endif
