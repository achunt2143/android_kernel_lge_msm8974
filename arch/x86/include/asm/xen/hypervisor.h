/******************************************************************************
 * hypervisor.h
 *
 * Linux-specific hypervisor handling.
 *
 * Copyright (c) 2002-2004, K A Fraser
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _ASM_X86_XEN_HYPERVISOR_H
#define _ASM_X86_XEN_HYPERVISOR_H

<<<<<<< HEAD
/* arch/i386/kernel/setup.c */
extern struct shared_info *HYPERVISOR_shared_info;
extern struct start_info *xen_start_info;

#include <asm/processor.h>

static inline uint32_t xen_cpuid_base(void)
{
	uint32_t base, eax, ebx, ecx, edx;
	char signature[13];

	for (base = 0x40000000; base < 0x40010000; base += 0x100) {
		cpuid(base, &eax, &ebx, &ecx, &edx);
		*(uint32_t *)(signature + 0) = ebx;
		*(uint32_t *)(signature + 4) = ecx;
		*(uint32_t *)(signature + 8) = edx;
		signature[12] = 0;

		if (!strcmp("XenVMMXenVMM", signature) && ((eax - base) >= 2))
			return base;
	}

	return 0;
}

#ifdef CONFIG_XEN
extern bool xen_hvm_need_lapic(void);

static inline bool xen_x2apic_para_available(void)
{
	return xen_hvm_need_lapic();
}
#else
static inline bool xen_x2apic_para_available(void)
{
	return (xen_cpuid_base() != 0);
}
#endif

extern void xen_set_iopl_mask(unsigned mask);

=======
extern struct shared_info *HYPERVISOR_shared_info;
extern struct start_info *xen_start_info;

#include <asm/bug.h>
#include <asm/processor.h>

#define XEN_SIGNATURE "XenVMMXenVMM"

static inline uint32_t xen_cpuid_base(void)
{
	return hypervisor_cpuid_base(XEN_SIGNATURE, 2);
}

struct pci_dev;

#ifdef CONFIG_XEN_PV_DOM0
bool xen_initdom_restore_msi(struct pci_dev *dev);
#else
static inline bool xen_initdom_restore_msi(struct pci_dev *dev) { return true; }
#endif

#ifdef CONFIG_HOTPLUG_CPU
void xen_arch_register_cpu(int num);
void xen_arch_unregister_cpu(int num);
#endif

#ifdef CONFIG_PVH
void __init xen_pvh_init(struct boot_params *boot_params);
void __init mem_map_via_hcall(struct boot_params *boot_params_p);
#ifdef CONFIG_XEN_PVH
void __init xen_reserve_extra_memory(struct boot_params *bootp);
#else
static inline void xen_reserve_extra_memory(struct boot_params *bootp) { }
#endif
#endif

/* Lazy mode for batching updates / context switch */
enum xen_lazy_mode {
	XEN_LAZY_NONE,
	XEN_LAZY_MMU,
	XEN_LAZY_CPU,
};

DECLARE_PER_CPU(enum xen_lazy_mode, xen_lazy_mode);
DECLARE_PER_CPU(unsigned int, xen_lazy_nesting);

static inline void enter_lazy(enum xen_lazy_mode mode)
{
	enum xen_lazy_mode old_mode = this_cpu_read(xen_lazy_mode);

	if (mode == old_mode) {
		this_cpu_inc(xen_lazy_nesting);
		return;
	}

	BUG_ON(old_mode != XEN_LAZY_NONE);

	this_cpu_write(xen_lazy_mode, mode);
}

static inline void leave_lazy(enum xen_lazy_mode mode)
{
	BUG_ON(this_cpu_read(xen_lazy_mode) != mode);

	if (this_cpu_read(xen_lazy_nesting) == 0)
		this_cpu_write(xen_lazy_mode, XEN_LAZY_NONE);
	else
		this_cpu_dec(xen_lazy_nesting);
}

enum xen_lazy_mode xen_get_lazy_mode(void);

#if defined(CONFIG_XEN_DOM0) && defined(CONFIG_ACPI)
void xen_sanitize_proc_cap_bits(uint32_t *buf);
#else
static inline void xen_sanitize_proc_cap_bits(uint32_t *buf)
{
	BUG();
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_XEN_HYPERVISOR_H */
