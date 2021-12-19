#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"
#include "msg.h"
#include "server_areena.h"
#include "server_data.h"

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
	sad->alusten_maara ++;
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
//	struct paikka etunurkka;
//	struct paikka oik_takanurkka;
//	struct paikka vas_takanurkka;

        float tangle;

        /* Paikat ennen kiertoa */
        pit = a->pituus/2.0;
        lev = a->leveys/2.0;
        pit_nurkka = sqrtf(pit*pit + lev*lev);

        tangle = atan(a->leveys/a->pituus);

        /* Kierrä suunnan mukaan */

        change_y = sinf(angle) * pit;
        change_x = cosf(angle) * pit;

        a->corners.etunurkka.x = a->p.x + change_x;
        a->corners.etunurkka.y = a->p.y + change_y;

        a->coll_max.x = a->coll_min.x = a->corners.etunurkka.x;
        a->coll_max.y = a->coll_min.y = a->corners.etunurkka.y;

        change_y = sin(M_PI + tangle + angle) * pit_nurkka;
        change_x = cos(M_PI + tangle + angle) * pit_nurkka;

        a->corners.oik_takanurkka.x = a->p.x + change_x;
        a->corners.oik_takanurkka.y = a->p.y + change_y;

        srv_coll_update(a, &a->corners.oik_takanurkka);

        change_y = sin(M_PI - tangle + angle) * pit_nurkka;
        change_x = cos(M_PI - tangle + angle) * pit_nurkka;

        a->corners.vas_takanurkka.x = a->p.x + change_x;
        a->corners.vas_takanurkka.y = a->p.y + change_y;

        srv_coll_update(a, &a->corners.vas_takanurkka);
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

	sad->leveys = WINDOW_X;
	sad->korkeus = WINDOW_Y;

	for (i = 0; i < alusten_maara; i++) {
		struct alus_server_data a;

		server_arvo_alus(&a, i, sad);
		server_lisaa_alus(sad, &a, i);
	}
}

void update_cli_at_server_storage(struct cli_update_to_server *upd, int id)
{
	struct timespec tm;
	int ret;

	ret = clock_gettime(CLOCK_MONOTONIC_COARSE, &tm);
	if (ret) {
		printf("Failed to get clock \n");
		tm.tv_sec = tm.tv_nsec = 0;
	}
	pthread_mutex_lock(&sad_lock);

	g_sad.alukset[id].suunta = upd->suunta;
	printf("Updated client %d direction to %d\n", id, (int)upd->suunta);
	if (tm.tv_sec != 0 || tm.tv_nsec != 0) {
		if (id == 0)
			g_sad.last_client1_update = tm;
		else
			g_sad.last_client2_update = tm;
	}
	pthread_mutex_unlock(&sad_lock);
}

void handle_cli_update(struct client *c)
{
	int ret;
	struct cli_update_to_server upd_msg;

	ret = recv(c->sock, &upd_msg, sizeof(upd_msg), MSG_WAITALL);
//	printf("Server received msg from client %d, sock %d, size %d, expected %ld\n", c->id, c->sock, ret, sizeof(upd_msg));
	if (!ret)
		exit(0);

	if (ret != sizeof(upd_msg))
	{
		perror("recv()\n");
		return;
	}
	if (upd_msg.hdr.command != CMD_CLI_UPDATE) {
		printf("Unexpected command. Expected CMD_CLI_UPDATE: got %d\n", upd_msg.hdr.command);
		return;
	}

	update_cli_at_server_storage(&upd_msg, c->id);
}

void game_over(struct areena_server_data *sad, struct alus_server_data *a)
{
	/* TODO: Mark player whose alus it was as lost */
	/* do something else */
}
void server_alus_update_pos(struct areena_server_data *sad, struct alus_server_data *a, struct timespec *tm)
{
//	printf("%s not implemented.\n", __func__);

//void uusi_paikka(struct areena *ar, struct alus *a)
//{
	int nop_x, nop_y, i;
	float angle;
	int matka;
//	struct alus_server_data *cli1 = &sad->alukset[0];
//	struct alus_server_data *cli2 = &sad->alukset[1];
	bool *loser = NULL;
	unsigned int *pts = NULL;
	unsigned long long int nsec = tm->tv_nsec;;
	unsigned long long int time_delta_us;
	int liik_x = 0, liik_y = 0;

	if (!sad->last_server_update.tv_sec && !sad->last_server_update.tv_nsec) {
		time_delta_us = 0;
		sad->last_server_update.tv_sec = tm->tv_sec;
		sad->last_server_update.tv_nsec = tm->tv_nsec;

		return;
	}

	time_delta_us = (tm->tv_sec - sad->last_server_update.tv_sec) * 1000000;
	time_delta_us += (nsec - sad->last_server_update.tv_nsec) / 1000;

	if (time_delta_us < MIN_SERVER_UPDATE_TIME_US)
		return;

	sad->last_server_update = *tm;

	if (a->id == 0) {
		loser = &sad->player1_lost;
		pts = &sad->pisteet_id1;
	} else if (a->id == 1) {
		loser = &sad->player2_lost;
		pts = &sad->pisteet_id2;
	}

	if (oonko_jaassa(&a->pups))
		matka = 0;

	else if (oonko_noppee(&a->pups))
		matka = NOP_MAX * (time_delta_us / MIN_SERVER_UPDATE_TIME_US);
	else
		matka = a->nopeus * (time_delta_us / MIN_SERVER_UPDATE_TIME_US);

	if (!matka)
		goto paikka_paivitetty;

	angle = a->suunta * M_PI / 180.0f;
	nop_y = sinf(angle) * matka;
	nop_x = cosf(angle) * matka;

	if (!nop_y && !nop_x)
		goto paikka_paivitetty;

	a->p_delta.x += nop_x;
	while (a->p_delta.x > NOP_MAX) {
		a->p.x++;
		a->p_delta.x -= NOP_MAX;
		liik_x++;
	}
	while (a->p_delta.x < -NOP_MAX) {
		a->p.x--;
		a->p_delta.x += NOP_MAX;
		liik_x--;
	}

	a->p_delta.y += nop_y;
	while (a->p_delta.y > NOP_MAX) {
		a->p.y++;
		a->p_delta.y -= NOP_MAX;
		liik_y++;
	}
	while (a->p_delta.y < -NOP_MAX) {
		a->p.y--;
		a->p_delta.y += NOP_MAX;
		liik_y--;
	}
	printf("Alus %d liikahtaa X sunntaan %d, Y suuntaan %d\n", a->id, liik_x,
	       liik_y);


	if ( a->p.x <= 0 ) {
		if (oonko_haamu(&a->pups) || (a->oma && oonko_kuolematon(&a->pups))) {
			/* Mee seinän läpi */
			a->p.x = sad->leveys;
			goto x_paivitetty;
		}
		if (loser) {
			*loser = true;
			printf("pellaja %d tormasi seinään (paikka X=%d)\n", a->id, a->p.x);
			game_over(sad, a);
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
	if ( a->p.x >= sad->leveys ) {
		if (oonko_haamu(&a->pups) || (a->oma && oonko_kuolematon(&a->pups))) {
			/* Mee seinän läpi */
			a->p.x = 0;
			goto x_paivitetty;
		}
		if (loser) {
			*loser = true;
			printf("pellaja %d tormasi seinään\n", a->id);
					game_over(sad, a);
			goto paikka_paivitetty;
		}

		a->p.x=sad->leveys-1;
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
		if (oonko_haamu(&a->pups) || (a->oma && oonko_kuolematon(&a->pups))) {
			/* Mee seinän läpi */
			a->p.y = sad->korkeus;
			goto paikka_paivitetty;
		}
		if (loser) {
			*loser = true;
			printf("pellaja %d tormasi seinään\n", a->id);
					game_over(sad, a);
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
	if (a->p.y >= sad->korkeus) {
		if (oonko_haamu(&a->pups) || (a->oma && oonko_kuolematon(&a->pups))) {
			/* Mee seinän läpi */
			a->p.y = 0;
			goto paikka_paivitetty;
		}
		if (loser) {
			*loser = true;
			printf("pellaja %d tormasi seinään\n", a->id);
					game_over(sad, a);
			goto paikka_paivitetty;
		}

		if (a->p.y >= sad->korkeus) {
			a->p.y = sad->korkeus - 1;
			a->p_delta.y = 0;
		}

		if ( a->suunta <= 180 )
			a->suunta = 360.0 - a->suunta;


	}
paikka_paivitetty:

	srv_alus_laske_nurkat(a);

	if (a->id == 0 || a->id == 1)
	{
	/* TODO: Remove expired power-ups at server side */
	//	poista_vanhat_pupit(a);
//void kato_pupit(int *active_pups, struct puppipuskusri *pups, struct areena *ar)
		kato_pupit(&sad->active_pups, &a->pups, pts, NULL);
	}

	if (a->id == 0 || a->id == 1)
		return;

	for ( i = 0; i < 2; i++) {
	struct alus_server_data *c = &sad->alukset[i];

	/* Spaghetti spaghetti spaghetti... */
	if (i)
		loser = &sad->player2_lost;
	else
		loser = &sad->player1_lost;

	if (!oonko_kuolematon(&c->pups)) {
		if ( srv_o_iholla(c, a)) {
			if ( tormasi(&c->corners, &a->corners)) {
				if (oonko_rikkova(&c->pups)) {
					if (!a->rikki) {
						/* TODO: Add points */
		//				lisaa_rikkopisteet(ar, oma);
						//pysayta_alus(a);
						a->nopeus = 0;
						a->rikki = 1;
					}
				}
				else
					*loser = true;
					//game_over(sad, c);
		//			loppu_punaa(ar);
			}
		}
	}
	}
//}




//	exit(1);
}

void position_update()
{
	int ret, i;
	struct timespec tm;
//	unsigned int time_delta_us = 0;
	struct areena_server_data sad_copy;

	printf("update positions at server\n");

	pthread_mutex_lock(&sad_lock);
	sad_copy = g_sad;
	pthread_mutex_unlock(&sad_lock);

	ret = clock_gettime(CLOCK_MONOTONIC_COARSE, &tm);
	if (ret) {
		/* We could assume 5ms delay which is the default tmo for
		 * select. In many cases that could be (close to) Ok even if
		 * player does constantly move the mouse because also the client
		 * is likely to have some sleep in order to not hog all the CPU.
		 *
		 * Well, I do not do that because - if the tmo is for some
		 * reason shorter (which is fully possible and also even likely)
		 * then we could at next iteration end up having _negative_ time
		 * elapse - which in turn would cause horror, terror and error :]
		 *
		 * Thus we just bail-out here and skip the iteration. Sorry
		 * client who missed the direction update :(
		 */
		printf("%s: Failed to get clock \n", __func__);
		return;
	}

	for (i = 0; i < sad_copy.alusten_maara; i++) {
		struct alus_server_data *a;

		a = &sad_copy.alukset[i];
		printf("update alus %d, id %d\n", i, a->id);
		server_alus_update_pos(&sad_copy, a, &tm);
	}
	/* TODO: this won't do. We have a race here. We must either keep the
	 * sad locked for whole func or see if someone has updated it
	 * in-between and redo the calculation here. I think keeping it locked
	 * is Ok. Then we don't need sad_copy either, we can directly update
	 * the storage.
	 */
	pthread_mutex_lock(&sad_lock);
	g_sad = sad_copy; /* = g_sad */;
	pthread_mutex_unlock(&sad_lock);
}

void * server_update_thread(void *data)
{
	struct client *cli_tbl = (struct client *)data;
	struct client *c1, *c2;
	fd_set rfds;
	int suurin_sukka;

	c1 = &cli_tbl[0];
	c2 = &cli_tbl[1];

	 /*
	 * use select() to check at the end of each position update update loop
	 * whether the clients have sent direction updates.
	 */
	FD_ZERO(&rfds);
	suurin_sukka = MAX(c1->sock, c2->sock);

	for (;;) {
		int ret;
		/* We update things by 5ms interval - or shorter if clients send
		 * more frequent position updates
		 */
		struct timeval tmo = {
			.tv_sec = 0,
			.tv_usec = 5000,
		};

		position_update();
		FD_SET(c1->sock, &rfds);
		FD_SET(c2->sock, &rfds);

		ret = select(suurin_sukka + 1, &rfds, NULL, NULL, &tmo);
		if (ret == -1) {
			perror("select()");
			continue;
		}
		if (ret) {
			if (FD_ISSET(c1->sock, &rfds))
				handle_cli_update(c1);
			if (FD_ISSET(c2->sock, &rfds))
				handle_cli_update(c2);
		}
	}

}

int server_send_arena(struct client *cli_tbl, int num_cli)
{
	struct server_data_update_msg msg;
	int i, ret;

	if (num_cli != 2) {
		printf("Unexpected num_cli %d\n", num_cli);
		return -EINVAL;
	}

	msg.hdr.size = sizeof(msg);
	msg.hdr.command = CMD_SERVER_DATA_UPDATE;
	pthread_mutex_lock(&sad_lock);
	msg.asd = g_sad;
	pthread_mutex_unlock(&sad_lock);

	for (i = 0; i < num_cli; i++) {
		ret = send(cli_tbl[i].sock, &msg, sizeof(msg), 0);
		if (ret != sizeof(msg)) {
			/* TODO: Return error if send fails because client
			 * has disconnected
			 */
			perror("send()\n");
			printf("Failed to send server data to client %d, sock %d\n", i, cli_tbl[i].sock);
		}
		else
			printf("Sent arena update to client %d, sock %d\n", i, cli_tbl[i].sock);
	}
	return 0;
}

int starttaa_server_areena_updater(struct client *cli_tbl, int num_cli)
{
	int ret;
	pthread_t tid;
	pthread_attr_t attr;

	if (num_cli != 2) {
		printf("Unexpected num_cli %d\n", num_cli);
		return -EINVAL;
	}

//	g_sa.s = *s;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/* Client tbl is static - we can just pass a pointer to it.
	 * If the tbl in main server ever gets to be allocated from stack
	 * we must copy the data to a better place. Now I don't bother.
	 */
	ret = pthread_create(&tid, &attr, server_update_thread, cli_tbl);
	if (ret)
		printf("%s: Thread creation failed %d\n", __func__, ret);

	return ret;
}

