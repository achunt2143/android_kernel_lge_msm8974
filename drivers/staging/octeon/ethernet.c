<<<<<<< HEAD
/**********************************************************************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2007 Cavium Networks
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
#include <linux/kernel.h>
#include <linux/init.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * This file is based on code from OCTEON SDK by Cavium Networks.
 *
 * Copyright (c) 2003-2007 Cavium Networks
 */

#include <linux/platform_device.h>
#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/phy.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
<<<<<<< HEAD

#include <net/dst.h>

#include <asm/octeon/octeon.h>

#include "ethernet-defines.h"
#include "octeon-ethernet.h"
=======
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>

#include <net/dst.h>

#include "octeon-ethernet.h"
#include "ethernet-defines.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "ethernet-mem.h"
#include "ethernet-rx.h"
#include "ethernet-tx.h"
#include "ethernet-mdio.h"
#include "ethernet-util.h"

<<<<<<< HEAD
#include <asm/octeon/cvmx-pip.h>
#include <asm/octeon/cvmx-pko.h>
#include <asm/octeon/cvmx-fau.h>
#include <asm/octeon/cvmx-ipd.h>
#include <asm/octeon/cvmx-helper.h>

#include <asm/octeon/cvmx-gmxx-defs.h>
#include <asm/octeon/cvmx-smix-defs.h>

#if defined(CONFIG_CAVIUM_OCTEON_NUM_PACKET_BUFFERS) \
	&& CONFIG_CAVIUM_OCTEON_NUM_PACKET_BUFFERS
int num_packet_buffers = CONFIG_CAVIUM_OCTEON_NUM_PACKET_BUFFERS;
#else
int num_packet_buffers = 1024;
#endif
module_param(num_packet_buffers, int, 0444);
MODULE_PARM_DESC(num_packet_buffers, "\n"
	"\tNumber of packet buffers to allocate and store in the\n"
	"\tFPA. By default, 1024 packet buffers are used unless\n"
	"\tCONFIG_CAVIUM_OCTEON_NUM_PACKET_BUFFERS is defined.");

int pow_receive_group = 15;
module_param(pow_receive_group, int, 0444);
MODULE_PARM_DESC(pow_receive_group, "\n"
	"\tPOW group to receive packets from. All ethernet hardware\n"
	"\twill be configured to send incomming packets to this POW\n"
	"\tgroup. Also any other software can submit packets to this\n"
	"\tgroup for the kernel to process.");

=======
#define OCTEON_MAX_MTU 65392

static int num_packet_buffers = 1024;
module_param(num_packet_buffers, int, 0444);
MODULE_PARM_DESC(num_packet_buffers, "\n"
	"\tNumber of packet buffers to allocate and store in the\n"
	"\tFPA. By default, 1024 packet buffers are used.\n");

static int pow_receive_group = 15;
module_param(pow_receive_group, int, 0444);
MODULE_PARM_DESC(pow_receive_group, "\n"
	"\tPOW group to receive packets from. All ethernet hardware\n"
	"\twill be configured to send incoming packets to this POW\n"
	"\tgroup. Also any other software can submit packets to this\n"
	"\tgroup for the kernel to process.");

static int receive_group_order;
module_param(receive_group_order, int, 0444);
MODULE_PARM_DESC(receive_group_order, "\n"
	"\tOrder (0..4) of receive groups to take into use. Ethernet hardware\n"
	"\twill be configured to send incoming packets to multiple POW\n"
	"\tgroups. pow_receive_group parameter is ignored when multiple\n"
	"\tgroups are taken into use and groups are allocated starting\n"
	"\tfrom 0. By default, a single group is used.\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int pow_send_group = -1;
module_param(pow_send_group, int, 0644);
MODULE_PARM_DESC(pow_send_group, "\n"
	"\tPOW group to send packets to other software on. This\n"
	"\tcontrols the creation of the virtual device pow0.\n"
	"\talways_use_pow also depends on this value.");

int always_use_pow;
module_param(always_use_pow, int, 0444);
MODULE_PARM_DESC(always_use_pow, "\n"
	"\tWhen set, always send to the pow group. This will cause\n"
	"\tpackets sent to real ethernet devices to be sent to the\n"
	"\tPOW group instead of the hardware. Unless some other\n"
	"\tapplication changes the config, packets will still be\n"
	"\treceived from the low level hardware. Use this option\n"
	"\tto allow a CVMX app to intercept all packets from the\n"
	"\tlinux kernel. You must specify pow_send_group along with\n"
	"\tthis option.");

char pow_send_list[128] = "";
module_param_string(pow_send_list, pow_send_list, sizeof(pow_send_list), 0444);
MODULE_PARM_DESC(pow_send_list, "\n"
	"\tComma separated list of ethernet devices that should use the\n"
	"\tPOW for transmit instead of the actual ethernet hardware. This\n"
	"\tis a per port version of always_use_pow. always_use_pow takes\n"
	"\tprecedence over this list. For example, setting this to\n"
	"\t\"eth2,spi3,spi7\" would cause these three devices to transmit\n"
	"\tusing the pow_send_group.");

<<<<<<< HEAD
int max_rx_cpus = -1;
module_param(max_rx_cpus, int, 0444);
MODULE_PARM_DESC(max_rx_cpus, "\n"
	"\t\tThe maximum number of CPUs to use for packet reception.\n"
	"\t\tUse -1 to use all available CPUs.");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rx_napi_weight = 32;
module_param(rx_napi_weight, int, 0444);
MODULE_PARM_DESC(rx_napi_weight, "The NAPI WEIGHT parameter.");

<<<<<<< HEAD
/*
 * The offset from mac_addr_base that should be used for the next port
 * that is configured.  By convention, if any mgmt ports exist on the
 * chip, they get the first mac addresses, The ports controlled by
 * this driver are numbered sequencially following any mgmt addresses
 * that may exist.
 */
static unsigned int cvm_oct_mac_addr_offset;

/**
 * cvm_oct_poll_queue - Workqueue for polling operations.
 */
struct workqueue_struct *cvm_oct_poll_queue;

/**
=======
/* Mask indicating which receive groups are in use. */
int pow_receive_groups;

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * cvm_oct_poll_queue_stopping - flag to indicate polling should stop.
 *
 * Set to one right before cvm_oct_poll_queue is destroyed.
 */
atomic_t cvm_oct_poll_queue_stopping = ATOMIC_INIT(0);

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Array of every ethernet device owned by this driver indexed by
 * the ipd input port number.
 */
struct net_device *cvm_oct_device[TOTAL_NUMBER_OF_PORTS];

u64 cvm_oct_tx_poll_interval;

static void cvm_oct_rx_refill_worker(struct work_struct *work);
static DECLARE_DELAYED_WORK(cvm_oct_rx_refill_work, cvm_oct_rx_refill_worker);

static void cvm_oct_rx_refill_worker(struct work_struct *work)
{
	/*
	 * FPA 0 may have been drained, try to refill it if we need
	 * more than num_packet_buffers / 2, otherwise normal receive
	 * processing will refill it.  If it were drained, no packets
	 * could be received so cvm_oct_napi_poll would never be
	 * invoked to do the refill.
	 */
	cvm_oct_rx_refill_pool(num_packet_buffers / 2);

	if (!atomic_read(&cvm_oct_poll_queue_stopping))
<<<<<<< HEAD
		queue_delayed_work(cvm_oct_poll_queue,
				   &cvm_oct_rx_refill_work, HZ);
=======
		schedule_delayed_work(&cvm_oct_rx_refill_work, HZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cvm_oct_periodic_worker(struct work_struct *work)
{
	struct octeon_ethernet *priv = container_of(work,
						    struct octeon_ethernet,
						    port_periodic_work.work);

	if (priv->poll)
		priv->poll(cvm_oct_device[priv->port]);

<<<<<<< HEAD
	cvm_oct_device[priv->port]->netdev_ops->ndo_get_stats(cvm_oct_device[priv->port]);

	if (!atomic_read(&cvm_oct_poll_queue_stopping))
		queue_delayed_work(cvm_oct_poll_queue, &priv->port_periodic_work, HZ);
 }

static __init void cvm_oct_configure_common_hw(void)
=======
	cvm_oct_device[priv->port]->netdev_ops->ndo_get_stats
						(cvm_oct_device[priv->port]);

	if (!atomic_read(&cvm_oct_poll_queue_stopping))
		schedule_delayed_work(&priv->port_periodic_work, HZ);
}

static void cvm_oct_configure_common_hw(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Setup the FPA */
	cvmx_fpa_enable();
	cvm_oct_mem_fill_fpa(CVMX_FPA_PACKET_POOL, CVMX_FPA_PACKET_POOL_SIZE,
			     num_packet_buffers);
	cvm_oct_mem_fill_fpa(CVMX_FPA_WQE_POOL, CVMX_FPA_WQE_POOL_SIZE,
			     num_packet_buffers);
	if (CVMX_FPA_OUTPUT_BUFFER_POOL != CVMX_FPA_PACKET_POOL)
		cvm_oct_mem_fill_fpa(CVMX_FPA_OUTPUT_BUFFER_POOL,
<<<<<<< HEAD
				     CVMX_FPA_OUTPUT_BUFFER_POOL_SIZE, 128);

	if (USE_RED)
		cvmx_helper_setup_red(num_packet_buffers / 4,
				      num_packet_buffers / 8);

=======
				     CVMX_FPA_OUTPUT_BUFFER_POOL_SIZE, 1024);

#ifdef __LITTLE_ENDIAN
	{
		union cvmx_ipd_ctl_status ipd_ctl_status;

		ipd_ctl_status.u64 = cvmx_read_csr(CVMX_IPD_CTL_STATUS);
		ipd_ctl_status.s.pkt_lend = 1;
		ipd_ctl_status.s.wqe_lend = 1;
		cvmx_write_csr(CVMX_IPD_CTL_STATUS, ipd_ctl_status.u64);
	}
#endif

	cvmx_helper_setup_red(num_packet_buffers / 4, num_packet_buffers / 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * cvm_oct_free_work- Free a work queue entry
 *
 * @work_queue_entry: Work queue entry to free
 *
 * Returns Zero on success, Negative on failure.
 */
int cvm_oct_free_work(void *work_queue_entry)
{
<<<<<<< HEAD
	cvmx_wqe_t *work = work_queue_entry;
=======
	struct cvmx_wqe *work = work_queue_entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int segments = work->word2.s.bufs;
	union cvmx_buf_ptr segment_ptr = work->packet_ptr;

	while (segments--) {
		union cvmx_buf_ptr next_ptr = *(union cvmx_buf_ptr *)
			cvmx_phys_to_ptr(segment_ptr.s.addr - 8);
		if (unlikely(!segment_ptr.s.i))
			cvmx_fpa_free(cvm_oct_get_buffer_ptr(segment_ptr),
				      segment_ptr.s.pool,
<<<<<<< HEAD
				      DONT_WRITEBACK(CVMX_FPA_PACKET_POOL_SIZE /
						     128));
		segment_ptr = next_ptr;
	}
	cvmx_fpa_free(work, CVMX_FPA_WQE_POOL, DONT_WRITEBACK(1));
=======
				      CVMX_FPA_PACKET_POOL_SIZE / 128);
		segment_ptr = next_ptr;
	}
	cvmx_fpa_free(work, CVMX_FPA_WQE_POOL, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL(cvm_oct_free_work);

/**
 * cvm_oct_common_get_stats - get the low level ethernet statistics
 * @dev:    Device to get the statistics from
 *
 * Returns Pointer to the statistics
 */
static struct net_device_stats *cvm_oct_common_get_stats(struct net_device *dev)
{
	cvmx_pip_port_status_t rx_status;
	cvmx_pko_port_status_t tx_status;
	struct octeon_ethernet *priv = netdev_priv(dev);

	if (priv->port < CVMX_PIP_NUM_INPUT_PORTS) {
		if (octeon_is_simulation()) {
			/* The simulator doesn't support statistics */
			memset(&rx_status, 0, sizeof(rx_status));
			memset(&tx_status, 0, sizeof(tx_status));
		} else {
			cvmx_pip_get_port_status(priv->port, 1, &rx_status);
			cvmx_pko_get_port_status(priv->port, 1, &tx_status);
		}

<<<<<<< HEAD
		priv->stats.rx_packets += rx_status.inb_packets;
		priv->stats.tx_packets += tx_status.packets;
		priv->stats.rx_bytes += rx_status.inb_octets;
		priv->stats.tx_bytes += tx_status.octets;
		priv->stats.multicast += rx_status.multicast_packets;
		priv->stats.rx_crc_errors += rx_status.inb_errors;
		priv->stats.rx_frame_errors += rx_status.fcs_align_err_packets;

		/*
		 * The drop counter must be incremented atomically
		 * since the RX tasklet also increments it.
		 */
#ifdef CONFIG_64BIT
		atomic64_add(rx_status.dropped_packets,
			     (atomic64_t *)&priv->stats.rx_dropped);
#else
		atomic_add(rx_status.dropped_packets,
			     (atomic_t *)&priv->stats.rx_dropped);
#endif
	}

	return &priv->stats;
=======
		dev->stats.rx_packets += rx_status.inb_packets;
		dev->stats.tx_packets += tx_status.packets;
		dev->stats.rx_bytes += rx_status.inb_octets;
		dev->stats.tx_bytes += tx_status.octets;
		dev->stats.multicast += rx_status.multicast_packets;
		dev->stats.rx_crc_errors += rx_status.inb_errors;
		dev->stats.rx_frame_errors += rx_status.fcs_align_err_packets;
		dev->stats.rx_dropped += rx_status.dropped_packets;
	}

	return &dev->stats;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * cvm_oct_common_change_mtu - change the link MTU
 * @dev:     Device to change
 * @new_mtu: The new MTU
 *
 * Returns Zero on success
 */
static int cvm_oct_common_change_mtu(struct net_device *dev, int new_mtu)
{
	struct octeon_ethernet *priv = netdev_priv(dev);
	int interface = INTERFACE(priv->port);
<<<<<<< HEAD
	int index = INDEX(priv->port);
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
	int vlan_bytes = 4;
#else
	int vlan_bytes = 0;
#endif

	/*
	 * Limit the MTU to make sure the ethernet packets are between
	 * 64 bytes and 65535 bytes.
	 */
	if ((new_mtu + 14 + 4 + vlan_bytes < 64)
	    || (new_mtu + 14 + 4 + vlan_bytes > 65392)) {
		pr_err("MTU must be between %d and %d.\n",
		       64 - 14 - 4 - vlan_bytes, 65392 - 14 - 4 - vlan_bytes);
		return -EINVAL;
	}
	dev->mtu = new_mtu;

	if ((interface < 2)
	    && (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		/* Add ethernet header and FCS, and VLAN if configured. */
		int max_packet = new_mtu + 14 + 4 + vlan_bytes;

		if (OCTEON_IS_MODEL(OCTEON_CN3XXX)
		    || OCTEON_IS_MODEL(OCTEON_CN58XX)) {
=======
#if IS_ENABLED(CONFIG_VLAN_8021Q)
	int vlan_bytes = VLAN_HLEN;
#else
	int vlan_bytes = 0;
#endif
	int mtu_overhead = ETH_HLEN + ETH_FCS_LEN + vlan_bytes;

	dev->mtu = new_mtu;

	if ((interface < 2) &&
	    (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		int index = INDEX(priv->port);
		/* Add ethernet header and FCS, and VLAN if configured. */
		int max_packet = new_mtu + mtu_overhead;

		if (OCTEON_IS_MODEL(OCTEON_CN3XXX) ||
		    OCTEON_IS_MODEL(OCTEON_CN58XX)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Signal errors on packets larger than the MTU */
			cvmx_write_csr(CVMX_GMXX_RXX_FRM_MAX(index, interface),
				       max_packet);
		} else {
			/*
			 * Set the hardware to truncate packets larger
			 * than the MTU and smaller the 64 bytes.
			 */
			union cvmx_pip_frm_len_chkx frm_len_chk;
<<<<<<< HEAD
			frm_len_chk.u64 = 0;
			frm_len_chk.s.minlen = 64;
=======

			frm_len_chk.u64 = 0;
			frm_len_chk.s.minlen = VLAN_ETH_ZLEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			frm_len_chk.s.maxlen = max_packet;
			cvmx_write_csr(CVMX_PIP_FRM_LEN_CHKX(interface),
				       frm_len_chk.u64);
		}
		/*
		 * Set the hardware to truncate packets larger than
		 * the MTU. The jabber register must be set to a
		 * multiple of 8 bytes, so round up.
		 */
		cvmx_write_csr(CVMX_GMXX_RXX_JABBER(index, interface),
			       (max_packet + 7) & ~7u);
	}
	return 0;
}

/**
 * cvm_oct_common_set_multicast_list - set the multicast list
 * @dev:    Device to work on
 */
static void cvm_oct_common_set_multicast_list(struct net_device *dev)
{
	union cvmx_gmxx_prtx_cfg gmx_cfg;
	struct octeon_ethernet *priv = netdev_priv(dev);
	int interface = INTERFACE(priv->port);
<<<<<<< HEAD
	int index = INDEX(priv->port);

	if ((interface < 2)
	    && (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		union cvmx_gmxx_rxx_adr_ctl control;
=======

	if ((interface < 2) &&
	    (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		union cvmx_gmxx_rxx_adr_ctl control;
		int index = INDEX(priv->port);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		control.u64 = 0;
		control.s.bcst = 1;	/* Allow broadcast MAC addresses */

		if (!netdev_mc_empty(dev) || (dev->flags & IFF_ALLMULTI) ||
		    (dev->flags & IFF_PROMISC))
			/* Force accept multicast packets */
			control.s.mcst = 2;
		else
<<<<<<< HEAD
			/* Force reject multicat packets */
=======
			/* Force reject multicast packets */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			control.s.mcst = 1;

		if (dev->flags & IFF_PROMISC)
			/*
			 * Reject matches if promisc. Since CAM is
			 * shut off, should accept everything.
			 */
			control.s.cam_mode = 0;
		else
			/* Filter packets based on the CAM */
			control.s.cam_mode = 1;

		gmx_cfg.u64 =
		    cvmx_read_csr(CVMX_GMXX_PRTX_CFG(index, interface));
		cvmx_write_csr(CVMX_GMXX_PRTX_CFG(index, interface),
			       gmx_cfg.u64 & ~1ull);

		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CTL(index, interface),
			       control.u64);
		if (dev->flags & IFF_PROMISC)
			cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM_EN
				       (index, interface), 0);
		else
			cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM_EN
				       (index, interface), 1);

		cvmx_write_csr(CVMX_GMXX_PRTX_CFG(index, interface),
			       gmx_cfg.u64);
	}
}

<<<<<<< HEAD
/**
 * cvm_oct_common_set_mac_address - set the hardware MAC address for a device
 * @dev:    The device in question.
 * @addr:   Address structure to change it too.

 * Returns Zero on success
 */
static int cvm_oct_common_set_mac_address(struct net_device *dev, void *addr)
=======
static int cvm_oct_set_mac_filter(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct octeon_ethernet *priv = netdev_priv(dev);
	union cvmx_gmxx_prtx_cfg gmx_cfg;
	int interface = INTERFACE(priv->port);
<<<<<<< HEAD
	int index = INDEX(priv->port);

	memcpy(dev->dev_addr, addr + 2, 6);

	if ((interface < 2)
	    && (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		int i;
		uint8_t *ptr = addr;
		uint64_t mac = 0;
		for (i = 0; i < 6; i++)
			mac = (mac << 8) | (uint64_t) (ptr[i + 2]);
=======

	if ((interface < 2) &&
	    (cvmx_helper_interface_get_mode(interface) !=
		CVMX_HELPER_INTERFACE_MODE_SPI)) {
		int i;
		const u8 *ptr = dev->dev_addr;
		u64 mac = 0;
		int index = INDEX(priv->port);

		for (i = 0; i < 6; i++)
			mac = (mac << 8) | (u64)ptr[i];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		gmx_cfg.u64 =
		    cvmx_read_csr(CVMX_GMXX_PRTX_CFG(index, interface));
		cvmx_write_csr(CVMX_GMXX_PRTX_CFG(index, interface),
			       gmx_cfg.u64 & ~1ull);

		cvmx_write_csr(CVMX_GMXX_SMACX(index, interface), mac);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM0(index, interface),
<<<<<<< HEAD
			       ptr[2]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM1(index, interface),
			       ptr[3]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM2(index, interface),
			       ptr[4]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM3(index, interface),
			       ptr[5]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM4(index, interface),
			       ptr[6]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM5(index, interface),
			       ptr[7]);
=======
			       ptr[0]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM1(index, interface),
			       ptr[1]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM2(index, interface),
			       ptr[2]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM3(index, interface),
			       ptr[3]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM4(index, interface),
			       ptr[4]);
		cvmx_write_csr(CVMX_GMXX_RXX_ADR_CAM5(index, interface),
			       ptr[5]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cvm_oct_common_set_multicast_list(dev);
		cvmx_write_csr(CVMX_GMXX_PRTX_CFG(index, interface),
			       gmx_cfg.u64);
	}
	return 0;
}

/**
<<<<<<< HEAD
=======
 * cvm_oct_common_set_mac_address - set the hardware MAC address for a device
 * @dev:    The device in question.
 * @addr:   Socket address.
 *
 * Returns Zero on success
 */
static int cvm_oct_common_set_mac_address(struct net_device *dev, void *addr)
{
	int r = eth_mac_addr(dev, addr);

	if (r)
		return r;
	return cvm_oct_set_mac_filter(dev);
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * cvm_oct_common_init - per network device initialization
 * @dev:    Device to initialize
 *
 * Returns Zero on success
 */
int cvm_oct_common_init(struct net_device *dev)
{
	struct octeon_ethernet *priv = netdev_priv(dev);
<<<<<<< HEAD
	struct sockaddr sa;
	u64 mac = ((u64)(octeon_bootinfo->mac_addr_base[0] & 0xff) << 40) |
		((u64)(octeon_bootinfo->mac_addr_base[1] & 0xff) << 32) |
		((u64)(octeon_bootinfo->mac_addr_base[2] & 0xff) << 24) |
		((u64)(octeon_bootinfo->mac_addr_base[3] & 0xff) << 16) |
		((u64)(octeon_bootinfo->mac_addr_base[4] & 0xff) << 8) |
		(u64)(octeon_bootinfo->mac_addr_base[5] & 0xff);

	mac += cvm_oct_mac_addr_offset;
	sa.sa_data[0] = (mac >> 40) & 0xff;
	sa.sa_data[1] = (mac >> 32) & 0xff;
	sa.sa_data[2] = (mac >> 24) & 0xff;
	sa.sa_data[3] = (mac >> 16) & 0xff;
	sa.sa_data[4] = (mac >> 8) & 0xff;
	sa.sa_data[5] = mac & 0xff;

	if (cvm_oct_mac_addr_offset >= octeon_bootinfo->mac_addr_count)
		printk(KERN_DEBUG "%s: Using MAC outside of the assigned range:"
			" %pM\n", dev->name, sa.sa_data);
	cvm_oct_mac_addr_offset++;
=======
	int ret;

	ret = of_get_ethdev_address(priv->of_node, dev);
	if (ret)
		eth_hw_addr_random(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Force the interface to use the POW send if always_use_pow
	 * was specified or it is in the pow send list.
	 */
<<<<<<< HEAD
	if ((pow_send_group != -1)
	    && (always_use_pow || strstr(pow_send_list, dev->name)))
		priv->queue = -1;

	if (priv->queue != -1) {
		dev->features |= NETIF_F_SG;
		if (USE_HW_TCPUDP_CHECKSUM)
			dev->features |= NETIF_F_IP_CSUM;
	}

	/* We do our own locking, Linux doesn't need to */
	dev->features |= NETIF_F_LLTX;
	SET_ETHTOOL_OPS(dev, &cvm_oct_ethtool_ops);

	cvm_oct_phy_setup_device(dev);
	dev->netdev_ops->ndo_set_mac_address(dev, &sa);
	dev->netdev_ops->ndo_change_mtu(dev, dev->mtu);
=======
	if ((pow_send_group != -1) &&
	    (always_use_pow || strstr(pow_send_list, dev->name)))
		priv->queue = -1;

	if (priv->queue != -1)
		dev->features |= NETIF_F_SG | NETIF_F_IP_CSUM;

	/* We do our own locking, Linux doesn't need to */
	dev->features |= NETIF_F_LLTX;
	dev->ethtool_ops = &cvm_oct_ethtool_ops;

	cvm_oct_set_mac_filter(dev);
	dev_set_mtu(dev, dev->mtu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Zero out stats for port so we won't mistakenly show
	 * counters from the bootloader.
	 */
	memset(dev->netdev_ops->ndo_get_stats(dev), 0,
	       sizeof(struct net_device_stats));

<<<<<<< HEAD
=======
	if (dev->netdev_ops->ndo_stop)
		dev->netdev_ops->ndo_stop(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

void cvm_oct_common_uninit(struct net_device *dev)
{
<<<<<<< HEAD
	struct octeon_ethernet *priv = netdev_priv(dev);

	if (priv->phydev)
		phy_disconnect(priv->phydev);
=======
	if (dev->phydev)
		phy_disconnect(dev->phydev);
}

int cvm_oct_common_open(struct net_device *dev,
			void (*link_poll)(struct net_device *))
{
	union cvmx_gmxx_prtx_cfg gmx_cfg;
	struct octeon_ethernet *priv = netdev_priv(dev);
	int interface = INTERFACE(priv->port);
	int index = INDEX(priv->port);
	union cvmx_helper_link_info link_info;
	int rv;

	rv = cvm_oct_phy_setup_device(dev);
	if (rv)
		return rv;

	gmx_cfg.u64 = cvmx_read_csr(CVMX_GMXX_PRTX_CFG(index, interface));
	gmx_cfg.s.en = 1;
	if (octeon_has_feature(OCTEON_FEATURE_PKND))
		gmx_cfg.s.pknd = priv->port;
	cvmx_write_csr(CVMX_GMXX_PRTX_CFG(index, interface), gmx_cfg.u64);

	if (octeon_is_simulation())
		return 0;

	if (dev->phydev) {
		int r = phy_read_status(dev->phydev);

		if (r == 0 && dev->phydev->link == 0)
			netif_carrier_off(dev);
		cvm_oct_adjust_link(dev);
	} else {
		link_info = cvmx_helper_link_get(priv->port);
		if (!link_info.s.link_up)
			netif_carrier_off(dev);
		priv->poll = link_poll;
		link_poll(dev);
	}

	return 0;
}

void cvm_oct_link_poll(struct net_device *dev)
{
	struct octeon_ethernet *priv = netdev_priv(dev);
	union cvmx_helper_link_info link_info;

	link_info = cvmx_helper_link_get(priv->port);
	if (link_info.u64 == priv->link_info)
		return;

	if (cvmx_helper_link_set(priv->port, link_info))
		link_info.u64 = priv->link_info;
	else
		priv->link_info = link_info.u64;

	if (link_info.s.link_up) {
		if (!netif_carrier_ok(dev))
			netif_carrier_on(dev);
	} else if (netif_carrier_ok(dev)) {
		netif_carrier_off(dev);
	}
	cvm_oct_note_carrier(priv, link_info);
}

static int cvm_oct_xaui_open(struct net_device *dev)
{
	return cvm_oct_common_open(dev, cvm_oct_link_poll);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct net_device_ops cvm_oct_npi_netdev_ops = {
	.ndo_init		= cvm_oct_common_init,
	.ndo_uninit		= cvm_oct_common_uninit,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
<<<<<<< HEAD
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};
<<<<<<< HEAD
static const struct net_device_ops cvm_oct_xaui_netdev_ops = {
	.ndo_init		= cvm_oct_xaui_init,
	.ndo_uninit		= cvm_oct_xaui_uninit,
	.ndo_open		= cvm_oct_xaui_open,
	.ndo_stop		= cvm_oct_xaui_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======

static const struct net_device_ops cvm_oct_xaui_netdev_ops = {
	.ndo_init		= cvm_oct_common_init,
	.ndo_uninit		= cvm_oct_common_uninit,
	.ndo_open		= cvm_oct_xaui_open,
	.ndo_stop		= cvm_oct_common_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};
<<<<<<< HEAD
static const struct net_device_ops cvm_oct_sgmii_netdev_ops = {
	.ndo_init		= cvm_oct_sgmii_init,
	.ndo_uninit		= cvm_oct_sgmii_uninit,
	.ndo_open		= cvm_oct_sgmii_open,
	.ndo_stop		= cvm_oct_sgmii_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======

static const struct net_device_ops cvm_oct_sgmii_netdev_ops = {
	.ndo_init		= cvm_oct_sgmii_init,
	.ndo_uninit		= cvm_oct_common_uninit,
	.ndo_open		= cvm_oct_sgmii_open,
	.ndo_stop		= cvm_oct_common_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct net_device_ops cvm_oct_spi_netdev_ops = {
	.ndo_init		= cvm_oct_spi_init,
	.ndo_uninit		= cvm_oct_spi_uninit,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
<<<<<<< HEAD
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};
<<<<<<< HEAD
static const struct net_device_ops cvm_oct_rgmii_netdev_ops = {
	.ndo_init		= cvm_oct_rgmii_init,
	.ndo_uninit		= cvm_oct_rgmii_uninit,
	.ndo_open		= cvm_oct_rgmii_open,
	.ndo_stop		= cvm_oct_rgmii_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======

static const struct net_device_ops cvm_oct_rgmii_netdev_ops = {
	.ndo_init		= cvm_oct_common_init,
	.ndo_uninit		= cvm_oct_common_uninit,
	.ndo_open		= cvm_oct_rgmii_open,
	.ndo_stop		= cvm_oct_common_stop,
	.ndo_start_xmit		= cvm_oct_xmit,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct net_device_ops cvm_oct_pow_netdev_ops = {
	.ndo_init		= cvm_oct_common_init,
	.ndo_start_xmit		= cvm_oct_xmit_pow,
	.ndo_set_rx_mode	= cvm_oct_common_set_multicast_list,
	.ndo_set_mac_address	= cvm_oct_common_set_mac_address,
<<<<<<< HEAD
	.ndo_do_ioctl		= cvm_oct_ioctl,
=======
	.ndo_eth_ioctl		= cvm_oct_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_change_mtu		= cvm_oct_common_change_mtu,
	.ndo_get_stats		= cvm_oct_common_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cvm_oct_poll_controller,
#endif
};

<<<<<<< HEAD
extern void octeon_mdiobus_force_mod_depencency(void);

static int __init cvm_oct_init_module(void)
=======
static struct device_node *cvm_oct_of_get_child
				(const struct device_node *parent, int reg_val)
{
	struct device_node *node;
	const __be32 *addr;
	int size;

	for_each_child_of_node(parent, node) {
		addr = of_get_property(node, "reg", &size);
		if (addr && (be32_to_cpu(*addr) == reg_val))
			break;
	}
	return node;
}

static struct device_node *cvm_oct_node_for_port(struct device_node *pip,
						 int interface, int port)
{
	struct device_node *ni, *np;

	ni = cvm_oct_of_get_child(pip, interface);
	if (!ni)
		return NULL;

	np = cvm_oct_of_get_child(ni, port);
	of_node_put(ni);

	return np;
}

static void cvm_set_rgmii_delay(struct octeon_ethernet *priv, int iface,
				int port)
{
	struct device_node *np = priv->of_node;
	u32 delay_value;
	bool rx_delay;
	bool tx_delay;

	/* By default, both RX/TX delay is enabled in
	 * __cvmx_helper_rgmii_enable().
	 */
	rx_delay = true;
	tx_delay = true;

	if (!of_property_read_u32(np, "rx-delay", &delay_value)) {
		cvmx_write_csr(CVMX_ASXX_RX_CLK_SETX(port, iface), delay_value);
		rx_delay = delay_value > 0;
	}
	if (!of_property_read_u32(np, "tx-delay", &delay_value)) {
		cvmx_write_csr(CVMX_ASXX_TX_CLK_SETX(port, iface), delay_value);
		tx_delay = delay_value > 0;
	}

	if (!rx_delay && !tx_delay)
		priv->phy_mode = PHY_INTERFACE_MODE_RGMII_ID;
	else if (!rx_delay)
		priv->phy_mode = PHY_INTERFACE_MODE_RGMII_RXID;
	else if (!tx_delay)
		priv->phy_mode = PHY_INTERFACE_MODE_RGMII_TXID;
	else
		priv->phy_mode = PHY_INTERFACE_MODE_RGMII;
}

static int cvm_oct_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int num_interfaces;
	int interface;
	int fau = FAU_NUM_PACKET_BUFFERS_TO_FREE;
	int qos;
<<<<<<< HEAD

	octeon_mdiobus_force_mod_depencency();
	pr_notice("cavium-ethernet %s\n", OCTEON_ETHERNET_VERSION);

	if (OCTEON_IS_MODEL(OCTEON_CN52XX))
		cvm_oct_mac_addr_offset = 2; /* First two are the mgmt ports. */
	else if (OCTEON_IS_MODEL(OCTEON_CN56XX))
		cvm_oct_mac_addr_offset = 1; /* First one is the mgmt port. */
	else
		cvm_oct_mac_addr_offset = 0;

	cvm_oct_poll_queue = create_singlethread_workqueue("octeon-ethernet");
	if (cvm_oct_poll_queue == NULL) {
		pr_err("octeon-ethernet: Cannot create workqueue");
		return -ENOMEM;
=======
	struct device_node *pip;
	int mtu_overhead = ETH_HLEN + ETH_FCS_LEN;

#if IS_ENABLED(CONFIG_VLAN_8021Q)
	mtu_overhead += VLAN_HLEN;
#endif

	pip = pdev->dev.of_node;
	if (!pip) {
		pr_err("Error: No 'pip' in /aliases\n");
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	cvm_oct_configure_common_hw();

	cvmx_helper_initialize_packet_io_global();

<<<<<<< HEAD
=======
	if (receive_group_order) {
		if (receive_group_order > 4)
			receive_group_order = 4;
		pow_receive_groups = (1 << (1 << receive_group_order)) - 1;
	} else {
		pow_receive_groups = BIT(pow_receive_group);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Change the input group for all ports before input is enabled */
	num_interfaces = cvmx_helper_get_number_of_interfaces();
	for (interface = 0; interface < num_interfaces; interface++) {
		int num_ports = cvmx_helper_ports_on_interface(interface);
		int port;

		for (port = cvmx_helper_get_ipd_port(interface, 0);
		     port < cvmx_helper_get_ipd_port(interface, num_ports);
		     port++) {
			union cvmx_pip_prt_tagx pip_prt_tagx;
<<<<<<< HEAD
			pip_prt_tagx.u64 =
			    cvmx_read_csr(CVMX_PIP_PRT_TAGX(port));
			pip_prt_tagx.s.grp = pow_receive_group;
=======

			pip_prt_tagx.u64 =
			    cvmx_read_csr(CVMX_PIP_PRT_TAGX(port));

			if (receive_group_order) {
				int tag_mask;

				/* We support only 16 groups at the moment, so
				 * always disable the two additional "hidden"
				 * tag_mask bits on CN68XX.
				 */
				if (OCTEON_IS_MODEL(OCTEON_CN68XX))
					pip_prt_tagx.u64 |= 0x3ull << 44;

				tag_mask = ~((1 << receive_group_order) - 1);
				pip_prt_tagx.s.grptagbase	= 0;
				pip_prt_tagx.s.grptagmask	= tag_mask;
				pip_prt_tagx.s.grptag		= 1;
				pip_prt_tagx.s.tag_mode		= 0;
				pip_prt_tagx.s.inc_prt_flag	= 1;
				pip_prt_tagx.s.ip6_dprt_flag	= 1;
				pip_prt_tagx.s.ip4_dprt_flag	= 1;
				pip_prt_tagx.s.ip6_sprt_flag	= 1;
				pip_prt_tagx.s.ip4_sprt_flag	= 1;
				pip_prt_tagx.s.ip6_dst_flag	= 1;
				pip_prt_tagx.s.ip4_dst_flag	= 1;
				pip_prt_tagx.s.ip6_src_flag	= 1;
				pip_prt_tagx.s.ip4_src_flag	= 1;
				pip_prt_tagx.s.grp		= 0;
			} else {
				pip_prt_tagx.s.grptag	= 0;
				pip_prt_tagx.s.grp	= pow_receive_group;
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cvmx_write_csr(CVMX_PIP_PRT_TAGX(port),
				       pip_prt_tagx.u64);
		}
	}

	cvmx_helper_ipd_and_packet_input_enable();

	memset(cvm_oct_device, 0, sizeof(cvm_oct_device));

	/*
	 * Initialize the FAU used for counting packet buffers that
	 * need to be freed.
	 */
	cvmx_fau_atomic_write32(FAU_NUM_PACKET_BUFFERS_TO_FREE, 0);

	/* Initialize the FAU used for counting tx SKBs that need to be freed */
	cvmx_fau_atomic_write32(FAU_TOTAL_TX_TO_CLEAN, 0);

	if ((pow_send_group != -1)) {
		struct net_device *dev;
<<<<<<< HEAD
		pr_info("\tConfiguring device for POW only access\n");
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev = alloc_etherdev(sizeof(struct octeon_ethernet));
		if (dev) {
			/* Initialize the device private structure. */
			struct octeon_ethernet *priv = netdev_priv(dev);

<<<<<<< HEAD
=======
			SET_NETDEV_DEV(dev, &pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dev->netdev_ops = &cvm_oct_pow_netdev_ops;
			priv->imode = CVMX_HELPER_INTERFACE_MODE_DISABLED;
			priv->port = CVMX_PIP_NUM_INPUT_PORTS;
			priv->queue = -1;
<<<<<<< HEAD
			strcpy(dev->name, "pow%d");
			for (qos = 0; qos < 16; qos++)
				skb_queue_head_init(&priv->tx_free_list[qos]);
=======
			strscpy(dev->name, "pow%d", sizeof(dev->name));
			for (qos = 0; qos < 16; qos++)
				skb_queue_head_init(&priv->tx_free_list[qos]);
			dev->min_mtu = VLAN_ETH_ZLEN - mtu_overhead;
			dev->max_mtu = OCTEON_MAX_MTU - mtu_overhead;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (register_netdev(dev) < 0) {
				pr_err("Failed to register ethernet device for POW\n");
				free_netdev(dev);
			} else {
				cvm_oct_device[CVMX_PIP_NUM_INPUT_PORTS] = dev;
				pr_info("%s: POW send group %d, receive group %d\n",
					dev->name, pow_send_group,
					pow_receive_group);
			}
		} else {
			pr_err("Failed to allocate ethernet device for POW\n");
		}
	}

	num_interfaces = cvmx_helper_get_number_of_interfaces();
	for (interface = 0; interface < num_interfaces; interface++) {
		cvmx_helper_interface_mode_t imode =
		    cvmx_helper_interface_get_mode(interface);
		int num_ports = cvmx_helper_ports_on_interface(interface);
		int port;
<<<<<<< HEAD

		for (port = cvmx_helper_get_ipd_port(interface, 0);
		     port < cvmx_helper_get_ipd_port(interface, num_ports);
		     port++) {
=======
		int port_index;

		for (port_index = 0,
		     port = cvmx_helper_get_ipd_port(interface, 0);
		     port < cvmx_helper_get_ipd_port(interface, num_ports);
		     port_index++, port++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct octeon_ethernet *priv;
			struct net_device *dev =
			    alloc_etherdev(sizeof(struct octeon_ethernet));
			if (!dev) {
<<<<<<< HEAD
				pr_err("Failed to allocate ethernet device for port %d\n", port);
=======
				pr_err("Failed to allocate ethernet device for port %d\n",
				       port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				continue;
			}

			/* Initialize the device private structure. */
<<<<<<< HEAD
			priv = netdev_priv(dev);
=======
			SET_NETDEV_DEV(dev, &pdev->dev);
			priv = netdev_priv(dev);
			priv->netdev = dev;
			priv->of_node = cvm_oct_node_for_port(pip, interface,
							      port_index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			INIT_DELAYED_WORK(&priv->port_periodic_work,
					  cvm_oct_periodic_worker);
			priv->imode = imode;
			priv->port = port;
			priv->queue = cvmx_pko_get_base_queue(priv->port);
			priv->fau = fau - cvmx_pko_get_num_queues(port) * 4;
<<<<<<< HEAD
=======
			priv->phy_mode = PHY_INTERFACE_MODE_NA;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			for (qos = 0; qos < 16; qos++)
				skb_queue_head_init(&priv->tx_free_list[qos]);
			for (qos = 0; qos < cvmx_pko_get_num_queues(port);
			     qos++)
				cvmx_fau_atomic_write32(priv->fau + qos * 4, 0);
<<<<<<< HEAD

			switch (priv->imode) {

=======
			dev->min_mtu = VLAN_ETH_ZLEN - mtu_overhead;
			dev->max_mtu = OCTEON_MAX_MTU - mtu_overhead;

			switch (priv->imode) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* These types don't support ports to IPD/PKO */
			case CVMX_HELPER_INTERFACE_MODE_DISABLED:
			case CVMX_HELPER_INTERFACE_MODE_PCIE:
			case CVMX_HELPER_INTERFACE_MODE_PICMG:
				break;

			case CVMX_HELPER_INTERFACE_MODE_NPI:
				dev->netdev_ops = &cvm_oct_npi_netdev_ops;
<<<<<<< HEAD
				strcpy(dev->name, "npi%d");
=======
				strscpy(dev->name, "npi%d", sizeof(dev->name));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;

			case CVMX_HELPER_INTERFACE_MODE_XAUI:
				dev->netdev_ops = &cvm_oct_xaui_netdev_ops;
<<<<<<< HEAD
				strcpy(dev->name, "xaui%d");
=======
				strscpy(dev->name, "xaui%d", sizeof(dev->name));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;

			case CVMX_HELPER_INTERFACE_MODE_LOOP:
				dev->netdev_ops = &cvm_oct_npi_netdev_ops;
<<<<<<< HEAD
				strcpy(dev->name, "loop%d");
				break;

			case CVMX_HELPER_INTERFACE_MODE_SGMII:
				dev->netdev_ops = &cvm_oct_sgmii_netdev_ops;
				strcpy(dev->name, "eth%d");
=======
				strscpy(dev->name, "loop%d", sizeof(dev->name));
				break;

			case CVMX_HELPER_INTERFACE_MODE_SGMII:
				priv->phy_mode = PHY_INTERFACE_MODE_SGMII;
				dev->netdev_ops = &cvm_oct_sgmii_netdev_ops;
				strscpy(dev->name, "eth%d", sizeof(dev->name));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;

			case CVMX_HELPER_INTERFACE_MODE_SPI:
				dev->netdev_ops = &cvm_oct_spi_netdev_ops;
<<<<<<< HEAD
				strcpy(dev->name, "spi%d");
				break;

			case CVMX_HELPER_INTERFACE_MODE_RGMII:
			case CVMX_HELPER_INTERFACE_MODE_GMII:
				dev->netdev_ops = &cvm_oct_rgmii_netdev_ops;
				strcpy(dev->name, "eth%d");
				break;
			}

			if (!dev->netdev_ops) {
				free_netdev(dev);
			} else if (register_netdev(dev) < 0) {
				pr_err("Failed to register ethernet device "
					 "for interface %d, port %d\n",
					 interface, priv->port);
=======
				strscpy(dev->name, "spi%d", sizeof(dev->name));
				break;

			case CVMX_HELPER_INTERFACE_MODE_GMII:
				priv->phy_mode = PHY_INTERFACE_MODE_GMII;
				dev->netdev_ops = &cvm_oct_rgmii_netdev_ops;
				strscpy(dev->name, "eth%d", sizeof(dev->name));
				break;

			case CVMX_HELPER_INTERFACE_MODE_RGMII:
				dev->netdev_ops = &cvm_oct_rgmii_netdev_ops;
				strscpy(dev->name, "eth%d", sizeof(dev->name));
				cvm_set_rgmii_delay(priv, interface,
						    port_index);
				break;
			}

			if (priv->of_node && of_phy_is_fixed_link(priv->of_node)) {
				if (of_phy_register_fixed_link(priv->of_node)) {
					netdev_err(dev, "Failed to register fixed link for interface %d, port %d\n",
						   interface, priv->port);
					dev->netdev_ops = NULL;
				}
			}

			if (!dev->netdev_ops) {
				free_netdev(dev);
			} else if (register_netdev(dev) < 0) {
				pr_err("Failed to register ethernet device for interface %d, port %d\n",
				       interface, priv->port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				free_netdev(dev);
			} else {
				cvm_oct_device[priv->port] = dev;
				fau -=
				    cvmx_pko_get_num_queues(priv->port) *
<<<<<<< HEAD
				    sizeof(uint32_t);
				queue_delayed_work(cvm_oct_poll_queue,
						   &priv->port_periodic_work, HZ);
=======
				    sizeof(u32);
				schedule_delayed_work(&priv->port_periodic_work,
						      HZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

	cvm_oct_tx_initialize();
	cvm_oct_rx_initialize();

	/*
<<<<<<< HEAD
	 * 150 uS: about 10 1500-byte packtes at 1GE.
	 */
	cvm_oct_tx_poll_interval = 150 * (octeon_get_clock_rate() / 1000000);

	queue_delayed_work(cvm_oct_poll_queue, &cvm_oct_rx_refill_work, HZ);
=======
	 * 150 uS: about 10 1500-byte packets at 1GE.
	 */
	cvm_oct_tx_poll_interval = 150 * (octeon_get_clock_rate() / 1000000);

	schedule_delayed_work(&cvm_oct_rx_refill_work, HZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void __exit cvm_oct_cleanup_module(void)
{
	int port;

	/* Disable POW interrupt */
	cvmx_write_csr(CVMX_POW_WQ_INT_THRX(pow_receive_group), 0);

	cvmx_ipd_disable();

	/* Free the interrupt handler */
	free_irq(OCTEON_IRQ_WORKQ0 + pow_receive_group, cvm_oct_device);

=======
static void cvm_oct_remove(struct platform_device *pdev)
{
	int port;

	cvmx_ipd_disable();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_inc_return(&cvm_oct_poll_queue_stopping);
	cancel_delayed_work_sync(&cvm_oct_rx_refill_work);

	cvm_oct_rx_shutdown();
	cvm_oct_tx_shutdown();

	cvmx_pko_disable();

	/* Free the ethernet devices */
	for (port = 0; port < TOTAL_NUMBER_OF_PORTS; port++) {
		if (cvm_oct_device[port]) {
			struct net_device *dev = cvm_oct_device[port];
			struct octeon_ethernet *priv = netdev_priv(dev);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cancel_delayed_work_sync(&priv->port_periodic_work);

			cvm_oct_tx_shutdown_dev(dev);
			unregister_netdev(dev);
			free_netdev(dev);
			cvm_oct_device[port] = NULL;
		}
	}

<<<<<<< HEAD
	destroy_workqueue(cvm_oct_poll_queue);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cvmx_pko_shutdown();

	cvmx_ipd_free_ptr();

	/* Free the HW pools */
	cvm_oct_mem_empty_fpa(CVMX_FPA_PACKET_POOL, CVMX_FPA_PACKET_POOL_SIZE,
			      num_packet_buffers);
	cvm_oct_mem_empty_fpa(CVMX_FPA_WQE_POOL, CVMX_FPA_WQE_POOL_SIZE,
			      num_packet_buffers);
	if (CVMX_FPA_OUTPUT_BUFFER_POOL != CVMX_FPA_PACKET_POOL)
		cvm_oct_mem_empty_fpa(CVMX_FPA_OUTPUT_BUFFER_POOL,
				      CVMX_FPA_OUTPUT_BUFFER_POOL_SIZE, 128);
}

<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cavium Networks <support@caviumnetworks.com>");
MODULE_DESCRIPTION("Cavium Networks Octeon ethernet driver.");
module_init(cvm_oct_init_module);
module_exit(cvm_oct_cleanup_module);
=======
static const struct of_device_id cvm_oct_match[] = {
	{
		.compatible = "cavium,octeon-3860-pip",
	},
	{},
};
MODULE_DEVICE_TABLE(of, cvm_oct_match);

static struct platform_driver cvm_oct_driver = {
	.probe		= cvm_oct_probe,
	.remove_new	= cvm_oct_remove,
	.driver		= {
		.name	= KBUILD_MODNAME,
		.of_match_table = cvm_oct_match,
	},
};

module_platform_driver(cvm_oct_driver);

MODULE_SOFTDEP("pre: mdio-cavium");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cavium Networks <support@caviumnetworks.com>");
MODULE_DESCRIPTION("Cavium Networks Octeon ethernet driver.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
