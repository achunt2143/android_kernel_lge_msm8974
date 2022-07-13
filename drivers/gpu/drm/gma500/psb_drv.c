<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**************************************************************************
 * Copyright (c) 2007-2011, Intel Corporation.
 * All Rights Reserved.
 * Copyright (c) 2008, Tungsten Graphics, Inc. Cedar Park, TX., USA.
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
 **************************************************************************/

#include <drm/drmP.h>
#include <drm/drm.h>
#include "gma_drm.h"
#include "psb_drv.h"
#include "framebuffer.h"
#include "psb_reg.h"
#include "psb_intel_reg.h"
#include "intel_bios.h"
#include "mid_bios.h"
#include <drm/drm_pciids.h>
#include "power.h"
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/spinlock.h>
#include <linux/pm_runtime.h>
#include <acpi/video.h>
#include <linux/module.h>

static int drm_psb_trap_pagefaults;

static int psb_probe(struct pci_dev *pdev, const struct pci_device_id *ent);

MODULE_PARM_DESC(trap_pagefaults, "Error and reset on MMU pagefaults");
module_param_named(trap_pagefaults, drm_psb_trap_pagefaults, int, 0600);


static DEFINE_PCI_DEVICE_TABLE(pciidlist) = {
	{ 0x8086, 0x8108, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &psb_chip_ops },
	{ 0x8086, 0x8109, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &psb_chip_ops },
#if defined(CONFIG_DRM_GMA600)
	{ 0x8086, 0x4100, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4101, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4102, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4103, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4104, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4105, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4106, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	{ 0x8086, 0x4107, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
	/* Atom E620 */
	{ 0x8086, 0x4108, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops},
#endif
#if defined(CONFIG_DRM_MEDFIELD)
	{0x8086, 0x0130, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0131, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0132, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0133, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0134, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0135, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0136, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
	{0x8086, 0x0137, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &mdfld_chip_ops},
#endif
#if defined(CONFIG_DRM_GMA3600)
	{ 0x8086, 0x0be0, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be1, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be2, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be3, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be4, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be5, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be6, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
	{ 0x8086, 0x0be7, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops},
#endif
=======
 **************************************************************************/

#include <linux/aperture.h>
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/pm_runtime.h>
#include <linux/spinlock.h>
#include <linux/delay.h>

#include <asm/set_memory.h>

#include <acpi/video.h>

#include <drm/drm.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_pciids.h>
#include <drm/drm_vblank.h>

#include "framebuffer.h"
#include "gem.h"
#include "intel_bios.h"
#include "mid_bios.h"
#include "power.h"
#include "psb_drv.h"
#include "psb_intel_reg.h"
#include "psb_irq.h"
#include "psb_reg.h"

static const struct drm_driver driver;
static int psb_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent);

/*
 * The table below contains a mapping of the PCI vendor ID and the PCI Device ID
 * to the different groups of PowerVR 5-series chip designs
 *
 * 0x8086 = Intel Corporation
 *
 * PowerVR SGX535    - Poulsbo    - Intel GMA 500, Intel Atom Z5xx
 * PowerVR SGX535    - Moorestown - Intel GMA 600
 * PowerVR SGX535    - Oaktrail   - Intel GMA 600, Intel Atom Z6xx, E6xx
 * PowerVR SGX545    - Cedartrail - Intel GMA 3600, Intel Atom D2500, N2600
 * PowerVR SGX545    - Cedartrail - Intel GMA 3650, Intel Atom D2550, D2700,
 *                                  N2800
 */
static const struct pci_device_id pciidlist[] = {
	/* Poulsbo */
	{ 0x8086, 0x8108, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &psb_chip_ops },
	{ 0x8086, 0x8109, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &psb_chip_ops },
	/* Oak Trail */
	{ 0x8086, 0x4100, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4101, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4102, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4103, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4104, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4105, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4106, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4107, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	{ 0x8086, 0x4108, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &oaktrail_chip_ops },
	/* Cedar Trail */
	{ 0x8086, 0x0be0, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be1, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be2, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be3, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be4, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be5, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be6, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be7, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be8, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0be9, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0bea, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0beb, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0bec, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0bed, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0bee, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
	{ 0x8086, 0x0bef, PCI_ANY_ID, PCI_ANY_ID, 0, 0, (long) &cdv_chip_ops },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, pciidlist);

/*
 * Standard IOCTLs.
 */
<<<<<<< HEAD

#define DRM_IOCTL_GMA_ADB	\
		DRM_IOWR(DRM_GMA_ADB + DRM_COMMAND_BASE, uint32_t)
#define DRM_IOCTL_GMA_MODE_OPERATION	\
		DRM_IOWR(DRM_GMA_MODE_OPERATION + DRM_COMMAND_BASE, \
			 struct drm_psb_mode_operation_arg)
#define DRM_IOCTL_GMA_STOLEN_MEMORY	\
		DRM_IOWR(DRM_GMA_STOLEN_MEMORY + DRM_COMMAND_BASE, \
			 struct drm_psb_stolen_memory_arg)
#define DRM_IOCTL_GMA_GAMMA	\
		DRM_IOWR(DRM_GMA_GAMMA + DRM_COMMAND_BASE, \
			 struct drm_psb_dpst_lut_arg)
#define DRM_IOCTL_GMA_DPST_BL	\
		DRM_IOWR(DRM_GMA_DPST_BL + DRM_COMMAND_BASE, \
			 uint32_t)
#define DRM_IOCTL_GMA_GET_PIPE_FROM_CRTC_ID	\
		DRM_IOWR(DRM_GMA_GET_PIPE_FROM_CRTC_ID + DRM_COMMAND_BASE, \
			 struct drm_psb_get_pipe_from_crtc_id_arg)
#define DRM_IOCTL_GMA_GEM_CREATE	\
		DRM_IOWR(DRM_GMA_GEM_CREATE + DRM_COMMAND_BASE, \
			 struct drm_psb_gem_create)
#define DRM_IOCTL_GMA_GEM_MMAP	\
		DRM_IOWR(DRM_GMA_GEM_MMAP + DRM_COMMAND_BASE, \
			 struct drm_psb_gem_mmap)

static int psb_adb_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
static int psb_mode_operation_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv);
static int psb_stolen_memory_ioctl(struct drm_device *dev, void *data,
				   struct drm_file *file_priv);
static int psb_gamma_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
static int psb_dpst_bl_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);

static struct drm_ioctl_desc psb_ioctls[] = {
	DRM_IOCTL_DEF_DRV(GMA_ADB, psb_adb_ioctl, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_MODE_OPERATION, psb_mode_operation_ioctl,
		      DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_STOLEN_MEMORY, psb_stolen_memory_ioctl,
		      DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_GAMMA, psb_gamma_ioctl, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_DPST_BL, psb_dpst_bl_ioctl, DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_GET_PIPE_FROM_CRTC_ID,
					psb_intel_get_pipe_from_crtc_id, 0),
	DRM_IOCTL_DEF_DRV(GMA_GEM_CREATE, psb_gem_create_ioctl,
						DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(GMA_GEM_MMAP, psb_gem_mmap_ioctl,
						DRM_UNLOCKED | DRM_AUTH),
};

static void psb_lastclose(struct drm_device *dev)
{
	return;
}

static void psb_do_takedown(struct drm_device *dev)
{
=======
static const struct drm_ioctl_desc psb_ioctls[] = {
};

/**
 *	psb_spank		-	reset the 2D engine
 *	@dev_priv: our PSB DRM device
 *
 *	Soft reset the graphics engine and then reload the necessary registers.
 */
static void psb_spank(struct drm_psb_private *dev_priv)
{
	PSB_WSGX32(_PSB_CS_RESET_BIF_RESET | _PSB_CS_RESET_DPM_RESET |
		_PSB_CS_RESET_TA_RESET | _PSB_CS_RESET_USE_RESET |
		_PSB_CS_RESET_ISP_RESET | _PSB_CS_RESET_TSP_RESET |
		_PSB_CS_RESET_TWOD_RESET, PSB_CR_SOFT_RESET);
	PSB_RSGX32(PSB_CR_SOFT_RESET);

	msleep(1);

	PSB_WSGX32(0, PSB_CR_SOFT_RESET);
	wmb();
	PSB_WSGX32(PSB_RSGX32(PSB_CR_BIF_CTRL) | _PSB_CB_CTRL_CLEAR_FAULT,
		   PSB_CR_BIF_CTRL);
	wmb();
	(void) PSB_RSGX32(PSB_CR_BIF_CTRL);

	msleep(1);
	PSB_WSGX32(PSB_RSGX32(PSB_CR_BIF_CTRL) & ~_PSB_CB_CTRL_CLEAR_FAULT,
		   PSB_CR_BIF_CTRL);
	(void) PSB_RSGX32(PSB_CR_BIF_CTRL);
	PSB_WSGX32(dev_priv->gtt.gatt_start, PSB_CR_BIF_TWOD_REQ_BASE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int psb_do_init(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_psb_private *dev_priv = dev->dev_private;
=======
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct psb_gtt *pg = &dev_priv->gtt;

	uint32_t stolen_gtt;

<<<<<<< HEAD
	int ret = -ENOMEM;

	if (pg->mmu_gatt_start & 0x0FFFFFFF) {
		dev_err(dev->dev, "Gatt must be 256M aligned. This is a bug.\n");
		ret = -EINVAL;
		goto out_err;
	}


	stolen_gtt = (pg->stolen_size >> PAGE_SHIFT) * 4;
	stolen_gtt = (stolen_gtt + PAGE_SIZE - 1) >> PAGE_SHIFT;
	stolen_gtt =
	    (stolen_gtt < pg->gtt_pages) ? stolen_gtt : pg->gtt_pages;
=======
	if (pg->mmu_gatt_start & 0x0FFFFFFF) {
		dev_err(dev->dev, "Gatt must be 256M aligned. This is a bug.\n");
		return -EINVAL;
	}

	stolen_gtt = (pg->stolen_size >> PAGE_SHIFT) * 4;
	stolen_gtt = (stolen_gtt + PAGE_SIZE - 1) >> PAGE_SHIFT;
	stolen_gtt = (stolen_gtt < pg->gtt_pages) ? stolen_gtt : pg->gtt_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_priv->gatt_free_offset = pg->mmu_gatt_start +
	    (stolen_gtt << PAGE_SHIFT) * 1024;

<<<<<<< HEAD
	if (1 || drm_debug) {
		uint32_t core_id = PSB_RSGX32(PSB_CR_CORE_ID);
		uint32_t core_rev = PSB_RSGX32(PSB_CR_CORE_REVISION);
		DRM_INFO("SGX core id = 0x%08x\n", core_id);
		DRM_INFO("SGX core rev major = 0x%02x, minor = 0x%02x\n",
			 (core_rev & _PSB_CC_REVISION_MAJOR_MASK) >>
			 _PSB_CC_REVISION_MAJOR_SHIFT,
			 (core_rev & _PSB_CC_REVISION_MINOR_MASK) >>
			 _PSB_CC_REVISION_MINOR_SHIFT);
		DRM_INFO
		    ("SGX core rev maintenance = 0x%02x, designer = 0x%02x\n",
		     (core_rev & _PSB_CC_REVISION_MAINTENANCE_MASK) >>
		     _PSB_CC_REVISION_MAINTENANCE_SHIFT,
		     (core_rev & _PSB_CC_REVISION_DESIGNER_MASK) >>
		     _PSB_CC_REVISION_DESIGNER_SHIFT);
	}


	spin_lock_init(&dev_priv->irqmask_lock);
	spin_lock_init(&dev_priv->lock_2d);
=======
	spin_lock_init(&dev_priv->irqmask_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	PSB_WSGX32(0x00000000, PSB_CR_BIF_BANK0);
	PSB_WSGX32(0x00000000, PSB_CR_BIF_BANK1);
	PSB_RSGX32(PSB_CR_BIF_BANK1);
<<<<<<< HEAD
	PSB_WSGX32(PSB_RSGX32(PSB_CR_BIF_CTRL) | _PSB_MMU_ER_MASK,
							PSB_CR_BIF_CTRL);
=======

	/* Do not bypass any MMU access, let them pagefault instead */
	PSB_WSGX32((PSB_RSGX32(PSB_CR_BIF_CTRL) & ~_PSB_MMU_ER_MASK),
		   PSB_CR_BIF_CTRL);
	PSB_RSGX32(PSB_CR_BIF_CTRL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	psb_spank(dev_priv);

	/* mmu_gatt ?? */
	PSB_WSGX32(pg->gatt_start, PSB_CR_BIF_TWOD_REQ_BASE);
<<<<<<< HEAD
	return 0;
out_err:
	psb_do_takedown(dev);
	return ret;
}

static int psb_driver_unload(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;

	/* Kill vblank etc here */

	gma_backlight_exit(dev);

	psb_modeset_cleanup(dev);

	if (dev_priv) {
		psb_lid_timer_takedown(dev_priv);
		gma_intel_opregion_exit(dev);

		if (dev_priv->ops->chip_teardown)
			dev_priv->ops->chip_teardown(dev);
		psb_do_takedown(dev);


		if (dev_priv->pf_pd) {
			psb_mmu_free_pagedir(dev_priv->pf_pd);
			dev_priv->pf_pd = NULL;
		}
		if (dev_priv->mmu) {
			struct psb_gtt *pg = &dev_priv->gtt;

			down_read(&pg->sem);
			psb_mmu_remove_pfn_sequence(
				psb_mmu_get_default_pd
				(dev_priv->mmu),
				pg->mmu_gatt_start,
				dev_priv->vram_stolen_size >> PAGE_SHIFT);
			up_read(&pg->sem);
			psb_mmu_driver_takedown(dev_priv->mmu);
			dev_priv->mmu = NULL;
		}
		psb_gtt_takedown(dev);
		if (dev_priv->scratch_page) {
			__free_page(dev_priv->scratch_page);
			dev_priv->scratch_page = NULL;
		}
		if (dev_priv->vdc_reg) {
			iounmap(dev_priv->vdc_reg);
			dev_priv->vdc_reg = NULL;
		}
		if (dev_priv->sgx_reg) {
			iounmap(dev_priv->sgx_reg);
			dev_priv->sgx_reg = NULL;
		}

		kfree(dev_priv);
		dev->dev_private = NULL;

		/*destroy VBT data*/
		psb_intel_destroy_bios(dev);
	}

	gma_power_uninit(dev);

	return 0;
}


static int psb_driver_load(struct drm_device *dev, unsigned long chipset)
{
	struct drm_psb_private *dev_priv;
	unsigned long resource_start;
	unsigned long irqflags;
	int ret = -ENOMEM;
	struct drm_connector *connector;
	struct psb_intel_encoder *psb_intel_encoder;

	dev_priv = kzalloc(sizeof(*dev_priv), GFP_KERNEL);
	if (dev_priv == NULL)
		return -ENOMEM;

	dev_priv->ops = (struct psb_ops *)chipset;
	dev_priv->dev = dev;
	dev->dev_private = (void *) dev_priv;

	pci_set_master(dev->pdev);

	if (!IS_PSB(dev)) {
		if (pci_enable_msi(dev->pdev))
			dev_warn(dev->dev, "Enabling MSI failed!\n");
	}

	dev_priv->num_pipe = dev_priv->ops->pipes;

	resource_start = pci_resource_start(dev->pdev, PSB_MMIO_RESOURCE);
=======
	PSB_RSGX32(PSB_CR_BIF_TWOD_REQ_BASE); /* Post */

	return 0;
}

static void psb_driver_unload(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);

	/* TODO: Kill vblank etc here */

	gma_backlight_exit(dev);
	psb_modeset_cleanup(dev);

	gma_irq_uninstall(dev);

	if (dev_priv->ops->chip_teardown)
		dev_priv->ops->chip_teardown(dev);

	psb_intel_opregion_fini(dev);

	if (dev_priv->pf_pd) {
		psb_mmu_free_pagedir(dev_priv->pf_pd);
		dev_priv->pf_pd = NULL;
	}
	if (dev_priv->mmu) {
		struct psb_gtt *pg = &dev_priv->gtt;

		psb_mmu_remove_pfn_sequence(
			psb_mmu_get_default_pd
			(dev_priv->mmu),
			pg->mmu_gatt_start,
			dev_priv->vram_stolen_size >> PAGE_SHIFT);
		psb_mmu_driver_takedown(dev_priv->mmu);
		dev_priv->mmu = NULL;
	}
	psb_gem_mm_fini(dev);
	psb_gtt_fini(dev);
	if (dev_priv->scratch_page) {
		set_pages_wb(dev_priv->scratch_page, 1);
		__free_page(dev_priv->scratch_page);
		dev_priv->scratch_page = NULL;
	}
	if (dev_priv->vdc_reg) {
		iounmap(dev_priv->vdc_reg);
		dev_priv->vdc_reg = NULL;
	}
	if (dev_priv->sgx_reg) {
		iounmap(dev_priv->sgx_reg);
		dev_priv->sgx_reg = NULL;
	}
	if (dev_priv->aux_reg) {
		iounmap(dev_priv->aux_reg);
		dev_priv->aux_reg = NULL;
	}
	pci_dev_put(dev_priv->aux_pdev);
	pci_dev_put(dev_priv->lpc_pdev);

	/* Destroy VBT data */
	psb_intel_destroy_bios(dev);

	gma_power_uninit(dev);
}

static void psb_device_release(void *data)
{
	struct drm_device *dev = data;

	psb_driver_unload(dev);
}

static int psb_driver_load(struct drm_device *dev, unsigned long flags)
{
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	unsigned long resource_start, resource_len;
	unsigned long irqflags;
	struct drm_connector_list_iter conn_iter;
	struct drm_connector *connector;
	struct gma_encoder *gma_encoder;
	struct psb_gtt *pg;
	int ret = -ENOMEM;

	/* initializing driver private data */

	dev_priv->ops = (struct psb_ops *)flags;

	pg = &dev_priv->gtt;

	pci_set_master(pdev);

	dev_priv->num_pipe = dev_priv->ops->pipes;

	resource_start = pci_resource_start(pdev, PSB_MMIO_RESOURCE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev_priv->vdc_reg =
	    ioremap(resource_start + PSB_VDC_OFFSET, PSB_VDC_SIZE);
	if (!dev_priv->vdc_reg)
		goto out_err;

	dev_priv->sgx_reg = ioremap(resource_start + dev_priv->ops->sgx_offset,
							PSB_SGX_SIZE);
	if (!dev_priv->sgx_reg)
		goto out_err;

<<<<<<< HEAD
=======
	if (IS_MRST(dev)) {
		int domain = pci_domain_nr(pdev->bus);

		dev_priv->aux_pdev =
			pci_get_domain_bus_and_slot(domain, 0,
						    PCI_DEVFN(3, 0));

		if (dev_priv->aux_pdev) {
			resource_start = pci_resource_start(dev_priv->aux_pdev,
							    PSB_AUX_RESOURCE);
			resource_len = pci_resource_len(dev_priv->aux_pdev,
							PSB_AUX_RESOURCE);
			dev_priv->aux_reg = ioremap(resource_start,
							    resource_len);
			if (!dev_priv->aux_reg)
				goto out_err;

			DRM_DEBUG_KMS("Found aux vdc");
		} else {
			/* Couldn't find the aux vdc so map to primary vdc */
			dev_priv->aux_reg = dev_priv->vdc_reg;
			DRM_DEBUG_KMS("Couldn't find aux pci device");
		}
		dev_priv->gmbus_reg = dev_priv->aux_reg;

		dev_priv->lpc_pdev =
			pci_get_domain_bus_and_slot(domain, 0,
						    PCI_DEVFN(31, 0));
		if (dev_priv->lpc_pdev) {
			pci_read_config_word(dev_priv->lpc_pdev, PSB_LPC_GBA,
				&dev_priv->lpc_gpio_base);
			pci_write_config_dword(dev_priv->lpc_pdev, PSB_LPC_GBA,
				(u32)dev_priv->lpc_gpio_base | (1L<<31));
			pci_read_config_word(dev_priv->lpc_pdev, PSB_LPC_GBA,
				&dev_priv->lpc_gpio_base);
			dev_priv->lpc_gpio_base &= 0xffc0;
			if (dev_priv->lpc_gpio_base)
				DRM_DEBUG_KMS("Found LPC GPIO at 0x%04x\n",
						dev_priv->lpc_gpio_base);
			else {
				pci_dev_put(dev_priv->lpc_pdev);
				dev_priv->lpc_pdev = NULL;
			}
		}
	} else {
		dev_priv->gmbus_reg = dev_priv->vdc_reg;
	}

	psb_intel_opregion_setup(dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = dev_priv->ops->chip_setup(dev);
	if (ret)
		goto out_err;

	/* Init OSPM support */
	gma_power_init(dev);

	ret = -ENOMEM;

	dev_priv->scratch_page = alloc_page(GFP_DMA32 | __GFP_ZERO);
	if (!dev_priv->scratch_page)
		goto out_err;

	set_pages_uc(dev_priv->scratch_page, 1);

<<<<<<< HEAD
	ret = psb_gtt_init(dev, 0);
	if (ret)
		goto out_err;

	dev_priv->mmu = psb_mmu_driver_init((void *)0,
					drm_psb_trap_pagefaults, 0,
					dev_priv);
=======
	ret = psb_gtt_init(dev);
	if (ret)
		goto out_err;
	ret = psb_gem_mm_init(dev);
	if (ret)
		goto out_err;

	ret = -ENOMEM;

	dev_priv->mmu = psb_mmu_driver_init(dev, 1, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dev_priv->mmu)
		goto out_err;

	dev_priv->pf_pd = psb_mmu_alloc_pd(dev_priv->mmu, 1, 0);
	if (!dev_priv->pf_pd)
		goto out_err;

<<<<<<< HEAD
	psb_mmu_set_pd_context(psb_mmu_get_default_pd(dev_priv->mmu), 0);
	psb_mmu_set_pd_context(dev_priv->pf_pd, 1);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = psb_do_init(dev);
	if (ret)
		return ret;

<<<<<<< HEAD
	PSB_WSGX32(0x20000000, PSB_CR_PDS_EXEC_BASE);
	PSB_WSGX32(0x30000000, PSB_CR_BIF_3D_REQ_BASE);

/*	igd_opregion_init(&dev_priv->opregion_dev); */
/*	acpi_video_register(); */
	if (dev_priv->lid_state)
		psb_lid_timer_init(dev_priv);

=======
	/* Add stolen memory to SGX MMU */
	ret = psb_mmu_insert_pfn_sequence(psb_mmu_get_default_pd(dev_priv->mmu),
					  dev_priv->stolen_base >> PAGE_SHIFT,
					  pg->gatt_start,
					  pg->stolen_size >> PAGE_SHIFT, 0);

	psb_mmu_set_pd_context(psb_mmu_get_default_pd(dev_priv->mmu), 0);
	psb_mmu_set_pd_context(dev_priv->pf_pd, 1);

	PSB_WSGX32(0x20000000, PSB_CR_PDS_EXEC_BASE);
	PSB_WSGX32(0x30000000, PSB_CR_BIF_3D_REQ_BASE);

	acpi_video_register();

	/* Setup vertical blanking handling */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = drm_vblank_init(dev, dev_priv->num_pipe);
	if (ret)
		goto out_err;

	/*
	 * Install interrupt handlers prior to powering off SGX or else we will
	 * crash.
	 */
	dev_priv->vdc_irq_mask = 0;
	dev_priv->pipestat[0] = 0;
	dev_priv->pipestat[1] = 0;
	dev_priv->pipestat[2] = 0;
	spin_lock_irqsave(&dev_priv->irqmask_lock, irqflags);
	PSB_WVDC32(0xFFFFFFFF, PSB_HWSTAM);
	PSB_WVDC32(0x00000000, PSB_INT_ENABLE_R);
	PSB_WVDC32(0xFFFFFFFF, PSB_INT_MASK_R);
	spin_unlock_irqrestore(&dev_priv->irqmask_lock, irqflags);
<<<<<<< HEAD
	if (IS_PSB(dev) && drm_core_check_feature(dev, DRIVER_MODESET))
		drm_irq_install(dev);

	dev->vblank_disable_allowed = 1;

	dev->max_vblank_count = 0xffffff; /* only 24 bits of frame count */

	dev->driver->get_vblank_counter = psb_get_vblank_counter;

	psb_modeset_init(dev);
	psb_fbdev_init(dev);
	drm_kms_helper_poll_init(dev);

	/* Only add backlight support if we have LVDS output */
	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    head) {
		psb_intel_encoder = psb_intel_attached_encoder(connector);

		switch (psb_intel_encoder->type) {
		case INTEL_OUTPUT_LVDS:
		case INTEL_OUTPUT_MIPI:
			ret = gma_backlight_init(dev);
			break;
		}
	}

	if (ret)
		return ret;
#if 0
	/*enable runtime pm at last*/
	pm_runtime_enable(&dev->pdev->dev);
	pm_runtime_set_active(&dev->pdev->dev);
#endif
	/*Intel drm driver load is done, continue doing pvr load*/
	return 0;
=======

	gma_irq_install(dev);

	dev->max_vblank_count = 0xffffff; /* only 24 bits of frame count */

	psb_modeset_init(dev);
	drm_kms_helper_poll_init(dev);

	/* Only add backlight support if we have LVDS or MIPI output */
	drm_connector_list_iter_begin(dev, &conn_iter);
	drm_for_each_connector_iter(connector, &conn_iter) {
		gma_encoder = gma_attached_encoder(connector);

		if (gma_encoder->type == INTEL_OUTPUT_LVDS ||
		    gma_encoder->type == INTEL_OUTPUT_MIPI) {
			ret = gma_backlight_init(dev);
			if (ret == 0)
				acpi_video_register_backlight();
			break;
		}
	}
	drm_connector_list_iter_end(&conn_iter);

	if (ret)
		return ret;
	psb_intel_opregion_enable_asle(dev);

	return devm_add_action_or_reset(dev->dev, psb_device_release, dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_err:
	psb_driver_unload(dev);
	return ret;
}

<<<<<<< HEAD
static int psb_driver_device_is_agp(struct drm_device *dev)
{
	return 0;
}

static inline void get_brightness(struct backlight_device *bd)
{
#ifdef CONFIG_BACKLIGHT_CLASS_DEVICE
	if (bd) {
		bd->props.brightness = bd->ops->get_brightness(bd);
		backlight_update_status(bd);
	}
#endif
}

static int psb_dpst_bl_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	struct drm_psb_private *dev_priv = psb_priv(dev);
	uint32_t *arg = data;

	dev_priv->blc_adj2 = *arg;
	get_brightness(dev_priv->backlight_device);
	return 0;
}

static int psb_adb_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv)
{
	struct drm_psb_private *dev_priv = psb_priv(dev);
	uint32_t *arg = data;

	dev_priv->blc_adj1 = *arg;
	get_brightness(dev_priv->backlight_device);
	return 0;
}

static int psb_gamma_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct drm_psb_dpst_lut_arg *lut_arg = data;
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
	struct drm_connector *connector;
	struct psb_intel_crtc *psb_intel_crtc;
	int i = 0;
	int32_t obj_id;

	obj_id = lut_arg->output_id;
	obj = drm_mode_object_find(dev, obj_id, DRM_MODE_OBJECT_CONNECTOR);
	if (!obj) {
		dev_dbg(dev->dev, "Invalid Connector object.\n");
		return -EINVAL;
	}

	connector = obj_to_connector(obj);
	crtc = connector->encoder->crtc;
	psb_intel_crtc = to_psb_intel_crtc(crtc);

	for (i = 0; i < 256; i++)
		psb_intel_crtc->lut_adj[i] = lut_arg->lut[i];

	psb_intel_crtc_load_lut(crtc);

	return 0;
}

static int psb_mode_operation_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	uint32_t obj_id;
	uint16_t op;
	struct drm_mode_modeinfo *umode;
	struct drm_display_mode *mode = NULL;
	struct drm_psb_mode_operation_arg *arg;
	struct drm_mode_object *obj;
	struct drm_connector *connector;
	struct drm_connector_helper_funcs *connector_funcs;
	int ret = 0;
	int resp = MODE_OK;

	arg = (struct drm_psb_mode_operation_arg *)data;
	obj_id = arg->obj_id;
	op = arg->operation;

	switch (op) {
	case PSB_MODE_OPERATION_MODE_VALID:
		umode = &arg->mode;

		mutex_lock(&dev->mode_config.mutex);

		obj = drm_mode_object_find(dev, obj_id,
					DRM_MODE_OBJECT_CONNECTOR);
		if (!obj) {
			ret = -EINVAL;
			goto mode_op_out;
		}

		connector = obj_to_connector(obj);

		mode = drm_mode_create(dev);
		if (!mode) {
			ret = -ENOMEM;
			goto mode_op_out;
		}

		/* drm_crtc_convert_umode(mode, umode); */
		{
			mode->clock = umode->clock;
			mode->hdisplay = umode->hdisplay;
			mode->hsync_start = umode->hsync_start;
			mode->hsync_end = umode->hsync_end;
			mode->htotal = umode->htotal;
			mode->hskew = umode->hskew;
			mode->vdisplay = umode->vdisplay;
			mode->vsync_start = umode->vsync_start;
			mode->vsync_end = umode->vsync_end;
			mode->vtotal = umode->vtotal;
			mode->vscan = umode->vscan;
			mode->vrefresh = umode->vrefresh;
			mode->flags = umode->flags;
			mode->type = umode->type;
			strncpy(mode->name, umode->name, DRM_DISPLAY_MODE_LEN);
			mode->name[DRM_DISPLAY_MODE_LEN-1] = 0;
		}

		connector_funcs = (struct drm_connector_helper_funcs *)
				   connector->helper_private;

		if (connector_funcs->mode_valid) {
			resp = connector_funcs->mode_valid(connector, mode);
			arg->data = resp;
		}

		/*do some clean up work*/
		if (mode)
			drm_mode_destroy(dev, mode);
mode_op_out:
		mutex_unlock(&dev->mode_config.mutex);
		return ret;

	default:
		dev_dbg(dev->dev, "Unsupported psb mode operation\n");
		return -EOPNOTSUPP;
	}
=======
/*
 * Hardware for gma500 is a hybrid device, which both acts as a PCI
 * device (for legacy vga functionality) but also more like an
 * integrated display on a SoC where the framebuffer simply
 * resides in main memory and not in a special PCI bar (that
 * internally redirects to a stolen range of main memory) like all
 * other integrated PCI display devices implement it.
 *
 * To catch all cases we need to remove conflicting firmware devices
 * for the stolen system memory and for the VGA functionality. As we
 * currently cannot easily find the framebuffer's location in stolen
 * memory, we remove all framebuffers here.
 *
 * TODO: Refactor psb_driver_load() to map vdc_reg earlier. Then
 *       we might be able to read the framebuffer range from the
 *       device.
 */
static int gma_remove_conflicting_framebuffers(struct pci_dev *pdev,
					       const struct drm_driver *req_driver)
{
	resource_size_t base = 0;
	resource_size_t size = U32_MAX; /* 4 GiB HW limit */
	const char *name = req_driver->name;
	int ret;

	ret = aperture_remove_conflicting_devices(base, size, name);
	if (ret)
		return ret;

	return __aperture_remove_legacy_vga_devices(pdev);
}

static int psb_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct drm_psb_private *dev_priv;
	struct drm_device *dev;
	int ret;

	ret = gma_remove_conflicting_framebuffers(pdev, &driver);
	if (ret)
		return ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	dev_priv = devm_drm_dev_alloc(&pdev->dev, &driver, struct drm_psb_private, dev);
	if (IS_ERR(dev_priv))
		return PTR_ERR(dev_priv);
	dev = &dev_priv->dev;

	pci_set_drvdata(pdev, dev);

	ret = psb_driver_load(dev, ent->driver_data);
	if (ret)
		return ret;

	ret = drm_dev_register(dev, ent->driver_data);
	if (ret)
		return ret;

	psb_fbdev_setup(dev_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int psb_stolen_memory_ioctl(struct drm_device *dev, void *data,
				   struct drm_file *file_priv)
{
	struct drm_psb_private *dev_priv = psb_priv(dev);
	struct drm_psb_stolen_memory_arg *arg = data;

	arg->base = dev_priv->stolen_base;
	arg->size = dev_priv->vram_stolen_size;

	return 0;
}

static int psb_driver_open(struct drm_device *dev, struct drm_file *priv)
{
	return 0;
}

static void psb_driver_close(struct drm_device *dev, struct drm_file *priv)
{
}

static long psb_unlocked_ioctl(struct file *filp, unsigned int cmd,
			       unsigned long arg)
{
	struct drm_file *file_priv = filp->private_data;
	struct drm_device *dev = file_priv->minor->dev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	static unsigned int runtime_allowed;

	if (runtime_allowed == 1 && dev_priv->is_lvds_on) {
		runtime_allowed++;
		pm_runtime_allow(&dev->pdev->dev);
		dev_priv->rpm_enabled = 1;
	}
	return drm_ioctl(filp, cmd, arg);
	/* FIXME: do we need to wrap the other side of this */
}


/* When a client dies:
 *    - Check for and clean up flipped page state
 */
static void psb_driver_preclose(struct drm_device *dev, struct drm_file *priv)
{
}

static void psb_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);
	drm_put_dev(dev);
}

static const struct dev_pm_ops psb_pm_ops = {
	.resume = gma_power_resume,
	.suspend = gma_power_suspend,
	.runtime_suspend = psb_runtime_suspend,
	.runtime_resume = psb_runtime_resume,
	.runtime_idle = psb_runtime_idle,
};

static struct vm_operations_struct psb_gem_vm_ops = {
	.fault = psb_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};
=======
static void psb_pci_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

	drm_dev_unregister(dev);
}

static DEFINE_RUNTIME_DEV_PM_OPS(psb_pm_ops, gma_power_suspend, gma_power_resume, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct file_operations psb_gem_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
<<<<<<< HEAD
	.unlocked_ioctl = psb_unlocked_ioctl,
	.mmap = drm_gem_mmap,
	.poll = drm_poll,
	.fasync = drm_fasync,
	.read = drm_read,
};

static struct drm_driver driver = {
	.driver_features = DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED | \
			   DRIVER_IRQ_VBL | DRIVER_MODESET | DRIVER_GEM ,
	.load = psb_driver_load,
	.unload = psb_driver_unload,

	.ioctls = psb_ioctls,
	.num_ioctls = DRM_ARRAY_SIZE(psb_ioctls),
	.device_is_agp = psb_driver_device_is_agp,
	.irq_preinstall = psb_irq_preinstall,
	.irq_postinstall = psb_irq_postinstall,
	.irq_uninstall = psb_irq_uninstall,
	.irq_handler = psb_irq_handler,
	.enable_vblank = psb_enable_vblank,
	.disable_vblank = psb_disable_vblank,
	.get_vblank_counter = psb_get_vblank_counter,
	.lastclose = psb_lastclose,
	.open = psb_driver_open,
	.preclose = psb_driver_preclose,
	.postclose = psb_driver_close,
	.reclaim_buffers = drm_core_reclaim_buffers,

	.gem_init_object = psb_gem_init_object,
	.gem_free_object = psb_gem_free_object,
	.gem_vm_ops = &psb_gem_vm_ops,
	.dumb_create = psb_gem_dumb_create,
	.dumb_map_offset = psb_gem_dumb_map_gtt,
	.dumb_destroy = psb_gem_dumb_destroy,
	.fops = &psb_gem_fops,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = PSB_DRM_DRIVER_DATE,
	.major = PSB_DRM_DRIVER_MAJOR,
	.minor = PSB_DRM_DRIVER_MINOR,
	.patchlevel = PSB_DRM_DRIVER_PATCHLEVEL
=======
	.unlocked_ioctl = drm_ioctl,
	.compat_ioctl = drm_compat_ioctl,
	.mmap = drm_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
};

static const struct drm_driver driver = {
	.driver_features = DRIVER_MODESET | DRIVER_GEM,

	.num_ioctls = ARRAY_SIZE(psb_ioctls),

	.dumb_create = psb_gem_dumb_create,
	.ioctls = psb_ioctls,
	.fops = &psb_gem_fops,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct pci_driver psb_pci_driver = {
	.name = DRIVER_NAME,
	.id_table = pciidlist,
<<<<<<< HEAD
	.probe = psb_probe,
	.remove = psb_remove,
	.driver = {
		.pm = &psb_pm_ops,
	}
};

static int psb_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	return drm_get_pci_dev(pdev, ent, &driver);
}

static int __init psb_init(void)
{
	return drm_pci_init(&driver, &psb_pci_driver);
=======
	.probe = psb_pci_probe,
	.remove = psb_pci_remove,
	.driver.pm = &psb_pm_ops,
};

static int __init psb_init(void)
{
	if (drm_firmware_drivers_only())
		return -ENODEV;

	return pci_register_driver(&psb_pci_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit psb_exit(void)
{
<<<<<<< HEAD
	drm_pci_exit(&driver, &psb_pci_driver);
=======
	pci_unregister_driver(&psb_pci_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

late_initcall(psb_init);
module_exit(psb_exit);

<<<<<<< HEAD
MODULE_AUTHOR("Alan Cox <alan@linux.intel.com> and others");
=======
MODULE_AUTHOR(DRIVER_AUTHOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
