<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/can/dev.h
 *
 * Definitions for the CAN network device driver interface
 *
 * Copyright (C) 2006 Andrey Volkov <avolkov@varma-el.com>
 *               Varma Electronics Oy
 *
 * Copyright (C) 2008 Wolfgang Grandegger <wg@grandegger.com>
 *
 */

<<<<<<< HEAD
#ifndef CAN_DEV_H
#define CAN_DEV_H

#include <linux/can.h>
#include <linux/can/netlink.h>
#include <linux/can/error.h>
=======
#ifndef _CAN_DEV_H
#define _CAN_DEV_H

#include <linux/can.h>
#include <linux/can/bittiming.h>
#include <linux/can/error.h>
#include <linux/can/length.h>
#include <linux/can/netlink.h>
#include <linux/can/skb.h>
#include <linux/ethtool.h>
#include <linux/netdevice.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * CAN mode
 */
enum can_mode {
	CAN_MODE_STOP = 0,
	CAN_MODE_START,
	CAN_MODE_SLEEP
};

<<<<<<< HEAD
=======
enum can_termination_gpio {
	CAN_TERMINATION_GPIO_DISABLED = 0,
	CAN_TERMINATION_GPIO_ENABLED,
	CAN_TERMINATION_GPIO_MAX,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * CAN common private data
 */
struct can_priv {
<<<<<<< HEAD
	struct can_device_stats can_stats;

	struct can_bittiming bittiming;
	struct can_bittiming_const *bittiming_const;
	struct can_clock clock;

	enum can_state state;
	u32 ctrlmode;
	u32 ctrlmode_supported;

	int restart_ms;
	struct timer_list restart_timer;

	int (*do_set_bittiming)(struct net_device *dev);
	int (*do_set_mode)(struct net_device *dev, enum can_mode mode);
=======
	struct net_device *dev;
	struct can_device_stats can_stats;

	const struct can_bittiming_const *bittiming_const,
		*data_bittiming_const;
	struct can_bittiming bittiming, data_bittiming;
	const struct can_tdc_const *tdc_const;
	struct can_tdc tdc;

	unsigned int bitrate_const_cnt;
	const u32 *bitrate_const;
	const u32 *data_bitrate_const;
	unsigned int data_bitrate_const_cnt;
	u32 bitrate_max;
	struct can_clock clock;

	unsigned int termination_const_cnt;
	const u16 *termination_const;
	u16 termination;
	struct gpio_desc *termination_gpio;
	u16 termination_gpio_ohms[CAN_TERMINATION_GPIO_MAX];

	unsigned int echo_skb_max;
	struct sk_buff **echo_skb;

	enum can_state state;

	/* CAN controller features - see include/uapi/linux/can/netlink.h */
	u32 ctrlmode;		/* current options setting */
	u32 ctrlmode_supported;	/* options that can be modified by netlink */

	int restart_ms;
	struct delayed_work restart_work;

	int (*do_set_bittiming)(struct net_device *dev);
	int (*do_set_data_bittiming)(struct net_device *dev);
	int (*do_set_mode)(struct net_device *dev, enum can_mode mode);
	int (*do_set_termination)(struct net_device *dev, u16 term);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*do_get_state)(const struct net_device *dev,
			    enum can_state *state);
	int (*do_get_berr_counter)(const struct net_device *dev,
				   struct can_berr_counter *bec);
<<<<<<< HEAD

	unsigned int echo_skb_max;
	struct sk_buff **echo_skb;
};

/*
 * get_can_dlc(value) - helper macro to cast a given data length code (dlc)
 * to __u8 and ensure the dlc value to be max. 8 bytes.
 *
 * To be used in the CAN netdriver receive path to ensure conformance with
 * ISO 11898-1 Chapter 8.4.2.3 (DLC field)
 */
#define get_can_dlc(i)	(min_t(__u8, (i), 8))

/* Drop a given socketbuffer if it does not contain a valid CAN frame. */
static inline int can_dropped_invalid_skb(struct net_device *dev,
					  struct sk_buff *skb)
{
	const struct can_frame *cf = (struct can_frame *)skb->data;

	if (unlikely(skb->len != sizeof(*cf) || cf->can_dlc > 8)) {
		kfree_skb(skb);
		dev->stats.tx_dropped++;
		return 1;
	}
=======
	int (*do_get_auto_tdcv)(const struct net_device *dev, u32 *tdcv);
};

static inline bool can_tdc_is_enabled(const struct can_priv *priv)
{
	return !!(priv->ctrlmode & CAN_CTRLMODE_TDC_MASK);
}

/*
 * can_get_relative_tdco() - TDCO relative to the sample point
 *
 * struct can_tdc::tdco represents the absolute offset from TDCV. Some
 * controllers use instead an offset relative to the Sample Point (SP)
 * such that:
 *
 * SSP = TDCV + absolute TDCO
 *     = TDCV + SP + relative TDCO
 *
 * -+----------- one bit ----------+-- TX pin
 *  |<--- Sample Point --->|
 *
 *                         --+----------- one bit ----------+-- RX pin
 *  |<-------- TDCV -------->|
 *                           |<------------------------>| absolute TDCO
 *                           |<--- Sample Point --->|
 *                           |                      |<->| relative TDCO
 *  |<------------- Secondary Sample Point ------------>|
 */
static inline s32 can_get_relative_tdco(const struct can_priv *priv)
{
	const struct can_bittiming *dbt = &priv->data_bittiming;
	s32 sample_point_in_tc = (CAN_SYNC_SEG + dbt->prop_seg +
				  dbt->phase_seg1) * dbt->brp;

	return (s32)priv->tdc.tdco - sample_point_in_tc;
}

/* helper to define static CAN controller features at device creation time */
static inline int __must_check can_set_static_ctrlmode(struct net_device *dev,
						       u32 static_mode)
{
	struct can_priv *priv = netdev_priv(dev);

	/* alloc_candev() succeeded => netdev_priv() is valid at this point */
	if (priv->ctrlmode_supported & static_mode) {
		netdev_warn(dev,
			    "Controller features can not be supported and static at the same time\n");
		return -EINVAL;
	}
	priv->ctrlmode = static_mode;

	/* override MTU which was set by default in can_setup()? */
	if (static_mode & CAN_CTRLMODE_FD)
		dev->mtu = CANFD_MTU;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
struct net_device *alloc_candev(int sizeof_priv, unsigned int echo_skb_max);
void free_candev(struct net_device *dev);

int open_candev(struct net_device *dev);
void close_candev(struct net_device *dev);
=======
static inline u32 can_get_static_ctrlmode(struct can_priv *priv)
{
	return priv->ctrlmode & ~priv->ctrlmode_supported;
}

static inline bool can_is_canxl_dev_mtu(unsigned int mtu)
{
	return (mtu >= CANXL_MIN_MTU && mtu <= CANXL_MAX_MTU);
}

/* drop skb if it does not contain a valid CAN frame for sending */
static inline bool can_dev_dropped_skb(struct net_device *dev, struct sk_buff *skb)
{
	struct can_priv *priv = netdev_priv(dev);

	if (priv->ctrlmode & CAN_CTRLMODE_LISTENONLY) {
		netdev_info_once(dev,
				 "interface in listen only mode, dropping skb\n");
		kfree_skb(skb);
		dev->stats.tx_dropped++;
		return true;
	}

	return can_dropped_invalid_skb(dev, skb);
}

void can_setup(struct net_device *dev);

struct net_device *alloc_candev_mqs(int sizeof_priv, unsigned int echo_skb_max,
				    unsigned int txqs, unsigned int rxqs);
#define alloc_candev(sizeof_priv, echo_skb_max) \
	alloc_candev_mqs(sizeof_priv, echo_skb_max, 1, 1)
#define alloc_candev_mq(sizeof_priv, echo_skb_max, count) \
	alloc_candev_mqs(sizeof_priv, echo_skb_max, count, count)
void free_candev(struct net_device *dev);

/* a candev safe wrapper around netdev_priv */
struct can_priv *safe_candev_priv(struct net_device *dev);

int open_candev(struct net_device *dev);
void close_candev(struct net_device *dev);
int can_change_mtu(struct net_device *dev, int new_mtu);
int can_eth_ioctl_hwts(struct net_device *netdev, struct ifreq *ifr, int cmd);
int can_ethtool_op_get_ts_info_hwts(struct net_device *dev,
				    struct ethtool_ts_info *info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int register_candev(struct net_device *dev);
void unregister_candev(struct net_device *dev);

int can_restart_now(struct net_device *dev);
void can_bus_off(struct net_device *dev);

<<<<<<< HEAD
void can_put_echo_skb(struct sk_buff *skb, struct net_device *dev,
		      unsigned int idx);
unsigned int can_get_echo_skb(struct net_device *dev, unsigned int idx);
void can_free_echo_skb(struct net_device *dev, unsigned int idx);

struct sk_buff *alloc_can_skb(struct net_device *dev, struct can_frame **cf);
struct sk_buff *alloc_can_err_skb(struct net_device *dev,
				  struct can_frame **cf);

#endif /* CAN_DEV_H */
=======
const char *can_get_state_str(const enum can_state state);
void can_state_get_by_berr_counter(const struct net_device *dev,
				   const struct can_berr_counter *bec,
				   enum can_state *tx_state,
				   enum can_state *rx_state);
void can_change_state(struct net_device *dev, struct can_frame *cf,
		      enum can_state tx_state, enum can_state rx_state);

#ifdef CONFIG_OF
void of_can_transceiver(struct net_device *dev);
#else
static inline void of_can_transceiver(struct net_device *dev) { }
#endif

extern struct rtnl_link_ops can_link_ops;
int can_netlink_register(void);
void can_netlink_unregister(void);

#endif /* !_CAN_DEV_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
