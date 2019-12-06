#include <stdbool.h>
#include "babbler/babbler.h"

struct connect_info {
	const char *remote_ip;
	short remote_port;
	const char *own_ip;
	short own_port;
};

int main(int arc, char *argv[])
{
	int ok, i;
	const char *nimi;
	static struct areena a;
	struct connect_info ci;

	ci.remote_ip = "127.0.0.1";
	ci.own_ip = "127.0.0.1";
	
	ci.remote_port = 16002;
	ci.own_port = 16001;

	srand(time(NULL));

	SDL_Log("leveys %u, korkeus %u\n",a.leveys, a.korkeus);

	a.leveys_offset = (a.leveys - WINDOW_X)/2;
	a.korkeus_offset = (a.korkeus - WINDOW_Y)/2;

	a.leveys = WINDOW_X;
	a.korkeus = WINDOW_Y;

	ok = luo_areena(&a, true);
	if (ok)
		 goto err_out;

uusiksi:
	a.pisteet = 0;
	putsaa_pupit(&a);
	ok = luo_alukset(&a);
	a.stop = 0;
	if (ok)
		goto out_font;

	nimi = alkuruutu(&a);


	for (i = 0; 1 ; i++) {
		a.pisteet ++;
		SDL_SetRenderDrawColor(a.p.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(a.p.renderer);
		uudet_paikat(&a);
		if (i && !(i%100)) {
			lisaa_alus(&a);
			arvo_powerup(&a);
		}
		if (a.pisteet && !(a.pisteet%500))
			valipisteet(&a);

		if (a.piirra(&a))
		{
			talleta_pisteet(&a, nimi);
			goto uusiksi;
		}
		usleep(LOOP_DELAY_US);
		get_input(&a);
		if (a.realstop) {
			talleta_pisteet(&a, nimi);
			break;
		}
		SDL_RenderPresent(a.p.renderer);
	}

	Mix_FreeChunk( a.s.new_ship );
	Mix_FreeChunk( a.s.crash );
	Mix_FreeChunk( a.s.points );
	Mix_FreeMusic( a.s.music );
	Mix_CloseAudio();

out_font:
	TTF_CloseFont( a.p.font );	
	TTF_Quit();
	SDL_Quit();

	return 0;

err_out:
	SDL_Quit();
	return -1;

}
