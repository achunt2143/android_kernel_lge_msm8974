<<<<<<< HEAD
#ifndef __LOCKD_NETNS_H__
#define __LOCKD_NETNS_H__

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LOCKD_NETNS_H__
#define __LOCKD_NETNS_H__

#include <linux/fs.h>
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netns/generic.h>

struct lockd_net {
	unsigned int nlmsvc_users;
<<<<<<< HEAD
};

extern int lockd_net_id;
=======
	unsigned long next_gc;
	unsigned long nrhosts;

	struct delayed_work grace_period_end;
	struct lock_manager lockd_manager;

	struct list_head nsm_handles;
};

extern unsigned int lockd_net_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
