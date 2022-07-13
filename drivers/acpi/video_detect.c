/*
<<<<<<< HEAD
=======
 *  Copyright (C) 2015       Red Hat Inc.
 *                           Hans de Goede <hdegoede@redhat.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Copyright (C) 2008       SuSE Linux Products GmbH
 *                           Thomas Renninger <trenn@suse.de>
 *
 *  May be copied or modified under the terms of the GNU General Public License
 *
 * video_detect.c:
<<<<<<< HEAD
 * Provides acpi_is_video_device() for early scanning of ACPI devices in scan.c
 * There a Linux specific (Spec does not provide a HID for video devices) is
 * assigned
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * After PCI devices are glued with ACPI devices
 * acpi_get_pci_dev() can be called to identify ACPI graphics
 * devices for which a real graphics card is plugged in
 *
<<<<<<< HEAD
 * Now acpi_video_get_capabilities() can be called to check which
 * capabilities the graphics cards plugged in support. The check for general
 * video capabilities will be triggered by the first caller of
 * acpi_video_get_capabilities(NULL); which will happen when the first
 * backlight switching supporting driver calls:
 * acpi_video_backlight_support();
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Depending on whether ACPI graphics extensions (cmp. ACPI spec Appendix B)
 * are available, video.ko should be used to handle the device.
 *
 * Otherwise vendor specific drivers like thinkpad_acpi, asus-laptop,
 * sony_acpi,... can take care about backlight brightness.
 *
<<<<<<< HEAD
 * If CONFIG_ACPI_VIDEO is neither set as "compiled in" (y) nor as a module (m)
 * this file will not be compiled, acpi_video_get_capabilities() and
 * acpi_video_backlight_support() will always return 0 and vendor specific
 * drivers always can handle backlight.
 *
=======
 * Backlight drivers can use acpi_video_get_backlight_type() to determine which
 * driver should handle the backlight. RAW/GPU-driver backlight drivers must
 * use the acpi_video_backlight_use_native() helper for this.
 *
 * If CONFIG_ACPI_VIDEO is neither set as "compiled in" (y) nor as a module (m)
 * this file will not be compiled and acpi_video_get_backlight_type() will
 * always return acpi_backlight_vendor.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/export.h>
#include <linux/acpi.h>
<<<<<<< HEAD
#include <linux/dmi.h>
#include <linux/pci.h>

#define PREFIX "ACPI: "

ACPI_MODULE_NAME("video");
#define _COMPONENT		ACPI_VIDEO_COMPONENT

static long acpi_video_support;
static bool acpi_video_caps_checked;

static acpi_status
acpi_backlight_cap_match(acpi_handle handle, u32 level, void *context,
			  void **retyurn_value)
{
	long *cap = context;
	acpi_handle h_dummy;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_BCM", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(handle, "_BCL", &h_dummy))) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Found generic backlight "
				  "support\n"));
		*cap |= ACPI_VIDEO_BACKLIGHT;
		if (ACPI_FAILURE(acpi_get_handle(handle, "_BQC", &h_dummy)))
			printk(KERN_WARNING FW_BUG PREFIX "No _BQC method, "
				"cannot determine initial brightness\n");
		/* We have backlight support, no need to scan further */
		return AE_CTRL_TERMINATE;
	}
	return 0;
}

/* Returns true if the device is a video device which can be handled by
 * video.ko.
 * The device will get a Linux specific CID added in scan.c to
 * identify the device as an ACPI graphics device
 * Be aware that the graphics device may not be physically present
 * Use acpi_video_get_capabilities() to detect general ACPI video
 * capabilities of present cards
 */
long acpi_is_video_device(struct acpi_device *device)
{
	acpi_handle h_dummy;
	long video_caps = 0;

	if (!device)
		return 0;

	/* Is this device able to support video switching ? */
	if (ACPI_SUCCESS(acpi_get_handle(device->handle, "_DOD", &h_dummy)) ||
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "_DOS", &h_dummy)))
		video_caps |= ACPI_VIDEO_OUTPUT_SWITCHING;

	/* Is this device able to retrieve a video ROM ? */
	if (ACPI_SUCCESS(acpi_get_handle(device->handle, "_ROM", &h_dummy)))
		video_caps |= ACPI_VIDEO_ROM_AVAILABLE;

	/* Is this device able to configure which video head to be POSTed ? */
	if (ACPI_SUCCESS(acpi_get_handle(device->handle, "_VPO", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "_GPD", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "_SPD", &h_dummy)))
		video_caps |= ACPI_VIDEO_DEVICE_POSTING;

	/* Only check for backlight functionality if one of the above hit. */
	if (video_caps)
		acpi_walk_namespace(ACPI_TYPE_DEVICE, device->handle,
				    ACPI_UINT32_MAX, acpi_backlight_cap_match, NULL,
				    &video_caps, NULL);

	return video_caps;
}
EXPORT_SYMBOL(acpi_is_video_device);

static acpi_status
find_video(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	long *cap = context;
	struct pci_dev *dev;
	struct acpi_device *acpi_dev;

	const struct acpi_device_id video_ids[] = {
		{ACPI_VIDEO_HID, 0},
		{"", 0},
	};
	if (acpi_bus_get_device(handle, &acpi_dev))
		return AE_OK;

	if (!acpi_match_device_ids(acpi_dev, video_ids)) {
=======
#include <linux/apple-gmux.h>
#include <linux/backlight.h>
#include <linux/dmi.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/platform_data/x86/nvidia-wmi-ec-backlight.h>
#include <linux/pnp.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <acpi/video.h>

static enum acpi_backlight_type acpi_backlight_cmdline = acpi_backlight_undef;
static enum acpi_backlight_type acpi_backlight_dmi = acpi_backlight_undef;

static void acpi_video_parse_cmdline(void)
{
	if (!strcmp("vendor", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_vendor;
	if (!strcmp("video", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_video;
	if (!strcmp("native", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_native;
	if (!strcmp("nvidia_wmi_ec", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_nvidia_wmi_ec;
	if (!strcmp("apple_gmux", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_apple_gmux;
	if (!strcmp("none", acpi_video_backlight_string))
		acpi_backlight_cmdline = acpi_backlight_none;
}

static acpi_status
find_video(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpi_device *acpi_dev = acpi_fetch_acpi_dev(handle);
	long *cap = context;
	struct pci_dev *dev;

	static const struct acpi_device_id video_ids[] = {
		{ACPI_VIDEO_HID, 0},
		{"", 0},
	};

	if (acpi_dev && !acpi_match_device_ids(acpi_dev, video_ids)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev = acpi_get_pci_dev(handle);
		if (!dev)
			return AE_OK;
		pci_dev_put(dev);
<<<<<<< HEAD
		*cap |= acpi_is_video_device(acpi_dev);
=======
		*cap |= acpi_is_video_device(handle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return AE_OK;
}

<<<<<<< HEAD
=======
/* This depends on ACPI_WMI which is X86 only */
#ifdef CONFIG_X86
static bool nvidia_wmi_ec_supported(void)
{
	struct wmi_brightness_args args = {
		.mode = WMI_BRIGHTNESS_MODE_GET,
		.val = 0,
		.ret = 0,
	};
	struct acpi_buffer buf = { (acpi_size)sizeof(args), &args };
	acpi_status status;

	status = wmi_evaluate_method(WMI_BRIGHTNESS_GUID, 0,
				     WMI_BRIGHTNESS_METHOD_SOURCE, &buf, &buf);
	if (ACPI_FAILURE(status))
		return false;

	/*
	 * If brightness is handled by the EC then nvidia-wmi-ec-backlight
	 * should be used, else the GPU driver(s) should be used.
	 */
	return args.ret == WMI_BRIGHTNESS_SOURCE_EC;
}
#else
static bool nvidia_wmi_ec_supported(void)
{
	return false;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Force to use vendor driver when the ACPI device is known to be
 * buggy */
static int video_detect_force_vendor(const struct dmi_system_id *d)
{
<<<<<<< HEAD
	acpi_video_support |= ACPI_VIDEO_BACKLIGHT_DMI_VENDOR;
	return 0;
}

static struct dmi_system_id video_detect_dmi_table[] = {
	/* On Samsung X360, the BIOS will set a flag (VDRV) if generic
	 * ACPI backlight device is used. This flag will definitively break
	 * the backlight interface (even the vendor interface) untill next
	 * reboot. It's why we should prevent video.ko from being used here
	 * and we can't rely on a later call to acpi_video_unregister().
	 */
	{
	 .callback = video_detect_force_vendor,
	 .ident = "X360",
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "X360"),
		DMI_MATCH(DMI_BOARD_NAME, "X360"),
		},
	},
	{
	.callback = video_detect_force_vendor,
	.ident = "Asus UL30VT",
	.matches = {
=======
	acpi_backlight_dmi = acpi_backlight_vendor;
	return 0;
}

static int video_detect_force_video(const struct dmi_system_id *d)
{
	acpi_backlight_dmi = acpi_backlight_video;
	return 0;
}

static int video_detect_force_native(const struct dmi_system_id *d)
{
	acpi_backlight_dmi = acpi_backlight_native;
	return 0;
}

static int video_detect_portege_r100(const struct dmi_system_id *d)
{
	struct pci_dev *dev;
	/* Search for Trident CyberBlade XP4m32 to confirm Portégé R100 */
	dev = pci_get_device(PCI_VENDOR_ID_TRIDENT, 0x2100, NULL);
	if (dev)
		acpi_backlight_dmi = acpi_backlight_vendor;
	return 0;
}

static const struct dmi_system_id video_detect_dmi_table[] = {
	/*
	 * Models which should use the vendor backlight interface,
	 * because of broken ACPI video backlight control.
	 */
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1128309 */
	 .callback = video_detect_force_vendor,
	 /* Acer KAV80 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "KAV80"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Asus UL30VT */
	 .matches = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK Computer Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "UL30VT"),
		},
	},
	{
<<<<<<< HEAD
	.callback = video_detect_force_vendor,
	.ident = "Asus UL30A",
	.matches = {
=======
	 .callback = video_detect_force_vendor,
	 /* Asus UL30A */
	 .matches = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK Computer Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "UL30A"),
		},
	},
<<<<<<< HEAD
	{ },
};

/*
 * Returns the video capabilities of a specific ACPI graphics device
 *
 * if NULL is passed as argument all ACPI devices are enumerated and
 * all graphics capabilities of physically present devices are
 * summarized and returned. This is cached and done only once.
 */
long acpi_video_get_capabilities(acpi_handle graphics_handle)
{
	long caps = 0;
	struct acpi_device *tmp_dev;
	acpi_status status;

	if (acpi_video_caps_checked && graphics_handle == NULL)
		return acpi_video_support;

	if (!graphics_handle) {
		/* Only do the global walk through all graphics devices once */
		acpi_walk_namespace(ACPI_TYPE_DEVICE, ACPI_ROOT_OBJECT,
				    ACPI_UINT32_MAX, find_video, NULL,
				    &caps, NULL);
		/* There might be boot param flags set already... */
		acpi_video_support |= caps;
		acpi_video_caps_checked = 1;
		/* Add blacklists here. Be careful to use the right *DMI* bits
		 * to still be able to override logic via boot params, e.g.:
		 *
		 *   if (dmi_name_in_vendors("XY")) {
		 *	acpi_video_support |=
		 *		ACPI_VIDEO_BACKLIGHT_DMI_VENDOR;
		 *}
		 */

		dmi_check_system(video_detect_dmi_table);
	} else {
		status = acpi_bus_get_device(graphics_handle, &tmp_dev);
		if (ACPI_FAILURE(status)) {
			ACPI_EXCEPTION((AE_INFO, status, "Invalid device"));
			return 0;
		}
		acpi_walk_namespace(ACPI_TYPE_DEVICE, graphics_handle,
				    ACPI_UINT32_MAX, find_video, NULL,
				    &caps, NULL);
	}
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "We have 0x%lX video support %s %s\n",
			  graphics_handle ? caps : acpi_video_support,
			  graphics_handle ? "on device " : "in general",
			  graphics_handle ? acpi_device_bid(tmp_dev) : ""));
	return caps;
}
EXPORT_SYMBOL(acpi_video_get_capabilities);

/* Returns true if video.ko can do backlight switching */
int acpi_video_backlight_support(void)
{
	/*
	 * We must check whether the ACPI graphics device is physically plugged
	 * in. Therefore this must be called after binding PCI and ACPI devices
	 */
	if (!acpi_video_caps_checked)
		acpi_video_get_capabilities(NULL);

	/* First check for boot param -> highest prio */
	if (acpi_video_support & ACPI_VIDEO_BACKLIGHT_FORCE_VENDOR)
		return 0;
	else if (acpi_video_support & ACPI_VIDEO_BACKLIGHT_FORCE_VIDEO)
		return 1;

	/* Then check for DMI blacklist -> second highest prio */
	if (acpi_video_support & ACPI_VIDEO_BACKLIGHT_DMI_VENDOR)
		return 0;
	else if (acpi_video_support & ACPI_VIDEO_BACKLIGHT_DMI_VIDEO)
		return 1;

	/* Then go the default way */
	return acpi_video_support & ACPI_VIDEO_BACKLIGHT;
}
EXPORT_SYMBOL(acpi_video_backlight_support);

/*
 * Use acpi_backlight=vendor/video to force that backlight switching
 * is processed by vendor specific acpi drivers or video.ko driver.
 */
static int __init acpi_backlight(char *str)
{
	if (str == NULL || *str == '\0')
		return 1;
	else {
		if (!strcmp("vendor", str))
			acpi_video_support |=
				ACPI_VIDEO_BACKLIGHT_FORCE_VENDOR;
		if (!strcmp("video", str))
			acpi_video_support |=
				ACPI_VIDEO_BACKLIGHT_FORCE_VIDEO;
	}
	return 1;
}
__setup("acpi_backlight=", acpi_backlight);
=======
	{
	 .callback = video_detect_force_vendor,
	 /* Asus X55U */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "X55U"),
		},
	},
	{
	 /* https://bugs.launchpad.net/bugs/1000146 */
	 .callback = video_detect_force_vendor,
	 /* Asus X101CH */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "X101CH"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Asus X401U */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "X401U"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Asus X501U */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "X501U"),
		},
	},
	{
	 /* https://bugs.launchpad.net/bugs/1000146 */
	 .callback = video_detect_force_vendor,
	 /* Asus 1015CX */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "1015CX"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Samsung N150/N210/N220 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "N150/N210/N220"),
		DMI_MATCH(DMI_BOARD_NAME, "N150/N210/N220"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Samsung NF110/NF210/NF310 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "NF110/NF210/NF310"),
		DMI_MATCH(DMI_BOARD_NAME, "NF110/NF210/NF310"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Samsung NC210 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "NC210/NC110"),
		DMI_MATCH(DMI_BOARD_NAME, "NC210/NC110"),
		},
	},

	/*
	 * Models which should use the vendor backlight interface,
	 * because of broken native backlight control.
	 */
	{
	 .callback = video_detect_force_vendor,
	 /* Sony Vaio PCG-FRV35 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "PCG-FRV35"),
		},
	},

	/*
	 * Toshiba models with Transflective display, these need to use
	 * the toshiba_acpi vendor driver for proper Transflective handling.
	 */
	{
	 .callback = video_detect_force_vendor,
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "PORTEGE R500"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "PORTEGE R600"),
		},
	},

	/*
	 * Toshiba Portégé R100 has working both acpi_video and toshiba_acpi
	 * vendor driver. But none of them gets activated as it has a VGA with
	 * no kernel driver (Trident CyberBlade XP4m32).
	 * The DMI strings are generic so check for the VGA chip in callback.
	 */
	{
	 .callback = video_detect_portege_r100,
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Portable PC"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "Version 1.0"),
		DMI_MATCH(DMI_BOARD_NAME, "Portable PC")
		},
	},

	/*
	 * Models which need acpi_video backlight control where the GPU drivers
	 * do not call acpi_video_register_backlight() because no internal panel
	 * is detected. Typically these are all-in-ones (monitors with builtin
	 * PC) where the panel connection shows up as regular DP instead of eDP.
	 */
	{
	 .callback = video_detect_force_video,
	 /* Apple iMac14,1 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "iMac14,1"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* Apple iMac14,2 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "iMac14,2"),
		},
	},

	/*
	 * These models have a working acpi_video backlight control, and using
	 * native backlight causes a regression where backlight does not work
	 * when userspace is not handling brightness key events. Disable
	 * native_backlight on these to fix this:
	 * https://bugzilla.kernel.org/show_bug.cgi?id=81691
	 */
	{
	 .callback = video_detect_force_video,
	 /* ThinkPad T420 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "ThinkPad T420"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* ThinkPad T520 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "ThinkPad T520"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* ThinkPad X201s */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "ThinkPad X201s"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* ThinkPad X201T */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "ThinkPad X201T"),
		},
	},

	/* The native backlight controls do not work on some older machines */
	{
	 /* https://bugs.freedesktop.org/show_bug.cgi?id=81515 */
	 .callback = video_detect_force_video,
	 /* HP ENVY 15 Notebook */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
		DMI_MATCH(DMI_PRODUCT_NAME, "HP ENVY 15 Notebook PC"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* SAMSUNG 870Z5E/880Z5E/680Z5E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "870Z5E/880Z5E/680Z5E"),
		},
	},
	{
	 .callback = video_detect_force_video,
	 /* SAMSUNG 370R4E/370R4V/370R5E/3570RE/370R5V */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME,
			  "370R4E/370R4V/370R5E/3570RE/370R5V"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1186097 */
	 .callback = video_detect_force_video,
	 /* SAMSUNG 3570R/370R/470R/450R/510R/4450RV */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME,
			  "3570R/370R/470R/450R/510R/4450RV"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1557060 */
	 .callback = video_detect_force_video,
	 /* SAMSUNG 670Z5E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "670Z5E"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1094948 */
	 .callback = video_detect_force_video,
	 /* SAMSUNG 730U3E/740U3E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "730U3E/740U3E"),
		},
	},
	{
	 /* https://bugs.freedesktop.org/show_bug.cgi?id=87286 */
	 .callback = video_detect_force_video,
	 /* SAMSUNG 900X3C/900X3D/900X3E/900X4C/900X4D */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME,
			  "900X3C/900X3D/900X3E/900X4C/900X4D"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1272633 */
	 .callback = video_detect_force_video,
	 /* Dell XPS14 L421X */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "XPS L421X"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1163574 */
	 .callback = video_detect_force_video,
	 /* Dell XPS15 L521X */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "XPS L521X"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=108971 */
	 .callback = video_detect_force_video,
	 /* SAMSUNG 530U4E/540U4E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "530U4E/540U4E"),
		},
	},
	{
	 /* https://bugs.launchpad.net/bugs/1894667 */
	 .callback = video_detect_force_video,
	 /* HP 635 Notebook */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
		DMI_MATCH(DMI_PRODUCT_NAME, "HP 635 Notebook PC"),
		},
	},

	/* Non win8 machines which need native backlight nevertheless */
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1201530 */
	 .callback = video_detect_force_native,
	 /* Lenovo Ideapad S405 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_BOARD_NAME, "Lenovo IdeaPad S405"),
		},
	},
	{
	 /* https://bugzilla.suse.com/show_bug.cgi?id=1208724 */
	 .callback = video_detect_force_native,
	 /* Lenovo Ideapad Z470 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "IdeaPad Z470"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1187004 */
	 .callback = video_detect_force_native,
	 /* Lenovo Ideapad Z570 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "Ideapad Z570"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Lenovo E41-25 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "81FS"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Lenovo E41-45 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "82BK"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Lenovo ThinkPad X131e (3371 AMD version) */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "3371"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Apple iMac11,3 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "iMac11,3"),
		},
	},
	{
	 /* https://gitlab.freedesktop.org/drm/amd/-/issues/1838 */
	 .callback = video_detect_force_native,
	 /* Apple iMac12,1 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "iMac12,1"),
		},
	},
	{
	 /* https://gitlab.freedesktop.org/drm/amd/-/issues/2753 */
	 .callback = video_detect_force_native,
	 /* Apple iMac12,2 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "iMac12,2"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1217249 */
	 .callback = video_detect_force_native,
	 /* Apple MacBook Pro 12,1 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Apple Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "MacBookPro12,1"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Dell Inspiron N4010 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron N4010"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Dell Vostro V131 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Vostro V131"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1123661 */
	 .callback = video_detect_force_native,
	 /* Dell XPS 17 L702X */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Dell System XPS L702X"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Dell Precision 7510 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Precision 7510"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Dell Studio 1569 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Studio 1569"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Acer Aspire 3830TG */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Aspire 3830TG"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Acer Aspire 4810T */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Aspire 4810T"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Acer Aspire 5738z */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Aspire 5738"),
		DMI_MATCH(DMI_BOARD_NAME, "JV50"),
		},
	},
	{
	 /* https://bugzilla.redhat.com/show_bug.cgi?id=1012674 */
	 .callback = video_detect_force_native,
	 /* Acer Aspire 5741 */
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Aspire 5741"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=42993 */
	 .callback = video_detect_force_native,
	 /* Acer Aspire 5750 */
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Aspire 5750"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=42833 */
	 .callback = video_detect_force_native,
	 /* Acer Extensa 5235 */
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Extensa 5235"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Acer TravelMate 4750 */
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "TravelMate 4750"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=207835 */
	 .callback = video_detect_force_native,
	 /* Acer TravelMate 5735Z */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "TravelMate 5735Z"),
		DMI_MATCH(DMI_BOARD_NAME, "BA51_MV"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=36322 */
	 .callback = video_detect_force_native,
	 /* Acer TravelMate 5760 */
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Acer"),
		DMI_MATCH(DMI_PRODUCT_NAME, "TravelMate 5760"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* ASUSTeK COMPUTER INC. GA401 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "GA401"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* ASUSTeK COMPUTER INC. GA502 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "GA502"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* ASUSTeK COMPUTER INC. GA503 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "GA503"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Asus U46E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK Computer Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "U46E"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Asus UX303UB */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "UX303UB"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* HP EliteBook 8460p */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
		DMI_MATCH(DMI_PRODUCT_NAME, "HP EliteBook 8460p"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* HP Pavilion g6-1d80nr / B4U19UA */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
		DMI_MATCH(DMI_PRODUCT_NAME, "HP Pavilion g6 Notebook PC"),
		DMI_MATCH(DMI_PRODUCT_SKU, "B4U19UA"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Samsung N150P */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "N150P"),
		DMI_MATCH(DMI_BOARD_NAME, "N150P"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Samsung N145P/N250P/N260P */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "N145P/N250P/N260P"),
		DMI_MATCH(DMI_BOARD_NAME, "N145P/N250P/N260P"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Samsung N250P */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "SAMSUNG ELECTRONICS CO., LTD."),
		DMI_MATCH(DMI_PRODUCT_NAME, "N250P"),
		DMI_MATCH(DMI_BOARD_NAME, "N250P"),
		},
	},
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=202401 */
	 .callback = video_detect_force_native,
	 /* Sony Vaio VPCEH3U1E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCEH3U1E"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Sony Vaio VPCY11S1E */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCY11S1E"),
		},
	},

	/*
	 * These Toshibas have a broken acpi-video interface for brightness
	 * control. They also have an issue where the panel is off after
	 * suspend until a special firmware call is made to turn it back
	 * on. This is handled by the toshiba_acpi kernel module, so that
	 * module must be enabled for these models to work correctly.
	 */
	{
	 /* https://bugzilla.kernel.org/show_bug.cgi?id=21012 */
	 .callback = video_detect_force_native,
	 /* Toshiba Portégé R700 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "PORTEGE R700"),
		},
	},
	{
	 /* Portégé: https://bugs.freedesktop.org/show_bug.cgi?id=82634 */
	 /* Satellite: https://bugzilla.kernel.org/show_bug.cgi?id=21012 */
	 .callback = video_detect_force_native,
	 /* Toshiba Satellite/Portégé R830 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "R830"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 /* Toshiba Satellite/Portégé Z830 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Z830"),
		},
	},

	/*
	 * Models which have nvidia-ec-wmi support, but should not use it.
	 * Note this indicates a likely firmware bug on these models and should
	 * be revisited if/when Linux gets support for dynamic mux mode.
	 */
	{
	 .callback = video_detect_force_native,
	 /* Dell G15 5515 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Dell G15 5515"),
		},
	},
	{
	 .callback = video_detect_force_native,
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Vostro 15 3535"),
		},
	},

	/*
	 * x86 android tablets which directly control the backlight through
	 * an external backlight controller, typically TI's LP8557.
	 * The backlight is directly controlled by the lp855x driver on these.
	 * This setup means that neither i915's native nor acpi_video backlight
	 * control works. Add a "vendor" quirk to disable both. Note these
	 * devices do not use vendor control in the typical meaning of
	 * vendor specific SMBIOS or ACPI calls being used.
	 */
	{
	 .callback = video_detect_force_vendor,
	 /* Lenovo Yoga Book X90F / X90L */
	 .matches = {
		DMI_EXACT_MATCH(DMI_SYS_VENDOR, "Intel Corporation"),
		DMI_EXACT_MATCH(DMI_PRODUCT_NAME, "CHERRYVIEW D1 PLATFORM"),
		DMI_EXACT_MATCH(DMI_PRODUCT_VERSION, "YETI-11"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /*
	  * Lenovo Yoga Tablet 2 830F/L or 1050F/L (The 8" and 10"
	  * Lenovo Yoga Tablet 2 use the same mainboard)
	  */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Intel Corp."),
		DMI_MATCH(DMI_PRODUCT_NAME, "VALLEYVIEW C0 PLATFORM"),
		DMI_MATCH(DMI_BOARD_NAME, "BYT-T FFD8"),
		/* Partial match on beginning of BIOS version */
		DMI_MATCH(DMI_BIOS_VERSION, "BLADE_21"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Lenovo Yoga Tab 3 Pro YT3-X90F */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Intel Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "CHERRYVIEW D1 PLATFORM"),
		DMI_MATCH(DMI_PRODUCT_VERSION, "Blade3-10A-001"),
		},
	},
	{
	 .callback = video_detect_force_vendor,
	 /* Xiaomi Mi Pad 2 */
	 .matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Xiaomi Inc"),
		DMI_MATCH(DMI_PRODUCT_NAME, "Mipad2"),
		},
	},
	{ },
};

static bool google_cros_ec_present(void)
{
	return acpi_dev_found("GOOG0004") || acpi_dev_found("GOOG000C");
}

/*
 * Windows 8 and newer no longer use the ACPI video interface, so it often
 * does not work. So on win8+ systems prefer native brightness control.
 * Chromebooks should always prefer native backlight control.
 */
static bool prefer_native_over_acpi_video(void)
{
	return acpi_osi_is_win8() || google_cros_ec_present();
}

/*
 * Determine which type of backlight interface to use on this system,
 * First check cmdline, then dmi quirks, then do autodetect.
 */
enum acpi_backlight_type __acpi_video_get_backlight_type(bool native, bool *auto_detect)
{
	static DEFINE_MUTEX(init_mutex);
	static bool nvidia_wmi_ec_present;
	static bool apple_gmux_present;
	static bool native_available;
	static bool init_done;
	static long video_caps;

	/* Parse cmdline, dmi and acpi only once */
	mutex_lock(&init_mutex);
	if (!init_done) {
		acpi_video_parse_cmdline();
		dmi_check_system(video_detect_dmi_table);
		acpi_walk_namespace(ACPI_TYPE_DEVICE, ACPI_ROOT_OBJECT,
				    ACPI_UINT32_MAX, find_video, NULL,
				    &video_caps, NULL);
		nvidia_wmi_ec_present = nvidia_wmi_ec_supported();
		apple_gmux_present = apple_gmux_detect(NULL, NULL);
		init_done = true;
	}
	if (native)
		native_available = true;
	mutex_unlock(&init_mutex);

	if (auto_detect)
		*auto_detect = false;

	/*
	 * The below heuristics / detection steps are in order of descending
	 * presedence. The commandline takes presedence over anything else.
	 */
	if (acpi_backlight_cmdline != acpi_backlight_undef)
		return acpi_backlight_cmdline;

	/* DMI quirks override any autodetection. */
	if (acpi_backlight_dmi != acpi_backlight_undef)
		return acpi_backlight_dmi;

	if (auto_detect)
		*auto_detect = true;

	/* Special cases such as nvidia_wmi_ec and apple gmux. */
	if (nvidia_wmi_ec_present)
		return acpi_backlight_nvidia_wmi_ec;

	if (apple_gmux_present)
		return acpi_backlight_apple_gmux;

	/* Use ACPI video if available, except when native should be preferred. */
	if ((video_caps & ACPI_VIDEO_BACKLIGHT) &&
	     !(native_available && prefer_native_over_acpi_video()))
		return acpi_backlight_video;

	/* Use native if available */
	if (native_available)
		return acpi_backlight_native;

	/*
	 * The vendor specific BIOS interfaces are only necessary for
	 * laptops from before ~2008.
	 *
	 * For laptops from ~2008 till ~2023 this point is never reached
	 * because on those (video_caps & ACPI_VIDEO_BACKLIGHT) above is true.
	 *
	 * Laptops from after ~2023 no longer support ACPI_VIDEO_BACKLIGHT,
	 * if this point is reached on those, this likely means that
	 * the GPU kms driver which sets native_available has not loaded yet.
	 *
	 * Returning acpi_backlight_vendor in this case is known to sometimes
	 * cause a non working vendor specific /sys/class/backlight device to
	 * get registered.
	 *
	 * Return acpi_backlight_none on laptops with ACPI tables written
	 * for Windows 8 (laptops from after ~2012) to avoid this problem.
	 */
	if (acpi_osi_is_win8())
		return acpi_backlight_none;

	/* No ACPI video/native (old hw), use vendor specific fw methods. */
	return acpi_backlight_vendor;
}
EXPORT_SYMBOL(__acpi_video_get_backlight_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
