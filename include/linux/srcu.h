<<<<<<< HEAD
/*
 * Sleepable Read-Copy Update mechanism for mutual exclusion
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2006
 *
 * Author: Paul McKenney <paulmck@us.ibm.com>
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 * 		Documentation/RCU/ *.txt
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Sleepable Read-Copy Update mechanism for mutual exclusion
 *
 * Copyright (C) IBM Corporation, 2006
 * Copyright (C) Fujitsu, 2012
 *
 * Author: Paul McKenney <paulmck@linux.ibm.com>
 *	   Lai Jiangshan <laijs@cn.fujitsu.com>
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *		Documentation/RCU/ *.txt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 */

#ifndef _LINUX_SRCU_H
#define _LINUX_SRCU_H

#include <linux/mutex.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD

struct srcu_struct_array {
	int c[2];
};

struct srcu_struct {
	int completed;
	struct srcu_struct_array __percpu *per_cpu_ref;
	struct mutex mutex;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
};

#ifndef CONFIG_PREEMPT
#define srcu_barrier() barrier()
#else /* #ifndef CONFIG_PREEMPT */
#define srcu_barrier()
#endif /* #else #ifndef CONFIG_PREEMPT */

#ifdef CONFIG_DEBUG_LOCK_ALLOC

int __init_srcu_struct(struct srcu_struct *sp, const char *name,
		       struct lock_class_key *key);

#define init_srcu_struct(sp) \
({ \
	static struct lock_class_key __srcu_key; \
	\
	__init_srcu_struct((sp), #sp, &__srcu_key); \
})

#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */

int init_srcu_struct(struct srcu_struct *sp);

#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

void cleanup_srcu_struct(struct srcu_struct *sp);
int __srcu_read_lock(struct srcu_struct *sp) __acquires(sp);
void __srcu_read_unlock(struct srcu_struct *sp, int idx) __releases(sp);
void synchronize_srcu(struct srcu_struct *sp);
void synchronize_srcu_expedited(struct srcu_struct *sp);
long srcu_batches_completed(struct srcu_struct *sp);
=======
#include <linux/workqueue.h>
#include <linux/rcu_segcblist.h>

struct srcu_struct;

#ifdef CONFIG_DEBUG_LOCK_ALLOC

int __init_srcu_struct(struct srcu_struct *ssp, const char *name,
		       struct lock_class_key *key);

#define init_srcu_struct(ssp) \
({ \
	static struct lock_class_key __srcu_key; \
	\
	__init_srcu_struct((ssp), #ssp, &__srcu_key); \
})

#define __SRCU_DEP_MAP_INIT(srcu_name)	.dep_map = { .name = #srcu_name },
#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */

int init_srcu_struct(struct srcu_struct *ssp);

#define __SRCU_DEP_MAP_INIT(srcu_name)
#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

#ifdef CONFIG_TINY_SRCU
#include <linux/srcutiny.h>
#elif defined(CONFIG_TREE_SRCU)
#include <linux/srcutree.h>
#else
#error "Unknown SRCU implementation specified to kernel configuration"
#endif

void call_srcu(struct srcu_struct *ssp, struct rcu_head *head,
		void (*func)(struct rcu_head *head));
void cleanup_srcu_struct(struct srcu_struct *ssp);
int __srcu_read_lock(struct srcu_struct *ssp) __acquires(ssp);
void __srcu_read_unlock(struct srcu_struct *ssp, int idx) __releases(ssp);
void synchronize_srcu(struct srcu_struct *ssp);
unsigned long get_state_synchronize_srcu(struct srcu_struct *ssp);
unsigned long start_poll_synchronize_srcu(struct srcu_struct *ssp);
bool poll_state_synchronize_srcu(struct srcu_struct *ssp, unsigned long cookie);

#ifdef CONFIG_NEED_SRCU_NMI_SAFE
int __srcu_read_lock_nmisafe(struct srcu_struct *ssp) __acquires(ssp);
void __srcu_read_unlock_nmisafe(struct srcu_struct *ssp, int idx) __releases(ssp);
#else
static inline int __srcu_read_lock_nmisafe(struct srcu_struct *ssp)
{
	return __srcu_read_lock(ssp);
}
static inline void __srcu_read_unlock_nmisafe(struct srcu_struct *ssp, int idx)
{
	__srcu_read_unlock(ssp, idx);
}
#endif /* CONFIG_NEED_SRCU_NMI_SAFE */

void srcu_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_DEBUG_LOCK_ALLOC

/**
 * srcu_read_lock_held - might we be in SRCU read-side critical section?
<<<<<<< HEAD
=======
 * @ssp: The srcu_struct structure to check
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If CONFIG_DEBUG_LOCK_ALLOC is selected, returns nonzero iff in an SRCU
 * read-side critical section.  In absence of CONFIG_DEBUG_LOCK_ALLOC,
 * this assumes we are in an SRCU read-side critical section unless it can
 * prove otherwise.
 *
 * Checks debug_lockdep_rcu_enabled() to prevent false positives during boot
 * and while lockdep is disabled.
 *
<<<<<<< HEAD
 * Note that if the CPU is in the idle loop from an RCU point of view
 * (ie: that we are in the section between rcu_idle_enter() and
 * rcu_idle_exit()) then srcu_read_lock_held() returns false even if
 * the CPU did an srcu_read_lock().  The reason for this is that RCU
 * ignores CPUs that are in such a section, considering these as in
 * extended quiescent state, so such a CPU is effectively never in an
 * RCU read-side critical section regardless of what RCU primitives it
 * invokes.  This state of affairs is required --- we need to keep an
 * RCU-free window in idle where the CPU may possibly enter into low
 * power mode. This way we can notice an extended quiescent state to
 * other CPUs that started a grace period. Otherwise we would delay any
 * grace period as long as we run in the idle task.
 *
 * Similarly, we avoid claiming an SRCU read lock held if the current
 * CPU is offline.
 */
static inline int srcu_read_lock_held(struct srcu_struct *sp)
{
	if (!debug_lockdep_rcu_enabled())
		return 1;
	if (rcu_is_cpu_idle())
		return 0;
	if (!rcu_lockdep_current_cpu_online())
		return 0;
	return lock_is_held(&sp->dep_map);
=======
 * Note that SRCU is based on its own statemachine and it doesn't
 * relies on normal RCU, it can be called from the CPU which
 * is in the idle loop from an RCU point of view or offline.
 */
static inline int srcu_read_lock_held(const struct srcu_struct *ssp)
{
	if (!debug_lockdep_rcu_enabled())
		return 1;
	return lock_is_held(&ssp->dep_map);
}

/*
 * Annotations provide deadlock detection for SRCU.
 *
 * Similar to other lockdep annotations, except there is an additional
 * srcu_lock_sync(), which is basically an empty *write*-side critical section,
 * see lock_sync() for more information.
 */

/* Annotates a srcu_read_lock() */
static inline void srcu_lock_acquire(struct lockdep_map *map)
{
	lock_map_acquire_read(map);
}

/* Annotates a srcu_read_lock() */
static inline void srcu_lock_release(struct lockdep_map *map)
{
	lock_map_release(map);
}

/* Annotates a synchronize_srcu() */
static inline void srcu_lock_sync(struct lockdep_map *map)
{
	lock_map_sync(map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */

<<<<<<< HEAD
static inline int srcu_read_lock_held(struct srcu_struct *sp)
=======
static inline int srcu_read_lock_held(const struct srcu_struct *ssp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 1;
}

<<<<<<< HEAD
#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

/**
 * srcu_dereference_check - fetch SRCU-protected pointer for later dereferencing
 * @p: the pointer to fetch and protect for later dereferencing
 * @sp: pointer to the srcu_struct, which is used to check that we
=======
#define srcu_lock_acquire(m) do { } while (0)
#define srcu_lock_release(m) do { } while (0)
#define srcu_lock_sync(m) do { } while (0)

#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

#define SRCU_NMI_UNKNOWN	0x0
#define SRCU_NMI_UNSAFE		0x1
#define SRCU_NMI_SAFE		0x2

#if defined(CONFIG_PROVE_RCU) && defined(CONFIG_TREE_SRCU)
void srcu_check_nmi_safety(struct srcu_struct *ssp, bool nmi_safe);
#else
static inline void srcu_check_nmi_safety(struct srcu_struct *ssp,
					 bool nmi_safe) { }
#endif


/**
 * srcu_dereference_check - fetch SRCU-protected pointer for later dereferencing
 * @p: the pointer to fetch and protect for later dereferencing
 * @ssp: pointer to the srcu_struct, which is used to check that we
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	really are in an SRCU read-side critical section.
 * @c: condition to check for update-side use
 *
 * If PROVE_RCU is enabled, invoking this outside of an RCU read-side
 * critical section will result in an RCU-lockdep splat, unless @c evaluates
 * to 1.  The @c argument will normally be a logical expression containing
 * lockdep_is_held() calls.
 */
<<<<<<< HEAD
#define srcu_dereference_check(p, sp, c) \
	__rcu_dereference_check((p), srcu_read_lock_held(sp) || (c), __rcu)
=======
#define srcu_dereference_check(p, ssp, c) \
	__rcu_dereference_check((p), __UNIQUE_ID(rcu), \
				(c) || srcu_read_lock_held(ssp), __rcu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * srcu_dereference - fetch SRCU-protected pointer for later dereferencing
 * @p: the pointer to fetch and protect for later dereferencing
<<<<<<< HEAD
 * @sp: pointer to the srcu_struct, which is used to check that we
=======
 * @ssp: pointer to the srcu_struct, which is used to check that we
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	really are in an SRCU read-side critical section.
 *
 * Makes rcu_dereference_check() do the dirty work.  If PROVE_RCU
 * is enabled, invoking this outside of an RCU read-side critical
 * section will result in an RCU-lockdep splat.
 */
<<<<<<< HEAD
#define srcu_dereference(p, sp) srcu_dereference_check((p), (sp), 0)

/**
 * srcu_read_lock - register a new reader for an SRCU-protected structure.
 * @sp: srcu_struct in which to register the new reader.
=======
#define srcu_dereference(p, ssp) srcu_dereference_check((p), (ssp), 0)

/**
 * srcu_dereference_notrace - no tracing and no lockdep calls from here
 * @p: the pointer to fetch and protect for later dereferencing
 * @ssp: pointer to the srcu_struct, which is used to check that we
 *	really are in an SRCU read-side critical section.
 */
#define srcu_dereference_notrace(p, ssp) srcu_dereference_check((p), (ssp), 1)

/**
 * srcu_read_lock - register a new reader for an SRCU-protected structure.
 * @ssp: srcu_struct in which to register the new reader.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Enter an SRCU read-side critical section.  Note that SRCU read-side
 * critical sections may be nested.  However, it is illegal to
 * call anything that waits on an SRCU grace period for the same
 * srcu_struct, whether directly or indirectly.  Please note that
 * one way to indirectly wait on an SRCU grace period is to acquire
 * a mutex that is held elsewhere while calling synchronize_srcu() or
 * synchronize_srcu_expedited().
 *
 * Note that srcu_read_lock() and the matching srcu_read_unlock() must
 * occur in the same context, for example, it is illegal to invoke
 * srcu_read_unlock() in an irq handler if the matching srcu_read_lock()
 * was invoked in process context.
 */
<<<<<<< HEAD
static inline int srcu_read_lock(struct srcu_struct *sp) __acquires(sp)
{
	int retval = __srcu_read_lock(sp);

	rcu_lock_acquire(&(sp)->dep_map);
	rcu_lockdep_assert(!rcu_is_cpu_idle(),
			   "srcu_read_lock() used illegally while idle");
=======
static inline int srcu_read_lock(struct srcu_struct *ssp) __acquires(ssp)
{
	int retval;

	srcu_check_nmi_safety(ssp, false);
	retval = __srcu_read_lock(ssp);
	srcu_lock_acquire(&ssp->dep_map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/**
<<<<<<< HEAD
 * srcu_read_unlock - unregister a old reader from an SRCU-protected structure.
 * @sp: srcu_struct in which to unregister the old reader.
=======
 * srcu_read_lock_nmisafe - register a new reader for an SRCU-protected structure.
 * @ssp: srcu_struct in which to register the new reader.
 *
 * Enter an SRCU read-side critical section, but in an NMI-safe manner.
 * See srcu_read_lock() for more information.
 */
static inline int srcu_read_lock_nmisafe(struct srcu_struct *ssp) __acquires(ssp)
{
	int retval;

	srcu_check_nmi_safety(ssp, true);
	retval = __srcu_read_lock_nmisafe(ssp);
	rcu_try_lock_acquire(&ssp->dep_map);
	return retval;
}

/* Used by tracing, cannot be traced and cannot invoke lockdep. */
static inline notrace int
srcu_read_lock_notrace(struct srcu_struct *ssp) __acquires(ssp)
{
	int retval;

	srcu_check_nmi_safety(ssp, false);
	retval = __srcu_read_lock(ssp);
	return retval;
}

/**
 * srcu_down_read - register a new reader for an SRCU-protected structure.
 * @ssp: srcu_struct in which to register the new reader.
 *
 * Enter a semaphore-like SRCU read-side critical section.  Note that
 * SRCU read-side critical sections may be nested.  However, it is
 * illegal to call anything that waits on an SRCU grace period for the
 * same srcu_struct, whether directly or indirectly.  Please note that
 * one way to indirectly wait on an SRCU grace period is to acquire
 * a mutex that is held elsewhere while calling synchronize_srcu() or
 * synchronize_srcu_expedited().  But if you want lockdep to help you
 * keep this stuff straight, you should instead use srcu_read_lock().
 *
 * The semaphore-like nature of srcu_down_read() means that the matching
 * srcu_up_read() can be invoked from some other context, for example,
 * from some other task or from an irq handler.  However, neither
 * srcu_down_read() nor srcu_up_read() may be invoked from an NMI handler.
 *
 * Calls to srcu_down_read() may be nested, similar to the manner in
 * which calls to down_read() may be nested.
 */
static inline int srcu_down_read(struct srcu_struct *ssp) __acquires(ssp)
{
	WARN_ON_ONCE(in_nmi());
	srcu_check_nmi_safety(ssp, false);
	return __srcu_read_lock(ssp);
}

/**
 * srcu_read_unlock - unregister a old reader from an SRCU-protected structure.
 * @ssp: srcu_struct in which to unregister the old reader.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @idx: return value from corresponding srcu_read_lock().
 *
 * Exit an SRCU read-side critical section.
 */
<<<<<<< HEAD
static inline void srcu_read_unlock(struct srcu_struct *sp, int idx)
	__releases(sp)
{
	rcu_lockdep_assert(!rcu_is_cpu_idle(),
			   "srcu_read_unlock() used illegally while idle");
	rcu_lock_release(&(sp)->dep_map);
	__srcu_read_unlock(sp, idx);
}

/**
 * srcu_read_lock_raw - register a new reader for an SRCU-protected structure.
 * @sp: srcu_struct in which to register the new reader.
 *
 * Enter an SRCU read-side critical section.  Similar to srcu_read_lock(),
 * but avoids the RCU-lockdep checking.  This means that it is legal to
 * use srcu_read_lock_raw() in one context, for example, in an exception
 * handler, and then have the matching srcu_read_unlock_raw() in another
 * context, for example in the task that took the exception.
 *
 * However, the entire SRCU read-side critical section must reside within a
 * single task.  For example, beware of using srcu_read_lock_raw() in
 * a device interrupt handler and srcu_read_unlock() in the interrupted
 * task:  This will not work if interrupts are threaded.
 */
static inline int srcu_read_lock_raw(struct srcu_struct *sp)
{
	unsigned long flags;
	int ret;

	local_irq_save(flags);
	ret =  __srcu_read_lock(sp);
	local_irq_restore(flags);
	return ret;
}

/**
 * srcu_read_unlock_raw - unregister reader from an SRCU-protected structure.
 * @sp: srcu_struct in which to unregister the old reader.
 * @idx: return value from corresponding srcu_read_lock_raw().
 *
 * Exit an SRCU read-side critical section without lockdep-RCU checking.
 * See srcu_read_lock_raw() for more details.
 */
static inline void srcu_read_unlock_raw(struct srcu_struct *sp, int idx)
{
	unsigned long flags;

	local_irq_save(flags);
	__srcu_read_unlock(sp, idx);
	local_irq_restore(flags);
}

=======
static inline void srcu_read_unlock(struct srcu_struct *ssp, int idx)
	__releases(ssp)
{
	WARN_ON_ONCE(idx & ~0x1);
	srcu_check_nmi_safety(ssp, false);
	srcu_lock_release(&ssp->dep_map);
	__srcu_read_unlock(ssp, idx);
}

/**
 * srcu_read_unlock_nmisafe - unregister a old reader from an SRCU-protected structure.
 * @ssp: srcu_struct in which to unregister the old reader.
 * @idx: return value from corresponding srcu_read_lock().
 *
 * Exit an SRCU read-side critical section, but in an NMI-safe manner.
 */
static inline void srcu_read_unlock_nmisafe(struct srcu_struct *ssp, int idx)
	__releases(ssp)
{
	WARN_ON_ONCE(idx & ~0x1);
	srcu_check_nmi_safety(ssp, true);
	rcu_lock_release(&ssp->dep_map);
	__srcu_read_unlock_nmisafe(ssp, idx);
}

/* Used by tracing, cannot be traced and cannot call lockdep. */
static inline notrace void
srcu_read_unlock_notrace(struct srcu_struct *ssp, int idx) __releases(ssp)
{
	srcu_check_nmi_safety(ssp, false);
	__srcu_read_unlock(ssp, idx);
}

/**
 * srcu_up_read - unregister a old reader from an SRCU-protected structure.
 * @ssp: srcu_struct in which to unregister the old reader.
 * @idx: return value from corresponding srcu_read_lock().
 *
 * Exit an SRCU read-side critical section, but not necessarily from
 * the same context as the maching srcu_down_read().
 */
static inline void srcu_up_read(struct srcu_struct *ssp, int idx)
	__releases(ssp)
{
	WARN_ON_ONCE(idx & ~0x1);
	WARN_ON_ONCE(in_nmi());
	srcu_check_nmi_safety(ssp, false);
	__srcu_read_unlock(ssp, idx);
}

/**
 * smp_mb__after_srcu_read_unlock - ensure full ordering after srcu_read_unlock
 *
 * Converts the preceding srcu_read_unlock into a two-way memory barrier.
 *
 * Call this after srcu_read_unlock, to guarantee that all memory operations
 * that occur after smp_mb__after_srcu_read_unlock will appear to happen after
 * the preceding srcu_read_unlock.
 */
static inline void smp_mb__after_srcu_read_unlock(void)
{
	/* __srcu_read_unlock has smp_mb() internally so nothing to do here. */
}

DEFINE_LOCK_GUARD_1(srcu, struct srcu_struct,
		    _T->idx = srcu_read_lock(_T->lock),
		    srcu_read_unlock(_T->lock, _T->idx),
		    int idx)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
