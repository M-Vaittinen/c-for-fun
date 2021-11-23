#ifndef _SERVER_AREENA_H
#define _SERVER_AREENA_H

#include "client.h"

void arvo_server_areena();
int starttaa_server_areena_updater(struct client *cli_tbl, int num_cli);
int server_send_arena(struct client *cli_tbl, int num_cli);

#endif
