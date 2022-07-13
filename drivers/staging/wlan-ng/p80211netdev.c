<<<<<<< HEAD
/* src/p80211/p80211knetdev.c
*
* Linux Kernel net device interface
*
* Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
* --------------------------------------------------------------------
*
* linux-wlan
*
*   The contents of this file are subject to the Mozilla Public
*   License Version 1.1 (the "License"); you may not use this file
*   except in compliance with the License. You may obtain a copy of
*   the License at http://www.mozilla.org/MPL/
*
*   Software distributed under the License is distributed on an "AS
*   IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*   implied. See the License for the specific language governing
*   rights and limitations under the License.
*
*   Alternatively, the contents of this file may be used under the
*   terms of the GNU Public License version 2 (the "GPL"), in which
*   case the provisions of the GPL are applicable instead of the
*   above.  If you wish to allow the use of your version of this file
*   only under the terms of the GPL and not to allow others to use
*   your version of this file under the MPL, indicate your decision
*   by deleting the provisions above and replace them with the notice
*   and other provisions required by the GPL.  If you do not delete
*   the provisions above, a recipient may use your version of this
*   file under either the MPL or the GPL.
*
* --------------------------------------------------------------------
*
* Inquiries regarding the linux-wlan Open Source project can be
* made directly to:
*
* AbsoluteValue Systems Inc.
* info@linux-wlan.com
* http://www.linux-wlan.com
*
* --------------------------------------------------------------------
*
* Portions of the development of this software were funded by
* Intersil Corporation as part of PRISM(R) chipset product development.
*
* --------------------------------------------------------------------
*
* The functions required for a Linux network device are defined here.
*
* --------------------------------------------------------------------
*/
=======
// SPDX-License-Identifier: (GPL-2.0 OR MPL-1.1)
/*
 *
 * Linux Kernel net device interface
 *
 * Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
 * --------------------------------------------------------------------
 *
 * linux-wlan
 *
 * --------------------------------------------------------------------
 *
 * Inquiries regarding the linux-wlan Open Source project can be
 * made directly to:
 *
 * AbsoluteValue Systems Inc.
 * info@linux-wlan.com
 * http://www.linux-wlan.com
 *
 * --------------------------------------------------------------------
 *
 * Portions of the development of this software were funded by
 * Intersil Corporation as part of PRISM(R) chipset product development.
 *
 * --------------------------------------------------------------------
 *
 * The functions required for a Linux network device are defined here.
 *
 * --------------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/kmod.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <linux/sockios.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/byteorder/generic.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <asm/byteorder.h>

#ifdef SIOCETHTOOL
#include <linux/ethtool.h>
#endif

#include <net/iw_handler.h>
#include <net/net_namespace.h>
#include <net/cfg80211.h>

#include "p80211types.h"
#include "p80211hdr.h"
#include "p80211conv.h"
#include "p80211mgmt.h"
#include "p80211msg.h"
#include "p80211netdev.h"
#include "p80211ioctl.h"
#include "p80211req.h"
#include "p80211metastruct.h"
#include "p80211metadef.h"

#include "cfg80211.c"

<<<<<<< HEAD
/* Support functions */
static void p80211netdev_rx_bh(unsigned long arg);

/* netdevice method functions */
static int p80211knetdev_init(netdevice_t *netdev);
static struct net_device_stats *p80211knetdev_get_stats(netdevice_t *netdev);
static int p80211knetdev_open(netdevice_t *netdev);
static int p80211knetdev_stop(netdevice_t *netdev);
static int p80211knetdev_hard_start_xmit(struct sk_buff *skb,
					 netdevice_t *netdev);
static void p80211knetdev_set_multicast_list(netdevice_t *dev);
static int p80211knetdev_do_ioctl(netdevice_t *dev, struct ifreq *ifr,
				  int cmd);
static int p80211knetdev_set_mac_address(netdevice_t *dev, void *addr);
static void p80211knetdev_tx_timeout(netdevice_t *netdev);
static int p80211_rx_typedrop(wlandevice_t *wlandev, u16 fc);
=======
/* netdevice method functions */
static int p80211knetdev_init(struct net_device *netdev);
static int p80211knetdev_open(struct net_device *netdev);
static int p80211knetdev_stop(struct net_device *netdev);
static netdev_tx_t p80211knetdev_hard_start_xmit(struct sk_buff *skb,
						 struct net_device *netdev);
static void p80211knetdev_set_multicast_list(struct net_device *dev);
static int p80211knetdev_siocdevprivate(struct net_device *dev, struct ifreq *ifr,
					void __user *data, int cmd);
static int p80211knetdev_set_mac_address(struct net_device *dev, void *addr);
static void p80211knetdev_tx_timeout(struct net_device *netdev, unsigned int txqueue);
static int p80211_rx_typedrop(struct wlandevice *wlandev, u16 fc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int wlan_watchdog = 5000;
module_param(wlan_watchdog, int, 0644);
MODULE_PARM_DESC(wlan_watchdog, "transmit timeout in milliseconds");

int wlan_wext_write = 1;
module_param(wlan_wext_write, int, 0644);
MODULE_PARM_DESC(wlan_wext_write, "enable write wireless extensions");

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_init
*
* Init method for a Linux netdevice.  Called in response to
* register_netdev.
*
* Arguments:
*	none
*
* Returns:
*	nothing
----------------------------------------------------------------*/
static int p80211knetdev_init(netdevice_t *netdev)
=======
 * p80211knetdev_init
 *
 * Init method for a Linux netdevice.  Called in response to
 * register_netdev.
 *
 * Arguments:
 *	none
 *
 * Returns:
 *	nothing
 *----------------------------------------------------------------
 */
static int p80211knetdev_init(struct net_device *netdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Called in response to register_netdev */
	/* This is usually the probe function, but the probe has */
	/* already been done by the MSD and the create_kdev */
	/* function.  All we do here is return success */
	return 0;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_get_stats
*
* Statistics retrieval for linux netdevices.  Here we're reporting
* the Linux i/f level statistics.  Hence, for the primary numbers,
* we don't want to report the numbers from the MIB.  Eventually,
* it might be useful to collect some of the error counters though.
*
* Arguments:
*	netdev		Linux netdevice
*
* Returns:
*	the address of the statistics structure
----------------------------------------------------------------*/
static struct net_device_stats *p80211knetdev_get_stats(netdevice_t *netdev)
{
	wlandevice_t *wlandev = netdev->ml_priv;

	/* TODO: review the MIB stats for items that correspond to
	   linux stats */

	return &(wlandev->linux_stats);
}

/*----------------------------------------------------------------
* p80211knetdev_open
*
* Linux netdevice open method.  Following a successful call here,
* the device is supposed to be ready for tx and rx.  In our
* situation that may not be entirely true due to the state of the
* MAC below.
*
* Arguments:
*	netdev		Linux network device structure
*
* Returns:
*	zero on success, non-zero otherwise
----------------------------------------------------------------*/
static int p80211knetdev_open(netdevice_t *netdev)
{
	int result = 0;		/* success */
	wlandevice_t *wlandev = netdev->ml_priv;
=======
 * p80211knetdev_open
 *
 * Linux netdevice open method.  Following a successful call here,
 * the device is supposed to be ready for tx and rx.  In our
 * situation that may not be entirely true due to the state of the
 * MAC below.
 *
 * Arguments:
 *	netdev		Linux network device structure
 *
 * Returns:
 *	zero on success, non-zero otherwise
 *----------------------------------------------------------------
 */
static int p80211knetdev_open(struct net_device *netdev)
{
	int result = 0;		/* success */
	struct wlandevice *wlandev = netdev->ml_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check to make sure the MSD is running */
	if (wlandev->msdstate != WLAN_MSD_RUNNING)
		return -ENODEV;

	/* Tell the MSD to open */
<<<<<<< HEAD
	if (wlandev->open != NULL) {
=======
	if (wlandev->open) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = wlandev->open(wlandev);
		if (result == 0) {
			netif_start_queue(wlandev->netdev);
			wlandev->state = WLAN_DEVICE_OPEN;
		}
	} else {
		result = -EAGAIN;
	}

	return result;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_stop
*
* Linux netdevice stop (close) method.  Following this call,
* no frames should go up or down through this interface.
*
* Arguments:
*	netdev		Linux network device structure
*
* Returns:
*	zero on success, non-zero otherwise
----------------------------------------------------------------*/
static int p80211knetdev_stop(netdevice_t *netdev)
{
	int result = 0;
	wlandevice_t *wlandev = netdev->ml_priv;

	if (wlandev->close != NULL)
=======
 * p80211knetdev_stop
 *
 * Linux netdevice stop (close) method.  Following this call,
 * no frames should go up or down through this interface.
 *
 * Arguments:
 *	netdev		Linux network device structure
 *
 * Returns:
 *	zero on success, non-zero otherwise
 *----------------------------------------------------------------
 */
static int p80211knetdev_stop(struct net_device *netdev)
{
	int result = 0;
	struct wlandevice *wlandev = netdev->ml_priv;

	if (wlandev->close)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = wlandev->close(wlandev);

	netif_stop_queue(wlandev->netdev);
	wlandev->state = WLAN_DEVICE_CLOSED;

	return result;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211netdev_rx
*
* Frame receive function called by the mac specific driver.
*
* Arguments:
*	wlandev		WLAN network device structure
*	skb		skbuff containing a full 802.11 frame.
* Returns:
*	nothing
* Side effects:
*
----------------------------------------------------------------*/
void p80211netdev_rx(wlandevice_t *wlandev, struct sk_buff *skb)
{
	/* Enqueue for post-irq processing */
	skb_queue_tail(&wlandev->nsd_rxq, skb);

	tasklet_schedule(&wlandev->rx_bh);

	return;
}

/*----------------------------------------------------------------
* p80211netdev_rx_bh
*
* Deferred processing of all received frames.
*
* Arguments:
*	wlandev		WLAN network device structure
*	skb		skbuff containing a full 802.11 frame.
* Returns:
*	nothing
* Side effects:
*
----------------------------------------------------------------*/
static void p80211netdev_rx_bh(unsigned long arg)
{
	wlandevice_t *wlandev = (wlandevice_t *) arg;
	struct sk_buff *skb = NULL;
	netdevice_t *dev = wlandev->netdev;
	struct p80211_hdr_a3 *hdr;
	u16 fc;

	/* Let's empty our our queue */
	while ((skb = skb_dequeue(&wlandev->nsd_rxq))) {
		if (wlandev->state == WLAN_DEVICE_OPEN) {

=======
 * p80211netdev_rx
 *
 * Frame receive function called by the mac specific driver.
 *
 * Arguments:
 *	wlandev		WLAN network device structure
 *	skb		skbuff containing a full 802.11 frame.
 * Returns:
 *	nothing
 * Side effects:
 *
 *----------------------------------------------------------------
 */
void p80211netdev_rx(struct wlandevice *wlandev, struct sk_buff *skb)
{
	/* Enqueue for post-irq processing */
	skb_queue_tail(&wlandev->nsd_rxq, skb);
	tasklet_schedule(&wlandev->rx_bh);
}

#define CONV_TO_ETHER_SKIPPED	0x01
#define CONV_TO_ETHER_FAILED	0x02

/**
 * p80211_convert_to_ether - conversion from 802.11 frame to ethernet frame
 * @wlandev: pointer to WLAN device
 * @skb: pointer to socket buffer
 *
 * Returns: 0 if conversion succeeded
 *	    CONV_TO_ETHER_FAILED if conversion failed
 *	    CONV_TO_ETHER_SKIPPED if frame is ignored
 */
static int p80211_convert_to_ether(struct wlandevice *wlandev,
				   struct sk_buff *skb)
{
	struct p80211_hdr *hdr;

	hdr = (struct p80211_hdr *)skb->data;
	if (p80211_rx_typedrop(wlandev, le16_to_cpu(hdr->frame_control)))
		return CONV_TO_ETHER_SKIPPED;

	/* perform mcast filtering: allow my local address through but reject
	 * anything else that isn't multicast
	 */
	if (wlandev->netdev->flags & IFF_ALLMULTI) {
		if (!ether_addr_equal_unaligned(wlandev->netdev->dev_addr,
						hdr->address1)) {
			if (!is_multicast_ether_addr(hdr->address1))
				return CONV_TO_ETHER_SKIPPED;
		}
	}

	if (skb_p80211_to_ether(wlandev, wlandev->ethconv, skb) == 0) {
		wlandev->netdev->stats.rx_packets++;
		wlandev->netdev->stats.rx_bytes += skb->len;
		netif_rx(skb);
		return 0;
	}

	netdev_dbg(wlandev->netdev, "%s failed.\n", __func__);
	return CONV_TO_ETHER_FAILED;
}

/**
 * p80211netdev_rx_bh - deferred processing of all received frames
 *
 * @t: pointer to the tasklet associated with this handler
 */
static void p80211netdev_rx_bh(struct tasklet_struct *t)
{
	struct wlandevice *wlandev = from_tasklet(wlandev, t, rx_bh);
	struct sk_buff *skb = NULL;
	struct net_device *dev = wlandev->netdev;

	/* Let's empty our queue */
	while ((skb = skb_dequeue(&wlandev->nsd_rxq))) {
		if (wlandev->state == WLAN_DEVICE_OPEN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (dev->type != ARPHRD_ETHER) {
				/* RAW frame; we shouldn't convert it */
				/* XXX Append the Prism Header here instead. */

				/* set up various data fields */
				skb->dev = dev;
				skb_reset_mac_header(skb);
				skb->ip_summed = CHECKSUM_NONE;
				skb->pkt_type = PACKET_OTHERHOST;
				skb->protocol = htons(ETH_P_80211_RAW);
<<<<<<< HEAD
				dev->last_rx = jiffies;

				wlandev->linux_stats.rx_packets++;
				wlandev->linux_stats.rx_bytes += skb->len;
				netif_rx_ni(skb);
				continue;
			} else {
				hdr = (struct p80211_hdr_a3 *) skb->data;
				fc = le16_to_cpu(hdr->fc);
				if (p80211_rx_typedrop(wlandev, fc)) {
					dev_kfree_skb(skb);
					continue;
				}

				/* perform mcast filtering */
				if (wlandev->netdev->flags & IFF_ALLMULTI) {
					/* allow my local address through */
					if (memcmp
					    (hdr->a1, wlandev->netdev->dev_addr,
					     ETH_ALEN) != 0) {
						/* but reject anything else that
						   isn't multicast */
						if (!(hdr->a1[0] & 0x01)) {
							dev_kfree_skb(skb);
							continue;
						}
					}
				}

				if (skb_p80211_to_ether
				    (wlandev, wlandev->ethconv, skb) == 0) {
					skb->dev->last_rx = jiffies;
					wlandev->linux_stats.rx_packets++;
					wlandev->linux_stats.rx_bytes +=
					    skb->len;
					netif_rx_ni(skb);
					continue;
				}
				pr_debug("p80211_to_ether failed.\n");
=======

				dev->stats.rx_packets++;
				dev->stats.rx_bytes += skb->len;
				netif_rx(skb);
				continue;
			} else {
				if (!p80211_convert_to_ether(wlandev, skb))
					continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
		dev_kfree_skb(skb);
	}
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_hard_start_xmit
*
* Linux netdevice method for transmitting a frame.
*
* Arguments:
*	skb	Linux sk_buff containing the frame.
*	netdev	Linux netdevice.
*
* Side effects:
*	If the lower layers report that buffers are full. netdev->tbusy
*	will be set to prevent higher layers from sending more traffic.
*
*	Note: If this function returns non-zero, higher layers retain
*	      ownership of the skb.
*
* Returns:
*	zero on success, non-zero on failure.
----------------------------------------------------------------*/
static int p80211knetdev_hard_start_xmit(struct sk_buff *skb,
					 netdevice_t *netdev)
{
	int result = 0;
	int txresult = -1;
	wlandevice_t *wlandev = netdev->ml_priv;
	union p80211_hdr p80211_hdr;
	struct p80211_metawep p80211_wep;

	if (skb == NULL)
=======
 * p80211knetdev_hard_start_xmit
 *
 * Linux netdevice method for transmitting a frame.
 *
 * Arguments:
 *	skb	Linux sk_buff containing the frame.
 *	netdev	Linux netdevice.
 *
 * Side effects:
 *	If the lower layers report that buffers are full. netdev->tbusy
 *	will be set to prevent higher layers from sending more traffic.
 *
 *	Note: If this function returns non-zero, higher layers retain
 *	      ownership of the skb.
 *
 * Returns:
 *	zero on success, non-zero on failure.
 *----------------------------------------------------------------
 */
static netdev_tx_t p80211knetdev_hard_start_xmit(struct sk_buff *skb,
						 struct net_device *netdev)
{
	int result = 0;
	int txresult;
	struct wlandevice *wlandev = netdev->ml_priv;
	struct p80211_hdr p80211_hdr;
	struct p80211_metawep p80211_wep;

	p80211_wep.data = NULL;

	if (!skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NETDEV_TX_OK;

	if (wlandev->state != WLAN_DEVICE_OPEN) {
		result = 1;
		goto failed;
	}

<<<<<<< HEAD
	memset(&p80211_hdr, 0, sizeof(union p80211_hdr));
	memset(&p80211_wep, 0, sizeof(struct p80211_metawep));

	if (netif_queue_stopped(netdev)) {
		pr_debug("called when queue stopped.\n");
=======
	memset(&p80211_hdr, 0, sizeof(p80211_hdr));
	memset(&p80211_wep, 0, sizeof(p80211_wep));

	if (netif_queue_stopped(netdev)) {
		netdev_dbg(netdev, "called when queue stopped.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = 1;
		goto failed;
	}

	netif_stop_queue(netdev);

	/* Check to see that a valid mode is set */
	switch (wlandev->macmode) {
	case WLAN_MACMODE_IBSS_STA:
	case WLAN_MACMODE_ESS_STA:
	case WLAN_MACMODE_ESS_AP:
		break;
	default:
		/* Mode isn't set yet, just drop the frame
		 * and return success .
		 * TODO: we need a saner way to handle this
		 */
<<<<<<< HEAD
		if (skb->protocol != ETH_P_80211_RAW) {
			netif_start_queue(wlandev->netdev);
			printk(KERN_NOTICE
			       "Tx attempt prior to association, frame dropped.\n");
			wlandev->linux_stats.tx_dropped++;
=======
		if (be16_to_cpu(skb->protocol) != ETH_P_80211_RAW) {
			netif_start_queue(wlandev->netdev);
			netdev_notice(netdev, "Tx attempt prior to association, frame dropped.\n");
			netdev->stats.tx_dropped++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			result = 0;
			goto failed;
		}
		break;
	}

	/* Check for raw transmits */
<<<<<<< HEAD
	if (skb->protocol == ETH_P_80211_RAW) {
=======
	if (be16_to_cpu(skb->protocol) == ETH_P_80211_RAW) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!capable(CAP_NET_ADMIN)) {
			result = 1;
			goto failed;
		}
		/* move the header over */
<<<<<<< HEAD
		memcpy(&p80211_hdr, skb->data, sizeof(union p80211_hdr));
		skb_pull(skb, sizeof(union p80211_hdr));
=======
		memcpy(&p80211_hdr, skb->data, sizeof(p80211_hdr));
		skb_pull(skb, sizeof(p80211_hdr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (skb_ether_to_p80211
		    (wlandev, wlandev->ethconv, skb, &p80211_hdr,
		     &p80211_wep) != 0) {
			/* convert failed */
<<<<<<< HEAD
			pr_debug("ether_to_80211(%d) failed.\n",
				 wlandev->ethconv);
=======
			netdev_dbg(netdev, "ether_to_80211(%d) failed.\n",
				   wlandev->ethconv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			result = 1;
			goto failed;
		}
	}
<<<<<<< HEAD
	if (wlandev->txframe == NULL) {
=======
	if (!wlandev->txframe) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = 1;
		goto failed;
	}

<<<<<<< HEAD
	netdev->trans_start = jiffies;

	wlandev->linux_stats.tx_packets++;
	/* count only the packet payload */
	wlandev->linux_stats.tx_bytes += skb->len;
=======
	netif_trans_update(netdev);

	netdev->stats.tx_packets++;
	/* count only the packet payload */
	netdev->stats.tx_bytes += skb->len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	txresult = wlandev->txframe(wlandev, skb, &p80211_hdr, &p80211_wep);

	if (txresult == 0) {
		/* success and more buf */
		/* avail, re: hw_txdata */
		netif_wake_queue(wlandev->netdev);
		result = NETDEV_TX_OK;
	} else if (txresult == 1) {
		/* success, no more avail */
<<<<<<< HEAD
		pr_debug("txframe success, no more bufs\n");
=======
		netdev_dbg(netdev, "txframe success, no more bufs\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* netdev->tbusy = 1;  don't set here, irqhdlr */
		/*   may have already cleared it */
		result = NETDEV_TX_OK;
	} else if (txresult == 2) {
		/* alloc failure, drop frame */
<<<<<<< HEAD
		pr_debug("txframe returned alloc_fail\n");
		result = NETDEV_TX_BUSY;
	} else {
		/* buffer full or queue busy, drop frame. */
		pr_debug("txframe returned full or busy\n");
=======
		netdev_dbg(netdev, "txframe returned alloc_fail\n");
		result = NETDEV_TX_BUSY;
	} else {
		/* buffer full or queue busy, drop frame. */
		netdev_dbg(netdev, "txframe returned full or busy\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = NETDEV_TX_BUSY;
	}

failed:
	/* Free up the WEP buffer if it's not the same as the skb */
	if ((p80211_wep.data) && (p80211_wep.data != skb->data))
<<<<<<< HEAD
		kzfree(p80211_wep.data);
=======
		kfree_sensitive(p80211_wep.data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* we always free the skb here, never in a lower level. */
	if (!result)
		dev_kfree_skb(skb);

	return result;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_set_multicast_list
*
* Called from higher lavers whenever there's a need to set/clear
* promiscuous mode or rewrite the multicast list.
*
* Arguments:
*	none
*
* Returns:
*	nothing
----------------------------------------------------------------*/
static void p80211knetdev_set_multicast_list(netdevice_t *dev)
{
	wlandevice_t *wlandev = dev->ml_priv;
=======
 * p80211knetdev_set_multicast_list
 *
 * Called from higher layers whenever there's a need to set/clear
 * promiscuous mode or rewrite the multicast list.
 *
 * Arguments:
 *	none
 *
 * Returns:
 *	nothing
 *----------------------------------------------------------------
 */
static void p80211knetdev_set_multicast_list(struct net_device *dev)
{
	struct wlandevice *wlandev = dev->ml_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* TODO:  real multicast support as well */

	if (wlandev->set_multicast_list)
		wlandev->set_multicast_list(wlandev, dev);
<<<<<<< HEAD

}

#ifdef SIOCETHTOOL

static int p80211netdev_ethtool(wlandevice_t *wlandev, void __user *useraddr)
{
	u32 ethcmd;
	struct ethtool_drvinfo info;
	struct ethtool_value edata;

	memset(&info, 0, sizeof(info));
	memset(&edata, 0, sizeof(edata));

	if (copy_from_user(&ethcmd, useraddr, sizeof(ethcmd)))
		return -EFAULT;

	switch (ethcmd) {
	case ETHTOOL_GDRVINFO:
		info.cmd = ethcmd;
		snprintf(info.driver, sizeof(info.driver), "p80211_%s",
			 wlandev->nsdname);
		snprintf(info.version, sizeof(info.version), "%s",
			 WLAN_RELEASE);

		if (copy_to_user(useraddr, &info, sizeof(info)))
			return -EFAULT;
		return 0;
#ifdef ETHTOOL_GLINK
	case ETHTOOL_GLINK:
		edata.cmd = ethcmd;

		if (wlandev->linkstatus &&
		    (wlandev->macmode != WLAN_MACMODE_NONE)) {
			edata.data = 1;
		} else {
			edata.data = 0;
		}

		if (copy_to_user(useraddr, &edata, sizeof(edata)))
			return -EFAULT;
		return 0;
#endif
	}

	return -EOPNOTSUPP;
}

#endif

/*----------------------------------------------------------------
* p80211knetdev_do_ioctl
*
* Handle an ioctl call on one of our devices.  Everything Linux
* ioctl specific is done here.  Then we pass the contents of the
* ifr->data to the request message handler.
*
* Arguments:
*	dev	Linux kernel netdevice
*	ifr	Our private ioctl request structure, typed for the
*		generic struct ifreq so we can use ptr to func
*		w/o cast.
*
* Returns:
*	zero on success, a negative errno on failure.  Possible values:
*		-ENETDOWN Device isn't up.
*		-EBUSY	cmd already in progress
*		-ETIME	p80211 cmd timed out (MSD may have its own timers)
*		-EFAULT memory fault copying msg from user buffer
*		-ENOMEM unable to allocate kernel msg buffer
*		-ENOSYS	bad magic, it the cmd really for us?
*		-EintR	sleeping on cmd, awakened by signal, cmd cancelled.
*
* Call Context:
*	Process thread (ioctl caller).  TODO: SMP support may require
*	locks.
----------------------------------------------------------------*/
static int p80211knetdev_do_ioctl(netdevice_t *dev, struct ifreq *ifr, int cmd)
{
	int result = 0;
	struct p80211ioctl_req *req = (struct p80211ioctl_req *) ifr;
	wlandevice_t *wlandev = dev->ml_priv;
	u8 *msgbuf;

	pr_debug("rx'd ioctl, cmd=%d, len=%d\n", cmd, req->len);

#ifdef SIOCETHTOOL
	if (cmd == SIOCETHTOOL) {
		result =
		    p80211netdev_ethtool(wlandev, (void __user *)ifr->ifr_data);
		goto bail;
	}
#endif

	/* Test the magic, assume ifr is good if it's there */
	if (req->magic != P80211_IOCTL_MAGIC) {
		result = -ENOSYS;
=======
}

/*----------------------------------------------------------------
 * p80211knetdev_siocdevprivate
 *
 * Handle an ioctl call on one of our devices.  Everything Linux
 * ioctl specific is done here.  Then we pass the contents of the
 * ifr->data to the request message handler.
 *
 * Arguments:
 *	dev	Linux kernel netdevice
 *	ifr	Our private ioctl request structure, typed for the
 *		generic struct ifreq so we can use ptr to func
 *		w/o cast.
 *
 * Returns:
 *	zero on success, a negative errno on failure.  Possible values:
 *		-ENETDOWN Device isn't up.
 *		-EBUSY	cmd already in progress
 *		-ETIME	p80211 cmd timed out (MSD may have its own timers)
 *		-EFAULT memory fault copying msg from user buffer
 *		-ENOMEM unable to allocate kernel msg buffer
 *		-EINVAL	bad magic, it the cmd really for us?
 *		-EintR	sleeping on cmd, awakened by signal, cmd cancelled.
 *
 * Call Context:
 *	Process thread (ioctl caller).  TODO: SMP support may require
 *	locks.
 *----------------------------------------------------------------
 */
static int p80211knetdev_siocdevprivate(struct net_device *dev,
					struct ifreq *ifr,
					void __user *data, int cmd)
{
	int result = 0;
	struct p80211ioctl_req *req = (struct p80211ioctl_req *)ifr;
	struct wlandevice *wlandev = dev->ml_priv;
	u8 *msgbuf;

	netdev_dbg(dev, "rx'd ioctl, cmd=%d, len=%d\n", cmd, req->len);

	if (in_compat_syscall())
		return -EOPNOTSUPP;

	/* Test the magic, assume ifr is good if it's there */
	if (req->magic != P80211_IOCTL_MAGIC) {
		result = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto bail;
	}

	if (cmd == P80211_IFTEST) {
		result = 0;
		goto bail;
	} else if (cmd != P80211_IFREQ) {
<<<<<<< HEAD
		result = -ENOSYS;
		goto bail;
	}

	/* Allocate a buf of size req->len */
	msgbuf = kmalloc(req->len, GFP_KERNEL);
	if (msgbuf) {
		if (copy_from_user(msgbuf, (void __user *)req->data, req->len))
			result = -EFAULT;
		else
			result = p80211req_dorequest(wlandev, msgbuf);

		if (result == 0) {
			if (copy_to_user
			    ((void __user *)req->data, msgbuf, req->len)) {
				result = -EFAULT;
			}
		}
		kfree(msgbuf);
	} else {
		result = -ENOMEM;
	}
=======
		result = -EINVAL;
		goto bail;
	}

	msgbuf = memdup_user(data, req->len);
	if (IS_ERR(msgbuf)) {
		result = PTR_ERR(msgbuf);
		goto bail;
	}

	result = p80211req_dorequest(wlandev, msgbuf);

	if (result == 0) {
		if (copy_to_user(data, msgbuf, req->len))
			result = -EFAULT;
	}
	kfree(msgbuf);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
bail:
	/* If allocate,copyfrom or copyto fails, return errno */
	return result;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211knetdev_set_mac_address
*
* Handles the ioctl for changing the MACAddress of a netdevice
*
* references: linux/netdevice.h and drivers/net/net_init.c
*
* NOTE: [MSM] We only prevent address changes when the netdev is
* up.  We don't control anything based on dot11 state.  If the
* address is changed on a STA that's currently associated, you
* will probably lose the ability to send and receive data frames.
* Just be aware.  Therefore, this should usually only be done
* prior to scan/join/auth/assoc.
*
* Arguments:
*	dev	netdevice struct
*	addr	the new MACAddress (a struct)
*
* Returns:
*	zero on success, a negative errno on failure.  Possible values:
*		-EBUSY	device is bussy (cmd not possible)
*		-and errors returned by: p80211req_dorequest(..)
*
* by: Collin R. Mulliner <collin@mulliner.org>
----------------------------------------------------------------*/
static int p80211knetdev_set_mac_address(netdevice_t *dev, void *addr)
{
	struct sockaddr *new_addr = addr;
	struct p80211msg_dot11req_mibset dot11req;
	p80211item_unk392_t *mibattr;
	p80211item_pstr6_t *macaddr;
	p80211item_uint32_t *resultcode;
	int result = 0;
=======
 * p80211knetdev_set_mac_address
 *
 * Handles the ioctl for changing the MACAddress of a netdevice
 *
 * references: linux/netdevice.h and drivers/net/net_init.c
 *
 * NOTE: [MSM] We only prevent address changes when the netdev is
 * up.  We don't control anything based on dot11 state.  If the
 * address is changed on a STA that's currently associated, you
 * will probably lose the ability to send and receive data frames.
 * Just be aware.  Therefore, this should usually only be done
 * prior to scan/join/auth/assoc.
 *
 * Arguments:
 *	dev	netdevice struct
 *	addr	the new MACAddress (a struct)
 *
 * Returns:
 *	zero on success, a negative errno on failure.  Possible values:
 *		-EBUSY	device is bussy (cmd not possible)
 *		-and errors returned by: p80211req_dorequest(..)
 *
 * by: Collin R. Mulliner <collin@mulliner.org>
 *----------------------------------------------------------------
 */
static int p80211knetdev_set_mac_address(struct net_device *dev, void *addr)
{
	struct sockaddr *new_addr = addr;
	struct p80211msg_dot11req_mibset dot11req;
	struct p80211item_unk392 *mibattr;
	struct p80211item_pstr6 *macaddr;
	struct p80211item_uint32 *resultcode;
	int result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we're running, we don't allow MAC address changes */
	if (netif_running(dev))
		return -EBUSY;

	/* Set up some convenience pointers. */
	mibattr = &dot11req.mibattribute;
<<<<<<< HEAD
	macaddr = (p80211item_pstr6_t *) &mibattr->data;
	resultcode = &dot11req.resultcode;

	/* Set up a dot11req_mibset */
	memset(&dot11req, 0, sizeof(struct p80211msg_dot11req_mibset));
	dot11req.msgcode = DIDmsg_dot11req_mibset;
	dot11req.msglen = sizeof(struct p80211msg_dot11req_mibset);
	memcpy(dot11req.devname,
	       ((wlandevice_t *) dev->ml_priv)->name, WLAN_DEVNAMELEN_MAX - 1);

	/* Set up the mibattribute argument */
	mibattr->did = DIDmsg_dot11req_mibset_mibattribute;
	mibattr->status = P80211ENUM_msgitem_status_data_ok;
	mibattr->len = sizeof(mibattr->data);

	macaddr->did = DIDmib_dot11mac_dot11OperationTable_dot11MACAddress;
=======
	macaddr = (struct p80211item_pstr6 *)&mibattr->data;
	resultcode = &dot11req.resultcode;

	/* Set up a dot11req_mibset */
	memset(&dot11req, 0, sizeof(dot11req));
	dot11req.msgcode = DIDMSG_DOT11REQ_MIBSET;
	dot11req.msglen = sizeof(dot11req);
	memcpy(dot11req.devname,
	       ((struct wlandevice *)dev->ml_priv)->name,
	       WLAN_DEVNAMELEN_MAX - 1);

	/* Set up the mibattribute argument */
	mibattr->did = DIDMSG_DOT11REQ_MIBSET_MIBATTRIBUTE;
	mibattr->status = P80211ENUM_msgitem_status_data_ok;
	mibattr->len = sizeof(mibattr->data);

	macaddr->did = DIDMIB_DOT11MAC_OPERATIONTABLE_MACADDRESS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	macaddr->status = P80211ENUM_msgitem_status_data_ok;
	macaddr->len = sizeof(macaddr->data);
	macaddr->data.len = ETH_ALEN;
	memcpy(&macaddr->data.data, new_addr->sa_data, ETH_ALEN);

	/* Set up the resultcode argument */
<<<<<<< HEAD
	resultcode->did = DIDmsg_dot11req_mibset_resultcode;
=======
	resultcode->did = DIDMSG_DOT11REQ_MIBSET_RESULTCODE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	resultcode->status = P80211ENUM_msgitem_status_no_value;
	resultcode->len = sizeof(resultcode->data);
	resultcode->data = 0;

	/* now fire the request */
<<<<<<< HEAD
	result = p80211req_dorequest(dev->ml_priv, (u8 *) &dot11req);
=======
	result = p80211req_dorequest(dev->ml_priv, (u8 *)&dot11req);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If the request wasn't successful, report an error and don't
	 * change the netdev address
	 */
	if (result != 0 || resultcode->data != P80211ENUM_resultcode_success) {
<<<<<<< HEAD
		printk(KERN_ERR
		       "Low-level driver failed dot11req_mibset(dot11MACAddress).\n");
		result = -EADDRNOTAVAIL;
	} else {
		/* everything's ok, change the addr in netdev */
		memcpy(dev->dev_addr, new_addr->sa_data, dev->addr_len);
=======
		netdev_err(dev, "Low-level driver failed dot11req_mibset(dot11MACAddress).\n");
		result = -EADDRNOTAVAIL;
	} else {
		/* everything's ok, change the addr in netdev */
		eth_hw_addr_set(dev, new_addr->sa_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return result;
}

<<<<<<< HEAD
static int wlan_change_mtu(netdevice_t *dev, int new_mtu)
{
	/* 2312 is max 802.11 payload, 20 is overhead, (ether + llc +snap)
	   and another 8 for wep. */
	if ((new_mtu < 68) || (new_mtu > (2312 - 20 - 8)))
		return -EINVAL;

	dev->mtu = new_mtu;

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct net_device_ops p80211_netdev_ops = {
	.ndo_init = p80211knetdev_init,
	.ndo_open = p80211knetdev_open,
	.ndo_stop = p80211knetdev_stop,
<<<<<<< HEAD
	.ndo_get_stats = p80211knetdev_get_stats,
	.ndo_start_xmit = p80211knetdev_hard_start_xmit,
	.ndo_set_rx_mode = p80211knetdev_set_multicast_list,
	.ndo_do_ioctl = p80211knetdev_do_ioctl,
	.ndo_set_mac_address = p80211knetdev_set_mac_address,
	.ndo_tx_timeout = p80211knetdev_tx_timeout,
	.ndo_change_mtu = wlan_change_mtu,
=======
	.ndo_start_xmit = p80211knetdev_hard_start_xmit,
	.ndo_set_rx_mode = p80211knetdev_set_multicast_list,
	.ndo_siocdevprivate = p80211knetdev_siocdevprivate,
	.ndo_set_mac_address = p80211knetdev_set_mac_address,
	.ndo_tx_timeout = p80211knetdev_tx_timeout,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_validate_addr = eth_validate_addr,
};

/*----------------------------------------------------------------
<<<<<<< HEAD
* wlan_setup
*
* Roughly matches the functionality of ether_setup.  Here
* we set up any members of the wlandevice structure that are common
* to all devices.  Additionally, we allocate a linux 'struct device'
* and perform the same setup as ether_setup.
*
* Note: It's important that the caller have setup the wlandev->name
*	ptr prior to calling this function.
*
* Arguments:
*	wlandev		ptr to the wlandev structure for the
*			interface.
*	physdev		ptr to usb device
* Returns:
*	zero on success, non-zero otherwise.
* Call Context:
*	Should be process thread.  We'll assume it might be
*	interrupt though.  When we add support for statically
*	compiled drivers, this function will be called in the
*	context of the kernel startup code.
----------------------------------------------------------------*/
int wlan_setup(wlandevice_t *wlandev, struct device *physdev)
{
	int result = 0;
	netdevice_t *netdev;
=======
 * wlan_setup
 *
 * Roughly matches the functionality of ether_setup.  Here
 * we set up any members of the wlandevice structure that are common
 * to all devices.  Additionally, we allocate a linux 'struct device'
 * and perform the same setup as ether_setup.
 *
 * Note: It's important that the caller have setup the wlandev->name
 *	ptr prior to calling this function.
 *
 * Arguments:
 *	wlandev		ptr to the wlandev structure for the
 *			interface.
 *	physdev		ptr to usb device
 * Returns:
 *	zero on success, non-zero otherwise.
 * Call Context:
 *	Should be process thread.  We'll assume it might be
 *	interrupt though.  When we add support for statically
 *	compiled drivers, this function will be called in the
 *	context of the kernel startup code.
 *----------------------------------------------------------------
 */
int wlan_setup(struct wlandevice *wlandev, struct device *physdev)
{
	int result = 0;
	struct net_device *netdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct wiphy *wiphy;
	struct wireless_dev *wdev;

	/* Set up the wlandev */
	wlandev->state = WLAN_DEVICE_CLOSED;
	wlandev->ethconv = WLAN_ETHCONV_8021h;
	wlandev->macmode = WLAN_MACMODE_NONE;

	/* Set up the rx queue */
	skb_queue_head_init(&wlandev->nsd_rxq);
<<<<<<< HEAD
	tasklet_init(&wlandev->rx_bh,
		     p80211netdev_rx_bh, (unsigned long)wlandev);

	/* Allocate and initialize the wiphy struct */
	wiphy = wlan_create_wiphy(physdev, wlandev);
	if (wiphy == NULL) {
		printk(KERN_ERR "Failed to alloc wiphy.\n");
=======
	tasklet_setup(&wlandev->rx_bh, p80211netdev_rx_bh);

	/* Allocate and initialize the wiphy struct */
	wiphy = wlan_create_wiphy(physdev, wlandev);
	if (!wiphy) {
		dev_err(physdev, "Failed to alloc wiphy.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}

	/* Allocate and initialize the struct device */
	netdev = alloc_netdev(sizeof(struct wireless_dev), "wlan%d",
<<<<<<< HEAD
				ether_setup);
	if (netdev == NULL) {
		printk(KERN_ERR "Failed to alloc netdev.\n");
=======
			      NET_NAME_UNKNOWN, ether_setup);
	if (!netdev) {
		dev_err(physdev, "Failed to alloc netdev.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wlan_free_wiphy(wiphy);
		result = 1;
	} else {
		wlandev->netdev = netdev;
		netdev->ml_priv = wlandev;
		netdev->netdev_ops = &p80211_netdev_ops;
		wdev = netdev_priv(netdev);
		wdev->wiphy = wiphy;
		wdev->iftype = NL80211_IFTYPE_STATION;
		netdev->ieee80211_ptr = wdev;
<<<<<<< HEAD
=======
		netdev->min_mtu = 68;
		/* 2312 is max 802.11 payload, 20 is overhead,
		 * (ether + llc + snap) and another 8 for wep.
		 */
		netdev->max_mtu = (2312 - 20 - 8);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		netif_stop_queue(netdev);
		netif_carrier_off(netdev);
	}

	return result;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* wlan_unsetup
*
* This function is paired with the wlan_setup routine.  It should
* be called after unregister_wlandev.  Basically, all it does is
* free the 'struct device' that's associated with the wlandev.
* We do it here because the 'struct device' isn't allocated
* explicitly in the driver code, it's done in wlan_setup.  To
* do the free in the driver might seem like 'magic'.
*
* Arguments:
*	wlandev		ptr to the wlandev structure for the
*			interface.
* Returns:
*	zero on success, non-zero otherwise.
* Call Context:
*	Should be process thread.  We'll assume it might be
*	interrupt though.  When we add support for statically
*	compiled drivers, this function will be called in the
*	context of the kernel startup code.
----------------------------------------------------------------*/
int wlan_unsetup(wlandevice_t *wlandev)
=======
 * wlan_unsetup
 *
 * This function is paired with the wlan_setup routine.  It should
 * be called after unregister_wlandev.  Basically, all it does is
 * free the 'struct device' that's associated with the wlandev.
 * We do it here because the 'struct device' isn't allocated
 * explicitly in the driver code, it's done in wlan_setup.  To
 * do the free in the driver might seem like 'magic'.
 *
 * Arguments:
 *	wlandev		ptr to the wlandev structure for the
 *			interface.
 * Call Context:
 *	Should be process thread.  We'll assume it might be
 *	interrupt though.  When we add support for statically
 *	compiled drivers, this function will be called in the
 *	context of the kernel startup code.
 *----------------------------------------------------------------
 */
void wlan_unsetup(struct wlandevice *wlandev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct wireless_dev *wdev;

	tasklet_kill(&wlandev->rx_bh);

	if (wlandev->netdev) {
		wdev = netdev_priv(wlandev->netdev);
		if (wdev->wiphy)
			wlan_free_wiphy(wdev->wiphy);
		free_netdev(wlandev->netdev);
		wlandev->netdev = NULL;
	}
<<<<<<< HEAD

	return 0;
}

/*----------------------------------------------------------------
* register_wlandev
*
* Roughly matches the functionality of register_netdev.  This function
* is called after the driver has successfully probed and set up the
* resources for the device.  It's now ready to become a named device
* in the Linux system.
*
* First we allocate a name for the device (if not already set), then
* we call the Linux function register_netdevice.
*
* Arguments:
*	wlandev		ptr to the wlandev structure for the
*			interface.
* Returns:
*	zero on success, non-zero otherwise.
* Call Context:
*	Can be either interrupt or not.
----------------------------------------------------------------*/
int register_wlandev(wlandevice_t *wlandev)
{
	int i = 0;

	i = register_netdev(wlandev->netdev);
	if (i)
		return i;

	return 0;
}

/*----------------------------------------------------------------
* unregister_wlandev
*
* Roughly matches the functionality of unregister_netdev.  This
* function is called to remove a named device from the system.
*
* First we tell linux that the device should no longer exist.
* Then we remove it from the list of known wlan devices.
*
* Arguments:
*	wlandev		ptr to the wlandev structure for the
*			interface.
* Returns:
*	zero on success, non-zero otherwise.
* Call Context:
*	Can be either interrupt or not.
----------------------------------------------------------------*/
int unregister_wlandev(wlandevice_t *wlandev)
=======
}

/*----------------------------------------------------------------
 * register_wlandev
 *
 * Roughly matches the functionality of register_netdev.  This function
 * is called after the driver has successfully probed and set up the
 * resources for the device.  It's now ready to become a named device
 * in the Linux system.
 *
 * First we allocate a name for the device (if not already set), then
 * we call the Linux function register_netdevice.
 *
 * Arguments:
 *	wlandev		ptr to the wlandev structure for the
 *			interface.
 * Returns:
 *	zero on success, non-zero otherwise.
 * Call Context:
 *	Can be either interrupt or not.
 *----------------------------------------------------------------
 */
int register_wlandev(struct wlandevice *wlandev)
{
	return register_netdev(wlandev->netdev);
}

/*----------------------------------------------------------------
 * unregister_wlandev
 *
 * Roughly matches the functionality of unregister_netdev.  This
 * function is called to remove a named device from the system.
 *
 * First we tell linux that the device should no longer exist.
 * Then we remove it from the list of known wlan devices.
 *
 * Arguments:
 *	wlandev		ptr to the wlandev structure for the
 *			interface.
 * Returns:
 *	zero on success, non-zero otherwise.
 * Call Context:
 *	Can be either interrupt or not.
 *----------------------------------------------------------------
 */
int unregister_wlandev(struct wlandevice *wlandev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sk_buff *skb;

	unregister_netdev(wlandev->netdev);

	/* Now to clean out the rx queue */
	while ((skb = skb_dequeue(&wlandev->nsd_rxq)))
		dev_kfree_skb(skb);

	return 0;
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211netdev_hwremoved
*
* Hardware removed notification. This function should be called
* immediately after an MSD has detected that the underlying hardware
* has been yanked out from under us.  The primary things we need
* to do are:
*   - Mark the wlandev
*   - Prevent any further traffic from the knetdev i/f
*   - Prevent any further requests from mgmt i/f
*   - If there are any waitq'd mgmt requests or mgmt-frame exchanges,
*     shut them down.
*   - Call the MSD hwremoved function.
*
* The remainder of the cleanup will be handled by unregister().
* Our primary goal here is to prevent as much tickling of the MSD
* as possible since the MSD is already in a 'wounded' state.
*
* TODO: As new features are added, this function should be
*       updated.
*
* Arguments:
*	wlandev		WLAN network device structure
* Returns:
*	nothing
* Side effects:
*
* Call context:
*	Usually interrupt.
----------------------------------------------------------------*/
void p80211netdev_hwremoved(wlandevice_t *wlandev)
=======
 * p80211netdev_hwremoved
 *
 * Hardware removed notification. This function should be called
 * immediately after an MSD has detected that the underlying hardware
 * has been yanked out from under us.  The primary things we need
 * to do are:
 *   - Mark the wlandev
 *   - Prevent any further traffic from the knetdev i/f
 *   - Prevent any further requests from mgmt i/f
 *   - If there are any waitq'd mgmt requests or mgmt-frame exchanges,
 *     shut them down.
 *   - Call the MSD hwremoved function.
 *
 * The remainder of the cleanup will be handled by unregister().
 * Our primary goal here is to prevent as much tickling of the MSD
 * as possible since the MSD is already in a 'wounded' state.
 *
 * TODO: As new features are added, this function should be
 *       updated.
 *
 * Arguments:
 *	wlandev		WLAN network device structure
 * Returns:
 *	nothing
 * Side effects:
 *
 * Call context:
 *	Usually interrupt.
 *----------------------------------------------------------------
 */
void p80211netdev_hwremoved(struct wlandevice *wlandev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	wlandev->hwremoved = 1;
	if (wlandev->state == WLAN_DEVICE_OPEN)
		netif_stop_queue(wlandev->netdev);

	netif_device_detach(wlandev->netdev);
}

/*----------------------------------------------------------------
<<<<<<< HEAD
* p80211_rx_typedrop
*
* Classifies the frame, increments the appropriate counter, and
* returns 0|1|2 indicating whether the driver should handle, ignore, or
* drop the frame
*
* Arguments:
*	wlandev		wlan device structure
*	fc		frame control field
*
* Returns:
*	zero if the frame should be handled by the driver,
*       one if the frame should be ignored
*       anything else means we drop it.
*
* Side effects:
*
* Call context:
*	interrupt
----------------------------------------------------------------*/
static int p80211_rx_typedrop(wlandevice_t *wlandev, u16 fc)
=======
 * p80211_rx_typedrop
 *
 * Classifies the frame, increments the appropriate counter, and
 * returns 0|1|2 indicating whether the driver should handle, ignore, or
 * drop the frame
 *
 * Arguments:
 *	wlandev		wlan device structure
 *	fc		frame control field
 *
 * Returns:
 *	zero if the frame should be handled by the driver,
 *       one if the frame should be ignored
 *       anything else means we drop it.
 *
 * Side effects:
 *
 * Call context:
 *	interrupt
 *----------------------------------------------------------------
 */
static int p80211_rx_typedrop(struct wlandevice *wlandev, u16 fc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 ftype;
	u16 fstype;
	int drop = 0;
	/* Classify frame, increment counter */
	ftype = WLAN_GET_FC_FTYPE(fc);
	fstype = WLAN_GET_FC_FSTYPE(fc);
<<<<<<< HEAD
#if 0
	pr_debug("rx_typedrop : ftype=%d fstype=%d.\n", ftype, fstype);
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (ftype) {
	case WLAN_FTYPE_MGMT:
		if ((wlandev->netdev->flags & IFF_PROMISC) ||
		    (wlandev->netdev->flags & IFF_ALLMULTI)) {
			drop = 1;
			break;
		}
<<<<<<< HEAD
		pr_debug("rx'd mgmt:\n");
		wlandev->rx.mgmt++;
		switch (fstype) {
		case WLAN_FSTYPE_ASSOCREQ:
			/* printk("assocreq"); */
			wlandev->rx.assocreq++;
			break;
		case WLAN_FSTYPE_ASSOCRESP:
			/* printk("assocresp"); */
			wlandev->rx.assocresp++;
			break;
		case WLAN_FSTYPE_REASSOCREQ:
			/* printk("reassocreq"); */
			wlandev->rx.reassocreq++;
			break;
		case WLAN_FSTYPE_REASSOCRESP:
			/* printk("reassocresp"); */
			wlandev->rx.reassocresp++;
			break;
		case WLAN_FSTYPE_PROBEREQ:
			/* printk("probereq"); */
			wlandev->rx.probereq++;
			break;
		case WLAN_FSTYPE_PROBERESP:
			/* printk("proberesp"); */
			wlandev->rx.proberesp++;
			break;
		case WLAN_FSTYPE_BEACON:
			/* printk("beacon"); */
			wlandev->rx.beacon++;
			break;
		case WLAN_FSTYPE_ATIM:
			/* printk("atim"); */
			wlandev->rx.atim++;
			break;
		case WLAN_FSTYPE_DISASSOC:
			/* printk("disassoc"); */
			wlandev->rx.disassoc++;
			break;
		case WLAN_FSTYPE_AUTHEN:
			/* printk("authen"); */
			wlandev->rx.authen++;
			break;
		case WLAN_FSTYPE_DEAUTHEN:
			/* printk("deauthen"); */
			wlandev->rx.deauthen++;
			break;
		default:
			/* printk("unknown"); */
			wlandev->rx.mgmt_unknown++;
			break;
		}
		/* printk("\n"); */
=======
		netdev_dbg(wlandev->netdev, "rx'd mgmt:\n");
		wlandev->rx.mgmt++;
		switch (fstype) {
		case WLAN_FSTYPE_ASSOCREQ:
			wlandev->rx.assocreq++;
			break;
		case WLAN_FSTYPE_ASSOCRESP:
			wlandev->rx.assocresp++;
			break;
		case WLAN_FSTYPE_REASSOCREQ:
			wlandev->rx.reassocreq++;
			break;
		case WLAN_FSTYPE_REASSOCRESP:
			wlandev->rx.reassocresp++;
			break;
		case WLAN_FSTYPE_PROBEREQ:
			wlandev->rx.probereq++;
			break;
		case WLAN_FSTYPE_PROBERESP:
			wlandev->rx.proberesp++;
			break;
		case WLAN_FSTYPE_BEACON:
			wlandev->rx.beacon++;
			break;
		case WLAN_FSTYPE_ATIM:
			wlandev->rx.atim++;
			break;
		case WLAN_FSTYPE_DISASSOC:
			wlandev->rx.disassoc++;
			break;
		case WLAN_FSTYPE_AUTHEN:
			wlandev->rx.authen++;
			break;
		case WLAN_FSTYPE_DEAUTHEN:
			wlandev->rx.deauthen++;
			break;
		default:
			wlandev->rx.mgmt_unknown++;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drop = 2;
		break;

	case WLAN_FTYPE_CTL:
		if ((wlandev->netdev->flags & IFF_PROMISC) ||
		    (wlandev->netdev->flags & IFF_ALLMULTI)) {
			drop = 1;
			break;
		}
<<<<<<< HEAD
		pr_debug("rx'd ctl:\n");
		wlandev->rx.ctl++;
		switch (fstype) {
		case WLAN_FSTYPE_PSPOLL:
			/* printk("pspoll"); */
			wlandev->rx.pspoll++;
			break;
		case WLAN_FSTYPE_RTS:
			/* printk("rts"); */
			wlandev->rx.rts++;
			break;
		case WLAN_FSTYPE_CTS:
			/* printk("cts"); */
			wlandev->rx.cts++;
			break;
		case WLAN_FSTYPE_ACK:
			/* printk("ack"); */
			wlandev->rx.ack++;
			break;
		case WLAN_FSTYPE_CFEND:
			/* printk("cfend"); */
			wlandev->rx.cfend++;
			break;
		case WLAN_FSTYPE_CFENDCFACK:
			/* printk("cfendcfack"); */
			wlandev->rx.cfendcfack++;
			break;
		default:
			/* printk("unknown"); */
			wlandev->rx.ctl_unknown++;
			break;
		}
		/* printk("\n"); */
=======
		netdev_dbg(wlandev->netdev, "rx'd ctl:\n");
		wlandev->rx.ctl++;
		switch (fstype) {
		case WLAN_FSTYPE_PSPOLL:
			wlandev->rx.pspoll++;
			break;
		case WLAN_FSTYPE_RTS:
			wlandev->rx.rts++;
			break;
		case WLAN_FSTYPE_CTS:
			wlandev->rx.cts++;
			break;
		case WLAN_FSTYPE_ACK:
			wlandev->rx.ack++;
			break;
		case WLAN_FSTYPE_CFEND:
			wlandev->rx.cfend++;
			break;
		case WLAN_FSTYPE_CFENDCFACK:
			wlandev->rx.cfendcfack++;
			break;
		default:
			wlandev->rx.ctl_unknown++;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		drop = 2;
		break;

	case WLAN_FTYPE_DATA:
		wlandev->rx.data++;
		switch (fstype) {
		case WLAN_FSTYPE_DATAONLY:
			wlandev->rx.dataonly++;
			break;
		case WLAN_FSTYPE_DATA_CFACK:
			wlandev->rx.data_cfack++;
			break;
		case WLAN_FSTYPE_DATA_CFPOLL:
			wlandev->rx.data_cfpoll++;
			break;
		case WLAN_FSTYPE_DATA_CFACK_CFPOLL:
			wlandev->rx.data__cfack_cfpoll++;
			break;
		case WLAN_FSTYPE_NULL:
<<<<<<< HEAD
			pr_debug("rx'd data:null\n");
			wlandev->rx.null++;
			break;
		case WLAN_FSTYPE_CFACK:
			pr_debug("rx'd data:cfack\n");
			wlandev->rx.cfack++;
			break;
		case WLAN_FSTYPE_CFPOLL:
			pr_debug("rx'd data:cfpoll\n");
			wlandev->rx.cfpoll++;
			break;
		case WLAN_FSTYPE_CFACK_CFPOLL:
			pr_debug("rx'd data:cfack_cfpoll\n");
			wlandev->rx.cfack_cfpoll++;
			break;
		default:
			/* printk("unknown"); */
=======
			netdev_dbg(wlandev->netdev, "rx'd data:null\n");
			wlandev->rx.null++;
			break;
		case WLAN_FSTYPE_CFACK:
			netdev_dbg(wlandev->netdev, "rx'd data:cfack\n");
			wlandev->rx.cfack++;
			break;
		case WLAN_FSTYPE_CFPOLL:
			netdev_dbg(wlandev->netdev, "rx'd data:cfpoll\n");
			wlandev->rx.cfpoll++;
			break;
		case WLAN_FSTYPE_CFACK_CFPOLL:
			netdev_dbg(wlandev->netdev, "rx'd data:cfack_cfpoll\n");
			wlandev->rx.cfack_cfpoll++;
			break;
		default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			wlandev->rx.data_unknown++;
			break;
		}

		break;
	}
	return drop;
}

<<<<<<< HEAD
static void p80211knetdev_tx_timeout(netdevice_t *netdev)
{
	wlandevice_t *wlandev = netdev->ml_priv;
=======
static void p80211knetdev_tx_timeout(struct net_device *netdev, unsigned int txqueue)
{
	struct wlandevice *wlandev = netdev->ml_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (wlandev->tx_timeout) {
		wlandev->tx_timeout(wlandev);
	} else {
<<<<<<< HEAD
		printk(KERN_WARNING "Implement tx_timeout for %s\n",
		       wlandev->nsdname);
=======
		netdev_warn(netdev, "Implement tx_timeout for %s\n",
			    wlandev->nsdname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		netif_wake_queue(wlandev->netdev);
	}
}
