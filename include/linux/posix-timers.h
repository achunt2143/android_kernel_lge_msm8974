<<<<<<< HEAD
#ifndef _linux_POSIX_TIMERS_H
#define _linux_POSIX_TIMERS_H

#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/timex.h>
#include <linux/alarmtimer.h>

union cpu_time_count {
	cputime_t cpu;
	unsigned long long sched;
};

struct cpu_timer_list {
	struct list_head entry;
	union cpu_time_count expires, incr;
	struct task_struct *task;
	int firing;
};

/*
 * Bit fields within a clockid:
 *
 * The most significant 29 bits hold either a pid or a file descriptor.
 *
 * Bit 2 indicates whether a cpu clock refers to a thread or a process.
 *
 * Bits 1 and 0 give the type: PROF=0, VIRT=1, SCHED=2, or FD=3.
 *
 * A clockid is invalid if bits 2, 1, and 0 are all set.
 */
#define CPUCLOCK_PID(clock)		((pid_t) ~((clock) >> 3))
#define CPUCLOCK_PERTHREAD(clock) \
	(((clock) & (clockid_t) CPUCLOCK_PERTHREAD_MASK) != 0)

#define CPUCLOCK_PERTHREAD_MASK	4
#define CPUCLOCK_WHICH(clock)	((clock) & (clockid_t) CPUCLOCK_CLOCK_MASK)
#define CPUCLOCK_CLOCK_MASK	3
#define CPUCLOCK_PROF		0
#define CPUCLOCK_VIRT		1
#define CPUCLOCK_SCHED		2
#define CPUCLOCK_MAX		3
#define CLOCKFD			CPUCLOCK_MAX
#define CLOCKFD_MASK		(CPUCLOCK_PERTHREAD_MASK|CPUCLOCK_CLOCK_MASK)

#define MAKE_PROCESS_CPUCLOCK(pid, clock) \
	((~(clockid_t) (pid) << 3) | (clockid_t) (clock))
#define MAKE_THREAD_CPUCLOCK(tid, clock) \
	MAKE_PROCESS_CPUCLOCK((tid), (clock) | CPUCLOCK_PERTHREAD_MASK)

#define FD_TO_CLOCKID(fd)	((~(clockid_t) (fd) << 3) | CLOCKFD)
#define CLOCKID_TO_FD(clk)	((unsigned int) ~((clk) >> 3))

/* POSIX.1b interval timer structure. */
struct k_itimer {
	struct list_head list;		/* free/ allocate list */
	spinlock_t it_lock;
	clockid_t it_clock;		/* which timer type */
	timer_t it_id;			/* timer id */
	int it_overrun;			/* overrun on pending signal  */
	int it_overrun_last;		/* overrun on last delivered signal */
	int it_requeue_pending;		/* waiting to requeue this timer */
#define REQUEUE_PENDING 1
	int it_sigev_notify;		/* notify word of sigevent struct */
	struct signal_struct *it_signal;
	union {
		struct pid *it_pid;	/* pid of process to send signal to */
		struct task_struct *it_process;	/* for clock_nanosleep */
	};
	struct sigqueue *sigq;		/* signal queue entry. */
	union {
		struct {
			struct hrtimer timer;
			ktime_t interval;
		} real;
		struct cpu_timer_list cpu;
		struct {
			unsigned int clock;
			unsigned int node;
			unsigned long incr;
			unsigned long expires;
		} mmtimer;
		struct {
			struct alarm alarmtimer;
			ktime_t interval;
		} alarm;
		struct rcu_head rcu;
	} it;
};

struct k_clock {
	int (*clock_getres) (const clockid_t which_clock, struct timespec *tp);
	int (*clock_set) (const clockid_t which_clock,
			  const struct timespec *tp);
	int (*clock_get) (const clockid_t which_clock, struct timespec * tp);
	int (*clock_adj) (const clockid_t which_clock, struct timex *tx);
	int (*timer_create) (struct k_itimer *timer);
	int (*nsleep) (const clockid_t which_clock, int flags,
		       struct timespec *, struct timespec __user *);
	long (*nsleep_restart) (struct restart_block *restart_block);
	int (*timer_set) (struct k_itimer * timr, int flags,
			  struct itimerspec * new_setting,
			  struct itimerspec * old_setting);
	int (*timer_del) (struct k_itimer * timr);
#define TIMER_RETRY 1
	void (*timer_get) (struct k_itimer * timr,
			   struct itimerspec * cur_setting);
};

extern struct k_clock clock_posix_cpu;
extern struct k_clock clock_posix_dynamic;

void posix_timers_register_clock(const clockid_t clock_id, struct k_clock *new_clock);

/* function to call to trigger timer event */
int posix_timer_event(struct k_itimer *timr, int si_private);

void posix_cpu_timer_schedule(struct k_itimer *timer);

void run_posix_cpu_timers(struct task_struct *task);
void posix_cpu_timers_exit(struct task_struct *task);
void posix_cpu_timers_exit_group(struct task_struct *task);

void set_process_cpu_timer(struct task_struct *task, unsigned int clock_idx,
			   cputime_t *newval, cputime_t *oldval);

long clock_nanosleep_restart(struct restart_block *restart_block);

void update_rlimit_cpu(struct task_struct *task, unsigned long rlim_new);

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _linux_POSIX_TIMERS_H
#define _linux_POSIX_TIMERS_H

#include <linux/alarmtimer.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/posix-timers_types.h>
#include <linux/spinlock.h>
#include <linux/timerqueue.h>

struct kernel_siginfo;
struct task_struct;

static inline clockid_t make_process_cpuclock(const unsigned int pid,
		const clockid_t clock)
{
	return ((~pid) << 3) | clock;
}
static inline clockid_t make_thread_cpuclock(const unsigned int tid,
		const clockid_t clock)
{
	return make_process_cpuclock(tid, clock | CPUCLOCK_PERTHREAD_MASK);
}

static inline clockid_t fd_to_clockid(const int fd)
{
	return make_process_cpuclock((unsigned int) fd, CLOCKFD);
}

static inline int clockid_to_fd(const clockid_t clk)
{
	return ~(clk >> 3);
}

#ifdef CONFIG_POSIX_TIMERS

/**
 * cpu_timer - Posix CPU timer representation for k_itimer
 * @node:	timerqueue node to queue in the task/sig
 * @head:	timerqueue head on which this timer is queued
 * @pid:	Pointer to target task PID
 * @elist:	List head for the expiry list
 * @firing:	Timer is currently firing
 * @handling:	Pointer to the task which handles expiry
 */
struct cpu_timer {
	struct timerqueue_node		node;
	struct timerqueue_head		*head;
	struct pid			*pid;
	struct list_head		elist;
	int				firing;
	struct task_struct __rcu	*handling;
};

static inline bool cpu_timer_enqueue(struct timerqueue_head *head,
				     struct cpu_timer *ctmr)
{
	ctmr->head = head;
	return timerqueue_add(head, &ctmr->node);
}

static inline bool cpu_timer_queued(struct cpu_timer *ctmr)
{
	return !!ctmr->head;
}

static inline bool cpu_timer_dequeue(struct cpu_timer *ctmr)
{
	if (cpu_timer_queued(ctmr)) {
		timerqueue_del(ctmr->head, &ctmr->node);
		ctmr->head = NULL;
		return true;
	}
	return false;
}

static inline u64 cpu_timer_getexpires(struct cpu_timer *ctmr)
{
	return ctmr->node.expires;
}

static inline void cpu_timer_setexpires(struct cpu_timer *ctmr, u64 exp)
{
	ctmr->node.expires = exp;
}

static inline void posix_cputimers_init(struct posix_cputimers *pct)
{
	memset(pct, 0, sizeof(*pct));
	pct->bases[0].nextevt = U64_MAX;
	pct->bases[1].nextevt = U64_MAX;
	pct->bases[2].nextevt = U64_MAX;
}

void posix_cputimers_group_init(struct posix_cputimers *pct, u64 cpu_limit);

static inline void posix_cputimers_rt_watchdog(struct posix_cputimers *pct,
					       u64 runtime)
{
	pct->bases[CPUCLOCK_SCHED].nextevt = runtime;
}

/* Init task static initializer */
#define INIT_CPU_TIMERBASE(b) {						\
	.nextevt	= U64_MAX,					\
}

#define INIT_CPU_TIMERBASES(b) {					\
	INIT_CPU_TIMERBASE(b[0]),					\
	INIT_CPU_TIMERBASE(b[1]),					\
	INIT_CPU_TIMERBASE(b[2]),					\
}

#define INIT_CPU_TIMERS(s)						\
	.posix_cputimers = {						\
		.bases = INIT_CPU_TIMERBASES(s.posix_cputimers.bases),	\
	},
#else
struct cpu_timer { };
#define INIT_CPU_TIMERS(s)
static inline void posix_cputimers_init(struct posix_cputimers *pct) { }
static inline void posix_cputimers_group_init(struct posix_cputimers *pct,
					      u64 cpu_limit) { }
#endif

#ifdef CONFIG_POSIX_CPU_TIMERS_TASK_WORK
void clear_posix_cputimers_work(struct task_struct *p);
void posix_cputimers_init_work(void);
#else
static inline void clear_posix_cputimers_work(struct task_struct *p) { }
static inline void posix_cputimers_init_work(void) { }
#endif

#define REQUEUE_PENDING 1

/**
 * struct k_itimer - POSIX.1b interval timer structure.
 * @list:		List head for binding the timer to signals->posix_timers
 * @t_hash:		Entry in the posix timer hash table
 * @it_lock:		Lock protecting the timer
 * @kclock:		Pointer to the k_clock struct handling this timer
 * @it_clock:		The posix timer clock id
 * @it_id:		The posix timer id for identifying the timer
 * @it_active:		Marker that timer is active
 * @it_overrun:		The overrun counter for pending signals
 * @it_overrun_last:	The overrun at the time of the last delivered signal
 * @it_requeue_pending:	Indicator that timer waits for being requeued on
 *			signal delivery
 * @it_sigev_notify:	The notify word of sigevent struct for signal delivery
 * @it_interval:	The interval for periodic timers
 * @it_signal:		Pointer to the creators signal struct
 * @it_pid:		The pid of the process/task targeted by the signal
 * @it_process:		The task to wakeup on clock_nanosleep (CPU timers)
 * @sigq:		Pointer to preallocated sigqueue
 * @it:			Union representing the various posix timer type
 *			internals.
 * @rcu:		RCU head for freeing the timer.
 */
struct k_itimer {
	struct list_head	list;
	struct hlist_node	t_hash;
	spinlock_t		it_lock;
	const struct k_clock	*kclock;
	clockid_t		it_clock;
	timer_t			it_id;
	int			it_active;
	s64			it_overrun;
	s64			it_overrun_last;
	int			it_requeue_pending;
	int			it_sigev_notify;
	ktime_t			it_interval;
	struct signal_struct	*it_signal;
	union {
		struct pid		*it_pid;
		struct task_struct	*it_process;
	};
	struct sigqueue		*sigq;
	union {
		struct {
			struct hrtimer	timer;
		} real;
		struct cpu_timer	cpu;
		struct {
			struct alarm	alarmtimer;
		} alarm;
	} it;
	struct rcu_head		rcu;
};

void run_posix_cpu_timers(void);
void posix_cpu_timers_exit(struct task_struct *task);
void posix_cpu_timers_exit_group(struct task_struct *task);
void set_process_cpu_timer(struct task_struct *task, unsigned int clock_idx,
			   u64 *newval, u64 *oldval);

int update_rlimit_cpu(struct task_struct *task, unsigned long rlim_new);

void posixtimer_rearm(struct kernel_siginfo *info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
