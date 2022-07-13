<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor security domain transition function definitions.
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

#include <linux/binfmts.h>
#include <linux/types.h>

<<<<<<< HEAD
#ifndef __AA_DOMAIN_H
#define __AA_DOMAIN_H

struct aa_domain {
	int size;
	char **table;
};

int apparmor_bprm_set_creds(struct linux_binprm *bprm);
int apparmor_bprm_secureexec(struct linux_binprm *bprm);
void apparmor_bprm_committing_creds(struct linux_binprm *bprm);
void apparmor_bprm_committed_creds(struct linux_binprm *bprm);

void aa_free_domain_entries(struct aa_domain *domain);
int aa_change_hat(const char *hats[], int count, u64 token, bool permtest);
int aa_change_profile(const char *ns_name, const char *name, bool onexec,
		      bool permtest);
=======
#include "label.h"

#ifndef __AA_DOMAIN_H
#define __AA_DOMAIN_H

#define AA_CHANGE_NOFLAGS 0
#define AA_CHANGE_TEST 1
#define AA_CHANGE_CHILD 2
#define AA_CHANGE_ONEXEC  4
#define AA_CHANGE_STACK 8

struct aa_label *x_table_lookup(struct aa_profile *profile, u32 xindex,
				const char **name);

int apparmor_bprm_creds_for_exec(struct linux_binprm *bprm);

int aa_change_hat(const char *hats[], int count, u64 token, int flags);
int aa_change_profile(const char *fqname, int flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __AA_DOMAIN_H */
