<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 * Copyright (c) 2011, Microsoft Corporation.
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
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:
 *   Haiyang Zhang <haiyangz@microsoft.com>
 *   Hank Janssen  <hjanssen@microsoft.com>
 *   K. Y. Srinivasan <kys@microsoft.com>
<<<<<<< HEAD
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _HYPERV_VMBUS_H
#define _HYPERV_VMBUS_H

#include <linux/list.h>
<<<<<<< HEAD
#include <asm/sync_bitops.h>
#include <linux/atomic.h>
#include <linux/hyperv.h>

/*
 * The below CPUID leaves are present if VersionAndFeatures.HypervisorPresent
 * is set by CPUID(HVCPUID_VERSION_FEATURES).
 */
enum hv_cpuid_function {
	HVCPUID_VERSION_FEATURES		= 0x00000001,
	HVCPUID_VENDOR_MAXFUNCTION		= 0x40000000,
	HVCPUID_INTERFACE			= 0x40000001,

	/*
	 * The remaining functions depend on the value of
	 * HVCPUID_INTERFACE
	 */
	HVCPUID_VERSION			= 0x40000002,
	HVCPUID_FEATURES			= 0x40000003,
	HVCPUID_ENLIGHTENMENT_INFO	= 0x40000004,
	HVCPUID_IMPLEMENTATION_LIMITS		= 0x40000005,
};

/* Define version of the synthetic interrupt controller. */
#define HV_SYNIC_VERSION		(1)

/* Define the expected SynIC version. */
#define HV_SYNIC_VERSION_1		(0x1)

/* Define synthetic interrupt controller message constants. */
#define HV_MESSAGE_SIZE			(256)
#define HV_MESSAGE_PAYLOAD_BYTE_COUNT	(240)
#define HV_MESSAGE_PAYLOAD_QWORD_COUNT	(30)
#define HV_ANY_VP			(0xFFFFFFFF)

/* Define synthetic interrupt controller flag constants. */
#define HV_EVENT_FLAGS_COUNT		(256 * 8)
#define HV_EVENT_FLAGS_BYTE_COUNT	(256)
#define HV_EVENT_FLAGS_DWORD_COUNT	(256 / sizeof(u32))

/* Define hypervisor message types. */
enum hv_message_type {
	HVMSG_NONE			= 0x00000000,

	/* Memory access messages. */
	HVMSG_UNMAPPED_GPA		= 0x80000000,
	HVMSG_GPA_INTERCEPT		= 0x80000001,

	/* Timer notification messages. */
	HVMSG_TIMER_EXPIRED			= 0x80000010,

	/* Error messages. */
	HVMSG_INVALID_VP_REGISTER_VALUE	= 0x80000020,
	HVMSG_UNRECOVERABLE_EXCEPTION	= 0x80000021,
	HVMSG_UNSUPPORTED_FEATURE		= 0x80000022,

	/* Trace buffer complete messages. */
	HVMSG_EVENTLOG_BUFFERCOMPLETE	= 0x80000040,

	/* Platform-specific processor intercept messages. */
	HVMSG_X64_IOPORT_INTERCEPT		= 0x80010000,
	HVMSG_X64_MSR_INTERCEPT		= 0x80010001,
	HVMSG_X64_CPUID_INTERCEPT		= 0x80010002,
	HVMSG_X64_EXCEPTION_INTERCEPT	= 0x80010003,
	HVMSG_X64_APIC_EOI			= 0x80010004,
	HVMSG_X64_LEGACY_FP_ERROR		= 0x80010005
};

/* Define the number of synthetic interrupt sources. */
#define HV_SYNIC_SINT_COUNT		(16)
#define HV_SYNIC_STIMER_COUNT		(4)

/* Define invalid partition identifier. */
#define HV_PARTITION_ID_INVALID		((u64)0x0)

/* Define connection identifier type. */
union hv_connection_id {
	u32 asu32;
	struct {
		u32 id:24;
		u32 reserved:8;
	} u;
};

/* Define port identifier type. */
union hv_port_id {
	u32 asu32;
	struct {
		u32 id:24;
		u32 reserved:8;
	} u ;
};

/* Define port type. */
enum hv_port_type {
	HVPORT_MSG	= 1,
	HVPORT_EVENT		= 2,
	HVPORT_MONITOR	= 3
};

/* Define port information structure. */
struct hv_port_info {
	enum hv_port_type port_type;
	u32 padding;
	union {
		struct {
			u32 target_sint;
			u32 target_vp;
			u64 rsvdz;
		} message_port_info;
		struct {
			u32 target_sint;
			u32 target_vp;
			u16 base_flag_bumber;
			u16 flag_count;
			u32 rsvdz;
		} event_port_info;
		struct {
			u64 monitor_address;
			u64 rsvdz;
		} monitor_port_info;
	};
};

struct hv_connection_info {
	enum hv_port_type port_type;
	u32 padding;
	union {
		struct {
			u64 rsvdz;
		} message_connection_info;
		struct {
			u64 rsvdz;
		} event_connection_info;
		struct {
			u64 monitor_address;
		} monitor_connection_info;
	};
};

/* Define synthetic interrupt controller message flags. */
union hv_message_flags {
	u8 asu8;
	struct {
		u8 msg_pending:1;
		u8 reserved:7;
	};
};

/* Define synthetic interrupt controller message header. */
struct hv_message_header {
	enum hv_message_type message_type;
	u8 payload_size;
	union hv_message_flags message_flags;
	u8 reserved[2];
	union {
		u64 sender;
		union hv_port_id port;
	};
};

/* Define timer message payload structure. */
struct hv_timer_message_payload {
	u32 timer_index;
	u32 reserved;
	u64 expiration_time;	/* When the timer expired */
	u64 delivery_time;	/* When the message was delivered */
};

/* Define synthetic interrupt controller message format. */
struct hv_message {
	struct hv_message_header header;
	union {
		u64 payload[HV_MESSAGE_PAYLOAD_QWORD_COUNT];
	} u ;
};

/* Define the number of message buffers associated with each port. */
#define HV_PORT_MESSAGE_BUFFER_COUNT	(16)

/* Define the synthetic interrupt message page layout. */
struct hv_message_page {
	struct hv_message sint_message[HV_SYNIC_SINT_COUNT];
};

/* Define the synthetic interrupt controller event flags format. */
union hv_synic_event_flags {
	u8 flags8[HV_EVENT_FLAGS_BYTE_COUNT];
	u32 flags32[HV_EVENT_FLAGS_DWORD_COUNT];
};

/* Define the synthetic interrupt flags page layout. */
struct hv_synic_event_flags_page {
	union hv_synic_event_flags sintevent_flags[HV_SYNIC_SINT_COUNT];
};

/* Define SynIC control register. */
union hv_synic_scontrol {
	u64 as_uint64;
	struct {
		u64 enable:1;
		u64 reserved:63;
	};
};

/* Define synthetic interrupt source. */
union hv_synic_sint {
	u64 as_uint64;
	struct {
		u64 vector:8;
		u64 reserved1:8;
		u64 masked:1;
		u64 auto_eoi:1;
		u64 reserved2:46;
	};
};

/* Define the format of the SIMP register */
union hv_synic_simp {
	u64 as_uint64;
	struct {
		u64 simp_enabled:1;
		u64 preserved:11;
		u64 base_simp_gpa:52;
	};
};

/* Define the format of the SIEFP register */
union hv_synic_siefp {
	u64 as_uint64;
	struct {
		u64 siefp_enabled:1;
		u64 preserved:11;
		u64 base_siefp_gpa:52;
	};
};
=======
#include <linux/bitops.h>
#include <asm/sync_bitops.h>
#include <asm/hyperv-tlfs.h>
#include <linux/atomic.h>
#include <linux/hyperv.h>
#include <linux/interrupt.h>

#include "hv_trace.h"

/*
 * Timeout for services such as KVP and fcopy.
 */
#define HV_UTIL_TIMEOUT 30

/*
 * Timeout for guest-host handshake for services.
 */
#define HV_UTIL_NEGO_TIMEOUT 55

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Definitions for the monitored notification facility */
union hv_monitor_trigger_group {
	u64 as_uint64;
	struct {
		u32 pending;
		u32 armed;
	};
};

struct hv_monitor_parameter {
	union hv_connection_id connectionid;
	u16 flagnumber;
	u16 rsvdz;
};

union hv_monitor_trigger_state {
	u32 asu32;

	struct {
		u32 group_enable:4;
		u32 rsvdz:28;
	};
};

/* struct hv_monitor_page Layout */
/* ------------------------------------------------------ */
/* | 0   | TriggerState (4 bytes) | Rsvd1 (4 bytes)     | */
/* | 8   | TriggerGroup[0]                              | */
/* | 10  | TriggerGroup[1]                              | */
/* | 18  | TriggerGroup[2]                              | */
/* | 20  | TriggerGroup[3]                              | */
/* | 28  | Rsvd2[0]                                     | */
/* | 30  | Rsvd2[1]                                     | */
/* | 38  | Rsvd2[2]                                     | */
/* | 40  | NextCheckTime[0][0]    | NextCheckTime[0][1] | */
/* | ...                                                | */
/* | 240 | Latency[0][0..3]                             | */
/* | 340 | Rsvz3[0]                                     | */
/* | 440 | Parameter[0][0]                              | */
/* | 448 | Parameter[0][1]                              | */
/* | ...                                                | */
/* | 840 | Rsvd4[0]                                     | */
/* ------------------------------------------------------ */
struct hv_monitor_page {
	union hv_monitor_trigger_state trigger_state;
	u32 rsvdz1;

	union hv_monitor_trigger_group trigger_group[4];
	u64 rsvdz2[3];

	s32 next_checktime[4][32];

	u16 latency[4][32];
	u64 rsvdz3[32];

	struct hv_monitor_parameter parameter[4][32];

	u8 rsvdz4[1984];
};

<<<<<<< HEAD
/* Declare the various hypercall operations. */
enum hv_call_code {
	HVCALL_POST_MESSAGE	= 0x005c,
	HVCALL_SIGNAL_EVENT	= 0x005d,
};
=======
#define HV_HYPERCALL_PARAM_ALIGN	sizeof(u64)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Definition of the hv_post_message hypercall input structure. */
struct hv_input_post_message {
	union hv_connection_id connectionid;
	u32 reserved;
<<<<<<< HEAD
	enum hv_message_type message_type;
=======
	u32 message_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 payload_size;
	u64 payload[HV_MESSAGE_PAYLOAD_QWORD_COUNT];
};

<<<<<<< HEAD
/* Definition of the hv_signal_event hypercall input structure. */
struct hv_input_signal_event {
	union hv_connection_id connectionid;
	u16 flag_number;
	u16 rsvdz;
};

/*
 * Versioning definitions used for guests reporting themselves to the
 * hypervisor, and visa versa.
 */

/* Version info reported by guest OS's */
enum hv_guest_os_vendor {
	HVGUESTOS_VENDOR_MICROSOFT	= 0x0001
};

enum hv_guest_os_microsoft_ids {
	HVGUESTOS_MICROSOFT_UNDEFINED	= 0x00,
	HVGUESTOS_MICROSOFT_MSDOS		= 0x01,
	HVGUESTOS_MICROSOFT_WINDOWS3X	= 0x02,
	HVGUESTOS_MICROSOFT_WINDOWS9X	= 0x03,
	HVGUESTOS_MICROSOFT_WINDOWSNT	= 0x04,
	HVGUESTOS_MICROSOFT_WINDOWSCE	= 0x05
};

/*
 * Declare the MSR used to identify the guest OS.
 */
#define HV_X64_MSR_GUEST_OS_ID	0x40000000

union hv_x64_msr_guest_os_id_contents {
	u64 as_uint64;
	struct {
		u64 build_number:16;
		u64 service_version:8; /* Service Pack, etc. */
		u64 minor_version:8;
		u64 major_version:8;
		u64 os_id:8; /* enum hv_guest_os_microsoft_ids (if Vendor=MS) */
		u64 vendor_id:16; /* enum hv_guest_os_vendor */
	};
};

/*
 * Declare the MSR used to setup pages used to communicate with the hypervisor.
 */
#define HV_X64_MSR_HYPERCALL	0x40000001

union hv_x64_msr_hypercall_contents {
	u64 as_uint64;
	struct {
		u64 enable:1;
		u64 reserved:11;
		u64 guest_physical_address:52;
	};
};


enum {
	VMBUS_MESSAGE_CONNECTION_ID	= 1,
=======

enum {
	VMBUS_MESSAGE_CONNECTION_ID	= 1,
	VMBUS_MESSAGE_CONNECTION_ID_4	= 4,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	VMBUS_MESSAGE_PORT_ID		= 1,
	VMBUS_EVENT_CONNECTION_ID	= 2,
	VMBUS_EVENT_PORT_ID		= 2,
	VMBUS_MONITOR_CONNECTION_ID	= 3,
	VMBUS_MONITOR_PORT_ID		= 3,
	VMBUS_MESSAGE_SINT		= 2,
};

<<<<<<< HEAD
/* #defines */

#define HV_PRESENT_BIT			0x80000000

#define HV_LINUX_GUEST_ID_LO		0x00000000
#define HV_LINUX_GUEST_ID_HI		0xB16B00B5
#define HV_LINUX_GUEST_ID		(((u64)HV_LINUX_GUEST_ID_HI << 32) | \
					   HV_LINUX_GUEST_ID_LO)

#define HV_CPU_POWER_MANAGEMENT		(1 << 0)
#define HV_RECOMMENDATIONS_MAX		4

#define HV_X64_MAX			5
#define HV_CAPS_MAX			8


#define HV_HYPERCALL_PARAM_ALIGN	sizeof(u64)


/* Service definitions */

#define HV_SERVICE_PARENT_PORT				(0)
#define HV_SERVICE_PARENT_CONNECTION			(0)

#define HV_SERVICE_CONNECT_RESPONSE_SUCCESS		(0)
#define HV_SERVICE_CONNECT_RESPONSE_INVALID_PARAMETER	(1)
#define HV_SERVICE_CONNECT_RESPONSE_UNKNOWN_SERVICE	(2)
#define HV_SERVICE_CONNECT_RESPONSE_CONNECTION_REJECTED	(3)

#define HV_SERVICE_CONNECT_REQUEST_MESSAGE_ID		(1)
#define HV_SERVICE_CONNECT_RESPONSE_MESSAGE_ID		(2)
#define HV_SERVICE_DISCONNECT_REQUEST_MESSAGE_ID	(3)
#define HV_SERVICE_DISCONNECT_RESPONSE_MESSAGE_ID	(4)
#define HV_SERVICE_MAX_MESSAGE_ID				(4)

#define HV_SERVICE_PROTOCOL_VERSION (0x0010)
#define HV_CONNECT_PAYLOAD_BYTE_COUNT 64

/* #define VMBUS_REVISION_NUMBER	6 */

/* Our local vmbus's port and connection id. Anything >0 is fine */
/* #define VMBUS_PORT_ID		11 */

/* 628180B8-308D-4c5e-B7DB-1BEB62E62EF4 */
static const uuid_le VMBUS_SERVICE_ID = {
	.b = {
		0xb8, 0x80, 0x81, 0x62, 0x8d, 0x30, 0x5e, 0x4c,
		0xb7, 0xdb, 0x1b, 0xeb, 0x62, 0xe6, 0x2e, 0xf4
	},
};



struct hv_input_signal_event_buffer {
	u64 align8;
	struct hv_input_signal_event event;
=======
/*
 * Per cpu state for channel handling
 */
struct hv_per_cpu_context {
	void *synic_message_page;
	void *synic_event_page;

	/*
	 * The page is only used in hv_post_message() for a TDX VM (with the
	 * paravisor) to post a messages to Hyper-V: when such a VM calls
	 * HVCALL_POST_MESSAGE, it can't use the hyperv_pcpu_input_arg (which
	 * is encrypted in such a VM) as the hypercall input page, because
	 * the input page for HVCALL_POST_MESSAGE must be decrypted in such a
	 * VM, so post_msg_page (which is decrypted in hv_synic_alloc()) is
	 * introduced for this purpose. See hyperv_init() for more comments.
	 */
	void *post_msg_page;

	/*
	 * Starting with win8, we can take channel interrupts on any CPU;
	 * we will manage the tasklet that handles events messages on a per CPU
	 * basis.
	 */
	struct tasklet_struct msg_dpc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct hv_context {
	/* We only support running on top of Hyper-V
<<<<<<< HEAD
	* So at this point this really can only contain the Hyper-V ID
	*/
	u64 guestid;

	void *hypercall_page;

	bool synic_initialized;

	/*
	 * This is used as an input param to HvCallSignalEvent hypercall. The
	 * input param is immutable in our usage and must be dynamic mem (vs
	 * stack or global). */
	struct hv_input_signal_event_buffer *signal_event_buffer;
	/* 8-bytes aligned of the buffer above */
	struct hv_input_signal_event *signal_event_param;

	void *synic_message_page[NR_CPUS];
	void *synic_event_page[NR_CPUS];
=======
	 * So at this point this really can only contain the Hyper-V ID
	 */
	u64 guestid;

	struct hv_per_cpu_context __percpu *cpu_context;

	/*
	 * To manage allocations in a NUMA node.
	 * Array indexed by numa node ID.
	 */
	struct cpumask *hv_numa_map;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

extern struct hv_context hv_context;

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Hv Interface */

extern int hv_init(void);

<<<<<<< HEAD
extern void hv_cleanup(void);

extern u16 hv_post_message(union hv_connection_id connection_id,
			 enum hv_message_type message_type,
			 void *payload, size_t payload_size);

extern u16 hv_signal_event(void);

extern void hv_synic_init(void *irqarg);

extern void hv_synic_cleanup(void *arg);


/* Interface */


int hv_ringbuffer_init(struct hv_ring_buffer_info *ring_info, void *buffer,
		   u32 buflen);

void hv_ringbuffer_cleanup(struct hv_ring_buffer_info *ring_info);

int hv_ringbuffer_write(struct hv_ring_buffer_info *ring_info,
		    struct scatterlist *sglist,
		    u32 sgcount);

int hv_ringbuffer_peek(struct hv_ring_buffer_info *ring_info, void *buffer,
		   u32 buflen);

int hv_ringbuffer_read(struct hv_ring_buffer_info *ring_info,
		   void *buffer,
		   u32 buflen,
		   u32 offset);

u32 hv_get_ringbuffer_interrupt_mask(struct hv_ring_buffer_info *ring_info);

void hv_ringbuffer_get_debuginfo(struct hv_ring_buffer_info *ring_info,
			    struct hv_ring_buffer_debug_info *debug_info);

/*
 * Maximum channels is determined by the size of the interrupt page
 * which is PAGE_SIZE. 1/2 of PAGE_SIZE is for send endpoint interrupt
 * and the other is receive endpoint interrupt
 */
#define MAX_NUM_CHANNELS	((PAGE_SIZE >> 1) << 3)	/* 16348 channels */

/* The value here must be in multiple of 32 */
/* TODO: Need to make this configurable */
#define MAX_NUM_CHANNELS_SUPPORTED	256

=======
extern int hv_post_message(union hv_connection_id connection_id,
			 enum hv_message_type message_type,
			 void *payload, size_t payload_size);

extern int hv_synic_alloc(void);

extern void hv_synic_free(void);

extern void hv_synic_enable_regs(unsigned int cpu);
extern int hv_synic_init(unsigned int cpu);

extern void hv_synic_disable_regs(unsigned int cpu);
extern int hv_synic_cleanup(unsigned int cpu);

/* Interface */

void hv_ringbuffer_pre_init(struct vmbus_channel *channel);

int hv_ringbuffer_init(struct hv_ring_buffer_info *ring_info,
		       struct page *pages, u32 pagecnt, u32 max_pkt_size);

void hv_ringbuffer_cleanup(struct hv_ring_buffer_info *ring_info);

int hv_ringbuffer_write(struct vmbus_channel *channel,
			const struct kvec *kv_list, u32 kv_count,
			u64 requestid, u64 *trans_id);

int hv_ringbuffer_read(struct vmbus_channel *channel,
		       void *buffer, u32 buflen, u32 *buffer_actual_len,
		       u64 *requestid, bool raw);

/*
 * The Maximum number of channels (16384) is determined by the size of the
 * interrupt page, which is HV_HYP_PAGE_SIZE. 1/2 of HV_HYP_PAGE_SIZE is to
 * send endpoint interrupts, and the other is to receive endpoint interrupts.
 */
#define MAX_NUM_CHANNELS	((HV_HYP_PAGE_SIZE >> 1) << 3)

/* The value here must be in multiple of 32 */
#define MAX_NUM_CHANNELS_SUPPORTED	256

#define MAX_CHANNEL_RELIDS					\
	max(MAX_NUM_CHANNELS_SUPPORTED, HV_EVENT_FLAGS_COUNT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum vmbus_connect_state {
	DISCONNECTED,
	CONNECTING,
	CONNECTED,
	DISCONNECTING
};

#define MAX_SIZE_CHANNEL_MESSAGE	HV_MESSAGE_PAYLOAD_BYTE_COUNT

<<<<<<< HEAD
struct vmbus_connection {
=======
/*
 * The CPU that Hyper-V will interrupt for VMBUS messages, such as
 * CHANNELMSG_OFFERCHANNEL and CHANNELMSG_RESCIND_CHANNELOFFER.
 */
#define VMBUS_CONNECT_CPU	0

struct vmbus_connection {
	u32 msg_conn_id;

	atomic_t offer_in_progress;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum vmbus_connect_state conn_state;

	atomic_t next_gpadl_handle;

<<<<<<< HEAD
=======
	struct completion  unload_event;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Represents channel interrupts. Each bit position represents a
	 * channel.  When a channel sends an interrupt via VMBUS, it finds its
	 * bit in the sendInterruptPage, set it and calls Hv to generate a port
	 * event. The other end receives the port event and parse the
	 * recvInterruptPage to see which bit is set
	 */
	void *int_page;
	void *send_int_page;
	void *recv_int_page;

	/*
	 * 2 pages - 1st page for parent->child notification and 2nd
	 * is child->parent notification
	 */
<<<<<<< HEAD
	void *monitor_pages;
=======
	struct hv_monitor_page *monitor_pages[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head chn_msg_list;
	spinlock_t channelmsg_lock;

	/* List of channels */
	struct list_head chn_list;
<<<<<<< HEAD
	spinlock_t channel_lock;

	struct workqueue_struct *work_queue;
=======
	struct mutex channel_mutex;

	/* Array of channels */
	struct vmbus_channel **channels;

	/*
	 * An offer message is handled first on the work_queue, and then
	 * is further handled on handle_primary_chan_wq or
	 * handle_sub_chan_wq.
	 */
	struct workqueue_struct *work_queue;
	struct workqueue_struct *handle_primary_chan_wq;
	struct workqueue_struct *handle_sub_chan_wq;
	struct workqueue_struct *rescind_work_queue;

	/*
	 * On suspension of the vmbus, the accumulated offer messages
	 * must be dropped.
	 */
	bool ignore_any_offer_msg;

	/*
	 * The number of sub-channels and hv_sock channels that should be
	 * cleaned up upon suspend: sub-channels will be re-created upon
	 * resume, and hv_sock channels should not survive suspend.
	 */
	atomic_t nr_chan_close_on_suspend;
	/*
	 * vmbus_bus_suspend() waits for "nr_chan_close_on_suspend" to
	 * drop to zero.
	 */
	struct completion ready_for_suspend_event;

	/*
	 * The number of primary channels that should be "fixed up"
	 * upon resume: these channels are re-offered upon resume, and some
	 * fields of the channel offers (i.e. child_relid and connection_id)
	 * can change, so the old offermsg must be fixed up, before the resume
	 * callbacks of the VSC drivers start to further touch the channels.
	 */
	atomic_t nr_chan_fixup_on_resume;
	/*
	 * vmbus_bus_resume() waits for "nr_chan_fixup_on_resume" to
	 * drop to zero.
	 */
	struct completion ready_for_resume_event;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


struct vmbus_msginfo {
	/* Bookkeeping stuff */
	struct list_head msglist_entry;

	/* The message itself */
<<<<<<< HEAD
	unsigned char msg[0];
=======
	unsigned char msg[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


extern struct vmbus_connection vmbus_connection;

<<<<<<< HEAD
/* General vmbus interface */

struct hv_device *vmbus_device_create(uuid_le *type,
					 uuid_le *instance,
					 struct vmbus_channel *channel);

int vmbus_device_register(struct hv_device *child_device_obj);
void vmbus_device_unregister(struct hv_device *device_obj);

/* static void */
/* VmbusChildDeviceDestroy( */
/* struct hv_device *); */
=======
int vmbus_negotiate_version(struct vmbus_channel_msginfo *msginfo, u32 version);

static inline void vmbus_send_interrupt(u32 relid)
{
	sync_set_bit(relid, vmbus_connection.send_int_page);
}

enum vmbus_message_handler_type {
	/* The related handler can sleep. */
	VMHT_BLOCKING = 0,

	/* The related handler must NOT sleep. */
	VMHT_NON_BLOCKING = 1,
};

struct vmbus_channel_message_table_entry {
	enum vmbus_channel_message_type message_type;
	enum vmbus_message_handler_type handler_type;
	void (*message_handler)(struct vmbus_channel_message_header *msg);
	u32 min_payload_len;
};

extern const struct vmbus_channel_message_table_entry
	channel_message_table[CHANNELMSG_COUNT];


/* General vmbus interface */

struct hv_device *vmbus_device_create(const guid_t *type,
				      const guid_t *instance,
				      struct vmbus_channel *channel);

int vmbus_device_register(struct hv_device *child_device_obj);
void vmbus_device_unregister(struct hv_device *device_obj);
int vmbus_add_channel_kobj(struct hv_device *device_obj,
			   struct vmbus_channel *channel);

void vmbus_remove_channel_attr_group(struct vmbus_channel *channel);

void vmbus_channel_map_relid(struct vmbus_channel *channel);
void vmbus_channel_unmap_relid(struct vmbus_channel *channel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct vmbus_channel *relid2channel(u32 relid);

void vmbus_free_channels(void);

/* Connection interface */

int vmbus_connect(void);
<<<<<<< HEAD

int vmbus_post_msg(void *buffer, size_t buflen);

int vmbus_set_event(u32 child_relid);

void vmbus_on_event(unsigned long data);

=======
void vmbus_disconnect(void);

int vmbus_post_msg(void *buffer, size_t buflen, bool can_sleep);

void vmbus_on_event(unsigned long data);
void vmbus_on_msg_dpc(unsigned long data);

int hv_kvp_init(struct hv_util_service *srv);
void hv_kvp_deinit(void);
int hv_kvp_pre_suspend(void);
int hv_kvp_pre_resume(void);
void hv_kvp_onchannelcallback(void *context);

int hv_vss_init(struct hv_util_service *srv);
void hv_vss_deinit(void);
int hv_vss_pre_suspend(void);
int hv_vss_pre_resume(void);
void hv_vss_onchannelcallback(void *context);

int hv_fcopy_init(struct hv_util_service *srv);
void hv_fcopy_deinit(void);
int hv_fcopy_pre_suspend(void);
int hv_fcopy_pre_resume(void);
void hv_fcopy_onchannelcallback(void *context);
void vmbus_initiate_unload(bool crash);

static inline void hv_poll_channel(struct vmbus_channel *channel,
				   void (*cb)(void *))
{
	if (!channel)
		return;
	cb(channel);
}

enum hvutil_device_state {
	HVUTIL_DEVICE_INIT = 0,  /* driver is loaded, waiting for userspace */
	HVUTIL_READY,            /* userspace is registered */
	HVUTIL_HOSTMSG_RECEIVED, /* message from the host was received */
	HVUTIL_USERSPACE_REQ,    /* request to userspace was sent */
	HVUTIL_USERSPACE_RECV,   /* reply from userspace was received */
	HVUTIL_DEVICE_DYING,     /* driver unload is in progress */
};

enum delay {
	INTERRUPT_DELAY = 0,
	MESSAGE_DELAY   = 1,
};

extern const struct vmbus_device vmbus_devs[];

static inline bool hv_is_perf_channel(struct vmbus_channel *channel)
{
	return vmbus_devs[channel->device_id].perf_device;
}

static inline bool hv_is_allocated_cpu(unsigned int cpu)
{
	struct vmbus_channel *channel, *sc;

	lockdep_assert_held(&vmbus_connection.channel_mutex);
	/*
	 * List additions/deletions as well as updates of the target CPUs are
	 * protected by channel_mutex.
	 */
	list_for_each_entry(channel, &vmbus_connection.chn_list, listentry) {
		if (!hv_is_perf_channel(channel))
			continue;
		if (channel->target_cpu == cpu)
			return true;
		list_for_each_entry(sc, &channel->sc_list, sc_list) {
			if (sc->target_cpu == cpu)
				return true;
		}
	}
	return false;
}

static inline void hv_set_allocated_cpu(unsigned int cpu)
{
	cpumask_set_cpu(cpu, &hv_context.hv_numa_map[cpu_to_node(cpu)]);
}

static inline void hv_clear_allocated_cpu(unsigned int cpu)
{
	if (hv_is_allocated_cpu(cpu))
		return;
	cpumask_clear_cpu(cpu, &hv_context.hv_numa_map[cpu_to_node(cpu)]);
}

static inline void hv_update_allocated_cpus(unsigned int old_cpu,
					  unsigned int new_cpu)
{
	hv_set_allocated_cpu(new_cpu);
	hv_clear_allocated_cpu(old_cpu);
}

#ifdef CONFIG_HYPERV_TESTING

int hv_debug_add_dev_dir(struct hv_device *dev);
void hv_debug_rm_dev_dir(struct hv_device *dev);
void hv_debug_rm_all_dir(void);
int hv_debug_init(void);
void hv_debug_delay_test(struct vmbus_channel *channel, enum delay delay_type);

#else /* CONFIG_HYPERV_TESTING */

static inline void hv_debug_rm_dev_dir(struct hv_device *dev) {};
static inline void hv_debug_rm_all_dir(void) {};
static inline void hv_debug_delay_test(struct vmbus_channel *channel,
				       enum delay delay_type) {};
static inline int hv_debug_init(void)
{
	return -1;
}

static inline int hv_debug_add_dev_dir(struct hv_device *dev)
{
	return -1;
}

#endif /* CONFIG_HYPERV_TESTING */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _HYPERV_VMBUS_H */
