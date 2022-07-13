<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/stddef.h>
#include <linux/sched.h>
#include <linux/elf.h>
#include <linux/crypto.h>
<<<<<<< HEAD
#include <asm/mman.h>

#define DEFINE(sym, val) \
	asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define STR(x) #x
#define DEFINE_STR(sym, val) asm volatile("\n->" #sym " " STR(val) " " #val: : )

#define BLANK() asm volatile("\n->" : : )

#define OFFSET(sym, str, mem) \
	DEFINE(sym, offsetof(struct str, mem));

=======
#include <linux/kbuild.h>
#include <asm/mman.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void foo(void)
{
#include <common-offsets.h>
}
