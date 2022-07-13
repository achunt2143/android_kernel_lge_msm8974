<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  i2c_pca_platform.c
 *
 *  Platform driver for the PCA9564 I2C controller.
 *
 *  Copyright (C) 2008 Pengutronix
 *
<<<<<<< HEAD
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 */
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/i2c-algo-pca.h>
<<<<<<< HEAD
#include <linux/i2c-pca-platform.h>
#include <linux/gpio.h>
#include <linux/io.h>
=======
#include <linux/platform_data/i2c-pca-platform.h>
#include <linux/gpio/consumer.h>
#include <linux/io.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/irq.h>

struct i2c_pca_pf_data {
	void __iomem			*reg_base;
	int				irq;	/* if 0, use polling */
<<<<<<< HEAD
	int				gpio;
	wait_queue_head_t		wait;
	struct i2c_adapter		adap;
	struct i2c_algo_pca_data	algo_data;
	unsigned long			io_base;
	unsigned long			io_size;
=======
	struct gpio_desc		*gpio;
	wait_queue_head_t		wait;
	struct i2c_adapter		adap;
	struct i2c_algo_pca_data	algo_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Read/Write functions for different register alignments */

static int i2c_pca_pf_readbyte8(void *pd, int reg)
{
	struct i2c_pca_pf_data *i2c = pd;
	return ioread8(i2c->reg_base + reg);
}

static int i2c_pca_pf_readbyte16(void *pd, int reg)
{
	struct i2c_pca_pf_data *i2c = pd;
	return ioread8(i2c->reg_base + reg * 2);
}

static int i2c_pca_pf_readbyte32(void *pd, int reg)
{
	struct i2c_pca_pf_data *i2c = pd;
	return ioread8(i2c->reg_base + reg * 4);
}

static void i2c_pca_pf_writebyte8(void *pd, int reg, int val)
{
	struct i2c_pca_pf_data *i2c = pd;
	iowrite8(val, i2c->reg_base + reg);
}

static void i2c_pca_pf_writebyte16(void *pd, int reg, int val)
{
	struct i2c_pca_pf_data *i2c = pd;
	iowrite8(val, i2c->reg_base + reg * 2);
}

static void i2c_pca_pf_writebyte32(void *pd, int reg, int val)
{
	struct i2c_pca_pf_data *i2c = pd;
	iowrite8(val, i2c->reg_base + reg * 4);
}


static int i2c_pca_pf_waitforcompletion(void *pd)
{
	struct i2c_pca_pf_data *i2c = pd;
	unsigned long timeout;
	long ret;

	if (i2c->irq) {
		ret = wait_event_timeout(i2c->wait,
			i2c->algo_data.read_byte(i2c, I2C_PCA_CON)
			& I2C_PCA_CON_SI, i2c->adap.timeout);
	} else {
		/* Do polling */
		timeout = jiffies + i2c->adap.timeout;
		do {
			ret = time_before(jiffies, timeout);
			if (i2c->algo_data.read_byte(i2c, I2C_PCA_CON)
					& I2C_PCA_CON_SI)
				break;
			udelay(100);
		} while (ret);
	}

	return ret > 0;
}

static void i2c_pca_pf_dummyreset(void *pd)
{
	struct i2c_pca_pf_data *i2c = pd;
<<<<<<< HEAD
	printk(KERN_WARNING "%s: No reset-pin found. Chip may get stuck!\n",
		i2c->adap.name);
=======

	dev_warn(&i2c->adap.dev, "No reset-pin found. Chip may get stuck!\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void i2c_pca_pf_resetchip(void *pd)
{
	struct i2c_pca_pf_data *i2c = pd;

<<<<<<< HEAD
	gpio_set_value(i2c->gpio, 0);
	ndelay(100);
	gpio_set_value(i2c->gpio, 1);
=======
	gpiod_set_value(i2c->gpio, 1);
	ndelay(100);
	gpiod_set_value(i2c->gpio, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t i2c_pca_pf_handler(int this_irq, void *dev_id)
{
	struct i2c_pca_pf_data *i2c = dev_id;

	if ((i2c->algo_data.read_byte(i2c, I2C_PCA_CON) & I2C_PCA_CON_SI) == 0)
		return IRQ_NONE;

	wake_up(&i2c->wait);

	return IRQ_HANDLED;
}


<<<<<<< HEAD
static int __devinit i2c_pca_pf_probe(struct platform_device *pdev)
=======
static int i2c_pca_pf_probe(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct i2c_pca_pf_data *i2c;
	struct resource *res;
	struct i2c_pca9564_pf_platform_data *platform_data =
<<<<<<< HEAD
				pdev->dev.platform_data;
	int ret = 0;
	int irq;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	/* If irq is 0, we do polling. */

	if (res == NULL) {
		ret = -ENODEV;
		goto e_print;
	}

	if (!request_mem_region(res->start, resource_size(res), res->name)) {
		ret = -ENOMEM;
		goto e_print;
	}

	i2c = kzalloc(sizeof(struct i2c_pca_pf_data), GFP_KERNEL);
	if (!i2c) {
		ret = -ENOMEM;
		goto e_alloc;
	}

	init_waitqueue_head(&i2c->wait);

	i2c->reg_base = ioremap(res->start, resource_size(res));
	if (!i2c->reg_base) {
		ret = -ENOMEM;
		goto e_remap;
	}
	i2c->io_base = res->start;
	i2c->io_size = resource_size(res);
	i2c->irq = irq;

	i2c->adap.nr = pdev->id >= 0 ? pdev->id : 0;
=======
				dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	int ret = 0;
	int irq;

	irq = platform_get_irq_optional(pdev, 0);
	/* If irq is 0, we do polling. */
	if (irq < 0)
		irq = 0;

	i2c = devm_kzalloc(&pdev->dev, sizeof(*i2c), GFP_KERNEL);
	if (!i2c)
		return -ENOMEM;

	i2c->reg_base = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(i2c->reg_base))
		return PTR_ERR(i2c->reg_base);


	init_waitqueue_head(&i2c->wait);

	i2c->irq = irq;

	i2c->adap.nr = pdev->id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	i2c->adap.owner = THIS_MODULE;
	snprintf(i2c->adap.name, sizeof(i2c->adap.name),
		 "PCA9564/PCA9665 at 0x%08lx",
		 (unsigned long) res->start);
	i2c->adap.algo_data = &i2c->algo_data;
	i2c->adap.dev.parent = &pdev->dev;
<<<<<<< HEAD
=======
	i2c->adap.dev.of_node = np;

	i2c->gpio = devm_gpiod_get_optional(&pdev->dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(i2c->gpio))
		return PTR_ERR(i2c->gpio);

	i2c->adap.timeout = HZ;
	ret = device_property_read_u32(&pdev->dev, "clock-frequency",
				       &i2c->algo_data.i2c_clock);
	if (ret)
		i2c->algo_data.i2c_clock = 59000;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (platform_data) {
		i2c->adap.timeout = platform_data->timeout;
		i2c->algo_data.i2c_clock = platform_data->i2c_clock_speed;
<<<<<<< HEAD
		i2c->gpio = platform_data->gpio;
	} else {
		i2c->adap.timeout = HZ;
		i2c->algo_data.i2c_clock = 59000;
		i2c->gpio = -1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	i2c->algo_data.data = i2c;
	i2c->algo_data.wait_for_completion = i2c_pca_pf_waitforcompletion;
<<<<<<< HEAD
	i2c->algo_data.reset_chip = i2c_pca_pf_dummyreset;
=======
	if (i2c->gpio)
		i2c->algo_data.reset_chip = i2c_pca_pf_resetchip;
	else
		i2c->algo_data.reset_chip = i2c_pca_pf_dummyreset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (res->flags & IORESOURCE_MEM_TYPE_MASK) {
	case IORESOURCE_MEM_32BIT:
		i2c->algo_data.write_byte = i2c_pca_pf_writebyte32;
		i2c->algo_data.read_byte = i2c_pca_pf_readbyte32;
		break;
	case IORESOURCE_MEM_16BIT:
		i2c->algo_data.write_byte = i2c_pca_pf_writebyte16;
		i2c->algo_data.read_byte = i2c_pca_pf_readbyte16;
		break;
	case IORESOURCE_MEM_8BIT:
	default:
		i2c->algo_data.write_byte = i2c_pca_pf_writebyte8;
		i2c->algo_data.read_byte = i2c_pca_pf_readbyte8;
		break;
	}

<<<<<<< HEAD
	/* Use gpio_is_valid() when in mainline */
	if (i2c->gpio > -1) {
		ret = gpio_request(i2c->gpio, i2c->adap.name);
		if (ret == 0) {
			gpio_direction_output(i2c->gpio, 1);
			i2c->algo_data.reset_chip = i2c_pca_pf_resetchip;
		} else {
			printk(KERN_WARNING "%s: Registering gpio failed!\n",
				i2c->adap.name);
			i2c->gpio = ret;
		}
	}

	if (irq) {
		ret = request_irq(irq, i2c_pca_pf_handler,
			IRQF_TRIGGER_FALLING, pdev->name, i2c);
		if (ret)
			goto e_reqirq;
	}

	if (i2c_pca_add_numbered_bus(&i2c->adap) < 0) {
		ret = -ENODEV;
		goto e_adapt;
	}

	platform_set_drvdata(pdev, i2c);

	printk(KERN_INFO "%s registered.\n", i2c->adap.name);

	return 0;

e_adapt:
	if (irq)
		free_irq(irq, i2c);
e_reqirq:
	if (i2c->gpio > -1)
		gpio_free(i2c->gpio);

	iounmap(i2c->reg_base);
e_remap:
	kfree(i2c);
e_alloc:
	release_mem_region(res->start, resource_size(res));
e_print:
	printk(KERN_ERR "Registering PCA9564/PCA9665 FAILED! (%d)\n", ret);
	return ret;
}

static int __devexit i2c_pca_pf_remove(struct platform_device *pdev)
{
	struct i2c_pca_pf_data *i2c = platform_get_drvdata(pdev);
	platform_set_drvdata(pdev, NULL);

	i2c_del_adapter(&i2c->adap);

	if (i2c->irq)
		free_irq(i2c->irq, i2c);

	if (i2c->gpio > -1)
		gpio_free(i2c->gpio);

	iounmap(i2c->reg_base);
	release_mem_region(i2c->io_base, i2c->io_size);
	kfree(i2c);

	return 0;
}

static struct platform_driver i2c_pca_pf_driver = {
	.probe = i2c_pca_pf_probe,
	.remove = __devexit_p(i2c_pca_pf_remove),
	.driver = {
		.name = "i2c-pca-platform",
		.owner = THIS_MODULE,
=======
	if (irq) {
		ret = devm_request_irq(&pdev->dev, irq, i2c_pca_pf_handler,
			IRQF_TRIGGER_FALLING, pdev->name, i2c);
		if (ret)
			return ret;
	}

	ret = i2c_pca_add_numbered_bus(&i2c->adap);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, i2c);

	dev_info(&pdev->dev, "registered.\n");

	return 0;
}

static void i2c_pca_pf_remove(struct platform_device *pdev)
{
	struct i2c_pca_pf_data *i2c = platform_get_drvdata(pdev);

	i2c_del_adapter(&i2c->adap);
}

#ifdef CONFIG_OF
static const struct of_device_id i2c_pca_of_match_table[] = {
	{ .compatible = "nxp,pca9564" },
	{ .compatible = "nxp,pca9665" },
	{},
};
MODULE_DEVICE_TABLE(of, i2c_pca_of_match_table);
#endif

static struct platform_driver i2c_pca_pf_driver = {
	.probe = i2c_pca_pf_probe,
	.remove_new = i2c_pca_pf_remove,
	.driver = {
		.name = "i2c-pca-platform",
		.of_match_table = of_match_ptr(i2c_pca_of_match_table),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(i2c_pca_pf_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Wolfram Sang <w.sang@pengutronix.de>");
=======
MODULE_AUTHOR("Wolfram Sang <kernel@pengutronix.de>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("I2C-PCA9564/PCA9665 platform driver");
MODULE_LICENSE("GPL");
