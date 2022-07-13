<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/mmc/core/host.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007-2008 Pierre Ossman
 *  Copyright (C) 2010 Linus Walleij
<<<<<<< HEAD
 *  Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  MMC host class device management
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/idr.h>
<<<<<<< HEAD
#include <linux/pagemap.h>
#include <linux/export.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/pm_runtime.h>

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>

#include "core.h"
#include "host.h"

#define cls_dev_to_mmc_host(d)	container_of(d, struct mmc_host, class_dev)

static void mmc_host_classdev_release(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	kfree(host->wlock_name);
	kfree(host);
}

static int mmc_host_runtime_suspend(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int ret = 0;

	if (!mmc_use_core_runtime_pm(host))
		return 0;

	ret = mmc_suspend_host(host);
	if (ret < 0)
		pr_err("%s: %s: suspend host failed: %d\n", mmc_hostname(host),
		       __func__, ret);

	return ret;
}

static int mmc_host_runtime_resume(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int ret = 0;

	if (!mmc_use_core_runtime_pm(host))
		return 0;

	ret = mmc_resume_host(host);
	if (ret < 0) {
		pr_err("%s: %s: resume host: failed: ret: %d\n",
		       mmc_hostname(host), __func__, ret);
		if (pm_runtime_suspended(dev))
			BUG_ON(1);
	}

	return ret;
}

static int mmc_host_suspend(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int ret = 0;
	unsigned long flags;

	if (!mmc_use_core_pm(host))
		return 0;

	spin_lock_irqsave(&host->clk_lock, flags);
	/*
	 * let the driver know that suspend is in progress and must
	 * be aborted on receiving a sdio card interrupt
	 */
	host->dev_status = DEV_SUSPENDING;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	if (!pm_runtime_suspended(dev)) {
		ret = mmc_suspend_host(host);
		if (ret < 0)
			pr_err("%s: %s: failed: ret: %d\n", mmc_hostname(host),
			       __func__, ret);
	}
	/*
	 * If SDIO function driver doesn't want to power off the card,
	 * atleast turn off clocks to allow deep sleep.
	 */
	if (!ret && host->card && mmc_card_sdio(host->card) &&
	    host->ios.clock) {
		spin_lock_irqsave(&host->clk_lock, flags);
		host->clk_old = host->ios.clock;
		host->ios.clock = 0;
		host->clk_gated = true;
		spin_unlock_irqrestore(&host->clk_lock, flags);
		mmc_set_ios(host);
	}
	spin_lock_irqsave(&host->clk_lock, flags);
	host->dev_status = DEV_SUSPENDED;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	return ret;
}

static int mmc_host_resume(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int ret = 0;

	if (!mmc_use_core_pm(host))
		return 0;

	if (!pm_runtime_suspended(dev)) {
		ret = mmc_resume_host(host);
		if (ret < 0)
			pr_err("%s: %s: failed: ret: %d\n", mmc_hostname(host),
			       __func__, ret);
	}
	host->dev_status = DEV_RESUMED;
	return ret;
}

static const struct dev_pm_ops mmc_host_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(mmc_host_suspend, mmc_host_resume)
	SET_RUNTIME_PM_OPS(mmc_host_runtime_suspend, mmc_host_runtime_resume,
			   pm_generic_runtime_idle)
};

static struct class mmc_host_class = {
	.name		= "mmc_host",
	.dev_release	= mmc_host_classdev_release,
	.pm		= &mmc_host_pm_ops,
=======
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pagemap.h>
#include <linux/pm_wakeup.h>
#include <linux/export.h>
#include <linux/leds.h>
#include <linux/slab.h>

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/slot-gpio.h>

#include "core.h"
#include "crypto.h"
#include "host.h"
#include "slot-gpio.h"
#include "pwrseq.h"
#include "sdio_ops.h"

#define cls_dev_to_mmc_host(d)	container_of(d, struct mmc_host, class_dev)

static DEFINE_IDA(mmc_host_ida);

#ifdef CONFIG_PM_SLEEP
static int mmc_host_class_prepare(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	/*
	 * It's safe to access the bus_ops pointer, as both userspace and the
	 * workqueue for detecting cards are frozen at this point.
	 */
	if (!host->bus_ops)
		return 0;

	/* Validate conditions for system suspend. */
	if (host->bus_ops->pre_suspend)
		return host->bus_ops->pre_suspend(host);

	return 0;
}

static void mmc_host_class_complete(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	_mmc_detect_change(host, 0, false);
}

static const struct dev_pm_ops mmc_host_class_dev_pm_ops = {
	.prepare = mmc_host_class_prepare,
	.complete = mmc_host_class_complete,
};

#define MMC_HOST_CLASS_DEV_PM_OPS (&mmc_host_class_dev_pm_ops)
#else
#define MMC_HOST_CLASS_DEV_PM_OPS NULL
#endif

static void mmc_host_classdev_release(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	wakeup_source_unregister(host->ws);
	if (of_alias_get_id(host->parent->of_node, "mmc") < 0)
		ida_free(&mmc_host_ida, host->index);
	kfree(host);
}

static int mmc_host_classdev_shutdown(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	__mmc_stop_host(host);
	return 0;
}

static const struct class mmc_host_class = {
	.name		= "mmc_host",
	.dev_release	= mmc_host_classdev_release,
	.shutdown_pre	= mmc_host_classdev_shutdown,
	.pm		= MMC_HOST_CLASS_DEV_PM_OPS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int mmc_register_host_class(void)
{
	return class_register(&mmc_host_class);
}

void mmc_unregister_host_class(void)
{
	class_unregister(&mmc_host_class);
}

<<<<<<< HEAD
static DEFINE_IDR(mmc_host_idr);
static DEFINE_SPINLOCK(mmc_host_lock);

#ifdef CONFIG_MMC_CLKGATE
static ssize_t clkgate_delay_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	return snprintf(buf, PAGE_SIZE, "%lu\n", host->clkgate_delay);
}

static ssize_t clkgate_delay_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	unsigned long flags, value;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	spin_lock_irqsave(&host->clk_lock, flags);
	host->clkgate_delay = value;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	return count;
}

/*
 * Enabling clock gating will make the core call out to the host
 * once up and once down when it performs a request or card operation
 * intermingled in any fashion. The driver will see this through
 * set_ios() operations with ios.clock field set to 0 to gate (disable)
 * the block clock, and to the old frequency to enable it again.
 */
static void mmc_host_clk_gate_delayed(struct mmc_host *host)
{
	unsigned long tick_ns;
	unsigned long freq = host->ios.clock;
	unsigned long flags;

	if (!freq) {
		pr_debug("%s: frequency set to 0 in disable function, "
			 "this means the clock is already disabled.\n",
			 mmc_hostname(host));
		return;
	}
	/*
	 * New requests may have appeared while we were scheduling,
	 * then there is no reason to delay the check before
	 * clk_disable().
	 */
	spin_lock_irqsave(&host->clk_lock, flags);

	/*
	 * Delay n bus cycles (at least 8 from MMC spec) before attempting
	 * to disable the MCI block clock. The reference count may have
	 * gone up again after this delay due to rescheduling!
	 */
	if (!host->clk_requests) {
		spin_unlock_irqrestore(&host->clk_lock, flags);
		tick_ns = DIV_ROUND_UP(1000000000, freq);
		ndelay(host->clk_delay * tick_ns);
	} else {
		/* New users appeared while waiting for this work */
		spin_unlock_irqrestore(&host->clk_lock, flags);
		return;
	}
	mutex_lock(&host->clk_gate_mutex);
	spin_lock_irqsave(&host->clk_lock, flags);
	if (!host->clk_requests) {
		spin_unlock_irqrestore(&host->clk_lock, flags);
		/* This will set host->ios.clock to 0 */
		mmc_gate_clock(host);
		spin_lock_irqsave(&host->clk_lock, flags);
		pr_debug("%s: gated MCI clock\n", mmc_hostname(host));
	}
	spin_unlock_irqrestore(&host->clk_lock, flags);
	mutex_unlock(&host->clk_gate_mutex);
}

/*
 * Internal work. Work to disable the clock at some later point.
 */
static void mmc_host_clk_gate_work(struct work_struct *work)
{
	struct mmc_host *host = container_of(work, struct mmc_host,
					      clk_gate_work.work);

	mmc_host_clk_gate_delayed(host);
}

/**
 *	mmc_host_clk_hold - ungate hardware MCI clocks
 *	@host: host to ungate.
 *
 *	Makes sure the host ios.clock is restored to a non-zero value
 *	past this call.	Increase clock reference count and ungate clock
 *	if we're the first user.
 */
void mmc_host_clk_hold(struct mmc_host *host)
{
	unsigned long flags;

	/* cancel any clock gating work scheduled by mmc_host_clk_release() */
	cancel_delayed_work_sync(&host->clk_gate_work);
	mutex_lock(&host->clk_gate_mutex);
	spin_lock_irqsave(&host->clk_lock, flags);
	if (host->clk_gated) {
		spin_unlock_irqrestore(&host->clk_lock, flags);
		mmc_ungate_clock(host);

		/* Reset clock scaling stats as host is out of idle */
		mmc_reset_clk_scale_stats(host);
		spin_lock_irqsave(&host->clk_lock, flags);
		pr_debug("%s: ungated MCI clock\n", mmc_hostname(host));
	}
	host->clk_requests++;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	mutex_unlock(&host->clk_gate_mutex);
}

/**
 *	mmc_host_may_gate_card - check if this card may be gated
 *	@card: card to check.
 */
bool mmc_host_may_gate_card(struct mmc_card *card)
{
	/* If there is no card we may gate it */
	if (!card)
		return true;

	/*
	 * SDIO3.0 card allows the clock to be gated off so check if
	 * that is the case or not.
	 */
	if (mmc_card_sdio(card) && card->cccr.async_intr_sup)
		return true;

	/*
	 * Don't gate SDIO cards! These need to be clocked at all times
	 * since they may be independent systems generating interrupts
	 * and other events. The clock requests counter from the core will
	 * go down to zero since the core does not need it, but we will not
	 * gate the clock, because there is somebody out there that may still
	 * be using it.
	 */
	return !(card->quirks & MMC_QUIRK_BROKEN_CLK_GATING);
}

/**
 *	mmc_host_clk_release - gate off hardware MCI clocks
 *	@host: host to gate.
 *
 *	Calls the host driver with ios.clock set to zero as often as possible
 *	in order to gate off hardware MCI clocks. Decrease clock reference
 *	count and schedule disabling of clock.
 */
void mmc_host_clk_release(struct mmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->clk_lock, flags);
	host->clk_requests--;
	if (mmc_host_may_gate_card(host->card) &&
	    !host->clk_requests)
		queue_delayed_work(system_nrt_wq, &host->clk_gate_work,
				msecs_to_jiffies(host->clkgate_delay));
	spin_unlock_irqrestore(&host->clk_lock, flags);
}

/**
 *	mmc_host_clk_rate - get current clock frequency setting
 *	@host: host to get the clock frequency for.
 *
 *	Returns current clock frequency regardless of gating.
 */
unsigned int mmc_host_clk_rate(struct mmc_host *host)
{
	unsigned long freq;
	unsigned long flags;

	spin_lock_irqsave(&host->clk_lock, flags);
	if (host->clk_gated)
		freq = host->clk_old;
	else
		freq = host->ios.clock;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	return freq;
}

/**
 *	mmc_host_clk_init - set up clock gating code
 *	@host: host with potential clock to control
 */
static inline void mmc_host_clk_init(struct mmc_host *host)
{
	host->clk_requests = 0;
	/* Hold MCI clock for 8 cycles by default */
	host->clk_delay = 8;
	/*
	 * Default clock gating delay is 0ms to avoid wasting power.
	 * This value can be tuned by writing into sysfs entry.
	 */
	host->clkgate_delay = 0;
	host->clk_gated = false;
	INIT_DELAYED_WORK(&host->clk_gate_work, mmc_host_clk_gate_work);
	spin_lock_init(&host->clk_lock);
	mutex_init(&host->clk_gate_mutex);
}

/**
 *	mmc_host_clk_exit - shut down clock gating code
 *	@host: host with potential clock to control
 */
static inline void mmc_host_clk_exit(struct mmc_host *host)
{
	/*
	 * Wait for any outstanding gate and then make sure we're
	 * ungated before exiting.
	 */
	if (cancel_delayed_work_sync(&host->clk_gate_work))
		mmc_host_clk_gate_delayed(host);
	if (host->clk_gated)
		mmc_host_clk_hold(host);
	/* There should be only one user now */
	WARN_ON(host->clk_requests > 1);
}

static inline void mmc_host_clk_sysfs_init(struct mmc_host *host)
{
	host->clkgate_delay_attr.show = clkgate_delay_show;
	host->clkgate_delay_attr.store = clkgate_delay_store;
	sysfs_attr_init(&host->clkgate_delay_attr.attr);
	host->clkgate_delay_attr.attr.name = "clkgate_delay";
	host->clkgate_delay_attr.attr.mode = S_IRUGO | S_IWUSR;
	if (device_create_file(&host->class_dev, &host->clkgate_delay_attr))
		pr_err("%s: Failed to create clkgate_delay sysfs entry\n",
				mmc_hostname(host));
}
#else

static inline void mmc_host_clk_init(struct mmc_host *host)
{
}

static inline void mmc_host_clk_exit(struct mmc_host *host)
{
}

static inline void mmc_host_clk_sysfs_init(struct mmc_host *host)
{
}

#endif

=======
/**
 * mmc_retune_enable() - enter a transfer mode that requires retuning
 * @host: host which should retune now
 */
void mmc_retune_enable(struct mmc_host *host)
{
	host->can_retune = 1;
	if (host->retune_period)
		mod_timer(&host->retune_timer,
			  jiffies + host->retune_period * HZ);
}

/*
 * Pause re-tuning for a small set of operations.  The pause begins after the
 * next command.
 */
void mmc_retune_pause(struct mmc_host *host)
{
	if (!host->retune_paused) {
		host->retune_paused = 1;
		mmc_retune_hold(host);
	}
}
EXPORT_SYMBOL(mmc_retune_pause);

void mmc_retune_unpause(struct mmc_host *host)
{
	if (host->retune_paused) {
		host->retune_paused = 0;
		mmc_retune_release(host);
	}
}
EXPORT_SYMBOL(mmc_retune_unpause);

/**
 * mmc_retune_disable() - exit a transfer mode that requires retuning
 * @host: host which should not retune anymore
 *
 * It is not meant for temporarily preventing retuning!
 */
void mmc_retune_disable(struct mmc_host *host)
{
	mmc_retune_unpause(host);
	host->can_retune = 0;
	del_timer_sync(&host->retune_timer);
	mmc_retune_clear(host);
}

void mmc_retune_timer_stop(struct mmc_host *host)
{
	del_timer_sync(&host->retune_timer);
}
EXPORT_SYMBOL(mmc_retune_timer_stop);

void mmc_retune_hold(struct mmc_host *host)
{
	if (!host->hold_retune)
		host->retune_now = 1;
	host->hold_retune += 1;
}

void mmc_retune_release(struct mmc_host *host)
{
	if (host->hold_retune)
		host->hold_retune -= 1;
	else
		WARN_ON(1);
}
EXPORT_SYMBOL(mmc_retune_release);

int mmc_retune(struct mmc_host *host)
{
	bool return_to_hs400 = false;
	int err;

	if (host->retune_now)
		host->retune_now = 0;
	else
		return 0;

	if (!host->need_retune || host->doing_retune || !host->card)
		return 0;

	host->need_retune = 0;

	host->doing_retune = 1;

	if (host->ios.timing == MMC_TIMING_MMC_HS400) {
		err = mmc_hs400_to_hs200(host->card);
		if (err)
			goto out;

		return_to_hs400 = true;
	}

	err = mmc_execute_tuning(host->card);
	if (err)
		goto out;

	if (return_to_hs400)
		err = mmc_hs200_to_hs400(host->card);
out:
	host->doing_retune = 0;

	return err;
}

static void mmc_retune_timer(struct timer_list *t)
{
	struct mmc_host *host = from_timer(host, t, retune_timer);

	mmc_retune_needed(host);
}

static void mmc_of_parse_timing_phase(struct device *dev, const char *prop,
				      struct mmc_clk_phase *phase)
{
	int degrees[2] = {0};
	int rc;

	rc = device_property_read_u32_array(dev, prop, degrees, 2);
	phase->valid = !rc;
	if (phase->valid) {
		phase->in_deg = degrees[0];
		phase->out_deg = degrees[1];
	}
}

void
mmc_of_parse_clk_phase(struct device *dev, struct mmc_clk_phase_map *map)
{
	mmc_of_parse_timing_phase(dev, "clk-phase-legacy",
				  &map->phase[MMC_TIMING_LEGACY]);
	mmc_of_parse_timing_phase(dev, "clk-phase-mmc-hs",
				  &map->phase[MMC_TIMING_MMC_HS]);
	mmc_of_parse_timing_phase(dev, "clk-phase-sd-hs",
				  &map->phase[MMC_TIMING_SD_HS]);
	mmc_of_parse_timing_phase(dev, "clk-phase-uhs-sdr12",
				  &map->phase[MMC_TIMING_UHS_SDR12]);
	mmc_of_parse_timing_phase(dev, "clk-phase-uhs-sdr25",
				  &map->phase[MMC_TIMING_UHS_SDR25]);
	mmc_of_parse_timing_phase(dev, "clk-phase-uhs-sdr50",
				  &map->phase[MMC_TIMING_UHS_SDR50]);
	mmc_of_parse_timing_phase(dev, "clk-phase-uhs-sdr104",
				  &map->phase[MMC_TIMING_UHS_SDR104]);
	mmc_of_parse_timing_phase(dev, "clk-phase-uhs-ddr50",
				  &map->phase[MMC_TIMING_UHS_DDR50]);
	mmc_of_parse_timing_phase(dev, "clk-phase-mmc-ddr52",
				  &map->phase[MMC_TIMING_MMC_DDR52]);
	mmc_of_parse_timing_phase(dev, "clk-phase-mmc-hs200",
				  &map->phase[MMC_TIMING_MMC_HS200]);
	mmc_of_parse_timing_phase(dev, "clk-phase-mmc-hs400",
				  &map->phase[MMC_TIMING_MMC_HS400]);
}
EXPORT_SYMBOL(mmc_of_parse_clk_phase);

/**
 * mmc_of_parse() - parse host's device properties
 * @host: host whose properties should be parsed.
 *
 * To keep the rest of the MMC subsystem unaware of whether DT has been
 * used to instantiate and configure this host instance or not, we
 * parse the properties and set respective generic mmc-host flags and
 * parameters.
 */
int mmc_of_parse(struct mmc_host *host)
{
	struct device *dev = host->parent;
	u32 bus_width, drv_type, cd_debounce_delay_ms;
	int ret;

	if (!dev || !dev_fwnode(dev))
		return 0;

	/* "bus-width" is translated to MMC_CAP_*_BIT_DATA flags */
	if (device_property_read_u32(dev, "bus-width", &bus_width) < 0) {
		dev_dbg(host->parent,
			"\"bus-width\" property is missing, assuming 1 bit.\n");
		bus_width = 1;
	}

	switch (bus_width) {
	case 8:
		host->caps |= MMC_CAP_8_BIT_DATA;
		fallthrough;	/* Hosts capable of 8-bit can also do 4 bits */
	case 4:
		host->caps |= MMC_CAP_4_BIT_DATA;
		break;
	case 1:
		break;
	default:
		dev_err(host->parent,
			"Invalid \"bus-width\" value %u!\n", bus_width);
		return -EINVAL;
	}

	/* f_max is obtained from the optional "max-frequency" property */
	device_property_read_u32(dev, "max-frequency", &host->f_max);

	/*
	 * Configure CD and WP pins. They are both by default active low to
	 * match the SDHCI spec. If GPIOs are provided for CD and / or WP, the
	 * mmc-gpio helpers are used to attach, configure and use them. If
	 * polarity inversion is specified in DT, one of MMC_CAP2_CD_ACTIVE_HIGH
	 * and MMC_CAP2_RO_ACTIVE_HIGH capability-2 flags is set. If the
	 * "broken-cd" property is provided, the MMC_CAP_NEEDS_POLL capability
	 * is set. If the "non-removable" property is found, the
	 * MMC_CAP_NONREMOVABLE capability is set and no card-detection
	 * configuration is performed.
	 */

	/* Parse Card Detection */

	if (device_property_read_bool(dev, "non-removable")) {
		host->caps |= MMC_CAP_NONREMOVABLE;
	} else {
		if (device_property_read_bool(dev, "cd-inverted"))
			host->caps2 |= MMC_CAP2_CD_ACTIVE_HIGH;

		if (device_property_read_u32(dev, "cd-debounce-delay-ms",
					     &cd_debounce_delay_ms))
			cd_debounce_delay_ms = 200;

		if (device_property_read_bool(dev, "broken-cd"))
			host->caps |= MMC_CAP_NEEDS_POLL;

		ret = mmc_gpiod_request_cd(host, "cd", 0, false,
					   cd_debounce_delay_ms * 1000);
		if (!ret)
			dev_info(host->parent, "Got CD GPIO\n");
		else if (ret != -ENOENT && ret != -ENOSYS)
			return ret;
	}

	/* Parse Write Protection */

	if (device_property_read_bool(dev, "wp-inverted"))
		host->caps2 |= MMC_CAP2_RO_ACTIVE_HIGH;

	ret = mmc_gpiod_request_ro(host, "wp", 0, 0);
	if (!ret)
		dev_info(host->parent, "Got WP GPIO\n");
	else if (ret != -ENOENT && ret != -ENOSYS)
		return ret;

	if (device_property_read_bool(dev, "disable-wp"))
		host->caps2 |= MMC_CAP2_NO_WRITE_PROTECT;

	if (device_property_read_bool(dev, "cap-sd-highspeed"))
		host->caps |= MMC_CAP_SD_HIGHSPEED;
	if (device_property_read_bool(dev, "cap-mmc-highspeed"))
		host->caps |= MMC_CAP_MMC_HIGHSPEED;
	if (device_property_read_bool(dev, "sd-uhs-sdr12"))
		host->caps |= MMC_CAP_UHS_SDR12;
	if (device_property_read_bool(dev, "sd-uhs-sdr25"))
		host->caps |= MMC_CAP_UHS_SDR25;
	if (device_property_read_bool(dev, "sd-uhs-sdr50"))
		host->caps |= MMC_CAP_UHS_SDR50;
	if (device_property_read_bool(dev, "sd-uhs-sdr104"))
		host->caps |= MMC_CAP_UHS_SDR104;
	if (device_property_read_bool(dev, "sd-uhs-ddr50"))
		host->caps |= MMC_CAP_UHS_DDR50;
	if (device_property_read_bool(dev, "cap-power-off-card"))
		host->caps |= MMC_CAP_POWER_OFF_CARD;
	if (device_property_read_bool(dev, "cap-mmc-hw-reset"))
		host->caps |= MMC_CAP_HW_RESET;
	if (device_property_read_bool(dev, "cap-sdio-irq"))
		host->caps |= MMC_CAP_SDIO_IRQ;
	if (device_property_read_bool(dev, "full-pwr-cycle"))
		host->caps2 |= MMC_CAP2_FULL_PWR_CYCLE;
	if (device_property_read_bool(dev, "full-pwr-cycle-in-suspend"))
		host->caps2 |= MMC_CAP2_FULL_PWR_CYCLE_IN_SUSPEND;
	if (device_property_read_bool(dev, "keep-power-in-suspend"))
		host->pm_caps |= MMC_PM_KEEP_POWER;
	if (device_property_read_bool(dev, "wakeup-source") ||
	    device_property_read_bool(dev, "enable-sdio-wakeup")) /* legacy */
		host->pm_caps |= MMC_PM_WAKE_SDIO_IRQ;
	if (device_property_read_bool(dev, "mmc-ddr-3_3v"))
		host->caps |= MMC_CAP_3_3V_DDR;
	if (device_property_read_bool(dev, "mmc-ddr-1_8v"))
		host->caps |= MMC_CAP_1_8V_DDR;
	if (device_property_read_bool(dev, "mmc-ddr-1_2v"))
		host->caps |= MMC_CAP_1_2V_DDR;
	if (device_property_read_bool(dev, "mmc-hs200-1_8v"))
		host->caps2 |= MMC_CAP2_HS200_1_8V_SDR;
	if (device_property_read_bool(dev, "mmc-hs200-1_2v"))
		host->caps2 |= MMC_CAP2_HS200_1_2V_SDR;
	if (device_property_read_bool(dev, "mmc-hs400-1_8v"))
		host->caps2 |= MMC_CAP2_HS400_1_8V | MMC_CAP2_HS200_1_8V_SDR;
	if (device_property_read_bool(dev, "mmc-hs400-1_2v"))
		host->caps2 |= MMC_CAP2_HS400_1_2V | MMC_CAP2_HS200_1_2V_SDR;
	if (device_property_read_bool(dev, "mmc-hs400-enhanced-strobe"))
		host->caps2 |= MMC_CAP2_HS400_ES;
	if (device_property_read_bool(dev, "no-sdio"))
		host->caps2 |= MMC_CAP2_NO_SDIO;
	if (device_property_read_bool(dev, "no-sd"))
		host->caps2 |= MMC_CAP2_NO_SD;
	if (device_property_read_bool(dev, "no-mmc"))
		host->caps2 |= MMC_CAP2_NO_MMC;
	if (device_property_read_bool(dev, "no-mmc-hs400"))
		host->caps2 &= ~(MMC_CAP2_HS400_1_8V | MMC_CAP2_HS400_1_2V |
				 MMC_CAP2_HS400_ES);

	/* Must be after "non-removable" check */
	if (device_property_read_u32(dev, "fixed-emmc-driver-type", &drv_type) == 0) {
		if (host->caps & MMC_CAP_NONREMOVABLE)
			host->fixed_drv_type = drv_type;
		else
			dev_err(host->parent,
				"can't use fixed driver type, media is removable\n");
	}

	host->dsr_req = !device_property_read_u32(dev, "dsr", &host->dsr);
	if (host->dsr_req && (host->dsr & ~0xffff)) {
		dev_err(host->parent,
			"device tree specified broken value for DSR: 0x%x, ignoring\n",
			host->dsr);
		host->dsr_req = 0;
	}

	device_property_read_u32(dev, "post-power-on-delay-ms",
				 &host->ios.power_delay_ms);

	return mmc_pwrseq_alloc(host);
}

EXPORT_SYMBOL(mmc_of_parse);

/**
 * mmc_of_parse_voltage - return mask of supported voltages
 * @host: host whose properties should be parsed.
 * @mask: mask of voltages available for MMC/SD/SDIO
 *
 * Parse the "voltage-ranges" property, returning zero if it is not
 * found, negative errno if the voltage-range specification is invalid,
 * or one if the voltage-range is specified and successfully parsed.
 */
int mmc_of_parse_voltage(struct mmc_host *host, u32 *mask)
{
	const char *prop = "voltage-ranges";
	struct device *dev = host->parent;
	u32 *voltage_ranges;
	int num_ranges, i;
	int ret;

	if (!device_property_present(dev, prop)) {
		dev_dbg(dev, "%s unspecified\n", prop);
		return 0;
	}

	ret = device_property_count_u32(dev, prop);
	if (ret < 0)
		return ret;

	num_ranges = ret / 2;
	if (!num_ranges) {
		dev_err(dev, "%s empty\n", prop);
		return -EINVAL;
	}

	voltage_ranges = kcalloc(2 * num_ranges, sizeof(*voltage_ranges), GFP_KERNEL);
	if (!voltage_ranges)
		return -ENOMEM;

	ret = device_property_read_u32_array(dev, prop, voltage_ranges, 2 * num_ranges);
	if (ret) {
		kfree(voltage_ranges);
		return ret;
	}

	for (i = 0; i < num_ranges; i++) {
		const int j = i * 2;
		u32 ocr_mask;

		ocr_mask = mmc_vddrange_to_ocrmask(voltage_ranges[j + 0],
						   voltage_ranges[j + 1]);
		if (!ocr_mask) {
			dev_err(dev, "range #%d in %s is invalid\n", i, prop);
			kfree(voltage_ranges);
			return -EINVAL;
		}
		*mask |= ocr_mask;
	}

	kfree(voltage_ranges);

	return 1;
}
EXPORT_SYMBOL(mmc_of_parse_voltage);

/**
 * mmc_first_nonreserved_index() - get the first index that is not reserved
 */
static int mmc_first_nonreserved_index(void)
{
	int max;

	max = of_alias_get_highest_id("mmc");
	if (max < 0)
		return 0;

	return max + 1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 *	mmc_alloc_host - initialise the per-host structure.
 *	@extra: sizeof private data structure
 *	@dev: pointer to host device model structure
 *
 *	Initialise the per-host structure.
 */
struct mmc_host *mmc_alloc_host(int extra, struct device *dev)
{
<<<<<<< HEAD
	int err;
	struct mmc_host *host;

	if (!idr_pre_get(&mmc_host_idr, GFP_KERNEL))
		return NULL;
=======
	int index;
	struct mmc_host *host;
	int alias_id, min_idx, max_idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	host = kzalloc(sizeof(struct mmc_host) + extra, GFP_KERNEL);
	if (!host)
		return NULL;

<<<<<<< HEAD
	spin_lock(&mmc_host_lock);
	err = idr_get_new(&mmc_host_idr, host, &host->index);
	spin_unlock(&mmc_host_lock);
	if (err)
		goto free;

	dev_set_name(&host->class_dev, "mmc%d", host->index);
=======
	/* scanning will be enabled when we're ready */
	host->rescan_disable = 1;

	alias_id = of_alias_get_id(dev->of_node, "mmc");
	if (alias_id >= 0) {
		index = alias_id;
	} else {
		min_idx = mmc_first_nonreserved_index();
		max_idx = 0;

		index = ida_alloc_range(&mmc_host_ida, min_idx, max_idx - 1,
					GFP_KERNEL);
		if (index < 0) {
			kfree(host);
			return NULL;
		}
	}

	host->index = index;

	dev_set_name(&host->class_dev, "mmc%d", host->index);
	host->ws = wakeup_source_register(NULL, dev_name(&host->class_dev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	host->parent = dev;
	host->class_dev.parent = dev;
	host->class_dev.class = &mmc_host_class;
	device_initialize(&host->class_dev);
<<<<<<< HEAD

	mmc_host_clk_init(host);

	spin_lock_init(&host->lock);
	init_waitqueue_head(&host->wq);
	host->wlock_name = kasprintf(GFP_KERNEL,
			"%s_detect", mmc_hostname(host));
	wake_lock_init(&host->detect_wake_lock, WAKE_LOCK_SUSPEND,
			host->wlock_name);
	INIT_DELAYED_WORK(&host->detect, mmc_rescan);
#ifdef CONFIG_PM
	host->pm_notify.notifier_call = mmc_pm_notify;
#endif
=======
	device_enable_async_suspend(&host->class_dev);

	if (mmc_gpio_alloc(host)) {
		put_device(&host->class_dev);
		return NULL;
	}

	spin_lock_init(&host->lock);
	init_waitqueue_head(&host->wq);
	INIT_DELAYED_WORK(&host->detect, mmc_rescan);
	INIT_WORK(&host->sdio_irq_work, sdio_irq_work);
	timer_setup(&host->retune_timer, mmc_retune_timer, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * By default, hosts do not support SGIO or large requests.
	 * They have to set these according to their abilities.
	 */
	host->max_segs = 1;
<<<<<<< HEAD
	host->max_seg_size = PAGE_CACHE_SIZE;

	host->max_req_size = PAGE_CACHE_SIZE;
	host->max_blk_size = 512;
	host->max_blk_count = PAGE_CACHE_SIZE / 512;

	return host;

free:
	kfree(host);
	return NULL;
=======
	host->max_seg_size = PAGE_SIZE;

	host->max_req_size = PAGE_SIZE;
	host->max_blk_size = 512;
	host->max_blk_count = PAGE_SIZE / 512;

	host->fixed_drv_type = -EINVAL;
	host->ios.power_delay_ms = 10;
	host->ios.power_mode = MMC_POWER_UNDEFINED;

	return host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(mmc_alloc_host);

<<<<<<< HEAD
static ssize_t show_enable(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	if (!host)
		return -EINVAL;

	return snprintf(buf, PAGE_SIZE, "%d\n", mmc_can_scale_clk(host));
}

static ssize_t store_enable(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	unsigned long value, freq;
	int retval = -EINVAL;

	if (!host)
		goto out;

	mmc_claim_host(host);
	if (!host->card || kstrtoul(buf, 0, &value))
		goto err;

	if (value && !mmc_can_scale_clk(host)) {
		host->caps2 |= MMC_CAP2_CLK_SCALE;
		mmc_init_clk_scaling(host);

		if (!mmc_can_scale_clk(host)) {
			host->caps2 &= ~MMC_CAP2_CLK_SCALE;
			goto err;
		}
	} else if (!value && mmc_can_scale_clk(host)) {
		host->caps2 &= ~MMC_CAP2_CLK_SCALE;
		mmc_disable_clk_scaling(host);

		/* Set to max. frequency, since we are disabling */
		if (host->bus_ops && host->bus_ops->change_bus_speed) {
			freq = mmc_get_max_frequency(host);
			if (host->bus_ops->change_bus_speed(host, &freq))
				goto err;
		}
		if (host->ops->notify_load &&
				host->ops->notify_load(host, MMC_LOAD_HIGH))
			goto err;
		host->clk_scaling.state = MMC_LOAD_HIGH;
		host->clk_scaling.initialized = false;
	}
	retval = count;
err:
	mmc_release_host(host);
out:
	return retval;
}

static ssize_t show_up_threshold(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	if (!host)
		return -EINVAL;

	return snprintf(buf, PAGE_SIZE, "%d\n", host->clk_scaling.up_threshold);
}

#define MAX_PERCENTAGE	100
static ssize_t store_up_threshold(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	unsigned long value;

	if (!host || kstrtoul(buf, 0, &value) || (value > MAX_PERCENTAGE))
		return -EINVAL;

	host->clk_scaling.up_threshold = value;

	pr_debug("%s: clkscale_up_thresh set to %lu\n",
			mmc_hostname(host), value);
	return count;
}

static ssize_t show_down_threshold(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	if (!host)
		return -EINVAL;

	return snprintf(buf, PAGE_SIZE, "%d\n",
			host->clk_scaling.down_threshold);
}

static ssize_t store_down_threshold(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	unsigned long value;

	if (!host || kstrtoul(buf, 0, &value) || (value > MAX_PERCENTAGE))
		return -EINVAL;

	host->clk_scaling.down_threshold = value;

	pr_debug("%s: clkscale_down_thresh set to %lu\n",
			mmc_hostname(host), value);
	return count;
}

static ssize_t show_polling(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);

	if (!host)
		return -EINVAL;

	return snprintf(buf, PAGE_SIZE, "%lu milliseconds\n",
			host->clk_scaling.polling_delay_ms);
}

static ssize_t store_polling(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	unsigned long value;

	if (!host || kstrtoul(buf, 0, &value))
		return -EINVAL;

	host->clk_scaling.polling_delay_ms = value;

	pr_debug("%s: clkscale_polling_delay_ms set to %lu\n",
			mmc_hostname(host), value);
	return count;
}

DEVICE_ATTR(enable, S_IRUGO | S_IWUSR,
		show_enable, store_enable);
DEVICE_ATTR(polling_interval, S_IRUGO | S_IWUSR,
		show_polling, store_polling);
DEVICE_ATTR(up_threshold, S_IRUGO | S_IWUSR,
		show_up_threshold, store_up_threshold);
DEVICE_ATTR(down_threshold, S_IRUGO | S_IWUSR,
		show_down_threshold, store_down_threshold);

static struct attribute *clk_scaling_attrs[] = {
	&dev_attr_enable.attr,
	&dev_attr_up_threshold.attr,
	&dev_attr_down_threshold.attr,
	&dev_attr_polling_interval.attr,
	NULL,
};

static struct attribute_group clk_scaling_attr_grp = {
	.name = "clk_scaling",
	.attrs = clk_scaling_attrs,
};

#ifdef CONFIG_MMC_PERF_PROFILING
static ssize_t
show_perf(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int64_t rtime_drv, wtime_drv;
	unsigned long rbytes_drv, wbytes_drv;

	spin_lock(&host->lock);

	rbytes_drv = host->perf.rbytes_drv;
	wbytes_drv = host->perf.wbytes_drv;

	rtime_drv = ktime_to_us(host->perf.rtime_drv);
	wtime_drv = ktime_to_us(host->perf.wtime_drv);

	spin_unlock(&host->lock);

	return snprintf(buf, PAGE_SIZE, "Write performance at driver Level:"
					"%lu bytes in %lld microseconds\n"
					"Read performance at driver Level:"
					"%lu bytes in %lld microseconds\n",
					wbytes_drv, wtime_drv,
					rbytes_drv, rtime_drv);
}

static ssize_t
set_perf(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	int64_t value;

	sscanf(buf, "%lld", &value);
	spin_lock(&host->lock);
	if (!value) {
		memset(&host->perf, 0, sizeof(host->perf));
		host->perf_enable = false;
	} else {
		host->perf_enable = true;
	}
	spin_unlock(&host->lock);

	return count;
}

static DEVICE_ATTR(perf, S_IRUGO | S_IWUSR,
		show_perf, set_perf);

#endif

static struct attribute *dev_attrs[] = {
#ifdef CONFIG_MMC_PERF_PROFILING
	&dev_attr_perf.attr,
#endif
	NULL,
};
static struct attribute_group dev_attr_grp = {
	.attrs = dev_attrs,
};

=======
static void devm_mmc_host_release(struct device *dev, void *res)
{
	mmc_free_host(*(struct mmc_host **)res);
}

struct mmc_host *devm_mmc_alloc_host(struct device *dev, int extra)
{
	struct mmc_host **dr, *host;

	dr = devres_alloc(devm_mmc_host_release, sizeof(*dr), GFP_KERNEL);
	if (!dr)
		return NULL;

	host = mmc_alloc_host(extra, dev);
	if (!host) {
		devres_free(dr);
		return NULL;
	}

	*dr = host;
	devres_add(dev, dr);

	return host;
}
EXPORT_SYMBOL(devm_mmc_alloc_host);

static int mmc_validate_host_caps(struct mmc_host *host)
{
	struct device *dev = host->parent;
	u32 caps = host->caps, caps2 = host->caps2;

	if (caps & MMC_CAP_SDIO_IRQ && !host->ops->enable_sdio_irq) {
		dev_warn(dev, "missing ->enable_sdio_irq() ops\n");
		return -EINVAL;
	}

	if (caps2 & (MMC_CAP2_HS400_ES | MMC_CAP2_HS400) &&
	    !(caps & MMC_CAP_8_BIT_DATA) && !(caps2 & MMC_CAP2_NO_MMC)) {
		dev_warn(dev, "drop HS400 support since no 8-bit bus\n");
		host->caps2 = caps2 & ~MMC_CAP2_HS400_ES & ~MMC_CAP2_HS400;
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 *	mmc_add_host - initialise host hardware
 *	@host: mmc host
 *
 *	Register the host with the driver model. The host must be
 *	prepared to start servicing requests before this function
 *	completes.
 */
int mmc_add_host(struct mmc_host *host)
{
	int err;

<<<<<<< HEAD
	WARN_ON((host->caps & MMC_CAP_SDIO_IRQ) &&
		!host->ops->enable_sdio_irq);

	err = pm_runtime_set_active(&host->class_dev);
	if (err)
		pr_err("%s: %s: failed setting runtime active: err: %d\n",
		       mmc_hostname(host), __func__, err);
	else if (mmc_use_core_runtime_pm(host))
		pm_runtime_enable(&host->class_dev);
=======
	err = mmc_validate_host_caps(host);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = device_add(&host->class_dev);
	if (err)
		return err;

<<<<<<< HEAD
	device_enable_async_suspend(&host->class_dev);
	led_trigger_register_simple(dev_name(&host->class_dev), &host->led);

#ifdef CONFIG_DEBUG_FS
	mmc_add_host_debugfs(host);
#endif
	mmc_host_clk_sysfs_init(host);

	host->clk_scaling.up_threshold = 35;
	host->clk_scaling.down_threshold = 5;
	host->clk_scaling.polling_delay_ms = 100;

	err = sysfs_create_group(&host->class_dev.kobj, &clk_scaling_attr_grp);
	if (err)
		pr_err("%s: failed to create clk scale sysfs group with err %d\n",
				__func__, err);

	err = sysfs_create_group(&host->class_dev.kobj, &dev_attr_grp);
	if (err)
		pr_err("%s: failed to create sysfs group with err %d\n",
							 __func__, err);

	mmc_start_host(host);
	if (!(host->pm_flags & MMC_PM_IGNORE_PM_NOTIFY))
		register_pm_notifier(&host->pm_notify);

=======
	led_trigger_register_simple(dev_name(&host->class_dev), &host->led);

	mmc_add_host_debugfs(host);

	mmc_start_host(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

EXPORT_SYMBOL(mmc_add_host);

/**
 *	mmc_remove_host - remove host hardware
 *	@host: mmc host
 *
 *	Unregister and remove all cards associated with this host,
 *	and power down the MMC bus. No new requests will be issued
 *	after this function has returned.
 */
void mmc_remove_host(struct mmc_host *host)
{
<<<<<<< HEAD
	if (!(host->pm_flags & MMC_PM_IGNORE_PM_NOTIFY))
		unregister_pm_notifier(&host->pm_notify);

	mmc_stop_host(host);

#ifdef CONFIG_DEBUG_FS
	mmc_remove_host_debugfs(host);
#endif
	sysfs_remove_group(&host->parent->kobj, &dev_attr_grp);
	sysfs_remove_group(&host->class_dev.kobj, &clk_scaling_attr_grp);
=======
	mmc_stop_host(host);

	mmc_remove_host_debugfs(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	device_del(&host->class_dev);

	led_trigger_unregister_simple(host->led);
<<<<<<< HEAD

	mmc_host_clk_exit(host);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL(mmc_remove_host);

/**
 *	mmc_free_host - free the host structure
 *	@host: mmc host
 *
 *	Free the host once all references to it have been dropped.
 */
void mmc_free_host(struct mmc_host *host)
{
<<<<<<< HEAD
	spin_lock(&mmc_host_lock);
	idr_remove(&mmc_host_idr, host->index);
	spin_unlock(&mmc_host_lock);
	wake_lock_destroy(&host->detect_wake_lock);

=======
	cancel_delayed_work_sync(&host->detect);
	mmc_pwrseq_free(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_device(&host->class_dev);
}

EXPORT_SYMBOL(mmc_free_host);
