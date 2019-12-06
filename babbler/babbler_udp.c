#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "babbler_ip.h"
#include "babbler_internal.h"
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct xx_data {
	void *data;
	size_t data_size;
	struct sockaddr_in sender;
};

static int sock_raw_recv(struct babbler *b, struct sockaddr_in *from, char *buf, size_t *size)
{
	int ret = -EINVAL;
	socklen_t fromlen = sizeof(*from);

	if (b && from && buf && size && *size)
	{
		ret = recvfrom(b->fd, buf, *size, 0, (struct sockaddr*)from, &fromlen);
		if (fromlen != sizeof(*from))
			printf("That's Very Strange. (%u != %u)\n", (unsigned)sizeof(*from), (unsigned)fromlen);
		if (ret ==-1)
		{
			ret = -errno;
			printf("recvfrom failed '%s'\n", strerror(errno));
		}
		else
		{
			printf("Received %u bytes\n", ret);
			*size = (size_t)ret;
			ret = 0;
		}
	}
	return ret;
}

static int rx_handler_for_rx_handle_resp(struct babbler *b, void *opaque)
{
	struct socket_babbler_udp *ub = (struct socket_babbler_udp *)b;
	struct xx_data *xd = (struct xx_data *)opaque;	

	return ub->raw_recv(b, &xd->sender, xd->data, &xd->data_size);
}

static int sock_handle_data_rdy(struct babbler *b, struct babbler_set *bs,
			 rx_handler_t rx_handler, void *opaque)
{
	int ret;

	if (rx_handler)
			ret = handle_recv_data_rdy(b, bs, rx_handler, opaque);
	return ret;
}

static int sock_raw_send(struct babbler *b, struct sockaddr_in *to, const char *buf, size_t *size)
{
	int ret = -EINVAL;

	if (b && to && buf && size)
		if (*size)
		{
			char ip[255];

			inet_ntop(AF_INET, &to->sin_addr, ip, sizeof(ip));
			ip[254] = '\0';
			printf("Sending %lu bytes to '%s':%hu\n",*size, ip , ntohs(to->sin_port));
			ret = sendto(b->fd, buf, *size, 0, (const struct sockaddr*)to, sizeof(*to));
			/* We return 0 if all the bytes were sent */
			if (ret == *size)
				ret = 0;
			else if (ret < 0)
				*size = 0;
			else
				*size = ret;
		}

	return ret;
}

static int sock_handle_data_resp(struct babbler *b, struct babbler_set *bs,
				 rx_handler_resp_t rx_handler, void *opaque)
{
	char rxbuf[1024];
	char txbuf[1024];
	size_t txsiz = 1024;
	int ret = -EINVAL;
	struct socket_babbler_udp *ub = (struct socket_babbler_udp *)b;
	struct xx_data d =
	{
		.data = rxbuf,
		.data_size = 1024,
	};
	

	if (rx_handler)
	{
		ret = sock_handle_data_rdy(b, bs, rx_handler_for_rx_handle_resp, &d);
		if (!ret && d.data_size > 0)
		{
			ret = rx_handler(b, rxbuf, d.data_size, txbuf, &txsiz, opaque);
			if (!ret && txsiz)
				ret = ub->raw_send(b, &d.sender, txbuf, &txsiz);
			else if (ret)
				printf("rx_handler reported error %d\n", ret);
		}
		else
			printf("Failed to get rx data. (ret %d, read %u bytes)\n", ret, (unsigned int)d.data_size);
	}
	return ret;
}

int b_socket_open(struct babbler *b)
{
	int ret = -EINVAL;

	if(b && b->bab == babbler_socket)
	{
		b->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (-1 != b->fd)
		{
			struct socket_babbler_udp *bu = (struct socket_babbler_udp *)b;

			if ( (b->openmode & O_ACCMODE) != O_WRONLY)
			{
				struct sockaddr_in a;

				a.sin_family = AF_INET;
				a.sin_addr.s_addr = bu->myip;
				a.sin_port = htons(bu->rxport);
				if ((ret = bind(b->fd, (const struct sockaddr *)&a, sizeof(a))))
				{
					ret = -errno;
					printf("bind failed (%s)\n", strerror(-ret));
					close(b->fd);
				}
			}
			else
				ret = 0;
		}
		else
		{
			ret = -errno;
			printf("socket create failed (%s)\n", strerror(-ret));
		}
	}
	return ret;
}

int b_sock_send(struct babbler *b, const char *buf, int *size)
{
	int ret = -EINVAL;
	if (b && size && buf && b->bab == babbler_socket)
	{
		struct sockaddr_in a;
		struct socket_babbler_udp *bu = (struct socket_babbler_udp *)b;

		a.sin_family = AF_INET;
		a.sin_addr.s_addr = bu->serverip;
		a.sin_port = htons(bu->txport);
		ret = sendto(b->fd, buf, *size, 0, 
                      (const struct sockaddr*) &a, sizeof(a));
		if (ret == *size)
			ret = 0;
		else if (ret < 0)
			*size = 0;
		else
			*size = ret;
	}
	return ret;
}

int b_sock_recv(struct babbler *b, char *buf, int *size)
{
	int ret = -EINVAL;

	if (b && size && buf && b->bab == babbler_socket)
	{
		ret = recv(b->fd, buf, *size, 0);
		if(ret >= 0)
		{
			*size = ret;
			ret = 0;
		}
		else
			printf("Recv failed w %d\n", ret);
	}
	return ret;
}

static void sock_add_to_poll(struct babbler *b, struct babbler_set *bs)
{
	if (b && bs && b->bab == babbler_socket && b->fd != -1)
	{
		FD_SET(b->fd, &bs->readset);
		bs->readset_users++;
		if (b->fd > bs->fdmax)
			bs->fdmax = b->fd;

	}
	else
		printf("Failed to add babbler for polling - not opened?\n");
}

struct babbler * create_socket_babbler(struct babbler_init* bi, size_t init_size)
{
	struct babbler *b = NULL;
	struct babbler_init_socket *bs =(struct babbler_init_socket *)bi;

	if (init_size < sizeof(struct babbler_init_socket))
		return NULL;

	switch (bs->protocol) {
		case IPPROTO_UDP:
		{
			struct socket_babbler_udp *bu = NULL;
			struct babbler_init_udp *ibu;

			if (init_size >= sizeof(*ibu) && (bu = calloc(1, sizeof(*bu))))
			{
				ibu = (struct babbler_init_udp *)bs;
				b = &bu->b;
				b->bab = babbler_socket;
				bu->rxport = ibu->rxport;
				bu->txport = ibu->txport;
				bu->raw_send = &sock_raw_send;
				bu->raw_recv = &sock_raw_recv;
				b->fd = -1;
				b = &bu->b;
				b->open = b_socket_open;
				b->close = b_close;
				b->add_to_poll = &sock_add_to_poll;
				b->destroy = b_destroy;
				b->openmode = bi->mode_flag;
				b->handle_data_resp = &sock_handle_data_resp;
				if ( (bi->mode_flag & O_ACCMODE) != O_RDONLY )
				{
					/* This is actually a bit clumsy. We should allow sending reponses to messages without knowing the server IP here. We can get the server from received headers. */
					if (1 != inet_pton(AF_INET, bs->serverip, &bu->serverip)) {
						printf("Oh dear. I can't code any more\n");
						return NULL;
					}
					b->send = b_sock_send;
				}
				if ( (bi->mode_flag & O_ACCMODE) != O_WRONLY )
				{
					if (1 != inet_pton(AF_INET, bs->myip, &bu->myip)) {
						printf("Oh dear. I can't code any more\n");
						return NULL;
					}
					b->receive = b_sock_recv;
				}
				b->check_data_rdy = recv_data_rdy;
				b->handle_data_rdy = &sock_handle_data_rdy;
			}
		}
		break;
		default:
			printf("Unknown babbler protocol\n");
	}
	return b;
}


