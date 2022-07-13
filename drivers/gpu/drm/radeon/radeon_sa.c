/*
 * Copyright 2011 Red Hat Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors:
 *    Jerome Glisse <glisse@freedesktop.org>
 */
<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"
=======
/* Algorithm:
 *
 * We store the last allocated bo in "hole", we always try to allocate
 * after the last allocated bo. Principle is that in a linear GPU ring
 * progression was is after last is the oldest bo we allocated and thus
 * the first one that should no longer be in use by the GPU.
 *
 * If it's not the case we skip over the bo after last to the closest
 * done bo if such one exist. If none exist and we are not asked to
 * block we report failure to allocate.
 *
 * If we are asked to block we wait on all the oldest fence of all
 * rings. We just wait for any of those fence to complete.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "radeon.h"

int radeon_sa_bo_manager_init(struct radeon_device *rdev,
			      struct radeon_sa_manager *sa_manager,
<<<<<<< HEAD
			      unsigned size, u32 domain)
{
	int r;

	sa_manager->bo = NULL;
	sa_manager->size = size;
	sa_manager->domain = domain;
	INIT_LIST_HEAD(&sa_manager->sa_bo);

	r = radeon_bo_create(rdev, size, RADEON_GPU_PAGE_SIZE, true,
			     RADEON_GEM_DOMAIN_CPU, &sa_manager->bo);
=======
			      unsigned int size, u32 sa_align, u32 domain,
			      u32 flags)
{
	int r;

	r = radeon_bo_create(rdev, size, RADEON_GPU_PAGE_SIZE, true,
			     domain, flags, NULL, NULL, &sa_manager->bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r) {
		dev_err(rdev->dev, "(%d) failed to allocate bo for manager\n", r);
		return r;
	}

<<<<<<< HEAD
=======
	sa_manager->domain = domain;

	drm_suballoc_manager_init(&sa_manager->base, size, sa_align);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return r;
}

void radeon_sa_bo_manager_fini(struct radeon_device *rdev,
			       struct radeon_sa_manager *sa_manager)
{
<<<<<<< HEAD
	struct radeon_sa_bo *sa_bo, *tmp;

	if (!list_empty(&sa_manager->sa_bo)) {
		dev_err(rdev->dev, "sa_manager is not empty, clearing anyway\n");
	}
	list_for_each_entry_safe(sa_bo, tmp, &sa_manager->sa_bo, list) {
		list_del_init(&sa_bo->list);
	}
	radeon_bo_unref(&sa_manager->bo);
	sa_manager->size = 0;
=======
	drm_suballoc_manager_fini(&sa_manager->base);
	radeon_bo_unref(&sa_manager->bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int radeon_sa_bo_manager_start(struct radeon_device *rdev,
			       struct radeon_sa_manager *sa_manager)
{
	int r;

	if (sa_manager->bo == NULL) {
		dev_err(rdev->dev, "no bo for sa manager\n");
		return -EINVAL;
	}

	/* map the buffer */
	r = radeon_bo_reserve(sa_manager->bo, false);
	if (r) {
		dev_err(rdev->dev, "(%d) failed to reserve manager bo\n", r);
		return r;
	}
	r = radeon_bo_pin(sa_manager->bo, sa_manager->domain, &sa_manager->gpu_addr);
	if (r) {
		radeon_bo_unreserve(sa_manager->bo);
		dev_err(rdev->dev, "(%d) failed to pin manager bo\n", r);
		return r;
	}
	r = radeon_bo_kmap(sa_manager->bo, &sa_manager->cpu_ptr);
	radeon_bo_unreserve(sa_manager->bo);
	return r;
}

int radeon_sa_bo_manager_suspend(struct radeon_device *rdev,
				 struct radeon_sa_manager *sa_manager)
{
	int r;

	if (sa_manager->bo == NULL) {
		dev_err(rdev->dev, "no bo for sa manager\n");
		return -EINVAL;
	}

	r = radeon_bo_reserve(sa_manager->bo, false);
	if (!r) {
		radeon_bo_kunmap(sa_manager->bo);
		radeon_bo_unpin(sa_manager->bo);
		radeon_bo_unreserve(sa_manager->bo);
	}
	return r;
}

<<<<<<< HEAD
/*
 * Principe is simple, we keep a list of sub allocation in offset
 * order (first entry has offset == 0, last entry has the highest
 * offset).
 *
 * When allocating new object we first check if there is room at
 * the end total_size - (last_object_offset + last_object_size) >=
 * alloc_size. If so we allocate new object there.
 *
 * When there is not enough room at the end, we start waiting for
 * each sub object until we reach object_offset+object_size >=
 * alloc_size, this object then become the sub object we return.
 *
 * Alignment can't be bigger than page size
 */
int radeon_sa_bo_new(struct radeon_device *rdev,
		     struct radeon_sa_manager *sa_manager,
		     struct radeon_sa_bo *sa_bo,
		     unsigned size, unsigned align)
{
	struct radeon_sa_bo *tmp;
	struct list_head *head;
	unsigned offset = 0, wasted = 0;

	BUG_ON(align > RADEON_GPU_PAGE_SIZE);
	BUG_ON(size > sa_manager->size);

	/* no one ? */
	head = sa_manager->sa_bo.prev;
	if (list_empty(&sa_manager->sa_bo)) {
		goto out;
	}

	/* look for a hole big enough */
	offset = 0;
	list_for_each_entry(tmp, &sa_manager->sa_bo, list) {
		/* room before this object ? */
		if ((tmp->offset - offset) >= size) {
			head = tmp->list.prev;
			goto out;
		}
		offset = tmp->offset + tmp->size;
		wasted = offset % align;
		if (wasted) {
			wasted = align - wasted;
		}
		offset += wasted;
	}
	/* room at the end ? */
	head = sa_manager->sa_bo.prev;
	tmp = list_entry(head, struct radeon_sa_bo, list);
	offset = tmp->offset + tmp->size;
	wasted = offset % align;
	if (wasted) {
		wasted = align - wasted;
	}
	offset += wasted;
	if ((sa_manager->size - offset) < size) {
		/* failed to find somethings big enough */
		return -ENOMEM;
	}

out:
	sa_bo->manager = sa_manager;
	sa_bo->offset = offset;
	sa_bo->size = size;
	list_add(&sa_bo->list, head);
	return 0;
}

void radeon_sa_bo_free(struct radeon_device *rdev, struct radeon_sa_bo *sa_bo)
{
	list_del_init(&sa_bo->list);
}
=======
int radeon_sa_bo_new(struct radeon_sa_manager *sa_manager,
		     struct drm_suballoc **sa_bo,
		     unsigned int size, unsigned int align)
{
	struct drm_suballoc *sa = drm_suballoc_new(&sa_manager->base, size,
						   GFP_KERNEL, false, align);

	if (IS_ERR(sa)) {
		*sa_bo = NULL;
		return PTR_ERR(sa);
	}

	*sa_bo = sa;
	return 0;
}

void radeon_sa_bo_free(struct drm_suballoc **sa_bo,
		       struct radeon_fence *fence)
{
	if (sa_bo == NULL || *sa_bo == NULL) {
		return;
	}

	if (fence)
		drm_suballoc_free(*sa_bo, &fence->base);
	else
		drm_suballoc_free(*sa_bo, NULL);

	*sa_bo = NULL;
}

#if defined(CONFIG_DEBUG_FS)
void radeon_sa_bo_dump_debug_info(struct radeon_sa_manager *sa_manager,
				  struct seq_file *m)
{
	struct drm_printer p = drm_seq_file_printer(m);

	drm_suballoc_dump_debug_info(&sa_manager->base, &p, sa_manager->gpu_addr);
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
