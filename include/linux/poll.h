<<<<<<< HEAD
#ifndef _LINUX_POLL_H
#define _LINUX_POLL_H

#include <asm/poll.h>

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_POLL_H
#define _LINUX_POLL_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/compiler.h>
#include <linux/ktime.h>
#include <linux/wait.h>
#include <linux/string.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/sysctl.h>
#include <asm/uaccess.h>

extern struct ctl_table epoll_table[]; /* for sysctl */
=======
#include <linux/uaccess.h>
#include <uapi/linux/poll.h>
#include <uapi/linux/eventpoll.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* ~832 bytes of stack space used max in sys_select/sys_poll before allocating
   additional memory. */
#define MAX_STACK_ALLOC 832
#define FRONTEND_STACK_ALLOC	256
#define SELECT_STACK_ALLOC	FRONTEND_STACK_ALLOC
#define POLL_STACK_ALLOC	FRONTEND_STACK_ALLOC
#define WQUEUES_STACK_ALLOC	(MAX_STACK_ALLOC - FRONTEND_STACK_ALLOC)
#define N_INLINE_POLL_ENTRIES	(WQUEUES_STACK_ALLOC / sizeof(struct poll_table_entry))

<<<<<<< HEAD
#define DEFAULT_POLLMASK (POLLIN | POLLOUT | POLLRDNORM | POLLWRNORM)
=======
#define DEFAULT_POLLMASK (EPOLLIN | EPOLLOUT | EPOLLRDNORM | EPOLLWRNORM)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct poll_table_struct;

/* 
 * structures and helpers for f_op->poll implementations
 */
typedef void (*poll_queue_proc)(struct file *, wait_queue_head_t *, struct poll_table_struct *);

/*
 * Do not touch the structure directly, use the access functions
 * poll_does_not_wait() and poll_requested_events() instead.
 */
typedef struct poll_table_struct {
	poll_queue_proc _qproc;
<<<<<<< HEAD
	unsigned long _key;
=======
	__poll_t _key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} poll_table;

static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)
{
	if (p && p->_qproc && wait_address)
		p->_qproc(filp, wait_address, p);
}

/*
 * Return true if it is guaranteed that poll will not wait. This is the case
 * if the poll() of another file descriptor in the set got an event, so there
 * is no need for waiting.
 */
static inline bool poll_does_not_wait(const poll_table *p)
{
	return p == NULL || p->_qproc == NULL;
}

/*
 * Return the set of events that the application wants to poll for.
 * This is useful for drivers that need to know whether a DMA transfer has
 * to be started implicitly on poll(). You typically only want to do that
 * if the application is actually polling for POLLIN and/or POLLOUT.
 */
<<<<<<< HEAD
static inline unsigned long poll_requested_events(const poll_table *p)
{
	return p ? p->_key : ~0UL;
=======
static inline __poll_t poll_requested_events(const poll_table *p)
{
	return p ? p->_key : ~(__poll_t)0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void init_poll_funcptr(poll_table *pt, poll_queue_proc qproc)
{
	pt->_qproc = qproc;
<<<<<<< HEAD
	pt->_key   = ~0UL; /* all events enabled */
=======
	pt->_key   = ~(__poll_t)0; /* all events enabled */
}

static inline bool file_can_poll(struct file *file)
{
	return file->f_op->poll;
}

static inline __poll_t vfs_poll(struct file *file, struct poll_table_struct *pt)
{
	if (unlikely(!file->f_op->poll))
		return DEFAULT_POLLMASK;
	return file->f_op->poll(file, pt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct poll_table_entry {
	struct file *filp;
<<<<<<< HEAD
	unsigned long key;
	wait_queue_t wait;
=======
	__poll_t key;
	wait_queue_entry_t wait;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wait_queue_head_t *wait_address;
};

/*
 * Structures and helpers for select/poll syscall
 */
struct poll_wqueues {
	poll_table pt;
	struct poll_table_page *table;
	struct task_struct *polling_task;
	int triggered;
	int error;
	int inline_index;
	struct poll_table_entry inline_entries[N_INLINE_POLL_ENTRIES];
};

extern void poll_initwait(struct poll_wqueues *pwq);
extern void poll_freewait(struct poll_wqueues *pwq);
<<<<<<< HEAD
extern int poll_schedule_timeout(struct poll_wqueues *pwq, int state,
				 ktime_t *expires, unsigned long slack);
extern long select_estimate_accuracy(struct timespec *tv);


static inline int poll_schedule(struct poll_wqueues *pwq, int state)
{
	return poll_schedule_timeout(pwq, state, NULL, 0);
}

/*
 * Scalable version of the fd_set.
 */

typedef struct {
	unsigned long *in, *out, *ex;
	unsigned long *res_in, *res_out, *res_ex;
} fd_set_bits;

/*
 * How many longwords for "nr" bits?
 */
#define FDS_BITPERLONG	(8*sizeof(long))
#define FDS_LONGS(nr)	(((nr)+FDS_BITPERLONG-1)/FDS_BITPERLONG)
#define FDS_BYTES(nr)	(FDS_LONGS(nr)*sizeof(long))

/*
 * We do a VERIFY_WRITE here even though we are only reading this time:
 * we'll write to it eventually..
 *
 * Use "unsigned long" accesses to let user-mode fd_set's be long-aligned.
 */
static inline
int get_fd_set(unsigned long nr, void __user *ufdset, unsigned long *fdset)
{
	nr = FDS_BYTES(nr);
	if (ufdset)
		return copy_from_user(fdset, ufdset, nr) ? -EFAULT : 0;

	memset(fdset, 0, nr);
	return 0;
}

static inline unsigned long __must_check
set_fd_set(unsigned long nr, void __user *ufdset, unsigned long *fdset)
{
	if (ufdset)
		return __copy_to_user(ufdset, fdset, FDS_BYTES(nr));
	return 0;
}

static inline
void zero_fd_set(unsigned long nr, unsigned long *fdset)
{
	memset(fdset, 0, FDS_BYTES(nr));
}

#define MAX_INT64_SECONDS (((s64)(~((u64)0)>>1)/HZ)-1)

extern int do_select(int n, fd_set_bits *fds, struct timespec *end_time);
extern int do_sys_poll(struct pollfd __user * ufds, unsigned int nfds,
		       struct timespec *end_time);
extern int core_sys_select(int n, fd_set __user *inp, fd_set __user *outp,
			   fd_set __user *exp, struct timespec *end_time);

extern int poll_select_set_timeout(struct timespec *to, long sec, long nsec);

#endif /* KERNEL */
=======
extern u64 select_estimate_accuracy(struct timespec64 *tv);

#define MAX_INT64_SECONDS (((s64)(~((u64)0)>>1)/HZ)-1)

extern int core_sys_select(int n, fd_set __user *inp, fd_set __user *outp,
			   fd_set __user *exp, struct timespec64 *end_time);

extern int poll_select_set_timeout(struct timespec64 *to, time64_t sec,
				   long nsec);

#define __MAP(v, from, to) \
	(from < to ? (v & from) * (to/from) : (v & from) / (from/to))

static inline __u16 mangle_poll(__poll_t val)
{
	__u16 v = (__force __u16)val;
#define M(X) __MAP(v, (__force __u16)EPOLL##X, POLL##X)
	return M(IN) | M(OUT) | M(PRI) | M(ERR) | M(NVAL) |
		M(RDNORM) | M(RDBAND) | M(WRNORM) | M(WRBAND) |
		M(HUP) | M(RDHUP) | M(MSG);
#undef M
}

static inline __poll_t demangle_poll(u16 val)
{
#define M(X) (__force __poll_t)__MAP(val, POLL##X, (__force __u16)EPOLL##X)
	return M(IN) | M(OUT) | M(PRI) | M(ERR) | M(NVAL) |
		M(RDNORM) | M(RDBAND) | M(WRNORM) | M(WRBAND) |
		M(HUP) | M(RDHUP) | M(MSG);
#undef M
}
#undef __MAP

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_POLL_H */
