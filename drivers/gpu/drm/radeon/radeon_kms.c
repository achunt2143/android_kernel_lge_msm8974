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
 */
<<<<<<< HEAD
#include "drmP.h"
#include "drm_sarea.h"
#include "radeon.h"
#include "radeon_drm.h"

#include <linux/vga_switcheroo.h>
#include <linux/slab.h>

int radeon_driver_unload_kms(struct drm_device *dev)
=======

#include <linux/pci.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vga_switcheroo.h>

#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/radeon_drm.h>

#include "radeon.h"
#include "radeon_asic.h"
#include "radeon_drv.h"
#include "radeon_kms.h"

#if defined(CONFIG_VGA_SWITCHEROO)
bool radeon_has_atpx(void);
#else
static inline bool radeon_has_atpx(void) { return false; }
#endif

/**
 * radeon_driver_unload_kms - Main unload function for KMS.
 *
 * @dev: drm dev pointer
 *
 * This is the main unload function for KMS (all asics).
 * It calls radeon_modeset_fini() to tear down the
 * displays, and radeon_device_fini() to tear down
 * the rest of the device (CP, writeback, etc.).
 * Returns 0 on success.
 */
void radeon_driver_unload_kms(struct drm_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct radeon_device *rdev = dev->dev_private;

	if (rdev == NULL)
<<<<<<< HEAD
		return 0;
	if (rdev->rmmio == NULL)
		goto done_free;
	radeon_modeset_fini(rdev);
	radeon_device_fini(rdev);

done_free:
	kfree(rdev);
	dev->dev_private = NULL;
	return 0;
}

int radeon_driver_load_kms(struct drm_device *dev, unsigned long flags)
{
=======
		return;

	if (rdev->rmmio == NULL)
		goto done_free;

	if (radeon_is_px(dev)) {
		pm_runtime_get_sync(dev->dev);
		pm_runtime_forbid(dev->dev);
	}

	radeon_acpi_fini(rdev);

	radeon_modeset_fini(rdev);
	radeon_device_fini(rdev);

	if (rdev->agp)
		arch_phys_wc_del(rdev->agp->agp_mtrr);
	kfree(rdev->agp);
	rdev->agp = NULL;

done_free:
	kfree(rdev);
	dev->dev_private = NULL;
}

/**
 * radeon_driver_load_kms - Main load function for KMS.
 *
 * @dev: drm dev pointer
 * @flags: device flags
 *
 * This is the main load function for KMS (all asics).
 * It calls radeon_device_init() to set up the non-display
 * parts of the chip (asic init, CP, writeback, etc.), and
 * radeon_modeset_init() to set up the display parts
 * (crtcs, encoders, hotplug detect, etc.).
 * Returns 0 on success, error on failure.
 */
int radeon_driver_load_kms(struct drm_device *dev, unsigned long flags)
{
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct radeon_device *rdev;
	int r, acpi_status;

	rdev = kzalloc(sizeof(struct radeon_device), GFP_KERNEL);
	if (rdev == NULL) {
		return -ENOMEM;
	}
	dev->dev_private = (void *)rdev;

<<<<<<< HEAD
	pci_set_master(dev->pdev);

	/* update BUS flag */
	if (drm_pci_device_is_agp(dev)) {
		flags |= RADEON_IS_AGP;
	} else if (pci_is_pcie(dev->pdev)) {
=======
#ifdef __alpha__
	rdev->hose = pdev->sysdata;
#endif

	if (pci_find_capability(pdev, PCI_CAP_ID_AGP))
		rdev->agp = radeon_agp_head_init(dev);
	if (rdev->agp) {
		rdev->agp->agp_mtrr = arch_phys_wc_add(
			rdev->agp->agp_info.aper_base,
			rdev->agp->agp_info.aper_size *
			1024 * 1024);
	}

	/* update BUS flag */
	if (pci_find_capability(pdev, PCI_CAP_ID_AGP)) {
		flags |= RADEON_IS_AGP;
	} else if (pci_is_pcie(pdev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags |= RADEON_IS_PCIE;
	} else {
		flags |= RADEON_IS_PCI;
	}

<<<<<<< HEAD
=======
	if ((radeon_runtime_pm != 0) &&
	    radeon_has_atpx() &&
	    ((flags & RADEON_IS_IGP) == 0) &&
	    !pci_is_thunderbolt_attached(pdev))
		flags |= RADEON_IS_PX;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* radeon_device_init should report only fatal error
	 * like memory allocation failure or iomapping failure,
	 * or memory manager initialization failure, it must
	 * properly initialize the GPU MC controller and permit
	 * VRAM allocation
	 */
<<<<<<< HEAD
	r = radeon_device_init(rdev, dev, dev->pdev, flags);
	if (r) {
		dev_err(&dev->pdev->dev, "Fatal error during GPU init\n");
		goto out;
	}

	/* Call ACPI methods */
	acpi_status = radeon_acpi_init(rdev);
	if (acpi_status)
		dev_dbg(&dev->pdev->dev, "Error during ACPI methods call\n");

=======
	r = radeon_device_init(rdev, dev, pdev, flags);
	if (r) {
		dev_err(dev->dev, "Fatal error during GPU init\n");
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Again modeset_init should fail only on fatal error
	 * otherwise it should provide enough functionalities
	 * for shadowfb to run
	 */
	r = radeon_modeset_init(rdev);
	if (r)
<<<<<<< HEAD
		dev_err(&dev->pdev->dev, "Fatal error during modeset init\n");
out:
	if (r)
		radeon_driver_unload_kms(dev);
	return r;
}

=======
		dev_err(dev->dev, "Fatal error during modeset init\n");

	/* Call ACPI methods: require modeset init
	 * but failure is not fatal
	 */
	if (!r) {
		acpi_status = radeon_acpi_init(rdev);
		if (acpi_status)
			dev_dbg(dev->dev, "Error during ACPI methods call\n");
	}

	if (radeon_is_px(dev)) {
		dev_pm_set_driver_flags(dev->dev, DPM_FLAG_NO_DIRECT_COMPLETE);
		pm_runtime_use_autosuspend(dev->dev);
		pm_runtime_set_autosuspend_delay(dev->dev, 5000);
		pm_runtime_set_active(dev->dev);
		pm_runtime_allow(dev->dev);
		pm_runtime_mark_last_busy(dev->dev);
		pm_runtime_put_autosuspend(dev->dev);
	}

out:
	if (r)
		radeon_driver_unload_kms(dev);


	return r;
}

/**
 * radeon_set_filp_rights - Set filp right.
 *
 * @dev: drm dev pointer
 * @owner: drm file
 * @applier: drm file
 * @value: value
 *
 * Sets the filp rights for the device (all asics).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void radeon_set_filp_rights(struct drm_device *dev,
				   struct drm_file **owner,
				   struct drm_file *applier,
				   uint32_t *value)
{
<<<<<<< HEAD
	mutex_lock(&dev->struct_mutex);
=======
	struct radeon_device *rdev = dev->dev_private;

	mutex_lock(&rdev->gem.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (*value == 1) {
		/* wants rights */
		if (!*owner)
			*owner = applier;
	} else if (*value == 0) {
		/* revokes rights */
		if (*owner == applier)
			*owner = NULL;
	}
	*value = *owner == applier ? 1 : 0;
<<<<<<< HEAD
	mutex_unlock(&dev->struct_mutex);
=======
	mutex_unlock(&rdev->gem.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Userspace get information ioctl
 */
<<<<<<< HEAD
int radeon_info_ioctl(struct drm_device *dev, void *data, struct drm_file *filp)
{
	struct radeon_device *rdev = dev->dev_private;
	struct drm_radeon_info *info;
	struct radeon_mode_info *minfo = &rdev->mode_info;
	uint32_t *value_ptr;
	uint32_t value;
	struct drm_crtc *crtc;
	int i, found;

	info = data;
	value_ptr = (uint32_t *)((unsigned long)info->value);
	if (DRM_COPY_FROM_USER(&value, value_ptr, sizeof(value)))
		return -EFAULT;

	switch (info->request) {
	case RADEON_INFO_DEVICE_ID:
		value = dev->pci_device;
		break;
	case RADEON_INFO_NUM_GB_PIPES:
		value = rdev->num_gb_pipes;
		break;
	case RADEON_INFO_NUM_Z_PIPES:
		value = rdev->num_z_pipes;
=======
/**
 * radeon_info_ioctl - answer a device specific request.
 *
 * @dev: drm device pointer
 * @data: request object
 * @filp: drm filp
 *
 * This function is used to pass device specific parameters to the userspace
 * drivers.  Examples include: pci device id, pipeline parms, tiling params,
 * etc. (all asics).
 * Returns 0 on success, -EINVAL on failure.
 */
int radeon_info_ioctl(struct drm_device *dev, void *data, struct drm_file *filp)
{
	struct radeon_device *rdev = dev->dev_private;
	struct drm_radeon_info *info = data;
	struct radeon_mode_info *minfo = &rdev->mode_info;
	uint32_t *value, value_tmp, *value_ptr, value_size;
	struct ttm_resource_manager *man;
	uint64_t value64;
	struct drm_crtc *crtc;
	int i, found;

	value_ptr = (uint32_t *)((unsigned long)info->value);
	value = &value_tmp;
	value_size = sizeof(uint32_t);

	switch (info->request) {
	case RADEON_INFO_DEVICE_ID:
		*value = to_pci_dev(dev->dev)->device;
		break;
	case RADEON_INFO_NUM_GB_PIPES:
		*value = rdev->num_gb_pipes;
		break;
	case RADEON_INFO_NUM_Z_PIPES:
		*value = rdev->num_z_pipes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case RADEON_INFO_ACCEL_WORKING:
		/* xf86-video-ati 6.13.0 relies on this being false for evergreen */
		if ((rdev->family >= CHIP_CEDAR) && (rdev->family <= CHIP_HEMLOCK))
<<<<<<< HEAD
			value = false;
		else
			value = rdev->accel_working;
		break;
	case RADEON_INFO_CRTC_FROM_ID:
		for (i = 0, found = 0; i < rdev->num_crtc; i++) {
			crtc = (struct drm_crtc *)minfo->crtcs[i];
			if (crtc && crtc->base.id == value) {
				struct radeon_crtc *radeon_crtc = to_radeon_crtc(crtc);
				value = radeon_crtc->crtc_id;
=======
			*value = false;
		else
			*value = rdev->accel_working;
		break;
	case RADEON_INFO_CRTC_FROM_ID:
		if (copy_from_user(value, value_ptr, sizeof(uint32_t))) {
			DRM_ERROR("copy_from_user %s:%u\n", __func__, __LINE__);
			return -EFAULT;
		}
		for (i = 0, found = 0; i < rdev->num_crtc; i++) {
			crtc = (struct drm_crtc *)minfo->crtcs[i];
			if (crtc && crtc->base.id == *value) {
				struct radeon_crtc *radeon_crtc = to_radeon_crtc(crtc);
				*value = radeon_crtc->crtc_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				found = 1;
				break;
			}
		}
		if (!found) {
<<<<<<< HEAD
			DRM_DEBUG_KMS("unknown crtc id %d\n", value);
=======
			DRM_DEBUG_KMS("unknown crtc id %d\n", *value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		break;
	case RADEON_INFO_ACCEL_WORKING2:
<<<<<<< HEAD
		value = rdev->accel_working;
		break;
	case RADEON_INFO_TILING_CONFIG:
		if (rdev->family >= CHIP_TAHITI)
			value = rdev->config.si.tile_config;
		else if (rdev->family >= CHIP_CAYMAN)
			value = rdev->config.cayman.tile_config;
		else if (rdev->family >= CHIP_CEDAR)
			value = rdev->config.evergreen.tile_config;
		else if (rdev->family >= CHIP_RV770)
			value = rdev->config.rv770.tile_config;
		else if (rdev->family >= CHIP_R600)
			value = rdev->config.r600.tile_config;
=======
		if (rdev->family == CHIP_HAWAII) {
			if (rdev->accel_working) {
				if (rdev->new_fw)
					*value = 3;
				else
					*value = 2;
			} else {
				*value = 0;
			}
		} else {
			*value = rdev->accel_working;
		}
		break;
	case RADEON_INFO_TILING_CONFIG:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.tile_config;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.tile_config;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.tile_config;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.tile_config;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.tile_config;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.tile_config;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else {
			DRM_DEBUG_KMS("tiling config is r6xx+ only!\n");
			return -EINVAL;
		}
		break;
	case RADEON_INFO_WANT_HYPERZ:
		/* The "value" here is both an input and output parameter.
		 * If the input value is 1, filp requests hyper-z access.
		 * If the input value is 0, filp revokes its hyper-z access.
		 *
		 * When returning, the value is 1 if filp owns hyper-z access,
		 * 0 otherwise. */
<<<<<<< HEAD
		if (value >= 2) {
			DRM_DEBUG_KMS("WANT_HYPERZ: invalid value %d\n", value);
			return -EINVAL;
		}
		radeon_set_filp_rights(dev, &rdev->hyperz_filp, filp, &value);
		break;
	case RADEON_INFO_WANT_CMASK:
		/* The same logic as Hyper-Z. */
		if (value >= 2) {
			DRM_DEBUG_KMS("WANT_CMASK: invalid value %d\n", value);
			return -EINVAL;
		}
		radeon_set_filp_rights(dev, &rdev->cmask_filp, filp, &value);
		break;
	case RADEON_INFO_CLOCK_CRYSTAL_FREQ:
		/* return clock value in KHz */
		value = rdev->clock.spll.reference_freq * 10;
		break;
	case RADEON_INFO_NUM_BACKENDS:
		if (rdev->family >= CHIP_TAHITI)
			value = rdev->config.si.max_backends_per_se *
				rdev->config.si.max_shader_engines;
		else if (rdev->family >= CHIP_CAYMAN)
			value = rdev->config.cayman.max_backends_per_se *
				rdev->config.cayman.max_shader_engines;
		else if (rdev->family >= CHIP_CEDAR)
			value = rdev->config.evergreen.max_backends;
		else if (rdev->family >= CHIP_RV770)
			value = rdev->config.rv770.max_backends;
		else if (rdev->family >= CHIP_R600)
			value = rdev->config.r600.max_backends;
=======
		if (copy_from_user(value, value_ptr, sizeof(uint32_t))) {
			DRM_ERROR("copy_from_user %s:%u\n", __func__, __LINE__);
			return -EFAULT;
		}
		if (*value >= 2) {
			DRM_DEBUG_KMS("WANT_HYPERZ: invalid value %d\n", *value);
			return -EINVAL;
		}
		radeon_set_filp_rights(dev, &rdev->hyperz_filp, filp, value);
		break;
	case RADEON_INFO_WANT_CMASK:
		/* The same logic as Hyper-Z. */
		if (copy_from_user(value, value_ptr, sizeof(uint32_t))) {
			DRM_ERROR("copy_from_user %s:%u\n", __func__, __LINE__);
			return -EFAULT;
		}
		if (*value >= 2) {
			DRM_DEBUG_KMS("WANT_CMASK: invalid value %d\n", *value);
			return -EINVAL;
		}
		radeon_set_filp_rights(dev, &rdev->cmask_filp, filp, value);
		break;
	case RADEON_INFO_CLOCK_CRYSTAL_FREQ:
		/* return clock value in KHz */
		if (rdev->asic->get_xclk)
			*value = radeon_get_xclk(rdev) * 10;
		else
			*value = rdev->clock.spll.reference_freq * 10;
		break;
	case RADEON_INFO_NUM_BACKENDS:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.max_backends_per_se *
				rdev->config.cik.max_shader_engines;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.max_backends_per_se *
				rdev->config.si.max_shader_engines;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.max_backends_per_se *
				rdev->config.cayman.max_shader_engines;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.max_backends;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.max_backends;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.max_backends;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else {
			return -EINVAL;
		}
		break;
	case RADEON_INFO_NUM_TILE_PIPES:
<<<<<<< HEAD
		if (rdev->family >= CHIP_TAHITI)
			value = rdev->config.si.max_tile_pipes;
		else if (rdev->family >= CHIP_CAYMAN)
			value = rdev->config.cayman.max_tile_pipes;
		else if (rdev->family >= CHIP_CEDAR)
			value = rdev->config.evergreen.max_tile_pipes;
		else if (rdev->family >= CHIP_RV770)
			value = rdev->config.rv770.max_tile_pipes;
		else if (rdev->family >= CHIP_R600)
			value = rdev->config.r600.max_tile_pipes;
=======
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.max_tile_pipes;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.max_tile_pipes;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.max_tile_pipes;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.max_tile_pipes;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.max_tile_pipes;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.max_tile_pipes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else {
			return -EINVAL;
		}
		break;
	case RADEON_INFO_FUSION_GART_WORKING:
<<<<<<< HEAD
		value = 1;
		break;
	case RADEON_INFO_BACKEND_MAP:
		if (rdev->family >= CHIP_TAHITI)
			value = rdev->config.si.backend_map;
		else if (rdev->family >= CHIP_CAYMAN)
			value = rdev->config.cayman.backend_map;
		else if (rdev->family >= CHIP_CEDAR)
			value = rdev->config.evergreen.backend_map;
		else if (rdev->family >= CHIP_RV770)
			value = rdev->config.rv770.backend_map;
		else if (rdev->family >= CHIP_R600)
			value = rdev->config.r600.backend_map;
=======
		*value = 1;
		break;
	case RADEON_INFO_BACKEND_MAP:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.backend_map;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.backend_map;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.backend_map;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.backend_map;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.backend_map;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.backend_map;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else {
			return -EINVAL;
		}
		break;
	case RADEON_INFO_VA_START:
		/* this is where we report if vm is supported or not */
		if (rdev->family < CHIP_CAYMAN)
			return -EINVAL;
<<<<<<< HEAD
		value = RADEON_VA_RESERVED_SIZE;
=======
		*value = RADEON_VA_RESERVED_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case RADEON_INFO_IB_VM_MAX_SIZE:
		/* this is where we report if vm is supported or not */
		if (rdev->family < CHIP_CAYMAN)
			return -EINVAL;
<<<<<<< HEAD
		value = RADEON_IB_VM_MAX_SIZE;
		break;
	case RADEON_INFO_MAX_PIPES:
		if (rdev->family >= CHIP_TAHITI)
			value = rdev->config.si.max_pipes_per_simd;
		else if (rdev->family >= CHIP_CAYMAN)
			value = rdev->config.cayman.max_pipes_per_simd;
		else if (rdev->family >= CHIP_CEDAR)
			value = rdev->config.evergreen.max_pipes;
		else if (rdev->family >= CHIP_RV770)
			value = rdev->config.rv770.max_pipes;
		else if (rdev->family >= CHIP_R600)
			value = rdev->config.r600.max_pipes;
=======
		*value = RADEON_IB_VM_MAX_SIZE;
		break;
	case RADEON_INFO_MAX_PIPES:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.max_cu_per_sh;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.max_cu_per_sh;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.max_pipes_per_simd;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.max_pipes;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.max_pipes;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.max_pipes;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else {
			return -EINVAL;
		}
		break;
<<<<<<< HEAD
=======
	case RADEON_INFO_TIMESTAMP:
		if (rdev->family < CHIP_R600) {
			DRM_DEBUG_KMS("timestamp is r6xx+ only!\n");
			return -EINVAL;
		}
		value = (uint32_t *)&value64;
		value_size = sizeof(uint64_t);
		value64 = radeon_get_gpu_clock_counter(rdev);
		break;
	case RADEON_INFO_MAX_SE:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.max_shader_engines;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.max_shader_engines;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.max_shader_engines;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.num_ses;
		else
			*value = 1;
		break;
	case RADEON_INFO_MAX_SH_PER_SE:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.max_sh_per_se;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.max_sh_per_se;
		else
			return -EINVAL;
		break;
	case RADEON_INFO_FASTFB_WORKING:
		*value = rdev->fastfb_working;
		break;
	case RADEON_INFO_RING_WORKING:
		if (copy_from_user(value, value_ptr, sizeof(uint32_t))) {
			DRM_ERROR("copy_from_user %s:%u\n", __func__, __LINE__);
			return -EFAULT;
		}
		switch (*value) {
		case RADEON_CS_RING_GFX:
		case RADEON_CS_RING_COMPUTE:
			*value = rdev->ring[RADEON_RING_TYPE_GFX_INDEX].ready;
			break;
		case RADEON_CS_RING_DMA:
			*value = rdev->ring[R600_RING_TYPE_DMA_INDEX].ready;
			*value |= rdev->ring[CAYMAN_RING_TYPE_DMA1_INDEX].ready;
			break;
		case RADEON_CS_RING_UVD:
			*value = rdev->ring[R600_RING_TYPE_UVD_INDEX].ready;
			break;
		case RADEON_CS_RING_VCE:
			*value = rdev->ring[TN_RING_TYPE_VCE1_INDEX].ready;
			break;
		default:
			return -EINVAL;
		}
		break;
	case RADEON_INFO_SI_TILE_MODE_ARRAY:
		if (rdev->family >= CHIP_BONAIRE) {
			value = rdev->config.cik.tile_mode_array;
			value_size = sizeof(uint32_t)*32;
		} else if (rdev->family >= CHIP_TAHITI) {
			value = rdev->config.si.tile_mode_array;
			value_size = sizeof(uint32_t)*32;
		} else {
			DRM_DEBUG_KMS("tile mode array is si+ only!\n");
			return -EINVAL;
		}
		break;
	case RADEON_INFO_CIK_MACROTILE_MODE_ARRAY:
		if (rdev->family >= CHIP_BONAIRE) {
			value = rdev->config.cik.macrotile_mode_array;
			value_size = sizeof(uint32_t)*16;
		} else {
			DRM_DEBUG_KMS("macrotile mode array is cik+ only!\n");
			return -EINVAL;
		}
		break;
	case RADEON_INFO_SI_CP_DMA_COMPUTE:
		*value = 1;
		break;
	case RADEON_INFO_SI_BACKEND_ENABLED_MASK:
		if (rdev->family >= CHIP_BONAIRE) {
			*value = rdev->config.cik.backend_enable_mask;
		} else if (rdev->family >= CHIP_TAHITI) {
			*value = rdev->config.si.backend_enable_mask;
		} else {
			DRM_DEBUG_KMS("BACKEND_ENABLED_MASK is si+ only!\n");
			return -EINVAL;
		}
		break;
	case RADEON_INFO_MAX_SCLK:
		if ((rdev->pm.pm_method == PM_METHOD_DPM) &&
		    rdev->pm.dpm_enabled)
			*value = rdev->pm.dpm.dyn_state.max_clock_voltage_on_ac.sclk * 10;
		else
			*value = rdev->pm.default_sclk * 10;
		break;
	case RADEON_INFO_VCE_FW_VERSION:
		*value = rdev->vce.fw_version;
		break;
	case RADEON_INFO_VCE_FB_VERSION:
		*value = rdev->vce.fb_version;
		break;
	case RADEON_INFO_NUM_BYTES_MOVED:
		value = (uint32_t *)&value64;
		value_size = sizeof(uint64_t);
		value64 = atomic64_read(&rdev->num_bytes_moved);
		break;
	case RADEON_INFO_VRAM_USAGE:
		value = (uint32_t *)&value64;
		value_size = sizeof(uint64_t);
		man = ttm_manager_type(&rdev->mman.bdev, TTM_PL_VRAM);
		value64 = ttm_resource_manager_usage(man);
		break;
	case RADEON_INFO_GTT_USAGE:
		value = (uint32_t *)&value64;
		value_size = sizeof(uint64_t);
		man = ttm_manager_type(&rdev->mman.bdev, TTM_PL_TT);
		value64 = ttm_resource_manager_usage(man);
		break;
	case RADEON_INFO_ACTIVE_CU_COUNT:
		if (rdev->family >= CHIP_BONAIRE)
			*value = rdev->config.cik.active_cus;
		else if (rdev->family >= CHIP_TAHITI)
			*value = rdev->config.si.active_cus;
		else if (rdev->family >= CHIP_CAYMAN)
			*value = rdev->config.cayman.active_simds;
		else if (rdev->family >= CHIP_CEDAR)
			*value = rdev->config.evergreen.active_simds;
		else if (rdev->family >= CHIP_RV770)
			*value = rdev->config.rv770.active_simds;
		else if (rdev->family >= CHIP_R600)
			*value = rdev->config.r600.active_simds;
		else
			*value = 1;
		break;
	case RADEON_INFO_CURRENT_GPU_TEMP:
		/* get temperature in millidegrees C */
		if (rdev->asic->pm.get_temperature)
			*value = radeon_get_temperature(rdev);
		else
			*value = 0;
		break;
	case RADEON_INFO_CURRENT_GPU_SCLK:
		/* get sclk in Mhz */
		if (rdev->pm.dpm_enabled)
			*value = radeon_dpm_get_current_sclk(rdev) / 100;
		else
			*value = rdev->pm.current_sclk / 100;
		break;
	case RADEON_INFO_CURRENT_GPU_MCLK:
		/* get mclk in Mhz */
		if (rdev->pm.dpm_enabled)
			*value = radeon_dpm_get_current_mclk(rdev) / 100;
		else
			*value = rdev->pm.current_mclk / 100;
		break;
	case RADEON_INFO_READ_REG:
		if (copy_from_user(value, value_ptr, sizeof(uint32_t))) {
			DRM_ERROR("copy_from_user %s:%u\n", __func__, __LINE__);
			return -EFAULT;
		}
		if (radeon_get_allowed_info_register(rdev, *value, value))
			return -EINVAL;
		break;
	case RADEON_INFO_VA_UNMAP_WORKING:
		*value = true;
		break;
	case RADEON_INFO_GPU_RESET_COUNTER:
		*value = atomic_read(&rdev->gpu_reset_counter);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		DRM_DEBUG_KMS("Invalid request %d\n", info->request);
		return -EINVAL;
	}
<<<<<<< HEAD
	if (DRM_COPY_TO_USER(value_ptr, &value, sizeof(uint32_t))) {
		DRM_ERROR("copy_to_user\n");
=======
	if (copy_to_user(value_ptr, (char *)value, value_size)) {
		DRM_ERROR("copy_to_user %s:%u\n", __func__, __LINE__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;
	}
	return 0;
}

<<<<<<< HEAD

/*
 * Outdated mess for old drm with Xorg being in charge (void function now).
 */
int radeon_driver_firstopen_kms(struct drm_device *dev)
{
	return 0;
}

void radeon_driver_lastclose_kms(struct drm_device *dev)
{
	vga_switcheroo_process_delayed_switch();
}

int radeon_driver_open_kms(struct drm_device *dev, struct drm_file *file_priv)
{
	struct radeon_device *rdev = dev->dev_private;

	file_priv->driver_priv = NULL;

	/* new gpu have virtual address space support */
	if (rdev->family >= CHIP_CAYMAN) {
		struct radeon_fpriv *fpriv;
		int r;

		fpriv = kzalloc(sizeof(*fpriv), GFP_KERNEL);
		if (unlikely(!fpriv)) {
			return -ENOMEM;
		}

		r = radeon_vm_init(rdev, &fpriv->vm);
		if (r) {
			radeon_vm_fini(rdev, &fpriv->vm);
			kfree(fpriv);
			return r;
		}

		file_priv->driver_priv = fpriv;
	}
	return 0;
}

=======
/**
 * radeon_driver_open_kms - drm callback for open
 *
 * @dev: drm dev pointer
 * @file_priv: drm file
 *
 * On device open, init vm on cayman+ (all asics).
 * Returns 0 on success, error on failure.
 */
int radeon_driver_open_kms(struct drm_device *dev, struct drm_file *file_priv)
{
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_fpriv *fpriv;
	struct radeon_vm *vm;
	int r;

	file_priv->driver_priv = NULL;

	r = pm_runtime_get_sync(dev->dev);
	if (r < 0) {
		pm_runtime_put_autosuspend(dev->dev);
		return r;
	}

	/* new gpu have virtual address space support */
	if (rdev->family >= CHIP_CAYMAN) {

		fpriv = kzalloc(sizeof(*fpriv), GFP_KERNEL);
		if (unlikely(!fpriv)) {
			r = -ENOMEM;
			goto err_suspend;
		}

		if (rdev->accel_working) {
			vm = &fpriv->vm;
			r = radeon_vm_init(rdev, vm);
			if (r)
				goto err_fpriv;

			r = radeon_bo_reserve(rdev->ring_tmp_bo.bo, false);
			if (r)
				goto err_vm_fini;

			/* map the ib pool buffer read only into
			 * virtual address space */
			vm->ib_bo_va = radeon_vm_bo_add(rdev, vm,
							rdev->ring_tmp_bo.bo);
			if (!vm->ib_bo_va) {
				r = -ENOMEM;
				goto err_vm_fini;
			}

			r = radeon_vm_bo_set_addr(rdev, vm->ib_bo_va,
						  RADEON_VA_IB_OFFSET,
						  RADEON_VM_PAGE_READABLE |
						  RADEON_VM_PAGE_SNOOPED);
			if (r)
				goto err_vm_fini;
		}
		file_priv->driver_priv = fpriv;
	}

	pm_runtime_mark_last_busy(dev->dev);
	pm_runtime_put_autosuspend(dev->dev);
	return 0;

err_vm_fini:
	radeon_vm_fini(rdev, vm);
err_fpriv:
	kfree(fpriv);

err_suspend:
	pm_runtime_mark_last_busy(dev->dev);
	pm_runtime_put_autosuspend(dev->dev);
	return r;
}

/**
 * radeon_driver_postclose_kms - drm callback for post close
 *
 * @dev: drm dev pointer
 * @file_priv: drm file
 *
 * On device close, tear down hyperz and cmask filps on r1xx-r5xx
 * (all asics).  And tear down vm on cayman+ (all asics).
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void radeon_driver_postclose_kms(struct drm_device *dev,
				 struct drm_file *file_priv)
{
	struct radeon_device *rdev = dev->dev_private;

<<<<<<< HEAD
	/* new gpu have virtual address space support */
	if (rdev->family >= CHIP_CAYMAN && file_priv->driver_priv) {
		struct radeon_fpriv *fpriv = file_priv->driver_priv;

		radeon_vm_fini(rdev, &fpriv->vm);
		kfree(fpriv);
		file_priv->driver_priv = NULL;
	}
}

void radeon_driver_preclose_kms(struct drm_device *dev,
				struct drm_file *file_priv)
{
	struct radeon_device *rdev = dev->dev_private;
=======
	pm_runtime_get_sync(dev->dev);

	mutex_lock(&rdev->gem.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rdev->hyperz_filp == file_priv)
		rdev->hyperz_filp = NULL;
	if (rdev->cmask_filp == file_priv)
		rdev->cmask_filp = NULL;
<<<<<<< HEAD
=======
	mutex_unlock(&rdev->gem.mutex);

	radeon_uvd_free_handles(rdev, file_priv);
	radeon_vce_free_handles(rdev, file_priv);

	/* new gpu have virtual address space support */
	if (rdev->family >= CHIP_CAYMAN && file_priv->driver_priv) {
		struct radeon_fpriv *fpriv = file_priv->driver_priv;
		struct radeon_vm *vm = &fpriv->vm;
		int r;

		if (rdev->accel_working) {
			r = radeon_bo_reserve(rdev->ring_tmp_bo.bo, false);
			if (!r) {
				if (vm->ib_bo_va)
					radeon_vm_bo_rmv(rdev, vm->ib_bo_va);
				radeon_bo_unreserve(rdev->ring_tmp_bo.bo);
			}
			radeon_vm_fini(rdev, vm);
		}

		kfree(fpriv);
		file_priv->driver_priv = NULL;
	}
	pm_runtime_mark_last_busy(dev->dev);
	pm_runtime_put_autosuspend(dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * VBlank related functions.
 */
<<<<<<< HEAD
u32 radeon_get_vblank_counter_kms(struct drm_device *dev, int crtc)
{
	struct radeon_device *rdev = dev->dev_private;

	if (crtc < 0 || crtc >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %d\n", crtc);
		return -EINVAL;
	}

	return radeon_get_vblank_counter(rdev, crtc);
}

int radeon_enable_vblank_kms(struct drm_device *dev, int crtc)
{
	struct radeon_device *rdev = dev->dev_private;

	if (crtc < 0 || crtc >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %d\n", crtc);
		return -EINVAL;
	}

	rdev->irq.crtc_vblank_int[crtc] = true;

	return radeon_irq_set(rdev);
}

void radeon_disable_vblank_kms(struct drm_device *dev, int crtc)
{
	struct radeon_device *rdev = dev->dev_private;

	if (crtc < 0 || crtc >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %d\n", crtc);
		return;
	}

	rdev->irq.crtc_vblank_int[crtc] = false;

	radeon_irq_set(rdev);
}

int radeon_get_vblank_timestamp_kms(struct drm_device *dev, int crtc,
				    int *max_error,
				    struct timeval *vblank_time,
				    unsigned flags)
{
	struct drm_crtc *drmcrtc;
	struct radeon_device *rdev = dev->dev_private;

	if (crtc < 0 || crtc >= dev->num_crtcs) {
		DRM_ERROR("Invalid crtc %d\n", crtc);
		return -EINVAL;
	}

	/* Get associated drm_crtc: */
	drmcrtc = &rdev->mode_info.crtcs[crtc]->base;
	if (!drmcrtc)
		return -EINVAL;

	/* Helper routine in DRM core does all the work: */
	return drm_calc_vbltimestamp_from_scanoutpos(dev, crtc, max_error,
						     vblank_time, flags,
						     drmcrtc);
}

/*
 * IOCTL.
 */
int radeon_dma_ioctl_kms(struct drm_device *dev, void *data,
			 struct drm_file *file_priv)
{
	/* Not valid in KMS. */
	return -EINVAL;
}

#define KMS_INVALID_IOCTL(name)						\
int name(struct drm_device *dev, void *data, struct drm_file *file_priv)\
{									\
	DRM_ERROR("invalid ioctl with kms %s\n", __func__);		\
	return -EINVAL;							\
}

/*
 * All these ioctls are invalid in kms world.
 */
KMS_INVALID_IOCTL(radeon_cp_init_kms)
KMS_INVALID_IOCTL(radeon_cp_start_kms)
KMS_INVALID_IOCTL(radeon_cp_stop_kms)
KMS_INVALID_IOCTL(radeon_cp_reset_kms)
KMS_INVALID_IOCTL(radeon_cp_idle_kms)
KMS_INVALID_IOCTL(radeon_cp_resume_kms)
KMS_INVALID_IOCTL(radeon_engine_reset_kms)
KMS_INVALID_IOCTL(radeon_fullscreen_kms)
KMS_INVALID_IOCTL(radeon_cp_swap_kms)
KMS_INVALID_IOCTL(radeon_cp_clear_kms)
KMS_INVALID_IOCTL(radeon_cp_vertex_kms)
KMS_INVALID_IOCTL(radeon_cp_indices_kms)
KMS_INVALID_IOCTL(radeon_cp_texture_kms)
KMS_INVALID_IOCTL(radeon_cp_stipple_kms)
KMS_INVALID_IOCTL(radeon_cp_indirect_kms)
KMS_INVALID_IOCTL(radeon_cp_vertex2_kms)
KMS_INVALID_IOCTL(radeon_cp_cmdbuf_kms)
KMS_INVALID_IOCTL(radeon_cp_getparam_kms)
KMS_INVALID_IOCTL(radeon_cp_flip_kms)
KMS_INVALID_IOCTL(radeon_mem_alloc_kms)
KMS_INVALID_IOCTL(radeon_mem_free_kms)
KMS_INVALID_IOCTL(radeon_mem_init_heap_kms)
KMS_INVALID_IOCTL(radeon_irq_emit_kms)
KMS_INVALID_IOCTL(radeon_irq_wait_kms)
KMS_INVALID_IOCTL(radeon_cp_setparam_kms)
KMS_INVALID_IOCTL(radeon_surface_alloc_kms)
KMS_INVALID_IOCTL(radeon_surface_free_kms)


struct drm_ioctl_desc radeon_ioctls_kms[] = {
	DRM_IOCTL_DEF_DRV(RADEON_CP_INIT, radeon_cp_init_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_CP_START, radeon_cp_start_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_CP_STOP, radeon_cp_stop_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_CP_RESET, radeon_cp_reset_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_CP_IDLE, radeon_cp_idle_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_CP_RESUME, radeon_cp_resume_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_RESET, radeon_engine_reset_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_FULLSCREEN, radeon_fullscreen_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_SWAP, radeon_cp_swap_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_CLEAR, radeon_cp_clear_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_VERTEX, radeon_cp_vertex_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_INDICES, radeon_cp_indices_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_TEXTURE, radeon_cp_texture_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_STIPPLE, radeon_cp_stipple_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_INDIRECT, radeon_cp_indirect_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_VERTEX2, radeon_cp_vertex2_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_CMDBUF, radeon_cp_cmdbuf_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_GETPARAM, radeon_cp_getparam_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_FLIP, radeon_cp_flip_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_ALLOC, radeon_mem_alloc_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_FREE, radeon_mem_free_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_INIT_HEAP, radeon_mem_init_heap_kms, DRM_AUTH|DRM_MASTER|DRM_ROOT_ONLY),
	DRM_IOCTL_DEF_DRV(RADEON_IRQ_EMIT, radeon_irq_emit_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_IRQ_WAIT, radeon_irq_wait_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_SETPARAM, radeon_cp_setparam_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_SURF_ALLOC, radeon_surface_alloc_kms, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(RADEON_SURF_FREE, radeon_surface_free_kms, DRM_AUTH),
	/* KMS */
	DRM_IOCTL_DEF_DRV(RADEON_GEM_INFO, radeon_gem_info_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_CREATE, radeon_gem_create_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_MMAP, radeon_gem_mmap_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_SET_DOMAIN, radeon_gem_set_domain_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_PREAD, radeon_gem_pread_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_PWRITE, radeon_gem_pwrite_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_WAIT_IDLE, radeon_gem_wait_idle_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_CS, radeon_cs_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_INFO, radeon_info_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_SET_TILING, radeon_gem_set_tiling_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_GET_TILING, radeon_gem_get_tiling_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_BUSY, radeon_gem_busy_ioctl, DRM_AUTH|DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(RADEON_GEM_VA, radeon_gem_va_ioctl, DRM_AUTH|DRM_UNLOCKED),
};
int radeon_max_kms_ioctl = DRM_ARRAY_SIZE(radeon_ioctls_kms);
=======
/**
 * radeon_get_vblank_counter_kms - get frame count
 *
 * @crtc: crtc to get the frame count from
 *
 * Gets the frame count on the requested crtc (all asics).
 * Returns frame count on success, -EINVAL on failure.
 */
u32 radeon_get_vblank_counter_kms(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	unsigned int pipe = crtc->index;
	int vpos, hpos, stat;
	u32 count;
	struct radeon_device *rdev = dev->dev_private;

	if (pipe >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %u\n", pipe);
		return -EINVAL;
	}

	/* The hw increments its frame counter at start of vsync, not at start
	 * of vblank, as is required by DRM core vblank counter handling.
	 * Cook the hw count here to make it appear to the caller as if it
	 * incremented at start of vblank. We measure distance to start of
	 * vblank in vpos. vpos therefore will be >= 0 between start of vblank
	 * and start of vsync, so vpos >= 0 means to bump the hw frame counter
	 * result by 1 to give the proper appearance to caller.
	 */
	if (rdev->mode_info.crtcs[pipe]) {
		/* Repeat readout if needed to provide stable result if
		 * we cross start of vsync during the queries.
		 */
		do {
			count = radeon_get_vblank_counter(rdev, pipe);
			/* Ask radeon_get_crtc_scanoutpos to return vpos as
			 * distance to start of vblank, instead of regular
			 * vertical scanout pos.
			 */
			stat = radeon_get_crtc_scanoutpos(
				dev, pipe, GET_DISTANCE_TO_VBLANKSTART,
				&vpos, &hpos, NULL, NULL,
				&rdev->mode_info.crtcs[pipe]->base.hwmode);
		} while (count != radeon_get_vblank_counter(rdev, pipe));

		if (((stat & (DRM_SCANOUTPOS_VALID | DRM_SCANOUTPOS_ACCURATE)) !=
		    (DRM_SCANOUTPOS_VALID | DRM_SCANOUTPOS_ACCURATE))) {
			DRM_DEBUG_VBL("Query failed! stat %d\n", stat);
		}
		else {
			DRM_DEBUG_VBL("crtc %u: dist from vblank start %d\n",
				      pipe, vpos);

			/* Bump counter if we are at >= leading edge of vblank,
			 * but before vsync where vpos would turn negative and
			 * the hw counter really increments.
			 */
			if (vpos >= 0)
				count++;
		}
	}
	else {
	    /* Fallback to use value as is. */
	    count = radeon_get_vblank_counter(rdev, pipe);
	    DRM_DEBUG_VBL("NULL mode info! Returned count may be wrong.\n");
	}

	return count;
}

/**
 * radeon_enable_vblank_kms - enable vblank interrupt
 *
 * @crtc: crtc to enable vblank interrupt for
 *
 * Enable the interrupt on the requested crtc (all asics).
 * Returns 0 on success, -EINVAL on failure.
 */
int radeon_enable_vblank_kms(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	unsigned int pipe = crtc->index;
	struct radeon_device *rdev = dev->dev_private;
	unsigned long irqflags;
	int r;

	if (pipe >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %d\n", pipe);
		return -EINVAL;
	}

	spin_lock_irqsave(&rdev->irq.lock, irqflags);
	rdev->irq.crtc_vblank_int[pipe] = true;
	r = radeon_irq_set(rdev);
	spin_unlock_irqrestore(&rdev->irq.lock, irqflags);
	return r;
}

/**
 * radeon_disable_vblank_kms - disable vblank interrupt
 *
 * @crtc: crtc to disable vblank interrupt for
 *
 * Disable the interrupt on the requested crtc (all asics).
 */
void radeon_disable_vblank_kms(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	unsigned int pipe = crtc->index;
	struct radeon_device *rdev = dev->dev_private;
	unsigned long irqflags;

	if (pipe >= rdev->num_crtc) {
		DRM_ERROR("Invalid crtc %d\n", pipe);
		return;
	}

	spin_lock_irqsave(&rdev->irq.lock, irqflags);
	rdev->irq.crtc_vblank_int[pipe] = false;
	radeon_irq_set(rdev);
	spin_unlock_irqrestore(&rdev->irq.lock, irqflags);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
