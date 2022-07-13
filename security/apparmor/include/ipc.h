<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor ipc mediation function definitions.
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
 * Copyright 2009-2017 Canonical Ltd.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __AA_IPC_H
#define __AA_IPC_H

#include <linux/sched.h>

<<<<<<< HEAD
struct aa_profile;

int aa_may_ptrace(struct task_struct *tracer_task, struct aa_profile *tracer,
		  struct aa_profile *tracee, unsigned int mode);

int aa_ptrace(struct task_struct *tracer, struct task_struct *tracee,
	      unsigned int mode);
=======
int aa_may_signal(const struct cred *subj_cred, struct aa_label *sender,
		  const struct cred *target_cred, struct aa_label *target,
		  int sig);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __AA_IPC_H */
