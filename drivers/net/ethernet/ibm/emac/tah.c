<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/net/ethernet/ibm/emac/tah.c
 *
 * Driver for PowerPC 4xx on-chip ethernet controller, TAH support.
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *                <benh@kernel.crashing.org>
 *
 * Based on the arch/ppc version of the driver:
 *
 * Copyright 2004 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
 *
 * Copyright (c) 2005 Eugene Surovegin <ebs@ebshome.net>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
=======
 */
#include <linux/mod_devicetable.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/io.h>

#include "emac.h"
#include "core.h"

<<<<<<< HEAD
int __devinit tah_attach(struct platform_device *ofdev, int channel)
{
	struct tah_instance *dev = dev_get_drvdata(&ofdev->dev);
=======
int tah_attach(struct platform_device *ofdev, int channel)
{
	struct tah_instance *dev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&dev->lock);
	/* Reset has been done at probe() time... nothing else to do for now */
	++dev->users;
	mutex_unlock(&dev->lock);

	return 0;
}

void tah_detach(struct platform_device *ofdev, int channel)
{
<<<<<<< HEAD
	struct tah_instance *dev = dev_get_drvdata(&ofdev->dev);
=======
	struct tah_instance *dev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&dev->lock);
	--dev->users;
	mutex_unlock(&dev->lock);
}

void tah_reset(struct platform_device *ofdev)
{
<<<<<<< HEAD
	struct tah_instance *dev = dev_get_drvdata(&ofdev->dev);
=======
	struct tah_instance *dev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tah_regs __iomem *p = dev->base;
	int n;

	/* Reset TAH */
	out_be32(&p->mr, TAH_MR_SR);
	n = 100;
	while ((in_be32(&p->mr) & TAH_MR_SR) && n)
		--n;

	if (unlikely(!n))
<<<<<<< HEAD
		printk(KERN_ERR "%s: reset timeout\n",
			ofdev->dev.of_node->full_name);
=======
		printk(KERN_ERR "%pOF: reset timeout\n", ofdev->dev.of_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* 10KB TAH TX FIFO accommodates the max MTU of 9000 */
	out_be32(&p->mr,
		 TAH_MR_CVR | TAH_MR_ST_768 | TAH_MR_TFS_10KB | TAH_MR_DTFP |
		 TAH_MR_DIG);
}

int tah_get_regs_len(struct platform_device *ofdev)
{
	return sizeof(struct emac_ethtool_regs_subhdr) +
		sizeof(struct tah_regs);
}

void *tah_dump_regs(struct platform_device *ofdev, void *buf)
{
<<<<<<< HEAD
	struct tah_instance *dev = dev_get_drvdata(&ofdev->dev);
=======
	struct tah_instance *dev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct emac_ethtool_regs_subhdr *hdr = buf;
	struct tah_regs *regs = (struct tah_regs *)(hdr + 1);

	hdr->version = 0;
	hdr->index = 0; /* for now, are there chips with more than one
			 * zmii ? if yes, then we'll add a cell_index
			 * like we do for emac
			 */
	memcpy_fromio(regs, dev->base, sizeof(struct tah_regs));
	return regs + 1;
}

<<<<<<< HEAD
static int __devinit tah_probe(struct platform_device *ofdev)
=======
static int tah_probe(struct platform_device *ofdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device_node *np = ofdev->dev.of_node;
	struct tah_instance *dev;
	struct resource regs;
	int rc;

	rc = -ENOMEM;
	dev = kzalloc(sizeof(struct tah_instance), GFP_KERNEL);
	if (dev == NULL)
		goto err_gone;

	mutex_init(&dev->lock);
	dev->ofdev = ofdev;

	rc = -ENXIO;
	if (of_address_to_resource(np, 0, &regs)) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: Can't get registers address\n",
		       np->full_name);
=======
		printk(KERN_ERR "%pOF: Can't get registers address\n", np);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_free;
	}

	rc = -ENOMEM;
	dev->base = (struct tah_regs __iomem *)ioremap(regs.start,
					       sizeof(struct tah_regs));
	if (dev->base == NULL) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: Can't map device registers!\n",
		       np->full_name);
		goto err_free;
	}

	dev_set_drvdata(&ofdev->dev, dev);
=======
		printk(KERN_ERR "%pOF: Can't map device registers!\n", np);
		goto err_free;
	}

	platform_set_drvdata(ofdev, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize TAH and enable IPv4 checksum verification, no TSO yet */
	tah_reset(ofdev);

<<<<<<< HEAD
	printk(KERN_INFO
	       "TAH %s initialized\n", ofdev->dev.of_node->full_name);
=======
	printk(KERN_INFO "TAH %pOF initialized\n", ofdev->dev.of_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wmb();

	return 0;

 err_free:
	kfree(dev);
 err_gone:
	return rc;
}

<<<<<<< HEAD
static int __devexit tah_remove(struct platform_device *ofdev)
{
	struct tah_instance *dev = dev_get_drvdata(&ofdev->dev);

	dev_set_drvdata(&ofdev->dev, NULL);
=======
static void tah_remove(struct platform_device *ofdev)
{
	struct tah_instance *dev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ON(dev->users != 0);

	iounmap(dev->base);
	kfree(dev);
<<<<<<< HEAD

	return 0;
}

static struct of_device_id tah_match[] =
=======
}

static const struct of_device_id tah_match[] =
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	{
		.compatible	= "ibm,tah",
	},
	/* For backward compat with old DT */
	{
		.type		= "tah",
	},
	{},
};

static struct platform_driver tah_driver = {
	.driver = {
		.name = "emac-tah",
<<<<<<< HEAD
		.owner = THIS_MODULE,
		.of_match_table = tah_match,
	},
	.probe = tah_probe,
	.remove = tah_remove,
=======
		.of_match_table = tah_match,
	},
	.probe = tah_probe,
	.remove_new = tah_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __init tah_init(void)
{
	return platform_driver_register(&tah_driver);
}

void tah_exit(void)
{
	platform_driver_unregister(&tah_driver);
}
