#ifndef _MSG_H
#define _MSG_H


#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include "connection.h"
#include <errno.h>

#define CMD_CLIENT_RDY		1
#define CMD_CLIENT_RDY_RESP	2
#define CMD_GET_ARENA		3
#define CMD_GET_ARENA_RESP	4

struct message {
	int size;
	int command;
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
