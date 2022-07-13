<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/mmc/core/bus.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007 Pierre Ossman
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  MMC card bus driver model
 */

#include <linux/export.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/stat.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>
=======
#include <linux/of.h>
#include <linux/pm_runtime.h>
#include <linux/sysfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

#include "core.h"
<<<<<<< HEAD
=======
#include "card.h"
#include "host.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "sdio_cis.h"
#include "bus.h"

#define to_mmc_driver(d)	container_of(d, struct mmc_driver, drv)
<<<<<<< HEAD
#define RUNTIME_SUSPEND_DELAY_MS 10000

static ssize_t mmc_type_show(struct device *dev,
=======

static ssize_t type_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct device_attribute *attr, char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	switch (card->type) {
	case MMC_TYPE_MMC:
<<<<<<< HEAD
		return sprintf(buf, "MMC\n");
	case MMC_TYPE_SD:
		return sprintf(buf, "SD\n");
	case MMC_TYPE_SDIO:
		return sprintf(buf, "SDIO\n");
	case MMC_TYPE_SD_COMBO:
		return sprintf(buf, "SDcombo\n");
=======
		return sysfs_emit(buf, "MMC\n");
	case MMC_TYPE_SD:
		return sysfs_emit(buf, "SD\n");
	case MMC_TYPE_SDIO:
		return sysfs_emit(buf, "SDIO\n");
	case MMC_TYPE_SD_COMBO:
		return sysfs_emit(buf, "SDcombo\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EFAULT;
	}
}
<<<<<<< HEAD

static struct device_attribute mmc_dev_attrs[] = {
	__ATTR(type, S_IRUGO, mmc_type_show, NULL),
	__ATTR_NULL,
};

/*
 * This currently matches any MMC driver to any MMC card - drivers
 * themselves make the decision whether to drive this card in their
 * probe method.
 */
static int mmc_bus_match(struct device *dev, struct device_driver *drv)
{
	return 1;
}

static int
mmc_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	const char *type;
=======
static DEVICE_ATTR_RO(type);

static struct attribute *mmc_dev_attrs[] = {
	&dev_attr_type.attr,
	NULL,
};
ATTRIBUTE_GROUPS(mmc_dev);

static int
mmc_bus_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	const struct mmc_card *card = mmc_dev_to_card(dev);
	const char *type;
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval = 0;

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SDcombo";
		break;
	default:
		type = NULL;
	}

	if (type) {
		retval = add_uevent_var(env, "MMC_TYPE=%s", type);
		if (retval)
			return retval;
	}

<<<<<<< HEAD
=======
	if (mmc_card_sdio(card) || mmc_card_sd_combo(card)) {
		retval = add_uevent_var(env, "SDIO_ID=%04X:%04X",
					card->cis.vendor, card->cis.device);
		if (retval)
			return retval;

		retval = add_uevent_var(env, "SDIO_REVISION=%u.%u",
					card->major_rev, card->minor_rev);
		if (retval)
			return retval;

		for (i = 0; i < card->num_info; i++) {
			retval = add_uevent_var(env, "SDIO_INFO%u=%s", i+1, card->info[i]);
			if (retval)
				return retval;
		}
	}

	/*
	 * SDIO (non-combo) cards are not handled by mmc_block driver and do not
	 * have accessible CID register which used by mmc_card_name() function.
	 */
	if (mmc_card_sdio(card))
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	retval = add_uevent_var(env, "MMC_NAME=%s", mmc_card_name(card));
	if (retval)
		return retval;

	/*
	 * Request the mmc_block device.  Note: that this is a direct request
	 * for the module it carries no information as to what is inserted.
	 */
	retval = add_uevent_var(env, "MODALIAS=mmc:block");

	return retval;
}

static int mmc_bus_probe(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	return drv->probe(card);
}

<<<<<<< HEAD
static int mmc_bus_remove(struct device *dev)
=======
static void mmc_bus_remove(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	drv->remove(card);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mmc_bus_shutdown(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
<<<<<<< HEAD

	if (!drv) {
		pr_debug("%s: %s: drv is NULL\n", dev_name(dev), __func__);
		return;
	}

	if (!card) {
		pr_debug("%s: %s: card is NULL\n", dev_name(dev), __func__);
		return;
	}

	if (drv->shutdown)
		drv->shutdown(card);
=======
	struct mmc_host *host = card->host;
	int ret;

	if (dev->driver && drv->shutdown)
		drv->shutdown(card);

	if (host->bus_ops->shutdown) {
		ret = host->bus_ops->shutdown(host);
		if (ret)
			pr_warn("%s: error %d during shutdown\n",
				mmc_hostname(host), ret);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PM_SLEEP
static int mmc_bus_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->suspend)
		ret = drv->suspend(card);
=======
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	ret = pm_generic_suspend(dev);
	if (ret)
		return ret;

	ret = host->bus_ops->suspend(host);
	if (ret)
		pm_generic_resume(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int mmc_bus_resume(struct device *dev)
{
<<<<<<< HEAD
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->resume)
		ret = drv->resume(card);
=======
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	ret = host->bus_ops->resume(host);
	if (ret)
		pr_warn("%s: error %d during resume (card was removed?)\n",
			mmc_hostname(host), ret);

	ret = pm_generic_resume(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_PM_RUNTIME

static int mmc_runtime_suspend(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	if (mmc_use_core_runtime_pm(card->host)) {
		/*
		 * If idle time bkops is running on the card, let's not get
		 * into suspend.
		 */
		if (mmc_card_doing_bkops(card) && mmc_card_is_prog_state(card))
			return -EBUSY;
		else
			return 0;
	} else {
		return mmc_power_save_host(card->host);
	}
=======
#ifdef CONFIG_PM
static int mmc_runtime_suspend(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;

	return host->bus_ops->runtime_suspend(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mmc_runtime_resume(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
<<<<<<< HEAD

	if (mmc_use_core_runtime_pm(card->host))
		return 0;
	else
		return mmc_power_restore_host(card->host);
}

static int mmc_runtime_idle(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret = 0;

	if (mmc_use_core_runtime_pm(card->host)) {
		ret = pm_schedule_suspend(dev, card->idle_timeout);
		if ((ret < 0) && (dev->power.runtime_error ||
				  dev->power.disable_depth > 0)) {
			pr_err("%s: %s: %s: pm_schedule_suspend failed: err: %d\n",
			       mmc_hostname(host), __func__, dev_name(dev),
			       ret);
			return ret;
		}
	}

	return ret;
}

#endif /* !CONFIG_PM_RUNTIME */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume,
			mmc_runtime_idle)
	SET_SYSTEM_SLEEP_PM_OPS(mmc_bus_suspend, mmc_bus_resume)
};

static ssize_t show_rpm_delay(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	if (!card) {
		pr_err("%s: %s: card is NULL\n", dev_name(dev), __func__);
		return -EINVAL;
	}

	return snprintf(buf, PAGE_SIZE, "%u\n", card->idle_timeout);
}

static ssize_t store_rpm_delay(struct device *dev, struct device_attribute
			       *attr, const char *buf, size_t count)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	unsigned int delay;

	if (!card) {
		pr_err("%s: %s: card is NULL\n", dev_name(dev), __func__);
		return -EINVAL;
	}

	if (!kstrtou32(buf, 0, &delay)) {
		if (delay < 2000) {
			pr_err("%s: %s: less than 2 sec delay is unsupported\n",
			       mmc_hostname(card->host), __func__);
			return -EINVAL;
		}
		card->idle_timeout = delay;
	}

	return count;
}

static struct bus_type mmc_bus_type = {
	.name		= "mmc",
	.dev_attrs	= mmc_dev_attrs,
	.match		= mmc_bus_match,
	.uevent		= mmc_bus_uevent,
	.probe		= mmc_bus_probe,
	.remove		= mmc_bus_remove,
	.shutdown        = mmc_bus_shutdown,
=======
	struct mmc_host *host = card->host;

	return host->bus_ops->runtime_resume(host);
}
#endif /* !CONFIG_PM */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume, NULL)
	SET_SYSTEM_SLEEP_PM_OPS(mmc_bus_suspend, mmc_bus_resume)
};

static const struct bus_type mmc_bus_type = {
	.name		= "mmc",
	.dev_groups	= mmc_dev_groups,
	.uevent		= mmc_bus_uevent,
	.probe		= mmc_bus_probe,
	.remove		= mmc_bus_remove,
	.shutdown	= mmc_bus_shutdown,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.pm		= &mmc_bus_pm_ops,
};

int mmc_register_bus(void)
{
	return bus_register(&mmc_bus_type);
}

void mmc_unregister_bus(void)
{
	bus_unregister(&mmc_bus_type);
}

/**
 *	mmc_register_driver - register a media driver
 *	@drv: MMC media driver
 */
int mmc_register_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	return driver_register(&drv->drv);
}

EXPORT_SYMBOL(mmc_register_driver);

/**
 *	mmc_unregister_driver - unregister a media driver
 *	@drv: MMC media driver
 */
void mmc_unregister_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	driver_unregister(&drv->drv);
}

EXPORT_SYMBOL(mmc_unregister_driver);

static void mmc_release_card(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	sdio_free_common_cis(card);

<<<<<<< HEAD
	if (card->info)
		kfree(card->info);
=======
	kfree(card->info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(card);
}

/*
 * Allocate and initialise a new MMC card structure.
 */
<<<<<<< HEAD
struct mmc_card *mmc_alloc_card(struct mmc_host *host, struct device_type *type)
=======
struct mmc_card *mmc_alloc_card(struct mmc_host *host, const struct device_type *type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mmc_card *card;

	card = kzalloc(sizeof(struct mmc_card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->host = host;

	device_initialize(&card->dev);

	card->dev.parent = mmc_classdev(host);
	card->dev.bus = &mmc_bus_type;
	card->dev.release = mmc_release_card;
	card->dev.type = type;

<<<<<<< HEAD
	spin_lock_init(&card->bkops_info.bkops_stats.lock);
	spin_lock_init(&card->wr_pack_stats.lock);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return card;
}

/*
 * Register a new MMC card with the driver model.
 */
int mmc_add_card(struct mmc_card *card)
{
	int ret;
	const char *type;
	const char *uhs_bus_speed_mode = "";
	static const char *const uhs_speeds[] = {
		[UHS_SDR12_BUS_SPEED] = "SDR12 ",
		[UHS_SDR25_BUS_SPEED] = "SDR25 ",
		[UHS_SDR50_BUS_SPEED] = "SDR50 ",
		[UHS_SDR104_BUS_SPEED] = "SDR104 ",
		[UHS_DDR50_BUS_SPEED] = "DDR50 ",
	};


	dev_set_name(&card->dev, "%s:%04x", mmc_hostname(card->host), card->rca);
<<<<<<< HEAD
=======
	dev_set_removable(&card->dev,
			  mmc_card_is_removable(card->host) ?
			  DEVICE_REMOVABLE : DEVICE_FIXED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		if (mmc_card_blockaddr(card)) {
			if (mmc_card_ext_capacity(card))
				type = "SDXC";
			else
				type = "SDHC";
		}
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SD-combo";
		if (mmc_card_blockaddr(card))
			type = "SDHC-combo";
		break;
	default:
		type = "?";
		break;
	}

<<<<<<< HEAD
	if (mmc_sd_card_uhs(card) &&
=======
	if (mmc_card_uhs(card) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		(card->sd_bus_speed < ARRAY_SIZE(uhs_speeds)))
		uhs_bus_speed_mode = uhs_speeds[card->sd_bus_speed];

	if (mmc_host_is_spi(card->host)) {
		pr_info("%s: new %s%s%s card on SPI\n",
			mmc_hostname(card->host),
<<<<<<< HEAD
			mmc_card_highspeed(card) ? "high speed " : "",
			mmc_card_ddr_mode(card) ? "DDR " : "",
=======
			mmc_card_hs(card) ? "high speed " : "",
			mmc_card_ddr52(card) ? "DDR " : "",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			type);
	} else {
		pr_info("%s: new %s%s%s%s%s%s card at address %04x\n",
			mmc_hostname(card->host),
			mmc_card_uhs(card) ? "ultra high speed " :
<<<<<<< HEAD
			(mmc_card_highspeed(card) ? "high speed " : ""),
			(mmc_card_hs400(card) ? "HS400 " : ""),
			(mmc_card_hs200(card) ? "HS200 " : ""),
			mmc_card_ddr_mode(card) ? "DDR " : "",
			uhs_bus_speed_mode, type, card->rca);
	}

#ifdef CONFIG_MACH_LGE
	/* LGE_CHANGE
	 * Adding Print for more information.
	 * 2014-01-16, B2-BSP-FS@lge.com
	 */
	printk(KERN_INFO "[LGE][MMC][%-18s( )] mmc_hostname:%s, type:%s\n", __func__, mmc_hostname(card->host), type);
#endif

#ifdef CONFIG_DEBUG_FS
	mmc_add_card_debugfs(card);
#endif
	mmc_init_context_info(card->host);

	ret = pm_runtime_set_active(&card->dev);
	if (ret)
		pr_err("%s: %s: failed setting runtime active: ret: %d\n",
		       mmc_hostname(card->host), __func__, ret);
	else if (!mmc_card_sdio(card) && mmc_use_core_runtime_pm(card->host))
		pm_runtime_enable(&card->dev);

	if (mmc_card_sdio(card)) {
		ret = device_init_wakeup(&card->dev, true);
		if (ret)
			pr_err("%s: %s: failed to init wakeup: %d\n",
			       mmc_hostname(card->host), __func__, ret);
	}
	ret = device_add(&card->dev);

#ifdef CONFIG_MACH_LGE
	/* LGE_CHANGE
	 * Adding Print for more information.
	 * 2014-01-16, B2-BSP-FS@lge.com
	 */
	if (ret) {
		printk(KERN_INFO "[LGE][MMC][%-18s( )] device_add & uevent posting fail!, ret:%d \n", __func__, ret);
		return ret;
	} else {
		printk(KERN_INFO "[LGE][MMC][%-18s( )] device_add & uevent posting complete!\n", __func__);
	}
#else
	if (ret)
		return ret;
#endif

	device_enable_async_suspend(&card->dev);
	if (mmc_use_core_runtime_pm(card->host) && !mmc_card_sdio(card)) {
		card->rpm_attrib.show = show_rpm_delay;
		card->rpm_attrib.store = store_rpm_delay;
		sysfs_attr_init(&card->rpm_attrib.attr);
		card->rpm_attrib.attr.name = "runtime_pm_timeout";
		card->rpm_attrib.attr.mode = S_IRUGO | S_IWUSR;

		ret = device_create_file(&card->dev, &card->rpm_attrib);
		if (ret)
			pr_err("%s: %s: creating runtime pm sysfs entry: failed: %d\n",
			       mmc_hostname(card->host), __func__, ret);
		/* Default timeout is 10 seconds */
		card->idle_timeout = RUNTIME_SUSPEND_DELAY_MS;
	}
=======
			(mmc_card_hs(card) ? "high speed " : ""),
			mmc_card_hs400(card) ? "HS400 " :
			(mmc_card_hs200(card) ? "HS200 " : ""),
			mmc_card_hs400es(card) ? "Enhanced strobe " : "",
			mmc_card_ddr52(card) ? "DDR " : "",
			uhs_bus_speed_mode, type, card->rca);
	}

	mmc_add_card_debugfs(card);
	card->dev.of_node = mmc_of_find_child_device(card->host, 0);

	device_enable_async_suspend(&card->dev);

	ret = device_add(&card->dev);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mmc_card_set_present(card);

	return 0;
}

/*
 * Unregister a new MMC card with the driver model, and
 * (eventually) free it.
 */
void mmc_remove_card(struct mmc_card *card)
{
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
	mmc_remove_card_debugfs(card);
#endif
=======
	struct mmc_host *host = card->host;

	mmc_remove_card_debugfs(card);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mmc_card_present(card)) {
		if (mmc_host_is_spi(card->host)) {
			pr_info("%s: SPI card removed\n",
				mmc_hostname(card->host));
		} else {
			pr_info("%s: card %04x removed\n",
				mmc_hostname(card->host), card->rca);
		}
		device_del(&card->dev);
<<<<<<< HEAD
	}

	kfree(card->wr_pack_stats.packing_events);
	kfree(card->cached_ext_csd);

	put_device(&card->dev);
}

=======
		of_node_put(card->dev.of_node);
	}

	if (host->cqe_enabled) {
		host->cqe_ops->cqe_disable(host);
		host->cqe_enabled = false;
	}

	put_device(&card->dev);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
