#ifndef _BABBLER_INTERNAL_H
#define _BABBLER_INTERNAL_H

#include "babbler.h"
#include "babbler_file.h"
#include <stddef.h>

int recv_data_rdy(struct babbler *b, struct babbler_set *bs);
int handle_recv_data_rdy(struct babbler *b, struct babbler_set *bs,
			 int (*rx_handler) (struct babbler *b, void *opaque), void *opaque);
void b_close(struct babbler *b);
void b_destroy(struct babbler **b);
/* Allocates and initializes a file babbler. Created babbler must be freed using
babbler->destroy() when babbler is no longer needed. */
struct babbler * create_file_babbler(struct babbler_init *bi, size_t init_size);
struct babbler * create_socket_babbler(struct babbler_init *bi, size_t init_size);

#endif
