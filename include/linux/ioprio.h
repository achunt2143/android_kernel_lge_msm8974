<<<<<<< HEAD
#ifndef IOPRIO_H
#define IOPRIO_H
#ifdef __KERNEL__
#include <linux/sched.h>
#include <linux/iocontext.h>
#endif /* __KERNEL__ */

/*
 * Gives us 8 prio classes with 13-bits of data for each class
 */
#define IOPRIO_BITS		(16)
#define IOPRIO_CLASS_SHIFT	(13)
#ifdef __KERNEL__
#define IOPRIO_PRIO_MASK	((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask)	((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask)	((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data)	(((class) << IOPRIO_CLASS_SHIFT) | data)

#define ioprio_valid(mask)	(IOPRIO_PRIO_CLASS((mask)) != IOPRIO_CLASS_NONE)
#endif /* __KERNEL__ */

/*
 * These are the io priority groups as implemented by CFQ. RT is the realtime
 * class, it always gets premium service. BE is the best-effort scheduling
 * class, the default for any process. IDLE is the idle scheduling class, it
 * is only served when no one else is using the disk.
 */
enum {
	IOPRIO_CLASS_NONE,
	IOPRIO_CLASS_RT,
	IOPRIO_CLASS_BE,
	IOPRIO_CLASS_IDLE,
};

/*
 * 8 best effort priority levels are supported
 */
#define IOPRIO_BE_NR	(8)

enum {
	IOPRIO_WHO_PROCESS = 1,
	IOPRIO_WHO_PGRP,
	IOPRIO_WHO_USER,
};

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef IOPRIO_H
#define IOPRIO_H

#include <linux/sched.h>
#include <linux/sched/rt.h>
#include <linux/iocontext.h>

#include <uapi/linux/ioprio.h>

/*
 * Default IO priority.
 */
#define IOPRIO_DEFAULT	IOPRIO_PRIO_VALUE(IOPRIO_CLASS_NONE, 0)

/*
 * Check that a priority value has a valid class.
 */
static inline bool ioprio_valid(unsigned short ioprio)
{
	unsigned short class = IOPRIO_PRIO_CLASS(ioprio);

	return class > IOPRIO_CLASS_NONE && class <= IOPRIO_CLASS_IDLE;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * if process has set io priority explicitly, use that. if not, convert
 * the cpu scheduler nice value to an io priority
 */
<<<<<<< HEAD
#define IOPRIO_NORM	(4)
static inline int task_ioprio(struct io_context *ioc)
{
	if (ioprio_valid(ioc->ioprio))
		return IOPRIO_PRIO_DATA(ioc->ioprio);

	return IOPRIO_NORM;
}

static inline int task_ioprio_class(struct io_context *ioc)
{
	if (ioprio_valid(ioc->ioprio))
		return IOPRIO_PRIO_CLASS(ioc->ioprio);

	return IOPRIO_CLASS_BE;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int task_nice_ioprio(struct task_struct *task)
{
	return (task_nice(task) + 20) / 5;
}

/*
 * This is for the case where the task hasn't asked for a specific IO class.
 * Check for idle and rt task process, and return appropriate IO class.
 */
static inline int task_nice_ioclass(struct task_struct *task)
{
	if (task->policy == SCHED_IDLE)
		return IOPRIO_CLASS_IDLE;
<<<<<<< HEAD
	else if (task->policy == SCHED_FIFO || task->policy == SCHED_RR)
=======
	else if (task_is_realtime(task))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return IOPRIO_CLASS_RT;
	else
		return IOPRIO_CLASS_BE;
}

<<<<<<< HEAD
/*
 * For inheritance, return the highest of the two given priorities
 */
extern int ioprio_best(unsigned short aprio, unsigned short bprio);

extern int set_task_ioprio(struct task_struct *task, int ioprio);

#endif /* __KERNEL__ */
#endif /* IOPRIO_H */
=======
#ifdef CONFIG_BLOCK
/*
 * If the task has set an I/O priority, use that. Otherwise, return
 * the default I/O priority.
 *
 * Expected to be called for current task or with task_lock() held to keep
 * io_context stable.
 */
static inline int __get_task_ioprio(struct task_struct *p)
{
	struct io_context *ioc = p->io_context;
	int prio;

	if (!ioc)
		return IOPRIO_DEFAULT;

	if (p != current)
		lockdep_assert_held(&p->alloc_lock);

	prio = ioc->ioprio;
	if (IOPRIO_PRIO_CLASS(prio) == IOPRIO_CLASS_NONE)
		prio = IOPRIO_PRIO_VALUE(task_nice_ioclass(p),
					 task_nice_ioprio(p));
	return prio;
}
#else
static inline int __get_task_ioprio(struct task_struct *p)
{
	return IOPRIO_DEFAULT;
}
#endif /* CONFIG_BLOCK */

static inline int get_current_ioprio(void)
{
	return __get_task_ioprio(current);
}

extern int set_task_ioprio(struct task_struct *task, int ioprio);

#ifdef CONFIG_BLOCK
extern int ioprio_check_cap(int ioprio);
#else
static inline int ioprio_check_cap(int ioprio)
{
	return -ENOTBLK;
}
#endif /* CONFIG_BLOCK */

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
