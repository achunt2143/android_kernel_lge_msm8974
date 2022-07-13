<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (c) 2007, Intel Corporation.
 * All Rights Reserved.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors: Thomas Hellstrom <thomas-at-tungstengraphics.com>
 *	    Alan Cox <alan@linux.intel.com>
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <linux/shmem_fs.h>
=======
#include "gem.h" /* TODO: for struct psb_gem_object, see psb_gtt_restore() */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "psb_drv.h"


/*
 *	GTT resource allocator - manage page mappings in GTT space
 */

<<<<<<< HEAD
=======
int psb_gtt_allocate_resource(struct drm_psb_private *pdev, struct resource *res,
			      const char *name, resource_size_t size, resource_size_t align,
			      bool stolen, u32 *offset)
{
	struct resource *root = pdev->gtt_mem;
	resource_size_t start, end;
	int ret;

	if (stolen) {
		/* The start of the GTT is backed by stolen pages. */
		start = root->start;
		end = root->start + pdev->gtt.stolen_size - 1;
	} else {
		/* The rest is backed by system pages. */
		start = root->start + pdev->gtt.stolen_size;
		end = root->end;
	}

	res->name = name;
	ret = allocate_resource(root, res, size, start, end, align, NULL, NULL);
	if (ret)
		return ret;
	*offset = res->start - root->start;

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 *	psb_gtt_mask_pte	-	generate GTT pte entry
 *	@pfn: page number to encode
 *	@type: type of memory in the GTT
 *
 *	Set the GTT entry for the appropriate memory type.
 */
<<<<<<< HEAD
static inline uint32_t psb_gtt_mask_pte(uint32_t pfn, int type)
{
	uint32_t mask = PSB_PTE_VALID;

=======
uint32_t psb_gtt_mask_pte(uint32_t pfn, int type)
{
	uint32_t mask = PSB_PTE_VALID;

	/* Ensure we explode rather than put an invalid low mapping of
	   a high mapping page into the gtt */
	BUG_ON(pfn & ~(0xFFFFFFFF >> PAGE_SHIFT));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (type & PSB_MMU_CACHED_MEMORY)
		mask |= PSB_PTE_CACHED;
	if (type & PSB_MMU_RO_MEMORY)
		mask |= PSB_PTE_RO;
	if (type & PSB_MMU_WO_MEMORY)
		mask |= PSB_PTE_WO;

	return (pfn << PAGE_SHIFT) | mask;
}

<<<<<<< HEAD
/**
 *	psb_gtt_entry		-	find the GTT entries for a gtt_range
 *	@dev: our DRM device
 *	@r: our GTT range
 *
 *	Given a gtt_range object return the GTT offset of the page table
 *	entries for this gtt_range
 */
static u32 *psb_gtt_entry(struct drm_device *dev, struct gtt_range *r)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	unsigned long offset;

	offset = r->resource.start - dev_priv->gtt_mem->start;

	return dev_priv->gtt_map + (offset >> PAGE_SHIFT);
}

/**
 *	psb_gtt_insert	-	put an object into the GTT
 *	@dev: our DRM device
 *	@r: our GTT range
 *
 *	Take our preallocated GTT range and insert the GEM object into
 *	the GTT. This is protected via the gtt mutex which the caller
 *	must hold.
 */
static int psb_gtt_insert(struct drm_device *dev, struct gtt_range *r)
{
	u32 *gtt_slot, pte;
	struct page **pages;
	int i;

	if (r->pages == NULL) {
		WARN_ON(1);
		return -EINVAL;
	}

	WARN_ON(r->stolen);	/* refcount these maybe ? */

	gtt_slot = psb_gtt_entry(dev, r);
	pages = r->pages;

	/* Make sure changes are visible to the GPU */
	set_pages_array_uc(pages, r->npage);

	/* Write our page entries into the GTT itself */
	for (i = r->roll; i < r->npage; i++) {
		pte = psb_gtt_mask_pte(page_to_pfn(r->pages[i]), 0);
		iowrite32(pte, gtt_slot++);
	}
	for (i = 0; i < r->roll; i++) {
		pte = psb_gtt_mask_pte(page_to_pfn(r->pages[i]), 0);
		iowrite32(pte, gtt_slot++);
	}
	/* Make sure all the entries are set before we return */
	ioread32(gtt_slot - 1);

	return 0;
}

/**
 *	psb_gtt_remove	-	remove an object from the GTT
 *	@dev: our DRM device
 *	@r: our GTT range
 *
 *	Remove a preallocated GTT range from the GTT. Overwrite all the
 *	page table entries with the dummy page. This is protected via the gtt
 *	mutex which the caller must hold.
 */
static void psb_gtt_remove(struct drm_device *dev, struct gtt_range *r)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	u32 *gtt_slot, pte;
	int i;

	WARN_ON(r->stolen);

	gtt_slot = psb_gtt_entry(dev, r);
	pte = psb_gtt_mask_pte(page_to_pfn(dev_priv->scratch_page), 0);

	for (i = 0; i < r->npage; i++)
		iowrite32(pte, gtt_slot++);
	ioread32(gtt_slot - 1);
	set_pages_array_wb(r->pages, r->npage);
}

/**
 *	psb_gtt_roll	-	set scrolling position
 *	@dev: our DRM device
 *	@r: the gtt mapping we are using
 *	@roll: roll offset
 *
 *	Roll an existing pinned mapping by moving the pages through the GTT.
 *	This allows us to implement hardware scrolling on the consoles without
 *	a 2D engine
 */
void psb_gtt_roll(struct drm_device *dev, struct gtt_range *r, int roll)
{
	u32 *gtt_slot, pte;
	int i;

	if (roll >= r->npage) {
		WARN_ON(1);
		return;
	}

	r->roll = roll;

	/* Not currently in the GTT - no worry we will write the mapping at
	   the right position when it gets pinned */
	if (!r->stolen && !r->in_gart)
		return;

	gtt_slot = psb_gtt_entry(dev, r);

	for (i = r->roll; i < r->npage; i++) {
		pte = psb_gtt_mask_pte(page_to_pfn(r->pages[i]), 0);
		iowrite32(pte, gtt_slot++);
	}
	for (i = 0; i < r->roll; i++) {
		pte = psb_gtt_mask_pte(page_to_pfn(r->pages[i]), 0);
		iowrite32(pte, gtt_slot++);
	}
	ioread32(gtt_slot - 1);
}

/**
 *	psb_gtt_attach_pages	-	attach and pin GEM pages
 *	@gt: the gtt range
 *
 *	Pin and build an in kernel list of the pages that back our GEM object.
 *	While we hold this the pages cannot be swapped out. This is protected
 *	via the gtt mutex which the caller must hold.
 */
static int psb_gtt_attach_pages(struct gtt_range *gt)
{
	struct inode *inode;
	struct address_space *mapping;
	int i;
	struct page *p;
	int pages = gt->gem.size / PAGE_SIZE;

	WARN_ON(gt->pages);

	/* This is the shared memory object that backs the GEM resource */
	inode = gt->gem.filp->f_path.dentry->d_inode;
	mapping = inode->i_mapping;

	gt->pages = kmalloc(pages * sizeof(struct page *), GFP_KERNEL);
	if (gt->pages == NULL)
		return -ENOMEM;
	gt->npage = pages;

	for (i = 0; i < pages; i++) {
		p = shmem_read_mapping_page(mapping, i);
		if (IS_ERR(p))
			goto err;
		gt->pages[i] = p;
	}
	return 0;

err:
	while (i--)
		page_cache_release(gt->pages[i]);
	kfree(gt->pages);
	gt->pages = NULL;
	return PTR_ERR(p);
}

/**
 *	psb_gtt_detach_pages	-	attach and pin GEM pages
 *	@gt: the gtt range
 *
 *	Undo the effect of psb_gtt_attach_pages. At this point the pages
 *	must have been removed from the GTT as they could now be paged out
 *	and move bus address. This is protected via the gtt mutex which the
 *	caller must hold.
 */
static void psb_gtt_detach_pages(struct gtt_range *gt)
{
	int i;
	for (i = 0; i < gt->npage; i++) {
		/* FIXME: do we need to force dirty */
		set_page_dirty(gt->pages[i]);
		page_cache_release(gt->pages[i]);
	}
	kfree(gt->pages);
	gt->pages = NULL;
}

/**
 *	psb_gtt_pin		-	pin pages into the GTT
 *	@gt: range to pin
 *
 *	Pin a set of pages into the GTT. The pins are refcounted so that
 *	multiple pins need multiple unpins to undo.
 *
 *	Non GEM backed objects treat this as a no-op as they are always GTT
 *	backed objects.
 */
int psb_gtt_pin(struct gtt_range *gt)
{
	int ret = 0;
	struct drm_device *dev = gt->gem.dev;
	struct drm_psb_private *dev_priv = dev->dev_private;

	mutex_lock(&dev_priv->gtt_mutex);

	if (gt->in_gart == 0 && gt->stolen == 0) {
		ret = psb_gtt_attach_pages(gt);
		if (ret < 0)
			goto out;
		ret = psb_gtt_insert(dev, gt);
		if (ret < 0) {
			psb_gtt_detach_pages(gt);
			goto out;
		}
	}
	gt->in_gart++;
out:
	mutex_unlock(&dev_priv->gtt_mutex);
	return ret;
}

/**
 *	psb_gtt_unpin		-	Drop a GTT pin requirement
 *	@gt: range to pin
 *
 *	Undoes the effect of psb_gtt_pin. On the last drop the GEM object
 *	will be removed from the GTT which will also drop the page references
 *	and allow the VM to clean up or page stuff.
 *
 *	Non GEM backed objects treat this as a no-op as they are always GTT
 *	backed objects.
 */
void psb_gtt_unpin(struct gtt_range *gt)
{
	struct drm_device *dev = gt->gem.dev;
	struct drm_psb_private *dev_priv = dev->dev_private;

	mutex_lock(&dev_priv->gtt_mutex);

	WARN_ON(!gt->in_gart);

	gt->in_gart--;
	if (gt->in_gart == 0 && gt->stolen == 0) {
		psb_gtt_remove(dev, gt);
		psb_gtt_detach_pages(gt);
	}
	mutex_unlock(&dev_priv->gtt_mutex);
}

/*
 *	GTT resource allocator - allocate and manage GTT address space
 */

/**
 *	psb_gtt_alloc_range	-	allocate GTT address space
 *	@dev: Our DRM device
 *	@len: length (bytes) of address space required
 *	@name: resource name
 *	@backed: resource should be backed by stolen pages
 *
 *	Ask the kernel core to find us a suitable range of addresses
 *	to use for a GTT mapping.
 *
 *	Returns a gtt_range structure describing the object, or NULL on
 *	error. On successful return the resource is both allocated and marked
 *	as in use.
 */
struct gtt_range *psb_gtt_alloc_range(struct drm_device *dev, int len,
						const char *name, int backed)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct gtt_range *gt;
	struct resource *r = dev_priv->gtt_mem;
	int ret;
	unsigned long start, end;

	if (backed) {
		/* The start of the GTT is the stolen pages */
		start = r->start;
		end = r->start + dev_priv->gtt.stolen_size - 1;
	} else {
		/* The rest we will use for GEM backed objects */
		start = r->start + dev_priv->gtt.stolen_size;
		end = r->end;
	}

	gt = kzalloc(sizeof(struct gtt_range), GFP_KERNEL);
	if (gt == NULL)
		return NULL;
	gt->resource.name = name;
	gt->stolen = backed;
	gt->in_gart = backed;
	gt->roll = 0;
	/* Ensure this is set for non GEM objects */
	gt->gem.dev = dev;
	ret = allocate_resource(dev_priv->gtt_mem, &gt->resource,
				len, start, end, PAGE_SIZE, NULL, NULL);
	if (ret == 0) {
		gt->offset = gt->resource.start - r->start;
		return gt;
	}
	kfree(gt);
	return NULL;
}

/**
 *	psb_gtt_free_range	-	release GTT address space
 *	@dev: our DRM device
 *	@gt: a mapping created with psb_gtt_alloc_range
 *
 *	Release a resource that was allocated with psb_gtt_alloc_range. If the
 *	object has been pinned by mmap users we clean this up here currently.
 */
void psb_gtt_free_range(struct drm_device *dev, struct gtt_range *gt)
{
	/* Undo the mmap pin if we are destroying the object */
	if (gt->mmapping) {
		psb_gtt_unpin(gt);
		gt->mmapping = 0;
	}
	WARN_ON(gt->in_gart && !gt->stolen);
	release_resource(&gt->resource);
	kfree(gt);
}

static void psb_gtt_alloc(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	init_rwsem(&dev_priv->gtt.sem);
}

void psb_gtt_takedown(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;

	if (dev_priv->gtt_map) {
		iounmap(dev_priv->gtt_map);
		dev_priv->gtt_map = NULL;
	}
	if (dev_priv->gtt_initialized) {
		pci_write_config_word(dev->pdev, PSB_GMCH_CTRL,
				      dev_priv->gmch_ctrl);
		PSB_WVDC32(dev_priv->pge_ctl, PSB_PGETBL_CTL);
		(void) PSB_RVDC32(PSB_PGETBL_CTL);
	}
	if (dev_priv->vram_addr)
		iounmap(dev_priv->gtt_map);
}

int psb_gtt_init(struct drm_device *dev, int resume)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	unsigned gtt_pages;
	unsigned long stolen_size, vram_stolen_size;
	unsigned i, num_pages;
	unsigned pfn_base;
	uint32_t vram_pages;
	uint32_t dvmt_mode = 0;
	struct psb_gtt *pg;

	int ret = 0;
	uint32_t pte;

	mutex_init(&dev_priv->gtt_mutex);

	psb_gtt_alloc(dev);
	pg = &dev_priv->gtt;

	/* Enable the GTT */
	pci_read_config_word(dev->pdev, PSB_GMCH_CTRL, &dev_priv->gmch_ctrl);
	pci_write_config_word(dev->pdev, PSB_GMCH_CTRL,
			      dev_priv->gmch_ctrl | _PSB_GMCH_ENABLED);

	dev_priv->pge_ctl = PSB_RVDC32(PSB_PGETBL_CTL);
	PSB_WVDC32(dev_priv->pge_ctl | _PSB_PGETBL_ENABLED, PSB_PGETBL_CTL);
	(void) PSB_RVDC32(PSB_PGETBL_CTL);

	/* The root resource we allocate address space from */
	dev_priv->gtt_initialized = 1;

	pg->gtt_phys_start = dev_priv->pge_ctl & PAGE_MASK;

	/*
	 *	The video mmu has a hw bug when accessing 0x0D0000000.
	 *	Make gatt start at 0x0e000,0000. This doesn't actually
	 *	matter for us but may do if the video acceleration ever
	 *	gets opened up.
	 */
	pg->mmu_gatt_start = 0xE0000000;

	pg->gtt_start = pci_resource_start(dev->pdev, PSB_GTT_RESOURCE);
	gtt_pages = pci_resource_len(dev->pdev, PSB_GTT_RESOURCE)
								>> PAGE_SHIFT;
	/* CDV doesn't report this. In which case the system has 64 gtt pages */
	if (pg->gtt_start == 0 || gtt_pages == 0) {
		dev_dbg(dev->dev, "GTT PCI BAR not initialized.\n");
		gtt_pages = 64;
		pg->gtt_start = dev_priv->pge_ctl;
	}

	pg->gatt_start = pci_resource_start(dev->pdev, PSB_GATT_RESOURCE);
	pg->gatt_pages = pci_resource_len(dev->pdev, PSB_GATT_RESOURCE)
								>> PAGE_SHIFT;
	dev_priv->gtt_mem = &dev->pdev->resource[PSB_GATT_RESOURCE];

	if (pg->gatt_pages == 0 || pg->gatt_start == 0) {
		static struct resource fudge;	/* Preferably peppermint */
		/* This can occur on CDV systems. Fudge it in this case.
		   We really don't care what imaginary space is being allocated
		   at this point */
		dev_dbg(dev->dev, "GATT PCI BAR not initialized.\n");
		pg->gatt_start = 0x40000000;
		pg->gatt_pages = (128 * 1024 * 1024) >> PAGE_SHIFT;
		/* This is a little confusing but in fact the GTT is providing
		   a view from the GPU into memory and not vice versa. As such
		   this is really allocating space that is not the same as the
		   CPU address space on CDV */
=======
static u32 __iomem *psb_gtt_entry(struct drm_psb_private *pdev, const struct resource *res)
{
	unsigned long offset = res->start - pdev->gtt_mem->start;

	return pdev->gtt_map + (offset >> PAGE_SHIFT);
}

/* Acquires GTT mutex internally. */
void psb_gtt_insert_pages(struct drm_psb_private *pdev, const struct resource *res,
			  struct page **pages)
{
	resource_size_t npages, i;
	u32 __iomem *gtt_slot;
	u32 pte;

	mutex_lock(&pdev->gtt_mutex);

	/* Write our page entries into the GTT itself */

	npages = resource_size(res) >> PAGE_SHIFT;
	gtt_slot = psb_gtt_entry(pdev, res);

	for (i = 0; i < npages; ++i, ++gtt_slot) {
		pte = psb_gtt_mask_pte(page_to_pfn(pages[i]), PSB_MMU_CACHED_MEMORY);
		iowrite32(pte, gtt_slot);
	}

	/* Make sure all the entries are set before we return */
	ioread32(gtt_slot - 1);

	mutex_unlock(&pdev->gtt_mutex);
}

/* Acquires GTT mutex internally. */
void psb_gtt_remove_pages(struct drm_psb_private *pdev, const struct resource *res)
{
	resource_size_t npages, i;
	u32 __iomem *gtt_slot;
	u32 pte;

	mutex_lock(&pdev->gtt_mutex);

	/* Install scratch page for the resource */

	pte = psb_gtt_mask_pte(page_to_pfn(pdev->scratch_page), PSB_MMU_CACHED_MEMORY);

	npages = resource_size(res) >> PAGE_SHIFT;
	gtt_slot = psb_gtt_entry(pdev, res);

	for (i = 0; i < npages; ++i, ++gtt_slot)
		iowrite32(pte, gtt_slot);

	/* Make sure all the entries are set before we return */
	ioread32(gtt_slot - 1);

	mutex_unlock(&pdev->gtt_mutex);
}

static int psb_gtt_enable(struct drm_psb_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->dev;
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	int ret;

	ret = pci_read_config_word(pdev, PSB_GMCH_CTRL, &dev_priv->gmch_ctrl);
	if (ret)
		return pcibios_err_to_errno(ret);
	ret = pci_write_config_word(pdev, PSB_GMCH_CTRL, dev_priv->gmch_ctrl | _PSB_GMCH_ENABLED);
	if (ret)
		return pcibios_err_to_errno(ret);

	dev_priv->pge_ctl = PSB_RVDC32(PSB_PGETBL_CTL);
	PSB_WVDC32(dev_priv->pge_ctl | _PSB_PGETBL_ENABLED, PSB_PGETBL_CTL);

	(void)PSB_RVDC32(PSB_PGETBL_CTL);

	return 0;
}

static void psb_gtt_disable(struct drm_psb_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->dev;
	struct pci_dev *pdev = to_pci_dev(dev->dev);

	pci_write_config_word(pdev, PSB_GMCH_CTRL, dev_priv->gmch_ctrl);
	PSB_WVDC32(dev_priv->pge_ctl, PSB_PGETBL_CTL);

	(void)PSB_RVDC32(PSB_PGETBL_CTL);
}

void psb_gtt_fini(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);

	iounmap(dev_priv->gtt_map);
	psb_gtt_disable(dev_priv);
	mutex_destroy(&dev_priv->gtt_mutex);
}

/* Clear GTT. Use a scratch page to avoid accidents or scribbles. */
static void psb_gtt_clear(struct drm_psb_private *pdev)
{
	resource_size_t pfn_base;
	unsigned long i;
	uint32_t pte;

	pfn_base = page_to_pfn(pdev->scratch_page);
	pte = psb_gtt_mask_pte(pfn_base, PSB_MMU_CACHED_MEMORY);

	for (i = 0; i < pdev->gtt.gtt_pages; ++i)
		iowrite32(pte, pdev->gtt_map + i);

	(void)ioread32(pdev->gtt_map + i - 1);
}

static void psb_gtt_init_ranges(struct drm_psb_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->dev;
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct psb_gtt *pg = &dev_priv->gtt;
	resource_size_t gtt_phys_start, mmu_gatt_start, gtt_start, gtt_pages,
			gatt_start, gatt_pages;
	struct resource *gtt_mem;

	/* The root resource we allocate address space from */
	gtt_phys_start = dev_priv->pge_ctl & PAGE_MASK;

	/*
	 * The video MMU has a HW bug when accessing 0x0d0000000. Make
	 * GATT start at 0x0e0000000. This doesn't actually matter for
	 * us now, but maybe will if the video acceleration ever gets
	 * opened up.
	 */
	mmu_gatt_start = 0xe0000000;

	gtt_start = pci_resource_start(pdev, PSB_GTT_RESOURCE);
	gtt_pages = pci_resource_len(pdev, PSB_GTT_RESOURCE) >> PAGE_SHIFT;

	/* CDV doesn't report this. In which case the system has 64 gtt pages */
	if (!gtt_start || !gtt_pages) {
		dev_dbg(dev->dev, "GTT PCI BAR not initialized.\n");
		gtt_pages = 64;
		gtt_start = dev_priv->pge_ctl;
	}

	gatt_start = pci_resource_start(pdev, PSB_GATT_RESOURCE);
	gatt_pages = pci_resource_len(pdev, PSB_GATT_RESOURCE) >> PAGE_SHIFT;

	if (!gatt_pages || !gatt_start) {
		static struct resource fudge;	/* Preferably peppermint */

		/*
		 * This can occur on CDV systems. Fudge it in this case. We
		 * really don't care what imaginary space is being allocated
		 * at this point.
		 */
		dev_dbg(dev->dev, "GATT PCI BAR not initialized.\n");
		gatt_start = 0x40000000;
		gatt_pages = (128 * 1024 * 1024) >> PAGE_SHIFT;

		/*
		 * This is a little confusing but in fact the GTT is providing
		 * a view from the GPU into memory and not vice versa. As such
		 * this is really allocating space that is not the same as the
		 * CPU address space on CDV.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fudge.start = 0x40000000;
		fudge.end = 0x40000000 + 128 * 1024 * 1024 - 1;
		fudge.name = "fudge";
		fudge.flags = IORESOURCE_MEM;
<<<<<<< HEAD
		dev_priv->gtt_mem = &fudge;
	}

	pci_read_config_dword(dev->pdev, PSB_BSM, &dev_priv->stolen_base);
	vram_stolen_size = pg->gtt_phys_start - dev_priv->stolen_base
								- PAGE_SIZE;

	stolen_size = vram_stolen_size;

	printk(KERN_INFO "Stolen memory information\n");
	printk(KERN_INFO "       base in RAM: 0x%x\n", dev_priv->stolen_base);
	printk(KERN_INFO "       size: %luK, calculated by (GTT RAM base) - (Stolen base), seems wrong\n",
		vram_stolen_size/1024);
	dvmt_mode = (dev_priv->gmch_ctrl >> 4) & 0x7;
	printk(KERN_INFO "      the correct size should be: %dM(dvmt mode=%d)\n",
		(dvmt_mode == 1) ? 1 : (2 << (dvmt_mode - 1)), dvmt_mode);

	if (resume && (gtt_pages != pg->gtt_pages) &&
	    (stolen_size != pg->stolen_size)) {
		dev_err(dev->dev, "GTT resume error.\n");
		ret = -EINVAL;
		goto out_err;
	}

	pg->gtt_pages = gtt_pages;
	pg->stolen_size = stolen_size;
	dev_priv->vram_stolen_size = vram_stolen_size;

	/*
	 *	Map the GTT and the stolen memory area
	 */
	dev_priv->gtt_map = ioremap_nocache(pg->gtt_phys_start,
						gtt_pages << PAGE_SHIFT);
	if (!dev_priv->gtt_map) {
		dev_err(dev->dev, "Failure to map gtt.\n");
		ret = -ENOMEM;
		goto out_err;
	}

	dev_priv->vram_addr = ioremap_wc(dev_priv->stolen_base, stolen_size);
	if (!dev_priv->vram_addr) {
		dev_err(dev->dev, "Failure to map stolen base.\n");
		ret = -ENOMEM;
		goto out_err;
	}

	/*
	 * Insert vram stolen pages into the GTT
	 */

	pfn_base = dev_priv->stolen_base >> PAGE_SHIFT;
	vram_pages = num_pages = vram_stolen_size >> PAGE_SHIFT;
	printk(KERN_INFO"Set up %d stolen pages starting at 0x%08x, GTT offset %dK\n",
		num_pages, pfn_base << PAGE_SHIFT, 0);
	for (i = 0; i < num_pages; ++i) {
		pte = psb_gtt_mask_pte(pfn_base + i, 0);
		iowrite32(pte, dev_priv->gtt_map + i);
	}

	/*
	 * Init rest of GTT to the scratch page to avoid accidents or scribbles
	 */

	pfn_base = page_to_pfn(dev_priv->scratch_page);
	pte = psb_gtt_mask_pte(pfn_base, 0);
	for (; i < gtt_pages; ++i)
		iowrite32(pte, dev_priv->gtt_map + i);

	(void) ioread32(dev_priv->gtt_map + i - 1);
	return 0;

out_err:
	psb_gtt_takedown(dev);
=======

		gtt_mem = &fudge;
	} else {
		gtt_mem = &pdev->resource[PSB_GATT_RESOURCE];
	}

	pg->gtt_phys_start = gtt_phys_start;
	pg->mmu_gatt_start = mmu_gatt_start;
	pg->gtt_start = gtt_start;
	pg->gtt_pages = gtt_pages;
	pg->gatt_start = gatt_start;
	pg->gatt_pages = gatt_pages;
	dev_priv->gtt_mem = gtt_mem;
}

int psb_gtt_init(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct psb_gtt *pg = &dev_priv->gtt;
	int ret;

	mutex_init(&dev_priv->gtt_mutex);

	ret = psb_gtt_enable(dev_priv);
	if (ret)
		goto err_mutex_destroy;

	psb_gtt_init_ranges(dev_priv);

	dev_priv->gtt_map = ioremap(pg->gtt_phys_start, pg->gtt_pages << PAGE_SHIFT);
	if (!dev_priv->gtt_map) {
		dev_err(dev->dev, "Failure to map gtt.\n");
		ret = -ENOMEM;
		goto err_psb_gtt_disable;
	}

	psb_gtt_clear(dev_priv);

	return 0;

err_psb_gtt_disable:
	psb_gtt_disable(dev_priv);
err_mutex_destroy:
	mutex_destroy(&dev_priv->gtt_mutex);
	return ret;
}

int psb_gtt_resume(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct psb_gtt *pg = &dev_priv->gtt;
	unsigned int old_gtt_pages = pg->gtt_pages;
	int ret;

	/* Enable the GTT */
	ret = psb_gtt_enable(dev_priv);
	if (ret)
		return ret;

	psb_gtt_init_ranges(dev_priv);

	if (old_gtt_pages != pg->gtt_pages) {
		dev_err(dev->dev, "GTT resume error.\n");
		ret = -ENODEV;
		goto err_psb_gtt_disable;
	}

	psb_gtt_clear(dev_priv);

err_psb_gtt_disable:
	psb_gtt_disable(dev_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
