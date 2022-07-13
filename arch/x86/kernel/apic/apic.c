<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Local APIC handling, local APIC timers
 *
 *	(c) 1999, 2000, 2009 Ingo Molnar <mingo@redhat.com>
 *
 *	Fixes
 *	Maciej W. Rozycki	:	Bits for genuine 82489DX APICs;
 *					thanks to Eric Gilmore
 *					and Rolf G. Tews
 *					for testing these extensively.
 *	Maciej W. Rozycki	:	Various updates and fixes.
 *	Mikael Pettersson	:	Power Management for UP-APIC.
 *	Pavel Machek and
 *	Mikael Pettersson	:	PM converted to driver model.
 */

#include <linux/perf_event.h>
#include <linux/kernel_stat.h>
#include <linux/mc146818rtc.h>
#include <linux/acpi_pmtmr.h>
<<<<<<< HEAD
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/bootmem.h>
#include <linux/ftrace.h>
#include <linux/ioport.h>
#include <linux/module.h>
=======
#include <linux/bitmap.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/memblock.h>
#include <linux/ftrace.h>
#include <linux/ioport.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/syscore_ops.h>
#include <linux/delay.h>
#include <linux/timex.h>
#include <linux/i8253.h>
#include <linux/dmar.h>
#include <linux/init.h>
#include <linux/cpu.h>
#include <linux/dmi.h>
#include <linux/smp.h>
#include <linux/mm.h>

<<<<<<< HEAD
#include <asm/perf_event.h>
#include <asm/x86_init.h>
#include <asm/pgalloc.h>
#include <linux/atomic.h>
#include <asm/mpspec.h>
#include <asm/i8259.h>
#include <asm/proto.h>
#include <asm/apic.h>
#include <asm/io_apic.h>
#include <asm/desc.h>
#include <asm/hpet.h>
#include <asm/idle.h>
=======
#include <xen/xen.h>

#include <asm/trace/irq_vectors.h>
#include <asm/irq_remapping.h>
#include <asm/pc-conf-reg.h>
#include <asm/perf_event.h>
#include <asm/x86_init.h>
#include <linux/atomic.h>
#include <asm/barrier.h>
#include <asm/mpspec.h>
#include <asm/i8259.h>
#include <asm/proto.h>
#include <asm/traps.h>
#include <asm/apic.h>
#include <asm/acpi.h>
#include <asm/io_apic.h>
#include <asm/desc.h>
#include <asm/hpet.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/mtrr.h>
#include <asm/time.h>
#include <asm/smp.h>
#include <asm/mce.h>
#include <asm/tsc.h>
#include <asm/hypervisor.h>
<<<<<<< HEAD

unsigned int num_processors;

unsigned disabled_cpus __cpuinitdata;

/* Processor that is doing the boot up */
unsigned int boot_cpu_physical_apicid = -1U;

/*
 * The highest APIC ID seen during enumeration.
 */
unsigned int max_physical_apicid;

/*
 * Bitmask of physically existing CPUs:
 */
physid_mask_t phys_cpu_present_map;

/*
 * Map cpu index to physical APIC ID
 */
DEFINE_EARLY_PER_CPU(u16, x86_cpu_to_apicid, BAD_APICID);
DEFINE_EARLY_PER_CPU(u16, x86_bios_cpu_apicid, BAD_APICID);
EXPORT_EARLY_PER_CPU_SYMBOL(x86_cpu_to_apicid);
EXPORT_EARLY_PER_CPU_SYMBOL(x86_bios_cpu_apicid);

#ifdef CONFIG_X86_32

/*
 * On x86_32, the mapping between cpu and logical apicid may vary
 * depending on apic in use.  The following early percpu variable is
 * used for the mapping.  This is where the behaviors of x86_64 and 32
 * actually diverge.  Let's keep it ugly for now.
 */
DEFINE_EARLY_PER_CPU(int, x86_cpu_to_logical_apicid, BAD_APICID);

/*
 * Knob to control our willingness to enable the local APIC.
 *
 * +1=force-enable
 */
static int force_enable_local_apic __initdata;
/*
 * APIC command line parameters
 */
static int __init parse_lapic(char *arg)
{
	force_enable_local_apic = 1;
	return 0;
}
early_param("lapic", parse_lapic);
/* Local APIC was disabled by the BIOS and enabled by the kernel */
static int enabled_via_apicbase;
=======
#include <asm/cpu_device_id.h>
#include <asm/intel-family.h>
#include <asm/irq_regs.h>
#include <asm/cpu.h>

#include "local.h"

/* Processor that is doing the boot up */
u32 boot_cpu_physical_apicid __ro_after_init = BAD_APICID;
EXPORT_SYMBOL_GPL(boot_cpu_physical_apicid);

u8 boot_cpu_apic_version __ro_after_init;

/*
 * This variable controls which CPUs receive external NMIs.  By default,
 * external NMIs are delivered only to the BSP.
 */
static int apic_extnmi __ro_after_init = APIC_EXTNMI_BSP;

/*
 * Hypervisor supports 15 bits of APIC ID in MSI Extended Destination ID
 */
static bool virt_ext_dest_id __ro_after_init;

/* For parallel bootup. */
unsigned long apic_mmio_base __ro_after_init;

static inline bool apic_accessible(void)
{
	return x2apic_mode || apic_mmio_base;
}

#ifdef CONFIG_X86_32
/* Local APIC was disabled by the BIOS and enabled by the kernel */
static int enabled_via_apicbase __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Handle interrupt mode configuration register (IMCR).
 * This register controls whether the interrupt signals
 * that reach the BSP come from the master PIC or from the
 * local APIC. Before entering Symmetric I/O Mode, either
 * the BIOS or the operating system must switch out of
 * PIC Mode by changing the IMCR.
 */
static inline void imcr_pic_to_apic(void)
{
<<<<<<< HEAD
	/* select IMCR register */
	outb(0x70, 0x22);
	/* NMI and 8259 INTR go through APIC */
	outb(0x01, 0x23);
=======
	/* NMI and 8259 INTR go through APIC */
	pc_conf_set(PC_CONF_MPS_IMCR, 0x01);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void imcr_apic_to_pic(void)
{
<<<<<<< HEAD
	/* select IMCR register */
	outb(0x70, 0x22);
	/* NMI and 8259 INTR go directly to BSP */
	outb(0x00, 0x23);
}
#endif

=======
	/* NMI and 8259 INTR go directly to BSP */
	pc_conf_set(PC_CONF_MPS_IMCR, 0x00);
}
#endif

/*
 * Knob to control our willingness to enable the local APIC.
 *
 * +1=force-enable
 */
static int force_enable_local_apic __initdata;

/*
 * APIC command line parameters
 */
static int __init parse_lapic(char *arg)
{
	if (IS_ENABLED(CONFIG_X86_32) && !arg)
		force_enable_local_apic = 1;
	else if (arg && !strncmp(arg, "notscdeadline", 13))
		setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
	return 0;
}
early_param("lapic", parse_lapic);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_64
static int apic_calibrate_pmtmr __initdata;
static __init int setup_apicpmtimer(char *s)
{
	apic_calibrate_pmtmr = 1;
	notsc_setup(NULL);
<<<<<<< HEAD
	return 0;
=======
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
__setup("apicpmtimer", setup_apicpmtimer);
#endif

<<<<<<< HEAD
int x2apic_mode;
#ifdef CONFIG_X86_X2APIC
/* x2apic enabled before OS handover */
int x2apic_preenabled;
static int x2apic_disabled;
static int nox2apic;
static __init int setup_nox2apic(char *str)
{
	if (x2apic_enabled()) {
		int apicid = native_apic_msr_read(APIC_ID);

		if (apicid >= 255) {
			pr_warning("Apicid: %08x, cannot enforce nox2apic\n",
				   apicid);
			return 0;
		}

		pr_warning("x2apic already enabled. will disable it\n");
	} else
		setup_clear_cpu_cap(X86_FEATURE_X2APIC);

	nox2apic = 1;

	return 0;
}
early_param("nox2apic", setup_nox2apic);
#endif

unsigned long mp_lapic_addr;
int disable_apic;
/* Disable local APIC timer from the kernel commandline or via dmi quirk */
static int disable_apic_timer __initdata;
/* Local APIC timer works in C2 */
int local_apic_timer_c2_ok;
EXPORT_SYMBOL_GPL(local_apic_timer_c2_ok);

int first_system_vector = 0xfe;

/*
 * Debug level, exported for io_apic.c
 */
unsigned int apic_verbosity;

int pic_mode;

/* Have we found an MP table */
int smp_found_config;
=======
static unsigned long mp_lapic_addr __ro_after_init;
bool apic_is_disabled __ro_after_init;
/* Disable local APIC timer from the kernel commandline or via dmi quirk */
static int disable_apic_timer __initdata;
/* Local APIC timer works in C2 */
int local_apic_timer_c2_ok __ro_after_init;
EXPORT_SYMBOL_GPL(local_apic_timer_c2_ok);

/*
 * Debug level, exported for io_apic.c
 */
int apic_verbosity __ro_after_init;

int pic_mode __ro_after_init;

/* Have we found an MP table */
int smp_found_config __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct resource lapic_resource = {
	.name = "Local APIC",
	.flags = IORESOURCE_MEM | IORESOURCE_BUSY,
};

<<<<<<< HEAD
unsigned int lapic_timer_frequency = 0;

static void apic_pm_activate(void);

static unsigned long apic_phys;

=======
unsigned int lapic_timer_period = 0;

static void apic_pm_activate(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Get the LAPIC version
 */
static inline int lapic_get_version(void)
{
	return GET_APIC_VERSION(apic_read(APIC_LVR));
}

/*
 * Check, if the APIC is integrated or a separate chip
 */
static inline int lapic_is_integrated(void)
{
<<<<<<< HEAD
#ifdef CONFIG_X86_64
	return 1;
#else
	return APIC_INTEGRATED(lapic_get_version());
#endif
=======
	return APIC_INTEGRATED(lapic_get_version());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check, whether this is a modern or a first generation APIC
 */
static int modern_apic(void)
{
	/* AMD systems use old APIC versions, so check the CPU */
	if (boot_cpu_data.x86_vendor == X86_VENDOR_AMD &&
	    boot_cpu_data.x86 >= 0xf)
		return 1;
<<<<<<< HEAD
=======

	/* Hygon systems use modern APIC */
	if (boot_cpu_data.x86_vendor == X86_VENDOR_HYGON)
		return 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return lapic_get_version() >= 0x14;
}

/*
 * right after this call apic become NOOP driven
 * so apic->write/read doesn't do anything
 */
static void __init apic_disable(void)
{
<<<<<<< HEAD
	pr_info("APIC: switched to apic NOOP\n");
	apic = &apic_noop;
}

void native_apic_wait_icr_idle(void)
{
	while (apic_read(APIC_ICR) & APIC_ICR_BUSY)
		cpu_relax();
}

u32 native_safe_apic_wait_icr_idle(void)
{
	u32 send_status;
	int timeout;

	timeout = 0;
	do {
		send_status = apic_read(APIC_ICR) & APIC_ICR_BUSY;
		if (!send_status)
			break;
		inc_irq_stat(icr_read_retry_count);
		udelay(100);
	} while (timeout++ < 1000);

	return send_status;
=======
	apic_install_driver(&apic_noop);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void native_apic_icr_write(u32 low, u32 id)
{
<<<<<<< HEAD
	apic_write(APIC_ICR2, SET_APIC_DEST_FIELD(id));
	apic_write(APIC_ICR, low);
=======
	unsigned long flags;

	local_irq_save(flags);
	apic_write(APIC_ICR2, SET_XAPIC_DEST_FIELD(id));
	apic_write(APIC_ICR, low);
	local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

u64 native_apic_icr_read(void)
{
	u32 icr1, icr2;

	icr2 = apic_read(APIC_ICR2);
	icr1 = apic_read(APIC_ICR);

	return icr1 | ((u64)icr2 << 32);
}

<<<<<<< HEAD
#ifdef CONFIG_X86_32
/**
 * get_physical_broadcast - Get number of physical broadcast IDs
 */
int get_physical_broadcast(void)
{
	return modern_apic() ? 0xff : 0xf;
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * lapic_get_maxlvt - get the maximum number of local vector table entries
 */
int lapic_get_maxlvt(void)
{
<<<<<<< HEAD
	unsigned int v;

	v = apic_read(APIC_LVR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * - we always have APIC integrated on 64bit mode
	 * - 82489DXs do not report # of LVT entries
	 */
<<<<<<< HEAD
	return APIC_INTEGRATED(GET_APIC_VERSION(v)) ? GET_APIC_MAXLVT(v) : 2;
=======
	return lapic_is_integrated() ? GET_APIC_MAXLVT(apic_read(APIC_LVR)) : 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Local APIC timer
 */

/* Clock divisor */
#define APIC_DIVISOR 16
<<<<<<< HEAD
=======
#define TSC_DIVISOR  8

/* i82489DX specific */
#define		I82489DX_BASE_DIVIDER		(((0x2) << 18))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This function sets up the local APIC timer, with a timeout of
 * 'clocks' APIC bus clock. During calibration we actually call
 * this function twice on the boot CPU, once with a bogus timeout
 * value, second time for real. The other (noncalibrating) CPUs
 * call this function only once, with the real, calibrated value.
 *
 * We do reads before writes even if unnecessary, to get around the
 * P5 APIC double write bug.
 */
static void __setup_APIC_LVTT(unsigned int clocks, int oneshot, int irqen)
{
	unsigned int lvtt_value, tmp_value;

	lvtt_value = LOCAL_TIMER_VECTOR;
	if (!oneshot)
		lvtt_value |= APIC_LVT_TIMER_PERIODIC;
<<<<<<< HEAD
	if (!lapic_is_integrated())
		lvtt_value |= SET_APIC_TIMER_BASE(APIC_TIMER_BASE_DIV);
=======
	else if (boot_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER))
		lvtt_value |= APIC_LVT_TIMER_TSCDEADLINE;

	/*
	 * The i82489DX APIC uses bit 18 and 19 for the base divider.  This
	 * overlaps with bit 18 on integrated APICs, but is not documented
	 * in the SDM. No problem though. i82489DX equipped systems do not
	 * have TSC deadline timer.
	 */
	if (!lapic_is_integrated())
		lvtt_value |= I82489DX_BASE_DIVIDER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!irqen)
		lvtt_value |= APIC_LVT_MASKED;

	apic_write(APIC_LVTT, lvtt_value);

<<<<<<< HEAD
=======
	if (lvtt_value & APIC_LVT_TIMER_TSCDEADLINE) {
		/*
		 * See Intel SDM: TSC-Deadline Mode chapter. In xAPIC mode,
		 * writing to the APIC LVTT and TSC_DEADLINE MSR isn't serialized.
		 * According to Intel, MFENCE can do the serialization here.
		 */
		asm volatile("mfence" : : : "memory");
		return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Divide PICLK by 16
	 */
	tmp_value = apic_read(APIC_TDCR);
	apic_write(APIC_TDCR,
		(tmp_value & ~(APIC_TDR_DIV_1 | APIC_TDR_DIV_TMBASE)) |
		APIC_TDR_DIV_16);

	if (!oneshot)
		apic_write(APIC_TMICT, clocks / APIC_DIVISOR);
}

/*
 * Setup extended LVT, AMD specific
 *
 * Software should use the LVT offsets the BIOS provides.  The offsets
 * are determined by the subsystems using it like those for MCE
 * threshold or IBS.  On K8 only offset 0 (APIC500) and MCE interrupts
 * are supported. Beginning with family 10h at least 4 offsets are
 * available.
 *
 * Since the offsets must be consistent for all cores, we keep track
 * of the LVT offsets in software and reserve the offset for the same
 * vector also to be used on other cores. An offset is freed by
 * setting the entry to APIC_EILVT_MASKED.
 *
 * If the BIOS is right, there should be no conflicts. Otherwise a
 * "[Firmware Bug]: ..." error message is generated. However, if
 * software does not properly determines the offsets, it is not
 * necessarily a BIOS bug.
 */

static atomic_t eilvt_offsets[APIC_EILVT_NR_MAX];

static inline int eilvt_entry_is_changeable(unsigned int old, unsigned int new)
{
	return (old & APIC_EILVT_MASKED)
		|| (new == APIC_EILVT_MASKED)
		|| ((new & ~APIC_EILVT_MASKED) == old);
}

static unsigned int reserve_eilvt_offset(int offset, unsigned int new)
{
	unsigned int rsvd, vector;

	if (offset >= APIC_EILVT_NR_MAX)
		return ~0;

	rsvd = atomic_read(&eilvt_offsets[offset]);
	do {
		vector = rsvd & ~APIC_EILVT_MASKED;	/* 0: unassigned */
		if (vector && !eilvt_entry_is_changeable(vector, new))
			/* may not change if vectors are different */
			return rsvd;
<<<<<<< HEAD
		rsvd = atomic_cmpxchg(&eilvt_offsets[offset], rsvd, new);
	} while (rsvd != new);

	rsvd &= ~APIC_EILVT_MASKED;
=======
	} while (!atomic_try_cmpxchg(&eilvt_offsets[offset], &rsvd, new));

	rsvd = new & ~APIC_EILVT_MASKED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rsvd && rsvd != vector)
		pr_info("LVT offset %d assigned for vector 0x%02x\n",
			offset, rsvd);

	return new;
}

/*
 * If mask=1, the LVT entry does not generate interrupts while mask=0
 * enables the vector. See also the BKDGs. Must be called with
 * preemption disabled.
 */

int setup_APIC_eilvt(u8 offset, u8 vector, u8 msg_type, u8 mask)
{
	unsigned long reg = APIC_EILVTn(offset);
	unsigned int new, old, reserved;

	new = (mask << 16) | (msg_type << 8) | vector;
	old = apic_read(reg);
	reserved = reserve_eilvt_offset(offset, new);

	if (reserved != new) {
		pr_err(FW_BUG "cpu %d, try to use APIC%lX (LVT offset %d) for "
		       "vector 0x%x, but the register is already in use for "
		       "vector 0x%x on another cpu\n",
		       smp_processor_id(), reg, offset, new, reserved);
		return -EINVAL;
	}

	if (!eilvt_entry_is_changeable(old, new)) {
		pr_err(FW_BUG "cpu %d, try to use APIC%lX (LVT offset %d) for "
		       "vector 0x%x, but the register is already in use for "
		       "vector 0x%x on this cpu\n",
		       smp_processor_id(), reg, offset, new, old);
		return -EBUSY;
	}

	apic_write(reg, new);

	return 0;
}
EXPORT_SYMBOL_GPL(setup_APIC_eilvt);

/*
 * Program the next event, relative to now
 */
static int lapic_next_event(unsigned long delta,
			    struct clock_event_device *evt)
{
	apic_write(APIC_TMICT, delta);
	return 0;
}

<<<<<<< HEAD
/*
 * Setup the lapic timer in periodic or oneshot mode
 */
static void lapic_timer_setup(enum clock_event_mode mode,
			      struct clock_event_device *evt)
{
	unsigned long flags;
=======
static int lapic_next_deadline(unsigned long delta,
			       struct clock_event_device *evt)
{
	u64 tsc;

	/* This MSR is special and need a special fence: */
	weak_wrmsr_fence();

	tsc = rdtsc();
	wrmsrl(MSR_IA32_TSC_DEADLINE, tsc + (((u64) delta) * TSC_DIVISOR));
	return 0;
}

static int lapic_timer_shutdown(struct clock_event_device *evt)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int v;

	/* Lapic used as dummy for broadcast ? */
	if (evt->features & CLOCK_EVT_FEAT_DUMMY)
<<<<<<< HEAD
		return;

	local_irq_save(flags);

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
	case CLOCK_EVT_MODE_ONESHOT:
		__setup_APIC_LVTT(lapic_timer_frequency,
				  mode != CLOCK_EVT_MODE_PERIODIC, 1);
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		v = apic_read(APIC_LVTT);
		v |= (APIC_LVT_MASKED | LOCAL_TIMER_VECTOR);
		apic_write(APIC_LVTT, v);
		apic_write(APIC_TMICT, 0);
		break;
	case CLOCK_EVT_MODE_RESUME:
		/* Nothing to do here */
		break;
	}

	local_irq_restore(flags);
=======
		return 0;

	v = apic_read(APIC_LVTT);
	v |= (APIC_LVT_MASKED | LOCAL_TIMER_VECTOR);
	apic_write(APIC_LVTT, v);
	apic_write(APIC_TMICT, 0);
	return 0;
}

static inline int
lapic_timer_set_periodic_oneshot(struct clock_event_device *evt, bool oneshot)
{
	/* Lapic used as dummy for broadcast ? */
	if (evt->features & CLOCK_EVT_FEAT_DUMMY)
		return 0;

	__setup_APIC_LVTT(lapic_timer_period, oneshot, 1);
	return 0;
}

static int lapic_timer_set_periodic(struct clock_event_device *evt)
{
	return lapic_timer_set_periodic_oneshot(evt, false);
}

static int lapic_timer_set_oneshot(struct clock_event_device *evt)
{
	return lapic_timer_set_periodic_oneshot(evt, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Local APIC timer broadcast function
 */
static void lapic_timer_broadcast(const struct cpumask *mask)
{
#ifdef CONFIG_SMP
<<<<<<< HEAD
	apic->send_IPI_mask(mask, LOCAL_TIMER_VECTOR);
=======
	__apic_send_IPI_mask(mask, LOCAL_TIMER_VECTOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}


/*
 * The local apic timer can be used for any function which is CPU local.
 */
static struct clock_event_device lapic_clockevent = {
<<<<<<< HEAD
	.name		= "lapic",
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT
			| CLOCK_EVT_FEAT_C3STOP | CLOCK_EVT_FEAT_DUMMY,
	.shift		= 32,
	.set_mode	= lapic_timer_setup,
	.set_next_event	= lapic_next_event,
	.broadcast	= lapic_timer_broadcast,
	.rating		= 100,
	.irq		= -1,
};
static DEFINE_PER_CPU(struct clock_event_device, lapic_events);

=======
	.name				= "lapic",
	.features			= CLOCK_EVT_FEAT_PERIODIC |
					  CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_C3STOP
					  | CLOCK_EVT_FEAT_DUMMY,
	.shift				= 32,
	.set_state_shutdown		= lapic_timer_shutdown,
	.set_state_periodic		= lapic_timer_set_periodic,
	.set_state_oneshot		= lapic_timer_set_oneshot,
	.set_state_oneshot_stopped	= lapic_timer_shutdown,
	.set_next_event			= lapic_next_event,
	.broadcast			= lapic_timer_broadcast,
	.rating				= 100,
	.irq				= -1,
};
static DEFINE_PER_CPU(struct clock_event_device, lapic_events);

static const struct x86_cpu_id deadline_match[] __initconst = {
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(HASWELL_X, X86_STEPPINGS(0x2, 0x2), 0x3a), /* EP */
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(HASWELL_X, X86_STEPPINGS(0x4, 0x4), 0x0f), /* EX */

	X86_MATCH_INTEL_FAM6_MODEL( BROADWELL_X,	0x0b000020),

	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(BROADWELL_D, X86_STEPPINGS(0x2, 0x2), 0x00000011),
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(BROADWELL_D, X86_STEPPINGS(0x3, 0x3), 0x0700000e),
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(BROADWELL_D, X86_STEPPINGS(0x4, 0x4), 0x0f00000c),
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(BROADWELL_D, X86_STEPPINGS(0x5, 0x5), 0x0e000003),

	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(SKYLAKE_X, X86_STEPPINGS(0x3, 0x3), 0x01000136),
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(SKYLAKE_X, X86_STEPPINGS(0x4, 0x4), 0x02000014),
	X86_MATCH_INTEL_FAM6_MODEL_STEPPINGS(SKYLAKE_X, X86_STEPPINGS(0x5, 0xf), 0),

	X86_MATCH_INTEL_FAM6_MODEL( HASWELL,		0x22),
	X86_MATCH_INTEL_FAM6_MODEL( HASWELL_L,		0x20),
	X86_MATCH_INTEL_FAM6_MODEL( HASWELL_G,		0x17),

	X86_MATCH_INTEL_FAM6_MODEL( BROADWELL,		0x25),
	X86_MATCH_INTEL_FAM6_MODEL( BROADWELL_G,	0x17),

	X86_MATCH_INTEL_FAM6_MODEL( SKYLAKE_L,		0xb2),
	X86_MATCH_INTEL_FAM6_MODEL( SKYLAKE,		0xb2),

	X86_MATCH_INTEL_FAM6_MODEL( KABYLAKE_L,		0x52),
	X86_MATCH_INTEL_FAM6_MODEL( KABYLAKE,		0x52),

	{},
};

static __init bool apic_validate_deadline_timer(void)
{
	const struct x86_cpu_id *m;
	u32 rev;

	if (!boot_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER))
		return false;
	if (boot_cpu_has(X86_FEATURE_HYPERVISOR))
		return true;

	m = x86_match_cpu(deadline_match);
	if (!m)
		return true;

	rev = (u32)m->driver_data;

	if (boot_cpu_data.microcode >= rev)
		return true;

	setup_clear_cpu_cap(X86_FEATURE_TSC_DEADLINE_TIMER);
	pr_err(FW_BUG "TSC_DEADLINE disabled due to Errata; "
	       "please update microcode to version: 0x%x (or later)\n", rev);
	return false;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Setup the local APIC timer for this CPU. Copy the initialized values
 * of the boot CPU and register the clock event in the framework.
 */
<<<<<<< HEAD
static void __cpuinit setup_APIC_timer(void)
{
	struct clock_event_device *levt = &__get_cpu_var(lapic_events);

	if (this_cpu_has(X86_FEATURE_ARAT)) {
		lapic_clockevent.features &= ~CLOCK_EVT_FEAT_C3STOP;
		/* Make LAPIC timer preferrable over percpu HPET */
=======
static void setup_APIC_timer(void)
{
	struct clock_event_device *levt = this_cpu_ptr(&lapic_events);

	if (this_cpu_has(X86_FEATURE_ARAT)) {
		lapic_clockevent.features &= ~CLOCK_EVT_FEAT_C3STOP;
		/* Make LAPIC timer preferable over percpu HPET */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lapic_clockevent.rating = 150;
	}

	memcpy(levt, &lapic_clockevent, sizeof(*levt));
	levt->cpumask = cpumask_of(smp_processor_id());

<<<<<<< HEAD
	clockevents_register_device(levt);
=======
	if (this_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER)) {
		levt->name = "lapic-deadline";
		levt->features &= ~(CLOCK_EVT_FEAT_PERIODIC |
				    CLOCK_EVT_FEAT_DUMMY);
		levt->set_next_event = lapic_next_deadline;
		clockevents_config_and_register(levt,
						tsc_khz * (1000 / TSC_DIVISOR),
						0xF, ~0UL);
	} else
		clockevents_register_device(levt);
}

/*
 * Install the updated TSC frequency from recalibration at the TSC
 * deadline clockevent devices.
 */
static void __lapic_update_tsc_freq(void *info)
{
	struct clock_event_device *levt = this_cpu_ptr(&lapic_events);

	if (!this_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER))
		return;

	clockevents_update_freq(levt, tsc_khz * (1000 / TSC_DIVISOR));
}

void lapic_update_tsc_freq(void)
{
	/*
	 * The clockevent device's ->mult and ->shift can both be
	 * changed. In order to avoid races, schedule the frequency
	 * update code on each CPU.
	 */
	on_each_cpu(__lapic_update_tsc_freq, NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * In this functions we calibrate APIC bus clocks to the external timer.
 *
 * We want to do the calibration only once since we want to have local timer
<<<<<<< HEAD
 * irqs syncron. CPUs connected by the same APIC bus have the very same bus
=======
 * irqs synchronous. CPUs connected by the same APIC bus have the very same bus
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * frequency.
 *
 * This was previously done by reading the PIT/HPET and waiting for a wrap
 * around to find out, that a tick has elapsed. I have a box, where the PIT
 * readout is broken, so it never gets out of the wait loop again. This was
 * also reported by others.
 *
 * Monitoring the jiffies value is inaccurate and the clockevents
 * infrastructure allows us to do a simple substitution of the interrupt
 * handler.
 *
 * The calibration routine also uses the pm_timer when possible, as the PIT
 * happens to run way too slow (factor 2.3 on my VAIO CoreDuo, which goes
 * back to normal later in the boot process).
 */

#define LAPIC_CAL_LOOPS		(HZ/10)

static __initdata int lapic_cal_loops = -1;
static __initdata long lapic_cal_t1, lapic_cal_t2;
static __initdata unsigned long long lapic_cal_tsc1, lapic_cal_tsc2;
static __initdata unsigned long lapic_cal_pm1, lapic_cal_pm2;
static __initdata unsigned long lapic_cal_j1, lapic_cal_j2;

/*
<<<<<<< HEAD
 * Temporary interrupt handler.
=======
 * Temporary interrupt handler and polled calibration function.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void __init lapic_cal_handler(struct clock_event_device *dev)
{
	unsigned long long tsc = 0;
	long tapic = apic_read(APIC_TMCCT);
	unsigned long pm = acpi_pm_read_early();

<<<<<<< HEAD
	if (cpu_has_tsc)
		rdtscll(tsc);
=======
	if (boot_cpu_has(X86_FEATURE_TSC))
		tsc = rdtsc();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (lapic_cal_loops++) {
	case 0:
		lapic_cal_t1 = tapic;
		lapic_cal_tsc1 = tsc;
		lapic_cal_pm1 = pm;
		lapic_cal_j1 = jiffies;
		break;

	case LAPIC_CAL_LOOPS:
		lapic_cal_t2 = tapic;
		lapic_cal_tsc2 = tsc;
		if (pm < lapic_cal_pm1)
			pm += ACPI_PM_OVRRUN;
		lapic_cal_pm2 = pm;
		lapic_cal_j2 = jiffies;
		break;
	}
}

static int __init
calibrate_by_pmtimer(long deltapm, long *delta, long *deltatsc)
{
	const long pm_100ms = PMTMR_TICKS_PER_SEC / 10;
	const long pm_thresh = pm_100ms / 100;
	unsigned long mult;
	u64 res;

#ifndef CONFIG_X86_PM_TIMER
	return -1;
#endif

	apic_printk(APIC_VERBOSE, "... PM-Timer delta = %ld\n", deltapm);

	/* Check, if the PM timer is available */
	if (!deltapm)
		return -1;

	mult = clocksource_hz2mult(PMTMR_TICKS_PER_SEC, 22);

	if (deltapm > (pm_100ms - pm_thresh) &&
	    deltapm < (pm_100ms + pm_thresh)) {
		apic_printk(APIC_VERBOSE, "... PM-Timer result ok\n");
		return 0;
	}

	res = (((u64)deltapm) *  mult) >> 22;
	do_div(res, 1000000);
<<<<<<< HEAD
	pr_warning("APIC calibration not consistent "
		   "with PM-Timer: %ldms instead of 100ms\n",(long)res);
=======
	pr_warn("APIC calibration not consistent "
		"with PM-Timer: %ldms instead of 100ms\n", (long)res);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Correct the lapic counter value */
	res = (((u64)(*delta)) * pm_100ms);
	do_div(res, deltapm);
	pr_info("APIC delta adjusted to PM-Timer: "
		"%lu (%ld)\n", (unsigned long)res, *delta);
	*delta = (long)res;

	/* Correct the tsc counter value */
<<<<<<< HEAD
	if (cpu_has_tsc) {
=======
	if (boot_cpu_has(X86_FEATURE_TSC)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		res = (((u64)(*deltatsc)) * pm_100ms);
		do_div(res, deltapm);
		apic_printk(APIC_VERBOSE, "TSC delta adjusted to "
					  "PM-Timer: %lu (%ld)\n",
					(unsigned long)res, *deltatsc);
		*deltatsc = (long)res;
	}

	return 0;
}

<<<<<<< HEAD
static int __init calibrate_APIC_clock(void)
{
	struct clock_event_device *levt = &__get_cpu_var(lapic_events);
	void (*real_handler)(struct clock_event_device *dev);
=======
static int __init lapic_init_clockevent(void)
{
	if (!lapic_timer_period)
		return -1;

	/* Calculate the scaled math multiplication factor */
	lapic_clockevent.mult = div_sc(lapic_timer_period/APIC_DIVISOR,
					TICK_NSEC, lapic_clockevent.shift);
	lapic_clockevent.max_delta_ns =
		clockevent_delta2ns(0x7FFFFFFF, &lapic_clockevent);
	lapic_clockevent.max_delta_ticks = 0x7FFFFFFF;
	lapic_clockevent.min_delta_ns =
		clockevent_delta2ns(0xF, &lapic_clockevent);
	lapic_clockevent.min_delta_ticks = 0xF;

	return 0;
}

bool __init apic_needs_pit(void)
{
	/*
	 * If the frequencies are not known, PIT is required for both TSC
	 * and apic timer calibration.
	 */
	if (!tsc_khz || !cpu_khz)
		return true;

	/* Is there an APIC at all or is it disabled? */
	if (!boot_cpu_has(X86_FEATURE_APIC) || apic_is_disabled)
		return true;

	/*
	 * If interrupt delivery mode is legacy PIC or virtual wire without
	 * configuration, the local APIC timer won't be set up. Make sure
	 * that the PIT is initialized.
	 */
	if (apic_intr_mode == APIC_PIC ||
	    apic_intr_mode == APIC_VIRTUAL_WIRE_NO_CONFIG)
		return true;

	/* Virt guests may lack ARAT, but still have DEADLINE */
	if (!boot_cpu_has(X86_FEATURE_ARAT))
		return true;

	/* Deadline timer is based on TSC so no further PIT action required */
	if (boot_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER))
		return false;

	/* APIC timer disabled? */
	if (disable_apic_timer)
		return true;
	/*
	 * The APIC timer frequency is known already, no PIT calibration
	 * required. If unknown, let the PIT be initialized.
	 */
	return lapic_timer_period == 0;
}

static int __init calibrate_APIC_clock(void)
{
	struct clock_event_device *levt = this_cpu_ptr(&lapic_events);
	u64 tsc_perj = 0, tsc_start = 0;
	unsigned long jif_start;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long deltaj;
	long delta, deltatsc;
	int pm_referenced = 0;

<<<<<<< HEAD
	/**
	 * check if lapic timer has already been calibrated by platform
	 * specific routine, such as tsc calibration code. if so, we just fill
	 * in the clockevent structure and return.
	 */

	if (lapic_timer_frequency) {
		apic_printk(APIC_VERBOSE, "lapic timer already calibrated %d\n",
				lapic_timer_frequency);
		lapic_clockevent.mult = div_sc(lapic_timer_frequency/APIC_DIVISOR,
					TICK_NSEC, lapic_clockevent.shift);
		lapic_clockevent.max_delta_ns =
			clockevent_delta2ns(0x7FFFFF, &lapic_clockevent);
		lapic_clockevent.min_delta_ns =
			clockevent_delta2ns(0xF, &lapic_clockevent);
=======
	if (boot_cpu_has(X86_FEATURE_TSC_DEADLINE_TIMER))
		return 0;

	/*
	 * Check if lapic timer has already been calibrated by platform
	 * specific routine, such as tsc calibration code. If so just fill
	 * in the clockevent structure and return.
	 */
	if (!lapic_init_clockevent()) {
		apic_printk(APIC_VERBOSE, "lapic timer already calibrated %d\n",
			    lapic_timer_period);
		/*
		 * Direct calibration methods must have an always running
		 * local APIC timer, no need for broadcast timer.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lapic_clockevent.features &= ~CLOCK_EVT_FEAT_DUMMY;
		return 0;
	}

<<<<<<< HEAD
	local_irq_disable();

	/* Replace the global interrupt handler */
	real_handler = global_clock_event->event_handler;
	global_clock_event->event_handler = lapic_cal_handler;
=======
	apic_printk(APIC_VERBOSE, "Using local APIC timer interrupts.\n"
		    "calibrating APIC timer ...\n");

	/*
	 * There are platforms w/o global clockevent devices. Instead of
	 * making the calibration conditional on that, use a polling based
	 * approach everywhere.
	 */
	local_irq_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Setup the APIC counter to maximum. There is no way the lapic
	 * can underflow in the 100ms detection time frame
	 */
	__setup_APIC_LVTT(0xffffffff, 0, 0);

<<<<<<< HEAD
	/* Let the interrupts run */
	local_irq_enable();

	while (lapic_cal_loops <= LAPIC_CAL_LOOPS)
		cpu_relax();

	local_irq_disable();

	/* Restore the real event handler */
	global_clock_event->event_handler = real_handler;

=======
	/*
	 * Methods to terminate the calibration loop:
	 *  1) Global clockevent if available (jiffies)
	 *  2) TSC if available and frequency is known
	 */
	jif_start = READ_ONCE(jiffies);

	if (tsc_khz) {
		tsc_start = rdtsc();
		tsc_perj = div_u64((u64)tsc_khz * 1000, HZ);
	}

	/*
	 * Enable interrupts so the tick can fire, if a global
	 * clockevent device is available
	 */
	local_irq_enable();

	while (lapic_cal_loops <= LAPIC_CAL_LOOPS) {
		/* Wait for a tick to elapse */
		while (1) {
			if (tsc_khz) {
				u64 tsc_now = rdtsc();
				if ((tsc_now - tsc_start) >= tsc_perj) {
					tsc_start += tsc_perj;
					break;
				}
			} else {
				unsigned long jif_now = READ_ONCE(jiffies);

				if (time_after(jif_now, jif_start)) {
					jif_start = jif_now;
					break;
				}
			}
			cpu_relax();
		}

		/* Invoke the calibration routine */
		local_irq_disable();
		lapic_cal_handler(NULL);
		local_irq_enable();
	}

	local_irq_disable();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Build delta t1-t2 as apic timer counts down */
	delta = lapic_cal_t1 - lapic_cal_t2;
	apic_printk(APIC_VERBOSE, "... lapic delta = %ld\n", delta);

	deltatsc = (long)(lapic_cal_tsc2 - lapic_cal_tsc1);

	/* we trust the PM based calibration if possible */
	pm_referenced = !calibrate_by_pmtimer(lapic_cal_pm2 - lapic_cal_pm1,
					&delta, &deltatsc);

<<<<<<< HEAD
	/* Calculate the scaled math multiplication factor */
	lapic_clockevent.mult = div_sc(delta, TICK_NSEC * LAPIC_CAL_LOOPS,
				       lapic_clockevent.shift);
	lapic_clockevent.max_delta_ns =
		clockevent_delta2ns(0x7FFFFFFF, &lapic_clockevent);
	lapic_clockevent.min_delta_ns =
		clockevent_delta2ns(0xF, &lapic_clockevent);

	lapic_timer_frequency = (delta * APIC_DIVISOR) / LAPIC_CAL_LOOPS;
=======
	lapic_timer_period = (delta * APIC_DIVISOR) / LAPIC_CAL_LOOPS;
	lapic_init_clockevent();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	apic_printk(APIC_VERBOSE, "..... delta %ld\n", delta);
	apic_printk(APIC_VERBOSE, "..... mult: %u\n", lapic_clockevent.mult);
	apic_printk(APIC_VERBOSE, "..... calibration result: %u\n",
<<<<<<< HEAD
		    lapic_timer_frequency);

	if (cpu_has_tsc) {
=======
		    lapic_timer_period);

	if (boot_cpu_has(X86_FEATURE_TSC)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		apic_printk(APIC_VERBOSE, "..... CPU clock speed is "
			    "%ld.%04ld MHz.\n",
			    (deltatsc / LAPIC_CAL_LOOPS) / (1000000 / HZ),
			    (deltatsc / LAPIC_CAL_LOOPS) % (1000000 / HZ));
	}

	apic_printk(APIC_VERBOSE, "..... host bus clock speed is "
		    "%u.%04u MHz.\n",
<<<<<<< HEAD
		    lapic_timer_frequency / (1000000 / HZ),
		    lapic_timer_frequency % (1000000 / HZ));
=======
		    lapic_timer_period / (1000000 / HZ),
		    lapic_timer_period % (1000000 / HZ));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Do a sanity check on the APIC calibration result
	 */
<<<<<<< HEAD
	if (lapic_timer_frequency < (1000000 / HZ)) {
		local_irq_enable();
		pr_warning("APIC frequency too slow, disabling apic timer\n");
=======
	if (lapic_timer_period < (1000000 / HZ)) {
		local_irq_enable();
		pr_warn("APIC frequency too slow, disabling apic timer\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -1;
	}

	levt->features &= ~CLOCK_EVT_FEAT_DUMMY;

	/*
<<<<<<< HEAD
	 * PM timer calibration failed or not turned on
	 * so lets try APIC timer based calibration
	 */
	if (!pm_referenced) {
=======
	 * PM timer calibration failed or not turned on so lets try APIC
	 * timer based calibration, if a global clockevent device is
	 * available.
	 */
	if (!pm_referenced && global_clock_event) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		apic_printk(APIC_VERBOSE, "... verify APIC timer\n");

		/*
		 * Setup the apic timer manually
		 */
		levt->event_handler = lapic_cal_handler;
<<<<<<< HEAD
		lapic_timer_setup(CLOCK_EVT_MODE_PERIODIC, levt);
=======
		lapic_timer_set_periodic(levt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lapic_cal_loops = -1;

		/* Let the interrupts run */
		local_irq_enable();

		while (lapic_cal_loops <= LAPIC_CAL_LOOPS)
			cpu_relax();

		/* Stop the lapic timer */
<<<<<<< HEAD
		lapic_timer_setup(CLOCK_EVT_MODE_SHUTDOWN, levt);
=======
		local_irq_disable();
		lapic_timer_shutdown(levt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Jiffies delta */
		deltaj = lapic_cal_j2 - lapic_cal_j1;
		apic_printk(APIC_VERBOSE, "... jiffies delta = %lu\n", deltaj);

		/* Check, if the jiffies result is consistent */
		if (deltaj >= LAPIC_CAL_LOOPS-2 && deltaj <= LAPIC_CAL_LOOPS+2)
			apic_printk(APIC_VERBOSE, "... jiffies result ok\n");
		else
			levt->features |= CLOCK_EVT_FEAT_DUMMY;
<<<<<<< HEAD
	} else
		local_irq_enable();

	if (levt->features & CLOCK_EVT_FEAT_DUMMY) {
		pr_warning("APIC timer disabled due to verification failure\n");
			return -1;
=======
	}
	local_irq_enable();

	if (levt->features & CLOCK_EVT_FEAT_DUMMY) {
		pr_warn("APIC timer disabled due to verification failure\n");
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

/*
 * Setup the boot APIC
 *
 * Calibrate and verify the result.
 */
void __init setup_boot_APIC_clock(void)
{
	/*
	 * The local apic timer can be disabled via the kernel
	 * commandline or from the CPU detection code. Register the lapic
	 * timer as a dummy clock event source on SMP systems, so the
	 * broadcast mechanism is used. On UP systems simply ignore it.
	 */
	if (disable_apic_timer) {
		pr_info("Disabling APIC timer\n");
		/* No broadcast on UP ! */
		if (num_possible_cpus() > 1) {
			lapic_clockevent.mult = 1;
			setup_APIC_timer();
		}
		return;
	}

<<<<<<< HEAD
	apic_printk(APIC_VERBOSE, "Using local APIC timer interrupts.\n"
		    "calibrating APIC timer ...\n");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (calibrate_APIC_clock()) {
		/* No broadcast on UP ! */
		if (num_possible_cpus() > 1)
			setup_APIC_timer();
		return;
	}

	/*
	 * If nmi_watchdog is set to IO_APIC, we need the
	 * PIT/HPET going.  Otherwise register lapic as a dummy
	 * device.
	 */
	lapic_clockevent.features &= ~CLOCK_EVT_FEAT_DUMMY;

	/* Setup the lapic or request the broadcast */
	setup_APIC_timer();
<<<<<<< HEAD
}

void __cpuinit setup_secondary_APIC_clock(void)
{
	setup_APIC_timer();
=======
	amd_e400_c1e_apic_setup();
}

void setup_secondary_APIC_clock(void)
{
	setup_APIC_timer();
	amd_e400_c1e_apic_setup();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The guts of the apic timer interrupt
 */
static void local_apic_timer_interrupt(void)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();
	struct clock_event_device *evt = &per_cpu(lapic_events, cpu);
=======
	struct clock_event_device *evt = this_cpu_ptr(&lapic_events);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Normally we should not be here till LAPIC has been initialized but
	 * in some cases like kdump, its possible that there is a pending LAPIC
	 * timer interrupt from previous kernel's context and is delivered in
	 * new kernel the moment interrupts are enabled.
	 *
	 * Interrupts are enabled early and LAPIC is setup much later, hence
	 * its possible that when we get here evt->event_handler is NULL.
	 * Check for event_handler being NULL and discard the interrupt as
	 * spurious.
	 */
	if (!evt->event_handler) {
<<<<<<< HEAD
		pr_warning("Spurious LAPIC timer interrupt on cpu %d\n", cpu);
		/* Switch it off */
		lapic_timer_setup(CLOCK_EVT_MODE_SHUTDOWN, evt);
=======
		pr_warn("Spurious LAPIC timer interrupt on cpu %d\n",
			smp_processor_id());
		/* Switch it off */
		lapic_timer_shutdown(evt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/*
	 * the NMI deadlock-detector uses this.
	 */
	inc_irq_stat(apic_timer_irqs);

	evt->event_handler(evt);
}

/*
 * Local APIC timer interrupt. This is the most natural way for doing
 * local interrupts, but local timer interrupts can be emulated by
 * broadcast interrupts too. [in case the hw doesn't support APIC timers]
 *
 * [ if a single-CPU system runs an SMP kernel then we call the local
 *   interrupt as well. Thus we cannot inline the local irq ... ]
 */
<<<<<<< HEAD
void __irq_entry smp_apic_timer_interrupt(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	/*
	 * NOTE! We'd better ACK the irq immediately,
	 * because timer handling can be slow.
	 */
	ack_APIC_irq();
	/*
	 * update_process_times() expects us to have done irq_enter().
	 * Besides, if we don't timer interrupts ignore the global
	 * interrupt lock, which is the WrongThing (tm) to do.
	 */
	irq_enter();
	exit_idle();
	local_apic_timer_interrupt();
	irq_exit();
=======
DEFINE_IDTENTRY_SYSVEC(sysvec_apic_timer_interrupt)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	apic_eoi();
	trace_local_timer_entry(LOCAL_TIMER_VECTOR);
	local_apic_timer_interrupt();
	trace_local_timer_exit(LOCAL_TIMER_VECTOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_irq_regs(old_regs);
}

<<<<<<< HEAD
int setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Local APIC start and shutdown
 */

/**
 * clear_local_APIC - shutdown the local APIC
 *
 * This is called, when a CPU is disabled and before rebooting, so the state of
 * the local APIC has no dangling leftovers. Also used to cleanout any BIOS
 * leftovers during boot.
 */
void clear_local_APIC(void)
{
	int maxlvt;
	u32 v;

<<<<<<< HEAD
	/* APIC hasn't been mapped yet */
	if (!x2apic_mode && !apic_phys)
=======
	if (!apic_accessible())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	maxlvt = lapic_get_maxlvt();
	/*
	 * Masking an LVT entry can trigger a local APIC error
	 * if the vector is zero. Mask LVTERR first to prevent this.
	 */
	if (maxlvt >= 3) {
		v = ERROR_APIC_VECTOR; /* any non-zero vector will do */
		apic_write(APIC_LVTERR, v | APIC_LVT_MASKED);
	}
	/*
	 * Careful: we have to set masks only first to deassert
	 * any level-triggered sources.
	 */
	v = apic_read(APIC_LVTT);
	apic_write(APIC_LVTT, v | APIC_LVT_MASKED);
	v = apic_read(APIC_LVT0);
	apic_write(APIC_LVT0, v | APIC_LVT_MASKED);
	v = apic_read(APIC_LVT1);
	apic_write(APIC_LVT1, v | APIC_LVT_MASKED);
	if (maxlvt >= 4) {
		v = apic_read(APIC_LVTPC);
		apic_write(APIC_LVTPC, v | APIC_LVT_MASKED);
	}

	/* lets not touch this if we didn't frob it */
#ifdef CONFIG_X86_THERMAL_VECTOR
	if (maxlvt >= 5) {
		v = apic_read(APIC_LVTTHMR);
		apic_write(APIC_LVTTHMR, v | APIC_LVT_MASKED);
	}
#endif
#ifdef CONFIG_X86_MCE_INTEL
	if (maxlvt >= 6) {
		v = apic_read(APIC_LVTCMCI);
		if (!(v & APIC_LVT_MASKED))
			apic_write(APIC_LVTCMCI, v | APIC_LVT_MASKED);
	}
#endif

	/*
	 * Clean APIC state for other OSs:
	 */
	apic_write(APIC_LVTT, APIC_LVT_MASKED);
	apic_write(APIC_LVT0, APIC_LVT_MASKED);
	apic_write(APIC_LVT1, APIC_LVT_MASKED);
	if (maxlvt >= 3)
		apic_write(APIC_LVTERR, APIC_LVT_MASKED);
	if (maxlvt >= 4)
		apic_write(APIC_LVTPC, APIC_LVT_MASKED);

	/* Integrated APIC (!82489DX) ? */
	if (lapic_is_integrated()) {
		if (maxlvt > 3)
			/* Clear ESR due to Pentium errata 3AP and 11AP */
			apic_write(APIC_ESR, 0);
		apic_read(APIC_ESR);
	}
}

/**
<<<<<<< HEAD
=======
 * apic_soft_disable - Clears and software disables the local APIC on hotplug
 *
 * Contrary to disable_local_APIC() this does not touch the enable bit in
 * MSR_IA32_APICBASE. Clearing that bit on systems based on the 3 wire APIC
 * bus would require a hardware reset as the APIC would lose track of bus
 * arbitration. On systems with FSB delivery APICBASE could be disabled,
 * but it has to be guaranteed that no interrupt is sent to the APIC while
 * in that state and it's not clear from the SDM whether it still responds
 * to INIT/SIPI messages. Stay on the safe side and use software disable.
 */
void apic_soft_disable(void)
{
	u32 value;

	clear_local_APIC();

	/* Soft disable APIC (implies clearing of registers for 82489DX!). */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_SPIV_APIC_ENABLED;
	apic_write(APIC_SPIV, value);
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * disable_local_APIC - clear and disable the local APIC
 */
void disable_local_APIC(void)
{
<<<<<<< HEAD
	unsigned int value;

	/* APIC hasn't been mapped yet */
	if (!x2apic_mode && !apic_phys)
		return;

	clear_local_APIC();

	/*
	 * Disable APIC (implies clearing of registers
	 * for 82489DX!).
	 */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_SPIV_APIC_ENABLED;
	apic_write(APIC_SPIV, value);
=======
	if (!apic_accessible())
		return;

	apic_soft_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_X86_32
	/*
	 * When LAPIC was disabled by the BIOS and enabled by the kernel,
	 * restore the disabled state.
	 */
	if (enabled_via_apicbase) {
		unsigned int l, h;

		rdmsr(MSR_IA32_APICBASE, l, h);
		l &= ~MSR_IA32_APICBASE_ENABLE;
		wrmsr(MSR_IA32_APICBASE, l, h);
	}
#endif
}

/*
 * If Linux enabled the LAPIC against the BIOS default disable it down before
 * re-entering the BIOS on shutdown.  Otherwise the BIOS may get confused and
 * not power-off.  Additionally clear all LVT entries before disable_local_APIC
 * for the case where Linux didn't enable the LAPIC.
 */
void lapic_shutdown(void)
{
	unsigned long flags;

<<<<<<< HEAD
	if (!cpu_has_apic && !apic_from_smp_config())
=======
	if (!boot_cpu_has(X86_FEATURE_APIC) && !apic_from_smp_config())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	local_irq_save(flags);

#ifdef CONFIG_X86_32
	if (!enabled_via_apicbase)
		clear_local_APIC();
	else
#endif
		disable_local_APIC();


	local_irq_restore(flags);
}

<<<<<<< HEAD
/*
 * This is to verify that we're looking at a real local APIC.
 * Check these against your board if the CPUs aren't getting
 * started for no apparent reason.
 */
int __init verify_local_APIC(void)
{
	unsigned int reg0, reg1;

	/*
	 * The version register is read-only in a real APIC.
	 */
	reg0 = apic_read(APIC_LVR);
	apic_printk(APIC_DEBUG, "Getting VERSION: %x\n", reg0);
	apic_write(APIC_LVR, reg0 ^ APIC_LVR_MASK);
	reg1 = apic_read(APIC_LVR);
	apic_printk(APIC_DEBUG, "Getting VERSION: %x\n", reg1);

	/*
	 * The two version reads above should print the same
	 * numbers.  If the second one is different, then we
	 * poke at a non-APIC.
	 */
	if (reg1 != reg0)
		return 0;

	/*
	 * Check if the version looks reasonably.
	 */
	reg1 = GET_APIC_VERSION(reg0);
	if (reg1 == 0x00 || reg1 == 0xff)
		return 0;
	reg1 = lapic_get_maxlvt();
	if (reg1 < 0x02 || reg1 == 0xff)
		return 0;

	/*
	 * The ID register is read/write in a real APIC.
	 */
	reg0 = apic_read(APIC_ID);
	apic_printk(APIC_DEBUG, "Getting ID: %x\n", reg0);
	apic_write(APIC_ID, reg0 ^ apic->apic_id_mask);
	reg1 = apic_read(APIC_ID);
	apic_printk(APIC_DEBUG, "Getting ID: %x\n", reg1);
	apic_write(APIC_ID, reg0);
	if (reg1 != (reg0 ^ apic->apic_id_mask))
		return 0;

	/*
	 * The next two are just to see if we have sane values.
	 * They're only really relevant if we're in Virtual Wire
	 * compatibility mode, but most boxes are anymore.
	 */
	reg0 = apic_read(APIC_LVT0);
	apic_printk(APIC_DEBUG, "Getting LVT0: %x\n", reg0);
	reg1 = apic_read(APIC_LVT1);
	apic_printk(APIC_DEBUG, "Getting LVT1: %x\n", reg1);

	return 1;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * sync_Arb_IDs - synchronize APIC bus arbitration IDs
 */
void __init sync_Arb_IDs(void)
{
	/*
	 * Unsupported on P4 - see Intel Dev. Manual Vol. 3, Ch. 8.6.1 And not
	 * needed on AMD.
	 */
	if (modern_apic() || boot_cpu_data.x86_vendor == X86_VENDOR_AMD)
		return;

	/*
	 * Wait for idle.
	 */
	apic_wait_icr_idle();

	apic_printk(APIC_DEBUG, "Synchronizing Arb IDs.\n");
	apic_write(APIC_ICR, APIC_DEST_ALLINC |
			APIC_INT_LEVELTRIG | APIC_DM_INIT);
}

<<<<<<< HEAD
=======
enum apic_intr_mode_id apic_intr_mode __ro_after_init;

static int __init __apic_intr_mode_select(void)
{
	/* Check kernel option */
	if (apic_is_disabled) {
		pr_info("APIC disabled via kernel command line\n");
		return APIC_PIC;
	}

	/* Check BIOS */
#ifdef CONFIG_X86_64
	/* On 64-bit, the APIC must be integrated, Check local APIC only */
	if (!boot_cpu_has(X86_FEATURE_APIC)) {
		apic_is_disabled = true;
		pr_info("APIC disabled by BIOS\n");
		return APIC_PIC;
	}
#else
	/* On 32-bit, the APIC may be integrated APIC or 82489DX */

	/* Neither 82489DX nor integrated APIC ? */
	if (!boot_cpu_has(X86_FEATURE_APIC) && !smp_found_config) {
		apic_is_disabled = true;
		return APIC_PIC;
	}

	/* If the BIOS pretends there is an integrated APIC ? */
	if (!boot_cpu_has(X86_FEATURE_APIC) &&
		APIC_INTEGRATED(boot_cpu_apic_version)) {
		apic_is_disabled = true;
		pr_err(FW_BUG "Local APIC not detected, force emulation\n");
		return APIC_PIC;
	}
#endif

	/* Check MP table or ACPI MADT configuration */
	if (!smp_found_config) {
		disable_ioapic_support();
		if (!acpi_lapic) {
			pr_info("APIC: ACPI MADT or MP tables are not detected\n");
			return APIC_VIRTUAL_WIRE_NO_CONFIG;
		}
		return APIC_VIRTUAL_WIRE;
	}

#ifdef CONFIG_SMP
	/* If SMP should be disabled, then really disable it! */
	if (!setup_max_cpus) {
		pr_info("APIC: SMP mode deactivated\n");
		return APIC_SYMMETRIC_IO_NO_ROUTING;
	}
#endif

	return APIC_SYMMETRIC_IO;
}

/* Select the interrupt delivery mode for the BSP */
void __init apic_intr_mode_select(void)
{
	apic_intr_mode = __apic_intr_mode_select();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * An initial setup of the virtual wire mode.
 */
void __init init_bsp_APIC(void)
{
	unsigned int value;

	/*
	 * Don't do the setup now if we have a SMP BIOS as the
	 * through-I/O-APIC virtual wire mode might be active.
	 */
<<<<<<< HEAD
	if (smp_found_config || !cpu_has_apic)
=======
	if (smp_found_config || !boot_cpu_has(X86_FEATURE_APIC))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/*
	 * Do not trust the local APIC being empty at bootup.
	 */
	clear_local_APIC();

	/*
	 * Enable APIC.
	 */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_VECTOR_MASK;
	value |= APIC_SPIV_APIC_ENABLED;

#ifdef CONFIG_X86_32
	/* This bit is reserved on P4/Xeon and should be cleared */
	if ((boot_cpu_data.x86_vendor == X86_VENDOR_INTEL) &&
	    (boot_cpu_data.x86 == 15))
		value &= ~APIC_SPIV_FOCUS_DISABLED;
	else
#endif
		value |= APIC_SPIV_FOCUS_DISABLED;
	value |= SPURIOUS_APIC_VECTOR;
	apic_write(APIC_SPIV, value);

	/*
	 * Set up the virtual wire mode.
	 */
	apic_write(APIC_LVT0, APIC_DM_EXTINT);
	value = APIC_DM_NMI;
	if (!lapic_is_integrated())		/* 82489DX */
		value |= APIC_LVT_LEVEL_TRIGGER;
<<<<<<< HEAD
	apic_write(APIC_LVT1, value);
}

static void __cpuinit lapic_setup_esr(void)
=======
	if (apic_extnmi == APIC_EXTNMI_NONE)
		value |= APIC_LVT_MASKED;
	apic_write(APIC_LVT1, value);
}

static void __init apic_bsp_setup(bool upmode);

/* Init the interrupt delivery mode for the BSP */
void __init apic_intr_mode_init(void)
{
	bool upmode = IS_ENABLED(CONFIG_UP_LATE_INIT);

	switch (apic_intr_mode) {
	case APIC_PIC:
		pr_info("APIC: Keep in PIC mode(8259)\n");
		return;
	case APIC_VIRTUAL_WIRE:
		pr_info("APIC: Switch to virtual wire mode setup\n");
		break;
	case APIC_VIRTUAL_WIRE_NO_CONFIG:
		pr_info("APIC: Switch to virtual wire mode setup with no configuration\n");
		upmode = true;
		break;
	case APIC_SYMMETRIC_IO:
		pr_info("APIC: Switch to symmetric I/O mode setup\n");
		break;
	case APIC_SYMMETRIC_IO_NO_ROUTING:
		pr_info("APIC: Switch to symmetric I/O mode setup in no SMP routine\n");
		break;
	}

	x86_64_probe_apic();

	x86_32_install_bigsmp();

	if (x86_platform.apic_post_init)
		x86_platform.apic_post_init();

	apic_bsp_setup(upmode);
}

static void lapic_setup_esr(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int oldvalue, value, maxlvt;

	if (!lapic_is_integrated()) {
		pr_info("No ESR for 82489DX.\n");
		return;
	}

	if (apic->disable_esr) {
		/*
		 * Something untraceable is creating bad interrupts on
		 * secondary quads ... for the moment, just leave the
		 * ESR disabled - we can't do anything useful with the
		 * errors anyway - mbligh
		 */
		pr_info("Leaving ESR disabled.\n");
		return;
	}

	maxlvt = lapic_get_maxlvt();
	if (maxlvt > 3)		/* Due to the Pentium erratum 3AP. */
		apic_write(APIC_ESR, 0);
	oldvalue = apic_read(APIC_ESR);

	/* enables sending errors */
	value = ERROR_APIC_VECTOR;
	apic_write(APIC_LVTERR, value);

	/*
	 * spec says clear errors after enabling vector.
	 */
	if (maxlvt > 3)
		apic_write(APIC_ESR, 0);
	value = apic_read(APIC_ESR);
	if (value != oldvalue)
		apic_printk(APIC_VERBOSE, "ESR value before enabling "
			"vector: 0x%08x  after: 0x%08x\n",
			oldvalue, value);
}

<<<<<<< HEAD
/**
 * setup_local_APIC - setup the local APIC
 *
 * Used to setup local APIC while initializing BSP or bringin up APs.
 * Always called with preemption disabled.
 */
void __cpuinit setup_local_APIC(void)
{
	int cpu = smp_processor_id();
	unsigned int value, queued;
	int i, j, acked = 0;
	unsigned long long tsc = 0, ntsc;
	long long max_loops = cpu_khz ? cpu_khz : 1000000;

	if (cpu_has_tsc)
		rdtscll(tsc);

	if (disable_apic) {
=======
#define APIC_IR_REGS		APIC_ISR_NR
#define APIC_IR_BITS		(APIC_IR_REGS * 32)
#define APIC_IR_MAPSIZE		(APIC_IR_BITS / BITS_PER_LONG)

union apic_ir {
	unsigned long	map[APIC_IR_MAPSIZE];
	u32		regs[APIC_IR_REGS];
};

static bool apic_check_and_ack(union apic_ir *irr, union apic_ir *isr)
{
	int i, bit;

	/* Read the IRRs */
	for (i = 0; i < APIC_IR_REGS; i++)
		irr->regs[i] = apic_read(APIC_IRR + i * 0x10);

	/* Read the ISRs */
	for (i = 0; i < APIC_IR_REGS; i++)
		isr->regs[i] = apic_read(APIC_ISR + i * 0x10);

	/*
	 * If the ISR map is not empty. ACK the APIC and run another round
	 * to verify whether a pending IRR has been unblocked and turned
	 * into a ISR.
	 */
	if (!bitmap_empty(isr->map, APIC_IR_BITS)) {
		/*
		 * There can be multiple ISR bits set when a high priority
		 * interrupt preempted a lower priority one. Issue an ACK
		 * per set bit.
		 */
		for_each_set_bit(bit, isr->map, APIC_IR_BITS)
			apic_eoi();
		return true;
	}

	return !bitmap_empty(irr->map, APIC_IR_BITS);
}

/*
 * After a crash, we no longer service the interrupts and a pending
 * interrupt from previous kernel might still have ISR bit set.
 *
 * Most probably by now the CPU has serviced that pending interrupt and it
 * might not have done the apic_eoi() because it thought, interrupt
 * came from i8259 as ExtInt. LAPIC did not get EOI so it does not clear
 * the ISR bit and cpu thinks it has already serviced the interrupt. Hence
 * a vector might get locked. It was noticed for timer irq (vector
 * 0x31). Issue an extra EOI to clear ISR.
 *
 * If there are pending IRR bits they turn into ISR bits after a higher
 * priority ISR bit has been acked.
 */
static void apic_pending_intr_clear(void)
{
	union apic_ir irr, isr;
	unsigned int i;

	/* 512 loops are way oversized and give the APIC a chance to obey. */
	for (i = 0; i < 512; i++) {
		if (!apic_check_and_ack(&irr, &isr))
			return;
	}
	/* Dump the IRR/ISR content if that failed */
	pr_warn("APIC: Stale IRR: %256pb ISR: %256pb\n", irr.map, isr.map);
}

/**
 * setup_local_APIC - setup the local APIC
 *
 * Used to setup local APIC while initializing BSP or bringing up APs.
 * Always called with preemption disabled.
 */
static void setup_local_APIC(void)
{
	int cpu = smp_processor_id();
	unsigned int value;

	if (apic_is_disabled) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		disable_ioapic_support();
		return;
	}

<<<<<<< HEAD
=======
	/*
	 * If this comes from kexec/kcrash the APIC might be enabled in
	 * SPIV. Soft disable it before doing further initialization.
	 */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_SPIV_APIC_ENABLED;
	apic_write(APIC_SPIV, value);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_32
	/* Pound the ESR really hard over the head with a big hammer - mbligh */
	if (lapic_is_integrated() && apic->disable_esr) {
		apic_write(APIC_ESR, 0);
		apic_write(APIC_ESR, 0);
		apic_write(APIC_ESR, 0);
		apic_write(APIC_ESR, 0);
	}
#endif
<<<<<<< HEAD
	perf_events_lapic_init();

	/*
	 * Double-check whether this APIC is really registered.
	 * This is meaningless in clustered apic mode, so we skip it.
	 */
	BUG_ON(!apic->apic_id_registered());

	/*
	 * Intel recommends to set DFR, LDR and TPR before enabling
	 * an APIC.  See e.g. "AP-388 82489DX User's Manual" (Intel
	 * document number 292116).  So here it goes...
	 */
	apic->init_apic_ldr();

#ifdef CONFIG_X86_32
	/*
	 * APIC LDR is initialized.  If logical_apicid mapping was
	 * initialized during get_smp_config(), make sure it matches the
	 * actual value.
	 */
	i = early_per_cpu(x86_cpu_to_logical_apicid, cpu);
	WARN_ON(i != BAD_APICID && i != logical_smp_processor_id());
	/* always use the value from LDR */
	early_per_cpu(x86_cpu_to_logical_apicid, cpu) =
		logical_smp_processor_id();

	/*
	 * Some NUMA implementations (NUMAQ) don't initialize apicid to
	 * node mapping during NUMA init.  Now that logical apicid is
	 * guaranteed to be known, give it another chance.  This is already
	 * a bit too late - percpu allocation has already happened without
	 * proper NUMA affinity.
	 */
	if (apic->x86_32_numa_cpu_node)
		set_apicid_to_node(early_per_cpu(x86_cpu_to_apicid, cpu),
				   apic->x86_32_numa_cpu_node(cpu));
#endif

	/*
	 * Set Task Priority to 'accept all'. We never change this
	 * later on.
	 */
	value = apic_read(APIC_TASKPRI);
	value &= ~APIC_TPRI_MASK;
	apic_write(APIC_TASKPRI, value);

	/*
	 * After a crash, we no longer service the interrupts and a pending
	 * interrupt from previous kernel might still have ISR bit set.
	 *
	 * Most probably by now CPU has serviced that pending interrupt and
	 * it might not have done the ack_APIC_irq() because it thought,
	 * interrupt came from i8259 as ExtInt. LAPIC did not get EOI so it
	 * does not clear the ISR bit and cpu thinks it has already serivced
	 * the interrupt. Hence a vector might get locked. It was noticed
	 * for timer irq (vector 0x31). Issue an extra EOI to clear ISR.
	 */
	do {
		queued = 0;
		for (i = APIC_ISR_NR - 1; i >= 0; i--)
			queued |= apic_read(APIC_IRR + i*0x10);

		for (i = APIC_ISR_NR - 1; i >= 0; i--) {
			value = apic_read(APIC_ISR + i*0x10);
			for (j = 31; j >= 0; j--) {
				if (value & (1<<j)) {
					ack_APIC_irq();
					acked++;
				}
			}
		}
		if (acked > 256) {
			printk(KERN_ERR "LAPIC pending interrupts after %d EOI\n",
			       acked);
			break;
		}
		if (cpu_has_tsc && cpu_khz) {
			rdtscll(ntsc);
			max_loops = (cpu_khz << 10) - (ntsc - tsc);
		} else
			max_loops--;
	} while (queued && max_loops > 0);
	WARN_ON(max_loops <= 0);
=======
	/*
	 * Intel recommends to set DFR, LDR and TPR before enabling
	 * an APIC.  See e.g. "AP-388 82489DX User's Manual" (Intel
	 * document number 292116).
	 *
	 * Except for APICs which operate in physical destination mode.
	 */
	if (apic->init_apic_ldr)
		apic->init_apic_ldr();

	/*
	 * Set Task Priority to 'accept all except vectors 0-31'.  An APIC
	 * vector in the 16-31 range could be delivered if TPR == 0, but we
	 * would think it's an exception and terrible things will happen.  We
	 * never change this later on.
	 */
	value = apic_read(APIC_TASKPRI);
	value &= ~APIC_TPRI_MASK;
	value |= 0x10;
	apic_write(APIC_TASKPRI, value);

	/* Clear eventually stale ISR/IRR bits */
	apic_pending_intr_clear();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Now that we are all set up, enable the APIC
	 */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_VECTOR_MASK;
	/*
	 * Enable APIC
	 */
	value |= APIC_SPIV_APIC_ENABLED;

#ifdef CONFIG_X86_32
	/*
	 * Some unknown Intel IO/APIC (or APIC) errata is biting us with
	 * certain networking cards. If high frequency interrupts are
	 * happening on a particular IOAPIC pin, plus the IOAPIC routing
	 * entry is masked/unmasked at a high rate as well then sooner or
	 * later IOAPIC line gets 'stuck', no more interrupts are received
	 * from the device. If focus CPU is disabled then the hang goes
	 * away, oh well :-(
	 *
	 * [ This bug can be reproduced easily with a level-triggered
	 *   PCI Ne2000 networking cards and PII/PIII processors, dual
	 *   BX chipset. ]
	 */
	/*
	 * Actually disabling the focus CPU check just makes the hang less
<<<<<<< HEAD
	 * frequent as it makes the interrupt distributon model be more
	 * like LRU than MRU (the short-term load is more even across CPUs).
	 * See also the comment in end_level_ioapic_irq().  --macro
=======
	 * frequent as it makes the interrupt distribution model be more
	 * like LRU than MRU (the short-term load is more even across CPUs).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */

	/*
	 * - enable focus processor (bit==0)
	 * - 64bit mode always use processor focus
	 *   so no need to set it
	 */
	value &= ~APIC_SPIV_FOCUS_DISABLED;
#endif

	/*
	 * Set spurious IRQ vector
	 */
	value |= SPURIOUS_APIC_VECTOR;
	apic_write(APIC_SPIV, value);

<<<<<<< HEAD
	/*
	 * Set up LVT0, LVT1:
	 *
	 * set up through-local-APIC on the BP's LINT0. This is not
=======
	perf_events_lapic_init();

	/*
	 * Set up LVT0, LVT1:
	 *
	 * set up through-local-APIC on the boot CPU's LINT0. This is not
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * strictly necessary in pure symmetric-IO mode, but sometimes
	 * we delegate interrupts to the 8259A.
	 */
	/*
	 * TODO: set up through-local-APIC from through-I/O-APIC? --macro
	 */
	value = apic_read(APIC_LVT0) & APIC_LVT_MASKED;
<<<<<<< HEAD
	if (!cpu && (pic_mode || !value)) {
=======
	if (!cpu && (pic_mode || !value || ioapic_is_disabled)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		value = APIC_DM_EXTINT;
		apic_printk(APIC_VERBOSE, "enabled ExtINT on CPU#%d\n", cpu);
	} else {
		value = APIC_DM_EXTINT | APIC_LVT_MASKED;
		apic_printk(APIC_VERBOSE, "masked ExtINT on CPU#%d\n", cpu);
	}
	apic_write(APIC_LVT0, value);

	/*
<<<<<<< HEAD
	 * only the BP should see the LINT1 NMI signal, obviously.
	 */
	if (!cpu)
		value = APIC_DM_NMI;
	else
		value = APIC_DM_NMI | APIC_LVT_MASKED;
	if (!lapic_is_integrated())		/* 82489DX */
=======
	 * Only the BSP sees the LINT1 NMI signal by default. This can be
	 * modified by apic_extnmi= boot option.
	 */
	if ((!cpu && apic_extnmi != APIC_EXTNMI_NONE) ||
	    apic_extnmi == APIC_EXTNMI_ALL)
		value = APIC_DM_NMI;
	else
		value = APIC_DM_NMI | APIC_LVT_MASKED;

	/* Is 82489DX ? */
	if (!lapic_is_integrated())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		value |= APIC_LVT_LEVEL_TRIGGER;
	apic_write(APIC_LVT1, value);

#ifdef CONFIG_X86_MCE_INTEL
	/* Recheck CMCI information after local APIC is up on CPU #0 */
	if (!cpu)
		cmci_recheck();
#endif
}

<<<<<<< HEAD
void __cpuinit end_local_APIC_setup(void)
=======
static void end_local_APIC_setup(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	lapic_setup_esr();

#ifdef CONFIG_X86_32
	{
		unsigned int value;
		/* Disable the local apic timer */
		value = apic_read(APIC_LVTT);
		value |= (APIC_LVT_MASKED | LOCAL_TIMER_VECTOR);
		apic_write(APIC_LVTT, value);
	}
#endif

	apic_pm_activate();
}

<<<<<<< HEAD
void __init bsp_end_local_APIC_setup(void)
{
	end_local_APIC_setup();

	/*
	 * Now that local APIC setup is completed for BP, configure the fault
	 * handling for interrupt remapping.
	 */
	if (intr_remapping_enabled)
		enable_drhd_fault_handling();

}

#ifdef CONFIG_X86_X2APIC
/*
 * Need to disable xapic and x2apic at the same time and then enable xapic mode
 */
static inline void __disable_x2apic(u64 msr)
{
	wrmsrl(MSR_IA32_APICBASE,
	       msr & ~(X2APIC_ENABLE | XAPIC_ENABLE));
	wrmsrl(MSR_IA32_APICBASE, msr & ~X2APIC_ENABLE);
}

static __init void disable_x2apic(void)
{
	u64 msr;

	if (!cpu_has_x2apic)
		return;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (msr & X2APIC_ENABLE) {
		u32 x2apic_id = read_apic_id();

		if (x2apic_id >= 255)
			panic("Cannot disable x2apic, id: %08x\n", x2apic_id);

		pr_info("Disabling x2apic\n");
		__disable_x2apic(msr);

		if (nox2apic) {
			clear_cpu_cap(&cpu_data(0), X86_FEATURE_X2APIC);
			setup_clear_cpu_cap(X86_FEATURE_X2APIC);
		}

		x2apic_disabled = 1;
		x2apic_mode = 0;

		register_lapic_address(mp_lapic_addr);
	}
}

void check_x2apic(void)
{
	if (x2apic_enabled()) {
		pr_info("x2apic enabled by BIOS, switching to x2apic ops\n");
		x2apic_preenabled = x2apic_mode = 1;
	}
}

void enable_x2apic(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (x2apic_disabled) {
		__disable_x2apic(msr);
		return;
	}

	if (!x2apic_mode)
		return;

	if (!(msr & X2APIC_ENABLE)) {
		printk_once(KERN_INFO "Enabling x2apic\n");
		wrmsrl(MSR_IA32_APICBASE, msr | X2APIC_ENABLE);
	}
}
#endif /* CONFIG_X86_X2APIC */

int __init enable_IR(void)
{
#ifdef CONFIG_IRQ_REMAP
	if (!intr_remapping_supported()) {
		pr_debug("intr-remapping not supported\n");
		return -1;
	}

	if (!x2apic_preenabled && skip_ioapic_setup) {
		pr_info("Skipped enabling intr-remap because of skipping "
			"io-apic setup\n");
		return -1;
	}

	return enable_intr_remapping();
#endif
	return -1;
}

void __init enable_IR_x2apic(void)
{
	unsigned long flags;
	int ret, x2apic_enabled = 0;
	int dmar_table_init_ret;

	dmar_table_init_ret = dmar_table_init();
	if (dmar_table_init_ret && !x2apic_supported())
=======
/*
 * APIC setup function for application processors. Called from smpboot.c
 */
void apic_ap_setup(void)
{
	setup_local_APIC();
	end_local_APIC_setup();
}

static __init void apic_read_boot_cpu_id(bool x2apic)
{
	/*
	 * This can be invoked from check_x2apic() before the APIC has been
	 * selected. But that code knows for sure that the BIOS enabled
	 * X2APIC.
	 */
	if (x2apic) {
		boot_cpu_physical_apicid = native_apic_msr_read(APIC_ID);
		boot_cpu_apic_version = GET_APIC_VERSION(native_apic_msr_read(APIC_LVR));
	} else {
		boot_cpu_physical_apicid = read_apic_id();
		boot_cpu_apic_version = GET_APIC_VERSION(apic_read(APIC_LVR));
	}
	topology_register_boot_apic(boot_cpu_physical_apicid);
	x86_32_probe_bigsmp_early();
}

#ifdef CONFIG_X86_X2APIC
int x2apic_mode;
EXPORT_SYMBOL_GPL(x2apic_mode);

enum {
	X2APIC_OFF,
	X2APIC_DISABLED,
	/* All states below here have X2APIC enabled */
	X2APIC_ON,
	X2APIC_ON_LOCKED
};
static int x2apic_state;

static bool x2apic_hw_locked(void)
{
	u64 x86_arch_cap_msr;
	u64 msr;

	x86_arch_cap_msr = x86_read_arch_cap_msr();
	if (x86_arch_cap_msr & ARCH_CAP_XAPIC_DISABLE) {
		rdmsrl(MSR_IA32_XAPIC_DISABLE_STATUS, msr);
		return (msr & LEGACY_XAPIC_DISABLED);
	}
	return false;
}

static void __x2apic_disable(void)
{
	u64 msr;

	if (!boot_cpu_has(X86_FEATURE_APIC))
		return;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (!(msr & X2APIC_ENABLE))
		return;
	/* Disable xapic and x2apic first and then reenable xapic mode */
	wrmsrl(MSR_IA32_APICBASE, msr & ~(X2APIC_ENABLE | XAPIC_ENABLE));
	wrmsrl(MSR_IA32_APICBASE, msr & ~X2APIC_ENABLE);
	printk_once(KERN_INFO "x2apic disabled\n");
}

static void __x2apic_enable(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, msr);
	if (msr & X2APIC_ENABLE)
		return;
	wrmsrl(MSR_IA32_APICBASE, msr | X2APIC_ENABLE);
	printk_once(KERN_INFO "x2apic enabled\n");
}

static int __init setup_nox2apic(char *str)
{
	if (x2apic_enabled()) {
		u32 apicid = native_apic_msr_read(APIC_ID);

		if (apicid >= 255) {
			pr_warn("Apicid: %08x, cannot enforce nox2apic\n",
				apicid);
			return 0;
		}
		if (x2apic_hw_locked()) {
			pr_warn("APIC locked in x2apic mode, can't disable\n");
			return 0;
		}
		pr_warn("x2apic already enabled.\n");
		__x2apic_disable();
	}
	setup_clear_cpu_cap(X86_FEATURE_X2APIC);
	x2apic_state = X2APIC_DISABLED;
	x2apic_mode = 0;
	return 0;
}
early_param("nox2apic", setup_nox2apic);

/* Called from cpu_init() to enable x2apic on (secondary) cpus */
void x2apic_setup(void)
{
	/*
	 * Try to make the AP's APIC state match that of the BSP,  but if the
	 * BSP is unlocked and the AP is locked then there is a state mismatch.
	 * Warn about the mismatch in case a GP fault occurs due to a locked AP
	 * trying to be turned off.
	 */
	if (x2apic_state != X2APIC_ON_LOCKED && x2apic_hw_locked())
		pr_warn("x2apic lock mismatch between BSP and AP.\n");
	/*
	 * If x2apic is not in ON or LOCKED state, disable it if already enabled
	 * from BIOS.
	 */
	if (x2apic_state < X2APIC_ON) {
		__x2apic_disable();
		return;
	}
	__x2apic_enable();
}

static __init void apic_set_fixmap(bool read_apic);

static __init void x2apic_disable(void)
{
	u32 x2apic_id, state = x2apic_state;

	x2apic_mode = 0;
	x2apic_state = X2APIC_DISABLED;

	if (state != X2APIC_ON)
		return;

	x2apic_id = read_apic_id();
	if (x2apic_id >= 255)
		panic("Cannot disable x2apic, id: %08x\n", x2apic_id);

	if (x2apic_hw_locked()) {
		pr_warn("Cannot disable locked x2apic, id: %08x\n", x2apic_id);
		return;
	}

	__x2apic_disable();
	/*
	 * Don't reread the APIC ID as it was already done from
	 * check_x2apic() and the APIC driver still is a x2APIC variant,
	 * which fails to do the read after x2APIC was disabled.
	 */
	apic_set_fixmap(false);
}

static __init void x2apic_enable(void)
{
	if (x2apic_state != X2APIC_OFF)
		return;

	x2apic_mode = 1;
	x2apic_state = X2APIC_ON;
	__x2apic_enable();
}

static __init void try_to_enable_x2apic(int remap_mode)
{
	if (x2apic_state == X2APIC_DISABLED)
		return;

	if (remap_mode != IRQ_REMAP_X2APIC_MODE) {
		u32 apic_limit = 255;

		/*
		 * Using X2APIC without IR is not architecturally supported
		 * on bare metal but may be supported in guests.
		 */
		if (!x86_init.hyper.x2apic_available()) {
			pr_info("x2apic: IRQ remapping doesn't support X2APIC mode\n");
			x2apic_disable();
			return;
		}

		/*
		 * If the hypervisor supports extended destination ID in
		 * MSI, that increases the maximum APIC ID that can be
		 * used for non-remapped IRQ domains.
		 */
		if (x86_init.hyper.msi_ext_dest_id()) {
			virt_ext_dest_id = 1;
			apic_limit = 32767;
		}

		/*
		 * Without IR, all CPUs can be addressed by IOAPIC/MSI only
		 * in physical mode, and CPUs with an APIC ID that cannot
		 * be addressed must not be brought online.
		 */
		x2apic_set_max_apicid(apic_limit);
		x2apic_phys = 1;
	}
	x2apic_enable();
}

void __init check_x2apic(void)
{
	if (x2apic_enabled()) {
		pr_info("x2apic: enabled by BIOS, switching to x2apic ops\n");
		x2apic_mode = 1;
		if (x2apic_hw_locked())
			x2apic_state = X2APIC_ON_LOCKED;
		else
			x2apic_state = X2APIC_ON;
		apic_read_boot_cpu_id(true);
	} else if (!boot_cpu_has(X86_FEATURE_X2APIC)) {
		x2apic_state = X2APIC_DISABLED;
	}
}
#else /* CONFIG_X86_X2APIC */
void __init check_x2apic(void)
{
	if (!apic_is_x2apic_enabled())
		return;
	/*
	 * Checkme: Can we simply turn off x2APIC here instead of disabling the APIC?
	 */
	pr_err("Kernel does not support x2APIC, please recompile with CONFIG_X86_X2APIC.\n");
	pr_err("Disabling APIC, expect reduced performance and functionality.\n");

	apic_is_disabled = true;
	setup_clear_cpu_cap(X86_FEATURE_APIC);
}

static inline void try_to_enable_x2apic(int remap_mode) { }
static inline void __x2apic_enable(void) { }
#endif /* !CONFIG_X86_X2APIC */

void __init enable_IR_x2apic(void)
{
	unsigned long flags;
	int ret, ir_stat;

	if (ioapic_is_disabled) {
		pr_info("Not enabling interrupt remapping due to skipped IO-APIC setup\n");
		return;
	}

	ir_stat = irq_remapping_prepare();
	if (ir_stat < 0 && !x2apic_supported())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	ret = save_ioapic_entries();
	if (ret) {
		pr_info("Saving IO-APIC state failed: %d\n", ret);
		return;
	}

	local_irq_save(flags);
	legacy_pic->mask_all();
	mask_ioapic_entries();

<<<<<<< HEAD
	if (x2apic_preenabled && nox2apic)
		disable_x2apic();

	if (dmar_table_init_ret)
		ret = -1;
	else
		ret = enable_IR();

	if (!x2apic_supported())
		goto skip_x2apic;

	if (ret < 0) {
		/* IR is required if there is APIC ID > 255 even when running
		 * under KVM
		 */
		if (max_physical_apicid > 255 ||
		    !hypervisor_x2apic_available()) {
			if (x2apic_preenabled)
				disable_x2apic();
			goto skip_x2apic;
		}
		/*
		 * without IR all CPUs can be addressed by IOAPIC/MSI
		 * only in physical mode
		 */
		x2apic_force_phys();
	}

	if (ret == IRQ_REMAP_XAPIC_MODE) {
		pr_info("x2apic not enabled, IRQ remapping is in xapic mode\n");
		goto skip_x2apic;
	}

	x2apic_enabled = 1;

	if (x2apic_supported() && !x2apic_mode) {
		x2apic_mode = 1;
		enable_x2apic();
		pr_info("Enabled x2apic\n");
	}

skip_x2apic:
	if (ret < 0) /* IR enabling failed */
=======
	/* If irq_remapping_prepare() succeeded, try to enable it */
	if (ir_stat >= 0)
		ir_stat = irq_remapping_enable();
	/* ir_stat contains the remap mode or an error code */
	try_to_enable_x2apic(ir_stat);

	if (ir_stat < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		restore_ioapic_entries();
	legacy_pic->restore_mask();
	local_irq_restore(flags);
}

#ifdef CONFIG_X86_64
/*
 * Detect and enable local APICs on non-SMP boards.
 * Original code written by Keir Fraser.
 * On AMD64 we trust the BIOS - if it says no APIC it is likely
 * not correctly set up (usually the APIC timer won't work etc.)
 */
<<<<<<< HEAD
static int __init detect_init_APIC(void)
{
	if (!cpu_has_apic) {
		pr_info("No local APIC present\n");
		return -1;
	}

	mp_lapic_addr = APIC_DEFAULT_PHYS_BASE;
	return 0;
}
#else

static int __init apic_verify(void)
=======
static bool __init detect_init_APIC(void)
{
	if (!boot_cpu_has(X86_FEATURE_APIC)) {
		pr_info("No local APIC present\n");
		return false;
	}

	register_lapic_address(APIC_DEFAULT_PHYS_BASE);
	return true;
}
#else

static bool __init apic_verify(unsigned long addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 features, h, l;

	/*
	 * The APIC feature bit should now be enabled
	 * in `cpuid'
	 */
	features = cpuid_edx(1);
	if (!(features & (1 << X86_FEATURE_APIC))) {
<<<<<<< HEAD
		pr_warning("Could not enable APIC!\n");
		return -1;
	}
	set_cpu_cap(&boot_cpu_data, X86_FEATURE_APIC);
	mp_lapic_addr = APIC_DEFAULT_PHYS_BASE;
=======
		pr_warn("Could not enable APIC!\n");
		return false;
	}
	set_cpu_cap(&boot_cpu_data, X86_FEATURE_APIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The BIOS may have set up the APIC at some other address */
	if (boot_cpu_data.x86 >= 6) {
		rdmsr(MSR_IA32_APICBASE, l, h);
		if (l & MSR_IA32_APICBASE_ENABLE)
<<<<<<< HEAD
			mp_lapic_addr = l & MSR_IA32_APICBASE_BASE;
	}

	pr_info("Found and enabled local APIC!\n");
	return 0;
}

int __init apic_force_enable(unsigned long addr)
{
	u32 h, l;

	if (disable_apic)
		return -1;
=======
			addr = l & MSR_IA32_APICBASE_BASE;
	}

	register_lapic_address(addr);
	pr_info("Found and enabled local APIC!\n");
	return true;
}

bool __init apic_force_enable(unsigned long addr)
{
	u32 h, l;

	if (apic_is_disabled)
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Some BIOSes disable the local APIC in the APIC_BASE
	 * MSR. This can only be done in software for Intel P6 or later
	 * and AMD K7 (Model > 1) or later.
	 */
	if (boot_cpu_data.x86 >= 6) {
		rdmsr(MSR_IA32_APICBASE, l, h);
		if (!(l & MSR_IA32_APICBASE_ENABLE)) {
			pr_info("Local APIC disabled by BIOS -- reenabling.\n");
			l &= ~MSR_IA32_APICBASE_BASE;
			l |= MSR_IA32_APICBASE_ENABLE | addr;
			wrmsr(MSR_IA32_APICBASE, l, h);
			enabled_via_apicbase = 1;
		}
	}
<<<<<<< HEAD
	return apic_verify();
=======
	return apic_verify(addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Detect and initialize APIC
 */
<<<<<<< HEAD
static int __init detect_init_APIC(void)
{
	/* Disabled by kernel option? */
	if (disable_apic)
		return -1;
=======
static bool __init detect_init_APIC(void)
{
	/* Disabled by kernel option? */
	if (apic_is_disabled)
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (boot_cpu_data.x86_vendor) {
	case X86_VENDOR_AMD:
		if ((boot_cpu_data.x86 == 6 && boot_cpu_data.x86_model > 1) ||
		    (boot_cpu_data.x86 >= 15))
			break;
		goto no_apic;
<<<<<<< HEAD
	case X86_VENDOR_INTEL:
		if (boot_cpu_data.x86 == 6 || boot_cpu_data.x86 == 15 ||
		    (boot_cpu_data.x86 == 5 && cpu_has_apic))
=======
	case X86_VENDOR_HYGON:
		break;
	case X86_VENDOR_INTEL:
		if (boot_cpu_data.x86 == 6 || boot_cpu_data.x86 == 15 ||
		    (boot_cpu_data.x86 == 5 && boot_cpu_has(X86_FEATURE_APIC)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		goto no_apic;
	default:
		goto no_apic;
	}

<<<<<<< HEAD
	if (!cpu_has_apic) {
=======
	if (!boot_cpu_has(X86_FEATURE_APIC)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Over-ride BIOS and try to enable the local APIC only if
		 * "lapic" specified.
		 */
		if (!force_enable_local_apic) {
			pr_info("Local APIC disabled by BIOS -- "
				"you can enable it with \"lapic\"\n");
<<<<<<< HEAD
			return -1;
		}
		if (apic_force_enable(APIC_DEFAULT_PHYS_BASE))
			return -1;
	} else {
		if (apic_verify())
			return -1;
=======
			return false;
		}
		if (!apic_force_enable(APIC_DEFAULT_PHYS_BASE))
			return false;
	} else {
		if (!apic_verify(APIC_DEFAULT_PHYS_BASE))
			return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	apic_pm_activate();

<<<<<<< HEAD
	return 0;

no_apic:
	pr_info("No local APIC present or hardware disabled\n");
	return -1;
=======
	return true;

no_apic:
	pr_info("No local APIC present or hardware disabled\n");
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

/**
 * init_apic_mappings - initialize APIC mappings
 */
void __init init_apic_mappings(void)
{
<<<<<<< HEAD
	unsigned int new_apicid;

	if (x2apic_mode) {
		boot_cpu_physical_apicid = read_apic_id();
		return;
	}

	/* If no local APIC can be found return early */
	if (!smp_found_config && detect_init_APIC()) {
		/* lets NOP'ify apic operations */
		pr_info("APIC: disable apic facility\n");
		apic_disable();
	} else {
		apic_phys = mp_lapic_addr;

		/*
		 * acpi lapic path already maps that address in
		 * acpi_register_lapic_address()
		 */
		if (!acpi_lapic && !smp_found_config)
			register_lapic_address(apic_phys);
	}

	/*
	 * Fetch the APIC ID of the BSP in case we have a
	 * default configuration (or the MP table is broken).
	 */
	new_apicid = read_apic_id();
	if (boot_cpu_physical_apicid != new_apicid) {
		boot_cpu_physical_apicid = new_apicid;
		/*
		 * yeah -- we lie about apic_version
		 * in case if apic was disabled via boot option
		 * but it's not a problem for SMP compiled kernel
		 * since smp_sanity_check is prepared for such a case
		 * and disable smp mode
		 */
		apic_version[new_apicid] =
			 GET_APIC_VERSION(apic_read(APIC_LVR));
	}
=======
	if (apic_validate_deadline_timer())
		pr_info("TSC deadline timer available\n");

	if (x2apic_mode)
		return;

	if (!smp_found_config) {
		if (!detect_init_APIC()) {
			pr_info("APIC: disable apic facility\n");
			apic_disable();
		}
	}
}

static __init void apic_set_fixmap(bool read_apic)
{
	set_fixmap_nocache(FIX_APIC_BASE, mp_lapic_addr);
	apic_mmio_base = APIC_BASE;
	apic_printk(APIC_VERBOSE, "mapped APIC to %16lx (%16lx)\n",
		    apic_mmio_base, mp_lapic_addr);
	if (read_apic)
		apic_read_boot_cpu_id(false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init register_lapic_address(unsigned long address)
{
<<<<<<< HEAD
	mp_lapic_addr = address;

	if (!x2apic_mode) {
		set_fixmap_nocache(FIX_APIC_BASE, address);
		apic_printk(APIC_VERBOSE, "mapped APIC to %16lx (%16lx)\n",
			    APIC_BASE, mp_lapic_addr);
	}
	if (boot_cpu_physical_apicid == -1U) {
		boot_cpu_physical_apicid  = read_apic_id();
		apic_version[boot_cpu_physical_apicid] =
			 GET_APIC_VERSION(apic_read(APIC_LVR));
	}
}

/*
 * This initializes the IO-APIC and APIC hardware if this is
 * a UP kernel.
 */
int apic_version[MAX_LOCAL_APIC];

int __init APIC_init_uniprocessor(void)
{
	if (disable_apic) {
		pr_info("Apic disabled\n");
		return -1;
	}
#ifdef CONFIG_X86_64
	if (!cpu_has_apic) {
		disable_apic = 1;
		pr_info("Apic disabled by BIOS\n");
		return -1;
	}
#else
	if (!smp_found_config && !cpu_has_apic)
		return -1;

	/*
	 * Complain if the BIOS pretends there is one.
	 */
	if (!cpu_has_apic &&
	    APIC_INTEGRATED(apic_version[boot_cpu_physical_apicid])) {
		pr_err("BIOS bug, local APIC 0x%x not detected!...\n",
			boot_cpu_physical_apicid);
		return -1;
	}
#endif

	default_setup_apic_routing();

	verify_local_APIC();
	connect_bsp_APIC();

#ifdef CONFIG_X86_64
	apic_write(APIC_ID, SET_APIC_ID(boot_cpu_physical_apicid));
#else
	/*
	 * Hack: In case of kdump, after a crash, kernel might be booting
	 * on a cpu with non-zero lapic id. But boot_cpu_physical_apicid
	 * might be zero if read from MP tables. Get it from LAPIC.
	 */
# ifdef CONFIG_CRASH_DUMP
	boot_cpu_physical_apicid = read_apic_id();
# endif
#endif
	physid_set_mask_of_physid(boot_cpu_physical_apicid, &phys_cpu_present_map);
	setup_local_APIC();

#ifdef CONFIG_X86_IO_APIC
	/*
	 * Now enable IO-APICs, actually call clear_IO_APIC
	 * We need clear_IO_APIC before enabling error vector
	 */
	if (!skip_ioapic_setup && nr_ioapics)
		enable_IO_APIC();
#endif

	bsp_end_local_APIC_setup();

#ifdef CONFIG_X86_IO_APIC
	if (smp_found_config && !skip_ioapic_setup && nr_ioapics)
		setup_IO_APIC();
	else {
		nr_ioapics = 0;
	}
#endif

	x86_init.timers.setup_percpu_clockev();
	return 0;
=======
	/* This should only happen once */
	WARN_ON_ONCE(mp_lapic_addr);
	mp_lapic_addr = address;

	if (!x2apic_mode)
		apic_set_fixmap(true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Local APIC interrupts
 */

/*
<<<<<<< HEAD
 * This interrupt should _never_ happen with our APIC/SMP architecture
 */
void smp_spurious_interrupt(struct pt_regs *regs)
{
	u32 v;

	irq_enter();
	exit_idle();
	/*
	 * Check if this really is a spurious interrupt and ACK it
	 * if it is a vectored one.  Just in case...
	 * Spurious interrupts should not be ACKed.
	 */
	v = apic_read(APIC_ISR + ((SPURIOUS_APIC_VECTOR & ~0x1f) >> 1));
	if (v & (1 << (SPURIOUS_APIC_VECTOR & 0x1f)))
		ack_APIC_irq();

	inc_irq_stat(irq_spurious_count);

	/* see sw-dev-man vol 3, chapter 7.4.13.5 */
	pr_info("spurious APIC interrupt on CPU#%d, "
		"should never happen.\n", smp_processor_id());
	irq_exit();
=======
 * Common handling code for spurious_interrupt and spurious_vector entry
 * points below. No point in allowing the compiler to inline it twice.
 */
static noinline void handle_spurious_interrupt(u8 vector)
{
	u32 v;

	trace_spurious_apic_entry(vector);

	inc_irq_stat(irq_spurious_count);

	/*
	 * If this is a spurious interrupt then do not acknowledge
	 */
	if (vector == SPURIOUS_APIC_VECTOR) {
		/* See SDM vol 3 */
		pr_info("Spurious APIC interrupt (vector 0xFF) on CPU#%d, should never happen.\n",
			smp_processor_id());
		goto out;
	}

	/*
	 * If it is a vectored one, verify it's set in the ISR. If set,
	 * acknowledge it.
	 */
	v = apic_read(APIC_ISR + ((vector & ~0x1f) >> 1));
	if (v & (1 << (vector & 0x1f))) {
		pr_info("Spurious interrupt (vector 0x%02x) on CPU#%d. Acked\n",
			vector, smp_processor_id());
		apic_eoi();
	} else {
		pr_info("Spurious interrupt (vector 0x%02x) on CPU#%d. Not pending!\n",
			vector, smp_processor_id());
	}
out:
	trace_spurious_apic_exit(vector);
}

/**
 * spurious_interrupt - Catch all for interrupts raised on unused vectors
 * @regs:	Pointer to pt_regs on stack
 * @vector:	The vector number
 *
 * This is invoked from ASM entry code to catch all interrupts which
 * trigger on an entry which is routed to the common_spurious idtentry
 * point.
 */
DEFINE_IDTENTRY_IRQ(spurious_interrupt)
{
	handle_spurious_interrupt(vector);
}

DEFINE_IDTENTRY_SYSVEC(sysvec_spurious_apic_interrupt)
{
	handle_spurious_interrupt(SPURIOUS_APIC_VECTOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This interrupt should never happen with our APIC/SMP architecture
 */
<<<<<<< HEAD
void smp_error_interrupt(struct pt_regs *regs)
{
	u32 v0, v1;
	u32 i = 0;
=======
DEFINE_IDTENTRY_SYSVEC(sysvec_error_interrupt)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	static const char * const error_interrupt_reason[] = {
		"Send CS error",		/* APIC Error Bit 0 */
		"Receive CS error",		/* APIC Error Bit 1 */
		"Send accept error",		/* APIC Error Bit 2 */
		"Receive accept error",		/* APIC Error Bit 3 */
		"Redirectable IPI",		/* APIC Error Bit 4 */
		"Send illegal vector",		/* APIC Error Bit 5 */
		"Received illegal vector",	/* APIC Error Bit 6 */
		"Illegal register address",	/* APIC Error Bit 7 */
	};
<<<<<<< HEAD

	irq_enter();
	exit_idle();
	/* First tickle the hardware, only then report what went on. -- REW */
	v0 = apic_read(APIC_ESR);
	apic_write(APIC_ESR, 0);
	v1 = apic_read(APIC_ESR);
	ack_APIC_irq();
	atomic_inc(&irq_err_count);

	apic_printk(APIC_DEBUG, KERN_DEBUG "APIC error on CPU%d: %02x(%02x)",
		    smp_processor_id(), v0 , v1);

	v1 = v1 & 0xff;
	while (v1) {
		if (v1 & 0x1)
			apic_printk(APIC_DEBUG, KERN_CONT " : %s", error_interrupt_reason[i]);
		i++;
		v1 >>= 1;
	};

	apic_printk(APIC_DEBUG, KERN_CONT "\n");

	irq_exit();
=======
	u32 v, i = 0;

	trace_error_apic_entry(ERROR_APIC_VECTOR);

	/* First tickle the hardware, only then report what went on. -- REW */
	if (lapic_get_maxlvt() > 3)	/* Due to the Pentium erratum 3AP. */
		apic_write(APIC_ESR, 0);
	v = apic_read(APIC_ESR);
	apic_eoi();
	atomic_inc(&irq_err_count);

	apic_printk(APIC_DEBUG, KERN_DEBUG "APIC error on CPU%d: %02x",
		    smp_processor_id(), v);

	v &= 0xff;
	while (v) {
		if (v & 0x1)
			apic_printk(APIC_DEBUG, KERN_CONT " : %s", error_interrupt_reason[i]);
		i++;
		v >>= 1;
	}

	apic_printk(APIC_DEBUG, KERN_CONT "\n");

	trace_error_apic_exit(ERROR_APIC_VECTOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * connect_bsp_APIC - attach the APIC to the interrupt system
 */
<<<<<<< HEAD
void __init connect_bsp_APIC(void)
=======
static void __init connect_bsp_APIC(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#ifdef CONFIG_X86_32
	if (pic_mode) {
		/*
		 * Do not trust the local APIC being empty at bootup.
		 */
		clear_local_APIC();
		/*
		 * PIC mode, enable APIC mode in the IMCR, i.e.  connect BSP's
		 * local APIC to INT and NMI lines.
		 */
		apic_printk(APIC_VERBOSE, "leaving PIC mode, "
				"enabling APIC mode.\n");
		imcr_pic_to_apic();
	}
#endif
<<<<<<< HEAD
	if (apic->enable_apic_mode)
		apic->enable_apic_mode();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * disconnect_bsp_APIC - detach the APIC from the interrupt system
 * @virt_wire_setup:	indicates, whether virtual wire mode is selected
 *
 * Virtual wire mode is necessary to deliver legacy interrupts even when the
 * APIC is disabled.
 */
void disconnect_bsp_APIC(int virt_wire_setup)
{
	unsigned int value;

#ifdef CONFIG_X86_32
	if (pic_mode) {
		/*
		 * Put the board back into PIC mode (has an effect only on
		 * certain older boards).  Note that APIC interrupts, including
		 * IPIs, won't work beyond this point!  The only exception are
		 * INIT IPIs.
		 */
		apic_printk(APIC_VERBOSE, "disabling APIC mode, "
				"entering PIC mode.\n");
		imcr_apic_to_pic();
		return;
	}
#endif

	/* Go back to Virtual Wire compatibility mode */

	/* For the spurious interrupt use vector F, and enable it */
	value = apic_read(APIC_SPIV);
	value &= ~APIC_VECTOR_MASK;
	value |= APIC_SPIV_APIC_ENABLED;
	value |= 0xf;
	apic_write(APIC_SPIV, value);

	if (!virt_wire_setup) {
		/*
		 * For LVT0 make it edge triggered, active high,
		 * external and enabled
		 */
		value = apic_read(APIC_LVT0);
		value &= ~(APIC_MODE_MASK | APIC_SEND_PENDING |
			APIC_INPUT_POLARITY | APIC_LVT_REMOTE_IRR |
			APIC_LVT_LEVEL_TRIGGER | APIC_LVT_MASKED);
		value |= APIC_LVT_REMOTE_IRR | APIC_SEND_PENDING;
		value = SET_APIC_DELIVERY_MODE(value, APIC_MODE_EXTINT);
		apic_write(APIC_LVT0, value);
	} else {
		/* Disable LVT0 */
		apic_write(APIC_LVT0, APIC_LVT_MASKED);
	}

	/*
	 * For LVT1 make it edge triggered, active high,
	 * nmi and enabled
	 */
	value = apic_read(APIC_LVT1);
	value &= ~(APIC_MODE_MASK | APIC_SEND_PENDING |
			APIC_INPUT_POLARITY | APIC_LVT_REMOTE_IRR |
			APIC_LVT_LEVEL_TRIGGER | APIC_LVT_MASKED);
	value |= APIC_LVT_REMOTE_IRR | APIC_SEND_PENDING;
	value = SET_APIC_DELIVERY_MODE(value, APIC_MODE_NMI);
	apic_write(APIC_LVT1, value);
}

<<<<<<< HEAD
void __cpuinit generic_processor_info(int apicid, int version)
{
	int cpu, max = nr_cpu_ids;
	bool boot_cpu_detected = physid_isset(boot_cpu_physical_apicid,
				phys_cpu_present_map);

	/*
	 * If boot cpu has not been detected yet, then only allow upto
	 * nr_cpu_ids - 1 processors and keep one slot free for boot cpu
	 */
	if (!boot_cpu_detected && num_processors >= nr_cpu_ids - 1 &&
	    apicid != boot_cpu_physical_apicid) {
		int thiscpu = max + disabled_cpus - 1;

		pr_warning(
			"ACPI: NR_CPUS/possible_cpus limit of %i almost"
			" reached. Keeping one slot for boot cpu."
			"  Processor %d/0x%x ignored.\n", max, thiscpu, apicid);

		disabled_cpus++;
		return;
	}

	if (num_processors >= nr_cpu_ids) {
		int thiscpu = max + disabled_cpus;

		pr_warning(
			"ACPI: NR_CPUS/possible_cpus limit of %i reached."
			"  Processor %d/0x%x ignored.\n", max, thiscpu, apicid);

		disabled_cpus++;
		return;
	}

	num_processors++;
	if (apicid == boot_cpu_physical_apicid) {
		/*
		 * x86_bios_cpu_apicid is required to have processors listed
		 * in same order as logical cpu numbers. Hence the first
		 * entry is BSP, and so on.
		 * boot_cpu_init() already hold bit 0 in cpu_present_mask
		 * for BSP.
		 */
		cpu = 0;
	} else
		cpu = cpumask_next_zero(-1, cpu_present_mask);

	/*
	 * Validate version
	 */
	if (version == 0x0) {
		pr_warning("BIOS bug: APIC version is 0 for CPU %d/0x%x, fixing up to 0x10\n",
			   cpu, apicid);
		version = 0x10;
	}
	apic_version[apicid] = version;

	if (version != apic_version[boot_cpu_physical_apicid]) {
		pr_warning("BIOS bug: APIC version mismatch, boot CPU: %x, CPU %d: version %x\n",
			apic_version[boot_cpu_physical_apicid], cpu, version);
	}

	physid_set(apicid, phys_cpu_present_map);
	if (apicid > max_physical_apicid)
		max_physical_apicid = apicid;

#if defined(CONFIG_SMP) || defined(CONFIG_X86_64)
	early_per_cpu(x86_cpu_to_apicid, cpu) = apicid;
	early_per_cpu(x86_bios_cpu_apicid, cpu) = apicid;
#endif
#ifdef CONFIG_X86_32
	early_per_cpu(x86_cpu_to_logical_apicid, cpu) =
		apic->x86_32_early_logical_apicid(cpu);
#endif
	set_cpu_possible(cpu, true);
	set_cpu_present(cpu, true);
}

int hard_smp_processor_id(void)
{
	return read_apic_id();
}

void default_init_apic_ldr(void)
{
	unsigned long val;

	apic_write(APIC_DFR, APIC_DFR_VALUE);
	val = apic_read(APIC_LDR) & ~APIC_LDR_MASK;
	val |= SET_APIC_LOGICAL_ID(1UL << smp_processor_id());
	apic_write(APIC_LDR, val);
}

=======
void __irq_msi_compose_msg(struct irq_cfg *cfg, struct msi_msg *msg,
			   bool dmar)
{
	memset(msg, 0, sizeof(*msg));

	msg->arch_addr_lo.base_address = X86_MSI_BASE_ADDRESS_LOW;
	msg->arch_addr_lo.dest_mode_logical = apic->dest_mode_logical;
	msg->arch_addr_lo.destid_0_7 = cfg->dest_apicid & 0xFF;

	msg->arch_data.delivery_mode = APIC_DELIVERY_MODE_FIXED;
	msg->arch_data.vector = cfg->vector;

	msg->address_hi = X86_MSI_BASE_ADDRESS_HIGH;
	/*
	 * Only the IOMMU itself can use the trick of putting destination
	 * APIC ID into the high bits of the address. Anything else would
	 * just be writing to memory if it tried that, and needs IR to
	 * address APICs which can't be addressed in the normal 32-bit
	 * address range at 0xFFExxxxx. That is typically just 8 bits, but
	 * some hypervisors allow the extended destination ID field in bits
	 * 5-11 to be used, giving support for 15 bits of APIC IDs in total.
	 */
	if (dmar)
		msg->arch_addr_hi.destid_8_31 = cfg->dest_apicid >> 8;
	else if (virt_ext_dest_id && cfg->dest_apicid < 0x8000)
		msg->arch_addr_lo.virt_destid_8_14 = cfg->dest_apicid >> 8;
	else
		WARN_ON_ONCE(cfg->dest_apicid > 0xFF);
}

u32 x86_msi_msg_get_destid(struct msi_msg *msg, bool extid)
{
	u32 dest = msg->arch_addr_lo.destid_0_7;

	if (extid)
		dest |= msg->arch_addr_hi.destid_8_31 << 8;
	return dest;
}
EXPORT_SYMBOL_GPL(x86_msi_msg_get_destid);

static void __init apic_bsp_up_setup(void)
{
	reset_phys_cpu_present_map(boot_cpu_physical_apicid);
}

/**
 * apic_bsp_setup - Setup function for local apic and io-apic
 * @upmode:		Force UP mode (for APIC_init_uniprocessor)
 */
static void __init apic_bsp_setup(bool upmode)
{
	connect_bsp_APIC();
	if (upmode)
		apic_bsp_up_setup();
	setup_local_APIC();

	enable_IO_APIC();
	end_local_APIC_setup();
	irq_remap_enable_fault_handling();
	setup_IO_APIC();
	lapic_update_legacy_vectors();
}

#ifdef CONFIG_UP_LATE_INIT
void __init up_late_init(void)
{
	if (apic_intr_mode == APIC_PIC)
		return;

	/* Setup local timer */
	x86_init.timers.setup_percpu_clockev();
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Power management
 */
#ifdef CONFIG_PM

static struct {
	/*
	 * 'active' is true if the local APIC was enabled by us and
	 * not the BIOS; this signifies that we are also responsible
	 * for disabling it before entering apm/acpi suspend
	 */
	int active;
	/* r/w apic fields */
<<<<<<< HEAD
	unsigned int apic_id;
=======
	u32 apic_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int apic_taskpri;
	unsigned int apic_ldr;
	unsigned int apic_dfr;
	unsigned int apic_spiv;
	unsigned int apic_lvtt;
	unsigned int apic_lvtpc;
	unsigned int apic_lvt0;
	unsigned int apic_lvt1;
	unsigned int apic_lvterr;
	unsigned int apic_tmict;
	unsigned int apic_tdcr;
	unsigned int apic_thmr;
<<<<<<< HEAD
=======
	unsigned int apic_cmci;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} apic_pm_state;

static int lapic_suspend(void)
{
	unsigned long flags;
	int maxlvt;

	if (!apic_pm_state.active)
		return 0;

	maxlvt = lapic_get_maxlvt();

	apic_pm_state.apic_id = apic_read(APIC_ID);
	apic_pm_state.apic_taskpri = apic_read(APIC_TASKPRI);
	apic_pm_state.apic_ldr = apic_read(APIC_LDR);
	apic_pm_state.apic_dfr = apic_read(APIC_DFR);
	apic_pm_state.apic_spiv = apic_read(APIC_SPIV);
	apic_pm_state.apic_lvtt = apic_read(APIC_LVTT);
	if (maxlvt >= 4)
		apic_pm_state.apic_lvtpc = apic_read(APIC_LVTPC);
	apic_pm_state.apic_lvt0 = apic_read(APIC_LVT0);
	apic_pm_state.apic_lvt1 = apic_read(APIC_LVT1);
	apic_pm_state.apic_lvterr = apic_read(APIC_LVTERR);
	apic_pm_state.apic_tmict = apic_read(APIC_TMICT);
	apic_pm_state.apic_tdcr = apic_read(APIC_TDCR);
#ifdef CONFIG_X86_THERMAL_VECTOR
	if (maxlvt >= 5)
		apic_pm_state.apic_thmr = apic_read(APIC_LVTTHMR);
#endif
<<<<<<< HEAD

	local_irq_save(flags);
	disable_local_APIC();

	if (intr_remapping_enabled)
		disable_intr_remapping();
=======
#ifdef CONFIG_X86_MCE_INTEL
	if (maxlvt >= 6)
		apic_pm_state.apic_cmci = apic_read(APIC_LVTCMCI);
#endif

	local_irq_save(flags);

	/*
	 * Mask IOAPIC before disabling the local APIC to prevent stale IRR
	 * entries on some implementations.
	 */
	mask_ioapic_entries();

	disable_local_APIC();

	irq_remapping_disable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_restore(flags);
	return 0;
}

static void lapic_resume(void)
{
	unsigned int l, h;
	unsigned long flags;
	int maxlvt;

	if (!apic_pm_state.active)
		return;

	local_irq_save(flags);
<<<<<<< HEAD
	if (intr_remapping_enabled) {
		/*
		 * IO-APIC and PIC have their own resume routines.
		 * We just mask them here to make sure the interrupt
		 * subsystem is completely quiet while we enable x2apic
		 * and interrupt-remapping.
		 */
		mask_ioapic_entries();
		legacy_pic->mask_all();
	}

	if (x2apic_mode)
		enable_x2apic();
	else {
=======

	/*
	 * IO-APIC and PIC have their own resume routines.
	 * We just mask them here to make sure the interrupt
	 * subsystem is completely quiet while we enable x2apic
	 * and interrupt-remapping.
	 */
	mask_ioapic_entries();
	legacy_pic->mask_all();

	if (x2apic_mode) {
		__x2apic_enable();
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Make sure the APICBASE points to the right address
		 *
		 * FIXME! This will be wrong if we ever support suspend on
		 * SMP! We'll need to do this as part of the CPU restore!
		 */
		if (boot_cpu_data.x86 >= 6) {
			rdmsr(MSR_IA32_APICBASE, l, h);
			l &= ~MSR_IA32_APICBASE_BASE;
			l |= MSR_IA32_APICBASE_ENABLE | mp_lapic_addr;
			wrmsr(MSR_IA32_APICBASE, l, h);
		}
	}

	maxlvt = lapic_get_maxlvt();
	apic_write(APIC_LVTERR, ERROR_APIC_VECTOR | APIC_LVT_MASKED);
	apic_write(APIC_ID, apic_pm_state.apic_id);
	apic_write(APIC_DFR, apic_pm_state.apic_dfr);
	apic_write(APIC_LDR, apic_pm_state.apic_ldr);
	apic_write(APIC_TASKPRI, apic_pm_state.apic_taskpri);
	apic_write(APIC_SPIV, apic_pm_state.apic_spiv);
	apic_write(APIC_LVT0, apic_pm_state.apic_lvt0);
	apic_write(APIC_LVT1, apic_pm_state.apic_lvt1);
<<<<<<< HEAD
#if defined(CONFIG_X86_MCE_P4THERMAL) || defined(CONFIG_X86_MCE_INTEL)
	if (maxlvt >= 5)
		apic_write(APIC_LVTTHMR, apic_pm_state.apic_thmr);
#endif
=======
#ifdef CONFIG_X86_THERMAL_VECTOR
	if (maxlvt >= 5)
		apic_write(APIC_LVTTHMR, apic_pm_state.apic_thmr);
#endif
#ifdef CONFIG_X86_MCE_INTEL
	if (maxlvt >= 6)
		apic_write(APIC_LVTCMCI, apic_pm_state.apic_cmci);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (maxlvt >= 4)
		apic_write(APIC_LVTPC, apic_pm_state.apic_lvtpc);
	apic_write(APIC_LVTT, apic_pm_state.apic_lvtt);
	apic_write(APIC_TDCR, apic_pm_state.apic_tdcr);
	apic_write(APIC_TMICT, apic_pm_state.apic_tmict);
	apic_write(APIC_ESR, 0);
	apic_read(APIC_ESR);
	apic_write(APIC_LVTERR, apic_pm_state.apic_lvterr);
	apic_write(APIC_ESR, 0);
	apic_read(APIC_ESR);

<<<<<<< HEAD
	if (intr_remapping_enabled)
		reenable_intr_remapping(x2apic_mode);
=======
	irq_remapping_reenable(x2apic_mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_restore(flags);
}

/*
 * This device has no shutdown method - fully functioning local APICs
 * are needed on every CPU up until machine_halt/restart/poweroff.
 */

static struct syscore_ops lapic_syscore_ops = {
	.resume		= lapic_resume,
	.suspend	= lapic_suspend,
};

<<<<<<< HEAD
static void __cpuinit apic_pm_activate(void)
=======
static void apic_pm_activate(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	apic_pm_state.active = 1;
}

static int __init init_lapic_sysfs(void)
{
	/* XXX: remove suspend/resume procs if !apic_pm_state.active? */
<<<<<<< HEAD
	if (cpu_has_apic)
=======
	if (boot_cpu_has(X86_FEATURE_APIC))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		register_syscore_ops(&lapic_syscore_ops);

	return 0;
}

/* local apic needs to resume before other devices access its registers. */
core_initcall(init_lapic_sysfs);

#else	/* CONFIG_PM */

static void apic_pm_activate(void) { }

#endif	/* CONFIG_PM */

#ifdef CONFIG_X86_64

<<<<<<< HEAD
static int __cpuinit apic_cluster_num(void)
{
	int i, clusters, zeros;
	unsigned id;
	u16 *bios_cpu_apicid;
	DECLARE_BITMAP(clustermap, NUM_APIC_CLUSTERS);

	bios_cpu_apicid = early_per_cpu_ptr(x86_bios_cpu_apicid);
	bitmap_zero(clustermap, NUM_APIC_CLUSTERS);

	for (i = 0; i < nr_cpu_ids; i++) {
		/* are we being called early in kernel startup? */
		if (bios_cpu_apicid) {
			id = bios_cpu_apicid[i];
		} else if (i < nr_cpu_ids) {
			if (cpu_present(i))
				id = per_cpu(x86_bios_cpu_apicid, i);
			else
				continue;
		} else
			break;

		if (id != BAD_APICID)
			__set_bit(APIC_CLUSTERID(id), clustermap);
	}

	/* Problem:  Partially populated chassis may not have CPUs in some of
	 * the APIC clusters they have been allocated.  Only present CPUs have
	 * x86_bios_cpu_apicid entries, thus causing zeroes in the bitmap.
	 * Since clusters are allocated sequentially, count zeros only if
	 * they are bounded by ones.
	 */
	clusters = 0;
	zeros = 0;
	for (i = 0; i < NUM_APIC_CLUSTERS; i++) {
		if (test_bit(i, clustermap)) {
			clusters += 1 + zeros;
			zeros = 0;
		} else
			++zeros;
	}

	return clusters;
}

static int __cpuinitdata multi_checked;
static int __cpuinitdata multi;

static int __cpuinit set_multi(const struct dmi_system_id *d)
=======
static int multi_checked;
static int multi;

static int set_multi(const struct dmi_system_id *d)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (multi)
		return 0;
	pr_info("APIC: %s detected, Multi Chassis\n", d->ident);
	multi = 1;
	return 0;
}

<<<<<<< HEAD
static const __cpuinitconst struct dmi_system_id multi_dmi_table[] = {
=======
static const struct dmi_system_id multi_dmi_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.callback = set_multi,
		.ident = "IBM System Summit2",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "IBM"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Summit2"),
		},
	},
	{}
};

<<<<<<< HEAD
static void __cpuinit dmi_check_multi(void)
=======
static void dmi_check_multi(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (multi_checked)
		return;

	dmi_check_system(multi_dmi_table);
	multi_checked = 1;
}

/*
 * apic_is_clustered_box() -- Check if we can expect good TSC
 *
 * Thus far, the major user of this is IBM's Summit2 series:
 * Clustered boxes may have unsynced TSC problems if they are
 * multi-chassis.
 * Use DMI to check them
 */
<<<<<<< HEAD
__cpuinit int apic_is_clustered_box(void)
{
	dmi_check_multi();
	if (multi)
		return 1;

	if (!is_vsmp_box())
		return 0;

	/*
	 * ScaleMP vSMPowered boxes have one cluster per board and TSCs are
	 * not guaranteed to be synced between boards
	 */
	if (apic_cluster_num() > 1)
		return 1;

	return 0;
=======
int apic_is_clustered_box(void)
{
	dmi_check_multi();
	return multi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

/*
 * APIC command line parameters
 */
static int __init setup_disableapic(char *arg)
{
<<<<<<< HEAD
	disable_apic = 1;
=======
	apic_is_disabled = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	setup_clear_cpu_cap(X86_FEATURE_APIC);
	return 0;
}
early_param("disableapic", setup_disableapic);

/* same as disableapic, for compatibility */
static int __init setup_nolapic(char *arg)
{
	return setup_disableapic(arg);
}
early_param("nolapic", setup_nolapic);

static int __init parse_lapic_timer_c2_ok(char *arg)
{
	local_apic_timer_c2_ok = 1;
	return 0;
}
early_param("lapic_timer_c2_ok", parse_lapic_timer_c2_ok);

static int __init parse_disable_apic_timer(char *arg)
{
	disable_apic_timer = 1;
	return 0;
}
early_param("noapictimer", parse_disable_apic_timer);

static int __init parse_nolapic_timer(char *arg)
{
	disable_apic_timer = 1;
	return 0;
}
early_param("nolapic_timer", parse_nolapic_timer);

static int __init apic_set_verbosity(char *arg)
{
	if (!arg)  {
<<<<<<< HEAD
#ifdef CONFIG_X86_64
		skip_ioapic_setup = 0;
		return 0;
#endif
		return -EINVAL;
=======
		if (IS_ENABLED(CONFIG_X86_32))
			return -EINVAL;

		ioapic_is_disabled = false;
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (strcmp("debug", arg) == 0)
		apic_verbosity = APIC_DEBUG;
	else if (strcmp("verbose", arg) == 0)
		apic_verbosity = APIC_VERBOSE;
<<<<<<< HEAD
	else {
		pr_warning("APIC Verbosity level %s not recognised"
			" use apic=verbose or apic=debug\n", arg);
		return -EINVAL;
	}
=======
#ifdef CONFIG_X86_64
	else {
		pr_warn("APIC Verbosity level %s not recognised"
			" use apic=verbose or apic=debug\n", arg);
		return -EINVAL;
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
early_param("apic", apic_set_verbosity);

static int __init lapic_insert_resource(void)
{
<<<<<<< HEAD
	if (!apic_phys)
		return -1;

	/* Put local APIC into the resource map. */
	lapic_resource.start = apic_phys;
=======
	if (!apic_mmio_base)
		return -1;

	/* Put local APIC into the resource map. */
	lapic_resource.start = apic_mmio_base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lapic_resource.end = lapic_resource.start + PAGE_SIZE - 1;
	insert_resource(&iomem_resource, &lapic_resource);

	return 0;
}

/*
<<<<<<< HEAD
 * need call insert after e820_reserve_resources()
 * that is using request_resource
 */
late_initcall(lapic_insert_resource);
=======
 * need call insert after e820__reserve_resources()
 * that is using request_resource
 */
late_initcall(lapic_insert_resource);

static int __init apic_set_extnmi(char *arg)
{
	if (!arg)
		return -EINVAL;

	if (!strncmp("all", arg, 3))
		apic_extnmi = APIC_EXTNMI_ALL;
	else if (!strncmp("none", arg, 4))
		apic_extnmi = APIC_EXTNMI_NONE;
	else if (!strncmp("bsp", arg, 3))
		apic_extnmi = APIC_EXTNMI_BSP;
	else {
		pr_warn("Unknown external NMI delivery mode `%s' ignored\n", arg);
		return -EINVAL;
	}

	return 0;
}
early_param("apic_extnmi", apic_set_extnmi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
