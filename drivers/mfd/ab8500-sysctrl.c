<<<<<<< HEAD
/*
 * Copyright (C) ST-Ericsson SA 2010
 * Author: Mattias Nilsson <mattias.i.nilsson@stericsson.com> for ST Ericsson.
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/err.h>
#include <linux/module.h>
#include <linux/platform_device.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * AB8500 system control driver
 *
 * Copyright (C) ST-Ericsson SA 2010
 * Author: Mattias Nilsson <mattias.i.nilsson@stericsson.com> for ST Ericsson.
 */

#include <linux/err.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/reboot.h>
#include <linux/signal.h>
#include <linux/power_supply.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mfd/abx500.h>
#include <linux/mfd/abx500/ab8500.h>
#include <linux/mfd/abx500/ab8500-sysctrl.h>

<<<<<<< HEAD
static struct device *sysctrl_dev;

=======
/* RtcCtrl bits */
#define AB8500_ALARM_MIN_LOW  0x08
#define AB8500_ALARM_MIN_MID 0x09
#define RTC_CTRL 0x0B
#define RTC_ALARM_ENABLE 0x4

static struct device *sysctrl_dev;

static void ab8500_power_off(void)
{
	sigset_t old;
	sigset_t all;
	static const char * const pss[] = {"ab8500_ac", "ab8500_usb"};
	int i;
	bool charger_present = false;
	union power_supply_propval val;
	struct power_supply *psy;
	int ret;

	if (sysctrl_dev == NULL) {
		pr_err("%s: sysctrl not initialized\n", __func__);
		return;
	}

	/*
	 * If we have a charger connected and we're powering off,
	 * reboot into charge-only mode.
	 */

	for (i = 0; i < ARRAY_SIZE(pss); i++) {
		psy = power_supply_get_by_name(pss[i]);
		if (!psy)
			continue;

		ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_ONLINE,
				&val);
		power_supply_put(psy);

		if (!ret && val.intval) {
			charger_present = true;
			break;
		}
	}

	if (!charger_present)
		goto shutdown;

	/* Check if battery is known */
	psy = power_supply_get_by_name("ab8500_btemp");
	if (psy) {
		ret = power_supply_get_property(psy,
				POWER_SUPPLY_PROP_TECHNOLOGY, &val);
		if (!ret && val.intval != POWER_SUPPLY_TECHNOLOGY_UNKNOWN) {
			pr_info("Charger '%s' is connected with known battery",
				pss[i]);
			pr_info(" - Rebooting.\n");
			machine_restart("charging");
		}
		power_supply_put(psy);
	}

shutdown:
	sigfillset(&all);

	if (!sigprocmask(SIG_BLOCK, &all, &old)) {
		(void)ab8500_sysctrl_set(AB8500_STW4500CTRL1,
					 AB8500_STW4500CTRL1_SWOFF |
					 AB8500_STW4500CTRL1_SWRESET4500N);
		(void)sigprocmask(SIG_SETMASK, &old, NULL);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline bool valid_bank(u8 bank)
{
	return ((bank == AB8500_SYS_CTRL1_BLOCK) ||
		(bank == AB8500_SYS_CTRL2_BLOCK));
}

int ab8500_sysctrl_read(u16 reg, u8 *value)
{
	u8 bank;

	if (sysctrl_dev == NULL)
<<<<<<< HEAD
		return -EAGAIN;
=======
		return -EPROBE_DEFER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bank = (reg >> 8);
	if (!valid_bank(bank))
		return -EINVAL;

	return abx500_get_register_interruptible(sysctrl_dev, bank,
		(u8)(reg & 0xFF), value);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(ab8500_sysctrl_read);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int ab8500_sysctrl_write(u16 reg, u8 mask, u8 value)
{
	u8 bank;

	if (sysctrl_dev == NULL)
<<<<<<< HEAD
		return -EAGAIN;

	bank = (reg >> 8);
	if (!valid_bank(bank))
		return -EINVAL;
=======
		return -EPROBE_DEFER;

	bank = (reg >> 8);
	if (!valid_bank(bank)) {
		pr_err("invalid bank\n");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return abx500_mask_and_set_register_interruptible(sysctrl_dev, bank,
		(u8)(reg & 0xFF), mask, value);
}
<<<<<<< HEAD

static int __devinit ab8500_sysctrl_probe(struct platform_device *pdev)
{
	sysctrl_dev = &pdev->dev;
	return 0;
}

static int __devexit ab8500_sysctrl_remove(struct platform_device *pdev)
{
	sysctrl_dev = NULL;
	return 0;
}

static struct platform_driver ab8500_sysctrl_driver = {
	.driver = {
		.name = "ab8500-sysctrl",
		.owner = THIS_MODULE,
	},
	.probe = ab8500_sysctrl_probe,
	.remove = __devexit_p(ab8500_sysctrl_remove),
=======
EXPORT_SYMBOL(ab8500_sysctrl_write);

static int ab8500_sysctrl_probe(struct platform_device *pdev)
{
	sysctrl_dev = &pdev->dev;

	if (!pm_power_off)
		pm_power_off = ab8500_power_off;

	return 0;
}

static void ab8500_sysctrl_remove(struct platform_device *pdev)
{
	sysctrl_dev = NULL;

	if (pm_power_off == ab8500_power_off)
		pm_power_off = NULL;
}

static const struct of_device_id ab8500_sysctrl_match[] = {
	{ .compatible = "stericsson,ab8500-sysctrl", },
	{}
};

static struct platform_driver ab8500_sysctrl_driver = {
	.driver = {
		.name = "ab8500-sysctrl",
		.of_match_table = ab8500_sysctrl_match,
	},
	.probe = ab8500_sysctrl_probe,
	.remove_new = ab8500_sysctrl_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init ab8500_sysctrl_init(void)
{
	return platform_driver_register(&ab8500_sysctrl_driver);
}
<<<<<<< HEAD
subsys_initcall(ab8500_sysctrl_init);

MODULE_AUTHOR("Mattias Nilsson <mattias.i.nilsson@stericsson.com");
MODULE_DESCRIPTION("AB8500 system control driver");
MODULE_LICENSE("GPL v2");
=======
arch_initcall(ab8500_sysctrl_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
