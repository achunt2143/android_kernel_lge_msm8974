<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/compr_mm.h
 *
 * Memory management for pre-boot and ramdisk uncompressors
 *
 * Authors: Alain Knaff <alain@knaff.lu>
 *
 */

#ifndef DECOMPR_MM_H
#define DECOMPR_MM_H

#ifdef STATIC

/* Code active when included from pre-boot environment: */

/*
 * Some architectures want to ensure there is no local data in their
 * pre-boot environment, so that data can arbitrarily relocated (via
 * GOT references).  This is achieved by defining STATIC_RW_DATA to
 * be null.
 */
#ifndef STATIC_RW_DATA
#define STATIC_RW_DATA static
#endif

<<<<<<< HEAD
=======
/*
 * When an architecture needs to share the malloc()/free() implementation
 * between compilation units, it needs to have non-local visibility.
 */
#ifndef MALLOC_VISIBLE
#define MALLOC_VISIBLE static
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* A trivial malloc implementation, adapted from
 *  malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 */
STATIC_RW_DATA unsigned long malloc_ptr;
STATIC_RW_DATA int malloc_count;

<<<<<<< HEAD
static void *malloc(int size)
=======
MALLOC_VISIBLE void *malloc(int size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void *p;

	if (size < 0)
		return NULL;
	if (!malloc_ptr)
		malloc_ptr = free_mem_ptr;

<<<<<<< HEAD
	malloc_ptr = (malloc_ptr + 3) & ~3;     /* Align */
=======
	malloc_ptr = (malloc_ptr + 7) & ~7;     /* Align */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	p = (void *)malloc_ptr;
	malloc_ptr += size;

	if (free_mem_end_ptr && malloc_ptr >= free_mem_end_ptr)
		return NULL;

	malloc_count++;
	return p;
}

<<<<<<< HEAD
static void free(void *where)
=======
MALLOC_VISIBLE void free(void *where)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	malloc_count--;
	if (!malloc_count)
		malloc_ptr = free_mem_ptr;
}

#define large_malloc(a) malloc(a)
#define large_free(a) free(a)

#define INIT

#else /* STATIC */

/* Code active when compiled standalone for use when loading ramdisk: */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

/* Use defines rather than static inline in order to avoid spurious
 * warnings when not needed (indeed large_malloc / large_free are not
 * needed by inflate */

#define malloc(a) kmalloc(a, GFP_KERNEL)
#define free(a) kfree(a)

#define large_malloc(a) vmalloc(a)
#define large_free(a) vfree(a)

#define INIT __init
#define STATIC

#include <linux/init.h>

#endif /* STATIC */

#endif /* DECOMPR_MM_H */
