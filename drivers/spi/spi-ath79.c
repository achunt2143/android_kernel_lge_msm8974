<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SPI controller driver for the Atheros AR71XX/AR724X/AR913X SoCs
 *
 * Copyright (C) 2009-2011 Gabor Juhos <juhosg@openwrt.org>
 *
 * This driver has been based on the spi-gpio.c:
 *	Copyright (C) 2006,2008 David Brownell
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/bitops.h>
#include <linux/gpio.h>

#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79_spi_platform.h>

#define DRV_NAME	"ath79-spi"

=======
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi-mem.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/err.h>

#define DRV_NAME	"ath79-spi"

#define ATH79_SPI_RRW_DELAY_FACTOR	12000
#define MHZ				(1000 * 1000)

#define AR71XX_SPI_REG_FS		0x00	/* Function Select */
#define AR71XX_SPI_REG_CTRL		0x04	/* SPI Control */
#define AR71XX_SPI_REG_IOC		0x08	/* SPI I/O Control */
#define AR71XX_SPI_REG_RDS		0x0c	/* Read Data Shift */

#define AR71XX_SPI_FS_GPIO		BIT(0)	/* Enable GPIO mode */

#define AR71XX_SPI_IOC_DO		BIT(0)	/* Data Out pin */
#define AR71XX_SPI_IOC_CLK		BIT(8)	/* CLK pin */
#define AR71XX_SPI_IOC_CS(n)		BIT(16 + (n))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct ath79_spi {
	struct spi_bitbang	bitbang;
	u32			ioc_base;
	u32			reg_ctrl;
	void __iomem		*base;
<<<<<<< HEAD
};

static inline u32 ath79_spi_rr(struct ath79_spi *sp, unsigned reg)
=======
	struct clk		*clk;
	unsigned int		rrw_delay;
};

static inline u32 ath79_spi_rr(struct ath79_spi *sp, unsigned int reg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return ioread32(sp->base + reg);
}

<<<<<<< HEAD
static inline void ath79_spi_wr(struct ath79_spi *sp, unsigned reg, u32 val)
=======
static inline void ath79_spi_wr(struct ath79_spi *sp, unsigned int reg, u32 val)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	iowrite32(val, sp->base + reg);
}

static inline struct ath79_spi *ath79_spidev_to_sp(struct spi_device *spi)
{
<<<<<<< HEAD
	return spi_master_get_devdata(spi->master);
=======
	return spi_controller_get_devdata(spi->controller);
}

static inline void ath79_spi_delay(struct ath79_spi *sp, unsigned int nsecs)
{
	if (nsecs > sp->rrw_delay)
		ndelay(nsecs - sp->rrw_delay);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ath79_spi_chipselect(struct spi_device *spi, int is_active)
{
	struct ath79_spi *sp = ath79_spidev_to_sp(spi);
	int cs_high = (spi->mode & SPI_CS_HIGH) ? is_active : !is_active;
<<<<<<< HEAD

	if (is_active) {
		/* set initial clock polarity */
		if (spi->mode & SPI_CPOL)
			sp->ioc_base |= AR71XX_SPI_IOC_CLK;
		else
			sp->ioc_base &= ~AR71XX_SPI_IOC_CLK;

		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, sp->ioc_base);
	}

	if (spi->chip_select) {
		struct ath79_spi_controller_data *cdata = spi->controller_data;

		/* SPI is normally active-low */
		gpio_set_value(cdata->gpio, cs_high);
	} else {
		if (cs_high)
			sp->ioc_base |= AR71XX_SPI_IOC_CS0;
		else
			sp->ioc_base &= ~AR71XX_SPI_IOC_CS0;

		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, sp->ioc_base);
	}

}

static int ath79_spi_setup_cs(struct spi_device *spi)
{
	struct ath79_spi *sp = ath79_spidev_to_sp(spi);
	struct ath79_spi_controller_data *cdata;

	cdata = spi->controller_data;
	if (spi->chip_select && !cdata)
		return -EINVAL;

=======
	u32 cs_bit = AR71XX_SPI_IOC_CS(spi_get_chipselect(spi, 0));

	if (cs_high)
		sp->ioc_base |= cs_bit;
	else
		sp->ioc_base &= ~cs_bit;

	ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, sp->ioc_base);
}

static void ath79_spi_enable(struct ath79_spi *sp)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* enable GPIO mode */
	ath79_spi_wr(sp, AR71XX_SPI_REG_FS, AR71XX_SPI_FS_GPIO);

	/* save CTRL register */
	sp->reg_ctrl = ath79_spi_rr(sp, AR71XX_SPI_REG_CTRL);
	sp->ioc_base = ath79_spi_rr(sp, AR71XX_SPI_REG_IOC);

<<<<<<< HEAD
	/* TODO: setup speed? */
	ath79_spi_wr(sp, AR71XX_SPI_REG_CTRL, 0x43);

	if (spi->chip_select) {
		int status = 0;

		status = gpio_request(cdata->gpio, dev_name(&spi->dev));
		if (status)
			return status;

		status = gpio_direction_output(cdata->gpio,
					       spi->mode & SPI_CS_HIGH);
		if (status) {
			gpio_free(cdata->gpio);
			return status;
		}
	} else {
		if (spi->mode & SPI_CS_HIGH)
			sp->ioc_base |= AR71XX_SPI_IOC_CS0;
		else
			sp->ioc_base &= ~AR71XX_SPI_IOC_CS0;
		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, sp->ioc_base);
	}

	return 0;
}

static void ath79_spi_cleanup_cs(struct spi_device *spi)
{
	struct ath79_spi *sp = ath79_spidev_to_sp(spi);

	if (spi->chip_select) {
		struct ath79_spi_controller_data *cdata = spi->controller_data;
		gpio_free(cdata->gpio);
	}

=======
	/* clear clk and mosi in the base state */
	sp->ioc_base &= ~(AR71XX_SPI_IOC_DO | AR71XX_SPI_IOC_CLK);

	/* TODO: setup speed? */
	ath79_spi_wr(sp, AR71XX_SPI_REG_CTRL, 0x43);
}

static void ath79_spi_disable(struct ath79_spi *sp)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* restore CTRL register */
	ath79_spi_wr(sp, AR71XX_SPI_REG_CTRL, sp->reg_ctrl);
	/* disable GPIO mode */
	ath79_spi_wr(sp, AR71XX_SPI_REG_FS, 0);
}

<<<<<<< HEAD
static int ath79_spi_setup(struct spi_device *spi)
{
	int status = 0;

	if (spi->bits_per_word > 32)
		return -EINVAL;

	if (!spi->controller_state) {
		status = ath79_spi_setup_cs(spi);
		if (status)
			return status;
	}

	status = spi_bitbang_setup(spi);
	if (status && !spi->controller_state)
		ath79_spi_cleanup_cs(spi);

	return status;
}

static void ath79_spi_cleanup(struct spi_device *spi)
{
	ath79_spi_cleanup_cs(spi);
	spi_bitbang_cleanup(spi);
}

static u32 ath79_spi_txrx_mode0(struct spi_device *spi, unsigned nsecs,
			       u32 word, u8 bits)
=======
static u32 ath79_spi_txrx_mode0(struct spi_device *spi, unsigned int nsecs,
			       u32 word, u8 bits, unsigned flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath79_spi *sp = ath79_spidev_to_sp(spi);
	u32 ioc = sp->ioc_base;

	/* clock starts at inactive polarity */
	for (word <<= (32 - bits); likely(bits); bits--) {
		u32 out;

		if (word & (1 << 31))
			out = ioc | AR71XX_SPI_IOC_DO;
		else
			out = ioc & ~AR71XX_SPI_IOC_DO;

<<<<<<< HEAD
		/* setup MSB (to slave) on trailing edge */
		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, out);
		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, out | AR71XX_SPI_IOC_CLK);
=======
		/* setup MSB (to target) on trailing edge */
		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, out);
		ath79_spi_delay(sp, nsecs);
		ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, out | AR71XX_SPI_IOC_CLK);
		ath79_spi_delay(sp, nsecs);
		if (bits == 1)
			ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, out);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		word <<= 1;
	}

	return ath79_spi_rr(sp, AR71XX_SPI_REG_RDS);
}

<<<<<<< HEAD
static __devinit int ath79_spi_probe(struct platform_device *pdev)
{
	struct spi_master *master;
	struct ath79_spi *sp;
	struct ath79_spi_platform_data *pdata;
	struct resource	*r;
	int ret;

	master = spi_alloc_master(&pdev->dev, sizeof(*sp));
	if (master == NULL) {
		dev_err(&pdev->dev, "failed to allocate spi master\n");
		return -ENOMEM;
	}

	sp = spi_master_get_devdata(master);
	platform_set_drvdata(pdev, sp);

	pdata = pdev->dev.platform_data;

	master->setup = ath79_spi_setup;
	master->cleanup = ath79_spi_cleanup;
	if (pdata) {
		master->bus_num = pdata->bus_num;
		master->num_chipselect = pdata->num_chipselect;
	} else {
		master->bus_num = -1;
		master->num_chipselect = 1;
	}

	sp->bitbang.master = spi_master_get(master);
	sp->bitbang.chipselect = ath79_spi_chipselect;
	sp->bitbang.txrx_word[SPI_MODE_0] = ath79_spi_txrx_mode0;
	sp->bitbang.setup_transfer = spi_bitbang_setup_transfer;
	sp->bitbang.flags = SPI_CS_HIGH;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		ret = -ENOENT;
		goto err_put_master;
	}

	sp->base = ioremap(r->start, resource_size(r));
	if (!sp->base) {
		ret = -ENXIO;
		goto err_put_master;
	}

	ret = spi_bitbang_start(&sp->bitbang);
	if (ret)
		goto err_unmap;

	return 0;

err_unmap:
	iounmap(sp->base);
err_put_master:
	platform_set_drvdata(pdev, NULL);
	spi_master_put(sp->bitbang.master);
=======
static int ath79_exec_mem_op(struct spi_mem *mem,
			     const struct spi_mem_op *op)
{
	struct ath79_spi *sp = ath79_spidev_to_sp(mem->spi);

	/* Ensures that reading is performed on device connected to hardware cs0 */
	if (spi_get_chipselect(mem->spi, 0) || spi_get_csgpiod(mem->spi, 0))
		return -ENOTSUPP;

	/* Only use for fast-read op. */
	if (op->cmd.opcode != 0x0b || op->data.dir != SPI_MEM_DATA_IN ||
	    op->addr.nbytes != 3 || op->dummy.nbytes != 1)
		return -EOPNOTSUPP;

	/* disable GPIO mode */
	ath79_spi_wr(sp, AR71XX_SPI_REG_FS, 0);

	memcpy_fromio(op->data.buf.in, sp->base + op->addr.val, op->data.nbytes);

	/* enable GPIO mode */
	ath79_spi_wr(sp, AR71XX_SPI_REG_FS, AR71XX_SPI_FS_GPIO);

	/* restore IOC register */
	ath79_spi_wr(sp, AR71XX_SPI_REG_IOC, sp->ioc_base);

	return 0;
}

static const struct spi_controller_mem_ops ath79_mem_ops = {
	.exec_op = ath79_exec_mem_op,
};

static int ath79_spi_probe(struct platform_device *pdev)
{
	struct spi_controller *host;
	struct ath79_spi *sp;
	unsigned long rate;
	int ret;

	host = spi_alloc_host(&pdev->dev, sizeof(*sp));
	if (host == NULL) {
		dev_err(&pdev->dev, "failed to allocate spi host\n");
		return -ENOMEM;
	}

	sp = spi_controller_get_devdata(host);
	host->dev.of_node = pdev->dev.of_node;
	platform_set_drvdata(pdev, sp);

	host->use_gpio_descriptors = true;
	host->bits_per_word_mask = SPI_BPW_RANGE_MASK(1, 32);
	host->flags = SPI_CONTROLLER_GPIO_SS;
	host->num_chipselect = 3;
	host->mem_ops = &ath79_mem_ops;

	sp->bitbang.ctlr = host;
	sp->bitbang.chipselect = ath79_spi_chipselect;
	sp->bitbang.txrx_word[SPI_MODE_0] = ath79_spi_txrx_mode0;
	sp->bitbang.flags = SPI_CS_HIGH;

	sp->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(sp->base)) {
		ret = PTR_ERR(sp->base);
		goto err_put_host;
	}

	sp->clk = devm_clk_get_enabled(&pdev->dev, "ahb");
	if (IS_ERR(sp->clk)) {
		ret = PTR_ERR(sp->clk);
		goto err_put_host;
	}

	rate = DIV_ROUND_UP(clk_get_rate(sp->clk), MHZ);
	if (!rate) {
		ret = -EINVAL;
		goto err_put_host;
	}

	sp->rrw_delay = ATH79_SPI_RRW_DELAY_FACTOR / rate;
	dev_dbg(&pdev->dev, "register read/write delay is %u nsecs\n",
		sp->rrw_delay);

	ath79_spi_enable(sp);
	ret = spi_bitbang_start(&sp->bitbang);
	if (ret)
		goto err_disable;

	return 0;

err_disable:
	ath79_spi_disable(sp);
err_put_host:
	spi_controller_put(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static __devexit int ath79_spi_remove(struct platform_device *pdev)
=======
static void ath79_spi_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ath79_spi *sp = platform_get_drvdata(pdev);

	spi_bitbang_stop(&sp->bitbang);
<<<<<<< HEAD
	iounmap(sp->base);
	platform_set_drvdata(pdev, NULL);
	spi_master_put(sp->bitbang.master);

	return 0;
}

static struct platform_driver ath79_spi_driver = {
	.probe		= ath79_spi_probe,
	.remove		= __devexit_p(ath79_spi_remove),
	.driver		= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
=======
	ath79_spi_disable(sp);
	spi_controller_put(sp->bitbang.ctlr);
}

static void ath79_spi_shutdown(struct platform_device *pdev)
{
	ath79_spi_remove(pdev);
}

static const struct of_device_id ath79_spi_of_match[] = {
	{ .compatible = "qca,ar7100-spi", },
	{ },
};
MODULE_DEVICE_TABLE(of, ath79_spi_of_match);

static struct platform_driver ath79_spi_driver = {
	.probe		= ath79_spi_probe,
	.remove_new	= ath79_spi_remove,
	.shutdown	= ath79_spi_shutdown,
	.driver		= {
		.name	= DRV_NAME,
		.of_match_table = ath79_spi_of_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
module_platform_driver(ath79_spi_driver);

MODULE_DESCRIPTION("SPI controller driver for Atheros AR71XX/AR724X/AR913X");
MODULE_AUTHOR("Gabor Juhos <juhosg@openwrt.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRV_NAME);
