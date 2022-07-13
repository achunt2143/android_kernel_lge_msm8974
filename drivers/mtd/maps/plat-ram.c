<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* drivers/mtd/maps/plat-ram.c
 *
 * (c) 2004-2005 Simtec Electronics
 *	http://www.simtec.co.uk/products/SWLINUX/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Generic platform device based RAM map
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
*/

#include <linux/module.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/plat-ram.h>

#include <asm/io.h>

/* private structure for each mtd platform ram device created */

struct platram_info {
	struct device		*dev;
	struct mtd_info		*mtd;
	struct map_info		 map;
<<<<<<< HEAD
	struct resource		*area;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct platdata_mtd_ram	*pdata;
};

/* to_platram_info()
 *
 * device private data to struct platram_info conversion
*/

static inline struct platram_info *to_platram_info(struct platform_device *dev)
{
<<<<<<< HEAD
	return (struct platram_info *)platform_get_drvdata(dev);
=======
	return platform_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* platram_setrw
 *
 * call the platform device's set rw/ro control
 *
 * to = 0 => read-only
 *    = 1 => read-write
*/

static inline void platram_setrw(struct platram_info *info, int to)
{
	if (info->pdata == NULL)
		return;

	if (info->pdata->set_rw != NULL)
		(info->pdata->set_rw)(info->dev, to);
}

/* platram_remove
 *
 * called to remove the device from the driver's control
*/

<<<<<<< HEAD
static int platram_remove(struct platform_device *pdev)
{
	struct platram_info *info = to_platram_info(pdev);

	platform_set_drvdata(pdev, NULL);

	dev_dbg(&pdev->dev, "removing device\n");

	if (info == NULL)
		return 0;
=======
static void platram_remove(struct platform_device *pdev)
{
	struct platram_info *info = to_platram_info(pdev);

	dev_dbg(&pdev->dev, "removing device\n");

	if (info == NULL)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (info->mtd) {
		mtd_device_unregister(info->mtd);
		map_destroy(info->mtd);
	}

	/* ensure ram is left read-only */

	platram_setrw(info, PLATRAM_RO);

<<<<<<< HEAD
	/* release resources */

	if (info->area) {
		release_resource(info->area);
		kfree(info->area);
	}

	if (info->map.virt != NULL)
		iounmap(info->map.virt);

	kfree(info);

	return 0;
=======
	kfree(info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* platram_probe
 *
 * called from device drive system when a device matching our
 * driver is found.
*/

static int platram_probe(struct platform_device *pdev)
{
	struct platdata_mtd_ram	*pdata;
	struct platram_info *info;
	struct resource *res;
	int err = 0;

	dev_dbg(&pdev->dev, "probe entered\n");

<<<<<<< HEAD
	if (pdev->dev.platform_data == NULL) {
=======
	if (dev_get_platdata(&pdev->dev) == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&pdev->dev, "no platform data supplied\n");
		err = -ENOENT;
		goto exit_error;
	}

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info == NULL) {
		dev_err(&pdev->dev, "no memory for flash info\n");
=======
	pdata = dev_get_platdata(&pdev->dev);

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOMEM;
		goto exit_error;
	}

	platform_set_drvdata(pdev, info);

	info->dev = &pdev->dev;
	info->pdata = pdata;

	/* get the resource for the memory mapping */
<<<<<<< HEAD

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (res == NULL) {
		dev_err(&pdev->dev, "no memory resource specified\n");
		err = -ENOENT;
=======
	info->map.virt = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(info->map.virt)) {
		err = PTR_ERR(info->map.virt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto exit_free;
	}

	dev_dbg(&pdev->dev, "got platform resource %p (0x%llx)\n", res,
		(unsigned long long)res->start);

	/* setup map parameters */

	info->map.phys = res->start;
	info->map.size = resource_size(res);
	info->map.name = pdata->mapname != NULL ?
			(char *)pdata->mapname : (char *)pdev->name;
	info->map.bankwidth = pdata->bankwidth;

<<<<<<< HEAD
	/* register our usage of the memory area */

	info->area = request_mem_region(res->start, info->map.size, pdev->name);
	if (info->area == NULL) {
		dev_err(&pdev->dev, "failed to request memory region\n");
		err = -EIO;
		goto exit_free;
	}

	/* remap the memory area */

	info->map.virt = ioremap(res->start, info->map.size);
	dev_dbg(&pdev->dev, "virt %p, %lu bytes\n", info->map.virt, info->map.size);

	if (info->map.virt == NULL) {
		dev_err(&pdev->dev, "failed to ioremap() region\n");
		err = -EIO;
		goto exit_free;
	}

=======
	dev_dbg(&pdev->dev, "virt %p, %lu bytes\n", info->map.virt, info->map.size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	simple_map_init(&info->map);

	dev_dbg(&pdev->dev, "initialised map, probing for mtd\n");

	/* probe for the right mtd map driver
	 * supplied by the platform_data struct */

	if (pdata->map_probes) {
<<<<<<< HEAD
		const char **map_probes = pdata->map_probes;
=======
		const char * const *map_probes = pdata->map_probes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for ( ; !info->mtd && *map_probes; map_probes++)
			info->mtd = do_map_probe(*map_probes , &info->map);
	}
	/* fallback to map_ram */
	else
		info->mtd = do_map_probe("map_ram", &info->map);

	if (info->mtd == NULL) {
		dev_err(&pdev->dev, "failed to probe for map_ram\n");
		err = -ENOMEM;
		goto exit_free;
	}

<<<<<<< HEAD
	info->mtd->owner = THIS_MODULE;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info->mtd->dev.parent = &pdev->dev;

	platram_setrw(info, PLATRAM_RW);

<<<<<<< HEAD
	/* check to see if there are any available partitions, or wether
=======
	/* check to see if there are any available partitions, or whether
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * to add this device whole */

	err = mtd_device_parse_register(info->mtd, pdata->probes, NULL,
					pdata->partitions,
					pdata->nr_partitions);
<<<<<<< HEAD
	if (!err)
		dev_info(&pdev->dev, "registered mtd device\n");
=======
	if (err) {
		dev_err(&pdev->dev, "failed to register mtd device\n");
		goto exit_free;
	}

	dev_info(&pdev->dev, "registered mtd device\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pdata->nr_partitions) {
		/* add the whole device. */
		err = mtd_device_register(info->mtd, NULL, 0);
		if (err) {
			dev_err(&pdev->dev,
				"failed to register the entire device\n");
<<<<<<< HEAD
		}
	}

	return err;
=======
			goto exit_free;
		}
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 exit_free:
	platram_remove(pdev);
 exit_error:
	return err;
}

/* device driver info */

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:mtd-ram");

static struct platform_driver platram_driver = {
	.probe		= platram_probe,
<<<<<<< HEAD
	.remove		= platram_remove,
	.driver		= {
		.name	= "mtd-ram",
		.owner	= THIS_MODULE,
	},
};

/* module init/exit */

static int __init platram_init(void)
{
	printk("Generic platform RAM MTD, (c) 2004 Simtec Electronics\n");
	return platform_driver_register(&platram_driver);
}

static void __exit platram_exit(void)
{
	platform_driver_unregister(&platram_driver);
}

module_init(platram_init);
module_exit(platram_exit);
=======
	.remove_new	= platram_remove,
	.driver		= {
		.name	= "mtd-ram",
	},
};

module_platform_driver(platram_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("MTD platform RAM map driver");
