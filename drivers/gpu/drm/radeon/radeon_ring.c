/*
 * Copyright 2008 Advanced Micro Devices, Inc.
 * Copyright 2008 Red Hat Inc.
 * Copyright 2009 Jerome Glisse.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Dave Airlie
 *          Alex Deucher
 *          Jerome Glisse
<<<<<<< HEAD
 */
#include <linux/seq_file.h>
#include <linux/slab.h>
#include "drmP.h"
#include "radeon_drm.h"
#include "radeon_reg.h"
#include "radeon.h"
#include "atom.h"

int radeon_debugfs_ib_init(struct radeon_device *rdev);
int radeon_debugfs_ring_init(struct radeon_device *rdev);

u32 radeon_get_ib_value(struct radeon_cs_parser *p, int idx)
{
	struct radeon_cs_chunk *ibc = &p->chunks[p->chunk_ib_idx];
	u32 pg_idx, pg_offset;
	u32 idx_value = 0;
	int new_page;

	pg_idx = (idx * 4) / PAGE_SIZE;
	pg_offset = (idx * 4) % PAGE_SIZE;

	if (ibc->kpage_idx[0] == pg_idx)
		return ibc->kpage[0][pg_offset/4];
	if (ibc->kpage_idx[1] == pg_idx)
		return ibc->kpage[1][pg_offset/4];

	new_page = radeon_cs_update_pages(p, pg_idx);
	if (new_page < 0) {
		p->parser_error = new_page;
		return 0;
	}

	idx_value = ibc->kpage[new_page][pg_offset/4];
	return idx_value;
}

void radeon_ring_write(struct radeon_ring *ring, uint32_t v)
{
#if DRM_DEBUG_CODE
	if (ring->count_dw <= 0) {
		DRM_ERROR("radeon: writting more dword to ring than expected !\n");
	}
#endif
	ring->ring[ring->wptr++] = v;
	ring->wptr &= ring->ptr_mask;
	ring->count_dw--;
	ring->ring_free_dw--;
}

/*
 * IB.
 */
bool radeon_ib_try_free(struct radeon_device *rdev, struct radeon_ib *ib)
{
	bool done = false;

	/* only free ib which have been emited */
	if (ib->fence && ib->fence->emitted) {
		if (radeon_fence_signaled(ib->fence)) {
			radeon_fence_unref(&ib->fence);
			radeon_sa_bo_free(rdev, &ib->sa_bo);
			done = true;
		}
	}
	return done;
}

int radeon_ib_get(struct radeon_device *rdev, int ring,
		  struct radeon_ib **ib, unsigned size)
{
	struct radeon_fence *fence;
	unsigned cretry = 0;
	int r = 0, i, idx;

	*ib = NULL;
	/* align size on 256 bytes */
	size = ALIGN(size, 256);

	r = radeon_fence_create(rdev, &fence, ring);
	if (r) {
		dev_err(rdev->dev, "failed to create fence for new IB\n");
		return r;
	}

	radeon_mutex_lock(&rdev->ib_pool.mutex);
	idx = rdev->ib_pool.head_id;
retry:
	if (cretry > 5) {
		dev_err(rdev->dev, "failed to get an ib after 5 retry\n");
		radeon_mutex_unlock(&rdev->ib_pool.mutex);
		radeon_fence_unref(&fence);
		return -ENOMEM;
	}
	cretry++;
	for (i = 0; i < RADEON_IB_POOL_SIZE; i++) {
		radeon_ib_try_free(rdev, &rdev->ib_pool.ibs[idx]);
		if (rdev->ib_pool.ibs[idx].fence == NULL) {
			r = radeon_sa_bo_new(rdev, &rdev->ib_pool.sa_manager,
					     &rdev->ib_pool.ibs[idx].sa_bo,
					     size, 256);
			if (!r) {
				*ib = &rdev->ib_pool.ibs[idx];
				(*ib)->ptr = rdev->ib_pool.sa_manager.cpu_ptr;
				(*ib)->ptr += ((*ib)->sa_bo.offset >> 2);
				(*ib)->gpu_addr = rdev->ib_pool.sa_manager.gpu_addr;
				(*ib)->gpu_addr += (*ib)->sa_bo.offset;
				(*ib)->fence = fence;
				(*ib)->vm_id = 0;
				(*ib)->is_const_ib = false;
				/* ib are most likely to be allocated in a ring fashion
				 * thus rdev->ib_pool.head_id should be the id of the
				 * oldest ib
				 */
				rdev->ib_pool.head_id = (1 + idx);
				rdev->ib_pool.head_id &= (RADEON_IB_POOL_SIZE - 1);
				radeon_mutex_unlock(&rdev->ib_pool.mutex);
				return 0;
			}
		}
		idx = (idx + 1) & (RADEON_IB_POOL_SIZE - 1);
	}
	/* this should be rare event, ie all ib scheduled none signaled yet.
	 */
	for (i = 0; i < RADEON_IB_POOL_SIZE; i++) {
		if (rdev->ib_pool.ibs[idx].fence && rdev->ib_pool.ibs[idx].fence->emitted) {
			r = radeon_fence_wait(rdev->ib_pool.ibs[idx].fence, false);
			if (!r) {
				goto retry;
			}
			/* an error happened */
			break;
		}
		idx = (idx + 1) & (RADEON_IB_POOL_SIZE - 1);
	}
	radeon_mutex_unlock(&rdev->ib_pool.mutex);
	radeon_fence_unref(&fence);
	return r;
}

void radeon_ib_free(struct radeon_device *rdev, struct radeon_ib **ib)
{
	struct radeon_ib *tmp = *ib;

	*ib = NULL;
	if (tmp == NULL) {
		return;
	}
	radeon_mutex_lock(&rdev->ib_pool.mutex);
	if (tmp->fence && !tmp->fence->emitted) {
		radeon_sa_bo_free(rdev, &tmp->sa_bo);
		radeon_fence_unref(&tmp->fence);
	}
	radeon_mutex_unlock(&rdev->ib_pool.mutex);
}

int radeon_ib_schedule(struct radeon_device *rdev, struct radeon_ib *ib)
{
	struct radeon_ring *ring = &rdev->ring[ib->fence->ring];
	int r = 0;

	if (!ib->length_dw || !ring->ready) {
		/* TODO: Nothings in the ib we should report. */
		DRM_ERROR("radeon: couldn't schedule IB(%u).\n", ib->idx);
		return -EINVAL;
	}

	/* 64 dwords should be enough for fence too */
	r = radeon_ring_lock(rdev, ring, 64);
	if (r) {
		DRM_ERROR("radeon: scheduling IB failed (%d).\n", r);
		return r;
	}
	radeon_ring_ib_execute(rdev, ib->fence->ring, ib);
	radeon_fence_emit(rdev, ib->fence);
	radeon_ring_unlock_commit(rdev, ring);
	return 0;
}

int radeon_ib_pool_init(struct radeon_device *rdev)
{
	struct radeon_sa_manager tmp;
	int i, r;

	r = radeon_sa_bo_manager_init(rdev, &tmp,
				      RADEON_IB_POOL_SIZE*64*1024,
				      RADEON_GEM_DOMAIN_GTT);
	if (r) {
		return r;
	}

	radeon_mutex_lock(&rdev->ib_pool.mutex);
	if (rdev->ib_pool.ready) {
		radeon_mutex_unlock(&rdev->ib_pool.mutex);
		radeon_sa_bo_manager_fini(rdev, &tmp);
		return 0;
	}

	rdev->ib_pool.sa_manager = tmp;
	INIT_LIST_HEAD(&rdev->ib_pool.sa_manager.sa_bo);
	for (i = 0; i < RADEON_IB_POOL_SIZE; i++) {
		rdev->ib_pool.ibs[i].fence = NULL;
		rdev->ib_pool.ibs[i].idx = i;
		rdev->ib_pool.ibs[i].length_dw = 0;
		INIT_LIST_HEAD(&rdev->ib_pool.ibs[i].sa_bo.list);
	}
	rdev->ib_pool.head_id = 0;
	rdev->ib_pool.ready = true;
	DRM_INFO("radeon: ib pool ready.\n");

	if (radeon_debugfs_ib_init(rdev)) {
		DRM_ERROR("Failed to register debugfs file for IB !\n");
	}
	if (radeon_debugfs_ring_init(rdev)) {
		DRM_ERROR("Failed to register debugfs file for rings !\n");
	}
	radeon_mutex_unlock(&rdev->ib_pool.mutex);
	return 0;
}

void radeon_ib_pool_fini(struct radeon_device *rdev)
{
	unsigned i;

	radeon_mutex_lock(&rdev->ib_pool.mutex);
	if (rdev->ib_pool.ready) {
		for (i = 0; i < RADEON_IB_POOL_SIZE; i++) {
			radeon_sa_bo_free(rdev, &rdev->ib_pool.ibs[i].sa_bo);
			radeon_fence_unref(&rdev->ib_pool.ibs[i].fence);
		}
		radeon_sa_bo_manager_fini(rdev, &rdev->ib_pool.sa_manager);
		rdev->ib_pool.ready = false;
	}
	radeon_mutex_unlock(&rdev->ib_pool.mutex);
}

int radeon_ib_pool_start(struct radeon_device *rdev)
{
	return radeon_sa_bo_manager_start(rdev, &rdev->ib_pool.sa_manager);
}

int radeon_ib_pool_suspend(struct radeon_device *rdev)
{
	return radeon_sa_bo_manager_suspend(rdev, &rdev->ib_pool.sa_manager);
}

/*
 * Ring.
 */
int radeon_ring_index(struct radeon_device *rdev, struct radeon_ring *ring)
{
	/* r1xx-r5xx only has CP ring */
	if (rdev->family < CHIP_R600)
		return RADEON_RING_TYPE_GFX_INDEX;

	if (rdev->family >= CHIP_CAYMAN) {
		if (ring == &rdev->ring[CAYMAN_RING_TYPE_CP1_INDEX])
			return CAYMAN_RING_TYPE_CP1_INDEX;
		else if (ring == &rdev->ring[CAYMAN_RING_TYPE_CP2_INDEX])
			return CAYMAN_RING_TYPE_CP2_INDEX;
	}
	return RADEON_RING_TYPE_GFX_INDEX;
}

void radeon_ring_free_size(struct radeon_device *rdev, struct radeon_ring *ring)
{
	u32 rptr;

	if (rdev->wb.enabled)
		rptr = le32_to_cpu(rdev->wb.wb[ring->rptr_offs/4]);
	else
		rptr = RREG32(ring->rptr_reg);
	ring->rptr = (rptr & ring->ptr_reg_mask) >> ring->ptr_reg_shift;
	/* This works because ring_size is a power of 2 */
	ring->ring_free_dw = (ring->rptr + (ring->ring_size / 4));
	ring->ring_free_dw -= ring->wptr;
	ring->ring_free_dw &= ring->ptr_mask;
	if (!ring->ring_free_dw) {
		ring->ring_free_dw = ring->ring_size / 4;
	}
}


=======
 *          Christian König
 */

#include <drm/drm_device.h>
#include <drm/drm_file.h>

#include "radeon.h"

/*
 * Rings
 * Most engines on the GPU are fed via ring buffers.  Ring
 * buffers are areas of GPU accessible memory that the host
 * writes commands into and the GPU reads commands out of.
 * There is a rptr (read pointer) that determines where the
 * GPU is currently reading, and a wptr (write pointer)
 * which determines where the host has written.  When the
 * pointers are equal, the ring is idle.  When the host
 * writes commands to the ring buffer, it increments the
 * wptr.  The GPU then starts fetching commands and executes
 * them until the pointers are equal again.
 */
static void radeon_debugfs_ring_init(struct radeon_device *rdev, struct radeon_ring *ring);

/**
 * radeon_ring_supports_scratch_reg - check if the ring supports
 * writing to scratch registers
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 *
 * Check if a specific ring supports writing to scratch registers (all asics).
 * Returns true if the ring supports writing to scratch regs, false if not.
 */
bool radeon_ring_supports_scratch_reg(struct radeon_device *rdev,
				      struct radeon_ring *ring)
{
	switch (ring->idx) {
	case RADEON_RING_TYPE_GFX_INDEX:
	case CAYMAN_RING_TYPE_CP1_INDEX:
	case CAYMAN_RING_TYPE_CP2_INDEX:
		return true;
	default:
		return false;
	}
}

/**
 * radeon_ring_free_size - update the free size
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 *
 * Update the free dw slots in the ring buffer (all asics).
 */
void radeon_ring_free_size(struct radeon_device *rdev, struct radeon_ring *ring)
{
	uint32_t rptr = radeon_ring_get_rptr(rdev, ring);

	/* This works because ring_size is a power of 2 */
	ring->ring_free_dw = rptr + (ring->ring_size / 4);
	ring->ring_free_dw -= ring->wptr;
	ring->ring_free_dw &= ring->ptr_mask;
	if (!ring->ring_free_dw) {
		/* this is an empty ring */
		ring->ring_free_dw = ring->ring_size / 4;
		/*  update lockup info to avoid false positive */
		radeon_ring_lockup_update(rdev, ring);
	}
}

/**
 * radeon_ring_alloc - allocate space on the ring buffer
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 * @ndw: number of dwords to allocate in the ring buffer
 *
 * Allocate @ndw dwords in the ring buffer (all asics).
 * Returns 0 on success, error on failure.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int radeon_ring_alloc(struct radeon_device *rdev, struct radeon_ring *ring, unsigned ndw)
{
	int r;

	/* make sure we aren't trying to allocate more space than there is on the ring */
	if (ndw > (ring->ring_size / 4))
		return -ENOMEM;
	/* Align requested size with padding so unlock_commit can
	 * pad safely */
<<<<<<< HEAD
=======
	radeon_ring_free_size(rdev, ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ndw = (ndw + ring->align_mask) & ~ring->align_mask;
	while (ndw > (ring->ring_free_dw - 1)) {
		radeon_ring_free_size(rdev, ring);
		if (ndw < ring->ring_free_dw) {
			break;
		}
<<<<<<< HEAD
		r = radeon_fence_wait_next(rdev, radeon_ring_index(rdev, ring));
=======
		r = radeon_fence_wait_next(rdev, ring->idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (r)
			return r;
	}
	ring->count_dw = ndw;
	ring->wptr_old = ring->wptr;
	return 0;
}

<<<<<<< HEAD
=======
/**
 * radeon_ring_lock - lock the ring and allocate space on it
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 * @ndw: number of dwords to allocate in the ring buffer
 *
 * Lock the ring and allocate @ndw dwords in the ring buffer
 * (all asics).
 * Returns 0 on success, error on failure.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int radeon_ring_lock(struct radeon_device *rdev, struct radeon_ring *ring, unsigned ndw)
{
	int r;

<<<<<<< HEAD
	mutex_lock(&ring->mutex);
	r = radeon_ring_alloc(rdev, ring, ndw);
	if (r) {
		mutex_unlock(&ring->mutex);
=======
	mutex_lock(&rdev->ring_lock);
	r = radeon_ring_alloc(rdev, ring, ndw);
	if (r) {
		mutex_unlock(&rdev->ring_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return r;
	}
	return 0;
}

<<<<<<< HEAD
void radeon_ring_commit(struct radeon_device *rdev, struct radeon_ring *ring)
{
	unsigned count_dw_pad;
	unsigned i;

	/* We pad to match fetch size */
	count_dw_pad = (ring->align_mask + 1) -
		       (ring->wptr & ring->align_mask);
	for (i = 0; i < count_dw_pad; i++) {
		radeon_ring_write(ring, ring->nop);
	}
	DRM_MEMORYBARRIER();
	WREG32(ring->wptr_reg, (ring->wptr << ring->ptr_reg_shift) & ring->ptr_reg_mask);
	(void)RREG32(ring->wptr_reg);
}

void radeon_ring_unlock_commit(struct radeon_device *rdev, struct radeon_ring *ring)
{
	radeon_ring_commit(rdev, ring);
	mutex_unlock(&ring->mutex);
}

void radeon_ring_unlock_undo(struct radeon_device *rdev, struct radeon_ring *ring)
{
	ring->wptr = ring->wptr_old;
	mutex_unlock(&ring->mutex);
}

int radeon_ring_init(struct radeon_device *rdev, struct radeon_ring *ring, unsigned ring_size,
		     unsigned rptr_offs, unsigned rptr_reg, unsigned wptr_reg,
		     u32 ptr_reg_shift, u32 ptr_reg_mask, u32 nop)
=======
/**
 * radeon_ring_commit - tell the GPU to execute the new
 * commands on the ring buffer
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 * @hdp_flush: Whether or not to perform an HDP cache flush
 *
 * Update the wptr (write pointer) to tell the GPU to
 * execute new commands on the ring buffer (all asics).
 */
void radeon_ring_commit(struct radeon_device *rdev, struct radeon_ring *ring,
			bool hdp_flush)
{
	/* If we are emitting the HDP flush via the ring buffer, we need to
	 * do it before padding.
	 */
	if (hdp_flush && rdev->asic->ring[ring->idx]->hdp_flush)
		rdev->asic->ring[ring->idx]->hdp_flush(rdev, ring);
	/* We pad to match fetch size */
	while (ring->wptr & ring->align_mask) {
		radeon_ring_write(ring, ring->nop);
	}
	mb();
	/* If we are emitting the HDP flush via MMIO, we need to do it after
	 * all CPU writes to VRAM finished.
	 */
	if (hdp_flush && rdev->asic->mmio_hdp_flush)
		rdev->asic->mmio_hdp_flush(rdev);
	radeon_ring_set_wptr(rdev, ring);
}

/**
 * radeon_ring_unlock_commit - tell the GPU to execute the new
 * commands on the ring buffer and unlock it
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 * @hdp_flush: Whether or not to perform an HDP cache flush
 *
 * Call radeon_ring_commit() then unlock the ring (all asics).
 */
void radeon_ring_unlock_commit(struct radeon_device *rdev, struct radeon_ring *ring,
			       bool hdp_flush)
{
	radeon_ring_commit(rdev, ring, hdp_flush);
	mutex_unlock(&rdev->ring_lock);
}

/**
 * radeon_ring_undo - reset the wptr
 *
 * @ring: radeon_ring structure holding ring information
 *
 * Reset the driver's copy of the wptr (all asics).
 */
void radeon_ring_undo(struct radeon_ring *ring)
{
	ring->wptr = ring->wptr_old;
}

/**
 * radeon_ring_unlock_undo - reset the wptr and unlock the ring
 *
 * @rdev:       radeon device structure
 * @ring: radeon_ring structure holding ring information
 *
 * Call radeon_ring_undo() then unlock the ring (all asics).
 */
void radeon_ring_unlock_undo(struct radeon_device *rdev, struct radeon_ring *ring)
{
	radeon_ring_undo(ring);
	mutex_unlock(&rdev->ring_lock);
}

/**
 * radeon_ring_lockup_update - update lockup variables
 *
 * @rdev:       radeon device structure
 * @ring: radeon_ring structure holding ring information
 *
 * Update the last rptr value and timestamp (all asics).
 */
void radeon_ring_lockup_update(struct radeon_device *rdev,
			       struct radeon_ring *ring)
{
	atomic_set(&ring->last_rptr, radeon_ring_get_rptr(rdev, ring));
	atomic64_set(&ring->last_activity, jiffies_64);
}

/**
 * radeon_ring_test_lockup() - check if ring is lockedup by recording information
 * @rdev:       radeon device structure
 * @ring:       radeon_ring structure holding ring information
 *
 */
bool radeon_ring_test_lockup(struct radeon_device *rdev, struct radeon_ring *ring)
{
	uint32_t rptr = radeon_ring_get_rptr(rdev, ring);
	uint64_t last = atomic64_read(&ring->last_activity);
	uint64_t elapsed;

	if (rptr != atomic_read(&ring->last_rptr)) {
		/* ring is still working, no lockup */
		radeon_ring_lockup_update(rdev, ring);
		return false;
	}

	elapsed = jiffies_to_msecs(jiffies_64 - last);
	if (radeon_lockup_timeout && elapsed >= radeon_lockup_timeout) {
		dev_err(rdev->dev, "ring %d stalled for more than %llumsec\n",
			ring->idx, elapsed);
		return true;
	}
	/* give a chance to the GPU ... */
	return false;
}

/**
 * radeon_ring_backup - Back up the content of a ring
 *
 * @rdev: radeon_device pointer
 * @ring: the ring we want to back up
 * @data: placeholder for returned commit data
 *
 * Saves all unprocessed commits from a ring, returns the number of dwords saved.
 */
unsigned radeon_ring_backup(struct radeon_device *rdev, struct radeon_ring *ring,
			    uint32_t **data)
{
	unsigned size, ptr, i;

	/* just in case lock the ring */
	mutex_lock(&rdev->ring_lock);
	*data = NULL;

	if (ring->ring_obj == NULL) {
		mutex_unlock(&rdev->ring_lock);
		return 0;
	}

	/* it doesn't make sense to save anything if all fences are signaled */
	if (!radeon_fence_count_emitted(rdev, ring->idx)) {
		mutex_unlock(&rdev->ring_lock);
		return 0;
	}

	/* calculate the number of dw on the ring */
	if (ring->rptr_save_reg)
		ptr = RREG32(ring->rptr_save_reg);
	else if (rdev->wb.enabled)
		ptr = le32_to_cpu(*ring->next_rptr_cpu_addr);
	else {
		/* no way to read back the next rptr */
		mutex_unlock(&rdev->ring_lock);
		return 0;
	}

	size = ring->wptr + (ring->ring_size / 4);
	size -= ptr;
	size &= ring->ptr_mask;
	if (size == 0) {
		mutex_unlock(&rdev->ring_lock);
		return 0;
	}

	/* and then save the content of the ring */
	*data = kvmalloc_array(size, sizeof(uint32_t), GFP_KERNEL);
	if (!*data) {
		mutex_unlock(&rdev->ring_lock);
		return 0;
	}
	for (i = 0; i < size; ++i) {
		(*data)[i] = ring->ring[ptr++];
		ptr &= ring->ptr_mask;
	}

	mutex_unlock(&rdev->ring_lock);
	return size;
}

/**
 * radeon_ring_restore - append saved commands to the ring again
 *
 * @rdev: radeon_device pointer
 * @ring: ring to append commands to
 * @size: number of dwords we want to write
 * @data: saved commands
 *
 * Allocates space on the ring and restore the previously saved commands.
 */
int radeon_ring_restore(struct radeon_device *rdev, struct radeon_ring *ring,
			unsigned size, uint32_t *data)
{
	int i, r;

	if (!size || !data)
		return 0;

	/* restore the saved ring content */
	r = radeon_ring_lock(rdev, ring, size);
	if (r)
		return r;

	for (i = 0; i < size; ++i) {
		radeon_ring_write(ring, data[i]);
	}

	radeon_ring_unlock_commit(rdev, ring, false);
	kvfree(data);
	return 0;
}

/**
 * radeon_ring_init - init driver ring struct.
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 * @ring_size: size of the ring
 * @rptr_offs: offset of the rptr writeback location in the WB buffer
 * @nop: nop packet for this ring
 *
 * Initialize the driver information for the selected ring (all asics).
 * Returns 0 on success, error on failure.
 */
int radeon_ring_init(struct radeon_device *rdev, struct radeon_ring *ring, unsigned ring_size,
		     unsigned rptr_offs, u32 nop)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int r;

	ring->ring_size = ring_size;
	ring->rptr_offs = rptr_offs;
<<<<<<< HEAD
	ring->rptr_reg = rptr_reg;
	ring->wptr_reg = wptr_reg;
	ring->ptr_reg_shift = ptr_reg_shift;
	ring->ptr_reg_mask = ptr_reg_mask;
	ring->nop = nop;
	/* Allocate ring buffer */
	if (ring->ring_obj == NULL) {
		r = radeon_bo_create(rdev, ring->ring_size, PAGE_SIZE, true,
					RADEON_GEM_DOMAIN_GTT,
					&ring->ring_obj);
=======
	ring->nop = nop;
	ring->rdev = rdev;
	/* Allocate ring buffer */
	if (ring->ring_obj == NULL) {
		r = radeon_bo_create(rdev, ring->ring_size, PAGE_SIZE, true,
				     RADEON_GEM_DOMAIN_GTT, 0, NULL,
				     NULL, &ring->ring_obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (r) {
			dev_err(rdev->dev, "(%d) ring create failed\n", r);
			return r;
		}
		r = radeon_bo_reserve(ring->ring_obj, false);
		if (unlikely(r != 0))
			return r;
		r = radeon_bo_pin(ring->ring_obj, RADEON_GEM_DOMAIN_GTT,
					&ring->gpu_addr);
		if (r) {
			radeon_bo_unreserve(ring->ring_obj);
			dev_err(rdev->dev, "(%d) ring pin failed\n", r);
			return r;
		}
		r = radeon_bo_kmap(ring->ring_obj,
				       (void **)&ring->ring);
		radeon_bo_unreserve(ring->ring_obj);
		if (r) {
			dev_err(rdev->dev, "(%d) ring map failed\n", r);
			return r;
		}
<<<<<<< HEAD
	}
	ring->ptr_mask = (ring->ring_size / 4) - 1;
	ring->ring_free_dw = ring->ring_size / 4;
	return 0;
}

=======
		radeon_debugfs_ring_init(rdev, ring);
	}
	ring->ptr_mask = (ring->ring_size / 4) - 1;
	ring->ring_free_dw = ring->ring_size / 4;
	if (rdev->wb.enabled) {
		u32 index = RADEON_WB_RING0_NEXT_RPTR + (ring->idx * 4);
		ring->next_rptr_gpu_addr = rdev->wb.gpu_addr + index;
		ring->next_rptr_cpu_addr = &rdev->wb.wb[index/4];
	}
	radeon_ring_lockup_update(rdev, ring);
	return 0;
}

/**
 * radeon_ring_fini - tear down the driver ring struct.
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 *
 * Tear down the driver information for the selected ring (all asics).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void radeon_ring_fini(struct radeon_device *rdev, struct radeon_ring *ring)
{
	int r;
	struct radeon_bo *ring_obj;

<<<<<<< HEAD
	mutex_lock(&ring->mutex);
	ring_obj = ring->ring_obj;
	ring->ring = NULL;
	ring->ring_obj = NULL;
	mutex_unlock(&ring->mutex);
=======
	mutex_lock(&rdev->ring_lock);
	ring_obj = ring->ring_obj;
	ring->ready = false;
	ring->ring = NULL;
	ring->ring_obj = NULL;
	mutex_unlock(&rdev->ring_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ring_obj) {
		r = radeon_bo_reserve(ring_obj, false);
		if (likely(r == 0)) {
			radeon_bo_kunmap(ring_obj);
			radeon_bo_unpin(ring_obj);
			radeon_bo_unreserve(ring_obj);
		}
		radeon_bo_unref(&ring_obj);
	}
}

/*
 * Debugfs info
 */
#if defined(CONFIG_DEBUG_FS)

<<<<<<< HEAD
static int radeon_debugfs_ring_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_device *dev = node->minor->dev;
	struct radeon_device *rdev = dev->dev_private;
	int ridx = *(int*)node->info_ent->data;
	struct radeon_ring *ring = &rdev->ring[ridx];
=======
static int radeon_debugfs_ring_info_show(struct seq_file *m, void *unused)
{
	struct radeon_ring *ring = m->private;
	struct radeon_device *rdev = ring->rdev;

	uint32_t rptr, wptr, rptr_next;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned count, i, j;

	radeon_ring_free_size(rdev, ring);
	count = (ring->ring_size / 4) - ring->ring_free_dw;
<<<<<<< HEAD
	seq_printf(m, "wptr(0x%04x): 0x%08x\n", ring->wptr_reg, RREG32(ring->wptr_reg));
	seq_printf(m, "rptr(0x%04x): 0x%08x\n", ring->rptr_reg, RREG32(ring->rptr_reg));
	seq_printf(m, "driver's copy of the wptr: 0x%08x\n", ring->wptr);
	seq_printf(m, "driver's copy of the rptr: 0x%08x\n", ring->rptr);
	seq_printf(m, "%u free dwords in ring\n", ring->ring_free_dw);
	seq_printf(m, "%u dwords in ring\n", count);
	i = ring->rptr;
	for (j = 0; j <= count; j++) {
		seq_printf(m, "r[%04d]=0x%08x\n", i, ring->ring[i]);
=======

	wptr = radeon_ring_get_wptr(rdev, ring);
	seq_printf(m, "wptr: 0x%08x [%5d]\n",
		   wptr, wptr);

	rptr = radeon_ring_get_rptr(rdev, ring);
	seq_printf(m, "rptr: 0x%08x [%5d]\n",
		   rptr, rptr);

	if (ring->rptr_save_reg) {
		rptr_next = RREG32(ring->rptr_save_reg);
		seq_printf(m, "rptr next(0x%04x): 0x%08x [%5d]\n",
			   ring->rptr_save_reg, rptr_next, rptr_next);
	} else
		rptr_next = ~0;

	seq_printf(m, "driver's copy of the wptr: 0x%08x [%5d]\n",
		   ring->wptr, ring->wptr);
	seq_printf(m, "last semaphore signal addr : 0x%016llx\n",
		   ring->last_semaphore_signal_addr);
	seq_printf(m, "last semaphore wait addr   : 0x%016llx\n",
		   ring->last_semaphore_wait_addr);
	seq_printf(m, "%u free dwords in ring\n", ring->ring_free_dw);
	seq_printf(m, "%u dwords in ring\n", count);

	if (!ring->ring)
		return 0;

	/* print 8 dw before current rptr as often it's the last executed
	 * packet that is the root issue
	 */
	i = (rptr + ring->ptr_mask + 1 - 32) & ring->ptr_mask;
	for (j = 0; j <= (count + 32); j++) {
		seq_printf(m, "r[%5d]=0x%08x", i, ring->ring[i]);
		if (rptr == i)
			seq_puts(m, " *");
		if (rptr_next == i)
			seq_puts(m, " #");
		seq_puts(m, "\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		i = (i + 1) & ring->ptr_mask;
	}
	return 0;
}

<<<<<<< HEAD
static int radeon_ring_type_gfx_index = RADEON_RING_TYPE_GFX_INDEX;
static int cayman_ring_type_cp1_index = CAYMAN_RING_TYPE_CP1_INDEX;
static int cayman_ring_type_cp2_index = CAYMAN_RING_TYPE_CP2_INDEX;

static struct drm_info_list radeon_debugfs_ring_info_list[] = {
	{"radeon_ring_gfx", radeon_debugfs_ring_info, 0, &radeon_ring_type_gfx_index},
	{"radeon_ring_cp1", radeon_debugfs_ring_info, 0, &cayman_ring_type_cp1_index},
	{"radeon_ring_cp2", radeon_debugfs_ring_info, 0, &cayman_ring_type_cp2_index},
};

static int radeon_debugfs_ib_info(struct seq_file *m, void *data)
{
	struct drm_info_node *node = (struct drm_info_node *) m->private;
	struct drm_device *dev = node->minor->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_ib *ib = &rdev->ib_pool.ibs[*((unsigned*)node->info_ent->data)];
	unsigned i;

	if (ib == NULL) {
		return 0;
	}
	seq_printf(m, "IB %04u\n", ib->idx);
	seq_printf(m, "IB fence %p\n", ib->fence);
	seq_printf(m, "IB size %05u dwords\n", ib->length_dw);
	for (i = 0; i < ib->length_dw; i++) {
		seq_printf(m, "[%05u]=0x%08X\n", i, ib->ptr[i]);
	}
	return 0;
}

static struct drm_info_list radeon_debugfs_ib_list[RADEON_IB_POOL_SIZE];
static char radeon_debugfs_ib_names[RADEON_IB_POOL_SIZE][32];
static unsigned radeon_debugfs_ib_idx[RADEON_IB_POOL_SIZE];
#endif

int radeon_debugfs_ring_init(struct radeon_device *rdev)
{
#if defined(CONFIG_DEBUG_FS)
	if (rdev->family >= CHIP_CAYMAN)
		return radeon_debugfs_add_files(rdev, radeon_debugfs_ring_info_list,
						ARRAY_SIZE(radeon_debugfs_ring_info_list));
	else
		return radeon_debugfs_add_files(rdev, radeon_debugfs_ring_info_list, 1);
#else
	return 0;
#endif
}

int radeon_debugfs_ib_init(struct radeon_device *rdev)
{
#if defined(CONFIG_DEBUG_FS)
	unsigned i;

	for (i = 0; i < RADEON_IB_POOL_SIZE; i++) {
		sprintf(radeon_debugfs_ib_names[i], "radeon_ib_%04u", i);
		radeon_debugfs_ib_idx[i] = i;
		radeon_debugfs_ib_list[i].name = radeon_debugfs_ib_names[i];
		radeon_debugfs_ib_list[i].show = &radeon_debugfs_ib_info;
		radeon_debugfs_ib_list[i].driver_features = 0;
		radeon_debugfs_ib_list[i].data = &radeon_debugfs_ib_idx[i];
	}
	return radeon_debugfs_add_files(rdev, radeon_debugfs_ib_list,
					RADEON_IB_POOL_SIZE);
#else
	return 0;
=======
DEFINE_SHOW_ATTRIBUTE(radeon_debugfs_ring_info);

static const char *radeon_debugfs_ring_idx_to_name(uint32_t ridx)
{
	switch (ridx) {
	case RADEON_RING_TYPE_GFX_INDEX:
		return "radeon_ring_gfx";
	case CAYMAN_RING_TYPE_CP1_INDEX:
		return "radeon_ring_cp1";
	case CAYMAN_RING_TYPE_CP2_INDEX:
		return "radeon_ring_cp2";
	case R600_RING_TYPE_DMA_INDEX:
		return "radeon_ring_dma1";
	case CAYMAN_RING_TYPE_DMA1_INDEX:
		return "radeon_ring_dma2";
	case R600_RING_TYPE_UVD_INDEX:
		return "radeon_ring_uvd";
	case TN_RING_TYPE_VCE1_INDEX:
		return "radeon_ring_vce1";
	case TN_RING_TYPE_VCE2_INDEX:
		return "radeon_ring_vce2";
	default:
		return NULL;

	}
}
#endif

static void radeon_debugfs_ring_init(struct radeon_device *rdev, struct radeon_ring *ring)
{
#if defined(CONFIG_DEBUG_FS)
	const char *ring_name = radeon_debugfs_ring_idx_to_name(ring->idx);
	struct dentry *root = rdev->ddev->primary->debugfs_root;

	if (ring_name)
		debugfs_create_file(ring_name, 0444, root, ring,
				    &radeon_debugfs_ring_info_fops);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}
