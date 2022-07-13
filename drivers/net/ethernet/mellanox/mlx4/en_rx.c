/*
 * Copyright (c) 2007 Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

<<<<<<< HEAD
=======
#include <linux/bpf.h>
#include <linux/bpf_trace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mlx4/cq.h>
#include <linux/slab.h>
#include <linux/mlx4/qp.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/vmalloc.h>

#include "mlx4_en.h"


static int mlx4_en_alloc_frag(struct mlx4_en_priv *priv,
			      struct mlx4_en_rx_desc *rx_desc,
			      struct page_frag *skb_frags,
			      struct mlx4_en_rx_alloc *ring_alloc,
			      int i)
{
	struct mlx4_en_frag_info *frag_info = &priv->frag_info[i];
	struct mlx4_en_rx_alloc *page_alloc = &ring_alloc[i];
	struct page *page;
	dma_addr_t dma;

	if (page_alloc->offset == frag_info->last_offset) {
		/* Allocate new page */
		page = alloc_pages(GFP_ATOMIC | __GFP_COMP, MLX4_EN_ALLOC_ORDER);
		if (!page)
			return -ENOMEM;

		skb_frags[i].page = page_alloc->page;
		skb_frags[i].offset = page_alloc->offset;
		page_alloc->page = page;
		page_alloc->offset = frag_info->frag_align;
	} else {
		page = page_alloc->page;
		get_page(page);

		skb_frags[i].page = page;
		skb_frags[i].offset = page_alloc->offset;
		page_alloc->offset += frag_info->frag_stride;
	}
	dma = dma_map_single(priv->ddev, page_address(skb_frags[i].page) +
			     skb_frags[i].offset, frag_info->frag_size,
			     PCI_DMA_FROMDEVICE);
	rx_desc->data[i].addr = cpu_to_be64(dma);
	return 0;
}

static int mlx4_en_init_allocator(struct mlx4_en_priv *priv,
				  struct mlx4_en_rx_ring *ring)
{
	struct mlx4_en_rx_alloc *page_alloc;
	int i;

	for (i = 0; i < priv->num_frags; i++) {
		page_alloc = &ring->page_alloc[i];
		page_alloc->page = alloc_pages(GFP_ATOMIC | __GFP_COMP,
					       MLX4_EN_ALLOC_ORDER);
		if (!page_alloc->page)
			goto out;

		page_alloc->offset = priv->frag_info[i].frag_align;
		en_dbg(DRV, priv, "Initialized allocator:%d with page:%p\n",
		       i, page_alloc->page);
	}
	return 0;

out:
	while (i--) {
		page_alloc = &ring->page_alloc[i];
		put_page(page_alloc->page);
		page_alloc->page = NULL;
	}
	return -ENOMEM;
}

static void mlx4_en_destroy_allocator(struct mlx4_en_priv *priv,
				      struct mlx4_en_rx_ring *ring)
{
	struct mlx4_en_rx_alloc *page_alloc;
	int i;

	for (i = 0; i < priv->num_frags; i++) {
		page_alloc = &ring->page_alloc[i];
		en_dbg(DRV, priv, "Freeing allocator:%d count:%d\n",
		       i, page_count(page_alloc->page));

		put_page(page_alloc->page);
		page_alloc->page = NULL;
	}
}


static void mlx4_en_init_rx_desc(struct mlx4_en_priv *priv,
				 struct mlx4_en_rx_ring *ring, int index)
{
	struct mlx4_en_rx_desc *rx_desc = ring->buf + ring->stride * index;
	struct skb_frag_struct *skb_frags = ring->rx_info +
					    (index << priv->log_rx_info);
=======
#include <linux/rculist.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/vmalloc.h>
#include <linux/irq.h>

#include <net/ip.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/ip6_checksum.h>
#endif

#include "mlx4_en.h"

static int mlx4_alloc_page(struct mlx4_en_priv *priv,
			   struct mlx4_en_rx_alloc *frag,
			   gfp_t gfp)
{
	struct page *page;
	dma_addr_t dma;

	page = alloc_page(gfp);
	if (unlikely(!page))
		return -ENOMEM;
	dma = dma_map_page(priv->ddev, page, 0, PAGE_SIZE, priv->dma_dir);
	if (unlikely(dma_mapping_error(priv->ddev, dma))) {
		__free_page(page);
		return -ENOMEM;
	}
	frag->page = page;
	frag->dma = dma;
	frag->page_offset = priv->rx_headroom;
	return 0;
}

static int mlx4_en_alloc_frags(struct mlx4_en_priv *priv,
			       struct mlx4_en_rx_ring *ring,
			       struct mlx4_en_rx_desc *rx_desc,
			       struct mlx4_en_rx_alloc *frags,
			       gfp_t gfp)
{
	int i;

	for (i = 0; i < priv->num_frags; i++, frags++) {
		if (!frags->page) {
			if (mlx4_alloc_page(priv, frags, gfp))
				return -ENOMEM;
			ring->rx_alloc_pages++;
		}
		rx_desc->data[i].addr = cpu_to_be64(frags->dma +
						    frags->page_offset);
	}
	return 0;
}

static void mlx4_en_free_frag(const struct mlx4_en_priv *priv,
			      struct mlx4_en_rx_alloc *frag)
{
	if (frag->page) {
		dma_unmap_page(priv->ddev, frag->dma,
			       PAGE_SIZE, priv->dma_dir);
		__free_page(frag->page);
	}
	/* We need to clear all fields, otherwise a change of priv->log_rx_info
	 * could lead to see garbage later in frag->page.
	 */
	memset(frag, 0, sizeof(*frag));
}

static void mlx4_en_init_rx_desc(const struct mlx4_en_priv *priv,
				 struct mlx4_en_rx_ring *ring, int index)
{
	struct mlx4_en_rx_desc *rx_desc = ring->buf + ring->stride * index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int possible_frags;
	int i;

	/* Set size and memtype fields */
	for (i = 0; i < priv->num_frags; i++) {
<<<<<<< HEAD
		skb_frag_size_set(&skb_frags[i], priv->frag_info[i].frag_size);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rx_desc->data[i].byte_count =
			cpu_to_be32(priv->frag_info[i].frag_size);
		rx_desc->data[i].lkey = cpu_to_be32(priv->mdev->mr.key);
	}

	/* If the number of used fragments does not fill up the ring stride,
	 * remaining (unused) fragments must be padded with null address/size
	 * and a special memory key */
	possible_frags = (ring->stride - sizeof(struct mlx4_en_rx_desc)) / DS_SIZE;
	for (i = priv->num_frags; i < possible_frags; i++) {
		rx_desc->data[i].byte_count = 0;
		rx_desc->data[i].lkey = cpu_to_be32(MLX4_EN_MEMTYPE_PAD);
		rx_desc->data[i].addr = 0;
	}
}

<<<<<<< HEAD

static int mlx4_en_prepare_rx_desc(struct mlx4_en_priv *priv,
				   struct mlx4_en_rx_ring *ring, int index)
{
	struct mlx4_en_rx_desc *rx_desc = ring->buf + (index * ring->stride);
	struct page_frag *skb_frags = ring->rx_info +
				      (index << priv->log_rx_info);
	int i;

	for (i = 0; i < priv->num_frags; i++)
		if (mlx4_en_alloc_frag(priv, rx_desc, skb_frags, ring->page_alloc, i))
			goto err;

	return 0;

err:
	while (i--) {
		dma_addr_t dma = be64_to_cpu(rx_desc->data[i].addr);
		pci_unmap_single(priv->mdev->pdev, dma, skb_frags[i].size,
				 PCI_DMA_FROMDEVICE);
		put_page(skb_frags[i].page);
	}
	return -ENOMEM;
=======
static int mlx4_en_prepare_rx_desc(struct mlx4_en_priv *priv,
				   struct mlx4_en_rx_ring *ring, int index,
				   gfp_t gfp)
{
	struct mlx4_en_rx_desc *rx_desc = ring->buf +
		(index << ring->log_stride);
	struct mlx4_en_rx_alloc *frags = ring->rx_info +
					(index << priv->log_rx_info);
	if (likely(ring->page_cache.index > 0)) {
		/* XDP uses a single page per frame */
		if (!frags->page) {
			ring->page_cache.index--;
			frags->page = ring->page_cache.buf[ring->page_cache.index].page;
			frags->dma  = ring->page_cache.buf[ring->page_cache.index].dma;
		}
		frags->page_offset = XDP_PACKET_HEADROOM;
		rx_desc->data[0].addr = cpu_to_be64(frags->dma +
						    XDP_PACKET_HEADROOM);
		return 0;
	}

	return mlx4_en_alloc_frags(priv, ring, rx_desc, frags, gfp);
}

static bool mlx4_en_is_ring_empty(const struct mlx4_en_rx_ring *ring)
{
	return ring->prod == ring->cons;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void mlx4_en_update_rx_prod_db(struct mlx4_en_rx_ring *ring)
{
	*ring->wqres.db.db = cpu_to_be32(ring->prod & 0xffff);
}

<<<<<<< HEAD
static void mlx4_en_free_rx_desc(struct mlx4_en_priv *priv,
				 struct mlx4_en_rx_ring *ring,
				 int index)
{
	struct page_frag *skb_frags;
	struct mlx4_en_rx_desc *rx_desc = ring->buf + (index << ring->log_stride);
	dma_addr_t dma;
	int nr;

	skb_frags = ring->rx_info + (index << priv->log_rx_info);
	for (nr = 0; nr < priv->num_frags; nr++) {
		en_dbg(DRV, priv, "Freeing fragment:%d\n", nr);
		dma = be64_to_cpu(rx_desc->data[nr].addr);

		en_dbg(DRV, priv, "Unmapping buffer at dma:0x%llx\n", (u64) dma);
		dma_unmap_single(priv->ddev, dma, skb_frags[nr].size,
				 PCI_DMA_FROMDEVICE);
		put_page(skb_frags[nr].page);
	}
}

=======
/* slow path */
static void mlx4_en_free_rx_desc(const struct mlx4_en_priv *priv,
				 struct mlx4_en_rx_ring *ring,
				 int index)
{
	struct mlx4_en_rx_alloc *frags;
	int nr;

	frags = ring->rx_info + (index << priv->log_rx_info);
	for (nr = 0; nr < priv->num_frags; nr++) {
		en_dbg(DRV, priv, "Freeing fragment:%d\n", nr);
		mlx4_en_free_frag(priv, frags + nr);
	}
}

/* Function not in fast-path */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mlx4_en_fill_rx_buffers(struct mlx4_en_priv *priv)
{
	struct mlx4_en_rx_ring *ring;
	int ring_ind;
	int buf_ind;
	int new_size;

	for (buf_ind = 0; buf_ind < priv->prof->rx_ring_size; buf_ind++) {
		for (ring_ind = 0; ring_ind < priv->rx_ring_num; ring_ind++) {
<<<<<<< HEAD
			ring = &priv->rx_ring[ring_ind];

			if (mlx4_en_prepare_rx_desc(priv, ring,
						    ring->actual_size)) {
				if (ring->actual_size < MLX4_EN_MIN_RX_SIZE) {
					en_err(priv, "Failed to allocate "
						     "enough rx buffers\n");
					return -ENOMEM;
				} else {
					new_size = rounddown_pow_of_two(ring->actual_size);
					en_warn(priv, "Only %d buffers allocated "
						      "reducing ring size to %d",
=======
			ring = priv->rx_ring[ring_ind];

			if (mlx4_en_prepare_rx_desc(priv, ring,
						    ring->actual_size,
						    GFP_KERNEL)) {
				if (ring->actual_size < MLX4_EN_MIN_RX_SIZE) {
					en_err(priv, "Failed to allocate enough rx buffers\n");
					return -ENOMEM;
				} else {
					new_size = rounddown_pow_of_two(ring->actual_size);
					en_warn(priv, "Only %d buffers allocated reducing ring size to %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						ring->actual_size, new_size);
					goto reduce_rings;
				}
			}
			ring->actual_size++;
			ring->prod++;
		}
	}
	return 0;

reduce_rings:
	for (ring_ind = 0; ring_ind < priv->rx_ring_num; ring_ind++) {
<<<<<<< HEAD
		ring = &priv->rx_ring[ring_ind];
=======
		ring = priv->rx_ring[ring_ind];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (ring->actual_size > new_size) {
			ring->actual_size--;
			ring->prod--;
			mlx4_en_free_rx_desc(priv, ring, ring->actual_size);
		}
	}

	return 0;
}

static void mlx4_en_free_rx_buf(struct mlx4_en_priv *priv,
				struct mlx4_en_rx_ring *ring)
{
	int index;

	en_dbg(DRV, priv, "Freeing Rx buf - cons:%d prod:%d\n",
	       ring->cons, ring->prod);

	/* Unmap and free Rx buffers */
<<<<<<< HEAD
	BUG_ON((u32) (ring->prod - ring->cons) > ring->actual_size);
	while (ring->cons != ring->prod) {
		index = ring->cons & ring->size_mask;
		en_dbg(DRV, priv, "Processing descriptor:%d\n", index);
		mlx4_en_free_rx_desc(priv, ring, index);
		++ring->cons;
=======
	for (index = 0; index < ring->size; index++) {
		en_dbg(DRV, priv, "Processing descriptor:%d\n", index);
		mlx4_en_free_rx_desc(priv, ring, index);
	}
	ring->cons = 0;
	ring->prod = 0;
}

void mlx4_en_set_num_rx_rings(struct mlx4_en_dev *mdev)
{
	int i;
	int num_of_eqs;
	int num_rx_rings;
	struct mlx4_dev *dev = mdev->dev;

	mlx4_foreach_port(i, dev, MLX4_PORT_TYPE_ETH) {
		num_of_eqs = max_t(int, MIN_RX_RINGS,
				   min_t(int,
					 mlx4_get_eqs_per_port(mdev->dev, i),
					 DEF_RX_RINGS));

		num_rx_rings = mlx4_low_memory_profile() ? MIN_RX_RINGS :
			min_t(int, num_of_eqs, num_online_cpus());
		mdev->profile.prof[i].rx_ring_num =
			rounddown_pow_of_two(num_rx_rings);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

int mlx4_en_create_rx_ring(struct mlx4_en_priv *priv,
<<<<<<< HEAD
			   struct mlx4_en_rx_ring *ring, u32 size, u16 stride)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	int err;
	int tmp;

=======
			   struct mlx4_en_rx_ring **pring,
			   u32 size, u16 stride, int node, int queue_index)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	struct mlx4_en_rx_ring *ring;
	int err = -ENOMEM;
	int tmp;

	ring = kzalloc_node(sizeof(*ring), GFP_KERNEL, node);
	if (!ring) {
		en_err(priv, "Failed to allocate RX ring structure\n");
		return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ring->prod = 0;
	ring->cons = 0;
	ring->size = size;
	ring->size_mask = size - 1;
	ring->stride = stride;
	ring->log_stride = ffs(ring->stride) - 1;
	ring->buf_size = ring->size * ring->stride + TXBB_SIZE;

<<<<<<< HEAD
	tmp = size * roundup_pow_of_two(MLX4_EN_MAX_RX_FRAGS *
					sizeof(struct skb_frag_struct));
	ring->rx_info = vmalloc(tmp);
	if (!ring->rx_info)
		return -ENOMEM;
=======
	if (xdp_rxq_info_reg(&ring->xdp_rxq, priv->dev, queue_index, 0) < 0)
		goto err_ring;

	tmp = size * roundup_pow_of_two(MLX4_EN_MAX_RX_FRAGS *
					sizeof(struct mlx4_en_rx_alloc));
	ring->rx_info = kvzalloc_node(tmp, GFP_KERNEL, node);
	if (!ring->rx_info) {
		err = -ENOMEM;
		goto err_xdp_info;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	en_dbg(DRV, priv, "Allocated rx_info ring at addr:%p size:%d\n",
		 ring->rx_info, tmp);

<<<<<<< HEAD
	err = mlx4_alloc_hwq_res(mdev->dev, &ring->wqres,
				 ring->buf_size, 2 * PAGE_SIZE);
	if (err)
		goto err_ring;

	err = mlx4_en_map_buffer(&ring->wqres.buf);
	if (err) {
		en_err(priv, "Failed to map RX buffer\n");
		goto err_hwq;
	}
	ring->buf = ring->wqres.buf.direct.buf;

	return 0;

err_hwq:
	mlx4_free_hwq_res(mdev->dev, &ring->wqres, ring->buf_size);
err_ring:
	vfree(ring->rx_info);
	ring->rx_info = NULL;
=======
	/* Allocate HW buffers on provided NUMA node */
	set_dev_node(&mdev->dev->persist->pdev->dev, node);
	err = mlx4_alloc_hwq_res(mdev->dev, &ring->wqres, ring->buf_size);
	set_dev_node(&mdev->dev->persist->pdev->dev, mdev->dev->numa_node);
	if (err)
		goto err_info;

	ring->buf = ring->wqres.buf.direct.buf;

	ring->hwtstamp_rx_filter = priv->hwtstamp_config.rx_filter;

	*pring = ring;
	return 0;

err_info:
	kvfree(ring->rx_info);
	ring->rx_info = NULL;
err_xdp_info:
	xdp_rxq_info_unreg(&ring->xdp_rxq);
err_ring:
	kfree(ring);
	*pring = NULL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

int mlx4_en_activate_rx_rings(struct mlx4_en_priv *priv)
{
	struct mlx4_en_rx_ring *ring;
	int i;
	int ring_ind;
	int err;
	int stride = roundup_pow_of_two(sizeof(struct mlx4_en_rx_desc) +
					DS_SIZE * priv->num_frags);

	for (ring_ind = 0; ring_ind < priv->rx_ring_num; ring_ind++) {
<<<<<<< HEAD
		ring = &priv->rx_ring[ring_ind];
=======
		ring = priv->rx_ring[ring_ind];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ring->prod = 0;
		ring->cons = 0;
		ring->actual_size = 0;
<<<<<<< HEAD
		ring->cqn = priv->rx_cq[ring_ind].mcq.cqn;

		ring->stride = stride;
		if (ring->stride <= TXBB_SIZE)
			ring->buf += TXBB_SIZE;
=======
		ring->cqn = priv->rx_cq[ring_ind]->mcq.cqn;

		ring->stride = stride;
		if (ring->stride <= TXBB_SIZE) {
			/* Stamp first unused send wqe */
			__be32 *ptr = (__be32 *)ring->buf;
			__be32 stamp = cpu_to_be32(1 << STAMP_SHIFT);
			*ptr = stamp;
			/* Move pointer to start of rx section */
			ring->buf += TXBB_SIZE;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ring->log_stride = ffs(ring->stride) - 1;
		ring->buf_size = ring->size * ring->stride;

		memset(ring->buf, 0, ring->buf_size);
		mlx4_en_update_rx_prod_db(ring);

<<<<<<< HEAD
		/* Initailize all descriptors */
		for (i = 0; i < ring->size; i++)
			mlx4_en_init_rx_desc(priv, ring, i);

		/* Initialize page allocators */
		err = mlx4_en_init_allocator(priv, ring);
		if (err) {
			en_err(priv, "Failed initializing ring allocator\n");
			if (ring->stride <= TXBB_SIZE)
				ring->buf -= TXBB_SIZE;
			ring_ind--;
			goto err_allocator;
		}
=======
		/* Initialize all descriptors */
		for (i = 0; i < ring->size; i++)
			mlx4_en_init_rx_desc(priv, ring, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	err = mlx4_en_fill_rx_buffers(priv);
	if (err)
		goto err_buffers;

	for (ring_ind = 0; ring_ind < priv->rx_ring_num; ring_ind++) {
<<<<<<< HEAD
		ring = &priv->rx_ring[ring_ind];
=======
		ring = priv->rx_ring[ring_ind];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ring->size_mask = ring->actual_size - 1;
		mlx4_en_update_rx_prod_db(ring);
	}

	return 0;

err_buffers:
	for (ring_ind = 0; ring_ind < priv->rx_ring_num; ring_ind++)
<<<<<<< HEAD
		mlx4_en_free_rx_buf(priv, &priv->rx_ring[ring_ind]);

	ring_ind = priv->rx_ring_num - 1;
err_allocator:
	while (ring_ind >= 0) {
		if (priv->rx_ring[ring_ind].stride <= TXBB_SIZE)
			priv->rx_ring[ring_ind].buf -= TXBB_SIZE;
		mlx4_en_destroy_allocator(priv, &priv->rx_ring[ring_ind]);
=======
		mlx4_en_free_rx_buf(priv, priv->rx_ring[ring_ind]);

	ring_ind = priv->rx_ring_num - 1;
	while (ring_ind >= 0) {
		if (priv->rx_ring[ring_ind]->stride <= TXBB_SIZE)
			priv->rx_ring[ring_ind]->buf -= TXBB_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ring_ind--;
	}
	return err;
}

<<<<<<< HEAD
void mlx4_en_destroy_rx_ring(struct mlx4_en_priv *priv,
			     struct mlx4_en_rx_ring *ring, u32 size, u16 stride)
{
	struct mlx4_en_dev *mdev = priv->mdev;

	mlx4_en_unmap_buffer(&ring->wqres.buf);
	mlx4_free_hwq_res(mdev->dev, &ring->wqres, size * stride + TXBB_SIZE);
	vfree(ring->rx_info);
	ring->rx_info = NULL;
=======
/* We recover from out of memory by scheduling our napi poll
 * function (mlx4_en_process_cq), which tries to allocate
 * all missing RX buffers (call to mlx4_en_refill_rx_buffers).
 */
void mlx4_en_recover_from_oom(struct mlx4_en_priv *priv)
{
	int ring;

	if (!priv->port_up)
		return;

	for (ring = 0; ring < priv->rx_ring_num; ring++) {
		if (mlx4_en_is_ring_empty(priv->rx_ring[ring])) {
			local_bh_disable();
			napi_schedule(&priv->rx_cq[ring]->napi);
			local_bh_enable();
		}
	}
}

/* When the rx ring is running in page-per-packet mode, a released frame can go
 * directly into a small cache, to avoid unmapping or touching the page
 * allocator. In bpf prog performance scenarios, buffers are either forwarded
 * or dropped, never converted to skbs, so every page can come directly from
 * this cache when it is sized to be a multiple of the napi budget.
 */
bool mlx4_en_rx_recycle(struct mlx4_en_rx_ring *ring,
			struct mlx4_en_rx_alloc *frame)
{
	struct mlx4_en_page_cache *cache = &ring->page_cache;

	if (cache->index >= MLX4_EN_CACHE_SIZE)
		return false;

	cache->buf[cache->index].page = frame->page;
	cache->buf[cache->index].dma = frame->dma;
	cache->index++;
	return true;
}

void mlx4_en_destroy_rx_ring(struct mlx4_en_priv *priv,
			     struct mlx4_en_rx_ring **pring,
			     u32 size, u16 stride)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	struct mlx4_en_rx_ring *ring = *pring;
	struct bpf_prog *old_prog;

	old_prog = rcu_dereference_protected(
					ring->xdp_prog,
					lockdep_is_held(&mdev->state_lock));
	if (old_prog)
		bpf_prog_put(old_prog);
	xdp_rxq_info_unreg(&ring->xdp_rxq);
	mlx4_free_hwq_res(mdev->dev, &ring->wqres, size * stride + TXBB_SIZE);
	kvfree(ring->rx_info);
	ring->rx_info = NULL;
	kfree(ring);
	*pring = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void mlx4_en_deactivate_rx_ring(struct mlx4_en_priv *priv,
				struct mlx4_en_rx_ring *ring)
{
<<<<<<< HEAD
	mlx4_en_free_rx_buf(priv, ring);
	if (ring->stride <= TXBB_SIZE)
		ring->buf -= TXBB_SIZE;
	mlx4_en_destroy_allocator(priv, ring);
}


/* Unmap a completed descriptor and free unused pages */
static int mlx4_en_complete_rx_desc(struct mlx4_en_priv *priv,
				    struct mlx4_en_rx_desc *rx_desc,
				    struct page_frag *skb_frags,
				    struct sk_buff *skb,
				    struct mlx4_en_rx_alloc *page_alloc,
				    int length)
{
	struct skb_frag_struct *skb_frags_rx = skb_shinfo(skb)->frags;
	struct mlx4_en_frag_info *frag_info;
	int nr;
	dma_addr_t dma;

	/* Collect used fragments while replacing them in the HW descirptors */
	for (nr = 0; nr < priv->num_frags; nr++) {
		frag_info = &priv->frag_info[nr];
		if (length <= frag_info->frag_prefix_size)
			break;

		/* Save page reference in skb */
		__skb_frag_set_page(&skb_frags_rx[nr], skb_frags[nr].page);
		skb_frag_size_set(&skb_frags_rx[nr], skb_frags[nr].size);
		skb_frags_rx[nr].page_offset = skb_frags[nr].offset;
		skb->truesize += frag_info->frag_stride;
		dma = be64_to_cpu(rx_desc->data[nr].addr);

		/* Allocate a replacement page */
		if (mlx4_en_alloc_frag(priv, rx_desc, skb_frags, page_alloc, nr))
			goto fail;

		/* Unmap buffer */
		dma_unmap_single(priv->ddev, dma, skb_frag_size(&skb_frags_rx[nr]),
				 PCI_DMA_FROMDEVICE);
	}
	/* Adjust size of last fragment to match actual length */
	if (nr > 0)
		skb_frag_size_set(&skb_frags_rx[nr - 1],
			length - priv->frag_info[nr - 1].frag_prefix_size);
	return nr;

fail:
	/* Drop all accumulated fragments (which have already been replaced in
	 * the descriptor) of this packet; remaining fragments are reused... */
	while (nr > 0) {
		nr--;
		__skb_frag_unref(&skb_frags_rx[nr]);
=======
	int i;

	for (i = 0; i < ring->page_cache.index; i++) {
		dma_unmap_page(priv->ddev, ring->page_cache.buf[i].dma,
			       PAGE_SIZE, priv->dma_dir);
		put_page(ring->page_cache.buf[i].page);
	}
	ring->page_cache.index = 0;
	mlx4_en_free_rx_buf(priv, ring);
	if (ring->stride <= TXBB_SIZE)
		ring->buf -= TXBB_SIZE;
}


static int mlx4_en_complete_rx_desc(struct mlx4_en_priv *priv,
				    struct mlx4_en_rx_alloc *frags,
				    struct sk_buff *skb,
				    int length)
{
	const struct mlx4_en_frag_info *frag_info = priv->frag_info;
	unsigned int truesize = 0;
	bool release = true;
	int nr, frag_size;
	struct page *page;
	dma_addr_t dma;

	/* Collect used fragments while replacing them in the HW descriptors */
	for (nr = 0;; frags++) {
		frag_size = min_t(int, length, frag_info->frag_size);

		page = frags->page;
		if (unlikely(!page))
			goto fail;

		dma = frags->dma;
		dma_sync_single_range_for_cpu(priv->ddev, dma, frags->page_offset,
					      frag_size, priv->dma_dir);

		__skb_fill_page_desc(skb, nr, page, frags->page_offset,
				     frag_size);

		truesize += frag_info->frag_stride;
		if (frag_info->frag_stride == PAGE_SIZE / 2) {
			frags->page_offset ^= PAGE_SIZE / 2;
			release = page_count(page) != 1 ||
				  page_is_pfmemalloc(page) ||
				  page_to_nid(page) != numa_mem_id();
		} else if (!priv->rx_headroom) {
			/* rx_headroom for non XDP setup is always 0.
			 * When XDP is set, the above condition will
			 * guarantee page is always released.
			 */
			u32 sz_align = ALIGN(frag_size, SMP_CACHE_BYTES);

			frags->page_offset += sz_align;
			release = frags->page_offset + frag_info->frag_size > PAGE_SIZE;
		}
		if (release) {
			dma_unmap_page(priv->ddev, dma, PAGE_SIZE, priv->dma_dir);
			frags->page = NULL;
		} else {
			page_ref_inc(page);
		}

		nr++;
		length -= frag_size;
		if (!length)
			break;
		frag_info++;
	}
	skb->truesize += truesize;
	return nr;

fail:
	while (nr > 0) {
		nr--;
		__skb_frag_unref(skb_shinfo(skb)->frags + nr, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD

static struct sk_buff *mlx4_en_rx_skb(struct mlx4_en_priv *priv,
				      struct mlx4_en_rx_desc *rx_desc,
				      struct page_frag *skb_frags,
				      struct mlx4_en_rx_alloc *page_alloc,
				      unsigned int length)
{
	struct sk_buff *skb;
	void *va;
	int used_frags;
	dma_addr_t dma;

	skb = netdev_alloc_skb(priv->dev, SMALL_PACKET_SIZE + NET_IP_ALIGN);
	if (!skb) {
		en_dbg(RX_ERR, priv, "Failed allocating skb\n");
		return NULL;
	}
	skb_reserve(skb, NET_IP_ALIGN);
	skb->len = length;

	/* Get pointer to first fragment so we could copy the headers into the
	 * (linear part of the) skb */
	va = page_address(skb_frags[0].page) + skb_frags[0].offset;

	if (length <= SMALL_PACKET_SIZE) {
		/* We are copying all relevant data to the skb - temporarily
		 * synch buffers for the copy */
		dma = be64_to_cpu(rx_desc->data[0].addr);
		dma_sync_single_for_cpu(priv->ddev, dma, length,
					DMA_FROM_DEVICE);
		skb_copy_to_linear_data(skb, va, length);
		dma_sync_single_for_device(priv->ddev, dma, length,
					   DMA_FROM_DEVICE);
		skb->tail += length;
	} else {

		/* Move relevant fragments to skb */
		used_frags = mlx4_en_complete_rx_desc(priv, rx_desc, skb_frags,
						      skb, page_alloc, length);
		if (unlikely(!used_frags)) {
			kfree_skb(skb);
			return NULL;
		}
		skb_shinfo(skb)->nr_frags = used_frags;

		/* Copy headers into the skb linear buffer */
		memcpy(skb->data, va, HEADER_COPY_SIZE);
		skb->tail += HEADER_COPY_SIZE;

		/* Skip headers in first fragment */
		skb_shinfo(skb)->frags[0].page_offset += HEADER_COPY_SIZE;

		/* Adjust size of first fragment */
		skb_frag_size_sub(&skb_shinfo(skb)->frags[0], HEADER_COPY_SIZE);
		skb->data_len = length - HEADER_COPY_SIZE;
	}
	return skb;
}

static void validate_loopback(struct mlx4_en_priv *priv, struct sk_buff *skb)
{
	int i;
	int offset = ETH_HLEN;

	for (i = 0; i < MLX4_LOOPBACK_TEST_PAYLOAD; i++, offset++) {
		if (*(skb->data + offset) != (unsigned char) (i & 0xff))
			goto out_loopback;
	}
	/* Loopback found */
	priv->loopback_ok = 1;

out_loopback:
	dev_kfree_skb_any(skb);
=======
static void validate_loopback(struct mlx4_en_priv *priv, void *va)
{
	const unsigned char *data = va + ETH_HLEN;
	int i;

	for (i = 0; i < MLX4_LOOPBACK_TEST_PAYLOAD; i++) {
		if (data[i] != (unsigned char)i)
			return;
	}
	/* Loopback found */
	priv->loopback_ok = 1;
}

static void mlx4_en_refill_rx_buffers(struct mlx4_en_priv *priv,
				      struct mlx4_en_rx_ring *ring)
{
	u32 missing = ring->actual_size - (ring->prod - ring->cons);

	/* Try to batch allocations, but not too much. */
	if (missing < 8)
		return;
	do {
		if (mlx4_en_prepare_rx_desc(priv, ring,
					    ring->prod & ring->size_mask,
					    GFP_ATOMIC | __GFP_MEMALLOC))
			break;
		ring->prod++;
	} while (likely(--missing));

	mlx4_en_update_rx_prod_db(ring);
}

/* When hardware doesn't strip the vlan, we need to calculate the checksum
 * over it and add it to the hardware's checksum calculation
 */
static inline __wsum get_fixed_vlan_csum(__wsum hw_checksum,
					 struct vlan_hdr *vlanh)
{
	return csum_add(hw_checksum, *(__wsum *)vlanh);
}

/* Although the stack expects checksum which doesn't include the pseudo
 * header, the HW adds it. To address that, we are subtracting the pseudo
 * header checksum from the checksum value provided by the HW.
 */
static int get_fixed_ipv4_csum(__wsum hw_checksum, struct sk_buff *skb,
			       struct iphdr *iph)
{
	__u16 length_for_csum = 0;
	__wsum csum_pseudo_header = 0;
	__u8 ipproto = iph->protocol;

	if (unlikely(ipproto == IPPROTO_SCTP))
		return -1;

	length_for_csum = (be16_to_cpu(iph->tot_len) - (iph->ihl << 2));
	csum_pseudo_header = csum_tcpudp_nofold(iph->saddr, iph->daddr,
						length_for_csum, ipproto, 0);
	skb->csum = csum_sub(hw_checksum, csum_pseudo_header);
	return 0;
}

#if IS_ENABLED(CONFIG_IPV6)
/* In IPv6 packets, hw_checksum lacks 6 bytes from IPv6 header:
 * 4 first bytes : priority, version, flow_lbl
 * and 2 additional bytes : nexthdr, hop_limit.
 */
static int get_fixed_ipv6_csum(__wsum hw_checksum, struct sk_buff *skb,
			       struct ipv6hdr *ipv6h)
{
	__u8 nexthdr = ipv6h->nexthdr;
	__wsum temp;

	if (unlikely(nexthdr == IPPROTO_FRAGMENT ||
		     nexthdr == IPPROTO_HOPOPTS ||
		     nexthdr == IPPROTO_SCTP))
		return -1;

	/* priority, version, flow_lbl */
	temp = csum_add(hw_checksum, *(__wsum *)ipv6h);
	/* nexthdr and hop_limit */
	skb->csum = csum_add(temp, (__force __wsum)*(__be16 *)&ipv6h->nexthdr);
	return 0;
}
#endif

#define short_frame(size) ((size) <= ETH_ZLEN + ETH_FCS_LEN)

/* We reach this function only after checking that any of
 * the (IPv4 | IPv6) bits are set in cqe->status.
 */
static int check_csum(struct mlx4_cqe *cqe, struct sk_buff *skb, void *va,
		      netdev_features_t dev_features)
{
	__wsum hw_checksum = 0;
	void *hdr;

	/* CQE csum doesn't cover padding octets in short ethernet
	 * frames. And the pad field is appended prior to calculating
	 * and appending the FCS field.
	 *
	 * Detecting these padded frames requires to verify and parse
	 * IP headers, so we simply force all those small frames to skip
	 * checksum complete.
	 */
	if (short_frame(skb->len))
		return -EINVAL;

	hdr = (u8 *)va + sizeof(struct ethhdr);
	hw_checksum = csum_unfold((__force __sum16)cqe->checksum);

	if (cqe->vlan_my_qpn & cpu_to_be32(MLX4_CQE_CVLAN_PRESENT_MASK) &&
	    !(dev_features & NETIF_F_HW_VLAN_CTAG_RX)) {
		hw_checksum = get_fixed_vlan_csum(hw_checksum, hdr);
		hdr += sizeof(struct vlan_hdr);
	}

#if IS_ENABLED(CONFIG_IPV6)
	if (cqe->status & cpu_to_be16(MLX4_CQE_STATUS_IPV6))
		return get_fixed_ipv6_csum(hw_checksum, skb, hdr);
#endif
	return get_fixed_ipv4_csum(hw_checksum, skb, hdr);
}

#if IS_ENABLED(CONFIG_IPV6)
#define MLX4_CQE_STATUS_IP_ANY (MLX4_CQE_STATUS_IPV4 | MLX4_CQE_STATUS_IPV6)
#else
#define MLX4_CQE_STATUS_IP_ANY (MLX4_CQE_STATUS_IPV4)
#endif

struct mlx4_en_xdp_buff {
	struct xdp_buff xdp;
	struct mlx4_cqe *cqe;
	struct mlx4_en_dev *mdev;
	struct mlx4_en_rx_ring *ring;
	struct net_device *dev;
};

int mlx4_en_xdp_rx_timestamp(const struct xdp_md *ctx, u64 *timestamp)
{
	struct mlx4_en_xdp_buff *_ctx = (void *)ctx;

	if (unlikely(_ctx->ring->hwtstamp_rx_filter != HWTSTAMP_FILTER_ALL))
		return -ENODATA;

	*timestamp = mlx4_en_get_hwtstamp(_ctx->mdev,
					  mlx4_en_get_cqe_ts(_ctx->cqe));
	return 0;
}

int mlx4_en_xdp_rx_hash(const struct xdp_md *ctx, u32 *hash,
			enum xdp_rss_hash_type *rss_type)
{
	struct mlx4_en_xdp_buff *_ctx = (void *)ctx;
	struct mlx4_cqe *cqe = _ctx->cqe;
	enum xdp_rss_hash_type xht = 0;
	__be16 status;

	if (unlikely(!(_ctx->dev->features & NETIF_F_RXHASH)))
		return -ENODATA;

	*hash = be32_to_cpu(cqe->immed_rss_invalid);
	status = cqe->status;
	if (status & cpu_to_be16(MLX4_CQE_STATUS_TCP))
		xht = XDP_RSS_L4_TCP;
	if (status & cpu_to_be16(MLX4_CQE_STATUS_UDP))
		xht = XDP_RSS_L4_UDP;
	if (status & cpu_to_be16(MLX4_CQE_STATUS_IPV4 | MLX4_CQE_STATUS_IPV4F))
		xht |= XDP_RSS_L3_IPV4;
	if (status & cpu_to_be16(MLX4_CQE_STATUS_IPV6)) {
		xht |= XDP_RSS_L3_IPV6;
		if (cqe->ipv6_ext_mask)
			xht |= XDP_RSS_L3_DYNHDR;
	}
	*rss_type = xht;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int mlx4_en_process_rx_cq(struct net_device *dev, struct mlx4_en_cq *cq, int budget)
{
	struct mlx4_en_priv *priv = netdev_priv(dev);
<<<<<<< HEAD
	struct mlx4_cqe *cqe;
	struct mlx4_en_rx_ring *ring = &priv->rx_ring[cq->ring];
	struct page_frag *skb_frags;
	struct mlx4_en_rx_desc *rx_desc;
	struct sk_buff *skb;
	int index;
	int nr;
	unsigned int length;
	int polled = 0;
	int ip_summed;
	struct ethhdr *ethh;
	u64 s_mac;

	if (!priv->port_up)
		return 0;

=======
	struct mlx4_en_xdp_buff mxbuf = {};
	int factor = priv->cqe_factor;
	struct mlx4_en_rx_ring *ring;
	struct bpf_prog *xdp_prog;
	int cq_ring = cq->ring;
	bool doorbell_pending;
	bool xdp_redir_flush;
	struct mlx4_cqe *cqe;
	int polled = 0;
	int index;

	if (unlikely(!priv->port_up || budget <= 0))
		return 0;

	ring = priv->rx_ring[cq_ring];

	xdp_prog = rcu_dereference_bh(ring->xdp_prog);
	xdp_init_buff(&mxbuf.xdp, priv->frag_info[0].frag_stride, &ring->xdp_rxq);
	doorbell_pending = false;
	xdp_redir_flush = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* We assume a 1:1 mapping between CQEs and Rx descriptors, so Rx
	 * descriptor offset can be deduced from the CQE index instead of
	 * reading 'cqe->index' */
	index = cq->mcq.cons_index & ring->size_mask;
<<<<<<< HEAD
	cqe = &cq->buf[index];
=======
	cqe = mlx4_en_get_cqe(cq->buf, index, priv->cqe_size) + factor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Process all completed CQEs */
	while (XNOR(cqe->owner_sr_opcode & MLX4_CQE_OWNER_MASK,
		    cq->mcq.cons_index & cq->size)) {
<<<<<<< HEAD

		skb_frags = ring->rx_info + (index << priv->log_rx_info);
		rx_desc = ring->buf + (index << ring->log_stride);

		/*
		 * make sure we read the CQE after we read the ownership bit
		 */
		rmb();
=======
		struct mlx4_en_rx_alloc *frags;
		enum pkt_hash_types hash_type;
		struct sk_buff *skb;
		unsigned int length;
		int ip_summed;
		void *va;
		int nr;

		frags = ring->rx_info + (index << priv->log_rx_info);
		va = page_address(frags[0].page) + frags[0].page_offset;
		net_prefetchw(va);
		/*
		 * make sure we read the CQE after we read the ownership bit
		 */
		dma_rmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* Drop packet on bad receive or bad checksum */
		if (unlikely((cqe->owner_sr_opcode & MLX4_CQE_OPCODE_MASK) ==
						MLX4_CQE_OPCODE_ERROR)) {
<<<<<<< HEAD
			en_err(priv, "CQE completed in error - vendor "
				  "syndrom:%d syndrom:%d\n",
				  ((struct mlx4_err_cqe *) cqe)->vendor_err_syndrome,
				  ((struct mlx4_err_cqe *) cqe)->syndrome);
=======
			en_err(priv, "CQE completed in error - vendor syndrome:%d syndrome:%d\n",
			       ((struct mlx4_err_cqe *)cqe)->vendor_err_syndrome,
			       ((struct mlx4_err_cqe *)cqe)->syndrome);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto next;
		}
		if (unlikely(cqe->badfcs_enc & MLX4_CQE_BAD_FCS)) {
			en_dbg(RX_ERR, priv, "Accepted frame with bad FCS\n");
			goto next;
		}

<<<<<<< HEAD
		/* Get pointer to first fragment since we haven't skb yet and
		 * cast it to ethhdr struct */
		ethh = (struct ethhdr *)(page_address(skb_frags[0].page) +
					 skb_frags[0].offset);
		s_mac = mlx4_en_mac_to_u64(ethh->h_source);

		/* If source MAC is equal to our own MAC and not performing
		 * the selftest or flb disabled - drop the packet */
		if (s_mac == priv->mac &&
			(!(dev->features & NETIF_F_LOOPBACK) ||
			 !priv->validate_loopback))
			goto next;
=======
		/* Check if we need to drop the packet if SRIOV is not enabled
		 * and not performing the selftest or flb disabled
		 */
		if (priv->flags & MLX4_EN_FLAG_RX_FILTER_NEEDED) {
			const struct ethhdr *ethh = va;
			dma_addr_t dma;
			/* Get pointer to first fragment since we haven't
			 * skb yet and cast it to ethhdr struct
			 */
			dma = frags[0].dma + frags[0].page_offset;
			dma_sync_single_for_cpu(priv->ddev, dma, sizeof(*ethh),
						DMA_FROM_DEVICE);

			if (is_multicast_ether_addr(ethh->h_dest)) {
				struct mlx4_mac_entry *entry;
				struct hlist_head *bucket;
				unsigned int mac_hash;

				/* Drop the packet, since HW loopback-ed it */
				mac_hash = ethh->h_source[MLX4_EN_MAC_HASH_IDX];
				bucket = &priv->mac_hash[mac_hash];
				hlist_for_each_entry_rcu_bh(entry, bucket, hlist) {
					if (ether_addr_equal_64bits(entry->mac,
								    ethh->h_source))
						goto next;
				}
			}
		}

		if (unlikely(priv->validate_loopback)) {
			validate_loopback(priv, va);
			goto next;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Packet is OK - process it.
		 */
		length = be32_to_cpu(cqe->byte_cnt);
		length -= ring->fcs_del;
<<<<<<< HEAD
		ring->bytes += length;
		ring->packets++;

		if (likely(dev->features & NETIF_F_RXCSUM)) {
			if ((cqe->status & cpu_to_be16(MLX4_CQE_STATUS_IPOK)) &&
			    (cqe->checksum == cpu_to_be16(0xffff))) {
				ring->csum_ok++;
				/* This packet is eligible for LRO if it is:
				 * - DIX Ethernet (type interpretation)
				 * - TCP/IP (v4)
				 * - without IP options
				 * - not an IP fragment */
				if (dev->features & NETIF_F_GRO) {
					struct sk_buff *gro_skb = napi_get_frags(&cq->napi);
					if (!gro_skb)
						goto next;

					nr = mlx4_en_complete_rx_desc(
						priv, rx_desc,
						skb_frags, gro_skb,
						ring->page_alloc, length);
					if (!nr)
						goto next;

					skb_shinfo(gro_skb)->nr_frags = nr;
					gro_skb->len = length;
					gro_skb->data_len = length;
					gro_skb->ip_summed = CHECKSUM_UNNECESSARY;

					if (cqe->vlan_my_qpn &
					    cpu_to_be32(MLX4_CQE_VLAN_PRESENT_MASK)) {
						u16 vid = be16_to_cpu(cqe->sl_vid);

						__vlan_hwaccel_put_tag(gro_skb, vid);
					}

					if (dev->features & NETIF_F_RXHASH)
						gro_skb->rxhash = be32_to_cpu(cqe->immed_rss_invalid);

					skb_record_rx_queue(gro_skb, cq->ring);
					napi_gro_frags(&cq->napi);

					goto next;
				}

				/* LRO not possible, complete processing here */
				ip_summed = CHECKSUM_UNNECESSARY;
			} else {
				ip_summed = CHECKSUM_NONE;
				ring->csum_none++;
			}
		} else {
			ip_summed = CHECKSUM_NONE;
			ring->csum_none++;
		}

		skb = mlx4_en_rx_skb(priv, rx_desc, skb_frags,
				     ring->page_alloc, length);
		if (!skb) {
			priv->stats.rx_dropped++;
			goto next;
		}

                if (unlikely(priv->validate_loopback)) {
			validate_loopback(priv, skb);
			goto next;
		}

		skb->ip_summed = ip_summed;
		skb->protocol = eth_type_trans(skb, dev);
		skb_record_rx_queue(skb, cq->ring);

		if (dev->features & NETIF_F_RXHASH)
			skb->rxhash = be32_to_cpu(cqe->immed_rss_invalid);

		if (be32_to_cpu(cqe->vlan_my_qpn) &
		    MLX4_CQE_VLAN_PRESENT_MASK)
			__vlan_hwaccel_put_tag(skb, be16_to_cpu(cqe->sl_vid));

		/* Push it up the stack */
		netif_receive_skb(skb);

next:
		++cq->mcq.cons_index;
		index = (cq->mcq.cons_index) & ring->size_mask;
		cqe = &cq->buf[index];
		if (++polled == budget) {
			/* We are here because we reached the NAPI budget -
			 * flush only pending LRO sessions */
			goto out;
		}
	}

out:
	AVG_PERF_COUNTER(priv->pstats.rx_coal_avg, polled);
	mlx4_cq_set_ci(&cq->mcq);
	wmb(); /* ensure HW sees CQ consumer before we post new buffers */
	ring->cons = cq->mcq.cons_index;
	ring->prod += polled; /* Polled descriptors were realocated in place */
	mlx4_en_update_rx_prod_db(ring);
=======

		/* A bpf program gets first chance to drop the packet. It may
		 * read bytes but not past the end of the frag.
		 */
		if (xdp_prog) {
			dma_addr_t dma;
			void *orig_data;
			u32 act;

			dma = frags[0].dma + frags[0].page_offset;
			dma_sync_single_for_cpu(priv->ddev, dma,
						priv->frag_info[0].frag_size,
						DMA_FROM_DEVICE);

			xdp_prepare_buff(&mxbuf.xdp, va - frags[0].page_offset,
					 frags[0].page_offset, length, true);
			orig_data = mxbuf.xdp.data;
			mxbuf.cqe = cqe;
			mxbuf.mdev = priv->mdev;
			mxbuf.ring = ring;
			mxbuf.dev = dev;

			act = bpf_prog_run_xdp(xdp_prog, &mxbuf.xdp);

			length = mxbuf.xdp.data_end - mxbuf.xdp.data;
			if (mxbuf.xdp.data != orig_data) {
				frags[0].page_offset = mxbuf.xdp.data -
					mxbuf.xdp.data_hard_start;
				va = mxbuf.xdp.data;
			}

			switch (act) {
			case XDP_PASS:
				break;
			case XDP_REDIRECT:
				if (likely(!xdp_do_redirect(dev, &mxbuf.xdp, xdp_prog))) {
					ring->xdp_redirect++;
					xdp_redir_flush = true;
					frags[0].page = NULL;
					goto next;
				}
				ring->xdp_redirect_fail++;
				trace_xdp_exception(dev, xdp_prog, act);
				goto xdp_drop_no_cnt;
			case XDP_TX:
				if (likely(!mlx4_en_xmit_frame(ring, frags, priv,
							length, cq_ring,
							&doorbell_pending))) {
					frags[0].page = NULL;
					goto next;
				}
				trace_xdp_exception(dev, xdp_prog, act);
				goto xdp_drop_no_cnt; /* Drop on xmit failure */
			default:
				bpf_warn_invalid_xdp_action(dev, xdp_prog, act);
				fallthrough;
			case XDP_ABORTED:
				trace_xdp_exception(dev, xdp_prog, act);
				fallthrough;
			case XDP_DROP:
				ring->xdp_drop++;
xdp_drop_no_cnt:
				goto next;
			}
		}

		ring->bytes += length;
		ring->packets++;

		skb = napi_get_frags(&cq->napi);
		if (unlikely(!skb))
			goto next;

		if (unlikely(ring->hwtstamp_rx_filter == HWTSTAMP_FILTER_ALL)) {
			u64 timestamp = mlx4_en_get_cqe_ts(cqe);

			mlx4_en_fill_hwtstamps(priv->mdev, skb_hwtstamps(skb),
					       timestamp);
		}
		skb_record_rx_queue(skb, cq_ring);

		if (likely(dev->features & NETIF_F_RXCSUM)) {
			/* TODO: For IP non TCP/UDP packets when csum complete is
			 * not an option (not supported or any other reason) we can
			 * actually check cqe IPOK status bit and report
			 * CHECKSUM_UNNECESSARY rather than CHECKSUM_NONE
			 */
			if ((cqe->status & cpu_to_be16(MLX4_CQE_STATUS_TCP |
						       MLX4_CQE_STATUS_UDP)) &&
			    (cqe->status & cpu_to_be16(MLX4_CQE_STATUS_IPOK)) &&
			    cqe->checksum == cpu_to_be16(0xffff)) {
				bool l2_tunnel;

				l2_tunnel = (dev->hw_enc_features & NETIF_F_RXCSUM) &&
					(cqe->vlan_my_qpn & cpu_to_be32(MLX4_CQE_L2_TUNNEL));
				ip_summed = CHECKSUM_UNNECESSARY;
				hash_type = PKT_HASH_TYPE_L4;
				if (l2_tunnel)
					skb->csum_level = 1;
				ring->csum_ok++;
			} else {
				if (!(priv->flags & MLX4_EN_FLAG_RX_CSUM_NON_TCP_UDP &&
				      (cqe->status & cpu_to_be16(MLX4_CQE_STATUS_IP_ANY))))
					goto csum_none;
				if (check_csum(cqe, skb, va, dev->features))
					goto csum_none;
				ip_summed = CHECKSUM_COMPLETE;
				hash_type = PKT_HASH_TYPE_L3;
				ring->csum_complete++;
			}
		} else {
csum_none:
			ip_summed = CHECKSUM_NONE;
			hash_type = PKT_HASH_TYPE_L3;
			ring->csum_none++;
		}
		skb->ip_summed = ip_summed;
		if (dev->features & NETIF_F_RXHASH)
			skb_set_hash(skb,
				     be32_to_cpu(cqe->immed_rss_invalid),
				     hash_type);

		if ((cqe->vlan_my_qpn &
		     cpu_to_be32(MLX4_CQE_CVLAN_PRESENT_MASK)) &&
		    (dev->features & NETIF_F_HW_VLAN_CTAG_RX))
			__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q),
					       be16_to_cpu(cqe->sl_vid));
		else if ((cqe->vlan_my_qpn &
			  cpu_to_be32(MLX4_CQE_SVLAN_PRESENT_MASK)) &&
			 (dev->features & NETIF_F_HW_VLAN_STAG_RX))
			__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021AD),
					       be16_to_cpu(cqe->sl_vid));

		nr = mlx4_en_complete_rx_desc(priv, frags, skb, length);
		if (likely(nr)) {
			skb_shinfo(skb)->nr_frags = nr;
			skb->len = length;
			skb->data_len = length;
			napi_gro_frags(&cq->napi);
		} else {
			__vlan_hwaccel_clear_tag(skb);
			skb_clear_hash(skb);
		}
next:
		++cq->mcq.cons_index;
		index = (cq->mcq.cons_index) & ring->size_mask;
		cqe = mlx4_en_get_cqe(cq->buf, index, priv->cqe_size) + factor;
		if (unlikely(++polled == budget))
			break;
	}

	if (xdp_redir_flush)
		xdp_do_flush();

	if (likely(polled)) {
		if (doorbell_pending) {
			priv->tx_cq[TX_XDP][cq_ring]->xdp_busy = true;
			mlx4_en_xmit_doorbell(priv->tx_ring[TX_XDP][cq_ring]);
		}

		mlx4_cq_set_ci(&cq->mcq);
		wmb(); /* ensure HW sees CQ consumer before we post new buffers */
		ring->cons = cq->mcq.cons_index;
	}

	mlx4_en_refill_rx_buffers(priv, ring);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return polled;
}


void mlx4_en_rx_irq(struct mlx4_cq *mcq)
{
	struct mlx4_en_cq *cq = container_of(mcq, struct mlx4_en_cq, mcq);
	struct mlx4_en_priv *priv = netdev_priv(cq->dev);

<<<<<<< HEAD
	if (priv->port_up)
		napi_schedule(&cq->napi);
=======
	if (likely(priv->port_up))
		napi_schedule_irqoff(&cq->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		mlx4_en_arm_cq(priv, cq);
}

/* Rx CQ polling - called by NAPI */
int mlx4_en_poll_rx_cq(struct napi_struct *napi, int budget)
{
	struct mlx4_en_cq *cq = container_of(napi, struct mlx4_en_cq, napi);
	struct net_device *dev = cq->dev;
	struct mlx4_en_priv *priv = netdev_priv(dev);
<<<<<<< HEAD
	int done;

	done = mlx4_en_process_rx_cq(dev, cq, budget);

	/* If we used up all the quota - we're probably not done yet... */
	if (done == budget)
		INC_PERF_COUNTER(priv->pstats.napi_quota);
	else {
		/* Done for now */
		napi_complete(napi);
		mlx4_en_arm_cq(priv, cq);
	}
	return done;
}


/* Calculate the last offset position that accommodates a full fragment
 * (assuming fagment size = stride-align) */
static int mlx4_en_last_alloc_offset(struct mlx4_en_priv *priv, u16 stride, u16 align)
{
	u16 res = MLX4_EN_ALLOC_SIZE % stride;
	u16 offset = MLX4_EN_ALLOC_SIZE - stride - res + align;

	en_dbg(DRV, priv, "Calculated last offset for stride:%d align:%d "
			    "res:%d offset:%d\n", stride, align, res, offset);
	return offset;
}


static int frag_sizes[] = {
	FRAG_SZ0,
	FRAG_SZ1,
	FRAG_SZ2,
	FRAG_SZ3
};

void mlx4_en_calc_rx_buf(struct net_device *dev)
{
	struct mlx4_en_priv *priv = netdev_priv(dev);
	int eff_mtu = dev->mtu + ETH_HLEN + VLAN_HLEN + ETH_LLC_SNAP_SIZE;
	int buf_size = 0;
	int i = 0;

	while (buf_size < eff_mtu) {
		priv->frag_info[i].frag_size =
			(eff_mtu > buf_size + frag_sizes[i]) ?
				frag_sizes[i] : eff_mtu - buf_size;
		priv->frag_info[i].frag_prefix_size = buf_size;
		if (!i)	{
			priv->frag_info[i].frag_align = NET_IP_ALIGN;
			priv->frag_info[i].frag_stride =
				ALIGN(frag_sizes[i] + NET_IP_ALIGN, SMP_CACHE_BYTES);
		} else {
			priv->frag_info[i].frag_align = 0;
			priv->frag_info[i].frag_stride =
				ALIGN(frag_sizes[i], SMP_CACHE_BYTES);
		}
		priv->frag_info[i].last_offset = mlx4_en_last_alloc_offset(
						priv, priv->frag_info[i].frag_stride,
						priv->frag_info[i].frag_align);
		buf_size += priv->frag_info[i].frag_size;
		i++;
=======
	struct mlx4_en_cq *xdp_tx_cq = NULL;
	bool clean_complete = true;
	int done;

	if (!budget)
		return 0;

	if (priv->tx_ring_num[TX_XDP]) {
		xdp_tx_cq = priv->tx_cq[TX_XDP][cq->ring];
		if (xdp_tx_cq->xdp_busy) {
			clean_complete = mlx4_en_process_tx_cq(dev, xdp_tx_cq,
							       budget) < budget;
			xdp_tx_cq->xdp_busy = !clean_complete;
		}
	}

	done = mlx4_en_process_rx_cq(dev, cq, budget);

	/* If we used up all the quota - we're probably not done yet... */
	if (done == budget || !clean_complete) {
		int cpu_curr;

		/* in case we got here because of !clean_complete */
		done = budget;

		cpu_curr = smp_processor_id();

		if (likely(cpumask_test_cpu(cpu_curr, cq->aff_mask)))
			return budget;

		/* Current cpu is not according to smp_irq_affinity -
		 * probably affinity changed. Need to stop this NAPI
		 * poll, and restart it on the right CPU.
		 * Try to avoid returning a too small value (like 0),
		 * to not fool net_rx_action() and its netdev_budget
		 */
		if (done)
			done--;
	}
	/* Done for now */
	if (likely(napi_complete_done(napi, done)))
		mlx4_en_arm_cq(priv, cq);
	return done;
}

void mlx4_en_calc_rx_buf(struct net_device *dev)
{
	struct mlx4_en_priv *priv = netdev_priv(dev);
	int eff_mtu = MLX4_EN_EFF_MTU(dev->mtu);
	int i = 0;

	/* bpf requires buffers to be set up as 1 packet per page.
	 * This only works when num_frags == 1.
	 */
	if (priv->tx_ring_num[TX_XDP]) {
		priv->frag_info[0].frag_size = eff_mtu;
		/* This will gain efficient xdp frame recycling at the
		 * expense of more costly truesize accounting
		 */
		priv->frag_info[0].frag_stride = PAGE_SIZE;
		priv->dma_dir = DMA_BIDIRECTIONAL;
		priv->rx_headroom = XDP_PACKET_HEADROOM;
		i = 1;
	} else {
		int frag_size_max = 2048, buf_size = 0;

		/* should not happen, right ? */
		if (eff_mtu > PAGE_SIZE + (MLX4_EN_MAX_RX_FRAGS - 1) * 2048)
			frag_size_max = PAGE_SIZE;

		while (buf_size < eff_mtu) {
			int frag_stride, frag_size = eff_mtu - buf_size;
			int pad, nb;

			if (i < MLX4_EN_MAX_RX_FRAGS - 1)
				frag_size = min(frag_size, frag_size_max);

			priv->frag_info[i].frag_size = frag_size;
			frag_stride = ALIGN(frag_size, SMP_CACHE_BYTES);
			/* We can only pack 2 1536-bytes frames in on 4K page
			 * Therefore, each frame would consume more bytes (truesize)
			 */
			nb = PAGE_SIZE / frag_stride;
			pad = (PAGE_SIZE - nb * frag_stride) / nb;
			pad &= ~(SMP_CACHE_BYTES - 1);
			priv->frag_info[i].frag_stride = frag_stride + pad;

			buf_size += frag_size;
			i++;
		}
		priv->dma_dir = DMA_FROM_DEVICE;
		priv->rx_headroom = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	priv->num_frags = i;
	priv->rx_skb_size = eff_mtu;
<<<<<<< HEAD
	priv->log_rx_info = ROUNDUP_LOG2(i * sizeof(struct skb_frag_struct));

	en_dbg(DRV, priv, "Rx buffer scatter-list (effective-mtu:%d "
		  "num_frags:%d):\n", eff_mtu, priv->num_frags);
	for (i = 0; i < priv->num_frags; i++) {
		en_dbg(DRV, priv, "  frag:%d - size:%d prefix:%d align:%d "
				"stride:%d last_offset:%d\n", i,
				priv->frag_info[i].frag_size,
				priv->frag_info[i].frag_prefix_size,
				priv->frag_info[i].frag_align,
				priv->frag_info[i].frag_stride,
				priv->frag_info[i].last_offset);
=======
	priv->log_rx_info = ROUNDUP_LOG2(i * sizeof(struct mlx4_en_rx_alloc));

	en_dbg(DRV, priv, "Rx buffer scatter-list (effective-mtu:%d num_frags:%d):\n",
	       eff_mtu, priv->num_frags);
	for (i = 0; i < priv->num_frags; i++) {
		en_dbg(DRV,
		       priv,
		       "  frag:%d - size:%d stride:%d\n",
		       i,
		       priv->frag_info[i].frag_size,
		       priv->frag_info[i].frag_stride);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* RSS related functions */

static int mlx4_en_config_rss_qp(struct mlx4_en_priv *priv, int qpn,
				 struct mlx4_en_rx_ring *ring,
				 enum mlx4_qp_state *state,
				 struct mlx4_qp *qp)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	struct mlx4_qp_context *context;
	int err = 0;

<<<<<<< HEAD
	context = kmalloc(sizeof *context , GFP_KERNEL);
	if (!context) {
		en_err(priv, "Failed to allocate qp context\n");
		return -ENOMEM;
	}
=======
	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = mlx4_qp_alloc(mdev->dev, qpn, qp);
	if (err) {
		en_err(priv, "Failed to allocate qp #%x\n", qpn);
		goto out;
	}
	qp->event = mlx4_en_sqp_event;

<<<<<<< HEAD
	memset(context, 0, sizeof *context);
	mlx4_en_fill_qp_context(priv, ring->actual_size, ring->stride, 0, 0,
				qpn, ring->cqn, context);
=======
	mlx4_en_fill_qp_context(priv, ring->actual_size, ring->stride, 0, 0,
				qpn, ring->cqn, -1, context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	context->db_rec_addr = cpu_to_be64(ring->wqres.db.dma);

	/* Cancel FCS removal if FW allows */
	if (mdev->dev->caps.flags & MLX4_DEV_CAP_FLAG_FCS_KEEP) {
		context->param3 |= cpu_to_be32(1 << 29);
<<<<<<< HEAD
		ring->fcs_del = ETH_FCS_LEN;
=======
		if (priv->dev->features & NETIF_F_RXFCS)
			ring->fcs_del = 0;
		else
			ring->fcs_del = ETH_FCS_LEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		ring->fcs_del = 0;

	err = mlx4_qp_to_ready(mdev->dev, &ring->wqres.mtt, context, qp, state);
	if (err) {
		mlx4_qp_remove(mdev->dev, qp);
		mlx4_qp_free(mdev->dev, qp);
	}
	mlx4_en_update_rx_prod_db(ring);
out:
	kfree(context);
	return err;
}

<<<<<<< HEAD
=======
int mlx4_en_create_drop_qp(struct mlx4_en_priv *priv)
{
	int err;
	u32 qpn;

	err = mlx4_qp_reserve_range(priv->mdev->dev, 1, 1, &qpn,
				    MLX4_RESERVE_A0_QP,
				    MLX4_RES_USAGE_DRIVER);
	if (err) {
		en_err(priv, "Failed reserving drop qpn\n");
		return err;
	}
	err = mlx4_qp_alloc(priv->mdev->dev, qpn, &priv->drop_qp);
	if (err) {
		en_err(priv, "Failed allocating drop qp\n");
		mlx4_qp_release_range(priv->mdev->dev, qpn, 1);
		return err;
	}

	return 0;
}

void mlx4_en_destroy_drop_qp(struct mlx4_en_priv *priv)
{
	u32 qpn;

	qpn = priv->drop_qp.qpn;
	mlx4_qp_remove(priv->mdev->dev, &priv->drop_qp);
	mlx4_qp_free(priv->mdev->dev, &priv->drop_qp);
	mlx4_qp_release_range(priv->mdev->dev, qpn, 1);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Allocate rx qp's and configure them according to rss map */
int mlx4_en_config_rss_steer(struct mlx4_en_priv *priv)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	struct mlx4_en_rss_map *rss_map = &priv->rss_map;
	struct mlx4_qp_context context;
	struct mlx4_rss_context *rss_context;
	int rss_rings;
	void *ptr;
	u8 rss_mask = (MLX4_RSS_IPV4 | MLX4_RSS_TCP_IPV4 | MLX4_RSS_IPV6 |
			MLX4_RSS_TCP_IPV6);
	int i, qpn;
	int err = 0;
	int good_qps = 0;
<<<<<<< HEAD
	static const u32 rsskey[10] = { 0xD181C62C, 0xF7F4DB5B, 0x1983A2FC,
				0x943E1ADB, 0xD9389E6B, 0xD1039C2C, 0xA74499AD,
				0x593D56D9, 0xF3253C06, 0x2ADC1FFC};

	en_dbg(DRV, priv, "Configuring rss steering\n");
	err = mlx4_qp_reserve_range(mdev->dev, priv->rx_ring_num,
				    priv->rx_ring_num,
				    &rss_map->base_qpn);
=======
	u8 flags;

	en_dbg(DRV, priv, "Configuring rss steering\n");

	flags = priv->rx_ring_num == 1 ? MLX4_RESERVE_A0_QP : 0;
	err = mlx4_qp_reserve_range(mdev->dev, priv->rx_ring_num,
				    priv->rx_ring_num,
				    &rss_map->base_qpn, flags,
				    MLX4_RES_USAGE_DRIVER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		en_err(priv, "Failed reserving %d qps\n", priv->rx_ring_num);
		return err;
	}

	for (i = 0; i < priv->rx_ring_num; i++) {
		qpn = rss_map->base_qpn + i;
<<<<<<< HEAD
		err = mlx4_en_config_rss_qp(priv, qpn, &priv->rx_ring[i],
=======
		err = mlx4_en_config_rss_qp(priv, qpn, priv->rx_ring[i],
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    &rss_map->state[i],
					    &rss_map->qps[i]);
		if (err)
			goto rss_err;

		++good_qps;
	}

<<<<<<< HEAD
	/* Configure RSS indirection qp */
	err = mlx4_qp_alloc(mdev->dev, priv->base_qpn, &rss_map->indir_qp);
	if (err) {
		en_err(priv, "Failed to allocate RSS indirection QP\n");
		goto rss_err;
	}
	rss_map->indir_qp.event = mlx4_en_sqp_event;
	mlx4_en_fill_qp_context(priv, 0, 0, 0, 1, priv->base_qpn,
				priv->rx_ring[0].cqn, &context);
=======
	if (priv->rx_ring_num == 1) {
		rss_map->indir_qp = &rss_map->qps[0];
		priv->base_qpn = rss_map->indir_qp->qpn;
		en_info(priv, "Optimized Non-RSS steering\n");
		return 0;
	}

	rss_map->indir_qp = kzalloc(sizeof(*rss_map->indir_qp), GFP_KERNEL);
	if (!rss_map->indir_qp) {
		err = -ENOMEM;
		goto rss_err;
	}

	/* Configure RSS indirection qp */
	err = mlx4_qp_alloc(mdev->dev, priv->base_qpn, rss_map->indir_qp);
	if (err) {
		en_err(priv, "Failed to allocate RSS indirection QP\n");
		goto qp_alloc_err;
	}

	rss_map->indir_qp->event = mlx4_en_sqp_event;
	mlx4_en_fill_qp_context(priv, 0, 0, 0, 1, priv->base_qpn,
				priv->rx_ring[0]->cqn, -1, &context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!priv->prof->rss_rings || priv->prof->rss_rings > priv->rx_ring_num)
		rss_rings = priv->rx_ring_num;
	else
		rss_rings = priv->prof->rss_rings;

	ptr = ((void *) &context) + offsetof(struct mlx4_qp_context, pri_path)
					+ MLX4_RSS_OFFSET_IN_QPC_PRI_PATH;
	rss_context = ptr;
	rss_context->base_qpn = cpu_to_be32(ilog2(rss_rings) << 24 |
					    (rss_map->base_qpn));
	rss_context->default_qpn = cpu_to_be32(rss_map->base_qpn);
	if (priv->mdev->profile.udp_rss) {
		rss_mask |=  MLX4_RSS_UDP_IPV4 | MLX4_RSS_UDP_IPV6;
		rss_context->base_qpn_udp = rss_context->default_qpn;
	}
<<<<<<< HEAD
	rss_context->flags = rss_mask;
	rss_context->hash_fn = MLX4_RSS_HASH_TOP;
	for (i = 0; i < 10; i++)
		rss_context->rss_key[i] = cpu_to_be32(rsskey[i]);

	err = mlx4_qp_to_ready(mdev->dev, &priv->res.mtt, &context,
			       &rss_map->indir_qp, &rss_map->indir_state);
=======

	if (mdev->dev->caps.tunnel_offload_mode == MLX4_TUNNEL_OFFLOAD_MODE_VXLAN) {
		en_info(priv, "Setting RSS context tunnel type to RSS on inner headers\n");
		rss_mask |= MLX4_RSS_BY_INNER_HEADERS;
	}

	rss_context->flags = rss_mask;
	rss_context->hash_fn = MLX4_RSS_HASH_TOP;
	if (priv->rss_hash_fn == ETH_RSS_HASH_XOR) {
		rss_context->hash_fn = MLX4_RSS_HASH_XOR;
	} else if (priv->rss_hash_fn == ETH_RSS_HASH_TOP) {
		rss_context->hash_fn = MLX4_RSS_HASH_TOP;
		memcpy(rss_context->rss_key, priv->rss_key,
		       MLX4_EN_RSS_KEY_SIZE);
	} else {
		en_err(priv, "Unknown RSS hash function requested\n");
		err = -EINVAL;
		goto indir_err;
	}

	err = mlx4_qp_to_ready(mdev->dev, &priv->res.mtt, &context,
			       rss_map->indir_qp, &rss_map->indir_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto indir_err;

	return 0;

indir_err:
	mlx4_qp_modify(mdev->dev, NULL, rss_map->indir_state,
<<<<<<< HEAD
		       MLX4_QP_STATE_RST, NULL, 0, 0, &rss_map->indir_qp);
	mlx4_qp_remove(mdev->dev, &rss_map->indir_qp);
	mlx4_qp_free(mdev->dev, &rss_map->indir_qp);
=======
		       MLX4_QP_STATE_RST, NULL, 0, 0, rss_map->indir_qp);
	mlx4_qp_remove(mdev->dev, rss_map->indir_qp);
	mlx4_qp_free(mdev->dev, rss_map->indir_qp);
qp_alloc_err:
	kfree(rss_map->indir_qp);
	rss_map->indir_qp = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
rss_err:
	for (i = 0; i < good_qps; i++) {
		mlx4_qp_modify(mdev->dev, NULL, rss_map->state[i],
			       MLX4_QP_STATE_RST, NULL, 0, 0, &rss_map->qps[i]);
		mlx4_qp_remove(mdev->dev, &rss_map->qps[i]);
		mlx4_qp_free(mdev->dev, &rss_map->qps[i]);
	}
	mlx4_qp_release_range(mdev->dev, rss_map->base_qpn, priv->rx_ring_num);
	return err;
}

void mlx4_en_release_rss_steer(struct mlx4_en_priv *priv)
{
	struct mlx4_en_dev *mdev = priv->mdev;
	struct mlx4_en_rss_map *rss_map = &priv->rss_map;
	int i;

<<<<<<< HEAD
	mlx4_qp_modify(mdev->dev, NULL, rss_map->indir_state,
		       MLX4_QP_STATE_RST, NULL, 0, 0, &rss_map->indir_qp);
	mlx4_qp_remove(mdev->dev, &rss_map->indir_qp);
	mlx4_qp_free(mdev->dev, &rss_map->indir_qp);
=======
	if (priv->rx_ring_num > 1) {
		mlx4_qp_modify(mdev->dev, NULL, rss_map->indir_state,
			       MLX4_QP_STATE_RST, NULL, 0, 0,
			       rss_map->indir_qp);
		mlx4_qp_remove(mdev->dev, rss_map->indir_qp);
		mlx4_qp_free(mdev->dev, rss_map->indir_qp);
		kfree(rss_map->indir_qp);
		rss_map->indir_qp = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < priv->rx_ring_num; i++) {
		mlx4_qp_modify(mdev->dev, NULL, rss_map->state[i],
			       MLX4_QP_STATE_RST, NULL, 0, 0, &rss_map->qps[i]);
		mlx4_qp_remove(mdev->dev, &rss_map->qps[i]);
		mlx4_qp_free(mdev->dev, &rss_map->qps[i]);
	}
	mlx4_qp_release_range(mdev->dev, rss_map->base_qpn, priv->rx_ring_num);
}
<<<<<<< HEAD





=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
