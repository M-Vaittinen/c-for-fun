#ifndef _PELIINFO_H
#define _PELIINFO_H

#include "estestats.h"
#include "tienpala.h"
#include "huijaukset.h"

typedef struct peliinfo
{
    int kierros;
    unsigned int pisteet;
    unsigned int pistekerroin;
    unsigned int nopeus;
    int suunta;
    int autonpaikka;
    int hidastus;
    int hidastus_alku;
    int speed_kierros;
    int hidastus_saved;
    tienpala *tie;
    int extratext_array_current;
    estestats stats;
    cheats huijaukset;
}peliinfo;

#endif
