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

	ret = recv(c->sock, &upd_msg, sizeof(upd_msg), 0);
	if (ret != sizeof(upd_msg))
		return;
	if (upd_msg.hdr.command != CMD_CLI_UPDATE)
		return;

	update_cli_at_server_storage(&upd_msg, c->id);
}

void server_alus_update_pos(struct alus_server_data *a, struct timespec *tm)
{
	printf("%s not implemented.", __func__);
	exit(1);
}

void position_update()
{
	int ret, i;
	struct timespec tm;
//	unsigned int time_delta_us = 0;
	struct areena_server_data sad_copy;

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
		server_alus_update_pos(a, &tm);
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
		if (ret) {
			/* TODO: Return error if send fails because client
			 * has disconnected
			 */
			perror("send()\n");
			printf("Failed to send server data to client %d\n", i);
		}
		printf("Sent arena update to client %d\n", i);
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

