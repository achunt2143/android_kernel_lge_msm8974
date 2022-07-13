<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Common Definitions for Janz MODULbus devices
 *
 * Copyright (c) 2010 Ira W. Snyder <iws@ovro.caltech.edu>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef JANZ_H
#define JANZ_H

struct janz_platform_data {
	/* MODULbus Module Number */
	unsigned int modno;
};

/* PLX bridge chip onboard registers */
struct janz_cmodio_onboard_regs {
	u8 unused1;

	/*
	 * Read access: interrupt status
	 * Write access: interrupt disable
	 */
	u8 int_disable;
	u8 unused2;

	/*
	 * Read access: MODULbus number (hex switch)
	 * Write access: interrupt enable
	 */
	u8 int_enable;
	u8 unused3;

	/* write-only */
	u8 reset_assert;
	u8 unused4;

	/* write-only */
	u8 reset_deassert;
	u8 unused5;

	/* read-write access to serial EEPROM */
	u8 eep;
	u8 unused6;

	/* write-only access to EEPROM chip select */
	u8 enid;
};

#endif /* JANZ_H */
