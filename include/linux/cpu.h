<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * include/linux/cpu.h - generic cpu definition
 *
 * This is mainly for topological representation. We define the 
 * basic 'struct cpu' here, which can be embedded in per-arch 
 * definitions of processors.
 *
 * Basic handling of the devices is done in drivers/base/cpu.c
<<<<<<< HEAD
 * and system devices are handled in drivers/base/sys.c. 
 *
 * CPUs are exported via sysfs in the class/cpu/devices/
=======
 *
 * CPUs are exported via sysfs in the devices/system/cpu
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * directory. 
 */
#ifndef _LINUX_CPU_H_
#define _LINUX_CPU_H_

#include <linux/node.h>
#include <linux/compiler.h>
#include <linux/cpumask.h>
<<<<<<< HEAD

struct device;
=======
#include <linux/cpuhotplug.h>
#include <linux/cpu_smt.h>

struct device;
struct device_node;
struct attribute_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct cpu {
	int node_id;		/* The node which contains the CPU */
	int hotpluggable;	/* creates sysfs control file if hotpluggable */
	struct device dev;
};

<<<<<<< HEAD
extern int register_cpu(struct cpu *cpu, int num);
extern struct device *get_cpu_device(unsigned cpu);
extern bool cpu_is_hotpluggable(unsigned cpu);
=======
extern void boot_cpu_init(void);
extern void boot_cpu_hotplug_init(void);
extern void cpu_init(void);
extern void trap_init(void);

extern int register_cpu(struct cpu *cpu, int num);
extern struct device *get_cpu_device(unsigned cpu);
extern bool cpu_is_hotpluggable(unsigned cpu);
extern bool arch_match_cpu_phys_id(int cpu, u64 phys_id);
extern bool arch_find_n_match_cpu_physical_id(struct device_node *cpun,
					      int cpu, unsigned int *thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int cpu_add_dev_attr(struct device_attribute *attr);
extern void cpu_remove_dev_attr(struct device_attribute *attr);

extern int cpu_add_dev_attr_group(struct attribute_group *attrs);
extern void cpu_remove_dev_attr_group(struct attribute_group *attrs);

<<<<<<< HEAD
extern int sched_create_sysfs_power_savings_entries(struct device *dev);

=======
extern ssize_t cpu_show_meltdown(struct device *dev,
				 struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_spectre_v1(struct device *dev,
				   struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_spectre_v2(struct device *dev,
				   struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_spec_store_bypass(struct device *dev,
					  struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_l1tf(struct device *dev,
			     struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_mds(struct device *dev,
			    struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_tsx_async_abort(struct device *dev,
					struct device_attribute *attr,
					char *buf);
extern ssize_t cpu_show_itlb_multihit(struct device *dev,
				      struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_srbds(struct device *dev, struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_mmio_stale_data(struct device *dev,
					struct device_attribute *attr,
					char *buf);
extern ssize_t cpu_show_retbleed(struct device *dev,
				 struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_spec_rstack_overflow(struct device *dev,
					     struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_gds(struct device *dev,
			    struct device_attribute *attr, char *buf);
extern ssize_t cpu_show_reg_file_data_sampling(struct device *dev,
					       struct device_attribute *attr, char *buf);

extern __printf(4, 5)
struct device *cpu_device_create(struct device *parent, void *drvdata,
				 const struct attribute_group **groups,
				 const char *fmt, ...);
extern bool arch_cpu_is_hotpluggable(int cpu);
extern int arch_register_cpu(int cpu);
extern void arch_unregister_cpu(int cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_HOTPLUG_CPU
extern void unregister_cpu(struct cpu *cpu);
extern ssize_t arch_cpu_probe(const char *, size_t);
extern ssize_t arch_cpu_release(const char *, size_t);
#endif
<<<<<<< HEAD
struct notifier_block;

#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
extern int arch_cpu_uevent(struct device *dev, struct kobj_uevent_env *env);
extern ssize_t arch_print_cpu_modalias(struct device *dev,
				       struct device_attribute *attr,
				       char *bufptr);
#endif

/*
 * CPU notifier priorities.
 */
enum {
	/*
	 * SCHED_ACTIVE marks a cpu which is coming up active during
	 * CPU_ONLINE and CPU_DOWN_FAILED and must be the first
	 * notifier.  CPUSET_ACTIVE adjusts cpuset according to
	 * cpu_active mask right after SCHED_ACTIVE.  During
	 * CPU_DOWN_PREPARE, SCHED_INACTIVE and CPUSET_INACTIVE are
	 * ordered in the similar way.
	 *
	 * This ordering guarantees consistent cpu_active mask and
	 * migration behavior to all cpu notifiers.
	 */
	CPU_PRI_SCHED_ACTIVE	= INT_MAX,
	CPU_PRI_CPUSET_ACTIVE	= INT_MAX - 1,
	CPU_PRI_SCHED_INACTIVE	= INT_MIN + 1,
	CPU_PRI_CPUSET_INACTIVE	= INT_MIN,

	/* migration should happen before other stuff but after perf */
	CPU_PRI_PERF		= 20,
	CPU_PRI_MIGRATION	= 10,
	/* bring up workqueues before normal notifiers and down after */
	CPU_PRI_WORKQUEUE_UP	= 5,
	CPU_PRI_WORKQUEUE_DOWN	= -5,
};

#define CPU_ONLINE		0x0002 /* CPU (unsigned)v is up */
#define CPU_UP_PREPARE		0x0003 /* CPU (unsigned)v coming up */
#define CPU_UP_CANCELED		0x0004 /* CPU (unsigned)v NOT coming up */
#define CPU_DOWN_PREPARE	0x0005 /* CPU (unsigned)v going down */
#define CPU_DOWN_FAILED		0x0006 /* CPU (unsigned)v NOT going down */
#define CPU_DEAD		0x0007 /* CPU (unsigned)v dead */
#define CPU_DYING		0x0008 /* CPU (unsigned)v not running any task,
					* not handling interrupts, soon dead.
					* Called on the dying cpu, interrupts
					* are already disabled. Must not
					* sleep, must not fail */
#define CPU_POST_DEAD		0x0009 /* CPU (unsigned)v dead, cpu_hotplug
					* lock is dropped */
#define CPU_STARTING		0x000A /* CPU (unsigned)v soon running.
					* Called on the new cpu, just before
					* enabling interrupts. Must not sleep,
					* must not fail */

/* Used for CPU hotplug events occurring while tasks are frozen due to a suspend
 * operation in progress
 */
#define CPU_TASKS_FROZEN	0x0010

#define CPU_ONLINE_FROZEN	(CPU_ONLINE | CPU_TASKS_FROZEN)
#define CPU_UP_PREPARE_FROZEN	(CPU_UP_PREPARE | CPU_TASKS_FROZEN)
#define CPU_UP_CANCELED_FROZEN	(CPU_UP_CANCELED | CPU_TASKS_FROZEN)
#define CPU_DOWN_PREPARE_FROZEN	(CPU_DOWN_PREPARE | CPU_TASKS_FROZEN)
#define CPU_DOWN_FAILED_FROZEN	(CPU_DOWN_FAILED | CPU_TASKS_FROZEN)
#define CPU_DEAD_FROZEN		(CPU_DEAD | CPU_TASKS_FROZEN)
#define CPU_DYING_FROZEN	(CPU_DYING | CPU_TASKS_FROZEN)
#define CPU_STARTING_FROZEN	(CPU_STARTING | CPU_TASKS_FROZEN)


#ifdef CONFIG_SMP
/* Need to know about CPUs going up/down? */
#if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE)
#define cpu_notifier(fn, pri) {					\
	static struct notifier_block fn##_nb __cpuinitdata =	\
		{ .notifier_call = fn, .priority = pri };	\
	register_cpu_notifier(&fn##_nb);			\
}
#else /* #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
#endif /* #else #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#ifdef CONFIG_HOTPLUG_CPU
extern int register_cpu_notifier(struct notifier_block *nb);
extern void unregister_cpu_notifier(struct notifier_block *nb);
#else

#ifndef MODULE
extern int register_cpu_notifier(struct notifier_block *nb);
#else
static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}
#endif

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}
#endif

int cpu_up(unsigned int cpu);
void notify_cpu_starting(unsigned int cpu);
extern void cpu_maps_update_begin(void);
extern void cpu_maps_update_done(void);

#else	/* CONFIG_SMP */

#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)

static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}
=======

#ifdef CONFIG_GENERIC_CPU_DEVICES
DECLARE_PER_CPU(struct cpu, cpu_devices);
#endif

/*
 * These states are not related to the core CPU hotplug mechanism. They are
 * used by various (sub)architectures to track internal state
 */
#define CPU_ONLINE		0x0002 /* CPU is up */
#define CPU_UP_PREPARE		0x0003 /* CPU coming up */
#define CPU_DEAD		0x0007 /* CPU dead */
#define CPU_DEAD_FROZEN		0x0008 /* CPU timed out on unplug */
#define CPU_POST_DEAD		0x0009 /* CPU successfully unplugged */
#define CPU_BROKEN		0x000B /* CPU did not die properly */

#ifdef CONFIG_SMP
extern bool cpuhp_tasks_frozen;
int add_cpu(unsigned int cpu);
int cpu_device_up(struct device *dev);
void notify_cpu_starting(unsigned int cpu);
extern void cpu_maps_update_begin(void);
extern void cpu_maps_update_done(void);
int bringup_hibernate_cpu(unsigned int sleep_cpu);
void bringup_nonboot_cpus(unsigned int max_cpus);

#else	/* CONFIG_SMP */
#define cpuhp_tasks_frozen	0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void cpu_maps_update_begin(void)
{
}

static inline void cpu_maps_update_done(void)
{
}

<<<<<<< HEAD
#endif /* CONFIG_SMP */
extern struct bus_type cpu_subsys;

#ifdef CONFIG_HOTPLUG_CPU
/* Stop CPUs going up and down. */

extern void get_online_cpus(void);
extern void put_online_cpus(void);
extern void cpu_hotplug_disable(void);
extern void cpu_hotplug_enable(void);
#define hotcpu_notifier(fn, pri)	cpu_notifier(fn, pri)
#define register_hotcpu_notifier(nb)	register_cpu_notifier(nb)
#define unregister_hotcpu_notifier(nb)	unregister_cpu_notifier(nb)
int cpu_down(unsigned int cpu);

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
extern void cpu_hotplug_driver_lock(void);
extern void cpu_hotplug_driver_unlock(void);
#else
static inline void cpu_hotplug_driver_lock(void)
{
}

static inline void cpu_hotplug_driver_unlock(void)
{
}
#endif

#else		/* CONFIG_HOTPLUG_CPU */

#define get_online_cpus()	do { } while (0)
#define put_online_cpus()	do { } while (0)
#define cpu_hotplug_disable()	do { } while (0)
#define cpu_hotplug_enable()	do { } while (0)
#define hotcpu_notifier(fn, pri)	do { (void)(fn); } while (0)
/* These aren't inline functions due to a GCC bug. */
#define register_hotcpu_notifier(nb)	({ (void)(nb); 0; })
#define unregister_hotcpu_notifier(nb)	({ (void)(nb); })
#endif		/* CONFIG_HOTPLUG_CPU */

#ifdef CONFIG_PM_SLEEP_SMP
extern int disable_nonboot_cpus(void);
extern void enable_nonboot_cpus(void);
#else /* !CONFIG_PM_SLEEP_SMP */
static inline int disable_nonboot_cpus(void) { return 0; }
static inline void enable_nonboot_cpus(void) {}
#endif /* !CONFIG_PM_SLEEP_SMP */

#define IDLE_START 1
#define IDLE_END 2

void idle_notifier_register(struct notifier_block *n);
void idle_notifier_unregister(struct notifier_block *n);
void idle_notifier_call_chain(unsigned long val);

=======
static inline int add_cpu(unsigned int cpu) { return 0;}

#endif /* CONFIG_SMP */
extern const struct bus_type cpu_subsys;

extern int lockdep_is_cpus_held(void);

#ifdef CONFIG_HOTPLUG_CPU
extern void cpus_write_lock(void);
extern void cpus_write_unlock(void);
extern void cpus_read_lock(void);
extern void cpus_read_unlock(void);
extern int  cpus_read_trylock(void);
extern void lockdep_assert_cpus_held(void);
extern void cpu_hotplug_disable(void);
extern void cpu_hotplug_enable(void);
void clear_tasks_mm_cpumask(int cpu);
int remove_cpu(unsigned int cpu);
int cpu_device_down(struct device *dev);
extern void smp_shutdown_nonboot_cpus(unsigned int primary_cpu);

#else /* CONFIG_HOTPLUG_CPU */

static inline void cpus_write_lock(void) { }
static inline void cpus_write_unlock(void) { }
static inline void cpus_read_lock(void) { }
static inline void cpus_read_unlock(void) { }
static inline int  cpus_read_trylock(void) { return true; }
static inline void lockdep_assert_cpus_held(void) { }
static inline void cpu_hotplug_disable(void) { }
static inline void cpu_hotplug_enable(void) { }
static inline int remove_cpu(unsigned int cpu) { return -EPERM; }
static inline void smp_shutdown_nonboot_cpus(unsigned int primary_cpu) { }
#endif	/* !CONFIG_HOTPLUG_CPU */

DEFINE_LOCK_GUARD_0(cpus_read_lock, cpus_read_lock(), cpus_read_unlock())

#ifdef CONFIG_PM_SLEEP_SMP
extern int freeze_secondary_cpus(int primary);
extern void thaw_secondary_cpus(void);

static inline int suspend_disable_secondary_cpus(void)
{
	int cpu = 0;

	if (IS_ENABLED(CONFIG_PM_SLEEP_SMP_NONZERO_CPU))
		cpu = -1;

	return freeze_secondary_cpus(cpu);
}
static inline void suspend_enable_secondary_cpus(void)
{
	return thaw_secondary_cpus();
}

#else /* !CONFIG_PM_SLEEP_SMP */
static inline void thaw_secondary_cpus(void) {}
static inline int suspend_disable_secondary_cpus(void) { return 0; }
static inline void suspend_enable_secondary_cpus(void) { }
#endif /* !CONFIG_PM_SLEEP_SMP */

void __noreturn cpu_startup_entry(enum cpuhp_state state);

void cpu_idle_poll_ctrl(bool enable);

bool cpu_in_idle(unsigned long pc);

void arch_cpu_idle(void);
void arch_cpu_idle_prepare(void);
void arch_cpu_idle_enter(void);
void arch_cpu_idle_exit(void);
void arch_tick_broadcast_enter(void);
void arch_tick_broadcast_exit(void);
void __noreturn arch_cpu_idle_dead(void);

#ifdef CONFIG_ARCH_HAS_CPU_FINALIZE_INIT
void arch_cpu_finalize_init(void);
#else
static inline void arch_cpu_finalize_init(void) { }
#endif

void play_idle_precise(u64 duration_ns, u64 latency_ns);

static inline void play_idle(unsigned long duration_us)
{
	play_idle_precise(duration_us * NSEC_PER_USEC, U64_MAX);
}

#ifdef CONFIG_HOTPLUG_CPU
void cpuhp_report_idle_dead(void);
#else
static inline void cpuhp_report_idle_dead(void) { }
#endif /* #ifdef CONFIG_HOTPLUG_CPU */

#ifdef CONFIG_CPU_MITIGATIONS
extern bool cpu_mitigations_off(void);
extern bool cpu_mitigations_auto_nosmt(void);
#else
static inline bool cpu_mitigations_off(void)
{
	return true;
}
static inline bool cpu_mitigations_auto_nosmt(void)
{
	return false;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_CPU_H_ */
