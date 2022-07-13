<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2004 IBM Corporation
 *
 * Authors:
 * Leendert van Doorn <leendert@watson.ibm.com>
 * Dave Safford <safford@watson.ibm.com>
 * Reiner Sailer <sailer@watson.ibm.com>
 * Kylene Hall <kjhall@us.ibm.com>
 *
 * Maintained by: <tpmdd-devel@lists.sourceforge.net>
 *
 * Device driver for TCG/TCPA TPM (trusted platform module).
 * Specifications at www.trustedcomputinggroup.org	 
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 * 
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/platform_device.h>
#include <linux/slab.h>
#include "tpm.h"

/* National definitions */
enum tpm_nsc_addr{
	TPM_NSC_IRQ = 0x07,
	TPM_NSC_BASE0_HI = 0x60,
	TPM_NSC_BASE0_LO = 0x61,
	TPM_NSC_BASE1_HI = 0x62,
	TPM_NSC_BASE1_LO = 0x63
};

enum tpm_nsc_index {
	NSC_LDN_INDEX = 0x07,
	NSC_SID_INDEX = 0x20,
	NSC_LDC_INDEX = 0x30,
	NSC_DIO_INDEX = 0x60,
	NSC_CIO_INDEX = 0x62,
	NSC_IRQ_INDEX = 0x70,
	NSC_ITS_INDEX = 0x71
};

enum tpm_nsc_status_loc {
	NSC_STATUS = 0x01,
	NSC_COMMAND = 0x01,
	NSC_DATA = 0x00
};

/* status bits */
enum tpm_nsc_status {
	NSC_STATUS_OBF = 0x01,	/* output buffer full */
	NSC_STATUS_IBF = 0x02,	/* input buffer full */
	NSC_STATUS_F0 = 0x04,	/* F0 */
	NSC_STATUS_A2 = 0x08,	/* A2 */
	NSC_STATUS_RDY = 0x10,	/* ready to receive command */
	NSC_STATUS_IBR = 0x20	/* ready to receive data */
};

/* command bits */
enum tpm_nsc_cmd_mode {
	NSC_COMMAND_NORMAL = 0x01,	/* normal mode */
	NSC_COMMAND_EOC = 0x03,
	NSC_COMMAND_CANCEL = 0x22
};
<<<<<<< HEAD
=======

struct tpm_nsc_priv {
	unsigned long base;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Wait for a certain status to appear
 */
static int wait_for_stat(struct tpm_chip *chip, u8 mask, u8 val, u8 * data)
{
<<<<<<< HEAD
	unsigned long stop;

	/* status immediately available check */
	*data = inb(chip->vendor.base + NSC_STATUS);
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);
	unsigned long stop;

	/* status immediately available check */
	*data = inb(priv->base + NSC_STATUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((*data & mask) == val)
		return 0;

	/* wait for status */
	stop = jiffies + 10 * HZ;
	do {
		msleep(TPM_TIMEOUT);
<<<<<<< HEAD
		*data = inb(chip->vendor.base + 1);
=======
		*data = inb(priv->base + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((*data & mask) == val)
			return 0;
	}
	while (time_before(jiffies, stop));

	return -EBUSY;
}

static int nsc_wait_for_ready(struct tpm_chip *chip)
{
<<<<<<< HEAD
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int status;
	unsigned long stop;

	/* status immediately available check */
<<<<<<< HEAD
	status = inb(chip->vendor.base + NSC_STATUS);
	if (status & NSC_STATUS_OBF)
		status = inb(chip->vendor.base + NSC_DATA);
=======
	status = inb(priv->base + NSC_STATUS);
	if (status & NSC_STATUS_OBF)
		status = inb(priv->base + NSC_DATA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status & NSC_STATUS_RDY)
		return 0;

	/* wait for status */
	stop = jiffies + 100;
	do {
		msleep(TPM_TIMEOUT);
<<<<<<< HEAD
		status = inb(chip->vendor.base + NSC_STATUS);
		if (status & NSC_STATUS_OBF)
			status = inb(chip->vendor.base + NSC_DATA);
=======
		status = inb(priv->base + NSC_STATUS);
		if (status & NSC_STATUS_OBF)
			status = inb(priv->base + NSC_DATA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (status & NSC_STATUS_RDY)
			return 0;
	}
	while (time_before(jiffies, stop));

<<<<<<< HEAD
	dev_info(chip->dev, "wait for ready failed\n");
=======
	dev_info(&chip->dev, "wait for ready failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EBUSY;
}


static int tpm_nsc_recv(struct tpm_chip *chip, u8 * buf, size_t count)
{
<<<<<<< HEAD
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *buffer = buf;
	u8 data, *p;
	u32 size;
	__be32 *native_size;

	if (count < 6)
		return -EIO;

	if (wait_for_stat(chip, NSC_STATUS_F0, NSC_STATUS_F0, &data) < 0) {
<<<<<<< HEAD
		dev_err(chip->dev, "F0 timeout\n");
		return -EIO;
	}
	if ((data =
	     inb(chip->vendor.base + NSC_DATA)) != NSC_COMMAND_NORMAL) {
		dev_err(chip->dev, "not in normal mode (0x%x)\n",
=======
		dev_err(&chip->dev, "F0 timeout\n");
		return -EIO;
	}

	data = inb(priv->base + NSC_DATA);
	if (data != NSC_COMMAND_NORMAL) {
		dev_err(&chip->dev, "not in normal mode (0x%x)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			data);
		return -EIO;
	}

	/* read the whole packet */
	for (p = buffer; p < &buffer[count]; p++) {
		if (wait_for_stat
		    (chip, NSC_STATUS_OBF, NSC_STATUS_OBF, &data) < 0) {
<<<<<<< HEAD
			dev_err(chip->dev,
=======
			dev_err(&chip->dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				"OBF timeout (while reading data)\n");
			return -EIO;
		}
		if (data & NSC_STATUS_F0)
			break;
<<<<<<< HEAD
		*p = inb(chip->vendor.base + NSC_DATA);
=======
		*p = inb(priv->base + NSC_DATA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if ((data & NSC_STATUS_F0) == 0 &&
	(wait_for_stat(chip, NSC_STATUS_F0, NSC_STATUS_F0, &data) < 0)) {
<<<<<<< HEAD
		dev_err(chip->dev, "F0 not set\n");
		return -EIO;
	}
	if ((data = inb(chip->vendor.base + NSC_DATA)) != NSC_COMMAND_EOC) {
		dev_err(chip->dev,
=======
		dev_err(&chip->dev, "F0 not set\n");
		return -EIO;
	}

	data = inb(priv->base + NSC_DATA);
	if (data != NSC_COMMAND_EOC) {
		dev_err(&chip->dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"expected end of command(0x%x)\n", data);
		return -EIO;
	}

	native_size = (__force __be32 *) (buf + 2);
	size = be32_to_cpu(*native_size);

	if (count < size)
		return -EIO;

	return size;
}

static int tpm_nsc_send(struct tpm_chip *chip, u8 * buf, size_t count)
{
<<<<<<< HEAD
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 data;
	int i;

	/*
	 * If we hit the chip with back to back commands it locks up
	 * and never set IBF. Hitting it with this "hammer" seems to
	 * fix it. Not sure why this is needed, we followed the flow
	 * chart in the manual to the letter.
	 */
<<<<<<< HEAD
	outb(NSC_COMMAND_CANCEL, chip->vendor.base + NSC_COMMAND);
=======
	outb(NSC_COMMAND_CANCEL, priv->base + NSC_COMMAND);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nsc_wait_for_ready(chip) != 0)
		return -EIO;

	if (wait_for_stat(chip, NSC_STATUS_IBF, 0, &data) < 0) {
<<<<<<< HEAD
		dev_err(chip->dev, "IBF timeout\n");
		return -EIO;
	}

	outb(NSC_COMMAND_NORMAL, chip->vendor.base + NSC_COMMAND);
	if (wait_for_stat(chip, NSC_STATUS_IBR, NSC_STATUS_IBR, &data) < 0) {
		dev_err(chip->dev, "IBR timeout\n");
=======
		dev_err(&chip->dev, "IBF timeout\n");
		return -EIO;
	}

	outb(NSC_COMMAND_NORMAL, priv->base + NSC_COMMAND);
	if (wait_for_stat(chip, NSC_STATUS_IBR, NSC_STATUS_IBR, &data) < 0) {
		dev_err(&chip->dev, "IBR timeout\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EIO;
	}

	for (i = 0; i < count; i++) {
		if (wait_for_stat(chip, NSC_STATUS_IBF, 0, &data) < 0) {
<<<<<<< HEAD
			dev_err(chip->dev,
				"IBF timeout (while writing data)\n");
			return -EIO;
		}
		outb(buf[i], chip->vendor.base + NSC_DATA);
	}

	if (wait_for_stat(chip, NSC_STATUS_IBF, 0, &data) < 0) {
		dev_err(chip->dev, "IBF timeout\n");
		return -EIO;
	}
	outb(NSC_COMMAND_EOC, chip->vendor.base + NSC_COMMAND);

	return count;
=======
			dev_err(&chip->dev,
				"IBF timeout (while writing data)\n");
			return -EIO;
		}
		outb(buf[i], priv->base + NSC_DATA);
	}

	if (wait_for_stat(chip, NSC_STATUS_IBF, 0, &data) < 0) {
		dev_err(&chip->dev, "IBF timeout\n");
		return -EIO;
	}
	outb(NSC_COMMAND_EOC, priv->base + NSC_COMMAND);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tpm_nsc_cancel(struct tpm_chip *chip)
{
<<<<<<< HEAD
	outb(NSC_COMMAND_CANCEL, chip->vendor.base + NSC_COMMAND);
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);

	outb(NSC_COMMAND_CANCEL, priv->base + NSC_COMMAND);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u8 tpm_nsc_status(struct tpm_chip *chip)
{
<<<<<<< HEAD
	return inb(chip->vendor.base + NSC_STATUS);
}

static const struct file_operations nsc_ops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = tpm_open,
	.read = tpm_read,
	.write = tpm_write,
	.release = tpm_release,
};

static DEVICE_ATTR(pubek, S_IRUGO, tpm_show_pubek, NULL);
static DEVICE_ATTR(pcrs, S_IRUGO, tpm_show_pcrs, NULL);
static DEVICE_ATTR(caps, S_IRUGO, tpm_show_caps, NULL);
static DEVICE_ATTR(cancel, S_IWUSR|S_IWGRP, NULL, tpm_store_cancel);

static struct attribute * nsc_attrs[] = {
	&dev_attr_pubek.attr,
	&dev_attr_pcrs.attr,
	&dev_attr_caps.attr,
	&dev_attr_cancel.attr,
	NULL,
};

static struct attribute_group nsc_attr_grp = { .attrs = nsc_attrs };

static const struct tpm_vendor_specific tpm_nsc = {
=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);

	return inb(priv->base + NSC_STATUS);
}

static bool tpm_nsc_req_canceled(struct tpm_chip *chip, u8 status)
{
	return (status == NSC_STATUS_RDY);
}

static const struct tpm_class_ops tpm_nsc = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.recv = tpm_nsc_recv,
	.send = tpm_nsc_send,
	.cancel = tpm_nsc_cancel,
	.status = tpm_nsc_status,
	.req_complete_mask = NSC_STATUS_OBF,
	.req_complete_val = NSC_STATUS_OBF,
<<<<<<< HEAD
	.req_canceled = NSC_STATUS_RDY,
	.attr_group = &nsc_attr_grp,
	.miscdev = { .fops = &nsc_ops, },
=======
	.req_canceled = tpm_nsc_req_canceled,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct platform_device *pdev = NULL;

static void tpm_nsc_remove(struct device *dev)
{
	struct tpm_chip *chip = dev_get_drvdata(dev);
<<<<<<< HEAD
	if ( chip ) {
		release_region(chip->vendor.base, 2);
		tpm_remove_hardware(chip->dev);
	}
}

static int tpm_nsc_suspend(struct platform_device *dev, pm_message_t msg)
{
	return tpm_pm_suspend(&dev->dev, msg);
}

static int tpm_nsc_resume(struct platform_device *dev)
{
	return tpm_pm_resume(&dev->dev);
}

static struct platform_driver nsc_drv = {
	.suspend         = tpm_nsc_suspend,
	.resume          = tpm_nsc_resume,
	.driver          = {
		.name    = "tpm_nsc",
		.owner   = THIS_MODULE,
	},
};

=======
	struct tpm_nsc_priv *priv = dev_get_drvdata(&chip->dev);

	tpm_chip_unregister(chip);
	release_region(priv->base, 2);
}

static SIMPLE_DEV_PM_OPS(tpm_nsc_pm, tpm_pm_suspend, tpm_pm_resume);

static struct platform_driver nsc_drv = {
	.driver          = {
		.name    = "tpm_nsc",
		.pm      = &tpm_nsc_pm,
	},
};

static inline int tpm_read_index(int base, int index)
{
	outb(index, base);
	return inb(base+1) & 0xFF;
}

static inline void tpm_write_index(int base, int index, int value)
{
	outb(index, base);
	outb(value & 0xFF, base+1);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init init_nsc(void)
{
	int rc = 0;
	int lo, hi, err;
	int nscAddrBase = TPM_ADDR;
	struct tpm_chip *chip;
	unsigned long base;
<<<<<<< HEAD
=======
	struct tpm_nsc_priv *priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* verify that it is a National part (SID) */
	if (tpm_read_index(TPM_ADDR, NSC_SID_INDEX) != 0xEF) {
		nscAddrBase = (tpm_read_index(TPM_SUPERIO_ADDR, 0x2C)<<8)|
			(tpm_read_index(TPM_SUPERIO_ADDR, 0x2B)&0xFE);
		if (tpm_read_index(nscAddrBase, NSC_SID_INDEX) != 0xF6)
			return -ENODEV;
	}

	err = platform_driver_register(&nsc_drv);
	if (err)
		return err;

	hi = tpm_read_index(nscAddrBase, TPM_NSC_BASE0_HI);
	lo = tpm_read_index(nscAddrBase, TPM_NSC_BASE0_LO);
	base = (hi<<8) | lo;

	/* enable the DPM module */
	tpm_write_index(nscAddrBase, NSC_LDC_INDEX, 0x01);

	pdev = platform_device_alloc("tpm_nscl0", -1);
	if (!pdev) {
		rc = -ENOMEM;
		goto err_unreg_drv;
	}

	pdev->num_resources = 0;
	pdev->dev.driver = &nsc_drv.driver;
	pdev->dev.release = tpm_nsc_remove;

	if ((rc = platform_device_add(pdev)) < 0)
		goto err_put_dev;

<<<<<<< HEAD
=======
	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		rc = -ENOMEM;
		goto err_del_dev;
	}

	priv->base = base;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (request_region(base, 2, "tpm_nsc0") == NULL ) {
		rc = -EBUSY;
		goto err_del_dev;
	}

<<<<<<< HEAD
	if (!(chip = tpm_register_hardware(&pdev->dev, &tpm_nsc))) {
=======
	chip = tpmm_chip_alloc(&pdev->dev, &tpm_nsc);
	if (IS_ERR(chip)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = -ENODEV;
		goto err_rel_reg;
	}

<<<<<<< HEAD
=======
	dev_set_drvdata(&chip->dev, priv);

	rc = tpm_chip_register(chip);
	if (rc)
		goto err_rel_reg;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev_dbg(&pdev->dev, "NSC TPM detected\n");
	dev_dbg(&pdev->dev,
		"NSC LDN 0x%x, SID 0x%x, SRID 0x%x\n",
		tpm_read_index(nscAddrBase,0x07), tpm_read_index(nscAddrBase,0x20),
		tpm_read_index(nscAddrBase,0x27));
	dev_dbg(&pdev->dev,
		"NSC SIOCF1 0x%x SIOCF5 0x%x SIOCF6 0x%x SIOCF8 0x%x\n",
		tpm_read_index(nscAddrBase,0x21), tpm_read_index(nscAddrBase,0x25),
		tpm_read_index(nscAddrBase,0x26), tpm_read_index(nscAddrBase,0x28));
	dev_dbg(&pdev->dev, "NSC IO Base0 0x%x\n",
		(tpm_read_index(nscAddrBase,0x60) << 8) | tpm_read_index(nscAddrBase,0x61));
	dev_dbg(&pdev->dev, "NSC IO Base1 0x%x\n",
		(tpm_read_index(nscAddrBase,0x62) << 8) | tpm_read_index(nscAddrBase,0x63));
	dev_dbg(&pdev->dev, "NSC Interrupt number and wakeup 0x%x\n",
		tpm_read_index(nscAddrBase,0x70));
	dev_dbg(&pdev->dev, "NSC IRQ type select 0x%x\n",
		tpm_read_index(nscAddrBase,0x71));
	dev_dbg(&pdev->dev,
		"NSC DMA channel select0 0x%x, select1 0x%x\n",
		tpm_read_index(nscAddrBase,0x74), tpm_read_index(nscAddrBase,0x75));
	dev_dbg(&pdev->dev,
		"NSC Config "
		"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		tpm_read_index(nscAddrBase,0xF0), tpm_read_index(nscAddrBase,0xF1),
		tpm_read_index(nscAddrBase,0xF2), tpm_read_index(nscAddrBase,0xF3),
		tpm_read_index(nscAddrBase,0xF4), tpm_read_index(nscAddrBase,0xF5),
		tpm_read_index(nscAddrBase,0xF6), tpm_read_index(nscAddrBase,0xF7),
		tpm_read_index(nscAddrBase,0xF8), tpm_read_index(nscAddrBase,0xF9));

	dev_info(&pdev->dev,
		 "NSC TPM revision %d\n",
		 tpm_read_index(nscAddrBase, 0x27) & 0x1F);

<<<<<<< HEAD
	chip->vendor.base = base;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

err_rel_reg:
	release_region(base, 2);
err_del_dev:
	platform_device_del(pdev);
err_put_dev:
	platform_device_put(pdev);
err_unreg_drv:
	platform_driver_unregister(&nsc_drv);
	return rc;
}

static void __exit cleanup_nsc(void)
{
	if (pdev) {
		tpm_nsc_remove(&pdev->dev);
		platform_device_unregister(pdev);
	}

	platform_driver_unregister(&nsc_drv);
}

module_init(init_nsc);
module_exit(cleanup_nsc);

<<<<<<< HEAD
MODULE_AUTHOR("Leendert van Doorn (leendert@watson.ibm.com)");
=======
MODULE_AUTHOR("Leendert van Doorn <leendert@watson.ibm.com>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("TPM Driver");
MODULE_VERSION("2.0");
MODULE_LICENSE("GPL");
