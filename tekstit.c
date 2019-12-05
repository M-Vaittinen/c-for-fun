#include "piirrettavat_tekstit.h"
#include "helpers.h"

static struct pirrettava_teksti g_pt[255] = {};
uint64_t g_vapaat[4] = { 0xffffffffffffffffULL, 0xffffffffffffffffULL,
			 0xffffffffffffffffULL, 0xffffffffffffffffULL };

struct pirrettava_teksti *varaa_piirrospaikka()
{
	int i, j;

	for (i = 0; i < 4; i++) {
		if (!g_vapaat[i])
			continue;
		for (j = 0; j < 64; j++)
			if ((g_vapaat[i] & (1LLU << j))) {
				g_vapaat[i] &= ~(1LLU << j);
				return &g_pt[i*64+j];
			}
		break;
	}
	return NULL;
}

void vapauta_piirrospaikka(struct pirrettava_teksti *paikka)
{
	int indeksi, kerroin;

	indeksi = ((unsigned long)paikka) - ((unsigned long)&g_pt[0]) / sizeof(*paikka);
	kerroin = indeksi/64;
	indeksi -= kerroin * 64;

	g_vapaat[kerroin] |= (1LLU << indeksi);
}

void piirra_tekstit(struct areena *a)
{
	uint64_t varatut;
	int i,j;

	for (i = 0; i < 4; i++) {
		varatut = (~g_vapaat[i]);
		for(j=0; varatut; j++)
			if (varatut & (1LLU<<j)) {
				struct pirrettava_teksti *pt = &g_pt[i*64+j];

				if (!pt->nakyvilla_kierros) {
					varatut &= (~(1LLU << j));
					vapauta_piirrospaikka(pt);
					continue;
				}

				if (!(pt->nakyvilla_kierros % pt->kokomuutos_kierroksia)){
					pt->leveys = pt->leveys + pt->kokomuutos_x_kierros;
					pt->korkeus = pt->korkeus + pt->kokomuutos_y_kierros;
				}

				varatut &= (~(1LLU << j));
				draw_text(a, pt->teksti, &pt->p, pt->leveys, pt->korkeus, &pt->v);
				pt->nakyvilla_kierros--;
				if (!pt->nakyvilla_kierros)
					vapauta_piirrospaikka(pt);
			}
	}
}


