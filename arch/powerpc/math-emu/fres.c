<<<<<<< HEAD
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int
fres(void *frD, void *frB)
{
#ifdef DEBUG
	printk("%s: %p %p\n", __func__, frD, frB);
#endif
	return -ENOSYS;
}
