<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor capability mediation functions
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

#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/gfp.h>
<<<<<<< HEAD

#include "include/apparmor.h"
#include "include/capability.h"
#include "include/context.h"
=======
#include <linux/security.h>

#include "include/apparmor.h"
#include "include/capability.h"
#include "include/cred.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "include/policy.h"
#include "include/audit.h"

/*
 * Table of capability names: we generate it from capabilities.h.
 */
#include "capability_names.h"

<<<<<<< HEAD
=======
struct aa_sfs_entry aa_sfs_entry_caps[] = {
	AA_SFS_FILE_STRING("mask", AA_SFS_CAPS_MASK),
	{ }
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct audit_cache {
	struct aa_profile *profile;
	kernel_cap_t caps;
};

static DEFINE_PER_CPU(struct audit_cache, audit_cache);

/**
 * audit_cb - call back for capability components of audit struct
<<<<<<< HEAD
 * @ab - audit buffer   (NOT NULL)
 * @va - audit struct to audit data from  (NOT NULL)
=======
 * @ab: audit buffer   (NOT NULL)
 * @va: audit struct to audit data from  (NOT NULL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void audit_cb(struct audit_buffer *ab, void *va)
{
	struct common_audit_data *sa = va;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	audit_log_format(ab, " capname=");
	audit_log_untrustedstring(ab, capability_names[sa->u.cap]);
}

/**
 * audit_caps - audit a capability
<<<<<<< HEAD
 * @profile: profile confining task (NOT NULL)
 * @task: task capability test was performed against (NOT NULL)
=======
 * @ad: audit data
 * @profile: profile being tested for confinement (NOT NULL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @cap: capability tested
 * @error: error code returned by test
 *
 * Do auditing of capability and handle, audit/complain/kill modes switching
 * and duplicate message elimination.
 *
<<<<<<< HEAD
 * Returns: 0 or sa->error on success,  error code on failure
 */
static int audit_caps(struct aa_profile *profile, struct task_struct *task,
		      int cap, int error)
{
	struct audit_cache *ent;
	int type = AUDIT_APPARMOR_AUTO;
	struct common_audit_data sa;
	struct apparmor_audit_data aad = {0,};
	COMMON_AUDIT_DATA_INIT(&sa, CAP);
	sa.aad = &aad;
	sa.tsk = task;
	sa.u.cap = cap;
	sa.aad->op = OP_CAPABLE;
	sa.aad->error = error;
=======
 * Returns: 0 or ad->error on success,  error code on failure
 */
static int audit_caps(struct apparmor_audit_data *ad, struct aa_profile *profile,
		      int cap, int error)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct audit_cache *ent;
	int type = AUDIT_APPARMOR_AUTO;

	ad->error = error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (likely(!error)) {
		/* test if auditing is being forced */
		if (likely((AUDIT_MODE(profile) != AUDIT_ALL) &&
<<<<<<< HEAD
			   !cap_raised(profile->caps.audit, cap)))
			return 0;
		type = AUDIT_APPARMOR_AUDIT;
	} else if (KILL_MODE(profile) ||
		   cap_raised(profile->caps.kill, cap)) {
		type = AUDIT_APPARMOR_KILL;
	} else if (cap_raised(profile->caps.quiet, cap) &&
=======
			   !cap_raised(rules->caps.audit, cap)))
			return 0;
		type = AUDIT_APPARMOR_AUDIT;
	} else if (KILL_MODE(profile) ||
		   cap_raised(rules->caps.kill, cap)) {
		type = AUDIT_APPARMOR_KILL;
	} else if (cap_raised(rules->caps.quiet, cap) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   AUDIT_MODE(profile) != AUDIT_NOQUIET &&
		   AUDIT_MODE(profile) != AUDIT_ALL) {
		/* quiet auditing */
		return error;
	}

	/* Do simple duplicate message elimination */
	ent = &get_cpu_var(audit_cache);
	if (profile == ent->profile && cap_raised(ent->caps, cap)) {
		put_cpu_var(audit_cache);
		if (COMPLAIN_MODE(profile))
			return complain_error(error);
		return error;
	} else {
		aa_put_profile(ent->profile);
		ent->profile = aa_get_profile(profile);
		cap_raise(ent->caps, cap);
	}
	put_cpu_var(audit_cache);

<<<<<<< HEAD
	return aa_audit(type, profile, GFP_ATOMIC, &sa, audit_cb);
=======
	return aa_audit(type, profile, ad, audit_cb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * profile_capable - test if profile allows use of capability @cap
 * @profile: profile being enforced    (NOT NULL, NOT unconfined)
 * @cap: capability to test if allowed
<<<<<<< HEAD
 *
 * Returns: 0 if allowed else -EPERM
 */
static int profile_capable(struct aa_profile *profile, int cap)
{
	return cap_raised(profile->caps.allow, cap) ? 0 : -EPERM;
=======
 * @opts: CAP_OPT_NOAUDIT bit determines whether audit record is generated
 * @ad: audit data (MAY BE NULL indicating no auditing)
 *
 * Returns: 0 if allowed else -EPERM
 */
static int profile_capable(struct aa_profile *profile, int cap,
			   unsigned int opts, struct apparmor_audit_data *ad)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	int error;

	if (cap_raised(rules->caps.allow, cap) &&
	    !cap_raised(rules->caps.denied, cap))
		error = 0;
	else
		error = -EPERM;

	if (opts & CAP_OPT_NOAUDIT) {
		if (!COMPLAIN_MODE(profile))
			return error;
		/* audit the cap request in complain mode but note that it
		 * should be optional.
		 */
		ad->info = "optional: no audit";
	}

	return audit_caps(ad, profile, cap, error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * aa_capable - test permission to use capability
<<<<<<< HEAD
 * @task: task doing capability test against (NOT NULL)
 * @profile: profile confining @task (NOT NULL)
 * @cap: capability to be tested
 * @audit: whether an audit record should be generated
=======
 * @subj_cred: cred we are testing capability against
 * @label: label being tested for capability (NOT NULL)
 * @cap: capability to be tested
 * @opts: CAP_OPT_NOAUDIT bit determines whether audit record is generated
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Look up capability in profile capability set.
 *
 * Returns: 0 on success, or else an error code.
 */
<<<<<<< HEAD
int aa_capable(struct task_struct *task, struct aa_profile *profile, int cap,
	       int audit)
{
	int error = profile_capable(profile, cap);

	if (!audit) {
		if (COMPLAIN_MODE(profile))
			return complain_error(error);
		return error;
	}

	return audit_caps(profile, task, cap, error);
=======
int aa_capable(const struct cred *subj_cred, struct aa_label *label,
	       int cap, unsigned int opts)
{
	struct aa_profile *profile;
	int error = 0;
	DEFINE_AUDIT_DATA(ad, LSM_AUDIT_DATA_CAP, AA_CLASS_CAP, OP_CAPABLE);

	ad.subj_cred = subj_cred;
	ad.common.u.cap = cap;
	error = fn_for_each_confined(label, profile,
			profile_capable(profile, cap, opts, &ad));

	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
