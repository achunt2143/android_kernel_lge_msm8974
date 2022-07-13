<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_ACPI_H
#define _ASM_X86_ACPI_H

/*
 *  Copyright (C) 2001 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *  Copyright (C) 2001 Patrick Mochel <mochel@osdl.org>
<<<<<<< HEAD
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <acpi/pdc_intel.h>

#include <asm/numa.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/mpspec.h>
#include <asm/trampoline.h>

#define COMPILER_DEPENDENT_INT64   long long
#define COMPILER_DEPENDENT_UINT64  unsigned long long

/*
 * Calling conventions:
 *
 * ACPI_SYSTEM_XFACE        - Interfaces to host OS (handlers, threads)
 * ACPI_EXTERNAL_XFACE      - External ACPI interfaces
 * ACPI_INTERNAL_XFACE      - Internal ACPI interfaces
 * ACPI_INTERNAL_VAR_XFACE  - Internal variable-parameter list interfaces
 */
#define ACPI_SYSTEM_XFACE
#define ACPI_EXTERNAL_XFACE
#define ACPI_INTERNAL_XFACE
#define ACPI_INTERNAL_VAR_XFACE

/* Asm macros */

#define ACPI_ASM_MACROS
#define BREAKPOINT3
#define ACPI_DISABLE_IRQS() local_irq_disable()
#define ACPI_ENABLE_IRQS()  local_irq_enable()
#define ACPI_FLUSH_CPU_CACHE()	wbinvd()

int __acpi_acquire_global_lock(unsigned int *lock);
int __acpi_release_global_lock(unsigned int *lock);

#define ACPI_ACQUIRE_GLOBAL_LOCK(facs, Acq) \
	((Acq) = __acpi_acquire_global_lock(&facs->global_lock))

#define ACPI_RELEASE_GLOBAL_LOCK(facs, Acq) \
	((Acq) = __acpi_release_global_lock(&facs->global_lock))

/*
 * Math helper asm macros
 */
#define ACPI_DIV_64_BY_32(n_hi, n_lo, d32, q32, r32) \
	asm("divl %2;"				     \
	    : "=a"(q32), "=d"(r32)		     \
	    : "r"(d32),				     \
	     "0"(n_lo), "1"(n_hi))


#define ACPI_SHIFT_RIGHT_64(n_hi, n_lo) \
	asm("shrl   $1,%2	;"	\
	    "rcrl   $1,%3;"		\
	    : "=r"(n_hi), "=r"(n_lo)	\
	    : "0"(n_hi), "1"(n_lo))
=======
 */
#include <acpi/proc_cap_intel.h>

#include <asm/numa.h>
#include <asm/fixmap.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/mpspec.h>
#include <asm/x86_init.h>
#include <asm/cpufeature.h>
#include <asm/irq_vectors.h>
#include <asm/xen/hypervisor.h>

#include <xen/xen.h>

#ifdef CONFIG_ACPI_APEI
# include <asm/pgtable_types.h>
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_ACPI
extern int acpi_lapic;
extern int acpi_ioapic;
extern int acpi_noirq;
extern int acpi_strict;
extern int acpi_disabled;
extern int acpi_pci_disabled;
extern int acpi_skip_timer_override;
extern int acpi_use_timer_override;
extern int acpi_fix_pin2_polarity;
<<<<<<< HEAD

extern u8 acpi_sci_flags;
extern int acpi_sci_override_gsi;
void acpi_pic_sci_set_trigger(unsigned int, u16);

extern int (*__acpi_register_gsi)(struct device *dev, u32 gsi,
				  int trigger, int polarity);
=======
extern int acpi_disable_cmcff;
extern bool acpi_int_src_ovr[NR_IRQS_LEGACY];

extern u8 acpi_sci_flags;
extern u32 acpi_sci_override_gsi;
void acpi_pic_sci_set_trigger(unsigned int, u16);

struct device;

extern int (*__acpi_register_gsi)(struct device *dev, u32 gsi,
				  int trigger, int polarity);
extern void (*__acpi_unregister_gsi)(u32 gsi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void disable_acpi(void)
{
	acpi_disabled = 1;
	acpi_pci_disabled = 1;
	acpi_noirq = 1;
}

extern int acpi_gsi_to_irq(u32 gsi, unsigned int *irq);

static inline void acpi_noirq_set(void) { acpi_noirq = 1; }
static inline void acpi_disable_pci(void)
{
	acpi_pci_disabled = 1;
	acpi_noirq_set();
}

/* Low-level suspend routine. */
<<<<<<< HEAD
extern int acpi_suspend_lowlevel(void);

extern const unsigned char acpi_wakeup_code[];
#define acpi_wakeup_address (__pa(TRAMPOLINE_SYM(acpi_wakeup_code)))

/* early initialization routine */
extern void acpi_reserve_wakeup_memory(void);
=======
extern int (*acpi_suspend_lowlevel)(void);

/* Physical address to resume after wakeup */
unsigned long acpi_get_wakeup_address(void);

static inline bool acpi_skip_set_wakeup_address(void)
{
	return cpu_feature_enabled(X86_FEATURE_XENPV);
}

#define acpi_skip_set_wakeup_address acpi_skip_set_wakeup_address
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Check if the CPU can handle C2 and deeper
 */
static inline unsigned int acpi_processor_cstate_check(unsigned int max_cstate)
{
	/*
	 * Early models (<=5) of AMD Opterons are not supposed to go into
	 * C2 state.
	 *
	 * Steppings 0x0A and later are good
	 */
	if (boot_cpu_data.x86 == 0x0F &&
	    boot_cpu_data.x86_vendor == X86_VENDOR_AMD &&
	    boot_cpu_data.x86_model <= 0x05 &&
<<<<<<< HEAD
	    boot_cpu_data.x86_mask < 0x0A)
		return 1;
	else if (amd_e400_c1e_detected)
=======
	    boot_cpu_data.x86_stepping < 0x0A)
		return 1;
	else if (boot_cpu_has(X86_BUG_AMD_APIC_C1E))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	else
		return max_cstate;
}

static inline bool arch_has_acpi_pdc(void)
{
	struct cpuinfo_x86 *c = &cpu_data(0);
	return (c->x86_vendor == X86_VENDOR_INTEL ||
		c->x86_vendor == X86_VENDOR_CENTAUR);
}

<<<<<<< HEAD
static inline void arch_acpi_set_pdc_bits(u32 *buf)
{
	struct cpuinfo_x86 *c = &cpu_data(0);

	buf[2] |= ACPI_PDC_C_CAPABILITY_SMP;

	if (cpu_has(c, X86_FEATURE_EST))
		buf[2] |= ACPI_PDC_EST_CAPABILITY_SWSMP;

	if (cpu_has(c, X86_FEATURE_ACPI))
		buf[2] |= ACPI_PDC_T_FFH;

	/*
	 * If mwait/monitor is unsupported, C2/C3_FFH will be disabled
	 */
	if (!cpu_has(c, X86_FEATURE_MWAIT))
		buf[2] &= ~(ACPI_PDC_C_C2C3_FFH);
}

=======
static inline void arch_acpi_set_proc_cap_bits(u32 *cap)
{
	struct cpuinfo_x86 *c = &cpu_data(0);

	*cap |= ACPI_PROC_CAP_C_CAPABILITY_SMP;

	/* Enable coordination with firmware's _TSD info */
	*cap |= ACPI_PROC_CAP_SMP_T_SWCOORD;

	if (cpu_has(c, X86_FEATURE_EST))
		*cap |= ACPI_PROC_CAP_EST_CAPABILITY_SWSMP;

	if (cpu_has(c, X86_FEATURE_ACPI))
		*cap |= ACPI_PROC_CAP_T_FFH;

	if (cpu_has(c, X86_FEATURE_HWP))
		*cap |= ACPI_PROC_CAP_COLLAB_PROC_PERF;

	/*
	 * If mwait/monitor is unsupported, C_C1_FFH and
	 * C2/C3_FFH will be disabled.
	 */
	if (!cpu_has(c, X86_FEATURE_MWAIT) ||
	    boot_option_idle_override == IDLE_NOMWAIT)
		*cap &= ~(ACPI_PROC_CAP_C_C1_FFH | ACPI_PROC_CAP_C_C2C3_FFH);

	if (xen_initial_domain()) {
		/*
		 * When Linux is running as Xen dom0, the hypervisor is the
		 * entity in charge of the processor power management, and so
		 * Xen needs to check the OS capabilities reported in the
		 * processor capabilities buffer matches what the hypervisor
		 * driver supports.
		 */
		xen_sanitize_proc_cap_bits(cap);
	}
}

static inline bool acpi_has_cpu_in_madt(void)
{
	return !!acpi_lapic;
}

#define ACPI_HAVE_ARCH_SET_ROOT_POINTER
static inline void acpi_arch_set_root_pointer(u64 addr)
{
	x86_init.acpi.set_root_pointer(addr);
}

#define ACPI_HAVE_ARCH_GET_ROOT_POINTER
static inline u64 acpi_arch_get_root_pointer(void)
{
	return x86_init.acpi.get_root_pointer();
}

void acpi_generic_reduced_hw_init(void);

void x86_default_set_root_pointer(u64 addr);
u64 x86_default_get_root_pointer(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* !CONFIG_ACPI */

#define acpi_lapic 0
#define acpi_ioapic 0
<<<<<<< HEAD
=======
#define acpi_disable_cmcff 0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void acpi_noirq_set(void) { }
static inline void acpi_disable_pci(void) { }
static inline void disable_acpi(void) { }

<<<<<<< HEAD
=======
static inline void acpi_generic_reduced_hw_init(void) { }

static inline void x86_default_set_root_pointer(u64 addr) { }

static inline u64 x86_default_get_root_pointer(void)
{
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* !CONFIG_ACPI */

#define ARCH_HAS_POWER_INIT	1

#ifdef CONFIG_ACPI_NUMA
<<<<<<< HEAD
extern int acpi_numa;
extern int x86_acpi_numa_init(void);
#endif /* CONFIG_ACPI_NUMA */

#define acpi_unlazy_tlb(x)	leave_mm(x)
=======
extern int x86_acpi_numa_init(void);
#endif /* CONFIG_ACPI_NUMA */

struct cper_ia_proc_ctx;

#ifdef CONFIG_ACPI_APEI
static inline pgprot_t arch_apei_get_mem_attribute(phys_addr_t addr)
{
	/*
	 * We currently have no way to look up the EFI memory map
	 * attributes for a region in a consistent way, because the
	 * memmap is discarded after efi_free_boot_services(). So if
	 * you call efi_mem_attributes() during boot and at runtime,
	 * you could theoretically see different attributes.
	 *
	 * We are yet to see any x86 platforms that require anything
	 * other than PAGE_KERNEL (some ARM64 platforms require the
	 * equivalent of PAGE_KERNEL_NOCACHE). Additionally, if SME
	 * is active, the ACPI information will not be encrypted,
	 * so return PAGE_KERNEL_NOENC until we know differently.
	 */
	return PAGE_KERNEL_NOENC;
}

int arch_apei_report_x86_error(struct cper_ia_proc_ctx *ctx_info,
			       u64 lapic_id);
#else
static inline int arch_apei_report_x86_error(struct cper_ia_proc_ctx *ctx_info,
					     u64 lapic_id)
{
	return -EINVAL;
}
#endif

#define ACPI_TABLE_UPGRADE_MAX_PHYS (max_low_pfn_mapped << PAGE_SHIFT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_ACPI_H */
