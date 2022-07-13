<<<<<<< HEAD
#ifndef _ASM_X86_NMI_H
#define _ASM_X86_NMI_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_NMI_H
#define _ASM_X86_NMI_H

#include <linux/irq_work.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pm.h>
#include <asm/irq.h>
#include <asm/io.h>

#ifdef CONFIG_X86_LOCAL_APIC

<<<<<<< HEAD
extern int avail_to_resrv_perfctr_nmi_bit(unsigned int);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int reserve_perfctr_nmi(unsigned int);
extern void release_perfctr_nmi(unsigned int);
extern int reserve_evntsel_nmi(unsigned int);
extern void release_evntsel_nmi(unsigned int);

<<<<<<< HEAD
struct ctl_table;
extern int proc_nmi_enabled(struct ctl_table *, int ,
			void __user *, size_t *, loff_t *);
extern int unknown_nmi_panic;

void arch_trigger_all_cpu_backtrace(void);
#define arch_trigger_all_cpu_backtrace arch_trigger_all_cpu_backtrace
#endif
=======
extern int unknown_nmi_panic;

#endif /* CONFIG_X86_LOCAL_APIC */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define NMI_FLAG_FIRST	1

enum {
	NMI_LOCAL=0,
	NMI_UNKNOWN,
<<<<<<< HEAD
=======
	NMI_SERR,
	NMI_IO_CHECK,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NMI_MAX
};

#define NMI_DONE	0
#define NMI_HANDLED	1

typedef int (*nmi_handler_t)(unsigned int, struct pt_regs *);

<<<<<<< HEAD
int register_nmi_handler(unsigned int, nmi_handler_t, unsigned long,
			 const char *);
=======
struct nmiaction {
	struct list_head	list;
	nmi_handler_t		handler;
	u64			max_duration;
	unsigned long		flags;
	const char		*name;
};

#define register_nmi_handler(t, fn, fg, n, init...)	\
({							\
	static struct nmiaction init fn##_na = {	\
		.list = LIST_HEAD_INIT(fn##_na.list),	\
		.handler = (fn),			\
		.name = (n),				\
		.flags = (fg),				\
	};						\
	__register_nmi_handler((t), &fn##_na);		\
})

int __register_nmi_handler(unsigned int, struct nmiaction *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void unregister_nmi_handler(unsigned int, const char *);

void stop_nmi(void);
void restart_nmi(void);
void local_touch_nmi(void);

#endif /* _ASM_X86_NMI_H */
