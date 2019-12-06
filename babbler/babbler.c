#include "babbler.h"
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "babbler_internal.h"

static void bs_clear_all(struct babbler_set *bs)
{
	if(bs)
	{
		FD_ZERO(&bs->exset);
		FD_ZERO(&bs->readset);
		FD_ZERO(&bs->writeset);
		bs->exset_users = bs->readset_users = bs->writeset_users = bs->fdmax = 0;
	}
}

static int bs_block(struct babbler_set *bs, struct timeval *tmo)
{
	fd_set * sets[3] = { NULL, NULL, NULL };
	int ret = -EINVAL;

	if(bs)
	{
		if(bs->readset_users)
			sets[0] = &bs->readset;
		if(bs->writeset_users)
			sets[1] = &bs->writeset;
		if(bs->exset_users)
			sets[2] = &bs->exset;
again:
		ret = select(bs->fdmax+1, sets[0], sets[1], sets[2], tmo);
		if (ret == EINTR)
			goto again;
	}
	return ret;
}

void init_bs(struct babbler_set *bs)
{
	bs->clear_all = &bs_clear_all;
	bs->block = &bs_block;
	bs->clear_all(bs);
}

struct babbler * create_babbler(struct babbler_init *binit, size_t init_size)
{
	struct babbler *b = NULL;
	if (binit)
	{
		switch (binit->bab)
		{
		case babbler_file:
			b = create_file_babbler(binit, init_size);
			break;
		case babbler_socket:
			b = create_socket_babbler(binit, init_size);
			break;
		default:
			printf("Unknown babbler type\n");
		}
	}
	return b;
}




