#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "areena.h"
#include "connection.h"
#include "msg.h"
#include "server.h"
#include "server_areena.h"

struct server_thread_args {
	struct server s;
};

static struct server_thread_args g_sa;
/*
struct client {
	int id;
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_len;
};
*/
static struct client g_cl_table[2];

int give_client_id(struct server *s, struct client *c)
{
	struct msg_client_rdy rdy;
	int ret;
	struct msg_client_rdy_resp resp = {
		.hdr = {
			.size = sizeof(resp),
			.command = CMD_CLIENT_RDY_RESP,
		},
		.client_id = c->id,
	};

	ret = recv(c->sock, &rdy, sizeof(rdy), 0);
	if (ret != sizeof(rdy)) {
		if (ret > 0)
			printf("Unexpected message size %u\n", ret);
		else
			perror("recv failed\n");
		return -EINVAL;
	}

	if (rdy.hdr.command != CMD_CLIENT_RDY) {
		printf("Unexpected message %d, expected CMD_CLIENT_RDY (%d)\n",
			rdy.hdr.command, CMD_CLIENT_RDY);
		return -EINVAL;
	}
	printf("Server received CMD_CLIENT_RDY from client %d\n", c->id);

	ret = send(c->sock, &resp, sizeof(resp), 0);
	if (ret != sizeof(resp)) {
		printf("Unexpected returnvalue from send %d\n", ret);
		perror("send\n");
		return -EINVAL;
	}
	printf("Sent client ID to sock %d\n", c->sock);

	return 0;
}

void *server_thread(void *param)
{
	struct server_thread_args *ar = (struct server_thread_args *)param;
	int sock, ret, i;
	struct sockaddr_in *addr;
//	struct areena a;
	struct server *s;

	if (!ar) {
		/* Forcibly exit so the client won't stay up all alone */
		printf("NULL server thread ard\n");
		exit(-EINVAL);
	}
	s = &ar->s;
	addr = &ar->s.ad;

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
		printf("Server listening sock %d\n", sock);
		pthread_mutex_lock(&g_ugly_solution);
		if (g_server_state < 1)
			g_server_state = 1;
		pthread_mutex_unlock(&g_ugly_solution);
		c->addr_len = sizeof(c->addr);
		c->sock = accept(sock, (struct sockaddr *)&c->addr, &c->addr_len);
		if (c->sock < 0) {
			perror("accept\n");
			exit(1);
		}
		printf("Client %d connected sock %d\n", i, c->sock);
		fflush(stdout);
		c->id = i;

		pthread_mutex_lock(&g_ugly_solution);
		g_server_state = 3;
		pthread_mutex_unlock(&g_ugly_solution);
	}

	//luo_areena(&a);
	arvo_server_areena();
/*
 * After the initial hand-shakes and arena creation we do one more thread
 * for updating the arena. This one shall be just a thread sending the arena
 * information to clients.
 */
	for (i = 0; i < 2; i++) {
		struct client *c = &g_cl_table[i];

		ret = give_client_id(s, c);
		if (ret)
			exit(ret);
	}
	printf("Clients have IDs\n");

	/* Add some 'wait for both clients ready' messages if needed.
	 * Right now I don't see it necessary as bot clients should be up
	 * because they have both connected. Thus we just start computing the
	 * positions, getting inputs and sending updates right away.
	 */

	ret = starttaa_server_areena_updater(&g_cl_table[0], 2);
	if (ret)
		exit(ret);

	printf("Arena updater started\n");

	for (;;) {
		int ret;

		ret = server_send_arena(&g_cl_table[0], 2);
		if (ret)
			return NULL;

		printf("thread running - ip %s\n", ar->s.ip);
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
