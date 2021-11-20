#include "msg.h"
#include "client_arena.h"
/*

int get_arena(struct server *s, struct client *c, struct areena *a)
{
	int ret;
	struct msg_get_arena arena_req;
	struct msg_get_arena_resp arena_rsp;

	arena_req.hdr.size = sizeof(arena_req);
	arena_req.hdr.command = CMD_GET_ARENA;

	ret = send(c->sock, &arena_req, sizeof(arena_req), 0);
	if (ret != sizeof(arena_req)) {
		printf("send returned %d\n", ret);
		perror("send\n");
		return -1;
	}

	ret = recv(c->sock, &arena_rsp, sizeof(arena_rsp), 0);
	if (ret != sizeof(arena_rsp)) {
		printf("recv returned %d\n", ret);
		perror("recv\n");
		return -1;
	}
	if (arena_rsp.hdr.command != CMD_GET_ARENA_RESP) {
		printf("unexpected resp %d\n", arena_rsp.hdr.command);
		return -1;
	}
	return 0;
};

*/
