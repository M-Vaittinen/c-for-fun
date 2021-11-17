/**
 *	Contains socket API wrapper which obfuscates the nice socket API
 *	to some c++ like objects =]
 *
 *	Yeah, I dare you to test it!
 *
 *	Well, the original idea was to unify polling of sockets and
 *	(sysfs) files so that they could be handled similarly while
 *	also ironing out some details like binding or filling recipient
 *	details to every send call. Currently the UDP portion is somehow
 *	working to the point where handle_data_resp() can be used to
 *	call a handler function which receives the UDP data packet
 *	and size as argument - and a fixed size resp buffer where
 *	handler can fill the response payload. Nice, right? =)
 *
 *	The file (sysfs) portion is largely unimplemented.
 *
 *	Basically you want to fill initialization information for
 *	your UDP (or file) babbler by filling the struct babbler_init_udp
 *	or struct babbler_init_file.
 *
 *	Then you allocate babbler object by calling create_babbler() and
 *	by passing the init data to this call.
 *
 *	After this you call babbler->open() and now your babbler is ready
 *	for RX / TX  and for polling (with help of bullshit(TM) a.k.a bs a.k.a
 *	babbler_set object).
 *
 *	Please see the tests/babbler-test.c for simplest and dullest usage
 *	example.
 */

#ifndef _BABBLER_H
#define _BABBLER_H

#include <sys/select.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Type of babbler
 */
enum babbler_type {
	babbler_unknown = 0,
	babbler_file,
	babbler_socket,
};

struct babbler_set;

/**
 *	Object used for polling the babbler (for incoming data)
 *
 */
struct babbler_set {
	fd_set readset;					///>Internal to BS
	fd_set writeset;				///>Internal to BS
	fd_set exset;					///>Internal to BS
	int exset_users;				///>Internal to BS
	int readset_users;				///>Internal to BS
	int writeset_users;				///>Internal to BS
	int fdmax;					///>Internal to BS
	void (*clear_all)(struct babbler_set *bs);	///<Clear assigned babblers from the set.
	int (*block)(struct babbler_set *bs, struct timeval *tmo);	///< block untill one of the assigned babblers has data available.
};

/* Initialize BS object for use.
   Note, caller should've allocated struct babbler_set
*/
void init_bs(struct babbler_set *bs);

struct babbler;

typedef int (*rx_handler_t) (struct babbler *b, void *opaque);
typedef int (*rx_handler_resp_t) (struct babbler *b, void *rxdata, size_t rxsiz, void *txdata, size_t *txsiz, void *opaque);

/**
 * The babbler object. Used for babbling with others over UDP socket (or files)
 *
 * Fill babbler_init_udp or babbler_init_file and pass them to create_babbler()
 * in order to allocate your own babbler with all nice methods.
 */
struct babbler {
	enum babbler_type bab;							///< Internal
	int fd;									///< Internal
	int openmode;								///< Internal
	int (*open) (struct babbler *b);					///< Open connection to beging with babbling
	void (*close) (struct babbler *b);					///< End of chatter
	void (*destroy) (struct babbler **b);					///< destroy babbler object and free resources
	int (*receive) (struct babbler *b, char *buf, int *size);		///< Oh, babbler set told there is data? Get what the other end had to say
	int (*send) (struct babbler *b, const char *buf, int *size);		///< Speak, friend, and enter.
	void (*add_to_poll)(struct babbler *b, struct babbler_set *bs);		///< Add babbler to set for cheking whether theres something to receive.
	int (*check_data_rdy) (struct babbler *b, struct babbler_set *bs);	///< After abbler_set has indicated there is data available in one of the added babblers
										///  You can use this to check if this babbler has data available
	int (*handle_data_rdy) (struct babbler *b, struct babbler_set *bs,
				rx_handler_t rx_handler, void *opaque);		///< Similar to check_data_rdy - but allows you to give a callback for which the data that was received is passed.
	int (*handle_data_resp) (struct babbler *b, struct babbler_set *bs,
				 rx_handler_resp_t rx_handler, void *opaque);
};

struct babbler_init {
	enum babbler_type bab;
	const char *label; /* must be file name for file babbler */
	int mode_flag; /* O_RDWR, O_RDONLY, O_WRONLY -
			  meaning bi-directional,
			  RX / read only and
			  TX / write only */
};

/*
 *	Used to initialize file babbler. (Mostly for polling incoming sysfs messages)
 */
struct babbler_init_file {
	struct babbler_init b;	///< generic babbler info
	bool is_sysfs;		///< is the underlying file a sysfs file?
};

struct babbler_init_socket {
	struct babbler_init b;	///< generic babbler info
	unsigned protocol; 	/* IPPROTO_UDP */
	char myip[16]; 		/// Only required for sockets with recv capabilities
	char serverip[16]; 	/// Only required for sockets with send.
};

struct babbler_init_udp {
	struct babbler_init_socket bs;	///< generic ip layer socket details
	const unsigned short rxport;	///< port we are listening for incoming UDP messages
	const unsigned short txport;	///< port in which we are sending (when not replying)
};

/**
 * Create babbler object
 *
 * @babbler_init pointer to struct babbler member contained in babbler type specific init data.
 * @init_size size of babbler type specific init data.
 *
 * Use:
 * 	struct babbler_init_udp u;
 *	create_babbler(&u.bs.b, sizeof(u));
 *	for UDP
 *
 * 	struct babbler_init_file f;
 *	create_babbler(&f.b, sizeof(f));
 *	for file
 */
struct babbler * create_babbler(struct babbler_init *binit, size_t init_size);

#endif
