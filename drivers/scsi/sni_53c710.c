<<<<<<< HEAD
/* -*- mode: c; c-basic-offset: 8 -*- */
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* SNI RM driver
 *
 * Copyright (C) 2001 by James.Bottomley@HansenPartnership.com
**-----------------------------------------------------------------------------
**
<<<<<<< HEAD
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
**
**-----------------------------------------------------------------------------
 */

/*
 * Based on lasi700.c
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/blkdev.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>

#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/irq.h>
#include <asm/delay.h>

#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_transport.h>
#include <scsi/scsi_transport_spi.h>

#include "53c700.h"

MODULE_AUTHOR("Thomas Bogendörfer");
MODULE_DESCRIPTION("SNI RM 53c710 SCSI Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:snirm_53c710");

#define SNIRM710_CLOCK	32

static struct scsi_host_template snirm710_template = {
	.name		= "SNI RM SCSI 53c710",
	.proc_name	= "snirm_53c710",
	.this_id	= 7,
	.module		= THIS_MODULE,
};

<<<<<<< HEAD
static int __devinit snirm710_probe(struct platform_device *dev)
=======
static int snirm710_probe(struct platform_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long base;
	struct NCR_700_Host_Parameters *hostdata;
	struct Scsi_Host *host;
	struct  resource *res;
<<<<<<< HEAD
=======
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	base = res->start;
	hostdata = kzalloc(sizeof(*hostdata), GFP_KERNEL);
<<<<<<< HEAD
	if (!hostdata) {
		dev_printk(KERN_ERR, dev, "Failed to allocate host data\n");
		return -ENOMEM;
	}

	hostdata->dev = &dev->dev;
	dma_set_mask(&dev->dev, DMA_BIT_MASK(32));
	hostdata->base = ioremap_nocache(base, 0x100);
=======
	if (!hostdata)
		return -ENOMEM;

	hostdata->dev = &dev->dev;
	dma_set_mask(&dev->dev, DMA_BIT_MASK(32));
	hostdata->base = ioremap(base, 0x100);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hostdata->differential = 0;

	hostdata->clock = SNIRM710_CLOCK;
	hostdata->force_le_on_be = 1;
	hostdata->chip710 = 1;
	hostdata->burst_length = 4;

	host = NCR_700_detect(&snirm710_template, hostdata, &dev->dev);
	if (!host)
		goto out_kfree;
	host->this_id = 7;
	host->base = base;
<<<<<<< HEAD
	host->irq = platform_get_irq(dev, 0);
=======
	host->irq = rc = platform_get_irq(dev, 0);
	if (rc < 0)
		goto out_put_host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if(request_irq(host->irq, NCR_700_intr, IRQF_SHARED, "snirm710", host)) {
		printk(KERN_ERR "snirm710: request_irq failed!\n");
		goto out_put_host;
	}

	dev_set_drvdata(&dev->dev, host);
	scsi_scan_host(host);

	return 0;

 out_put_host:
	scsi_host_put(host);
 out_kfree:
	iounmap(hostdata->base);
	kfree(hostdata);
	return -ENODEV;
}

<<<<<<< HEAD
static int __exit snirm710_driver_remove(struct platform_device *dev)
=======
static void snirm710_driver_remove(struct platform_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct Scsi_Host *host = dev_get_drvdata(&dev->dev);
	struct NCR_700_Host_Parameters *hostdata =
		(struct NCR_700_Host_Parameters *)host->hostdata[0];

	scsi_remove_host(host);
	NCR_700_release(host);
	free_irq(host->irq, host);
	iounmap(hostdata->base);
	kfree(hostdata);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver snirm710_driver = {
	.probe	= snirm710_probe,
<<<<<<< HEAD
	.remove	= __devexit_p(snirm710_driver_remove),
	.driver	= {
		.name	= "snirm_53c710",
		.owner	= THIS_MODULE,
	},
};

static int __init snirm710_init(void)
{
	return platform_driver_register(&snirm710_driver);
}

static void __exit snirm710_exit(void)
{
	platform_driver_unregister(&snirm710_driver);
}

module_init(snirm710_init);
module_exit(snirm710_exit);
=======
	.remove_new = snirm710_driver_remove,
	.driver	= {
		.name	= "snirm_53c710",
	},
};
module_platform_driver(snirm710_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
