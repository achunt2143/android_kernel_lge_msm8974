<<<<<<< HEAD
/*
 * include/asm-m68knommu/flat.h -- uClinux flat-format executables
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * flat.h -- uClinux flat-format executables
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __M68KNOMMU_FLAT_H__
#define __M68KNOMMU_FLAT_H__

<<<<<<< HEAD
#define	flat_argvp_envp_on_stack()		1
#define	flat_old_ram_flag(flags)		(flags)
#define	flat_reloc_valid(reloc, size)		((reloc) <= (size))
#define	flat_get_addr_from_rp(rp, relval, flags, p)	get_unaligned(rp)
#define	flat_put_addr_at_rp(rp, val, relval)	put_unaligned(val,rp)
#define	flat_get_relocate_addr(rel)		(rel)
#define	flat_set_persistent(relval, p)		0
=======
#include <asm-generic/flat.h>

#define FLAT_PLAT_INIT(regs) \
	do { \
		if (current->mm) \
			(regs)->d5 = current->mm->start_data; \
	} while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __M68KNOMMU_FLAT_H__ */
