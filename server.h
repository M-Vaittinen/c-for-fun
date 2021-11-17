#ifndef __SERVER_H
#define __SERVER_H

#include <arpa/inet.h>

struct server {
	char ip[33];
    	struct sockaddr_in ad;
	short int port;
};

int server_start(struct server *s);

#endif
