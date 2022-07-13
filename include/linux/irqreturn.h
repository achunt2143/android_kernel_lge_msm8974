<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_IRQRETURN_H
#define _LINUX_IRQRETURN_H

/**
<<<<<<< HEAD
 * enum irqreturn
 * @IRQ_NONE		interrupt was not from this device
 * @IRQ_HANDLED		interrupt was handled by this device
 * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
=======
 * enum irqreturn - irqreturn type values
 * @IRQ_NONE:		interrupt was not from this device or was not handled
 * @IRQ_HANDLED:	interrupt was handled by this device
 * @IRQ_WAKE_THREAD:	handler requests to wake the handler thread
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};

typedef enum irqreturn irqreturn_t;
<<<<<<< HEAD
#define IRQ_RETVAL(x)	((x) != IRQ_NONE)
=======
#define IRQ_RETVAL(x)	((x) ? IRQ_HANDLED : IRQ_NONE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
