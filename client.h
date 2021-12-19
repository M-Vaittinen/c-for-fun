#ifndef _CLIENT_H
#define _CLIENT_H

#include "server.h"

struct client {
	int id;
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_len;
//	int sockaddr_in addr;
};

int connect_client(struct client *c, struct server *s);
int client_get_id(struct client *c, struct server *s);
int start_server_updater(struct client *c);
void send_suunta_to_server(struct client *c, float suunta);

#endif
