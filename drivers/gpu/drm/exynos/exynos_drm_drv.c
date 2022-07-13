<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
 *	Joonyoung Shim <jy0922.shim@samsung.com>
 *	Seung-Woo Kim <sw0312.kim@samsung.com>
<<<<<<< HEAD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "drmP.h"
#include "drm.h"
#include "drm_crtc_helper.h"

#include <drm/exynos_drm.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_crtc.h"
#include "exynos_drm_encoder.h"
#include "exynos_drm_fbdev.h"
#include "exynos_drm_fb.h"
#include "exynos_drm_gem.h"
=======
 */

#include <linux/component.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/uaccess.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>
#include <drm/exynos_drm.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_fb.h"
#include "exynos_drm_fbdev.h"
#include "exynos_drm_g2d.h"
#include "exynos_drm_gem.h"
#include "exynos_drm_ipp.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "exynos_drm_plane.h"
#include "exynos_drm_vidi.h"

#define DRIVER_NAME	"exynos"
#define DRIVER_DESC	"Samsung SoC DRM"
<<<<<<< HEAD
#define DRIVER_DATE	"20110530"
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	0

#define VBLANK_OFF_DELAY	50000

static int exynos_drm_load(struct drm_device *dev, unsigned long flags)
{
	struct exynos_drm_private *private;
	int ret;
	int nr;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	private = kzalloc(sizeof(struct exynos_drm_private), GFP_KERNEL);
	if (!private) {
		DRM_ERROR("failed to allocate private\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&private->pageflip_event_list);
	dev->dev_private = (void *)private;

	drm_mode_config_init(dev);

	/* init kms poll for handling hpd */
	drm_kms_helper_poll_init(dev);

	exynos_drm_mode_config_init(dev);

	/*
	 * EXYNOS4 is enough to have two CRTCs and each crtc would be used
	 * without dependency of hardware.
	 */
	for (nr = 0; nr < MAX_CRTC; nr++) {
		ret = exynos_drm_crtc_create(dev, nr);
		if (ret)
			goto err_crtc;
	}

	for (nr = 0; nr < MAX_PLANE; nr++) {
		ret = exynos_plane_init(dev, nr);
		if (ret)
			goto err_crtc;
	}

	ret = drm_vblank_init(dev, MAX_CRTC);
	if (ret)
		goto err_crtc;

	/*
	 * probe sub drivers such as display controller and hdmi driver,
	 * that were registered at probe() of platform driver
	 * to the sub driver and create encoder and connector for them.
	 */
	ret = exynos_drm_device_register(dev);
	if (ret)
		goto err_vblank;

	/* setup possible_clones. */
	exynos_drm_encoder_setup(dev);

	/*
	 * create and configure fb helper and also exynos specific
	 * fbdev object.
	 */
	ret = exynos_drm_fbdev_init(dev);
	if (ret) {
		DRM_ERROR("failed to initialize drm fbdev\n");
		goto err_drm_device;
	}

	drm_vblank_offdelay = VBLANK_OFF_DELAY;

	return 0;

err_drm_device:
	exynos_drm_device_unregister(dev);
err_vblank:
	drm_vblank_cleanup(dev);
err_crtc:
	drm_mode_config_cleanup(dev);
	kfree(private);

	return ret;
}

static int exynos_drm_unload(struct drm_device *dev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	exynos_drm_fbdev_fini(dev);
	exynos_drm_device_unregister(dev);
	drm_vblank_cleanup(dev);
	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);
	kfree(dev->dev_private);

	dev->dev_private = NULL;

	return 0;
}

static int exynos_drm_open(struct drm_device *dev, struct drm_file *file)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	return exynos_drm_subdrv_open(dev, file);
}

static void exynos_drm_preclose(struct drm_device *dev,
					struct drm_file *file)
{
	struct exynos_drm_private *private = dev->dev_private;
	struct drm_pending_vblank_event *e, *t;
	unsigned long flags;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	/* release events of current file */
	spin_lock_irqsave(&dev->event_lock, flags);
	list_for_each_entry_safe(e, t, &private->pageflip_event_list,
			base.link) {
		if (e->base.file_priv == file) {
			list_del(&e->base.link);
			e->base.destroy(&e->base);
		}
	}
	spin_unlock_irqrestore(&dev->event_lock, flags);

	exynos_drm_subdrv_close(dev, file);
=======
#define DRIVER_DATE	"20180330"

/*
 * Interface history:
 *
 * 1.0 - Original version
 * 1.1 - Upgrade IPP driver to version 2.0
 */
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	1

static int exynos_drm_open(struct drm_device *dev, struct drm_file *file)
{
	struct drm_exynos_file_private *file_priv;
	int ret;

	file_priv = kzalloc(sizeof(*file_priv), GFP_KERNEL);
	if (!file_priv)
		return -ENOMEM;

	file->driver_priv = file_priv;
	ret = g2d_open(dev, file);
	if (ret)
		goto err_file_priv_free;

	return ret;

err_file_priv_free:
	kfree(file_priv);
	file->driver_priv = NULL;
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void exynos_drm_postclose(struct drm_device *dev, struct drm_file *file)
{
<<<<<<< HEAD
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	if (!file->driver_priv)
		return;

=======
	g2d_close(dev, file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(file->driver_priv);
	file->driver_priv = NULL;
}

<<<<<<< HEAD
static void exynos_drm_lastclose(struct drm_device *dev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	exynos_drm_fbdev_restore_mode(dev);
}

static struct vm_operations_struct exynos_drm_gem_vm_ops = {
	.fault = exynos_drm_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static struct drm_ioctl_desc exynos_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_CREATE, exynos_drm_gem_create_ioctl,
			DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_MAP_OFFSET,
			exynos_drm_gem_map_offset_ioctl, DRM_UNLOCKED |
			DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_MMAP,
			exynos_drm_gem_mmap_ioctl, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_PLANE_SET_ZPOS, exynos_plane_set_zpos_ioctl,
			DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_VIDI_CONNECTION,
			vidi_connection_ioctl, DRM_UNLOCKED | DRM_AUTH),
};

static const struct file_operations exynos_drm_driver_fops = {
	.owner		= THIS_MODULE,
	.open		= drm_open,
	.mmap		= exynos_drm_gem_mmap,
	.poll		= drm_poll,
	.read		= drm_read,
	.unlocked_ioctl	= drm_ioctl,
	.release	= drm_release,
};

static struct drm_driver exynos_drm_driver = {
	.driver_features	= DRIVER_HAVE_IRQ | DRIVER_BUS_PLATFORM |
				  DRIVER_MODESET | DRIVER_GEM,
	.load			= exynos_drm_load,
	.unload			= exynos_drm_unload,
	.open			= exynos_drm_open,
	.preclose		= exynos_drm_preclose,
	.lastclose		= exynos_drm_lastclose,
	.postclose		= exynos_drm_postclose,
	.get_vblank_counter	= drm_vblank_count,
	.enable_vblank		= exynos_drm_crtc_enable_vblank,
	.disable_vblank		= exynos_drm_crtc_disable_vblank,
	.gem_init_object	= exynos_drm_gem_init_object,
	.gem_free_object	= exynos_drm_gem_free_object,
	.gem_vm_ops		= &exynos_drm_gem_vm_ops,
	.dumb_create		= exynos_drm_gem_dumb_create,
	.dumb_map_offset	= exynos_drm_gem_dumb_map_offset,
	.dumb_destroy		= exynos_drm_gem_dumb_destroy,
	.ioctls			= exynos_ioctls,
=======
static const struct drm_ioctl_desc exynos_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_CREATE, exynos_drm_gem_create_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_MAP, exynos_drm_gem_map_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_GET, exynos_drm_gem_get_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_VIDI_CONNECTION, vidi_connection_ioctl,
			DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_GET_VER, exynos_g2d_get_ver_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_SET_CMDLIST, exynos_g2d_set_cmdlist_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_EXEC, exynos_g2d_exec_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_GET_RESOURCES,
			exynos_drm_ipp_get_res_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_GET_CAPS, exynos_drm_ipp_get_caps_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_GET_LIMITS,
			exynos_drm_ipp_get_limits_ioctl,
			DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_COMMIT, exynos_drm_ipp_commit_ioctl,
			DRM_RENDER_ALLOW),
};

DEFINE_DRM_GEM_FOPS(exynos_drm_driver_fops);

static const struct drm_driver exynos_drm_driver = {
	.driver_features	= DRIVER_MODESET | DRIVER_GEM
				  | DRIVER_ATOMIC | DRIVER_RENDER,
	.open			= exynos_drm_open,
	.postclose		= exynos_drm_postclose,
	.dumb_create		= exynos_drm_gem_dumb_create,
	.gem_prime_import	= exynos_drm_gem_prime_import,
	.gem_prime_import_sg_table	= exynos_drm_gem_prime_import_sg_table,
	.ioctls			= exynos_ioctls,
	.num_ioctls		= ARRAY_SIZE(exynos_ioctls),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.fops			= &exynos_drm_driver_fops,
	.name	= DRIVER_NAME,
	.desc	= DRIVER_DESC,
	.date	= DRIVER_DATE,
	.major	= DRIVER_MAJOR,
	.minor	= DRIVER_MINOR,
};

<<<<<<< HEAD
static int exynos_drm_platform_probe(struct platform_device *pdev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	exynos_drm_driver.num_ioctls = DRM_ARRAY_SIZE(exynos_ioctls);

	return drm_platform_init(&exynos_drm_driver, pdev);
}

static int exynos_drm_platform_remove(struct platform_device *pdev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	drm_platform_exit(&exynos_drm_driver, pdev);

	return 0;
}

static struct platform_driver exynos_drm_platform_driver = {
	.probe		= exynos_drm_platform_probe,
	.remove		= __devexit_p(exynos_drm_platform_remove),
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "exynos-drm",
	},
};

static int __init exynos_drm_init(void)
{
	int ret;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);

#ifdef CONFIG_DRM_EXYNOS_FIMD
	ret = platform_driver_register(&fimd_driver);
	if (ret < 0)
		goto out_fimd;
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
	ret = platform_driver_register(&hdmi_driver);
	if (ret < 0)
		goto out_hdmi;
	ret = platform_driver_register(&mixer_driver);
	if (ret < 0)
		goto out_mixer;
	ret = platform_driver_register(&exynos_drm_common_hdmi_driver);
	if (ret < 0)
		goto out_common_hdmi;
#endif

#ifdef CONFIG_DRM_EXYNOS_VIDI
	ret = platform_driver_register(&vidi_driver);
	if (ret < 0)
		goto out_vidi;
#endif

	ret = platform_driver_register(&exynos_drm_platform_driver);
	if (ret < 0)
		goto out;

	return 0;

out:
#ifdef CONFIG_DRM_EXYNOS_VIDI
out_vidi:
	platform_driver_unregister(&vidi_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
	platform_driver_unregister(&exynos_drm_common_hdmi_driver);
out_common_hdmi:
	platform_driver_unregister(&mixer_driver);
out_mixer:
	platform_driver_unregister(&hdmi_driver);
out_hdmi:
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMD
	platform_driver_unregister(&fimd_driver);
out_fimd:
#endif
	return ret;
}

static void __exit exynos_drm_exit(void)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	platform_driver_unregister(&exynos_drm_platform_driver);

#ifdef CONFIG_DRM_EXYNOS_HDMI
	platform_driver_unregister(&exynos_drm_common_hdmi_driver);
	platform_driver_unregister(&mixer_driver);
	platform_driver_unregister(&hdmi_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_VIDI
	platform_driver_unregister(&vidi_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMD
	platform_driver_unregister(&fimd_driver);
#endif
=======
static int exynos_drm_suspend(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	return  drm_mode_config_helper_suspend(drm_dev);
}

static void exynos_drm_resume(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	drm_mode_config_helper_resume(drm_dev);
}

static const struct dev_pm_ops exynos_drm_pm_ops = {
	.prepare = exynos_drm_suspend,
	.complete = exynos_drm_resume,
};

/* forward declaration */
static struct platform_driver exynos_drm_platform_driver;

struct exynos_drm_driver_info {
	struct platform_driver *driver;
	unsigned int flags;
};

#define DRM_COMPONENT_DRIVER	BIT(0)	/* supports component framework */
#define DRM_VIRTUAL_DEVICE	BIT(1)	/* create virtual platform device */
#define DRM_FIMC_DEVICE		BIT(2)	/* devices shared with V4L2 subsystem */

#define DRV_PTR(drv, cond) (IS_ENABLED(cond) ? &drv : NULL)

/*
 * Connector drivers should not be placed before associated crtc drivers,
 * because connector requires pipe number of its crtc during initialization.
 */
static struct exynos_drm_driver_info exynos_drm_drivers[] = {
	{
		DRV_PTR(fimd_driver, CONFIG_DRM_EXYNOS_FIMD),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(exynos5433_decon_driver, CONFIG_DRM_EXYNOS5433_DECON),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(decon_driver, CONFIG_DRM_EXYNOS7_DECON),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(mixer_driver, CONFIG_DRM_EXYNOS_MIXER),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(dp_driver, CONFIG_DRM_EXYNOS_DP),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(dsi_driver, CONFIG_DRM_EXYNOS_DSI),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(mic_driver, CONFIG_DRM_EXYNOS_MIC),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(hdmi_driver, CONFIG_DRM_EXYNOS_HDMI),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(vidi_driver, CONFIG_DRM_EXYNOS_VIDI),
		DRM_COMPONENT_DRIVER | DRM_VIRTUAL_DEVICE
	}, {
		DRV_PTR(g2d_driver, CONFIG_DRM_EXYNOS_G2D),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(fimc_driver, CONFIG_DRM_EXYNOS_FIMC),
		DRM_COMPONENT_DRIVER | DRM_FIMC_DEVICE,
	}, {
		DRV_PTR(rotator_driver, CONFIG_DRM_EXYNOS_ROTATOR),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(scaler_driver, CONFIG_DRM_EXYNOS_SCALER),
		DRM_COMPONENT_DRIVER
	}, {
		DRV_PTR(gsc_driver, CONFIG_DRM_EXYNOS_GSC),
		DRM_COMPONENT_DRIVER
	}, {
		&exynos_drm_platform_driver,
		DRM_VIRTUAL_DEVICE
	}
};

static struct component_match *exynos_drm_match_add(struct device *dev)
{
	struct component_match *match = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(exynos_drm_drivers); ++i) {
		struct exynos_drm_driver_info *info = &exynos_drm_drivers[i];
		struct device *p = NULL, *d;

		if (!info->driver || !(info->flags & DRM_COMPONENT_DRIVER))
			continue;

		while ((d = platform_find_device_by_driver(p, &info->driver->driver))) {
			put_device(p);

			if (!(info->flags & DRM_FIMC_DEVICE) ||
			    exynos_drm_check_fimc_device(d) == 0)
				component_match_add(dev, &match, component_compare_dev, d);
			p = d;
		}
		put_device(p);
	}

	return match ?: ERR_PTR(-ENODEV);
}

static int exynos_drm_bind(struct device *dev)
{
	struct exynos_drm_private *private;
	struct drm_encoder *encoder;
	struct drm_device *drm;
	unsigned int clone_mask;
	int ret;

	drm = drm_dev_alloc(&exynos_drm_driver, dev);
	if (IS_ERR(drm))
		return PTR_ERR(drm);

	private = kzalloc(sizeof(struct exynos_drm_private), GFP_KERNEL);
	if (!private) {
		ret = -ENOMEM;
		goto err_free_drm;
	}

	init_waitqueue_head(&private->wait);
	spin_lock_init(&private->lock);

	dev_set_drvdata(dev, drm);
	drm->dev_private = (void *)private;

	drm_mode_config_init(drm);

	exynos_drm_mode_config_init(drm);

	/* setup possible_clones. */
	clone_mask = 0;
	list_for_each_entry(encoder, &drm->mode_config.encoder_list, head)
		clone_mask |= drm_encoder_mask(encoder);

	list_for_each_entry(encoder, &drm->mode_config.encoder_list, head)
		encoder->possible_clones = clone_mask;

	/* Try to bind all sub drivers. */
	ret = component_bind_all(drm->dev, drm);
	if (ret)
		goto err_mode_config_cleanup;

	ret = drm_vblank_init(drm, drm->mode_config.num_crtc);
	if (ret)
		goto err_unbind_all;

	drm_mode_config_reset(drm);

	/* init kms poll for handling hpd */
	drm_kms_helper_poll_init(drm);

	/* register the DRM device */
	ret = drm_dev_register(drm, 0);
	if (ret < 0)
		goto err_cleanup_poll;

	exynos_drm_fbdev_setup(drm);

	return 0;

err_cleanup_poll:
	drm_kms_helper_poll_fini(drm);
err_unbind_all:
	component_unbind_all(drm->dev, drm);
err_mode_config_cleanup:
	drm_mode_config_cleanup(drm);
	exynos_drm_cleanup_dma(drm);
	kfree(private);
	dev_set_drvdata(dev, NULL);
err_free_drm:
	drm_dev_put(drm);

	return ret;
}

static void exynos_drm_unbind(struct device *dev)
{
	struct drm_device *drm = dev_get_drvdata(dev);

	drm_dev_unregister(drm);

	drm_kms_helper_poll_fini(drm);
	drm_atomic_helper_shutdown(drm);

	component_unbind_all(drm->dev, drm);
	drm_mode_config_cleanup(drm);
	exynos_drm_cleanup_dma(drm);

	kfree(drm->dev_private);
	drm->dev_private = NULL;
	dev_set_drvdata(dev, NULL);

	drm_dev_put(drm);
}

static const struct component_master_ops exynos_drm_ops = {
	.bind		= exynos_drm_bind,
	.unbind		= exynos_drm_unbind,
};

static int exynos_drm_platform_probe(struct platform_device *pdev)
{
	struct component_match *match;

	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	match = exynos_drm_match_add(&pdev->dev);
	if (IS_ERR(match))
		return PTR_ERR(match);

	return component_master_add_with_match(&pdev->dev, &exynos_drm_ops,
					       match);
}

static void exynos_drm_platform_remove(struct platform_device *pdev)
{
	component_master_del(&pdev->dev, &exynos_drm_ops);
}

static void exynos_drm_platform_shutdown(struct platform_device *pdev)
{
	struct drm_device *drm = platform_get_drvdata(pdev);

	if (drm)
		drm_atomic_helper_shutdown(drm);
}

static struct platform_driver exynos_drm_platform_driver = {
	.probe	= exynos_drm_platform_probe,
	.remove_new	= exynos_drm_platform_remove,
	.shutdown = exynos_drm_platform_shutdown,
	.driver	= {
		.name	= "exynos-drm",
		.pm	= &exynos_drm_pm_ops,
	},
};

static void exynos_drm_unregister_devices(void)
{
	int i;

	for (i = ARRAY_SIZE(exynos_drm_drivers) - 1; i >= 0; --i) {
		struct exynos_drm_driver_info *info = &exynos_drm_drivers[i];
		struct device *dev;

		if (!info->driver || !(info->flags & DRM_VIRTUAL_DEVICE))
			continue;

		while ((dev = platform_find_device_by_driver(NULL,
						&info->driver->driver))) {
			put_device(dev);
			platform_device_unregister(to_platform_device(dev));
		}
	}
}

static int exynos_drm_register_devices(void)
{
	struct platform_device *pdev;
	int i;

	for (i = 0; i < ARRAY_SIZE(exynos_drm_drivers); ++i) {
		struct exynos_drm_driver_info *info = &exynos_drm_drivers[i];

		if (!info->driver || !(info->flags & DRM_VIRTUAL_DEVICE))
			continue;

		pdev = platform_device_register_simple(
					info->driver->driver.name, -1, NULL, 0);
		if (IS_ERR(pdev))
			goto fail;
	}

	return 0;
fail:
	exynos_drm_unregister_devices();
	return PTR_ERR(pdev);
}

static void exynos_drm_unregister_drivers(void)
{
	int i;

	for (i = ARRAY_SIZE(exynos_drm_drivers) - 1; i >= 0; --i) {
		struct exynos_drm_driver_info *info = &exynos_drm_drivers[i];

		if (!info->driver)
			continue;

		platform_driver_unregister(info->driver);
	}
}

static int exynos_drm_register_drivers(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(exynos_drm_drivers); ++i) {
		struct exynos_drm_driver_info *info = &exynos_drm_drivers[i];

		if (!info->driver)
			continue;

		ret = platform_driver_register(info->driver);
		if (ret)
			goto fail;
	}
	return 0;
fail:
	exynos_drm_unregister_drivers();
	return ret;
}

static int exynos_drm_init(void)
{
	int ret;

	if (drm_firmware_drivers_only())
		return -ENODEV;

	ret = exynos_drm_register_devices();
	if (ret)
		return ret;

	ret = exynos_drm_register_drivers();
	if (ret)
		goto err_unregister_pdevs;

	return 0;

err_unregister_pdevs:
	exynos_drm_unregister_devices();

	return ret;
}

static void exynos_drm_exit(void)
{
	exynos_drm_unregister_drivers();
	exynos_drm_unregister_devices();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(exynos_drm_init);
module_exit(exynos_drm_exit);

MODULE_AUTHOR("Inki Dae <inki.dae@samsung.com>");
MODULE_AUTHOR("Joonyoung Shim <jy0922.shim@samsung.com>");
MODULE_AUTHOR("Seung-Woo Kim <sw0312.kim@samsung.com>");
MODULE_DESCRIPTION("Samsung SoC DRM Driver");
MODULE_LICENSE("GPL");
