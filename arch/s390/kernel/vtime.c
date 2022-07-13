<<<<<<< HEAD
/*
 *  arch/s390/kernel/vtime.c
 *    Virtual cpu timer based timer functions.
 *
 *  S390 version
 *    Copyright (C) 2004 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Jan Glauber <jan.glauber@de.ibm.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/types.h>
#include <linux/timex.h>
#include <linux/notifier.h>
#include <linux/kernel_stat.h>
#include <linux/rcupdate.h>
#include <linux/posix-timers.h>
#include <linux/cpu.h>
#include <linux/kprobes.h>

#include <asm/timer.h>
#include <asm/irq_regs.h>
#include <asm/cputime.h>
#include <asm/irq.h>
#include "entry.h"

static DEFINE_PER_CPU(struct vtimer_queue, virt_cpu_timer);

DEFINE_PER_CPU(struct s390_idle_data, s390_idle);

static inline __u64 get_vtimer(void)
{
	__u64 timer;

	asm volatile("STPT %0" : "=m" (timer));
	return timer;
}

static inline void set_vtimer(__u64 expires)
{
	__u64 timer;

	asm volatile ("  STPT %0\n"  /* Store current cpu timer value */
		      "  SPT %1"     /* Set new value immediately afterwards */
		      : "=m" (timer) : "m" (expires) );
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *    Virtual cpu timer based timer functions.
 *
 *    Copyright IBM Corp. 2004, 2012
 *    Author(s): Jan Glauber <jan.glauber@de.ibm.com>
 */

#include <linux/kernel_stat.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/time.h>
#include <asm/alternative.h>
#include <asm/cputime.h>
#include <asm/vtimer.h>
#include <asm/vtime.h>
#include <asm/cpu_mf.h>
#include <asm/smp.h>

#include "entry.h"

static void virt_timer_expire(void);

static LIST_HEAD(virt_timer_list);
static DEFINE_SPINLOCK(virt_timer_lock);
static atomic64_t virt_timer_current;
static atomic64_t virt_timer_elapsed;

DEFINE_PER_CPU(u64, mt_cycles[8]);
static DEFINE_PER_CPU(u64, mt_scaling_mult) = { 1 };
static DEFINE_PER_CPU(u64, mt_scaling_div) = { 1 };
static DEFINE_PER_CPU(u64, mt_scaling_jiffies);

static inline u64 get_vtimer(void)
{
	u64 timer;

	asm volatile("stpt %0" : "=Q" (timer));
	return timer;
}

static inline void set_vtimer(u64 expires)
{
	u64 timer;

	asm volatile(
		"	stpt	%0\n"	/* Store current cpu timer value */
		"	spt	%1"	/* Set new value imm. afterwards */
		: "=Q" (timer) : "Q" (expires));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	S390_lowcore.system_timer += S390_lowcore.last_update_timer - timer;
	S390_lowcore.last_update_timer = expires;
}

<<<<<<< HEAD
=======
static inline int virt_timer_forward(u64 elapsed)
{
	BUG_ON(!irqs_disabled());

	if (list_empty(&virt_timer_list))
		return 0;
	elapsed = atomic64_add_return(elapsed, &virt_timer_elapsed);
	return elapsed >= atomic64_read(&virt_timer_current);
}

static void update_mt_scaling(void)
{
	u64 cycles_new[8], *cycles_old;
	u64 delta, fac, mult, div;
	int i;

	stcctm(MT_DIAG, smp_cpu_mtid + 1, cycles_new);
	cycles_old = this_cpu_ptr(mt_cycles);
	fac = 1;
	mult = div = 0;
	for (i = 0; i <= smp_cpu_mtid; i++) {
		delta = cycles_new[i] - cycles_old[i];
		div += delta;
		mult *= i + 1;
		mult += delta * fac;
		fac *= i + 1;
	}
	div *= fac;
	if (div > 0) {
		/* Update scaling factor */
		__this_cpu_write(mt_scaling_mult, mult);
		__this_cpu_write(mt_scaling_div, div);
		memcpy(cycles_old, cycles_new,
		       sizeof(u64) * (smp_cpu_mtid + 1));
	}
	__this_cpu_write(mt_scaling_jiffies, jiffies_64);
}

static inline u64 update_tsk_timer(unsigned long *tsk_vtime, u64 new)
{
	u64 delta;

	delta = new - *tsk_vtime;
	*tsk_vtime = new;
	return delta;
}


static inline u64 scale_vtime(u64 vtime)
{
	u64 mult = __this_cpu_read(mt_scaling_mult);
	u64 div = __this_cpu_read(mt_scaling_div);

	if (smp_cpu_mtid)
		return vtime * mult / div;
	return vtime;
}

static void account_system_index_scaled(struct task_struct *p, u64 cputime,
					enum cpu_usage_stat index)
{
	p->stimescaled += cputime_to_nsecs(scale_vtime(cputime));
	account_system_index_time(p, cputime_to_nsecs(cputime), index);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Update process times based on virtual cpu times stored by entry.S
 * to the lowcore fields user_timer, system_timer & steal_clock.
 */
<<<<<<< HEAD
static void do_account_vtime(struct task_struct *tsk, int hardirq_offset)
{
	struct thread_info *ti = task_thread_info(tsk);
	__u64 timer, clock, user, system, steal;

	timer = S390_lowcore.last_update_timer;
	clock = S390_lowcore.last_update_clock;
	asm volatile ("  STPT %0\n"    /* Store current cpu timer value */
		      "  STCK %1"      /* Store current tod clock value */
		      : "=m" (S390_lowcore.last_update_timer),
		        "=m" (S390_lowcore.last_update_clock) );
	S390_lowcore.system_timer += timer - S390_lowcore.last_update_timer;
	S390_lowcore.steal_timer += S390_lowcore.last_update_clock - clock;

	user = S390_lowcore.user_timer - ti->user_timer;
	S390_lowcore.steal_timer -= user;
	ti->user_timer = S390_lowcore.user_timer;
	account_user_time(tsk, user, user);

	system = S390_lowcore.system_timer - ti->system_timer;
	S390_lowcore.steal_timer -= system;
	ti->system_timer = S390_lowcore.system_timer;
	account_system_time(tsk, hardirq_offset, system, system);

	steal = S390_lowcore.steal_timer;
	if ((s64) steal > 0) {
		S390_lowcore.steal_timer = 0;
		account_steal_time(steal);
	}
}

void account_vtime(struct task_struct *prev, struct task_struct *next)
{
	struct thread_info *ti;

	do_account_vtime(prev, 0);
	ti = task_thread_info(prev);
	ti->user_timer = S390_lowcore.user_timer;
	ti->system_timer = S390_lowcore.system_timer;
	ti = task_thread_info(next);
	S390_lowcore.user_timer = ti->user_timer;
	S390_lowcore.system_timer = ti->system_timer;
}

void account_process_tick(struct task_struct *tsk, int user_tick)
{
	do_account_vtime(tsk, HARDIRQ_OFFSET);
=======
static int do_account_vtime(struct task_struct *tsk)
{
	u64 timer, clock, user, guest, system, hardirq, softirq;

	timer = S390_lowcore.last_update_timer;
	clock = S390_lowcore.last_update_clock;
	asm volatile(
		"	stpt	%0\n"	/* Store current cpu timer value */
		"	stckf	%1"	/* Store current tod clock value */
		: "=Q" (S390_lowcore.last_update_timer),
		  "=Q" (S390_lowcore.last_update_clock)
		: : "cc");
	clock = S390_lowcore.last_update_clock - clock;
	timer -= S390_lowcore.last_update_timer;

	if (hardirq_count())
		S390_lowcore.hardirq_timer += timer;
	else
		S390_lowcore.system_timer += timer;

	/* Update MT utilization calculation */
	if (smp_cpu_mtid &&
	    time_after64(jiffies_64, this_cpu_read(mt_scaling_jiffies)))
		update_mt_scaling();

	/* Calculate cputime delta */
	user = update_tsk_timer(&tsk->thread.user_timer,
				READ_ONCE(S390_lowcore.user_timer));
	guest = update_tsk_timer(&tsk->thread.guest_timer,
				 READ_ONCE(S390_lowcore.guest_timer));
	system = update_tsk_timer(&tsk->thread.system_timer,
				  READ_ONCE(S390_lowcore.system_timer));
	hardirq = update_tsk_timer(&tsk->thread.hardirq_timer,
				   READ_ONCE(S390_lowcore.hardirq_timer));
	softirq = update_tsk_timer(&tsk->thread.softirq_timer,
				   READ_ONCE(S390_lowcore.softirq_timer));
	S390_lowcore.steal_timer +=
		clock - user - guest - system - hardirq - softirq;

	/* Push account value */
	if (user) {
		account_user_time(tsk, cputime_to_nsecs(user));
		tsk->utimescaled += cputime_to_nsecs(scale_vtime(user));
	}

	if (guest) {
		account_guest_time(tsk, cputime_to_nsecs(guest));
		tsk->utimescaled += cputime_to_nsecs(scale_vtime(guest));
	}

	if (system)
		account_system_index_scaled(tsk, system, CPUTIME_SYSTEM);
	if (hardirq)
		account_system_index_scaled(tsk, hardirq, CPUTIME_IRQ);
	if (softirq)
		account_system_index_scaled(tsk, softirq, CPUTIME_SOFTIRQ);

	return virt_timer_forward(user + guest + system + hardirq + softirq);
}

void vtime_task_switch(struct task_struct *prev)
{
	do_account_vtime(prev);
	prev->thread.user_timer = S390_lowcore.user_timer;
	prev->thread.guest_timer = S390_lowcore.guest_timer;
	prev->thread.system_timer = S390_lowcore.system_timer;
	prev->thread.hardirq_timer = S390_lowcore.hardirq_timer;
	prev->thread.softirq_timer = S390_lowcore.softirq_timer;
	S390_lowcore.user_timer = current->thread.user_timer;
	S390_lowcore.guest_timer = current->thread.guest_timer;
	S390_lowcore.system_timer = current->thread.system_timer;
	S390_lowcore.hardirq_timer = current->thread.hardirq_timer;
	S390_lowcore.softirq_timer = current->thread.softirq_timer;
}

/*
 * In s390, accounting pending user time also implies
 * accounting system time in order to correctly compute
 * the stolen time accounting.
 */
void vtime_flush(struct task_struct *tsk)
{
	u64 steal, avg_steal;

	if (do_account_vtime(tsk))
		virt_timer_expire();

	steal = S390_lowcore.steal_timer;
	avg_steal = S390_lowcore.avg_steal_timer;
	if ((s64) steal > 0) {
		S390_lowcore.steal_timer = 0;
		account_steal_time(cputime_to_nsecs(steal));
		avg_steal += steal;
	}
	S390_lowcore.avg_steal_timer = avg_steal / 2;
}

static u64 vtime_delta(void)
{
	u64 timer = S390_lowcore.last_update_timer;

	S390_lowcore.last_update_timer = get_vtimer();

	return timer - S390_lowcore.last_update_timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Update process times based on virtual cpu times stored by entry.S
 * to the lowcore fields user_timer, system_timer & steal_clock.
 */
<<<<<<< HEAD
void account_system_vtime(struct task_struct *tsk)
{
	struct thread_info *ti = task_thread_info(tsk);
	__u64 timer, system;

	timer = S390_lowcore.last_update_timer;
	S390_lowcore.last_update_timer = get_vtimer();
	S390_lowcore.system_timer += timer - S390_lowcore.last_update_timer;

	system = S390_lowcore.system_timer - ti->system_timer;
	S390_lowcore.steal_timer -= system;
	ti->system_timer = S390_lowcore.system_timer;
	account_system_time(tsk, 0, system, system);
}
EXPORT_SYMBOL_GPL(account_system_vtime);

void __kprobes vtime_stop_cpu(void)
{
	struct s390_idle_data *idle = &__get_cpu_var(s390_idle);
	struct vtimer_queue *vq = &__get_cpu_var(virt_cpu_timer);
	unsigned long long idle_time;
	unsigned long psw_mask;

	trace_hardirqs_on();
	/* Don't trace preempt off for idle. */
	stop_critical_timings();

	/* Wait for external, I/O or machine check interrupt. */
	psw_mask = psw_kernel_bits | PSW_MASK_WAIT | PSW_MASK_DAT |
		PSW_MASK_IO | PSW_MASK_EXT | PSW_MASK_MCHECK;
	idle->nohz_delay = 0;

	/* Call the assembler magic in entry.S */
	psw_idle(idle, vq, psw_mask, !list_empty(&vq->list));

	/* Reenable preemption tracer. */
	start_critical_timings();

	/* Account time spent with enabled wait psw loaded as idle time. */
	idle->sequence++;
	smp_wmb();
	idle_time = idle->idle_exit - idle->idle_enter;
	idle->idle_time += idle_time;
	idle->idle_enter = idle->idle_exit = 0ULL;
	idle->idle_count++;
	account_idle_time(idle_time);
	smp_wmb();
	idle->sequence++;
}

cputime64_t s390_get_idle_time(int cpu)
{
	struct s390_idle_data *idle = &per_cpu(s390_idle, cpu);
	unsigned long long now, idle_enter, idle_exit;
	unsigned int sequence;

	do {
		now = get_clock();
		sequence = ACCESS_ONCE(idle->sequence);
		idle_enter = ACCESS_ONCE(idle->idle_enter);
		idle_exit = ACCESS_ONCE(idle->idle_exit);
	} while ((sequence & 1) || (idle->sequence != sequence));
	return idle_enter ? ((idle_exit ? : now) - idle_enter) : 0;
=======
void vtime_account_kernel(struct task_struct *tsk)
{
	u64 delta = vtime_delta();

	if (tsk->flags & PF_VCPU)
		S390_lowcore.guest_timer += delta;
	else
		S390_lowcore.system_timer += delta;

	virt_timer_forward(delta);
}
EXPORT_SYMBOL_GPL(vtime_account_kernel);

void vtime_account_softirq(struct task_struct *tsk)
{
	u64 delta = vtime_delta();

	S390_lowcore.softirq_timer += delta;

	virt_timer_forward(delta);
}

void vtime_account_hardirq(struct task_struct *tsk)
{
	u64 delta = vtime_delta();

	S390_lowcore.hardirq_timer += delta;

	virt_timer_forward(delta);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Sorted add to a list. List is linear searched until first bigger
 * element is found.
 */
static void list_add_sorted(struct vtimer_list *timer, struct list_head *head)
{
<<<<<<< HEAD
	struct vtimer_list *event;

	list_for_each_entry(event, head, entry) {
		if (event->expires > timer->expires) {
			list_add_tail(&timer->entry, &event->entry);
=======
	struct vtimer_list *tmp;

	list_for_each_entry(tmp, head, entry) {
		if (tmp->expires > timer->expires) {
			list_add_tail(&timer->entry, &tmp->entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		}
	}
	list_add_tail(&timer->entry, head);
}

/*
<<<<<<< HEAD
 * Do the callback functions of expired vtimer events.
 * Called from within the interrupt handler.
 */
static void do_callbacks(struct list_head *cb_list)
{
	struct vtimer_queue *vq;
	struct vtimer_list *event, *tmp;

	if (list_empty(cb_list))
		return;

	vq = &__get_cpu_var(virt_cpu_timer);

	list_for_each_entry_safe(event, tmp, cb_list, entry) {
		list_del_init(&event->entry);
		(event->function)(event->data);
		if (event->interval) {
			/* Recharge interval timer */
			event->expires = event->interval + vq->elapsed;
			spin_lock(&vq->lock);
			list_add_sorted(event, &vq->list);
			spin_unlock(&vq->lock);
=======
 * Handler for expired virtual CPU timer.
 */
static void virt_timer_expire(void)
{
	struct vtimer_list *timer, *tmp;
	unsigned long elapsed;
	LIST_HEAD(cb_list);

	/* walk timer list, fire all expired timers */
	spin_lock(&virt_timer_lock);
	elapsed = atomic64_read(&virt_timer_elapsed);
	list_for_each_entry_safe(timer, tmp, &virt_timer_list, entry) {
		if (timer->expires < elapsed)
			/* move expired timer to the callback queue */
			list_move_tail(&timer->entry, &cb_list);
		else
			timer->expires -= elapsed;
	}
	if (!list_empty(&virt_timer_list)) {
		timer = list_first_entry(&virt_timer_list,
					 struct vtimer_list, entry);
		atomic64_set(&virt_timer_current, timer->expires);
	}
	atomic64_sub(elapsed, &virt_timer_elapsed);
	spin_unlock(&virt_timer_lock);

	/* Do callbacks and recharge periodic timers */
	list_for_each_entry_safe(timer, tmp, &cb_list, entry) {
		list_del_init(&timer->entry);
		timer->function(timer->data);
		if (timer->interval) {
			/* Recharge interval timer */
			timer->expires = timer->interval +
				atomic64_read(&virt_timer_elapsed);
			spin_lock(&virt_timer_lock);
			list_add_sorted(timer, &virt_timer_list);
			spin_unlock(&virt_timer_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

<<<<<<< HEAD
/*
 * Handler for the virtual CPU timer.
 */
static void do_cpu_timer_interrupt(struct ext_code ext_code,
				   unsigned int param32, unsigned long param64)
{
	struct vtimer_queue *vq;
	struct vtimer_list *event, *tmp;
	struct list_head cb_list;	/* the callback queue */
	__u64 elapsed, next;

	kstat_cpu(smp_processor_id()).irqs[EXTINT_TMR]++;
	INIT_LIST_HEAD(&cb_list);
	vq = &__get_cpu_var(virt_cpu_timer);

	/* walk timer list, fire all expired events */
	spin_lock(&vq->lock);

	elapsed = vq->elapsed + (vq->timer - S390_lowcore.async_enter_timer);
	BUG_ON((s64) elapsed < 0);
	vq->elapsed = 0;
	list_for_each_entry_safe(event, tmp, &vq->list, entry) {
		if (event->expires < elapsed)
			/* move expired timer to the callback queue */
			list_move_tail(&event->entry, &cb_list);
		else
			event->expires -= elapsed;
	}
	spin_unlock(&vq->lock);

	do_callbacks(&cb_list);

	/* next event is first in list */
	next = VTIMER_MAX_SLICE;
	spin_lock(&vq->lock);
	if (!list_empty(&vq->list)) {
		event = list_first_entry(&vq->list, struct vtimer_list, entry);
		next = event->expires;
	}
	spin_unlock(&vq->lock);
	/*
	 * To improve precision add the time spent by the
	 * interrupt handler to the elapsed time.
	 * Note: CPU timer counts down and we got an interrupt,
	 *	 the current content is negative
	 */
	elapsed = S390_lowcore.async_enter_timer - get_vtimer();
	set_vtimer(next - elapsed);
	vq->timer = next - elapsed;
	vq->elapsed = elapsed;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void init_virt_timer(struct vtimer_list *timer)
{
	timer->function = NULL;
	INIT_LIST_HEAD(&timer->entry);
}
EXPORT_SYMBOL(init_virt_timer);

static inline int vtimer_pending(struct vtimer_list *timer)
{
<<<<<<< HEAD
	return (!list_empty(&timer->entry));
}

/*
 * this function should only run on the specified CPU
 */
static void internal_add_vtimer(struct vtimer_list *timer)
{
	struct vtimer_queue *vq;
	unsigned long flags;
	__u64 left, expires;

	vq = &per_cpu(virt_cpu_timer, timer->cpu);
	spin_lock_irqsave(&vq->lock, flags);

	BUG_ON(timer->cpu != smp_processor_id());

	if (list_empty(&vq->list)) {
		/* First timer on this cpu, just program it. */
		list_add(&timer->entry, &vq->list);
		set_vtimer(timer->expires);
		vq->timer = timer->expires;
		vq->elapsed = 0;
	} else {
		/* Check progress of old timers. */
		expires = timer->expires;
		left = get_vtimer();
		if (likely((s64) expires < (s64) left)) {
			/* The new timer expires before the current timer. */
			set_vtimer(expires);
			vq->elapsed += vq->timer - left;
			vq->timer = expires;
		} else {
			vq->elapsed += vq->timer - left;
			vq->timer = left;
		}
		/* Insert new timer into per cpu list. */
		timer->expires += vq->elapsed;
		list_add_sorted(timer, &vq->list);
	}

	spin_unlock_irqrestore(&vq->lock, flags);
	/* release CPU acquired in prepare_vtimer or mod_virt_timer() */
	put_cpu();
}

static inline void prepare_vtimer(struct vtimer_list *timer)
{
	BUG_ON(!timer->function);
	BUG_ON(!timer->expires || timer->expires > VTIMER_MAX_SLICE);
	BUG_ON(vtimer_pending(timer));
	timer->cpu = get_cpu();
}

/*
 * add_virt_timer - add an oneshot virtual CPU timer
 */
void add_virt_timer(void *new)
{
	struct vtimer_list *timer;

	timer = (struct vtimer_list *)new;
	prepare_vtimer(timer);
	timer->interval = 0;
	internal_add_vtimer(timer);
=======
	return !list_empty(&timer->entry);
}

static void internal_add_vtimer(struct vtimer_list *timer)
{
	if (list_empty(&virt_timer_list)) {
		/* First timer, just program it. */
		atomic64_set(&virt_timer_current, timer->expires);
		atomic64_set(&virt_timer_elapsed, 0);
		list_add(&timer->entry, &virt_timer_list);
	} else {
		/* Update timer against current base. */
		timer->expires += atomic64_read(&virt_timer_elapsed);
		if (likely((s64) timer->expires <
			   (s64) atomic64_read(&virt_timer_current)))
			/* The new timer expires before the current timer. */
			atomic64_set(&virt_timer_current, timer->expires);
		/* Insert new timer into the list. */
		list_add_sorted(timer, &virt_timer_list);
	}
}

static void __add_vtimer(struct vtimer_list *timer, int periodic)
{
	unsigned long flags;

	timer->interval = periodic ? timer->expires : 0;
	spin_lock_irqsave(&virt_timer_lock, flags);
	internal_add_vtimer(timer);
	spin_unlock_irqrestore(&virt_timer_lock, flags);
}

/*
 * add_virt_timer - add a oneshot virtual CPU timer
 */
void add_virt_timer(struct vtimer_list *timer)
{
	__add_vtimer(timer, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(add_virt_timer);

/*
 * add_virt_timer_int - add an interval virtual CPU timer
 */
<<<<<<< HEAD
void add_virt_timer_periodic(void *new)
{
	struct vtimer_list *timer;

	timer = (struct vtimer_list *)new;
	prepare_vtimer(timer);
	timer->interval = timer->expires;
	internal_add_vtimer(timer);
}
EXPORT_SYMBOL(add_virt_timer_periodic);

static int __mod_vtimer(struct vtimer_list *timer, __u64 expires, int periodic)
{
	struct vtimer_queue *vq;
	unsigned long flags;
	int cpu;

	BUG_ON(!timer->function);
	BUG_ON(!expires || expires > VTIMER_MAX_SLICE);

	if (timer->expires == expires && vtimer_pending(timer))
		return 1;

	cpu = get_cpu();
	vq = &per_cpu(virt_cpu_timer, cpu);

	/* disable interrupts before test if timer is pending */
	spin_lock_irqsave(&vq->lock, flags);

	/* if timer isn't pending add it on the current CPU */
	if (!vtimer_pending(timer)) {
		spin_unlock_irqrestore(&vq->lock, flags);

		if (periodic)
			timer->interval = expires;
		else
			timer->interval = 0;
		timer->expires = expires;
		timer->cpu = cpu;
		internal_add_vtimer(timer);
		return 0;
	}

	/* check if we run on the right CPU */
	BUG_ON(timer->cpu != cpu);

	list_del_init(&timer->entry);
	timer->expires = expires;
	if (periodic)
		timer->interval = expires;

	/* the timer can't expire anymore so we can release the lock */
	spin_unlock_irqrestore(&vq->lock, flags);
	internal_add_vtimer(timer);
	return 1;
}

/*
 * If we change a pending timer the function must be called on the CPU
 * where the timer is running on.
 *
 * returns whether it has modified a pending timer (1) or not (0)
 */
int mod_virt_timer(struct vtimer_list *timer, __u64 expires)
=======
void add_virt_timer_periodic(struct vtimer_list *timer)
{
	__add_vtimer(timer, 1);
}
EXPORT_SYMBOL(add_virt_timer_periodic);

static int __mod_vtimer(struct vtimer_list *timer, u64 expires, int periodic)
{
	unsigned long flags;
	int rc;

	BUG_ON(!timer->function);

	if (timer->expires == expires && vtimer_pending(timer))
		return 1;
	spin_lock_irqsave(&virt_timer_lock, flags);
	rc = vtimer_pending(timer);
	if (rc)
		list_del_init(&timer->entry);
	timer->interval = periodic ? expires : 0;
	timer->expires = expires;
	internal_add_vtimer(timer);
	spin_unlock_irqrestore(&virt_timer_lock, flags);
	return rc;
}

/*
 * returns whether it has modified a pending timer (1) or not (0)
 */
int mod_virt_timer(struct vtimer_list *timer, u64 expires)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __mod_vtimer(timer, expires, 0);
}
EXPORT_SYMBOL(mod_virt_timer);

/*
<<<<<<< HEAD
 * If we change a pending timer the function must be called on the CPU
 * where the timer is running on.
 *
 * returns whether it has modified a pending timer (1) or not (0)
 */
int mod_virt_timer_periodic(struct vtimer_list *timer, __u64 expires)
=======
 * returns whether it has modified a pending timer (1) or not (0)
 */
int mod_virt_timer_periodic(struct vtimer_list *timer, u64 expires)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __mod_vtimer(timer, expires, 1);
}
EXPORT_SYMBOL(mod_virt_timer_periodic);

/*
<<<<<<< HEAD
 * delete a virtual timer
=======
 * Delete a virtual timer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * returns whether the deleted timer was pending (1) or not (0)
 */
int del_virt_timer(struct vtimer_list *timer)
{
	unsigned long flags;
<<<<<<< HEAD
	struct vtimer_queue *vq;

	/* check if timer is pending */
	if (!vtimer_pending(timer))
		return 0;

	vq = &per_cpu(virt_cpu_timer, timer->cpu);
	spin_lock_irqsave(&vq->lock, flags);

	/* we don't interrupt a running timer, just let it expire! */
	list_del_init(&timer->entry);

	spin_unlock_irqrestore(&vq->lock, flags);
=======

	if (!vtimer_pending(timer))
		return 0;
	spin_lock_irqsave(&virt_timer_lock, flags);
	list_del_init(&timer->entry);
	spin_unlock_irqrestore(&virt_timer_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}
EXPORT_SYMBOL(del_virt_timer);

/*
 * Start the virtual CPU timer on the current CPU.
 */
<<<<<<< HEAD
void init_cpu_vtimer(void)
{
	struct vtimer_queue *vq;

	/* initialize per cpu vtimer structure */
	vq = &__get_cpu_var(virt_cpu_timer);
	INIT_LIST_HEAD(&vq->list);
	spin_lock_init(&vq->lock);

	/* enable cpu timer interrupts */
	__ctl_set_bit(0,10);

	/* set initial cpu timer */
	set_vtimer(0x7fffffffffffffffULL);
}

static int __cpuinit s390_nohz_notify(struct notifier_block *self,
				      unsigned long action, void *hcpu)
{
	struct s390_idle_data *idle;
	long cpu = (long) hcpu;

	idle = &per_cpu(s390_idle, cpu);
	switch (action) {
	case CPU_DYING:
	case CPU_DYING_FROZEN:
		idle->nohz_delay = 0;
	default:
		break;
	}
	return NOTIFY_OK;
}

void __init vtime_init(void)
{
	/* request the cpu timer external interrupt */
	if (register_external_interrupt(0x1005, do_cpu_timer_interrupt))
		panic("Couldn't request external interrupt 0x1005");

	/* Enable cpu timer interrupts on the boot cpu. */
	init_cpu_vtimer();
	cpu_notifier(s390_nohz_notify, 0);
}

=======
void vtime_init(void)
{
	/* set initial cpu timer */
	set_vtimer(VTIMER_MAX_SLICE);
	/* Setup initial MT scaling values */
	if (smp_cpu_mtid) {
		__this_cpu_write(mt_scaling_jiffies, jiffies);
		__this_cpu_write(mt_scaling_mult, 1);
		__this_cpu_write(mt_scaling_div, 1);
		stcctm(MT_DIAG, smp_cpu_mtid + 1, this_cpu_ptr(mt_cycles));
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
