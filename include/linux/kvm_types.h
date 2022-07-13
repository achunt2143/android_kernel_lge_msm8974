<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __KVM_TYPES_H__
#define __KVM_TYPES_H__

<<<<<<< HEAD
#include <asm/types.h>
=======
struct kvm;
struct kvm_async_pf;
struct kvm_device_ops;
struct kvm_gfn_range;
struct kvm_interrupt;
struct kvm_irq_routing_table;
struct kvm_memory_slot;
struct kvm_one_reg;
struct kvm_run;
struct kvm_userspace_memory_region;
struct kvm_vcpu;
struct kvm_vcpu_init;
struct kvm_memslots;

enum kvm_mr_change;

#include <linux/bits.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/spinlock_types.h>

#include <asm/kvm_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Address types:
 *
 *  gva - guest virtual address
 *  gpa - guest physical address
 *  gfn - guest frame number
 *  hva - host virtual address
 *  hpa - host physical address
 *  hfn - host frame number
 */

typedef unsigned long  gva_t;
typedef u64            gpa_t;
typedef u64            gfn_t;

<<<<<<< HEAD
=======
#define INVALID_GPA	(~(gpa_t)0)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef unsigned long  hva_t;
typedef u64            hpa_t;
typedef u64            hfn_t;

<<<<<<< HEAD
typedef hfn_t pfn_t;

union kvm_ioapic_redirect_entry {
	u64 bits;
	struct {
		u8 vector;
		u8 delivery_mode:3;
		u8 dest_mode:1;
		u8 delivery_status:1;
		u8 polarity:1;
		u8 remote_irr:1;
		u8 trig_mode:1;
		u8 mask:1;
		u8 reserve:7;
		u8 reserved[4];
		u8 dest_id;
	} fields;
};

struct kvm_lapic_irq {
	u32 vector;
	u32 delivery_mode;
	u32 dest_mode;
	u32 level;
	u32 trig_mode;
	u32 shorthand;
	u32 dest_id;
};
=======
typedef hfn_t kvm_pfn_t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct gfn_to_hva_cache {
	u64 generation;
	gpa_t gpa;
	unsigned long hva;
	unsigned long len;
	struct kvm_memory_slot *memslot;
};

<<<<<<< HEAD
=======
struct gfn_to_pfn_cache {
	u64 generation;
	gpa_t gpa;
	unsigned long uhva;
	struct kvm_memory_slot *memslot;
	struct kvm *kvm;
	struct list_head list;
	rwlock_t lock;
	struct mutex refresh_lock;
	void *khva;
	kvm_pfn_t pfn;
	bool active;
	bool valid;
};

#ifdef KVM_ARCH_NR_OBJS_PER_MEMORY_CACHE
/*
 * Memory caches are used to preallocate memory ahead of various MMU flows,
 * e.g. page fault handlers.  Gracefully handling allocation failures deep in
 * MMU flows is problematic, as is triggering reclaim, I/O, etc... while
 * holding MMU locks.  Note, these caches act more like prefetch buffers than
 * classical caches, i.e. objects are not returned to the cache on being freed.
 *
 * The @capacity field and @objects array are lazily initialized when the cache
 * is topped up (__kvm_mmu_topup_memory_cache()).
 */
struct kvm_mmu_memory_cache {
	gfp_t gfp_zero;
	gfp_t gfp_custom;
	struct kmem_cache *kmem_cache;
	int capacity;
	int nobjs;
	void **objects;
};
#endif

#define HALT_POLL_HIST_COUNT			32

struct kvm_vm_stat_generic {
	u64 remote_tlb_flush;
	u64 remote_tlb_flush_requests;
};

struct kvm_vcpu_stat_generic {
	u64 halt_successful_poll;
	u64 halt_attempted_poll;
	u64 halt_poll_invalid;
	u64 halt_wakeup;
	u64 halt_poll_success_ns;
	u64 halt_poll_fail_ns;
	u64 halt_wait_ns;
	u64 halt_poll_success_hist[HALT_POLL_HIST_COUNT];
	u64 halt_poll_fail_hist[HALT_POLL_HIST_COUNT];
	u64 halt_wait_hist[HALT_POLL_HIST_COUNT];
	u64 blocking;
};

#define KVM_STATS_NAME_SIZE	48

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __KVM_TYPES_H__ */
