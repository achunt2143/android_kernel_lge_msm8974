<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
  Specialised functions for managing Chained mode

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
unsigned int stmmac_jumbo_frm(void *p, struct sk_buff *skb, int csum)
{
	struct stmmac_priv *priv = (struct stmmac_priv *) p;
	unsigned int txsize = priv->dma_tx_size;
	unsigned int entry = priv->cur_tx % txsize;
	struct dma_desc *desc = priv->dma_tx + entry;
	unsigned int nopaged_len = skb_headlen(skb);
	unsigned int bmax;
	unsigned int i = 1, len;
=======
static int jumbo_frm(struct stmmac_tx_queue *tx_q, struct sk_buff *skb,
		     int csum)
{
	unsigned int nopaged_len = skb_headlen(skb);
	struct stmmac_priv *priv = tx_q->priv_data;
	unsigned int entry = tx_q->cur_tx;
	unsigned int bmax, des2;
	unsigned int i = 1, len;
	struct dma_desc *desc;

	desc = tx_q->dma_tx + entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (priv->plat->enh_desc)
		bmax = BUF_SIZE_8KiB;
	else
		bmax = BUF_SIZE_2KiB;

	len = nopaged_len - bmax;

<<<<<<< HEAD
	desc->des2 = dma_map_single(priv->device, skb->data,
				    bmax, DMA_TO_DEVICE);
	priv->hw->desc->prepare_tx_desc(desc, 1, bmax, csum);

	while (len != 0) {
		entry = (++priv->cur_tx) % txsize;
		desc = priv->dma_tx + entry;

		if (len > bmax) {
			desc->des2 = dma_map_single(priv->device,
						    (skb->data + bmax * i),
						    bmax, DMA_TO_DEVICE);
			priv->hw->desc->prepare_tx_desc(desc, 0, bmax,
							csum);
			priv->hw->desc->set_tx_owner(desc);
			priv->tx_skbuff[entry] = NULL;
			len -= bmax;
			i++;
		} else {
			desc->des2 = dma_map_single(priv->device,
						    (skb->data + bmax * i), len,
						    DMA_TO_DEVICE);
			priv->hw->desc->prepare_tx_desc(desc, 0, len,
							csum);
			priv->hw->desc->set_tx_owner(desc);
			priv->tx_skbuff[entry] = NULL;
			len = 0;
		}
	}
	return entry;
}

static unsigned int stmmac_is_jumbo_frm(int len, int enh_desc)
=======
	des2 = dma_map_single(priv->device, skb->data,
			      bmax, DMA_TO_DEVICE);
	desc->des2 = cpu_to_le32(des2);
	if (dma_mapping_error(priv->device, des2))
		return -1;
	tx_q->tx_skbuff_dma[entry].buf = des2;
	tx_q->tx_skbuff_dma[entry].len = bmax;
	/* do not close the descriptor and do not set own bit */
	stmmac_prepare_tx_desc(priv, desc, 1, bmax, csum, STMMAC_CHAIN_MODE,
			0, false, skb->len);

	while (len != 0) {
		tx_q->tx_skbuff[entry] = NULL;
		entry = STMMAC_GET_ENTRY(entry, priv->dma_conf.dma_tx_size);
		desc = tx_q->dma_tx + entry;

		if (len > bmax) {
			des2 = dma_map_single(priv->device,
					      (skb->data + bmax * i),
					      bmax, DMA_TO_DEVICE);
			desc->des2 = cpu_to_le32(des2);
			if (dma_mapping_error(priv->device, des2))
				return -1;
			tx_q->tx_skbuff_dma[entry].buf = des2;
			tx_q->tx_skbuff_dma[entry].len = bmax;
			stmmac_prepare_tx_desc(priv, desc, 0, bmax, csum,
					STMMAC_CHAIN_MODE, 1, false, skb->len);
			len -= bmax;
			i++;
		} else {
			des2 = dma_map_single(priv->device,
					      (skb->data + bmax * i), len,
					      DMA_TO_DEVICE);
			desc->des2 = cpu_to_le32(des2);
			if (dma_mapping_error(priv->device, des2))
				return -1;
			tx_q->tx_skbuff_dma[entry].buf = des2;
			tx_q->tx_skbuff_dma[entry].len = len;
			/* last descriptor can be set now */
			stmmac_prepare_tx_desc(priv, desc, 0, len, csum,
					STMMAC_CHAIN_MODE, 1, true, skb->len);
			len = 0;
		}
	}

	tx_q->cur_tx = entry;

	return entry;
}

static unsigned int is_jumbo_frm(int len, int enh_desc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int ret = 0;

	if ((enh_desc && (len > BUF_SIZE_8KiB)) ||
	    (!enh_desc && (len > BUF_SIZE_2KiB))) {
		ret = 1;
	}

	return ret;
}

<<<<<<< HEAD
static void stmmac_refill_desc3(int bfsize, struct dma_desc *p)
{
}

static void stmmac_init_desc3(int des3_as_data_buf, struct dma_desc *p)
{
}

static void stmmac_clean_desc3(struct dma_desc *p)
{
}

static void stmmac_init_dma_chain(struct dma_desc *des, dma_addr_t phy_addr,
				  unsigned int size)
=======
static void init_dma_chain(void *des, dma_addr_t phy_addr,
				  unsigned int size, unsigned int extend_desc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * In chained mode the des3 points to the next element in the ring.
	 * The latest element has to point to the head.
	 */
	int i;
<<<<<<< HEAD
	struct dma_desc *p = des;
	dma_addr_t dma_phy = phy_addr;

	for (i = 0; i < (size - 1); i++) {
		dma_phy += sizeof(struct dma_desc);
		p->des3 = (unsigned int)dma_phy;
		p++;
	}
	p->des3 = (unsigned int)phy_addr;
}

static int stmmac_set_16kib_bfsize(int mtu)
{
	/* Not supported */
	return 0;
}

const struct stmmac_ring_mode_ops ring_mode_ops = {
	.is_jumbo_frm = stmmac_is_jumbo_frm,
	.jumbo_frm = stmmac_jumbo_frm,
	.refill_desc3 = stmmac_refill_desc3,
	.init_desc3 = stmmac_init_desc3,
	.init_dma_chain = stmmac_init_dma_chain,
	.clean_desc3 = stmmac_clean_desc3,
	.set_16kib_bfsize = stmmac_set_16kib_bfsize,
=======
	dma_addr_t dma_phy = phy_addr;

	if (extend_desc) {
		struct dma_extended_desc *p = (struct dma_extended_desc *)des;
		for (i = 0; i < (size - 1); i++) {
			dma_phy += sizeof(struct dma_extended_desc);
			p->basic.des3 = cpu_to_le32((unsigned int)dma_phy);
			p++;
		}
		p->basic.des3 = cpu_to_le32((unsigned int)phy_addr);

	} else {
		struct dma_desc *p = (struct dma_desc *)des;
		for (i = 0; i < (size - 1); i++) {
			dma_phy += sizeof(struct dma_desc);
			p->des3 = cpu_to_le32((unsigned int)dma_phy);
			p++;
		}
		p->des3 = cpu_to_le32((unsigned int)phy_addr);
	}
}

static void refill_desc3(struct stmmac_rx_queue *rx_q, struct dma_desc *p)
{
	struct stmmac_priv *priv = rx_q->priv_data;

	if (priv->hwts_rx_en && !priv->extend_desc)
		/* NOTE: Device will overwrite des3 with timestamp value if
		 * 1588-2002 time stamping is enabled, hence reinitialize it
		 * to keep explicit chaining in the descriptor.
		 */
		p->des3 = cpu_to_le32((unsigned int)(rx_q->dma_rx_phy +
				      (((rx_q->dirty_rx) + 1) %
				       priv->dma_conf.dma_rx_size) *
				      sizeof(struct dma_desc)));
}

static void clean_desc3(struct stmmac_tx_queue *tx_q, struct dma_desc *p)
{
	struct stmmac_priv *priv = tx_q->priv_data;
	unsigned int entry = tx_q->dirty_tx;

	if (tx_q->tx_skbuff_dma[entry].last_segment && !priv->extend_desc &&
	    priv->hwts_tx_en)
		/* NOTE: Device will overwrite des3 with timestamp value if
		 * 1588-2002 time stamping is enabled, hence reinitialize it
		 * to keep explicit chaining in the descriptor.
		 */
		p->des3 = cpu_to_le32((unsigned int)((tx_q->dma_tx_phy +
				      ((tx_q->dirty_tx + 1) %
				       priv->dma_conf.dma_tx_size))
				      * sizeof(struct dma_desc)));
}

const struct stmmac_mode_ops chain_mode_ops = {
	.init = init_dma_chain,
	.is_jumbo_frm = is_jumbo_frm,
	.jumbo_frm = jumbo_frm,
	.refill_desc3 = refill_desc3,
	.clean_desc3 = clean_desc3,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
