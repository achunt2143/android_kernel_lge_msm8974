<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor capability mediation definitions.
 *
 * Copyright (C) 1998-2008 Novell/SUSE
<<<<<<< HEAD
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
 * Copyright 2009-2013 Canonical Ltd.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __AA_CAPABILITY_H
#define __AA_CAPABILITY_H

#include <linux/sched.h>

<<<<<<< HEAD
struct aa_profile;
=======
#include "apparmorfs.h"

struct aa_label;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* aa_caps - confinement data for capabilities
 * @allowed: capabilities mask
 * @audit: caps that are to be audited
<<<<<<< HEAD
=======
 * @denied: caps that are explicitly denied
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @quiet: caps that should not be audited
 * @kill: caps that when requested will result in the task being killed
 * @extended: caps that are subject finer grained mediation
 */
struct aa_caps {
	kernel_cap_t allow;
	kernel_cap_t audit;
<<<<<<< HEAD
=======
	kernel_cap_t denied;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kernel_cap_t quiet;
	kernel_cap_t kill;
	kernel_cap_t extended;
};

<<<<<<< HEAD
int aa_capable(struct task_struct *task, struct aa_profile *profile, int cap,
	       int audit);
=======
extern struct aa_sfs_entry aa_sfs_entry_caps[];

int aa_capable(const struct cred *subj_cred, struct aa_label *label,
	       int cap, unsigned int opts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void aa_free_cap_rules(struct aa_caps *caps)
{
	/* NOP */
}

#endif /* __AA_CAPBILITY_H */
