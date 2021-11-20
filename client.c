#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "connection.h"
#include "client.h"
#include "msg.h"
#include "server_data.h"

int connect_client(struct client *c, struct server *s)
{
	int ret;
	socklen_t len = sizeof(s->ad);

	if (!s || !c) {
		printf("No server or client\n");
		return -1;
	}

	c->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	c->addr = s->ad;
	ret = connect(c->sock, (const struct sockaddr *)&s->ad, len);
	if (ret == -1) {
		perror("Connect()\n");
		return -1;
	}
	printf("Client Connected\n");

	return 0;
}

int client_get_id(struct client *c, struct server *s)
{
	int ret;
	struct msg_client_rdy rdy;
	struct msg_client_rdy_resp rsp;

	rdy.hdr.size = sizeof(rdy);
	rdy.hdr.command = CMD_CLIENT_RDY;

	ret = send(c->sock, &rdy, sizeof(rdy), 0);
	if (ret != sizeof(rdy)) {
		perror("send\n");
		printf("send return %d\n", ret);
		return -1;
	}

	ret = recv(c->sock, &rsp, sizeof(rsp), 0);
	if (ret != sizeof(rsp)) {
		perror("recv\n");
		printf("recv return %d\n", ret);
		return -1;
	}
	if (rsp.hdr.command != CMD_CLIENT_RDY_RESP) {
		printf("Unexpected resp command %d, expected CMD_CLIENT_RDY_RESP %d\n", rsp.hdr.command, CMD_CLIENT_RDY_RESP);
		return -1;
	}
	c->id = rsp.client_id;

	return 0;
}
