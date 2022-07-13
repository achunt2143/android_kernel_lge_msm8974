<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Freescale PowerQUICC Ethernet Driver -- MIIM bus implementation
 * Provides Bus interface for MIIM regs
 *
 * Author: Andy Fleming <afleming@freescale.com>
 * Modifier: Sandeep Gopalpet <sandeep.kumar@freescale.com>
 *
 * Copyright 2002-2004, 2008-2009 Freescale Semiconductor, Inc.
 *
 * Based on gianfar_mii.c and ucc_geth_mii.c (Li Yang, Kim Phillips)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/crc32.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_mdio.h>
#include <linux/of_platform.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/ucc.h>

#include "gianfar.h"
#include "fsl_pq_mdio.h"

struct fsl_pq_mdio_priv {
	void __iomem *map;
	struct fsl_pq_mdio __iomem *regs;
};

/*
 * Write value to the PHY at mii_id at register regnum,
 * on the bus attached to the local interface, which may be different from the
 * generic mdio bus (tied to a single interface), waiting until the write is
 * done before returning. This is helpful in programming interfaces like
 * the TBI which control interfaces like onchip SERDES and are always tied to
 * the local mdio pins, which may not be the same as system mdio bus, used for
 * controlling the external PHYs, for example.
 */
int fsl_pq_local_mdio_write(struct fsl_pq_mdio __iomem *regs, int mii_id,
		int regnum, u16 value)
{
	/* Set the PHY address and the register address we want to write */
	out_be32(&regs->miimadd, (mii_id << 8) | regnum);

	/* Write out the value we want */
	out_be32(&regs->miimcon, value);

	/* Wait for the transaction to finish */
	while (in_be32(&regs->miimind) & MIIMIND_BUSY)
		cpu_relax();

	return 0;
}

/*
 * Read the bus for PHY at addr mii_id, register regnum, and
 * return the value.  Clears miimcom first.  All PHY operation
 * done on the bus attached to the local interface,
 * which may be different from the generic mdio bus
 * This is helpful in programming interfaces like
 * the TBI which, in turn, control interfaces like onchip SERDES
 * and are always tied to the local mdio pins, which may not be the
 * same as system mdio bus, used for controlling the external PHYs, for eg.
 */
int fsl_pq_local_mdio_read(struct fsl_pq_mdio __iomem *regs,
		int mii_id, int regnum)
{
	u16 value;

	/* Set the PHY address and the register address we want to read */
	out_be32(&regs->miimadd, (mii_id << 8) | regnum);

	/* Clear miimcom, and then initiate a read */
	out_be32(&regs->miimcom, 0);
	out_be32(&regs->miimcom, MII_READ_COMMAND);

	/* Wait for the transaction to finish */
	while (in_be32(&regs->miimind) & (MIIMIND_NOTVALID | MIIMIND_BUSY))
		cpu_relax();

	/* Grab the value of the register from miimstat */
	value = in_be32(&regs->miimstat);

	return value;
}

static struct fsl_pq_mdio __iomem *fsl_pq_mdio_get_regs(struct mii_bus *bus)
{
	struct fsl_pq_mdio_priv *priv = bus->priv;

	return priv->regs;
}

/*
 * Write value to the PHY at mii_id at register regnum,
 * on the bus, waiting until the write is done before returning.
 */
int fsl_pq_mdio_write(struct mii_bus *bus, int mii_id, int regnum, u16 value)
{
	struct fsl_pq_mdio __iomem *regs = fsl_pq_mdio_get_regs(bus);

	/* Write to the local MII regs */
	return fsl_pq_local_mdio_write(regs, mii_id, regnum, value);
}

/*
 * Read the bus for PHY at addr mii_id, register regnum, and
 * return the value.  Clears miimcom first.
 */
int fsl_pq_mdio_read(struct mii_bus *bus, int mii_id, int regnum)
{
	struct fsl_pq_mdio __iomem *regs = fsl_pq_mdio_get_regs(bus);

	/* Read the local MII regs */
	return fsl_pq_local_mdio_read(regs, mii_id, regnum);
}

/* Reset the MIIM registers, and wait for the bus to free */
static int fsl_pq_mdio_reset(struct mii_bus *bus)
{
	struct fsl_pq_mdio __iomem *regs = fsl_pq_mdio_get_regs(bus);
	int timeout = PHY_INIT_TIMEOUT;
=======
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_mdio.h>
#include <linux/property.h>

#include <asm/io.h>
#if IS_ENABLED(CONFIG_UCC_GETH)
#include <soc/fsl/qe/ucc.h>
#endif

#include "gianfar.h"

#define MIIMIND_BUSY		0x00000001
#define MIIMIND_NOTVALID	0x00000004
#define MIIMCFG_INIT_VALUE	0x00000007
#define MIIMCFG_RESET		0x80000000

#define MII_READ_COMMAND	0x00000001

struct fsl_pq_mii {
	u32 miimcfg;	/* MII management configuration reg */
	u32 miimcom;	/* MII management command reg */
	u32 miimadd;	/* MII management address reg */
	u32 miimcon;	/* MII management control reg */
	u32 miimstat;	/* MII management status reg */
	u32 miimind;	/* MII management indication reg */
};

struct fsl_pq_mdio {
	u8 res1[16];
	u32 ieventm;	/* MDIO Interrupt event register (for etsec2)*/
	u32 imaskm;	/* MDIO Interrupt mask register (for etsec2)*/
	u8 res2[4];
	u32 emapm;	/* MDIO Event mapping register (for etsec2)*/
	u8 res3[1280];
	struct fsl_pq_mii mii;
	u8 res4[28];
	u32 utbipar;	/* TBI phy address reg (only on UCC) */
	u8 res5[2728];
} __packed;

/* Number of microseconds to wait for an MII register to respond */
#define MII_TIMEOUT	1000

struct fsl_pq_mdio_priv {
	void __iomem *map;
	struct fsl_pq_mii __iomem *regs;
};

/*
 * Per-device-type data.  Each type of device tree node that we support gets
 * one of these.
 *
 * @mii_offset: the offset of the MII registers within the memory map of the
 * node.  Some nodes define only the MII registers, and some define the whole
 * MAC (which includes the MII registers).
 *
 * @get_tbipa: determines the address of the TBIPA register
 *
 * @ucc_configure: a special function for extra QE configuration
 */
struct fsl_pq_mdio_data {
	unsigned int mii_offset;	/* offset of the MII registers */
	uint32_t __iomem * (*get_tbipa)(void __iomem *p);
	void (*ucc_configure)(phys_addr_t start, phys_addr_t end);
};

/*
 * Write value to the PHY at mii_id at register regnum, on the bus attached
 * to the local interface, which may be different from the generic mdio bus
 * (tied to a single interface), waiting until the write is done before
 * returning. This is helpful in programming interfaces like the TBI which
 * control interfaces like onchip SERDES and are always tied to the local
 * mdio pins, which may not be the same as system mdio bus, used for
 * controlling the external PHYs, for example.
 */
static int fsl_pq_mdio_write(struct mii_bus *bus, int mii_id, int regnum,
		u16 value)
{
	struct fsl_pq_mdio_priv *priv = bus->priv;
	struct fsl_pq_mii __iomem *regs = priv->regs;
	unsigned int timeout;

	/* Set the PHY address and the register address we want to write */
	iowrite32be((mii_id << 8) | regnum, &regs->miimadd);

	/* Write out the value we want */
	iowrite32be(value, &regs->miimcon);

	/* Wait for the transaction to finish */
	timeout = MII_TIMEOUT;
	while ((ioread32be(&regs->miimind) & MIIMIND_BUSY) && timeout) {
		cpu_relax();
		timeout--;
	}

	return timeout ? 0 : -ETIMEDOUT;
}

/*
 * Read the bus for PHY at addr mii_id, register regnum, and return the value.
 * Clears miimcom first.
 *
 * All PHY operation done on the bus attached to the local interface, which
 * may be different from the generic mdio bus.  This is helpful in programming
 * interfaces like the TBI which, in turn, control interfaces like on-chip
 * SERDES and are always tied to the local mdio pins, which may not be the
 * same as system mdio bus, used for controlling the external PHYs, for eg.
 */
static int fsl_pq_mdio_read(struct mii_bus *bus, int mii_id, int regnum)
{
	struct fsl_pq_mdio_priv *priv = bus->priv;
	struct fsl_pq_mii __iomem *regs = priv->regs;
	unsigned int timeout;
	u16 value;

	/* Set the PHY address and the register address we want to read */
	iowrite32be((mii_id << 8) | regnum, &regs->miimadd);

	/* Clear miimcom, and then initiate a read */
	iowrite32be(0, &regs->miimcom);
	iowrite32be(MII_READ_COMMAND, &regs->miimcom);

	/* Wait for the transaction to finish, normally less than 100us */
	timeout = MII_TIMEOUT;
	while ((ioread32be(&regs->miimind) &
	       (MIIMIND_NOTVALID | MIIMIND_BUSY)) && timeout) {
		cpu_relax();
		timeout--;
	}

	if (!timeout)
		return -ETIMEDOUT;

	/* Grab the value of the register from miimstat */
	value = ioread32be(&regs->miimstat);

	dev_dbg(&bus->dev, "read %04x from address %x/%x\n", value, mii_id, regnum);
	return value;
}

/* Reset the MIIM registers, and wait for the bus to free */
static int fsl_pq_mdio_reset(struct mii_bus *bus)
{
	struct fsl_pq_mdio_priv *priv = bus->priv;
	struct fsl_pq_mii __iomem *regs = priv->regs;
	unsigned int timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&bus->mdio_lock);

	/* Reset the management interface */
<<<<<<< HEAD
	out_be32(&regs->miimcfg, MIIMCFG_RESET);

	/* Setup the MII Mgmt clock speed */
	out_be32(&regs->miimcfg, MIIMCFG_INIT_VALUE);

	/* Wait until the bus is free */
	while ((in_be32(&regs->miimind) & MIIMIND_BUSY) && timeout--)
		cpu_relax();

	mutex_unlock(&bus->mdio_lock);

	if (timeout < 0) {
		printk(KERN_ERR "%s: The MII Bus is stuck!\n",
				bus->name);
=======
	iowrite32be(MIIMCFG_RESET, &regs->miimcfg);

	/* Setup the MII Mgmt clock speed */
	iowrite32be(MIIMCFG_INIT_VALUE, &regs->miimcfg);

	/* Wait until the bus is free */
	timeout = MII_TIMEOUT;
	while ((ioread32be(&regs->miimind) & MIIMIND_BUSY) && timeout) {
		cpu_relax();
		timeout--;
	}

	mutex_unlock(&bus->mdio_lock);

	if (!timeout) {
		dev_err(&bus->dev, "timeout waiting for MII bus\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBUSY;
	}

	return 0;
}

<<<<<<< HEAD
void fsl_pq_mdio_bus_name(char *name, struct device_node *np)
{
	const u32 *addr;
	u64 taddr = OF_BAD_ADDR;

	addr = of_get_address(np, 0, NULL, NULL);
	if (addr)
		taddr = of_translate_address(np, addr);

	snprintf(name, MII_BUS_ID_SIZE, "%s@%llx", np->name,
		(unsigned long long)taddr);
}
EXPORT_SYMBOL_GPL(fsl_pq_mdio_bus_name);


static u32 __iomem *get_gfar_tbipa(struct fsl_pq_mdio __iomem *regs, struct device_node *np)
{
#if defined(CONFIG_GIANFAR) || defined(CONFIG_GIANFAR_MODULE)
	struct gfar __iomem *enet_regs;

	/*
	 * This is mildly evil, but so is our hardware for doing this.
	 * Also, we have to cast back to struct gfar because of
	 * definition weirdness done in gianfar.h.
	 */
	if(of_device_is_compatible(np, "fsl,gianfar-mdio") ||
		of_device_is_compatible(np, "fsl,gianfar-tbi") ||
		of_device_is_compatible(np, "gianfar")) {
		enet_regs = (struct gfar __iomem *)regs;
		return &enet_regs->tbipa;
	} else if (of_device_is_compatible(np, "fsl,etsec2-mdio") ||
			of_device_is_compatible(np, "fsl,etsec2-tbi")) {
		return of_iomap(np, 1);
	}
#endif
	return NULL;
}


static int get_ucc_id_for_range(u64 start, u64 end, u32 *ucc_id)
{
#if defined(CONFIG_UCC_GETH) || defined(CONFIG_UCC_GETH_MODULE)
	struct device_node *np = NULL;
	int err = 0;

	for_each_compatible_node(np, NULL, "ucc_geth") {
		struct resource tempres;

		err = of_address_to_resource(np, 0, &tempres);
		if (err)
			continue;

		/* if our mdio regs fall within this UCC regs range */
		if ((start >= tempres.start) && (end <= tempres.end)) {
			/* Find the id of the UCC */
			const u32 *id;

			id = of_get_property(np, "cell-index", NULL);
			if (!id) {
				id = of_get_property(np, "device-id", NULL);
				if (!id)
					continue;
			}

			*ucc_id = *id;

			return 0;
		}
	}

	if (err)
		return err;
	else
		return -EINVAL;
#else
	return -ENODEV;
#endif
}

static int fsl_pq_mdio_probe(struct platform_device *ofdev)
{
	struct device_node *np = ofdev->dev.of_node;
	struct device_node *tbi;
	struct fsl_pq_mdio_priv *priv;
	struct fsl_pq_mdio __iomem *regs = NULL;
	void __iomem *map;
	u32 __iomem *tbipa;
	struct mii_bus *new_bus;
	int tbiaddr = -1;
	const u32 *addrp;
	u64 addr = 0, size = 0;
	int err;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	new_bus = mdiobus_alloc();
	if (!new_bus) {
		err = -ENOMEM;
		goto err_free_priv;
	}

	new_bus->name = "Freescale PowerQUICC MII Bus",
	new_bus->read = &fsl_pq_mdio_read,
	new_bus->write = &fsl_pq_mdio_write,
	new_bus->reset = &fsl_pq_mdio_reset,
	new_bus->priv = priv;
	fsl_pq_mdio_bus_name(new_bus->id, np);

	addrp = of_get_address(np, 0, &size, NULL);
	if (!addrp) {
		err = -EINVAL;
		goto err_free_bus;
	}

	/* Set the PHY base address */
	addr = of_translate_address(np, addrp);
	if (addr == OF_BAD_ADDR) {
		err = -EINVAL;
		goto err_free_bus;
	}

	map = ioremap(addr, size);
	if (!map) {
		err = -ENOMEM;
		goto err_free_bus;
	}
	priv->map = map;

	if (of_device_is_compatible(np, "fsl,gianfar-mdio") ||
			of_device_is_compatible(np, "fsl,gianfar-tbi") ||
			of_device_is_compatible(np, "fsl,ucc-mdio") ||
			of_device_is_compatible(np, "ucc_geth_phy"))
		map -= offsetof(struct fsl_pq_mdio, miimcfg);
	regs = map;
	priv->regs = regs;

	new_bus->irq = kcalloc(PHY_MAX_ADDR, sizeof(int), GFP_KERNEL);

	if (NULL == new_bus->irq) {
		err = -ENOMEM;
		goto err_unmap_regs;
	}

	new_bus->parent = &ofdev->dev;
	dev_set_drvdata(&ofdev->dev, new_bus);

	if (of_device_is_compatible(np, "fsl,gianfar-mdio") ||
			of_device_is_compatible(np, "fsl,gianfar-tbi") ||
			of_device_is_compatible(np, "fsl,etsec2-mdio") ||
			of_device_is_compatible(np, "fsl,etsec2-tbi") ||
			of_device_is_compatible(np, "gianfar")) {
		tbipa = get_gfar_tbipa(regs, np);
		if (!tbipa) {
			err = -EINVAL;
			goto err_free_irqs;
		}
	} else if (of_device_is_compatible(np, "fsl,ucc-mdio") ||
			of_device_is_compatible(np, "ucc_geth_phy")) {
		u32 id;
		static u32 mii_mng_master;

		tbipa = &regs->utbipar;

		if ((err = get_ucc_id_for_range(addr, addr + size, &id)))
			goto err_free_irqs;

		if (!mii_mng_master) {
			mii_mng_master = id;
			ucc_set_qe_mux_mii_mng(id - 1);
		}
	} else {
		err = -ENODEV;
		goto err_free_irqs;
	}

	for_each_child_of_node(np, tbi) {
		if (!strncmp(tbi->type, "tbi-phy", 8))
			break;
	}

	if (tbi) {
		const u32 *prop = of_get_property(tbi, "reg", NULL);

		if (prop)
			tbiaddr = *prop;

		if (tbiaddr == -1) {
			err = -EBUSY;
			goto err_free_irqs;
		} else {
			out_be32(tbipa, tbiaddr);
		}
	}

	err = of_mdiobus_register(new_bus, np);
	if (err) {
		printk (KERN_ERR "%s: Cannot register as MDIO bus\n",
				new_bus->name);
		goto err_free_irqs;
=======
#if IS_ENABLED(CONFIG_GIANFAR)
/*
 * Return the TBIPA address, starting from the address
 * of the mapped GFAR MDIO registers (struct gfar)
 * This is mildly evil, but so is our hardware for doing this.
 * Also, we have to cast back to struct gfar because of
 * definition weirdness done in gianfar.h.
 */
static uint32_t __iomem *get_gfar_tbipa_from_mdio(void __iomem *p)
{
	struct gfar __iomem *enet_regs = p;

	return &enet_regs->tbipa;
}

/*
 * Return the TBIPA address, starting from the address
 * of the mapped GFAR MII registers (gfar_mii_regs[] within struct gfar)
 */
static uint32_t __iomem *get_gfar_tbipa_from_mii(void __iomem *p)
{
	return get_gfar_tbipa_from_mdio(container_of(p, struct gfar, gfar_mii_regs));
}

/*
 * Return the TBIPAR address for an eTSEC2 node
 */
static uint32_t __iomem *get_etsec_tbipa(void __iomem *p)
{
	return p;
}
#endif

#if IS_ENABLED(CONFIG_UCC_GETH)
/*
 * Return the TBIPAR address for a QE MDIO node, starting from the address
 * of the mapped MII registers (struct fsl_pq_mii)
 */
static uint32_t __iomem *get_ucc_tbipa(void __iomem *p)
{
	struct fsl_pq_mdio __iomem *mdio = container_of(p, struct fsl_pq_mdio, mii);

	return &mdio->utbipar;
}

/*
 * Find the UCC node that controls the given MDIO node
 *
 * For some reason, the QE MDIO nodes are not children of the UCC devices
 * that control them.  Therefore, we need to scan all UCC nodes looking for
 * the one that encompases the given MDIO node.  We do this by comparing
 * physical addresses.  The 'start' and 'end' addresses of the MDIO node are
 * passed, and the correct UCC node will cover the entire address range.
 *
 * This assumes that there is only one QE MDIO node in the entire device tree.
 */
static void ucc_configure(phys_addr_t start, phys_addr_t end)
{
	static bool found_mii_master;
	struct device_node *np = NULL;

	if (found_mii_master)
		return;

	for_each_compatible_node(np, NULL, "ucc_geth") {
		struct resource res;
		const uint32_t *iprop;
		uint32_t id;
		int ret;

		ret = of_address_to_resource(np, 0, &res);
		if (ret < 0) {
			pr_debug("fsl-pq-mdio: no address range in node %pOF\n",
				 np);
			continue;
		}

		/* if our mdio regs fall within this UCC regs range */
		if ((start < res.start) || (end > res.end))
			continue;

		iprop = of_get_property(np, "cell-index", NULL);
		if (!iprop) {
			iprop = of_get_property(np, "device-id", NULL);
			if (!iprop) {
				pr_debug("fsl-pq-mdio: no UCC ID in node %pOF\n",
					 np);
				continue;
			}
		}

		id = be32_to_cpup(iprop);

		/*
		 * cell-index and device-id for QE nodes are
		 * numbered from 1, not 0.
		 */
		if (ucc_set_qe_mux_mii_mng(id - 1) < 0) {
			pr_debug("fsl-pq-mdio: invalid UCC ID in node %pOF\n",
				 np);
			continue;
		}

		pr_debug("fsl-pq-mdio: setting node UCC%u to MII master\n", id);
		found_mii_master = true;
	}
}

#endif

static const struct of_device_id fsl_pq_mdio_match[] = {
#if IS_ENABLED(CONFIG_GIANFAR)
	{
		.compatible = "fsl,gianfar-tbi",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = 0,
			.get_tbipa = get_gfar_tbipa_from_mii,
		},
	},
	{
		.compatible = "fsl,gianfar-mdio",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = 0,
			.get_tbipa = get_gfar_tbipa_from_mii,
		},
	},
	{
		.type = "mdio",
		.compatible = "gianfar",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = offsetof(struct fsl_pq_mdio, mii),
			.get_tbipa = get_gfar_tbipa_from_mdio,
		},
	},
	{
		.compatible = "fsl,etsec2-tbi",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = offsetof(struct fsl_pq_mdio, mii),
			.get_tbipa = get_etsec_tbipa,
		},
	},
	{
		.compatible = "fsl,etsec2-mdio",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = offsetof(struct fsl_pq_mdio, mii),
			.get_tbipa = get_etsec_tbipa,
		},
	},
#endif
#if IS_ENABLED(CONFIG_UCC_GETH)
	{
		.compatible = "fsl,ucc-mdio",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = 0,
			.get_tbipa = get_ucc_tbipa,
			.ucc_configure = ucc_configure,
		},
	},
	{
		/* Legacy UCC MDIO node */
		.type = "mdio",
		.compatible = "ucc_geth_phy",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = 0,
			.get_tbipa = get_ucc_tbipa,
			.ucc_configure = ucc_configure,
		},
	},
#endif
	/* No Kconfig option for Fman support yet */
	{
		.compatible = "fsl,fman-mdio",
		.data = &(struct fsl_pq_mdio_data) {
			.mii_offset = 0,
			/* Fman TBI operations are handled elsewhere */
		},
	},

	{},
};
MODULE_DEVICE_TABLE(of, fsl_pq_mdio_match);

static void set_tbipa(const u32 tbipa_val, struct platform_device *pdev,
		      uint32_t __iomem * (*get_tbipa)(void __iomem *),
		      void __iomem *reg_map, struct resource *reg_res)
{
	struct device_node *np = pdev->dev.of_node;
	uint32_t __iomem *tbipa;
	bool tbipa_mapped;

	tbipa = of_iomap(np, 1);
	if (tbipa) {
		tbipa_mapped = true;
	} else {
		tbipa_mapped = false;
		tbipa = (*get_tbipa)(reg_map);

		/*
		 * Add consistency check to make sure TBI is contained within
		 * the mapped range (not because we would get a segfault,
		 * rather to catch bugs in computing TBI address). Print error
		 * message but continue anyway.
		 */
		if ((void *)tbipa > reg_map + resource_size(reg_res) - 4)
			dev_err(&pdev->dev, "invalid register map (should be at least 0x%04zx to contain TBI address)\n",
				((void *)tbipa - reg_map) + 4);
	}

	iowrite32be(be32_to_cpu(tbipa_val), tbipa);

	if (tbipa_mapped)
		iounmap(tbipa);
}

static int fsl_pq_mdio_probe(struct platform_device *pdev)
{
	const struct fsl_pq_mdio_data *data;
	struct device_node *np = pdev->dev.of_node;
	struct resource res;
	struct device_node *tbi;
	struct fsl_pq_mdio_priv *priv;
	struct mii_bus *new_bus;
	int err;

	data = device_get_match_data(&pdev->dev);
	if (!data) {
		dev_err(&pdev->dev, "Failed to match device\n");
		return -ENODEV;
	}

	new_bus = mdiobus_alloc_size(sizeof(*priv));
	if (!new_bus)
		return -ENOMEM;

	priv = new_bus->priv;
	new_bus->name = "Freescale PowerQUICC MII Bus";
	new_bus->read = &fsl_pq_mdio_read;
	new_bus->write = &fsl_pq_mdio_write;
	new_bus->reset = &fsl_pq_mdio_reset;

	err = of_address_to_resource(np, 0, &res);
	if (err < 0) {
		dev_err(&pdev->dev, "could not obtain address information\n");
		goto error;
	}

	snprintf(new_bus->id, MII_BUS_ID_SIZE, "%pOFn@%llx", np,
		 (unsigned long long)res.start);

	priv->map = of_iomap(np, 0);
	if (!priv->map) {
		err = -ENOMEM;
		goto error;
	}

	/*
	 * Some device tree nodes represent only the MII registers, and
	 * others represent the MAC and MII registers.  The 'mii_offset' field
	 * contains the offset of the MII registers inside the mapped register
	 * space.
	 */
	if (data->mii_offset > resource_size(&res)) {
		dev_err(&pdev->dev, "invalid register map\n");
		err = -EINVAL;
		goto error;
	}
	priv->regs = priv->map + data->mii_offset;

	new_bus->parent = &pdev->dev;
	platform_set_drvdata(pdev, new_bus);

	if (data->get_tbipa) {
		for_each_child_of_node(np, tbi) {
			if (of_node_is_type(tbi, "tbi-phy")) {
				dev_dbg(&pdev->dev, "found TBI PHY node %pOFP\n",
					tbi);
				break;
			}
		}

		if (tbi) {
			const u32 *prop = of_get_property(tbi, "reg", NULL);
			if (!prop) {
				dev_err(&pdev->dev,
					"missing 'reg' property in node %pOF\n",
					tbi);
				err = -EBUSY;
				goto error;
			}
			set_tbipa(*prop, pdev,
				  data->get_tbipa, priv->map, &res);
		}
	}

	if (data->ucc_configure)
		data->ucc_configure(res.start, res.end);

	err = of_mdiobus_register(new_bus, np);
	if (err) {
		dev_err(&pdev->dev, "cannot register %s as MDIO bus\n",
			new_bus->name);
		goto error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
err_free_irqs:
	kfree(new_bus->irq);
err_unmap_regs:
	iounmap(priv->map);
err_free_bus:
	kfree(new_bus);
err_free_priv:
	kfree(priv);
=======
error:
	if (priv->map)
		iounmap(priv->map);

	kfree(new_bus);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}


<<<<<<< HEAD
static int fsl_pq_mdio_remove(struct platform_device *ofdev)
{
	struct device *device = &ofdev->dev;
=======
static void fsl_pq_mdio_remove(struct platform_device *pdev)
{
	struct device *device = &pdev->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mii_bus *bus = dev_get_drvdata(device);
	struct fsl_pq_mdio_priv *priv = bus->priv;

	mdiobus_unregister(bus);

<<<<<<< HEAD
	dev_set_drvdata(device, NULL);

	iounmap(priv->map);
	bus->priv = NULL;
	mdiobus_free(bus);
	kfree(priv);

	return 0;
}

static struct of_device_id fsl_pq_mdio_match[] = {
	{
		.type = "mdio",
		.compatible = "ucc_geth_phy",
	},
	{
		.type = "mdio",
		.compatible = "gianfar",
	},
	{
		.compatible = "fsl,ucc-mdio",
	},
	{
		.compatible = "fsl,gianfar-tbi",
	},
	{
		.compatible = "fsl,gianfar-mdio",
	},
	{
		.compatible = "fsl,etsec2-tbi",
	},
	{
		.compatible = "fsl,etsec2-mdio",
	},
	{},
};
MODULE_DEVICE_TABLE(of, fsl_pq_mdio_match);

static struct platform_driver fsl_pq_mdio_driver = {
	.driver = {
		.name = "fsl-pq_mdio",
		.owner = THIS_MODULE,
		.of_match_table = fsl_pq_mdio_match,
	},
	.probe = fsl_pq_mdio_probe,
	.remove = fsl_pq_mdio_remove,
=======
	iounmap(priv->map);
	mdiobus_free(bus);
}

static struct platform_driver fsl_pq_mdio_driver = {
	.driver = {
		.name = "fsl-pq_mdio",
		.of_match_table = fsl_pq_mdio_match,
	},
	.probe = fsl_pq_mdio_probe,
	.remove_new = fsl_pq_mdio_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(fsl_pq_mdio_driver);

<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Freescale PQ MDIO helpers");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
