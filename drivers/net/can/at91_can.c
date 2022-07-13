<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * at91_can.c - CAN network driver for AT91 SoC CAN controller
 *
 * (C) 2007 by Hans J. Koch <hjk@hansjkoch.de>
<<<<<<< HEAD
 * (C) 2008, 2009, 2010, 2011 by Marc Kleine-Budde <kernel@pengutronix.de>
 *
 * This software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2 as distributed in the 'COPYING'
 * file from the main directory of the linux kernel source.
 *
 *
 * Your platform definition file should specify something like:
 *
 * static struct at91_can_data ek_can_data = {
 *	transceiver_switch = sam9263ek_transceiver_switch,
 * };
 *
 * at91_add_device_can(&ek_can_data);
 *
 */

#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/if_arp.h>
#include <linux/init.h>
=======
 * (C) 2008, 2009, 2010, 2011, 2023 by Marc Kleine-Budde <kernel@pengutronix.de>
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/ethtool.h>
#include <linux/if_arp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/phy/phy.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/types.h>

#include <linux/can/dev.h>
#include <linux/can/error.h>
<<<<<<< HEAD

#include <mach/board.h>

#define AT91_MB_MASK(i)		((1 << (i)) - 1)

/* Common registers */
enum at91_reg {
	AT91_MR		= 0x000,
	AT91_IER	= 0x004,
	AT91_IDR	= 0x008,
	AT91_IMR	= 0x00C,
	AT91_SR		= 0x010,
	AT91_BR		= 0x014,
	AT91_TIM	= 0x018,
	AT91_TIMESTP	= 0x01C,
	AT91_ECR	= 0x020,
	AT91_TCR	= 0x024,
	AT91_ACR	= 0x028,
};

/* Mailbox registers (0 <= i <= 15) */
#define AT91_MMR(i)		(enum at91_reg)(0x200 + ((i) * 0x20))
#define AT91_MAM(i)		(enum at91_reg)(0x204 + ((i) * 0x20))
#define AT91_MID(i)		(enum at91_reg)(0x208 + ((i) * 0x20))
#define AT91_MFID(i)		(enum at91_reg)(0x20C + ((i) * 0x20))
#define AT91_MSR(i)		(enum at91_reg)(0x210 + ((i) * 0x20))
#define AT91_MDL(i)		(enum at91_reg)(0x214 + ((i) * 0x20))
#define AT91_MDH(i)		(enum at91_reg)(0x218 + ((i) * 0x20))
#define AT91_MCR(i)		(enum at91_reg)(0x21C + ((i) * 0x20))

/* Register bits */
#define AT91_MR_CANEN		BIT(0)
#define AT91_MR_LPM		BIT(1)
#define AT91_MR_ABM		BIT(2)
#define AT91_MR_OVL		BIT(3)
#define AT91_MR_TEOF		BIT(4)
#define AT91_MR_TTM		BIT(5)
#define AT91_MR_TIMFRZ		BIT(6)
#define AT91_MR_DRPT		BIT(7)

#define AT91_SR_RBSY		BIT(29)

#define AT91_MMR_PRIO_SHIFT	(16)

#define AT91_MID_MIDE		BIT(29)

#define AT91_MSR_MRTR		BIT(20)
#define AT91_MSR_MABT		BIT(22)
#define AT91_MSR_MRDY		BIT(23)
#define AT91_MSR_MMI		BIT(24)

#define AT91_MCR_MRTR		BIT(20)
#define AT91_MCR_MTCR		BIT(23)

/* Mailbox Modes */
enum at91_mb_mode {
	AT91_MB_MODE_DISABLED	= 0,
	AT91_MB_MODE_RX		= 1,
	AT91_MB_MODE_RX_OVRWR	= 2,
	AT91_MB_MODE_TX		= 3,
	AT91_MB_MODE_CONSUMER	= 4,
	AT91_MB_MODE_PRODUCER	= 5,
};

/* Interrupt mask bits */
#define AT91_IRQ_ERRA		(1 << 16)
#define AT91_IRQ_WARN		(1 << 17)
#define AT91_IRQ_ERRP		(1 << 18)
#define AT91_IRQ_BOFF		(1 << 19)
#define AT91_IRQ_SLEEP		(1 << 20)
#define AT91_IRQ_WAKEUP		(1 << 21)
#define AT91_IRQ_TOVF		(1 << 22)
#define AT91_IRQ_TSTP		(1 << 23)
#define AT91_IRQ_CERR		(1 << 24)
#define AT91_IRQ_SERR		(1 << 25)
#define AT91_IRQ_AERR		(1 << 26)
#define AT91_IRQ_FERR		(1 << 27)
#define AT91_IRQ_BERR		(1 << 28)

#define AT91_IRQ_ERR_ALL	(0x1fff0000)
#define AT91_IRQ_ERR_FRAME	(AT91_IRQ_CERR | AT91_IRQ_SERR | \
				 AT91_IRQ_AERR | AT91_IRQ_FERR | AT91_IRQ_BERR)
#define AT91_IRQ_ERR_LINE	(AT91_IRQ_ERRA | AT91_IRQ_WARN | \
				 AT91_IRQ_ERRP | AT91_IRQ_BOFF)

#define AT91_IRQ_ALL		(0x1fffffff)
=======
#include <linux/can/rx-offload.h>

#define AT91_MB_MASK(i) ((1 << (i)) - 1)

/* Common registers */
enum at91_reg {
	AT91_MR = 0x000,
	AT91_IER = 0x004,
	AT91_IDR = 0x008,
	AT91_IMR = 0x00C,
	AT91_SR = 0x010,
	AT91_BR = 0x014,
	AT91_TIM = 0x018,
	AT91_TIMESTP = 0x01C,
	AT91_ECR = 0x020,
	AT91_TCR = 0x024,
	AT91_ACR = 0x028,
};

/* Mailbox registers (0 <= i <= 15) */
#define AT91_MMR(i) ((enum at91_reg)(0x200 + ((i) * 0x20)))
#define AT91_MAM(i) ((enum at91_reg)(0x204 + ((i) * 0x20)))
#define AT91_MID(i) ((enum at91_reg)(0x208 + ((i) * 0x20)))
#define AT91_MFID(i) ((enum at91_reg)(0x20C + ((i) * 0x20)))
#define AT91_MSR(i) ((enum at91_reg)(0x210 + ((i) * 0x20)))
#define AT91_MDL(i) ((enum at91_reg)(0x214 + ((i) * 0x20)))
#define AT91_MDH(i) ((enum at91_reg)(0x218 + ((i) * 0x20)))
#define AT91_MCR(i) ((enum at91_reg)(0x21C + ((i) * 0x20)))

/* Register bits */
#define AT91_MR_CANEN BIT(0)
#define AT91_MR_LPM BIT(1)
#define AT91_MR_ABM BIT(2)
#define AT91_MR_OVL BIT(3)
#define AT91_MR_TEOF BIT(4)
#define AT91_MR_TTM BIT(5)
#define AT91_MR_TIMFRZ BIT(6)
#define AT91_MR_DRPT BIT(7)

#define AT91_SR_RBSY BIT(29)
#define AT91_SR_TBSY BIT(30)
#define AT91_SR_OVLSY BIT(31)

#define AT91_BR_PHASE2_MASK GENMASK(2, 0)
#define AT91_BR_PHASE1_MASK GENMASK(6, 4)
#define AT91_BR_PROPAG_MASK GENMASK(10, 8)
#define AT91_BR_SJW_MASK GENMASK(13, 12)
#define AT91_BR_BRP_MASK GENMASK(22, 16)
#define AT91_BR_SMP BIT(24)

#define AT91_TIM_TIMER_MASK GENMASK(15, 0)

#define AT91_ECR_REC_MASK GENMASK(8, 0)
#define AT91_ECR_TEC_MASK GENMASK(23, 16)

#define AT91_TCR_TIMRST BIT(31)

#define AT91_MMR_MTIMEMARK_MASK GENMASK(15, 0)
#define AT91_MMR_PRIOR_MASK GENMASK(19, 16)
#define AT91_MMR_MOT_MASK GENMASK(26, 24)

#define AT91_MID_MIDVB_MASK GENMASK(17, 0)
#define AT91_MID_MIDVA_MASK GENMASK(28, 18)
#define AT91_MID_MIDE BIT(29)

#define AT91_MSR_MTIMESTAMP_MASK GENMASK(15, 0)
#define AT91_MSR_MDLC_MASK GENMASK(19, 16)
#define AT91_MSR_MRTR BIT(20)
#define AT91_MSR_MABT BIT(22)
#define AT91_MSR_MRDY BIT(23)
#define AT91_MSR_MMI BIT(24)

#define AT91_MCR_MDLC_MASK GENMASK(19, 16)
#define AT91_MCR_MRTR BIT(20)
#define AT91_MCR_MACR BIT(22)
#define AT91_MCR_MTCR BIT(23)

/* Mailbox Modes */
enum at91_mb_mode {
	AT91_MB_MODE_DISABLED = 0,
	AT91_MB_MODE_RX = 1,
	AT91_MB_MODE_RX_OVRWR = 2,
	AT91_MB_MODE_TX = 3,
	AT91_MB_MODE_CONSUMER = 4,
	AT91_MB_MODE_PRODUCER = 5,
};

/* Interrupt mask bits */
#define AT91_IRQ_ERRA BIT(16)
#define AT91_IRQ_WARN BIT(17)
#define AT91_IRQ_ERRP BIT(18)
#define AT91_IRQ_BOFF BIT(19)
#define AT91_IRQ_SLEEP BIT(20)
#define AT91_IRQ_WAKEUP BIT(21)
#define AT91_IRQ_TOVF BIT(22)
#define AT91_IRQ_TSTP BIT(23)
#define AT91_IRQ_CERR BIT(24)
#define AT91_IRQ_SERR BIT(25)
#define AT91_IRQ_AERR BIT(26)
#define AT91_IRQ_FERR BIT(27)
#define AT91_IRQ_BERR BIT(28)

#define AT91_IRQ_ERR_ALL (0x1fff0000)
#define AT91_IRQ_ERR_FRAME (AT91_IRQ_CERR | AT91_IRQ_SERR | \
			    AT91_IRQ_AERR | AT91_IRQ_FERR | AT91_IRQ_BERR)
#define AT91_IRQ_ERR_LINE (AT91_IRQ_ERRA | AT91_IRQ_WARN | \
			   AT91_IRQ_ERRP | AT91_IRQ_BOFF)

#define AT91_IRQ_ALL (0x1fffffff)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum at91_devtype {
	AT91_DEVTYPE_SAM9263,
	AT91_DEVTYPE_SAM9X5,
};

struct at91_devtype_data {
	unsigned int rx_first;
<<<<<<< HEAD
	unsigned int rx_split;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int rx_last;
	unsigned int tx_shift;
	enum at91_devtype type;
};

struct at91_priv {
	struct can_priv can;		/* must be the first member! */
<<<<<<< HEAD
	struct net_device *dev;
	struct napi_struct napi;

	void __iomem *reg_base;

	u32 reg_sr;
	unsigned int tx_next;
	unsigned int tx_echo;
	unsigned int rx_next;
=======
	struct can_rx_offload offload;
	struct phy *transceiver;

	void __iomem *reg_base;

	unsigned int tx_head;
	unsigned int tx_tail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct at91_devtype_data devtype_data;

	struct clk *clk;
	struct at91_can_data *pdata;

	canid_t mb0_id;
};

<<<<<<< HEAD
static const struct at91_devtype_data at91_devtype_data[] __devinitconst = {
	[AT91_DEVTYPE_SAM9263] = {
		.rx_first = 1,
		.rx_split = 8,
		.rx_last = 11,
		.tx_shift = 2,
	},
	[AT91_DEVTYPE_SAM9X5] = {
		.rx_first = 0,
		.rx_split = 4,
		.rx_last = 5,
		.tx_shift = 1,
	},
};

static struct can_bittiming_const at91_bittiming_const = {
=======
static inline struct at91_priv *rx_offload_to_priv(struct can_rx_offload *offload)
{
	return container_of(offload, struct at91_priv, offload);
}

static const struct at91_devtype_data at91_at91sam9263_data = {
	.rx_first = 1,
	.rx_last = 11,
	.tx_shift = 2,
	.type = AT91_DEVTYPE_SAM9263,
};

static const struct at91_devtype_data at91_at91sam9x5_data = {
	.rx_first = 0,
	.rx_last = 5,
	.tx_shift = 1,
	.type = AT91_DEVTYPE_SAM9X5,
};

static const struct can_bittiming_const at91_bittiming_const = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name		= KBUILD_MODNAME,
	.tseg1_min	= 4,
	.tseg1_max	= 16,
	.tseg2_min	= 2,
	.tseg2_max	= 8,
	.sjw_max	= 4,
<<<<<<< HEAD
	.brp_min 	= 2,
=======
	.brp_min	= 2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.brp_max	= 128,
	.brp_inc	= 1,
};

#define AT91_IS(_model) \
<<<<<<< HEAD
static inline int at91_is_sam##_model(const struct at91_priv *priv) \
=======
static inline int __maybe_unused at91_is_sam##_model(const struct at91_priv *priv) \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{ \
	return priv->devtype_data.type == AT91_DEVTYPE_SAM##_model; \
}

AT91_IS(9263);
AT91_IS(9X5);

static inline unsigned int get_mb_rx_first(const struct at91_priv *priv)
{
	return priv->devtype_data.rx_first;
}

static inline unsigned int get_mb_rx_last(const struct at91_priv *priv)
{
	return priv->devtype_data.rx_last;
}

<<<<<<< HEAD
static inline unsigned int get_mb_rx_split(const struct at91_priv *priv)
{
	return priv->devtype_data.rx_split;
}

static inline unsigned int get_mb_rx_num(const struct at91_priv *priv)
{
	return get_mb_rx_last(priv) - get_mb_rx_first(priv) + 1;
}

static inline unsigned int get_mb_rx_low_last(const struct at91_priv *priv)
{
	return get_mb_rx_split(priv) - 1;
}

static inline unsigned int get_mb_rx_low_mask(const struct at91_priv *priv)
{
	return AT91_MB_MASK(get_mb_rx_split(priv)) &
		~AT91_MB_MASK(get_mb_rx_first(priv));
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned int get_mb_tx_shift(const struct at91_priv *priv)
{
	return priv->devtype_data.tx_shift;
}

static inline unsigned int get_mb_tx_num(const struct at91_priv *priv)
{
	return 1 << get_mb_tx_shift(priv);
}

static inline unsigned int get_mb_tx_first(const struct at91_priv *priv)
{
	return get_mb_rx_last(priv) + 1;
}

static inline unsigned int get_mb_tx_last(const struct at91_priv *priv)
{
	return get_mb_tx_first(priv) + get_mb_tx_num(priv) - 1;
}

<<<<<<< HEAD
static inline unsigned int get_next_prio_shift(const struct at91_priv *priv)
=======
static inline unsigned int get_head_prio_shift(const struct at91_priv *priv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return get_mb_tx_shift(priv);
}

<<<<<<< HEAD
static inline unsigned int get_next_prio_mask(const struct at91_priv *priv)
=======
static inline unsigned int get_head_prio_mask(const struct at91_priv *priv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0xf << get_mb_tx_shift(priv);
}

<<<<<<< HEAD
static inline unsigned int get_next_mb_mask(const struct at91_priv *priv)
=======
static inline unsigned int get_head_mb_mask(const struct at91_priv *priv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return AT91_MB_MASK(get_mb_tx_shift(priv));
}

<<<<<<< HEAD
static inline unsigned int get_next_mask(const struct at91_priv *priv)
{
	return get_next_mb_mask(priv) | get_next_prio_mask(priv);
=======
static inline unsigned int get_head_mask(const struct at91_priv *priv)
{
	return get_head_mb_mask(priv) | get_head_prio_mask(priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline unsigned int get_irq_mb_rx(const struct at91_priv *priv)
{
	return AT91_MB_MASK(get_mb_rx_last(priv) + 1) &
		~AT91_MB_MASK(get_mb_rx_first(priv));
}

static inline unsigned int get_irq_mb_tx(const struct at91_priv *priv)
{
	return AT91_MB_MASK(get_mb_tx_last(priv) + 1) &
		~AT91_MB_MASK(get_mb_tx_first(priv));
}

<<<<<<< HEAD
static inline unsigned int get_tx_next_mb(const struct at91_priv *priv)
{
	return (priv->tx_next & get_next_mb_mask(priv)) + get_mb_tx_first(priv);
}

static inline unsigned int get_tx_next_prio(const struct at91_priv *priv)
{
	return (priv->tx_next >> get_next_prio_shift(priv)) & 0xf;
}

static inline unsigned int get_tx_echo_mb(const struct at91_priv *priv)
{
	return (priv->tx_echo & get_next_mb_mask(priv)) + get_mb_tx_first(priv);
=======
static inline unsigned int get_tx_head_mb(const struct at91_priv *priv)
{
	return (priv->tx_head & get_head_mb_mask(priv)) + get_mb_tx_first(priv);
}

static inline unsigned int get_tx_head_prio(const struct at91_priv *priv)
{
	return (priv->tx_head >> get_head_prio_shift(priv)) & 0xf;
}

static inline unsigned int get_tx_tail_mb(const struct at91_priv *priv)
{
	return (priv->tx_tail & get_head_mb_mask(priv)) + get_mb_tx_first(priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline u32 at91_read(const struct at91_priv *priv, enum at91_reg reg)
{
<<<<<<< HEAD
	return __raw_readl(priv->reg_base + reg);
}

static inline void at91_write(const struct at91_priv *priv, enum at91_reg reg,
		u32 value)
{
	__raw_writel(value, priv->reg_base + reg);
}

static inline void set_mb_mode_prio(const struct at91_priv *priv,
		unsigned int mb, enum at91_mb_mode mode, int prio)
{
	at91_write(priv, AT91_MMR(mb), (mode << 24) | (prio << 16));
}

static inline void set_mb_mode(const struct at91_priv *priv, unsigned int mb,
		enum at91_mb_mode mode)
=======
	return readl_relaxed(priv->reg_base + reg);
}

static inline void at91_write(const struct at91_priv *priv, enum at91_reg reg,
			      u32 value)
{
	writel_relaxed(value, priv->reg_base + reg);
}

static inline void set_mb_mode_prio(const struct at91_priv *priv,
				    unsigned int mb, enum at91_mb_mode mode,
				    u8 prio)
{
	const u32 reg_mmr = FIELD_PREP(AT91_MMR_MOT_MASK, mode) |
		FIELD_PREP(AT91_MMR_PRIOR_MASK, prio);

	at91_write(priv, AT91_MMR(mb), reg_mmr);
}

static inline void set_mb_mode(const struct at91_priv *priv, unsigned int mb,
			       enum at91_mb_mode mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	set_mb_mode_prio(priv, mb, mode, 0);
}

static inline u32 at91_can_id_to_reg_mid(canid_t can_id)
{
	u32 reg_mid;

	if (can_id & CAN_EFF_FLAG)
<<<<<<< HEAD
		reg_mid = (can_id & CAN_EFF_MASK) | AT91_MID_MIDE;
	else
		reg_mid = (can_id & CAN_SFF_MASK) << 18;
=======
		reg_mid = FIELD_PREP(AT91_MID_MIDVA_MASK | AT91_MID_MIDVB_MASK, can_id) |
			AT91_MID_MIDE;
	else
		reg_mid = FIELD_PREP(AT91_MID_MIDVA_MASK, can_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return reg_mid;
}

<<<<<<< HEAD
/*
 * Swtich transceiver on or off
 */
static void at91_transceiver_switch(const struct at91_priv *priv, int on)
{
	if (priv->pdata && priv->pdata->transceiver_switch)
		priv->pdata->transceiver_switch(on);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void at91_setup_mailboxes(struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);
	unsigned int i;
	u32 reg_mid;

<<<<<<< HEAD
	/*
	 * Due to a chip bug (errata 50.2.6.3 & 50.3.5.3) the first
	 * mailbox is disabled. The next 11 mailboxes are used as a
	 * reception FIFO. The last mailbox is configured with
=======
	/* Due to a chip bug (errata 50.2.6.3 & 50.3.5.3) the first
	 * mailbox is disabled. The next mailboxes are used as a
	 * reception FIFO. The last of the RX mailboxes is configured with
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * overwrite option. The overwrite flag indicates a FIFO
	 * overflow.
	 */
	reg_mid = at91_can_id_to_reg_mid(priv->mb0_id);
	for (i = 0; i < get_mb_rx_first(priv); i++) {
		set_mb_mode(priv, i, AT91_MB_MODE_DISABLED);
		at91_write(priv, AT91_MID(i), reg_mid);
		at91_write(priv, AT91_MCR(i), 0x0);	/* clear dlc */
	}

	for (i = get_mb_rx_first(priv); i < get_mb_rx_last(priv); i++)
		set_mb_mode(priv, i, AT91_MB_MODE_RX);
	set_mb_mode(priv, get_mb_rx_last(priv), AT91_MB_MODE_RX_OVRWR);

	/* reset acceptance mask and id register */
	for (i = get_mb_rx_first(priv); i <= get_mb_rx_last(priv); i++) {
		at91_write(priv, AT91_MAM(i), 0x0);
		at91_write(priv, AT91_MID(i), AT91_MID_MIDE);
	}

<<<<<<< HEAD
	/* The last 4 mailboxes are used for transmitting. */
	for (i = get_mb_tx_first(priv); i <= get_mb_tx_last(priv); i++)
		set_mb_mode_prio(priv, i, AT91_MB_MODE_TX, 0);

	/* Reset tx and rx helper pointers */
	priv->tx_next = priv->tx_echo = 0;
	priv->rx_next = get_mb_rx_first(priv);
=======
	/* The last mailboxes are used for transmitting. */
	for (i = get_mb_tx_first(priv); i <= get_mb_tx_last(priv); i++)
		set_mb_mode_prio(priv, i, AT91_MB_MODE_TX, 0);

	/* Reset tx helper pointers */
	priv->tx_head = priv->tx_tail = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int at91_set_bittiming(struct net_device *dev)
{
	const struct at91_priv *priv = netdev_priv(dev);
	const struct can_bittiming *bt = &priv->can.bittiming;
<<<<<<< HEAD
	u32 reg_br;

	reg_br = ((priv->can.ctrlmode & CAN_CTRLMODE_3_SAMPLES) ? 1 << 24 : 0) |
		((bt->brp - 1) << 16) | ((bt->sjw - 1) << 12) |
		((bt->prop_seg - 1) << 8) | ((bt->phase_seg1 - 1) << 4) |
		((bt->phase_seg2 - 1) << 0);

	netdev_info(dev, "writing AT91_BR: 0x%08x\n", reg_br);
=======
	u32 reg_br = 0;

	if (priv->can.ctrlmode & CAN_CTRLMODE_3_SAMPLES)
		reg_br |= AT91_BR_SMP;

	reg_br |= FIELD_PREP(AT91_BR_BRP_MASK, bt->brp - 1) |
		FIELD_PREP(AT91_BR_SJW_MASK, bt->sjw - 1) |
		FIELD_PREP(AT91_BR_PROPAG_MASK, bt->prop_seg - 1) |
		FIELD_PREP(AT91_BR_PHASE1_MASK, bt->phase_seg1 - 1) |
		FIELD_PREP(AT91_BR_PHASE2_MASK, bt->phase_seg2 - 1);

	netdev_dbg(dev, "writing AT91_BR: 0x%08x\n", reg_br);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	at91_write(priv, AT91_BR, reg_br);

	return 0;
}

static int at91_get_berr_counter(const struct net_device *dev,
<<<<<<< HEAD
		struct can_berr_counter *bec)
=======
				 struct can_berr_counter *bec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct at91_priv *priv = netdev_priv(dev);
	u32 reg_ecr = at91_read(priv, AT91_ECR);

<<<<<<< HEAD
	bec->rxerr = reg_ecr & 0xff;
	bec->txerr = reg_ecr >> 16;
=======
	bec->rxerr = FIELD_GET(AT91_ECR_REC_MASK, reg_ecr);
	bec->txerr = FIELD_GET(AT91_ECR_TEC_MASK, reg_ecr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void at91_chip_start(struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);
	u32 reg_mr, reg_ier;

	/* disable interrupts */
	at91_write(priv, AT91_IDR, AT91_IRQ_ALL);

	/* disable chip */
	reg_mr = at91_read(priv, AT91_MR);
	at91_write(priv, AT91_MR, reg_mr & ~AT91_MR_CANEN);

	at91_set_bittiming(dev);
	at91_setup_mailboxes(dev);
<<<<<<< HEAD
	at91_transceiver_switch(priv, 1);

	/* enable chip */
	at91_write(priv, AT91_MR, AT91_MR_CANEN);

	priv->can.state = CAN_STATE_ERROR_ACTIVE;

	/* Enable interrupts */
	reg_ier = get_irq_mb_rx(priv) | AT91_IRQ_ERRP | AT91_IRQ_ERR_FRAME;
	at91_write(priv, AT91_IDR, AT91_IRQ_ALL);
=======

	/* enable chip */
	if (priv->can.ctrlmode & CAN_CTRLMODE_LISTENONLY)
		reg_mr = AT91_MR_CANEN | AT91_MR_ABM;
	else
		reg_mr = AT91_MR_CANEN;
	at91_write(priv, AT91_MR, reg_mr);

	priv->can.state = CAN_STATE_ERROR_ACTIVE;

	/* Dummy read to clear latched line error interrupts on
	 * sam9x5 and newer SoCs.
	 */
	at91_read(priv, AT91_SR);

	/* Enable interrupts */
	reg_ier = get_irq_mb_rx(priv) | AT91_IRQ_ERR_LINE | AT91_IRQ_ERR_FRAME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	at91_write(priv, AT91_IER, reg_ier);
}

static void at91_chip_stop(struct net_device *dev, enum can_state state)
{
	struct at91_priv *priv = netdev_priv(dev);
	u32 reg_mr;

<<<<<<< HEAD
=======
	/* Abort any pending TX requests. However this doesn't seem to
	 * work in case of bus-off on sama5d3.
	 */
	at91_write(priv, AT91_ACR, get_irq_mb_tx(priv));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* disable interrupts */
	at91_write(priv, AT91_IDR, AT91_IRQ_ALL);

	reg_mr = at91_read(priv, AT91_MR);
	at91_write(priv, AT91_MR, reg_mr & ~AT91_MR_CANEN);

<<<<<<< HEAD
	at91_transceiver_switch(priv, 0);
	priv->can.state = state;
}

/*
 * theory of operation:
=======
	priv->can.state = state;
}

/* theory of operation:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * According to the datasheet priority 0 is the highest priority, 15
 * is the lowest. If two mailboxes have the same priority level the
 * message of the mailbox with the lowest number is sent first.
 *
 * We use the first TX mailbox (AT91_MB_TX_FIRST) with prio 0, then
 * the next mailbox with prio 0, and so on, until all mailboxes are
 * used. Then we start from the beginning with mailbox
 * AT91_MB_TX_FIRST, but with prio 1, mailbox AT91_MB_TX_FIRST + 1
 * prio 1. When we reach the last mailbox with prio 15, we have to
 * stop sending, waiting for all messages to be delivered, then start
 * again with mailbox AT91_MB_TX_FIRST prio 0.
 *
<<<<<<< HEAD
 * We use the priv->tx_next as counter for the next transmission
 * mailbox, but without the offset AT91_MB_TX_FIRST. The lower bits
 * encode the mailbox number, the upper 4 bits the mailbox priority:
 *
 * priv->tx_next = (prio << get_next_prio_shift(priv)) |
=======
 * We use the priv->tx_head as counter for the next transmission
 * mailbox, but without the offset AT91_MB_TX_FIRST. The lower bits
 * encode the mailbox number, the upper 4 bits the mailbox priority:
 *
 * priv->tx_head = (prio << get_next_prio_shift(priv)) |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *                 (mb - get_mb_tx_first(priv));
 *
 */
static netdev_tx_t at91_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);
<<<<<<< HEAD
	struct net_device_stats *stats = &dev->stats;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct can_frame *cf = (struct can_frame *)skb->data;
	unsigned int mb, prio;
	u32 reg_mid, reg_mcr;

<<<<<<< HEAD
	if (can_dropped_invalid_skb(dev, skb))
		return NETDEV_TX_OK;

	mb = get_tx_next_mb(priv);
	prio = get_tx_next_prio(priv);
=======
	if (can_dev_dropped_skb(dev, skb))
		return NETDEV_TX_OK;

	mb = get_tx_head_mb(priv);
	prio = get_tx_head_prio(priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(!(at91_read(priv, AT91_MSR(mb)) & AT91_MSR_MRDY))) {
		netif_stop_queue(dev);

		netdev_err(dev, "BUG! TX buffer full when queue awake!\n");
		return NETDEV_TX_BUSY;
	}
	reg_mid = at91_can_id_to_reg_mid(cf->can_id);
<<<<<<< HEAD
	reg_mcr = ((cf->can_id & CAN_RTR_FLAG) ? AT91_MCR_MRTR : 0) |
		(cf->can_dlc << 16) | AT91_MCR_MTCR;
=======

	reg_mcr = FIELD_PREP(AT91_MCR_MDLC_MASK, cf->len) |
		AT91_MCR_MTCR;

	if (cf->can_id & CAN_RTR_FLAG)
		reg_mcr |= AT91_MCR_MRTR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* disable MB while writing ID (see datasheet) */
	set_mb_mode(priv, mb, AT91_MB_MODE_DISABLED);
	at91_write(priv, AT91_MID(mb), reg_mid);
	set_mb_mode_prio(priv, mb, AT91_MB_MODE_TX, prio);

	at91_write(priv, AT91_MDL(mb), *(u32 *)(cf->data + 0));
	at91_write(priv, AT91_MDH(mb), *(u32 *)(cf->data + 4));

	/* This triggers transmission */
	at91_write(priv, AT91_MCR(mb), reg_mcr);

<<<<<<< HEAD
	stats->tx_bytes += cf->can_dlc;

	/* _NOTE_: subtract AT91_MB_TX_FIRST offset from mb! */
	can_put_echo_skb(skb, dev, mb - get_mb_tx_first(priv));

	/*
	 * we have to stop the queue and deliver all messages in case
	 * of a prio+mb counter wrap around. This is the case if
	 * tx_next buffer prio and mailbox equals 0.
=======
	/* _NOTE_: subtract AT91_MB_TX_FIRST offset from mb! */
	can_put_echo_skb(skb, dev, mb - get_mb_tx_first(priv), 0);

	/* we have to stop the queue and deliver all messages in case
	 * of a prio+mb counter wrap around. This is the case if
	 * tx_head buffer prio and mailbox equals 0.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * also stop the queue if next buffer is still in use
	 * (== not ready)
	 */
<<<<<<< HEAD
	priv->tx_next++;
	if (!(at91_read(priv, AT91_MSR(get_tx_next_mb(priv))) &
	      AT91_MSR_MRDY) ||
	    (priv->tx_next & get_next_mask(priv)) == 0)
=======
	priv->tx_head++;
	if (!(at91_read(priv, AT91_MSR(get_tx_head_mb(priv))) &
	      AT91_MSR_MRDY) ||
	    (priv->tx_head & get_head_mask(priv)) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		netif_stop_queue(dev);

	/* Enable interrupt for this mailbox */
	at91_write(priv, AT91_IER, 1 << mb);

	return NETDEV_TX_OK;
}

<<<<<<< HEAD
/**
 * at91_activate_rx_low - activate lower rx mailboxes
 * @priv: a91 context
 *
 * Reenables the lower mailboxes for reception of new CAN messages
 */
static inline void at91_activate_rx_low(const struct at91_priv *priv)
{
	u32 mask = get_mb_rx_low_mask(priv);
	at91_write(priv, AT91_TCR, mask);
}

/**
 * at91_activate_rx_mb - reactive single rx mailbox
 * @priv: a91 context
 * @mb: mailbox to reactivate
 *
 * Reenables given mailbox for reception of new CAN messages
 */
static inline void at91_activate_rx_mb(const struct at91_priv *priv,
		unsigned int mb)
{
	u32 mask = 1 << mb;
	at91_write(priv, AT91_TCR, mask);
=======
static inline u32 at91_get_timestamp(const struct at91_priv *priv)
{
	return at91_read(priv, AT91_TIM);
}

static inline struct sk_buff *
at91_alloc_can_err_skb(struct net_device *dev,
		       struct can_frame **cf, u32 *timestamp)
{
	const struct at91_priv *priv = netdev_priv(dev);

	*timestamp = at91_get_timestamp(priv);

	return alloc_can_err_skb(dev, cf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * at91_rx_overflow_err - send error frame due to rx overflow
 * @dev: net device
 */
static void at91_rx_overflow_err(struct net_device *dev)
{
	struct net_device_stats *stats = &dev->stats;
	struct sk_buff *skb;
<<<<<<< HEAD
	struct can_frame *cf;
=======
	struct at91_priv *priv = netdev_priv(dev);
	struct can_frame *cf;
	u32 timestamp;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netdev_dbg(dev, "RX buffer overflow\n");
	stats->rx_over_errors++;
	stats->rx_errors++;

<<<<<<< HEAD
	skb = alloc_can_err_skb(dev, &cf);
=======
	skb = at91_alloc_can_err_skb(dev, &cf, &timestamp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(!skb))
		return;

	cf->can_id |= CAN_ERR_CRTL;
	cf->data[1] = CAN_ERR_CRTL_RX_OVERFLOW;
<<<<<<< HEAD
	netif_receive_skb(skb);

	stats->rx_packets++;
	stats->rx_bytes += cf->can_dlc;
}

/**
 * at91_read_mb - read CAN msg from mailbox (lowlevel impl)
 * @dev: net device
 * @mb: mailbox number to read from
 * @cf: can frame where to store message
 *
 * Reads a CAN message from the given mailbox and stores data into
 * given can frame. "mb" and "cf" must be valid.
 */
static void at91_read_mb(struct net_device *dev, unsigned int mb,
		struct can_frame *cf)
{
	const struct at91_priv *priv = netdev_priv(dev);
	u32 reg_msr, reg_mid;

	reg_mid = at91_read(priv, AT91_MID(mb));
	if (reg_mid & AT91_MID_MIDE)
		cf->can_id = ((reg_mid >> 0) & CAN_EFF_MASK) | CAN_EFF_FLAG;
	else
		cf->can_id = (reg_mid >> 18) & CAN_SFF_MASK;

	reg_msr = at91_read(priv, AT91_MSR(mb));
	cf->can_dlc = get_can_dlc((reg_msr >> 16) & 0xf);

	if (reg_msr & AT91_MSR_MRTR)
		cf->can_id |= CAN_RTR_FLAG;
	else {
=======

	err = can_rx_offload_queue_timestamp(&priv->offload, skb, timestamp);
	if (err)
		stats->rx_fifo_errors++;
}

/**
 * at91_mailbox_read - read CAN msg from mailbox
 * @offload: rx-offload
 * @mb: mailbox number to read from
 * @timestamp: pointer to 32 bit timestamp
 * @drop: true indicated mailbox to mark as read and drop frame
 *
 * Reads a CAN message from the given mailbox if not empty.
 */
static struct sk_buff *at91_mailbox_read(struct can_rx_offload *offload,
					 unsigned int mb, u32 *timestamp,
					 bool drop)
{
	const struct at91_priv *priv = rx_offload_to_priv(offload);
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 reg_msr, reg_mid;

	reg_msr = at91_read(priv, AT91_MSR(mb));
	if (!(reg_msr & AT91_MSR_MRDY))
		return NULL;

	if (unlikely(drop)) {
		skb = ERR_PTR(-ENOBUFS);
		goto mark_as_read;
	}

	skb = alloc_can_skb(offload->dev, &cf);
	if (unlikely(!skb)) {
		skb = ERR_PTR(-ENOMEM);
		goto mark_as_read;
	}

	reg_mid = at91_read(priv, AT91_MID(mb));
	if (reg_mid & AT91_MID_MIDE)
		cf->can_id = FIELD_GET(AT91_MID_MIDVA_MASK | AT91_MID_MIDVB_MASK, reg_mid) |
			CAN_EFF_FLAG;
	else
		cf->can_id = FIELD_GET(AT91_MID_MIDVA_MASK, reg_mid);

	/* extend timestamp to full 32 bit */
	*timestamp = FIELD_GET(AT91_MSR_MTIMESTAMP_MASK, reg_msr) << 16;

	cf->len = can_cc_dlc2len(FIELD_GET(AT91_MSR_MDLC_MASK, reg_msr));

	if (reg_msr & AT91_MSR_MRTR) {
		cf->can_id |= CAN_RTR_FLAG;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*(u32 *)(cf->data + 0) = at91_read(priv, AT91_MDL(mb));
		*(u32 *)(cf->data + 4) = at91_read(priv, AT91_MDH(mb));
	}

	/* allow RX of extended frames */
	at91_write(priv, AT91_MID(mb), AT91_MID_MIDE);

	if (unlikely(mb == get_mb_rx_last(priv) && reg_msr & AT91_MSR_MMI))
<<<<<<< HEAD
		at91_rx_overflow_err(dev);
}

/**
 * at91_read_msg - read CAN message from mailbox
 * @dev: net device
 * @mb: mail box to read from
 *
 * Reads a CAN message from given mailbox, and put into linux network
 * RX queue, does all housekeeping chores (stats, ...)
 */
static void at91_read_msg(struct net_device *dev, unsigned int mb)
{
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;

	skb = alloc_can_skb(dev, &cf);
	if (unlikely(!skb)) {
		stats->rx_dropped++;
		return;
	}

	at91_read_mb(dev, mb, cf);
	netif_receive_skb(skb);

	stats->rx_packets++;
	stats->rx_bytes += cf->can_dlc;
}

/**
 * at91_poll_rx - read multiple CAN messages from mailboxes
 * @dev: net device
 * @quota: max number of pkgs we're allowed to receive
 *
 * Theory of Operation:
 *
 * About 3/4 of the mailboxes (get_mb_rx_first()...get_mb_rx_last())
 * on the chip are reserved for RX. We split them into 2 groups. The
 * lower group ranges from get_mb_rx_first() to get_mb_rx_low_last().
 *
 * Like it or not, but the chip always saves a received CAN message
 * into the first free mailbox it finds (starting with the
 * lowest). This makes it very difficult to read the messages in the
 * right order from the chip. This is how we work around that problem:
 *
 * The first message goes into mb nr. 1 and issues an interrupt. All
 * rx ints are disabled in the interrupt handler and a napi poll is
 * scheduled. We read the mailbox, but do _not_ reenable the mb (to
 * receive another message).
 *
 *    lower mbxs      upper
 *     ____^______    __^__
 *    /           \  /     \
 * +-+-+-+-+-+-+-+-++-+-+-+-+
 * | |x|x|x|x|x|x|x|| | | | |
 * +-+-+-+-+-+-+-+-++-+-+-+-+
 *  0 0 0 0 0 0  0 0 0 0 1 1  \ mail
 *  0 1 2 3 4 5  6 7 8 9 0 1  / box
 *  ^
 *  |
 *   \
 *     unused, due to chip bug
 *
 * The variable priv->rx_next points to the next mailbox to read a
 * message from. As long we're in the lower mailboxes we just read the
 * mailbox but not reenable it.
 *
 * With completion of the last of the lower mailboxes, we reenable the
 * whole first group, but continue to look for filled mailboxes in the
 * upper mailboxes. Imagine the second group like overflow mailboxes,
 * which takes CAN messages if the lower goup is full. While in the
 * upper group we reenable the mailbox right after reading it. Giving
 * the chip more room to store messages.
 *
 * After finishing we look again in the lower group if we've still
 * quota.
 *
 */
static int at91_poll_rx(struct net_device *dev, int quota)
{
	struct at91_priv *priv = netdev_priv(dev);
	u32 reg_sr = at91_read(priv, AT91_SR);
	const unsigned long *addr = (unsigned long *)&reg_sr;
	unsigned int mb;
	int received = 0;

	if (priv->rx_next > get_mb_rx_low_last(priv) &&
	    reg_sr & get_mb_rx_low_mask(priv))
		netdev_info(dev,
			"order of incoming frames cannot be guaranteed\n");

 again:
	for (mb = find_next_bit(addr, get_mb_tx_first(priv), priv->rx_next);
	     mb < get_mb_tx_first(priv) && quota > 0;
	     reg_sr = at91_read(priv, AT91_SR),
	     mb = find_next_bit(addr, get_mb_tx_first(priv), ++priv->rx_next)) {
		at91_read_msg(dev, mb);

		/* reactivate mailboxes */
		if (mb == get_mb_rx_low_last(priv))
			/* all lower mailboxed, if just finished it */
			at91_activate_rx_low(priv);
		else if (mb > get_mb_rx_low_last(priv))
			/* only the mailbox we read */
			at91_activate_rx_mb(priv, mb);

		received++;
		quota--;
	}

	/* upper group completed, look again in lower */
	if (priv->rx_next > get_mb_rx_low_last(priv) &&
	    quota > 0 && mb > get_mb_rx_last(priv)) {
		priv->rx_next = get_mb_rx_first(priv);
		goto again;
	}

	return received;
}

static void at91_poll_err_frame(struct net_device *dev,
		struct can_frame *cf, u32 reg_sr)
{
	struct at91_priv *priv = netdev_priv(dev);

	/* CRC error */
	if (reg_sr & AT91_IRQ_CERR) {
		netdev_dbg(dev, "CERR irq\n");
		dev->stats.rx_errors++;
		priv->can.can_stats.bus_error++;
		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;
	}

	/* Stuffing Error */
	if (reg_sr & AT91_IRQ_SERR) {
		netdev_dbg(dev, "SERR irq\n");
		dev->stats.rx_errors++;
		priv->can.can_stats.bus_error++;
		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;
		cf->data[2] |= CAN_ERR_PROT_STUFF;
	}

	/* Acknowledgement Error */
	if (reg_sr & AT91_IRQ_AERR) {
		netdev_dbg(dev, "AERR irq\n");
		dev->stats.tx_errors++;
		cf->can_id |= CAN_ERR_ACK;
	}

	/* Form error */
	if (reg_sr & AT91_IRQ_FERR) {
		netdev_dbg(dev, "FERR irq\n");
		dev->stats.rx_errors++;
		priv->can.can_stats.bus_error++;
		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;
		cf->data[2] |= CAN_ERR_PROT_FORM;
	}

	/* Bit Error */
	if (reg_sr & AT91_IRQ_BERR) {
		netdev_dbg(dev, "BERR irq\n");
		dev->stats.tx_errors++;
		priv->can.can_stats.bus_error++;
		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;
		cf->data[2] |= CAN_ERR_PROT_BIT;
	}
}

static int at91_poll_err(struct net_device *dev, int quota, u32 reg_sr)
{
	struct sk_buff *skb;
	struct can_frame *cf;

	if (quota == 0)
		return 0;

	skb = alloc_can_err_skb(dev, &cf);
	if (unlikely(!skb))
		return 0;

	at91_poll_err_frame(dev, cf, reg_sr);
	netif_receive_skb(skb);

	dev->stats.rx_packets++;
	dev->stats.rx_bytes += cf->can_dlc;

	return 1;
}

static int at91_poll(struct napi_struct *napi, int quota)
{
	struct net_device *dev = napi->dev;
	const struct at91_priv *priv = netdev_priv(dev);
	u32 reg_sr = at91_read(priv, AT91_SR);
	int work_done = 0;

	if (reg_sr & get_irq_mb_rx(priv))
		work_done += at91_poll_rx(dev, quota - work_done);

	/*
	 * The error bits are clear on read,
	 * so use saved value from irq handler.
	 */
	reg_sr |= priv->reg_sr;
	if (reg_sr & AT91_IRQ_ERR_FRAME)
		work_done += at91_poll_err(dev, quota - work_done, reg_sr);

	if (work_done < quota) {
		/* enable IRQs for frame errors and all mailboxes >= rx_next */
		u32 reg_ier = AT91_IRQ_ERR_FRAME;
		reg_ier |= get_irq_mb_rx(priv) & ~AT91_MB_MASK(priv->rx_next);

		napi_complete(napi);
		at91_write(priv, AT91_IER, reg_ier);
	}

	return work_done;
}

/*
 * theory of operation:
 *
 * priv->tx_echo holds the number of the oldest can_frame put for
 * transmission into the hardware, but not yet ACKed by the CAN tx
 * complete IRQ.
 *
 * We iterate from priv->tx_echo to priv->tx_next and check if the
=======
		at91_rx_overflow_err(offload->dev);

 mark_as_read:
	at91_write(priv, AT91_MCR(mb), AT91_MCR_MTCR);

	return skb;
}

/* theory of operation:
 *
 * priv->tx_tail holds the number of the oldest can_frame put for
 * transmission into the hardware, but not yet ACKed by the CAN tx
 * complete IRQ.
 *
 * We iterate from priv->tx_tail to priv->tx_head and check if the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * packet has been transmitted, echo it back to the CAN framework. If
 * we discover a not yet transmitted package, stop looking for more.
 *
 */
static void at91_irq_tx(struct net_device *dev, u32 reg_sr)
{
	struct at91_priv *priv = netdev_priv(dev);
	u32 reg_msr;
	unsigned int mb;

<<<<<<< HEAD
	/* masking of reg_sr not needed, already done by at91_irq */

	for (/* nix */; (priv->tx_next - priv->tx_echo) > 0; priv->tx_echo++) {
		mb = get_tx_echo_mb(priv);
=======
	for (/* nix */; (priv->tx_head - priv->tx_tail) > 0; priv->tx_tail++) {
		mb = get_tx_tail_mb(priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* no event in mailbox? */
		if (!(reg_sr & (1 << mb)))
			break;

		/* Disable irq for this TX mailbox */
		at91_write(priv, AT91_IDR, 1 << mb);

<<<<<<< HEAD
		/*
		 * only echo if mailbox signals us a transfer
=======
		/* only echo if mailbox signals us a transfer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * complete (MSR_MRDY). Otherwise it's a tansfer
		 * abort. "can_bus_off()" takes care about the skbs
		 * parked in the echo queue.
		 */
		reg_msr = at91_read(priv, AT91_MSR(mb));
<<<<<<< HEAD
		if (likely(reg_msr & AT91_MSR_MRDY &&
			   ~reg_msr & AT91_MSR_MABT)) {
			/* _NOTE_: subtract AT91_MB_TX_FIRST offset from mb! */
			can_get_echo_skb(dev, mb - get_mb_tx_first(priv));
			dev->stats.tx_packets++;
		}
	}

	/*
	 * restart queue if we don't have a wrap around but restart if
	 * we get a TX int for the last can frame directly before a
	 * wrap around.
	 */
	if ((priv->tx_next & get_next_mask(priv)) != 0 ||
	    (priv->tx_echo & get_next_mask(priv)) == 0)
		netif_wake_queue(dev);
}

static void at91_irq_err_state(struct net_device *dev,
		struct can_frame *cf, enum can_state new_state)
{
	struct at91_priv *priv = netdev_priv(dev);
	u32 reg_idr = 0, reg_ier = 0;
	struct can_berr_counter bec;

	at91_get_berr_counter(dev, &bec);

	switch (priv->can.state) {
	case CAN_STATE_ERROR_ACTIVE:
		/*
		 * from: ERROR_ACTIVE
		 * to  : ERROR_WARNING, ERROR_PASSIVE, BUS_OFF
		 * =>  : there was a warning int
		 */
		if (new_state >= CAN_STATE_ERROR_WARNING &&
		    new_state <= CAN_STATE_BUS_OFF) {
			netdev_dbg(dev, "Error Warning IRQ\n");
			priv->can.can_stats.error_warning++;

			cf->can_id |= CAN_ERR_CRTL;
			cf->data[1] = (bec.txerr > bec.rxerr) ?
				CAN_ERR_CRTL_TX_WARNING :
				CAN_ERR_CRTL_RX_WARNING;
		}
	case CAN_STATE_ERROR_WARNING:	/* fallthrough */
		/*
		 * from: ERROR_ACTIVE, ERROR_WARNING
		 * to  : ERROR_PASSIVE, BUS_OFF
		 * =>  : error passive int
		 */
		if (new_state >= CAN_STATE_ERROR_PASSIVE &&
		    new_state <= CAN_STATE_BUS_OFF) {
			netdev_dbg(dev, "Error Passive IRQ\n");
			priv->can.can_stats.error_passive++;

			cf->can_id |= CAN_ERR_CRTL;
			cf->data[1] = (bec.txerr > bec.rxerr) ?
				CAN_ERR_CRTL_TX_PASSIVE :
				CAN_ERR_CRTL_RX_PASSIVE;
		}
		break;
	case CAN_STATE_BUS_OFF:
		/*
		 * from: BUS_OFF
		 * to  : ERROR_ACTIVE, ERROR_WARNING, ERROR_PASSIVE
		 */
		if (new_state <= CAN_STATE_ERROR_PASSIVE) {
			cf->can_id |= CAN_ERR_RESTARTED;

			netdev_dbg(dev, "restarted\n");
			priv->can.can_stats.restarts++;

			netif_carrier_on(dev);
			netif_wake_queue(dev);
		}
		break;
	default:
		break;
	}


	/* process state changes depending on the new state */
	switch (new_state) {
	case CAN_STATE_ERROR_ACTIVE:
		/*
		 * actually we want to enable AT91_IRQ_WARN here, but
		 * it screws up the system under certain
		 * circumstances. so just enable AT91_IRQ_ERRP, thus
		 * the "fallthrough"
		 */
		netdev_dbg(dev, "Error Active\n");
		cf->can_id |= CAN_ERR_PROT;
		cf->data[2] = CAN_ERR_PROT_ACTIVE;
	case CAN_STATE_ERROR_WARNING:	/* fallthrough */
		reg_idr = AT91_IRQ_ERRA | AT91_IRQ_WARN | AT91_IRQ_BOFF;
		reg_ier = AT91_IRQ_ERRP;
		break;
	case CAN_STATE_ERROR_PASSIVE:
		reg_idr = AT91_IRQ_ERRA | AT91_IRQ_WARN | AT91_IRQ_ERRP;
		reg_ier = AT91_IRQ_BOFF;
		break;
	case CAN_STATE_BUS_OFF:
		reg_idr = AT91_IRQ_ERRA | AT91_IRQ_ERRP |
			AT91_IRQ_WARN | AT91_IRQ_BOFF;
		reg_ier = 0;

		cf->can_id |= CAN_ERR_BUSOFF;

		netdev_dbg(dev, "bus-off\n");
		netif_carrier_off(dev);
		priv->can.can_stats.bus_off++;

		/* turn off chip, if restart is disabled */
		if (!priv->can.restart_ms) {
			at91_chip_stop(dev, CAN_STATE_BUS_OFF);
			return;
		}
		break;
	default:
		break;
	}

	at91_write(priv, AT91_IDR, reg_idr);
	at91_write(priv, AT91_IER, reg_ier);
}

static int at91_get_state_by_bec(const struct net_device *dev,
		enum can_state *state)
{
	struct can_berr_counter bec;
	int err;

	err = at91_get_berr_counter(dev, &bec);
	if (err)
		return err;

	if (bec.txerr < 96 && bec.rxerr < 96)
		*state = CAN_STATE_ERROR_ACTIVE;
	else if (bec.txerr < 128 && bec.rxerr < 128)
		*state = CAN_STATE_ERROR_WARNING;
	else if (bec.txerr < 256 && bec.rxerr < 256)
		*state = CAN_STATE_ERROR_PASSIVE;
	else
		*state = CAN_STATE_BUS_OFF;

	return 0;
}


static void at91_irq_err(struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);
	struct sk_buff *skb;
	struct can_frame *cf;
	enum can_state new_state;
	u32 reg_sr;
	int err;

	if (at91_is_sam9263(priv)) {
		reg_sr = at91_read(priv, AT91_SR);

		/* we need to look at the unmasked reg_sr */
		if (unlikely(reg_sr & AT91_IRQ_BOFF))
			new_state = CAN_STATE_BUS_OFF;
		else if (unlikely(reg_sr & AT91_IRQ_ERRP))
			new_state = CAN_STATE_ERROR_PASSIVE;
		else if (unlikely(reg_sr & AT91_IRQ_WARN))
			new_state = CAN_STATE_ERROR_WARNING;
		else if (likely(reg_sr & AT91_IRQ_ERRA))
			new_state = CAN_STATE_ERROR_ACTIVE;
		else {
			netdev_err(dev, "BUG! hardware in undefined state\n");
			return;
		}
	} else {
		err = at91_get_state_by_bec(dev, &new_state);
		if (err)
			return;
	}
=======
		if (unlikely(!(reg_msr & AT91_MSR_MRDY &&
			       ~reg_msr & AT91_MSR_MABT)))
			continue;

		/* _NOTE_: subtract AT91_MB_TX_FIRST offset from mb! */
		dev->stats.tx_bytes +=
			can_get_echo_skb(dev, mb - get_mb_tx_first(priv), NULL);
		dev->stats.tx_packets++;
	}

	/* restart queue if we don't have a wrap around but restart if
	 * we get a TX int for the last can frame directly before a
	 * wrap around.
	 */
	if ((priv->tx_head & get_head_mask(priv)) != 0 ||
	    (priv->tx_tail & get_head_mask(priv)) == 0)
		netif_wake_queue(dev);
}

static void at91_irq_err_line(struct net_device *dev, const u32 reg_sr)
{
	struct net_device_stats *stats = &dev->stats;
	enum can_state new_state, rx_state, tx_state;
	struct at91_priv *priv = netdev_priv(dev);
	struct can_berr_counter bec;
	struct sk_buff *skb;
	struct can_frame *cf;
	u32 timestamp;
	int err;

	at91_get_berr_counter(dev, &bec);
	can_state_get_by_berr_counter(dev, &bec, &tx_state, &rx_state);

	/* The chip automatically recovers from bus-off after 128
	 * occurrences of 11 consecutive recessive bits.
	 *
	 * After an auto-recovered bus-off, the error counters no
	 * longer reflect this fact. On the sam9263 the state bits in
	 * the SR register show the current state (based on the
	 * current error counters), while on sam9x5 and newer SoCs
	 * these bits are latched.
	 *
	 * Take any latched bus-off information from the SR register
	 * into account when calculating the CAN new state, to start
	 * the standard CAN bus off handling.
	 */
	if (reg_sr & AT91_IRQ_BOFF)
		rx_state = CAN_STATE_BUS_OFF;

	new_state = max(tx_state, rx_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* state hasn't changed */
	if (likely(new_state == priv->can.state))
		return;

<<<<<<< HEAD
	skb = alloc_can_err_skb(dev, &cf);
	if (unlikely(!skb))
		return;

	at91_irq_err_state(dev, cf, new_state);
	netif_rx(skb);

	dev->stats.rx_packets++;
	dev->stats.rx_bytes += cf->can_dlc;

	priv->can.state = new_state;
}

/*
 * interrupt handler
 */
=======
	/* The skb allocation might fail, but can_change_state()
	 * handles cf == NULL.
	 */
	skb = at91_alloc_can_err_skb(dev, &cf, &timestamp);
	can_change_state(dev, cf, tx_state, rx_state);

	if (new_state == CAN_STATE_BUS_OFF) {
		at91_chip_stop(dev, CAN_STATE_BUS_OFF);
		can_bus_off(dev);
	}

	if (unlikely(!skb))
		return;

	if (new_state != CAN_STATE_BUS_OFF) {
		cf->can_id |= CAN_ERR_CNT;
		cf->data[6] = bec.txerr;
		cf->data[7] = bec.rxerr;
	}

	err = can_rx_offload_queue_timestamp(&priv->offload, skb, timestamp);
	if (err)
		stats->rx_fifo_errors++;
}

static void at91_irq_err_frame(struct net_device *dev, const u32 reg_sr)
{
	struct net_device_stats *stats = &dev->stats;
	struct at91_priv *priv = netdev_priv(dev);
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 timestamp;
	int err;

	priv->can.can_stats.bus_error++;

	skb = at91_alloc_can_err_skb(dev, &cf, &timestamp);
	if (cf)
		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;

	if (reg_sr & AT91_IRQ_CERR) {
		netdev_dbg(dev, "CRC error\n");

		stats->rx_errors++;
		if (cf)
			cf->data[3] |= CAN_ERR_PROT_LOC_CRC_SEQ;
	}

	if (reg_sr & AT91_IRQ_SERR) {
		netdev_dbg(dev, "Stuff error\n");

		stats->rx_errors++;
		if (cf)
			cf->data[2] |= CAN_ERR_PROT_STUFF;
	}

	if (reg_sr & AT91_IRQ_AERR) {
		netdev_dbg(dev, "NACK error\n");

		stats->tx_errors++;
		if (cf) {
			cf->can_id |= CAN_ERR_ACK;
			cf->data[2] |= CAN_ERR_PROT_TX;
		}
	}

	if (reg_sr & AT91_IRQ_FERR) {
		netdev_dbg(dev, "Format error\n");

		stats->rx_errors++;
		if (cf)
			cf->data[2] |= CAN_ERR_PROT_FORM;
	}

	if (reg_sr & AT91_IRQ_BERR) {
		netdev_dbg(dev, "Bit error\n");

		stats->tx_errors++;
		if (cf)
			cf->data[2] |= CAN_ERR_PROT_TX | CAN_ERR_PROT_BIT;
	}

	if (!cf)
		return;

	err = can_rx_offload_queue_timestamp(&priv->offload, skb, timestamp);
	if (err)
		stats->rx_fifo_errors++;
}

static u32 at91_get_reg_sr_rx(const struct at91_priv *priv, u32 *reg_sr_p)
{
	const u32 reg_sr = at91_read(priv, AT91_SR);

	*reg_sr_p |= reg_sr;

	return reg_sr & get_irq_mb_rx(priv);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static irqreturn_t at91_irq(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	struct at91_priv *priv = netdev_priv(dev);
	irqreturn_t handled = IRQ_NONE;
<<<<<<< HEAD
	u32 reg_sr, reg_imr;

	reg_sr = at91_read(priv, AT91_SR);
	reg_imr = at91_read(priv, AT91_IMR);

	/* Ignore masked interrupts */
	reg_sr &= reg_imr;
	if (!reg_sr)
		goto exit;

	handled = IRQ_HANDLED;

	/* Receive or error interrupt? -> napi */
	if (reg_sr & (get_irq_mb_rx(priv) | AT91_IRQ_ERR_FRAME)) {
		/*
		 * The error bits are clear on read,
		 * save for later use.
		 */
		priv->reg_sr = reg_sr;
		at91_write(priv, AT91_IDR,
			   get_irq_mb_rx(priv) | AT91_IRQ_ERR_FRAME);
		napi_schedule(&priv->napi);
	}

	/* Transmission complete interrupt */
	if (reg_sr & get_irq_mb_tx(priv))
		at91_irq_tx(dev, reg_sr);

	at91_irq_err(dev);

 exit:
=======
	u32 reg_sr = 0, reg_sr_rx;
	int ret;

	/* Receive interrupt
	 * Some bits of AT91_SR are cleared on read, keep them in reg_sr.
	 */
	while ((reg_sr_rx = at91_get_reg_sr_rx(priv, &reg_sr))) {
		ret = can_rx_offload_irq_offload_timestamp(&priv->offload,
							   reg_sr_rx);
		handled = IRQ_HANDLED;

		if (!ret)
			break;
	}

	/* Transmission complete interrupt */
	if (reg_sr & get_irq_mb_tx(priv)) {
		at91_irq_tx(dev, reg_sr);
		handled = IRQ_HANDLED;
	}

	/* Line Error interrupt */
	if (reg_sr & AT91_IRQ_ERR_LINE ||
	    priv->can.state > CAN_STATE_ERROR_ACTIVE) {
		at91_irq_err_line(dev, reg_sr);
		handled = IRQ_HANDLED;
	}

	/* Frame Error Interrupt */
	if (reg_sr & AT91_IRQ_ERR_FRAME) {
		at91_irq_err_frame(dev, reg_sr);
		handled = IRQ_HANDLED;
	}

	if (handled)
		can_rx_offload_irq_finish(&priv->offload);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return handled;
}

static int at91_open(struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);
	int err;

<<<<<<< HEAD
	err = clk_prepare_enable(priv->clk);
=======
	err = phy_power_on(priv->transceiver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	/* check or determine and set bittime */
	err = open_candev(dev);
	if (err)
<<<<<<< HEAD
		goto out;

	/* register interrupt handler */
	if (request_irq(dev->irq, at91_irq, IRQF_SHARED,
			dev->name, dev)) {
		err = -EAGAIN;
		goto out_close;
	}

	/* start chip and queuing */
	at91_chip_start(dev);
	napi_enable(&priv->napi);
=======
		goto out_phy_power_off;

	err = clk_prepare_enable(priv->clk);
	if (err)
		goto out_close_candev;

	/* register interrupt handler */
	err = request_irq(dev->irq, at91_irq, IRQF_SHARED,
			  dev->name, dev);
	if (err)
		goto out_clock_disable_unprepare;

	/* start chip and queuing */
	at91_chip_start(dev);
	can_rx_offload_enable(&priv->offload);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netif_start_queue(dev);

	return 0;

<<<<<<< HEAD
 out_close:
	close_candev(dev);
 out:
	clk_disable_unprepare(priv->clk);
=======
 out_clock_disable_unprepare:
	clk_disable_unprepare(priv->clk);
 out_close_candev:
	close_candev(dev);
 out_phy_power_off:
	phy_power_off(priv->transceiver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
/*
 * stop CAN bus activity
=======
/* stop CAN bus activity
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int at91_close(struct net_device *dev)
{
	struct at91_priv *priv = netdev_priv(dev);

	netif_stop_queue(dev);
<<<<<<< HEAD
	napi_disable(&priv->napi);
=======
	can_rx_offload_disable(&priv->offload);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	at91_chip_stop(dev, CAN_STATE_STOPPED);

	free_irq(dev->irq, dev);
	clk_disable_unprepare(priv->clk);
<<<<<<< HEAD
=======
	phy_power_off(priv->transceiver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	close_candev(dev);

	return 0;
}

static int at91_set_mode(struct net_device *dev, enum can_mode mode)
{
	switch (mode) {
	case CAN_MODE_START:
		at91_chip_start(dev);
		netif_wake_queue(dev);
		break;

	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static const struct net_device_ops at91_netdev_ops = {
	.ndo_open	= at91_open,
	.ndo_stop	= at91_close,
	.ndo_start_xmit	= at91_start_xmit,
<<<<<<< HEAD
};

static ssize_t at91_sysfs_show_mb0_id(struct device *dev,
		struct device_attribute *attr, char *buf)
=======
	.ndo_change_mtu = can_change_mtu,
};

static const struct ethtool_ops at91_ethtool_ops = {
	.get_ts_info = ethtool_op_get_ts_info,
};

static ssize_t mb0_id_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct at91_priv *priv = netdev_priv(to_net_dev(dev));

	if (priv->mb0_id & CAN_EFF_FLAG)
<<<<<<< HEAD
		return snprintf(buf, PAGE_SIZE, "0x%08x\n", priv->mb0_id);
	else
		return snprintf(buf, PAGE_SIZE, "0x%03x\n", priv->mb0_id);
}

static ssize_t at91_sysfs_set_mb0_id(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
=======
		return sysfs_emit(buf, "0x%08x\n", priv->mb0_id);
	else
		return sysfs_emit(buf, "0x%03x\n", priv->mb0_id);
}

static ssize_t mb0_id_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *ndev = to_net_dev(dev);
	struct at91_priv *priv = netdev_priv(ndev);
	unsigned long can_id;
	ssize_t ret;
	int err;

	rtnl_lock();

	if (ndev->flags & IFF_UP) {
		ret = -EBUSY;
		goto out;
	}

<<<<<<< HEAD
	err = strict_strtoul(buf, 0, &can_id);
=======
	err = kstrtoul(buf, 0, &can_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		ret = err;
		goto out;
	}

	if (can_id & CAN_EFF_FLAG)
		can_id &= CAN_EFF_MASK | CAN_EFF_FLAG;
	else
		can_id &= CAN_SFF_MASK;

	priv->mb0_id = can_id;
	ret = count;

 out:
	rtnl_unlock();
	return ret;
}

<<<<<<< HEAD
static DEVICE_ATTR(mb0_id, S_IWUSR | S_IRUGO,
	at91_sysfs_show_mb0_id, at91_sysfs_set_mb0_id);
=======
static DEVICE_ATTR_RW(mb0_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *at91_sysfs_attrs[] = {
	&dev_attr_mb0_id.attr,
	NULL,
};

<<<<<<< HEAD
static struct attribute_group at91_sysfs_attr_group = {
	.attrs = at91_sysfs_attrs,
};

static int __devinit at91_can_probe(struct platform_device *pdev)
{
	const struct at91_devtype_data *devtype_data;
	enum at91_devtype devtype;
=======
static const struct attribute_group at91_sysfs_attr_group = {
	.attrs = at91_sysfs_attrs,
};

#if defined(CONFIG_OF)
static const struct of_device_id at91_can_dt_ids[] = {
	{
		.compatible = "atmel,at91sam9x5-can",
		.data = &at91_at91sam9x5_data,
	}, {
		.compatible = "atmel,at91sam9263-can",
		.data = &at91_at91sam9263_data,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, at91_can_dt_ids);
#endif

static const struct at91_devtype_data *at91_can_get_driver_data(struct platform_device *pdev)
{
	if (pdev->dev.of_node) {
		const struct of_device_id *match;

		match = of_match_node(at91_can_dt_ids, pdev->dev.of_node);
		if (!match) {
			dev_err(&pdev->dev, "no matching node found in dtb\n");
			return NULL;
		}
		return (const struct at91_devtype_data *)match->data;
	}
	return (const struct at91_devtype_data *)
		platform_get_device_id(pdev)->driver_data;
}

static int at91_can_probe(struct platform_device *pdev)
{
	const struct at91_devtype_data *devtype_data;
	struct phy *transceiver;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev;
	struct at91_priv *priv;
	struct resource *res;
	struct clk *clk;
	void __iomem *addr;
	int err, irq;

<<<<<<< HEAD
	devtype = pdev->id_entry->driver_data;
	devtype_data = &at91_devtype_data[devtype];
=======
	devtype_data = at91_can_get_driver_data(pdev);
	if (!devtype_data) {
		dev_err(&pdev->dev, "no driver data\n");
		err = -ENODEV;
		goto exit;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clk = clk_get(&pdev->dev, "can_clk");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "no clock defined\n");
		err = -ENODEV;
		goto exit;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!res || irq <= 0) {
		err = -ENODEV;
		goto exit_put;
	}

	if (!request_mem_region(res->start,
				resource_size(res),
				pdev->name)) {
		err = -EBUSY;
		goto exit_put;
	}

<<<<<<< HEAD
	addr = ioremap_nocache(res->start, resource_size(res));
=======
	addr = ioremap(res->start, resource_size(res));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!addr) {
		err = -ENOMEM;
		goto exit_release;
	}

	dev = alloc_candev(sizeof(struct at91_priv),
			   1 << devtype_data->tx_shift);
	if (!dev) {
		err = -ENOMEM;
		goto exit_iounmap;
	}

<<<<<<< HEAD
	dev->netdev_ops	= &at91_netdev_ops;
=======
	transceiver = devm_phy_optional_get(&pdev->dev, NULL);
	if (IS_ERR(transceiver)) {
		err = PTR_ERR(transceiver);
		dev_err_probe(&pdev->dev, err, "failed to get phy\n");
		goto exit_iounmap;
	}

	dev->netdev_ops	= &at91_netdev_ops;
	dev->ethtool_ops = &at91_ethtool_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev->irq = irq;
	dev->flags |= IFF_ECHO;

	priv = netdev_priv(dev);
	priv->can.clock.freq = clk_get_rate(clk);
	priv->can.bittiming_const = &at91_bittiming_const;
	priv->can.do_set_mode = at91_set_mode;
	priv->can.do_get_berr_counter = at91_get_berr_counter;
<<<<<<< HEAD
	priv->can.ctrlmode_supported = CAN_CTRLMODE_3_SAMPLES;
	priv->dev = dev;
	priv->reg_base = addr;
	priv->devtype_data = *devtype_data;
	priv->devtype_data.type = devtype;
	priv->clk = clk;
	priv->pdata = pdev->dev.platform_data;
	priv->mb0_id = 0x7ff;

	netif_napi_add(dev, &priv->napi, at91_poll, get_mb_rx_num(priv));
=======
	priv->can.ctrlmode_supported = CAN_CTRLMODE_3_SAMPLES |
		CAN_CTRLMODE_LISTENONLY;
	priv->reg_base = addr;
	priv->devtype_data = *devtype_data;
	priv->clk = clk;
	priv->pdata = dev_get_platdata(&pdev->dev);
	priv->mb0_id = 0x7ff;
	priv->offload.mailbox_read = at91_mailbox_read;
	priv->offload.mb_first = devtype_data->rx_first;
	priv->offload.mb_last = devtype_data->rx_last;

	can_rx_offload_add_timestamp(dev, &priv->offload);

	if (transceiver)
		priv->can.bitrate_max = transceiver->attrs.max_link_rate;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (at91_is_sam9263(priv))
		dev->sysfs_groups[0] = &at91_sysfs_attr_group;

<<<<<<< HEAD
	dev_set_drvdata(&pdev->dev, dev);
=======
	platform_set_drvdata(pdev, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SET_NETDEV_DEV(dev, &pdev->dev);

	err = register_candev(dev);
	if (err) {
		dev_err(&pdev->dev, "registering netdev failed\n");
		goto exit_free;
	}

	dev_info(&pdev->dev, "device registered (reg_base=%p, irq=%d)\n",
		 priv->reg_base, dev->irq);

	return 0;

 exit_free:
	free_candev(dev);
 exit_iounmap:
	iounmap(addr);
 exit_release:
	release_mem_region(res->start, resource_size(res));
 exit_put:
	clk_put(clk);
 exit:
	return err;
}

<<<<<<< HEAD
static int __devexit at91_can_remove(struct platform_device *pdev)
=======
static void at91_can_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct at91_priv *priv = netdev_priv(dev);
	struct resource *res;

	unregister_netdev(dev);

<<<<<<< HEAD
	platform_set_drvdata(pdev, NULL);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iounmap(priv->reg_base);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

	clk_put(priv->clk);

	free_candev(dev);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct platform_device_id at91_can_id_table[] = {
	{
<<<<<<< HEAD
		.name = "at91_can",
		.driver_data = AT91_DEVTYPE_SAM9263,
	}, {
		.name = "at91sam9x5_can",
		.driver_data = AT91_DEVTYPE_SAM9X5,
=======
		.name = "at91sam9x5_can",
		.driver_data = (kernel_ulong_t)&at91_at91sam9x5_data,
	}, {
		.name = "at91_can",
		.driver_data = (kernel_ulong_t)&at91_at91sam9263_data,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}, {
		/* sentinel */
	}
};
<<<<<<< HEAD

static struct platform_driver at91_can_driver = {
	.probe = at91_can_probe,
	.remove = __devexit_p(at91_can_remove),
	.driver = {
		.name = KBUILD_MODNAME,
		.owner = THIS_MODULE,
=======
MODULE_DEVICE_TABLE(platform, at91_can_id_table);

static struct platform_driver at91_can_driver = {
	.probe = at91_can_probe,
	.remove_new = at91_can_remove,
	.driver = {
		.name = KBUILD_MODNAME,
		.of_match_table = of_match_ptr(at91_can_dt_ids),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.id_table = at91_can_id_table,
};

module_platform_driver(at91_can_driver);

MODULE_AUTHOR("Marc Kleine-Budde <mkl@pengutronix.de>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(KBUILD_MODNAME " CAN netdevice driver");
