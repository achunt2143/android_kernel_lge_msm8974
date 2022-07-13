<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ST M48T86 / Dallas DS12887 RTC driver
 * Copyright (c) 2006 Tower Technologies
 *
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This drivers only supports the clock running in BCD and 24H mode.
 * If it will be ever adapted to binary and 12H mode, care must be taken
 * to not introduce bugs.
 */

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/m48t86.h>
#include <linux/bcd.h>

#define M48T86_REG_SEC		0x00
#define M48T86_REG_SECALRM	0x01
#define M48T86_REG_MIN		0x02
#define M48T86_REG_MINALRM	0x03
#define M48T86_REG_HOUR		0x04
#define M48T86_REG_HOURALRM	0x05
#define M48T86_REG_DOW		0x06 /* 1 = sunday */
#define M48T86_REG_DOM		0x07
#define M48T86_REG_MONTH	0x08 /* 1 - 12 */
#define M48T86_REG_YEAR		0x09 /* 0 - 99 */
#define M48T86_REG_A		0x0A
#define M48T86_REG_B		0x0B
#define M48T86_REG_C		0x0C
#define M48T86_REG_D		0x0D

#define M48T86_REG_B_H24	(1 << 1)
#define M48T86_REG_B_DM		(1 << 2)
#define M48T86_REG_B_SET	(1 << 7)
#define M48T86_REG_D_VRT	(1 << 7)

#define DRV_VERSION "0.1"

=======
#include <linux/mod_devicetable.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/bcd.h>
#include <linux/io.h>

#define M48T86_SEC		0x00
#define M48T86_SECALRM		0x01
#define M48T86_MIN		0x02
#define M48T86_MINALRM		0x03
#define M48T86_HOUR		0x04
#define M48T86_HOURALRM		0x05
#define M48T86_DOW		0x06 /* 1 = sunday */
#define M48T86_DOM		0x07
#define M48T86_MONTH		0x08 /* 1 - 12 */
#define M48T86_YEAR		0x09 /* 0 - 99 */
#define M48T86_A		0x0a
#define M48T86_B		0x0b
#define M48T86_B_SET		BIT(7)
#define M48T86_B_DM		BIT(2)
#define M48T86_B_H24		BIT(1)
#define M48T86_C		0x0c
#define M48T86_D		0x0d
#define M48T86_D_VRT		BIT(7)
#define M48T86_NVRAM(x)		(0x0e + (x))
#define M48T86_NVRAM_LEN	114

struct m48t86_rtc_info {
	void __iomem *index_reg;
	void __iomem *data_reg;
	struct rtc_device *rtc;
};

static unsigned char m48t86_readb(struct device *dev, unsigned long addr)
{
	struct m48t86_rtc_info *info = dev_get_drvdata(dev);
	unsigned char value;

	writeb(addr, info->index_reg);
	value = readb(info->data_reg);

	return value;
}

static void m48t86_writeb(struct device *dev,
			  unsigned char value, unsigned long addr)
{
	struct m48t86_rtc_info *info = dev_get_drvdata(dev);

	writeb(addr, info->index_reg);
	writeb(value, info->data_reg);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int m48t86_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char reg;
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct m48t86_ops *ops = pdev->dev.platform_data;

	reg = ops->readbyte(M48T86_REG_B);

	if (reg & M48T86_REG_B_DM) {
		/* data (binary) mode */
		tm->tm_sec	= ops->readbyte(M48T86_REG_SEC);
		tm->tm_min	= ops->readbyte(M48T86_REG_MIN);
		tm->tm_hour	= ops->readbyte(M48T86_REG_HOUR) & 0x3F;
		tm->tm_mday	= ops->readbyte(M48T86_REG_DOM);
		/* tm_mon is 0-11 */
		tm->tm_mon	= ops->readbyte(M48T86_REG_MONTH) - 1;
		tm->tm_year	= ops->readbyte(M48T86_REG_YEAR) + 100;
		tm->tm_wday	= ops->readbyte(M48T86_REG_DOW);
	} else {
		/* bcd mode */
		tm->tm_sec	= bcd2bin(ops->readbyte(M48T86_REG_SEC));
		tm->tm_min	= bcd2bin(ops->readbyte(M48T86_REG_MIN));
		tm->tm_hour	= bcd2bin(ops->readbyte(M48T86_REG_HOUR) & 0x3F);
		tm->tm_mday	= bcd2bin(ops->readbyte(M48T86_REG_DOM));
		/* tm_mon is 0-11 */
		tm->tm_mon	= bcd2bin(ops->readbyte(M48T86_REG_MONTH)) - 1;
		tm->tm_year	= bcd2bin(ops->readbyte(M48T86_REG_YEAR)) + 100;
		tm->tm_wday	= bcd2bin(ops->readbyte(M48T86_REG_DOW));
	}

	/* correct the hour if the clock is in 12h mode */
	if (!(reg & M48T86_REG_B_H24))
		if (ops->readbyte(M48T86_REG_HOUR) & 0x80)
			tm->tm_hour += 12;

	return rtc_valid_tm(tm);
=======

	reg = m48t86_readb(dev, M48T86_B);

	if (reg & M48T86_B_DM) {
		/* data (binary) mode */
		tm->tm_sec	= m48t86_readb(dev, M48T86_SEC);
		tm->tm_min	= m48t86_readb(dev, M48T86_MIN);
		tm->tm_hour	= m48t86_readb(dev, M48T86_HOUR) & 0x3f;
		tm->tm_mday	= m48t86_readb(dev, M48T86_DOM);
		/* tm_mon is 0-11 */
		tm->tm_mon	= m48t86_readb(dev, M48T86_MONTH) - 1;
		tm->tm_year	= m48t86_readb(dev, M48T86_YEAR) + 100;
		tm->tm_wday	= m48t86_readb(dev, M48T86_DOW);
	} else {
		/* bcd mode */
		tm->tm_sec	= bcd2bin(m48t86_readb(dev, M48T86_SEC));
		tm->tm_min	= bcd2bin(m48t86_readb(dev, M48T86_MIN));
		tm->tm_hour	= bcd2bin(m48t86_readb(dev, M48T86_HOUR) &
					  0x3f);
		tm->tm_mday	= bcd2bin(m48t86_readb(dev, M48T86_DOM));
		/* tm_mon is 0-11 */
		tm->tm_mon	= bcd2bin(m48t86_readb(dev, M48T86_MONTH)) - 1;
		tm->tm_year	= bcd2bin(m48t86_readb(dev, M48T86_YEAR)) + 100;
		tm->tm_wday	= bcd2bin(m48t86_readb(dev, M48T86_DOW));
	}

	/* correct the hour if the clock is in 12h mode */
	if (!(reg & M48T86_B_H24))
		if (m48t86_readb(dev, M48T86_HOUR) & 0x80)
			tm->tm_hour += 12;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int m48t86_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char reg;
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct m48t86_ops *ops = pdev->dev.platform_data;

	reg = ops->readbyte(M48T86_REG_B);

	/* update flag and 24h mode */
	reg |= M48T86_REG_B_SET | M48T86_REG_B_H24;
	ops->writebyte(reg, M48T86_REG_B);

	if (reg & M48T86_REG_B_DM) {
		/* data (binary) mode */
		ops->writebyte(tm->tm_sec, M48T86_REG_SEC);
		ops->writebyte(tm->tm_min, M48T86_REG_MIN);
		ops->writebyte(tm->tm_hour, M48T86_REG_HOUR);
		ops->writebyte(tm->tm_mday, M48T86_REG_DOM);
		ops->writebyte(tm->tm_mon + 1, M48T86_REG_MONTH);
		ops->writebyte(tm->tm_year % 100, M48T86_REG_YEAR);
		ops->writebyte(tm->tm_wday, M48T86_REG_DOW);
	} else {
		/* bcd mode */
		ops->writebyte(bin2bcd(tm->tm_sec), M48T86_REG_SEC);
		ops->writebyte(bin2bcd(tm->tm_min), M48T86_REG_MIN);
		ops->writebyte(bin2bcd(tm->tm_hour), M48T86_REG_HOUR);
		ops->writebyte(bin2bcd(tm->tm_mday), M48T86_REG_DOM);
		ops->writebyte(bin2bcd(tm->tm_mon + 1), M48T86_REG_MONTH);
		ops->writebyte(bin2bcd(tm->tm_year % 100), M48T86_REG_YEAR);
		ops->writebyte(bin2bcd(tm->tm_wday), M48T86_REG_DOW);
	}

	/* update ended */
	reg &= ~M48T86_REG_B_SET;
	ops->writebyte(reg, M48T86_REG_B);
=======

	reg = m48t86_readb(dev, M48T86_B);

	/* update flag and 24h mode */
	reg |= M48T86_B_SET | M48T86_B_H24;
	m48t86_writeb(dev, reg, M48T86_B);

	if (reg & M48T86_B_DM) {
		/* data (binary) mode */
		m48t86_writeb(dev, tm->tm_sec, M48T86_SEC);
		m48t86_writeb(dev, tm->tm_min, M48T86_MIN);
		m48t86_writeb(dev, tm->tm_hour, M48T86_HOUR);
		m48t86_writeb(dev, tm->tm_mday, M48T86_DOM);
		m48t86_writeb(dev, tm->tm_mon + 1, M48T86_MONTH);
		m48t86_writeb(dev, tm->tm_year % 100, M48T86_YEAR);
		m48t86_writeb(dev, tm->tm_wday, M48T86_DOW);
	} else {
		/* bcd mode */
		m48t86_writeb(dev, bin2bcd(tm->tm_sec), M48T86_SEC);
		m48t86_writeb(dev, bin2bcd(tm->tm_min), M48T86_MIN);
		m48t86_writeb(dev, bin2bcd(tm->tm_hour), M48T86_HOUR);
		m48t86_writeb(dev, bin2bcd(tm->tm_mday), M48T86_DOM);
		m48t86_writeb(dev, bin2bcd(tm->tm_mon + 1), M48T86_MONTH);
		m48t86_writeb(dev, bin2bcd(tm->tm_year % 100), M48T86_YEAR);
		m48t86_writeb(dev, bin2bcd(tm->tm_wday), M48T86_DOW);
	}

	/* update ended */
	reg &= ~M48T86_B_SET;
	m48t86_writeb(dev, reg, M48T86_B);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int m48t86_rtc_proc(struct device *dev, struct seq_file *seq)
{
	unsigned char reg;
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct m48t86_ops *ops = pdev->dev.platform_data;

	reg = ops->readbyte(M48T86_REG_B);

	seq_printf(seq, "mode\t\t: %s\n",
		 (reg & M48T86_REG_B_DM) ? "binary" : "bcd");

	reg = ops->readbyte(M48T86_REG_D);

	seq_printf(seq, "battery\t\t: %s\n",
		 (reg & M48T86_REG_D_VRT) ? "ok" : "exhausted");
=======

	reg = m48t86_readb(dev, M48T86_B);

	seq_printf(seq, "mode\t\t: %s\n",
		   (reg & M48T86_B_DM) ? "binary" : "bcd");

	reg = m48t86_readb(dev, M48T86_D);

	seq_printf(seq, "battery\t\t: %s\n",
		   (reg & M48T86_D_VRT) ? "ok" : "exhausted");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct rtc_class_ops m48t86_rtc_ops = {
	.read_time	= m48t86_rtc_read_time,
	.set_time	= m48t86_rtc_set_time,
	.proc		= m48t86_rtc_proc,
};

<<<<<<< HEAD
static int __devinit m48t86_rtc_probe(struct platform_device *dev)
{
	unsigned char reg;
	struct m48t86_ops *ops = dev->dev.platform_data;
	struct rtc_device *rtc = rtc_device_register("m48t86",
				&dev->dev, &m48t86_rtc_ops, THIS_MODULE);

	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(dev, rtc);

	/* read battery status */
	reg = ops->readbyte(M48T86_REG_D);
	dev_info(&dev->dev, "battery %s\n",
		(reg & M48T86_REG_D_VRT) ? "ok" : "exhausted");
=======
static int m48t86_nvram_read(void *priv, unsigned int off, void *buf,
			     size_t count)
{
	struct device *dev = priv;
	unsigned int i;

	for (i = 0; i < count; i++)
		((u8 *)buf)[i] = m48t86_readb(dev, M48T86_NVRAM(off + i));

	return 0;
}

static int m48t86_nvram_write(void *priv, unsigned int off, void *buf,
			      size_t count)
{
	struct device *dev = priv;
	unsigned int i;

	for (i = 0; i < count; i++)
		m48t86_writeb(dev, ((u8 *)buf)[i], M48T86_NVRAM(off + i));

	return 0;
}

/*
 * The RTC is an optional feature at purchase time on some Technologic Systems
 * boards. Verify that it actually exists by checking if the last two bytes
 * of the NVRAM can be changed.
 *
 * This is based on the method used in their rtc7800.c example.
 */
static bool m48t86_verify_chip(struct platform_device *pdev)
{
	unsigned int offset0 = M48T86_NVRAM(M48T86_NVRAM_LEN - 2);
	unsigned int offset1 = M48T86_NVRAM(M48T86_NVRAM_LEN - 1);
	unsigned char tmp0, tmp1;

	tmp0 = m48t86_readb(&pdev->dev, offset0);
	tmp1 = m48t86_readb(&pdev->dev, offset1);

	m48t86_writeb(&pdev->dev, 0x00, offset0);
	m48t86_writeb(&pdev->dev, 0x55, offset1);
	if (m48t86_readb(&pdev->dev, offset1) == 0x55) {
		m48t86_writeb(&pdev->dev, 0xaa, offset1);
		if (m48t86_readb(&pdev->dev, offset1) == 0xaa &&
		    m48t86_readb(&pdev->dev, offset0) == 0x00) {
			m48t86_writeb(&pdev->dev, tmp0, offset0);
			m48t86_writeb(&pdev->dev, tmp1, offset1);

			return true;
		}
	}
	return false;
}

static int m48t86_rtc_probe(struct platform_device *pdev)
{
	struct m48t86_rtc_info *info;
	unsigned char reg;
	int err;
	struct nvmem_config m48t86_nvmem_cfg = {
		.name = "m48t86_nvram",
		.word_size = 1,
		.stride = 1,
		.size = M48T86_NVRAM_LEN,
		.reg_read = m48t86_nvram_read,
		.reg_write = m48t86_nvram_write,
		.priv = &pdev->dev,
	};

	info = devm_kzalloc(&pdev->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->index_reg = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(info->index_reg))
		return PTR_ERR(info->index_reg);

	info->data_reg = devm_platform_ioremap_resource(pdev, 1);
	if (IS_ERR(info->data_reg))
		return PTR_ERR(info->data_reg);

	dev_set_drvdata(&pdev->dev, info);

	if (!m48t86_verify_chip(pdev)) {
		dev_info(&pdev->dev, "RTC not present\n");
		return -ENODEV;
	}

	info->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(info->rtc))
		return PTR_ERR(info->rtc);

	info->rtc->ops = &m48t86_rtc_ops;

	err = devm_rtc_register_device(info->rtc);
	if (err)
		return err;

	devm_rtc_nvmem_register(info->rtc, &m48t86_nvmem_cfg);

	/* read battery status */
	reg = m48t86_readb(&pdev->dev, M48T86_D);
	dev_info(&pdev->dev, "battery %s\n",
		 (reg & M48T86_D_VRT) ? "ok" : "exhausted");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int __devexit m48t86_rtc_remove(struct platform_device *dev)
{
	struct rtc_device *rtc = platform_get_drvdata(dev);

 	if (rtc)
		rtc_device_unregister(rtc);

	platform_set_drvdata(dev, NULL);

	return 0;
}
=======
static const struct of_device_id m48t86_rtc_of_ids[] = {
	{ .compatible = "st,m48t86" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, m48t86_rtc_of_ids);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct platform_driver m48t86_rtc_platform_driver = {
	.driver		= {
		.name	= "rtc-m48t86",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe		= m48t86_rtc_probe,
	.remove		= __devexit_p(m48t86_rtc_remove),
=======
		.of_match_table = m48t86_rtc_of_ids,
	},
	.probe		= m48t86_rtc_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(m48t86_rtc_platform_driver);

MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_DESCRIPTION("M48T86 RTC driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION(DRV_VERSION);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:rtc-m48t86");
