<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor /proc/<pid>/attr/ interface function definitions.
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __AA_PROCATTR_H
#define __AA_PROCATTR_H

<<<<<<< HEAD
#define AA_DO_TEST 1
#define AA_ONEXEC  1

int aa_getprocattr(struct aa_profile *profile, char **string);
int aa_setprocattr_changehat(char *args, size_t size, int test);
int aa_setprocattr_changeprofile(char *fqname, bool onexec, int test);
int aa_setprocattr_permipc(char *fqname);
=======
int aa_getprocattr(struct aa_label *label, char **string, bool newline);
int aa_setprocattr_changehat(char *args, size_t size, int flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __AA_PROCATTR_H */
