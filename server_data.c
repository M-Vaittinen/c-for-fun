#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
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
}

void *cli_server_data_updater(void *dater)
{
	struct sdata_updater_args *a = (struct sdata_updater_args *)dater;
	struct client *c = &a->c;
	struct areena_server_data *asd = a->asd;
	int ret;

	for (;;) {
		struct server_data_update_msg tmp;

		/*
		 * UDP would be faster and better, right?
		 * OTOH - missing client updates would not be nice.
		 */
		ret = recv(c->sock, &tmp, sizeof(tmp), 0);
		if (ret != sizeof(tmp)) {
			printf("unexpected update message size %d. Out of sync?\n", ret);
			continue;
		}

		if (tmp.hdr.command != CMD_SERVER_DATA_UPDATE) {
			printf("Unexpected command %d. Expected CMD_SERVER_DATA_UPDATE (%d)\n",
				tmp.hdr.command, CMD_SERVER_DATA_UPDATE);
			continue;
		}
		printf("Received server data update\n");
		pthread_mutex_lock(&asd_mtx);
		*asd = tmp.asd;
		pthread_mutex_unlock(&asd_mtx);
	}

	return NULL;
/*
 * Bah. We have own thread here. We can just do blocking recv().
 * OTOH - we will need nonblocking stuff when we start sending client
 * dir-changes to the server.
 *
	fd_set rfds;
	FD_SET(c->sock, &rfds);

	for (;;) {
		FD_ZERO(&rfds);
		ret = select(c->sock + 1, &rfds, NULL, NULL, NULL);
		if (ret == -1) {
			perror("select()");
			continue;
		}
		if (ret) {
			struct server_data_update_msg tmp;
		}
	}
	*/
}

int start_server_updater(struct client *c)
{
	int ret;
	pthread_t tid;
	pthread_attr_t attr;

	g_args.c = *c;
//	asd = &g_asd;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	ret = pthread_create(&tid, &attr, cli_server_data_updater, &g_args);
	if (ret)
		printf("Client updater creation failed %d\n", ret);

	return ret;
}

