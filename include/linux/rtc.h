<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic RTC interface.
 * This version contains the part of the user interface to the Real Time Clock
 * service. It is used with both the legacy mc146818 and also  EFI
 * Struct rtc_time and first 12 ioctl by Paul Gortmaker, 1996 - separated out
 * from <linux/mc146818rtc.h> to this file for 2.4 kernels.
 *
 * Copyright (C) 1999 Hewlett-Packard Co.
 * Copyright (C) 1999 Stephane Eranian <eranian@hpl.hp.com>
 */
#ifndef _LINUX_RTC_H_
#define _LINUX_RTC_H_

<<<<<<< HEAD
/*
 * The struct used to pass data via the following ioctl. Similar to the
 * struct tm in <time.h>, but it needs to be here so that the kernel
 * source is self contained, allowing cross-compiles, etc. etc.
 */

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

/*
 * This data structure is inspired by the EFI (v0.92) wakeup
 * alarm API.
 */
struct rtc_wkalrm {
	unsigned char enabled;	/* 0 = alarm disabled, 1 = alarm enabled */
	unsigned char pending;  /* 0 = alarm not pending, 1 = alarm pending */
	struct rtc_time time;	/* time the alarm is set to */
};

/*
 * Data structure to control PLL correction some better RTC feature
 * pll_value is used to get or set current value of correction,
 * the rest of the struct is used to query HW capabilities.
 * This is modeled after the RTC used in Q40/Q60 computers but
 * should be sufficiently flexible for other devices
 *
 * +ve pll_value means clock will run faster by
 *   pll_value*pll_posmult/pll_clock
 * -ve pll_value means clock will run slower by
 *   pll_value*pll_negmult/pll_clock
 */

struct rtc_pll_info {
	int pll_ctrl;       /* placeholder for fancier control */
	int pll_value;      /* get/set correction value */
	int pll_max;        /* max +ve (faster) adjustment value */
	int pll_min;        /* max -ve (slower) adjustment value */
	int pll_posmult;    /* factor for +ve correction */
	int pll_negmult;    /* factor for -ve correction */
	long pll_clock;     /* base PLL frequency */
};

/*
 * ioctl calls that are permitted to the /dev/rtc interface, if
 * any of the RTC drivers are enabled.
 */

#define RTC_AIE_ON	_IO('p', 0x01)	/* Alarm int. enable on		*/
#define RTC_AIE_OFF	_IO('p', 0x02)	/* ... off			*/
#define RTC_UIE_ON	_IO('p', 0x03)	/* Update int. enable on	*/
#define RTC_UIE_OFF	_IO('p', 0x04)	/* ... off			*/
#define RTC_PIE_ON	_IO('p', 0x05)	/* Periodic int. enable on	*/
#define RTC_PIE_OFF	_IO('p', 0x06)	/* ... off			*/
#define RTC_WIE_ON	_IO('p', 0x0f)  /* Watchdog int. enable on	*/
#define RTC_WIE_OFF	_IO('p', 0x10)  /* ... off			*/

#define RTC_ALM_SET	_IOW('p', 0x07, struct rtc_time) /* Set alarm time  */
#define RTC_ALM_READ	_IOR('p', 0x08, struct rtc_time) /* Read alarm time */
#define RTC_RD_TIME	_IOR('p', 0x09, struct rtc_time) /* Read RTC time   */
#define RTC_SET_TIME	_IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */
#define RTC_IRQP_READ	_IOR('p', 0x0b, unsigned long)	 /* Read IRQ rate   */
#define RTC_IRQP_SET	_IOW('p', 0x0c, unsigned long)	 /* Set IRQ rate    */
#define RTC_EPOCH_READ	_IOR('p', 0x0d, unsigned long)	 /* Read epoch      */
#define RTC_EPOCH_SET	_IOW('p', 0x0e, unsigned long)	 /* Set epoch       */

#define RTC_WKALM_SET	_IOW('p', 0x0f, struct rtc_wkalrm)/* Set wakeup alarm*/
#define RTC_WKALM_RD	_IOR('p', 0x10, struct rtc_wkalrm)/* Get wakeup alarm*/

#define RTC_PLL_GET	_IOR('p', 0x11, struct rtc_pll_info)  /* Get PLL correction */
#define RTC_PLL_SET	_IOW('p', 0x12, struct rtc_pll_info)  /* Set PLL correction */
#ifdef CONFIG_LGE_PM_RTC_PWROFF_ALARM
#define RTC_DEVICE_UP	_IOW('p', 0x13, struct rtc_wkalrm)  /* Set Power Off alarm */
#endif

/* interrupt flags */
#define RTC_IRQF 0x80	/* Any of the following is active */
#define RTC_PF 0x40	/* Periodic interrupt */
#define RTC_AF 0x20	/* Alarm interrupt */
#define RTC_UF 0x10	/* Update interrupt for 1Hz RTC */


#define RTC_MAX_FREQ	8192

#ifdef __KERNEL__

#include <linux/types.h>
#include <linux/interrupt.h>
=======

#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/nvmem-provider.h>
#include <uapi/linux/rtc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int rtc_month_days(unsigned int month, unsigned int year);
extern int rtc_year_days(unsigned int day, unsigned int month, unsigned int year);
extern int rtc_valid_tm(struct rtc_time *tm);
<<<<<<< HEAD
extern int rtc_tm_to_time(struct rtc_time *tm, unsigned long *time);
extern void rtc_time_to_tm(unsigned long time, struct rtc_time *tm);
ktime_t rtc_tm_to_ktime(struct rtc_time tm);
struct rtc_time rtc_ktime_to_tm(ktime_t kt);

=======
extern time64_t rtc_tm_to_time64(struct rtc_time *tm);
extern void rtc_time64_to_tm(time64_t time, struct rtc_time *tm);
ktime_t rtc_tm_to_ktime(struct rtc_time tm);
struct rtc_time rtc_ktime_to_tm(ktime_t kt);

/*
 * rtc_tm_sub - Return the difference in seconds.
 */
static inline time64_t rtc_tm_sub(struct rtc_time *lhs, struct rtc_time *rhs)
{
	return rtc_tm_to_time64(lhs) - rtc_tm_to_time64(rhs);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/device.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/timerqueue.h>
#include <linux/workqueue.h>

<<<<<<< HEAD
extern struct class *rtc_class;
=======
extern const struct class rtc_class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * For these RTC methods the device parameter is the physical device
 * on whatever bus holds the hardware (I2C, Platform, SPI, etc), which
 * was passed to rtc_device_register().  Its driver_data normally holds
 * device state, including the rtc_device pointer for the RTC.
 *
 * Most of these methods are called with rtc_device.ops_lock held,
 * through the rtc_*(struct rtc_device *, ...) calls.
 *
 * The (current) exceptions are mostly filesystem hooks:
 *   - the proc() hook for procfs
<<<<<<< HEAD
 *   - non-ioctl() chardev hooks:  open(), release(), read_callback()
 *
 * REVISIT those periodic irq calls *do* have ops_lock when they're
 * issued through ioctl() ...
 */
struct rtc_class_ops {
	int (*open)(struct device *);
	void (*release)(struct device *);
=======
 */
struct rtc_class_ops {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*ioctl)(struct device *, unsigned int, unsigned long);
	int (*read_time)(struct device *, struct rtc_time *);
	int (*set_time)(struct device *, struct rtc_time *);
	int (*read_alarm)(struct device *, struct rtc_wkalrm *);
	int (*set_alarm)(struct device *, struct rtc_wkalrm *);
	int (*proc)(struct device *, struct seq_file *);
<<<<<<< HEAD
	int (*set_mmss)(struct device *, unsigned long secs);
	int (*read_callback)(struct device *, int data);
	int (*alarm_irq_enable)(struct device *, unsigned int enabled);
#ifdef CONFIG_LGE_PM_RTC_PWROFF_ALARM
	int (*set_po_alarm)(struct device *, struct rtc_wkalrm *);
#endif
};

#define RTC_DEVICE_NAME_SIZE 20
typedef struct rtc_task {
	void (*func)(void *private_data);
	void *private_data;
} rtc_task_t;


struct rtc_timer {
	struct rtc_task	task;
	struct timerqueue_node node;
	ktime_t period;
	int enabled;
};


/* flags */
#define RTC_DEV_BUSY 0

struct rtc_device
{
=======
	int (*alarm_irq_enable)(struct device *, unsigned int enabled);
	int (*read_offset)(struct device *, long *offset);
	int (*set_offset)(struct device *, long offset);
	int (*param_get)(struct device *, struct rtc_param *param);
	int (*param_set)(struct device *, struct rtc_param *param);
};

struct rtc_device;

struct rtc_timer {
	struct timerqueue_node node;
	ktime_t period;
	void (*func)(struct rtc_device *rtc);
	struct rtc_device *rtc;
	int enabled;
};

/* flags */
#define RTC_DEV_BUSY 0
#define RTC_NO_CDEV  1

struct rtc_device {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device dev;
	struct module *owner;

	int id;
<<<<<<< HEAD
	char name[RTC_DEVICE_NAME_SIZE];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	const struct rtc_class_ops *ops;
	struct mutex ops_lock;

	struct cdev char_dev;
	unsigned long flags;

	unsigned long irq_data;
	spinlock_t irq_lock;
	wait_queue_head_t irq_queue;
	struct fasync_struct *async_queue;

<<<<<<< HEAD
	struct rtc_task *irq_task;
	spinlock_t irq_task_lock;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int irq_freq;
	int max_user_freq;

	struct timerqueue_head timerqueue;
	struct rtc_timer aie_timer;
	struct rtc_timer uie_rtctimer;
	struct hrtimer pie_timer; /* sub second exp, so needs hrtimer */
	int pie_enabled;
	struct work_struct irqwork;
<<<<<<< HEAD
	/* Some hardware can't support UIE mode */
	int uie_unsupported;
=======

	/*
	 * This offset specifies the update timing of the RTC.
	 *
	 * tsched     t1 write(t2.tv_sec - 1sec))  t2 RTC increments seconds
	 *
	 * The offset defines how tsched is computed so that the write to
	 * the RTC (t2.tv_sec - 1sec) is correct versus the time required
	 * for the transport of the write and the time which the RTC needs
	 * to increment seconds the first time after the write (t2).
	 *
	 * For direct accessible RTCs tsched ~= t1 because the write time
	 * is negligible. For RTCs behind slow busses the transport time is
	 * significant and has to be taken into account.
	 *
	 * The time between the write (t1) and the first increment after
	 * the write (t2) is RTC specific. For a MC146818 RTC it's 500ms,
	 * for many others it's exactly 1 second. Consult the datasheet.
	 *
	 * The value of this offset is also used to calculate the to be
	 * written value (t2.tv_sec - 1sec) at tsched.
	 *
	 * The default value for this is NSEC_PER_SEC + 10 msec default
	 * transport time. The offset can be adjusted by drivers so the
	 * calculation for the to be written value at tsched becomes
	 * correct:
	 *
	 *	newval = tsched + set_offset_nsec - NSEC_PER_SEC
	 * and  (tsched + set_offset_nsec) % NSEC_PER_SEC == 0
	 */
	unsigned long set_offset_nsec;

	unsigned long features[BITS_TO_LONGS(RTC_FEATURE_CNT)];

	time64_t range_min;
	timeu64_t range_max;
	timeu64_t alarm_offset_max;
	time64_t start_secs;
	time64_t offset_secs;
	bool set_start_time;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_RTC_INTF_DEV_UIE_EMUL
	struct work_struct uie_task;
	struct timer_list uie_timer;
	/* Those fields are protected by rtc->irq_lock */
	unsigned int oldsecs;
	unsigned int uie_irq_active:1;
	unsigned int stop_uie_polling:1;
	unsigned int uie_task_active:1;
	unsigned int uie_timer_active:1;
#endif
};
#define to_rtc_device(d) container_of(d, struct rtc_device, dev)

<<<<<<< HEAD
extern struct rtc_device *rtc_device_register(const char *name,
					struct device *dev,
					const struct rtc_class_ops *ops,
					struct module *owner);
extern void rtc_device_unregister(struct rtc_device *rtc);

extern int rtc_read_time(struct rtc_device *rtc, struct rtc_time *tm);
extern int rtc_set_time(struct rtc_device *rtc, struct rtc_time *tm);
extern int rtc_set_mmss(struct rtc_device *rtc, unsigned long secs);
=======
#define rtc_lock(d) mutex_lock(&d->ops_lock)
#define rtc_unlock(d) mutex_unlock(&d->ops_lock)

/* useful timestamps */
#define RTC_TIMESTAMP_BEGIN_0000	-62167219200ULL /* 0000-01-01 00:00:00 */
#define RTC_TIMESTAMP_BEGIN_1900	-2208988800LL /* 1900-01-01 00:00:00 */
#define RTC_TIMESTAMP_BEGIN_2000	946684800LL /* 2000-01-01 00:00:00 */
#define RTC_TIMESTAMP_END_2063		2966371199LL /* 2063-12-31 23:59:59 */
#define RTC_TIMESTAMP_END_2079		3471292799LL /* 2079-12-31 23:59:59 */
#define RTC_TIMESTAMP_END_2099		4102444799LL /* 2099-12-31 23:59:59 */
#define RTC_TIMESTAMP_END_2199		7258118399LL /* 2199-12-31 23:59:59 */
#define RTC_TIMESTAMP_END_9999		253402300799LL /* 9999-12-31 23:59:59 */

extern struct rtc_device *devm_rtc_device_register(struct device *dev,
					const char *name,
					const struct rtc_class_ops *ops,
					struct module *owner);
struct rtc_device *devm_rtc_allocate_device(struct device *dev);
int __devm_rtc_register_device(struct module *owner, struct rtc_device *rtc);

extern int rtc_read_time(struct rtc_device *rtc, struct rtc_time *tm);
extern int rtc_set_time(struct rtc_device *rtc, struct rtc_time *tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __rtc_read_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm);
extern int rtc_read_alarm(struct rtc_device *rtc,
			struct rtc_wkalrm *alrm);
extern int rtc_set_alarm(struct rtc_device *rtc,
				struct rtc_wkalrm *alrm);
extern int rtc_initialize_alarm(struct rtc_device *rtc,
				struct rtc_wkalrm *alrm);
extern void rtc_update_irq(struct rtc_device *rtc,
			unsigned long num, unsigned long events);

<<<<<<< HEAD
extern struct rtc_device *rtc_class_open(char *name);
extern void rtc_class_close(struct rtc_device *rtc);

extern int rtc_irq_register(struct rtc_device *rtc,
				struct rtc_task *task);
extern void rtc_irq_unregister(struct rtc_device *rtc,
				struct rtc_task *task);
extern int rtc_irq_set_state(struct rtc_device *rtc,
				struct rtc_task *task, int enabled);
extern int rtc_irq_set_freq(struct rtc_device *rtc,
				struct rtc_task *task, int freq);
=======
extern struct rtc_device *rtc_class_open(const char *name);
extern void rtc_class_close(struct rtc_device *rtc);

extern int rtc_irq_set_state(struct rtc_device *rtc, int enabled);
extern int rtc_irq_set_freq(struct rtc_device *rtc, int freq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int rtc_update_irq_enable(struct rtc_device *rtc, unsigned int enabled);
extern int rtc_alarm_irq_enable(struct rtc_device *rtc, unsigned int enabled);
extern int rtc_dev_update_irq_enable_emul(struct rtc_device *rtc,
						unsigned int enabled);

void rtc_handle_legacy_irq(struct rtc_device *rtc, int num, int mode);
<<<<<<< HEAD
void rtc_aie_update_irq(void *private);
void rtc_uie_update_irq(void *private);
enum hrtimer_restart rtc_pie_update_irq(struct hrtimer *timer);

int rtc_register(rtc_task_t *task);
int rtc_unregister(rtc_task_t *task);
int rtc_control(rtc_task_t *t, unsigned int cmd, unsigned long arg);

void rtc_timer_init(struct rtc_timer *timer, void (*f)(void* p), void* data);
int rtc_timer_start(struct rtc_device *rtc, struct rtc_timer* timer,
			ktime_t expires, ktime_t period);
int rtc_timer_cancel(struct rtc_device *rtc, struct rtc_timer* timer);
void rtc_timer_do_work(struct work_struct *work);

#ifdef CONFIG_LGE_PM_RTC_PWROFF_ALARM
/*
enum RTC_PWR_TYPE {
	LG_RTC_POWER_ON_ENABLE,
	LG_RTC_POWER_ON_TIME,
	LG_RTC_POWER_ON_MAX,
};
*/

extern int rtc_set_po_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm);
/*extern void write_rtc_pwron_in_misc(struct rtc_wkalrm *alarm, enum RTC_PWR_TYPE type);*/
/*extern unsigned long read_rtc_pwron_in_misc(struct rtc_wkalrm *alarm, enum RTC_PWR_TYPE type);*/
#endif

=======
void rtc_aie_update_irq(struct rtc_device *rtc);
void rtc_uie_update_irq(struct rtc_device *rtc);
enum hrtimer_restart rtc_pie_update_irq(struct hrtimer *timer);

void rtc_timer_init(struct rtc_timer *timer, void (*f)(struct rtc_device *r),
		    struct rtc_device *rtc);
int rtc_timer_start(struct rtc_device *rtc, struct rtc_timer *timer,
		    ktime_t expires, ktime_t period);
void rtc_timer_cancel(struct rtc_device *rtc, struct rtc_timer *timer);
int rtc_read_offset(struct rtc_device *rtc, long *offset);
int rtc_set_offset(struct rtc_device *rtc, long offset);
void rtc_timer_do_work(struct work_struct *work);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline bool is_leap_year(unsigned int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400);
}

<<<<<<< HEAD
#ifdef CONFIG_RTC_HCTOSYS
=======
/**
 * rtc_bound_alarmtime() - Return alarm time bound by rtc limit
 * @rtc: Pointer to rtc device structure
 * @requested: Requested alarm timeout
 *
 * Return: Alarm timeout bound by maximum alarm time supported by rtc.
 */
static inline ktime_t rtc_bound_alarmtime(struct rtc_device *rtc,
					  ktime_t requested)
{
	if (rtc->alarm_offset_max &&
	    rtc->alarm_offset_max * MSEC_PER_SEC < ktime_to_ms(requested))
		return ms_to_ktime(rtc->alarm_offset_max * MSEC_PER_SEC);

	return requested;
}

#define devm_rtc_register_device(device) \
	__devm_rtc_register_device(THIS_MODULE, device)

#ifdef CONFIG_RTC_HCTOSYS_DEVICE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int rtc_hctosys_ret;
#else
#define rtc_hctosys_ret -ENODEV
#endif

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
#ifdef CONFIG_RTC_NVMEM
int devm_rtc_nvmem_register(struct rtc_device *rtc,
			    struct nvmem_config *nvmem_config);
#else
static inline int devm_rtc_nvmem_register(struct rtc_device *rtc,
					  struct nvmem_config *nvmem_config)
{
	return 0;
}
#endif

#ifdef CONFIG_RTC_INTF_SYSFS
int rtc_add_group(struct rtc_device *rtc, const struct attribute_group *grp);
int rtc_add_groups(struct rtc_device *rtc, const struct attribute_group **grps);
#else
static inline
int rtc_add_group(struct rtc_device *rtc, const struct attribute_group *grp)
{
	return 0;
}

static inline
int rtc_add_groups(struct rtc_device *rtc, const struct attribute_group **grps)
{
	return 0;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_RTC_H_ */
