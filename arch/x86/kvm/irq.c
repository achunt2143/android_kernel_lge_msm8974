<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * irq.c: API for in kernel interrupt controller
 * Copyright (c) 2007, Intel Corporation.
 * Copyright 2009 Red Hat, Inc. and/or its affiliates.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 * Authors:
 *   Yaozu (Eddie) Dong <Eddie.dong@intel.com>
 *
 */

#include <linux/module.h>
=======
 * Authors:
 *   Yaozu (Eddie) Dong <Eddie.dong@intel.com>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kvm_host.h>

#include "irq.h"
#include "i8254.h"
#include "x86.h"
<<<<<<< HEAD
=======
#include "xen.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * check if there are pending timer events
 * to be processed.
 */
int kvm_cpu_has_pending_timer(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	return apic_has_pending_timer(vcpu);
}
EXPORT_SYMBOL(kvm_cpu_has_pending_timer);
=======
	int r = 0;

	if (lapic_in_kernel(vcpu))
		r = apic_has_pending_timer(vcpu);
	if (kvm_xen_timer_enabled(vcpu))
		r += kvm_xen_has_pending_timer(vcpu);

	return r;
}

/*
 * check if there is a pending userspace external interrupt
 */
static int pending_userspace_extint(struct kvm_vcpu *v)
{
	return v->arch.pending_external_vector != -1;
}

/*
 * check if there is pending interrupt from
 * non-APIC source without intack.
 */
int kvm_cpu_has_extint(struct kvm_vcpu *v)
{
	/*
	 * FIXME: interrupt.injected represents an interrupt whose
	 * side-effects have already been applied (e.g. bit from IRR
	 * already moved to ISR). Therefore, it is incorrect to rely
	 * on interrupt.injected to know if there is a pending
	 * interrupt in the user-mode LAPIC.
	 * This leads to nVMX/nSVM not be able to distinguish
	 * if it should exit from L2 to L1 on EXTERNAL_INTERRUPT on
	 * pending interrupt or should re-inject an injected
	 * interrupt.
	 */
	if (!lapic_in_kernel(v))
		return v->arch.interrupt.injected;

	if (kvm_xen_has_interrupt(v))
		return 1;

	if (!kvm_apic_accept_pic_intr(v))
		return 0;

	if (irqchip_split(v->kvm))
		return pending_userspace_extint(v);
	else
		return v->kvm->arch.vpic->output;
}

/*
 * check if there is injectable interrupt:
 * when virtual interrupt delivery enabled,
 * interrupt from apic will handled by hardware,
 * we don't need to check it here.
 */
int kvm_cpu_has_injectable_intr(struct kvm_vcpu *v)
{
	if (kvm_cpu_has_extint(v))
		return 1;

	if (!is_guest_mode(v) && kvm_vcpu_apicv_active(v))
		return 0;

	return kvm_apic_has_interrupt(v) != -1; /* LAPIC */
}
EXPORT_SYMBOL_GPL(kvm_cpu_has_injectable_intr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * check if there is pending interrupt without
 * intack.
 */
int kvm_cpu_has_interrupt(struct kvm_vcpu *v)
{
<<<<<<< HEAD
	struct kvm_pic *s;

	if (!irqchip_in_kernel(v->kvm))
		return v->arch.interrupt.pending;

	if (kvm_apic_has_interrupt(v) == -1) {	/* LAPIC */
		if (kvm_apic_accept_pic_intr(v)) {
			s = pic_irqchip(v->kvm);	/* PIC */
			return s->output;
		} else
			return 0;
	}
	return 1;
=======
	if (kvm_cpu_has_extint(v))
		return 1;

	return kvm_apic_has_interrupt(v) != -1;	/* LAPIC */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(kvm_cpu_has_interrupt);

/*
<<<<<<< HEAD
=======
 * Read pending interrupt(from non-APIC source)
 * vector and intack.
 */
static int kvm_cpu_get_extint(struct kvm_vcpu *v)
{
	if (!kvm_cpu_has_extint(v)) {
		WARN_ON(!lapic_in_kernel(v));
		return -1;
	}

	if (!lapic_in_kernel(v))
		return v->arch.interrupt.nr;

#ifdef CONFIG_KVM_XEN
	if (kvm_xen_has_interrupt(v))
		return v->kvm->arch.xen.upcall_vector;
#endif

	if (irqchip_split(v->kvm)) {
		int vector = v->arch.pending_external_vector;

		v->arch.pending_external_vector = -1;
		return vector;
	} else
		return kvm_pic_read_irq(v->kvm); /* PIC */
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Read pending interrupt vector and intack.
 */
int kvm_cpu_get_interrupt(struct kvm_vcpu *v)
{
<<<<<<< HEAD
	struct kvm_pic *s;
	int vector;

	if (!irqchip_in_kernel(v->kvm))
		return v->arch.interrupt.nr;

	vector = kvm_get_apic_interrupt(v);	/* APIC */
	if (vector == -1) {
		if (kvm_apic_accept_pic_intr(v)) {
			s = pic_irqchip(v->kvm);
			s->output = 0;		/* PIC */
			vector = kvm_pic_read_irq(v->kvm);
		}
	}
	return vector;
=======
	int vector = kvm_cpu_get_extint(v);
	if (vector != -1)
		return vector;			/* PIC */

	return kvm_get_apic_interrupt(v);	/* APIC */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(kvm_cpu_get_interrupt);

void kvm_inject_pending_timer_irqs(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	kvm_inject_apic_timer_irqs(vcpu);
	/* TODO: PIT, RTC etc. */
}
EXPORT_SYMBOL_GPL(kvm_inject_pending_timer_irqs);
=======
	if (lapic_in_kernel(vcpu))
		kvm_inject_apic_timer_irqs(vcpu);
	if (kvm_xen_timer_enabled(vcpu))
		kvm_xen_inject_timer_irqs(vcpu);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void __kvm_migrate_timers(struct kvm_vcpu *vcpu)
{
	__kvm_migrate_apic_timer(vcpu);
	__kvm_migrate_pit_timer(vcpu);
<<<<<<< HEAD
=======
	static_call_cond(kvm_x86_migrate_timers)(vcpu);
}

bool kvm_arch_irqfd_allowed(struct kvm *kvm, struct kvm_irqfd *args)
{
	bool resample = args->flags & KVM_IRQFD_FLAG_RESAMPLE;

	return resample ? irqchip_kernel(kvm) : irqchip_in_kernel(kvm);
}

bool kvm_arch_irqchip_in_kernel(struct kvm *kvm)
{
	return irqchip_in_kernel(kvm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
