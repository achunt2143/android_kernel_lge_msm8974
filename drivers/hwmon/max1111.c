<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * max1111.c - +2.7V, Low-Power, Multichannel, Serial 8-bit ADCs
 *
 * Based on arch/arm/mach-pxa/corgi_ssp.c
 *
 * Copyright (C) 2004-2005 Richard Purdie
 *
 * Copyright (C) 2008 Marvell International Ltd.
 *	Eric Miao <eric.miao@marvell.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>

<<<<<<< HEAD
=======
enum chips { max1110, max1111, max1112, max1113 };

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MAX1111_TX_BUF_SIZE	1
#define MAX1111_RX_BUF_SIZE	2

/* MAX1111 Commands */
#define MAX1111_CTRL_PD0      (1u << 0)
#define MAX1111_CTRL_PD1      (1u << 1)
#define MAX1111_CTRL_SGL      (1u << 2)
#define MAX1111_CTRL_UNI      (1u << 3)
<<<<<<< HEAD
=======
#define MAX1110_CTRL_SEL_SH   (4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MAX1111_CTRL_SEL_SH   (5)	/* NOTE: bit 4 is ignored */
#define MAX1111_CTRL_STR      (1u << 7)

struct max1111_data {
	struct spi_device	*spi;
	struct device		*hwmon_dev;
	struct spi_message	msg;
	struct spi_transfer	xfer[2];
	uint8_t tx_buf[MAX1111_TX_BUF_SIZE];
	uint8_t rx_buf[MAX1111_RX_BUF_SIZE];
	struct mutex		drvdata_lock;
	/* protect msg, xfer and buffers from multiple access */
<<<<<<< HEAD
=======
	int			sel_sh;
	int			lsb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int max1111_read(struct device *dev, int channel)
{
	struct max1111_data *data = dev_get_drvdata(dev);
	uint8_t v1, v2;
	int err;

	/* writing to drvdata struct is not thread safe, wait on mutex */
	mutex_lock(&data->drvdata_lock);

<<<<<<< HEAD
	data->tx_buf[0] = (channel << MAX1111_CTRL_SEL_SH) |
=======
	data->tx_buf[0] = (channel << data->sel_sh) |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		MAX1111_CTRL_PD0 | MAX1111_CTRL_PD1 |
		MAX1111_CTRL_SGL | MAX1111_CTRL_UNI | MAX1111_CTRL_STR;

	err = spi_sync(data->spi, &data->msg);
	if (err < 0) {
		dev_err(dev, "spi_sync failed with %d\n", err);
		mutex_unlock(&data->drvdata_lock);
		return err;
	}

	v1 = data->rx_buf[0];
	v2 = data->rx_buf[1];

	mutex_unlock(&data->drvdata_lock);

	if ((v1 & 0xc0) || (v2 & 0x3f))
		return -EINVAL;

	return (v1 << 2) | (v2 >> 6);
}

#ifdef CONFIG_SHARPSL_PM
static struct max1111_data *the_max1111;

<<<<<<< HEAD
int max1111_read_channel(int channel)
{
=======
int max1111_read_channel(int channel);
int max1111_read_channel(int channel)
{
	if (!the_max1111 || !the_max1111->spi)
		return -ENODEV;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return max1111_read(&the_max1111->spi->dev, channel);
}
EXPORT_SYMBOL(max1111_read_channel);
#endif

/*
 * NOTE: SPI devices do not have a default 'name' attribute, which is
 * likely to be used by hwmon applications to distinguish between
 * different devices, explicitly add a name attribute here.
 */
<<<<<<< HEAD
static ssize_t show_name(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "max1111\n");
=======
static ssize_t name_show(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", to_spi_device(dev)->modalias);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t show_adc(struct device *dev,
			struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
=======
	struct max1111_data *data = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int channel = to_sensor_dev_attr(attr)->index;
	int ret;

	ret = max1111_read(dev, channel);
	if (ret < 0)
		return ret;

	/*
<<<<<<< HEAD
	 * assume the reference voltage to be 2.048V, with an 8-bit sample,
	 * the LSB weight is 8mV
	 */
	return sprintf(buf, "%d\n", ret * 8);
=======
	 * Assume the reference voltage to be 2.048V or 4.096V, with an 8-bit
	 * sample. The LSB weight is 8mV or 16mV depending on the chip type.
	 */
	return sprintf(buf, "%d\n", ret * data->lsb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define MAX1111_ADC_ATTR(_id)		\
	SENSOR_DEVICE_ATTR(in##_id##_input, S_IRUGO, show_adc, NULL, _id)

<<<<<<< HEAD
static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);
=======
static DEVICE_ATTR_RO(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static MAX1111_ADC_ATTR(0);
static MAX1111_ADC_ATTR(1);
static MAX1111_ADC_ATTR(2);
static MAX1111_ADC_ATTR(3);
<<<<<<< HEAD
=======
static MAX1111_ADC_ATTR(4);
static MAX1111_ADC_ATTR(5);
static MAX1111_ADC_ATTR(6);
static MAX1111_ADC_ATTR(7);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct attribute *max1111_attributes[] = {
	&dev_attr_name.attr,
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	NULL,
};

static const struct attribute_group max1111_attr_group = {
	.attrs	= max1111_attributes,
};

<<<<<<< HEAD
static int __devinit setup_transfer(struct max1111_data *data)
=======
static struct attribute *max1110_attributes[] = {
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in5_input.dev_attr.attr,
	&sensor_dev_attr_in6_input.dev_attr.attr,
	&sensor_dev_attr_in7_input.dev_attr.attr,
	NULL,
};

static const struct attribute_group max1110_attr_group = {
	.attrs	= max1110_attributes,
};

static int setup_transfer(struct max1111_data *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct spi_message *m;
	struct spi_transfer *x;

	m = &data->msg;
	x = &data->xfer[0];

	spi_message_init(m);

	x->tx_buf = &data->tx_buf[0];
	x->len = MAX1111_TX_BUF_SIZE;
	spi_message_add_tail(x, m);

	x++;
	x->rx_buf = &data->rx_buf[0];
	x->len = MAX1111_RX_BUF_SIZE;
	spi_message_add_tail(x, m);

	return 0;
}

<<<<<<< HEAD
static int __devinit max1111_probe(struct spi_device *spi)
{
=======
static int max1111_probe(struct spi_device *spi)
{
	enum chips chip = spi_get_device_id(spi)->driver_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct max1111_data *data;
	int err;

	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	err = spi_setup(spi);
	if (err < 0)
		return err;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct max1111_data), GFP_KERNEL);
	if (data == NULL) {
		dev_err(&spi->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}

	err = setup_transfer(data);
	if (err)
		goto err_free_data;
=======
	data = devm_kzalloc(&spi->dev, sizeof(struct max1111_data), GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	switch (chip) {
	case max1110:
		data->lsb = 8;
		data->sel_sh = MAX1110_CTRL_SEL_SH;
		break;
	case max1111:
		data->lsb = 8;
		data->sel_sh = MAX1111_CTRL_SEL_SH;
		break;
	case max1112:
		data->lsb = 16;
		data->sel_sh = MAX1110_CTRL_SEL_SH;
		break;
	case max1113:
		data->lsb = 16;
		data->sel_sh = MAX1111_CTRL_SEL_SH;
		break;
	}
	err = setup_transfer(data);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_init(&data->drvdata_lock);

	data->spi = spi;
	spi_set_drvdata(spi, data);

	err = sysfs_create_group(&spi->dev.kobj, &max1111_attr_group);
	if (err) {
		dev_err(&spi->dev, "failed to create attribute group\n");
<<<<<<< HEAD
		goto err_free_data;
=======
		return err;
	}
	if (chip == max1110 || chip == max1112) {
		err = sysfs_create_group(&spi->dev.kobj, &max1110_attr_group);
		if (err) {
			dev_err(&spi->dev,
				"failed to create extended attribute group\n");
			goto err_remove;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	data->hwmon_dev = hwmon_device_register(&spi->dev);
	if (IS_ERR(data->hwmon_dev)) {
		dev_err(&spi->dev, "failed to create hwmon device\n");
		err = PTR_ERR(data->hwmon_dev);
		goto err_remove;
	}

#ifdef CONFIG_SHARPSL_PM
	the_max1111 = data;
#endif
	return 0;

err_remove:
<<<<<<< HEAD
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
err_free_data:
	kfree(data);
	return err;
}

static int __devexit max1111_remove(struct spi_device *spi)
{
	struct max1111_data *data = spi_get_drvdata(spi);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	mutex_destroy(&data->drvdata_lock);
	kfree(data);
	return 0;
}

static struct spi_driver max1111_driver = {
	.driver		= {
		.name	= "max1111",
		.owner	= THIS_MODULE,
	},
	.probe		= max1111_probe,
	.remove		= __devexit_p(max1111_remove),
=======
	sysfs_remove_group(&spi->dev.kobj, &max1110_attr_group);
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	return err;
}

static void max1111_remove(struct spi_device *spi)
{
	struct max1111_data *data = spi_get_drvdata(spi);

#ifdef CONFIG_SHARPSL_PM
	the_max1111 = NULL;
#endif
	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&spi->dev.kobj, &max1110_attr_group);
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	mutex_destroy(&data->drvdata_lock);
}

static const struct spi_device_id max1111_ids[] = {
	{ "max1110", max1110 },
	{ "max1111", max1111 },
	{ "max1112", max1112 },
	{ "max1113", max1113 },
	{ },
};
MODULE_DEVICE_TABLE(spi, max1111_ids);

static struct spi_driver max1111_driver = {
	.driver		= {
		.name	= "max1111",
	},
	.id_table	= max1111_ids,
	.probe		= max1111_probe,
	.remove		= max1111_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_spi_driver(max1111_driver);

MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("MAX1111 ADC Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:max1111");
=======
MODULE_DESCRIPTION("MAX1110/MAX1111/MAX1112/MAX1113 ADC Driver");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
