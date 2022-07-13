<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_APIC_H
#define _ASM_X86_APIC_H

#include <linux/cpumask.h>
<<<<<<< HEAD
#include <linux/pm.h>

#include <asm/alternative.h>
#include <asm/cpufeature.h>
#include <asm/processor.h>
=======
#include <linux/static_call.h>

#include <asm/alternative.h>
#include <asm/cpufeature.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/apicdef.h>
#include <linux/atomic.h>
#include <asm/fixmap.h>
#include <asm/mpspec.h>
#include <asm/msr.h>
<<<<<<< HEAD
=======
#include <asm/hardirq.h>
#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ARCH_APICTIMER_STOPS_ON_C3	1

/*
 * Debugging macros
 */
#define APIC_QUIET   0
#define APIC_VERBOSE 1
#define APIC_DEBUG   2

<<<<<<< HEAD
=======
/* Macros for apic_extnmi which controls external NMI masking */
#define APIC_EXTNMI_BSP		0 /* Default */
#define APIC_EXTNMI_ALL		1
#define APIC_EXTNMI_NONE	2

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Define the default level of output to be very little
 * This can be turned up by using apic=verbose for more
 * information and apic=debug for _lots_ of information.
 * apic_verbosity is defined in apic.c
 */
#define apic_printk(v, s, a...) do {       \
		if ((v) <= apic_verbosity) \
			printk(s, ##a);    \
	} while (0)


#if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
<<<<<<< HEAD
extern void generic_apic_probe(void);
#else
static inline void generic_apic_probe(void)
{
}
#endif

#ifdef CONFIG_X86_LOCAL_APIC

extern unsigned int apic_verbosity;
extern int local_apic_timer_c2_ok;

extern int disable_apic;
extern unsigned int lapic_timer_frequency;

#ifdef CONFIG_SMP
extern void __inquire_remote_apic(int apicid);
#else /* CONFIG_SMP */
static inline void __inquire_remote_apic(int apicid)
{
}
#endif /* CONFIG_SMP */

static inline void default_inquire_remote_apic(int apicid)
{
	if (apic_verbosity >= APIC_DEBUG)
		__inquire_remote_apic(apicid);
}
=======
extern void x86_32_probe_apic(void);
#else
static inline void x86_32_probe_apic(void) { }
#endif

extern u32 cpuid_to_apicid[];

#define CPU_ACPIID_INVALID	U32_MAX

#ifdef CONFIG_X86_LOCAL_APIC

extern int apic_verbosity;
extern int local_apic_timer_c2_ok;

extern bool apic_is_disabled;
extern unsigned int lapic_timer_period;

extern enum apic_intr_mode_id apic_intr_mode;
enum apic_intr_mode_id {
	APIC_PIC,
	APIC_VIRTUAL_WIRE,
	APIC_VIRTUAL_WIRE_NO_CONFIG,
	APIC_SYMMETRIC_IO,
	APIC_SYMMETRIC_IO_NO_ROUTING
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * With 82489DX we can't rely on apic feature bit
 * retrieved via cpuid but still have to deal with
 * such an apic chip so we assume that SMP configuration
 * is found from MP table (64bit case uses ACPI mostly
 * which set smp presence flag as well so we are safe
 * to use this helper too).
 */
static inline bool apic_from_smp_config(void)
{
<<<<<<< HEAD
	return smp_found_config && !disable_apic;
=======
	return smp_found_config && !apic_is_disabled;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Basic functions accessing APICs.
 */
#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#endif

<<<<<<< HEAD
#ifdef CONFIG_X86_64
extern int is_vsmp_box(void);
#else
static inline int is_vsmp_box(void)
{
	return 0;
}
#endif
extern void xapic_wait_icr_idle(void);
extern u32 safe_xapic_wait_icr_idle(void);
extern void xapic_icr_write(u32, u32);
extern int setup_profiling_timer(unsigned int);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void native_apic_mem_write(u32 reg, u32 v)
{
	volatile u32 *addr = (volatile u32 *)(APIC_BASE + reg);

<<<<<<< HEAD
	alternative_io("movl %0, %1", "xchgl %0, %1", X86_FEATURE_11AP,
=======
	alternative_io("movl %0, %P1", "xchgl %0, %P1", X86_BUG_11AP,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       ASM_OUTPUT2("=r" (v), "=m" (*addr)),
		       ASM_OUTPUT2("0" (v), "m" (*addr)));
}

static inline u32 native_apic_mem_read(u32 reg)
{
<<<<<<< HEAD
	return *((volatile u32 *)(APIC_BASE + reg));
}

extern void native_apic_wait_icr_idle(void);
extern u32 native_safe_apic_wait_icr_idle(void);
extern void native_apic_icr_write(u32 low, u32 id);
extern u64 native_apic_icr_read(void);

extern int x2apic_mode;

#ifdef CONFIG_X86_X2APIC
/*
 * Make previous memory operations globally visible before
 * sending the IPI through x2apic wrmsr. We need a serializing instruction or
 * mfence for this.
 */
static inline void x2apic_wrmsr_fence(void)
{
	asm volatile("mfence" : : : "memory");
}

=======
	return readl((void __iomem *)(APIC_BASE + reg));
}

static inline void native_apic_mem_eoi(void)
{
	native_apic_mem_write(APIC_EOI, APIC_EOI_ACK);
}

extern void native_apic_icr_write(u32 low, u32 id);
extern u64 native_apic_icr_read(void);

static inline bool apic_is_x2apic_enabled(void)
{
	u64 msr;

	if (rdmsrl_safe(MSR_IA32_APICBASE, &msr))
		return false;
	return msr & X2APIC_ENABLE;
}

extern void enable_IR_x2apic(void);

extern int get_physical_broadcast(void);

extern int lapic_get_maxlvt(void);
extern void clear_local_APIC(void);
extern void disconnect_bsp_APIC(int virt_wire_setup);
extern void disable_local_APIC(void);
extern void apic_soft_disable(void);
extern void lapic_shutdown(void);
extern void sync_Arb_IDs(void);
extern void init_bsp_APIC(void);
extern void apic_intr_mode_select(void);
extern void apic_intr_mode_init(void);
extern void init_apic_mappings(void);
void register_lapic_address(unsigned long address);
extern void setup_boot_APIC_clock(void);
extern void setup_secondary_APIC_clock(void);
extern void lapic_update_tsc_freq(void);

#ifdef CONFIG_X86_64
static inline bool apic_force_enable(unsigned long addr)
{
	return false;
}
#else
extern bool apic_force_enable(unsigned long addr);
#endif

extern void apic_ap_setup(void);

/*
 * On 32bit this is mach-xxx local
 */
#ifdef CONFIG_X86_64
extern int apic_is_clustered_box(void);
#else
static inline int apic_is_clustered_box(void)
{
	return 0;
}
#endif

extern int setup_APIC_eilvt(u8 lvt_off, u8 vector, u8 msg_type, u8 mask);
extern void lapic_assign_system_vectors(void);
extern void lapic_assign_legacy_vector(unsigned int isairq, bool replace);
extern void lapic_update_legacy_vectors(void);
extern void lapic_online(void);
extern void lapic_offline(void);
extern bool apic_needs_pit(void);

extern void apic_send_IPI_allbutself(unsigned int vector);

extern void topology_register_apic(u32 apic_id, u32 acpi_id, bool present);
extern void topology_register_boot_apic(u32 apic_id);
extern int topology_hotplug_apic(u32 apic_id, u32 acpi_id);
extern void topology_hotunplug_apic(unsigned int cpu);
extern void topology_apply_cmdline_limits_early(void);
extern void topology_init_possible_cpus(void);
extern void topology_reset_possible_cpus_up(void);

#else /* !CONFIG_X86_LOCAL_APIC */
static inline void lapic_shutdown(void) { }
#define local_apic_timer_c2_ok		1
static inline void init_apic_mappings(void) { }
static inline void disable_local_APIC(void) { }
# define setup_boot_APIC_clock x86_init_noop
# define setup_secondary_APIC_clock x86_init_noop
static inline void lapic_update_tsc_freq(void) { }
static inline void init_bsp_APIC(void) { }
static inline void apic_intr_mode_select(void) { }
static inline void apic_intr_mode_init(void) { }
static inline void lapic_assign_system_vectors(void) { }
static inline void lapic_assign_legacy_vector(unsigned int i, bool r) { }
static inline bool apic_needs_pit(void) { return true; }
static inline void topology_apply_cmdline_limits_early(void) { }
static inline void topology_init_possible_cpus(void) { }
#endif /* !CONFIG_X86_LOCAL_APIC */

#ifdef CONFIG_X86_X2APIC
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void native_apic_msr_write(u32 reg, u32 v)
{
	if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
	    reg == APIC_LVR)
		return;

	wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
}

<<<<<<< HEAD
=======
static inline void native_apic_msr_eoi(void)
{
	__wrmsr(APIC_BASE_MSR + (APIC_EOI >> 4), APIC_EOI_ACK, 0);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline u32 native_apic_msr_read(u32 reg)
{
	u64 msr;

	if (reg == APIC_DFR)
		return -1;

	rdmsrl(APIC_BASE_MSR + (reg >> 4), msr);
	return (u32)msr;
}

<<<<<<< HEAD
static inline void native_x2apic_wait_icr_idle(void)
{
	/* no need to wait for icr idle in x2apic */
	return;
}

static inline u32 native_safe_x2apic_wait_icr_idle(void)
{
	/* no need to wait for icr idle in x2apic */
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void native_x2apic_icr_write(u32 low, u32 id)
{
	wrmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), ((__u64) id) << 32 | low);
}

static inline u64 native_x2apic_icr_read(void)
{
	unsigned long val;

	rdmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), val);
	return val;
}

<<<<<<< HEAD
extern int x2apic_phys;
extern int x2apic_preenabled;
extern void check_x2apic(void);
extern void enable_x2apic(void);
extern void x2apic_icr_write(u32 low, u32 id);
static inline int x2apic_enabled(void)
{
	u64 msr;

	if (!cpu_has_x2apic)
		return 0;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (msr & X2APIC_ENABLE)
		return 1;
	return 0;
}

#define x2apic_supported()	(cpu_has_x2apic)
static inline void x2apic_force_phys(void)
{
	x2apic_phys = 1;
}
#else
static inline void disable_x2apic(void)
{
}
static inline void check_x2apic(void)
{
}
static inline void enable_x2apic(void)
{
}
static inline int x2apic_enabled(void)
{
	return 0;
}
static inline void x2apic_force_phys(void)
{
}

#define	nox2apic	0
#define	x2apic_preenabled 0
#define	x2apic_supported()	0
#endif

extern void enable_IR_x2apic(void);

extern int get_physical_broadcast(void);

extern int lapic_get_maxlvt(void);
extern void clear_local_APIC(void);
extern void connect_bsp_APIC(void);
extern void disconnect_bsp_APIC(int virt_wire_setup);
extern void disable_local_APIC(void);
extern void lapic_shutdown(void);
extern int verify_local_APIC(void);
extern void sync_Arb_IDs(void);
extern void init_bsp_APIC(void);
extern void setup_local_APIC(void);
extern void end_local_APIC_setup(void);
extern void bsp_end_local_APIC_setup(void);
extern void init_apic_mappings(void);
void register_lapic_address(unsigned long address);
extern void setup_boot_APIC_clock(void);
extern void setup_secondary_APIC_clock(void);
extern int APIC_init_uniprocessor(void);
extern int apic_force_enable(unsigned long addr);

/*
 * On 32bit this is mach-xxx local
 */
#ifdef CONFIG_X86_64
extern int apic_is_clustered_box(void);
#else
static inline int apic_is_clustered_box(void)
{
	return 0;
}
#endif

extern int setup_APIC_eilvt(u8 lvt_off, u8 vector, u8 msg_type, u8 mask);

#else /* !CONFIG_X86_LOCAL_APIC */
static inline void lapic_shutdown(void) { }
#define local_apic_timer_c2_ok		1
static inline void init_apic_mappings(void) { }
static inline void disable_local_APIC(void) { }
# define setup_boot_APIC_clock x86_init_noop
# define setup_secondary_APIC_clock x86_init_noop
#endif /* !CONFIG_X86_LOCAL_APIC */

#ifdef CONFIG_X86_64
#define	SET_APIC_ID(x)		(apic->set_apic_id(x))
#else

#endif

/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
=======
extern int x2apic_mode;
extern int x2apic_phys;
extern void __init x2apic_set_max_apicid(u32 apicid);
extern void x2apic_setup(void);
static inline int x2apic_enabled(void)
{
	return boot_cpu_has(X86_FEATURE_X2APIC) && apic_is_x2apic_enabled();
}

#define x2apic_supported()	(boot_cpu_has(X86_FEATURE_X2APIC))
#else /* !CONFIG_X86_X2APIC */
static inline void x2apic_setup(void) { }
static inline int x2apic_enabled(void) { return 0; }
static inline u32 native_apic_msr_read(u32 reg) { BUG(); }
#define x2apic_mode		(0)
#define	x2apic_supported()	(0)
#endif /* !CONFIG_X86_X2APIC */
extern void __init check_x2apic(void);

struct irq_data;

/*
 * Copyright 2004 James Cleverdon, IBM.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Generic APIC sub-arch data struct.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */
struct apic {
<<<<<<< HEAD
	char *name;

	int (*probe)(void);
	int (*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);
	int (*apic_id_valid)(int apicid);
	int (*apic_id_registered)(void);

	u32 irq_delivery_mode;
	u32 irq_dest_mode;

	const struct cpumask *(*target_cpus)(void);

	int disable_esr;

	int dest_logical;
	unsigned long (*check_apicid_used)(physid_mask_t *map, int apicid);
	unsigned long (*check_apicid_present)(int apicid);

	void (*vector_allocation_domain)(int cpu, struct cpumask *retmask);
	void (*init_apic_ldr)(void);

	void (*ioapic_phys_id_map)(physid_mask_t *phys_map, physid_mask_t *retmap);

	void (*setup_apic_routing)(void);
	int (*multi_timer_check)(int apic, int irq);
	int (*cpu_present_to_apicid)(int mps_cpu);
	void (*apicid_to_cpu_present)(int phys_apicid, physid_mask_t *retmap);
	void (*setup_portio_remap)(void);
	int (*check_phys_apicid_present)(int phys_apicid);
	void (*enable_apic_mode)(void);
	int (*phys_pkg_id)(int cpuid_apic, int index_msb);

	/*
	 * When one of the next two hooks returns 1 the apic
	 * is switched to this. Essentially they are additional
	 * probe functions:
	 */
	int (*mps_oem_check)(struct mpc_table *mpc, char *oem, char *productid);

	unsigned int (*get_apic_id)(unsigned long x);
	unsigned long (*set_apic_id)(unsigned int id);
	unsigned long apic_id_mask;

	unsigned int (*cpu_mask_to_apicid)(const struct cpumask *cpumask);
	unsigned int (*cpu_mask_to_apicid_and)(const struct cpumask *cpumask,
					       const struct cpumask *andmask);

	/* ipi */
	void (*send_IPI_mask)(const struct cpumask *mask, int vector);
	void (*send_IPI_mask_allbutself)(const struct cpumask *mask,
					 int vector);
	void (*send_IPI_allbutself)(int vector);
	void (*send_IPI_all)(int vector);
	void (*send_IPI_self)(int vector);

	/* wakeup_secondary_cpu */
	int (*wakeup_secondary_cpu)(int apicid, unsigned long start_eip);

	int trampoline_phys_low;
	int trampoline_phys_high;

	void (*wait_for_init_deassert)(atomic_t *deassert);
	void (*smp_callin_clear_local_apic)(void);
	void (*inquire_remote_apic)(int apicid);

	/* apic ops */
	u32 (*read)(u32 reg);
	void (*write)(u32 reg, u32 v);
	u64 (*icr_read)(void);
	void (*icr_write)(u32 low, u32 high);
	void (*wait_icr_idle)(void);
	u32 (*safe_wait_icr_idle)(void);

#ifdef CONFIG_X86_32
	/*
	 * Called very early during boot from get_smp_config().  It should
	 * return the logical apicid.  x86_[bios]_cpu_to_apicid is
	 * initialized before this function is called.
	 *
	 * If logical apicid can't be determined that early, the function
	 * may return BAD_APICID.  Logical apicid will be configured after
	 * init_apic_ldr() while bringing up CPUs.  Note that NUMA affinity
	 * won't be applied properly during early boot in this case.
	 */
	int (*x86_32_early_logical_apicid)(int cpu);

	/*
	 * Optional method called from setup_local_APIC() after logical
	 * apicid is guaranteed to be known to initialize apicid -> node
	 * mapping if NUMA initialization hasn't done so already.  Don't
	 * add new users.
	 */
	int (*x86_32_numa_cpu_node)(int cpu);
#endif
=======
	/* Hotpath functions first */
	void	(*eoi)(void);
	void	(*native_eoi)(void);
	void	(*write)(u32 reg, u32 v);
	u32	(*read)(u32 reg);

	/* IPI related functions */
	void	(*wait_icr_idle)(void);
	u32	(*safe_wait_icr_idle)(void);

	void	(*send_IPI)(int cpu, int vector);
	void	(*send_IPI_mask)(const struct cpumask *mask, int vector);
	void	(*send_IPI_mask_allbutself)(const struct cpumask *msk, int vec);
	void	(*send_IPI_allbutself)(int vector);
	void	(*send_IPI_all)(int vector);
	void	(*send_IPI_self)(int vector);

	u32	disable_esr		: 1,
		dest_mode_logical	: 1,
		x2apic_set_max_apicid	: 1,
		nmi_to_offline_cpu	: 1;

	u32	(*calc_dest_apicid)(unsigned int cpu);

	/* ICR related functions */
	u64	(*icr_read)(void);
	void	(*icr_write)(u32 low, u32 high);

	/* The limit of the APIC ID space. */
	u32	max_apic_id;

	/* Probe, setup and smpboot functions */
	int	(*probe)(void);
	int	(*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);

	void	(*init_apic_ldr)(void);
	u32	(*cpu_present_to_apicid)(int mps_cpu);

	u32	(*get_apic_id)(u32 id);

	/* wakeup_secondary_cpu */
	int	(*wakeup_secondary_cpu)(u32 apicid, unsigned long start_eip);
	/* wakeup secondary CPU using 64-bit wakeup point */
	int	(*wakeup_secondary_cpu_64)(u32 apicid, unsigned long start_eip);

	char	*name;
};

struct apic_override {
	void	(*eoi)(void);
	void	(*native_eoi)(void);
	void	(*write)(u32 reg, u32 v);
	u32	(*read)(u32 reg);
	void	(*send_IPI)(int cpu, int vector);
	void	(*send_IPI_mask)(const struct cpumask *mask, int vector);
	void	(*send_IPI_mask_allbutself)(const struct cpumask *msk, int vec);
	void	(*send_IPI_allbutself)(int vector);
	void	(*send_IPI_all)(int vector);
	void	(*send_IPI_self)(int vector);
	u64	(*icr_read)(void);
	void	(*icr_write)(u32 low, u32 high);
	int	(*wakeup_secondary_cpu)(u32 apicid, unsigned long start_eip);
	int	(*wakeup_secondary_cpu_64)(u32 apicid, unsigned long start_eip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Pointer to the local APIC driver in use on this system (there's
 * always just one such driver in use - the kernel decides via an
 * early probing process which one it picks - and then sticks to it):
 */
extern struct apic *apic;

/*
 * APIC drivers are probed based on how they are listed in the .apicdrivers
 * section. So the order is important and enforced by the ordering
 * of different apic driver files in the Makefile.
 *
 * For the files having two apic drivers, we use apic_drivers()
 * to enforce the order with in them.
 */
#define apic_driver(sym)					\
<<<<<<< HEAD
	static struct apic *__apicdrivers_##sym __used		\
	__aligned(sizeof(struct apic *))			\
	__section(.apicdrivers) = { &sym }
=======
	static const struct apic *__apicdrivers_##sym __used		\
	__aligned(sizeof(struct apic *))			\
	__section(".apicdrivers") = { &sym }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define apic_drivers(sym1, sym2)					\
	static struct apic *__apicdrivers_##sym1##sym2[2] __used	\
	__aligned(sizeof(struct apic *))				\
<<<<<<< HEAD
	__section(.apicdrivers) = { &sym1, &sym2 }
=======
	__section(".apicdrivers") = { &sym1, &sym2 }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct apic *__apicdrivers[], *__apicdrivers_end[];

/*
 * APIC functionality to boot other CPUs - only used on SMP:
 */
#ifdef CONFIG_SMP
<<<<<<< HEAD
extern atomic_t init_deasserted;
extern int wakeup_secondary_cpu_via_nmi(int apicid, unsigned long start_eip);
#endif

#ifdef CONFIG_X86_LOCAL_APIC

static inline u32 apic_read(u32 reg)
{
	return apic->read(reg);
}

static inline void apic_write(u32 reg, u32 val)
{
	apic->write(reg, val);
}

static inline u64 apic_icr_read(void)
{
	return apic->icr_read();
}

static inline void apic_icr_write(u32 low, u32 high)
{
	apic->icr_write(low, high);
}

static inline void apic_wait_icr_idle(void)
{
	apic->wait_icr_idle();
}

static inline u32 safe_apic_wait_icr_idle(void)
{
	return apic->safe_wait_icr_idle();
=======
extern int lapic_can_unplug_cpu(void);
#endif

#ifdef CONFIG_X86_LOCAL_APIC
extern struct apic_override __x86_apic_override;

void __init apic_setup_apic_calls(void);
void __init apic_install_driver(struct apic *driver);

#define apic_update_callback(_callback, _fn) {					\
		__x86_apic_override._callback = _fn;				\
		apic->_callback = _fn;						\
		static_call_update(apic_call_##_callback, _fn);			\
		pr_info("APIC: %s() replaced with %ps()\n", #_callback, _fn);	\
}

#define DECLARE_APIC_CALL(__cb)							\
	DECLARE_STATIC_CALL(apic_call_##__cb, *apic->__cb)

DECLARE_APIC_CALL(eoi);
DECLARE_APIC_CALL(native_eoi);
DECLARE_APIC_CALL(icr_read);
DECLARE_APIC_CALL(icr_write);
DECLARE_APIC_CALL(read);
DECLARE_APIC_CALL(send_IPI);
DECLARE_APIC_CALL(send_IPI_mask);
DECLARE_APIC_CALL(send_IPI_mask_allbutself);
DECLARE_APIC_CALL(send_IPI_allbutself);
DECLARE_APIC_CALL(send_IPI_all);
DECLARE_APIC_CALL(send_IPI_self);
DECLARE_APIC_CALL(wait_icr_idle);
DECLARE_APIC_CALL(wakeup_secondary_cpu);
DECLARE_APIC_CALL(wakeup_secondary_cpu_64);
DECLARE_APIC_CALL(write);

static __always_inline u32 apic_read(u32 reg)
{
	return static_call(apic_call_read)(reg);
}

static __always_inline void apic_write(u32 reg, u32 val)
{
	static_call(apic_call_write)(reg, val);
}

static __always_inline void apic_eoi(void)
{
	static_call(apic_call_eoi)();
}

static __always_inline void apic_native_eoi(void)
{
	static_call(apic_call_native_eoi)();
}

static __always_inline u64 apic_icr_read(void)
{
	return static_call(apic_call_icr_read)();
}

static __always_inline void apic_icr_write(u32 low, u32 high)
{
	static_call(apic_call_icr_write)(low, high);
}

static __always_inline void __apic_send_IPI(int cpu, int vector)
{
	static_call(apic_call_send_IPI)(cpu, vector);
}

static __always_inline void __apic_send_IPI_mask(const struct cpumask *mask, int vector)
{
	static_call_mod(apic_call_send_IPI_mask)(mask, vector);
}

static __always_inline void __apic_send_IPI_mask_allbutself(const struct cpumask *mask, int vector)
{
	static_call(apic_call_send_IPI_mask_allbutself)(mask, vector);
}

static __always_inline void __apic_send_IPI_allbutself(int vector)
{
	static_call(apic_call_send_IPI_allbutself)(vector);
}

static __always_inline void __apic_send_IPI_all(int vector)
{
	static_call(apic_call_send_IPI_all)(vector);
}

static __always_inline void __apic_send_IPI_self(int vector)
{
	static_call_mod(apic_call_send_IPI_self)(vector);
}

static __always_inline void apic_wait_icr_idle(void)
{
	static_call_cond(apic_call_wait_icr_idle)();
}

static __always_inline u32 safe_apic_wait_icr_idle(void)
{
	return apic->safe_wait_icr_idle ? apic->safe_wait_icr_idle() : 0;
}

static __always_inline bool apic_id_valid(u32 apic_id)
{
	return apic_id <= apic->max_apic_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else /* CONFIG_X86_LOCAL_APIC */

static inline u32 apic_read(u32 reg) { return 0; }
static inline void apic_write(u32 reg, u32 val) { }
<<<<<<< HEAD
=======
static inline void apic_eoi(void) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline u64 apic_icr_read(void) { return 0; }
static inline void apic_icr_write(u32 low, u32 high) { }
static inline void apic_wait_icr_idle(void) { }
static inline u32 safe_apic_wait_icr_idle(void) { return 0; }
<<<<<<< HEAD

#endif /* CONFIG_X86_LOCAL_APIC */

static inline void ack_APIC_irq(void)
{
	/*
	 * ack_APIC_irq() actually gets compiled as a single instruction
	 * ... yummie.
	 */

	/* Docs say use 0 for future compatibility */
	apic_write(APIC_EOI, 0);
}

static inline unsigned default_get_apic_id(unsigned long x)
{
	unsigned int ver = GET_APIC_VERSION(apic_read(APIC_LVR));

	if (APIC_XAPIC(ver) || boot_cpu_has(X86_FEATURE_EXTD_APICID))
		return (x >> 24) & 0xFF;
	else
		return (x >> 24) & 0x0F;
}

/*
 * Warm reset vector default position:
 */
#define DEFAULT_TRAMPOLINE_PHYS_LOW		0x467
#define DEFAULT_TRAMPOLINE_PHYS_HIGH		0x469

#ifdef CONFIG_X86_64
extern int default_acpi_madt_oem_check(char *, char *);

extern void apic_send_IPI_self(int vector);

DECLARE_PER_CPU(int, x2apic_extra_bits);

extern int default_cpu_present_to_apicid(int mps_cpu);
extern int default_check_phys_apicid_present(int phys_apicid);
#endif

static inline void default_wait_for_init_deassert(atomic_t *deassert)
{
	while (!atomic_read(deassert))
		cpu_relax();
	return;
}

extern void generic_bigsmp_probe(void);


=======
static inline void apic_set_eoi_cb(void (*eoi)(void)) {}
static inline void apic_native_eoi(void) { WARN_ON_ONCE(1); }
static inline void apic_setup_apic_calls(void) { }

#define apic_update_callback(_callback, _fn) do { } while (0)

#endif /* CONFIG_X86_LOCAL_APIC */

extern void apic_ack_irq(struct irq_data *data);

static inline bool lapic_vector_set_in_irr(unsigned int vector)
{
	u32 irr = apic_read(APIC_IRR + (vector / 32 * 0x10));

	return !!(irr & (1U << (vector % 32)));
}

/*
 * Warm reset vector position:
 */
#define TRAMPOLINE_PHYS_LOW		0x467
#define TRAMPOLINE_PHYS_HIGH		0x469

extern void generic_bigsmp_probe(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_LOCAL_APIC

#include <asm/smp.h>

<<<<<<< HEAD
#define APIC_DFR_VALUE	(APIC_DFR_FLAT)

static inline const struct cpumask *default_target_cpus(void)
{
#ifdef CONFIG_SMP
	return cpu_online_mask;
#else
	return cpumask_of(0);
#endif
}

DECLARE_EARLY_PER_CPU(u16, x86_bios_cpu_apicid);


static inline unsigned int read_apic_id(void)
{
	unsigned int reg;

	reg = apic_read(APIC_ID);
=======
extern struct apic apic_noop;

static inline u32 read_apic_id(void)
{
	u32 reg = apic_read(APIC_ID);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return apic->get_apic_id(reg);
}

<<<<<<< HEAD
static inline int default_apic_id_valid(int apicid)
{
	return (apicid < 255);
}

extern void default_setup_apic_routing(void);

extern struct apic apic_noop;

#ifdef CONFIG_X86_32

static inline int noop_x86_32_early_logical_apicid(int cpu)
{
	return BAD_APICID;
}

/*
 * Set up the logical destination ID.
 *
 * Intel recommends to set DFR, LDR and TPR before enabling
 * an APIC.  See e.g. "AP-388 82489DX User's Manual" (Intel
 * document number 292116).  So here it goes...
 */
extern void default_init_apic_ldr(void);

static inline int default_apic_id_registered(void)
{
	return physid_isset(read_apic_id(), phys_cpu_present_map);
}

static inline int default_phys_pkg_id(int cpuid_apic, int index_msb)
{
	return cpuid_apic >> index_msb;
}

#endif

static inline unsigned int
default_cpu_mask_to_apicid(const struct cpumask *cpumask)
{
	return cpumask_bits(cpumask)[0] & APIC_ALL_CPUS;
}

static inline unsigned int
default_cpu_mask_to_apicid_and(const struct cpumask *cpumask,
			       const struct cpumask *andmask)
{
	unsigned long mask1 = cpumask_bits(cpumask)[0];
	unsigned long mask2 = cpumask_bits(andmask)[0];
	unsigned long mask3 = cpumask_bits(cpu_online_mask)[0];

	return (unsigned int)(mask1 & mask2 & mask3);
}

static inline unsigned long default_check_apicid_used(physid_mask_t *map, int apicid)
{
	return physid_isset(apicid, *map);
}

static inline unsigned long default_check_apicid_present(int bit)
{
	return physid_isset(bit, phys_cpu_present_map);
}

static inline void default_ioapic_phys_id_map(physid_mask_t *phys_map, physid_mask_t *retmap)
{
	*retmap = *phys_map;
}

static inline int __default_cpu_present_to_apicid(int mps_cpu)
{
	if (mps_cpu < nr_cpu_ids && cpu_present(mps_cpu))
		return (int)per_cpu(x86_bios_cpu_apicid, mps_cpu);
	else
		return BAD_APICID;
}

static inline int
__default_check_phys_apicid_present(int phys_apicid)
{
	return physid_isset(phys_apicid, phys_cpu_present_map);
}

#ifdef CONFIG_X86_32
static inline int default_cpu_present_to_apicid(int mps_cpu)
{
	return __default_cpu_present_to_apicid(mps_cpu);
}

static inline int
default_check_phys_apicid_present(int phys_apicid)
{
	return __default_check_phys_apicid_present(phys_apicid);
}
#else
extern int default_cpu_present_to_apicid(int mps_cpu);
extern int default_check_phys_apicid_present(int phys_apicid);
#endif

#endif /* CONFIG_X86_LOCAL_APIC */
=======
#ifdef CONFIG_X86_64
typedef int (*wakeup_cpu_handler)(int apicid, unsigned long start_eip);
extern int default_acpi_madt_oem_check(char *, char *);
extern void x86_64_probe_apic(void);
#else
static inline int default_acpi_madt_oem_check(char *a, char *b) { return 0; }
static inline void x86_64_probe_apic(void) { }
#endif

extern int default_apic_id_valid(u32 apicid);

extern u32 apic_default_calc_apicid(unsigned int cpu);
extern u32 apic_flat_calc_apicid(unsigned int cpu);

extern u32 default_cpu_present_to_apicid(int mps_cpu);

void apic_send_nmi_to_offline_cpu(unsigned int cpu);

#else /* CONFIG_X86_LOCAL_APIC */

static inline u32 read_apic_id(void) { return 0; }

#endif /* !CONFIG_X86_LOCAL_APIC */

#ifdef CONFIG_SMP
void apic_smt_update(void);
#else
static inline void apic_smt_update(void) { }
#endif

struct msi_msg;
struct irq_cfg;

extern void __irq_msi_compose_msg(struct irq_cfg *cfg, struct msi_msg *msg,
				  bool dmar);

extern void ioapic_zap_locks(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_APIC_H */
