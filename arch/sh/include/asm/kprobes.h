<<<<<<< HEAD
#ifndef __ASM_SH_KPROBES_H
#define __ASM_SH_KPROBES_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_KPROBES_H
#define __ASM_SH_KPROBES_H

#include <asm-generic/kprobes.h>

#define BREAKPOINT_INSTRUCTION	0xc33a

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_KPROBES

#include <linux/types.h>
#include <linux/ptrace.h>

typedef insn_size_t kprobe_opcode_t;
<<<<<<< HEAD
#define BREAKPOINT_INSTRUCTION	0xc33a
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MAX_INSN_SIZE 16
#define MAX_STACK_SIZE 64
#define MIN_STACK_SIZE(ADDR) (((MAX_STACK_SIZE) < \
	(((unsigned long)current_thread_info()) + THREAD_SIZE - (ADDR))) \
	? (MAX_STACK_SIZE) \
	: (((unsigned long)current_thread_info()) + THREAD_SIZE - (ADDR)))

#define flush_insn_slot(p)		do { } while (0)
#define kretprobe_blacklist_size	0

struct kprobe;

void arch_remove_kprobe(struct kprobe *);
<<<<<<< HEAD
void kretprobe_trampoline(void);
void jprobe_return_end(void);
=======
void __kretprobe_trampoline(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Architecture specific copy of original instruction*/
struct arch_specific_insn {
	/* copy of the original instruction */
	kprobe_opcode_t insn[MAX_INSN_SIZE];
};

struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned long kprobe_status;
<<<<<<< HEAD
	unsigned long jprobe_saved_r15;
	struct pt_regs jprobe_saved_regs;
	kprobe_opcode_t jprobes_stack[MAX_STACK_SIZE];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct prev_kprobe prev_kprobe;
};

extern int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
<<<<<<< HEAD
extern int kprobe_exceptions_notify(struct notifier_block *self,
				    unsigned long val, void *data);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int kprobe_handle_illslot(unsigned long pc);
#else

#define kprobe_handle_illslot(pc)	(-1)

#endif /* CONFIG_KPROBES */
#endif /* __ASM_SH_KPROBES_H */
