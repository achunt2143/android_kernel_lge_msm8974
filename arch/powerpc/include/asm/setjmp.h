<<<<<<< HEAD
/*
 * Copyright © 2008 Michael Neuling IBM Corporation
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright © 2008 Michael Neuling IBM Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _ASM_POWERPC_SETJMP_H
#define _ASM_POWERPC_SETJMP_H

#define JMP_BUF_LEN    23

<<<<<<< HEAD
extern long setjmp(long *);
extern void longjmp(long *, long);
=======
typedef long jmp_buf[JMP_BUF_LEN];

extern int setjmp(jmp_buf env) __attribute__((returns_twice));
extern void longjmp(jmp_buf env, int val) __attribute__((noreturn));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_POWERPC_SETJMP_H */
