/* rtc-starfire.c: Starfire platform RTC driver.
 *
<<<<<<< HEAD
=======
 * Author: David S. Miller
 * License: GPL
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

#include <asm/oplib.h>

<<<<<<< HEAD
MODULE_AUTHOR("David S. Miller <davem@davemloft.net>");
MODULE_DESCRIPTION("Starfire RTC driver");
MODULE_LICENSE("GPL");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u32 starfire_get_time(void)
{
	static char obp_gettod[32];
	static u32 unix_tod;

	sprintf(obp_gettod, "h# %08x unix-gettod",
		(unsigned int) (long) &unix_tod);
	prom_feval(obp_gettod);

	return unix_tod;
}

static int starfire_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	rtc_time_to_tm(starfire_get_time(), tm);
	return rtc_valid_tm(tm);
=======
	rtc_time64_to_tm(starfire_get_time(), tm);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct rtc_class_ops starfire_rtc_ops = {
	.read_time	= starfire_read_time,
};

static int __init starfire_rtc_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct rtc_device *rtc = rtc_device_register("starfire", &pdev->dev,
				     &starfire_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(pdev, rtc);

	return 0;
}

static int __exit starfire_rtc_remove(struct platform_device *pdev)
{
	struct rtc_device *rtc = platform_get_drvdata(pdev);

	rtc_device_unregister(rtc);

	return 0;
=======
	struct rtc_device *rtc;

	rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	rtc->ops = &starfire_rtc_ops;
	rtc->range_max = U32_MAX;

	platform_set_drvdata(pdev, rtc);

	return devm_rtc_register_device(rtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver starfire_rtc_driver = {
	.driver		= {
		.name	= "rtc-starfire",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(starfire_rtc_remove),
};

static int __init starfire_rtc_init(void)
{
	return platform_driver_probe(&starfire_rtc_driver, starfire_rtc_probe);
}

static void __exit starfire_rtc_exit(void)
{
	platform_driver_unregister(&starfire_rtc_driver);
}

module_init(starfire_rtc_init);
module_exit(starfire_rtc_exit);
=======
	},
};

builtin_platform_driver_probe(starfire_rtc_driver, starfire_rtc_probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
