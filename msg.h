#ifndef _MSG_H
#define _MSG_H


#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include "connection.h"
#include <errno.h>
#include "server_data.h"

#define CMD_CLIENT_RDY		1
#define CMD_CLIENT_RDY_RESP	2
#define CMD_GET_ARENA		3
#define CMD_GET_ARENA_RESP	4
#define CMD_SERVER_DATA_UPDATE	5
#define CMD_CLI_UPDATE		6


struct message {
	int size;
	int command;
};
struct server_data_update_msg {
	struct message hdr;
	/* Do we need time? Add if needed */
	struct areena_server_data asd;
};

struct cli_update_to_server {
	struct message hdr;
	float suunta; /* TODO: we could probably use INTs. 1 degree accuracy
			should be sufficient.
		      */
};

struct msg_client_rdy_resp {
	struct message hdr;
	int client_id;
};

struct msg_client_rdy{
	struct message hdr;
};

struct msg_get_arena {
	struct message hdr;
};

struct msg_get_arena_resp {
	struct message hdr;
};

#endif
