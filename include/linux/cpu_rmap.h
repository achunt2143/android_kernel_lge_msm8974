<<<<<<< HEAD
/*
 * cpu_rmap.c: CPU affinity reverse-map support
 * Copyright 2011 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LINUX_CPU_RMAP_H
#define __LINUX_CPU_RMAP_H

/*
 * cpu_rmap.c: CPU affinity reverse-map support
 * Copyright 2011 Solarflare Communications Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/cpumask.h>
#include <linux/gfp.h>
#include <linux/slab.h>
<<<<<<< HEAD

/**
 * struct cpu_rmap - CPU affinity reverse-map
 * @size: Number of objects to be reverse-mapped
 * @used: Number of objects added
=======
#include <linux/kref.h>

/**
 * struct cpu_rmap - CPU affinity reverse-map
 * @refcount: kref for object
 * @size: Number of objects to be reverse-mapped
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @obj: Pointer to array of object pointers
 * @near: For each CPU, the index and distance to the nearest object,
 *      based on affinity masks
 */
struct cpu_rmap {
<<<<<<< HEAD
	u16		size, used;
=======
	struct kref	refcount;
	u16		size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void		**obj;
	struct {
		u16	index;
		u16	dist;
<<<<<<< HEAD
	}		near[0];
=======
	}		near[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#define CPU_RMAP_DIST_INF 0xffff

extern struct cpu_rmap *alloc_cpu_rmap(unsigned int size, gfp_t flags);
<<<<<<< HEAD

/**
 * free_cpu_rmap - free CPU affinity reverse-map
 * @rmap: Reverse-map allocated with alloc_cpu_rmap(), or %NULL
 */
static inline void free_cpu_rmap(struct cpu_rmap *rmap)
{
	kfree(rmap);
}
=======
extern int cpu_rmap_put(struct cpu_rmap *rmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int cpu_rmap_add(struct cpu_rmap *rmap, void *obj);
extern int cpu_rmap_update(struct cpu_rmap *rmap, u16 index,
			   const struct cpumask *affinity);

static inline u16 cpu_rmap_lookup_index(struct cpu_rmap *rmap, unsigned int cpu)
{
	return rmap->near[cpu].index;
}

static inline void *cpu_rmap_lookup_obj(struct cpu_rmap *rmap, unsigned int cpu)
{
	return rmap->obj[rmap->near[cpu].index];
}

<<<<<<< HEAD
#ifdef CONFIG_GENERIC_HARDIRQS

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * alloc_irq_cpu_rmap - allocate CPU affinity reverse-map for IRQs
 * @size: Number of objects to be mapped
 *
 * Must be called in process context.
 */
static inline struct cpu_rmap *alloc_irq_cpu_rmap(unsigned int size)
{
	return alloc_cpu_rmap(size, GFP_KERNEL);
}
extern void free_irq_cpu_rmap(struct cpu_rmap *rmap);

<<<<<<< HEAD
extern int irq_cpu_rmap_add(struct cpu_rmap *rmap, int irq);

#endif
=======
int irq_cpu_rmap_remove(struct cpu_rmap *rmap, int irq);
extern int irq_cpu_rmap_add(struct cpu_rmap *rmap, int irq);

#endif /* __LINUX_CPU_RMAP_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
