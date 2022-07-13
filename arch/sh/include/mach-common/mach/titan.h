<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Platform definitions for Titan
 */
#ifndef _ASM_SH_TITAN_H
#define _ASM_SH_TITAN_H

<<<<<<< HEAD
=======
#include <linux/sh_intc.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __IO_PREFIX titan
#include <asm/io_generic.h>

/* IRQ assignments */
<<<<<<< HEAD
#define TITAN_IRQ_WAN		2	/* eth0 (WAN) */
#define TITAN_IRQ_LAN		5	/* eth1 (LAN) */
#define TITAN_IRQ_MPCIA		8	/* mPCI A */
#define TITAN_IRQ_MPCIB		11	/* mPCI B */
#define TITAN_IRQ_USB		11	/* USB */
=======
#define TITAN_IRQ_WAN		evt2irq(0x240)	/* eth0 (WAN) */
#define TITAN_IRQ_LAN		evt2irq(0x2a0)	/* eth1 (LAN) */
#define TITAN_IRQ_MPCIA		evt2irq(0x300)	/* mPCI A */
#define TITAN_IRQ_MPCIB		evt2irq(0x360)	/* mPCI B */
#define TITAN_IRQ_USB		evt2irq(0x360)	/* USB */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_SH_TITAN_H */
