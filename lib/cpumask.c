<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/cpumask.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/bootmem.h>

int __first_cpu(const cpumask_t *srcp)
{
	return min_t(int, NR_CPUS, find_first_bit(srcp->bits, NR_CPUS));
}
EXPORT_SYMBOL(__first_cpu);

int __next_cpu(int n, const cpumask_t *srcp)
{
	return min_t(int, NR_CPUS, find_next_bit(srcp->bits, NR_CPUS, n+1));
}
EXPORT_SYMBOL(__next_cpu);

#if NR_CPUS > 64
int __next_cpu_nr(int n, const cpumask_t *srcp)
{
	return min_t(int, nr_cpu_ids,
				find_next_bit(srcp->bits, nr_cpu_ids, n+1));
}
EXPORT_SYMBOL(__next_cpu_nr);
#endif

/**
 * cpumask_next_and - get the next cpu in *src1p & *src2p
 * @n: the cpu prior to the place to search (ie. return will be > @n)
 * @src1p: the first cpumask pointer
 * @src2p: the second cpumask pointer
 *
 * Returns >= nr_cpu_ids if no further cpus set in both.
 */
int cpumask_next_and(int n, const struct cpumask *src1p,
		     const struct cpumask *src2p)
{
	while ((n = cpumask_next(n, src1p)) < nr_cpu_ids)
		if (cpumask_test_cpu(n, src2p))
			break;
	return n;
}
EXPORT_SYMBOL(cpumask_next_and);

/**
 * cpumask_any_but - return a "random" in a cpumask, but not this one.
 * @mask: the cpumask to search
 * @cpu: the cpu to ignore.
 *
 * Often used to find any cpu but smp_processor_id() in a mask.
 * Returns >= nr_cpu_ids if no cpus set.
 */
int cpumask_any_but(const struct cpumask *mask, unsigned int cpu)
{
	unsigned int i;

	cpumask_check(cpu);
	for_each_cpu(i, mask)
		if (i != cpu)
			break;
	return i;
}
=======
#include <linux/memblock.h>
#include <linux/numa.h>

/**
 * cpumask_next_wrap - helper to implement for_each_cpu_wrap
 * @n: the cpu prior to the place to search
 * @mask: the cpumask pointer
 * @start: the start point of the iteration
 * @wrap: assume @n crossing @start terminates the iteration
 *
 * Return: >= nr_cpu_ids on completion
 *
 * Note: the @wrap argument is required for the start condition when
 * we cannot assume @start is set in @mask.
 */
unsigned int cpumask_next_wrap(int n, const struct cpumask *mask, int start, bool wrap)
{
	unsigned int next;

again:
	next = cpumask_next(n, mask);

	if (wrap && n < start && next >= start) {
		return nr_cpumask_bits;

	} else if (next >= nr_cpumask_bits) {
		wrap = true;
		n = -1;
		goto again;
	}

	return next;
}
EXPORT_SYMBOL(cpumask_next_wrap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* These are not inline because of header tangles. */
#ifdef CONFIG_CPUMASK_OFFSTACK
/**
 * alloc_cpumask_var_node - allocate a struct cpumask on a given node
 * @mask: pointer to cpumask_var_t where the cpumask is returned
 * @flags: GFP_ flags
<<<<<<< HEAD
 *
 * Only defined when CONFIG_CPUMASK_OFFSTACK=y, otherwise is
 * a nop returning a constant 1 (in <linux/cpumask.h>)
 * Returns TRUE if memory allocation succeeded, FALSE otherwise.
=======
 * @node: memory node from which to allocate or %NUMA_NO_NODE
 *
 * Only defined when CONFIG_CPUMASK_OFFSTACK=y, otherwise is
 * a nop returning a constant 1 (in <linux/cpumask.h>).
 *
 * Return: TRUE if memory allocation succeeded, FALSE otherwise.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * In addition, mask will be NULL if this fails.  Note that gcc is
 * usually smart enough to know that mask can never be NULL if
 * CONFIG_CPUMASK_OFFSTACK=n, so does code elimination in that case
 * too.
 */
bool alloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags, int node)
{
	*mask = kmalloc_node(cpumask_size(), flags, node);

#ifdef CONFIG_DEBUG_PER_CPU_MAPS
	if (!*mask) {
		printk(KERN_ERR "=> alloc_cpumask_var: failed!\n");
		dump_stack();
	}
#endif
<<<<<<< HEAD
	/* FIXME: Bandaid to save us from old primitives which go to NR_CPUS. */
	if (*mask) {
		unsigned char *ptr = (unsigned char *)cpumask_bits(*mask);
		unsigned int tail;
		tail = BITS_TO_LONGS(NR_CPUS - nr_cpumask_bits) * sizeof(long);
		memset(ptr + cpumask_size() - tail, 0, tail);
	}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return *mask != NULL;
}
EXPORT_SYMBOL(alloc_cpumask_var_node);

<<<<<<< HEAD
bool zalloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags, int node)
{
	return alloc_cpumask_var_node(mask, flags | __GFP_ZERO, node);
}
EXPORT_SYMBOL(zalloc_cpumask_var_node);

/**
 * alloc_cpumask_var - allocate a struct cpumask
 * @mask: pointer to cpumask_var_t where the cpumask is returned
 * @flags: GFP_ flags
 *
 * Only defined when CONFIG_CPUMASK_OFFSTACK=y, otherwise is
 * a nop returning a constant 1 (in <linux/cpumask.h>).
 *
 * See alloc_cpumask_var_node.
 */
bool alloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
{
	return alloc_cpumask_var_node(mask, flags, NUMA_NO_NODE);
}
EXPORT_SYMBOL(alloc_cpumask_var);

bool zalloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
{
	return alloc_cpumask_var(mask, flags | __GFP_ZERO);
}
EXPORT_SYMBOL(zalloc_cpumask_var);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * alloc_bootmem_cpumask_var - allocate a struct cpumask from the bootmem arena.
 * @mask: pointer to cpumask_var_t where the cpumask is returned
 *
 * Only defined when CONFIG_CPUMASK_OFFSTACK=y, otherwise is
 * a nop (in <linux/cpumask.h>).
 * Either returns an allocated (zero-filled) cpumask, or causes the
 * system to panic.
 */
void __init alloc_bootmem_cpumask_var(cpumask_var_t *mask)
{
<<<<<<< HEAD
	*mask = alloc_bootmem(cpumask_size());
=======
	*mask = memblock_alloc(cpumask_size(), SMP_CACHE_BYTES);
	if (!*mask)
		panic("%s: Failed to allocate %u bytes\n", __func__,
		      cpumask_size());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * free_cpumask_var - frees memory allocated for a struct cpumask.
 * @mask: cpumask to free
 *
 * This is safe on a NULL mask.
 */
void free_cpumask_var(cpumask_var_t mask)
{
	kfree(mask);
}
EXPORT_SYMBOL(free_cpumask_var);

/**
 * free_bootmem_cpumask_var - frees result of alloc_bootmem_cpumask_var
 * @mask: cpumask to free
 */
void __init free_bootmem_cpumask_var(cpumask_var_t mask)
{
<<<<<<< HEAD
	free_bootmem((unsigned long)mask, cpumask_size());
}
#endif
=======
	memblock_free(mask, cpumask_size());
}
#endif

/**
 * cpumask_local_spread - select the i'th cpu based on NUMA distances
 * @i: index number
 * @node: local numa_node
 *
 * Return: online CPU according to a numa aware policy; local cpus are returned
 * first, followed by non-local ones, then it wraps around.
 *
 * For those who wants to enumerate all CPUs based on their NUMA distances,
 * i.e. call this function in a loop, like:
 *
 * for (i = 0; i < num_online_cpus(); i++) {
 *	cpu = cpumask_local_spread(i, node);
 *	do_something(cpu);
 * }
 *
 * There's a better alternative based on for_each()-like iterators:
 *
 *	for_each_numa_hop_mask(mask, node) {
 *		for_each_cpu_andnot(cpu, mask, prev)
 *			do_something(cpu);
 *		prev = mask;
 *	}
 *
 * It's simpler and more verbose than above. Complexity of iterator-based
 * enumeration is O(sched_domains_numa_levels * nr_cpu_ids), while
 * cpumask_local_spread() when called for each cpu is
 * O(sched_domains_numa_levels * nr_cpu_ids * log(nr_cpu_ids)).
 */
unsigned int cpumask_local_spread(unsigned int i, int node)
{
	unsigned int cpu;

	/* Wrap: we always want a cpu. */
	i %= num_online_cpus();

	cpu = sched_numa_find_nth_cpu(cpu_online_mask, i, node);

	WARN_ON(cpu >= nr_cpu_ids);
	return cpu;
}
EXPORT_SYMBOL(cpumask_local_spread);

static DEFINE_PER_CPU(int, distribute_cpu_mask_prev);

/**
 * cpumask_any_and_distribute - Return an arbitrary cpu within src1p & src2p.
 * @src1p: first &cpumask for intersection
 * @src2p: second &cpumask for intersection
 *
 * Iterated calls using the same srcp1 and srcp2 will be distributed within
 * their intersection.
 *
 * Return: >= nr_cpu_ids if the intersection is empty.
 */
unsigned int cpumask_any_and_distribute(const struct cpumask *src1p,
			       const struct cpumask *src2p)
{
	unsigned int next, prev;

	/* NOTE: our first selection will skip 0. */
	prev = __this_cpu_read(distribute_cpu_mask_prev);

	next = find_next_and_bit_wrap(cpumask_bits(src1p), cpumask_bits(src2p),
					nr_cpumask_bits, prev + 1);
	if (next < nr_cpu_ids)
		__this_cpu_write(distribute_cpu_mask_prev, next);

	return next;
}
EXPORT_SYMBOL(cpumask_any_and_distribute);

/**
 * cpumask_any_distribute - Return an arbitrary cpu from srcp
 * @srcp: &cpumask for selection
 *
 * Return: >= nr_cpu_ids if the intersection is empty.
 */
unsigned int cpumask_any_distribute(const struct cpumask *srcp)
{
	unsigned int next, prev;

	/* NOTE: our first selection will skip 0. */
	prev = __this_cpu_read(distribute_cpu_mask_prev);
	next = find_next_bit_wrap(cpumask_bits(srcp), nr_cpumask_bits, prev + 1);
	if (next < nr_cpu_ids)
		__this_cpu_write(distribute_cpu_mask_prev, next);

	return next;
}
EXPORT_SYMBOL(cpumask_any_distribute);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
