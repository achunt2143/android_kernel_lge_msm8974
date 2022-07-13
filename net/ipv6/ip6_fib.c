<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Linux INET6 implementation
 *	Forwarding Information Database
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>
 *
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

/*
 * 	Changes:
 * 	Yuji SEKIYA @USAGI:	Support default route on router node;
 * 				remove ip6_null_entry from the top of
 * 				routing table.
 * 	Ville Nuorvala:		Fixed routing subtrees.
 */
=======
 *	Changes:
 *	Yuji SEKIYA @USAGI:	Support default route on router node;
 *				remove ip6_null_entry from the top of
 *				routing table.
 *	Ville Nuorvala:		Fixed routing subtrees.
 */

#define pr_fmt(fmt) "IPv6: " fmt

#include <linux/bpf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/net.h>
#include <linux/route.h>
#include <linux/netdevice.h>
#include <linux/in6.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <net/ipv6.h>
#include <net/ndisc.h>
#include <net/addrconf.h>

#include <net/ip6_fib.h>
#include <net/ip6_route.h>

#define RT6_DEBUG 2

#if RT6_DEBUG >= 3
#define RT6_TRACE(x...) printk(KERN_DEBUG x)
#else
#define RT6_TRACE(x...) do { ; } while (0)
#endif

static struct kmem_cache * fib6_node_kmem __read_mostly;

enum fib_walk_state_t
{
#ifdef CONFIG_IPV6_SUBTREES
	FWS_S,
#endif
	FWS_L,
	FWS_R,
	FWS_C,
	FWS_U
};

struct fib6_cleaner_t
{
	struct fib6_walker_t w;
	struct net *net;
	int (*func)(struct rt6_info *, void *arg);
	void *arg;
};

static DEFINE_RWLOCK(fib6_walker_lock);

=======
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/ndisc.h>
#include <net/addrconf.h>
#include <net/lwtunnel.h>
#include <net/fib_notifier.h>

#include <net/ip_fib.h>
#include <net/ip6_fib.h>
#include <net/ip6_route.h>

static struct kmem_cache *fib6_node_kmem __read_mostly;

struct fib6_cleaner {
	struct fib6_walker w;
	struct net *net;
	int (*func)(struct fib6_info *, void *arg);
	int sernum;
	void *arg;
	bool skip_notify;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IPV6_SUBTREES
#define FWS_INIT FWS_S
#else
#define FWS_INIT FWS_L
#endif

<<<<<<< HEAD
static void fib6_prune_clones(struct net *net, struct fib6_node *fn,
			      struct rt6_info *rt);
static struct rt6_info *fib6_find_prefix(struct net *net, struct fib6_node *fn);
static struct fib6_node *fib6_repair_tree(struct net *net, struct fib6_node *fn);
static int fib6_walk(struct fib6_walker_t *w);
static int fib6_walk_continue(struct fib6_walker_t *w);
=======
static struct fib6_info *fib6_find_prefix(struct net *net,
					 struct fib6_table *table,
					 struct fib6_node *fn);
static struct fib6_node *fib6_repair_tree(struct net *net,
					  struct fib6_table *table,
					  struct fib6_node *fn);
static int fib6_walk(struct net *net, struct fib6_walker *w);
static int fib6_walk_continue(struct fib6_walker *w);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	A routing update causes an increase of the serial number on the
 *	affected subtree. This allows for cached routes to be asynchronously
 *	tested when modifications are made to the destination cache as a
 *	result of redirects, path MTU changes, etc.
 */

<<<<<<< HEAD
static __u32 rt_sernum;

static void fib6_gc_timer_cb(unsigned long arg);

static LIST_HEAD(fib6_walkers);
#define FOR_WALKERS(w) list_for_each_entry(w, &fib6_walkers, lh)

static inline void fib6_walker_link(struct fib6_walker_t *w)
{
	write_lock_bh(&fib6_walker_lock);
	list_add(&w->lh, &fib6_walkers);
	write_unlock_bh(&fib6_walker_lock);
}

static inline void fib6_walker_unlink(struct fib6_walker_t *w)
{
	write_lock_bh(&fib6_walker_lock);
	list_del(&w->lh);
	write_unlock_bh(&fib6_walker_lock);
}
static __inline__ u32 fib6_new_sernum(void)
{
	u32 n = ++rt_sernum;
	if ((__s32)n <= 0)
		rt_sernum = n = 1;
	return n;
=======
static void fib6_gc_timer_cb(struct timer_list *t);

#define FOR_WALKERS(net, w) \
	list_for_each_entry(w, &(net)->ipv6.fib6_walkers, lh)

static void fib6_walker_link(struct net *net, struct fib6_walker *w)
{
	write_lock_bh(&net->ipv6.fib6_walker_lock);
	list_add(&w->lh, &net->ipv6.fib6_walkers);
	write_unlock_bh(&net->ipv6.fib6_walker_lock);
}

static void fib6_walker_unlink(struct net *net, struct fib6_walker *w)
{
	write_lock_bh(&net->ipv6.fib6_walker_lock);
	list_del(&w->lh);
	write_unlock_bh(&net->ipv6.fib6_walker_lock);
}

static int fib6_new_sernum(struct net *net)
{
	int new, old = atomic_read(&net->ipv6.fib6_sernum);

	do {
		new = old < INT_MAX ? old + 1 : 1;
	} while (!atomic_try_cmpxchg(&net->ipv6.fib6_sernum, &old, new));

	return new;
}

enum {
	FIB6_NO_SERNUM_CHANGE = 0,
};

void fib6_update_sernum(struct net *net, struct fib6_info *f6i)
{
	struct fib6_node *fn;

	fn = rcu_dereference_protected(f6i->fib6_node,
			lockdep_is_held(&f6i->fib6_table->tb6_lock));
	if (fn)
		WRITE_ONCE(fn->fn_sernum, fib6_new_sernum(net));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Auxiliary address test functions for the radix tree.
 *
 *	These assume a 32bit processor (although it will work on
 *	64bit processors)
 */

/*
 *	test bit
 */
#if defined(__LITTLE_ENDIAN)
# define BITOP_BE32_SWIZZLE	(0x1F & ~7)
#else
# define BITOP_BE32_SWIZZLE	0
#endif

<<<<<<< HEAD
static __inline__ __be32 addr_bit_set(const void *token, int fn_bit)
=======
static __be32 addr_bit_set(const void *token, int fn_bit)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const __be32 *addr = token;
	/*
	 * Here,
<<<<<<< HEAD
	 * 	1 << ((~fn_bit ^ BITOP_BE32_SWIZZLE) & 0x1f)
=======
	 *	1 << ((~fn_bit ^ BITOP_BE32_SWIZZLE) & 0x1f)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * is optimized version of
	 *	htonl(1 << ((~fn_bit)&0x1F))
	 * See include/asm-generic/bitops/le.h.
	 */
	return (__force __be32)(1 << ((~fn_bit ^ BITOP_BE32_SWIZZLE) & 0x1f)) &
	       addr[fn_bit >> 5];
}

<<<<<<< HEAD
static __inline__ struct fib6_node * node_alloc(void)
=======
struct fib6_info *fib6_info_alloc(gfp_t gfp_flags, bool with_fib6_nh)
{
	struct fib6_info *f6i;
	size_t sz = sizeof(*f6i);

	if (with_fib6_nh)
		sz += sizeof(struct fib6_nh);

	f6i = kzalloc(sz, gfp_flags);
	if (!f6i)
		return NULL;

	/* fib6_siblings is a union with nh_list, so this initializes both */
	INIT_LIST_HEAD(&f6i->fib6_siblings);
	refcount_set(&f6i->fib6_ref, 1);

	INIT_HLIST_NODE(&f6i->gc_link);

	return f6i;
}

void fib6_info_destroy_rcu(struct rcu_head *head)
{
	struct fib6_info *f6i = container_of(head, struct fib6_info, rcu);

	WARN_ON(f6i->fib6_node);

	if (f6i->nh)
		nexthop_put(f6i->nh);
	else
		fib6_nh_release(f6i->fib6_nh);

	ip_fib_metrics_put(f6i->fib6_metrics);
	kfree(f6i);
}
EXPORT_SYMBOL_GPL(fib6_info_destroy_rcu);

static struct fib6_node *node_alloc(struct net *net)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib6_node *fn;

	fn = kmem_cache_zalloc(fib6_node_kmem, GFP_ATOMIC);
<<<<<<< HEAD
=======
	if (fn)
		net->ipv6.rt6_stats->fib_nodes++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return fn;
}

<<<<<<< HEAD
static __inline__ void node_free(struct fib6_node * fn)
{
	kmem_cache_free(fib6_node_kmem, fn);
}

static __inline__ void rt6_release(struct rt6_info *rt)
{
	if (atomic_dec_and_test(&rt->rt6i_ref))
		dst_free(&rt->dst);
=======
static void node_free_immediate(struct net *net, struct fib6_node *fn)
{
	kmem_cache_free(fib6_node_kmem, fn);
	net->ipv6.rt6_stats->fib_nodes--;
}

static void node_free_rcu(struct rcu_head *head)
{
	struct fib6_node *fn = container_of(head, struct fib6_node, rcu);

	kmem_cache_free(fib6_node_kmem, fn);
}

static void node_free(struct net *net, struct fib6_node *fn)
{
	call_rcu(&fn->rcu, node_free_rcu);
	net->ipv6.rt6_stats->fib_nodes--;
}

static void fib6_free_table(struct fib6_table *table)
{
	inetpeer_invalidate_tree(&table->tb6_peers);
	kfree(table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void fib6_link_table(struct net *net, struct fib6_table *tb)
{
	unsigned int h;

	/*
	 * Initialize table lock at a single place to give lockdep a key,
	 * tables aren't visible prior to being linked to the list.
	 */
<<<<<<< HEAD
	rwlock_init(&tb->tb6_lock);

=======
	spin_lock_init(&tb->tb6_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	h = tb->tb6_id & (FIB6_TABLE_HASHSZ - 1);

	/*
	 * No protection necessary, this is the only list mutatation
	 * operation, tables never disappear once they exist.
	 */
	hlist_add_head_rcu(&tb->tb6_hlist, &net->ipv6.fib_table_hash[h]);
}

#ifdef CONFIG_IPV6_MULTIPLE_TABLES

static struct fib6_table *fib6_alloc_table(struct net *net, u32 id)
{
	struct fib6_table *table;

	table = kzalloc(sizeof(*table), GFP_ATOMIC);
	if (table) {
		table->tb6_id = id;
<<<<<<< HEAD
		table->tb6_root.leaf = net->ipv6.ip6_null_entry;
		table->tb6_root.fn_flags = RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
=======
		rcu_assign_pointer(table->tb6_root.leaf,
				   net->ipv6.fib6_null_entry);
		table->tb6_root.fn_flags = RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
		inet_peer_base_init(&table->tb6_peers);
		INIT_HLIST_HEAD(&table->tb6_gc_hlist);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return table;
}

struct fib6_table *fib6_new_table(struct net *net, u32 id)
{
	struct fib6_table *tb;

	if (id == 0)
		id = RT6_TABLE_MAIN;
	tb = fib6_get_table(net, id);
	if (tb)
		return tb;

	tb = fib6_alloc_table(net, id);
	if (tb)
		fib6_link_table(net, tb);

	return tb;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(fib6_new_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct fib6_table *fib6_get_table(struct net *net, u32 id)
{
	struct fib6_table *tb;
	struct hlist_head *head;
<<<<<<< HEAD
	struct hlist_node *node;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int h;

	if (id == 0)
		id = RT6_TABLE_MAIN;
	h = id & (FIB6_TABLE_HASHSZ - 1);
	rcu_read_lock();
	head = &net->ipv6.fib_table_hash[h];
<<<<<<< HEAD
	hlist_for_each_entry_rcu(tb, node, head, tb6_hlist) {
=======
	hlist_for_each_entry_rcu(tb, head, tb6_hlist) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (tb->tb6_id == id) {
			rcu_read_unlock();
			return tb;
		}
	}
	rcu_read_unlock();

	return NULL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(fib6_get_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __net_init fib6_tables_init(struct net *net)
{
	fib6_link_table(net, net->ipv6.fib6_main_tbl);
	fib6_link_table(net, net->ipv6.fib6_local_tbl);
}
#else

struct fib6_table *fib6_new_table(struct net *net, u32 id)
{
	return fib6_get_table(net, id);
}

struct fib6_table *fib6_get_table(struct net *net, u32 id)
{
	  return net->ipv6.fib6_main_tbl;
}

struct dst_entry *fib6_rule_lookup(struct net *net, struct flowi6 *fl6,
<<<<<<< HEAD
				   int flags, pol_lookup_t lookup)
{
	return (struct dst_entry *) lookup(net, net->ipv6.fib6_main_tbl, fl6, flags);
=======
				   const struct sk_buff *skb,
				   int flags, pol_lookup_t lookup)
{
	struct rt6_info *rt;

	rt = pol_lookup_func(lookup,
			net, net->ipv6.fib6_main_tbl, fl6, skb, flags);
	if (rt->dst.error == -EAGAIN) {
		ip6_rt_put_flags(rt, flags);
		rt = net->ipv6.ip6_null_entry;
		if (!(flags & RT6_LOOKUP_F_DST_NOREF))
			dst_hold(&rt->dst);
	}

	return &rt->dst;
}

/* called with rcu lock held; no reference taken on fib6_info */
int fib6_lookup(struct net *net, int oif, struct flowi6 *fl6,
		struct fib6_result *res, int flags)
{
	return fib6_table_lookup(net, net->ipv6.fib6_main_tbl, oif, fl6,
				 res, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_init fib6_tables_init(struct net *net)
{
	fib6_link_table(net, net->ipv6.fib6_main_tbl);
}

#endif

<<<<<<< HEAD
static int fib6_dump_node(struct fib6_walker_t *w)
{
	int res;
	struct rt6_info *rt;

	for (rt = w->leaf; rt; rt = rt->dst.rt6_next) {
		res = rt6_dump_route(rt, w->args);
		if (res < 0) {
			/* Frame is full, suspend walking */
			w->leaf = rt;
			return 1;
		}
		WARN_ON(res == 0);
=======
unsigned int fib6_tables_seq_read(struct net *net)
{
	unsigned int h, fib_seq = 0;

	rcu_read_lock();
	for (h = 0; h < FIB6_TABLE_HASHSZ; h++) {
		struct hlist_head *head = &net->ipv6.fib_table_hash[h];
		struct fib6_table *tb;

		hlist_for_each_entry_rcu(tb, head, tb6_hlist)
			fib_seq += tb->fib_seq;
	}
	rcu_read_unlock();

	return fib_seq;
}

static int call_fib6_entry_notifier(struct notifier_block *nb,
				    enum fib_event_type event_type,
				    struct fib6_info *rt,
				    struct netlink_ext_ack *extack)
{
	struct fib6_entry_notifier_info info = {
		.info.extack = extack,
		.rt = rt,
	};

	return call_fib6_notifier(nb, event_type, &info.info);
}

static int call_fib6_multipath_entry_notifier(struct notifier_block *nb,
					      enum fib_event_type event_type,
					      struct fib6_info *rt,
					      unsigned int nsiblings,
					      struct netlink_ext_ack *extack)
{
	struct fib6_entry_notifier_info info = {
		.info.extack = extack,
		.rt = rt,
		.nsiblings = nsiblings,
	};

	return call_fib6_notifier(nb, event_type, &info.info);
}

int call_fib6_entry_notifiers(struct net *net,
			      enum fib_event_type event_type,
			      struct fib6_info *rt,
			      struct netlink_ext_ack *extack)
{
	struct fib6_entry_notifier_info info = {
		.info.extack = extack,
		.rt = rt,
	};

	rt->fib6_table->fib_seq++;
	return call_fib6_notifiers(net, event_type, &info.info);
}

int call_fib6_multipath_entry_notifiers(struct net *net,
					enum fib_event_type event_type,
					struct fib6_info *rt,
					unsigned int nsiblings,
					struct netlink_ext_ack *extack)
{
	struct fib6_entry_notifier_info info = {
		.info.extack = extack,
		.rt = rt,
		.nsiblings = nsiblings,
	};

	rt->fib6_table->fib_seq++;
	return call_fib6_notifiers(net, event_type, &info.info);
}

int call_fib6_entry_notifiers_replace(struct net *net, struct fib6_info *rt)
{
	struct fib6_entry_notifier_info info = {
		.rt = rt,
		.nsiblings = rt->fib6_nsiblings,
	};

	rt->fib6_table->fib_seq++;
	return call_fib6_notifiers(net, FIB_EVENT_ENTRY_REPLACE, &info.info);
}

struct fib6_dump_arg {
	struct net *net;
	struct notifier_block *nb;
	struct netlink_ext_ack *extack;
};

static int fib6_rt_dump(struct fib6_info *rt, struct fib6_dump_arg *arg)
{
	enum fib_event_type fib_event = FIB_EVENT_ENTRY_REPLACE;
	int err;

	if (!rt || rt == arg->net->ipv6.fib6_null_entry)
		return 0;

	if (rt->fib6_nsiblings)
		err = call_fib6_multipath_entry_notifier(arg->nb, fib_event,
							 rt,
							 rt->fib6_nsiblings,
							 arg->extack);
	else
		err = call_fib6_entry_notifier(arg->nb, fib_event, rt,
					       arg->extack);

	return err;
}

static int fib6_node_dump(struct fib6_walker *w)
{
	int err;

	err = fib6_rt_dump(w->leaf, w->args);
	w->leaf = NULL;
	return err;
}

static int fib6_table_dump(struct net *net, struct fib6_table *tb,
			   struct fib6_walker *w)
{
	int err;

	w->root = &tb->tb6_root;
	spin_lock_bh(&tb->tb6_lock);
	err = fib6_walk(net, w);
	spin_unlock_bh(&tb->tb6_lock);
	return err;
}

/* Called with rcu_read_lock() */
int fib6_tables_dump(struct net *net, struct notifier_block *nb,
		     struct netlink_ext_ack *extack)
{
	struct fib6_dump_arg arg;
	struct fib6_walker *w;
	unsigned int h;
	int err = 0;

	w = kzalloc(sizeof(*w), GFP_ATOMIC);
	if (!w)
		return -ENOMEM;

	w->func = fib6_node_dump;
	arg.net = net;
	arg.nb = nb;
	arg.extack = extack;
	w->args = &arg;

	for (h = 0; h < FIB6_TABLE_HASHSZ; h++) {
		struct hlist_head *head = &net->ipv6.fib_table_hash[h];
		struct fib6_table *tb;

		hlist_for_each_entry_rcu(tb, head, tb6_hlist) {
			err = fib6_table_dump(net, tb, w);
			if (err)
				goto out;
		}
	}

out:
	kfree(w);

	/* The tree traversal function should never return a positive value. */
	return err > 0 ? -EINVAL : err;
}

static int fib6_dump_node(struct fib6_walker *w)
{
	int res;
	struct fib6_info *rt;

	for_each_fib6_walker_rt(w) {
		res = rt6_dump_route(rt, w->args, w->skip_in_node);
		if (res >= 0) {
			/* Frame is full, suspend walking */
			w->leaf = rt;

			/* We'll restart from this node, so if some routes were
			 * already dumped, skip them next time.
			 */
			w->skip_in_node += res;

			return 1;
		}
		w->skip_in_node = 0;

		/* Multipath routes are dumped in one route with the
		 * RTA_MULTIPATH attribute. Jump 'rt' to point to the
		 * last sibling of this route (no need to dump the
		 * sibling routes again)
		 */
		if (rt->fib6_nsiblings)
			rt = list_last_entry(&rt->fib6_siblings,
					     struct fib6_info,
					     fib6_siblings);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	w->leaf = NULL;
	return 0;
}

static void fib6_dump_end(struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct fib6_walker_t *w = (void*)cb->args[2];
=======
	struct net *net = sock_net(cb->skb->sk);
	struct fib6_walker *w = (void *)cb->args[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (w) {
		if (cb->args[4]) {
			cb->args[4] = 0;
<<<<<<< HEAD
			fib6_walker_unlink(w);
=======
			fib6_walker_unlink(net, w);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		cb->args[2] = 0;
		kfree(w);
	}
<<<<<<< HEAD
	cb->done = (void*)cb->args[3];
=======
	cb->done = (void *)cb->args[3];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cb->args[1] = 3;
}

static int fib6_dump_done(struct netlink_callback *cb)
{
	fib6_dump_end(cb);
	return cb->done ? cb->done(cb) : 0;
}

static int fib6_dump_table(struct fib6_table *table, struct sk_buff *skb,
			   struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct fib6_walker_t *w;
=======
	struct net *net = sock_net(skb->sk);
	struct fib6_walker *w;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int res;

	w = (void *)cb->args[2];
	w->root = &table->tb6_root;

	if (cb->args[4] == 0) {
		w->count = 0;
		w->skip = 0;
<<<<<<< HEAD

		read_lock_bh(&table->tb6_lock);
		res = fib6_walk(w);
		read_unlock_bh(&table->tb6_lock);
		if (res > 0) {
			cb->args[4] = 1;
			cb->args[5] = w->root->fn_sernum;
		}
	} else {
		if (cb->args[5] != w->root->fn_sernum) {
			/* Begin at the root if the tree changed */
			cb->args[5] = w->root->fn_sernum;
			w->state = FWS_INIT;
			w->node = w->root;
			w->skip = w->count;
		} else
			w->skip = 0;

		read_lock_bh(&table->tb6_lock);
		res = fib6_walk_continue(w);
		read_unlock_bh(&table->tb6_lock);
		if (res <= 0) {
			fib6_walker_unlink(w);
=======
		w->skip_in_node = 0;

		spin_lock_bh(&table->tb6_lock);
		res = fib6_walk(net, w);
		spin_unlock_bh(&table->tb6_lock);
		if (res > 0) {
			cb->args[4] = 1;
			cb->args[5] = READ_ONCE(w->root->fn_sernum);
		}
	} else {
		int sernum = READ_ONCE(w->root->fn_sernum);
		if (cb->args[5] != sernum) {
			/* Begin at the root if the tree changed */
			cb->args[5] = sernum;
			w->state = FWS_INIT;
			w->node = w->root;
			w->skip = w->count;
			w->skip_in_node = 0;
		} else
			w->skip = 0;

		spin_lock_bh(&table->tb6_lock);
		res = fib6_walk_continue(w);
		spin_unlock_bh(&table->tb6_lock);
		if (res <= 0) {
			fib6_walker_unlink(net, w);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cb->args[4] = 0;
		}
	}

	return res;
}

static int inet6_dump_fib(struct sk_buff *skb, struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct net *net = sock_net(skb->sk);
	unsigned int h, s_h;
	unsigned int e = 0, s_e;
	struct rt6_rtnl_dump_arg arg;
	struct fib6_walker_t *w;
	struct fib6_table *tb;
	struct hlist_node *node;
	struct hlist_head *head;
	int res = 0;

	s_h = cb->args[0];
	s_e = cb->args[1];
=======
	struct rt6_rtnl_dump_arg arg = {
		.filter.dump_exceptions = true,
		.filter.dump_routes = true,
		.filter.rtnl_held = true,
	};
	const struct nlmsghdr *nlh = cb->nlh;
	struct net *net = sock_net(skb->sk);
	unsigned int h, s_h;
	unsigned int e = 0, s_e;
	struct fib6_walker *w;
	struct fib6_table *tb;
	struct hlist_head *head;
	int res = 0;

	if (cb->strict_check) {
		int err;

		err = ip_valid_fib_dump_req(net, nlh, &arg.filter, cb);
		if (err < 0)
			return err;
	} else if (nlmsg_len(nlh) >= sizeof(struct rtmsg)) {
		struct rtmsg *rtm = nlmsg_data(nlh);

		if (rtm->rtm_flags & RTM_F_PREFIX)
			arg.filter.flags = RTM_F_PREFIX;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	w = (void *)cb->args[2];
	if (!w) {
		/* New dump:
		 *
<<<<<<< HEAD
		 * 1. hook callback destructor.
		 */
		cb->args[3] = (long)cb->done;
		cb->done = fib6_dump_done;

		/*
		 * 2. allocate and initialize walker.
=======
		 * 1. allocate and initialize walker.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		w = kzalloc(sizeof(*w), GFP_ATOMIC);
		if (!w)
			return -ENOMEM;
		w->func = fib6_dump_node;
		cb->args[2] = (long)w;
<<<<<<< HEAD
=======

		/* 2. hook callback destructor.
		 */
		cb->args[3] = (long)cb->done;
		cb->done = fib6_dump_done;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	arg.skb = skb;
	arg.cb = cb;
	arg.net = net;
	w->args = &arg;

<<<<<<< HEAD
=======
	if (arg.filter.table_id) {
		tb = fib6_get_table(net, arg.filter.table_id);
		if (!tb) {
			if (rtnl_msg_family(cb->nlh) != PF_INET6)
				goto out;

			NL_SET_ERR_MSG_MOD(cb->extack, "FIB table does not exist");
			return -ENOENT;
		}

		if (!cb->args[0]) {
			res = fib6_dump_table(tb, skb, cb);
			if (!res)
				cb->args[0] = 1;
		}
		goto out;
	}

	s_h = cb->args[0];
	s_e = cb->args[1];

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_lock();
	for (h = s_h; h < FIB6_TABLE_HASHSZ; h++, s_e = 0) {
		e = 0;
		head = &net->ipv6.fib_table_hash[h];
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tb, node, head, tb6_hlist) {
=======
		hlist_for_each_entry_rcu(tb, head, tb6_hlist) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (e < s_e)
				goto next;
			res = fib6_dump_table(tb, skb, cb);
			if (res != 0)
<<<<<<< HEAD
				goto out;
=======
				goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
next:
			e++;
		}
	}
<<<<<<< HEAD
out:
	rcu_read_unlock();
	cb->args[1] = e;
	cb->args[0] = h;

=======
out_unlock:
	rcu_read_unlock();
	cb->args[1] = e;
	cb->args[0] = h;
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	res = res < 0 ? res : skb->len;
	if (res <= 0)
		fib6_dump_end(cb);
	return res;
}

<<<<<<< HEAD
=======
void fib6_metric_set(struct fib6_info *f6i, int metric, u32 val)
{
	if (!f6i)
		return;

	if (f6i->fib6_metrics == &dst_default_metrics) {
		struct dst_metrics *p = kzalloc(sizeof(*p), GFP_ATOMIC);

		if (!p)
			return;

		refcount_set(&p->refcnt, 1);
		f6i->fib6_metrics = p;
	}

	f6i->fib6_metrics->metrics[metric - 1] = val;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Routing Table
 *
 *	return the appropriate node for a routing tree "add" operation
 *	by either creating and inserting or by returning an existing
 *	node.
 */

<<<<<<< HEAD
static struct fib6_node * fib6_add_1(struct fib6_node *root, void *addr,
				     int addrlen, int plen,
				     int offset, int allow_create,
				     int replace_required)
=======
static struct fib6_node *fib6_add_1(struct net *net,
				    struct fib6_table *table,
				    struct fib6_node *root,
				    struct in6_addr *addr, int plen,
				    int offset, int allow_create,
				    int replace_required,
				    struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib6_node *fn, *in, *ln;
	struct fib6_node *pn = NULL;
	struct rt6key *key;
	int	bit;
	__be32	dir = 0;
<<<<<<< HEAD
	__u32	sernum = fib6_new_sernum();

	RT6_TRACE("fib6_add_1\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* insert node in tree */

	fn = root;

	do {
<<<<<<< HEAD
		key = (struct rt6key *)((u8 *)fn->leaf + offset);
=======
		struct fib6_info *leaf = rcu_dereference_protected(fn->leaf,
					    lockdep_is_held(&table->tb6_lock));
		key = (struct rt6key *)((u8 *)leaf + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 *	Prefix match
		 */
		if (plen < fn->fn_bit ||
		    !ipv6_prefix_equal(&key->addr, addr, fn->fn_bit)) {
			if (!allow_create) {
				if (replace_required) {
<<<<<<< HEAD
					pr_warn("IPv6: Can't replace route, "
						"no match found\n");
					return ERR_PTR(-ENOENT);
				}
				pr_warn("IPv6: NLM_F_CREATE should be set "
					"when creating new route\n");
=======
					NL_SET_ERR_MSG(extack,
						       "Can not replace route - no match found");
					pr_warn("Can't replace route, no match found\n");
					return ERR_PTR(-ENOENT);
				}
				pr_warn("NLM_F_CREATE should be set when creating new route\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
			goto insert_above;
		}

		/*
		 *	Exact match ?
		 */

		if (plen == fn->fn_bit) {
			/* clean up an intermediate node */
			if (!(fn->fn_flags & RTN_RTINFO)) {
<<<<<<< HEAD
				rt6_release(fn->leaf);
				fn->leaf = NULL;
			}

			fn->fn_sernum = sernum;

=======
				RCU_INIT_POINTER(fn->leaf, NULL);
				fib6_info_release(leaf);
			/* remove null_entry in the root node */
			} else if (fn->fn_flags & RTN_TL_ROOT &&
				   rcu_access_pointer(fn->leaf) ==
				   net->ipv6.fib6_null_entry) {
				RCU_INIT_POINTER(fn->leaf, NULL);
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return fn;
		}

		/*
		 *	We have more bits to go
		 */

		/* Try to walk down on tree. */
<<<<<<< HEAD
		fn->fn_sernum = sernum;
		dir = addr_bit_set(addr, fn->fn_bit);
		pn = fn;
		fn = dir ? fn->right: fn->left;
=======
		dir = addr_bit_set(addr, fn->fn_bit);
		pn = fn;
		fn = dir ?
		     rcu_dereference_protected(fn->right,
					lockdep_is_held(&table->tb6_lock)) :
		     rcu_dereference_protected(fn->left,
					lockdep_is_held(&table->tb6_lock));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (fn);

	if (!allow_create) {
		/* We should not create new node because
		 * NLM_F_REPLACE was specified without NLM_F_CREATE
		 * I assume it is safe to require NLM_F_CREATE when
		 * REPLACE flag is used! Later we may want to remove the
		 * check for replace_required, because according
		 * to netlink specification, NLM_F_CREATE
		 * MUST be specified if new route is created.
		 * That would keep IPv6 consistent with IPv4
		 */
		if (replace_required) {
<<<<<<< HEAD
			pr_warn("IPv6: Can't replace route, no match found\n");
			return ERR_PTR(-ENOENT);
		}
		pr_warn("IPv6: NLM_F_CREATE should be set "
			"when creating new route\n");
=======
			NL_SET_ERR_MSG(extack,
				       "Can not replace route - no match found");
			pr_warn("Can't replace route, no match found\n");
			return ERR_PTR(-ENOENT);
		}
		pr_warn("NLM_F_CREATE should be set when creating new route\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/*
	 *	We walked to the bottom of tree.
	 *	Create new leaf node without children.
	 */

<<<<<<< HEAD
	ln = node_alloc();

	if (!ln)
		return NULL;
	ln->fn_bit = plen;

	ln->parent = pn;
	ln->fn_sernum = sernum;

	if (dir)
		pn->right = ln;
	else
		pn->left  = ln;
=======
	ln = node_alloc(net);

	if (!ln)
		return ERR_PTR(-ENOMEM);
	ln->fn_bit = plen;
	RCU_INIT_POINTER(ln->parent, pn);

	if (dir)
		rcu_assign_pointer(pn->right, ln);
	else
		rcu_assign_pointer(pn->left, ln);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ln;


insert_above:
	/*
	 * split since we don't have a common prefix anymore or
	 * we have a less significant route.
	 * we've to insert an intermediate node on the list
	 * this new node will point to the one we need to create
	 * and the current
	 */

<<<<<<< HEAD
	pn = fn->parent;
=======
	pn = rcu_dereference_protected(fn->parent,
				       lockdep_is_held(&table->tb6_lock));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* find 1st bit in difference between the 2 addrs.

	   See comment in __ipv6_addr_diff: bit may be an invalid value,
	   but if it is >= plen, the value is ignored in any case.
	 */

<<<<<<< HEAD
	bit = __ipv6_addr_diff(addr, &key->addr, addrlen);
=======
	bit = __ipv6_addr_diff(addr, &key->addr, sizeof(*addr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *		(intermediate)[in]
	 *	          /	   \
	 *	(new leaf node)[ln] (old node)[fn]
	 */
	if (plen > bit) {
<<<<<<< HEAD
		in = node_alloc();
		ln = node_alloc();

		if (!in || !ln) {
			if (in)
				node_free(in);
			if (ln)
				node_free(ln);
			return NULL;
=======
		in = node_alloc(net);
		ln = node_alloc(net);

		if (!in || !ln) {
			if (in)
				node_free_immediate(net, in);
			if (ln)
				node_free_immediate(net, ln);
			return ERR_PTR(-ENOMEM);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/*
		 * new intermediate node.
		 * RTN_RTINFO will
		 * be off since that an address that chooses one of
		 * the branches would not match less specific routes
		 * in the other branch
		 */

		in->fn_bit = bit;

<<<<<<< HEAD
		in->parent = pn;
		in->leaf = fn->leaf;
		atomic_inc(&in->leaf->rt6i_ref);

		in->fn_sernum = sernum;

		/* update parent pointer */
		if (dir)
			pn->right = in;
		else
			pn->left  = in;

		ln->fn_bit = plen;

		ln->parent = in;
		fn->parent = in;

		ln->fn_sernum = sernum;

		if (addr_bit_set(addr, bit)) {
			in->right = ln;
			in->left  = fn;
		} else {
			in->left  = ln;
			in->right = fn;
=======
		RCU_INIT_POINTER(in->parent, pn);
		in->leaf = fn->leaf;
		fib6_info_hold(rcu_dereference_protected(in->leaf,
				lockdep_is_held(&table->tb6_lock)));

		/* update parent pointer */
		if (dir)
			rcu_assign_pointer(pn->right, in);
		else
			rcu_assign_pointer(pn->left, in);

		ln->fn_bit = plen;

		RCU_INIT_POINTER(ln->parent, in);
		rcu_assign_pointer(fn->parent, in);

		if (addr_bit_set(addr, bit)) {
			rcu_assign_pointer(in->right, ln);
			rcu_assign_pointer(in->left, fn);
		} else {
			rcu_assign_pointer(in->left, ln);
			rcu_assign_pointer(in->right, fn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else { /* plen <= bit */

		/*
		 *		(new leaf node)[ln]
		 *	          /	   \
		 *	     (old node)[fn] NULL
		 */

<<<<<<< HEAD
		ln = node_alloc();

		if (!ln)
			return NULL;

		ln->fn_bit = plen;

		ln->parent = pn;

		ln->fn_sernum = sernum;

		if (dir)
			pn->right = ln;
		else
			pn->left  = ln;

		if (addr_bit_set(&key->addr, plen))
			ln->right = fn;
		else
			ln->left  = fn;

		fn->parent = ln;
=======
		ln = node_alloc(net);

		if (!ln)
			return ERR_PTR(-ENOMEM);

		ln->fn_bit = plen;

		RCU_INIT_POINTER(ln->parent, pn);

		if (addr_bit_set(&key->addr, plen))
			RCU_INIT_POINTER(ln->right, fn);
		else
			RCU_INIT_POINTER(ln->left, fn);

		rcu_assign_pointer(fn->parent, ln);

		if (dir)
			rcu_assign_pointer(pn->right, ln);
		else
			rcu_assign_pointer(pn->left, ln);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ln;
}

<<<<<<< HEAD
static void fib6_purge_rt(struct rt6_info *rt, struct fib6_node *fn,
			  struct net *net)
{
	if (atomic_read(&rt->rt6i_ref) != 1) {
=======
static void __fib6_drop_pcpu_from(struct fib6_nh *fib6_nh,
				  const struct fib6_info *match,
				  const struct fib6_table *table)
{
	int cpu;

	if (!fib6_nh->rt6i_pcpu)
		return;

	/* release the reference to this fib entry from
	 * all of its cached pcpu routes
	 */
	for_each_possible_cpu(cpu) {
		struct rt6_info **ppcpu_rt;
		struct rt6_info *pcpu_rt;

		ppcpu_rt = per_cpu_ptr(fib6_nh->rt6i_pcpu, cpu);
		pcpu_rt = *ppcpu_rt;

		/* only dropping the 'from' reference if the cached route
		 * is using 'match'. The cached pcpu_rt->from only changes
		 * from a fib6_info to NULL (ip6_dst_destroy); it can never
		 * change from one fib6_info reference to another
		 */
		if (pcpu_rt && rcu_access_pointer(pcpu_rt->from) == match) {
			struct fib6_info *from;

			from = xchg((__force struct fib6_info **)&pcpu_rt->from, NULL);
			fib6_info_release(from);
		}
	}
}

struct fib6_nh_pcpu_arg {
	struct fib6_info	*from;
	const struct fib6_table *table;
};

static int fib6_nh_drop_pcpu_from(struct fib6_nh *nh, void *_arg)
{
	struct fib6_nh_pcpu_arg *arg = _arg;

	__fib6_drop_pcpu_from(nh, arg->from, arg->table);
	return 0;
}

static void fib6_drop_pcpu_from(struct fib6_info *f6i,
				const struct fib6_table *table)
{
	/* Make sure rt6_make_pcpu_route() wont add other percpu routes
	 * while we are cleaning them here.
	 */
	f6i->fib6_destroying = 1;
	mb(); /* paired with the cmpxchg() in rt6_make_pcpu_route() */

	if (f6i->nh) {
		struct fib6_nh_pcpu_arg arg = {
			.from = f6i,
			.table = table
		};

		nexthop_for_each_fib6_nh(f6i->nh, fib6_nh_drop_pcpu_from,
					 &arg);
	} else {
		struct fib6_nh *fib6_nh;

		fib6_nh = f6i->fib6_nh;
		__fib6_drop_pcpu_from(fib6_nh, f6i, table);
	}
}

static void fib6_purge_rt(struct fib6_info *rt, struct fib6_node *fn,
			  struct net *net)
{
	struct fib6_table *table = rt->fib6_table;

	/* Flush all cached dst in exception table */
	rt6_flush_exceptions(rt);
	fib6_drop_pcpu_from(rt, table);

	if (rt->nh && !list_empty(&rt->nh_list))
		list_del_init(&rt->nh_list);

	if (refcount_read(&rt->fib6_ref) != 1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* This route is used as dummy address holder in some split
		 * nodes. It is not leaked, but it still holds other resources,
		 * which must be released in time. So, scan ascendant nodes
		 * and replace dummy references to this route with references
		 * to still alive ones.
		 */
		while (fn) {
<<<<<<< HEAD
			if (!(fn->fn_flags & RTN_RTINFO) && fn->leaf == rt) {
				fn->leaf = fib6_find_prefix(net, fn);
				atomic_inc(&fn->leaf->rt6i_ref);
				rt6_release(rt);
			}
			fn = fn->parent;
		}
		/* No more references are possible at this point. */
		BUG_ON(atomic_read(&rt->rt6i_ref) != 1);
	}
=======
			struct fib6_info *leaf = rcu_dereference_protected(fn->leaf,
					    lockdep_is_held(&table->tb6_lock));
			struct fib6_info *new_leaf;
			if (!(fn->fn_flags & RTN_RTINFO) && leaf == rt) {
				new_leaf = fib6_find_prefix(net, table, fn);
				fib6_info_hold(new_leaf);

				rcu_assign_pointer(fn->leaf, new_leaf);
				fib6_info_release(rt);
			}
			fn = rcu_dereference_protected(fn->parent,
				    lockdep_is_held(&table->tb6_lock));
		}
	}

	fib6_clean_expires(rt);
	fib6_remove_gc_list(rt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Insert routing information in a node.
 */

<<<<<<< HEAD
static int fib6_add_rt2node(struct fib6_node *fn, struct rt6_info *rt,
			    struct nl_info *info)
{
	struct rt6_info *iter = NULL;
	struct rt6_info **ins;
=======
static int fib6_add_rt2node(struct fib6_node *fn, struct fib6_info *rt,
			    struct nl_info *info,
			    struct netlink_ext_ack *extack)
{
	struct fib6_info *leaf = rcu_dereference_protected(fn->leaf,
				    lockdep_is_held(&rt->fib6_table->tb6_lock));
	struct fib6_info *iter = NULL;
	struct fib6_info __rcu **ins;
	struct fib6_info __rcu **fallback_ins = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int replace = (info->nlh &&
		       (info->nlh->nlmsg_flags & NLM_F_REPLACE));
	int add = (!info->nlh ||
		   (info->nlh->nlmsg_flags & NLM_F_CREATE));
	int found = 0;
<<<<<<< HEAD

	ins = &fn->leaf;

	for (iter = fn->leaf; iter; iter = iter->dst.rt6_next) {
=======
	bool rt_can_ecmp = rt6_qualify_for_ecmp(rt);
	bool notify_sibling_rt = false;
	u16 nlflags = NLM_F_EXCL;
	int err;

	if (info->nlh && (info->nlh->nlmsg_flags & NLM_F_APPEND))
		nlflags |= NLM_F_APPEND;

	ins = &fn->leaf;

	for (iter = leaf; iter;
	     iter = rcu_dereference_protected(iter->fib6_next,
				lockdep_is_held(&rt->fib6_table->tb6_lock))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 *	Search for duplicates
		 */

<<<<<<< HEAD
		if (iter->rt6i_metric == rt->rt6i_metric) {
=======
		if (iter->fib6_metric == rt->fib6_metric) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 *	Same priority level
			 */
			if (info->nlh &&
			    (info->nlh->nlmsg_flags & NLM_F_EXCL))
				return -EEXIST;
<<<<<<< HEAD
			if (replace) {
				found++;
				break;
			}

			if (iter->dst.dev == rt->dst.dev &&
			    iter->rt6i_idev == rt->rt6i_idev &&
			    ipv6_addr_equal(&iter->rt6i_gateway,
					    &rt->rt6i_gateway)) {
				if (!(iter->rt6i_flags & RTF_EXPIRES))
					return -EEXIST;
				if (!(rt->rt6i_flags & RTF_EXPIRES))
					rt6_clean_expires(iter);
				else
					rt6_set_expires(iter, rt->dst.expires);
				return -EEXIST;
			}
		}

		if (iter->rt6i_metric > rt->rt6i_metric)
			break;

		ins = &iter->dst.rt6_next;
=======

			nlflags &= ~NLM_F_EXCL;
			if (replace) {
				if (rt_can_ecmp == rt6_qualify_for_ecmp(iter)) {
					found++;
					break;
				}
				fallback_ins = fallback_ins ?: ins;
				goto next_iter;
			}

			if (rt6_duplicate_nexthop(iter, rt)) {
				if (rt->fib6_nsiblings)
					rt->fib6_nsiblings = 0;
				if (!(iter->fib6_flags & RTF_EXPIRES))
					return -EEXIST;
				if (!(rt->fib6_flags & RTF_EXPIRES)) {
					fib6_clean_expires(iter);
					fib6_remove_gc_list(iter);
				} else {
					fib6_set_expires(iter, rt->expires);
					fib6_add_gc_list(iter);
				}

				if (rt->fib6_pmtu)
					fib6_metric_set(iter, RTAX_MTU,
							rt->fib6_pmtu);
				return -EEXIST;
			}
			/* If we have the same destination and the same metric,
			 * but not the same gateway, then the route we try to
			 * add is sibling to this route, increment our counter
			 * of siblings, and later we will add our route to the
			 * list.
			 * Only static routes (which don't have flag
			 * RTF_EXPIRES) are used for ECMPv6.
			 *
			 * To avoid long list, we only had siblings if the
			 * route have a gateway.
			 */
			if (rt_can_ecmp &&
			    rt6_qualify_for_ecmp(iter))
				rt->fib6_nsiblings++;
		}

		if (iter->fib6_metric > rt->fib6_metric)
			break;

next_iter:
		ins = &iter->fib6_next;
	}

	if (fallback_ins && !found) {
		/* No matching route with same ecmp-able-ness found, replace
		 * first matching route
		 */
		ins = fallback_ins;
		iter = rcu_dereference_protected(*ins,
				    lockdep_is_held(&rt->fib6_table->tb6_lock));
		found++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Reset round-robin state, if necessary */
	if (ins == &fn->leaf)
		fn->rr_ptr = NULL;

<<<<<<< HEAD
=======
	/* Link this route to others same route. */
	if (rt->fib6_nsiblings) {
		unsigned int fib6_nsiblings;
		struct fib6_info *sibling, *temp_sibling;

		/* Find the first route that have the same metric */
		sibling = leaf;
		notify_sibling_rt = true;
		while (sibling) {
			if (sibling->fib6_metric == rt->fib6_metric &&
			    rt6_qualify_for_ecmp(sibling)) {
				list_add_tail(&rt->fib6_siblings,
					      &sibling->fib6_siblings);
				break;
			}
			sibling = rcu_dereference_protected(sibling->fib6_next,
				    lockdep_is_held(&rt->fib6_table->tb6_lock));
			notify_sibling_rt = false;
		}
		/* For each sibling in the list, increment the counter of
		 * siblings. BUG() if counters does not match, list of siblings
		 * is broken!
		 */
		fib6_nsiblings = 0;
		list_for_each_entry_safe(sibling, temp_sibling,
					 &rt->fib6_siblings, fib6_siblings) {
			sibling->fib6_nsiblings++;
			BUG_ON(sibling->fib6_nsiblings != rt->fib6_nsiblings);
			fib6_nsiblings++;
		}
		BUG_ON(fib6_nsiblings != rt->fib6_nsiblings);
		rt6_multipath_rebalance(temp_sibling);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	insert node
	 */
	if (!replace) {
		if (!add)
<<<<<<< HEAD
			pr_warn("IPv6: NLM_F_CREATE should be set when creating new route\n");

add:
		rt->dst.rt6_next = iter;
		*ins = rt;
		rt->rt6i_node = fn;
		atomic_inc(&rt->rt6i_ref);
		inet6_rt_notify(RTM_NEWROUTE, rt, info);
=======
			pr_warn("NLM_F_CREATE should be set when creating new route\n");

add:
		nlflags |= NLM_F_CREATE;

		/* The route should only be notified if it is the first
		 * route in the node or if it is added as a sibling
		 * route to the first route in the node.
		 */
		if (!info->skip_notify_kernel &&
		    (notify_sibling_rt || ins == &fn->leaf)) {
			enum fib_event_type fib_event;

			if (notify_sibling_rt)
				fib_event = FIB_EVENT_ENTRY_APPEND;
			else
				fib_event = FIB_EVENT_ENTRY_REPLACE;
			err = call_fib6_entry_notifiers(info->nl_net,
							fib_event, rt,
							extack);
			if (err) {
				struct fib6_info *sibling, *next_sibling;

				/* If the route has siblings, then it first
				 * needs to be unlinked from them.
				 */
				if (!rt->fib6_nsiblings)
					return err;

				list_for_each_entry_safe(sibling, next_sibling,
							 &rt->fib6_siblings,
							 fib6_siblings)
					sibling->fib6_nsiblings--;
				rt->fib6_nsiblings = 0;
				list_del_init(&rt->fib6_siblings);
				rt6_multipath_rebalance(next_sibling);
				return err;
			}
		}

		rcu_assign_pointer(rt->fib6_next, iter);
		fib6_info_hold(rt);
		rcu_assign_pointer(rt->fib6_node, fn);
		rcu_assign_pointer(*ins, rt);
		if (!info->skip_notify)
			inet6_rt_notify(RTM_NEWROUTE, rt, info, nlflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		info->nl_net->ipv6.rt6_stats->fib_rt_entries++;

		if (!(fn->fn_flags & RTN_RTINFO)) {
			info->nl_net->ipv6.rt6_stats->fib_route_nodes++;
			fn->fn_flags |= RTN_RTINFO;
		}

	} else {
<<<<<<< HEAD
		if (!found) {
			if (add)
				goto add;
			pr_warn("IPv6: NLM_F_REPLACE set, but no existing node found!\n");
			return -ENOENT;
		}
		*ins = rt;
		rt->rt6i_node = fn;
		rt->dst.rt6_next = iter->dst.rt6_next;
		atomic_inc(&rt->rt6i_ref);
		inet6_rt_notify(RTM_NEWROUTE, rt, info);
=======
		int nsiblings;

		if (!found) {
			if (add)
				goto add;
			pr_warn("NLM_F_REPLACE set, but no existing node found!\n");
			return -ENOENT;
		}

		if (!info->skip_notify_kernel && ins == &fn->leaf) {
			err = call_fib6_entry_notifiers(info->nl_net,
							FIB_EVENT_ENTRY_REPLACE,
							rt, extack);
			if (err)
				return err;
		}

		fib6_info_hold(rt);
		rcu_assign_pointer(rt->fib6_node, fn);
		rt->fib6_next = iter->fib6_next;
		rcu_assign_pointer(*ins, rt);
		if (!info->skip_notify)
			inet6_rt_notify(RTM_NEWROUTE, rt, info, NLM_F_REPLACE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!(fn->fn_flags & RTN_RTINFO)) {
			info->nl_net->ipv6.rt6_stats->fib_route_nodes++;
			fn->fn_flags |= RTN_RTINFO;
		}
<<<<<<< HEAD
		fib6_purge_rt(iter, fn, info->nl_net);
		rt6_release(iter);
=======
		nsiblings = iter->fib6_nsiblings;
		iter->fib6_node = NULL;
		fib6_purge_rt(iter, fn, info->nl_net);
		if (rcu_access_pointer(fn->rr_ptr) == iter)
			fn->rr_ptr = NULL;
		fib6_info_release(iter);

		if (nsiblings) {
			/* Replacing an ECMP route, remove all siblings */
			ins = &rt->fib6_next;
			iter = rcu_dereference_protected(*ins,
				    lockdep_is_held(&rt->fib6_table->tb6_lock));
			while (iter) {
				if (iter->fib6_metric > rt->fib6_metric)
					break;
				if (rt6_qualify_for_ecmp(iter)) {
					*ins = iter->fib6_next;
					iter->fib6_node = NULL;
					fib6_purge_rt(iter, fn, info->nl_net);
					if (rcu_access_pointer(fn->rr_ptr) == iter)
						fn->rr_ptr = NULL;
					fib6_info_release(iter);
					nsiblings--;
					info->nl_net->ipv6.rt6_stats->fib_rt_entries--;
				} else {
					ins = &iter->fib6_next;
				}
				iter = rcu_dereference_protected(*ins,
					lockdep_is_held(&rt->fib6_table->tb6_lock));
			}
			WARN_ON(nsiblings != 0);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static __inline__ void fib6_start_gc(struct net *net, struct rt6_info *rt)
{
	if (!timer_pending(&net->ipv6.ip6_fib_timer) &&
	    (rt->rt6i_flags & (RTF_EXPIRES | RTF_CACHE)))
=======
static void fib6_start_gc(struct net *net, struct fib6_info *rt)
{
	if (!timer_pending(&net->ipv6.ip6_fib_timer) &&
	    (rt->fib6_flags & RTF_EXPIRES))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mod_timer(&net->ipv6.ip6_fib_timer,
			  jiffies + net->ipv6.sysctl.ip6_rt_gc_interval);
}

void fib6_force_start_gc(struct net *net)
{
	if (!timer_pending(&net->ipv6.ip6_fib_timer))
		mod_timer(&net->ipv6.ip6_fib_timer,
			  jiffies + net->ipv6.sysctl.ip6_rt_gc_interval);
}

<<<<<<< HEAD
=======
static void __fib6_update_sernum_upto_root(struct fib6_info *rt,
					   int sernum)
{
	struct fib6_node *fn = rcu_dereference_protected(rt->fib6_node,
				lockdep_is_held(&rt->fib6_table->tb6_lock));

	/* paired with smp_rmb() in fib6_get_cookie_safe() */
	smp_wmb();
	while (fn) {
		WRITE_ONCE(fn->fn_sernum, sernum);
		fn = rcu_dereference_protected(fn->parent,
				lockdep_is_held(&rt->fib6_table->tb6_lock));
	}
}

void fib6_update_sernum_upto_root(struct net *net, struct fib6_info *rt)
{
	__fib6_update_sernum_upto_root(rt, fib6_new_sernum(net));
}

/* allow ipv4 to update sernum via ipv6_stub */
void fib6_update_sernum_stub(struct net *net, struct fib6_info *f6i)
{
	spin_lock_bh(&f6i->fib6_table->tb6_lock);
	fib6_update_sernum_upto_root(net, f6i);
	spin_unlock_bh(&f6i->fib6_table->tb6_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Add routing information to the routing tree.
 *	<destination addr>/<source addr>
 *	with source addr info in sub-trees
<<<<<<< HEAD
 */

int fib6_add(struct fib6_node *root, struct rt6_info *rt, struct nl_info *info)
{
	struct fib6_node *fn, *pn = NULL;
=======
 *	Need to own table->tb6_lock
 */

int fib6_add(struct fib6_node *root, struct fib6_info *rt,
	     struct nl_info *info, struct netlink_ext_ack *extack)
{
	struct fib6_table *table = rt->fib6_table;
	struct fib6_node *fn;
#ifdef CONFIG_IPV6_SUBTREES
	struct fib6_node *pn = NULL;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = -ENOMEM;
	int allow_create = 1;
	int replace_required = 0;

	if (info->nlh) {
		if (!(info->nlh->nlmsg_flags & NLM_F_CREATE))
			allow_create = 0;
		if (info->nlh->nlmsg_flags & NLM_F_REPLACE)
			replace_required = 1;
	}
	if (!allow_create && !replace_required)
<<<<<<< HEAD
		pr_warn("IPv6: RTM_NEWROUTE with no NLM_F_CREATE or NLM_F_REPLACE\n");

	fn = fib6_add_1(root, &rt->rt6i_dst.addr, sizeof(struct in6_addr),
			rt->rt6i_dst.plen, offsetof(struct rt6_info, rt6i_dst),
			allow_create, replace_required);

	if (IS_ERR(fn)) {
		err = PTR_ERR(fn);
		fn = NULL;
	}

	if (!fn)
		goto out;

	pn = fn;

#ifdef CONFIG_IPV6_SUBTREES
	if (rt->rt6i_src.plen) {
		struct fib6_node *sn;

		if (!fn->subtree) {
=======
		pr_warn("RTM_NEWROUTE with no NLM_F_CREATE or NLM_F_REPLACE\n");

	fn = fib6_add_1(info->nl_net, table, root,
			&rt->fib6_dst.addr, rt->fib6_dst.plen,
			offsetof(struct fib6_info, fib6_dst), allow_create,
			replace_required, extack);
	if (IS_ERR(fn)) {
		err = PTR_ERR(fn);
		fn = NULL;
		goto out;
	}

#ifdef CONFIG_IPV6_SUBTREES
	pn = fn;

	if (rt->fib6_src.plen) {
		struct fib6_node *sn;

		if (!rcu_access_pointer(fn->subtree)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct fib6_node *sfn;

			/*
			 * Create subtree.
			 *
			 *		fn[main tree]
			 *		|
			 *		sfn[subtree root]
			 *		   \
			 *		    sn[new leaf node]
			 */

			/* Create subtree root node */
<<<<<<< HEAD
			sfn = node_alloc();
			if (!sfn)
				goto st_failure;

			sfn->leaf = info->nl_net->ipv6.ip6_null_entry;
			atomic_inc(&info->nl_net->ipv6.ip6_null_entry->rt6i_ref);
			sfn->fn_flags = RTN_ROOT;
			sfn->fn_sernum = fib6_new_sernum();

			/* Now add the first leaf node to new subtree */

			sn = fib6_add_1(sfn, &rt->rt6i_src.addr,
					sizeof(struct in6_addr), rt->rt6i_src.plen,
					offsetof(struct rt6_info, rt6i_src),
					allow_create, replace_required);

			if (IS_ERR(sn)) {
				err = PTR_ERR(sn);
				sn = NULL;
			}
			if (!sn) {
				/* If it is failed, discard just allocated
				   root, and then (in st_failure) stale node
				   in main tree.
				 */
				node_free(sfn);
				goto st_failure;
			}

			/* Now link new subtree to main tree */
			sfn->parent = fn;
			fn->subtree = sfn;
		} else {
			sn = fib6_add_1(fn->subtree, &rt->rt6i_src.addr,
					sizeof(struct in6_addr), rt->rt6i_src.plen,
					offsetof(struct rt6_info, rt6i_src),
					allow_create, replace_required);

			if (IS_ERR(sn)) {
				err = PTR_ERR(sn);
				sn = NULL;
			}
			if (!sn)
				goto st_failure;
		}

		if (!fn->leaf) {
			fn->leaf = rt;
			atomic_inc(&rt->rt6i_ref);
=======
			sfn = node_alloc(info->nl_net);
			if (!sfn)
				goto failure;

			fib6_info_hold(info->nl_net->ipv6.fib6_null_entry);
			rcu_assign_pointer(sfn->leaf,
					   info->nl_net->ipv6.fib6_null_entry);
			sfn->fn_flags = RTN_ROOT;

			/* Now add the first leaf node to new subtree */

			sn = fib6_add_1(info->nl_net, table, sfn,
					&rt->fib6_src.addr, rt->fib6_src.plen,
					offsetof(struct fib6_info, fib6_src),
					allow_create, replace_required, extack);

			if (IS_ERR(sn)) {
				/* If it is failed, discard just allocated
				   root, and then (in failure) stale node
				   in main tree.
				 */
				node_free_immediate(info->nl_net, sfn);
				err = PTR_ERR(sn);
				goto failure;
			}

			/* Now link new subtree to main tree */
			rcu_assign_pointer(sfn->parent, fn);
			rcu_assign_pointer(fn->subtree, sfn);
		} else {
			sn = fib6_add_1(info->nl_net, table, FIB6_SUBTREE(fn),
					&rt->fib6_src.addr, rt->fib6_src.plen,
					offsetof(struct fib6_info, fib6_src),
					allow_create, replace_required, extack);

			if (IS_ERR(sn)) {
				err = PTR_ERR(sn);
				goto failure;
			}
		}

		if (!rcu_access_pointer(fn->leaf)) {
			if (fn->fn_flags & RTN_TL_ROOT) {
				/* put back null_entry for root node */
				rcu_assign_pointer(fn->leaf,
					    info->nl_net->ipv6.fib6_null_entry);
			} else {
				fib6_info_hold(rt);
				rcu_assign_pointer(fn->leaf, rt);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		fn = sn;
	}
#endif

<<<<<<< HEAD
	err = fib6_add_rt2node(fn, rt, info);
	if (!err) {
		fib6_start_gc(info->nl_net, rt);
		if (!(rt->rt6i_flags & RTF_CACHE))
			fib6_prune_clones(info->nl_net, pn, rt);
=======
	err = fib6_add_rt2node(fn, rt, info, extack);
	if (!err) {
		if (rt->nh)
			list_add(&rt->nh_list, &rt->nh->f6i_list);
		__fib6_update_sernum_upto_root(rt, fib6_new_sernum(info->nl_net));

		if (rt->fib6_flags & RTF_EXPIRES)
			fib6_add_gc_list(rt);

		fib6_start_gc(info->nl_net, rt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

out:
	if (err) {
#ifdef CONFIG_IPV6_SUBTREES
		/*
		 * If fib6_add_1 has cleared the old leaf pointer in the
		 * super-tree leaf node we have to find a new one for it.
		 */
<<<<<<< HEAD
		if (pn != fn && pn->leaf == rt) {
			pn->leaf = NULL;
			atomic_dec(&rt->rt6i_ref);
		}
		if (pn != fn && !pn->leaf && !(pn->fn_flags & RTN_RTINFO)) {
			pn->leaf = fib6_find_prefix(info->nl_net, pn);
#if RT6_DEBUG >= 2
			if (!pn->leaf) {
				WARN_ON(pn->leaf == NULL);
				pn->leaf = info->nl_net->ipv6.ip6_null_entry;
			}
#endif
			atomic_inc(&pn->leaf->rt6i_ref);
		}
#endif
		dst_free(&rt->dst);
	}
	return err;

#ifdef CONFIG_IPV6_SUBTREES
	/* Subtree creation failed, probably main tree node
	   is orphan. If it is, shoot it.
	 */
st_failure:
	if (fn && !(fn->fn_flags & (RTN_RTINFO|RTN_ROOT)))
		fib6_repair_tree(info->nl_net, fn);
	dst_free(&rt->dst);
	return err;
#endif
=======
		if (pn != fn) {
			struct fib6_info *pn_leaf =
				rcu_dereference_protected(pn->leaf,
				    lockdep_is_held(&table->tb6_lock));
			if (pn_leaf == rt) {
				pn_leaf = NULL;
				RCU_INIT_POINTER(pn->leaf, NULL);
				fib6_info_release(rt);
			}
			if (!pn_leaf && !(pn->fn_flags & RTN_RTINFO)) {
				pn_leaf = fib6_find_prefix(info->nl_net, table,
							   pn);
				if (!pn_leaf)
					pn_leaf =
					    info->nl_net->ipv6.fib6_null_entry;
				fib6_info_hold(pn_leaf);
				rcu_assign_pointer(pn->leaf, pn_leaf);
			}
		}
#endif
		goto failure;
	} else if (fib6_requires_src(rt)) {
		fib6_routes_require_src_inc(info->nl_net);
	}
	return err;

failure:
	/* fn->leaf could be NULL and fib6_repair_tree() needs to be called if:
	 * 1. fn is an intermediate node and we failed to add the new
	 * route to it in both subtree creation failure and fib6_add_rt2node()
	 * failure case.
	 * 2. fn is the root node in the table and we fail to add the first
	 * default route to it.
	 */
	if (fn &&
	    (!(fn->fn_flags & (RTN_RTINFO|RTN_ROOT)) ||
	     (fn->fn_flags & RTN_TL_ROOT &&
	      !rcu_access_pointer(fn->leaf))))
		fib6_repair_tree(info->nl_net, table, fn);
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Routing tree lookup
 *
 */

struct lookup_args {
<<<<<<< HEAD
	int			offset;		/* key offset on rt6_info	*/
	const struct in6_addr	*addr;		/* search key			*/
};

static struct fib6_node * fib6_lookup_1(struct fib6_node *root,
					struct lookup_args *args)
=======
	int			offset;		/* key offset on fib6_info */
	const struct in6_addr	*addr;		/* search key			*/
};

static struct fib6_node *fib6_node_lookup_1(struct fib6_node *root,
					    struct lookup_args *args)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib6_node *fn;
	__be32 dir;

	if (unlikely(args->offset == 0))
		return NULL;

	/*
	 *	Descend on a tree
	 */

	fn = root;

	for (;;) {
		struct fib6_node *next;

		dir = addr_bit_set(args->addr, fn->fn_bit);

<<<<<<< HEAD
		next = dir ? fn->right : fn->left;
=======
		next = dir ? rcu_dereference(fn->right) :
			     rcu_dereference(fn->left);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (next) {
			fn = next;
			continue;
		}
		break;
	}

	while (fn) {
<<<<<<< HEAD
		if (FIB6_SUBTREE(fn) || fn->fn_flags & RTN_RTINFO) {
			struct rt6key *key;

			key = (struct rt6key *) ((u8 *) fn->leaf +
						 args->offset);

			if (ipv6_prefix_equal(&key->addr, args->addr, key->plen)) {
#ifdef CONFIG_IPV6_SUBTREES
				if (fn->subtree) {
					struct fib6_node *sfn;
					sfn = fib6_lookup_1(fn->subtree,
							    args + 1);
=======
		struct fib6_node *subtree = FIB6_SUBTREE(fn);

		if (subtree || fn->fn_flags & RTN_RTINFO) {
			struct fib6_info *leaf = rcu_dereference(fn->leaf);
			struct rt6key *key;

			if (!leaf)
				goto backtrack;

			key = (struct rt6key *) ((u8 *)leaf + args->offset);

			if (ipv6_prefix_equal(&key->addr, args->addr, key->plen)) {
#ifdef CONFIG_IPV6_SUBTREES
				if (subtree) {
					struct fib6_node *sfn;
					sfn = fib6_node_lookup_1(subtree,
								 args + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					if (!sfn)
						goto backtrack;
					fn = sfn;
				}
#endif
				if (fn->fn_flags & RTN_RTINFO)
					return fn;
			}
		}
<<<<<<< HEAD
#ifdef CONFIG_IPV6_SUBTREES
backtrack:
#endif
		if (fn->fn_flags & RTN_ROOT)
			break;

		fn = fn->parent;
=======
backtrack:
		if (fn->fn_flags & RTN_ROOT)
			break;

		fn = rcu_dereference(fn->parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return NULL;
}

<<<<<<< HEAD
struct fib6_node * fib6_lookup(struct fib6_node *root, const struct in6_addr *daddr,
			       const struct in6_addr *saddr)
=======
/* called with rcu_read_lock() held
 */
struct fib6_node *fib6_node_lookup(struct fib6_node *root,
				   const struct in6_addr *daddr,
				   const struct in6_addr *saddr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib6_node *fn;
	struct lookup_args args[] = {
		{
<<<<<<< HEAD
			.offset = offsetof(struct rt6_info, rt6i_dst),
=======
			.offset = offsetof(struct fib6_info, fib6_dst),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			.addr = daddr,
		},
#ifdef CONFIG_IPV6_SUBTREES
		{
<<<<<<< HEAD
			.offset = offsetof(struct rt6_info, rt6i_src),
=======
			.offset = offsetof(struct fib6_info, fib6_src),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			.addr = saddr,
		},
#endif
		{
			.offset = 0,	/* sentinel */
		}
	};

<<<<<<< HEAD
	fn = fib6_lookup_1(root, daddr ? args : args + 1);
=======
	fn = fib6_node_lookup_1(root, daddr ? args : args + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!fn || fn->fn_flags & RTN_TL_ROOT)
		fn = root;

	return fn;
}

/*
 *	Get node with specified destination prefix (and source prefix,
 *	if subtrees are used)
<<<<<<< HEAD
 */


static struct fib6_node * fib6_locate_1(struct fib6_node *root,
					const struct in6_addr *addr,
					int plen, int offset)
{
	struct fib6_node *fn;

	for (fn = root; fn ; ) {
		struct rt6key *key = (struct rt6key *)((u8 *)fn->leaf + offset);
=======
 *	exact_match == true means we try to find fn with exact match of
 *	the passed in prefix addr
 *	exact_match == false means we try to find fn with longest prefix
 *	match of the passed in prefix addr. This is useful for finding fn
 *	for cached route as it will be stored in the exception table under
 *	the node with longest prefix length.
 */


static struct fib6_node *fib6_locate_1(struct fib6_node *root,
				       const struct in6_addr *addr,
				       int plen, int offset,
				       bool exact_match)
{
	struct fib6_node *fn, *prev = NULL;

	for (fn = root; fn ; ) {
		struct fib6_info *leaf = rcu_dereference(fn->leaf);
		struct rt6key *key;

		/* This node is being deleted */
		if (!leaf) {
			if (plen <= fn->fn_bit)
				goto out;
			else
				goto next;
		}

		key = (struct rt6key *)((u8 *)leaf + offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 *	Prefix match
		 */
		if (plen < fn->fn_bit ||
		    !ipv6_prefix_equal(&key->addr, addr, fn->fn_bit))
<<<<<<< HEAD
			return NULL;
=======
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (plen == fn->fn_bit)
			return fn;

<<<<<<< HEAD
=======
		if (fn->fn_flags & RTN_RTINFO)
			prev = fn;

next:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 *	We have more bits to go
		 */
		if (addr_bit_set(addr, fn->fn_bit))
<<<<<<< HEAD
			fn = fn->right;
		else
			fn = fn->left;
	}
	return NULL;
}

struct fib6_node * fib6_locate(struct fib6_node *root,
			       const struct in6_addr *daddr, int dst_len,
			       const struct in6_addr *saddr, int src_len)
=======
			fn = rcu_dereference(fn->right);
		else
			fn = rcu_dereference(fn->left);
	}
out:
	if (exact_match)
		return NULL;
	else
		return prev;
}

struct fib6_node *fib6_locate(struct fib6_node *root,
			      const struct in6_addr *daddr, int dst_len,
			      const struct in6_addr *saddr, int src_len,
			      bool exact_match)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fib6_node *fn;

	fn = fib6_locate_1(root, daddr, dst_len,
<<<<<<< HEAD
			   offsetof(struct rt6_info, rt6i_dst));
=======
			   offsetof(struct fib6_info, fib6_dst),
			   exact_match);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_IPV6_SUBTREES
	if (src_len) {
		WARN_ON(saddr == NULL);
<<<<<<< HEAD
		if (fn && fn->subtree)
			fn = fib6_locate_1(fn->subtree, saddr, src_len,
					   offsetof(struct rt6_info, rt6i_src));
=======
		if (fn) {
			struct fib6_node *subtree = FIB6_SUBTREE(fn);

			if (subtree) {
				fn = fib6_locate_1(subtree, saddr, src_len,
					   offsetof(struct fib6_info, fib6_src),
					   exact_match);
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

	if (fn && fn->fn_flags & RTN_RTINFO)
		return fn;

	return NULL;
}


/*
 *	Deletion
 *
 */

<<<<<<< HEAD
static struct rt6_info *fib6_find_prefix(struct net *net, struct fib6_node *fn)
{
	if (fn->fn_flags & RTN_ROOT)
		return net->ipv6.ip6_null_entry;

	while (fn) {
		if (fn->left)
			return fn->left->leaf;
		if (fn->right)
			return fn->right->leaf;
=======
static struct fib6_info *fib6_find_prefix(struct net *net,
					 struct fib6_table *table,
					 struct fib6_node *fn)
{
	struct fib6_node *child_left, *child_right;

	if (fn->fn_flags & RTN_ROOT)
		return net->ipv6.fib6_null_entry;

	while (fn) {
		child_left = rcu_dereference_protected(fn->left,
				    lockdep_is_held(&table->tb6_lock));
		child_right = rcu_dereference_protected(fn->right,
				    lockdep_is_held(&table->tb6_lock));
		if (child_left)
			return rcu_dereference_protected(child_left->leaf,
					lockdep_is_held(&table->tb6_lock));
		if (child_right)
			return rcu_dereference_protected(child_right->leaf,
					lockdep_is_held(&table->tb6_lock));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		fn = FIB6_SUBTREE(fn);
	}
	return NULL;
}

/*
 *	Called to trim the tree of intermediate nodes when possible. "fn"
 *	is the node we want to try and remove.
<<<<<<< HEAD
 */

static struct fib6_node *fib6_repair_tree(struct net *net,
					   struct fib6_node *fn)
{
	int children;
	int nstate;
	struct fib6_node *child, *pn;
	struct fib6_walker_t *w;
	int iter = 0;

	for (;;) {
		RT6_TRACE("fixing tree: plen=%d iter=%d\n", fn->fn_bit, iter);
=======
 *	Need to own table->tb6_lock
 */

static struct fib6_node *fib6_repair_tree(struct net *net,
					  struct fib6_table *table,
					  struct fib6_node *fn)
{
	int children;
	int nstate;
	struct fib6_node *child;
	struct fib6_walker *w;
	int iter = 0;

	/* Set fn->leaf to null_entry for root node. */
	if (fn->fn_flags & RTN_TL_ROOT) {
		rcu_assign_pointer(fn->leaf, net->ipv6.fib6_null_entry);
		return fn;
	}

	for (;;) {
		struct fib6_node *fn_r = rcu_dereference_protected(fn->right,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_node *fn_l = rcu_dereference_protected(fn->left,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_node *pn = rcu_dereference_protected(fn->parent,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_node *pn_r = rcu_dereference_protected(pn->right,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_node *pn_l = rcu_dereference_protected(pn->left,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_info *fn_leaf = rcu_dereference_protected(fn->leaf,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_info *pn_leaf = rcu_dereference_protected(pn->leaf,
					    lockdep_is_held(&table->tb6_lock));
		struct fib6_info *new_fn_leaf;

		pr_debug("fixing tree: plen=%d iter=%d\n", fn->fn_bit, iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		iter++;

		WARN_ON(fn->fn_flags & RTN_RTINFO);
		WARN_ON(fn->fn_flags & RTN_TL_ROOT);
<<<<<<< HEAD
		WARN_ON(fn->leaf != NULL);

		children = 0;
		child = NULL;
		if (fn->right) child = fn->right, children |= 1;
		if (fn->left) child = fn->left, children |= 2;
=======
		WARN_ON(fn_leaf);

		children = 0;
		child = NULL;
		if (fn_r) {
			child = fn_r;
			children |= 1;
		}
		if (fn_l) {
			child = fn_l;
			children |= 2;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (children == 3 || FIB6_SUBTREE(fn)
#ifdef CONFIG_IPV6_SUBTREES
		    /* Subtree root (i.e. fn) may have one child */
		    || (children && fn->fn_flags & RTN_ROOT)
#endif
		    ) {
<<<<<<< HEAD
			fn->leaf = fib6_find_prefix(net, fn);
#if RT6_DEBUG >= 2
			if (!fn->leaf) {
				WARN_ON(!fn->leaf);
				fn->leaf = net->ipv6.ip6_null_entry;
			}
#endif
			atomic_inc(&fn->leaf->rt6i_ref);
			return fn->parent;
		}

		pn = fn->parent;
#ifdef CONFIG_IPV6_SUBTREES
		if (FIB6_SUBTREE(pn) == fn) {
			WARN_ON(!(fn->fn_flags & RTN_ROOT));
			FIB6_SUBTREE(pn) = NULL;
=======
			new_fn_leaf = fib6_find_prefix(net, table, fn);
#if RT6_DEBUG >= 2
			if (!new_fn_leaf) {
				WARN_ON(!new_fn_leaf);
				new_fn_leaf = net->ipv6.fib6_null_entry;
			}
#endif
			fib6_info_hold(new_fn_leaf);
			rcu_assign_pointer(fn->leaf, new_fn_leaf);
			return pn;
		}

#ifdef CONFIG_IPV6_SUBTREES
		if (FIB6_SUBTREE(pn) == fn) {
			WARN_ON(!(fn->fn_flags & RTN_ROOT));
			RCU_INIT_POINTER(pn->subtree, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			nstate = FWS_L;
		} else {
			WARN_ON(fn->fn_flags & RTN_ROOT);
#endif
<<<<<<< HEAD
			if (pn->right == fn) pn->right = child;
			else if (pn->left == fn) pn->left = child;
=======
			if (pn_r == fn)
				rcu_assign_pointer(pn->right, child);
			else if (pn_l == fn)
				rcu_assign_pointer(pn->left, child);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if RT6_DEBUG >= 2
			else
				WARN_ON(1);
#endif
			if (child)
<<<<<<< HEAD
				child->parent = pn;
=======
				rcu_assign_pointer(child->parent, pn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			nstate = FWS_R;
#ifdef CONFIG_IPV6_SUBTREES
		}
#endif

<<<<<<< HEAD
		read_lock(&fib6_walker_lock);
		FOR_WALKERS(w) {
			if (!child) {
				if (w->root == fn) {
					w->root = w->node = NULL;
					RT6_TRACE("W %p adjusted by delroot 1\n", w);
				} else if (w->node == fn) {
					RT6_TRACE("W %p adjusted by delnode 1, s=%d/%d\n", w, w->state, nstate);
=======
		read_lock(&net->ipv6.fib6_walker_lock);
		FOR_WALKERS(net, w) {
			if (!child) {
				if (w->node == fn) {
					pr_debug("W %p adjusted by delnode 1, s=%d/%d\n",
						 w, w->state, nstate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					w->node = pn;
					w->state = nstate;
				}
			} else {
<<<<<<< HEAD
				if (w->root == fn) {
					w->root = child;
					RT6_TRACE("W %p adjusted by delroot 2\n", w);
				}
				if (w->node == fn) {
					w->node = child;
					if (children&2) {
						RT6_TRACE("W %p adjusted by delnode 2, s=%d\n", w, w->state);
						w->state = w->state>=FWS_R ? FWS_U : FWS_INIT;
					} else {
						RT6_TRACE("W %p adjusted by delnode 2, s=%d\n", w, w->state);
						w->state = w->state>=FWS_C ? FWS_U : FWS_INIT;
=======
				if (w->node == fn) {
					w->node = child;
					if (children&2) {
						pr_debug("W %p adjusted by delnode 2, s=%d\n",
							 w, w->state);
						w->state = w->state >= FWS_R ? FWS_U : FWS_INIT;
					} else {
						pr_debug("W %p adjusted by delnode 2, s=%d\n",
							 w, w->state);
						w->state = w->state >= FWS_C ? FWS_U : FWS_INIT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					}
				}
			}
		}
<<<<<<< HEAD
		read_unlock(&fib6_walker_lock);

		node_free(fn);
		if (pn->fn_flags & RTN_RTINFO || FIB6_SUBTREE(pn))
			return pn;

		rt6_release(pn->leaf);
		pn->leaf = NULL;
=======
		read_unlock(&net->ipv6.fib6_walker_lock);

		node_free(net, fn);
		if (pn->fn_flags & RTN_RTINFO || FIB6_SUBTREE(pn))
			return pn;

		RCU_INIT_POINTER(pn->leaf, NULL);
		fib6_info_release(pn_leaf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fn = pn;
	}
}

<<<<<<< HEAD
static void fib6_del_route(struct fib6_node *fn, struct rt6_info **rtp,
			   struct nl_info *info)
{
	struct fib6_walker_t *w;
	struct rt6_info *rt = *rtp;
	struct net *net = info->nl_net;

	RT6_TRACE("fib6_del_route\n");

	/* Unlink it */
	*rtp = rt->dst.rt6_next;
	rt->rt6i_node = NULL;
=======
static void fib6_del_route(struct fib6_table *table, struct fib6_node *fn,
			   struct fib6_info __rcu **rtp, struct nl_info *info)
{
	struct fib6_info *leaf, *replace_rt = NULL;
	struct fib6_walker *w;
	struct fib6_info *rt = rcu_dereference_protected(*rtp,
				    lockdep_is_held(&table->tb6_lock));
	struct net *net = info->nl_net;
	bool notify_del = false;

	/* If the deleted route is the first in the node and it is not part of
	 * a multipath route, then we need to replace it with the next route
	 * in the node, if exists.
	 */
	leaf = rcu_dereference_protected(fn->leaf,
					 lockdep_is_held(&table->tb6_lock));
	if (leaf == rt && !rt->fib6_nsiblings) {
		if (rcu_access_pointer(rt->fib6_next))
			replace_rt = rcu_dereference_protected(rt->fib6_next,
					    lockdep_is_held(&table->tb6_lock));
		else
			notify_del = true;
	}

	/* Unlink it */
	*rtp = rt->fib6_next;
	rt->fib6_node = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	net->ipv6.rt6_stats->fib_rt_entries--;
	net->ipv6.rt6_stats->fib_discarded_routes++;

	/* Reset round-robin state, if necessary */
<<<<<<< HEAD
	if (fn->rr_ptr == rt)
		fn->rr_ptr = NULL;

	/* Adjust walkers */
	read_lock(&fib6_walker_lock);
	FOR_WALKERS(w) {
		if (w->state == FWS_C && w->leaf == rt) {
			RT6_TRACE("walker %p adjusted by delroute\n", w);
			w->leaf = rt->dst.rt6_next;
=======
	if (rcu_access_pointer(fn->rr_ptr) == rt)
		fn->rr_ptr = NULL;

	/* Remove this entry from other siblings */
	if (rt->fib6_nsiblings) {
		struct fib6_info *sibling, *next_sibling;

		/* The route is deleted from a multipath route. If this
		 * multipath route is the first route in the node, then we need
		 * to emit a delete notification. Otherwise, we need to skip
		 * the notification.
		 */
		if (rt->fib6_metric == leaf->fib6_metric &&
		    rt6_qualify_for_ecmp(leaf))
			notify_del = true;
		list_for_each_entry_safe(sibling, next_sibling,
					 &rt->fib6_siblings, fib6_siblings)
			sibling->fib6_nsiblings--;
		rt->fib6_nsiblings = 0;
		list_del_init(&rt->fib6_siblings);
		rt6_multipath_rebalance(next_sibling);
	}

	/* Adjust walkers */
	read_lock(&net->ipv6.fib6_walker_lock);
	FOR_WALKERS(net, w) {
		if (w->state == FWS_C && w->leaf == rt) {
			pr_debug("walker %p adjusted by delroute\n", w);
			w->leaf = rcu_dereference_protected(rt->fib6_next,
					    lockdep_is_held(&table->tb6_lock));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!w->leaf)
				w->state = FWS_U;
		}
	}
<<<<<<< HEAD
	read_unlock(&fib6_walker_lock);

	rt->dst.rt6_next = NULL;

	/* If it was last route, expunge its radix tree node */
	if (!fn->leaf) {
		fn->fn_flags &= ~RTN_RTINFO;
		net->ipv6.rt6_stats->fib_route_nodes--;
		fn = fib6_repair_tree(net, fn);
=======
	read_unlock(&net->ipv6.fib6_walker_lock);

	/* If it was last route, call fib6_repair_tree() to:
	 * 1. For root node, put back null_entry as how the table was created.
	 * 2. For other nodes, expunge its radix tree node.
	 */
	if (!rcu_access_pointer(fn->leaf)) {
		if (!(fn->fn_flags & RTN_TL_ROOT)) {
			fn->fn_flags &= ~RTN_RTINFO;
			net->ipv6.rt6_stats->fib_route_nodes--;
		}
		fn = fib6_repair_tree(net, table, fn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	fib6_purge_rt(rt, fn, net);

<<<<<<< HEAD
	inet6_rt_notify(RTM_DELROUTE, rt, info);
	rt6_release(rt);
}

int fib6_del(struct rt6_info *rt, struct nl_info *info)
{
	struct net *net = info->nl_net;
	struct fib6_node *fn = rt->rt6i_node;
	struct rt6_info **rtp;

#if RT6_DEBUG >= 2
	if (rt->dst.obsolete>0) {
		WARN_ON(fn != NULL);
		return -ENOENT;
	}
#endif
	if (!fn || rt == net->ipv6.ip6_null_entry)
=======
	if (!info->skip_notify_kernel) {
		if (notify_del)
			call_fib6_entry_notifiers(net, FIB_EVENT_ENTRY_DEL,
						  rt, NULL);
		else if (replace_rt)
			call_fib6_entry_notifiers_replace(net, replace_rt);
	}
	if (!info->skip_notify)
		inet6_rt_notify(RTM_DELROUTE, rt, info, 0);

	fib6_info_release(rt);
}

/* Need to own table->tb6_lock */
int fib6_del(struct fib6_info *rt, struct nl_info *info)
{
	struct net *net = info->nl_net;
	struct fib6_info __rcu **rtp;
	struct fib6_info __rcu **rtp_next;
	struct fib6_table *table;
	struct fib6_node *fn;

	if (rt == net->ipv6.fib6_null_entry)
		return -ENOENT;

	table = rt->fib6_table;
	fn = rcu_dereference_protected(rt->fib6_node,
				       lockdep_is_held(&table->tb6_lock));
	if (!fn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOENT;

	WARN_ON(!(fn->fn_flags & RTN_RTINFO));

<<<<<<< HEAD
	if (!(rt->rt6i_flags & RTF_CACHE)) {
		struct fib6_node *pn = fn;
#ifdef CONFIG_IPV6_SUBTREES
		/* clones of this route might be in another subtree */
		if (rt->rt6i_src.plen) {
			while (!(pn->fn_flags & RTN_ROOT))
				pn = pn->parent;
			pn = pn->parent;
		}
#endif
		fib6_prune_clones(info->nl_net, pn, rt);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Walk the leaf entries looking for ourself
	 */

<<<<<<< HEAD
	for (rtp = &fn->leaf; *rtp; rtp = &(*rtp)->dst.rt6_next) {
		if (*rtp == rt) {
			fib6_del_route(fn, rtp, info);
			return 0;
		}
=======
	for (rtp = &fn->leaf; *rtp; rtp = rtp_next) {
		struct fib6_info *cur = rcu_dereference_protected(*rtp,
					lockdep_is_held(&table->tb6_lock));
		if (rt == cur) {
			if (fib6_requires_src(cur))
				fib6_routes_require_src_dec(info->nl_net);
			fib6_del_route(table, fn, rtp, info);
			return 0;
		}
		rtp_next = &cur->fib6_next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return -ENOENT;
}

/*
 *	Tree traversal function.
 *
 *	Certainly, it is not interrupt safe.
 *	However, it is internally reenterable wrt itself and fib6_add/fib6_del.
 *	It means, that we can modify tree during walking
 *	and use this function for garbage collection, clone pruning,
 *	cleaning tree when a device goes down etc. etc.
 *
 *	It guarantees that every node will be traversed,
 *	and that it will be traversed only once.
 *
 *	Callback function w->func may return:
 *	0 -> continue walking.
 *	positive value -> walking is suspended (used by tree dumps,
 *	and probably by gc, if it will be split to several slices)
 *	negative value -> terminate walking.
 *
 *	The function itself returns:
 *	0   -> walk is complete.
 *	>0  -> walk is incomplete (i.e. suspended)
 *	<0  -> walk is terminated by an error.
<<<<<<< HEAD
 */

static int fib6_walk_continue(struct fib6_walker_t *w)
{
	struct fib6_node *fn, *pn;
=======
 *
 *	This function is called with tb6_lock held.
 */

static int fib6_walk_continue(struct fib6_walker *w)
{
	struct fib6_node *fn, *pn, *left, *right;

	/* w->root should always be table->tb6_root */
	WARN_ON_ONCE(!(w->root->fn_flags & RTN_TL_ROOT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (;;) {
		fn = w->node;
		if (!fn)
			return 0;

<<<<<<< HEAD
		if (w->prune && fn != w->root &&
		    fn->fn_flags & RTN_RTINFO && w->state < FWS_C) {
			w->state = FWS_C;
			w->leaf = fn->leaf;
		}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (w->state) {
#ifdef CONFIG_IPV6_SUBTREES
		case FWS_S:
			if (FIB6_SUBTREE(fn)) {
				w->node = FIB6_SUBTREE(fn);
				continue;
			}
			w->state = FWS_L;
<<<<<<< HEAD
#endif
		case FWS_L:
			if (fn->left) {
				w->node = fn->left;
=======
			fallthrough;
#endif
		case FWS_L:
			left = rcu_dereference_protected(fn->left, 1);
			if (left) {
				w->node = left;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				w->state = FWS_INIT;
				continue;
			}
			w->state = FWS_R;
<<<<<<< HEAD
		case FWS_R:
			if (fn->right) {
				w->node = fn->right;
=======
			fallthrough;
		case FWS_R:
			right = rcu_dereference_protected(fn->right, 1);
			if (right) {
				w->node = right;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				w->state = FWS_INIT;
				continue;
			}
			w->state = FWS_C;
<<<<<<< HEAD
			w->leaf = fn->leaf;
=======
			w->leaf = rcu_dereference_protected(fn->leaf, 1);
			fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case FWS_C:
			if (w->leaf && fn->fn_flags & RTN_RTINFO) {
				int err;

<<<<<<< HEAD
				if (w->count < w->skip) {
					w->count++;
					continue;
=======
				if (w->skip) {
					w->skip--;
					goto skip;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}

				err = w->func(w);
				if (err)
					return err;

				w->count++;
				continue;
			}
<<<<<<< HEAD
			w->state = FWS_U;
		case FWS_U:
			if (fn == w->root)
				return 0;
			pn = fn->parent;
=======
skip:
			w->state = FWS_U;
			fallthrough;
		case FWS_U:
			if (fn == w->root)
				return 0;
			pn = rcu_dereference_protected(fn->parent, 1);
			left = rcu_dereference_protected(pn->left, 1);
			right = rcu_dereference_protected(pn->right, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			w->node = pn;
#ifdef CONFIG_IPV6_SUBTREES
			if (FIB6_SUBTREE(pn) == fn) {
				WARN_ON(!(fn->fn_flags & RTN_ROOT));
				w->state = FWS_L;
				continue;
			}
#endif
<<<<<<< HEAD
			if (pn->left == fn) {
				w->state = FWS_R;
				continue;
			}
			if (pn->right == fn) {
				w->state = FWS_C;
				w->leaf = w->node->leaf;
=======
			if (left == fn) {
				w->state = FWS_R;
				continue;
			}
			if (right == fn) {
				w->state = FWS_C;
				w->leaf = rcu_dereference_protected(w->node->leaf, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				continue;
			}
#if RT6_DEBUG >= 2
			WARN_ON(1);
#endif
		}
	}
}

<<<<<<< HEAD
static int fib6_walk(struct fib6_walker_t *w)
=======
static int fib6_walk(struct net *net, struct fib6_walker *w)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int res;

	w->state = FWS_INIT;
	w->node = w->root;

<<<<<<< HEAD
	fib6_walker_link(w);
	res = fib6_walk_continue(w);
	if (res <= 0)
		fib6_walker_unlink(w);
	return res;
}

static int fib6_clean_node(struct fib6_walker_t *w)
{
	int res;
	struct rt6_info *rt;
	struct fib6_cleaner_t *c = container_of(w, struct fib6_cleaner_t, w);
	struct nl_info info = {
		.nl_net = c->net,
	};

	for (rt = w->leaf; rt; rt = rt->dst.rt6_next) {
		res = c->func(rt, c->arg);
		if (res < 0) {
=======
	fib6_walker_link(net, w);
	res = fib6_walk_continue(w);
	if (res <= 0)
		fib6_walker_unlink(net, w);
	return res;
}

static int fib6_clean_node(struct fib6_walker *w)
{
	int res;
	struct fib6_info *rt;
	struct fib6_cleaner *c = container_of(w, struct fib6_cleaner, w);
	struct nl_info info = {
		.nl_net = c->net,
		.skip_notify = c->skip_notify,
	};

	if (c->sernum != FIB6_NO_SERNUM_CHANGE &&
	    READ_ONCE(w->node->fn_sernum) != c->sernum)
		WRITE_ONCE(w->node->fn_sernum, c->sernum);

	if (!c->func) {
		WARN_ON_ONCE(c->sernum == FIB6_NO_SERNUM_CHANGE);
		w->leaf = NULL;
		return 0;
	}

	for_each_fib6_walker_rt(w) {
		res = c->func(rt, c->arg);
		if (res == -1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			w->leaf = rt;
			res = fib6_del(rt, &info);
			if (res) {
#if RT6_DEBUG >= 2
<<<<<<< HEAD
				printk(KERN_DEBUG "fib6_clean_node: del failed: rt=%p@%p err=%d\n", rt, rt->rt6i_node, res);
=======
				pr_debug("%s: del failed: rt=%p@%p err=%d\n",
					 __func__, rt,
					 rcu_access_pointer(rt->fib6_node),
					 res);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
				continue;
			}
			return 0;
<<<<<<< HEAD
=======
		} else if (res == -2) {
			if (WARN_ON(!rt->fib6_nsiblings))
				continue;
			rt = list_last_entry(&rt->fib6_siblings,
					     struct fib6_info, fib6_siblings);
			continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		WARN_ON(res != 0);
	}
	w->leaf = rt;
	return 0;
}

/*
 *	Convenient frontend to tree walker.
 *
 *	func is called on each route.
<<<<<<< HEAD
 *		It may return -1 -> delete this route.
 *		              0  -> continue walking
 *
 *	prune==1 -> only immediate children of node (certainly,
 *	ignoring pure split nodes) will be scanned.
 */

static void fib6_clean_tree(struct net *net, struct fib6_node *root,
			    int (*func)(struct rt6_info *, void *arg),
			    int prune, void *arg)
{
	struct fib6_cleaner_t c;

	c.w.root = root;
	c.w.func = fib6_clean_node;
	c.w.prune = prune;
	c.w.count = 0;
	c.w.skip = 0;
	c.func = func;
	c.arg = arg;
	c.net = net;

	fib6_walk(&c.w);
}

void fib6_clean_all_ro(struct net *net, int (*func)(struct rt6_info *, void *arg),
		    int prune, void *arg)
{
	struct fib6_table *table;
	struct hlist_node *node;
=======
 *		It may return -2 -> skip multipath route.
 *			      -1 -> delete this route.
 *		              0  -> continue walking
 */

static void fib6_clean_tree(struct net *net, struct fib6_node *root,
			    int (*func)(struct fib6_info *, void *arg),
			    int sernum, void *arg, bool skip_notify)
{
	struct fib6_cleaner c;

	c.w.root = root;
	c.w.func = fib6_clean_node;
	c.w.count = 0;
	c.w.skip = 0;
	c.w.skip_in_node = 0;
	c.func = func;
	c.sernum = sernum;
	c.arg = arg;
	c.net = net;
	c.skip_notify = skip_notify;

	fib6_walk(net, &c.w);
}

static void __fib6_clean_all(struct net *net,
			     int (*func)(struct fib6_info *, void *),
			     int sernum, void *arg, bool skip_notify)
{
	struct fib6_table *table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hlist_head *head;
	unsigned int h;

	rcu_read_lock();
	for (h = 0; h < FIB6_TABLE_HASHSZ; h++) {
		head = &net->ipv6.fib_table_hash[h];
<<<<<<< HEAD
		hlist_for_each_entry_rcu(table, node, head, tb6_hlist) {
			read_lock_bh(&table->tb6_lock);
			fib6_clean_tree(net, &table->tb6_root,
					func, prune, arg);
			read_unlock_bh(&table->tb6_lock);
		}
	}
	rcu_read_unlock();
}
void fib6_clean_all(struct net *net, int (*func)(struct rt6_info *, void *arg),
		    int prune, void *arg)
{
	struct fib6_table *table;
	struct hlist_node *node;
	struct hlist_head *head;
	unsigned int h;

	rcu_read_lock();
	for (h = 0; h < FIB6_TABLE_HASHSZ; h++) {
		head = &net->ipv6.fib_table_hash[h];
		hlist_for_each_entry_rcu(table, node, head, tb6_hlist) {
			write_lock_bh(&table->tb6_lock);
			fib6_clean_tree(net, &table->tb6_root,
					func, prune, arg);
			write_unlock_bh(&table->tb6_lock);
=======
		hlist_for_each_entry_rcu(table, head, tb6_hlist) {
			spin_lock_bh(&table->tb6_lock);
			fib6_clean_tree(net, &table->tb6_root,
					func, sernum, arg, skip_notify);
			spin_unlock_bh(&table->tb6_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	rcu_read_unlock();
}

<<<<<<< HEAD
static int fib6_prune_clone(struct rt6_info *rt, void *arg)
{
	if (rt->rt6i_flags & RTF_CACHE) {
		RT6_TRACE("pruning clone %p\n", rt);
		return -1;
	}

	return 0;
}

static void fib6_prune_clones(struct net *net, struct fib6_node *fn,
			      struct rt6_info *rt)
{
	fib6_clean_tree(net, fn, fib6_prune_clone, 1, rt);
=======
void fib6_clean_all(struct net *net, int (*func)(struct fib6_info *, void *),
		    void *arg)
{
	__fib6_clean_all(net, func, FIB6_NO_SERNUM_CHANGE, arg, false);
}

void fib6_clean_all_skip_notify(struct net *net,
				int (*func)(struct fib6_info *, void *),
				void *arg)
{
	__fib6_clean_all(net, func, FIB6_NO_SERNUM_CHANGE, arg, true);
}

static void fib6_flush_trees(struct net *net)
{
	int new_sernum = fib6_new_sernum(net);

	__fib6_clean_all(net, NULL, new_sernum, NULL, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Garbage collection
 */

<<<<<<< HEAD
static struct fib6_gc_args
{
	int			timeout;
	int			more;
} gc_args;

static int fib6_age(struct rt6_info *rt, void *arg)
=======
static int fib6_age(struct fib6_info *rt, struct fib6_gc_args *gc_args)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long now = jiffies;

	/*
	 *	check addrconf expiration here.
	 *	Routes are expired even if they are in use.
<<<<<<< HEAD
	 *
	 *	Also age clones. Note, that clones are aged out
	 *	only if they are not in use now.
	 */

	if (rt->rt6i_flags & RTF_EXPIRES && rt->dst.expires) {
		if (time_after(now, rt->dst.expires)) {
			RT6_TRACE("expiring %p\n", rt);
			return -1;
		}
		gc_args.more++;
	} else if (rt->rt6i_flags & RTF_CACHE) {
		if (atomic_read(&rt->dst.__refcnt) == 0 &&
		    time_after_eq(now, rt->dst.lastuse + gc_args.timeout)) {
			RT6_TRACE("aging clone %p\n", rt);
			return -1;
		} else if (rt->rt6i_flags & RTF_GATEWAY) {
			struct neighbour *neigh;
			__u8 neigh_flags = 0;

			neigh = dst_neigh_lookup(&rt->dst, &rt->rt6i_gateway);
			if (neigh) {
				neigh_flags = neigh->flags;
				neigh_release(neigh);
			}
			if (!(neigh_flags & NTF_ROUTER)) {
				RT6_TRACE("purging route %p via non-router but gateway\n",
					  rt);
				return -1;
			}
		}
		gc_args.more++;
	}

	return 0;
}

static DEFINE_SPINLOCK(fib6_gc_lock);

void fib6_run_gc(unsigned long expires, struct net *net, bool force)
{
	unsigned long now;

	if (force) {
		spin_lock_bh(&fib6_gc_lock);
	} else if (!spin_trylock_bh(&fib6_gc_lock)) {
=======
	 */

	if (rt->fib6_flags & RTF_EXPIRES && rt->expires) {
		if (time_after(now, rt->expires)) {
			pr_debug("expiring %p\n", rt);
			return -1;
		}
		gc_args->more++;
	}

	/*	Also age clones in the exception table.
	 *	Note, that clones are aged out
	 *	only if they are not in use now.
	 */
	rt6_age_exceptions(rt, gc_args, now);

	return 0;
}

static void fib6_gc_table(struct net *net,
			  struct fib6_table *tb6,
			  struct fib6_gc_args *gc_args)
{
	struct fib6_info *rt;
	struct hlist_node *n;
	struct nl_info info = {
		.nl_net = net,
		.skip_notify = false,
	};

	hlist_for_each_entry_safe(rt, n, &tb6->tb6_gc_hlist, gc_link)
		if (fib6_age(rt, gc_args) == -1)
			fib6_del(rt, &info);
}

static void fib6_gc_all(struct net *net, struct fib6_gc_args *gc_args)
{
	struct fib6_table *table;
	struct hlist_head *head;
	unsigned int h;

	rcu_read_lock();
	for (h = 0; h < FIB6_TABLE_HASHSZ; h++) {
		head = &net->ipv6.fib_table_hash[h];
		hlist_for_each_entry_rcu(table, head, tb6_hlist) {
			spin_lock_bh(&table->tb6_lock);

			fib6_gc_table(net, table, gc_args);

			spin_unlock_bh(&table->tb6_lock);
		}
	}
	rcu_read_unlock();
}

void fib6_run_gc(unsigned long expires, struct net *net, bool force)
{
	struct fib6_gc_args gc_args;
	unsigned long now;

	if (force) {
		spin_lock_bh(&net->ipv6.fib6_gc_lock);
	} else if (!spin_trylock_bh(&net->ipv6.fib6_gc_lock)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mod_timer(&net->ipv6.ip6_fib_timer, jiffies + HZ);
		return;
	}
	gc_args.timeout = expires ? (int)expires :
			  net->ipv6.sysctl.ip6_rt_gc_interval;
<<<<<<< HEAD

	gc_args.more = icmp6_dst_gc();

	fib6_clean_all(net, fib6_age, 0, NULL);
=======
	gc_args.more = 0;

	fib6_gc_all(net, &gc_args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	now = jiffies;
	net->ipv6.ip6_rt_last_gc = now;

	if (gc_args.more)
		mod_timer(&net->ipv6.ip6_fib_timer,
			  round_jiffies(now
					+ net->ipv6.sysctl.ip6_rt_gc_interval));
	else
		del_timer(&net->ipv6.ip6_fib_timer);
<<<<<<< HEAD
	spin_unlock_bh(&fib6_gc_lock);
}

static void fib6_gc_timer_cb(unsigned long arg)
{
	fib6_run_gc(0, (struct net *)arg, true);
=======
	spin_unlock_bh(&net->ipv6.fib6_gc_lock);
}

static void fib6_gc_timer_cb(struct timer_list *t)
{
	struct net *arg = from_timer(arg, t, ipv6.ip6_fib_timer);

	fib6_run_gc(0, arg, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __net_init fib6_net_init(struct net *net)
{
	size_t size = sizeof(struct hlist_head) * FIB6_TABLE_HASHSZ;
<<<<<<< HEAD

	setup_timer(&net->ipv6.ip6_fib_timer, fib6_gc_timer_cb, (unsigned long)net);

	net->ipv6.rt6_stats = kzalloc(sizeof(*net->ipv6.rt6_stats), GFP_KERNEL);
	if (!net->ipv6.rt6_stats)
		goto out_timer;
=======
	int err;

	err = fib6_notifier_init(net);
	if (err)
		return err;

	/* Default to 3-tuple */
	net->ipv6.sysctl.multipath_hash_fields =
		FIB_MULTIPATH_HASH_FIELD_DEFAULT_MASK;

	spin_lock_init(&net->ipv6.fib6_gc_lock);
	rwlock_init(&net->ipv6.fib6_walker_lock);
	INIT_LIST_HEAD(&net->ipv6.fib6_walkers);
	timer_setup(&net->ipv6.ip6_fib_timer, fib6_gc_timer_cb, 0);

	net->ipv6.rt6_stats = kzalloc(sizeof(*net->ipv6.rt6_stats), GFP_KERNEL);
	if (!net->ipv6.rt6_stats)
		goto out_notifier;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Avoid false sharing : Use at least a full cache line */
	size = max_t(size_t, size, L1_CACHE_BYTES);

	net->ipv6.fib_table_hash = kzalloc(size, GFP_KERNEL);
	if (!net->ipv6.fib_table_hash)
		goto out_rt6_stats;

	net->ipv6.fib6_main_tbl = kzalloc(sizeof(*net->ipv6.fib6_main_tbl),
					  GFP_KERNEL);
	if (!net->ipv6.fib6_main_tbl)
		goto out_fib_table_hash;

	net->ipv6.fib6_main_tbl->tb6_id = RT6_TABLE_MAIN;
<<<<<<< HEAD
	net->ipv6.fib6_main_tbl->tb6_root.leaf = net->ipv6.ip6_null_entry;
	net->ipv6.fib6_main_tbl->tb6_root.fn_flags =
		RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
=======
	rcu_assign_pointer(net->ipv6.fib6_main_tbl->tb6_root.leaf,
			   net->ipv6.fib6_null_entry);
	net->ipv6.fib6_main_tbl->tb6_root.fn_flags =
		RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
	inet_peer_base_init(&net->ipv6.fib6_main_tbl->tb6_peers);
	INIT_HLIST_HEAD(&net->ipv6.fib6_main_tbl->tb6_gc_hlist);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_IPV6_MULTIPLE_TABLES
	net->ipv6.fib6_local_tbl = kzalloc(sizeof(*net->ipv6.fib6_local_tbl),
					   GFP_KERNEL);
	if (!net->ipv6.fib6_local_tbl)
		goto out_fib6_main_tbl;
	net->ipv6.fib6_local_tbl->tb6_id = RT6_TABLE_LOCAL;
<<<<<<< HEAD
	net->ipv6.fib6_local_tbl->tb6_root.leaf = net->ipv6.ip6_null_entry;
	net->ipv6.fib6_local_tbl->tb6_root.fn_flags =
		RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
=======
	rcu_assign_pointer(net->ipv6.fib6_local_tbl->tb6_root.leaf,
			   net->ipv6.fib6_null_entry);
	net->ipv6.fib6_local_tbl->tb6_root.fn_flags =
		RTN_ROOT | RTN_TL_ROOT | RTN_RTINFO;
	inet_peer_base_init(&net->ipv6.fib6_local_tbl->tb6_peers);
	INIT_HLIST_HEAD(&net->ipv6.fib6_local_tbl->tb6_gc_hlist);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	fib6_tables_init(net);

	return 0;

#ifdef CONFIG_IPV6_MULTIPLE_TABLES
out_fib6_main_tbl:
	kfree(net->ipv6.fib6_main_tbl);
#endif
out_fib_table_hash:
	kfree(net->ipv6.fib_table_hash);
out_rt6_stats:
	kfree(net->ipv6.rt6_stats);
<<<<<<< HEAD
out_timer:
	return -ENOMEM;
 }

static void fib6_net_exit(struct net *net)
{
	rt6_ifdown(net, NULL);
	del_timer_sync(&net->ipv6.ip6_fib_timer);

#ifdef CONFIG_IPV6_MULTIPLE_TABLES
	kfree(net->ipv6.fib6_local_tbl);
#endif
	kfree(net->ipv6.fib6_main_tbl);
	kfree(net->ipv6.fib_table_hash);
	kfree(net->ipv6.rt6_stats);
=======
out_notifier:
	fib6_notifier_exit(net);
	return -ENOMEM;
}

static void fib6_net_exit(struct net *net)
{
	unsigned int i;

	del_timer_sync(&net->ipv6.ip6_fib_timer);

	for (i = 0; i < FIB6_TABLE_HASHSZ; i++) {
		struct hlist_head *head = &net->ipv6.fib_table_hash[i];
		struct hlist_node *tmp;
		struct fib6_table *tb;

		hlist_for_each_entry_safe(tb, tmp, head, tb6_hlist) {
			hlist_del(&tb->tb6_hlist);
			fib6_free_table(tb);
		}
	}

	kfree(net->ipv6.fib_table_hash);
	kfree(net->ipv6.rt6_stats);
	fib6_notifier_exit(net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations fib6_net_ops = {
	.init = fib6_net_init,
	.exit = fib6_net_exit,
};

int __init fib6_init(void)
{
	int ret = -ENOMEM;

<<<<<<< HEAD
	fib6_node_kmem = kmem_cache_create("fib6_nodes",
					   sizeof(struct fib6_node),
					   0, SLAB_HWCACHE_ALIGN,
					   NULL);
=======
	fib6_node_kmem = KMEM_CACHE(fib6_node,
				    SLAB_HWCACHE_ALIGN | SLAB_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!fib6_node_kmem)
		goto out;

	ret = register_pernet_subsys(&fib6_net_ops);
	if (ret)
		goto out_kmem_cache_create;

<<<<<<< HEAD
	ret = __rtnl_register(PF_INET6, RTM_GETROUTE, NULL, inet6_dump_fib,
			      NULL);
	if (ret)
		goto out_unregister_subsys;
=======
	ret = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETROUTE, NULL,
				   inet6_dump_fib, 0);
	if (ret)
		goto out_unregister_subsys;

	__fib6_flush_trees = fib6_flush_trees;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;

out_unregister_subsys:
	unregister_pernet_subsys(&fib6_net_ops);
out_kmem_cache_create:
	kmem_cache_destroy(fib6_node_kmem);
	goto out;
}

void fib6_gc_cleanup(void)
{
	unregister_pernet_subsys(&fib6_net_ops);
	kmem_cache_destroy(fib6_node_kmem);
}
<<<<<<< HEAD
=======

#ifdef CONFIG_PROC_FS
static int ipv6_route_native_seq_show(struct seq_file *seq, void *v)
{
	struct fib6_info *rt = v;
	struct ipv6_route_iter *iter = seq->private;
	struct fib6_nh *fib6_nh = rt->fib6_nh;
	unsigned int flags = rt->fib6_flags;
	const struct net_device *dev;

	if (rt->nh)
		fib6_nh = nexthop_fib6_nh(rt->nh);

	seq_printf(seq, "%pi6 %02x ", &rt->fib6_dst.addr, rt->fib6_dst.plen);

#ifdef CONFIG_IPV6_SUBTREES
	seq_printf(seq, "%pi6 %02x ", &rt->fib6_src.addr, rt->fib6_src.plen);
#else
	seq_puts(seq, "00000000000000000000000000000000 00 ");
#endif
	if (fib6_nh->fib_nh_gw_family) {
		flags |= RTF_GATEWAY;
		seq_printf(seq, "%pi6", &fib6_nh->fib_nh_gw6);
	} else {
		seq_puts(seq, "00000000000000000000000000000000");
	}

	dev = fib6_nh->fib_nh_dev;
	seq_printf(seq, " %08x %08x %08x %08x %8s\n",
		   rt->fib6_metric, refcount_read(&rt->fib6_ref), 0,
		   flags, dev ? dev->name : "");
	iter->w.leaf = NULL;
	return 0;
}

static int ipv6_route_yield(struct fib6_walker *w)
{
	struct ipv6_route_iter *iter = w->args;

	if (!iter->skip)
		return 1;

	do {
		iter->w.leaf = rcu_dereference_protected(
				iter->w.leaf->fib6_next,
				lockdep_is_held(&iter->tbl->tb6_lock));
		iter->skip--;
		if (!iter->skip && iter->w.leaf)
			return 1;
	} while (iter->w.leaf);

	return 0;
}

static void ipv6_route_seq_setup_walk(struct ipv6_route_iter *iter,
				      struct net *net)
{
	memset(&iter->w, 0, sizeof(iter->w));
	iter->w.func = ipv6_route_yield;
	iter->w.root = &iter->tbl->tb6_root;
	iter->w.state = FWS_INIT;
	iter->w.node = iter->w.root;
	iter->w.args = iter;
	iter->sernum = READ_ONCE(iter->w.root->fn_sernum);
	INIT_LIST_HEAD(&iter->w.lh);
	fib6_walker_link(net, &iter->w);
}

static struct fib6_table *ipv6_route_seq_next_table(struct fib6_table *tbl,
						    struct net *net)
{
	unsigned int h;
	struct hlist_node *node;

	if (tbl) {
		h = (tbl->tb6_id & (FIB6_TABLE_HASHSZ - 1)) + 1;
		node = rcu_dereference(hlist_next_rcu(&tbl->tb6_hlist));
	} else {
		h = 0;
		node = NULL;
	}

	while (!node && h < FIB6_TABLE_HASHSZ) {
		node = rcu_dereference(
			hlist_first_rcu(&net->ipv6.fib_table_hash[h++]));
	}
	return hlist_entry_safe(node, struct fib6_table, tb6_hlist);
}

static void ipv6_route_check_sernum(struct ipv6_route_iter *iter)
{
	int sernum = READ_ONCE(iter->w.root->fn_sernum);

	if (iter->sernum != sernum) {
		iter->sernum = sernum;
		iter->w.state = FWS_INIT;
		iter->w.node = iter->w.root;
		WARN_ON(iter->w.skip);
		iter->w.skip = iter->w.count;
	}
}

static void *ipv6_route_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	int r;
	struct fib6_info *n;
	struct net *net = seq_file_net(seq);
	struct ipv6_route_iter *iter = seq->private;

	++(*pos);
	if (!v)
		goto iter_table;

	n = rcu_dereference(((struct fib6_info *)v)->fib6_next);
	if (n)
		return n;

iter_table:
	ipv6_route_check_sernum(iter);
	spin_lock_bh(&iter->tbl->tb6_lock);
	r = fib6_walk_continue(&iter->w);
	spin_unlock_bh(&iter->tbl->tb6_lock);
	if (r > 0) {
		return iter->w.leaf;
	} else if (r < 0) {
		fib6_walker_unlink(net, &iter->w);
		return NULL;
	}
	fib6_walker_unlink(net, &iter->w);

	iter->tbl = ipv6_route_seq_next_table(iter->tbl, net);
	if (!iter->tbl)
		return NULL;

	ipv6_route_seq_setup_walk(iter, net);
	goto iter_table;
}

static void *ipv6_route_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct net *net = seq_file_net(seq);
	struct ipv6_route_iter *iter = seq->private;

	rcu_read_lock();
	iter->tbl = ipv6_route_seq_next_table(NULL, net);
	iter->skip = *pos;

	if (iter->tbl) {
		loff_t p = 0;

		ipv6_route_seq_setup_walk(iter, net);
		return ipv6_route_seq_next(seq, NULL, &p);
	} else {
		return NULL;
	}
}

static bool ipv6_route_iter_active(struct ipv6_route_iter *iter)
{
	struct fib6_walker *w = &iter->w;
	return w->node && !(w->state == FWS_U && w->node == w->root);
}

static void ipv6_route_native_seq_stop(struct seq_file *seq, void *v)
	__releases(RCU)
{
	struct net *net = seq_file_net(seq);
	struct ipv6_route_iter *iter = seq->private;

	if (ipv6_route_iter_active(iter))
		fib6_walker_unlink(net, &iter->w);

	rcu_read_unlock();
}

#if IS_BUILTIN(CONFIG_IPV6) && defined(CONFIG_BPF_SYSCALL)
static int ipv6_route_prog_seq_show(struct bpf_prog *prog,
				    struct bpf_iter_meta *meta,
				    void *v)
{
	struct bpf_iter__ipv6_route ctx;

	ctx.meta = meta;
	ctx.rt = v;
	return bpf_iter_run_prog(prog, &ctx);
}

static int ipv6_route_seq_show(struct seq_file *seq, void *v)
{
	struct ipv6_route_iter *iter = seq->private;
	struct bpf_iter_meta meta;
	struct bpf_prog *prog;
	int ret;

	meta.seq = seq;
	prog = bpf_iter_get_info(&meta, false);
	if (!prog)
		return ipv6_route_native_seq_show(seq, v);

	ret = ipv6_route_prog_seq_show(prog, &meta, v);
	iter->w.leaf = NULL;

	return ret;
}

static void ipv6_route_seq_stop(struct seq_file *seq, void *v)
{
	struct bpf_iter_meta meta;
	struct bpf_prog *prog;

	if (!v) {
		meta.seq = seq;
		prog = bpf_iter_get_info(&meta, true);
		if (prog)
			(void)ipv6_route_prog_seq_show(prog, &meta, v);
	}

	ipv6_route_native_seq_stop(seq, v);
}
#else
static int ipv6_route_seq_show(struct seq_file *seq, void *v)
{
	return ipv6_route_native_seq_show(seq, v);
}

static void ipv6_route_seq_stop(struct seq_file *seq, void *v)
{
	ipv6_route_native_seq_stop(seq, v);
}
#endif

const struct seq_operations ipv6_route_seq_ops = {
	.start	= ipv6_route_seq_start,
	.next	= ipv6_route_seq_next,
	.stop	= ipv6_route_seq_stop,
	.show	= ipv6_route_seq_show
};
#endif /* CONFIG_PROC_FS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
