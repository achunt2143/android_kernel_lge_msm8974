<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * A driver for the RTC embedded in the Cirrus Logic EP93XX processors
 * Copyright (c) 2006 Tower Technologies
 *
 * Author: Alessandro Zummo <a.zummo@towertech.it>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
=======
 */

#include <linux/module.h>
#include <linux/mod_devicetable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/gfp.h>

#define EP93XX_RTC_DATA			0x000
#define EP93XX_RTC_MATCH		0x004
#define EP93XX_RTC_STATUS		0x008
<<<<<<< HEAD
#define  EP93XX_RTC_STATUS_INTR		 (1<<0)
#define EP93XX_RTC_LOAD			0x00C
#define EP93XX_RTC_CONTROL		0x010
#define  EP93XX_RTC_CONTROL_MIE		 (1<<0)
=======
#define  EP93XX_RTC_STATUS_INTR		 BIT(0)
#define EP93XX_RTC_LOAD			0x00C
#define EP93XX_RTC_CONTROL		0x010
#define  EP93XX_RTC_CONTROL_MIE		 BIT(0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define EP93XX_RTC_SWCOMP		0x108
#define  EP93XX_RTC_SWCOMP_DEL_MASK	 0x001f0000
#define  EP93XX_RTC_SWCOMP_DEL_SHIFT	 16
#define  EP93XX_RTC_SWCOMP_INT_MASK	 0x0000ffff
#define  EP93XX_RTC_SWCOMP_INT_SHIFT	 0

<<<<<<< HEAD
#define DRV_VERSION "0.3"

/*
 * struct device dev.platform_data is used to store our private data
 * because struct rtc_device does not have a variable to hold it.
 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct ep93xx_rtc {
	void __iomem	*mmio_base;
	struct rtc_device *rtc;
};

static int ep93xx_rtc_get_swcomp(struct device *dev, unsigned short *preload,
<<<<<<< HEAD
				unsigned short *delete)
{
	struct ep93xx_rtc *ep93xx_rtc = dev->platform_data;
	unsigned long comp;

	comp = __raw_readl(ep93xx_rtc->mmio_base + EP93XX_RTC_SWCOMP);
=======
				 unsigned short *delete)
{
	struct ep93xx_rtc *ep93xx_rtc = dev_get_drvdata(dev);
	unsigned long comp;

	comp = readl(ep93xx_rtc->mmio_base + EP93XX_RTC_SWCOMP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (preload)
		*preload = (comp & EP93XX_RTC_SWCOMP_INT_MASK)
				>> EP93XX_RTC_SWCOMP_INT_SHIFT;

	if (delete)
		*delete = (comp & EP93XX_RTC_SWCOMP_DEL_MASK)
				>> EP93XX_RTC_SWCOMP_DEL_SHIFT;

	return 0;
}

static int ep93xx_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	struct ep93xx_rtc *ep93xx_rtc = dev->platform_data;
	unsigned long time;

	 time = __raw_readl(ep93xx_rtc->mmio_base + EP93XX_RTC_DATA);

	rtc_time_to_tm(time, tm);
	return 0;
}

static int ep93xx_rtc_set_mmss(struct device *dev, unsigned long secs)
{
	struct ep93xx_rtc *ep93xx_rtc = dev->platform_data;

	__raw_writel(secs + 1, ep93xx_rtc->mmio_base + EP93XX_RTC_LOAD);
=======
	struct ep93xx_rtc *ep93xx_rtc = dev_get_drvdata(dev);
	unsigned long time;

	time = readl(ep93xx_rtc->mmio_base + EP93XX_RTC_DATA);

	rtc_time64_to_tm(time, tm);
	return 0;
}

static int ep93xx_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct ep93xx_rtc *ep93xx_rtc = dev_get_drvdata(dev);
	unsigned long secs = rtc_tm_to_time64(tm);

	writel(secs + 1, ep93xx_rtc->mmio_base + EP93XX_RTC_LOAD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int ep93xx_rtc_proc(struct device *dev, struct seq_file *seq)
{
	unsigned short preload, delete;

	ep93xx_rtc_get_swcomp(dev, &preload, &delete);

	seq_printf(seq, "preload\t\t: %d\n", preload);
	seq_printf(seq, "delete\t\t: %d\n", delete);

	return 0;
}

static const struct rtc_class_ops ep93xx_rtc_ops = {
	.read_time	= ep93xx_rtc_read_time,
<<<<<<< HEAD
	.set_mmss	= ep93xx_rtc_set_mmss,
	.proc		= ep93xx_rtc_proc,
};

static ssize_t ep93xx_rtc_show_comp_preload(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	unsigned short preload;

	ep93xx_rtc_get_swcomp(dev, &preload, NULL);

	return sprintf(buf, "%d\n", preload);
}
static DEVICE_ATTR(comp_preload, S_IRUGO, ep93xx_rtc_show_comp_preload, NULL);

static ssize_t ep93xx_rtc_show_comp_delete(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	unsigned short delete;

	ep93xx_rtc_get_swcomp(dev, NULL, &delete);

	return sprintf(buf, "%d\n", delete);
}
static DEVICE_ATTR(comp_delete, S_IRUGO, ep93xx_rtc_show_comp_delete, NULL);
=======
	.set_time	= ep93xx_rtc_set_time,
	.proc		= ep93xx_rtc_proc,
};

static ssize_t comp_preload_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	unsigned short preload;

	ep93xx_rtc_get_swcomp(dev->parent, &preload, NULL);

	return sprintf(buf, "%d\n", preload);
}
static DEVICE_ATTR_RO(comp_preload);

static ssize_t comp_delete_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	unsigned short delete;

	ep93xx_rtc_get_swcomp(dev->parent, NULL, &delete);

	return sprintf(buf, "%d\n", delete);
}
static DEVICE_ATTR_RO(comp_delete);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *ep93xx_rtc_attrs[] = {
	&dev_attr_comp_preload.attr,
	&dev_attr_comp_delete.attr,
	NULL
};

static const struct attribute_group ep93xx_rtc_sysfs_files = {
	.attrs	= ep93xx_rtc_attrs,
};

<<<<<<< HEAD
static int __init ep93xx_rtc_probe(struct platform_device *pdev)
{
	struct ep93xx_rtc *ep93xx_rtc;
	struct resource *res;
=======
static int ep93xx_rtc_probe(struct platform_device *pdev)
{
	struct ep93xx_rtc *ep93xx_rtc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	ep93xx_rtc = devm_kzalloc(&pdev->dev, sizeof(*ep93xx_rtc), GFP_KERNEL);
	if (!ep93xx_rtc)
		return -ENOMEM;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENXIO;

	if (!devm_request_mem_region(&pdev->dev, res->start,
				     resource_size(res), pdev->name))
		return -EBUSY;

	ep93xx_rtc->mmio_base = devm_ioremap(&pdev->dev, res->start,
					     resource_size(res));
	if (!ep93xx_rtc->mmio_base)
		return -ENXIO;

	pdev->dev.platform_data = ep93xx_rtc;
	platform_set_drvdata(pdev, ep93xx_rtc);

	ep93xx_rtc->rtc = rtc_device_register(pdev->name,
				&pdev->dev, &ep93xx_rtc_ops, THIS_MODULE);
	if (IS_ERR(ep93xx_rtc->rtc)) {
		err = PTR_ERR(ep93xx_rtc->rtc);
		goto exit;
	}

	err = sysfs_create_group(&pdev->dev.kobj, &ep93xx_rtc_sysfs_files);
	if (err)
		goto fail;

	return 0;

fail:
	rtc_device_unregister(ep93xx_rtc->rtc);
exit:
	platform_set_drvdata(pdev, NULL);
	pdev->dev.platform_data = NULL;
	return err;
}

static int __exit ep93xx_rtc_remove(struct platform_device *pdev)
{
	struct ep93xx_rtc *ep93xx_rtc = platform_get_drvdata(pdev);

	sysfs_remove_group(&pdev->dev.kobj, &ep93xx_rtc_sysfs_files);
	platform_set_drvdata(pdev, NULL);
	rtc_device_unregister(ep93xx_rtc->rtc);
	pdev->dev.platform_data = NULL;

	return 0;
}

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:ep93xx-rtc");
=======
	ep93xx_rtc->mmio_base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(ep93xx_rtc->mmio_base))
		return PTR_ERR(ep93xx_rtc->mmio_base);

	platform_set_drvdata(pdev, ep93xx_rtc);

	ep93xx_rtc->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(ep93xx_rtc->rtc))
		return PTR_ERR(ep93xx_rtc->rtc);

	ep93xx_rtc->rtc->ops = &ep93xx_rtc_ops;
	ep93xx_rtc->rtc->range_max = U32_MAX;

	err = rtc_add_group(ep93xx_rtc->rtc, &ep93xx_rtc_sysfs_files);
	if (err)
		return err;

	return devm_rtc_register_device(ep93xx_rtc->rtc);
}

static const struct of_device_id ep93xx_rtc_of_ids[] = {
	{ .compatible = "cirrus,ep9301-rtc" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, ep93xx_rtc_of_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct platform_driver ep93xx_rtc_driver = {
	.driver		= {
		.name	= "ep93xx-rtc",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(ep93xx_rtc_remove),
};

static int __init ep93xx_rtc_init(void)
{
        return platform_driver_probe(&ep93xx_rtc_driver, ep93xx_rtc_probe);
}

static void __exit ep93xx_rtc_exit(void)
{
	platform_driver_unregister(&ep93xx_rtc_driver);
}
=======
		.of_match_table = ep93xx_rtc_of_ids,
	},
	.probe		= ep93xx_rtc_probe,
};

module_platform_driver(ep93xx_rtc_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_DESCRIPTION("EP93XX RTC driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION(DRV_VERSION);

module_init(ep93xx_rtc_init);
module_exit(ep93xx_rtc_exit);
=======
MODULE_ALIAS("platform:ep93xx-rtc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
