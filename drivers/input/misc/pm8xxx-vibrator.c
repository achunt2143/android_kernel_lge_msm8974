<<<<<<< HEAD
/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/mfd/pm8xxx/core.h>

#define VIB_DRV			0x4A

#define VIB_DRV_SEL_MASK	0xf8
#define VIB_DRV_SEL_SHIFT	0x03
#define VIB_DRV_EN_MANUAL_MASK	0xfc
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 */

#include <linux/errno.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define VIB_MAX_LEVEL_mV	(3100)
#define VIB_MIN_LEVEL_mV	(1200)
#define VIB_MAX_LEVELS		(VIB_MAX_LEVEL_mV - VIB_MIN_LEVEL_mV)

#define MAX_FF_SPEED		0xff

<<<<<<< HEAD
=======
struct pm8xxx_regs {
	unsigned int enable_addr;
	unsigned int enable_mask;

	unsigned int drv_addr;
	unsigned int drv_mask;
	unsigned int drv_shift;
	unsigned int drv_en_manual_mask;
};

static const struct pm8xxx_regs pm8058_regs = {
	.drv_addr = 0x4A,
	.drv_mask = 0xf8,
	.drv_shift = 3,
	.drv_en_manual_mask = 0xfc,
};

static struct pm8xxx_regs pm8916_regs = {
	.enable_addr = 0xc046,
	.enable_mask = BIT(7),
	.drv_addr = 0xc041,
	.drv_mask = 0x1F,
	.drv_shift = 0,
	.drv_en_manual_mask = 0,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * struct pm8xxx_vib - structure to hold vibrator data
 * @vib_input_dev: input device supporting force feedback
 * @work: work structure to set the vibration parameters
<<<<<<< HEAD
 * @dev: device supporting force feedback
 * @speed: speed of vibration set from userland
 * @active: state of vibrator
 * @level: level of vibration to set in the chip
 * @reg_vib_drv: VIB_DRV register value
=======
 * @regmap: regmap for register read/write
 * @regs: registers' info
 * @speed: speed of vibration set from userland
 * @active: state of vibrator
 * @level: level of vibration to set in the chip
 * @reg_vib_drv: regs->drv_addr register value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct pm8xxx_vib {
	struct input_dev *vib_input_dev;
	struct work_struct work;
<<<<<<< HEAD
	struct device *dev;
=======
	struct regmap *regmap;
	const struct pm8xxx_regs *regs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int speed;
	int level;
	bool active;
	u8  reg_vib_drv;
};

/**
<<<<<<< HEAD
 * pm8xxx_vib_read_u8 - helper to read a byte from pmic chip
 * @vib: pointer to vibrator structure
 * @data: placeholder for data to be read
 * @reg: register address
 */
static int pm8xxx_vib_read_u8(struct pm8xxx_vib *vib,
				 u8 *data, u16 reg)
{
	int rc;

	rc = pm8xxx_readb(vib->dev->parent, reg, data);
	if (rc < 0)
		dev_warn(vib->dev, "Error reading pm8xxx reg 0x%x(0x%x)\n",
				reg, rc);
	return rc;
}

/**
 * pm8xxx_vib_write_u8 - helper to write a byte to pmic chip
 * @vib: pointer to vibrator structure
 * @data: data to write
 * @reg: register address
 */
static int pm8xxx_vib_write_u8(struct pm8xxx_vib *vib,
				 u8 data, u16 reg)
{
	int rc;

	rc = pm8xxx_writeb(vib->dev->parent, reg, data);
	if (rc < 0)
		dev_warn(vib->dev, "Error writing pm8xxx reg 0x%x(0x%x)\n",
				reg, rc);
	return rc;
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * pm8xxx_vib_set - handler to start/stop vibration
 * @vib: pointer to vibrator structure
 * @on: state to set
 */
static int pm8xxx_vib_set(struct pm8xxx_vib *vib, bool on)
{
	int rc;
<<<<<<< HEAD
	u8 val = vib->reg_vib_drv;

	if (on)
		val |= ((vib->level << VIB_DRV_SEL_SHIFT) & VIB_DRV_SEL_MASK);
	else
		val &= ~VIB_DRV_SEL_MASK;

	rc = pm8xxx_vib_write_u8(vib, val, VIB_DRV);
=======
	unsigned int val = vib->reg_vib_drv;
	const struct pm8xxx_regs *regs = vib->regs;

	if (on)
		val |= (vib->level << regs->drv_shift) & regs->drv_mask;
	else
		val &= ~regs->drv_mask;

	rc = regmap_write(vib->regmap, regs->drv_addr, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc < 0)
		return rc;

	vib->reg_vib_drv = val;
<<<<<<< HEAD
	return 0;
=======

	if (regs->enable_mask)
		rc = regmap_update_bits(vib->regmap, regs->enable_addr,
					regs->enable_mask, on ? ~0 : 0);

	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * pm8xxx_work_handler - worker to set vibration level
 * @work: pointer to work_struct
 */
static void pm8xxx_work_handler(struct work_struct *work)
{
	struct pm8xxx_vib *vib = container_of(work, struct pm8xxx_vib, work);
<<<<<<< HEAD
	int rc;
	u8 val;

	rc = pm8xxx_vib_read_u8(vib, &val, VIB_DRV);
=======
	const struct pm8xxx_regs *regs = vib->regs;
	int rc;
	unsigned int val;

	rc = regmap_read(vib->regmap, regs->drv_addr, &val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc < 0)
		return;

	/*
	 * pmic vibrator supports voltage ranges from 1.2 to 3.1V, so
	 * scale the level to fit into these ranges.
	 */
	if (vib->speed) {
		vib->active = true;
		vib->level = ((VIB_MAX_LEVELS * vib->speed) / MAX_FF_SPEED) +
						VIB_MIN_LEVEL_mV;
		vib->level /= 100;
	} else {
		vib->active = false;
		vib->level = VIB_MIN_LEVEL_mV / 100;
	}

	pm8xxx_vib_set(vib, vib->active);
}

/**
 * pm8xxx_vib_close - callback of input close callback
 * @dev: input device pointer
 *
 * Turns off the vibrator.
 */
static void pm8xxx_vib_close(struct input_dev *dev)
{
	struct pm8xxx_vib *vib = input_get_drvdata(dev);

	cancel_work_sync(&vib->work);
	if (vib->active)
		pm8xxx_vib_set(vib, false);
}

/**
 * pm8xxx_vib_play_effect - function to handle vib effects.
 * @dev: input device pointer
 * @data: data of effect
 * @effect: effect to play
 *
 * Currently this driver supports only rumble effects.
 */
static int pm8xxx_vib_play_effect(struct input_dev *dev, void *data,
				  struct ff_effect *effect)
{
	struct pm8xxx_vib *vib = input_get_drvdata(dev);

	vib->speed = effect->u.rumble.strong_magnitude >> 8;
	if (!vib->speed)
		vib->speed = effect->u.rumble.weak_magnitude >> 9;

	schedule_work(&vib->work);

	return 0;
}

<<<<<<< HEAD
static int __devinit pm8xxx_vib_probe(struct platform_device *pdev)

=======
static int pm8xxx_vib_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pm8xxx_vib *vib;
	struct input_dev *input_dev;
	int error;
<<<<<<< HEAD
	u8 val;

	vib = kzalloc(sizeof(*vib), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!vib || !input_dev) {
		dev_err(&pdev->dev, "couldn't allocate memory\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	INIT_WORK(&vib->work, pm8xxx_work_handler);
	vib->dev = &pdev->dev;
	vib->vib_input_dev = input_dev;

	/* operate in manual mode */
	error = pm8xxx_vib_read_u8(vib, &val, VIB_DRV);
	if (error < 0)
		goto err_free_mem;
	val &= ~VIB_DRV_EN_MANUAL_MASK;
	error = pm8xxx_vib_write_u8(vib, val, VIB_DRV);
	if (error < 0)
		goto err_free_mem;

=======
	unsigned int val;
	const struct pm8xxx_regs *regs;

	vib = devm_kzalloc(&pdev->dev, sizeof(*vib), GFP_KERNEL);
	if (!vib)
		return -ENOMEM;

	vib->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!vib->regmap)
		return -ENODEV;

	input_dev = devm_input_allocate_device(&pdev->dev);
	if (!input_dev)
		return -ENOMEM;

	INIT_WORK(&vib->work, pm8xxx_work_handler);
	vib->vib_input_dev = input_dev;

	regs = of_device_get_match_data(&pdev->dev);

	/* operate in manual mode */
	error = regmap_read(vib->regmap, regs->drv_addr, &val);
	if (error < 0)
		return error;

	val &= regs->drv_en_manual_mask;
	error = regmap_write(vib->regmap, regs->drv_addr, val);
	if (error < 0)
		return error;

	vib->regs = regs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vib->reg_vib_drv = val;

	input_dev->name = "pm8xxx_vib_ffmemless";
	input_dev->id.version = 1;
<<<<<<< HEAD
	input_dev->dev.parent = &pdev->dev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	input_dev->close = pm8xxx_vib_close;
	input_set_drvdata(input_dev, vib);
	input_set_capability(vib->vib_input_dev, EV_FF, FF_RUMBLE);

	error = input_ff_create_memless(input_dev, NULL,
					pm8xxx_vib_play_effect);
	if (error) {
		dev_err(&pdev->dev,
			"couldn't register vibrator as FF device\n");
<<<<<<< HEAD
		goto err_free_mem;
=======
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = input_register_device(input_dev);
	if (error) {
		dev_err(&pdev->dev, "couldn't register input device\n");
<<<<<<< HEAD
		goto err_destroy_memless;
=======
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, vib);
	return 0;
<<<<<<< HEAD

err_destroy_memless:
	input_ff_destroy(input_dev);
err_free_mem:
	input_free_device(input_dev);
	kfree(vib);

	return error;
}

static int __devexit pm8xxx_vib_remove(struct platform_device *pdev)
{
	struct pm8xxx_vib *vib = platform_get_drvdata(pdev);

	input_unregister_device(vib->vib_input_dev);
	kfree(vib);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
=======
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int pm8xxx_vib_suspend(struct device *dev)
{
	struct pm8xxx_vib *vib = dev_get_drvdata(dev);

	/* Turn off the vibrator */
	pm8xxx_vib_set(vib, false);

	return 0;
}
<<<<<<< HEAD
#endif

static SIMPLE_DEV_PM_OPS(pm8xxx_vib_pm_ops, pm8xxx_vib_suspend, NULL);

static struct platform_driver pm8xxx_vib_driver = {
	.probe		= pm8xxx_vib_probe,
	.remove		= __devexit_p(pm8xxx_vib_remove),
	.driver		= {
		.name	= "pm8xxx-vib",
		.owner	= THIS_MODULE,
		.pm	= &pm8xxx_vib_pm_ops,
=======

static DEFINE_SIMPLE_DEV_PM_OPS(pm8xxx_vib_pm_ops, pm8xxx_vib_suspend, NULL);

static const struct of_device_id pm8xxx_vib_id_table[] = {
	{ .compatible = "qcom,pm8058-vib", .data = &pm8058_regs },
	{ .compatible = "qcom,pm8921-vib", .data = &pm8058_regs },
	{ .compatible = "qcom,pm8916-vib", .data = &pm8916_regs },
	{ }
};
MODULE_DEVICE_TABLE(of, pm8xxx_vib_id_table);

static struct platform_driver pm8xxx_vib_driver = {
	.probe		= pm8xxx_vib_probe,
	.driver		= {
		.name	= "pm8xxx-vib",
		.pm	= pm_sleep_ptr(&pm8xxx_vib_pm_ops),
		.of_match_table = pm8xxx_vib_id_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
module_platform_driver(pm8xxx_vib_driver);

MODULE_ALIAS("platform:pm8xxx_vib");
MODULE_DESCRIPTION("PMIC8xxx vibrator driver based on ff-memless framework");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Amy Maloche <amaloche@codeaurora.org>");
