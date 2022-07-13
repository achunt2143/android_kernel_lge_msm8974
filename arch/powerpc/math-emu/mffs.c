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

#include <asm/sfp-machine.h>
#include <math-emu/soft-fp.h>

int
mffs(u32 *frD)
{
	frD[1] = __FPU_FPSCR;

#ifdef DEBUG
	printk("%s: frD %p: %08x.%08x\n", __func__, frD, frD[0], frD[1]);
#endif

	return 0;
}
