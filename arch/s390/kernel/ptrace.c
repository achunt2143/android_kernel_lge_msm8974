<<<<<<< HEAD
/*
 *  Ptrace user space interface.
 *
 *    Copyright IBM Corp. 1999,2010
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  Ptrace user space interface.
 *
 *    Copyright IBM Corp. 1999, 2010
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Denis Joseph Barrow
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/sched.h>
=======
#include "asm/ptrace.h"
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task_stack.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <linux/user.h>
#include <linux/security.h>
#include <linux/audit.h>
#include <linux/signal.h>
#include <linux/elf.h>
#include <linux/regset.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
#include <linux/seccomp.h>
#include <linux/compat.h>
#include <trace/syscall.h>
#include <asm/segment.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/switch_to.h>
=======
#include <linux/seccomp.h>
#include <linux/compat.h>
#include <trace/syscall.h>
#include <asm/guarded_storage.h>
#include <asm/access-regs.h>
#include <asm/page.h>
#include <linux/uaccess.h>
#include <asm/unistd.h>
#include <asm/runtime_instr.h>
#include <asm/facility.h>
#include <asm/fpu.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "entry.h"

#ifdef CONFIG_COMPAT
#include "compat_ptrace.h"
#endif

<<<<<<< HEAD
#define CREATE_TRACE_POINTS
#include <trace/events/syscalls.h>

enum s390_regset {
	REGSET_GENERAL,
	REGSET_FP,
	REGSET_LAST_BREAK,
	REGSET_SYSTEM_CALL,
	REGSET_GENERAL_EXTENDED,
};

void update_per_regs(struct task_struct *task)
{
	struct pt_regs *regs = task_pt_regs(task);
	struct thread_struct *thread = &task->thread;
	struct per_regs old, new;

	/* Copy user specified PER registers */
	new.control = thread->per_user.control;
	new.start = thread->per_user.start;
	new.end = thread->per_user.end;

	/* merge TIF_SINGLE_STEP into user specified PER registers. */
	if (test_tsk_thread_flag(task, TIF_SINGLE_STEP)) {
		new.control |= PER_EVENT_IFETCH;
		new.start = 0;
		new.end = PSW_ADDR_INSN;
	}

	/* Take care of the PER enablement bit in the PSW. */
	if (!(new.control & PER_EVENT_MASK)) {
=======
void update_cr_regs(struct task_struct *task)
{
	struct pt_regs *regs = task_pt_regs(task);
	struct thread_struct *thread = &task->thread;
	union ctlreg0 cr0_old, cr0_new;
	union ctlreg2 cr2_old, cr2_new;
	int cr0_changed, cr2_changed;
	union {
		struct ctlreg regs[3];
		struct {
			struct ctlreg control;
			struct ctlreg start;
			struct ctlreg end;
		};
	} old, new;

	local_ctl_store(0, &cr0_old.reg);
	local_ctl_store(2, &cr2_old.reg);
	cr0_new = cr0_old;
	cr2_new = cr2_old;
	/* Take care of the enable/disable of transactional execution. */
	if (MACHINE_HAS_TE) {
		/* Set or clear transaction execution TXC bit 8. */
		cr0_new.tcx = 1;
		if (task->thread.per_flags & PER_FLAG_NO_TE)
			cr0_new.tcx = 0;
		/* Set or clear transaction execution TDC bits 62 and 63. */
		cr2_new.tdc = 0;
		if (task->thread.per_flags & PER_FLAG_TE_ABORT_RAND) {
			if (task->thread.per_flags & PER_FLAG_TE_ABORT_RAND_TEND)
				cr2_new.tdc = 1;
			else
				cr2_new.tdc = 2;
		}
	}
	/* Take care of enable/disable of guarded storage. */
	if (MACHINE_HAS_GS) {
		cr2_new.gse = 0;
		if (task->thread.gs_cb)
			cr2_new.gse = 1;
	}
	/* Load control register 0/2 iff changed */
	cr0_changed = cr0_new.val != cr0_old.val;
	cr2_changed = cr2_new.val != cr2_old.val;
	if (cr0_changed)
		local_ctl_load(0, &cr0_new.reg);
	if (cr2_changed)
		local_ctl_load(2, &cr2_new.reg);
	/* Copy user specified PER registers */
	new.control.val = thread->per_user.control;
	new.start.val = thread->per_user.start;
	new.end.val = thread->per_user.end;

	/* merge TIF_SINGLE_STEP into user specified PER registers. */
	if (test_tsk_thread_flag(task, TIF_SINGLE_STEP) ||
	    test_tsk_thread_flag(task, TIF_UPROBE_SINGLESTEP)) {
		if (test_tsk_thread_flag(task, TIF_BLOCK_STEP))
			new.control.val |= PER_EVENT_BRANCH;
		else
			new.control.val |= PER_EVENT_IFETCH;
		new.control.val |= PER_CONTROL_SUSPENSION;
		new.control.val |= PER_EVENT_TRANSACTION_END;
		if (test_tsk_thread_flag(task, TIF_UPROBE_SINGLESTEP))
			new.control.val |= PER_EVENT_IFETCH;
		new.start.val = 0;
		new.end.val = -1UL;
	}

	/* Take care of the PER enablement bit in the PSW. */
	if (!(new.control.val & PER_EVENT_MASK)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		regs->psw.mask &= ~PSW_MASK_PER;
		return;
	}
	regs->psw.mask |= PSW_MASK_PER;
<<<<<<< HEAD
	__ctl_store(old, 9, 11);
	if (memcmp(&new, &old, sizeof(struct per_regs)) != 0)
		__ctl_load(new, 9, 11);
=======
	__local_ctl_store(9, 11, old.regs);
	if (memcmp(&new, &old, sizeof(struct per_regs)) != 0)
		__local_ctl_load(9, 11, new.regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void user_enable_single_step(struct task_struct *task)
{
<<<<<<< HEAD
	set_tsk_thread_flag(task, TIF_SINGLE_STEP);
	if (task == current)
		update_per_regs(task);
=======
	clear_tsk_thread_flag(task, TIF_BLOCK_STEP);
	set_tsk_thread_flag(task, TIF_SINGLE_STEP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void user_disable_single_step(struct task_struct *task)
{
<<<<<<< HEAD
	clear_tsk_thread_flag(task, TIF_SINGLE_STEP);
	if (task == current)
		update_per_regs(task);
=======
	clear_tsk_thread_flag(task, TIF_BLOCK_STEP);
	clear_tsk_thread_flag(task, TIF_SINGLE_STEP);
}

void user_enable_block_step(struct task_struct *task)
{
	set_tsk_thread_flag(task, TIF_SINGLE_STEP);
	set_tsk_thread_flag(task, TIF_BLOCK_STEP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Called by kernel/ptrace.c when detaching..
 *
 * Clear all debugging related fields.
 */
void ptrace_disable(struct task_struct *task)
{
	memset(&task->thread.per_user, 0, sizeof(task->thread.per_user));
	memset(&task->thread.per_event, 0, sizeof(task->thread.per_event));
	clear_tsk_thread_flag(task, TIF_SINGLE_STEP);
	clear_tsk_thread_flag(task, TIF_PER_TRAP);
<<<<<<< HEAD
}

#ifndef CONFIG_64BIT
# define __ADDR_MASK 3
#else
# define __ADDR_MASK 7
#endif
=======
	task->thread.per_flags = 0;
}

#define __ADDR_MASK 7
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline unsigned long __peek_user_per(struct task_struct *child,
					    addr_t addr)
{
<<<<<<< HEAD
	struct per_struct_kernel *dummy = NULL;

	if (addr == (addr_t) &dummy->cr9)
		/* Control bits of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PER_EVENT_IFETCH : child->thread.per_user.control;
	else if (addr == (addr_t) &dummy->cr10)
		/* Start address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			0 : child->thread.per_user.start;
	else if (addr == (addr_t) &dummy->cr11)
		/* End address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PSW_ADDR_INSN : child->thread.per_user.end;
	else if (addr == (addr_t) &dummy->bits)
		/* Single-step bit. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			(1UL << (BITS_PER_LONG - 1)) : 0;
	else if (addr == (addr_t) &dummy->starting_addr)
		/* Start address of the user specified per set. */
		return child->thread.per_user.start;
	else if (addr == (addr_t) &dummy->ending_addr)
		/* End address of the user specified per set. */
		return child->thread.per_user.end;
	else if (addr == (addr_t) &dummy->perc_atmid)
		/* PER code, ATMID and AI of the last PER trap */
		return (unsigned long)
			child->thread.per_event.cause << (BITS_PER_LONG - 16);
	else if (addr == (addr_t) &dummy->address)
		/* Address of the last PER trap */
		return child->thread.per_event.address;
	else if (addr == (addr_t) &dummy->access_id)
=======
	if (addr == offsetof(struct per_struct_kernel, cr9))
		/* Control bits of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PER_EVENT_IFETCH : child->thread.per_user.control;
	else if (addr == offsetof(struct per_struct_kernel, cr10))
		/* Start address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			0 : child->thread.per_user.start;
	else if (addr == offsetof(struct per_struct_kernel, cr11))
		/* End address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			-1UL : child->thread.per_user.end;
	else if (addr == offsetof(struct per_struct_kernel, bits))
		/* Single-step bit. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			(1UL << (BITS_PER_LONG - 1)) : 0;
	else if (addr == offsetof(struct per_struct_kernel, starting_addr))
		/* Start address of the user specified per set. */
		return child->thread.per_user.start;
	else if (addr == offsetof(struct per_struct_kernel, ending_addr))
		/* End address of the user specified per set. */
		return child->thread.per_user.end;
	else if (addr == offsetof(struct per_struct_kernel, perc_atmid))
		/* PER code, ATMID and AI of the last PER trap */
		return (unsigned long)
			child->thread.per_event.cause << (BITS_PER_LONG - 16);
	else if (addr == offsetof(struct per_struct_kernel, address))
		/* Address of the last PER trap */
		return child->thread.per_event.address;
	else if (addr == offsetof(struct per_struct_kernel, access_id))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Access id of the last PER trap */
		return (unsigned long)
			child->thread.per_event.paid << (BITS_PER_LONG - 8);
	return 0;
}

/*
 * Read the word at offset addr from the user area of a process. The
 * trouble here is that the information is littered over different
 * locations. The process registers are found on the kernel stack,
 * the floating point stuff and the trace settings are stored in
 * the task structure. In addition the different structures in
 * struct user contain pad bytes that should be read as zeroes.
 * Lovely...
 */
static unsigned long __peek_user(struct task_struct *child, addr_t addr)
{
<<<<<<< HEAD
	struct user *dummy = NULL;
	addr_t offset, tmp;

	if (addr < (addr_t) &dummy->regs.acrs) {
=======
	addr_t offset, tmp;

	if (addr < offsetof(struct user, regs.acrs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * psw and gprs are stored on the stack
		 */
		tmp = *(addr_t *)((addr_t) &task_pt_regs(child)->psw + addr);
<<<<<<< HEAD
		if (addr == (addr_t) &dummy->regs.psw.mask)
			/* Return a clean psw mask. */
			tmp = psw_user_bits | (tmp & PSW_MASK_USER);

	} else if (addr < (addr_t) &dummy->regs.orig_gpr2) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy->regs.acrs;
#ifdef CONFIG_64BIT
=======
		if (addr == offsetof(struct user, regs.psw.mask)) {
			/* Return a clean psw mask. */
			tmp &= PSW_MASK_USER | PSW_MASK_RI;
			tmp |= PSW_USER_BITS;
		}

	} else if (addr < offsetof(struct user, regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - offsetof(struct user, regs.acrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Very special case: old & broken 64 bit gdb reading
		 * from acrs[15]. Result is a 64 bit value. Read the
		 * 32 bit acrs[15] value and shift it by 32. Sick...
		 */
<<<<<<< HEAD
		if (addr == (addr_t) &dummy->regs.acrs[15])
			tmp = ((unsigned long) child->thread.acrs[15]) << 32;
		else
#endif
		tmp = *(addr_t *)((addr_t) &child->thread.acrs + offset);

	} else if (addr == (addr_t) &dummy->regs.orig_gpr2) {
=======
		if (addr == offsetof(struct user, regs.acrs[15]))
			tmp = ((unsigned long) child->thread.acrs[15]) << 32;
		else
			tmp = *(addr_t *)((addr_t) &child->thread.acrs + offset);

	} else if (addr == offsetof(struct user, regs.orig_gpr2)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * orig_gpr2 is stored on the kernel stack
		 */
		tmp = (addr_t) task_pt_regs(child)->orig_gpr2;

<<<<<<< HEAD
	} else if (addr < (addr_t) &dummy->regs.fp_regs) {
=======
	} else if (addr < offsetof(struct user, regs.fp_regs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * prevent reads of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		tmp = 0;

<<<<<<< HEAD
	} else if (addr < (addr_t) (&dummy->regs.fp_regs + 1)) {
		/* 
		 * floating point regs. are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy->regs.fp_regs;
		tmp = *(addr_t *)((addr_t) &child->thread.fp_regs + offset);
		if (addr == (addr_t) &dummy->regs.fp_regs.fpc)
			tmp &= (unsigned long) FPC_VALID_MASK
				<< (BITS_PER_LONG - 32);

	} else if (addr < (addr_t) (&dummy->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy->regs.per_info;
=======
	} else if (addr == offsetof(struct user, regs.fp_regs.fpc)) {
		/*
		 * floating point control reg. is in the thread structure
		 */
		tmp = child->thread.ufpu.fpc;
		tmp <<= BITS_PER_LONG - 32;

	} else if (addr < offsetof(struct user, regs.fp_regs) + sizeof(s390_fp_regs)) {
		/*
		 * floating point regs. are in the child->thread.ufpu.vxrs array
		 */
		offset = addr - offsetof(struct user, regs.fp_regs.fprs);
		tmp = *(addr_t *)((addr_t)child->thread.ufpu.vxrs + 2 * offset);
	} else if (addr < offsetof(struct user, regs.per_info) + sizeof(per_struct)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= offsetof(struct user, regs.per_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmp = __peek_user_per(child, addr);

	} else
		tmp = 0;

	return tmp;
}

static int
peek_user(struct task_struct *child, addr_t addr, addr_t data)
{
	addr_t tmp, mask;

	/*
	 * Stupid gdb peeks/pokes the access registers in 64 bit with
	 * an alignment of 4. Programmers from hell...
	 */
	mask = __ADDR_MASK;
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	if (addr >= (addr_t) &((struct user *) NULL)->regs.acrs &&
	    addr < (addr_t) &((struct user *) NULL)->regs.orig_gpr2)
		mask = 3;
#endif
=======
	if (addr >= offsetof(struct user, regs.acrs) &&
	    addr < offsetof(struct user, regs.orig_gpr2))
		mask = 3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((addr & mask) || addr > sizeof(struct user) - __ADDR_MASK)
		return -EIO;

	tmp = __peek_user(child, addr);
	return put_user(tmp, (addr_t __user *) data);
}

static inline void __poke_user_per(struct task_struct *child,
				   addr_t addr, addr_t data)
{
<<<<<<< HEAD
	struct per_struct_kernel *dummy = NULL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * There are only three fields in the per_info struct that the
	 * debugger user can write to.
	 * 1) cr9: the debugger wants to set a new PER event mask
	 * 2) starting_addr: the debugger wants to set a new starting
	 *    address to use with the PER event mask.
	 * 3) ending_addr: the debugger wants to set a new ending
	 *    address to use with the PER event mask.
	 * The user specified PER event mask and the start and end
	 * addresses are used only if single stepping is not in effect.
	 * Writes to any other field in per_info are ignored.
	 */
<<<<<<< HEAD
	if (addr == (addr_t) &dummy->cr9)
		/* PER event mask of the user specified per set. */
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == (addr_t) &dummy->starting_addr)
		/* Starting address of the user specified per set. */
		child->thread.per_user.start = data;
	else if (addr == (addr_t) &dummy->ending_addr)
=======
	if (addr == offsetof(struct per_struct_kernel, cr9))
		/* PER event mask of the user specified per set. */
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == offsetof(struct per_struct_kernel, starting_addr))
		/* Starting address of the user specified per set. */
		child->thread.per_user.start = data;
	else if (addr == offsetof(struct per_struct_kernel, ending_addr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Ending address of the user specified per set. */
		child->thread.per_user.end = data;
}

/*
 * Write a word to the user area of a process at location addr. This
 * operation does have an additional problem compared to peek_user.
 * Stores to the program status word and on the floating point
 * control register needs to get checked for validity.
 */
static int __poke_user(struct task_struct *child, addr_t addr, addr_t data)
{
<<<<<<< HEAD
	struct user *dummy = NULL;
	addr_t offset;

	if (addr < (addr_t) &dummy->regs.acrs) {
		/*
		 * psw and gprs are stored on the stack
		 */
		if (addr == (addr_t) &dummy->regs.psw.mask &&
		    (((data^psw_user_bits) & ~PSW_MASK_USER) ||
		     (((data^psw_user_bits) & PSW_MASK_ASC) &&
		      ((data|psw_user_bits) & PSW_MASK_ASC) == PSW_MASK_ASC) ||
		     ((data & PSW_MASK_EA) && !(data & PSW_MASK_BA))))
			/* Invalid psw mask. */
			return -EINVAL;
		*(addr_t *)((addr_t) &task_pt_regs(child)->psw + addr) = data;

	} else if (addr < (addr_t) (&dummy->regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy->regs.acrs;
#ifdef CONFIG_64BIT
=======
	addr_t offset;


	if (addr < offsetof(struct user, regs.acrs)) {
		struct pt_regs *regs = task_pt_regs(child);
		/*
		 * psw and gprs are stored on the stack
		 */
		if (addr == offsetof(struct user, regs.psw.mask)) {
			unsigned long mask = PSW_MASK_USER;

			mask |= is_ri_task(child) ? PSW_MASK_RI : 0;
			if ((data ^ PSW_USER_BITS) & ~mask)
				/* Invalid psw mask. */
				return -EINVAL;
			if ((data & PSW_MASK_ASC) == PSW_ASC_HOME)
				/* Invalid address-space-control bits */
				return -EINVAL;
			if ((data & PSW_MASK_EA) && !(data & PSW_MASK_BA))
				/* Invalid addressing mode bits */
				return -EINVAL;
		}

		if (test_pt_regs_flag(regs, PIF_SYSCALL) &&
			addr == offsetof(struct user, regs.gprs[2])) {
			struct pt_regs *regs = task_pt_regs(child);

			regs->int_code = 0x20000 | (data & 0xffff);
		}
		*(addr_t *)((addr_t) &regs->psw + addr) = data;
	} else if (addr < offsetof(struct user, regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - offsetof(struct user, regs.acrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Very special case: old & broken 64 bit gdb writing
		 * to acrs[15] with a 64 bit value. Ignore the lower
		 * half of the value and write the upper 32 bit to
		 * acrs[15]. Sick...
		 */
<<<<<<< HEAD
		if (addr == (addr_t) &dummy->regs.acrs[15])
			child->thread.acrs[15] = (unsigned int) (data >> 32);
		else
#endif
		*(addr_t *)((addr_t) &child->thread.acrs + offset) = data;

	} else if (addr == (addr_t) &dummy->regs.orig_gpr2) {
=======
		if (addr == offsetof(struct user, regs.acrs[15]))
			child->thread.acrs[15] = (unsigned int) (data >> 32);
		else
			*(addr_t *)((addr_t) &child->thread.acrs + offset) = data;

	} else if (addr == offsetof(struct user, regs.orig_gpr2)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * orig_gpr2 is stored on the kernel stack
		 */
		task_pt_regs(child)->orig_gpr2 = data;

<<<<<<< HEAD
	} else if (addr < (addr_t) &dummy->regs.fp_regs) {
=======
	} else if (addr < offsetof(struct user, regs.fp_regs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * prevent writes of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		return 0;

<<<<<<< HEAD
	} else if (addr < (addr_t) (&dummy->regs.fp_regs + 1)) {
		/*
		 * floating point regs. are stored in the thread structure
		 */
		if (addr == (addr_t) &dummy->regs.fp_regs.fpc &&
		    (data & ~((unsigned long) FPC_VALID_MASK
			      << (BITS_PER_LONG - 32))) != 0)
			return -EINVAL;
		offset = addr - (addr_t) &dummy->regs.fp_regs;
		*(addr_t *)((addr_t) &child->thread.fp_regs + offset) = data;

	} else if (addr < (addr_t) (&dummy->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy->regs.per_info;
=======
	} else if (addr == offsetof(struct user, regs.fp_regs.fpc)) {
		/*
		 * floating point control reg. is in the thread structure
		 */
		if ((unsigned int)data != 0)
			return -EINVAL;
		child->thread.ufpu.fpc = data >> (BITS_PER_LONG - 32);

	} else if (addr < offsetof(struct user, regs.fp_regs) + sizeof(s390_fp_regs)) {
		/*
		 * floating point regs. are in the child->thread.ufpu.vxrs array
		 */
		offset = addr - offsetof(struct user, regs.fp_regs.fprs);
		*(addr_t *)((addr_t)child->thread.ufpu.vxrs + 2 * offset) = data;
	} else if (addr < offsetof(struct user, regs.per_info) + sizeof(per_struct)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= offsetof(struct user, regs.per_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__poke_user_per(child, addr, data);

	}

	return 0;
}

static int poke_user(struct task_struct *child, addr_t addr, addr_t data)
{
	addr_t mask;

	/*
	 * Stupid gdb peeks/pokes the access registers in 64 bit with
	 * an alignment of 4. Programmers from hell indeed...
	 */
	mask = __ADDR_MASK;
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	if (addr >= (addr_t) &((struct user *) NULL)->regs.acrs &&
	    addr < (addr_t) &((struct user *) NULL)->regs.orig_gpr2)
		mask = 3;
#endif
=======
	if (addr >= offsetof(struct user, regs.acrs) &&
	    addr < offsetof(struct user, regs.orig_gpr2))
		mask = 3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((addr & mask) || addr > sizeof(struct user) - __ADDR_MASK)
		return -EIO;

	return __poke_user(child, addr, data);
}

long arch_ptrace(struct task_struct *child, long request,
		 unsigned long addr, unsigned long data)
{
	ptrace_area parea; 
	int copied, ret;

	switch (request) {
	case PTRACE_PEEKUSR:
		/* read the word at location addr in the USER area. */
		return peek_user(child, addr, data);

	case PTRACE_POKEUSR:
		/* write the word at location addr in the USER area */
		return poke_user(child, addr, data);

	case PTRACE_PEEKUSR_AREA:
	case PTRACE_POKEUSR_AREA:
		if (copy_from_user(&parea, (void __force __user *) addr,
							sizeof(parea)))
			return -EFAULT;
		addr = parea.kernel_addr;
		data = parea.process_addr;
		copied = 0;
		while (copied < parea.len) {
			if (request == PTRACE_PEEKUSR_AREA)
				ret = peek_user(child, addr, data);
			else {
				addr_t utmp;
				if (get_user(utmp,
					     (addr_t __force __user *) data))
					return -EFAULT;
				ret = poke_user(child, addr, utmp);
			}
			if (ret)
				return ret;
			addr += sizeof(unsigned long);
			data += sizeof(unsigned long);
			copied += sizeof(unsigned long);
		}
		return 0;
	case PTRACE_GET_LAST_BREAK:
<<<<<<< HEAD
		put_user(task_thread_info(child)->last_break,
			 (unsigned long __user *) data);
		return 0;
	default:
		/* Removing high order bit from addr (only for 31 bit). */
		addr &= PSW_ADDR_INSN;
=======
		return put_user(child->thread.last_break, (unsigned long __user *)data);
	case PTRACE_ENABLE_TE:
		if (!MACHINE_HAS_TE)
			return -EIO;
		child->thread.per_flags &= ~PER_FLAG_NO_TE;
		return 0;
	case PTRACE_DISABLE_TE:
		if (!MACHINE_HAS_TE)
			return -EIO;
		child->thread.per_flags |= PER_FLAG_NO_TE;
		child->thread.per_flags &= ~PER_FLAG_TE_ABORT_RAND;
		return 0;
	case PTRACE_TE_ABORT_RAND:
		if (!MACHINE_HAS_TE || (child->thread.per_flags & PER_FLAG_NO_TE))
			return -EIO;
		switch (data) {
		case 0UL:
			child->thread.per_flags &= ~PER_FLAG_TE_ABORT_RAND;
			break;
		case 1UL:
			child->thread.per_flags |= PER_FLAG_TE_ABORT_RAND;
			child->thread.per_flags |= PER_FLAG_TE_ABORT_RAND_TEND;
			break;
		case 2UL:
			child->thread.per_flags |= PER_FLAG_TE_ABORT_RAND;
			child->thread.per_flags &= ~PER_FLAG_TE_ABORT_RAND_TEND;
			break;
		default:
			return -EINVAL;
		}
		return 0;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ptrace_request(child, request, addr, data);
	}
}

#ifdef CONFIG_COMPAT
/*
 * Now the fun part starts... a 31 bit program running in the
 * 31 bit emulation tracing another program. PTRACE_PEEKTEXT,
 * PTRACE_PEEKDATA, PTRACE_POKETEXT and PTRACE_POKEDATA are easy
 * to handle, the difference to the 64 bit versions of the requests
 * is that the access is done in multiples of 4 byte instead of
 * 8 bytes (sizeof(unsigned long) on 31/64 bit).
 * The ugly part are PTRACE_PEEKUSR, PTRACE_PEEKUSR_AREA,
 * PTRACE_POKEUSR and PTRACE_POKEUSR_AREA. If the traced program
 * is a 31 bit program too, the content of struct user can be
 * emulated. A 31 bit program peeking into the struct user of
 * a 64 bit program is a no-no.
 */

/*
 * Same as peek_user_per but for a 31 bit program.
 */
static inline __u32 __peek_user_per_compat(struct task_struct *child,
					   addr_t addr)
{
<<<<<<< HEAD
	struct compat_per_struct_kernel *dummy32 = NULL;

	if (addr == (addr_t) &dummy32->cr9)
		/* Control bits of the active per set. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			PER_EVENT_IFETCH : child->thread.per_user.control;
	else if (addr == (addr_t) &dummy32->cr10)
		/* Start address of the active per set. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			0 : child->thread.per_user.start;
	else if (addr == (addr_t) &dummy32->cr11)
		/* End address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PSW32_ADDR_INSN : child->thread.per_user.end;
	else if (addr == (addr_t) &dummy32->bits)
		/* Single-step bit. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			0x80000000 : 0;
	else if (addr == (addr_t) &dummy32->starting_addr)
		/* Start address of the user specified per set. */
		return (__u32) child->thread.per_user.start;
	else if (addr == (addr_t) &dummy32->ending_addr)
		/* End address of the user specified per set. */
		return (__u32) child->thread.per_user.end;
	else if (addr == (addr_t) &dummy32->perc_atmid)
		/* PER code, ATMID and AI of the last PER trap */
		return (__u32) child->thread.per_event.cause << 16;
	else if (addr == (addr_t) &dummy32->address)
		/* Address of the last PER trap */
		return (__u32) child->thread.per_event.address;
	else if (addr == (addr_t) &dummy32->access_id)
=======
	if (addr == offsetof(struct compat_per_struct_kernel, cr9))
		/* Control bits of the active per set. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			PER_EVENT_IFETCH : child->thread.per_user.control;
	else if (addr == offsetof(struct compat_per_struct_kernel, cr10))
		/* Start address of the active per set. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			0 : child->thread.per_user.start;
	else if (addr == offsetof(struct compat_per_struct_kernel, cr11))
		/* End address of the active per set. */
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PSW32_ADDR_INSN : child->thread.per_user.end;
	else if (addr == offsetof(struct compat_per_struct_kernel, bits))
		/* Single-step bit. */
		return (__u32) test_thread_flag(TIF_SINGLE_STEP) ?
			0x80000000 : 0;
	else if (addr == offsetof(struct compat_per_struct_kernel, starting_addr))
		/* Start address of the user specified per set. */
		return (__u32) child->thread.per_user.start;
	else if (addr == offsetof(struct compat_per_struct_kernel, ending_addr))
		/* End address of the user specified per set. */
		return (__u32) child->thread.per_user.end;
	else if (addr == offsetof(struct compat_per_struct_kernel, perc_atmid))
		/* PER code, ATMID and AI of the last PER trap */
		return (__u32) child->thread.per_event.cause << 16;
	else if (addr == offsetof(struct compat_per_struct_kernel, address))
		/* Address of the last PER trap */
		return (__u32) child->thread.per_event.address;
	else if (addr == offsetof(struct compat_per_struct_kernel, access_id))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Access id of the last PER trap */
		return (__u32) child->thread.per_event.paid << 24;
	return 0;
}

/*
 * Same as peek_user but for a 31 bit program.
 */
static u32 __peek_user_compat(struct task_struct *child, addr_t addr)
{
<<<<<<< HEAD
	struct compat_user *dummy32 = NULL;
	addr_t offset;
	__u32 tmp;

	if (addr < (addr_t) &dummy32->regs.acrs) {
=======
	addr_t offset;
	__u32 tmp;

	if (addr < offsetof(struct compat_user, regs.acrs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct pt_regs *regs = task_pt_regs(child);
		/*
		 * psw and gprs are stored on the stack
		 */
<<<<<<< HEAD
		if (addr == (addr_t) &dummy32->regs.psw.mask) {
			/* Fake a 31 bit psw mask. */
			tmp = (__u32)(regs->psw.mask >> 32);
			tmp = psw32_user_bits | (tmp & PSW32_MASK_USER);
		} else if (addr == (addr_t) &dummy32->regs.psw.addr) {
=======
		if (addr == offsetof(struct compat_user, regs.psw.mask)) {
			/* Fake a 31 bit psw mask. */
			tmp = (__u32)(regs->psw.mask >> 32);
			tmp &= PSW32_MASK_USER | PSW32_MASK_RI;
			tmp |= PSW32_USER_BITS;
		} else if (addr == offsetof(struct compat_user, regs.psw.addr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Fake a 31 bit psw address. */
			tmp = (__u32) regs->psw.addr |
				(__u32)(regs->psw.mask & PSW_MASK_BA);
		} else {
			/* gpr 0-15 */
			tmp = *(__u32 *)((addr_t) &regs->psw + addr*2 + 4);
		}
<<<<<<< HEAD
	} else if (addr < (addr_t) (&dummy32->regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy32->regs.acrs;
		tmp = *(__u32*)((addr_t) &child->thread.acrs + offset);

	} else if (addr == (addr_t) (&dummy32->regs.orig_gpr2)) {
=======
	} else if (addr < offsetof(struct compat_user, regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - offsetof(struct compat_user, regs.acrs);
		tmp = *(__u32*)((addr_t) &child->thread.acrs + offset);

	} else if (addr == offsetof(struct compat_user, regs.orig_gpr2)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * orig_gpr2 is stored on the kernel stack
		 */
		tmp = *(__u32*)((addr_t) &task_pt_regs(child)->orig_gpr2 + 4);

<<<<<<< HEAD
	} else if (addr < (addr_t) &dummy32->regs.fp_regs) {
=======
	} else if (addr < offsetof(struct compat_user, regs.fp_regs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * prevent reads of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		tmp = 0;

<<<<<<< HEAD
	} else if (addr < (addr_t) (&dummy32->regs.fp_regs + 1)) {
		/*
		 * floating point regs. are stored in the thread structure 
		 */
	        offset = addr - (addr_t) &dummy32->regs.fp_regs;
		tmp = *(__u32 *)((addr_t) &child->thread.fp_regs + offset);

	} else if (addr < (addr_t) (&dummy32->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy32->regs.per_info;
=======
	} else if (addr == offsetof(struct compat_user, regs.fp_regs.fpc)) {
		/*
		 * floating point control reg. is in the thread structure
		 */
		tmp = child->thread.ufpu.fpc;

	} else if (addr < offsetof(struct compat_user, regs.fp_regs) + sizeof(s390_fp_regs)) {
		/*
		 * floating point regs. are in the child->thread.ufpu.vxrs array
		 */
		offset = addr - offsetof(struct compat_user, regs.fp_regs.fprs);
		tmp = *(__u32 *)((addr_t)child->thread.ufpu.vxrs + 2 * offset);
	} else if (addr < offsetof(struct compat_user, regs.per_info) + sizeof(struct compat_per_struct_kernel)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= offsetof(struct compat_user, regs.per_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmp = __peek_user_per_compat(child, addr);

	} else
		tmp = 0;

	return tmp;
}

static int peek_user_compat(struct task_struct *child,
			    addr_t addr, addr_t data)
{
	__u32 tmp;

	if (!is_compat_task() || (addr & 3) || addr > sizeof(struct user) - 3)
		return -EIO;

	tmp = __peek_user_compat(child, addr);
	return put_user(tmp, (__u32 __user *) data);
}

/*
 * Same as poke_user_per but for a 31 bit program.
 */
static inline void __poke_user_per_compat(struct task_struct *child,
					  addr_t addr, __u32 data)
{
<<<<<<< HEAD
	struct compat_per_struct_kernel *dummy32 = NULL;

	if (addr == (addr_t) &dummy32->cr9)
		/* PER event mask of the user specified per set. */
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == (addr_t) &dummy32->starting_addr)
		/* Starting address of the user specified per set. */
		child->thread.per_user.start = data;
	else if (addr == (addr_t) &dummy32->ending_addr)
=======
	if (addr == offsetof(struct compat_per_struct_kernel, cr9))
		/* PER event mask of the user specified per set. */
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == offsetof(struct compat_per_struct_kernel, starting_addr))
		/* Starting address of the user specified per set. */
		child->thread.per_user.start = data;
	else if (addr == offsetof(struct compat_per_struct_kernel, ending_addr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Ending address of the user specified per set. */
		child->thread.per_user.end = data;
}

/*
 * Same as poke_user but for a 31 bit program.
 */
static int __poke_user_compat(struct task_struct *child,
			      addr_t addr, addr_t data)
{
<<<<<<< HEAD
	struct compat_user *dummy32 = NULL;
	__u32 tmp = (__u32) data;
	addr_t offset;

	if (addr < (addr_t) &dummy32->regs.acrs) {
=======
	__u32 tmp = (__u32) data;
	addr_t offset;

	if (addr < offsetof(struct compat_user, regs.acrs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct pt_regs *regs = task_pt_regs(child);
		/*
		 * psw, gprs, acrs and orig_gpr2 are stored on the stack
		 */
<<<<<<< HEAD
		if (addr == (addr_t) &dummy32->regs.psw.mask) {
			/* Build a 64 bit psw mask from 31 bit mask. */
			if (((tmp^psw32_user_bits) & ~PSW32_MASK_USER) ||
			    (((tmp^psw32_user_bits) & PSW32_MASK_ASC) &&
			     ((tmp|psw32_user_bits) & PSW32_MASK_ASC)
			     == PSW32_MASK_ASC))
				/* Invalid psw mask. */
				return -EINVAL;
			regs->psw.mask = (regs->psw.mask & ~PSW_MASK_USER) |
				(regs->psw.mask & PSW_MASK_BA) |
				(__u64)(tmp & PSW32_MASK_USER) << 32;
		} else if (addr == (addr_t) &dummy32->regs.psw.addr) {
=======
		if (addr == offsetof(struct compat_user, regs.psw.mask)) {
			__u32 mask = PSW32_MASK_USER;

			mask |= is_ri_task(child) ? PSW32_MASK_RI : 0;
			/* Build a 64 bit psw mask from 31 bit mask. */
			if ((tmp ^ PSW32_USER_BITS) & ~mask)
				/* Invalid psw mask. */
				return -EINVAL;
			if ((data & PSW32_MASK_ASC) == PSW32_ASC_HOME)
				/* Invalid address-space-control bits */
				return -EINVAL;
			regs->psw.mask = (regs->psw.mask & ~PSW_MASK_USER) |
				(regs->psw.mask & PSW_MASK_BA) |
				(__u64)(tmp & mask) << 32;
		} else if (addr == offsetof(struct compat_user, regs.psw.addr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Build a 64 bit psw address from 31 bit address. */
			regs->psw.addr = (__u64) tmp & PSW32_ADDR_INSN;
			/* Transfer 31 bit amode bit to psw mask. */
			regs->psw.mask = (regs->psw.mask & ~PSW_MASK_BA) |
				(__u64)(tmp & PSW32_ADDR_AMODE);
		} else {
<<<<<<< HEAD
			/* gpr 0-15 */
			*(__u32*)((addr_t) &regs->psw + addr*2 + 4) = tmp;
		}
	} else if (addr < (addr_t) (&dummy32->regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy32->regs.acrs;
		*(__u32*)((addr_t) &child->thread.acrs + offset) = tmp;

	} else if (addr == (addr_t) (&dummy32->regs.orig_gpr2)) {
=======
			if (test_pt_regs_flag(regs, PIF_SYSCALL) &&
				addr == offsetof(struct compat_user, regs.gprs[2])) {
				struct pt_regs *regs = task_pt_regs(child);

				regs->int_code = 0x20000 | (data & 0xffff);
			}
			/* gpr 0-15 */
			*(__u32*)((addr_t) &regs->psw + addr*2 + 4) = tmp;
		}
	} else if (addr < offsetof(struct compat_user, regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
		offset = addr - offsetof(struct compat_user, regs.acrs);
		*(__u32*)((addr_t) &child->thread.acrs + offset) = tmp;

	} else if (addr == offsetof(struct compat_user, regs.orig_gpr2)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * orig_gpr2 is stored on the kernel stack
		 */
		*(__u32*)((addr_t) &task_pt_regs(child)->orig_gpr2 + 4) = tmp;

<<<<<<< HEAD
	} else if (addr < (addr_t) &dummy32->regs.fp_regs) {
=======
	} else if (addr < offsetof(struct compat_user, regs.fp_regs)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * prevent writess of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		return 0;

<<<<<<< HEAD
	} else if (addr < (addr_t) (&dummy32->regs.fp_regs + 1)) {
		/*
		 * floating point regs. are stored in the thread structure 
		 */
		if (addr == (addr_t) &dummy32->regs.fp_regs.fpc &&
		    (tmp & ~FPC_VALID_MASK) != 0)
			/* Invalid floating point control. */
			return -EINVAL;
	        offset = addr - (addr_t) &dummy32->regs.fp_regs;
		*(__u32 *)((addr_t) &child->thread.fp_regs + offset) = tmp;

	} else if (addr < (addr_t) (&dummy32->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy32->regs.per_info;
=======
	} else if (addr == offsetof(struct compat_user, regs.fp_regs.fpc)) {
		/*
		 * floating point control reg. is in the thread structure
		 */
		child->thread.ufpu.fpc = data;

	} else if (addr < offsetof(struct compat_user, regs.fp_regs) + sizeof(s390_fp_regs)) {
		/*
		 * floating point regs. are in the child->thread.ufpu.vxrs array
		 */
		offset = addr - offsetof(struct compat_user, regs.fp_regs.fprs);
		*(__u32 *)((addr_t)child->thread.ufpu.vxrs + 2 * offset) = tmp;
	} else if (addr < offsetof(struct compat_user, regs.per_info) + sizeof(struct compat_per_struct_kernel)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= offsetof(struct compat_user, regs.per_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__poke_user_per_compat(child, addr, data);
	}

	return 0;
}

static int poke_user_compat(struct task_struct *child,
			    addr_t addr, addr_t data)
{
	if (!is_compat_task() || (addr & 3) ||
	    addr > sizeof(struct compat_user) - 3)
		return -EIO;

	return __poke_user_compat(child, addr, data);
}

long compat_arch_ptrace(struct task_struct *child, compat_long_t request,
			compat_ulong_t caddr, compat_ulong_t cdata)
{
	unsigned long addr = caddr;
	unsigned long data = cdata;
	compat_ptrace_area parea;
	int copied, ret;

	switch (request) {
	case PTRACE_PEEKUSR:
		/* read the word at location addr in the USER area. */
		return peek_user_compat(child, addr, data);

	case PTRACE_POKEUSR:
		/* write the word at location addr in the USER area */
		return poke_user_compat(child, addr, data);

	case PTRACE_PEEKUSR_AREA:
	case PTRACE_POKEUSR_AREA:
		if (copy_from_user(&parea, (void __force __user *) addr,
							sizeof(parea)))
			return -EFAULT;
		addr = parea.kernel_addr;
		data = parea.process_addr;
		copied = 0;
		while (copied < parea.len) {
			if (request == PTRACE_PEEKUSR_AREA)
				ret = peek_user_compat(child, addr, data);
			else {
				__u32 utmp;
				if (get_user(utmp,
					     (__u32 __force __user *) data))
					return -EFAULT;
				ret = poke_user_compat(child, addr, utmp);
			}
			if (ret)
				return ret;
			addr += sizeof(unsigned int);
			data += sizeof(unsigned int);
			copied += sizeof(unsigned int);
		}
		return 0;
	case PTRACE_GET_LAST_BREAK:
<<<<<<< HEAD
		put_user(task_thread_info(child)->last_break,
			 (unsigned int __user *) data);
		return 0;
=======
		return put_user(child->thread.last_break, (unsigned int __user *)data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return compat_ptrace_request(child, request, addr, data);
}
#endif

<<<<<<< HEAD
asmlinkage long do_syscall_trace_enter(struct pt_regs *regs)
{
	long ret = 0;

	/* Do the secure computing check first. */
	secure_computing_strict(regs->gprs[2]);

	/*
	 * The sysc_tracesys code in entry.S stored the system
	 * call number to gprs[2].
	 */
	if (test_thread_flag(TIF_SYSCALL_TRACE) &&
	    (tracehook_report_syscall_entry(regs) ||
	     regs->gprs[2] >= NR_syscalls)) {
		/*
		 * Tracing decided this syscall should not happen or the
		 * debugger stored an invalid system call number. Skip
		 * the system call and the system call restart handling.
		 */
		clear_thread_flag(TIF_SYSCALL);
		ret = -1;
	}

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_enter(regs, regs->gprs[2]);

	audit_syscall_entry(is_compat_task() ?
				AUDIT_ARCH_S390 : AUDIT_ARCH_S390X,
			    regs->gprs[2], regs->orig_gpr2,
			    regs->gprs[3], regs->gprs[4],
			    regs->gprs[5]);
	return ret ?: regs->gprs[2];
}

asmlinkage void do_syscall_trace_exit(struct pt_regs *regs)
{
	audit_syscall_exit(regs);

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_exit(regs, regs->gprs[2]);

	if (test_thread_flag(TIF_SYSCALL_TRACE))
		tracehook_report_syscall_exit(regs, 0);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * user_regset definitions.
 */

static int s390_regs_get(struct task_struct *target,
			 const struct user_regset *regset,
<<<<<<< HEAD
			 unsigned int pos, unsigned int count,
			 void *kbuf, void __user *ubuf)
{
	if (target == current)
		save_access_regs(target->thread.acrs);

	if (kbuf) {
		unsigned long *k = kbuf;
		while (count > 0) {
			*k++ = __peek_user(target, pos);
			count -= sizeof(*k);
			pos += sizeof(*k);
		}
	} else {
		unsigned long __user *u = ubuf;
		while (count > 0) {
			if (__put_user(__peek_user(target, pos), u++))
				return -EFAULT;
			count -= sizeof(*u);
			pos += sizeof(*u);
		}
	}
=======
			 struct membuf to)
{
	unsigned pos;
	if (target == current)
		save_access_regs(target->thread.acrs);

	for (pos = 0; pos < sizeof(s390_regs); pos += sizeof(long))
		membuf_store(&to, __peek_user(target, pos));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int s390_regs_set(struct task_struct *target,
			 const struct user_regset *regset,
			 unsigned int pos, unsigned int count,
			 const void *kbuf, const void __user *ubuf)
{
	int rc = 0;

	if (target == current)
		save_access_regs(target->thread.acrs);

	if (kbuf) {
		const unsigned long *k = kbuf;
		while (count > 0 && !rc) {
			rc = __poke_user(target, pos, *k++);
			count -= sizeof(*k);
			pos += sizeof(*k);
		}
	} else {
		const unsigned long  __user *u = ubuf;
		while (count > 0 && !rc) {
			unsigned long word;
			rc = __get_user(word, u++);
			if (rc)
				break;
			rc = __poke_user(target, pos, word);
			count -= sizeof(*u);
			pos += sizeof(*u);
		}
	}

	if (rc == 0 && target == current)
		restore_access_regs(target->thread.acrs);

	return rc;
}

static int s390_fpregs_get(struct task_struct *target,
<<<<<<< HEAD
			   const struct user_regset *regset, unsigned int pos,
			   unsigned int count, void *kbuf, void __user *ubuf)
{
	if (target == current)
		save_fp_regs(&target->thread.fp_regs);

	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				   &target->thread.fp_regs, 0, -1);
=======
			   const struct user_regset *regset,
			   struct membuf to)
{
	_s390_fp_regs fp_regs;

	if (target == current)
		save_user_fpu_regs();

	fp_regs.fpc = target->thread.ufpu.fpc;
	fpregs_store(&fp_regs, &target->thread.ufpu);

	return membuf_write(&to, &fp_regs, sizeof(fp_regs));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int s390_fpregs_set(struct task_struct *target,
			   const struct user_regset *regset, unsigned int pos,
			   unsigned int count, const void *kbuf,
			   const void __user *ubuf)
{
	int rc = 0;
<<<<<<< HEAD

	if (target == current)
		save_fp_regs(&target->thread.fp_regs);

	/* If setting FPC, must validate it first. */
	if (count > 0 && pos < offsetof(s390_fp_regs, fprs)) {
		u32 fpc[2] = { target->thread.fp_regs.fpc, 0 };
		rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &fpc,
					0, offsetof(s390_fp_regs, fprs));
		if (rc)
			return rc;
		if ((fpc[0] & ~FPC_VALID_MASK) != 0 || fpc[1] != 0)
			return -EINVAL;
		target->thread.fp_regs.fpc = fpc[0];
=======
	freg_t fprs[__NUM_FPRS];

	if (target == current)
		save_user_fpu_regs();
	convert_vx_to_fp(fprs, target->thread.ufpu.vxrs);
	if (count > 0 && pos < offsetof(s390_fp_regs, fprs)) {
		u32 ufpc[2] = { target->thread.ufpu.fpc, 0 };
		rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf, &ufpc,
					0, offsetof(s390_fp_regs, fprs));
		if (rc)
			return rc;
		if (ufpc[1] != 0)
			return -EINVAL;
		target->thread.ufpu.fpc = ufpc[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (rc == 0 && count > 0)
		rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
<<<<<<< HEAD
					target->thread.fp_regs.fprs,
					offsetof(s390_fp_regs, fprs), -1);

	if (rc == 0 && target == current)
		restore_fp_regs(&target->thread.fp_regs);

	return rc;
}

#ifdef CONFIG_64BIT

static int s390_last_break_get(struct task_struct *target,
			       const struct user_regset *regset,
			       unsigned int pos, unsigned int count,
			       void *kbuf, void __user *ubuf)
{
	if (count > 0) {
		if (kbuf) {
			unsigned long *k = kbuf;
			*k = task_thread_info(target)->last_break;
		} else {
			unsigned long  __user *u = ubuf;
			if (__put_user(task_thread_info(target)->last_break, u))
				return -EFAULT;
		}
	}
	return 0;
=======
					fprs, offsetof(s390_fp_regs, fprs), -1);
	if (rc)
		return rc;
	convert_fp_to_vx(target->thread.ufpu.vxrs, fprs);
	return rc;
}

static int s390_last_break_get(struct task_struct *target,
			       const struct user_regset *regset,
			       struct membuf to)
{
	return membuf_store(&to, target->thread.last_break);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int s390_last_break_set(struct task_struct *target,
			       const struct user_regset *regset,
			       unsigned int pos, unsigned int count,
			       const void *kbuf, const void __user *ubuf)
{
	return 0;
}

<<<<<<< HEAD
#endif

static int s390_system_call_get(struct task_struct *target,
				const struct user_regset *regset,
				unsigned int pos, unsigned int count,
				void *kbuf, void __user *ubuf)
{
	unsigned int *data = &task_thread_info(target)->system_call;
	return user_regset_copyout(&pos, &count, &kbuf, &ubuf,
				   data, 0, sizeof(unsigned int));
=======
static int s390_tdb_get(struct task_struct *target,
			const struct user_regset *regset,
			struct membuf to)
{
	struct pt_regs *regs = task_pt_regs(target);
	size_t size;

	if (!(regs->int_code & 0x200))
		return -ENODATA;
	size = sizeof(target->thread.trap_tdb.data);
	return membuf_write(&to, target->thread.trap_tdb.data, size);
}

static int s390_tdb_set(struct task_struct *target,
			const struct user_regset *regset,
			unsigned int pos, unsigned int count,
			const void *kbuf, const void __user *ubuf)
{
	return 0;
}

static int s390_vxrs_low_get(struct task_struct *target,
			     const struct user_regset *regset,
			     struct membuf to)
{
	__u64 vxrs[__NUM_VXRS_LOW];
	int i;

	if (!cpu_has_vx())
		return -ENODEV;
	if (target == current)
		save_user_fpu_regs();
	for (i = 0; i < __NUM_VXRS_LOW; i++)
		vxrs[i] = target->thread.ufpu.vxrs[i].low;
	return membuf_write(&to, vxrs, sizeof(vxrs));
}

static int s390_vxrs_low_set(struct task_struct *target,
			     const struct user_regset *regset,
			     unsigned int pos, unsigned int count,
			     const void *kbuf, const void __user *ubuf)
{
	__u64 vxrs[__NUM_VXRS_LOW];
	int i, rc;

	if (!cpu_has_vx())
		return -ENODEV;
	if (target == current)
		save_user_fpu_regs();

	for (i = 0; i < __NUM_VXRS_LOW; i++)
		vxrs[i] = target->thread.ufpu.vxrs[i].low;

	rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf, vxrs, 0, -1);
	if (rc == 0)
		for (i = 0; i < __NUM_VXRS_LOW; i++)
			target->thread.ufpu.vxrs[i].low = vxrs[i];

	return rc;
}

static int s390_vxrs_high_get(struct task_struct *target,
			      const struct user_regset *regset,
			      struct membuf to)
{
	if (!cpu_has_vx())
		return -ENODEV;
	if (target == current)
		save_user_fpu_regs();
	return membuf_write(&to, target->thread.ufpu.vxrs + __NUM_VXRS_LOW,
			    __NUM_VXRS_HIGH * sizeof(__vector128));
}

static int s390_vxrs_high_set(struct task_struct *target,
			      const struct user_regset *regset,
			      unsigned int pos, unsigned int count,
			      const void *kbuf, const void __user *ubuf)
{
	int rc;

	if (!cpu_has_vx())
		return -ENODEV;
	if (target == current)
		save_user_fpu_regs();

	rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				target->thread.ufpu.vxrs + __NUM_VXRS_LOW, 0, -1);
	return rc;
}

static int s390_system_call_get(struct task_struct *target,
				const struct user_regset *regset,
				struct membuf to)
{
	return membuf_store(&to, target->thread.system_call);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int s390_system_call_set(struct task_struct *target,
				const struct user_regset *regset,
				unsigned int pos, unsigned int count,
				const void *kbuf, const void __user *ubuf)
{
<<<<<<< HEAD
	unsigned int *data = &task_thread_info(target)->system_call;
=======
	unsigned int *data = &target->thread.system_call;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				  data, 0, sizeof(unsigned int));
}

<<<<<<< HEAD
static const struct user_regset s390_regsets[] = {
	[REGSET_GENERAL] = {
=======
static int s390_gs_cb_get(struct task_struct *target,
			  const struct user_regset *regset,
			  struct membuf to)
{
	struct gs_cb *data = target->thread.gs_cb;

	if (!MACHINE_HAS_GS)
		return -ENODEV;
	if (!data)
		return -ENODATA;
	if (target == current)
		save_gs_cb(data);
	return membuf_write(&to, data, sizeof(struct gs_cb));
}

static int s390_gs_cb_set(struct task_struct *target,
			  const struct user_regset *regset,
			  unsigned int pos, unsigned int count,
			  const void *kbuf, const void __user *ubuf)
{
	struct gs_cb gs_cb = { }, *data = NULL;
	int rc;

	if (!MACHINE_HAS_GS)
		return -ENODEV;
	if (!target->thread.gs_cb) {
		data = kzalloc(sizeof(*data), GFP_KERNEL);
		if (!data)
			return -ENOMEM;
	}
	if (!target->thread.gs_cb)
		gs_cb.gsd = 25;
	else if (target == current)
		save_gs_cb(&gs_cb);
	else
		gs_cb = *target->thread.gs_cb;
	rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&gs_cb, 0, sizeof(gs_cb));
	if (rc) {
		kfree(data);
		return -EFAULT;
	}
	preempt_disable();
	if (!target->thread.gs_cb)
		target->thread.gs_cb = data;
	*target->thread.gs_cb = gs_cb;
	if (target == current) {
		local_ctl_set_bit(2, CR2_GUARDED_STORAGE_BIT);
		restore_gs_cb(target->thread.gs_cb);
	}
	preempt_enable();
	return rc;
}

static int s390_gs_bc_get(struct task_struct *target,
			  const struct user_regset *regset,
			  struct membuf to)
{
	struct gs_cb *data = target->thread.gs_bc_cb;

	if (!MACHINE_HAS_GS)
		return -ENODEV;
	if (!data)
		return -ENODATA;
	return membuf_write(&to, data, sizeof(struct gs_cb));
}

static int s390_gs_bc_set(struct task_struct *target,
			  const struct user_regset *regset,
			  unsigned int pos, unsigned int count,
			  const void *kbuf, const void __user *ubuf)
{
	struct gs_cb *data = target->thread.gs_bc_cb;

	if (!MACHINE_HAS_GS)
		return -ENODEV;
	if (!data) {
		data = kzalloc(sizeof(*data), GFP_KERNEL);
		if (!data)
			return -ENOMEM;
		target->thread.gs_bc_cb = data;
	}
	return user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				  data, 0, sizeof(struct gs_cb));
}

static bool is_ri_cb_valid(struct runtime_instr_cb *cb)
{
	return (cb->rca & 0x1f) == 0 &&
		(cb->roa & 0xfff) == 0 &&
		(cb->rla & 0xfff) == 0xfff &&
		cb->s == 1 &&
		cb->k == 1 &&
		cb->h == 0 &&
		cb->reserved1 == 0 &&
		cb->ps == 1 &&
		cb->qs == 0 &&
		cb->pc == 1 &&
		cb->qc == 0 &&
		cb->reserved2 == 0 &&
		cb->reserved3 == 0 &&
		cb->reserved4 == 0 &&
		cb->reserved5 == 0 &&
		cb->reserved6 == 0 &&
		cb->reserved7 == 0 &&
		cb->reserved8 == 0 &&
		cb->rla >= cb->roa &&
		cb->rca >= cb->roa &&
		cb->rca <= cb->rla+1 &&
		cb->m < 3;
}

static int s390_runtime_instr_get(struct task_struct *target,
				const struct user_regset *regset,
				struct membuf to)
{
	struct runtime_instr_cb *data = target->thread.ri_cb;

	if (!test_facility(64))
		return -ENODEV;
	if (!data)
		return -ENODATA;

	return membuf_write(&to, data, sizeof(struct runtime_instr_cb));
}

static int s390_runtime_instr_set(struct task_struct *target,
				  const struct user_regset *regset,
				  unsigned int pos, unsigned int count,
				  const void *kbuf, const void __user *ubuf)
{
	struct runtime_instr_cb ri_cb = { }, *data = NULL;
	int rc;

	if (!test_facility(64))
		return -ENODEV;

	if (!target->thread.ri_cb) {
		data = kzalloc(sizeof(*data), GFP_KERNEL);
		if (!data)
			return -ENOMEM;
	}

	if (target->thread.ri_cb) {
		if (target == current)
			store_runtime_instr_cb(&ri_cb);
		else
			ri_cb = *target->thread.ri_cb;
	}

	rc = user_regset_copyin(&pos, &count, &kbuf, &ubuf,
				&ri_cb, 0, sizeof(struct runtime_instr_cb));
	if (rc) {
		kfree(data);
		return -EFAULT;
	}

	if (!is_ri_cb_valid(&ri_cb)) {
		kfree(data);
		return -EINVAL;
	}
	/*
	 * Override access key in any case, since user space should
	 * not be able to set it, nor should it care about it.
	 */
	ri_cb.key = PAGE_DEFAULT_KEY >> 4;
	preempt_disable();
	if (!target->thread.ri_cb)
		target->thread.ri_cb = data;
	*target->thread.ri_cb = ri_cb;
	if (target == current)
		load_runtime_instr_cb(target->thread.ri_cb);
	preempt_enable();

	return 0;
}

static const struct user_regset s390_regsets[] = {
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_PRSTATUS,
		.n = sizeof(s390_regs) / sizeof(long),
		.size = sizeof(long),
		.align = sizeof(long),
<<<<<<< HEAD
		.get = s390_regs_get,
		.set = s390_regs_set,
	},
	[REGSET_FP] = {
=======
		.regset_get = s390_regs_get,
		.set = s390_regs_set,
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_PRFPREG,
		.n = sizeof(s390_fp_regs) / sizeof(long),
		.size = sizeof(long),
		.align = sizeof(long),
<<<<<<< HEAD
		.get = s390_fpregs_get,
		.set = s390_fpregs_set,
	},
#ifdef CONFIG_64BIT
	[REGSET_LAST_BREAK] = {
		.core_note_type = NT_S390_LAST_BREAK,
		.n = 1,
		.size = sizeof(long),
		.align = sizeof(long),
		.get = s390_last_break_get,
		.set = s390_last_break_set,
	},
#endif
	[REGSET_SYSTEM_CALL] = {
=======
		.regset_get = s390_fpregs_get,
		.set = s390_fpregs_set,
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_S390_SYSTEM_CALL,
		.n = 1,
		.size = sizeof(unsigned int),
		.align = sizeof(unsigned int),
<<<<<<< HEAD
		.get = s390_system_call_get,
		.set = s390_system_call_set,
	},
};

static const struct user_regset_view user_s390_view = {
	.name = UTS_MACHINE,
=======
		.regset_get = s390_system_call_get,
		.set = s390_system_call_set,
	},
	{
		.core_note_type = NT_S390_LAST_BREAK,
		.n = 1,
		.size = sizeof(long),
		.align = sizeof(long),
		.regset_get = s390_last_break_get,
		.set = s390_last_break_set,
	},
	{
		.core_note_type = NT_S390_TDB,
		.n = 1,
		.size = 256,
		.align = 1,
		.regset_get = s390_tdb_get,
		.set = s390_tdb_set,
	},
	{
		.core_note_type = NT_S390_VXRS_LOW,
		.n = __NUM_VXRS_LOW,
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_vxrs_low_get,
		.set = s390_vxrs_low_set,
	},
	{
		.core_note_type = NT_S390_VXRS_HIGH,
		.n = __NUM_VXRS_HIGH,
		.size = sizeof(__vector128),
		.align = sizeof(__vector128),
		.regset_get = s390_vxrs_high_get,
		.set = s390_vxrs_high_set,
	},
	{
		.core_note_type = NT_S390_GS_CB,
		.n = sizeof(struct gs_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_gs_cb_get,
		.set = s390_gs_cb_set,
	},
	{
		.core_note_type = NT_S390_GS_BC,
		.n = sizeof(struct gs_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_gs_bc_get,
		.set = s390_gs_bc_set,
	},
	{
		.core_note_type = NT_S390_RI_CB,
		.n = sizeof(struct runtime_instr_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_runtime_instr_get,
		.set = s390_runtime_instr_set,
	},
};

static const struct user_regset_view user_s390_view = {
	.name = "s390x",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.e_machine = EM_S390,
	.regsets = s390_regsets,
	.n = ARRAY_SIZE(s390_regsets)
};

#ifdef CONFIG_COMPAT
static int s390_compat_regs_get(struct task_struct *target,
				const struct user_regset *regset,
<<<<<<< HEAD
				unsigned int pos, unsigned int count,
				void *kbuf, void __user *ubuf)
{
	if (target == current)
		save_access_regs(target->thread.acrs);

	if (kbuf) {
		compat_ulong_t *k = kbuf;
		while (count > 0) {
			*k++ = __peek_user_compat(target, pos);
			count -= sizeof(*k);
			pos += sizeof(*k);
		}
	} else {
		compat_ulong_t __user *u = ubuf;
		while (count > 0) {
			if (__put_user(__peek_user_compat(target, pos), u++))
				return -EFAULT;
			count -= sizeof(*u);
			pos += sizeof(*u);
		}
	}
=======
				struct membuf to)
{
	unsigned n;

	if (target == current)
		save_access_regs(target->thread.acrs);

	for (n = 0; n < sizeof(s390_compat_regs); n += sizeof(compat_ulong_t))
		membuf_store(&to, __peek_user_compat(target, n));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int s390_compat_regs_set(struct task_struct *target,
				const struct user_regset *regset,
				unsigned int pos, unsigned int count,
				const void *kbuf, const void __user *ubuf)
{
	int rc = 0;

	if (target == current)
		save_access_regs(target->thread.acrs);

	if (kbuf) {
		const compat_ulong_t *k = kbuf;
		while (count > 0 && !rc) {
			rc = __poke_user_compat(target, pos, *k++);
			count -= sizeof(*k);
			pos += sizeof(*k);
		}
	} else {
		const compat_ulong_t  __user *u = ubuf;
		while (count > 0 && !rc) {
			compat_ulong_t word;
			rc = __get_user(word, u++);
			if (rc)
				break;
			rc = __poke_user_compat(target, pos, word);
			count -= sizeof(*u);
			pos += sizeof(*u);
		}
	}

	if (rc == 0 && target == current)
		restore_access_regs(target->thread.acrs);

	return rc;
}

static int s390_compat_regs_high_get(struct task_struct *target,
				     const struct user_regset *regset,
<<<<<<< HEAD
				     unsigned int pos, unsigned int count,
				     void *kbuf, void __user *ubuf)
{
	compat_ulong_t *gprs_high;

	gprs_high = (compat_ulong_t *)
		&task_pt_regs(target)->gprs[pos / sizeof(compat_ulong_t)];
	if (kbuf) {
		compat_ulong_t *k = kbuf;
		while (count > 0) {
			*k++ = *gprs_high;
			gprs_high += 2;
			count -= sizeof(*k);
		}
	} else {
		compat_ulong_t __user *u = ubuf;
		while (count > 0) {
			if (__put_user(*gprs_high, u++))
				return -EFAULT;
			gprs_high += 2;
			count -= sizeof(*u);
		}
	}
=======
				     struct membuf to)
{
	compat_ulong_t *gprs_high;
	int i;

	gprs_high = (compat_ulong_t *)task_pt_regs(target)->gprs;
	for (i = 0; i < NUM_GPRS; i++, gprs_high += 2)
		membuf_store(&to, *gprs_high);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int s390_compat_regs_high_set(struct task_struct *target,
				     const struct user_regset *regset,
				     unsigned int pos, unsigned int count,
				     const void *kbuf, const void __user *ubuf)
{
	compat_ulong_t *gprs_high;
	int rc = 0;

	gprs_high = (compat_ulong_t *)
		&task_pt_regs(target)->gprs[pos / sizeof(compat_ulong_t)];
	if (kbuf) {
		const compat_ulong_t *k = kbuf;
		while (count > 0) {
			*gprs_high = *k++;
			*gprs_high += 2;
			count -= sizeof(*k);
		}
	} else {
		const compat_ulong_t  __user *u = ubuf;
		while (count > 0 && !rc) {
			unsigned long word;
			rc = __get_user(word, u++);
			if (rc)
				break;
			*gprs_high = word;
			*gprs_high += 2;
			count -= sizeof(*u);
		}
	}

	return rc;
}

static int s390_compat_last_break_get(struct task_struct *target,
				      const struct user_regset *regset,
<<<<<<< HEAD
				      unsigned int pos, unsigned int count,
				      void *kbuf, void __user *ubuf)
{
	compat_ulong_t last_break;

	if (count > 0) {
		last_break = task_thread_info(target)->last_break;
		if (kbuf) {
			unsigned long *k = kbuf;
			*k = last_break;
		} else {
			unsigned long  __user *u = ubuf;
			if (__put_user(last_break, u))
				return -EFAULT;
		}
	}
	return 0;
=======
				      struct membuf to)
{
	compat_ulong_t last_break = target->thread.last_break;

	return membuf_store(&to, (unsigned long)last_break);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int s390_compat_last_break_set(struct task_struct *target,
				      const struct user_regset *regset,
				      unsigned int pos, unsigned int count,
				      const void *kbuf, const void __user *ubuf)
{
	return 0;
}

static const struct user_regset s390_compat_regsets[] = {
<<<<<<< HEAD
	[REGSET_GENERAL] = {
=======
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_PRSTATUS,
		.n = sizeof(s390_compat_regs) / sizeof(compat_long_t),
		.size = sizeof(compat_long_t),
		.align = sizeof(compat_long_t),
<<<<<<< HEAD
		.get = s390_compat_regs_get,
		.set = s390_compat_regs_set,
	},
	[REGSET_FP] = {
=======
		.regset_get = s390_compat_regs_get,
		.set = s390_compat_regs_set,
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_PRFPREG,
		.n = sizeof(s390_fp_regs) / sizeof(compat_long_t),
		.size = sizeof(compat_long_t),
		.align = sizeof(compat_long_t),
<<<<<<< HEAD
		.get = s390_fpregs_get,
		.set = s390_fpregs_set,
	},
	[REGSET_LAST_BREAK] = {
		.core_note_type = NT_S390_LAST_BREAK,
		.n = 1,
		.size = sizeof(long),
		.align = sizeof(long),
		.get = s390_compat_last_break_get,
		.set = s390_compat_last_break_set,
	},
	[REGSET_SYSTEM_CALL] = {
=======
		.regset_get = s390_fpregs_get,
		.set = s390_fpregs_set,
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_S390_SYSTEM_CALL,
		.n = 1,
		.size = sizeof(compat_uint_t),
		.align = sizeof(compat_uint_t),
<<<<<<< HEAD
		.get = s390_system_call_get,
		.set = s390_system_call_set,
	},
	[REGSET_GENERAL_EXTENDED] = {
=======
		.regset_get = s390_system_call_get,
		.set = s390_system_call_set,
	},
	{
		.core_note_type = NT_S390_LAST_BREAK,
		.n = 1,
		.size = sizeof(long),
		.align = sizeof(long),
		.regset_get = s390_compat_last_break_get,
		.set = s390_compat_last_break_set,
	},
	{
		.core_note_type = NT_S390_TDB,
		.n = 1,
		.size = 256,
		.align = 1,
		.regset_get = s390_tdb_get,
		.set = s390_tdb_set,
	},
	{
		.core_note_type = NT_S390_VXRS_LOW,
		.n = __NUM_VXRS_LOW,
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_vxrs_low_get,
		.set = s390_vxrs_low_set,
	},
	{
		.core_note_type = NT_S390_VXRS_HIGH,
		.n = __NUM_VXRS_HIGH,
		.size = sizeof(__vector128),
		.align = sizeof(__vector128),
		.regset_get = s390_vxrs_high_get,
		.set = s390_vxrs_high_set,
	},
	{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.core_note_type = NT_S390_HIGH_GPRS,
		.n = sizeof(s390_compat_regs_high) / sizeof(compat_long_t),
		.size = sizeof(compat_long_t),
		.align = sizeof(compat_long_t),
<<<<<<< HEAD
		.get = s390_compat_regs_high_get,
		.set = s390_compat_regs_high_set,
	},
=======
		.regset_get = s390_compat_regs_high_get,
		.set = s390_compat_regs_high_set,
	},
	{
		.core_note_type = NT_S390_GS_CB,
		.n = sizeof(struct gs_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_gs_cb_get,
		.set = s390_gs_cb_set,
	},
	{
		.core_note_type = NT_S390_GS_BC,
		.n = sizeof(struct gs_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_gs_bc_get,
		.set = s390_gs_bc_set,
	},
	{
		.core_note_type = NT_S390_RI_CB,
		.n = sizeof(struct runtime_instr_cb) / sizeof(__u64),
		.size = sizeof(__u64),
		.align = sizeof(__u64),
		.regset_get = s390_runtime_instr_get,
		.set = s390_runtime_instr_set,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct user_regset_view user_s390_compat_view = {
	.name = "s390",
	.e_machine = EM_S390,
	.regsets = s390_compat_regsets,
	.n = ARRAY_SIZE(s390_compat_regsets)
};
#endif

const struct user_regset_view *task_user_regset_view(struct task_struct *task)
{
#ifdef CONFIG_COMPAT
	if (test_tsk_thread_flag(task, TIF_31BIT))
		return &user_s390_compat_view;
#endif
	return &user_s390_view;
}

static const char *gpr_names[NUM_GPRS] = {
	"r0", "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
	"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
};

unsigned long regs_get_register(struct pt_regs *regs, unsigned int offset)
{
	if (offset >= NUM_GPRS)
		return 0;
	return regs->gprs[offset];
}

int regs_query_register_offset(const char *name)
{
	unsigned long offset;

	if (!name || *name != 'r')
		return -EINVAL;
<<<<<<< HEAD
	if (strict_strtoul(name + 1, 10, &offset))
=======
	if (kstrtoul(name + 1, 10, &offset))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	if (offset >= NUM_GPRS)
		return -EINVAL;
	return offset;
}

const char *regs_query_register_name(unsigned int offset)
{
	if (offset >= NUM_GPRS)
		return NULL;
	return gpr_names[offset];
}

static int regs_within_kernel_stack(struct pt_regs *regs, unsigned long addr)
{
	unsigned long ksp = kernel_stack_pointer(regs);

	return (addr & ~(THREAD_SIZE - 1)) == (ksp & ~(THREAD_SIZE - 1));
}

/**
 * regs_get_kernel_stack_nth() - get Nth entry of the stack
 * @regs:pt_regs which contains kernel stack pointer.
 * @n:stack entry number.
 *
 * regs_get_kernel_stack_nth() returns @n th entry of the kernel stack which
 * is specifined by @regs. If the @n th entry is NOT in the kernel stack,
 * this returns 0.
 */
unsigned long regs_get_kernel_stack_nth(struct pt_regs *regs, unsigned int n)
{
	unsigned long addr;

	addr = kernel_stack_pointer(regs) + n * sizeof(long);
	if (!regs_within_kernel_stack(regs, addr))
		return 0;
	return *(unsigned long *)addr;
}
