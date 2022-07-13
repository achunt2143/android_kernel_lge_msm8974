<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_TOPOLOGY_H
#define _ASM_POWERPC_TOPOLOGY_H
#ifdef __KERNEL__


struct device;
struct device_node;
<<<<<<< HEAD
=======
struct drmem_lmb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_NUMA

/*
<<<<<<< HEAD
 * Before going off node we want the VM to try and reclaim from the local
 * node. It does this if the remote distance is larger than RECLAIM_DISTANCE.
 * With the default REMOTE_DISTANCE of 20 and the default RECLAIM_DISTANCE of
 * 20, we never reclaim and go off node straight away.
 *
 * To fix this we choose a smaller value of RECLAIM_DISTANCE.
 */
#define RECLAIM_DISTANCE 10

/*
 * Avoid creating an extra level of balancing (SD_ALLNODES) on the largest
 * POWER7 boxes which have a maximum of 32 nodes.
 */
#define SD_NODES_PER_DOMAIN 32

#include <asm/mmzone.h>

static inline int cpu_to_node(int cpu)
{
	return numa_cpu_lookup_table[cpu];
}

#define parent_node(node)	(node)

=======
 * If zone_reclaim_mode is enabled, a RECLAIM_DISTANCE of 10 will mean that
 * all zones on all nodes will be eligible for zone_reclaim().
 */
#define RECLAIM_DISTANCE 10

#include <asm/mmzone.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define cpumask_of_node(node) ((node) == -1 ?				\
			       cpu_all_mask :				\
			       node_to_cpumask_map[node])

struct pci_bus;
#ifdef CONFIG_PCI
extern int pcibus_to_node(struct pci_bus *bus);
#else
static inline int pcibus_to_node(struct pci_bus *bus)
{
	return -1;
}
#endif

#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))

<<<<<<< HEAD
/* sched_domains SD_NODE_INIT for PPC64 machines */
#define SD_NODE_INIT (struct sched_domain) {				\
	.min_interval		= 8,					\
	.max_interval		= 32,					\
	.busy_factor		= 32,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 3,					\
	.idle_idx		= 1,					\
	.newidle_idx		= 0,					\
	.wake_idx		= 0,					\
	.forkexec_idx		= 0,					\
									\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 0*SD_BALANCE_NEWIDLE			\
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
int cpu_relative_distance(__be32 *cpu1_assoc, __be32 *cpu2_assoc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int __node_distance(int, int);
#define node_distance(a, b) __node_distance(a, b)

extern void __init dump_numa_cpu_topology(void);

extern int sysfs_add_device_to_node(struct device *dev, int nid);
extern void sysfs_remove_device_from_node(struct device *dev, int nid);

<<<<<<< HEAD
#else

=======
static inline void update_numa_cpu_lookup_table(unsigned int cpu, int node)
{
	numa_cpu_lookup_table[cpu] = node;
}

static inline int early_cpu_to_node(int cpu)
{
	int nid;

	nid = numa_cpu_lookup_table[cpu];

	/*
	 * Fall back to node 0 if nid is unset (it should be, except bugs).
	 * This allows callers to safely do NODE_DATA(early_cpu_to_node(cpu)).
	 */
	return (nid < 0) ? 0 : nid;
}

int of_drconf_to_nid_single(struct drmem_lmb *lmb);
void update_numa_distance(struct device_node *node);

extern void map_cpu_to_node(int cpu, int node);
#ifdef CONFIG_HOTPLUG_CPU
extern void unmap_cpu_from_node(unsigned long cpu);
#endif /* CONFIG_HOTPLUG_CPU */

#else

static inline int early_cpu_to_node(int cpu) { return 0; }

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void dump_numa_cpu_topology(void) {}

static inline int sysfs_add_device_to_node(struct device *dev, int nid)
{
	return 0;
}

static inline void sysfs_remove_device_from_node(struct device *dev,
						int nid)
{
}
<<<<<<< HEAD
#endif /* CONFIG_NUMA */

#if defined(CONFIG_NUMA) && defined(CONFIG_PPC_SPLPAR)
extern int start_topology_update(void);
extern int stop_topology_update(void);
#else
static inline int start_topology_update(void)
{
	return 0;
}
static inline int stop_topology_update(void)
{
	return 0;
}
=======

static inline void update_numa_cpu_lookup_table(unsigned int cpu, int node) {}

static inline int cpu_relative_distance(__be32 *cpu1_assoc, __be32 *cpu2_assoc)
{
	return 0;
}

static inline int of_drconf_to_nid_single(struct drmem_lmb *lmb)
{
	return first_online_node;
}

static inline void update_numa_distance(struct device_node *node) {}

#ifdef CONFIG_SMP
static inline void map_cpu_to_node(int cpu, int node) {}
#ifdef CONFIG_HOTPLUG_CPU
static inline void unmap_cpu_from_node(unsigned long cpu) {}
#endif /* CONFIG_HOTPLUG_CPU */
#endif /* CONFIG_SMP */

#endif /* CONFIG_NUMA */

#if defined(CONFIG_NUMA) && defined(CONFIG_PPC_SPLPAR)
void find_and_update_cpu_nid(int cpu);
extern int cpu_to_coregroup_id(int cpu);
#else
static inline void find_and_update_cpu_nid(int cpu) {}
static inline int cpu_to_coregroup_id(int cpu)
{
#ifdef CONFIG_SMP
	return cpu_to_core_id(cpu);
#else
	return 0;
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_NUMA && CONFIG_PPC_SPLPAR */

#include <asm-generic/topology.h>

#ifdef CONFIG_SMP
#include <asm/cputable.h>
<<<<<<< HEAD
#define smt_capable()		(cpu_has_feature(CPU_FTR_SMT))
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PPC64
#include <asm/smp.h>

<<<<<<< HEAD
#define topology_thread_cpumask(cpu)	(per_cpu(cpu_sibling_map, cpu))
#define topology_core_cpumask(cpu)	(per_cpu(cpu_core_map, cpu))
#define topology_core_id(cpu)		(cpu_to_core_id(cpu))
#endif
#endif

=======
#define topology_physical_package_id(cpu)	(cpu_to_chip_id(cpu))

#define topology_sibling_cpumask(cpu)	(per_cpu(cpu_sibling_map, cpu))
#define topology_core_cpumask(cpu)	(per_cpu(cpu_core_map, cpu))
#define topology_core_id(cpu)		(cpu_to_core_id(cpu))

#endif
#endif

#ifdef CONFIG_HOTPLUG_SMT
#include <linux/cpu_smt.h>
#include <asm/cputhreads.h>

static inline bool topology_is_primary_thread(unsigned int cpu)
{
	return cpu == cpu_first_thread_sibling(cpu);
}

static inline bool topology_smt_thread_allowed(unsigned int cpu)
{
	return cpu_thread_in_core(cpu) < cpu_smt_num_threads;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_TOPOLOGY_H */
