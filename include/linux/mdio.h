<<<<<<< HEAD
/*
 * linux/mdio.h: definitions for MDIO (clause 45) transceivers
 * Copyright 2006-2009 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef __LINUX_MDIO_H__
#define __LINUX_MDIO_H__

#include <linux/types.h>
#include <linux/mii.h>

/* MDIO Manageable Devices (MMDs). */
#define MDIO_MMD_PMAPMD		1	/* Physical Medium Attachment/
					 * Physical Medium Dependent */
#define MDIO_MMD_WIS		2	/* WAN Interface Sublayer */
#define MDIO_MMD_PCS		3	/* Physical Coding Sublayer */
#define MDIO_MMD_PHYXS		4	/* PHY Extender Sublayer */
#define MDIO_MMD_DTEXS		5	/* DTE Extender Sublayer */
#define MDIO_MMD_TC		6	/* Transmission Convergence */
#define MDIO_MMD_AN		7	/* Auto-Negotiation */
#define MDIO_MMD_C22EXT		29	/* Clause 22 extension */
#define MDIO_MMD_VEND1		30	/* Vendor specific 1 */
#define MDIO_MMD_VEND2		31	/* Vendor specific 2 */

/* Generic MDIO registers. */
#define MDIO_CTRL1		MII_BMCR
#define MDIO_STAT1		MII_BMSR
#define MDIO_DEVID1		MII_PHYSID1
#define MDIO_DEVID2		MII_PHYSID2
#define MDIO_SPEED		4	/* Speed ability */
#define MDIO_DEVS1		5	/* Devices in package */
#define MDIO_DEVS2		6
#define MDIO_CTRL2		7	/* 10G control 2 */
#define MDIO_STAT2		8	/* 10G status 2 */
#define MDIO_PMA_TXDIS		9	/* 10G PMA/PMD transmit disable */
#define MDIO_PMA_RXDET		10	/* 10G PMA/PMD receive signal detect */
#define MDIO_PMA_EXTABLE	11	/* 10G PMA/PMD extended ability */
#define MDIO_PKGID1		14	/* Package identifier */
#define MDIO_PKGID2		15
#define MDIO_AN_ADVERTISE	16	/* AN advertising (base page) */
#define MDIO_AN_LPA		19	/* AN LP abilities (base page) */
#define MDIO_PHYXS_LNSTAT	24	/* PHY XGXS lane state */

/* Media-dependent registers. */
#define MDIO_PMA_10GBT_SWAPPOL	130	/* 10GBASE-T pair swap & polarity */
#define MDIO_PMA_10GBT_TXPWR	131	/* 10GBASE-T TX power control */
#define MDIO_PMA_10GBT_SNR	133	/* 10GBASE-T SNR margin, lane A.
					 * Lanes B-D are numbered 134-136. */
#define MDIO_PMA_10GBR_FECABLE	170	/* 10GBASE-R FEC ability */
#define MDIO_PCS_10GBX_STAT1	24	/* 10GBASE-X PCS status 1 */
#define MDIO_PCS_10GBRT_STAT1	32	/* 10GBASE-R/-T PCS status 1 */
#define MDIO_PCS_10GBRT_STAT2	33	/* 10GBASE-R/-T PCS status 2 */
#define MDIO_AN_10GBT_CTRL	32	/* 10GBASE-T auto-negotiation control */
#define MDIO_AN_10GBT_STAT	33	/* 10GBASE-T auto-negotiation status */
#define MDIO_AN_EEE_ADV		60	/* EEE advertisement */

/* LASI (Link Alarm Status Interrupt) registers, defined by XENPAK MSA. */
#define MDIO_PMA_LASI_RXCTRL	0x9000	/* RX_ALARM control */
#define MDIO_PMA_LASI_TXCTRL	0x9001	/* TX_ALARM control */
#define MDIO_PMA_LASI_CTRL	0x9002	/* LASI control */
#define MDIO_PMA_LASI_RXSTAT	0x9003	/* RX_ALARM status */
#define MDIO_PMA_LASI_TXSTAT	0x9004	/* TX_ALARM status */
#define MDIO_PMA_LASI_STAT	0x9005	/* LASI status */

/* Control register 1. */
/* Enable extended speed selection */
#define MDIO_CTRL1_SPEEDSELEXT		(BMCR_SPEED1000 | BMCR_SPEED100)
/* All speed selection bits */
#define MDIO_CTRL1_SPEEDSEL		(MDIO_CTRL1_SPEEDSELEXT | 0x003c)
#define MDIO_CTRL1_FULLDPLX		BMCR_FULLDPLX
#define MDIO_CTRL1_LPOWER		BMCR_PDOWN
#define MDIO_CTRL1_RESET		BMCR_RESET
#define MDIO_PMA_CTRL1_LOOPBACK		0x0001
#define MDIO_PMA_CTRL1_SPEED1000	BMCR_SPEED1000
#define MDIO_PMA_CTRL1_SPEED100		BMCR_SPEED100
#define MDIO_PCS_CTRL1_LOOPBACK		BMCR_LOOPBACK
#define MDIO_PHYXS_CTRL1_LOOPBACK	BMCR_LOOPBACK
#define MDIO_AN_CTRL1_RESTART		BMCR_ANRESTART
#define MDIO_AN_CTRL1_ENABLE		BMCR_ANENABLE
#define MDIO_AN_CTRL1_XNP		0x2000	/* Enable extended next page */

/* 10 Gb/s */
#define MDIO_CTRL1_SPEED10G		(MDIO_CTRL1_SPEEDSELEXT | 0x00)
/* 10PASS-TS/2BASE-TL */
#define MDIO_CTRL1_SPEED10P2B		(MDIO_CTRL1_SPEEDSELEXT | 0x04)

/* Status register 1. */
#define MDIO_STAT1_LPOWERABLE		0x0002	/* Low-power ability */
#define MDIO_STAT1_LSTATUS		BMSR_LSTATUS
#define MDIO_STAT1_FAULT		0x0080	/* Fault */
#define MDIO_AN_STAT1_LPABLE		0x0001	/* Link partner AN ability */
#define MDIO_AN_STAT1_ABLE		BMSR_ANEGCAPABLE
#define MDIO_AN_STAT1_RFAULT		BMSR_RFAULT
#define MDIO_AN_STAT1_COMPLETE		BMSR_ANEGCOMPLETE
#define MDIO_AN_STAT1_PAGE		0x0040	/* Page received */
#define MDIO_AN_STAT1_XNP		0x0080	/* Extended next page status */

/* Speed register. */
#define MDIO_SPEED_10G			0x0001	/* 10G capable */
#define MDIO_PMA_SPEED_2B		0x0002	/* 2BASE-TL capable */
#define MDIO_PMA_SPEED_10P		0x0004	/* 10PASS-TS capable */
#define MDIO_PMA_SPEED_1000		0x0010	/* 1000M capable */
#define MDIO_PMA_SPEED_100		0x0020	/* 100M capable */
#define MDIO_PMA_SPEED_10		0x0040	/* 10M capable */
#define MDIO_PCS_SPEED_10P2B		0x0002	/* 10PASS-TS/2BASE-TL capable */

/* Device present registers. */
#define MDIO_DEVS_PRESENT(devad)	(1 << (devad))
#define MDIO_DEVS_PMAPMD		MDIO_DEVS_PRESENT(MDIO_MMD_PMAPMD)
#define MDIO_DEVS_WIS			MDIO_DEVS_PRESENT(MDIO_MMD_WIS)
#define MDIO_DEVS_PCS			MDIO_DEVS_PRESENT(MDIO_MMD_PCS)
#define MDIO_DEVS_PHYXS			MDIO_DEVS_PRESENT(MDIO_MMD_PHYXS)
#define MDIO_DEVS_DTEXS			MDIO_DEVS_PRESENT(MDIO_MMD_DTEXS)
#define MDIO_DEVS_TC			MDIO_DEVS_PRESENT(MDIO_MMD_TC)
#define MDIO_DEVS_AN			MDIO_DEVS_PRESENT(MDIO_MMD_AN)
#define MDIO_DEVS_C22EXT		MDIO_DEVS_PRESENT(MDIO_MMD_C22EXT)

/* Control register 2. */
#define MDIO_PMA_CTRL2_TYPE		0x000f	/* PMA/PMD type selection */
#define MDIO_PMA_CTRL2_10GBCX4		0x0000	/* 10GBASE-CX4 type */
#define MDIO_PMA_CTRL2_10GBEW		0x0001	/* 10GBASE-EW type */
#define MDIO_PMA_CTRL2_10GBLW		0x0002	/* 10GBASE-LW type */
#define MDIO_PMA_CTRL2_10GBSW		0x0003	/* 10GBASE-SW type */
#define MDIO_PMA_CTRL2_10GBLX4		0x0004	/* 10GBASE-LX4 type */
#define MDIO_PMA_CTRL2_10GBER		0x0005	/* 10GBASE-ER type */
#define MDIO_PMA_CTRL2_10GBLR		0x0006	/* 10GBASE-LR type */
#define MDIO_PMA_CTRL2_10GBSR		0x0007	/* 10GBASE-SR type */
#define MDIO_PMA_CTRL2_10GBLRM		0x0008	/* 10GBASE-LRM type */
#define MDIO_PMA_CTRL2_10GBT		0x0009	/* 10GBASE-T type */
#define MDIO_PMA_CTRL2_10GBKX4		0x000a	/* 10GBASE-KX4 type */
#define MDIO_PMA_CTRL2_10GBKR		0x000b	/* 10GBASE-KR type */
#define MDIO_PMA_CTRL2_1000BT		0x000c	/* 1000BASE-T type */
#define MDIO_PMA_CTRL2_1000BKX		0x000d	/* 1000BASE-KX type */
#define MDIO_PMA_CTRL2_100BTX		0x000e	/* 100BASE-TX type */
#define MDIO_PMA_CTRL2_10BT		0x000f	/* 10BASE-T type */
#define MDIO_PCS_CTRL2_TYPE		0x0003	/* PCS type selection */
#define MDIO_PCS_CTRL2_10GBR		0x0000	/* 10GBASE-R type */
#define MDIO_PCS_CTRL2_10GBX		0x0001	/* 10GBASE-X type */
#define MDIO_PCS_CTRL2_10GBW		0x0002	/* 10GBASE-W type */
#define MDIO_PCS_CTRL2_10GBT		0x0003	/* 10GBASE-T type */

/* Status register 2. */
#define MDIO_STAT2_RXFAULT		0x0400	/* Receive fault */
#define MDIO_STAT2_TXFAULT		0x0800	/* Transmit fault */
#define MDIO_STAT2_DEVPRST		0xc000	/* Device present */
#define MDIO_STAT2_DEVPRST_VAL		0x8000	/* Device present value */
#define MDIO_PMA_STAT2_LBABLE		0x0001	/* PMA loopback ability */
#define MDIO_PMA_STAT2_10GBEW		0x0002	/* 10GBASE-EW ability */
#define MDIO_PMA_STAT2_10GBLW		0x0004	/* 10GBASE-LW ability */
#define MDIO_PMA_STAT2_10GBSW		0x0008	/* 10GBASE-SW ability */
#define MDIO_PMA_STAT2_10GBLX4		0x0010	/* 10GBASE-LX4 ability */
#define MDIO_PMA_STAT2_10GBER		0x0020	/* 10GBASE-ER ability */
#define MDIO_PMA_STAT2_10GBLR		0x0040	/* 10GBASE-LR ability */
#define MDIO_PMA_STAT2_10GBSR		0x0080	/* 10GBASE-SR ability */
#define MDIO_PMD_STAT2_TXDISAB		0x0100	/* PMD TX disable ability */
#define MDIO_PMA_STAT2_EXTABLE		0x0200	/* Extended abilities */
#define MDIO_PMA_STAT2_RXFLTABLE	0x1000	/* Receive fault ability */
#define MDIO_PMA_STAT2_TXFLTABLE	0x2000	/* Transmit fault ability */
#define MDIO_PCS_STAT2_10GBR		0x0001	/* 10GBASE-R capable */
#define MDIO_PCS_STAT2_10GBX		0x0002	/* 10GBASE-X capable */
#define MDIO_PCS_STAT2_10GBW		0x0004	/* 10GBASE-W capable */
#define MDIO_PCS_STAT2_RXFLTABLE	0x1000	/* Receive fault ability */
#define MDIO_PCS_STAT2_TXFLTABLE	0x2000	/* Transmit fault ability */

/* Transmit disable register. */
#define MDIO_PMD_TXDIS_GLOBAL		0x0001	/* Global PMD TX disable */
#define MDIO_PMD_TXDIS_0		0x0002	/* PMD TX disable 0 */
#define MDIO_PMD_TXDIS_1		0x0004	/* PMD TX disable 1 */
#define MDIO_PMD_TXDIS_2		0x0008	/* PMD TX disable 2 */
#define MDIO_PMD_TXDIS_3		0x0010	/* PMD TX disable 3 */

/* Receive signal detect register. */
#define MDIO_PMD_RXDET_GLOBAL		0x0001	/* Global PMD RX signal detect */
#define MDIO_PMD_RXDET_0		0x0002	/* PMD RX signal detect 0 */
#define MDIO_PMD_RXDET_1		0x0004	/* PMD RX signal detect 1 */
#define MDIO_PMD_RXDET_2		0x0008	/* PMD RX signal detect 2 */
#define MDIO_PMD_RXDET_3		0x0010	/* PMD RX signal detect 3 */

/* Extended abilities register. */
#define MDIO_PMA_EXTABLE_10GCX4		0x0001	/* 10GBASE-CX4 ability */
#define MDIO_PMA_EXTABLE_10GBLRM	0x0002	/* 10GBASE-LRM ability */
#define MDIO_PMA_EXTABLE_10GBT		0x0004	/* 10GBASE-T ability */
#define MDIO_PMA_EXTABLE_10GBKX4	0x0008	/* 10GBASE-KX4 ability */
#define MDIO_PMA_EXTABLE_10GBKR		0x0010	/* 10GBASE-KR ability */
#define MDIO_PMA_EXTABLE_1000BT		0x0020	/* 1000BASE-T ability */
#define MDIO_PMA_EXTABLE_1000BKX	0x0040	/* 1000BASE-KX ability */
#define MDIO_PMA_EXTABLE_100BTX		0x0080	/* 100BASE-TX ability */
#define MDIO_PMA_EXTABLE_10BT		0x0100	/* 10BASE-T ability */

/* PHY XGXS lane state register. */
#define MDIO_PHYXS_LNSTAT_SYNC0		0x0001
#define MDIO_PHYXS_LNSTAT_SYNC1		0x0002
#define MDIO_PHYXS_LNSTAT_SYNC2		0x0004
#define MDIO_PHYXS_LNSTAT_SYNC3		0x0008
#define MDIO_PHYXS_LNSTAT_ALIGN		0x1000

/* PMA 10GBASE-T pair swap & polarity */
#define MDIO_PMA_10GBT_SWAPPOL_ABNX	0x0001	/* Pair A/B uncrossed */
#define MDIO_PMA_10GBT_SWAPPOL_CDNX	0x0002	/* Pair C/D uncrossed */
#define MDIO_PMA_10GBT_SWAPPOL_AREV	0x0100	/* Pair A polarity reversed */
#define MDIO_PMA_10GBT_SWAPPOL_BREV	0x0200	/* Pair B polarity reversed */
#define MDIO_PMA_10GBT_SWAPPOL_CREV	0x0400	/* Pair C polarity reversed */
#define MDIO_PMA_10GBT_SWAPPOL_DREV	0x0800	/* Pair D polarity reversed */

/* PMA 10GBASE-T TX power register. */
#define MDIO_PMA_10GBT_TXPWR_SHORT	0x0001	/* Short-reach mode */

/* PMA 10GBASE-T SNR registers. */
/* Value is SNR margin in dB, clamped to range [-127, 127], plus 0x8000. */
#define MDIO_PMA_10GBT_SNR_BIAS		0x8000
#define MDIO_PMA_10GBT_SNR_MAX		127

/* PMA 10GBASE-R FEC ability register. */
#define MDIO_PMA_10GBR_FECABLE_ABLE	0x0001	/* FEC ability */
#define MDIO_PMA_10GBR_FECABLE_ERRABLE	0x0002	/* FEC error indic. ability */

/* PCS 10GBASE-R/-T status register 1. */
#define MDIO_PCS_10GBRT_STAT1_BLKLK	0x0001	/* Block lock attained */

/* PCS 10GBASE-R/-T status register 2. */
#define MDIO_PCS_10GBRT_STAT2_ERR	0x00ff
#define MDIO_PCS_10GBRT_STAT2_BER	0x3f00

/* AN 10GBASE-T control register. */
#define MDIO_AN_10GBT_CTRL_ADV10G	0x1000	/* Advertise 10GBASE-T */

/* AN 10GBASE-T status register. */
#define MDIO_AN_10GBT_STAT_LPTRR	0x0200	/* LP training reset req. */
#define MDIO_AN_10GBT_STAT_LPLTABLE	0x0400	/* LP loop timing ability */
#define MDIO_AN_10GBT_STAT_LP10G	0x0800	/* LP is 10GBT capable */
#define MDIO_AN_10GBT_STAT_REMOK	0x1000	/* Remote OK */
#define MDIO_AN_10GBT_STAT_LOCOK	0x2000	/* Local OK */
#define MDIO_AN_10GBT_STAT_MS		0x4000	/* Master/slave config */
#define MDIO_AN_10GBT_STAT_MSFLT	0x8000	/* Master/slave config fault */

/* AN EEE Advertisement register. */
#define MDIO_AN_EEE_ADV_100TX		0x0002	/* Advertise 100TX EEE cap */
#define MDIO_AN_EEE_ADV_1000T		0x0004	/* Advertise 1000T EEE cap */

/* LASI RX_ALARM control/status registers. */
#define MDIO_PMA_LASI_RX_PHYXSLFLT	0x0001	/* PHY XS RX local fault */
#define MDIO_PMA_LASI_RX_PCSLFLT	0x0008	/* PCS RX local fault */
#define MDIO_PMA_LASI_RX_PMALFLT	0x0010	/* PMA/PMD RX local fault */
#define MDIO_PMA_LASI_RX_OPTICPOWERFLT	0x0020	/* RX optical power fault */
#define MDIO_PMA_LASI_RX_WISLFLT	0x0200	/* WIS local fault */

/* LASI TX_ALARM control/status registers. */
#define MDIO_PMA_LASI_TX_PHYXSLFLT	0x0001	/* PHY XS TX local fault */
#define MDIO_PMA_LASI_TX_PCSLFLT	0x0008	/* PCS TX local fault */
#define MDIO_PMA_LASI_TX_PMALFLT	0x0010	/* PMA/PMD TX local fault */
#define MDIO_PMA_LASI_TX_LASERPOWERFLT	0x0080	/* Laser output power fault */
#define MDIO_PMA_LASI_TX_LASERTEMPFLT	0x0100	/* Laser temperature fault */
#define MDIO_PMA_LASI_TX_LASERBICURRFLT	0x0200	/* Laser bias current fault */

/* LASI control/status registers. */
#define MDIO_PMA_LASI_LSALARM		0x0001	/* LS_ALARM enable/status */
#define MDIO_PMA_LASI_TXALARM		0x0002	/* TX_ALARM enable/status */
#define MDIO_PMA_LASI_RXALARM		0x0004	/* RX_ALARM enable/status */

/* Mapping between MDIO PRTAD/DEVAD and mii_ioctl_data::phy_id */

#define MDIO_PHY_ID_C45			0x8000
#define MDIO_PHY_ID_PRTAD		0x03e0
#define MDIO_PHY_ID_DEVAD		0x001f
#define MDIO_PHY_ID_C45_MASK						\
	(MDIO_PHY_ID_C45 | MDIO_PHY_ID_PRTAD | MDIO_PHY_ID_DEVAD)

static inline __u16 mdio_phy_id_c45(int prtad, int devad)
{
	return MDIO_PHY_ID_C45 | (prtad << 5) | devad;
}

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * linux/mdio.h: definitions for MDIO (clause 45) transceivers
 * Copyright 2006-2009 Solarflare Communications Inc.
 */
#ifndef __LINUX_MDIO_H__
#define __LINUX_MDIO_H__

#include <uapi/linux/mdio.h>
#include <linux/bitfield.h>
#include <linux/mod_devicetable.h>

struct gpio_desc;
struct mii_bus;
struct reset_control;

/* Multiple levels of nesting are possible. However typically this is
 * limited to nested DSA like layer, a MUX layer, and the normal
 * user. Instead of trying to handle the general case, just define
 * these cases.
 */
enum mdio_mutex_lock_class {
	MDIO_MUTEX_NORMAL,
	MDIO_MUTEX_MUX,
	MDIO_MUTEX_NESTED,
};

struct mdio_device {
	struct device dev;

	struct mii_bus *bus;
	char modalias[MDIO_NAME_SIZE];

	int (*bus_match)(struct device *dev, struct device_driver *drv);
	void (*device_free)(struct mdio_device *mdiodev);
	void (*device_remove)(struct mdio_device *mdiodev);

	/* Bus address of the MDIO device (0-31) */
	int addr;
	int flags;
	int reset_state;
	struct gpio_desc *reset_gpio;
	struct reset_control *reset_ctrl;
	unsigned int reset_assert_delay;
	unsigned int reset_deassert_delay;
};

static inline struct mdio_device *to_mdio_device(const struct device *dev)
{
	return container_of(dev, struct mdio_device, dev);
}

/* struct mdio_driver_common: Common to all MDIO drivers */
struct mdio_driver_common {
	struct device_driver driver;
	int flags;
};
#define MDIO_DEVICE_FLAG_PHY		1

static inline struct mdio_driver_common *
to_mdio_common_driver(const struct device_driver *driver)
{
	return container_of(driver, struct mdio_driver_common, driver);
}

/* struct mdio_driver: Generic MDIO driver */
struct mdio_driver {
	struct mdio_driver_common mdiodrv;

	/*
	 * Called during discovery.  Used to set
	 * up device-specific structures, if any
	 */
	int (*probe)(struct mdio_device *mdiodev);

	/* Clears up any memory if needed */
	void (*remove)(struct mdio_device *mdiodev);

	/* Quiesces the device on system shutdown, turns off interrupts etc */
	void (*shutdown)(struct mdio_device *mdiodev);
};

static inline struct mdio_driver *
to_mdio_driver(const struct device_driver *driver)
{
	return container_of(to_mdio_common_driver(driver), struct mdio_driver,
			    mdiodrv);
}

/* device driver data */
static inline void mdiodev_set_drvdata(struct mdio_device *mdio, void *data)
{
	dev_set_drvdata(&mdio->dev, data);
}

static inline void *mdiodev_get_drvdata(struct mdio_device *mdio)
{
	return dev_get_drvdata(&mdio->dev);
}

void mdio_device_free(struct mdio_device *mdiodev);
struct mdio_device *mdio_device_create(struct mii_bus *bus, int addr);
int mdio_device_register(struct mdio_device *mdiodev);
void mdio_device_remove(struct mdio_device *mdiodev);
void mdio_device_reset(struct mdio_device *mdiodev, int value);
int mdio_driver_register(struct mdio_driver *drv);
void mdio_driver_unregister(struct mdio_driver *drv);
int mdio_device_bus_match(struct device *dev, struct device_driver *drv);

static inline void mdio_device_get(struct mdio_device *mdiodev)
{
	get_device(&mdiodev->dev);
}

static inline void mdio_device_put(struct mdio_device *mdiodev)
{
	mdio_device_free(mdiodev);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline bool mdio_phy_id_is_c45(int phy_id)
{
	return (phy_id & MDIO_PHY_ID_C45) && !(phy_id & ~MDIO_PHY_ID_C45_MASK);
}

static inline __u16 mdio_phy_id_prtad(int phy_id)
{
	return (phy_id & MDIO_PHY_ID_PRTAD) >> 5;
}

static inline __u16 mdio_phy_id_devad(int phy_id)
{
	return phy_id & MDIO_PHY_ID_DEVAD;
}

/**
 * struct mdio_if_info - Ethernet controller MDIO interface
 * @prtad: PRTAD of the PHY (%MDIO_PRTAD_NONE if not present/unknown)
 * @mmds: Mask of MMDs expected to be present in the PHY.  This must be
 *	non-zero unless @prtad = %MDIO_PRTAD_NONE.
 * @mode_support: MDIO modes supported.  If %MDIO_SUPPORTS_C22 is set then
 *	MII register access will be passed through with @devad =
 *	%MDIO_DEVAD_NONE.  If %MDIO_EMULATE_C22 is set then access to
 *	commonly used clause 22 registers will be translated into
 *	clause 45 registers.
 * @dev: Net device structure
 * @mdio_read: Register read function; returns value or negative error code
 * @mdio_write: Register write function; returns 0 or negative error code
 */
struct mdio_if_info {
	int prtad;
	u32 mmds;
	unsigned mode_support;

	struct net_device *dev;
	int (*mdio_read)(struct net_device *dev, int prtad, int devad,
			 u16 addr);
	int (*mdio_write)(struct net_device *dev, int prtad, int devad,
			  u16 addr, u16 val);
};

#define MDIO_PRTAD_NONE			(-1)
#define MDIO_DEVAD_NONE			(-1)
#define MDIO_SUPPORTS_C22		1
#define MDIO_SUPPORTS_C45		2
#define MDIO_EMULATE_C22		4

struct ethtool_cmd;
struct ethtool_pauseparam;
extern int mdio45_probe(struct mdio_if_info *mdio, int prtad);
extern int mdio_set_flag(const struct mdio_if_info *mdio,
			 int prtad, int devad, u16 addr, int mask,
			 bool sense);
extern int mdio45_links_ok(const struct mdio_if_info *mdio, u32 mmds);
extern int mdio45_nway_restart(const struct mdio_if_info *mdio);
extern void mdio45_ethtool_gset_npage(const struct mdio_if_info *mdio,
				      struct ethtool_cmd *ecmd,
				      u32 npage_adv, u32 npage_lpa);
extern void
<<<<<<< HEAD
mdio45_ethtool_spauseparam_an(const struct mdio_if_info *mdio,
			      const struct ethtool_pauseparam *ecmd);
=======
mdio45_ethtool_ksettings_get_npage(const struct mdio_if_info *mdio,
				   struct ethtool_link_ksettings *cmd,
				   u32 npage_adv, u32 npage_lpa);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * mdio45_ethtool_gset - get settings for ETHTOOL_GSET
 * @mdio: MDIO interface
 * @ecmd: Ethtool request structure
 *
 * Since the CSRs for auto-negotiation using next pages are not fully
 * standardised, this function does not attempt to decode them.  Use
 * mdio45_ethtool_gset_npage() to specify advertisement bits from next
 * pages.
 */
static inline void mdio45_ethtool_gset(const struct mdio_if_info *mdio,
				       struct ethtool_cmd *ecmd)
{
	mdio45_ethtool_gset_npage(mdio, ecmd, 0, 0);
}

<<<<<<< HEAD
extern int mdio_mii_ioctl(const struct mdio_if_info *mdio,
			  struct mii_ioctl_data *mii_data, int cmd);

#endif /* __KERNEL__ */
=======
/**
 * mdio45_ethtool_ksettings_get - get settings for ETHTOOL_GLINKSETTINGS
 * @mdio: MDIO interface
 * @cmd: Ethtool request structure
 *
 * Since the CSRs for auto-negotiation using next pages are not fully
 * standardised, this function does not attempt to decode them.  Use
 * mdio45_ethtool_ksettings_get_npage() to specify advertisement bits
 * from next pages.
 */
static inline void
mdio45_ethtool_ksettings_get(const struct mdio_if_info *mdio,
			     struct ethtool_link_ksettings *cmd)
{
	mdio45_ethtool_ksettings_get_npage(mdio, cmd, 0, 0);
}

extern int mdio_mii_ioctl(const struct mdio_if_info *mdio,
			  struct mii_ioctl_data *mii_data, int cmd);

/**
 * mmd_eee_cap_to_ethtool_sup_t
 * @eee_cap: value of the MMD EEE Capability register
 *
 * A small helper function that translates MMD EEE Capability (3.20) bits
 * to ethtool supported settings.
 */
static inline u32 mmd_eee_cap_to_ethtool_sup_t(u16 eee_cap)
{
	u32 supported = 0;

	if (eee_cap & MDIO_EEE_100TX)
		supported |= SUPPORTED_100baseT_Full;
	if (eee_cap & MDIO_EEE_1000T)
		supported |= SUPPORTED_1000baseT_Full;
	if (eee_cap & MDIO_EEE_10GT)
		supported |= SUPPORTED_10000baseT_Full;
	if (eee_cap & MDIO_EEE_1000KX)
		supported |= SUPPORTED_1000baseKX_Full;
	if (eee_cap & MDIO_EEE_10GKX4)
		supported |= SUPPORTED_10000baseKX4_Full;
	if (eee_cap & MDIO_EEE_10GKR)
		supported |= SUPPORTED_10000baseKR_Full;

	return supported;
}

/**
 * mmd_eee_adv_to_ethtool_adv_t
 * @eee_adv: value of the MMD EEE Advertisement/Link Partner Ability registers
 *
 * A small helper function that translates the MMD EEE Advertisment (7.60)
 * and MMD EEE Link Partner Ability (7.61) bits to ethtool advertisement
 * settings.
 */
static inline u32 mmd_eee_adv_to_ethtool_adv_t(u16 eee_adv)
{
	u32 adv = 0;

	if (eee_adv & MDIO_EEE_100TX)
		adv |= ADVERTISED_100baseT_Full;
	if (eee_adv & MDIO_EEE_1000T)
		adv |= ADVERTISED_1000baseT_Full;
	if (eee_adv & MDIO_EEE_10GT)
		adv |= ADVERTISED_10000baseT_Full;
	if (eee_adv & MDIO_EEE_1000KX)
		adv |= ADVERTISED_1000baseKX_Full;
	if (eee_adv & MDIO_EEE_10GKX4)
		adv |= ADVERTISED_10000baseKX4_Full;
	if (eee_adv & MDIO_EEE_10GKR)
		adv |= ADVERTISED_10000baseKR_Full;

	return adv;
}

/**
 * ethtool_adv_to_mmd_eee_adv_t
 * @adv: the ethtool advertisement settings
 *
 * A small helper function that translates ethtool advertisement settings
 * to EEE advertisements for the MMD EEE Advertisement (7.60) and
 * MMD EEE Link Partner Ability (7.61) registers.
 */
static inline u16 ethtool_adv_to_mmd_eee_adv_t(u32 adv)
{
	u16 reg = 0;

	if (adv & ADVERTISED_100baseT_Full)
		reg |= MDIO_EEE_100TX;
	if (adv & ADVERTISED_1000baseT_Full)
		reg |= MDIO_EEE_1000T;
	if (adv & ADVERTISED_10000baseT_Full)
		reg |= MDIO_EEE_10GT;
	if (adv & ADVERTISED_1000baseKX_Full)
		reg |= MDIO_EEE_1000KX;
	if (adv & ADVERTISED_10000baseKX4_Full)
		reg |= MDIO_EEE_10GKX4;
	if (adv & ADVERTISED_10000baseKR_Full)
		reg |= MDIO_EEE_10GKR;

	return reg;
}

/**
 * linkmode_adv_to_mii_10gbt_adv_t
 * @advertising: the linkmode advertisement settings
 *
 * A small helper function that translates linkmode advertisement
 * settings to phy autonegotiation advertisements for the C45
 * 10GBASE-T AN CONTROL (7.32) register.
 */
static inline u32 linkmode_adv_to_mii_10gbt_adv_t(unsigned long *advertising)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
			      advertising))
		result |= MDIO_AN_10GBT_CTRL_ADV2_5G;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT,
			      advertising))
		result |= MDIO_AN_10GBT_CTRL_ADV5G;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			      advertising))
		result |= MDIO_AN_10GBT_CTRL_ADV10G;

	return result;
}

/**
 * mii_10gbt_stat_mod_linkmode_lpa_t
 * @advertising: target the linkmode advertisement settings
 * @lpa: value of the C45 10GBASE-T AN STATUS register
 *
 * A small helper function that translates C45 10GBASE-T AN STATUS register bits
 * to linkmode advertisement settings. Other bits in advertising aren't changed.
 */
static inline void mii_10gbt_stat_mod_linkmode_lpa_t(unsigned long *advertising,
						     u32 lpa)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
			 advertising, lpa & MDIO_AN_10GBT_STAT_LP2_5G);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT,
			 advertising, lpa & MDIO_AN_10GBT_STAT_LP5G);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			 advertising, lpa & MDIO_AN_10GBT_STAT_LP10G);
}

/**
 * mii_t1_adv_l_mod_linkmode_t
 * @advertising: target the linkmode advertisement settings
 * @lpa: value of the BASE-T1 Autonegotiation Advertisement [15:0] Register
 *
 * A small helper function that translates BASE-T1 Autonegotiation
 * Advertisement [15:0] Register bits to linkmode advertisement settings.
 * Other bits in advertising aren't changed.
 */
static inline void mii_t1_adv_l_mod_linkmode_t(unsigned long *advertising, u32 lpa)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_Pause_BIT, advertising,
			 lpa & MDIO_AN_T1_ADV_L_PAUSE_CAP);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, advertising,
			 lpa & MDIO_AN_T1_ADV_L_PAUSE_ASYM);
}

/**
 * mii_t1_adv_m_mod_linkmode_t
 * @advertising: target the linkmode advertisement settings
 * @lpa: value of the BASE-T1 Autonegotiation Advertisement [31:16] Register
 *
 * A small helper function that translates BASE-T1 Autonegotiation
 * Advertisement [31:16] Register bits to linkmode advertisement settings.
 * Other bits in advertising aren't changed.
 */
static inline void mii_t1_adv_m_mod_linkmode_t(unsigned long *advertising, u32 lpa)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10baseT1L_Full_BIT,
			 advertising, lpa & MDIO_AN_T1_ADV_M_B10L);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_100baseT1_Full_BIT,
			 advertising, lpa & MDIO_AN_T1_ADV_M_100BT1);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_1000baseT1_Full_BIT,
			 advertising, lpa & MDIO_AN_T1_ADV_M_1000BT1);
}

/**
 * linkmode_adv_to_mii_t1_adv_l_t
 * @advertising: the linkmode advertisement settings
 *
 * A small helper function that translates linkmode advertisement
 * settings to phy autonegotiation advertisements for the
 * BASE-T1 Autonegotiation Advertisement [15:0] Register.
 */
static inline u32 linkmode_adv_to_mii_t1_adv_l_t(unsigned long *advertising)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_Pause_BIT, advertising))
		result |= MDIO_AN_T1_ADV_L_PAUSE_CAP;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, advertising))
		result |= MDIO_AN_T1_ADV_L_PAUSE_ASYM;

	return result;
}

/**
 * linkmode_adv_to_mii_t1_adv_m_t
 * @advertising: the linkmode advertisement settings
 *
 * A small helper function that translates linkmode advertisement
 * settings to phy autonegotiation advertisements for the
 * BASE-T1 Autonegotiation Advertisement [31:16] Register.
 */
static inline u32 linkmode_adv_to_mii_t1_adv_m_t(unsigned long *advertising)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10baseT1L_Full_BIT, advertising))
		result |= MDIO_AN_T1_ADV_M_B10L;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_100baseT1_Full_BIT, advertising))
		result |= MDIO_AN_T1_ADV_M_100BT1;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_1000baseT1_Full_BIT, advertising))
		result |= MDIO_AN_T1_ADV_M_1000BT1;

	return result;
}

/**
 * mii_eee_cap1_mod_linkmode_t()
 * @adv: target the linkmode advertisement settings
 * @val: register value
 *
 * A function that translates value of following registers to the linkmode:
 * IEEE 802.3-2018 45.2.3.10 "EEE control and capability 1" register (3.20)
 * IEEE 802.3-2018 45.2.7.13 "EEE advertisement 1" register (7.60)
 * IEEE 802.3-2018 45.2.7.14 "EEE link partner ability 1" register (7.61)
 */
static inline void mii_eee_cap1_mod_linkmode_t(unsigned long *adv, u32 val)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT,
			 adv, val & MDIO_EEE_100TX);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT,
			 adv, val & MDIO_EEE_1000T);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT,
			 adv, val & MDIO_EEE_10GT);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_1000baseKX_Full_BIT,
			 adv, val & MDIO_EEE_1000KX);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseKX4_Full_BIT,
			 adv, val & MDIO_EEE_10GKX4);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseKR_Full_BIT,
			 adv, val & MDIO_EEE_10GKR);
}

/**
 * mii_eee_cap2_mod_linkmode_sup_t()
 * @adv: target the linkmode settings
 * @val: register value
 *
 * A function that translates value of following registers to the linkmode:
 * IEEE 802.3-2022 45.2.3.11 "EEE control and capability 2" register (3.21)
 */
static inline void mii_eee_cap2_mod_linkmode_sup_t(unsigned long *adv, u32 val)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
			 adv, val & MDIO_EEE_2_5GT);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT,
			 adv, val & MDIO_EEE_5GT);
}

/**
 * mii_eee_cap2_mod_linkmode_adv_t()
 * @adv: target the linkmode advertisement settings
 * @val: register value
 *
 * A function that translates value of following registers to the linkmode:
 * IEEE 802.3-2022 45.2.7.16 "EEE advertisement 2" register (7.62)
 * IEEE 802.3-2022 45.2.7.17 "EEE link partner ability 2" register (7.63)
 * Note: Currently this function is the same as mii_eee_cap2_mod_linkmode_sup_t.
 *       For certain, not yet supported, modes however the bits differ.
 *       Therefore create separate functions already.
 */
static inline void mii_eee_cap2_mod_linkmode_adv_t(unsigned long *adv, u32 val)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT,
			 adv, val & MDIO_EEE_2_5GT);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT,
			 adv, val & MDIO_EEE_5GT);
}

/**
 * linkmode_to_mii_eee_cap1_t()
 * @adv: the linkmode advertisement settings
 *
 * A function that translates linkmode to value for IEEE 802.3-2018 45.2.7.13
 * "EEE advertisement 1" register (7.60)
 */
static inline u32 linkmode_to_mii_eee_cap1_t(unsigned long *adv)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT, adv))
		result |= MDIO_EEE_100TX;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT, adv))
		result |= MDIO_EEE_1000T;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT, adv))
		result |= MDIO_EEE_10GT;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_1000baseKX_Full_BIT, adv))
		result |= MDIO_EEE_1000KX;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10000baseKX4_Full_BIT, adv))
		result |= MDIO_EEE_10GKX4;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10000baseKR_Full_BIT, adv))
		result |= MDIO_EEE_10GKR;

	return result;
}

/**
 * linkmode_to_mii_eee_cap2_t()
 * @adv: the linkmode advertisement settings
 *
 * A function that translates linkmode to value for IEEE 802.3-2022 45.2.7.16
 * "EEE advertisement 2" register (7.62)
 */
static inline u32 linkmode_to_mii_eee_cap2_t(unsigned long *adv)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT, adv))
		result |= MDIO_EEE_2_5GT;
	if (linkmode_test_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT, adv))
		result |= MDIO_EEE_5GT;

	return result;
}

/**
 * mii_10base_t1_adv_mod_linkmode_t()
 * @adv: linkmode advertisement settings
 * @val: register value
 *
 * A function that translates IEEE 802.3cg-2019 45.2.7.26 "10BASE-T1 AN status"
 * register (7.527) value to the linkmode.
 */
static inline void mii_10base_t1_adv_mod_linkmode_t(unsigned long *adv, u16 val)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10baseT1L_Full_BIT,
			 adv, val & MDIO_AN_10BT1_AN_CTRL_ADV_EEE_T1L);
}

/**
 * linkmode_adv_to_mii_10base_t1_t()
 * @adv: linkmode advertisement settings
 *
 * A function that translates the linkmode to IEEE 802.3cg-2019 45.2.7.25
 * "10BASE-T1 AN control" register (7.526) value.
 */
static inline u32 linkmode_adv_to_mii_10base_t1_t(unsigned long *adv)
{
	u32 result = 0;

	if (linkmode_test_bit(ETHTOOL_LINK_MODE_10baseT1L_Full_BIT, adv))
		result |= MDIO_AN_10BT1_AN_CTRL_ADV_EEE_T1L;

	return result;
}

/**
 * mii_c73_mod_linkmode - convert a Clause 73 advertisement to linkmodes
 * @adv: linkmode advertisement setting
 * @lpa: array of three u16s containing the advertisement
 *
 * Convert an IEEE 802.3 Clause 73 advertisement to ethtool link modes.
 */
static inline void mii_c73_mod_linkmode(unsigned long *adv, u16 *lpa)
{
	linkmode_mod_bit(ETHTOOL_LINK_MODE_Pause_BIT,
			 adv, lpa[0] & MDIO_AN_C73_0_PAUSE);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT,
			 adv, lpa[0] & MDIO_AN_C73_0_ASM_DIR);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_1000baseKX_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_1000BASE_KX);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseKX4_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_10GBASE_KX4);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_40000baseKR4_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_40GBASE_KR4);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_40000baseCR4_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_40GBASE_CR4);
	/* 100GBASE_CR10 and 100GBASE_KP4 not implemented */
	linkmode_mod_bit(ETHTOOL_LINK_MODE_100000baseKR4_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_100GBASE_KR4);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_100000baseCR4_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_100GBASE_CR4);
	/* 25GBASE_R_S not implemented */
	/* The 25GBASE_R bit can be used for 25Gbase KR or CR modes */
	linkmode_mod_bit(ETHTOOL_LINK_MODE_25000baseKR_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_25GBASE_R);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_25000baseCR_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_25GBASE_R);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_10000baseKR_Full_BIT,
			 adv, lpa[1] & MDIO_AN_C73_1_10GBASE_KR);
	linkmode_mod_bit(ETHTOOL_LINK_MODE_2500baseX_Full_BIT,
			 adv, lpa[2] & MDIO_AN_C73_2_2500BASE_KX);
	/* 5GBASE_KR not implemented */
}

int __mdiobus_read(struct mii_bus *bus, int addr, u32 regnum);
int __mdiobus_write(struct mii_bus *bus, int addr, u32 regnum, u16 val);
int __mdiobus_modify(struct mii_bus *bus, int addr, u32 regnum, u16 mask,
		     u16 set);
int __mdiobus_modify_changed(struct mii_bus *bus, int addr, u32 regnum,
			     u16 mask, u16 set);

int mdiobus_read(struct mii_bus *bus, int addr, u32 regnum);
int mdiobus_read_nested(struct mii_bus *bus, int addr, u32 regnum);
int mdiobus_write(struct mii_bus *bus, int addr, u32 regnum, u16 val);
int mdiobus_write_nested(struct mii_bus *bus, int addr, u32 regnum, u16 val);
int mdiobus_modify(struct mii_bus *bus, int addr, u32 regnum, u16 mask,
		   u16 set);
int mdiobus_modify_changed(struct mii_bus *bus, int addr, u32 regnum,
			   u16 mask, u16 set);
int __mdiobus_c45_read(struct mii_bus *bus, int addr, int devad, u32 regnum);
int mdiobus_c45_read(struct mii_bus *bus, int addr, int devad, u32 regnum);
int mdiobus_c45_read_nested(struct mii_bus *bus, int addr, int devad,
			     u32 regnum);
int __mdiobus_c45_write(struct mii_bus *bus, int addr,  int devad, u32 regnum,
			u16 val);
int mdiobus_c45_write(struct mii_bus *bus, int addr,  int devad, u32 regnum,
		      u16 val);
int mdiobus_c45_write_nested(struct mii_bus *bus, int addr,  int devad,
			     u32 regnum, u16 val);
int mdiobus_c45_modify(struct mii_bus *bus, int addr, int devad, u32 regnum,
		       u16 mask, u16 set);

int mdiobus_c45_modify_changed(struct mii_bus *bus, int addr, int devad,
			       u32 regnum, u16 mask, u16 set);

static inline int __mdiodev_read(struct mdio_device *mdiodev, u32 regnum)
{
	return __mdiobus_read(mdiodev->bus, mdiodev->addr, regnum);
}

static inline int __mdiodev_write(struct mdio_device *mdiodev, u32 regnum,
				  u16 val)
{
	return __mdiobus_write(mdiodev->bus, mdiodev->addr, regnum, val);
}

static inline int __mdiodev_modify(struct mdio_device *mdiodev, u32 regnum,
				   u16 mask, u16 set)
{
	return __mdiobus_modify(mdiodev->bus, mdiodev->addr, regnum, mask, set);
}

static inline int __mdiodev_modify_changed(struct mdio_device *mdiodev,
					   u32 regnum, u16 mask, u16 set)
{
	return __mdiobus_modify_changed(mdiodev->bus, mdiodev->addr, regnum,
					mask, set);
}

static inline int mdiodev_read(struct mdio_device *mdiodev, u32 regnum)
{
	return mdiobus_read(mdiodev->bus, mdiodev->addr, regnum);
}

static inline int mdiodev_write(struct mdio_device *mdiodev, u32 regnum,
				u16 val)
{
	return mdiobus_write(mdiodev->bus, mdiodev->addr, regnum, val);
}

static inline int mdiodev_modify(struct mdio_device *mdiodev, u32 regnum,
				 u16 mask, u16 set)
{
	return mdiobus_modify(mdiodev->bus, mdiodev->addr, regnum, mask, set);
}

static inline int mdiodev_modify_changed(struct mdio_device *mdiodev,
					 u32 regnum, u16 mask, u16 set)
{
	return mdiobus_modify_changed(mdiodev->bus, mdiodev->addr, regnum,
				      mask, set);
}

static inline int mdiodev_c45_modify(struct mdio_device *mdiodev, int devad,
				     u32 regnum, u16 mask, u16 set)
{
	return mdiobus_c45_modify(mdiodev->bus, mdiodev->addr, devad, regnum,
				  mask, set);
}

static inline int mdiodev_c45_modify_changed(struct mdio_device *mdiodev,
					     int devad, u32 regnum, u16 mask,
					     u16 set)
{
	return mdiobus_c45_modify_changed(mdiodev->bus, mdiodev->addr, devad,
					  regnum, mask, set);
}

static inline int mdiodev_c45_read(struct mdio_device *mdiodev, int devad,
				   u16 regnum)
{
	return mdiobus_c45_read(mdiodev->bus, mdiodev->addr, devad, regnum);
}

static inline int mdiodev_c45_write(struct mdio_device *mdiodev, u32 devad,
				    u16 regnum, u16 val)
{
	return mdiobus_c45_write(mdiodev->bus, mdiodev->addr, devad, regnum,
				 val);
}

int mdiobus_register_device(struct mdio_device *mdiodev);
int mdiobus_unregister_device(struct mdio_device *mdiodev);
bool mdiobus_is_registered_device(struct mii_bus *bus, int addr);
struct phy_device *mdiobus_get_phy(struct mii_bus *bus, int addr);

/**
 * mdio_module_driver() - Helper macro for registering mdio drivers
 * @_mdio_driver: driver to register
 *
 * Helper macro for MDIO drivers which do not do anything special in module
 * init/exit. Each module may only use this macro once, and calling it
 * replaces module_init() and module_exit().
 */
#define mdio_module_driver(_mdio_driver)				\
static int __init mdio_module_init(void)				\
{									\
	return mdio_driver_register(&_mdio_driver);			\
}									\
module_init(mdio_module_init);						\
static void __exit mdio_module_exit(void)				\
{									\
	mdio_driver_unregister(&_mdio_driver);				\
}									\
module_exit(mdio_module_exit)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __LINUX_MDIO_H__ */
