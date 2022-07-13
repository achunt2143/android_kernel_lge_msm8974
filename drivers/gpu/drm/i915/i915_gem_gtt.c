<<<<<<< HEAD
/*
 * Copyright © 2010 Daniel Vetter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "drmP.h"
#include "drm.h"
#include "i915_drm.h"
#include "i915_drv.h"
#include "i915_trace.h"
#include "intel_drv.h"

/* PPGTT support for Sandybdrige/Gen6 and later */
static void i915_ppgtt_clear_range(struct i915_hw_ppgtt *ppgtt,
				   unsigned first_entry,
				   unsigned num_entries)
{
	uint32_t *pt_vaddr;
	uint32_t scratch_pte;
	unsigned act_pd = first_entry / I915_PPGTT_PT_ENTRIES;
	unsigned first_pte = first_entry % I915_PPGTT_PT_ENTRIES;
	unsigned last_pte, i;

	scratch_pte = GEN6_PTE_ADDR_ENCODE(ppgtt->scratch_page_dma_addr);
	scratch_pte |= GEN6_PTE_VALID | GEN6_PTE_CACHE_LLC;

	while (num_entries) {
		last_pte = first_pte + num_entries;
		if (last_pte > I915_PPGTT_PT_ENTRIES)
			last_pte = I915_PPGTT_PT_ENTRIES;

		pt_vaddr = kmap_atomic(ppgtt->pt_pages[act_pd]);

		for (i = first_pte; i < last_pte; i++)
			pt_vaddr[i] = scratch_pte;

		kunmap_atomic(pt_vaddr);

		num_entries -= last_pte - first_pte;
		first_pte = 0;
		act_pd++;
	}
}

int i915_gem_init_aliasing_ppgtt(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct i915_hw_ppgtt *ppgtt;
	unsigned first_pd_entry_in_global_pt;
	int i;
	int ret = -ENOMEM;

	/* ppgtt PDEs reside in the global gtt pagetable, which has 512*1024
	 * entries. For aliasing ppgtt support we just steal them at the end for
	 * now. */
	first_pd_entry_in_global_pt = dev_priv->mm.gtt->gtt_total_entries - I915_PPGTT_PD_ENTRIES;

	ppgtt = kzalloc(sizeof(*ppgtt), GFP_KERNEL);
	if (!ppgtt)
		return ret;

	ppgtt->num_pd_entries = I915_PPGTT_PD_ENTRIES;
	ppgtt->pt_pages = kzalloc(sizeof(struct page *)*ppgtt->num_pd_entries,
				  GFP_KERNEL);
	if (!ppgtt->pt_pages)
		goto err_ppgtt;

	for (i = 0; i < ppgtt->num_pd_entries; i++) {
		ppgtt->pt_pages[i] = alloc_page(GFP_KERNEL);
		if (!ppgtt->pt_pages[i])
			goto err_pt_alloc;
	}

	if (dev_priv->mm.gtt->needs_dmar) {
		ppgtt->pt_dma_addr = kzalloc(sizeof(dma_addr_t)
						*ppgtt->num_pd_entries,
					     GFP_KERNEL);
		if (!ppgtt->pt_dma_addr)
			goto err_pt_alloc;
	}

	for (i = 0; i < ppgtt->num_pd_entries; i++) {
		dma_addr_t pt_addr;
		if (dev_priv->mm.gtt->needs_dmar) {
			pt_addr = pci_map_page(dev->pdev, ppgtt->pt_pages[i],
					       0, 4096,
					       PCI_DMA_BIDIRECTIONAL);

			if (pci_dma_mapping_error(dev->pdev,
						  pt_addr)) {
				ret = -EIO;
				goto err_pd_pin;

			}
			ppgtt->pt_dma_addr[i] = pt_addr;
		} else
			pt_addr = page_to_phys(ppgtt->pt_pages[i]);
	}

	ppgtt->scratch_page_dma_addr = dev_priv->mm.gtt->scratch_page_dma;

	i915_ppgtt_clear_range(ppgtt, 0,
			       ppgtt->num_pd_entries*I915_PPGTT_PT_ENTRIES);

	ppgtt->pd_offset = (first_pd_entry_in_global_pt)*sizeof(uint32_t);

	dev_priv->mm.aliasing_ppgtt = ppgtt;

	return 0;

err_pd_pin:
	if (ppgtt->pt_dma_addr) {
		for (i--; i >= 0; i--)
			pci_unmap_page(dev->pdev, ppgtt->pt_dma_addr[i],
				       4096, PCI_DMA_BIDIRECTIONAL);
	}
err_pt_alloc:
	kfree(ppgtt->pt_dma_addr);
	for (i = 0; i < ppgtt->num_pd_entries; i++) {
		if (ppgtt->pt_pages[i])
			__free_page(ppgtt->pt_pages[i]);
	}
	kfree(ppgtt->pt_pages);
err_ppgtt:
	kfree(ppgtt);

	return ret;
}

void i915_gem_cleanup_aliasing_ppgtt(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct i915_hw_ppgtt *ppgtt = dev_priv->mm.aliasing_ppgtt;
	int i;

	if (!ppgtt)
		return;

	if (ppgtt->pt_dma_addr) {
		for (i = 0; i < ppgtt->num_pd_entries; i++)
			pci_unmap_page(dev->pdev, ppgtt->pt_dma_addr[i],
				       4096, PCI_DMA_BIDIRECTIONAL);
	}

	kfree(ppgtt->pt_dma_addr);
	for (i = 0; i < ppgtt->num_pd_entries; i++)
		__free_page(ppgtt->pt_pages[i]);
	kfree(ppgtt->pt_pages);
	kfree(ppgtt);
}

static void i915_ppgtt_insert_sg_entries(struct i915_hw_ppgtt *ppgtt,
					 struct scatterlist *sg_list,
					 unsigned sg_len,
					 unsigned first_entry,
					 uint32_t pte_flags)
{
	uint32_t *pt_vaddr, pte;
	unsigned act_pd = first_entry / I915_PPGTT_PT_ENTRIES;
	unsigned first_pte = first_entry % I915_PPGTT_PT_ENTRIES;
	unsigned i, j, m, segment_len;
	dma_addr_t page_addr;
	struct scatterlist *sg;

	/* init sg walking */
	sg = sg_list;
	i = 0;
	segment_len = sg_dma_len(sg) >> PAGE_SHIFT;
	m = 0;

	while (i < sg_len) {
		pt_vaddr = kmap_atomic(ppgtt->pt_pages[act_pd]);

		for (j = first_pte; j < I915_PPGTT_PT_ENTRIES; j++) {
			page_addr = sg_dma_address(sg) + (m << PAGE_SHIFT);
			pte = GEN6_PTE_ADDR_ENCODE(page_addr);
			pt_vaddr[j] = pte | pte_flags;

			/* grab the next page */
			m++;
			if (m == segment_len) {
				sg = sg_next(sg);
				i++;
				if (i == sg_len)
					break;

				segment_len = sg_dma_len(sg) >> PAGE_SHIFT;
				m = 0;
			}
		}

		kunmap_atomic(pt_vaddr);

		first_pte = 0;
		act_pd++;
	}
}

static void i915_ppgtt_insert_pages(struct i915_hw_ppgtt *ppgtt,
				    unsigned first_entry, unsigned num_entries,
				    struct page **pages, uint32_t pte_flags)
{
	uint32_t *pt_vaddr, pte;
	unsigned act_pd = first_entry / I915_PPGTT_PT_ENTRIES;
	unsigned first_pte = first_entry % I915_PPGTT_PT_ENTRIES;
	unsigned last_pte, i;
	dma_addr_t page_addr;

	while (num_entries) {
		last_pte = first_pte + num_entries;
		last_pte = min_t(unsigned, last_pte, I915_PPGTT_PT_ENTRIES);

		pt_vaddr = kmap_atomic(ppgtt->pt_pages[act_pd]);

		for (i = first_pte; i < last_pte; i++) {
			page_addr = page_to_phys(*pages);
			pte = GEN6_PTE_ADDR_ENCODE(page_addr);
			pt_vaddr[i] = pte | pte_flags;

			pages++;
		}

		kunmap_atomic(pt_vaddr);

		num_entries -= last_pte - first_pte;
		first_pte = 0;
		act_pd++;
	}
}

void i915_ppgtt_bind_object(struct i915_hw_ppgtt *ppgtt,
			    struct drm_i915_gem_object *obj,
			    enum i915_cache_level cache_level)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t pte_flags = GEN6_PTE_VALID;

	switch (cache_level) {
	case I915_CACHE_LLC_MLC:
		pte_flags |= GEN6_PTE_CACHE_LLC_MLC;
		break;
	case I915_CACHE_LLC:
		pte_flags |= GEN6_PTE_CACHE_LLC;
		break;
	case I915_CACHE_NONE:
		pte_flags |= GEN6_PTE_UNCACHED;
		break;
	default:
		BUG();
	}

	if (dev_priv->mm.gtt->needs_dmar) {
		BUG_ON(!obj->sg_list);

		i915_ppgtt_insert_sg_entries(ppgtt,
					     obj->sg_list,
					     obj->num_sg,
					     obj->gtt_space->start >> PAGE_SHIFT,
					     pte_flags);
	} else
		i915_ppgtt_insert_pages(ppgtt,
					obj->gtt_space->start >> PAGE_SHIFT,
					obj->base.size >> PAGE_SHIFT,
					obj->pages,
					pte_flags);
}

void i915_ppgtt_unbind_object(struct i915_hw_ppgtt *ppgtt,
			      struct drm_i915_gem_object *obj)
{
	i915_ppgtt_clear_range(ppgtt,
			       obj->gtt_space->start >> PAGE_SHIFT,
			       obj->base.size >> PAGE_SHIFT);
}

/* XXX kill agp_type! */
static unsigned int cache_level_to_agp_type(struct drm_device *dev,
					    enum i915_cache_level cache_level)
{
	switch (cache_level) {
	case I915_CACHE_LLC_MLC:
		if (INTEL_INFO(dev)->gen >= 6)
			return AGP_USER_CACHED_MEMORY_LLC_MLC;
		/* Older chipsets do not have this extra level of CPU
		 * cacheing, so fallthrough and request the PTE simply
		 * as cached.
		 */
	case I915_CACHE_LLC:
		return AGP_USER_CACHED_MEMORY;
	default:
	case I915_CACHE_NONE:
		return AGP_USER_MEMORY;
	}
}

static bool do_idling(struct drm_i915_private *dev_priv)
{
	bool ret = dev_priv->mm.interruptible;

	if (unlikely(dev_priv->mm.gtt->do_idle_maps)) {
		dev_priv->mm.interruptible = false;
		if (i915_gpu_idle(dev_priv->dev, false)) {
			DRM_ERROR("Couldn't idle GPU\n");
			/* Wait a bit, in hopes it avoids the hang */
			udelay(10);
		}
	}

	return ret;
}

static void undo_idling(struct drm_i915_private *dev_priv, bool interruptible)
{
	if (unlikely(dev_priv->mm.gtt->do_idle_maps))
		dev_priv->mm.interruptible = interruptible;
}

void i915_gem_restore_gtt_mappings(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj;

	/* First fill our portion of the GTT with scratch pages */
	intel_gtt_clear_range(dev_priv->mm.gtt_start / PAGE_SIZE,
			      (dev_priv->mm.gtt_end - dev_priv->mm.gtt_start) / PAGE_SIZE);

	list_for_each_entry(obj, &dev_priv->mm.gtt_list, gtt_list) {
		i915_gem_clflush_object(obj);
		i915_gem_gtt_rebind_object(obj, obj->cache_level);
	}

	intel_gtt_chipset_flush();
}

int i915_gem_gtt_bind_object(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned int agp_type = cache_level_to_agp_type(dev, obj->cache_level);
	int ret;

	if (dev_priv->mm.gtt->needs_dmar) {
		ret = intel_gtt_map_memory(obj->pages,
					   obj->base.size >> PAGE_SHIFT,
					   &obj->sg_list,
					   &obj->num_sg);
		if (ret != 0)
			return ret;

		intel_gtt_insert_sg_entries(obj->sg_list,
					    obj->num_sg,
					    obj->gtt_space->start >> PAGE_SHIFT,
					    agp_type);
	} else
		intel_gtt_insert_pages(obj->gtt_space->start >> PAGE_SHIFT,
				       obj->base.size >> PAGE_SHIFT,
				       obj->pages,
				       agp_type);

	return 0;
}

void i915_gem_gtt_rebind_object(struct drm_i915_gem_object *obj,
				enum i915_cache_level cache_level)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned int agp_type = cache_level_to_agp_type(dev, cache_level);

	if (dev_priv->mm.gtt->needs_dmar) {
		BUG_ON(!obj->sg_list);

		intel_gtt_insert_sg_entries(obj->sg_list,
					    obj->num_sg,
					    obj->gtt_space->start >> PAGE_SHIFT,
					    agp_type);
	} else
		intel_gtt_insert_pages(obj->gtt_space->start >> PAGE_SHIFT,
				       obj->base.size >> PAGE_SHIFT,
				       obj->pages,
				       agp_type);
}

void i915_gem_gtt_unbind_object(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	bool interruptible;

	interruptible = do_idling(dev_priv);

	intel_gtt_clear_range(obj->gtt_space->start >> PAGE_SHIFT,
			      obj->base.size >> PAGE_SHIFT);

	if (obj->sg_list) {
		intel_gtt_unmap_memory(obj->sg_list, obj->num_sg);
		obj->sg_list = NULL;
	}

	undo_idling(dev_priv, interruptible);
}
=======
// SPDX-License-Identifier: MIT
/*
 * Copyright © 2010 Daniel Vetter
 * Copyright © 2020 Intel Corporation
 */

#include <linux/slab.h> /* fault-inject.h is not standalone! */

#include <linux/fault-inject.h>
#include <linux/log2.h>
#include <linux/random.h>
#include <linux/seq_file.h>
#include <linux/stop_machine.h>

#include <asm/set_memory.h>
#include <asm/smp.h>

#include "display/intel_frontbuffer.h"
#include "gt/intel_gt.h"
#include "gt/intel_gt_requests.h"

#include "i915_drv.h"
#include "i915_gem_evict.h"
#include "i915_scatterlist.h"
#include "i915_trace.h"
#include "i915_vgpu.h"

int i915_gem_gtt_prepare_pages(struct drm_i915_gem_object *obj,
			       struct sg_table *pages)
{
	do {
		if (dma_map_sg_attrs(obj->base.dev->dev,
				     pages->sgl, pages->nents,
				     DMA_BIDIRECTIONAL,
				     DMA_ATTR_SKIP_CPU_SYNC |
				     DMA_ATTR_NO_KERNEL_MAPPING |
				     DMA_ATTR_NO_WARN))
			return 0;

		/*
		 * If the DMA remap fails, one cause can be that we have
		 * too many objects pinned in a small remapping table,
		 * such as swiotlb. Incrementally purge all other objects and
		 * try again - if there are no more pages to remove from
		 * the DMA remapper, i915_gem_shrink will return 0.
		 */
		GEM_BUG_ON(obj->mm.pages == pages);
	} while (i915_gem_shrink(NULL, to_i915(obj->base.dev),
				 obj->base.size >> PAGE_SHIFT, NULL,
				 I915_SHRINK_BOUND |
				 I915_SHRINK_UNBOUND));

	return -ENOSPC;
}

void i915_gem_gtt_finish_pages(struct drm_i915_gem_object *obj,
			       struct sg_table *pages)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;

	/* XXX This does not prevent more requests being submitted! */
	if (unlikely(ggtt->do_idle_maps))
		/* Wait a bit, in the hope it avoids the hang */
		usleep_range(100, 250);

	dma_unmap_sg(i915->drm.dev, pages->sgl, pages->nents,
		     DMA_BIDIRECTIONAL);
}

/**
 * i915_gem_gtt_reserve - reserve a node in an address_space (GTT)
 * @vm: the &struct i915_address_space
 * @ww: An optional struct i915_gem_ww_ctx.
 * @node: the &struct drm_mm_node (typically i915_vma.mode)
 * @size: how much space to allocate inside the GTT,
 *        must be #I915_GTT_PAGE_SIZE aligned
 * @offset: where to insert inside the GTT,
 *          must be #I915_GTT_MIN_ALIGNMENT aligned, and the node
 *          (@offset + @size) must fit within the address space
 * @color: color to apply to node, if this node is not from a VMA,
 *         color must be #I915_COLOR_UNEVICTABLE
 * @flags: control search and eviction behaviour
 *
 * i915_gem_gtt_reserve() tries to insert the @node at the exact @offset inside
 * the address space (using @size and @color). If the @node does not fit, it
 * tries to evict any overlapping nodes from the GTT, including any
 * neighbouring nodes if the colors do not match (to ensure guard pages between
 * differing domains). See i915_gem_evict_for_node() for the gory details
 * on the eviction algorithm. #PIN_NONBLOCK may used to prevent waiting on
 * evicting active overlapping objects, and any overlapping node that is pinned
 * or marked as unevictable will also result in failure.
 *
 * Returns: 0 on success, -ENOSPC if no suitable hole is found, -EINTR if
 * asked to wait for eviction and interrupted.
 */
int i915_gem_gtt_reserve(struct i915_address_space *vm,
			 struct i915_gem_ww_ctx *ww,
			 struct drm_mm_node *node,
			 u64 size, u64 offset, unsigned long color,
			 unsigned int flags)
{
	int err;

	GEM_BUG_ON(!size);
	GEM_BUG_ON(!IS_ALIGNED(size, I915_GTT_PAGE_SIZE));
	GEM_BUG_ON(!IS_ALIGNED(offset, I915_GTT_MIN_ALIGNMENT));
	GEM_BUG_ON(range_overflows(offset, size, vm->total));
	GEM_BUG_ON(vm == &to_gt(vm->i915)->ggtt->alias->vm);
	GEM_BUG_ON(drm_mm_node_allocated(node));

	node->size = size;
	node->start = offset;
	node->color = color;

	err = drm_mm_reserve_node(&vm->mm, node);
	if (err != -ENOSPC)
		return err;

	if (flags & PIN_NOEVICT)
		return -ENOSPC;

	err = i915_gem_evict_for_node(vm, ww, node, flags);
	if (err == 0)
		err = drm_mm_reserve_node(&vm->mm, node);

	return err;
}

static u64 random_offset(u64 start, u64 end, u64 len, u64 align)
{
	u64 range, addr;

	GEM_BUG_ON(range_overflows(start, len, end));
	GEM_BUG_ON(round_up(start, align) > round_down(end - len, align));

	range = round_down(end - len, align) - round_up(start, align);
	if (range) {
		if (sizeof(unsigned long) == sizeof(u64)) {
			addr = get_random_u64();
		} else {
			addr = get_random_u32();
			if (range > U32_MAX) {
				addr <<= 32;
				addr |= get_random_u32();
			}
		}
		div64_u64_rem(addr, range, &addr);
		start += addr;
	}

	return round_up(start, align);
}

/**
 * i915_gem_gtt_insert - insert a node into an address_space (GTT)
 * @vm: the &struct i915_address_space
 * @ww: An optional struct i915_gem_ww_ctx.
 * @node: the &struct drm_mm_node (typically i915_vma.node)
 * @size: how much space to allocate inside the GTT,
 *        must be #I915_GTT_PAGE_SIZE aligned
 * @alignment: required alignment of starting offset, may be 0 but
 *             if specified, this must be a power-of-two and at least
 *             #I915_GTT_MIN_ALIGNMENT
 * @color: color to apply to node
 * @start: start of any range restriction inside GTT (0 for all),
 *         must be #I915_GTT_PAGE_SIZE aligned
 * @end: end of any range restriction inside GTT (U64_MAX for all),
 *       must be #I915_GTT_PAGE_SIZE aligned if not U64_MAX
 * @flags: control search and eviction behaviour
 *
 * i915_gem_gtt_insert() first searches for an available hole into which
 * is can insert the node. The hole address is aligned to @alignment and
 * its @size must then fit entirely within the [@start, @end] bounds. The
 * nodes on either side of the hole must match @color, or else a guard page
 * will be inserted between the two nodes (or the node evicted). If no
 * suitable hole is found, first a victim is randomly selected and tested
 * for eviction, otherwise then the LRU list of objects within the GTT
 * is scanned to find the first set of replacement nodes to create the hole.
 * Those old overlapping nodes are evicted from the GTT (and so must be
 * rebound before any future use). Any node that is currently pinned cannot
 * be evicted (see i915_vma_pin()). Similar if the node's VMA is currently
 * active and #PIN_NONBLOCK is specified, that node is also skipped when
 * searching for an eviction candidate. See i915_gem_evict_something() for
 * the gory details on the eviction algorithm.
 *
 * Returns: 0 on success, -ENOSPC if no suitable hole is found, -EINTR if
 * asked to wait for eviction and interrupted.
 */
int i915_gem_gtt_insert(struct i915_address_space *vm,
			struct i915_gem_ww_ctx *ww,
			struct drm_mm_node *node,
			u64 size, u64 alignment, unsigned long color,
			u64 start, u64 end, unsigned int flags)
{
	enum drm_mm_insert_mode mode;
	u64 offset;
	int err;

	lockdep_assert_held(&vm->mutex);

	GEM_BUG_ON(!size);
	GEM_BUG_ON(!IS_ALIGNED(size, I915_GTT_PAGE_SIZE));
	GEM_BUG_ON(alignment && !is_power_of_2(alignment));
	GEM_BUG_ON(alignment && !IS_ALIGNED(alignment, I915_GTT_MIN_ALIGNMENT));
	GEM_BUG_ON(start >= end);
	GEM_BUG_ON(start > 0  && !IS_ALIGNED(start, I915_GTT_PAGE_SIZE));
	GEM_BUG_ON(end < U64_MAX && !IS_ALIGNED(end, I915_GTT_PAGE_SIZE));
	GEM_BUG_ON(vm == &to_gt(vm->i915)->ggtt->alias->vm);
	GEM_BUG_ON(drm_mm_node_allocated(node));

	if (unlikely(range_overflows(start, size, end)))
		return -ENOSPC;

	if (unlikely(round_up(start, alignment) > round_down(end - size, alignment)))
		return -ENOSPC;

	mode = DRM_MM_INSERT_BEST;
	if (flags & PIN_HIGH)
		mode = DRM_MM_INSERT_HIGHEST;
	if (flags & PIN_MAPPABLE)
		mode = DRM_MM_INSERT_LOW;

	/* We only allocate in PAGE_SIZE/GTT_PAGE_SIZE (4096) chunks,
	 * so we know that we always have a minimum alignment of 4096.
	 * The drm_mm range manager is optimised to return results
	 * with zero alignment, so where possible use the optimal
	 * path.
	 */
	BUILD_BUG_ON(I915_GTT_MIN_ALIGNMENT > I915_GTT_PAGE_SIZE);
	if (alignment <= I915_GTT_MIN_ALIGNMENT)
		alignment = 0;

	err = drm_mm_insert_node_in_range(&vm->mm, node,
					  size, alignment, color,
					  start, end, mode);
	if (err != -ENOSPC)
		return err;

	if (mode & DRM_MM_INSERT_ONCE) {
		err = drm_mm_insert_node_in_range(&vm->mm, node,
						  size, alignment, color,
						  start, end,
						  DRM_MM_INSERT_BEST);
		if (err != -ENOSPC)
			return err;
	}

	if (flags & PIN_NOEVICT)
		return -ENOSPC;

	/*
	 * No free space, pick a slot at random.
	 *
	 * There is a pathological case here using a GTT shared between
	 * mmap and GPU (i.e. ggtt/aliasing_ppgtt but not full-ppgtt):
	 *
	 *    |<-- 256 MiB aperture -->||<-- 1792 MiB unmappable -->|
	 *         (64k objects)             (448k objects)
	 *
	 * Now imagine that the eviction LRU is ordered top-down (just because
	 * pathology meets real life), and that we need to evict an object to
	 * make room inside the aperture. The eviction scan then has to walk
	 * the 448k list before it finds one within range. And now imagine that
	 * it has to search for a new hole between every byte inside the memcpy,
	 * for several simultaneous clients.
	 *
	 * On a full-ppgtt system, if we have run out of available space, there
	 * will be lots and lots of objects in the eviction list! Again,
	 * searching that LRU list may be slow if we are also applying any
	 * range restrictions (e.g. restriction to low 4GiB) and so, for
	 * simplicity and similarilty between different GTT, try the single
	 * random replacement first.
	 */
	offset = random_offset(start, end,
			       size, alignment ?: I915_GTT_MIN_ALIGNMENT);
	err = i915_gem_gtt_reserve(vm, ww, node, size, offset, color, flags);
	if (err != -ENOSPC)
		return err;

	if (flags & PIN_NOSEARCH)
		return -ENOSPC;

	/* Randomly selected placement is pinned, do a search */
	err = i915_gem_evict_something(vm, ww, size, alignment, color,
				       start, end, flags);
	if (err)
		return err;

	return drm_mm_insert_node_in_range(&vm->mm, node,
					   size, alignment, color,
					   start, end, DRM_MM_INSERT_EVICT);
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/i915_gem_gtt.c"
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
