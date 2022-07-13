/*
 * Performance events:
 *
 *    Copyright (C) 2008-2009, Thomas Gleixner <tglx@linutronix.de>
 *    Copyright (C) 2008-2011, Red Hat, Inc., Ingo Molnar
 *    Copyright (C) 2008-2011, Red Hat, Inc., Peter Zijlstra
 *
 * Data type definitions, declarations, prototypes.
 *
 *    Started by: Thomas Gleixner and Ingo Molnar
 *
 * For licencing details see kernel-base/COPYING
 */
#ifndef _LINUX_PERF_EVENT_H
#define _LINUX_PERF_EVENT_H

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/byteorder.h>

/*
 * User-space ABI bits:
 */

/*
 * attr.type
 */
enum perf_type_id {
	PERF_TYPE_HARDWARE			= 0,
	PERF_TYPE_SOFTWARE			= 1,
	PERF_TYPE_TRACEPOINT			= 2,
	PERF_TYPE_HW_CACHE			= 3,
	PERF_TYPE_RAW				= 4,
	PERF_TYPE_BREAKPOINT			= 5,

	PERF_TYPE_MAX,				/* non-ABI */
};

/*
 * Generalized performance event event_id types, used by the
 * attr.event_id parameter of the sys_perf_event_open()
 * syscall:
 */
enum perf_hw_id {
	/*
	 * Common hardware events, generalized by the kernel:
	 */
	PERF_COUNT_HW_CPU_CYCLES		= 0,
	PERF_COUNT_HW_INSTRUCTIONS		= 1,
	PERF_COUNT_HW_CACHE_REFERENCES		= 2,
	PERF_COUNT_HW_CACHE_MISSES		= 3,
	PERF_COUNT_HW_BRANCH_INSTRUCTIONS	= 4,
	PERF_COUNT_HW_BRANCH_MISSES		= 5,
	PERF_COUNT_HW_BUS_CYCLES		= 6,
	PERF_COUNT_HW_STALLED_CYCLES_FRONTEND	= 7,
	PERF_COUNT_HW_STALLED_CYCLES_BACKEND	= 8,
	PERF_COUNT_HW_REF_CPU_CYCLES		= 9,

	PERF_COUNT_HW_MAX,			/* non-ABI */
};

/*
 * Generalized hardware cache events:
 *
 *       { L1-D, L1-I, LLC, ITLB, DTLB, BPU, NODE } x
 *       { read, write, prefetch } x
 *       { accesses, misses }
 */
enum perf_hw_cache_id {
	PERF_COUNT_HW_CACHE_L1D			= 0,
	PERF_COUNT_HW_CACHE_L1I			= 1,
	PERF_COUNT_HW_CACHE_LL			= 2,
	PERF_COUNT_HW_CACHE_DTLB		= 3,
	PERF_COUNT_HW_CACHE_ITLB		= 4,
	PERF_COUNT_HW_CACHE_BPU			= 5,
	PERF_COUNT_HW_CACHE_NODE		= 6,

	PERF_COUNT_HW_CACHE_MAX,		/* non-ABI */
};

enum perf_hw_cache_op_id {
	PERF_COUNT_HW_CACHE_OP_READ		= 0,
	PERF_COUNT_HW_CACHE_OP_WRITE		= 1,
	PERF_COUNT_HW_CACHE_OP_PREFETCH		= 2,

	PERF_COUNT_HW_CACHE_OP_MAX,		/* non-ABI */
};

enum perf_hw_cache_op_result_id {
	PERF_COUNT_HW_CACHE_RESULT_ACCESS	= 0,
	PERF_COUNT_HW_CACHE_RESULT_MISS		= 1,

	PERF_COUNT_HW_CACHE_RESULT_MAX,		/* non-ABI */
};

/*
 * Special "software" events provided by the kernel, even if the hardware
 * does not support performance events. These events measure various
 * physical and sw events of the kernel (and allow the profiling of them as
 * well):
 */
enum perf_sw_ids {
	PERF_COUNT_SW_CPU_CLOCK			= 0,
	PERF_COUNT_SW_TASK_CLOCK		= 1,
	PERF_COUNT_SW_PAGE_FAULTS		= 2,
	PERF_COUNT_SW_CONTEXT_SWITCHES		= 3,
	PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
	PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
	PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
	PERF_COUNT_SW_ALIGNMENT_FAULTS		= 7,
	PERF_COUNT_SW_EMULATION_FAULTS		= 8,

	PERF_COUNT_SW_MAX,			/* non-ABI */
};

/*
 * Bits that can be set in attr.sample_type to request information
 * in the overflow packets.
 */
enum perf_event_sample_format {
	PERF_SAMPLE_IP				= 1U << 0,
	PERF_SAMPLE_TID				= 1U << 1,
	PERF_SAMPLE_TIME			= 1U << 2,
	PERF_SAMPLE_ADDR			= 1U << 3,
	PERF_SAMPLE_READ			= 1U << 4,
	PERF_SAMPLE_CALLCHAIN			= 1U << 5,
	PERF_SAMPLE_ID				= 1U << 6,
	PERF_SAMPLE_CPU				= 1U << 7,
	PERF_SAMPLE_PERIOD			= 1U << 8,
	PERF_SAMPLE_STREAM_ID			= 1U << 9,
	PERF_SAMPLE_RAW				= 1U << 10,
	PERF_SAMPLE_BRANCH_STACK		= 1U << 11,

	PERF_SAMPLE_MAX = 1U << 12,		/* non-ABI */
};

/*
 * values to program into branch_sample_type when PERF_SAMPLE_BRANCH is set
 *
 * If the user does not pass priv level information via branch_sample_type,
 * the kernel uses the event's priv level. Branch and event priv levels do
 * not have to match. Branch priv level is checked for permissions.
 *
 * The branch types can be combined, however BRANCH_ANY covers all types
 * of branches and therefore it supersedes all the other types.
 */
enum perf_branch_sample_type {
	PERF_SAMPLE_BRANCH_USER		= 1U << 0, /* user branches */
	PERF_SAMPLE_BRANCH_KERNEL	= 1U << 1, /* kernel branches */
	PERF_SAMPLE_BRANCH_HV		= 1U << 2, /* hypervisor branches */

	PERF_SAMPLE_BRANCH_ANY		= 1U << 3, /* any branch types */
	PERF_SAMPLE_BRANCH_ANY_CALL	= 1U << 4, /* any call branch */
	PERF_SAMPLE_BRANCH_ANY_RETURN	= 1U << 5, /* any return branch */
	PERF_SAMPLE_BRANCH_IND_CALL	= 1U << 6, /* indirect calls */

	PERF_SAMPLE_BRANCH_MAX		= 1U << 7, /* non-ABI */
};

#define PERF_SAMPLE_BRANCH_PLM_ALL \
	(PERF_SAMPLE_BRANCH_USER|\
	 PERF_SAMPLE_BRANCH_KERNEL|\
	 PERF_SAMPLE_BRANCH_HV)

/*
 * The format of the data returned by read() on a perf event fd,
 * as specified by attr.read_format:
 *
 * struct read_format {
 *	{ u64		value;
 *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
 *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
 *	  { u64		id;           } && PERF_FORMAT_ID
 *	} && !PERF_FORMAT_GROUP
 *
 *	{ u64		nr;
 *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
 *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
 *	  { u64		value;
 *	    { u64	id;           } && PERF_FORMAT_ID
 *	  }		cntr[nr];
 *	} && PERF_FORMAT_GROUP
 * };
 */
enum perf_event_read_format {
	PERF_FORMAT_TOTAL_TIME_ENABLED		= 1U << 0,
	PERF_FORMAT_TOTAL_TIME_RUNNING		= 1U << 1,
	PERF_FORMAT_ID				= 1U << 2,
	PERF_FORMAT_GROUP			= 1U << 3,

	PERF_FORMAT_MAX = 1U << 4,		/* non-ABI */
};

#define PERF_ATTR_SIZE_VER0	64	/* sizeof first published struct */
#define PERF_ATTR_SIZE_VER1	72	/* add: config2 */
#define PERF_ATTR_SIZE_VER2	80	/* add: branch_sample_type */

/*
 * Hardware event_id to monitor via a performance monitoring event:
 */
struct perf_event_attr {

	/*
	 * Major type: hardware/software/tracepoint/etc.
	 */
	__u32			type;

	/*
	 * Size of the attr structure, for fwd/bwd compat.
	 */
	__u32			size;

	/*
	 * Type specific configuration information.
	 */
	__u64			config;

	union {
		__u64		sample_period;
		__u64		sample_freq;
	};

	__u64			sample_type;
	__u64			read_format;

	__u64			disabled       :  1, /* off by default        */
				inherit	       :  1, /* children inherit it   */
				pinned	       :  1, /* must always be on PMU */
				exclusive      :  1, /* only group on PMU     */
				exclude_user   :  1, /* don't count user      */
				exclude_kernel :  1, /* ditto kernel          */
				exclude_hv     :  1, /* ditto hypervisor      */
				exclude_idle   :  1, /* don't count when idle */
				mmap           :  1, /* include mmap data     */
				comm	       :  1, /* include comm data     */
				freq           :  1, /* use freq, not period  */
				inherit_stat   :  1, /* per task counts       */
				enable_on_exec :  1, /* next exec enables     */
				task           :  1, /* trace fork/exit       */
				watermark      :  1, /* wakeup_watermark      */
				/*
				 * precise_ip:
				 *
				 *  0 - SAMPLE_IP can have arbitrary skid
				 *  1 - SAMPLE_IP must have constant skid
				 *  2 - SAMPLE_IP requested to have 0 skid
				 *  3 - SAMPLE_IP must have 0 skid
				 *
				 *  See also PERF_RECORD_MISC_EXACT_IP
				 */
				precise_ip     :  2, /* skid constraint       */
				mmap_data      :  1, /* non-exec mmap data    */
				sample_id_all  :  1, /* sample_type all events */

				exclude_host   :  1, /* don't count in host   */
				exclude_guest  :  1, /* don't count in guest  */
				constraint_duplicate : 1,

				__reserved_1   : 42;

	union {
		__u32		wakeup_events;	  /* wakeup every n events */
		__u32		wakeup_watermark; /* bytes before wakeup   */
	};

	__u32			bp_type;
	union {
		__u64		bp_addr;
		__u64		config1; /* extension of config */
	};
	union {
		__u64		bp_len;
		__u64		config2; /* extension of config1 */
	};
	__u64	branch_sample_type; /* enum branch_sample_type */
};

/*
 * Ioctls that can be done on a perf event fd:
 */
#define PERF_EVENT_IOC_ENABLE		_IO ('$', 0)
#define PERF_EVENT_IOC_DISABLE		_IO ('$', 1)
#define PERF_EVENT_IOC_REFRESH		_IO ('$', 2)
#define PERF_EVENT_IOC_RESET		_IO ('$', 3)
#define PERF_EVENT_IOC_PERIOD		_IOW('$', 4, __u64)
#define PERF_EVENT_IOC_SET_OUTPUT	_IO ('$', 5)
#define PERF_EVENT_IOC_SET_FILTER	_IOW('$', 6, char *)

enum perf_event_ioc_flags {
	PERF_IOC_FLAG_GROUP		= 1U << 0,
};

/*
 * Structure of the page that can be mapped via mmap
 */
struct perf_event_mmap_page {
	__u32	version;		/* version number of this structure */
	__u32	compat_version;		/* lowest version this is compat with */

	/*
	 * Bits needed to read the hw events in user-space.
	 *
	 *   u32 seq, time_mult, time_shift, idx, width;
	 *   u64 count, enabled, running;
	 *   u64 cyc, time_offset;
	 *   s64 pmc = 0;
	 *
	 *   do {
	 *     seq = pc->lock;
	 *     barrier()
	 *
	 *     enabled = pc->time_enabled;
	 *     running = pc->time_running;
	 *
	 *     if (pc->cap_usr_time && enabled != running) {
	 *       cyc = rdtsc();
	 *       time_offset = pc->time_offset;
	 *       time_mult   = pc->time_mult;
	 *       time_shift  = pc->time_shift;
	 *     }
	 *
	 *     idx = pc->index;
	 *     count = pc->offset;
	 *     if (pc->cap_usr_rdpmc && idx) {
	 *       width = pc->pmc_width;
	 *       pmc = rdpmc(idx - 1);
	 *     }
	 *
	 *     barrier();
	 *   } while (pc->lock != seq);
	 *
	 * NOTE: for obvious reason this only works on self-monitoring
	 *       processes.
	 */
	__u32	lock;			/* seqlock for synchronization */
	__u32	index;			/* hardware event identifier */
	__s64	offset;			/* add to hardware event value */
	__u64	time_enabled;		/* time event active */
	__u64	time_running;		/* time event on cpu */
	union {
		__u64	capabilities;
		__u64	cap_usr_time  : 1,
			cap_usr_rdpmc : 1,
			cap_____res   : 62;
	};

	/*
	 * If cap_usr_rdpmc this field provides the bit-width of the value
	 * read using the rdpmc() or equivalent instruction. This can be used
	 * to sign extend the result like:
	 *
	 *   pmc <<= 64 - width;
	 *   pmc >>= 64 - width; // signed shift right
	 *   count += pmc;
	 */
	__u16	pmc_width;

	/*
	 * If cap_usr_time the below fields can be used to compute the time
	 * delta since time_enabled (in ns) using rdtsc or similar.
	 *
	 *   u64 quot, rem;
	 *   u64 delta;
	 *
	 *   quot = (cyc >> time_shift);
	 *   rem = cyc & ((1 << time_shift) - 1);
	 *   delta = time_offset + quot * time_mult +
	 *              ((rem * time_mult) >> time_shift);
	 *
	 * Where time_offset,time_mult,time_shift and cyc are read in the
	 * seqcount loop described above. This delta can then be added to
	 * enabled and possible running (if idx), improving the scaling:
	 *
	 *   enabled += delta;
	 *   if (idx)
	 *     running += delta;
	 *
	 *   quot = count / running;
	 *   rem  = count % running;
	 *   count = quot * enabled + (rem * enabled) / running;
	 */
	__u16	time_shift;
	__u32	time_mult;
	__u64	time_offset;

		/*
		 * Hole for extension of the self monitor capabilities
		 */

	__u64	__reserved[120];	/* align to 1k */

	/*
	 * Control data for the mmap() data buffer.
	 *
	 * User-space reading the @data_head value should issue an smp_rmb(),
	 * after reading this value.
	 *
	 * When the mapping is PROT_WRITE the @data_tail value should be
	 * written by userspace to reflect the last read data, after issueing
	 * an smp_mb() to separate the data read from the ->data_tail store.
	 * In this case the kernel will not over-write unread data.
	 *
	 * See perf_output_put_handle() for the data ordering.
	 */
	__u64   data_head;		/* head in the data section */
	__u64	data_tail;		/* user-space written tail */
};

#define PERF_RECORD_MISC_CPUMODE_MASK		(7 << 0)
#define PERF_RECORD_MISC_CPUMODE_UNKNOWN	(0 << 0)
#define PERF_RECORD_MISC_KERNEL			(1 << 0)
#define PERF_RECORD_MISC_USER			(2 << 0)
#define PERF_RECORD_MISC_HYPERVISOR		(3 << 0)
#define PERF_RECORD_MISC_GUEST_KERNEL		(4 << 0)
#define PERF_RECORD_MISC_GUEST_USER		(5 << 0)

/*
 * Indicates that the content of PERF_SAMPLE_IP points to
 * the actual instruction that triggered the event. See also
 * perf_event_attr::precise_ip.
 */
#define PERF_RECORD_MISC_EXACT_IP		(1 << 14)
/*
 * Reserve the last bit to indicate some extended misc field
 */
#define PERF_RECORD_MISC_EXT_RESERVED		(1 << 15)

struct perf_event_header {
	__u32	type;
	__u16	misc;
	__u16	size;
};

enum perf_event_type {

	/*
	 * If perf_event_attr.sample_id_all is set then all event types will
	 * have the sample_type selected fields related to where/when
	 * (identity) an event took place (TID, TIME, ID, CPU, STREAM_ID)
	 * described in PERF_RECORD_SAMPLE below, it will be stashed just after
	 * the perf_event_header and the fields already present for the existing
	 * fields, i.e. at the end of the payload. That way a newer perf.data
	 * file will be supported by older perf tools, with these new optional
	 * fields being ignored.
	 *
	 * The MMAP events record the PROT_EXEC mappings so that we can
	 * correlate userspace IPs to code. They have the following structure:
	 *
	 * struct {
	 *	struct perf_event_header	header;
	 *
	 *	u32				pid, tid;
	 *	u64				addr;
	 *	u64				len;
	 *	u64				pgoff;
	 *	char				filename[];
	 * };
	 */
	PERF_RECORD_MMAP			= 1,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *	u64				id;
	 *	u64				lost;
	 * };
	 */
	PERF_RECORD_LOST			= 2,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *
	 *	u32				pid, tid;
	 *	char				comm[];
	 * };
	 */
	PERF_RECORD_COMM			= 3,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *	u32				pid, ppid;
	 *	u32				tid, ptid;
	 *	u64				time;
	 * };
	 */
	PERF_RECORD_EXIT			= 4,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *	u64				time;
	 *	u64				id;
	 *	u64				stream_id;
	 * };
	 */
	PERF_RECORD_THROTTLE			= 5,
	PERF_RECORD_UNTHROTTLE			= 6,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *	u32				pid, ppid;
	 *	u32				tid, ptid;
	 *	u64				time;
	 * };
	 */
	PERF_RECORD_FORK			= 7,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *	u32				pid, tid;
	 *
	 *	struct read_format		values;
	 * };
	 */
	PERF_RECORD_READ			= 8,

	/*
	 * struct {
	 *	struct perf_event_header	header;
	 *
	 *	{ u64			ip;	  } && PERF_SAMPLE_IP
	 *	{ u32			pid, tid; } && PERF_SAMPLE_TID
	 *	{ u64			time;     } && PERF_SAMPLE_TIME
	 *	{ u64			addr;     } && PERF_SAMPLE_ADDR
	 *	{ u64			id;	  } && PERF_SAMPLE_ID
	 *	{ u64			stream_id;} && PERF_SAMPLE_STREAM_ID
	 *	{ u32			cpu, res; } && PERF_SAMPLE_CPU
	 *	{ u64			period;   } && PERF_SAMPLE_PERIOD
	 *
	 *	{ struct read_format	values;	  } && PERF_SAMPLE_READ
	 *
	 *	{ u64			nr,
	 *	  u64			ips[nr];  } && PERF_SAMPLE_CALLCHAIN
	 *
	 *	#
	 *	# The RAW record below is opaque data wrt the ABI
	 *	#
	 *	# That is, the ABI doesn't make any promises wrt to
	 *	# the stability of its content, it may vary depending
	 *	# on event, hardware, kernel version and phase of
	 *	# the moon.
	 *	#
	 *	# In other words, PERF_SAMPLE_RAW contents are not an ABI.
	 *	#
	 *
	 *	{ u32			size;
	 *	  char                  data[size];}&& PERF_SAMPLE_RAW
	 *
	 *	{ u64 from, to, flags } lbr[nr];} && PERF_SAMPLE_BRANCH_STACK
	 * };
	 */
	PERF_RECORD_SAMPLE			= 9,

	PERF_RECORD_MAX,			/* non-ABI */
};

enum perf_callchain_context {
	PERF_CONTEXT_HV			= (__u64)-32,
	PERF_CONTEXT_KERNEL		= (__u64)-128,
	PERF_CONTEXT_USER		= (__u64)-512,

	PERF_CONTEXT_GUEST		= (__u64)-2048,
	PERF_CONTEXT_GUEST_KERNEL	= (__u64)-2176,
	PERF_CONTEXT_GUEST_USER		= (__u64)-2560,

	PERF_CONTEXT_MAX		= (__u64)-4095,
};

#define PERF_FLAG_FD_NO_GROUP		(1U << 0)
#define PERF_FLAG_FD_OUTPUT		(1U << 1)
#define PERF_FLAG_PID_CGROUP		(1U << 2) /* pid=cgroup id, per-cpu mode only */

#ifdef __KERNEL__
/*
=======
#include <uapi/linux/perf_event.h>
#include <uapi/linux/bpf_perf_event.h>

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Kernel-internal data types and definitions:
 */

#ifdef CONFIG_PERF_EVENTS
<<<<<<< HEAD
# include <linux/cgroup.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# include <asm/perf_event.h>
# include <asm/local64.h>
#endif

<<<<<<< HEAD
struct perf_guest_info_callbacks {
	int				(*is_in_guest)(void);
	int				(*is_user_mode)(void);
	unsigned long			(*get_guest_ip)(void);
};

#ifdef CONFIG_HAVE_HW_BREAKPOINT
=======
#define PERF_GUEST_ACTIVE	0x01
#define PERF_GUEST_USER	0x02

struct perf_guest_info_callbacks {
	unsigned int			(*state)(void);
	unsigned long			(*get_ip)(void);
	unsigned int			(*handle_intel_pt_intr)(void);
};

#ifdef CONFIG_HAVE_HW_BREAKPOINT
#include <linux/rhashtable-types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/hw_breakpoint.h>
#endif

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>
#include <linux/hrtimer.h>
#include <linux/fs.h>
#include <linux/pid_namespace.h>
#include <linux/workqueue.h>
#include <linux/ftrace.h>
#include <linux/cpu.h>
#include <linux/irq_work.h>
#include <linux/static_key.h>
<<<<<<< HEAD
#include <linux/atomic.h>
#include <linux/sysfs.h>
#include <asm/local.h>

#define PERF_MAX_STACK_DEPTH		255

struct perf_callchain_entry {
	__u64				nr;
	__u64				ip[PERF_MAX_STACK_DEPTH];
};

struct perf_raw_record {
	u32				size;
	void				*data;
};

/*
 * single taken branch record layout:
 *
 *      from: source instruction (may not always be a branch insn)
 *        to: branch target
 *   mispred: branch target was mispredicted
 * predicted: branch target was predicted
 *
 * support for mispred, predicted is optional. In case it
 * is not supported mispred = predicted = 0.
 */
struct perf_branch_entry {
	__u64	from;
	__u64	to;
	__u64	mispred:1,  /* target mispredicted */
		predicted:1,/* target predicted */
		reserved:62;
};
=======
#include <linux/jump_label_ratelimit.h>
#include <linux/atomic.h>
#include <linux/sysfs.h>
#include <linux/perf_regs.h>
#include <linux/cgroup.h>
#include <linux/refcount.h>
#include <linux/security.h>
#include <linux/static_call.h>
#include <linux/lockdep.h>
#include <asm/local.h>

struct perf_callchain_entry {
	__u64				nr;
	__u64				ip[]; /* /proc/sys/kernel/perf_event_max_stack */
};

struct perf_callchain_entry_ctx {
	struct perf_callchain_entry *entry;
	u32			    max_stack;
	u32			    nr;
	short			    contexts;
	bool			    contexts_maxed;
};

typedef unsigned long (*perf_copy_f)(void *dst, const void *src,
				     unsigned long off, unsigned long len);

struct perf_raw_frag {
	union {
		struct perf_raw_frag	*next;
		unsigned long		pad;
	};
	perf_copy_f			copy;
	void				*data;
	u32				size;
} __packed;

struct perf_raw_record {
	struct perf_raw_frag		frag;
	u32				size;
};

static __always_inline bool perf_raw_frag_last(const struct perf_raw_frag *frag)
{
	return frag->pad < sizeof(u64);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * branch stack layout:
 *  nr: number of taken branches stored in entries[]
<<<<<<< HEAD
=======
 *  hw_idx: The low level index of raw branch records
 *          for the most recent branch.
 *          -1ULL means invalid/unknown.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Note that nr can vary from sample to sample
 * branches (to, from) are stored from most recent
 * to least recent, i.e., entries[0] contains the most
 * recent branch.
<<<<<<< HEAD
 */
struct perf_branch_stack {
	__u64				nr;
	struct perf_branch_entry	entries[0];
=======
 * The entries[] is an abstraction of raw branch records,
 * which may not be stored in age order in HW, e.g. Intel LBR.
 * The hw_idx is to expose the low level index of raw
 * branch record for the most recent branch aka entries[0].
 * The hw_idx index is between -1 (unknown) and max depth,
 * which can be retrieved in /sys/devices/cpu/caps/branches.
 * For the architectures whose raw branch records are
 * already stored in age order, the hw_idx should be 0.
 */
struct perf_branch_stack {
	__u64				nr;
	__u64				hw_idx;
	struct perf_branch_entry	entries[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct task_struct;

/*
 * extra PMU register associated with an event
 */
struct hw_perf_event_extra {
	u64		config;	/* register value */
	unsigned int	reg;	/* register address or index */
	int		alloc;	/* extra register already allocated */
	int		idx;	/* index in shared_regs->regs[] */
};

/**
<<<<<<< HEAD
=======
 * hw_perf_event::flag values
 *
 * PERF_EVENT_FLAG_ARCH bits are reserved for architecture-specific
 * usage.
 */
#define PERF_EVENT_FLAG_ARCH			0x000fffff
#define PERF_EVENT_FLAG_USER_READ_CNT		0x80000000

static_assert((PERF_EVENT_FLAG_USER_READ_CNT & PERF_EVENT_FLAG_ARCH) == 0);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * struct hw_perf_event - performance event hardware details:
 */
struct hw_perf_event {
#ifdef CONFIG_PERF_EVENTS
	union {
		struct { /* hardware */
			u64		config;
			u64		last_tag;
			unsigned long	config_base;
			unsigned long	event_base;
<<<<<<< HEAD
			int		idx;
			int		last_cpu;
=======
			int		event_base_rdpmc;
			int		idx;
			int		last_cpu;
			int		flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			struct hw_perf_event_extra extra_reg;
			struct hw_perf_event_extra branch_reg;
		};
		struct { /* software */
			struct hrtimer	hrtimer;
		};
<<<<<<< HEAD
#ifdef CONFIG_HAVE_HW_BREAKPOINT
		struct { /* breakpoint */
			struct arch_hw_breakpoint	info;
			struct list_head		bp_list;
=======
		struct { /* tracepoint */
			/* for tp_event->class */
			struct list_head	tp_list;
		};
		struct { /* amd_power */
			u64	pwr_acc;
			u64	ptsc;
		};
#ifdef CONFIG_HAVE_HW_BREAKPOINT
		struct { /* breakpoint */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Crufty hack to avoid the chicken and egg
			 * problem hw_breakpoint has with context
			 * creation and event initalization.
			 */
<<<<<<< HEAD
			struct task_struct		*bp_target;
		};
#endif
	};
	int				state;
	local64_t			prev_count;
	u64				sample_period;
	u64				last_period;
	local64_t			period_left;
	u64                             interrupts_seq;
	u64				interrupts;

	u64				freq_time_stamp;
	u64				freq_count_stamp;
#endif
};

/*
 * hw_perf_event::state flags
=======
			struct arch_hw_breakpoint	info;
			struct rhlist_head		bp_list;
		};
#endif
		struct { /* amd_iommu */
			u8	iommu_bank;
			u8	iommu_cntr;
			u16	padding;
			u64	conf;
			u64	conf1;
		};
	};
	/*
	 * If the event is a per task event, this will point to the task in
	 * question. See the comment in perf_event_alloc().
	 */
	struct task_struct		*target;

	/*
	 * PMU would store hardware filter configuration
	 * here.
	 */
	void				*addr_filters;

	/* Last sync'ed generation of filters */
	unsigned long			addr_filters_gen;

/*
 * hw_perf_event::state flags; used to track the PERF_EF_* state.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define PERF_HES_STOPPED	0x01 /* the counter is stopped */
#define PERF_HES_UPTODATE	0x02 /* event->count up-to-date */
#define PERF_HES_ARCH		0x04

<<<<<<< HEAD
struct perf_event;
=======
	int				state;

	/*
	 * The last observed hardware counter value, updated with a
	 * local64_cmpxchg() such that pmu::read() can be called nested.
	 */
	local64_t			prev_count;

	/*
	 * The period to start the next sample with.
	 */
	u64				sample_period;

	union {
		struct { /* Sampling */
			/*
			 * The period we started this sample with.
			 */
			u64				last_period;

			/*
			 * However much is left of the current period;
			 * note that this is a full 64bit value and
			 * allows for generation of periods longer
			 * than hardware might allow.
			 */
			local64_t			period_left;
		};
		struct { /* Topdown events counting for context switch */
			u64				saved_metric;
			u64				saved_slots;
		};
	};

	/*
	 * State for throttling the event, see __perf_event_overflow() and
	 * perf_adjust_freq_unthr_context().
	 */
	u64                             interrupts_seq;
	u64				interrupts;

	/*
	 * State for freq target events, see __perf_event_overflow() and
	 * perf_adjust_freq_unthr_context().
	 */
	u64				freq_time_stamp;
	u64				freq_count_stamp;
#endif
};

struct perf_event;
struct perf_event_pmu_context;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Common implementation detail of pmu::{start,commit,cancel}_txn
 */
<<<<<<< HEAD
#define PERF_EVENT_TXN 0x1
=======
#define PERF_PMU_TXN_ADD  0x1		/* txn to add/schedule event on PMU */
#define PERF_PMU_TXN_READ 0x2		/* txn to read event group from PMU */

/**
 * pmu::capabilities flags
 */
#define PERF_PMU_CAP_NO_INTERRUPT		0x0001
#define PERF_PMU_CAP_NO_NMI			0x0002
#define PERF_PMU_CAP_AUX_NO_SG			0x0004
#define PERF_PMU_CAP_EXTENDED_REGS		0x0008
#define PERF_PMU_CAP_EXCLUSIVE			0x0010
#define PERF_PMU_CAP_ITRACE			0x0020
#define PERF_PMU_CAP_NO_EXCLUDE			0x0040
#define PERF_PMU_CAP_AUX_OUTPUT			0x0080
#define PERF_PMU_CAP_EXTENDED_HW_TYPE		0x0100

struct perf_output_handle;

#define PMU_NULL_DEV	((void *)(~0UL))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct pmu - generic performance monitoring unit
 */
struct pmu {
	struct list_head		entry;

<<<<<<< HEAD
	struct device			*dev;
	const struct attribute_group	**attr_groups;
	char				*name;
	int				type;

	int * __percpu			pmu_disable_count;
	struct perf_cpu_context * __percpu pmu_cpu_context;
	int				task_ctx_nr;
	u32                             events_across_hotplug:1,
					reserved:31;
=======
	struct module			*module;
	struct device			*dev;
	struct device			*parent;
	const struct attribute_group	**attr_groups;
	const struct attribute_group	**attr_update;
	const char			*name;
	int				type;

	/*
	 * various common per-pmu feature flags
	 */
	int				capabilities;

	int __percpu			*pmu_disable_count;
	struct perf_cpu_pmu_context __percpu *cpu_pmu_context;
	atomic_t			exclusive_cnt; /* < 0: cpu; > 0: tsk */
	int				task_ctx_nr;
	int				hrtimer_interval_ms;

	/* number of address filters this PMU can do */
	unsigned int			nr_addr_filters;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Fully disable/enable this PMU, can be used to protect from the PMI
	 * as well as for lazy/batch writing of the MSRs.
	 */
	void (*pmu_enable)		(struct pmu *pmu); /* optional */
	void (*pmu_disable)		(struct pmu *pmu); /* optional */

	/*
	 * Try and initialize the event for this PMU.
<<<<<<< HEAD
	 * Should return -ENOENT when the @event doesn't match this PMU.
	 */
	int (*event_init)		(struct perf_event *event);

=======
	 *
	 * Returns:
	 *  -ENOENT	-- @event is not for this PMU
	 *
	 *  -ENODEV	-- @event is for this PMU but PMU not present
	 *  -EBUSY	-- @event is for this PMU but PMU temporarily unavailable
	 *  -EINVAL	-- @event is for this PMU but @event is not valid
	 *  -EOPNOTSUPP -- @event is for this PMU, @event is valid, but not supported
	 *  -EACCES	-- @event is for this PMU, @event is valid, but no privileges
	 *
	 *  0		-- @event is for this PMU and valid
	 *
	 * Other error return values are allowed.
	 */
	int (*event_init)		(struct perf_event *event);

	/*
	 * Notification that the event was mapped or unmapped.  Called
	 * in the context of the mapping task.
	 */
	void (*event_mapped)		(struct perf_event *event, struct mm_struct *mm); /* optional */
	void (*event_unmapped)		(struct perf_event *event, struct mm_struct *mm); /* optional */

	/*
	 * Flags for ->add()/->del()/ ->start()/->stop(). There are
	 * matching hw_perf_event::state flags.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PERF_EF_START	0x01		/* start the counter when adding    */
#define PERF_EF_RELOAD	0x02		/* reload the counter when starting */
#define PERF_EF_UPDATE	0x04		/* update the counter when stopping */

	/*
<<<<<<< HEAD
	 * Adds/Removes a counter to/from the PMU, can be done inside
	 * a transaction, see the ->*_txn() methods.
=======
	 * Adds/Removes a counter to/from the PMU, can be done inside a
	 * transaction, see the ->*_txn() methods.
	 *
	 * The add/del callbacks will reserve all hardware resources required
	 * to service the event, this includes any counter constraint
	 * scheduling etc.
	 *
	 * Called with IRQs disabled and the PMU disabled on the CPU the event
	 * is on.
	 *
	 * ->add() called without PERF_EF_START should result in the same state
	 *  as ->add() followed by ->stop().
	 *
	 * ->del() must always PERF_EF_UPDATE stop an event. If it calls
	 *  ->stop() that must deal with already being stopped without
	 *  PERF_EF_UPDATE.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	int  (*add)			(struct perf_event *event, int flags);
	void (*del)			(struct perf_event *event, int flags);

	/*
<<<<<<< HEAD
	 * Starts/Stops a counter present on the PMU. The PMI handler
	 * should stop the counter when perf_event_overflow() returns
	 * !0. ->start() will be used to continue.
=======
	 * Starts/Stops a counter present on the PMU.
	 *
	 * The PMI handler should stop the counter when perf_event_overflow()
	 * returns !0. ->start() will be used to continue.
	 *
	 * Also used to change the sample period.
	 *
	 * Called with IRQs disabled and the PMU disabled on the CPU the event
	 * is on -- will be called from NMI context with the PMU generates
	 * NMIs.
	 *
	 * ->stop() with PERF_EF_UPDATE will read the counter and update
	 *  period/count values like ->read() would.
	 *
	 * ->start() with PERF_EF_RELOAD will reprogram the counter
	 *  value, must be preceded by a ->stop() with PERF_EF_UPDATE.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	void (*start)			(struct perf_event *event, int flags);
	void (*stop)			(struct perf_event *event, int flags);

	/*
	 * Updates the counter value of the event.
<<<<<<< HEAD
=======
	 *
	 * For sampling capable PMUs this will also update the software period
	 * hw_perf_event::period_left field.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	void (*read)			(struct perf_event *event);

	/*
	 * Group events scheduling is treated as a transaction, add
	 * group events as a whole and perform one schedulability test.
	 * If the test fails, roll back the whole group
	 *
	 * Start the transaction, after this ->add() doesn't need to
	 * do schedulability tests.
<<<<<<< HEAD
	 */
	void (*start_txn)		(struct pmu *pmu); /* optional */
=======
	 *
	 * Optional.
	 */
	void (*start_txn)		(struct pmu *pmu, unsigned int txn_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If ->start_txn() disabled the ->add() schedulability test
	 * then ->commit_txn() is required to perform one. On success
	 * the transaction is closed. On error the transaction is kept
	 * open until ->cancel_txn() is called.
<<<<<<< HEAD
	 */
	int  (*commit_txn)		(struct pmu *pmu); /* optional */
	/*
	 * Will cancel the transaction, assumes ->del() is called
	 * for each successful ->add() during the transaction.
	 */
	void (*cancel_txn)		(struct pmu *pmu); /* optional */

	/*
	 * Will return the value for perf_event_mmap_page::index for this event,
	 * if no implementation is provided it will default to: event->hw.idx + 1.
=======
	 *
	 * Optional.
	 */
	int  (*commit_txn)		(struct pmu *pmu);
	/*
	 * Will cancel the transaction, assumes ->del() is called
	 * for each successful ->add() during the transaction.
	 *
	 * Optional.
	 */
	void (*cancel_txn)		(struct pmu *pmu);

	/*
	 * Will return the value for perf_event_mmap_page::index for this event,
	 * if no implementation is provided it will default to 0 (see
	 * perf_event_idx_default).
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	int (*event_idx)		(struct perf_event *event); /*optional */

	/*
<<<<<<< HEAD
	 * flush branch stack on context-switches (needed in cpu-wide mode)
	 */
	void (*flush_branch_stack)	(void);
};

/**
 * enum perf_event_active_state - the states of a event
 */
enum perf_event_active_state {
=======
	 * context-switches callback
	 */
	void (*sched_task)		(struct perf_event_pmu_context *pmu_ctx,
					bool sched_in);

	/*
	 * Kmem cache of PMU specific data
	 */
	struct kmem_cache		*task_ctx_cache;

	/*
	 * PMU specific parts of task perf event context (i.e. ctx->task_ctx_data)
	 * can be synchronized using this function. See Intel LBR callstack support
	 * implementation and Perf core context switch handling callbacks for usage
	 * examples.
	 */
	void (*swap_task_ctx)		(struct perf_event_pmu_context *prev_epc,
					 struct perf_event_pmu_context *next_epc);
					/* optional */

	/*
	 * Set up pmu-private data structures for an AUX area
	 */
	void *(*setup_aux)		(struct perf_event *event, void **pages,
					 int nr_pages, bool overwrite);
					/* optional */

	/*
	 * Free pmu-private AUX data structures
	 */
	void (*free_aux)		(void *aux); /* optional */

	/*
	 * Take a snapshot of the AUX buffer without touching the event
	 * state, so that preempting ->start()/->stop() callbacks does
	 * not interfere with their logic. Called in PMI context.
	 *
	 * Returns the size of AUX data copied to the output handle.
	 *
	 * Optional.
	 */
	long (*snapshot_aux)		(struct perf_event *event,
					 struct perf_output_handle *handle,
					 unsigned long size);

	/*
	 * Validate address range filters: make sure the HW supports the
	 * requested configuration and number of filters; return 0 if the
	 * supplied filters are valid, -errno otherwise.
	 *
	 * Runs in the context of the ioctl()ing process and is not serialized
	 * with the rest of the PMU callbacks.
	 */
	int (*addr_filters_validate)	(struct list_head *filters);
					/* optional */

	/*
	 * Synchronize address range filter configuration:
	 * translate hw-agnostic filters into hardware configuration in
	 * event::hw::addr_filters.
	 *
	 * Runs as a part of filter sync sequence that is done in ->start()
	 * callback by calling perf_event_addr_filters_sync().
	 *
	 * May (and should) traverse event::addr_filters::list, for which its
	 * caller provides necessary serialization.
	 */
	void (*addr_filters_sync)	(struct perf_event *event);
					/* optional */

	/*
	 * Check if event can be used for aux_output purposes for
	 * events of this PMU.
	 *
	 * Runs from perf_event_open(). Should return 0 for "no match"
	 * or non-zero for "match".
	 */
	int (*aux_output_match)		(struct perf_event *event);
					/* optional */

	/*
	 * Skip programming this PMU on the given CPU. Typically needed for
	 * big.LITTLE things.
	 */
	bool (*filter)			(struct pmu *pmu, int cpu); /* optional */

	/*
	 * Check period value for PERF_EVENT_IOC_PERIOD ioctl.
	 */
	int (*check_period)		(struct perf_event *event, u64 value); /* optional */
};

enum perf_addr_filter_action_t {
	PERF_ADDR_FILTER_ACTION_STOP = 0,
	PERF_ADDR_FILTER_ACTION_START,
	PERF_ADDR_FILTER_ACTION_FILTER,
};

/**
 * struct perf_addr_filter - address range filter definition
 * @entry:	event's filter list linkage
 * @path:	object file's path for file-based filters
 * @offset:	filter range offset
 * @size:	filter range size (size==0 means single address trigger)
 * @action:	filter/start/stop
 *
 * This is a hardware-agnostic filter configuration as specified by the user.
 */
struct perf_addr_filter {
	struct list_head	entry;
	struct path		path;
	unsigned long		offset;
	unsigned long		size;
	enum perf_addr_filter_action_t	action;
};

/**
 * struct perf_addr_filters_head - container for address range filters
 * @list:	list of filters for this event
 * @lock:	spinlock that serializes accesses to the @list and event's
 *		(and its children's) filter generations.
 * @nr_file_filters:	number of file-based filters
 *
 * A child event will use parent's @list (and therefore @lock), so they are
 * bundled together; see perf_event_addr_filters().
 */
struct perf_addr_filters_head {
	struct list_head	list;
	raw_spinlock_t		lock;
	unsigned int		nr_file_filters;
};

struct perf_addr_filter_range {
	unsigned long		start;
	unsigned long		size;
};

/**
 * enum perf_event_state - the states of an event:
 */
enum perf_event_state {
	PERF_EVENT_STATE_DEAD		= -4,
	PERF_EVENT_STATE_EXIT		= -3,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	PERF_EVENT_STATE_ERROR		= -2,
	PERF_EVENT_STATE_OFF		= -1,
	PERF_EVENT_STATE_INACTIVE	=  0,
	PERF_EVENT_STATE_ACTIVE		=  1,
};

struct file;
struct perf_sample_data;

typedef void (*perf_overflow_handler_t)(struct perf_event *,
					struct perf_sample_data *,
					struct pt_regs *regs);

<<<<<<< HEAD
enum perf_group_flag {
	PERF_GROUP_SOFTWARE		= 0x1,
};
=======
/*
 * Event capabilities. For event_caps and groups caps.
 *
 * PERF_EV_CAP_SOFTWARE: Is a software event.
 * PERF_EV_CAP_READ_ACTIVE_PKG: A CPU event (or cgroup event) that can be read
 * from any CPU in the package where it is active.
 * PERF_EV_CAP_SIBLING: An event with this flag must be a group sibling and
 * cannot be a group leader. If an event with this flag is detached from the
 * group it is scheduled out and moved into an unrecoverable ERROR state.
 */
#define PERF_EV_CAP_SOFTWARE		BIT(0)
#define PERF_EV_CAP_READ_ACTIVE_PKG	BIT(1)
#define PERF_EV_CAP_SIBLING		BIT(2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SWEVENT_HLIST_BITS		8
#define SWEVENT_HLIST_SIZE		(1 << SWEVENT_HLIST_BITS)

struct swevent_hlist {
	struct hlist_head		heads[SWEVENT_HLIST_SIZE];
	struct rcu_head			rcu_head;
};

#define PERF_ATTACH_CONTEXT	0x01
#define PERF_ATTACH_GROUP	0x02
#define PERF_ATTACH_TASK	0x04
<<<<<<< HEAD

#ifdef CONFIG_CGROUP_PERF
/*
 * perf_cgroup_info keeps track of time_enabled for a cgroup.
 * This is a per-cpu dynamically allocated data structure.
 */
struct perf_cgroup_info {
	u64				time;
	u64				timestamp;
};

struct perf_cgroup {
	struct				cgroup_subsys_state css;
	struct				perf_cgroup_info *info;	/* timing info, one per cpu */
};
#endif

struct ring_buffer;
=======
#define PERF_ATTACH_TASK_DATA	0x08
#define PERF_ATTACH_ITRACE	0x10
#define PERF_ATTACH_SCHED_CB	0x20
#define PERF_ATTACH_CHILD	0x40

struct bpf_prog;
struct perf_cgroup;
struct perf_buffer;

struct pmu_event_list {
	raw_spinlock_t		lock;
	struct list_head	list;
};

/*
 * event->sibling_list is modified whole holding both ctx->lock and ctx->mutex
 * as such iteration must hold either lock. However, since ctx->lock is an IRQ
 * safe lock, and is only held by the CPU doing the modification, having IRQs
 * disabled is sufficient since it will hold-off the IPIs.
 */
#ifdef CONFIG_PROVE_LOCKING
#define lockdep_assert_event_ctx(event)				\
	WARN_ON_ONCE(__lockdep_enabled &&			\
		     (this_cpu_read(hardirqs_enabled) &&	\
		      lockdep_is_held(&(event)->ctx->mutex) != LOCK_STATE_HELD))
#else
#define lockdep_assert_event_ctx(event)
#endif

#define for_each_sibling_event(sibling, event)			\
	lockdep_assert_event_ctx(event);			\
	if ((event)->group_leader == (event))			\
		list_for_each_entry((sibling), &(event)->sibling_list, sibling_list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct perf_event - performance event kernel representation:
 */
struct perf_event {
#ifdef CONFIG_PERF_EVENTS
<<<<<<< HEAD
	struct list_head		group_entry;
	struct list_head		event_entry;
	struct list_head		sibling_list;
	struct hlist_node		hlist_entry;
	int				nr_siblings;
	int				group_flags;
	struct perf_event		*group_leader;

	/*
	 * Protect the pmu, attributes and context of a group leader.
	 * Note: does not protect the pointer to the group_leader.
	 */
	struct mutex			group_leader_mutex;
	struct pmu			*pmu;

	enum perf_event_active_state	state;
=======
	/*
	 * entry onto perf_event_context::event_list;
	 *   modifications require ctx->lock
	 *   RCU safe iterations.
	 */
	struct list_head		event_entry;

	/*
	 * Locked for modification by both ctx->mutex and ctx->lock; holding
	 * either sufficies for read.
	 */
	struct list_head		sibling_list;
	struct list_head		active_list;
	/*
	 * Node on the pinned or flexible tree located at the event context;
	 */
	struct rb_node			group_node;
	u64				group_index;
	/*
	 * We need storage to track the entries in perf_pmu_migrate_context; we
	 * cannot use the event_entry because of RCU and we want to keep the
	 * group in tact which avoids us using the other two entries.
	 */
	struct list_head		migrate_entry;

	struct hlist_node		hlist_entry;
	struct list_head		active_entry;
	int				nr_siblings;

	/* Not serialized. Only written during event initialization. */
	int				event_caps;
	/* The cumulative AND of all event_caps for events in this group. */
	int				group_caps;

	unsigned int			group_generation;
	struct perf_event		*group_leader;
	/*
	 * event->pmu will always point to pmu in which this event belongs.
	 * Whereas event->pmu_ctx->pmu may point to other pmu when group of
	 * different pmu events is created.
	 */
	struct pmu			*pmu;
	void				*pmu_private;

	enum perf_event_state		state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int			attach_state;
	local64_t			count;
	atomic64_t			child_count;

	/*
	 * These are the total time in nanoseconds that the event
	 * has been enabled (i.e. eligible to run, and the task has
	 * been scheduled in, if this is a per-task event)
	 * and running (scheduled onto the CPU), respectively.
<<<<<<< HEAD
	 *
	 * They are computed from tstamp_enabled, tstamp_running and
	 * tstamp_stopped when the event is in INACTIVE or ACTIVE state.
	 */
	u64				total_time_enabled;
	u64				total_time_running;

	/*
	 * These are timestamps used for computing total_time_enabled
	 * and total_time_running when the event is in INACTIVE or
	 * ACTIVE state, measured in nanoseconds from an arbitrary point
	 * in time.
	 * tstamp_enabled: the notional time when the event was enabled
	 * tstamp_running: the notional time when the event was scheduled on
	 * tstamp_stopped: in INACTIVE state, the notional time when the
	 *	event was scheduled off.
	 */
	u64				tstamp_enabled;
	u64				tstamp_running;
	u64				tstamp_stopped;

	/*
	 * timestamp shadows the actual context timing but it can
	 * be safely used in NMI interrupt context. It reflects the
	 * context time as it was when the event was last scheduled in.
	 *
	 * ctx_time already accounts for ctx->timestamp. Therefore to
	 * compute ctx_time for a sample, simply add perf_clock().
	 */
	u64				shadow_ctx_time;
=======
	 */
	u64				total_time_enabled;
	u64				total_time_running;
	u64				tstamp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct perf_event_attr		attr;
	u16				header_size;
	u16				id_header_size;
	u16				read_size;
	struct hw_perf_event		hw;

	struct perf_event_context	*ctx;
<<<<<<< HEAD
=======
	/*
	 * event->pmu_ctx points to perf_event_pmu_context in which the event
	 * is added. This pmu_ctx can be of other pmu for sw event when that
	 * sw event is part of a group which also contains non-sw events.
	 */
	struct perf_event_pmu_context	*pmu_ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_long_t			refcount;

	/*
	 * These accumulate total time (in nanoseconds) that children
	 * events have been enabled and running, respectively.
	 */
	atomic64_t			child_total_time_enabled;
	atomic64_t			child_total_time_running;

	/*
	 * Protect attach/detach and child_list:
	 */
	struct mutex			child_mutex;
	struct list_head		child_list;
	struct perf_event		*parent;

	int				oncpu;
	int				cpu;

	struct list_head		owner_entry;
	struct task_struct		*owner;

	/* mmap bits */
	struct mutex			mmap_mutex;
	atomic_t			mmap_count;

<<<<<<< HEAD
	struct ring_buffer		*rb;
	struct list_head		rb_entry;
=======
	struct perf_buffer		*rb;
	struct list_head		rb_entry;
	unsigned long			rcu_batches;
	int				rcu_pending;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* poll related */
	wait_queue_head_t		waitq;
	struct fasync_struct		*fasync;

	/* delayed work for NMIs and such */
<<<<<<< HEAD
	int				pending_wakeup;
	int				pending_kill;
	int				pending_disable;
	struct irq_work			pending;

	atomic_t			event_limit;

=======
	unsigned int			pending_wakeup;
	unsigned int			pending_kill;
	unsigned int			pending_disable;
	unsigned int			pending_sigtrap;
	unsigned long			pending_addr;	/* SIGTRAP */
	struct irq_work			pending_irq;
	struct callback_head		pending_task;
	unsigned int			pending_work;

	atomic_t			event_limit;

	/* address range filters */
	struct perf_addr_filters_head	addr_filters;
	/* vma address array for file-based filders */
	struct perf_addr_filter_range	*addr_filter_ranges;
	unsigned long			addr_filters_gen;

	/* for aux_output events */
	struct perf_event		*aux_event;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void (*destroy)(struct perf_event *);
	struct rcu_head			rcu_head;

	struct pid_namespace		*ns;
	u64				id;

<<<<<<< HEAD
	perf_overflow_handler_t		overflow_handler;
	void				*overflow_handler_context;

#ifdef CONFIG_EVENT_TRACING
	struct ftrace_event_call	*tp_event;
=======
	atomic64_t			lost_samples;

	u64				(*clock)(void);
	perf_overflow_handler_t		overflow_handler;
	void				*overflow_handler_context;
#ifdef CONFIG_BPF_SYSCALL
	perf_overflow_handler_t		orig_overflow_handler;
	struct bpf_prog			*prog;
	u64				bpf_cookie;
#endif

#ifdef CONFIG_EVENT_TRACING
	struct trace_event_call		*tp_event;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct event_filter		*filter;
#ifdef CONFIG_FUNCTION_TRACER
	struct ftrace_ops               ftrace_ops;
#endif
#endif

#ifdef CONFIG_CGROUP_PERF
	struct perf_cgroup		*cgrp; /* cgroup event is attach to */
<<<<<<< HEAD
	int				cgrp_defer_enabled;
#endif

#endif /* CONFIG_PERF_EVENTS */
};

=======
#endif

#ifdef CONFIG_SECURITY
	void *security;
#endif
	struct list_head		sb_list;

	/*
	 * Certain events gets forwarded to another pmu internally by over-
	 * writing kernel copy of event->attr.type without user being aware
	 * of it. event->orig_type contains original 'type' requested by
	 * user.
	 */
	__u32				orig_type;
#endif /* CONFIG_PERF_EVENTS */
};

/*
 *           ,-----------------------[1:n]------------------------.
 *           V                                                    V
 * perf_event_context <-[1:n]-> perf_event_pmu_context <-[1:n]- perf_event
 *                                        |                       |
 *                                        `--[n:1]-> pmu <-[1:n]--'
 *
 *
 * struct perf_event_pmu_context  lifetime is refcount based and RCU freed
 * (similar to perf_event_context). Locking is as if it were a member of
 * perf_event_context; specifically:
 *
 *   modification, both: ctx->mutex && ctx->lock
 *   reading, either:    ctx->mutex || ctx->lock
 *
 * There is one exception to this; namely put_pmu_ctx() isn't always called
 * with ctx->mutex held; this means that as long as we can guarantee the epc
 * has events the above rules hold.
 *
 * Specificially, sys_perf_event_open()'s group_leader case depends on
 * ctx->mutex pinning the configuration. Since we hold a reference on
 * group_leader (through the filedesc) it can't go away, therefore it's
 * associated pmu_ctx must exist and cannot change due to ctx->mutex.
 *
 * perf_event holds a refcount on perf_event_context
 * perf_event holds a refcount on perf_event_pmu_context
 */
struct perf_event_pmu_context {
	struct pmu			*pmu;
	struct perf_event_context       *ctx;

	struct list_head		pmu_ctx_entry;

	struct list_head		pinned_active;
	struct list_head		flexible_active;

	/* Used to avoid freeing per-cpu perf_event_pmu_context */
	unsigned int			embedded : 1;

	unsigned int			nr_events;
	unsigned int			nr_cgroups;

	atomic_t			refcount; /* event <-> epc */
	struct rcu_head			rcu_head;

	void				*task_ctx_data; /* pmu specific data */
	/*
	 * Set when one or more (plausibly active) event can't be scheduled
	 * due to pmu overcommit or pmu constraints, except tolerant to
	 * events not necessary to be active due to scheduling constraints,
	 * such as cgroups.
	 */
	int				rotate_necessary;
};

struct perf_event_groups {
	struct rb_root	tree;
	u64		index;
};


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * struct perf_event_context - event context structure
 *
 * Used as a container for task events and CPU events as well:
 */
struct perf_event_context {
<<<<<<< HEAD
	struct pmu			*pmu;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Protect the states of the events in the list,
	 * nr_active, and the list:
	 */
	raw_spinlock_t			lock;
	/*
	 * Protect the list of events.  Locking either mutex or lock
	 * is sufficient to ensure the list doesn't change; to change
	 * the list you need to lock both the mutex and the spinlock.
	 */
	struct mutex			mutex;

<<<<<<< HEAD
	struct list_head		pinned_groups;
	struct list_head		flexible_groups;
	struct list_head		event_list;
	int				nr_events;
	int				nr_active;
	int				is_active;
	int				nr_stat;
	int				nr_freq;
	int				rotate_disable;
	atomic_t			refcount;
=======
	struct list_head		pmu_ctx_list;
	struct perf_event_groups	pinned_groups;
	struct perf_event_groups	flexible_groups;
	struct list_head		event_list;

	int				nr_events;
	int				nr_user;
	int				is_active;

	int				nr_task_data;
	int				nr_stat;
	int				nr_freq;
	int				rotate_disable;

	refcount_t			refcount; /* event <-> ctx */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct		*task;

	/*
	 * Context clock, runs when context enabled.
	 */
	u64				time;
	u64				timestamp;
<<<<<<< HEAD
=======
	u64				timeoffset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * These fields let us detect when two contexts have both
	 * been cloned (inherited) from a common ancestor.
	 */
	struct perf_event_context	*parent_ctx;
	u64				parent_gen;
	u64				generation;
	int				pin_count;
<<<<<<< HEAD
	int				nr_cgroups;	 /* cgroup evts */
	int				nr_branch_stack; /* branch_stack evt */
	struct rcu_head			rcu_head;
=======
#ifdef CONFIG_CGROUP_PERF
	int				nr_cgroups;	 /* cgroup evts */
#endif
	struct rcu_head			rcu_head;

	/*
	 * Sum (event->pending_sigtrap + event->pending_work)
	 *
	 * The SIGTRAP is targeted at ctx->task, as such it won't do changing
	 * that until the signal is delivered.
	 */
	local_t				nr_pending;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Number of contexts where an event can trigger:
 *	task, softirq, hardirq, nmi.
 */
#define PERF_NR_CONTEXTS	4

<<<<<<< HEAD
=======
struct perf_cpu_pmu_context {
	struct perf_event_pmu_context	epc;
	struct perf_event_pmu_context	*task_epc;

	struct list_head		sched_cb_entry;
	int				sched_cb_usage;

	int				active_oncpu;
	int				exclusive;

	raw_spinlock_t			hrtimer_lock;
	struct hrtimer			hrtimer;
	ktime_t				hrtimer_interval;
	unsigned int			hrtimer_active;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * struct perf_event_cpu_context - per cpu event context structure
 */
struct perf_cpu_context {
	struct perf_event_context	ctx;
	struct perf_event_context	*task_ctx;
<<<<<<< HEAD
	int				active_oncpu;
	int				exclusive;
	struct list_head		rotation_list;
	int				jiffies_interval;
	struct pmu			*unique_pmu;
	struct perf_cgroup		*cgrp;
=======
	int				online;

#ifdef CONFIG_CGROUP_PERF
	struct perf_cgroup		*cgrp;
#endif

	/*
	 * Per-CPU storage for iterators used in visit_groups_merge. The default
	 * storage is of size 2 to hold the CPU and any CPU event iterators.
	 */
	int				heap_size;
	struct perf_event		**heap;
	struct perf_event		*heap_default[2];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct perf_output_handle {
	struct perf_event		*event;
<<<<<<< HEAD
	struct ring_buffer		*rb;
	unsigned long			wakeup;
	unsigned long			size;
	void				*addr;
	int				page;
};

#ifdef CONFIG_PERF_EVENTS

extern int perf_pmu_register(struct pmu *pmu, char *name, int type);
extern void perf_pmu_unregister(struct pmu *pmu);

extern int perf_num_counters(void);
extern const char *perf_pmu_name(void);
=======
	struct perf_buffer		*rb;
	unsigned long			wakeup;
	unsigned long			size;
	u64				aux_flags;
	union {
		void			*addr;
		unsigned long		head;
	};
	int				page;
};

struct bpf_perf_event_data_kern {
	bpf_user_pt_regs_t *regs;
	struct perf_sample_data *data;
	struct perf_event *event;
};

#ifdef CONFIG_CGROUP_PERF

/*
 * perf_cgroup_info keeps track of time_enabled for a cgroup.
 * This is a per-cpu dynamically allocated data structure.
 */
struct perf_cgroup_info {
	u64				time;
	u64				timestamp;
	u64				timeoffset;
	int				active;
};

struct perf_cgroup {
	struct cgroup_subsys_state	css;
	struct perf_cgroup_info	__percpu *info;
};

/*
 * Must ensure cgroup is pinned (css_get) before calling
 * this function. In other words, we cannot call this function
 * if there is no cgroup event for the current CPU context.
 */
static inline struct perf_cgroup *
perf_cgroup_from_task(struct task_struct *task, struct perf_event_context *ctx)
{
	return container_of(task_css_check(task, perf_event_cgrp_id,
					   ctx ? lockdep_is_held(&ctx->lock)
					       : true),
			    struct perf_cgroup, css);
}
#endif /* CONFIG_CGROUP_PERF */

#ifdef CONFIG_PERF_EVENTS

extern struct perf_event_context *perf_cpu_task_ctx(void);

extern void *perf_aux_output_begin(struct perf_output_handle *handle,
				   struct perf_event *event);
extern void perf_aux_output_end(struct perf_output_handle *handle,
				unsigned long size);
extern int perf_aux_output_skip(struct perf_output_handle *handle,
				unsigned long size);
extern void *perf_get_aux(struct perf_output_handle *handle);
extern void perf_aux_output_flag(struct perf_output_handle *handle, u64 flags);
extern void perf_event_itrace_started(struct perf_event *event);

extern int perf_pmu_register(struct pmu *pmu, const char *name, int type);
extern void perf_pmu_unregister(struct pmu *pmu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void __perf_event_task_sched_in(struct task_struct *prev,
				       struct task_struct *task);
extern void __perf_event_task_sched_out(struct task_struct *prev,
					struct task_struct *next);
<<<<<<< HEAD
extern int perf_event_init_task(struct task_struct *child);
extern void perf_event_exit_task(struct task_struct *child);
extern void perf_event_free_task(struct task_struct *task);
extern void perf_event_delayed_put(struct task_struct *task);
extern void perf_event_print_debug(void);
extern void perf_pmu_disable(struct pmu *pmu);
extern void perf_pmu_enable(struct pmu *pmu);
extern int perf_event_task_disable(void);
extern int perf_event_task_enable(void);
=======
extern int perf_event_init_task(struct task_struct *child, u64 clone_flags);
extern void perf_event_exit_task(struct task_struct *child);
extern void perf_event_free_task(struct task_struct *task);
extern void perf_event_delayed_put(struct task_struct *task);
extern struct file *perf_event_get(unsigned int fd);
extern const struct perf_event *perf_get_event(struct file *file);
extern const struct perf_event_attr *perf_event_attrs(struct perf_event *event);
extern void perf_event_print_debug(void);
extern void perf_pmu_disable(struct pmu *pmu);
extern void perf_pmu_enable(struct pmu *pmu);
extern void perf_sched_cb_dec(struct pmu *pmu);
extern void perf_sched_cb_inc(struct pmu *pmu);
extern int perf_event_task_disable(void);
extern int perf_event_task_enable(void);

extern void perf_pmu_resched(struct pmu *pmu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int perf_event_refresh(struct perf_event *event, int refresh);
extern void perf_event_update_userpage(struct perf_event *event);
extern int perf_event_release_kernel(struct perf_event *event);
extern struct perf_event *
perf_event_create_kernel_counter(struct perf_event_attr *attr,
				int cpu,
				struct task_struct *task,
				perf_overflow_handler_t callback,
				void *context);
extern void perf_pmu_migrate_context(struct pmu *pmu,
				int src_cpu, int dst_cpu);
<<<<<<< HEAD
extern u64 perf_event_read_value(struct perf_event *event,
				 u64 *enabled, u64 *running);


struct perf_sample_data {
	u64				type;

	u64				ip;
=======
int perf_event_read_local(struct perf_event *event, u64 *value,
			  u64 *enabled, u64 *running);
extern u64 perf_event_read_value(struct perf_event *event,
				 u64 *enabled, u64 *running);

extern struct perf_callchain_entry *perf_callchain(struct perf_event *event, struct pt_regs *regs);

static inline bool branch_sample_no_flags(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_NO_FLAGS;
}

static inline bool branch_sample_no_cycles(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_NO_CYCLES;
}

static inline bool branch_sample_type(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_TYPE_SAVE;
}

static inline bool branch_sample_hw_index(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_HW_INDEX;
}

static inline bool branch_sample_priv(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_PRIV_SAVE;
}

static inline bool branch_sample_counters(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_COUNTERS;
}

static inline bool branch_sample_call_stack(const struct perf_event *event)
{
	return event->attr.branch_sample_type & PERF_SAMPLE_BRANCH_CALL_STACK;
}

struct perf_sample_data {
	/*
	 * Fields set by perf_sample_data_init() unconditionally,
	 * group so as to minimize the cachelines touched.
	 */
	u64				sample_flags;
	u64				period;
	u64				dyn_size;

	/*
	 * Fields commonly set by __perf_event_header__init_id(),
	 * group so as to minimize the cachelines touched.
	 */
	u64				type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct {
		u32	pid;
		u32	tid;
	}				tid_entry;
	u64				time;
<<<<<<< HEAD
	u64				addr;
	u64				id;
	u64				stream_id;
=======
	u64				id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct {
		u32	cpu;
		u32	reserved;
	}				cpu_entry;
<<<<<<< HEAD
	u64				period;
	struct perf_callchain_entry	*callchain;
	struct perf_raw_record		*raw;
	struct perf_branch_stack	*br_stack;
};

static inline void perf_sample_data_init(struct perf_sample_data *data, u64 addr)
{
	data->addr = addr;
	data->raw  = NULL;
	data->br_stack = NULL;
=======

	/*
	 * The other fields, optionally {set,used} by
	 * perf_{prepare,output}_sample().
	 */
	u64				ip;
	struct perf_callchain_entry	*callchain;
	struct perf_raw_record		*raw;
	struct perf_branch_stack	*br_stack;
	u64				*br_stack_cntr;
	union perf_sample_weight	weight;
	union  perf_mem_data_src	data_src;
	u64				txn;

	struct perf_regs		regs_user;
	struct perf_regs		regs_intr;
	u64				stack_user_size;

	u64				stream_id;
	u64				cgroup;
	u64				addr;
	u64				phys_addr;
	u64				data_page_size;
	u64				code_page_size;
	u64				aux_size;
} ____cacheline_aligned;

/* default value for data source */
#define PERF_MEM_NA (PERF_MEM_S(OP, NA)   |\
		    PERF_MEM_S(LVL, NA)   |\
		    PERF_MEM_S(SNOOP, NA) |\
		    PERF_MEM_S(LOCK, NA)  |\
		    PERF_MEM_S(TLB, NA)   |\
		    PERF_MEM_S(LVLNUM, NA))

static inline void perf_sample_data_init(struct perf_sample_data *data,
					 u64 addr, u64 period)
{
	/* remaining struct members initialized in perf_prepare_sample() */
	data->sample_flags = PERF_SAMPLE_PERIOD;
	data->period = period;
	data->dyn_size = 0;

	if (addr) {
		data->addr = addr;
		data->sample_flags |= PERF_SAMPLE_ADDR;
	}
}

static inline void perf_sample_save_callchain(struct perf_sample_data *data,
					      struct perf_event *event,
					      struct pt_regs *regs)
{
	int size = 1;

	data->callchain = perf_callchain(event, regs);
	size += data->callchain->nr;

	data->dyn_size += size * sizeof(u64);
	data->sample_flags |= PERF_SAMPLE_CALLCHAIN;
}

static inline void perf_sample_save_raw_data(struct perf_sample_data *data,
					     struct perf_raw_record *raw)
{
	struct perf_raw_frag *frag = &raw->frag;
	u32 sum = 0;
	int size;

	do {
		sum += frag->size;
		if (perf_raw_frag_last(frag))
			break;
		frag = frag->next;
	} while (1);

	size = round_up(sum + sizeof(u32), sizeof(u64));
	raw->size = size - sizeof(u32);
	frag->pad = raw->size - sum;

	data->raw = raw;
	data->dyn_size += size;
	data->sample_flags |= PERF_SAMPLE_RAW;
}

static inline void perf_sample_save_brstack(struct perf_sample_data *data,
					    struct perf_event *event,
					    struct perf_branch_stack *brs,
					    u64 *brs_cntr)
{
	int size = sizeof(u64); /* nr */

	if (branch_sample_hw_index(event))
		size += sizeof(u64);
	size += brs->nr * sizeof(struct perf_branch_entry);

	/*
	 * The extension space for counters is appended after the
	 * struct perf_branch_stack. It is used to store the occurrences
	 * of events of each branch.
	 */
	if (brs_cntr)
		size += brs->nr * sizeof(u64);

	data->br_stack = brs;
	data->br_stack_cntr = brs_cntr;
	data->dyn_size += size;
	data->sample_flags |= PERF_SAMPLE_BRANCH_STACK;
}

static inline u32 perf_sample_data_size(struct perf_sample_data *data,
					struct perf_event *event)
{
	u32 size = sizeof(struct perf_event_header);

	size += event->header_size + event->id_header_size;
	size += data->dyn_size;

	return size;
}

/*
 * Clear all bitfields in the perf_branch_entry.
 * The to and from fields are not cleared because they are
 * systematically modified by caller.
 */
static inline void perf_clear_branch_entry_bitfields(struct perf_branch_entry *br)
{
	br->mispred = 0;
	br->predicted = 0;
	br->in_tx = 0;
	br->abort = 0;
	br->cycles = 0;
	br->type = 0;
	br->spec = PERF_BR_SPEC_NA;
	br->reserved = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

extern void perf_output_sample(struct perf_output_handle *handle,
			       struct perf_event_header *header,
			       struct perf_sample_data *data,
			       struct perf_event *event);
<<<<<<< HEAD
extern void perf_prepare_sample(struct perf_event_header *header,
=======
extern void perf_prepare_sample(struct perf_sample_data *data,
				struct perf_event *event,
				struct pt_regs *regs);
extern void perf_prepare_header(struct perf_event_header *header,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct perf_sample_data *data,
				struct perf_event *event,
				struct pt_regs *regs);

extern int perf_event_overflow(struct perf_event *event,
				 struct perf_sample_data *data,
				 struct pt_regs *regs);

<<<<<<< HEAD
=======
extern void perf_event_output_forward(struct perf_event *event,
				     struct perf_sample_data *data,
				     struct pt_regs *regs);
extern void perf_event_output_backward(struct perf_event *event,
				       struct perf_sample_data *data,
				       struct pt_regs *regs);
extern int perf_event_output(struct perf_event *event,
			     struct perf_sample_data *data,
			     struct pt_regs *regs);

static inline bool
__is_default_overflow_handler(perf_overflow_handler_t overflow_handler)
{
	if (likely(overflow_handler == perf_event_output_forward))
		return true;
	if (unlikely(overflow_handler == perf_event_output_backward))
		return true;
	return false;
}

#define is_default_overflow_handler(event) \
	__is_default_overflow_handler((event)->overflow_handler)

#ifdef CONFIG_BPF_SYSCALL
static inline bool uses_default_overflow_handler(struct perf_event *event)
{
	if (likely(is_default_overflow_handler(event)))
		return true;

	return __is_default_overflow_handler(event->orig_overflow_handler);
}
#else
#define uses_default_overflow_handler(event) \
	is_default_overflow_handler(event)
#endif

extern void
perf_event_header__init_id(struct perf_event_header *header,
			   struct perf_sample_data *data,
			   struct perf_event *event);
extern void
perf_event__output_id_sample(struct perf_event *event,
			     struct perf_output_handle *handle,
			     struct perf_sample_data *sample);

extern void
perf_log_lost_samples(struct perf_event *event, u64 lost);

static inline bool event_has_any_exclude_flag(struct perf_event *event)
{
	struct perf_event_attr *attr = &event->attr;

	return attr->exclude_idle || attr->exclude_user ||
	       attr->exclude_kernel || attr->exclude_hv ||
	       attr->exclude_guest || attr->exclude_host;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline bool is_sampling_event(struct perf_event *event)
{
	return event->attr.sample_period != 0;
}

/*
 * Return 1 for a software event, 0 for a hardware event
 */
static inline int is_software_event(struct perf_event *event)
{
<<<<<<< HEAD
	return event->pmu->task_ctx_nr == perf_sw_context;
=======
	return event->event_caps & PERF_EV_CAP_SOFTWARE;
}

/*
 * Return 1 for event in sw context, 0 for event in hw context
 */
static inline int in_software_context(struct perf_event *event)
{
	return event->pmu_ctx->pmu->task_ctx_nr == perf_sw_context;
}

static inline int is_exclusive_pmu(struct pmu *pmu)
{
	return pmu->capabilities & PERF_PMU_CAP_EXCLUSIVE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

extern struct static_key perf_swevent_enabled[PERF_COUNT_SW_MAX];

<<<<<<< HEAD
=======
extern void ___perf_sw_event(u32, u64, struct pt_regs *, u64);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void __perf_sw_event(u32, u64, struct pt_regs *, u64);

#ifndef perf_arch_fetch_caller_regs
static inline void perf_arch_fetch_caller_regs(struct pt_regs *regs, unsigned long ip) { }
#endif

/*
<<<<<<< HEAD
 * Take a snapshot of the regs. Skip ip and frame pointer to
 * the nth caller. We only need a few of the regs:
 * - ip for PERF_SAMPLE_IP
 * - cs for user_mode() tests
 * - bp for callchains
 * - eflags, for future purposes, just in case
 */
static inline void perf_fetch_caller_regs(struct pt_regs *regs)
{
	memset(regs, 0, sizeof(*regs));

=======
 * When generating a perf sample in-line, instead of from an interrupt /
 * exception, we lack a pt_regs. This is typically used from software events
 * like: SW_CONTEXT_SWITCHES, SW_MIGRATIONS and the tie-in with tracepoints.
 *
 * We typically don't need a full set, but (for x86) do require:
 * - ip for PERF_SAMPLE_IP
 * - cs for user_mode() tests
 * - sp for PERF_SAMPLE_CALLCHAIN
 * - eflags for MISC bits and CALLCHAIN (see: perf_hw_regs())
 *
 * NOTE: assumes @regs is otherwise already 0 filled; this is important for
 * things like PERF_SAMPLE_REGS_INTR.
 */
static inline void perf_fetch_caller_regs(struct pt_regs *regs)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	perf_arch_fetch_caller_regs(regs, CALLER_ADDR0);
}

static __always_inline void
perf_sw_event(u32 event_id, u64 nr, struct pt_regs *regs, u64 addr)
{
<<<<<<< HEAD
	struct pt_regs hot_regs;

	if (static_key_false(&perf_swevent_enabled[event_id])) {
		if (!regs) {
			perf_fetch_caller_regs(&hot_regs);
			regs = &hot_regs;
		}
		__perf_sw_event(event_id, nr, regs, addr);
	}
}

extern struct static_key_deferred perf_sched_events;
=======
	if (static_key_false(&perf_swevent_enabled[event_id]))
		__perf_sw_event(event_id, nr, regs, addr);
}

DECLARE_PER_CPU(struct pt_regs, __perf_regs[4]);

/*
 * 'Special' version for the scheduler, it hard assumes no recursion,
 * which is guaranteed by us not actually scheduling inside other swevents
 * because those disable preemption.
 */
static __always_inline void __perf_sw_event_sched(u32 event_id, u64 nr, u64 addr)
{
	struct pt_regs *regs = this_cpu_ptr(&__perf_regs[0]);

	perf_fetch_caller_regs(regs);
	___perf_sw_event(event_id, nr, regs, addr);
}

extern struct static_key_false perf_sched_events;

static __always_inline bool __perf_sw_enabled(int swevt)
{
	return static_key_false(&perf_swevent_enabled[swevt]);
}

static inline void perf_event_task_migrate(struct task_struct *task)
{
	if (__perf_sw_enabled(PERF_COUNT_SW_CPU_MIGRATIONS))
		task->sched_migrated = 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void perf_event_task_sched_in(struct task_struct *prev,
					    struct task_struct *task)
{
<<<<<<< HEAD
	if (static_key_false(&perf_sched_events.key))
		__perf_event_task_sched_in(prev, task);
=======
	if (static_branch_unlikely(&perf_sched_events))
		__perf_event_task_sched_in(prev, task);

	if (__perf_sw_enabled(PERF_COUNT_SW_CPU_MIGRATIONS) &&
	    task->sched_migrated) {
		__perf_sw_event_sched(PERF_COUNT_SW_CPU_MIGRATIONS, 1, 0);
		task->sched_migrated = 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void perf_event_task_sched_out(struct task_struct *prev,
					     struct task_struct *next)
{
<<<<<<< HEAD
	perf_sw_event(PERF_COUNT_SW_CONTEXT_SWITCHES, 1, NULL, 0);

	if (static_key_false(&perf_sched_events.key))
=======
	if (__perf_sw_enabled(PERF_COUNT_SW_CONTEXT_SWITCHES))
		__perf_sw_event_sched(PERF_COUNT_SW_CONTEXT_SWITCHES, 1, 0);

#ifdef CONFIG_CGROUP_PERF
	if (__perf_sw_enabled(PERF_COUNT_SW_CGROUP_SWITCHES) &&
	    perf_cgroup_from_task(prev, NULL) !=
	    perf_cgroup_from_task(next, NULL))
		__perf_sw_event_sched(PERF_COUNT_SW_CGROUP_SWITCHES, 1, 0);
#endif

	if (static_branch_unlikely(&perf_sched_events))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__perf_event_task_sched_out(prev, next);
}

extern void perf_event_mmap(struct vm_area_struct *vma);
<<<<<<< HEAD
extern struct perf_guest_info_callbacks *perf_guest_cbs;
extern int perf_register_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);
extern int perf_unregister_guest_info_callbacks(struct perf_guest_info_callbacks *callbacks);

extern void perf_event_comm(struct task_struct *tsk);
extern void perf_event_fork(struct task_struct *tsk);
=======

extern void perf_event_ksymbol(u16 ksym_type, u64 addr, u32 len,
			       bool unregister, const char *sym);
extern void perf_event_bpf_event(struct bpf_prog *prog,
				 enum perf_bpf_event_type type,
				 u16 flags);

#ifdef CONFIG_GUEST_PERF_EVENTS
extern struct perf_guest_info_callbacks __rcu *perf_guest_cbs;

DECLARE_STATIC_CALL(__perf_guest_state, *perf_guest_cbs->state);
DECLARE_STATIC_CALL(__perf_guest_get_ip, *perf_guest_cbs->get_ip);
DECLARE_STATIC_CALL(__perf_guest_handle_intel_pt_intr, *perf_guest_cbs->handle_intel_pt_intr);

static inline unsigned int perf_guest_state(void)
{
	return static_call(__perf_guest_state)();
}
static inline unsigned long perf_guest_get_ip(void)
{
	return static_call(__perf_guest_get_ip)();
}
static inline unsigned int perf_guest_handle_intel_pt_intr(void)
{
	return static_call(__perf_guest_handle_intel_pt_intr)();
}
extern void perf_register_guest_info_callbacks(struct perf_guest_info_callbacks *cbs);
extern void perf_unregister_guest_info_callbacks(struct perf_guest_info_callbacks *cbs);
#else
static inline unsigned int perf_guest_state(void)		 { return 0; }
static inline unsigned long perf_guest_get_ip(void)		 { return 0; }
static inline unsigned int perf_guest_handle_intel_pt_intr(void) { return 0; }
#endif /* CONFIG_GUEST_PERF_EVENTS */

extern void perf_event_exec(void);
extern void perf_event_comm(struct task_struct *tsk, bool exec);
extern void perf_event_namespaces(struct task_struct *tsk);
extern void perf_event_fork(struct task_struct *tsk);
extern void perf_event_text_poke(const void *addr,
				 const void *old_bytes, size_t old_len,
				 const void *new_bytes, size_t new_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Callchains */
DECLARE_PER_CPU(struct perf_callchain_entry, perf_callchain_entry);

<<<<<<< HEAD
extern void perf_callchain_user(struct perf_callchain_entry *entry, struct pt_regs *regs);
extern void perf_callchain_kernel(struct perf_callchain_entry *entry, struct pt_regs *regs);

static inline void perf_callchain_store(struct perf_callchain_entry *entry, u64 ip)
{
	if (entry->nr < PERF_MAX_STACK_DEPTH)
		entry->ip[entry->nr++] = ip;
=======
extern void perf_callchain_user(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs);
extern void perf_callchain_kernel(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs);
extern struct perf_callchain_entry *
get_perf_callchain(struct pt_regs *regs, u32 init_nr, bool kernel, bool user,
		   u32 max_stack, bool crosstask, bool add_mark);
extern int get_callchain_buffers(int max_stack);
extern void put_callchain_buffers(void);
extern struct perf_callchain_entry *get_callchain_entry(int *rctx);
extern void put_callchain_entry(int rctx);

extern int sysctl_perf_event_max_stack;
extern int sysctl_perf_event_max_contexts_per_stack;

static inline int perf_callchain_store_context(struct perf_callchain_entry_ctx *ctx, u64 ip)
{
	if (ctx->contexts < sysctl_perf_event_max_contexts_per_stack) {
		struct perf_callchain_entry *entry = ctx->entry;
		entry->ip[entry->nr++] = ip;
		++ctx->contexts;
		return 0;
	} else {
		ctx->contexts_maxed = true;
		return -1; /* no more room, stop walking the stack */
	}
}

static inline int perf_callchain_store(struct perf_callchain_entry_ctx *ctx, u64 ip)
{
	if (ctx->nr < ctx->max_stack && !ctx->contexts_maxed) {
		struct perf_callchain_entry *entry = ctx->entry;
		entry->ip[entry->nr++] = ip;
		++ctx->nr;
		return 0;
	} else {
		return -1; /* no more room, stop walking the stack */
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

extern int sysctl_perf_event_paranoid;
extern int sysctl_perf_event_mlock;
extern int sysctl_perf_event_sample_rate;
<<<<<<< HEAD

extern int perf_proc_update_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos);

static inline bool perf_paranoid_any(void)
{
	return sysctl_perf_event_paranoid > 2;
}

static inline bool perf_paranoid_tracepoint_raw(void)
=======
extern int sysctl_perf_cpu_time_max_percent;

extern void perf_sample_event_took(u64 sample_len_ns);

int perf_event_max_sample_rate_handler(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos);
int perf_cpu_time_max_percent_handler(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos);
int perf_event_max_stack_handler(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos);

/* Access to perf_event_open(2) syscall. */
#define PERF_SECURITY_OPEN		0

/* Finer grained perf_event_open(2) access control. */
#define PERF_SECURITY_CPU		1
#define PERF_SECURITY_KERNEL		2
#define PERF_SECURITY_TRACEPOINT	3

static inline int perf_is_paranoid(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return sysctl_perf_event_paranoid > -1;
}

<<<<<<< HEAD
static inline bool perf_paranoid_cpu(void)
{
	return sysctl_perf_event_paranoid > 0;
}

static inline bool perf_paranoid_kernel(void)
{
	return sysctl_perf_event_paranoid > 1;
}

extern void perf_event_init(void);
extern void perf_tp_event(u64 addr, u64 count, void *record,
			  int entry_size, struct pt_regs *regs,
			  struct hlist_head *head, int rctx);
=======
static inline int perf_allow_kernel(struct perf_event_attr *attr)
{
	if (sysctl_perf_event_paranoid > 1 && !perfmon_capable())
		return -EACCES;

	return security_perf_event_open(attr, PERF_SECURITY_KERNEL);
}

static inline int perf_allow_cpu(struct perf_event_attr *attr)
{
	if (sysctl_perf_event_paranoid > 0 && !perfmon_capable())
		return -EACCES;

	return security_perf_event_open(attr, PERF_SECURITY_CPU);
}

static inline int perf_allow_tracepoint(struct perf_event_attr *attr)
{
	if (sysctl_perf_event_paranoid > -1 && !perfmon_capable())
		return -EPERM;

	return security_perf_event_open(attr, PERF_SECURITY_TRACEPOINT);
}

extern void perf_event_init(void);
extern void perf_tp_event(u16 event_type, u64 count, void *record,
			  int entry_size, struct pt_regs *regs,
			  struct hlist_head *head, int rctx,
			  struct task_struct *task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void perf_bp_event(struct perf_event *event, void *data);

#ifndef perf_misc_flags
# define perf_misc_flags(regs) \
		(user_mode(regs) ? PERF_RECORD_MISC_USER : PERF_RECORD_MISC_KERNEL)
# define perf_instruction_pointer(regs)	instruction_pointer(regs)
#endif
<<<<<<< HEAD
=======
#ifndef perf_arch_bpf_user_pt_regs
# define perf_arch_bpf_user_pt_regs(regs) regs
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline bool has_branch_stack(struct perf_event *event)
{
	return event->attr.sample_type & PERF_SAMPLE_BRANCH_STACK;
}

<<<<<<< HEAD
extern int perf_output_begin(struct perf_output_handle *handle,
			     struct perf_event *event, unsigned int size);
extern void perf_output_end(struct perf_output_handle *handle);
extern void perf_output_copy(struct perf_output_handle *handle,
			     const void *buf, unsigned int len);
extern int perf_swevent_get_recursion_context(void);
extern void perf_swevent_put_recursion_context(int rctx);
extern void perf_event_enable(struct perf_event *event);
extern void perf_event_disable(struct perf_event *event);
extern void perf_event_task_tick(void);
#else
=======
static inline bool needs_branch_stack(struct perf_event *event)
{
	return event->attr.branch_sample_type != 0;
}

static inline bool has_aux(struct perf_event *event)
{
	return event->pmu->setup_aux;
}

static inline bool is_write_backward(struct perf_event *event)
{
	return !!event->attr.write_backward;
}

static inline bool has_addr_filter(struct perf_event *event)
{
	return event->pmu->nr_addr_filters;
}

/*
 * An inherited event uses parent's filters
 */
static inline struct perf_addr_filters_head *
perf_event_addr_filters(struct perf_event *event)
{
	struct perf_addr_filters_head *ifh = &event->addr_filters;

	if (event->parent)
		ifh = &event->parent->addr_filters;

	return ifh;
}

extern void perf_event_addr_filters_sync(struct perf_event *event);
extern void perf_report_aux_output_id(struct perf_event *event, u64 hw_id);

extern int perf_output_begin(struct perf_output_handle *handle,
			     struct perf_sample_data *data,
			     struct perf_event *event, unsigned int size);
extern int perf_output_begin_forward(struct perf_output_handle *handle,
				     struct perf_sample_data *data,
				     struct perf_event *event,
				     unsigned int size);
extern int perf_output_begin_backward(struct perf_output_handle *handle,
				      struct perf_sample_data *data,
				      struct perf_event *event,
				      unsigned int size);

extern void perf_output_end(struct perf_output_handle *handle);
extern unsigned int perf_output_copy(struct perf_output_handle *handle,
			     const void *buf, unsigned int len);
extern unsigned int perf_output_skip(struct perf_output_handle *handle,
				     unsigned int len);
extern long perf_output_copy_aux(struct perf_output_handle *aux_handle,
				 struct perf_output_handle *handle,
				 unsigned long from, unsigned long to);
extern int perf_swevent_get_recursion_context(void);
extern void perf_swevent_put_recursion_context(int rctx);
extern u64 perf_swevent_set_period(struct perf_event *event);
extern void perf_event_enable(struct perf_event *event);
extern void perf_event_disable(struct perf_event *event);
extern void perf_event_disable_local(struct perf_event *event);
extern void perf_event_disable_inatomic(struct perf_event *event);
extern void perf_event_task_tick(void);
extern int perf_event_account_interrupt(struct perf_event *event);
extern int perf_event_period(struct perf_event *event, u64 value);
extern u64 perf_event_pause(struct perf_event *event, bool reset);
#else /* !CONFIG_PERF_EVENTS: */
static inline void *
perf_aux_output_begin(struct perf_output_handle *handle,
		      struct perf_event *event)				{ return NULL; }
static inline void
perf_aux_output_end(struct perf_output_handle *handle, unsigned long size)
									{ }
static inline int
perf_aux_output_skip(struct perf_output_handle *handle,
		     unsigned long size)				{ return -EINVAL; }
static inline void *
perf_get_aux(struct perf_output_handle *handle)				{ return NULL; }
static inline void
perf_event_task_migrate(struct task_struct *task)			{ }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void
perf_event_task_sched_in(struct task_struct *prev,
			 struct task_struct *task)			{ }
static inline void
perf_event_task_sched_out(struct task_struct *prev,
			  struct task_struct *next)			{ }
<<<<<<< HEAD
static inline int perf_event_init_task(struct task_struct *child)	{ return 0; }
static inline void perf_event_exit_task(struct task_struct *child)	{ }
static inline void perf_event_free_task(struct task_struct *task)	{ }
static inline void perf_event_delayed_put(struct task_struct *task)	{ }
=======
static inline int perf_event_init_task(struct task_struct *child,
				       u64 clone_flags)			{ return 0; }
static inline void perf_event_exit_task(struct task_struct *child)	{ }
static inline void perf_event_free_task(struct task_struct *task)	{ }
static inline void perf_event_delayed_put(struct task_struct *task)	{ }
static inline struct file *perf_event_get(unsigned int fd)	{ return ERR_PTR(-EINVAL); }
static inline const struct perf_event *perf_get_event(struct file *file)
{
	return ERR_PTR(-EINVAL);
}
static inline const struct perf_event_attr *perf_event_attrs(struct perf_event *event)
{
	return ERR_PTR(-EINVAL);
}
static inline int perf_event_read_local(struct perf_event *event, u64 *value,
					u64 *enabled, u64 *running)
{
	return -EINVAL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void perf_event_print_debug(void)				{ }
static inline int perf_event_task_disable(void)				{ return -EINVAL; }
static inline int perf_event_task_enable(void)				{ return -EINVAL; }
static inline int perf_event_refresh(struct perf_event *event, int refresh)
{
	return -EINVAL;
}

static inline void
perf_sw_event(u32 event_id, u64 nr, struct pt_regs *regs, u64 addr)	{ }
static inline void
perf_bp_event(struct perf_event *event, void *data)			{ }

<<<<<<< HEAD
static inline int perf_register_guest_info_callbacks
(struct perf_guest_info_callbacks *callbacks)				{ return 0; }
static inline int perf_unregister_guest_info_callbacks
(struct perf_guest_info_callbacks *callbacks)				{ return 0; }

static inline void perf_event_mmap(struct vm_area_struct *vma)		{ }
static inline void perf_event_comm(struct task_struct *tsk)		{ }
static inline void perf_event_fork(struct task_struct *tsk)		{ }
static inline void perf_event_init(void)				{ }
static inline int  perf_swevent_get_recursion_context(void)		{ return -1; }
static inline void perf_swevent_put_recursion_context(int rctx)		{ }
static inline void perf_event_enable(struct perf_event *event)		{ }
static inline void perf_event_disable(struct perf_event *event)		{ }
static inline void perf_event_task_tick(void)				{ }
=======
static inline void perf_event_mmap(struct vm_area_struct *vma)		{ }

typedef int (perf_ksymbol_get_name_f)(char *name, int name_len, void *data);
static inline void perf_event_ksymbol(u16 ksym_type, u64 addr, u32 len,
				      bool unregister, const char *sym)	{ }
static inline void perf_event_bpf_event(struct bpf_prog *prog,
					enum perf_bpf_event_type type,
					u16 flags)			{ }
static inline void perf_event_exec(void)				{ }
static inline void perf_event_comm(struct task_struct *tsk, bool exec)	{ }
static inline void perf_event_namespaces(struct task_struct *tsk)	{ }
static inline void perf_event_fork(struct task_struct *tsk)		{ }
static inline void perf_event_text_poke(const void *addr,
					const void *old_bytes,
					size_t old_len,
					const void *new_bytes,
					size_t new_len)			{ }
static inline void perf_event_init(void)				{ }
static inline int  perf_swevent_get_recursion_context(void)		{ return -1; }
static inline void perf_swevent_put_recursion_context(int rctx)		{ }
static inline u64 perf_swevent_set_period(struct perf_event *event)	{ return 0; }
static inline void perf_event_enable(struct perf_event *event)		{ }
static inline void perf_event_disable(struct perf_event *event)		{ }
static inline int __perf_event_disable(void *info)			{ return -1; }
static inline void perf_event_task_tick(void)				{ }
static inline int perf_event_release_kernel(struct perf_event *event)	{ return 0; }
static inline int perf_event_period(struct perf_event *event, u64 value)
{
	return -EINVAL;
}
static inline u64 perf_event_pause(struct perf_event *event, bool reset)
{
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#if defined(CONFIG_PERF_EVENTS) && defined(CONFIG_CPU_SUP_INTEL)
extern void perf_restore_debug_store(void);
#else
static inline void perf_restore_debug_store(void)			{ }
#endif

#define perf_output_put(handle, x) perf_output_copy((handle), &(x), sizeof(x))

<<<<<<< HEAD
/*
 * This has to have a higher priority than migration_notifier in sched.c.
 */
#define perf_cpu_notifier(fn)						\
do {									\
	static struct notifier_block fn##_nb __cpuinitdata =		\
		{ .notifier_call = fn, .priority = CPU_PRI_PERF };	\
	fn(&fn##_nb, (unsigned long)CPU_UP_PREPARE,			\
		(void *)(unsigned long)smp_processor_id());		\
	fn(&fn##_nb, (unsigned long)CPU_STARTING,			\
		(void *)(unsigned long)smp_processor_id());		\
	fn(&fn##_nb, (unsigned long)CPU_ONLINE,				\
		(void *)(unsigned long)smp_processor_id());		\
	register_cpu_notifier(&fn##_nb);				\
} while (0)


#define PMU_FORMAT_ATTR(_name, _format)					\
=======
struct perf_pmu_events_attr {
	struct device_attribute attr;
	u64 id;
	const char *event_str;
};

struct perf_pmu_events_ht_attr {
	struct device_attribute			attr;
	u64					id;
	const char				*event_str_ht;
	const char				*event_str_noht;
};

struct perf_pmu_events_hybrid_attr {
	struct device_attribute			attr;
	u64					id;
	const char				*event_str;
	u64					pmu_type;
};

struct perf_pmu_format_hybrid_attr {
	struct device_attribute			attr;
	u64					pmu_type;
};

ssize_t perf_event_sysfs_show(struct device *dev, struct device_attribute *attr,
			      char *page);

#define PMU_EVENT_ATTR(_name, _var, _id, _show)				\
static struct perf_pmu_events_attr _var = {				\
	.attr = __ATTR(_name, 0444, _show, NULL),			\
	.id   =  _id,							\
};

#define PMU_EVENT_ATTR_STRING(_name, _var, _str)			    \
static struct perf_pmu_events_attr _var = {				    \
	.attr		= __ATTR(_name, 0444, perf_event_sysfs_show, NULL), \
	.id		= 0,						    \
	.event_str	= _str,						    \
};

#define PMU_EVENT_ATTR_ID(_name, _show, _id)				\
	(&((struct perf_pmu_events_attr[]) {				\
		{ .attr = __ATTR(_name, 0444, _show, NULL),		\
		  .id = _id, }						\
	})[0].attr.attr)

#define PMU_FORMAT_ATTR_SHOW(_name, _format)				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t								\
_name##_show(struct device *dev,					\
			       struct device_attribute *attr,		\
			       char *page)				\
{									\
	BUILD_BUG_ON(sizeof(_format) >= PAGE_SIZE);			\
	return sprintf(page, _format "\n");				\
}									\
<<<<<<< HEAD
									\
static struct device_attribute format_attr_##_name = __ATTR_RO(_name)

#endif /* __KERNEL__ */
=======

#define PMU_FORMAT_ATTR(_name, _format)					\
	PMU_FORMAT_ATTR_SHOW(_name, _format)				\
									\
static struct device_attribute format_attr_##_name = __ATTR_RO(_name)

/* Performance counter hotplug functions */
#ifdef CONFIG_PERF_EVENTS
int perf_event_init_cpu(unsigned int cpu);
int perf_event_exit_cpu(unsigned int cpu);
#else
#define perf_event_init_cpu	NULL
#define perf_event_exit_cpu	NULL
#endif

extern void arch_perf_update_userpage(struct perf_event *event,
				      struct perf_event_mmap_page *userpg,
				      u64 now);

/*
 * Snapshot branch stack on software events.
 *
 * Branch stack can be very useful in understanding software events. For
 * example, when a long function, e.g. sys_perf_event_open, returns an
 * errno, it is not obvious why the function failed. Branch stack could
 * provide very helpful information in this type of scenarios.
 *
 * On software event, it is necessary to stop the hardware branch recorder
 * fast. Otherwise, the hardware register/buffer will be flushed with
 * entries of the triggering event. Therefore, static call is used to
 * stop the hardware recorder.
 */

/*
 * cnt is the number of entries allocated for entries.
 * Return number of entries copied to .
 */
typedef int (perf_snapshot_branch_stack_t)(struct perf_branch_entry *entries,
					   unsigned int cnt);
DECLARE_STATIC_CALL(perf_snapshot_branch_stack, perf_snapshot_branch_stack_t);

#ifndef PERF_NEEDS_LOPWR_CB
static inline void perf_lopwr_cb(bool mode)
{
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_PERF_EVENT_H */
