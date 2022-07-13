<<<<<<< HEAD
#ifndef __ASM_MACH_GENERIC_KMALLOC_H
#define __ASM_MACH_GENERIC_KMALLOC_H


#ifndef CONFIG_DMA_COHERENT
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_MACH_GENERIC_KMALLOC_H
#define __ASM_MACH_GENERIC_KMALLOC_H

#ifdef CONFIG_DMA_NONCOHERENT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Total overkill for most systems but need as a safe default.
 * Set this one if any device in the system might do non-coherent DMA.
 */
#define ARCH_DMA_MINALIGN	128
#endif

#endif /* __ASM_MACH_GENERIC_KMALLOC_H */
