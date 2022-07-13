<<<<<<< HEAD
/*
 * linux/drivers/s390/crypto/ap_bus.c
 *
 * Copyright (C) 2006 IBM Corporation
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright IBM Corp. 2006, 2023
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author(s): Cornelia Huck <cornelia.huck@de.ibm.com>
 *	      Martin Schwidefsky <schwidefsky@de.ibm.com>
 *	      Ralph Wuerthner <rwuerthn@de.ibm.com>
 *	      Felix Beck <felix.beck@de.ibm.com>
 *	      Holger Dengler <hd@linux.vnet.ibm.com>
<<<<<<< HEAD
 *
 * Adjunct processor bus.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
 *	      Harald Freudenberger <freude@linux.ibm.com>
 *
 * Adjunct processor bus.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define KMSG_COMPONENT "ap"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel_stat.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/err.h>
=======
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/freezer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/notifier.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <asm/reset.h>
#include <asm/airq.h>
=======
#include <asm/airq.h>
#include <asm/tpi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/atomic.h>
#include <asm/isc.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <asm/facility.h>
<<<<<<< HEAD

#include "ap_bus.h"

/* Some prototypes. */
static void ap_scan_bus(struct work_struct *);
static void ap_poll_all(unsigned long);
static enum hrtimer_restart ap_poll_timeout(struct hrtimer *);
static int ap_poll_thread_start(void);
static void ap_poll_thread_stop(void);
static void ap_request_timeout(unsigned long);
static inline void ap_schedule_poll_timer(void);
static int __ap_poll_device(struct ap_device *ap_dev, unsigned long *flags);
static int ap_device_remove(struct device *dev);
static int ap_device_probe(struct device *dev);
static void ap_interrupt_handler(void *unused1, void *unused2);
static void ap_reset(struct ap_device *ap_dev);
static void ap_config_timeout(unsigned long ptr);
static int ap_select_domain(void);

/*
 * Module description.
 */
MODULE_AUTHOR("IBM Corporation");
MODULE_DESCRIPTION("Adjunct Processor Bus driver, "
		   "Copyright 2006 IBM Corporation");
MODULE_LICENSE("GPL");

/*
 * Module parameter
 */
int ap_domain_index = -1;	/* Adjunct Processor Domain Index */
module_param_named(domain, ap_domain_index, int, 0000);
MODULE_PARM_DESC(domain, "domain index for ap devices");
EXPORT_SYMBOL(ap_domain_index);

static int ap_thread_flag = 0;
module_param_named(poll_thread, ap_thread_flag, int, 0000);
MODULE_PARM_DESC(poll_thread, "Turn on/off poll thread, default is 0 (off).");

static struct device *ap_root_device = NULL;
static DEFINE_SPINLOCK(ap_device_list_lock);
static LIST_HEAD(ap_device_list);

/*
 * Workqueue & timer for bus rescan.
 */
static struct workqueue_struct *ap_work_queue;
static struct timer_list ap_config_timer;
static int ap_config_time = AP_CONFIG_TIME;
static DECLARE_WORK(ap_config_work, ap_scan_bus);
=======
#include <linux/crypto.h>
#include <linux/mod_devicetable.h>
#include <linux/debugfs.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <asm/uv.h>

#include "ap_bus.h"
#include "ap_debug.h"

/*
 * Module parameters; note though this file itself isn't modular.
 */
int ap_domain_index = -1;	/* Adjunct Processor Domain Index */
static DEFINE_SPINLOCK(ap_domain_lock);
module_param_named(domain, ap_domain_index, int, 0440);
MODULE_PARM_DESC(domain, "domain index for ap devices");
EXPORT_SYMBOL(ap_domain_index);

static int ap_thread_flag;
module_param_named(poll_thread, ap_thread_flag, int, 0440);
MODULE_PARM_DESC(poll_thread, "Turn on/off poll thread, default is 0 (off).");

static char *apm_str;
module_param_named(apmask, apm_str, charp, 0440);
MODULE_PARM_DESC(apmask, "AP bus adapter mask.");

static char *aqm_str;
module_param_named(aqmask, aqm_str, charp, 0440);
MODULE_PARM_DESC(aqmask, "AP bus domain mask.");

static int ap_useirq = 1;
module_param_named(useirq, ap_useirq, int, 0440);
MODULE_PARM_DESC(useirq, "Use interrupt if available, default is 1 (on).");

atomic_t ap_max_msg_size = ATOMIC_INIT(AP_DEFAULT_MAX_MSG_SIZE);
EXPORT_SYMBOL(ap_max_msg_size);

static struct device *ap_root_device;

/* Hashtable of all queue devices on the AP bus */
DEFINE_HASHTABLE(ap_queues, 8);
/* lock used for the ap_queues hashtable */
DEFINE_SPINLOCK(ap_queues_lock);

/* Default permissions (ioctl, card and domain masking) */
struct ap_perms ap_perms;
EXPORT_SYMBOL(ap_perms);
DEFINE_MUTEX(ap_perms_mutex);
EXPORT_SYMBOL(ap_perms_mutex);

/* # of bindings complete since init */
static atomic64_t ap_bindings_complete_count = ATOMIC64_INIT(0);

/* completion for APQN bindings complete */
static DECLARE_COMPLETION(ap_apqn_bindings_complete);

static struct ap_config_info *ap_qci_info;
static struct ap_config_info *ap_qci_info_old;

/*
 * AP bus related debug feature things.
 */
debug_info_t *ap_dbf_info;

/*
 * AP bus rescan related things.
 */
static bool ap_scan_bus(void);
static bool ap_scan_bus_result; /* result of last ap_scan_bus() */
static DEFINE_MUTEX(ap_scan_bus_mutex); /* mutex ap_scan_bus() invocations */
static atomic64_t ap_scan_bus_count; /* counter ap_scan_bus() invocations */
static int ap_scan_bus_time = AP_CONFIG_TIME;
static struct timer_list ap_scan_bus_timer;
static void ap_scan_bus_wq_callback(struct work_struct *);
static DECLARE_WORK(ap_scan_bus_work, ap_scan_bus_wq_callback);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Tasklet & timer for AP request polling and interrupts
 */
<<<<<<< HEAD
static DECLARE_TASKLET(ap_tasklet, ap_poll_all, 0);
static atomic_t ap_poll_requests = ATOMIC_INIT(0);
static DECLARE_WAIT_QUEUE_HEAD(ap_poll_wait);
static struct task_struct *ap_poll_kthread = NULL;
static DEFINE_MUTEX(ap_poll_thread_mutex);
static DEFINE_SPINLOCK(ap_poll_timer_lock);
static void *ap_interrupt_indicator;
static struct hrtimer ap_poll_timer;
/* In LPAR poll with 4kHz frequency. Poll every 250000 nanoseconds.
 * If z/VM change to 1500000 nanoseconds to adjust to z/VM polling.*/
static unsigned long long poll_timeout = 250000;

/* Suspend flag */
static int ap_suspend_flag;
/* Flag to check if domain was set through module parameter domain=. This is
 * important when supsend and resume is done in a z/VM environment where the
 * domain might change. */
static int user_set_domain = 0;
static struct bus_type ap_bus_type;

/**
 * ap_using_interrupts() - Returns non-zero if interrupt support is
 * available.
 */
static inline int ap_using_interrupts(void)
{
	return ap_interrupt_indicator != NULL;
}

/**
 * ap_intructions_available() - Test if AP instructions are available.
 *
 * Returns 0 if the AP instructions are installed.
 */
static inline int ap_instructions_available(void)
{
	register unsigned long reg0 asm ("0") = AP_MKQID(0,0);
	register unsigned long reg1 asm ("1") = -ENODEV;
	register unsigned long reg2 asm ("2") = 0UL;

	asm volatile(
		"   .long 0xb2af0000\n"		/* PQAP(TAPQ) */
		"0: la    %1,0\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		: "+d" (reg0), "+d" (reg1), "+d" (reg2) : : "cc" );
	return reg1;
=======
static void ap_tasklet_fn(unsigned long);
static DECLARE_TASKLET_OLD(ap_tasklet, ap_tasklet_fn);
static DECLARE_WAIT_QUEUE_HEAD(ap_poll_wait);
static struct task_struct *ap_poll_kthread;
static DEFINE_MUTEX(ap_poll_thread_mutex);
static DEFINE_SPINLOCK(ap_poll_timer_lock);
static struct hrtimer ap_poll_timer;
/*
 * In LPAR poll with 4kHz frequency. Poll every 250000 nanoseconds.
 * If z/VM change to 1500000 nanoseconds to adjust to z/VM polling.
 */
static unsigned long poll_high_timeout = 250000UL;

/*
 * Some state machine states only require a low frequency polling.
 * We use 25 Hz frequency for these.
 */
static unsigned long poll_low_timeout = 40000000UL;

/* Maximum domain id, if not given via qci */
static int ap_max_domain_id = 15;
/* Maximum adapter id, if not given via qci */
static int ap_max_adapter_id = 63;

static const struct bus_type ap_bus_type;

/* Adapter interrupt definitions */
static void ap_interrupt_handler(struct airq_struct *airq,
				 struct tpi_info *tpi_info);

static bool ap_irq_flag;

static struct airq_struct ap_airq = {
	.handler = ap_interrupt_handler,
	.isc = AP_ISC,
};

/**
 * ap_airq_ptr() - Get the address of the adapter interrupt indicator
 *
 * Returns the address of the local-summary-indicator of the adapter
 * interrupt handler for AP, or NULL if adapter interrupts are not
 * available.
 */
void *ap_airq_ptr(void)
{
	if (ap_irq_flag)
		return ap_airq.lsi_ptr;
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ap_interrupts_available(): Test if AP interrupts are available.
 *
 * Returns 1 if AP interrupts are available.
 */
static int ap_interrupts_available(void)
{
<<<<<<< HEAD
	return test_facility(2) && test_facility(65);
}

/**
 * ap_test_queue(): Test adjunct processor queue.
 * @qid: The AP queue number
 * @queue_depth: Pointer to queue depth value
 * @device_type: Pointer to device type value
 *
 * Returns AP queue status structure.
 */
static inline struct ap_queue_status
ap_test_queue(ap_qid_t qid, int *queue_depth, int *device_type)
{
	register unsigned long reg0 asm ("0") = qid;
	register struct ap_queue_status reg1 asm ("1");
	register unsigned long reg2 asm ("2") = 0UL;

	asm volatile(".long 0xb2af0000"		/* PQAP(TAPQ) */
		     : "+d" (reg0), "=d" (reg1), "+d" (reg2) : : "cc");
	*device_type = (int) (reg2 >> 24);
	*queue_depth = (int) (reg2 & 0xff);
	return reg1;
}

/**
 * ap_reset_queue(): Reset adjunct processor queue.
 * @qid: The AP queue number
 *
 * Returns AP queue status structure.
 */
static inline struct ap_queue_status ap_reset_queue(ap_qid_t qid)
{
	register unsigned long reg0 asm ("0") = qid | 0x01000000UL;
	register struct ap_queue_status reg1 asm ("1");
	register unsigned long reg2 asm ("2") = 0UL;

	asm volatile(
		".long 0xb2af0000"		/* PQAP(RAPQ) */
		: "+d" (reg0), "=d" (reg1), "+d" (reg2) : : "cc");
	return reg1;
}

#ifdef CONFIG_64BIT
/**
 * ap_queue_interruption_control(): Enable interruption for a specific AP.
 * @qid: The AP queue number
 * @ind: The notification indicator byte
 *
 * Returns AP queue status.
 */
static inline struct ap_queue_status
ap_queue_interruption_control(ap_qid_t qid, void *ind)
{
	register unsigned long reg0 asm ("0") = qid | 0x03000000UL;
	register unsigned long reg1_in asm ("1") = 0x0000800000000000UL | AP_ISC;
	register struct ap_queue_status reg1_out asm ("1");
	register void *reg2 asm ("2") = ind;
	asm volatile(
		".long 0xb2af0000"		/* PQAP(RAPQ) */
		: "+d" (reg0), "+d" (reg1_in), "=d" (reg1_out), "+d" (reg2)
		:
		: "cc" );
	return reg1_out;
}
#endif

#ifdef CONFIG_64BIT
static inline struct ap_queue_status
__ap_query_functions(ap_qid_t qid, unsigned int *functions)
{
	register unsigned long reg0 asm ("0") = 0UL | qid | (1UL << 23);
	register struct ap_queue_status reg1 asm ("1") = AP_QUEUE_STATUS_INVALID;
	register unsigned long reg2 asm ("2");

	asm volatile(
		".long 0xb2af0000\n"
		"0:\n"
		EX_TABLE(0b, 0b)
		: "+d" (reg0), "+d" (reg1), "=d" (reg2)
		:
		: "cc");

	*functions = (unsigned int)(reg2 >> 32);
	return reg1;
}
#endif

/**
 * ap_query_functions(): Query supported functions.
 * @qid: The AP queue number
 * @functions: Pointer to functions field.
 *
 * Returns
 *   0	     on success.
 *   -ENODEV  if queue not valid.
 *   -EBUSY   if device busy.
 *   -EINVAL  if query function is not supported
 */
static int ap_query_functions(ap_qid_t qid, unsigned int *functions)
{
#ifdef CONFIG_64BIT
	struct ap_queue_status status;
	int i;
	status = __ap_query_functions(qid, functions);

	for (i = 0; i < AP_MAX_RESET; i++) {
		if (ap_queue_status_invalid_test(&status))
			return -ENODEV;

		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			return 0;
		case AP_RESPONSE_RESET_IN_PROGRESS:
		case AP_RESPONSE_BUSY:
			break;
		case AP_RESPONSE_Q_NOT_AVAIL:
		case AP_RESPONSE_DECONFIGURED:
		case AP_RESPONSE_CHECKSTOPPED:
		case AP_RESPONSE_INVALID_ADDRESS:
			return -ENODEV;
		case AP_RESPONSE_OTHERWISE_CHANGED:
			break;
		default:
			break;
		}
		if (i < AP_MAX_RESET - 1) {
			udelay(5);
			status = __ap_query_functions(qid, functions);
		}
	}
	return -EBUSY;
#else
	return -EINVAL;
#endif
}

/**
 * ap_4096_commands_availablen(): Check for availability of 4096 bit RSA
 * support.
 * @qid: The AP queue number
 *
 * Returns 1 if 4096 bit RSA keys are support fo the AP, returns 0 if not.
 */
int ap_4096_commands_available(ap_qid_t qid)
{
	unsigned int functions;

	if (ap_query_functions(qid, &functions))
		return 0;

	return test_ap_facility(functions, 1) &&
	       test_ap_facility(functions, 2);
}
EXPORT_SYMBOL(ap_4096_commands_available);

/**
 * ap_queue_enable_interruption(): Enable interruption on an AP.
 * @qid: The AP queue number
 * @ind: the notification indicator byte
 *
 * Enables interruption on AP queue via ap_queue_interruption_control(). Based
 * on the return value it waits a while and tests the AP queue if interrupts
 * have been switched on using ap_test_queue().
 */
static int ap_queue_enable_interruption(ap_qid_t qid, void *ind)
{
#ifdef CONFIG_64BIT
	struct ap_queue_status status;
	int t_depth, t_device_type, rc, i;

	rc = -EBUSY;
	status = ap_queue_interruption_control(qid, ind);

	for (i = 0; i < AP_MAX_RESET; i++) {
		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			if (status.int_enabled)
				return 0;
			break;
		case AP_RESPONSE_RESET_IN_PROGRESS:
		case AP_RESPONSE_BUSY:
			break;
		case AP_RESPONSE_Q_NOT_AVAIL:
		case AP_RESPONSE_DECONFIGURED:
		case AP_RESPONSE_CHECKSTOPPED:
		case AP_RESPONSE_INVALID_ADDRESS:
			return -ENODEV;
		case AP_RESPONSE_OTHERWISE_CHANGED:
			if (status.int_enabled)
				return 0;
			break;
		default:
			break;
		}
		if (i < AP_MAX_RESET - 1) {
			udelay(5);
			status = ap_test_queue(qid, &t_depth, &t_device_type);
		}
	}
	return rc;
#else
	return -EINVAL;
#endif
}

/**
 * __ap_send(): Send message to adjunct processor queue.
 * @qid: The AP queue number
 * @psmid: The program supplied message identifier
 * @msg: The message text
 * @length: The message length
 * @special: Special Bit
 *
 * Returns AP queue status structure.
 * Condition code 1 on NQAP can't happen because the L bit is 1.
 * Condition code 2 on NQAP also means the send is incomplete,
 * because a segment boundary was reached. The NQAP is repeated.
 */
static inline struct ap_queue_status
__ap_send(ap_qid_t qid, unsigned long long psmid, void *msg, size_t length,
	  unsigned int special)
{
	typedef struct { char _[length]; } msgblock;
	register unsigned long reg0 asm ("0") = qid | 0x40000000UL;
	register struct ap_queue_status reg1 asm ("1");
	register unsigned long reg2 asm ("2") = (unsigned long) msg;
	register unsigned long reg3 asm ("3") = (unsigned long) length;
	register unsigned long reg4 asm ("4") = (unsigned int) (psmid >> 32);
	register unsigned long reg5 asm ("5") = (unsigned int) psmid;

	if (special == 1)
		reg0 |= 0x400000UL;

	asm volatile (
		"0: .long 0xb2ad0042\n"		/* DQAP */
		"   brc   2,0b"
		: "+d" (reg0), "=d" (reg1), "+d" (reg2), "+d" (reg3)
		: "d" (reg4), "d" (reg5), "m" (*(msgblock *) msg)
		: "cc" );
	return reg1;
}

int ap_send(ap_qid_t qid, unsigned long long psmid, void *msg, size_t length)
{
	struct ap_queue_status status;

	status = __ap_send(qid, psmid, msg, length, 0);
	switch (status.response_code) {
	case AP_RESPONSE_NORMAL:
		return 0;
	case AP_RESPONSE_Q_FULL:
	case AP_RESPONSE_RESET_IN_PROGRESS:
		return -EBUSY;
	case AP_RESPONSE_REQ_FAC_NOT_INST:
		return -EINVAL;
	default:	/* Device is gone. */
		return -ENODEV;
	}
}
EXPORT_SYMBOL(ap_send);

/**
 * __ap_recv(): Receive message from adjunct processor queue.
 * @qid: The AP queue number
 * @psmid: Pointer to program supplied message identifier
 * @msg: The message text
 * @length: The message length
 *
 * Returns AP queue status structure.
 * Condition code 1 on DQAP means the receive has taken place
 * but only partially.	The response is incomplete, hence the
 * DQAP is repeated.
 * Condition code 2 on DQAP also means the receive is incomplete,
 * this time because a segment boundary was reached. Again, the
 * DQAP is repeated.
 * Note that gpr2 is used by the DQAP instruction to keep track of
 * any 'residual' length, in case the instruction gets interrupted.
 * Hence it gets zeroed before the instruction.
 */
static inline struct ap_queue_status
__ap_recv(ap_qid_t qid, unsigned long long *psmid, void *msg, size_t length)
{
	typedef struct { char _[length]; } msgblock;
	register unsigned long reg0 asm("0") = qid | 0x80000000UL;
	register struct ap_queue_status reg1 asm ("1");
	register unsigned long reg2 asm("2") = 0UL;
	register unsigned long reg4 asm("4") = (unsigned long) msg;
	register unsigned long reg5 asm("5") = (unsigned long) length;
	register unsigned long reg6 asm("6") = 0UL;
	register unsigned long reg7 asm("7") = 0UL;


	asm volatile(
		"0: .long 0xb2ae0064\n"
		"   brc   6,0b\n"
		: "+d" (reg0), "=d" (reg1), "+d" (reg2),
		"+d" (reg4), "+d" (reg5), "+d" (reg6), "+d" (reg7),
		"=m" (*(msgblock *) msg) : : "cc" );
	*psmid = (((unsigned long long) reg6) << 32) + reg7;
	return reg1;
}

int ap_recv(ap_qid_t qid, unsigned long long *psmid, void *msg, size_t length)
{
	struct ap_queue_status status;

	status = __ap_recv(qid, psmid, msg, length);
	switch (status.response_code) {
	case AP_RESPONSE_NORMAL:
		return 0;
	case AP_RESPONSE_NO_PENDING_REPLY:
		if (status.queue_empty)
			return -ENOENT;
		return -EBUSY;
	case AP_RESPONSE_RESET_IN_PROGRESS:
		return -EBUSY;
	default:
		return -ENODEV;
	}
}
EXPORT_SYMBOL(ap_recv);

/**
 * ap_query_queue(): Check if an AP queue is available.
 * @qid: The AP queue number
 * @queue_depth: Pointer to queue depth value
 * @device_type: Pointer to device type value
 *
 * The test is repeated for AP_MAX_RESET times.
 */
static int ap_query_queue(ap_qid_t qid, int *queue_depth, int *device_type)
{
	struct ap_queue_status status;
	int t_depth, t_device_type, rc, i;

	rc = -EBUSY;
	for (i = 0; i < AP_MAX_RESET; i++) {
		status = ap_test_queue(qid, &t_depth, &t_device_type);
		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			*queue_depth = t_depth + 1;
			*device_type = t_device_type;
			rc = 0;
			break;
		case AP_RESPONSE_Q_NOT_AVAIL:
			rc = -ENODEV;
			break;
		case AP_RESPONSE_RESET_IN_PROGRESS:
			break;
		case AP_RESPONSE_DECONFIGURED:
			rc = -ENODEV;
			break;
		case AP_RESPONSE_CHECKSTOPPED:
			rc = -ENODEV;
			break;
		case AP_RESPONSE_INVALID_ADDRESS:
			rc = -ENODEV;
			break;
		case AP_RESPONSE_OTHERWISE_CHANGED:
			break;
		case AP_RESPONSE_BUSY:
			break;
		default:
			BUG();
		}
		if (rc != -EBUSY)
			break;
		if (i < AP_MAX_RESET - 1)
			udelay(5);
	}
	return rc;
}

/**
 * ap_init_queue(): Reset an AP queue.
 * @qid: The AP queue number
 *
 * Reset an AP queue and wait for it to become available again.
 */
static int ap_init_queue(ap_qid_t qid)
{
	struct ap_queue_status status;
	int rc, dummy, i;

	rc = -ENODEV;
	status = ap_reset_queue(qid);
	for (i = 0; i < AP_MAX_RESET; i++) {
		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			if (status.queue_empty)
				rc = 0;
			break;
		case AP_RESPONSE_Q_NOT_AVAIL:
		case AP_RESPONSE_DECONFIGURED:
		case AP_RESPONSE_CHECKSTOPPED:
			i = AP_MAX_RESET;	/* return with -ENODEV */
			break;
		case AP_RESPONSE_RESET_IN_PROGRESS:
			rc = -EBUSY;
		case AP_RESPONSE_BUSY:
		default:
			break;
		}
		if (rc != -ENODEV && rc != -EBUSY)
			break;
		if (i < AP_MAX_RESET - 1) {
			udelay(5);
			status = ap_test_queue(qid, &dummy, &dummy);
		}
	}
	if (rc == 0 && ap_using_interrupts()) {
		rc = ap_queue_enable_interruption(qid, ap_interrupt_indicator);
		/* If interruption mode is supported by the machine,
		* but an AP can not be enabled for interruption then
		* the AP will be discarded.    */
		if (rc)
			pr_err("Registering adapter interrupts for "
			       "AP %d failed\n", AP_QID_DEVICE(qid));
	}
	return rc;
}

/**
 * ap_increase_queue_count(): Arm request timeout.
 * @ap_dev: Pointer to an AP device.
 *
 * Arm request timeout if an AP device was idle and a new request is submitted.
 */
static void ap_increase_queue_count(struct ap_device *ap_dev)
{
	int timeout = ap_dev->drv->request_timeout;

	ap_dev->queue_count++;
	if (ap_dev->queue_count == 1) {
		mod_timer(&ap_dev->timeout, jiffies + timeout);
		ap_dev->reset = AP_RESET_ARMED;
	}
}

/**
 * ap_decrease_queue_count(): Decrease queue count.
 * @ap_dev: Pointer to an AP device.
 *
 * If AP device is still alive, re-schedule request timeout if there are still
 * pending requests.
 */
static void ap_decrease_queue_count(struct ap_device *ap_dev)
{
	int timeout = ap_dev->drv->request_timeout;

	ap_dev->queue_count--;
	if (ap_dev->queue_count > 0)
		mod_timer(&ap_dev->timeout, jiffies + timeout);
	else
		/*
		 * The timeout timer should to be disabled now - since
		 * del_timer_sync() is very expensive, we just tell via the
		 * reset flag to ignore the pending timeout timer.
		 */
		ap_dev->reset = AP_RESET_IGNORE;
}

/*
 * AP device related attributes.
 */
static ssize_t ap_hwtype_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", ap_dev->device_type);
}

static DEVICE_ATTR(hwtype, 0444, ap_hwtype_show, NULL);
static ssize_t ap_depth_show(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", ap_dev->queue_depth);
}

static DEVICE_ATTR(depth, 0444, ap_depth_show, NULL);
static ssize_t ap_request_count_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	int rc;

	spin_lock_bh(&ap_dev->lock);
	rc = snprintf(buf, PAGE_SIZE, "%d\n", ap_dev->total_request_count);
	spin_unlock_bh(&ap_dev->lock);
	return rc;
}

static DEVICE_ATTR(request_count, 0444, ap_request_count_show, NULL);

static ssize_t ap_modalias_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "ap:t%02X", to_ap_dev(dev)->device_type);
}

static DEVICE_ATTR(modalias, 0444, ap_modalias_show, NULL);

static struct attribute *ap_dev_attrs[] = {
	&dev_attr_hwtype.attr,
	&dev_attr_depth.attr,
	&dev_attr_request_count.attr,
	&dev_attr_modalias.attr,
	NULL
};
static struct attribute_group ap_dev_attr_group = {
	.attrs = ap_dev_attrs
};

/**
 * ap_bus_match()
 * @dev: Pointer to device
 * @drv: Pointer to device_driver
 *
 * AP bus driver registration/unregistration.
 */
static int ap_bus_match(struct device *dev, struct device_driver *drv)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	struct ap_driver *ap_drv = to_ap_drv(drv);
	struct ap_device_id *id;

	/*
	 * Compare device type of the device with the list of
	 * supported types of the device_driver.
	 */
	for (id = ap_drv->ids; id->match_flags; id++) {
		if ((id->match_flags & AP_DEVICE_ID_MATCH_DEVICE_TYPE) &&
		    (id->dev_type != ap_dev->device_type))
			continue;
		return 1;
	}
	return 0;
}

/**
 * ap_uevent(): Uevent function for AP devices.
 * @dev: Pointer to device
 * @env: Pointer to kobj_uevent_env
 *
 * It sets up a single environment variable DEV_TYPE which contains the
 * hardware device type.
 */
static int ap_uevent (struct device *dev, struct kobj_uevent_env *env)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	int retval = 0;

	if (!ap_dev)
		return -ENODEV;

	/* Set up DEV_TYPE environment variable. */
	retval = add_uevent_var(env, "DEV_TYPE=%04X", ap_dev->device_type);
	if (retval)
		return retval;

	/* Add MODALIAS= */
	retval = add_uevent_var(env, "MODALIAS=ap:t%02X", ap_dev->device_type);

	return retval;
}

static int ap_bus_suspend(struct device *dev, pm_message_t state)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	unsigned long flags;

	if (!ap_suspend_flag) {
		ap_suspend_flag = 1;

		/* Disable scanning for devices, thus we do not want to scan
		 * for them after removing.
		 */
		del_timer_sync(&ap_config_timer);
		if (ap_work_queue != NULL) {
			destroy_workqueue(ap_work_queue);
			ap_work_queue = NULL;
		}

		tasklet_disable(&ap_tasklet);
	}
	/* Poll on the device until all requests are finished. */
	do {
		flags = 0;
		spin_lock_bh(&ap_dev->lock);
		__ap_poll_device(ap_dev, &flags);
		spin_unlock_bh(&ap_dev->lock);
	} while ((flags & 1) || (flags & 2));

	spin_lock_bh(&ap_dev->lock);
	ap_dev->unregistered = 1;
	spin_unlock_bh(&ap_dev->lock);

	return 0;
}

static int ap_bus_resume(struct device *dev)
{
	int rc = 0;
	struct ap_device *ap_dev = to_ap_dev(dev);

	if (ap_suspend_flag) {
		ap_suspend_flag = 0;
		if (!ap_interrupts_available())
			ap_interrupt_indicator = NULL;
		if (!user_set_domain) {
			ap_domain_index = -1;
			ap_select_domain();
		}
		init_timer(&ap_config_timer);
		ap_config_timer.function = ap_config_timeout;
		ap_config_timer.data = 0;
		ap_config_timer.expires = jiffies + ap_config_time * HZ;
		add_timer(&ap_config_timer);
		ap_work_queue = create_singlethread_workqueue("kapwork");
		if (!ap_work_queue)
			return -ENOMEM;
		tasklet_enable(&ap_tasklet);
		if (!ap_using_interrupts())
			ap_schedule_poll_timer();
		else
			tasklet_schedule(&ap_tasklet);
		if (ap_thread_flag)
			rc = ap_poll_thread_start();
	}
	if (AP_QID_QUEUE(ap_dev->qid) != ap_domain_index) {
		spin_lock_bh(&ap_dev->lock);
		ap_dev->qid = AP_MKQID(AP_QID_DEVICE(ap_dev->qid),
				       ap_domain_index);
		spin_unlock_bh(&ap_dev->lock);
	}
	queue_work(ap_work_queue, &ap_config_work);

	return rc;
}

static struct bus_type ap_bus_type = {
	.name = "ap",
	.match = &ap_bus_match,
	.uevent = &ap_uevent,
	.suspend = ap_bus_suspend,
	.resume = ap_bus_resume
};

static int ap_device_probe(struct device *dev)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	struct ap_driver *ap_drv = to_ap_drv(dev->driver);
	int rc;

	ap_dev->drv = ap_drv;
	rc = ap_drv->probe ? ap_drv->probe(ap_dev) : -ENODEV;
	if (!rc) {
		spin_lock_bh(&ap_device_list_lock);
		list_add(&ap_dev->list, &ap_device_list);
		spin_unlock_bh(&ap_device_list_lock);
	}
	return rc;
}

/**
 * __ap_flush_queue(): Flush requests.
 * @ap_dev: Pointer to the AP device
 *
 * Flush all requests from the request/pending queue of an AP device.
 */
static void __ap_flush_queue(struct ap_device *ap_dev)
{
	struct ap_message *ap_msg, *next;

	list_for_each_entry_safe(ap_msg, next, &ap_dev->pendingq, list) {
		list_del_init(&ap_msg->list);
		ap_dev->pendingq_count--;
		ap_dev->drv->receive(ap_dev, ap_msg, ERR_PTR(-ENODEV));
	}
	list_for_each_entry_safe(ap_msg, next, &ap_dev->requestq, list) {
		list_del_init(&ap_msg->list);
		ap_dev->requestq_count--;
		ap_dev->drv->receive(ap_dev, ap_msg, ERR_PTR(-ENODEV));
	}
}

void ap_flush_queue(struct ap_device *ap_dev)
{
	spin_lock_bh(&ap_dev->lock);
	__ap_flush_queue(ap_dev);
	spin_unlock_bh(&ap_dev->lock);
}
EXPORT_SYMBOL(ap_flush_queue);

static int ap_device_remove(struct device *dev)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	struct ap_driver *ap_drv = ap_dev->drv;

	ap_flush_queue(ap_dev);
	del_timer_sync(&ap_dev->timeout);
	spin_lock_bh(&ap_device_list_lock);
	list_del_init(&ap_dev->list);
	spin_unlock_bh(&ap_device_list_lock);
	if (ap_drv->remove)
		ap_drv->remove(ap_dev);
	spin_lock_bh(&ap_dev->lock);
	atomic_sub(ap_dev->queue_count, &ap_poll_requests);
	spin_unlock_bh(&ap_dev->lock);
	return 0;
}

int ap_driver_register(struct ap_driver *ap_drv, struct module *owner,
		       char *name)
{
	struct device_driver *drv = &ap_drv->driver;

	drv->bus = &ap_bus_type;
	drv->probe = ap_device_probe;
	drv->remove = ap_device_remove;
	drv->owner = owner;
	drv->name = name;
	return driver_register(drv);
}
EXPORT_SYMBOL(ap_driver_register);

void ap_driver_unregister(struct ap_driver *ap_drv)
{
	driver_unregister(&ap_drv->driver);
}
EXPORT_SYMBOL(ap_driver_unregister);

/*
 * AP bus attributes.
 */
static ssize_t ap_domain_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ap_domain_index);
}

static BUS_ATTR(ap_domain, 0444, ap_domain_show, NULL);

static ssize_t ap_config_time_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ap_config_time);
}

static ssize_t ap_interrupts_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n",
			ap_using_interrupts() ? 1 : 0);
}

static BUS_ATTR(ap_interrupts, 0444, ap_interrupts_show, NULL);

static ssize_t ap_config_time_store(struct bus_type *bus,
				    const char *buf, size_t count)
{
	int time;

	if (sscanf(buf, "%d\n", &time) != 1 || time < 5 || time > 120)
		return -EINVAL;
	ap_config_time = time;
	if (!timer_pending(&ap_config_timer) ||
	    !mod_timer(&ap_config_timer, jiffies + ap_config_time * HZ)) {
		ap_config_timer.expires = jiffies + ap_config_time * HZ;
		add_timer(&ap_config_timer);
	}
	return count;
}

static BUS_ATTR(config_time, 0644, ap_config_time_show, ap_config_time_store);

static ssize_t ap_poll_thread_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ap_poll_kthread ? 1 : 0);
}

static ssize_t ap_poll_thread_store(struct bus_type *bus,
				    const char *buf, size_t count)
{
	int flag, rc;

	if (sscanf(buf, "%d\n", &flag) != 1)
		return -EINVAL;
	if (flag) {
		rc = ap_poll_thread_start();
		if (rc)
			return rc;
	}
	else
		ap_poll_thread_stop();
	return count;
}

static BUS_ATTR(poll_thread, 0644, ap_poll_thread_show, ap_poll_thread_store);

static ssize_t poll_timeout_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%llu\n", poll_timeout);
}

static ssize_t poll_timeout_store(struct bus_type *bus, const char *buf,
				  size_t count)
{
	unsigned long long time;
	ktime_t hr_time;

	/* 120 seconds = maximum poll interval */
	if (sscanf(buf, "%llu\n", &time) != 1 || time < 1 ||
	    time > 120000000000ULL)
		return -EINVAL;
	poll_timeout = time;
	hr_time = ktime_set(0, poll_timeout);

	if (!hrtimer_is_queued(&ap_poll_timer) ||
	    !hrtimer_forward(&ap_poll_timer, hrtimer_get_expires(&ap_poll_timer), hr_time)) {
		hrtimer_set_expires(&ap_poll_timer, hr_time);
		hrtimer_start_expires(&ap_poll_timer, HRTIMER_MODE_ABS);
	}
	return count;
}

static BUS_ATTR(poll_timeout, 0644, poll_timeout_show, poll_timeout_store);

static struct bus_attribute *const ap_bus_attrs[] = {
	&bus_attr_ap_domain,
	&bus_attr_config_time,
	&bus_attr_poll_thread,
	&bus_attr_ap_interrupts,
	&bus_attr_poll_timeout,
	NULL,
};

/**
 * ap_select_domain(): Select an AP domain.
 *
 * Pick one of the 16 AP domains.
 */
static int ap_select_domain(void)
{
	int queue_depth, device_type, count, max_count, best_domain;
	int rc, i, j;

	/*
	 * We want to use a single domain. Either the one specified with
	 * the "domain=" parameter or the domain with the maximum number
	 * of devices.
	 */
	if (ap_domain_index >= 0 && ap_domain_index < AP_DOMAINS)
		/* Domain has already been selected. */
		return 0;
	best_domain = -1;
	max_count = 0;
	for (i = 0; i < AP_DOMAINS; i++) {
		count = 0;
		for (j = 0; j < AP_DEVICES; j++) {
			ap_qid_t qid = AP_MKQID(j, i);
			rc = ap_query_queue(qid, &queue_depth, &device_type);
			if (rc)
				continue;
			count++;
		}
		if (count > max_count) {
			max_count = count;
			best_domain = i;
		}
	}
	if (best_domain >= 0){
		ap_domain_index = best_domain;
		return 0;
	}
	return -ENODEV;
}

/**
 * ap_probe_device_type(): Find the device type of an AP.
 * @ap_dev: pointer to the AP device.
 *
 * Find the device type if query queue returned a device type of 0.
 */
static int ap_probe_device_type(struct ap_device *ap_dev)
{
	static unsigned char msg[] = {
		0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x58,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x01,0x00,0x43,0x43,0x41,0x2d,0x41,0x50,
		0x50,0x4c,0x20,0x20,0x20,0x01,0x01,0x01,
		0x00,0x00,0x00,0x00,0x50,0x4b,0x00,0x00,
		0x00,0x00,0x01,0x1c,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x05,0xb8,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x70,0x00,0x41,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x54,0x32,0x01,0x00,0xa0,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0xb8,0x05,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,
		0x49,0x43,0x53,0x46,0x20,0x20,0x20,0x20,
		0x50,0x4b,0x0a,0x00,0x50,0x4b,0x43,0x53,
		0x2d,0x31,0x2e,0x32,0x37,0x00,0x11,0x22,
		0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00,
		0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,
		0x99,0x00,0x11,0x22,0x33,0x44,0x55,0x66,
		0x77,0x88,0x99,0x00,0x11,0x22,0x33,0x44,
		0x55,0x66,0x77,0x88,0x99,0x00,0x11,0x22,
		0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00,
		0x11,0x22,0x33,0x5d,0x00,0x5b,0x00,0x77,
		0x88,0x1e,0x00,0x00,0x57,0x00,0x00,0x00,
		0x00,0x04,0x00,0x00,0x4f,0x00,0x00,0x00,
		0x03,0x02,0x00,0x00,0x40,0x01,0x00,0x01,
		0xce,0x02,0x68,0x2d,0x5f,0xa9,0xde,0x0c,
		0xf6,0xd2,0x7b,0x58,0x4b,0xf9,0x28,0x68,
		0x3d,0xb4,0xf4,0xef,0x78,0xd5,0xbe,0x66,
		0x63,0x42,0xef,0xf8,0xfd,0xa4,0xf8,0xb0,
		0x8e,0x29,0xc2,0xc9,0x2e,0xd8,0x45,0xb8,
		0x53,0x8c,0x6f,0x4e,0x72,0x8f,0x6c,0x04,
		0x9c,0x88,0xfc,0x1e,0xc5,0x83,0x55,0x57,
		0xf7,0xdd,0xfd,0x4f,0x11,0x36,0x95,0x5d,
	};
	struct ap_queue_status status;
	unsigned long long psmid;
	char *reply;
	int rc, i;

	reply = (void *) get_zeroed_page(GFP_KERNEL);
	if (!reply) {
		rc = -ENOMEM;
		goto out;
	}

	status = __ap_send(ap_dev->qid, 0x0102030405060708ULL,
			   msg, sizeof(msg), 0);
	if (status.response_code != AP_RESPONSE_NORMAL) {
		rc = -ENODEV;
		goto out_free;
	}

	/* Wait for the test message to complete. */
	for (i = 0; i < 6; i++) {
		mdelay(300);
		status = __ap_recv(ap_dev->qid, &psmid, reply, 4096);
		if (status.response_code == AP_RESPONSE_NORMAL &&
		    psmid == 0x0102030405060708ULL)
			break;
	}
	if (i < 6) {
		/* Got an answer. */
		if (reply[0] == 0x00 && reply[1] == 0x86)
			ap_dev->device_type = AP_DEVICE_TYPE_PCICC;
		else
			ap_dev->device_type = AP_DEVICE_TYPE_PCICA;
		rc = 0;
	} else
		rc = -ENODEV;

out_free:
	free_page((unsigned long) reply);
out:
	return rc;
}

static void ap_interrupt_handler(void *unused1, void *unused2)
{
	kstat_cpu(smp_processor_id()).irqs[IOINT_APB]++;
	tasklet_schedule(&ap_tasklet);
}

/**
 * __ap_scan_bus(): Scan the AP bus.
 * @dev: Pointer to device
 * @data: Pointer to data
 *
 * Scan the AP bus for new devices.
 */
static int __ap_scan_bus(struct device *dev, void *data)
{
	return to_ap_dev(dev)->qid == (ap_qid_t)(unsigned long) data;
}

static void ap_device_release(struct device *dev)
{
	struct ap_device *ap_dev = to_ap_dev(dev);

	kfree(ap_dev);
}

static void ap_scan_bus(struct work_struct *unused)
{
	struct ap_device *ap_dev;
	struct device *dev;
	ap_qid_t qid;
	int queue_depth, device_type;
	unsigned int device_functions;
	int rc, i;

	if (ap_select_domain() != 0)
		return;
	for (i = 0; i < AP_DEVICES; i++) {
		qid = AP_MKQID(i, ap_domain_index);
		dev = bus_find_device(&ap_bus_type, NULL,
				      (void *)(unsigned long)qid,
				      __ap_scan_bus);
		rc = ap_query_queue(qid, &queue_depth, &device_type);
		if (dev) {
			if (rc == -EBUSY) {
				set_current_state(TASK_UNINTERRUPTIBLE);
				schedule_timeout(AP_RESET_TIMEOUT);
				rc = ap_query_queue(qid, &queue_depth,
						    &device_type);
			}
			ap_dev = to_ap_dev(dev);
			spin_lock_bh(&ap_dev->lock);
			if (rc || ap_dev->unregistered) {
				spin_unlock_bh(&ap_dev->lock);
				if (ap_dev->unregistered)
					i--;
				device_unregister(dev);
				put_device(dev);
				continue;
			}
			spin_unlock_bh(&ap_dev->lock);
			put_device(dev);
			continue;
		}
		if (rc)
			continue;
		rc = ap_init_queue(qid);
		if (rc)
			continue;
		ap_dev = kzalloc(sizeof(*ap_dev), GFP_KERNEL);
		if (!ap_dev)
			break;
		ap_dev->qid = qid;
		ap_dev->queue_depth = queue_depth;
		ap_dev->unregistered = 1;
		spin_lock_init(&ap_dev->lock);
		INIT_LIST_HEAD(&ap_dev->pendingq);
		INIT_LIST_HEAD(&ap_dev->requestq);
		INIT_LIST_HEAD(&ap_dev->list);
		setup_timer(&ap_dev->timeout, ap_request_timeout,
			    (unsigned long) ap_dev);
		switch (device_type) {
		case 0:
			if (ap_probe_device_type(ap_dev)) {
				kfree(ap_dev);
				continue;
			}
			break;
		case 10:
			if (ap_query_functions(qid, &device_functions)) {
				kfree(ap_dev);
				continue;
			}
			if (test_ap_facility(device_functions, 3))
				ap_dev->device_type = AP_DEVICE_TYPE_CEX3C;
			else if (test_ap_facility(device_functions, 4))
				ap_dev->device_type = AP_DEVICE_TYPE_CEX3A;
			else {
				kfree(ap_dev);
				continue;
			}
			break;
		default:
			ap_dev->device_type = device_type;
		}

		ap_dev->device.bus = &ap_bus_type;
		ap_dev->device.parent = ap_root_device;
		if (dev_set_name(&ap_dev->device, "card%02x",
				 AP_QID_DEVICE(ap_dev->qid))) {
			kfree(ap_dev);
			continue;
		}
		ap_dev->device.release = ap_device_release;
		rc = device_register(&ap_dev->device);
		if (rc) {
			put_device(&ap_dev->device);
			continue;
		}
		/* Add device attributes. */
		rc = sysfs_create_group(&ap_dev->device.kobj,
					&ap_dev_attr_group);
		if (!rc) {
			spin_lock_bh(&ap_dev->lock);
			ap_dev->unregistered = 0;
			spin_unlock_bh(&ap_dev->lock);
		}
		else
			device_unregister(&ap_dev->device);
	}
}

static void
ap_config_timeout(unsigned long ptr)
{
	queue_work(ap_work_queue, &ap_config_work);
	ap_config_timer.expires = jiffies + ap_config_time * HZ;
	add_timer(&ap_config_timer);
}

/**
 * __ap_schedule_poll_timer(): Schedule poll timer.
 *
 * Set up the timer to run the poll tasklet
 */
static inline void __ap_schedule_poll_timer(void)
{
	ktime_t hr_time;

	spin_lock_bh(&ap_poll_timer_lock);
	if (hrtimer_is_queued(&ap_poll_timer) || ap_suspend_flag)
		goto out;
	if (ktime_to_ns(hrtimer_expires_remaining(&ap_poll_timer)) <= 0) {
		hr_time = ktime_set(0, poll_timeout);
		hrtimer_forward_now(&ap_poll_timer, hr_time);
		hrtimer_restart(&ap_poll_timer);
	}
out:
	spin_unlock_bh(&ap_poll_timer_lock);
}

/**
 * ap_schedule_poll_timer(): Schedule poll timer.
 *
 * Set up the timer to run the poll tasklet
 */
static inline void ap_schedule_poll_timer(void)
{
	if (ap_using_interrupts())
		return;
	__ap_schedule_poll_timer();
}

/**
 * ap_poll_read(): Receive pending reply messages from an AP device.
 * @ap_dev: pointer to the AP device
 * @flags: pointer to control flags, bit 2^0 is set if another poll is
 *	   required, bit 2^1 is set if the poll timer needs to get armed
 *
 * Returns 0 if the device is still present, -ENODEV if not.
 */
static int ap_poll_read(struct ap_device *ap_dev, unsigned long *flags)
{
	struct ap_queue_status status;
	struct ap_message *ap_msg;

	if (ap_dev->queue_count <= 0)
		return 0;
	status = __ap_recv(ap_dev->qid, &ap_dev->reply->psmid,
			   ap_dev->reply->message, ap_dev->reply->length);
	switch (status.response_code) {
	case AP_RESPONSE_NORMAL:
		atomic_dec(&ap_poll_requests);
		ap_decrease_queue_count(ap_dev);
		list_for_each_entry(ap_msg, &ap_dev->pendingq, list) {
			if (ap_msg->psmid != ap_dev->reply->psmid)
				continue;
			list_del_init(&ap_msg->list);
			ap_dev->pendingq_count--;
			ap_dev->drv->receive(ap_dev, ap_msg, ap_dev->reply);
			break;
		}
		if (ap_dev->queue_count > 0)
			*flags |= 1;
		break;
	case AP_RESPONSE_NO_PENDING_REPLY:
		if (status.queue_empty) {
			/* The card shouldn't forget requests but who knows. */
			atomic_sub(ap_dev->queue_count, &ap_poll_requests);
			ap_dev->queue_count = 0;
			list_splice_init(&ap_dev->pendingq, &ap_dev->requestq);
			ap_dev->requestq_count += ap_dev->pendingq_count;
			ap_dev->pendingq_count = 0;
		} else
			*flags |= 2;
		break;
	default:
		return -ENODEV;
	}
	return 0;
}

/**
 * ap_poll_write(): Send messages from the request queue to an AP device.
 * @ap_dev: pointer to the AP device
 * @flags: pointer to control flags, bit 2^0 is set if another poll is
 *	   required, bit 2^1 is set if the poll timer needs to get armed
 *
 * Returns 0 if the device is still present, -ENODEV if not.
 */
static int ap_poll_write(struct ap_device *ap_dev, unsigned long *flags)
{
	struct ap_queue_status status;
	struct ap_message *ap_msg;

	if (ap_dev->requestq_count <= 0 ||
	    ap_dev->queue_count >= ap_dev->queue_depth)
		return 0;
	/* Start the next request on the queue. */
	ap_msg = list_entry(ap_dev->requestq.next, struct ap_message, list);
	status = __ap_send(ap_dev->qid, ap_msg->psmid,
			   ap_msg->message, ap_msg->length, ap_msg->special);
	switch (status.response_code) {
	case AP_RESPONSE_NORMAL:
		atomic_inc(&ap_poll_requests);
		ap_increase_queue_count(ap_dev);
		list_move_tail(&ap_msg->list, &ap_dev->pendingq);
		ap_dev->requestq_count--;
		ap_dev->pendingq_count++;
		if (ap_dev->queue_count < ap_dev->queue_depth &&
		    ap_dev->requestq_count > 0)
			*flags |= 1;
		*flags |= 2;
		break;
	case AP_RESPONSE_RESET_IN_PROGRESS:
		__ap_schedule_poll_timer();
	case AP_RESPONSE_Q_FULL:
		*flags |= 2;
		break;
	case AP_RESPONSE_MESSAGE_TOO_BIG:
	case AP_RESPONSE_REQ_FAC_NOT_INST:
		return -EINVAL;
	default:
		return -ENODEV;
	}
	return 0;
}

/**
 * ap_poll_queue(): Poll AP device for pending replies and send new messages.
 * @ap_dev: pointer to the bus device
 * @flags: pointer to control flags, bit 2^0 is set if another poll is
 *	   required, bit 2^1 is set if the poll timer needs to get armed
 *
 * Poll AP device for pending replies and send new messages. If either
 * ap_poll_read or ap_poll_write returns -ENODEV unregister the device.
 * Returns 0.
 */
static inline int ap_poll_queue(struct ap_device *ap_dev, unsigned long *flags)
{
	int rc;

	rc = ap_poll_read(ap_dev, flags);
	if (rc)
		return rc;
	return ap_poll_write(ap_dev, flags);
}

/**
 * __ap_queue_message(): Queue a message to a device.
 * @ap_dev: pointer to the AP device
 * @ap_msg: the message to be queued
 *
 * Queue a message to a device. Returns 0 if successful.
 */
static int __ap_queue_message(struct ap_device *ap_dev, struct ap_message *ap_msg)
{
	struct ap_queue_status status;

	if (list_empty(&ap_dev->requestq) &&
	    ap_dev->queue_count < ap_dev->queue_depth) {
		status = __ap_send(ap_dev->qid, ap_msg->psmid,
				   ap_msg->message, ap_msg->length,
				   ap_msg->special);
		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			list_add_tail(&ap_msg->list, &ap_dev->pendingq);
			atomic_inc(&ap_poll_requests);
			ap_dev->pendingq_count++;
			ap_increase_queue_count(ap_dev);
			ap_dev->total_request_count++;
			break;
		case AP_RESPONSE_Q_FULL:
		case AP_RESPONSE_RESET_IN_PROGRESS:
			list_add_tail(&ap_msg->list, &ap_dev->requestq);
			ap_dev->requestq_count++;
			ap_dev->total_request_count++;
			return -EBUSY;
		case AP_RESPONSE_REQ_FAC_NOT_INST:
		case AP_RESPONSE_MESSAGE_TOO_BIG:
			ap_dev->drv->receive(ap_dev, ap_msg, ERR_PTR(-EINVAL));
			return -EINVAL;
		default:	/* Device is gone. */
			ap_dev->drv->receive(ap_dev, ap_msg, ERR_PTR(-ENODEV));
			return -ENODEV;
		}
	} else {
		list_add_tail(&ap_msg->list, &ap_dev->requestq);
		ap_dev->requestq_count++;
		ap_dev->total_request_count++;
		return -EBUSY;
	}
	ap_schedule_poll_timer();
	return 0;
}

void ap_queue_message(struct ap_device *ap_dev, struct ap_message *ap_msg)
{
	unsigned long flags;
	int rc;

	spin_lock_bh(&ap_dev->lock);
	if (!ap_dev->unregistered) {
		/* Make room on the queue by polling for finished requests. */
		rc = ap_poll_queue(ap_dev, &flags);
		if (!rc)
			rc = __ap_queue_message(ap_dev, ap_msg);
		if (!rc)
			wake_up(&ap_poll_wait);
		if (rc == -ENODEV)
			ap_dev->unregistered = 1;
	} else {
		ap_dev->drv->receive(ap_dev, ap_msg, ERR_PTR(-ENODEV));
		rc = -ENODEV;
	}
	spin_unlock_bh(&ap_dev->lock);
	if (rc == -ENODEV)
		device_unregister(&ap_dev->device);
}
EXPORT_SYMBOL(ap_queue_message);

/**
 * ap_cancel_message(): Cancel a crypto request.
 * @ap_dev: The AP device that has the message queued
 * @ap_msg: The message that is to be removed
 *
 * Cancel a crypto request. This is done by removing the request
 * from the device pending or request queue. Note that the
 * request stays on the AP queue. When it finishes the message
 * reply will be discarded because the psmid can't be found.
 */
void ap_cancel_message(struct ap_device *ap_dev, struct ap_message *ap_msg)
{
	struct ap_message *tmp;

	spin_lock_bh(&ap_dev->lock);
	if (!list_empty(&ap_msg->list)) {
		list_for_each_entry(tmp, &ap_dev->pendingq, list)
			if (tmp->psmid == ap_msg->psmid) {
				ap_dev->pendingq_count--;
				goto found;
			}
		ap_dev->requestq_count--;
	found:
		list_del_init(&ap_msg->list);
	}
	spin_unlock_bh(&ap_dev->lock);
}
EXPORT_SYMBOL(ap_cancel_message);
=======
	return test_facility(65);
}

/**
 * ap_qci_available(): Test if AP configuration
 * information can be queried via QCI subfunction.
 *
 * Returns 1 if subfunction PQAP(QCI) is available.
 */
static int ap_qci_available(void)
{
	return test_facility(12);
}

/**
 * ap_apft_available(): Test if AP facilities test (APFT)
 * facility is available.
 *
 * Returns 1 if APFT is available.
 */
static int ap_apft_available(void)
{
	return test_facility(15);
}

/*
 * ap_qact_available(): Test if the PQAP(QACT) subfunction is available.
 *
 * Returns 1 if the QACT subfunction is available.
 */
static inline int ap_qact_available(void)
{
	if (ap_qci_info)
		return ap_qci_info->qact;
	return 0;
}

/*
 * ap_sb_available(): Test if the AP secure binding facility is available.
 *
 * Returns 1 if secure binding facility is available.
 */
int ap_sb_available(void)
{
	if (ap_qci_info)
		return ap_qci_info->apsb;
	return 0;
}

/*
 * ap_is_se_guest(): Check for SE guest with AP pass-through support.
 */
bool ap_is_se_guest(void)
{
	return is_prot_virt_guest() && ap_sb_available();
}
EXPORT_SYMBOL(ap_is_se_guest);

/*
 * ap_fetch_qci_info(): Fetch cryptographic config info
 *
 * Returns the ap configuration info fetched via PQAP(QCI).
 * On success 0 is returned, on failure a negative errno
 * is returned, e.g. if the PQAP(QCI) instruction is not
 * available, the return value will be -EOPNOTSUPP.
 */
static inline int ap_fetch_qci_info(struct ap_config_info *info)
{
	if (!ap_qci_available())
		return -EOPNOTSUPP;
	if (!info)
		return -EINVAL;
	return ap_qci(info);
}

/**
 * ap_init_qci_info(): Allocate and query qci config info.
 * Does also update the static variables ap_max_domain_id
 * and ap_max_adapter_id if this info is available.
 */
static void __init ap_init_qci_info(void)
{
	if (!ap_qci_available()) {
		AP_DBF_INFO("%s QCI not supported\n", __func__);
		return;
	}

	ap_qci_info = kzalloc(sizeof(*ap_qci_info), GFP_KERNEL);
	if (!ap_qci_info)
		return;
	ap_qci_info_old = kzalloc(sizeof(*ap_qci_info_old), GFP_KERNEL);
	if (!ap_qci_info_old) {
		kfree(ap_qci_info);
		ap_qci_info = NULL;
		return;
	}
	if (ap_fetch_qci_info(ap_qci_info) != 0) {
		kfree(ap_qci_info);
		kfree(ap_qci_info_old);
		ap_qci_info = NULL;
		ap_qci_info_old = NULL;
		return;
	}
	AP_DBF_INFO("%s successful fetched initial qci info\n", __func__);

	if (ap_qci_info->apxa) {
		if (ap_qci_info->na) {
			ap_max_adapter_id = ap_qci_info->na;
			AP_DBF_INFO("%s new ap_max_adapter_id is %d\n",
				    __func__, ap_max_adapter_id);
		}
		if (ap_qci_info->nd) {
			ap_max_domain_id = ap_qci_info->nd;
			AP_DBF_INFO("%s new ap_max_domain_id is %d\n",
				    __func__, ap_max_domain_id);
		}
	}

	memcpy(ap_qci_info_old, ap_qci_info, sizeof(*ap_qci_info));
}

/*
 * ap_test_config(): helper function to extract the nrth bit
 *		     within the unsigned int array field.
 */
static inline int ap_test_config(unsigned int *field, unsigned int nr)
{
	return ap_test_bit((field + (nr >> 5)), (nr & 0x1f));
}

/*
 * ap_test_config_card_id(): Test, whether an AP card ID is configured.
 *
 * Returns 0 if the card is not configured
 *	   1 if the card is configured or
 *	     if the configuration information is not available
 */
static inline int ap_test_config_card_id(unsigned int id)
{
	if (id > ap_max_adapter_id)
		return 0;
	if (ap_qci_info)
		return ap_test_config(ap_qci_info->apm, id);
	return 1;
}

/*
 * ap_test_config_usage_domain(): Test, whether an AP usage domain
 * is configured.
 *
 * Returns 0 if the usage domain is not configured
 *	   1 if the usage domain is configured or
 *	     if the configuration information is not available
 */
int ap_test_config_usage_domain(unsigned int domain)
{
	if (domain > ap_max_domain_id)
		return 0;
	if (ap_qci_info)
		return ap_test_config(ap_qci_info->aqm, domain);
	return 1;
}
EXPORT_SYMBOL(ap_test_config_usage_domain);

/*
 * ap_test_config_ctrl_domain(): Test, whether an AP control domain
 * is configured.
 * @domain AP control domain ID
 *
 * Returns 1 if the control domain is configured
 *	   0 in all other cases
 */
int ap_test_config_ctrl_domain(unsigned int domain)
{
	if (!ap_qci_info || domain > ap_max_domain_id)
		return 0;
	return ap_test_config(ap_qci_info->adm, domain);
}
EXPORT_SYMBOL(ap_test_config_ctrl_domain);

/*
 * ap_queue_info(): Check and get AP queue info.
 * Returns: 1 if APQN exists and info is filled,
 *	    0 if APQN seems to exist but there is no info
 *	      available (eg. caused by an asynch pending error)
 *	   -1 invalid APQN, TAPQ error or AP queue status which
 *	      indicates there is no APQN.
 */
static int ap_queue_info(ap_qid_t qid, struct ap_tapq_hwinfo *hwinfo,
			 bool *decfg, bool *cstop)
{
	struct ap_queue_status status;

	hwinfo->value = 0;

	/* make sure we don't run into a specifiation exception */
	if (AP_QID_CARD(qid) > ap_max_adapter_id ||
	    AP_QID_QUEUE(qid) > ap_max_domain_id)
		return -1;

	/* call TAPQ on this APQN */
	status = ap_test_queue(qid, ap_apft_available(), hwinfo);

	switch (status.response_code) {
	case AP_RESPONSE_NORMAL:
	case AP_RESPONSE_RESET_IN_PROGRESS:
	case AP_RESPONSE_DECONFIGURED:
	case AP_RESPONSE_CHECKSTOPPED:
	case AP_RESPONSE_BUSY:
		/* For all these RCs the tapq info should be available */
		break;
	default:
		/* On a pending async error the info should be available */
		if (!status.async)
			return -1;
		break;
	}

	/* There should be at least one of the mode bits set */
	if (WARN_ON_ONCE(!hwinfo->value))
		return 0;

	*decfg = status.response_code == AP_RESPONSE_DECONFIGURED;
	*cstop = status.response_code == AP_RESPONSE_CHECKSTOPPED;

	return 1;
}

void ap_wait(enum ap_sm_wait wait)
{
	ktime_t hr_time;

	switch (wait) {
	case AP_SM_WAIT_AGAIN:
	case AP_SM_WAIT_INTERRUPT:
		if (ap_irq_flag)
			break;
		if (ap_poll_kthread) {
			wake_up(&ap_poll_wait);
			break;
		}
		fallthrough;
	case AP_SM_WAIT_LOW_TIMEOUT:
	case AP_SM_WAIT_HIGH_TIMEOUT:
		spin_lock_bh(&ap_poll_timer_lock);
		if (!hrtimer_is_queued(&ap_poll_timer)) {
			hr_time =
				wait == AP_SM_WAIT_LOW_TIMEOUT ?
				poll_low_timeout : poll_high_timeout;
			hrtimer_forward_now(&ap_poll_timer, hr_time);
			hrtimer_restart(&ap_poll_timer);
		}
		spin_unlock_bh(&ap_poll_timer_lock);
		break;
	case AP_SM_WAIT_NONE:
	default:
		break;
	}
}

/**
 * ap_request_timeout(): Handling of request timeouts
 * @t: timer making this callback
 *
 * Handles request timeouts.
 */
void ap_request_timeout(struct timer_list *t)
{
	struct ap_queue *aq = from_timer(aq, t, timeout);

	spin_lock_bh(&aq->lock);
	ap_wait(ap_sm_event(aq, AP_SM_EVENT_TIMEOUT));
	spin_unlock_bh(&aq->lock);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * ap_poll_timeout(): AP receive polling for finished AP requests.
 * @unused: Unused pointer.
 *
 * Schedules the AP tasklet using a high resolution timer.
 */
static enum hrtimer_restart ap_poll_timeout(struct hrtimer *unused)
{
	tasklet_schedule(&ap_tasklet);
	return HRTIMER_NORESTART;
}

/**
<<<<<<< HEAD
 * ap_reset(): Reset a not responding AP device.
 * @ap_dev: Pointer to the AP device
 *
 * Reset a not responding AP device and move all requests from the
 * pending queue to the request queue.
 */
static void ap_reset(struct ap_device *ap_dev)
{
	int rc;

	ap_dev->reset = AP_RESET_IGNORE;
	atomic_sub(ap_dev->queue_count, &ap_poll_requests);
	ap_dev->queue_count = 0;
	list_splice_init(&ap_dev->pendingq, &ap_dev->requestq);
	ap_dev->requestq_count += ap_dev->pendingq_count;
	ap_dev->pendingq_count = 0;
	rc = ap_init_queue(ap_dev->qid);
	if (rc == -ENODEV)
		ap_dev->unregistered = 1;
	else
		__ap_schedule_poll_timer();
}

static int __ap_poll_device(struct ap_device *ap_dev, unsigned long *flags)
{
	if (!ap_dev->unregistered) {
		if (ap_poll_queue(ap_dev, flags))
			ap_dev->unregistered = 1;
		if (ap_dev->reset == AP_RESET_DO)
			ap_reset(ap_dev);
	}
	return 0;
}

/**
 * ap_poll_all(): Poll all AP devices.
 * @dummy: Unused variable
 *
 * Poll all AP devices on the bus in a round robin fashion. Continue
 * polling until bit 2^0 of the control flags is not set. If bit 2^1
 * of the control flags has been set arm the poll timer.
 */
static void ap_poll_all(unsigned long dummy)
{
	unsigned long flags;
	struct ap_device *ap_dev;

	/* Reset the indicator if interrupts are used. Thus new interrupts can
	 * be received. Doing it in the beginning of the tasklet is therefor
	 * important that no requests on any AP get lost.
	 */
	if (ap_using_interrupts())
		xchg((u8 *)ap_interrupt_indicator, 0);
	do {
		flags = 0;
		spin_lock(&ap_device_list_lock);
		list_for_each_entry(ap_dev, &ap_device_list, list) {
			spin_lock(&ap_dev->lock);
			__ap_poll_device(ap_dev, &flags);
			spin_unlock(&ap_dev->lock);
		}
		spin_unlock(&ap_device_list_lock);
	} while (flags & 1);
	if (flags & 2)
		ap_schedule_poll_timer();
=======
 * ap_interrupt_handler() - Schedule ap_tasklet on interrupt
 * @airq: pointer to adapter interrupt descriptor
 * @tpi_info: ignored
 */
static void ap_interrupt_handler(struct airq_struct *airq,
				 struct tpi_info *tpi_info)
{
	inc_irq_stat(IRQIO_APB);
	tasklet_schedule(&ap_tasklet);
}

/**
 * ap_tasklet_fn(): Tasklet to poll all AP devices.
 * @dummy: Unused variable
 *
 * Poll all AP devices on the bus.
 */
static void ap_tasklet_fn(unsigned long dummy)
{
	int bkt;
	struct ap_queue *aq;
	enum ap_sm_wait wait = AP_SM_WAIT_NONE;

	/* Reset the indicator if interrupts are used. Thus new interrupts can
	 * be received. Doing it in the beginning of the tasklet is therefore
	 * important that no requests on any AP get lost.
	 */
	if (ap_irq_flag)
		xchg(ap_airq.lsi_ptr, 0);

	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode) {
		spin_lock_bh(&aq->lock);
		wait = min(wait, ap_sm_event_loop(aq, AP_SM_EVENT_POLL));
		spin_unlock_bh(&aq->lock);
	}
	spin_unlock_bh(&ap_queues_lock);

	ap_wait(wait);
}

static int ap_pending_requests(void)
{
	int bkt;
	struct ap_queue *aq;

	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode) {
		if (aq->queue_count == 0)
			continue;
		spin_unlock_bh(&ap_queues_lock);
		return 1;
	}
	spin_unlock_bh(&ap_queues_lock);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ap_poll_thread(): Thread that polls for finished requests.
 * @data: Unused pointer
 *
 * AP bus poll thread. The purpose of this thread is to poll for
 * finished requests in a loop if there is a "free" cpu - that is
 * a cpu that doesn't have anything better to do. The polling stops
 * as soon as there is another task or if all messages have been
 * delivered.
 */
static int ap_poll_thread(void *data)
{
	DECLARE_WAITQUEUE(wait, current);
<<<<<<< HEAD
	unsigned long flags;
	int requests;
	struct ap_device *ap_dev;

	set_user_nice(current, 19);
	while (1) {
		if (ap_suspend_flag)
			return 0;
		if (need_resched()) {
			schedule();
			continue;
		}
		add_wait_queue(&ap_poll_wait, &wait);
		set_current_state(TASK_INTERRUPTIBLE);
		if (kthread_should_stop())
			break;
		requests = atomic_read(&ap_poll_requests);
		if (requests <= 0)
			schedule();
		set_current_state(TASK_RUNNING);
		remove_wait_queue(&ap_poll_wait, &wait);

		flags = 0;
		spin_lock_bh(&ap_device_list_lock);
		list_for_each_entry(ap_dev, &ap_device_list, list) {
			spin_lock(&ap_dev->lock);
			__ap_poll_device(ap_dev, &flags);
			spin_unlock(&ap_dev->lock);
		}
		spin_unlock_bh(&ap_device_list_lock);
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&ap_poll_wait, &wait);
=======

	set_user_nice(current, MAX_NICE);
	set_freezable();
	while (!kthread_should_stop()) {
		add_wait_queue(&ap_poll_wait, &wait);
		set_current_state(TASK_INTERRUPTIBLE);
		if (!ap_pending_requests()) {
			schedule();
			try_to_freeze();
		}
		set_current_state(TASK_RUNNING);
		remove_wait_queue(&ap_poll_wait, &wait);
		if (need_resched()) {
			schedule();
			try_to_freeze();
			continue;
		}
		ap_tasklet_fn(0);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ap_poll_thread_start(void)
{
	int rc;

<<<<<<< HEAD
	if (ap_using_interrupts() || ap_suspend_flag)
		return 0;
	mutex_lock(&ap_poll_thread_mutex);
	if (!ap_poll_kthread) {
		ap_poll_kthread = kthread_run(ap_poll_thread, NULL, "appoll");
		rc = IS_ERR(ap_poll_kthread) ? PTR_ERR(ap_poll_kthread) : 0;
		if (rc)
			ap_poll_kthread = NULL;
	}
	else
		rc = 0;
=======
	if (ap_irq_flag || ap_poll_kthread)
		return 0;
	mutex_lock(&ap_poll_thread_mutex);
	ap_poll_kthread = kthread_run(ap_poll_thread, NULL, "appoll");
	rc = PTR_ERR_OR_ZERO(ap_poll_kthread);
	if (rc)
		ap_poll_kthread = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&ap_poll_thread_mutex);
	return rc;
}

static void ap_poll_thread_stop(void)
{
<<<<<<< HEAD
	mutex_lock(&ap_poll_thread_mutex);
	if (ap_poll_kthread) {
		kthread_stop(ap_poll_kthread);
		ap_poll_kthread = NULL;
	}
	mutex_unlock(&ap_poll_thread_mutex);
}

/**
 * ap_request_timeout(): Handling of request timeouts
 * @data: Holds the AP device.
 *
 * Handles request timeouts.
 */
static void ap_request_timeout(unsigned long data)
{
	struct ap_device *ap_dev = (struct ap_device *) data;

	if (ap_dev->reset == AP_RESET_ARMED) {
		ap_dev->reset = AP_RESET_DO;

		if (ap_using_interrupts())
			tasklet_schedule(&ap_tasklet);
	}
}

static void ap_reset_domain(void)
{
	int i;

	if (ap_domain_index != -1)
		for (i = 0; i < AP_DEVICES; i++)
			ap_reset_queue(AP_MKQID(i, ap_domain_index));
}

static void ap_reset_all(void)
{
	int i, j;

	for (i = 0; i < AP_DOMAINS; i++)
		for (j = 0; j < AP_DEVICES; j++)
			ap_reset_queue(AP_MKQID(j, i));
}

static struct reset_call ap_reset_call = {
	.fn = ap_reset_all,
};
=======
	if (!ap_poll_kthread)
		return;
	mutex_lock(&ap_poll_thread_mutex);
	kthread_stop(ap_poll_kthread);
	ap_poll_kthread = NULL;
	mutex_unlock(&ap_poll_thread_mutex);
}

#define is_card_dev(x) ((x)->parent == ap_root_device)
#define is_queue_dev(x) ((x)->parent != ap_root_device)

/**
 * ap_bus_match()
 * @dev: Pointer to device
 * @drv: Pointer to device_driver
 *
 * AP bus driver registration/unregistration.
 */
static int ap_bus_match(struct device *dev, struct device_driver *drv)
{
	struct ap_driver *ap_drv = to_ap_drv(drv);
	struct ap_device_id *id;

	/*
	 * Compare device type of the device with the list of
	 * supported types of the device_driver.
	 */
	for (id = ap_drv->ids; id->match_flags; id++) {
		if (is_card_dev(dev) &&
		    id->match_flags & AP_DEVICE_ID_MATCH_CARD_TYPE &&
		    id->dev_type == to_ap_dev(dev)->device_type)
			return 1;
		if (is_queue_dev(dev) &&
		    id->match_flags & AP_DEVICE_ID_MATCH_QUEUE_TYPE &&
		    id->dev_type == to_ap_dev(dev)->device_type)
			return 1;
	}
	return 0;
}

/**
 * ap_uevent(): Uevent function for AP devices.
 * @dev: Pointer to device
 * @env: Pointer to kobj_uevent_env
 *
 * It sets up a single environment variable DEV_TYPE which contains the
 * hardware device type.
 */
static int ap_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	int rc = 0;
	const struct ap_device *ap_dev = to_ap_dev(dev);

	/* Uevents from ap bus core don't need extensions to the env */
	if (dev == ap_root_device)
		return 0;

	if (is_card_dev(dev)) {
		struct ap_card *ac = to_ap_card(&ap_dev->device);

		/* Set up DEV_TYPE environment variable. */
		rc = add_uevent_var(env, "DEV_TYPE=%04X", ap_dev->device_type);
		if (rc)
			return rc;
		/* Add MODALIAS= */
		rc = add_uevent_var(env, "MODALIAS=ap:t%02X", ap_dev->device_type);
		if (rc)
			return rc;

		/* Add MODE=<accel|cca|ep11> */
		if (ac->hwinfo.accel)
			rc = add_uevent_var(env, "MODE=accel");
		else if (ac->hwinfo.cca)
			rc = add_uevent_var(env, "MODE=cca");
		else if (ac->hwinfo.ep11)
			rc = add_uevent_var(env, "MODE=ep11");
		if (rc)
			return rc;
	} else {
		struct ap_queue *aq = to_ap_queue(&ap_dev->device);

		/* Add MODE=<accel|cca|ep11> */
		if (aq->card->hwinfo.accel)
			rc = add_uevent_var(env, "MODE=accel");
		else if (aq->card->hwinfo.cca)
			rc = add_uevent_var(env, "MODE=cca");
		else if (aq->card->hwinfo.ep11)
			rc = add_uevent_var(env, "MODE=ep11");
		if (rc)
			return rc;
	}

	return 0;
}

static void ap_send_init_scan_done_uevent(void)
{
	char *envp[] = { "INITSCAN=done", NULL };

	kobject_uevent_env(&ap_root_device->kobj, KOBJ_CHANGE, envp);
}

static void ap_send_bindings_complete_uevent(void)
{
	char buf[32];
	char *envp[] = { "BINDINGS=complete", buf, NULL };

	snprintf(buf, sizeof(buf), "COMPLETECOUNT=%llu",
		 atomic64_inc_return(&ap_bindings_complete_count));
	kobject_uevent_env(&ap_root_device->kobj, KOBJ_CHANGE, envp);
}

void ap_send_config_uevent(struct ap_device *ap_dev, bool cfg)
{
	char buf[16];
	char *envp[] = { buf, NULL };

	snprintf(buf, sizeof(buf), "CONFIG=%d", cfg ? 1 : 0);

	kobject_uevent_env(&ap_dev->device.kobj, KOBJ_CHANGE, envp);
}
EXPORT_SYMBOL(ap_send_config_uevent);

void ap_send_online_uevent(struct ap_device *ap_dev, int online)
{
	char buf[16];
	char *envp[] = { buf, NULL };

	snprintf(buf, sizeof(buf), "ONLINE=%d", online ? 1 : 0);

	kobject_uevent_env(&ap_dev->device.kobj, KOBJ_CHANGE, envp);
}
EXPORT_SYMBOL(ap_send_online_uevent);

static void ap_send_mask_changed_uevent(unsigned long *newapm,
					unsigned long *newaqm)
{
	char buf[100];
	char *envp[] = { buf, NULL };

	if (newapm)
		snprintf(buf, sizeof(buf),
			 "APMASK=0x%016lx%016lx%016lx%016lx\n",
			 newapm[0], newapm[1], newapm[2], newapm[3]);
	else
		snprintf(buf, sizeof(buf),
			 "AQMASK=0x%016lx%016lx%016lx%016lx\n",
			 newaqm[0], newaqm[1], newaqm[2], newaqm[3]);

	kobject_uevent_env(&ap_root_device->kobj, KOBJ_CHANGE, envp);
}

/*
 * calc # of bound APQNs
 */

struct __ap_calc_ctrs {
	unsigned int apqns;
	unsigned int bound;
};

static int __ap_calc_helper(struct device *dev, void *arg)
{
	struct __ap_calc_ctrs *pctrs = (struct __ap_calc_ctrs *)arg;

	if (is_queue_dev(dev)) {
		pctrs->apqns++;
		if (dev->driver)
			pctrs->bound++;
	}

	return 0;
}

static void ap_calc_bound_apqns(unsigned int *apqns, unsigned int *bound)
{
	struct __ap_calc_ctrs ctrs;

	memset(&ctrs, 0, sizeof(ctrs));
	bus_for_each_dev(&ap_bus_type, NULL, (void *)&ctrs, __ap_calc_helper);

	*apqns = ctrs.apqns;
	*bound = ctrs.bound;
}

/*
 * After ap bus scan do check if all existing APQNs are
 * bound to device drivers.
 */
static void ap_check_bindings_complete(void)
{
	unsigned int apqns, bound;

	if (atomic64_read(&ap_scan_bus_count) >= 1) {
		ap_calc_bound_apqns(&apqns, &bound);
		if (bound == apqns) {
			if (!completion_done(&ap_apqn_bindings_complete)) {
				complete_all(&ap_apqn_bindings_complete);
				pr_debug("%s all apqn bindings complete\n", __func__);
			}
			ap_send_bindings_complete_uevent();
		}
	}
}

/*
 * Interface to wait for the AP bus to have done one initial ap bus
 * scan and all detected APQNs have been bound to device drivers.
 * If these both conditions are not fulfilled, this function blocks
 * on a condition with wait_for_completion_interruptible_timeout().
 * If these both conditions are fulfilled (before the timeout hits)
 * the return value is 0. If the timeout (in jiffies) hits instead
 * -ETIME is returned. On failures negative return values are
 * returned to the caller.
 */
int ap_wait_apqn_bindings_complete(unsigned long timeout)
{
	int rc = 0;
	long l;

	if (completion_done(&ap_apqn_bindings_complete))
		return 0;

	if (timeout)
		l = wait_for_completion_interruptible_timeout(
			&ap_apqn_bindings_complete, timeout);
	else
		l = wait_for_completion_interruptible(
			&ap_apqn_bindings_complete);
	if (l < 0)
		rc = l == -ERESTARTSYS ? -EINTR : l;
	else if (l == 0 && timeout)
		rc = -ETIME;

	pr_debug("%s rc=%d\n", __func__, rc);
	return rc;
}
EXPORT_SYMBOL(ap_wait_apqn_bindings_complete);

static int __ap_queue_devices_with_id_unregister(struct device *dev, void *data)
{
	if (is_queue_dev(dev) &&
	    AP_QID_CARD(to_ap_queue(dev)->qid) == (int)(long)data)
		device_unregister(dev);
	return 0;
}

static int __ap_revise_reserved(struct device *dev, void *dummy)
{
	int rc, card, queue, devres, drvres;

	if (is_queue_dev(dev)) {
		card = AP_QID_CARD(to_ap_queue(dev)->qid);
		queue = AP_QID_QUEUE(to_ap_queue(dev)->qid);
		mutex_lock(&ap_perms_mutex);
		devres = test_bit_inv(card, ap_perms.apm) &&
			test_bit_inv(queue, ap_perms.aqm);
		mutex_unlock(&ap_perms_mutex);
		drvres = to_ap_drv(dev->driver)->flags
			& AP_DRIVER_FLAG_DEFAULT;
		if (!!devres != !!drvres) {
			pr_debug("%s reprobing queue=%02x.%04x\n",
				 __func__, card, queue);
			rc = device_reprobe(dev);
			if (rc)
				AP_DBF_WARN("%s reprobing queue=%02x.%04x failed\n",
					    __func__, card, queue);
		}
	}

	return 0;
}

static void ap_bus_revise_bindings(void)
{
	bus_for_each_dev(&ap_bus_type, NULL, NULL, __ap_revise_reserved);
}

/**
 * ap_owned_by_def_drv: indicates whether an AP adapter is reserved for the
 *			default host driver or not.
 * @card: the APID of the adapter card to check
 * @queue: the APQI of the queue to check
 *
 * Note: the ap_perms_mutex must be locked by the caller of this function.
 *
 * Return: an int specifying whether the AP adapter is reserved for the host (1)
 *	   or not (0).
 */
int ap_owned_by_def_drv(int card, int queue)
{
	int rc = 0;

	if (card < 0 || card >= AP_DEVICES || queue < 0 || queue >= AP_DOMAINS)
		return -EINVAL;

	if (test_bit_inv(card, ap_perms.apm) &&
	    test_bit_inv(queue, ap_perms.aqm))
		rc = 1;

	return rc;
}
EXPORT_SYMBOL(ap_owned_by_def_drv);

/**
 * ap_apqn_in_matrix_owned_by_def_drv: indicates whether every APQN contained in
 *				       a set is reserved for the host drivers
 *				       or not.
 * @apm: a bitmap specifying a set of APIDs comprising the APQNs to check
 * @aqm: a bitmap specifying a set of APQIs comprising the APQNs to check
 *
 * Note: the ap_perms_mutex must be locked by the caller of this function.
 *
 * Return: an int specifying whether each APQN is reserved for the host (1) or
 *	   not (0)
 */
int ap_apqn_in_matrix_owned_by_def_drv(unsigned long *apm,
				       unsigned long *aqm)
{
	int card, queue, rc = 0;

	for (card = 0; !rc && card < AP_DEVICES; card++)
		if (test_bit_inv(card, apm) &&
		    test_bit_inv(card, ap_perms.apm))
			for (queue = 0; !rc && queue < AP_DOMAINS; queue++)
				if (test_bit_inv(queue, aqm) &&
				    test_bit_inv(queue, ap_perms.aqm))
					rc = 1;

	return rc;
}
EXPORT_SYMBOL(ap_apqn_in_matrix_owned_by_def_drv);

static int ap_device_probe(struct device *dev)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	struct ap_driver *ap_drv = to_ap_drv(dev->driver);
	int card, queue, devres, drvres, rc = -ENODEV;

	if (!get_device(dev))
		return rc;

	if (is_queue_dev(dev)) {
		/*
		 * If the apqn is marked as reserved/used by ap bus and
		 * default drivers, only probe with drivers with the default
		 * flag set. If it is not marked, only probe with drivers
		 * with the default flag not set.
		 */
		card = AP_QID_CARD(to_ap_queue(dev)->qid);
		queue = AP_QID_QUEUE(to_ap_queue(dev)->qid);
		mutex_lock(&ap_perms_mutex);
		devres = test_bit_inv(card, ap_perms.apm) &&
			test_bit_inv(queue, ap_perms.aqm);
		mutex_unlock(&ap_perms_mutex);
		drvres = ap_drv->flags & AP_DRIVER_FLAG_DEFAULT;
		if (!!devres != !!drvres)
			goto out;
	}

	/* Add queue/card to list of active queues/cards */
	spin_lock_bh(&ap_queues_lock);
	if (is_queue_dev(dev))
		hash_add(ap_queues, &to_ap_queue(dev)->hnode,
			 to_ap_queue(dev)->qid);
	spin_unlock_bh(&ap_queues_lock);

	rc = ap_drv->probe ? ap_drv->probe(ap_dev) : -ENODEV;

	if (rc) {
		spin_lock_bh(&ap_queues_lock);
		if (is_queue_dev(dev))
			hash_del(&to_ap_queue(dev)->hnode);
		spin_unlock_bh(&ap_queues_lock);
	}

out:
	if (rc)
		put_device(dev);
	return rc;
}

static void ap_device_remove(struct device *dev)
{
	struct ap_device *ap_dev = to_ap_dev(dev);
	struct ap_driver *ap_drv = to_ap_drv(dev->driver);

	/* prepare ap queue device removal */
	if (is_queue_dev(dev))
		ap_queue_prepare_remove(to_ap_queue(dev));

	/* driver's chance to clean up gracefully */
	if (ap_drv->remove)
		ap_drv->remove(ap_dev);

	/* now do the ap queue device remove */
	if (is_queue_dev(dev))
		ap_queue_remove(to_ap_queue(dev));

	/* Remove queue/card from list of active queues/cards */
	spin_lock_bh(&ap_queues_lock);
	if (is_queue_dev(dev))
		hash_del(&to_ap_queue(dev)->hnode);
	spin_unlock_bh(&ap_queues_lock);

	put_device(dev);
}

struct ap_queue *ap_get_qdev(ap_qid_t qid)
{
	int bkt;
	struct ap_queue *aq;

	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode) {
		if (aq->qid == qid) {
			get_device(&aq->ap_dev.device);
			spin_unlock_bh(&ap_queues_lock);
			return aq;
		}
	}
	spin_unlock_bh(&ap_queues_lock);

	return NULL;
}
EXPORT_SYMBOL(ap_get_qdev);

int ap_driver_register(struct ap_driver *ap_drv, struct module *owner,
		       char *name)
{
	struct device_driver *drv = &ap_drv->driver;

	drv->bus = &ap_bus_type;
	drv->owner = owner;
	drv->name = name;
	return driver_register(drv);
}
EXPORT_SYMBOL(ap_driver_register);

void ap_driver_unregister(struct ap_driver *ap_drv)
{
	driver_unregister(&ap_drv->driver);
}
EXPORT_SYMBOL(ap_driver_unregister);

/*
 * Enforce a synchronous AP bus rescan.
 * Returns true if the bus scan finds a change in the AP configuration
 * and AP devices have been added or deleted when this function returns.
 */
bool ap_bus_force_rescan(void)
{
	unsigned long scan_counter = atomic64_read(&ap_scan_bus_count);
	bool rc = false;

	pr_debug(">%s scan counter=%lu\n", __func__, scan_counter);

	/* Only trigger AP bus scans after the initial scan is done */
	if (scan_counter <= 0)
		goto out;

	/* Try to acquire the AP scan bus mutex */
	if (mutex_trylock(&ap_scan_bus_mutex)) {
		/* mutex acquired, run the AP bus scan */
		ap_scan_bus_result = ap_scan_bus();
		rc = ap_scan_bus_result;
		mutex_unlock(&ap_scan_bus_mutex);
		goto out;
	}

	/*
	 * Mutex acquire failed. So there is currently another task
	 * already running the AP bus scan. Then let's simple wait
	 * for the lock which means the other task has finished and
	 * stored the result in ap_scan_bus_result.
	 */
	if (mutex_lock_interruptible(&ap_scan_bus_mutex)) {
		/* some error occurred, ignore and go out */
		goto out;
	}
	rc = ap_scan_bus_result;
	mutex_unlock(&ap_scan_bus_mutex);

out:
	pr_debug("%s rc=%d\n", __func__, rc);
	return rc;
}
EXPORT_SYMBOL(ap_bus_force_rescan);

/*
 * A config change has happened, force an ap bus rescan.
 */
void ap_bus_cfg_chg(void)
{
	pr_debug("%s config change, forcing bus rescan\n", __func__);

	ap_bus_force_rescan();
}

/*
 * hex2bitmap() - parse hex mask string and set bitmap.
 * Valid strings are "0x012345678" with at least one valid hex number.
 * Rest of the bitmap to the right is padded with 0. No spaces allowed
 * within the string, the leading 0x may be omitted.
 * Returns the bitmask with exactly the bits set as given by the hex
 * string (both in big endian order).
 */
static int hex2bitmap(const char *str, unsigned long *bitmap, int bits)
{
	int i, n, b;

	/* bits needs to be a multiple of 8 */
	if (bits & 0x07)
		return -EINVAL;

	if (str[0] == '0' && str[1] == 'x')
		str++;
	if (*str == 'x')
		str++;

	for (i = 0; isxdigit(*str) && i < bits; str++) {
		b = hex_to_bin(*str);
		for (n = 0; n < 4; n++)
			if (b & (0x08 >> n))
				set_bit_inv(i + n, bitmap);
		i += 4;
	}

	if (*str == '\n')
		str++;
	if (*str)
		return -EINVAL;
	return 0;
}

/*
 * modify_bitmap() - parse bitmask argument and modify an existing
 * bit mask accordingly. A concatenation (done with ',') of these
 * terms is recognized:
 *   +<bitnr>[-<bitnr>] or -<bitnr>[-<bitnr>]
 * <bitnr> may be any valid number (hex, decimal or octal) in the range
 * 0...bits-1; the leading + or - is required. Here are some examples:
 *   +0-15,+32,-128,-0xFF
 *   -0-255,+1-16,+0x128
 *   +1,+2,+3,+4,-5,-7-10
 * Returns the new bitmap after all changes have been applied. Every
 * positive value in the string will set a bit and every negative value
 * in the string will clear a bit. As a bit may be touched more than once,
 * the last 'operation' wins:
 * +0-255,-128 = first bits 0-255 will be set, then bit 128 will be
 * cleared again. All other bits are unmodified.
 */
static int modify_bitmap(const char *str, unsigned long *bitmap, int bits)
{
	int a, i, z;
	char *np, sign;

	/* bits needs to be a multiple of 8 */
	if (bits & 0x07)
		return -EINVAL;

	while (*str) {
		sign = *str++;
		if (sign != '+' && sign != '-')
			return -EINVAL;
		a = z = simple_strtoul(str, &np, 0);
		if (str == np || a >= bits)
			return -EINVAL;
		str = np;
		if (*str == '-') {
			z = simple_strtoul(++str, &np, 0);
			if (str == np || a > z || z >= bits)
				return -EINVAL;
			str = np;
		}
		for (i = a; i <= z; i++)
			if (sign == '+')
				set_bit_inv(i, bitmap);
			else
				clear_bit_inv(i, bitmap);
		while (*str == ',' || *str == '\n')
			str++;
	}

	return 0;
}

static int ap_parse_bitmap_str(const char *str, unsigned long *bitmap, int bits,
			       unsigned long *newmap)
{
	unsigned long size;
	int rc;

	size = BITS_TO_LONGS(bits) * sizeof(unsigned long);
	if (*str == '+' || *str == '-') {
		memcpy(newmap, bitmap, size);
		rc = modify_bitmap(str, newmap, bits);
	} else {
		memset(newmap, 0, size);
		rc = hex2bitmap(str, newmap, bits);
	}
	return rc;
}

int ap_parse_mask_str(const char *str,
		      unsigned long *bitmap, int bits,
		      struct mutex *lock)
{
	unsigned long *newmap, size;
	int rc;

	/* bits needs to be a multiple of 8 */
	if (bits & 0x07)
		return -EINVAL;

	size = BITS_TO_LONGS(bits) * sizeof(unsigned long);
	newmap = kmalloc(size, GFP_KERNEL);
	if (!newmap)
		return -ENOMEM;
	if (mutex_lock_interruptible(lock)) {
		kfree(newmap);
		return -ERESTARTSYS;
	}
	rc = ap_parse_bitmap_str(str, bitmap, bits, newmap);
	if (rc == 0)
		memcpy(bitmap, newmap, size);
	mutex_unlock(lock);
	kfree(newmap);
	return rc;
}
EXPORT_SYMBOL(ap_parse_mask_str);

/*
 * AP bus attributes.
 */

static ssize_t ap_domain_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_domain_index);
}

static ssize_t ap_domain_store(const struct bus_type *bus,
			       const char *buf, size_t count)
{
	int domain;

	if (sscanf(buf, "%i\n", &domain) != 1 ||
	    domain < 0 || domain > ap_max_domain_id ||
	    !test_bit_inv(domain, ap_perms.aqm))
		return -EINVAL;

	spin_lock_bh(&ap_domain_lock);
	ap_domain_index = domain;
	spin_unlock_bh(&ap_domain_lock);

	AP_DBF_INFO("%s stored new default domain=%d\n",
		    __func__, domain);

	return count;
}

static BUS_ATTR_RW(ap_domain);

static ssize_t ap_control_domain_mask_show(const struct bus_type *bus, char *buf)
{
	if (!ap_qci_info)	/* QCI not supported */
		return sysfs_emit(buf, "not supported\n");

	return sysfs_emit(buf, "0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
			  ap_qci_info->adm[0], ap_qci_info->adm[1],
			  ap_qci_info->adm[2], ap_qci_info->adm[3],
			  ap_qci_info->adm[4], ap_qci_info->adm[5],
			  ap_qci_info->adm[6], ap_qci_info->adm[7]);
}

static BUS_ATTR_RO(ap_control_domain_mask);

static ssize_t ap_usage_domain_mask_show(const struct bus_type *bus, char *buf)
{
	if (!ap_qci_info)	/* QCI not supported */
		return sysfs_emit(buf, "not supported\n");

	return sysfs_emit(buf, "0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
			  ap_qci_info->aqm[0], ap_qci_info->aqm[1],
			  ap_qci_info->aqm[2], ap_qci_info->aqm[3],
			  ap_qci_info->aqm[4], ap_qci_info->aqm[5],
			  ap_qci_info->aqm[6], ap_qci_info->aqm[7]);
}

static BUS_ATTR_RO(ap_usage_domain_mask);

static ssize_t ap_adapter_mask_show(const struct bus_type *bus, char *buf)
{
	if (!ap_qci_info)	/* QCI not supported */
		return sysfs_emit(buf, "not supported\n");

	return sysfs_emit(buf, "0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
			  ap_qci_info->apm[0], ap_qci_info->apm[1],
			  ap_qci_info->apm[2], ap_qci_info->apm[3],
			  ap_qci_info->apm[4], ap_qci_info->apm[5],
			  ap_qci_info->apm[6], ap_qci_info->apm[7]);
}

static BUS_ATTR_RO(ap_adapter_mask);

static ssize_t ap_interrupts_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_irq_flag ? 1 : 0);
}

static BUS_ATTR_RO(ap_interrupts);

static ssize_t config_time_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_scan_bus_time);
}

static ssize_t config_time_store(const struct bus_type *bus,
				 const char *buf, size_t count)
{
	int time;

	if (sscanf(buf, "%d\n", &time) != 1 || time < 5 || time > 120)
		return -EINVAL;
	ap_scan_bus_time = time;
	mod_timer(&ap_scan_bus_timer, jiffies + ap_scan_bus_time * HZ);
	return count;
}

static BUS_ATTR_RW(config_time);

static ssize_t poll_thread_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_poll_kthread ? 1 : 0);
}

static ssize_t poll_thread_store(const struct bus_type *bus,
				 const char *buf, size_t count)
{
	bool value;
	int rc;

	rc = kstrtobool(buf, &value);
	if (rc)
		return rc;

	if (value) {
		rc = ap_poll_thread_start();
		if (rc)
			count = rc;
	} else {
		ap_poll_thread_stop();
	}
	return count;
}

static BUS_ATTR_RW(poll_thread);

static ssize_t poll_timeout_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%lu\n", poll_high_timeout);
}

static ssize_t poll_timeout_store(const struct bus_type *bus, const char *buf,
				  size_t count)
{
	unsigned long value;
	ktime_t hr_time;
	int rc;

	rc = kstrtoul(buf, 0, &value);
	if (rc)
		return rc;

	/* 120 seconds = maximum poll interval */
	if (value > 120000000000UL)
		return -EINVAL;
	poll_high_timeout = value;
	hr_time = poll_high_timeout;

	spin_lock_bh(&ap_poll_timer_lock);
	hrtimer_cancel(&ap_poll_timer);
	hrtimer_set_expires(&ap_poll_timer, hr_time);
	hrtimer_start_expires(&ap_poll_timer, HRTIMER_MODE_ABS);
	spin_unlock_bh(&ap_poll_timer_lock);

	return count;
}

static BUS_ATTR_RW(poll_timeout);

static ssize_t ap_max_domain_id_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_max_domain_id);
}

static BUS_ATTR_RO(ap_max_domain_id);

static ssize_t ap_max_adapter_id_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%d\n", ap_max_adapter_id);
}

static BUS_ATTR_RO(ap_max_adapter_id);

static ssize_t apmask_show(const struct bus_type *bus, char *buf)
{
	int rc;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;
	rc = sysfs_emit(buf, "0x%016lx%016lx%016lx%016lx\n",
			ap_perms.apm[0], ap_perms.apm[1],
			ap_perms.apm[2], ap_perms.apm[3]);
	mutex_unlock(&ap_perms_mutex);

	return rc;
}

static int __verify_card_reservations(struct device_driver *drv, void *data)
{
	int rc = 0;
	struct ap_driver *ap_drv = to_ap_drv(drv);
	unsigned long *newapm = (unsigned long *)data;

	/*
	 * increase the driver's module refcounter to be sure it is not
	 * going away when we invoke the callback function.
	 */
	if (!try_module_get(drv->owner))
		return 0;

	if (ap_drv->in_use) {
		rc = ap_drv->in_use(newapm, ap_perms.aqm);
		if (rc)
			rc = -EBUSY;
	}

	/* release the driver's module */
	module_put(drv->owner);

	return rc;
}

static int apmask_commit(unsigned long *newapm)
{
	int rc;
	unsigned long reserved[BITS_TO_LONGS(AP_DEVICES)];

	/*
	 * Check if any bits in the apmask have been set which will
	 * result in queues being removed from non-default drivers
	 */
	if (bitmap_andnot(reserved, newapm, ap_perms.apm, AP_DEVICES)) {
		rc = bus_for_each_drv(&ap_bus_type, NULL, reserved,
				      __verify_card_reservations);
		if (rc)
			return rc;
	}

	memcpy(ap_perms.apm, newapm, APMASKSIZE);

	return 0;
}

static ssize_t apmask_store(const struct bus_type *bus, const char *buf,
			    size_t count)
{
	int rc, changes = 0;
	DECLARE_BITMAP(newapm, AP_DEVICES);

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	rc = ap_parse_bitmap_str(buf, ap_perms.apm, AP_DEVICES, newapm);
	if (rc)
		goto done;

	changes = memcmp(ap_perms.apm, newapm, APMASKSIZE);
	if (changes)
		rc = apmask_commit(newapm);

done:
	mutex_unlock(&ap_perms_mutex);
	if (rc)
		return rc;

	if (changes) {
		ap_bus_revise_bindings();
		ap_send_mask_changed_uevent(newapm, NULL);
	}

	return count;
}

static BUS_ATTR_RW(apmask);

static ssize_t aqmask_show(const struct bus_type *bus, char *buf)
{
	int rc;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;
	rc = sysfs_emit(buf, "0x%016lx%016lx%016lx%016lx\n",
			ap_perms.aqm[0], ap_perms.aqm[1],
			ap_perms.aqm[2], ap_perms.aqm[3]);
	mutex_unlock(&ap_perms_mutex);

	return rc;
}

static int __verify_queue_reservations(struct device_driver *drv, void *data)
{
	int rc = 0;
	struct ap_driver *ap_drv = to_ap_drv(drv);
	unsigned long *newaqm = (unsigned long *)data;

	/*
	 * increase the driver's module refcounter to be sure it is not
	 * going away when we invoke the callback function.
	 */
	if (!try_module_get(drv->owner))
		return 0;

	if (ap_drv->in_use) {
		rc = ap_drv->in_use(ap_perms.apm, newaqm);
		if (rc)
			rc = -EBUSY;
	}

	/* release the driver's module */
	module_put(drv->owner);

	return rc;
}

static int aqmask_commit(unsigned long *newaqm)
{
	int rc;
	unsigned long reserved[BITS_TO_LONGS(AP_DOMAINS)];

	/*
	 * Check if any bits in the aqmask have been set which will
	 * result in queues being removed from non-default drivers
	 */
	if (bitmap_andnot(reserved, newaqm, ap_perms.aqm, AP_DOMAINS)) {
		rc = bus_for_each_drv(&ap_bus_type, NULL, reserved,
				      __verify_queue_reservations);
		if (rc)
			return rc;
	}

	memcpy(ap_perms.aqm, newaqm, AQMASKSIZE);

	return 0;
}

static ssize_t aqmask_store(const struct bus_type *bus, const char *buf,
			    size_t count)
{
	int rc, changes = 0;
	DECLARE_BITMAP(newaqm, AP_DOMAINS);

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	rc = ap_parse_bitmap_str(buf, ap_perms.aqm, AP_DOMAINS, newaqm);
	if (rc)
		goto done;

	changes = memcmp(ap_perms.aqm, newaqm, APMASKSIZE);
	if (changes)
		rc = aqmask_commit(newaqm);

done:
	mutex_unlock(&ap_perms_mutex);
	if (rc)
		return rc;

	if (changes) {
		ap_bus_revise_bindings();
		ap_send_mask_changed_uevent(NULL, newaqm);
	}

	return count;
}

static BUS_ATTR_RW(aqmask);

static ssize_t scans_show(const struct bus_type *bus, char *buf)
{
	return sysfs_emit(buf, "%llu\n", atomic64_read(&ap_scan_bus_count));
}

static ssize_t scans_store(const struct bus_type *bus, const char *buf,
			   size_t count)
{
	AP_DBF_INFO("%s force AP bus rescan\n", __func__);

	ap_bus_force_rescan();

	return count;
}

static BUS_ATTR_RW(scans);

static ssize_t bindings_show(const struct bus_type *bus, char *buf)
{
	int rc;
	unsigned int apqns, n;

	ap_calc_bound_apqns(&apqns, &n);
	if (atomic64_read(&ap_scan_bus_count) >= 1 && n == apqns)
		rc = sysfs_emit(buf, "%u/%u (complete)\n", n, apqns);
	else
		rc = sysfs_emit(buf, "%u/%u\n", n, apqns);

	return rc;
}

static BUS_ATTR_RO(bindings);

static ssize_t features_show(const struct bus_type *bus, char *buf)
{
	int n = 0;

	if (!ap_qci_info)	/* QCI not supported */
		return sysfs_emit(buf, "-\n");

	if (ap_qci_info->apsc)
		n += sysfs_emit_at(buf, n, "APSC ");
	if (ap_qci_info->apxa)
		n += sysfs_emit_at(buf, n, "APXA ");
	if (ap_qci_info->qact)
		n += sysfs_emit_at(buf, n, "QACT ");
	if (ap_qci_info->rc8a)
		n += sysfs_emit_at(buf, n, "RC8A ");
	if (ap_qci_info->apsb)
		n += sysfs_emit_at(buf, n, "APSB ");

	sysfs_emit_at(buf, n == 0 ? 0 : n - 1, "\n");

	return n;
}

static BUS_ATTR_RO(features);

static struct attribute *ap_bus_attrs[] = {
	&bus_attr_ap_domain.attr,
	&bus_attr_ap_control_domain_mask.attr,
	&bus_attr_ap_usage_domain_mask.attr,
	&bus_attr_ap_adapter_mask.attr,
	&bus_attr_config_time.attr,
	&bus_attr_poll_thread.attr,
	&bus_attr_ap_interrupts.attr,
	&bus_attr_poll_timeout.attr,
	&bus_attr_ap_max_domain_id.attr,
	&bus_attr_ap_max_adapter_id.attr,
	&bus_attr_apmask.attr,
	&bus_attr_aqmask.attr,
	&bus_attr_scans.attr,
	&bus_attr_bindings.attr,
	&bus_attr_features.attr,
	NULL,
};
ATTRIBUTE_GROUPS(ap_bus);

static const struct bus_type ap_bus_type = {
	.name = "ap",
	.bus_groups = ap_bus_groups,
	.match = &ap_bus_match,
	.uevent = &ap_uevent,
	.probe = ap_device_probe,
	.remove = ap_device_remove,
};

/**
 * ap_select_domain(): Select an AP domain if possible and we haven't
 * already done so before.
 */
static void ap_select_domain(void)
{
	struct ap_queue_status status;
	int card, dom;

	/*
	 * Choose the default domain. Either the one specified with
	 * the "domain=" parameter or the first domain with at least
	 * one valid APQN.
	 */
	spin_lock_bh(&ap_domain_lock);
	if (ap_domain_index >= 0) {
		/* Domain has already been selected. */
		goto out;
	}
	for (dom = 0; dom <= ap_max_domain_id; dom++) {
		if (!ap_test_config_usage_domain(dom) ||
		    !test_bit_inv(dom, ap_perms.aqm))
			continue;
		for (card = 0; card <= ap_max_adapter_id; card++) {
			if (!ap_test_config_card_id(card) ||
			    !test_bit_inv(card, ap_perms.apm))
				continue;
			status = ap_test_queue(AP_MKQID(card, dom),
					       ap_apft_available(),
					       NULL);
			if (status.response_code == AP_RESPONSE_NORMAL)
				break;
		}
		if (card <= ap_max_adapter_id)
			break;
	}
	if (dom <= ap_max_domain_id) {
		ap_domain_index = dom;
		AP_DBF_INFO("%s new default domain is %d\n",
			    __func__, ap_domain_index);
	}
out:
	spin_unlock_bh(&ap_domain_lock);
}

/*
 * This function checks the type and returns either 0 for not
 * supported or the highest compatible type value (which may
 * include the input type value).
 */
static int ap_get_compatible_type(ap_qid_t qid, int rawtype, unsigned int func)
{
	int comp_type = 0;

	/* < CEX4 is not supported */
	if (rawtype < AP_DEVICE_TYPE_CEX4) {
		AP_DBF_WARN("%s queue=%02x.%04x unsupported type %d\n",
			    __func__, AP_QID_CARD(qid),
			    AP_QID_QUEUE(qid), rawtype);
		return 0;
	}
	/* up to CEX8 known and fully supported */
	if (rawtype <= AP_DEVICE_TYPE_CEX8)
		return rawtype;
	/*
	 * unknown new type > CEX8, check for compatibility
	 * to the highest known and supported type which is
	 * currently CEX8 with the help of the QACT function.
	 */
	if (ap_qact_available()) {
		struct ap_queue_status status;
		union ap_qact_ap_info apinfo = {0};

		apinfo.mode = (func >> 26) & 0x07;
		apinfo.cat = AP_DEVICE_TYPE_CEX8;
		status = ap_qact(qid, 0, &apinfo);
		if (status.response_code == AP_RESPONSE_NORMAL &&
		    apinfo.cat >= AP_DEVICE_TYPE_CEX4 &&
		    apinfo.cat <= AP_DEVICE_TYPE_CEX8)
			comp_type = apinfo.cat;
	}
	if (!comp_type)
		AP_DBF_WARN("%s queue=%02x.%04x unable to map type %d\n",
			    __func__, AP_QID_CARD(qid),
			    AP_QID_QUEUE(qid), rawtype);
	else if (comp_type != rawtype)
		AP_DBF_INFO("%s queue=%02x.%04x map type %d to %d\n",
			    __func__, AP_QID_CARD(qid), AP_QID_QUEUE(qid),
			    rawtype, comp_type);
	return comp_type;
}

/*
 * Helper function to be used with bus_find_dev
 * matches for the card device with the given id
 */
static int __match_card_device_with_id(struct device *dev, const void *data)
{
	return is_card_dev(dev) && to_ap_card(dev)->id == (int)(long)(void *)data;
}

/*
 * Helper function to be used with bus_find_dev
 * matches for the queue device with a given qid
 */
static int __match_queue_device_with_qid(struct device *dev, const void *data)
{
	return is_queue_dev(dev) && to_ap_queue(dev)->qid == (int)(long)data;
}

/*
 * Helper function to be used with bus_find_dev
 * matches any queue device with given queue id
 */
static int __match_queue_device_with_queue_id(struct device *dev, const void *data)
{
	return is_queue_dev(dev) &&
		AP_QID_QUEUE(to_ap_queue(dev)->qid) == (int)(long)data;
}

/* Helper function for notify_config_changed */
static int __drv_notify_config_changed(struct device_driver *drv, void *data)
{
	struct ap_driver *ap_drv = to_ap_drv(drv);

	if (try_module_get(drv->owner)) {
		if (ap_drv->on_config_changed)
			ap_drv->on_config_changed(ap_qci_info, ap_qci_info_old);
		module_put(drv->owner);
	}

	return 0;
}

/* Notify all drivers about an qci config change */
static inline void notify_config_changed(void)
{
	bus_for_each_drv(&ap_bus_type, NULL, NULL,
			 __drv_notify_config_changed);
}

/* Helper function for notify_scan_complete */
static int __drv_notify_scan_complete(struct device_driver *drv, void *data)
{
	struct ap_driver *ap_drv = to_ap_drv(drv);

	if (try_module_get(drv->owner)) {
		if (ap_drv->on_scan_complete)
			ap_drv->on_scan_complete(ap_qci_info,
						 ap_qci_info_old);
		module_put(drv->owner);
	}

	return 0;
}

/* Notify all drivers about bus scan complete */
static inline void notify_scan_complete(void)
{
	bus_for_each_drv(&ap_bus_type, NULL, NULL,
			 __drv_notify_scan_complete);
}

/*
 * Helper function for ap_scan_bus().
 * Remove card device and associated queue devices.
 */
static inline void ap_scan_rm_card_dev_and_queue_devs(struct ap_card *ac)
{
	bus_for_each_dev(&ap_bus_type, NULL,
			 (void *)(long)ac->id,
			 __ap_queue_devices_with_id_unregister);
	device_unregister(&ac->ap_dev.device);
}

/*
 * Helper function for ap_scan_bus().
 * Does the scan bus job for all the domains within
 * a valid adapter given by an ap_card ptr.
 */
static inline void ap_scan_domains(struct ap_card *ac)
{
	struct ap_tapq_hwinfo hwinfo;
	bool decfg, chkstop;
	struct ap_queue *aq;
	struct device *dev;
	ap_qid_t qid;
	int rc, dom;

	/*
	 * Go through the configuration for the domains and compare them
	 * to the existing queue devices. Also take care of the config
	 * and error state for the queue devices.
	 */

	for (dom = 0; dom <= ap_max_domain_id; dom++) {
		qid = AP_MKQID(ac->id, dom);
		dev = bus_find_device(&ap_bus_type, NULL,
				      (void *)(long)qid,
				      __match_queue_device_with_qid);
		aq = dev ? to_ap_queue(dev) : NULL;
		if (!ap_test_config_usage_domain(dom)) {
			if (dev) {
				AP_DBF_INFO("%s(%d,%d) not in config anymore, rm queue dev\n",
					    __func__, ac->id, dom);
				device_unregister(dev);
			}
			goto put_dev_and_continue;
		}
		/* domain is valid, get info from this APQN */
		rc = ap_queue_info(qid, &hwinfo, &decfg, &chkstop);
		switch (rc) {
		case -1:
			if (dev) {
				AP_DBF_INFO("%s(%d,%d) queue_info() failed, rm queue dev\n",
					    __func__, ac->id, dom);
				device_unregister(dev);
			}
			fallthrough;
		case 0:
			goto put_dev_and_continue;
		default:
			break;
		}
		/* if no queue device exists, create a new one */
		if (!aq) {
			aq = ap_queue_create(qid, ac->ap_dev.device_type);
			if (!aq) {
				AP_DBF_WARN("%s(%d,%d) ap_queue_create() failed\n",
					    __func__, ac->id, dom);
				continue;
			}
			aq->card = ac;
			aq->config = !decfg;
			aq->chkstop = chkstop;
			aq->se_bstate = hwinfo.bs;
			dev = &aq->ap_dev.device;
			dev->bus = &ap_bus_type;
			dev->parent = &ac->ap_dev.device;
			dev_set_name(dev, "%02x.%04x", ac->id, dom);
			/* register queue device */
			rc = device_register(dev);
			if (rc) {
				AP_DBF_WARN("%s(%d,%d) device_register() failed\n",
					    __func__, ac->id, dom);
				goto put_dev_and_continue;
			}
			/* get it and thus adjust reference counter */
			get_device(dev);
			if (decfg) {
				AP_DBF_INFO("%s(%d,%d) new (decfg) queue dev created\n",
					    __func__, ac->id, dom);
			} else if (chkstop) {
				AP_DBF_INFO("%s(%d,%d) new (chkstop) queue dev created\n",
					    __func__, ac->id, dom);
			} else {
				/* nudge the queue's state machine */
				ap_queue_init_state(aq);
				AP_DBF_INFO("%s(%d,%d) new queue dev created\n",
					    __func__, ac->id, dom);
			}
			goto put_dev_and_continue;
		}
		/* handle state changes on already existing queue device */
		spin_lock_bh(&aq->lock);
		/* SE bind state */
		aq->se_bstate = hwinfo.bs;
		/* checkstop state */
		if (chkstop && !aq->chkstop) {
			/* checkstop on */
			aq->chkstop = true;
			if (aq->dev_state > AP_DEV_STATE_UNINITIATED) {
				aq->dev_state = AP_DEV_STATE_ERROR;
				aq->last_err_rc = AP_RESPONSE_CHECKSTOPPED;
			}
			spin_unlock_bh(&aq->lock);
			pr_debug("%s(%d,%d) queue dev checkstop on\n",
				 __func__, ac->id, dom);
			/* 'receive' pending messages with -EAGAIN */
			ap_flush_queue(aq);
			goto put_dev_and_continue;
		} else if (!chkstop && aq->chkstop) {
			/* checkstop off */
			aq->chkstop = false;
			if (aq->dev_state > AP_DEV_STATE_UNINITIATED)
				_ap_queue_init_state(aq);
			spin_unlock_bh(&aq->lock);
			pr_debug("%s(%d,%d) queue dev checkstop off\n",
				 __func__, ac->id, dom);
			goto put_dev_and_continue;
		}
		/* config state change */
		if (decfg && aq->config) {
			/* config off this queue device */
			aq->config = false;
			if (aq->dev_state > AP_DEV_STATE_UNINITIATED) {
				aq->dev_state = AP_DEV_STATE_ERROR;
				aq->last_err_rc = AP_RESPONSE_DECONFIGURED;
			}
			spin_unlock_bh(&aq->lock);
			pr_debug("%s(%d,%d) queue dev config off\n",
				 __func__, ac->id, dom);
			ap_send_config_uevent(&aq->ap_dev, aq->config);
			/* 'receive' pending messages with -EAGAIN */
			ap_flush_queue(aq);
			goto put_dev_and_continue;
		} else if (!decfg && !aq->config) {
			/* config on this queue device */
			aq->config = true;
			if (aq->dev_state > AP_DEV_STATE_UNINITIATED)
				_ap_queue_init_state(aq);
			spin_unlock_bh(&aq->lock);
			pr_debug("%s(%d,%d) queue dev config on\n",
				 __func__, ac->id, dom);
			ap_send_config_uevent(&aq->ap_dev, aq->config);
			goto put_dev_and_continue;
		}
		/* handle other error states */
		if (!decfg && aq->dev_state == AP_DEV_STATE_ERROR) {
			spin_unlock_bh(&aq->lock);
			/* 'receive' pending messages with -EAGAIN */
			ap_flush_queue(aq);
			/* re-init (with reset) the queue device */
			ap_queue_init_state(aq);
			AP_DBF_INFO("%s(%d,%d) queue dev reinit enforced\n",
				    __func__, ac->id, dom);
			goto put_dev_and_continue;
		}
		spin_unlock_bh(&aq->lock);
put_dev_and_continue:
		put_device(dev);
	}
}

/*
 * Helper function for ap_scan_bus().
 * Does the scan bus job for the given adapter id.
 */
static inline void ap_scan_adapter(int ap)
{
	struct ap_tapq_hwinfo hwinfo;
	int rc, dom, comp_type;
	bool decfg, chkstop;
	struct ap_card *ac;
	struct device *dev;
	ap_qid_t qid;

	/* Is there currently a card device for this adapter ? */
	dev = bus_find_device(&ap_bus_type, NULL,
			      (void *)(long)ap,
			      __match_card_device_with_id);
	ac = dev ? to_ap_card(dev) : NULL;

	/* Adapter not in configuration ? */
	if (!ap_test_config_card_id(ap)) {
		if (ac) {
			AP_DBF_INFO("%s(%d) ap not in config any more, rm card and queue devs\n",
				    __func__, ap);
			ap_scan_rm_card_dev_and_queue_devs(ac);
			put_device(dev);
		}
		return;
	}

	/*
	 * Adapter ap is valid in the current configuration. So do some checks:
	 * If no card device exists, build one. If a card device exists, check
	 * for type and functions changed. For all this we need to find a valid
	 * APQN first.
	 */

	for (dom = 0; dom <= ap_max_domain_id; dom++)
		if (ap_test_config_usage_domain(dom)) {
			qid = AP_MKQID(ap, dom);
			if (ap_queue_info(qid, &hwinfo, &decfg, &chkstop) > 0)
				break;
		}
	if (dom > ap_max_domain_id) {
		/* Could not find one valid APQN for this adapter */
		if (ac) {
			AP_DBF_INFO("%s(%d) no type info (no APQN found), rm card and queue devs\n",
				    __func__, ap);
			ap_scan_rm_card_dev_and_queue_devs(ac);
			put_device(dev);
		} else {
			pr_debug("%s(%d) no type info (no APQN found), ignored\n",
				 __func__, ap);
		}
		return;
	}
	if (!hwinfo.at) {
		/* No apdater type info available, an unusable adapter */
		if (ac) {
			AP_DBF_INFO("%s(%d) no valid type (0) info, rm card and queue devs\n",
				    __func__, ap);
			ap_scan_rm_card_dev_and_queue_devs(ac);
			put_device(dev);
		} else {
			pr_debug("%s(%d) no valid type (0) info, ignored\n",
				 __func__, ap);
		}
		return;
	}
	hwinfo.value &= TAPQ_CARD_HWINFO_MASK; /* filter card specific hwinfo */
	if (ac) {
		/* Check APQN against existing card device for changes */
		if (ac->hwinfo.at != hwinfo.at) {
			AP_DBF_INFO("%s(%d) hwtype %d changed, rm card and queue devs\n",
				    __func__, ap, hwinfo.at);
			ap_scan_rm_card_dev_and_queue_devs(ac);
			put_device(dev);
			ac = NULL;
		} else if (ac->hwinfo.fac != hwinfo.fac) {
			AP_DBF_INFO("%s(%d) functions 0x%08x changed, rm card and queue devs\n",
				    __func__, ap, hwinfo.fac);
			ap_scan_rm_card_dev_and_queue_devs(ac);
			put_device(dev);
			ac = NULL;
		} else {
			/* handle checkstop state change */
			if (chkstop && !ac->chkstop) {
				/* checkstop on */
				ac->chkstop = true;
				AP_DBF_INFO("%s(%d) card dev checkstop on\n",
					    __func__, ap);
			} else if (!chkstop && ac->chkstop) {
				/* checkstop off */
				ac->chkstop = false;
				AP_DBF_INFO("%s(%d) card dev checkstop off\n",
					    __func__, ap);
			}
			/* handle config state change */
			if (decfg && ac->config) {
				ac->config = false;
				AP_DBF_INFO("%s(%d) card dev config off\n",
					    __func__, ap);
				ap_send_config_uevent(&ac->ap_dev, ac->config);
			} else if (!decfg && !ac->config) {
				ac->config = true;
				AP_DBF_INFO("%s(%d) card dev config on\n",
					    __func__, ap);
				ap_send_config_uevent(&ac->ap_dev, ac->config);
			}
		}
	}

	if (!ac) {
		/* Build a new card device */
		comp_type = ap_get_compatible_type(qid, hwinfo.at, hwinfo.fac);
		if (!comp_type) {
			AP_DBF_WARN("%s(%d) type %d, can't get compatibility type\n",
				    __func__, ap, hwinfo.at);
			return;
		}
		ac = ap_card_create(ap, hwinfo, comp_type);
		if (!ac) {
			AP_DBF_WARN("%s(%d) ap_card_create() failed\n",
				    __func__, ap);
			return;
		}
		ac->config = !decfg;
		ac->chkstop = chkstop;
		dev = &ac->ap_dev.device;
		dev->bus = &ap_bus_type;
		dev->parent = ap_root_device;
		dev_set_name(dev, "card%02x", ap);
		/* maybe enlarge ap_max_msg_size to support this card */
		if (ac->maxmsgsize > atomic_read(&ap_max_msg_size)) {
			atomic_set(&ap_max_msg_size, ac->maxmsgsize);
			AP_DBF_INFO("%s(%d) ap_max_msg_size update to %d byte\n",
				    __func__, ap,
				    atomic_read(&ap_max_msg_size));
		}
		/* Register the new card device with AP bus */
		rc = device_register(dev);
		if (rc) {
			AP_DBF_WARN("%s(%d) device_register() failed\n",
				    __func__, ap);
			put_device(dev);
			return;
		}
		/* get it and thus adjust reference counter */
		get_device(dev);
		if (decfg)
			AP_DBF_INFO("%s(%d) new (decfg) card dev type=%d func=0x%08x created\n",
				    __func__, ap, hwinfo.at, hwinfo.fac);
		else if (chkstop)
			AP_DBF_INFO("%s(%d) new (chkstop) card dev type=%d func=0x%08x created\n",
				    __func__, ap, hwinfo.at, hwinfo.fac);
		else
			AP_DBF_INFO("%s(%d) new card dev type=%d func=0x%08x created\n",
				    __func__, ap, hwinfo.at, hwinfo.fac);
	}

	/* Verify the domains and the queue devices for this card */
	ap_scan_domains(ac);

	/* release the card device */
	put_device(&ac->ap_dev.device);
}

/**
 * ap_get_configuration - get the host AP configuration
 *
 * Stores the host AP configuration information returned from the previous call
 * to Query Configuration Information (QCI), then retrieves and stores the
 * current AP configuration returned from QCI.
 *
 * Return: true if the host AP configuration changed between calls to QCI;
 * otherwise, return false.
 */
static bool ap_get_configuration(void)
{
	if (!ap_qci_info)	/* QCI not supported */
		return false;

	memcpy(ap_qci_info_old, ap_qci_info, sizeof(*ap_qci_info));
	ap_fetch_qci_info(ap_qci_info);

	return memcmp(ap_qci_info, ap_qci_info_old,
		      sizeof(struct ap_config_info)) != 0;
}

/*
 * ap_config_has_new_aps - Check current against old qci info if
 * new adapters have appeared. Returns true if at least one new
 * adapter in the apm mask is showing up. Existing adapters or
 * receding adapters are not counted.
 */
static bool ap_config_has_new_aps(void)
{

	unsigned long m[BITS_TO_LONGS(AP_DEVICES)];

	if (!ap_qci_info)
		return false;

	bitmap_andnot(m, (unsigned long *)ap_qci_info->apm,
		      (unsigned long *)ap_qci_info_old->apm, AP_DEVICES);
	if (!bitmap_empty(m, AP_DEVICES))
		return true;

	return false;
}

/*
 * ap_config_has_new_doms - Check current against old qci info if
 * new (usage) domains have appeared. Returns true if at least one
 * new domain in the aqm mask is showing up. Existing domains or
 * receding domains are not counted.
 */
static bool ap_config_has_new_doms(void)
{
	unsigned long m[BITS_TO_LONGS(AP_DOMAINS)];

	if (!ap_qci_info)
		return false;

	bitmap_andnot(m, (unsigned long *)ap_qci_info->aqm,
		      (unsigned long *)ap_qci_info_old->aqm, AP_DOMAINS);
	if (!bitmap_empty(m, AP_DOMAINS))
		return true;

	return false;
}

/**
 * ap_scan_bus(): Scan the AP bus for new devices
 * Always run under mutex ap_scan_bus_mutex protection
 * which needs to get locked/unlocked by the caller!
 * Returns true if any config change has been detected
 * during the scan, otherwise false.
 */
static bool ap_scan_bus(void)
{
	bool config_changed;
	int ap;

	pr_debug(">%s\n", __func__);

	/* (re-)fetch configuration via QCI */
	config_changed = ap_get_configuration();
	if (config_changed) {
		if (ap_config_has_new_aps() || ap_config_has_new_doms()) {
			/*
			 * Appearance of new adapters and/or domains need to
			 * build new ap devices which need to get bound to an
			 * device driver. Thus reset the APQN bindings complete
			 * completion.
			 */
			reinit_completion(&ap_apqn_bindings_complete);
		}
		/* post a config change notify */
		notify_config_changed();
	}
	ap_select_domain();

	/* loop over all possible adapters */
	for (ap = 0; ap <= ap_max_adapter_id; ap++)
		ap_scan_adapter(ap);

	/* scan complete notify */
	if (config_changed)
		notify_scan_complete();

	/* check if there is at least one queue available with default domain */
	if (ap_domain_index >= 0) {
		struct device *dev =
			bus_find_device(&ap_bus_type, NULL,
					(void *)(long)ap_domain_index,
					__match_queue_device_with_queue_id);
		if (dev)
			put_device(dev);
		else
			AP_DBF_INFO("%s no queue device with default domain %d available\n",
				    __func__, ap_domain_index);
	}

	if (atomic64_inc_return(&ap_scan_bus_count) == 1) {
		pr_debug("%s init scan complete\n", __func__);
		ap_send_init_scan_done_uevent();
	}

	ap_check_bindings_complete();

	mod_timer(&ap_scan_bus_timer, jiffies + ap_scan_bus_time * HZ);

	pr_debug("<%s config_changed=%d\n", __func__, config_changed);

	return config_changed;
}

/*
 * Callback for the ap_scan_bus_timer
 * Runs periodically, workqueue timer (ap_scan_bus_time)
 */
static void ap_scan_bus_timer_callback(struct timer_list *unused)
{
	/*
	 * schedule work into the system long wq which when
	 * the work is finally executed, calls the AP bus scan.
	 */
	queue_work(system_long_wq, &ap_scan_bus_work);
}

/*
 * Callback for the ap_scan_bus_work
 */
static void ap_scan_bus_wq_callback(struct work_struct *unused)
{
	/*
	 * Try to invoke an ap_scan_bus(). If the mutex acquisition
	 * fails there is currently another task already running the
	 * AP scan bus and there is no need to wait and re-trigger the
	 * scan again. Please note at the end of the scan bus function
	 * the AP scan bus timer is re-armed which triggers then the
	 * ap_scan_bus_timer_callback which enqueues a work into the
	 * system_long_wq which invokes this function here again.
	 */
	if (mutex_trylock(&ap_scan_bus_mutex)) {
		ap_scan_bus_result = ap_scan_bus();
		mutex_unlock(&ap_scan_bus_mutex);
	}
}

static int __init ap_debug_init(void)
{
	ap_dbf_info = debug_register("ap", 2, 1,
				     AP_DBF_MAX_SPRINTF_ARGS * sizeof(long));
	debug_register_view(ap_dbf_info, &debug_sprintf_view);
	debug_set_level(ap_dbf_info, DBF_ERR);

	return 0;
}

static void __init ap_perms_init(void)
{
	/* all resources usable if no kernel parameter string given */
	memset(&ap_perms.ioctlm, 0xFF, sizeof(ap_perms.ioctlm));
	memset(&ap_perms.apm, 0xFF, sizeof(ap_perms.apm));
	memset(&ap_perms.aqm, 0xFF, sizeof(ap_perms.aqm));

	/* apm kernel parameter string */
	if (apm_str) {
		memset(&ap_perms.apm, 0, sizeof(ap_perms.apm));
		ap_parse_mask_str(apm_str, ap_perms.apm, AP_DEVICES,
				  &ap_perms_mutex);
	}

	/* aqm kernel parameter string */
	if (aqm_str) {
		memset(&ap_perms.aqm, 0, sizeof(ap_perms.aqm));
		ap_parse_mask_str(aqm_str, ap_perms.aqm, AP_DOMAINS,
				  &ap_perms_mutex);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * ap_module_init(): The module initialization code.
 *
 * Initializes the module.
 */
<<<<<<< HEAD
int __init ap_module_init(void)
{
	int rc, i;

	if (ap_domain_index < -1 || ap_domain_index >= AP_DOMAINS) {
		pr_warning("%d is not a valid cryptographic domain\n",
			   ap_domain_index);
		return -EINVAL;
	}
	/* In resume callback we need to know if the user had set the domain.
	 * If so, we can not just reset it.
	 */
	if (ap_domain_index >= 0)
		user_set_domain = 1;

	if (ap_instructions_available() != 0) {
		pr_warning("The hardware system does not support "
			   "AP instructions\n");
		return -ENODEV;
	}
	if (ap_interrupts_available()) {
		isc_register(AP_ISC);
		ap_interrupt_indicator = s390_register_adapter_interrupt(
			&ap_interrupt_handler, NULL, AP_ISC);
		if (IS_ERR(ap_interrupt_indicator)) {
			ap_interrupt_indicator = NULL;
			isc_unregister(AP_ISC);
		}
	}

	register_reset_call(&ap_reset_call);
=======
static int __init ap_module_init(void)
{
	int rc;

	rc = ap_debug_init();
	if (rc)
		return rc;

	if (!ap_instructions_available()) {
		pr_warn("The hardware system does not support AP instructions\n");
		return -ENODEV;
	}

	/* init ap_queue hashtable */
	hash_init(ap_queues);

	/* set up the AP permissions (ioctls, ap and aq masks) */
	ap_perms_init();

	/* Get AP configuration data if available */
	ap_init_qci_info();

	/* check default domain setting */
	if (ap_domain_index < -1 || ap_domain_index > ap_max_domain_id ||
	    (ap_domain_index >= 0 &&
	     !test_bit_inv(ap_domain_index, ap_perms.aqm))) {
		pr_warn("%d is not a valid cryptographic domain\n",
			ap_domain_index);
		ap_domain_index = -1;
	}

	/* enable interrupts if available */
	if (ap_interrupts_available() && ap_useirq) {
		rc = register_adapter_interrupt(&ap_airq);
		ap_irq_flag = (rc == 0);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Create /sys/bus/ap. */
	rc = bus_register(&ap_bus_type);
	if (rc)
		goto out;
<<<<<<< HEAD
	for (i = 0; ap_bus_attrs[i]; i++) {
		rc = bus_create_file(&ap_bus_type, ap_bus_attrs[i]);
		if (rc)
			goto out_bus;
	}

	/* Create /sys/devices/ap. */
	ap_root_device = root_device_register("ap");
	rc = IS_ERR(ap_root_device) ? PTR_ERR(ap_root_device) : 0;
	if (rc)
		goto out_bus;

	ap_work_queue = create_singlethread_workqueue("kapwork");
	if (!ap_work_queue) {
		rc = -ENOMEM;
		goto out_root;
	}

	if (ap_select_domain() == 0)
		ap_scan_bus(NULL);

	/* Setup the AP bus rescan timer. */
	init_timer(&ap_config_timer);
	ap_config_timer.function = ap_config_timeout;
	ap_config_timer.data = 0;
	ap_config_timer.expires = jiffies + ap_config_time * HZ;
	add_timer(&ap_config_timer);

	/* Setup the high resultion poll timer.
	 * If we are running under z/VM adjust polling to z/VM polling rate.
	 */
	if (MACHINE_IS_VM)
		poll_timeout = 1500000;
	spin_lock_init(&ap_poll_timer_lock);
=======

	/* Create /sys/devices/ap. */
	ap_root_device = root_device_register("ap");
	rc = PTR_ERR_OR_ZERO(ap_root_device);
	if (rc)
		goto out_bus;
	ap_root_device->bus = &ap_bus_type;

	/* Setup the AP bus rescan timer. */
	timer_setup(&ap_scan_bus_timer, ap_scan_bus_timer_callback, 0);

	/*
	 * Setup the high resolution poll timer.
	 * If we are running under z/VM adjust polling to z/VM polling rate.
	 */
	if (MACHINE_IS_VM)
		poll_high_timeout = 1500000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hrtimer_init(&ap_poll_timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	ap_poll_timer.function = ap_poll_timeout;

	/* Start the low priority AP bus poll thread. */
	if (ap_thread_flag) {
		rc = ap_poll_thread_start();
		if (rc)
			goto out_work;
	}

<<<<<<< HEAD
	return 0;

out_work:
	del_timer_sync(&ap_config_timer);
	hrtimer_cancel(&ap_poll_timer);
	destroy_workqueue(ap_work_queue);
out_root:
	root_device_unregister(ap_root_device);
out_bus:
	while (i--)
		bus_remove_file(&ap_bus_type, ap_bus_attrs[i]);
	bus_unregister(&ap_bus_type);
out:
	unregister_reset_call(&ap_reset_call);
	if (ap_using_interrupts()) {
		s390_unregister_adapter_interrupt(ap_interrupt_indicator, AP_ISC);
		isc_unregister(AP_ISC);
	}
	return rc;
}

static int __ap_match_all(struct device *dev, void *data)
{
	return 1;
}

/**
 * ap_modules_exit(): The module termination code
 *
 * Terminates the module.
 */
void ap_module_exit(void)
{
	int i;
	struct device *dev;

	ap_reset_domain();
	ap_poll_thread_stop();
	del_timer_sync(&ap_config_timer);
	hrtimer_cancel(&ap_poll_timer);
	destroy_workqueue(ap_work_queue);
	tasklet_kill(&ap_tasklet);
	root_device_unregister(ap_root_device);
	while ((dev = bus_find_device(&ap_bus_type, NULL, NULL,
		    __ap_match_all)))
	{
		device_unregister(dev);
		put_device(dev);
	}
	for (i = 0; ap_bus_attrs[i]; i++)
		bus_remove_file(&ap_bus_type, ap_bus_attrs[i]);
	bus_unregister(&ap_bus_type);
	unregister_reset_call(&ap_reset_call);
	if (ap_using_interrupts()) {
		s390_unregister_adapter_interrupt(ap_interrupt_indicator, AP_ISC);
		isc_unregister(AP_ISC);
	}
}

module_init(ap_module_init);
module_exit(ap_module_exit);
=======
	queue_work(system_long_wq, &ap_scan_bus_work);

	return 0;

out_work:
	hrtimer_cancel(&ap_poll_timer);
	root_device_unregister(ap_root_device);
out_bus:
	bus_unregister(&ap_bus_type);
out:
	if (ap_irq_flag)
		unregister_adapter_interrupt(&ap_airq);
	kfree(ap_qci_info);
	return rc;
}
device_initcall(ap_module_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
