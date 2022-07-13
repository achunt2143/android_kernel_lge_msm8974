<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_U64_STATS_SYNC_H
#define _LINUX_U64_STATS_SYNC_H

/*
<<<<<<< HEAD
 * To properly implement 64bits network statistics on 32bit and 64bit hosts,
 * we provide a synchronization point, that is a noop on 64bit or UP kernels.
 *
 * Key points :
 * 1) Use a seqcount on SMP 32bits, with low overhead.
 * 2) Whole thing is a noop on 64bit arches or UP kernels.
 * 3) Write side must ensure mutual exclusion or one seqcount update could
 *    be lost, thus blocking readers forever.
 *    If this synchronization point is not a mutex, but a spinlock or
 *    spinlock_bh() or disable_bh() :
 * 3.1) Write side should not sleep.
 * 3.2) Write side should not allow preemption.
 * 3.3) If applicable, interrupts should be disabled.
 *
 * 4) If reader fetches several counters, there is no guarantee the whole values
 *    are consistent (remember point 1) : this is a noop on 64bit arches anyway)
 *
 * 5) readers are allowed to sleep or be preempted/interrupted : They perform
 *    pure reads. But if they have to fetch many values, it's better to not allow
 *    preemptions/interruptions to avoid many retries.
 *
 * 6) If counter might be written by an interrupt, readers should block interrupts.
 *    (On UP, there is no seqcount_t protection, a reader allowing interrupts could
 *     read partial values)
 *
 * 7) For irq and softirq uses, readers can use u64_stats_fetch_begin_irq() and
 *    u64_stats_fetch_retry_irq() helpers
=======
 * Protect against 64-bit values tearing on 32-bit architectures. This is
 * typically used for statistics read/update in different subsystems.
 *
 * Key points :
 *
 * -  Use a seqcount on 32-bit
 * -  The whole thing is a no-op on 64-bit architectures.
 *
 * Usage constraints:
 *
 * 1) Write side must ensure mutual exclusion, or one seqcount update could
 *    be lost, thus blocking readers forever.
 *
 * 2) Write side must disable preemption, or a seqcount reader can preempt the
 *    writer and also spin forever.
 *
 * 3) Write side must use the _irqsave() variant if other writers, or a reader,
 *    can be invoked from an IRQ context. On 64bit systems this variant does not
 *    disable interrupts.
 *
 * 4) If reader fetches several counters, there is no guarantee the whole values
 *    are consistent w.r.t. each other (remember point #2: seqcounts are not
 *    used for 64bit architectures).
 *
 * 5) Readers are allowed to sleep or be preempted/interrupted: they perform
 *    pure reads.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Usage :
 *
 * Stats producer (writer) should use following template granted it already got
 * an exclusive access to counters (a lock is already taken, or per cpu
 * data is used [in a non preemptable context])
 *
 *   spin_lock_bh(...) or other synchronization to get exclusive access
 *   ...
 *   u64_stats_update_begin(&stats->syncp);
<<<<<<< HEAD
 *   stats->bytes64 += len; // non atomic operation
 *   stats->packets64++;    // non atomic operation
=======
 *   u64_stats_add(&stats->bytes64, len); // non atomic operation
 *   u64_stats_inc(&stats->packets64);    // non atomic operation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *   u64_stats_update_end(&stats->syncp);
 *
 * While a consumer (reader) should use following template to get consistent
 * snapshot for each variable (but no guarantee on several ones)
 *
 * u64 tbytes, tpackets;
 * unsigned int start;
 *
 * do {
 *         start = u64_stats_fetch_begin(&stats->syncp);
<<<<<<< HEAD
 *         tbytes = stats->bytes64; // non atomic operation
 *         tpackets = stats->packets64; // non atomic operation
=======
 *         tbytes = u64_stats_read(&stats->bytes64); // non atomic operation
 *         tpackets = u64_stats_read(&stats->packets64); // non atomic operation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * } while (u64_stats_fetch_retry(&stats->syncp, start));
 *
 *
 * Example of use in drivers/net/loopback.c, using per_cpu containers,
 * in BH disabled context.
 */
#include <linux/seqlock.h>

struct u64_stats_sync {
<<<<<<< HEAD
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
=======
#if BITS_PER_LONG == 32
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	seqcount_t	seq;
#endif
};

<<<<<<< HEAD

#if BITS_PER_LONG == 32 && defined(CONFIG_SMP)
# define u64_stats_init(syncp)	seqcount_init(syncp.seq)
#else
# define u64_stats_init(syncp)	do { } while (0)
#endif

static inline void u64_stats_update_begin(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_begin(&syncp->seq);
#endif
=======
#if BITS_PER_LONG == 64
#include <asm/local64.h>

typedef struct {
	local64_t	v;
} u64_stats_t ;

static inline u64 u64_stats_read(const u64_stats_t *p)
{
	return local64_read(&p->v);
}

static inline void u64_stats_set(u64_stats_t *p, u64 val)
{
	local64_set(&p->v, val);
}

static inline void u64_stats_add(u64_stats_t *p, unsigned long val)
{
	local64_add(val, &p->v);
}

static inline void u64_stats_inc(u64_stats_t *p)
{
	local64_inc(&p->v);
}

static inline void u64_stats_init(struct u64_stats_sync *syncp) { }
static inline void __u64_stats_update_begin(struct u64_stats_sync *syncp) { }
static inline void __u64_stats_update_end(struct u64_stats_sync *syncp) { }
static inline unsigned long __u64_stats_irqsave(void) { return 0; }
static inline void __u64_stats_irqrestore(unsigned long flags) { }
static inline unsigned int __u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
	return 0;
}
static inline bool __u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
					   unsigned int start)
{
	return false;
}

#else /* 64 bit */

typedef struct {
	u64		v;
} u64_stats_t;

static inline u64 u64_stats_read(const u64_stats_t *p)
{
	return p->v;
}

static inline void u64_stats_set(u64_stats_t *p, u64 val)
{
	p->v = val;
}

static inline void u64_stats_add(u64_stats_t *p, unsigned long val)
{
	p->v += val;
}

static inline void u64_stats_inc(u64_stats_t *p)
{
	p->v++;
}

#define u64_stats_init(syncp)				\
	do {						\
		struct u64_stats_sync *__s = (syncp);	\
		seqcount_init(&__s->seq);		\
	} while (0)

static inline void __u64_stats_update_begin(struct u64_stats_sync *syncp)
{
	preempt_disable_nested();
	write_seqcount_begin(&syncp->seq);
}

static inline void __u64_stats_update_end(struct u64_stats_sync *syncp)
{
	write_seqcount_end(&syncp->seq);
	preempt_enable_nested();
}

static inline unsigned long __u64_stats_irqsave(void)
{
	unsigned long flags;

	local_irq_save(flags);
	return flags;
}

static inline void __u64_stats_irqrestore(unsigned long flags)
{
	local_irq_restore(flags);
}

static inline unsigned int __u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
	return read_seqcount_begin(&syncp->seq);
}

static inline bool __u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
					   unsigned int start)
{
	return read_seqcount_retry(&syncp->seq, start);
}
#endif /* !64 bit */

static inline void u64_stats_update_begin(struct u64_stats_sync *syncp)
{
	__u64_stats_update_begin(syncp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void u64_stats_update_end(struct u64_stats_sync *syncp)
{
<<<<<<< HEAD
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_end(&syncp->seq);
#endif
=======
	__u64_stats_update_end(syncp);
}

static inline unsigned long u64_stats_update_begin_irqsave(struct u64_stats_sync *syncp)
{
	unsigned long flags = __u64_stats_irqsave();

	__u64_stats_update_begin(syncp);
	return flags;
}

static inline void u64_stats_update_end_irqrestore(struct u64_stats_sync *syncp,
						   unsigned long flags)
{
	__u64_stats_update_end(syncp);
	__u64_stats_irqrestore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline unsigned int u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
<<<<<<< HEAD
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_begin(&syncp->seq);
#else
#if BITS_PER_LONG==32
	preempt_disable();
#endif
	return 0;
#endif
=======
	return __u64_stats_fetch_begin(syncp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline bool u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
					 unsigned int start)
{
<<<<<<< HEAD
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_retry(&syncp->seq, start);
#else
#if BITS_PER_LONG==32
	preempt_enable();
#endif
	return false;
#endif
}

/*
 * In case irq handlers can update u64 counters, readers can use following helpers
 * - SMP 32bit arches use seqcount protection, irq safe.
 * - UP 32bit must disable irqs.
 * - 64bit have no problem atomically reading u64 values, irq safe.
 */
static inline unsigned int u64_stats_fetch_begin_irq(const struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_begin(&syncp->seq);
#else
#if BITS_PER_LONG==32
	local_irq_disable();
#endif
	return 0;
#endif
}

static inline bool u64_stats_fetch_retry_irq(const struct u64_stats_sync *syncp,
					 unsigned int start)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	return read_seqcount_retry(&syncp->seq, start);
#else
#if BITS_PER_LONG==32
	local_irq_enable();
#endif
	return false;
#endif
=======
	return __u64_stats_fetch_retry(syncp, start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif /* _LINUX_U64_STATS_SYNC_H */
