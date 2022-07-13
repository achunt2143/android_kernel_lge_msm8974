<<<<<<< HEAD
#ifndef __UM_IRQFLAGS_H
#define __UM_IRQFLAGS_H

extern int get_signals(void);
extern int set_signals(int enable);
extern void block_signals(void);
extern void unblock_signals(void);

static inline unsigned long arch_local_save_flags(void)
{
	return get_signals();
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	set_signals(flags);
}

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_IRQFLAGS_H
#define __UM_IRQFLAGS_H

extern int signals_enabled;
int um_set_signals(int enable);
void block_signals(void);
void unblock_signals(void);

#define arch_local_save_flags arch_local_save_flags
static inline unsigned long arch_local_save_flags(void)
{
	return signals_enabled;
}

#define arch_local_irq_restore arch_local_irq_restore
static inline void arch_local_irq_restore(unsigned long flags)
{
	um_set_signals(flags);
}

#define arch_local_irq_enable arch_local_irq_enable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void arch_local_irq_enable(void)
{
	unblock_signals();
}

<<<<<<< HEAD
=======
#define arch_local_irq_disable arch_local_irq_disable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void arch_local_irq_disable(void)
{
	block_signals();
}

<<<<<<< HEAD
static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags;
	flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}

static inline bool arch_irqs_disabled(void)
{
	return arch_local_save_flags() == 0;
}
=======
#define ARCH_IRQ_DISABLED	0

#include <asm-generic/irqflags.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
