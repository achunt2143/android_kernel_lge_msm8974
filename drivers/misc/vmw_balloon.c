<<<<<<< HEAD
/*
 * VMware Balloon driver.
 *
 * Copyright (C) 2000-2010, VMware, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Maintained by: Dmitry Torokhov <dtor@vmware.com>
 */

/*
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * VMware Balloon driver.
 *
 * Copyright (C) 2000-2018, VMware, Inc. All Rights Reserved.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This is VMware physical memory management driver for Linux. The driver
 * acts like a "balloon" that can be inflated to reclaim physical pages by
 * reserving them in the guest and invalidating them in the monitor,
 * freeing up the underlying machine pages so they can be allocated to
 * other guests.  The balloon can also be deflated to allow the guest to
 * use more physical memory. Higher level policies can control the sizes
 * of balloons in VMs in order to manage physical memory resources.
 */

//#define DEBUG
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/types.h>
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/mm.h>
=======
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
=======
#include <linux/rwsem.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/balloon_compaction.h>
#include <linux/vmw_vmci_defs.h>
#include <linux/vmw_vmci_api.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/hypervisor.h>

MODULE_AUTHOR("VMware, Inc.");
MODULE_DESCRIPTION("VMware Memory Control (Balloon) Driver");
<<<<<<< HEAD
MODULE_VERSION("1.2.1.3-k");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("dmi:*:svnVMware*:*");
MODULE_ALIAS("vmware_vmmemctl");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
/*
 * Various constants controlling rate of inflaint/deflating balloon,
 * measured in pages.
 */

/*
 * Rate of allocating memory when there is no memory pressure
 * (driver performs non-sleeping allocations).
 */
#define VMW_BALLOON_NOSLEEP_ALLOC_MAX	16384U

/*
 * Rates of memory allocaton when guest experiences memory pressure
 * (driver performs sleeping allocations).
 */
#define VMW_BALLOON_RATE_ALLOC_MIN	512U
#define VMW_BALLOON_RATE_ALLOC_MAX	2048U
#define VMW_BALLOON_RATE_ALLOC_INC	16U

/*
 * Rates for releasing pages while deflating balloon.
 */
#define VMW_BALLOON_RATE_FREE_MIN	512U
#define VMW_BALLOON_RATE_FREE_MAX	16384U
#define VMW_BALLOON_RATE_FREE_INC	16U

/*
 * When guest is under memory pressure, use a reduced page allocation
 * rate for next several cycles.
 */
#define VMW_BALLOON_SLOW_CYCLES		4

/*
 * Use __GFP_HIGHMEM to allow pages from HIGHMEM zone. We don't
 * allow wait (__GFP_WAIT) for NOSLEEP page allocations. Use
 * __GFP_NOWARN, to suppress page allocation failure warnings.
 */
#define VMW_PAGE_ALLOC_NOSLEEP		(__GFP_HIGHMEM|__GFP_NOWARN)

/*
 * Use GFP_HIGHUSER when executing in a separate kernel thread
 * context and allocation can sleep.  This is less stressful to
 * the guest memory system, since it allows the thread to block
 * while memory is reclaimed, and won't take pages from emergency
 * low-memory pools.
 */
#define VMW_PAGE_ALLOC_CANSLEEP		(GFP_HIGHUSER)

/* Maximum number of page allocations without yielding processor */
#define VMW_BALLOON_YIELD_THRESHOLD	1024
=======
static bool __read_mostly vmwballoon_shrinker_enable;
module_param(vmwballoon_shrinker_enable, bool, 0444);
MODULE_PARM_DESC(vmwballoon_shrinker_enable,
	"Enable non-cooperative out-of-memory protection. Disabled by default as it may degrade performance.");

/* Delay in seconds after shrink before inflation. */
#define VMBALLOON_SHRINK_DELAY		(5)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Maximum number of refused pages we accumulate during inflation cycle */
#define VMW_BALLOON_MAX_REFUSED		16

<<<<<<< HEAD
=======
/* Magic number for the balloon mount-point */
#define BALLOON_VMW_MAGIC		0x0ba11007

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Hypervisor communication port definitions.
 */
#define VMW_BALLOON_HV_PORT		0x5670
#define VMW_BALLOON_HV_MAGIC		0x456c6d6f
<<<<<<< HEAD
#define VMW_BALLOON_PROTOCOL_VERSION	2
#define VMW_BALLOON_GUEST_ID		1	/* Linux */

#define VMW_BALLOON_CMD_START		0
#define VMW_BALLOON_CMD_GET_TARGET	1
#define VMW_BALLOON_CMD_LOCK		2
#define VMW_BALLOON_CMD_UNLOCK		3
#define VMW_BALLOON_CMD_GUEST_ID	4

/* error codes */
#define VMW_BALLOON_SUCCESS		0
#define VMW_BALLOON_FAILURE		-1
#define VMW_BALLOON_ERROR_CMD_INVALID	1
#define VMW_BALLOON_ERROR_PPN_INVALID	2
#define VMW_BALLOON_ERROR_PPN_LOCKED	3
#define VMW_BALLOON_ERROR_PPN_UNLOCKED	4
#define VMW_BALLOON_ERROR_PPN_PINNED	5
#define VMW_BALLOON_ERROR_PPN_NOTNEEDED	6
#define VMW_BALLOON_ERROR_RESET		7
#define VMW_BALLOON_ERROR_BUSY		8

#define VMWARE_BALLOON_CMD(cmd, data, result)		\
({							\
	unsigned long __stat, __dummy1, __dummy2;	\
	__asm__ __volatile__ ("inl (%%dx)" :		\
		"=a"(__stat),				\
		"=c"(__dummy1),				\
		"=d"(__dummy2),				\
		"=b"(result) :				\
		"0"(VMW_BALLOON_HV_MAGIC),		\
		"1"(VMW_BALLOON_CMD_##cmd),		\
		"2"(VMW_BALLOON_HV_PORT),		\
		"3"(data) :				\
		"memory");				\
	result &= -1UL;					\
	__stat & -1UL;					\
})

#ifdef CONFIG_DEBUG_FS
struct vmballoon_stats {
	unsigned int timer;

	/* allocation statistics */
	unsigned int alloc;
	unsigned int alloc_fail;
	unsigned int sleep_alloc;
	unsigned int sleep_alloc_fail;
	unsigned int refused_alloc;
	unsigned int refused_free;
	unsigned int free;

	/* monitor operations */
	unsigned int lock;
	unsigned int lock_fail;
	unsigned int unlock;
	unsigned int unlock_fail;
	unsigned int target;
	unsigned int target_fail;
	unsigned int start;
	unsigned int start_fail;
	unsigned int guest_type;
	unsigned int guest_type_fail;
};

#define STATS_INC(stat) (stat)++
#else
#define STATS_INC(stat)
#endif

struct vmballoon {

	/* list of reserved physical pages */
	struct list_head pages;

	/* transient list of non-balloonable pages */
	struct list_head refused_pages;
	unsigned int n_refused_pages;

	/* balloon size in pages */
	unsigned int size;
	unsigned int target;

	/* reset flag */
	bool reset_required;

	/* adjustment rates (pages per second) */
	unsigned int rate_alloc;
	unsigned int rate_free;

	/* slowdown page allocations for next few cycles */
	unsigned int slow_allocation_cycles;

#ifdef CONFIG_DEBUG_FS
	/* statistics */
	struct vmballoon_stats stats;

	/* debugfs file exporting statistics */
	struct dentry *dbg_entry;
#endif

	struct sysinfo sysinfo;

	struct delayed_work dwork;
=======
#define VMW_BALLOON_GUEST_ID		1	/* Linux */

enum vmwballoon_capabilities {
	/*
	 * Bit 0 is reserved and not associated to any capability.
	 */
	VMW_BALLOON_BASIC_CMDS			= (1 << 1),
	VMW_BALLOON_BATCHED_CMDS		= (1 << 2),
	VMW_BALLOON_BATCHED_2M_CMDS		= (1 << 3),
	VMW_BALLOON_SIGNALLED_WAKEUP_CMD	= (1 << 4),
	VMW_BALLOON_64_BIT_TARGET		= (1 << 5)
};

#define VMW_BALLOON_CAPABILITIES_COMMON	(VMW_BALLOON_BASIC_CMDS \
					| VMW_BALLOON_BATCHED_CMDS \
					| VMW_BALLOON_BATCHED_2M_CMDS \
					| VMW_BALLOON_SIGNALLED_WAKEUP_CMD)

#define VMW_BALLOON_2M_ORDER		(PMD_SHIFT - PAGE_SHIFT)

/*
 * 64-bit targets are only supported in 64-bit
 */
#ifdef CONFIG_64BIT
#define VMW_BALLOON_CAPABILITIES	(VMW_BALLOON_CAPABILITIES_COMMON \
					| VMW_BALLOON_64_BIT_TARGET)
#else
#define VMW_BALLOON_CAPABILITIES	VMW_BALLOON_CAPABILITIES_COMMON
#endif

enum vmballoon_page_size_type {
	VMW_BALLOON_4K_PAGE,
	VMW_BALLOON_2M_PAGE,
	VMW_BALLOON_LAST_SIZE = VMW_BALLOON_2M_PAGE
};

#define VMW_BALLOON_NUM_PAGE_SIZES	(VMW_BALLOON_LAST_SIZE + 1)

static const char * const vmballoon_page_size_names[] = {
	[VMW_BALLOON_4K_PAGE]			= "4k",
	[VMW_BALLOON_2M_PAGE]			= "2M"
};

enum vmballoon_op {
	VMW_BALLOON_INFLATE,
	VMW_BALLOON_DEFLATE
};

enum vmballoon_op_stat_type {
	VMW_BALLOON_OP_STAT,
	VMW_BALLOON_OP_FAIL_STAT
};

#define VMW_BALLOON_OP_STAT_TYPES	(VMW_BALLOON_OP_FAIL_STAT + 1)

/**
 * enum vmballoon_cmd_type - backdoor commands.
 *
 * Availability of the commands is as followed:
 *
 * %VMW_BALLOON_CMD_START, %VMW_BALLOON_CMD_GET_TARGET and
 * %VMW_BALLOON_CMD_GUEST_ID are always available.
 *
 * If the host reports %VMW_BALLOON_BASIC_CMDS are supported then
 * %VMW_BALLOON_CMD_LOCK and %VMW_BALLOON_CMD_UNLOCK commands are available.
 *
 * If the host reports %VMW_BALLOON_BATCHED_CMDS are supported then
 * %VMW_BALLOON_CMD_BATCHED_LOCK and VMW_BALLOON_CMD_BATCHED_UNLOCK commands
 * are available.
 *
 * If the host reports %VMW_BALLOON_BATCHED_2M_CMDS are supported then
 * %VMW_BALLOON_CMD_BATCHED_2M_LOCK and %VMW_BALLOON_CMD_BATCHED_2M_UNLOCK
 * are supported.
 *
 * If the host reports  VMW_BALLOON_SIGNALLED_WAKEUP_CMD is supported then
 * VMW_BALLOON_CMD_VMCI_DOORBELL_SET command is supported.
 *
 * @VMW_BALLOON_CMD_START: Communicating supported version with the hypervisor.
 * @VMW_BALLOON_CMD_GET_TARGET: Gets the balloon target size.
 * @VMW_BALLOON_CMD_LOCK: Informs the hypervisor about a ballooned page.
 * @VMW_BALLOON_CMD_UNLOCK: Informs the hypervisor about a page that is about
 *			    to be deflated from the balloon.
 * @VMW_BALLOON_CMD_GUEST_ID: Informs the hypervisor about the type of OS that
 *			      runs in the VM.
 * @VMW_BALLOON_CMD_BATCHED_LOCK: Inform the hypervisor about a batch of
 *				  ballooned pages (up to 512).
 * @VMW_BALLOON_CMD_BATCHED_UNLOCK: Inform the hypervisor about a batch of
 *				  pages that are about to be deflated from the
 *				  balloon (up to 512).
 * @VMW_BALLOON_CMD_BATCHED_2M_LOCK: Similar to @VMW_BALLOON_CMD_BATCHED_LOCK
 *				     for 2MB pages.
 * @VMW_BALLOON_CMD_BATCHED_2M_UNLOCK: Similar to
 *				       @VMW_BALLOON_CMD_BATCHED_UNLOCK for 2MB
 *				       pages.
 * @VMW_BALLOON_CMD_VMCI_DOORBELL_SET: A command to set doorbell notification
 *				       that would be invoked when the balloon
 *				       size changes.
 * @VMW_BALLOON_CMD_LAST: Value of the last command.
 */
enum vmballoon_cmd_type {
	VMW_BALLOON_CMD_START,
	VMW_BALLOON_CMD_GET_TARGET,
	VMW_BALLOON_CMD_LOCK,
	VMW_BALLOON_CMD_UNLOCK,
	VMW_BALLOON_CMD_GUEST_ID,
	/* No command 5 */
	VMW_BALLOON_CMD_BATCHED_LOCK = 6,
	VMW_BALLOON_CMD_BATCHED_UNLOCK,
	VMW_BALLOON_CMD_BATCHED_2M_LOCK,
	VMW_BALLOON_CMD_BATCHED_2M_UNLOCK,
	VMW_BALLOON_CMD_VMCI_DOORBELL_SET,
	VMW_BALLOON_CMD_LAST = VMW_BALLOON_CMD_VMCI_DOORBELL_SET,
};

#define VMW_BALLOON_CMD_NUM	(VMW_BALLOON_CMD_LAST + 1)

enum vmballoon_error_codes {
	VMW_BALLOON_SUCCESS,
	VMW_BALLOON_ERROR_CMD_INVALID,
	VMW_BALLOON_ERROR_PPN_INVALID,
	VMW_BALLOON_ERROR_PPN_LOCKED,
	VMW_BALLOON_ERROR_PPN_UNLOCKED,
	VMW_BALLOON_ERROR_PPN_PINNED,
	VMW_BALLOON_ERROR_PPN_NOTNEEDED,
	VMW_BALLOON_ERROR_RESET,
	VMW_BALLOON_ERROR_BUSY
};

#define VMW_BALLOON_SUCCESS_WITH_CAPABILITIES	(0x03000000)

#define VMW_BALLOON_CMD_WITH_TARGET_MASK			\
	((1UL << VMW_BALLOON_CMD_GET_TARGET)		|	\
	 (1UL << VMW_BALLOON_CMD_LOCK)			|	\
	 (1UL << VMW_BALLOON_CMD_UNLOCK)		|	\
	 (1UL << VMW_BALLOON_CMD_BATCHED_LOCK)		|	\
	 (1UL << VMW_BALLOON_CMD_BATCHED_UNLOCK)	|	\
	 (1UL << VMW_BALLOON_CMD_BATCHED_2M_LOCK)	|	\
	 (1UL << VMW_BALLOON_CMD_BATCHED_2M_UNLOCK))

static const char * const vmballoon_cmd_names[] = {
	[VMW_BALLOON_CMD_START]			= "start",
	[VMW_BALLOON_CMD_GET_TARGET]		= "target",
	[VMW_BALLOON_CMD_LOCK]			= "lock",
	[VMW_BALLOON_CMD_UNLOCK]		= "unlock",
	[VMW_BALLOON_CMD_GUEST_ID]		= "guestType",
	[VMW_BALLOON_CMD_BATCHED_LOCK]		= "batchLock",
	[VMW_BALLOON_CMD_BATCHED_UNLOCK]	= "batchUnlock",
	[VMW_BALLOON_CMD_BATCHED_2M_LOCK]	= "2m-lock",
	[VMW_BALLOON_CMD_BATCHED_2M_UNLOCK]	= "2m-unlock",
	[VMW_BALLOON_CMD_VMCI_DOORBELL_SET]	= "doorbellSet"
};

enum vmballoon_stat_page {
	VMW_BALLOON_PAGE_STAT_ALLOC,
	VMW_BALLOON_PAGE_STAT_ALLOC_FAIL,
	VMW_BALLOON_PAGE_STAT_REFUSED_ALLOC,
	VMW_BALLOON_PAGE_STAT_REFUSED_FREE,
	VMW_BALLOON_PAGE_STAT_FREE,
	VMW_BALLOON_PAGE_STAT_LAST = VMW_BALLOON_PAGE_STAT_FREE
};

#define VMW_BALLOON_PAGE_STAT_NUM	(VMW_BALLOON_PAGE_STAT_LAST + 1)

enum vmballoon_stat_general {
	VMW_BALLOON_STAT_TIMER,
	VMW_BALLOON_STAT_DOORBELL,
	VMW_BALLOON_STAT_RESET,
	VMW_BALLOON_STAT_SHRINK,
	VMW_BALLOON_STAT_SHRINK_FREE,
	VMW_BALLOON_STAT_LAST = VMW_BALLOON_STAT_SHRINK_FREE
};

#define VMW_BALLOON_STAT_NUM		(VMW_BALLOON_STAT_LAST + 1)

static DEFINE_STATIC_KEY_TRUE(vmw_balloon_batching);
static DEFINE_STATIC_KEY_FALSE(balloon_stat_enabled);

struct vmballoon_ctl {
	struct list_head pages;
	struct list_head refused_pages;
	struct list_head prealloc_pages;
	unsigned int n_refused_pages;
	unsigned int n_pages;
	enum vmballoon_page_size_type page_size;
	enum vmballoon_op op;
};

/**
 * struct vmballoon_batch_entry - a batch entry for lock or unlock.
 *
 * @status: the status of the operation, which is written by the hypervisor.
 * @reserved: reserved for future use. Must be set to zero.
 * @pfn: the physical frame number of the page to be locked or unlocked.
 */
struct vmballoon_batch_entry {
	u64 status : 5;
	u64 reserved : PAGE_SHIFT - 5;
	u64 pfn : 52;
} __packed;

struct vmballoon {
	/**
	 * @max_page_size: maximum supported page size for ballooning.
	 *
	 * Protected by @conf_sem
	 */
	enum vmballoon_page_size_type max_page_size;

	/**
	 * @size: balloon actual size in basic page size (frames).
	 *
	 * While we currently do not support size which is bigger than 32-bit,
	 * in preparation for future support, use 64-bits.
	 */
	atomic64_t size;

	/**
	 * @target: balloon target size in basic page size (frames).
	 *
	 * We do not protect the target under the assumption that setting the
	 * value is always done through a single write. If this assumption ever
	 * breaks, we would have to use X_ONCE for accesses, and suffer the less
	 * optimized code. Although we may read stale target value if multiple
	 * accesses happen at once, the performance impact should be minor.
	 */
	unsigned long target;

	/**
	 * @reset_required: reset flag
	 *
	 * Setting this flag may introduce races, but the code is expected to
	 * handle them gracefully. In the worst case, another operation will
	 * fail as reset did not take place. Clearing the flag is done while
	 * holding @conf_sem for write.
	 */
	bool reset_required;

	/**
	 * @capabilities: hypervisor balloon capabilities.
	 *
	 * Protected by @conf_sem.
	 */
	unsigned long capabilities;

	/**
	 * @batch_page: pointer to communication batch page.
	 *
	 * When batching is used, batch_page points to a page, which holds up to
	 * %VMW_BALLOON_BATCH_MAX_PAGES entries for locking or unlocking.
	 */
	struct vmballoon_batch_entry *batch_page;

	/**
	 * @batch_max_pages: maximum pages that can be locked/unlocked.
	 *
	 * Indicates the number of pages that the hypervisor can lock or unlock
	 * at once, according to whether batching is enabled. If batching is
	 * disabled, only a single page can be locked/unlock on each operation.
	 *
	 * Protected by @conf_sem.
	 */
	unsigned int batch_max_pages;

	/**
	 * @page: page to be locked/unlocked by the hypervisor
	 *
	 * @page is only used when batching is disabled and a single page is
	 * reclaimed on each iteration.
	 *
	 * Protected by @comm_lock.
	 */
	struct page *page;

	/**
	 * @shrink_timeout: timeout until the next inflation.
	 *
	 * After an shrink event, indicates the time in jiffies after which
	 * inflation is allowed again. Can be written concurrently with reads,
	 * so must use READ_ONCE/WRITE_ONCE when accessing.
	 */
	unsigned long shrink_timeout;

	/* statistics */
	struct vmballoon_stats *stats;

	/**
	 * @b_dev_info: balloon device information descriptor.
	 */
	struct balloon_dev_info b_dev_info;

	struct delayed_work dwork;

	/**
	 * @huge_pages - list of the inflated 2MB pages.
	 *
	 * Protected by @b_dev_info.pages_lock .
	 */
	struct list_head huge_pages;

	/**
	 * @vmci_doorbell.
	 *
	 * Protected by @conf_sem.
	 */
	struct vmci_handle vmci_doorbell;

	/**
	 * @conf_sem: semaphore to protect the configuration and the statistics.
	 */
	struct rw_semaphore conf_sem;

	/**
	 * @comm_lock: lock to protect the communication with the host.
	 *
	 * Lock ordering: @conf_sem -> @comm_lock .
	 */
	spinlock_t comm_lock;

	/**
	 * @shrinker: shrinker interface that is used to avoid over-inflation.
	 */
	struct shrinker *shrinker;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct vmballoon balloon;

<<<<<<< HEAD
=======
struct vmballoon_stats {
	/* timer / doorbell operations */
	atomic64_t general_stat[VMW_BALLOON_STAT_NUM];

	/* allocation statistics for huge and small pages */
	atomic64_t
	       page_stat[VMW_BALLOON_PAGE_STAT_NUM][VMW_BALLOON_NUM_PAGE_SIZES];

	/* Monitor operations: total operations, and failures */
	atomic64_t ops[VMW_BALLOON_CMD_NUM][VMW_BALLOON_OP_STAT_TYPES];
};

static inline bool is_vmballoon_stats_on(void)
{
	return IS_ENABLED(CONFIG_DEBUG_FS) &&
		static_branch_unlikely(&balloon_stat_enabled);
}

static inline void vmballoon_stats_op_inc(struct vmballoon *b, unsigned int op,
					  enum vmballoon_op_stat_type type)
{
	if (is_vmballoon_stats_on())
		atomic64_inc(&b->stats->ops[op][type]);
}

static inline void vmballoon_stats_gen_inc(struct vmballoon *b,
					   enum vmballoon_stat_general stat)
{
	if (is_vmballoon_stats_on())
		atomic64_inc(&b->stats->general_stat[stat]);
}

static inline void vmballoon_stats_gen_add(struct vmballoon *b,
					   enum vmballoon_stat_general stat,
					   unsigned int val)
{
	if (is_vmballoon_stats_on())
		atomic64_add(val, &b->stats->general_stat[stat]);
}

static inline void vmballoon_stats_page_inc(struct vmballoon *b,
					    enum vmballoon_stat_page stat,
					    enum vmballoon_page_size_type size)
{
	if (is_vmballoon_stats_on())
		atomic64_inc(&b->stats->page_stat[stat][size]);
}

static inline void vmballoon_stats_page_add(struct vmballoon *b,
					    enum vmballoon_stat_page stat,
					    enum vmballoon_page_size_type size,
					    unsigned int val)
{
	if (is_vmballoon_stats_on())
		atomic64_add(val, &b->stats->page_stat[stat][size]);
}

static inline unsigned long
__vmballoon_cmd(struct vmballoon *b, unsigned long cmd, unsigned long arg1,
		unsigned long arg2, unsigned long *result)
{
	unsigned long status, dummy1, dummy2, dummy3, local_result;

	vmballoon_stats_op_inc(b, cmd, VMW_BALLOON_OP_STAT);

	asm volatile ("inl %%dx" :
		"=a"(status),
		"=c"(dummy1),
		"=d"(dummy2),
		"=b"(local_result),
		"=S"(dummy3) :
		"0"(VMW_BALLOON_HV_MAGIC),
		"1"(cmd),
		"2"(VMW_BALLOON_HV_PORT),
		"3"(arg1),
		"4"(arg2) :
		"memory");

	/* update the result if needed */
	if (result)
		*result = (cmd == VMW_BALLOON_CMD_START) ? dummy1 :
							   local_result;

	/* update target when applicable */
	if (status == VMW_BALLOON_SUCCESS &&
	    ((1ul << cmd) & VMW_BALLOON_CMD_WITH_TARGET_MASK))
		WRITE_ONCE(b->target, local_result);

	if (status != VMW_BALLOON_SUCCESS &&
	    status != VMW_BALLOON_SUCCESS_WITH_CAPABILITIES) {
		vmballoon_stats_op_inc(b, cmd, VMW_BALLOON_OP_FAIL_STAT);
		pr_debug("%s: %s [0x%lx,0x%lx) failed, returned %ld\n",
			 __func__, vmballoon_cmd_names[cmd], arg1, arg2,
			 status);
	}

	/* mark reset required accordingly */
	if (status == VMW_BALLOON_ERROR_RESET)
		b->reset_required = true;

	return status;
}

static __always_inline unsigned long
vmballoon_cmd(struct vmballoon *b, unsigned long cmd, unsigned long arg1,
	      unsigned long arg2)
{
	unsigned long dummy;

	return __vmballoon_cmd(b, cmd, arg1, arg2, &dummy);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Send "start" command to the host, communicating supported version
 * of the protocol.
 */
<<<<<<< HEAD
static bool vmballoon_send_start(struct vmballoon *b)
{
	unsigned long status, dummy;

	STATS_INC(b->stats.start);

	status = VMWARE_BALLOON_CMD(START, VMW_BALLOON_PROTOCOL_VERSION, dummy);
	if (status == VMW_BALLOON_SUCCESS)
		return true;

	pr_debug("%s - failed, hv returns %ld\n", __func__, status);
	STATS_INC(b->stats.start_fail);
	return false;
}

static bool vmballoon_check_status(struct vmballoon *b, unsigned long status)
{
	switch (status) {
	case VMW_BALLOON_SUCCESS:
		return true;

	case VMW_BALLOON_ERROR_RESET:
		b->reset_required = true;
		/* fall through */

	default:
		return false;
	}
}

/*
=======
static int vmballoon_send_start(struct vmballoon *b, unsigned long req_caps)
{
	unsigned long status, capabilities;

	status = __vmballoon_cmd(b, VMW_BALLOON_CMD_START, req_caps, 0,
				 &capabilities);

	switch (status) {
	case VMW_BALLOON_SUCCESS_WITH_CAPABILITIES:
		b->capabilities = capabilities;
		break;
	case VMW_BALLOON_SUCCESS:
		b->capabilities = VMW_BALLOON_BASIC_CMDS;
		break;
	default:
		return -EIO;
	}

	/*
	 * 2MB pages are only supported with batching. If batching is for some
	 * reason disabled, do not use 2MB pages, since otherwise the legacy
	 * mechanism is used with 2MB pages, causing a failure.
	 */
	b->max_page_size = VMW_BALLOON_4K_PAGE;
	if ((b->capabilities & VMW_BALLOON_BATCHED_2M_CMDS) &&
	    (b->capabilities & VMW_BALLOON_BATCHED_CMDS))
		b->max_page_size = VMW_BALLOON_2M_PAGE;


	return 0;
}

/**
 * vmballoon_send_guest_id - communicate guest type to the host.
 *
 * @b: pointer to the balloon.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Communicate guest type to the host so that it can adjust ballooning
 * algorithm to the one most appropriate for the guest. This command
 * is normally issued after sending "start" command and is part of
 * standard reset sequence.
<<<<<<< HEAD
 */
static bool vmballoon_send_guest_id(struct vmballoon *b)
{
	unsigned long status, dummy;

	status = VMWARE_BALLOON_CMD(GUEST_ID, VMW_BALLOON_GUEST_ID, dummy);

	STATS_INC(b->stats.guest_type);

	if (vmballoon_check_status(b, status))
		return true;

	pr_debug("%s - failed, hv returns %ld\n", __func__, status);
	STATS_INC(b->stats.guest_type_fail);
	return false;
}

/*
 * Retrieve desired balloon size from the host.
 */
static bool vmballoon_send_get_target(struct vmballoon *b, u32 *new_target)
{
	unsigned long status;
	unsigned long target;
	unsigned long limit;
	u32 limit32;

	/*
	 * si_meminfo() is cheap. Moreover, we want to provide dynamic
	 * max balloon size later. So let us call si_meminfo() every
	 * iteration.
	 */
	si_meminfo(&b->sysinfo);
	limit = b->sysinfo.totalram;

	/* Ensure limit fits in 32-bits */
	limit32 = (u32)limit;
	if (limit != limit32)
		return false;

	/* update stats */
	STATS_INC(b->stats.target);

	status = VMWARE_BALLOON_CMD(GET_TARGET, limit, target);
	if (vmballoon_check_status(b, status)) {
		*new_target = target;
		return true;
	}

	pr_debug("%s - failed, hv returns %ld\n", __func__, status);
	STATS_INC(b->stats.target_fail);
	return false;
}

/*
 * Notify the host about allocated page so that host can use it without
 * fear that guest will need it. Host may reject some pages, we need to
 * check the return value and maybe submit a different page.
 */
static int vmballoon_send_lock_page(struct vmballoon *b, unsigned long pfn,
				     unsigned int *hv_status)
{
	unsigned long status, dummy;
	u32 pfn32;

	pfn32 = (u32)pfn;
	if (pfn32 != pfn)
		return -1;

	STATS_INC(b->stats.lock);

	*hv_status = status = VMWARE_BALLOON_CMD(LOCK, pfn, dummy);
	if (vmballoon_check_status(b, status))
		return 0;

	pr_debug("%s - ppn %lx, hv returns %ld\n", __func__, pfn, status);
	STATS_INC(b->stats.lock_fail);
	return 1;
}

/*
 * Notify the host that guest intends to release given page back into
 * the pool of available (to the guest) pages.
 */
static bool vmballoon_send_unlock_page(struct vmballoon *b, unsigned long pfn)
{
	unsigned long status, dummy;
	u32 pfn32;

	pfn32 = (u32)pfn;
	if (pfn32 != pfn)
		return false;

	STATS_INC(b->stats.unlock);

	status = VMWARE_BALLOON_CMD(UNLOCK, pfn, dummy);
	if (vmballoon_check_status(b, status))
		return true;

	pr_debug("%s - ppn %lx, hv returns %ld\n", __func__, pfn, status);
	STATS_INC(b->stats.unlock_fail);
	return false;
}

/*
 * Quickly release all pages allocated for the balloon. This function is
 * called when host decides to "reset" balloon for one reason or another.
 * Unlike normal "deflate" we do not (shall not) notify host of the pages
 * being released.
 */
static void vmballoon_pop(struct vmballoon *b)
{
	struct page *page, *next;
	unsigned int count = 0;

	list_for_each_entry_safe(page, next, &b->pages, lru) {
		list_del(&page->lru);
		__free_page(page);
		STATS_INC(b->stats.free);
		b->size--;

		if (++count >= b->rate_free) {
			count = 0;
			cond_resched();
		}
	}
=======
 *
 * Return: zero on success or appropriate error code.
 */
static int vmballoon_send_guest_id(struct vmballoon *b)
{
	unsigned long status;

	status = vmballoon_cmd(b, VMW_BALLOON_CMD_GUEST_ID,
			       VMW_BALLOON_GUEST_ID, 0);

	return status == VMW_BALLOON_SUCCESS ? 0 : -EIO;
}

/**
 * vmballoon_page_order() - return the order of the page
 * @page_size: the size of the page.
 *
 * Return: the allocation order.
 */
static inline
unsigned int vmballoon_page_order(enum vmballoon_page_size_type page_size)
{
	return page_size == VMW_BALLOON_2M_PAGE ? VMW_BALLOON_2M_ORDER : 0;
}

/**
 * vmballoon_page_in_frames() - returns the number of frames in a page.
 * @page_size: the size of the page.
 *
 * Return: the number of 4k frames.
 */
static inline unsigned int
vmballoon_page_in_frames(enum vmballoon_page_size_type page_size)
{
	return 1 << vmballoon_page_order(page_size);
}

/**
 * vmballoon_mark_page_offline() - mark a page as offline
 * @page: pointer for the page.
 * @page_size: the size of the page.
 */
static void
vmballoon_mark_page_offline(struct page *page,
			    enum vmballoon_page_size_type page_size)
{
	int i;

	for (i = 0; i < vmballoon_page_in_frames(page_size); i++)
		__SetPageOffline(page + i);
}

/**
 * vmballoon_mark_page_online() - mark a page as online
 * @page: pointer for the page.
 * @page_size: the size of the page.
 */
static void
vmballoon_mark_page_online(struct page *page,
			   enum vmballoon_page_size_type page_size)
{
	int i;

	for (i = 0; i < vmballoon_page_in_frames(page_size); i++)
		__ClearPageOffline(page + i);
}

/**
 * vmballoon_send_get_target() - Retrieve desired balloon size from the host.
 *
 * @b: pointer to the balloon.
 *
 * Return: zero on success, EINVAL if limit does not fit in 32-bit, as required
 * by the host-guest protocol and EIO if an error occurred in communicating with
 * the host.
 */
static int vmballoon_send_get_target(struct vmballoon *b)
{
	unsigned long status;
	unsigned long limit;

	limit = totalram_pages();

	/* Ensure limit fits in 32-bits if 64-bit targets are not supported */
	if (!(b->capabilities & VMW_BALLOON_64_BIT_TARGET) &&
	    limit != (u32)limit)
		return -EINVAL;

	status = vmballoon_cmd(b, VMW_BALLOON_CMD_GET_TARGET, limit, 0);

	return status == VMW_BALLOON_SUCCESS ? 0 : -EIO;
}

/**
 * vmballoon_alloc_page_list - allocates a list of pages.
 *
 * @b: pointer to the balloon.
 * @ctl: pointer for the %struct vmballoon_ctl, which defines the operation.
 * @req_n_pages: the number of requested pages.
 *
 * Tries to allocate @req_n_pages. Add them to the list of balloon pages in
 * @ctl.pages and updates @ctl.n_pages to reflect the number of pages.
 *
 * Return: zero on success or error code otherwise.
 */
static int vmballoon_alloc_page_list(struct vmballoon *b,
				     struct vmballoon_ctl *ctl,
				     unsigned int req_n_pages)
{
	struct page *page;
	unsigned int i;

	for (i = 0; i < req_n_pages; i++) {
		/*
		 * First check if we happen to have pages that were allocated
		 * before. This happens when 2MB page rejected during inflation
		 * by the hypervisor, and then split into 4KB pages.
		 */
		if (!list_empty(&ctl->prealloc_pages)) {
			page = list_first_entry(&ctl->prealloc_pages,
						struct page, lru);
			list_del(&page->lru);
		} else {
			if (ctl->page_size == VMW_BALLOON_2M_PAGE)
				page = alloc_pages(__GFP_HIGHMEM|__GFP_NOWARN|
					__GFP_NOMEMALLOC, VMW_BALLOON_2M_ORDER);
			else
				page = balloon_page_alloc();

			vmballoon_stats_page_inc(b, VMW_BALLOON_PAGE_STAT_ALLOC,
						 ctl->page_size);
		}

		if (page) {
			/* Success. Add the page to the list and continue. */
			list_add(&page->lru, &ctl->pages);
			continue;
		}

		/* Allocation failed. Update statistics and stop. */
		vmballoon_stats_page_inc(b, VMW_BALLOON_PAGE_STAT_ALLOC_FAIL,
					 ctl->page_size);
		break;
	}

	ctl->n_pages = i;

	return req_n_pages == ctl->n_pages ? 0 : -ENOMEM;
}

/**
 * vmballoon_handle_one_result - Handle lock/unlock result for a single page.
 *
 * @b: pointer for %struct vmballoon.
 * @page: pointer for the page whose result should be handled.
 * @page_size: size of the page.
 * @status: status of the operation as provided by the hypervisor.
 */
static int vmballoon_handle_one_result(struct vmballoon *b, struct page *page,
				       enum vmballoon_page_size_type page_size,
				       unsigned long status)
{
	/* On success do nothing. The page is already on the balloon list. */
	if (likely(status == VMW_BALLOON_SUCCESS))
		return 0;

	pr_debug("%s: failed comm pfn %lx status %lu page_size %s\n", __func__,
		 page_to_pfn(page), status,
		 vmballoon_page_size_names[page_size]);

	/* Error occurred */
	vmballoon_stats_page_inc(b, VMW_BALLOON_PAGE_STAT_REFUSED_ALLOC,
				 page_size);

	return -EIO;
}

/**
 * vmballoon_status_page - returns the status of (un)lock operation
 *
 * @b: pointer to the balloon.
 * @idx: index for the page for which the operation is performed.
 * @p: pointer to where the page struct is returned.
 *
 * Following a lock or unlock operation, returns the status of the operation for
 * an individual page. Provides the page that the operation was performed on on
 * the @page argument.
 *
 * Returns: The status of a lock or unlock operation for an individual page.
 */
static unsigned long vmballoon_status_page(struct vmballoon *b, int idx,
					   struct page **p)
{
	if (static_branch_likely(&vmw_balloon_batching)) {
		/* batching mode */
		*p = pfn_to_page(b->batch_page[idx].pfn);
		return b->batch_page[idx].status;
	}

	/* non-batching mode */
	*p = b->page;

	/*
	 * If a failure occurs, the indication will be provided in the status
	 * of the entire operation, which is considered before the individual
	 * page status. So for non-batching mode, the indication is always of
	 * success.
	 */
	return VMW_BALLOON_SUCCESS;
}

/**
 * vmballoon_lock_op - notifies the host about inflated/deflated pages.
 * @b: pointer to the balloon.
 * @num_pages: number of inflated/deflated pages.
 * @page_size: size of the page.
 * @op: the type of operation (lock or unlock).
 *
 * Notify the host about page(s) that were ballooned (or removed from the
 * balloon) so that host can use it without fear that guest will need it (or
 * stop using them since the VM does). Host may reject some pages, we need to
 * check the return value and maybe submit a different page. The pages that are
 * inflated/deflated are pointed by @b->page.
 *
 * Return: result as provided by the hypervisor.
 */
static unsigned long vmballoon_lock_op(struct vmballoon *b,
				       unsigned int num_pages,
				       enum vmballoon_page_size_type page_size,
				       enum vmballoon_op op)
{
	unsigned long cmd, pfn;

	lockdep_assert_held(&b->comm_lock);

	if (static_branch_likely(&vmw_balloon_batching)) {
		if (op == VMW_BALLOON_INFLATE)
			cmd = page_size == VMW_BALLOON_2M_PAGE ?
				VMW_BALLOON_CMD_BATCHED_2M_LOCK :
				VMW_BALLOON_CMD_BATCHED_LOCK;
		else
			cmd = page_size == VMW_BALLOON_2M_PAGE ?
				VMW_BALLOON_CMD_BATCHED_2M_UNLOCK :
				VMW_BALLOON_CMD_BATCHED_UNLOCK;

		pfn = PHYS_PFN(virt_to_phys(b->batch_page));
	} else {
		cmd = op == VMW_BALLOON_INFLATE ? VMW_BALLOON_CMD_LOCK :
						  VMW_BALLOON_CMD_UNLOCK;
		pfn = page_to_pfn(b->page);

		/* In non-batching mode, PFNs must fit in 32-bit */
		if (unlikely(pfn != (u32)pfn))
			return VMW_BALLOON_ERROR_PPN_INVALID;
	}

	return vmballoon_cmd(b, cmd, pfn, num_pages);
}

/**
 * vmballoon_add_page - adds a page towards lock/unlock operation.
 *
 * @b: pointer to the balloon.
 * @idx: index of the page to be ballooned in this batch.
 * @p: pointer to the page that is about to be ballooned.
 *
 * Adds the page to be ballooned. Must be called while holding @comm_lock.
 */
static void vmballoon_add_page(struct vmballoon *b, unsigned int idx,
			       struct page *p)
{
	lockdep_assert_held(&b->comm_lock);

	if (static_branch_likely(&vmw_balloon_batching))
		b->batch_page[idx] = (struct vmballoon_batch_entry)
					{ .pfn = page_to_pfn(p) };
	else
		b->page = p;
}

/**
 * vmballoon_lock - lock or unlock a batch of pages.
 *
 * @b: pointer to the balloon.
 * @ctl: pointer for the %struct vmballoon_ctl, which defines the operation.
 *
 * Notifies the host of about ballooned pages (after inflation or deflation,
 * according to @ctl). If the host rejects the page put it on the
 * @ctl refuse list. These refused page are then released when moving to the
 * next size of pages.
 *
 * Note that we neither free any @page here nor put them back on the ballooned
 * pages list. Instead we queue it for later processing. We do that for several
 * reasons. First, we do not want to free the page under the lock. Second, it
 * allows us to unify the handling of lock and unlock. In the inflate case, the
 * caller will check if there are too many refused pages and release them.
 * Although it is not identical to the past behavior, it should not affect
 * performance.
 */
static int vmballoon_lock(struct vmballoon *b, struct vmballoon_ctl *ctl)
{
	unsigned long batch_status;
	struct page *page;
	unsigned int i, num_pages;

	num_pages = ctl->n_pages;
	if (num_pages == 0)
		return 0;

	/* communication with the host is done under the communication lock */
	spin_lock(&b->comm_lock);

	i = 0;
	list_for_each_entry(page, &ctl->pages, lru)
		vmballoon_add_page(b, i++, page);

	batch_status = vmballoon_lock_op(b, ctl->n_pages, ctl->page_size,
					 ctl->op);

	/*
	 * Iterate over the pages in the provided list. Since we are changing
	 * @ctl->n_pages we are saving the original value in @num_pages and
	 * use this value to bound the loop.
	 */
	for (i = 0; i < num_pages; i++) {
		unsigned long status;

		status = vmballoon_status_page(b, i, &page);

		/*
		 * Failure of the whole batch overrides a single operation
		 * results.
		 */
		if (batch_status != VMW_BALLOON_SUCCESS)
			status = batch_status;

		/* Continue if no error happened */
		if (!vmballoon_handle_one_result(b, page, ctl->page_size,
						 status))
			continue;

		/*
		 * Error happened. Move the pages to the refused list and update
		 * the pages number.
		 */
		list_move(&page->lru, &ctl->refused_pages);
		ctl->n_pages--;
		ctl->n_refused_pages++;
	}

	spin_unlock(&b->comm_lock);

	return batch_status == VMW_BALLOON_SUCCESS ? 0 : -EIO;
}

/**
 * vmballoon_release_page_list() - Releases a page list
 *
 * @page_list: list of pages to release.
 * @n_pages: pointer to the number of pages.
 * @page_size: whether the pages in the list are 2MB (or else 4KB).
 *
 * Releases the list of pages and zeros the number of pages.
 */
static void vmballoon_release_page_list(struct list_head *page_list,
				       int *n_pages,
				       enum vmballoon_page_size_type page_size)
{
	struct page *page, *tmp;

	list_for_each_entry_safe(page, tmp, page_list, lru) {
		list_del(&page->lru);
		__free_pages(page, vmballoon_page_order(page_size));
	}

	if (n_pages)
		*n_pages = 0;
}


/*
 * Release pages that were allocated while attempting to inflate the
 * balloon but were refused by the host for one reason or another.
 */
static void vmballoon_release_refused_pages(struct vmballoon *b,
					    struct vmballoon_ctl *ctl)
{
	vmballoon_stats_page_inc(b, VMW_BALLOON_PAGE_STAT_REFUSED_FREE,
				 ctl->page_size);

	vmballoon_release_page_list(&ctl->refused_pages, &ctl->n_refused_pages,
				    ctl->page_size);
}

/**
 * vmballoon_change - retrieve the required balloon change
 *
 * @b: pointer for the balloon.
 *
 * Return: the required change for the balloon size. A positive number
 * indicates inflation, a negative number indicates a deflation.
 */
static int64_t vmballoon_change(struct vmballoon *b)
{
	int64_t size, target;

	size = atomic64_read(&b->size);
	target = READ_ONCE(b->target);

	/*
	 * We must cast first because of int sizes
	 * Otherwise we might get huge positives instead of negatives
	 */

	if (b->reset_required)
		return 0;

	/* consider a 2MB slack on deflate, unless the balloon is emptied */
	if (target < size && target != 0 &&
	    size - target < vmballoon_page_in_frames(VMW_BALLOON_2M_PAGE))
		return 0;

	/* If an out-of-memory recently occurred, inflation is disallowed. */
	if (target > size && time_before(jiffies, READ_ONCE(b->shrink_timeout)))
		return 0;

	return target - size;
}

/**
 * vmballoon_enqueue_page_list() - Enqueues list of pages after inflation.
 *
 * @b: pointer to balloon.
 * @pages: list of pages to enqueue.
 * @n_pages: pointer to number of pages in list. The value is zeroed.
 * @page_size: whether the pages are 2MB or 4KB pages.
 *
 * Enqueues the provides list of pages in the ballooned page list, clears the
 * list and zeroes the number of pages that was provided.
 */
static void vmballoon_enqueue_page_list(struct vmballoon *b,
					struct list_head *pages,
					unsigned int *n_pages,
					enum vmballoon_page_size_type page_size)
{
	unsigned long flags;
	struct page *page;

	if (page_size == VMW_BALLOON_4K_PAGE) {
		balloon_page_list_enqueue(&b->b_dev_info, pages);
	} else {
		/*
		 * Keep the huge pages in a local list which is not available
		 * for the balloon compaction mechanism.
		 */
		spin_lock_irqsave(&b->b_dev_info.pages_lock, flags);

		list_for_each_entry(page, pages, lru) {
			vmballoon_mark_page_offline(page, VMW_BALLOON_2M_PAGE);
		}

		list_splice_init(pages, &b->huge_pages);
		__count_vm_events(BALLOON_INFLATE, *n_pages *
				  vmballoon_page_in_frames(VMW_BALLOON_2M_PAGE));
		spin_unlock_irqrestore(&b->b_dev_info.pages_lock, flags);
	}

	*n_pages = 0;
}

/**
 * vmballoon_dequeue_page_list() - Dequeues page lists for deflation.
 *
 * @b: pointer to balloon.
 * @pages: list of pages to enqueue.
 * @n_pages: pointer to number of pages in list. The value is zeroed.
 * @page_size: whether the pages are 2MB or 4KB pages.
 * @n_req_pages: the number of requested pages.
 *
 * Dequeues the number of requested pages from the balloon for deflation. The
 * number of dequeued pages may be lower, if not enough pages in the requested
 * size are available.
 */
static void vmballoon_dequeue_page_list(struct vmballoon *b,
					struct list_head *pages,
					unsigned int *n_pages,
					enum vmballoon_page_size_type page_size,
					unsigned int n_req_pages)
{
	struct page *page, *tmp;
	unsigned int i = 0;
	unsigned long flags;

	/* In the case of 4k pages, use the compaction infrastructure */
	if (page_size == VMW_BALLOON_4K_PAGE) {
		*n_pages = balloon_page_list_dequeue(&b->b_dev_info, pages,
						     n_req_pages);
		return;
	}

	/* 2MB pages */
	spin_lock_irqsave(&b->b_dev_info.pages_lock, flags);
	list_for_each_entry_safe(page, tmp, &b->huge_pages, lru) {
		vmballoon_mark_page_online(page, VMW_BALLOON_2M_PAGE);

		list_move(&page->lru, pages);
		if (++i == n_req_pages)
			break;
	}

	__count_vm_events(BALLOON_DEFLATE,
			  i * vmballoon_page_in_frames(VMW_BALLOON_2M_PAGE));
	spin_unlock_irqrestore(&b->b_dev_info.pages_lock, flags);
	*n_pages = i;
}

/**
 * vmballoon_split_refused_pages() - Split the 2MB refused pages to 4k.
 *
 * If inflation of 2MB pages was denied by the hypervisor, it is likely to be
 * due to one or few 4KB pages. These 2MB pages may keep being allocated and
 * then being refused. To prevent this case, this function splits the refused
 * pages into 4KB pages and adds them into @prealloc_pages list.
 *
 * @ctl: pointer for the %struct vmballoon_ctl, which defines the operation.
 */
static void vmballoon_split_refused_pages(struct vmballoon_ctl *ctl)
{
	struct page *page, *tmp;
	unsigned int i, order;

	order = vmballoon_page_order(ctl->page_size);

	list_for_each_entry_safe(page, tmp, &ctl->refused_pages, lru) {
		list_del(&page->lru);
		split_page(page, order);
		for (i = 0; i < (1 << order); i++)
			list_add(&page[i].lru, &ctl->prealloc_pages);
	}
	ctl->n_refused_pages = 0;
}

/**
 * vmballoon_inflate() - Inflate the balloon towards its target size.
 *
 * @b: pointer to the balloon.
 */
static void vmballoon_inflate(struct vmballoon *b)
{
	int64_t to_inflate_frames;
	struct vmballoon_ctl ctl = {
		.pages = LIST_HEAD_INIT(ctl.pages),
		.refused_pages = LIST_HEAD_INIT(ctl.refused_pages),
		.prealloc_pages = LIST_HEAD_INIT(ctl.prealloc_pages),
		.page_size = b->max_page_size,
		.op = VMW_BALLOON_INFLATE
	};

	while ((to_inflate_frames = vmballoon_change(b)) > 0) {
		unsigned int to_inflate_pages, page_in_frames;
		int alloc_error, lock_error = 0;

		VM_BUG_ON(!list_empty(&ctl.pages));
		VM_BUG_ON(ctl.n_pages != 0);

		page_in_frames = vmballoon_page_in_frames(ctl.page_size);

		to_inflate_pages = min_t(unsigned long, b->batch_max_pages,
					 DIV_ROUND_UP_ULL(to_inflate_frames,
							  page_in_frames));

		/* Start by allocating */
		alloc_error = vmballoon_alloc_page_list(b, &ctl,
							to_inflate_pages);

		/* Actually lock the pages by telling the hypervisor */
		lock_error = vmballoon_lock(b, &ctl);

		/*
		 * If an error indicates that something serious went wrong,
		 * stop the inflation.
		 */
		if (lock_error)
			break;

		/* Update the balloon size */
		atomic64_add(ctl.n_pages * page_in_frames, &b->size);

		vmballoon_enqueue_page_list(b, &ctl.pages, &ctl.n_pages,
					    ctl.page_size);

		/*
		 * If allocation failed or the number of refused pages exceeds
		 * the maximum allowed, move to the next page size.
		 */
		if (alloc_error ||
		    ctl.n_refused_pages >= VMW_BALLOON_MAX_REFUSED) {
			if (ctl.page_size == VMW_BALLOON_4K_PAGE)
				break;

			/*
			 * Split the refused pages to 4k. This will also empty
			 * the refused pages list.
			 */
			vmballoon_split_refused_pages(&ctl);
			ctl.page_size--;
		}

		cond_resched();
	}

	/*
	 * Release pages that were allocated while attempting to inflate the
	 * balloon but were refused by the host for one reason or another,
	 * and update the statistics.
	 */
	if (ctl.n_refused_pages != 0)
		vmballoon_release_refused_pages(b, &ctl);

	vmballoon_release_page_list(&ctl.prealloc_pages, NULL, ctl.page_size);
}

/**
 * vmballoon_deflate() - Decrease the size of the balloon.
 *
 * @b: pointer to the balloon
 * @n_frames: the number of frames to deflate. If zero, automatically
 * calculated according to the target size.
 * @coordinated: whether to coordinate with the host
 *
 * Decrease the size of the balloon allowing guest to use more memory.
 *
 * Return: The number of deflated frames (i.e., basic page size units)
 */
static unsigned long vmballoon_deflate(struct vmballoon *b, uint64_t n_frames,
				       bool coordinated)
{
	unsigned long deflated_frames = 0;
	unsigned long tried_frames = 0;
	struct vmballoon_ctl ctl = {
		.pages = LIST_HEAD_INIT(ctl.pages),
		.refused_pages = LIST_HEAD_INIT(ctl.refused_pages),
		.page_size = VMW_BALLOON_4K_PAGE,
		.op = VMW_BALLOON_DEFLATE
	};

	/* free pages to reach target */
	while (true) {
		unsigned int to_deflate_pages, n_unlocked_frames;
		unsigned int page_in_frames;
		int64_t to_deflate_frames;
		bool deflated_all;

		page_in_frames = vmballoon_page_in_frames(ctl.page_size);

		VM_BUG_ON(!list_empty(&ctl.pages));
		VM_BUG_ON(ctl.n_pages);
		VM_BUG_ON(!list_empty(&ctl.refused_pages));
		VM_BUG_ON(ctl.n_refused_pages);

		/*
		 * If we were requested a specific number of frames, we try to
		 * deflate this number of frames. Otherwise, deflation is
		 * performed according to the target and balloon size.
		 */
		to_deflate_frames = n_frames ? n_frames - tried_frames :
					       -vmballoon_change(b);

		/* break if no work to do */
		if (to_deflate_frames <= 0)
			break;

		/*
		 * Calculate the number of frames based on current page size,
		 * but limit the deflated frames to a single chunk
		 */
		to_deflate_pages = min_t(unsigned long, b->batch_max_pages,
					 DIV_ROUND_UP_ULL(to_deflate_frames,
							  page_in_frames));

		/* First take the pages from the balloon pages. */
		vmballoon_dequeue_page_list(b, &ctl.pages, &ctl.n_pages,
					    ctl.page_size, to_deflate_pages);

		/*
		 * Before pages are moving to the refused list, count their
		 * frames as frames that we tried to deflate.
		 */
		tried_frames += ctl.n_pages * page_in_frames;

		/*
		 * Unlock the pages by communicating with the hypervisor if the
		 * communication is coordinated (i.e., not pop). We ignore the
		 * return code. Instead we check if all the pages we manage to
		 * unlock all the pages. If we failed, we will move to the next
		 * page size, and would eventually try again later.
		 */
		if (coordinated)
			vmballoon_lock(b, &ctl);

		/*
		 * Check if we deflated enough. We will move to the next page
		 * size if we did not manage to do so. This calculation takes
		 * place now, as once the pages are released, the number of
		 * pages is zeroed.
		 */
		deflated_all = (ctl.n_pages == to_deflate_pages);

		/* Update local and global counters */
		n_unlocked_frames = ctl.n_pages * page_in_frames;
		atomic64_sub(n_unlocked_frames, &b->size);
		deflated_frames += n_unlocked_frames;

		vmballoon_stats_page_add(b, VMW_BALLOON_PAGE_STAT_FREE,
					 ctl.page_size, ctl.n_pages);

		/* free the ballooned pages */
		vmballoon_release_page_list(&ctl.pages, &ctl.n_pages,
					    ctl.page_size);

		/* Return the refused pages to the ballooned list. */
		vmballoon_enqueue_page_list(b, &ctl.refused_pages,
					    &ctl.n_refused_pages,
					    ctl.page_size);

		/* If we failed to unlock all the pages, move to next size. */
		if (!deflated_all) {
			if (ctl.page_size == b->max_page_size)
				break;
			ctl.page_size++;
		}

		cond_resched();
	}

	return deflated_frames;
}

/**
 * vmballoon_deinit_batching - disables batching mode.
 *
 * @b: pointer to &struct vmballoon.
 *
 * Disables batching, by deallocating the page for communication with the
 * hypervisor and disabling the static key to indicate that batching is off.
 */
static void vmballoon_deinit_batching(struct vmballoon *b)
{
	free_page((unsigned long)b->batch_page);
	b->batch_page = NULL;
	static_branch_disable(&vmw_balloon_batching);
	b->batch_max_pages = 1;
}

/**
 * vmballoon_init_batching - enable batching mode.
 *
 * @b: pointer to &struct vmballoon.
 *
 * Enables batching, by allocating a page for communication with the hypervisor
 * and enabling the static_key to use batching.
 *
 * Return: zero on success or an appropriate error-code.
 */
static int vmballoon_init_batching(struct vmballoon *b)
{
	struct page *page;

	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page)
		return -ENOMEM;

	b->batch_page = page_address(page);
	b->batch_max_pages = PAGE_SIZE / sizeof(struct vmballoon_batch_entry);

	static_branch_enable(&vmw_balloon_batching);

	return 0;
}

/*
 * Receive notification and resize balloon
 */
static void vmballoon_doorbell(void *client_data)
{
	struct vmballoon *b = client_data;

	vmballoon_stats_gen_inc(b, VMW_BALLOON_STAT_DOORBELL);

	mod_delayed_work(system_freezable_wq, &b->dwork, 0);
}

/*
 * Clean up vmci doorbell
 */
static void vmballoon_vmci_cleanup(struct vmballoon *b)
{
	vmballoon_cmd(b, VMW_BALLOON_CMD_VMCI_DOORBELL_SET,
		      VMCI_INVALID_ID, VMCI_INVALID_ID);

	if (!vmci_handle_is_invalid(b->vmci_doorbell)) {
		vmci_doorbell_destroy(b->vmci_doorbell);
		b->vmci_doorbell = VMCI_INVALID_HANDLE;
	}
}

/**
 * vmballoon_vmci_init - Initialize vmci doorbell.
 *
 * @b: pointer to the balloon.
 *
 * Return: zero on success or when wakeup command not supported. Error-code
 * otherwise.
 *
 * Initialize vmci doorbell, to get notified as soon as balloon changes.
 */
static int vmballoon_vmci_init(struct vmballoon *b)
{
	unsigned long error;

	if ((b->capabilities & VMW_BALLOON_SIGNALLED_WAKEUP_CMD) == 0)
		return 0;

	error = vmci_doorbell_create(&b->vmci_doorbell, VMCI_FLAG_DELAYED_CB,
				     VMCI_PRIVILEGE_FLAG_RESTRICTED,
				     vmballoon_doorbell, b);

	if (error != VMCI_SUCCESS)
		goto fail;

	error =	__vmballoon_cmd(b, VMW_BALLOON_CMD_VMCI_DOORBELL_SET,
				b->vmci_doorbell.context,
				b->vmci_doorbell.resource, NULL);

	if (error != VMW_BALLOON_SUCCESS)
		goto fail;

	return 0;
fail:
	vmballoon_vmci_cleanup(b);
	return -EIO;
}

/**
 * vmballoon_pop - Quickly release all pages allocate for the balloon.
 *
 * @b: pointer to the balloon.
 *
 * This function is called when host decides to "reset" balloon for one reason
 * or another. Unlike normal "deflate" we do not (shall not) notify host of the
 * pages being released.
 */
static void vmballoon_pop(struct vmballoon *b)
{
	unsigned long size;

	while ((size = atomic64_read(&b->size)))
		vmballoon_deflate(b, size, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Perform standard reset sequence by popping the balloon (in case it
 * is not  empty) and then restarting protocol. This operation normally
 * happens when host responds with VMW_BALLOON_ERROR_RESET to a command.
 */
static void vmballoon_reset(struct vmballoon *b)
{
<<<<<<< HEAD
	/* free all pages, skipping monitor unlock */
	vmballoon_pop(b);

	if (vmballoon_send_start(b)) {
		b->reset_required = false;
		if (!vmballoon_send_guest_id(b))
			pr_err("failed to send guest ID to the host\n");
	}
}

/*
 * Allocate (or reserve) a page for the balloon and notify the host.  If host
 * refuses the page put it on "refuse" list and allocate another one until host
 * is satisfied. "Refused" pages are released at the end of inflation cycle
 * (when we allocate b->rate_alloc pages).
 */
static int vmballoon_reserve_page(struct vmballoon *b, bool can_sleep)
{
	struct page *page;
	gfp_t flags;
	unsigned int hv_status;
	int locked;
	flags = can_sleep ? VMW_PAGE_ALLOC_CANSLEEP : VMW_PAGE_ALLOC_NOSLEEP;

	do {
		if (!can_sleep)
			STATS_INC(b->stats.alloc);
		else
			STATS_INC(b->stats.sleep_alloc);

		page = alloc_page(flags);
		if (!page) {
			if (!can_sleep)
				STATS_INC(b->stats.alloc_fail);
			else
				STATS_INC(b->stats.sleep_alloc_fail);
			return -ENOMEM;
		}

		/* inform monitor */
		locked = vmballoon_send_lock_page(b, page_to_pfn(page), &hv_status);
		if (locked > 0) {
			STATS_INC(b->stats.refused_alloc);

			if (hv_status == VMW_BALLOON_ERROR_RESET ||
			    hv_status == VMW_BALLOON_ERROR_PPN_NOTNEEDED) {
				__free_page(page);
				return -EIO;
			}

			/*
			 * Place page on the list of non-balloonable pages
			 * and retry allocation, unless we already accumulated
			 * too many of them, in which case take a breather.
			 */
			list_add(&page->lru, &b->refused_pages);
			if (++b->n_refused_pages >= VMW_BALLOON_MAX_REFUSED)
				return -EIO;
		}
	} while (locked != 0);

	/* track allocated page */
	list_add(&page->lru, &b->pages);

	/* update balloon size */
	b->size++;

	return 0;
}

/*
 * Release the page allocated for the balloon. Note that we first notify
 * the host so it can make sure the page will be available for the guest
 * to use, if needed.
 */
static int vmballoon_release_page(struct vmballoon *b, struct page *page)
{
	if (!vmballoon_send_unlock_page(b, page_to_pfn(page)))
		return -EIO;

	list_del(&page->lru);

	/* deallocate page */
	__free_page(page);
	STATS_INC(b->stats.free);

	/* update balloon size */
	b->size--;

	return 0;
}

/*
 * Release pages that were allocated while attempting to inflate the
 * balloon but were refused by the host for one reason or another.
 */
static void vmballoon_release_refused_pages(struct vmballoon *b)
{
	struct page *page, *next;

	list_for_each_entry_safe(page, next, &b->refused_pages, lru) {
		list_del(&page->lru);
		__free_page(page);
		STATS_INC(b->stats.refused_free);
	}

	b->n_refused_pages = 0;
}

/*
 * Inflate the balloon towards its target size. Note that we try to limit
 * the rate of allocation to make sure we are not choking the rest of the
 * system.
 */
static void vmballoon_inflate(struct vmballoon *b)
{
	unsigned int goal;
	unsigned int rate;
	unsigned int i;
	unsigned int allocations = 0;
	int error = 0;
	bool alloc_can_sleep = false;

	pr_debug("%s - size: %d, target %d\n", __func__, b->size, b->target);

	/*
	 * First try NOSLEEP page allocations to inflate balloon.
	 *
	 * If we do not throttle nosleep allocations, we can drain all
	 * free pages in the guest quickly (if the balloon target is high).
	 * As a side-effect, draining free pages helps to inform (force)
	 * the guest to start swapping if balloon target is not met yet,
	 * which is a desired behavior. However, balloon driver can consume
	 * all available CPU cycles if too many pages are allocated in a
	 * second. Therefore, we throttle nosleep allocations even when
	 * the guest is not under memory pressure. OTOH, if we have already
	 * predicted that the guest is under memory pressure, then we
	 * slowdown page allocations considerably.
	 */

	goal = b->target - b->size;
	/*
	 * Start with no sleep allocation rate which may be higher
	 * than sleeping allocation rate.
	 */
	rate = b->slow_allocation_cycles ?
			b->rate_alloc : VMW_BALLOON_NOSLEEP_ALLOC_MAX;

	pr_debug("%s - goal: %d, no-sleep rate: %d, sleep rate: %d\n",
		 __func__, goal, rate, b->rate_alloc);

	for (i = 0; i < goal; i++) {

		error = vmballoon_reserve_page(b, alloc_can_sleep);
		if (error) {
			if (error != -ENOMEM) {
				/*
				 * Not a page allocation failure, stop this
				 * cycle. Maybe we'll get new target from
				 * the host soon.
				 */
				break;
			}

			if (alloc_can_sleep) {
				/*
				 * CANSLEEP page allocation failed, so guest
				 * is under severe memory pressure. Quickly
				 * decrease allocation rate.
				 */
				b->rate_alloc = max(b->rate_alloc / 2,
						    VMW_BALLOON_RATE_ALLOC_MIN);
				break;
			}

			/*
			 * NOSLEEP page allocation failed, so the guest is
			 * under memory pressure. Let us slow down page
			 * allocations for next few cycles so that the guest
			 * gets out of memory pressure. Also, if we already
			 * allocated b->rate_alloc pages, let's pause,
			 * otherwise switch to sleeping allocations.
			 */
			b->slow_allocation_cycles = VMW_BALLOON_SLOW_CYCLES;

			if (i >= b->rate_alloc)
				break;

			alloc_can_sleep = true;
			/* Lower rate for sleeping allocations. */
			rate = b->rate_alloc;
		}

		if (++allocations > VMW_BALLOON_YIELD_THRESHOLD) {
			cond_resched();
			allocations = 0;
		}

		if (i >= rate) {
			/* We allocated enough pages, let's take a break. */
			break;
		}
	}

	/*
	 * We reached our goal without failures so try increasing
	 * allocation rate.
	 */
	if (error == 0 && i >= b->rate_alloc) {
		unsigned int mult = i / b->rate_alloc;

		b->rate_alloc =
			min(b->rate_alloc + mult * VMW_BALLOON_RATE_ALLOC_INC,
			    VMW_BALLOON_RATE_ALLOC_MAX);
	}

	vmballoon_release_refused_pages(b);
}

/*
 * Decrease the size of the balloon allowing guest to use more memory.
 */
static void vmballoon_deflate(struct vmballoon *b)
{
	struct page *page, *next;
	unsigned int i = 0;
	unsigned int goal;
	int error;

	pr_debug("%s - size: %d, target %d\n", __func__, b->size, b->target);

	/* limit deallocation rate */
	goal = min(b->size - b->target, b->rate_free);

	pr_debug("%s - goal: %d, rate: %d\n", __func__, goal, b->rate_free);

	/* free pages to reach target */
	list_for_each_entry_safe(page, next, &b->pages, lru) {
		error = vmballoon_release_page(b, page);
		if (error) {
			/* quickly decrease rate in case of error */
			b->rate_free = max(b->rate_free / 2,
					   VMW_BALLOON_RATE_FREE_MIN);
			return;
		}

		if (++i >= goal)
			break;
	}

	/* slowly increase rate if there were no errors */
	b->rate_free = min(b->rate_free + VMW_BALLOON_RATE_FREE_INC,
			   VMW_BALLOON_RATE_FREE_MAX);
}

/*
 * Balloon work function: reset protocol, if needed, get the new size and
 * adjust balloon as needed. Repeat in 1 sec.
=======
	int error;

	down_write(&b->conf_sem);

	vmballoon_vmci_cleanup(b);

	/* free all pages, skipping monitor unlock */
	vmballoon_pop(b);

	if (vmballoon_send_start(b, VMW_BALLOON_CAPABILITIES))
		goto unlock;

	if ((b->capabilities & VMW_BALLOON_BATCHED_CMDS) != 0) {
		if (vmballoon_init_batching(b)) {
			/*
			 * We failed to initialize batching, inform the monitor
			 * about it by sending a null capability.
			 *
			 * The guest will retry in one second.
			 */
			vmballoon_send_start(b, 0);
			goto unlock;
		}
	} else if ((b->capabilities & VMW_BALLOON_BASIC_CMDS) != 0) {
		vmballoon_deinit_batching(b);
	}

	vmballoon_stats_gen_inc(b, VMW_BALLOON_STAT_RESET);
	b->reset_required = false;

	error = vmballoon_vmci_init(b);
	if (error)
		pr_err_once("failed to initialize vmci doorbell\n");

	if (vmballoon_send_guest_id(b))
		pr_err_once("failed to send guest ID to the host\n");

unlock:
	up_write(&b->conf_sem);
}

/**
 * vmballoon_work - periodic balloon worker for reset, inflation and deflation.
 *
 * @work: pointer to the &work_struct which is provided by the workqueue.
 *
 * Resets the protocol if needed, gets the new size and adjusts balloon as
 * needed. Repeat in 1 sec.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void vmballoon_work(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct vmballoon *b = container_of(dwork, struct vmballoon, dwork);
<<<<<<< HEAD
	unsigned int target;

	STATS_INC(b->stats.timer);
=======
	int64_t change = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (b->reset_required)
		vmballoon_reset(b);

<<<<<<< HEAD
	if (b->slow_allocation_cycles > 0)
		b->slow_allocation_cycles--;

	if (vmballoon_send_get_target(b, &target)) {
		/* update target, adjust size */
		b->target = target;

		if (b->size < target)
			vmballoon_inflate(b);
		else if (b->size > target)
			vmballoon_deflate(b);
	}

=======
	down_read(&b->conf_sem);

	/*
	 * Update the stats while holding the semaphore to ensure that
	 * @stats_enabled is consistent with whether the stats are actually
	 * enabled
	 */
	vmballoon_stats_gen_inc(b, VMW_BALLOON_STAT_TIMER);

	if (!vmballoon_send_get_target(b))
		change = vmballoon_change(b);

	if (change != 0) {
		pr_debug("%s - size: %llu, target %lu\n", __func__,
			 atomic64_read(&b->size), READ_ONCE(b->target));

		if (change > 0)
			vmballoon_inflate(b);
		else  /* (change < 0) */
			vmballoon_deflate(b, 0, true);
	}

	up_read(&b->conf_sem);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We are using a freezable workqueue so that balloon operations are
	 * stopped while the system transitions to/from sleep/hibernation.
	 */
	queue_delayed_work(system_freezable_wq,
			   dwork, round_jiffies_relative(HZ));
<<<<<<< HEAD
=======

}

/**
 * vmballoon_shrinker_scan() - deflate the balloon due to memory pressure.
 * @shrinker: pointer to the balloon shrinker.
 * @sc: page reclaim information.
 *
 * Returns: number of pages that were freed during deflation.
 */
static unsigned long vmballoon_shrinker_scan(struct shrinker *shrinker,
					     struct shrink_control *sc)
{
	struct vmballoon *b = &balloon;
	unsigned long deflated_frames;

	pr_debug("%s - size: %llu", __func__, atomic64_read(&b->size));

	vmballoon_stats_gen_inc(b, VMW_BALLOON_STAT_SHRINK);

	/*
	 * If the lock is also contended for read, we cannot easily reclaim and
	 * we bail out.
	 */
	if (!down_read_trylock(&b->conf_sem))
		return 0;

	deflated_frames = vmballoon_deflate(b, sc->nr_to_scan, true);

	vmballoon_stats_gen_add(b, VMW_BALLOON_STAT_SHRINK_FREE,
				deflated_frames);

	/*
	 * Delay future inflation for some time to mitigate the situations in
	 * which balloon continuously grows and shrinks. Use WRITE_ONCE() since
	 * the access is asynchronous.
	 */
	WRITE_ONCE(b->shrink_timeout, jiffies + HZ * VMBALLOON_SHRINK_DELAY);

	up_read(&b->conf_sem);

	return deflated_frames;
}

/**
 * vmballoon_shrinker_count() - return the number of ballooned pages.
 * @shrinker: pointer to the balloon shrinker.
 * @sc: page reclaim information.
 *
 * Returns: number of 4k pages that are allocated for the balloon and can
 *	    therefore be reclaimed under pressure.
 */
static unsigned long vmballoon_shrinker_count(struct shrinker *shrinker,
					      struct shrink_control *sc)
{
	struct vmballoon *b = &balloon;

	return atomic64_read(&b->size);
}

static void vmballoon_unregister_shrinker(struct vmballoon *b)
{
	shrinker_free(b->shrinker);
	b->shrinker = NULL;
}

static int vmballoon_register_shrinker(struct vmballoon *b)
{
	/* Do nothing if the shrinker is not enabled */
	if (!vmwballoon_shrinker_enable)
		return 0;

	b->shrinker = shrinker_alloc(0, "vmw-balloon");
	if (!b->shrinker)
		return -ENOMEM;

	b->shrinker->scan_objects = vmballoon_shrinker_scan;
	b->shrinker->count_objects = vmballoon_shrinker_count;
	b->shrinker->private_data = b;

	shrinker_register(b->shrinker);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * DEBUGFS Interface
 */
#ifdef CONFIG_DEBUG_FS

<<<<<<< HEAD
static int vmballoon_debug_show(struct seq_file *f, void *offset)
{
	struct vmballoon *b = f->private;
	struct vmballoon_stats *stats = &b->stats;

	/* format size info */
	seq_printf(f,
		   "target:             %8d pages\n"
		   "current:            %8d pages\n",
		   b->target, b->size);

	/* format rate info */
	seq_printf(f,
		   "rateNoSleepAlloc:   %8d pages/sec\n"
		   "rateSleepAlloc:     %8d pages/sec\n"
		   "rateFree:           %8d pages/sec\n",
		   VMW_BALLOON_NOSLEEP_ALLOC_MAX,
		   b->rate_alloc, b->rate_free);

	seq_printf(f,
		   "\n"
		   "timer:              %8u\n"
		   "start:              %8u (%4u failed)\n"
		   "guestType:          %8u (%4u failed)\n"
		   "lock:               %8u (%4u failed)\n"
		   "unlock:             %8u (%4u failed)\n"
		   "target:             %8u (%4u failed)\n"
		   "primNoSleepAlloc:   %8u (%4u failed)\n"
		   "primCanSleepAlloc:  %8u (%4u failed)\n"
		   "primFree:           %8u\n"
		   "errAlloc:           %8u\n"
		   "errFree:            %8u\n",
		   stats->timer,
		   stats->start, stats->start_fail,
		   stats->guest_type, stats->guest_type_fail,
		   stats->lock,  stats->lock_fail,
		   stats->unlock, stats->unlock_fail,
		   stats->target, stats->target_fail,
		   stats->alloc, stats->alloc_fail,
		   stats->sleep_alloc, stats->sleep_alloc_fail,
		   stats->free,
		   stats->refused_alloc, stats->refused_free);

	return 0;
}

static int vmballoon_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, vmballoon_debug_show, inode->i_private);
}

static const struct file_operations vmballoon_debug_fops = {
	.owner		= THIS_MODULE,
	.open		= vmballoon_debug_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init vmballoon_debugfs_init(struct vmballoon *b)
{
	int error;

	b->dbg_entry = debugfs_create_file("vmmemctl", S_IRUGO, NULL, b,
					   &vmballoon_debug_fops);
	if (IS_ERR(b->dbg_entry)) {
		error = PTR_ERR(b->dbg_entry);
		pr_err("failed to create debugfs entry, error: %d\n", error);
		return error;
=======
static const char * const vmballoon_stat_page_names[] = {
	[VMW_BALLOON_PAGE_STAT_ALLOC]		= "alloc",
	[VMW_BALLOON_PAGE_STAT_ALLOC_FAIL]	= "allocFail",
	[VMW_BALLOON_PAGE_STAT_REFUSED_ALLOC]	= "errAlloc",
	[VMW_BALLOON_PAGE_STAT_REFUSED_FREE]	= "errFree",
	[VMW_BALLOON_PAGE_STAT_FREE]		= "free"
};

static const char * const vmballoon_stat_names[] = {
	[VMW_BALLOON_STAT_TIMER]		= "timer",
	[VMW_BALLOON_STAT_DOORBELL]		= "doorbell",
	[VMW_BALLOON_STAT_RESET]		= "reset",
	[VMW_BALLOON_STAT_SHRINK]		= "shrink",
	[VMW_BALLOON_STAT_SHRINK_FREE]		= "shrinkFree"
};

static int vmballoon_enable_stats(struct vmballoon *b)
{
	int r = 0;

	down_write(&b->conf_sem);

	/* did we somehow race with another reader which enabled stats? */
	if (b->stats)
		goto out;

	b->stats = kzalloc(sizeof(*b->stats), GFP_KERNEL);

	if (!b->stats) {
		/* allocation failed */
		r = -ENOMEM;
		goto out;
	}
	static_key_enable(&balloon_stat_enabled.key);
out:
	up_write(&b->conf_sem);
	return r;
}

/**
 * vmballoon_debug_show - shows statistics of balloon operations.
 * @f: pointer to the &struct seq_file.
 * @offset: ignored.
 *
 * Provides the statistics that can be accessed in vmmemctl in the debugfs.
 * To avoid the overhead - mainly that of memory - of collecting the statistics,
 * we only collect statistics after the first time the counters are read.
 *
 * Return: zero on success or an error code.
 */
static int vmballoon_debug_show(struct seq_file *f, void *offset)
{
	struct vmballoon *b = f->private;
	int i, j;

	/* enables stats if they are disabled */
	if (!b->stats) {
		int r = vmballoon_enable_stats(b);

		if (r)
			return r;
	}

	/* format capabilities info */
	seq_printf(f, "%-22s: %#16x\n", "balloon capabilities",
		   VMW_BALLOON_CAPABILITIES);
	seq_printf(f, "%-22s: %#16lx\n", "used capabilities", b->capabilities);
	seq_printf(f, "%-22s: %16s\n", "is resetting",
		   b->reset_required ? "y" : "n");

	/* format size info */
	seq_printf(f, "%-22s: %16lu\n", "target", READ_ONCE(b->target));
	seq_printf(f, "%-22s: %16llu\n", "current", atomic64_read(&b->size));

	for (i = 0; i < VMW_BALLOON_CMD_NUM; i++) {
		if (vmballoon_cmd_names[i] == NULL)
			continue;

		seq_printf(f, "%-22s: %16llu (%llu failed)\n",
			   vmballoon_cmd_names[i],
			   atomic64_read(&b->stats->ops[i][VMW_BALLOON_OP_STAT]),
			   atomic64_read(&b->stats->ops[i][VMW_BALLOON_OP_FAIL_STAT]));
	}

	for (i = 0; i < VMW_BALLOON_STAT_NUM; i++)
		seq_printf(f, "%-22s: %16llu\n",
			   vmballoon_stat_names[i],
			   atomic64_read(&b->stats->general_stat[i]));

	for (i = 0; i < VMW_BALLOON_PAGE_STAT_NUM; i++) {
		for (j = 0; j < VMW_BALLOON_NUM_PAGE_SIZES; j++)
			seq_printf(f, "%-18s(%s): %16llu\n",
				   vmballoon_stat_page_names[i],
				   vmballoon_page_size_names[j],
				   atomic64_read(&b->stats->page_stat[i][j]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static void __exit vmballoon_debugfs_exit(struct vmballoon *b)
{
	debugfs_remove(b->dbg_entry);
=======
DEFINE_SHOW_ATTRIBUTE(vmballoon_debug);

static void __init vmballoon_debugfs_init(struct vmballoon *b)
{
	debugfs_create_file("vmmemctl", S_IRUGO, NULL, b,
			    &vmballoon_debug_fops);
}

static void __exit vmballoon_debugfs_exit(struct vmballoon *b)
{
	static_key_disable(&balloon_stat_enabled.key);
	debugfs_lookup_and_remove("vmmemctl", NULL);
	kfree(b->stats);
	b->stats = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#else

<<<<<<< HEAD
static inline int vmballoon_debugfs_init(struct vmballoon *b)
{
	return 0;
=======
static inline void vmballoon_debugfs_init(struct vmballoon *b)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void vmballoon_debugfs_exit(struct vmballoon *b)
{
}

#endif	/* CONFIG_DEBUG_FS */

<<<<<<< HEAD
=======

#ifdef CONFIG_BALLOON_COMPACTION
/**
 * vmballoon_migratepage() - migrates a balloon page.
 * @b_dev_info: balloon device information descriptor.
 * @newpage: the page to which @page should be migrated.
 * @page: a ballooned page that should be migrated.
 * @mode: migration mode, ignored.
 *
 * This function is really open-coded, but that is according to the interface
 * that balloon_compaction provides.
 *
 * Return: zero on success, -EAGAIN when migration cannot be performed
 *	   momentarily, and -EBUSY if migration failed and should be retried
 *	   with that specific page.
 */
static int vmballoon_migratepage(struct balloon_dev_info *b_dev_info,
				 struct page *newpage, struct page *page,
				 enum migrate_mode mode)
{
	unsigned long status, flags;
	struct vmballoon *b;
	int ret;

	b = container_of(b_dev_info, struct vmballoon, b_dev_info);

	/*
	 * If the semaphore is taken, there is ongoing configuration change
	 * (i.e., balloon reset), so try again.
	 */
	if (!down_read_trylock(&b->conf_sem))
		return -EAGAIN;

	spin_lock(&b->comm_lock);
	/*
	 * We must start by deflating and not inflating, as otherwise the
	 * hypervisor may tell us that it has enough memory and the new page is
	 * not needed. Since the old page is isolated, we cannot use the list
	 * interface to unlock it, as the LRU field is used for isolation.
	 * Instead, we use the native interface directly.
	 */
	vmballoon_add_page(b, 0, page);
	status = vmballoon_lock_op(b, 1, VMW_BALLOON_4K_PAGE,
				   VMW_BALLOON_DEFLATE);

	if (status == VMW_BALLOON_SUCCESS)
		status = vmballoon_status_page(b, 0, &page);

	/*
	 * If a failure happened, let the migration mechanism know that it
	 * should not retry.
	 */
	if (status != VMW_BALLOON_SUCCESS) {
		spin_unlock(&b->comm_lock);
		ret = -EBUSY;
		goto out_unlock;
	}

	/*
	 * The page is isolated, so it is safe to delete it without holding
	 * @pages_lock . We keep holding @comm_lock since we will need it in a
	 * second.
	 */
	balloon_page_delete(page);

	put_page(page);

	/* Inflate */
	vmballoon_add_page(b, 0, newpage);
	status = vmballoon_lock_op(b, 1, VMW_BALLOON_4K_PAGE,
				   VMW_BALLOON_INFLATE);

	if (status == VMW_BALLOON_SUCCESS)
		status = vmballoon_status_page(b, 0, &newpage);

	spin_unlock(&b->comm_lock);

	if (status != VMW_BALLOON_SUCCESS) {
		/*
		 * A failure happened. While we can deflate the page we just
		 * inflated, this deflation can also encounter an error. Instead
		 * we will decrease the size of the balloon to reflect the
		 * change and report failure.
		 */
		atomic64_dec(&b->size);
		ret = -EBUSY;
	} else {
		/*
		 * Success. Take a reference for the page, and we will add it to
		 * the list after acquiring the lock.
		 */
		get_page(newpage);
		ret = MIGRATEPAGE_SUCCESS;
	}

	/* Update the balloon list under the @pages_lock */
	spin_lock_irqsave(&b->b_dev_info.pages_lock, flags);

	/*
	 * On inflation success, we already took a reference for the @newpage.
	 * If we succeed just insert it to the list and update the statistics
	 * under the lock.
	 */
	if (ret == MIGRATEPAGE_SUCCESS) {
		balloon_page_insert(&b->b_dev_info, newpage);
		__count_vm_event(BALLOON_MIGRATE);
	}

	/*
	 * We deflated successfully, so regardless to the inflation success, we
	 * need to reduce the number of isolated_pages.
	 */
	b->b_dev_info.isolated_pages--;
	spin_unlock_irqrestore(&b->b_dev_info.pages_lock, flags);

out_unlock:
	up_read(&b->conf_sem);
	return ret;
}

/**
 * vmballoon_compaction_init() - initialized compaction for the balloon.
 *
 * @b: pointer to the balloon.
 *
 * If during the initialization a failure occurred, this function does not
 * perform cleanup. The caller must call vmballoon_compaction_deinit() in this
 * case.
 *
 * Return: zero on success or error code on failure.
 */
static __init void vmballoon_compaction_init(struct vmballoon *b)
{
	b->b_dev_info.migratepage = vmballoon_migratepage;
}

#else /* CONFIG_BALLOON_COMPACTION */
static inline void vmballoon_compaction_init(struct vmballoon *b)
{
}
#endif /* CONFIG_BALLOON_COMPACTION */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init vmballoon_init(void)
{
	int error;

	/*
	 * Check if we are running on VMware's hypervisor and bail out
	 * if we are not.
	 */
<<<<<<< HEAD
	if (x86_hyper != &x86_hyper_vmware)
		return -ENODEV;

	INIT_LIST_HEAD(&balloon.pages);
	INIT_LIST_HEAD(&balloon.refused_pages);

	/* initialize rates */
	balloon.rate_alloc = VMW_BALLOON_RATE_ALLOC_MAX;
	balloon.rate_free = VMW_BALLOON_RATE_FREE_MAX;

	INIT_DELAYED_WORK(&balloon.dwork, vmballoon_work);

	/*
	 * Start balloon.
	 */
	if (!vmballoon_send_start(&balloon)) {
		pr_err("failed to send start command to the host\n");
		return -EIO;
	}

	if (!vmballoon_send_guest_id(&balloon)) {
		pr_err("failed to send guest ID to the host\n");
		return -EIO;
	}

	error = vmballoon_debugfs_init(&balloon);
	if (error)
		return error;

	queue_delayed_work(system_freezable_wq, &balloon.dwork, 0);

	return 0;
}
module_init(vmballoon_init);

static void __exit vmballoon_exit(void)
{
=======
	if (x86_hyper_type != X86_HYPER_VMWARE)
		return -ENODEV;

	INIT_DELAYED_WORK(&balloon.dwork, vmballoon_work);

	error = vmballoon_register_shrinker(&balloon);
	if (error)
		return error;

	/*
	 * Initialization of compaction must be done after the call to
	 * balloon_devinfo_init() .
	 */
	balloon_devinfo_init(&balloon.b_dev_info);
	vmballoon_compaction_init(&balloon);

	INIT_LIST_HEAD(&balloon.huge_pages);
	spin_lock_init(&balloon.comm_lock);
	init_rwsem(&balloon.conf_sem);
	balloon.vmci_doorbell = VMCI_INVALID_HANDLE;
	balloon.batch_page = NULL;
	balloon.page = NULL;
	balloon.reset_required = true;

	queue_delayed_work(system_freezable_wq, &balloon.dwork, 0);

	vmballoon_debugfs_init(&balloon);

	return 0;
}

/*
 * Using late_initcall() instead of module_init() allows the balloon to use the
 * VMCI doorbell even when the balloon is built into the kernel. Otherwise the
 * VMCI is probed only after the balloon is initialized. If the balloon is used
 * as a module, late_initcall() is equivalent to module_init().
 */
late_initcall(vmballoon_init);

static void __exit vmballoon_exit(void)
{
	vmballoon_unregister_shrinker(&balloon);
	vmballoon_vmci_cleanup(&balloon);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cancel_delayed_work_sync(&balloon.dwork);

	vmballoon_debugfs_exit(&balloon);

	/*
	 * Deallocate all reserved memory, and reset connection with monitor.
	 * Reset connection before deallocating memory to avoid potential for
	 * additional spurious resets from guest touching deallocated pages.
	 */
<<<<<<< HEAD
	vmballoon_send_start(&balloon);
=======
	vmballoon_send_start(&balloon, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vmballoon_pop(&balloon);
}
module_exit(vmballoon_exit);
