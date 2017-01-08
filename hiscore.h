#ifndef _HISCORE_H
#define _HISCORE_H

#include "estestats.h"
#include "peliinfo.h"

#define NAME_MAX 1024

typedef struct hiscore
{
    int used;
    char name[NAME_MAX];
    unsigned int max_speed;
    unsigned int points;
    estestats stats;
}hiscore;


void print_fivebests(hiscore *fivebests,int hcamnt);
void read_scores(hiscore *fivebests,int hcamnt);
void update_fivebests(peliinfo *pi,char *name,hiscore *fivebests,int hcamnt);

#endif
