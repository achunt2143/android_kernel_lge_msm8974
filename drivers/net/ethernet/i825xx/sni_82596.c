<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sni_82596.c -- driver for intel 82596 ethernet controller, as
 *  		  used in older SNI RM machines
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>

#define SNI_82596_DRIVER_VERSION "SNI RM 82596 driver - Revision: 0.01"

static const char sni_82596_string[] = "snirm_82596";

<<<<<<< HEAD
#define DMA_ALLOC                      dma_alloc_coherent
#define DMA_FREE                       dma_free_coherent
#define DMA_WBACK(priv, addr, len)     do { } while (0)
#define DMA_INV(priv, addr, len)       do { } while (0)
#define DMA_WBACK_INV(priv, addr, len) do { } while (0)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SYSBUS      0x00004400

/* big endian CPU, 82596 little endian */
#define SWAP32(x)   cpu_to_le32((u32)(x))
#define SWAP16(x)   cpu_to_le16((u16)(x))

#define OPT_MPU_16BIT    0x01

#include "lib82596.c"

MODULE_AUTHOR("Thomas Bogendoerfer");
MODULE_DESCRIPTION("i82596 driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:snirm_82596");
module_param(i596_debug, int, 0);
MODULE_PARM_DESC(i596_debug, "82596 debug mask");

static inline void ca(struct net_device *dev)
{
	struct i596_private *lp = netdev_priv(dev);

	writel(0, lp->ca);
}


static void mpu_port(struct net_device *dev, int c, dma_addr_t x)
{
	struct i596_private *lp = netdev_priv(dev);

	u32 v = (u32) (c) | (u32) (x);

	if (lp->options & OPT_MPU_16BIT) {
		writew(v & 0xffff, lp->mpu_port);
		wmb();  /* order writes to MPU port */
		udelay(1);
		writew(v >> 16, lp->mpu_port);
	} else {
		writel(v, lp->mpu_port);
		wmb();  /* order writes to MPU port */
		udelay(1);
		writel(v, lp->mpu_port);
	}
}


<<<<<<< HEAD
static int __devinit sni_82596_probe(struct platform_device *dev)
=======
static int sni_82596_probe(struct platform_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct	net_device *netdevice;
	struct i596_private *lp;
	struct  resource *res, *ca, *idprom, *options;
	int	retval = -ENOMEM;
	void __iomem *mpu_addr;
	void __iomem *ca_addr;
	u8 __iomem *eth_addr;
<<<<<<< HEAD
=======
	u8 mac[ETH_ALEN];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	ca = platform_get_resource(dev, IORESOURCE_MEM, 1);
	options = platform_get_resource(dev, 0, 0);
	idprom = platform_get_resource(dev, IORESOURCE_MEM, 2);
	if (!res || !ca || !options || !idprom)
		return -ENODEV;
<<<<<<< HEAD
	mpu_addr = ioremap_nocache(res->start, 4);
	if (!mpu_addr)
		return -ENOMEM;
	ca_addr = ioremap_nocache(ca->start, 4);
=======
	mpu_addr = ioremap(res->start, 4);
	if (!mpu_addr)
		return -ENOMEM;
	ca_addr = ioremap(ca->start, 4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ca_addr)
		goto probe_failed_free_mpu;

	printk(KERN_INFO "Found i82596 at 0x%x\n", res->start);

	netdevice = alloc_etherdev(sizeof(struct i596_private));
	if (!netdevice)
		goto probe_failed_free_ca;

	SET_NETDEV_DEV(netdevice, &dev->dev);
	platform_set_drvdata (dev, netdevice);

	netdevice->base_addr = res->start;
	netdevice->irq = platform_get_irq(dev, 0);

<<<<<<< HEAD
	eth_addr = ioremap_nocache(idprom->start, 0x10);
=======
	eth_addr = ioremap(idprom->start, 0x10);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!eth_addr)
		goto probe_failed;

	/* someone seems to like messed up stuff */
<<<<<<< HEAD
	netdevice->dev_addr[0] = readb(eth_addr + 0x0b);
	netdevice->dev_addr[1] = readb(eth_addr + 0x0a);
	netdevice->dev_addr[2] = readb(eth_addr + 0x09);
	netdevice->dev_addr[3] = readb(eth_addr + 0x08);
	netdevice->dev_addr[4] = readb(eth_addr + 0x07);
	netdevice->dev_addr[5] = readb(eth_addr + 0x06);
	iounmap(eth_addr);

	if (!netdevice->irq) {
		printk(KERN_ERR "%s: IRQ not found for i82596 at 0x%lx\n",
			__FILE__, netdevice->base_addr);
=======
	mac[0] = readb(eth_addr + 0x0b);
	mac[1] = readb(eth_addr + 0x0a);
	mac[2] = readb(eth_addr + 0x09);
	mac[3] = readb(eth_addr + 0x08);
	mac[4] = readb(eth_addr + 0x07);
	mac[5] = readb(eth_addr + 0x06);
	eth_hw_addr_set(netdevice, mac);
	iounmap(eth_addr);

	if (netdevice->irq < 0) {
		printk(KERN_ERR "%s: IRQ not found for i82596 at 0x%lx\n",
			__FILE__, netdevice->base_addr);
		retval = netdevice->irq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto probe_failed;
	}

	lp = netdev_priv(netdevice);
	lp->options = options->flags & IORESOURCE_BITS;
	lp->ca = ca_addr;
	lp->mpu_port = mpu_addr;

<<<<<<< HEAD
	retval = i82596_probe(netdevice);
	if (retval == 0)
		return 0;

=======
	lp->dma = dma_alloc_coherent(&dev->dev, sizeof(struct i596_dma),
				     &lp->dma_addr, GFP_KERNEL);
	if (!lp->dma)
		goto probe_failed;

	retval = i82596_probe(netdevice);
	if (retval)
		goto probe_failed_free_dma;
	return 0;

probe_failed_free_dma:
	dma_free_coherent(&dev->dev, sizeof(struct i596_dma), lp->dma,
			  lp->dma_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
probe_failed:
	free_netdev(netdevice);
probe_failed_free_ca:
	iounmap(ca_addr);
probe_failed_free_mpu:
	iounmap(mpu_addr);
	return retval;
}

<<<<<<< HEAD
static int __devexit sni_82596_driver_remove(struct platform_device *pdev)
=======
static void sni_82596_driver_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct i596_private *lp = netdev_priv(dev);

	unregister_netdev(dev);
<<<<<<< HEAD
	DMA_FREE(dev->dev.parent, sizeof(struct i596_private),
		 lp->dma, lp->dma_addr);
	iounmap(lp->ca);
	iounmap(lp->mpu_port);
	free_netdev (dev);
	return 0;
=======
	dma_free_coherent(&pdev->dev, sizeof(struct i596_private), lp->dma,
			  lp->dma_addr);
	iounmap(lp->ca);
	iounmap(lp->mpu_port);
	free_netdev (dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver sni_82596_driver = {
	.probe	= sni_82596_probe,
<<<<<<< HEAD
	.remove	= __devexit_p(sni_82596_driver_remove),
	.driver	= {
		.name	= sni_82596_string,
		.owner	= THIS_MODULE,
	},
};

static int __devinit sni_82596_init(void)
=======
	.remove_new = sni_82596_driver_remove,
	.driver	= {
		.name	= sni_82596_string,
	},
};

static int sni_82596_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	printk(KERN_INFO SNI_82596_DRIVER_VERSION "\n");
	return platform_driver_register(&sni_82596_driver);
}


static void __exit sni_82596_exit(void)
{
	platform_driver_unregister(&sni_82596_driver);
}

module_init(sni_82596_init);
module_exit(sni_82596_exit);
