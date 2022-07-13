<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/char/hw_random/timeriomem-rng.c
 *
 * Copyright (C) 2009 Alexander Clouter <alex@digriz.org.uk>
 *
 * Derived from drivers/char/hw_random/omap-rng.c
 *   Copyright 2005 (c) MontaVista Software, Inc.
 *   Author: Deepak Saxena <dsaxena@plexity.net>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Overview:
 *   This driver is useful for platforms that have an IO range that provides
 *   periodic random data from a single IO memory address.  All the platform
 *   has to do is provide the address and 'wait time' that new data becomes
 *   available.
 *
 * TODO: add support for reading sizes other than 32bits and masking
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/hw_random.h>
#include <linux/io.h>
#include <linux/timeriomem-rng.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/completion.h>

static struct timeriomem_rng_data *timeriomem_rng_data;

static void timeriomem_rng_trigger(unsigned long);
static DEFINE_TIMER(timeriomem_rng_timer, timeriomem_rng_trigger, 0, 0);

/*
 * have data return 1, however return 0 if we have nothing
 */
static int timeriomem_rng_data_present(struct hwrng *rng, int wait)
{
	if (rng->priv == 0)
		return 1;

	if (!wait || timeriomem_rng_data->present)
		return timeriomem_rng_data->present;

	wait_for_completion(&timeriomem_rng_data->completion);

	return 1;
}

static int timeriomem_rng_data_read(struct hwrng *rng, u32 *data)
{
	unsigned long cur;
	s32 delay;

	*data = readl(timeriomem_rng_data->address);

	if (rng->priv != 0) {
		cur = jiffies;

		delay = cur - timeriomem_rng_timer.expires;
		delay = rng->priv - (delay % rng->priv);

		timeriomem_rng_timer.expires = cur + delay;
		timeriomem_rng_data->present = 0;

		init_completion(&timeriomem_rng_data->completion);
		add_timer(&timeriomem_rng_timer);
	}

	return 4;
}

static void timeriomem_rng_trigger(unsigned long dummy)
{
	timeriomem_rng_data->present = 1;
	complete(&timeriomem_rng_data->completion);
}

static struct hwrng timeriomem_rng_ops = {
	.name		= "timeriomem",
	.data_present	= timeriomem_rng_data_present,
	.data_read	= timeriomem_rng_data_read,
	.priv		= 0,
};

static int __devinit timeriomem_rng_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!res)
		return -ENOENT;

	timeriomem_rng_data = pdev->dev.platform_data;

	timeriomem_rng_data->address = ioremap(res->start, resource_size(res));
	if (!timeriomem_rng_data->address)
		return -EIO;

	if (timeriomem_rng_data->period != 0
		&& usecs_to_jiffies(timeriomem_rng_data->period) > 0) {
		timeriomem_rng_timer.expires = jiffies;

		timeriomem_rng_ops.priv = usecs_to_jiffies(
						timeriomem_rng_data->period);
	}
	timeriomem_rng_data->present = 1;

	ret = hwrng_register(&timeriomem_rng_ops);
	if (ret)
		goto failed;

	dev_info(&pdev->dev, "32bits from 0x%p @ %dus\n",
			timeriomem_rng_data->address,
			timeriomem_rng_data->period);

	return 0;

failed:
	dev_err(&pdev->dev, "problem registering\n");
	iounmap(timeriomem_rng_data->address);

	return ret;
}

static int __devexit timeriomem_rng_remove(struct platform_device *pdev)
{
	del_timer_sync(&timeriomem_rng_timer);
	hwrng_unregister(&timeriomem_rng_ops);

	iounmap(timeriomem_rng_data->address);

	return 0;
}

static struct platform_driver timeriomem_rng_driver = {
	.driver = {
		.name		= "timeriomem_rng",
		.owner		= THIS_MODULE,
	},
	.probe		= timeriomem_rng_probe,
	.remove		= __devexit_p(timeriomem_rng_remove),
=======
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/hw_random.h>
#include <linux/io.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/timeriomem-rng.h>

struct timeriomem_rng_private {
	void __iomem		*io_base;
	ktime_t			period;
	unsigned int		present:1;

	struct hrtimer		timer;
	struct completion	completion;

	struct hwrng		rng_ops;
};

static int timeriomem_rng_read(struct hwrng *hwrng, void *data,
				size_t max, bool wait)
{
	struct timeriomem_rng_private *priv =
		container_of(hwrng, struct timeriomem_rng_private, rng_ops);
	int retval = 0;
	int period_us = ktime_to_us(priv->period);

	/*
	 * There may not have been enough time for new data to be generated
	 * since the last request.  If the caller doesn't want to wait, let them
	 * bail out.  Otherwise, wait for the completion.  If the new data has
	 * already been generated, the completion should already be available.
	 */
	if (!wait && !priv->present)
		return 0;

	wait_for_completion(&priv->completion);

	do {
		/*
		 * After the first read, all additional reads will need to wait
		 * for the RNG to generate new data.  Since the period can have
		 * a wide range of values (1us to 1s have been observed), allow
		 * for 1% tolerance in the sleep time rather than a fixed value.
		 */
		if (retval > 0)
			usleep_range(period_us,
					period_us + max(1, period_us / 100));

		*(u32 *)data = readl(priv->io_base);
		retval += sizeof(u32);
		data += sizeof(u32);
		max -= sizeof(u32);
	} while (wait && max > sizeof(u32));

	/*
	 * Block any new callers until the RNG has had time to generate new
	 * data.
	 */
	priv->present = 0;
	reinit_completion(&priv->completion);
	hrtimer_forward_now(&priv->timer, priv->period);
	hrtimer_restart(&priv->timer);

	return retval;
}

static enum hrtimer_restart timeriomem_rng_trigger(struct hrtimer *timer)
{
	struct timeriomem_rng_private *priv
		= container_of(timer, struct timeriomem_rng_private, timer);

	priv->present = 1;
	complete(&priv->completion);

	return HRTIMER_NORESTART;
}

static int timeriomem_rng_probe(struct platform_device *pdev)
{
	struct timeriomem_rng_data *pdata = pdev->dev.platform_data;
	struct timeriomem_rng_private *priv;
	struct resource *res;
	int err = 0;
	int period;

	if (!pdev->dev.of_node && !pdata) {
		dev_err(&pdev->dev, "timeriomem_rng_data is missing\n");
		return -EINVAL;
	}

	/* Allocate memory for the device structure (and zero it) */
	priv = devm_kzalloc(&pdev->dev,
			sizeof(struct timeriomem_rng_private), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);

	priv->io_base = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(priv->io_base))
		return PTR_ERR(priv->io_base);

	if (res->start % 4 != 0 || resource_size(res) < 4) {
		dev_err(&pdev->dev,
			"address must be at least four bytes wide and 32-bit aligned\n");
		return -EINVAL;
	}

	if (pdev->dev.of_node) {
		int i;

		if (!of_property_read_u32(pdev->dev.of_node,
						"period", &i))
			period = i;
		else {
			dev_err(&pdev->dev, "missing period\n");
			return -EINVAL;
		}

		if (!of_property_read_u32(pdev->dev.of_node,
						"quality", &i))
			priv->rng_ops.quality = i;
	} else {
		period = pdata->period;
		priv->rng_ops.quality = pdata->quality;
	}

	priv->period = ns_to_ktime(period * NSEC_PER_USEC);
	init_completion(&priv->completion);
	hrtimer_init(&priv->timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	priv->timer.function = timeriomem_rng_trigger;

	priv->rng_ops.name = dev_name(&pdev->dev);
	priv->rng_ops.read = timeriomem_rng_read;

	/* Assume random data is already available. */
	priv->present = 1;
	complete(&priv->completion);

	err = devm_hwrng_register(&pdev->dev, &priv->rng_ops);
	if (err) {
		dev_err(&pdev->dev, "problem registering\n");
		return err;
	}

	dev_info(&pdev->dev, "32bits from 0x%p @ %dus\n",
			priv->io_base, period);

	return 0;
}

static void timeriomem_rng_remove(struct platform_device *pdev)
{
	struct timeriomem_rng_private *priv = platform_get_drvdata(pdev);

	hrtimer_cancel(&priv->timer);
}

static const struct of_device_id timeriomem_rng_match[] = {
	{ .compatible = "timeriomem_rng" },
	{},
};
MODULE_DEVICE_TABLE(of, timeriomem_rng_match);

static struct platform_driver timeriomem_rng_driver = {
	.driver = {
		.name		= "timeriomem_rng",
		.of_match_table	= timeriomem_rng_match,
	},
	.probe		= timeriomem_rng_probe,
	.remove_new	= timeriomem_rng_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(timeriomem_rng_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander Clouter <alex@digriz.org.uk>");
MODULE_DESCRIPTION("Timer IOMEM H/W RNG driver");
