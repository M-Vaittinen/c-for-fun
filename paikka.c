#include "paikka.h"
#include "areena.h"
#include "helpers.h"
#include "alus.h"

void uusi_paikka(struct areena *ar, struct alus *a)
{
	int nop_x, nop_y;
	float angle;
	int noppeus;
	struct alus *oma = &ar->alukset[0];

	if (oonko_jaassa(a))
		noppeus = 0;
	else if (oonko_noppee(a))
		noppeus = NOP_MAX;
	else
		noppeus = a->nopeus;

	if (!noppeus)
		goto paikka_paivitetty;

	angle = a->suunta * M_PI / 180.0f;
	nop_y = sinf(angle) * noppeus;
	nop_x = cosf(angle) * noppeus;

	a->p_delta.x += nop_x;
	while (a->p_delta.x > NOP_MAX) {
		a->p.x++;
		a->p_delta.x -= NOP_MAX;
	}
	while (a->p_delta.x < -NOP_MAX) {
		a->p.x--;
		a->p_delta.x += NOP_MAX;
	}

	a->p_delta.y += nop_y;
	while (a->p_delta.y > NOP_MAX) {
		a->p.y++;
		a->p_delta.y -= NOP_MAX;
	}
	while (a->p_delta.y < -NOP_MAX) {
		a->p.y--;
		a->p_delta.y += NOP_MAX;
	}

	if ( a->p.x <= 0 ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.x = ar->leveys;
			goto x_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		a->p.x=1;
		a->p_delta.x=0;

		if (a->suunta > 90 && a->suunta < 270) {
			if ( a->suunta <= 180.0)
				a->suunta = 90.0 - (a->suunta - 90.0);
			else
				a->suunta = 270.0 - a->suunta + 270.0;
		}
	}
	if ( a->p.x >= ar->leveys ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.x = 0;
			goto x_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		a->p.x=ar->leveys-1;
		a->p_delta.x=0;

		if (a->suunta > 270 || a->suunta < 90) {

			if( a->suunta <= 90.0)
				a->suunta = 90.0 - a->suunta + 90.0;
			else
				a->suunta = 270.0 - (a->suunta - 270);
		}
	}
x_paivitetty:
	if ( a->p.y <= 0 ) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.y = ar->korkeus;
			goto paikka_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}

		if (a->p.y <= 0) {
			a->p.y = 1;
			a->p_delta.y = 0;
		}

		if ((a->suunta <= 270 && a->suunta >= 180) )
			a->suunta = 180 - (a->suunta - 180);
		else if (a->suunta > 270)
			a->suunta = 360.0 - a->suunta;
	}
	if (a->p.y >= ar->korkeus) {
		if (oonko_haamu(a) || (a->oma && oonko_kuolematon(a))) {
			/* Mee seinän läpi */
			a->p.y = 0;
			goto paikka_paivitetty;
		}
		if (a->oma) {
			printf("Törmäsit seinään\n");
			loppu_punaa(ar);	
			goto paikka_paivitetty;
		}
		if (a->p.y >= ar->korkeus) {
			a->p.y = ar->korkeus - 1;
			a->p_delta.y = 0;
		}

		if ( a->suunta <= 180 )
			a->suunta = 360.0 - a->suunta;


	}
paikka_paivitetty:

	alus_laske_nurkat(a);

	if (a->oma)
	{
		poista_vanhat_pupit(a);
		kato_pupit(ar, a);
	}

	if (a->oma)
		return;

	if (!oonko_kuolematon(oma)) {
		if ( o_iholla(oma,a)) {
			if ( tormasi(oma, a)) {
				if (oonko_rikkova(oma)) {
					if (!a->rikki) {
						lisaa_rikkopisteet(ar, oma);
						pysayta_alus(a);
						a->rikki = 1;
					}
				}
				else
					loppu_punaa(ar);
			}
		}
	}
}

void uudet_paikat(struct areena *a)
{
	int i;

	for (i = 0; i < a->alusten_maara && !a->stop; i++)
		uusi_paikka(a, &a->alukset[i]);
}




