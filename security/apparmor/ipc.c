<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor ipc mediation
 *
 * Copyright (C) 1998-2008 Novell/SUSE
<<<<<<< HEAD
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 */

#include <linux/gfp.h>
#include <linux/ptrace.h>

#include "include/audit.h"
#include "include/capability.h"
#include "include/context.h"
#include "include/policy.h"
#include "include/ipc.h"

/* call back to audit ptrace fields */
static void audit_cb(struct audit_buffer *ab, void *va)
{
	struct common_audit_data *sa = va;
	audit_log_format(ab, " target=");
	audit_log_untrustedstring(ab, sa->aad->target);
}

/**
 * aa_audit_ptrace - do auditing for ptrace
 * @profile: profile being enforced  (NOT NULL)
 * @target: profile being traced (NOT NULL)
 * @error: error condition
 *
 * Returns: %0 or error code
 */
static int aa_audit_ptrace(struct aa_profile *profile,
			   struct aa_profile *target, int error)
{
	struct common_audit_data sa;
	struct apparmor_audit_data aad = {0,};
	COMMON_AUDIT_DATA_INIT(&sa, NONE);
	sa.aad = &aad;
	aad.op = OP_PTRACE;
	aad.target = target;
	aad.error = error;

	return aa_audit(AUDIT_APPARMOR_AUTO, profile, GFP_ATOMIC, &sa,
			audit_cb);
}

/**
 * aa_may_ptrace - test if tracer task can trace the tracee
 * @tracer_task: task who will do the tracing  (NOT NULL)
 * @tracer: profile of the task doing the tracing  (NOT NULL)
 * @tracee: task to be traced
 * @mode: whether PTRACE_MODE_READ || PTRACE_MODE_ATTACH
 *
 * Returns: %0 else error code if permission denied or error
 */
int aa_may_ptrace(struct task_struct *tracer_task, struct aa_profile *tracer,
		  struct aa_profile *tracee, unsigned int mode)
{
	/* TODO: currently only based on capability, not extended ptrace
	 *       rules,
	 *       Test mode for PTRACE_MODE_READ || PTRACE_MODE_ATTACH
	 */

	if (unconfined(tracer) || tracer == tracee)
		return 0;
	/* log this capability request */
	return aa_capable(tracer_task, tracer, CAP_SYS_PTRACE, 1);
}

/**
 * aa_ptrace - do ptrace permission check and auditing
 * @tracer: task doing the tracing (NOT NULL)
 * @tracee: task being traced (NOT NULL)
 * @mode: ptrace mode either PTRACE_MODE_READ || PTRACE_MODE_ATTACH
 *
 * Returns: %0 else error code if permission denied or error
 */
int aa_ptrace(struct task_struct *tracer, struct task_struct *tracee,
	      unsigned int mode)
{
	/*
	 * tracer can ptrace tracee when
	 * - tracer is unconfined ||
	 *   - tracer is in complain mode
	 *   - tracer has rules allowing it to trace tracee currently this is:
	 *       - confined by the same profile ||
	 *       - tracer profile has CAP_SYS_PTRACE
	 */

	struct aa_profile *tracer_p;
	/* cred released below */
	const struct cred *cred = get_task_cred(tracer);
	int error = 0;
	tracer_p = aa_cred_profile(cred);

	if (!unconfined(tracer_p)) {
		/* lcred released below */
		const struct cred *lcred = get_task_cred(tracee);
		struct aa_profile *tracee_p = aa_cred_profile(lcred);

		error = aa_may_ptrace(tracer, tracer_p, tracee_p, mode);
		error = aa_audit_ptrace(tracer_p, tracee_p, error);

		put_cred(lcred);
	}
	put_cred(cred);

	return error;
=======
 * Copyright 2009-2017 Canonical Ltd.
 */

#include <linux/gfp.h>

#include "include/audit.h"
#include "include/capability.h"
#include "include/cred.h"
#include "include/policy.h"
#include "include/ipc.h"
#include "include/sig_names.h"


static inline int map_signal_num(int sig)
{
	if (sig > SIGRTMAX)
		return SIGUNKNOWN;
	else if (sig >= SIGRTMIN)
		return sig - SIGRTMIN + SIGRT_BASE;
	else if (sig < MAXMAPPED_SIG)
		return sig_map[sig];
	return SIGUNKNOWN;
}

/**
 * audit_signal_mask - convert mask to permission string
 * @mask: permission mask to convert
 *
 * Returns: pointer to static string
 */
static const char *audit_signal_mask(u32 mask)
{
	if (mask & MAY_READ)
		return "receive";
	if (mask & MAY_WRITE)
		return "send";
	return "";
}

/**
 * audit_signal_cb() - call back for signal specific audit fields
 * @ab: audit_buffer  (NOT NULL)
 * @va: audit struct to audit values of  (NOT NULL)
 */
static void audit_signal_cb(struct audit_buffer *ab, void *va)
{
	struct common_audit_data *sa = va;
	struct apparmor_audit_data *ad = aad(sa);

	if (ad->request & AA_SIGNAL_PERM_MASK) {
		audit_log_format(ab, " requested_mask=\"%s\"",
				 audit_signal_mask(ad->request));
		if (ad->denied & AA_SIGNAL_PERM_MASK) {
			audit_log_format(ab, " denied_mask=\"%s\"",
					 audit_signal_mask(ad->denied));
		}
	}
	if (ad->signal == SIGUNKNOWN)
		audit_log_format(ab, "signal=unknown(%d)",
				 ad->unmappedsig);
	else if (ad->signal < MAXMAPPED_SIGNAME)
		audit_log_format(ab, " signal=%s", sig_names[ad->signal]);
	else
		audit_log_format(ab, " signal=rtmin+%d",
				 ad->signal - SIGRT_BASE);
	audit_log_format(ab, " peer=");
	aa_label_xaudit(ab, labels_ns(ad->subj_label), ad->peer,
			FLAGS_NONE, GFP_ATOMIC);
}

static int profile_signal_perm(const struct cred *cred,
			       struct aa_profile *profile,
			       struct aa_label *peer, u32 request,
			       struct apparmor_audit_data *ad)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_perms perms;
	aa_state_t state;

	if (profile_unconfined(profile) ||
	    !ANY_RULE_MEDIATES(&profile->rules, AA_CLASS_SIGNAL))
		return 0;

	ad->subj_cred = cred;
	ad->peer = peer;
	/* TODO: secondary cache check <profile, profile, perm> */
	state = aa_dfa_next(rules->policy->dfa,
			    rules->policy->start[AA_CLASS_SIGNAL],
			    ad->signal);
	aa_label_match(profile, rules, peer, state, false, request, &perms);
	aa_apply_modes_to_perms(profile, &perms);
	return aa_check_perms(profile, &perms, request, ad, audit_signal_cb);
}

int aa_may_signal(const struct cred *subj_cred, struct aa_label *sender,
		  const struct cred *target_cred, struct aa_label *target,
		  int sig)
{
	struct aa_profile *profile;
	DEFINE_AUDIT_DATA(ad, LSM_AUDIT_DATA_NONE, AA_CLASS_SIGNAL, OP_SIGNAL);

	ad.signal = map_signal_num(sig);
	ad.unmappedsig = sig;
	return xcheck_labels(sender, target, profile,
			     profile_signal_perm(subj_cred, profile, target,
						 MAY_WRITE, &ad),
			     profile_signal_perm(target_cred, profile, sender,
						 MAY_READ, &ad));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
