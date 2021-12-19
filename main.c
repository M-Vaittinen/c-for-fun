#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include "paikka.h"
#include "seina.h"
#include "alus.h"
#include "media.h"
#include "areena.h"
#include "common.h"
#include "connection.h"
#include "hiscore.h"
#include "piirrettavat_tekstit.h"
#include "helpers.h"
#include "server.h"
#include "client.h"
#include "client_arena.h"

#define VERSION "0.1 - \"suuttunut siivoaja\""

#define MOVE_THRESH 10
#define MOVE_JUMP_THRESH 6000

#define PISTEET_NAKYY_KIERROSTA 75
#define PISTEKOON_PIENENNYS_ALUSSA 150
#define PISTEKOON_MUUTOS_STEPPI 10
#define PISTEKOKO_SAMANA_LOOPIT 5

/* Should use cond variable or some nicer solution */
pthread_mutex_t g_ugly_solution = PTHREAD_MUTEX_INITIALIZER;
int g_server_state = 0;

void print_version()
{
	printf("Version %s\n", VERSION);
}
void print_help(char *name)
{
	printf("Käyttö:\n");
	printf("%s [ --server <server-ip> ] [ --start-server ] [ --help ]\n",
	       name);
	printf(" -s --server:\n");
	printf("		Server IP for two-player. \n" \
	       "		One of the players must start the server,\n");
	printf("		Second one can just connect to it. Both must\n");
	printf("		specify the IP used by the server starter\n");
	printf(" -S --start-serves:\n");
	printf("		Start the server\n");
	printf(" -h --help:\n");
	printf("		Display this help and exit\n");
	printf(" -v --version:\n");
	printf("		Display version information and exit\n");
}

int music_init(struct sounds *s)
{
	int ret;

	memset(s,0,sizeof(*s));

	//Initialize SDL_mixer
	if( (ret = Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ))
		return ret;

	/* Taustamusiikki */
	s->music = Mix_LoadMUS( "/home/mvaittin/.kolomiosnd/Blazer Rail.wav" );
	/* pisteet 500 */
	s->points = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/glass_breaking_2.wav" );
	/* törmäys */
	s->crash = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/kolmiopelihavio.wav" );
	/* Spawnaus */
	s->new_ship = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/bottle_pop_2.wav" );
	s->pupaanet[PUP_SPEED] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/NOPEUS.wav");
	s->pupaanet[PUP_COOL] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/UPPEE.wav");
	s->pupaanet[PUP_IMMORTAL] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/tiktok.wav");
	s->pupaanet[PUP_DESTROY] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/nauru.wav");
	s->pupaanet[PUP_FREEZE] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/");
	s->pupaanet[PUP_PASS_WALLS] = Mix_LoadWAV( "/home/mvaittin/.kolomiosnd/WARP.wav");

	return !(s->music && s->new_ship && s->crash && s->points);
}

/* Tää ei toimi jos puskuri on täys */
#define jokaselle_saadulle_powerupille(a,p) \
for ((p) = &(a)->pups.pbuf[(a)->pups.first]; (a)->pups.first != (a)->pups.last && (p) != &(a)->pups.pbuf[(a)->pups.last + 1]; (p)++)

void togglefullscreen(struct areena *a, SDL_Window* window, SDL_Renderer* renderer)
{
	Uint32 flags = SDL_GetWindowFlags(window);

	flags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
	SDL_SetWindowFullscreen(window, flags | SDL_WINDOWPOS_CENTERED);

	if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP))
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_RenderSetLogicalSize(renderer, a->leveys, a->korkeus);
	}
	else
	{
		SDL_SetWindowSize(window, a->leveys, a->korkeus);
	}
}

void test_display(struct areena *a, SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, a->leveys + a->leveys_offset,
				 a->korkeus + a->korkeus_offset);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void get_input(struct areena *a, int id)
{
	struct alus *oma = &a->alukset[id];
	float fix = 0;
	const Uint8 *state;
	struct paikka hiiri = {0,0};;

	/* Is this needed anymore? */
	SDL_PumpEvents();
	state = SDL_GetKeyboardState(NULL);

	if(state) {
		if (state[SDL_SCANCODE_UP])
			oma->nopeus+=100;
		if (state[SDL_SCANCODE_DOWN])
			oma->nopeus-=100;
	}

	if (oma->nopeus < 0)
		oma->nopeus = 0;
	if (oma->nopeus > NOP_MAX)
		oma->nopeus = NOP_MAX;

	if (SDL_GetMouseState(&hiiri.x,&hiiri.y) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		a->realstop = 1;
		return;
	}

	hiiri.x -= a->leveys_offset;
	hiiri.y -= a->korkeus_offset;

	if ((!oonko_kuolematon(&oma->pups)) && isin_kolmio(&oma->corners.vas_takanurkka, &oma->corners.oik_takanurkka,
	    &oma->corners.etunurkka, &hiiri)) {
		/* TBD: ponder if this should be done or not. */
		/* If yes, the server should be informed. But this is a place
		 * for a cheat :/
		 */
		SDL_Log("Törmäsit hiireen!\n");
		loppu_punaa(a);
	}

	hiiri.x -= oma->p.x;
	hiiri.y -= oma->p.y;

	if (hiiri.x < 0)
		fix = -180.0;

	if (!hiiri.y) {
		if (hiiri.x<0)
			oma->suunta = 180;
		else if (!hiiri.x)
			;
		else
			oma->suunta = 0;
	} else if (!hiiri.x) {
		if (hiiri.y<0)
			oma->suunta = 90;
		else
			oma->suunta = 270;
	} else
		oma->suunta = 180.0/M_PI * atan((float)-hiiri.y/(float)-hiiri.x);
	oma->suunta += fix;
	if (oma->suunta < 0)
		oma->suunta += 360;

}
void valipisteet(struct areena *ar)
{
	static int i = 0, j = 0;
	static char pisteet[255];
	struct pirrettava_teksti *pt = varaa_piirrospaikka();

	static char *ptsptr;

	struct paikka p = { .x = ar->leveys/2 - 100, .y = ar->korkeus/2 -100, };

	struct SDL_Color v_table[] = {
		{ 255, 255, 255, SDL_ALPHA_OPAQUE },
		{ 0, 255, 255, SDL_ALPHA_OPAQUE },
		{ 255, 0, 255, SDL_ALPHA_OPAQUE },
		{ 255, 255, 0, SDL_ALPHA_OPAQUE },
		{ 0, 0, 255, SDL_ALPHA_OPAQUE },
		{ 255, 0, 0, SDL_ALPHA_OPAQUE },
		{ 0, 255, 0, SDL_ALPHA_OPAQUE },
	};

 	ptsptr = &pisteet[i*10];

	i = (i+1)%10;

	snprintf(ptsptr, 10, "%u", ar->pisteet);
	ptsptr[9] = 0;

	if (!pt) {
		SDL_Log("Piirrospooli täys\n");
		return;
	}
	else
		SDL_Log("Lisataan valipiste\n");

	pt->teksti = ptsptr;
	pt->nakyvilla_kierros = PISTEET_NAKYY_KIERROSTA;
	pt->p = p;
	pt->leveys = 120;
	pt->korkeus = 60;
	pt->kokomuutos_kierroksia = PISTEKOKO_SAMANA_LOOPIT;
	pt->kokomuutos_x_kierros = PISTEKOON_MUUTOS_STEPPI;
	pt->kokomuutos_y_kierros = PISTEKOON_MUUTOS_STEPPI/2;
	pt->v = v_table[j];
	j++;
	j %= 7;


	Mix_PlayChannel( -1, ar->s.points, 0 );
}

#define PELAAJIA 5
static const char *nimi[PELAAJIA] = { "Muru", "Jasper", "Joona", "Iivari", "Mestari-Isi" };

const char * alkuruutu(struct areena *a)
{
	struct paikka p[PELAAJIA] = {
	};
	struct SDL_Color v = { 51, 221, 255, SDL_ALPHA_OPAQUE };
	uint32_t state;
	int korkeus = a->korkeus/PELAAJIA;
	int y,i;

	p[0].x = 100,
	p[0].y = a->korkeus/4 -5,

	draw_text(a, "Aloita Peli", &p[0], 500, 100, &v);
	p[0].x = 100;
	p[0].y = (a->korkeus/4)*3 -100;
	v.r = 225;
	v.g = 255;
	v.b = 255;

	draw_text(a, "Paina Nayttoa", &p[0], 500, 100, &v);

	v.r = 0;
	v.g = 225;
	v.b = 0;
	v.a = SDL_ALPHA_OPAQUE/2;

	for (i = 0; i < PELAAJIA; i++) {
		char tmp[255];
		unsigned int pisteet;

		pisteet = hae_pisteet(nimi[i]);

		p[i].x = a->leveys - a->leveys/4,
		p[i].y = i*korkeus;

		snprintf(tmp, 255, "%s %u", nimi[i], pisteet);
		tmp[254] = 0;

		draw_text(a, tmp, &p[i], a->leveys/4, korkeus, &v);
		SDL_RenderDrawLine(a->p.renderer, a->leveys_offset, p[i].y + korkeus + a->korkeus_offset, a->leveys + a->leveys_offset, p[i].y + korkeus + a->korkeus_offset);
	}

	SDL_RenderPresent(a->p.renderer);


	for (;;) {
		SDL_PumpEvents();
		if ((state = SDL_GetMouseState(NULL,&y)) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (i = 0; i < PELAAJIA; i++) 
				if (y < (i+1)*korkeus + a->korkeus_offset)
					return nimi[i];
			continue;
		}
		if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			a->realstop = 1;
			return NULL;
		}
		usleep(10000);
	}
}
#define OPTSTRING "Shs:v?"

static struct option long_options[] =
{
    {"start-server", no_argument, 0, 'S'},
    {"server" , required_argument, 0, 's'},
    {"version",  no_argument, 0, 'v'},
    {"help",  no_argument, 0, 'h'},
    {0,0,0,0}
};
static int parse_args(int argc, char *argv[], struct server *s)
{
	int index;
	int c;
	bool ip_given = false;
	int ret = 1;

	memset(s, 0, sizeof(*s));

	while(-1 != (c = getopt_long(argc, argv, OPTSTRING, long_options,
				     &index))) {
		switch(c)
		{
		case ':':
			return -1;
		case 'v':
			print_version();
			return 1;
		case '?':
			printf("Unknown parameter\n");
			ret = -1;
		case 'h':
			print_version();
			print_help(argv[0]);
			printf("Usage: ./ampu [-s server-ip ]\n");
			return ret;
        	case 's':
		{
			int str_len;
		/* server IP */

			if (!optarg) {
				printf ("-s missing IP\n");
				return -1;
			}
			str_len=strlen(optarg);
			if(str_len >= 32) {
				printf("Liika pitka IP. max 32 tavua\n");
				return -1;;
			} else {
				memcpy(s->ip, optarg, str_len+1);
				ip_given = true;
			}
			break;
		}
		case 'S':
			s->start = true;

			break;
		}
	}
	if (s->start && !ip_given) {
		printf("Server start requested, no IP goven\n");
		return -1;
	}

	if (ip_given) {
		printf("IP given as '%s', validate and convert to binary\n",
			s->ip);
		memset(&s->ad, 0, sizeof(s->ad));
		if(1 != inet_pton(AF_INET,s->ip, &s->ad.sin_addr)) {
			printf("Failed to convert ip %s", s->ip);
			return -1;
		}
		s->ad.sin_port = htons(SERVER_PORT);
		s->ad.sin_family = AF_INET;
	}

	return 0;
}

static void uus_data_serverilta(struct client *c, struct areena *a)
{
	struct areena_server_data sd;

	client_get_serverdata(c, &sd);
	update_areena_by_serverdata(a, &sd, c->id);
}

int main(int arc, char *argv[])
{
	int ok, i, ret;
	const char *nimi;
	static struct areena a;
	SDL_Window* window = NULL;
	struct server s;
	struct client c;
	bool use_server = false;
	struct alus *oma;

	srand(time(NULL));

	ok = parse_args(arc, argv, &s);
	if (ok) {
		if (ok == 1)
			ok = 0;
		return ok;
	}

	if (s.ip[0] != 0) {
		printf("Server given as '%s'\n", s.ip);
		use_server = true;
	} else {
		printf("No server\n");
	}

	if (s.start) {
		ret = server_start(&s);
		if (ret)
			return ret;
recheck:
		pthread_mutex_lock(&g_ugly_solution);
		if (!g_server_state) {
			pthread_mutex_unlock(&g_ugly_solution);
			sleep(1);
			goto recheck;
		}
		pthread_mutex_unlock(&g_ugly_solution);
		printf("server started\n");
	}

	if (s.ip[0] != 0) {
		ret = connect_client(&c, &s);

		if (ret)
			return ret;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	if (TTF_Init()) {
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &a.p.renderer)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		goto err_out;
	}

	SDL_GetWindowSize(window, &a.leveys, &a.korkeus);

	SDL_Log("leveys %u, korkeus %u\n",a.leveys, a.korkeus);

	a.leveys_offset = (a.leveys - WINDOW_X)/2;
	a.korkeus_offset = (a.korkeus - WINDOW_Y)/2;

	a.leveys = WINDOW_X;
	a.korkeus = WINDOW_Y;
	SDL_SetRenderDrawBlendMode(a.p.renderer, SDL_BLENDMODE_BLEND);

	if (use_server) {
		if (s.start) {
recheck2:
			pthread_mutex_lock(&g_ugly_solution);
			if (g_server_state != 3) {
				pthread_mutex_unlock(&g_ugly_solution);
				sleep(1);
				goto recheck2;
			}
			pthread_mutex_unlock(&g_ugly_solution);
		}
		ret = client_get_id(&c, &s);
		if (ret)
			return -1;
//		ret = get_arena(&s, &c, &a);
		ret = start_server_updater(&c);
		if (ret)
			return -1;

		printf("My client ID %d, sock %d\n", c.id, c.sock);
	}

	ok = luo_areena(&a);
	if (ok)
		 goto err_out;

	read_scores();

	a.p.font = TTF_OpenFont("/usr/share/fonts/liberation/LiberationMono-Regular.ttf", 24);
	if (!a.p.font) {
		SDL_Log("Fontti ei auennu %s\n",SDL_GetError());
		goto err_out;
	}
	if (music_init(&a.s))
		goto out_font;

	Mix_PlayMusic( a.s.music, -1 );
uusiksi:
	a.pisteet = 0;
	putsaa_pupit(&a);
	ok = luo_alukset(&a);
	a.stop = 0;
	if (ok)
		goto out_font;

	nimi = alkuruutu(&a);

	if (use_server && c.id < ALUKSET_MAX)
		oma = &a.alukset[c.id];
	else
		oma = &a.alukset[0];

	for (i = 0; 1 ; i++) {
		float suunta_vanha = oma->suunta;
		a.pisteet ++;
		SDL_SetRenderDrawColor(a.p.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(a.p.renderer);
		if (use_server)
			uus_data_serverilta(&c, &a);

		uudet_paikat(&a);
		if (i && !(i%100)) {
//			lisaa_alus(&a);
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
		get_input(&a, use_server?c.id:0);
		if (a.realstop) {
			talleta_pisteet(&a, nimi);
			break;
		}
		if (oma->suunta != suunta_vanha)
			send_suunta_to_server(&c, oma->suunta);
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
