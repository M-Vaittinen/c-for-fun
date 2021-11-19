#ifndef __SERVER_H
#define __SERVER_H

#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>

extern pthread_mutex_t g_ugly_solution;
extern int g_server_state;

struct server {
	char ip[33];
    	struct sockaddr_in ad;
//	short int port;
	bool start;
};

int server_start(struct server *s);

#endif
