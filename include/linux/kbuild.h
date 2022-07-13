<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __LINUX_KBUILD_H
#define __LINUX_KBUILD_H

#define DEFINE(sym, val) \
<<<<<<< HEAD
        asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define BLANK() asm volatile("\n->" : : )
=======
	asm volatile("\n.ascii \"->" #sym " %0 " #val "\"" : : "i" (val))

#define BLANK() asm volatile("\n.ascii \"->\"" : : )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define OFFSET(sym, str, mem) \
	DEFINE(sym, offsetof(struct str, mem))

#define COMMENT(x) \
<<<<<<< HEAD
	asm volatile("\n->#" x)
=======
	asm volatile("\n.ascii \"->#" x "\"")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
