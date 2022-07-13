/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
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
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#ifndef _TTM_EXECBUF_UTIL_H_
#define _TTM_EXECBUF_UTIL_H_

<<<<<<< HEAD
#include "ttm/ttm_bo_api.h"
#include <linux/list.h>

=======
#include <linux/list.h>

struct ww_acquire_ctx;
struct dma_fence;
struct ttm_buffer_object;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * struct ttm_validate_buffer
 *
 * @head:           list head for thread-private list.
 * @bo:             refcounted buffer object pointer.
<<<<<<< HEAD
 * @new_sync_obj_arg: New sync_obj_arg for @bo, to be used once
 * adding a new sync object.
 * @reserved:       Indicates whether @bo has been reserved for validation.
 * @removed:        Indicates whether @bo has been removed from lru lists.
 * @put_count:      Number of outstanding references on bo::list_kref.
 * @old_sync_obj:   Pointer to a sync object about to be unreferenced
=======
 * @num_shared:     How many shared fences we want to add.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

struct ttm_validate_buffer {
	struct list_head head;
	struct ttm_buffer_object *bo;
<<<<<<< HEAD
	void *new_sync_obj_arg;
	bool reserved;
	bool removed;
	int put_count;
	void *old_sync_obj;
=======
	unsigned int num_shared;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * function ttm_eu_backoff_reservation
 *
<<<<<<< HEAD
=======
 * @ticket:   ww_acquire_ctx from reserve call
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @list:     thread private list of ttm_validate_buffer structs.
 *
 * Undoes all buffer validation reservations for bos pointed to by
 * the list entries.
 */
<<<<<<< HEAD

extern void ttm_eu_backoff_reservation(struct list_head *list);
=======
void ttm_eu_backoff_reservation(struct ww_acquire_ctx *ticket,
				struct list_head *list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * function ttm_eu_reserve_buffers
 *
<<<<<<< HEAD
 * @list:    thread private list of ttm_validate_buffer structs.
=======
 * @ticket:  [out] ww_acquire_ctx filled in by call, or NULL if only
 *           non-blocking reserves should be tried.
 * @list:    thread private list of ttm_validate_buffer structs.
 * @intr:    should the wait be interruptible
 * @dups:    [out] optional list of duplicates.
 * @del_lru: true if BOs should be removed from the LRU.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Tries to reserve bos pointed to by the list entries for validation.
 * If the function returns 0, all buffers are marked as "unfenced",
 * taken off the lru lists and are not synced for write CPU usage.
 *
 * If the function detects a deadlock due to multiple threads trying to
 * reserve the same buffers in reverse order, all threads except one will
 * back off and retry. This function may sleep while waiting for
 * CPU write reservations to be cleared, and for other threads to
 * unreserve their buffers.
 *
<<<<<<< HEAD
 * This function may return -ERESTART or -EAGAIN if the calling process
 * receives a signal while waiting. In that case, no buffers on the list
 * will be reserved upon return.
=======
 * If intr is set to true, this function may return -ERESTARTSYS if the
 * calling process receives a signal while waiting. In that case, no
 * buffers on the list will be reserved upon return.
 *
 * If dups is non NULL all buffers already reserved by the current thread
 * (e.g. duplicates) are added to this list, otherwise -EALREADY is returned
 * on the first already reserved buffer and all buffers from the list are
 * unreserved again.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Buffers reserved by this function should be unreserved by
 * a call to either ttm_eu_backoff_reservation() or
 * ttm_eu_fence_buffer_objects() when command submission is complete or
 * has failed.
 */
<<<<<<< HEAD

extern int ttm_eu_reserve_buffers(struct list_head *list);
=======
int ttm_eu_reserve_buffers(struct ww_acquire_ctx *ticket,
			   struct list_head *list, bool intr,
			   struct list_head *dups);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * function ttm_eu_fence_buffer_objects.
 *
<<<<<<< HEAD
 * @list:        thread private list of ttm_validate_buffer structs.
 * @sync_obj:    The new sync object for the buffers.
=======
 * @ticket:      ww_acquire_ctx from reserve call
 * @list:        thread private list of ttm_validate_buffer structs.
 * @fence:       The new exclusive fence for the buffers.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function should be called when command submission is complete, and
 * it will add a new sync object to bos pointed to by entries on @list.
 * It also unreserves all buffers, putting them on lru lists.
 *
 */
<<<<<<< HEAD

extern void ttm_eu_fence_buffer_objects(struct list_head *list, void *sync_obj);
=======
void ttm_eu_fence_buffer_objects(struct ww_acquire_ctx *ticket,
				 struct list_head *list,
				 struct dma_fence *fence);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
