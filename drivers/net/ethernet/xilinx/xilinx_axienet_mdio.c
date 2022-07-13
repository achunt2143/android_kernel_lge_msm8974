<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MDIO bus driver for the Xilinx Axi Ethernet device
 *
 * Copyright (c) 2009 Secret Lab Technologies, Ltd.
 * Copyright (c) 2010 - 2011 Michal Simek <monstr@monstr.eu>
 * Copyright (c) 2010 - 2011 PetaLogix
<<<<<<< HEAD
 * Copyright (c) 2010 - 2012 Xilinx, Inc. All rights reserved.
 */

#include <linux/of_address.h>
#include <linux/of_mdio.h>
#include <linux/jiffies.h>

#include "xilinx_axienet.h"

#define MAX_MDIO_FREQ		2500000 /* 2.5 MHz */
#define DEFAULT_CLOCK_DIVISOR	XAE_MDIO_DIV_DFT

/* Wait till MDIO interface is ready to accept a new transaction.*/
int axienet_mdio_wait_until_ready(struct axienet_local *lp)
{
	long end = jiffies + 2;
	while (!(axienet_ior(lp, XAE_MDIO_MCR_OFFSET) &
		 XAE_MDIO_MCR_READY_MASK)) {
		if (end - jiffies <= 0) {
			WARN_ON(1);
			return -ETIMEDOUT;
		}
		udelay(1);
	}
	return 0;
=======
 * Copyright (c) 2019 SED Systems, a division of Calian Ltd.
 * Copyright (c) 2010 - 2012 Xilinx, Inc. All rights reserved.
 */

#include <linux/clk.h>
#include <linux/of_address.h>
#include <linux/of_mdio.h>
#include <linux/jiffies.h>
#include <linux/iopoll.h>

#include "xilinx_axienet.h"

#define DEFAULT_MDIO_FREQ	2500000 /* 2.5 MHz */
#define DEFAULT_HOST_CLOCK	150000000 /* 150 MHz */

/* Wait till MDIO interface is ready to accept a new transaction.*/
static int axienet_mdio_wait_until_ready(struct axienet_local *lp)
{
	u32 val;

	return readx_poll_timeout(axinet_ior_read_mcr, lp,
				  val, val & XAE_MDIO_MCR_READY_MASK,
				  1, 20000);
}

/* Enable the MDIO MDC. Called prior to a read/write operation */
static void axienet_mdio_mdc_enable(struct axienet_local *lp)
{
	axienet_iow(lp, XAE_MDIO_MC_OFFSET,
		    ((u32)lp->mii_clk_div | XAE_MDIO_MC_MDIOEN_MASK));
}

/* Disable the MDIO MDC. Called after a read/write operation*/
static void axienet_mdio_mdc_disable(struct axienet_local *lp)
{
	u32 mc_reg;

	mc_reg = axienet_ior(lp, XAE_MDIO_MC_OFFSET);
	axienet_iow(lp, XAE_MDIO_MC_OFFSET,
		    (mc_reg & ~XAE_MDIO_MC_MDIOEN_MASK));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * axienet_mdio_read - MDIO interface read function
 * @bus:	Pointer to mii bus structure
 * @phy_id:	Address of the PHY device
 * @reg:	PHY register to read
 *
<<<<<<< HEAD
 * returns:	The register contents on success, -ETIMEDOUT on a timeout
=======
 * Return:	The register contents on success, -ETIMEDOUT on a timeout
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Reads the contents of the requested register from the requested PHY
 * address by first writing the details into MCR register. After a while
 * the register MRD is read to obtain the PHY register content.
 */
static int axienet_mdio_read(struct mii_bus *bus, int phy_id, int reg)
{
	u32 rc;
	int ret;
	struct axienet_local *lp = bus->priv;

<<<<<<< HEAD
	ret = axienet_mdio_wait_until_ready(lp);
	if (ret < 0)
		return ret;
=======
	axienet_mdio_mdc_enable(lp);

	ret = axienet_mdio_wait_until_ready(lp);
	if (ret < 0) {
		axienet_mdio_mdc_disable(lp);
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	axienet_iow(lp, XAE_MDIO_MCR_OFFSET,
		    (((phy_id << XAE_MDIO_MCR_PHYAD_SHIFT) &
		      XAE_MDIO_MCR_PHYAD_MASK) |
		     ((reg << XAE_MDIO_MCR_REGAD_SHIFT) &
		      XAE_MDIO_MCR_REGAD_MASK) |
		     XAE_MDIO_MCR_INITIATE_MASK |
		     XAE_MDIO_MCR_OP_READ_MASK));

	ret = axienet_mdio_wait_until_ready(lp);
<<<<<<< HEAD
	if (ret < 0)
		return ret;
=======
	if (ret < 0) {
		axienet_mdio_mdc_disable(lp);
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = axienet_ior(lp, XAE_MDIO_MRD_OFFSET) & 0x0000FFFF;

	dev_dbg(lp->dev, "axienet_mdio_read(phy_id=%i, reg=%x) == %x\n",
		phy_id, reg, rc);

<<<<<<< HEAD
=======
	axienet_mdio_mdc_disable(lp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

/**
 * axienet_mdio_write - MDIO interface write function
 * @bus:	Pointer to mii bus structure
 * @phy_id:	Address of the PHY device
 * @reg:	PHY register to write to
 * @val:	Value to be written into the register
 *
<<<<<<< HEAD
 * returns:	0 on success, -ETIMEDOUT on a timeout
 *
 * Writes the value to the requested register by first writing the value
 * into MWD register. The the MCR register is then appropriately setup
=======
 * Return:	0 on success, -ETIMEDOUT on a timeout
 *
 * Writes the value to the requested register by first writing the value
 * into MWD register. The MCR register is then appropriately setup
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * to finish the write operation.
 */
static int axienet_mdio_write(struct mii_bus *bus, int phy_id, int reg,
			      u16 val)
{
	int ret;
	struct axienet_local *lp = bus->priv;

	dev_dbg(lp->dev, "axienet_mdio_write(phy_id=%i, reg=%x, val=%x)\n",
		phy_id, reg, val);

<<<<<<< HEAD
	ret = axienet_mdio_wait_until_ready(lp);
	if (ret < 0)
		return ret;

	axienet_iow(lp, XAE_MDIO_MWD_OFFSET, (u32) val);
=======
	axienet_mdio_mdc_enable(lp);

	ret = axienet_mdio_wait_until_ready(lp);
	if (ret < 0) {
		axienet_mdio_mdc_disable(lp);
		return ret;
	}

	axienet_iow(lp, XAE_MDIO_MWD_OFFSET, (u32)val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	axienet_iow(lp, XAE_MDIO_MCR_OFFSET,
		    (((phy_id << XAE_MDIO_MCR_PHYAD_SHIFT) &
		      XAE_MDIO_MCR_PHYAD_MASK) |
		     ((reg << XAE_MDIO_MCR_REGAD_SHIFT) &
		      XAE_MDIO_MCR_REGAD_MASK) |
		     XAE_MDIO_MCR_INITIATE_MASK |
		     XAE_MDIO_MCR_OP_WRITE_MASK));

	ret = axienet_mdio_wait_until_ready(lp);
<<<<<<< HEAD
	if (ret < 0)
		return ret;
=======
	if (ret < 0) {
		axienet_mdio_mdc_disable(lp);
		return ret;
	}
	axienet_mdio_mdc_disable(lp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
 * axienet_mdio_setup - MDIO setup function
 * @lp:		Pointer to axienet local data structure.
 * @np:		Pointer to device node
 *
 * returns:	0 on success, -ETIMEDOUT on a timeout, -ENOMEM when
 *		mdiobus_alloc (to allocate memory for mii bus structure) fails.
 *
 * Sets up the MDIO interface by initializing the MDIO clock and enabling the
 * MDIO interface in hardware. Register the MDIO interface.
 **/
int axienet_mdio_setup(struct axienet_local *lp, struct device_node *np)
{
	int ret;
	u32 clk_div, host_clock;
	u32 *property_p;
	struct mii_bus *bus;
	struct resource res;
	struct device_node *np1;
=======
 * axienet_mdio_enable - MDIO hardware setup function
 * @lp:		Pointer to axienet local data structure.
 * @np:		Pointer to mdio device tree node.
 *
 * Return:	0 on success, -ETIMEDOUT on a timeout, -EOVERFLOW on a clock
 *		divisor overflow.
 *
 * Sets up the MDIO interface by initializing the MDIO clock and enabling the
 * MDIO interface in hardware.
 **/
static int axienet_mdio_enable(struct axienet_local *lp, struct device_node *np)
{
	u32 mdio_freq = DEFAULT_MDIO_FREQ;
	u32 host_clock;
	u32 clk_div;
	int ret;

	lp->mii_clk_div = 0;

	if (lp->axi_clk) {
		host_clock = clk_get_rate(lp->axi_clk);
	} else {
		struct device_node *np1;

		/* Legacy fallback: detect CPU clock frequency and use as AXI
		 * bus clock frequency. This only works on certain platforms.
		 */
		np1 = of_find_node_by_name(NULL, "cpu");
		if (!np1) {
			netdev_warn(lp->ndev, "Could not find CPU device node.\n");
			host_clock = DEFAULT_HOST_CLOCK;
		} else {
			int ret = of_property_read_u32(np1, "clock-frequency",
						       &host_clock);
			if (ret) {
				netdev_warn(lp->ndev, "CPU clock-frequency property not found.\n");
				host_clock = DEFAULT_HOST_CLOCK;
			}
			of_node_put(np1);
		}
		netdev_info(lp->ndev, "Setting assumed host clock to %u\n",
			    host_clock);
	}

	if (np)
		of_property_read_u32(np, "clock-frequency", &mdio_freq);
	if (mdio_freq != DEFAULT_MDIO_FREQ)
		netdev_info(lp->ndev, "Setting non-standard mdio bus frequency to %u Hz\n",
			    mdio_freq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* clk_div can be calculated by deriving it from the equation:
	 * fMDIO = fHOST / ((1 + clk_div) * 2)
	 *
	 * Where fMDIO <= 2500000, so we get:
	 * fHOST / ((1 + clk_div) * 2) <= 2500000
	 *
	 * Then we get:
	 * 1 / ((1 + clk_div) * 2) <= (2500000 / fHOST)
	 *
	 * Then we get:
	 * 1 / (1 + clk_div) <= ((2500000 * 2) / fHOST)
	 *
	 * Then we get:
	 * 1 / (1 + clk_div) <= (5000000 / fHOST)
	 *
	 * So:
	 * (1 + clk_div) >= (fHOST / 5000000)
	 *
	 * And finally:
	 * clk_div >= (fHOST / 5000000) - 1
	 *
	 * fHOST can be read from the flattened device tree as property
	 * "clock-frequency" from the CPU
	 */

<<<<<<< HEAD
	np1 = of_find_node_by_name(NULL, "cpu");
	if (!np1) {
		printk(KERN_WARNING "%s(): Could not find CPU device node.",
		       __func__);
		printk(KERN_WARNING "Setting MDIO clock divisor to "
		       "default %d\n", DEFAULT_CLOCK_DIVISOR);
		clk_div = DEFAULT_CLOCK_DIVISOR;
		goto issue;
	}
	property_p = (u32 *) of_get_property(np1, "clock-frequency", NULL);
	if (!property_p) {
		printk(KERN_WARNING "%s(): Could not find CPU property: "
		       "clock-frequency.", __func__);
		printk(KERN_WARNING "Setting MDIO clock divisor to "
		       "default %d\n", DEFAULT_CLOCK_DIVISOR);
		clk_div = DEFAULT_CLOCK_DIVISOR;
		goto issue;
	}

	host_clock = be32_to_cpup(property_p);
	clk_div = (host_clock / (MAX_MDIO_FREQ * 2)) - 1;
	/* If there is any remainder from the division of
	 * fHOST / (MAX_MDIO_FREQ * 2), then we need to add
	 * 1 to the clock divisor or we will surely be above 2.5 MHz */
	if (host_clock % (MAX_MDIO_FREQ * 2))
		clk_div++;

	printk(KERN_DEBUG "%s(): Setting MDIO clock divisor to %u based "
	       "on %u Hz host clock.\n", __func__, clk_div, host_clock);

	of_node_put(np1);
issue:
	axienet_iow(lp, XAE_MDIO_MC_OFFSET,
		    (((u32) clk_div) | XAE_MDIO_MC_MDIOEN_MASK));

	ret = axienet_mdio_wait_until_ready(lp);
	if (ret < 0)
		return ret;
=======
	clk_div = (host_clock / (mdio_freq * 2)) - 1;
	/* If there is any remainder from the division of
	 * fHOST / (mdio_freq * 2), then we need to add
	 * 1 to the clock divisor or we will surely be
	 * above the requested frequency
	 */
	if (host_clock % (mdio_freq * 2))
		clk_div++;

	/* Check for overflow of mii_clk_div */
	if (clk_div & ~XAE_MDIO_MC_CLOCK_DIVIDE_MAX) {
		netdev_warn(lp->ndev, "MDIO clock divisor overflow\n");
		return -EOVERFLOW;
	}
	lp->mii_clk_div = (u8)clk_div;

	netdev_dbg(lp->ndev,
		   "Setting MDIO clock divisor to %u/%u Hz host clock.\n",
		   lp->mii_clk_div, host_clock);

	axienet_mdio_mdc_enable(lp);

	ret = axienet_mdio_wait_until_ready(lp);
	if (ret)
		axienet_mdio_mdc_disable(lp);

	return ret;
}

/**
 * axienet_mdio_setup - MDIO setup function
 * @lp:		Pointer to axienet local data structure.
 *
 * Return:	0 on success, -ETIMEDOUT on a timeout, -EOVERFLOW on a clock
 *		divisor overflow, -ENOMEM when mdiobus_alloc (to allocate
 *		memory for mii bus structure) fails.
 *
 * Sets up the MDIO interface by initializing the MDIO clock.
 * Register the MDIO interface.
 **/
int axienet_mdio_setup(struct axienet_local *lp)
{
	struct device_node *mdio_node;
	struct mii_bus *bus;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bus = mdiobus_alloc();
	if (!bus)
		return -ENOMEM;

<<<<<<< HEAD
	np1 = of_get_parent(lp->phy_node);
	of_address_to_resource(np1, 0, &res);
	snprintf(bus->id, MII_BUS_ID_SIZE, "%.8llx",
		 (unsigned long long) res.start);
=======
	snprintf(bus->id, MII_BUS_ID_SIZE, "axienet-%.8llx",
		 (unsigned long long)lp->regs_start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bus->priv = lp;
	bus->name = "Xilinx Axi Ethernet MDIO";
	bus->read = axienet_mdio_read;
	bus->write = axienet_mdio_write;
	bus->parent = lp->dev;
<<<<<<< HEAD
	bus->irq = lp->mdio_irqs; /* preallocated IRQ table */
	lp->mii_bus = bus;

	ret = of_mdiobus_register(bus, np1);
	if (ret) {
		mdiobus_free(bus);
		return ret;
	}
	return 0;
=======
	lp->mii_bus = bus;

	mdio_node = of_get_child_by_name(lp->dev->of_node, "mdio");
	ret = axienet_mdio_enable(lp, mdio_node);
	if (ret < 0)
		goto unregister;
	ret = of_mdiobus_register(bus, mdio_node);
	if (ret)
		goto unregister_mdio_enabled;
	of_node_put(mdio_node);
	axienet_mdio_mdc_disable(lp);
	return 0;

unregister_mdio_enabled:
	axienet_mdio_mdc_disable(lp);
unregister:
	of_node_put(mdio_node);
	mdiobus_free(bus);
	lp->mii_bus = NULL;
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * axienet_mdio_teardown - MDIO remove function
 * @lp:		Pointer to axienet local data structure.
 *
 * Unregisters the MDIO and frees any associate memory for mii bus.
 */
void axienet_mdio_teardown(struct axienet_local *lp)
{
	mdiobus_unregister(lp->mii_bus);
<<<<<<< HEAD
	kfree(lp->mii_bus->irq);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mdiobus_free(lp->mii_bus);
	lp->mii_bus = NULL;
}
