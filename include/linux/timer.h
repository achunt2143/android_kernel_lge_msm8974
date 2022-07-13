<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_TIMER_H
#define _LINUX_TIMER_H

#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/stddef.h>
#include <linux/debugobjects.h>
#include <linux/stringify.h>
<<<<<<< HEAD

struct tvec_base;

struct timer_list {
	/*
	 * All fields that change during normal runtime grouped to the
	 * same cacheline
	 */
	struct list_head entry;
	unsigned long expires;
	struct tvec_base *base;

	void (*function)(unsigned long);
	unsigned long data;

	int slack;

#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;
#endif
};

extern struct tvec_base boot_tvec_bases;
=======
#include <linux/timer_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_LOCKDEP
/*
 * NB: because we have to copy the lockdep_map, setting the lockdep_map key
 * (second argument) here is required, otherwise it could be initialised to
 * the copy of the lockdep_map later! We use the pointer to and the string
 * "<file>:<line>" as the key resp. the name of the lockdep_map.
 */
#define __TIMER_LOCKDEP_MAP_INITIALIZER(_kn)				\
	.lockdep_map = STATIC_LOCKDEP_MAP_INIT(_kn, &_kn),
#else
#define __TIMER_LOCKDEP_MAP_INITIALIZER(_kn)
#endif

/*
<<<<<<< HEAD
 * Note that all tvec_bases are 2 byte aligned and lower bit of
 * base in timer_list is guaranteed to be zero. Use the LSB to
 * indicate whether the timer is deferrable.
 *
 * A deferrable timer will work normally when the system is busy, but
 * will not cause a CPU to come out of idle just to service it; instead,
 * the timer will be serviced when the CPU eventually wakes up with a
 * subsequent non-deferrable timer.
 */
#define TBASE_DEFERRABLE_FLAG		(0x1)

#define TIMER_INITIALIZER(_function, _expires, _data) {		\
		.entry = { .prev = TIMER_ENTRY_STATIC },	\
		.function = (_function),			\
		.expires = (_expires),				\
		.data = (_data),				\
		.base = &boot_tvec_bases,			\
		.slack = -1,					\
		__TIMER_LOCKDEP_MAP_INITIALIZER(		\
			__FILE__ ":" __stringify(__LINE__))	\
	}

#define TBASE_MAKE_DEFERRED(ptr) ((struct tvec_base *)		\
		  ((unsigned char *)(ptr) + TBASE_DEFERRABLE_FLAG))

#define TIMER_DEFERRED_INITIALIZER(_function, _expires, _data) {\
		.entry = { .prev = TIMER_ENTRY_STATIC },	\
		.function = (_function),			\
		.expires = (_expires),				\
		.data = (_data),				\
		.base = TBASE_MAKE_DEFERRED(&boot_tvec_bases),	\
		__TIMER_LOCKDEP_MAP_INITIALIZER(		\
			__FILE__ ":" __stringify(__LINE__))	\
	}

#define DEFINE_TIMER(_name, _function, _expires, _data)		\
	struct timer_list _name =				\
		TIMER_INITIALIZER(_function, _expires, _data)

void init_timer_key(struct timer_list *timer,
		    const char *name,
		    struct lock_class_key *key);
void init_timer_deferrable_key(struct timer_list *timer,
			       const char *name,
			       struct lock_class_key *key);

#ifdef CONFIG_LOCKDEP
#define init_timer(timer)						\
	do {								\
		static struct lock_class_key __key;			\
		init_timer_key((timer), #timer, &__key);		\
	} while (0)

#define init_timer_deferrable(timer)					\
	do {								\
		static struct lock_class_key __key;			\
		init_timer_deferrable_key((timer), #timer, &__key);	\
	} while (0)

#define init_timer_on_stack(timer)					\
	do {								\
		static struct lock_class_key __key;			\
		init_timer_on_stack_key((timer), #timer, &__key);	\
	} while (0)

#define setup_timer(timer, fn, data)					\
	do {								\
		static struct lock_class_key __key;			\
		setup_timer_key((timer), #timer, &__key, (fn), (data));\
	} while (0)

#define setup_timer_on_stack(timer, fn, data)				\
	do {								\
		static struct lock_class_key __key;			\
		setup_timer_on_stack_key((timer), #timer, &__key,	\
					 (fn), (data));			\
	} while (0)
#define setup_deferrable_timer_on_stack(timer, fn, data)		\
	do {								\
		static struct lock_class_key __key;			\
		setup_deferrable_timer_on_stack_key((timer), #timer,	\
						    &__key, (fn),	\
						    (data));		\
	} while (0)
#else
#define init_timer(timer)\
	init_timer_key((timer), NULL, NULL)
#define init_timer_deferrable(timer)\
	init_timer_deferrable_key((timer), NULL, NULL)
#define init_timer_on_stack(timer)\
	init_timer_on_stack_key((timer), NULL, NULL)
#define setup_timer(timer, fn, data)\
	setup_timer_key((timer), NULL, NULL, (fn), (data))
#define setup_timer_on_stack(timer, fn, data)\
	setup_timer_on_stack_key((timer), NULL, NULL, (fn), (data))
#define setup_deferrable_timer_on_stack(timer, fn, data)\
	setup_deferrable_timer_on_stack_key((timer), NULL, NULL, (fn), (data))
#endif

#ifdef CONFIG_DEBUG_OBJECTS_TIMERS
extern void init_timer_on_stack_key(struct timer_list *timer,
				    const char *name,
				    struct lock_class_key *key);
extern void destroy_timer_on_stack(struct timer_list *timer);
#else
static inline void destroy_timer_on_stack(struct timer_list *timer) { }
static inline void init_timer_on_stack_key(struct timer_list *timer,
					   const char *name,
					   struct lock_class_key *key)
{
	init_timer_key(timer, name, key);
}
#endif

static inline void setup_timer_key(struct timer_list * timer,
				const char *name,
				struct lock_class_key *key,
				void (*function)(unsigned long),
				unsigned long data)
{
	timer->function = function;
	timer->data = data;
	init_timer_key(timer, name, key);
}

static inline void setup_timer_on_stack_key(struct timer_list *timer,
					const char *name,
					struct lock_class_key *key,
					void (*function)(unsigned long),
					unsigned long data)
{
	timer->function = function;
	timer->data = data;
	init_timer_on_stack_key(timer, name, key);
}

extern void setup_deferrable_timer_on_stack_key(struct timer_list *timer,
						const char *name,
						struct lock_class_key *key,
						void (*function)(unsigned long),
						unsigned long data);
=======
 * @TIMER_DEFERRABLE: A deferrable timer will work normally when the
 * system is busy, but will not cause a CPU to come out of idle just
 * to service it; instead, the timer will be serviced when the CPU
 * eventually wakes up with a subsequent non-deferrable timer.
 *
 * @TIMER_IRQSAFE: An irqsafe timer is executed with IRQ disabled and
 * it's safe to wait for the completion of the running instance from
 * IRQ handlers, for example, by calling del_timer_sync().
 *
 * Note: The irq disabled callback execution is a special case for
 * workqueue locking issues. It's not meant for executing random crap
 * with interrupts disabled. Abuse is monitored!
 *
 * @TIMER_PINNED: A pinned timer will always expire on the CPU on which the
 * timer was enqueued. When a particular CPU is required, add_timer_on()
 * has to be used. Enqueue via mod_timer() and add_timer() is always done
 * on the local CPU.
 */
#define TIMER_CPUMASK		0x0003FFFF
#define TIMER_MIGRATING		0x00040000
#define TIMER_BASEMASK		(TIMER_CPUMASK | TIMER_MIGRATING)
#define TIMER_DEFERRABLE	0x00080000
#define TIMER_PINNED		0x00100000
#define TIMER_IRQSAFE		0x00200000
#define TIMER_INIT_FLAGS	(TIMER_DEFERRABLE | TIMER_PINNED | TIMER_IRQSAFE)
#define TIMER_ARRAYSHIFT	22
#define TIMER_ARRAYMASK		0xFFC00000

#define TIMER_TRACE_FLAGMASK	(TIMER_MIGRATING | TIMER_DEFERRABLE | TIMER_PINNED | TIMER_IRQSAFE)

#define __TIMER_INITIALIZER(_function, _flags) {		\
		.entry = { .next = TIMER_ENTRY_STATIC },	\
		.function = (_function),			\
		.flags = (_flags),				\
		__TIMER_LOCKDEP_MAP_INITIALIZER(FILE_LINE)	\
	}

#define DEFINE_TIMER(_name, _function)				\
	struct timer_list _name =				\
		__TIMER_INITIALIZER(_function, 0)

/*
 * LOCKDEP and DEBUG timer interfaces.
 */
void init_timer_key(struct timer_list *timer,
		    void (*func)(struct timer_list *), unsigned int flags,
		    const char *name, struct lock_class_key *key);

#ifdef CONFIG_DEBUG_OBJECTS_TIMERS
extern void init_timer_on_stack_key(struct timer_list *timer,
				    void (*func)(struct timer_list *),
				    unsigned int flags, const char *name,
				    struct lock_class_key *key);
#else
static inline void init_timer_on_stack_key(struct timer_list *timer,
					   void (*func)(struct timer_list *),
					   unsigned int flags,
					   const char *name,
					   struct lock_class_key *key)
{
	init_timer_key(timer, func, flags, name, key);
}
#endif

#ifdef CONFIG_LOCKDEP
#define __init_timer(_timer, _fn, _flags)				\
	do {								\
		static struct lock_class_key __key;			\
		init_timer_key((_timer), (_fn), (_flags), #_timer, &__key);\
	} while (0)

#define __init_timer_on_stack(_timer, _fn, _flags)			\
	do {								\
		static struct lock_class_key __key;			\
		init_timer_on_stack_key((_timer), (_fn), (_flags),	\
					#_timer, &__key);		 \
	} while (0)
#else
#define __init_timer(_timer, _fn, _flags)				\
	init_timer_key((_timer), (_fn), (_flags), NULL, NULL)
#define __init_timer_on_stack(_timer, _fn, _flags)			\
	init_timer_on_stack_key((_timer), (_fn), (_flags), NULL, NULL)
#endif

/**
 * timer_setup - prepare a timer for first use
 * @timer: the timer in question
 * @callback: the function to call when timer expires
 * @flags: any TIMER_* flags
 *
 * Regular timer initialization should use either DEFINE_TIMER() above,
 * or timer_setup(). For timers on the stack, timer_setup_on_stack() must
 * be used and must be balanced with a call to destroy_timer_on_stack().
 */
#define timer_setup(timer, callback, flags)			\
	__init_timer((timer), (callback), (flags))

#define timer_setup_on_stack(timer, callback, flags)		\
	__init_timer_on_stack((timer), (callback), (flags))

#ifdef CONFIG_DEBUG_OBJECTS_TIMERS
extern void destroy_timer_on_stack(struct timer_list *timer);
#else
static inline void destroy_timer_on_stack(struct timer_list *timer) { }
#endif

#define from_timer(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * timer_pending - is a timer pending?
 * @timer: the timer in question
 *
 * timer_pending will tell whether a given timer is currently pending,
 * or not. Callers must ensure serialization wrt. other operations done
 * to this timer, eg. interrupt contexts, or other CPUs on SMP.
 *
<<<<<<< HEAD
 * return value: 1 if the timer is pending, 0 if not.
 */
static inline int timer_pending(const struct timer_list * timer)
{
	return timer->entry.next != NULL;
}

extern void add_timer_on(struct timer_list *timer, int cpu);
extern int del_timer(struct timer_list * timer);
extern int mod_timer(struct timer_list *timer, unsigned long expires);
extern int mod_timer_pending(struct timer_list *timer, unsigned long expires);
extern int mod_timer_pinned(struct timer_list *timer, unsigned long expires);

extern void set_timer_slack(struct timer_list *time, int slack_hz);

#define TIMER_NOT_PINNED	0
#define TIMER_PINNED		1
=======
 * Returns: 1 if the timer is pending, 0 if not.
 */
static inline int timer_pending(const struct timer_list * timer)
{
	return !hlist_unhashed_lockless(&timer->entry);
}

extern void add_timer_on(struct timer_list *timer, int cpu);
extern int mod_timer(struct timer_list *timer, unsigned long expires);
extern int mod_timer_pending(struct timer_list *timer, unsigned long expires);
extern int timer_reduce(struct timer_list *timer, unsigned long expires);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The jiffies value which is added to now, when there is no timer
 * in the timer wheel:
 */
#define NEXT_TIMER_MAX_DELTA	((1UL << 30) - 1)

<<<<<<< HEAD
/*
 * Return when the next timer-wheel timeout occurs (in absolute jiffies),
 * locks the timer base and does the comparison against the given
 * jiffie.
 */
extern unsigned long get_next_timer_interrupt(unsigned long now);

extern void add_timer(struct timer_list *timer);

extern int try_to_del_timer_sync(struct timer_list *timer);

#ifdef CONFIG_SMP
  extern int del_timer_sync(struct timer_list *timer);
#else
# define del_timer_sync(t)		del_timer(t)
#endif

#define del_singleshot_timer_sync(t) del_timer_sync(t)

extern void init_timers(void);
extern void run_local_timers(void);
=======
extern void add_timer(struct timer_list *timer);
extern void add_timer_local(struct timer_list *timer);
extern void add_timer_global(struct timer_list *timer);

extern int try_to_del_timer_sync(struct timer_list *timer);
extern int timer_delete_sync(struct timer_list *timer);
extern int timer_delete(struct timer_list *timer);
extern int timer_shutdown_sync(struct timer_list *timer);
extern int timer_shutdown(struct timer_list *timer);

/**
 * del_timer_sync - Delete a pending timer and wait for a running callback
 * @timer:	The timer to be deleted
 *
 * See timer_delete_sync() for detailed explanation.
 *
 * Do not use in new code. Use timer_delete_sync() instead.
 *
 * Returns:
 * * %0	- The timer was not pending
 * * %1	- The timer was pending and deactivated
 */
static inline int del_timer_sync(struct timer_list *timer)
{
	return timer_delete_sync(timer);
}

/**
 * del_timer - Delete a pending timer
 * @timer:	The timer to be deleted
 *
 * See timer_delete() for detailed explanation.
 *
 * Do not use in new code. Use timer_delete() instead.
 *
 * Returns:
 * * %0	- The timer was not pending
 * * %1	- The timer was pending and deactivated
 */
static inline int del_timer(struct timer_list *timer)
{
	return timer_delete(timer);
}

extern void init_timers(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct hrtimer;
extern enum hrtimer_restart it_real_fn(struct hrtimer *);

unsigned long __round_jiffies(unsigned long j, int cpu);
unsigned long __round_jiffies_relative(unsigned long j, int cpu);
unsigned long round_jiffies(unsigned long j);
unsigned long round_jiffies_relative(unsigned long j);

unsigned long __round_jiffies_up(unsigned long j, int cpu);
unsigned long __round_jiffies_up_relative(unsigned long j, int cpu);
unsigned long round_jiffies_up(unsigned long j);
unsigned long round_jiffies_up_relative(unsigned long j);

<<<<<<< HEAD
=======
#ifdef CONFIG_HOTPLUG_CPU
int timers_prepare_cpu(unsigned int cpu);
int timers_dead_cpu(unsigned int cpu);
#else
#define timers_prepare_cpu	NULL
#define timers_dead_cpu		NULL
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
