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
#include <math-emu/double.h>

int
fctiw(u32 *frD, void *frB)
{
	FP_DECL_D(B);
	FP_DECL_EX;
	unsigned int r;

	FP_UNPACK_DP(B, frB);
	FP_TO_INT_D(r, B, 32, 1);
	frD[1] = r;

#ifdef DEBUG
	printk("%s: D %p, B %p: ", __func__, frD, frB);
	dump_double(frD);
	printk("\n");
#endif

	return 0;
}
