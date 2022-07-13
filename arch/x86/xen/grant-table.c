<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0 OR MIT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 * grant_table.c
 * x86 specific part
 *
 * Granting foreign access to our memory reservation.
 *
 * Copyright (c) 2005-2006, Christopher Clark
 * Copyright (c) 2004-2005, K A Fraser
 * Copyright (c) 2008 Isaku Yamahata <yamahata at valinux co jp>
 *                    VA Linux Systems Japan. Split out x86 specific part.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/vmalloc.h>

#include <xen/interface/xen.h>
#include <xen/page.h>
#include <xen/grant_table.h>
<<<<<<< HEAD

#include <asm/pgtable.h>

static int map_pte_fn(pte_t *pte, struct page *pmd_page,
		      unsigned long addr, void *data)
{
	unsigned long **frames = (unsigned long **)data;

	set_pte_at(&init_mm, addr, pte, mfn_pte((*frames)[0], PAGE_KERNEL));
	(*frames)++;
	return 0;
}

/*
 * This function is used to map shared frames to store grant status. It is
 * different from map_pte_fn above, the frames type here is uint64_t.
 */
static int map_pte_fn_status(pte_t *pte, struct page *pmd_page,
			     unsigned long addr, void *data)
{
	uint64_t **frames = (uint64_t **)data;

	set_pte_at(&init_mm, addr, pte, mfn_pte((*frames)[0], PAGE_KERNEL));
	(*frames)++;
	return 0;
}

static int unmap_pte_fn(pte_t *pte, struct page *pmd_page,
			unsigned long addr, void *data)
{

	set_pte_at(&init_mm, addr, pte, __pte(0));
	return 0;
}
=======
#include <xen/xen.h>


static struct gnttab_vm_area {
	struct vm_struct *area;
	pte_t **ptes;
	int idx;
} gnttab_shared_vm_area, gnttab_status_vm_area;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int arch_gnttab_map_shared(unsigned long *frames, unsigned long nr_gframes,
			   unsigned long max_nr_gframes,
			   void **__shared)
{
<<<<<<< HEAD
	int rc;
	void *shared = *__shared;

	if (shared == NULL) {
		struct vm_struct *area =
			alloc_vm_area(PAGE_SIZE * max_nr_gframes, NULL);
		BUG_ON(area == NULL);
		shared = area->addr;
		*__shared = shared;
	}

	rc = apply_to_page_range(&init_mm, (unsigned long)shared,
				 PAGE_SIZE * nr_gframes,
				 map_pte_fn, &frames);
	return rc;
=======
	void *shared = *__shared;
	unsigned long addr;
	unsigned long i;

	if (shared == NULL)
		*__shared = shared = gnttab_shared_vm_area.area->addr;

	addr = (unsigned long)shared;

	for (i = 0; i < nr_gframes; i++) {
		set_pte_at(&init_mm, addr, gnttab_shared_vm_area.ptes[i],
			   mfn_pte(frames[i], PAGE_KERNEL));
		addr += PAGE_SIZE;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int arch_gnttab_map_status(uint64_t *frames, unsigned long nr_gframes,
			   unsigned long max_nr_gframes,
			   grant_status_t **__shared)
{
<<<<<<< HEAD
	int rc;
	grant_status_t *shared = *__shared;

	if (shared == NULL) {
		/* No need to pass in PTE as we are going to do it
		 * in apply_to_page_range anyhow. */
		struct vm_struct *area =
			alloc_vm_area(PAGE_SIZE * max_nr_gframes, NULL);
		BUG_ON(area == NULL);
		shared = area->addr;
		*__shared = shared;
	}

	rc = apply_to_page_range(&init_mm, (unsigned long)shared,
				 PAGE_SIZE * nr_gframes,
				 map_pte_fn_status, &frames);
	return rc;
=======
	grant_status_t *shared = *__shared;
	unsigned long addr;
	unsigned long i;

	if (shared == NULL)
		*__shared = shared = gnttab_status_vm_area.area->addr;

	addr = (unsigned long)shared;

	for (i = 0; i < nr_gframes; i++) {
		set_pte_at(&init_mm, addr, gnttab_status_vm_area.ptes[i],
			   mfn_pte(frames[i], PAGE_KERNEL));
		addr += PAGE_SIZE;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_gnttab_unmap(void *shared, unsigned long nr_gframes)
{
<<<<<<< HEAD
	apply_to_page_range(&init_mm, (unsigned long)shared,
			    PAGE_SIZE * nr_gframes, unmap_pte_fn, NULL);
}
=======
	pte_t **ptes;
	unsigned long addr;
	unsigned long i;

	if (shared == gnttab_status_vm_area.area->addr)
		ptes = gnttab_status_vm_area.ptes;
	else
		ptes = gnttab_shared_vm_area.ptes;

	addr = (unsigned long)shared;

	for (i = 0; i < nr_gframes; i++) {
		set_pte_at(&init_mm, addr, ptes[i], __pte(0));
		addr += PAGE_SIZE;
	}
}

static int gnttab_apply(pte_t *pte, unsigned long addr, void *data)
{
	struct gnttab_vm_area *area = data;

	area->ptes[area->idx++] = pte;
	return 0;
}

static int arch_gnttab_valloc(struct gnttab_vm_area *area, unsigned nr_frames)
{
	area->ptes = kmalloc_array(nr_frames, sizeof(*area->ptes), GFP_KERNEL);
	if (area->ptes == NULL)
		return -ENOMEM;
	area->area = get_vm_area(PAGE_SIZE * nr_frames, VM_IOREMAP);
	if (!area->area)
		goto out_free_ptes;
	if (apply_to_page_range(&init_mm, (unsigned long)area->area->addr,
			PAGE_SIZE * nr_frames, gnttab_apply, area))
		goto out_free_vm_area;
	return 0;
out_free_vm_area:
	free_vm_area(area->area);
out_free_ptes:
	kfree(area->ptes);
	return -ENOMEM;
}

static void arch_gnttab_vfree(struct gnttab_vm_area *area)
{
	free_vm_area(area->area);
	kfree(area->ptes);
}

int arch_gnttab_init(unsigned long nr_shared, unsigned long nr_status)
{
	int ret;

	if (!xen_pv_domain())
		return 0;

	ret = arch_gnttab_valloc(&gnttab_shared_vm_area, nr_shared);
	if (ret < 0)
		return ret;

	/*
	 * Always allocate the space for the status frames in case
	 * we're migrated to a host with V2 support.
	 */
	ret = arch_gnttab_valloc(&gnttab_status_vm_area, nr_status);
	if (ret < 0)
		goto err;

	return 0;
err:
	arch_gnttab_vfree(&gnttab_shared_vm_area);
	return -ENOMEM;
}

#ifdef CONFIG_XEN_PVH
#include <xen/events.h>
#include <xen/xen-ops.h>
static int __init xen_pvh_gnttab_setup(void)
{
	if (!xen_pvh_domain())
		return -ENODEV;

	xen_auto_xlat_grant_frames.count = gnttab_max_grant_frames();

	return xen_xlate_map_ballooned_pages(&xen_auto_xlat_grant_frames.pfn,
					     &xen_auto_xlat_grant_frames.vaddr,
					     xen_auto_xlat_grant_frames.count);
}
/* Call it _before_ __gnttab_init as we need to initialize the
 * xen_auto_xlat_grant_frames first. */
core_initcall(xen_pvh_gnttab_setup);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
