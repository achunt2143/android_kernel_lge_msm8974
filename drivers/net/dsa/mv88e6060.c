<<<<<<< HEAD
/*
 * net/dsa/mv88e6060.c - Driver for Marvell 88e6060 switch chips
 * Copyright (c) 2008-2009 Marvell Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * net/dsa/mv88e6060.c - Driver for Marvell 88e6060 switch chips
 * Copyright (c) 2008-2009 Marvell Semiconductor
 */

#include <linux/delay.h>
#include <linux/etherdevice.h>
#include <linux/jiffies.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/list.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <net/dsa.h>
<<<<<<< HEAD

#define REG_PORT(p)		(8 + (p))
#define REG_GLOBAL		0x0f

static int reg_read(struct dsa_switch *ds, int addr, int reg)
{
	return mdiobus_read(ds->master_mii_bus, ds->pd->sw_addr + addr, reg);
}

#define REG_READ(addr, reg)					\
	({							\
		int __ret;					\
								\
		__ret = reg_read(ds, addr, reg);		\
		if (__ret < 0)					\
			return __ret;				\
		__ret;						\
	})


static int reg_write(struct dsa_switch *ds, int addr, int reg, u16 val)
{
	return mdiobus_write(ds->master_mii_bus, ds->pd->sw_addr + addr,
			     reg, val);
}

#define REG_WRITE(addr, reg, val)				\
	({							\
		int __ret;					\
								\
		__ret = reg_write(ds, addr, reg, val);		\
		if (__ret < 0)					\
			return __ret;				\
	})

static char *mv88e6060_probe(struct mii_bus *bus, int sw_addr)
{
	int ret;

	ret = mdiobus_read(bus, sw_addr + REG_PORT(0), 0x03);
	if (ret >= 0) {
		ret &= 0xfff0;
		if (ret == 0x0600)
=======
#include "mv88e6060.h"

static int reg_read(struct mv88e6060_priv *priv, int addr, int reg)
{
	return mdiobus_read_nested(priv->bus, priv->sw_addr + addr, reg);
}

static int reg_write(struct mv88e6060_priv *priv, int addr, int reg, u16 val)
{
	return mdiobus_write_nested(priv->bus, priv->sw_addr + addr, reg, val);
}

static const char *mv88e6060_get_name(struct mii_bus *bus, int sw_addr)
{
	int ret;

	ret = mdiobus_read(bus, sw_addr + REG_PORT(0), PORT_SWITCH_ID);
	if (ret >= 0) {
		if (ret == PORT_SWITCH_ID_6060)
			return "Marvell 88E6060 (A0)";
		if (ret == PORT_SWITCH_ID_6060_R1 ||
		    ret == PORT_SWITCH_ID_6060_R2)
			return "Marvell 88E6060 (B0)";
		if ((ret & PORT_SWITCH_ID_6060_MASK) == PORT_SWITCH_ID_6060)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return "Marvell 88E6060";
	}

	return NULL;
}

<<<<<<< HEAD
static int mv88e6060_switch_reset(struct dsa_switch *ds)
{
	int i;
	int ret;

	/*
	 * Set all ports to the disabled state.
	 */
	for (i = 0; i < 6; i++) {
		ret = REG_READ(REG_PORT(i), 0x04);
		REG_WRITE(REG_PORT(i), 0x04, ret & 0xfffc);
	}

	/*
	 * Wait for transmit queues to drain.
	 */
	msleep(2);

	/*
	 * Reset the switch.
	 */
	REG_WRITE(REG_GLOBAL, 0x0a, 0xa130);

	/*
	 * Wait up to one second for reset to complete.
	 */
	for (i = 0; i < 1000; i++) {
		ret = REG_READ(REG_GLOBAL, 0x00);
		if ((ret & 0x8000) == 0x0000)
			break;

		msleep(1);
	}
	if (i == 1000)
=======
static enum dsa_tag_protocol mv88e6060_get_tag_protocol(struct dsa_switch *ds,
							int port,
							enum dsa_tag_protocol m)
{
	return DSA_TAG_PROTO_TRAILER;
}

static int mv88e6060_switch_reset(struct mv88e6060_priv *priv)
{
	int i;
	int ret;
	unsigned long timeout;

	/* Set all ports to the disabled state. */
	for (i = 0; i < MV88E6060_PORTS; i++) {
		ret = reg_read(priv, REG_PORT(i), PORT_CONTROL);
		if (ret < 0)
			return ret;
		ret = reg_write(priv, REG_PORT(i), PORT_CONTROL,
				ret & ~PORT_CONTROL_STATE_MASK);
		if (ret)
			return ret;
	}

	/* Wait for transmit queues to drain. */
	usleep_range(2000, 4000);

	/* Reset the switch. */
	ret = reg_write(priv, REG_GLOBAL, GLOBAL_ATU_CONTROL,
			GLOBAL_ATU_CONTROL_SWRESET |
			GLOBAL_ATU_CONTROL_LEARNDIS);
	if (ret)
		return ret;

	/* Wait up to one second for reset to complete. */
	timeout = jiffies + 1 * HZ;
	while (time_before(jiffies, timeout)) {
		ret = reg_read(priv, REG_GLOBAL, GLOBAL_STATUS);
		if (ret < 0)
			return ret;

		if (ret & GLOBAL_STATUS_INIT_READY)
			break;

		usleep_range(1000, 2000);
	}
	if (time_after(jiffies, timeout))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ETIMEDOUT;

	return 0;
}

<<<<<<< HEAD
static int mv88e6060_setup_global(struct dsa_switch *ds)
{
	/*
	 * Disable discarding of frames with excessive collisions,
	 * set the maximum frame size to 1536 bytes, and mask all
	 * interrupt sources.
	 */
	REG_WRITE(REG_GLOBAL, 0x04, 0x0800);

	/*
	 * Enable automatic address learning, set the address
	 * database size to 1024 entries, and set the default aging
	 * time to 5 minutes.
	 */
	REG_WRITE(REG_GLOBAL, 0x0a, 0x2130);

	return 0;
}

static int mv88e6060_setup_port(struct dsa_switch *ds, int p)
{
	int addr = REG_PORT(p);

	/*
	 * Do not force flow control, disable Ingress and Egress
=======
static int mv88e6060_setup_global(struct mv88e6060_priv *priv)
{
	int ret;

	/* Disable discarding of frames with excessive collisions,
	 * set the maximum frame size to 1536 bytes, and mask all
	 * interrupt sources.
	 */
	ret = reg_write(priv, REG_GLOBAL, GLOBAL_CONTROL,
			GLOBAL_CONTROL_MAX_FRAME_1536);
	if (ret)
		return ret;

	/* Disable automatic address learning.
	 */
	return reg_write(priv, REG_GLOBAL, GLOBAL_ATU_CONTROL,
			 GLOBAL_ATU_CONTROL_LEARNDIS);
}

static int mv88e6060_setup_port(struct mv88e6060_priv *priv, int p)
{
	int addr = REG_PORT(p);
	int ret;

	if (dsa_is_unused_port(priv->ds, p))
		return 0;

	/* Do not force flow control, disable Ingress and Egress
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Header tagging, disable VLAN tunneling, and set the port
	 * state to Forwarding.  Additionally, if this is the CPU
	 * port, enable Ingress and Egress Trailer tagging mode.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, 0x04, dsa_is_cpu_port(ds, p) ?  0x4103 : 0x0003);

	/*
	 * Port based VLAN map: give each port its own address
=======
	ret = reg_write(priv, addr, PORT_CONTROL,
			dsa_is_cpu_port(priv->ds, p) ?
			PORT_CONTROL_TRAILER |
			PORT_CONTROL_INGRESS_MODE |
			PORT_CONTROL_STATE_FORWARDING :
			PORT_CONTROL_STATE_FORWARDING);
	if (ret)
		return ret;

	/* Port based VLAN map: give each port its own address
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * database, allow the CPU port to talk to each of the 'real'
	 * ports, and allow each of the 'real' ports to only talk to
	 * the CPU port.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, 0x06,
			((p & 0xf) << 12) |
			 (dsa_is_cpu_port(ds, p) ?
				ds->phys_port_mask :
				(1 << ds->dst->cpu_port)));

	/*
	 * Port Association Vector: when learning source addresses
=======
	ret = reg_write(priv, addr, PORT_VLAN_MAP,
			((p & 0xf) << PORT_VLAN_MAP_DBNUM_SHIFT) |
			(dsa_is_cpu_port(priv->ds, p) ?
			 dsa_user_ports(priv->ds) :
			 BIT(dsa_to_port(priv->ds, p)->cpu_dp->index)));
	if (ret)
		return ret;

	/* Port Association Vector: when learning source addresses
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * of packets, add the address to the address database using
	 * a port bitmap that has only the bit for this port set and
	 * the other bits clear.
	 */
<<<<<<< HEAD
	REG_WRITE(addr, 0x0b, 1 << p);

	return 0;
=======
	return reg_write(priv, addr, PORT_ASSOC_VECTOR, BIT(p));
}

static int mv88e6060_setup_addr(struct mv88e6060_priv *priv)
{
	u8 addr[ETH_ALEN];
	int ret;
	u16 val;

	eth_random_addr(addr);

	val = addr[0] << 8 | addr[1];

	/* The multicast bit is always transmitted as a zero, so the switch uses
	 * bit 8 for "DiffAddr", where 0 means all ports transmit the same SA.
	 */
	val &= 0xfeff;

	ret = reg_write(priv, REG_GLOBAL, GLOBAL_MAC_01, val);
	if (ret)
		return ret;

	ret = reg_write(priv, REG_GLOBAL, GLOBAL_MAC_23,
			(addr[2] << 8) | addr[3]);
	if (ret)
		return ret;

	return reg_write(priv, REG_GLOBAL, GLOBAL_MAC_45,
			 (addr[4] << 8) | addr[5]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mv88e6060_setup(struct dsa_switch *ds)
{
<<<<<<< HEAD
	int i;
	int ret;

	ret = mv88e6060_switch_reset(ds);
=======
	struct mv88e6060_priv *priv = ds->priv;
	int ret;
	int i;

	priv->ds = ds;

	ret = mv88e6060_switch_reset(priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return ret;

	/* @@@ initialise atu */

<<<<<<< HEAD
	ret = mv88e6060_setup_global(ds);
	if (ret < 0)
		return ret;

	for (i = 0; i < 6; i++) {
		ret = mv88e6060_setup_port(ds, i);
=======
	ret = mv88e6060_setup_global(priv);
	if (ret < 0)
		return ret;

	ret = mv88e6060_setup_addr(priv);
	if (ret < 0)
		return ret;

	for (i = 0; i < MV88E6060_PORTS; i++) {
		ret = mv88e6060_setup_port(priv, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int mv88e6060_set_addr(struct dsa_switch *ds, u8 *addr)
{
	REG_WRITE(REG_GLOBAL, 0x01, (addr[0] << 8) | addr[1]);
	REG_WRITE(REG_GLOBAL, 0x02, (addr[2] << 8) | addr[3]);
	REG_WRITE(REG_GLOBAL, 0x03, (addr[4] << 8) | addr[5]);

	return 0;
}

static int mv88e6060_port_to_phy_addr(int port)
{
	if (port >= 0 && port <= 5)
=======
static int mv88e6060_port_to_phy_addr(int port)
{
	if (port >= 0 && port < MV88E6060_PORTS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return port;
	return -1;
}

static int mv88e6060_phy_read(struct dsa_switch *ds, int port, int regnum)
{
<<<<<<< HEAD
=======
	struct mv88e6060_priv *priv = ds->priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int addr;

	addr = mv88e6060_port_to_phy_addr(port);
	if (addr == -1)
		return 0xffff;

<<<<<<< HEAD
	return reg_read(ds, addr, regnum);
=======
	return reg_read(priv, addr, regnum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
mv88e6060_phy_write(struct dsa_switch *ds, int port, int regnum, u16 val)
{
<<<<<<< HEAD
=======
	struct mv88e6060_priv *priv = ds->priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int addr;

	addr = mv88e6060_port_to_phy_addr(port);
	if (addr == -1)
		return 0xffff;

<<<<<<< HEAD
	return reg_write(ds, addr, regnum, val);
}

static void mv88e6060_poll_link(struct dsa_switch *ds)
{
	int i;

	for (i = 0; i < DSA_MAX_PORTS; i++) {
		struct net_device *dev;
		int uninitialized_var(port_status);
		int link;
		int speed;
		int duplex;
		int fc;

		dev = ds->ports[i];
		if (dev == NULL)
			continue;

		link = 0;
		if (dev->flags & IFF_UP) {
			port_status = reg_read(ds, REG_PORT(i), 0x00);
			if (port_status < 0)
				continue;

			link = !!(port_status & 0x1000);
		}

		if (!link) {
			if (netif_carrier_ok(dev)) {
				printk(KERN_INFO "%s: link down\n", dev->name);
				netif_carrier_off(dev);
			}
			continue;
		}

		speed = (port_status & 0x0100) ? 100 : 10;
		duplex = (port_status & 0x0200) ? 1 : 0;
		fc = ((port_status & 0xc000) == 0xc000) ? 1 : 0;

		if (!netif_carrier_ok(dev)) {
			printk(KERN_INFO "%s: link up, %d Mb/s, %s duplex, "
					 "flow control %sabled\n", dev->name,
					 speed, duplex ? "full" : "half",
					 fc ? "en" : "dis");
			netif_carrier_on(dev);
		}
	}
}

static struct dsa_switch_driver mv88e6060_switch_driver = {
	.tag_protocol	= htons(ETH_P_TRAILER),
	.probe		= mv88e6060_probe,
	.setup		= mv88e6060_setup,
	.set_addr	= mv88e6060_set_addr,
	.phy_read	= mv88e6060_phy_read,
	.phy_write	= mv88e6060_phy_write,
	.poll_link	= mv88e6060_poll_link,
};

static int __init mv88e6060_init(void)
{
	register_switch_driver(&mv88e6060_switch_driver);
	return 0;
}
module_init(mv88e6060_init);

static void __exit mv88e6060_cleanup(void)
{
	unregister_switch_driver(&mv88e6060_switch_driver);
}
module_exit(mv88e6060_cleanup);
=======
	return reg_write(priv, addr, regnum, val);
}

static void mv88e6060_phylink_get_caps(struct dsa_switch *ds, int port,
				       struct phylink_config *config)
{
	unsigned long *interfaces = config->supported_interfaces;
	struct mv88e6060_priv *priv = ds->priv;
	int addr = REG_PORT(port);
	int ret;

	ret = reg_read(priv, addr, PORT_STATUS);
	if (ret < 0) {
		dev_err(ds->dev,
			"port %d: unable to read status register: %pe\n",
			port, ERR_PTR(ret));
		return;
	}

	/* If the port is configured in SNI mode (acts as a 10Mbps PHY),
	 * it should have phy-mode = "sni", but that doesn't yet exist, so
	 * forcibly fail validation until the need arises to introduce it.
	 */
	if (!(ret & PORT_STATUS_PORTMODE)) {
		dev_warn(ds->dev, "port %d: SNI mode not supported\n", port);
		return;
	}

	config->mac_capabilities = MAC_100 | MAC_10 | MAC_SYM_PAUSE;

	if (port >= 4) {
		/* Ports 4 and 5 can support MII, REVMII and REVRMII modes */
		__set_bit(PHY_INTERFACE_MODE_MII, interfaces);
		__set_bit(PHY_INTERFACE_MODE_REVMII, interfaces);
		__set_bit(PHY_INTERFACE_MODE_REVRMII, interfaces);
	}
	if (port <= 4) {
		/* Ports 0 to 3 have internal PHYs, and port 4 can optionally
		 * use an internal PHY.
		 */
		/* Internal PHY */
		__set_bit(PHY_INTERFACE_MODE_INTERNAL, interfaces);
		/* Default phylib interface mode */
		__set_bit(PHY_INTERFACE_MODE_GMII, interfaces);
	}
}

static const struct dsa_switch_ops mv88e6060_switch_ops = {
	.get_tag_protocol = mv88e6060_get_tag_protocol,
	.setup		= mv88e6060_setup,
	.phy_read	= mv88e6060_phy_read,
	.phy_write	= mv88e6060_phy_write,
	.phylink_get_caps = mv88e6060_phylink_get_caps,
};

static int mv88e6060_probe(struct mdio_device *mdiodev)
{
	struct device *dev = &mdiodev->dev;
	struct mv88e6060_priv *priv;
	struct dsa_switch *ds;
	const char *name;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->bus = mdiodev->bus;
	priv->sw_addr = mdiodev->addr;

	name = mv88e6060_get_name(priv->bus, priv->sw_addr);
	if (!name)
		return -ENODEV;

	dev_info(dev, "switch %s detected\n", name);

	ds = devm_kzalloc(dev, sizeof(*ds), GFP_KERNEL);
	if (!ds)
		return -ENOMEM;

	ds->dev = dev;
	ds->num_ports = MV88E6060_PORTS;
	ds->priv = priv;
	ds->dev = dev;
	ds->ops = &mv88e6060_switch_ops;

	dev_set_drvdata(dev, ds);

	return dsa_register_switch(ds);
}

static void mv88e6060_remove(struct mdio_device *mdiodev)
{
	struct dsa_switch *ds = dev_get_drvdata(&mdiodev->dev);

	if (!ds)
		return;

	dsa_unregister_switch(ds);
}

static void mv88e6060_shutdown(struct mdio_device *mdiodev)
{
	struct dsa_switch *ds = dev_get_drvdata(&mdiodev->dev);

	if (!ds)
		return;

	dsa_switch_shutdown(ds);

	dev_set_drvdata(&mdiodev->dev, NULL);
}

static const struct of_device_id mv88e6060_of_match[] = {
	{
		.compatible = "marvell,mv88e6060",
	},
	{ /* sentinel */ },
};

static struct mdio_driver mv88e6060_driver = {
	.probe	= mv88e6060_probe,
	.remove = mv88e6060_remove,
	.shutdown = mv88e6060_shutdown,
	.mdiodrv.driver = {
		.name = "mv88e6060",
		.of_match_table = mv88e6060_of_match,
	},
};

mdio_module_driver(mv88e6060_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Lennert Buytenhek <buytenh@wantstofly.org>");
MODULE_DESCRIPTION("Driver for Marvell 88E6060 ethernet switch chip");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mv88e6060");
