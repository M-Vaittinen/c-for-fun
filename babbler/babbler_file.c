#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "babbler.h"
#include "babbler_file.h"
#include "babbler_internal.h"

static int file_check_data_rdy(struct babbler *b, struct babbler_set *bs)
{
	struct file_babbler *fb = (struct file_babbler *)b;

	if (!b || !bs || b->bab != babbler_file)
		return -EINVAL;

	if (fb->is_sysfs)
		return FD_ISSET(b->fd, &bs->exset);
	return recv_data_rdy(b, bs);
}

static int file_handle_data_rdy(struct babbler *b, struct babbler_set *bs,
			 int (*rx_handler) (struct babbler *b, void *opaque),
			 void *opaque)
{
	int r = -EINVAL;

	if (b && bs && rx_handler && b->bab == babbler_file)
	{
		struct file_babbler *fb = (struct file_babbler *)b;
		if (fb->is_sysfs)
		{
			r = 0;
			if (FD_ISSET(b->fd, &bs->exset) && (r = rx_handler(b ,opaque)))
				printf("Failed to handle incoming data\n");
			else
				r = 0;
		}
		else
			r = handle_recv_data_rdy(b, bs, rx_handler, opaque);
	}
	return r;
}

static int b_file_open(struct babbler *b)
{
	struct file_babbler *bf;
	if(b && b->bab == babbler_file)
	{
		bf = (struct file_babbler *)b;

		b->fd = open(bf->name, b->openmode | O_CLOEXEC);
		if (b->fd != -1)
			return 0;
		else
			perror("open failed\n");
	}
	return -1;
}

static void file_add_to_poll(struct babbler *b, struct babbler_set *bs)
{
	if (b && bs && b->bab == babbler_file)
	{
		struct file_babbler *fb = (struct file_babbler *)b;
		if (fb->is_sysfs)
		{
			/* Sysfs files are polled using exset */
			FD_SET(b->fd, &bs->exset);
			bs->exset_users++;
		}
		else
		{
			FD_SET(b->fd, &bs->readset);
			bs->readset_users++;
			if (b->fd > bs->fdmax)
				bs->fdmax = b->fd;
		}
	}
}
int b_file_receive(struct babbler *b, char *buf, int *size)
{
	printf("%s(): STill undone %s:%d\n", __func__, __FILE__, __LINE__);
	return -1;
}

static int b_file_send(struct babbler *b, const char *buf, int *size)
{
	printf("%s(): STill undone %s:%d\n", __func__, __FILE__, __LINE__);
	return -1;
}
struct babbler * create_file_babbler(struct babbler_init *bi, size_t init_size)
{
	struct babbler *b = NULL;
	struct file_babbler *fb;
	struct babbler_init_file *bf = (struct babbler_init_file *)bi;
	
	if (init_size < sizeof(struct babbler_init_file))
		return NULL;

	fb = calloc(1, sizeof(struct file_babbler));
	if (fb)
	{
		b = &fb->b;
		if (NAME_MAX < snprintf(fb->name, NAME_MAX, "%s", bf->b.label)) {
			printf("too long filename %s\n", bf->b.label);
			return NULL;
		}
		b->bab = babbler_file;
		b->fd = -1;
		b->open = b_file_open;
		b->close = b_close;
		b->destroy = b_destroy;
		b->openmode = bi->mode_flag;
		b->add_to_poll = &file_add_to_poll;
		if ( bi->mode_flag != O_RDONLY )
			b->send = &b_file_send;
		if ( bi->mode_flag != O_WRONLY)
			b->receive = &b_file_receive;
		fb->is_sysfs = bf->is_sysfs;
		b->handle_data_rdy = &file_handle_data_rdy;
		b->check_data_rdy = &file_check_data_rdy; 
	}

	return b;
}


