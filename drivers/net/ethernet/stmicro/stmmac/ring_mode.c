<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
  Specialised functions for managing Ring mode

  Copyright(C) 2011  STMicroelectronics Ltd

  It defines all the functions used to handle the normal/enhanced
  descriptors in case of the DMA is configured to work in chained or
  in ring mode.

<<<<<<< HEAD
  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

#include "stmmac.h"

<<<<<<< HEAD
static unsigned int stmmac_jumbo_frm(void *p, struct sk_buff *skb, int csum)
{
	struct stmmac_priv *priv = (struct stmmac_priv *) p;
	unsigned int txsize = priv->dma_tx_size;
	unsigned int entry = priv->cur_tx % txsize;
	struct dma_desc *desc = priv->dma_tx + entry;
	unsigned int nopaged_len = skb_headlen(skb);
	unsigned int bmax, len;
=======
static int jumbo_frm(struct stmmac_tx_queue *tx_q, struct sk_buff *skb,
		     int csum)
{
	unsigned int nopaged_len = skb_headlen(skb);
	struct stmmac_priv *priv = tx_q->priv_data;
	unsigned int entry = tx_q->cur_tx;
	unsigned int bmax, len, des2;
	struct dma_desc *desc;

	if (priv->extend_desc)
		desc = (struct dma_desc *)(tx_q->dma_etx + entry);
	else
		desc = tx_q->dma_tx + entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (priv->plat->enh_desc)
		bmax = BUF_SIZE_8KiB;
	else
		bmax = BUF_SIZE_2KiB;

	len = nopaged_len - bmax;

	if (nopaged_len > BUF_SIZE_8KiB) {

<<<<<<< HEAD
		desc->des2 = dma_map_single(priv->device, skb->data,
					    bmax, DMA_TO_DEVICE);
		desc->des3 = desc->des2 + BUF_SIZE_4KiB;
		priv->hw->desc->prepare_tx_desc(desc, 1, bmax,
						csum);

		entry = (++priv->cur_tx) % txsize;
		desc = priv->dma_tx + entry;

		desc->des2 = dma_map_single(priv->device, skb->data + bmax,
					    len, DMA_TO_DEVICE);
		desc->des3 = desc->des2 + BUF_SIZE_4KiB;
		priv->hw->desc->prepare_tx_desc(desc, 0, len, csum);
		priv->hw->desc->set_tx_owner(desc);
		priv->tx_skbuff[entry] = NULL;
	} else {
		desc->des2 = dma_map_single(priv->device, skb->data,
					    nopaged_len, DMA_TO_DEVICE);
		desc->des3 = desc->des2 + BUF_SIZE_4KiB;
		priv->hw->desc->prepare_tx_desc(desc, 1, nopaged_len, csum);
	}

	return entry;
}

static unsigned int stmmac_is_jumbo_frm(int len, int enh_desc)
=======
		des2 = dma_map_single(priv->device, skb->data, bmax,
				      DMA_TO_DEVICE);
		desc->des2 = cpu_to_le32(des2);
		if (dma_mapping_error(priv->device, des2))
			return -1;

		tx_q->tx_skbuff_dma[entry].buf = des2;
		tx_q->tx_skbuff_dma[entry].len = bmax;
		tx_q->tx_skbuff_dma[entry].is_jumbo = true;

		desc->des3 = cpu_to_le32(des2 + BUF_SIZE_4KiB);
		stmmac_prepare_tx_desc(priv, desc, 1, bmax, csum,
				STMMAC_RING_MODE, 0, false, skb->len);
		tx_q->tx_skbuff[entry] = NULL;
		entry = STMMAC_GET_ENTRY(entry, priv->dma_conf.dma_tx_size);

		if (priv->extend_desc)
			desc = (struct dma_desc *)(tx_q->dma_etx + entry);
		else
			desc = tx_q->dma_tx + entry;

		des2 = dma_map_single(priv->device, skb->data + bmax, len,
				      DMA_TO_DEVICE);
		desc->des2 = cpu_to_le32(des2);
		if (dma_mapping_error(priv->device, des2))
			return -1;
		tx_q->tx_skbuff_dma[entry].buf = des2;
		tx_q->tx_skbuff_dma[entry].len = len;
		tx_q->tx_skbuff_dma[entry].is_jumbo = true;

		desc->des3 = cpu_to_le32(des2 + BUF_SIZE_4KiB);
		stmmac_prepare_tx_desc(priv, desc, 0, len, csum,
				STMMAC_RING_MODE, 1, !skb_is_nonlinear(skb),
				skb->len);
	} else {
		des2 = dma_map_single(priv->device, skb->data,
				      nopaged_len, DMA_TO_DEVICE);
		desc->des2 = cpu_to_le32(des2);
		if (dma_mapping_error(priv->device, des2))
			return -1;
		tx_q->tx_skbuff_dma[entry].buf = des2;
		tx_q->tx_skbuff_dma[entry].len = nopaged_len;
		tx_q->tx_skbuff_dma[entry].is_jumbo = true;
		desc->des3 = cpu_to_le32(des2 + BUF_SIZE_4KiB);
		stmmac_prepare_tx_desc(priv, desc, 1, nopaged_len, csum,
				STMMAC_RING_MODE, 0, !skb_is_nonlinear(skb),
				skb->len);
	}

	tx_q->cur_tx = entry;

	return entry;
}

static unsigned int is_jumbo_frm(int len, int enh_desc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int ret = 0;

	if (len >= BUF_SIZE_4KiB)
		ret = 1;

	return ret;
}

<<<<<<< HEAD
static void stmmac_refill_desc3(int bfsize, struct dma_desc *p)
{
	/* Fill DES3 in case of RING mode */
	if (bfsize >= BUF_SIZE_8KiB)
		p->des3 = p->des2 + BUF_SIZE_8KiB;
}

/* In ring mode we need to fill the desc3 because it is used
 * as buffer */
static void stmmac_init_desc3(int des3_as_data_buf, struct dma_desc *p)
{
	if (unlikely(des3_as_data_buf))
		p->des3 = p->des2 + BUF_SIZE_8KiB;
}

static void stmmac_init_dma_chain(struct dma_desc *des, dma_addr_t phy_addr,
				  unsigned int size)
{
}

static void stmmac_clean_desc3(struct dma_desc *p)
{
	if (unlikely(p->des3))
		p->des3 = 0;
}

static int stmmac_set_16kib_bfsize(int mtu)
{
	int ret = 0;
	if (unlikely(mtu >= BUF_SIZE_8KiB))
=======
static void refill_desc3(struct stmmac_rx_queue *rx_q, struct dma_desc *p)
{
	struct stmmac_priv *priv = rx_q->priv_data;

	/* Fill DES3 in case of RING mode */
	if (priv->dma_conf.dma_buf_sz == BUF_SIZE_16KiB)
		p->des3 = cpu_to_le32(le32_to_cpu(p->des2) + BUF_SIZE_8KiB);
}

/* In ring mode we need to fill the desc3 because it is used as buffer */
static void init_desc3(struct dma_desc *p)
{
	p->des3 = cpu_to_le32(le32_to_cpu(p->des2) + BUF_SIZE_8KiB);
}

static void clean_desc3(struct stmmac_tx_queue *tx_q, struct dma_desc *p)
{
	struct stmmac_priv *priv = tx_q->priv_data;
	unsigned int entry = tx_q->dirty_tx;

	/* des3 is only used for jumbo frames tx or time stamping */
	if (unlikely(tx_q->tx_skbuff_dma[entry].is_jumbo ||
		     (tx_q->tx_skbuff_dma[entry].last_segment &&
		      !priv->extend_desc && priv->hwts_tx_en)))
		p->des3 = 0;
}

static int set_16kib_bfsize(int mtu)
{
	int ret = 0;
	if (unlikely(mtu > BUF_SIZE_8KiB))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = BUF_SIZE_16KiB;
	return ret;
}

<<<<<<< HEAD
const struct stmmac_ring_mode_ops ring_mode_ops = {
	.is_jumbo_frm = stmmac_is_jumbo_frm,
	.jumbo_frm = stmmac_jumbo_frm,
	.refill_desc3 = stmmac_refill_desc3,
	.init_desc3 = stmmac_init_desc3,
	.init_dma_chain = stmmac_init_dma_chain,
	.clean_desc3 = stmmac_clean_desc3,
	.set_16kib_bfsize = stmmac_set_16kib_bfsize,
=======
const struct stmmac_mode_ops ring_mode_ops = {
	.is_jumbo_frm = is_jumbo_frm,
	.jumbo_frm = jumbo_frm,
	.refill_desc3 = refill_desc3,
	.init_desc3 = init_desc3,
	.clean_desc3 = clean_desc3,
	.set_16kib_bfsize = set_16kib_bfsize,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
