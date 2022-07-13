<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 * Licensed under the GPL
 */

#include <linux/interrupt.h>
#include "irq_kern.h"
#include "os.h"
#include "sigio.h"
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 */

#include <linux/interrupt.h>
#include <irq_kern.h>
#include <os.h>
#include <sigio.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Protected by sigio_lock() called from write_sigio_workaround */
static int sigio_irq_fd = -1;

static irqreturn_t sigio_interrupt(int irq, void *data)
{
	char c;

	os_read_file(sigio_irq_fd, &c, sizeof(c));
<<<<<<< HEAD
	reactivate_fd(sigio_irq_fd, SIGIO_WRITE_IRQ);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return IRQ_HANDLED;
}

int write_sigio_irq(int fd)
{
	int err;

	err = um_request_irq(SIGIO_WRITE_IRQ, fd, IRQ_READ, sigio_interrupt,
<<<<<<< HEAD
			     IRQF_SAMPLE_RANDOM, "write sigio",
			     NULL);
	if (err) {
=======
			     0, "write sigio", NULL);
	if (err < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		printk(KERN_ERR "write_sigio_irq : um_request_irq failed, "
		       "err = %d\n", err);
		return -1;
	}
	sigio_irq_fd = fd;
	return 0;
}

/* These are called from os-Linux/sigio.c to protect its pollfds arrays. */
<<<<<<< HEAD
static DEFINE_SPINLOCK(sigio_spinlock);

void sigio_lock(void)
{
	spin_lock(&sigio_spinlock);
=======
static DEFINE_MUTEX(sigio_mutex);

void sigio_lock(void)
{
	mutex_lock(&sigio_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void sigio_unlock(void)
{
<<<<<<< HEAD
	spin_unlock(&sigio_spinlock);
=======
	mutex_unlock(&sigio_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
