<<<<<<< HEAD
/* rtc-sun4v.c: Hypervisor based RTC for SUN4V systems.
 *
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */

#include <linux/kernel.h>
#include <linux/module.h>
=======
// SPDX-License-Identifier: GPL-2.0
/* rtc-sun4v.c: Hypervisor based RTC for SUN4V systems.
 *
 * Author: David S. Miller
 *
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

#include <asm/hypervisor.h>

static unsigned long hypervisor_get_time(void)
{
	unsigned long ret, time;
	int retries = 10000;

retry:
	ret = sun4v_tod_get(&time);
	if (ret == HV_EOK)
		return time;
	if (ret == HV_EWOULDBLOCK) {
		if (--retries > 0) {
			udelay(100);
			goto retry;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "SUN4V: tod_get() timed out.\n");
		return 0;
	}
	printk(KERN_WARNING "SUN4V: tod_get() not supported.\n");
=======
		pr_warn("tod_get() timed out.\n");
		return 0;
	}
	pr_warn("tod_get() not supported.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int sun4v_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	rtc_time_to_tm(hypervisor_get_time(), tm);
=======
	rtc_time64_to_tm(hypervisor_get_time(), tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int hypervisor_set_time(unsigned long secs)
{
	unsigned long ret;
	int retries = 10000;

retry:
	ret = sun4v_tod_set(secs);
	if (ret == HV_EOK)
		return 0;
	if (ret == HV_EWOULDBLOCK) {
		if (--retries > 0) {
			udelay(100);
			goto retry;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "SUN4V: tod_set() timed out.\n");
		return -EAGAIN;
	}
	printk(KERN_WARNING "SUN4V: tod_set() not supported.\n");
=======
		pr_warn("tod_set() timed out.\n");
		return -EAGAIN;
	}
	pr_warn("tod_set() not supported.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EOPNOTSUPP;
}

static int sun4v_set_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	unsigned long secs;
	int err;

	err = rtc_tm_to_time(tm, &secs);
	if (err)
		return err;

	return hypervisor_set_time(secs);
=======
	return hypervisor_set_time(rtc_tm_to_time64(tm));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct rtc_class_ops sun4v_rtc_ops = {
	.read_time	= sun4v_read_time,
	.set_time	= sun4v_set_time,
};

static int __init sun4v_rtc_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct rtc_device *rtc = rtc_device_register("sun4v", &pdev->dev,
				     &sun4v_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(pdev, rtc);
	return 0;
}

static int __exit sun4v_rtc_remove(struct platform_device *pdev)
{
	struct rtc_device *rtc = platform_get_drvdata(pdev);

	rtc_device_unregister(rtc);
	return 0;
=======
	struct rtc_device *rtc;

	rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	rtc->ops = &sun4v_rtc_ops;
	rtc->range_max = U64_MAX;
	platform_set_drvdata(pdev, rtc);

	return devm_rtc_register_device(rtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver sun4v_rtc_driver = {
	.driver		= {
		.name	= "rtc-sun4v",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(sun4v_rtc_remove),
};

static int __init sun4v_rtc_init(void)
{
	return platform_driver_probe(&sun4v_rtc_driver, sun4v_rtc_probe);
}

static void __exit sun4v_rtc_exit(void)
{
	platform_driver_unregister(&sun4v_rtc_driver);
}

module_init(sun4v_rtc_init);
module_exit(sun4v_rtc_exit);

MODULE_AUTHOR("David S. Miller <davem@davemloft.net>");
MODULE_DESCRIPTION("SUN4V RTC driver");
MODULE_LICENSE("GPL");
=======
	},
};

builtin_platform_driver_probe(sun4v_rtc_driver, sun4v_rtc_probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
