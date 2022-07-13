<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* $Id: cache.h,v 1.6 2004/03/11 18:08:05 lethal Exp $
 *
 * include/asm-sh/cache.h
 *
 * Copyright 1999 (C) Niibe Yutaka
 * Copyright 2002, 2003 (C) Paul Mundt
 */
#ifndef __ASM_SH_CACHE_H
#define __ASM_SH_CACHE_H
<<<<<<< HEAD
#ifdef __KERNEL__
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/init.h>
#include <cpu/cache.h>

#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)

<<<<<<< HEAD
#define __read_mostly __attribute__((__section__(".data..read_mostly")))
=======
/*
 * Some drivers need to perform DMA into kmalloc'ed buffers
 * and so we have to increase the kmalloc minalign for this.
 */
#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#define __read_mostly __section(".data..read_mostly")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __ASSEMBLY__
struct cache_info {
	unsigned int ways;		/* Number of cache ways */
	unsigned int sets;		/* Number of cache sets */
	unsigned int linesz;		/* Cache line size (bytes) */

	unsigned int way_size;		/* sets * line size */

	/*
	 * way_incr is the address offset for accessing the next way
	 * in memory mapped cache array ops.
	 */
	unsigned int way_incr;
	unsigned int entry_shift;
	unsigned int entry_mask;

	/*
	 * Compute a mask which selects the address bits which overlap between
	 * 1. those used to select the cache set during indexing
	 * 2. those in the physical page number.
	 */
	unsigned int alias_mask;
	unsigned int n_aliases;		/* Number of aliases */

	unsigned long flags;
};
#endif /* __ASSEMBLY__ */
<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_CACHE_H */
