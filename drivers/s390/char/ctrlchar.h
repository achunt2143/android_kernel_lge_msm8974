<<<<<<< HEAD
/*
 *  drivers/s390/char/ctrlchar.c
 *  Unified handling of special chars.
 *
 *    Copyright (C) 2001 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Unified handling of special chars.
 *
 *    Copyright IBM Corp. 2001
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Fritz Elfert <felfert@millenux.com> <elfert@de.ibm.com>
 *
 */

#include <linux/tty.h>
<<<<<<< HEAD
=======
#include <linux/sysrq.h>
#include <linux/workqueue.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern unsigned int
ctrlchar_handle(const unsigned char *buf, int len, struct tty_struct *tty);


#define CTRLCHAR_NONE  (1 << 8)
#define CTRLCHAR_CTRL  (2 << 8)
#define CTRLCHAR_SYSRQ (3 << 8)

#define CTRLCHAR_MASK (~0xffu)
<<<<<<< HEAD
=======


#ifdef CONFIG_MAGIC_SYSRQ
struct sysrq_work {
	int key;
	struct work_struct work;
};

void schedule_sysrq_work(struct sysrq_work *sw);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
