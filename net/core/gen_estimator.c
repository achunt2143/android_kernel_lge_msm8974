<<<<<<< HEAD
/*
 * net/sched/gen_estimator.c	Simple rate estimator.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * net/sched/gen_estimator.c	Simple rate estimator.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *		Eric Dumazet <edumazet@google.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Changes:
 *              Jamal Hadi Salim - moved it to net/core and reshulfed
 *              names to make it usable in general net subsystem.
 */

<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/in.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/gen_stats.h>

/*
   This code is NOT intended to be used for statistics collection,
   its purpose is to provide a base for statistical multiplexing
   for controlled load service.
   If you need only statistics, run a user level daemon which
   periodically reads byte counters.

   Unfortunately, rate estimation is not a very easy task.
   F.e. I did not find a simple way to estimate the current peak rate
   and even failed to formulate the problem 8)8)

   So I preferred not to built an estimator into the scheduler,
   but run this task separately.
   Ideally, it should be kernel thread(s), but for now it runs
   from timers, which puts apparent top bounds on the number of rated
   flows, has minimal overhead on small, but is enough
   to handle controlled load service, sets of aggregates.

   We measure rate over A=(1<<interval) seconds and evaluate EWMA:

   avrate = avrate*(1-W) + rate*W

   where W is chosen as negative power of 2: W = 2^(-ewma_log)

   The resulting time constant is:

   T = A/(-ln(1-W))


   NOTES.

   * avbps is scaled by 2^5, avpps is scaled by 2^10.
   * both values are reported as 32 bit unsigned values. bps can
     overflow for fast links : max speed being 34360Mbit/sec
   * Minimal interval is HZ/4=250msec (it is the greatest common divisor
     for HZ=100 and HZ=1024 8)), maximal interval
     is (HZ*2^EST_MAX_INTERVAL)/4 = 8sec. Shorter intervals
     are too expensive, longer ones can be implemented
     at user level painlessly.
 */

#define EST_MAX_INTERVAL	5

struct gen_estimator
{
	struct list_head	list;
	struct gnet_stats_basic_packed	*bstats;
	struct gnet_stats_rate_est	*rate_est;
	spinlock_t		*stats_lock;
	int			ewma_log;
	u64			last_bytes;
	u64			avbps;
	u32			last_packets;
	u32			avpps;
	struct rcu_head		e_rcu;
	struct rb_node		node;
};

struct gen_estimator_head
{
	struct timer_list	timer;
	struct list_head	list;
};

static struct gen_estimator_head elist[EST_MAX_INTERVAL+1];

/* Protects against NULL dereference */
static DEFINE_RWLOCK(est_lock);

/* Protects against soft lockup during large deletion */
static struct rb_root est_root = RB_ROOT;
static DEFINE_SPINLOCK(est_tree_lock);

static void est_timer(unsigned long arg)
{
	int idx = (int)arg;
	struct gen_estimator *e;

	rcu_read_lock();
	list_for_each_entry_rcu(e, &elist[idx].list, list) {
		u64 nbytes;
		u64 brate;
		u32 npackets;
		u32 rate;

		spin_lock(e->stats_lock);
		read_lock(&est_lock);
		if (e->bstats == NULL)
			goto skip;

		nbytes = e->bstats->bytes;
		npackets = e->bstats->packets;
		brate = (nbytes - e->last_bytes)<<(7 - idx);
		e->last_bytes = nbytes;
		e->avbps += (brate >> e->ewma_log) - (e->avbps >> e->ewma_log);
		e->rate_est->bps = (e->avbps+0xF)>>5;

		rate = (npackets - e->last_packets)<<(12 - idx);
		e->last_packets = npackets;
		e->avpps += (rate >> e->ewma_log) - (e->avpps >> e->ewma_log);
		e->rate_est->pps = (e->avpps+0x1FF)>>10;
skip:
		read_unlock(&est_lock);
		spin_unlock(e->stats_lock);
	}

	if (!list_empty(&elist[idx].list))
		mod_timer(&elist[idx].timer, jiffies + ((HZ/4) << idx));
	rcu_read_unlock();
}

static void gen_add_node(struct gen_estimator *est)
{
	struct rb_node **p = &est_root.rb_node, *parent = NULL;

	while (*p) {
		struct gen_estimator *e;

		parent = *p;
		e = rb_entry(parent, struct gen_estimator, node);

		if (est->bstats > e->bstats)
			p = &parent->rb_right;
		else
			p = &parent->rb_left;
	}
	rb_link_node(&est->node, parent, p);
	rb_insert_color(&est->node, &est_root);
}

static
struct gen_estimator *gen_find_node(const struct gnet_stats_basic_packed *bstats,
				    const struct gnet_stats_rate_est *rate_est)
{
	struct rb_node *p = est_root.rb_node;

	while (p) {
		struct gen_estimator *e;

		e = rb_entry(p, struct gen_estimator, node);

		if (bstats > e->bstats)
			p = p->rb_right;
		else if (bstats < e->bstats || rate_est != e->rate_est)
			p = p->rb_left;
		else
			return e;
	}
	return NULL;
=======
#include <linux/slab.h>
#include <linux/seqlock.h>
#include <net/sock.h>
#include <net/gen_stats.h>

/* This code is NOT intended to be used for statistics collection,
 * its purpose is to provide a base for statistical multiplexing
 * for controlled load service.
 * If you need only statistics, run a user level daemon which
 * periodically reads byte counters.
 */

struct net_rate_estimator {
	struct gnet_stats_basic_sync	*bstats;
	spinlock_t		*stats_lock;
	bool			running;
	struct gnet_stats_basic_sync __percpu *cpu_bstats;
	u8			ewma_log;
	u8			intvl_log; /* period : (250ms << intvl_log) */

	seqcount_t		seq;
	u64			last_packets;
	u64			last_bytes;

	u64			avpps;
	u64			avbps;

	unsigned long           next_jiffies;
	struct timer_list       timer;
	struct rcu_head		rcu;
};

static void est_fetch_counters(struct net_rate_estimator *e,
			       struct gnet_stats_basic_sync *b)
{
	gnet_stats_basic_sync_init(b);
	if (e->stats_lock)
		spin_lock(e->stats_lock);

	gnet_stats_add_basic(b, e->cpu_bstats, e->bstats, e->running);

	if (e->stats_lock)
		spin_unlock(e->stats_lock);

}

static void est_timer(struct timer_list *t)
{
	struct net_rate_estimator *est = from_timer(est, t, timer);
	struct gnet_stats_basic_sync b;
	u64 b_bytes, b_packets;
	u64 rate, brate;

	est_fetch_counters(est, &b);
	b_bytes = u64_stats_read(&b.bytes);
	b_packets = u64_stats_read(&b.packets);

	brate = (b_bytes - est->last_bytes) << (10 - est->intvl_log);
	brate = (brate >> est->ewma_log) - (est->avbps >> est->ewma_log);

	rate = (b_packets - est->last_packets) << (10 - est->intvl_log);
	rate = (rate >> est->ewma_log) - (est->avpps >> est->ewma_log);

	write_seqcount_begin(&est->seq);
	est->avbps += brate;
	est->avpps += rate;
	write_seqcount_end(&est->seq);

	est->last_bytes = b_bytes;
	est->last_packets = b_packets;

	est->next_jiffies += ((HZ/4) << est->intvl_log);

	if (unlikely(time_after_eq(jiffies, est->next_jiffies))) {
		/* Ouch... timer was delayed. */
		est->next_jiffies = jiffies + 1;
	}
	mod_timer(&est->timer, est->next_jiffies);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * gen_new_estimator - create a new rate estimator
 * @bstats: basic statistics
<<<<<<< HEAD
 * @rate_est: rate estimator statistics
 * @stats_lock: statistics lock
=======
 * @cpu_bstats: bstats per cpu
 * @rate_est: rate estimator statistics
 * @lock: lock for statistics and control path
 * @running: true if @bstats represents a running qdisc, thus @bstats'
 *           internal values might change during basic reads. Only used
 *           if @bstats_cpu is NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @opt: rate estimator configuration TLV
 *
 * Creates a new rate estimator with &bstats as source and &rate_est
 * as destination. A new timer with the interval specified in the
 * configuration TLV is created. Upon each interval, the latest statistics
 * will be read from &bstats and the estimated rate will be stored in
<<<<<<< HEAD
 * &rate_est with the statistics lock grabed during this period.
=======
 * &rate_est with the statistics lock grabbed during this period.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns 0 on success or a negative error code.
 *
 */
<<<<<<< HEAD
int gen_new_estimator(struct gnet_stats_basic_packed *bstats,
		      struct gnet_stats_rate_est *rate_est,
		      spinlock_t *stats_lock,
		      struct nlattr *opt)
{
	struct gen_estimator *est;
	struct gnet_estimator *parm = nla_data(opt);
	int idx;
=======
int gen_new_estimator(struct gnet_stats_basic_sync *bstats,
		      struct gnet_stats_basic_sync __percpu *cpu_bstats,
		      struct net_rate_estimator __rcu **rate_est,
		      spinlock_t *lock,
		      bool running,
		      struct nlattr *opt)
{
	struct gnet_estimator *parm = nla_data(opt);
	struct net_rate_estimator *old, *est;
	struct gnet_stats_basic_sync b;
	int intvl_log;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nla_len(opt) < sizeof(*parm))
		return -EINVAL;

<<<<<<< HEAD
	if (parm->interval < -2 || parm->interval > 3)
		return -EINVAL;

	est = kzalloc(sizeof(*est), GFP_KERNEL);
	if (est == NULL)
		return -ENOBUFS;

	idx = parm->interval + 2;
	est->bstats = bstats;
	est->rate_est = rate_est;
	est->stats_lock = stats_lock;
	est->ewma_log = parm->ewma_log;
	est->last_bytes = bstats->bytes;
	est->avbps = rate_est->bps<<5;
	est->last_packets = bstats->packets;
	est->avpps = rate_est->pps<<10;

	spin_lock_bh(&est_tree_lock);
	if (!elist[idx].timer.function) {
		INIT_LIST_HEAD(&elist[idx].list);
		setup_timer(&elist[idx].timer, est_timer, idx);
	}

	if (list_empty(&elist[idx].list))
		mod_timer(&elist[idx].timer, jiffies + ((HZ/4) << idx));

	list_add_rcu(&est->list, &elist[idx].list);
	gen_add_node(est);
	spin_unlock_bh(&est_tree_lock);

=======
	/* allowed timer periods are :
	 * -2 : 250ms,   -1 : 500ms,    0 : 1 sec
	 *  1 : 2 sec,    2 : 4 sec,    3 : 8 sec
	 */
	if (parm->interval < -2 || parm->interval > 3)
		return -EINVAL;

	if (parm->ewma_log == 0 || parm->ewma_log >= 31)
		return -EINVAL;

	est = kzalloc(sizeof(*est), GFP_KERNEL);
	if (!est)
		return -ENOBUFS;

	seqcount_init(&est->seq);
	intvl_log = parm->interval + 2;
	est->bstats = bstats;
	est->stats_lock = lock;
	est->running  = running;
	est->ewma_log = parm->ewma_log;
	est->intvl_log = intvl_log;
	est->cpu_bstats = cpu_bstats;

	if (lock)
		local_bh_disable();
	est_fetch_counters(est, &b);
	if (lock)
		local_bh_enable();
	est->last_bytes = u64_stats_read(&b.bytes);
	est->last_packets = u64_stats_read(&b.packets);

	if (lock)
		spin_lock_bh(lock);
	old = rcu_dereference_protected(*rate_est, 1);
	if (old) {
		del_timer_sync(&old->timer);
		est->avbps = old->avbps;
		est->avpps = old->avpps;
	}

	est->next_jiffies = jiffies + ((HZ/4) << intvl_log);
	timer_setup(&est->timer, est_timer, 0);
	mod_timer(&est->timer, est->next_jiffies);

	rcu_assign_pointer(*rate_est, est);
	if (lock)
		spin_unlock_bh(lock);
	if (old)
		kfree_rcu(old, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
EXPORT_SYMBOL(gen_new_estimator);

/**
 * gen_kill_estimator - remove a rate estimator
<<<<<<< HEAD
 * @bstats: basic statistics
 * @rate_est: rate estimator statistics
 *
 * Removes the rate estimator specified by &bstats and &rate_est.
 *
 * Note : Caller should respect an RCU grace period before freeing stats_lock
 */
void gen_kill_estimator(struct gnet_stats_basic_packed *bstats,
			struct gnet_stats_rate_est *rate_est)
{
	struct gen_estimator *e;

	spin_lock_bh(&est_tree_lock);
	while ((e = gen_find_node(bstats, rate_est))) {
		rb_erase(&e->node, &est_root);

		write_lock(&est_lock);
		e->bstats = NULL;
		write_unlock(&est_lock);

		list_del_rcu(&e->list);
		kfree_rcu(e, e_rcu);
	}
	spin_unlock_bh(&est_tree_lock);
=======
 * @rate_est: rate estimator
 *
 * Removes the rate estimator.
 *
 */
void gen_kill_estimator(struct net_rate_estimator __rcu **rate_est)
{
	struct net_rate_estimator *est;

	est = xchg((__force struct net_rate_estimator **)rate_est, NULL);
	if (est) {
		timer_shutdown_sync(&est->timer);
		kfree_rcu(est, rcu);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(gen_kill_estimator);

/**
 * gen_replace_estimator - replace rate estimator configuration
 * @bstats: basic statistics
<<<<<<< HEAD
 * @rate_est: rate estimator statistics
 * @stats_lock: statistics lock
=======
 * @cpu_bstats: bstats per cpu
 * @rate_est: rate estimator statistics
 * @lock: lock for statistics and control path
 * @running: true if @bstats represents a running qdisc, thus @bstats'
 *           internal values might change during basic reads. Only used
 *           if @cpu_bstats is NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @opt: rate estimator configuration TLV
 *
 * Replaces the configuration of a rate estimator by calling
 * gen_kill_estimator() and gen_new_estimator().
 *
 * Returns 0 on success or a negative error code.
 */
<<<<<<< HEAD
int gen_replace_estimator(struct gnet_stats_basic_packed *bstats,
			  struct gnet_stats_rate_est *rate_est,
			  spinlock_t *stats_lock, struct nlattr *opt)
{
	gen_kill_estimator(bstats, rate_est);
	return gen_new_estimator(bstats, rate_est, stats_lock, opt);
=======
int gen_replace_estimator(struct gnet_stats_basic_sync *bstats,
			  struct gnet_stats_basic_sync __percpu *cpu_bstats,
			  struct net_rate_estimator __rcu **rate_est,
			  spinlock_t *lock,
			  bool running, struct nlattr *opt)
{
	return gen_new_estimator(bstats, cpu_bstats, rate_est,
				 lock, running, opt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(gen_replace_estimator);

/**
 * gen_estimator_active - test if estimator is currently in use
<<<<<<< HEAD
 * @bstats: basic statistics
 * @rate_est: rate estimator statistics
 *
 * Returns true if estimator is active, and false if not.
 */
bool gen_estimator_active(const struct gnet_stats_basic_packed *bstats,
			  const struct gnet_stats_rate_est *rate_est)
{
	bool res;

	ASSERT_RTNL();

	spin_lock_bh(&est_tree_lock);
	res = gen_find_node(bstats, rate_est) != NULL;
	spin_unlock_bh(&est_tree_lock);

	return res;
}
EXPORT_SYMBOL(gen_estimator_active);
=======
 * @rate_est: rate estimator
 *
 * Returns true if estimator is active, and false if not.
 */
bool gen_estimator_active(struct net_rate_estimator __rcu **rate_est)
{
	return !!rcu_access_pointer(*rate_est);
}
EXPORT_SYMBOL(gen_estimator_active);

bool gen_estimator_read(struct net_rate_estimator __rcu **rate_est,
			struct gnet_stats_rate_est64 *sample)
{
	struct net_rate_estimator *est;
	unsigned seq;

	rcu_read_lock();
	est = rcu_dereference(*rate_est);
	if (!est) {
		rcu_read_unlock();
		return false;
	}

	do {
		seq = read_seqcount_begin(&est->seq);
		sample->bps = est->avbps >> 8;
		sample->pps = est->avpps >> 8;
	} while (read_seqcount_retry(&est->seq, seq));

	rcu_read_unlock();
	return true;
}
EXPORT_SYMBOL(gen_estimator_read);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
