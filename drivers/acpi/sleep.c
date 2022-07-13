<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * sleep.c - ACPI sleep support.
 *
 * Copyright (c) 2005 Alexey Starikovskiy <alexey.y.starikovskiy@intel.com>
 * Copyright (c) 2004 David Shaohua Li <shaohua.li@intel.com>
 * Copyright (c) 2000-2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
 *
 */

=======
 */

#define pr_fmt(fmt) "ACPI: PM: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/dmi.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/interrupt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/suspend.h>
#include <linux/reboot.h>
#include <linux/acpi.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>

#include <asm/io.h>

#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
=======
#include <linux/syscore_ops.h>
#include <asm/io.h>
#include <trace/events/power.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "internal.h"
#include "sleep.h"

<<<<<<< HEAD
u8 wake_sleep_flags = ACPI_NO_OPTIONAL_METHODS;
static unsigned int gts, bfs;
static int set_param_wake_flag(const char *val, struct kernel_param *kp)
{
	int ret = param_set_int(val, kp);

	if (ret)
		return ret;

	if (kp->arg == (const char *)&gts) {
		if (gts)
			wake_sleep_flags |= ACPI_EXECUTE_GTS;
		else
			wake_sleep_flags &= ~ACPI_EXECUTE_GTS;
	}
	if (kp->arg == (const char *)&bfs) {
		if (bfs)
			wake_sleep_flags |= ACPI_EXECUTE_BFS;
		else
			wake_sleep_flags &= ~ACPI_EXECUTE_BFS;
	}
	return ret;
}
module_param_call(gts, set_param_wake_flag, param_get_int, &gts, 0644);
module_param_call(bfs, set_param_wake_flag, param_get_int, &bfs, 0644);
MODULE_PARM_DESC(gts, "Enable evaluation of _GTS on suspend.");
MODULE_PARM_DESC(bfs, "Enable evaluation of _BFS on resume".);

=======
/*
 * Some HW-full platforms do not have _S5, so they may need
 * to leverage efi power off for a shutdown.
 */
bool acpi_no_s5;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u8 sleep_states[ACPI_S_STATE_COUNT];

static void acpi_sleep_tts_switch(u32 acpi_state)
{
<<<<<<< HEAD
	union acpi_object in_arg = { ACPI_TYPE_INTEGER };
	struct acpi_object_list arg_list = { 1, &in_arg };
	acpi_status status = AE_OK;

	in_arg.integer.value = acpi_state;
	status = acpi_evaluate_object(NULL, "\\_TTS", &arg_list, NULL);
=======
	acpi_status status;

	status = acpi_execute_simple_method(NULL, "\\_TTS", acpi_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ACPI_FAILURE(status) && status != AE_NOT_FOUND) {
		/*
		 * OS can't evaluate the _TTS object correctly. Some warning
		 * message will be printed. But it won't break anything.
		 */
<<<<<<< HEAD
		printk(KERN_NOTICE "Failure in evaluating _TTS object\n");
=======
		pr_notice("Failure in evaluating _TTS object\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int tts_notify_reboot(struct notifier_block *this,
			unsigned long code, void *x)
{
	acpi_sleep_tts_switch(ACPI_STATE_S5);
	return NOTIFY_DONE;
}

static struct notifier_block tts_notifier = {
	.notifier_call	= tts_notify_reboot,
	.next		= NULL,
	.priority	= 0,
};

<<<<<<< HEAD
static int acpi_sleep_prepare(u32 acpi_state)
{
#ifdef CONFIG_ACPI_SLEEP
	/* do we have a wakeup address for S2 and S3? */
	if (acpi_state == ACPI_STATE_S3) {
		if (!acpi_wakeup_address) {
			return -EFAULT;
		}
		acpi_set_firmware_waking_vector(
				(acpi_physical_address)acpi_wakeup_address);

	}
	ACPI_FLUSH_CPU_CACHE();
#endif
	printk(KERN_INFO PREFIX "Preparing to enter system sleep state S%d\n",
		acpi_state);
=======
#ifndef acpi_skip_set_wakeup_address
#define acpi_skip_set_wakeup_address() false
#endif

static int acpi_sleep_prepare(u32 acpi_state)
{
#ifdef CONFIG_ACPI_SLEEP
	unsigned long acpi_wakeup_address;

	/* do we have a wakeup address for S2 and S3? */
	if (acpi_state == ACPI_STATE_S3 && !acpi_skip_set_wakeup_address()) {
		acpi_wakeup_address = acpi_get_wakeup_address();
		if (!acpi_wakeup_address)
			return -EFAULT;
		acpi_set_waking_vector(acpi_wakeup_address);

	}
#endif
	pr_info("Preparing to enter system sleep state S%d\n", acpi_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_enable_wakeup_devices(acpi_state);
	acpi_enter_sleep_state_prep(acpi_state);
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_ACPI_SLEEP
static u32 acpi_target_sleep_state = ACPI_STATE_S0;

=======
bool acpi_sleep_state_supported(u8 sleep_state)
{
	acpi_status status;
	u8 type_a, type_b;

	status = acpi_get_sleep_type_data(sleep_state, &type_a, &type_b);
	return ACPI_SUCCESS(status) && (!acpi_gbl_reduced_hardware
		|| (acpi_gbl_FADT.sleep_control.address
			&& acpi_gbl_FADT.sleep_status.address));
}

#ifdef CONFIG_ACPI_SLEEP
static u32 acpi_target_sleep_state = ACPI_STATE_S0;

u32 acpi_target_system_state(void)
{
	return acpi_target_sleep_state;
}
EXPORT_SYMBOL_GPL(acpi_target_system_state);

static bool pwr_btn_event_pending;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The ACPI specification wants us to save NVS memory regions during hibernation
 * and to restore them during the subsequent resume.  Windows does that also for
 * suspend to RAM.  However, it is known that this mechanism does not work on
 * all machines, so we allow the user to disable it with the help of the
 * 'acpi_sleep=nonvs' kernel command line option.
 */
static bool nvs_nosave;

void __init acpi_nvs_nosave(void)
{
	nvs_nosave = true;
}

/*
<<<<<<< HEAD
=======
 * The ACPI specification wants us to save NVS memory regions during hibernation
 * but says nothing about saving NVS during S3.  Not all versions of Windows
 * save NVS on S3 suspend either, and it is clear that not all systems need
 * NVS to be saved at S3 time.  To improve suspend/resume time, allow the
 * user to disable saving NVS on S3 if their system does not require it, but
 * continue to save/restore NVS for S4 as specified.
 */
static bool nvs_nosave_s3;

void __init acpi_nvs_nosave_s3(void)
{
	nvs_nosave_s3 = true;
}

static int __init init_nvs_save_s3(const struct dmi_system_id *d)
{
	nvs_nosave_s3 = false;
	return 0;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ACPI 1.0 wants us to execute _PTS before suspending devices, so we allow the
 * user to request that behavior by using the 'acpi_old_suspend_ordering'
 * kernel command line option that causes the following variable to be set.
 */
static bool old_suspend_ordering;

void __init acpi_old_suspend_ordering(void)
{
	old_suspend_ordering = true;
}

static int __init init_old_suspend_ordering(const struct dmi_system_id *d)
{
	acpi_old_suspend_ordering();
	return 0;
}

static int __init init_nvs_nosave(const struct dmi_system_id *d)
{
	acpi_nvs_nosave();
	return 0;
}

<<<<<<< HEAD
static struct dmi_system_id __initdata acpisleep_dmi_table[] = {
=======
bool acpi_sleep_default_s3;

static int __init init_default_s3(const struct dmi_system_id *d)
{
	acpi_sleep_default_s3 = true;
	return 0;
}

static const struct dmi_system_id acpisleep_dmi_table[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
	.callback = init_old_suspend_ordering,
	.ident = "Abit KN9 (nForce4 variant)",
	.matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "http://www.abit.com.tw/"),
		DMI_MATCH(DMI_BOARD_NAME, "KN9 Series(NF-CK804)"),
		},
	},
	{
	.callback = init_old_suspend_ordering,
	.ident = "HP xw4600 Workstation",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Hewlett-Packard"),
		DMI_MATCH(DMI_PRODUCT_NAME, "HP xw4600 Workstation"),
		},
	},
	{
	.callback = init_old_suspend_ordering,
	.ident = "Asus Pundit P1-AH2 (M2N8L motherboard)",
	.matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTek Computer INC."),
		DMI_MATCH(DMI_BOARD_NAME, "M2N8L"),
		},
	},
	{
	.callback = init_old_suspend_ordering,
	.ident = "Panasonic CF51-2L",
	.matches = {
		DMI_MATCH(DMI_BOARD_VENDOR,
				"Matsushita Electric Industrial Co.,Ltd."),
		DMI_MATCH(DMI_BOARD_NAME, "CF51-2L"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-FW41E_H",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-FW41E_H"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-FW21E",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-FW21E"),
		},
	},
	{
	.callback = init_nvs_nosave,
<<<<<<< HEAD
=======
	.ident = "Sony Vaio VGN-FW21M",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-FW21M"),
		},
	},
	{
	.callback = init_nvs_nosave,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ident = "Sony Vaio VPCEB17FX",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCEB17FX"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-SR11M",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-SR11M"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Everex StepNote Series",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Everex Systems, Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "Everex StepNote Series"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VPCEB1Z1E",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCEB1Z1E"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-NW130D",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-NW130D"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VPCCW29FX",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCCW29FX"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Averatec AV1020-ED2",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "AVERATEC"),
		DMI_MATCH(DMI_PRODUCT_NAME, "1000 Series"),
		},
	},
	{
	.callback = init_old_suspend_ordering,
	.ident = "Asus A8N-SLI DELUXE",
	.matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
		DMI_MATCH(DMI_BOARD_NAME, "A8N-SLI DELUXE"),
		},
	},
	{
	.callback = init_old_suspend_ordering,
	.ident = "Asus A8N-SLI Premium",
	.matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
		DMI_MATCH(DMI_BOARD_NAME, "A8N-SLI Premium"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-SR26GN_P",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-SR26GN_P"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VPCEB1S1E",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VPCEB1S1E"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Sony Vaio VGN-FW520F",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "Sony Corporation"),
		DMI_MATCH(DMI_PRODUCT_NAME, "VGN-FW520F"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Asus K54C",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK Computer Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "K54C"),
		},
	},
	{
	.callback = init_nvs_nosave,
	.ident = "Asus K54HR",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK Computer Inc."),
		DMI_MATCH(DMI_PRODUCT_NAME, "K54HR"),
		},
	},
<<<<<<< HEAD
	{},
};

static void acpi_sleep_dmi_check(void)
{
=======
	{
	.callback = init_nvs_save_s3,
	.ident = "Asus 1025C",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "ASUSTeK COMPUTER INC."),
		DMI_MATCH(DMI_PRODUCT_NAME, "1025C"),
		},
	},
	/*
	 * https://bugzilla.kernel.org/show_bug.cgi?id=189431
	 * Lenovo G50-45 is a platform later than 2012, but needs nvs memory
	 * saving during S3.
	 */
	{
	.callback = init_nvs_save_s3,
	.ident = "Lenovo G50-45",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "80E3"),
		},
	},
	{
	.callback = init_nvs_save_s3,
	.ident = "Lenovo G40-45",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "80E1"),
		},
	},
	/*
	 * ThinkPad X1 Tablet(2016) cannot do suspend-to-idle using
	 * the Low Power S0 Idle firmware interface (see
	 * https://bugzilla.kernel.org/show_bug.cgi?id=199057).
	 */
	{
	.callback = init_default_s3,
	.ident = "ThinkPad X1 Tablet(2016)",
	.matches = {
		DMI_MATCH(DMI_SYS_VENDOR, "LENOVO"),
		DMI_MATCH(DMI_PRODUCT_NAME, "20GGA00L00"),
		},
	},
	{},
};

static bool ignore_blacklist;

void __init acpi_sleep_no_blacklist(void)
{
	ignore_blacklist = true;
}

static void __init acpi_sleep_dmi_check(void)
{
	if (ignore_blacklist)
		return;

	if (dmi_get_bios_year() >= 2012)
		acpi_nvs_nosave_s3();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dmi_check_system(acpisleep_dmi_table);
}

/**
 * acpi_pm_freeze - Disable the GPEs and suspend EC transactions.
 */
static int acpi_pm_freeze(void)
{
	acpi_disable_all_gpes();
<<<<<<< HEAD
	acpi_os_wait_events_complete(NULL);
=======
	acpi_os_wait_events_complete();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_ec_block_transactions();
	return 0;
}

/**
<<<<<<< HEAD
 * acpi_pre_suspend - Enable wakeup devices, "freeze" EC and save NVS.
=======
 * acpi_pm_pre_suspend - Enable wakeup devices, "freeze" EC and save NVS.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int acpi_pm_pre_suspend(void)
{
	acpi_pm_freeze();
	return suspend_nvs_save();
}

/**
 *	__acpi_pm_prepare - Prepare the platform to enter the target state.
 *
 *	If necessary, set the firmware waking vector and do arch-specific
 *	nastiness to get the wakeup code to the waking vector.
 */
static int __acpi_pm_prepare(void)
{
	int error = acpi_sleep_prepare(acpi_target_sleep_state);
	if (error)
		acpi_target_sleep_state = ACPI_STATE_S0;

	return error;
}

/**
 *	acpi_pm_prepare - Prepare the platform to enter the target sleep
 *		state and disable the GPEs.
 */
static int acpi_pm_prepare(void)
{
	int error = __acpi_pm_prepare();
	if (!error)
		error = acpi_pm_pre_suspend();

	return error;
}

/**
 *	acpi_pm_finish - Instruct the platform to leave a sleep state.
 *
 *	This is called after we wake back up (or if entering the sleep state
 *	failed).
 */
static void acpi_pm_finish(void)
{
<<<<<<< HEAD
=======
	struct acpi_device *pwr_btn_adev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 acpi_state = acpi_target_sleep_state;

	acpi_ec_unblock_transactions();
	suspend_nvs_free();

	if (acpi_state == ACPI_STATE_S0)
		return;

<<<<<<< HEAD
	printk(KERN_INFO PREFIX "Waking up from system sleep state S%d\n",
		acpi_state);
=======
	pr_info("Waking up from system sleep state S%d\n", acpi_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	acpi_disable_wakeup_devices(acpi_state);
	acpi_leave_sleep_state(acpi_state);

	/* reset firmware waking vector */
<<<<<<< HEAD
	acpi_set_firmware_waking_vector((acpi_physical_address) 0);

	acpi_target_sleep_state = ACPI_STATE_S0;
}

/**
 *	acpi_pm_end - Finish up suspend sequence.
 */
static void acpi_pm_end(void)
{
=======
	acpi_set_waking_vector(0);

	acpi_target_sleep_state = ACPI_STATE_S0;

	acpi_resume_power_resources();

	/* If we were woken with the fixed power button, provide a small
	 * hint to userspace in the form of a wakeup event on the fixed power
	 * button device (if it can be found).
	 *
	 * We delay the event generation til now, as the PM layer requires
	 * timekeeping to be running before we generate events. */
	if (!pwr_btn_event_pending)
		return;

	pwr_btn_event_pending = false;
	pwr_btn_adev = acpi_dev_get_first_match_dev(ACPI_BUTTON_HID_POWERF,
						    NULL, -1);
	if (pwr_btn_adev) {
		pm_wakeup_event(&pwr_btn_adev->dev, 0);
		acpi_dev_put(pwr_btn_adev);
	}
}

/**
 * acpi_pm_start - Start system PM transition.
 * @acpi_state: The target ACPI power state to transition to.
 */
static void acpi_pm_start(u32 acpi_state)
{
	acpi_target_sleep_state = acpi_state;
	acpi_sleep_tts_switch(acpi_target_sleep_state);
	acpi_scan_lock_acquire();
}

/**
 * acpi_pm_end - Finish up system PM transition.
 */
static void acpi_pm_end(void)
{
	acpi_turn_off_unused_power_resources();
	acpi_scan_lock_release();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * This is necessary in case acpi_pm_finish() is not called during a
	 * failing transition to a sleep state.
	 */
	acpi_target_sleep_state = ACPI_STATE_S0;
	acpi_sleep_tts_switch(acpi_target_sleep_state);
}
#else /* !CONFIG_ACPI_SLEEP */
<<<<<<< HEAD
#define acpi_target_sleep_state	ACPI_STATE_S0
=======
#define sleep_no_lps0	(1)
#define acpi_target_sleep_state	ACPI_STATE_S0
#define acpi_sleep_default_s3	(1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void acpi_sleep_dmi_check(void) {}
#endif /* CONFIG_ACPI_SLEEP */

#ifdef CONFIG_SUSPEND
static u32 acpi_suspend_states[] = {
	[PM_SUSPEND_ON] = ACPI_STATE_S0,
	[PM_SUSPEND_STANDBY] = ACPI_STATE_S1,
	[PM_SUSPEND_MEM] = ACPI_STATE_S3,
	[PM_SUSPEND_MAX] = ACPI_STATE_S5
};

/**
<<<<<<< HEAD
 *	acpi_suspend_begin - Set the target system sleep state to the state
 *		associated with given @pm_state, if supported.
=======
 * acpi_suspend_begin - Set the target system sleep state to the state
 *	associated with given @pm_state, if supported.
 * @pm_state: The target system power management state.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int acpi_suspend_begin(suspend_state_t pm_state)
{
	u32 acpi_state = acpi_suspend_states[pm_state];
<<<<<<< HEAD
	int error = 0;

	error = nvs_nosave ? 0 : suspend_nvs_alloc();
	if (error)
		return error;

	if (sleep_states[acpi_state]) {
		acpi_target_sleep_state = acpi_state;
		acpi_sleep_tts_switch(acpi_target_sleep_state);
	} else {
		printk(KERN_ERR "ACPI does not support this state: %d\n",
			pm_state);
		error = -ENOSYS;
	}
	return error;
=======
	int error;

	error = (nvs_nosave || nvs_nosave_s3) ? 0 : suspend_nvs_alloc();
	if (error)
		return error;

	if (!sleep_states[acpi_state]) {
		pr_err("ACPI does not support sleep state S%u\n", acpi_state);
		return -ENOSYS;
	}
	if (acpi_state > ACPI_STATE_S1)
		pm_set_suspend_via_firmware();

	acpi_pm_start(acpi_state);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *	acpi_suspend_enter - Actually enter a sleep state.
 *	@pm_state: ignored
 *
 *	Flush caches and go to sleep. For STR we have to call arch-specific
 *	assembly, which in turn call acpi_enter_sleep_state().
 *	It's unfortunate, but it works. Please fix if you're feeling frisky.
 */
static int acpi_suspend_enter(suspend_state_t pm_state)
{
	acpi_status status = AE_OK;
	u32 acpi_state = acpi_target_sleep_state;
	int error;

<<<<<<< HEAD
	ACPI_FLUSH_CPU_CACHE();

	switch (acpi_state) {
	case ACPI_STATE_S1:
		barrier();
		status = acpi_enter_sleep_state(acpi_state, wake_sleep_flags);
		break;

	case ACPI_STATE_S3:
		error = acpi_suspend_lowlevel();
		if (error)
			return error;
		pr_info(PREFIX "Low-level resume complete\n");
		break;
	}
=======
	trace_suspend_resume(TPS("acpi_suspend"), acpi_state, true);
	switch (acpi_state) {
	case ACPI_STATE_S1:
		barrier();
		status = acpi_enter_sleep_state(acpi_state);
		break;

	case ACPI_STATE_S3:
		if (!acpi_suspend_lowlevel)
			return -ENOSYS;
		error = acpi_suspend_lowlevel();
		if (error)
			return error;
		pr_info("Low-level resume complete\n");
		pm_set_resume_via_firmware();
		break;
	}
	trace_suspend_resume(TPS("acpi_suspend"), acpi_state, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* This violates the spec but is required for bug compatibility. */
	acpi_write_bit_register(ACPI_BITREG_SCI_ENABLE, 1);

<<<<<<< HEAD
	/* Reprogram control registers and execute _BFS */
	acpi_leave_sleep_state_prep(acpi_state, wake_sleep_flags);
=======
	/* Reprogram control registers */
	acpi_leave_sleep_state_prep(acpi_state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ACPI 3.0 specs (P62) says that it's the responsibility
	 * of the OSPM to clear the status bit [ implying that the
	 * POWER_BUTTON event should not reach userspace ]
<<<<<<< HEAD
	 */
	if (ACPI_SUCCESS(status) && (acpi_state == ACPI_STATE_S3))
		acpi_clear_event(ACPI_EVENT_POWER_BUTTON);

	/*
	 * Disable and clear GPE status before interrupt is enabled. Some GPEs
	 * (like wakeup GPE) haven't handler, this can avoid such GPE misfire.
	 * acpi_leave_sleep_state will reenable specific GPEs later
	 */
	acpi_disable_all_gpes();
	/* Allow EC transactions to happen. */
	acpi_ec_unblock_transactions_early();
=======
	 *
	 * However, we do generate a small hint for userspace in the form of
	 * a wakeup event. We flag this condition for now and generate the
	 * event later, as we're currently too early in resume to be able to
	 * generate wakeup events.
	 */
	if (ACPI_SUCCESS(status) && (acpi_state == ACPI_STATE_S3)) {
		acpi_event_status pwr_btn_status = ACPI_EVENT_FLAG_DISABLED;

		acpi_get_event_status(ACPI_EVENT_POWER_BUTTON, &pwr_btn_status);

		if (pwr_btn_status & ACPI_EVENT_FLAG_STATUS_SET) {
			acpi_clear_event(ACPI_EVENT_POWER_BUTTON);
			/* Flag for later */
			pwr_btn_event_pending = true;
		}
	}

	/*
	 * Disable all GPE and clear their status bits before interrupts are
	 * enabled. Some GPEs (like wakeup GPEs) have no handlers and this can
	 * prevent them from producing spurious interrups.
	 *
	 * acpi_leave_sleep_state() will reenable specific GPEs later.
	 *
	 * Because this code runs on one CPU with disabled interrupts (all of
	 * the other CPUs are offline at this time), it need not acquire any
	 * sleeping locks which may trigger an implicit preemption point even
	 * if there is no contention, so avoid doing that by using a low-level
	 * library routine here.
	 */
	acpi_hw_disable_all_gpes();
	/* Allow EC transactions to happen. */
	acpi_ec_unblock_transactions();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	suspend_nvs_restore();

	return ACPI_SUCCESS(status) ? 0 : -EFAULT;
}

static int acpi_suspend_state_valid(suspend_state_t pm_state)
{
	u32 acpi_state;

	switch (pm_state) {
	case PM_SUSPEND_ON:
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		acpi_state = acpi_suspend_states[pm_state];

		return sleep_states[acpi_state];
	default:
		return 0;
	}
}

static const struct platform_suspend_ops acpi_suspend_ops = {
	.valid = acpi_suspend_state_valid,
	.begin = acpi_suspend_begin,
	.prepare_late = acpi_pm_prepare,
	.enter = acpi_suspend_enter,
	.wake = acpi_pm_finish,
	.end = acpi_pm_end,
};

/**
<<<<<<< HEAD
 *	acpi_suspend_begin_old - Set the target system sleep state to the
 *		state associated with given @pm_state, if supported, and
 *		execute the _PTS control method.  This function is used if the
 *		pre-ACPI 2.0 suspend ordering has been requested.
=======
 * acpi_suspend_begin_old - Set the target system sleep state to the
 *	state associated with given @pm_state, if supported, and
 *	execute the _PTS control method.  This function is used if the
 *	pre-ACPI 2.0 suspend ordering has been requested.
 * @pm_state: The target suspend state for the system.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int acpi_suspend_begin_old(suspend_state_t pm_state)
{
	int error = acpi_suspend_begin(pm_state);
	if (!error)
		error = __acpi_pm_prepare();

	return error;
}

/*
 * The following callbacks are used if the pre-ACPI 2.0 suspend ordering has
 * been requested.
 */
static const struct platform_suspend_ops acpi_suspend_ops_old = {
	.valid = acpi_suspend_state_valid,
	.begin = acpi_suspend_begin_old,
	.prepare_late = acpi_pm_pre_suspend,
	.enter = acpi_suspend_enter,
	.wake = acpi_pm_finish,
	.end = acpi_pm_end,
	.recover = acpi_pm_finish,
};
<<<<<<< HEAD
#endif /* CONFIG_SUSPEND */
=======

static bool s2idle_wakeup;

int acpi_s2idle_begin(void)
{
	acpi_scan_lock_acquire();
	return 0;
}

int acpi_s2idle_prepare(void)
{
	if (acpi_sci_irq_valid()) {
		int error;

		error = enable_irq_wake(acpi_sci_irq);
		if (error)
			pr_warn("Warning: Failed to enable wakeup from IRQ %d: %d\n",
				acpi_sci_irq, error);

		acpi_ec_set_gpe_wake_mask(ACPI_GPE_ENABLE);
	}

	acpi_enable_wakeup_devices(ACPI_STATE_S0);

	/* Change the configuration of GPEs to avoid spurious wakeup. */
	acpi_enable_all_wakeup_gpes();
	acpi_os_wait_events_complete();

	s2idle_wakeup = true;
	return 0;
}

bool acpi_s2idle_wake(void)
{
	if (!acpi_sci_irq_valid())
		return pm_wakeup_pending();

	while (pm_wakeup_pending()) {
		/*
		 * If IRQD_WAKEUP_ARMED is set for the SCI at this point, the
		 * SCI has not triggered while suspended, so bail out (the
		 * wakeup is pending anyway and the SCI is not the source of
		 * it).
		 */
		if (irqd_is_wakeup_armed(irq_get_irq_data(acpi_sci_irq))) {
			pm_pr_dbg("Wakeup unrelated to ACPI SCI\n");
			return true;
		}

		/*
		 * If the status bit of any enabled fixed event is set, the
		 * wakeup is regarded as valid.
		 */
		if (acpi_any_fixed_event_status_set()) {
			pm_pr_dbg("ACPI fixed event wakeup\n");
			return true;
		}

		/* Check wakeups from drivers sharing the SCI. */
		if (acpi_check_wakeup_handlers()) {
			pm_pr_dbg("ACPI custom handler wakeup\n");
			return true;
		}

		/*
		 * Check non-EC GPE wakeups and if there are none, cancel the
		 * SCI-related wakeup and dispatch the EC GPE.
		 */
		if (acpi_ec_dispatch_gpe()) {
			pm_pr_dbg("ACPI non-EC GPE wakeup\n");
			return true;
		}

		acpi_os_wait_events_complete();

		/*
		 * The SCI is in the "suspended" state now and it cannot produce
		 * new wakeup events till the rearming below, so if any of them
		 * are pending here, they must be resulting from the processing
		 * of EC events above or coming from somewhere else.
		 */
		if (pm_wakeup_pending()) {
			pm_pr_dbg("Wakeup after ACPI Notify sync\n");
			return true;
		}

		pm_pr_dbg("Rearming ACPI SCI for wakeup\n");

		pm_wakeup_clear(acpi_sci_irq);
		rearm_wake_irq(acpi_sci_irq);
	}

	return false;
}

void acpi_s2idle_restore(void)
{
	/*
	 * Drain pending events before restoring the working-state configuration
	 * of GPEs.
	 */
	acpi_os_wait_events_complete(); /* synchronize GPE processing */
	acpi_ec_flush_work(); /* flush the EC driver's workqueues */
	acpi_os_wait_events_complete(); /* synchronize Notify handling */

	s2idle_wakeup = false;

	acpi_enable_all_runtime_gpes();

	acpi_disable_wakeup_devices(ACPI_STATE_S0);

	if (acpi_sci_irq_valid()) {
		acpi_ec_set_gpe_wake_mask(ACPI_GPE_DISABLE);
		disable_irq_wake(acpi_sci_irq);
	}
}

void acpi_s2idle_end(void)
{
	acpi_scan_lock_release();
}

static const struct platform_s2idle_ops acpi_s2idle_ops = {
	.begin = acpi_s2idle_begin,
	.prepare = acpi_s2idle_prepare,
	.wake = acpi_s2idle_wake,
	.restore = acpi_s2idle_restore,
	.end = acpi_s2idle_end,
};

void __weak acpi_s2idle_setup(void)
{
	if (acpi_gbl_FADT.flags & ACPI_FADT_LOW_POWER_S0)
		pr_info("Efficient low-power S0 idle declared\n");

	s2idle_set_ops(&acpi_s2idle_ops);
}

static void __init acpi_sleep_suspend_setup(void)
{
	bool suspend_ops_needed = false;
	int i;

	for (i = ACPI_STATE_S1; i < ACPI_STATE_S4; i++)
		if (acpi_sleep_state_supported(i)) {
			sleep_states[i] = 1;
			suspend_ops_needed = true;
		}

	if (suspend_ops_needed)
		suspend_set_ops(old_suspend_ordering ?
				&acpi_suspend_ops_old : &acpi_suspend_ops);

	acpi_s2idle_setup();
}

#else /* !CONFIG_SUSPEND */
#define s2idle_wakeup		(false)
static inline void acpi_sleep_suspend_setup(void) {}
#endif /* !CONFIG_SUSPEND */

bool acpi_s2idle_wakeup(void)
{
	return s2idle_wakeup;
}

#ifdef CONFIG_PM_SLEEP
static u32 saved_bm_rld;

static int  acpi_save_bm_rld(void)
{
	acpi_read_bit_register(ACPI_BITREG_BUS_MASTER_RLD, &saved_bm_rld);
	return 0;
}

static void  acpi_restore_bm_rld(void)
{
	u32 resumed_bm_rld = 0;

	acpi_read_bit_register(ACPI_BITREG_BUS_MASTER_RLD, &resumed_bm_rld);
	if (resumed_bm_rld == saved_bm_rld)
		return;

	acpi_write_bit_register(ACPI_BITREG_BUS_MASTER_RLD, saved_bm_rld);
}

static struct syscore_ops acpi_sleep_syscore_ops = {
	.suspend = acpi_save_bm_rld,
	.resume = acpi_restore_bm_rld,
};

static void acpi_sleep_syscore_init(void)
{
	register_syscore_ops(&acpi_sleep_syscore_ops);
}
#else
static inline void acpi_sleep_syscore_init(void) {}
#endif /* CONFIG_PM_SLEEP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_HIBERNATION
static unsigned long s4_hardware_signature;
static struct acpi_table_facs *facs;
<<<<<<< HEAD
static bool nosigcheck;

void __init acpi_no_s4_hw_signature(void)
{
	nosigcheck = true;
}

static int acpi_hibernation_begin(void)
{
	int error;

	error = nvs_nosave ? 0 : suspend_nvs_alloc();
	if (!error) {
		acpi_target_sleep_state = ACPI_STATE_S4;
		acpi_sleep_tts_switch(acpi_target_sleep_state);
	}

	return error;
=======
int acpi_check_s4_hw_signature = -1; /* Default behaviour is just to warn */

static int acpi_hibernation_begin(pm_message_t stage)
{
	if (!nvs_nosave) {
		int error = suspend_nvs_alloc();
		if (error)
			return error;
	}

	if (stage.event == PM_EVENT_HIBERNATE)
		pm_set_suspend_via_firmware();

	acpi_pm_start(ACPI_STATE_S4);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int acpi_hibernation_enter(void)
{
	acpi_status status = AE_OK;

<<<<<<< HEAD
	ACPI_FLUSH_CPU_CACHE();

	/* This shouldn't return.  If it returns, we have a problem */
	status = acpi_enter_sleep_state(ACPI_STATE_S4, wake_sleep_flags);
	/* Reprogram control registers and execute _BFS */
	acpi_leave_sleep_state_prep(ACPI_STATE_S4, wake_sleep_flags);
=======
	/* This shouldn't return.  If it returns, we have a problem */
	status = acpi_enter_sleep_state(ACPI_STATE_S4);
	/* Reprogram control registers */
	acpi_leave_sleep_state_prep(ACPI_STATE_S4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ACPI_SUCCESS(status) ? 0 : -EFAULT;
}

static void acpi_hibernation_leave(void)
{
<<<<<<< HEAD
=======
	pm_set_resume_via_firmware();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * If ACPI is not enabled by the BIOS and the boot kernel, we need to
	 * enable it here.
	 */
	acpi_enable();
<<<<<<< HEAD
	/* Reprogram control registers and execute _BFS */
	acpi_leave_sleep_state_prep(ACPI_STATE_S4, wake_sleep_flags);
	/* Check the hardware signature */
	if (facs && s4_hardware_signature != facs->hardware_signature) {
		printk(KERN_EMERG "ACPI: Hardware changed while hibernated, "
			"cannot resume!\n");
		panic("ACPI S4 hardware signature mismatch");
	}
	/* Restore the NVS memory area */
	suspend_nvs_restore();
	/* Allow EC transactions to happen. */
	acpi_ec_unblock_transactions_early();
=======
	/* Reprogram control registers */
	acpi_leave_sleep_state_prep(ACPI_STATE_S4);
	/* Check the hardware signature */
	if (facs && s4_hardware_signature != facs->hardware_signature)
		pr_crit("Hardware changed while hibernated, success doubtful!\n");
	/* Restore the NVS memory area */
	suspend_nvs_restore();
	/* Allow EC transactions to happen. */
	acpi_ec_unblock_transactions();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acpi_pm_thaw(void)
{
	acpi_ec_unblock_transactions();
	acpi_enable_all_runtime_gpes();
}

static const struct platform_hibernation_ops acpi_hibernation_ops = {
	.begin = acpi_hibernation_begin,
	.end = acpi_pm_end,
	.pre_snapshot = acpi_pm_prepare,
	.finish = acpi_pm_finish,
	.prepare = acpi_pm_prepare,
	.enter = acpi_hibernation_enter,
	.leave = acpi_hibernation_leave,
	.pre_restore = acpi_pm_freeze,
	.restore_cleanup = acpi_pm_thaw,
};

/**
<<<<<<< HEAD
 *	acpi_hibernation_begin_old - Set the target system sleep state to
 *		ACPI_STATE_S4 and execute the _PTS control method.  This
 *		function is used if the pre-ACPI 2.0 suspend ordering has been
 *		requested.
 */
static int acpi_hibernation_begin_old(void)
=======
 * acpi_hibernation_begin_old - Set the target system sleep state to
 *	ACPI_STATE_S4 and execute the _PTS control method.  This
 *	function is used if the pre-ACPI 2.0 suspend ordering has been
 *	requested.
 * @stage: The power management event message.
 */
static int acpi_hibernation_begin_old(pm_message_t stage)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error;
	/*
	 * The _TTS object should always be evaluated before the _PTS object.
	 * When the old_suspended_ordering is true, the _PTS object is
	 * evaluated in the acpi_sleep_prepare.
	 */
	acpi_sleep_tts_switch(ACPI_STATE_S4);

	error = acpi_sleep_prepare(ACPI_STATE_S4);
<<<<<<< HEAD

	if (!error) {
		if (!nvs_nosave)
			error = suspend_nvs_alloc();
		if (!error)
			acpi_target_sleep_state = ACPI_STATE_S4;
	}
	return error;
=======
	if (error)
		return error;

	if (!nvs_nosave) {
		error = suspend_nvs_alloc();
		if (error)
			return error;
	}

	if (stage.event == PM_EVENT_HIBERNATE)
		pm_set_suspend_via_firmware();

	acpi_target_sleep_state = ACPI_STATE_S4;
	acpi_scan_lock_acquire();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The following callbacks are used if the pre-ACPI 2.0 suspend ordering has
 * been requested.
 */
static const struct platform_hibernation_ops acpi_hibernation_ops_old = {
	.begin = acpi_hibernation_begin_old,
	.end = acpi_pm_end,
	.pre_snapshot = acpi_pm_pre_suspend,
	.prepare = acpi_pm_freeze,
	.finish = acpi_pm_finish,
	.enter = acpi_hibernation_enter,
	.leave = acpi_hibernation_leave,
	.pre_restore = acpi_pm_freeze,
	.restore_cleanup = acpi_pm_thaw,
	.recover = acpi_pm_finish,
};
<<<<<<< HEAD
#endif /* CONFIG_HIBERNATION */

int acpi_suspend(u32 acpi_state)
{
	suspend_state_t states[] = {
		[1] = PM_SUSPEND_STANDBY,
		[3] = PM_SUSPEND_MEM,
		[5] = PM_SUSPEND_MAX
	};

	if (acpi_state < 6 && states[acpi_state])
		return pm_suspend(states[acpi_state]);
	if (acpi_state == 4)
		return hibernate();
	return -EINVAL;
}

#ifdef CONFIG_PM
/**
 *	acpi_pm_device_sleep_state - return preferred power state of ACPI device
 *		in the system sleep state given by %acpi_target_sleep_state
 *	@dev: device to examine; its driver model wakeup flags control
 *		whether it should be able to wake up the system
 *	@d_min_p: used to store the upper limit of allowed states range
 *	Return value: preferred power state of the device on success, -ENODEV on
 *		failure (ie. if there's no 'struct acpi_device' for @dev)
 *
 *	Find the lowest power (highest number) ACPI device power state that
 *	device @dev can be in while the system is in the sleep state represented
 *	by %acpi_target_sleep_state.  If @wake is nonzero, the device should be
 *	able to wake up the system from this sleep state.  If @d_min_p is set,
 *	the highest power (lowest number) device power state of @dev allowed
 *	in this system sleep state is stored at the location pointed to by it.
 *
 *	The caller must ensure that @dev is valid before using this function.
 *	The caller is also responsible for figuring out if the device is
 *	supposed to be able to wake up the system and passing this information
 *	via @wake.
 */

int acpi_pm_device_sleep_state(struct device *dev, int *d_min_p)
{
	acpi_handle handle = DEVICE_ACPI_HANDLE(dev);
	struct acpi_device *adev;
	char acpi_method[] = "_SxD";
	unsigned long long d_min, d_max;

	if (!handle || ACPI_FAILURE(acpi_bus_get_device(handle, &adev))) {
		printk(KERN_DEBUG "ACPI handle has no context!\n");
		return -ENODEV;
	}

	acpi_method[2] = '0' + acpi_target_sleep_state;
	/*
	 * If the sleep state is S0, we will return D3, but if the device has
	 * _S0W, we will use the value from _S0W
	 */
	d_min = ACPI_STATE_D0;
	d_max = ACPI_STATE_D3;

	/*
	 * If present, _SxD methods return the minimum D-state (highest power
	 * state) we can use for the corresponding S-states.  Otherwise, the
	 * minimum D-state is D0 (ACPI 3.x).
	 *
	 * NOTE: We rely on acpi_evaluate_integer() not clobbering the integer
	 * provided -- that's our fault recovery, we ignore retval.
	 */
	if (acpi_target_sleep_state > ACPI_STATE_S0)
		acpi_evaluate_integer(handle, acpi_method, NULL, &d_min);

	/*
	 * If _PRW says we can wake up the system from the target sleep state,
	 * the D-state returned by _SxD is sufficient for that (we assume a
	 * wakeup-aware driver if wake is set).  Still, if _SxW exists
	 * (ACPI 3.x), it should return the maximum (lowest power) D-state that
	 * can wake the system.  _S0W may be valid, too.
	 */
	if (acpi_target_sleep_state == ACPI_STATE_S0 ||
	    (device_may_wakeup(dev) && adev->wakeup.flags.valid &&
	     adev->wakeup.sleep_state >= acpi_target_sleep_state)) {
		acpi_status status;

		acpi_method[3] = 'W';
		status = acpi_evaluate_integer(handle, acpi_method, NULL,
						&d_max);
		if (ACPI_FAILURE(status)) {
			if (acpi_target_sleep_state != ACPI_STATE_S0 ||
			    status != AE_NOT_FOUND)
				d_max = d_min;
		} else if (d_max < d_min) {
			/* Warn the user of the broken DSDT */
			printk(KERN_WARNING "ACPI: Wrong value from %s\n",
				acpi_method);
			/* Sanitize it */
			d_min = d_max;
		}
	}

	if (d_min_p)
		*d_min_p = d_min;
	return d_max;
}
#endif /* CONFIG_PM */

#ifdef CONFIG_PM_SLEEP
/**
 * acpi_pm_device_run_wake - Enable/disable wake-up for given device.
 * @phys_dev: Device to enable/disable the platform to wake-up the system for.
 * @enable: Whether enable or disable the wake-up functionality.
 *
 * Find the ACPI device object corresponding to @pci_dev and try to
 * enable/disable the GPE associated with it.
 */
int acpi_pm_device_run_wake(struct device *phys_dev, bool enable)
{
	struct acpi_device *dev;
	acpi_handle handle;

	if (!device_run_wake(phys_dev))
		return -EINVAL;

	handle = DEVICE_ACPI_HANDLE(phys_dev);
	if (!handle || ACPI_FAILURE(acpi_bus_get_device(handle, &dev))) {
		dev_dbg(phys_dev, "ACPI handle has no context in %s!\n",
			__func__);
		return -ENODEV;
	}

	if (enable) {
		acpi_enable_wakeup_device_power(dev, ACPI_STATE_S0);
		acpi_enable_gpe(dev->wakeup.gpe_device, dev->wakeup.gpe_number);
	} else {
		acpi_disable_gpe(dev->wakeup.gpe_device, dev->wakeup.gpe_number);
		acpi_disable_wakeup_device_power(dev);
	}

	return 0;
}

/**
 *	acpi_pm_device_sleep_wake - enable or disable the system wake-up
 *                                  capability of given device
 *	@dev: device to handle
 *	@enable: 'true' - enable, 'false' - disable the wake-up capability
 */
int acpi_pm_device_sleep_wake(struct device *dev, bool enable)
{
	acpi_handle handle;
	struct acpi_device *adev;
	int error;

	if (!device_can_wakeup(dev))
		return -EINVAL;

	handle = DEVICE_ACPI_HANDLE(dev);
	if (!handle || ACPI_FAILURE(acpi_bus_get_device(handle, &adev))) {
		dev_dbg(dev, "ACPI handle has no context in %s!\n", __func__);
		return -ENODEV;
	}

	error = enable ?
		acpi_enable_wakeup_device_power(adev, acpi_target_sleep_state) :
		acpi_disable_wakeup_device_power(adev);
	if (!error)
		dev_info(dev, "wake-up capability %s by ACPI\n",
				enable ? "enabled" : "disabled");

	return error;
}
#endif  /* CONFIG_PM_SLEEP */

static void acpi_power_off_prepare(void)
=======

static void acpi_sleep_hibernate_setup(void)
{
	if (!acpi_sleep_state_supported(ACPI_STATE_S4))
		return;

	hibernation_set_ops(old_suspend_ordering ?
			&acpi_hibernation_ops_old : &acpi_hibernation_ops);
	sleep_states[ACPI_STATE_S4] = 1;
	if (!acpi_check_s4_hw_signature)
		return;

	acpi_get_table(ACPI_SIG_FACS, 1, (struct acpi_table_header **)&facs);
	if (facs) {
		/*
		 * s4_hardware_signature is the local variable which is just
		 * used to warn about mismatch after we're attempting to
		 * resume (in violation of the ACPI specification.)
		 */
		s4_hardware_signature = facs->hardware_signature;

		if (acpi_check_s4_hw_signature > 0) {
			/*
			 * If we're actually obeying the ACPI specification
			 * then the signature is written out as part of the
			 * swsusp header, in order to allow the boot kernel
			 * to gracefully decline to resume.
			 */
			swsusp_hardware_signature = facs->hardware_signature;
		}
	}
}
#else /* !CONFIG_HIBERNATION */
static inline void acpi_sleep_hibernate_setup(void) {}
#endif /* !CONFIG_HIBERNATION */

static int acpi_power_off_prepare(struct sys_off_data *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Prepare to power off the system */
	acpi_sleep_prepare(ACPI_STATE_S5);
	acpi_disable_all_gpes();
<<<<<<< HEAD
}

static void acpi_power_off(void)
{
	/* acpi_sleep_prepare(ACPI_STATE_S5) should have already been called */
	printk(KERN_DEBUG "%s called\n", __func__);
	local_irq_disable();
	acpi_enter_sleep_state(ACPI_STATE_S5, wake_sleep_flags);
}

/*
 * ACPI 2.0 created the optional _GTS and _BFS,
 * but industry adoption has been neither rapid nor broad.
 *
 * Linux gets into trouble when it executes poorly validated
 * paths through the BIOS, so disable _GTS and _BFS by default,
 * but do speak up and offer the option to enable them.
 */
static void __init acpi_gts_bfs_check(void)
{
	acpi_handle dummy;

	if (ACPI_SUCCESS(acpi_get_handle(ACPI_ROOT_OBJECT, METHOD_PATHNAME__GTS, &dummy)))
	{
		printk(KERN_NOTICE PREFIX "BIOS offers _GTS\n");
		printk(KERN_NOTICE PREFIX "If \"acpi.gts=1\" improves suspend, "
			"please notify linux-acpi@vger.kernel.org\n");
	}
	if (ACPI_SUCCESS(acpi_get_handle(ACPI_ROOT_OBJECT, METHOD_PATHNAME__BFS, &dummy)))
	{
		printk(KERN_NOTICE PREFIX "BIOS offers _BFS\n");
		printk(KERN_NOTICE PREFIX "If \"acpi.bfs=1\" improves resume, "
			"please notify linux-acpi@vger.kernel.org\n");
	}
=======
	acpi_os_wait_events_complete();
	return NOTIFY_DONE;
}

static int acpi_power_off(struct sys_off_data *data)
{
	/* acpi_sleep_prepare(ACPI_STATE_S5) should have already been called */
	pr_debug("%s called\n", __func__);
	local_irq_disable();
	acpi_enter_sleep_state(ACPI_STATE_S5);
	return NOTIFY_DONE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int __init acpi_sleep_init(void)
{
<<<<<<< HEAD
	acpi_status status;
	u8 type_a, type_b;
#ifdef CONFIG_SUSPEND
	int i = 0;
#endif

	if (acpi_disabled)
		return 0;
=======
	char supported[ACPI_S_STATE_COUNT * 3 + 1];
	char *pos = supported;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	acpi_sleep_dmi_check();

	sleep_states[ACPI_STATE_S0] = 1;
<<<<<<< HEAD
	printk(KERN_INFO PREFIX "(supports S0");

#ifdef CONFIG_SUSPEND
	for (i = ACPI_STATE_S1; i < ACPI_STATE_S4; i++) {
		status = acpi_get_sleep_type_data(i, &type_a, &type_b);
		if (ACPI_SUCCESS(status)) {
			sleep_states[i] = 1;
			printk(" S%d", i);
		}
	}

	suspend_set_ops(old_suspend_ordering ?
		&acpi_suspend_ops_old : &acpi_suspend_ops);
#endif

#ifdef CONFIG_HIBERNATION
	status = acpi_get_sleep_type_data(ACPI_STATE_S4, &type_a, &type_b);
	if (ACPI_SUCCESS(status)) {
		hibernation_set_ops(old_suspend_ordering ?
			&acpi_hibernation_ops_old : &acpi_hibernation_ops);
		sleep_states[ACPI_STATE_S4] = 1;
		printk(" S4");
		if (!nosigcheck) {
			acpi_get_table(ACPI_SIG_FACS, 1,
				(struct acpi_table_header **)&facs);
			if (facs)
				s4_hardware_signature =
					facs->hardware_signature;
		}
	}
#endif
	status = acpi_get_sleep_type_data(ACPI_STATE_S5, &type_a, &type_b);
	if (ACPI_SUCCESS(status)) {
		sleep_states[ACPI_STATE_S5] = 1;
		printk(" S5");
		pm_power_off_prepare = acpi_power_off_prepare;
		pm_power_off = acpi_power_off;
	}
	printk(")\n");
=======

	acpi_sleep_syscore_init();
	acpi_sleep_suspend_setup();
	acpi_sleep_hibernate_setup();

	if (acpi_sleep_state_supported(ACPI_STATE_S5)) {
		sleep_states[ACPI_STATE_S5] = 1;

		register_sys_off_handler(SYS_OFF_MODE_POWER_OFF_PREPARE,
					 SYS_OFF_PRIO_FIRMWARE,
					 acpi_power_off_prepare, NULL);

		register_sys_off_handler(SYS_OFF_MODE_POWER_OFF,
					 SYS_OFF_PRIO_FIRMWARE,
					 acpi_power_off, NULL);

		/*
		 * Windows uses S5 for reboot, so some BIOSes depend on it to
		 * perform proper reboot.
		 */
		register_sys_off_handler(SYS_OFF_MODE_RESTART_PREPARE,
					 SYS_OFF_PRIO_FIRMWARE,
					 acpi_power_off_prepare, NULL);
	} else {
		acpi_no_s5 = true;
	}

	supported[0] = 0;
	for (i = 0; i < ACPI_S_STATE_COUNT; i++) {
		if (sleep_states[i])
			pos += sprintf(pos, " S%d", i);
	}
	pr_info("(supports%s)\n", supported);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Register the tts_notifier to reboot notifier list so that the _TTS
	 * object can also be evaluated when the system enters S5.
	 */
	register_reboot_notifier(&tts_notifier);
<<<<<<< HEAD
	acpi_gts_bfs_check();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
