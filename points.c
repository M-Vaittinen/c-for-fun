#define GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termio.h>
#include <sgtty.h>
#include <sys/select.h>
#include <sys/types.h>
#include "areena.h"
#include "hiscore.h"

#define TALL_TIEDOSTO "kolmiopisteet.txt"

static struct hiscore *hchead = NULL;

struct hiscore * lisaa_hiscore(struct hiscore *h, const char *nimi, unsigned int pisteet)
{
	struct hiscore *head, *new;

	if (!h) {
		head = calloc(sizeof(*head),1);
		if (!head) {
			SDL_Log("Alloc Failed\n");
			return NULL;
		}
		head->head = head;
		head->prev = NULL;
		head->next = NULL;
		head->name[0] = 0;
		hchead = head;
	}
	else 
		head = h->head;

	if (!head) {
		SDL_Log("NULL HEAD\n");
		return NULL;
	}
	new = calloc(sizeof(*new),1);
	if (!new)
		return NULL;
	new->head = head;
	new->prev = head;
	new->next = head->next;
	head->next = new;
	strncpy(new->name, nimi, NAME_MAX);
	new->name[NAME_MAX-1] = 0;
	new->points = pisteet;
	return new;
}

struct hiscore * etsi_nimella(struct hiscore *head, const char *name)
{
	struct hiscore *h;

	if (!head || !name)
		return NULL;

	joka_huippupisteelle(head, h) {
		SDL_Log("%s: Comparing '%s' to '%s'\n", __func__, h->name, name);
		if (!strcmp(h->name, name)) {
			SDL_Log("MATCH\n");
			break;
		}
		SDL_Log("Nomatch\n");
	}

	return h;
}

unsigned int hae_pisteet(const char *name)
{
	struct hiscore *hc;

	hc = etsi_nimella(hchead, name);
	if (!hc)
		return 0;

	return hc->points;
}

void paivita_pisteet(const char *name, unsigned int pisteet)
{
	struct hiscore *hc;

	if (!name)
		return;

	hc = etsi_nimella(hchead, name);
	if (!hc) {
		hc = lisaa_hiscore(hchead, name, pisteet);
		SDL_Log("NULL hc - add new entry\n");
	}
	else
	{
		SDL_Log("Old entry for '%s' found, old pts %u, new pts %u\n",
			name, hc->points, pisteet);
		if (hc->points < pisteet) {
			SDL_Log("Updating pts for %s\n", name);
			hc->points = pisteet;
		}
	}
}

void read_scores()
{
	FILE *rf;
	unsigned int pisteet;
	char *name;
	static struct hiscore *hc = NULL;

	rf=fopen(TALL_TIEDOSTO,"r");
	if(rf)
	{
		while( 2 == fscanf(rf,"%m[^!]!%u\n",&name,&pisteet))
		{
			printf("Found hiscore %u for %s from file\n", pisteet, name);
			hc = lisaa_hiscore(hc, name, pisteet);
			if (!hc) {
				SDL_Log("jokin meni mönkään\n");
			}
			free(name);
		}
		fclose(rf);
	}
}

int write_scores()
{
	FILE *wf;
	struct hiscore *hc;

	if (!hchead) {
		SDL_Log("Ei pistehiä\n");
		return 0;
	}

	wf=fopen(TALL_TIEDOSTO,"w");
	if(wf)
	{
		for(hc = hchead->next; hc; hc=hc->next) {
			if (!hc->name || hc->name[0] == 0)
				continue;
			SDL_Log("Writing to file: %s pts %u\n", hc->name, hc->points);
			fprintf(wf, "%s!%u\n", hc->name, hc->points);
		}

		fclose(wf);
	}
	else
		return errno;
	return 0;
}

void talleta_pisteet(struct areena *ar, const char *name)
{
	paivita_pisteet(name, ar->pisteet);
	write_scores();
}

