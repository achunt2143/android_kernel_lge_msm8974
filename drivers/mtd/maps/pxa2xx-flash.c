<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Map driver for Intel XScale PXA2xx platforms.
 *
 * Author:	Nicolas Pitre
 * Copyright:	(C) 2001 MontaVista Software Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>
<<<<<<< HEAD
#include <mach/hardware.h>

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/mach/flash.h>

#define CACHELINESIZE	32

static void pxa2xx_map_inval_cache(struct map_info *map, unsigned long from,
				      ssize_t len)
{
	unsigned long start = (unsigned long)map->cached + from;
	unsigned long end = start + len;

	start &= ~(CACHELINESIZE - 1);
	while (start < end) {
		/* invalidate D cache line */
		asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (start));
		start += CACHELINESIZE;
	}
}

struct pxa2xx_flash_info {
	struct mtd_info		*mtd;
	struct map_info		map;
};

<<<<<<< HEAD

static const char *probes[] = { "RedBoot", "cmdlinepart", NULL };


static int __devinit pxa2xx_flash_probe(struct platform_device *pdev)
{
	struct flash_platform_data *flash = pdev->dev.platform_data;
=======
static const char * const probes[] = { "RedBoot", "cmdlinepart", NULL };

static int pxa2xx_flash_probe(struct platform_device *pdev)
{
	struct flash_platform_data *flash = dev_get_platdata(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pxa2xx_flash_info *info;
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	info = kzalloc(sizeof(struct pxa2xx_flash_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

<<<<<<< HEAD
	info->map.name = (char *) flash->name;
=======
	info->map.name = flash->name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info->map.bankwidth = flash->width;
	info->map.phys = res->start;
	info->map.size = resource_size(res);

	info->map.virt = ioremap(info->map.phys, info->map.size);
	if (!info->map.virt) {
		printk(KERN_WARNING "Failed to ioremap %s\n",
		       info->map.name);
<<<<<<< HEAD
		return -ENOMEM;
	}
	info->map.cached =
		ioremap_cached(info->map.phys, info->map.size);
=======
		kfree(info);
		return -ENOMEM;
	}
	info->map.cached = ioremap_cache(info->map.phys, info->map.size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!info->map.cached)
		printk(KERN_WARNING "Failed to ioremap cached %s\n",
		       info->map.name);
	info->map.inval_cache = pxa2xx_map_inval_cache;
	simple_map_init(&info->map);

	printk(KERN_NOTICE
	       "Probing %s at physical address 0x%08lx"
	       " (%d-bit bankwidth)\n",
	       info->map.name, (unsigned long)info->map.phys,
	       info->map.bankwidth * 8);

	info->mtd = do_map_probe(flash->map_name, &info->map);

	if (!info->mtd) {
		iounmap((void *)info->map.virt);
		if (info->map.cached)
			iounmap(info->map.cached);
<<<<<<< HEAD
		return -EIO;
	}
	info->mtd->owner = THIS_MODULE;
=======
		kfree(info);
		return -EIO;
	}
	info->mtd->dev.parent = &pdev->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mtd_device_parse_register(info->mtd, probes, NULL, flash->parts,
				  flash->nr_parts);

	platform_set_drvdata(pdev, info);
	return 0;
}

<<<<<<< HEAD
static int __devexit pxa2xx_flash_remove(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

=======
static void pxa2xx_flash_remove(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mtd_device_unregister(info->mtd);

	map_destroy(info->mtd);
	iounmap(info->map.virt);
	if (info->map.cached)
		iounmap(info->map.cached);
	kfree(info);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PM
static void pxa2xx_flash_shutdown(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

	if (info && mtd_suspend(info->mtd) == 0)
		mtd_resume(info->mtd);
}
#else
#define pxa2xx_flash_shutdown NULL
#endif

static struct platform_driver pxa2xx_flash_driver = {
	.driver = {
		.name		= "pxa2xx-flash",
<<<<<<< HEAD
		.owner		= THIS_MODULE,
	},
	.probe		= pxa2xx_flash_probe,
	.remove		= __devexit_p(pxa2xx_flash_remove),
=======
	},
	.probe		= pxa2xx_flash_probe,
	.remove_new	= pxa2xx_flash_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.shutdown	= pxa2xx_flash_shutdown,
};

module_platform_driver(pxa2xx_flash_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicolas Pitre <nico@fluxnic.net>");
MODULE_DESCRIPTION("MTD map driver for Intel XScale PXA2xx");
