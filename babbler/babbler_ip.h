#ifndef __BABBLER_IP
#define __BABBLER_IP

#include "babbler.h"
#include <stdint.h>
#include <arpa/inet.h>

struct socket_babbler_udp {
	struct babbler b;
	uint32_t myip;
	uint32_t serverip;
	unsigned short rxport;
	unsigned short txport;
	int (*raw_send) (struct babbler *b, struct sockaddr_in *to,
			 const char *buf, size_t *size);
	int (*raw_recv) (struct babbler *b, struct sockaddr_in *from,
			 char *buf, size_t *size);
};

#endif

