#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

struct server_thread_args {
	struct server s;
};


static struct server_thread_args g_sa;

void *server_thread(void *param)
{
	struct server_thread_args *a = (struct server_thread_args *)param;
	int sock;

	if (!a) {
		/* Forcibly exit so the client won't stay up all alone */
		printf("NULL server thread ard\n");
		exit(-EINVAL);
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1) {
		perror("socket\n");
		exit(1);
	} else {
		printf("Socket %d created\n", sock);
	}

	for (;;) {
		printf("thread running - ip %s\n", a->s.ip);
		sleep(1);
	}

	return NULL;
}

int server_start(struct server *s)
{
	int ret;
	pthread_t tid;
	pthread_attr_t attr;

	g_sa.s = *s;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	ret = pthread_create(&tid, &attr, server_thread, &g_sa);
	if (!ret)
		printf("Thread creation failed %d\n", ret);

	return ret;
}
