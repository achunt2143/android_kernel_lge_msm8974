<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Driver for the Analog Devices digital potentiometers (SPI bus)
 *
 * Copyright (C) 2010-2011 Michael Hennerich, Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/spi/spi.h>
#include <linux/module.h>

#include "ad525x_dpot.h"

/* SPI bus functions */
static int write8(void *client, u8 val)
{
	u8 data = val;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return spi_write(client, &data, 1);
}

static int write16(void *client, u8 reg, u8 val)
{
	u8 data[2] = {reg, val};
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return spi_write(client, data, 2);
}

static int write24(void *client, u8 reg, u16 val)
{
	u8 data[3] = {reg, val >> 8, val};
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return spi_write(client, data, 3);
}

static int read8(void *client)
{
	int ret;
	u8 data;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = spi_read(client, &data, 1);
	if (ret < 0)
		return ret;

	return data;
}

static int read16(void *client, u8 reg)
{
	int ret;
	u8 buf_rx[2];

	write16(client, reg, 0);
	ret = spi_read(client, buf_rx, 2);
	if (ret < 0)
		return ret;

	return (buf_rx[0] << 8) |  buf_rx[1];
}

static int read24(void *client, u8 reg)
{
	int ret;
	u8 buf_rx[3];

	write24(client, reg, 0);
	ret = spi_read(client, buf_rx, 3);
	if (ret < 0)
		return ret;

	return (buf_rx[1] << 8) |  buf_rx[2];
}

static const struct ad_dpot_bus_ops bops = {
	.read_d8	= read8,
	.read_r8d8	= read16,
	.read_r8d16	= read24,
	.write_d8	= write8,
	.write_r8d8	= write16,
	.write_r8d16	= write24,
};
<<<<<<< HEAD
static int __devinit ad_dpot_spi_probe(struct spi_device *spi)
=======
static int ad_dpot_spi_probe(struct spi_device *spi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ad_dpot_bus_data bdata = {
		.client = spi,
		.bops = &bops,
	};

	return ad_dpot_probe(&spi->dev, &bdata,
			     spi_get_device_id(spi)->driver_data,
			     spi_get_device_id(spi)->name);
}

<<<<<<< HEAD
static int __devexit ad_dpot_spi_remove(struct spi_device *spi)
{
	return ad_dpot_remove(&spi->dev);
=======
static void ad_dpot_spi_remove(struct spi_device *spi)
{
	ad_dpot_remove(&spi->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct spi_device_id ad_dpot_spi_id[] = {
	{"ad5160", AD5160_ID},
	{"ad5161", AD5161_ID},
	{"ad5162", AD5162_ID},
	{"ad5165", AD5165_ID},
	{"ad5200", AD5200_ID},
	{"ad5201", AD5201_ID},
	{"ad5203", AD5203_ID},
	{"ad5204", AD5204_ID},
	{"ad5206", AD5206_ID},
	{"ad5207", AD5207_ID},
	{"ad5231", AD5231_ID},
	{"ad5232", AD5232_ID},
	{"ad5233", AD5233_ID},
	{"ad5235", AD5235_ID},
	{"ad5260", AD5260_ID},
	{"ad5262", AD5262_ID},
	{"ad5263", AD5263_ID},
	{"ad5290", AD5290_ID},
	{"ad5291", AD5291_ID},
	{"ad5292", AD5292_ID},
	{"ad5293", AD5293_ID},
	{"ad7376", AD7376_ID},
	{"ad8400", AD8400_ID},
	{"ad8402", AD8402_ID},
	{"ad8403", AD8403_ID},
	{"adn2850", ADN2850_ID},
	{"ad5270", AD5270_ID},
	{"ad5271", AD5271_ID},
	{}
};
MODULE_DEVICE_TABLE(spi, ad_dpot_spi_id);

static struct spi_driver ad_dpot_spi_driver = {
	.driver = {
		.name	= "ad_dpot",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe		= ad_dpot_spi_probe,
	.remove		= __devexit_p(ad_dpot_spi_remove),
=======
	},
	.probe		= ad_dpot_spi_probe,
	.remove		= ad_dpot_spi_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= ad_dpot_spi_id,
};

module_spi_driver(ad_dpot_spi_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
=======
MODULE_AUTHOR("Michael Hennerich <michael.hennerich@analog.com>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION("digital potentiometer SPI bus driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:ad_dpot");
