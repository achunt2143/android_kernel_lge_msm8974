<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ARM_MMU_H
#define __ARM_MMU_H

#ifdef CONFIG_MMU

typedef struct {
#ifdef CONFIG_CPU_HAS_ASID
<<<<<<< HEAD
	u64 id;
#endif
	unsigned int kvm_seq;
	unsigned long	sigpage;
=======
	atomic64_t	id;
#else
	int		switch_pending;
#endif
	atomic_t	vmalloc_seq;
	unsigned long	sigpage;
#ifdef CONFIG_VDSO
	unsigned long	vdso;
#endif
#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long	exec_fdpic_loadmap;
	unsigned long	interp_fdpic_loadmap;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} mm_context_t;

#ifdef CONFIG_CPU_HAS_ASID
#define ASID_BITS	8
#define ASID_MASK	((~0ULL) << ASID_BITS)
<<<<<<< HEAD
#define ASID(mm)	((mm)->context.id & ~ASID_MASK)
=======
#define ASID(mm)	((unsigned int)((mm)->context.id.counter & ~ASID_MASK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define ASID(mm)	(0)
#endif

#else

/*
 * From nommu.h:
 *  Copyright (C) 2002, David McCullough <davidm@snapgear.com>
 *  modified for 2.6 by Hyok S. Choi <hyok.choi@samsung.com>
 */
typedef struct {
<<<<<<< HEAD
	unsigned long		end_brk;
=======
	unsigned long	end_brk;
#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long	exec_fdpic_loadmap;
	unsigned long	interp_fdpic_loadmap;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} mm_context_t;

#endif

<<<<<<< HEAD
/*
 * switch_mm() may do a full cache flush over the context switch,
 * so enable interrupts over the context switch to avoid high
 * latency.
 */
#define __ARCH_WANT_INTERRUPTS_ON_CTXSW

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
