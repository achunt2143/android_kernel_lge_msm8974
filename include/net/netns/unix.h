<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Unix network namespace
 */
#ifndef __NETNS_UNIX_H__
#define __NETNS_UNIX_H__

<<<<<<< HEAD
struct ctl_table_header;
struct netns_unix {
=======
#include <linux/spinlock.h>

struct unix_table {
	spinlock_t		*locks;
	struct hlist_head	*buckets;
};

struct ctl_table_header;
struct netns_unix {
	struct unix_table	table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			sysctl_max_dgram_qlen;
	struct ctl_table_header	*ctl;
};

#endif /* __NETNS_UNIX_H__ */
