<<<<<<< HEAD
/* 
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include "linux/module.h"
=======
// SPDX-License-Identifier: GPL-2.0
/* 
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#include <linux/module.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void mcount(void);
EXPORT_SYMBOL(mcount);
