<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_GENERIC_MMU_H
#define __ASM_GENERIC_MMU_H

/*
 * This is the mmu.h header for nommu implementations.
 * Architectures with an MMU need something more complex.
 */
#ifndef __ASSEMBLY__
typedef struct {
<<<<<<< HEAD
	struct vm_list_struct	*vmlist;
	unsigned long		end_brk;
=======
	unsigned long		end_brk;

#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long		exec_fdpic_loadmap;
	unsigned long		interp_fdpic_loadmap;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} mm_context_t;
#endif

#endif /* __ASM_GENERIC_MMU_H */
