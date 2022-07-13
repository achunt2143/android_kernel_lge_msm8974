<<<<<<< HEAD
#ifndef _ASM_X86_MMAN_H
#define _ASM_X86_MMAN_H

#define MAP_32BIT	0x40		/* only give out 32bit addresses */

#include <asm-generic/mman.h>

#endif /* _ASM_X86_MMAN_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_MMAN_H__
#define __ASM_MMAN_H__

#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
#define arch_calc_vm_prot_bits(prot, key) (		\
		((key) & 0x1 ? VM_PKEY_BIT0 : 0) |      \
		((key) & 0x2 ? VM_PKEY_BIT1 : 0) |      \
		((key) & 0x4 ? VM_PKEY_BIT2 : 0) |      \
		((key) & 0x8 ? VM_PKEY_BIT3 : 0))
#endif

#include <uapi/asm/mman.h>

#endif /* __ASM_MMAN_H__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
