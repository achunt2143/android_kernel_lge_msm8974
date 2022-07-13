<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This contains the io-permission bitmap code - written by obz, with changes
 * by Linus. 32/64 bits code unification by Miguel Botón.
 */
<<<<<<< HEAD

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/smp.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/thread_info.h>
#include <linux/syscalls.h>
#include <linux/bitmap.h>
#include <asm/syscalls.h>

/*
 * this changes the io permissions bitmap in the current task.
 */
asmlinkage long sys_ioperm(unsigned long from, unsigned long num, int turn_on)
{
	struct thread_struct *t = &current->thread;
	struct tss_struct *tss;
	unsigned int i, max_long, bytes, bytes_updated;

	if ((from + num <= from) || (from + num > IO_BITMAP_BITS))
		return -EINVAL;
	if (turn_on && !capable(CAP_SYS_RAWIO))
=======
#include <linux/capability.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/bitmap.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <asm/io_bitmap.h>
#include <asm/desc.h>
#include <asm/syscalls.h>

#ifdef CONFIG_X86_IOPL_IOPERM

static atomic64_t io_bitmap_sequence;

void io_bitmap_share(struct task_struct *tsk)
{
	/* Can be NULL when current->thread.iopl_emul == 3 */
	if (current->thread.io_bitmap) {
		/*
		 * Take a refcount on current's bitmap. It can be used by
		 * both tasks as long as none of them changes the bitmap.
		 */
		refcount_inc(&current->thread.io_bitmap->refcnt);
		tsk->thread.io_bitmap = current->thread.io_bitmap;
	}
	set_tsk_thread_flag(tsk, TIF_IO_BITMAP);
}

static void task_update_io_bitmap(struct task_struct *tsk)
{
	struct thread_struct *t = &tsk->thread;

	if (t->iopl_emul == 3 || t->io_bitmap) {
		/* TSS update is handled on exit to user space */
		set_tsk_thread_flag(tsk, TIF_IO_BITMAP);
	} else {
		clear_tsk_thread_flag(tsk, TIF_IO_BITMAP);
		/* Invalidate TSS */
		preempt_disable();
		tss_update_io_bitmap();
		preempt_enable();
	}
}

void io_bitmap_exit(struct task_struct *tsk)
{
	struct io_bitmap *iobm = tsk->thread.io_bitmap;

	tsk->thread.io_bitmap = NULL;
	task_update_io_bitmap(tsk);
	if (iobm && refcount_dec_and_test(&iobm->refcnt))
		kfree(iobm);
}

/*
 * This changes the io permissions bitmap in the current task.
 */
long ksys_ioperm(unsigned long from, unsigned long num, int turn_on)
{
	struct thread_struct *t = &current->thread;
	unsigned int i, max_long;
	struct io_bitmap *iobm;

	if ((from + num <= from) || (from + num > IO_BITMAP_BITS))
		return -EINVAL;
	if (turn_on && (!capable(CAP_SYS_RAWIO) ||
			security_locked_down(LOCKDOWN_IOPORT)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	/*
	 * If it's the first ioperm() call in this thread's lifetime, set the
	 * IO bitmap up. ioperm() is much less timing critical than clone(),
	 * this is why we delay this operation until now:
	 */
<<<<<<< HEAD
	if (!t->io_bitmap_ptr) {
		unsigned long *bitmap = kmalloc(IO_BITMAP_BYTES, GFP_KERNEL);

		if (!bitmap)
			return -ENOMEM;

		memset(bitmap, 0xff, IO_BITMAP_BYTES);
		t->io_bitmap_ptr = bitmap;
		set_thread_flag(TIF_IO_BITMAP);
	}

	/*
	 * do it in the per-thread copy and in the TSS ...
	 *
	 * Disable preemption via get_cpu() - we must not switch away
	 * because the ->io_bitmap_max value must match the bitmap
	 * contents:
	 */
	tss = &per_cpu(init_tss, get_cpu());

	if (turn_on)
		bitmap_clear(t->io_bitmap_ptr, from, num);
	else
		bitmap_set(t->io_bitmap_ptr, from, num);
=======
	iobm = t->io_bitmap;
	if (!iobm) {
		/* No point to allocate a bitmap just to clear permissions */
		if (!turn_on)
			return 0;
		iobm = kmalloc(sizeof(*iobm), GFP_KERNEL);
		if (!iobm)
			return -ENOMEM;

		memset(iobm->bitmap, 0xff, sizeof(iobm->bitmap));
		refcount_set(&iobm->refcnt, 1);
	}

	/*
	 * If the bitmap is not shared, then nothing can take a refcount as
	 * current can obviously not fork at the same time. If it's shared
	 * duplicate it and drop the refcount on the original one.
	 */
	if (refcount_read(&iobm->refcnt) > 1) {
		iobm = kmemdup(iobm, sizeof(*iobm), GFP_KERNEL);
		if (!iobm)
			return -ENOMEM;
		refcount_set(&iobm->refcnt, 1);
		io_bitmap_exit(current);
	}

	/*
	 * Store the bitmap pointer (might be the same if the task already
	 * head one). Must be done here so freeing the bitmap when all
	 * permissions are dropped has the pointer set up.
	 */
	t->io_bitmap = iobm;
	/* Mark it active for context switching and exit to user mode */
	set_thread_flag(TIF_IO_BITMAP);

	/*
	 * Update the tasks bitmap. The update of the TSS bitmap happens on
	 * exit to user mode. So this needs no protection.
	 */
	if (turn_on)
		bitmap_clear(iobm->bitmap, from, num);
	else
		bitmap_set(iobm->bitmap, from, num);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Search for a (possibly new) maximum. This is simple and stupid,
	 * to keep it obviously correct:
	 */
<<<<<<< HEAD
	max_long = 0;
	for (i = 0; i < IO_BITMAP_LONGS; i++)
		if (t->io_bitmap_ptr[i] != ~0UL)
			max_long = i;

	bytes = (max_long + 1) * sizeof(unsigned long);
	bytes_updated = max(bytes, t->io_bitmap_max);

	t->io_bitmap_max = bytes;

	/* Update the TSS: */
	memcpy(tss->io_bitmap, t->io_bitmap_ptr, bytes_updated);

	put_cpu();
=======
	max_long = UINT_MAX;
	for (i = 0; i < IO_BITMAP_LONGS; i++) {
		if (iobm->bitmap[i] != ~0UL)
			max_long = i;
	}
	/* All permissions dropped? */
	if (max_long == UINT_MAX) {
		io_bitmap_exit(current);
		return 0;
	}

	iobm->max = (max_long + 1) * sizeof(unsigned long);

	/*
	 * Update the sequence number to force a TSS update on return to
	 * user mode.
	 */
	iobm->sequence = atomic64_add_return(1, &io_bitmap_sequence);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/*
 * sys_iopl has to be used when you want to access the IO ports
 * beyond the 0x3ff range: to get the full 65536 ports bitmapped
 * you'd need 8kB of bitmaps/process, which is a bit excessive.
 *
 * Here we just change the flags value on the stack: we allow
 * only the super-user to do it. This depends on the stack-layout
 * on system-call entry - see also fork() and the signal handling
 * code.
 */
long sys_iopl(unsigned int level, struct pt_regs *regs)
{
	unsigned int old = (regs->flags >> 12) & 3;
	struct thread_struct *t = &current->thread;

	if (level > 3)
		return -EINVAL;
	/* Trying to gain more privileges? */
	if (level > old) {
		if (!capable(CAP_SYS_RAWIO))
			return -EPERM;
	}
	regs->flags = (regs->flags & ~X86_EFLAGS_IOPL) | (level << 12);
	t->iopl = level << 12;
	set_iopl_mask(t->iopl);

	return 0;
}
=======
SYSCALL_DEFINE3(ioperm, unsigned long, from, unsigned long, num, int, turn_on)
{
	return ksys_ioperm(from, num, turn_on);
}

/*
 * The sys_iopl functionality depends on the level argument, which if
 * granted for the task is used to enable access to all 65536 I/O ports.
 *
 * This does not use the IOPL mechanism provided by the CPU as that would
 * also allow the user space task to use the CLI/STI instructions.
 *
 * Disabling interrupts in a user space task is dangerous as it might lock
 * up the machine and the semantics vs. syscalls and exceptions is
 * undefined.
 *
 * Setting IOPL to level 0-2 is disabling I/O permissions. Level 3
 * 3 enables them.
 *
 * IOPL is strictly per thread and inherited on fork.
 */
SYSCALL_DEFINE1(iopl, unsigned int, level)
{
	struct thread_struct *t = &current->thread;
	unsigned int old;

	if (level > 3)
		return -EINVAL;

	old = t->iopl_emul;

	/* No point in going further if nothing changes */
	if (level == old)
		return 0;

	/* Trying to gain more privileges? */
	if (level > old) {
		if (!capable(CAP_SYS_RAWIO) ||
		    security_locked_down(LOCKDOWN_IOPORT))
			return -EPERM;
	}

	t->iopl_emul = level;
	task_update_io_bitmap(current);

	return 0;
}

#else /* CONFIG_X86_IOPL_IOPERM */

long ksys_ioperm(unsigned long from, unsigned long num, int turn_on)
{
	return -ENOSYS;
}
SYSCALL_DEFINE3(ioperm, unsigned long, from, unsigned long, num, int, turn_on)
{
	return -ENOSYS;
}

SYSCALL_DEFINE1(iopl, unsigned int, level)
{
	return -ENOSYS;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
