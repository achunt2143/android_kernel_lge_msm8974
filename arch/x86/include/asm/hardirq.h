<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_HARDIRQ_H
#define _ASM_X86_HARDIRQ_H

#include <linux/threads.h>
<<<<<<< HEAD
#include <linux/irq.h>

typedef struct {
	unsigned int __softirq_pending;
=======
#include <asm/current.h>

typedef struct {
#if IS_ENABLED(CONFIG_KVM_INTEL)
	u8	     kvm_cpu_l1tf_flush_l1d;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int __nmi_count;	/* arch dependent */
#ifdef CONFIG_X86_LOCAL_APIC
	unsigned int apic_timer_irqs;	/* arch dependent */
	unsigned int irq_spurious_count;
	unsigned int icr_read_retry_count;
#endif
<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_KVM)
	unsigned int kvm_posted_intr_ipis;
	unsigned int kvm_posted_intr_wakeup_ipis;
	unsigned int kvm_posted_intr_nested_ipis;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int x86_platform_ipis;	/* arch dependent */
	unsigned int apic_perf_irqs;
	unsigned int apic_irq_work_irqs;
#ifdef CONFIG_SMP
	unsigned int irq_resched_count;
	unsigned int irq_call_count;
<<<<<<< HEAD
	unsigned int irq_tlb_count;
#endif
=======
#endif
	unsigned int irq_tlb_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_X86_THERMAL_VECTOR
	unsigned int irq_thermal_count;
#endif
#ifdef CONFIG_X86_MCE_THRESHOLD
	unsigned int irq_threshold_count;
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_X86_MCE_AMD
	unsigned int irq_deferred_error_count;
#endif
#ifdef CONFIG_X86_HV_CALLBACK_VECTOR
	unsigned int irq_hv_callback_count;
#endif
#if IS_ENABLED(CONFIG_HYPERV)
	unsigned int irq_hv_reenlightenment_count;
	unsigned int hyperv_stimer0_count;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} ____cacheline_aligned irq_cpustat_t;

DECLARE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);

<<<<<<< HEAD
/* We can have at most NR_VECTORS irqs routed to a cpu at a time */
#define MAX_HARDIRQS_PER_CPU NR_VECTORS

#define __ARCH_IRQ_STAT

#define inc_irq_stat(member)	percpu_inc(irq_stat.member)

#define local_softirq_pending()	percpu_read(irq_stat.__softirq_pending)

#define __ARCH_SET_SOFTIRQ_PENDING

#define set_softirq_pending(x)	percpu_write(irq_stat.__softirq_pending, (x))
#define or_softirq_pending(x)	percpu_or(irq_stat.__softirq_pending, (x))
=======
#define __ARCH_IRQ_STAT

#define inc_irq_stat(member)	this_cpu_inc(irq_stat.member)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void ack_bad_irq(unsigned int irq);

extern u64 arch_irq_stat_cpu(unsigned int cpu);
#define arch_irq_stat_cpu	arch_irq_stat_cpu

extern u64 arch_irq_stat(void);
#define arch_irq_stat		arch_irq_stat

<<<<<<< HEAD
=======
#define local_softirq_pending_ref       pcpu_hot.softirq_pending

#if IS_ENABLED(CONFIG_KVM_INTEL)
static inline void kvm_set_cpu_l1tf_flush_l1d(void)
{
	__this_cpu_write(irq_stat.kvm_cpu_l1tf_flush_l1d, 1);
}

static __always_inline void kvm_clear_cpu_l1tf_flush_l1d(void)
{
	__this_cpu_write(irq_stat.kvm_cpu_l1tf_flush_l1d, 0);
}

static __always_inline bool kvm_get_cpu_l1tf_flush_l1d(void)
{
	return __this_cpu_read(irq_stat.kvm_cpu_l1tf_flush_l1d);
}
#else /* !IS_ENABLED(CONFIG_KVM_INTEL) */
static inline void kvm_set_cpu_l1tf_flush_l1d(void) { }
#endif /* IS_ENABLED(CONFIG_KVM_INTEL) */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_HARDIRQ_H */
