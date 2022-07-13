/* i915_drv.h -- Private header for the I915 driver -*- linux-c -*-
 */
/*
 *
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _I915_DRV_H_
#define _I915_DRV_H_

<<<<<<< HEAD
#include "i915_reg.h"
#include "intel_bios.h"
#include "intel_ringbuffer.h"
#include <linux/io-mapping.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <drm/intel-gtt.h>
#include <linux/backlight.h>

/* General customization:
 */

#define DRIVER_AUTHOR		"Tungsten Graphics, Inc."

#define DRIVER_NAME		"i915"
#define DRIVER_DESC		"Intel Graphics"
#define DRIVER_DATE		"20080730"

enum pipe {
	PIPE_A = 0,
	PIPE_B,
	PIPE_C,
	I915_MAX_PIPES
};
#define pipe_name(p) ((p) + 'A')

enum plane {
	PLANE_A = 0,
	PLANE_B,
	PLANE_C,
};
#define plane_name(p) ((p) + 'A')

#define I915_GEM_GPU_DOMAINS	(~(I915_GEM_DOMAIN_CPU | I915_GEM_DOMAIN_GTT))

#define for_each_pipe(p) for ((p) = 0; (p) < dev_priv->num_pipe; (p)++)

/* Interface history:
 *
 * 1.1: Original.
 * 1.2: Add Power Management
 * 1.3: Add vblank support
 * 1.4: Fix cmdbuffer path, add heap destroy
 * 1.5: Add vblank pipe configuration
 * 1.6: - New ioctl for scheduling buffer swaps on vertical blank
 *      - Support vertical blank on secondary display pipe
 */
#define DRIVER_MAJOR		1
#define DRIVER_MINOR		6
#define DRIVER_PATCHLEVEL	0

#define WATCH_COHERENCY	0
#define WATCH_LISTS	0

#define I915_GEM_PHYS_CURSOR_0 1
#define I915_GEM_PHYS_CURSOR_1 2
#define I915_GEM_PHYS_OVERLAY_REGS 3
#define I915_MAX_PHYS_OBJECT (I915_GEM_PHYS_OVERLAY_REGS)

struct drm_i915_gem_phys_object {
	int id;
	struct page **page_list;
	drm_dma_handle_t *handle;
	struct drm_i915_gem_object *cur_obj;
};

struct mem_block {
	struct mem_block *next;
	struct mem_block *prev;
	int start;
	int size;
	struct drm_file *file_priv; /* NULL: free, -1: heap, other: real files */
};

struct opregion_header;
struct opregion_acpi;
struct opregion_swsci;
struct opregion_asle;
struct drm_i915_private;

struct intel_opregion {
	struct opregion_header *header;
	struct opregion_acpi *acpi;
	struct opregion_swsci *swsci;
	struct opregion_asle *asle;
	void *vbt;
	u32 __iomem *lid_state;
};
#define OPREGION_SIZE            (8*1024)

struct intel_overlay;
struct intel_overlay_error_state;

struct drm_i915_master_private {
	drm_local_map_t *sarea;
	struct _drm_i915_sarea *sarea_priv;
};
#define I915_FENCE_REG_NONE -1
#define I915_MAX_NUM_FENCES 16
/* 16 fences + sign bit for FENCE_REG_NONE */
#define I915_MAX_NUM_FENCE_BITS 5

struct drm_i915_fence_reg {
	struct list_head lru_list;
	struct drm_i915_gem_object *obj;
	uint32_t setup_seqno;
	int pin_count;
};

struct sdvo_device_mapping {
	u8 initialized;
	u8 dvo_port;
	u8 slave_addr;
	u8 dvo_wiring;
	u8 i2c_pin;
	u8 ddc_pin;
};

struct intel_display_error_state;

struct drm_i915_error_state {
	u32 eir;
	u32 pgtbl_er;
	u32 pipestat[I915_MAX_PIPES];
	u32 tail[I915_NUM_RINGS];
	u32 head[I915_NUM_RINGS];
	u32 ipeir[I915_NUM_RINGS];
	u32 ipehr[I915_NUM_RINGS];
	u32 instdone[I915_NUM_RINGS];
	u32 acthd[I915_NUM_RINGS];
	u32 semaphore_mboxes[I915_NUM_RINGS][I915_NUM_RINGS - 1];
	/* our own tracking of ring head and tail */
	u32 cpu_ring_head[I915_NUM_RINGS];
	u32 cpu_ring_tail[I915_NUM_RINGS];
	u32 error; /* gen6+ */
	u32 instpm[I915_NUM_RINGS];
	u32 instps[I915_NUM_RINGS];
	u32 instdone1;
	u32 seqno[I915_NUM_RINGS];
	u64 bbaddr;
	u32 fault_reg[I915_NUM_RINGS];
	u32 done_reg;
	u32 faddr[I915_NUM_RINGS];
	u64 fence[I915_MAX_NUM_FENCES];
	struct timeval time;
	struct drm_i915_error_ring {
		struct drm_i915_error_object {
			int page_count;
			u32 gtt_offset;
			u32 *pages[0];
		} *ringbuffer, *batchbuffer;
		struct drm_i915_error_request {
			long jiffies;
			u32 seqno;
			u32 tail;
		} *requests;
		int num_requests;
	} ring[I915_NUM_RINGS];
	struct drm_i915_error_buffer {
		u32 size;
		u32 name;
		u32 seqno;
		u32 gtt_offset;
		u32 read_domains;
		u32 write_domain;
		s32 fence_reg:I915_MAX_NUM_FENCE_BITS;
		s32 pinned:2;
		u32 tiling:2;
		u32 dirty:1;
		u32 purgeable:1;
		s32 ring:4;
		u32 cache_level:2;
	} *active_bo, *pinned_bo;
	u32 active_bo_count, pinned_bo_count;
	struct intel_overlay_error_state *overlay;
	struct intel_display_error_state *display;
};

struct drm_i915_display_funcs {
	void (*dpms)(struct drm_crtc *crtc, int mode);
	bool (*fbc_enabled)(struct drm_device *dev);
	void (*enable_fbc)(struct drm_crtc *crtc, unsigned long interval);
	void (*disable_fbc)(struct drm_device *dev);
	int (*get_display_clock_speed)(struct drm_device *dev);
	int (*get_fifo_size)(struct drm_device *dev, int plane);
	void (*update_wm)(struct drm_device *dev);
	void (*update_sprite_wm)(struct drm_device *dev, int pipe,
				 uint32_t sprite_width, int pixel_size);
	int (*crtc_mode_set)(struct drm_crtc *crtc,
			     struct drm_display_mode *mode,
			     struct drm_display_mode *adjusted_mode,
			     int x, int y,
			     struct drm_framebuffer *old_fb);
	void (*write_eld)(struct drm_connector *connector,
			  struct drm_crtc *crtc);
	void (*fdi_link_train)(struct drm_crtc *crtc);
	void (*init_clock_gating)(struct drm_device *dev);
	void (*init_pch_clock_gating)(struct drm_device *dev);
	int (*queue_flip)(struct drm_device *dev, struct drm_crtc *crtc,
			  struct drm_framebuffer *fb,
			  struct drm_i915_gem_object *obj);
	int (*update_plane)(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			    int x, int y);
	void (*force_wake_get)(struct drm_i915_private *dev_priv);
	void (*force_wake_put)(struct drm_i915_private *dev_priv);
	/* clock updates for mode set */
	/* cursor updates */
	/* render clock increase/decrease */
	/* display clock increase/decrease */
	/* pll clock increase/decrease */
};

struct intel_device_info {
	u8 gen;
	u8 is_mobile:1;
	u8 is_i85x:1;
	u8 is_i915g:1;
	u8 is_i945gm:1;
	u8 is_g33:1;
	u8 need_gfx_hws:1;
	u8 is_g4x:1;
	u8 is_pineview:1;
	u8 is_broadwater:1;
	u8 is_crestline:1;
	u8 is_ivybridge:1;
	u8 has_force_wake:1;
	u8 has_fbc:1;
	u8 has_pipe_cxsr:1;
	u8 has_hotplug:1;
	u8 cursor_needs_physical:1;
	u8 has_overlay:1;
	u8 overlay_needs_physical:1;
	u8 supports_tv:1;
	u8 has_bsd_ring:1;
	u8 has_blt_ring:1;
	u8 has_llc:1;
};

#define I915_PPGTT_PD_ENTRIES 512
#define I915_PPGTT_PT_ENTRIES 1024
struct i915_hw_ppgtt {
	unsigned num_pd_entries;
	struct page **pt_pages;
	uint32_t pd_offset;
	dma_addr_t *pt_dma_addr;
	dma_addr_t scratch_page_dma_addr;
};

enum no_fbc_reason {
	FBC_NO_OUTPUT, /* no outputs enabled to compress */
	FBC_STOLEN_TOO_SMALL, /* not enough space to hold compressed buffers */
	FBC_UNSUPPORTED_MODE, /* interlace or doublescanned mode */
	FBC_MODE_TOO_LARGE, /* mode too large for compression */
	FBC_BAD_PLANE, /* fbc not supported on plane */
	FBC_NOT_TILED, /* buffer not tiled */
	FBC_MULTIPLE_PIPES, /* more than one pipe active */
	FBC_MODULE_PARAM,
};

enum intel_pch {
	PCH_IBX,	/* Ibexpeak PCH */
	PCH_CPT,	/* Cougarpoint PCH */
};

#define QUIRK_PIPEA_FORCE (1<<0)
#define QUIRK_LVDS_SSC_DISABLE (1<<1)
#define QUIRK_INVERT_BRIGHTNESS (1<<2)
#define QUIRK_NO_PCH_PWM_ENABLE (1<<3)

struct intel_fbdev;
struct intel_fbc_work;

struct intel_gmbus {
	struct i2c_adapter adapter;
	bool force_bit;
	bool has_gpio;
	u32 reg0;
	u32 gpio_reg;
	struct i2c_algo_bit_data bit_algo;
	struct drm_i915_private *dev_priv;
};

typedef struct drm_i915_private {
	struct drm_device *dev;

	const struct intel_device_info *info;

	int has_gem;
	int relative_constants_mode;

	void __iomem *regs;
	/** gt_fifo_count and the subsequent register write are synchronized
	 * with dev->struct_mutex. */
	unsigned gt_fifo_count;
	/** forcewake_count is protected by gt_lock */
	unsigned forcewake_count;
	/** gt_lock is also taken in irq contexts. */
	struct spinlock gt_lock;

	struct intel_gmbus *gmbus;

	/** gmbus_mutex protects against concurrent usage of the single hw gmbus
	 * controller on different i2c buses. */
	struct mutex gmbus_mutex;

	struct pci_dev *bridge_dev;
	struct intel_ring_buffer ring[I915_NUM_RINGS];
	uint32_t next_seqno;

	drm_dma_handle_t *status_page_dmah;
	uint32_t counter;
	drm_local_map_t hws_map;
	struct drm_i915_gem_object *pwrctx;
	struct drm_i915_gem_object *renderctx;

	struct resource mch_res;

	unsigned int cpp;
	int back_offset;
	int front_offset;
	int current_page;
	int page_flipping;

	atomic_t irq_received;

	/* protects the irq masks */
	spinlock_t irq_lock;
	/** Cached value of IMR to avoid reads in updating the bitfield */
	u32 pipestat[2];
	u32 irq_mask;
	u32 gt_irq_mask;
	u32 pch_irq_mask;

	u32 hotplug_supported_mask;
	struct work_struct hotplug_work;

	int tex_lru_log_granularity;
	int allow_batchbuffer;
	unsigned int sr01, adpa, ppcr, dvob, dvoc, lvds;
	int vblank_pipe;
	int num_pipe;

	/* For hangcheck timer */
#define DRM_I915_HANGCHECK_PERIOD 1500 /* in ms */
	struct timer_list hangcheck_timer;
	int hangcheck_count;
	uint32_t last_acthd;
	uint32_t last_acthd_bsd;
	uint32_t last_acthd_blt;
	uint32_t last_instdone;
	uint32_t last_instdone1;

	unsigned long cfb_size;
	unsigned int cfb_fb;
	enum plane cfb_plane;
	int cfb_y;
	struct intel_fbc_work *fbc_work;

	struct intel_opregion opregion;

	/* overlay */
	struct intel_overlay *overlay;
	bool sprite_scaling_enabled;

	/* LVDS info */
	int backlight_level;  /* restore backlight to this value */
	bool backlight_enabled;
	struct drm_display_mode *lfp_lvds_vbt_mode; /* if any */
	struct drm_display_mode *sdvo_lvds_vbt_mode; /* if any */

	/* Feature bits from the VBIOS */
	unsigned int int_tv_support:1;
	unsigned int lvds_dither:1;
	unsigned int lvds_vbt:1;
	unsigned int int_crt_support:1;
	unsigned int lvds_use_ssc:1;
	unsigned int display_clock_mode:1;
	int lvds_ssc_freq;
	unsigned int bios_lvds_val; /* initial [PCH_]LVDS reg val in VBIOS */
	unsigned int lvds_val; /* used for checking LVDS channel mode */
	struct {
		int rate;
		int lanes;
		int preemphasis;
		int vswing;

		bool initialized;
		bool support;
		int bpp;
		struct edp_power_seq pps;
	} edp;
	bool no_aux_handshake;

	struct notifier_block lid_notifier;

	int crt_ddc_pin;
	struct drm_i915_fence_reg fence_regs[I915_MAX_NUM_FENCES]; /* assume 965 */
	int fence_reg_start; /* 4 if userland hasn't ioctl'd us yet */
	int num_fence_regs; /* 8 on pre-965, 16 otherwise */

	unsigned int fsb_freq, mem_freq, is_ddr3;

	spinlock_t error_lock;
	struct drm_i915_error_state *first_error;
	struct work_struct error_work;
	struct completion error_completion;
	struct workqueue_struct *wq;

	/* Display functions */
	struct drm_i915_display_funcs display;

	/* PCH chipset type */
	enum intel_pch pch_type;

	unsigned long quirks;

	/* Register state */
	bool modeset_on_lid;
	u8 saveLBB;
	u32 saveDSPACNTR;
	u32 saveDSPBCNTR;
	u32 saveDSPARB;
	u32 saveHWS;
	u32 savePIPEACONF;
	u32 savePIPEBCONF;
	u32 savePIPEASRC;
	u32 savePIPEBSRC;
	u32 saveFPA0;
	u32 saveFPA1;
	u32 saveDPLL_A;
	u32 saveDPLL_A_MD;
	u32 saveHTOTAL_A;
	u32 saveHBLANK_A;
	u32 saveHSYNC_A;
	u32 saveVTOTAL_A;
	u32 saveVBLANK_A;
	u32 saveVSYNC_A;
	u32 saveBCLRPAT_A;
	u32 saveTRANSACONF;
	u32 saveTRANS_HTOTAL_A;
	u32 saveTRANS_HBLANK_A;
	u32 saveTRANS_HSYNC_A;
	u32 saveTRANS_VTOTAL_A;
	u32 saveTRANS_VBLANK_A;
	u32 saveTRANS_VSYNC_A;
	u32 savePIPEASTAT;
	u32 saveDSPASTRIDE;
	u32 saveDSPASIZE;
	u32 saveDSPAPOS;
	u32 saveDSPAADDR;
	u32 saveDSPASURF;
	u32 saveDSPATILEOFF;
	u32 savePFIT_PGM_RATIOS;
	u32 saveBLC_HIST_CTL;
	u32 saveBLC_PWM_CTL;
	u32 saveBLC_PWM_CTL2;
	u32 saveBLC_CPU_PWM_CTL;
	u32 saveBLC_CPU_PWM_CTL2;
	u32 saveFPB0;
	u32 saveFPB1;
	u32 saveDPLL_B;
	u32 saveDPLL_B_MD;
	u32 saveHTOTAL_B;
	u32 saveHBLANK_B;
	u32 saveHSYNC_B;
	u32 saveVTOTAL_B;
	u32 saveVBLANK_B;
	u32 saveVSYNC_B;
	u32 saveBCLRPAT_B;
	u32 saveTRANSBCONF;
	u32 saveTRANS_HTOTAL_B;
	u32 saveTRANS_HBLANK_B;
	u32 saveTRANS_HSYNC_B;
	u32 saveTRANS_VTOTAL_B;
	u32 saveTRANS_VBLANK_B;
	u32 saveTRANS_VSYNC_B;
	u32 savePIPEBSTAT;
	u32 saveDSPBSTRIDE;
	u32 saveDSPBSIZE;
	u32 saveDSPBPOS;
	u32 saveDSPBADDR;
	u32 saveDSPBSURF;
	u32 saveDSPBTILEOFF;
	u32 saveVGA0;
	u32 saveVGA1;
	u32 saveVGA_PD;
	u32 saveVGACNTRL;
	u32 saveADPA;
	u32 saveLVDS;
	u32 savePP_ON_DELAYS;
	u32 savePP_OFF_DELAYS;
	u32 saveDVOA;
	u32 saveDVOB;
	u32 saveDVOC;
	u32 savePP_ON;
	u32 savePP_OFF;
	u32 savePP_CONTROL;
	u32 savePP_DIVISOR;
	u32 savePFIT_CONTROL;
	u32 save_palette_a[256];
	u32 save_palette_b[256];
	u32 saveDPFC_CB_BASE;
	u32 saveFBC_CFB_BASE;
	u32 saveFBC_LL_BASE;
	u32 saveFBC_CONTROL;
	u32 saveFBC_CONTROL2;
	u32 saveIER;
	u32 saveIIR;
	u32 saveIMR;
	u32 saveDEIER;
	u32 saveDEIMR;
	u32 saveGTIER;
	u32 saveGTIMR;
	u32 saveFDI_RXA_IMR;
	u32 saveFDI_RXB_IMR;
	u32 saveCACHE_MODE_0;
	u32 saveMI_ARB_STATE;
	u32 saveSWF0[16];
	u32 saveSWF1[16];
	u32 saveSWF2[3];
	u8 saveMSR;
	u8 saveSR[8];
	u8 saveGR[25];
	u8 saveAR_INDEX;
	u8 saveAR[21];
	u8 saveDACMASK;
	u8 saveCR[37];
	uint64_t saveFENCE[I915_MAX_NUM_FENCES];
	u32 saveCURACNTR;
	u32 saveCURAPOS;
	u32 saveCURABASE;
	u32 saveCURBCNTR;
	u32 saveCURBPOS;
	u32 saveCURBBASE;
	u32 saveCURSIZE;
	u32 saveDP_B;
	u32 saveDP_C;
	u32 saveDP_D;
	u32 savePIPEA_GMCH_DATA_M;
	u32 savePIPEB_GMCH_DATA_M;
	u32 savePIPEA_GMCH_DATA_N;
	u32 savePIPEB_GMCH_DATA_N;
	u32 savePIPEA_DP_LINK_M;
	u32 savePIPEB_DP_LINK_M;
	u32 savePIPEA_DP_LINK_N;
	u32 savePIPEB_DP_LINK_N;
	u32 saveFDI_RXA_CTL;
	u32 saveFDI_TXA_CTL;
	u32 saveFDI_RXB_CTL;
	u32 saveFDI_TXB_CTL;
	u32 savePFA_CTL_1;
	u32 savePFB_CTL_1;
	u32 savePFA_WIN_SZ;
	u32 savePFB_WIN_SZ;
	u32 savePFA_WIN_POS;
	u32 savePFB_WIN_POS;
	u32 savePCH_DREF_CONTROL;
	u32 saveDISP_ARB_CTL;
	u32 savePIPEA_DATA_M1;
	u32 savePIPEA_DATA_N1;
	u32 savePIPEA_LINK_M1;
	u32 savePIPEA_LINK_N1;
	u32 savePIPEB_DATA_M1;
	u32 savePIPEB_DATA_N1;
	u32 savePIPEB_LINK_M1;
	u32 savePIPEB_LINK_N1;
	u32 saveMCHBAR_RENDER_STANDBY;
	u32 savePCH_PORT_HOTPLUG;

	struct {
		/** Bridge to intel-gtt-ko */
		const struct intel_gtt *gtt;
		/** Memory allocator for GTT stolen memory */
		struct drm_mm stolen;
		/** Memory allocator for GTT */
		struct drm_mm gtt_space;
		/** List of all objects in gtt_space. Used to restore gtt
		 * mappings on resume */
		struct list_head gtt_list;

		/** Usable portion of the GTT for GEM */
		unsigned long gtt_start;
		unsigned long gtt_mappable_end;
		unsigned long gtt_end;

		struct io_mapping *gtt_mapping;
		int gtt_mtrr;

		/** PPGTT used for aliasing the PPGTT with the GTT */
		struct i915_hw_ppgtt *aliasing_ppgtt;

		struct shrinker inactive_shrinker;

		/**
		 * List of objects currently involved in rendering.
		 *
		 * Includes buffers having the contents of their GPU caches
		 * flushed, not necessarily primitives.  last_rendering_seqno
		 * represents when the rendering involved will be completed.
		 *
		 * A reference is held on the buffer while on this list.
		 */
		struct list_head active_list;

		/**
		 * List of objects which are not in the ringbuffer but which
		 * still have a write_domain which needs to be flushed before
		 * unbinding.
		 *
		 * last_rendering_seqno is 0 while an object is in this list.
		 *
		 * A reference is held on the buffer while on this list.
		 */
		struct list_head flushing_list;

		/**
		 * LRU list of objects which are not in the ringbuffer and
		 * are ready to unbind, but are still in the GTT.
		 *
		 * last_rendering_seqno is 0 while an object is in this list.
		 *
		 * A reference is not held on the buffer while on this list,
		 * as merely being GTT-bound shouldn't prevent its being
		 * freed, and we'll pull it off the list in the free path.
		 */
		struct list_head inactive_list;

		/**
		 * LRU list of objects which are not in the ringbuffer but
		 * are still pinned in the GTT.
		 */
		struct list_head pinned_list;

		/** LRU list of objects with fence regs on them. */
		struct list_head fence_list;

		/**
		 * List of objects currently pending being freed.
		 *
		 * These objects are no longer in use, but due to a signal
		 * we were prevented from freeing them at the appointed time.
		 */
		struct list_head deferred_free_list;

		/**
		 * We leave the user IRQ off as much as possible,
		 * but this means that requests will finish and never
		 * be retired once the system goes idle. Set a timer to
		 * fire periodically while the ring is running. When it
		 * fires, go retire requests.
		 */
		struct delayed_work retire_work;

		/**
		 * Are we in a non-interruptible section of code like
		 * modesetting?
		 */
		bool interruptible;

		/**
		 * Flag if the X Server, and thus DRM, is not currently in
		 * control of the device.
		 *
		 * This is set between LeaveVT and EnterVT.  It needs to be
		 * replaced with a semaphore.  It also needs to be
		 * transitioned away from for kernel modesetting.
		 */
		int suspended;

		/**
		 * Flag if the hardware appears to be wedged.
		 *
		 * This is set when attempts to idle the device timeout.
		 * It prevents command submission from occurring and makes
		 * every pending request fail
		 */
		atomic_t wedged;

		/** Bit 6 swizzling required for X tiling */
		uint32_t bit_6_swizzle_x;
		/** Bit 6 swizzling required for Y tiling */
		uint32_t bit_6_swizzle_y;

		/* storage for physical objects */
		struct drm_i915_gem_phys_object *phys_objs[I915_MAX_PHYS_OBJECT];

		/* accounting, useful for userland debugging */
		size_t gtt_total;
		size_t mappable_gtt_total;
		size_t object_memory;
		u32 object_count;
	} mm;
	struct sdvo_device_mapping sdvo_mappings[2];
	/* indicate whether the LVDS_BORDER should be enabled or not */
	unsigned int lvds_border_bits;
	/* Panel fitter placement and size for Ironlake+ */
	u32 pch_pf_pos, pch_pf_size;

	struct drm_crtc *plane_to_crtc_mapping[3];
	struct drm_crtc *pipe_to_crtc_mapping[3];
	wait_queue_head_t pending_flip_queue;
	bool flip_pending_is_done;

	/* Reclocking support */
	bool render_reclock_avail;
	bool lvds_downclock_avail;
	/* indicates the reduced downclock for LVDS*/
	int lvds_downclock;
	struct work_struct idle_work;
	struct timer_list idle_timer;
	bool busy;
	u16 orig_clock;
	int child_dev_num;
	struct child_device_config *child_dev;
	struct drm_connector *int_lvds_connector;
	struct drm_connector *int_edp_connector;

	bool mchbar_need_disable;

	struct work_struct rps_work;
	spinlock_t rps_lock;
	u32 pm_iir;

	u8 cur_delay;
	u8 min_delay;
	u8 max_delay;
	u8 fmax;
	u8 fstart;

	u64 last_count1;
	unsigned long last_time1;
	unsigned long chipset_power;
	u64 last_count2;
	struct timespec last_time2;
	unsigned long gfx_power;
	int c_m;
	int r_t;
	u8 corr;
	spinlock_t *mchdev_lock;

	enum no_fbc_reason no_fbc_reason;

	struct drm_mm_node *compressed_fb;
	struct drm_mm_node *compressed_llb;

	unsigned long last_gpu_reset;

	/* list of fbdev register on this device */
	struct intel_fbdev *fbdev;

	struct backlight_device *backlight;

	struct drm_property *broadcast_rgb_property;
	struct drm_property *force_audio_property;
} drm_i915_private_t;

enum hdmi_force_audio {
	HDMI_AUDIO_OFF_DVI = -2,	/* no aux data for HDMI-DVI converter */
	HDMI_AUDIO_OFF,			/* force turn off HDMI audio */
	HDMI_AUDIO_AUTO,		/* trust EDID */
	HDMI_AUDIO_ON,			/* force turn on HDMI audio */
};

enum i915_cache_level {
	I915_CACHE_NONE,
	I915_CACHE_LLC,
	I915_CACHE_LLC_MLC, /* gen6+ */
};

struct drm_i915_gem_object {
	struct drm_gem_object base;

	/** Current space allocated to this object in the GTT, if any. */
	struct drm_mm_node *gtt_space;
	struct list_head gtt_list;

	/** This object's place on the active/flushing/inactive lists */
	struct list_head ring_list;
	struct list_head mm_list;
	/** This object's place on GPU write list */
	struct list_head gpu_write_list;
	/** This object's place in the batchbuffer or on the eviction list */
	struct list_head exec_list;

	/**
	 * This is set if the object is on the active or flushing lists
	 * (has pending rendering), and is not set if it's on inactive (ready
	 * to be unbound).
	 */
	unsigned int active:1;

	/**
	 * This is set if the object has been written to since last bound
	 * to the GTT
	 */
	unsigned int dirty:1;

	/**
	 * This is set if the object has been written to since the last
	 * GPU flush.
	 */
	unsigned int pending_gpu_write:1;

	/**
	 * Fence register bits (if any) for this object.  Will be set
	 * as needed when mapped into the GTT.
	 * Protected by dev->struct_mutex.
	 */
	signed int fence_reg:I915_MAX_NUM_FENCE_BITS;

	/**
	 * Advice: are the backing pages purgeable?
	 */
	unsigned int madv:2;

	/**
	 * Current tiling mode for the object.
	 */
	unsigned int tiling_mode:2;
	unsigned int tiling_changed:1;

	/** How many users have pinned this object in GTT space. The following
	 * users can each hold at most one reference: pwrite/pread, pin_ioctl
	 * (via user_pin_count), execbuffer (objects are not allowed multiple
	 * times for the same batchbuffer), and the framebuffer code. When
	 * switching/pageflipping, the framebuffer code has at most two buffers
	 * pinned per crtc.
	 *
	 * In the worst case this is 1 + 1 + 1 + 2*2 = 7. That would fit into 3
	 * bits with absolutely no headroom. So use 4 bits. */
	unsigned int pin_count:4;
#define DRM_I915_GEM_OBJECT_MAX_PIN_COUNT 0xf

	/**
	 * Is the object at the current location in the gtt mappable and
	 * fenceable? Used to avoid costly recalculations.
	 */
	unsigned int map_and_fenceable:1;

	/**
	 * Whether the current gtt mapping needs to be mappable (and isn't just
	 * mappable by accident). Track pin and fault separate for a more
	 * accurate mappable working set.
	 */
	unsigned int fault_mappable:1;
	unsigned int pin_mappable:1;

	/*
	 * Is the GPU currently using a fence to access this buffer,
	 */
	unsigned int pending_fenced_gpu_access:1;
	unsigned int fenced_gpu_access:1;

	unsigned int cache_level:2;

	unsigned int has_aliasing_ppgtt_mapping:1;

	struct page **pages;

	/**
	 * DMAR support
	 */
	struct scatterlist *sg_list;
	int num_sg;

	/**
	 * Used for performing relocations during execbuffer insertion.
	 */
	struct hlist_node exec_node;
	unsigned long exec_handle;
	struct drm_i915_gem_exec_object2 *exec_entry;

	/**
	 * Current offset of the object in GTT space.
	 *
	 * This is the same as gtt_space->start
	 */
	uint32_t gtt_offset;

	/** Breadcrumb of last rendering to the buffer. */
	uint32_t last_rendering_seqno;
	struct intel_ring_buffer *ring;

	/** Breadcrumb of last fenced GPU access to the buffer. */
	uint32_t last_fenced_seqno;
	struct intel_ring_buffer *last_fenced_ring;

	/** Current tiling stride for the object, if it's tiled. */
	uint32_t stride;

	/** Record of address bit 17 of each page at last unbind. */
	unsigned long *bit_17;


	/**
	 * If present, while GEM_DOMAIN_CPU is in the read domain this array
	 * flags which individual pages are valid.
	 */
	uint8_t *page_cpu_valid;

	/** User space pin count and filp owning the pin */
	uint32_t user_pin_count;
	struct drm_file *pin_filp;

	/** for phy allocated objects */
	struct drm_i915_gem_phys_object *phys_obj;

	/**
	 * Number of crtcs where this object is currently the fb, but
	 * will be page flipped away on the next vblank.  When it
	 * reaches 0, dev_priv->pending_flip_queue will be woken up.
	 */
	atomic_t pending_flip;
};

#define to_intel_bo(x) container_of(x, struct drm_i915_gem_object, base)

/**
 * Request queue structure.
 *
 * The request queue allows us to note sequence numbers that have been emitted
 * and may be associated with active buffers to be retired.
 *
 * By keeping this list, we can avoid having to do questionable
 * sequence-number comparisons on buffer last_rendering_seqnos, and associate
 * an emission time with seqnos for tracking how far ahead of the GPU we are.
 */
struct drm_i915_gem_request {
	/** On Which ring this request was generated */
	struct intel_ring_buffer *ring;

	/** GEM sequence number associated with this request. */
	uint32_t seqno;

	/** Postion in the ringbuffer of the end of the request */
	u32 tail;

	/** Time at which this request was emitted, in jiffies. */
	unsigned long emitted_jiffies;

	/** global list entry for this request */
	struct list_head list;

	struct drm_i915_file_private *file_priv;
	/** file_priv list entry for this request */
	struct list_head client_list;
};

struct drm_i915_file_private {
	struct {
		struct spinlock lock;
		struct list_head request_list;
	} mm;
};

#define INTEL_INFO(dev)	(((struct drm_i915_private *) (dev)->dev_private)->info)

#define IS_I830(dev)		((dev)->pci_device == 0x3577)
#define IS_845G(dev)		((dev)->pci_device == 0x2562)
#define IS_I85X(dev)		(INTEL_INFO(dev)->is_i85x)
#define IS_I865G(dev)		((dev)->pci_device == 0x2572)
#define IS_I915G(dev)		(INTEL_INFO(dev)->is_i915g)
#define IS_I915GM(dev)		((dev)->pci_device == 0x2592)
#define IS_I945G(dev)		((dev)->pci_device == 0x2772)
#define IS_I945GM(dev)		(INTEL_INFO(dev)->is_i945gm)
#define IS_BROADWATER(dev)	(INTEL_INFO(dev)->is_broadwater)
#define IS_CRESTLINE(dev)	(INTEL_INFO(dev)->is_crestline)
#define IS_GM45(dev)		((dev)->pci_device == 0x2A42)
#define IS_G4X(dev)		(INTEL_INFO(dev)->is_g4x)
#define IS_PINEVIEW_G(dev)	((dev)->pci_device == 0xa001)
#define IS_PINEVIEW_M(dev)	((dev)->pci_device == 0xa011)
#define IS_PINEVIEW(dev)	(INTEL_INFO(dev)->is_pineview)
#define IS_G33(dev)		(INTEL_INFO(dev)->is_g33)
#define IS_IRONLAKE_D(dev)	((dev)->pci_device == 0x0042)
#define IS_IRONLAKE_M(dev)	((dev)->pci_device == 0x0046)
#define IS_IVYBRIDGE(dev)	(INTEL_INFO(dev)->is_ivybridge)
#define IS_MOBILE(dev)		(INTEL_INFO(dev)->is_mobile)

/*
 * The genX designation typically refers to the render engine, so render
 * capability related checks should use IS_GEN, while display and other checks
 * have their own (e.g. HAS_PCH_SPLIT for ILK+ display, IS_foo for particular
 * chips, etc.).
 */
#define IS_GEN2(dev)	(INTEL_INFO(dev)->gen == 2)
#define IS_GEN3(dev)	(INTEL_INFO(dev)->gen == 3)
#define IS_GEN4(dev)	(INTEL_INFO(dev)->gen == 4)
#define IS_GEN5(dev)	(INTEL_INFO(dev)->gen == 5)
#define IS_GEN6(dev)	(INTEL_INFO(dev)->gen == 6)
#define IS_GEN7(dev)	(INTEL_INFO(dev)->gen == 7)

#define HAS_BSD(dev)            (INTEL_INFO(dev)->has_bsd_ring)
#define HAS_BLT(dev)            (INTEL_INFO(dev)->has_blt_ring)
#define HAS_LLC(dev)            (INTEL_INFO(dev)->has_llc)
#define I915_NEED_GFX_HWS(dev)	(INTEL_INFO(dev)->need_gfx_hws)

#define HAS_ALIASING_PPGTT(dev)	(INTEL_INFO(dev)->gen >=6)

#define HAS_OVERLAY(dev)		(INTEL_INFO(dev)->has_overlay)
#define OVERLAY_NEEDS_PHYSICAL(dev)	(INTEL_INFO(dev)->overlay_needs_physical)
=======
#include <uapi/drm/i915_drm.h>

#include <linux/pm_qos.h>

#include <drm/ttm/ttm_device.h>

#include "display/intel_display_limits.h"
#include "display/intel_display_core.h"

#include "gem/i915_gem_context_types.h"
#include "gem/i915_gem_shrinker.h"
#include "gem/i915_gem_stolen.h"

#include "gt/intel_engine.h"
#include "gt/intel_gt_types.h"
#include "gt/intel_region_lmem.h"
#include "gt/intel_workarounds.h"
#include "gt/uc/intel_uc.h"

#include "soc/intel_pch.h"

#include "i915_drm_client.h"
#include "i915_gem.h"
#include "i915_gpu_error.h"
#include "i915_params.h"
#include "i915_perf_types.h"
#include "i915_scheduler.h"
#include "i915_utils.h"
#include "intel_device_info.h"
#include "intel_memory_region.h"
#include "intel_runtime_pm.h"
#include "intel_step.h"
#include "intel_uncore.h"

struct drm_i915_clock_gating_funcs;
struct vlv_s0ix_state;
struct intel_pxp;

#define GEM_QUIRK_PIN_SWIZZLED_PAGES	BIT(0)

/* Data Stolen Memory (DSM) aka "i915 stolen memory" */
struct i915_dsm {
	/*
	 * The start and end of DSM which we can optionally use to create GEM
	 * objects backed by stolen memory.
	 *
	 * Note that usable_size tells us exactly how much of this we are
	 * actually allowed to use, given that some portion of it is in fact
	 * reserved for use by hardware functions.
	 */
	struct resource stolen;

	/*
	 * Reserved portion of DSM.
	 */
	struct resource reserved;

	/*
	 * Total size minus reserved ranges.
	 *
	 * DSM is segmented in hardware with different portions offlimits to
	 * certain functions.
	 *
	 * The drm_mm is initialised to the total accessible range, as found
	 * from the PCI config. On Broadwell+, this is further restricted to
	 * avoid the first page! The upper end of DSM is reserved for hardware
	 * functions and similarly removed from the accessible range.
	 */
	resource_size_t usable_size;
};

struct i915_suspend_saved_registers {
	u32 saveDSPARB;
	u32 saveSWF0[16];
	u32 saveSWF1[16];
	u32 saveSWF3[3];
	u16 saveGCDGMBUS;
};

#define MAX_L3_SLICES 2
struct intel_l3_parity {
	u32 *remap_info[MAX_L3_SLICES];
	struct work_struct error_work;
	int which_slice;
};

struct i915_gem_mm {
	/*
	 * Shortcut for the stolen region. This points to either
	 * INTEL_REGION_STOLEN_SMEM for integrated platforms, or
	 * INTEL_REGION_STOLEN_LMEM for discrete, or NULL if the device doesn't
	 * support stolen.
	 */
	struct intel_memory_region *stolen_region;
	/** Memory allocator for GTT stolen memory */
	struct drm_mm stolen;
	/** Protects the usage of the GTT stolen memory allocator. This is
	 * always the inner lock when overlapping with struct_mutex. */
	struct mutex stolen_lock;

	/* Protects bound_list/unbound_list and #drm_i915_gem_object.mm.link */
	spinlock_t obj_lock;

	/**
	 * List of objects which are purgeable.
	 */
	struct list_head purge_list;

	/**
	 * List of objects which have allocated pages and are shrinkable.
	 */
	struct list_head shrink_list;

	/**
	 * List of objects which are pending destruction.
	 */
	struct llist_head free_list;
	struct work_struct free_work;
	/**
	 * Count of objects pending destructions. Used to skip needlessly
	 * waiting on an RCU barrier if no objects are waiting to be freed.
	 */
	atomic_t free_count;

	/**
	 * tmpfs instance used for shmem backed objects
	 */
	struct vfsmount *gemfs;

	struct intel_memory_region *regions[INTEL_REGION_UNKNOWN];

	struct notifier_block oom_notifier;
	struct notifier_block vmap_notifier;
	struct shrinker *shrinker;

	/* shrinker accounting, also useful for userland debugging */
	u64 shrink_memory;
	u32 shrink_count;
};

struct i915_virtual_gpu {
	struct mutex lock; /* serialises sending of g2v_notify command pkts */
	bool active;
	u32 caps;
	u32 *initial_mmio;
	u8 *initial_cfg_space;
	struct list_head entry;
};

struct i915_selftest_stash {
	atomic_t counter;
	struct ida mock_region_instances;
};

struct drm_i915_private {
	struct drm_device drm;

	struct intel_display display;

	/* FIXME: Device release actions should all be moved to drmm_ */
	bool do_release;

	/* i915 device parameters */
	struct i915_params params;

	const struct intel_device_info *__info; /* Use INTEL_INFO() to access. */
	struct intel_runtime_info __runtime; /* Use RUNTIME_INFO() to access. */
	struct intel_driver_caps caps;

	struct i915_dsm dsm;

	struct intel_uncore uncore;
	struct intel_uncore_mmio_debug mmio_debug;

	struct i915_virtual_gpu vgpu;

	struct intel_gvt *gvt;

	struct {
		struct pci_dev *pdev;
		struct resource mch_res;
		bool mchbar_need_disable;
	} gmch;

	/*
	 * Chaining user engines happens in multiple stages, starting with a
	 * simple lock-less linked list created by intel_engine_add_user(),
	 * which later gets sorted and converted to an intermediate regular
	 * list, just to be converted once again to its final rb tree structure
	 * in intel_engines_driver_register().
	 *
	 * Make sure to use the right iterator helper, depending on if the code
	 * in question runs before or after intel_engines_driver_register() --
	 * for_each_uabi_engine() can only be used afterwards!
	 */
	union {
		struct llist_head uabi_engines_llist;
		struct list_head uabi_engines_list;
		struct rb_root uabi_engines;
	};
	unsigned int engine_uabi_class_count[I915_LAST_UABI_ENGINE_CLASS + 1];

	/* protects the irq masks */
	spinlock_t irq_lock;

	bool display_irqs_enabled;

	/* Sideband mailbox protection */
	struct mutex sb_lock;
	struct pm_qos_request sb_qos;

	/** Cached value of IMR to avoid reads in updating the bitfield */
	union {
		u32 irq_mask;
		u32 de_irq_mask[I915_MAX_PIPES];
	};
	u32 pipestat_irq_mask[I915_MAX_PIPES];

	bool preserve_bios_swizzle;

	unsigned int fsb_freq, mem_freq, is_ddr3;
	unsigned int skl_preferred_vco_freq;

	unsigned int max_dotclk_freq;
	unsigned int hpll_freq;
	unsigned int czclk_freq;

	/**
	 * wq - Driver workqueue for GEM.
	 *
	 * NOTE: Work items scheduled here are not allowed to grab any modeset
	 * locks, for otherwise the flushing done in the pageflip code will
	 * result in deadlocks.
	 */
	struct workqueue_struct *wq;

	/**
	 * unordered_wq - internal workqueue for unordered work
	 *
	 * This workqueue should be used for all unordered work
	 * scheduling within i915, which used to be scheduled on the
	 * system_wq before moving to a driver instance due
	 * deprecation of flush_scheduled_work().
	 */
	struct workqueue_struct *unordered_wq;

	/* pm private clock gating functions */
	const struct drm_i915_clock_gating_funcs *clock_gating_funcs;

	/* PCH chipset type */
	enum intel_pch pch_type;
	unsigned short pch_id;

	unsigned long gem_quirks;

	struct i915_gem_mm mm;

	struct intel_l3_parity l3_parity;

	/*
	 * edram size in MB.
	 * Cannot be determined by PCIID. You must always read a register.
	 */
	u32 edram_size_mb;

	struct i915_gpu_error gpu_error;

	u32 suspend_count;
	struct i915_suspend_saved_registers regfile;
	struct vlv_s0ix_state *vlv_s0ix_state;

	struct dram_info {
		bool wm_lv_0_adjust_needed;
		u8 num_channels;
		bool symmetric_memory;
		enum intel_dram_type {
			INTEL_DRAM_UNKNOWN,
			INTEL_DRAM_DDR3,
			INTEL_DRAM_DDR4,
			INTEL_DRAM_LPDDR3,
			INTEL_DRAM_LPDDR4,
			INTEL_DRAM_DDR5,
			INTEL_DRAM_LPDDR5,
		} type;
		u8 num_qgv_points;
		u8 num_psf_gv_points;
	} dram_info;

	struct intel_runtime_pm runtime_pm;

	struct i915_perf perf;

	struct i915_hwmon *hwmon;

	struct intel_gt *gt[I915_MAX_GT];

	struct kobject *sysfs_gt;

	/* Quick lookup of media GT (current platforms only have one) */
	struct intel_gt *media_gt;

	struct {
		struct i915_gem_contexts {
			spinlock_t lock; /* locks list */
			struct list_head list;
		} contexts;

		/*
		 * We replace the local file with a global mappings as the
		 * backing storage for the mmap is on the device and not
		 * on the struct file, and we do not want to prolong the
		 * lifetime of the local fd. To minimise the number of
		 * anonymous inodes we create, we use a global singleton to
		 * share the global mapping.
		 */
		struct file *mmap_singleton;
	} gem;

	struct intel_pxp *pxp;

	/* For i915gm/i945gm vblank irq workaround */
	u8 vblank_enabled;

	bool irq_enabled;

	struct i915_pmu pmu;

	/* The TTM device structure. */
	struct ttm_device bdev;

	I915_SELFTEST_DECLARE(struct i915_selftest_stash selftest;)

	/*
	 * NOTE: This is the dri1/ums dungeon, don't add stuff here. Your patch
	 * will be rejected. Instead look for a better place.
	 */
};

static inline struct drm_i915_private *to_i915(const struct drm_device *dev)
{
	return container_of(dev, struct drm_i915_private, drm);
}

static inline struct drm_i915_private *kdev_to_i915(struct device *kdev)
{
	return dev_get_drvdata(kdev);
}

static inline struct drm_i915_private *pdev_to_i915(struct pci_dev *pdev)
{
	return pci_get_drvdata(pdev);
}

static inline struct intel_gt *to_gt(const struct drm_i915_private *i915)
{
	return i915->gt[0];
}

#define rb_to_uabi_engine(rb) \
	rb_entry_safe(rb, struct intel_engine_cs, uabi_node)

#define for_each_uabi_engine(engine__, i915__) \
	for ((engine__) = rb_to_uabi_engine(rb_first(&(i915__)->uabi_engines));\
	     (engine__); \
	     (engine__) = rb_to_uabi_engine(rb_next(&(engine__)->uabi_node)))

#define INTEL_INFO(i915)	((i915)->__info)
#define RUNTIME_INFO(i915)	(&(i915)->__runtime)
#define DRIVER_CAPS(i915)	(&(i915)->caps)

#define INTEL_DEVID(i915)	(RUNTIME_INFO(i915)->device_id)

#define IP_VER(ver, rel)		((ver) << 8 | (rel))

#define GRAPHICS_VER(i915)		(RUNTIME_INFO(i915)->graphics.ip.ver)
#define GRAPHICS_VER_FULL(i915)		IP_VER(RUNTIME_INFO(i915)->graphics.ip.ver, \
					       RUNTIME_INFO(i915)->graphics.ip.rel)
#define IS_GRAPHICS_VER(i915, from, until) \
	(GRAPHICS_VER(i915) >= (from) && GRAPHICS_VER(i915) <= (until))

#define MEDIA_VER(i915)			(RUNTIME_INFO(i915)->media.ip.ver)
#define MEDIA_VER_FULL(i915)		IP_VER(RUNTIME_INFO(i915)->media.ip.ver, \
					       RUNTIME_INFO(i915)->media.ip.rel)
#define IS_MEDIA_VER(i915, from, until) \
	(MEDIA_VER(i915) >= (from) && MEDIA_VER(i915) <= (until))

#define INTEL_REVID(i915)	(to_pci_dev((i915)->drm.dev)->revision)

#define INTEL_DISPLAY_STEP(__i915) (RUNTIME_INFO(__i915)->step.display_step)
#define INTEL_GRAPHICS_STEP(__i915) (RUNTIME_INFO(__i915)->step.graphics_step)
#define INTEL_MEDIA_STEP(__i915) (RUNTIME_INFO(__i915)->step.media_step)
#define INTEL_BASEDIE_STEP(__i915) (RUNTIME_INFO(__i915)->step.basedie_step)

#define IS_DISPLAY_STEP(__i915, since, until) \
	(drm_WARN_ON(&(__i915)->drm, INTEL_DISPLAY_STEP(__i915) == STEP_NONE), \
	 INTEL_DISPLAY_STEP(__i915) >= (since) && INTEL_DISPLAY_STEP(__i915) < (until))

#define IS_GRAPHICS_STEP(__i915, since, until) \
	(drm_WARN_ON(&(__i915)->drm, INTEL_GRAPHICS_STEP(__i915) == STEP_NONE), \
	 INTEL_GRAPHICS_STEP(__i915) >= (since) && INTEL_GRAPHICS_STEP(__i915) < (until))

#define IS_MEDIA_STEP(__i915, since, until) \
	(drm_WARN_ON(&(__i915)->drm, INTEL_MEDIA_STEP(__i915) == STEP_NONE), \
	 INTEL_MEDIA_STEP(__i915) >= (since) && INTEL_MEDIA_STEP(__i915) < (until))

#define IS_BASEDIE_STEP(__i915, since, until) \
	(drm_WARN_ON(&(__i915)->drm, INTEL_BASEDIE_STEP(__i915) == STEP_NONE), \
	 INTEL_BASEDIE_STEP(__i915) >= (since) && INTEL_BASEDIE_STEP(__i915) < (until))

static __always_inline unsigned int
__platform_mask_index(const struct intel_runtime_info *info,
		      enum intel_platform p)
{
	const unsigned int pbits =
		BITS_PER_TYPE(info->platform_mask[0]) - INTEL_SUBPLATFORM_BITS;

	/* Expand the platform_mask array if this fails. */
	BUILD_BUG_ON(INTEL_MAX_PLATFORMS >
		     pbits * ARRAY_SIZE(info->platform_mask));

	return p / pbits;
}

static __always_inline unsigned int
__platform_mask_bit(const struct intel_runtime_info *info,
		    enum intel_platform p)
{
	const unsigned int pbits =
		BITS_PER_TYPE(info->platform_mask[0]) - INTEL_SUBPLATFORM_BITS;

	return p % pbits + INTEL_SUBPLATFORM_BITS;
}

static inline u32
intel_subplatform(const struct intel_runtime_info *info, enum intel_platform p)
{
	const unsigned int pi = __platform_mask_index(info, p);

	return info->platform_mask[pi] & INTEL_SUBPLATFORM_MASK;
}

static __always_inline bool
IS_PLATFORM(const struct drm_i915_private *i915, enum intel_platform p)
{
	const struct intel_runtime_info *info = RUNTIME_INFO(i915);
	const unsigned int pi = __platform_mask_index(info, p);
	const unsigned int pb = __platform_mask_bit(info, p);

	BUILD_BUG_ON(!__builtin_constant_p(p));

	return info->platform_mask[pi] & BIT(pb);
}

static __always_inline bool
IS_SUBPLATFORM(const struct drm_i915_private *i915,
	       enum intel_platform p, unsigned int s)
{
	const struct intel_runtime_info *info = RUNTIME_INFO(i915);
	const unsigned int pi = __platform_mask_index(info, p);
	const unsigned int pb = __platform_mask_bit(info, p);
	const unsigned int msb = BITS_PER_TYPE(info->platform_mask[0]) - 1;
	const u32 mask = info->platform_mask[pi];

	BUILD_BUG_ON(!__builtin_constant_p(p));
	BUILD_BUG_ON(!__builtin_constant_p(s));
	BUILD_BUG_ON((s) >= INTEL_SUBPLATFORM_BITS);

	/* Shift and test on the MSB position so sign flag can be used. */
	return ((mask << (msb - pb)) & (mask << (msb - s))) & BIT(msb);
}

#define IS_MOBILE(i915)	(INTEL_INFO(i915)->is_mobile)
#define IS_DGFX(i915)   (INTEL_INFO(i915)->is_dgfx)

#define IS_I830(i915)	IS_PLATFORM(i915, INTEL_I830)
#define IS_I845G(i915)	IS_PLATFORM(i915, INTEL_I845G)
#define IS_I85X(i915)	IS_PLATFORM(i915, INTEL_I85X)
#define IS_I865G(i915)	IS_PLATFORM(i915, INTEL_I865G)
#define IS_I915G(i915)	IS_PLATFORM(i915, INTEL_I915G)
#define IS_I915GM(i915)	IS_PLATFORM(i915, INTEL_I915GM)
#define IS_I945G(i915)	IS_PLATFORM(i915, INTEL_I945G)
#define IS_I945GM(i915)	IS_PLATFORM(i915, INTEL_I945GM)
#define IS_I965G(i915)	IS_PLATFORM(i915, INTEL_I965G)
#define IS_I965GM(i915)	IS_PLATFORM(i915, INTEL_I965GM)
#define IS_G45(i915)	IS_PLATFORM(i915, INTEL_G45)
#define IS_GM45(i915)	IS_PLATFORM(i915, INTEL_GM45)
#define IS_G4X(i915)	(IS_G45(i915) || IS_GM45(i915))
#define IS_PINEVIEW(i915)	IS_PLATFORM(i915, INTEL_PINEVIEW)
#define IS_G33(i915)	IS_PLATFORM(i915, INTEL_G33)
#define IS_IRONLAKE(i915)	IS_PLATFORM(i915, INTEL_IRONLAKE)
#define IS_IRONLAKE_M(i915) \
	(IS_PLATFORM(i915, INTEL_IRONLAKE) && IS_MOBILE(i915))
#define IS_SANDYBRIDGE(i915) IS_PLATFORM(i915, INTEL_SANDYBRIDGE)
#define IS_IVYBRIDGE(i915)	IS_PLATFORM(i915, INTEL_IVYBRIDGE)
#define IS_IVB_GT1(i915)	(IS_IVYBRIDGE(i915) && \
				 INTEL_INFO(i915)->gt == 1)
#define IS_VALLEYVIEW(i915)	IS_PLATFORM(i915, INTEL_VALLEYVIEW)
#define IS_CHERRYVIEW(i915)	IS_PLATFORM(i915, INTEL_CHERRYVIEW)
#define IS_HASWELL(i915)	IS_PLATFORM(i915, INTEL_HASWELL)
#define IS_BROADWELL(i915)	IS_PLATFORM(i915, INTEL_BROADWELL)
#define IS_SKYLAKE(i915)	IS_PLATFORM(i915, INTEL_SKYLAKE)
#define IS_BROXTON(i915)	IS_PLATFORM(i915, INTEL_BROXTON)
#define IS_KABYLAKE(i915)	IS_PLATFORM(i915, INTEL_KABYLAKE)
#define IS_GEMINILAKE(i915)	IS_PLATFORM(i915, INTEL_GEMINILAKE)
#define IS_COFFEELAKE(i915)	IS_PLATFORM(i915, INTEL_COFFEELAKE)
#define IS_COMETLAKE(i915)	IS_PLATFORM(i915, INTEL_COMETLAKE)
#define IS_ICELAKE(i915)	IS_PLATFORM(i915, INTEL_ICELAKE)
#define IS_JASPERLAKE(i915)	IS_PLATFORM(i915, INTEL_JASPERLAKE)
#define IS_ELKHARTLAKE(i915)	IS_PLATFORM(i915, INTEL_ELKHARTLAKE)
#define IS_TIGERLAKE(i915)	IS_PLATFORM(i915, INTEL_TIGERLAKE)
#define IS_ROCKETLAKE(i915)	IS_PLATFORM(i915, INTEL_ROCKETLAKE)
#define IS_DG1(i915)        IS_PLATFORM(i915, INTEL_DG1)
#define IS_ALDERLAKE_S(i915) IS_PLATFORM(i915, INTEL_ALDERLAKE_S)
#define IS_ALDERLAKE_P(i915) IS_PLATFORM(i915, INTEL_ALDERLAKE_P)
#define IS_XEHPSDV(i915) IS_PLATFORM(i915, INTEL_XEHPSDV)
#define IS_DG2(i915)	IS_PLATFORM(i915, INTEL_DG2)
#define IS_PONTEVECCHIO(i915) IS_PLATFORM(i915, INTEL_PONTEVECCHIO)
#define IS_METEORLAKE(i915) IS_PLATFORM(i915, INTEL_METEORLAKE)
#define IS_LUNARLAKE(i915) 0

#define IS_DG2_G10(i915) \
	IS_SUBPLATFORM(i915, INTEL_DG2, INTEL_SUBPLATFORM_G10)
#define IS_DG2_G11(i915) \
	IS_SUBPLATFORM(i915, INTEL_DG2, INTEL_SUBPLATFORM_G11)
#define IS_DG2_G12(i915) \
	IS_SUBPLATFORM(i915, INTEL_DG2, INTEL_SUBPLATFORM_G12)
#define IS_RAPTORLAKE_S(i915) \
	IS_SUBPLATFORM(i915, INTEL_ALDERLAKE_S, INTEL_SUBPLATFORM_RPL)
#define IS_ALDERLAKE_P_N(i915) \
	IS_SUBPLATFORM(i915, INTEL_ALDERLAKE_P, INTEL_SUBPLATFORM_N)
#define IS_RAPTORLAKE_P(i915) \
	IS_SUBPLATFORM(i915, INTEL_ALDERLAKE_P, INTEL_SUBPLATFORM_RPL)
#define IS_RAPTORLAKE_U(i915) \
	IS_SUBPLATFORM(i915, INTEL_ALDERLAKE_P, INTEL_SUBPLATFORM_RPLU)
#define IS_HASWELL_EARLY_SDV(i915) (IS_HASWELL(i915) && \
				    (INTEL_DEVID(i915) & 0xFF00) == 0x0C00)
#define IS_BROADWELL_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_BROADWELL, INTEL_SUBPLATFORM_ULT)
#define IS_BROADWELL_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_BROADWELL, INTEL_SUBPLATFORM_ULX)
#define IS_BROADWELL_GT3(i915)	(IS_BROADWELL(i915) && \
				 INTEL_INFO(i915)->gt == 3)
#define IS_HASWELL_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_HASWELL, INTEL_SUBPLATFORM_ULT)
#define IS_HASWELL_GT3(i915)	(IS_HASWELL(i915) && \
				 INTEL_INFO(i915)->gt == 3)
#define IS_HASWELL_GT1(i915)	(IS_HASWELL(i915) && \
				 INTEL_INFO(i915)->gt == 1)
/* ULX machines are also considered ULT. */
#define IS_HASWELL_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_HASWELL, INTEL_SUBPLATFORM_ULX)
#define IS_SKYLAKE_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_SKYLAKE, INTEL_SUBPLATFORM_ULT)
#define IS_SKYLAKE_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_SKYLAKE, INTEL_SUBPLATFORM_ULX)
#define IS_KABYLAKE_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_KABYLAKE, INTEL_SUBPLATFORM_ULT)
#define IS_KABYLAKE_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_KABYLAKE, INTEL_SUBPLATFORM_ULX)
#define IS_SKYLAKE_GT2(i915)	(IS_SKYLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 2)
#define IS_SKYLAKE_GT3(i915)	(IS_SKYLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 3)
#define IS_SKYLAKE_GT4(i915)	(IS_SKYLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 4)
#define IS_KABYLAKE_GT2(i915)	(IS_KABYLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 2)
#define IS_KABYLAKE_GT3(i915)	(IS_KABYLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 3)
#define IS_COFFEELAKE_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_COFFEELAKE, INTEL_SUBPLATFORM_ULT)
#define IS_COFFEELAKE_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_COFFEELAKE, INTEL_SUBPLATFORM_ULX)
#define IS_COFFEELAKE_GT2(i915)	(IS_COFFEELAKE(i915) && \
				 INTEL_INFO(i915)->gt == 2)
#define IS_COFFEELAKE_GT3(i915)	(IS_COFFEELAKE(i915) && \
				 INTEL_INFO(i915)->gt == 3)

#define IS_COMETLAKE_ULT(i915) \
	IS_SUBPLATFORM(i915, INTEL_COMETLAKE, INTEL_SUBPLATFORM_ULT)
#define IS_COMETLAKE_ULX(i915) \
	IS_SUBPLATFORM(i915, INTEL_COMETLAKE, INTEL_SUBPLATFORM_ULX)
#define IS_COMETLAKE_GT2(i915)	(IS_COMETLAKE(i915) && \
				 INTEL_INFO(i915)->gt == 2)

#define IS_ICL_WITH_PORT_F(i915) \
	IS_SUBPLATFORM(i915, INTEL_ICELAKE, INTEL_SUBPLATFORM_PORTF)

#define IS_TIGERLAKE_UY(i915) \
	IS_SUBPLATFORM(i915, INTEL_TIGERLAKE, INTEL_SUBPLATFORM_UY)

#define IS_XEHPSDV_GRAPHICS_STEP(__i915, since, until) \
	(IS_XEHPSDV(__i915) && IS_GRAPHICS_STEP(__i915, since, until))

#define IS_PVC_BD_STEP(__i915, since, until) \
	(IS_PONTEVECCHIO(__i915) && \
	 IS_BASEDIE_STEP(__i915, since, until))

#define IS_PVC_CT_STEP(__i915, since, until) \
	(IS_PONTEVECCHIO(__i915) && \
	 IS_GRAPHICS_STEP(__i915, since, until))

#define IS_LP(i915)		(INTEL_INFO(i915)->is_lp)
#define IS_GEN9_LP(i915)	(GRAPHICS_VER(i915) == 9 && IS_LP(i915))
#define IS_GEN9_BC(i915)	(GRAPHICS_VER(i915) == 9 && !IS_LP(i915))

#define __HAS_ENGINE(engine_mask, id) ((engine_mask) & BIT(id))
#define HAS_ENGINE(gt, id) __HAS_ENGINE((gt)->info.engine_mask, id)

#define __ENGINE_INSTANCES_MASK(mask, first, count) ({			\
	unsigned int first__ = (first);					\
	unsigned int count__ = (count);					\
	((mask) & GENMASK(first__ + count__ - 1, first__)) >> first__;	\
})

#define ENGINE_INSTANCES_MASK(gt, first, count) \
	__ENGINE_INSTANCES_MASK((gt)->info.engine_mask, first, count)

#define RCS_MASK(gt) \
	ENGINE_INSTANCES_MASK(gt, RCS0, I915_MAX_RCS)
#define BCS_MASK(gt) \
	ENGINE_INSTANCES_MASK(gt, BCS0, I915_MAX_BCS)
#define VDBOX_MASK(gt) \
	ENGINE_INSTANCES_MASK(gt, VCS0, I915_MAX_VCS)
#define VEBOX_MASK(gt) \
	ENGINE_INSTANCES_MASK(gt, VECS0, I915_MAX_VECS)
#define CCS_MASK(gt) \
	ENGINE_INSTANCES_MASK(gt, CCS0, I915_MAX_CCS)

#define HAS_MEDIA_RATIO_MODE(i915) (INTEL_INFO(i915)->has_media_ratio_mode)

/*
 * The Gen7 cmdparser copies the scanned buffer to the ggtt for execution
 * All later gens can run the final buffer from the ppgtt
 */
#define CMDPARSER_USES_GGTT(i915) (GRAPHICS_VER(i915) == 7)

#define HAS_LLC(i915)	(INTEL_INFO(i915)->has_llc)
#define HAS_SNOOP(i915)	(INTEL_INFO(i915)->has_snoop)
#define HAS_EDRAM(i915)	((i915)->edram_size_mb)
#define HAS_SECURE_BATCHES(i915) (GRAPHICS_VER(i915) < 6)
#define HAS_WT(i915)	HAS_EDRAM(i915)

#define HWS_NEEDS_PHYSICAL(i915)	(INTEL_INFO(i915)->hws_needs_physical)

#define HAS_LOGICAL_RING_CONTEXTS(i915) \
		(INTEL_INFO(i915)->has_logical_ring_contexts)
#define HAS_LOGICAL_RING_ELSQ(i915) \
		(INTEL_INFO(i915)->has_logical_ring_elsq)

#define HAS_EXECLISTS(i915) HAS_LOGICAL_RING_CONTEXTS(i915)

#define INTEL_PPGTT(i915) (RUNTIME_INFO(i915)->ppgtt_type)
#define HAS_PPGTT(i915) \
	(INTEL_PPGTT(i915) != INTEL_PPGTT_NONE)
#define HAS_FULL_PPGTT(i915) \
	(INTEL_PPGTT(i915) >= INTEL_PPGTT_FULL)

#define HAS_PAGE_SIZES(i915, sizes) ({ \
	GEM_BUG_ON((sizes) == 0); \
	((sizes) & ~RUNTIME_INFO(i915)->page_sizes) == 0; \
})

/* Early gen2 have a totally busted CS tlb and require pinned batches. */
#define HAS_BROKEN_CS_TLB(i915)	(IS_I830(i915) || IS_I845G(i915))

#define NEEDS_RC6_CTX_CORRUPTION_WA(i915)	\
	(IS_BROADWELL(i915) || GRAPHICS_VER(i915) == 9)

/* WaRsDisableCoarsePowerGating:skl,cnl */
#define NEEDS_WaRsDisableCoarsePowerGating(i915)			\
	(IS_SKYLAKE_GT3(i915) || IS_SKYLAKE_GT4(i915))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* With the 945 and later, Y tiling got adjusted so that it was 32 128-byte
 * rows, which changed the alignment requirements and fence programming.
 */
<<<<<<< HEAD
#define HAS_128_BYTE_Y_TILING(dev) (!IS_GEN2(dev) && !(IS_I915G(dev) || \
						      IS_I915GM(dev)))
#define SUPPORTS_DIGITAL_OUTPUTS(dev)	(!IS_GEN2(dev) && !IS_PINEVIEW(dev))
#define SUPPORTS_INTEGRATED_HDMI(dev)	(IS_G4X(dev) || IS_GEN5(dev))
#define SUPPORTS_INTEGRATED_DP(dev)	(IS_G4X(dev) || IS_GEN5(dev))
#define SUPPORTS_EDP(dev)		(IS_IRONLAKE_M(dev))
#define SUPPORTS_TV(dev)		(INTEL_INFO(dev)->supports_tv)
#define I915_HAS_HOTPLUG(dev)		 (INTEL_INFO(dev)->has_hotplug)
/* dsparb controlled by hw only */
#define DSPARB_HWCONTROL(dev) (IS_G4X(dev) || IS_IRONLAKE(dev))

#define HAS_FW_BLC(dev) (INTEL_INFO(dev)->gen > 2)
#define HAS_PIPE_CXSR(dev) (INTEL_INFO(dev)->has_pipe_cxsr)
#define I915_HAS_FBC(dev) (INTEL_INFO(dev)->has_fbc)

#define HAS_PCH_SPLIT(dev) (IS_GEN5(dev) || IS_GEN6(dev) || IS_IVYBRIDGE(dev))
#define HAS_PIPE_CONTROL(dev) (INTEL_INFO(dev)->gen >= 5)

#define INTEL_PCH_TYPE(dev) (((struct drm_i915_private *)(dev)->dev_private)->pch_type)
#define HAS_PCH_CPT(dev) (INTEL_PCH_TYPE(dev) == PCH_CPT)
#define HAS_PCH_IBX(dev) (INTEL_PCH_TYPE(dev) == PCH_IBX)

#define HAS_FORCE_WAKE(dev) (INTEL_INFO(dev)->has_force_wake)

#include "i915_trace.h"

/**
 * RC6 is a special power stage which allows the GPU to enter an very
 * low-voltage mode when idle, using down to 0V while at this stage.  This
 * stage is entered automatically when the GPU is idle when RC6 support is
 * enabled, and as soon as new workload arises GPU wakes up automatically as well.
 *
 * There are different RC6 modes available in Intel GPU, which differentiate
 * among each other with the latency required to enter and leave RC6 and
 * voltage consumed by the GPU in different states.
 *
 * The combination of the following flags define which states GPU is allowed
 * to enter, while RC6 is the normal RC6 state, RC6p is the deep RC6, and
 * RC6pp is deepest RC6. Their support by hardware varies according to the
 * GPU, BIOS, chipset and platform. RC6 is usually the safest one and the one
 * which brings the most power savings; deeper states save more power, but
 * require higher latency to switch to and wake up.
 */
#define INTEL_RC6_ENABLE			(1<<0)
#define INTEL_RC6p_ENABLE			(1<<1)
#define INTEL_RC6pp_ENABLE			(1<<2)

extern struct drm_ioctl_desc i915_ioctls[];
extern int i915_max_ioctl;
extern unsigned int i915_fbpercrtc __always_unused;
extern int i915_panel_ignore_lid __read_mostly;
extern unsigned int i915_powersave __read_mostly;
extern int i915_semaphores __read_mostly;
extern unsigned int i915_lvds_downclock __read_mostly;
extern int i915_panel_use_ssc __read_mostly;
extern int i915_vbt_sdvo_panel_type __read_mostly;
extern int i915_enable_rc6 __read_mostly;
extern int i915_enable_fbc __read_mostly;
extern bool i915_enable_hangcheck __read_mostly;
extern int i915_enable_ppgtt __read_mostly;

extern int i915_suspend(struct drm_device *dev, pm_message_t state);
extern int i915_resume(struct drm_device *dev);
extern int i915_master_create(struct drm_device *dev, struct drm_master *master);
extern void i915_master_destroy(struct drm_device *dev, struct drm_master *master);

				/* i915_dma.c */
extern void i915_kernel_lost_context(struct drm_device * dev);
extern int i915_driver_load(struct drm_device *, unsigned long flags);
extern int i915_driver_unload(struct drm_device *);
extern int i915_driver_open(struct drm_device *dev, struct drm_file *file_priv);
extern void i915_driver_lastclose(struct drm_device * dev);
extern void i915_driver_preclose(struct drm_device *dev,
				 struct drm_file *file_priv);
extern void i915_driver_postclose(struct drm_device *dev,
				  struct drm_file *file_priv);
extern int i915_driver_device_is_agp(struct drm_device * dev);
extern long i915_compat_ioctl(struct file *filp, unsigned int cmd,
			      unsigned long arg);
extern int i915_emit_box(struct drm_device *dev,
			 struct drm_clip_rect *box,
			 int DR1, int DR4);
extern int i915_reset(struct drm_device *dev, u8 flags);
extern unsigned long i915_chipset_val(struct drm_i915_private *dev_priv);
extern unsigned long i915_mch_val(struct drm_i915_private *dev_priv);
extern unsigned long i915_gfx_val(struct drm_i915_private *dev_priv);
extern void i915_update_gfx_val(struct drm_i915_private *dev_priv);


/* i915_irq.c */
void i915_hangcheck_elapsed(unsigned long data);
void i915_handle_error(struct drm_device *dev, bool wedged);
extern int i915_irq_emit(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
extern int i915_irq_wait(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);

extern void intel_irq_init(struct drm_device *dev);

extern int i915_vblank_pipe_set(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
extern int i915_vblank_pipe_get(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
extern int i915_vblank_swap(struct drm_device *dev, void *data,
			    struct drm_file *file_priv);

void
i915_enable_pipestat(drm_i915_private_t *dev_priv, int pipe, u32 mask);

void
i915_disable_pipestat(drm_i915_private_t *dev_priv, int pipe, u32 mask);

void intel_enable_asle(struct drm_device *dev);

#ifdef CONFIG_DEBUG_FS
extern void i915_destroy_error_state(struct drm_device *dev);
#else
#define i915_destroy_error_state(x)
#endif


/* i915_gem.c */
int i915_gem_init_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_create_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int i915_gem_pread_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_pwrite_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int i915_gem_mmap_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_mmap_gtt_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_set_domain_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv);
int i915_gem_sw_finish_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);
int i915_gem_execbuffer(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_execbuffer2(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_pin_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv);
int i915_gem_unpin_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_busy_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_throttle_ioctl(struct drm_device *dev, void *data,
			    struct drm_file *file_priv);
int i915_gem_madvise_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_entervt_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_leavevt_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_set_tiling(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_get_tiling(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_get_aperture_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
void i915_gem_load(struct drm_device *dev);
int i915_gem_init_object(struct drm_gem_object *obj);
int __must_check i915_gem_flush_ring(struct intel_ring_buffer *ring,
				     uint32_t invalidate_domains,
				     uint32_t flush_domains);
struct drm_i915_gem_object *i915_gem_alloc_object(struct drm_device *dev,
						  size_t size);
void i915_gem_free_object(struct drm_gem_object *obj);
int __must_check i915_gem_object_pin(struct drm_i915_gem_object *obj,
				     uint32_t alignment,
				     bool map_and_fenceable);
void i915_gem_object_unpin(struct drm_i915_gem_object *obj);
int __must_check i915_gem_object_unbind(struct drm_i915_gem_object *obj);
void i915_gem_release_mmap(struct drm_i915_gem_object *obj);
void i915_gem_lastclose(struct drm_device *dev);

int __must_check i915_mutex_lock_interruptible(struct drm_device *dev);
int __must_check i915_gem_object_wait_rendering(struct drm_i915_gem_object *obj);
void i915_gem_object_move_to_active(struct drm_i915_gem_object *obj,
				    struct intel_ring_buffer *ring,
				    u32 seqno);

int i915_gem_dumb_create(struct drm_file *file_priv,
			 struct drm_device *dev,
			 struct drm_mode_create_dumb *args);
int i915_gem_mmap_gtt(struct drm_file *file_priv, struct drm_device *dev,
		      uint32_t handle, uint64_t *offset);
int i915_gem_dumb_destroy(struct drm_file *file_priv, struct drm_device *dev,
			  uint32_t handle);
/**
 * Returns true if seq1 is later than seq2.
 */
static inline bool
i915_seqno_passed(uint32_t seq1, uint32_t seq2)
{
	return (int32_t)(seq1 - seq2) >= 0;
}

u32 i915_gem_next_request_seqno(struct intel_ring_buffer *ring);

int __must_check i915_gem_object_get_fence(struct drm_i915_gem_object *obj,
					   struct intel_ring_buffer *pipelined);
int __must_check i915_gem_object_put_fence(struct drm_i915_gem_object *obj);

static inline void
i915_gem_object_pin_fence(struct drm_i915_gem_object *obj)
{
	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
		dev_priv->fence_regs[obj->fence_reg].pin_count++;
	}
}

static inline void
i915_gem_object_unpin_fence(struct drm_i915_gem_object *obj)
{
	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
		dev_priv->fence_regs[obj->fence_reg].pin_count--;
	}
}

void i915_gem_retire_requests(struct drm_device *dev);
void i915_gem_retire_requests_ring(struct intel_ring_buffer *ring);

void i915_gem_reset(struct drm_device *dev);
void i915_gem_clflush_object(struct drm_i915_gem_object *obj);
int __must_check i915_gem_object_set_domain(struct drm_i915_gem_object *obj,
					    uint32_t read_domains,
					    uint32_t write_domain);
int __must_check i915_gem_object_finish_gpu(struct drm_i915_gem_object *obj);
int __must_check i915_gem_init_hw(struct drm_device *dev);
void i915_gem_init_swizzling(struct drm_device *dev);
void i915_gem_init_ppgtt(struct drm_device *dev);
void i915_gem_cleanup_ringbuffer(struct drm_device *dev);
void i915_gem_do_init(struct drm_device *dev,
		      unsigned long start,
		      unsigned long mappable_end,
		      unsigned long end);
int __must_check i915_gpu_idle(struct drm_device *dev, bool do_retire);
int __must_check i915_gem_idle(struct drm_device *dev);
int __must_check i915_add_request(struct intel_ring_buffer *ring,
				  struct drm_file *file,
				  struct drm_i915_gem_request *request);
int __must_check i915_wait_request(struct intel_ring_buffer *ring,
				   uint32_t seqno,
				   bool do_retire);
int i915_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
int __must_check
i915_gem_object_set_to_gtt_domain(struct drm_i915_gem_object *obj,
				  bool write);
int __must_check
i915_gem_object_pin_to_display_plane(struct drm_i915_gem_object *obj,
				     u32 alignment,
				     struct intel_ring_buffer *pipelined);
int i915_gem_attach_phys_object(struct drm_device *dev,
				struct drm_i915_gem_object *obj,
				int id,
				int align);
void i915_gem_detach_phys_object(struct drm_device *dev,
				 struct drm_i915_gem_object *obj);
void i915_gem_free_all_phys_object(struct drm_device *dev);
void i915_gem_release(struct drm_device *dev, struct drm_file *file);

uint32_t
i915_gem_get_unfenced_gtt_alignment(struct drm_device *dev,
				    uint32_t size,
				    int tiling_mode);

int i915_gem_object_set_cache_level(struct drm_i915_gem_object *obj,
				    enum i915_cache_level cache_level);

/* i915_gem_gtt.c */
int __must_check i915_gem_init_aliasing_ppgtt(struct drm_device *dev);
void i915_gem_cleanup_aliasing_ppgtt(struct drm_device *dev);
void i915_ppgtt_bind_object(struct i915_hw_ppgtt *ppgtt,
			    struct drm_i915_gem_object *obj,
			    enum i915_cache_level cache_level);
void i915_ppgtt_unbind_object(struct i915_hw_ppgtt *ppgtt,
			      struct drm_i915_gem_object *obj);

void i915_gem_restore_gtt_mappings(struct drm_device *dev);
int __must_check i915_gem_gtt_bind_object(struct drm_i915_gem_object *obj);
void i915_gem_gtt_rebind_object(struct drm_i915_gem_object *obj,
				enum i915_cache_level cache_level);
void i915_gem_gtt_unbind_object(struct drm_i915_gem_object *obj);

/* i915_gem_evict.c */
int __must_check i915_gem_evict_something(struct drm_device *dev, int min_size,
					  unsigned alignment, bool mappable);
int __must_check i915_gem_evict_everything(struct drm_device *dev,
					   bool purgeable_only);
int __must_check i915_gem_evict_inactive(struct drm_device *dev,
					 bool purgeable_only);

/* i915_gem_tiling.c */
void i915_gem_detect_bit_6_swizzle(struct drm_device *dev);
void i915_gem_object_do_bit_17_swizzle(struct drm_i915_gem_object *obj);
void i915_gem_object_save_bit_17_swizzle(struct drm_i915_gem_object *obj);

/* i915_gem_debug.c */
void i915_gem_dump_object(struct drm_i915_gem_object *obj, int len,
			  const char *where, uint32_t mark);
#if WATCH_LISTS
int i915_verify_lists(struct drm_device *dev);
#else
#define i915_verify_lists(dev) 0
#endif
void i915_gem_object_check_coherency(struct drm_i915_gem_object *obj,
				     int handle);
void i915_gem_dump_object(struct drm_i915_gem_object *obj, int len,
			  const char *where, uint32_t mark);

/* i915_debugfs.c */
int i915_debugfs_init(struct drm_minor *minor);
void i915_debugfs_cleanup(struct drm_minor *minor);

/* i915_suspend.c */
extern int i915_save_state(struct drm_device *dev);
extern int i915_restore_state(struct drm_device *dev);

/* i915_suspend.c */
extern int i915_save_state(struct drm_device *dev);
extern int i915_restore_state(struct drm_device *dev);

/* intel_i2c.c */
extern int intel_setup_gmbus(struct drm_device *dev);
extern void intel_teardown_gmbus(struct drm_device *dev);
extern void intel_gmbus_set_speed(struct i2c_adapter *adapter, int speed);
extern void intel_gmbus_force_bit(struct i2c_adapter *adapter, bool force_bit);
extern inline bool intel_gmbus_is_forced_bit(struct i2c_adapter *adapter)
{
	return container_of(adapter, struct intel_gmbus, adapter)->force_bit;
}
extern void intel_i2c_reset(struct drm_device *dev);

/* intel_opregion.c */
extern int intel_opregion_setup(struct drm_device *dev);
#ifdef CONFIG_ACPI
extern void intel_opregion_init(struct drm_device *dev);
extern void intel_opregion_fini(struct drm_device *dev);
extern void intel_opregion_asle_intr(struct drm_device *dev);
extern void intel_opregion_gse_intr(struct drm_device *dev);
extern void intel_opregion_enable_asle(struct drm_device *dev);
#else
static inline void intel_opregion_init(struct drm_device *dev) { return; }
static inline void intel_opregion_fini(struct drm_device *dev) { return; }
static inline void intel_opregion_asle_intr(struct drm_device *dev) { return; }
static inline void intel_opregion_gse_intr(struct drm_device *dev) { return; }
static inline void intel_opregion_enable_asle(struct drm_device *dev) { return; }
#endif

/* intel_acpi.c */
#ifdef CONFIG_ACPI
extern void intel_register_dsm_handler(void);
extern void intel_unregister_dsm_handler(void);
#else
static inline void intel_register_dsm_handler(void) { return; }
static inline void intel_unregister_dsm_handler(void) { return; }
#endif /* CONFIG_ACPI */

/* modesetting */
extern void i915_redisable_vga(struct drm_device *dev);
extern void intel_modeset_init(struct drm_device *dev);
extern void intel_modeset_gem_init(struct drm_device *dev);
extern void intel_modeset_cleanup(struct drm_device *dev);
extern int intel_modeset_vga_set_state(struct drm_device *dev, bool state);
extern bool intel_fbc_enabled(struct drm_device *dev);
extern void intel_disable_fbc(struct drm_device *dev);
extern bool ironlake_set_drps(struct drm_device *dev, u8 val);
extern void ironlake_init_pch_refclk(struct drm_device *dev);
extern void ironlake_enable_rc6(struct drm_device *dev);
extern void gen6_set_rps(struct drm_device *dev, u8 val);
extern void intel_detect_pch(struct drm_device *dev);
extern int intel_trans_dp_port_sel(struct drm_crtc *crtc);

extern void __gen6_gt_force_wake_get(struct drm_i915_private *dev_priv);
extern void __gen6_gt_force_wake_mt_get(struct drm_i915_private *dev_priv);
extern void __gen6_gt_force_wake_put(struct drm_i915_private *dev_priv);
extern void __gen6_gt_force_wake_mt_put(struct drm_i915_private *dev_priv);

/* overlay */
#ifdef CONFIG_DEBUG_FS
extern struct intel_overlay_error_state *intel_overlay_capture_error_state(struct drm_device *dev);
extern void intel_overlay_print_error_state(struct seq_file *m, struct intel_overlay_error_state *error);

extern struct intel_display_error_state *intel_display_capture_error_state(struct drm_device *dev);
extern void intel_display_print_error_state(struct seq_file *m,
					    struct drm_device *dev,
					    struct intel_display_error_state *error);
#endif

#define LP_RING(d) (&((struct drm_i915_private *)(d))->ring[RCS])

#define BEGIN_LP_RING(n) \
	intel_ring_begin(LP_RING(dev_priv), (n))

#define OUT_RING(x) \
	intel_ring_emit(LP_RING(dev_priv), x)

#define ADVANCE_LP_RING() \
	intel_ring_advance(LP_RING(dev_priv))

/**
 * Lock test for when it's just for synchronization of ring access.
 *
 * In that case, we don't need to do it when GEM is initialized as nobody else
 * has access to the ring.
 */
#define RING_LOCK_TEST_WITH_RETURN(dev, file) do {			\
	if (LP_RING(dev->dev_private)->obj == NULL)			\
		LOCK_TEST_WITH_RETURN(dev, file);			\
} while (0)

/* On SNB platform, before reading ring registers forcewake bit
 * must be set to prevent GT core from power down and stale values being
 * returned.
 */
void gen6_gt_force_wake_get(struct drm_i915_private *dev_priv);
void gen6_gt_force_wake_put(struct drm_i915_private *dev_priv);
int __gen6_gt_wait_for_fifo(struct drm_i915_private *dev_priv);

#define __i915_read(x, y) \
	u##x i915_read##x(struct drm_i915_private *dev_priv, u32 reg);

__i915_read(8, b)
__i915_read(16, w)
__i915_read(32, l)
__i915_read(64, q)
#undef __i915_read

#define __i915_write(x, y) \
	void i915_write##x(struct drm_i915_private *dev_priv, u32 reg, u##x val);

__i915_write(8, b)
__i915_write(16, w)
__i915_write(32, l)
__i915_write(64, q)
#undef __i915_write

#define I915_READ8(reg)		i915_read8(dev_priv, (reg))
#define I915_WRITE8(reg, val)	i915_write8(dev_priv, (reg), (val))

#define I915_READ16(reg)	i915_read16(dev_priv, (reg))
#define I915_WRITE16(reg, val)	i915_write16(dev_priv, (reg), (val))
#define I915_READ16_NOTRACE(reg)	readw(dev_priv->regs + (reg))
#define I915_WRITE16_NOTRACE(reg, val)	writew(val, dev_priv->regs + (reg))

#define I915_READ(reg)		i915_read32(dev_priv, (reg))
#define I915_WRITE(reg, val)	i915_write32(dev_priv, (reg), (val))
#define I915_READ_NOTRACE(reg)		readl(dev_priv->regs + (reg))
#define I915_WRITE_NOTRACE(reg, val)	writel(val, dev_priv->regs + (reg))

#define I915_WRITE64(reg, val)	i915_write64(dev_priv, (reg), (val))
#define I915_READ64(reg)	i915_read64(dev_priv, (reg))

#define POSTING_READ(reg)	(void)I915_READ_NOTRACE(reg)
#define POSTING_READ16(reg)	(void)I915_READ16_NOTRACE(reg)

=======
#define HAS_128_BYTE_Y_TILING(i915) (GRAPHICS_VER(i915) != 2 && \
					 !(IS_I915G(i915) || IS_I915GM(i915)))

#define HAS_RC6(i915)		 (INTEL_INFO(i915)->has_rc6)
#define HAS_RC6p(i915)		 (INTEL_INFO(i915)->has_rc6p)
#define HAS_RC6pp(i915)		 (false) /* HW was never validated */

#define HAS_RPS(i915)	(INTEL_INFO(i915)->has_rps)

#define HAS_HECI_PXP(i915) \
	(INTEL_INFO(i915)->has_heci_pxp)

#define HAS_HECI_GSCFI(i915) \
	(INTEL_INFO(i915)->has_heci_gscfi)

#define HAS_HECI_GSC(i915) (HAS_HECI_PXP(i915) || HAS_HECI_GSCFI(i915))

#define HAS_RUNTIME_PM(i915) (INTEL_INFO(i915)->has_runtime_pm)
#define HAS_64BIT_RELOC(i915) (INTEL_INFO(i915)->has_64bit_reloc)

#define HAS_OA_BPC_REPORTING(i915) \
	(INTEL_INFO(i915)->has_oa_bpc_reporting)
#define HAS_OA_SLICE_CONTRIB_LIMITS(i915) \
	(INTEL_INFO(i915)->has_oa_slice_contrib_limits)
#define HAS_OAM(i915) \
	(INTEL_INFO(i915)->has_oam)

/*
 * Set this flag, when platform requires 64K GTT page sizes or larger for
 * device local memory access.
 */
#define HAS_64K_PAGES(i915) (INTEL_INFO(i915)->has_64k_pages)

#define HAS_REGION(i915, i) (INTEL_INFO(i915)->memory_regions & (i))
#define HAS_LMEM(i915) HAS_REGION(i915, REGION_LMEM)

#define HAS_EXTRA_GT_LIST(i915)   (INTEL_INFO(i915)->extra_gt_list)

/*
 * Platform has the dedicated compression control state for each lmem surfaces
 * stored in lmem to support the 3D and media compression formats.
 */
#define HAS_FLAT_CCS(i915)   (INTEL_INFO(i915)->has_flat_ccs)

#define HAS_GT_UC(i915)	(INTEL_INFO(i915)->has_gt_uc)

#define HAS_POOLED_EU(i915)	(RUNTIME_INFO(i915)->has_pooled_eu)

#define HAS_GLOBAL_MOCS_REGISTERS(i915)	(INTEL_INFO(i915)->has_global_mocs)

#define HAS_GMD_ID(i915)	(INTEL_INFO(i915)->has_gmd_id)

#define HAS_L3_CCS_READ(i915) (INTEL_INFO(i915)->has_l3_ccs_read)

/* DPF == dynamic parity feature */
#define HAS_L3_DPF(i915) (INTEL_INFO(i915)->has_l3_dpf)
#define NUM_L3_SLICES(i915) (IS_HASWELL_GT3(i915) ? \
				 2 : HAS_L3_DPF(i915))

#define HAS_GUC_DEPRIVILEGE(i915) \
	(INTEL_INFO(i915)->has_guc_deprivilege)

#define HAS_GUC_TLB_INVALIDATION(i915)	(INTEL_INFO(i915)->has_guc_tlb_invalidation)

#define HAS_3D_PIPELINE(i915)	(INTEL_INFO(i915)->has_3d_pipeline)

#define HAS_ONE_EU_PER_FUSE_BIT(i915)	(INTEL_INFO(i915)->has_one_eu_per_fuse_bit)

#define HAS_LMEMBAR_SMEM_STOLEN(i915) (!HAS_LMEM(i915) && \
				       GRAPHICS_VER_FULL(i915) >= IP_VER(12, 70))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
