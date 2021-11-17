#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "babbler/babbler.h"

struct connect_info {
	const char *remote_ip;
	short remote_port;
	const char *own_ip;
	short own_port;
};

struct connection {
	struct babbler *b_rx;
	struct babbler *b_tx;
	struct babbler *b_tx2;
};

/* We keep this endianess independent value and don't bother converting it */
#define CLIENT_REGISTER 0x01000001

struct client_register_msg {
	uint32_t cmd;
};

struct client_register_resp {
	uint32_t id;
};
#endif
