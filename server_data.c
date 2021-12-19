#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include "areena.h"
#include "client.h"
#include "server_data.h"
#include "msg.h"

struct sdata_updater_args {
/*	struct server *s; */
	struct client c;
	struct areena_server_data *asd;
};

static struct sdata_updater_args g_args;

pthread_mutex_t asd_mtx = PTHREAD_MUTEX_INITIALIZER;
static struct areena_server_data g_asd;

void client_get_serverdata(struct client *c, struct areena_server_data *d)
{
	/* TODO: Get data from server? See how many layers of background
	 * activity we need...
	 */
	pthread_mutex_lock(&asd_mtx);
	*d = g_asd;
	pthread_mutex_unlock(&asd_mtx);
/*
	printf("sent by server at %ld %ld, recvd by client at %ld %ld\n",
	       d->last_server_update.tv_sec, d->last_server_update.tv_nsec,
	       d->last_server_update_c.tv_sec, d->last_server_update_c.tv_nsec);
*/
}

void *cli_server_data_updater(void *dater)
{
	struct sdata_updater_args *a = (struct sdata_updater_args *)dater;
	struct client *c = &a->c;
	struct areena_server_data *asd = a->asd;
	int ret;

	for (;;) {
		struct server_data_update_msg tmp;
		struct timespec ts;

		/*
		 * UDP would be faster and better, right?
		 * OTOH - missing client updates would not be nice.
		 */
		printf("Waiting for data from client socket %d\n", c->sock);
		ret = recv(c->sock, &tmp, sizeof(tmp), MSG_WAITALL);
		if (ret != sizeof(tmp)) {
			printf("unexpected update message size %d. Exoected %d Out of sync?\n", ret, (int)sizeof(tmp));
			continue;
		}

		if (tmp.hdr.command != CMD_SERVER_DATA_UPDATE) {
			printf("Unexpected command %d. Expected CMD_SERVER_DATA_UPDATE (%d)\n",
				tmp.hdr.command, CMD_SERVER_DATA_UPDATE);
			continue;
		}
		ret = clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
		if (ret) {
			printf("Failed to get clock \n");
			ts.tv_sec = ts.tv_nsec = 0;
		}
		printf("Received server data update\n");
		pthread_mutex_lock(&asd_mtx);
		asd->last_server_update_c = ts;
		*asd = tmp.asd;
		pthread_mutex_unlock(&asd_mtx);
	}

	return NULL;
}

int start_server_updater(struct client *c)
{
	int ret;
	pthread_t tid;
	pthread_attr_t attr;

	g_args.c = *c;
	g_args.asd = &g_asd;

	printf("Launching clieng side data receiver\n");

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	ret = pthread_create(&tid, &attr, cli_server_data_updater, &g_args);
	if (ret)
		printf("Client updater creation failed %d\n", ret);

	return ret;
}

