<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NET_EVENT_H
#define _NET_EVENT_H

/*
 *	Generic netevent notifiers
 *
 *	Authors:
 *      Tom Tucker              <tom@opengridcomputing.com>
 *      Steve Wise              <swise@opengridcomputing.com>
 *
 * 	Changes:
 */

struct dst_entry;
<<<<<<< HEAD
=======
struct neighbour;
struct notifier_block ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct netevent_redirect {
	struct dst_entry *old;
	struct dst_entry *new;
<<<<<<< HEAD
=======
	struct neighbour *neigh;
	const void *daddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum netevent_notif_type {
	NETEVENT_NEIGH_UPDATE = 1, /* arg is struct neighbour ptr */
	NETEVENT_REDIRECT,	   /* arg is struct netevent_redirect ptr */
<<<<<<< HEAD
};

extern int register_netevent_notifier(struct notifier_block *nb);
extern int unregister_netevent_notifier(struct notifier_block *nb);
extern int call_netevent_notifiers(unsigned long val, void *v);
=======
	NETEVENT_DELAY_PROBE_TIME_UPDATE, /* arg is struct neigh_parms ptr */
	NETEVENT_IPV4_MPATH_HASH_UPDATE, /* arg is struct net ptr */
	NETEVENT_IPV6_MPATH_HASH_UPDATE, /* arg is struct net ptr */
	NETEVENT_IPV4_FWD_UPDATE_PRIORITY_UPDATE, /* arg is struct net ptr */
};

int register_netevent_notifier(struct notifier_block *nb);
int unregister_netevent_notifier(struct notifier_block *nb);
int call_netevent_notifiers(unsigned long val, void *v);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
