<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor auditing functions
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

#include <linux/audit.h>
#include <linux/socket.h>

#include "include/apparmor.h"
#include "include/audit.h"
#include "include/policy.h"
<<<<<<< HEAD

const char *const op_table[] = {
	"null",

	"sysctl",
	"capable",

	"unlink",
	"mkdir",
	"rmdir",
	"mknod",
	"truncate",
	"link",
	"symlink",
	"rename_src",
	"rename_dest",
	"chmod",
	"chown",
	"getattr",
	"open",

	"file_perm",
	"file_lock",
	"file_mmap",
	"file_mprotect",

	"create",
	"post_create",
	"bind",
	"connect",
	"listen",
	"accept",
	"sendmsg",
	"recvmsg",
	"getsockname",
	"getpeername",
	"getsockopt",
	"setsockopt",
	"socket_shutdown",

	"ptrace",

	"exec",
	"change_hat",
	"change_profile",
	"change_onexec",

	"setprocattr",
	"setrlimit",

	"profile_replace",
	"profile_load",
	"profile_remove"
};
=======
#include "include/policy_ns.h"
#include "include/secid.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const char *const audit_mode_names[] = {
	"normal",
	"quiet_denied",
	"quiet",
	"noquiet",
	"all"
};

static const char *const aa_audit_type[] = {
	"AUDIT",
	"ALLOWED",
	"DENIED",
	"HINT",
	"STATUS",
	"ERROR",
<<<<<<< HEAD
	"KILLED"
	"AUTO"
};

=======
	"KILLED",
	"AUTO"
};

static const char *const aa_class_names[] = {
	"none",
	"unknown",
	"file",
	"cap",
	"net",
	"rlimits",
	"domain",
	"mount",
	"unknown",
	"ptrace",
	"signal",
	"xmatch",
	"unknown",
	"unknown",
	"net",
	"unknown",
	"label",
	"posix_mqueue",
	"io_uring",
	"module",
	"lsm",
	"namespace",
	"io_uring",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"X",
	"dbus",
};


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Currently AppArmor auditing is fed straight into the audit framework.
 *
 * TODO:
 * netlink interface for complain mode
 * user auditing, - send user auditing to netlink interface
 * system control of whether user audit messages go to system log
 */

/**
<<<<<<< HEAD
 * audit_base - core AppArmor function.
 * @ab: audit buffer to fill (NOT NULL)
 * @ca: audit structure containing data to audit (NOT NULL)
 *
 * Record common AppArmor audit data from @sa
 */
static void audit_pre(struct audit_buffer *ab, void *ca)
{
	struct common_audit_data *sa = ca;
	struct task_struct *tsk = sa->tsk ? sa->tsk : current;

	if (aa_g_audit_header) {
		audit_log_format(ab, "apparmor=");
		audit_log_string(ab, aa_audit_type[sa->aad->type]);
	}

	if (sa->aad->op) {
		audit_log_format(ab, " operation=");
		audit_log_string(ab, op_table[sa->aad->op]);
	}

	if (sa->aad->info) {
		audit_log_format(ab, " info=");
		audit_log_string(ab, sa->aad->info);
		if (sa->aad->error)
			audit_log_format(ab, " error=%d", sa->aad->error);
	}

	if (sa->aad->profile) {
		struct aa_profile *profile = sa->aad->profile;
		pid_t pid;
		rcu_read_lock();
		pid = rcu_dereference(tsk->real_parent)->pid;
		rcu_read_unlock();
		audit_log_format(ab, " parent=%d", pid);
		if (profile->ns != root_ns) {
			audit_log_format(ab, " namespace=");
			audit_log_untrustedstring(ab, profile->ns->base.hname);
		}
		audit_log_format(ab, " profile=");
		audit_log_untrustedstring(ab, profile->base.hname);
	}

	if (sa->aad->name) {
		audit_log_format(ab, " name=");
		audit_log_untrustedstring(ab, sa->aad->name);
=======
 * audit_pre() - core AppArmor function.
 * @ab: audit buffer to fill (NOT NULL)
 * @va: audit structure containing data to audit (NOT NULL)
 *
 * Record common AppArmor audit data from @va
 */
static void audit_pre(struct audit_buffer *ab, void *va)
{
	struct apparmor_audit_data *ad = aad_of_va(va);

	if (aa_g_audit_header) {
		audit_log_format(ab, "apparmor=\"%s\"",
				 aa_audit_type[ad->type]);
	}

	if (ad->op)
		audit_log_format(ab, " operation=\"%s\"", ad->op);

	if (ad->class)
		audit_log_format(ab, " class=\"%s\"",
				 ad->class <= AA_CLASS_LAST ?
				 aa_class_names[ad->class] :
				 "unknown");

	if (ad->info) {
		audit_log_format(ab, " info=\"%s\"", ad->info);
		if (ad->error)
			audit_log_format(ab, " error=%d", ad->error);
	}

	if (ad->subj_label) {
		struct aa_label *label = ad->subj_label;

		if (label_isprofile(label)) {
			struct aa_profile *profile = labels_profile(label);

			if (profile->ns != root_ns) {
				audit_log_format(ab, " namespace=");
				audit_log_untrustedstring(ab,
						       profile->ns->base.hname);
			}
			audit_log_format(ab, " profile=");
			audit_log_untrustedstring(ab, profile->base.hname);
		} else {
			audit_log_format(ab, " label=");
			aa_label_xaudit(ab, root_ns, label, FLAG_VIEW_SUBNS,
					GFP_ATOMIC);
		}
	}

	if (ad->name) {
		audit_log_format(ab, " name=");
		audit_log_untrustedstring(ab, ad->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
 * aa_audit_msg - Log a message to the audit subsystem
<<<<<<< HEAD
 * @sa: audit event structure (NOT NULL)
 * @cb: optional callback fn for type specific fields (MAYBE NULL)
 */
void aa_audit_msg(int type, struct common_audit_data *sa,
		  void (*cb) (struct audit_buffer *, void *))
{
	sa->aad->type = type;
	common_lsm_audit(sa, audit_pre, cb);
=======
 * @type: audit type for the message
 * @ad: audit event structure (NOT NULL)
 * @cb: optional callback fn for type specific fields (MAYBE NULL)
 */
void aa_audit_msg(int type, struct apparmor_audit_data *ad,
		  void (*cb) (struct audit_buffer *, void *))
{
	ad->type = type;
	common_lsm_audit(&ad->common, audit_pre, cb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * aa_audit - Log a profile based audit event to the audit subsystem
 * @type: audit type for the message
 * @profile: profile to check against (NOT NULL)
<<<<<<< HEAD
 * @gfp: allocation flags to use
 * @sa: audit event (NOT NULL)
=======
 * @ad: audit event (NOT NULL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @cb: optional callback fn for type specific fields (MAYBE NULL)
 *
 * Handle default message switching based off of audit mode flags
 *
 * Returns: error on failure
 */
<<<<<<< HEAD
int aa_audit(int type, struct aa_profile *profile, gfp_t gfp,
	     struct common_audit_data *sa,
	     void (*cb) (struct audit_buffer *, void *))
{
	BUG_ON(!profile);

	if (type == AUDIT_APPARMOR_AUTO) {
		if (likely(!sa->aad->error)) {
=======
int aa_audit(int type, struct aa_profile *profile,
	     struct apparmor_audit_data *ad,
	     void (*cb) (struct audit_buffer *, void *))
{
	AA_BUG(!profile);

	if (type == AUDIT_APPARMOR_AUTO) {
		if (likely(!ad->error)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (AUDIT_MODE(profile) != AUDIT_ALL)
				return 0;
			type = AUDIT_APPARMOR_AUDIT;
		} else if (COMPLAIN_MODE(profile))
			type = AUDIT_APPARMOR_ALLOWED;
		else
			type = AUDIT_APPARMOR_DENIED;
	}
	if (AUDIT_MODE(profile) == AUDIT_QUIET ||
	    (type == AUDIT_APPARMOR_DENIED &&
<<<<<<< HEAD
	     AUDIT_MODE(profile) == AUDIT_QUIET))
		return sa->aad->error;
=======
	     AUDIT_MODE(profile) == AUDIT_QUIET_DENIED))
		return ad->error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (KILL_MODE(profile) && type == AUDIT_APPARMOR_DENIED)
		type = AUDIT_APPARMOR_KILL;

<<<<<<< HEAD
	if (!unconfined(profile))
		sa->aad->profile = profile;

	aa_audit_msg(type, sa, cb);

	if (sa->aad->type == AUDIT_APPARMOR_KILL)
		(void)send_sig_info(SIGKILL, NULL, sa->tsk ? sa->tsk : current);

	if (sa->aad->type == AUDIT_APPARMOR_ALLOWED)
		return complain_error(sa->aad->error);

	return sa->aad->error;
=======
	ad->subj_label = &profile->label;

	aa_audit_msg(type, ad, cb);

	if (ad->type == AUDIT_APPARMOR_KILL)
		(void)send_sig_info(SIGKILL, NULL,
			ad->common.type == LSM_AUDIT_DATA_TASK &&
			ad->common.u.tsk ? ad->common.u.tsk : current);

	if (ad->type == AUDIT_APPARMOR_ALLOWED)
		return complain_error(ad->error);

	return ad->error;
}

struct aa_audit_rule {
	struct aa_label *label;
};

void aa_audit_rule_free(void *vrule)
{
	struct aa_audit_rule *rule = vrule;

	if (rule) {
		if (!IS_ERR(rule->label))
			aa_put_label(rule->label);
		kfree(rule);
	}
}

int aa_audit_rule_init(u32 field, u32 op, char *rulestr, void **vrule)
{
	struct aa_audit_rule *rule;

	switch (field) {
	case AUDIT_SUBJ_ROLE:
		if (op != Audit_equal && op != Audit_not_equal)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}

	rule = kzalloc(sizeof(struct aa_audit_rule), GFP_KERNEL);

	if (!rule)
		return -ENOMEM;

	/* Currently rules are treated as coming from the root ns */
	rule->label = aa_label_parse(&root_ns->unconfined->label, rulestr,
				     GFP_KERNEL, true, false);
	if (IS_ERR(rule->label)) {
		int err = PTR_ERR(rule->label);
		aa_audit_rule_free(rule);
		return err;
	}

	*vrule = rule;
	return 0;
}

int aa_audit_rule_known(struct audit_krule *rule)
{
	int i;

	for (i = 0; i < rule->field_count; i++) {
		struct audit_field *f = &rule->fields[i];

		switch (f->type) {
		case AUDIT_SUBJ_ROLE:
			return 1;
		}
	}

	return 0;
}

int aa_audit_rule_match(u32 sid, u32 field, u32 op, void *vrule)
{
	struct aa_audit_rule *rule = vrule;
	struct aa_label *label;
	int found = 0;

	label = aa_secid_to_label(sid);

	if (!label)
		return -ENOENT;

	if (aa_label_is_subset(label, rule->label))
		found = 1;

	switch (field) {
	case AUDIT_SUBJ_ROLE:
		switch (op) {
		case Audit_equal:
			return found;
		case Audit_not_equal:
			return !found;
		}
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
