/*
 * Freescale iMX PATA driver
 *
 * Copyright (C) 2011 Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 * Based on pata_platform - Copyright (C) 2006 - 2007  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * TODO:
 * - dmaengine support
<<<<<<< HEAD
 * - check if timing stuff needed
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <scsi/scsi_host.h>
#include <linux/ata.h>
#include <linux/libata.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#define DRV_NAME "pata_imx"

=======
 */

#include <linux/ata.h>
#include <linux/clk.h>
#include <linux/libata.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>

#define DRV_NAME "pata_imx"

#define PATA_IMX_ATA_TIME_OFF		0x00
#define PATA_IMX_ATA_TIME_ON		0x01
#define PATA_IMX_ATA_TIME_1		0x02
#define PATA_IMX_ATA_TIME_2W		0x03
#define PATA_IMX_ATA_TIME_2R		0x04
#define PATA_IMX_ATA_TIME_AX		0x05
#define PATA_IMX_ATA_TIME_PIO_RDX	0x06
#define PATA_IMX_ATA_TIME_4		0x07
#define PATA_IMX_ATA_TIME_9		0x08

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PATA_IMX_ATA_CONTROL		0x24
#define PATA_IMX_ATA_CTRL_FIFO_RST_B	(1<<7)
#define PATA_IMX_ATA_CTRL_ATA_RST_B	(1<<6)
#define PATA_IMX_ATA_CTRL_IORDY_EN	(1<<0)
#define PATA_IMX_ATA_INT_EN		0x2C
#define PATA_IMX_ATA_INTR_ATA_INTRQ2	(1<<3)
#define PATA_IMX_DRIVE_DATA		0xA0
#define PATA_IMX_DRIVE_CONTROL		0xD8

<<<<<<< HEAD
struct pata_imx_priv {
	struct clk *clk;
	/* timings/interrupt/control regs */
	u8 *host_regs;
	u32 ata_ctl;
};

static int pata_imx_set_mode(struct ata_link *link, struct ata_device **unused)
{
	struct ata_device *dev;
	struct ata_port *ap = link->ap;
	struct pata_imx_priv *priv = ap->host->private_data;
	u32 val;

	ata_for_each_dev(dev, link, ENABLED) {
		dev->pio_mode = dev->xfer_mode = XFER_PIO_0;
		dev->xfer_shift = ATA_SHIFT_PIO;
		dev->flags |= ATA_DFLAG_PIO;

		val = __raw_readl(priv->host_regs + PATA_IMX_ATA_CONTROL);
		if (ata_pio_need_iordy(dev))
			val |= PATA_IMX_ATA_CTRL_IORDY_EN;
		else
			val &= ~PATA_IMX_ATA_CTRL_IORDY_EN;
		__raw_writel(val, priv->host_regs + PATA_IMX_ATA_CONTROL);

		ata_dev_printk(dev, KERN_INFO, "configured for PIO\n");
	}
	return 0;
}

static struct scsi_host_template pata_imx_sht = {
=======
static u32 pio_t4[] = { 30,  20,  15,  10,  10 };
static u32 pio_t9[] = { 20,  15,  10,  10,  10 };
static u32 pio_tA[] = { 35,  35,  35,  35,  35 };

struct pata_imx_priv {
	struct clk *clk;
	/* timings/interrupt/control regs */
	void __iomem *host_regs;
	u32 ata_ctl;
};

static void pata_imx_set_timing(struct ata_device *adev,
				struct pata_imx_priv *priv)
{
	struct ata_timing timing;
	unsigned long clkrate;
	u32 T, mode;

	clkrate = clk_get_rate(priv->clk);

	if (adev->pio_mode < XFER_PIO_0 || adev->pio_mode > XFER_PIO_4 ||
	    !clkrate)
		return;

	T = 1000000000 / clkrate;
	ata_timing_compute(adev, adev->pio_mode, &timing, T * 1000, 0);

	mode = adev->pio_mode - XFER_PIO_0;

	writeb(3, priv->host_regs + PATA_IMX_ATA_TIME_OFF);
	writeb(3, priv->host_regs + PATA_IMX_ATA_TIME_ON);
	writeb(timing.setup, priv->host_regs + PATA_IMX_ATA_TIME_1);
	writeb(timing.act8b, priv->host_regs + PATA_IMX_ATA_TIME_2W);
	writeb(timing.act8b, priv->host_regs + PATA_IMX_ATA_TIME_2R);
	writeb(1, priv->host_regs + PATA_IMX_ATA_TIME_PIO_RDX);

	writeb(pio_t4[mode] / T + 1, priv->host_regs + PATA_IMX_ATA_TIME_4);
	writeb(pio_t9[mode] / T + 1, priv->host_regs + PATA_IMX_ATA_TIME_9);
	writeb(pio_tA[mode] / T + 1, priv->host_regs + PATA_IMX_ATA_TIME_AX);
}

static void pata_imx_set_piomode(struct ata_port *ap, struct ata_device *adev)
{
	struct pata_imx_priv *priv = ap->host->private_data;
	u32 val;

	pata_imx_set_timing(adev, priv);

	val = __raw_readl(priv->host_regs + PATA_IMX_ATA_CONTROL);
	if (ata_pio_need_iordy(adev))
		val |= PATA_IMX_ATA_CTRL_IORDY_EN;
	else
		val &= ~PATA_IMX_ATA_CTRL_IORDY_EN;
	__raw_writel(val, priv->host_regs + PATA_IMX_ATA_CONTROL);
}

static const struct scsi_host_template pata_imx_sht = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ATA_PIO_SHT(DRV_NAME),
};

static struct ata_port_operations pata_imx_port_ops = {
	.inherits		= &ata_sff_port_ops,
<<<<<<< HEAD
	.sff_data_xfer		= ata_sff_data_xfer_noirq,
	.cable_detect		= ata_cable_unknown,
	.set_mode		= pata_imx_set_mode,
=======
	.sff_data_xfer		= ata_sff_data_xfer32,
	.cable_detect		= ata_cable_unknown,
	.set_piomode		= pata_imx_set_piomode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void pata_imx_setup_port(struct ata_ioports *ioaddr)
{
	/* Fixup the port shift for platforms that need it */
	ioaddr->data_addr	= ioaddr->cmd_addr + (ATA_REG_DATA    << 2);
	ioaddr->error_addr	= ioaddr->cmd_addr + (ATA_REG_ERR     << 2);
	ioaddr->feature_addr	= ioaddr->cmd_addr + (ATA_REG_FEATURE << 2);
	ioaddr->nsect_addr	= ioaddr->cmd_addr + (ATA_REG_NSECT   << 2);
	ioaddr->lbal_addr	= ioaddr->cmd_addr + (ATA_REG_LBAL    << 2);
	ioaddr->lbam_addr	= ioaddr->cmd_addr + (ATA_REG_LBAM    << 2);
	ioaddr->lbah_addr	= ioaddr->cmd_addr + (ATA_REG_LBAH    << 2);
	ioaddr->device_addr	= ioaddr->cmd_addr + (ATA_REG_DEVICE  << 2);
	ioaddr->status_addr	= ioaddr->cmd_addr + (ATA_REG_STATUS  << 2);
	ioaddr->command_addr	= ioaddr->cmd_addr + (ATA_REG_CMD     << 2);
}

<<<<<<< HEAD
static int __devinit pata_imx_probe(struct platform_device *pdev)
=======
static int pata_imx_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ata_host *host;
	struct ata_port *ap;
	struct pata_imx_priv *priv;
	int irq = 0;
	struct resource *io_res;
<<<<<<< HEAD

	io_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (io_res == NULL)
		return -EINVAL;

	irq = platform_get_irq(pdev, 0);
	if (irq <= 0)
		return -EINVAL;
=======
	int ret;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	priv = devm_kzalloc(&pdev->dev,
				sizeof(struct pata_imx_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
	priv->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "Failed to get clock\n");
		return PTR_ERR(priv->clk);
	}

	clk_enable(priv->clk);

	host = ata_host_alloc(&pdev->dev, 1);
	if (!host)
		goto free_priv;
=======
	priv->clk = devm_clk_get_enabled(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "Failed to get and enable clock\n");
		return PTR_ERR(priv->clk);
	}

	host = ata_host_alloc(&pdev->dev, 1);
	if (!host)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	host->private_data = priv;
	ap = host->ports[0];

	ap->ops = &pata_imx_port_ops;
<<<<<<< HEAD
	ap->pio_mask = ATA_PIO0;
	ap->flags |= ATA_FLAG_SLAVE_POSS;

	priv->host_regs = devm_ioremap(&pdev->dev, io_res->start,
		resource_size(io_res));
	if (!priv->host_regs) {
		dev_err(&pdev->dev, "failed to map IO/CTL base\n");
		goto free_priv;
	}
=======
	ap->pio_mask = ATA_PIO4;
	ap->flags |= ATA_FLAG_SLAVE_POSS;

	priv->host_regs = devm_platform_get_and_ioremap_resource(pdev, 0, &io_res);
	if (IS_ERR(priv->host_regs))
		return PTR_ERR(priv->host_regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ap->ioaddr.cmd_addr = priv->host_regs + PATA_IMX_DRIVE_DATA;
	ap->ioaddr.ctl_addr = priv->host_regs + PATA_IMX_DRIVE_CONTROL;

	ap->ioaddr.altstatus_addr = ap->ioaddr.ctl_addr;

	pata_imx_setup_port(&ap->ioaddr);

	ata_port_desc(ap, "cmd 0x%llx ctl 0x%llx",
		(unsigned long long)io_res->start + PATA_IMX_DRIVE_DATA,
		(unsigned long long)io_res->start + PATA_IMX_DRIVE_CONTROL);

	/* deassert resets */
	__raw_writel(PATA_IMX_ATA_CTRL_FIFO_RST_B |
			PATA_IMX_ATA_CTRL_ATA_RST_B,
			priv->host_regs + PATA_IMX_ATA_CONTROL);
	/* enable interrupts */
	__raw_writel(PATA_IMX_ATA_INTR_ATA_INTRQ2,
			priv->host_regs + PATA_IMX_ATA_INT_EN);

	/* activate */
<<<<<<< HEAD
	return ata_host_activate(host, irq, ata_sff_interrupt, 0,
				&pata_imx_sht);

free_priv:
	clk_disable(priv->clk);
	clk_put(priv->clk);
	return -ENOMEM;
}

static int __devexit pata_imx_remove(struct platform_device *pdev)
{
	struct ata_host *host = dev_get_drvdata(&pdev->dev);
=======
	ret = ata_host_activate(host, irq, ata_sff_interrupt, 0,
				&pata_imx_sht);

	if (ret)
		return ret;

	return 0;
}

static void pata_imx_remove(struct platform_device *pdev)
{
	struct ata_host *host = platform_get_drvdata(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pata_imx_priv *priv = host->private_data;

	ata_host_detach(host);

	__raw_writel(0, priv->host_regs + PATA_IMX_ATA_INT_EN);
<<<<<<< HEAD

	clk_disable(priv->clk);
	clk_put(priv->clk);

	return 0;
}

#ifdef CONFIG_PM
=======
}

#ifdef CONFIG_PM_SLEEP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pata_imx_suspend(struct device *dev)
{
	struct ata_host *host = dev_get_drvdata(dev);
	struct pata_imx_priv *priv = host->private_data;
<<<<<<< HEAD
	int ret;

	ret = ata_host_suspend(host, PMSG_SUSPEND);
	if (!ret) {
		__raw_writel(0, priv->host_regs + PATA_IMX_ATA_INT_EN);
		priv->ata_ctl =
			__raw_readl(priv->host_regs + PATA_IMX_ATA_CONTROL);
		clk_disable(priv->clk);
	}

	return ret;
=======

	ata_host_suspend(host, PMSG_SUSPEND);

	__raw_writel(0, priv->host_regs + PATA_IMX_ATA_INT_EN);
	priv->ata_ctl = __raw_readl(priv->host_regs + PATA_IMX_ATA_CONTROL);
	clk_disable_unprepare(priv->clk);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pata_imx_resume(struct device *dev)
{
	struct ata_host *host = dev_get_drvdata(dev);
	struct pata_imx_priv *priv = host->private_data;

<<<<<<< HEAD
	clk_enable(priv->clk);
=======
	int ret = clk_prepare_enable(priv->clk);
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__raw_writel(priv->ata_ctl, priv->host_regs + PATA_IMX_ATA_CONTROL);

	__raw_writel(PATA_IMX_ATA_INTR_ATA_INTRQ2,
			priv->host_regs + PATA_IMX_ATA_INT_EN);

	ata_host_resume(host);

	return 0;
}
<<<<<<< HEAD

static const struct dev_pm_ops pata_imx_pm_ops = {
	.suspend	= pata_imx_suspend,
	.resume		= pata_imx_resume,
};
#endif

static struct platform_driver pata_imx_driver = {
	.probe		= pata_imx_probe,
	.remove		= __devexit_p(pata_imx_remove),
	.driver = {
		.name		= DRV_NAME,
		.owner		= THIS_MODULE,
#ifdef CONFIG_PM
		.pm		= &pata_imx_pm_ops,
#endif
=======
#endif

static SIMPLE_DEV_PM_OPS(pata_imx_pm_ops, pata_imx_suspend, pata_imx_resume);

static const struct of_device_id imx_pata_dt_ids[] = {
	{
		.compatible = "fsl,imx27-pata",
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, imx_pata_dt_ids);

static struct platform_driver pata_imx_driver = {
	.probe		= pata_imx_probe,
	.remove_new	= pata_imx_remove,
	.driver = {
		.name		= DRV_NAME,
		.of_match_table	= imx_pata_dt_ids,
		.pm		= &pata_imx_pm_ops,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(pata_imx_driver);

MODULE_AUTHOR("Arnaud Patard <arnaud.patard@rtp-net.org>");
MODULE_DESCRIPTION("low-level driver for iMX PATA");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
