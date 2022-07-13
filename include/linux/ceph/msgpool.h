<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _FS_CEPH_MSGPOOL
#define _FS_CEPH_MSGPOOL

#include <linux/mempool.h>
<<<<<<< HEAD
#include "messenger.h"
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * we use memory pools for preallocating messages we may receive, to
 * avoid unexpected OOM conditions.
 */
struct ceph_msgpool {
	const char *name;
	mempool_t *pool;
	int type;               /* preallocated message type */
	int front_len;          /* preallocated payload size */
<<<<<<< HEAD
};

extern int ceph_msgpool_init(struct ceph_msgpool *pool, int type,
			     int front_len, int size, bool blocking,
			     const char *name);
extern void ceph_msgpool_destroy(struct ceph_msgpool *pool);
extern struct ceph_msg *ceph_msgpool_get(struct ceph_msgpool *,
					 int front_len);
=======
	int max_data_items;
};

int ceph_msgpool_init(struct ceph_msgpool *pool, int type,
		      int front_len, int max_data_items, int size,
		      const char *name);
extern void ceph_msgpool_destroy(struct ceph_msgpool *pool);
struct ceph_msg *ceph_msgpool_get(struct ceph_msgpool *pool, int front_len,
				  int max_data_items);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void ceph_msgpool_put(struct ceph_msgpool *, struct ceph_msg *);

#endif
