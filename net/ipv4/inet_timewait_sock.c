<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Generic TIME_WAIT sockets functions
 *
 *		From code orinally in TCP
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/kmemcheck.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/module.h>
#include <net/inet_hashtables.h>
#include <net/inet_timewait_sock.h>
#include <net/ip.h>


/**
<<<<<<< HEAD
 *	inet_twsk_unhash - unhash a timewait socket from established hash
 *	@tw: timewait socket
 *
 *	unhash a timewait socket from established hash, if hashed.
 *	ehash lock must be held by caller.
 *	Returns 1 if caller should call inet_twsk_put() after lock release.
 */
int inet_twsk_unhash(struct inet_timewait_sock *tw)
{
	if (hlist_nulls_unhashed(&tw->tw_node))
		return 0;

	hlist_nulls_del_rcu(&tw->tw_node);
	sk_nulls_node_init(&tw->tw_node);
	/*
	 * We cannot call inet_twsk_put() ourself under lock,
	 * caller must call it for us.
	 */
	return 1;
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	inet_twsk_bind_unhash - unhash a timewait socket from bind hash
 *	@tw: timewait socket
 *	@hashinfo: hashinfo pointer
 *
 *	unhash a timewait socket from bind hash, if hashed.
 *	bind hash lock must be held by caller.
 *	Returns 1 if caller should call inet_twsk_put() after lock release.
 */
<<<<<<< HEAD
int inet_twsk_bind_unhash(struct inet_timewait_sock *tw,
			  struct inet_hashinfo *hashinfo)
{
	struct inet_bind_bucket *tb = tw->tw_tb;

	if (!tb)
		return 0;

	__hlist_del(&tw->tw_bind_node);
	tw->tw_tb = NULL;
	inet_bind_bucket_destroy(hashinfo->bind_bucket_cachep, tb);
	/*
	 * We cannot call inet_twsk_put() ourself under lock,
	 * caller must call it for us.
	 */
	return 1;
}

/* Must be called with locally disabled BHs. */
static void __inet_twsk_kill(struct inet_timewait_sock *tw,
			     struct inet_hashinfo *hashinfo)
{
	struct inet_bind_hashbucket *bhead;
	int refcnt;
	/* Unlink from established hashes. */
	spinlock_t *lock = inet_ehash_lockp(hashinfo, tw->tw_hash);

	spin_lock(lock);
	refcnt = inet_twsk_unhash(tw);
=======
void inet_twsk_bind_unhash(struct inet_timewait_sock *tw,
			  struct inet_hashinfo *hashinfo)
{
	struct inet_bind2_bucket *tb2 = tw->tw_tb2;
	struct inet_bind_bucket *tb = tw->tw_tb;

	if (!tb)
		return;

	__sk_del_bind_node((struct sock *)tw);
	tw->tw_tb = NULL;
	tw->tw_tb2 = NULL;
	inet_bind2_bucket_destroy(hashinfo->bind2_bucket_cachep, tb2);
	inet_bind_bucket_destroy(hashinfo->bind_bucket_cachep, tb);

	__sock_put((struct sock *)tw);
}

/* Must be called with locally disabled BHs. */
static void inet_twsk_kill(struct inet_timewait_sock *tw)
{
	struct inet_hashinfo *hashinfo = tw->tw_dr->hashinfo;
	spinlock_t *lock = inet_ehash_lockp(hashinfo, tw->tw_hash);
	struct inet_bind_hashbucket *bhead, *bhead2;

	spin_lock(lock);
	sk_nulls_del_node_init_rcu((struct sock *)tw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(lock);

	/* Disassociate with bind bucket. */
	bhead = &hashinfo->bhash[inet_bhashfn(twsk_net(tw), tw->tw_num,
			hashinfo->bhash_size)];
<<<<<<< HEAD

	spin_lock(&bhead->lock);
	refcnt += inet_twsk_bind_unhash(tw, hashinfo);
	spin_unlock(&bhead->lock);

#ifdef SOCK_REFCNT_DEBUG
	if (atomic_read(&tw->tw_refcnt) != 1) {
		printk(KERN_DEBUG "%s timewait_sock %p refcnt=%d\n",
		       tw->tw_prot->name, tw, atomic_read(&tw->tw_refcnt));
	}
#endif
	while (refcnt) {
		inet_twsk_put(tw);
		refcnt--;
	}
}

static noinline void inet_twsk_free(struct inet_timewait_sock *tw)
{
	struct module *owner = tw->tw_prot->owner;
	twsk_destructor((struct sock *)tw);
#ifdef SOCK_REFCNT_DEBUG
	pr_debug("%s timewait_sock %p released\n", tw->tw_prot->name, tw);
#endif
	release_net(twsk_net(tw));
=======
	bhead2 = inet_bhashfn_portaddr(hashinfo, (struct sock *)tw,
				       twsk_net(tw), tw->tw_num);

	spin_lock(&bhead->lock);
	spin_lock(&bhead2->lock);
	inet_twsk_bind_unhash(tw, hashinfo);
	spin_unlock(&bhead2->lock);
	spin_unlock(&bhead->lock);

	refcount_dec(&tw->tw_dr->tw_refcount);
	inet_twsk_put(tw);
}

void inet_twsk_free(struct inet_timewait_sock *tw)
{
	struct module *owner = tw->tw_prot->owner;
	twsk_destructor((struct sock *)tw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kmem_cache_free(tw->tw_prot->twsk_prot->twsk_slab, tw);
	module_put(owner);
}

void inet_twsk_put(struct inet_timewait_sock *tw)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&tw->tw_refcnt))
=======
	if (refcount_dec_and_test(&tw->tw_refcnt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inet_twsk_free(tw);
}
EXPORT_SYMBOL_GPL(inet_twsk_put);

<<<<<<< HEAD
=======
static void inet_twsk_add_node_rcu(struct inet_timewait_sock *tw,
				   struct hlist_nulls_head *list)
{
	hlist_nulls_add_head_rcu(&tw->tw_node, list);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Enter the time wait state. This is called with locally disabled BH.
 * Essentially we whip up a timewait bucket, copy the relevant info into it
 * from the SK, and mess with hash chains and list linkage.
 */
<<<<<<< HEAD
void __inet_twsk_hashdance(struct inet_timewait_sock *tw, struct sock *sk,
=======
void inet_twsk_hashdance(struct inet_timewait_sock *tw, struct sock *sk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   struct inet_hashinfo *hashinfo)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);
	struct inet_ehash_bucket *ehead = inet_ehash_bucket(hashinfo, sk->sk_hash);
	spinlock_t *lock = inet_ehash_lockp(hashinfo, sk->sk_hash);
<<<<<<< HEAD
	struct inet_bind_hashbucket *bhead;
=======
	struct inet_bind_hashbucket *bhead, *bhead2;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Step 1: Put TW into bind hash. Original socket stays there too.
	   Note, that any socket with inet->num != 0 MUST be bound in
	   binding cache, even if it is closed.
	 */
	bhead = &hashinfo->bhash[inet_bhashfn(twsk_net(tw), inet->inet_num,
			hashinfo->bhash_size)];
<<<<<<< HEAD
	spin_lock(&bhead->lock);
	tw->tw_tb = icsk->icsk_bind_hash;
	WARN_ON(!icsk->icsk_bind_hash);
	inet_twsk_add_bind_node(tw, &tw->tw_tb->owners);
=======
	bhead2 = inet_bhashfn_portaddr(hashinfo, sk, twsk_net(tw), inet->inet_num);

	spin_lock(&bhead->lock);
	spin_lock(&bhead2->lock);

	tw->tw_tb = icsk->icsk_bind_hash;
	WARN_ON(!icsk->icsk_bind_hash);

	tw->tw_tb2 = icsk->icsk_bind2_hash;
	WARN_ON(!icsk->icsk_bind2_hash);
	sk_add_bind_node((struct sock *)tw, &tw->tw_tb2->owners);

	spin_unlock(&bhead2->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&bhead->lock);

	spin_lock(lock);

<<<<<<< HEAD
	/*
	 * Step 2: Hash TW into TIMEWAIT chain.
	 * Should be done before removing sk from established chain
	 * because readers are lockless and search established first.
	 */
	inet_twsk_add_node_rcu(tw, &ehead->twchain);

	/* Step 3: Remove SK from established hash. */
	if (__sk_nulls_del_node_init_rcu(sk))
		sock_prot_inuse_add(sock_net(sk), sk->sk_prot, -1);

	/*
	 * Notes :
	 * - We initially set tw_refcnt to 0 in inet_twsk_alloc()
	 * - We add one reference for the bhash link
	 * - We add one reference for the ehash link
	 * - We want this refcnt update done before allowing other
	 *   threads to find this tw in ehash chain.
	 */
	atomic_add(1 + 1 + 1, &tw->tw_refcnt);

	spin_unlock(lock);
}
EXPORT_SYMBOL_GPL(__inet_twsk_hashdance);

struct inet_timewait_sock *inet_twsk_alloc(const struct sock *sk, const int state)
{
	struct inet_timewait_sock *tw =
		kmem_cache_alloc(sk->sk_prot_creator->twsk_prot->twsk_slab,
				 GFP_ATOMIC);
	if (tw != NULL) {
		const struct inet_sock *inet = inet_sk(sk);

		kmemcheck_annotate_bitfield(tw, flags);

=======
	inet_twsk_add_node_rcu(tw, &ehead->chain);

	/* Step 3: Remove SK from hash chain */
	if (__sk_nulls_del_node_init_rcu(sk))
		sock_prot_inuse_add(sock_net(sk), sk->sk_prot, -1);

	spin_unlock(lock);

	/* tw_refcnt is set to 3 because we have :
	 * - one reference for bhash chain.
	 * - one reference for ehash chain.
	 * - one reference for timer.
	 * We can use atomic_set() because prior spin_lock()/spin_unlock()
	 * committed into memory all tw fields.
	 * Also note that after this point, we lost our implicit reference
	 * so we are not allowed to use tw anymore.
	 */
	refcount_set(&tw->tw_refcnt, 3);
}
EXPORT_SYMBOL_GPL(inet_twsk_hashdance);

static void tw_timer_handler(struct timer_list *t)
{
	struct inet_timewait_sock *tw = from_timer(tw, t, tw_timer);

	inet_twsk_kill(tw);
}

struct inet_timewait_sock *inet_twsk_alloc(const struct sock *sk,
					   struct inet_timewait_death_row *dr,
					   const int state)
{
	struct inet_timewait_sock *tw;

	if (refcount_read(&dr->tw_refcount) - 1 >=
	    READ_ONCE(dr->sysctl_max_tw_buckets))
		return NULL;

	tw = kmem_cache_alloc(sk->sk_prot_creator->twsk_prot->twsk_slab,
			      GFP_ATOMIC);
	if (tw) {
		const struct inet_sock *inet = inet_sk(sk);

		tw->tw_dr	    = dr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Give us an identity. */
		tw->tw_daddr	    = inet->inet_daddr;
		tw->tw_rcv_saddr    = inet->inet_rcv_saddr;
		tw->tw_bound_dev_if = sk->sk_bound_dev_if;
		tw->tw_tos	    = inet->tos;
		tw->tw_num	    = inet->inet_num;
		tw->tw_state	    = TCP_TIME_WAIT;
		tw->tw_substate	    = state;
		tw->tw_sport	    = inet->inet_sport;
		tw->tw_dport	    = inet->inet_dport;
		tw->tw_family	    = sk->sk_family;
		tw->tw_reuse	    = sk->sk_reuse;
<<<<<<< HEAD
		tw->tw_hash	    = sk->sk_hash;
		tw->tw_ipv6only	    = 0;
		tw->tw_transparent  = inet->transparent;
		tw->tw_prot	    = sk->sk_prot_creator;
		twsk_net_set(tw, hold_net(sock_net(sk)));
=======
		tw->tw_reuseport    = sk->sk_reuseport;
		tw->tw_hash	    = sk->sk_hash;
		tw->tw_ipv6only	    = 0;
		tw->tw_transparent  = inet_test_bit(TRANSPARENT, sk);
		tw->tw_prot	    = sk->sk_prot_creator;
		atomic64_set(&tw->tw_cookie, atomic64_read(&sk->sk_cookie));
		twsk_net_set(tw, sock_net(sk));
		timer_setup(&tw->tw_timer, tw_timer_handler, TIMER_PINNED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Because we use RCU lookups, we should not set tw_refcnt
		 * to a non null value before everything is setup for this
		 * timewait socket.
		 */
<<<<<<< HEAD
		atomic_set(&tw->tw_refcnt, 0);
		inet_twsk_dead_node_init(tw);
=======
		refcount_set(&tw->tw_refcnt, 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__module_get(tw->tw_prot->owner);
	}

	return tw;
}
EXPORT_SYMBOL_GPL(inet_twsk_alloc);

<<<<<<< HEAD
/* Returns non-zero if quota exceeded.  */
static int inet_twdr_do_twkill_work(struct inet_timewait_death_row *twdr,
				    const int slot)
{
	struct inet_timewait_sock *tw;
	struct hlist_node *node;
	unsigned int killed;
	int ret;

	/* NOTE: compare this to previous version where lock
	 * was released after detaching chain. It was racy,
	 * because tw buckets are scheduled in not serialized context
	 * in 2.3 (with netfilter), and with softnet it is common, because
	 * soft irqs are not sequenced.
	 */
	killed = 0;
	ret = 0;
rescan:
	inet_twsk_for_each_inmate(tw, node, &twdr->cells[slot]) {
		__inet_twsk_del_dead_node(tw);
		spin_unlock(&twdr->death_lock);
		__inet_twsk_kill(tw, twdr->hashinfo);
#ifdef CONFIG_NET_NS
		NET_INC_STATS_BH(twsk_net(tw), LINUX_MIB_TIMEWAITED);
#endif
		inet_twsk_put(tw);
		killed++;
		spin_lock(&twdr->death_lock);
		if (killed > INET_TWDR_TWKILL_QUOTA) {
			ret = 1;
			break;
		}

		/* While we dropped twdr->death_lock, another cpu may have
		 * killed off the next TW bucket in the list, therefore
		 * do a fresh re-read of the hlist head node with the
		 * lock reacquired.  We still use the hlist traversal
		 * macro in order to get the prefetches.
		 */
		goto rescan;
	}

	twdr->tw_count -= killed;
#ifndef CONFIG_NET_NS
	NET_ADD_STATS_BH(&init_net, LINUX_MIB_TIMEWAITED, killed);
#endif
	return ret;
}

void inet_twdr_hangman(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int unsigned need_timer;

	twdr = (struct inet_timewait_death_row *)data;
	spin_lock(&twdr->death_lock);

	if (twdr->tw_count == 0)
		goto out;

	need_timer = 0;
	if (inet_twdr_do_twkill_work(twdr, twdr->slot)) {
		twdr->thread_slots |= (1 << twdr->slot);
		schedule_work(&twdr->twkill_work);
		need_timer = 1;
	} else {
		/* We purged the entire slot, anything left?  */
		if (twdr->tw_count)
			need_timer = 1;
		twdr->slot = ((twdr->slot + 1) & (INET_TWDR_TWKILL_SLOTS - 1));
	}
	if (need_timer)
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);
out:
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twdr_hangman);

void inet_twdr_twkill_work(struct work_struct *work)
{
	struct inet_timewait_death_row *twdr =
		container_of(work, struct inet_timewait_death_row, twkill_work);
	int i;

	BUILD_BUG_ON((INET_TWDR_TWKILL_SLOTS - 1) >
			(sizeof(twdr->thread_slots) * 8));

	while (twdr->thread_slots) {
		spin_lock_bh(&twdr->death_lock);
		for (i = 0; i < INET_TWDR_TWKILL_SLOTS; i++) {
			if (!(twdr->thread_slots & (1 << i)))
				continue;

			while (inet_twdr_do_twkill_work(twdr, i) != 0) {
				if (need_resched()) {
					spin_unlock_bh(&twdr->death_lock);
					schedule();
					spin_lock_bh(&twdr->death_lock);
				}
			}

			twdr->thread_slots &= ~(1 << i);
		}
		spin_unlock_bh(&twdr->death_lock);
	}
}
EXPORT_SYMBOL_GPL(inet_twdr_twkill_work);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* These are always called from BH context.  See callers in
 * tcp_input.c to verify this.
 */

<<<<<<< HEAD
/* This is for handling early-kills of TIME_WAIT sockets. */
void inet_twsk_deschedule(struct inet_timewait_sock *tw,
			  struct inet_timewait_death_row *twdr)
{
	spin_lock(&twdr->death_lock);
	if (inet_twsk_del_dead_node(tw)) {
		inet_twsk_put(tw);
		if (--twdr->tw_count == 0)
			del_timer(&twdr->tw_timer);
	}
	spin_unlock(&twdr->death_lock);
	__inet_twsk_kill(tw, twdr->hashinfo);
}
EXPORT_SYMBOL(inet_twsk_deschedule);

void inet_twsk_schedule(struct inet_timewait_sock *tw,
		       struct inet_timewait_death_row *twdr,
		       const int timeo, const int timewait_len)
{
	struct hlist_head *list;
	int slot;

=======
/* This is for handling early-kills of TIME_WAIT sockets.
 * Warning : consume reference.
 * Caller should not access tw anymore.
 */
void inet_twsk_deschedule_put(struct inet_timewait_sock *tw)
{
	if (del_timer_sync(&tw->tw_timer))
		inet_twsk_kill(tw);
	inet_twsk_put(tw);
}
EXPORT_SYMBOL(inet_twsk_deschedule_put);

void __inet_twsk_schedule(struct inet_timewait_sock *tw, int timeo, bool rearm)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* timeout := RTO * 3.5
	 *
	 * 3.5 = 1+2+0.5 to wait for two retransmits.
	 *
	 * RATIONALE: if FIN arrived and we entered TIME-WAIT state,
	 * our ACK acking that FIN can be lost. If N subsequent retransmitted
	 * FINs (or previous seqments) are lost (probability of such event
	 * is p^(N+1), where p is probability to lose single packet and
	 * time to detect the loss is about RTO*(2^N - 1) with exponential
	 * backoff). Normal timewait length is calculated so, that we
	 * waited at least for one retransmitted FIN (maximal RTO is 120sec).
	 * [ BTW Linux. following BSD, violates this requirement waiting
	 *   only for 60sec, we should wait at least for 240 secs.
	 *   Well, 240 consumes too much of resources 8)
	 * ]
	 * This interval is not reduced to catch old duplicate and
	 * responces to our wandering segments living for two MSLs.
	 * However, if we use PAWS to detect
	 * old duplicates, we can reduce the interval to bounds required
	 * by RTO, rather than MSL. So, if peer understands PAWS, we
	 * kill tw bucket after 3.5*RTO (it is important that this number
	 * is greater than TS tick!) and detect old duplicates with help
	 * of PAWS.
	 */
<<<<<<< HEAD
	slot = (timeo + (1 << INET_TWDR_RECYCLE_TICK) - 1) >> INET_TWDR_RECYCLE_TICK;

	spin_lock(&twdr->death_lock);

	/* Unlink it, if it was scheduled */
	if (inet_twsk_del_dead_node(tw))
		twdr->tw_count--;
	else
		atomic_inc(&tw->tw_refcnt);

	if (slot >= INET_TWDR_RECYCLE_SLOTS) {
		/* Schedule to slow timer */
		if (timeo >= timewait_len) {
			slot = INET_TWDR_TWKILL_SLOTS - 1;
		} else {
			slot = DIV_ROUND_UP(timeo, twdr->period);
			if (slot >= INET_TWDR_TWKILL_SLOTS)
				slot = INET_TWDR_TWKILL_SLOTS - 1;
		}
		tw->tw_ttd = jiffies + timeo;
		slot = (twdr->slot + slot) & (INET_TWDR_TWKILL_SLOTS - 1);
		list = &twdr->cells[slot];
	} else {
		tw->tw_ttd = jiffies + (slot << INET_TWDR_RECYCLE_TICK);

		if (twdr->twcal_hand < 0) {
			twdr->twcal_hand = 0;
			twdr->twcal_jiffie = jiffies;
			twdr->twcal_timer.expires = twdr->twcal_jiffie +
					      (slot << INET_TWDR_RECYCLE_TICK);
			add_timer(&twdr->twcal_timer);
		} else {
			if (time_after(twdr->twcal_timer.expires,
				       jiffies + (slot << INET_TWDR_RECYCLE_TICK)))
				mod_timer(&twdr->twcal_timer,
					  jiffies + (slot << INET_TWDR_RECYCLE_TICK));
			slot = (twdr->twcal_hand + slot) & (INET_TWDR_RECYCLE_SLOTS - 1);
		}
		list = &twdr->twcal_row[slot];
	}

	hlist_add_head(&tw->tw_death_node, list);

	if (twdr->tw_count++ == 0)
		mod_timer(&twdr->tw_timer, jiffies + twdr->period);
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twsk_schedule);

void inet_twdr_twcal_tick(unsigned long data)
{
	struct inet_timewait_death_row *twdr;
	int n, slot;
	unsigned long j;
	unsigned long now = jiffies;
	int killed = 0;
	int adv = 0;

	twdr = (struct inet_timewait_death_row *)data;

	spin_lock(&twdr->death_lock);
	if (twdr->twcal_hand < 0)
		goto out;

	slot = twdr->twcal_hand;
	j = twdr->twcal_jiffie;

	for (n = 0; n < INET_TWDR_RECYCLE_SLOTS; n++) {
		if (time_before_eq(j, now)) {
			struct hlist_node *node, *safe;
			struct inet_timewait_sock *tw;

			inet_twsk_for_each_inmate_safe(tw, node, safe,
						       &twdr->twcal_row[slot]) {
				__inet_twsk_del_dead_node(tw);
				__inet_twsk_kill(tw, twdr->hashinfo);
#ifdef CONFIG_NET_NS
				NET_INC_STATS_BH(twsk_net(tw), LINUX_MIB_TIMEWAITKILLED);
#endif
				inet_twsk_put(tw);
				killed++;
			}
		} else {
			if (!adv) {
				adv = 1;
				twdr->twcal_jiffie = j;
				twdr->twcal_hand = slot;
			}

			if (!hlist_empty(&twdr->twcal_row[slot])) {
				mod_timer(&twdr->twcal_timer, j);
				goto out;
			}
		}
		j += 1 << INET_TWDR_RECYCLE_TICK;
		slot = (slot + 1) & (INET_TWDR_RECYCLE_SLOTS - 1);
	}
	twdr->twcal_hand = -1;

out:
	if ((twdr->tw_count -= killed) == 0)
		del_timer(&twdr->tw_timer);
#ifndef CONFIG_NET_NS
	NET_ADD_STATS_BH(&init_net, LINUX_MIB_TIMEWAITKILLED, killed);
#endif
	spin_unlock(&twdr->death_lock);
}
EXPORT_SYMBOL_GPL(inet_twdr_twcal_tick);

void inet_twsk_purge(struct inet_hashinfo *hashinfo,
		     struct inet_timewait_death_row *twdr, int family)
{
	struct inet_timewait_sock *tw;
	struct sock *sk;
	struct hlist_nulls_node *node;
	unsigned int slot;
=======

	if (!rearm) {
		bool kill = timeo <= 4*HZ;

		__NET_INC_STATS(twsk_net(tw), kill ? LINUX_MIB_TIMEWAITKILLED :
						     LINUX_MIB_TIMEWAITED);
		BUG_ON(mod_timer(&tw->tw_timer, jiffies + timeo));
		refcount_inc(&tw->tw_dr->tw_refcount);
	} else {
		mod_timer_pending(&tw->tw_timer, jiffies + timeo);
	}
}
EXPORT_SYMBOL_GPL(__inet_twsk_schedule);

/* Remove all non full sockets (TIME_WAIT and NEW_SYN_RECV) for dead netns */
void inet_twsk_purge(struct inet_hashinfo *hashinfo, int family)
{
	struct hlist_nulls_node *node;
	unsigned int slot;
	struct sock *sk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (slot = 0; slot <= hashinfo->ehash_mask; slot++) {
		struct inet_ehash_bucket *head = &hashinfo->ehash[slot];
restart_rcu:
<<<<<<< HEAD
		rcu_read_lock();
restart:
		sk_nulls_for_each_rcu(sk, node, &head->twchain) {
			tw = inet_twsk(sk);
			if ((tw->tw_family != family) ||
				atomic_read(&twsk_net(tw)->count))
				continue;

			if (unlikely(!atomic_inc_not_zero(&tw->tw_refcnt)))
				continue;

			if (unlikely((tw->tw_family != family) ||
				     atomic_read(&twsk_net(tw)->count))) {
				inet_twsk_put(tw);
=======
		cond_resched();
		rcu_read_lock();
restart:
		sk_nulls_for_each_rcu(sk, node, &head->chain) {
			int state = inet_sk_state_load(sk);

			if ((1 << state) & ~(TCPF_TIME_WAIT |
					     TCPF_NEW_SYN_RECV))
				continue;

			if (sk->sk_family != family ||
			    refcount_read(&sock_net(sk)->ns.count))
				continue;

			if (unlikely(!refcount_inc_not_zero(&sk->sk_refcnt)))
				continue;

			if (unlikely(sk->sk_family != family ||
				     refcount_read(&sock_net(sk)->ns.count))) {
				sock_gen_put(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto restart;
			}

			rcu_read_unlock();
			local_bh_disable();
<<<<<<< HEAD
			inet_twsk_deschedule(tw, twdr);
			local_bh_enable();
			inet_twsk_put(tw);
=======
			if (state == TCP_TIME_WAIT) {
				inet_twsk_deschedule_put(inet_twsk(sk));
			} else {
				struct request_sock *req = inet_reqsk(sk);

				inet_csk_reqsk_queue_drop_and_put(req->rsk_listener,
								  req);
			}
			local_bh_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto restart_rcu;
		}
		/* If the nulls value we got at the end of this lookup is
		 * not the expected one, we must restart lookup.
		 * We probably met an item that was moved to another chain.
		 */
		if (get_nulls_value(node) != slot)
			goto restart;
		rcu_read_unlock();
	}
}
EXPORT_SYMBOL_GPL(inet_twsk_purge);
