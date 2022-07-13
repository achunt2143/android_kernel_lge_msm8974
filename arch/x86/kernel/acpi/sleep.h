<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Variables and functions used by the code in sleep.c
 */

<<<<<<< HEAD
#include <asm/trampoline.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/linkage.h>

extern unsigned long saved_video_mode;
extern long saved_magic;

extern int wakeup_pmode_return;

extern u8 wake_sleep_flags;
<<<<<<< HEAD
extern asmlinkage void acpi_enter_s3(void);

extern unsigned long acpi_copy_wakeup_routine(unsigned long);
extern void wakeup_long64(void);

extern void do_suspend_lowlevel(void);
=======

extern void wakeup_long64(void);

extern void do_suspend_lowlevel(void);

extern int x86_acpi_suspend_lowlevel(void);

asmlinkage acpi_status x86_acpi_enter_sleep_state(u8 state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
