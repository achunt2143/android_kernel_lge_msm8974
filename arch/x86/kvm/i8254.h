<<<<<<< HEAD
#ifndef __I8254_H
#define __I8254_H

#include "iodev.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __I8254_H
#define __I8254_H

#include <linux/kthread.h>

#include <kvm/iodev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct kvm_kpit_channel_state {
	u32 count; /* can be 65536 */
	u16 latched_count;
	u8 count_latched;
	u8 status_latched;
	u8 status;
	u8 read_state;
	u8 write_state;
	u8 write_latch;
	u8 rw_mode;
	u8 mode;
	u8 bcd; /* not supported */
	u8 gate; /* timer start */
	ktime_t count_load_time;
};

struct kvm_kpit_state {
<<<<<<< HEAD
	struct kvm_kpit_channel_state channels[3];
	u32 flags;
	struct kvm_timer pit_timer;
	bool is_periodic;
	u32    speaker_data_on;
	struct mutex lock;
	struct kvm_pit *pit;
	spinlock_t inject_lock;
	unsigned long irq_ack;
=======
	/* All members before "struct mutex lock" are protected by the lock. */
	struct kvm_kpit_channel_state channels[3];
	u32 flags;
	bool is_periodic;
	s64 period; 				/* unit: ns */
	struct hrtimer timer;

	struct mutex lock;
	atomic_t reinject;
	atomic_t pending; /* accumulated triggered timers */
	atomic_t irq_ack;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct kvm_irq_ack_notifier irq_ack_notifier;
};

struct kvm_pit {
	struct kvm_io_device dev;
	struct kvm_io_device speaker_dev;
	struct kvm *kvm;
	struct kvm_kpit_state pit_state;
	int irq_source_id;
	struct kvm_irq_mask_notifier mask_notifier;
<<<<<<< HEAD
	struct workqueue_struct *wq;
	struct work_struct expired;
=======
	struct kthread_worker *worker;
	struct kthread_work expired;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define KVM_PIT_BASE_ADDRESS	    0x40
#define KVM_SPEAKER_BASE_ADDRESS    0x61
#define KVM_PIT_MEM_LENGTH	    4
#define KVM_PIT_FREQ		    1193181
#define KVM_MAX_PIT_INTR_INTERVAL   HZ / 100
#define KVM_PIT_CHANNEL_MASK	    0x3

<<<<<<< HEAD
void kvm_pit_load_count(struct kvm *kvm, int channel, u32 val, int hpet_legacy_start);
struct kvm_pit *kvm_create_pit(struct kvm *kvm, u32 flags);
void kvm_free_pit(struct kvm *kvm);
void kvm_pit_reset(struct kvm_pit *pit);
=======
struct kvm_pit *kvm_create_pit(struct kvm *kvm, u32 flags);
void kvm_free_pit(struct kvm *kvm);

void kvm_pit_load_count(struct kvm_pit *pit, int channel, u32 val,
		int hpet_legacy_start);
void kvm_pit_set_reinject(struct kvm_pit *pit, bool reinject);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
