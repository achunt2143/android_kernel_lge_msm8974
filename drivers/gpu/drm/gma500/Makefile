<<<<<<< HEAD
#
#	KMS driver for the GMA500
#
ccflags-y += -Iinclude/drm

gma500_gfx-y += gem_glue.o \
	  accel_2d.o \
	  backlight.o \
	  framebuffer.o \
	  gem.o \
	  gtt.o \
	  intel_bios.o \
	  intel_i2c.o \
	  intel_gmbus.o \
	  intel_opregion.o \
	  mmu.o \
	  power.o \
=======
# SPDX-License-Identifier: GPL-2.0
#
#	KMS driver for the GMA500
#

gma500_gfx-y += \
	  backlight.o \
	  cdv_device.o \
	  cdv_intel_crt.o \
	  cdv_intel_display.o \
	  cdv_intel_dp.o \
	  cdv_intel_hdmi.o \
	  cdv_intel_lvds.o \
	  framebuffer.o \
	  gem.o \
	  gma_device.o \
	  gma_display.o \
	  gtt.o \
	  intel_bios.o \
	  intel_gmbus.o \
	  intel_i2c.o \
	  mid_bios.o \
	  mmu.o \
	  oaktrail_device.o \
	  oaktrail_crtc.o \
	  oaktrail_hdmi.o \
	  oaktrail_hdmi_i2c.o \
	  oaktrail_lvds.o \
	  oaktrail_lvds_i2c.o \
	  power.o \
	  psb_device.o \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  psb_drv.o \
	  psb_intel_display.o \
	  psb_intel_lvds.o \
	  psb_intel_modes.o \
	  psb_intel_sdvo.o \
<<<<<<< HEAD
	  psb_lid.o \
	  psb_irq.o \
	  psb_device.o \
	  mid_bios.o

gma500_gfx-$(CONFIG_DRM_GMA3600) +=  cdv_device.o \
	  cdv_intel_crt.o \
	  cdv_intel_display.o \
	  cdv_intel_hdmi.o \
	  cdv_intel_lvds.o

gma500_gfx-$(CONFIG_DRM_GMA600) += oaktrail_device.o \
	  oaktrail_crtc.o \
	  oaktrail_lvds.o \
	  oaktrail_hdmi.o \
	  oaktrail_hdmi_i2c.o

gma500_gfx-$(CONFIG_DRM_MEDFIELD) += mdfld_device.o \
	  mdfld_output.o \
	  mdfld_intel_display.o \
	  mdfld_dsi_output.o \
	  mdfld_dsi_dpi.o \
	  mdfld_dsi_pkg_sender.o \
	  mdfld_tpo_vid.o \
	  mdfld_tmd_vid.o \
	  tc35876x-dsi-lvds.o
=======
	  psb_irq.o

gma500_gfx-$(CONFIG_ACPI) +=  opregion.o
gma500_gfx-$(CONFIG_DRM_FBDEV_EMULATION) +=  fbdev.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

obj-$(CONFIG_DRM_GMA500) += gma500_gfx.o
