<<<<<<< HEAD
/*
 * atmel platform data
 *
 * GPL v2 Only
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * atmel platform data
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ATMEL_H__
#define __ATMEL_H__

<<<<<<< HEAD
#include <linux/mtd/nand.h>

 /* NAND / SmartMedia */
struct atmel_nand_data {
	int		enable_pin;		/* chip enable */
	int		det_pin;		/* card detect */
	int		rdy_pin;		/* ready/busy */
	u8		rdy_pin_active_low;	/* rdy_pin value is inverted */
	u8		ale;			/* address line number connected to ALE */
	u8		cle;			/* address line number connected to CLE */
	u8		bus_width_16;		/* buswidth is 16 bit */
	u8		ecc_mode;		/* ecc mode */
	u8		on_flash_bbt;		/* bbt on flash */
	struct mtd_partition *parts;
	unsigned int	num_parts;
};
=======
/* FIXME: this needs a better location, but gets stuff building again */
#ifdef CONFIG_ATMEL_PM
extern int at91_suspend_entering_slow_clock(void);
#else
static inline int at91_suspend_entering_slow_clock(void)
{
	return 0;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ATMEL_H__ */
