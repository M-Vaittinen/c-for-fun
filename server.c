#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "connection.h"
#include "server.h"

struct server_thread_args {
	struct server s;
};


static struct server_thread_args g_sa;

struct client {
	int id;
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_len;
};

static struct client g_cl_table[2];

void *server_thread(void *param)
{
	struct server_thread_args *a = (struct server_thread_args *)param;
	int sock, ret, i;;
	struct sockaddr_in *addr;

	if (!a) {
		/* Forcibly exit so the client won't stay up all alone */
		printf("NULL server thread ard\n");
		exit(-EINVAL);
	}
	addr = &a->s.ad;
	addr->sin_port = htons(SERVER_PORT);
	addr->sin_family = AF_INET;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
		perror("socket\n");
		exit(1);
	} else {
		printf("Socket %d created\n", sock);
	}
	ret = bind(sock, (struct sockaddr *)addr, sizeof(*addr));
	if (ret == -1) {
		perror("bind\n");
		exit(1);
	}

	for (i = 0; i < 2; i++) {
		struct client *c = &g_cl_table[i];

		ret = listen(sock, 2 - i);
		if (ret == -1) {
			perror("listen\n");
			exit(1);
		}
		c->addr_len = sizeof(c->addr);
		c->sock = accept(sock, (struct sockaddr *)&c->addr, &c->addr_len);
		if (c->sock < 0) {
			perror("accept\n");
			exit(1);
		}
		printf("Client %d connected sock %d\n", i, c->sock);
		c->id = i;
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
	if (ret)
		printf("Thread creation failed %d\n", ret);

	return ret;
}
