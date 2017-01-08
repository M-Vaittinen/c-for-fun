#ifndef _ESTE_H
#define _ESTE_H

typedef enum estetyyppi
{
    este_unused=0,
    este_block,
    este_speed,
    este_pisteet,
    este_last
}estetyyppi;



typedef struct este
{
    int type;
    char merkki;
    int paikka;
    int varileveys;
    int varialkuleveys;
    const char *vari_alku;
    const char *vari_loppu;
}este;


#endif
