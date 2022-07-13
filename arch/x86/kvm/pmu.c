<<<<<<< HEAD
/*
 * Kernel-based Virtual Machine -- Performane Monitoring Unit support
 *
 * Copyright 2011 Red Hat, Inc. and/or its affiliates.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Kernel-based Virtual Machine -- Performance Monitoring Unit support
 *
 * Copyright 2015 Red Hat, Inc. and/or its affiliates.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Authors:
 *   Avi Kivity   <avi@redhat.com>
 *   Gleb Natapov <gleb@redhat.com>
<<<<<<< HEAD
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */
=======
 *   Wei Huang    <wei@redhat.com>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/types.h>
#include <linux/kvm_host.h>
#include <linux/perf_event.h>
<<<<<<< HEAD
#include "x86.h"
#include "cpuid.h"
#include "lapic.h"

static struct kvm_arch_event_perf_mapping {
	u8 eventsel;
	u8 unit_mask;
	unsigned event_type;
	bool inexact;
} arch_events[] = {
	/* Index must match CPUID 0x0A.EBX bit vector */
	[0] = { 0x3c, 0x00, PERF_COUNT_HW_CPU_CYCLES },
	[1] = { 0xc0, 0x00, PERF_COUNT_HW_INSTRUCTIONS },
	[2] = { 0x3c, 0x01, PERF_COUNT_HW_BUS_CYCLES  },
	[3] = { 0x2e, 0x4f, PERF_COUNT_HW_CACHE_REFERENCES },
	[4] = { 0x2e, 0x41, PERF_COUNT_HW_CACHE_MISSES },
	[5] = { 0xc4, 0x00, PERF_COUNT_HW_BRANCH_INSTRUCTIONS },
	[6] = { 0xc5, 0x00, PERF_COUNT_HW_BRANCH_MISSES },
	[7] = { 0x00, 0x30, PERF_COUNT_HW_REF_CPU_CYCLES },
};

/* mapping between fixed pmc index and arch_events array */
int fixed_pmc_events[] = {1, 0, 7};

static bool pmc_is_gp(struct kvm_pmc *pmc)
{
	return pmc->type == KVM_PMC_GP;
}

static inline u64 pmc_bitmask(struct kvm_pmc *pmc)
{
	struct kvm_pmu *pmu = &pmc->vcpu->arch.pmu;

	return pmu->counter_bitmask[pmc->type];
}

static inline bool pmc_enabled(struct kvm_pmc *pmc)
{
	struct kvm_pmu *pmu = &pmc->vcpu->arch.pmu;
	return test_bit(pmc->idx, (unsigned long *)&pmu->global_ctrl);
}

static inline struct kvm_pmc *get_gp_pmc(struct kvm_pmu *pmu, u32 msr,
					 u32 base)
{
	if (msr >= base && msr < base + pmu->nr_arch_gp_counters)
		return &pmu->gp_counters[msr - base];
	return NULL;
}

static inline struct kvm_pmc *get_fixed_pmc(struct kvm_pmu *pmu, u32 msr)
{
	int base = MSR_CORE_PERF_FIXED_CTR0;
	if (msr >= base && msr < base + pmu->nr_arch_fixed_counters)
		return &pmu->fixed_counters[msr - base];
	return NULL;
}

static inline struct kvm_pmc *get_fixed_pmc_idx(struct kvm_pmu *pmu, int idx)
{
	return get_fixed_pmc(pmu, MSR_CORE_PERF_FIXED_CTR0 + idx);
}

static struct kvm_pmc *global_idx_to_pmc(struct kvm_pmu *pmu, int idx)
{
	if (idx < X86_PMC_IDX_FIXED)
		return get_gp_pmc(pmu, MSR_P6_EVNTSEL0 + idx, MSR_P6_EVNTSEL0);
	else
		return get_fixed_pmc_idx(pmu, idx - X86_PMC_IDX_FIXED);
}

void kvm_deliver_pmi(struct kvm_vcpu *vcpu)
{
	if (vcpu->arch.apic)
		kvm_apic_local_deliver(vcpu->arch.apic, APIC_LVTPC);
}

static void trigger_pmi(struct irq_work *irq_work)
{
	struct kvm_pmu *pmu = container_of(irq_work, struct kvm_pmu,
			irq_work);
	struct kvm_vcpu *vcpu = container_of(pmu, struct kvm_vcpu,
			arch.pmu);

	kvm_deliver_pmi(vcpu);
=======
#include <linux/bsearch.h>
#include <linux/sort.h>
#include <asm/perf_event.h>
#include <asm/cpu_device_id.h>
#include "x86.h"
#include "cpuid.h"
#include "lapic.h"
#include "pmu.h"

/* This is enough to filter the vast majority of currently defined events. */
#define KVM_PMU_EVENT_FILTER_MAX_EVENTS 300

struct x86_pmu_capability __read_mostly kvm_pmu_cap;
EXPORT_SYMBOL_GPL(kvm_pmu_cap);

struct kvm_pmu_emulated_event_selectors __read_mostly kvm_pmu_eventsel;
EXPORT_SYMBOL_GPL(kvm_pmu_eventsel);

/* Precise Distribution of Instructions Retired (PDIR) */
static const struct x86_cpu_id vmx_pebs_pdir_cpu[] = {
	X86_MATCH_INTEL_FAM6_MODEL(ICELAKE_D, NULL),
	X86_MATCH_INTEL_FAM6_MODEL(ICELAKE_X, NULL),
	/* Instruction-Accurate PDIR (PDIR++) */
	X86_MATCH_INTEL_FAM6_MODEL(SAPPHIRERAPIDS_X, NULL),
	{}
};

/* Precise Distribution (PDist) */
static const struct x86_cpu_id vmx_pebs_pdist_cpu[] = {
	X86_MATCH_INTEL_FAM6_MODEL(SAPPHIRERAPIDS_X, NULL),
	{}
};

/* NOTE:
 * - Each perf counter is defined as "struct kvm_pmc";
 * - There are two types of perf counters: general purpose (gp) and fixed.
 *   gp counters are stored in gp_counters[] and fixed counters are stored
 *   in fixed_counters[] respectively. Both of them are part of "struct
 *   kvm_pmu";
 * - pmu.c understands the difference between gp counters and fixed counters.
 *   However AMD doesn't support fixed-counters;
 * - There are three types of index to access perf counters (PMC):
 *     1. MSR (named msr): For example Intel has MSR_IA32_PERFCTRn and AMD
 *        has MSR_K7_PERFCTRn and, for families 15H and later,
 *        MSR_F15H_PERF_CTRn, where MSR_F15H_PERF_CTR[0-3] are
 *        aliased to MSR_K7_PERFCTRn.
 *     2. MSR Index (named idx): This normally is used by RDPMC instruction.
 *        For instance AMD RDPMC instruction uses 0000_0003h in ECX to access
 *        C001_0007h (MSR_K7_PERCTR3). Intel has a similar mechanism, except
 *        that it also supports fixed counters. idx can be used to as index to
 *        gp and fixed counters.
 *     3. Global PMC Index (named pmc): pmc is an index specific to PMU
 *        code. Each pmc, stored in kvm_pmc.idx field, is unique across
 *        all perf counters (both gp and fixed). The mapping relationship
 *        between pmc and perf counters is as the following:
 *        * Intel: [0 .. KVM_INTEL_PMC_MAX_GENERIC-1] <=> gp counters
 *                 [KVM_FIXED_PMC_BASE_IDX .. KVM_FIXED_PMC_BASE_IDX + 2] <=> fixed
 *        * AMD:   [0 .. AMD64_NUM_COUNTERS-1] and, for families 15H
 *          and later, [0 .. AMD64_NUM_COUNTERS_CORE-1] <=> gp counters
 */

static struct kvm_pmu_ops kvm_pmu_ops __read_mostly;

#define KVM_X86_PMU_OP(func)					     \
	DEFINE_STATIC_CALL_NULL(kvm_x86_pmu_##func,			     \
				*(((struct kvm_pmu_ops *)0)->func));
#define KVM_X86_PMU_OP_OPTIONAL KVM_X86_PMU_OP
#include <asm/kvm-x86-pmu-ops.h>

void kvm_pmu_ops_update(const struct kvm_pmu_ops *pmu_ops)
{
	memcpy(&kvm_pmu_ops, pmu_ops, sizeof(kvm_pmu_ops));

#define __KVM_X86_PMU_OP(func) \
	static_call_update(kvm_x86_pmu_##func, kvm_pmu_ops.func);
#define KVM_X86_PMU_OP(func) \
	WARN_ON(!kvm_pmu_ops.func); __KVM_X86_PMU_OP(func)
#define KVM_X86_PMU_OP_OPTIONAL __KVM_X86_PMU_OP
#include <asm/kvm-x86-pmu-ops.h>
#undef __KVM_X86_PMU_OP
}

static inline void __kvm_perf_overflow(struct kvm_pmc *pmc, bool in_pmi)
{
	struct kvm_pmu *pmu = pmc_to_pmu(pmc);
	bool skip_pmi = false;

	if (pmc->perf_event && pmc->perf_event->attr.precise_ip) {
		if (!in_pmi) {
			/*
			 * TODO: KVM is currently _choosing_ to not generate records
			 * for emulated instructions, avoiding BUFFER_OVF PMI when
			 * there are no records. Strictly speaking, it should be done
			 * as well in the right context to improve sampling accuracy.
			 */
			skip_pmi = true;
		} else {
			/* Indicate PEBS overflow PMI to guest. */
			skip_pmi = __test_and_set_bit(GLOBAL_STATUS_BUFFER_OVF_BIT,
						      (unsigned long *)&pmu->global_status);
		}
	} else {
		__set_bit(pmc->idx, (unsigned long *)&pmu->global_status);
	}

	if (pmc->intr && !skip_pmi)
		kvm_make_request(KVM_REQ_PMI, pmc->vcpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void kvm_perf_overflow(struct perf_event *perf_event,
			      struct perf_sample_data *data,
			      struct pt_regs *regs)
{
	struct kvm_pmc *pmc = perf_event->overflow_handler_context;
<<<<<<< HEAD
	struct kvm_pmu *pmu = &pmc->vcpu->arch.pmu;
	__set_bit(pmc->idx, (unsigned long *)&pmu->global_status);
}

static void kvm_perf_overflow_intr(struct perf_event *perf_event,
		struct perf_sample_data *data, struct pt_regs *regs)
{
	struct kvm_pmc *pmc = perf_event->overflow_handler_context;
	struct kvm_pmu *pmu = &pmc->vcpu->arch.pmu;
	if (!test_and_set_bit(pmc->idx, (unsigned long *)&pmu->reprogram_pmi)) {
		kvm_perf_overflow(perf_event, data, regs);
		kvm_make_request(KVM_REQ_PMU, pmc->vcpu);
		/*
		 * Inject PMI. If vcpu was in a guest mode during NMI PMI
		 * can be ejected on a guest mode re-entry. Otherwise we can't
		 * be sure that vcpu wasn't executing hlt instruction at the
		 * time of vmexit and is not going to re-enter guest mode until,
		 * woken up. So we should wake it, but this is impossible from
		 * NMI context. Do it from irq work instead.
		 */
		if (!kvm_is_in_guest())
			irq_work_queue(&pmc->vcpu->arch.pmu.irq_work);
		else
			kvm_make_request(KVM_REQ_PMI, pmc->vcpu);
	}
}

static u64 read_pmc(struct kvm_pmc *pmc)
{
	u64 counter, enabled, running;

	counter = pmc->counter;

	if (pmc->perf_event)
		counter += perf_event_read_value(pmc->perf_event,
						 &enabled, &running);

	/* FIXME: Scaling needed? */

	return counter & pmc_bitmask(pmc);
}

static void stop_counter(struct kvm_pmc *pmc)
{
	if (pmc->perf_event) {
		pmc->counter = read_pmc(pmc);
		perf_event_release_kernel(pmc->perf_event);
		pmc->perf_event = NULL;
	}
}

static void reprogram_counter(struct kvm_pmc *pmc, u32 type,
		unsigned config, bool exclude_user, bool exclude_kernel,
		bool intr)
{
=======

	/*
	 * Ignore asynchronous overflow events for counters that are scheduled
	 * to be reprogrammed, e.g. if a PMI for the previous event races with
	 * KVM's handling of a related guest WRMSR.
	 */
	if (test_and_set_bit(pmc->idx, pmc_to_pmu(pmc)->reprogram_pmi))
		return;

	__kvm_perf_overflow(pmc, true);

	kvm_make_request(KVM_REQ_PMU, pmc->vcpu);
}

static u64 pmc_get_pebs_precise_level(struct kvm_pmc *pmc)
{
	/*
	 * For some model specific pebs counters with special capabilities
	 * (PDIR, PDIR++, PDIST), KVM needs to raise the event precise
	 * level to the maximum value (currently 3, backwards compatible)
	 * so that the perf subsystem would assign specific hardware counter
	 * with that capability for vPMC.
	 */
	if ((pmc->idx == 0 && x86_match_cpu(vmx_pebs_pdist_cpu)) ||
	    (pmc->idx == 32 && x86_match_cpu(vmx_pebs_pdir_cpu)))
		return 3;

	/*
	 * The non-zero precision level of guest event makes the ordinary
	 * guest event becomes a guest PEBS event and triggers the host
	 * PEBS PMI handler to determine whether the PEBS overflow PMI
	 * comes from the host counters or the guest.
	 */
	return 1;
}

static u64 get_sample_period(struct kvm_pmc *pmc, u64 counter_value)
{
	u64 sample_period = (-counter_value) & pmc_bitmask(pmc);

	if (!sample_period)
		sample_period = pmc_bitmask(pmc) + 1;
	return sample_period;
}

static int pmc_reprogram_counter(struct kvm_pmc *pmc, u32 type, u64 config,
				 bool exclude_user, bool exclude_kernel,
				 bool intr)
{
	struct kvm_pmu *pmu = pmc_to_pmu(pmc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct perf_event *event;
	struct perf_event_attr attr = {
		.type = type,
		.size = sizeof(attr),
		.pinned = true,
		.exclude_idle = true,
		.exclude_host = 1,
		.exclude_user = exclude_user,
		.exclude_kernel = exclude_kernel,
		.config = config,
	};
<<<<<<< HEAD

	attr.sample_period = (-pmc->counter) & pmc_bitmask(pmc);

	event = perf_event_create_kernel_counter(&attr, -1, current,
						 intr ? kvm_perf_overflow_intr :
						 kvm_perf_overflow, pmc);
	if (IS_ERR(event)) {
		printk_once("kvm: pmu event creation failed %ld\n",
				PTR_ERR(event));
		return;
	}

	pmc->perf_event = event;
	clear_bit(pmc->idx, (unsigned long*)&pmc->vcpu->arch.pmu.reprogram_pmi);
}

static unsigned find_arch_event(struct kvm_pmu *pmu, u8 event_select,
		u8 unit_mask)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(arch_events); i++)
		if (arch_events[i].eventsel == event_select
				&& arch_events[i].unit_mask == unit_mask
				&& (pmu->available_event_types & (1 << i)))
			break;

	if (i == ARRAY_SIZE(arch_events))
		return PERF_COUNT_HW_MAX;

	return arch_events[i].event_type;
}

static void reprogram_gp_counter(struct kvm_pmc *pmc, u64 eventsel)
{
	unsigned config, type = PERF_TYPE_RAW;
	u8 event_select, unit_mask;
=======
	bool pebs = test_bit(pmc->idx, (unsigned long *)&pmu->pebs_enable);

	attr.sample_period = get_sample_period(pmc, pmc->counter);

	if ((attr.config & HSW_IN_TX_CHECKPOINTED) &&
	    guest_cpuid_is_intel(pmc->vcpu)) {
		/*
		 * HSW_IN_TX_CHECKPOINTED is not supported with nonzero
		 * period. Just clear the sample period so at least
		 * allocating the counter doesn't fail.
		 */
		attr.sample_period = 0;
	}
	if (pebs) {
		/*
		 * For most PEBS hardware events, the difference in the software
		 * precision levels of guest and host PEBS events will not affect
		 * the accuracy of the PEBS profiling result, because the "event IP"
		 * in the PEBS record is calibrated on the guest side.
		 */
		attr.precise_ip = pmc_get_pebs_precise_level(pmc);
	}

	event = perf_event_create_kernel_counter(&attr, -1, current,
						 kvm_perf_overflow, pmc);
	if (IS_ERR(event)) {
		pr_debug_ratelimited("kvm_pmu: event creation failed %ld for pmc->idx = %d\n",
			    PTR_ERR(event), pmc->idx);
		return PTR_ERR(event);
	}

	pmc->perf_event = event;
	pmc_to_pmu(pmc)->event_count++;
	pmc->is_paused = false;
	pmc->intr = intr || pebs;
	return 0;
}

static bool pmc_pause_counter(struct kvm_pmc *pmc)
{
	u64 counter = pmc->counter;
	u64 prev_counter;

	/* update counter, reset event value to avoid redundant accumulation */
	if (pmc->perf_event && !pmc->is_paused)
		counter += perf_event_pause(pmc->perf_event, true);

	/*
	 * Snapshot the previous counter *after* accumulating state from perf.
	 * If overflow already happened, hardware (via perf) is responsible for
	 * generating a PMI.  KVM just needs to detect overflow on emulated
	 * counter events that haven't yet been processed.
	 */
	prev_counter = counter & pmc_bitmask(pmc);

	counter += pmc->emulated_counter;
	pmc->counter = counter & pmc_bitmask(pmc);

	pmc->emulated_counter = 0;
	pmc->is_paused = true;

	return pmc->counter < prev_counter;
}

static bool pmc_resume_counter(struct kvm_pmc *pmc)
{
	if (!pmc->perf_event)
		return false;

	/* recalibrate sample period and check if it's accepted by perf core */
	if (is_sampling_event(pmc->perf_event) &&
	    perf_event_period(pmc->perf_event,
			      get_sample_period(pmc, pmc->counter)))
		return false;

	if (test_bit(pmc->idx, (unsigned long *)&pmc_to_pmu(pmc)->pebs_enable) !=
	    (!!pmc->perf_event->attr.precise_ip))
		return false;

	/* reuse perf_event to serve as pmc_reprogram_counter() does*/
	perf_event_enable(pmc->perf_event);
	pmc->is_paused = false;

	return true;
}

static void pmc_release_perf_event(struct kvm_pmc *pmc)
{
	if (pmc->perf_event) {
		perf_event_release_kernel(pmc->perf_event);
		pmc->perf_event = NULL;
		pmc->current_config = 0;
		pmc_to_pmu(pmc)->event_count--;
	}
}

static void pmc_stop_counter(struct kvm_pmc *pmc)
{
	if (pmc->perf_event) {
		pmc->counter = pmc_read_counter(pmc);
		pmc_release_perf_event(pmc);
	}
}

static void pmc_update_sample_period(struct kvm_pmc *pmc)
{
	if (!pmc->perf_event || pmc->is_paused ||
	    !is_sampling_event(pmc->perf_event))
		return;

	perf_event_period(pmc->perf_event,
			  get_sample_period(pmc, pmc->counter));
}

void pmc_write_counter(struct kvm_pmc *pmc, u64 val)
{
	/*
	 * Drop any unconsumed accumulated counts, the WRMSR is a write, not a
	 * read-modify-write.  Adjust the counter value so that its value is
	 * relative to the current count, as reading the current count from
	 * perf is faster than pausing and repgrogramming the event in order to
	 * reset it to '0'.  Note, this very sneakily offsets the accumulated
	 * emulated count too, by using pmc_read_counter()!
	 */
	pmc->emulated_counter = 0;
	pmc->counter += val - pmc_read_counter(pmc);
	pmc->counter &= pmc_bitmask(pmc);
	pmc_update_sample_period(pmc);
}
EXPORT_SYMBOL_GPL(pmc_write_counter);

static int filter_cmp(const void *pa, const void *pb, u64 mask)
{
	u64 a = *(u64 *)pa & mask;
	u64 b = *(u64 *)pb & mask;

	return (a > b) - (a < b);
}


static int filter_sort_cmp(const void *pa, const void *pb)
{
	return filter_cmp(pa, pb, (KVM_PMU_MASKED_ENTRY_EVENT_SELECT |
				   KVM_PMU_MASKED_ENTRY_EXCLUDE));
}

/*
 * For the event filter, searching is done on the 'includes' list and
 * 'excludes' list separately rather than on the 'events' list (which
 * has both).  As a result the exclude bit can be ignored.
 */
static int filter_event_cmp(const void *pa, const void *pb)
{
	return filter_cmp(pa, pb, (KVM_PMU_MASKED_ENTRY_EVENT_SELECT));
}

static int find_filter_index(u64 *events, u64 nevents, u64 key)
{
	u64 *fe = bsearch(&key, events, nevents, sizeof(events[0]),
			  filter_event_cmp);

	if (!fe)
		return -1;

	return fe - events;
}

static bool is_filter_entry_match(u64 filter_event, u64 umask)
{
	u64 mask = filter_event >> (KVM_PMU_MASKED_ENTRY_UMASK_MASK_SHIFT - 8);
	u64 match = filter_event & KVM_PMU_MASKED_ENTRY_UMASK_MATCH;

	BUILD_BUG_ON((KVM_PMU_ENCODE_MASKED_ENTRY(0, 0xff, 0, false) >>
		     (KVM_PMU_MASKED_ENTRY_UMASK_MASK_SHIFT - 8)) !=
		     ARCH_PERFMON_EVENTSEL_UMASK);

	return (umask & mask) == match;
}

static bool filter_contains_match(u64 *events, u64 nevents, u64 eventsel)
{
	u64 event_select = eventsel & kvm_pmu_ops.EVENTSEL_EVENT;
	u64 umask = eventsel & ARCH_PERFMON_EVENTSEL_UMASK;
	int i, index;

	index = find_filter_index(events, nevents, event_select);
	if (index < 0)
		return false;

	/*
	 * Entries are sorted by the event select.  Walk the list in both
	 * directions to process all entries with the targeted event select.
	 */
	for (i = index; i < nevents; i++) {
		if (filter_event_cmp(&events[i], &event_select))
			break;

		if (is_filter_entry_match(events[i], umask))
			return true;
	}

	for (i = index - 1; i >= 0; i--) {
		if (filter_event_cmp(&events[i], &event_select))
			break;

		if (is_filter_entry_match(events[i], umask))
			return true;
	}

	return false;
}

static bool is_gp_event_allowed(struct kvm_x86_pmu_event_filter *f,
				u64 eventsel)
{
	if (filter_contains_match(f->includes, f->nr_includes, eventsel) &&
	    !filter_contains_match(f->excludes, f->nr_excludes, eventsel))
		return f->action == KVM_PMU_EVENT_ALLOW;

	return f->action == KVM_PMU_EVENT_DENY;
}

static bool is_fixed_event_allowed(struct kvm_x86_pmu_event_filter *filter,
				   int idx)
{
	int fixed_idx = idx - KVM_FIXED_PMC_BASE_IDX;

	if (filter->action == KVM_PMU_EVENT_DENY &&
	    test_bit(fixed_idx, (ulong *)&filter->fixed_counter_bitmap))
		return false;
	if (filter->action == KVM_PMU_EVENT_ALLOW &&
	    !test_bit(fixed_idx, (ulong *)&filter->fixed_counter_bitmap))
		return false;

	return true;
}

static bool check_pmu_event_filter(struct kvm_pmc *pmc)
{
	struct kvm_x86_pmu_event_filter *filter;
	struct kvm *kvm = pmc->vcpu->kvm;

	filter = srcu_dereference(kvm->arch.pmu_event_filter, &kvm->srcu);
	if (!filter)
		return true;

	if (pmc_is_gp(pmc))
		return is_gp_event_allowed(filter, pmc->eventsel);

	return is_fixed_event_allowed(filter, pmc->idx);
}

static bool pmc_event_is_allowed(struct kvm_pmc *pmc)
{
	return pmc_is_globally_enabled(pmc) && pmc_speculative_in_use(pmc) &&
	       check_pmu_event_filter(pmc);
}

static int reprogram_counter(struct kvm_pmc *pmc)
{
	struct kvm_pmu *pmu = pmc_to_pmu(pmc);
	u64 eventsel = pmc->eventsel;
	u64 new_config = eventsel;
	bool emulate_overflow;
	u8 fixed_ctr_ctrl;

	emulate_overflow = pmc_pause_counter(pmc);

	if (!pmc_event_is_allowed(pmc))
		return 0;

	if (emulate_overflow)
		__kvm_perf_overflow(pmc, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (eventsel & ARCH_PERFMON_EVENTSEL_PIN_CONTROL)
		printk_once("kvm pmu: pin control bit is ignored\n");

<<<<<<< HEAD
	pmc->eventsel = eventsel;

	stop_counter(pmc);

	if (!(eventsel & ARCH_PERFMON_EVENTSEL_ENABLE) || !pmc_enabled(pmc))
		return;

	event_select = eventsel & ARCH_PERFMON_EVENTSEL_EVENT;
	unit_mask = (eventsel & ARCH_PERFMON_EVENTSEL_UMASK) >> 8;

	if (!(eventsel & (ARCH_PERFMON_EVENTSEL_EDGE |
				ARCH_PERFMON_EVENTSEL_INV |
				ARCH_PERFMON_EVENTSEL_CMASK))) {
		config = find_arch_event(&pmc->vcpu->arch.pmu, event_select,
				unit_mask);
		if (config != PERF_COUNT_HW_MAX)
			type = PERF_TYPE_HARDWARE;
	}

	if (type == PERF_TYPE_RAW)
		config = eventsel & X86_RAW_EVENT_MASK;

	reprogram_counter(pmc, type, config,
			!(eventsel & ARCH_PERFMON_EVENTSEL_USR),
			!(eventsel & ARCH_PERFMON_EVENTSEL_OS),
			eventsel & ARCH_PERFMON_EVENTSEL_INT);
}

static void reprogram_fixed_counter(struct kvm_pmc *pmc, u8 en_pmi, int idx)
{
	unsigned en = en_pmi & 0x3;
	bool pmi = en_pmi & 0x8;

	stop_counter(pmc);

	if (!en || !pmc_enabled(pmc))
		return;

	reprogram_counter(pmc, PERF_TYPE_HARDWARE,
			arch_events[fixed_pmc_events[idx]].event_type,
			!(en & 0x2), /* exclude user */
			!(en & 0x1), /* exclude kernel */
			pmi);
}

static inline u8 fixed_en_pmi(u64 ctrl, int idx)
{
	return (ctrl >> (idx * 4)) & 0xf;
}

static void reprogram_fixed_counters(struct kvm_pmu *pmu, u64 data)
{
	int i;

	for (i = 0; i < pmu->nr_arch_fixed_counters; i++) {
		u8 en_pmi = fixed_en_pmi(data, i);
		struct kvm_pmc *pmc = get_fixed_pmc_idx(pmu, i);

		if (fixed_en_pmi(pmu->fixed_ctr_ctrl, i) == en_pmi)
			continue;

		reprogram_fixed_counter(pmc, en_pmi, i);
	}

	pmu->fixed_ctr_ctrl = data;
}

static void reprogram_idx(struct kvm_pmu *pmu, int idx)
{
	struct kvm_pmc *pmc = global_idx_to_pmc(pmu, idx);

	if (!pmc)
		return;

	if (pmc_is_gp(pmc))
		reprogram_gp_counter(pmc, pmc->eventsel);
	else {
		int fidx = idx - X86_PMC_IDX_FIXED;
		reprogram_fixed_counter(pmc,
				fixed_en_pmi(pmu->fixed_ctr_ctrl, fidx), fidx);
	}
}

static void global_ctrl_changed(struct kvm_pmu *pmu, u64 data)
{
	int bit;
	u64 diff = pmu->global_ctrl ^ data;

	pmu->global_ctrl = data;

	for_each_set_bit(bit, (unsigned long *)&diff, X86_PMC_IDX_MAX)
		reprogram_idx(pmu, bit);
}

bool kvm_pmu_msr(struct kvm_vcpu *vcpu, u32 msr)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	int ret;

	switch (msr) {
	case MSR_CORE_PERF_FIXED_CTR_CTRL:
	case MSR_CORE_PERF_GLOBAL_STATUS:
	case MSR_CORE_PERF_GLOBAL_CTRL:
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		ret = pmu->version > 1;
		break;
	default:
		ret = get_gp_pmc(pmu, msr, MSR_IA32_PERFCTR0)
			|| get_gp_pmc(pmu, msr, MSR_P6_EVNTSEL0)
			|| get_fixed_pmc(pmu, msr);
		break;
	}
	return ret;
}

int kvm_pmu_get_msr(struct kvm_vcpu *vcpu, u32 index, u64 *data)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	struct kvm_pmc *pmc;

	switch (index) {
	case MSR_CORE_PERF_FIXED_CTR_CTRL:
		*data = pmu->fixed_ctr_ctrl;
		return 0;
	case MSR_CORE_PERF_GLOBAL_STATUS:
		*data = pmu->global_status;
		return 0;
	case MSR_CORE_PERF_GLOBAL_CTRL:
		*data = pmu->global_ctrl;
		return 0;
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		*data = pmu->global_ovf_ctrl;
		return 0;
	default:
		if ((pmc = get_gp_pmc(pmu, index, MSR_IA32_PERFCTR0)) ||
				(pmc = get_fixed_pmc(pmu, index))) {
			*data = read_pmc(pmc);
			return 0;
		} else if ((pmc = get_gp_pmc(pmu, index, MSR_P6_EVNTSEL0))) {
			*data = pmc->eventsel;
			return 0;
		}
	}
	return 1;
}

int kvm_pmu_set_msr(struct kvm_vcpu *vcpu, u32 index, u64 data)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	struct kvm_pmc *pmc;

	switch (index) {
	case MSR_CORE_PERF_FIXED_CTR_CTRL:
		if (pmu->fixed_ctr_ctrl == data)
			return 0;
		if (!(data & 0xfffffffffffff444ull)) {
			reprogram_fixed_counters(pmu, data);
			return 0;
		}
		break;
	case MSR_CORE_PERF_GLOBAL_STATUS:
		break; /* RO MSR */
	case MSR_CORE_PERF_GLOBAL_CTRL:
		if (pmu->global_ctrl == data)
			return 0;
		if (!(data & pmu->global_ctrl_mask)) {
			global_ctrl_changed(pmu, data);
			return 0;
		}
		break;
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		if (!(data & (pmu->global_ctrl_mask & ~(3ull<<62)))) {
			pmu->global_status &= ~data;
			pmu->global_ovf_ctrl = data;
			return 0;
		}
		break;
	default:
		if ((pmc = get_gp_pmc(pmu, index, MSR_IA32_PERFCTR0)) ||
				(pmc = get_fixed_pmc(pmu, index))) {
			data = (s64)(s32)data;
			pmc->counter += data - read_pmc(pmc);
			return 0;
		} else if ((pmc = get_gp_pmc(pmu, index, MSR_P6_EVNTSEL0))) {
			if (data == pmc->eventsel)
				return 0;
			if (!(data & 0xffffffff00200000ull)) {
				reprogram_gp_counter(pmc, data);
				return 0;
			}
		}
	}
	return 1;
}

int kvm_pmu_read_pmc(struct kvm_vcpu *vcpu, unsigned pmc, u64 *data)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	bool fast_mode = pmc & (1u << 31);
	bool fixed = pmc & (1u << 30);
	struct kvm_pmc *counters;
	u64 ctr;

	pmc &= ~(3u << 30);
	if (!fixed && pmc >= pmu->nr_arch_gp_counters)
		return 1;
	if (fixed && pmc >= pmu->nr_arch_fixed_counters)
		return 1;
	counters = fixed ? pmu->fixed_counters : pmu->gp_counters;
	ctr = read_pmc(&counters[pmc]);
	if (fast_mode)
		ctr = (u32)ctr;
	*data = ctr;
=======
	if (pmc_is_fixed(pmc)) {
		fixed_ctr_ctrl = fixed_ctrl_field(pmu->fixed_ctr_ctrl,
						  pmc->idx - KVM_FIXED_PMC_BASE_IDX);
		if (fixed_ctr_ctrl & 0x1)
			eventsel |= ARCH_PERFMON_EVENTSEL_OS;
		if (fixed_ctr_ctrl & 0x2)
			eventsel |= ARCH_PERFMON_EVENTSEL_USR;
		if (fixed_ctr_ctrl & 0x8)
			eventsel |= ARCH_PERFMON_EVENTSEL_INT;
		new_config = (u64)fixed_ctr_ctrl;
	}

	if (pmc->current_config == new_config && pmc_resume_counter(pmc))
		return 0;

	pmc_release_perf_event(pmc);

	pmc->current_config = new_config;

	return pmc_reprogram_counter(pmc, PERF_TYPE_RAW,
				     (eventsel & pmu->raw_event_mask),
				     !(eventsel & ARCH_PERFMON_EVENTSEL_USR),
				     !(eventsel & ARCH_PERFMON_EVENTSEL_OS),
				     eventsel & ARCH_PERFMON_EVENTSEL_INT);
}

void kvm_pmu_handle_event(struct kvm_vcpu *vcpu)
{
	DECLARE_BITMAP(bitmap, X86_PMC_IDX_MAX);
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc;
	int bit;

	bitmap_copy(bitmap, pmu->reprogram_pmi, X86_PMC_IDX_MAX);

	/*
	 * The reprogramming bitmap can be written asynchronously by something
	 * other than the task that holds vcpu->mutex, take care to clear only
	 * the bits that will actually processed.
	 */
	BUILD_BUG_ON(sizeof(bitmap) != sizeof(atomic64_t));
	atomic64_andnot(*(s64 *)bitmap, &pmu->__reprogram_pmi);

	kvm_for_each_pmc(pmu, pmc, bit, bitmap) {
		/*
		 * If reprogramming fails, e.g. due to contention, re-set the
		 * regprogram bit set, i.e. opportunistically try again on the
		 * next PMU refresh.  Don't make a new request as doing so can
		 * stall the guest if reprogramming repeatedly fails.
		 */
		if (reprogram_counter(pmc))
			set_bit(pmc->idx, pmu->reprogram_pmi);
	}

	/*
	 * Unused perf_events are only released if the corresponding MSRs
	 * weren't accessed during the last vCPU time slice. kvm_arch_sched_in
	 * triggers KVM_REQ_PMU if cleanup is needed.
	 */
	if (unlikely(pmu->need_cleanup))
		kvm_pmu_cleanup(vcpu);
}

int kvm_pmu_check_rdpmc_early(struct kvm_vcpu *vcpu, unsigned int idx)
{
	/*
	 * On Intel, VMX interception has priority over RDPMC exceptions that
	 * aren't already handled by the emulator, i.e. there are no additional
	 * check needed for Intel PMUs.
	 *
	 * On AMD, _all_ exceptions on RDPMC have priority over SVM intercepts,
	 * i.e. an invalid PMC results in a #GP, not #VMEXIT.
	 */
	if (!kvm_pmu_ops.check_rdpmc_early)
		return 0;

	return static_call(kvm_x86_pmu_check_rdpmc_early)(vcpu, idx);
}

bool is_vmware_backdoor_pmc(u32 pmc_idx)
{
	switch (pmc_idx) {
	case VMWARE_BACKDOOR_PMC_HOST_TSC:
	case VMWARE_BACKDOOR_PMC_REAL_TIME:
	case VMWARE_BACKDOOR_PMC_APPARENT_TIME:
		return true;
	}
	return false;
}

static int kvm_pmu_rdpmc_vmware(struct kvm_vcpu *vcpu, unsigned idx, u64 *data)
{
	u64 ctr_val;

	switch (idx) {
	case VMWARE_BACKDOOR_PMC_HOST_TSC:
		ctr_val = rdtsc();
		break;
	case VMWARE_BACKDOOR_PMC_REAL_TIME:
		ctr_val = ktime_get_boottime_ns();
		break;
	case VMWARE_BACKDOOR_PMC_APPARENT_TIME:
		ctr_val = ktime_get_boottime_ns() +
			vcpu->kvm->arch.kvmclock_offset;
		break;
	default:
		return 1;
	}

	*data = ctr_val;
	return 0;
}

int kvm_pmu_rdpmc(struct kvm_vcpu *vcpu, unsigned idx, u64 *data)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc;
	u64 mask = ~0ull;

	if (!pmu->version)
		return 1;

	if (is_vmware_backdoor_pmc(idx))
		return kvm_pmu_rdpmc_vmware(vcpu, idx, data);

	pmc = static_call(kvm_x86_pmu_rdpmc_ecx_to_pmc)(vcpu, idx, &mask);
	if (!pmc)
		return 1;

	if (!kvm_is_cr4_bit_set(vcpu, X86_CR4_PCE) &&
	    (static_call(kvm_x86_get_cpl)(vcpu) != 0) &&
	    kvm_is_cr0_bit_set(vcpu, X86_CR0_PE))
		return 1;

	*data = pmc_read_counter(pmc) & mask;
	return 0;
}

void kvm_pmu_deliver_pmi(struct kvm_vcpu *vcpu)
{
	if (lapic_in_kernel(vcpu)) {
		static_call_cond(kvm_x86_pmu_deliver_pmi)(vcpu);
		kvm_apic_local_deliver(vcpu->arch.apic, APIC_LVTPC);
	}
}

bool kvm_pmu_is_valid_msr(struct kvm_vcpu *vcpu, u32 msr)
{
	switch (msr) {
	case MSR_CORE_PERF_GLOBAL_STATUS:
	case MSR_CORE_PERF_GLOBAL_CTRL:
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		return kvm_pmu_has_perf_global_ctrl(vcpu_to_pmu(vcpu));
	default:
		break;
	}
	return static_call(kvm_x86_pmu_msr_idx_to_pmc)(vcpu, msr) ||
		static_call(kvm_x86_pmu_is_valid_msr)(vcpu, msr);
}

static void kvm_pmu_mark_pmc_in_use(struct kvm_vcpu *vcpu, u32 msr)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc = static_call(kvm_x86_pmu_msr_idx_to_pmc)(vcpu, msr);

	if (pmc)
		__set_bit(pmc->idx, pmu->pmc_in_use);
}

int kvm_pmu_get_msr(struct kvm_vcpu *vcpu, struct msr_data *msr_info)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	u32 msr = msr_info->index;

	switch (msr) {
	case MSR_CORE_PERF_GLOBAL_STATUS:
	case MSR_AMD64_PERF_CNTR_GLOBAL_STATUS:
		msr_info->data = pmu->global_status;
		break;
	case MSR_AMD64_PERF_CNTR_GLOBAL_CTL:
	case MSR_CORE_PERF_GLOBAL_CTRL:
		msr_info->data = pmu->global_ctrl;
		break;
	case MSR_AMD64_PERF_CNTR_GLOBAL_STATUS_CLR:
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		msr_info->data = 0;
		break;
	default:
		return static_call(kvm_x86_pmu_get_msr)(vcpu, msr_info);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
void kvm_pmu_cpuid_update(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	struct kvm_cpuid_entry2 *entry;
	unsigned bitmap_len;

=======
int kvm_pmu_set_msr(struct kvm_vcpu *vcpu, struct msr_data *msr_info)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	u32 msr = msr_info->index;
	u64 data = msr_info->data;
	u64 diff;

	/*
	 * Note, AMD ignores writes to reserved bits and read-only PMU MSRs,
	 * whereas Intel generates #GP on attempts to write reserved/RO MSRs.
	 */
	switch (msr) {
	case MSR_CORE_PERF_GLOBAL_STATUS:
		if (!msr_info->host_initiated)
			return 1; /* RO MSR */
		fallthrough;
	case MSR_AMD64_PERF_CNTR_GLOBAL_STATUS:
		/* Per PPR, Read-only MSR. Writes are ignored. */
		if (!msr_info->host_initiated)
			break;

		if (data & pmu->global_status_mask)
			return 1;

		pmu->global_status = data;
		break;
	case MSR_AMD64_PERF_CNTR_GLOBAL_CTL:
		data &= ~pmu->global_ctrl_mask;
		fallthrough;
	case MSR_CORE_PERF_GLOBAL_CTRL:
		if (!kvm_valid_perf_global_ctrl(pmu, data))
			return 1;

		if (pmu->global_ctrl != data) {
			diff = pmu->global_ctrl ^ data;
			pmu->global_ctrl = data;
			reprogram_counters(pmu, diff);
		}
		break;
	case MSR_CORE_PERF_GLOBAL_OVF_CTRL:
		/*
		 * GLOBAL_OVF_CTRL, a.k.a. GLOBAL STATUS_RESET, clears bits in
		 * GLOBAL_STATUS, and so the set of reserved bits is the same.
		 */
		if (data & pmu->global_status_mask)
			return 1;
		fallthrough;
	case MSR_AMD64_PERF_CNTR_GLOBAL_STATUS_CLR:
		if (!msr_info->host_initiated)
			pmu->global_status &= ~data;
		break;
	default:
		kvm_pmu_mark_pmc_in_use(vcpu, msr_info->index);
		return static_call(kvm_x86_pmu_set_msr)(vcpu, msr_info);
	}

	return 0;
}

static void kvm_pmu_reset(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc;
	int i;

	pmu->need_cleanup = false;

	bitmap_zero(pmu->reprogram_pmi, X86_PMC_IDX_MAX);

	kvm_for_each_pmc(pmu, pmc, i, pmu->all_valid_pmc_idx) {
		pmc_stop_counter(pmc);
		pmc->counter = 0;
		pmc->emulated_counter = 0;

		if (pmc_is_gp(pmc))
			pmc->eventsel = 0;
	}

	pmu->fixed_ctr_ctrl = pmu->global_ctrl = pmu->global_status = 0;

	static_call_cond(kvm_x86_pmu_reset)(vcpu);
}


/*
 * Refresh the PMU configuration for the vCPU, e.g. if userspace changes CPUID
 * and/or PERF_CAPABILITIES.
 */
void kvm_pmu_refresh(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);

	if (KVM_BUG_ON(kvm_vcpu_has_run(vcpu), vcpu->kvm))
		return;

	/*
	 * Stop/release all existing counters/events before realizing the new
	 * vPMU model.
	 */
	kvm_pmu_reset(vcpu);

	pmu->version = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pmu->nr_arch_gp_counters = 0;
	pmu->nr_arch_fixed_counters = 0;
	pmu->counter_bitmask[KVM_PMC_GP] = 0;
	pmu->counter_bitmask[KVM_PMC_FIXED] = 0;
<<<<<<< HEAD
	pmu->version = 0;

	entry = kvm_find_cpuid_entry(vcpu, 0xa, 0);
	if (!entry)
		return;

	pmu->version = entry->eax & 0xff;
	if (!pmu->version)
		return;

	pmu->nr_arch_gp_counters = min((int)(entry->eax >> 8) & 0xff,
			X86_PMC_MAX_GENERIC);
	pmu->counter_bitmask[KVM_PMC_GP] =
		((u64)1 << ((entry->eax >> 16) & 0xff)) - 1;
	bitmap_len = (entry->eax >> 24) & 0xff;
	pmu->available_event_types = ~entry->ebx & ((1ull << bitmap_len) - 1);

	if (pmu->version == 1) {
		pmu->nr_arch_fixed_counters = 0;
	} else {
		pmu->nr_arch_fixed_counters = min((int)(entry->edx & 0x1f),
				X86_PMC_MAX_FIXED);
		pmu->counter_bitmask[KVM_PMC_FIXED] =
			((u64)1 << ((entry->edx >> 5) & 0xff)) - 1;
	}

	pmu->global_ctrl = ((1 << pmu->nr_arch_gp_counters) - 1) |
		(((1ull << pmu->nr_arch_fixed_counters) - 1) << X86_PMC_IDX_FIXED);
	pmu->global_ctrl_mask = ~pmu->global_ctrl;
=======
	pmu->reserved_bits = 0xffffffff00200000ull;
	pmu->raw_event_mask = X86_RAW_EVENT_MASK;
	pmu->global_ctrl_mask = ~0ull;
	pmu->global_status_mask = ~0ull;
	pmu->fixed_ctr_ctrl_mask = ~0ull;
	pmu->pebs_enable_mask = ~0ull;
	pmu->pebs_data_cfg_mask = ~0ull;
	bitmap_zero(pmu->all_valid_pmc_idx, X86_PMC_IDX_MAX);

	if (!vcpu->kvm->arch.enable_pmu)
		return;

	static_call(kvm_x86_pmu_refresh)(vcpu);

	/*
	 * At RESET, both Intel and AMD CPUs set all enable bits for general
	 * purpose counters in IA32_PERF_GLOBAL_CTRL (so that software that
	 * was written for v1 PMUs don't unknowingly leave GP counters disabled
	 * in the global controls).  Emulate that behavior when refreshing the
	 * PMU so that userspace doesn't need to manually set PERF_GLOBAL_CTRL.
	 */
	if (kvm_pmu_has_perf_global_ctrl(pmu) && pmu->nr_arch_gp_counters)
		pmu->global_ctrl = GENMASK_ULL(pmu->nr_arch_gp_counters - 1, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void kvm_pmu_init(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	int i;
	struct kvm_pmu *pmu = &vcpu->arch.pmu;

	memset(pmu, 0, sizeof(*pmu));
	for (i = 0; i < X86_PMC_MAX_GENERIC; i++) {
		pmu->gp_counters[i].type = KVM_PMC_GP;
		pmu->gp_counters[i].vcpu = vcpu;
		pmu->gp_counters[i].idx = i;
	}
	for (i = 0; i < X86_PMC_MAX_FIXED; i++) {
		pmu->fixed_counters[i].type = KVM_PMC_FIXED;
		pmu->fixed_counters[i].vcpu = vcpu;
		pmu->fixed_counters[i].idx = i + X86_PMC_IDX_FIXED;
	}
	init_irq_work(&pmu->irq_work, trigger_pmi);
	kvm_pmu_cpuid_update(vcpu);
}

void kvm_pmu_reset(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	int i;

	irq_work_sync(&pmu->irq_work);
	for (i = 0; i < X86_PMC_MAX_GENERIC; i++) {
		struct kvm_pmc *pmc = &pmu->gp_counters[i];
		stop_counter(pmc);
		pmc->counter = pmc->eventsel = 0;
	}

	for (i = 0; i < X86_PMC_MAX_FIXED; i++)
		stop_counter(&pmu->fixed_counters[i]);

	pmu->fixed_ctr_ctrl = pmu->global_ctrl = pmu->global_status =
		pmu->global_ovf_ctrl = 0;
=======
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);

	memset(pmu, 0, sizeof(*pmu));
	static_call(kvm_x86_pmu_init)(vcpu);
	kvm_pmu_refresh(vcpu);
}

/* Release perf_events for vPMCs that have been unused for a full time slice.  */
void kvm_pmu_cleanup(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc = NULL;
	DECLARE_BITMAP(bitmask, X86_PMC_IDX_MAX);
	int i;

	pmu->need_cleanup = false;

	bitmap_andnot(bitmask, pmu->all_valid_pmc_idx,
		      pmu->pmc_in_use, X86_PMC_IDX_MAX);

	kvm_for_each_pmc(pmu, pmc, i, bitmask) {
		if (pmc->perf_event && !pmc_speculative_in_use(pmc))
			pmc_stop_counter(pmc);
	}

	static_call_cond(kvm_x86_pmu_cleanup)(vcpu);

	bitmap_zero(pmu->pmc_in_use, X86_PMC_IDX_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void kvm_pmu_destroy(struct kvm_vcpu *vcpu)
{
	kvm_pmu_reset(vcpu);
}

<<<<<<< HEAD
void kvm_handle_pmu_event(struct kvm_vcpu *vcpu)
{
	struct kvm_pmu *pmu = &vcpu->arch.pmu;
	u64 bitmask;
	int bit;

	bitmask = pmu->reprogram_pmi;

	for_each_set_bit(bit, (unsigned long *)&bitmask, X86_PMC_IDX_MAX) {
		struct kvm_pmc *pmc = global_idx_to_pmc(pmu, bit);

		if (unlikely(!pmc || !pmc->perf_event)) {
			clear_bit(bit, (unsigned long *)&pmu->reprogram_pmi);
			continue;
		}

		reprogram_idx(pmu, bit);
	}
}
=======
static void kvm_pmu_incr_counter(struct kvm_pmc *pmc)
{
	pmc->emulated_counter++;
	kvm_pmu_request_counter_reprogram(pmc);
}

static inline bool cpl_is_matched(struct kvm_pmc *pmc)
{
	bool select_os, select_user;
	u64 config;

	if (pmc_is_gp(pmc)) {
		config = pmc->eventsel;
		select_os = config & ARCH_PERFMON_EVENTSEL_OS;
		select_user = config & ARCH_PERFMON_EVENTSEL_USR;
	} else {
		config = fixed_ctrl_field(pmc_to_pmu(pmc)->fixed_ctr_ctrl,
					  pmc->idx - KVM_FIXED_PMC_BASE_IDX);
		select_os = config & 0x1;
		select_user = config & 0x2;
	}

	/*
	 * Skip the CPL lookup, which isn't free on Intel, if the result will
	 * be the same regardless of the CPL.
	 */
	if (select_os == select_user)
		return select_os;

	return (static_call(kvm_x86_get_cpl)(pmc->vcpu) == 0) ? select_os : select_user;
}

void kvm_pmu_trigger_event(struct kvm_vcpu *vcpu, u64 eventsel)
{
	DECLARE_BITMAP(bitmap, X86_PMC_IDX_MAX);
	struct kvm_pmu *pmu = vcpu_to_pmu(vcpu);
	struct kvm_pmc *pmc;
	int i;

	BUILD_BUG_ON(sizeof(pmu->global_ctrl) * BITS_PER_BYTE != X86_PMC_IDX_MAX);

	if (!kvm_pmu_has_perf_global_ctrl(pmu))
		bitmap_copy(bitmap, pmu->all_valid_pmc_idx, X86_PMC_IDX_MAX);
	else if (!bitmap_and(bitmap, pmu->all_valid_pmc_idx,
			     (unsigned long *)&pmu->global_ctrl, X86_PMC_IDX_MAX))
		return;

	kvm_for_each_pmc(pmu, pmc, i, bitmap) {
		/*
		 * Ignore checks for edge detect (all events currently emulated
		 * but KVM are always rising edges), pin control (unsupported
		 * by modern CPUs), and counter mask and its invert flag (KVM
		 * doesn't emulate multiple events in a single clock cycle).
		 *
		 * Note, the uppermost nibble of AMD's mask overlaps Intel's
		 * IN_TX (bit 32) and IN_TXCP (bit 33), as well as two reserved
		 * bits (bits 35:34).  Checking the "in HLE/RTM transaction"
		 * flags is correct as the vCPU can't be in a transaction if
		 * KVM is emulating an instruction.  Checking the reserved bits
		 * might be wrong if they are defined in the future, but so
		 * could ignoring them, so do the simple thing for now.
		 */
		if (((pmc->eventsel ^ eventsel) & AMD64_RAW_EVENT_MASK_NB) ||
		    !pmc_event_is_allowed(pmc) || !cpl_is_matched(pmc))
			continue;

		kvm_pmu_incr_counter(pmc);
	}
}
EXPORT_SYMBOL_GPL(kvm_pmu_trigger_event);

static bool is_masked_filter_valid(const struct kvm_x86_pmu_event_filter *filter)
{
	u64 mask = kvm_pmu_ops.EVENTSEL_EVENT |
		   KVM_PMU_MASKED_ENTRY_UMASK_MASK |
		   KVM_PMU_MASKED_ENTRY_UMASK_MATCH |
		   KVM_PMU_MASKED_ENTRY_EXCLUDE;
	int i;

	for (i = 0; i < filter->nevents; i++) {
		if (filter->events[i] & ~mask)
			return false;
	}

	return true;
}

static void convert_to_masked_filter(struct kvm_x86_pmu_event_filter *filter)
{
	int i, j;

	for (i = 0, j = 0; i < filter->nevents; i++) {
		/*
		 * Skip events that are impossible to match against a guest
		 * event.  When filtering, only the event select + unit mask
		 * of the guest event is used.  To maintain backwards
		 * compatibility, impossible filters can't be rejected :-(
		 */
		if (filter->events[i] & ~(kvm_pmu_ops.EVENTSEL_EVENT |
					  ARCH_PERFMON_EVENTSEL_UMASK))
			continue;
		/*
		 * Convert userspace events to a common in-kernel event so
		 * only one code path is needed to support both events.  For
		 * the in-kernel events use masked events because they are
		 * flexible enough to handle both cases.  To convert to masked
		 * events all that's needed is to add an "all ones" umask_mask,
		 * (unmasked filter events don't support EXCLUDE).
		 */
		filter->events[j++] = filter->events[i] |
				      (0xFFULL << KVM_PMU_MASKED_ENTRY_UMASK_MASK_SHIFT);
	}

	filter->nevents = j;
}

static int prepare_filter_lists(struct kvm_x86_pmu_event_filter *filter)
{
	int i;

	if (!(filter->flags & KVM_PMU_EVENT_FLAG_MASKED_EVENTS))
		convert_to_masked_filter(filter);
	else if (!is_masked_filter_valid(filter))
		return -EINVAL;

	/*
	 * Sort entries by event select and includes vs. excludes so that all
	 * entries for a given event select can be processed efficiently during
	 * filtering.  The EXCLUDE flag uses a more significant bit than the
	 * event select, and so the sorted list is also effectively split into
	 * includes and excludes sub-lists.
	 */
	sort(&filter->events, filter->nevents, sizeof(filter->events[0]),
	     filter_sort_cmp, NULL);

	i = filter->nevents;
	/* Find the first EXCLUDE event (only supported for masked events). */
	if (filter->flags & KVM_PMU_EVENT_FLAG_MASKED_EVENTS) {
		for (i = 0; i < filter->nevents; i++) {
			if (filter->events[i] & KVM_PMU_MASKED_ENTRY_EXCLUDE)
				break;
		}
	}

	filter->nr_includes = i;
	filter->nr_excludes = filter->nevents - filter->nr_includes;
	filter->includes = filter->events;
	filter->excludes = filter->events + filter->nr_includes;

	return 0;
}

int kvm_vm_ioctl_set_pmu_event_filter(struct kvm *kvm, void __user *argp)
{
	struct kvm_pmu_event_filter __user *user_filter = argp;
	struct kvm_x86_pmu_event_filter *filter;
	struct kvm_pmu_event_filter tmp;
	struct kvm_vcpu *vcpu;
	unsigned long i;
	size_t size;
	int r;

	if (copy_from_user(&tmp, user_filter, sizeof(tmp)))
		return -EFAULT;

	if (tmp.action != KVM_PMU_EVENT_ALLOW &&
	    tmp.action != KVM_PMU_EVENT_DENY)
		return -EINVAL;

	if (tmp.flags & ~KVM_PMU_EVENT_FLAGS_VALID_MASK)
		return -EINVAL;

	if (tmp.nevents > KVM_PMU_EVENT_FILTER_MAX_EVENTS)
		return -E2BIG;

	size = struct_size(filter, events, tmp.nevents);
	filter = kzalloc(size, GFP_KERNEL_ACCOUNT);
	if (!filter)
		return -ENOMEM;

	filter->action = tmp.action;
	filter->nevents = tmp.nevents;
	filter->fixed_counter_bitmap = tmp.fixed_counter_bitmap;
	filter->flags = tmp.flags;

	r = -EFAULT;
	if (copy_from_user(filter->events, user_filter->events,
			   sizeof(filter->events[0]) * filter->nevents))
		goto cleanup;

	r = prepare_filter_lists(filter);
	if (r)
		goto cleanup;

	mutex_lock(&kvm->lock);
	filter = rcu_replace_pointer(kvm->arch.pmu_event_filter, filter,
				     mutex_is_locked(&kvm->lock));
	mutex_unlock(&kvm->lock);
	synchronize_srcu_expedited(&kvm->srcu);

	BUILD_BUG_ON(sizeof(((struct kvm_pmu *)0)->reprogram_pmi) >
		     sizeof(((struct kvm_pmu *)0)->__reprogram_pmi));

	kvm_for_each_vcpu(i, vcpu, kvm)
		atomic64_set(&vcpu_to_pmu(vcpu)->__reprogram_pmi, -1ull);

	kvm_make_all_cpus_request(kvm, KVM_REQ_PMU);

	r = 0;
cleanup:
	kfree(filter);
	return r;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
