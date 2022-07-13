<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Texas Instruments Ethernet Switch Driver
 *
 * Copyright (C) 2012 Texas Instruments
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
#include <linux/phy.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

#include <linux/platform_data/cpsw.h>

#include "cpsw_ale.h"
#include "davinci_cpdma.h"

#define CPSW_DEBUG	(NETIF_MSG_HW		| NETIF_MSG_WOL		| \
			 NETIF_MSG_DRV		| NETIF_MSG_LINK	| \
			 NETIF_MSG_IFUP		| NETIF_MSG_INTR	| \
			 NETIF_MSG_PROBE	| NETIF_MSG_TIMER	| \
			 NETIF_MSG_IFDOWN	| NETIF_MSG_RX_ERR	| \
			 NETIF_MSG_TX_ERR	| NETIF_MSG_TX_DONE	| \
			 NETIF_MSG_PKTDATA	| NETIF_MSG_TX_QUEUED	| \
			 NETIF_MSG_RX_STATUS)

#define cpsw_info(priv, type, format, ...)		\
do {								\
	if (netif_msg_##type(priv) && net_ratelimit())		\
		dev_info(priv->dev, format, ## __VA_ARGS__);	\
} while (0)

#define cpsw_err(priv, type, format, ...)		\
do {								\
	if (netif_msg_##type(priv) && net_ratelimit())		\
		dev_err(priv->dev, format, ## __VA_ARGS__);	\
} while (0)

#define cpsw_dbg(priv, type, format, ...)		\
do {								\
	if (netif_msg_##type(priv) && net_ratelimit())		\
		dev_dbg(priv->dev, format, ## __VA_ARGS__);	\
} while (0)

#define cpsw_notice(priv, type, format, ...)		\
do {								\
	if (netif_msg_##type(priv) && net_ratelimit())		\
		dev_notice(priv->dev, format, ## __VA_ARGS__);	\
} while (0)

#define CPSW_MAJOR_VERSION(reg)		(reg >> 8 & 0x7)
#define CPSW_MINOR_VERSION(reg)		(reg & 0xff)
#define CPSW_RTL_VERSION(reg)		((reg >> 11) & 0x1f)

#define CPDMA_RXTHRESH		0x0c0
#define CPDMA_RXFREE		0x0e0
#define CPDMA_TXHDP		0x00
#define CPDMA_RXHDP		0x20
#define CPDMA_TXCP		0x40
#define CPDMA_RXCP		0x60

#define cpsw_dma_regs(base, offset)		\
	(void __iomem *)((base) + (offset))
#define cpsw_dma_rxthresh(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_RXTHRESH)
#define cpsw_dma_rxfree(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_RXFREE)
#define cpsw_dma_txhdp(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_TXHDP)
#define cpsw_dma_rxhdp(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_RXHDP)
#define cpsw_dma_txcp(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_TXCP)
#define cpsw_dma_rxcp(base, offset)		\
	(void __iomem *)((base) + (offset) + CPDMA_RXCP)

#define CPSW_POLL_WEIGHT	64
#define CPSW_MIN_PACKET_SIZE	60
#define CPSW_MAX_PACKET_SIZE	(1500 + 14 + 4 + 4)

#define RX_PRIORITY_MAPPING	0x76543210
#define TX_PRIORITY_MAPPING	0x33221100
#define CPDMA_TX_PRIORITY_MAP	0x76543210

#define cpsw_enable_irq(priv)	\
	do {			\
		u32 i;		\
		for (i = 0; i < priv->num_irqs; i++) \
			enable_irq(priv->irqs_table[i]); \
	} while (0);
#define cpsw_disable_irq(priv)	\
	do {			\
		u32 i;		\
		for (i = 0; i < priv->num_irqs; i++) \
			disable_irq_nosync(priv->irqs_table[i]); \
	} while (0);
=======
#include <linux/net_tstamp.h>
#include <linux/phy.h>
#include <linux/phy/phy.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>
#include <linux/of_platform.h>
#include <linux/if_vlan.h>
#include <linux/kmemleak.h>
#include <linux/sys_soc.h>
#include <net/page_pool/helpers.h>
#include <linux/bpf.h>
#include <linux/bpf_trace.h>

#include <linux/pinctrl/consumer.h>
#include <net/pkt_cls.h>

#include "cpsw.h"
#include "cpsw_ale.h"
#include "cpsw_priv.h"
#include "cpsw_sl.h"
#include "cpts.h"
#include "davinci_cpdma.h"

#include <net/pkt_sched.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int debug_level;
module_param(debug_level, int, 0);
MODULE_PARM_DESC(debug_level, "cpsw debug level (NETIF_MSG bits)");

static int ale_ageout = 10;
module_param(ale_ageout, int, 0);
MODULE_PARM_DESC(ale_ageout, "cpsw ale ageout interval (seconds)");

static int rx_packet_max = CPSW_MAX_PACKET_SIZE;
module_param(rx_packet_max, int, 0);
MODULE_PARM_DESC(rx_packet_max, "maximum receive packet size (bytes)");

<<<<<<< HEAD
struct cpsw_ss_regs {
	u32	id_ver;
	u32	soft_reset;
	u32	control;
	u32	int_control;
	u32	rx_thresh_en;
	u32	rx_en;
	u32	tx_en;
	u32	misc_en;
};

struct cpsw_regs {
	u32	id_ver;
	u32	control;
	u32	soft_reset;
	u32	stat_port_en;
	u32	ptype;
};

struct cpsw_slave_regs {
	u32	max_blks;
	u32	blk_cnt;
	u32	flow_thresh;
	u32	port_vlan;
	u32	tx_pri_map;
	u32	ts_ctl;
	u32	ts_seq_ltype;
	u32	ts_vlan;
	u32	sa_lo;
	u32	sa_hi;
};

struct cpsw_host_regs {
	u32	max_blks;
	u32	blk_cnt;
	u32	flow_thresh;
	u32	port_vlan;
	u32	tx_pri_map;
	u32	cpdma_tx_pri_map;
	u32	cpdma_rx_chan_map;
};

struct cpsw_sliver_regs {
	u32	id_ver;
	u32	mac_control;
	u32	mac_status;
	u32	soft_reset;
	u32	rx_maxlen;
	u32	__reserved_0;
	u32	rx_pause;
	u32	tx_pause;
	u32	__reserved_1;
	u32	rx_pri_map;
};

struct cpsw_slave {
	struct cpsw_slave_regs __iomem	*regs;
	struct cpsw_sliver_regs __iomem	*sliver;
	int				slave_num;
	u32				mac_control;
	struct cpsw_slave_data		*data;
	struct phy_device		*phy;
};

struct cpsw_priv {
	spinlock_t			lock;
	struct platform_device		*pdev;
	struct net_device		*ndev;
	struct resource			*cpsw_res;
	struct resource			*cpsw_ss_res;
	struct napi_struct		napi;
	struct device			*dev;
	struct cpsw_platform_data	data;
	struct cpsw_regs __iomem	*regs;
	struct cpsw_ss_regs __iomem	*ss_regs;
	struct cpsw_host_regs __iomem	*host_port_regs;
	u32				msg_enable;
	struct net_device_stats		stats;
	int				rx_packet_max;
	int				host_port;
	struct clk			*clk;
	u8				mac_addr[ETH_ALEN];
	struct cpsw_slave		*slaves;
	struct cpdma_ctlr		*dma;
	struct cpdma_chan		*txch, *rxch;
	struct cpsw_ale			*ale;
	/* snapshot of IRQ numbers */
	u32 irqs_table[4];
	u32 num_irqs;
};

#define napi_to_priv(napi)	container_of(napi, struct cpsw_priv, napi)
#define for_each_slave(priv, func, arg...)			\
	do {							\
		int idx;					\
		for (idx = 0; idx < (priv)->data.slaves; idx++)	\
			(func)((priv)->slaves + idx, ##arg);	\
	} while (0)

static void cpsw_intr_enable(struct cpsw_priv *priv)
{
	__raw_writel(0xFF, &priv->ss_regs->tx_en);
	__raw_writel(0xFF, &priv->ss_regs->rx_en);

	cpdma_ctlr_int_ctrl(priv->dma, true);
	return;
}

static void cpsw_intr_disable(struct cpsw_priv *priv)
{
	__raw_writel(0, &priv->ss_regs->tx_en);
	__raw_writel(0, &priv->ss_regs->rx_en);

	cpdma_ctlr_int_ctrl(priv->dma, false);
	return;
}

void cpsw_tx_handler(void *token, int len, int status)
{
	struct sk_buff		*skb = token;
	struct net_device	*ndev = skb->dev;
	struct cpsw_priv	*priv = netdev_priv(ndev);

	if (unlikely(netif_queue_stopped(ndev)))
		netif_wake_queue(ndev);
	priv->stats.tx_packets++;
	priv->stats.tx_bytes += len;
	dev_kfree_skb_any(skb);
}

void cpsw_rx_handler(void *token, int len, int status)
{
	struct sk_buff		*skb = token;
	struct net_device	*ndev = skb->dev;
	struct cpsw_priv	*priv = netdev_priv(ndev);
	int			ret = 0;

	/* free and bail if we are shutting down */
	if (unlikely(!netif_running(ndev)) ||
			unlikely(!netif_carrier_ok(ndev))) {
		dev_kfree_skb_any(skb);
		return;
	}
	if (likely(status >= 0)) {
		skb_put(skb, len);
		skb->protocol = eth_type_trans(skb, ndev);
		netif_receive_skb(skb);
		priv->stats.rx_bytes += len;
		priv->stats.rx_packets++;
		skb = NULL;
	}

	if (unlikely(!netif_running(ndev))) {
		if (skb)
			dev_kfree_skb_any(skb);
		return;
	}

	if (likely(!skb)) {
		skb = netdev_alloc_skb_ip_align(ndev, priv->rx_packet_max);
		if (WARN_ON(!skb))
			return;

		ret = cpdma_chan_submit(priv->rxch, skb, skb->data,
					skb_tailroom(skb), GFP_KERNEL);
	}
	WARN_ON(ret < 0);
}

static irqreturn_t cpsw_interrupt(int irq, void *dev_id)
{
	struct cpsw_priv *priv = dev_id;

	if (likely(netif_running(priv->ndev))) {
		cpsw_intr_disable(priv);
		cpsw_disable_irq(priv);
		napi_schedule(&priv->napi);
	}
	return IRQ_HANDLED;
}

static inline int cpsw_get_slave_port(struct cpsw_priv *priv, u32 slave_num)
{
	if (priv->host_port == 0)
		return slave_num + 1;
	else
		return slave_num;
}

static int cpsw_poll(struct napi_struct *napi, int budget)
{
	struct cpsw_priv	*priv = napi_to_priv(napi);
	int			num_tx, num_rx;

	num_tx = cpdma_chan_process(priv->txch, 128);
	num_rx = cpdma_chan_process(priv->rxch, budget);

	if (num_rx || num_tx)
		cpsw_dbg(priv, intr, "poll %d rx, %d tx pkts\n",
			 num_rx, num_tx);

	if (num_rx < budget) {
		napi_complete(napi);
		cpsw_intr_enable(priv);
		cpdma_ctlr_eoi(priv->dma);
		cpsw_enable_irq(priv);
	}

	return num_rx;
}

static inline void soft_reset(const char *module, void __iomem *reg)
{
	unsigned long timeout = jiffies + HZ;

	__raw_writel(1, reg);
	do {
		cpu_relax();
	} while ((__raw_readl(reg) & 1) && time_after(timeout, jiffies));

	WARN(__raw_readl(reg) & 1, "failed to soft-reset %s\n", module);
}

#define mac_hi(mac)	(((mac)[0] << 0) | ((mac)[1] << 8) |	\
			 ((mac)[2] << 16) | ((mac)[3] << 24))
#define mac_lo(mac)	(((mac)[4] << 0) | ((mac)[5] << 8))

static void cpsw_set_slave_mac(struct cpsw_slave *slave,
			       struct cpsw_priv *priv)
{
	__raw_writel(mac_hi(priv->mac_addr), &slave->regs->sa_hi);
	__raw_writel(mac_lo(priv->mac_addr), &slave->regs->sa_lo);
=======
static int descs_pool_size = CPSW_CPDMA_DESCS_POOL_SIZE_DEFAULT;
module_param(descs_pool_size, int, 0444);
MODULE_PARM_DESC(descs_pool_size, "Number of CPDMA CPPI descriptors in pool");

#define for_each_slave(priv, func, arg...)				\
	do {								\
		struct cpsw_slave *slave;				\
		struct cpsw_common *cpsw = (priv)->cpsw;		\
		int n;							\
		if (cpsw->data.dual_emac)				\
			(func)((cpsw)->slaves + priv->emac_port, ##arg);\
		else							\
			for (n = cpsw->data.slaves,			\
					slave = cpsw->slaves;		\
					n; n--)				\
				(func)(slave++, ##arg);			\
	} while (0)

static int cpsw_slave_index_priv(struct cpsw_common *cpsw,
				 struct cpsw_priv *priv)
{
	return cpsw->data.dual_emac ? priv->emac_port : cpsw->data.active_slave;
}

static int cpsw_get_slave_port(u32 slave_num)
{
	return slave_num + 1;
}

static int cpsw_ndo_vlan_rx_add_vid(struct net_device *ndev,
				    __be16 proto, u16 vid);

static void cpsw_set_promiscious(struct net_device *ndev, bool enable)
{
	struct cpsw_common *cpsw = ndev_to_cpsw(ndev);
	struct cpsw_ale *ale = cpsw->ale;
	int i;

	if (cpsw->data.dual_emac) {
		bool flag = false;

		/* Enabling promiscuous mode for one interface will be
		 * common for both the interface as the interface shares
		 * the same hardware resource.
		 */
		for (i = 0; i < cpsw->data.slaves; i++)
			if (cpsw->slaves[i].ndev->flags & IFF_PROMISC)
				flag = true;

		if (!enable && flag) {
			enable = true;
			dev_err(&ndev->dev, "promiscuity not disabled as the other interface is still in promiscuity mode\n");
		}

		if (enable) {
			/* Enable Bypass */
			cpsw_ale_control_set(ale, 0, ALE_BYPASS, 1);

			dev_dbg(&ndev->dev, "promiscuity enabled\n");
		} else {
			/* Disable Bypass */
			cpsw_ale_control_set(ale, 0, ALE_BYPASS, 0);
			dev_dbg(&ndev->dev, "promiscuity disabled\n");
		}
	} else {
		if (enable) {
			unsigned long timeout = jiffies + HZ;

			/* Disable Learn for all ports (host is port 0 and slaves are port 1 and up */
			for (i = 0; i <= cpsw->data.slaves; i++) {
				cpsw_ale_control_set(ale, i,
						     ALE_PORT_NOLEARN, 1);
				cpsw_ale_control_set(ale, i,
						     ALE_PORT_NO_SA_UPDATE, 1);
			}

			/* Clear All Untouched entries */
			cpsw_ale_control_set(ale, 0, ALE_AGEOUT, 1);
			do {
				cpu_relax();
				if (cpsw_ale_control_get(ale, 0, ALE_AGEOUT))
					break;
			} while (time_after(timeout, jiffies));
			cpsw_ale_control_set(ale, 0, ALE_AGEOUT, 1);

			/* Clear all mcast from ALE */
			cpsw_ale_flush_multicast(ale, ALE_ALL_PORTS, -1);
			__hw_addr_ref_unsync_dev(&ndev->mc, ndev, NULL);

			/* Flood All Unicast Packets to Host port */
			cpsw_ale_control_set(ale, 0, ALE_P0_UNI_FLOOD, 1);
			dev_dbg(&ndev->dev, "promiscuity enabled\n");
		} else {
			/* Don't Flood All Unicast Packets to Host port */
			cpsw_ale_control_set(ale, 0, ALE_P0_UNI_FLOOD, 0);

			/* Enable Learn for all ports (host is port 0 and slaves are port 1 and up */
			for (i = 0; i <= cpsw->data.slaves; i++) {
				cpsw_ale_control_set(ale, i,
						     ALE_PORT_NOLEARN, 0);
				cpsw_ale_control_set(ale, i,
						     ALE_PORT_NO_SA_UPDATE, 0);
			}
			dev_dbg(&ndev->dev, "promiscuity disabled\n");
		}
	}
}

/**
 * cpsw_set_mc - adds multicast entry to the table if it's not added or deletes
 * if it's not deleted
 * @ndev: device to sync
 * @addr: address to be added or deleted
 * @vid: vlan id, if vid < 0 set/unset address for real device
 * @add: add address if the flag is set or remove otherwise
 */
static int cpsw_set_mc(struct net_device *ndev, const u8 *addr,
		       int vid, int add)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	int mask, flags, ret;

	if (vid < 0) {
		if (cpsw->data.dual_emac)
			vid = cpsw->slaves[priv->emac_port].port_vlan;
		else
			vid = 0;
	}

	mask = cpsw->data.dual_emac ? ALE_PORT_HOST : ALE_ALL_PORTS;
	flags = vid ? ALE_VLAN : 0;

	if (add)
		ret = cpsw_ale_add_mcast(cpsw->ale, addr, mask, flags, vid, 0);
	else
		ret = cpsw_ale_del_mcast(cpsw->ale, addr, 0, flags, vid);

	return ret;
}

static int cpsw_update_vlan_mc(struct net_device *vdev, int vid, void *ctx)
{
	struct addr_sync_ctx *sync_ctx = ctx;
	struct netdev_hw_addr *ha;
	int found = 0, ret = 0;

	if (!vdev || !(vdev->flags & IFF_UP))
		return 0;

	/* vlan address is relevant if its sync_cnt != 0 */
	netdev_for_each_mc_addr(ha, vdev) {
		if (ether_addr_equal(ha->addr, sync_ctx->addr)) {
			found = ha->sync_cnt;
			break;
		}
	}

	if (found)
		sync_ctx->consumed++;

	if (sync_ctx->flush) {
		if (!found)
			cpsw_set_mc(sync_ctx->ndev, sync_ctx->addr, vid, 0);
		return 0;
	}

	if (found)
		ret = cpsw_set_mc(sync_ctx->ndev, sync_ctx->addr, vid, 1);

	return ret;
}

static int cpsw_add_mc_addr(struct net_device *ndev, const u8 *addr, int num)
{
	struct addr_sync_ctx sync_ctx;
	int ret;

	sync_ctx.consumed = 0;
	sync_ctx.addr = addr;
	sync_ctx.ndev = ndev;
	sync_ctx.flush = 0;

	ret = vlan_for_each(ndev, cpsw_update_vlan_mc, &sync_ctx);
	if (sync_ctx.consumed < num && !ret)
		ret = cpsw_set_mc(ndev, addr, -1, 1);

	return ret;
}

static int cpsw_del_mc_addr(struct net_device *ndev, const u8 *addr, int num)
{
	struct addr_sync_ctx sync_ctx;

	sync_ctx.consumed = 0;
	sync_ctx.addr = addr;
	sync_ctx.ndev = ndev;
	sync_ctx.flush = 1;

	vlan_for_each(ndev, cpsw_update_vlan_mc, &sync_ctx);
	if (sync_ctx.consumed == num)
		cpsw_set_mc(ndev, addr, -1, 0);

	return 0;
}

static int cpsw_purge_vlan_mc(struct net_device *vdev, int vid, void *ctx)
{
	struct addr_sync_ctx *sync_ctx = ctx;
	struct netdev_hw_addr *ha;
	int found = 0;

	if (!vdev || !(vdev->flags & IFF_UP))
		return 0;

	/* vlan address is relevant if its sync_cnt != 0 */
	netdev_for_each_mc_addr(ha, vdev) {
		if (ether_addr_equal(ha->addr, sync_ctx->addr)) {
			found = ha->sync_cnt;
			break;
		}
	}

	if (!found)
		return 0;

	sync_ctx->consumed++;
	cpsw_set_mc(sync_ctx->ndev, sync_ctx->addr, vid, 0);
	return 0;
}

static int cpsw_purge_all_mc(struct net_device *ndev, const u8 *addr, int num)
{
	struct addr_sync_ctx sync_ctx;

	sync_ctx.addr = addr;
	sync_ctx.ndev = ndev;
	sync_ctx.consumed = 0;

	vlan_for_each(ndev, cpsw_purge_vlan_mc, &sync_ctx);
	if (sync_ctx.consumed < num)
		cpsw_set_mc(ndev, addr, -1, 0);

	return 0;
}

static void cpsw_ndo_set_rx_mode(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	int slave_port = -1;

	if (cpsw->data.dual_emac)
		slave_port = priv->emac_port + 1;

	if (ndev->flags & IFF_PROMISC) {
		/* Enable promiscuous mode */
		cpsw_set_promiscious(ndev, true);
		cpsw_ale_set_allmulti(cpsw->ale, IFF_ALLMULTI, slave_port);
		return;
	} else {
		/* Disable promiscuous mode */
		cpsw_set_promiscious(ndev, false);
	}

	/* Restore allmulti on vlans if necessary */
	cpsw_ale_set_allmulti(cpsw->ale,
			      ndev->flags & IFF_ALLMULTI, slave_port);

	/* add/remove mcast address either for real netdev or for vlan */
	__hw_addr_ref_sync_dev(&ndev->mc, ndev, cpsw_add_mc_addr,
			       cpsw_del_mc_addr);
}

static unsigned int cpsw_rxbuf_total_len(unsigned int len)
{
	len += CPSW_HEADROOM_NA;
	len += SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	return SKB_DATA_ALIGN(len);
}

static void cpsw_rx_handler(void *token, int len, int status)
{
	struct page		*new_page, *page = token;
	void			*pa = page_address(page);
	struct cpsw_meta_xdp	*xmeta = pa + CPSW_XMETA_OFFSET;
	struct cpsw_common	*cpsw = ndev_to_cpsw(xmeta->ndev);
	int			pkt_size = cpsw->rx_packet_max;
	int			ret = 0, port, ch = xmeta->ch;
	int			headroom = CPSW_HEADROOM_NA;
	struct net_device	*ndev = xmeta->ndev;
	struct cpsw_priv	*priv;
	struct page_pool	*pool;
	struct sk_buff		*skb;
	struct xdp_buff		xdp;
	dma_addr_t		dma;

	if (cpsw->data.dual_emac && status >= 0) {
		port = CPDMA_RX_SOURCE_PORT(status);
		if (port)
			ndev = cpsw->slaves[--port].ndev;
	}

	priv = netdev_priv(ndev);
	pool = cpsw->page_pool[ch];
	if (unlikely(status < 0) || unlikely(!netif_running(ndev))) {
		/* In dual emac mode check for all interfaces */
		if (cpsw->data.dual_emac && cpsw->usage_count &&
		    (status >= 0)) {
			/* The packet received is for the interface which
			 * is already down and the other interface is up
			 * and running, instead of freeing which results
			 * in reducing of the number of rx descriptor in
			 * DMA engine, requeue page back to cpdma.
			 */
			new_page = page;
			goto requeue;
		}

		/* the interface is going down, pages are purged */
		page_pool_recycle_direct(pool, page);
		return;
	}

	new_page = page_pool_dev_alloc_pages(pool);
	if (unlikely(!new_page)) {
		new_page = page;
		ndev->stats.rx_dropped++;
		goto requeue;
	}

	if (priv->xdp_prog) {
		int size = len;

		xdp_init_buff(&xdp, PAGE_SIZE, &priv->xdp_rxq[ch]);
		if (status & CPDMA_RX_VLAN_ENCAP) {
			headroom += CPSW_RX_VLAN_ENCAP_HDR_SIZE;
			size -= CPSW_RX_VLAN_ENCAP_HDR_SIZE;
		}

		xdp_prepare_buff(&xdp, pa, headroom, size, false);

		port = priv->emac_port + cpsw->data.dual_emac;
		ret = cpsw_run_xdp(priv, ch, &xdp, page, port, &len);
		if (ret != CPSW_XDP_PASS)
			goto requeue;

		headroom = xdp.data - xdp.data_hard_start;

		/* XDP prog can modify vlan tag, so can't use encap header */
		status &= ~CPDMA_RX_VLAN_ENCAP;
	}

	/* pass skb to netstack if no XDP prog or returned XDP_PASS */
	skb = build_skb(pa, cpsw_rxbuf_total_len(pkt_size));
	if (!skb) {
		ndev->stats.rx_dropped++;
		page_pool_recycle_direct(pool, page);
		goto requeue;
	}

	skb_reserve(skb, headroom);
	skb_put(skb, len);
	skb->dev = ndev;
	if (status & CPDMA_RX_VLAN_ENCAP)
		cpsw_rx_vlan_encap(skb);
	if (priv->rx_ts_enabled)
		cpts_rx_timestamp(cpsw->cpts, skb);
	skb->protocol = eth_type_trans(skb, ndev);

	/* mark skb for recycling */
	skb_mark_for_recycle(skb);
	netif_receive_skb(skb);

	ndev->stats.rx_bytes += len;
	ndev->stats.rx_packets++;

requeue:
	xmeta = page_address(new_page) + CPSW_XMETA_OFFSET;
	xmeta->ndev = ndev;
	xmeta->ch = ch;

	dma = page_pool_get_dma_addr(new_page) + CPSW_HEADROOM_NA;
	ret = cpdma_chan_submit_mapped(cpsw->rxv[ch].ch, new_page, dma,
				       pkt_size, 0);
	if (ret < 0) {
		WARN_ON(ret == -ENOMEM);
		page_pool_recycle_direct(pool, new_page);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void _cpsw_adjust_link(struct cpsw_slave *slave,
			      struct cpsw_priv *priv, bool *link)
{
	struct phy_device	*phy = slave->phy;
	u32			mac_control = 0;
	u32			slave_port;
<<<<<<< HEAD
=======
	struct cpsw_common *cpsw = priv->cpsw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!phy)
		return;

<<<<<<< HEAD
	slave_port = cpsw_get_slave_port(priv, slave->slave_num);

	if (phy->link) {
		mac_control = priv->data.mac_control;

		/* enable forwarding */
		cpsw_ale_control_set(priv->ale, slave_port,
				     ALE_PORT_STATE, ALE_PORT_STATE_FORWARD);

		if (phy->speed == 1000)
			mac_control |= BIT(7);	/* GIGABITEN	*/
		if (phy->duplex)
			mac_control |= BIT(0);	/* FULLDUPLEXEN	*/
		*link = true;
	} else {
		mac_control = 0;
		/* disable forwarding */
		cpsw_ale_control_set(priv->ale, slave_port,
				     ALE_PORT_STATE, ALE_PORT_STATE_DISABLE);
	}

	if (mac_control != slave->mac_control) {
		phy_print_status(phy);
		__raw_writel(mac_control, &slave->sliver->mac_control);
	}
=======
	slave_port = cpsw_get_slave_port(slave->slave_num);

	if (phy->link) {
		mac_control = CPSW_SL_CTL_GMII_EN;

		if (phy->speed == 1000)
			mac_control |= CPSW_SL_CTL_GIG;
		if (phy->duplex)
			mac_control |= CPSW_SL_CTL_FULLDUPLEX;

		/* set speed_in input in case RMII mode is used in 100Mbps */
		if (phy->speed == 100)
			mac_control |= CPSW_SL_CTL_IFCTL_A;
		/* in band mode only works in 10Mbps RGMII mode */
		else if ((phy->speed == 10) && phy_interface_is_rgmii(phy))
			mac_control |= CPSW_SL_CTL_EXT_EN; /* In Band mode */

		if (priv->rx_pause)
			mac_control |= CPSW_SL_CTL_RX_FLOW_EN;

		if (priv->tx_pause)
			mac_control |= CPSW_SL_CTL_TX_FLOW_EN;

		if (mac_control != slave->mac_control)
			cpsw_sl_ctl_set(slave->mac_sl, mac_control);

		/* enable forwarding */
		cpsw_ale_control_set(cpsw->ale, slave_port,
				     ALE_PORT_STATE, ALE_PORT_STATE_FORWARD);

		*link = true;

		if (priv->shp_cfg_speed &&
		    priv->shp_cfg_speed != slave->phy->speed &&
		    !cpsw_shp_is_off(priv))
			dev_warn(priv->dev,
				 "Speed was changed, CBS shaper speeds are changed!");
	} else {
		mac_control = 0;
		/* disable forwarding */
		cpsw_ale_control_set(cpsw->ale, slave_port,
				     ALE_PORT_STATE, ALE_PORT_STATE_DISABLE);

		cpsw_sl_wait_for_idle(slave->mac_sl, 100);

		cpsw_sl_ctl_reset(slave->mac_sl);
	}

	if (mac_control != slave->mac_control)
		phy_print_status(phy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	slave->mac_control = mac_control;
}

static void cpsw_adjust_link(struct net_device *ndev)
{
	struct cpsw_priv	*priv = netdev_priv(ndev);
<<<<<<< HEAD
=======
	struct cpsw_common	*cpsw = priv->cpsw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool			link = false;

	for_each_slave(priv, _cpsw_adjust_link, priv, &link);

	if (link) {
<<<<<<< HEAD
		netif_carrier_on(ndev);
		if (netif_running(ndev))
			netif_wake_queue(ndev);
	} else {
		netif_carrier_off(ndev);
		netif_stop_queue(ndev);
	}
}

static inline int __show_stat(char *buf, int maxlen, const char *name, u32 val)
{
	static char *leader = "........................................";

	if (!val)
		return 0;
	else
		return snprintf(buf, maxlen, "%s %s %10d\n", name,
				leader + strlen(name), val);
=======
		if (cpsw_need_resplit(cpsw))
			cpsw_split_res(cpsw);

		netif_carrier_on(ndev);
		if (netif_running(ndev))
			netif_tx_wake_all_queues(ndev);
	} else {
		netif_carrier_off(ndev);
		netif_tx_stop_all_queues(ndev);
	}
}

static inline void cpsw_add_dual_emac_def_ale_entries(
		struct cpsw_priv *priv, struct cpsw_slave *slave,
		u32 slave_port)
{
	struct cpsw_common *cpsw = priv->cpsw;
	u32 port_mask = 1 << slave_port | ALE_PORT_HOST;

	if (cpsw->version == CPSW_VERSION_1)
		slave_write(slave, slave->port_vlan, CPSW1_PORT_VLAN);
	else
		slave_write(slave, slave->port_vlan, CPSW2_PORT_VLAN);
	cpsw_ale_add_vlan(cpsw->ale, slave->port_vlan, port_mask,
			  port_mask, port_mask, 0);
	cpsw_ale_add_mcast(cpsw->ale, priv->ndev->broadcast,
			   ALE_PORT_HOST, ALE_VLAN, slave->port_vlan, 0);
	cpsw_ale_add_ucast(cpsw->ale, priv->mac_addr,
			   HOST_PORT_NUM, ALE_VLAN |
			   ALE_SECURE, slave->port_vlan);
	cpsw_ale_control_set(cpsw->ale, slave_port,
			     ALE_PORT_DROP_UNKNOWN_VLAN, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cpsw_slave_open(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
<<<<<<< HEAD
	char name[32];
	u32 slave_port;

	sprintf(name, "slave-%d", slave->slave_num);

	soft_reset(name, &slave->sliver->soft_reset);

	/* setup priority mapping */
	__raw_writel(RX_PRIORITY_MAPPING, &slave->sliver->rx_pri_map);
	__raw_writel(TX_PRIORITY_MAPPING, &slave->regs->tx_pri_map);

	/* setup max packet size, and mac address */
	__raw_writel(priv->rx_packet_max, &slave->sliver->rx_maxlen);
=======
	u32 slave_port;
	struct phy_device *phy;
	struct cpsw_common *cpsw = priv->cpsw;

	cpsw_sl_reset(slave->mac_sl, 100);
	cpsw_sl_ctl_reset(slave->mac_sl);

	/* setup priority mapping */
	cpsw_sl_reg_write(slave->mac_sl, CPSW_SL_RX_PRI_MAP,
			  RX_PRIORITY_MAPPING);

	switch (cpsw->version) {
	case CPSW_VERSION_1:
		slave_write(slave, TX_PRIORITY_MAPPING, CPSW1_TX_PRI_MAP);
		/* Increase RX FIFO size to 5 for supporting fullduplex
		 * flow control mode
		 */
		slave_write(slave,
			    (CPSW_MAX_BLKS_TX << CPSW_MAX_BLKS_TX_SHIFT) |
			    CPSW_MAX_BLKS_RX, CPSW1_MAX_BLKS);
		break;
	case CPSW_VERSION_2:
	case CPSW_VERSION_3:
	case CPSW_VERSION_4:
		slave_write(slave, TX_PRIORITY_MAPPING, CPSW2_TX_PRI_MAP);
		/* Increase RX FIFO size to 5 for supporting fullduplex
		 * flow control mode
		 */
		slave_write(slave,
			    (CPSW_MAX_BLKS_TX << CPSW_MAX_BLKS_TX_SHIFT) |
			    CPSW_MAX_BLKS_RX, CPSW2_MAX_BLKS);
		break;
	}

	/* setup max packet size, and mac address */
	cpsw_sl_reg_write(slave->mac_sl, CPSW_SL_RX_MAXLEN,
			  cpsw->rx_packet_max);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cpsw_set_slave_mac(slave, priv);

	slave->mac_control = 0;	/* no link yet */

<<<<<<< HEAD
	slave_port = cpsw_get_slave_port(priv, slave->slave_num);

	cpsw_ale_add_mcast(priv->ale, priv->ndev->broadcast,
			   1 << slave_port, 0, ALE_MCAST_FWD_2);

	slave->phy = phy_connect(priv->ndev, slave->data->phy_id,
				 &cpsw_adjust_link, 0, slave->data->phy_if);
	if (IS_ERR(slave->phy)) {
		dev_err(priv->dev, "phy %s not found on slave %d\n",
			slave->data->phy_id, slave->slave_num);
		slave->phy = NULL;
	} else {
		dev_info(priv->dev, "phy found : id is : 0x%x\n",
			 slave->phy->phy_id);
		phy_start(slave->phy);
	}
=======
	slave_port = cpsw_get_slave_port(slave->slave_num);

	if (cpsw->data.dual_emac)
		cpsw_add_dual_emac_def_ale_entries(priv, slave, slave_port);
	else
		cpsw_ale_add_mcast(cpsw->ale, priv->ndev->broadcast,
				   1 << slave_port, 0, 0, ALE_MCAST_FWD_2);

	if (slave->data->phy_node) {
		phy = of_phy_connect(priv->ndev, slave->data->phy_node,
				 &cpsw_adjust_link, 0, slave->data->phy_if);
		if (!phy) {
			dev_err(priv->dev, "phy \"%pOF\" not found on slave %d\n",
				slave->data->phy_node,
				slave->slave_num);
			return;
		}
	} else {
		phy = phy_connect(priv->ndev, slave->data->phy_id,
				 &cpsw_adjust_link, slave->data->phy_if);
		if (IS_ERR(phy)) {
			dev_err(priv->dev,
				"phy \"%s\" not found on slave %d, err %ld\n",
				slave->data->phy_id, slave->slave_num,
				PTR_ERR(phy));
			return;
		}
	}

	phy->mac_managed_pm = true;

	slave->phy = phy;

	phy_attached_info(slave->phy);

	phy_start(slave->phy);

	/* Configure GMII_SEL register */
	if (!IS_ERR(slave->data->ifphy))
		phy_set_mode_ext(slave->data->ifphy, PHY_MODE_ETHERNET,
				 slave->data->phy_if);
	else
		cpsw_phy_sel(cpsw->dev, slave->phy->interface,
			     slave->slave_num);
}

static inline void cpsw_add_default_vlan(struct cpsw_priv *priv)
{
	struct cpsw_common *cpsw = priv->cpsw;
	const int vlan = cpsw->data.default_vlan;
	u32 reg;
	int i;
	int unreg_mcast_mask;

	reg = (cpsw->version == CPSW_VERSION_1) ? CPSW1_PORT_VLAN :
	       CPSW2_PORT_VLAN;

	writel(vlan, &cpsw->host_port_regs->port_vlan);

	for (i = 0; i < cpsw->data.slaves; i++)
		slave_write(cpsw->slaves + i, vlan, reg);

	if (priv->ndev->flags & IFF_ALLMULTI)
		unreg_mcast_mask = ALE_ALL_PORTS;
	else
		unreg_mcast_mask = ALE_PORT_1 | ALE_PORT_2;

	cpsw_ale_add_vlan(cpsw->ale, vlan, ALE_ALL_PORTS,
			  ALE_ALL_PORTS, ALE_ALL_PORTS,
			  unreg_mcast_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cpsw_init_host_port(struct cpsw_priv *priv)
{
<<<<<<< HEAD
	/* soft reset the controller and initialize ale */
	soft_reset("cpsw", &priv->regs->soft_reset);
	cpsw_ale_start(priv->ale);

	/* switch to vlan unaware mode */
	cpsw_ale_control_set(priv->ale, 0, ALE_VLAN_AWARE, 0);

	/* setup host port priority mapping */
	__raw_writel(CPDMA_TX_PRIORITY_MAP,
		     &priv->host_port_regs->cpdma_tx_pri_map);
	__raw_writel(0, &priv->host_port_regs->cpdma_rx_chan_map);

	cpsw_ale_control_set(priv->ale, priv->host_port,
			     ALE_PORT_STATE, ALE_PORT_STATE_FORWARD);

	cpsw_ale_add_ucast(priv->ale, priv->mac_addr, priv->host_port, 0);
	cpsw_ale_add_mcast(priv->ale, priv->ndev->broadcast,
			   1 << priv->host_port, 0, ALE_MCAST_FWD_2);
}

static int cpsw_ndo_open(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int i, ret;
	u32 reg;

	cpsw_intr_disable(priv);
	netif_carrier_off(ndev);

	ret = clk_enable(priv->clk);
	if (ret < 0) {
		dev_err(priv->dev, "unable to turn on device clock\n");
		return ret;
	}

	reg = __raw_readl(&priv->regs->id_ver);

	dev_info(priv->dev, "initializing cpsw version %d.%d (%d)\n",
		 CPSW_MAJOR_VERSION(reg), CPSW_MINOR_VERSION(reg),
		 CPSW_RTL_VERSION(reg));

	/* initialize host and slave ports */
	cpsw_init_host_port(priv);
	for_each_slave(priv, cpsw_slave_open, priv);

	/* setup tx dma to fixed prio and zero offset */
	cpdma_control_set(priv->dma, CPDMA_TX_PRIO_FIXED, 1);
	cpdma_control_set(priv->dma, CPDMA_RX_BUFFER_OFFSET, 0);

	/* disable priority elevation and enable statistics on all ports */
	__raw_writel(0, &priv->regs->ptype);

	/* enable statistics collection only on the host port */
	__raw_writel(0x7, &priv->regs->stat_port_en);

	if (WARN_ON(!priv->data.rx_descs))
		priv->data.rx_descs = 128;

	for (i = 0; i < priv->data.rx_descs; i++) {
		struct sk_buff *skb;

		ret = -ENOMEM;
		skb = netdev_alloc_skb_ip_align(priv->ndev,
						priv->rx_packet_max);
		if (!skb)
			break;
		ret = cpdma_chan_submit(priv->rxch, skb, skb->data,
					skb_tailroom(skb), GFP_KERNEL);
		if (WARN_ON(ret < 0))
			break;
	}
	/* continue even if we didn't manage to submit all receive descs */
	cpsw_info(priv, ifup, "submitted %d rx descriptors\n", i);

	cpdma_ctlr_start(priv->dma);
	cpsw_intr_enable(priv);
	napi_enable(&priv->napi);
	cpdma_ctlr_eoi(priv->dma);

	return 0;
}

static void cpsw_slave_stop(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
=======
	u32 fifo_mode;
	u32 control_reg;
	struct cpsw_common *cpsw = priv->cpsw;

	/* soft reset the controller and initialize ale */
	soft_reset("cpsw", &cpsw->regs->soft_reset);
	cpsw_ale_start(cpsw->ale);

	/* switch to vlan unaware mode */
	cpsw_ale_control_set(cpsw->ale, HOST_PORT_NUM, ALE_VLAN_AWARE,
			     CPSW_ALE_VLAN_AWARE);
	control_reg = readl(&cpsw->regs->control);
	control_reg |= CPSW_VLAN_AWARE | CPSW_RX_VLAN_ENCAP;
	writel(control_reg, &cpsw->regs->control);
	fifo_mode = (cpsw->data.dual_emac) ? CPSW_FIFO_DUAL_MAC_MODE :
		     CPSW_FIFO_NORMAL_MODE;
	writel(fifo_mode, &cpsw->host_port_regs->tx_in_ctl);

	/* setup host port priority mapping */
	writel_relaxed(CPDMA_TX_PRIORITY_MAP,
		       &cpsw->host_port_regs->cpdma_tx_pri_map);
	writel_relaxed(0, &cpsw->host_port_regs->cpdma_rx_chan_map);

	cpsw_ale_control_set(cpsw->ale, HOST_PORT_NUM,
			     ALE_PORT_STATE, ALE_PORT_STATE_FORWARD);

	if (!cpsw->data.dual_emac) {
		cpsw_ale_add_ucast(cpsw->ale, priv->mac_addr, HOST_PORT_NUM,
				   0, 0);
		cpsw_ale_add_mcast(cpsw->ale, priv->ndev->broadcast,
				   ALE_PORT_HOST, 0, 0, ALE_MCAST_FWD_2);
	}
}

static void cpsw_slave_stop(struct cpsw_slave *slave, struct cpsw_common *cpsw)
{
	u32 slave_port;

	slave_port = cpsw_get_slave_port(slave->slave_num);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!slave->phy)
		return;
	phy_stop(slave->phy);
	phy_disconnect(slave->phy);
	slave->phy = NULL;
<<<<<<< HEAD
=======
	cpsw_ale_control_set(cpsw->ale, slave_port,
			     ALE_PORT_STATE, ALE_PORT_STATE_DISABLE);
	cpsw_sl_reset(slave->mac_sl, 100);
	cpsw_sl_ctl_reset(slave->mac_sl);
}

static int cpsw_restore_vlans(struct net_device *vdev, int vid, void *arg)
{
	struct cpsw_priv *priv = arg;

	if (!vdev)
		return 0;

	cpsw_ndo_vlan_rx_add_vid(priv->ndev, 0, vid);
	return 0;
}

/* restore resources after port reset */
static void cpsw_restore(struct cpsw_priv *priv)
{
	/* restore vlan configurations */
	vlan_for_each(priv->ndev, cpsw_restore_vlans, priv);

	/* restore MQPRIO offload */
	for_each_slave(priv, cpsw_mqprio_resume, priv);

	/* restore CBS offload */
	for_each_slave(priv, cpsw_cbs_resume, priv);
}

static int cpsw_ndo_open(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	int ret;
	u32 reg;

	ret = pm_runtime_resume_and_get(cpsw->dev);
	if (ret < 0)
		return ret;

	netif_carrier_off(ndev);

	/* Notify the stack of the actual queue counts. */
	ret = netif_set_real_num_tx_queues(ndev, cpsw->tx_ch_num);
	if (ret) {
		dev_err(priv->dev, "cannot set real number of tx queues\n");
		goto err_cleanup;
	}

	ret = netif_set_real_num_rx_queues(ndev, cpsw->rx_ch_num);
	if (ret) {
		dev_err(priv->dev, "cannot set real number of rx queues\n");
		goto err_cleanup;
	}

	reg = cpsw->version;

	dev_info(priv->dev, "initializing cpsw version %d.%d (%d)\n",
		 CPSW_MAJOR_VERSION(reg), CPSW_MINOR_VERSION(reg),
		 CPSW_RTL_VERSION(reg));

	/* Initialize host and slave ports */
	if (!cpsw->usage_count)
		cpsw_init_host_port(priv);
	for_each_slave(priv, cpsw_slave_open, priv);

	/* Add default VLAN */
	if (!cpsw->data.dual_emac)
		cpsw_add_default_vlan(priv);
	else
		cpsw_ale_add_vlan(cpsw->ale, cpsw->data.default_vlan,
				  ALE_ALL_PORTS, ALE_ALL_PORTS, 0, 0);

	/* initialize shared resources for every ndev */
	if (!cpsw->usage_count) {
		/* disable priority elevation */
		writel_relaxed(0, &cpsw->regs->ptype);

		/* enable statistics collection only on all ports */
		writel_relaxed(0x7, &cpsw->regs->stat_port_en);

		/* Enable internal fifo flow control */
		writel(0x7, &cpsw->regs->flow_control);

		napi_enable(&cpsw->napi_rx);
		napi_enable(&cpsw->napi_tx);

		if (cpsw->tx_irq_disabled) {
			cpsw->tx_irq_disabled = false;
			enable_irq(cpsw->irqs_table[1]);
		}

		if (cpsw->rx_irq_disabled) {
			cpsw->rx_irq_disabled = false;
			enable_irq(cpsw->irqs_table[0]);
		}

		/* create rxqs for both infs in dual mac as they use same pool
		 * and must be destroyed together when no users.
		 */
		ret = cpsw_create_xdp_rxqs(cpsw);
		if (ret < 0)
			goto err_cleanup;

		ret = cpsw_fill_rx_channels(priv);
		if (ret < 0)
			goto err_cleanup;

		if (cpsw->cpts) {
			if (cpts_register(cpsw->cpts))
				dev_err(priv->dev, "error registering cpts device\n");
			else
				writel(0x10, &cpsw->wr_regs->misc_en);
		}
	}

	cpsw_restore(priv);

	/* Enable Interrupt pacing if configured */
	if (cpsw->coal_intvl != 0) {
		struct ethtool_coalesce coal;

		coal.rx_coalesce_usecs = cpsw->coal_intvl;
		cpsw_set_coalesce(ndev, &coal, NULL, NULL);
	}

	cpdma_ctlr_start(cpsw->dma);
	cpsw_intr_enable(cpsw);
	cpsw->usage_count++;

	return 0;

err_cleanup:
	if (!cpsw->usage_count) {
		napi_disable(&cpsw->napi_rx);
		napi_disable(&cpsw->napi_tx);
		cpdma_ctlr_stop(cpsw->dma);
		cpsw_destroy_xdp_rxqs(cpsw);
	}

	for_each_slave(priv, cpsw_slave_stop, cpsw);
	pm_runtime_put_sync(cpsw->dev);
	netif_carrier_off(priv->ndev);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cpsw_ndo_stop(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
<<<<<<< HEAD

	cpsw_info(priv, ifdown, "shutting down cpsw device\n");
	cpsw_intr_disable(priv);
	cpdma_ctlr_int_ctrl(priv->dma, false);
	cpdma_ctlr_stop(priv->dma);
	netif_stop_queue(priv->ndev);
	napi_disable(&priv->napi);
	netif_carrier_off(priv->ndev);
	cpsw_ale_stop(priv->ale);
	for_each_slave(priv, cpsw_slave_stop, priv);
	clk_disable(priv->clk);
=======
	struct cpsw_common *cpsw = priv->cpsw;

	cpsw_info(priv, ifdown, "shutting down cpsw device\n");
	__hw_addr_ref_unsync_dev(&ndev->mc, ndev, cpsw_purge_all_mc);
	netif_tx_stop_all_queues(priv->ndev);
	netif_carrier_off(priv->ndev);

	if (cpsw->usage_count <= 1) {
		napi_disable(&cpsw->napi_rx);
		napi_disable(&cpsw->napi_tx);
		cpts_unregister(cpsw->cpts);
		cpsw_intr_disable(cpsw);
		cpdma_ctlr_stop(cpsw->dma);
		cpsw_ale_stop(cpsw->ale);
		cpsw_destroy_xdp_rxqs(cpsw);
	}
	for_each_slave(priv, cpsw_slave_stop, cpsw);

	if (cpsw_need_resplit(cpsw))
		cpsw_split_res(cpsw);

	cpsw->usage_count--;
	pm_runtime_put_sync(cpsw->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static netdev_tx_t cpsw_ndo_start_xmit(struct sk_buff *skb,
				       struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
<<<<<<< HEAD
	int ret;

	ndev->trans_start = jiffies;

	if (skb_padto(skb, CPSW_MIN_PACKET_SIZE)) {
		cpsw_err(priv, tx_err, "packet pad failed\n");
		priv->stats.tx_dropped++;
		return NETDEV_TX_OK;
	}

	ret = cpdma_chan_submit(priv->txch, skb, skb->data,
				skb->len, GFP_KERNEL);
=======
	struct cpsw_common *cpsw = priv->cpsw;
	struct cpts *cpts = cpsw->cpts;
	struct netdev_queue *txq;
	struct cpdma_chan *txch;
	int ret, q_idx;

	if (skb_put_padto(skb, CPSW_MIN_PACKET_SIZE)) {
		cpsw_err(priv, tx_err, "packet pad failed\n");
		ndev->stats.tx_dropped++;
		return NET_XMIT_DROP;
	}

	if (skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP &&
	    priv->tx_ts_enabled && cpts_can_timestamp(cpts, skb))
		skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;

	q_idx = skb_get_queue_mapping(skb);
	if (q_idx >= cpsw->tx_ch_num)
		q_idx = q_idx % cpsw->tx_ch_num;

	txch = cpsw->txv[q_idx].ch;
	txq = netdev_get_tx_queue(ndev, q_idx);
	skb_tx_timestamp(skb);
	ret = cpdma_chan_submit(txch, skb, skb->data, skb->len,
				priv->emac_port + cpsw->data.dual_emac);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(ret != 0)) {
		cpsw_err(priv, tx_err, "desc submit failed\n");
		goto fail;
	}

<<<<<<< HEAD
	return NETDEV_TX_OK;
fail:
	priv->stats.tx_dropped++;
	netif_stop_queue(ndev);
	return NETDEV_TX_BUSY;
}

static void cpsw_ndo_change_rx_flags(struct net_device *ndev, int flags)
{
	/*
	 * The switch cannot operate in promiscuous mode without substantial
	 * headache.  For promiscuous mode to work, we would need to put the
	 * ALE in bypass mode and route all traffic to the host port.
	 * Subsequently, the host will need to operate as a "bridge", learn,
	 * and flood as needed.  For now, we simply complain here and
	 * do nothing about it :-)
	 */
	if ((flags & IFF_PROMISC) && (ndev->flags & IFF_PROMISC))
		dev_err(&ndev->dev, "promiscuity ignored!\n");

	/*
	 * The switch cannot filter multicast traffic unless it is configured
	 * in "VLAN Aware" mode.  Unfortunately, VLAN awareness requires a
	 * whole bunch of additional logic that this driver does not implement
	 * at present.
	 */
	if ((flags & IFF_ALLMULTI) && !(ndev->flags & IFF_ALLMULTI))
		dev_err(&ndev->dev, "multicast traffic cannot be filtered!\n");
}

static void cpsw_ndo_tx_timeout(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	cpsw_err(priv, tx_err, "transmit timeout, restarting dma\n");
	priv->stats.tx_errors++;
	cpsw_intr_disable(priv);
	cpdma_ctlr_int_ctrl(priv->dma, false);
	cpdma_chan_stop(priv->txch);
	cpdma_chan_start(priv->txch);
	cpdma_ctlr_int_ctrl(priv->dma, true);
	cpsw_intr_enable(priv);
	cpdma_ctlr_eoi(priv->dma);
}

static struct net_device_stats *cpsw_ndo_get_stats(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	return &priv->stats;
=======
	/* If there is no more tx desc left free then we need to
	 * tell the kernel to stop sending us tx frames.
	 */
	if (unlikely(!cpdma_check_free_tx_desc(txch))) {
		netif_tx_stop_queue(txq);

		/* Barrier, so that stop_queue visible to other cpus */
		smp_mb__after_atomic();

		if (cpdma_check_free_tx_desc(txch))
			netif_tx_wake_queue(txq);
	}

	return NETDEV_TX_OK;
fail:
	ndev->stats.tx_dropped++;
	netif_tx_stop_queue(txq);

	/* Barrier, so that stop_queue visible to other cpus */
	smp_mb__after_atomic();

	if (cpdma_check_free_tx_desc(txch))
		netif_tx_wake_queue(txq);

	return NETDEV_TX_BUSY;
}

static int cpsw_ndo_set_mac_address(struct net_device *ndev, void *p)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct sockaddr *addr = (struct sockaddr *)p;
	struct cpsw_common *cpsw = priv->cpsw;
	int flags = 0;
	u16 vid = 0;
	int ret;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	ret = pm_runtime_resume_and_get(cpsw->dev);
	if (ret < 0)
		return ret;

	if (cpsw->data.dual_emac) {
		vid = cpsw->slaves[priv->emac_port].port_vlan;
		flags = ALE_VLAN;
	}

	cpsw_ale_del_ucast(cpsw->ale, priv->mac_addr, HOST_PORT_NUM,
			   flags, vid);
	cpsw_ale_add_ucast(cpsw->ale, addr->sa_data, HOST_PORT_NUM,
			   flags, vid);

	memcpy(priv->mac_addr, addr->sa_data, ETH_ALEN);
	eth_hw_addr_set(ndev, priv->mac_addr);
	for_each_slave(priv, cpsw_set_slave_mac, priv);

	pm_runtime_put(cpsw->dev);

	return 0;
}

static inline int cpsw_add_vlan_ale_entry(struct cpsw_priv *priv,
				unsigned short vid)
{
	int ret;
	int unreg_mcast_mask = 0;
	int mcast_mask;
	u32 port_mask;
	struct cpsw_common *cpsw = priv->cpsw;

	if (cpsw->data.dual_emac) {
		port_mask = (1 << (priv->emac_port + 1)) | ALE_PORT_HOST;

		mcast_mask = ALE_PORT_HOST;
		if (priv->ndev->flags & IFF_ALLMULTI)
			unreg_mcast_mask = mcast_mask;
	} else {
		port_mask = ALE_ALL_PORTS;
		mcast_mask = port_mask;

		if (priv->ndev->flags & IFF_ALLMULTI)
			unreg_mcast_mask = ALE_ALL_PORTS;
		else
			unreg_mcast_mask = ALE_PORT_1 | ALE_PORT_2;
	}

	ret = cpsw_ale_add_vlan(cpsw->ale, vid, port_mask, 0, port_mask,
				unreg_mcast_mask);
	if (ret != 0)
		return ret;

	ret = cpsw_ale_add_ucast(cpsw->ale, priv->mac_addr,
				 HOST_PORT_NUM, ALE_VLAN, vid);
	if (ret != 0)
		goto clean_vid;

	ret = cpsw_ale_add_mcast(cpsw->ale, priv->ndev->broadcast,
				 mcast_mask, ALE_VLAN, vid, 0);
	if (ret != 0)
		goto clean_vlan_ucast;
	return 0;

clean_vlan_ucast:
	cpsw_ale_del_ucast(cpsw->ale, priv->mac_addr,
			   HOST_PORT_NUM, ALE_VLAN, vid);
clean_vid:
	cpsw_ale_del_vlan(cpsw->ale, vid, 0);
	return ret;
}

static int cpsw_ndo_vlan_rx_add_vid(struct net_device *ndev,
				    __be16 proto, u16 vid)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	int ret;

	if (vid == cpsw->data.default_vlan)
		return 0;

	ret = pm_runtime_resume_and_get(cpsw->dev);
	if (ret < 0)
		return ret;

	if (cpsw->data.dual_emac) {
		/* In dual EMAC, reserved VLAN id should not be used for
		 * creating VLAN interfaces as this can break the dual
		 * EMAC port separation
		 */
		int i;

		for (i = 0; i < cpsw->data.slaves; i++) {
			if (vid == cpsw->slaves[i].port_vlan) {
				ret = -EINVAL;
				goto err;
			}
		}
	}

	dev_info(priv->dev, "Adding vlanid %d to vlan filter\n", vid);
	ret = cpsw_add_vlan_ale_entry(priv, vid);
err:
	pm_runtime_put(cpsw->dev);
	return ret;
}

static int cpsw_ndo_vlan_rx_kill_vid(struct net_device *ndev,
				     __be16 proto, u16 vid)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	int ret;

	if (vid == cpsw->data.default_vlan)
		return 0;

	ret = pm_runtime_resume_and_get(cpsw->dev);
	if (ret < 0)
		return ret;

	if (cpsw->data.dual_emac) {
		int i;

		for (i = 0; i < cpsw->data.slaves; i++) {
			if (vid == cpsw->slaves[i].port_vlan)
				goto err;
		}
	}

	dev_info(priv->dev, "removing vlanid %d from vlan filter\n", vid);
	ret = cpsw_ale_del_vlan(cpsw->ale, vid, 0);
	ret |= cpsw_ale_del_ucast(cpsw->ale, priv->mac_addr,
				  HOST_PORT_NUM, ALE_VLAN, vid);
	ret |= cpsw_ale_del_mcast(cpsw->ale, priv->ndev->broadcast,
				  0, ALE_VLAN, vid);
	ret |= cpsw_ale_flush_multicast(cpsw->ale, ALE_PORT_HOST, vid);
err:
	pm_runtime_put(cpsw->dev);
	return ret;
}

static int cpsw_ndo_xdp_xmit(struct net_device *ndev, int n,
			     struct xdp_frame **frames, u32 flags)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_common *cpsw = priv->cpsw;
	struct xdp_frame *xdpf;
	int i, nxmit = 0, port;

	if (unlikely(flags & ~XDP_XMIT_FLAGS_MASK))
		return -EINVAL;

	for (i = 0; i < n; i++) {
		xdpf = frames[i];
		if (xdpf->len < CPSW_MIN_PACKET_SIZE)
			break;

		port = priv->emac_port + cpsw->data.dual_emac;
		if (cpsw_xdp_tx_frame(priv, xdpf, NULL, port))
			break;
		nxmit++;
	}

	return nxmit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void cpsw_ndo_poll_controller(struct net_device *ndev)
{
<<<<<<< HEAD
	struct cpsw_priv *priv = netdev_priv(ndev);

	cpsw_intr_disable(priv);
	cpdma_ctlr_int_ctrl(priv->dma, false);
	cpsw_interrupt(ndev->irq, priv);
	cpdma_ctlr_int_ctrl(priv->dma, true);
	cpsw_intr_enable(priv);
	cpdma_ctlr_eoi(priv->dma);
=======
	struct cpsw_common *cpsw = ndev_to_cpsw(ndev);

	cpsw_intr_disable(cpsw);
	cpsw_rx_interrupt(cpsw->irqs_table[0], cpsw);
	cpsw_tx_interrupt(cpsw->irqs_table[1], cpsw);
	cpsw_intr_enable(cpsw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static const struct net_device_ops cpsw_netdev_ops = {
	.ndo_open		= cpsw_ndo_open,
	.ndo_stop		= cpsw_ndo_stop,
	.ndo_start_xmit		= cpsw_ndo_start_xmit,
<<<<<<< HEAD
	.ndo_change_rx_flags	= cpsw_ndo_change_rx_flags,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_tx_timeout		= cpsw_ndo_tx_timeout,
	.ndo_get_stats		= cpsw_ndo_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cpsw_ndo_poll_controller,
#endif
=======
	.ndo_set_mac_address	= cpsw_ndo_set_mac_address,
	.ndo_eth_ioctl		= cpsw_ndo_ioctl,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_tx_timeout		= cpsw_ndo_tx_timeout,
	.ndo_set_rx_mode	= cpsw_ndo_set_rx_mode,
	.ndo_set_tx_maxrate	= cpsw_ndo_set_tx_maxrate,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cpsw_ndo_poll_controller,
#endif
	.ndo_vlan_rx_add_vid	= cpsw_ndo_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= cpsw_ndo_vlan_rx_kill_vid,
	.ndo_setup_tc           = cpsw_ndo_setup_tc,
	.ndo_bpf		= cpsw_ndo_bpf,
	.ndo_xdp_xmit		= cpsw_ndo_xdp_xmit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void cpsw_get_drvinfo(struct net_device *ndev,
			     struct ethtool_drvinfo *info)
{
<<<<<<< HEAD
	struct cpsw_priv *priv = netdev_priv(ndev);
	strcpy(info->driver, "TI CPSW Driver v1.0");
	strcpy(info->version, "1.0");
	strcpy(info->bus_info, priv->pdev->name);
}

static u32 cpsw_get_msglevel(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	return priv->msg_enable;
}

static void cpsw_set_msglevel(struct net_device *ndev, u32 value)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	priv->msg_enable = value;
}

static const struct ethtool_ops cpsw_ethtool_ops = {
=======
	struct cpsw_common *cpsw = ndev_to_cpsw(ndev);
	struct platform_device	*pdev = to_platform_device(cpsw->dev);

	strscpy(info->driver, "cpsw", sizeof(info->driver));
	strscpy(info->version, "1.0", sizeof(info->version));
	strscpy(info->bus_info, pdev->name, sizeof(info->bus_info));
}

static int cpsw_set_pauseparam(struct net_device *ndev,
			       struct ethtool_pauseparam *pause)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	bool link;

	priv->rx_pause = pause->rx_pause ? true : false;
	priv->tx_pause = pause->tx_pause ? true : false;

	for_each_slave(priv, _cpsw_adjust_link, priv, &link);
	return 0;
}

static int cpsw_set_channels(struct net_device *ndev,
			     struct ethtool_channels *chs)
{
	return cpsw_set_channels_common(ndev, chs, cpsw_rx_handler);
}

static const struct ethtool_ops cpsw_ethtool_ops = {
	.supported_coalesce_params = ETHTOOL_COALESCE_RX_USECS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_drvinfo	= cpsw_get_drvinfo,
	.get_msglevel	= cpsw_get_msglevel,
	.set_msglevel	= cpsw_set_msglevel,
	.get_link	= ethtool_op_get_link,
<<<<<<< HEAD
};

static void cpsw_slave_init(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
	void __iomem		*regs = priv->regs;
	int			slave_num = slave->slave_num;
	struct cpsw_slave_data	*data = priv->data.slave_data + slave_num;

	slave->data	= data;
	slave->regs	= regs + data->slave_reg_ofs;
	slave->sliver	= regs + data->sliver_reg_ofs;
}

static int __devinit cpsw_probe(struct platform_device *pdev)
{
	struct cpsw_platform_data	*data = pdev->dev.platform_data;
	struct net_device		*ndev;
	struct cpsw_priv		*priv;
	struct cpdma_params		dma_params;
	struct cpsw_ale_params		ale_params;
	void __iomem			*regs;
	struct resource			*res;
	int ret = 0, i, k = 0;

	if (!data) {
		pr_err("platform data missing\n");
		return -ENODEV;
	}

	ndev = alloc_etherdev(sizeof(struct cpsw_priv));
	if (!ndev) {
		pr_err("error allocating net_device\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, ndev);
	priv = netdev_priv(ndev);
	spin_lock_init(&priv->lock);
	priv->data = *data;
	priv->pdev = pdev;
	priv->ndev = ndev;
	priv->dev  = &ndev->dev;
	priv->msg_enable = netif_msg_init(debug_level, CPSW_DEBUG);
	priv->rx_packet_max = max(rx_packet_max, 128);

	if (is_valid_ether_addr(data->slave_data[0].mac_addr)) {
		memcpy(priv->mac_addr, data->slave_data[0].mac_addr, ETH_ALEN);
		pr_info("Detected MACID = %pM", priv->mac_addr);
	} else {
		random_ether_addr(priv->mac_addr);
		pr_info("Random MACID = %pM", priv->mac_addr);
	}

	memcpy(ndev->dev_addr, priv->mac_addr, ETH_ALEN);

	priv->slaves = kzalloc(sizeof(struct cpsw_slave) * data->slaves,
			       GFP_KERNEL);
	if (!priv->slaves) {
		ret = -EBUSY;
		goto clean_ndev_ret;
	}
	for (i = 0; i < data->slaves; i++)
		priv->slaves[i].slave_num = i;

	priv->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(priv->dev, "failed to get device clock)\n");
		ret = -EBUSY;
	}

	priv->cpsw_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!priv->cpsw_res) {
		dev_err(priv->dev, "error getting i/o resource\n");
		ret = -ENOENT;
		goto clean_clk_ret;
	}

	if (!request_mem_region(priv->cpsw_res->start,
				resource_size(priv->cpsw_res), ndev->name)) {
		dev_err(priv->dev, "failed request i/o region\n");
		ret = -ENXIO;
		goto clean_clk_ret;
	}

	regs = ioremap(priv->cpsw_res->start, resource_size(priv->cpsw_res));
	if (!regs) {
		dev_err(priv->dev, "unable to map i/o region\n");
		goto clean_cpsw_iores_ret;
	}
	priv->regs = regs;
	priv->host_port = data->host_port_num;
	priv->host_port_regs = regs + data->host_port_reg_ofs;

	priv->cpsw_ss_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!priv->cpsw_ss_res) {
		dev_err(priv->dev, "error getting i/o resource\n");
		ret = -ENOENT;
		goto clean_clk_ret;
	}

	if (!request_mem_region(priv->cpsw_ss_res->start,
			resource_size(priv->cpsw_ss_res), ndev->name)) {
		dev_err(priv->dev, "failed request i/o region\n");
		ret = -ENXIO;
		goto clean_clk_ret;
	}

	regs = ioremap(priv->cpsw_ss_res->start,
				resource_size(priv->cpsw_ss_res));
	if (!regs) {
		dev_err(priv->dev, "unable to map i/o region\n");
		goto clean_cpsw_ss_iores_ret;
	}
	priv->ss_regs = regs;

	for_each_slave(priv, cpsw_slave_init, priv);

	memset(&dma_params, 0, sizeof(dma_params));
	dma_params.dev		= &pdev->dev;
	dma_params.dmaregs	= cpsw_dma_regs((u32)priv->regs,
						data->cpdma_reg_ofs);
	dma_params.rxthresh	= cpsw_dma_rxthresh((u32)priv->regs,
						    data->cpdma_reg_ofs);
	dma_params.rxfree	= cpsw_dma_rxfree((u32)priv->regs,
						  data->cpdma_reg_ofs);
	dma_params.txhdp	= cpsw_dma_txhdp((u32)priv->regs,
						 data->cpdma_sram_ofs);
	dma_params.rxhdp	= cpsw_dma_rxhdp((u32)priv->regs,
						 data->cpdma_sram_ofs);
	dma_params.txcp		= cpsw_dma_txcp((u32)priv->regs,
						data->cpdma_sram_ofs);
	dma_params.rxcp		= cpsw_dma_rxcp((u32)priv->regs,
						data->cpdma_sram_ofs);

	dma_params.num_chan		= data->channels;
	dma_params.has_soft_reset	= true;
	dma_params.min_packet_size	= CPSW_MIN_PACKET_SIZE;
	dma_params.desc_mem_size	= data->bd_ram_size;
	dma_params.desc_align		= 16;
	dma_params.has_ext_regs		= true;
	dma_params.desc_mem_phys        = data->no_bd_ram ? 0 :
			(u32 __force)priv->cpsw_res->start + data->bd_ram_ofs;
	dma_params.desc_hw_addr         = data->hw_ram_addr ?
			data->hw_ram_addr : dma_params.desc_mem_phys ;

	priv->dma = cpdma_ctlr_create(&dma_params);
	if (!priv->dma) {
		dev_err(priv->dev, "error initializing dma\n");
		ret = -ENOMEM;
		goto clean_iomap_ret;
	}

	priv->txch = cpdma_chan_create(priv->dma, tx_chan_num(0),
				       cpsw_tx_handler);
	priv->rxch = cpdma_chan_create(priv->dma, rx_chan_num(0),
				       cpsw_rx_handler);

	if (WARN_ON(!priv->txch || !priv->rxch)) {
		dev_err(priv->dev, "error initializing dma channels\n");
		ret = -ENOMEM;
		goto clean_dma_ret;
	}

	memset(&ale_params, 0, sizeof(ale_params));
	ale_params.dev			= &ndev->dev;
	ale_params.ale_regs		= (void *)((u32)priv->regs) +
						((u32)data->ale_reg_ofs);
	ale_params.ale_ageout		= ale_ageout;
	ale_params.ale_entries		= data->ale_entries;
	ale_params.ale_ports		= data->slaves;

	priv->ale = cpsw_ale_create(&ale_params);
	if (!priv->ale) {
		dev_err(priv->dev, "error initializing ale engine\n");
		ret = -ENODEV;
		goto clean_dma_ret;
	}

	ndev->irq = platform_get_irq(pdev, 0);
	if (ndev->irq < 0) {
		dev_err(priv->dev, "error getting irq resource\n");
		ret = -ENOENT;
		goto clean_ale_ret;
	}

	while ((res = platform_get_resource(priv->pdev, IORESOURCE_IRQ, k))) {
		for (i = res->start; i <= res->end; i++) {
			if (request_irq(i, cpsw_interrupt, IRQF_DISABLED,
					dev_name(&pdev->dev), priv)) {
				dev_err(priv->dev, "error attaching irq\n");
				goto clean_ale_ret;
			}
			priv->irqs_table[k] = i;
			priv->num_irqs = k;
		}
		k++;
	}

	ndev->flags |= IFF_ALLMULTI;	/* see cpsw_ndo_change_rx_flags() */

	ndev->netdev_ops = &cpsw_netdev_ops;
	SET_ETHTOOL_OPS(ndev, &cpsw_ethtool_ops);
	netif_napi_add(ndev, &priv->napi, cpsw_poll, CPSW_POLL_WEIGHT);

	/* register the network device */
	SET_NETDEV_DEV(ndev, &pdev->dev);
	ret = register_netdev(ndev);
	if (ret) {
		dev_err(priv->dev, "error registering net device\n");
		ret = -ENODEV;
		goto clean_irq_ret;
	}

	cpsw_notice(priv, probe, "initialized device (regs %x, irq %d)\n",
		  priv->cpsw_res->start, ndev->irq);

	return 0;

clean_irq_ret:
	free_irq(ndev->irq, priv);
clean_ale_ret:
	cpsw_ale_destroy(priv->ale);
clean_dma_ret:
	cpdma_chan_destroy(priv->txch);
	cpdma_chan_destroy(priv->rxch);
	cpdma_ctlr_destroy(priv->dma);
clean_iomap_ret:
	iounmap(priv->regs);
clean_cpsw_ss_iores_ret:
	release_mem_region(priv->cpsw_ss_res->start,
			   resource_size(priv->cpsw_ss_res));
clean_cpsw_iores_ret:
	release_mem_region(priv->cpsw_res->start,
			   resource_size(priv->cpsw_res));
clean_clk_ret:
	clk_put(priv->clk);
	kfree(priv->slaves);
clean_ndev_ret:
	free_netdev(ndev);
	return ret;
}

static int __devexit cpsw_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct cpsw_priv *priv = netdev_priv(ndev);

	pr_info("removing device");
	platform_set_drvdata(pdev, NULL);

	free_irq(ndev->irq, priv);
	cpsw_ale_destroy(priv->ale);
	cpdma_chan_destroy(priv->txch);
	cpdma_chan_destroy(priv->rxch);
	cpdma_ctlr_destroy(priv->dma);
	iounmap(priv->regs);
	release_mem_region(priv->cpsw_res->start,
			   resource_size(priv->cpsw_res));
	release_mem_region(priv->cpsw_ss_res->start,
			   resource_size(priv->cpsw_ss_res));
	clk_put(priv->clk);
	kfree(priv->slaves);
	free_netdev(ndev);

	return 0;
}

static int cpsw_suspend(struct device *dev)
{
	struct platform_device	*pdev = to_platform_device(dev);
	struct net_device	*ndev = platform_get_drvdata(pdev);

	if (netif_running(ndev))
		cpsw_ndo_stop(ndev);
=======
	.get_ts_info	= cpsw_get_ts_info,
	.get_coalesce	= cpsw_get_coalesce,
	.set_coalesce	= cpsw_set_coalesce,
	.get_sset_count		= cpsw_get_sset_count,
	.get_strings		= cpsw_get_strings,
	.get_ethtool_stats	= cpsw_get_ethtool_stats,
	.get_pauseparam		= cpsw_get_pauseparam,
	.set_pauseparam		= cpsw_set_pauseparam,
	.get_wol	= cpsw_get_wol,
	.set_wol	= cpsw_set_wol,
	.get_regs_len	= cpsw_get_regs_len,
	.get_regs	= cpsw_get_regs,
	.begin		= cpsw_ethtool_op_begin,
	.complete	= cpsw_ethtool_op_complete,
	.get_channels	= cpsw_get_channels,
	.set_channels	= cpsw_set_channels,
	.get_link_ksettings	= cpsw_get_link_ksettings,
	.set_link_ksettings	= cpsw_set_link_ksettings,
	.get_eee	= cpsw_get_eee,
	.set_eee	= cpsw_set_eee,
	.nway_reset	= cpsw_nway_reset,
	.get_ringparam = cpsw_get_ringparam,
	.set_ringparam = cpsw_set_ringparam,
};

static int cpsw_probe_dt(struct cpsw_platform_data *data,
			 struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct device_node *slave_node;
	int i = 0, ret;
	u32 prop;

	if (!node)
		return -EINVAL;

	if (of_property_read_u32(node, "slaves", &prop)) {
		dev_err(&pdev->dev, "Missing slaves property in the DT.\n");
		return -EINVAL;
	}
	data->slaves = prop;

	if (of_property_read_u32(node, "active_slave", &prop)) {
		dev_err(&pdev->dev, "Missing active_slave property in the DT.\n");
		return -EINVAL;
	}
	data->active_slave = prop;

	data->slave_data = devm_kcalloc(&pdev->dev,
					data->slaves,
					sizeof(struct cpsw_slave_data),
					GFP_KERNEL);
	if (!data->slave_data)
		return -ENOMEM;

	if (of_property_read_u32(node, "cpdma_channels", &prop)) {
		dev_err(&pdev->dev, "Missing cpdma_channels property in the DT.\n");
		return -EINVAL;
	}
	data->channels = prop;

	if (of_property_read_u32(node, "bd_ram_size", &prop)) {
		dev_err(&pdev->dev, "Missing bd_ram_size property in the DT.\n");
		return -EINVAL;
	}
	data->bd_ram_size = prop;

	if (of_property_read_u32(node, "mac_control", &prop)) {
		dev_err(&pdev->dev, "Missing mac_control property in the DT.\n");
		return -EINVAL;
	}
	data->mac_control = prop;

	if (of_property_read_bool(node, "dual_emac"))
		data->dual_emac = true;

	/*
	 * Populate all the child nodes here...
	 */
	ret = of_platform_populate(node, NULL, NULL, &pdev->dev);
	/* We do not want to force this, as in some cases may not have child */
	if (ret)
		dev_warn(&pdev->dev, "Doesn't have any child node\n");

	for_each_available_child_of_node(node, slave_node) {
		struct cpsw_slave_data *slave_data = data->slave_data + i;
		int lenp;
		const __be32 *parp;

		/* This is no slave child node, continue */
		if (!of_node_name_eq(slave_node, "slave"))
			continue;

		slave_data->ifphy = devm_of_phy_get(&pdev->dev, slave_node,
						    NULL);
		if (!IS_ENABLED(CONFIG_TI_CPSW_PHY_SEL) &&
		    IS_ERR(slave_data->ifphy)) {
			ret = PTR_ERR(slave_data->ifphy);
			dev_err(&pdev->dev,
				"%d: Error retrieving port phy: %d\n", i, ret);
			goto err_node_put;
		}

		slave_data->slave_node = slave_node;
		slave_data->phy_node = of_parse_phandle(slave_node,
							"phy-handle", 0);
		parp = of_get_property(slave_node, "phy_id", &lenp);
		if (slave_data->phy_node) {
			dev_dbg(&pdev->dev,
				"slave[%d] using phy-handle=\"%pOF\"\n",
				i, slave_data->phy_node);
		} else if (of_phy_is_fixed_link(slave_node)) {
			/* In the case of a fixed PHY, the DT node associated
			 * to the PHY is the Ethernet MAC DT node.
			 */
			ret = of_phy_register_fixed_link(slave_node);
			if (ret) {
				dev_err_probe(&pdev->dev, ret, "failed to register fixed-link phy\n");
				goto err_node_put;
			}
			slave_data->phy_node = of_node_get(slave_node);
		} else if (parp) {
			u32 phyid;
			struct device_node *mdio_node;
			struct platform_device *mdio;

			if (lenp != (sizeof(__be32) * 2)) {
				dev_err(&pdev->dev, "Invalid slave[%d] phy_id property\n", i);
				goto no_phy_slave;
			}
			mdio_node = of_find_node_by_phandle(be32_to_cpup(parp));
			phyid = be32_to_cpup(parp+1);
			mdio = of_find_device_by_node(mdio_node);
			of_node_put(mdio_node);
			if (!mdio) {
				dev_err(&pdev->dev, "Missing mdio platform device\n");
				ret = -EINVAL;
				goto err_node_put;
			}
			snprintf(slave_data->phy_id, sizeof(slave_data->phy_id),
				 PHY_ID_FMT, mdio->name, phyid);
			put_device(&mdio->dev);
		} else {
			dev_err(&pdev->dev,
				"No slave[%d] phy_id, phy-handle, or fixed-link property\n",
				i);
			goto no_phy_slave;
		}
		ret = of_get_phy_mode(slave_node, &slave_data->phy_if);
		if (ret) {
			dev_err(&pdev->dev, "Missing or malformed slave[%d] phy-mode property\n",
				i);
			goto err_node_put;
		}

no_phy_slave:
		ret = of_get_mac_address(slave_node, slave_data->mac_addr);
		if (ret) {
			ret = ti_cm_get_macid(&pdev->dev, i,
					      slave_data->mac_addr);
			if (ret)
				goto err_node_put;
		}
		if (data->dual_emac) {
			if (of_property_read_u32(slave_node, "dual_emac_res_vlan",
						 &prop)) {
				dev_err(&pdev->dev, "Missing dual_emac_res_vlan in DT.\n");
				slave_data->dual_emac_res_vlan = i+1;
				dev_err(&pdev->dev, "Using %d as Reserved VLAN for %d slave\n",
					slave_data->dual_emac_res_vlan, i);
			} else {
				slave_data->dual_emac_res_vlan = prop;
			}
		}

		i++;
		if (i == data->slaves) {
			ret = 0;
			goto err_node_put;
		}
	}

	return 0;

err_node_put:
	of_node_put(slave_node);
	return ret;
}

static void cpsw_remove_dt(struct platform_device *pdev)
{
	struct cpsw_common *cpsw = platform_get_drvdata(pdev);
	struct cpsw_platform_data *data = &cpsw->data;
	struct device_node *node = pdev->dev.of_node;
	struct device_node *slave_node;
	int i = 0;

	for_each_available_child_of_node(node, slave_node) {
		struct cpsw_slave_data *slave_data = &data->slave_data[i];

		if (!of_node_name_eq(slave_node, "slave"))
			continue;

		if (of_phy_is_fixed_link(slave_node))
			of_phy_deregister_fixed_link(slave_node);

		of_node_put(slave_data->phy_node);

		i++;
		if (i == data->slaves) {
			of_node_put(slave_node);
			break;
		}
	}

	of_platform_depopulate(&pdev->dev);
}

static int cpsw_probe_dual_emac(struct cpsw_priv *priv)
{
	struct cpsw_common		*cpsw = priv->cpsw;
	struct cpsw_platform_data	*data = &cpsw->data;
	struct net_device		*ndev;
	struct cpsw_priv		*priv_sl2;
	int ret = 0;

	ndev = devm_alloc_etherdev_mqs(cpsw->dev, sizeof(struct cpsw_priv),
				       CPSW_MAX_QUEUES, CPSW_MAX_QUEUES);
	if (!ndev) {
		dev_err(cpsw->dev, "cpsw: error allocating net_device\n");
		return -ENOMEM;
	}

	priv_sl2 = netdev_priv(ndev);
	priv_sl2->cpsw = cpsw;
	priv_sl2->ndev = ndev;
	priv_sl2->dev  = &ndev->dev;
	priv_sl2->msg_enable = netif_msg_init(debug_level, CPSW_DEBUG);

	if (is_valid_ether_addr(data->slave_data[1].mac_addr)) {
		memcpy(priv_sl2->mac_addr, data->slave_data[1].mac_addr,
			ETH_ALEN);
		dev_info(cpsw->dev, "cpsw: Detected MACID = %pM\n",
			 priv_sl2->mac_addr);
	} else {
		eth_random_addr(priv_sl2->mac_addr);
		dev_info(cpsw->dev, "cpsw: Random MACID = %pM\n",
			 priv_sl2->mac_addr);
	}
	eth_hw_addr_set(ndev, priv_sl2->mac_addr);

	priv_sl2->emac_port = 1;
	cpsw->slaves[1].ndev = ndev;
	ndev->features |= NETIF_F_HW_VLAN_CTAG_FILTER | NETIF_F_HW_VLAN_CTAG_RX;
	ndev->xdp_features = NETDEV_XDP_ACT_BASIC | NETDEV_XDP_ACT_REDIRECT |
			     NETDEV_XDP_ACT_NDO_XMIT;

	ndev->netdev_ops = &cpsw_netdev_ops;
	ndev->ethtool_ops = &cpsw_ethtool_ops;

	/* register the network device */
	SET_NETDEV_DEV(ndev, cpsw->dev);
	ndev->dev.of_node = cpsw->slaves[1].data->slave_node;
	ret = register_netdev(ndev);
	if (ret)
		dev_err(cpsw->dev, "cpsw: error registering net device\n");

	return ret;
}

static const struct of_device_id cpsw_of_mtable[] = {
	{ .compatible = "ti,cpsw"},
	{ .compatible = "ti,am335x-cpsw"},
	{ .compatible = "ti,am4372-cpsw"},
	{ .compatible = "ti,dra7-cpsw"},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, cpsw_of_mtable);

static const struct soc_device_attribute cpsw_soc_devices[] = {
	{ .family = "AM33xx", .revision = "ES1.0"},
	{ /* sentinel */ }
};

static int cpsw_probe(struct platform_device *pdev)
{
	struct device			*dev = &pdev->dev;
	struct clk			*clk;
	struct cpsw_platform_data	*data;
	struct net_device		*ndev;
	struct cpsw_priv		*priv;
	void __iomem			*ss_regs;
	struct resource			*ss_res;
	struct gpio_descs		*mode;
	const struct soc_device_attribute *soc;
	struct cpsw_common		*cpsw;
	int ret = 0, ch;
	int irq;

	cpsw = devm_kzalloc(dev, sizeof(struct cpsw_common), GFP_KERNEL);
	if (!cpsw)
		return -ENOMEM;

	platform_set_drvdata(pdev, cpsw);
	cpsw_slave_index = cpsw_slave_index_priv;

	cpsw->dev = dev;

	mode = devm_gpiod_get_array_optional(dev, "mode", GPIOD_OUT_LOW);
	if (IS_ERR(mode)) {
		ret = PTR_ERR(mode);
		dev_err(dev, "gpio request failed, ret %d\n", ret);
		return ret;
	}

	clk = devm_clk_get(dev, "fck");
	if (IS_ERR(clk)) {
		ret = PTR_ERR(clk);
		dev_err(dev, "fck is not found %d\n", ret);
		return ret;
	}
	cpsw->bus_freq_mhz = clk_get_rate(clk) / 1000000;

	ss_regs = devm_platform_get_and_ioremap_resource(pdev, 0, &ss_res);
	if (IS_ERR(ss_regs))
		return PTR_ERR(ss_regs);
	cpsw->regs = ss_regs;

	cpsw->wr_regs = devm_platform_ioremap_resource(pdev, 1);
	if (IS_ERR(cpsw->wr_regs))
		return PTR_ERR(cpsw->wr_regs);

	/* RX IRQ */
	irq = platform_get_irq(pdev, 1);
	if (irq < 0)
		return irq;
	cpsw->irqs_table[0] = irq;

	/* TX IRQ */
	irq = platform_get_irq(pdev, 2);
	if (irq < 0)
		return irq;
	cpsw->irqs_table[1] = irq;

	/* get misc irq*/
	irq = platform_get_irq(pdev, 3);
	if (irq <= 0)
		return irq;
	cpsw->misc_irq = irq;

	/*
	 * This may be required here for child devices.
	 */
	pm_runtime_enable(dev);

	/* Need to enable clocks with runtime PM api to access module
	 * registers
	 */
	ret = pm_runtime_resume_and_get(dev);
	if (ret < 0)
		goto clean_runtime_disable_ret;

	ret = cpsw_probe_dt(&cpsw->data, pdev);
	if (ret)
		goto clean_dt_ret;

	soc = soc_device_match(cpsw_soc_devices);
	if (soc)
		cpsw->quirk_irq = true;

	data = &cpsw->data;
	cpsw->slaves = devm_kcalloc(dev,
				    data->slaves, sizeof(struct cpsw_slave),
				    GFP_KERNEL);
	if (!cpsw->slaves) {
		ret = -ENOMEM;
		goto clean_dt_ret;
	}

	cpsw->rx_packet_max = max(rx_packet_max, CPSW_MAX_PACKET_SIZE);
	cpsw->descs_pool_size = descs_pool_size;

	ret = cpsw_init_common(cpsw, ss_regs, ale_ageout,
			       ss_res->start + CPSW2_BD_OFFSET,
			       descs_pool_size);
	if (ret)
		goto clean_dt_ret;

	ch = cpsw->quirk_irq ? 0 : 7;
	cpsw->txv[0].ch = cpdma_chan_create(cpsw->dma, ch, cpsw_tx_handler, 0);
	if (IS_ERR(cpsw->txv[0].ch)) {
		dev_err(dev, "error initializing tx dma channel\n");
		ret = PTR_ERR(cpsw->txv[0].ch);
		goto clean_cpts;
	}

	cpsw->rxv[0].ch = cpdma_chan_create(cpsw->dma, 0, cpsw_rx_handler, 1);
	if (IS_ERR(cpsw->rxv[0].ch)) {
		dev_err(dev, "error initializing rx dma channel\n");
		ret = PTR_ERR(cpsw->rxv[0].ch);
		goto clean_cpts;
	}
	cpsw_split_res(cpsw);

	/* setup netdev */
	ndev = devm_alloc_etherdev_mqs(dev, sizeof(struct cpsw_priv),
				       CPSW_MAX_QUEUES, CPSW_MAX_QUEUES);
	if (!ndev) {
		dev_err(dev, "error allocating net_device\n");
		ret = -ENOMEM;
		goto clean_cpts;
	}

	priv = netdev_priv(ndev);
	priv->cpsw = cpsw;
	priv->ndev = ndev;
	priv->dev  = dev;
	priv->msg_enable = netif_msg_init(debug_level, CPSW_DEBUG);
	priv->emac_port = 0;

	if (is_valid_ether_addr(data->slave_data[0].mac_addr)) {
		memcpy(priv->mac_addr, data->slave_data[0].mac_addr, ETH_ALEN);
		dev_info(dev, "Detected MACID = %pM\n", priv->mac_addr);
	} else {
		eth_random_addr(priv->mac_addr);
		dev_info(dev, "Random MACID = %pM\n", priv->mac_addr);
	}

	eth_hw_addr_set(ndev, priv->mac_addr);

	cpsw->slaves[0].ndev = ndev;

	ndev->features |= NETIF_F_HW_VLAN_CTAG_FILTER | NETIF_F_HW_VLAN_CTAG_RX;
	ndev->xdp_features = NETDEV_XDP_ACT_BASIC | NETDEV_XDP_ACT_REDIRECT |
			     NETDEV_XDP_ACT_NDO_XMIT;

	ndev->netdev_ops = &cpsw_netdev_ops;
	ndev->ethtool_ops = &cpsw_ethtool_ops;
	netif_napi_add(ndev, &cpsw->napi_rx,
		       cpsw->quirk_irq ? cpsw_rx_poll : cpsw_rx_mq_poll);
	netif_napi_add_tx(ndev, &cpsw->napi_tx,
			  cpsw->quirk_irq ? cpsw_tx_poll : cpsw_tx_mq_poll);

	/* register the network device */
	SET_NETDEV_DEV(ndev, dev);
	ndev->dev.of_node = cpsw->slaves[0].data->slave_node;
	ret = register_netdev(ndev);
	if (ret) {
		dev_err(dev, "error registering net device\n");
		ret = -ENODEV;
		goto clean_cpts;
	}

	if (cpsw->data.dual_emac) {
		ret = cpsw_probe_dual_emac(priv);
		if (ret) {
			cpsw_err(priv, probe, "error probe slave 2 emac interface\n");
			goto clean_unregister_netdev_ret;
		}
	}

	/* Grab RX and TX IRQs. Note that we also have RX_THRESHOLD and
	 * MISC IRQs which are always kept disabled with this driver so
	 * we will not request them.
	 *
	 * If anyone wants to implement support for those, make sure to
	 * first request and append them to irqs_table array.
	 */
	ret = devm_request_irq(dev, cpsw->irqs_table[0], cpsw_rx_interrupt,
			       0, dev_name(dev), cpsw);
	if (ret < 0) {
		dev_err(dev, "error attaching irq (%d)\n", ret);
		goto clean_unregister_netdev_ret;
	}


	ret = devm_request_irq(dev, cpsw->irqs_table[1], cpsw_tx_interrupt,
			       0, dev_name(&pdev->dev), cpsw);
	if (ret < 0) {
		dev_err(dev, "error attaching irq (%d)\n", ret);
		goto clean_unregister_netdev_ret;
	}

	if (!cpsw->cpts)
		goto skip_cpts;

	ret = devm_request_irq(&pdev->dev, cpsw->misc_irq, cpsw_misc_interrupt,
			       0, dev_name(&pdev->dev), cpsw);
	if (ret < 0) {
		dev_err(dev, "error attaching misc irq (%d)\n", ret);
		goto clean_unregister_netdev_ret;
	}

	/* Enable misc CPTS evnt_pend IRQ */
	cpts_set_irqpoll(cpsw->cpts, false);

skip_cpts:
	cpsw_notice(priv, probe,
		    "initialized device (regs %pa, irq %d, pool size %d)\n",
		    &ss_res->start, cpsw->irqs_table[0], descs_pool_size);

	pm_runtime_put(&pdev->dev);

	return 0;

clean_unregister_netdev_ret:
	unregister_netdev(ndev);
clean_cpts:
	cpts_release(cpsw->cpts);
	cpdma_ctlr_destroy(cpsw->dma);
clean_dt_ret:
	cpsw_remove_dt(pdev);
	pm_runtime_put_sync(&pdev->dev);
clean_runtime_disable_ret:
	pm_runtime_disable(&pdev->dev);
	return ret;
}

static void cpsw_remove(struct platform_device *pdev)
{
	struct cpsw_common *cpsw = platform_get_drvdata(pdev);
	int i, ret;

	ret = pm_runtime_resume_and_get(&pdev->dev);
	if (ret < 0) {
		/* Note, if this error path is taken, we're leaking some
		 * resources.
		 */
		dev_err(&pdev->dev, "Failed to resume device (%pe)\n",
			ERR_PTR(ret));
		return;
	}

	for (i = 0; i < cpsw->data.slaves; i++)
		if (cpsw->slaves[i].ndev)
			unregister_netdev(cpsw->slaves[i].ndev);

	cpts_release(cpsw->cpts);
	cpdma_ctlr_destroy(cpsw->dma);
	cpsw_remove_dt(pdev);
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
}

#ifdef CONFIG_PM_SLEEP
static int cpsw_suspend(struct device *dev)
{
	struct cpsw_common *cpsw = dev_get_drvdata(dev);
	int i;

	rtnl_lock();

	for (i = 0; i < cpsw->data.slaves; i++)
		if (cpsw->slaves[i].ndev)
			if (netif_running(cpsw->slaves[i].ndev))
				cpsw_ndo_stop(cpsw->slaves[i].ndev);

	rtnl_unlock();

	/* Select sleep pin state */
	pinctrl_pm_select_sleep_state(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int cpsw_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device	*pdev = to_platform_device(dev);
	struct net_device	*ndev = platform_get_drvdata(pdev);

	if (netif_running(ndev))
		cpsw_ndo_open(ndev);
	return 0;
}

static const struct dev_pm_ops cpsw_pm_ops = {
	.suspend	= cpsw_suspend,
	.resume		= cpsw_resume,
};
=======
	struct cpsw_common *cpsw = dev_get_drvdata(dev);
	int i;

	/* Select default pin state */
	pinctrl_pm_select_default_state(dev);

	/* shut up ASSERT_RTNL() warning in netif_set_real_num_tx/rx_queues */
	rtnl_lock();

	for (i = 0; i < cpsw->data.slaves; i++)
		if (cpsw->slaves[i].ndev)
			if (netif_running(cpsw->slaves[i].ndev))
				cpsw_ndo_open(cpsw->slaves[i].ndev);

	rtnl_unlock();

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(cpsw_pm_ops, cpsw_suspend, cpsw_resume);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct platform_driver cpsw_driver = {
	.driver = {
		.name	 = "cpsw",
<<<<<<< HEAD
		.owner	 = THIS_MODULE,
		.pm	 = &cpsw_pm_ops,
	},
	.probe = cpsw_probe,
	.remove = __devexit_p(cpsw_remove),
};

static int __init cpsw_init(void)
{
	return platform_driver_register(&cpsw_driver);
}
late_initcall(cpsw_init);

static void __exit cpsw_exit(void)
{
	platform_driver_unregister(&cpsw_driver);
}
module_exit(cpsw_exit);
=======
		.pm	 = &cpsw_pm_ops,
		.of_match_table = cpsw_of_mtable,
	},
	.probe = cpsw_probe,
	.remove_new = cpsw_remove,
};

module_platform_driver(cpsw_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cyril Chemparathy <cyril@ti.com>");
MODULE_AUTHOR("Mugunthan V N <mugunthanvnm@ti.com>");
MODULE_DESCRIPTION("TI CPSW Ethernet driver");
