/*
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
#ifndef _ASM_X86_TOPOLOGY_H
#define _ASM_X86_TOPOLOGY_H

<<<<<<< HEAD
#ifdef CONFIG_X86_32
# ifdef CONFIG_X86_HT
#  define ENABLE_TOPO_DEFINES
# endif
#else
# ifdef CONFIG_SMP
#  define ENABLE_TOPO_DEFINES
# endif
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * to preserve the visibility of NUMA_NO_NODE definition,
 * moved to there from here.  May be used independent of
 * CONFIG_NUMA.
 */
#include <linux/numa.h>
<<<<<<< HEAD

#ifdef CONFIG_NUMA
#include <linux/cpumask.h>

#include <asm/mpspec.h>
=======
#include <linux/cpumask.h>

#ifdef CONFIG_NUMA

#include <asm/mpspec.h>
#include <asm/percpu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Mappings between logical cpu number and node number */
DECLARE_EARLY_PER_CPU(int, x86_cpu_to_node_map);

#ifdef CONFIG_DEBUG_PER_CPU_MAPS
/*
 * override generic percpu implementation of cpu_to_node
 */
extern int __cpu_to_node(int cpu);
#define cpu_to_node __cpu_to_node

extern int early_cpu_to_node(int cpu);

#else	/* !CONFIG_DEBUG_PER_CPU_MAPS */

/* Same function but used if called before per_cpu areas are setup */
static inline int early_cpu_to_node(int cpu)
{
	return early_per_cpu(x86_cpu_to_node_map, cpu);
}

#endif /* !CONFIG_DEBUG_PER_CPU_MAPS */

/* Mappings between node number and cpus on that node. */
extern cpumask_var_t node_to_cpumask_map[MAX_NUMNODES];

#ifdef CONFIG_DEBUG_PER_CPU_MAPS
extern const struct cpumask *cpumask_of_node(int node);
#else
/* Returns a pointer to the cpumask of CPUs on Node 'node'. */
static inline const struct cpumask *cpumask_of_node(int node)
{
	return node_to_cpumask_map[node];
}
#endif

extern void setup_node_to_cpumask_map(void);

<<<<<<< HEAD
/*
 * Returns the number of the node containing Node 'node'. This
 * architecture is flat, so it is a pretty simple function!
 */
#define parent_node(node) (node)

#define pcibus_to_node(bus) __pcibus_to_node(bus)

#ifdef CONFIG_X86_32
# define SD_CACHE_NICE_TRIES	1
# define SD_IDLE_IDX		1
#else
# define SD_CACHE_NICE_TRIES	2
# define SD_IDLE_IDX		2
#endif

/* sched_domains SD_NODE_INIT for NUMA machines */
#define SD_NODE_INIT (struct sched_domain) {				\
	.min_interval		= 8,					\
	.max_interval		= 32,					\
	.busy_factor		= 32,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= SD_CACHE_NICE_TRIES,			\
	.busy_idx		= 3,					\
	.idle_idx		= SD_IDLE_IDX,				\
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
				| 0*SD_POWERSAVINGS_BALANCE		\
				| 0*SD_SHARE_PKG_RESOURCES		\
				| 1*SD_SERIALIZE			\
				| 0*SD_PREFER_SIBLING			\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 1,					\
}

=======
#define pcibus_to_node(bus) __pcibus_to_node(bus)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int __node_distance(int, int);
#define node_distance(a, b) __node_distance(a, b)

#else /* !CONFIG_NUMA */

static inline int numa_node_id(void)
{
	return 0;
}
/*
 * indicate override:
 */
#define numa_node_id numa_node_id

static inline int early_cpu_to_node(int cpu)
{
	return 0;
}

static inline void setup_node_to_cpumask_map(void) { }

#endif

#include <asm-generic/topology.h>

<<<<<<< HEAD
extern const struct cpumask *cpu_coregroup_mask(int cpu);

#define topology_physical_package_id(cpu)	(cpu_data(cpu).phys_proc_id)
#define topology_core_id(cpu)			(cpu_data(cpu).cpu_core_id)

#ifdef ENABLE_TOPO_DEFINES
#define topology_core_cpumask(cpu)		(per_cpu(cpu_core_map, cpu))
#define topology_thread_cpumask(cpu)		(per_cpu(cpu_sibling_map, cpu))

/* indicates that pointers to the topology cpumask_t maps are valid */
#define arch_provides_topology_pointers		yes
#endif

=======
/* Topology information */
enum x86_topology_domains {
	TOPO_SMT_DOMAIN,
	TOPO_CORE_DOMAIN,
	TOPO_MODULE_DOMAIN,
	TOPO_TILE_DOMAIN,
	TOPO_DIE_DOMAIN,
	TOPO_DIEGRP_DOMAIN,
	TOPO_PKG_DOMAIN,
	TOPO_MAX_DOMAIN,
};

struct x86_topology_system {
	unsigned int	dom_shifts[TOPO_MAX_DOMAIN];
	unsigned int	dom_size[TOPO_MAX_DOMAIN];
};

extern struct x86_topology_system x86_topo_system;

static inline unsigned int topology_get_domain_size(enum x86_topology_domains dom)
{
	return x86_topo_system.dom_size[dom];
}

static inline unsigned int topology_get_domain_shift(enum x86_topology_domains dom)
{
	return dom == TOPO_SMT_DOMAIN ? 0 : x86_topo_system.dom_shifts[dom - 1];
}

extern const struct cpumask *cpu_coregroup_mask(int cpu);
extern const struct cpumask *cpu_clustergroup_mask(int cpu);

#define topology_logical_package_id(cpu)	(cpu_data(cpu).topo.logical_pkg_id)
#define topology_physical_package_id(cpu)	(cpu_data(cpu).topo.pkg_id)
#define topology_logical_die_id(cpu)		(cpu_data(cpu).topo.logical_die_id)
#define topology_die_id(cpu)			(cpu_data(cpu).topo.die_id)
#define topology_core_id(cpu)			(cpu_data(cpu).topo.core_id)
#define topology_ppin(cpu)			(cpu_data(cpu).ppin)

#define topology_amd_node_id(cpu)		(cpu_data(cpu).topo.amd_node_id)

extern unsigned int __max_dies_per_package;
extern unsigned int __max_logical_packages;
extern unsigned int __max_threads_per_core;
extern unsigned int __num_threads_per_package;
extern unsigned int __num_cores_per_package;

static inline unsigned int topology_max_packages(void)
{
	return __max_logical_packages;
}

static inline unsigned int topology_max_dies_per_package(void)
{
	return __max_dies_per_package;
}

static inline unsigned int topology_num_cores_per_package(void)
{
	return __num_cores_per_package;
}

static inline unsigned int topology_num_threads_per_package(void)
{
	return __num_threads_per_package;
}

#ifdef CONFIG_X86_LOCAL_APIC
int topology_get_logical_id(u32 apicid, enum x86_topology_domains at_level);
#else
static inline int topology_get_logical_id(u32 apicid, enum x86_topology_domains at_level)
{
	return 0;
}
#endif

#ifdef CONFIG_SMP
#define topology_cluster_id(cpu)		(cpu_data(cpu).topo.l2c_id)
#define topology_die_cpumask(cpu)		(per_cpu(cpu_die_map, cpu))
#define topology_cluster_cpumask(cpu)		(cpu_clustergroup_mask(cpu))
#define topology_core_cpumask(cpu)		(per_cpu(cpu_core_map, cpu))
#define topology_sibling_cpumask(cpu)		(per_cpu(cpu_sibling_map, cpu))


static inline int topology_phys_to_logical_pkg(unsigned int pkg)
{
	return topology_get_logical_id(pkg << x86_topo_system.dom_shifts[TOPO_PKG_DOMAIN],
				       TOPO_PKG_DOMAIN);
}

extern int __max_smt_threads;

static inline int topology_max_smt_threads(void)
{
	return __max_smt_threads;
}

#include <linux/cpu_smt.h>

extern unsigned int __amd_nodes_per_pkg;

static inline unsigned int topology_amd_nodes_per_pkg(void)
{
	return __amd_nodes_per_pkg;
}

extern struct cpumask __cpu_primary_thread_mask;
#define cpu_primary_thread_mask ((const struct cpumask *)&__cpu_primary_thread_mask)

/**
 * topology_is_primary_thread - Check whether CPU is the primary SMT thread
 * @cpu:	CPU to check
 */
static inline bool topology_is_primary_thread(unsigned int cpu)
{
	return cpumask_test_cpu(cpu, cpu_primary_thread_mask);
}

#else /* CONFIG_SMP */
static inline int topology_phys_to_logical_pkg(unsigned int pkg) { return 0; }
static inline int topology_max_smt_threads(void) { return 1; }
static inline bool topology_is_primary_thread(unsigned int cpu) { return true; }
static inline unsigned int topology_amd_nodes_per_pkg(void) { return 1; }
#endif /* !CONFIG_SMP */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void arch_fix_phys_package_id(int num, u32 slot)
{
}

struct pci_bus;
<<<<<<< HEAD
void x86_pci_root_bus_resources(int bus, struct list_head *resources);

#ifdef CONFIG_SMP
#define mc_capable()	((boot_cpu_data.x86_max_cores > 1) && \
			(cpumask_weight(cpu_core_mask(0)) != nr_cpu_ids))
#define smt_capable()			(smp_num_siblings > 1)
#endif

#ifdef CONFIG_NUMA
extern int get_mp_bus_to_node(int busnum);
extern void set_mp_bus_to_node(int busnum, int node);
#else
static inline int get_mp_bus_to_node(int busnum)
{
	return 0;
}
static inline void set_mp_bus_to_node(int busnum, int node)
{
}
=======
int x86_pci_root_bus_node(int bus);
void x86_pci_root_bus_resources(int bus, struct list_head *resources);

extern bool x86_topology_update;

#ifdef CONFIG_SCHED_MC_PRIO
#include <asm/percpu.h>

DECLARE_PER_CPU_READ_MOSTLY(int, sched_core_priority);
extern unsigned int __read_mostly sysctl_sched_itmt_enabled;

/* Interface to set priority of a cpu */
void sched_set_itmt_core_prio(int prio, int core_cpu);

/* Interface to notify scheduler that system supports ITMT */
int sched_set_itmt_support(void);

/* Interface to notify scheduler that system revokes ITMT support */
void sched_clear_itmt_support(void);

#else /* CONFIG_SCHED_MC_PRIO */

#define sysctl_sched_itmt_enabled	0
static inline void sched_set_itmt_core_prio(int prio, int core_cpu)
{
}
static inline int sched_set_itmt_support(void)
{
	return 0;
}
static inline void sched_clear_itmt_support(void)
{
}
#endif /* CONFIG_SCHED_MC_PRIO */

#if defined(CONFIG_SMP) && defined(CONFIG_X86_64)
#include <asm/cpufeature.h>

DECLARE_STATIC_KEY_FALSE(arch_scale_freq_key);

#define arch_scale_freq_invariant() static_branch_likely(&arch_scale_freq_key)

DECLARE_PER_CPU(unsigned long, arch_freq_scale);

static inline long arch_scale_freq_capacity(int cpu)
{
	return per_cpu(arch_freq_scale, cpu);
}
#define arch_scale_freq_capacity arch_scale_freq_capacity

extern void arch_set_max_freq_ratio(bool turbo_disabled);
extern void freq_invariance_set_perf_ratio(u64 ratio, bool turbo_disabled);
#else
static inline void arch_set_max_freq_ratio(bool turbo_disabled) { }
static inline void freq_invariance_set_perf_ratio(u64 ratio, bool turbo_disabled) { }
#endif

extern void arch_scale_freq_tick(void);
#define arch_scale_freq_tick arch_scale_freq_tick

#ifdef CONFIG_ACPI_CPPC_LIB
void init_freq_invariance_cppc(void);
#define arch_init_invariance_cppc init_freq_invariance_cppc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* _ASM_X86_TOPOLOGY_H */
