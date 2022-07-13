<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * fs/dcache.c
 *
 * Complete reimplementation
 * (C) 1997 Thomas Schoebel-Theuer,
 * with heavy changes by Linus Torvalds
 */

/*
 * Notes on the allocation strategy:
 *
 * The dcache is a master of the icache - whenever a dcache entry
 * exists, the inode will always exist. "iput()" is done either when
 * the dcache entry is deleted or garbage collected.
 */

<<<<<<< HEAD
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/fs.h>
=======
#include <linux/ratelimit.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/fscrypt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fsnotify.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/hash.h>
#include <linux/cache.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/mount.h>
#include <linux/file.h>
#include <asm/uaccess.h>
#include <linux/security.h>
#include <linux/seqlock.h>
#include <linux/swap.h>
#include <linux/bootmem.h>
#include <linux/fs_struct.h>
#include <linux/hardirq.h>
#include <linux/bit_spinlock.h>
#include <linux/rculist_bl.h>
#include <linux/prefetch.h>
#include <linux/ratelimit.h>
=======
#include <linux/security.h>
#include <linux/seqlock.h>
#include <linux/memblock.h>
#include <linux/bit_spinlock.h>
#include <linux/rculist_bl.h>
#include <linux/list_lru.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"
#include "mount.h"

/*
 * Usage:
 * dcache->d_inode->i_lock protects:
 *   - i_dentry, d_u.d_alias, d_inode of aliases
 * dcache_hash_bucket lock protects:
 *   - the dcache hash table
<<<<<<< HEAD
 * s_anon bl list spinlock protects:
 *   - the s_anon list (see __d_drop)
 * dcache_lru_lock protects:
=======
 * s_roots bl list spinlock protects:
 *   - the s_roots list (see __d_drop)
 * dentry->d_sb->s_dentry_lru_lock protects:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   - the dcache lru lists and counters
 * d_lock protects:
 *   - d_flags
 *   - d_name
 *   - d_lru
 *   - d_count
 *   - d_unhashed()
<<<<<<< HEAD
 *   - d_parent and d_subdirs
 *   - childrens' d_child and d_parent
=======
 *   - d_parent and d_chilren
 *   - childrens' d_sib and d_parent
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   - d_u.d_alias, d_inode
 *
 * Ordering:
 * dentry->d_inode->i_lock
 *   dentry->d_lock
<<<<<<< HEAD
 *     dcache_lru_lock
 *     dcache_hash_bucket lock
 *     s_anon lock
=======
 *     dentry->d_sb->s_dentry_lru_lock
 *     dcache_hash_bucket lock
 *     s_roots lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If there is an ancestor relationship:
 * dentry->d_parent->...->d_parent->d_lock
 *   ...
 *     dentry->d_parent->d_lock
 *       dentry->d_lock
 *
 * If no ancestor relationship:
<<<<<<< HEAD
 * if (dentry1 < dentry2)
 *   dentry1->d_lock
 *     dentry2->d_lock
=======
 * arbitrary, since it's serialized on rename_lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int sysctl_vfs_cache_pressure __read_mostly = 100;
EXPORT_SYMBOL_GPL(sysctl_vfs_cache_pressure);

<<<<<<< HEAD
static __cacheline_aligned_in_smp DEFINE_SPINLOCK(dcache_lru_lock);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
__cacheline_aligned_in_smp DEFINE_SEQLOCK(rename_lock);

EXPORT_SYMBOL(rename_lock);

<<<<<<< HEAD
static struct kmem_cache *dentry_cache __read_mostly;
=======
static struct kmem_cache *dentry_cache __ro_after_init;

const struct qstr empty_name = QSTR_INIT("", 0);
EXPORT_SYMBOL(empty_name);
const struct qstr slash_name = QSTR_INIT("/", 1);
EXPORT_SYMBOL(slash_name);
const struct qstr dotdot_name = QSTR_INIT("..", 2);
EXPORT_SYMBOL(dotdot_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the single most critical data structure when it comes
 * to the dcache: the hashtable for lookups. Somebody should try
 * to make this good - I've just made it work.
 *
 * This hash-function tries to avoid losing too many bits of hash
 * information, yet avoid using a prime hash-size or similar.
 */
<<<<<<< HEAD
#define D_HASHBITS     d_hash_shift
#define D_HASHMASK     d_hash_mask

static unsigned int d_hash_mask __read_mostly;
static unsigned int d_hash_shift __read_mostly;

static struct hlist_bl_head *dentry_hashtable __read_mostly;

static inline struct hlist_bl_head *d_hash(const struct dentry *parent,
					unsigned int hash)
{
	hash += (unsigned long) parent / L1_CACHE_BYTES;
	hash = hash + (hash >> D_HASHBITS);
	return dentry_hashtable + (hash & D_HASHMASK);
}

/* Statistics gathering. */
struct dentry_stat_t dentry_stat = {
	.age_limit = 45,
};

static DEFINE_PER_CPU(unsigned int, nr_dentry);

#if defined(CONFIG_SYSCTL) && defined(CONFIG_PROC_FS)
static int get_nr_dentry(void)
{
	int i;
	int sum = 0;
=======

static unsigned int d_hash_shift __ro_after_init;

static struct hlist_bl_head *dentry_hashtable __ro_after_init;

static inline struct hlist_bl_head *d_hash(unsigned int hash)
{
	return dentry_hashtable + (hash >> d_hash_shift);
}

#define IN_LOOKUP_SHIFT 10
static struct hlist_bl_head in_lookup_hashtable[1 << IN_LOOKUP_SHIFT];

static inline struct hlist_bl_head *in_lookup_hash(const struct dentry *parent,
					unsigned int hash)
{
	hash += (unsigned long) parent / L1_CACHE_BYTES;
	return in_lookup_hashtable + hash_32(hash, IN_LOOKUP_SHIFT);
}

struct dentry_stat_t {
	long nr_dentry;
	long nr_unused;
	long age_limit;		/* age in seconds */
	long want_pages;	/* pages requested by system */
	long nr_negative;	/* # of unused negative dentries */
	long dummy;		/* Reserved for future use */
};

static DEFINE_PER_CPU(long, nr_dentry);
static DEFINE_PER_CPU(long, nr_dentry_unused);
static DEFINE_PER_CPU(long, nr_dentry_negative);

#if defined(CONFIG_SYSCTL) && defined(CONFIG_PROC_FS)
/* Statistics gathering. */
static struct dentry_stat_t dentry_stat = {
	.age_limit = 45,
};

/*
 * Here we resort to our own counters instead of using generic per-cpu counters
 * for consistency with what the vfs inode code does. We are expected to harvest
 * better code and performance by having our own specialized counters.
 *
 * Please note that the loop is done over all possible CPUs, not over all online
 * CPUs. The reason for this is that we don't want to play games with CPUs going
 * on and off. If one of them goes off, we will just keep their counters.
 *
 * glommer: See cffbc8a for details, and if you ever intend to change this,
 * please update all vfs counters to match.
 */
static long get_nr_dentry(void)
{
	int i;
	long sum = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for_each_possible_cpu(i)
		sum += per_cpu(nr_dentry, i);
	return sum < 0 ? 0 : sum;
}

<<<<<<< HEAD
int proc_nr_dentry(ctl_table *table, int write, void __user *buffer,
		   size_t *lenp, loff_t *ppos)
{
	dentry_stat.nr_dentry = get_nr_dentry();
	return proc_dointvec(table, write, buffer, lenp, ppos);
}
=======
static long get_nr_dentry_unused(void)
{
	int i;
	long sum = 0;
	for_each_possible_cpu(i)
		sum += per_cpu(nr_dentry_unused, i);
	return sum < 0 ? 0 : sum;
}

static long get_nr_dentry_negative(void)
{
	int i;
	long sum = 0;

	for_each_possible_cpu(i)
		sum += per_cpu(nr_dentry_negative, i);
	return sum < 0 ? 0 : sum;
}

static int proc_nr_dentry(struct ctl_table *table, int write, void *buffer,
			  size_t *lenp, loff_t *ppos)
{
	dentry_stat.nr_dentry = get_nr_dentry();
	dentry_stat.nr_unused = get_nr_dentry_unused();
	dentry_stat.nr_negative = get_nr_dentry_negative();
	return proc_doulongvec_minmax(table, write, buffer, lenp, ppos);
}

static struct ctl_table fs_dcache_sysctls[] = {
	{
		.procname	= "dentry-state",
		.data		= &dentry_stat,
		.maxlen		= 6*sizeof(long),
		.mode		= 0444,
		.proc_handler	= proc_nr_dentry,
	},
};

static int __init init_fs_dcache_sysctls(void)
{
	register_sysctl_init("fs", fs_dcache_sysctls);
	return 0;
}
fs_initcall(init_fs_dcache_sysctls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * Compare 2 name strings, return 0 if they match, otherwise non-zero.
 * The strings are both count bytes long, and count is non-zero.
 */
#ifdef CONFIG_DCACHE_WORD_ACCESS

#include <asm/word-at-a-time.h>
/*
 * NOTE! 'cs' and 'scount' come from a dentry, so it has a
 * aligned allocation for this particular component. We don't
 * strictly need the load_unaligned_zeropad() safety, but it
 * doesn't hurt either.
 *
 * In contrast, 'ct' and 'tcount' can be from a pathname, and do
 * need the careful unaligned handling.
 */
static inline int dentry_string_cmp(const unsigned char *cs, const unsigned char *ct, unsigned tcount)
{
	unsigned long a,b,mask;

	for (;;) {
<<<<<<< HEAD
		a = *(unsigned long *)cs;
=======
		a = read_word_at_a_time(cs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		b = load_unaligned_zeropad(ct);
		if (tcount < sizeof(unsigned long))
			break;
		if (unlikely(a != b))
			return 1;
		cs += sizeof(unsigned long);
		ct += sizeof(unsigned long);
		tcount -= sizeof(unsigned long);
		if (!tcount)
			return 0;
	}
<<<<<<< HEAD
	mask = ~(~0ul << tcount*8);
=======
	mask = bytemask_from_count(tcount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return unlikely(!!((a ^ b) & mask));
}

#else

static inline int dentry_string_cmp(const unsigned char *cs, const unsigned char *ct, unsigned tcount)
{
	do {
		if (*cs != *ct)
			return 1;
		cs++;
		ct++;
		tcount--;
	} while (tcount);
	return 0;
}

#endif

static inline int dentry_cmp(const struct dentry *dentry, const unsigned char *ct, unsigned tcount)
{
<<<<<<< HEAD
	const unsigned char *cs;
	/*
	 * Be careful about RCU walk racing with rename:
	 * use ACCESS_ONCE to fetch the name pointer.
=======
	/*
	 * Be careful about RCU walk racing with rename:
	 * use 'READ_ONCE' to fetch the name pointer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * NOTE! Even if a rename will mean that the length
	 * was not loaded atomically, we don't care. The
	 * RCU walk will check the sequence count eventually,
	 * and catch it. And we won't overrun the buffer,
	 * because we're reading the name pointer atomically,
	 * and a dentry name is guaranteed to be properly
	 * terminated with a NUL byte.
	 *
	 * End result: even if 'len' is wrong, we'll exit
	 * early because the data cannot match (there can
	 * be no NUL in the ct/tcount data)
	 */
<<<<<<< HEAD
	cs = ACCESS_ONCE(dentry->d_name.name);
	smp_read_barrier_depends();
	return dentry_string_cmp(cs, ct, tcount);
}

=======
	const unsigned char *cs = READ_ONCE(dentry->d_name.name);

	return dentry_string_cmp(cs, ct, tcount);
}

struct external_name {
	union {
		atomic_t count;
		struct rcu_head head;
	} u;
	unsigned char name[];
};

static inline struct external_name *external_name(struct dentry *dentry)
{
	return container_of(dentry->d_name.name, struct external_name, name[0]);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __d_free(struct rcu_head *head)
{
	struct dentry *dentry = container_of(head, struct dentry, d_u.d_rcu);

<<<<<<< HEAD
	if (dname_external(dentry))
		kfree(dentry->d_name.name);
	kmem_cache_free(dentry_cache, dentry); 
}

/*
 * no locks, please.
 */
static void d_free(struct dentry *dentry)
{
	WARN_ON(!list_empty(&dentry->d_u.d_alias));
	BUG_ON(dentry->d_count);
	this_cpu_dec(nr_dentry);
	if (dentry->d_op && dentry->d_op->d_release)
		dentry->d_op->d_release(dentry);

	/* if dentry was never visible to RCU, immediate free is OK */
	if (!(dentry->d_flags & DCACHE_RCUACCESS))
		__d_free(&dentry->d_u.d_rcu);
	else
		call_rcu(&dentry->d_u.d_rcu, __d_free);
=======
	kmem_cache_free(dentry_cache, dentry); 
}

static void __d_free_external(struct rcu_head *head)
{
	struct dentry *dentry = container_of(head, struct dentry, d_u.d_rcu);
	kfree(external_name(dentry));
	kmem_cache_free(dentry_cache, dentry);
}

static inline int dname_external(const struct dentry *dentry)
{
	return dentry->d_name.name != dentry->d_iname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void take_dentry_name_snapshot(struct name_snapshot *name, struct dentry *dentry)
{
	spin_lock(&dentry->d_lock);
<<<<<<< HEAD
	if (unlikely(dname_external(dentry))) {
		u32 len;
		char *p;

		for (;;) {
			len = dentry->d_name.len;
			spin_unlock(&dentry->d_lock);

			p = kmalloc(len + 1, GFP_KERNEL | __GFP_NOFAIL);

			spin_lock(&dentry->d_lock);
			if (dentry->d_name.len <= len)
				break;
			kfree(p);
		}
		memcpy(p, dentry->d_name.name, dentry->d_name.len + 1);
		spin_unlock(&dentry->d_lock);

		name->name = p;
	} else {
		memcpy(name->inline_name, dentry->d_iname, DNAME_INLINE_LEN);
		spin_unlock(&dentry->d_lock);
		name->name = name->inline_name;
	}
=======
	name->name = dentry->d_name;
	if (unlikely(dname_external(dentry))) {
		atomic_inc(&external_name(dentry)->u.count);
	} else {
		memcpy(name->inline_name, dentry->d_iname,
		       dentry->d_name.len + 1);
		name->name.name = name->inline_name;
	}
	spin_unlock(&dentry->d_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(take_dentry_name_snapshot);

void release_dentry_name_snapshot(struct name_snapshot *name)
{
<<<<<<< HEAD
	if (unlikely(name->name != name->inline_name))
		kfree(name->name);
}
EXPORT_SYMBOL(release_dentry_name_snapshot);

/**
 * dentry_rcuwalk_barrier - invalidate in-progress rcu-walk lookups
 * @dentry: the target dentry
 * After this call, in-progress rcu-walk path lookup will fail. This
 * should be called after unhashing, and after changing d_inode (if
 * the dentry has not already been unhashed).
 */
static inline void dentry_rcuwalk_barrier(struct dentry *dentry)
{
	assert_spin_locked(&dentry->d_lock);
	/* Go through a barrier */
	write_seqcount_barrier(&dentry->d_seq);
}

/*
 * Release the dentry's inode, using the filesystem
 * d_iput() operation if defined. Dentry has no refcount
 * and is unhashed.
 */
static void dentry_iput(struct dentry * dentry)
	__releases(dentry->d_lock)
	__releases(dentry->d_inode->i_lock)
{
	struct inode *inode = dentry->d_inode;
	if (inode) {
		dentry->d_inode = NULL;
		list_del_init(&dentry->d_u.d_alias);
		spin_unlock(&dentry->d_lock);
		spin_unlock(&inode->i_lock);
		if (!inode->i_nlink)
			fsnotify_inoderemove(inode);
		if (dentry->d_op && dentry->d_op->d_iput)
			dentry->d_op->d_iput(dentry, inode);
		else
			iput(inode);
	} else {
		spin_unlock(&dentry->d_lock);
	}
=======
	if (unlikely(name->name.name != name->inline_name)) {
		struct external_name *p;
		p = container_of(name->name.name, struct external_name, name[0]);
		if (unlikely(atomic_dec_and_test(&p->u.count)))
			kfree_rcu(p, u.head);
	}
}
EXPORT_SYMBOL(release_dentry_name_snapshot);

static inline void __d_set_inode_and_type(struct dentry *dentry,
					  struct inode *inode,
					  unsigned type_flags)
{
	unsigned flags;

	dentry->d_inode = inode;
	flags = READ_ONCE(dentry->d_flags);
	flags &= ~DCACHE_ENTRY_TYPE;
	flags |= type_flags;
	smp_store_release(&dentry->d_flags, flags);
}

static inline void __d_clear_type_and_inode(struct dentry *dentry)
{
	unsigned flags = READ_ONCE(dentry->d_flags);

	flags &= ~DCACHE_ENTRY_TYPE;
	WRITE_ONCE(dentry->d_flags, flags);
	dentry->d_inode = NULL;
	if (dentry->d_flags & DCACHE_LRU_LIST)
		this_cpu_inc(nr_dentry_negative);
}

static void dentry_free(struct dentry *dentry)
{
	WARN_ON(!hlist_unhashed(&dentry->d_u.d_alias));
	if (unlikely(dname_external(dentry))) {
		struct external_name *p = external_name(dentry);
		if (likely(atomic_dec_and_test(&p->u.count))) {
			call_rcu(&dentry->d_u.d_rcu, __d_free_external);
			return;
		}
	}
	/* if dentry was never visible to RCU, immediate free is OK */
	if (dentry->d_flags & DCACHE_NORCU)
		__d_free(&dentry->d_u.d_rcu);
	else
		call_rcu(&dentry->d_u.d_rcu, __d_free);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Release the dentry's inode, using the filesystem
<<<<<<< HEAD
 * d_iput() operation if defined. dentry remains in-use.
=======
 * d_iput() operation if defined.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void dentry_unlink_inode(struct dentry * dentry)
	__releases(dentry->d_lock)
	__releases(dentry->d_inode->i_lock)
{
	struct inode *inode = dentry->d_inode;
<<<<<<< HEAD
	dentry->d_inode = NULL;
	list_del_init(&dentry->d_u.d_alias);
	dentry_rcuwalk_barrier(dentry);
=======

	raw_write_seqcount_begin(&dentry->d_seq);
	__d_clear_type_and_inode(dentry);
	hlist_del_init(&dentry->d_u.d_alias);
	raw_write_seqcount_end(&dentry->d_seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&dentry->d_lock);
	spin_unlock(&inode->i_lock);
	if (!inode->i_nlink)
		fsnotify_inoderemove(inode);
	if (dentry->d_op && dentry->d_op->d_iput)
		dentry->d_op->d_iput(dentry, inode);
	else
		iput(inode);
}

/*
<<<<<<< HEAD
 * dentry_lru_(add|del|prune|move_tail) must be called with d_lock held.
 */
static void dentry_lru_add(struct dentry *dentry)
{
	if (list_empty(&dentry->d_lru)) {
		spin_lock(&dcache_lru_lock);
		list_add(&dentry->d_lru, &dentry->d_sb->s_dentry_lru);
		dentry->d_sb->s_nr_dentry_unused++;
		dentry_stat.nr_unused++;
		spin_unlock(&dcache_lru_lock);
	}
}

static void __dentry_lru_del(struct dentry *dentry)
{
	list_del_init(&dentry->d_lru);
	dentry->d_flags &= ~DCACHE_SHRINK_LIST;
	dentry->d_sb->s_nr_dentry_unused--;
	dentry_stat.nr_unused--;
}

/*
 * Remove a dentry with references from the LRU.
 */
static void dentry_lru_del(struct dentry *dentry)
{
	if (!list_empty(&dentry->d_lru)) {
		spin_lock(&dcache_lru_lock);
		__dentry_lru_del(dentry);
		spin_unlock(&dcache_lru_lock);
	}
}

/*
 * Remove a dentry that is unreferenced and about to be pruned
 * (unhashed and destroyed) from the LRU, and inform the file system.
 * This wrapper should be called _prior_ to unhashing a victim dentry.
 */
static void dentry_lru_prune(struct dentry *dentry)
{
	if (!list_empty(&dentry->d_lru)) {
		if (dentry->d_flags & DCACHE_OP_PRUNE)
			dentry->d_op->d_prune(dentry);

		spin_lock(&dcache_lru_lock);
		__dentry_lru_del(dentry);
		spin_unlock(&dcache_lru_lock);
	}
}

static void dentry_lru_move_list(struct dentry *dentry, struct list_head *list)
{
	spin_lock(&dcache_lru_lock);
	if (list_empty(&dentry->d_lru)) {
		list_add_tail(&dentry->d_lru, list);
		dentry->d_sb->s_nr_dentry_unused++;
		dentry_stat.nr_unused++;
	} else {
		list_move_tail(&dentry->d_lru, list);
	}
	spin_unlock(&dcache_lru_lock);
}

/**
 * d_kill - kill dentry and return parent
 * @dentry: dentry to kill
 * @parent: parent dentry
 *
 * The dentry must already be unhashed and removed from the LRU.
 *
 * If this is the root of the dentry tree, return NULL.
 *
 * dentry->d_lock and parent->d_lock must be held by caller, and are dropped by
 * d_kill.
 */
static struct dentry *d_kill(struct dentry *dentry, struct dentry *parent)
	__releases(dentry->d_lock)
	__releases(parent->d_lock)
	__releases(dentry->d_inode->i_lock)
{
	__list_del_entry(&dentry->d_child);
	/*
	 * Inform ascending readers that we are no longer attached to the
	 * dentry tree
	 */
	dentry->d_flags |= DCACHE_DENTRY_KILLED;
	if (parent)
		spin_unlock(&parent->d_lock);
	dentry_iput(dentry);
	/*
	 * dentry_iput drops the locks, at which point nobody (except
	 * transient RCU lookups) can reach this dentry.
	 */
	d_free(dentry);
	return parent;
}

/*
 * Unhash a dentry without inserting an RCU walk barrier or checking that
 * dentry->d_lock is locked.  The caller must take care of that, if
 * appropriate.
 */
static void __d_shrink(struct dentry *dentry)
{
	if (!d_unhashed(dentry)) {
		struct hlist_bl_head *b;
		if (unlikely(dentry->d_flags & DCACHE_DISCONNECTED))
			b = &dentry->d_sb->s_anon;
		else
			b = d_hash(dentry->d_parent, dentry->d_name.hash);

		hlist_bl_lock(b);
		__hlist_bl_del(&dentry->d_hash);
		dentry->d_hash.pprev = NULL;
		hlist_bl_unlock(b);
	}
}
=======
 * The DCACHE_LRU_LIST bit is set whenever the 'd_lru' entry
 * is in use - which includes both the "real" per-superblock
 * LRU list _and_ the DCACHE_SHRINK_LIST use.
 *
 * The DCACHE_SHRINK_LIST bit is set whenever the dentry is
 * on the shrink list (ie not on the superblock LRU list).
 *
 * The per-cpu "nr_dentry_unused" counters are updated with
 * the DCACHE_LRU_LIST bit.
 *
 * The per-cpu "nr_dentry_negative" counters are only updated
 * when deleted from or added to the per-superblock LRU list, not
 * from/to the shrink list. That is to avoid an unneeded dec/inc
 * pair when moving from LRU to shrink list in select_collect().
 *
 * These helper functions make sure we always follow the
 * rules. d_lock must be held by the caller.
 */
#define D_FLAG_VERIFY(dentry,x) WARN_ON_ONCE(((dentry)->d_flags & (DCACHE_LRU_LIST | DCACHE_SHRINK_LIST)) != (x))
static void d_lru_add(struct dentry *dentry)
{
	D_FLAG_VERIFY(dentry, 0);
	dentry->d_flags |= DCACHE_LRU_LIST;
	this_cpu_inc(nr_dentry_unused);
	if (d_is_negative(dentry))
		this_cpu_inc(nr_dentry_negative);
	WARN_ON_ONCE(!list_lru_add_obj(
			&dentry->d_sb->s_dentry_lru, &dentry->d_lru));
}

static void d_lru_del(struct dentry *dentry)
{
	D_FLAG_VERIFY(dentry, DCACHE_LRU_LIST);
	dentry->d_flags &= ~DCACHE_LRU_LIST;
	this_cpu_dec(nr_dentry_unused);
	if (d_is_negative(dentry))
		this_cpu_dec(nr_dentry_negative);
	WARN_ON_ONCE(!list_lru_del_obj(
			&dentry->d_sb->s_dentry_lru, &dentry->d_lru));
}

static void d_shrink_del(struct dentry *dentry)
{
	D_FLAG_VERIFY(dentry, DCACHE_SHRINK_LIST | DCACHE_LRU_LIST);
	list_del_init(&dentry->d_lru);
	dentry->d_flags &= ~(DCACHE_SHRINK_LIST | DCACHE_LRU_LIST);
	this_cpu_dec(nr_dentry_unused);
}

static void d_shrink_add(struct dentry *dentry, struct list_head *list)
{
	D_FLAG_VERIFY(dentry, 0);
	list_add(&dentry->d_lru, list);
	dentry->d_flags |= DCACHE_SHRINK_LIST | DCACHE_LRU_LIST;
	this_cpu_inc(nr_dentry_unused);
}

/*
 * These can only be called under the global LRU lock, ie during the
 * callback for freeing the LRU list. "isolate" removes it from the
 * LRU lists entirely, while shrink_move moves it to the indicated
 * private list.
 */
static void d_lru_isolate(struct list_lru_one *lru, struct dentry *dentry)
{
	D_FLAG_VERIFY(dentry, DCACHE_LRU_LIST);
	dentry->d_flags &= ~DCACHE_LRU_LIST;
	this_cpu_dec(nr_dentry_unused);
	if (d_is_negative(dentry))
		this_cpu_dec(nr_dentry_negative);
	list_lru_isolate(lru, &dentry->d_lru);
}

static void d_lru_shrink_move(struct list_lru_one *lru, struct dentry *dentry,
			      struct list_head *list)
{
	D_FLAG_VERIFY(dentry, DCACHE_LRU_LIST);
	dentry->d_flags |= DCACHE_SHRINK_LIST;
	if (d_is_negative(dentry))
		this_cpu_dec(nr_dentry_negative);
	list_lru_isolate_move(lru, &dentry->d_lru, list);
}

static void ___d_drop(struct dentry *dentry)
{
	struct hlist_bl_head *b;
	/*
	 * Hashed dentries are normally on the dentry hashtable,
	 * with the exception of those newly allocated by
	 * d_obtain_root, which are always IS_ROOT:
	 */
	if (unlikely(IS_ROOT(dentry)))
		b = &dentry->d_sb->s_roots;
	else
		b = d_hash(dentry->d_name.hash);

	hlist_bl_lock(b);
	__hlist_bl_del(&dentry->d_hash);
	hlist_bl_unlock(b);
}

void __d_drop(struct dentry *dentry)
{
	if (!d_unhashed(dentry)) {
		___d_drop(dentry);
		dentry->d_hash.pprev = NULL;
		write_seqcount_invalidate(&dentry->d_seq);
	}
}
EXPORT_SYMBOL(__d_drop);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * d_drop - drop a dentry
 * @dentry: dentry to drop
 *
 * d_drop() unhashes the entry from the parent dentry hashes, so that it won't
 * be found through a VFS lookup any more. Note that this is different from
 * deleting the dentry - d_delete will try to mark the dentry negative if
 * possible, giving a successful _negative_ lookup, while d_drop will
 * just make the cache lookup fail.
 *
 * d_drop() is used mainly for stuff that wants to invalidate a dentry for some
 * reason (NFS timeouts or autofs deletes).
 *
<<<<<<< HEAD
 * __d_drop requires dentry->d_lock.
 */
void __d_drop(struct dentry *dentry)
{
	if (!d_unhashed(dentry)) {
		__d_shrink(dentry);
		dentry_rcuwalk_barrier(dentry);
	}
}
EXPORT_SYMBOL(__d_drop);

=======
 * __d_drop requires dentry->d_lock
 *
 * ___d_drop doesn't mark dentry as "unhashed"
 * (dentry->d_hash.pprev will be LIST_POISON2, not NULL).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void d_drop(struct dentry *dentry)
{
	spin_lock(&dentry->d_lock);
	__d_drop(dentry);
	spin_unlock(&dentry->d_lock);
}
EXPORT_SYMBOL(d_drop);

<<<<<<< HEAD
/*
 * d_clear_need_lookup - drop a dentry from cache and clear the need lookup flag
 * @dentry: dentry to drop
 *
 * This is called when we do a lookup on a placeholder dentry that needed to be
 * looked up.  The dentry should have been hashed in order for it to be found by
 * the lookup code, but now needs to be unhashed while we do the actual lookup
 * and clear the DCACHE_NEED_LOOKUP flag.
 */
void d_clear_need_lookup(struct dentry *dentry)
{
	spin_lock(&dentry->d_lock);
	__d_drop(dentry);
	dentry->d_flags &= ~DCACHE_NEED_LOOKUP;
	spin_unlock(&dentry->d_lock);
}
EXPORT_SYMBOL(d_clear_need_lookup);

/*
 * Finish off a dentry we've decided to kill.
 * dentry->d_lock must be held, returns with it unlocked.
 * If ref is non-zero, then decrement the refcount too.
 * Returns dentry requiring refcount drop, or NULL if we're done.
 */
static inline struct dentry *dentry_kill(struct dentry *dentry, int ref)
	__releases(dentry->d_lock)
{
	struct inode *inode;
	struct dentry *parent;

	inode = dentry->d_inode;
	if (inode && !spin_trylock(&inode->i_lock)) {
relock:
		spin_unlock(&dentry->d_lock);
		cpu_relax();
		return dentry; /* try again with same dentry */
	}
	if (IS_ROOT(dentry))
		parent = NULL;
	else
		parent = dentry->d_parent;
	if (parent && !spin_trylock(&parent->d_lock)) {
		if (inode)
			spin_unlock(&inode->i_lock);
		goto relock;
	}

	if (ref)
		dentry->d_count--;
	/*
	 * if dentry was on the d_lru list delete it from there.
	 * inform the fs via d_prune that this dentry is about to be
	 * unhashed and destroyed.
	 */
	dentry_lru_prune(dentry);
	/* if it was on the hash then remove it */
	__d_drop(dentry);
	return d_kill(dentry, parent);
}

=======
static inline void dentry_unlist(struct dentry *dentry)
{
	struct dentry *next;
	/*
	 * Inform d_walk() and shrink_dentry_list() that we are no longer
	 * attached to the dentry tree
	 */
	dentry->d_flags |= DCACHE_DENTRY_KILLED;
	if (unlikely(hlist_unhashed(&dentry->d_sib)))
		return;
	__hlist_del(&dentry->d_sib);
	/*
	 * Cursors can move around the list of children.  While we'd been
	 * a normal list member, it didn't matter - ->d_sib.next would've
	 * been updated.  However, from now on it won't be and for the
	 * things like d_walk() it might end up with a nasty surprise.
	 * Normally d_walk() doesn't care about cursors moving around -
	 * ->d_lock on parent prevents that and since a cursor has no children
	 * of its own, we get through it without ever unlocking the parent.
	 * There is one exception, though - if we ascend from a child that
	 * gets killed as soon as we unlock it, the next sibling is found
	 * using the value left in its ->d_sib.next.  And if _that_
	 * pointed to a cursor, and cursor got moved (e.g. by lseek())
	 * before d_walk() regains parent->d_lock, we'll end up skipping
	 * everything the cursor had been moved past.
	 *
	 * Solution: make sure that the pointer left behind in ->d_sib.next
	 * points to something that won't be moving around.  I.e. skip the
	 * cursors.
	 */
	while (dentry->d_sib.next) {
		next = hlist_entry(dentry->d_sib.next, struct dentry, d_sib);
		if (likely(!(next->d_flags & DCACHE_DENTRY_CURSOR)))
			break;
		dentry->d_sib.next = next->d_sib.next;
	}
}

static struct dentry *__dentry_kill(struct dentry *dentry)
{
	struct dentry *parent = NULL;
	bool can_free = true;

	/*
	 * The dentry is now unrecoverably dead to the world.
	 */
	lockref_mark_dead(&dentry->d_lockref);

	/*
	 * inform the fs via d_prune that this dentry is about to be
	 * unhashed and destroyed.
	 */
	if (dentry->d_flags & DCACHE_OP_PRUNE)
		dentry->d_op->d_prune(dentry);

	if (dentry->d_flags & DCACHE_LRU_LIST) {
		if (!(dentry->d_flags & DCACHE_SHRINK_LIST))
			d_lru_del(dentry);
	}
	/* if it was on the hash then remove it */
	__d_drop(dentry);
	if (dentry->d_inode)
		dentry_unlink_inode(dentry);
	else
		spin_unlock(&dentry->d_lock);
	this_cpu_dec(nr_dentry);
	if (dentry->d_op && dentry->d_op->d_release)
		dentry->d_op->d_release(dentry);

	cond_resched();
	/* now that it's negative, ->d_parent is stable */
	if (!IS_ROOT(dentry)) {
		parent = dentry->d_parent;
		spin_lock(&parent->d_lock);
	}
	spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
	dentry_unlist(dentry);
	if (dentry->d_flags & DCACHE_SHRINK_LIST)
		can_free = false;
	spin_unlock(&dentry->d_lock);
	if (likely(can_free))
		dentry_free(dentry);
	if (parent && --parent->d_lockref.count) {
		spin_unlock(&parent->d_lock);
		return NULL;
	}
	return parent;
}

/*
 * Lock a dentry for feeding it to __dentry_kill().
 * Called under rcu_read_lock() and dentry->d_lock; the former
 * guarantees that nothing we access will be freed under us.
 * Note that dentry is *not* protected from concurrent dentry_kill(),
 * d_delete(), etc.
 *
 * Return false if dentry is busy.  Otherwise, return true and have
 * that dentry's inode locked.
 */

static bool lock_for_kill(struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;

	if (unlikely(dentry->d_lockref.count))
		return false;

	if (!inode || likely(spin_trylock(&inode->i_lock)))
		return true;

	do {
		spin_unlock(&dentry->d_lock);
		spin_lock(&inode->i_lock);
		spin_lock(&dentry->d_lock);
		if (likely(inode == dentry->d_inode))
			break;
		spin_unlock(&inode->i_lock);
		inode = dentry->d_inode;
	} while (inode);
	if (likely(!dentry->d_lockref.count))
		return true;
	if (inode)
		spin_unlock(&inode->i_lock);
	return false;
}

/*
 * Decide if dentry is worth retaining.  Usually this is called with dentry
 * locked; if not locked, we are more limited and might not be able to tell
 * without a lock.  False in this case means "punt to locked path and recheck".
 *
 * In case we aren't locked, these predicates are not "stable". However, it is
 * sufficient that at some point after we dropped the reference the dentry was
 * hashed and the flags had the proper value. Other dentry users may have
 * re-gotten a reference to the dentry and change that, but our work is done -
 * we can leave the dentry around with a zero refcount.
 */
static inline bool retain_dentry(struct dentry *dentry, bool locked)
{
	unsigned int d_flags;

	smp_rmb();
	d_flags = READ_ONCE(dentry->d_flags);

	// Unreachable? Nobody would be able to look it up, no point retaining
	if (unlikely(d_unhashed(dentry)))
		return false;

	// Same if it's disconnected
	if (unlikely(d_flags & DCACHE_DISCONNECTED))
		return false;

	// ->d_delete() might tell us not to bother, but that requires
	// ->d_lock; can't decide without it
	if (unlikely(d_flags & DCACHE_OP_DELETE)) {
		if (!locked || dentry->d_op->d_delete(dentry))
			return false;
	}

	// Explicitly told not to bother
	if (unlikely(d_flags & DCACHE_DONTCACHE))
		return false;

	// At this point it looks like we ought to keep it.  We also might
	// need to do something - put it on LRU if it wasn't there already
	// and mark it referenced if it was on LRU, but not marked yet.
	// Unfortunately, both actions require ->d_lock, so in lockless
	// case we'd have to punt rather than doing those.
	if (unlikely(!(d_flags & DCACHE_LRU_LIST))) {
		if (!locked)
			return false;
		d_lru_add(dentry);
	} else if (unlikely(!(d_flags & DCACHE_REFERENCED))) {
		if (!locked)
			return false;
		dentry->d_flags |= DCACHE_REFERENCED;
	}
	return true;
}

void d_mark_dontcache(struct inode *inode)
{
	struct dentry *de;

	spin_lock(&inode->i_lock);
	hlist_for_each_entry(de, &inode->i_dentry, d_u.d_alias) {
		spin_lock(&de->d_lock);
		de->d_flags |= DCACHE_DONTCACHE;
		spin_unlock(&de->d_lock);
	}
	inode->i_state |= I_DONTCACHE;
	spin_unlock(&inode->i_lock);
}
EXPORT_SYMBOL(d_mark_dontcache);

/*
 * Try to do a lockless dput(), and return whether that was successful.
 *
 * If unsuccessful, we return false, having already taken the dentry lock.
 * In that case refcount is guaranteed to be zero and we have already
 * decided that it's not worth keeping around.
 *
 * The caller needs to hold the RCU read lock, so that the dentry is
 * guaranteed to stay around even if the refcount goes down to zero!
 */
static inline bool fast_dput(struct dentry *dentry)
{
	int ret;

	/*
	 * try to decrement the lockref optimistically.
	 */
	ret = lockref_put_return(&dentry->d_lockref);

	/*
	 * If the lockref_put_return() failed due to the lock being held
	 * by somebody else, the fast path has failed. We will need to
	 * get the lock, and then check the count again.
	 */
	if (unlikely(ret < 0)) {
		spin_lock(&dentry->d_lock);
		if (WARN_ON_ONCE(dentry->d_lockref.count <= 0)) {
			spin_unlock(&dentry->d_lock);
			return true;
		}
		dentry->d_lockref.count--;
		goto locked;
	}

	/*
	 * If we weren't the last ref, we're done.
	 */
	if (ret)
		return true;

	/*
	 * Can we decide that decrement of refcount is all we needed without
	 * taking the lock?  There's a very common case when it's all we need -
	 * dentry looks like it ought to be retained and there's nothing else
	 * to do.
	 */
	if (retain_dentry(dentry, false))
		return true;

	/*
	 * Either not worth retaining or we can't tell without the lock.
	 * Get the lock, then.  We've already decremented the refcount to 0,
	 * but we'll need to re-check the situation after getting the lock.
	 */
	spin_lock(&dentry->d_lock);

	/*
	 * Did somebody else grab a reference to it in the meantime, and
	 * we're no longer the last user after all? Alternatively, somebody
	 * else could have killed it and marked it dead. Either way, we
	 * don't need to do anything else.
	 */
locked:
	if (dentry->d_lockref.count || retain_dentry(dentry, true)) {
		spin_unlock(&dentry->d_lock);
		return true;
	}
	return false;
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* 
 * This is dput
 *
 * This is complicated by the fact that we do not want to put
 * dentries that are no longer on any hash chain on the unused
 * list: we'd much rather just get rid of them immediately.
 *
 * However, that implies that we have to traverse the dentry
 * tree upwards to the parents which might _also_ now be
 * scheduled for deletion (it may have been only waiting for
 * its last child to go away).
 *
 * This tail recursion is done by hand as we don't want to depend
 * on the compiler to always get this right (gcc generally doesn't).
 * Real recursion would eat up our stack space.
 */

/*
 * dput - release a dentry
 * @dentry: dentry to release 
 *
 * Release a dentry. This will drop the usage count and if appropriate
 * call the dentry unlink method as well as removing it from the queues and
 * releasing its resources. If the parent dentries were scheduled for release
 * they too may now get deleted.
 */
void dput(struct dentry *dentry)
{
	if (!dentry)
		return;
<<<<<<< HEAD

repeat:
	if (dentry->d_count == 1)
		might_sleep();
	spin_lock(&dentry->d_lock);
	BUG_ON(!dentry->d_count);
	if (dentry->d_count > 1) {
		dentry->d_count--;
		spin_unlock(&dentry->d_lock);
		return;
	}

	if (dentry->d_flags & DCACHE_OP_DELETE) {
		if (dentry->d_op->d_delete(dentry))
			goto kill_it;
	}

	/* Unreachable? Get rid of it */
 	if (d_unhashed(dentry))
		goto kill_it;

	/*
	 * If this dentry needs lookup, don't set the referenced flag so that it
	 * is more likely to be cleaned up by the dcache shrinker in case of
	 * memory pressure.
	 */
	if (!d_need_lookup(dentry))
		dentry->d_flags |= DCACHE_REFERENCED;
	dentry_lru_add(dentry);

	dentry->d_count--;
	spin_unlock(&dentry->d_lock);
	return;

kill_it:
	dentry = dentry_kill(dentry, 1);
	if (dentry)
		goto repeat;
}
EXPORT_SYMBOL(dput);

/**
 * d_invalidate - invalidate a dentry
 * @dentry: dentry to invalidate
 *
 * Try to invalidate the dentry if it turns out to be
 * possible. If there are other dentries that can be
 * reached through this one we can't delete it and we
 * return -EBUSY. On success we return 0.
 *
 * no dcache lock.
 */
 
int d_invalidate(struct dentry * dentry)
{
	/*
	 * If it's already been dropped, return OK.
	 */
	spin_lock(&dentry->d_lock);
	if (d_unhashed(dentry)) {
		spin_unlock(&dentry->d_lock);
		return 0;
	}
	/*
	 * Check whether to do a partial shrink_dcache
	 * to get rid of unused child entries.
	 */
	if (!list_empty(&dentry->d_subdirs)) {
		spin_unlock(&dentry->d_lock);
		shrink_dcache_parent(dentry);
		spin_lock(&dentry->d_lock);
	}

	/*
	 * Somebody else still using it?
	 *
	 * If it's a directory, we can't drop it
	 * for fear of somebody re-populating it
	 * with children (even though dropping it
	 * would make it unreachable from the root,
	 * we might still populate it if it was a
	 * working directory or similar).
	 * We also need to leave mountpoints alone,
	 * directory or not.
	 */
	if (dentry->d_count > 1 && dentry->d_inode) {
		if (S_ISDIR(dentry->d_inode->i_mode) || d_mountpoint(dentry)) {
			spin_unlock(&dentry->d_lock);
			return -EBUSY;
		}
	}

	__d_drop(dentry);
	spin_unlock(&dentry->d_lock);
	return 0;
}
EXPORT_SYMBOL(d_invalidate);

/* This must be called with d_lock held */
static inline void __dget_dlock(struct dentry *dentry)
{
	dentry->d_count++;
}

static inline void __dget(struct dentry *dentry)
{
	spin_lock(&dentry->d_lock);
	__dget_dlock(dentry);
=======
	might_sleep();
	rcu_read_lock();
	if (likely(fast_dput(dentry))) {
		rcu_read_unlock();
		return;
	}
	while (lock_for_kill(dentry)) {
		rcu_read_unlock();
		dentry = __dentry_kill(dentry);
		if (!dentry)
			return;
		if (retain_dentry(dentry, true)) {
			spin_unlock(&dentry->d_lock);
			return;
		}
		rcu_read_lock();
	}
	rcu_read_unlock();
	spin_unlock(&dentry->d_lock);
}
EXPORT_SYMBOL(dput);

static void to_shrink_list(struct dentry *dentry, struct list_head *list)
__must_hold(&dentry->d_lock)
{
	if (!(dentry->d_flags & DCACHE_SHRINK_LIST)) {
		if (dentry->d_flags & DCACHE_LRU_LIST)
			d_lru_del(dentry);
		d_shrink_add(dentry, list);
	}
}

void dput_to_list(struct dentry *dentry, struct list_head *list)
{
	rcu_read_lock();
	if (likely(fast_dput(dentry))) {
		rcu_read_unlock();
		return;
	}
	rcu_read_unlock();
	to_shrink_list(dentry, list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&dentry->d_lock);
}

struct dentry *dget_parent(struct dentry *dentry)
{
<<<<<<< HEAD
	struct dentry *ret;
=======
	int gotref;
	struct dentry *ret;
	unsigned seq;

	/*
	 * Do optimistic parent lookup without any
	 * locking.
	 */
	rcu_read_lock();
	seq = raw_seqcount_begin(&dentry->d_seq);
	ret = READ_ONCE(dentry->d_parent);
	gotref = lockref_get_not_zero(&ret->d_lockref);
	rcu_read_unlock();
	if (likely(gotref)) {
		if (!read_seqcount_retry(&dentry->d_seq, seq))
			return ret;
		dput(ret);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

repeat:
	/*
	 * Don't need rcu_dereference because we re-check it was correct under
	 * the lock.
	 */
	rcu_read_lock();
	ret = dentry->d_parent;
	spin_lock(&ret->d_lock);
	if (unlikely(ret != dentry->d_parent)) {
		spin_unlock(&ret->d_lock);
		rcu_read_unlock();
		goto repeat;
	}
	rcu_read_unlock();
<<<<<<< HEAD
	BUG_ON(!ret->d_count);
	ret->d_count++;
=======
	BUG_ON(!ret->d_lockref.count);
	ret->d_lockref.count++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&ret->d_lock);
	return ret;
}
EXPORT_SYMBOL(dget_parent);

<<<<<<< HEAD
/**
 * d_find_alias - grab a hashed alias of inode
 * @inode: inode in question
 * @want_discon:  flag, used by d_splice_alias, to request
 *          that only a DISCONNECTED alias be returned.
 *
 * If inode has a hashed alias, or is a directory and has any alias,
 * acquire the reference to alias and return it. Otherwise return NULL.
 * Notice that if inode is a directory there can be only one alias and
 * it can be unhashed only if it has no children, or if it is the root
 * of a filesystem.
 *
 * If the inode has an IS_ROOT, DCACHE_DISCONNECTED alias, then prefer
 * any other hashed alias over that one unless @want_discon is set,
 * in which case only return an IS_ROOT, DCACHE_DISCONNECTED alias.
 */
static struct dentry *__d_find_alias(struct inode *inode, int want_discon)
{
	struct dentry *alias, *discon_alias;

again:
	discon_alias = NULL;
	list_for_each_entry(alias, &inode->i_dentry, d_u.d_alias) {
		spin_lock(&alias->d_lock);
 		if (S_ISDIR(inode->i_mode) || !d_unhashed(alias)) {
			if (IS_ROOT(alias) &&
			    (alias->d_flags & DCACHE_DISCONNECTED)) {
				discon_alias = alias;
			} else if (!want_discon) {
				__dget_dlock(alias);
				spin_unlock(&alias->d_lock);
				return alias;
			}
		}
		spin_unlock(&alias->d_lock);
	}
	if (discon_alias) {
		alias = discon_alias;
		spin_lock(&alias->d_lock);
		if (S_ISDIR(inode->i_mode) || !d_unhashed(alias)) {
			if (IS_ROOT(alias) &&
			    (alias->d_flags & DCACHE_DISCONNECTED)) {
				__dget_dlock(alias);
				spin_unlock(&alias->d_lock);
				return alias;
			}
		}
		spin_unlock(&alias->d_lock);
		goto again;
	}
	return NULL;
}

struct dentry *d_find_alias(struct inode *inode)
{
	struct dentry *de = NULL;

	if (!list_empty(&inode->i_dentry)) {
		spin_lock(&inode->i_lock);
		de = __d_find_alias(inode, 0);
		spin_unlock(&inode->i_lock);
	}
	return de;
}
EXPORT_SYMBOL(d_find_alias);

/*
 *	Try to kill dentries associated with this inode.
 * WARNING: you must own a reference to inode.
 */
void d_prune_aliases(struct inode *inode)
{
	struct dentry *dentry;
restart:
	spin_lock(&inode->i_lock);
	list_for_each_entry(dentry, &inode->i_dentry, d_u.d_alias) {
		spin_lock(&dentry->d_lock);
		if (!dentry->d_count) {
			__dget_dlock(dentry);
			__d_drop(dentry);
			spin_unlock(&dentry->d_lock);
			spin_unlock(&inode->i_lock);
			dput(dentry);
			goto restart;
		}
		spin_unlock(&dentry->d_lock);
	}
	spin_unlock(&inode->i_lock);
}
EXPORT_SYMBOL(d_prune_aliases);

/*
 * Try to throw away a dentry - free the inode, dput the parent.
 * Requires dentry->d_lock is held, and dentry->d_count == 0.
 * Releases dentry->d_lock.
 *
 * This may fail if locks cannot be acquired no problem, just try again.
 */
static void try_prune_one_dentry(struct dentry *dentry)
	__releases(dentry->d_lock)
{
	struct dentry *parent;

	parent = dentry_kill(dentry, 0);
	/*
	 * If dentry_kill returns NULL, we have nothing more to do.
	 * if it returns the same dentry, trylocks failed. In either
	 * case, just loop again.
	 *
	 * Otherwise, we need to prune ancestors too. This is necessary
	 * to prevent quadratic behavior of shrink_dcache_parent(), but
	 * is also expected to be beneficial in reducing dentry cache
	 * fragmentation.
	 */
	if (!parent)
		return;
	if (parent == dentry)
		return;

	/* Prune ancestors. */
	dentry = parent;
	while (dentry) {
		spin_lock(&dentry->d_lock);
		if (dentry->d_count > 1) {
			dentry->d_count--;
			spin_unlock(&dentry->d_lock);
			return;
		}
		dentry = dentry_kill(dentry, 1);
	}
}

static void shrink_dentry_list(struct list_head *list)
{
	struct dentry *dentry;

	rcu_read_lock();
	for (;;) {
		dentry = list_entry_rcu(list->prev, struct dentry, d_lru);
		if (&dentry->d_lru == list)
			break; /* empty */
		spin_lock(&dentry->d_lock);
		if (dentry != list_entry(list->prev, struct dentry, d_lru)) {
			spin_unlock(&dentry->d_lock);
			continue;
		}

		/*
		 * We found an inuse dentry which was not removed from
		 * the LRU because of laziness during lookup.  Do not free
		 * it - just keep it off the LRU list.
		 */
		if (dentry->d_count) {
			dentry_lru_del(dentry);
			spin_unlock(&dentry->d_lock);
			continue;
		}

		rcu_read_unlock();

		try_prune_one_dentry(dentry);

		rcu_read_lock();
	}
	rcu_read_unlock();
}

/**
 * prune_dcache_sb - shrink the dcache
 * @sb: superblock
 * @count: number of entries to try to free
 *
 * Attempt to shrink the superblock dcache LRU by @count entries. This is
 * done when we need more memory an called from the superblock shrinker
 * function.
 *
 * This function may fail to free any resources if all the dentries are in
 * use.
 */
void prune_dcache_sb(struct super_block *sb, int count)
{
	struct dentry *dentry;
	LIST_HEAD(referenced);
	LIST_HEAD(tmp);

relock:
	spin_lock(&dcache_lru_lock);
	while (!list_empty(&sb->s_dentry_lru)) {
		dentry = list_entry(sb->s_dentry_lru.prev,
				struct dentry, d_lru);
		BUG_ON(dentry->d_sb != sb);

		if (!spin_trylock(&dentry->d_lock)) {
			spin_unlock(&dcache_lru_lock);
			cpu_relax();
			goto relock;
		}

		if (dentry->d_flags & DCACHE_REFERENCED) {
			dentry->d_flags &= ~DCACHE_REFERENCED;
			list_move(&dentry->d_lru, &referenced);
			spin_unlock(&dentry->d_lock);
		} else {
			list_move_tail(&dentry->d_lru, &tmp);
			dentry->d_flags |= DCACHE_SHRINK_LIST;
			spin_unlock(&dentry->d_lock);
			if (!--count)
				break;
		}
		cond_resched_lock(&dcache_lru_lock);
	}
	if (!list_empty(&referenced))
		list_splice(&referenced, &sb->s_dentry_lru);
	spin_unlock(&dcache_lru_lock);

	shrink_dentry_list(&tmp);
}

/**
 * shrink_dcache_sb - shrink dcache for a superblock
 * @sb: superblock
 *
 * Shrink the dcache for the specified super block. This is used to free
 * the dcache before unmounting a file system.
 */
void shrink_dcache_sb(struct super_block *sb)
{
	LIST_HEAD(tmp);

	spin_lock(&dcache_lru_lock);
	while (!list_empty(&sb->s_dentry_lru)) {
		list_splice_init(&sb->s_dentry_lru, &tmp);
		spin_unlock(&dcache_lru_lock);
		shrink_dentry_list(&tmp);
		spin_lock(&dcache_lru_lock);
	}
	spin_unlock(&dcache_lru_lock);
}
EXPORT_SYMBOL(shrink_dcache_sb);

/*
 * destroy a single subtree of dentries for unmount
 * - see the comments on shrink_dcache_for_umount() for a description of the
 *   locking
 */
static void shrink_dcache_for_umount_subtree(struct dentry *dentry)
{
	struct dentry *parent;

	BUG_ON(!IS_ROOT(dentry));

	for (;;) {
		/* descend to the first leaf in the current subtree */
		while (!list_empty(&dentry->d_subdirs))
			dentry = list_entry(dentry->d_subdirs.next,
					    struct dentry, d_child);

		/* consume the dentries from this leaf up through its parents
		 * until we find one with children or run out altogether */
		do {
			struct inode *inode;

			/*
			 * remove the dentry from the lru, and inform
			 * the fs that this dentry is about to be
			 * unhashed and destroyed.
			 */
			dentry_lru_prune(dentry);
			__d_shrink(dentry);

			if (dentry->d_count != 0) {
				printk(KERN_ERR
				       "BUG: Dentry %p{i=%lx,n=%s}"
				       " still in use (%d)"
				       " [unmount of %s %s]\n",
				       dentry,
				       dentry->d_inode ?
				       dentry->d_inode->i_ino : 0UL,
				       dentry->d_name.name,
				       dentry->d_count,
				       dentry->d_sb->s_type->name,
				       dentry->d_sb->s_id);
				BUG();
			}

			if (IS_ROOT(dentry)) {
				parent = NULL;
				list_del(&dentry->d_child);
			} else {
				parent = dentry->d_parent;
				parent->d_count--;
				list_del(&dentry->d_child);
			}

			inode = dentry->d_inode;
			if (inode) {
				dentry->d_inode = NULL;
				list_del_init(&dentry->d_u.d_alias);
				if (dentry->d_op && dentry->d_op->d_iput)
					dentry->d_op->d_iput(dentry, inode);
				else
					iput(inode);
			}

			d_free(dentry);

			/* finished when we fall off the top of the tree,
			 * otherwise we ascend to the parent and move to the
			 * next sibling if there is one */
			if (!parent)
				return;
			dentry = parent;
		} while (list_empty(&dentry->d_subdirs));

		dentry = list_entry(dentry->d_subdirs.next,
				    struct dentry, d_child);
	}
}

/*
 * destroy the dentries attached to a superblock on unmounting
 * - we don't need to use dentry->d_lock because:
 *   - the superblock is detached from all mountings and open files, so the
 *     dentry trees will not be rearranged by the VFS
 *   - s_umount is write-locked, so the memory pressure shrinker will ignore
 *     any dentries belonging to this superblock that it comes across
 *   - the filesystem itself is no longer permitted to rearrange the dentries
 *     in this superblock
 */
void shrink_dcache_for_umount(struct super_block *sb)
{
	struct dentry *dentry;

	if (down_read_trylock(&sb->s_umount))
		BUG();

	dentry = sb->s_root;
	sb->s_root = NULL;
	dentry->d_count--;
	shrink_dcache_for_umount_subtree(dentry);

	while (!hlist_bl_empty(&sb->s_anon)) {
		dentry = hlist_bl_entry(hlist_bl_first(&sb->s_anon), struct dentry, d_hash);
		shrink_dcache_for_umount_subtree(dentry);
	}
}


/*
 * Search for at least 1 mount point in the dentry's subdirs.
 * We descend to the next level whenever the d_subdirs
 * list is non-empty and continue searching.
 */
 
/**
 * have_submounts - check for mounts over a dentry
 * @parent: dentry to check.
 *
 * Return true if the parent or its subdirectories contain
 * a mount point
 */
int have_submounts(struct dentry *parent)
{
	struct dentry *this_parent;
	struct list_head *next;
	unsigned seq;
	int locked = 0;

	seq = read_seqbegin(&rename_lock);
again:
	this_parent = parent;

	if (d_mountpoint(parent))
		goto positive;
	spin_lock(&this_parent->d_lock);
repeat:
	next = this_parent->d_subdirs.next;
resume:
	while (next != &this_parent->d_subdirs) {
		struct list_head *tmp = next;
		struct dentry *dentry = list_entry(tmp, struct dentry, d_child);
		next = tmp->next;

		spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
		/* Have we found a mount point ? */
		if (d_mountpoint(dentry)) {
			spin_unlock(&dentry->d_lock);
			spin_unlock(&this_parent->d_lock);
			goto positive;
		}
		if (!list_empty(&dentry->d_subdirs)) {
			spin_unlock(&this_parent->d_lock);
			spin_release(&dentry->d_lock.dep_map, 1, _RET_IP_);
			this_parent = dentry;
			spin_acquire(&this_parent->d_lock.dep_map, 0, 1, _RET_IP_);
			goto repeat;
		}
		spin_unlock(&dentry->d_lock);
	}
	/*
	 * All done at this level ... ascend and resume the search.
	 */
	rcu_read_lock();
ascend:
	if (this_parent != parent) {
		struct dentry *child = this_parent;
		this_parent = child->d_parent;

		spin_unlock(&child->d_lock);
		spin_lock(&this_parent->d_lock);

		/* might go back up the wrong parent if we have had a rename */
		if (!locked && read_seqretry(&rename_lock, seq))
			goto rename_retry;
		next = child->d_child.next;
		while (unlikely(child->d_flags & DCACHE_DENTRY_KILLED)) {
			if (next == &this_parent->d_subdirs)
				goto ascend;
			child = list_entry(next, struct dentry, d_child);
			next = next->next;
		}
		rcu_read_unlock();
		goto resume;
	}
	if (!locked && read_seqretry(&rename_lock, seq))
		goto rename_retry;
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (locked)
		write_sequnlock(&rename_lock);
	return 0; /* No mount points found in tree */
positive:
	if (!locked && read_seqretry(&rename_lock, seq))
		goto rename_retry_unlocked;
	if (locked)
		write_sequnlock(&rename_lock);
	return 1;

rename_retry:
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (locked)
		goto again;
rename_retry_unlocked:
	locked = 1;
	write_seqlock(&rename_lock);
	goto again;
}
EXPORT_SYMBOL(have_submounts);

/*
 * Search the dentry child list for the specified parent,
 * and move any unused dentries to the end of the unused
 * list for prune_dcache(). We descend to the next level
 * whenever the d_subdirs list is non-empty and continue
 * searching.
 *
 * It returns zero iff there are no unused children,
 * otherwise  it returns the number of children moved to
 * the end of the unused list. This may not be the total
 * number of unused children, because select_parent can
 * drop the lock and return early due to latency
 * constraints.
 */
static int select_parent(struct dentry *parent, struct list_head *dispose)
{
	struct dentry *this_parent;
	struct list_head *next;
	unsigned seq;
	int found = 0;
	int locked = 0;

	seq = read_seqbegin(&rename_lock);
again:
	this_parent = parent;
	spin_lock(&this_parent->d_lock);
repeat:
	next = this_parent->d_subdirs.next;
resume:
	while (next != &this_parent->d_subdirs) {
		struct list_head *tmp = next;
		struct dentry *dentry = list_entry(tmp, struct dentry, d_child);
		next = tmp->next;

		spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);

		/*
		 * move only zero ref count dentries to the dispose list.
		 *
		 * Those which are presently on the shrink list, being processed
		 * by shrink_dentry_list(), shouldn't be moved.  Otherwise the
		 * loop in shrink_dcache_parent() might not make any progress
		 * and loop forever.
		 */
		if (dentry->d_count) {
			dentry_lru_del(dentry);
		} else if (!(dentry->d_flags & DCACHE_SHRINK_LIST)) {
			dentry_lru_move_list(dentry, dispose);
			dentry->d_flags |= DCACHE_SHRINK_LIST;
			found++;
		}
		/*
		 * We can return to the caller if we have found some (this
		 * ensures forward progress). We'll be coming back to find
		 * the rest.
		 */
		if (found && need_resched()) {
			spin_unlock(&dentry->d_lock);
			rcu_read_lock();
			goto out;
		}

		/*
		 * Descend a level if the d_subdirs list is non-empty.
		 */
		if (!list_empty(&dentry->d_subdirs)) {
			spin_unlock(&this_parent->d_lock);
			spin_release(&dentry->d_lock.dep_map, 1, _RET_IP_);
			this_parent = dentry;
			spin_acquire(&this_parent->d_lock.dep_map, 0, 1, _RET_IP_);
			goto repeat;
		}

		spin_unlock(&dentry->d_lock);
	}
	/*
	 * All done at this level ... ascend and resume the search.
	 */
	rcu_read_lock();
ascend:
	if (this_parent != parent) {
		struct dentry *child = this_parent;
		this_parent = child->d_parent;

		spin_unlock(&child->d_lock);
		spin_lock(&this_parent->d_lock);

		/* might go back up the wrong parent if we have had a rename */
		if (!locked && read_seqretry(&rename_lock, seq))
			goto rename_retry;
		/* go into the first sibling still alive */
		do {
			next = child->d_child.next;
			if (next == &this_parent->d_subdirs)
				goto ascend;
			child = list_entry(next, struct dentry, d_child);
		} while (unlikely(child->d_flags & DCACHE_DENTRY_KILLED));
		rcu_read_unlock();
		goto resume;
	}
out:
	if (!locked && read_seqretry(&rename_lock, seq))
		goto rename_retry;
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (locked)
		write_sequnlock(&rename_lock);
	return found;

rename_retry:
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (found)
		return found;
	if (locked)
		goto again;
	locked = 1;
	write_seqlock(&rename_lock);
	goto again;
}

/**
 * shrink_dcache_parent - prune dcache
 * @parent: parent of entries to prune
 *
 * Prune the dcache to remove unused children of the parent dentry.
 */
void shrink_dcache_parent(struct dentry * parent)
{
	LIST_HEAD(dispose);
	int found;

	while ((found = select_parent(parent, &dispose)) != 0) {
		shrink_dentry_list(&dispose);
		cond_resched();
	}
}
EXPORT_SYMBOL(shrink_dcache_parent);

/**
 * __d_alloc	-	allocate a dcache entry
 * @sb: filesystem it will belong to
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
 
struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
{
	struct dentry *dentry;
	char *dname;

	dentry = kmem_cache_alloc(dentry_cache, GFP_KERNEL);
	if (!dentry)
		return NULL;

	/*
	 * We guarantee that the inline name is always NUL-terminated.
	 * This way the memcpy() done by the name switching in rename
	 * will still always have a NUL at the end, even if we might
	 * be overwriting an internal NUL character
	 */
	dentry->d_iname[DNAME_INLINE_LEN-1] = 0;
	if (name->len > DNAME_INLINE_LEN-1) {
		dname = kmalloc(name->len + 1, GFP_KERNEL);
		if (!dname) {
			kmem_cache_free(dentry_cache, dentry); 
			return NULL;
		}
	} else  {
		dname = dentry->d_iname;
	}	

	dentry->d_name.len = name->len;
	dentry->d_name.hash = name->hash;
	memcpy(dname, name->name, name->len);
	dname[name->len] = 0;

	/* Make sure we always see the terminating NUL character */
	smp_wmb();
	dentry->d_name.name = dname;

	dentry->d_count = 1;
	dentry->d_flags = 0;
	spin_lock_init(&dentry->d_lock);
	seqcount_init(&dentry->d_seq);
	dentry->d_inode = NULL;
	dentry->d_parent = dentry;
	dentry->d_sb = sb;
	dentry->d_op = NULL;
	dentry->d_fsdata = NULL;
	INIT_HLIST_BL_NODE(&dentry->d_hash);
	INIT_LIST_HEAD(&dentry->d_lru);
	INIT_LIST_HEAD(&dentry->d_subdirs);
	INIT_LIST_HEAD(&dentry->d_u.d_alias);
	INIT_LIST_HEAD(&dentry->d_child);
	d_set_d_op(dentry, dentry->d_sb->s_d_op);

	this_cpu_inc(nr_dentry);

	return dentry;
}

/**
 * d_alloc	-	allocate a dcache entry
 * @parent: parent of entry to allocate
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
{
	struct dentry *dentry = __d_alloc(parent->d_sb, name);
	if (!dentry)
		return NULL;

	spin_lock(&parent->d_lock);
	/*
	 * don't need child lock because it is not subject
	 * to concurrency here
	 */
	__dget_dlock(parent);
	dentry->d_parent = parent;
	list_add(&dentry->d_child, &parent->d_subdirs);
	spin_unlock(&parent->d_lock);

	return dentry;
}
EXPORT_SYMBOL(d_alloc);

struct dentry *d_alloc_pseudo(struct super_block *sb, const struct qstr *name)
{
	struct dentry *dentry = __d_alloc(sb, name);
	if (dentry)
		dentry->d_flags |= DCACHE_DISCONNECTED;
	return dentry;
}
EXPORT_SYMBOL(d_alloc_pseudo);

struct dentry *d_alloc_name(struct dentry *parent, const char *name)
{
	struct qstr q;

	q.name = name;
	q.len = strlen(name);
	q.hash = full_name_hash(q.name, q.len);
	return d_alloc(parent, &q);
}
EXPORT_SYMBOL(d_alloc_name);

void d_set_d_op(struct dentry *dentry, const struct dentry_operations *op)
{
	WARN_ON_ONCE(dentry->d_op);
	WARN_ON_ONCE(dentry->d_flags & (DCACHE_OP_HASH	|
				DCACHE_OP_COMPARE	|
				DCACHE_OP_REVALIDATE	|
				DCACHE_OP_DELETE ));
	dentry->d_op = op;
	if (!op)
		return;
	if (op->d_hash)
		dentry->d_flags |= DCACHE_OP_HASH;
	if (op->d_compare)
		dentry->d_flags |= DCACHE_OP_COMPARE;
	if (op->d_revalidate)
		dentry->d_flags |= DCACHE_OP_REVALIDATE;
	if (op->d_delete)
		dentry->d_flags |= DCACHE_OP_DELETE;
	if (op->d_prune)
		dentry->d_flags |= DCACHE_OP_PRUNE;

}
EXPORT_SYMBOL(d_set_d_op);

static void __d_instantiate(struct dentry *dentry, struct inode *inode)
{
	spin_lock(&dentry->d_lock);
	if (inode) {
		if (unlikely(IS_AUTOMOUNT(inode)))
			dentry->d_flags |= DCACHE_NEED_AUTOMOUNT;
		list_add(&dentry->d_u.d_alias, &inode->i_dentry);
	}
	dentry->d_inode = inode;
	dentry_rcuwalk_barrier(dentry);
	spin_unlock(&dentry->d_lock);
	fsnotify_d_instantiate(dentry, inode);
}

/**
 * d_instantiate - fill in inode information for a dentry
 * @entry: dentry to complete
 * @inode: inode to attach to this dentry
 *
 * Fill in inode information in the entry.
 *
 * This turns negative dentries into productive full members
 * of society.
 *
 * NOTE! This assumes that the inode count has been incremented
 * (or otherwise set) by the caller to indicate that it is now
 * in use by the dcache.
 */
 
void d_instantiate(struct dentry *entry, struct inode * inode)
{
	BUG_ON(!list_empty(&entry->d_u.d_alias));
	if (inode)
		spin_lock(&inode->i_lock);
	__d_instantiate(entry, inode);
	if (inode)
		spin_unlock(&inode->i_lock);
	security_d_instantiate(entry, inode);
}
EXPORT_SYMBOL(d_instantiate);

/**
 * d_instantiate_unique - instantiate a non-aliased dentry
 * @entry: dentry to instantiate
 * @inode: inode to attach to this dentry
 *
 * Fill in inode information in the entry. On success, it returns NULL.
 * If an unhashed alias of "entry" already exists, then we return the
 * aliased dentry instead and drop one reference to inode.
 *
 * Note that in order to avoid conflicts with rename() etc, the caller
 * had better be holding the parent directory semaphore.
 *
 * This also assumes that the inode count has been incremented
 * (or otherwise set) by the caller to indicate that it is now
 * in use by the dcache.
 */
static struct dentry *__d_instantiate_unique(struct dentry *entry,
					     struct inode *inode)
{
	struct dentry *alias;
	int len = entry->d_name.len;
	const char *name = entry->d_name.name;
	unsigned int hash = entry->d_name.hash;

	if (!inode) {
		__d_instantiate(entry, NULL);
		return NULL;
	}

	list_for_each_entry(alias, &inode->i_dentry, d_u.d_alias) {
		/*
		 * Don't need alias->d_lock here, because aliases with
		 * d_parent == entry->d_parent are not subject to name or
		 * parent changes, because the parent inode i_mutex is held.
		 */
		if (alias->d_name.hash != hash)
			continue;
		if (alias->d_parent != entry->d_parent)
			continue;
		if (alias->d_name.len != len)
			continue;
		if (dentry_cmp(alias, name, len))
			continue;
		__dget(alias);
		return alias;
	}

	__d_instantiate(entry, inode);
	return NULL;
}

struct dentry *d_instantiate_unique(struct dentry *entry, struct inode *inode)
{
	struct dentry *result;

	BUG_ON(!list_empty(&entry->d_u.d_alias));

	if (inode)
		spin_lock(&inode->i_lock);
	result = __d_instantiate_unique(entry, inode);
	if (inode)
		spin_unlock(&inode->i_lock);

	if (!result) {
		security_d_instantiate(entry, inode);
		return NULL;
	}

	BUG_ON(!d_unhashed(result));
	iput(inode);
	return result;
}

EXPORT_SYMBOL(d_instantiate_unique);

struct dentry *d_make_root(struct inode *root_inode)
{
	struct dentry *res = NULL;

	if (root_inode) {
		static const struct qstr name = QSTR_INIT("/", 1);

		res = __d_alloc(root_inode->i_sb, &name);
		if (res)
			d_instantiate(res, root_inode);
		else
			iput(root_inode);
	}
	return res;
}
EXPORT_SYMBOL(d_make_root);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct dentry * __d_find_any_alias(struct inode *inode)
{
	struct dentry *alias;

<<<<<<< HEAD
	if (list_empty(&inode->i_dentry))
		return NULL;
	alias = list_first_entry(&inode->i_dentry, struct dentry, d_u.d_alias);
	__dget(alias);
=======
	if (hlist_empty(&inode->i_dentry))
		return NULL;
	alias = hlist_entry(inode->i_dentry.first, struct dentry, d_u.d_alias);
	lockref_get(&alias->d_lockref);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return alias;
}

/**
 * d_find_any_alias - find any alias for a given inode
 * @inode: inode to find an alias for
 *
 * If any aliases exist for the given inode, take and return a
 * reference for one of them.  If no aliases exist, return %NULL.
 */
struct dentry *d_find_any_alias(struct inode *inode)
{
	struct dentry *de;

	spin_lock(&inode->i_lock);
	de = __d_find_any_alias(inode);
	spin_unlock(&inode->i_lock);
	return de;
}
EXPORT_SYMBOL(d_find_any_alias);

<<<<<<< HEAD
/**
 * d_obtain_alias - find or allocate a dentry for a given inode
=======
static struct dentry *__d_find_alias(struct inode *inode)
{
	struct dentry *alias;

	if (S_ISDIR(inode->i_mode))
		return __d_find_any_alias(inode);

	hlist_for_each_entry(alias, &inode->i_dentry, d_u.d_alias) {
		spin_lock(&alias->d_lock);
 		if (!d_unhashed(alias)) {
			dget_dlock(alias);
			spin_unlock(&alias->d_lock);
			return alias;
		}
		spin_unlock(&alias->d_lock);
	}
	return NULL;
}

/**
 * d_find_alias - grab a hashed alias of inode
 * @inode: inode in question
 *
 * If inode has a hashed alias, or is a directory and has any alias,
 * acquire the reference to alias and return it. Otherwise return NULL.
 * Notice that if inode is a directory there can be only one alias and
 * it can be unhashed only if it has no children, or if it is the root
 * of a filesystem, or if the directory was renamed and d_revalidate
 * was the first vfs operation to notice.
 *
 * If the inode has an IS_ROOT, DCACHE_DISCONNECTED alias, then prefer
 * any other hashed alias over that one.
 */
struct dentry *d_find_alias(struct inode *inode)
{
	struct dentry *de = NULL;

	if (!hlist_empty(&inode->i_dentry)) {
		spin_lock(&inode->i_lock);
		de = __d_find_alias(inode);
		spin_unlock(&inode->i_lock);
	}
	return de;
}
EXPORT_SYMBOL(d_find_alias);

/*
 *  Caller MUST be holding rcu_read_lock() and be guaranteed
 *  that inode won't get freed until rcu_read_unlock().
 */
struct dentry *d_find_alias_rcu(struct inode *inode)
{
	struct hlist_head *l = &inode->i_dentry;
	struct dentry *de = NULL;

	spin_lock(&inode->i_lock);
	// ->i_dentry and ->i_rcu are colocated, but the latter won't be
	// used without having I_FREEING set, which means no aliases left
	if (likely(!(inode->i_state & I_FREEING) && !hlist_empty(l))) {
		if (S_ISDIR(inode->i_mode)) {
			de = hlist_entry(l->first, struct dentry, d_u.d_alias);
		} else {
			hlist_for_each_entry(de, l, d_u.d_alias)
				if (!d_unhashed(de))
					break;
		}
	}
	spin_unlock(&inode->i_lock);
	return de;
}

/*
 *	Try to kill dentries associated with this inode.
 * WARNING: you must own a reference to inode.
 */
void d_prune_aliases(struct inode *inode)
{
	LIST_HEAD(dispose);
	struct dentry *dentry;

	spin_lock(&inode->i_lock);
	hlist_for_each_entry(dentry, &inode->i_dentry, d_u.d_alias) {
		spin_lock(&dentry->d_lock);
		if (!dentry->d_lockref.count)
			to_shrink_list(dentry, &dispose);
		spin_unlock(&dentry->d_lock);
	}
	spin_unlock(&inode->i_lock);
	shrink_dentry_list(&dispose);
}
EXPORT_SYMBOL(d_prune_aliases);

static inline void shrink_kill(struct dentry *victim)
{
	do {
		rcu_read_unlock();
		victim = __dentry_kill(victim);
		rcu_read_lock();
	} while (victim && lock_for_kill(victim));
	rcu_read_unlock();
	if (victim)
		spin_unlock(&victim->d_lock);
}

void shrink_dentry_list(struct list_head *list)
{
	while (!list_empty(list)) {
		struct dentry *dentry;

		dentry = list_entry(list->prev, struct dentry, d_lru);
		spin_lock(&dentry->d_lock);
		rcu_read_lock();
		if (!lock_for_kill(dentry)) {
			bool can_free;
			rcu_read_unlock();
			d_shrink_del(dentry);
			can_free = dentry->d_flags & DCACHE_DENTRY_KILLED;
			spin_unlock(&dentry->d_lock);
			if (can_free)
				dentry_free(dentry);
			continue;
		}
		d_shrink_del(dentry);
		shrink_kill(dentry);
	}
}

static enum lru_status dentry_lru_isolate(struct list_head *item,
		struct list_lru_one *lru, spinlock_t *lru_lock, void *arg)
{
	struct list_head *freeable = arg;
	struct dentry	*dentry = container_of(item, struct dentry, d_lru);


	/*
	 * we are inverting the lru lock/dentry->d_lock here,
	 * so use a trylock. If we fail to get the lock, just skip
	 * it
	 */
	if (!spin_trylock(&dentry->d_lock))
		return LRU_SKIP;

	/*
	 * Referenced dentries are still in use. If they have active
	 * counts, just remove them from the LRU. Otherwise give them
	 * another pass through the LRU.
	 */
	if (dentry->d_lockref.count) {
		d_lru_isolate(lru, dentry);
		spin_unlock(&dentry->d_lock);
		return LRU_REMOVED;
	}

	if (dentry->d_flags & DCACHE_REFERENCED) {
		dentry->d_flags &= ~DCACHE_REFERENCED;
		spin_unlock(&dentry->d_lock);

		/*
		 * The list move itself will be made by the common LRU code. At
		 * this point, we've dropped the dentry->d_lock but keep the
		 * lru lock. This is safe to do, since every list movement is
		 * protected by the lru lock even if both locks are held.
		 *
		 * This is guaranteed by the fact that all LRU management
		 * functions are intermediated by the LRU API calls like
		 * list_lru_add_obj and list_lru_del_obj. List movement in this file
		 * only ever occur through this functions or through callbacks
		 * like this one, that are called from the LRU API.
		 *
		 * The only exceptions to this are functions like
		 * shrink_dentry_list, and code that first checks for the
		 * DCACHE_SHRINK_LIST flag.  Those are guaranteed to be
		 * operating only with stack provided lists after they are
		 * properly isolated from the main list.  It is thus, always a
		 * local access.
		 */
		return LRU_ROTATE;
	}

	d_lru_shrink_move(lru, dentry, freeable);
	spin_unlock(&dentry->d_lock);

	return LRU_REMOVED;
}

/**
 * prune_dcache_sb - shrink the dcache
 * @sb: superblock
 * @sc: shrink control, passed to list_lru_shrink_walk()
 *
 * Attempt to shrink the superblock dcache LRU by @sc->nr_to_scan entries. This
 * is done when we need more memory and called from the superblock shrinker
 * function.
 *
 * This function may fail to free any resources if all the dentries are in
 * use.
 */
long prune_dcache_sb(struct super_block *sb, struct shrink_control *sc)
{
	LIST_HEAD(dispose);
	long freed;

	freed = list_lru_shrink_walk(&sb->s_dentry_lru, sc,
				     dentry_lru_isolate, &dispose);
	shrink_dentry_list(&dispose);
	return freed;
}

static enum lru_status dentry_lru_isolate_shrink(struct list_head *item,
		struct list_lru_one *lru, spinlock_t *lru_lock, void *arg)
{
	struct list_head *freeable = arg;
	struct dentry	*dentry = container_of(item, struct dentry, d_lru);

	/*
	 * we are inverting the lru lock/dentry->d_lock here,
	 * so use a trylock. If we fail to get the lock, just skip
	 * it
	 */
	if (!spin_trylock(&dentry->d_lock))
		return LRU_SKIP;

	d_lru_shrink_move(lru, dentry, freeable);
	spin_unlock(&dentry->d_lock);

	return LRU_REMOVED;
}


/**
 * shrink_dcache_sb - shrink dcache for a superblock
 * @sb: superblock
 *
 * Shrink the dcache for the specified super block. This is used to free
 * the dcache before unmounting a file system.
 */
void shrink_dcache_sb(struct super_block *sb)
{
	do {
		LIST_HEAD(dispose);

		list_lru_walk(&sb->s_dentry_lru,
			dentry_lru_isolate_shrink, &dispose, 1024);
		shrink_dentry_list(&dispose);
	} while (list_lru_count(&sb->s_dentry_lru) > 0);
}
EXPORT_SYMBOL(shrink_dcache_sb);

/**
 * enum d_walk_ret - action to talke during tree walk
 * @D_WALK_CONTINUE:	contrinue walk
 * @D_WALK_QUIT:	quit walk
 * @D_WALK_NORETRY:	quit when retry is needed
 * @D_WALK_SKIP:	skip this dentry and its children
 */
enum d_walk_ret {
	D_WALK_CONTINUE,
	D_WALK_QUIT,
	D_WALK_NORETRY,
	D_WALK_SKIP,
};

/**
 * d_walk - walk the dentry tree
 * @parent:	start of walk
 * @data:	data passed to @enter() and @finish()
 * @enter:	callback when first entering the dentry
 *
 * The @enter() callbacks are called with d_lock held.
 */
static void d_walk(struct dentry *parent, void *data,
		   enum d_walk_ret (*enter)(void *, struct dentry *))
{
	struct dentry *this_parent, *dentry;
	unsigned seq = 0;
	enum d_walk_ret ret;
	bool retry = true;

again:
	read_seqbegin_or_lock(&rename_lock, &seq);
	this_parent = parent;
	spin_lock(&this_parent->d_lock);

	ret = enter(data, this_parent);
	switch (ret) {
	case D_WALK_CONTINUE:
		break;
	case D_WALK_QUIT:
	case D_WALK_SKIP:
		goto out_unlock;
	case D_WALK_NORETRY:
		retry = false;
		break;
	}
repeat:
	dentry = d_first_child(this_parent);
resume:
	hlist_for_each_entry_from(dentry, d_sib) {
		if (unlikely(dentry->d_flags & DCACHE_DENTRY_CURSOR))
			continue;

		spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);

		ret = enter(data, dentry);
		switch (ret) {
		case D_WALK_CONTINUE:
			break;
		case D_WALK_QUIT:
			spin_unlock(&dentry->d_lock);
			goto out_unlock;
		case D_WALK_NORETRY:
			retry = false;
			break;
		case D_WALK_SKIP:
			spin_unlock(&dentry->d_lock);
			continue;
		}

		if (!hlist_empty(&dentry->d_children)) {
			spin_unlock(&this_parent->d_lock);
			spin_release(&dentry->d_lock.dep_map, _RET_IP_);
			this_parent = dentry;
			spin_acquire(&this_parent->d_lock.dep_map, 0, 1, _RET_IP_);
			goto repeat;
		}
		spin_unlock(&dentry->d_lock);
	}
	/*
	 * All done at this level ... ascend and resume the search.
	 */
	rcu_read_lock();
ascend:
	if (this_parent != parent) {
		dentry = this_parent;
		this_parent = dentry->d_parent;

		spin_unlock(&dentry->d_lock);
		spin_lock(&this_parent->d_lock);

		/* might go back up the wrong parent if we have had a rename. */
		if (need_seqretry(&rename_lock, seq))
			goto rename_retry;
		/* go into the first sibling still alive */
		hlist_for_each_entry_continue(dentry, d_sib) {
			if (likely(!(dentry->d_flags & DCACHE_DENTRY_KILLED))) {
				rcu_read_unlock();
				goto resume;
			}
		}
		goto ascend;
	}
	if (need_seqretry(&rename_lock, seq))
		goto rename_retry;
	rcu_read_unlock();

out_unlock:
	spin_unlock(&this_parent->d_lock);
	done_seqretry(&rename_lock, seq);
	return;

rename_retry:
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	BUG_ON(seq & 1);
	if (!retry)
		return;
	seq = 1;
	goto again;
}

struct check_mount {
	struct vfsmount *mnt;
	unsigned int mounted;
};

static enum d_walk_ret path_check_mount(void *data, struct dentry *dentry)
{
	struct check_mount *info = data;
	struct path path = { .mnt = info->mnt, .dentry = dentry };

	if (likely(!d_mountpoint(dentry)))
		return D_WALK_CONTINUE;
	if (__path_is_mountpoint(&path)) {
		info->mounted = 1;
		return D_WALK_QUIT;
	}
	return D_WALK_CONTINUE;
}

/**
 * path_has_submounts - check for mounts over a dentry in the
 *                      current namespace.
 * @parent: path to check.
 *
 * Return true if the parent or its subdirectories contain
 * a mount point in the current namespace.
 */
int path_has_submounts(const struct path *parent)
{
	struct check_mount data = { .mnt = parent->mnt, .mounted = 0 };

	read_seqlock_excl(&mount_lock);
	d_walk(parent->dentry, &data, path_check_mount);
	read_sequnlock_excl(&mount_lock);

	return data.mounted;
}
EXPORT_SYMBOL(path_has_submounts);

/*
 * Called by mount code to set a mountpoint and check if the mountpoint is
 * reachable (e.g. NFS can unhash a directory dentry and then the complete
 * subtree can become unreachable).
 *
 * Only one of d_invalidate() and d_set_mounted() must succeed.  For
 * this reason take rename_lock and d_lock on dentry and ancestors.
 */
int d_set_mounted(struct dentry *dentry)
{
	struct dentry *p;
	int ret = -ENOENT;
	write_seqlock(&rename_lock);
	for (p = dentry->d_parent; !IS_ROOT(p); p = p->d_parent) {
		/* Need exclusion wrt. d_invalidate() */
		spin_lock(&p->d_lock);
		if (unlikely(d_unhashed(p))) {
			spin_unlock(&p->d_lock);
			goto out;
		}
		spin_unlock(&p->d_lock);
	}
	spin_lock(&dentry->d_lock);
	if (!d_unlinked(dentry)) {
		ret = -EBUSY;
		if (!d_mountpoint(dentry)) {
			dentry->d_flags |= DCACHE_MOUNTED;
			ret = 0;
		}
	}
 	spin_unlock(&dentry->d_lock);
out:
	write_sequnlock(&rename_lock);
	return ret;
}

/*
 * Search the dentry child list of the specified parent,
 * and move any unused dentries to the end of the unused
 * list for prune_dcache(). We descend to the next level
 * whenever the d_children list is non-empty and continue
 * searching.
 *
 * It returns zero iff there are no unused children,
 * otherwise  it returns the number of children moved to
 * the end of the unused list. This may not be the total
 * number of unused children, because select_parent can
 * drop the lock and return early due to latency
 * constraints.
 */

struct select_data {
	struct dentry *start;
	union {
		long found;
		struct dentry *victim;
	};
	struct list_head dispose;
};

static enum d_walk_ret select_collect(void *_data, struct dentry *dentry)
{
	struct select_data *data = _data;
	enum d_walk_ret ret = D_WALK_CONTINUE;

	if (data->start == dentry)
		goto out;

	if (dentry->d_flags & DCACHE_SHRINK_LIST) {
		data->found++;
	} else if (!dentry->d_lockref.count) {
		to_shrink_list(dentry, &data->dispose);
		data->found++;
	} else if (dentry->d_lockref.count < 0) {
		data->found++;
	}
	/*
	 * We can return to the caller if we have found some (this
	 * ensures forward progress). We'll be coming back to find
	 * the rest.
	 */
	if (!list_empty(&data->dispose))
		ret = need_resched() ? D_WALK_QUIT : D_WALK_NORETRY;
out:
	return ret;
}

static enum d_walk_ret select_collect2(void *_data, struct dentry *dentry)
{
	struct select_data *data = _data;
	enum d_walk_ret ret = D_WALK_CONTINUE;

	if (data->start == dentry)
		goto out;

	if (!dentry->d_lockref.count) {
		if (dentry->d_flags & DCACHE_SHRINK_LIST) {
			rcu_read_lock();
			data->victim = dentry;
			return D_WALK_QUIT;
		}
		to_shrink_list(dentry, &data->dispose);
	}
	/*
	 * We can return to the caller if we have found some (this
	 * ensures forward progress). We'll be coming back to find
	 * the rest.
	 */
	if (!list_empty(&data->dispose))
		ret = need_resched() ? D_WALK_QUIT : D_WALK_NORETRY;
out:
	return ret;
}

/**
 * shrink_dcache_parent - prune dcache
 * @parent: parent of entries to prune
 *
 * Prune the dcache to remove unused children of the parent dentry.
 */
void shrink_dcache_parent(struct dentry *parent)
{
	for (;;) {
		struct select_data data = {.start = parent};

		INIT_LIST_HEAD(&data.dispose);
		d_walk(parent, &data, select_collect);

		if (!list_empty(&data.dispose)) {
			shrink_dentry_list(&data.dispose);
			continue;
		}

		cond_resched();
		if (!data.found)
			break;
		data.victim = NULL;
		d_walk(parent, &data, select_collect2);
		if (data.victim) {
			spin_lock(&data.victim->d_lock);
			if (!lock_for_kill(data.victim)) {
				spin_unlock(&data.victim->d_lock);
				rcu_read_unlock();
			} else {
				shrink_kill(data.victim);
			}
		}
		if (!list_empty(&data.dispose))
			shrink_dentry_list(&data.dispose);
	}
}
EXPORT_SYMBOL(shrink_dcache_parent);

static enum d_walk_ret umount_check(void *_data, struct dentry *dentry)
{
	/* it has busy descendents; complain about those instead */
	if (!hlist_empty(&dentry->d_children))
		return D_WALK_CONTINUE;

	/* root with refcount 1 is fine */
	if (dentry == _data && dentry->d_lockref.count == 1)
		return D_WALK_CONTINUE;

	WARN(1, "BUG: Dentry %p{i=%lx,n=%pd} "
			" still in use (%d) [unmount of %s %s]\n",
		       dentry,
		       dentry->d_inode ?
		       dentry->d_inode->i_ino : 0UL,
		       dentry,
		       dentry->d_lockref.count,
		       dentry->d_sb->s_type->name,
		       dentry->d_sb->s_id);
	return D_WALK_CONTINUE;
}

static void do_one_tree(struct dentry *dentry)
{
	shrink_dcache_parent(dentry);
	d_walk(dentry, dentry, umount_check);
	d_drop(dentry);
	dput(dentry);
}

/*
 * destroy the dentries attached to a superblock on unmounting
 */
void shrink_dcache_for_umount(struct super_block *sb)
{
	struct dentry *dentry;

	WARN(down_read_trylock(&sb->s_umount), "s_umount should've been locked");

	dentry = sb->s_root;
	sb->s_root = NULL;
	do_one_tree(dentry);

	while (!hlist_bl_empty(&sb->s_roots)) {
		dentry = dget(hlist_bl_entry(hlist_bl_first(&sb->s_roots), struct dentry, d_hash));
		do_one_tree(dentry);
	}
}

static enum d_walk_ret find_submount(void *_data, struct dentry *dentry)
{
	struct dentry **victim = _data;
	if (d_mountpoint(dentry)) {
		*victim = dget_dlock(dentry);
		return D_WALK_QUIT;
	}
	return D_WALK_CONTINUE;
}

/**
 * d_invalidate - detach submounts, prune dcache, and drop
 * @dentry: dentry to invalidate (aka detach, prune and drop)
 */
void d_invalidate(struct dentry *dentry)
{
	bool had_submounts = false;
	spin_lock(&dentry->d_lock);
	if (d_unhashed(dentry)) {
		spin_unlock(&dentry->d_lock);
		return;
	}
	__d_drop(dentry);
	spin_unlock(&dentry->d_lock);

	/* Negative dentries can be dropped without further checks */
	if (!dentry->d_inode)
		return;

	shrink_dcache_parent(dentry);
	for (;;) {
		struct dentry *victim = NULL;
		d_walk(dentry, &victim, find_submount);
		if (!victim) {
			if (had_submounts)
				shrink_dcache_parent(dentry);
			return;
		}
		had_submounts = true;
		detach_mounts(victim);
		dput(victim);
	}
}
EXPORT_SYMBOL(d_invalidate);

/**
 * __d_alloc	-	allocate a dcache entry
 * @sb: filesystem it will belong to
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
 
static struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
{
	struct dentry *dentry;
	char *dname;
	int err;

	dentry = kmem_cache_alloc_lru(dentry_cache, &sb->s_dentry_lru,
				      GFP_KERNEL);
	if (!dentry)
		return NULL;

	/*
	 * We guarantee that the inline name is always NUL-terminated.
	 * This way the memcpy() done by the name switching in rename
	 * will still always have a NUL at the end, even if we might
	 * be overwriting an internal NUL character
	 */
	dentry->d_iname[DNAME_INLINE_LEN-1] = 0;
	if (unlikely(!name)) {
		name = &slash_name;
		dname = dentry->d_iname;
	} else if (name->len > DNAME_INLINE_LEN-1) {
		size_t size = offsetof(struct external_name, name[1]);
		struct external_name *p = kmalloc(size + name->len,
						  GFP_KERNEL_ACCOUNT |
						  __GFP_RECLAIMABLE);
		if (!p) {
			kmem_cache_free(dentry_cache, dentry); 
			return NULL;
		}
		atomic_set(&p->u.count, 1);
		dname = p->name;
	} else  {
		dname = dentry->d_iname;
	}	

	dentry->d_name.len = name->len;
	dentry->d_name.hash = name->hash;
	memcpy(dname, name->name, name->len);
	dname[name->len] = 0;

	/* Make sure we always see the terminating NUL character */
	smp_store_release(&dentry->d_name.name, dname); /* ^^^ */

	dentry->d_lockref.count = 1;
	dentry->d_flags = 0;
	spin_lock_init(&dentry->d_lock);
	seqcount_spinlock_init(&dentry->d_seq, &dentry->d_lock);
	dentry->d_inode = NULL;
	dentry->d_parent = dentry;
	dentry->d_sb = sb;
	dentry->d_op = NULL;
	dentry->d_fsdata = NULL;
	INIT_HLIST_BL_NODE(&dentry->d_hash);
	INIT_LIST_HEAD(&dentry->d_lru);
	INIT_HLIST_HEAD(&dentry->d_children);
	INIT_HLIST_NODE(&dentry->d_u.d_alias);
	INIT_HLIST_NODE(&dentry->d_sib);
	d_set_d_op(dentry, dentry->d_sb->s_d_op);

	if (dentry->d_op && dentry->d_op->d_init) {
		err = dentry->d_op->d_init(dentry);
		if (err) {
			if (dname_external(dentry))
				kfree(external_name(dentry));
			kmem_cache_free(dentry_cache, dentry);
			return NULL;
		}
	}

	this_cpu_inc(nr_dentry);

	return dentry;
}

/**
 * d_alloc	-	allocate a dcache entry
 * @parent: parent of entry to allocate
 * @name: qstr of the name
 *
 * Allocates a dentry. It returns %NULL if there is insufficient memory
 * available. On a success the dentry is returned. The name passed in is
 * copied and the copy passed in may be reused after this call.
 */
struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
{
	struct dentry *dentry = __d_alloc(parent->d_sb, name);
	if (!dentry)
		return NULL;
	spin_lock(&parent->d_lock);
	/*
	 * don't need child lock because it is not subject
	 * to concurrency here
	 */
	dentry->d_parent = dget_dlock(parent);
	hlist_add_head(&dentry->d_sib, &parent->d_children);
	spin_unlock(&parent->d_lock);

	return dentry;
}
EXPORT_SYMBOL(d_alloc);

struct dentry *d_alloc_anon(struct super_block *sb)
{
	return __d_alloc(sb, NULL);
}
EXPORT_SYMBOL(d_alloc_anon);

struct dentry *d_alloc_cursor(struct dentry * parent)
{
	struct dentry *dentry = d_alloc_anon(parent->d_sb);
	if (dentry) {
		dentry->d_flags |= DCACHE_DENTRY_CURSOR;
		dentry->d_parent = dget(parent);
	}
	return dentry;
}

/**
 * d_alloc_pseudo - allocate a dentry (for lookup-less filesystems)
 * @sb: the superblock
 * @name: qstr of the name
 *
 * For a filesystem that just pins its dentries in memory and never
 * performs lookups at all, return an unhashed IS_ROOT dentry.
 * This is used for pipes, sockets et.al. - the stuff that should
 * never be anyone's children or parents.  Unlike all other
 * dentries, these will not have RCU delay between dropping the
 * last reference and freeing them.
 *
 * The only user is alloc_file_pseudo() and that's what should
 * be considered a public interface.  Don't use directly.
 */
struct dentry *d_alloc_pseudo(struct super_block *sb, const struct qstr *name)
{
	static const struct dentry_operations anon_ops = {
		.d_dname = simple_dname
	};
	struct dentry *dentry = __d_alloc(sb, name);
	if (likely(dentry)) {
		dentry->d_flags |= DCACHE_NORCU;
		if (!sb->s_d_op)
			d_set_d_op(dentry, &anon_ops);
	}
	return dentry;
}

struct dentry *d_alloc_name(struct dentry *parent, const char *name)
{
	struct qstr q;

	q.name = name;
	q.hash_len = hashlen_string(parent, name);
	return d_alloc(parent, &q);
}
EXPORT_SYMBOL(d_alloc_name);

void d_set_d_op(struct dentry *dentry, const struct dentry_operations *op)
{
	WARN_ON_ONCE(dentry->d_op);
	WARN_ON_ONCE(dentry->d_flags & (DCACHE_OP_HASH	|
				DCACHE_OP_COMPARE	|
				DCACHE_OP_REVALIDATE	|
				DCACHE_OP_WEAK_REVALIDATE	|
				DCACHE_OP_DELETE	|
				DCACHE_OP_REAL));
	dentry->d_op = op;
	if (!op)
		return;
	if (op->d_hash)
		dentry->d_flags |= DCACHE_OP_HASH;
	if (op->d_compare)
		dentry->d_flags |= DCACHE_OP_COMPARE;
	if (op->d_revalidate)
		dentry->d_flags |= DCACHE_OP_REVALIDATE;
	if (op->d_weak_revalidate)
		dentry->d_flags |= DCACHE_OP_WEAK_REVALIDATE;
	if (op->d_delete)
		dentry->d_flags |= DCACHE_OP_DELETE;
	if (op->d_prune)
		dentry->d_flags |= DCACHE_OP_PRUNE;
	if (op->d_real)
		dentry->d_flags |= DCACHE_OP_REAL;

}
EXPORT_SYMBOL(d_set_d_op);

static unsigned d_flags_for_inode(struct inode *inode)
{
	unsigned add_flags = DCACHE_REGULAR_TYPE;

	if (!inode)
		return DCACHE_MISS_TYPE;

	if (S_ISDIR(inode->i_mode)) {
		add_flags = DCACHE_DIRECTORY_TYPE;
		if (unlikely(!(inode->i_opflags & IOP_LOOKUP))) {
			if (unlikely(!inode->i_op->lookup))
				add_flags = DCACHE_AUTODIR_TYPE;
			else
				inode->i_opflags |= IOP_LOOKUP;
		}
		goto type_determined;
	}

	if (unlikely(!(inode->i_opflags & IOP_NOFOLLOW))) {
		if (unlikely(inode->i_op->get_link)) {
			add_flags = DCACHE_SYMLINK_TYPE;
			goto type_determined;
		}
		inode->i_opflags |= IOP_NOFOLLOW;
	}

	if (unlikely(!S_ISREG(inode->i_mode)))
		add_flags = DCACHE_SPECIAL_TYPE;

type_determined:
	if (unlikely(IS_AUTOMOUNT(inode)))
		add_flags |= DCACHE_NEED_AUTOMOUNT;
	return add_flags;
}

static void __d_instantiate(struct dentry *dentry, struct inode *inode)
{
	unsigned add_flags = d_flags_for_inode(inode);
	WARN_ON(d_in_lookup(dentry));

	spin_lock(&dentry->d_lock);
	/*
	 * Decrement negative dentry count if it was in the LRU list.
	 */
	if (dentry->d_flags & DCACHE_LRU_LIST)
		this_cpu_dec(nr_dentry_negative);
	hlist_add_head(&dentry->d_u.d_alias, &inode->i_dentry);
	raw_write_seqcount_begin(&dentry->d_seq);
	__d_set_inode_and_type(dentry, inode, add_flags);
	raw_write_seqcount_end(&dentry->d_seq);
	fsnotify_update_flags(dentry);
	spin_unlock(&dentry->d_lock);
}

/**
 * d_instantiate - fill in inode information for a dentry
 * @entry: dentry to complete
 * @inode: inode to attach to this dentry
 *
 * Fill in inode information in the entry.
 *
 * This turns negative dentries into productive full members
 * of society.
 *
 * NOTE! This assumes that the inode count has been incremented
 * (or otherwise set) by the caller to indicate that it is now
 * in use by the dcache.
 */
 
void d_instantiate(struct dentry *entry, struct inode * inode)
{
	BUG_ON(!hlist_unhashed(&entry->d_u.d_alias));
	if (inode) {
		security_d_instantiate(entry, inode);
		spin_lock(&inode->i_lock);
		__d_instantiate(entry, inode);
		spin_unlock(&inode->i_lock);
	}
}
EXPORT_SYMBOL(d_instantiate);

/*
 * This should be equivalent to d_instantiate() + unlock_new_inode(),
 * with lockdep-related part of unlock_new_inode() done before
 * anything else.  Use that instead of open-coding d_instantiate()/
 * unlock_new_inode() combinations.
 */
void d_instantiate_new(struct dentry *entry, struct inode *inode)
{
	BUG_ON(!hlist_unhashed(&entry->d_u.d_alias));
	BUG_ON(!inode);
	lockdep_annotate_inode_mutex_key(inode);
	security_d_instantiate(entry, inode);
	spin_lock(&inode->i_lock);
	__d_instantiate(entry, inode);
	WARN_ON(!(inode->i_state & I_NEW));
	inode->i_state &= ~I_NEW & ~I_CREATING;
	smp_mb();
	wake_up_bit(&inode->i_state, __I_NEW);
	spin_unlock(&inode->i_lock);
}
EXPORT_SYMBOL(d_instantiate_new);

struct dentry *d_make_root(struct inode *root_inode)
{
	struct dentry *res = NULL;

	if (root_inode) {
		res = d_alloc_anon(root_inode->i_sb);
		if (res)
			d_instantiate(res, root_inode);
		else
			iput(root_inode);
	}
	return res;
}
EXPORT_SYMBOL(d_make_root);

static struct dentry *__d_obtain_alias(struct inode *inode, bool disconnected)
{
	struct super_block *sb;
	struct dentry *new, *res;

	if (!inode)
		return ERR_PTR(-ESTALE);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	sb = inode->i_sb;

	res = d_find_any_alias(inode); /* existing alias? */
	if (res)
		goto out;

	new = d_alloc_anon(sb);
	if (!new) {
		res = ERR_PTR(-ENOMEM);
		goto out;
	}

	security_d_instantiate(new, inode);
	spin_lock(&inode->i_lock);
	res = __d_find_any_alias(inode); /* recheck under lock */
	if (likely(!res)) { /* still no alias, attach a disconnected dentry */
		unsigned add_flags = d_flags_for_inode(inode);

		if (disconnected)
			add_flags |= DCACHE_DISCONNECTED;

		spin_lock(&new->d_lock);
		__d_set_inode_and_type(new, inode, add_flags);
		hlist_add_head(&new->d_u.d_alias, &inode->i_dentry);
		if (!disconnected) {
			hlist_bl_lock(&sb->s_roots);
			hlist_bl_add_head(&new->d_hash, &sb->s_roots);
			hlist_bl_unlock(&sb->s_roots);
		}
		spin_unlock(&new->d_lock);
		spin_unlock(&inode->i_lock);
		inode = NULL; /* consumed by new->d_inode */
		res = new;
	} else {
		spin_unlock(&inode->i_lock);
		dput(new);
	}

 out:
	iput(inode);
	return res;
}

/**
 * d_obtain_alias - find or allocate a DISCONNECTED dentry for a given inode
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @inode: inode to allocate the dentry for
 *
 * Obtain a dentry for an inode resulting from NFS filehandle conversion or
 * similar open by handle operations.  The returned dentry may be anonymous,
 * or may have a full name (if the inode was already in the cache).
 *
 * When called on a directory inode, we must ensure that the inode only ever
 * has one dentry.  If a dentry is found, that is returned instead of
 * allocating a new one.
 *
 * On successful return, the reference to the inode has been transferred
 * to the dentry.  In case of an error the reference on the inode is released.
 * To make it easier to use in export operations a %NULL or IS_ERR inode may
<<<<<<< HEAD
 * be passed in and will be the error will be propagate to the return value,
=======
 * be passed in and the error will be propagated to the return value,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * with a %NULL @inode replaced by ERR_PTR(-ESTALE).
 */
struct dentry *d_obtain_alias(struct inode *inode)
{
<<<<<<< HEAD
	static const struct qstr anonstring = { .name = "/", .len = 1 };
	struct dentry *tmp;
	struct dentry *res;

	if (!inode)
		return ERR_PTR(-ESTALE);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	res = d_find_any_alias(inode);
	if (res)
		goto out_iput;

	tmp = __d_alloc(inode->i_sb, &anonstring);
	if (!tmp) {
		res = ERR_PTR(-ENOMEM);
		goto out_iput;
	}

	spin_lock(&inode->i_lock);
	res = __d_find_any_alias(inode);
	if (res) {
		spin_unlock(&inode->i_lock);
		dput(tmp);
		goto out_iput;
	}

	/* attach a disconnected dentry */
	spin_lock(&tmp->d_lock);
	tmp->d_inode = inode;
	tmp->d_flags |= DCACHE_DISCONNECTED;
	list_add(&tmp->d_u.d_alias, &inode->i_dentry);
	hlist_bl_lock(&tmp->d_sb->s_anon);
	hlist_bl_add_head(&tmp->d_hash, &tmp->d_sb->s_anon);
	hlist_bl_unlock(&tmp->d_sb->s_anon);
	spin_unlock(&tmp->d_lock);
	spin_unlock(&inode->i_lock);
	security_d_instantiate(tmp, inode);

	return tmp;

 out_iput:
	if (res && !IS_ERR(res))
		security_d_instantiate(res, inode);
	iput(inode);
	return res;
=======
	return __d_obtain_alias(inode, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(d_obtain_alias);

/**
<<<<<<< HEAD
 * d_splice_alias - splice a disconnected dentry into the tree if one exists
 * @inode:  the inode which may have a disconnected dentry
 * @dentry: a negative dentry which we want to point to the inode.
 *
 * If inode is a directory and has a 'disconnected' dentry (i.e. IS_ROOT and
 * DCACHE_DISCONNECTED), then d_move that in place of the given dentry
 * and return it, else simply d_add the inode to the dentry and return NULL.
 *
 * This is needed in the lookup routine of any filesystem that is exportable
 * (via knfsd) so that we can build dcache paths to directories effectively.
 *
 * If a dentry was found and moved, then it is returned.  Otherwise NULL
 * is returned.  This matches the expected return value of ->lookup.
 *
 */
struct dentry *d_splice_alias(struct inode *inode, struct dentry *dentry)
{
	struct dentry *new = NULL;

	if (IS_ERR(inode))
		return ERR_CAST(inode);

	if (inode && S_ISDIR(inode->i_mode)) {
		spin_lock(&inode->i_lock);
		new = __d_find_alias(inode, 1);
		if (new) {
			BUG_ON(!(new->d_flags & DCACHE_DISCONNECTED));
			spin_unlock(&inode->i_lock);
			security_d_instantiate(new, inode);
			d_move(new, dentry);
			iput(inode);
		} else {
			/* already taking inode->i_lock, so d_add() by hand */
			__d_instantiate(dentry, inode);
			spin_unlock(&inode->i_lock);
			security_d_instantiate(dentry, inode);
			d_rehash(dentry);
		}
	} else
		d_add(dentry, inode);
	return new;
}
EXPORT_SYMBOL(d_splice_alias);
=======
 * d_obtain_root - find or allocate a dentry for a given inode
 * @inode: inode to allocate the dentry for
 *
 * Obtain an IS_ROOT dentry for the root of a filesystem.
 *
 * We must ensure that directory inodes only ever have one dentry.  If a
 * dentry is found, that is returned instead of allocating a new one.
 *
 * On successful return, the reference to the inode has been transferred
 * to the dentry.  In case of an error the reference on the inode is
 * released.  A %NULL or IS_ERR inode may be passed in and will be the
 * error will be propagate to the return value, with a %NULL @inode
 * replaced by ERR_PTR(-ESTALE).
 */
struct dentry *d_obtain_root(struct inode *inode)
{
	return __d_obtain_alias(inode, false);
}
EXPORT_SYMBOL(d_obtain_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * d_add_ci - lookup or allocate new dentry with case-exact name
 * @inode:  the inode case-insensitive lookup has found
 * @dentry: the negative dentry that was passed to the parent's lookup func
 * @name:   the case-exact name to be associated with the returned dentry
 *
 * This is to avoid filling the dcache with case-insensitive names to the
 * same inode, only the actual correct case is stored in the dcache for
 * case-insensitive filesystems.
 *
<<<<<<< HEAD
 * For a case-insensitive lookup match and if the the case-exact dentry
 * already exists in in the dcache, use it and return it.
=======
 * For a case-insensitive lookup match and if the case-exact dentry
 * already exists in the dcache, use it and return it.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If no entry exists with the exact case name, allocate new dentry with
 * the exact case, and return the spliced entry.
 */
struct dentry *d_add_ci(struct dentry *dentry, struct inode *inode,
			struct qstr *name)
{
<<<<<<< HEAD
	int error;
	struct dentry *found;
	struct dentry *new;
=======
	struct dentry *found, *res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * First check if a dentry matching the name already exists,
	 * if not go ahead and create it now.
	 */
	found = d_hash_and_lookup(dentry->d_parent, name);
<<<<<<< HEAD
	if (!found) {
		new = d_alloc(dentry->d_parent, name);
		if (!new) {
			error = -ENOMEM;
			goto err_out;
		}

		found = d_splice_alias(inode, new);
		if (found) {
			dput(new);
			return found;
		}
		return new;
	}

	/*
	 * If a matching dentry exists, and it's not negative use it.
	 *
	 * Decrement the reference count to balance the iget() done
	 * earlier on.
	 */
	if (found->d_inode) {
		if (unlikely(found->d_inode != inode)) {
			/* This can't happen because bad inodes are unhashed. */
			BUG_ON(!is_bad_inode(inode));
			BUG_ON(!is_bad_inode(found->d_inode));
		}
		iput(inode);
		return found;
	}

	/*
	 * We are going to instantiate this dentry, unhash it and clear the
	 * lookup flag so we can do that.
	 */
	if (unlikely(d_need_lookup(found)))
		d_clear_need_lookup(found);

	/*
	 * Negative dentry: instantiate it unless the inode is a directory and
	 * already has a dentry.
	 */
	new = d_splice_alias(inode, found);
	if (new) {
		dput(found);
		found = new;
	}
	return found;

err_out:
	iput(inode);
	return ERR_PTR(error);
}
EXPORT_SYMBOL(d_add_ci);

/*
 * Do the slow-case of the dentry name compare.
 *
 * Unlike the dentry_cmp() function, we need to atomically
 * load the name, length and inode information, so that the
 * filesystem can rely on them, and can use the 'name' and
 * 'len' information without worrying about walking off the
 * end of memory etc.
 *
 * Thus the read_seqcount_retry() and the "duplicate" info
 * in arguments (the low-level filesystem should not look
 * at the dentry inode or name contents directly, since
 * rename can change them while we're in RCU mode).
 */
enum slow_d_compare {
	D_COMP_OK,
	D_COMP_NOMATCH,
	D_COMP_SEQRETRY,
};

static noinline enum slow_d_compare slow_dentry_cmp(
		const struct dentry *parent,
		struct inode *inode,
		struct dentry *dentry,
		unsigned int seq,
		const struct qstr *name)
{
	int tlen = dentry->d_name.len;
	const char *tname = dentry->d_name.name;
	struct inode *i = dentry->d_inode;

	if (read_seqcount_retry(&dentry->d_seq, seq)) {
		cpu_relax();
		return D_COMP_SEQRETRY;
	}
	if (parent->d_op->d_compare(parent, inode,
				dentry, i,
				tlen, tname, name))
		return D_COMP_NOMATCH;
	return D_COMP_OK;
=======
	if (found) {
		iput(inode);
		return found;
	}
	if (d_in_lookup(dentry)) {
		found = d_alloc_parallel(dentry->d_parent, name,
					dentry->d_wait);
		if (IS_ERR(found) || !d_in_lookup(found)) {
			iput(inode);
			return found;
		}
	} else {
		found = d_alloc(dentry->d_parent, name);
		if (!found) {
			iput(inode);
			return ERR_PTR(-ENOMEM);
		} 
	}
	res = d_splice_alias(inode, found);
	if (res) {
		d_lookup_done(found);
		dput(found);
		return res;
	}
	return found;
}
EXPORT_SYMBOL(d_add_ci);

/**
 * d_same_name - compare dentry name with case-exact name
 * @parent: parent dentry
 * @dentry: the negative dentry that was passed to the parent's lookup func
 * @name:   the case-exact name to be associated with the returned dentry
 *
 * Return: true if names are same, or false
 */
bool d_same_name(const struct dentry *dentry, const struct dentry *parent,
		 const struct qstr *name)
{
	if (likely(!(parent->d_flags & DCACHE_OP_COMPARE))) {
		if (dentry->d_name.len != name->len)
			return false;
		return dentry_cmp(dentry, name->name, name->len) == 0;
	}
	return parent->d_op->d_compare(dentry,
				       dentry->d_name.len, dentry->d_name.name,
				       name) == 0;
}
EXPORT_SYMBOL_GPL(d_same_name);

/*
 * This is __d_lookup_rcu() when the parent dentry has
 * DCACHE_OP_COMPARE, which makes things much nastier.
 */
static noinline struct dentry *__d_lookup_rcu_op_compare(
	const struct dentry *parent,
	const struct qstr *name,
	unsigned *seqp)
{
	u64 hashlen = name->hash_len;
	struct hlist_bl_head *b = d_hash(hashlen_hash(hashlen));
	struct hlist_bl_node *node;
	struct dentry *dentry;

	hlist_bl_for_each_entry_rcu(dentry, node, b, d_hash) {
		int tlen;
		const char *tname;
		unsigned seq;

seqretry:
		seq = raw_seqcount_begin(&dentry->d_seq);
		if (dentry->d_parent != parent)
			continue;
		if (d_unhashed(dentry))
			continue;
		if (dentry->d_name.hash != hashlen_hash(hashlen))
			continue;
		tlen = dentry->d_name.len;
		tname = dentry->d_name.name;
		/* we want a consistent (name,len) pair */
		if (read_seqcount_retry(&dentry->d_seq, seq)) {
			cpu_relax();
			goto seqretry;
		}
		if (parent->d_op->d_compare(dentry, tlen, tname, name) != 0)
			continue;
		*seqp = seq;
		return dentry;
	}
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * __d_lookup_rcu - search for a dentry (racy, store-free)
 * @parent: parent dentry
 * @name: qstr of name we wish to find
 * @seqp: returns d_seq value at the point where the dentry was found
<<<<<<< HEAD
 * @inode: returns dentry->d_inode when the inode was found valid.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns: dentry, or NULL
 *
 * __d_lookup_rcu is the dcache lookup function for rcu-walk name
 * resolution (store-free path walking) design described in
 * Documentation/filesystems/path-lookup.txt.
 *
 * This is not to be used outside core vfs.
 *
 * __d_lookup_rcu must only be used in rcu-walk mode, ie. with vfsmount lock
 * held, and rcu_read_lock held. The returned dentry must not be stored into
 * without taking d_lock and checking d_seq sequence count against @seq
 * returned here.
 *
<<<<<<< HEAD
 * A refcount may be taken on the found dentry with the __d_rcu_to_refcount
=======
 * A refcount may be taken on the found dentry with the d_rcu_to_refcount
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * function.
 *
 * Alternatively, __d_lookup_rcu may be called again to look up the child of
 * the returned dentry, so long as its parent's seqlock is checked after the
 * child is looked up. Thus, an interlocking stepping of sequence lock checks
 * is formed, giving integrity down the path walk.
 *
 * NOTE! The caller *has* to check the resulting dentry against the sequence
 * number we've returned before using any of the resulting dentry state!
 */
struct dentry *__d_lookup_rcu(const struct dentry *parent,
				const struct qstr *name,
<<<<<<< HEAD
				unsigned *seqp, struct inode *inode)
{
	u64 hashlen = name->hash_len;
	const unsigned char *str = name->name;
	struct hlist_bl_head *b = d_hash(parent, hashlen_hash(hashlen));
=======
				unsigned *seqp)
{
	u64 hashlen = name->hash_len;
	const unsigned char *str = name->name;
	struct hlist_bl_head *b = d_hash(hashlen_hash(hashlen));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hlist_bl_node *node;
	struct dentry *dentry;

	/*
	 * Note: There is significant duplication with __d_lookup_rcu which is
	 * required to prevent single threaded performance regressions
	 * especially on architectures where smp_rmb (in seqcounts) are costly.
	 * Keep the two functions in sync.
	 */

<<<<<<< HEAD
=======
	if (unlikely(parent->d_flags & DCACHE_OP_COMPARE))
		return __d_lookup_rcu_op_compare(parent, name, seqp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The hash list is protected using RCU.
	 *
	 * Carefully use d_seq when comparing a candidate dentry, to avoid
	 * races with d_move().
	 *
	 * It is possible that concurrent renames can mess up our list
	 * walk here and result in missing our dentry, resulting in the
	 * false-negative result. d_lookup() protects against concurrent
	 * renames using rename_lock seqlock.
	 *
	 * See Documentation/filesystems/path-lookup.txt for more details.
	 */
	hlist_bl_for_each_entry_rcu(dentry, node, b, d_hash) {
		unsigned seq;

<<<<<<< HEAD
seqretry:
		/*
		 * The dentry sequence count protects us from concurrent
		 * renames, and thus protects inode, parent and name fields.
		 *
		 * The caller must perform a seqcount check in order
		 * to do anything useful with the returned dentry,
		 * including using the 'd_inode' pointer.
=======
		/*
		 * The dentry sequence count protects us from concurrent
		 * renames, and thus protects parent and name fields.
		 *
		 * The caller must perform a seqcount check in order
		 * to do anything useful with the returned dentry.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 *
		 * NOTE! We do a "raw" seqcount_begin here. That means that
		 * we don't wait for the sequence count to stabilize if it
		 * is in the middle of a sequence change. If we do the slow
		 * dentry compare, we will do seqretries until it is stable,
		 * and if we end up with a successful lookup, we actually
		 * want to exit RCU lookup anyway.
<<<<<<< HEAD
=======
		 *
		 * Note that raw_seqcount_begin still *does* smp_rmb(), so
		 * we are still guaranteed NUL-termination of ->d_name.name.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		seq = raw_seqcount_begin(&dentry->d_seq);
		if (dentry->d_parent != parent)
			continue;
		if (d_unhashed(dentry))
			continue;
<<<<<<< HEAD
		*seqp = seq;

		if (unlikely(parent->d_flags & DCACHE_OP_COMPARE)) {
			if (dentry->d_name.hash != hashlen_hash(hashlen))
				continue;
			switch (slow_dentry_cmp(parent, inode, dentry, seq, name)) {
			case D_COMP_OK:
				return dentry;
			case D_COMP_NOMATCH:
				continue;
			default:
				goto seqretry;
			}
		}

		if (dentry->d_name.hash_len != hashlen)
			continue;
		if (!dentry_cmp(dentry, str, hashlen_len(hashlen)))
			return dentry;
=======
		if (dentry->d_name.hash_len != hashlen)
			continue;
		if (dentry_cmp(dentry, str, hashlen_len(hashlen)) != 0)
			continue;
		*seqp = seq;
		return dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return NULL;
}

/**
 * d_lookup - search for a dentry
 * @parent: parent dentry
 * @name: qstr of name we wish to find
 * Returns: dentry, or NULL
 *
 * d_lookup searches the children of the parent dentry for the name in
 * question. If the dentry is found its reference count is incremented and the
 * dentry is returned. The caller must use dput to free the entry when it has
 * finished using it. %NULL is returned if the dentry does not exist.
 */
struct dentry *d_lookup(const struct dentry *parent, const struct qstr *name)
{
	struct dentry *dentry;
	unsigned seq;

<<<<<<< HEAD
        do {
                seq = read_seqbegin(&rename_lock);
                dentry = __d_lookup(parent, name);
                if (dentry)
=======
	do {
		seq = read_seqbegin(&rename_lock);
		dentry = __d_lookup(parent, name);
		if (dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
	} while (read_seqretry(&rename_lock, seq));
	return dentry;
}
EXPORT_SYMBOL(d_lookup);

/**
 * __d_lookup - search for a dentry (racy)
 * @parent: parent dentry
 * @name: qstr of name we wish to find
 * Returns: dentry, or NULL
 *
 * __d_lookup is like d_lookup, however it may (rarely) return a
 * false-negative result due to unrelated rename activity.
 *
 * __d_lookup is slightly faster by avoiding rename_lock read seqlock,
 * however it must be used carefully, eg. with a following d_lookup in
 * the case of failure.
 *
 * __d_lookup callers must be commented.
 */
struct dentry *__d_lookup(const struct dentry *parent, const struct qstr *name)
{
<<<<<<< HEAD
	unsigned int len = name->len;
	unsigned int hash = name->hash;
	const unsigned char *str = name->name;
	struct hlist_bl_head *b = d_hash(parent, hash);
=======
	unsigned int hash = name->hash;
	struct hlist_bl_head *b = d_hash(hash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct hlist_bl_node *node;
	struct dentry *found = NULL;
	struct dentry *dentry;

	/*
	 * Note: There is significant duplication with __d_lookup_rcu which is
	 * required to prevent single threaded performance regressions
	 * especially on architectures where smp_rmb (in seqcounts) are costly.
	 * Keep the two functions in sync.
	 */

	/*
	 * The hash list is protected using RCU.
	 *
	 * Take d_lock when comparing a candidate dentry, to avoid races
	 * with d_move().
	 *
	 * It is possible that concurrent renames can mess up our list
	 * walk here and result in missing our dentry, resulting in the
	 * false-negative result. d_lookup() protects against concurrent
	 * renames using rename_lock seqlock.
	 *
	 * See Documentation/filesystems/path-lookup.txt for more details.
	 */
	rcu_read_lock();
	
	hlist_bl_for_each_entry_rcu(dentry, node, b, d_hash) {

		if (dentry->d_name.hash != hash)
			continue;

		spin_lock(&dentry->d_lock);
		if (dentry->d_parent != parent)
			goto next;
		if (d_unhashed(dentry))
			goto next;

<<<<<<< HEAD
		/*
		 * It is safe to compare names since d_move() cannot
		 * change the qstr (protected by d_lock).
		 */
		if (parent->d_flags & DCACHE_OP_COMPARE) {
			int tlen = dentry->d_name.len;
			const char *tname = dentry->d_name.name;
			if (parent->d_op->d_compare(parent, parent->d_inode,
						dentry, dentry->d_inode,
						tlen, tname, name))
				goto next;
		} else {
			if (dentry->d_name.len != len)
				goto next;
			if (dentry_cmp(dentry, str, len))
				goto next;
		}

		dentry->d_count++;
=======
		if (!d_same_name(dentry, parent, name))
			goto next;

		dentry->d_lockref.count++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		found = dentry;
		spin_unlock(&dentry->d_lock);
		break;
next:
		spin_unlock(&dentry->d_lock);
 	}
 	rcu_read_unlock();

 	return found;
}

/**
 * d_hash_and_lookup - hash the qstr then search for a dentry
 * @dir: Directory to search in
 * @name: qstr of name we wish to find
 *
<<<<<<< HEAD
 * On hash failure or on lookup failure NULL is returned.
 */
struct dentry *d_hash_and_lookup(struct dentry *dir, struct qstr *name)
{
	struct dentry *dentry = NULL;

=======
 * On lookup failure NULL is returned; on bad name - ERR_PTR(-error)
 */
struct dentry *d_hash_and_lookup(struct dentry *dir, struct qstr *name)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check for a fs-specific hash function. Note that we must
	 * calculate the standard hash first, as the d_op->d_hash()
	 * routine may choose to leave the hash value unchanged.
	 */
<<<<<<< HEAD
	name->hash = full_name_hash(name->name, name->len);
	if (dir->d_flags & DCACHE_OP_HASH) {
		if (dir->d_op->d_hash(dir, dir->d_inode, name) < 0)
			goto out;
	}
	dentry = d_lookup(dir, name);
out:
	return dentry;
}

/**
 * d_validate - verify dentry provided from insecure source (deprecated)
 * @dentry: The dentry alleged to be valid child of @dparent
 * @dparent: The parent dentry (known to be valid)
 *
 * An insecure source has sent us a dentry, here we verify it and dget() it.
 * This is used by ncpfs in its readdir implementation.
 * Zero is returned in the dentry is invalid.
 *
 * This function is slow for big directories, and deprecated, do not use it.
 */
int d_validate(struct dentry *dentry, struct dentry *dparent)
{
	struct dentry *child;

	spin_lock(&dparent->d_lock);
	list_for_each_entry(child, &dparent->d_subdirs, d_child) {
		if (dentry == child) {
			spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
			__dget_dlock(dentry);
			spin_unlock(&dentry->d_lock);
			spin_unlock(&dparent->d_lock);
			return 1;
		}
	}
	spin_unlock(&dparent->d_lock);

	return 0;
}
EXPORT_SYMBOL(d_validate);
=======
	name->hash = full_name_hash(dir, name->name, name->len);
	if (dir->d_flags & DCACHE_OP_HASH) {
		int err = dir->d_op->d_hash(dir, name);
		if (unlikely(err < 0))
			return ERR_PTR(err);
	}
	return d_lookup(dir, name);
}
EXPORT_SYMBOL(d_hash_and_lookup);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * When a file is deleted, we have two options:
 * - turn this dentry into a negative dentry
 * - unhash this dentry and free it.
 *
 * Usually, we want to just turn this into
 * a negative dentry, but if anybody else is
 * currently using the dentry or the inode
 * we can't do that and we fall back on removing
 * it from the hash queues and waiting for
 * it to be deleted later when it has no users
 */
 
/**
 * d_delete - delete a dentry
 * @dentry: The dentry to delete
 *
 * Turn the dentry into a negative dentry if possible, otherwise
 * remove it from the hash queues so it can be deleted later
 */
 
void d_delete(struct dentry * dentry)
{
<<<<<<< HEAD
	struct inode *inode;
	int isdir = 0;
	/*
	 * Are we the only user?
	 */
again:
	spin_lock(&dentry->d_lock);
	inode = dentry->d_inode;
	isdir = S_ISDIR(inode->i_mode);
	if (dentry->d_count == 1) {
		if (inode && !spin_trylock(&inode->i_lock)) {
			spin_unlock(&dentry->d_lock);
			cpu_relax();
			goto again;
		}
		dentry->d_flags &= ~DCACHE_CANT_MOUNT;
		dentry_unlink_inode(dentry);
		fsnotify_nameremove(dentry, isdir);
		return;
	}

	if (!d_unhashed(dentry))
		__d_drop(dentry);

	spin_unlock(&dentry->d_lock);

	fsnotify_nameremove(dentry, isdir);
}
EXPORT_SYMBOL(d_delete);

static void __d_rehash(struct dentry * entry, struct hlist_bl_head *b)
{
	BUG_ON(!d_unhashed(entry));
	hlist_bl_lock(b);
	entry->d_flags |= DCACHE_RCUACCESS;
=======
	struct inode *inode = dentry->d_inode;

	spin_lock(&inode->i_lock);
	spin_lock(&dentry->d_lock);
	/*
	 * Are we the only user?
	 */
	if (dentry->d_lockref.count == 1) {
		dentry->d_flags &= ~DCACHE_CANT_MOUNT;
		dentry_unlink_inode(dentry);
	} else {
		__d_drop(dentry);
		spin_unlock(&dentry->d_lock);
		spin_unlock(&inode->i_lock);
	}
}
EXPORT_SYMBOL(d_delete);

static void __d_rehash(struct dentry *entry)
{
	struct hlist_bl_head *b = d_hash(entry->d_name.hash);

	hlist_bl_lock(b);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hlist_bl_add_head_rcu(&entry->d_hash, b);
	hlist_bl_unlock(b);
}

<<<<<<< HEAD
static void _d_rehash(struct dentry * entry)
{
	__d_rehash(entry, d_hash(entry->d_parent, entry->d_name.hash));
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * d_rehash	- add an entry back to the hash
 * @entry: dentry to add to the hash
 *
 * Adds a dentry to the hash according to its name.
 */
 
void d_rehash(struct dentry * entry)
{
	spin_lock(&entry->d_lock);
<<<<<<< HEAD
	_d_rehash(entry);
=======
	__d_rehash(entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&entry->d_lock);
}
EXPORT_SYMBOL(d_rehash);

<<<<<<< HEAD
/**
 * dentry_update_name_case - update case insensitive dentry with a new name
 * @dentry: dentry to be updated
 * @name: new name
 *
 * Update a case insensitive dentry with new case of name.
 *
 * dentry must have been returned by d_lookup with name @name. Old and new
 * name lengths must match (ie. no d_compare which allows mismatched name
 * lengths).
 *
 * Parent inode i_mutex must be held over d_lookup and into this call (to
 * keep renames and concurrent inserts, and readdir(2) away).
 */
void dentry_update_name_case(struct dentry *dentry, struct qstr *name)
{
	BUG_ON(!mutex_is_locked(&dentry->d_parent->d_inode->i_mutex));
	BUG_ON(dentry->d_name.len != name->len); /* d_lookup gives this */

	spin_lock(&dentry->d_lock);
	write_seqcount_begin(&dentry->d_seq);
	memcpy((unsigned char *)dentry->d_name.name, name->name, name->len);
	write_seqcount_end(&dentry->d_seq);
	spin_unlock(&dentry->d_lock);
}
EXPORT_SYMBOL(dentry_update_name_case);

static void switch_names(struct dentry *dentry, struct dentry *target)
{
	if (dname_external(target)) {
		if (dname_external(dentry)) {
=======
static inline unsigned start_dir_add(struct inode *dir)
{
	preempt_disable_nested();
	for (;;) {
		unsigned n = dir->i_dir_seq;
		if (!(n & 1) && cmpxchg(&dir->i_dir_seq, n, n + 1) == n)
			return n;
		cpu_relax();
	}
}

static inline void end_dir_add(struct inode *dir, unsigned int n,
			       wait_queue_head_t *d_wait)
{
	smp_store_release(&dir->i_dir_seq, n + 2);
	preempt_enable_nested();
	wake_up_all(d_wait);
}

static void d_wait_lookup(struct dentry *dentry)
{
	if (d_in_lookup(dentry)) {
		DECLARE_WAITQUEUE(wait, current);
		add_wait_queue(dentry->d_wait, &wait);
		do {
			set_current_state(TASK_UNINTERRUPTIBLE);
			spin_unlock(&dentry->d_lock);
			schedule();
			spin_lock(&dentry->d_lock);
		} while (d_in_lookup(dentry));
	}
}

struct dentry *d_alloc_parallel(struct dentry *parent,
				const struct qstr *name,
				wait_queue_head_t *wq)
{
	unsigned int hash = name->hash;
	struct hlist_bl_head *b = in_lookup_hash(parent, hash);
	struct hlist_bl_node *node;
	struct dentry *new = d_alloc(parent, name);
	struct dentry *dentry;
	unsigned seq, r_seq, d_seq;

	if (unlikely(!new))
		return ERR_PTR(-ENOMEM);

retry:
	rcu_read_lock();
	seq = smp_load_acquire(&parent->d_inode->i_dir_seq);
	r_seq = read_seqbegin(&rename_lock);
	dentry = __d_lookup_rcu(parent, name, &d_seq);
	if (unlikely(dentry)) {
		if (!lockref_get_not_dead(&dentry->d_lockref)) {
			rcu_read_unlock();
			goto retry;
		}
		if (read_seqcount_retry(&dentry->d_seq, d_seq)) {
			rcu_read_unlock();
			dput(dentry);
			goto retry;
		}
		rcu_read_unlock();
		dput(new);
		return dentry;
	}
	if (unlikely(read_seqretry(&rename_lock, r_seq))) {
		rcu_read_unlock();
		goto retry;
	}

	if (unlikely(seq & 1)) {
		rcu_read_unlock();
		goto retry;
	}

	hlist_bl_lock(b);
	if (unlikely(READ_ONCE(parent->d_inode->i_dir_seq) != seq)) {
		hlist_bl_unlock(b);
		rcu_read_unlock();
		goto retry;
	}
	/*
	 * No changes for the parent since the beginning of d_lookup().
	 * Since all removals from the chain happen with hlist_bl_lock(),
	 * any potential in-lookup matches are going to stay here until
	 * we unlock the chain.  All fields are stable in everything
	 * we encounter.
	 */
	hlist_bl_for_each_entry(dentry, node, b, d_u.d_in_lookup_hash) {
		if (dentry->d_name.hash != hash)
			continue;
		if (dentry->d_parent != parent)
			continue;
		if (!d_same_name(dentry, parent, name))
			continue;
		hlist_bl_unlock(b);
		/* now we can try to grab a reference */
		if (!lockref_get_not_dead(&dentry->d_lockref)) {
			rcu_read_unlock();
			goto retry;
		}

		rcu_read_unlock();
		/*
		 * somebody is likely to be still doing lookup for it;
		 * wait for them to finish
		 */
		spin_lock(&dentry->d_lock);
		d_wait_lookup(dentry);
		/*
		 * it's not in-lookup anymore; in principle we should repeat
		 * everything from dcache lookup, but it's likely to be what
		 * d_lookup() would've found anyway.  If it is, just return it;
		 * otherwise we really have to repeat the whole thing.
		 */
		if (unlikely(dentry->d_name.hash != hash))
			goto mismatch;
		if (unlikely(dentry->d_parent != parent))
			goto mismatch;
		if (unlikely(d_unhashed(dentry)))
			goto mismatch;
		if (unlikely(!d_same_name(dentry, parent, name)))
			goto mismatch;
		/* OK, it *is* a hashed match; return it */
		spin_unlock(&dentry->d_lock);
		dput(new);
		return dentry;
	}
	rcu_read_unlock();
	/* we can't take ->d_lock here; it's OK, though. */
	new->d_flags |= DCACHE_PAR_LOOKUP;
	new->d_wait = wq;
	hlist_bl_add_head(&new->d_u.d_in_lookup_hash, b);
	hlist_bl_unlock(b);
	return new;
mismatch:
	spin_unlock(&dentry->d_lock);
	dput(dentry);
	goto retry;
}
EXPORT_SYMBOL(d_alloc_parallel);

/*
 * - Unhash the dentry
 * - Retrieve and clear the waitqueue head in dentry
 * - Return the waitqueue head
 */
static wait_queue_head_t *__d_lookup_unhash(struct dentry *dentry)
{
	wait_queue_head_t *d_wait;
	struct hlist_bl_head *b;

	lockdep_assert_held(&dentry->d_lock);

	b = in_lookup_hash(dentry->d_parent, dentry->d_name.hash);
	hlist_bl_lock(b);
	dentry->d_flags &= ~DCACHE_PAR_LOOKUP;
	__hlist_bl_del(&dentry->d_u.d_in_lookup_hash);
	d_wait = dentry->d_wait;
	dentry->d_wait = NULL;
	hlist_bl_unlock(b);
	INIT_HLIST_NODE(&dentry->d_u.d_alias);
	INIT_LIST_HEAD(&dentry->d_lru);
	return d_wait;
}

void __d_lookup_unhash_wake(struct dentry *dentry)
{
	spin_lock(&dentry->d_lock);
	wake_up_all(__d_lookup_unhash(dentry));
	spin_unlock(&dentry->d_lock);
}
EXPORT_SYMBOL(__d_lookup_unhash_wake);

/* inode->i_lock held if inode is non-NULL */

static inline void __d_add(struct dentry *dentry, struct inode *inode)
{
	wait_queue_head_t *d_wait;
	struct inode *dir = NULL;
	unsigned n;
	spin_lock(&dentry->d_lock);
	if (unlikely(d_in_lookup(dentry))) {
		dir = dentry->d_parent->d_inode;
		n = start_dir_add(dir);
		d_wait = __d_lookup_unhash(dentry);
	}
	if (inode) {
		unsigned add_flags = d_flags_for_inode(inode);
		hlist_add_head(&dentry->d_u.d_alias, &inode->i_dentry);
		raw_write_seqcount_begin(&dentry->d_seq);
		__d_set_inode_and_type(dentry, inode, add_flags);
		raw_write_seqcount_end(&dentry->d_seq);
		fsnotify_update_flags(dentry);
	}
	__d_rehash(dentry);
	if (dir)
		end_dir_add(dir, n, d_wait);
	spin_unlock(&dentry->d_lock);
	if (inode)
		spin_unlock(&inode->i_lock);
}

/**
 * d_add - add dentry to hash queues
 * @entry: dentry to add
 * @inode: The inode to attach to this dentry
 *
 * This adds the entry to the hash queues and initializes @inode.
 * The entry was actually filled in earlier during d_alloc().
 */

void d_add(struct dentry *entry, struct inode *inode)
{
	if (inode) {
		security_d_instantiate(entry, inode);
		spin_lock(&inode->i_lock);
	}
	__d_add(entry, inode);
}
EXPORT_SYMBOL(d_add);

/**
 * d_exact_alias - find and hash an exact unhashed alias
 * @entry: dentry to add
 * @inode: The inode to go with this dentry
 *
 * If an unhashed dentry with the same name/parent and desired
 * inode already exists, hash and return it.  Otherwise, return
 * NULL.
 *
 * Parent directory should be locked.
 */
struct dentry *d_exact_alias(struct dentry *entry, struct inode *inode)
{
	struct dentry *alias;
	unsigned int hash = entry->d_name.hash;

	spin_lock(&inode->i_lock);
	hlist_for_each_entry(alias, &inode->i_dentry, d_u.d_alias) {
		/*
		 * Don't need alias->d_lock here, because aliases with
		 * d_parent == entry->d_parent are not subject to name or
		 * parent changes, because the parent inode i_mutex is held.
		 */
		if (alias->d_name.hash != hash)
			continue;
		if (alias->d_parent != entry->d_parent)
			continue;
		if (!d_same_name(alias, entry->d_parent, &entry->d_name))
			continue;
		spin_lock(&alias->d_lock);
		if (!d_unhashed(alias)) {
			spin_unlock(&alias->d_lock);
			alias = NULL;
		} else {
			dget_dlock(alias);
			__d_rehash(alias);
			spin_unlock(&alias->d_lock);
		}
		spin_unlock(&inode->i_lock);
		return alias;
	}
	spin_unlock(&inode->i_lock);
	return NULL;
}
EXPORT_SYMBOL(d_exact_alias);

static void swap_names(struct dentry *dentry, struct dentry *target)
{
	if (unlikely(dname_external(target))) {
		if (unlikely(dname_external(dentry))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Both external: swap the pointers
			 */
			swap(target->d_name.name, dentry->d_name.name);
		} else {
			/*
			 * dentry:internal, target:external.  Steal target's
			 * storage and make target internal.
			 */
			memcpy(target->d_iname, dentry->d_name.name,
					dentry->d_name.len + 1);
			dentry->d_name.name = target->d_name.name;
			target->d_name.name = target->d_iname;
		}
	} else {
<<<<<<< HEAD
		if (dname_external(dentry)) {
=======
		if (unlikely(dname_external(dentry))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * dentry:external, target:internal.  Give dentry's
			 * storage to target and make dentry internal
			 */
			memcpy(dentry->d_iname, target->d_name.name,
					target->d_name.len + 1);
			target->d_name.name = dentry->d_name.name;
			dentry->d_name.name = dentry->d_iname;
		} else {
			/*
<<<<<<< HEAD
			 * Both are internal.  Just copy target to dentry
			 */
			memcpy(dentry->d_iname, target->d_name.name,
					target->d_name.len + 1);
			dentry->d_name.len = target->d_name.len;
			return;
		}
	}
	swap(dentry->d_name.len, target->d_name.len);
}

static void dentry_lock_for_move(struct dentry *dentry, struct dentry *target)
{
	/*
	 * XXXX: do we really need to take target->d_lock?
	 */
	if (IS_ROOT(dentry) || dentry->d_parent == target->d_parent)
		spin_lock(&target->d_parent->d_lock);
	else {
		if (d_ancestor(dentry->d_parent, target->d_parent)) {
			spin_lock(&dentry->d_parent->d_lock);
			spin_lock_nested(&target->d_parent->d_lock,
						DENTRY_D_LOCK_NESTED);
		} else {
			spin_lock(&target->d_parent->d_lock);
			spin_lock_nested(&dentry->d_parent->d_lock,
						DENTRY_D_LOCK_NESTED);
		}
	}
	if (target < dentry) {
		spin_lock_nested(&target->d_lock, 2);
		spin_lock_nested(&dentry->d_lock, 3);
	} else {
		spin_lock_nested(&dentry->d_lock, 2);
		spin_lock_nested(&target->d_lock, 3);
	}
}

static void dentry_unlock_parents_for_move(struct dentry *dentry,
					struct dentry *target)
{
	if (target->d_parent != dentry->d_parent)
		spin_unlock(&dentry->d_parent->d_lock);
	if (target->d_parent != target)
		spin_unlock(&target->d_parent->d_lock);
}

/*
 * When switching names, the actual string doesn't strictly have to
 * be preserved in the target - because we're dropping the target
 * anyway. As such, we can just do a simple memcpy() to copy over
 * the new name before we switch.
 *
 * Note that we have to be a lot more careful about getting the hash
 * switched - we have to switch the hash value properly even if it
 * then no longer matches the actual (corrupted) string of the target.
 * The hash value has to match the hash queue that the dentry is on..
 */
=======
			 * Both are internal.
			 */
			unsigned int i;
			BUILD_BUG_ON(!IS_ALIGNED(DNAME_INLINE_LEN, sizeof(long)));
			for (i = 0; i < DNAME_INLINE_LEN / sizeof(long); i++) {
				swap(((long *) &dentry->d_iname)[i],
				     ((long *) &target->d_iname)[i]);
			}
		}
	}
	swap(dentry->d_name.hash_len, target->d_name.hash_len);
}

static void copy_name(struct dentry *dentry, struct dentry *target)
{
	struct external_name *old_name = NULL;
	if (unlikely(dname_external(dentry)))
		old_name = external_name(dentry);
	if (unlikely(dname_external(target))) {
		atomic_inc(&external_name(target)->u.count);
		dentry->d_name = target->d_name;
	} else {
		memcpy(dentry->d_iname, target->d_name.name,
				target->d_name.len + 1);
		dentry->d_name.name = dentry->d_iname;
		dentry->d_name.hash_len = target->d_name.hash_len;
	}
	if (old_name && likely(atomic_dec_and_test(&old_name->u.count)))
		kfree_rcu(old_name, u.head);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * __d_move - move a dentry
 * @dentry: entry to move
 * @target: new dentry
<<<<<<< HEAD
=======
 * @exchange: exchange the two dentries
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Update the dcache to reflect the move of a file name. Negative
 * dcache entries should not be moved in this way. Caller must hold
 * rename_lock, the i_mutex of the source and target directories,
 * and the sb->s_vfs_rename_mutex if they differ. See lock_rename().
 */
<<<<<<< HEAD
static void __d_move(struct dentry * dentry, struct dentry * target)
{
	if (!dentry->d_inode)
		printk(KERN_WARNING "VFS: moving negative dcache entry\n");

	BUG_ON(d_ancestor(dentry, target));
	BUG_ON(d_ancestor(target, dentry));

	dentry_lock_for_move(dentry, target);

	write_seqcount_begin(&dentry->d_seq);
	write_seqcount_begin(&target->d_seq);

	/* __d_drop does write_seqcount_barrier, but they're OK to nest. */

	/*
	 * Move the dentry to the target hash queue. Don't bother checking
	 * for the same hash queue because of how unlikely it is.
	 */
	__d_drop(dentry);
	__d_rehash(dentry, d_hash(target->d_parent, target->d_name.hash));

	/* Unhash the target: dput() will then get rid of it */
	__d_drop(target);

	list_del(&dentry->d_child);
	list_del(&target->d_child);

	/* Switch the names.. */
	switch_names(dentry, target);
	swap(dentry->d_name.hash, target->d_name.hash);

	/* ... and switch the parents */
	if (IS_ROOT(dentry)) {
		dentry->d_parent = target->d_parent;
		target->d_parent = target;
		INIT_LIST_HEAD(&target->d_child);
	} else {
		swap(dentry->d_parent, target->d_parent);

		/* And add them back to the (new) parent lists */
		list_add(&target->d_child, &target->d_parent->d_subdirs);
	}

	list_add(&dentry->d_child, &dentry->d_parent->d_subdirs);
=======
static void __d_move(struct dentry *dentry, struct dentry *target,
		     bool exchange)
{
	struct dentry *old_parent, *p;
	wait_queue_head_t *d_wait;
	struct inode *dir = NULL;
	unsigned n;

	WARN_ON(!dentry->d_inode);
	if (WARN_ON(dentry == target))
		return;

	BUG_ON(d_ancestor(target, dentry));
	old_parent = dentry->d_parent;
	p = d_ancestor(old_parent, target);
	if (IS_ROOT(dentry)) {
		BUG_ON(p);
		spin_lock(&target->d_parent->d_lock);
	} else if (!p) {
		/* target is not a descendent of dentry->d_parent */
		spin_lock(&target->d_parent->d_lock);
		spin_lock_nested(&old_parent->d_lock, DENTRY_D_LOCK_NESTED);
	} else {
		BUG_ON(p == dentry);
		spin_lock(&old_parent->d_lock);
		if (p != target)
			spin_lock_nested(&target->d_parent->d_lock,
					DENTRY_D_LOCK_NESTED);
	}
	spin_lock_nested(&dentry->d_lock, 2);
	spin_lock_nested(&target->d_lock, 3);

	if (unlikely(d_in_lookup(target))) {
		dir = target->d_parent->d_inode;
		n = start_dir_add(dir);
		d_wait = __d_lookup_unhash(target);
	}

	write_seqcount_begin(&dentry->d_seq);
	write_seqcount_begin_nested(&target->d_seq, DENTRY_D_LOCK_NESTED);

	/* unhash both */
	if (!d_unhashed(dentry))
		___d_drop(dentry);
	if (!d_unhashed(target))
		___d_drop(target);

	/* ... and switch them in the tree */
	dentry->d_parent = target->d_parent;
	if (!exchange) {
		copy_name(dentry, target);
		target->d_hash.pprev = NULL;
		dentry->d_parent->d_lockref.count++;
		if (dentry != old_parent) /* wasn't IS_ROOT */
			WARN_ON(!--old_parent->d_lockref.count);
	} else {
		target->d_parent = old_parent;
		swap_names(dentry, target);
		if (!hlist_unhashed(&target->d_sib))
			__hlist_del(&target->d_sib);
		hlist_add_head(&target->d_sib, &target->d_parent->d_children);
		__d_rehash(target);
		fsnotify_update_flags(target);
	}
	if (!hlist_unhashed(&dentry->d_sib))
		__hlist_del(&dentry->d_sib);
	hlist_add_head(&dentry->d_sib, &dentry->d_parent->d_children);
	__d_rehash(dentry);
	fsnotify_update_flags(dentry);
	fscrypt_handle_d_move(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	write_seqcount_end(&target->d_seq);
	write_seqcount_end(&dentry->d_seq);

<<<<<<< HEAD
	dentry_unlock_parents_for_move(dentry, target);
	spin_unlock(&target->d_lock);
	fsnotify_d_move(dentry);
=======
	if (dir)
		end_dir_add(dir, n, d_wait);

	if (dentry->d_parent != old_parent)
		spin_unlock(&dentry->d_parent->d_lock);
	if (dentry != old_parent)
		spin_unlock(&old_parent->d_lock);
	spin_unlock(&target->d_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&dentry->d_lock);
}

/*
 * d_move - move a dentry
 * @dentry: entry to move
 * @target: new dentry
 *
 * Update the dcache to reflect the move of a file name. Negative
 * dcache entries should not be moved in this way. See the locking
 * requirements for __d_move.
 */
void d_move(struct dentry *dentry, struct dentry *target)
{
	write_seqlock(&rename_lock);
<<<<<<< HEAD
	__d_move(dentry, target);
=======
	__d_move(dentry, target, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	write_sequnlock(&rename_lock);
}
EXPORT_SYMBOL(d_move);

<<<<<<< HEAD
=======
/*
 * d_exchange - exchange two dentries
 * @dentry1: first dentry
 * @dentry2: second dentry
 */
void d_exchange(struct dentry *dentry1, struct dentry *dentry2)
{
	write_seqlock(&rename_lock);

	WARN_ON(!dentry1->d_inode);
	WARN_ON(!dentry2->d_inode);
	WARN_ON(IS_ROOT(dentry1));
	WARN_ON(IS_ROOT(dentry2));

	__d_move(dentry1, dentry2, true);

	write_sequnlock(&rename_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * d_ancestor - search for an ancestor
 * @p1: ancestor dentry
 * @p2: child dentry
 *
 * Returns the ancestor dentry of p2 which is a child of p1, if p1 is
 * an ancestor of p2, else NULL.
 */
struct dentry *d_ancestor(struct dentry *p1, struct dentry *p2)
{
	struct dentry *p;

	for (p = p2; !IS_ROOT(p); p = p->d_parent) {
		if (p->d_parent == p1)
			return p;
	}
	return NULL;
}

/*
 * This helper attempts to cope with remotely renamed directories
 *
 * It assumes that the caller is already holding
<<<<<<< HEAD
 * dentry->d_parent->d_inode->i_mutex, inode->i_lock and rename_lock
=======
 * dentry->d_parent->d_inode->i_mutex, and rename_lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Note: If ever the locking in lock_rename() changes, then please
 * remember to update this too...
 */
<<<<<<< HEAD
static struct dentry *__d_unalias(struct inode *inode,
		struct dentry *dentry, struct dentry *alias)
{
	struct mutex *m1 = NULL, *m2 = NULL;
	struct dentry *ret;
=======
static int __d_unalias(struct dentry *dentry, struct dentry *alias)
{
	struct mutex *m1 = NULL;
	struct rw_semaphore *m2 = NULL;
	int ret = -ESTALE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If alias and dentry share a parent, then no extra locks required */
	if (alias->d_parent == dentry->d_parent)
		goto out_unalias;

	/* See lock_rename() */
<<<<<<< HEAD
	ret = ERR_PTR(-EBUSY);
	if (!mutex_trylock(&dentry->d_sb->s_vfs_rename_mutex))
		goto out_err;
	m1 = &dentry->d_sb->s_vfs_rename_mutex;
	if (!mutex_trylock(&alias->d_parent->d_inode->i_mutex))
		goto out_err;
	m2 = &alias->d_parent->d_inode->i_mutex;
out_unalias:
	__d_move(alias, dentry);
	ret = alias;
out_err:
	spin_unlock(&inode->i_lock);
	if (m2)
		mutex_unlock(m2);
=======
	if (!mutex_trylock(&dentry->d_sb->s_vfs_rename_mutex))
		goto out_err;
	m1 = &dentry->d_sb->s_vfs_rename_mutex;
	if (!inode_trylock_shared(alias->d_parent->d_inode))
		goto out_err;
	m2 = &alias->d_parent->d_inode->i_rwsem;
out_unalias:
	__d_move(alias, dentry, false);
	ret = 0;
out_err:
	if (m2)
		up_read(m2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (m1)
		mutex_unlock(m1);
	return ret;
}

<<<<<<< HEAD
/*
 * Prepare an anonymous dentry for life in the superblock's dentry tree as a
 * named dentry in place of the dentry to be replaced.
 * returns with anon->d_lock held!
 */
static void __d_materialise_dentry(struct dentry *dentry, struct dentry *anon)
{
	struct dentry *dparent, *aparent;

	dentry_lock_for_move(anon, dentry);

	write_seqcount_begin(&dentry->d_seq);
	write_seqcount_begin(&anon->d_seq);

	dparent = dentry->d_parent;
	aparent = anon->d_parent;

	switch_names(dentry, anon);
	swap(dentry->d_name.hash, anon->d_name.hash);

	dentry->d_parent = (aparent == anon) ? dentry : aparent;
	list_del(&dentry->d_child);
	if (!IS_ROOT(dentry))
		list_add(&dentry->d_child, &dentry->d_parent->d_subdirs);
	else
		INIT_LIST_HEAD(&dentry->d_child);

	anon->d_parent = (dparent == dentry) ? anon : dparent;
	list_del(&anon->d_child);
	if (!IS_ROOT(anon))
		list_add(&anon->d_child, &anon->d_parent->d_subdirs);
	else
		INIT_LIST_HEAD(&anon->d_child);

	write_seqcount_end(&dentry->d_seq);
	write_seqcount_end(&anon->d_seq);

	dentry_unlock_parents_for_move(anon, dentry);
	spin_unlock(&dentry->d_lock);

	/* anon->d_lock still locked, returns locked */
	anon->d_flags &= ~DCACHE_DISCONNECTED;
}

/**
 * d_materialise_unique - introduce an inode into the tree
 * @dentry: candidate dentry
 * @inode: inode to bind to the dentry, to which aliases may be attached
 *
 * Introduces an dentry into the tree, substituting an extant disconnected
 * root directory alias in its place if there is one. Caller must hold the
 * i_mutex of the parent directory.
 */
struct dentry *d_materialise_unique(struct dentry *dentry, struct inode *inode)
{
	struct dentry *actual;

	BUG_ON(!d_unhashed(dentry));

	if (!inode) {
		actual = dentry;
		__d_instantiate(dentry, NULL);
		d_rehash(actual);
		goto out_nolock;
	}

	spin_lock(&inode->i_lock);

	if (S_ISDIR(inode->i_mode)) {
		struct dentry *alias;

		/* Does an aliased dentry already exist? */
		alias = __d_find_alias(inode, 0);
		if (alias) {
			actual = alias;
			write_seqlock(&rename_lock);

			if (d_ancestor(alias, dentry)) {
				/* Check for loops */
				actual = ERR_PTR(-ELOOP);
				spin_unlock(&inode->i_lock);
			} else if (IS_ROOT(alias)) {
				/* Is this an anonymous mountpoint that we
				 * could splice into our tree? */
				__d_materialise_dentry(dentry, alias);
				write_sequnlock(&rename_lock);
				__d_drop(alias);
				goto found;
			} else {
				/* Nope, but we must(!) avoid directory
				 * aliasing. This drops inode->i_lock */
				actual = __d_unalias(inode, dentry, alias);
			}
			write_sequnlock(&rename_lock);
			if (IS_ERR(actual)) {
				if (PTR_ERR(actual) == -ELOOP)
					pr_warn_ratelimited(
						"VFS: Lookup of '%s' in %s %s"
						" would have caused loop\n",
						dentry->d_name.name,
						inode->i_sb->s_type->name,
						inode->i_sb->s_id);
				dput(alias);
			}
			goto out_nolock;
		}
	}

	/* Add a unique reference */
	actual = __d_instantiate_unique(dentry, inode);
	if (!actual)
		actual = dentry;
	else
		BUG_ON(!d_unhashed(actual));

	spin_lock(&actual->d_lock);
found:
	_d_rehash(actual);
	spin_unlock(&actual->d_lock);
	spin_unlock(&inode->i_lock);
out_nolock:
	if (actual == dentry) {
		security_d_instantiate(dentry, inode);
		return NULL;
	}

	iput(inode);
	return actual;
}
EXPORT_SYMBOL_GPL(d_materialise_unique);

static int prepend(char **buffer, int *buflen, const char *str, int namelen)
{
	*buflen -= namelen;
	if (*buflen < 0)
		return -ENAMETOOLONG;
	*buffer -= namelen;
	memcpy(*buffer, str, namelen);
	return 0;
}

static int prepend_name(char **buffer, int *buflen, struct qstr *name)
{
	return prepend(buffer, buflen, name->name, name->len);
}

/**
 * prepend_path - Prepend path string to a buffer
 * @path: the dentry/vfsmount to report
 * @root: root vfsmnt/dentry
 * @buffer: pointer to the end of the buffer
 * @buflen: pointer to buffer length
 *
 * Caller holds the rename_lock.
 */
static int prepend_path(const struct path *path,
			const struct path *root,
			char **buffer, int *buflen)
{
	struct dentry *dentry = path->dentry;
	struct vfsmount *vfsmnt = path->mnt;
	struct mount *mnt = real_mount(vfsmnt);
	char *orig_buffer = *buffer;
	int orig_len = *buflen;
	bool slash = false;
	int error = 0;

	while (dentry != root->dentry || vfsmnt != root->mnt) {
		struct dentry * parent;

		if (dentry == vfsmnt->mnt_root || IS_ROOT(dentry)) {
			/* Escaped? */
			if (dentry != vfsmnt->mnt_root) {
				*buffer = orig_buffer;
				*buflen = orig_len;
				slash = false;
				error = 3;
				goto global_root;
			}
			/* Global root? */
			if (!mnt_has_parent(mnt))
				goto global_root;
			dentry = mnt->mnt_mountpoint;
			mnt = mnt->mnt_parent;
			vfsmnt = &mnt->mnt;
			continue;
		}
		parent = dentry->d_parent;
		prefetch(parent);
		spin_lock(&dentry->d_lock);
		error = prepend_name(buffer, buflen, &dentry->d_name);
		spin_unlock(&dentry->d_lock);
		if (!error)
			error = prepend(buffer, buflen, "/", 1);
		if (error)
			break;

		slash = true;
		dentry = parent;
	}

	if (!error && !slash)
		error = prepend(buffer, buflen, "/", 1);

	return error;

global_root:
	/*
	 * Filesystems needing to implement special "root names"
	 * should do so with ->d_dname()
	 */
	if (IS_ROOT(dentry) &&
	    (dentry->d_name.len != 1 || dentry->d_name.name[0] != '/')) {
		WARN(1, "Root dentry has weird name <%.*s>\n",
		     (int) dentry->d_name.len, dentry->d_name.name);
	}
	if (!slash)
		error = prepend(buffer, buflen, "/", 1);
	if (!error)
		error = real_mount(vfsmnt)->mnt_ns ? 1 : 2;
	return error;
}

/**
 * __d_path - return the path of a dentry
 * @path: the dentry/vfsmount to report
 * @root: root vfsmnt/dentry
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * Convert a dentry into an ASCII path name.
 *
 * Returns a pointer into the buffer or an error code if the
 * path was too long.
 *
 * "buflen" should be positive.
 *
 * If the path is not reachable from the supplied root, return %NULL.
 */
char *__d_path(const struct path *path,
	       const struct path *root,
	       char *buf, int buflen)
{
	char *res = buf + buflen;
	int error;

	prepend(&res, &buflen, "\0", 1);
	br_read_lock(&vfsmount_lock);
	write_seqlock(&rename_lock);
	error = prepend_path(path, root, &res, &buflen);
	write_sequnlock(&rename_lock);
	br_read_unlock(&vfsmount_lock);

	if (error < 0)
		return ERR_PTR(error);
	if (error > 0)
		return NULL;
	return res;
}

char *d_absolute_path(const struct path *path,
	       char *buf, int buflen)
{
	struct path root = {};
	char *res = buf + buflen;
	int error;

	prepend(&res, &buflen, "\0", 1);
	br_read_lock(&vfsmount_lock);
	write_seqlock(&rename_lock);
	error = prepend_path(path, &root, &res, &buflen);
	write_sequnlock(&rename_lock);
	br_read_unlock(&vfsmount_lock);

	if (error > 1)
		error = -EINVAL;
	if (error < 0)
		return ERR_PTR(error);
	return res;
}

/*
 * same as __d_path but appends "(deleted)" for unlinked files.
 */
static int path_with_deleted(const struct path *path,
			     const struct path *root,
			     char **buf, int *buflen)
{
	prepend(buf, buflen, "\0", 1);
	if (d_unlinked(path->dentry)) {
		int error = prepend(buf, buflen, " (deleted)", 10);
		if (error)
			return error;
	}

	return prepend_path(path, root, buf, buflen);
}

static int prepend_unreachable(char **buffer, int *buflen)
{
	return prepend(buffer, buflen, "(unreachable)", 13);
}

/**
 * d_path - return the path of a dentry
 * @path: path to report
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * Convert a dentry into an ASCII path name. If the entry has been deleted
 * the string " (deleted)" is appended. Note that this is ambiguous.
 *
 * Returns a pointer into the buffer or an error code if the path was
 * too long. Note: Callers should use the returned pointer, not the passed
 * in buffer, to use the name! The implementation often starts at an offset
 * into the buffer, and may leave 0 bytes at the start.
 *
 * "buflen" should be positive.
 */
char *d_path(const struct path *path, char *buf, int buflen)
{
	char *res = buf + buflen;
	struct path root;
	int error;

	/*
	 * We have various synthetic filesystems that never get mounted.  On
	 * these filesystems dentries are never used for lookup purposes, and
	 * thus don't need to be hashed.  They also don't need a name until a
	 * user wants to identify the object in /proc/pid/fd/.  The little hack
	 * below allows us to generate a name for these objects on demand:
	 */
	if (path->dentry->d_op && path->dentry->d_op->d_dname)
		return path->dentry->d_op->d_dname(path->dentry, buf, buflen);

	get_fs_root(current->fs, &root);
	br_read_lock(&vfsmount_lock);
	write_seqlock(&rename_lock);
	error = path_with_deleted(path, &root, &res, &buflen);
	write_sequnlock(&rename_lock);
	br_read_unlock(&vfsmount_lock);
	if (error < 0)
		res = ERR_PTR(error);
	path_put(&root);
	return res;
}
EXPORT_SYMBOL(d_path);

/**
 * d_path_with_unreachable - return the path of a dentry
 * @path: path to report
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * The difference from d_path() is that this prepends "(unreachable)"
 * to paths which are unreachable from the current process' root.
 */
char *d_path_with_unreachable(const struct path *path, char *buf, int buflen)
{
	char *res = buf + buflen;
	struct path root;
	int error;

	if (path->dentry->d_op && path->dentry->d_op->d_dname)
		return path->dentry->d_op->d_dname(path->dentry, buf, buflen);

	get_fs_root(current->fs, &root);
	write_seqlock(&rename_lock);
	error = path_with_deleted(path, &root, &res, &buflen);
	if (error > 0)
		error = prepend_unreachable(&res, &buflen);
	write_sequnlock(&rename_lock);
	path_put(&root);
	if (error)
		res =  ERR_PTR(error);

	return res;
}

/*
 * Helper function for dentry_operations.d_dname() members
 */
char *dynamic_dname(struct dentry *dentry, char *buffer, int buflen,
			const char *fmt, ...)
{
	va_list args;
	char temp[256];
	int sz;

	va_start(args, fmt);
	sz = vsnprintf(temp, sizeof(temp), fmt, args) + 1;
	va_end(args);

	if (sz > sizeof(temp) || sz > buflen)
		return ERR_PTR(-ENAMETOOLONG);

	buffer += buflen - sz;
	return memcpy(buffer, temp, sz);
}

/*
 * Write full pathname from the root of the filesystem into the buffer.
 */
static char *__dentry_path(struct dentry *dentry, char *buf, int buflen)
{
	char *end = buf + buflen;
	char *retval;

	prepend(&end, &buflen, "\0", 1);
	if (buflen < 1)
		goto Elong;
	/* Get '/' right */
	retval = end-1;
	*retval = '/';

	while (!IS_ROOT(dentry)) {
		struct dentry *parent = dentry->d_parent;
		int error;

		prefetch(parent);
		spin_lock(&dentry->d_lock);
		error = prepend_name(&end, &buflen, &dentry->d_name);
		spin_unlock(&dentry->d_lock);
		if (error != 0 || prepend(&end, &buflen, "/", 1) != 0)
			goto Elong;

		retval = end;
		dentry = parent;
	}
	return retval;
Elong:
	return ERR_PTR(-ENAMETOOLONG);
}

char *dentry_path_raw(struct dentry *dentry, char *buf, int buflen)
{
	char *retval;

	write_seqlock(&rename_lock);
	retval = __dentry_path(dentry, buf, buflen);
	write_sequnlock(&rename_lock);

	return retval;
}
EXPORT_SYMBOL(dentry_path_raw);

char *dentry_path(struct dentry *dentry, char *buf, int buflen)
{
	char *p = NULL;
	char *retval;

	write_seqlock(&rename_lock);
	if (d_unlinked(dentry)) {
		p = buf + buflen;
		if (prepend(&p, &buflen, "//deleted", 10) != 0)
			goto Elong;
		buflen++;
	}
	retval = __dentry_path(dentry, buf, buflen);
	write_sequnlock(&rename_lock);
	if (!IS_ERR(retval) && p)
		*p = '/';	/* restore '/' overriden with '\0' */
	return retval;
Elong:
	return ERR_PTR(-ENAMETOOLONG);
}

/*
 * NOTE! The user-level library version returns a
 * character pointer. The kernel system call just
 * returns the length of the buffer filled (which
 * includes the ending '\0' character), or a negative
 * error value. So libc would do something like
 *
 *	char *getcwd(char * buf, size_t size)
 *	{
 *		int retval;
 *
 *		retval = sys_getcwd(buf, size);
 *		if (retval >= 0)
 *			return buf;
 *		errno = -retval;
 *		return NULL;
 *	}
 */
SYSCALL_DEFINE2(getcwd, char __user *, buf, unsigned long, size)
{
	int error;
	struct path pwd, root;
	char *page = (char *) __get_free_page(GFP_USER);

	if (!page)
		return -ENOMEM;

	get_fs_root_and_pwd(current->fs, &root, &pwd);

	error = -ENOENT;
	br_read_lock(&vfsmount_lock);
	write_seqlock(&rename_lock);
	if (!d_unlinked(pwd.dentry)) {
		unsigned long len;
		char *cwd = page + PAGE_SIZE;
		int buflen = PAGE_SIZE;

		prepend(&cwd, &buflen, "\0", 1);
		error = prepend_path(&pwd, &root, &cwd, &buflen);
		write_sequnlock(&rename_lock);
		br_read_unlock(&vfsmount_lock);

		if (error < 0)
			goto out;

		/* Unreachable from current root */
		if (error > 0) {
			error = prepend_unreachable(&cwd, &buflen);
			if (error)
				goto out;
		}

		error = -ERANGE;
		len = PAGE_SIZE + page - cwd;
		if (len <= size) {
			error = len;
			if (copy_to_user(buf, cwd, len))
				error = -EFAULT;
		}
	} else {
		write_sequnlock(&rename_lock);
		br_read_unlock(&vfsmount_lock);
	}

out:
	path_put(&pwd);
	path_put(&root);
	free_page((unsigned long) page);
	return error;
}
=======
/**
 * d_splice_alias - splice a disconnected dentry into the tree if one exists
 * @inode:  the inode which may have a disconnected dentry
 * @dentry: a negative dentry which we want to point to the inode.
 *
 * If inode is a directory and has an IS_ROOT alias, then d_move that in
 * place of the given dentry and return it, else simply d_add the inode
 * to the dentry and return NULL.
 *
 * If a non-IS_ROOT directory is found, the filesystem is corrupt, and
 * we should error out: directories can't have multiple aliases.
 *
 * This is needed in the lookup routine of any filesystem that is exportable
 * (via knfsd) so that we can build dcache paths to directories effectively.
 *
 * If a dentry was found and moved, then it is returned.  Otherwise NULL
 * is returned.  This matches the expected return value of ->lookup.
 *
 * Cluster filesystems may call this function with a negative, hashed dentry.
 * In that case, we know that the inode will be a regular file, and also this
 * will only occur during atomic_open. So we need to check for the dentry
 * being already hashed only in the final case.
 */
struct dentry *d_splice_alias(struct inode *inode, struct dentry *dentry)
{
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	BUG_ON(!d_unhashed(dentry));

	if (!inode)
		goto out;

	security_d_instantiate(dentry, inode);
	spin_lock(&inode->i_lock);
	if (S_ISDIR(inode->i_mode)) {
		struct dentry *new = __d_find_any_alias(inode);
		if (unlikely(new)) {
			/* The reference to new ensures it remains an alias */
			spin_unlock(&inode->i_lock);
			write_seqlock(&rename_lock);
			if (unlikely(d_ancestor(new, dentry))) {
				write_sequnlock(&rename_lock);
				dput(new);
				new = ERR_PTR(-ELOOP);
				pr_warn_ratelimited(
					"VFS: Lookup of '%s' in %s %s"
					" would have caused loop\n",
					dentry->d_name.name,
					inode->i_sb->s_type->name,
					inode->i_sb->s_id);
			} else if (!IS_ROOT(new)) {
				struct dentry *old_parent = dget(new->d_parent);
				int err = __d_unalias(dentry, new);
				write_sequnlock(&rename_lock);
				if (err) {
					dput(new);
					new = ERR_PTR(err);
				}
				dput(old_parent);
			} else {
				__d_move(new, dentry, false);
				write_sequnlock(&rename_lock);
			}
			iput(inode);
			return new;
		}
	}
out:
	__d_add(dentry, inode);
	return NULL;
}
EXPORT_SYMBOL(d_splice_alias);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Test whether new_dentry is a subdirectory of old_dentry.
 *
 * Trivially implemented using the dcache structure
 */

/**
 * is_subdir - is new dentry a subdirectory of old_dentry
 * @new_dentry: new dentry
 * @old_dentry: old dentry
 *
<<<<<<< HEAD
 * Returns 1 if new_dentry is a subdirectory of the parent (at any depth).
 * Returns 0 otherwise.
 * Caller must ensure that "new_dentry" is pinned before calling is_subdir()
 */
  
int is_subdir(struct dentry *new_dentry, struct dentry *old_dentry)
{
	int result;
	unsigned seq;

	if (new_dentry == old_dentry)
		return 1;
=======
 * Returns true if new_dentry is a subdirectory of the parent (at any depth).
 * Returns false otherwise.
 * Caller must ensure that "new_dentry" is pinned before calling is_subdir()
 */
  
bool is_subdir(struct dentry *new_dentry, struct dentry *old_dentry)
{
	bool result;
	unsigned seq;

	if (new_dentry == old_dentry)
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	do {
		/* for restarting inner loop in case of seq retry */
		seq = read_seqbegin(&rename_lock);
		/*
		 * Need rcu_readlock to protect against the d_parent trashing
		 * due to d_move
		 */
		rcu_read_lock();
		if (d_ancestor(old_dentry, new_dentry))
<<<<<<< HEAD
			result = 1;
		else
			result = 0;
=======
			result = true;
		else
			result = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rcu_read_unlock();
	} while (read_seqretry(&rename_lock, seq));

	return result;
}
<<<<<<< HEAD

void d_genocide(struct dentry *root)
{
	struct dentry *this_parent;
	struct list_head *next;
	unsigned seq;
	int locked = 0;

	seq = read_seqbegin(&rename_lock);
again:
	this_parent = root;
	spin_lock(&this_parent->d_lock);
repeat:
	next = this_parent->d_subdirs.next;
resume:
	while (next != &this_parent->d_subdirs) {
		struct list_head *tmp = next;
		struct dentry *dentry = list_entry(tmp, struct dentry, d_child);
		next = tmp->next;

		spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
		if (d_unhashed(dentry) || !dentry->d_inode) {
			spin_unlock(&dentry->d_lock);
			continue;
		}
		if (!list_empty(&dentry->d_subdirs)) {
			spin_unlock(&this_parent->d_lock);
			spin_release(&dentry->d_lock.dep_map, 1, _RET_IP_);
			this_parent = dentry;
			spin_acquire(&this_parent->d_lock.dep_map, 0, 1, _RET_IP_);
			goto repeat;
		}
		if (!(dentry->d_flags & DCACHE_GENOCIDE)) {
			dentry->d_flags |= DCACHE_GENOCIDE;
			dentry->d_count--;
		}
		spin_unlock(&dentry->d_lock);
	}
	rcu_read_lock();
ascend:
	if (this_parent != root) {
		struct dentry *child = this_parent;
		if (!(this_parent->d_flags & DCACHE_GENOCIDE)) {
			this_parent->d_flags |= DCACHE_GENOCIDE;
			this_parent->d_count--;
		}
		this_parent = child->d_parent;

		spin_unlock(&child->d_lock);
		spin_lock(&this_parent->d_lock);

		/* might go back up the wrong parent if we have had a rename */
		if (!locked && read_seqretry(&rename_lock, seq))
			goto rename_retry;
		next = child->d_child.next;
		while (unlikely(child->d_flags & DCACHE_DENTRY_KILLED)) {
			if (next == &this_parent->d_subdirs)
				goto ascend;
			child = list_entry(next, struct dentry, d_child);
			next = next->next;
		}
		rcu_read_unlock();
		goto resume;
	}
	if (!locked && read_seqretry(&rename_lock, seq))
		goto rename_retry;
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (locked)
		write_sequnlock(&rename_lock);
	return;

rename_retry:
	spin_unlock(&this_parent->d_lock);
	rcu_read_unlock();
	if (locked)
		goto again;
	locked = 1;
	write_seqlock(&rename_lock);
	goto again;
}

void d_tmpfile(struct dentry *dentry, struct inode *inode)
{
	inode_dec_link_count(inode);
	BUG_ON(dentry->d_name.name != dentry->d_iname ||
		!list_empty(&dentry->d_u.d_alias) ||
=======
EXPORT_SYMBOL(is_subdir);

static enum d_walk_ret d_genocide_kill(void *data, struct dentry *dentry)
{
	struct dentry *root = data;
	if (dentry != root) {
		if (d_unhashed(dentry) || !dentry->d_inode)
			return D_WALK_SKIP;

		if (!(dentry->d_flags & DCACHE_GENOCIDE)) {
			dentry->d_flags |= DCACHE_GENOCIDE;
			dentry->d_lockref.count--;
		}
	}
	return D_WALK_CONTINUE;
}

void d_genocide(struct dentry *parent)
{
	d_walk(parent, parent, d_genocide_kill);
}

void d_mark_tmpfile(struct file *file, struct inode *inode)
{
	struct dentry *dentry = file->f_path.dentry;

	BUG_ON(dentry->d_name.name != dentry->d_iname ||
		!hlist_unhashed(&dentry->d_u.d_alias) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		!d_unlinked(dentry));
	spin_lock(&dentry->d_parent->d_lock);
	spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
	dentry->d_name.len = sprintf(dentry->d_iname, "#%llu",
				(unsigned long long)inode->i_ino);
	spin_unlock(&dentry->d_lock);
	spin_unlock(&dentry->d_parent->d_lock);
<<<<<<< HEAD
=======
}
EXPORT_SYMBOL(d_mark_tmpfile);

void d_tmpfile(struct file *file, struct inode *inode)
{
	struct dentry *dentry = file->f_path.dentry;

	inode_dec_link_count(inode);
	d_mark_tmpfile(file, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	d_instantiate(dentry, inode);
}
EXPORT_SYMBOL(d_tmpfile);

<<<<<<< HEAD
/**
 * find_inode_number - check for dentry with name
 * @dir: directory to check
 * @name: Name to find.
 *
 * Check whether a dentry already exists for the given name,
 * and return the inode number if it has an inode. Otherwise
 * 0 is returned.
 *
 * This routine is used to post-process directory listings for
 * filesystems using synthetic inode numbers, and is necessary
 * to keep getcwd() working.
 */
 
ino_t find_inode_number(struct dentry *dir, struct qstr *name)
{
	struct dentry * dentry;
	ino_t ino = 0;

	dentry = d_hash_and_lookup(dir, name);
	if (dentry) {
		if (dentry->d_inode)
			ino = dentry->d_inode->i_ino;
		dput(dentry);
	}
	return ino;
}
EXPORT_SYMBOL(find_inode_number);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __initdata unsigned long dhash_entries;
static int __init set_dhash_entries(char *str)
{
	if (!str)
		return 0;
	dhash_entries = simple_strtoul(str, &str, 0);
	return 1;
}
__setup("dhash_entries=", set_dhash_entries);

static void __init dcache_init_early(void)
{
<<<<<<< HEAD
	unsigned int loop;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If hashes are distributed across NUMA nodes, defer
	 * hash allocation until vmalloc space is available.
	 */
	if (hashdist)
		return;

	dentry_hashtable =
		alloc_large_system_hash("Dentry cache",
					sizeof(struct hlist_bl_head),
					dhash_entries,
					13,
<<<<<<< HEAD
					HASH_EARLY,
					&d_hash_shift,
					&d_hash_mask,
					0);

	for (loop = 0; loop < (1U << d_hash_shift); loop++)
		INIT_HLIST_BL_HEAD(dentry_hashtable + loop);
=======
					HASH_EARLY | HASH_ZERO,
					&d_hash_shift,
					NULL,
					0,
					0);
	d_hash_shift = 32 - d_hash_shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init dcache_init(void)
{
<<<<<<< HEAD
	unsigned int loop;

	/* 
	 * A constructor could be added for stable state like the lists,
	 * but it is probably not worth it because of the cache nature
	 * of the dcache. 
	 */
	dentry_cache = KMEM_CACHE(dentry,
		SLAB_RECLAIM_ACCOUNT|SLAB_PANIC|SLAB_MEM_SPREAD);
=======
	/*
	 * A constructor could be added for stable state like the lists,
	 * but it is probably not worth it because of the cache nature
	 * of the dcache.
	 */
	dentry_cache = KMEM_CACHE_USERCOPY(dentry,
		SLAB_RECLAIM_ACCOUNT|SLAB_PANIC|SLAB_ACCOUNT,
		d_iname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Hash may have been set up in dcache_init_early */
	if (!hashdist)
		return;

	dentry_hashtable =
		alloc_large_system_hash("Dentry cache",
					sizeof(struct hlist_bl_head),
					dhash_entries,
					13,
<<<<<<< HEAD
					0,
					&d_hash_shift,
					&d_hash_mask,
					0);

	for (loop = 0; loop < (1U << d_hash_shift); loop++)
		INIT_HLIST_BL_HEAD(dentry_hashtable + loop);
}

/* SLAB cache for __getname() consumers */
struct kmem_cache *names_cachep __read_mostly;
EXPORT_SYMBOL(names_cachep);

EXPORT_SYMBOL(d_genocide);

void __init vfs_caches_init_early(void)
{
=======
					HASH_ZERO,
					&d_hash_shift,
					NULL,
					0,
					0);
	d_hash_shift = 32 - d_hash_shift;
}

/* SLAB cache for __getname() consumers */
struct kmem_cache *names_cachep __ro_after_init;
EXPORT_SYMBOL(names_cachep);

void __init vfs_caches_init_early(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(in_lookup_hashtable); i++)
		INIT_HLIST_BL_HEAD(&in_lookup_hashtable[i]);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dcache_init_early();
	inode_init_early();
}

<<<<<<< HEAD
void __init vfs_caches_init(unsigned long mempages)
{
	unsigned long reserve;

	/* Base hash sizes on available memory, with a reserve equal to
           150% of current kernel size */

	reserve = min((mempages - nr_free_pages()) * 3/2, mempages - 1);
	mempages -= reserve;

	names_cachep = kmem_cache_create("names_cache", PATH_MAX, 0,
			SLAB_HWCACHE_ALIGN|SLAB_PANIC, NULL);

	dcache_init();
	inode_init();
	files_init(mempages);
=======
void __init vfs_caches_init(void)
{
	names_cachep = kmem_cache_create_usercopy("names_cache", PATH_MAX, 0,
			SLAB_HWCACHE_ALIGN|SLAB_PANIC, 0, PATH_MAX, NULL);

	dcache_init();
	inode_init();
	files_init();
	files_maxfiles_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mnt_init();
	bdev_cache_init();
	chrdev_init();
}
