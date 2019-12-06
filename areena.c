#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "areena.h"
#include "helpers.h"
#include "piirrettavat_tekstit.h"


static const char *rikkopts_str = "100";

static int alusta_seinat(struct areena *a)
{
	int i;
	struct SDL_Color v = {
		.r = 255,
		.g = 255,
		.b = 255,
		.a = SDL_ALPHA_OPAQUE,
	};
	struct paikka alut[] = { 
		{ .x = 0, .y =  0},
		{ .x = a->leveys, .y =  0 },
		{ .x = a->leveys, .y = a->korkeus },
		{ .x = 0, .y = a->korkeus },
	};
	struct paikka loput[] = {
		{ .x = a->leveys, .y = 0 },
		{ .x = a->leveys, .y = a->korkeus },
		{ .x = 0, .y = a->korkeus },
		{ .x = 0, .y = 0 },
	};

	if (a->seinien_maara != 4) {
		printf("outo seinamaara %d\n", a->seinien_maara);
		return -EINVAL;
	}

	for (i = 0; i < 4; i++)
		alusta_seina(&a->seinat[i], &alut[i], &loput[i], &v);

	return 0;
}


int piirra_areena(struct areena *a)
{
	int i;

	for (i = 0; i < a->seinien_maara; i++)
		a->seinat[i].piirra(a, &a->seinat[i]);

	for (i = 0; i < a->alusten_maara; i++)
		a->alukset[i].piirra(a, &a->alukset[i]);

	hanskaa_pupit(a);
	piirra_tekstit(a);

	if (a->stop) {
		char pisteet[255];
		SDL_Color valk = {255, 255, 255, SDL_ALPHA_OPAQUE/2 };
		struct paikka p = { .x = a->leveys/2 - 50,
				    .y = a->korkeus/2 -50, };

		snprintf(pisteet, 255, "%u", a->pisteet);
		SDL_Log("Pisteita palajo? %s\n", pisteet);

		draw_text(a, pisteet, &p, 200, 200, &valk);
		SDL_RenderPresent(a->p.renderer);
		return -1;	
	}
	return 0;
}

void update_client_arena(struct areena *a, struct areena_update *au)
{
	
}

int get_current_arena(struct areena *a)
{
	struct areena_update au;
	int ret;

	ret = a->connection.send_update_req(&a->connection);
	if (!ret)
		return ret;

	ret = a->connection.get_update_resp(&a->connection, &au);
	update_client_arena(a, &au);
	return ret;
}

int client_register(struct areena *a)
{
	struct client_register_msg msg = {
		.cmd = CLIENT_REGISTER;
	};
	struct client_register_resp resp;
	int ret, size;

	size = sizeof(msg);
	ret = a->connection.b_tx->send(a->connection.b_tx, (char *)&msg, &size);
	if (ret || size != sizeof(msg)) {
		printf("SEND returned %d\n", ret);
		return ret;
	}

	size = sizeof(resp);
	ret = a->connection.b_tx->receive(a->connection.b_tx, (char *)&resp, &size);
	if (ret)
		return ret;

	/* Don't care about endianess - we don't convert ID when sending */
	a->connection.id = resp.id;
	printf("%s: Connection ID %d\n", a->connection.id);
	return 0;
}

int connect_to_server(struct connection *connection, struct connect_info *ci)
{
	int ret;
	struct babbler_init_udp init = {
		.bs = {
			.b = {
				.bab = babbler_socket,
				.label = "rxtx",
				.mode_flag = O_RDWR,
			},
			.protocol = IPPROTO_UDP,
		},
		.txport = ci->port,
	};

	strcpy(init.bs.myip, ci->ip);
	a->connection.b_tx = create_babbler(&u.bs.b, sizeof(u));
	if (!a->connection.b_tx)
		return -1;

	a->connection.b_tx->open(a->connection.b_tx);

	return 0;
}

int client_register_to_server(struct areena *a, struct connect_info *ci)
{
	int ret;

	ret = connect_to_server(a->connection, ci);
	if (ret)
		return ret;

	ret = client_register(a);

	return ret;
}

int clien_get_initial_areena(struct areena *a, struct connect_info *ci)
{
	int ret;

	ret = client_register_to_server(a, ci);
	if (ret)
		return ret;

	ret = get_current_arena(a);
	if (ret)
		return ret;
}

int create_server_babbler(struct areena *a, struct connect_info *ci)
{
	struct babbler_init_udp init = {
		.bs = {
			.b = {
				.bab = babbler_socket,
				.label = "rx",
				.mode_flag = O_RDONLY,
			},
			.protocol = IPPROTO_UDP,
		},
		.rxport = ci->port,
	};

	strcpy(init.bs.myip, ci->ip);
	a->connection.b_rx = create_babbler(&u.bs.b, sizeof(u));
	if (!a->connection.b_rx)
		return -1;

	a->connection.b_rx->open(a->connection.b_rx);

	return 0;
}
int wait_for_client(struct areena *a, int clientno)
{
	struct babbler_set bs;
	struct timeval tmo;
	struct client_register_msg msg;
	int size, ret;
	struct client_register_resp resp;
	struct babbler *b;
	struct socket_babbler_udp *ub;
	struct sockaddr_in client;

	resp.id = clientno;

	retry:
	b = a->connection.b_rx;
	ub = (struct socket_babbler_udp *)a->connection.b_rx;
	tmo = {1,1};
	init_bs(&bs);
	b->add_to_poll(b, &bs);

	while (!bs.block(&bs, &tmo)) {
		printf("Server waiting for connections...\n");
	}
	size = sizeof(msg);
	ret = ub->raw_recv(b, &client,  &msg, &size);
//	ret = rx->receive(a->connection.b_rx, &msg, &size);
	if (size != sizeof(msg) || ret) {
		printf("Strange stuff received, size %d, ret %d\n", size, ret);
		goto retry;
	}
	if (clientno == 1)
		b = a->connection.b_tx;
	else
		b = a->connection.b_tx2;

	server_create_tx_babbler(b, &client);
}
int luo_areena(struct areena *a, struct connect_info *ci, bool serveri)
{
	int ok;

	a->stop = 0;
	a->realstop = 0;
	a->seinien_maara = 4;
	a->seinat = calloc(a->seinien_maara, sizeof(*a->seinat));
	if (!a->seinat) {
		printf("Muisti loppu\n");
		return -ENOMEM;
	}
	ok = alusta_seinat(a);
	if (ok) {
		printf("Seinen alustus män mönkään\n");
		return -1;
	}
	if (serveri) {
		ok = create_server_babbler(a, ci);
		if (ok)
			return ok;

		ok = wait_for_client(a, 1);
		/*
			wait_for_2nd_client(a);
		*/

		memset(&a->pups[0], 0, sizeof(a->pups));
		a->active_pups = 0;

	} else {
		clien_get_initial_areena(&a, ci);
		a->piirra = piirra_areena;
	}

	return 0;
}


void lisaa_rikkopisteet(struct areena *ar, struct alus *oma)
{
	struct pirrettava_teksti *pt = varaa_piirrospaikka();

	ar->pisteet += 100;
	if (!pt) {
		SDL_Log("Piirrospooli täys\n");
		return;
	}
	else
		SDL_Log("Lisataan rikkopts %p\n", rikkopts_str );

	pt->teksti = rikkopts_str;
	SDL_Log("Added str %s %p\n",pt->teksti, pt->teksti);
	pt->nakyvilla_kierros = 20;
	pt->p = oma->p;
	pt->leveys = ar->leveys/20;
	pt->korkeus=ar->korkeus/20;
	pt->kokomuutos_kierroksia = 20;
	pt->kokomuutos_x_kierros = 0;
	pt->kokomuutos_y_kierros = 0;
	pt->v.r = 255;
	pt->v.g = 255;
	pt->v.b = 255;
	pt->v.a = 255;
}

int arvo_powerup(struct areena *ar)
{
	if (ar->active_pups < MAX_PUPS) {
		unsigned long long chance = rand() %UUDEN_PUPIN_TSAANNSSI;

		if (!chance) {
			/* Arpa suosi, tehdään uus poweruppi */

			int i;
			struct powerup *p = &ar->pups[0];
			struct paikka pa;

			for (i = 0; i < MAX_PUPS && p->expire; i++, p++);

			if (i == MAX_PUPS) {
				SDL_Log("Viiiirrhheita Koodissa\n");
				return -1;
			}
			ar->active_pups++;
			pa.x = PUPS_KOKO + (rand() % ar->leveys) - 2*PUPS_KOKO;
			pa.y = PUPS_KOKO + (rand() % ar->korkeus) - 2*PUPS_KOKO;
			create_random_powerup_to_place(p, PUPS_KOKO, &pa);
		}
	}
	return 0;
}



