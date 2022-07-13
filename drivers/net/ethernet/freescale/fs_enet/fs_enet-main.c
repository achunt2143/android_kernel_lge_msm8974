/*
 * Combined Ethernet driver for Motorola MPC8xx and MPC82xx.
 *
 * Copyright (c) 2003 Intracom S.A.
 *  by Pantelis Antoniou <panto@intracom.gr>
 *
 * 2005 (c) MontaVista Software, Inc.
 * Vitaly Bordug <vbordug@ru.mvista.com>
 *
 * Heavily based on original FEC driver by Dan Malek <dan@embeddededge.com>
 * and modifications by Joakim Tjernlund <joakim.tjernlund@lumentis.se>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/bitops.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/phy.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/of_mdio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/of_net.h>

#include <linux/vmalloc.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
=======
#include <linux/property.h>
#include <linux/of.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/pgtable.h>

#include <linux/vmalloc.h>
#include <asm/irq.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "fs_enet.h"

/*************************************************/

MODULE_AUTHOR("Pantelis Antoniou <panto@intracom.gr>");
MODULE_DESCRIPTION("Freescale Ethernet Driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION(DRV_MODULE_VERSION);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int fs_enet_debug = -1; /* -1 == use FS_ENET_DEF_MSG_ENABLE as value */
module_param(fs_enet_debug, int, 0);
MODULE_PARM_DESC(fs_enet_debug,
		 "Freescale bitmapped debugging message enable value");

<<<<<<< HEAD
=======
#define RX_RING_SIZE	32
#define TX_RING_SIZE	64

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_NET_POLL_CONTROLLER
static void fs_enet_netpoll(struct net_device *dev);
#endif

static void fs_set_multicast_list(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	(*fep->ops->set_multicast_list)(dev);
}

static void skb_align(struct sk_buff *skb, int align)
{
	int off = ((unsigned long)skb->data) & (align - 1);

	if (off)
		skb_reserve(skb, align - off);
}

<<<<<<< HEAD
/* NAPI receive function */
static int fs_enet_rx_napi(struct napi_struct *napi, int budget)
=======
/* NAPI function */
static int fs_enet_napi(struct napi_struct *napi, int budget)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fs_enet_private *fep = container_of(napi, struct fs_enet_private, napi);
	struct net_device *dev = fep->ndev;
	const struct fs_platform_info *fpi = fep->fpi;
	cbd_t __iomem *bdp;
<<<<<<< HEAD
	struct sk_buff *skb, *skbn, *skbt;
	int received = 0;
	u16 pkt_len, sc;
	int curidx;

	/*
	 * First, grab all of the stats for the incoming packet.
	 * These get messed up if we get called due to a busy condition.
	 */
	bdp = fep->cur_rx;

	/* clear RX status bits for napi*/
	(*fep->ops->napi_clear_rx_event)(dev);

	while (((sc = CBDR_SC(bdp)) & BD_ENET_RX_EMPTY) == 0) {
		curidx = bdp - fep->rx_bd_base;

		/*
		 * Since we have allocated space to hold a complete frame,
		 * the last indicator should be set.
		 */
		if ((sc & BD_ENET_RX_LAST) == 0)
			dev_warn(fep->dev, "rcv is not +last\n");

		/*
		 * Check for errors.
		 */
		if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH | BD_ENET_RX_CL |
			  BD_ENET_RX_NO | BD_ENET_RX_CR | BD_ENET_RX_OV)) {
			fep->stats.rx_errors++;
			/* Frame too long or too short. */
			if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH))
				fep->stats.rx_length_errors++;
			/* Frame alignment */
			if (sc & (BD_ENET_RX_NO | BD_ENET_RX_CL))
				fep->stats.rx_frame_errors++;
			/* CRC Error */
			if (sc & BD_ENET_RX_CR)
				fep->stats.rx_crc_errors++;
			/* FIFO overrun */
			if (sc & BD_ENET_RX_OV)
				fep->stats.rx_crc_errors++;

			skb = fep->rx_skbuff[curidx];

			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE);

			skbn = skb;

		} else {
			skb = fep->rx_skbuff[curidx];

			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE);

			/*
			 * Process the incoming frame.
			 */
			fep->stats.rx_packets++;
			pkt_len = CBDR_DATLEN(bdp) - 4;	/* remove CRC */
			fep->stats.rx_bytes += pkt_len + 4;

			if (pkt_len <= fpi->rx_copybreak) {
				/* +2 to make IP header L1 cache aligned */
				skbn = netdev_alloc_skb(dev, pkt_len + 2);
				if (skbn != NULL) {
					skb_reserve(skbn, 2);	/* align IP header */
					skb_copy_from_linear_data(skb,
						      skbn->data, pkt_len);
					/* swap */
					skbt = skb;
					skb = skbn;
					skbn = skbt;
				}
			} else {
				skbn = netdev_alloc_skb(dev, ENET_RX_FRSIZE);

				if (skbn)
					skb_align(skbn, ENET_RX_ALIGN);
			}

			if (skbn != NULL) {
				skb_put(skb, pkt_len);	/* Make room */
				skb->protocol = eth_type_trans(skb, dev);
				received++;
				netif_receive_skb(skb);
			} else {
				dev_warn(fep->dev,
					 "Memory squeeze, dropping packet.\n");
				fep->stats.rx_dropped++;
				skbn = skb;
			}
		}

		fep->rx_skbuff[curidx] = skbn;
		CBDW_BUFADDR(bdp, dma_map_single(fep->dev, skbn->data,
			     L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
			     DMA_FROM_DEVICE));
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (sc & ~BD_ENET_RX_STATS) | BD_ENET_RX_EMPTY);

		/*
		 * Update BD pointer to next entry.
		 */
		if ((sc & BD_ENET_RX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->rx_bd_base;

		(*fep->ops->rx_bd_done)(dev);

		if (received >= budget)
			break;
	}

	fep->cur_rx = bdp;

	if (received < budget) {
		/* done */
		napi_complete(napi);
		(*fep->ops->napi_enable_rx)(dev);
	}
	return received;
}

/* non NAPI receive function */
static int fs_enet_rx_non_napi(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	const struct fs_platform_info *fpi = fep->fpi;
	cbd_t __iomem *bdp;
	struct sk_buff *skb, *skbn, *skbt;
	int received = 0;
	u16 pkt_len, sc;
	int curidx;
	/*
	 * First, grab all of the stats for the incoming packet.
	 * These get messed up if we get called due to a busy condition.
	 */
	bdp = fep->cur_rx;

	while (((sc = CBDR_SC(bdp)) & BD_ENET_RX_EMPTY) == 0) {

		curidx = bdp - fep->rx_bd_base;

		/*
		 * Since we have allocated space to hold a complete frame,
		 * the last indicator should be set.
		 */
		if ((sc & BD_ENET_RX_LAST) == 0)
			dev_warn(fep->dev, "rcv is not +last\n");

		/*
		 * Check for errors.
		 */
		if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH | BD_ENET_RX_CL |
			  BD_ENET_RX_NO | BD_ENET_RX_CR | BD_ENET_RX_OV)) {
			fep->stats.rx_errors++;
			/* Frame too long or too short. */
			if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH))
				fep->stats.rx_length_errors++;
			/* Frame alignment */
			if (sc & (BD_ENET_RX_NO | BD_ENET_RX_CL))
				fep->stats.rx_frame_errors++;
			/* CRC Error */
			if (sc & BD_ENET_RX_CR)
				fep->stats.rx_crc_errors++;
			/* FIFO overrun */
			if (sc & BD_ENET_RX_OV)
				fep->stats.rx_crc_errors++;

			skb = fep->rx_skbuff[curidx];

			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE);

			skbn = skb;

		} else {

			skb = fep->rx_skbuff[curidx];

			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE);

			/*
			 * Process the incoming frame.
			 */
			fep->stats.rx_packets++;
			pkt_len = CBDR_DATLEN(bdp) - 4;	/* remove CRC */
			fep->stats.rx_bytes += pkt_len + 4;

			if (pkt_len <= fpi->rx_copybreak) {
				/* +2 to make IP header L1 cache aligned */
				skbn = netdev_alloc_skb(dev, pkt_len + 2);
				if (skbn != NULL) {
					skb_reserve(skbn, 2);	/* align IP header */
					skb_copy_from_linear_data(skb,
						      skbn->data, pkt_len);
					/* swap */
					skbt = skb;
					skb = skbn;
					skbn = skbt;
				}
			} else {
				skbn = netdev_alloc_skb(dev, ENET_RX_FRSIZE);

				if (skbn)
					skb_align(skbn, ENET_RX_ALIGN);
			}

			if (skbn != NULL) {
				skb_put(skb, pkt_len);	/* Make room */
				skb->protocol = eth_type_trans(skb, dev);
				received++;
				netif_rx(skb);
			} else {
				dev_warn(fep->dev,
					 "Memory squeeze, dropping packet.\n");
				fep->stats.rx_dropped++;
				skbn = skb;
			}
		}

		fep->rx_skbuff[curidx] = skbn;
		CBDW_BUFADDR(bdp, dma_map_single(fep->dev, skbn->data,
			     L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
			     DMA_FROM_DEVICE));
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (sc & ~BD_ENET_RX_STATS) | BD_ENET_RX_EMPTY);

		/*
		 * Update BD pointer to next entry.
		 */
		if ((sc & BD_ENET_RX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->rx_bd_base;

		(*fep->ops->rx_bd_done)(dev);
	}

	fep->cur_rx = bdp;

	return 0;
}

static void fs_enet_tx(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	cbd_t __iomem *bdp;
	struct sk_buff *skb;
	int dirtyidx, do_wake, do_restart;
	u16 sc;
=======
	struct sk_buff *skb, *skbn;
	int received = 0;
	u16 pkt_len, sc;
	int curidx;
	int dirtyidx, do_wake, do_restart;
	int tx_left = TX_RING_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&fep->tx_lock);
	bdp = fep->dirty_tx;

<<<<<<< HEAD
	do_wake = do_restart = 0;
	while (((sc = CBDR_SC(bdp)) & BD_ENET_TX_READY) == 0) {
=======
	/* clear status bits for napi*/
	(*fep->ops->napi_clear_event)(dev);

	do_wake = do_restart = 0;
	while (((sc = CBDR_SC(bdp)) & BD_ENET_TX_READY) == 0 && tx_left) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dirtyidx = bdp - fep->tx_bd_base;

		if (fep->tx_free == fep->tx_ring)
			break;

		skb = fep->tx_skbuff[dirtyidx];

		/*
		 * Check for errors.
		 */
		if (sc & (BD_ENET_TX_HB | BD_ENET_TX_LC |
			  BD_ENET_TX_RL | BD_ENET_TX_UN | BD_ENET_TX_CSL)) {

			if (sc & BD_ENET_TX_HB)	/* No heartbeat */
<<<<<<< HEAD
				fep->stats.tx_heartbeat_errors++;
			if (sc & BD_ENET_TX_LC)	/* Late collision */
				fep->stats.tx_window_errors++;
			if (sc & BD_ENET_TX_RL)	/* Retrans limit */
				fep->stats.tx_aborted_errors++;
			if (sc & BD_ENET_TX_UN)	/* Underrun */
				fep->stats.tx_fifo_errors++;
			if (sc & BD_ENET_TX_CSL)	/* Carrier lost */
				fep->stats.tx_carrier_errors++;

			if (sc & (BD_ENET_TX_LC | BD_ENET_TX_RL | BD_ENET_TX_UN)) {
				fep->stats.tx_errors++;
				do_restart = 1;
			}
		} else
			fep->stats.tx_packets++;
=======
				dev->stats.tx_heartbeat_errors++;
			if (sc & BD_ENET_TX_LC)	/* Late collision */
				dev->stats.tx_window_errors++;
			if (sc & BD_ENET_TX_RL)	/* Retrans limit */
				dev->stats.tx_aborted_errors++;
			if (sc & BD_ENET_TX_UN)	/* Underrun */
				dev->stats.tx_fifo_errors++;
			if (sc & BD_ENET_TX_CSL)	/* Carrier lost */
				dev->stats.tx_carrier_errors++;

			if (sc & (BD_ENET_TX_LC | BD_ENET_TX_RL | BD_ENET_TX_UN)) {
				dev->stats.tx_errors++;
				do_restart = 1;
			}
		} else
			dev->stats.tx_packets++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (sc & BD_ENET_TX_READY) {
			dev_warn(fep->dev,
				 "HEY! Enet xmit interrupt and TX_READY.\n");
		}

		/*
		 * Deferred means some collisions occurred during transmit,
		 * but we eventually sent the packet OK.
		 */
		if (sc & BD_ENET_TX_DEF)
<<<<<<< HEAD
			fep->stats.collisions++;

		/* unmap */
		dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				skb->len, DMA_TO_DEVICE);
=======
			dev->stats.collisions++;

		/* unmap */
		if (fep->mapped_as_page[dirtyidx])
			dma_unmap_page(fep->dev, CBDR_BUFADDR(bdp),
				       CBDR_DATLEN(bdp), DMA_TO_DEVICE);
		else
			dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
					 CBDR_DATLEN(bdp), DMA_TO_DEVICE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Free the sk buffer associated with this last transmit.
		 */
<<<<<<< HEAD
		dev_kfree_skb_irq(skb);
		fep->tx_skbuff[dirtyidx] = NULL;
=======
		if (skb) {
			dev_kfree_skb(skb);
			fep->tx_skbuff[dirtyidx] = NULL;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Update pointer to next buffer descriptor to be transmitted.
		 */
		if ((sc & BD_ENET_TX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->tx_bd_base;

		/*
		 * Since we have freed up a buffer, the ring is no longer
		 * full.
		 */
<<<<<<< HEAD
		if (!fep->tx_free++)
			do_wake = 1;
=======
		if (++fep->tx_free == MAX_SKB_FRAGS)
			do_wake = 1;
		tx_left--;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	fep->dirty_tx = bdp;

	if (do_restart)
		(*fep->ops->tx_restart)(dev);

	spin_unlock(&fep->tx_lock);

	if (do_wake)
		netif_wake_queue(dev);
<<<<<<< HEAD
=======

	/*
	 * First, grab all of the stats for the incoming packet.
	 * These get messed up if we get called due to a busy condition.
	 */
	bdp = fep->cur_rx;

	while (((sc = CBDR_SC(bdp)) & BD_ENET_RX_EMPTY) == 0 &&
	       received < budget) {
		curidx = bdp - fep->rx_bd_base;

		/*
		 * Since we have allocated space to hold a complete frame,
		 * the last indicator should be set.
		 */
		if ((sc & BD_ENET_RX_LAST) == 0)
			dev_warn(fep->dev, "rcv is not +last\n");

		/*
		 * Check for errors.
		 */
		if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH | BD_ENET_RX_CL |
			  BD_ENET_RX_NO | BD_ENET_RX_CR | BD_ENET_RX_OV)) {
			dev->stats.rx_errors++;
			/* Frame too long or too short. */
			if (sc & (BD_ENET_RX_LG | BD_ENET_RX_SH))
				dev->stats.rx_length_errors++;
			/* Frame alignment */
			if (sc & (BD_ENET_RX_NO | BD_ENET_RX_CL))
				dev->stats.rx_frame_errors++;
			/* CRC Error */
			if (sc & BD_ENET_RX_CR)
				dev->stats.rx_crc_errors++;
			/* FIFO overrun */
			if (sc & BD_ENET_RX_OV)
				dev->stats.rx_crc_errors++;

			skbn = fep->rx_skbuff[curidx];
		} else {
			skb = fep->rx_skbuff[curidx];

			/*
			 * Process the incoming frame.
			 */
			dev->stats.rx_packets++;
			pkt_len = CBDR_DATLEN(bdp) - 4;	/* remove CRC */
			dev->stats.rx_bytes += pkt_len + 4;

			if (pkt_len <= fpi->rx_copybreak) {
				/* +2 to make IP header L1 cache aligned */
				skbn = netdev_alloc_skb(dev, pkt_len + 2);
				if (skbn != NULL) {
					skb_reserve(skbn, 2);	/* align IP header */
					skb_copy_from_linear_data(skb,
						      skbn->data, pkt_len);
					swap(skb, skbn);
					dma_sync_single_for_cpu(fep->dev,
						CBDR_BUFADDR(bdp),
						L1_CACHE_ALIGN(pkt_len),
						DMA_FROM_DEVICE);
				}
			} else {
				skbn = netdev_alloc_skb(dev, ENET_RX_FRSIZE);

				if (skbn) {
					dma_addr_t dma;

					skb_align(skbn, ENET_RX_ALIGN);

					dma_unmap_single(fep->dev,
						CBDR_BUFADDR(bdp),
						L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
						DMA_FROM_DEVICE);

					dma = dma_map_single(fep->dev,
						skbn->data,
						L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
						DMA_FROM_DEVICE);
					CBDW_BUFADDR(bdp, dma);
				}
			}

			if (skbn != NULL) {
				skb_put(skb, pkt_len);	/* Make room */
				skb->protocol = eth_type_trans(skb, dev);
				received++;
				netif_receive_skb(skb);
			} else {
				dev->stats.rx_dropped++;
				skbn = skb;
			}
		}

		fep->rx_skbuff[curidx] = skbn;
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (sc & ~BD_ENET_RX_STATS) | BD_ENET_RX_EMPTY);

		/*
		 * Update BD pointer to next entry.
		 */
		if ((sc & BD_ENET_RX_WRAP) == 0)
			bdp++;
		else
			bdp = fep->rx_bd_base;

		(*fep->ops->rx_bd_done)(dev);
	}

	fep->cur_rx = bdp;

	if (received < budget && tx_left) {
		/* done */
		napi_complete_done(napi, received);
		(*fep->ops->napi_enable)(dev);

		return received;
	}

	return budget;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The interrupt handler.
 * This is called from the MPC core interrupt.
 */
static irqreturn_t
fs_enet_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	struct fs_enet_private *fep;
<<<<<<< HEAD
	const struct fs_platform_info *fpi;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 int_events;
	u32 int_clr_events;
	int nr, napi_ok;
	int handled;

	fep = netdev_priv(dev);
<<<<<<< HEAD
	fpi = fep->fpi;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nr = 0;
	while ((int_events = (*fep->ops->get_int_events)(dev)) != 0) {
		nr++;

		int_clr_events = int_events;
<<<<<<< HEAD
		if (fpi->use_napi)
			int_clr_events &= ~fep->ev_napi_rx;
=======
		int_clr_events &= ~fep->ev_napi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		(*fep->ops->clear_int_events)(dev, int_clr_events);

		if (int_events & fep->ev_err)
			(*fep->ops->ev_error)(dev, int_events);

<<<<<<< HEAD
		if (int_events & fep->ev_rx) {
			if (!fpi->use_napi)
				fs_enet_rx_non_napi(dev);
			else {
				napi_ok = napi_schedule_prep(&fep->napi);

				(*fep->ops->napi_disable_rx)(dev);
				(*fep->ops->clear_int_events)(dev, fep->ev_napi_rx);

				/* NOTE: it is possible for FCCs in NAPI mode    */
				/* to submit a spurious interrupt while in poll  */
				if (napi_ok)
					__napi_schedule(&fep->napi);
			}
		}

		if (int_events & fep->ev_tx)
			fs_enet_tx(dev);
=======
		if (int_events & fep->ev) {
			napi_ok = napi_schedule_prep(&fep->napi);

			(*fep->ops->napi_disable)(dev);
			(*fep->ops->clear_int_events)(dev, fep->ev_napi);

			/* NOTE: it is possible for FCCs in NAPI mode    */
			/* to submit a spurious interrupt while in poll  */
			if (napi_ok)
				__napi_schedule(&fep->napi);
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	handled = nr > 0;
	return IRQ_RETVAL(handled);
}

void fs_init_bds(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	cbd_t __iomem *bdp;
	struct sk_buff *skb;
	int i;

	fs_cleanup_bds(dev);

	fep->dirty_tx = fep->cur_tx = fep->tx_bd_base;
	fep->tx_free = fep->tx_ring;
	fep->cur_rx = fep->rx_bd_base;

	/*
	 * Initialize the receive buffer descriptors.
	 */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		skb = netdev_alloc_skb(dev, ENET_RX_FRSIZE);
<<<<<<< HEAD
		if (skb == NULL) {
			dev_warn(fep->dev,
				 "Memory squeeze, unable to allocate skb\n");
			break;
		}
=======
		if (skb == NULL)
			break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb_align(skb, ENET_RX_ALIGN);
		fep->rx_skbuff[i] = skb;
		CBDW_BUFADDR(bdp,
			dma_map_single(fep->dev, skb->data,
				L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
				DMA_FROM_DEVICE));
		CBDW_DATLEN(bdp, 0);	/* zero */
		CBDW_SC(bdp, BD_ENET_RX_EMPTY |
			((i < fep->rx_ring - 1) ? 0 : BD_SC_WRAP));
	}
	/*
	 * if we failed, fillup remainder
	 */
	for (; i < fep->rx_ring; i++, bdp++) {
		fep->rx_skbuff[i] = NULL;
		CBDW_SC(bdp, (i < fep->rx_ring - 1) ? 0 : BD_SC_WRAP);
	}

	/*
	 * ...and the same for transmit.
	 */
	for (i = 0, bdp = fep->tx_bd_base; i < fep->tx_ring; i++, bdp++) {
		fep->tx_skbuff[i] = NULL;
		CBDW_BUFADDR(bdp, 0);
		CBDW_DATLEN(bdp, 0);
		CBDW_SC(bdp, (i < fep->tx_ring - 1) ? 0 : BD_SC_WRAP);
	}
}

void fs_cleanup_bds(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct sk_buff *skb;
	cbd_t __iomem *bdp;
	int i;

	/*
	 * Reset SKB transmit buffers.
	 */
	for (i = 0, bdp = fep->tx_bd_base; i < fep->tx_ring; i++, bdp++) {
		if ((skb = fep->tx_skbuff[i]) == NULL)
			continue;

		/* unmap */
		dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
				skb->len, DMA_TO_DEVICE);

		fep->tx_skbuff[i] = NULL;
		dev_kfree_skb(skb);
	}

	/*
	 * Reset SKB receive buffers
	 */
	for (i = 0, bdp = fep->rx_bd_base; i < fep->rx_ring; i++, bdp++) {
		if ((skb = fep->rx_skbuff[i]) == NULL)
			continue;

		/* unmap */
		dma_unmap_single(fep->dev, CBDR_BUFADDR(bdp),
			L1_CACHE_ALIGN(PKT_MAXBUF_SIZE),
			DMA_FROM_DEVICE);

		fep->rx_skbuff[i] = NULL;

		dev_kfree_skb(skb);
	}
}

/**********************************************************************************/

#ifdef CONFIG_FS_ENET_MPC5121_FEC
/*
 * MPC5121 FEC requeries 4-byte alignment for TX data buffer!
 */
static struct sk_buff *tx_skb_align_workaround(struct net_device *dev,
					       struct sk_buff *skb)
{
	struct sk_buff *new_skb;
<<<<<<< HEAD
	struct fs_enet_private *fep = netdev_priv(dev);

	/* Alloc new skb */
	new_skb = netdev_alloc_skb(dev, skb->len + 4);
	if (!new_skb) {
		if (net_ratelimit()) {
			dev_warn(fep->dev,
				 "Memory squeeze, dropping tx packet.\n");
		}
		return NULL;
	}
=======

	if (skb_linearize(skb))
		return NULL;

	/* Alloc new skb */
	new_skb = netdev_alloc_skb(dev, skb->len + 4);
	if (!new_skb)
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Make sure new skb is properly aligned */
	skb_align(new_skb, 4);

	/* Copy data to new skb ... */
	skb_copy_from_linear_data(skb, new_skb->data, skb->len);
	skb_put(new_skb, skb->len);

	/* ... and free an old one */
	dev_kfree_skb_any(skb);

	return new_skb;
}
#endif

<<<<<<< HEAD
static int fs_enet_start_xmit(struct sk_buff *skb, struct net_device *dev)
=======
static netdev_tx_t
fs_enet_start_xmit(struct sk_buff *skb, struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	cbd_t __iomem *bdp;
	int curidx;
	u16 sc;
<<<<<<< HEAD
	unsigned long flags;

#ifdef CONFIG_FS_ENET_MPC5121_FEC
	if (((unsigned long)skb->data) & 0x3) {
=======
	int nr_frags;
	skb_frag_t *frag;
	int len;
#ifdef CONFIG_FS_ENET_MPC5121_FEC
	int is_aligned = 1;
	int i;

	if (!IS_ALIGNED((unsigned long)skb->data, 4)) {
		is_aligned = 0;
	} else {
		nr_frags = skb_shinfo(skb)->nr_frags;
		frag = skb_shinfo(skb)->frags;
		for (i = 0; i < nr_frags; i++, frag++) {
			if (!IS_ALIGNED(skb_frag_off(frag), 4)) {
				is_aligned = 0;
				break;
			}
		}
	}

	if (!is_aligned) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		skb = tx_skb_align_workaround(dev, skb);
		if (!skb) {
			/*
			 * We have lost packet due to memory allocation error
			 * in tx_skb_align_workaround(). Hopefully original
			 * skb is still valid, so try transmit it later.
			 */
			return NETDEV_TX_BUSY;
		}
	}
#endif
<<<<<<< HEAD
	spin_lock_irqsave(&fep->tx_lock, flags);
=======

	spin_lock(&fep->tx_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Fill in a Tx ring entry
	 */
	bdp = fep->cur_tx;

<<<<<<< HEAD
	if (!fep->tx_free || (CBDR_SC(bdp) & BD_ENET_TX_READY)) {
		netif_stop_queue(dev);
		spin_unlock_irqrestore(&fep->tx_lock, flags);
=======
	nr_frags = skb_shinfo(skb)->nr_frags;
	if (fep->tx_free <= nr_frags || (CBDR_SC(bdp) & BD_ENET_TX_READY)) {
		netif_stop_queue(dev);
		spin_unlock(&fep->tx_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Ooops.  All transmit buffers are full.  Bail out.
		 * This should not happen, since the tx queue should be stopped.
		 */
		dev_warn(fep->dev, "tx queue full!.\n");
		return NETDEV_TX_BUSY;
	}

	curidx = bdp - fep->tx_bd_base;
<<<<<<< HEAD
	/*
	 * Clear all of the status flags.
	 */
	CBDC_SC(bdp, BD_ENET_TX_STATS);

	/*
	 * Save skb pointer.
	 */
	fep->tx_skbuff[curidx] = skb;

	fep->stats.tx_bytes += skb->len;

=======

	len = skb->len;
	dev->stats.tx_bytes += len;
	if (nr_frags)
		len -= skb->data_len;
	fep->tx_free -= nr_frags + 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Push the data cache so the CPM does not get stale memory data.
	 */
	CBDW_BUFADDR(bdp, dma_map_single(fep->dev,
<<<<<<< HEAD
				skb->data, skb->len, DMA_TO_DEVICE));
	CBDW_DATLEN(bdp, skb->len);

	/*
	 * If this was the last BD in the ring, start at the beginning again.
	 */
	if ((CBDR_SC(bdp) & BD_ENET_TX_WRAP) == 0)
		fep->cur_tx++;
	else
		fep->cur_tx = fep->tx_bd_base;

	if (!--fep->tx_free)
		netif_stop_queue(dev);
=======
				skb->data, len, DMA_TO_DEVICE));
	CBDW_DATLEN(bdp, len);

	fep->mapped_as_page[curidx] = 0;
	frag = skb_shinfo(skb)->frags;
	while (nr_frags) {
		CBDC_SC(bdp,
			BD_ENET_TX_STATS | BD_ENET_TX_INTR | BD_ENET_TX_LAST |
			BD_ENET_TX_TC);
		CBDS_SC(bdp, BD_ENET_TX_READY);

		if ((CBDR_SC(bdp) & BD_ENET_TX_WRAP) == 0) {
			bdp++;
			curidx++;
		} else {
			bdp = fep->tx_bd_base;
			curidx = 0;
		}

		len = skb_frag_size(frag);
		CBDW_BUFADDR(bdp, skb_frag_dma_map(fep->dev, frag, 0, len,
						   DMA_TO_DEVICE));
		CBDW_DATLEN(bdp, len);

		fep->tx_skbuff[curidx] = NULL;
		fep->mapped_as_page[curidx] = 1;

		frag++;
		nr_frags--;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Trigger transmission start */
	sc = BD_ENET_TX_READY | BD_ENET_TX_INTR |
	     BD_ENET_TX_LAST | BD_ENET_TX_TC;

	/* note that while FEC does not have this bit
	 * it marks it as available for software use
	 * yay for hw reuse :) */
	if (skb->len <= 60)
		sc |= BD_ENET_TX_PAD;
<<<<<<< HEAD
	CBDS_SC(bdp, sc);

=======
	CBDC_SC(bdp, BD_ENET_TX_STATS);
	CBDS_SC(bdp, sc);

	/* Save skb pointer. */
	fep->tx_skbuff[curidx] = skb;

	/* If this was the last BD in the ring, start at the beginning again. */
	if ((CBDR_SC(bdp) & BD_ENET_TX_WRAP) == 0)
		bdp++;
	else
		bdp = fep->tx_bd_base;
	fep->cur_tx = bdp;

	if (fep->tx_free < MAX_SKB_FRAGS)
		netif_stop_queue(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_tx_timestamp(skb);

	(*fep->ops->tx_kickstart)(dev);

<<<<<<< HEAD
	spin_unlock_irqrestore(&fep->tx_lock, flags);
=======
	spin_unlock(&fep->tx_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static void fs_timeout(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;
	int wake = 0;

	fep->stats.tx_errors++;
=======
static void fs_timeout_work(struct work_struct *work)
{
	struct fs_enet_private *fep = container_of(work, struct fs_enet_private,
						   timeout_work);
	struct net_device *dev = fep->ndev;
	unsigned long flags;
	int wake = 0;

	dev->stats.tx_errors++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&fep->lock, flags);

	if (dev->flags & IFF_UP) {
<<<<<<< HEAD
		phy_stop(fep->phydev);
		(*fep->ops->stop)(dev);
		(*fep->ops->restart)(dev);
		phy_start(fep->phydev);
	}

	phy_start(fep->phydev);
	wake = fep->tx_free && !(CBDR_SC(fep->cur_tx) & BD_ENET_TX_READY);
=======
		phy_stop(dev->phydev);
		(*fep->ops->stop)(dev);
		(*fep->ops->restart)(dev);
	}

	phy_start(dev->phydev);
	wake = fep->tx_free >= MAX_SKB_FRAGS &&
	       !(CBDR_SC(fep->cur_tx) & BD_ENET_TX_READY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&fep->lock, flags);

	if (wake)
		netif_wake_queue(dev);
}

<<<<<<< HEAD
=======
static void fs_timeout(struct net_device *dev, unsigned int txqueue)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	schedule_work(&fep->timeout_work);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*-----------------------------------------------------------------------------
 *  generic link-change handler - should be sufficient for most cases
 *-----------------------------------------------------------------------------*/
static void generic_adjust_link(struct  net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
<<<<<<< HEAD
	struct phy_device *phydev = fep->phydev;
=======
	struct phy_device *phydev = dev->phydev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int new_state = 0;

	if (phydev->link) {
		/* adjust to duplex mode */
		if (phydev->duplex != fep->oldduplex) {
			new_state = 1;
			fep->oldduplex = phydev->duplex;
		}

		if (phydev->speed != fep->oldspeed) {
			new_state = 1;
			fep->oldspeed = phydev->speed;
		}

		if (!fep->oldlink) {
			new_state = 1;
			fep->oldlink = 1;
		}

		if (new_state)
			fep->ops->restart(dev);
	} else if (fep->oldlink) {
		new_state = 1;
		fep->oldlink = 0;
		fep->oldspeed = 0;
		fep->oldduplex = -1;
	}

	if (new_state && netif_msg_link(fep))
		phy_print_status(phydev);
}


static void fs_adjust_link(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;

	spin_lock_irqsave(&fep->lock, flags);

	if(fep->ops->adjust_link)
		fep->ops->adjust_link(dev);
	else
		generic_adjust_link(dev);

	spin_unlock_irqrestore(&fep->lock, flags);
}

static int fs_init_phy(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct phy_device *phydev;
	phy_interface_t iface;

	fep->oldlink = 0;
	fep->oldspeed = 0;
	fep->oldduplex = -1;

	iface = fep->fpi->use_rmii ?
		PHY_INTERFACE_MODE_RMII : PHY_INTERFACE_MODE_MII;

	phydev = of_phy_connect(dev, fep->fpi->phy_node, &fs_adjust_link, 0,
				iface);
	if (!phydev) {
<<<<<<< HEAD
		phydev = of_phy_connect_fixed_link(dev, &fs_adjust_link,
						   iface);
	}
	if (!phydev) {
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&dev->dev, "Could not attach to PHY\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	fep->phydev = phydev;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int fs_enet_open(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	int r;
	int err;

	/* to initialize the fep->cur_rx,... */
<<<<<<< HEAD
	/* not doing this, will cause a crash in fs_enet_rx_napi */
	fs_init_bds(fep->ndev);

	if (fep->fpi->use_napi)
		napi_enable(&fep->napi);
=======
	/* not doing this, will cause a crash in fs_enet_napi */
	fs_init_bds(fep->ndev);

	napi_enable(&fep->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Install our interrupt handler. */
	r = request_irq(fep->interrupt, fs_enet_interrupt, IRQF_SHARED,
			"fs_enet-mac", dev);
	if (r != 0) {
		dev_err(fep->dev, "Could not allocate FS_ENET IRQ!");
<<<<<<< HEAD
		if (fep->fpi->use_napi)
			napi_disable(&fep->napi);
=======
		napi_disable(&fep->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	err = fs_init_phy(dev);
	if (err) {
		free_irq(fep->interrupt, dev);
<<<<<<< HEAD
		if (fep->fpi->use_napi)
			napi_disable(&fep->napi);
		return err;
	}
	phy_start(fep->phydev);
=======
		napi_disable(&fep->napi);
		return err;
	}
	phy_start(dev->phydev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netif_start_queue(dev);

	return 0;
}

static int fs_enet_close(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;

	netif_stop_queue(dev);
	netif_carrier_off(dev);
<<<<<<< HEAD
	if (fep->fpi->use_napi)
		napi_disable(&fep->napi);
	phy_stop(fep->phydev);
=======
	napi_disable(&fep->napi);
	cancel_work_sync(&fep->timeout_work);
	phy_stop(dev->phydev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&fep->lock, flags);
	spin_lock(&fep->tx_lock);
	(*fep->ops->stop)(dev);
	spin_unlock(&fep->tx_lock);
	spin_unlock_irqrestore(&fep->lock, flags);

	/* release any irqs */
<<<<<<< HEAD
	phy_disconnect(fep->phydev);
	fep->phydev = NULL;
=======
	phy_disconnect(dev->phydev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_irq(fep->interrupt, dev);

	return 0;
}

<<<<<<< HEAD
static struct net_device_stats *fs_enet_get_stats(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	return &fep->stats;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*************************************************************************/

static void fs_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info)
{
<<<<<<< HEAD
	strcpy(info->driver, DRV_MODULE_NAME);
	strcpy(info->version, DRV_MODULE_VERSION);
=======
	strscpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fs_get_regs_len(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	return (*fep->ops->get_regs_len)(dev);
}

static void fs_get_regs(struct net_device *dev, struct ethtool_regs *regs,
			 void *p)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	unsigned long flags;
	int r, len;

	len = regs->len;

	spin_lock_irqsave(&fep->lock, flags);
	r = (*fep->ops->get_regs)(dev, p, &len);
	spin_unlock_irqrestore(&fep->lock, flags);

	if (r == 0)
		regs->version = 0;
}

<<<<<<< HEAD
static int fs_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	if (!fep->phydev)
		return -ENODEV;

	return phy_ethtool_gset(fep->phydev, cmd);
}

static int fs_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	if (!fep->phydev)
		return -ENODEV;

	return phy_ethtool_sset(fep->phydev, cmd);
}

static int fs_nway_reset(struct net_device *dev)
{
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u32 fs_get_msglevel(struct net_device *dev)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	return fep->msg_enable;
}

static void fs_set_msglevel(struct net_device *dev, u32 value)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	fep->msg_enable = value;
}

<<<<<<< HEAD
static const struct ethtool_ops fs_ethtool_ops = {
	.get_drvinfo = fs_get_drvinfo,
	.get_regs_len = fs_get_regs_len,
	.get_settings = fs_get_settings,
	.set_settings = fs_set_settings,
	.nway_reset = fs_nway_reset,
=======
static int fs_get_tunable(struct net_device *dev,
			  const struct ethtool_tunable *tuna, void *data)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct fs_platform_info *fpi = fep->fpi;
	int ret = 0;

	switch (tuna->id) {
	case ETHTOOL_RX_COPYBREAK:
		*(u32 *)data = fpi->rx_copybreak;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int fs_set_tunable(struct net_device *dev,
			  const struct ethtool_tunable *tuna, const void *data)
{
	struct fs_enet_private *fep = netdev_priv(dev);
	struct fs_platform_info *fpi = fep->fpi;
	int ret = 0;

	switch (tuna->id) {
	case ETHTOOL_RX_COPYBREAK:
		fpi->rx_copybreak = *(u32 *)data;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static const struct ethtool_ops fs_ethtool_ops = {
	.get_drvinfo = fs_get_drvinfo,
	.get_regs_len = fs_get_regs_len,
	.nway_reset = phy_ethtool_nway_reset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_link = ethtool_op_get_link,
	.get_msglevel = fs_get_msglevel,
	.set_msglevel = fs_set_msglevel,
	.get_regs = fs_get_regs,
<<<<<<< HEAD
};

static int fs_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct fs_enet_private *fep = netdev_priv(dev);

	if (!netif_running(dev))
		return -EINVAL;

	return phy_mii_ioctl(fep->phydev, rq, cmd);
}

extern int fs_mii_connect(struct net_device *dev);
extern void fs_mii_disconnect(struct net_device *dev);

/**************************************************************************************/

#ifdef CONFIG_FS_ENET_HAS_FEC
#define IS_FEC(match) ((match)->data == &fs_fec_ops)
#else
#define IS_FEC(match) 0
=======
	.get_ts_info = ethtool_op_get_ts_info,
	.get_link_ksettings = phy_ethtool_get_link_ksettings,
	.set_link_ksettings = phy_ethtool_set_link_ksettings,
	.get_tunable = fs_get_tunable,
	.set_tunable = fs_set_tunable,
};

/**************************************************************************************/

#ifdef CONFIG_FS_ENET_HAS_FEC
#define IS_FEC(ops) ((ops) == &fs_fec_ops)
#else
#define IS_FEC(ops) 0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static const struct net_device_ops fs_enet_netdev_ops = {
	.ndo_open		= fs_enet_open,
	.ndo_stop		= fs_enet_close,
<<<<<<< HEAD
	.ndo_get_stats		= fs_enet_get_stats,
	.ndo_start_xmit		= fs_enet_start_xmit,
	.ndo_tx_timeout		= fs_timeout,
	.ndo_set_rx_mode	= fs_set_multicast_list,
	.ndo_do_ioctl		= fs_ioctl,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_change_mtu		= eth_change_mtu,
=======
	.ndo_start_xmit		= fs_enet_start_xmit,
	.ndo_tx_timeout		= fs_timeout,
	.ndo_set_rx_mode	= fs_set_multicast_list,
	.ndo_eth_ioctl		= phy_do_ioctl_running,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= eth_mac_addr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= fs_enet_netpoll,
#endif
};

<<<<<<< HEAD
static struct of_device_id fs_enet_match[];
static int __devinit fs_enet_probe(struct platform_device *ofdev)
{
	const struct of_device_id *match;
=======
static int fs_enet_probe(struct platform_device *ofdev)
{
	const struct fs_ops *ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *ndev;
	struct fs_enet_private *fep;
	struct fs_platform_info *fpi;
	const u32 *data;
<<<<<<< HEAD
	const u8 *mac_addr;
	const char *phy_connection_type;
	int privsize, len, ret = -ENODEV;

	match = of_match_device(fs_enet_match, &ofdev->dev);
	if (!match)
=======
	struct clk *clk;
	int err;
	const char *phy_connection_type;
	int privsize, len, ret = -ENODEV;

	ops = device_get_match_data(&ofdev->dev);
	if (!ops)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	fpi = kzalloc(sizeof(*fpi), GFP_KERNEL);
	if (!fpi)
		return -ENOMEM;

<<<<<<< HEAD
	if (!IS_FEC(match)) {
=======
	if (!IS_FEC(ops)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		data = of_get_property(ofdev->dev.of_node, "fsl,cpm-command", &len);
		if (!data || len != 4)
			goto out_free_fpi;

		fpi->cp_command = *data;
	}

<<<<<<< HEAD
	fpi->rx_ring = 32;
	fpi->tx_ring = 32;
	fpi->rx_copybreak = 240;
	fpi->use_napi = 1;
	fpi->napi_weight = 17;
	fpi->phy_node = of_parse_phandle(ofdev->dev.of_node, "phy-handle", 0);
	if ((!fpi->phy_node) && (!of_get_property(ofdev->dev.of_node, "fixed-link",
						  NULL)))
		goto out_free_fpi;
=======
	fpi->rx_ring = RX_RING_SIZE;
	fpi->tx_ring = TX_RING_SIZE;
	fpi->rx_copybreak = 240;
	fpi->napi_weight = 17;
	fpi->phy_node = of_parse_phandle(ofdev->dev.of_node, "phy-handle", 0);
	if (!fpi->phy_node && of_phy_is_fixed_link(ofdev->dev.of_node)) {
		err = of_phy_register_fixed_link(ofdev->dev.of_node);
		if (err)
			goto out_free_fpi;

		/* In the case of a fixed PHY, the DT node associated
		 * to the PHY is the Ethernet MAC DT node.
		 */
		fpi->phy_node = of_node_get(ofdev->dev.of_node);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (of_device_is_compatible(ofdev->dev.of_node, "fsl,mpc5125-fec")) {
		phy_connection_type = of_get_property(ofdev->dev.of_node,
						"phy-connection-type", NULL);
		if (phy_connection_type && !strcmp("rmii", phy_connection_type))
			fpi->use_rmii = 1;
	}

<<<<<<< HEAD
	privsize = sizeof(*fep) +
	           sizeof(struct sk_buff **) *
	           (fpi->rx_ring + fpi->tx_ring);
=======
	/* make clock lookup non-fatal (the driver is shared among platforms),
	 * but require enable to succeed when a clock was specified/found,
	 * keep a reference to the clock upon successful acquisition
	 */
	clk = devm_clk_get(&ofdev->dev, "per");
	if (!IS_ERR(clk)) {
		ret = clk_prepare_enable(clk);
		if (ret)
			goto out_deregister_fixed_link;

		fpi->clk_per = clk;
	}

	privsize = sizeof(*fep) +
	           sizeof(struct sk_buff **) *
		     (fpi->rx_ring + fpi->tx_ring) +
		   sizeof(char) * fpi->tx_ring;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ndev = alloc_etherdev(privsize);
	if (!ndev) {
		ret = -ENOMEM;
		goto out_put;
	}

	SET_NETDEV_DEV(ndev, &ofdev->dev);
<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, ndev);
=======
	platform_set_drvdata(ofdev, ndev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	fep = netdev_priv(ndev);
	fep->dev = &ofdev->dev;
	fep->ndev = ndev;
	fep->fpi = fpi;
<<<<<<< HEAD
	fep->ops = match->data;
=======
	fep->ops = ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = fep->ops->setup_data(ndev);
	if (ret)
		goto out_free_dev;

	fep->rx_skbuff = (struct sk_buff **)&fep[1];
	fep->tx_skbuff = fep->rx_skbuff + fpi->rx_ring;
<<<<<<< HEAD
=======
	fep->mapped_as_page = (char *)(fep->rx_skbuff + fpi->rx_ring +
				       fpi->tx_ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_init(&fep->lock);
	spin_lock_init(&fep->tx_lock);

<<<<<<< HEAD
	mac_addr = of_get_mac_address(ofdev->dev.of_node);
	if (mac_addr)
		memcpy(ndev->dev_addr, mac_addr, 6);
=======
	of_get_ethdev_address(ofdev->dev.of_node, ndev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = fep->ops->allocate_bd(ndev);
	if (ret)
		goto out_cleanup_data;

	fep->rx_bd_base = fep->ring_base;
	fep->tx_bd_base = fep->rx_bd_base + fpi->rx_ring;

	fep->tx_ring = fpi->tx_ring;
	fep->rx_ring = fpi->rx_ring;

	ndev->netdev_ops = &fs_enet_netdev_ops;
	ndev->watchdog_timeo = 2 * HZ;
<<<<<<< HEAD
	if (fpi->use_napi)
		netif_napi_add(ndev, &fep->napi, fs_enet_rx_napi,
		               fpi->napi_weight);

	ndev->ethtool_ops = &fs_ethtool_ops;

	init_timer(&fep->phy_timer_list);

	netif_carrier_off(ndev);

=======
	INIT_WORK(&fep->timeout_work, fs_timeout_work);
	netif_napi_add_weight(ndev, &fep->napi, fs_enet_napi,
			      fpi->napi_weight);

	ndev->ethtool_ops = &fs_ethtool_ops;

	netif_carrier_off(ndev);

	ndev->features |= NETIF_F_SG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = register_netdev(ndev);
	if (ret)
		goto out_free_bd;

	pr_info("%s: fs_enet: %pM\n", ndev->name, ndev->dev_addr);

	return 0;

out_free_bd:
	fep->ops->free_bd(ndev);
out_cleanup_data:
	fep->ops->cleanup_data(ndev);
out_free_dev:
	free_netdev(ndev);
<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, NULL);
out_put:
	of_node_put(fpi->phy_node);
=======
out_put:
	clk_disable_unprepare(fpi->clk_per);
out_deregister_fixed_link:
	of_node_put(fpi->phy_node);
	if (of_phy_is_fixed_link(ofdev->dev.of_node))
		of_phy_deregister_fixed_link(ofdev->dev.of_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_free_fpi:
	kfree(fpi);
	return ret;
}

<<<<<<< HEAD
static int fs_enet_remove(struct platform_device *ofdev)
{
	struct net_device *ndev = dev_get_drvdata(&ofdev->dev);
=======
static void fs_enet_remove(struct platform_device *ofdev)
{
	struct net_device *ndev = platform_get_drvdata(ofdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct fs_enet_private *fep = netdev_priv(ndev);

	unregister_netdev(ndev);

	fep->ops->free_bd(ndev);
	fep->ops->cleanup_data(ndev);
	dev_set_drvdata(fep->dev, NULL);
	of_node_put(fep->fpi->phy_node);
<<<<<<< HEAD
	free_netdev(ndev);
	return 0;
}

static struct of_device_id fs_enet_match[] = {
=======
	clk_disable_unprepare(fep->fpi->clk_per);
	if (of_phy_is_fixed_link(ofdev->dev.of_node))
		of_phy_deregister_fixed_link(ofdev->dev.of_node);
	free_netdev(ndev);
}

static const struct of_device_id fs_enet_match[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_FS_ENET_HAS_SCC
	{
		.compatible = "fsl,cpm1-scc-enet",
		.data = (void *)&fs_scc_ops,
	},
	{
		.compatible = "fsl,cpm2-scc-enet",
		.data = (void *)&fs_scc_ops,
	},
#endif
#ifdef CONFIG_FS_ENET_HAS_FCC
	{
		.compatible = "fsl,cpm2-fcc-enet",
		.data = (void *)&fs_fcc_ops,
	},
#endif
#ifdef CONFIG_FS_ENET_HAS_FEC
#ifdef CONFIG_FS_ENET_MPC5121_FEC
	{
		.compatible = "fsl,mpc5121-fec",
		.data = (void *)&fs_fec_ops,
	},
	{
		.compatible = "fsl,mpc5125-fec",
		.data = (void *)&fs_fec_ops,
	},
#else
	{
		.compatible = "fsl,pq1-fec-enet",
		.data = (void *)&fs_fec_ops,
	},
#endif
#endif
	{}
};
MODULE_DEVICE_TABLE(of, fs_enet_match);

static struct platform_driver fs_enet_driver = {
	.driver = {
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.name = "fs_enet",
		.of_match_table = fs_enet_match,
	},
	.probe = fs_enet_probe,
<<<<<<< HEAD
	.remove = fs_enet_remove,
=======
	.remove_new = fs_enet_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#ifdef CONFIG_NET_POLL_CONTROLLER
static void fs_enet_netpoll(struct net_device *dev)
{
       disable_irq(dev->irq);
       fs_enet_interrupt(dev->irq, dev);
       enable_irq(dev->irq);
}
#endif

module_platform_driver(fs_enet_driver);
