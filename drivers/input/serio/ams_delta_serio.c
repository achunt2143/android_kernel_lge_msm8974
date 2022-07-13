<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Amstrad E3 (Delta) keyboard port driver
 *
 *  Copyright (c) 2006 Matt Callow
 *  Copyright (c) 2010 Janusz Krzysztofik
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Thanks to Cliff Lawson for his help
 *
 * The Amstrad Delta keyboard (aka mailboard) uses normal PC-AT style serial
 * transmission.  The keyboard port is formed of two GPIO lines, for clock
 * and data.  Due to strict timing requirements of the interface,
 * the serial data stream is read and processed by a FIQ handler.
 * The resulting words are fetched by this driver from a circular buffer.
 *
 * Standard AT keyboard driver (atkbd) is used for handling the keyboard data.
 * However, when used with the E3 mailboard that producecs non-standard
 * scancodes, a custom key table must be prepared and loaded from userspace.
 */
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/irq.h>
=======
#include <linux/irq.h>
#include <linux/platform_data/ams-delta-fiq.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/serio.h>
#include <linux/slab.h>
#include <linux/module.h>

<<<<<<< HEAD
#include <asm/mach-types.h>
#include <plat/board-ams-delta.h>

#include <mach/ams-delta-fiq.h>
=======
#define DRIVER_NAME	"ams-delta-serio"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Matt Callow");
MODULE_DESCRIPTION("AMS Delta (E3) keyboard port driver");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
static struct serio *ams_delta_serio;

static int check_data(int data)
=======
struct ams_delta_serio {
	struct serio *serio;
	struct regulator *vcc;
	unsigned int *fiq_buffer;
};

static int check_data(struct serio *serio, int data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i, parity = 0;

	/* check valid stop bit */
	if (!(data & 0x400)) {
<<<<<<< HEAD
		dev_warn(&ams_delta_serio->dev,
				"invalid stop bit, data=0x%X\n",
				data);
=======
		dev_warn(&serio->dev, "invalid stop bit, data=0x%X\n", data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return SERIO_FRAME;
	}
	/* calculate the parity */
	for (i = 1; i < 10; i++) {
		if (data & (1 << i))
			parity++;
	}
	/* it should be odd */
	if (!(parity & 0x01)) {
<<<<<<< HEAD
		dev_warn(&ams_delta_serio->dev,
				"paritiy check failed, data=0x%X parity=0x%X\n",
				data, parity);
=======
		dev_warn(&serio->dev,
			 "parity check failed, data=0x%X parity=0x%X\n", data,
			 parity);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return SERIO_PARITY;
	}
	return 0;
}

static irqreturn_t ams_delta_serio_interrupt(int irq, void *dev_id)
{
<<<<<<< HEAD
	int *circ_buff = &fiq_buffer[FIQ_CIRC_BUFF];
	int data, dfl;
	u8 scancode;

	fiq_buffer[FIQ_IRQ_PEND] = 0;
=======
	struct ams_delta_serio *priv = dev_id;
	int *circ_buff = &priv->fiq_buffer[FIQ_CIRC_BUFF];
	int data, dfl;
	u8 scancode;

	priv->fiq_buffer[FIQ_IRQ_PEND] = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Read data from the circular buffer, check it
	 * and then pass it on the serio
	 */
<<<<<<< HEAD
	while (fiq_buffer[FIQ_KEYS_CNT] > 0) {

		data = circ_buff[fiq_buffer[FIQ_HEAD_OFFSET]++];
		fiq_buffer[FIQ_KEYS_CNT]--;
		if (fiq_buffer[FIQ_HEAD_OFFSET] == fiq_buffer[FIQ_BUF_LEN])
			fiq_buffer[FIQ_HEAD_OFFSET] = 0;

		dfl = check_data(data);
		scancode = (u8) (data >> 1) & 0xFF;
		serio_interrupt(ams_delta_serio, scancode, dfl);
=======
	while (priv->fiq_buffer[FIQ_KEYS_CNT] > 0) {

		data = circ_buff[priv->fiq_buffer[FIQ_HEAD_OFFSET]++];
		priv->fiq_buffer[FIQ_KEYS_CNT]--;
		if (priv->fiq_buffer[FIQ_HEAD_OFFSET] ==
		    priv->fiq_buffer[FIQ_BUF_LEN])
			priv->fiq_buffer[FIQ_HEAD_OFFSET] = 0;

		dfl = check_data(priv->serio, data);
		scancode = (u8) (data >> 1) & 0xFF;
		serio_interrupt(priv->serio, scancode, dfl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return IRQ_HANDLED;
}

static int ams_delta_serio_open(struct serio *serio)
{
<<<<<<< HEAD
	/* enable keyboard */
	gpio_set_value(AMS_DELTA_GPIO_PIN_KEYBRD_PWR, 1);

	return 0;
=======
	struct ams_delta_serio *priv = serio->port_data;

	/* enable keyboard */
	return regulator_enable(priv->vcc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ams_delta_serio_close(struct serio *serio)
{
<<<<<<< HEAD
	/* disable keyboard */
	gpio_set_value(AMS_DELTA_GPIO_PIN_KEYBRD_PWR, 0);
}

static const struct gpio ams_delta_gpios[] __initconst_or_module = {
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_DATA,
		.flags	= GPIOF_DIR_IN,
		.label	= "serio-data",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_CLK,
		.flags	= GPIOF_DIR_IN,
		.label	= "serio-clock",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_PWR,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "serio-power",
	},
	{
		.gpio	= AMS_DELTA_GPIO_PIN_KEYBRD_DATAOUT,
		.flags	= GPIOF_OUT_INIT_LOW,
		.label	= "serio-dataout",
	},
};

static int __init ams_delta_serio_init(void)
{
	int err;

	if (!machine_is_ams_delta())
		return -ENODEV;

	ams_delta_serio = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (!ams_delta_serio)
		return -ENOMEM;

	ams_delta_serio->id.type = SERIO_8042;
	ams_delta_serio->open = ams_delta_serio_open;
	ams_delta_serio->close = ams_delta_serio_close;
	strlcpy(ams_delta_serio->name, "AMS DELTA keyboard adapter",
			sizeof(ams_delta_serio->name));
	strlcpy(ams_delta_serio->phys, "GPIO/serio0",
			sizeof(ams_delta_serio->phys));

	err = gpio_request_array(ams_delta_gpios,
				ARRAY_SIZE(ams_delta_gpios));
	if (err) {
		pr_err("ams_delta_serio: Couldn't request gpio pins\n");
		goto serio;
	}

	err = request_irq(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK),
			ams_delta_serio_interrupt, IRQ_TYPE_EDGE_RISING,
			"ams-delta-serio", 0);
	if (err < 0) {
		pr_err("ams_delta_serio: couldn't request gpio interrupt %d\n",
				gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK));
		goto gpio;
	}
	/*
	 * Since GPIO register handling for keyboard clock pin is performed
	 * at FIQ level, switch back from edge to simple interrupt handler
	 * to avoid bad interaction.
	 */
	irq_set_handler(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK),
			handle_simple_irq);

	serio_register_port(ams_delta_serio);
	dev_info(&ams_delta_serio->dev, "%s\n", ams_delta_serio->name);

	return 0;
gpio:
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
serio:
	kfree(ams_delta_serio);
	return err;
}
module_init(ams_delta_serio_init);

static void __exit ams_delta_serio_exit(void)
{
	serio_unregister_port(ams_delta_serio);
	free_irq(gpio_to_irq(AMS_DELTA_GPIO_PIN_KEYBRD_CLK), 0);
	gpio_free_array(ams_delta_gpios,
			ARRAY_SIZE(ams_delta_gpios));
}
module_exit(ams_delta_serio_exit);
=======
	struct ams_delta_serio *priv = serio->port_data;

	/* disable keyboard */
	regulator_disable(priv->vcc);
}

static int ams_delta_serio_init(struct platform_device *pdev)
{
	struct ams_delta_serio *priv;
	struct serio *serio;
	int irq, err;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->fiq_buffer = pdev->dev.platform_data;
	if (!priv->fiq_buffer)
		return -EINVAL;

	priv->vcc = devm_regulator_get(&pdev->dev, "vcc");
	if (IS_ERR(priv->vcc)) {
		err = PTR_ERR(priv->vcc);
		dev_err(&pdev->dev, "regulator request failed (%d)\n", err);
		/*
		 * When running on a non-dt platform and requested regulator
		 * is not available, devm_regulator_get() never returns
		 * -EPROBE_DEFER as it is not able to justify if the regulator
		 * may still appear later.  On the other hand, the board can
		 * still set full constriants flag at late_initcall in order
		 * to instruct devm_regulator_get() to returnn a dummy one
		 * if sufficient.  Hence, if we get -ENODEV here, let's convert
		 * it to -EPROBE_DEFER and wait for the board to decide or
		 * let Deferred Probe infrastructure handle this error.
		 */
		if (err == -ENODEV)
			err = -EPROBE_DEFER;
		return err;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return -ENXIO;

	err = devm_request_irq(&pdev->dev, irq, ams_delta_serio_interrupt,
			       IRQ_TYPE_EDGE_RISING, DRIVER_NAME, priv);
	if (err < 0) {
		dev_err(&pdev->dev, "IRQ request failed (%d)\n", err);
		return err;
	}

	serio = kzalloc(sizeof(*serio), GFP_KERNEL);
	if (!serio)
		return -ENOMEM;

	priv->serio = serio;

	serio->id.type = SERIO_8042;
	serio->open = ams_delta_serio_open;
	serio->close = ams_delta_serio_close;
	strscpy(serio->name, "AMS DELTA keyboard adapter", sizeof(serio->name));
	strscpy(serio->phys, dev_name(&pdev->dev), sizeof(serio->phys));
	serio->dev.parent = &pdev->dev;
	serio->port_data = priv;

	serio_register_port(serio);

	platform_set_drvdata(pdev, priv);

	dev_info(&serio->dev, "%s\n", serio->name);

	return 0;
}

static void ams_delta_serio_exit(struct platform_device *pdev)
{
	struct ams_delta_serio *priv = platform_get_drvdata(pdev);

	serio_unregister_port(priv->serio);
}

static struct platform_driver ams_delta_serio_driver = {
	.probe	= ams_delta_serio_init,
	.remove_new = ams_delta_serio_exit,
	.driver	= {
		.name	= DRIVER_NAME
	},
};
module_platform_driver(ams_delta_serio_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
