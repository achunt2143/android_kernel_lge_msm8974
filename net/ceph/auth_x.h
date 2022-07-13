<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _FS_CEPH_AUTH_X_H
#define _FS_CEPH_AUTH_X_H

#include <linux/rbtree.h>

#include <linux/ceph/auth.h>

#include "crypto.h"
#include "auth_x_protocol.h"

/*
 * Handle ticket for a single service.
 */
struct ceph_x_ticket_handler {
	struct rb_node node;
<<<<<<< HEAD
	unsigned service;

	struct ceph_crypto_key session_key;
	struct ceph_timespec validity;
=======
	unsigned int service;

	struct ceph_crypto_key session_key;
	bool have_key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	u64 secret_id;
	struct ceph_buffer *ticket_blob;

<<<<<<< HEAD
	unsigned long renew_after, expires;
};


struct ceph_x_authorizer {
	struct ceph_buffer *buf;
	unsigned service;
	u64 nonce;
	u64 secret_id;
	char reply_buf[128];  /* big enough for encrypted blob */
=======
	time64_t renew_after, expires;
};

#define CEPHX_AU_ENC_BUF_LEN	128  /* big enough for encrypted blob */

struct ceph_x_authorizer {
	struct ceph_authorizer base;
	struct ceph_crypto_key session_key;
	struct ceph_buffer *buf;
	unsigned int service;
	u64 nonce;
	u64 secret_id;
	char enc_buf[CEPHX_AU_ENC_BUF_LEN] __aligned(8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ceph_x_info {
	struct ceph_crypto_key secret;

	bool starting;
	u64 server_challenge;

<<<<<<< HEAD
	unsigned have_keys;
=======
	unsigned int have_keys;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rb_root ticket_handlers;

	struct ceph_x_authorizer auth_authorizer;
};

<<<<<<< HEAD
extern int ceph_x_init(struct ceph_auth_client *ac);

#endif

=======
int ceph_x_init(struct ceph_auth_client *ac);

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
