<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SH SCI SPI interface
 *
 * Copyright (c) 2008 Magnus Damm
 *
 * Based on S3C24XX GPIO based SPI driver, which is:
 *   Copyright (c) 2006 Ben Dooks
 *   Copyright (c) 2006 Simtec Electronics
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
=======
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>

#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/module.h>

#include <asm/spi.h>
#include <asm/io.h>

struct sh_sci_spi {
	struct spi_bitbang bitbang;

	void __iomem *membase;
	unsigned char val;
	struct sh_spi_info *info;
	struct platform_device *dev;
};

#define SCSPTR(sp)	(sp->membase + 0x1c)
#define PIN_SCK		(1 << 2)
#define PIN_TXD		(1 << 0)
#define PIN_RXD		PIN_TXD
#define PIN_INIT	((1 << 1) | (1 << 3) | PIN_SCK | PIN_TXD)

static inline void setbits(struct sh_sci_spi *sp, int bits, int on)
{
	/*
	 * We are the only user of SCSPTR so no locking is required.
	 * Reading bit 2 and 0 in SCSPTR gives pin state as input.
	 * Writing the same bits sets the output value.
	 * This makes regular read-modify-write difficult so we
	 * use sp->val to keep track of the latest register value.
	 */

	if (on)
		sp->val |= bits;
	else
		sp->val &= ~bits;

	iowrite8(sp->val, SCSPTR(sp));
}

static inline void setsck(struct spi_device *dev, int on)
{
<<<<<<< HEAD
	setbits(spi_master_get_devdata(dev->master), PIN_SCK, on);
=======
	setbits(spi_controller_get_devdata(dev->controller), PIN_SCK, on);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void setmosi(struct spi_device *dev, int on)
{
<<<<<<< HEAD
	setbits(spi_master_get_devdata(dev->master), PIN_TXD, on);
=======
	setbits(spi_controller_get_devdata(dev->controller), PIN_TXD, on);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline u32 getmiso(struct spi_device *dev)
{
<<<<<<< HEAD
	struct sh_sci_spi *sp = spi_master_get_devdata(dev->master);
=======
	struct sh_sci_spi *sp = spi_controller_get_devdata(dev->controller);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (ioread8(SCSPTR(sp)) & PIN_RXD) ? 1 : 0;
}

#define spidelay(x) ndelay(x)

#include "spi-bitbang-txrx.h"

static u32 sh_sci_spi_txrx_mode0(struct spi_device *spi,
<<<<<<< HEAD
				      unsigned nsecs, u32 word, u8 bits)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 0, 0, word, bits);
}

static u32 sh_sci_spi_txrx_mode1(struct spi_device *spi,
				      unsigned nsecs, u32 word, u8 bits)
{
	return bitbang_txrx_be_cpha1(spi, nsecs, 0, 0, word, bits);
}

static u32 sh_sci_spi_txrx_mode2(struct spi_device *spi,
				      unsigned nsecs, u32 word, u8 bits)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 1, 0, word, bits);
}

static u32 sh_sci_spi_txrx_mode3(struct spi_device *spi,
				      unsigned nsecs, u32 word, u8 bits)
{
	return bitbang_txrx_be_cpha1(spi, nsecs, 1, 0, word, bits);
=======
				 unsigned nsecs, u32 word, u8 bits,
				 unsigned flags)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 0, flags, word, bits);
}

static u32 sh_sci_spi_txrx_mode1(struct spi_device *spi,
				 unsigned nsecs, u32 word, u8 bits,
				 unsigned flags)
{
	return bitbang_txrx_be_cpha1(spi, nsecs, 0, flags, word, bits);
}

static u32 sh_sci_spi_txrx_mode2(struct spi_device *spi,
				 unsigned nsecs, u32 word, u8 bits,
				 unsigned flags)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 1, flags, word, bits);
}

static u32 sh_sci_spi_txrx_mode3(struct spi_device *spi,
				 unsigned nsecs, u32 word, u8 bits,
				 unsigned flags)
{
	return bitbang_txrx_be_cpha1(spi, nsecs, 1, flags, word, bits);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void sh_sci_spi_chipselect(struct spi_device *dev, int value)
{
<<<<<<< HEAD
	struct sh_sci_spi *sp = spi_master_get_devdata(dev->master);

	if (sp->info && sp->info->chip_select)
		(sp->info->chip_select)(sp->info, dev->chip_select, value);
=======
	struct sh_sci_spi *sp = spi_controller_get_devdata(dev->controller);

	if (sp->info->chip_select)
		(sp->info->chip_select)(sp->info, spi_get_chipselect(dev, 0), value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int sh_sci_spi_probe(struct platform_device *dev)
{
	struct resource	*r;
<<<<<<< HEAD
	struct spi_master *master;
	struct sh_sci_spi *sp;
	int ret;

	master = spi_alloc_master(&dev->dev, sizeof(struct sh_sci_spi));
	if (master == NULL) {
		dev_err(&dev->dev, "failed to allocate spi master\n");
=======
	struct spi_controller *host;
	struct sh_sci_spi *sp;
	int ret;

	host = spi_alloc_host(&dev->dev, sizeof(struct sh_sci_spi));
	if (host == NULL) {
		dev_err(&dev->dev, "failed to allocate spi host\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENOMEM;
		goto err0;
	}

<<<<<<< HEAD
	sp = spi_master_get_devdata(master);

	platform_set_drvdata(dev, sp);
	sp->info = dev->dev.platform_data;

	/* setup spi bitbang adaptor */
	sp->bitbang.master = spi_master_get(master);
	sp->bitbang.master->bus_num = sp->info->bus_num;
	sp->bitbang.master->num_chipselect = sp->info->num_chipselect;
=======
	sp = spi_controller_get_devdata(host);

	platform_set_drvdata(dev, sp);
	sp->info = dev_get_platdata(&dev->dev);
	if (!sp->info) {
		dev_err(&dev->dev, "platform data is missing\n");
		ret = -ENOENT;
		goto err1;
	}

	/* setup spi bitbang adaptor */
	sp->bitbang.ctlr = host;
	sp->bitbang.ctlr->bus_num = sp->info->bus_num;
	sp->bitbang.ctlr->num_chipselect = sp->info->num_chipselect;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sp->bitbang.chipselect = sh_sci_spi_chipselect;

	sp->bitbang.txrx_word[SPI_MODE_0] = sh_sci_spi_txrx_mode0;
	sp->bitbang.txrx_word[SPI_MODE_1] = sh_sci_spi_txrx_mode1;
	sp->bitbang.txrx_word[SPI_MODE_2] = sh_sci_spi_txrx_mode2;
	sp->bitbang.txrx_word[SPI_MODE_3] = sh_sci_spi_txrx_mode3;

	r = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		ret = -ENOENT;
		goto err1;
	}
	sp->membase = ioremap(r->start, resource_size(r));
	if (!sp->membase) {
		ret = -ENXIO;
		goto err1;
	}
	sp->val = ioread8(SCSPTR(sp));
	setbits(sp, PIN_INIT, 1);

	ret = spi_bitbang_start(&sp->bitbang);
	if (!ret)
		return 0;

	setbits(sp, PIN_INIT, 0);
	iounmap(sp->membase);
 err1:
<<<<<<< HEAD
	spi_master_put(sp->bitbang.master);
=======
	spi_controller_put(sp->bitbang.ctlr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 err0:
	return ret;
}

<<<<<<< HEAD
static int sh_sci_spi_remove(struct platform_device *dev)
{
	struct sh_sci_spi *sp = platform_get_drvdata(dev);

	iounmap(sp->membase);
	setbits(sp, PIN_INIT, 0);
	spi_bitbang_stop(&sp->bitbang);
	spi_master_put(sp->bitbang.master);
	return 0;
=======
static void sh_sci_spi_remove(struct platform_device *dev)
{
	struct sh_sci_spi *sp = platform_get_drvdata(dev);

	spi_bitbang_stop(&sp->bitbang);
	setbits(sp, PIN_INIT, 0);
	iounmap(sp->membase);
	spi_controller_put(sp->bitbang.ctlr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver sh_sci_spi_drv = {
	.probe		= sh_sci_spi_probe,
<<<<<<< HEAD
	.remove		= sh_sci_spi_remove,
	.driver		= {
		.name	= "spi_sh_sci",
		.owner	= THIS_MODULE,
=======
	.remove_new	= sh_sci_spi_remove,
	.driver		= {
		.name	= "spi_sh_sci",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
module_platform_driver(sh_sci_spi_drv);

MODULE_DESCRIPTION("SH SCI SPI Driver");
MODULE_AUTHOR("Magnus Damm <damm@opensource.se>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:spi_sh_sci");
