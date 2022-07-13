<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Interface to booleans in the security server. This is exported
 * for the selinuxfs.
 *
 * Author: Karl MacMillan <kmacmillan@tresys.com>
 *
 * Copyright (C) 2003 - 2004 Tresys Technology, LLC
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or modify
 *  	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _SELINUX_CONDITIONAL_H_
#define _SELINUX_CONDITIONAL_H_

<<<<<<< HEAD
int security_get_bools(int *len, char ***names, int **values);

int security_set_bools(int len, int *values);

int security_get_bool_value(int bool);
=======
#include "security.h"

int security_get_bools(struct selinux_policy *policy, u32 *len, char ***names,
		       int **values);

int security_set_bools(u32 len, int *values);

int security_get_bool_value(u32 index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
