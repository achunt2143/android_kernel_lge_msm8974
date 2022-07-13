/*
 * include/linux/topology.h
 *
 * Written by: Matthew Dobson, IBM Corporation
 *
 * Copyright (C) 2002, IBM Corp.
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <colpatch@us.ibm.com>
 */
#ifndef _LINUX_TOPOLOGY_H
#define _LINUX_TOPOLOGY_H

<<<<<<< HEAD
=======
#include <linux/arch_topology.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/cpumask.h>
#include <linux/bitops.h>
#include <linux/mmzone.h>
#include <linux/smp.h>
#include <linux/percpu.h>
#include <asm/topology.h>

<<<<<<< HEAD
#ifndef node_has_online_mem
#define node_has_online_mem(nid) (1)
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef nr_cpus_node
#define nr_cpus_node(node) cpumask_weight(cpumask_of_node(node))
#endif

#define for_each_node_with_cpus(node)			\
	for_each_online_node(node)			\
		if (nr_cpus_node(node))

int arch_update_cpu_topology(void);

/* Conform to ACPI 2.0 SLIT distance definitions */
#define LOCAL_DISTANCE		10
#define REMOTE_DISTANCE		20
<<<<<<< HEAD
=======
#define DISTANCE_BITS           8
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef node_distance
#define node_distance(from,to)	((from) == (to) ? LOCAL_DISTANCE : REMOTE_DISTANCE)
#endif
#ifndef RECLAIM_DISTANCE
/*
 * If the distance between nodes in a system is larger than RECLAIM_DISTANCE
 * (in whatever arch specific measurement units returned by node_distance())
<<<<<<< HEAD
 * then switch on zone reclaim on boot.
 */
#define RECLAIM_DISTANCE 30
#endif
=======
 * and node_reclaim_mode is enabled then the VM will only call node_reclaim()
 * on nodes within this distance.
 */
#define RECLAIM_DISTANCE 30
#endif

/*
 * The following tunable allows platforms to override the default node
 * reclaim distance (RECLAIM_DISTANCE) if remote memory accesses are
 * sufficiently fast that the default value actually hurts
 * performance.
 *
 * AMD EPYC machines use this because even though the 2-hop distance
 * is 32 (3.2x slower than a local memory access) performance actually
 * *improves* if allowed to reclaim memory and load balance tasks
 * between NUMA nodes 2-hops apart.
 */
extern int __read_mostly node_reclaim_distance;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef PENALTY_FOR_NODE_WITH_CPUS
#define PENALTY_FOR_NODE_WITH_CPUS	(1)
#endif

<<<<<<< HEAD
/*
 * Below are the 3 major initializers used in building sched_domains:
 * SD_SIBLING_INIT, for SMT domains
 * SD_CPU_INIT, for SMP domains
 * SD_NODE_INIT, for NUMA domains
 *
 * Any architecture that cares to do any tuning to these values should do so
 * by defining their own arch-specific initializer in include/asm/topology.h.
 * A definition there will automagically override these default initializers
 * and allow arch-specific performance tuning of sched_domains.
 * (Only non-zero and non-null fields need be specified.)
 */

#ifdef CONFIG_SCHED_SMT
/* MCD - Do we really need this?  It is always on if CONFIG_SCHED_SMT is,
 * so can't we drop this in favor of CONFIG_SCHED_SMT?
 */
#define ARCH_HAS_SCHED_WAKE_IDLE
/* Common values for SMT siblings */
#ifndef SD_SIBLING_INIT
#define SD_SIBLING_INIT (struct sched_domain) {				\
	.min_interval		= 1,					\
	.max_interval		= 2,					\
	.busy_factor		= 64,					\
	.imbalance_pct		= 110,					\
									\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 1*SD_BALANCE_EXEC			\
				| 1*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 1*SD_WAKE_AFFINE			\
				| 1*SD_SHARE_CPUPOWER			\
				| 0*SD_POWERSAVINGS_BALANCE		\
				| 1*SD_SHARE_PKG_RESOURCES		\
				| 0*SD_SERIALIZE			\
				| 0*SD_PREFER_SIBLING			\
				| arch_sd_sibling_asym_packing()	\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 1,					\
	.smt_gain		= 1178,	/* 15% */			\
}
#endif
#endif /* CONFIG_SCHED_SMT */

#ifdef CONFIG_SCHED_MC
/* Common values for MC siblings. for now mostly derived from SD_CPU_INIT */
#ifndef SD_MC_INIT
#define SD_MC_INIT (struct sched_domain) {				\
	.min_interval		= 1,					\
	.max_interval		= 4,					\
	.busy_factor		= 1,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 2,					\
	.wake_idx		= 0,					\
	.forkexec_idx		= 0,					\
									\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 1*SD_BALANCE_EXEC			\
				| 1*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 1*SD_WAKE_AFFINE			\
				| 0*SD_PREFER_LOCAL			\
				| 0*SD_SHARE_CPUPOWER			\
				| 1*SD_SHARE_PKG_RESOURCES		\
				| 0*SD_SERIALIZE			\
				| sd_balance_for_mc_power()		\
				| sd_power_saving_flags()		\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 1,					\
}
#endif
#endif /* CONFIG_SCHED_MC */

/* Common values for CPUs */
#ifndef SD_CPU_INIT
#define SD_CPU_INIT (struct sched_domain) {				\
	.min_interval		= 1,					\
	.max_interval		= 4,					\
	.busy_factor		= 64,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 2,					\
	.idle_idx		= 1,					\
	.newidle_idx		= 0,					\
	.wake_idx		= 0,					\
	.forkexec_idx		= 0,					\
									\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 1*SD_BALANCE_EXEC			\
				| 1*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 1*SD_WAKE_AFFINE			\
				| 0*SD_PREFER_LOCAL			\
				| 0*SD_SHARE_CPUPOWER			\
				| 1*SD_SHARE_PKG_RESOURCES		\
				| 0*SD_SERIALIZE			\
				| sd_balance_for_package_power()	\
				| sd_power_saving_flags()		\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 1,					\
}
#endif

/* sched_domains SD_ALLNODES_INIT for NUMA machines */
#define SD_ALLNODES_INIT (struct sched_domain) {			\
	.min_interval		= 64,					\
	.max_interval		= 64*num_online_cpus(),			\
	.busy_factor		= 128,					\
	.imbalance_pct		= 133,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 3,					\
	.idle_idx		= 3,					\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 0*SD_BALANCE_EXEC			\
				| 0*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 0*SD_WAKE_AFFINE			\
				| 0*SD_SHARE_CPUPOWER			\
				| 0*SD_POWERSAVINGS_BALANCE		\
				| 0*SD_SHARE_PKG_RESOURCES		\
				| 1*SD_SERIALIZE			\
				| 0*SD_PREFER_SIBLING			\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 64,					\
}

#ifndef SD_NODES_PER_DOMAIN
#define SD_NODES_PER_DOMAIN 16
#endif

#ifdef CONFIG_SCHED_BOOK
#ifndef SD_BOOK_INIT
#error Please define an appropriate SD_BOOK_INIT in include/asm/topology.h!!!
#endif
#endif /* CONFIG_SCHED_BOOK */

#ifdef CONFIG_NUMA
#ifndef SD_NODE_INIT
#error Please define an appropriate SD_NODE_INIT in include/asm/topology.h!!!
#endif

#endif /* CONFIG_NUMA */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_USE_PERCPU_NUMA_NODE_ID
DECLARE_PER_CPU(int, numa_node);

#ifndef numa_node_id
/* Returns the number of the current Node. */
static inline int numa_node_id(void)
{
<<<<<<< HEAD
	return __this_cpu_read(numa_node);
=======
	return raw_cpu_read(numa_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifndef cpu_to_node
static inline int cpu_to_node(int cpu)
{
	return per_cpu(numa_node, cpu);
}
#endif

#ifndef set_numa_node
static inline void set_numa_node(int node)
{
<<<<<<< HEAD
	percpu_write(numa_node, node);
=======
	this_cpu_write(numa_node, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifndef set_cpu_numa_node
static inline void set_cpu_numa_node(int cpu, int node)
{
	per_cpu(numa_node, cpu) = node;
}
#endif

#else	/* !CONFIG_USE_PERCPU_NUMA_NODE_ID */

/* Returns the number of the current Node. */
#ifndef numa_node_id
static inline int numa_node_id(void)
{
	return cpu_to_node(raw_smp_processor_id());
}
#endif

#endif	/* [!]CONFIG_USE_PERCPU_NUMA_NODE_ID */

#ifdef CONFIG_HAVE_MEMORYLESS_NODES

/*
 * N.B., Do NOT reference the '_numa_mem_' per cpu variable directly.
 * It will not be defined when CONFIG_HAVE_MEMORYLESS_NODES is not defined.
 * Use the accessor functions set_numa_mem(), numa_mem_id() and cpu_to_mem().
 */
DECLARE_PER_CPU(int, _numa_mem_);

#ifndef set_numa_mem
static inline void set_numa_mem(int node)
{
<<<<<<< HEAD
	percpu_write(_numa_mem_, node);
=======
	this_cpu_write(_numa_mem_, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifndef numa_mem_id
/* Returns the number of the nearest Node with memory */
static inline int numa_mem_id(void)
{
<<<<<<< HEAD
	return __this_cpu_read(_numa_mem_);
=======
	return raw_cpu_read(_numa_mem_);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifndef cpu_to_mem
static inline int cpu_to_mem(int cpu)
{
	return per_cpu(_numa_mem_, cpu);
}
#endif

#ifndef set_cpu_numa_mem
static inline void set_cpu_numa_mem(int cpu, int node)
{
	per_cpu(_numa_mem_, cpu) = node;
}
#endif

#else	/* !CONFIG_HAVE_MEMORYLESS_NODES */

#ifndef numa_mem_id
/* Returns the number of the nearest Node with memory */
static inline int numa_mem_id(void)
{
	return numa_node_id();
}
#endif

#ifndef cpu_to_mem
static inline int cpu_to_mem(int cpu)
{
	return cpu_to_node(cpu);
}
#endif

#endif	/* [!]CONFIG_HAVE_MEMORYLESS_NODES */

<<<<<<< HEAD
#ifndef topology_physical_package_id
#define topology_physical_package_id(cpu)	((void)(cpu), -1)
#endif
#ifndef topology_core_id
#define topology_core_id(cpu)			((void)(cpu), 0)
#endif
#ifndef topology_thread_cpumask
#define topology_thread_cpumask(cpu)		cpumask_of(cpu)
=======
#if defined(topology_die_id) && defined(topology_die_cpumask)
#define TOPOLOGY_DIE_SYSFS
#endif
#if defined(topology_cluster_id) && defined(topology_cluster_cpumask)
#define TOPOLOGY_CLUSTER_SYSFS
#endif
#if defined(topology_book_id) && defined(topology_book_cpumask)
#define TOPOLOGY_BOOK_SYSFS
#endif
#if defined(topology_drawer_id) && defined(topology_drawer_cpumask)
#define TOPOLOGY_DRAWER_SYSFS
#endif

#ifndef topology_physical_package_id
#define topology_physical_package_id(cpu)	((void)(cpu), -1)
#endif
#ifndef topology_die_id
#define topology_die_id(cpu)			((void)(cpu), -1)
#endif
#ifndef topology_cluster_id
#define topology_cluster_id(cpu)		((void)(cpu), -1)
#endif
#ifndef topology_core_id
#define topology_core_id(cpu)			((void)(cpu), 0)
#endif
#ifndef topology_book_id
#define topology_book_id(cpu)			((void)(cpu), -1)
#endif
#ifndef topology_drawer_id
#define topology_drawer_id(cpu)			((void)(cpu), -1)
#endif
#ifndef topology_ppin
#define topology_ppin(cpu)			((void)(cpu), 0ull)
#endif
#ifndef topology_sibling_cpumask
#define topology_sibling_cpumask(cpu)		cpumask_of(cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifndef topology_core_cpumask
#define topology_core_cpumask(cpu)		cpumask_of(cpu)
#endif
<<<<<<< HEAD
=======
#ifndef topology_cluster_cpumask
#define topology_cluster_cpumask(cpu)		cpumask_of(cpu)
#endif
#ifndef topology_die_cpumask
#define topology_die_cpumask(cpu)		cpumask_of(cpu)
#endif
#ifndef topology_book_cpumask
#define topology_book_cpumask(cpu)		cpumask_of(cpu)
#endif
#ifndef topology_drawer_cpumask
#define topology_drawer_cpumask(cpu)		cpumask_of(cpu)
#endif

#if defined(CONFIG_SCHED_SMT) && !defined(cpu_smt_mask)
static inline const struct cpumask *cpu_smt_mask(int cpu)
{
	return topology_sibling_cpumask(cpu);
}
#endif

static inline const struct cpumask *cpu_cpu_mask(int cpu)
{
	return cpumask_of_node(cpu_to_node(cpu));
}

#ifdef CONFIG_NUMA
int sched_numa_find_nth_cpu(const struct cpumask *cpus, int cpu, int node);
extern const struct cpumask *sched_numa_hop_mask(unsigned int node, unsigned int hops);
#else
static __always_inline int sched_numa_find_nth_cpu(const struct cpumask *cpus, int cpu, int node)
{
	return cpumask_nth_and(cpu, cpus, cpu_online_mask);
}

static inline const struct cpumask *
sched_numa_hop_mask(unsigned int node, unsigned int hops)
{
	return ERR_PTR(-EOPNOTSUPP);
}
#endif	/* CONFIG_NUMA */

/**
 * for_each_numa_hop_mask - iterate over cpumasks of increasing NUMA distance
 *                          from a given node.
 * @mask: the iteration variable.
 * @node: the NUMA node to start the search from.
 *
 * Requires rcu_lock to be held.
 *
 * Yields cpu_online_mask for @node == NUMA_NO_NODE.
 */
#define for_each_numa_hop_mask(mask, node)				       \
	for (unsigned int __hops = 0;					       \
	     mask = (node != NUMA_NO_NODE || __hops) ?			       \
		     sched_numa_hop_mask(node, __hops) :		       \
		     cpu_online_mask,					       \
	     !IS_ERR_OR_NULL(mask);					       \
	     __hops++)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_TOPOLOGY_H */
