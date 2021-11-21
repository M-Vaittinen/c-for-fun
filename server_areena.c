#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_data.h"
#include "helpers.h"

pthread_mutex_t sad_lock = PTHREAD_MUTEX_INITIALIZER;
static struct areena_server_data g_sad;

void luo_server_alus(struct alus_server_data *a, float leveys, float pituus,
		     struct paikka *p,
		     float suunta, int nopeus, int id)
{
	memset(a, 0, sizeof(*a));
	a->id = id;
	a->p = *p;
	a->suunta = suunta;
	a->nopeus = nopeus;
	a->pituus = pituus;
	a->leveys = leveys;
}

void server_lisaa_alus(struct areena_server_data *sad,
		       struct alus_server_data *a, int id)
{
	pthread_mutex_lock(&sad_lock);
	sad->alukset[id] = *a;
	pthread_mutex_unlock(&sad_lock);
}

int srv_o_iholla(struct alus_server_data *a, struct alus_server_data *a2)
{
        return ((a->coll_min.x <= a2->coll_max.x) &&
                (a->coll_max.x >= a2->coll_min.x) &&
                (a->coll_min.y <= a2->coll_max.y ) &&
                (a->coll_max.y >= a2->coll_min.y));
}

static void srv_coll_update(struct alus_server_data *a, struct paikka *p)
{
        a->coll_max.x = MAX(a->coll_max.x, p->x);
        a->coll_max.y = MAX(a->coll_max.y, p->y);
        a->coll_min.x = MIN(a->coll_min.x, p->x);
        a->coll_min.y = MIN(a->coll_min.y, p->y);
}

void srv_alus_laske_nurkat(struct alus_server_data *a)
{
        float change_x;
        float change_y;
        float pit, pit_nurkka;
        float lev;
        float angle = a->suunta * M_PI / 180.0f;
	struct paikka etunurkka;
	struct paikka oik_takanurkka;
	struct paikka vas_takanurkka;

        float tangle;

        /* Paikat ennen kiertoa */
        pit = a->pituus/2.0;
        lev = a->leveys/2.0;
        pit_nurkka = sqrtf(pit*pit + lev*lev);

        tangle = atan(a->leveys/a->pituus);

        /* Kierrä suunnan mukaan */

        change_y = sinf(angle) * pit;
        change_x = cosf(angle) * pit;

        etunurkka.x = a->p.x + change_x;
        etunurkka.y = a->p.y + change_y;

        a->coll_max.x = a->coll_min.x = etunurkka.x;
        a->coll_max.y = a->coll_min.y = etunurkka.y;

        change_y = sin(M_PI + tangle + angle) * pit_nurkka;
        change_x = cos(M_PI + tangle + angle) * pit_nurkka;

        oik_takanurkka.x = a->p.x + change_x;
        oik_takanurkka.y = a->p.y + change_y;

        srv_coll_update(a, &oik_takanurkka);

        change_y = sin(M_PI - tangle + angle) * pit_nurkka;
        change_x = cos(M_PI - tangle + angle) * pit_nurkka;

        vas_takanurkka.x = a->p.x + change_x;
        vas_takanurkka.y = a->p.y + change_y;

        srv_coll_update(a, &vas_takanurkka);
}

void server_arvo_alus(struct alus_server_data *a, int id,
		      struct areena_server_data *sad)
{
	float suunta, pit, lev;
	struct paikka p;
	int nop;
	float koko = (float) (rand() % 20);

	if (id >= ALUKSET_MAX) {
		printf("Oh-No - liikaa aluksia\n");
		return;
	}
uus:
	p.x = 1 + (rand() % WINDOW_X /* a->leveys */) - 2;
	p.y = 1 + (rand() % WINDOW_Y /* a->korkeus */) - 2;
        lev = 5 + koko;
        pit = 10 + koko*2;
        suunta = (float)(rand() % 360);
        nop = rand() % NOP_MAX;

	srv_alus_laske_nurkat(a);

	if ( (id > 0 && srv_o_iholla(&sad->alukset[0], a)) || (id > 1 && srv_o_iholla(&sad->alukset[1], a)))
		goto uus;

	luo_server_alus(a, lev, pit, &p, suunta, nop, id);
}

void arvo_server_areena()
{
	int alusten_maara = 20;
	int i;
	struct areena_server_data *sad = &g_sad;

	for (i = 0; i < alusten_maara; i++) {
		struct alus_server_data a;

		server_arvo_alus(&a, i, sad);
		server_lisaa_alus(sad, &a, i);
	}
}
