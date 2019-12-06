#include "babbler.h"
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


int recv_data_rdy(struct babbler *b, struct babbler_set *bs)
{
	if (b && bs)
		return FD_ISSET(b->fd, &bs->readset);
	return -EINVAL;
}

void b_close(struct babbler *b)
{
	if(b && b->fd != -1) {
		close(b->fd);
		b->fd = -1;
	}
}

void b_destroy(struct babbler **b)
{
	if (b && *b)
	{
		(*b)->close(*b);
		free(*b);
		*b = NULL;
	}
}

int handle_recv_data_rdy(struct babbler *b, struct babbler_set *bs,
			 int (*rx_handler) (struct babbler *b, void *opaque),
			 void *opaque)
{
	int r;
	r = recv_data_rdy(b, bs);
	if (r && r != -EINVAL && (r = rx_handler(b ,opaque)))
		printf("Failed to handle incoming data\n");
	return r;
}


