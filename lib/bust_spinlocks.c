<<<<<<< HEAD
/*
 * lib/bust_spinlocks.c
 *
 * Provides a minimal bust_spinlocks for architectures which don't have one of their own.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * lib/bust_spinlocks.c
 *
 * Provides a minimal bust_spinlocks for architectures which don't
 * have one of their own.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * bust_spinlocks() clears any spinlocks which would prevent oops, die(), BUG()
 * and panic() information from reaching the user.
 */

#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/printk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <linux/tty.h>
#include <linux/wait.h>
#include <linux/vt_kern.h>
#include <linux/console.h>

<<<<<<< HEAD

void __attribute__((weak)) bust_spinlocks(int yes)
=======
void bust_spinlocks(int yes)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (yes) {
		++oops_in_progress;
	} else {
<<<<<<< HEAD
#ifdef CONFIG_VT
		unblank_screen();
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		console_unblank();
		if (--oops_in_progress == 0)
			wake_up_klogd();
	}
}
<<<<<<< HEAD


=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
