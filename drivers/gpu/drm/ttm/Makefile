<<<<<<< HEAD
#
# Makefile for the drm device driver.  This driver provides support for the

ccflags-y := -Iinclude/drm
ttm-y := ttm_agp_backend.o ttm_memory.o ttm_tt.o ttm_bo.o \
	ttm_bo_util.o ttm_bo_vm.o ttm_module.o \
	ttm_object.o ttm_lock.o ttm_execbuf_util.o ttm_page_alloc.o \
	ttm_bo_manager.o

ifeq ($(CONFIG_SWIOTLB),y)
ttm-y += ttm_page_alloc_dma.o
endif

obj-$(CONFIG_DRM_TTM) += ttm.o
=======
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the drm device driver.  This driver provides support for the

ttm-y := ttm_tt.o ttm_bo.o ttm_bo_util.o ttm_bo_vm.o ttm_module.o \
	ttm_execbuf_util.o ttm_range_manager.o ttm_resource.o ttm_pool.o \
	ttm_device.o ttm_sys_manager.o
ttm-$(CONFIG_AGP) += ttm_agp_backend.o

obj-$(CONFIG_DRM_TTM) += ttm.o
obj-$(CONFIG_DRM_TTM_KUNIT_TEST) += tests/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
