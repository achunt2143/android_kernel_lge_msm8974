<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * nvec_kbd: keyboard driver for a NVIDIA compliant embedded controller
 *
 * Copyright (C) 2011 The AC100 Kernel Team <ac100@lists.launchpad.net>
 *
 * Authors:  Pierre-Hugues Husson <phhusson@free.fr>
 *           Marc Dietrich <marvin24@gmx.de>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/platform_device.h>

#include "nvec-keytable.h"
#include "nvec.h"

<<<<<<< HEAD
#define ACK_KBD_EVENT {'\x05', '\xed', '\x01'}

static const char led_on[3] = "\x05\xed\x07";
static const char led_off[3] = "\x05\xed\x00";
=======
enum kbd_subcmds {
	CNFG_WAKE = 3,
	CNFG_WAKE_KEY_REPORTING,
	SET_LEDS = 0xed,
	ENABLE_KBD = 0xf4,
	DISABLE_KBD,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned char keycodes[ARRAY_SIZE(code_tab_102us)
			      + ARRAY_SIZE(extcode_tab_us102)];

struct nvec_keys {
	struct input_dev *input;
	struct notifier_block notifier;
	struct nvec_chip *nvec;
	bool caps_lock;
};

static struct nvec_keys keys_dev;

static void nvec_kbd_toggle_led(void)
{
<<<<<<< HEAD
	keys_dev.caps_lock = !keys_dev.caps_lock;

	if (keys_dev.caps_lock)
		nvec_write_async(keys_dev.nvec, led_on, sizeof(led_on));
	else
		nvec_write_async(keys_dev.nvec, led_off, sizeof(led_off));
=======
	char buf[] = { NVEC_KBD, SET_LEDS, 0 };

	keys_dev.caps_lock = !keys_dev.caps_lock;

	if (keys_dev.caps_lock)
		/* should be BIT(0) only, firmware bug? */
		buf[2] = BIT(0) | BIT(1) | BIT(2);

	nvec_write_async(keys_dev.nvec, buf, sizeof(buf));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int nvec_keys_notifier(struct notifier_block *nb,
			      unsigned long event_type, void *data)
{
	int code, state;
<<<<<<< HEAD
	unsigned char *msg = (unsigned char *)data;
=======
	unsigned char *msg = data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (event_type == NVEC_KB_EVT) {
		int _size = (msg[0] & (3 << 5)) >> 5;

/* power on/off button */
		if (_size == NVEC_VAR_SIZE)
			return NOTIFY_STOP;

		if (_size == NVEC_3BYTES)
			msg++;

		code = msg[1] & 0x7f;
		state = msg[1] & 0x80;

		if (code_tabs[_size][code] == KEY_CAPSLOCK && state)
			nvec_kbd_toggle_led();

		input_report_key(keys_dev.input, code_tabs[_size][code],
				 !state);
		input_sync(keys_dev.input);

		return NOTIFY_STOP;
	}

	return NOTIFY_DONE;
}

static int nvec_kbd_event(struct input_dev *dev, unsigned int type,
			  unsigned int code, int value)
{
<<<<<<< HEAD
	unsigned char buf[] = ACK_KBD_EVENT;
	struct nvec_chip *nvec = keys_dev.nvec;
=======
	struct nvec_chip *nvec = keys_dev.nvec;
	char buf[] = { NVEC_KBD, SET_LEDS, 0 };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (type == EV_REP)
		return 0;

	if (type != EV_LED)
		return -1;

	if (code != LED_CAPSL)
		return -1;

	buf[2] = !!value;
	nvec_write_async(nvec, buf, sizeof(buf));

	return 0;
}

<<<<<<< HEAD
static int __devinit nvec_kbd_probe(struct platform_device *pdev)
=======
static int nvec_kbd_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	int i, j, err;
	struct input_dev *idev;
<<<<<<< HEAD
=======
	char	clear_leds[] = { NVEC_KBD, SET_LEDS, 0 },
		enable_kbd[] = { NVEC_KBD, ENABLE_KBD },
		cnfg_wake[] = { NVEC_KBD, CNFG_WAKE, true, true },
		cnfg_wake_key_reporting[] = { NVEC_KBD, CNFG_WAKE_KEY_REPORTING,
						true };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	j = 0;

	for (i = 0; i < ARRAY_SIZE(code_tab_102us); ++i)
		keycodes[j++] = code_tab_102us[i];

	for (i = 0; i < ARRAY_SIZE(extcode_tab_us102); ++i)
		keycodes[j++] = extcode_tab_us102[i];

<<<<<<< HEAD
	idev = input_allocate_device();
=======
	idev = devm_input_allocate_device(&pdev->dev);
	if (!idev)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	idev->name = "nvec keyboard";
	idev->phys = "nvec";
	idev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP) | BIT_MASK(EV_LED);
	idev->ledbit[0] = BIT_MASK(LED_CAPSL);
	idev->event = nvec_kbd_event;
	idev->keycode = keycodes;
	idev->keycodesize = sizeof(unsigned char);
	idev->keycodemax = ARRAY_SIZE(keycodes);

	for (i = 0; i < ARRAY_SIZE(keycodes); ++i)
		set_bit(keycodes[i], idev->keybit);

	clear_bit(0, idev->keybit);
	err = input_register_device(idev);
	if (err)
<<<<<<< HEAD
		goto fail;
=======
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	keys_dev.input = idev;
	keys_dev.notifier.notifier_call = nvec_keys_notifier;
	keys_dev.nvec = nvec;
	nvec_register_notifier(nvec, &keys_dev.notifier, 0);

	/* Enable keyboard */
<<<<<<< HEAD
	nvec_write_async(nvec, "\x05\xf4", 2);

	/* keyboard reset? */
	nvec_write_async(nvec, "\x05\x03\x01\x01", 4);
	nvec_write_async(nvec, "\x05\x04\x01", 3);
	nvec_write_async(nvec, "\x06\x01\xff\x03", 4);
/*	FIXME
	wait until keyboard reset is finished
	or until we have a sync write */
	mdelay(1000);

	/* Disable caps lock LED */
	nvec_write_async(nvec, led_off, sizeof(led_off));

	return 0;

fail:
	input_free_device(idev);
	return err;
=======
	nvec_write_async(nvec, enable_kbd, 2);

	/* configures wake on special keys */
	nvec_write_async(nvec, cnfg_wake, 4);
	/* enable wake key reporting */
	nvec_write_async(nvec, cnfg_wake_key_reporting, 3);

	/* Disable caps lock LED */
	nvec_write_async(nvec, clear_leds, sizeof(clear_leds));

	return 0;
}

static void nvec_kbd_remove(struct platform_device *pdev)
{
	struct nvec_chip *nvec = dev_get_drvdata(pdev->dev.parent);
	char disable_kbd[] = { NVEC_KBD, DISABLE_KBD },
	     uncnfg_wake_key_reporting[] = { NVEC_KBD, CNFG_WAKE_KEY_REPORTING,
						false };
	nvec_write_async(nvec, uncnfg_wake_key_reporting, 3);
	nvec_write_async(nvec, disable_kbd, 2);
	nvec_unregister_notifier(nvec, &keys_dev.notifier);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver nvec_kbd_driver = {
	.probe  = nvec_kbd_probe,
<<<<<<< HEAD
	.driver = {
		.name = "nvec-kbd",
		.owner = THIS_MODULE,
	},
};

static int __init nvec_kbd_init(void)
{
	return platform_driver_register(&nvec_kbd_driver);
}

module_init(nvec_kbd_init);

MODULE_AUTHOR("Marc Dietrich <marvin24@gmx.de>");
MODULE_DESCRIPTION("NVEC keyboard driver");
=======
	.remove_new = nvec_kbd_remove,
	.driver = {
		.name = "nvec-kbd",
	},
};

module_platform_driver(nvec_kbd_driver);

MODULE_AUTHOR("Marc Dietrich <marvin24@gmx.de>");
MODULE_DESCRIPTION("NVEC keyboard driver");
MODULE_ALIAS("platform:nvec-kbd");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
