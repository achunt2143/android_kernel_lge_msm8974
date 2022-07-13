<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ip27-irq.c: Highlevel interrupt handling for IP27 architecture.
 *
 * Copyright (C) 1999, 2000 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Copyright (C) 1999 - 2001 Kanoj Sarcar
 */

<<<<<<< HEAD
#undef DEBUG

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/timex.h>
#include <linux/smp.h>
#include <linux/random.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
#include <linux/bitops.h>

#include <asm/bootinfo.h>
#include <asm/io.h>
#include <asm/mipsregs.h>

#include <asm/processor.h>
#include <asm/pci/bridge.h>
#include <asm/sn/addrs.h>
#include <asm/sn/agent.h>
#include <asm/sn/arch.h>
#include <asm/sn/hub.h>
#include <asm/sn/intr.h>

/*
 * Linux has a controller-independent x86 interrupt architecture.
 * every controller has a 'controller-template', that is used
 * by the main code to do the right thing. Each driver-visible
 * interrupt source is transparently wired to the appropriate
 * controller. Thus drivers need not be aware of the
 * interrupt-controller.
 *
 * Various interrupt controllers we handle: 8259 PIC, SMP IO-APIC,
 * PIIX4's internal 8259 PIC and SGI's Visual Workstation Cobalt (IO-)APIC.
 * (IO-APICs assumed to be messaging to Pentium local-APICs)
 *
 * the code is designed to be easily extended with new/different
 * interrupt controllers, without having to do assembly magic.
 */

extern asmlinkage void ip27_irq(void);

extern struct bridge_controller *irq_to_bridge[];
extern int irq_to_slot[];

/*
 * use these macros to get the encoded nasid and widget id
 * from the irq value
 */
#define IRQ_TO_BRIDGE(i)		irq_to_bridge[(i)]
#define	SLOT_FROM_PCI_IRQ(i)		irq_to_slot[i]

static inline int alloc_level(int cpu, int irq)
{
	struct hub_data *hub = hub_data(cpu_to_node(cpu));
	struct slice_data *si = cpu_data[cpu].data;
	int level;

	level = find_first_zero_bit(hub->irq_alloc_mask, LEVELS_PER_SLICE);
	if (level >= LEVELS_PER_SLICE)
		panic("Cpu %d flooded with devices", cpu);

	__set_bit(level, hub->irq_alloc_mask);
	si->level_to_irq[level] = irq;
=======
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/sched.h>

#include <asm/io.h>
#include <asm/irq_cpu.h>
#include <asm/sn/addrs.h>
#include <asm/sn/agent.h>
#include <asm/sn/arch.h>
#include <asm/sn/intr.h>
#include <asm/sn/irq_alloc.h>

#include "ip27-common.h"

struct hub_irq_data {
	u64	*irq_mask[2];
	cpuid_t	cpu;
};

static DECLARE_BITMAP(hub_irq_map, IP27_HUB_IRQ_COUNT);

static DEFINE_PER_CPU(unsigned long [2], irq_enable_mask);

static inline int alloc_level(void)
{
	int level;

again:
	level = find_first_zero_bit(hub_irq_map, IP27_HUB_IRQ_COUNT);
	if (level >= IP27_HUB_IRQ_COUNT)
		return -ENOSPC;

	if (test_and_set_bit(level, hub_irq_map))
		goto again;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return level;
}

<<<<<<< HEAD
static inline int find_level(cpuid_t *cpunum, int irq)
{
	int cpu, i;

	for_each_online_cpu(cpu) {
		struct slice_data *si = cpu_data[cpu].data;

		for (i = BASE_PCI_IRQ; i < LEVELS_PER_SLICE; i++)
			if (si->level_to_irq[i] == irq) {
				*cpunum = cpu;

				return i;
			}
	}

	panic("Could not identify cpu/level for irq %d", irq);
}

/*
 * Find first bit set
 */
static int ms1bit(unsigned long x)
{
	int b = 0, s;

	s = 16; if (x >> 16 == 0) s = 0; b += s; x >>= s;
	s =  8; if (x >>  8 == 0) s = 0; b += s; x >>= s;
	s =  4; if (x >>  4 == 0) s = 0; b += s; x >>= s;
	s =  2; if (x >>  2 == 0) s = 0; b += s; x >>= s;
	s =  1; if (x >>  1 == 0) s = 0; b += s;

	return b;
}

=======
static void enable_hub_irq(struct irq_data *d)
{
	struct hub_irq_data *hd = irq_data_get_irq_chip_data(d);
	unsigned long *mask = per_cpu(irq_enable_mask, hd->cpu);

	set_bit(d->hwirq, mask);
	__raw_writeq(mask[0], hd->irq_mask[0]);
	__raw_writeq(mask[1], hd->irq_mask[1]);
}

static void disable_hub_irq(struct irq_data *d)
{
	struct hub_irq_data *hd = irq_data_get_irq_chip_data(d);
	unsigned long *mask = per_cpu(irq_enable_mask, hd->cpu);

	clear_bit(d->hwirq, mask);
	__raw_writeq(mask[0], hd->irq_mask[0]);
	__raw_writeq(mask[1], hd->irq_mask[1]);
}

static void setup_hub_mask(struct hub_irq_data *hd, const struct cpumask *mask)
{
	nasid_t nasid;
	int cpu;

	cpu = cpumask_first_and(mask, cpu_online_mask);
	if (cpu >= nr_cpu_ids)
		cpu = cpumask_any(cpu_online_mask);

	nasid = cpu_to_node(cpu);
	hd->cpu = cpu;
	if (!cputoslice(cpu)) {
		hd->irq_mask[0] = REMOTE_HUB_PTR(nasid, PI_INT_MASK0_A);
		hd->irq_mask[1] = REMOTE_HUB_PTR(nasid, PI_INT_MASK1_A);
	} else {
		hd->irq_mask[0] = REMOTE_HUB_PTR(nasid, PI_INT_MASK0_B);
		hd->irq_mask[1] = REMOTE_HUB_PTR(nasid, PI_INT_MASK1_B);
	}
}

static int set_affinity_hub_irq(struct irq_data *d, const struct cpumask *mask,
				bool force)
{
	struct hub_irq_data *hd = irq_data_get_irq_chip_data(d);

	if (!hd)
		return -EINVAL;

	if (irqd_is_started(d))
		disable_hub_irq(d);

	setup_hub_mask(hd, mask);

	if (irqd_is_started(d))
		enable_hub_irq(d);

	irq_data_update_effective_affinity(d, cpumask_of(hd->cpu));

	return 0;
}

static struct irq_chip hub_irq_type = {
	.name		  = "HUB",
	.irq_mask	  = disable_hub_irq,
	.irq_unmask	  = enable_hub_irq,
	.irq_set_affinity = set_affinity_hub_irq,
};

static int hub_domain_alloc(struct irq_domain *domain, unsigned int virq,
			    unsigned int nr_irqs, void *arg)
{
	struct irq_alloc_info *info = arg;
	struct hub_irq_data *hd;
	struct hub_data *hub;
	struct irq_desc *desc;
	int swlevel;

	if (nr_irqs > 1 || !info)
		return -EINVAL;

	hd = kzalloc(sizeof(*hd), GFP_KERNEL);
	if (!hd)
		return -ENOMEM;

	swlevel = alloc_level();
	if (unlikely(swlevel < 0)) {
		kfree(hd);
		return -EAGAIN;
	}
	irq_domain_set_info(domain, virq, swlevel, &hub_irq_type, hd,
			    handle_level_irq, NULL, NULL);

	/* use CPU connected to nearest hub */
	hub = hub_data(info->nasid);
	setup_hub_mask(hd, &hub->h_cpus);
	info->nasid = cpu_to_node(hd->cpu);

	/* Make sure it's not already pending when we connect it. */
	REMOTE_HUB_CLR_INTR(info->nasid, swlevel);

	desc = irq_to_desc(virq);
	desc->irq_common_data.node = info->nasid;
	cpumask_copy(desc->irq_common_data.affinity, &hub->h_cpus);

	return 0;
}

static void hub_domain_free(struct irq_domain *domain,
			    unsigned int virq, unsigned int nr_irqs)
{
	struct irq_data *irqd;

	if (nr_irqs > 1)
		return;

	irqd = irq_domain_get_irq_data(domain, virq);
	if (irqd && irqd->chip_data)
		kfree(irqd->chip_data);
}

static const struct irq_domain_ops hub_domain_ops = {
	.alloc = hub_domain_alloc,
	.free  = hub_domain_free,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This code is unnecessarily complex, because we do
 * intr enabling. Basically, once we grab the set of intrs we need
 * to service, we must mask _all_ these interrupts; firstly, to make
 * sure the same intr does not intr again, causing recursion that
 * can lead to stack overflow. Secondly, we can not just mask the
 * one intr we are do_IRQing, because the non-masked intrs in the
 * first set might intr again, causing multiple servicings of the
 * same intr. This effect is mostly seen for intercpu intrs.
 * Kanoj 05.13.00
 */

<<<<<<< HEAD
static void ip27_do_irq_mask0(void)
{
	int irq, swlevel;
	hubreg_t pend0, mask0;
	cpuid_t cpu = smp_processor_id();
	int pi_int_mask0 =
		(cputoslice(cpu) == 0) ?  PI_INT_MASK0_A : PI_INT_MASK0_B;

	/* copied from Irix intpend0() */
	pend0 = LOCAL_HUB_L(PI_INT_PEND0);
	mask0 = LOCAL_HUB_L(pi_int_mask0);

	pend0 &= mask0;		/* Pick intrs we should look at */
	if (!pend0)
		return;

	swlevel = ms1bit(pend0);
=======
static void ip27_do_irq_mask0(struct irq_desc *desc)
{
	cpuid_t cpu = smp_processor_id();
	unsigned long *mask = per_cpu(irq_enable_mask, cpu);
	struct irq_domain *domain;
	u64 pend0;
	int ret;

	/* copied from Irix intpend0() */
	pend0 = LOCAL_HUB_L(PI_INT_PEND0);

	pend0 &= mask[0];		/* Pick intrs we should look at */
	if (!pend0)
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SMP
	if (pend0 & (1UL << CPU_RESCHED_A_IRQ)) {
		LOCAL_HUB_CLR_INTR(CPU_RESCHED_A_IRQ);
		scheduler_ipi();
	} else if (pend0 & (1UL << CPU_RESCHED_B_IRQ)) {
		LOCAL_HUB_CLR_INTR(CPU_RESCHED_B_IRQ);
		scheduler_ipi();
	} else if (pend0 & (1UL << CPU_CALL_A_IRQ)) {
		LOCAL_HUB_CLR_INTR(CPU_CALL_A_IRQ);
<<<<<<< HEAD
		smp_call_function_interrupt();
	} else if (pend0 & (1UL << CPU_CALL_B_IRQ)) {
		LOCAL_HUB_CLR_INTR(CPU_CALL_B_IRQ);
		smp_call_function_interrupt();
	} else
#endif
	{
		/* "map" swlevel to irq */
		struct slice_data *si = cpu_data[cpu].data;

		irq = si->level_to_irq[swlevel];
		do_IRQ(irq);
=======
		generic_smp_call_function_interrupt();
	} else if (pend0 & (1UL << CPU_CALL_B_IRQ)) {
		LOCAL_HUB_CLR_INTR(CPU_CALL_B_IRQ);
		generic_smp_call_function_interrupt();
	} else
#endif
	{
		domain = irq_desc_get_handler_data(desc);
		ret = generic_handle_domain_irq(domain, __ffs(pend0));
		if (ret)
			spurious_interrupt();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	LOCAL_HUB_L(PI_INT_PEND0);
}

<<<<<<< HEAD
static void ip27_do_irq_mask1(void)
{
	int irq, swlevel;
	hubreg_t pend1, mask1;
	cpuid_t cpu = smp_processor_id();
	int pi_int_mask1 = (cputoslice(cpu) == 0) ?  PI_INT_MASK1_A : PI_INT_MASK1_B;
	struct slice_data *si = cpu_data[cpu].data;

	/* copied from Irix intpend0() */
	pend1 = LOCAL_HUB_L(PI_INT_PEND1);
	mask1 = LOCAL_HUB_L(pi_int_mask1);

	pend1 &= mask1;		/* Pick intrs we should look at */
	if (!pend1)
		return;

	swlevel = ms1bit(pend1);
	/* "map" swlevel to irq */
	irq = si->level_to_irq[swlevel];
	LOCAL_HUB_CLR_INTR(swlevel);
	do_IRQ(irq);
=======
static void ip27_do_irq_mask1(struct irq_desc *desc)
{
	cpuid_t cpu = smp_processor_id();
	unsigned long *mask = per_cpu(irq_enable_mask, cpu);
	struct irq_domain *domain;
	u64 pend1;
	int ret;

	/* copied from Irix intpend0() */
	pend1 = LOCAL_HUB_L(PI_INT_PEND1);

	pend1 &= mask[1];		/* Pick intrs we should look at */
	if (!pend1)
		return;

	domain = irq_desc_get_handler_data(desc);
	ret = generic_handle_domain_irq(domain, __ffs(pend1) + 64);
	if (ret)
		spurious_interrupt();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	LOCAL_HUB_L(PI_INT_PEND1);
}

<<<<<<< HEAD
static void ip27_prof_timer(void)
{
	panic("CPU %d got a profiling interrupt", smp_processor_id());
}

static void ip27_hub_error(void)
{
	panic("CPU %d got a hub error interrupt", smp_processor_id());
}

static int intr_connect_level(int cpu, int bit)
{
	nasid_t nasid = COMPACT_TO_NASID_NODEID(cpu_to_node(cpu));
	struct slice_data *si = cpu_data[cpu].data;

	set_bit(bit, si->irq_enable_mask);

	if (!cputoslice(cpu)) {
		REMOTE_HUB_S(nasid, PI_INT_MASK0_A, si->irq_enable_mask[0]);
		REMOTE_HUB_S(nasid, PI_INT_MASK1_A, si->irq_enable_mask[1]);
	} else {
		REMOTE_HUB_S(nasid, PI_INT_MASK0_B, si->irq_enable_mask[0]);
		REMOTE_HUB_S(nasid, PI_INT_MASK1_B, si->irq_enable_mask[1]);
	}

	return 0;
}

static int intr_disconnect_level(int cpu, int bit)
{
	nasid_t nasid = COMPACT_TO_NASID_NODEID(cpu_to_node(cpu));
	struct slice_data *si = cpu_data[cpu].data;

	clear_bit(bit, si->irq_enable_mask);

	if (!cputoslice(cpu)) {
		REMOTE_HUB_S(nasid, PI_INT_MASK0_A, si->irq_enable_mask[0]);
		REMOTE_HUB_S(nasid, PI_INT_MASK1_A, si->irq_enable_mask[1]);
	} else {
		REMOTE_HUB_S(nasid, PI_INT_MASK0_B, si->irq_enable_mask[0]);
		REMOTE_HUB_S(nasid, PI_INT_MASK1_B, si->irq_enable_mask[1]);
	}

	return 0;
}

/* Startup one of the (PCI ...) IRQs routes over a bridge.  */
static unsigned int startup_bridge_irq(struct irq_data *d)
{
	struct bridge_controller *bc;
	bridgereg_t device;
	bridge_t *bridge;
	int pin, swlevel;
	cpuid_t cpu;

	pin = SLOT_FROM_PCI_IRQ(d->irq);
	bc = IRQ_TO_BRIDGE(d->irq);
	bridge = bc->base;

	pr_debug("bridge_startup(): irq= 0x%x  pin=%d\n", d->irq, pin);
	/*
	 * "map" irq to a swlevel greater than 6 since the first 6 bits
	 * of INT_PEND0 are taken
	 */
	swlevel = find_level(&cpu, d->irq);
	bridge->b_int_addr[pin].addr = (0x20000 | swlevel | (bc->nasid << 8));
	bridge->b_int_enable |= (1 << pin);
	bridge->b_int_enable |= 0x7ffffe00;	/* more stuff in int_enable */

	/*
	 * Enable sending of an interrupt clear packt to the hub on a high to
	 * low transition of the interrupt pin.
	 *
	 * IRIX sets additional bits in the address which are documented as
	 * reserved in the bridge docs.
	 */
	bridge->b_int_mode |= (1UL << pin);

	/*
	 * We assume the bridge to have a 1:1 mapping between devices
	 * (slots) and intr pins.
	 */
	device = bridge->b_int_device;
	device &= ~(7 << (pin*3));
	device |= (pin << (pin*3));
	bridge->b_int_device = device;

        bridge->b_wid_tflush;

	intr_connect_level(cpu, swlevel);

        return 0;       /* Never anything pending.  */
}

/* Shutdown one of the (PCI ...) IRQs routes over a bridge.  */
static void shutdown_bridge_irq(struct irq_data *d)
{
	struct bridge_controller *bc = IRQ_TO_BRIDGE(d->irq);
	bridge_t *bridge = bc->base;
	int pin, swlevel;
	cpuid_t cpu;

	pr_debug("bridge_shutdown: irq 0x%x\n", d->irq);
	pin = SLOT_FROM_PCI_IRQ(d->irq);

	/*
	 * map irq to a swlevel greater than 6 since the first 6 bits
	 * of INT_PEND0 are taken
	 */
	swlevel = find_level(&cpu, d->irq);
	intr_disconnect_level(cpu, swlevel);

	bridge->b_int_enable &= ~(1 << pin);
	bridge->b_wid_tflush;
}

static inline void enable_bridge_irq(struct irq_data *d)
{
	cpuid_t cpu;
	int swlevel;

	swlevel = find_level(&cpu, d->irq);	/* Criminal offence */
	intr_connect_level(cpu, swlevel);
}

static inline void disable_bridge_irq(struct irq_data *d)
{
	cpuid_t cpu;
	int swlevel;

	swlevel = find_level(&cpu, d->irq);	/* Criminal offence */
	intr_disconnect_level(cpu, swlevel);
}

static struct irq_chip bridge_irq_type = {
	.name		= "bridge",
	.irq_startup	= startup_bridge_irq,
	.irq_shutdown	= shutdown_bridge_irq,
	.irq_mask	= disable_bridge_irq,
	.irq_unmask	= enable_bridge_irq,
};

void register_bridge_irq(unsigned int irq)
{
	irq_set_chip_and_handler(irq, &bridge_irq_type, handle_level_irq);
}

int request_bridge_irq(struct bridge_controller *bc)
{
	int irq = allocate_irqno();
	int swlevel, cpu;
	nasid_t nasid;

	if (irq < 0)
		return irq;

	/*
	 * "map" irq to a swlevel greater than 6 since the first 6 bits
	 * of INT_PEND0 are taken
	 */
	cpu = bc->irq_cpu;
	swlevel = alloc_level(cpu, irq);
	if (unlikely(swlevel < 0)) {
		free_irqno(irq);

		return -EAGAIN;
	}

	/* Make sure it's not already pending when we connect it. */
	nasid = COMPACT_TO_NASID_NODEID(cpu_to_node(cpu));
	REMOTE_HUB_CLR_INTR(nasid, swlevel);

	intr_connect_level(cpu, swlevel);

	register_bridge_irq(irq);

	return irq;
}

asmlinkage void plat_irq_dispatch(void)
{
	unsigned long pending = read_c0_cause() & read_c0_status();
	extern unsigned int rt_timer_irq;

	if (pending & CAUSEF_IP4)
		do_IRQ(rt_timer_irq);
	else if (pending & CAUSEF_IP2)	/* PI_INT_PEND_0 or CC_PEND_{A|B} */
		ip27_do_irq_mask0();
	else if (pending & CAUSEF_IP3)	/* PI_INT_PEND_1 */
		ip27_do_irq_mask1();
	else if (pending & CAUSEF_IP5)
		ip27_prof_timer();
	else if (pending & CAUSEF_IP6)
		ip27_hub_error();
=======
void install_ipi(void)
{
	int cpu = smp_processor_id();
	unsigned long *mask = per_cpu(irq_enable_mask, cpu);
	int slice = LOCAL_HUB_L(PI_CPU_NUM);
	int resched, call;

	resched = CPU_RESCHED_A_IRQ + slice;
	set_bit(resched, mask);
	LOCAL_HUB_CLR_INTR(resched);

	call = CPU_CALL_A_IRQ + slice;
	set_bit(call, mask);
	LOCAL_HUB_CLR_INTR(call);

	if (slice == 0) {
		LOCAL_HUB_S(PI_INT_MASK0_A, mask[0]);
		LOCAL_HUB_S(PI_INT_MASK1_A, mask[1]);
	} else {
		LOCAL_HUB_S(PI_INT_MASK0_B, mask[0]);
		LOCAL_HUB_S(PI_INT_MASK1_B, mask[1]);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init arch_init_irq(void)
{
<<<<<<< HEAD
}

void install_ipi(void)
{
	int slice = LOCAL_HUB_L(PI_CPU_NUM);
	int cpu = smp_processor_id();
	struct slice_data *si = cpu_data[cpu].data;
	struct hub_data *hub = hub_data(cpu_to_node(cpu));
	int resched, call;

	resched = CPU_RESCHED_A_IRQ + slice;
	__set_bit(resched, hub->irq_alloc_mask);
	__set_bit(resched, si->irq_enable_mask);
	LOCAL_HUB_CLR_INTR(resched);

	call = CPU_CALL_A_IRQ + slice;
	__set_bit(call, hub->irq_alloc_mask);
	__set_bit(call, si->irq_enable_mask);
	LOCAL_HUB_CLR_INTR(call);

	if (slice == 0) {
		LOCAL_HUB_S(PI_INT_MASK0_A, si->irq_enable_mask[0]);
		LOCAL_HUB_S(PI_INT_MASK1_A, si->irq_enable_mask[1]);
	} else {
		LOCAL_HUB_S(PI_INT_MASK0_B, si->irq_enable_mask[0]);
		LOCAL_HUB_S(PI_INT_MASK1_B, si->irq_enable_mask[1]);
	}
=======
	struct irq_domain *domain;
	struct fwnode_handle *fn;
	int i;

	mips_cpu_irq_init();

	/*
	 * Some interrupts are reserved by hardware or by software convention.
	 * Mark these as reserved right away so they won't be used accidentally
	 * later.
	 */
	for (i = 0; i <= CPU_CALL_B_IRQ; i++)
		set_bit(i, hub_irq_map);

	for (i = NI_BRDCAST_ERR_A; i <= MSC_PANIC_INTR; i++)
		set_bit(i, hub_irq_map);

	fn = irq_domain_alloc_named_fwnode("HUB");
	WARN_ON(fn == NULL);
	if (!fn)
		return;
	domain = irq_domain_create_linear(fn, IP27_HUB_IRQ_COUNT,
					  &hub_domain_ops, NULL);
	WARN_ON(domain == NULL);
	if (!domain)
		return;

	irq_set_default_host(domain);

	irq_set_percpu_devid(IP27_HUB_PEND0_IRQ);
	irq_set_chained_handler_and_data(IP27_HUB_PEND0_IRQ, ip27_do_irq_mask0,
					 domain);
	irq_set_percpu_devid(IP27_HUB_PEND1_IRQ);
	irq_set_chained_handler_and_data(IP27_HUB_PEND1_IRQ, ip27_do_irq_mask1,
					 domain);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
