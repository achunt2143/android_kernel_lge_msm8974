<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_S390_TOPOLOGY_H
#define _ASM_S390_TOPOLOGY_H

#include <linux/cpumask.h>
<<<<<<< HEAD
#include <asm/sysinfo.h>

struct cpu;

#ifdef CONFIG_SCHED_BOOK

extern unsigned char cpu_core_id[NR_CPUS];
extern cpumask_t cpu_core_map[NR_CPUS];

static inline const struct cpumask *cpu_coregroup_mask(int cpu)
{
	return &cpu_core_map[cpu];
}

#define topology_core_id(cpu)		(cpu_core_id[cpu])
#define topology_core_cpumask(cpu)	(&cpu_core_map[cpu])
#define mc_capable()			(1)

extern unsigned char cpu_book_id[NR_CPUS];
extern cpumask_t cpu_book_map[NR_CPUS];

static inline const struct cpumask *cpu_book_mask(int cpu)
{
	return &cpu_book_map[cpu];
}

#define topology_book_id(cpu)		(cpu_book_id[cpu])
#define topology_book_cpumask(cpu)	(&cpu_book_map[cpu])

=======
#include <asm/numa.h>

struct sysinfo_15_1_x;
struct cpu;

#ifdef CONFIG_SCHED_TOPOLOGY

struct cpu_topology_s390 {
	unsigned short thread_id;
	unsigned short core_id;
	unsigned short socket_id;
	unsigned short book_id;
	unsigned short drawer_id;
	unsigned short dedicated : 1;
	int booted_cores;
	cpumask_t thread_mask;
	cpumask_t core_mask;
	cpumask_t book_mask;
	cpumask_t drawer_mask;
};

extern struct cpu_topology_s390 cpu_topology[NR_CPUS];

#define topology_physical_package_id(cpu) (cpu_topology[cpu].socket_id)
#define topology_thread_id(cpu)		  (cpu_topology[cpu].thread_id)
#define topology_sibling_cpumask(cpu)	  (&cpu_topology[cpu].thread_mask)
#define topology_core_id(cpu)		  (cpu_topology[cpu].core_id)
#define topology_core_cpumask(cpu)	  (&cpu_topology[cpu].core_mask)
#define topology_book_id(cpu)		  (cpu_topology[cpu].book_id)
#define topology_book_cpumask(cpu)	  (&cpu_topology[cpu].book_mask)
#define topology_drawer_id(cpu)		  (cpu_topology[cpu].drawer_id)
#define topology_drawer_cpumask(cpu)	  (&cpu_topology[cpu].drawer_mask)
#define topology_cpu_dedicated(cpu)	  (cpu_topology[cpu].dedicated)
#define topology_booted_cores(cpu)	  (cpu_topology[cpu].booted_cores)

#define mc_capable() 1

void topology_init_early(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int topology_cpu_init(struct cpu *);
int topology_set_cpu_management(int fc);
void topology_schedule_update(void);
void store_topology(struct sysinfo_15_1_x *info);
<<<<<<< HEAD
void topology_expect_change(void);

#else /* CONFIG_SCHED_BOOK */

static inline void topology_schedule_update(void) { }
static inline int topology_cpu_init(struct cpu *cpu) { return 0; }
static inline void topology_expect_change(void) { }

#endif /* CONFIG_SCHED_BOOK */
=======
void update_cpu_masks(void);
void topology_expect_change(void);
const struct cpumask *cpu_coregroup_mask(int cpu);

#else /* CONFIG_SCHED_TOPOLOGY */

static inline void topology_init_early(void) { }
static inline void topology_schedule_update(void) { }
static inline int topology_cpu_init(struct cpu *cpu) { return 0; }
static inline int topology_cpu_dedicated(int cpu_nr) { return 0; }
static inline int topology_booted_cores(int cpu_nr) { return 1; }
static inline void update_cpu_masks(void) { }
static inline void topology_expect_change(void) { }

#endif /* CONFIG_SCHED_TOPOLOGY */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define POLARIZATION_UNKNOWN	(-1)
#define POLARIZATION_HRZ	(0)
#define POLARIZATION_VL		(1)
#define POLARIZATION_VM		(2)
#define POLARIZATION_VH		(3)

<<<<<<< HEAD
extern int cpu_polarization[];

static inline void cpu_set_polarization(int cpu, int val)
{
#ifdef CONFIG_SCHED_BOOK
	cpu_polarization[cpu] = val;
#endif
}

static inline int cpu_read_polarization(int cpu)
{
#ifdef CONFIG_SCHED_BOOK
	return cpu_polarization[cpu];
#else
	return POLARIZATION_HRZ;
#endif
}

#ifdef CONFIG_SCHED_BOOK
void s390_init_cpu_topology(void);
#else
static inline void s390_init_cpu_topology(void)
{
};
#endif

#define SD_BOOK_INIT	SD_CPU_INIT

=======
#define SD_BOOK_INIT	SD_CPU_INIT

#ifdef CONFIG_NUMA

#define cpu_to_node cpu_to_node
static inline int cpu_to_node(int cpu)
{
	return 0;
}

/* Returns a pointer to the cpumask of CPUs on node 'node'. */
#define cpumask_of_node cpumask_of_node
static inline const struct cpumask *cpumask_of_node(int node)
{
	return cpu_possible_mask;
}

#define pcibus_to_node(bus) __pcibus_to_node(bus)

#else /* !CONFIG_NUMA */

#define numa_node_id numa_node_id
static inline int numa_node_id(void)
{
	return 0;
}

#endif /* CONFIG_NUMA */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/topology.h>

#endif /* _ASM_S390_TOPOLOGY_H */
