<<<<<<< HEAD
/**************************************************************************
 *
 * Copyright © 2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
=======
/* SPDX-License-Identifier: GPL-2.0 OR MIT */
/**************************************************************************
 *
 * Copyright 2009-2023 VMware, Inc., Palo Alto, CA., USA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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

#ifndef _VMWGFX_DRV_H_
#define _VMWGFX_DRV_H_

<<<<<<< HEAD
#include "vmwgfx_reg.h"
#include "drmP.h"
#include "vmwgfx_drm.h"
#include "drm_hashtab.h"
#include "linux/suspend.h"
#include "ttm/ttm_bo_driver.h"
#include "ttm/ttm_object.h"
#include "ttm/ttm_lock.h"
#include "ttm/ttm_execbuf_util.h"
#include "ttm/ttm_module.h"
#include "vmwgfx_fence.h"

#define VMWGFX_DRIVER_DATE "20120209"
#define VMWGFX_DRIVER_MAJOR 2
#define VMWGFX_DRIVER_MINOR 4
#define VMWGFX_DRIVER_PATCHLEVEL 0
#define VMWGFX_FILE_PAGE_OFFSET 0x00100000
#define VMWGFX_FIFO_STATIC_SIZE (1024*1024)
#define VMWGFX_MAX_RELOCATIONS 2048
#define VMWGFX_MAX_VALIDATIONS 2048
#define VMWGFX_MAX_DISPLAYS 16
#define VMWGFX_CMD_BOUNCE_INIT_SIZE 32768

#define VMW_PL_GMR TTM_PL_PRIV0
#define VMW_PL_FLAG_GMR TTM_PL_FLAG_PRIV0
=======
#include <linux/suspend.h>
#include <linux/sync_file.h>
#include <linux/hashtable.h>

#include <drm/drm_auth.h>
#include <drm/drm_device.h>
#include <drm/drm_file.h>
#include <drm/drm_rect.h>

#include <drm/ttm/ttm_execbuf_util.h>
#include <drm/ttm/ttm_tt.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_bo.h>

#include "ttm_object.h"

#include "vmwgfx_fence.h"
#include "vmwgfx_reg.h"
#include "vmwgfx_validation.h"

/*
 * FIXME: vmwgfx_drm.h needs to be last due to dependencies.
 * uapi headers should not depend on header files outside uapi/.
 */
#include <drm/vmwgfx_drm.h>


#define VMWGFX_DRIVER_NAME "vmwgfx"
#define VMWGFX_DRIVER_DATE "20211206"
#define VMWGFX_DRIVER_MAJOR 2
#define VMWGFX_DRIVER_MINOR 20
#define VMWGFX_DRIVER_PATCHLEVEL 0
#define VMWGFX_FIFO_STATIC_SIZE (1024*1024)
#define VMWGFX_MAX_DISPLAYS 16
#define VMWGFX_CMD_BOUNCE_INIT_SIZE 32768

#define VMWGFX_MIN_INITIAL_WIDTH 1280
#define VMWGFX_MIN_INITIAL_HEIGHT 800

#define VMWGFX_PCI_ID_SVGA2              0x0405
#define VMWGFX_PCI_ID_SVGA3              0x0406

/*
 * This has to match get_count_order(SVGA_IRQFLAG_MAX)
 */
#define VMWGFX_MAX_NUM_IRQS 6

/*
 * Perhaps we should have sysfs entries for these.
 */
#define VMWGFX_NUM_GB_CONTEXT 256
#define VMWGFX_NUM_GB_SHADER 20000
#define VMWGFX_NUM_GB_SURFACE 32768
#define VMWGFX_NUM_GB_SCREEN_TARGET VMWGFX_MAX_DISPLAYS
#define VMWGFX_NUM_DXCONTEXT 256
#define VMWGFX_NUM_DXQUERY 512
#define VMWGFX_NUM_MOB (VMWGFX_NUM_GB_CONTEXT +\
			VMWGFX_NUM_GB_SHADER +\
			VMWGFX_NUM_GB_SURFACE +\
			VMWGFX_NUM_GB_SCREEN_TARGET)

#define VMW_PL_GMR      (TTM_PL_PRIV + 0)
#define VMW_PL_MOB      (TTM_PL_PRIV + 1)
#define VMW_PL_SYSTEM   (TTM_PL_PRIV + 2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define VMW_RES_CONTEXT ttm_driver_type0
#define VMW_RES_SURFACE ttm_driver_type1
#define VMW_RES_STREAM ttm_driver_type2
#define VMW_RES_FENCE ttm_driver_type3
<<<<<<< HEAD

struct vmw_fpriv {
	struct drm_master *locked_master;
	struct ttm_object_file *tfile;
	struct list_head fence_events;
};

struct vmw_dma_buffer {
	struct ttm_buffer_object base;
	struct list_head validate_list;
	bool gmr_bound;
	uint32_t cur_validate_node;
	bool on_validate_list;
};

struct vmw_resource {
	struct kref kref;
	struct vmw_private *dev_priv;
	struct idr *idr;
	int id;
	enum ttm_object_type res_type;
	bool avail;
	void (*remove_from_lists) (struct vmw_resource *res);
	void (*hw_destroy) (struct vmw_resource *res);
	void (*res_free) (struct vmw_resource *res);
	struct list_head validate_head;
	struct list_head query_head; /* Protected by the cmdbuf mutex */
	/* TODO is a generic snooper needed? */
#if 0
	void (*snoop)(struct vmw_resource *res,
		      struct ttm_object_file *tfile,
		      SVGA3dCmdHeader *header);
	void *snoop_priv;
#endif
};

struct vmw_cursor_snooper {
	struct drm_crtc *crtc;
=======
#define VMW_RES_SHADER ttm_driver_type4
#define VMW_RES_HT_ORDER 12

#define VMW_CURSOR_SNOOP_FORMAT SVGA3D_A8R8G8B8
#define VMW_CURSOR_SNOOP_WIDTH 64
#define VMW_CURSOR_SNOOP_HEIGHT 64

#define MKSSTAT_CAPACITY_LOG2 5U
#define MKSSTAT_CAPACITY (1U << MKSSTAT_CAPACITY_LOG2)

struct vmw_fpriv {
	struct ttm_object_file *tfile;
	bool gb_aware; /* user-space is guest-backed aware */
};

struct vmwgfx_hash_item {
	struct hlist_node head;
	unsigned long key;
};


/**
 * struct vmw_validate_buffer - Carries validation info about buffers.
 *
 * @base: Validation info for TTM.
 * @hash: Hash entry for quick lookup of the TTM buffer object.
 *
 * This structure contains also driver private validation info
 * on top of the info needed by TTM.
 */
struct vmw_validate_buffer {
	struct ttm_validate_buffer base;
	struct vmwgfx_hash_item hash;
	bool validate_as_mob;
};

struct vmw_res_func;


/**
 * struct vmw-resource - base class for hardware resources
 *
 * @kref: For refcounting.
 * @dev_priv: Pointer to the device private for this resource. Immutable.
 * @id: Device id. Protected by @dev_priv::resource_lock.
 * @guest_memory_size: Guest memory buffer size. Immutable.
 * @res_dirty: Resource contains data not yet in the guest memory buffer.
 * Protected by resource reserved.
 * @guest_memory_dirty: Guest memory buffer contains data not yet in the HW
 * resource. Protected by resource reserved.
 * @coherent: Emulate coherency by tracking vm accesses.
 * @guest_memory_bo: The guest memory buffer if any. Protected by resource
 * reserved.
 * @guest_memory_offset: Offset into the guest memory buffer if any. Protected
 * by resource reserved. Note that only a few resource types can have a
 * @guest_memory_offset different from zero.
 * @pin_count: The pin count for this resource. A pinned resource has a
 * pin-count greater than zero. It is not on the resource LRU lists and its
 * guest memory buffer is pinned. Hence it can't be evicted.
 * @func: Method vtable for this resource. Immutable.
 * @mob_node; Node for the MOB guest memory rbtree. Protected by
 * @guest_memory_bo reserved.
 * @lru_head: List head for the LRU list. Protected by @dev_priv::resource_lock.
 * @binding_head: List head for the context binding list. Protected by
 * the @dev_priv::binding_mutex
 * @res_free: The resource destructor.
 * @hw_destroy: Callback to destroy the resource on the device, as part of
 * resource destruction.
 */
struct vmw_bo;
struct vmw_bo;
struct vmw_resource_dirty;
struct vmw_resource {
	struct kref kref;
	struct vmw_private *dev_priv;
	int id;
	u32 used_prio;
	unsigned long guest_memory_size;
	u32 res_dirty : 1;
	u32 guest_memory_dirty : 1;
	u32 coherent : 1;
	struct vmw_bo *guest_memory_bo;
	unsigned long guest_memory_offset;
	unsigned long pin_count;
	const struct vmw_res_func *func;
	struct rb_node mob_node;
	struct list_head lru_head;
	struct list_head binding_head;
	struct vmw_resource_dirty *dirty;
	void (*res_free) (struct vmw_resource *res);
	void (*hw_destroy) (struct vmw_resource *res);
};


/*
 * Resources that are managed using ioctls.
 */
enum vmw_res_type {
	vmw_res_context,
	vmw_res_surface,
	vmw_res_stream,
	vmw_res_shader,
	vmw_res_dx_context,
	vmw_res_cotable,
	vmw_res_view,
	vmw_res_streamoutput,
	vmw_res_max
};

/*
 * Resources that are managed using command streams.
 */
enum vmw_cmdbuf_res_type {
	vmw_cmdbuf_res_shader,
	vmw_cmdbuf_res_view,
	vmw_cmdbuf_res_streamoutput
};

struct vmw_cmdbuf_res_manager;

struct vmw_cursor_snooper {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t age;
	uint32_t *image;
};

struct vmw_framebuffer;
struct vmw_surface_offset;

<<<<<<< HEAD
struct vmw_surface {
	struct vmw_resource res;
	struct list_head lru_head; /* Protected by the resource lock */
	uint32_t flags;
	uint32_t format;
	uint32_t mip_levels[DRM_VMW_MAX_SURFACE_FACES];
	struct drm_vmw_size *sizes;
	uint32_t num_sizes;

	bool scanout;

	/* TODO so far just a extra pointer */
	struct vmw_cursor_snooper snooper;
	struct ttm_buffer_object *backup;
	struct vmw_surface_offset *offsets;
	uint32_t backup_size;
};

struct vmw_marker_queue {
	struct list_head head;
	struct timespec lag;
	struct timespec lag_time;
	spinlock_t lock;
=======
/**
 * struct vmw_surface_metadata - Metadata describing a surface.
 *
 * @flags: Device flags.
 * @format: Surface SVGA3D_x format.
 * @mip_levels: Mip level for each face. For GB first index is used only.
 * @multisample_count: Sample count.
 * @multisample_pattern: Sample patterns.
 * @quality_level: Quality level.
 * @autogen_filter: Filter for automatically generated mipmaps.
 * @array_size: Number of array elements for a 1D/2D texture. For cubemap
                texture number of faces * array_size. This should be 0 for pre
		SM4 device.
 * @buffer_byte_stride: Buffer byte stride.
 * @num_sizes: Size of @sizes. For GB surface this should always be 1.
 * @base_size: Surface dimension.
 * @sizes: Array representing mip sizes. Legacy only.
 * @scanout: Whether this surface will be used for scanout.
 *
 * This tracks metadata for both legacy and guest backed surface.
 */
struct vmw_surface_metadata {
	u64 flags;
	u32 format;
	u32 mip_levels[DRM_VMW_MAX_SURFACE_FACES];
	u32 multisample_count;
	u32 multisample_pattern;
	u32 quality_level;
	u32 autogen_filter;
	u32 array_size;
	u32 num_sizes;
	u32 buffer_byte_stride;
	struct drm_vmw_size base_size;
	struct drm_vmw_size *sizes;
	bool scanout;
};

/**
 * struct vmw_surface: Resource structure for a surface.
 *
 * @res: The base resource for this surface.
 * @metadata: Metadata for this surface resource.
 * @snooper: Cursor data. Legacy surface only.
 * @offsets: Legacy surface only.
 * @view_list: List of views bound to this surface.
 */
struct vmw_surface {
	struct vmw_resource res;
	struct vmw_surface_metadata metadata;
	struct vmw_cursor_snooper snooper;
	struct vmw_surface_offset *offsets;
	struct list_head view_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct vmw_fifo_state {
	unsigned long reserved_size;
<<<<<<< HEAD
	__le32 *dynamic_buffer;
	__le32 *static_buffer;
=======
	u32 *dynamic_buffer;
	u32 *static_buffer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long static_buffer_size;
	bool using_bounce_buffer;
	uint32_t capabilities;
	struct mutex fifo_mutex;
	struct rw_semaphore rwsem;
<<<<<<< HEAD
	struct vmw_marker_queue marker_queue;
};

struct vmw_relocation {
	SVGAGuestPtr *location;
	uint32_t index;
};

struct vmw_sw_context{
	struct ida bo_list;
	uint32_t last_cid;
	bool cid_valid;
	bool kernel; /**< is the called made from the kernel */
	struct vmw_resource *cur_ctx;
	uint32_t last_sid;
	uint32_t sid_translation;
	bool sid_valid;
	struct ttm_object_file *tfile;
	struct list_head validate_nodes;
	struct vmw_relocation relocs[VMWGFX_MAX_RELOCATIONS];
	uint32_t cur_reloc;
	struct ttm_validate_buffer val_bufs[VMWGFX_MAX_VALIDATIONS];
	uint32_t cur_val_buf;
	uint32_t *cmd_bounce;
	uint32_t cmd_bounce_size;
	struct list_head resource_list;
	uint32_t fence_flags;
	struct list_head query_list;
	struct ttm_buffer_object *cur_query_bo;
	uint32_t cur_query_cid;
	bool query_cid_valid;
=======
};

/**
 * struct vmw_res_cache_entry - resource information cache entry
 * @handle: User-space handle of a resource.
 * @res: Non-ref-counted pointer to the resource.
 * @valid_handle: Whether the @handle member is valid.
 * @valid: Whether the entry is valid, which also implies that the execbuf
 * code holds a reference to the resource, and it's placed on the
 * validation list.
 *
 * Used to avoid frequent repeated user-space handle lookups of the
 * same resource.
 */
struct vmw_res_cache_entry {
	uint32_t handle;
	struct vmw_resource *res;
	void *private;
	unsigned short valid_handle;
	unsigned short valid;
};

/**
 * enum vmw_dma_map_mode - indicate how to perform TTM page dma mappings.
 */
enum vmw_dma_map_mode {
	vmw_dma_alloc_coherent, /* Use TTM coherent pages */
	vmw_dma_map_populate,   /* Unmap from DMA just after unpopulate */
	vmw_dma_map_bind,       /* Unmap from DMA just before unbind */
	vmw_dma_map_max
};

/**
 * struct vmw_sg_table - Scatter/gather table for binding, with additional
 * device-specific information.
 *
 * @sgt: Pointer to a struct sg_table with binding information
 * @num_regions: Number of regions with device-address contiguous pages
 */
struct vmw_sg_table {
	enum vmw_dma_map_mode mode;
	struct page **pages;
	const dma_addr_t *addrs;
	struct sg_table *sgt;
	unsigned long num_pages;
};

/**
 * struct vmw_piter - Page iterator that iterates over a list of pages
 * and DMA addresses that could be either a scatter-gather list or
 * arrays
 *
 * @pages: Array of page pointers to the pages.
 * @addrs: DMA addresses to the pages if coherent pages are used.
 * @iter: Scatter-gather page iterator. Current position in SG list.
 * @i: Current position in arrays.
 * @num_pages: Number of pages total.
 * @next: Function to advance the iterator. Returns false if past the list
 * of pages, true otherwise.
 * @dma_address: Function to return the DMA address of the current page.
 */
struct vmw_piter {
	struct page **pages;
	const dma_addr_t *addrs;
	struct sg_dma_page_iter iter;
	unsigned long i;
	unsigned long num_pages;
	bool (*next)(struct vmw_piter *);
	dma_addr_t (*dma_address)(struct vmw_piter *);
};


struct vmw_ttm_tt {
	struct ttm_tt dma_ttm;
	struct vmw_private *dev_priv;
	int gmr_id;
	struct vmw_mob *mob;
	int mem_type;
	struct sg_table sgt;
	struct vmw_sg_table vsgt;
	bool mapped;
	bool bound;
};

/*
 * enum vmw_display_unit_type - Describes the display unit
 */
enum vmw_display_unit_type {
	vmw_du_invalid = 0,
	vmw_du_legacy,
	vmw_du_screen_object,
	vmw_du_screen_target,
	vmw_du_max
};

struct vmw_validation_context;
struct vmw_ctx_validation_info;

/**
 * struct vmw_sw_context - Command submission context
 * @res_ht: Pointer hash table used to find validation duplicates
 * @kernel: Whether the command buffer originates from kernel code rather
 * than from user-space
 * @fp: If @kernel is false, points to the file of the client. Otherwise
 * NULL
 * @cmd_bounce: Command bounce buffer used for command validation before
 * copying to fifo space
 * @cmd_bounce_size: Current command bounce buffer size
 * @cur_query_bo: Current buffer object used as query result buffer
 * @bo_relocations: List of buffer object relocations
 * @res_relocations: List of resource relocations
 * @buf_start: Pointer to start of memory where command validation takes
 * place
 * @res_cache: Cache of recently looked up resources
 * @last_query_ctx: Last context that submitted a query
 * @needs_post_query_barrier: Whether a query barrier is needed after
 * command submission
 * @staged_bindings: Cached per-context binding tracker
 * @staged_bindings_inuse: Whether the cached per-context binding tracker
 * is in use
 * @staged_cmd_res: List of staged command buffer managed resources in this
 * command buffer
 * @ctx_list: List of context resources referenced in this command buffer
 * @dx_ctx_node: Validation metadata of the current DX context
 * @dx_query_mob: The MOB used for DX queries
 * @dx_query_ctx: The DX context used for the last DX query
 * @man: Pointer to the command buffer managed resource manager
 * @ctx: The validation context
 */
struct vmw_sw_context{
	DECLARE_HASHTABLE(res_ht, VMW_RES_HT_ORDER);
	bool kernel;
	struct vmw_fpriv *fp;
	struct drm_file *filp;
	uint32_t *cmd_bounce;
	uint32_t cmd_bounce_size;
	struct vmw_bo *cur_query_bo;
	struct list_head bo_relocations;
	struct list_head res_relocations;
	uint32_t *buf_start;
	struct vmw_res_cache_entry res_cache[vmw_res_max];
	struct vmw_resource *last_query_ctx;
	bool needs_post_query_barrier;
	struct vmw_ctx_binding_state *staged_bindings;
	bool staged_bindings_inuse;
	struct list_head staged_cmd_res;
	struct list_head ctx_list;
	struct vmw_ctx_validation_info *dx_ctx_node;
	struct vmw_bo *dx_query_mob;
	struct vmw_resource *dx_query_ctx;
	struct vmw_cmdbuf_res_manager *man;
	struct vmw_validation_context *ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct vmw_legacy_display;
struct vmw_overlay;

<<<<<<< HEAD
struct vmw_master {
	struct ttm_lock lock;
	struct mutex fb_surf_mutex;
	struct list_head fb_surf;
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct vmw_vga_topology_state {
	uint32_t width;
	uint32_t height;
	uint32_t primary;
	uint32_t pos_x;
	uint32_t pos_y;
};

<<<<<<< HEAD
struct vmw_private {
	struct ttm_bo_device bdev;
	struct ttm_bo_global_ref bo_global_ref;
	struct drm_global_reference mem_global_ref;

	struct vmw_fifo_state fifo;

	struct drm_device *dev;
	unsigned long vmw_chipset;
	unsigned int io_start;
	uint32_t vram_start;
	uint32_t vram_size;
	uint32_t mmio_start;
	uint32_t mmio_size;
	uint32_t fb_max_width;
	uint32_t fb_max_height;
	uint32_t initial_width;
	uint32_t initial_height;
	__le32 __iomem *mmio_virt;
	int mmio_mtrr;
	uint32_t capabilities;
	uint32_t max_gmr_descriptors;
	uint32_t max_gmr_ids;
	uint32_t max_gmr_pages;
	uint32_t memory_size;
	bool has_gmr;
	struct mutex hw_mutex;

	/*
	 * VGA registers.
	 */

	struct vmw_vga_topology_state vga_save[VMWGFX_MAX_DISPLAYS];
	uint32_t vga_width;
	uint32_t vga_height;
	uint32_t vga_bpp;
	uint32_t vga_bpl;
	uint32_t vga_pitchlock;

	uint32_t num_displays;
=======

/*
 * struct vmw_otable - Guest Memory OBject table metadata
 *
 * @size:           Size of the table (page-aligned).
 * @page_table:     Pointer to a struct vmw_mob holding the page table.
 */
struct vmw_otable {
	unsigned long size;
	struct vmw_mob *page_table;
	bool enabled;
};

struct vmw_otable_batch {
	unsigned num_otables;
	struct vmw_otable *otables;
	struct vmw_resource *context;
	struct vmw_bo *otable_bo;
};

enum {
	VMW_IRQTHREAD_FENCE,
	VMW_IRQTHREAD_CMDBUF,
	VMW_IRQTHREAD_MAX
};

/**
 * enum vmw_sm_type - Graphics context capability supported by device.
 * @VMW_SM_LEGACY: Pre DX context.
 * @VMW_SM_4: Context support upto SM4.
 * @VMW_SM_4_1: Context support upto SM4_1.
 * @VMW_SM_5: Context support up to SM5.
 * @VMW_SM_5_1X: Adds support for sm5_1 and gl43 extensions.
 * @VMW_SM_MAX: Should be the last.
 */
enum vmw_sm_type {
	VMW_SM_LEGACY = 0,
	VMW_SM_4,
	VMW_SM_4_1,
	VMW_SM_5,
	VMW_SM_5_1X,
	VMW_SM_MAX
};

struct vmw_private {
	struct drm_device drm;
	struct ttm_device bdev;

	struct drm_vma_offset_manager vma_manager;
	u32 pci_id;
	resource_size_t io_start;
	resource_size_t vram_start;
	resource_size_t vram_size;
	resource_size_t max_primary_mem;
	u32 __iomem *rmmio;
	u32 *fifo_mem;
	resource_size_t fifo_mem_size;
	uint32_t fb_max_width;
	uint32_t fb_max_height;
	uint32_t texture_max_width;
	uint32_t texture_max_height;
	uint32_t stdu_max_width;
	uint32_t stdu_max_height;
	uint32_t initial_width;
	uint32_t initial_height;
	uint32_t capabilities;
	uint32_t capabilities2;
	uint32_t max_gmr_ids;
	uint32_t max_gmr_pages;
	uint32_t max_mob_pages;
	uint32_t max_mob_size;
	uint32_t memory_size;
	bool has_gmr;
	bool has_mob;
	spinlock_t hw_lock;
	bool assume_16bpp;
	u32 irqs[VMWGFX_MAX_NUM_IRQS];
	u32 num_irq_vectors;

	enum vmw_sm_type sm_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Framebuffer info.
	 */

<<<<<<< HEAD
	void *fb_info;
	struct vmw_legacy_display *ldu_priv;
	struct vmw_screen_object_display *sou_priv;
	struct vmw_overlay *overlay_priv;
=======
	enum vmw_display_unit_type active_display_unit;
	struct vmw_legacy_display *ldu_priv;
	struct vmw_overlay *overlay_priv;
	struct drm_property *hotplug_mode_update_property;
	struct drm_property *implicit_placement_property;
	spinlock_t cursor_lock;
	struct drm_atomic_state *suspend_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Context and surface management.
	 */

<<<<<<< HEAD
	rwlock_t resource_lock;
	struct idr context_idr;
	struct idr surface_idr;
	struct idr stream_idr;

	/*
	 * Block lastclose from racing with firstopen.
	 */

	struct mutex init_mutex;
=======
	spinlock_t resource_lock;
	struct idr res_idr[vmw_res_max];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * A resource manager for kernel-only surfaces and
	 * contexts.
	 */

	struct ttm_object_device *tdev;

	/*
	 * Fencing and IRQs.
	 */

	atomic_t marker_seq;
	wait_queue_head_t fence_queue;
	wait_queue_head_t fifo_queue;
<<<<<<< HEAD
	int fence_queue_waiters; /* Protected by hw_mutex */
	int goal_queue_waiters; /* Protected by hw_mutex */
	atomic_t fifo_queue_waiters;
	uint32_t last_read_seqno;
	spinlock_t irq_lock;
	struct vmw_fence_manager *fman;
	uint32_t irq_mask;
=======
	spinlock_t waiter_lock;
	int fence_queue_waiters; /* Protected by waiter_lock */
	int goal_queue_waiters; /* Protected by waiter_lock */
	int cmdbuf_waiters; /* Protected by waiter_lock */
	int error_waiters; /* Protected by waiter_lock */
	int fifo_queue_waiters; /* Protected by waiter_lock */
	uint32_t last_read_seqno;
	struct vmw_fence_manager *fman;
	uint32_t irq_mask; /* Updates protected by waiter_lock */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Device state
	 */

	uint32_t traces_state;
	uint32_t enable_state;
	uint32_t config_done_state;

	/**
	 * Execbuf
	 */
	/**
	 * Protected by the cmdbuf mutex.
	 */

	struct vmw_sw_context ctx;
	struct mutex cmdbuf_mutex;
<<<<<<< HEAD

	/**
	 * Operating mode.
	 */

	bool stealth;
	bool is_opened;
	bool enable_fb;

	/**
	 * Master management.
	 */

	struct vmw_master *active_master;
	struct vmw_master fbdev_master;
	struct notifier_block pm_nb;
	bool suspended;

	struct mutex release_mutex;
	uint32_t num_3d_resources;
=======
	struct mutex binding_mutex;

	/**
	 * PM management.
	 */
	struct notifier_block pm_nb;
	bool refuse_hibernation;
	bool suspend_locked;

	atomic_t num_fifo_resources;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Query processing. These members
	 * are protected by the cmdbuf mutex.
	 */

<<<<<<< HEAD
	struct ttm_buffer_object *dummy_query_bo;
	struct ttm_buffer_object *pinned_bo;
	uint32_t query_cid;
=======
	struct vmw_bo *dummy_query_bo;
	struct vmw_bo *pinned_bo;
	uint32_t query_cid;
	uint32_t query_cid_valid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool dummy_query_bo_pinned;

	/*
	 * Surface swapping. The "surface_lru" list is protected by the
	 * resource lock in order to be able to destroy a surface and take
	 * it off the lru atomically. "used_memory_size" is currently
	 * protected by the cmdbuf mutex for simplicity.
	 */

<<<<<<< HEAD
	struct list_head surface_lru;
	uint32_t used_memory_size;
};

=======
	struct list_head res_lru[vmw_res_max];
	uint32_t used_memory_size;

	/*
	 * DMA mapping stuff.
	 */
	enum vmw_dma_map_mode map_mode;

	/*
	 * Guest Backed stuff
	 */
	struct vmw_otable_batch otable_batch;

	struct vmw_fifo_state *fifo;
	struct vmw_cmdbuf_man *cman;
	DECLARE_BITMAP(irqthread_pending, VMW_IRQTHREAD_MAX);

	uint32 *devcaps;

	/*
	 * mksGuestStat instance-descriptor and pid arrays
	 */
	struct page *mksstat_user_pages[MKSSTAT_CAPACITY];
	atomic_t mksstat_user_pids[MKSSTAT_CAPACITY];

#if IS_ENABLED(CONFIG_DRM_VMWGFX_MKSSTATS)
	struct page *mksstat_kern_pages[MKSSTAT_CAPACITY];
	u8 mksstat_kern_top_timer[MKSSTAT_CAPACITY];
	atomic_t mksstat_kern_pids[MKSSTAT_CAPACITY];
#endif
};

static inline struct vmw_surface *vmw_res_to_srf(struct vmw_resource *res)
{
	return container_of(res, struct vmw_surface, res);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct vmw_private *vmw_priv(struct drm_device *dev)
{
	return (struct vmw_private *)dev->dev_private;
}

<<<<<<< HEAD
=======
static inline struct vmw_private *vmw_priv_from_ttm(struct ttm_device *bdev)
{
	return container_of(bdev, struct vmw_private, bdev);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct vmw_fpriv *vmw_fpriv(struct drm_file *file_priv)
{
	return (struct vmw_fpriv *)file_priv->driver_priv;
}

<<<<<<< HEAD
static inline struct vmw_master *vmw_master(struct drm_master *master)
{
	return (struct vmw_master *) master->driver_priv;
}

static inline void vmw_write(struct vmw_private *dev_priv,
			     unsigned int offset, uint32_t value)
{
	outl(offset, dev_priv->io_start + VMWGFX_INDEX_PORT);
	outl(value, dev_priv->io_start + VMWGFX_VALUE_PORT);
=======
/*
 * SVGA v3 has mmio register access and lacks fifo cmds
 */
static inline bool vmw_is_svga_v3(const struct vmw_private *dev)
{
	return dev->pci_id == VMWGFX_PCI_ID_SVGA3;
}

/*
 * The locking here is fine-grained, so that it is performed once
 * for every read- and write operation. This is of course costly, but we
 * don't perform much register access in the timing critical paths anyway.
 * Instead we have the extra benefit of being sure that we don't forget
 * the hw lock around register accesses.
 */
static inline void vmw_write(struct vmw_private *dev_priv,
			     unsigned int offset, uint32_t value)
{
	if (vmw_is_svga_v3(dev_priv)) {
		iowrite32(value, dev_priv->rmmio + offset);
	} else {
		spin_lock(&dev_priv->hw_lock);
		outl(offset, dev_priv->io_start + SVGA_INDEX_PORT);
		outl(value, dev_priv->io_start + SVGA_VALUE_PORT);
		spin_unlock(&dev_priv->hw_lock);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline uint32_t vmw_read(struct vmw_private *dev_priv,
				unsigned int offset)
{
<<<<<<< HEAD
	uint32_t val;

	outl(offset, dev_priv->io_start + VMWGFX_INDEX_PORT);
	val = inl(dev_priv->io_start + VMWGFX_VALUE_PORT);
	return val;
}

int vmw_3d_resource_inc(struct vmw_private *dev_priv, bool unhide_svga);
void vmw_3d_resource_dec(struct vmw_private *dev_priv, bool hide_svga);
=======
	u32 val;

	if (vmw_is_svga_v3(dev_priv)) {
		val = ioread32(dev_priv->rmmio + offset);
	} else {
		spin_lock(&dev_priv->hw_lock);
		outl(offset, dev_priv->io_start + SVGA_INDEX_PORT);
		val = inl(dev_priv->io_start + SVGA_VALUE_PORT);
		spin_unlock(&dev_priv->hw_lock);
	}

	return val;
}

/**
 * has_sm4_context - Does the device support SM4 context.
 * @dev_priv: Device private.
 *
 * Return: Bool value if device support SM4 context or not.
 */
static inline bool has_sm4_context(const struct vmw_private *dev_priv)
{
	return (dev_priv->sm_type >= VMW_SM_4);
}

/**
 * has_sm4_1_context - Does the device support SM4_1 context.
 * @dev_priv: Device private.
 *
 * Return: Bool value if device support SM4_1 context or not.
 */
static inline bool has_sm4_1_context(const struct vmw_private *dev_priv)
{
	return (dev_priv->sm_type >= VMW_SM_4_1);
}

/**
 * has_sm5_context - Does the device support SM5 context.
 * @dev_priv: Device private.
 *
 * Return: Bool value if device support SM5 context or not.
 */
static inline bool has_sm5_context(const struct vmw_private *dev_priv)
{
	return (dev_priv->sm_type >= VMW_SM_5);
}

/**
 * has_gl43_context - Does the device support GL43 context.
 * @dev_priv: Device private.
 *
 * Return: Bool value if device support SM5 context or not.
 */
static inline bool has_gl43_context(const struct vmw_private *dev_priv)
{
	return (dev_priv->sm_type >= VMW_SM_5_1X);
}


static inline u32 vmw_max_num_uavs(struct vmw_private *dev_priv)
{
	return (has_gl43_context(dev_priv) ?
			SVGA3D_DX11_1_MAX_UAVIEWS : SVGA3D_MAX_UAVIEWS);
}

extern void vmw_svga_enable(struct vmw_private *dev_priv);
extern void vmw_svga_disable(struct vmw_private *dev_priv);
bool vmwgfx_supported(struct vmw_private *vmw);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * GMR utilities - vmwgfx_gmr.c
 */

extern int vmw_gmr_bind(struct vmw_private *dev_priv,
<<<<<<< HEAD
			struct page *pages[],
=======
			const struct vmw_sg_table *vsgt,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			unsigned long num_pages,
			int gmr_id);
extern void vmw_gmr_unbind(struct vmw_private *dev_priv, int gmr_id);

/**
 * Resource utilities - vmwgfx_resource.c
 */
<<<<<<< HEAD

extern struct vmw_resource *vmw_context_alloc(struct vmw_private *dev_priv);
extern void vmw_resource_unreference(struct vmw_resource **p_res);
extern struct vmw_resource *vmw_resource_reference(struct vmw_resource *res);
extern int vmw_context_destroy_ioctl(struct drm_device *dev, void *data,
				     struct drm_file *file_priv);
extern int vmw_context_define_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
extern int vmw_context_check(struct vmw_private *dev_priv,
			     struct ttm_object_file *tfile,
			     int id,
			     struct vmw_resource **p_res);
extern int vmw_user_lookup_handle(struct vmw_private *dev_priv,
				  struct ttm_object_file *tfile,
				  uint32_t handle,
				  struct vmw_surface **out_surf,
				  struct vmw_dma_buffer **out_buf);
extern void vmw_surface_res_free(struct vmw_resource *res);
extern int vmw_surface_init(struct vmw_private *dev_priv,
			    struct vmw_surface *srf,
			    void (*res_free) (struct vmw_resource *res));
extern int vmw_user_surface_lookup_handle(struct vmw_private *dev_priv,
					  struct ttm_object_file *tfile,
					  uint32_t handle,
					  struct vmw_surface **out);
extern int vmw_surface_destroy_ioctl(struct drm_device *dev, void *data,
				     struct drm_file *file_priv);
extern int vmw_surface_define_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
extern int vmw_surface_reference_ioctl(struct drm_device *dev, void *data,
				       struct drm_file *file_priv);
extern int vmw_surface_check(struct vmw_private *dev_priv,
			     struct ttm_object_file *tfile,
			     uint32_t handle, int *id);
extern int vmw_surface_validate(struct vmw_private *dev_priv,
				struct vmw_surface *srf);
extern void vmw_dmabuf_bo_free(struct ttm_buffer_object *bo);
extern int vmw_dmabuf_init(struct vmw_private *dev_priv,
			   struct vmw_dma_buffer *vmw_bo,
			   size_t size, struct ttm_placement *placement,
			   bool interuptable,
			   void (*bo_free) (struct ttm_buffer_object *bo));
extern int vmw_dmabuf_alloc_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file_priv);
extern int vmw_dmabuf_unref_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file_priv);
extern uint32_t vmw_dmabuf_validate_node(struct ttm_buffer_object *bo,
					 uint32_t cur_validate_node);
extern void vmw_dmabuf_validate_clear(struct ttm_buffer_object *bo);
extern int vmw_user_dmabuf_lookup(struct ttm_object_file *tfile,
				  uint32_t id, struct vmw_dma_buffer **out);
=======
struct vmw_user_resource_conv;

extern void vmw_resource_unreference(struct vmw_resource **p_res);
extern struct vmw_resource *vmw_resource_reference(struct vmw_resource *res);
extern struct vmw_resource *
vmw_resource_reference_unless_doomed(struct vmw_resource *res);
extern int vmw_resource_validate(struct vmw_resource *res, bool intr,
				 bool dirtying);
extern int vmw_resource_reserve(struct vmw_resource *res, bool interruptible,
				bool no_backup);
extern bool vmw_resource_needs_backup(const struct vmw_resource *res);
extern int vmw_user_lookup_handle(struct vmw_private *dev_priv,
				  struct drm_file *filp,
				  uint32_t handle,
				  struct vmw_surface **out_surf,
				  struct vmw_bo **out_buf);
extern int vmw_user_resource_lookup_handle(
	struct vmw_private *dev_priv,
	struct ttm_object_file *tfile,
	uint32_t handle,
	const struct vmw_user_resource_conv *converter,
	struct vmw_resource **p_res);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int vmw_stream_claim_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file_priv);
extern int vmw_stream_unref_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file_priv);
extern int vmw_user_stream_lookup(struct vmw_private *dev_priv,
				  struct ttm_object_file *tfile,
				  uint32_t *inout_id,
				  struct vmw_resource **out);
<<<<<<< HEAD
extern void vmw_resource_unreserve(struct list_head *list);

/**
 * DMA buffer helper routines - vmwgfx_dmabuf.c
 */
extern int vmw_dmabuf_to_placement(struct vmw_private *vmw_priv,
				   struct vmw_dma_buffer *bo,
				   struct ttm_placement *placement,
				   bool interruptible);
extern int vmw_dmabuf_to_vram(struct vmw_private *dev_priv,
			      struct vmw_dma_buffer *buf,
			      bool pin, bool interruptible);
extern int vmw_dmabuf_to_vram_or_gmr(struct vmw_private *dev_priv,
				     struct vmw_dma_buffer *buf,
				     bool pin, bool interruptible);
extern int vmw_dmabuf_to_start_of_vram(struct vmw_private *vmw_priv,
				       struct vmw_dma_buffer *bo,
				       bool pin, bool interruptible);
extern int vmw_dmabuf_unpin(struct vmw_private *vmw_priv,
			    struct vmw_dma_buffer *bo,
			    bool interruptible);
extern void vmw_bo_get_guest_ptr(const struct ttm_buffer_object *buf,
				 SVGAGuestPtr *ptr);
extern void vmw_bo_pin(struct ttm_buffer_object *bo, bool pin);
=======
extern void vmw_resource_unreserve(struct vmw_resource *res,
				   bool dirty_set,
				   bool dirty,
				   bool switch_guest_memory,
				   struct vmw_bo *new_guest_memory,
				   unsigned long new_guest_memory_offset);
extern void vmw_query_move_notify(struct ttm_buffer_object *bo,
				  struct ttm_resource *old_mem,
				  struct ttm_resource *new_mem);
int vmw_query_readback_all(struct vmw_bo *dx_query_mob);
void vmw_resource_evict_all(struct vmw_private *dev_priv);
void vmw_resource_unbind_list(struct vmw_bo *vbo);
void vmw_resource_mob_attach(struct vmw_resource *res);
void vmw_resource_mob_detach(struct vmw_resource *res);
void vmw_resource_dirty_update(struct vmw_resource *res, pgoff_t start,
			       pgoff_t end);
int vmw_resources_clean(struct vmw_bo *vbo, pgoff_t start,
			pgoff_t end, pgoff_t *num_prefault);

/**
 * vmw_resource_mob_attached - Whether a resource currently has a mob attached
 * @res: The resource
 *
 * Return: true if the resource has a mob attached, false otherwise.
 */
static inline bool vmw_resource_mob_attached(const struct vmw_resource *res)
{
	return !RB_EMPTY_NODE(&res->mob_node);
}

/**
 * GEM related functionality - vmwgfx_gem.c
 */
struct vmw_bo_params;
int vmw_gem_object_create(struct vmw_private *vmw,
			  struct vmw_bo_params *params,
			  struct vmw_bo **p_vbo);
extern int vmw_gem_object_create_with_handle(struct vmw_private *dev_priv,
					     struct drm_file *filp,
					     uint32_t size,
					     uint32_t *handle,
					     struct vmw_bo **p_vbo);
extern int vmw_gem_object_create_ioctl(struct drm_device *dev, void *data,
				       struct drm_file *filp);
extern void vmw_debugfs_gem_init(struct vmw_private *vdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Misc Ioctl functionality - vmwgfx_ioctl.c
 */

extern int vmw_getparam_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv);
extern int vmw_get_cap_3d_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
extern int vmw_present_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);
extern int vmw_present_readback_ioctl(struct drm_device *dev, void *data,
				      struct drm_file *file_priv);
<<<<<<< HEAD
extern unsigned int vmw_fops_poll(struct file *filp,
				  struct poll_table_struct *wait);
extern ssize_t vmw_fops_read(struct file *filp, char __user *buffer,
			     size_t count, loff_t *offset);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Fifo utilities - vmwgfx_fifo.c
 */

<<<<<<< HEAD
extern int vmw_fifo_init(struct vmw_private *dev_priv,
			 struct vmw_fifo_state *fifo);
extern void vmw_fifo_release(struct vmw_private *dev_priv,
			     struct vmw_fifo_state *fifo);
extern void *vmw_fifo_reserve(struct vmw_private *dev_priv, uint32_t bytes);
extern void vmw_fifo_commit(struct vmw_private *dev_priv, uint32_t bytes);
extern int vmw_fifo_send_fence(struct vmw_private *dev_priv,
			       uint32_t *seqno);
extern void vmw_fifo_ping_host(struct vmw_private *dev_priv, uint32_t reason);
extern bool vmw_fifo_have_3d(struct vmw_private *dev_priv);
extern bool vmw_fifo_have_pitchlock(struct vmw_private *dev_priv);
extern int vmw_fifo_emit_dummy_query(struct vmw_private *dev_priv,
				     uint32_t cid);

/**
 * TTM glue - vmwgfx_ttm_glue.c
 */

extern int vmw_ttm_global_init(struct vmw_private *dev_priv);
extern void vmw_ttm_global_release(struct vmw_private *dev_priv);
extern int vmw_mmap(struct file *filp, struct vm_area_struct *vma);

/**
 * TTM buffer object driver - vmwgfx_buffer.c
 */

extern struct ttm_placement vmw_vram_placement;
extern struct ttm_placement vmw_vram_ne_placement;
extern struct ttm_placement vmw_vram_sys_placement;
extern struct ttm_placement vmw_vram_gmr_placement;
extern struct ttm_placement vmw_vram_gmr_ne_placement;
extern struct ttm_placement vmw_sys_placement;
extern struct ttm_placement vmw_evictable_placement;
extern struct ttm_placement vmw_srf_placement;
extern struct ttm_bo_driver vmw_bo_driver;
extern int vmw_dma_quiescent(struct drm_device *dev);
=======
extern struct vmw_fifo_state *vmw_fifo_create(struct vmw_private *dev_priv);
extern void vmw_fifo_destroy(struct vmw_private *dev_priv);
extern bool vmw_cmd_supported(struct vmw_private *vmw);
extern void *
vmw_cmd_ctx_reserve(struct vmw_private *dev_priv, uint32_t bytes, int ctx_id);
extern void vmw_cmd_commit(struct vmw_private *dev_priv, uint32_t bytes);
extern void vmw_cmd_commit_flush(struct vmw_private *dev_priv, uint32_t bytes);
extern int vmw_cmd_send_fence(struct vmw_private *dev_priv, uint32_t *seqno);
extern bool vmw_supports_3d(struct vmw_private *dev_priv);
extern void vmw_fifo_ping_host(struct vmw_private *dev_priv, uint32_t reason);
extern bool vmw_fifo_have_pitchlock(struct vmw_private *dev_priv);
extern int vmw_cmd_emit_dummy_query(struct vmw_private *dev_priv,
				    uint32_t cid);
extern int vmw_cmd_flush(struct vmw_private *dev_priv,
			 bool interruptible);

#define VMW_CMD_CTX_RESERVE(__priv, __bytes, __ctx_id)                        \
({                                                                            \
	vmw_cmd_ctx_reserve(__priv, __bytes, __ctx_id) ? : ({                 \
		DRM_ERROR("FIFO reserve failed at %s for %u bytes\n",         \
			  __func__, (unsigned int) __bytes);                  \
		NULL;                                                         \
	});                                                                   \
})

#define VMW_CMD_RESERVE(__priv, __bytes)                                     \
	VMW_CMD_CTX_RESERVE(__priv, __bytes, SVGA3D_INVALID_ID)


/**
 * vmw_fifo_caps - Returns the capabilities of the FIFO command
 * queue or 0 if fifo memory isn't present.
 * @dev_priv: The device private context
 */
static inline uint32_t vmw_fifo_caps(const struct vmw_private *dev_priv)
{
	if (!dev_priv->fifo_mem || !dev_priv->fifo)
		return 0;
	return dev_priv->fifo->capabilities;
}


/**
 * vmw_is_cursor_bypass3_enabled - Returns TRUE iff Cursor Bypass 3
 * is enabled in the FIFO.
 * @dev_priv: The device private context
 */
static inline bool
vmw_is_cursor_bypass3_enabled(const struct vmw_private *dev_priv)
{
	return (vmw_fifo_caps(dev_priv) & SVGA_FIFO_CAP_CURSOR_BYPASS_3) != 0;
}

/**
 * TTM buffer object driver - vmwgfx_ttm_buffer.c
 */

extern const size_t vmw_tt_size;
extern struct ttm_placement vmw_vram_placement;
extern struct ttm_placement vmw_sys_placement;
extern struct ttm_device_funcs vmw_bo_driver;
extern const struct vmw_sg_table *
vmw_bo_sg_table(struct ttm_buffer_object *bo);
int vmw_bo_create_and_populate(struct vmw_private *dev_priv,
			       size_t bo_size,
			       u32 domain,
			       struct vmw_bo **bo_p);

extern void vmw_piter_start(struct vmw_piter *viter,
			    const struct vmw_sg_table *vsgt,
			    unsigned long p_offs);

/**
 * vmw_piter_next - Advance the iterator one page.
 *
 * @viter: Pointer to the iterator to advance.
 *
 * Returns false if past the list of pages, true otherwise.
 */
static inline bool vmw_piter_next(struct vmw_piter *viter)
{
	return viter->next(viter);
}

/**
 * vmw_piter_dma_addr - Return the DMA address of the current page.
 *
 * @viter: Pointer to the iterator
 *
 * Returns the DMA address of the page pointed to by @viter.
 */
static inline dma_addr_t vmw_piter_dma_addr(struct vmw_piter *viter)
{
	return viter->dma_address(viter);
}

/**
 * vmw_piter_page - Return a pointer to the current page.
 *
 * @viter: Pointer to the iterator
 *
 * Returns the DMA address of the page pointed to by @viter.
 */
static inline struct page *vmw_piter_page(struct vmw_piter *viter)
{
	return viter->pages[viter->i];
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Command submission - vmwgfx_execbuf.c
 */

extern int vmw_execbuf_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);
extern int vmw_execbuf_process(struct drm_file *file_priv,
			       struct vmw_private *dev_priv,
			       void __user *user_commands,
			       void *kernel_commands,
			       uint32_t command_size,
			       uint64_t throttle_us,
<<<<<<< HEAD
			       struct drm_vmw_fence_rep __user
			       *user_fence_rep,
			       struct vmw_fence_obj **out_fence);

extern void
vmw_execbuf_release_pinned_bo(struct vmw_private *dev_priv,
			      bool only_on_cid_match, uint32_t cid);
=======
			       uint32_t dx_context_handle,
			       struct drm_vmw_fence_rep __user
			       *user_fence_rep,
			       struct vmw_fence_obj **out_fence,
			       uint32_t flags);
extern void __vmw_execbuf_release_pinned_bo(struct vmw_private *dev_priv,
					    struct vmw_fence_obj *fence);
extern void vmw_execbuf_release_pinned_bo(struct vmw_private *dev_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int vmw_execbuf_fence_commands(struct drm_file *file_priv,
				      struct vmw_private *dev_priv,
				      struct vmw_fence_obj **p_fence,
				      uint32_t *p_handle);
<<<<<<< HEAD
extern void vmw_execbuf_copy_fence_user(struct vmw_private *dev_priv,
=======
extern int vmw_execbuf_copy_fence_user(struct vmw_private *dev_priv,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					struct vmw_fpriv *vmw_fp,
					int ret,
					struct drm_vmw_fence_rep __user
					*user_fence_rep,
					struct vmw_fence_obj *fence,
<<<<<<< HEAD
					uint32_t fence_handle);
=======
					uint32_t fence_handle,
					int32_t out_fence_fd);
bool vmw_cmd_describe(const void *buf, u32 *size, char const **cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * IRQs and wating - vmwgfx_irq.c
 */

<<<<<<< HEAD
extern irqreturn_t vmw_irq_handler(DRM_IRQ_ARGS);
extern int vmw_wait_seqno(struct vmw_private *dev_priv, bool lazy,
			     uint32_t seqno, bool interruptible,
			     unsigned long timeout);
extern void vmw_irq_preinstall(struct drm_device *dev);
extern int vmw_irq_postinstall(struct drm_device *dev);
=======
extern int vmw_irq_install(struct vmw_private *dev_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void vmw_irq_uninstall(struct drm_device *dev);
extern bool vmw_seqno_passed(struct vmw_private *dev_priv,
				uint32_t seqno);
extern int vmw_fallback_wait(struct vmw_private *dev_priv,
			     bool lazy,
			     bool fifo_idle,
			     uint32_t seqno,
			     bool interruptible,
			     unsigned long timeout);
<<<<<<< HEAD
extern void vmw_update_seqno(struct vmw_private *dev_priv,
				struct vmw_fifo_state *fifo_state);
=======
extern void vmw_update_seqno(struct vmw_private *dev_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void vmw_seqno_waiter_add(struct vmw_private *dev_priv);
extern void vmw_seqno_waiter_remove(struct vmw_private *dev_priv);
extern void vmw_goal_waiter_add(struct vmw_private *dev_priv);
extern void vmw_goal_waiter_remove(struct vmw_private *dev_priv);
<<<<<<< HEAD

/**
 * Rudimentary fence-like objects currently used only for throttling -
 * vmwgfx_marker.c
 */

extern void vmw_marker_queue_init(struct vmw_marker_queue *queue);
extern void vmw_marker_queue_takedown(struct vmw_marker_queue *queue);
extern int vmw_marker_push(struct vmw_marker_queue *queue,
			  uint32_t seqno);
extern int vmw_marker_pull(struct vmw_marker_queue *queue,
			  uint32_t signaled_seqno);
extern int vmw_wait_lag(struct vmw_private *dev_priv,
			struct vmw_marker_queue *queue, uint32_t us);

/**
 * Kernel framebuffer - vmwgfx_fb.c
 */

int vmw_fb_init(struct vmw_private *vmw_priv);
int vmw_fb_close(struct vmw_private *dev_priv);
int vmw_fb_off(struct vmw_private *vmw_priv);
int vmw_fb_on(struct vmw_private *vmw_priv);
=======
extern void vmw_generic_waiter_add(struct vmw_private *dev_priv, u32 flag,
				   int *waiter_count);
extern void vmw_generic_waiter_remove(struct vmw_private *dev_priv,
				      u32 flag, int *waiter_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Kernel modesetting - vmwgfx_kms.c
 */

int vmw_kms_init(struct vmw_private *dev_priv);
int vmw_kms_close(struct vmw_private *dev_priv);
<<<<<<< HEAD
int vmw_kms_save_vga(struct vmw_private *vmw_priv);
int vmw_kms_restore_vga(struct vmw_private *vmw_priv);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int vmw_kms_cursor_bypass_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
void vmw_kms_cursor_post_execbuf(struct vmw_private *dev_priv);
void vmw_kms_cursor_snoop(struct vmw_surface *srf,
			  struct ttm_object_file *tfile,
			  struct ttm_buffer_object *bo,
			  SVGA3dCmdHeader *header);
int vmw_kms_write_svga(struct vmw_private *vmw_priv,
		       unsigned width, unsigned height, unsigned pitch,
		       unsigned bpp, unsigned depth);
<<<<<<< HEAD
void vmw_kms_idle_workqueues(struct vmw_master *vmaster);
bool vmw_kms_validate_mode_vram(struct vmw_private *dev_priv,
				uint32_t pitch,
				uint32_t height);
u32 vmw_get_vblank_counter(struct drm_device *dev, int crtc);
int vmw_enable_vblank(struct drm_device *dev, int crtc);
void vmw_disable_vblank(struct drm_device *dev, int crtc);
=======
bool vmw_kms_validate_mode_vram(struct vmw_private *dev_priv,
				uint32_t pitch,
				uint32_t height);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int vmw_kms_present(struct vmw_private *dev_priv,
		    struct drm_file *file_priv,
		    struct vmw_framebuffer *vfb,
		    struct vmw_surface *surface,
		    uint32_t sid, int32_t destX, int32_t destY,
		    struct drm_vmw_rect *clips,
		    uint32_t num_clips);
<<<<<<< HEAD
int vmw_kms_readback(struct vmw_private *dev_priv,
		     struct drm_file *file_priv,
		     struct vmw_framebuffer *vfb,
		     struct drm_vmw_fence_rep __user *user_fence_rep,
		     struct drm_vmw_rect *clips,
		     uint32_t num_clips);
int vmw_kms_update_layout_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
=======
int vmw_kms_update_layout_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
void vmw_kms_legacy_hotspot_clear(struct vmw_private *dev_priv);
int vmw_kms_suspend(struct drm_device *dev);
int vmw_kms_resume(struct drm_device *dev);
void vmw_kms_lost_device(struct drm_device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int vmw_dumb_create(struct drm_file *file_priv,
		    struct drm_device *dev,
		    struct drm_mode_create_dumb *args);
<<<<<<< HEAD

int vmw_dumb_map_offset(struct drm_file *file_priv,
			struct drm_device *dev, uint32_t handle,
			uint64_t *offset);
int vmw_dumb_destroy(struct drm_file *file_priv,
		     struct drm_device *dev,
		     uint32_t handle);
=======
extern int vmw_resource_pin(struct vmw_resource *res, bool interruptible);
extern void vmw_resource_unpin(struct vmw_resource *res);
extern enum vmw_res_type vmw_res_type(const struct vmw_resource *res);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * Overlay control - vmwgfx_overlay.c
 */

int vmw_overlay_init(struct vmw_private *dev_priv);
int vmw_overlay_close(struct vmw_private *dev_priv);
int vmw_overlay_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv);
<<<<<<< HEAD
int vmw_overlay_stop_all(struct vmw_private *dev_priv);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int vmw_overlay_resume_all(struct vmw_private *dev_priv);
int vmw_overlay_pause_all(struct vmw_private *dev_priv);
int vmw_overlay_claim(struct vmw_private *dev_priv, uint32_t *out);
int vmw_overlay_unref(struct vmw_private *dev_priv, uint32_t stream_id);
int vmw_overlay_num_overlays(struct vmw_private *dev_priv);
int vmw_overlay_num_free_overlays(struct vmw_private *dev_priv);

/**
 * GMR Id manager
 */

<<<<<<< HEAD
extern const struct ttm_mem_type_manager_func vmw_gmrid_manager_func;
=======
int vmw_gmrid_man_init(struct vmw_private *dev_priv, int type);
void vmw_gmrid_man_fini(struct vmw_private *dev_priv, int type);

/**
 * System memory manager
 */
int vmw_sys_man_init(struct vmw_private *dev_priv);
void vmw_sys_man_fini(struct vmw_private *dev_priv);

/**
 * Prime - vmwgfx_prime.c
 */

extern const struct dma_buf_ops vmw_prime_dmabuf_ops;
extern int vmw_prime_fd_to_handle(struct drm_device *dev,
				  struct drm_file *file_priv,
				  int fd, u32 *handle);
extern int vmw_prime_handle_to_fd(struct drm_device *dev,
				  struct drm_file *file_priv,
				  uint32_t handle, uint32_t flags,
				  int *prime_fd);
struct drm_gem_object *vmw_prime_import_sg_table(struct drm_device *dev,
						 struct dma_buf_attachment *attach,
						 struct sg_table *table);

/*
 * MemoryOBject management -  vmwgfx_mob.c
 */
struct vmw_mob;
extern int vmw_mob_bind(struct vmw_private *dev_priv, struct vmw_mob *mob,
			const struct vmw_sg_table *vsgt,
			unsigned long num_data_pages, int32_t mob_id);
extern void vmw_mob_unbind(struct vmw_private *dev_priv,
			   struct vmw_mob *mob);
extern void vmw_mob_destroy(struct vmw_mob *mob);
extern struct vmw_mob *vmw_mob_create(unsigned long data_pages);
extern int vmw_otables_setup(struct vmw_private *dev_priv);
extern void vmw_otables_takedown(struct vmw_private *dev_priv);

/*
 * Context management - vmwgfx_context.c
 */

extern const struct vmw_user_resource_conv *user_context_converter;

extern int vmw_context_define_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
extern int vmw_extended_context_define_ioctl(struct drm_device *dev, void *data,
					     struct drm_file *file_priv);
extern int vmw_context_destroy_ioctl(struct drm_device *dev, void *data,
				     struct drm_file *file_priv);
extern struct list_head *vmw_context_binding_list(struct vmw_resource *ctx);
extern struct vmw_cmdbuf_res_manager *
vmw_context_res_man(struct vmw_resource *ctx);
extern struct vmw_resource *vmw_context_cotable(struct vmw_resource *ctx,
						SVGACOTableType cotable_type);
struct vmw_ctx_binding_state;
extern struct vmw_ctx_binding_state *
vmw_context_binding_state(struct vmw_resource *ctx);
extern void vmw_dx_context_scrub_cotables(struct vmw_resource *ctx,
					  bool readback);
extern int vmw_context_bind_dx_query(struct vmw_resource *ctx_res,
				     struct vmw_bo *mob);
extern struct vmw_bo *
vmw_context_get_dx_query_mob(struct vmw_resource *ctx_res);


/*
 * Surface management - vmwgfx_surface.c
 */

extern const struct vmw_user_resource_conv *user_surface_converter;

extern int vmw_surface_destroy_ioctl(struct drm_device *dev, void *data,
				     struct drm_file *file_priv);
extern int vmw_surface_define_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
extern int vmw_surface_reference_ioctl(struct drm_device *dev, void *data,
				       struct drm_file *file_priv);
extern int vmw_gb_surface_define_ioctl(struct drm_device *dev, void *data,
				       struct drm_file *file_priv);
extern int vmw_gb_surface_reference_ioctl(struct drm_device *dev, void *data,
					  struct drm_file *file_priv);
extern int vmw_gb_surface_define_ext_ioctl(struct drm_device *dev,
					   void *data,
					   struct drm_file *file_priv);
extern int vmw_gb_surface_reference_ext_ioctl(struct drm_device *dev,
					      void *data,
					      struct drm_file *file_priv);

int vmw_gb_surface_define(struct vmw_private *dev_priv,
			  const struct vmw_surface_metadata *req,
			  struct vmw_surface **srf_out);

/*
 * Shader management - vmwgfx_shader.c
 */

extern const struct vmw_user_resource_conv *user_shader_converter;

extern int vmw_shader_define_ioctl(struct drm_device *dev, void *data,
				   struct drm_file *file_priv);
extern int vmw_shader_destroy_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
extern int vmw_compat_shader_add(struct vmw_private *dev_priv,
				 struct vmw_cmdbuf_res_manager *man,
				 u32 user_key, const void *bytecode,
				 SVGA3dShaderType shader_type,
				 size_t size,
				 struct list_head *list);
extern int vmw_shader_remove(struct vmw_cmdbuf_res_manager *man,
			     u32 user_key, SVGA3dShaderType shader_type,
			     struct list_head *list);
extern int vmw_dx_shader_add(struct vmw_cmdbuf_res_manager *man,
			     struct vmw_resource *ctx,
			     u32 user_key,
			     SVGA3dShaderType shader_type,
			     struct list_head *list);
extern void vmw_dx_shader_cotable_list_scrub(struct vmw_private *dev_priv,
					     struct list_head *list,
					     bool readback);

extern struct vmw_resource *
vmw_shader_lookup(struct vmw_cmdbuf_res_manager *man,
		  u32 user_key, SVGA3dShaderType shader_type);

/*
 * Streamoutput management
 */
struct vmw_resource *
vmw_dx_streamoutput_lookup(struct vmw_cmdbuf_res_manager *man,
			   u32 user_key);
int vmw_dx_streamoutput_add(struct vmw_cmdbuf_res_manager *man,
			    struct vmw_resource *ctx,
			    SVGA3dStreamOutputId user_key,
			    struct list_head *list);
void vmw_dx_streamoutput_set_size(struct vmw_resource *res, u32 size);
int vmw_dx_streamoutput_remove(struct vmw_cmdbuf_res_manager *man,
			       SVGA3dStreamOutputId user_key,
			       struct list_head *list);
void vmw_dx_streamoutput_cotable_list_scrub(struct vmw_private *dev_priv,
					    struct list_head *list,
					    bool readback);

/*
 * Command buffer managed resources - vmwgfx_cmdbuf_res.c
 */

extern struct vmw_cmdbuf_res_manager *
vmw_cmdbuf_res_man_create(struct vmw_private *dev_priv);
extern void vmw_cmdbuf_res_man_destroy(struct vmw_cmdbuf_res_manager *man);
extern struct vmw_resource *
vmw_cmdbuf_res_lookup(struct vmw_cmdbuf_res_manager *man,
		      enum vmw_cmdbuf_res_type res_type,
		      u32 user_key);
extern void vmw_cmdbuf_res_revert(struct list_head *list);
extern void vmw_cmdbuf_res_commit(struct list_head *list);
extern int vmw_cmdbuf_res_add(struct vmw_cmdbuf_res_manager *man,
			      enum vmw_cmdbuf_res_type res_type,
			      u32 user_key,
			      struct vmw_resource *res,
			      struct list_head *list);
extern int vmw_cmdbuf_res_remove(struct vmw_cmdbuf_res_manager *man,
				 enum vmw_cmdbuf_res_type res_type,
				 u32 user_key,
				 struct list_head *list,
				 struct vmw_resource **res);

/*
 * COTable management - vmwgfx_cotable.c
 */
extern const SVGACOTableType vmw_cotable_scrub_order[];
extern struct vmw_resource *vmw_cotable_alloc(struct vmw_private *dev_priv,
					      struct vmw_resource *ctx,
					      u32 type);
extern int vmw_cotable_notify(struct vmw_resource *res, int id);
extern int vmw_cotable_scrub(struct vmw_resource *res, bool readback);
extern void vmw_cotable_add_resource(struct vmw_resource *ctx,
				     struct list_head *head);

/*
 * Command buffer managerment vmwgfx_cmdbuf.c
 */
struct vmw_cmdbuf_man;
struct vmw_cmdbuf_header;

extern struct vmw_cmdbuf_man *
vmw_cmdbuf_man_create(struct vmw_private *dev_priv);
extern int vmw_cmdbuf_set_pool_size(struct vmw_cmdbuf_man *man, size_t size);
extern void vmw_cmdbuf_remove_pool(struct vmw_cmdbuf_man *man);
extern void vmw_cmdbuf_man_destroy(struct vmw_cmdbuf_man *man);
extern int vmw_cmdbuf_idle(struct vmw_cmdbuf_man *man, bool interruptible,
			   unsigned long timeout);
extern void *vmw_cmdbuf_reserve(struct vmw_cmdbuf_man *man, size_t size,
				int ctx_id, bool interruptible,
				struct vmw_cmdbuf_header *header);
extern void vmw_cmdbuf_commit(struct vmw_cmdbuf_man *man, size_t size,
			      struct vmw_cmdbuf_header *header,
			      bool flush);
extern void *vmw_cmdbuf_alloc(struct vmw_cmdbuf_man *man,
			      size_t size, bool interruptible,
			      struct vmw_cmdbuf_header **p_header);
extern void vmw_cmdbuf_header_free(struct vmw_cmdbuf_header *header);
extern int vmw_cmdbuf_cur_flush(struct vmw_cmdbuf_man *man,
				bool interruptible);
extern void vmw_cmdbuf_irqthread(struct vmw_cmdbuf_man *man);

/* CPU blit utilities - vmwgfx_blit.c */

/**
 * struct vmw_diff_cpy - CPU blit information structure
 *
 * @rect: The output bounding box rectangle.
 * @line: The current line of the blit.
 * @line_offset: Offset of the current line segment.
 * @cpp: Bytes per pixel (granularity information).
 * @memcpy: Which memcpy function to use.
 */
struct vmw_diff_cpy {
	struct drm_rect rect;
	size_t line;
	size_t line_offset;
	int cpp;
	void (*do_cpy)(struct vmw_diff_cpy *diff, u8 *dest, const u8 *src,
		       size_t n);
};

#define VMW_CPU_BLIT_INITIALIZER {	\
	.do_cpy = vmw_memcpy,		\
}

#define VMW_CPU_BLIT_DIFF_INITIALIZER(_cpp) {	  \
	.line = 0,				  \
	.line_offset = 0,			  \
	.rect = { .x1 = INT_MAX/2,		  \
		  .y1 = INT_MAX/2,		  \
		  .x2 = INT_MIN/2,		  \
		  .y2 = INT_MIN/2		  \
	},					  \
	.cpp = _cpp,				  \
	.do_cpy = vmw_diff_memcpy,		  \
}

void vmw_diff_memcpy(struct vmw_diff_cpy *diff, u8 *dest, const u8 *src,
		     size_t n);

void vmw_memcpy(struct vmw_diff_cpy *diff, u8 *dest, const u8 *src, size_t n);

int vmw_bo_cpu_blit(struct ttm_buffer_object *dst,
		    u32 dst_offset, u32 dst_stride,
		    struct ttm_buffer_object *src,
		    u32 src_offset, u32 src_stride,
		    u32 w, u32 h,
		    struct vmw_diff_cpy *diff);

/* Host messaging -vmwgfx_msg.c: */
void vmw_disable_backdoor(void);
int vmw_host_get_guestinfo(const char *guest_info_param,
			   char *buffer, size_t *length);
__printf(1, 2) int vmw_host_printf(const char *fmt, ...);
int vmw_msg_ioctl(struct drm_device *dev, void *data,
		  struct drm_file *file_priv);

/* Host mksGuestStats -vmwgfx_msg.c: */
int vmw_mksstat_get_kern_slot(pid_t pid, struct vmw_private *dev_priv);

int vmw_mksstat_reset_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv);
int vmw_mksstat_add_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv);
int vmw_mksstat_remove_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv);
int vmw_mksstat_remove_all(struct vmw_private *dev_priv);

/* VMW logging */

/**
 * VMW_DEBUG_USER - Debug output for user-space debugging.
 *
 * @fmt: printf() like format string.
 *
 * This macro is for logging user-space error and debugging messages for e.g.
 * command buffer execution errors due to malformed commands, invalid context,
 * etc.
 */
#define VMW_DEBUG_USER(fmt, ...)                                              \
	DRM_DEBUG_DRIVER(fmt, ##__VA_ARGS__)

/* Resource dirtying - vmwgfx_page_dirty.c */
void vmw_bo_dirty_scan(struct vmw_bo *vbo);
int vmw_bo_dirty_add(struct vmw_bo *vbo);
void vmw_bo_dirty_transfer_to_res(struct vmw_resource *res);
void vmw_bo_dirty_clear_res(struct vmw_resource *res);
void vmw_bo_dirty_release(struct vmw_bo *vbo);
void vmw_bo_dirty_unmap(struct vmw_bo *vbo,
			pgoff_t start, pgoff_t end);
vm_fault_t vmw_bo_vm_fault(struct vm_fault *vmf);
vm_fault_t vmw_bo_vm_mkwrite(struct vm_fault *vmf);


/**
 * VMW_DEBUG_KMS - Debug output for kernel mode-setting
 *
 * This macro is for debugging vmwgfx mode-setting code.
 */
#define VMW_DEBUG_KMS(fmt, ...)                                               \
	DRM_DEBUG_DRIVER(fmt, ##__VA_ARGS__)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Inline helper functions
 */

static inline void vmw_surface_unreference(struct vmw_surface **srf)
{
	struct vmw_surface *tmp_srf = *srf;
	struct vmw_resource *res = &tmp_srf->res;
	*srf = NULL;

	vmw_resource_unreference(&res);
}

static inline struct vmw_surface *vmw_surface_reference(struct vmw_surface *srf)
{
	(void) vmw_resource_reference(&srf->res);
	return srf;
}

<<<<<<< HEAD
static inline void vmw_dmabuf_unreference(struct vmw_dma_buffer **buf)
{
	struct vmw_dma_buffer *tmp_buf = *buf;
	struct ttm_buffer_object *bo = &tmp_buf->base;
	*buf = NULL;

	ttm_bo_unref(&bo);
}

static inline struct vmw_dma_buffer *vmw_dmabuf_reference(struct vmw_dma_buffer *buf)
{
	if (ttm_bo_reference(&buf->base))
		return buf;
	return NULL;
}

static inline struct ttm_mem_global *vmw_mem_glob(struct vmw_private *dev_priv)
{
	return (struct ttm_mem_global *) dev_priv->mem_global_ref.object;
}
=======
static inline void vmw_fifo_resource_inc(struct vmw_private *dev_priv)
{
	atomic_inc(&dev_priv->num_fifo_resources);
}

static inline void vmw_fifo_resource_dec(struct vmw_private *dev_priv)
{
	atomic_dec(&dev_priv->num_fifo_resources);
}

/**
 * vmw_fifo_mem_read - Perform a MMIO read from the fifo memory
 *
 * @fifo_reg: The fifo register to read from
 *
 * This function is intended to be equivalent to ioread32() on
 * memremap'd memory, but without byteswapping.
 */
static inline u32 vmw_fifo_mem_read(struct vmw_private *vmw, uint32 fifo_reg)
{
	BUG_ON(vmw_is_svga_v3(vmw));
	return READ_ONCE(*(vmw->fifo_mem + fifo_reg));
}

/**
 * vmw_fifo_mem_write - Perform a MMIO write to volatile memory
 *
 * @addr: The fifo register to write to
 *
 * This function is intended to be equivalent to iowrite32 on
 * memremap'd memory, but without byteswapping.
 */
static inline void vmw_fifo_mem_write(struct vmw_private *vmw, u32 fifo_reg,
				      u32 value)
{
	BUG_ON(vmw_is_svga_v3(vmw));
	WRITE_ONCE(*(vmw->fifo_mem + fifo_reg), value);
}

static inline u32 vmw_fence_read(struct vmw_private *dev_priv)
{
	u32 fence;
	if (vmw_is_svga_v3(dev_priv))
		fence = vmw_read(dev_priv, SVGA_REG_FENCE);
	else
		fence = vmw_fifo_mem_read(dev_priv, SVGA_FIFO_FENCE);
	return fence;
}

static inline void vmw_fence_write(struct vmw_private *dev_priv,
				  u32 fence)
{
	BUG_ON(vmw_is_svga_v3(dev_priv));
	vmw_fifo_mem_write(dev_priv, SVGA_FIFO_FENCE, fence);
}

static inline u32 vmw_irq_status_read(struct vmw_private *vmw)
{
	u32 status;
	if (vmw_is_svga_v3(vmw))
		status = vmw_read(vmw, SVGA_REG_IRQ_STATUS);
	else
		status = inl(vmw->io_start + SVGA_IRQSTATUS_PORT);
	return status;
}

static inline void vmw_irq_status_write(struct vmw_private *vmw,
					uint32 status)
{
	if (vmw_is_svga_v3(vmw))
		vmw_write(vmw, SVGA_REG_IRQ_STATUS, status);
	else
		outl(status, vmw->io_start + SVGA_IRQSTATUS_PORT);
}

static inline bool vmw_has_fences(struct vmw_private *vmw)
{
	if ((vmw->capabilities & (SVGA_CAP_COMMAND_BUFFERS |
				  SVGA_CAP_CMD_BUFFERS_2)) != 0)
		return true;
	return (vmw_fifo_caps(vmw) & SVGA_FIFO_CAP_FENCE) != 0;
}

static inline bool vmw_shadertype_is_valid(enum vmw_sm_type shader_model,
					   u32 shader_type)
{
	SVGA3dShaderType max_allowed = SVGA3D_SHADERTYPE_PREDX_MAX;

	if (shader_model >= VMW_SM_5)
		max_allowed = SVGA3D_SHADERTYPE_MAX;
	else if (shader_model >= VMW_SM_4)
		max_allowed = SVGA3D_SHADERTYPE_DX10_MAX;
	return shader_type >= SVGA3D_SHADERTYPE_MIN && shader_type < max_allowed;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
