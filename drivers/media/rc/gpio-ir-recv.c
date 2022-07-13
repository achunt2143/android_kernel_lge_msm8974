<<<<<<< HEAD
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/pm_qos.h>
#include <linux/timer.h>
#include <media/rc-core.h>
#include <media/gpio-ir-recv.h>

#define GPIO_IR_DRIVER_NAME	"gpio-rc-recv"
#define GPIO_IR_DEVICE_NAME	"gpio_ir_recv"

static int gpio_ir_timeout = 200;
module_param_named(gpio_ir_timeout, gpio_ir_timeout, int, 0664);

static int __init gpio_ir_timeout_setup(char *p)
{
	gpio_ir_timeout = memparse(p, NULL);
	return 0;
}

early_param("gpio_ir_timeout", gpio_ir_timeout_setup);

struct gpio_rc_dev {
	struct rc_dev *rcdev;
	struct pm_qos_request pm_qos_req;
	struct timer_list gpio_ir_timer;
	int gpio_nr;
	bool active_low;
	int can_sleep;
	bool can_wakeup;
	bool pm_qos_vote;
	int gpio_irq_latency;
=======
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/pm_qos.h>
#include <linux/irq.h>
#include <media/rc-core.h>

#define GPIO_IR_DEVICE_NAME	"gpio_ir_recv"

struct gpio_rc_dev {
	struct rc_dev *rcdev;
	struct gpio_desc *gpiod;
	int irq;
	struct device *pmdev;
	struct pm_qos_request qos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static irqreturn_t gpio_ir_recv_irq(int irq, void *dev_id)
{
<<<<<<< HEAD
	struct gpio_rc_dev *gpio_dev = dev_id;
	int gval;
	int rc = 0;
	enum raw_event_type type = IR_SPACE;

	if (!gpio_dev->pm_qos_vote && gpio_dev->can_wakeup) {
		gpio_dev->pm_qos_vote = 1;
		pm_qos_update_request(&gpio_dev->pm_qos_req,
					 gpio_dev->gpio_irq_latency);
	}

	if (gpio_dev->can_sleep)
		gval = gpio_get_value_cansleep(gpio_dev->gpio_nr);
	else
		gval = gpio_get_value(gpio_dev->gpio_nr);

	if (gval < 0)
		goto err_get_value;

	if (gpio_dev->active_low)
		gval = !gval;

	if (gval == 1)
		type = IR_PULSE;

	rc = ir_raw_event_store_edge(gpio_dev->rcdev, type);
	if (rc < 0)
		goto err_get_value;

	ir_raw_event_handle(gpio_dev->rcdev);

	if (gpio_dev->can_wakeup)
		mod_timer(&gpio_dev->gpio_ir_timer,
				jiffies + msecs_to_jiffies(gpio_ir_timeout));
err_get_value:
	return IRQ_HANDLED;
}

static void gpio_ir_timer(unsigned long data)
{
	struct gpio_rc_dev *gpio_dev = (struct gpio_rc_dev *)data;

	pm_qos_update_request(&gpio_dev->pm_qos_req, PM_QOS_DEFAULT_VALUE);
	pm_qos_request_active(&gpio_dev->pm_qos_req);
	gpio_dev->pm_qos_vote = 0;
}

static int __devinit gpio_ir_recv_probe(struct platform_device *pdev)
{
	struct gpio_rc_dev *gpio_dev;
	struct rc_dev *rcdev;
	const struct gpio_ir_recv_platform_data *pdata =
					pdev->dev.platform_data;
	int rc;

	if (!pdata)
		return -EINVAL;

	if (pdata->gpio_nr < 0)
		return -EINVAL;

	gpio_dev = kzalloc(sizeof(struct gpio_rc_dev), GFP_KERNEL);
	if (!gpio_dev)
		return -ENOMEM;

	rcdev = rc_allocate_device();
	if (!rcdev) {
		rc = -ENOMEM;
		goto err_allocate_device;
	}

	rcdev->driver_type = RC_DRIVER_IR_RAW;
	rcdev->allowed_protos = RC_TYPE_ALL;
	rcdev->input_name = GPIO_IR_DEVICE_NAME;
	rcdev->input_id.bustype = BUS_HOST;
	rcdev->driver_name = GPIO_IR_DRIVER_NAME;
	rcdev->map_name = RC_MAP_SAMSUNG_NECX;

	gpio_dev->rcdev = rcdev;
	gpio_dev->gpio_nr = pdata->gpio_nr;
	gpio_dev->active_low = pdata->active_low;
	gpio_dev->can_wakeup = pdata->can_wakeup;
	gpio_dev->gpio_irq_latency = pdata->swfi_latency + 1;
	gpio_dev->pm_qos_vote = 0;

	rc = gpio_request(pdata->gpio_nr, "gpio-ir-recv");
	if (rc < 0)
		goto err_gpio_request;

	gpio_dev->can_sleep = gpio_cansleep(pdata->gpio_nr);

	rc  = gpio_direction_input(pdata->gpio_nr);
	if (rc < 0)
		goto err_gpio_direction_input;

	rc = rc_register_device(rcdev);
	if (rc < 0) {
		dev_err(&pdev->dev, "failed to register rc device\n");
		goto err_register_rc_device;
=======
	int val;
	struct gpio_rc_dev *gpio_dev = dev_id;
	struct device *pmdev = gpio_dev->pmdev;

	/*
	 * For some cpuidle systems, not all:
	 * Respond to interrupt taking more latency when cpu in idle.
	 * Invoke asynchronous pm runtime get from interrupt context,
	 * this may introduce a millisecond delay to call resume callback,
	 * where to disable cpuilde.
	 *
	 * Two issues lead to fail to decode first frame, one is latency to
	 * respond to interrupt, another is delay introduced by async api.
	 */
	if (pmdev)
		pm_runtime_get(pmdev);

	val = gpiod_get_value(gpio_dev->gpiod);
	if (val >= 0)
		ir_raw_event_store_edge(gpio_dev->rcdev, val == 1);

	if (pmdev) {
		pm_runtime_mark_last_busy(pmdev);
		pm_runtime_put_autosuspend(pmdev);
	}

	return IRQ_HANDLED;
}

static int gpio_ir_recv_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct gpio_rc_dev *gpio_dev;
	struct rc_dev *rcdev;
	u32 period = 0;
	int rc;

	if (!np)
		return -ENODEV;

	gpio_dev = devm_kzalloc(dev, sizeof(*gpio_dev), GFP_KERNEL);
	if (!gpio_dev)
		return -ENOMEM;

	gpio_dev->gpiod = devm_gpiod_get(dev, NULL, GPIOD_IN);
	if (IS_ERR(gpio_dev->gpiod))
		return dev_err_probe(dev, PTR_ERR(gpio_dev->gpiod),
				     "error getting gpio\n");
	gpio_dev->irq = gpiod_to_irq(gpio_dev->gpiod);
	if (gpio_dev->irq < 0)
		return gpio_dev->irq;

	rcdev = devm_rc_allocate_device(dev, RC_DRIVER_IR_RAW);
	if (!rcdev)
		return -ENOMEM;

	rcdev->priv = gpio_dev;
	rcdev->device_name = GPIO_IR_DEVICE_NAME;
	rcdev->input_phys = GPIO_IR_DEVICE_NAME "/input0";
	rcdev->input_id.bustype = BUS_HOST;
	rcdev->input_id.vendor = 0x0001;
	rcdev->input_id.product = 0x0001;
	rcdev->input_id.version = 0x0100;
	rcdev->dev.parent = dev;
	rcdev->driver_name = KBUILD_MODNAME;
	rcdev->min_timeout = 1;
	rcdev->timeout = IR_DEFAULT_TIMEOUT;
	rcdev->max_timeout = 10 * IR_DEFAULT_TIMEOUT;
	rcdev->allowed_protocols = RC_PROTO_BIT_ALL_IR_DECODER;
	rcdev->map_name = of_get_property(np, "linux,rc-map-name", NULL);
	if (!rcdev->map_name)
		rcdev->map_name = RC_MAP_EMPTY;

	gpio_dev->rcdev = rcdev;
	if (of_property_read_bool(np, "wakeup-source"))
		device_init_wakeup(dev, true);

	rc = devm_rc_register_device(dev, rcdev);
	if (rc < 0) {
		dev_err(dev, "failed to register rc device (%d)\n", rc);
		return rc;
	}

	of_property_read_u32(np, "linux,autosuspend-period", &period);
	if (period) {
		gpio_dev->pmdev = dev;
		pm_runtime_set_autosuspend_delay(dev, period);
		pm_runtime_use_autosuspend(dev);
		pm_runtime_set_suspended(dev);
		pm_runtime_enable(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, gpio_dev);

<<<<<<< HEAD
	rc = request_any_context_irq(gpio_to_irq(pdata->gpio_nr),
				gpio_ir_recv_irq,
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
					"gpio-ir-recv-irq", gpio_dev);
	if (rc < 0)
		goto err_request_irq;

	if (gpio_dev->can_wakeup) {
		pm_qos_add_request(&gpio_dev->pm_qos_req,
					PM_QOS_CPU_DMA_LATENCY,
					PM_QOS_DEFAULT_VALUE);
		device_init_wakeup(&pdev->dev, pdata->can_wakeup);
		setup_timer(&gpio_dev->gpio_ir_timer, gpio_ir_timer,
						(unsigned long)gpio_dev);
	}

	return 0;

err_request_irq:
	platform_set_drvdata(pdev, NULL);
	rc_unregister_device(rcdev);
err_register_rc_device:
err_gpio_direction_input:
	gpio_free(pdata->gpio_nr);
err_gpio_request:
	rc_free_device(rcdev);
	rcdev = NULL;
err_allocate_device:
	kfree(gpio_dev);
	return rc;
}

static int __devexit gpio_ir_recv_remove(struct platform_device *pdev)
{
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	if (gpio_dev->can_wakeup) {
		del_timer_sync(&gpio_dev->gpio_ir_timer);
		pm_qos_remove_request(&gpio_dev->pm_qos_req);
	}
	free_irq(gpio_to_irq(gpio_dev->gpio_nr), gpio_dev);
	platform_set_drvdata(pdev, NULL);
	rc_unregister_device(gpio_dev->rcdev);
	gpio_free(gpio_dev->gpio_nr);
	rc_free_device(gpio_dev->rcdev);
	kfree(gpio_dev);
	return 0;
=======
	return devm_request_irq(dev, gpio_dev->irq, gpio_ir_recv_irq,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"gpio-ir-recv-irq", gpio_dev);
}

static void gpio_ir_recv_remove(struct platform_device *pdev)
{
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);
	struct device *pmdev = gpio_dev->pmdev;

	if (pmdev) {
		pm_runtime_get_sync(pmdev);
		cpu_latency_qos_remove_request(&gpio_dev->qos);

		pm_runtime_disable(pmdev);
		pm_runtime_put_noidle(pmdev);
		pm_runtime_set_suspended(pmdev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_PM
static int gpio_ir_recv_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	if (device_may_wakeup(dev))
		enable_irq_wake(gpio_to_irq(gpio_dev->gpio_nr));
	else
		disable_irq(gpio_to_irq(gpio_dev->gpio_nr));
=======
	struct gpio_rc_dev *gpio_dev = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(gpio_dev->irq);
	else
		disable_irq(gpio_dev->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int gpio_ir_recv_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct gpio_rc_dev *gpio_dev = platform_get_drvdata(pdev);

	if (device_may_wakeup(dev))
		disable_irq_wake(gpio_to_irq(gpio_dev->gpio_nr));
	else
		enable_irq(gpio_to_irq(gpio_dev->gpio_nr));
=======
	struct gpio_rc_dev *gpio_dev = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(gpio_dev->irq);
	else
		enable_irq(gpio_dev->irq);

	return 0;
}

static int gpio_ir_recv_runtime_suspend(struct device *dev)
{
	struct gpio_rc_dev *gpio_dev = dev_get_drvdata(dev);

	cpu_latency_qos_remove_request(&gpio_dev->qos);

	return 0;
}

static int gpio_ir_recv_runtime_resume(struct device *dev)
{
	struct gpio_rc_dev *gpio_dev = dev_get_drvdata(dev);

	cpu_latency_qos_add_request(&gpio_dev->qos, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct dev_pm_ops gpio_ir_recv_pm_ops = {
	.suspend        = gpio_ir_recv_suspend,
	.resume         = gpio_ir_recv_resume,
<<<<<<< HEAD
};
#endif

static struct platform_driver gpio_ir_recv_driver = {
	.probe  = gpio_ir_recv_probe,
	.remove = __devexit_p(gpio_ir_recv_remove),
	.driver = {
		.name   = GPIO_IR_DRIVER_NAME,
		.owner  = THIS_MODULE,
=======
	.runtime_suspend = gpio_ir_recv_runtime_suspend,
	.runtime_resume  = gpio_ir_recv_runtime_resume,
};
#endif

static const struct of_device_id gpio_ir_recv_of_match[] = {
	{ .compatible = "gpio-ir-receiver", },
	{ },
};
MODULE_DEVICE_TABLE(of, gpio_ir_recv_of_match);

static struct platform_driver gpio_ir_recv_driver = {
	.probe  = gpio_ir_recv_probe,
	.remove_new = gpio_ir_recv_remove,
	.driver = {
		.name   = KBUILD_MODNAME,
		.of_match_table = gpio_ir_recv_of_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PM
		.pm	= &gpio_ir_recv_pm_ops,
#endif
	},
};
<<<<<<< HEAD

static int __init gpio_ir_recv_init(void)
{
	return platform_driver_register(&gpio_ir_recv_driver);
}
module_init(gpio_ir_recv_init);

static void __exit gpio_ir_recv_exit(void)
{
	platform_driver_unregister(&gpio_ir_recv_driver);
}
module_exit(gpio_ir_recv_exit);
=======
module_platform_driver(gpio_ir_recv_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("GPIO IR Receiver driver");
MODULE_LICENSE("GPL v2");
