<<<<<<< HEAD
/**********************************************************************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2010 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
**********************************************************************/
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * This file is based on code from OCTEON SDK by Cavium Networks.
 *
 * Copyright (c) 2003-2010 Cavium Networks
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * External interface for the Cavium Octeon ethernet driver.
 */
#ifndef OCTEON_ETHERNET_H
#define OCTEON_ETHERNET_H

<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/phy.h>

#ifdef CONFIG_CAVIUM_OCTEON_SOC

#include <asm/octeon/octeon.h>

#include <asm/octeon/cvmx-asxx-defs.h>
#include <asm/octeon/cvmx-config.h>
#include <asm/octeon/cvmx-fau.h>
#include <asm/octeon/cvmx-gmxx-defs.h>
#include <asm/octeon/cvmx-helper.h>
#include <asm/octeon/cvmx-helper-util.h>
#include <asm/octeon/cvmx-ipd.h>
#include <asm/octeon/cvmx-ipd-defs.h>
#include <asm/octeon/cvmx-npi-defs.h>
#include <asm/octeon/cvmx-pip.h>
#include <asm/octeon/cvmx-pko.h>
#include <asm/octeon/cvmx-pow.h>
#include <asm/octeon/cvmx-scratch.h>
#include <asm/octeon/cvmx-spi.h>
#include <asm/octeon/cvmx-spxx-defs.h>
#include <asm/octeon/cvmx-stxx-defs.h>
#include <asm/octeon/cvmx-wqe.h>

#else

#include "octeon-stubs.h"

#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * This is the definition of the Ethernet driver's private
 * driver state stored in netdev_priv(dev).
 */
struct octeon_ethernet {
	/* PKO hardware output port */
	int port;
	/* PKO hardware queue for the port */
	int queue;
	/* Hardware fetch and add to count outstanding tx buffers */
	int fau;
<<<<<<< HEAD
=======
	/* My netdev. */
	struct net_device *netdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Type of port. This is one of the enums in
	 * cvmx_helper_interface_mode_t
	 */
	int imode;
<<<<<<< HEAD
	/* List of outstanding tx buffers per queue */
	struct sk_buff_head tx_free_list[16];
	/* Device statistics */
	struct net_device_stats stats;
	struct phy_device *phydev;
	unsigned int last_link;
	/* Last negotiated link state */
	uint64_t link_info;
	/* Called periodically to check link status */
	void (*poll) (struct net_device *dev);
	struct delayed_work	port_periodic_work;
	struct work_struct	port_work;	/* may be unused. */
=======
	/* PHY mode */
	phy_interface_t phy_mode;
	/* List of outstanding tx buffers per queue */
	struct sk_buff_head tx_free_list[16];
	unsigned int last_speed;
	unsigned int last_link;
	/* Last negotiated link state */
	u64 link_info;
	/* Called periodically to check link status */
	void (*poll)(struct net_device *dev);
	struct delayed_work	port_periodic_work;
	struct device_node	*of_node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int cvm_oct_free_work(void *work_queue_entry);

<<<<<<< HEAD
extern int cvm_oct_rgmii_init(struct net_device *dev);
extern void cvm_oct_rgmii_uninit(struct net_device *dev);
extern int cvm_oct_rgmii_open(struct net_device *dev);
extern int cvm_oct_rgmii_stop(struct net_device *dev);

extern int cvm_oct_sgmii_init(struct net_device *dev);
extern void cvm_oct_sgmii_uninit(struct net_device *dev);
extern int cvm_oct_sgmii_open(struct net_device *dev);
extern int cvm_oct_sgmii_stop(struct net_device *dev);

extern int cvm_oct_spi_init(struct net_device *dev);
extern void cvm_oct_spi_uninit(struct net_device *dev);
extern int cvm_oct_xaui_init(struct net_device *dev);
extern void cvm_oct_xaui_uninit(struct net_device *dev);
extern int cvm_oct_xaui_open(struct net_device *dev);
extern int cvm_oct_xaui_stop(struct net_device *dev);

extern int cvm_oct_common_init(struct net_device *dev);
extern void cvm_oct_common_uninit(struct net_device *dev);

extern int always_use_pow;
extern int pow_send_group;
extern int pow_receive_group;
extern char pow_send_list[];
extern struct net_device *cvm_oct_device[];
extern struct workqueue_struct *cvm_oct_poll_queue;
extern atomic_t cvm_oct_poll_queue_stopping;
extern u64 cvm_oct_tx_poll_interval;

extern int max_rx_cpus;
=======
int cvm_oct_rgmii_open(struct net_device *dev);

int cvm_oct_sgmii_init(struct net_device *dev);
int cvm_oct_sgmii_open(struct net_device *dev);

int cvm_oct_spi_init(struct net_device *dev);
void cvm_oct_spi_uninit(struct net_device *dev);

int cvm_oct_common_init(struct net_device *dev);
void cvm_oct_common_uninit(struct net_device *dev);
void cvm_oct_adjust_link(struct net_device *dev);
int cvm_oct_common_stop(struct net_device *dev);
int cvm_oct_common_open(struct net_device *dev,
			void (*link_poll)(struct net_device *));
void cvm_oct_note_carrier(struct octeon_ethernet *priv,
			  union cvmx_helper_link_info li);
void cvm_oct_link_poll(struct net_device *dev);

extern int always_use_pow;
extern int pow_send_group;
extern int pow_receive_groups;
extern char pow_send_list[];
extern struct net_device *cvm_oct_device[];
extern atomic_t cvm_oct_poll_queue_stopping;
extern u64 cvm_oct_tx_poll_interval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int rx_napi_weight;

#endif
