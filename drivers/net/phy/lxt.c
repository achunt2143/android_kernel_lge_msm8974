<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/net/phy/lxt.c
 *
 * Driver for Intel LXT PHYs
 *
 * Author: Andy Fleming
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>

#include <asm/io.h>
#include <asm/irq.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* The Level one LXT970 is used by many boards				     */

#define MII_LXT970_IER       17  /* Interrupt Enable Register */

#define MII_LXT970_IER_IEN	0x0002

#define MII_LXT970_ISR       18  /* Interrupt Status Register */

<<<<<<< HEAD
=======
#define MII_LXT970_IRS_MINT  BIT(15)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MII_LXT970_CONFIG    19  /* Configuration Register    */

/* ------------------------------------------------------------------------- */
/* The Level one LXT971 is used on some of my custom boards                  */

/* register definitions for the 971 */
#define MII_LXT971_IER		18  /* Interrupt Enable Register */
#define MII_LXT971_IER_IEN	0x00f2

#define MII_LXT971_ISR		19  /* Interrupt Status Register */
<<<<<<< HEAD
=======
#define MII_LXT971_ISR_MASK	0x00f0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* register definitions for the 973 */
#define MII_LXT973_PCR 16 /* Port Configuration Register */
#define PCR_FIBER_SELECT 1

MODULE_DESCRIPTION("Intel LXT PHY driver");
MODULE_AUTHOR("Andy Fleming");
MODULE_LICENSE("GPL");

static int lxt970_ack_interrupt(struct phy_device *phydev)
{
	int err;

	err = phy_read(phydev, MII_BMSR);

	if (err < 0)
		return err;

	err = phy_read(phydev, MII_LXT970_ISR);

	if (err < 0)
		return err;

	return 0;
}

static int lxt970_config_intr(struct phy_device *phydev)
{
	int err;

<<<<<<< HEAD
	if(phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, MII_LXT970_IER, MII_LXT970_IER_IEN);
	else
		err = phy_write(phydev, MII_LXT970_IER, 0);
=======
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED) {
		err = lxt970_ack_interrupt(phydev);
		if (err)
			return err;

		err = phy_write(phydev, MII_LXT970_IER, MII_LXT970_IER_IEN);
	} else {
		err = phy_write(phydev, MII_LXT970_IER, 0);
		if (err)
			return err;

		err = lxt970_ack_interrupt(phydev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
static int lxt970_config_init(struct phy_device *phydev)
{
	int err;

	err = phy_write(phydev, MII_LXT970_CONFIG, 0);

	return err;
=======
static irqreturn_t lxt970_handle_interrupt(struct phy_device *phydev)
{
	int irq_status;

	/* The interrupt status register is cleared by reading BMSR
	 * followed by MII_LXT970_ISR
	 */
	irq_status = phy_read(phydev, MII_BMSR);
	if (irq_status < 0) {
		phy_error(phydev);
		return IRQ_NONE;
	}

	irq_status = phy_read(phydev, MII_LXT970_ISR);
	if (irq_status < 0) {
		phy_error(phydev);
		return IRQ_NONE;
	}

	if (!(irq_status & MII_LXT970_IRS_MINT))
		return IRQ_NONE;

	phy_trigger_machine(phydev);

	return IRQ_HANDLED;
}

static int lxt970_config_init(struct phy_device *phydev)
{
	return phy_write(phydev, MII_LXT970_CONFIG, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static int lxt971_ack_interrupt(struct phy_device *phydev)
{
	int err = phy_read(phydev, MII_LXT971_ISR);

	if (err < 0)
		return err;

	return 0;
}

static int lxt971_config_intr(struct phy_device *phydev)
{
	int err;

<<<<<<< HEAD
	if(phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, MII_LXT971_IER, MII_LXT971_IER_IEN);
	else
		err = phy_write(phydev, MII_LXT971_IER, 0);
=======
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED) {
		err = lxt971_ack_interrupt(phydev);
		if (err)
			return err;

		err = phy_write(phydev, MII_LXT971_IER, MII_LXT971_IER_IEN);
	} else {
		err = phy_write(phydev, MII_LXT971_IER, 0);
		if (err)
			return err;

		err = lxt971_ack_interrupt(phydev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
=======
static irqreturn_t lxt971_handle_interrupt(struct phy_device *phydev)
{
	int irq_status;

	irq_status = phy_read(phydev, MII_LXT971_ISR);
	if (irq_status < 0) {
		phy_error(phydev);
		return IRQ_NONE;
	}

	if (!(irq_status & MII_LXT971_ISR_MASK))
		return IRQ_NONE;

	phy_trigger_machine(phydev);

	return IRQ_HANDLED;
}

/*
 * A2 version of LXT973 chip has an ERRATA: it randomly return the contents
 * of the previous even register when you read a odd register regularly
 */

static int lxt973a2_update_link(struct phy_device *phydev)
{
	int status;
	int control;
	int retry = 8; /* we try 8 times */

	/* Do a fake read */
	status = phy_read(phydev, MII_BMSR);

	if (status < 0)
		return status;

	control = phy_read(phydev, MII_BMCR);
	if (control < 0)
		return control;

	do {
		/* Read link and autonegotiation status */
		status = phy_read(phydev, MII_BMSR);
	} while (status >= 0 && retry-- && status == control);

	if (status < 0)
		return status;

	if ((status & BMSR_LSTATUS) == 0)
		phydev->link = 0;
	else
		phydev->link = 1;

	return 0;
}

static int lxt973a2_read_status(struct phy_device *phydev)
{
	int adv;
	int err;
	int lpa;

	/* Update the link, but return if there was an error */
	err = lxt973a2_update_link(phydev);
	if (err)
		return err;

	if (AUTONEG_ENABLE == phydev->autoneg) {
		int retry = 1;

		adv = phy_read(phydev, MII_ADVERTISE);

		if (adv < 0)
			return adv;

		do {
			lpa = phy_read(phydev, MII_LPA);

			if (lpa < 0)
				return lpa;

			/* If both registers are equal, it is suspect but not
			 * impossible, hence a new try
			 */
		} while (lpa == adv && retry--);

		mii_lpa_to_linkmode_lpa_t(phydev->lp_advertising, lpa);

		lpa &= adv;

		phydev->speed = SPEED_10;
		phydev->duplex = DUPLEX_HALF;
		phydev->pause = phydev->asym_pause = 0;

		if (lpa & (LPA_100FULL | LPA_100HALF)) {
			phydev->speed = SPEED_100;

			if (lpa & LPA_100FULL)
				phydev->duplex = DUPLEX_FULL;
		} else {
			if (lpa & LPA_10FULL)
				phydev->duplex = DUPLEX_FULL;
		}

		phy_resolve_aneg_pause(phydev);
	} else {
		err = genphy_read_status_fixed(phydev);
		if (err < 0)
			return err;

		phydev->pause = phydev->asym_pause = 0;
		linkmode_zero(phydev->lp_advertising);
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int lxt973_probe(struct phy_device *phydev)
{
	int val = phy_read(phydev, MII_LXT973_PCR);

	if (val & PCR_FIBER_SELECT) {
		/*
		 * If fiber is selected, then the only correct setting
		 * is 100Mbps, full duplex, and auto negotiation off.
		 */
		val = phy_read(phydev, MII_BMCR);
		val |= (BMCR_SPEED100 | BMCR_FULLDPLX);
		val &= ~BMCR_ANENABLE;
		phy_write(phydev, MII_BMCR, val);
		/* Remember that the port is in fiber mode. */
		phydev->priv = lxt973_probe;
<<<<<<< HEAD
=======
		phydev->port = PORT_FIBRE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		phydev->priv = NULL;
	}
	return 0;
}

static int lxt973_config_aneg(struct phy_device *phydev)
{
	/* Do nothing if port is in fiber mode. */
	return phydev->priv ? 0 : genphy_config_aneg(phydev);
}

<<<<<<< HEAD
static struct phy_driver lxt970_driver = {
	.phy_id		= 0x78100000,
	.name		= "LXT970",
	.phy_id_mask	= 0xfffffff0,
	.features	= PHY_BASIC_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= lxt970_config_init,
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.ack_interrupt	= lxt970_ack_interrupt,
	.config_intr	= lxt970_config_intr,
	.driver 	= { .owner = THIS_MODULE,},
};

static struct phy_driver lxt971_driver = {
	.phy_id		= 0x001378e0,
	.name		= "LXT971",
	.phy_id_mask	= 0xfffffff0,
	.features	= PHY_BASIC_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.ack_interrupt	= lxt971_ack_interrupt,
	.config_intr	= lxt971_config_intr,
	.driver 	= { .owner = THIS_MODULE,},
};

static struct phy_driver lxt973_driver = {
	.phy_id		= 0x00137a10,
	.name		= "LXT973",
	.phy_id_mask	= 0xfffffff0,
	.features	= PHY_BASIC_FEATURES,
	.flags		= 0,
	.probe		= lxt973_probe,
	.config_aneg	= lxt973_config_aneg,
	.read_status	= genphy_read_status,
	.driver 	= { .owner = THIS_MODULE,},
};

static int __init lxt_init(void)
{
	int ret;

	ret = phy_driver_register(&lxt970_driver);
	if (ret)
		goto err1;

	ret = phy_driver_register(&lxt971_driver);
	if (ret)
		goto err2;

	ret = phy_driver_register(&lxt973_driver);
	if (ret)
		goto err3;
	return 0;

 err3:
	phy_driver_unregister(&lxt971_driver);
 err2:
	phy_driver_unregister(&lxt970_driver);
 err1:
	return ret;
}

static void __exit lxt_exit(void)
{
	phy_driver_unregister(&lxt970_driver);
	phy_driver_unregister(&lxt971_driver);
	phy_driver_unregister(&lxt973_driver);
}

module_init(lxt_init);
module_exit(lxt_exit);
=======
static struct phy_driver lxt97x_driver[] = {
{
	.phy_id		= 0x78100000,
	.name		= "LXT970",
	.phy_id_mask	= 0xfffffff0,
	/* PHY_BASIC_FEATURES */
	.config_init	= lxt970_config_init,
	.config_intr	= lxt970_config_intr,
	.handle_interrupt = lxt970_handle_interrupt,
}, {
	.phy_id		= 0x001378e0,
	.name		= "LXT971",
	.phy_id_mask	= 0xfffffff0,
	/* PHY_BASIC_FEATURES */
	.config_intr	= lxt971_config_intr,
	.handle_interrupt = lxt971_handle_interrupt,
	.suspend	= genphy_suspend,
	.resume		= genphy_resume,
}, {
	.phy_id		= 0x00137a10,
	.name		= "LXT973-A2",
	.phy_id_mask	= 0xffffffff,
	/* PHY_BASIC_FEATURES */
	.flags		= 0,
	.probe		= lxt973_probe,
	.config_aneg	= lxt973_config_aneg,
	.read_status	= lxt973a2_read_status,
	.suspend	= genphy_suspend,
	.resume		= genphy_resume,
}, {
	.phy_id		= 0x00137a10,
	.name		= "LXT973",
	.phy_id_mask	= 0xfffffff0,
	/* PHY_BASIC_FEATURES */
	.flags		= 0,
	.probe		= lxt973_probe,
	.config_aneg	= lxt973_config_aneg,
	.suspend	= genphy_suspend,
	.resume		= genphy_resume,
} };

module_phy_driver(lxt97x_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct mdio_device_id __maybe_unused lxt_tbl[] = {
	{ 0x78100000, 0xfffffff0 },
	{ 0x001378e0, 0xfffffff0 },
	{ 0x00137a10, 0xfffffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, lxt_tbl);
