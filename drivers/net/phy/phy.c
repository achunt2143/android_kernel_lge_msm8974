<<<<<<< HEAD
/*
 * drivers/net/phy/phy.c
 *
 * Framework for configuring and reading PHY devices
=======
// SPDX-License-Identifier: GPL-2.0+
/* Framework for configuring and reading PHY devices
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Based on code in sungem_phy.c and gianfar_phy.c
 *
 * Author: Andy Fleming
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
 * Copyright (c) 2006, 2007  Maciej W. Rozycki
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
=======
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
=======
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
<<<<<<< HEAD
#include <linux/phy.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#include <linux/atomic.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
=======
#include <linux/ethtool_netlink.h>
#include <linux/phy.h>
#include <linux/phy_led_triggers.h>
#include <linux/sfp.h>
#include <linux/workqueue.h>
#include <linux/mdio.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/suspend.h>
#include <net/netlink.h>
#include <net/genetlink.h>
#include <net/sock.h>

#define PHY_STATE_TIME	HZ

#define PHY_STATE_STR(_state)			\
	case PHY_##_state:			\
		return __stringify(_state);	\

static const char *phy_state_to_str(enum phy_state st)
{
	switch (st) {
	PHY_STATE_STR(DOWN)
	PHY_STATE_STR(READY)
	PHY_STATE_STR(UP)
	PHY_STATE_STR(RUNNING)
	PHY_STATE_STR(NOLINK)
	PHY_STATE_STR(CABLETEST)
	PHY_STATE_STR(HALTED)
	PHY_STATE_STR(ERROR)
	}

	return NULL;
}

static void phy_process_state_change(struct phy_device *phydev,
				     enum phy_state old_state)
{
	if (old_state != phydev->state) {
		phydev_dbg(phydev, "PHY state change %s -> %s\n",
			   phy_state_to_str(old_state),
			   phy_state_to_str(phydev->state));
		if (phydev->drv && phydev->drv->link_change_notify)
			phydev->drv->link_change_notify(phydev);
	}
}

static void phy_link_up(struct phy_device *phydev)
{
	phydev->phy_link_change(phydev, true);
	phy_led_trigger_change_speed(phydev);
}

static void phy_link_down(struct phy_device *phydev)
{
	phydev->phy_link_change(phydev, false);
	phy_led_trigger_change_speed(phydev);
	WRITE_ONCE(phydev->link_down_events, phydev->link_down_events + 1);
}

static const char *phy_pause_str(struct phy_device *phydev)
{
	bool local_pause, local_asym_pause;

	if (phydev->autoneg == AUTONEG_DISABLE)
		goto no_pause;

	local_pause = linkmode_test_bit(ETHTOOL_LINK_MODE_Pause_BIT,
					phydev->advertising);
	local_asym_pause = linkmode_test_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT,
					     phydev->advertising);

	if (local_pause && phydev->pause)
		return "rx/tx";

	if (local_asym_pause && phydev->asym_pause) {
		if (local_pause)
			return "rx";
		if (phydev->pause)
			return "tx";
	}

no_pause:
	return "off";
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_print_status - Convenience function to print out the current phy status
 * @phydev: the phy_device struct
 */
void phy_print_status(struct phy_device *phydev)
{
<<<<<<< HEAD
	pr_info("PHY: %s - Link is %s", dev_name(&phydev->dev),
			phydev->link ? "Up" : "Down");
	if (phydev->link)
		printk(KERN_CONT " - %d/%s", phydev->speed,
				DUPLEX_FULL == phydev->duplex ?
				"Full" : "Half");

	printk(KERN_CONT "\n");
}
EXPORT_SYMBOL(phy_print_status);


/**
 * phy_clear_interrupt - Ack the phy device's interrupt
 * @phydev: the phy_device struct
 *
 * If the @phydev driver has an ack_interrupt function, call it to
 * ack and clear the phy device's interrupt.
 *
 * Returns 0 on success on < 0 on error.
 */
static int phy_clear_interrupt(struct phy_device *phydev)
{
	int err = 0;

	if (phydev->drv->ack_interrupt)
		err = phydev->drv->ack_interrupt(phydev);

	return err;
}
=======
	if (phydev->link) {
		netdev_info(phydev->attached_dev,
			"Link is Up - %s/%s %s- flow control %s\n",
			phy_speed_to_str(phydev->speed),
			phy_duplex_to_str(phydev->duplex),
			phydev->downshifted_rate ? "(downshifted) " : "",
			phy_pause_str(phydev));
	} else	{
		netdev_info(phydev->attached_dev, "Link is Down\n");
	}
}
EXPORT_SYMBOL(phy_print_status);

/**
 * phy_get_rate_matching - determine if rate matching is supported
 * @phydev: The phy device to return rate matching for
 * @iface: The interface mode to use
 *
 * This determines the type of rate matching (if any) that @phy supports
 * using @iface. @iface may be %PHY_INTERFACE_MODE_NA to determine if any
 * interface supports rate matching.
 *
 * Return: The type of rate matching @phy supports for @iface, or
 *         %RATE_MATCH_NONE.
 */
int phy_get_rate_matching(struct phy_device *phydev,
			  phy_interface_t iface)
{
	int ret = RATE_MATCH_NONE;

	if (phydev->drv->get_rate_matching) {
		mutex_lock(&phydev->lock);
		ret = phydev->drv->get_rate_matching(phydev, iface);
		mutex_unlock(&phydev->lock);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(phy_get_rate_matching);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_config_interrupt - configure the PHY device for the requested interrupts
 * @phydev: the phy_device struct
 * @interrupts: interrupt flags to configure for this @phydev
 *
<<<<<<< HEAD
 * Returns 0 on success on < 0 on error.
 */
static int phy_config_interrupt(struct phy_device *phydev, u32 interrupts)
{
	int err = 0;

	phydev->interrupts = interrupts;
	if (phydev->drv->config_intr)
		err = phydev->drv->config_intr(phydev);

	return err;
}

=======
 * Returns 0 on success or < 0 on error.
 */
static int phy_config_interrupt(struct phy_device *phydev, bool interrupts)
{
	phydev->interrupts = interrupts ? 1 : 0;
	if (phydev->drv->config_intr)
		return phydev->drv->config_intr(phydev);

	return 0;
}

/**
 * phy_restart_aneg - restart auto-negotiation
 * @phydev: target phy_device struct
 *
 * Restart the autonegotiation on @phydev.  Returns >= 0 on success or
 * negative errno on error.
 */
int phy_restart_aneg(struct phy_device *phydev)
{
	int ret;

	if (phydev->is_c45 && !(phydev->c45_ids.devices_in_package & BIT(0)))
		ret = genphy_c45_restart_aneg(phydev);
	else
		ret = genphy_restart_aneg(phydev);

	return ret;
}
EXPORT_SYMBOL_GPL(phy_restart_aneg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_aneg_done - return auto-negotiation status
 * @phydev: target phy_device struct
 *
<<<<<<< HEAD
 * Description: Reads the status register and returns 0 either if
 *   auto-negotiation is incomplete, or if there was an error.
 *   Returns BMSR_ANEGCOMPLETE if auto-negotiation is done.
 */
static inline int phy_aneg_done(struct phy_device *phydev)
{
	int retval;

	retval = phy_read(phydev, MII_BMSR);

	return (retval < 0) ? retval : (retval & BMSR_ANEGCOMPLETE);
}

/* A structure for mapping a particular speed and duplex
 * combination to a particular SUPPORTED and ADVERTISED value */
struct phy_setting {
	int speed;
	int duplex;
	u32 setting;
};

/* A mapping of all SUPPORTED settings to speed/duplex */
static const struct phy_setting settings[] = {
	{
		.speed = 10000,
		.duplex = DUPLEX_FULL,
		.setting = SUPPORTED_10000baseT_Full,
	},
	{
		.speed = SPEED_1000,
		.duplex = DUPLEX_FULL,
		.setting = SUPPORTED_1000baseT_Full,
	},
	{
		.speed = SPEED_1000,
		.duplex = DUPLEX_HALF,
		.setting = SUPPORTED_1000baseT_Half,
	},
	{
		.speed = SPEED_100,
		.duplex = DUPLEX_FULL,
		.setting = SUPPORTED_100baseT_Full,
	},
	{
		.speed = SPEED_100,
		.duplex = DUPLEX_HALF,
		.setting = SUPPORTED_100baseT_Half,
	},
	{
		.speed = SPEED_10,
		.duplex = DUPLEX_FULL,
		.setting = SUPPORTED_10baseT_Full,
	},
	{
		.speed = SPEED_10,
		.duplex = DUPLEX_HALF,
		.setting = SUPPORTED_10baseT_Half,
	},
};

#define MAX_NUM_SETTINGS ARRAY_SIZE(settings)

/**
 * phy_find_setting - find a PHY settings array entry that matches speed & duplex
 * @speed: speed to match
 * @duplex: duplex to match
 *
 * Description: Searches the settings array for the setting which
 *   matches the desired speed and duplex, and returns the index
 *   of that setting.  Returns the index of the last setting if
 *   none of the others match.
 */
static inline int phy_find_setting(int speed, int duplex)
{
	int idx = 0;

	while (idx < ARRAY_SIZE(settings) &&
			(settings[idx].speed != speed ||
			settings[idx].duplex != duplex))
		idx++;

	return idx < MAX_NUM_SETTINGS ? idx : MAX_NUM_SETTINGS - 1;
}

/**
 * phy_find_valid - find a PHY setting that matches the requested features mask
 * @idx: The first index in settings[] to search
 * @features: A mask of the valid settings
 *
 * Description: Returns the index of the first valid setting less
 *   than or equal to the one pointed to by idx, as determined by
 *   the mask in features.  Returns the index of the last setting
 *   if nothing else matches.
 */
static inline int phy_find_valid(int idx, u32 features)
{
	while (idx < MAX_NUM_SETTINGS && !(settings[idx].setting & features))
		idx++;

	return idx < MAX_NUM_SETTINGS ? idx : MAX_NUM_SETTINGS - 1;
}
=======
 * Description: Return the auto-negotiation status from this @phydev
 * Returns > 0 on success or < 0 on error. 0 means that auto-negotiation
 * is still pending.
 */
int phy_aneg_done(struct phy_device *phydev)
{
	if (phydev->drv && phydev->drv->aneg_done)
		return phydev->drv->aneg_done(phydev);
	else if (phydev->is_c45)
		return genphy_c45_aneg_done(phydev);
	else
		return genphy_aneg_done(phydev);
}
EXPORT_SYMBOL(phy_aneg_done);

/**
 * phy_find_valid - find a PHY setting that matches the requested parameters
 * @speed: desired speed
 * @duplex: desired duplex
 * @supported: mask of supported link modes
 *
 * Locate a supported phy setting that is, in priority order:
 * - an exact match for the specified speed and duplex mode
 * - a match for the specified speed, or slower speed
 * - the slowest supported speed
 * Returns the matched phy_setting entry, or %NULL if no supported phy
 * settings were found.
 */
static const struct phy_setting *
phy_find_valid(int speed, int duplex, unsigned long *supported)
{
	return phy_lookup_setting(speed, duplex, supported, false);
}

/**
 * phy_supported_speeds - return all speeds currently supported by a phy device
 * @phy: The phy device to return supported speeds of.
 * @speeds: buffer to store supported speeds in.
 * @size:   size of speeds buffer.
 *
 * Description: Returns the number of supported speeds, and fills the speeds
 * buffer with the supported speeds. If speeds buffer is too small to contain
 * all currently supported speeds, will return as many speeds as can fit.
 */
unsigned int phy_supported_speeds(struct phy_device *phy,
				  unsigned int *speeds,
				  unsigned int size)
{
	return phy_speeds(speeds, size, phy->supported);
}

/**
 * phy_check_valid - check if there is a valid PHY setting which matches
 *		     speed, duplex, and feature mask
 * @speed: speed to match
 * @duplex: duplex to match
 * @features: A mask of the valid settings
 *
 * Description: Returns true if there is a valid setting, false otherwise.
 */
bool phy_check_valid(int speed, int duplex, unsigned long *features)
{
	return !!phy_lookup_setting(speed, duplex, features, true);
}
EXPORT_SYMBOL(phy_check_valid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_sanitize_settings - make sure the PHY is set to supported speed and duplex
 * @phydev: the target phy_device struct
 *
 * Description: Make sure the PHY is set to supported speeds and
 *   duplexes.  Drop down by one in this order:  1000/FULL,
 *   1000/HALF, 100/FULL, 100/HALF, 10/FULL, 10/HALF.
 */
static void phy_sanitize_settings(struct phy_device *phydev)
{
<<<<<<< HEAD
	u32 features = phydev->supported;
	int idx;

	/* Sanitize settings based on PHY capabilities */
	if ((features & SUPPORTED_Autoneg) == 0)
		phydev->autoneg = AUTONEG_DISABLE;

	idx = phy_find_valid(phy_find_setting(phydev->speed, phydev->duplex),
			features);

	phydev->speed = settings[idx].speed;
	phydev->duplex = settings[idx].duplex;
}

/**
 * phy_ethtool_sset - generic ethtool sset function, handles all the details
 * @phydev: target phy_device struct
 * @cmd: ethtool_cmd
 *
 * A few notes about parameter checking:
 * - We don't set port or transceiver, so we don't care what they
 *   were set to.
 * - phy_start_aneg() will make sure forced settings are sane, and
 *   choose the next best ones from the ones selected, so we don't
 *   care if ethtool tries to give us bad values.
 */
int phy_ethtool_sset(struct phy_device *phydev, struct ethtool_cmd *cmd)
{
	u32 speed = ethtool_cmd_speed(cmd);

	if (cmd->phy_address != phydev->addr)
		return -EINVAL;

	/* We make sure that we don't pass unsupported
	 * values in to the PHY */
	cmd->advertising &= phydev->supported;

	/* Verify the settings we care about. */
	if (cmd->autoneg != AUTONEG_ENABLE && cmd->autoneg != AUTONEG_DISABLE)
		return -EINVAL;

	if (cmd->autoneg == AUTONEG_ENABLE && cmd->advertising == 0)
		return -EINVAL;

	if (cmd->autoneg == AUTONEG_DISABLE &&
	    ((speed != SPEED_1000 &&
	      speed != SPEED_100 &&
	      speed != SPEED_10) ||
	     (cmd->duplex != DUPLEX_HALF &&
	      cmd->duplex != DUPLEX_FULL)))
		return -EINVAL;

	phydev->autoneg = cmd->autoneg;

	phydev->speed = speed;

	phydev->advertising = cmd->advertising;

	if (AUTONEG_ENABLE == cmd->autoneg)
		phydev->advertising |= ADVERTISED_Autoneg;
	else
		phydev->advertising &= ~ADVERTISED_Autoneg;

	phydev->duplex = cmd->duplex;

	/* Restart the PHY */
	phy_start_aneg(phydev);

	return 0;
}
EXPORT_SYMBOL(phy_ethtool_sset);

int phy_ethtool_gset(struct phy_device *phydev, struct ethtool_cmd *cmd)
{
	cmd->supported = phydev->supported;

	cmd->advertising = phydev->advertising;

	ethtool_cmd_speed_set(cmd, phydev->speed);
	cmd->duplex = phydev->duplex;
	cmd->port = PORT_MII;
	cmd->phy_address = phydev->addr;
	cmd->transceiver = XCVR_EXTERNAL;
	cmd->autoneg = phydev->autoneg;

	return 0;
}
EXPORT_SYMBOL(phy_ethtool_gset);
=======
	const struct phy_setting *setting;

	setting = phy_find_valid(phydev->speed, phydev->duplex,
				 phydev->supported);
	if (setting) {
		phydev->speed = setting->speed;
		phydev->duplex = setting->duplex;
	} else {
		/* We failed to find anything (no supported speeds?) */
		phydev->speed = SPEED_UNKNOWN;
		phydev->duplex = DUPLEX_UNKNOWN;
	}
}

void phy_ethtool_ksettings_get(struct phy_device *phydev,
			       struct ethtool_link_ksettings *cmd)
{
	mutex_lock(&phydev->lock);
	linkmode_copy(cmd->link_modes.supported, phydev->supported);
	linkmode_copy(cmd->link_modes.advertising, phydev->advertising);
	linkmode_copy(cmd->link_modes.lp_advertising, phydev->lp_advertising);

	cmd->base.speed = phydev->speed;
	cmd->base.duplex = phydev->duplex;
	cmd->base.master_slave_cfg = phydev->master_slave_get;
	cmd->base.master_slave_state = phydev->master_slave_state;
	cmd->base.rate_matching = phydev->rate_matching;
	if (phydev->interface == PHY_INTERFACE_MODE_MOCA)
		cmd->base.port = PORT_BNC;
	else
		cmd->base.port = phydev->port;
	cmd->base.transceiver = phy_is_internal(phydev) ?
				XCVR_INTERNAL : XCVR_EXTERNAL;
	cmd->base.phy_address = phydev->mdio.addr;
	cmd->base.autoneg = phydev->autoneg;
	cmd->base.eth_tp_mdix_ctrl = phydev->mdix_ctrl;
	cmd->base.eth_tp_mdix = phydev->mdix;
	mutex_unlock(&phydev->lock);
}
EXPORT_SYMBOL(phy_ethtool_ksettings_get);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_mii_ioctl - generic PHY MII ioctl interface
 * @phydev: the phy_device struct
 * @ifr: &struct ifreq for socket ioctl's
 * @cmd: ioctl cmd to execute
 *
 * Note that this function is currently incompatible with the
 * PHYCONTROL layer.  It changes registers without regard to
 * current state.  Use at own risk.
 */
<<<<<<< HEAD
int phy_mii_ioctl(struct phy_device *phydev,
		struct ifreq *ifr, int cmd)
{
	struct mii_ioctl_data *mii_data = if_mii(ifr);
	u16 val = mii_data->val_in;

	switch (cmd) {
	case SIOCGMIIPHY:
		mii_data->phy_id = phydev->addr;
		/* fall through */

	case SIOCGMIIREG:
		mii_data->val_out = mdiobus_read(phydev->bus, mii_data->phy_id,
						 mii_data->reg_num);
		break;

	case SIOCSMIIREG:
		if (mii_data->phy_id == phydev->addr) {
			switch(mii_data->reg_num) {
			case MII_BMCR:
				if ((val & (BMCR_RESET|BMCR_ANENABLE)) == 0)
					phydev->autoneg = AUTONEG_DISABLE;
				else
					phydev->autoneg = AUTONEG_ENABLE;
				if ((!phydev->autoneg) && (val & BMCR_FULLDPLX))
					phydev->duplex = DUPLEX_FULL;
				else
					phydev->duplex = DUPLEX_HALF;
				if ((!phydev->autoneg) &&
						(val & BMCR_SPEED1000))
					phydev->speed = SPEED_1000;
				else if ((!phydev->autoneg) &&
						(val & BMCR_SPEED100))
					phydev->speed = SPEED_100;
				break;
			case MII_ADVERTISE:
				phydev->advertising = val;
=======
int phy_mii_ioctl(struct phy_device *phydev, struct ifreq *ifr, int cmd)
{
	struct mii_ioctl_data *mii_data = if_mii(ifr);
	struct kernel_hwtstamp_config kernel_cfg;
	struct netlink_ext_ack extack = {};
	u16 val = mii_data->val_in;
	bool change_autoneg = false;
	struct hwtstamp_config cfg;
	int prtad, devad;
	int ret;

	switch (cmd) {
	case SIOCGMIIPHY:
		mii_data->phy_id = phydev->mdio.addr;
		fallthrough;

	case SIOCGMIIREG:
		if (mdio_phy_id_is_c45(mii_data->phy_id)) {
			prtad = mdio_phy_id_prtad(mii_data->phy_id);
			devad = mdio_phy_id_devad(mii_data->phy_id);
			mii_data->val_out = mdiobus_c45_read(
				phydev->mdio.bus, prtad, devad,
				mii_data->reg_num);
		} else {
			mii_data->val_out = mdiobus_read(
				phydev->mdio.bus, mii_data->phy_id,
				mii_data->reg_num);
		}
		return 0;

	case SIOCSMIIREG:
		if (mdio_phy_id_is_c45(mii_data->phy_id)) {
			prtad = mdio_phy_id_prtad(mii_data->phy_id);
			devad = mdio_phy_id_devad(mii_data->phy_id);
		} else {
			prtad = mii_data->phy_id;
			devad = mii_data->reg_num;
		}
		if (prtad == phydev->mdio.addr) {
			switch (devad) {
			case MII_BMCR:
				if ((val & (BMCR_RESET | BMCR_ANENABLE)) == 0) {
					if (phydev->autoneg == AUTONEG_ENABLE)
						change_autoneg = true;
					phydev->autoneg = AUTONEG_DISABLE;
					if (val & BMCR_FULLDPLX)
						phydev->duplex = DUPLEX_FULL;
					else
						phydev->duplex = DUPLEX_HALF;
					if (val & BMCR_SPEED1000)
						phydev->speed = SPEED_1000;
					else if (val & BMCR_SPEED100)
						phydev->speed = SPEED_100;
					else phydev->speed = SPEED_10;
				} else {
					if (phydev->autoneg == AUTONEG_DISABLE)
						change_autoneg = true;
					phydev->autoneg = AUTONEG_ENABLE;
				}
				break;
			case MII_ADVERTISE:
				mii_adv_mod_linkmode_adv_t(phydev->advertising,
							   val);
				change_autoneg = true;
				break;
			case MII_CTRL1000:
				mii_ctrl1000_mod_linkmode_adv_t(phydev->advertising,
							        val);
				change_autoneg = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			default:
				/* do nothing */
				break;
			}
		}

<<<<<<< HEAD
		mdiobus_write(phydev->bus, mii_data->phy_id,
			      mii_data->reg_num, val);

		if (mii_data->reg_num == MII_BMCR &&
		    val & BMCR_RESET &&
		    phydev->drv->config_init) {
			phy_scan_fixups(phydev);
			phydev->drv->config_init(phydev);
		}
		break;

	case SIOCSHWTSTAMP:
		if (phydev->drv->hwtstamp)
			return phydev->drv->hwtstamp(phydev, ifr);
		/* fall through */
=======
		if (mdio_phy_id_is_c45(mii_data->phy_id))
			mdiobus_c45_write(phydev->mdio.bus, prtad, devad,
					  mii_data->reg_num, val);
		else
			mdiobus_write(phydev->mdio.bus, prtad, devad, val);

		if (prtad == phydev->mdio.addr &&
		    devad == MII_BMCR &&
		    val & BMCR_RESET)
			return phy_init_hw(phydev);

		if (change_autoneg)
			return phy_start_aneg(phydev);

		return 0;

	case SIOCSHWTSTAMP:
		if (phydev->mii_ts && phydev->mii_ts->hwtstamp) {
			if (copy_from_user(&cfg, ifr->ifr_data, sizeof(cfg)))
				return -EFAULT;

			hwtstamp_config_to_kernel(&kernel_cfg, &cfg);
			ret = phydev->mii_ts->hwtstamp(phydev->mii_ts, &kernel_cfg, &extack);
			if (ret)
				return ret;

			hwtstamp_config_from_kernel(&cfg, &kernel_cfg);
			if (copy_to_user(ifr->ifr_data, &cfg, sizeof(cfg)))
				return -EFAULT;

			return 0;
		}
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	default:
		return -EOPNOTSUPP;
	}
<<<<<<< HEAD

	return 0;
}
EXPORT_SYMBOL(phy_mii_ioctl);
=======
}
EXPORT_SYMBOL(phy_mii_ioctl);

/**
 * phy_do_ioctl - generic ndo_eth_ioctl implementation
 * @dev: the net_device struct
 * @ifr: &struct ifreq for socket ioctl's
 * @cmd: ioctl cmd to execute
 */
int phy_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	if (!dev->phydev)
		return -ENODEV;

	return phy_mii_ioctl(dev->phydev, ifr, cmd);
}
EXPORT_SYMBOL(phy_do_ioctl);

/**
 * phy_do_ioctl_running - generic ndo_eth_ioctl implementation but test first
 *
 * @dev: the net_device struct
 * @ifr: &struct ifreq for socket ioctl's
 * @cmd: ioctl cmd to execute
 *
 * Same as phy_do_ioctl, but ensures that net_device is running before
 * handling the ioctl.
 */
int phy_do_ioctl_running(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	if (!netif_running(dev))
		return -ENODEV;

	return phy_do_ioctl(dev, ifr, cmd);
}
EXPORT_SYMBOL(phy_do_ioctl_running);

/**
 * __phy_hwtstamp_get - Get hardware timestamping configuration from PHY
 *
 * @phydev: the PHY device structure
 * @config: structure holding the timestamping configuration
 *
 * Query the PHY device for its current hardware timestamping configuration.
 */
int __phy_hwtstamp_get(struct phy_device *phydev,
		       struct kernel_hwtstamp_config *config)
{
	if (!phydev)
		return -ENODEV;

	return -EOPNOTSUPP;
}

/**
 * __phy_hwtstamp_set - Modify PHY hardware timestamping configuration
 *
 * @phydev: the PHY device structure
 * @config: structure holding the timestamping configuration
 * @extack: netlink extended ack structure, for error reporting
 */
int __phy_hwtstamp_set(struct phy_device *phydev,
		       struct kernel_hwtstamp_config *config,
		       struct netlink_ext_ack *extack)
{
	if (!phydev)
		return -ENODEV;

	if (phydev->mii_ts && phydev->mii_ts->hwtstamp)
		return phydev->mii_ts->hwtstamp(phydev->mii_ts, config, extack);

	return -EOPNOTSUPP;
}

/**
 * phy_queue_state_machine - Trigger the state machine to run soon
 *
 * @phydev: the phy_device struct
 * @jiffies: Run the state machine after these jiffies
 */
void phy_queue_state_machine(struct phy_device *phydev, unsigned long jiffies)
{
	mod_delayed_work(system_power_efficient_wq, &phydev->state_queue,
			 jiffies);
}
EXPORT_SYMBOL(phy_queue_state_machine);

/**
 * phy_trigger_machine - Trigger the state machine to run now
 *
 * @phydev: the phy_device struct
 */
void phy_trigger_machine(struct phy_device *phydev)
{
	phy_queue_state_machine(phydev, 0);
}
EXPORT_SYMBOL(phy_trigger_machine);

static void phy_abort_cable_test(struct phy_device *phydev)
{
	int err;

	ethnl_cable_test_finished(phydev);

	err = phy_init_hw(phydev);
	if (err)
		phydev_err(phydev, "Error while aborting cable test");
}

/**
 * phy_ethtool_get_strings - Get the statistic counter names
 *
 * @phydev: the phy_device struct
 * @data: Where to put the strings
 */
int phy_ethtool_get_strings(struct phy_device *phydev, u8 *data)
{
	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	phydev->drv->get_strings(phydev, data);
	mutex_unlock(&phydev->lock);

	return 0;
}
EXPORT_SYMBOL(phy_ethtool_get_strings);

/**
 * phy_ethtool_get_sset_count - Get the number of statistic counters
 *
 * @phydev: the phy_device struct
 */
int phy_ethtool_get_sset_count(struct phy_device *phydev)
{
	int ret;

	if (!phydev->drv)
		return -EIO;

	if (phydev->drv->get_sset_count &&
	    phydev->drv->get_strings &&
	    phydev->drv->get_stats) {
		mutex_lock(&phydev->lock);
		ret = phydev->drv->get_sset_count(phydev);
		mutex_unlock(&phydev->lock);

		return ret;
	}

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(phy_ethtool_get_sset_count);

/**
 * phy_ethtool_get_stats - Get the statistic counters
 *
 * @phydev: the phy_device struct
 * @stats: What counters to get
 * @data: Where to store the counters
 */
int phy_ethtool_get_stats(struct phy_device *phydev,
			  struct ethtool_stats *stats, u64 *data)
{
	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	phydev->drv->get_stats(phydev, stats, data);
	mutex_unlock(&phydev->lock);

	return 0;
}
EXPORT_SYMBOL(phy_ethtool_get_stats);

/**
 * phy_ethtool_get_plca_cfg - Get PLCA RS configuration
 * @phydev: the phy_device struct
 * @plca_cfg: where to store the retrieved configuration
 *
 * Retrieve the PLCA configuration from the PHY. Return 0 on success or a
 * negative value if an error occurred.
 */
int phy_ethtool_get_plca_cfg(struct phy_device *phydev,
			     struct phy_plca_cfg *plca_cfg)
{
	int ret;

	if (!phydev->drv) {
		ret = -EIO;
		goto out;
	}

	if (!phydev->drv->get_plca_cfg) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	mutex_lock(&phydev->lock);
	ret = phydev->drv->get_plca_cfg(phydev, plca_cfg);

	mutex_unlock(&phydev->lock);
out:
	return ret;
}

/**
 * plca_check_valid - Check PLCA configuration before enabling
 * @phydev: the phy_device struct
 * @plca_cfg: current PLCA configuration
 * @extack: extack for reporting useful error messages
 *
 * Checks whether the PLCA and PHY configuration are consistent and it is safe
 * to enable PLCA. Returns 0 on success or a negative value if the PLCA or PHY
 * configuration is not consistent.
 */
static int plca_check_valid(struct phy_device *phydev,
			    const struct phy_plca_cfg *plca_cfg,
			    struct netlink_ext_ack *extack)
{
	int ret = 0;

	if (!linkmode_test_bit(ETHTOOL_LINK_MODE_10baseT1S_P2MP_Half_BIT,
			       phydev->advertising)) {
		ret = -EOPNOTSUPP;
		NL_SET_ERR_MSG(extack,
			       "Point to Multi-Point mode is not enabled");
	} else if (plca_cfg->node_id >= 255) {
		NL_SET_ERR_MSG(extack, "PLCA node ID is not set");
		ret = -EINVAL;
	}

	return ret;
}

/**
 * phy_ethtool_set_plca_cfg - Set PLCA RS configuration
 * @phydev: the phy_device struct
 * @plca_cfg: new PLCA configuration to apply
 * @extack: extack for reporting useful error messages
 *
 * Sets the PLCA configuration in the PHY. Return 0 on success or a
 * negative value if an error occurred.
 */
int phy_ethtool_set_plca_cfg(struct phy_device *phydev,
			     const struct phy_plca_cfg *plca_cfg,
			     struct netlink_ext_ack *extack)
{
	struct phy_plca_cfg *curr_plca_cfg;
	int ret;

	if (!phydev->drv) {
		ret = -EIO;
		goto out;
	}

	if (!phydev->drv->set_plca_cfg ||
	    !phydev->drv->get_plca_cfg) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	curr_plca_cfg = kmalloc(sizeof(*curr_plca_cfg), GFP_KERNEL);
	if (!curr_plca_cfg) {
		ret = -ENOMEM;
		goto out;
	}

	mutex_lock(&phydev->lock);

	ret = phydev->drv->get_plca_cfg(phydev, curr_plca_cfg);
	if (ret)
		goto out_drv;

	if (curr_plca_cfg->enabled < 0 && plca_cfg->enabled >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'enable' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	if (curr_plca_cfg->node_id < 0 && plca_cfg->node_id >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'local node ID' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	if (curr_plca_cfg->node_cnt < 0 && plca_cfg->node_cnt >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'node count' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	if (curr_plca_cfg->to_tmr < 0 && plca_cfg->to_tmr >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'TO timer' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	if (curr_plca_cfg->burst_cnt < 0 && plca_cfg->burst_cnt >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'burst count' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	if (curr_plca_cfg->burst_tmr < 0 && plca_cfg->burst_tmr >= 0) {
		NL_SET_ERR_MSG(extack,
			       "PHY does not support changing the PLCA 'burst timer' attribute");
		ret = -EINVAL;
		goto out_drv;
	}

	// if enabling PLCA, perform a few sanity checks
	if (plca_cfg->enabled > 0) {
		// allow setting node_id concurrently with enabled
		if (plca_cfg->node_id >= 0)
			curr_plca_cfg->node_id = plca_cfg->node_id;

		ret = plca_check_valid(phydev, curr_plca_cfg, extack);
		if (ret)
			goto out_drv;
	}

	ret = phydev->drv->set_plca_cfg(phydev, plca_cfg);

out_drv:
	kfree(curr_plca_cfg);
	mutex_unlock(&phydev->lock);
out:
	return ret;
}

/**
 * phy_ethtool_get_plca_status - Get PLCA RS status information
 * @phydev: the phy_device struct
 * @plca_st: where to store the retrieved status information
 *
 * Retrieve the PLCA status information from the PHY. Return 0 on success or a
 * negative value if an error occurred.
 */
int phy_ethtool_get_plca_status(struct phy_device *phydev,
				struct phy_plca_status *plca_st)
{
	int ret;

	if (!phydev->drv) {
		ret = -EIO;
		goto out;
	}

	if (!phydev->drv->get_plca_status) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	mutex_lock(&phydev->lock);
	ret = phydev->drv->get_plca_status(phydev, plca_st);

	mutex_unlock(&phydev->lock);
out:
	return ret;
}

/**
 * phy_start_cable_test - Start a cable test
 *
 * @phydev: the phy_device struct
 * @extack: extack for reporting useful error messages
 */
int phy_start_cable_test(struct phy_device *phydev,
			 struct netlink_ext_ack *extack)
{
	struct net_device *dev = phydev->attached_dev;
	int err = -ENOMEM;

	if (!(phydev->drv &&
	      phydev->drv->cable_test_start &&
	      phydev->drv->cable_test_get_status)) {
		NL_SET_ERR_MSG(extack,
			       "PHY driver does not support cable testing");
		return -EOPNOTSUPP;
	}

	mutex_lock(&phydev->lock);
	if (phydev->state == PHY_CABLETEST) {
		NL_SET_ERR_MSG(extack,
			       "PHY already performing a test");
		err = -EBUSY;
		goto out;
	}

	if (phydev->state < PHY_UP ||
	    phydev->state > PHY_CABLETEST) {
		NL_SET_ERR_MSG(extack,
			       "PHY not configured. Try setting interface up");
		err = -EBUSY;
		goto out;
	}

	err = ethnl_cable_test_alloc(phydev, ETHTOOL_MSG_CABLE_TEST_NTF);
	if (err)
		goto out;

	/* Mark the carrier down until the test is complete */
	phy_link_down(phydev);

	netif_testing_on(dev);
	err = phydev->drv->cable_test_start(phydev);
	if (err) {
		netif_testing_off(dev);
		phy_link_up(phydev);
		goto out_free;
	}

	phydev->state = PHY_CABLETEST;

	if (phy_polling_mode(phydev))
		phy_trigger_machine(phydev);

	mutex_unlock(&phydev->lock);

	return 0;

out_free:
	ethnl_cable_test_free(phydev);
out:
	mutex_unlock(&phydev->lock);

	return err;
}
EXPORT_SYMBOL(phy_start_cable_test);

/**
 * phy_start_cable_test_tdr - Start a raw TDR cable test
 *
 * @phydev: the phy_device struct
 * @extack: extack for reporting useful error messages
 * @config: Configuration of the test to run
 */
int phy_start_cable_test_tdr(struct phy_device *phydev,
			     struct netlink_ext_ack *extack,
			     const struct phy_tdr_config *config)
{
	struct net_device *dev = phydev->attached_dev;
	int err = -ENOMEM;

	if (!(phydev->drv &&
	      phydev->drv->cable_test_tdr_start &&
	      phydev->drv->cable_test_get_status)) {
		NL_SET_ERR_MSG(extack,
			       "PHY driver does not support cable test TDR");
		return -EOPNOTSUPP;
	}

	mutex_lock(&phydev->lock);
	if (phydev->state == PHY_CABLETEST) {
		NL_SET_ERR_MSG(extack,
			       "PHY already performing a test");
		err = -EBUSY;
		goto out;
	}

	if (phydev->state < PHY_UP ||
	    phydev->state > PHY_CABLETEST) {
		NL_SET_ERR_MSG(extack,
			       "PHY not configured. Try setting interface up");
		err = -EBUSY;
		goto out;
	}

	err = ethnl_cable_test_alloc(phydev, ETHTOOL_MSG_CABLE_TEST_TDR_NTF);
	if (err)
		goto out;

	/* Mark the carrier down until the test is complete */
	phy_link_down(phydev);

	netif_testing_on(dev);
	err = phydev->drv->cable_test_tdr_start(phydev, config);
	if (err) {
		netif_testing_off(dev);
		phy_link_up(phydev);
		goto out_free;
	}

	phydev->state = PHY_CABLETEST;

	if (phy_polling_mode(phydev))
		phy_trigger_machine(phydev);

	mutex_unlock(&phydev->lock);

	return 0;

out_free:
	ethnl_cable_test_free(phydev);
out:
	mutex_unlock(&phydev->lock);

	return err;
}
EXPORT_SYMBOL(phy_start_cable_test_tdr);

int phy_config_aneg(struct phy_device *phydev)
{
	if (phydev->drv->config_aneg)
		return phydev->drv->config_aneg(phydev);

	/* Clause 45 PHYs that don't implement Clause 22 registers are not
	 * allowed to call genphy_config_aneg()
	 */
	if (phydev->is_c45 && !(phydev->c45_ids.devices_in_package & BIT(0)))
		return genphy_c45_config_aneg(phydev);

	return genphy_config_aneg(phydev);
}
EXPORT_SYMBOL(phy_config_aneg);

/**
 * phy_check_link_status - check link status and set state accordingly
 * @phydev: the phy_device struct
 *
 * Description: Check for link and whether autoneg was triggered / is running
 * and set state accordingly
 */
static int phy_check_link_status(struct phy_device *phydev)
{
	int err;

	lockdep_assert_held(&phydev->lock);

	/* Keep previous state if loopback is enabled because some PHYs
	 * report that Link is Down when loopback is enabled.
	 */
	if (phydev->loopback_enabled)
		return 0;

	err = phy_read_status(phydev);
	if (err)
		return err;

	if (phydev->link && phydev->state != PHY_RUNNING) {
		phy_check_downshift(phydev);
		phydev->state = PHY_RUNNING;
		err = genphy_c45_eee_is_active(phydev,
					       NULL, NULL, NULL);
		if (err <= 0)
			phydev->enable_tx_lpi = false;
		else
			phydev->enable_tx_lpi = phydev->eee_cfg.tx_lpi_enabled;

		phy_link_up(phydev);
	} else if (!phydev->link && phydev->state != PHY_NOLINK) {
		phydev->state = PHY_NOLINK;
		phydev->enable_tx_lpi = false;
		phy_link_down(phydev);
	}

	return 0;
}

/**
 * _phy_start_aneg - start auto-negotiation for this PHY device
 * @phydev: the phy_device struct
 *
 * Description: Sanitizes the settings (if we're not autonegotiating
 *   them), and then calls the driver's config_aneg function.
 *   If the PHYCONTROL Layer is operating, we change the state to
 *   reflect the beginning of Auto-negotiation or forcing.
 */
int _phy_start_aneg(struct phy_device *phydev)
{
	int err;

	lockdep_assert_held(&phydev->lock);

	if (!phydev->drv)
		return -EIO;

	if (AUTONEG_DISABLE == phydev->autoneg)
		phy_sanitize_settings(phydev);

	err = phy_config_aneg(phydev);
	if (err < 0)
		return err;

	if (phy_is_started(phydev))
		err = phy_check_link_status(phydev);

	return err;
}
EXPORT_SYMBOL(_phy_start_aneg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_start_aneg - start auto-negotiation for this PHY device
 * @phydev: the phy_device struct
 *
 * Description: Sanitizes the settings (if we're not autonegotiating
 *   them), and then calls the driver's config_aneg function.
 *   If the PHYCONTROL Layer is operating, we change the state to
 *   reflect the beginning of Auto-negotiation or forcing.
 */
int phy_start_aneg(struct phy_device *phydev)
{
	int err;

	mutex_lock(&phydev->lock);
<<<<<<< HEAD

	if (AUTONEG_DISABLE == phydev->autoneg)
		phy_sanitize_settings(phydev);

	err = phydev->drv->config_aneg(phydev);

	if (err < 0)
		goto out_unlock;

	if (phydev->state != PHY_HALTED) {
		if (AUTONEG_ENABLE == phydev->autoneg) {
			phydev->state = PHY_AN;
			phydev->link_timeout = PHY_AN_TIMEOUT;
		} else {
			phydev->state = PHY_FORCING;
			phydev->link_timeout = PHY_FORCE_TIMEOUT;
		}
	}

out_unlock:
	mutex_unlock(&phydev->lock);
=======
	err = _phy_start_aneg(phydev);
	mutex_unlock(&phydev->lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL(phy_start_aneg);

<<<<<<< HEAD

static void phy_change(struct work_struct *work);
=======
static int phy_poll_aneg_done(struct phy_device *phydev)
{
	unsigned int retries = 100;
	int ret;

	do {
		msleep(100);
		ret = phy_aneg_done(phydev);
	} while (!ret && --retries);

	if (!ret)
		return -ETIMEDOUT;

	return ret < 0 ? ret : 0;
}

int phy_ethtool_ksettings_set(struct phy_device *phydev,
			      const struct ethtool_link_ksettings *cmd)
{
	__ETHTOOL_DECLARE_LINK_MODE_MASK(advertising);
	u8 autoneg = cmd->base.autoneg;
	u8 duplex = cmd->base.duplex;
	u32 speed = cmd->base.speed;

	if (cmd->base.phy_address != phydev->mdio.addr)
		return -EINVAL;

	linkmode_copy(advertising, cmd->link_modes.advertising);

	/* We make sure that we don't pass unsupported values in to the PHY */
	linkmode_and(advertising, advertising, phydev->supported);

	/* Verify the settings we care about. */
	if (autoneg != AUTONEG_ENABLE && autoneg != AUTONEG_DISABLE)
		return -EINVAL;

	if (autoneg == AUTONEG_ENABLE && linkmode_empty(advertising))
		return -EINVAL;

	if (autoneg == AUTONEG_DISABLE &&
	    ((speed != SPEED_1000 &&
	      speed != SPEED_100 &&
	      speed != SPEED_10) ||
	     (duplex != DUPLEX_HALF &&
	      duplex != DUPLEX_FULL)))
		return -EINVAL;

	mutex_lock(&phydev->lock);
	phydev->autoneg = autoneg;

	if (autoneg == AUTONEG_DISABLE) {
		phydev->speed = speed;
		phydev->duplex = duplex;
	}

	linkmode_copy(phydev->advertising, advertising);

	linkmode_mod_bit(ETHTOOL_LINK_MODE_Autoneg_BIT,
			 phydev->advertising, autoneg == AUTONEG_ENABLE);

	phydev->master_slave_set = cmd->base.master_slave_cfg;
	phydev->mdix_ctrl = cmd->base.eth_tp_mdix_ctrl;

	/* Restart the PHY */
	if (phy_is_started(phydev)) {
		phydev->state = PHY_UP;
		phy_trigger_machine(phydev);
	} else {
		_phy_start_aneg(phydev);
	}

	mutex_unlock(&phydev->lock);
	return 0;
}
EXPORT_SYMBOL(phy_ethtool_ksettings_set);

/**
 * phy_speed_down - set speed to lowest speed supported by both link partners
 * @phydev: the phy_device struct
 * @sync: perform action synchronously
 *
 * Description: Typically used to save energy when waiting for a WoL packet
 *
 * WARNING: Setting sync to false may cause the system being unable to suspend
 * in case the PHY generates an interrupt when finishing the autonegotiation.
 * This interrupt may wake up the system immediately after suspend.
 * Therefore use sync = false only if you're sure it's safe with the respective
 * network chip.
 */
int phy_speed_down(struct phy_device *phydev, bool sync)
{
	__ETHTOOL_DECLARE_LINK_MODE_MASK(adv_tmp);
	int ret = 0;

	mutex_lock(&phydev->lock);

	if (phydev->autoneg != AUTONEG_ENABLE)
		goto out;

	linkmode_copy(adv_tmp, phydev->advertising);

	ret = phy_speed_down_core(phydev);
	if (ret)
		goto out;

	linkmode_copy(phydev->adv_old, adv_tmp);

	if (linkmode_equal(phydev->advertising, adv_tmp)) {
		ret = 0;
		goto out;
	}

	ret = phy_config_aneg(phydev);
	if (ret)
		goto out;

	ret = sync ? phy_poll_aneg_done(phydev) : 0;
out:
	mutex_unlock(&phydev->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(phy_speed_down);

/**
 * phy_speed_up - (re)set advertised speeds to all supported speeds
 * @phydev: the phy_device struct
 *
 * Description: Used to revert the effect of phy_speed_down
 */
int phy_speed_up(struct phy_device *phydev)
{
	__ETHTOOL_DECLARE_LINK_MODE_MASK(adv_tmp);
	int ret = 0;

	mutex_lock(&phydev->lock);

	if (phydev->autoneg != AUTONEG_ENABLE)
		goto out;

	if (linkmode_empty(phydev->adv_old))
		goto out;

	linkmode_copy(adv_tmp, phydev->advertising);
	linkmode_copy(phydev->advertising, phydev->adv_old);
	linkmode_zero(phydev->adv_old);

	if (linkmode_equal(phydev->advertising, adv_tmp))
		goto out;

	ret = phy_config_aneg(phydev);
out:
	mutex_unlock(&phydev->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(phy_speed_up);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_start_machine - start PHY state machine tracking
 * @phydev: the phy_device struct
<<<<<<< HEAD
 * @handler: callback function for state change notifications
 *
 * Description: The PHY infrastructure can run a state machine
 *   which tracks whether the PHY is starting up, negotiating,
 *   etc.  This function starts the timer which tracks the state
 *   of the PHY.  If you want to be notified when the state changes,
 *   pass in the callback @handler, otherwise, pass NULL.  If you
 *   want to maintain your own state machine, do not call this
 *   function.
 */
void phy_start_machine(struct phy_device *phydev,
		void (*handler)(struct net_device *))
{
	phydev->adjust_state = handler;

	schedule_delayed_work(&phydev->state_queue, HZ);
}
=======
 *
 * Description: The PHY infrastructure can run a state machine
 *   which tracks whether the PHY is starting up, negotiating,
 *   etc.  This function starts the delayed workqueue which tracks
 *   the state of the PHY. If you want to maintain your own state machine,
 *   do not call this function.
 */
void phy_start_machine(struct phy_device *phydev)
{
	phy_trigger_machine(phydev);
}
EXPORT_SYMBOL_GPL(phy_start_machine);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_stop_machine - stop the PHY state machine tracking
 * @phydev: target phy_device struct
 *
<<<<<<< HEAD
 * Description: Stops the state machine timer, sets the state to UP
 *   (unless it wasn't up yet). This function must be called BEFORE
 *   phy_detach.
=======
 * Description: Stops the state machine delayed workqueue, sets the
 *   state to UP (unless it wasn't up yet). This function must be
 *   called BEFORE phy_detach.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void phy_stop_machine(struct phy_device *phydev)
{
	cancel_delayed_work_sync(&phydev->state_queue);

	mutex_lock(&phydev->lock);
<<<<<<< HEAD
	if (phydev->state > PHY_UP)
		phydev->state = PHY_UP;
	mutex_unlock(&phydev->lock);

	phydev->adjust_state = NULL;
}

/**
 * phy_force_reduction - reduce PHY speed/duplex settings by one step
 * @phydev: target phy_device struct
 *
 * Description: Reduces the speed/duplex settings by one notch,
 *   in this order--
 *   1000/FULL, 1000/HALF, 100/FULL, 100/HALF, 10/FULL, 10/HALF.
 *   The function bottoms out at 10/HALF.
 */
static void phy_force_reduction(struct phy_device *phydev)
{
	int idx;

	idx = phy_find_setting(phydev->speed, phydev->duplex);
	
	idx++;

	idx = phy_find_valid(idx, phydev->supported);

	phydev->speed = settings[idx].speed;
	phydev->duplex = settings[idx].duplex;

	pr_info("Trying %d/%s\n", phydev->speed,
			DUPLEX_FULL == phydev->duplex ?
			"FULL" : "HALF");
}


/**
 * phy_error - enter HALTED state for this PHY device
 * @phydev: target phy_device struct
 *
 * Moves the PHY to the HALTED state in response to a read
 * or write error, and tells the controller the link is down.
 * Must not be called from interrupt context, or while the
 * phydev->lock is held.
 */
static void phy_error(struct phy_device *phydev)
{
	mutex_lock(&phydev->lock);
	phydev->state = PHY_HALTED;
	mutex_unlock(&phydev->lock);
=======
	if (phy_is_started(phydev))
		phydev->state = PHY_UP;
	mutex_unlock(&phydev->lock);
}

static void phy_process_error(struct phy_device *phydev)
{
	/* phydev->lock must be held for the state change to be safe */
	if (!mutex_is_locked(&phydev->lock))
		phydev_err(phydev, "PHY-device data unsafe context\n");

	phydev->state = PHY_ERROR;

	phy_trigger_machine(phydev);
}

static void phy_error_precise(struct phy_device *phydev,
			      const void *func, int err)
{
	WARN(1, "%pS: returned: %d\n", func, err);
	phy_process_error(phydev);
}

/**
 * phy_error - enter ERROR state for this PHY device
 * @phydev: target phy_device struct
 *
 * Moves the PHY to the ERROR state in response to a read
 * or write error, and tells the controller the link is down.
 * Must be called with phydev->lock held.
 */
void phy_error(struct phy_device *phydev)
{
	WARN_ON(1);
	phy_process_error(phydev);
}
EXPORT_SYMBOL(phy_error);

/**
 * phy_disable_interrupts - Disable the PHY interrupts from the PHY side
 * @phydev: target phy_device struct
 */
int phy_disable_interrupts(struct phy_device *phydev)
{
	/* Disable PHY interrupts */
	return phy_config_interrupt(phydev, PHY_INTERRUPT_DISABLED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * phy_interrupt - PHY interrupt handler
 * @irq: interrupt line
 * @phy_dat: phy_device pointer
 *
<<<<<<< HEAD
 * Description: When a PHY interrupt occurs, the handler disables
 * interrupts, and schedules a work task to clear the interrupt.
=======
 * Description: Handle PHY interrupt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static irqreturn_t phy_interrupt(int irq, void *phy_dat)
{
	struct phy_device *phydev = phy_dat;
<<<<<<< HEAD

	if (PHY_HALTED == phydev->state)
		return IRQ_NONE;		/* It can't be ours.  */

	/* The MDIO bus is not allowed to be written in interrupt
	 * context, so we need to disable the irq here.  A work
	 * queue will write the PHY to disable and clear the
	 * interrupt, and then reenable the irq line. */
	disable_irq_nosync(irq);
	atomic_inc(&phydev->irq_disable);

	schedule_work(&phydev->phy_queue);

	return IRQ_HANDLED;
=======
	irqreturn_t ret;

	/* Wakeup interrupts may occur during a system sleep transition.
	 * Postpone handling until the PHY has resumed.
	 */
	if (IS_ENABLED(CONFIG_PM_SLEEP) && phydev->irq_suspended) {
		struct net_device *netdev = phydev->attached_dev;

		if (netdev) {
			struct device *parent = netdev->dev.parent;

			if (netdev->wol_enabled)
				pm_system_wakeup();
			else if (device_may_wakeup(&netdev->dev))
				pm_wakeup_dev_event(&netdev->dev, 0, true);
			else if (parent && device_may_wakeup(parent))
				pm_wakeup_dev_event(parent, 0, true);
		}

		phydev->irq_rerun = 1;
		disable_irq_nosync(irq);
		return IRQ_HANDLED;
	}

	mutex_lock(&phydev->lock);
	ret = phydev->drv->handle_interrupt(phydev);
	mutex_unlock(&phydev->lock);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * phy_enable_interrupts - Enable the interrupts from the PHY side
 * @phydev: target phy_device struct
 */
static int phy_enable_interrupts(struct phy_device *phydev)
{
<<<<<<< HEAD
	int err;

	err = phy_clear_interrupt(phydev);

	if (err < 0)
		return err;

	err = phy_config_interrupt(phydev, PHY_INTERRUPT_ENABLED);

	return err;
}

/**
 * phy_disable_interrupts - Disable the PHY interrupts from the PHY side
 * @phydev: target phy_device struct
 */
static int phy_disable_interrupts(struct phy_device *phydev)
{
	int err;

	/* Disable PHY interrupts */
	err = phy_config_interrupt(phydev, PHY_INTERRUPT_DISABLED);

	if (err)
		goto phy_err;

	/* Clear the interrupt */
	err = phy_clear_interrupt(phydev);

	if (err)
		goto phy_err;

	return 0;

phy_err:
	phy_error(phydev);

	return err;
}

/**
 * phy_start_interrupts - request and enable interrupts for a PHY device
 * @phydev: target phy_device struct
 *
 * Description: Request the interrupt for the given PHY.
 *   If this fails, then we set irq to PHY_POLL.
 *   Otherwise, we enable the interrupts in the PHY.
 *   This should only be called with a valid IRQ number.
 *   Returns 0 on success or < 0 on error.
 */
int phy_start_interrupts(struct phy_device *phydev)
{
	int err = 0;

	INIT_WORK(&phydev->phy_queue, phy_change);

	atomic_set(&phydev->irq_disable, 0);
	if (request_irq(phydev->irq, phy_interrupt,
				IRQF_SHARED,
				"phy_interrupt",
				phydev) < 0) {
		printk(KERN_WARNING "%s: Can't get IRQ %d (PHY)\n",
				phydev->bus->name,
				phydev->irq);
		phydev->irq = PHY_POLL;
		return 0;
	}

	err = phy_enable_interrupts(phydev);

	return err;
}
EXPORT_SYMBOL(phy_start_interrupts);

/**
 * phy_stop_interrupts - disable interrupts from a PHY device
 * @phydev: target phy_device struct
 */
int phy_stop_interrupts(struct phy_device *phydev)
{
	int err;

	err = phy_disable_interrupts(phydev);

	if (err)
		phy_error(phydev);

	free_irq(phydev->irq, phydev);

	/*
	 * Cannot call flush_scheduled_work() here as desired because
	 * of rtnl_lock(), but we do not really care about what would
	 * be done, except from enable_irq(), so cancel any work
	 * possibly pending and take care of the matter below.
	 */
	cancel_work_sync(&phydev->phy_queue);
	/*
	 * If work indeed has been cancelled, disable_irq() will have
	 * been left unbalanced from phy_interrupt() and enable_irq()
	 * has to be called so that other devices on the line work.
	 */
	while (atomic_dec_return(&phydev->irq_disable) >= 0)
		enable_irq(phydev->irq);

	return err;
}
EXPORT_SYMBOL(phy_stop_interrupts);


/**
 * phy_change - Scheduled by the phy_interrupt/timer to handle PHY changes
 * @work: work_struct that describes the work to be done
 */
static void phy_change(struct work_struct *work)
{
	int err;
	struct phy_device *phydev =
		container_of(work, struct phy_device, phy_queue);

	if (phydev->drv->did_interrupt &&
	    !phydev->drv->did_interrupt(phydev))
		goto ignore;

	err = phy_disable_interrupts(phydev);

	if (err)
		goto phy_err;

	mutex_lock(&phydev->lock);
	if ((PHY_RUNNING == phydev->state) || (PHY_NOLINK == phydev->state))
		phydev->state = PHY_CHANGELINK;
	mutex_unlock(&phydev->lock);

	atomic_dec(&phydev->irq_disable);
	enable_irq(phydev->irq);

	/* Reenable interrupts */
	if (PHY_HALTED != phydev->state)
		err = phy_config_interrupt(phydev, PHY_INTERRUPT_ENABLED);

	if (err)
		goto irq_enable_err;

	/* reschedule state queue work to run as soon as possible */
	cancel_delayed_work_sync(&phydev->state_queue);
	schedule_delayed_work(&phydev->state_queue, 0);

	return;

ignore:
	atomic_dec(&phydev->irq_disable);
	enable_irq(phydev->irq);
	return;

irq_enable_err:
	disable_irq(phydev->irq);
	atomic_inc(&phydev->irq_disable);
phy_err:
	phy_error(phydev);
=======
	return phy_config_interrupt(phydev, PHY_INTERRUPT_ENABLED);
}

/**
 * phy_request_interrupt - request and enable interrupt for a PHY device
 * @phydev: target phy_device struct
 *
 * Description: Request and enable the interrupt for the given PHY.
 *   If this fails, then we set irq to PHY_POLL.
 *   This should only be called with a valid IRQ number.
 */
void phy_request_interrupt(struct phy_device *phydev)
{
	int err;

	err = request_threaded_irq(phydev->irq, NULL, phy_interrupt,
				   IRQF_ONESHOT | IRQF_SHARED,
				   phydev_name(phydev), phydev);
	if (err) {
		phydev_warn(phydev, "Error %d requesting IRQ %d, falling back to polling\n",
			    err, phydev->irq);
		phydev->irq = PHY_POLL;
	} else {
		if (phy_enable_interrupts(phydev)) {
			phydev_warn(phydev, "Can't enable interrupt, falling back to polling\n");
			phy_free_interrupt(phydev);
			phydev->irq = PHY_POLL;
		}
	}
}
EXPORT_SYMBOL(phy_request_interrupt);

/**
 * phy_free_interrupt - disable and free interrupt for a PHY device
 * @phydev: target phy_device struct
 *
 * Description: Disable and free the interrupt for the given PHY.
 *   This should only be called with a valid IRQ number.
 */
void phy_free_interrupt(struct phy_device *phydev)
{
	phy_disable_interrupts(phydev);
	free_irq(phydev->irq, phydev);
}
EXPORT_SYMBOL(phy_free_interrupt);

enum phy_state_work {
	PHY_STATE_WORK_NONE,
	PHY_STATE_WORK_ANEG,
	PHY_STATE_WORK_SUSPEND,
};

static enum phy_state_work _phy_state_machine(struct phy_device *phydev)
{
	enum phy_state_work state_work = PHY_STATE_WORK_NONE;
	struct net_device *dev = phydev->attached_dev;
	enum phy_state old_state = phydev->state;
	const void *func = NULL;
	bool finished = false;
	int err = 0;

	switch (phydev->state) {
	case PHY_DOWN:
	case PHY_READY:
		break;
	case PHY_UP:
		state_work = PHY_STATE_WORK_ANEG;
		break;
	case PHY_NOLINK:
	case PHY_RUNNING:
		err = phy_check_link_status(phydev);
		func = &phy_check_link_status;
		break;
	case PHY_CABLETEST:
		err = phydev->drv->cable_test_get_status(phydev, &finished);
		if (err) {
			phy_abort_cable_test(phydev);
			netif_testing_off(dev);
			state_work = PHY_STATE_WORK_ANEG;
			phydev->state = PHY_UP;
			break;
		}

		if (finished) {
			ethnl_cable_test_finished(phydev);
			netif_testing_off(dev);
			state_work = PHY_STATE_WORK_ANEG;
			phydev->state = PHY_UP;
		}
		break;
	case PHY_HALTED:
	case PHY_ERROR:
		if (phydev->link) {
			phydev->link = 0;
			phy_link_down(phydev);
		}
		state_work = PHY_STATE_WORK_SUSPEND;
		break;
	}

	if (state_work == PHY_STATE_WORK_ANEG) {
		err = _phy_start_aneg(phydev);
		func = &_phy_start_aneg;
	}

	if (err == -ENODEV)
		return state_work;

	if (err < 0)
		phy_error_precise(phydev, func, err);

	phy_process_state_change(phydev, old_state);

	/* Only re-schedule a PHY state machine change if we are polling the
	 * PHY, if PHY_MAC_INTERRUPT is set, then we will be moving
	 * between states from phy_mac_interrupt().
	 *
	 * In state PHY_HALTED the PHY gets suspended, so rescheduling the
	 * state machine would be pointless and possibly error prone when
	 * called from phy_disconnect() synchronously.
	 */
	if (phy_polling_mode(phydev) && phy_is_started(phydev))
		phy_queue_state_machine(phydev, PHY_STATE_TIME);

	return state_work;
}

/* unlocked part of the PHY state machine */
static void _phy_state_machine_post_work(struct phy_device *phydev,
					 enum phy_state_work state_work)
{
	if (state_work == PHY_STATE_WORK_SUSPEND)
		phy_suspend(phydev);
}

/**
 * phy_state_machine - Handle the state machine
 * @work: work_struct that describes the work to be done
 */
void phy_state_machine(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct phy_device *phydev =
			container_of(dwork, struct phy_device, state_queue);
	enum phy_state_work state_work;

	mutex_lock(&phydev->lock);
	state_work = _phy_state_machine(phydev);
	mutex_unlock(&phydev->lock);

	_phy_state_machine_post_work(phydev, state_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * phy_stop - Bring down the PHY link, and stop checking the status
 * @phydev: target phy_device struct
 */
void phy_stop(struct phy_device *phydev)
{
<<<<<<< HEAD
	mutex_lock(&phydev->lock);

	if (PHY_HALTED == phydev->state)
		goto out_unlock;

	if (phydev->irq != PHY_POLL) {
		/* Disable PHY Interrupts */
		phy_config_interrupt(phydev, PHY_INTERRUPT_DISABLED);

		/* Clear any pending interrupts */
		phy_clear_interrupt(phydev);
	}

	phydev->state = PHY_HALTED;

out_unlock:
	mutex_unlock(&phydev->lock);

	/*
	 * Cannot call flush_scheduled_work() here as desired because
	 * of rtnl_lock(), but PHY_HALTED shall guarantee phy_change()
	 * will not reenable interrupts.
	 */
}

=======
	struct net_device *dev = phydev->attached_dev;
	enum phy_state_work state_work;
	enum phy_state old_state;

	if (!phy_is_started(phydev) && phydev->state != PHY_DOWN &&
	    phydev->state != PHY_ERROR) {
		WARN(1, "called from state %s\n",
		     phy_state_to_str(phydev->state));
		return;
	}

	mutex_lock(&phydev->lock);
	old_state = phydev->state;

	if (phydev->state == PHY_CABLETEST) {
		phy_abort_cable_test(phydev);
		netif_testing_off(dev);
	}

	if (phydev->sfp_bus)
		sfp_upstream_stop(phydev->sfp_bus);

	phydev->state = PHY_HALTED;
	phy_process_state_change(phydev, old_state);

	state_work = _phy_state_machine(phydev);
	mutex_unlock(&phydev->lock);

	_phy_state_machine_post_work(phydev, state_work);
	phy_stop_machine(phydev);

	/* Cannot call flush_scheduled_work() here as desired because
	 * of rtnl_lock(), but PHY_HALTED shall guarantee irq handler
	 * will not reenable interrupts.
	 */
}
EXPORT_SYMBOL(phy_stop);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * phy_start - start or restart a PHY device
 * @phydev: target phy_device struct
 *
 * Description: Indicates the attached device's readiness to
 *   handle PHY-related work.  Used during startup to start the
 *   PHY, and after a call to phy_stop() to resume operation.
 *   Also used to indicate the MDIO bus has cleared an error
 *   condition.
 */
void phy_start(struct phy_device *phydev)
{
	mutex_lock(&phydev->lock);

<<<<<<< HEAD
	switch (phydev->state) {
		case PHY_STARTING:
			phydev->state = PHY_PENDING;
			break;
		case PHY_READY:
			phydev->state = PHY_UP;
			break;
		case PHY_HALTED:
			phydev->state = PHY_RESUMING;
		default:
			break;
	}
	mutex_unlock(&phydev->lock);
}
EXPORT_SYMBOL(phy_stop);
EXPORT_SYMBOL(phy_start);

/**
 * phy_state_machine - Handle the state machine
 * @work: work_struct that describes the work to be done
 */
void phy_state_machine(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct phy_device *phydev =
			container_of(dwork, struct phy_device, state_queue);
	int needs_aneg = 0;
	int err = 0;

	mutex_lock(&phydev->lock);

	if (phydev->adjust_state)
		phydev->adjust_state(phydev->attached_dev);

	switch(phydev->state) {
		case PHY_DOWN:
		case PHY_STARTING:
		case PHY_READY:
		case PHY_PENDING:
			break;
		case PHY_UP:
			needs_aneg = 1;

			phydev->link_timeout = PHY_AN_TIMEOUT;

			break;
		case PHY_AN:
			err = phy_read_status(phydev);

			if (err < 0)
				break;

			/* If the link is down, give up on
			 * negotiation for now */
			if (!phydev->link) {
				phydev->state = PHY_NOLINK;
				netif_carrier_off(phydev->attached_dev);
				phydev->adjust_link(phydev->attached_dev);
				break;
			}

			/* Check if negotiation is done.  Break
			 * if there's an error */
			err = phy_aneg_done(phydev);
			if (err < 0)
				break;

			/* If AN is done, we're running */
			if (err > 0) {
				phydev->state = PHY_RUNNING;
				netif_carrier_on(phydev->attached_dev);
				phydev->adjust_link(phydev->attached_dev);

			} else if (0 == phydev->link_timeout--) {
				int idx;

				needs_aneg = 1;
				/* If we have the magic_aneg bit,
				 * we try again */
				if (phydev->drv->flags & PHY_HAS_MAGICANEG)
					break;

				/* The timer expired, and we still
				 * don't have a setting, so we try
				 * forcing it until we find one that
				 * works, starting from the fastest speed,
				 * and working our way down */
				idx = phy_find_valid(0, phydev->supported);

				phydev->speed = settings[idx].speed;
				phydev->duplex = settings[idx].duplex;

				phydev->autoneg = AUTONEG_DISABLE;

				pr_info("Trying %d/%s\n", phydev->speed,
						DUPLEX_FULL ==
						phydev->duplex ?
						"FULL" : "HALF");
			}
			break;
		case PHY_NOLINK:
			err = phy_read_status(phydev);

			if (err)
				break;

			if (phydev->link) {
				phydev->state = PHY_RUNNING;
				netif_carrier_on(phydev->attached_dev);
				phydev->adjust_link(phydev->attached_dev);
			}
			break;
		case PHY_FORCING:
			err = genphy_update_link(phydev);

			if (err)
				break;

			if (phydev->link) {
				phydev->state = PHY_RUNNING;
				netif_carrier_on(phydev->attached_dev);
			} else {
				if (0 == phydev->link_timeout--) {
					phy_force_reduction(phydev);
					needs_aneg = 1;
				}
			}

			phydev->adjust_link(phydev->attached_dev);
			break;
		case PHY_RUNNING:
			/* Only register a CHANGE if we are
			 * polling */
			if (PHY_POLL == phydev->irq)
				phydev->state = PHY_CHANGELINK;
			break;
		case PHY_CHANGELINK:
			err = phy_read_status(phydev);

			if (err)
				break;

			if (phydev->link) {
				phydev->state = PHY_RUNNING;
				netif_carrier_on(phydev->attached_dev);
			} else {
				phydev->state = PHY_NOLINK;
				netif_carrier_off(phydev->attached_dev);
			}

			phydev->adjust_link(phydev->attached_dev);

			if (PHY_POLL != phydev->irq)
				err = phy_config_interrupt(phydev,
						PHY_INTERRUPT_ENABLED);
			break;
		case PHY_HALTED:
			if (phydev->link) {
				phydev->link = 0;
				netif_carrier_off(phydev->attached_dev);
				phydev->adjust_link(phydev->attached_dev);
			}
			break;
		case PHY_RESUMING:

			err = phy_clear_interrupt(phydev);

			if (err)
				break;

			err = phy_config_interrupt(phydev,
					PHY_INTERRUPT_ENABLED);

			if (err)
				break;

			if (AUTONEG_ENABLE == phydev->autoneg) {
				err = phy_aneg_done(phydev);
				if (err < 0)
					break;

				/* err > 0 if AN is done.
				 * Otherwise, it's 0, and we're
				 * still waiting for AN */
				if (err > 0) {
					err = phy_read_status(phydev);
					if (err)
						break;

					if (phydev->link) {
						phydev->state = PHY_RUNNING;
						netif_carrier_on(phydev->attached_dev);
					} else
						phydev->state = PHY_NOLINK;
					phydev->adjust_link(phydev->attached_dev);
				} else {
					phydev->state = PHY_AN;
					phydev->link_timeout = PHY_AN_TIMEOUT;
				}
			} else {
				err = phy_read_status(phydev);
				if (err)
					break;

				if (phydev->link) {
					phydev->state = PHY_RUNNING;
					netif_carrier_on(phydev->attached_dev);
				} else
					phydev->state = PHY_NOLINK;
				phydev->adjust_link(phydev->attached_dev);
			}
			break;
	}

	mutex_unlock(&phydev->lock);

	if (needs_aneg)
		err = phy_start_aneg(phydev);

	if (err < 0)
		phy_error(phydev);

	schedule_delayed_work(&phydev->state_queue, PHY_STATE_TIME * HZ);
}
=======
	if (phydev->state != PHY_READY && phydev->state != PHY_HALTED) {
		WARN(1, "called from state %s\n",
		     phy_state_to_str(phydev->state));
		goto out;
	}

	if (phydev->sfp_bus)
		sfp_upstream_start(phydev->sfp_bus);

	/* if phy was suspended, bring the physical link up again */
	__phy_resume(phydev);

	phydev->state = PHY_UP;

	phy_start_machine(phydev);
out:
	mutex_unlock(&phydev->lock);
}
EXPORT_SYMBOL(phy_start);

/**
 * phy_mac_interrupt - MAC says the link has changed
 * @phydev: phy_device struct with changed link
 *
 * The MAC layer is able to indicate there has been a change in the PHY link
 * status. Trigger the state machine and work a work queue.
 */
void phy_mac_interrupt(struct phy_device *phydev)
{
	/* Trigger a state machine change */
	phy_trigger_machine(phydev);
}
EXPORT_SYMBOL(phy_mac_interrupt);

/**
 * phy_init_eee - init and check the EEE feature
 * @phydev: target phy_device struct
 * @clk_stop_enable: PHY may stop the clock during LPI
 *
 * Description: it checks if the Energy-Efficient Ethernet (EEE)
 * is supported by looking at the MMD registers 3.20 and 7.60/61
 * and it programs the MMD register 3.0 setting the "Clock stop enable"
 * bit if required.
 */
int phy_init_eee(struct phy_device *phydev, bool clk_stop_enable)
{
	int ret;

	if (!phydev->drv)
		return -EIO;

	ret = genphy_c45_eee_is_active(phydev, NULL, NULL, NULL);
	if (ret < 0)
		return ret;
	if (!ret)
		return -EPROTONOSUPPORT;

	if (clk_stop_enable)
		/* Configure the PHY to stop receiving xMII
		 * clock while it is signaling LPI.
		 */
		ret = phy_set_bits_mmd(phydev, MDIO_MMD_PCS, MDIO_CTRL1,
				       MDIO_PCS_CTRL1_CLKSTOP_EN);

	return ret < 0 ? ret : 0;
}
EXPORT_SYMBOL(phy_init_eee);

/**
 * phy_get_eee_err - report the EEE wake error count
 * @phydev: target phy_device struct
 *
 * Description: it is to report the number of time where the PHY
 * failed to complete its normal wake sequence.
 */
int phy_get_eee_err(struct phy_device *phydev)
{
	int ret;

	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	ret = phy_read_mmd(phydev, MDIO_MMD_PCS, MDIO_PCS_EEE_WK_ERR);
	mutex_unlock(&phydev->lock);

	return ret;
}
EXPORT_SYMBOL(phy_get_eee_err);

/**
 * phy_ethtool_get_eee - get EEE supported and status
 * @phydev: target phy_device struct
 * @data: ethtool_keee data
 *
 * Description: reports the Supported/Advertisement/LP Advertisement
 * capabilities, etc.
 */
int phy_ethtool_get_eee(struct phy_device *phydev, struct ethtool_keee *data)
{
	int ret;

	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	ret = genphy_c45_ethtool_get_eee(phydev, data);
	eeecfg_to_eee(data, &phydev->eee_cfg);
	mutex_unlock(&phydev->lock);

	return ret;
}
EXPORT_SYMBOL(phy_ethtool_get_eee);

/**
 * phy_ethtool_set_eee_noneg - Adjusts MAC LPI configuration without PHY
 *			       renegotiation
 * @phydev: pointer to the target PHY device structure
 * @data: pointer to the ethtool_keee structure containing the new EEE settings
 *
 * This function updates the Energy Efficient Ethernet (EEE) configuration
 * for cases where only the MAC's Low Power Idle (LPI) configuration changes,
 * without triggering PHY renegotiation. It ensures that the MAC is properly
 * informed of the new LPI settings by cycling the link down and up, which
 * is necessary for the MAC to adopt the new configuration. This adjustment
 * is done only if there is a change in the tx_lpi_enabled or tx_lpi_timer
 * configuration.
 */
static void phy_ethtool_set_eee_noneg(struct phy_device *phydev,
				      struct ethtool_keee *data)
{
	if (phydev->eee_cfg.tx_lpi_enabled != data->tx_lpi_enabled ||
	    phydev->eee_cfg.tx_lpi_timer != data->tx_lpi_timer) {
		eee_to_eeecfg(&phydev->eee_cfg, data);
		phydev->enable_tx_lpi = eeecfg_mac_can_tx_lpi(&phydev->eee_cfg);
		if (phydev->link) {
			phydev->link = false;
			phy_link_down(phydev);
			phydev->link = true;
			phy_link_up(phydev);
		}
	}
}

/**
 * phy_ethtool_set_eee - set EEE supported and status
 * @phydev: target phy_device struct
 * @data: ethtool_keee data
 *
 * Description: it is to program the Advertisement EEE register.
 */
int phy_ethtool_set_eee(struct phy_device *phydev, struct ethtool_keee *data)
{
	int ret;

	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	ret = genphy_c45_ethtool_set_eee(phydev, data);
	if (ret >= 0) {
		if (ret == 0)
			phy_ethtool_set_eee_noneg(phydev, data);
		eee_to_eeecfg(&phydev->eee_cfg, data);
	}
	mutex_unlock(&phydev->lock);

	return ret < 0 ? ret : 0;
}
EXPORT_SYMBOL(phy_ethtool_set_eee);

/**
 * phy_ethtool_set_wol - Configure Wake On LAN
 *
 * @phydev: target phy_device struct
 * @wol: Configuration requested
 */
int phy_ethtool_set_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	int ret;

	if (phydev->drv && phydev->drv->set_wol) {
		mutex_lock(&phydev->lock);
		ret = phydev->drv->set_wol(phydev, wol);
		mutex_unlock(&phydev->lock);

		return ret;
	}

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(phy_ethtool_set_wol);

/**
 * phy_ethtool_get_wol - Get the current Wake On LAN configuration
 *
 * @phydev: target phy_device struct
 * @wol: Store the current configuration here
 */
void phy_ethtool_get_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol)
{
	if (phydev->drv && phydev->drv->get_wol) {
		mutex_lock(&phydev->lock);
		phydev->drv->get_wol(phydev, wol);
		mutex_unlock(&phydev->lock);
	}
}
EXPORT_SYMBOL(phy_ethtool_get_wol);

int phy_ethtool_get_link_ksettings(struct net_device *ndev,
				   struct ethtool_link_ksettings *cmd)
{
	struct phy_device *phydev = ndev->phydev;

	if (!phydev)
		return -ENODEV;

	phy_ethtool_ksettings_get(phydev, cmd);

	return 0;
}
EXPORT_SYMBOL(phy_ethtool_get_link_ksettings);

int phy_ethtool_set_link_ksettings(struct net_device *ndev,
				   const struct ethtool_link_ksettings *cmd)
{
	struct phy_device *phydev = ndev->phydev;

	if (!phydev)
		return -ENODEV;

	return phy_ethtool_ksettings_set(phydev, cmd);
}
EXPORT_SYMBOL(phy_ethtool_set_link_ksettings);

/**
 * phy_ethtool_nway_reset - Restart auto negotiation
 * @ndev: Network device to restart autoneg for
 */
int phy_ethtool_nway_reset(struct net_device *ndev)
{
	struct phy_device *phydev = ndev->phydev;
	int ret;

	if (!phydev)
		return -ENODEV;

	if (!phydev->drv)
		return -EIO;

	mutex_lock(&phydev->lock);
	ret = phy_restart_aneg(phydev);
	mutex_unlock(&phydev->lock);

	return ret;
}
EXPORT_SYMBOL(phy_ethtool_nway_reset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
