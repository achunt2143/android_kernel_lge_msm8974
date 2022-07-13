<<<<<<< HEAD
#ifndef ___ASM_SPARC_UACCESS_H
#define ___ASM_SPARC_UACCESS_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_UACCESS_H
#define ___ASM_SPARC_UACCESS_H

#include <asm/extable.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(__sparc__) && defined(__arch64__)
#include <asm/uaccess_64.h>
#else
#include <asm/uaccess_32.h>
#endif
<<<<<<< HEAD
=======

long strncpy_from_user(char *dest, const char __user *src, long count);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
