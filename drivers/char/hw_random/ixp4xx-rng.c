<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/char/hw_random/ixp4xx-rng.c
 *
 * RNG driver for Intel IXP4xx family of NPUs
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright 2005 (c) MontaVista Software, Inc.
 *
 * Fixes by Michael Buesch
<<<<<<< HEAD
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/hw_random.h>

#include <asm/io.h>
#include <mach/hardware.h>

=======
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/hw_random.h>
#include <linux/of.h>
#include <linux/soc/ixp4xx/cpu.h>

#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int ixp4xx_rng_data_read(struct hwrng *rng, u32 *buffer)
{
	void __iomem * rng_base = (void __iomem *)rng->priv;

	*buffer = __raw_readl(rng_base);

	return 4;
}

static struct hwrng ixp4xx_rng_ops = {
	.name		= "ixp4xx",
	.data_read	= ixp4xx_rng_data_read,
};

<<<<<<< HEAD
static int __init ixp4xx_rng_init(void)
{
	void __iomem * rng_base;
	int err;

	rng_base = ioremap(0x70002100, 4);
	if (!rng_base)
		return -ENOMEM;
	ixp4xx_rng_ops.priv = (unsigned long)rng_base;
	err = hwrng_register(&ixp4xx_rng_ops);
	if (err)
		iounmap(rng_base);

	return err;
}

static void __exit ixp4xx_rng_exit(void)
{
	void __iomem * rng_base = (void __iomem *)ixp4xx_rng_ops.priv;

	hwrng_unregister(&ixp4xx_rng_ops);
	iounmap(rng_base);
}

module_init(ixp4xx_rng_init);
module_exit(ixp4xx_rng_exit);

MODULE_AUTHOR("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("H/W Random Number Generator (RNG) driver for IXP4xx");
=======
static int ixp4xx_rng_probe(struct platform_device *pdev)
{
	void __iomem * rng_base;
	struct device *dev = &pdev->dev;

	if (!cpu_is_ixp46x()) /* includes IXP455 */
		return -ENOSYS;

	rng_base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(rng_base))
		return PTR_ERR(rng_base);

	ixp4xx_rng_ops.priv = (unsigned long)rng_base;
	return devm_hwrng_register(dev, &ixp4xx_rng_ops);
}

static const struct of_device_id ixp4xx_rng_of_match[] = {
	{
		.compatible = "intel,ixp46x-rng",
	},
	{},
};
MODULE_DEVICE_TABLE(of, ixp4xx_rng_of_match);

static struct platform_driver ixp4xx_rng_driver = {
	.driver = {
		.name = "ixp4xx-hwrandom",
		.of_match_table = ixp4xx_rng_of_match,
	},
	.probe = ixp4xx_rng_probe,
};
module_platform_driver(ixp4xx_rng_driver);

MODULE_AUTHOR("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("H/W Pseudo-Random Number Generator (RNG) driver for IXP45x/46x");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
