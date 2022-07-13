<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * parport-to-butterfly adapter
 *
 * Copyright (C) 2005 David Brownell
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/parport.h>

#include <linux/sched.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/spi/flash.h>

#include <linux/mtd/partitions.h>

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This uses SPI to talk with an "AVR Butterfly", which is a $US20 card
 * with a battery powered AVR microcontroller and lots of goodies.  You
 * can use GCC to develop firmware for this.
 *
<<<<<<< HEAD
 * See Documentation/spi/butterfly for information about how to build
 * and use this custom parallel port cable.
 */


=======
 * See Documentation/spi/butterfly.rst for information about how to build
 * and use this custom parallel port cable.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* DATA output bits (pins 2..9 == D0..D7) */
#define	butterfly_nreset (1 << 1)		/* pin 3 */

#define	spi_sck_bit	(1 << 0)		/* pin 2 */
#define	spi_mosi_bit	(1 << 7)		/* pin 9 */

#define	vcc_bits	((1 << 6) | (1 << 5))	/* pins 7, 8 */

/* STATUS input bits */
#define	spi_miso_bit	PARPORT_STATUS_BUSY	/* pin 11 */

/* CONTROL output bits */
#define	spi_cs_bit	PARPORT_CONTROL_SELECT	/* pin 17 */

<<<<<<< HEAD


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct butterfly *spidev_to_pp(struct spi_device *spi)
{
	return spi->controller_data;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct butterfly {
	/* REVISIT ... for now, this must be first */
	struct spi_bitbang	bitbang;

	struct parport		*port;
	struct pardevice	*pd;

	u8			lastbyte;

	struct spi_device	*dataflash;
	struct spi_device	*butterfly;
	struct spi_board_info	info[2];

};

/*----------------------------------------------------------------------*/

static inline void
setsck(struct spi_device *spi, int is_on)
{
	struct butterfly	*pp = spidev_to_pp(spi);
	u8			bit, byte = pp->lastbyte;

	bit = spi_sck_bit;

	if (is_on)
		byte |= bit;
	else
		byte &= ~bit;
	parport_write_data(pp->port, byte);
	pp->lastbyte = byte;
}

static inline void
setmosi(struct spi_device *spi, int is_on)
{
	struct butterfly	*pp = spidev_to_pp(spi);
	u8			bit, byte = pp->lastbyte;

	bit = spi_mosi_bit;

	if (is_on)
		byte |= bit;
	else
		byte &= ~bit;
	parport_write_data(pp->port, byte);
	pp->lastbyte = byte;
}

static inline int getmiso(struct spi_device *spi)
{
	struct butterfly	*pp = spidev_to_pp(spi);
	int			value;
	u8			bit;

	bit = spi_miso_bit;

	/* only STATUS_BUSY is NOT negated */
	value = !(parport_read_status(pp->port) & bit);
	return (bit == PARPORT_STATUS_BUSY) ? value : !value;
}

static void butterfly_chipselect(struct spi_device *spi, int value)
{
	struct butterfly	*pp = spidev_to_pp(spi);

	/* set default clock polarity */
	if (value != BITBANG_CS_INACTIVE)
		setsck(spi, spi->mode & SPI_CPOL);

	/* here, value == "activate or not";
	 * most PARPORT_CONTROL_* bits are negated, so we must
	 * morph it to value == "bit value to write in control register"
	 */
	if (spi_cs_bit == PARPORT_CONTROL_INIT)
		value = !value;

	parport_frob_control(pp->port, spi_cs_bit, value ? spi_cs_bit : 0);
}

<<<<<<< HEAD

/* we only needed to implement one mode here, and choose SPI_MODE_0 */

#define	spidelay(X)	do{}while(0)
//#define	spidelay	ndelay
=======
/* we only needed to implement one mode here, and choose SPI_MODE_0 */

#define spidelay(X)	do { } while (0)
/* #define spidelay	ndelay */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "spi-bitbang-txrx.h"

static u32
<<<<<<< HEAD
butterfly_txrx_word_mode0(struct spi_device *spi,
		unsigned nsecs,
		u32 word, u8 bits)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 0, 0, word, bits);
=======
butterfly_txrx_word_mode0(struct spi_device *spi, unsigned nsecs, u32 word,
			  u8 bits, unsigned flags)
{
	return bitbang_txrx_be_cpha0(spi, nsecs, 0, flags, word, bits);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*----------------------------------------------------------------------*/

/* override default partitioning with cmdlinepart */
static struct mtd_partition partitions[] = { {
	/* JFFS2 wants partitions of 4*N blocks for this device,
	 * so sectors 0 and 1 can't be partitions by themselves.
	 */

	/* sector 0 = 8 pages * 264 bytes/page (1 block)
	 * sector 1 = 248 pages * 264 bytes/page
	 */
<<<<<<< HEAD
	.name		= "bookkeeping",	// 66 KB
	.offset		= 0,
	.size		= (8 + 248) * 264,
//	.mask_flags	= MTD_WRITEABLE,
=======
	.name		= "bookkeeping",	/* 66 KB */
	.offset		= 0,
	.size		= (8 + 248) * 264,
	/* .mask_flags	= MTD_WRITEABLE, */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}, {
	/* sector 2 = 256 pages * 264 bytes/page
	 * sectors 3-5 = 512 pages * 264 bytes/page
	 */
<<<<<<< HEAD
	.name		= "filesystem",		// 462 KB
=======
	.name		= "filesystem",		/* 462 KB */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.offset		= MTDPART_OFS_APPEND,
	.size		= MTDPART_SIZ_FULL,
} };

static struct flash_platform_data flash = {
	.name		= "butterflash",
	.parts		= partitions,
	.nr_parts	= ARRAY_SIZE(partitions),
};

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* REVISIT remove this ugly global and its "only one" limitation */
static struct butterfly *butterfly;

static void butterfly_attach(struct parport *p)
{
	struct pardevice	*pd;
	int			status;
	struct butterfly	*pp;
<<<<<<< HEAD
	struct spi_master	*master;
	struct device		*dev = p->physport->dev;
=======
	struct spi_controller	*host;
	struct device		*dev = p->physport->dev;
	struct pardev_cb	butterfly_cb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (butterfly || !dev)
		return;

	/* REVISIT:  this just _assumes_ a butterfly is there ... no probe,
	 * and no way to be selective about what it binds to.
	 */

<<<<<<< HEAD
	master = spi_alloc_master(dev, sizeof *pp);
	if (!master) {
		status = -ENOMEM;
		goto done;
	}
	pp = spi_master_get_devdata(master);
=======
	host = spi_alloc_host(dev, sizeof(*pp));
	if (!host) {
		status = -ENOMEM;
		goto done;
	}
	pp = spi_controller_get_devdata(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * SPI and bitbang hookup
	 *
	 * use default setup(), cleanup(), and transfer() methods; and
	 * only bother implementing mode 0.  Start it later.
	 */
<<<<<<< HEAD
	master->bus_num = 42;
	master->num_chipselect = 2;

	pp->bitbang.master = spi_master_get(master);
=======
	host->bus_num = 42;
	host->num_chipselect = 2;

	pp->bitbang.ctlr = host;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pp->bitbang.chipselect = butterfly_chipselect;
	pp->bitbang.txrx_word[SPI_MODE_0] = butterfly_txrx_word_mode0;

	/*
	 * parport hookup
	 */
	pp->port = p;
<<<<<<< HEAD
	pd = parport_register_device(p, "spi_butterfly",
			NULL, NULL, NULL,
			0 /* FLAGS */, pp);
=======
	memset(&butterfly_cb, 0, sizeof(butterfly_cb));
	butterfly_cb.private = pp;
	pd = parport_register_dev_model(p, "spi_butterfly", &butterfly_cb, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pd) {
		status = -ENOMEM;
		goto clean0;
	}
	pp->pd = pd;

	status = parport_claim(pd);
	if (status < 0)
		goto clean1;

	/*
	 * Butterfly reset, powerup, run firmware
	 */
	pr_debug("%s: powerup/reset Butterfly\n", p->name);

	/* nCS for dataflash (this bit is inverted on output) */
	parport_frob_control(pp->port, spi_cs_bit, 0);

	/* stabilize power with chip in reset (nRESET), and
	 * spi_sck_bit clear (CPOL=0)
	 */
	pp->lastbyte |= vcc_bits;
	parport_write_data(pp->port, pp->lastbyte);
	msleep(5);

	/* take it out of reset; assume long reset delay */
	pp->lastbyte |= butterfly_nreset;
	parport_write_data(pp->port, pp->lastbyte);
	msleep(100);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Start SPI ... for now, hide that we're two physical busses.
	 */
	status = spi_bitbang_start(&pp->bitbang);
	if (status < 0)
		goto clean2;

	/* Bus 1 lets us talk to at45db041b (firmware disables AVR SPI), AVR
	 * (firmware resets at45, acts as spi slave) or neither (we ignore
	 * both, AVR uses AT45).  Here we expect firmware for the first option.
	 */

	pp->info[0].max_speed_hz = 15 * 1000 * 1000;
	strcpy(pp->info[0].modalias, "mtd_dataflash");
	pp->info[0].platform_data = &flash;
	pp->info[0].chip_select = 1;
	pp->info[0].controller_data = pp;
<<<<<<< HEAD
	pp->dataflash = spi_new_device(pp->bitbang.master, &pp->info[0]);
	if (pp->dataflash)
		pr_debug("%s: dataflash at %s\n", p->name,
				dev_name(&pp->dataflash->dev));

	// dev_info(_what?_, ...)
=======
	pp->dataflash = spi_new_device(pp->bitbang.ctlr, &pp->info[0]);
	if (pp->dataflash)
		pr_debug("%s: dataflash at %s\n", p->name,
			 dev_name(&pp->dataflash->dev));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("%s: AVR Butterfly\n", p->name);
	butterfly = pp;
	return;

clean2:
	/* turn off VCC */
	parport_write_data(pp->port, 0);

	parport_release(pp->pd);
clean1:
	parport_unregister_device(pd);
clean0:
<<<<<<< HEAD
	(void) spi_master_put(pp->bitbang.master);
=======
	spi_controller_put(host);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
done:
	pr_debug("%s: butterfly probe, fail %d\n", p->name, status);
}

static void butterfly_detach(struct parport *p)
{
	struct butterfly	*pp;
<<<<<<< HEAD
	int			status;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* FIXME this global is ugly ... but, how to quickly get from
	 * the parport to the "struct butterfly" associated with it?
	 * "old school" driver-internal device lists?
	 */
	if (!butterfly || butterfly->port != p)
		return;
	pp = butterfly;
	butterfly = NULL;

	/* stop() unregisters child devices too */
<<<<<<< HEAD
	status = spi_bitbang_stop(&pp->bitbang);
=======
	spi_bitbang_stop(&pp->bitbang);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* turn off VCC */
	parport_write_data(pp->port, 0);
	msleep(10);

	parport_release(pp->pd);
	parport_unregister_device(pp->pd);

<<<<<<< HEAD
	(void) spi_master_put(pp->bitbang.master);
=======
	spi_controller_put(pp->bitbang.ctlr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct parport_driver butterfly_driver = {
	.name =		"spi_butterfly",
<<<<<<< HEAD
	.attach =	butterfly_attach,
	.detach =	butterfly_detach,
};


static int __init butterfly_init(void)
{
	return parport_register_driver(&butterfly_driver);
}
device_initcall(butterfly_init);

static void __exit butterfly_exit(void)
{
	parport_unregister_driver(&butterfly_driver);
}
module_exit(butterfly_exit);
=======
	.match_port =	butterfly_attach,
	.detach =	butterfly_detach,
	.devmodel = true,
};
module_parport_driver(butterfly_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_DESCRIPTION("Parport Adapter driver for AVR Butterfly");
MODULE_LICENSE("GPL");
