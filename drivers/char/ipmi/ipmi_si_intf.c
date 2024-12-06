<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ipmi_si.c
 *
 * The interface to the IPMI driver for the system interfaces (KCS, SMIC,
 * BT).
 *
 * Author: MontaVista Software, Inc.
 *         Corey Minyard <minyard@mvista.com>
 *         source@mvista.com
 *
 * Copyright 2002 MontaVista Software Inc.
 * Copyright 2006 IBM Corp., Christian Krafft <krafft@de.ibm.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * This file holds the "policy" for the interface to the SMI state
 * machine.  It does the configuration, handles timers and interrupts,
 * and drives the real SMI state machine.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "ipmi_si: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/pci.h>
#include <linux/ioport.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <linux/rcupdate.h>
#include <linux/ipmi.h>
#include <linux/ipmi_smi.h>
<<<<<<< HEAD
#include <asm/io.h>
#include "ipmi_si_sm.h"
#include <linux/init.h>
#include <linux/dmi.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/pnp.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#define PFX "ipmi_si: "
=======
#include "ipmi_si.h"
#include "ipmi_si_sm.h"
#include <linux/string.h>
#include <linux/ctype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Measure times between events in the driver. */
#undef DEBUG_TIMING

/* Call every 10 ms. */
#define SI_TIMEOUT_TIME_USEC	10000
#define SI_USEC_PER_JIFFY	(1000000/HZ)
#define SI_TIMEOUT_JIFFIES	(SI_TIMEOUT_TIME_USEC/SI_USEC_PER_JIFFY)
#define SI_SHORT_TIMEOUT_USEC  250 /* .25ms when the SM request a
				      short timeout */

enum si_intf_state {
	SI_NORMAL,
	SI_GETTING_FLAGS,
	SI_GETTING_EVENTS,
	SI_CLEARING_FLAGS,
<<<<<<< HEAD
	SI_CLEARING_FLAGS_THEN_SET_IRQ,
	SI_GETTING_MESSAGES,
	SI_ENABLE_INTERRUPTS1,
	SI_ENABLE_INTERRUPTS2,
	SI_DISABLE_INTERRUPTS1,
	SI_DISABLE_INTERRUPTS2
=======
	SI_GETTING_MESSAGES,
	SI_CHECKING_ENABLES,
	SI_SETTING_ENABLES
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* FIXME - add watchdog stuff. */
};

/* Some BT-specific defines we need here. */
#define IPMI_BT_INTMASK_REG		2
#define IPMI_BT_INTMASK_CLEAR_IRQ_BIT	2
#define IPMI_BT_INTMASK_ENABLE_IRQ_BIT	1

<<<<<<< HEAD
enum si_type {
    SI_KCS, SI_SMIC, SI_BT
};
static char *si_to_str[] = { "kcs", "smic", "bt" };

static char *ipmi_addr_src_to_str[] = { NULL, "hotmod", "hardcoded", "SPMI",
					"ACPI", "SMBIOS", "PCI",
					"device-tree", "default" };

#define DEVICE_NAME "ipmi_si"

static struct platform_driver ipmi_driver;
=======
/* 'invalid' to allow a firmware-specified interface to be disabled */
const char *const si_to_str[] = { "invalid", "kcs", "smic", "bt", NULL };

static bool initialized;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Indexes into stats[] in smi_info below.
 */
enum si_stat_indexes {
	/*
	 * Number of times the driver requested a timer while an operation
	 * was in progress.
	 */
	SI_STAT_short_timeouts = 0,

	/*
	 * Number of times the driver requested a timer while nothing was in
	 * progress.
	 */
	SI_STAT_long_timeouts,

	/* Number of times the interface was idle while being polled. */
	SI_STAT_idles,

	/* Number of interrupts the driver handled. */
	SI_STAT_interrupts,

	/* Number of time the driver got an ATTN from the hardware. */
	SI_STAT_attentions,

	/* Number of times the driver requested flags from the hardware. */
	SI_STAT_flag_fetches,

	/* Number of times the hardware didn't follow the state machine. */
	SI_STAT_hosed_count,

	/* Number of completed messages. */
	SI_STAT_complete_transactions,

	/* Number of IPMI events received from the hardware. */
	SI_STAT_events,

	/* Number of watchdog pretimeouts. */
	SI_STAT_watchdog_pretimeouts,

<<<<<<< HEAD
	/* Number of asyncronous messages received. */
=======
	/* Number of asynchronous messages received. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SI_STAT_incoming_messages,


	/* This *must* remain last, add new values above this. */
	SI_NUM_STATS
};

struct smi_info {
<<<<<<< HEAD
	int                    intf_num;
	ipmi_smi_t             intf;
	struct si_sm_data      *si_sm;
	struct si_sm_handlers  *handlers;
	enum si_type           si_type;
	spinlock_t             si_lock;
	struct list_head       xmit_msgs;
	struct list_head       hp_xmit_msgs;
=======
	int                    si_num;
	struct ipmi_smi        *intf;
	struct si_sm_data      *si_sm;
	const struct si_sm_handlers *handlers;
	spinlock_t             si_lock;
	struct ipmi_smi_msg    *waiting_msg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ipmi_smi_msg    *curr_msg;
	enum si_intf_state     si_state;

	/*
	 * Used to handle the various types of I/O that can occur with
	 * IPMI
	 */
	struct si_sm_io io;
<<<<<<< HEAD
	int (*io_setup)(struct smi_info *info);
	void (*io_cleanup)(struct smi_info *info);
	int (*irq_setup)(struct smi_info *info);
	void (*irq_cleanup)(struct smi_info *info);
	unsigned int io_size;
	enum ipmi_addr_src addr_source; /* ACPI, PCI, SMBIOS, hardcode, etc. */
	void (*addr_source_cleanup)(struct smi_info *info);
	void *addr_source_data;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Per-OEM handler, called from handle_flags().  Returns 1
	 * when handle_flags() needs to be re-run or 0 indicating it
	 * set si_state itself.
	 */
	int (*oem_data_avail_handler)(struct smi_info *smi_info);

	/*
	 * Flags from the last GET_MSG_FLAGS command, used when an ATTN
	 * is set to hold the flags until we are done handling everything
	 * from the flags.
	 */
#define RECEIVE_MSG_AVAIL	0x01
#define EVENT_MSG_BUFFER_FULL	0x02
#define WDT_PRE_TIMEOUT_INT	0x08
#define OEM0_DATA_AVAIL     0x20
#define OEM1_DATA_AVAIL     0x40
#define OEM2_DATA_AVAIL     0x80
#define OEM_DATA_AVAIL      (OEM0_DATA_AVAIL | \
			     OEM1_DATA_AVAIL | \
			     OEM2_DATA_AVAIL)
	unsigned char       msg_flags;

	/* Does the BMC have an event buffer? */
<<<<<<< HEAD
	char		    has_event_buffer;
=======
	bool		    has_event_buffer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If set to true, this will request events the next time the
	 * state machine is idle.
	 */
	atomic_t            req_events;

	/*
	 * If true, run the state machine to completion on every send
	 * call.  Generally used after a panic to make sure stuff goes
	 * out.
	 */
<<<<<<< HEAD
	int                 run_to_completion;

	/* The I/O port of an SI interface. */
	int                 port;

	/*
	 * The space between start addresses of the two ports.  For
	 * instance, if the first port is 0xca2 and the spacing is 4, then
	 * the second port is 0xca6.
	 */
	unsigned int        spacing;

	/* zero if no irq; */
	int                 irq;
=======
	bool                run_to_completion;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The timer for this si. */
	struct timer_list   si_timer;

<<<<<<< HEAD
=======
	/* This flag is set, if the timer can be set */
	bool		    timer_can_start;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* This flag is set, if the timer is running (timer_pending() isn't enough) */
	bool		    timer_running;

	/* The time (in jiffies) the last timeout occurred at. */
	unsigned long       last_timeout_jiffies;

<<<<<<< HEAD
	/* Used to gracefully stop the timer without race conditions. */
	atomic_t            stop_operation;
=======
	/* Are we waiting for the events, pretimeouts, received msgs? */
	atomic_t            need_watch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The driver will disable interrupts when it gets into a
	 * situation where it cannot handle messages due to lack of
	 * memory.  Once that situation clears up, it will re-enable
	 * interrupts.
	 */
<<<<<<< HEAD
	int interrupt_disabled;
=======
	bool interrupt_disabled;

	/*
	 * Does the BMC support events?
	 */
	bool supports_event_msg_buff;

	/*
	 * Can we disable interrupts the global enables receive irq
	 * bit?  There are currently two forms of brokenness, some
	 * systems cannot disable the bit (which is technically within
	 * the spec but a bad idea) and some systems have the bit
	 * forced to zero even though interrupts work (which is
	 * clearly outside the spec).  The next bool tells which form
	 * of brokenness is present.
	 */
	bool cannot_disable_irq;

	/*
	 * Some systems are broken and cannot set the irq enable
	 * bit, even if they support interrupts.
	 */
	bool irq_enable_broken;

	/* Is the driver in maintenance mode? */
	bool in_maintenance_mode;

	/*
	 * Did we get an attention that we did not handle?
	 */
	bool got_attn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* From the get device id response... */
	struct ipmi_device_id device_id;

<<<<<<< HEAD
	/* Driver model stuff. */
	struct device *dev;
	struct platform_device *pdev;

	/*
	 * True if we allocated the device, false if it came from
	 * someplace else (like PCI).
	 */
	int dev_registered;

	/* Slave address, could be reported from DMI. */
	unsigned char slave_addr;
=======
	/* Have we added the device group to the device? */
	bool dev_group_added;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Counters and things for the proc filesystem. */
	atomic_t stats[SI_NUM_STATS];

	struct task_struct *thread;

	struct list_head link;
<<<<<<< HEAD
	union ipmi_smi_info_union addr_info;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define smi_inc_stat(smi, stat) \
	atomic_inc(&(smi)->stats[SI_STAT_ ## stat])
#define smi_get_stat(smi, stat) \
	((unsigned int) atomic_read(&(smi)->stats[SI_STAT_ ## stat]))

<<<<<<< HEAD
#define SI_MAX_PARMS 4

static int force_kipmid[SI_MAX_PARMS];
static int num_force_kipmid;
#ifdef CONFIG_PCI
static int pci_registered;
#endif
#ifdef CONFIG_ACPI
static int pnp_registered;
#endif

static unsigned int kipmid_max_busy_us[SI_MAX_PARMS];
static int num_max_busy_us;

static int unload_when_empty = 1;

static int add_smi(struct smi_info *smi);
static int try_smi_init(struct smi_info *smi);
static void cleanup_one_si(struct smi_info *to_clean);
static void cleanup_ipmi_si(void);

=======
#define IPMI_MAX_INTFS 4
static int force_kipmid[IPMI_MAX_INTFS];
static int num_force_kipmid;

static unsigned int kipmid_max_busy_us[IPMI_MAX_INTFS];
static int num_max_busy_us;

static bool unload_when_empty = true;

static int try_smi_init(struct smi_info *smi);
static void cleanup_one_si(struct smi_info *smi_info);
static void cleanup_ipmi_si(void);

#ifdef DEBUG_TIMING
void debug_timestamp(struct smi_info *smi_info, char *msg)
{
	struct timespec64 t;

	ktime_get_ts64(&t);
	dev_dbg(smi_info->io.dev, "**%s: %lld.%9.9ld\n",
		msg, t.tv_sec, t.tv_nsec);
}
#else
#define debug_timestamp(smi_info, x)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ATOMIC_NOTIFIER_HEAD(xaction_notifier_list);
static int register_xaction_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&xaction_notifier_list, nb);
}

static void deliver_recv_msg(struct smi_info *smi_info,
			     struct ipmi_smi_msg *msg)
{
	/* Deliver the message to the upper layer. */
	ipmi_smi_msg_received(smi_info->intf, msg);
}

static void return_hosed_msg(struct smi_info *smi_info, int cCode)
{
	struct ipmi_smi_msg *msg = smi_info->curr_msg;

	if (cCode < 0 || cCode > IPMI_ERR_UNSPECIFIED)
		cCode = IPMI_ERR_UNSPECIFIED;
	/* else use it as is */

	/* Make it a response */
	msg->rsp[0] = msg->data[0] | 4;
	msg->rsp[1] = msg->data[1];
	msg->rsp[2] = cCode;
	msg->rsp_size = 3;

	smi_info->curr_msg = NULL;
	deliver_recv_msg(smi_info, msg);
}

static enum si_sm_result start_next_msg(struct smi_info *smi_info)
{
	int              rv;
<<<<<<< HEAD
	struct list_head *entry = NULL;
#ifdef DEBUG_TIMING
	struct timeval t;
#endif

	/* Pick the high priority queue first. */
	if (!list_empty(&(smi_info->hp_xmit_msgs))) {
		entry = smi_info->hp_xmit_msgs.next;
	} else if (!list_empty(&(smi_info->xmit_msgs))) {
		entry = smi_info->xmit_msgs.next;
	}

	if (!entry) {
=======

	if (!smi_info->waiting_msg) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		smi_info->curr_msg = NULL;
		rv = SI_SM_IDLE;
	} else {
		int err;

<<<<<<< HEAD
		list_del(entry);
		smi_info->curr_msg = list_entry(entry,
						struct ipmi_smi_msg,
						link);
#ifdef DEBUG_TIMING
		do_gettimeofday(&t);
		printk(KERN_DEBUG "**Start2: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif
=======
		smi_info->curr_msg = smi_info->waiting_msg;
		smi_info->waiting_msg = NULL;
		debug_timestamp(smi_info, "Start2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = atomic_notifier_call_chain(&xaction_notifier_list,
				0, smi_info);
		if (err & NOTIFY_STOP_MASK) {
			rv = SI_SM_CALL_WITHOUT_DELAY;
			goto out;
		}
		err = smi_info->handlers->start_transaction(
			smi_info->si_sm,
			smi_info->curr_msg->data,
			smi_info->curr_msg->data_size);
		if (err)
			return_hosed_msg(smi_info, err);

		rv = SI_SM_CALL_WITHOUT_DELAY;
	}
<<<<<<< HEAD
 out:
	return rv;
}

static void start_enable_irq(struct smi_info *smi_info)
{
	unsigned char msg[2];

	/*
	 * If we are enabling interrupts, we have to tell the
	 * BMC to use them.
	 */
	msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
	msg[1] = IPMI_GET_BMC_GLOBAL_ENABLES_CMD;

	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 2);
	smi_info->si_state = SI_ENABLE_INTERRUPTS1;
}

static void start_disable_irq(struct smi_info *smi_info)
=======
out:
	return rv;
}

static void smi_mod_timer(struct smi_info *smi_info, unsigned long new_val)
{
	if (!smi_info->timer_can_start)
		return;
	smi_info->last_timeout_jiffies = jiffies;
	mod_timer(&smi_info->si_timer, new_val);
	smi_info->timer_running = true;
}

/*
 * Start a new message and (re)start the timer and thread.
 */
static void start_new_msg(struct smi_info *smi_info, unsigned char *msg,
			  unsigned int size)
{
	smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

	if (smi_info->thread)
		wake_up_process(smi_info->thread);

	smi_info->handlers->start_transaction(smi_info->si_sm, msg, size);
}

static void start_check_enables(struct smi_info *smi_info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned char msg[2];

	msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
	msg[1] = IPMI_GET_BMC_GLOBAL_ENABLES_CMD;

<<<<<<< HEAD
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 2);
	smi_info->si_state = SI_DISABLE_INTERRUPTS1;
=======
	start_new_msg(smi_info, msg, 2);
	smi_info->si_state = SI_CHECKING_ENABLES;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void start_clear_flags(struct smi_info *smi_info)
{
	unsigned char msg[3];

	/* Make sure the watchdog pre-timeout flag is not set at startup. */
	msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
	msg[1] = IPMI_CLEAR_MSG_FLAGS_CMD;
	msg[2] = WDT_PRE_TIMEOUT_INT;

<<<<<<< HEAD
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 3);
	smi_info->si_state = SI_CLEARING_FLAGS;
}

static void smi_mod_timer(struct smi_info *smi_info, unsigned long new_val)
{
	smi_info->last_timeout_jiffies = jiffies;
	mod_timer(&smi_info->si_timer, new_val);
	smi_info->timer_running = true;
=======
	start_new_msg(smi_info, msg, 3);
	smi_info->si_state = SI_CLEARING_FLAGS;
}

static void start_getting_msg_queue(struct smi_info *smi_info)
{
	smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
	smi_info->curr_msg->data[1] = IPMI_GET_MSG_CMD;
	smi_info->curr_msg->data_size = 2;

	start_new_msg(smi_info, smi_info->curr_msg->data,
		      smi_info->curr_msg->data_size);
	smi_info->si_state = SI_GETTING_MESSAGES;
}

static void start_getting_events(struct smi_info *smi_info)
{
	smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
	smi_info->curr_msg->data[1] = IPMI_READ_EVENT_MSG_BUFFER_CMD;
	smi_info->curr_msg->data_size = 2;

	start_new_msg(smi_info, smi_info->curr_msg->data,
		      smi_info->curr_msg->data_size);
	smi_info->si_state = SI_GETTING_EVENTS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * When we have a situtaion where we run out of memory and cannot
 * allocate messages, we just leave them in the BMC and run the system
 * polled until we can allocate some memory.  Once we have some
 * memory, we will re-enable the interrupt.
<<<<<<< HEAD
 */
static inline void disable_si_irq(struct smi_info *smi_info)
{
	if ((smi_info->irq) && (!smi_info->interrupt_disabled)) {
		start_disable_irq(smi_info);
		smi_info->interrupt_disabled = 1;
		if (!atomic_read(&smi_info->stop_operation))
			smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);
	}
}

static inline void enable_si_irq(struct smi_info *smi_info)
{
	if ((smi_info->irq) && (smi_info->interrupt_disabled)) {
		start_enable_irq(smi_info);
		smi_info->interrupt_disabled = 0;
	}
=======
 *
 * Note that we cannot just use disable_irq(), since the interrupt may
 * be shared.
 */
static inline bool disable_si_irq(struct smi_info *smi_info)
{
	if ((smi_info->io.irq) && (!smi_info->interrupt_disabled)) {
		smi_info->interrupt_disabled = true;
		start_check_enables(smi_info);
		return true;
	}
	return false;
}

static inline bool enable_si_irq(struct smi_info *smi_info)
{
	if ((smi_info->io.irq) && (smi_info->interrupt_disabled)) {
		smi_info->interrupt_disabled = false;
		start_check_enables(smi_info);
		return true;
	}
	return false;
}

/*
 * Allocate a message.  If unable to allocate, start the interrupt
 * disable process and return NULL.  If able to allocate but
 * interrupts are disabled, free the message and return NULL after
 * starting the interrupt enable process.
 */
static struct ipmi_smi_msg *alloc_msg_handle_irq(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg;

	msg = ipmi_alloc_smi_msg();
	if (!msg) {
		if (!disable_si_irq(smi_info))
			smi_info->si_state = SI_NORMAL;
	} else if (enable_si_irq(smi_info)) {
		ipmi_free_smi_msg(msg);
		msg = NULL;
	}
	return msg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void handle_flags(struct smi_info *smi_info)
{
<<<<<<< HEAD
 retry:
=======
retry:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (smi_info->msg_flags & WDT_PRE_TIMEOUT_INT) {
		/* Watchdog pre-timeout */
		smi_inc_stat(smi_info, watchdog_pretimeouts);

		start_clear_flags(smi_info);
		smi_info->msg_flags &= ~WDT_PRE_TIMEOUT_INT;
		ipmi_smi_watchdog_pretimeout(smi_info->intf);
	} else if (smi_info->msg_flags & RECEIVE_MSG_AVAIL) {
		/* Messages available. */
<<<<<<< HEAD
		smi_info->curr_msg = ipmi_alloc_smi_msg();
		if (!smi_info->curr_msg) {
			disable_si_irq(smi_info);
			smi_info->si_state = SI_NORMAL;
			return;
		}
		enable_si_irq(smi_info);

		smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
		smi_info->curr_msg->data[1] = IPMI_GET_MSG_CMD;
		smi_info->curr_msg->data_size = 2;

		smi_info->handlers->start_transaction(
			smi_info->si_sm,
			smi_info->curr_msg->data,
			smi_info->curr_msg->data_size);
		smi_info->si_state = SI_GETTING_MESSAGES;
	} else if (smi_info->msg_flags & EVENT_MSG_BUFFER_FULL) {
		/* Events available. */
		smi_info->curr_msg = ipmi_alloc_smi_msg();
		if (!smi_info->curr_msg) {
			disable_si_irq(smi_info);
			smi_info->si_state = SI_NORMAL;
			return;
		}
		enable_si_irq(smi_info);

		smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
		smi_info->curr_msg->data[1] = IPMI_READ_EVENT_MSG_BUFFER_CMD;
		smi_info->curr_msg->data_size = 2;

		smi_info->handlers->start_transaction(
			smi_info->si_sm,
			smi_info->curr_msg->data,
			smi_info->curr_msg->data_size);
		smi_info->si_state = SI_GETTING_EVENTS;
=======
		smi_info->curr_msg = alloc_msg_handle_irq(smi_info);
		if (!smi_info->curr_msg)
			return;

		start_getting_msg_queue(smi_info);
	} else if (smi_info->msg_flags & EVENT_MSG_BUFFER_FULL) {
		/* Events available. */
		smi_info->curr_msg = alloc_msg_handle_irq(smi_info);
		if (!smi_info->curr_msg)
			return;

		start_getting_events(smi_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (smi_info->msg_flags & OEM_DATA_AVAIL &&
		   smi_info->oem_data_avail_handler) {
		if (smi_info->oem_data_avail_handler(smi_info))
			goto retry;
	} else
		smi_info->si_state = SI_NORMAL;
}

<<<<<<< HEAD
static void handle_transaction_done(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg;
#ifdef DEBUG_TIMING
	struct timeval t;

	do_gettimeofday(&t);
	printk(KERN_DEBUG "**Done: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif
=======
/*
 * Global enables we care about.
 */
#define GLOBAL_ENABLES_MASK (IPMI_BMC_EVT_MSG_BUFF | IPMI_BMC_RCV_MSG_INTR | \
			     IPMI_BMC_EVT_MSG_INTR)

static u8 current_global_enables(struct smi_info *smi_info, u8 base,
				 bool *irq_on)
{
	u8 enables = 0;

	if (smi_info->supports_event_msg_buff)
		enables |= IPMI_BMC_EVT_MSG_BUFF;

	if (((smi_info->io.irq && !smi_info->interrupt_disabled) ||
	     smi_info->cannot_disable_irq) &&
	    !smi_info->irq_enable_broken)
		enables |= IPMI_BMC_RCV_MSG_INTR;

	if (smi_info->supports_event_msg_buff &&
	    smi_info->io.irq && !smi_info->interrupt_disabled &&
	    !smi_info->irq_enable_broken)
		enables |= IPMI_BMC_EVT_MSG_INTR;

	*irq_on = enables & (IPMI_BMC_EVT_MSG_INTR | IPMI_BMC_RCV_MSG_INTR);

	return enables;
}

static void check_bt_irq(struct smi_info *smi_info, bool irq_on)
{
	u8 irqstate = smi_info->io.inputb(&smi_info->io, IPMI_BT_INTMASK_REG);

	irqstate &= IPMI_BT_INTMASK_ENABLE_IRQ_BIT;

	if ((bool)irqstate == irq_on)
		return;

	if (irq_on)
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG,
				     IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
	else
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG, 0);
}

static void handle_transaction_done(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg;

	debug_timestamp(smi_info, "Done");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (smi_info->si_state) {
	case SI_NORMAL:
		if (!smi_info->curr_msg)
			break;

		smi_info->curr_msg->rsp_size
			= smi_info->handlers->get_result(
				smi_info->si_sm,
				smi_info->curr_msg->rsp,
				IPMI_MAX_MSG_LENGTH);

		/*
		 * Do this here becase deliver_recv_msg() releases the
		 * lock, and a new message can be put in during the
		 * time the lock is released.
		 */
		msg = smi_info->curr_msg;
		smi_info->curr_msg = NULL;
		deliver_recv_msg(smi_info, msg);
		break;

	case SI_GETTING_FLAGS:
	{
		unsigned char msg[4];
		unsigned int  len;

		/* We got the flags from the SMI, now handle them. */
		len = smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0) {
			/* Error fetching flags, just give up for now. */
			smi_info->si_state = SI_NORMAL;
		} else if (len < 4) {
			/*
			 * Hmm, no flags.  That's technically illegal, but
			 * don't use uninitialized data.
			 */
			smi_info->si_state = SI_NORMAL;
		} else {
			smi_info->msg_flags = msg[3];
			handle_flags(smi_info);
		}
		break;
	}

	case SI_CLEARING_FLAGS:
<<<<<<< HEAD
	case SI_CLEARING_FLAGS_THEN_SET_IRQ:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		unsigned char msg[3];

		/* We cleared the flags. */
		smi_info->handlers->get_result(smi_info->si_sm, msg, 3);
		if (msg[2] != 0) {
			/* Error clearing flags */
<<<<<<< HEAD
			dev_warn(smi_info->dev,
				 "Error clearing flags: %2.2x\n", msg[2]);
		}
		if (smi_info->si_state == SI_CLEARING_FLAGS_THEN_SET_IRQ)
			start_enable_irq(smi_info);
		else
			smi_info->si_state = SI_NORMAL;
=======
			dev_warn_ratelimited(smi_info->io.dev,
				 "Error clearing flags: %2.2x\n", msg[2]);
		}
		smi_info->si_state = SI_NORMAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	case SI_GETTING_EVENTS:
	{
		smi_info->curr_msg->rsp_size
			= smi_info->handlers->get_result(
				smi_info->si_sm,
				smi_info->curr_msg->rsp,
				IPMI_MAX_MSG_LENGTH);

		/*
		 * Do this here becase deliver_recv_msg() releases the
		 * lock, and a new message can be put in during the
		 * time the lock is released.
		 */
		msg = smi_info->curr_msg;
		smi_info->curr_msg = NULL;
		if (msg->rsp[2] != 0) {
			/* Error getting event, probably done. */
			msg->done(msg);

			/* Take off the event flag. */
			smi_info->msg_flags &= ~EVENT_MSG_BUFFER_FULL;
			handle_flags(smi_info);
		} else {
			smi_inc_stat(smi_info, events);

			/*
			 * Do this before we deliver the message
			 * because delivering the message releases the
			 * lock and something else can mess with the
			 * state.
			 */
			handle_flags(smi_info);

			deliver_recv_msg(smi_info, msg);
		}
		break;
	}

	case SI_GETTING_MESSAGES:
	{
		smi_info->curr_msg->rsp_size
			= smi_info->handlers->get_result(
				smi_info->si_sm,
				smi_info->curr_msg->rsp,
				IPMI_MAX_MSG_LENGTH);

		/*
		 * Do this here becase deliver_recv_msg() releases the
		 * lock, and a new message can be put in during the
		 * time the lock is released.
		 */
		msg = smi_info->curr_msg;
		smi_info->curr_msg = NULL;
		if (msg->rsp[2] != 0) {
			/* Error getting event, probably done. */
			msg->done(msg);

			/* Take off the msg flag. */
			smi_info->msg_flags &= ~RECEIVE_MSG_AVAIL;
			handle_flags(smi_info);
		} else {
			smi_inc_stat(smi_info, incoming_messages);

			/*
			 * Do this before we deliver the message
			 * because delivering the message releases the
			 * lock and something else can mess with the
			 * state.
			 */
			handle_flags(smi_info);

			deliver_recv_msg(smi_info, msg);
		}
		break;
	}

<<<<<<< HEAD
	case SI_ENABLE_INTERRUPTS1:
	{
		unsigned char msg[4];
=======
	case SI_CHECKING_ENABLES:
	{
		unsigned char msg[4];
		u8 enables;
		bool irq_on;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* We got the flags from the SMI, now handle them. */
		smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0) {
<<<<<<< HEAD
			dev_warn(smi_info->dev, "Could not enable interrupts"
				 ", failed get, using polled mode.\n");
			smi_info->si_state = SI_NORMAL;
		} else {
			msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
			msg[1] = IPMI_SET_BMC_GLOBAL_ENABLES_CMD;
			msg[2] = (msg[3] |
				  IPMI_BMC_RCV_MSG_INTR |
				  IPMI_BMC_EVT_MSG_INTR);
			smi_info->handlers->start_transaction(
				smi_info->si_sm, msg, 3);
			smi_info->si_state = SI_ENABLE_INTERRUPTS2;
=======
			dev_warn_ratelimited(smi_info->io.dev,
				"Couldn't get irq info: %x,\n"
				"Maybe ok, but ipmi might run very slowly.\n",
				msg[2]);
			smi_info->si_state = SI_NORMAL;
			break;
		}
		enables = current_global_enables(smi_info, 0, &irq_on);
		if (smi_info->io.si_type == SI_BT)
			/* BT has its own interrupt enable bit. */
			check_bt_irq(smi_info, irq_on);
		if (enables != (msg[3] & GLOBAL_ENABLES_MASK)) {
			/* Enables are not correct, fix them. */
			msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
			msg[1] = IPMI_SET_BMC_GLOBAL_ENABLES_CMD;
			msg[2] = enables | (msg[3] & ~GLOBAL_ENABLES_MASK);
			smi_info->handlers->start_transaction(
				smi_info->si_sm, msg, 3);
			smi_info->si_state = SI_SETTING_ENABLES;
		} else if (smi_info->supports_event_msg_buff) {
			smi_info->curr_msg = ipmi_alloc_smi_msg();
			if (!smi_info->curr_msg) {
				smi_info->si_state = SI_NORMAL;
				break;
			}
			start_getting_events(smi_info);
		} else {
			smi_info->si_state = SI_NORMAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}

<<<<<<< HEAD
	case SI_ENABLE_INTERRUPTS2:
	{
		unsigned char msg[4];

		/* We got the flags from the SMI, now handle them. */
		smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0)
			dev_warn(smi_info->dev, "Could not enable interrupts"
				 ", failed set, using polled mode.\n");
		else
			smi_info->interrupt_disabled = 0;
		smi_info->si_state = SI_NORMAL;
		break;
	}

	case SI_DISABLE_INTERRUPTS1:
	{
		unsigned char msg[4];

		/* We got the flags from the SMI, now handle them. */
		smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0) {
			dev_warn(smi_info->dev, "Could not disable interrupts"
				 ", failed get.\n");
			smi_info->si_state = SI_NORMAL;
		} else {
			msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
			msg[1] = IPMI_SET_BMC_GLOBAL_ENABLES_CMD;
			msg[2] = (msg[3] &
				  ~(IPMI_BMC_RCV_MSG_INTR |
				    IPMI_BMC_EVT_MSG_INTR));
			smi_info->handlers->start_transaction(
				smi_info->si_sm, msg, 3);
			smi_info->si_state = SI_DISABLE_INTERRUPTS2;
		}
		break;
	}

	case SI_DISABLE_INTERRUPTS2:
	{
		unsigned char msg[4];

		/* We got the flags from the SMI, now handle them. */
		smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0) {
			dev_warn(smi_info->dev, "Could not disable interrupts"
				 ", failed set.\n");
		}
		smi_info->si_state = SI_NORMAL;
		break;
	}
=======
	case SI_SETTING_ENABLES:
	{
		unsigned char msg[4];

		smi_info->handlers->get_result(smi_info->si_sm, msg, 4);
		if (msg[2] != 0)
			dev_warn_ratelimited(smi_info->io.dev,
				 "Could not set the global enables: 0x%x.\n",
				 msg[2]);

		if (smi_info->supports_event_msg_buff) {
			smi_info->curr_msg = ipmi_alloc_smi_msg();
			if (!smi_info->curr_msg) {
				smi_info->si_state = SI_NORMAL;
				break;
			}
			start_getting_events(smi_info);
		} else {
			smi_info->si_state = SI_NORMAL;
		}
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Called on timeouts and events.  Timeouts should pass the elapsed
 * time, interrupts should pass in zero.  Must be called with
 * si_lock held and interrupts disabled.
 */
static enum si_sm_result smi_event_handler(struct smi_info *smi_info,
					   int time)
{
	enum si_sm_result si_sm_result;

<<<<<<< HEAD
 restart:
=======
restart:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * There used to be a loop here that waited a little while
	 * (around 25us) before giving up.  That turned out to be
	 * pointless, the minimum delays I was seeing were in the 300us
	 * range, which is far too long to wait in an interrupt.  So
	 * we just run until the state machine tells us something
	 * happened or it needs a delay.
	 */
	si_sm_result = smi_info->handlers->event(smi_info->si_sm, time);
	time = 0;
	while (si_sm_result == SI_SM_CALL_WITHOUT_DELAY)
		si_sm_result = smi_info->handlers->event(smi_info->si_sm, 0);

	if (si_sm_result == SI_SM_TRANSACTION_COMPLETE) {
		smi_inc_stat(smi_info, complete_transactions);

		handle_transaction_done(smi_info);
<<<<<<< HEAD
		si_sm_result = smi_info->handlers->event(smi_info->si_sm, 0);
=======
		goto restart;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (si_sm_result == SI_SM_HOSED) {
		smi_inc_stat(smi_info, hosed_count);

		/*
		 * Do the before return_hosed_msg, because that
		 * releases the lock.
		 */
		smi_info->si_state = SI_NORMAL;
		if (smi_info->curr_msg != NULL) {
			/*
			 * If we were handling a user message, format
			 * a response to send to the upper layer to
			 * tell it about the error.
			 */
			return_hosed_msg(smi_info, IPMI_ERR_UNSPECIFIED);
		}
<<<<<<< HEAD
		si_sm_result = smi_info->handlers->event(smi_info->si_sm, 0);
=======
		goto restart;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * We prefer handling attn over new messages.  But don't do
	 * this if there is not yet an upper layer to handle anything.
	 */
<<<<<<< HEAD
	if (likely(smi_info->intf) && si_sm_result == SI_SM_ATTN) {
		unsigned char msg[2];

		smi_inc_stat(smi_info, attentions);

		/*
		 * Got a attn, send down a get message flags to see
		 * what's causing it.  It would be better to handle
		 * this in the upper layer, but due to the way
		 * interrupts work with the SMI, that's not really
		 * possible.
		 */
		msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
		msg[1] = IPMI_GET_MSG_FLAGS_CMD;

		smi_info->handlers->start_transaction(
			smi_info->si_sm, msg, 2);
		smi_info->si_state = SI_GETTING_FLAGS;
		goto restart;
=======
	if (si_sm_result == SI_SM_ATTN || smi_info->got_attn) {
		unsigned char msg[2];

		if (smi_info->si_state != SI_NORMAL) {
			/*
			 * We got an ATTN, but we are doing something else.
			 * Handle the ATTN later.
			 */
			smi_info->got_attn = true;
		} else {
			smi_info->got_attn = false;
			smi_inc_stat(smi_info, attentions);

			/*
			 * Got a attn, send down a get message flags to see
			 * what's causing it.  It would be better to handle
			 * this in the upper layer, but due to the way
			 * interrupts work with the SMI, that's not really
			 * possible.
			 */
			msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
			msg[1] = IPMI_GET_MSG_FLAGS_CMD;

			start_new_msg(smi_info, msg, 2);
			smi_info->si_state = SI_GETTING_FLAGS;
			goto restart;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* If we are currently idle, try to start the next message. */
	if (si_sm_result == SI_SM_IDLE) {
		smi_inc_stat(smi_info, idles);

		si_sm_result = start_next_msg(smi_info);
		if (si_sm_result != SI_SM_IDLE)
			goto restart;
	}

	if ((si_sm_result == SI_SM_IDLE)
	    && (atomic_read(&smi_info->req_events))) {
		/*
		 * We are idle and the upper layer requested that I fetch
		 * events, so do so.
		 */
		atomic_set(&smi_info->req_events, 0);

<<<<<<< HEAD
		smi_info->curr_msg = ipmi_alloc_smi_msg();
		if (!smi_info->curr_msg)
			goto out;

		smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
		smi_info->curr_msg->data[1] = IPMI_READ_EVENT_MSG_BUFFER_CMD;
		smi_info->curr_msg->data_size = 2;

		smi_info->handlers->start_transaction(
			smi_info->si_sm,
			smi_info->curr_msg->data,
			smi_info->curr_msg->data_size);
		smi_info->si_state = SI_GETTING_EVENTS;
		goto restart;
	}
 out:
	return si_sm_result;
}

static void sender(void                *send_info,
		   struct ipmi_smi_msg *msg,
		   int                 priority)
{
	struct smi_info   *smi_info = send_info;
	enum si_sm_result result;
	unsigned long     flags;
#ifdef DEBUG_TIMING
	struct timeval    t;
#endif

	if (atomic_read(&smi_info->stop_operation)) {
		msg->rsp[0] = msg->data[0] | 4;
		msg->rsp[1] = msg->data[1];
		msg->rsp[2] = IPMI_ERR_UNSPECIFIED;
		msg->rsp_size = 3;
		deliver_recv_msg(smi_info, msg);
		return;
	}

#ifdef DEBUG_TIMING
	do_gettimeofday(&t);
	printk("**Enqueue: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif

	if (smi_info->run_to_completion) {
		/*
		 * If we are running to completion, then throw it in
		 * the list and run transactions until everything is
		 * clear.  Priority doesn't matter here.
		 */

		/*
		 * Run to completion means we are single-threaded, no
		 * need for locks.
		 */
		list_add_tail(&(msg->link), &(smi_info->xmit_msgs));

		result = smi_event_handler(smi_info, 0);
		while (result != SI_SM_IDLE) {
			udelay(SI_SHORT_TIMEOUT_USEC);
			result = smi_event_handler(smi_info,
						   SI_SHORT_TIMEOUT_USEC);
		}
		return;
	}

	spin_lock_irqsave(&smi_info->si_lock, flags);
	if (priority > 0)
		list_add_tail(&msg->link, &smi_info->hp_xmit_msgs);
	else
		list_add_tail(&msg->link, &smi_info->xmit_msgs);

=======
		/*
		 * Take this opportunity to check the interrupt and
		 * message enable state for the BMC.  The BMC can be
		 * asynchronously reset, and may thus get interrupts
		 * disable and messages disabled.
		 */
		if (smi_info->supports_event_msg_buff || smi_info->io.irq) {
			start_check_enables(smi_info);
		} else {
			smi_info->curr_msg = alloc_msg_handle_irq(smi_info);
			if (!smi_info->curr_msg)
				goto out;

			start_getting_events(smi_info);
		}
		goto restart;
	}

	if (si_sm_result == SI_SM_IDLE && smi_info->timer_running) {
		/* Ok it if fails, the timer will just go off. */
		if (del_timer(&smi_info->si_timer))
			smi_info->timer_running = false;
	}

out:
	return si_sm_result;
}

static void check_start_timer_thread(struct smi_info *smi_info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (smi_info->si_state == SI_NORMAL && smi_info->curr_msg == NULL) {
		smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

		if (smi_info->thread)
			wake_up_process(smi_info->thread);

		start_next_msg(smi_info);
		smi_event_handler(smi_info, 0);
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&smi_info->si_lock, flags);
}

static void set_run_to_completion(void *send_info, int i_run_to_completion)
{
	struct smi_info   *smi_info = send_info;
	enum si_sm_result result;

	smi_info->run_to_completion = i_run_to_completion;
	if (i_run_to_completion) {
		result = smi_event_handler(smi_info, 0);
		while (result != SI_SM_IDLE) {
			udelay(SI_SHORT_TIMEOUT_USEC);
			result = smi_event_handler(smi_info,
						   SI_SHORT_TIMEOUT_USEC);
		}
	}
}

/*
 * Use -1 in the nsec value of the busy waiting timespec to tell that
 * we are spinning in kipmid looking for something and not delaying
 * between checks
 */
static inline void ipmi_si_set_not_busy(struct timespec *ts)
{
	ts->tv_nsec = -1;
}
static inline int ipmi_si_is_busy(struct timespec *ts)
{
	return ts->tv_nsec != -1;
}

static int ipmi_thread_busy_wait(enum si_sm_result smi_result,
				 const struct smi_info *smi_info,
				 struct timespec *busy_until)
{
	unsigned int max_busy_us = 0;

	if (smi_info->intf_num < num_max_busy_us)
		max_busy_us = kipmid_max_busy_us[smi_info->intf_num];
	if (max_busy_us == 0 || smi_result != SI_SM_CALL_WITH_DELAY)
		ipmi_si_set_not_busy(busy_until);
	else if (!ipmi_si_is_busy(busy_until)) {
		getnstimeofday(busy_until);
		timespec_add_ns(busy_until, max_busy_us*NSEC_PER_USEC);
	} else {
		struct timespec now;
		getnstimeofday(&now);
		if (unlikely(timespec_compare(&now, busy_until) > 0)) {
			ipmi_si_set_not_busy(busy_until);
			return 0;
		}
	}
	return 1;
=======
}

static void flush_messages(void *send_info)
{
	struct smi_info *smi_info = send_info;
	enum si_sm_result result;

	/*
	 * Currently, this function is called only in run-to-completion
	 * mode.  This means we are single-threaded, no need for locks.
	 */
	result = smi_event_handler(smi_info, 0);
	while (result != SI_SM_IDLE) {
		udelay(SI_SHORT_TIMEOUT_USEC);
		result = smi_event_handler(smi_info, SI_SHORT_TIMEOUT_USEC);
	}
}

static void sender(void                *send_info,
		   struct ipmi_smi_msg *msg)
{
	struct smi_info   *smi_info = send_info;
	unsigned long     flags;

	debug_timestamp(smi_info, "Enqueue");

	if (smi_info->run_to_completion) {
		/*
		 * If we are running to completion, start it.  Upper
		 * layer will call flush_messages to clear it out.
		 */
		smi_info->waiting_msg = msg;
		return;
	}

	spin_lock_irqsave(&smi_info->si_lock, flags);
	/*
	 * The following two lines don't need to be under the lock for
	 * the lock's sake, but they do need SMP memory barriers to
	 * avoid getting things out of order.  We are already claiming
	 * the lock, anyway, so just do it under the lock to avoid the
	 * ordering problem.
	 */
	BUG_ON(smi_info->waiting_msg);
	smi_info->waiting_msg = msg;
	check_start_timer_thread(smi_info);
	spin_unlock_irqrestore(&smi_info->si_lock, flags);
}

static void set_run_to_completion(void *send_info, bool i_run_to_completion)
{
	struct smi_info   *smi_info = send_info;

	smi_info->run_to_completion = i_run_to_completion;
	if (i_run_to_completion)
		flush_messages(smi_info);
}

/*
 * Use -1 as a special constant to tell that we are spinning in kipmid
 * looking for something and not delaying between checks
 */
#define IPMI_TIME_NOT_BUSY ns_to_ktime(-1ull)
static inline bool ipmi_thread_busy_wait(enum si_sm_result smi_result,
					 const struct smi_info *smi_info,
					 ktime_t *busy_until)
{
	unsigned int max_busy_us = 0;

	if (smi_info->si_num < num_max_busy_us)
		max_busy_us = kipmid_max_busy_us[smi_info->si_num];
	if (max_busy_us == 0 || smi_result != SI_SM_CALL_WITH_DELAY)
		*busy_until = IPMI_TIME_NOT_BUSY;
	else if (*busy_until == IPMI_TIME_NOT_BUSY) {
		*busy_until = ktime_get() + max_busy_us * NSEC_PER_USEC;
	} else {
		if (unlikely(ktime_get() > *busy_until)) {
			*busy_until = IPMI_TIME_NOT_BUSY;
			return false;
		}
	}
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
 * A busy-waiting loop for speeding up IPMI operation.
 *
 * Lousy hardware makes this hard.  This is only enabled for systems
 * that are not BT and do not have interrupts.  It starts spinning
 * when an operation is complete or until max_busy tells it to stop
 * (if that is enabled).  See the paragraph on kimid_max_busy_us in
<<<<<<< HEAD
 * Documentation/IPMI.txt for details.
=======
 * Documentation/driver-api/ipmi.rst for details.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int ipmi_thread(void *data)
{
	struct smi_info *smi_info = data;
	unsigned long flags;
	enum si_sm_result smi_result;
<<<<<<< HEAD
	struct timespec busy_until;

	ipmi_si_set_not_busy(&busy_until);
	set_user_nice(current, 19);
=======
	ktime_t busy_until = IPMI_TIME_NOT_BUSY;

	set_user_nice(current, MAX_NICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (!kthread_should_stop()) {
		int busy_wait;

		spin_lock_irqsave(&(smi_info->si_lock), flags);
		smi_result = smi_event_handler(smi_info, 0);

		/*
		 * If the driver is doing something, there is a possible
		 * race with the timer.  If the timer handler see idle,
		 * and the thread here sees something else, the timer
		 * handler won't restart the timer even though it is
		 * required.  So start it here if necessary.
		 */
		if (smi_result != SI_SM_IDLE && !smi_info->timer_running)
			smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

		spin_unlock_irqrestore(&(smi_info->si_lock), flags);
		busy_wait = ipmi_thread_busy_wait(smi_result, smi_info,
						  &busy_until);
<<<<<<< HEAD
		if (smi_result == SI_SM_CALL_WITHOUT_DELAY)
			; /* do nothing */
		else if (smi_result == SI_SM_CALL_WITH_DELAY && busy_wait)
			schedule();
		else if (smi_result == SI_SM_IDLE)
			schedule_timeout_interruptible(100);
		else
			schedule_timeout_interruptible(1);
=======
		if (smi_result == SI_SM_CALL_WITHOUT_DELAY) {
			; /* do nothing */
		} else if (smi_result == SI_SM_CALL_WITH_DELAY && busy_wait) {
			/*
			 * In maintenance mode we run as fast as
			 * possible to allow firmware updates to
			 * complete as fast as possible, but normally
			 * don't bang on the scheduler.
			 */
			if (smi_info->in_maintenance_mode)
				schedule();
			else
				usleep_range(100, 200);
		} else if (smi_result == SI_SM_IDLE) {
			if (atomic_read(&smi_info->need_watch)) {
				schedule_timeout_interruptible(100);
			} else {
				/* Wait to be woken up when we are needed. */
				__set_current_state(TASK_INTERRUPTIBLE);
				schedule();
			}
		} else {
			schedule_timeout_interruptible(1);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}


static void poll(void *send_info)
{
	struct smi_info *smi_info = send_info;
	unsigned long flags = 0;
<<<<<<< HEAD
	int run_to_completion = smi_info->run_to_completion;
=======
	bool run_to_completion = smi_info->run_to_completion;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Make sure there is some delay in the poll loop so we can
	 * drive time forward and timeout things.
	 */
	udelay(10);
	if (!run_to_completion)
		spin_lock_irqsave(&smi_info->si_lock, flags);
	smi_event_handler(smi_info, 10);
	if (!run_to_completion)
		spin_unlock_irqrestore(&smi_info->si_lock, flags);
}

static void request_events(void *send_info)
{
	struct smi_info *smi_info = send_info;

<<<<<<< HEAD
	if (atomic_read(&smi_info->stop_operation) ||
				!smi_info->has_event_buffer)
=======
	if (!smi_info->has_event_buffer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	atomic_set(&smi_info->req_events, 1);
}

<<<<<<< HEAD
static int initialized;

static void smi_timeout(unsigned long data)
{
	struct smi_info   *smi_info = (struct smi_info *) data;
=======
static void set_need_watch(void *send_info, unsigned int watch_mask)
{
	struct smi_info *smi_info = send_info;
	unsigned long flags;
	int enable;

	enable = !!watch_mask;

	atomic_set(&smi_info->need_watch, enable);
	spin_lock_irqsave(&smi_info->si_lock, flags);
	check_start_timer_thread(smi_info);
	spin_unlock_irqrestore(&smi_info->si_lock, flags);
}

static void smi_timeout(struct timer_list *t)
{
	struct smi_info   *smi_info = from_timer(smi_info, t, si_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum si_sm_result smi_result;
	unsigned long     flags;
	unsigned long     jiffies_now;
	long              time_diff;
	long		  timeout;
<<<<<<< HEAD
#ifdef DEBUG_TIMING
	struct timeval    t;
#endif

	spin_lock_irqsave(&(smi_info->si_lock), flags);
#ifdef DEBUG_TIMING
	do_gettimeofday(&t);
	printk(KERN_DEBUG "**Timer: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif
=======

	spin_lock_irqsave(&(smi_info->si_lock), flags);
	debug_timestamp(smi_info, "Timer");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	jiffies_now = jiffies;
	time_diff = (((long)jiffies_now - (long)smi_info->last_timeout_jiffies)
		     * SI_USEC_PER_JIFFY);
	smi_result = smi_event_handler(smi_info, time_diff);

<<<<<<< HEAD
	if ((smi_info->irq) && (!smi_info->interrupt_disabled)) {
=======
	if ((smi_info->io.irq) && (!smi_info->interrupt_disabled)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Running with interrupts, only do long timeouts. */
		timeout = jiffies + SI_TIMEOUT_JIFFIES;
		smi_inc_stat(smi_info, long_timeouts);
		goto do_mod_timer;
	}

	/*
	 * If the state machine asks for a short delay, then shorten
	 * the timer timeout.
	 */
	if (smi_result == SI_SM_CALL_WITH_DELAY) {
		smi_inc_stat(smi_info, short_timeouts);
		timeout = jiffies + 1;
	} else {
		smi_inc_stat(smi_info, long_timeouts);
		timeout = jiffies + SI_TIMEOUT_JIFFIES;
	}

<<<<<<< HEAD
 do_mod_timer:
=======
do_mod_timer:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (smi_result != SI_SM_IDLE)
		smi_mod_timer(smi_info, timeout);
	else
		smi_info->timer_running = false;
	spin_unlock_irqrestore(&(smi_info->si_lock), flags);
}

<<<<<<< HEAD
static irqreturn_t si_irq_handler(int irq, void *data)
{
	struct smi_info *smi_info = data;
	unsigned long   flags;
#ifdef DEBUG_TIMING
	struct timeval  t;
#endif
=======
irqreturn_t ipmi_si_irq_handler(int irq, void *data)
{
	struct smi_info *smi_info = data;
	unsigned long   flags;

	if (smi_info->io.si_type == SI_BT)
		/* We need to clear the IRQ flag for the BT interface. */
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG,
				     IPMI_BT_INTMASK_CLEAR_IRQ_BIT
				     | IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&(smi_info->si_lock), flags);

	smi_inc_stat(smi_info, interrupts);

<<<<<<< HEAD
#ifdef DEBUG_TIMING
	do_gettimeofday(&t);
	printk(KERN_DEBUG "**Interrupt: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif
=======
	debug_timestamp(smi_info, "Interrupt");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	smi_event_handler(smi_info, 0);
	spin_unlock_irqrestore(&(smi_info->si_lock), flags);
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static irqreturn_t si_bt_irq_handler(int irq, void *data)
{
	struct smi_info *smi_info = data;
	/* We need to clear the IRQ flag for the BT interface. */
	smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG,
			     IPMI_BT_INTMASK_CLEAR_IRQ_BIT
			     | IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
	return si_irq_handler(irq, data);
}

static int smi_start_processing(void       *send_info,
				ipmi_smi_t intf)
=======
static int smi_start_processing(void            *send_info,
				struct ipmi_smi *intf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct smi_info *new_smi = send_info;
	int             enable = 0;

	new_smi->intf = intf;

<<<<<<< HEAD
	/* Try to claim any interrupts. */
	if (new_smi->irq_setup)
		new_smi->irq_setup(new_smi);

	/* Set up the timer that drives the interface. */
	setup_timer(&new_smi->si_timer, smi_timeout, (long)new_smi);
	smi_mod_timer(new_smi, jiffies + SI_TIMEOUT_JIFFIES);

	/*
	 * Check if the user forcefully enabled the daemon.
	 */
	if (new_smi->intf_num < num_force_kipmid)
		enable = force_kipmid[new_smi->intf_num];
=======
	/* Set up the timer that drives the interface. */
	timer_setup(&new_smi->si_timer, smi_timeout, 0);
	new_smi->timer_can_start = true;
	smi_mod_timer(new_smi, jiffies + SI_TIMEOUT_JIFFIES);

	/* Try to claim any interrupts. */
	if (new_smi->io.irq_setup) {
		new_smi->io.irq_handler_data = new_smi;
		new_smi->io.irq_setup(&new_smi->io);
	}

	/*
	 * Check if the user forcefully enabled the daemon.
	 */
	if (new_smi->si_num < num_force_kipmid)
		enable = force_kipmid[new_smi->si_num];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The BT interface is efficient enough to not need a thread,
	 * and there is no need for a thread if we have interrupts.
	 */
<<<<<<< HEAD
	else if ((new_smi->si_type != SI_BT) && (!new_smi->irq))
=======
	else if ((new_smi->io.si_type != SI_BT) && (!new_smi->io.irq))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		enable = 1;

	if (enable) {
		new_smi->thread = kthread_run(ipmi_thread, new_smi,
<<<<<<< HEAD
					      "kipmi%d", new_smi->intf_num);
		if (IS_ERR(new_smi->thread)) {
			dev_notice(new_smi->dev, "Could not start"
				   " kernel thread due to error %ld, only using"
				   " timers to drive the interface\n",
=======
					      "kipmi%d", new_smi->si_num);
		if (IS_ERR(new_smi->thread)) {
			dev_notice(new_smi->io.dev,
				   "Could not start kernel thread due to error %ld, only using timers to drive the interface\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   PTR_ERR(new_smi->thread));
			new_smi->thread = NULL;
		}
	}

	return 0;
}

static int get_smi_info(void *send_info, struct ipmi_smi_info *data)
{
	struct smi_info *smi = send_info;

<<<<<<< HEAD
	data->addr_src = smi->addr_source;
	data->dev = smi->dev;
	data->addr_info = smi->addr_info;
	get_device(smi->dev);
=======
	data->addr_src = smi->io.addr_source;
	data->dev = smi->io.dev;
	data->addr_info = smi->io.addr_info;
	get_device(smi->io.dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void set_maintenance_mode(void *send_info, int enable)
=======
static void set_maintenance_mode(void *send_info, bool enable)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct smi_info   *smi_info = send_info;

	if (!enable)
		atomic_set(&smi_info->req_events, 0);
<<<<<<< HEAD
}

static struct ipmi_smi_handlers handlers = {
	.owner                  = THIS_MODULE,
	.start_processing       = smi_start_processing,
	.get_smi_info		= get_smi_info,
	.sender			= sender,
	.request_events		= request_events,
	.set_maintenance_mode   = set_maintenance_mode,
	.set_run_to_completion  = set_run_to_completion,
	.poll			= poll,
};

/*
 * There can be 4 IO ports passed in (with or without IRQs), 4 addresses,
 * a default IO port, and 1 ACPI/SPMI address.  That sets SI_MAX_DRIVERS.
 */

=======
	smi_info->in_maintenance_mode = enable;
}

static void shutdown_smi(void *send_info);
static const struct ipmi_smi_handlers handlers = {
	.owner                  = THIS_MODULE,
	.start_processing       = smi_start_processing,
	.shutdown               = shutdown_smi,
	.get_smi_info		= get_smi_info,
	.sender			= sender,
	.request_events		= request_events,
	.set_need_watch		= set_need_watch,
	.set_maintenance_mode   = set_maintenance_mode,
	.set_run_to_completion  = set_run_to_completion,
	.flush_messages		= flush_messages,
	.poll			= poll,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static LIST_HEAD(smi_infos);
static DEFINE_MUTEX(smi_infos_lock);
static int smi_num; /* Used to sequence the SMIs */

<<<<<<< HEAD
#define DEFAULT_REGSPACING	1
#define DEFAULT_REGSIZE		1

static bool          si_trydefaults = 1;
static char          *si_type[SI_MAX_PARMS];
#define MAX_SI_TYPE_STR 30
static char          si_type_str[MAX_SI_TYPE_STR];
static unsigned long addrs[SI_MAX_PARMS];
static unsigned int num_addrs;
static unsigned int  ports[SI_MAX_PARMS];
static unsigned int num_ports;
static int           irqs[SI_MAX_PARMS];
static unsigned int num_irqs;
static int           regspacings[SI_MAX_PARMS];
static unsigned int num_regspacings;
static int           regsizes[SI_MAX_PARMS];
static unsigned int num_regsizes;
static int           regshifts[SI_MAX_PARMS];
static unsigned int num_regshifts;
static int slave_addrs[SI_MAX_PARMS]; /* Leaving 0 chooses the default value */
static unsigned int num_slave_addrs;

#define IPMI_IO_ADDR_SPACE  0
#define IPMI_MEM_ADDR_SPACE 1
static char *addr_space_to_str[] = { "i/o", "mem" };

static int hotmod_handler(const char *val, struct kernel_param *kp);

module_param_call(hotmod, hotmod_handler, NULL, NULL, 0200);
MODULE_PARM_DESC(hotmod, "Add and remove interfaces.  See"
		 " Documentation/IPMI.txt in the kernel sources for the"
		 " gory details.");

module_param_named(trydefaults, si_trydefaults, bool, 0);
MODULE_PARM_DESC(trydefaults, "Setting this to 'false' will disable the"
		 " default scan of the KCS and SMIC interface at the standard"
		 " address");
module_param_string(type, si_type_str, MAX_SI_TYPE_STR, 0);
MODULE_PARM_DESC(type, "Defines the type of each interface, each"
		 " interface separated by commas.  The types are 'kcs',"
		 " 'smic', and 'bt'.  For example si_type=kcs,bt will set"
		 " the first interface to kcs and the second to bt");
module_param_array(addrs, ulong, &num_addrs, 0);
MODULE_PARM_DESC(addrs, "Sets the memory address of each interface, the"
		 " addresses separated by commas.  Only use if an interface"
		 " is in memory.  Otherwise, set it to zero or leave"
		 " it blank.");
module_param_array(ports, uint, &num_ports, 0);
MODULE_PARM_DESC(ports, "Sets the port address of each interface, the"
		 " addresses separated by commas.  Only use if an interface"
		 " is a port.  Otherwise, set it to zero or leave"
		 " it blank.");
module_param_array(irqs, int, &num_irqs, 0);
MODULE_PARM_DESC(irqs, "Sets the interrupt of each interface, the"
		 " addresses separated by commas.  Only use if an interface"
		 " has an interrupt.  Otherwise, set it to zero or leave"
		 " it blank.");
module_param_array(regspacings, int, &num_regspacings, 0);
MODULE_PARM_DESC(regspacings, "The number of bytes between the start address"
		 " and each successive register used by the interface.  For"
		 " instance, if the start address is 0xca2 and the spacing"
		 " is 2, then the second address is at 0xca4.  Defaults"
		 " to 1.");
module_param_array(regsizes, int, &num_regsizes, 0);
MODULE_PARM_DESC(regsizes, "The size of the specific IPMI register in bytes."
		 " This should generally be 1, 2, 4, or 8 for an 8-bit,"
		 " 16-bit, 32-bit, or 64-bit register.  Use this if you"
		 " the 8-bit IPMI register has to be read from a larger"
		 " register.");
module_param_array(regshifts, int, &num_regshifts, 0);
MODULE_PARM_DESC(regshifts, "The amount to shift the data read from the."
		 " IPMI register, in bits.  For instance, if the data"
		 " is read from a 32-bit word and the IPMI data is in"
		 " bit 8-15, then the shift would be 8");
module_param_array(slave_addrs, int, &num_slave_addrs, 0);
MODULE_PARM_DESC(slave_addrs, "Set the default IPMB slave address for"
		 " the controller.  Normally this is 0x20, but can be"
		 " overridden by this parm.  This is an array indexed"
		 " by interface number.");
module_param_array(force_kipmid, int, &num_force_kipmid, 0);
MODULE_PARM_DESC(force_kipmid, "Force the kipmi daemon to be enabled (1) or"
		 " disabled(0).  Normally the IPMI driver auto-detects"
		 " this, but the value may be overridden by this parm.");
module_param(unload_when_empty, int, 0);
MODULE_PARM_DESC(unload_when_empty, "Unload the module if no interfaces are"
		 " specified or found, default is 1.  Setting to 0"
		 " is useful for hot add of devices using hotmod.");
module_param_array(kipmid_max_busy_us, uint, &num_max_busy_us, 0644);
MODULE_PARM_DESC(kipmid_max_busy_us,
		 "Max time (in microseconds) to busy-wait for IPMI data before"
		 " sleeping. 0 (default) means to wait forever. Set to 100-500"
		 " if kipmid is using up a lot of CPU time.");


static void std_irq_cleanup(struct smi_info *info)
{
	if (info->si_type == SI_BT)
		/* Disable the interrupt in the BT interface. */
		info->io.outputb(&info->io, IPMI_BT_INTMASK_REG, 0);
	free_irq(info->irq, info);
}

static int std_irq_setup(struct smi_info *info)
{
	int rv;

	if (!info->irq)
		return 0;

	if (info->si_type == SI_BT) {
		rv = request_irq(info->irq,
				 si_bt_irq_handler,
				 IRQF_SHARED | IRQF_DISABLED,
				 DEVICE_NAME,
				 info);
		if (!rv)
			/* Enable the interrupt in the BT interface. */
			info->io.outputb(&info->io, IPMI_BT_INTMASK_REG,
					 IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
	} else
		rv = request_irq(info->irq,
				 si_irq_handler,
				 IRQF_SHARED | IRQF_DISABLED,
				 DEVICE_NAME,
				 info);
	if (rv) {
		dev_warn(info->dev, "%s unable to claim interrupt %d,"
			 " running polled\n",
			 DEVICE_NAME, info->irq);
		info->irq = 0;
	} else {
		info->irq_cleanup = std_irq_cleanup;
		dev_info(info->dev, "Using irq %d\n", info->irq);
=======
static const char * const addr_space_to_str[] = { "i/o", "mem" };

module_param_array(force_kipmid, int, &num_force_kipmid, 0);
MODULE_PARM_DESC(force_kipmid,
		 "Force the kipmi daemon to be enabled (1) or disabled(0).  Normally the IPMI driver auto-detects this, but the value may be overridden by this parm.");
module_param(unload_when_empty, bool, 0);
MODULE_PARM_DESC(unload_when_empty,
		 "Unload the module if no interfaces are specified or found, default is 1.  Setting to 0 is useful for hot add of devices using hotmod.");
module_param_array(kipmid_max_busy_us, uint, &num_max_busy_us, 0644);
MODULE_PARM_DESC(kipmid_max_busy_us,
		 "Max time (in microseconds) to busy-wait for IPMI data before sleeping. 0 (default) means to wait forever. Set to 100-500 if kipmid is using up a lot of CPU time.");

void ipmi_irq_finish_setup(struct si_sm_io *io)
{
	if (io->si_type == SI_BT)
		/* Enable the interrupt in the BT interface. */
		io->outputb(io, IPMI_BT_INTMASK_REG,
			    IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
}

void ipmi_irq_start_cleanup(struct si_sm_io *io)
{
	if (io->si_type == SI_BT)
		/* Disable the interrupt in the BT interface. */
		io->outputb(io, IPMI_BT_INTMASK_REG, 0);
}

static void std_irq_cleanup(struct si_sm_io *io)
{
	ipmi_irq_start_cleanup(io);
	free_irq(io->irq, io->irq_handler_data);
}

int ipmi_std_irq_setup(struct si_sm_io *io)
{
	int rv;

	if (!io->irq)
		return 0;

	rv = request_irq(io->irq,
			 ipmi_si_irq_handler,
			 IRQF_SHARED,
			 SI_DEVICE_NAME,
			 io->irq_handler_data);
	if (rv) {
		dev_warn(io->dev, "%s unable to claim interrupt %d, running polled\n",
			 SI_DEVICE_NAME, io->irq);
		io->irq = 0;
	} else {
		io->irq_cleanup = std_irq_cleanup;
		ipmi_irq_finish_setup(io);
		dev_info(io->dev, "Using irq %d\n", io->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return rv;
}

<<<<<<< HEAD
static unsigned char port_inb(struct si_sm_io *io, unsigned int offset)
{
	unsigned int addr = io->addr_data;

	return inb(addr + (offset * io->regspacing));
}

static void port_outb(struct si_sm_io *io, unsigned int offset,
		      unsigned char b)
{
	unsigned int addr = io->addr_data;

	outb(b, addr + (offset * io->regspacing));
}

static unsigned char port_inw(struct si_sm_io *io, unsigned int offset)
{
	unsigned int addr = io->addr_data;

	return (inw(addr + (offset * io->regspacing)) >> io->regshift) & 0xff;
}

static void port_outw(struct si_sm_io *io, unsigned int offset,
		      unsigned char b)
{
	unsigned int addr = io->addr_data;

	outw(b << io->regshift, addr + (offset * io->regspacing));
}

static unsigned char port_inl(struct si_sm_io *io, unsigned int offset)
{
	unsigned int addr = io->addr_data;

	return (inl(addr + (offset * io->regspacing)) >> io->regshift) & 0xff;
}

static void port_outl(struct si_sm_io *io, unsigned int offset,
		      unsigned char b)
{
	unsigned int addr = io->addr_data;

	outl(b << io->regshift, addr+(offset * io->regspacing));
}

static void port_cleanup(struct smi_info *info)
{
	unsigned int addr = info->io.addr_data;
	int          idx;

	if (addr) {
		for (idx = 0; idx < info->io_size; idx++)
			release_region(addr + idx * info->io.regspacing,
				       info->io.regsize);
	}
}

static int port_setup(struct smi_info *info)
{
	unsigned int addr = info->io.addr_data;
	int          idx;

	if (!addr)
		return -ENODEV;

	info->io_cleanup = port_cleanup;

	/*
	 * Figure out the actual inb/inw/inl/etc routine to use based
	 * upon the register size.
	 */
	switch (info->io.regsize) {
	case 1:
		info->io.inputb = port_inb;
		info->io.outputb = port_outb;
		break;
	case 2:
		info->io.inputb = port_inw;
		info->io.outputb = port_outw;
		break;
	case 4:
		info->io.inputb = port_inl;
		info->io.outputb = port_outl;
		break;
	default:
		dev_warn(info->dev, "Invalid register size: %d\n",
			 info->io.regsize);
		return -EINVAL;
	}

	/*
	 * Some BIOSes reserve disjoint I/O regions in their ACPI
	 * tables.  This causes problems when trying to register the
	 * entire I/O region.  Therefore we must register each I/O
	 * port separately.
	 */
	for (idx = 0; idx < info->io_size; idx++) {
		if (request_region(addr + idx * info->io.regspacing,
				   info->io.regsize, DEVICE_NAME) == NULL) {
			/* Undo allocations */
			while (idx--) {
				release_region(addr + idx * info->io.regspacing,
					       info->io.regsize);
			}
			return -EIO;
		}
	}
	return 0;
}

static unsigned char intf_mem_inb(struct si_sm_io *io, unsigned int offset)
{
	return readb((io->addr)+(offset * io->regspacing));
}

static void intf_mem_outb(struct si_sm_io *io, unsigned int offset,
		     unsigned char b)
{
	writeb(b, (io->addr)+(offset * io->regspacing));
}

static unsigned char intf_mem_inw(struct si_sm_io *io, unsigned int offset)
{
	return (readw((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}

static void intf_mem_outw(struct si_sm_io *io, unsigned int offset,
		     unsigned char b)
{
	writeb(b << io->regshift, (io->addr)+(offset * io->regspacing));
}

static unsigned char intf_mem_inl(struct si_sm_io *io, unsigned int offset)
{
	return (readl((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}

static void intf_mem_outl(struct si_sm_io *io, unsigned int offset,
		     unsigned char b)
{
	writel(b << io->regshift, (io->addr)+(offset * io->regspacing));
}

#ifdef readq
static unsigned char mem_inq(struct si_sm_io *io, unsigned int offset)
{
	return (readq((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}

static void mem_outq(struct si_sm_io *io, unsigned int offset,
		     unsigned char b)
{
	writeq(b << io->regshift, (io->addr)+(offset * io->regspacing));
}
#endif

static void mem_cleanup(struct smi_info *info)
{
	unsigned long addr = info->io.addr_data;
	int           mapsize;

	if (info->io.addr) {
		iounmap(info->io.addr);

		mapsize = ((info->io_size * info->io.regspacing)
			   - (info->io.regspacing - info->io.regsize));

		release_mem_region(addr, mapsize);
	}
}

static int mem_setup(struct smi_info *info)
{
	unsigned long addr = info->io.addr_data;
	int           mapsize;

	if (!addr)
		return -ENODEV;

	info->io_cleanup = mem_cleanup;

	/*
	 * Figure out the actual readb/readw/readl/etc routine to use based
	 * upon the register size.
	 */
	switch (info->io.regsize) {
	case 1:
		info->io.inputb = intf_mem_inb;
		info->io.outputb = intf_mem_outb;
		break;
	case 2:
		info->io.inputb = intf_mem_inw;
		info->io.outputb = intf_mem_outw;
		break;
	case 4:
		info->io.inputb = intf_mem_inl;
		info->io.outputb = intf_mem_outl;
		break;
#ifdef readq
	case 8:
		info->io.inputb = mem_inq;
		info->io.outputb = mem_outq;
		break;
#endif
	default:
		dev_warn(info->dev, "Invalid register size: %d\n",
			 info->io.regsize);
		return -EINVAL;
	}

	/*
	 * Calculate the total amount of memory to claim.  This is an
	 * unusual looking calculation, but it avoids claiming any
	 * more memory than it has to.  It will claim everything
	 * between the first address to the end of the last full
	 * register.
	 */
	mapsize = ((info->io_size * info->io.regspacing)
		   - (info->io.regspacing - info->io.regsize));

	if (request_mem_region(addr, mapsize, DEVICE_NAME) == NULL)
		return -EIO;

	info->io.addr = ioremap(addr, mapsize);
	if (info->io.addr == NULL) {
		release_mem_region(addr, mapsize);
		return -EIO;
	}
	return 0;
}

/*
 * Parms come in as <op1>[:op2[:op3...]].  ops are:
 *   add|remove,kcs|bt|smic,mem|i/o,<address>[,<opt1>[,<opt2>[,...]]]
 * Options are:
 *   rsp=<regspacing>
 *   rsi=<regsize>
 *   rsh=<regshift>
 *   irq=<irq>
 *   ipmb=<ipmb addr>
 */
enum hotmod_op { HM_ADD, HM_REMOVE };
struct hotmod_vals {
	char *name;
	int  val;
};
static struct hotmod_vals hotmod_ops[] = {
	{ "add",	HM_ADD },
	{ "remove",	HM_REMOVE },
	{ NULL }
};
static struct hotmod_vals hotmod_si[] = {
	{ "kcs",	SI_KCS },
	{ "smic",	SI_SMIC },
	{ "bt",		SI_BT },
	{ NULL }
};
static struct hotmod_vals hotmod_as[] = {
	{ "mem",	IPMI_MEM_ADDR_SPACE },
	{ "i/o",	IPMI_IO_ADDR_SPACE },
	{ NULL }
};

static int parse_str(struct hotmod_vals *v, int *val, char *name, char **curr)
{
	char *s;
	int  i;

	s = strchr(*curr, ',');
	if (!s) {
		printk(KERN_WARNING PFX "No hotmod %s given.\n", name);
		return -EINVAL;
	}
	*s = '\0';
	s++;
	for (i = 0; hotmod_ops[i].name; i++) {
		if (strcmp(*curr, v[i].name) == 0) {
			*val = v[i].val;
			*curr = s;
			return 0;
		}
	}

	printk(KERN_WARNING PFX "Invalid hotmod %s '%s'\n", name, *curr);
	return -EINVAL;
}

static int check_hotmod_int_op(const char *curr, const char *option,
			       const char *name, int *val)
{
	char *n;

	if (strcmp(curr, name) == 0) {
		if (!option) {
			printk(KERN_WARNING PFX
			       "No option given for '%s'\n",
			       curr);
			return -EINVAL;
		}
		*val = simple_strtoul(option, &n, 0);
		if ((*n != '\0') || (*option == '\0')) {
			printk(KERN_WARNING PFX
			       "Bad option given for '%s'\n",
			       curr);
			return -EINVAL;
		}
		return 1;
	}
	return 0;
}

static struct smi_info *smi_info_alloc(void)
{
	struct smi_info *info = kzalloc(sizeof(*info), GFP_KERNEL);

	if (info)
		spin_lock_init(&info->si_lock);
	return info;
}

static int hotmod_handler(const char *val, struct kernel_param *kp)
{
	char *str = kstrdup(val, GFP_KERNEL);
	int  rv;
	char *next, *curr, *s, *n, *o;
	enum hotmod_op op;
	enum si_type si_type;
	int  addr_space;
	unsigned long addr;
	int regspacing;
	int regsize;
	int regshift;
	int irq;
	int ipmb;
	int ival;
	int len;
	struct smi_info *info;

	if (!str)
		return -ENOMEM;

	/* Kill any trailing spaces, as we can get a "\n" from echo. */
	len = strlen(str);
	ival = len - 1;
	while ((ival >= 0) && isspace(str[ival])) {
		str[ival] = '\0';
		ival--;
	}

	for (curr = str; curr; curr = next) {
		regspacing = 1;
		regsize = 1;
		regshift = 0;
		irq = 0;
		ipmb = 0; /* Choose the default if not specified */

		next = strchr(curr, ':');
		if (next) {
			*next = '\0';
			next++;
		}

		rv = parse_str(hotmod_ops, &ival, "operation", &curr);
		if (rv)
			break;
		op = ival;

		rv = parse_str(hotmod_si, &ival, "interface type", &curr);
		if (rv)
			break;
		si_type = ival;

		rv = parse_str(hotmod_as, &addr_space, "address space", &curr);
		if (rv)
			break;

		s = strchr(curr, ',');
		if (s) {
			*s = '\0';
			s++;
		}
		addr = simple_strtoul(curr, &n, 0);
		if ((*n != '\0') || (*curr == '\0')) {
			printk(KERN_WARNING PFX "Invalid hotmod address"
			       " '%s'\n", curr);
			break;
		}

		while (s) {
			curr = s;
			s = strchr(curr, ',');
			if (s) {
				*s = '\0';
				s++;
			}
			o = strchr(curr, '=');
			if (o) {
				*o = '\0';
				o++;
			}
			rv = check_hotmod_int_op(curr, o, "rsp", &regspacing);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "rsi", &regsize);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "rsh", &regshift);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "irq", &irq);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "ipmb", &ipmb);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;

			rv = -EINVAL;
			printk(KERN_WARNING PFX
			       "Invalid hotmod option '%s'\n",
			       curr);
			goto out;
		}

		if (op == HM_ADD) {
			info = smi_info_alloc();
			if (!info) {
				rv = -ENOMEM;
				goto out;
			}

			info->addr_source = SI_HOTMOD;
			info->si_type = si_type;
			info->io.addr_data = addr;
			info->io.addr_type = addr_space;
			if (addr_space == IPMI_MEM_ADDR_SPACE)
				info->io_setup = mem_setup;
			else
				info->io_setup = port_setup;

			info->io.addr = NULL;
			info->io.regspacing = regspacing;
			if (!info->io.regspacing)
				info->io.regspacing = DEFAULT_REGSPACING;
			info->io.regsize = regsize;
			if (!info->io.regsize)
				info->io.regsize = DEFAULT_REGSPACING;
			info->io.regshift = regshift;
			info->irq = irq;
			if (info->irq)
				info->irq_setup = std_irq_setup;
			info->slave_addr = ipmb;

			if (!add_smi(info)) {
				if (try_smi_init(info))
					cleanup_one_si(info);
			} else {
				kfree(info);
			}
		} else {
			/* remove */
			struct smi_info *e, *tmp_e;

			mutex_lock(&smi_infos_lock);
			list_for_each_entry_safe(e, tmp_e, &smi_infos, link) {
				if (e->io.addr_type != addr_space)
					continue;
				if (e->si_type != si_type)
					continue;
				if (e->io.addr_data == addr)
					cleanup_one_si(e);
			}
			mutex_unlock(&smi_infos_lock);
		}
	}
	rv = len;
 out:
	kfree(str);
	return rv;
}

static int __devinit hardcode_find_bmc(void)
{
	int ret = -ENODEV;
	int             i;
	struct smi_info *info;

	for (i = 0; i < SI_MAX_PARMS; i++) {
		if (!ports[i] && !addrs[i])
			continue;

		info = smi_info_alloc();
		if (!info)
			return -ENOMEM;

		info->addr_source = SI_HARDCODED;
		printk(KERN_INFO PFX "probing via hardcoded address\n");

		if (!si_type[i] || strcmp(si_type[i], "kcs") == 0) {
			info->si_type = SI_KCS;
		} else if (strcmp(si_type[i], "smic") == 0) {
			info->si_type = SI_SMIC;
		} else if (strcmp(si_type[i], "bt") == 0) {
			info->si_type = SI_BT;
		} else {
			printk(KERN_WARNING PFX "Interface type specified "
			       "for interface %d, was invalid: %s\n",
			       i, si_type[i]);
			kfree(info);
			continue;
		}

		if (ports[i]) {
			/* An I/O port */
			info->io_setup = port_setup;
			info->io.addr_data = ports[i];
			info->io.addr_type = IPMI_IO_ADDR_SPACE;
		} else if (addrs[i]) {
			/* A memory port */
			info->io_setup = mem_setup;
			info->io.addr_data = addrs[i];
			info->io.addr_type = IPMI_MEM_ADDR_SPACE;
		} else {
			printk(KERN_WARNING PFX "Interface type specified "
			       "for interface %d, but port and address were "
			       "not set or set to zero.\n", i);
			kfree(info);
			continue;
		}

		info->io.addr = NULL;
		info->io.regspacing = regspacings[i];
		if (!info->io.regspacing)
			info->io.regspacing = DEFAULT_REGSPACING;
		info->io.regsize = regsizes[i];
		if (!info->io.regsize)
			info->io.regsize = DEFAULT_REGSPACING;
		info->io.regshift = regshifts[i];
		info->irq = irqs[i];
		if (info->irq)
			info->irq_setup = std_irq_setup;
		info->slave_addr = slave_addrs[i];

		if (!add_smi(info)) {
			if (try_smi_init(info))
				cleanup_one_si(info);
			ret = 0;
		} else {
			kfree(info);
		}
	}
	return ret;
}

#ifdef CONFIG_ACPI

#include <linux/acpi.h>

/*
 * Once we get an ACPI failure, we don't try any more, because we go
 * through the tables sequentially.  Once we don't find a table, there
 * are no more.
 */
static int acpi_failure;

/* For GPE-type interrupts. */
static u32 ipmi_acpi_gpe(acpi_handle gpe_device,
	u32 gpe_number, void *context)
{
	struct smi_info *smi_info = context;
	unsigned long   flags;
#ifdef DEBUG_TIMING
	struct timeval t;
#endif

	spin_lock_irqsave(&(smi_info->si_lock), flags);

	smi_inc_stat(smi_info, interrupts);

#ifdef DEBUG_TIMING
	do_gettimeofday(&t);
	printk("**ACPI_GPE: %d.%9.9d\n", t.tv_sec, t.tv_usec);
#endif
	smi_event_handler(smi_info, 0);
	spin_unlock_irqrestore(&(smi_info->si_lock), flags);

	return ACPI_INTERRUPT_HANDLED;
}

static void acpi_gpe_irq_cleanup(struct smi_info *info)
{
	if (!info->irq)
		return;

	acpi_remove_gpe_handler(NULL, info->irq, &ipmi_acpi_gpe);
}

static int acpi_gpe_irq_setup(struct smi_info *info)
{
	acpi_status status;

	if (!info->irq)
		return 0;

	/* FIXME - is level triggered right? */
	status = acpi_install_gpe_handler(NULL,
					  info->irq,
					  ACPI_GPE_LEVEL_TRIGGERED,
					  &ipmi_acpi_gpe,
					  info);
	if (status != AE_OK) {
		dev_warn(info->dev, "%s unable to claim ACPI GPE %d,"
			 " running polled\n", DEVICE_NAME, info->irq);
		info->irq = 0;
		return -EINVAL;
	} else {
		info->irq_cleanup = acpi_gpe_irq_cleanup;
		dev_info(info->dev, "Using ACPI GPE %d\n", info->irq);
		return 0;
	}
}

/*
 * Defined at
 * http://h21007.www2.hp.com/portal/download/files/unprot/hpspmi.pdf
 */
struct SPMITable {
	s8	Signature[4];
	u32	Length;
	u8	Revision;
	u8	Checksum;
	s8	OEMID[6];
	s8	OEMTableID[8];
	s8	OEMRevision[4];
	s8	CreatorID[4];
	s8	CreatorRevision[4];
	u8	InterfaceType;
	u8	IPMIlegacy;
	s16	SpecificationRevision;

	/*
	 * Bit 0 - SCI interrupt supported
	 * Bit 1 - I/O APIC/SAPIC
	 */
	u8	InterruptType;

	/*
	 * If bit 0 of InterruptType is set, then this is the SCI
	 * interrupt in the GPEx_STS register.
	 */
	u8	GPE;

	s16	Reserved;

	/*
	 * If bit 1 of InterruptType is set, then this is the I/O
	 * APIC/SAPIC interrupt.
	 */
	u32	GlobalSystemInterrupt;

	/* The actual register address. */
	struct acpi_generic_address addr;

	u8	UID[4];

	s8      spmi_id[1]; /* A '\0' terminated array starts here. */
};

static int __devinit try_init_spmi(struct SPMITable *spmi)
{
	struct smi_info  *info;

	if (spmi->IPMIlegacy != 1) {
		printk(KERN_INFO PFX "Bad SPMI legacy %d\n", spmi->IPMIlegacy);
		return -ENODEV;
	}

	info = smi_info_alloc();
	if (!info) {
		printk(KERN_ERR PFX "Could not allocate SI data (3)\n");
		return -ENOMEM;
	}

	info->addr_source = SI_SPMI;
	printk(KERN_INFO PFX "probing via SPMI\n");

	/* Figure out the interface type. */
	switch (spmi->InterfaceType) {
	case 1:	/* KCS */
		info->si_type = SI_KCS;
		break;
	case 2:	/* SMIC */
		info->si_type = SI_SMIC;
		break;
	case 3:	/* BT */
		info->si_type = SI_BT;
		break;
	default:
		printk(KERN_INFO PFX "Unknown ACPI/SPMI SI type %d\n",
		       spmi->InterfaceType);
		kfree(info);
		return -EIO;
	}

	if (spmi->InterruptType & 1) {
		/* We've got a GPE interrupt. */
		info->irq = spmi->GPE;
		info->irq_setup = acpi_gpe_irq_setup;
	} else if (spmi->InterruptType & 2) {
		/* We've got an APIC/SAPIC interrupt. */
		info->irq = spmi->GlobalSystemInterrupt;
		info->irq_setup = std_irq_setup;
	} else {
		/* Use the default interrupt setting. */
		info->irq = 0;
		info->irq_setup = NULL;
	}

	if (spmi->addr.bit_width) {
		/* A (hopefully) properly formed register bit width. */
		info->io.regspacing = spmi->addr.bit_width / 8;
	} else {
		info->io.regspacing = DEFAULT_REGSPACING;
	}
	info->io.regsize = info->io.regspacing;
	info->io.regshift = spmi->addr.bit_offset;

	if (spmi->addr.space_id == ACPI_ADR_SPACE_SYSTEM_MEMORY) {
		info->io_setup = mem_setup;
		info->io.addr_type = IPMI_MEM_ADDR_SPACE;
	} else if (spmi->addr.space_id == ACPI_ADR_SPACE_SYSTEM_IO) {
		info->io_setup = port_setup;
		info->io.addr_type = IPMI_IO_ADDR_SPACE;
	} else {
		kfree(info);
		printk(KERN_WARNING PFX "Unknown ACPI I/O Address type\n");
		return -EIO;
	}
	info->io.addr_data = spmi->addr.address;

	pr_info("ipmi_si: SPMI: %s %#lx regsize %d spacing %d irq %d\n",
		 (info->io.addr_type == IPMI_IO_ADDR_SPACE) ? "io" : "mem",
		 info->io.addr_data, info->io.regsize, info->io.regspacing,
		 info->irq);

	if (add_smi(info))
		kfree(info);

	return 0;
}

static void __devinit spmi_find_bmc(void)
{
	acpi_status      status;
	struct SPMITable *spmi;
	int              i;

	if (acpi_disabled)
		return;

	if (acpi_failure)
		return;

	for (i = 0; ; i++) {
		status = acpi_get_table(ACPI_SIG_SPMI, i+1,
					(struct acpi_table_header **)&spmi);
		if (status != AE_OK)
			return;

		try_init_spmi(spmi);
	}
}

static int __devinit ipmi_pnp_probe(struct pnp_dev *dev,
				    const struct pnp_device_id *dev_id)
{
	struct acpi_device *acpi_dev;
	struct smi_info *info;
	struct resource *res, *res_second;
	acpi_handle handle;
	acpi_status status;
	unsigned long long tmp;

	acpi_dev = pnp_acpi_device(dev);
	if (!acpi_dev)
		return -ENODEV;

	info = smi_info_alloc();
	if (!info)
		return -ENOMEM;

	info->addr_source = SI_ACPI;
	printk(KERN_INFO PFX "probing via ACPI\n");

	handle = acpi_dev->handle;
	info->addr_info.acpi_info.acpi_handle = handle;

	/* _IFT tells us the interface type: KCS, BT, etc */
	status = acpi_evaluate_integer(handle, "_IFT", NULL, &tmp);
	if (ACPI_FAILURE(status))
		goto err_free;

	switch (tmp) {
	case 1:
		info->si_type = SI_KCS;
		break;
	case 2:
		info->si_type = SI_SMIC;
		break;
	case 3:
		info->si_type = SI_BT;
		break;
	default:
		dev_info(&dev->dev, "unknown IPMI type %lld\n", tmp);
		goto err_free;
	}

	res = pnp_get_resource(dev, IORESOURCE_IO, 0);
	if (res) {
		info->io_setup = port_setup;
		info->io.addr_type = IPMI_IO_ADDR_SPACE;
	} else {
		res = pnp_get_resource(dev, IORESOURCE_MEM, 0);
		if (res) {
			info->io_setup = mem_setup;
			info->io.addr_type = IPMI_MEM_ADDR_SPACE;
		}
	}
	if (!res) {
		dev_err(&dev->dev, "no I/O or memory address\n");
		goto err_free;
	}
	info->io.addr_data = res->start;

	info->io.regspacing = DEFAULT_REGSPACING;
	res_second = pnp_get_resource(dev,
			       (info->io.addr_type == IPMI_IO_ADDR_SPACE) ?
					IORESOURCE_IO : IORESOURCE_MEM,
			       1);
	if (res_second) {
		if (res_second->start > info->io.addr_data)
			info->io.regspacing = res_second->start - info->io.addr_data;
	}
	info->io.regsize = DEFAULT_REGSPACING;
	info->io.regshift = 0;

	/* If _GPE exists, use it; otherwise use standard interrupts */
	status = acpi_evaluate_integer(handle, "_GPE", NULL, &tmp);
	if (ACPI_SUCCESS(status)) {
		info->irq = tmp;
		info->irq_setup = acpi_gpe_irq_setup;
	} else if (pnp_irq_valid(dev, 0)) {
		info->irq = pnp_irq(dev, 0);
		info->irq_setup = std_irq_setup;
	}

	info->dev = &dev->dev;
	pnp_set_drvdata(dev, info);

	dev_info(info->dev, "%pR regsize %d spacing %d irq %d\n",
		 res, info->io.regsize, info->io.regspacing,
		 info->irq);

	if (add_smi(info))
		goto err_free;

	return 0;

err_free:
	kfree(info);
	return -EINVAL;
}

static void __devexit ipmi_pnp_remove(struct pnp_dev *dev)
{
	struct smi_info *info = pnp_get_drvdata(dev);

	cleanup_one_si(info);
}

static const struct pnp_device_id pnp_dev_table[] = {
	{"IPI0001", 0},
	{"", 0},
};

static struct pnp_driver ipmi_pnp_driver = {
	.name		= DEVICE_NAME,
	.probe		= ipmi_pnp_probe,
	.remove		= __devexit_p(ipmi_pnp_remove),
	.id_table	= pnp_dev_table,
};
#endif

#ifdef CONFIG_DMI
struct dmi_ipmi_data {
	u8   		type;
	u8   		addr_space;
	unsigned long	base_addr;
	u8   		irq;
	u8              offset;
	u8              slave_addr;
};

static int __devinit decode_dmi(const struct dmi_header *dm,
				struct dmi_ipmi_data *dmi)
{
	const u8	*data = (const u8 *)dm;
	unsigned long  	base_addr;
	u8		reg_spacing;
	u8              len = dm->length;

	dmi->type = data[4];

	memcpy(&base_addr, data+8, sizeof(unsigned long));
	if (len >= 0x11) {
		if (base_addr & 1) {
			/* I/O */
			base_addr &= 0xFFFE;
			dmi->addr_space = IPMI_IO_ADDR_SPACE;
		} else
			/* Memory */
			dmi->addr_space = IPMI_MEM_ADDR_SPACE;

		/* If bit 4 of byte 0x10 is set, then the lsb for the address
		   is odd. */
		dmi->base_addr = base_addr | ((data[0x10] & 0x10) >> 4);

		dmi->irq = data[0x11];

		/* The top two bits of byte 0x10 hold the register spacing. */
		reg_spacing = (data[0x10] & 0xC0) >> 6;
		switch (reg_spacing) {
		case 0x00: /* Byte boundaries */
		    dmi->offset = 1;
		    break;
		case 0x01: /* 32-bit boundaries */
		    dmi->offset = 4;
		    break;
		case 0x02: /* 16-byte boundaries */
		    dmi->offset = 16;
		    break;
		default:
		    /* Some other interface, just ignore it. */
		    return -EIO;
		}
	} else {
		/* Old DMI spec. */
		/*
		 * Note that technically, the lower bit of the base
		 * address should be 1 if the address is I/O and 0 if
		 * the address is in memory.  So many systems get that
		 * wrong (and all that I have seen are I/O) so we just
		 * ignore that bit and assume I/O.  Systems that use
		 * memory should use the newer spec, anyway.
		 */
		dmi->base_addr = base_addr & 0xfffe;
		dmi->addr_space = IPMI_IO_ADDR_SPACE;
		dmi->offset = 1;
	}

	dmi->slave_addr = data[6];

	return 0;
}

static void __devinit try_init_dmi(struct dmi_ipmi_data *ipmi_data)
{
	struct smi_info *info;

	info = smi_info_alloc();
	if (!info) {
		printk(KERN_ERR PFX "Could not allocate SI data\n");
		return;
	}

	info->addr_source = SI_SMBIOS;
	printk(KERN_INFO PFX "probing via SMBIOS\n");

	switch (ipmi_data->type) {
	case 0x01: /* KCS */
		info->si_type = SI_KCS;
		break;
	case 0x02: /* SMIC */
		info->si_type = SI_SMIC;
		break;
	case 0x03: /* BT */
		info->si_type = SI_BT;
		break;
	default:
		kfree(info);
		return;
	}

	switch (ipmi_data->addr_space) {
	case IPMI_MEM_ADDR_SPACE:
		info->io_setup = mem_setup;
		info->io.addr_type = IPMI_MEM_ADDR_SPACE;
		break;

	case IPMI_IO_ADDR_SPACE:
		info->io_setup = port_setup;
		info->io.addr_type = IPMI_IO_ADDR_SPACE;
		break;

	default:
		kfree(info);
		printk(KERN_WARNING PFX "Unknown SMBIOS I/O Address type: %d\n",
		       ipmi_data->addr_space);
		return;
	}
	info->io.addr_data = ipmi_data->base_addr;

	info->io.regspacing = ipmi_data->offset;
	if (!info->io.regspacing)
		info->io.regspacing = DEFAULT_REGSPACING;
	info->io.regsize = DEFAULT_REGSPACING;
	info->io.regshift = 0;

	info->slave_addr = ipmi_data->slave_addr;

	info->irq = ipmi_data->irq;
	if (info->irq)
		info->irq_setup = std_irq_setup;

	pr_info("ipmi_si: SMBIOS: %s %#lx regsize %d spacing %d irq %d\n",
		 (info->io.addr_type == IPMI_IO_ADDR_SPACE) ? "io" : "mem",
		 info->io.addr_data, info->io.regsize, info->io.regspacing,
		 info->irq);

	if (add_smi(info))
		kfree(info);
}

static void __devinit dmi_find_bmc(void)
{
	const struct dmi_device *dev = NULL;
	struct dmi_ipmi_data data;
	int                  rv;

	while ((dev = dmi_find_device(DMI_DEV_TYPE_IPMI, NULL, dev))) {
		memset(&data, 0, sizeof(data));
		rv = decode_dmi((const struct dmi_header *) dev->device_data,
				&data);
		if (!rv)
			try_init_dmi(&data);
	}
}
#endif /* CONFIG_DMI */

#ifdef CONFIG_PCI

#define PCI_ERMC_CLASSCODE		0x0C0700
#define PCI_ERMC_CLASSCODE_MASK		0xffffff00
#define PCI_ERMC_CLASSCODE_TYPE_MASK	0xff
#define PCI_ERMC_CLASSCODE_TYPE_SMIC	0x00
#define PCI_ERMC_CLASSCODE_TYPE_KCS	0x01
#define PCI_ERMC_CLASSCODE_TYPE_BT	0x02

#define PCI_HP_VENDOR_ID    0x103C
#define PCI_MMC_DEVICE_ID   0x121A
#define PCI_MMC_ADDR_CW     0x10

static void ipmi_pci_cleanup(struct smi_info *info)
{
	struct pci_dev *pdev = info->addr_source_data;

	pci_disable_device(pdev);
}

static int __devinit ipmi_pci_probe(struct pci_dev *pdev,
				    const struct pci_device_id *ent)
{
	int rv;
	int class_type = pdev->class & PCI_ERMC_CLASSCODE_TYPE_MASK;
	struct smi_info *info;

	info = smi_info_alloc();
	if (!info)
		return -ENOMEM;

	info->addr_source = SI_PCI;
	dev_info(&pdev->dev, "probing via PCI");

	switch (class_type) {
	case PCI_ERMC_CLASSCODE_TYPE_SMIC:
		info->si_type = SI_SMIC;
		break;

	case PCI_ERMC_CLASSCODE_TYPE_KCS:
		info->si_type = SI_KCS;
		break;

	case PCI_ERMC_CLASSCODE_TYPE_BT:
		info->si_type = SI_BT;
		break;

	default:
		kfree(info);
		dev_info(&pdev->dev, "Unknown IPMI type: %d\n", class_type);
		return -ENOMEM;
	}

	rv = pci_enable_device(pdev);
	if (rv) {
		dev_err(&pdev->dev, "couldn't enable PCI device\n");
		kfree(info);
		return rv;
	}

	info->addr_source_cleanup = ipmi_pci_cleanup;
	info->addr_source_data = pdev;

	if (pci_resource_flags(pdev, 0) & IORESOURCE_IO) {
		info->io_setup = port_setup;
		info->io.addr_type = IPMI_IO_ADDR_SPACE;
	} else {
		info->io_setup = mem_setup;
		info->io.addr_type = IPMI_MEM_ADDR_SPACE;
	}
	info->io.addr_data = pci_resource_start(pdev, 0);

	info->io.regspacing = DEFAULT_REGSPACING;
	info->io.regsize = DEFAULT_REGSPACING;
	info->io.regshift = 0;

	info->irq = pdev->irq;
	if (info->irq)
		info->irq_setup = std_irq_setup;

	info->dev = &pdev->dev;
	pci_set_drvdata(pdev, info);

	dev_info(&pdev->dev, "%pR regsize %d spacing %d irq %d\n",
		&pdev->resource[0], info->io.regsize, info->io.regspacing,
		info->irq);

	if (add_smi(info))
		kfree(info);

	return 0;
}

static void __devexit ipmi_pci_remove(struct pci_dev *pdev)
{
	struct smi_info *info = pci_get_drvdata(pdev);
	cleanup_one_si(info);
}

#ifdef CONFIG_PM
static int ipmi_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	return 0;
}

static int ipmi_pci_resume(struct pci_dev *pdev)
{
	return 0;
}
#endif

static struct pci_device_id ipmi_pci_devices[] = {
	{ PCI_DEVICE(PCI_HP_VENDOR_ID, PCI_MMC_DEVICE_ID) },
	{ PCI_DEVICE_CLASS(PCI_ERMC_CLASSCODE, PCI_ERMC_CLASSCODE_MASK) },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, ipmi_pci_devices);

static struct pci_driver ipmi_pci_driver = {
	.name =         DEVICE_NAME,
	.id_table =     ipmi_pci_devices,
	.probe =        ipmi_pci_probe,
	.remove =       __devexit_p(ipmi_pci_remove),
#ifdef CONFIG_PM
	.suspend =      ipmi_pci_suspend,
	.resume =       ipmi_pci_resume,
#endif
};
#endif /* CONFIG_PCI */

static struct of_device_id ipmi_match[];
static int __devinit ipmi_probe(struct platform_device *dev)
{
#ifdef CONFIG_OF
	const struct of_device_id *match;
	struct smi_info *info;
	struct resource resource;
	const __be32 *regsize, *regspacing, *regshift;
	struct device_node *np = dev->dev.of_node;
	int ret;
	int proplen;

	dev_info(&dev->dev, "probing via device tree\n");

	match = of_match_device(ipmi_match, &dev->dev);
	if (!match)
		return -EINVAL;

	ret = of_address_to_resource(np, 0, &resource);
	if (ret) {
		dev_warn(&dev->dev, PFX "invalid address from OF\n");
		return ret;
	}

	regsize = of_get_property(np, "reg-size", &proplen);
	if (regsize && proplen != 4) {
		dev_warn(&dev->dev, PFX "invalid regsize from OF\n");
		return -EINVAL;
	}

	regspacing = of_get_property(np, "reg-spacing", &proplen);
	if (regspacing && proplen != 4) {
		dev_warn(&dev->dev, PFX "invalid regspacing from OF\n");
		return -EINVAL;
	}

	regshift = of_get_property(np, "reg-shift", &proplen);
	if (regshift && proplen != 4) {
		dev_warn(&dev->dev, PFX "invalid regshift from OF\n");
		return -EINVAL;
	}

	info = smi_info_alloc();

	if (!info) {
		dev_err(&dev->dev,
			"could not allocate memory for OF probe\n");
		return -ENOMEM;
	}

	info->si_type		= (enum si_type) match->data;
	info->addr_source	= SI_DEVICETREE;
	info->irq_setup		= std_irq_setup;

	if (resource.flags & IORESOURCE_IO) {
		info->io_setup		= port_setup;
		info->io.addr_type	= IPMI_IO_ADDR_SPACE;
	} else {
		info->io_setup		= mem_setup;
		info->io.addr_type	= IPMI_MEM_ADDR_SPACE;
	}

	info->io.addr_data	= resource.start;

	info->io.regsize	= regsize ? be32_to_cpup(regsize) : DEFAULT_REGSIZE;
	info->io.regspacing	= regspacing ? be32_to_cpup(regspacing) : DEFAULT_REGSPACING;
	info->io.regshift	= regshift ? be32_to_cpup(regshift) : 0;

	info->irq		= irq_of_parse_and_map(dev->dev.of_node, 0);
	info->dev		= &dev->dev;

	dev_dbg(&dev->dev, "addr 0x%lx regsize %d spacing %d irq %d\n",
		info->io.addr_data, info->io.regsize, info->io.regspacing,
		info->irq);

	dev_set_drvdata(&dev->dev, info);

	if (add_smi(info)) {
		kfree(info);
		return -EBUSY;
	}
#endif
	return 0;
}

static int __devexit ipmi_remove(struct platform_device *dev)
{
#ifdef CONFIG_OF
	cleanup_one_si(dev_get_drvdata(&dev->dev));
#endif
	return 0;
}

static struct of_device_id ipmi_match[] =
{
	{ .type = "ipmi", .compatible = "ipmi-kcs",
	  .data = (void *)(unsigned long) SI_KCS },
	{ .type = "ipmi", .compatible = "ipmi-smic",
	  .data = (void *)(unsigned long) SI_SMIC },
	{ .type = "ipmi", .compatible = "ipmi-bt",
	  .data = (void *)(unsigned long) SI_BT },
	{},
};

static struct platform_driver ipmi_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = ipmi_match,
	},
	.probe		= ipmi_probe,
	.remove		= __devexit_p(ipmi_remove),
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int wait_for_msg_done(struct smi_info *smi_info)
{
	enum si_sm_result     smi_result;

	smi_result = smi_info->handlers->event(smi_info->si_sm, 0);
	for (;;) {
		if (smi_result == SI_SM_CALL_WITH_DELAY ||
		    smi_result == SI_SM_CALL_WITH_TICK_DELAY) {
			schedule_timeout_uninterruptible(1);
			smi_result = smi_info->handlers->event(
				smi_info->si_sm, jiffies_to_usecs(1));
		} else if (smi_result == SI_SM_CALL_WITHOUT_DELAY) {
			smi_result = smi_info->handlers->event(
				smi_info->si_sm, 0);
		} else
			break;
	}
	if (smi_result == SI_SM_HOSED)
		/*
		 * We couldn't get the state machine to run, so whatever's at
		 * the port is probably not an IPMI SMI interface.
		 */
		return -ENODEV;

	return 0;
}

static int try_get_dev_id(struct smi_info *smi_info)
{
	unsigned char         msg[2];
	unsigned char         *resp;
	unsigned long         resp_len;
	int                   rv = 0;
<<<<<<< HEAD
=======
	unsigned int          retry_count = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	resp = kmalloc(IPMI_MAX_MSG_LENGTH, GFP_KERNEL);
	if (!resp)
		return -ENOMEM;

	/*
	 * Do a Get Device ID command, since it comes back with some
	 * useful info.
	 */
	msg[0] = IPMI_NETFN_APP_REQUEST << 2;
	msg[1] = IPMI_GET_DEVICE_ID_CMD;
<<<<<<< HEAD
=======

retry:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 2);

	rv = wait_for_msg_done(smi_info);
	if (rv)
		goto out;

	resp_len = smi_info->handlers->get_result(smi_info->si_sm,
						  resp, IPMI_MAX_MSG_LENGTH);

	/* Check and record info from the get device id, in case we need it. */
<<<<<<< HEAD
	rv = ipmi_demangle_device_id(resp, resp_len, &smi_info->device_id);

 out:
=======
	rv = ipmi_demangle_device_id(resp[0] >> 2, resp[1],
			resp + 2, resp_len - 2, &smi_info->device_id);
	if (rv) {
		/* record completion code */
		unsigned char cc = *(resp + 2);

		if (cc != IPMI_CC_NO_ERROR &&
		    ++retry_count <= GET_DEVICE_ID_MAX_RETRY) {
			dev_warn_ratelimited(smi_info->io.dev,
			    "BMC returned 0x%2.2x, retry get bmc device id\n",
			    cc);
			goto retry;
		}
	}

out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(resp);
	return rv;
}

<<<<<<< HEAD
=======
static int get_global_enables(struct smi_info *smi_info, u8 *enables)
{
	unsigned char         msg[3];
	unsigned char         *resp;
	unsigned long         resp_len;
	int                   rv;

	resp = kmalloc(IPMI_MAX_MSG_LENGTH, GFP_KERNEL);
	if (!resp)
		return -ENOMEM;

	msg[0] = IPMI_NETFN_APP_REQUEST << 2;
	msg[1] = IPMI_GET_BMC_GLOBAL_ENABLES_CMD;
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 2);

	rv = wait_for_msg_done(smi_info);
	if (rv) {
		dev_warn(smi_info->io.dev,
			 "Error getting response from get global enables command: %d\n",
			 rv);
		goto out;
	}

	resp_len = smi_info->handlers->get_result(smi_info->si_sm,
						  resp, IPMI_MAX_MSG_LENGTH);

	if (resp_len < 4 ||
			resp[0] != (IPMI_NETFN_APP_REQUEST | 1) << 2 ||
			resp[1] != IPMI_GET_BMC_GLOBAL_ENABLES_CMD   ||
			resp[2] != 0) {
		dev_warn(smi_info->io.dev,
			 "Invalid return from get global enables command: %ld %x %x %x\n",
			 resp_len, resp[0], resp[1], resp[2]);
		rv = -EINVAL;
		goto out;
	} else {
		*enables = resp[3];
	}

out:
	kfree(resp);
	return rv;
}

/*
 * Returns 1 if it gets an error from the command.
 */
static int set_global_enables(struct smi_info *smi_info, u8 enables)
{
	unsigned char         msg[3];
	unsigned char         *resp;
	unsigned long         resp_len;
	int                   rv;

	resp = kmalloc(IPMI_MAX_MSG_LENGTH, GFP_KERNEL);
	if (!resp)
		return -ENOMEM;

	msg[0] = IPMI_NETFN_APP_REQUEST << 2;
	msg[1] = IPMI_SET_BMC_GLOBAL_ENABLES_CMD;
	msg[2] = enables;
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 3);

	rv = wait_for_msg_done(smi_info);
	if (rv) {
		dev_warn(smi_info->io.dev,
			 "Error getting response from set global enables command: %d\n",
			 rv);
		goto out;
	}

	resp_len = smi_info->handlers->get_result(smi_info->si_sm,
						  resp, IPMI_MAX_MSG_LENGTH);

	if (resp_len < 3 ||
			resp[0] != (IPMI_NETFN_APP_REQUEST | 1) << 2 ||
			resp[1] != IPMI_SET_BMC_GLOBAL_ENABLES_CMD) {
		dev_warn(smi_info->io.dev,
			 "Invalid return from set global enables command: %ld %x %x\n",
			 resp_len, resp[0], resp[1]);
		rv = -EINVAL;
		goto out;
	}

	if (resp[2] != 0)
		rv = 1;

out:
	kfree(resp);
	return rv;
}

/*
 * Some BMCs do not support clearing the receive irq bit in the global
 * enables (even if they don't support interrupts on the BMC).  Check
 * for this and handle it properly.
 */
static void check_clr_rcv_irq(struct smi_info *smi_info)
{
	u8 enables = 0;
	int rv;

	rv = get_global_enables(smi_info, &enables);
	if (!rv) {
		if ((enables & IPMI_BMC_RCV_MSG_INTR) == 0)
			/* Already clear, should work ok. */
			return;

		enables &= ~IPMI_BMC_RCV_MSG_INTR;
		rv = set_global_enables(smi_info, enables);
	}

	if (rv < 0) {
		dev_err(smi_info->io.dev,
			"Cannot check clearing the rcv irq: %d\n", rv);
		return;
	}

	if (rv) {
		/*
		 * An error when setting the event buffer bit means
		 * clearing the bit is not supported.
		 */
		dev_warn(smi_info->io.dev,
			 "The BMC does not support clearing the recv irq bit, compensating, but the BMC needs to be fixed.\n");
		smi_info->cannot_disable_irq = true;
	}
}

/*
 * Some BMCs do not support setting the interrupt bits in the global
 * enables even if they support interrupts.  Clearly bad, but we can
 * compensate.
 */
static void check_set_rcv_irq(struct smi_info *smi_info)
{
	u8 enables = 0;
	int rv;

	if (!smi_info->io.irq)
		return;

	rv = get_global_enables(smi_info, &enables);
	if (!rv) {
		enables |= IPMI_BMC_RCV_MSG_INTR;
		rv = set_global_enables(smi_info, enables);
	}

	if (rv < 0) {
		dev_err(smi_info->io.dev,
			"Cannot check setting the rcv irq: %d\n", rv);
		return;
	}

	if (rv) {
		/*
		 * An error when setting the event buffer bit means
		 * setting the bit is not supported.
		 */
		dev_warn(smi_info->io.dev,
			 "The BMC does not support setting the recv irq bit, compensating, but the BMC needs to be fixed.\n");
		smi_info->cannot_disable_irq = true;
		smi_info->irq_enable_broken = true;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int try_enable_event_buffer(struct smi_info *smi_info)
{
	unsigned char         msg[3];
	unsigned char         *resp;
	unsigned long         resp_len;
	int                   rv = 0;

	resp = kmalloc(IPMI_MAX_MSG_LENGTH, GFP_KERNEL);
	if (!resp)
		return -ENOMEM;

	msg[0] = IPMI_NETFN_APP_REQUEST << 2;
	msg[1] = IPMI_GET_BMC_GLOBAL_ENABLES_CMD;
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 2);

	rv = wait_for_msg_done(smi_info);
	if (rv) {
<<<<<<< HEAD
		printk(KERN_WARNING PFX "Error getting response from get"
		       " global enables command, the event buffer is not"
		       " enabled.\n");
=======
		pr_warn("Error getting response from get global enables command, the event buffer is not enabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	resp_len = smi_info->handlers->get_result(smi_info->si_sm,
						  resp, IPMI_MAX_MSG_LENGTH);

	if (resp_len < 4 ||
			resp[0] != (IPMI_NETFN_APP_REQUEST | 1) << 2 ||
			resp[1] != IPMI_GET_BMC_GLOBAL_ENABLES_CMD   ||
			resp[2] != 0) {
<<<<<<< HEAD
		printk(KERN_WARNING PFX "Invalid return from get global"
		       " enables command, cannot enable the event buffer.\n");
=======
		pr_warn("Invalid return from get global enables command, cannot enable the event buffer\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rv = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	if (resp[3] & IPMI_BMC_EVT_MSG_BUFF)
		/* buffer is already enabled, nothing to do. */
		goto out;
=======
	if (resp[3] & IPMI_BMC_EVT_MSG_BUFF) {
		/* buffer is already enabled, nothing to do. */
		smi_info->supports_event_msg_buff = true;
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	msg[0] = IPMI_NETFN_APP_REQUEST << 2;
	msg[1] = IPMI_SET_BMC_GLOBAL_ENABLES_CMD;
	msg[2] = resp[3] | IPMI_BMC_EVT_MSG_BUFF;
	smi_info->handlers->start_transaction(smi_info->si_sm, msg, 3);

	rv = wait_for_msg_done(smi_info);
	if (rv) {
<<<<<<< HEAD
		printk(KERN_WARNING PFX "Error getting response from set"
		       " global, enables command, the event buffer is not"
		       " enabled.\n");
=======
		pr_warn("Error getting response from set global, enables command, the event buffer is not enabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	resp_len = smi_info->handlers->get_result(smi_info->si_sm,
						  resp, IPMI_MAX_MSG_LENGTH);

	if (resp_len < 3 ||
			resp[0] != (IPMI_NETFN_APP_REQUEST | 1) << 2 ||
			resp[1] != IPMI_SET_BMC_GLOBAL_ENABLES_CMD) {
<<<<<<< HEAD
		printk(KERN_WARNING PFX "Invalid return from get global,"
		       "enables command, not enable the event buffer.\n");
=======
		pr_warn("Invalid return from get global, enables command, not enable the event buffer\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rv = -EINVAL;
		goto out;
	}

	if (resp[2] != 0)
		/*
		 * An error when setting the event buffer bit means
		 * that the event buffer is not supported.
		 */
		rv = -ENOENT;
<<<<<<< HEAD
 out:
=======
	else
		smi_info->supports_event_msg_buff = true;

out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(resp);
	return rv;
}

<<<<<<< HEAD
static int smi_type_proc_show(struct seq_file *m, void *v)
{
	struct smi_info *smi = m->private;

	return seq_printf(m, "%s\n", si_to_str[smi->si_type]);
}

static int smi_type_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, smi_type_proc_show, PDE(inode)->data);
}

static const struct file_operations smi_type_proc_ops = {
	.open		= smi_type_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int smi_si_stats_proc_show(struct seq_file *m, void *v)
{
	struct smi_info *smi = m->private;

	seq_printf(m, "interrupts_enabled:    %d\n",
		       smi->irq && !smi->interrupt_disabled);
	seq_printf(m, "short_timeouts:        %u\n",
		       smi_get_stat(smi, short_timeouts));
	seq_printf(m, "long_timeouts:         %u\n",
		       smi_get_stat(smi, long_timeouts));
	seq_printf(m, "idles:                 %u\n",
		       smi_get_stat(smi, idles));
	seq_printf(m, "interrupts:            %u\n",
		       smi_get_stat(smi, interrupts));
	seq_printf(m, "attentions:            %u\n",
		       smi_get_stat(smi, attentions));
	seq_printf(m, "flag_fetches:          %u\n",
		       smi_get_stat(smi, flag_fetches));
	seq_printf(m, "hosed_count:           %u\n",
		       smi_get_stat(smi, hosed_count));
	seq_printf(m, "complete_transactions: %u\n",
		       smi_get_stat(smi, complete_transactions));
	seq_printf(m, "events:                %u\n",
		       smi_get_stat(smi, events));
	seq_printf(m, "watchdog_pretimeouts:  %u\n",
		       smi_get_stat(smi, watchdog_pretimeouts));
	seq_printf(m, "incoming_messages:     %u\n",
		       smi_get_stat(smi, incoming_messages));
	return 0;
}

static int smi_si_stats_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, smi_si_stats_proc_show, PDE(inode)->data);
}

static const struct file_operations smi_si_stats_proc_ops = {
	.open		= smi_si_stats_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int smi_params_proc_show(struct seq_file *m, void *v)
{
	struct smi_info *smi = m->private;

	return seq_printf(m,
		       "%s,%s,0x%lx,rsp=%d,rsi=%d,rsh=%d,irq=%d,ipmb=%d\n",
		       si_to_str[smi->si_type],
		       addr_space_to_str[smi->io.addr_type],
		       smi->io.addr_data,
		       smi->io.regspacing,
		       smi->io.regsize,
		       smi->io.regshift,
		       smi->irq,
		       smi->slave_addr);
}

static int smi_params_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, smi_params_proc_show, PDE(inode)->data);
}

static const struct file_operations smi_params_proc_ops = {
	.open		= smi_params_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
=======
#define IPMI_SI_ATTR(name) \
static ssize_t name##_show(struct device *dev,			\
			   struct device_attribute *attr,		\
			   char *buf)					\
{									\
	struct smi_info *smi_info = dev_get_drvdata(dev);		\
									\
	return sysfs_emit(buf, "%u\n", smi_get_stat(smi_info, name));	\
}									\
static DEVICE_ATTR_RO(name)

static ssize_t type_show(struct device *dev,
			 struct device_attribute *attr,
			 char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%s\n", si_to_str[smi_info->io.si_type]);
}
static DEVICE_ATTR_RO(type);

static ssize_t interrupts_enabled_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);
	int enabled = smi_info->io.irq && !smi_info->interrupt_disabled;

	return sysfs_emit(buf, "%d\n", enabled);
}
static DEVICE_ATTR_RO(interrupts_enabled);

IPMI_SI_ATTR(short_timeouts);
IPMI_SI_ATTR(long_timeouts);
IPMI_SI_ATTR(idles);
IPMI_SI_ATTR(interrupts);
IPMI_SI_ATTR(attentions);
IPMI_SI_ATTR(flag_fetches);
IPMI_SI_ATTR(hosed_count);
IPMI_SI_ATTR(complete_transactions);
IPMI_SI_ATTR(events);
IPMI_SI_ATTR(watchdog_pretimeouts);
IPMI_SI_ATTR(incoming_messages);

static ssize_t params_show(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);

	return sysfs_emit(buf,
			"%s,%s,0x%lx,rsp=%d,rsi=%d,rsh=%d,irq=%d,ipmb=%d\n",
			si_to_str[smi_info->io.si_type],
			addr_space_to_str[smi_info->io.addr_space],
			smi_info->io.addr_data,
			smi_info->io.regspacing,
			smi_info->io.regsize,
			smi_info->io.regshift,
			smi_info->io.irq,
			smi_info->io.slave_addr);
}
static DEVICE_ATTR_RO(params);

static struct attribute *ipmi_si_dev_attrs[] = {
	&dev_attr_type.attr,
	&dev_attr_interrupts_enabled.attr,
	&dev_attr_short_timeouts.attr,
	&dev_attr_long_timeouts.attr,
	&dev_attr_idles.attr,
	&dev_attr_interrupts.attr,
	&dev_attr_attentions.attr,
	&dev_attr_flag_fetches.attr,
	&dev_attr_hosed_count.attr,
	&dev_attr_complete_transactions.attr,
	&dev_attr_events.attr,
	&dev_attr_watchdog_pretimeouts.attr,
	&dev_attr_incoming_messages.attr,
	&dev_attr_params.attr,
	NULL
};

static const struct attribute_group ipmi_si_dev_attr_group = {
	.attrs		= ipmi_si_dev_attrs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * oem_data_avail_to_receive_msg_avail
 * @info - smi_info structure with msg_flags set
 *
 * Converts flags from OEM_DATA_AVAIL to RECEIVE_MSG_AVAIL
 * Returns 1 indicating need to re-run handle_flags().
 */
static int oem_data_avail_to_receive_msg_avail(struct smi_info *smi_info)
{
	smi_info->msg_flags = ((smi_info->msg_flags & ~OEM_DATA_AVAIL) |
			       RECEIVE_MSG_AVAIL);
	return 1;
}

/*
 * setup_dell_poweredge_oem_data_handler
 * @info - smi_info.device_id must be populated
 *
 * Systems that match, but have firmware version < 1.40 may assert
 * OEM0_DATA_AVAIL on their own, without being told via Set Flags that
 * it's safe to do so.  Such systems will de-assert OEM1_DATA_AVAIL
 * upon receipt of IPMI_GET_MSG_CMD, so we should treat these flags
 * as RECEIVE_MSG_AVAIL instead.
 *
 * As Dell has no plans to release IPMI 1.5 firmware that *ever*
 * assert the OEM[012] bits, and if it did, the driver would have to
 * change to handle that properly, we don't actually check for the
 * firmware version.
 * Device ID = 0x20                BMC on PowerEdge 8G servers
 * Device Revision = 0x80
 * Firmware Revision1 = 0x01       BMC version 1.40
 * Firmware Revision2 = 0x40       BCD encoded
 * IPMI Version = 0x51             IPMI 1.5
 * Manufacturer ID = A2 02 00      Dell IANA
 *
 * Additionally, PowerEdge systems with IPMI < 1.5 may also assert
 * OEM0_DATA_AVAIL and needs to be treated as RECEIVE_MSG_AVAIL.
 *
 */
#define DELL_POWEREDGE_8G_BMC_DEVICE_ID  0x20
#define DELL_POWEREDGE_8G_BMC_DEVICE_REV 0x80
#define DELL_POWEREDGE_8G_BMC_IPMI_VERSION 0x51
#define DELL_IANA_MFR_ID 0x0002a2
static void setup_dell_poweredge_oem_data_handler(struct smi_info *smi_info)
{
	struct ipmi_device_id *id = &smi_info->device_id;
	if (id->manufacturer_id == DELL_IANA_MFR_ID) {
		if (id->device_id       == DELL_POWEREDGE_8G_BMC_DEVICE_ID  &&
		    id->device_revision == DELL_POWEREDGE_8G_BMC_DEVICE_REV &&
		    id->ipmi_version   == DELL_POWEREDGE_8G_BMC_IPMI_VERSION) {
			smi_info->oem_data_avail_handler =
				oem_data_avail_to_receive_msg_avail;
		} else if (ipmi_version_major(id) < 1 ||
			   (ipmi_version_major(id) == 1 &&
			    ipmi_version_minor(id) < 5)) {
			smi_info->oem_data_avail_handler =
				oem_data_avail_to_receive_msg_avail;
		}
	}
}

#define CANNOT_RETURN_REQUESTED_LENGTH 0xCA
static void return_hosed_msg_badsize(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg = smi_info->curr_msg;

	/* Make it a response */
	msg->rsp[0] = msg->data[0] | 4;
	msg->rsp[1] = msg->data[1];
	msg->rsp[2] = CANNOT_RETURN_REQUESTED_LENGTH;
	msg->rsp_size = 3;
	smi_info->curr_msg = NULL;
	deliver_recv_msg(smi_info, msg);
}

/*
 * dell_poweredge_bt_xaction_handler
 * @info - smi_info.device_id must be populated
 *
 * Dell PowerEdge servers with the BT interface (x6xx and 1750) will
 * not respond to a Get SDR command if the length of the data
 * requested is exactly 0x3A, which leads to command timeouts and no
 * data returned.  This intercepts such commands, and causes userspace
 * callers to try again with a different-sized buffer, which succeeds.
 */

#define STORAGE_NETFN 0x0A
#define STORAGE_CMD_GET_SDR 0x23
static int dell_poweredge_bt_xaction_handler(struct notifier_block *self,
					     unsigned long unused,
					     void *in)
{
	struct smi_info *smi_info = in;
	unsigned char *data = smi_info->curr_msg->data;
	unsigned int size   = smi_info->curr_msg->data_size;
	if (size >= 8 &&
	    (data[0]>>2) == STORAGE_NETFN &&
	    data[1] == STORAGE_CMD_GET_SDR &&
	    data[7] == 0x3A) {
		return_hosed_msg_badsize(smi_info);
		return NOTIFY_STOP;
	}
	return NOTIFY_DONE;
}

static struct notifier_block dell_poweredge_bt_xaction_notifier = {
	.notifier_call	= dell_poweredge_bt_xaction_handler,
};

/*
 * setup_dell_poweredge_bt_xaction_handler
 * @info - smi_info.device_id must be filled in already
 *
 * Fills in smi_info.device_id.start_transaction_pre_hook
 * when we know what function to use there.
 */
static void
setup_dell_poweredge_bt_xaction_handler(struct smi_info *smi_info)
{
	struct ipmi_device_id *id = &smi_info->device_id;
	if (id->manufacturer_id == DELL_IANA_MFR_ID &&
<<<<<<< HEAD
	    smi_info->si_type == SI_BT)
=======
	    smi_info->io.si_type == SI_BT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		register_xaction_notifier(&dell_poweredge_bt_xaction_notifier);
}

/*
 * setup_oem_data_handler
 * @info - smi_info.device_id must be filled in already
 *
 * Fills in smi_info.device_id.oem_data_available_handler
 * when we know what function to use there.
 */

static void setup_oem_data_handler(struct smi_info *smi_info)
{
	setup_dell_poweredge_oem_data_handler(smi_info);
}

static void setup_xaction_handlers(struct smi_info *smi_info)
{
	setup_dell_poweredge_bt_xaction_handler(smi_info);
}

<<<<<<< HEAD
static inline void wait_for_timer_and_thread(struct smi_info *smi_info)
{
	if (smi_info->intf) {
		/*
		 * The timer and thread are only running if the
		 * interface has been started up and registered.
		 */
		if (smi_info->thread != NULL)
			kthread_stop(smi_info->thread);
		del_timer_sync(&smi_info->si_timer);
	}
}

static __devinitdata struct ipmi_default_vals
{
	int type;
	int port;
} ipmi_defaults[] =
{
	{ .type = SI_KCS, .port = 0xca2 },
	{ .type = SI_SMIC, .port = 0xca9 },
	{ .type = SI_BT, .port = 0xe4 },
	{ .port = 0 }
};

static void __devinit default_find_bmc(void)
{
	struct smi_info *info;
	int             i;

	for (i = 0; ; i++) {
		if (!ipmi_defaults[i].port)
			break;
#ifdef CONFIG_PPC
		if (check_legacy_ioport(ipmi_defaults[i].port))
			continue;
#endif
		info = smi_info_alloc();
		if (!info)
			return;

		info->addr_source = SI_DEFAULT;

		info->si_type = ipmi_defaults[i].type;
		info->io_setup = port_setup;
		info->io.addr_data = ipmi_defaults[i].port;
		info->io.addr_type = IPMI_IO_ADDR_SPACE;

		info->io.addr = NULL;
		info->io.regspacing = DEFAULT_REGSPACING;
		info->io.regsize = DEFAULT_REGSPACING;
		info->io.regshift = 0;

		if (add_smi(info) == 0) {
			if ((try_smi_init(info)) == 0) {
				/* Found one... */
				printk(KERN_INFO PFX "Found default %s"
				" state machine at %s address 0x%lx\n",
				si_to_str[info->si_type],
				addr_space_to_str[info->io.addr_type],
				info->io.addr_data);
			} else
				cleanup_one_si(info);
		} else {
			kfree(info);
		}
	}
}

static int is_new_interface(struct smi_info *info)
=======
static void check_for_broken_irqs(struct smi_info *smi_info)
{
	check_clr_rcv_irq(smi_info);
	check_set_rcv_irq(smi_info);
}

static inline void stop_timer_and_thread(struct smi_info *smi_info)
{
	if (smi_info->thread != NULL) {
		kthread_stop(smi_info->thread);
		smi_info->thread = NULL;
	}

	smi_info->timer_can_start = false;
	del_timer_sync(&smi_info->si_timer);
}

static struct smi_info *find_dup_si(struct smi_info *info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct smi_info *e;

	list_for_each_entry(e, &smi_infos, link) {
<<<<<<< HEAD
		if (e->io.addr_type != info->io.addr_type)
			continue;
		if (e->io.addr_data == info->io.addr_data)
			return 0;
	}

	return 1;
}

static int add_smi(struct smi_info *new_smi)
{
	int rv = 0;

	printk(KERN_INFO PFX "Adding %s-specified %s state machine",
			ipmi_addr_src_to_str[new_smi->addr_source],
			si_to_str[new_smi->si_type]);
	mutex_lock(&smi_infos_lock);
	if (!is_new_interface(new_smi)) {
		printk(KERN_CONT " duplicate interface\n");
		rv = -EBUSY;
		goto out_err;
	}

	printk(KERN_CONT "\n");

	/* So we know not to free it unless we have allocated one. */
	new_smi->intf = NULL;
	new_smi->si_sm = NULL;
	new_smi->handlers = NULL;

	list_add_tail(&new_smi->link, &smi_infos);

=======
		if (e->io.addr_space != info->io.addr_space)
			continue;
		if (e->io.addr_data == info->io.addr_data) {
			/*
			 * This is a cheap hack, ACPI doesn't have a defined
			 * slave address but SMBIOS does.  Pick it up from
			 * any source that has it available.
			 */
			if (info->io.slave_addr && !e->io.slave_addr)
				e->io.slave_addr = info->io.slave_addr;
			return e;
		}
	}

	return NULL;
}

int ipmi_si_add_smi(struct si_sm_io *io)
{
	int rv = 0;
	struct smi_info *new_smi, *dup;

	/*
	 * If the user gave us a hard-coded device at the same
	 * address, they presumably want us to use it and not what is
	 * in the firmware.
	 */
	if (io->addr_source != SI_HARDCODED && io->addr_source != SI_HOTMOD &&
	    ipmi_si_hardcode_match(io->addr_space, io->addr_data)) {
		dev_info(io->dev,
			 "Hard-coded device at this address already exists");
		return -ENODEV;
	}

	if (!io->io_setup) {
		if (io->addr_space == IPMI_IO_ADDR_SPACE) {
			io->io_setup = ipmi_si_port_setup;
		} else if (io->addr_space == IPMI_MEM_ADDR_SPACE) {
			io->io_setup = ipmi_si_mem_setup;
		} else {
			return -EINVAL;
		}
	}

	new_smi = kzalloc(sizeof(*new_smi), GFP_KERNEL);
	if (!new_smi)
		return -ENOMEM;
	spin_lock_init(&new_smi->si_lock);

	new_smi->io = *io;

	mutex_lock(&smi_infos_lock);
	dup = find_dup_si(new_smi);
	if (dup) {
		if (new_smi->io.addr_source == SI_ACPI &&
		    dup->io.addr_source == SI_SMBIOS) {
			/* We prefer ACPI over SMBIOS. */
			dev_info(dup->io.dev,
				 "Removing SMBIOS-specified %s state machine in favor of ACPI\n",
				 si_to_str[new_smi->io.si_type]);
			cleanup_one_si(dup);
		} else {
			dev_info(new_smi->io.dev,
				 "%s-specified %s state machine: duplicate\n",
				 ipmi_addr_src_to_str(new_smi->io.addr_source),
				 si_to_str[new_smi->io.si_type]);
			rv = -EBUSY;
			kfree(new_smi);
			goto out_err;
		}
	}

	pr_info("Adding %s-specified %s state machine\n",
		ipmi_addr_src_to_str(new_smi->io.addr_source),
		si_to_str[new_smi->io.si_type]);

	list_add_tail(&new_smi->link, &smi_infos);

	if (initialized)
		rv = try_smi_init(new_smi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_err:
	mutex_unlock(&smi_infos_lock);
	return rv;
}

<<<<<<< HEAD
=======
/*
 * Try to start up an interface.  Must be called with smi_infos_lock
 * held, primarily to keep smi_num consistent, we only one to do these
 * one at a time.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int try_smi_init(struct smi_info *new_smi)
{
	int rv = 0;
	int i;

<<<<<<< HEAD
	printk(KERN_INFO PFX "Trying %s-specified %s state"
	       " machine at %s address 0x%lx, slave address 0x%x,"
	       " irq %d\n",
	       ipmi_addr_src_to_str[new_smi->addr_source],
	       si_to_str[new_smi->si_type],
	       addr_space_to_str[new_smi->io.addr_type],
	       new_smi->io.addr_data,
	       new_smi->slave_addr, new_smi->irq);

	switch (new_smi->si_type) {
=======
	pr_info("Trying %s-specified %s state machine at %s address 0x%lx, slave address 0x%x, irq %d\n",
		ipmi_addr_src_to_str(new_smi->io.addr_source),
		si_to_str[new_smi->io.si_type],
		addr_space_to_str[new_smi->io.addr_space],
		new_smi->io.addr_data,
		new_smi->io.slave_addr, new_smi->io.irq);

	switch (new_smi->io.si_type) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case SI_KCS:
		new_smi->handlers = &kcs_smi_handlers;
		break;

	case SI_SMIC:
		new_smi->handlers = &smic_smi_handlers;
		break;

	case SI_BT:
		new_smi->handlers = &bt_smi_handlers;
		break;

	default:
		/* No support for anything else yet. */
		rv = -EIO;
		goto out_err;
	}

<<<<<<< HEAD
	/* Allocate the state machine's data and initialize it. */
	new_smi->si_sm = kmalloc(new_smi->handlers->size(), GFP_KERNEL);
	if (!new_smi->si_sm) {
		printk(KERN_ERR PFX
		       "Could not allocate state machine memory\n");
		rv = -ENOMEM;
		goto out_err;
	}
	new_smi->io_size = new_smi->handlers->init_data(new_smi->si_sm,
							&new_smi->io);

	/* Now that we know the I/O size, we can set up the I/O. */
	rv = new_smi->io_setup(new_smi);
	if (rv) {
		printk(KERN_ERR PFX "Could not set up I/O space\n");
=======
	new_smi->si_num = smi_num;

	/* Do this early so it's available for logs. */
	if (!new_smi->io.dev) {
		pr_err("IPMI interface added with no device\n");
		rv = -EIO;
		goto out_err;
	}

	/* Allocate the state machine's data and initialize it. */
	new_smi->si_sm = kmalloc(new_smi->handlers->size(), GFP_KERNEL);
	if (!new_smi->si_sm) {
		rv = -ENOMEM;
		goto out_err;
	}
	new_smi->io.io_size = new_smi->handlers->init_data(new_smi->si_sm,
							   &new_smi->io);

	/* Now that we know the I/O size, we can set up the I/O. */
	rv = new_smi->io.io_setup(&new_smi->io);
	if (rv) {
		dev_err(new_smi->io.dev, "Could not set up I/O space\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_err;
	}

	/* Do low-level detection first. */
	if (new_smi->handlers->detect(new_smi->si_sm)) {
<<<<<<< HEAD
		if (new_smi->addr_source)
			printk(KERN_INFO PFX "Interface detection failed\n");
=======
		if (new_smi->io.addr_source)
			dev_err(new_smi->io.dev,
				"Interface detection failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rv = -ENODEV;
		goto out_err;
	}

	/*
	 * Attempt a get device id command.  If it fails, we probably
	 * don't have a BMC here.
	 */
	rv = try_get_dev_id(new_smi);
	if (rv) {
<<<<<<< HEAD
		if (new_smi->addr_source)
			printk(KERN_INFO PFX "There appears to be no BMC"
			       " at this location\n");
=======
		if (new_smi->io.addr_source)
			dev_err(new_smi->io.dev,
			       "There appears to be no BMC at this location\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_err;
	}

	setup_oem_data_handler(new_smi);
	setup_xaction_handlers(new_smi);
<<<<<<< HEAD

	INIT_LIST_HEAD(&(new_smi->xmit_msgs));
	INIT_LIST_HEAD(&(new_smi->hp_xmit_msgs));
	new_smi->curr_msg = NULL;
	atomic_set(&new_smi->req_events, 0);
	new_smi->run_to_completion = 0;
	for (i = 0; i < SI_NUM_STATS; i++)
		atomic_set(&new_smi->stats[i], 0);

	new_smi->interrupt_disabled = 1;
	atomic_set(&new_smi->stop_operation, 0);
	new_smi->intf_num = smi_num;
	smi_num++;

	rv = try_enable_event_buffer(new_smi);
	if (rv == 0)
		new_smi->has_event_buffer = 1;
=======
	check_for_broken_irqs(new_smi);

	new_smi->waiting_msg = NULL;
	new_smi->curr_msg = NULL;
	atomic_set(&new_smi->req_events, 0);
	new_smi->run_to_completion = false;
	for (i = 0; i < SI_NUM_STATS; i++)
		atomic_set(&new_smi->stats[i], 0);

	new_smi->interrupt_disabled = true;
	atomic_set(&new_smi->need_watch, 0);

	rv = try_enable_event_buffer(new_smi);
	if (rv == 0)
		new_smi->has_event_buffer = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Start clearing the flags before we enable interrupts or the
	 * timer to avoid racing with the timer.
	 */
	start_clear_flags(new_smi);
<<<<<<< HEAD
	/* IRQ is defined to be set when non-zero. */
	if (new_smi->irq)
		new_smi->si_state = SI_CLEARING_FLAGS_THEN_SET_IRQ;

	if (!new_smi->dev) {
		/*
		 * If we don't already have a device from something
		 * else (like PCI), then register a new one.
		 */
		new_smi->pdev = platform_device_alloc("ipmi_si",
						      new_smi->intf_num);
		if (!new_smi->pdev) {
			printk(KERN_ERR PFX
			       "Unable to allocate platform device\n");
			goto out_err;
		}
		new_smi->dev = &new_smi->pdev->dev;
		new_smi->dev->driver = &ipmi_driver.driver;

		rv = platform_device_add(new_smi->pdev);
		if (rv) {
			printk(KERN_ERR PFX
			       "Unable to register system interface device:"
			       " %d\n",
			       rv);
			goto out_err;
		}
		new_smi->dev_registered = 1;
	}

	rv = ipmi_register_smi(&handlers,
			       new_smi,
			       &new_smi->device_id,
			       new_smi->dev,
			       "bmc",
			       new_smi->slave_addr);
	if (rv) {
		dev_err(new_smi->dev, "Unable to register device: error %d\n",
			rv);
		goto out_err_stop_timer;
	}

	rv = ipmi_smi_add_proc_entry(new_smi->intf, "type",
				     &smi_type_proc_ops,
				     new_smi);
	if (rv) {
		dev_err(new_smi->dev, "Unable to create proc entry: %d\n", rv);
		goto out_err_stop_timer;
	}

	rv = ipmi_smi_add_proc_entry(new_smi->intf, "si_stats",
				     &smi_si_stats_proc_ops,
				     new_smi);
	if (rv) {
		dev_err(new_smi->dev, "Unable to create proc entry: %d\n", rv);
		goto out_err_stop_timer;
	}

	rv = ipmi_smi_add_proc_entry(new_smi->intf, "params",
				     &smi_params_proc_ops,
				     new_smi);
	if (rv) {
		dev_err(new_smi->dev, "Unable to create proc entry: %d\n", rv);
		goto out_err_stop_timer;
	}

	dev_info(new_smi->dev, "IPMI %s interface initialized\n",
		 si_to_str[new_smi->si_type]);

	return 0;

 out_err_stop_timer:
	atomic_inc(&new_smi->stop_operation);
	wait_for_timer_and_thread(new_smi);

 out_err:
	new_smi->interrupt_disabled = 1;

	if (new_smi->intf) {
		ipmi_unregister_smi(new_smi->intf);
		new_smi->intf = NULL;
	}

	if (new_smi->irq_cleanup) {
		new_smi->irq_cleanup(new_smi);
		new_smi->irq_cleanup = NULL;
	}

	/*
	 * Wait until we know that we are out of any interrupt
	 * handlers might have been running before we freed the
	 * interrupt.
	 */
	synchronize_sched();

	if (new_smi->si_sm) {
		if (new_smi->handlers)
			new_smi->handlers->cleanup(new_smi->si_sm);
		kfree(new_smi->si_sm);
		new_smi->si_sm = NULL;
	}
	if (new_smi->addr_source_cleanup) {
		new_smi->addr_source_cleanup(new_smi);
		new_smi->addr_source_cleanup = NULL;
	}
	if (new_smi->io_cleanup) {
		new_smi->io_cleanup(new_smi);
		new_smi->io_cleanup = NULL;
	}

	if (new_smi->dev_registered) {
		platform_device_unregister(new_smi->pdev);
		new_smi->dev_registered = 0;
	}
=======

	/*
	 * IRQ is defined to be set when non-zero.  req_events will
	 * cause a global flags check that will enable interrupts.
	 */
	if (new_smi->io.irq) {
		new_smi->interrupt_disabled = false;
		atomic_set(&new_smi->req_events, 1);
	}

	dev_set_drvdata(new_smi->io.dev, new_smi);
	rv = device_add_group(new_smi->io.dev, &ipmi_si_dev_attr_group);
	if (rv) {
		dev_err(new_smi->io.dev,
			"Unable to add device attributes: error %d\n",
			rv);
		goto out_err;
	}
	new_smi->dev_group_added = true;

	rv = ipmi_register_smi(&handlers,
			       new_smi,
			       new_smi->io.dev,
			       new_smi->io.slave_addr);
	if (rv) {
		dev_err(new_smi->io.dev,
			"Unable to register device: error %d\n",
			rv);
		goto out_err;
	}

	/* Don't increment till we know we have succeeded. */
	smi_num++;

	dev_info(new_smi->io.dev, "IPMI %s interface initialized\n",
		 si_to_str[new_smi->io.si_type]);

	WARN_ON(new_smi->io.dev->init_name != NULL);

 out_err:
	if (rv && new_smi->io.io_cleanup) {
		new_smi->io.io_cleanup(&new_smi->io);
		new_smi->io.io_cleanup = NULL;
	}

	if (rv && new_smi->si_sm) {
		kfree(new_smi->si_sm);
		new_smi->si_sm = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rv;
}

<<<<<<< HEAD
static int __devinit init_ipmi_si(void)
{
	int  i;
	char *str;
	int  rv;
=======
static int __init init_ipmi_si(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct smi_info *e;
	enum ipmi_addr_src type = SI_INVALID;

	if (initialized)
		return 0;
<<<<<<< HEAD
	initialized = 1;

	rv = platform_driver_register(&ipmi_driver);
	if (rv) {
		printk(KERN_ERR PFX "Unable to register driver: %d\n", rv);
		return rv;
	}


	/* Parse out the si_type string into its components. */
	str = si_type_str;
	if (*str != '\0') {
		for (i = 0; (i < SI_MAX_PARMS) && (*str != '\0'); i++) {
			si_type[i] = str;
			str = strchr(str, ',');
			if (str) {
				*str = '\0';
				str++;
			} else {
				break;
			}
		}
	}

	printk(KERN_INFO "IPMI System Interface driver.\n");

	/* If the user gave us a device, they presumably want us to use it */
	if (!hardcode_find_bmc())
		return 0;

#ifdef CONFIG_PCI
	rv = pci_register_driver(&ipmi_pci_driver);
	if (rv)
		printk(KERN_ERR PFX "Unable to register PCI driver: %d\n", rv);
	else
		pci_registered = 1;
#endif

#ifdef CONFIG_ACPI
	pnp_register_driver(&ipmi_pnp_driver);
	pnp_registered = 1;
#endif

#ifdef CONFIG_DMI
	dmi_find_bmc();
#endif

#ifdef CONFIG_ACPI
	spmi_find_bmc();
#endif
=======

	ipmi_hardcode_init();

	pr_info("IPMI System Interface driver\n");

	ipmi_si_platform_init();

	ipmi_si_pci_init();

	ipmi_si_parisc_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We prefer devices with interrupts, but in the case of a machine
	   with multiple BMCs we assume that there will be several instances
	   of a given type so if we succeed in registering a type then also
	   try to register everything else of the same type */
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&smi_infos_lock);
	list_for_each_entry(e, &smi_infos, link) {
		/* Try to register a device if it has an IRQ and we either
		   haven't successfully registered a device yet or this
		   device has the same type as one we successfully registered */
<<<<<<< HEAD
		if (e->irq && (!type || e->addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->addr_source;
=======
		if (e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

	/* type will only have been set if we successfully registered an si */
<<<<<<< HEAD
	if (type) {
		mutex_unlock(&smi_infos_lock);
		return 0;
	}
=======
	if (type)
		goto skip_fallback_noirq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Fall back to the preferred device */

	list_for_each_entry(e, &smi_infos, link) {
<<<<<<< HEAD
		if (!e->irq && (!type || e->addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->addr_source;
			}
		}
	}
=======
		if (!e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
			}
		}
	}

skip_fallback_noirq:
	initialized = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&smi_infos_lock);

	if (type)
		return 0;

<<<<<<< HEAD
	if (si_trydefaults) {
		mutex_lock(&smi_infos_lock);
		if (list_empty(&smi_infos)) {
			/* No BMC was found, try defaults. */
			mutex_unlock(&smi_infos_lock);
			default_find_bmc();
		} else
			mutex_unlock(&smi_infos_lock);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&smi_infos_lock);
	if (unload_when_empty && list_empty(&smi_infos)) {
		mutex_unlock(&smi_infos_lock);
		cleanup_ipmi_si();
<<<<<<< HEAD
		printk(KERN_WARNING PFX
		       "Unable to find any System Interface(s)\n");
=======
		pr_warn("Unable to find any System Interface(s)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	} else {
		mutex_unlock(&smi_infos_lock);
		return 0;
	}
}
module_init(init_ipmi_si);

<<<<<<< HEAD
static void cleanup_one_si(struct smi_info *to_clean)
{
	int           rv = 0;
	unsigned long flags;

	if (!to_clean)
		return;

	list_del(&to_clean->link);

	/* Tell the driver that we are shutting down. */
	atomic_inc(&to_clean->stop_operation);

	/*
	 * Make sure the timer and thread are stopped and will not run
	 * again.
	 */
	wait_for_timer_and_thread(to_clean);

	/*
	 * Timeouts are stopped, now make sure the interrupts are off
	 * for the device.  A little tricky with locks to make sure
	 * there are no races.
	 */
	spin_lock_irqsave(&to_clean->si_lock, flags);
	while (to_clean->curr_msg || (to_clean->si_state != SI_NORMAL)) {
		spin_unlock_irqrestore(&to_clean->si_lock, flags);
		poll(to_clean);
		schedule_timeout_uninterruptible(1);
		spin_lock_irqsave(&to_clean->si_lock, flags);
	}
	disable_si_irq(to_clean);
	spin_unlock_irqrestore(&to_clean->si_lock, flags);
	while (to_clean->curr_msg || (to_clean->si_state != SI_NORMAL)) {
		poll(to_clean);
		schedule_timeout_uninterruptible(1);
	}

	/* Clean up interrupts and make sure that everything is done. */
	if (to_clean->irq_cleanup)
		to_clean->irq_cleanup(to_clean);
	while (to_clean->curr_msg || (to_clean->si_state != SI_NORMAL)) {
		poll(to_clean);
		schedule_timeout_uninterruptible(1);
	}

	if (to_clean->intf)
		rv = ipmi_unregister_smi(to_clean->intf);

	if (rv) {
		printk(KERN_ERR PFX "Unable to unregister device: errno=%d\n",
		       rv);
	}

	if (to_clean->handlers)
		to_clean->handlers->cleanup(to_clean->si_sm);

	kfree(to_clean->si_sm);

	if (to_clean->addr_source_cleanup)
		to_clean->addr_source_cleanup(to_clean);
	if (to_clean->io_cleanup)
		to_clean->io_cleanup(to_clean);

	if (to_clean->dev_registered)
		platform_device_unregister(to_clean->pdev);

	kfree(to_clean);
=======
static void wait_msg_processed(struct smi_info *smi_info)
{
	unsigned long jiffies_now;
	long time_diff;

	while (smi_info->curr_msg || (smi_info->si_state != SI_NORMAL)) {
		jiffies_now = jiffies;
		time_diff = (((long)jiffies_now - (long)smi_info->last_timeout_jiffies)
		     * SI_USEC_PER_JIFFY);
		smi_event_handler(smi_info, time_diff);
		schedule_timeout_uninterruptible(1);
	}
}

static void shutdown_smi(void *send_info)
{
	struct smi_info *smi_info = send_info;

	if (smi_info->dev_group_added) {
		device_remove_group(smi_info->io.dev, &ipmi_si_dev_attr_group);
		smi_info->dev_group_added = false;
	}
	if (smi_info->io.dev)
		dev_set_drvdata(smi_info->io.dev, NULL);

	/*
	 * Make sure that interrupts, the timer and the thread are
	 * stopped and will not run again.
	 */
	smi_info->interrupt_disabled = true;
	if (smi_info->io.irq_cleanup) {
		smi_info->io.irq_cleanup(&smi_info->io);
		smi_info->io.irq_cleanup = NULL;
	}
	stop_timer_and_thread(smi_info);

	/*
	 * Wait until we know that we are out of any interrupt
	 * handlers might have been running before we freed the
	 * interrupt.
	 */
	synchronize_rcu();

	/*
	 * Timeouts are stopped, now make sure the interrupts are off
	 * in the BMC.  Note that timers and CPU interrupts are off,
	 * so no need for locks.
	 */
	wait_msg_processed(smi_info);

	if (smi_info->handlers)
		disable_si_irq(smi_info);

	wait_msg_processed(smi_info);

	if (smi_info->handlers)
		smi_info->handlers->cleanup(smi_info->si_sm);

	if (smi_info->io.io_cleanup) {
		smi_info->io.io_cleanup(&smi_info->io);
		smi_info->io.io_cleanup = NULL;
	}

	kfree(smi_info->si_sm);
	smi_info->si_sm = NULL;

	smi_info->intf = NULL;
}

/*
 * Must be called with smi_infos_lock held, to serialize the
 * smi_info->intf check.
 */
static void cleanup_one_si(struct smi_info *smi_info)
{
	if (!smi_info)
		return;

	list_del(&smi_info->link);
	ipmi_unregister_smi(smi_info->intf);
	kfree(smi_info);
}

void ipmi_si_remove_by_dev(struct device *dev)
{
	struct smi_info *e;

	mutex_lock(&smi_infos_lock);
	list_for_each_entry(e, &smi_infos, link) {
		if (e->io.dev == dev) {
			cleanup_one_si(e);
			break;
		}
	}
	mutex_unlock(&smi_infos_lock);
}

struct device *ipmi_si_remove_by_data(int addr_space, enum si_type si_type,
				      unsigned long addr)
{
	/* remove */
	struct smi_info *e, *tmp_e;
	struct device *dev = NULL;

	mutex_lock(&smi_infos_lock);
	list_for_each_entry_safe(e, tmp_e, &smi_infos, link) {
		if (e->io.addr_space != addr_space)
			continue;
		if (e->io.si_type != si_type)
			continue;
		if (e->io.addr_data == addr) {
			dev = get_device(e->io.dev);
			cleanup_one_si(e);
		}
	}
	mutex_unlock(&smi_infos_lock);

	return dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cleanup_ipmi_si(void)
{
	struct smi_info *e, *tmp_e;

	if (!initialized)
		return;

<<<<<<< HEAD
#ifdef CONFIG_PCI
	if (pci_registered)
		pci_unregister_driver(&ipmi_pci_driver);
#endif
#ifdef CONFIG_ACPI
	if (pnp_registered)
		pnp_unregister_driver(&ipmi_pnp_driver);
#endif

	platform_driver_unregister(&ipmi_driver);
=======
	ipmi_si_pci_shutdown();

	ipmi_si_parisc_shutdown();

	ipmi_si_platform_shutdown();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&smi_infos_lock);
	list_for_each_entry_safe(e, tmp_e, &smi_infos, link)
		cleanup_one_si(e);
	mutex_unlock(&smi_infos_lock);
<<<<<<< HEAD
}
module_exit(cleanup_ipmi_si);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Corey Minyard <minyard@mvista.com>");
MODULE_DESCRIPTION("Interface to the IPMI driver for the KCS, SMIC, and BT"
		   " system interfaces.");
=======

	ipmi_si_hardcode_exit();
	ipmi_si_hotmod_exit();
}
module_exit(cleanup_ipmi_si);

MODULE_ALIAS("platform:dmi-ipmi-si");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Corey Minyard <minyard@mvista.com>");
MODULE_DESCRIPTION("Interface to the IPMI driver for the KCS, SMIC, and BT system interfaces.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
