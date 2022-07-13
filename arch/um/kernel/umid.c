<<<<<<< HEAD
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <asm/errno.h>
#include "init.h"
#include "kern.h"
#include "os.h"

/* Changed by set_umid_arg */
static int umid_inited = 0;
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#include <asm/errno.h>
#include <init.h>
#include <kern.h>
#include <os.h>

/* Changed by set_umid_arg */
static int umid_inited;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init set_umid_arg(char *name, int *add)
{
	int err;

	if (umid_inited) {
<<<<<<< HEAD
		printf("umid already set\n");
=======
		os_warn("umid already set\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	*add = 0;
	err = set_umid(name);
	if (err == -EEXIST)
<<<<<<< HEAD
		printf("umid '%s' already in use\n", name);
=======
		os_warn("umid '%s' already in use\n", name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (!err)
		umid_inited = 1;

	return 0;
}

__uml_setup("umid=", set_umid_arg,
"umid=<name>\n"
"    This is used to assign a unique identity to this UML machine and\n"
"    is used for naming the pid file and management console socket.\n\n"
);

