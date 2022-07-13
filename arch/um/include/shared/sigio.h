<<<<<<< HEAD
/* 
 * Copyright (C) 2002 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2002 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __SIGIO_H__
#define __SIGIO_H__

extern int write_sigio_irq(int fd);
<<<<<<< HEAD
extern int register_sigio_fd(int fd);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void sigio_lock(void);
extern void sigio_unlock(void);

#endif
