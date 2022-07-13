<<<<<<< HEAD
/*
 * Copyright (C) 2000,2001,2004 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/clocksource.h>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2000,2001,2004 Broadcom Corporation
 */
#include <linux/clocksource.h>
#include <linux/sched_clock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/time.h>

#include <asm/sibyte/bcm1480_regs.h>
#include <asm/sibyte/sb1250_regs.h>
#include <asm/sibyte/bcm1480_int.h>
#include <asm/sibyte/bcm1480_scd.h>

#include <asm/sibyte/sb1250.h>

<<<<<<< HEAD
static cycle_t bcm1480_hpt_read(struct clocksource *cs)
{
	return (cycle_t) __raw_readq(IOADDR(A_SCD_ZBBUS_CYCLE_COUNT));
=======
static u64 bcm1480_hpt_read(struct clocksource *cs)
{
	return (u64) __raw_readq(IOADDR(A_SCD_ZBBUS_CYCLE_COUNT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct clocksource bcm1480_clocksource = {
	.name	= "zbbus-cycles",
<<<<<<< HEAD
	.rating	= 200,
=======
	.rating = 200,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read	= bcm1480_hpt_read,
	.mask	= CLOCKSOURCE_MASK(64),
	.flags	= CLOCK_SOURCE_IS_CONTINUOUS,
};

<<<<<<< HEAD
=======
static u64 notrace sb1480_read_sched_clock(void)
{
	return __raw_readq(IOADDR(A_SCD_ZBBUS_CYCLE_COUNT));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __init sb1480_clocksource_init(void)
{
	struct clocksource *cs = &bcm1480_clocksource;
	unsigned int plldiv;
	unsigned long zbbus;

	plldiv = G_BCM1480_SYS_PLL_DIV(__raw_readq(IOADDR(A_SCD_SYSTEM_CFG)));
	zbbus = ((plldiv >> 1) * 50000000) + ((plldiv & 1) * 25000000);
	clocksource_register_hz(cs, zbbus);
<<<<<<< HEAD
=======

	sched_clock_register(sb1480_read_sched_clock, 64, zbbus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
