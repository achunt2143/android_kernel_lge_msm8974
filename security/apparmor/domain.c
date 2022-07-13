<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor policy attachment and domain transitions
 *
 * Copyright (C) 2002-2008 Novell/SUSE
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

#include <linux/errno.h>
#include <linux/fdtable.h>
<<<<<<< HEAD
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/syscalls.h>
#include <linux/tracehook.h>
#include <linux/personality.h>

#include "include/audit.h"
#include "include/apparmorfs.h"
#include "include/context.h"
=======
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/syscalls.h>
#include <linux/personality.h>
#include <linux/xattr.h>
#include <linux/user_namespace.h>

#include "include/audit.h"
#include "include/apparmorfs.h"
#include "include/cred.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "include/domain.h"
#include "include/file.h"
#include "include/ipc.h"
#include "include/match.h"
#include "include/path.h"
#include "include/policy.h"
<<<<<<< HEAD

/**
 * aa_free_domain_entries - free entries in a domain table
 * @domain: the domain table to free  (MAYBE NULL)
 */
void aa_free_domain_entries(struct aa_domain *domain)
{
	int i;
	if (domain) {
		if (!domain->table)
			return;

		for (i = 0; i < domain->size; i++)
			kzfree(domain->table[i]);
		kzfree(domain->table);
		domain->table = NULL;
	}
}

/**
 * may_change_ptraced_domain - check if can change profile on ptraced task
 * @task: task we want to change profile of   (NOT NULL)
 * @to_profile: profile to change to  (NOT NULL)
 *
 * Check if the task is ptraced and if so if the tracing task is allowed
=======
#include "include/policy_ns.h"

/**
 * may_change_ptraced_domain - check if can change profile on ptraced task
 * @to_cred: cred of task changing domain
 * @to_label: profile to change to  (NOT NULL)
 * @info: message if there is an error
 *
 * Check if current is ptraced and if so if the tracing task is allowed
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * to trace the new domain
 *
 * Returns: %0 or error if change not allowed
 */
<<<<<<< HEAD
static int may_change_ptraced_domain(struct task_struct *task,
				     struct aa_profile *to_profile)
{
	struct task_struct *tracer;
	const struct cred *cred = NULL;
	struct aa_profile *tracerp = NULL;
	int error = 0;

	rcu_read_lock();
	tracer = ptrace_parent(task);
	if (tracer) {
		/* released below */
		cred = get_task_cred(tracer);
		tracerp = aa_cred_profile(cred);
	}

	/* not ptraced */
	if (!tracer || unconfined(tracerp))
		goto out;

	error = aa_may_ptrace(tracer, tracerp, to_profile, PTRACE_MODE_ATTACH);

out:
	rcu_read_unlock();
	if (cred)
		put_cred(cred);

	return error;
}

/**
 * change_profile_perms - find permissions for change_profile
 * @profile: the current profile  (NOT NULL)
 * @ns: the namespace being switched to  (NOT NULL)
 * @name: the name of the profile to change to  (NOT NULL)
 * @request: requested perms
 * @start: state to start matching in
 *
 * Returns: permission set
 */
static struct file_perms change_profile_perms(struct aa_profile *profile,
					      struct aa_namespace *ns,
					      const char *name, u32 request,
					      unsigned int start)
{
	struct file_perms perms;
	struct path_cond cond = { };
	unsigned int state;

	if (unconfined(profile)) {
		perms.allow = AA_MAY_CHANGE_PROFILE | AA_MAY_ONEXEC;
		perms.audit = perms.quiet = perms.kill = 0;
		return perms;
	} else if (!profile->file.dfa) {
		return nullperms;
	} else if ((ns == profile->ns)) {
		/* try matching against rules with out namespace prepended */
		aa_str_perms(profile->file.dfa, start, name, &cond, &perms);
		if (COMBINED_PERM_MASK(perms) & request)
			return perms;
	}

	/* try matching with namespace name and then profile */
	state = aa_dfa_match(profile->file.dfa, start, ns->base.name);
	state = aa_dfa_match_len(profile->file.dfa, state, ":", 1);
	aa_str_perms(profile->file.dfa, state, name, &cond, &perms);

	return perms;
}

/**
 * __attach_match_ - find an attachment match
 * @name - to match against  (NOT NULL)
 * @head - profile list to walk  (NOT NULL)
=======
static int may_change_ptraced_domain(const struct cred *to_cred,
				     struct aa_label *to_label,
				     const char **info)
{
	struct task_struct *tracer;
	struct aa_label *tracerl = NULL;
	const struct cred *tracer_cred = NULL;

	int error = 0;

	rcu_read_lock();
	tracer = ptrace_parent(current);
	if (tracer) {
		/* released below */
		tracerl = aa_get_task_label(tracer);
		tracer_cred = get_task_cred(tracer);
	}
	/* not ptraced */
	if (!tracer || unconfined(tracerl))
		goto out;

	error = aa_may_ptrace(tracer_cred, tracerl, to_cred, to_label,
			      PTRACE_MODE_ATTACH);

out:
	rcu_read_unlock();
	aa_put_label(tracerl);
	put_cred(tracer_cred);

	if (error)
		*info = "ptrace prevents transition";
	return error;
}

/**** TODO: dedup to aa_label_match - needs perm and dfa, merging
 * specifically this is an exact copy of aa_label_match except
 * aa_compute_perms is replaced with aa_compute_fperms
 * and policy->dfa with file->dfa
 ****/
/* match a profile and its associated ns component if needed
 * Assumes visibility test has already been done.
 * If a subns profile is not to be matched should be prescreened with
 * visibility test.
 */
static inline aa_state_t match_component(struct aa_profile *profile,
					 struct aa_profile *tp,
					 bool stack, aa_state_t state)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	const char *ns_name;

	if (stack)
		state = aa_dfa_match(rules->file->dfa, state, "&");
	if (profile->ns == tp->ns)
		return aa_dfa_match(rules->file->dfa, state, tp->base.hname);

	/* try matching with namespace name and then profile */
	ns_name = aa_ns_name(profile->ns, tp->ns, true);
	state = aa_dfa_match_len(rules->file->dfa, state, ":", 1);
	state = aa_dfa_match(rules->file->dfa, state, ns_name);
	state = aa_dfa_match_len(rules->file->dfa, state, ":", 1);
	return aa_dfa_match(rules->file->dfa, state, tp->base.hname);
}

/**
 * label_compound_match - find perms for full compound label
 * @profile: profile to find perms for
 * @label: label to check access permissions for
 * @stack: whether this is a stacking request
 * @state: state to start match in
 * @subns: whether to do permission checks on components in a subns
 * @request: permissions to request
 * @perms: perms struct to set
 *
 * Returns: 0 on success else ERROR
 *
 * For the label A//&B//&C this does the perm match for A//&B//&C
 * @perms should be preinitialized with allperms OR a previous permission
 *        check to be stacked.
 */
static int label_compound_match(struct aa_profile *profile,
				struct aa_label *label, bool stack,
				aa_state_t state, bool subns, u32 request,
				struct aa_perms *perms)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_profile *tp;
	struct label_it i;
	struct path_cond cond = { };

	/* find first subcomponent that is visible */
	label_for_each(i, label, tp) {
		if (!aa_ns_visible(profile->ns, tp->ns, subns))
			continue;
		state = match_component(profile, tp, stack, state);
		if (!state)
			goto fail;
		goto next;
	}

	/* no component visible */
	*perms = allperms;
	return 0;

next:
	label_for_each_cont(i, label, tp) {
		if (!aa_ns_visible(profile->ns, tp->ns, subns))
			continue;
		state = aa_dfa_match(rules->file->dfa, state, "//&");
		state = match_component(profile, tp, false, state);
		if (!state)
			goto fail;
	}
	*perms = *(aa_lookup_fperms(rules->file, state, &cond));
	aa_apply_modes_to_perms(profile, perms);
	if ((perms->allow & request) != request)
		return -EACCES;

	return 0;

fail:
	*perms = nullperms;
	return -EACCES;
}

/**
 * label_components_match - find perms for all subcomponents of a label
 * @profile: profile to find perms for
 * @label: label to check access permissions for
 * @stack: whether this is a stacking request
 * @start: state to start match in
 * @subns: whether to do permission checks on components in a subns
 * @request: permissions to request
 * @perms: an initialized perms struct to add accumulation to
 *
 * Returns: 0 on success else ERROR
 *
 * For the label A//&B//&C this does the perm match for each of A and B and C
 * @perms should be preinitialized with allperms OR a previous permission
 *        check to be stacked.
 */
static int label_components_match(struct aa_profile *profile,
				  struct aa_label *label, bool stack,
				  aa_state_t start, bool subns, u32 request,
				  struct aa_perms *perms)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_profile *tp;
	struct label_it i;
	struct aa_perms tmp;
	struct path_cond cond = { };
	aa_state_t state = 0;

	/* find first subcomponent to test */
	label_for_each(i, label, tp) {
		if (!aa_ns_visible(profile->ns, tp->ns, subns))
			continue;
		state = match_component(profile, tp, stack, start);
		if (!state)
			goto fail;
		goto next;
	}

	/* no subcomponents visible - no change in perms */
	return 0;

next:
	tmp = *(aa_lookup_fperms(rules->file, state, &cond));
	aa_apply_modes_to_perms(profile, &tmp);
	aa_perms_accum(perms, &tmp);
	label_for_each_cont(i, label, tp) {
		if (!aa_ns_visible(profile->ns, tp->ns, subns))
			continue;
		state = match_component(profile, tp, stack, start);
		if (!state)
			goto fail;
		tmp = *(aa_lookup_fperms(rules->file, state, &cond));
		aa_apply_modes_to_perms(profile, &tmp);
		aa_perms_accum(perms, &tmp);
	}

	if ((perms->allow & request) != request)
		return -EACCES;

	return 0;

fail:
	*perms = nullperms;
	return -EACCES;
}

/**
 * label_match - do a multi-component label match
 * @profile: profile to match against (NOT NULL)
 * @label: label to match (NOT NULL)
 * @stack: whether this is a stacking request
 * @state: state to start in
 * @subns: whether to match subns components
 * @request: permission request
 * @perms: Returns computed perms (NOT NULL)
 *
 * Returns: the state the match finished in, may be the none matching state
 */
static int label_match(struct aa_profile *profile, struct aa_label *label,
		       bool stack, aa_state_t state, bool subns, u32 request,
		       struct aa_perms *perms)
{
	int error;

	*perms = nullperms;
	error = label_compound_match(profile, label, stack, state, subns,
				     request, perms);
	if (!error)
		return error;

	*perms = allperms;
	return label_components_match(profile, label, stack, state, subns,
				      request, perms);
}

/******* end TODO: dedup *****/

/**
 * change_profile_perms - find permissions for change_profile
 * @profile: the current profile  (NOT NULL)
 * @target: label to transition to (NOT NULL)
 * @stack: whether this is a stacking request
 * @request: requested perms
 * @start: state to start matching in
 * @perms: Returns computed perms (NOT NULL)
 *
 *
 * Returns: permission set
 *
 * currently only matches full label A//&B//&C or individual components A, B, C
 * not arbitrary combinations. Eg. A//&B, C
 */
static int change_profile_perms(struct aa_profile *profile,
				struct aa_label *target, bool stack,
				u32 request, aa_state_t start,
				struct aa_perms *perms)
{
	if (profile_unconfined(profile)) {
		perms->allow = AA_MAY_CHANGE_PROFILE | AA_MAY_ONEXEC;
		perms->audit = perms->quiet = perms->kill = 0;
		return 0;
	}

	/* TODO: add profile in ns screening */
	return label_match(profile, target, stack, start, true, request, perms);
}

/**
 * aa_xattrs_match - check whether a file matches the xattrs defined in profile
 * @bprm: binprm struct for the process to validate
 * @profile: profile to match against (NOT NULL)
 * @state: state to start match in
 *
 * Returns: number of extended attributes that matched, or < 0 on error
 */
static int aa_xattrs_match(const struct linux_binprm *bprm,
			   struct aa_profile *profile, aa_state_t state)
{
	int i;
	struct dentry *d;
	char *value = NULL;
	struct aa_attachment *attach = &profile->attach;
	int size, value_size = 0, ret = attach->xattr_count;

	if (!bprm || !attach->xattr_count)
		return 0;
	might_sleep();

	/* transition from exec match to xattr set */
	state = aa_dfa_outofband_transition(attach->xmatch->dfa, state);
	d = bprm->file->f_path.dentry;

	for (i = 0; i < attach->xattr_count; i++) {
		size = vfs_getxattr_alloc(&nop_mnt_idmap, d, attach->xattrs[i],
					  &value, value_size, GFP_KERNEL);
		if (size >= 0) {
			u32 index, perm;

			/*
			 * Check the xattr presence before value. This ensure
			 * that not present xattr can be distinguished from a 0
			 * length value or rule that matches any value
			 */
			state = aa_dfa_null_transition(attach->xmatch->dfa,
						       state);
			/* Check xattr value */
			state = aa_dfa_match_len(attach->xmatch->dfa, state,
						 value, size);
			index = ACCEPT_TABLE(attach->xmatch->dfa)[state];
			perm = attach->xmatch->perms[index].allow;
			if (!(perm & MAY_EXEC)) {
				ret = -EINVAL;
				goto out;
			}
		}
		/* transition to next element */
		state = aa_dfa_outofband_transition(attach->xmatch->dfa, state);
		if (size < 0) {
			/*
			 * No xattr match, so verify if transition to
			 * next element was valid. IFF so the xattr
			 * was optional.
			 */
			if (!state) {
				ret = -EINVAL;
				goto out;
			}
			/* don't count missing optional xattr as matched */
			ret--;
		}
	}

out:
	kfree(value);
	return ret;
}

/**
 * find_attach - do attachment search for unconfined processes
 * @bprm: binprm structure of transitioning task
 * @ns: the current namespace  (NOT NULL)
 * @head: profile list to walk  (NOT NULL)
 * @name: to match against  (NOT NULL)
 * @info: info message if there was an error (NOT NULL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Do a linear search on the profiles in the list.  There is a matching
 * preference where an exact match is preferred over a name which uses
 * expressions to match, and matching expressions with the greatest
 * xmatch_len are preferred.
 *
 * Requires: @head not be shared or have appropriate locks held
 *
<<<<<<< HEAD
 * Returns: profile or NULL if no match found
 */
static struct aa_profile *__attach_match(const char *name,
					 struct list_head *head)
{
	int len = 0;
	struct aa_profile *profile, *candidate = NULL;

	list_for_each_entry(profile, head, base.list) {
		if (profile->flags & PFLAG_NULL)
			continue;
		if (profile->xmatch && profile->xmatch_len > len) {
			unsigned int state = aa_dfa_match(profile->xmatch,
							  DFA_START, name);
			u32 perm = dfa_user_allow(profile->xmatch, state);
			/* any accepting state means a valid match. */
			if (perm & MAY_EXEC) {
				candidate = profile;
				len = profile->xmatch_len;
			}
		} else if (!strcmp(profile->base.name, name))
			/* exact non-re match, no more searching required */
			return profile;
	}

	return candidate;
}

/**
 * find_attach - do attachment search for unconfined processes
 * @ns: the current namespace  (NOT NULL)
 * @list: list to search  (NOT NULL)
 * @name: the executable name to match against  (NOT NULL)
 *
 * Returns: profile or NULL if no match found
 */
static struct aa_profile *find_attach(struct aa_namespace *ns,
				      struct list_head *list, const char *name)
{
	struct aa_profile *profile;

	read_lock(&ns->lock);
	profile = aa_get_profile(__attach_match(name, list));
	read_unlock(&ns->lock);

	return profile;
}

/**
 * separate_fqname - separate the namespace and profile names
 * @fqname: the fqname name to split  (NOT NULL)
 * @ns_name: the namespace name if it exists  (NOT NULL)
 *
 * This is the xtable equivalent routine of aa_split_fqname.  It finds the
 * split in an xtable fqname which contains an embedded \0 instead of a :
 * if a namespace is specified.  This is done so the xtable is constant and
 * isn't re-split on every lookup.
 *
 * Either the profile or namespace name may be optional but if the namespace
 * is specified the profile name termination must be present.  This results
 * in the following possible encodings:
 * profile_name\0
 * :ns_name\0profile_name\0
 * :ns_name\0\0
 *
 * NOTE: the xtable fqname is pre-validated at load time in unpack_trans_table
 *
 * Returns: profile name if it is specified else NULL
 */
static const char *separate_fqname(const char *fqname, const char **ns_name)
{
	const char *name;

	if (fqname[0] == ':') {
		/* In this case there is guaranteed to be two \0 terminators
		 * in the string.  They are verified at load time by
		 * by unpack_trans_table
		 */
		*ns_name = fqname + 1;		/* skip : */
		name = *ns_name + strlen(*ns_name) + 1;
		if (!*name)
			name = NULL;
	} else {
		*ns_name = NULL;
		name = fqname;
	}

	return name;
=======
 * Returns: label or NULL if no match found
 */
static struct aa_label *find_attach(const struct linux_binprm *bprm,
				    struct aa_ns *ns, struct list_head *head,
				    const char *name, const char **info)
{
	int candidate_len = 0, candidate_xattrs = 0;
	bool conflict = false;
	struct aa_profile *profile, *candidate = NULL;

	AA_BUG(!name);
	AA_BUG(!head);

	rcu_read_lock();
restart:
	list_for_each_entry_rcu(profile, head, base.list) {
		struct aa_attachment *attach = &profile->attach;

		if (profile->label.flags & FLAG_NULL &&
		    &profile->label == ns_unconfined(profile->ns))
			continue;

		/* Find the "best" matching profile. Profiles must
		 * match the path and extended attributes (if any)
		 * associated with the file. A more specific path
		 * match will be preferred over a less specific one,
		 * and a match with more matching extended attributes
		 * will be preferred over one with fewer. If the best
		 * match has both the same level of path specificity
		 * and the same number of matching extended attributes
		 * as another profile, signal a conflict and refuse to
		 * match.
		 */
		if (attach->xmatch->dfa) {
			unsigned int count;
			aa_state_t state;
			u32 index, perm;

			state = aa_dfa_leftmatch(attach->xmatch->dfa,
					attach->xmatch->start[AA_CLASS_XMATCH],
					name, &count);
			index = ACCEPT_TABLE(attach->xmatch->dfa)[state];
			perm = attach->xmatch->perms[index].allow;
			/* any accepting state means a valid match. */
			if (perm & MAY_EXEC) {
				int ret = 0;

				if (count < candidate_len)
					continue;

				if (bprm && attach->xattr_count) {
					long rev = READ_ONCE(ns->revision);

					if (!aa_get_profile_not0(profile))
						goto restart;
					rcu_read_unlock();
					ret = aa_xattrs_match(bprm, profile,
							      state);
					rcu_read_lock();
					aa_put_profile(profile);
					if (rev !=
					    READ_ONCE(ns->revision))
						/* policy changed */
						goto restart;
					/*
					 * Fail matching if the xattrs don't
					 * match
					 */
					if (ret < 0)
						continue;
				}
				/*
				 * TODO: allow for more flexible best match
				 *
				 * The new match isn't more specific
				 * than the current best match
				 */
				if (count == candidate_len &&
				    ret <= candidate_xattrs) {
					/* Match is equivalent, so conflict */
					if (ret == candidate_xattrs)
						conflict = true;
					continue;
				}

				/* Either the same length with more matching
				 * xattrs, or a longer match
				 */
				candidate = profile;
				candidate_len = max(count, attach->xmatch_len);
				candidate_xattrs = ret;
				conflict = false;
			}
		} else if (!strcmp(profile->base.name, name)) {
			/*
			 * old exact non-re match, without conditionals such
			 * as xattrs. no more searching required
			 */
			candidate = profile;
			goto out;
		}
	}

	if (!candidate || conflict) {
		if (conflict)
			*info = "conflicting profile attachments";
		rcu_read_unlock();
		return NULL;
	}

out:
	candidate = aa_get_newest_profile(candidate);
	rcu_read_unlock();

	return &candidate->label;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const char *next_name(int xtype, const char *name)
{
	return NULL;
}

/**
 * x_table_lookup - lookup an x transition name via transition table
 * @profile: current profile (NOT NULL)
 * @xindex: index into x transition table
<<<<<<< HEAD
 *
 * Returns: refcounted profile, or NULL on failure (MAYBE NULL)
 */
static struct aa_profile *x_table_lookup(struct aa_profile *profile, u32 xindex)
{
	struct aa_profile *new_profile = NULL;
	struct aa_namespace *ns = profile->ns;
	u32 xtype = xindex & AA_X_TYPE_MASK;
	int index = xindex & AA_X_INDEX_MASK;
	const char *name;

	/* index is guaranteed to be in range, validated at load time */
	for (name = profile->file.trans.table[index]; !new_profile && name;
	     name = next_name(xtype, name)) {
		struct aa_namespace *new_ns;
		const char *xname = NULL;

		new_ns = NULL;
		if (xindex & AA_X_CHILD) {
			/* release by caller */
			new_profile = aa_find_child(profile, name);
			continue;
		} else if (*name == ':') {
			/* switching namespace */
			const char *ns_name;
			xname = name = separate_fqname(name, &ns_name);
			if (!xname)
				/* no name so use profile name */
				xname = profile->base.hname;
			if (*ns_name == '@') {
				/* TODO: variable support */
				;
			}
			/* released below */
			new_ns = aa_find_namespace(ns, ns_name);
			if (!new_ns)
				continue;
		} else if (*name == '@') {
			/* TODO: variable support */
			continue;
		} else {
			/* basic namespace lookup */
			xname = name;
		}

		/* released by caller */
		new_profile = aa_lookup_profile(new_ns ? new_ns : ns, xname);
		aa_put_namespace(new_ns);
	}

	/* released by caller */
	return new_profile;
}

/**
 * x_to_profile - get target profile for a given xindex
 * @profile: current profile  (NOT NULL)
 * @name: name to lookup (NOT NULL)
 * @xindex: index into x transition table
 *
 * find profile for a transition index
 *
 * Returns: refcounted profile or NULL if not found available
 */
static struct aa_profile *x_to_profile(struct aa_profile *profile,
				       const char *name, u32 xindex)
{
	struct aa_profile *new_profile = NULL;
	struct aa_namespace *ns = profile->ns;
	u32 xtype = xindex & AA_X_TYPE_MASK;
=======
 * @name: returns: name tested to find label (NOT NULL)
 *
 * Returns: refcounted label, or NULL on failure (MAYBE NULL)
 */
struct aa_label *x_table_lookup(struct aa_profile *profile, u32 xindex,
				const char **name)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_label *label = NULL;
	u32 xtype = xindex & AA_X_TYPE_MASK;
	int index = xindex & AA_X_INDEX_MASK;

	AA_BUG(!name);

	/* index is guaranteed to be in range, validated at load time */
	/* TODO: move lookup parsing to unpack time so this is a straight
	 *       index into the resultant label
	 */
	for (*name = rules->file->trans.table[index]; !label && *name;
	     *name = next_name(xtype, *name)) {
		if (xindex & AA_X_CHILD) {
			struct aa_profile *new_profile;
			/* release by caller */
			new_profile = aa_find_child(profile, *name);
			if (new_profile)
				label = &new_profile->label;
			continue;
		}
		label = aa_label_parse(&profile->label, *name, GFP_KERNEL,
				       true, false);
		if (IS_ERR(label))
			label = NULL;
	}

	/* released by caller */

	return label;
}

/**
 * x_to_label - get target label for a given xindex
 * @profile: current profile  (NOT NULL)
 * @bprm: binprm structure of transitioning task
 * @name: name to lookup (NOT NULL)
 * @xindex: index into x transition table
 * @lookupname: returns: name used in lookup if one was specified (NOT NULL)
 * @info: info message if there was an error (NOT NULL)
 *
 * find label for a transition index
 *
 * Returns: refcounted label or NULL if not found available
 */
static struct aa_label *x_to_label(struct aa_profile *profile,
				   const struct linux_binprm *bprm,
				   const char *name, u32 xindex,
				   const char **lookupname,
				   const char **info)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_label *new = NULL;
	struct aa_ns *ns = profile->ns;
	u32 xtype = xindex & AA_X_TYPE_MASK;
	const char *stack = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (xtype) {
	case AA_X_NONE:
		/* fail exec unless ix || ux fallback - handled by caller */
<<<<<<< HEAD
		return NULL;
	case AA_X_NAME:
		if (xindex & AA_X_CHILD)
			/* released by caller */
			new_profile = find_attach(ns, &profile->base.profiles,
						  name);
		else
			/* released by caller */
			new_profile = find_attach(ns, &ns->base.profiles,
						  name);
		break;
	case AA_X_TABLE:
		/* released by caller */
		new_profile = x_table_lookup(profile, xindex);
		break;
	}

	/* released by caller */
	return new_profile;
}

/**
 * apparmor_bprm_set_creds - set the new creds on the bprm struct
 * @bprm: binprm for the exec  (NOT NULL)
 *
 * Returns: %0 or error on failure
 */
int apparmor_bprm_set_creds(struct linux_binprm *bprm)
{
	struct aa_task_cxt *cxt;
	struct aa_profile *profile, *new_profile = NULL;
	struct aa_namespace *ns;
	char *buffer = NULL;
	unsigned int state;
	struct file_perms perms = {};
	struct path_cond cond = {
		bprm->file->f_path.dentry->d_inode->i_uid,
		bprm->file->f_path.dentry->d_inode->i_mode
	};
	const char *name = NULL, *target = NULL, *info = NULL;
	int error = cap_bprm_set_creds(bprm);
	if (error)
		return error;

	if (bprm->cred_prepared)
		return 0;

	cxt = bprm->cred->security;
	BUG_ON(!cxt);

	profile = aa_get_profile(aa_newest_version(cxt->profile));
	/*
	 * get the namespace from the replacement profile as replacement
	 * can change the namespace
	 */
	ns = profile->ns;
	state = profile->file.start;

	/* buffer freed below, name is pointer into buffer */
	error = aa_path_name(&bprm->file->f_path, profile->path_flags, &buffer,
			     &name, &info);
	if (error) {
		if (profile->flags &
		    (PFLAG_IX_ON_NAME_ERROR | PFLAG_UNCONFINED))
			error = 0;
=======
		*lookupname = NULL;
		break;
	case AA_X_TABLE:
		/* TODO: fix when perm mapping done at unload */
		stack = rules->file->trans.table[xindex & AA_X_INDEX_MASK];
		if (*stack != '&') {
			/* released by caller */
			new = x_table_lookup(profile, xindex, lookupname);
			stack = NULL;
			break;
		}
		fallthrough;	/* to X_NAME */
	case AA_X_NAME:
		if (xindex & AA_X_CHILD)
			/* released by caller */
			new = find_attach(bprm, ns, &profile->base.profiles,
					  name, info);
		else
			/* released by caller */
			new = find_attach(bprm, ns, &ns->base.profiles,
					  name, info);
		*lookupname = name;
		break;
	}

	if (!new) {
		if (xindex & AA_X_INHERIT) {
			/* (p|c|n)ix - don't change profile but do
			 * use the newest version
			 */
			*info = "ix fallback";
			/* no profile && no error */
			new = aa_get_newest_label(&profile->label);
		} else if (xindex & AA_X_UNCONFINED) {
			new = aa_get_newest_label(ns_unconfined(profile->ns));
			*info = "ux fallback";
		}
	}

	if (new && stack) {
		/* base the stack on post domain transition */
		struct aa_label *base = new;

		new = aa_label_parse(base, stack, GFP_KERNEL, true, false);
		if (IS_ERR(new))
			new = NULL;
		aa_put_label(base);
	}

	/* released by caller */
	return new;
}

static struct aa_label *profile_transition(const struct cred *subj_cred,
					   struct aa_profile *profile,
					   const struct linux_binprm *bprm,
					   char *buffer, struct path_cond *cond,
					   bool *secure_exec)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	struct aa_label *new = NULL;
	const char *info = NULL, *name = NULL, *target = NULL;
	aa_state_t state = rules->file->start[AA_CLASS_FILE];
	struct aa_perms perms = {};
	bool nonewprivs = false;
	int error = 0;

	AA_BUG(!profile);
	AA_BUG(!bprm);
	AA_BUG(!buffer);

	error = aa_path_name(&bprm->file->f_path, profile->path_flags, buffer,
			     &name, &info, profile->disconnected);
	if (error) {
		if (profile_unconfined(profile) ||
		    (profile->label.flags & FLAG_IX_ON_NAME_ERROR)) {
			AA_DEBUG("name lookup ix on error");
			error = 0;
			new = aa_get_newest_label(&profile->label);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		name = bprm->filename;
		goto audit;
	}

<<<<<<< HEAD
	/* Test for onexec first as onexec directives override other
	 * x transitions.
	 */
	if (unconfined(profile)) {
		/* unconfined task */
		if (cxt->onexec)
			/* change_profile on exec already been granted */
			new_profile = aa_get_profile(cxt->onexec);
		else
			new_profile = find_attach(ns, &ns->base.profiles, name);
		if (!new_profile)
			goto cleanup;
		/*
		 * NOTE: Domain transitions from unconfined are allowed
		 * even when no_new_privs is set because this aways results
		 * in a further reduction of permissions.
		 */
		goto apply;
	}

	/* find exec permissions for name */
	state = aa_str_perms(profile->file.dfa, state, name, &cond, &perms);
	if (cxt->onexec) {
		struct file_perms cp;
		info = "change_profile onexec";
		if (!(perms.allow & AA_MAY_ONEXEC))
			goto audit;

		/* test if this exec can be paired with change_profile onexec.
		 * onexec permission is linked to exec with a standard pairing
		 * exec\0change_profile
		 */
		state = aa_dfa_null_transition(profile->file.dfa, state);
		cp = change_profile_perms(profile, cxt->onexec->ns,
					  cxt->onexec->base.name,
					  AA_MAY_ONEXEC, state);

		if (!(cp.allow & AA_MAY_ONEXEC))
			goto audit;
		new_profile = aa_get_profile(aa_newest_version(cxt->onexec));
		goto apply;
	}

	if (perms.allow & MAY_EXEC) {
		/* exec permission determine how to transition */
		new_profile = x_to_profile(profile, name, perms.xindex);
		if (!new_profile) {
			if (perms.xindex & AA_X_INHERIT) {
				/* (p|c|n)ix - don't change profile but do
				 * use the newest version, which was picked
				 * up above when getting profile
				 */
				info = "ix fallback";
				new_profile = aa_get_profile(profile);
				goto x_clear;
			} else if (perms.xindex & AA_X_UNCONFINED) {
				new_profile = aa_get_profile(ns->unconfined);
				info = "ux fallback";
			} else {
				error = -ENOENT;
				info = "profile not found";
			}
		}
	} else if (COMPLAIN_MODE(profile)) {
		/* no exec permission - are we in learning mode */
		new_profile = aa_new_null_profile(profile, 0);
=======
	if (profile_unconfined(profile)) {
		new = find_attach(bprm, profile->ns,
				  &profile->ns->base.profiles, name, &info);
		if (new) {
			AA_DEBUG("unconfined attached to new label");
			return new;
		}
		AA_DEBUG("unconfined exec no attachment");
		return aa_get_newest_label(&profile->label);
	}

	/* find exec permissions for name */
	state = aa_str_perms(rules->file, state, name, cond, &perms);
	if (perms.allow & MAY_EXEC) {
		/* exec permission determine how to transition */
		new = x_to_label(profile, bprm, name, perms.xindex, &target,
				 &info);
		if (new && new->proxy == profile->label.proxy && info) {
			/* hack ix fallback - improve how this is detected */
			goto audit;
		} else if (!new) {
			error = -EACCES;
			info = "profile transition not found";
			/* remove MAY_EXEC to audit as failure */
			perms.allow &= ~MAY_EXEC;
		}
	} else if (COMPLAIN_MODE(profile)) {
		/* no exec permission - learning mode */
		struct aa_profile *new_profile = NULL;

		new_profile = aa_new_learning_profile(profile, false, name,
						      GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!new_profile) {
			error = -ENOMEM;
			info = "could not create null profile";
		} else {
			error = -EACCES;
<<<<<<< HEAD
			target = new_profile->base.hname;
=======
			new = &new_profile->label;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		perms.xindex |= AA_X_UNSAFE;
	} else
		/* fail exec */
		error = -EACCES;

<<<<<<< HEAD
	/*
	 * Policy has specified a domain transition, if no_new_privs then
	 * fail the exec.
	 */
	if (bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS) {
		aa_put_profile(new_profile);
		error = -EPERM;
		goto cleanup;
	}

	if (!new_profile)
		goto audit;
=======
	if (!new)
		goto audit;


	if (!(perms.xindex & AA_X_UNSAFE)) {
		if (DEBUG_ON) {
			dbg_printk("apparmor: scrubbing environment variables"
				   " for %s profile=", name);
			aa_label_printk(new, GFP_KERNEL);
			dbg_printk("\n");
		}
		*secure_exec = true;
	}

audit:
	aa_audit_file(subj_cred, profile, &perms, OP_EXEC, MAY_EXEC, name,
		      target, new,
		      cond->uid, info, error);
	if (!new || nonewprivs) {
		aa_put_label(new);
		return ERR_PTR(error);
	}

	return new;
}

static int profile_onexec(const struct cred *subj_cred,
			  struct aa_profile *profile, struct aa_label *onexec,
			  bool stack, const struct linux_binprm *bprm,
			  char *buffer, struct path_cond *cond,
			  bool *secure_exec)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	aa_state_t state = rules->file->start[AA_CLASS_FILE];
	struct aa_perms perms = {};
	const char *xname = NULL, *info = "change_profile onexec";
	int error = -EACCES;

	AA_BUG(!profile);
	AA_BUG(!onexec);
	AA_BUG(!bprm);
	AA_BUG(!buffer);

	if (profile_unconfined(profile)) {
		/* change_profile on exec already granted */
		/*
		 * NOTE: Domain transitions from unconfined are allowed
		 * even when no_new_privs is set because this aways results
		 * in a further reduction of permissions.
		 */
		return 0;
	}

	error = aa_path_name(&bprm->file->f_path, profile->path_flags, buffer,
			     &xname, &info, profile->disconnected);
	if (error) {
		if (profile_unconfined(profile) ||
		    (profile->label.flags & FLAG_IX_ON_NAME_ERROR)) {
			AA_DEBUG("name lookup ix on error");
			error = 0;
		}
		xname = bprm->filename;
		goto audit;
	}

	/* find exec permissions for name */
	state = aa_str_perms(rules->file, state, xname, cond, &perms);
	if (!(perms.allow & AA_MAY_ONEXEC)) {
		info = "no change_onexec valid for executable";
		goto audit;
	}
	/* test if this exec can be paired with change_profile onexec.
	 * onexec permission is linked to exec with a standard pairing
	 * exec\0change_profile
	 */
	state = aa_dfa_null_transition(rules->file->dfa, state);
	error = change_profile_perms(profile, onexec, stack, AA_MAY_ONEXEC,
				     state, &perms);
	if (error) {
		perms.allow &= ~AA_MAY_ONEXEC;
		goto audit;
	}

	if (!(perms.xindex & AA_X_UNSAFE)) {
		if (DEBUG_ON) {
			dbg_printk("apparmor: scrubbing environment "
				   "variables for %s label=", xname);
			aa_label_printk(onexec, GFP_KERNEL);
			dbg_printk("\n");
		}
		*secure_exec = true;
	}

audit:
	return aa_audit_file(subj_cred, profile, &perms, OP_EXEC,
			     AA_MAY_ONEXEC, xname,
			     NULL, onexec, cond->uid, info, error);
}

/* ensure none ns domain transitions are correctly applied with onexec */

static struct aa_label *handle_onexec(const struct cred *subj_cred,
				      struct aa_label *label,
				      struct aa_label *onexec, bool stack,
				      const struct linux_binprm *bprm,
				      char *buffer, struct path_cond *cond,
				      bool *unsafe)
{
	struct aa_profile *profile;
	struct aa_label *new;
	int error;

	AA_BUG(!label);
	AA_BUG(!onexec);
	AA_BUG(!bprm);
	AA_BUG(!buffer);

	if (!stack) {
		error = fn_for_each_in_ns(label, profile,
				profile_onexec(subj_cred, profile, onexec, stack,
					       bprm, buffer, cond, unsafe));
		if (error)
			return ERR_PTR(error);
		new = fn_label_build_in_ns(label, profile, GFP_KERNEL,
				aa_get_newest_label(onexec),
				profile_transition(subj_cred, profile, bprm,
						   buffer,
						   cond, unsafe));

	} else {
		/* TODO: determine how much we want to loosen this */
		error = fn_for_each_in_ns(label, profile,
				profile_onexec(subj_cred, profile, onexec, stack, bprm,
					       buffer, cond, unsafe));
		if (error)
			return ERR_PTR(error);
		new = fn_label_build_in_ns(label, profile, GFP_KERNEL,
				aa_label_merge(&profile->label, onexec,
					       GFP_KERNEL),
				profile_transition(subj_cred, profile, bprm,
						   buffer,
						   cond, unsafe));
	}

	if (new)
		return new;

	/* TODO: get rid of GLOBAL_ROOT_UID */
	error = fn_for_each_in_ns(label, profile,
			aa_audit_file(subj_cred, profile, &nullperms,
				      OP_CHANGE_ONEXEC,
				      AA_MAY_ONEXEC, bprm->filename, NULL,
				      onexec, GLOBAL_ROOT_UID,
				      "failed to build target label", -ENOMEM));
	return ERR_PTR(error);
}

/**
 * apparmor_bprm_creds_for_exec - Update the new creds on the bprm struct
 * @bprm: binprm for the exec  (NOT NULL)
 *
 * Returns: %0 or error on failure
 *
 * TODO: once the other paths are done see if we can't refactor into a fn
 */
int apparmor_bprm_creds_for_exec(struct linux_binprm *bprm)
{
	struct aa_task_ctx *ctx;
	struct aa_label *label, *new = NULL;
	const struct cred *subj_cred;
	struct aa_profile *profile;
	char *buffer = NULL;
	const char *info = NULL;
	int error = 0;
	bool unsafe = false;
	vfsuid_t vfsuid = i_uid_into_vfsuid(file_mnt_idmap(bprm->file),
					    file_inode(bprm->file));
	struct path_cond cond = {
		vfsuid_into_kuid(vfsuid),
		file_inode(bprm->file)->i_mode
	};

	subj_cred = current_cred();
	ctx = task_ctx(current);
	AA_BUG(!cred_label(bprm->cred));
	AA_BUG(!ctx);

	label = aa_get_newest_label(cred_label(bprm->cred));

	/*
	 * Detect no new privs being set, and store the label it
	 * occurred under. Ideally this would happen when nnp
	 * is set but there isn't a good way to do that yet.
	 *
	 * Testing for unconfined must be done before the subset test
	 */
	if ((bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS) && !unconfined(label) &&
	    !ctx->nnp)
		ctx->nnp = aa_get_label(label);

	/* buffer freed below, name is pointer into buffer */
	buffer = aa_get_buffer(false);
	if (!buffer) {
		error = -ENOMEM;
		goto done;
	}

	/* Test for onexec first as onexec override other x transitions. */
	if (ctx->onexec)
		new = handle_onexec(subj_cred, label, ctx->onexec, ctx->token,
				    bprm, buffer, &cond, &unsafe);
	else
		new = fn_label_build(label, profile, GFP_KERNEL,
				profile_transition(subj_cred, profile, bprm,
						   buffer,
						   &cond, &unsafe));

	AA_BUG(!new);
	if (IS_ERR(new)) {
		error = PTR_ERR(new);
		goto done;
	} else if (!new) {
		error = -ENOMEM;
		goto done;
	}

	/* Policy has specified a domain transitions. If no_new_privs and
	 * confined ensure the transition is to confinement that is subset
	 * of the confinement when the task entered no new privs.
	 *
	 * NOTE: Domain transitions from unconfined and to stacked
	 * subsets are allowed even when no_new_privs is set because this
	 * aways results in a further reduction of permissions.
	 */
	if ((bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS) &&
	    !unconfined(label) &&
	    !aa_label_is_unconfined_subset(new, ctx->nnp)) {
		error = -EPERM;
		info = "no new privs";
		goto audit;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (bprm->unsafe & LSM_UNSAFE_SHARE) {
		/* FIXME: currently don't mediate shared state */
		;
	}

<<<<<<< HEAD
	if (bprm->unsafe & (LSM_UNSAFE_PTRACE | LSM_UNSAFE_PTRACE_CAP)) {
		error = may_change_ptraced_domain(current, new_profile);
		if (error) {
			aa_put_profile(new_profile);
			goto audit;
		}
	}

	/* Determine if secure exec is needed.
	 * Can be at this point for the following reasons:
	 * 1. unconfined switching to confined
	 * 2. confined switching to different confinement
	 * 3. confined switching to unconfined
	 *
	 * Cases 2 and 3 are marked as requiring secure exec
	 * (unless policy specified "unsafe exec")
	 *
	 * bprm->unsafe is used to cache the AA_X_UNSAFE permission
	 * to avoid having to recompute in secureexec
	 */
	if (!(perms.xindex & AA_X_UNSAFE)) {
		AA_DEBUG("scrubbing environment variables for %s profile=%s\n",
			 name, new_profile->base.hname);
		bprm->unsafe |= AA_SECURE_X_NEEDED;
	}
apply:
	target = new_profile->base.hname;
	/* when transitioning profiles clear unsafe personality bits */
	bprm->per_clear |= PER_CLEAR_ON_SETID;

x_clear:
	aa_put_profile(cxt->profile);
	/* transfer new profile reference will be released when cxt is freed */
	cxt->profile = new_profile;

	/* clear out all temporary/transitional state from the context */
	aa_put_profile(cxt->previous);
	aa_put_profile(cxt->onexec);
	cxt->previous = NULL;
	cxt->onexec = NULL;
	cxt->token = 0;

audit:
	error = aa_audit_file(profile, &perms, GFP_KERNEL, OP_EXEC, MAY_EXEC,
			      name, target, cond.uid, info, error);

cleanup:
	aa_put_profile(profile);
	kfree(buffer);

	return error;
}

/**
 * apparmor_bprm_secureexec - determine if secureexec is needed
 * @bprm: binprm for exec  (NOT NULL)
 *
 * Returns: %1 if secureexec is needed else %0
 */
int apparmor_bprm_secureexec(struct linux_binprm *bprm)
{
	int ret = cap_bprm_secureexec(bprm);

	/* the decision to use secure exec is computed in set_creds
	 * and stored in bprm->unsafe.
	 */
	if (!ret && (bprm->unsafe & AA_SECURE_X_NEEDED))
		ret = 1;

	return ret;
}

/**
 * apparmor_bprm_committing_creds - do task cleanup on committing new creds
 * @bprm: binprm for the exec  (NOT NULL)
 */
void apparmor_bprm_committing_creds(struct linux_binprm *bprm)
{
	struct aa_profile *profile = __aa_current_profile();
	struct aa_task_cxt *new_cxt = bprm->cred->security;

	/* bail out if unconfined or not changing profile */
	if ((new_cxt->profile == profile) ||
	    (unconfined(new_cxt->profile)))
		return;

	current->pdeath_signal = 0;

	/* reset soft limits and set hard limits for the new profile */
	__aa_transition_rlimits(profile, new_cxt->profile);
}

/**
 * apparmor_bprm_commited_cred - do cleanup after new creds committed
 * @bprm: binprm for the exec  (NOT NULL)
 */
void apparmor_bprm_committed_creds(struct linux_binprm *bprm)
{
	/* TODO: cleanup signals - ipc mediation */
	return;
=======
	if (bprm->unsafe & (LSM_UNSAFE_PTRACE)) {
		/* TODO: test needs to be profile of label to new */
		error = may_change_ptraced_domain(bprm->cred, new, &info);
		if (error)
			goto audit;
	}

	if (unsafe) {
		if (DEBUG_ON) {
			dbg_printk("scrubbing environment variables for %s "
				   "label=", bprm->filename);
			aa_label_printk(new, GFP_KERNEL);
			dbg_printk("\n");
		}
		bprm->secureexec = 1;
	}

	if (label->proxy != new->proxy) {
		/* when transitioning clear unsafe personality bits */
		if (DEBUG_ON) {
			dbg_printk("apparmor: clearing unsafe personality "
				   "bits. %s label=", bprm->filename);
			aa_label_printk(new, GFP_KERNEL);
			dbg_printk("\n");
		}
		bprm->per_clear |= PER_CLEAR_ON_SETID;
	}
	aa_put_label(cred_label(bprm->cred));
	/* transfer reference, released when cred is freed */
	set_cred_label(bprm->cred, new);

done:
	aa_put_label(label);
	aa_put_buffer(buffer);

	return error;

audit:
	error = fn_for_each(label, profile,
			aa_audit_file(current_cred(), profile, &nullperms,
				      OP_EXEC, MAY_EXEC,
				      bprm->filename, NULL, new,
				      vfsuid_into_kuid(vfsuid), info, error));
	aa_put_label(new);
	goto done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Functions for self directed profile change
 */

<<<<<<< HEAD
/**
 * new_compound_name - create an hname with @n2 appended to @n1
 * @n1: base of hname  (NOT NULL)
 * @n2: name to append (NOT NULL)
 *
 * Returns: new name or NULL on error
 */
static char *new_compound_name(const char *n1, const char *n2)
{
	char *name = kmalloc(strlen(n1) + strlen(n2) + 3, GFP_KERNEL);
	if (name)
		sprintf(name, "%s//%s", n1, n2);
	return name;
=======

/* helper fn for change_hat
 *
 * Returns: label for hat transition OR ERR_PTR.  Does NOT return NULL
 */
static struct aa_label *build_change_hat(const struct cred *subj_cred,
					 struct aa_profile *profile,
					 const char *name, bool sibling)
{
	struct aa_profile *root, *hat = NULL;
	const char *info = NULL;
	int error = 0;

	if (sibling && PROFILE_IS_HAT(profile)) {
		root = aa_get_profile_rcu(&profile->parent);
	} else if (!sibling && !PROFILE_IS_HAT(profile)) {
		root = aa_get_profile(profile);
	} else {
		info = "conflicting target types";
		error = -EPERM;
		goto audit;
	}

	hat = aa_find_child(root, name);
	if (!hat) {
		error = -ENOENT;
		if (COMPLAIN_MODE(profile)) {
			hat = aa_new_learning_profile(profile, true, name,
						      GFP_KERNEL);
			if (!hat) {
				info = "failed null profile create";
				error = -ENOMEM;
			}
		}
	}
	aa_put_profile(root);

audit:
	aa_audit_file(subj_cred, profile, &nullperms, OP_CHANGE_HAT,
		      AA_MAY_CHANGEHAT,
		      name, hat ? hat->base.hname : NULL,
		      hat ? &hat->label : NULL, GLOBAL_ROOT_UID, info,
		      error);
	if (!hat || (error && error != -ENOENT))
		return ERR_PTR(error);
	/* if hat && error - complain mode, already audited and we adjust for
	 * complain mode allow by returning hat->label
	 */
	return &hat->label;
}

/* helper fn for changing into a hat
 *
 * Returns: label for hat transition or ERR_PTR. Does not return NULL
 */
static struct aa_label *change_hat(const struct cred *subj_cred,
				   struct aa_label *label, const char *hats[],
				   int count, int flags)
{
	struct aa_profile *profile, *root, *hat = NULL;
	struct aa_label *new;
	struct label_it it;
	bool sibling = false;
	const char *name, *info = NULL;
	int i, error;

	AA_BUG(!label);
	AA_BUG(!hats);
	AA_BUG(count < 1);

	if (PROFILE_IS_HAT(labels_profile(label)))
		sibling = true;

	/*find first matching hat */
	for (i = 0; i < count && !hat; i++) {
		name = hats[i];
		label_for_each_in_ns(it, labels_ns(label), label, profile) {
			if (sibling && PROFILE_IS_HAT(profile)) {
				root = aa_get_profile_rcu(&profile->parent);
			} else if (!sibling && !PROFILE_IS_HAT(profile)) {
				root = aa_get_profile(profile);
			} else {	/* conflicting change type */
				info = "conflicting targets types";
				error = -EPERM;
				goto fail;
			}
			hat = aa_find_child(root, name);
			aa_put_profile(root);
			if (!hat) {
				if (!COMPLAIN_MODE(profile))
					goto outer_continue;
				/* complain mode succeed as if hat */
			} else if (!PROFILE_IS_HAT(hat)) {
				info = "target not hat";
				error = -EPERM;
				aa_put_profile(hat);
				goto fail;
			}
			aa_put_profile(hat);
		}
		/* found a hat for all profiles in ns */
		goto build;
outer_continue:
	;
	}
	/* no hats that match, find appropriate error
	 *
	 * In complain mode audit of the failure is based off of the first
	 * hat supplied.  This is done due how userspace interacts with
	 * change_hat.
	 */
	name = NULL;
	label_for_each_in_ns(it, labels_ns(label), label, profile) {
		if (!list_empty(&profile->base.profiles)) {
			info = "hat not found";
			error = -ENOENT;
			goto fail;
		}
	}
	info = "no hats defined";
	error = -ECHILD;

fail:
	label_for_each_in_ns(it, labels_ns(label), label, profile) {
		/*
		 * no target as it has failed to be found or built
		 *
		 * change_hat uses probing and should not log failures
		 * related to missing hats
		 */
		/* TODO: get rid of GLOBAL_ROOT_UID */
		if (count > 1 || COMPLAIN_MODE(profile)) {
			aa_audit_file(subj_cred, profile, &nullperms,
				      OP_CHANGE_HAT,
				      AA_MAY_CHANGEHAT, name, NULL, NULL,
				      GLOBAL_ROOT_UID, info, error);
		}
	}
	return ERR_PTR(error);

build:
	new = fn_label_build_in_ns(label, profile, GFP_KERNEL,
				   build_change_hat(subj_cred, profile, name,
						    sibling),
				   aa_get_label(&profile->label));
	if (!new) {
		info = "label build failed";
		error = -ENOMEM;
		goto fail;
	} /* else if (IS_ERR) build_change_hat has logged error so return new */

	return new;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * aa_change_hat - change hat to/from subprofile
 * @hats: vector of hat names to try changing into (MAYBE NULL if @count == 0)
 * @count: number of hat names in @hats
 * @token: magic value to validate the hat change
<<<<<<< HEAD
 * @permtest: true if this is just a permission test
=======
 * @flags: flags affecting behavior of the change
 *
 * Returns %0 on success, error otherwise.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Change to the first profile specified in @hats that exists, and store
 * the @hat_magic in the current task context.  If the count == 0 and the
 * @token matches that stored in the current task context, return to the
 * top level profile.
 *
<<<<<<< HEAD
 * Returns %0 on success, error otherwise.
 */
int aa_change_hat(const char *hats[], int count, u64 token, bool permtest)
{
	const struct cred *cred;
	struct aa_task_cxt *cxt;
	struct aa_profile *profile, *previous_profile, *hat = NULL;
	char *name = NULL;
	int i;
	struct file_perms perms = {};
	const char *target = NULL, *info = NULL;
	int error = 0;

	/*
	 * Fail explicitly requested domain transitions if no_new_privs.
	 * There is no exception for unconfined as change_hat is not
	 * available.
	 */
	if (task_no_new_privs(current))
		return -EPERM;

	/* released below */
	cred = get_current_cred();
	cxt = cred->security;
	profile = aa_cred_profile(cred);
	previous_profile = cxt->previous;

	if (unconfined(profile)) {
		info = "unconfined";
		error = -EPERM;
		goto audit;
	}

	if (count) {
		/* attempting to change into a new hat or switch to a sibling */
		struct aa_profile *root;
		root = PROFILE_IS_HAT(profile) ? profile->parent : profile;

		/* find first matching hat */
		for (i = 0; i < count && !hat; i++)
			/* released below */
			hat = aa_find_child(root, hats[i]);
		if (!hat) {
			if (!COMPLAIN_MODE(root) || permtest) {
				if (list_empty(&root->base.profiles))
					error = -ECHILD;
				else
					error = -ENOENT;
				goto out;
			}

			/*
			 * In complain mode and failed to match any hats.
			 * Audit the failure is based off of the first hat
			 * supplied.  This is done due how userspace
			 * interacts with change_hat.
			 *
			 * TODO: Add logging of all failed hats
			 */

			/* freed below */
			name = new_compound_name(root->base.hname, hats[0]);
			target = name;
			/* released below */
			hat = aa_new_null_profile(profile, 1);
			if (!hat) {
				info = "failed null profile create";
				error = -ENOMEM;
				goto audit;
			}
		} else {
			target = hat->base.hname;
			if (!PROFILE_IS_HAT(hat)) {
				info = "target not hat";
				error = -EPERM;
				goto audit;
			}
		}

		error = may_change_ptraced_domain(current, hat);
		if (error) {
			info = "ptraced";
			error = -EPERM;
			goto audit;
		}

		if (!permtest) {
			error = aa_set_current_hat(hat, token);
			if (error == -EACCES)
				/* kill task in case of brute force attacks */
				perms.kill = AA_MAY_CHANGEHAT;
			else if (name && !error)
				/* reset error for learning of new hats */
				error = -ENOENT;
		}
	} else if (previous_profile) {
		/* Return to saved profile.  Kill task if restore fails
		 * to avoid brute force attacks
		 */
		target = previous_profile->base.hname;
		error = aa_restore_previous_profile(token);
		perms.kill = AA_MAY_CHANGEHAT;
	} else
		/* ignore restores when there is no saved profile */
		goto out;

audit:
	if (!permtest)
		error = aa_audit_file(profile, &perms, GFP_KERNEL,
				      OP_CHANGE_HAT, AA_MAY_CHANGEHAT, NULL,
				      target, 0, info, error);

out:
	aa_put_profile(hat);
	kfree(name);
	put_cred(cred);
=======
 * change_hat only applies to profiles in the current ns, and each profile
 * in the ns must make the same transition otherwise change_hat will fail.
 */
int aa_change_hat(const char *hats[], int count, u64 token, int flags)
{
	const struct cred *subj_cred;
	struct aa_task_ctx *ctx = task_ctx(current);
	struct aa_label *label, *previous, *new = NULL, *target = NULL;
	struct aa_profile *profile;
	struct aa_perms perms = {};
	const char *info = NULL;
	int error = 0;

	/* released below */
	subj_cred = get_current_cred();
	label = aa_get_newest_cred_label(subj_cred);
	previous = aa_get_newest_label(ctx->previous);

	/*
	 * Detect no new privs being set, and store the label it
	 * occurred under. Ideally this would happen when nnp
	 * is set but there isn't a good way to do that yet.
	 *
	 * Testing for unconfined must be done before the subset test
	 */
	if (task_no_new_privs(current) && !unconfined(label) && !ctx->nnp)
		ctx->nnp = aa_get_label(label);

	if (unconfined(label)) {
		info = "unconfined can not change_hat";
		error = -EPERM;
		goto fail;
	}

	if (count) {
		new = change_hat(subj_cred, label, hats, count, flags);
		AA_BUG(!new);
		if (IS_ERR(new)) {
			error = PTR_ERR(new);
			new = NULL;
			/* already audited */
			goto out;
		}

		/* target cred is the same as current except new label */
		error = may_change_ptraced_domain(subj_cred, new, &info);
		if (error)
			goto fail;

		/*
		 * no new privs prevents domain transitions that would
		 * reduce restrictions.
		 */
		if (task_no_new_privs(current) && !unconfined(label) &&
		    !aa_label_is_unconfined_subset(new, ctx->nnp)) {
			/* not an apparmor denial per se, so don't log it */
			AA_DEBUG("no_new_privs - change_hat denied");
			error = -EPERM;
			goto out;
		}

		if (flags & AA_CHANGE_TEST)
			goto out;

		target = new;
		error = aa_set_current_hat(new, token);
		if (error == -EACCES)
			/* kill task in case of brute force attacks */
			goto kill;
	} else if (previous && !(flags & AA_CHANGE_TEST)) {
		/*
		 * no new privs prevents domain transitions that would
		 * reduce restrictions.
		 */
		if (task_no_new_privs(current) && !unconfined(label) &&
		    !aa_label_is_unconfined_subset(previous, ctx->nnp)) {
			/* not an apparmor denial per se, so don't log it */
			AA_DEBUG("no_new_privs - change_hat denied");
			error = -EPERM;
			goto out;
		}

		/* Return to saved label.  Kill task if restore fails
		 * to avoid brute force attacks
		 */
		target = previous;
		error = aa_restore_previous_label(token);
		if (error) {
			if (error == -EACCES)
				goto kill;
			goto fail;
		}
	} /* else ignore @flags && restores when there is no saved profile */

out:
	aa_put_label(new);
	aa_put_label(previous);
	aa_put_label(label);
	put_cred(subj_cred);

	return error;

kill:
	info = "failed token match";
	perms.kill = AA_MAY_CHANGEHAT;

fail:
	fn_for_each_in_ns(label, profile,
		aa_audit_file(subj_cred, profile, &perms, OP_CHANGE_HAT,
			      AA_MAY_CHANGEHAT, NULL, NULL, target,
			      GLOBAL_ROOT_UID, info, error));

	goto out;
}


static int change_profile_perms_wrapper(const char *op, const char *name,
					const struct cred *subj_cred,
					struct aa_profile *profile,
					struct aa_label *target, bool stack,
					u32 request, struct aa_perms *perms)
{
	struct aa_ruleset *rules = list_first_entry(&profile->rules,
						    typeof(*rules), list);
	const char *info = NULL;
	int error = 0;

	if (!error)
		error = change_profile_perms(profile, target, stack, request,
					     rules->file->start[AA_CLASS_FILE],
					     perms);
	if (error)
		error = aa_audit_file(subj_cred, profile, perms, op, request,
				      name,
				      NULL, target, GLOBAL_ROOT_UID, info,
				      error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
}

<<<<<<< HEAD
/**
 * aa_change_profile - perform a one-way profile transition
 * @ns_name: name of the profile namespace to change to (MAYBE NULL)
 * @hname: name of profile to change to (MAYBE NULL)
 * @onexec: whether this transition is to take place immediately or at exec
 * @permtest: true if this is just a permission test
=======
static const char *stack_msg = "change_profile unprivileged unconfined converted to stacking";

/**
 * aa_change_profile - perform a one-way profile transition
 * @fqname: name of profile may include namespace (NOT NULL)
 * @flags: flags affecting change behavior
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Change to new profile @name.  Unlike with hats, there is no way
 * to change back.  If @name isn't specified the current profile name is
 * used.
 * If @onexec then the transition is delayed until
 * the next exec.
 *
 * Returns %0 on success, error otherwise.
 */
<<<<<<< HEAD
int aa_change_profile(const char *ns_name, const char *hname, bool onexec,
		      bool permtest)
{
	const struct cred *cred;
	struct aa_task_cxt *cxt;
	struct aa_profile *profile, *target = NULL;
	struct aa_namespace *ns = NULL;
	struct file_perms perms = {};
	const char *name = NULL, *info = NULL;
	int op, error = 0;
	u32 request;

	if (!hname && !ns_name)
		return -EINVAL;

	if (onexec) {
		request = AA_MAY_ONEXEC;
		op = OP_CHANGE_ONEXEC;
	} else {
		request = AA_MAY_CHANGE_PROFILE;
		op = OP_CHANGE_PROFILE;
	}

	cred = get_current_cred();
	cxt = cred->security;
	profile = aa_cred_profile(cred);

	/*
	 * Fail explicitly requested domain transitions if no_new_privs
	 * and not unconfined.
	 * Domain transitions from unconfined are allowed even when
	 * no_new_privs is set because this aways results in a reduction
	 * of permissions.
	 */
	if (task_no_new_privs(current) && !unconfined(profile)) {
		put_cred(cred);
		return -EPERM;
	}

	if (ns_name) {
		/* released below */
		ns = aa_find_namespace(profile->ns, ns_name);
		if (!ns) {
			/* we don't create new namespace in complain mode */
			name = ns_name;
			info = "namespace not found";
			error = -ENOENT;
			goto audit;
		}
	} else
		/* released below */
		ns = aa_get_namespace(profile->ns);

	/* if the name was not specified, use the name of the current profile */
	if (!hname) {
		if (unconfined(profile))
			hname = ns->unconfined->base.hname;
		else
			hname = profile->base.hname;
	}

	perms = change_profile_perms(profile, ns, hname, request,
				     profile->file.start);
	if (!(perms.allow & request)) {
		error = -EACCES;
		goto audit;
	}

	/* released below */
	target = aa_lookup_profile(ns, hname);
	if (!target) {
		info = "profile not found";
		error = -ENOENT;
		if (permtest || !COMPLAIN_MODE(profile))
			goto audit;
		/* released below */
		target = aa_new_null_profile(profile, 0);
		if (!target) {
=======
int aa_change_profile(const char *fqname, int flags)
{
	struct aa_label *label, *new = NULL, *target = NULL;
	struct aa_profile *profile;
	struct aa_perms perms = {};
	const char *info = NULL;
	const char *auditname = fqname;		/* retain leading & if stack */
	bool stack = flags & AA_CHANGE_STACK;
	struct aa_task_ctx *ctx = task_ctx(current);
	const struct cred *subj_cred = get_current_cred();
	int error = 0;
	char *op;
	u32 request;

	label = aa_get_current_label();

	/*
	 * Detect no new privs being set, and store the label it
	 * occurred under. Ideally this would happen when nnp
	 * is set but there isn't a good way to do that yet.
	 *
	 * Testing for unconfined must be done before the subset test
	 */
	if (task_no_new_privs(current) && !unconfined(label) && !ctx->nnp)
		ctx->nnp = aa_get_label(label);

	if (!fqname || !*fqname) {
		aa_put_label(label);
		AA_DEBUG("no profile name");
		return -EINVAL;
	}

	if (flags & AA_CHANGE_ONEXEC) {
		request = AA_MAY_ONEXEC;
		if (stack)
			op = OP_STACK_ONEXEC;
		else
			op = OP_CHANGE_ONEXEC;
	} else {
		request = AA_MAY_CHANGE_PROFILE;
		if (stack)
			op = OP_STACK;
		else
			op = OP_CHANGE_PROFILE;
	}

	/* This should move to a per profile test. Requires pushing build
	 * into callback
	 */
	if (!stack && unconfined(label) &&
	    label == &labels_ns(label)->unconfined->label &&
	    aa_unprivileged_unconfined_restricted &&
	    /* TODO: refactor so this check is a fn */
	    cap_capable(current_cred(), &init_user_ns, CAP_MAC_OVERRIDE,
			CAP_OPT_NOAUDIT)) {
		/* regardless of the request in this case apparmor
		 * stacks against unconfined so admin set policy can't be
		 * by-passed
		 */
		stack = true;
		perms.audit = request;
		(void) fn_for_each_in_ns(label, profile,
				aa_audit_file(subj_cred, profile, &perms, op,
					      request, auditname, NULL, target,
					      GLOBAL_ROOT_UID, stack_msg, 0));
		perms.audit = 0;
	}

	if (*fqname == '&') {
		stack = true;
		/* don't have label_parse() do stacking */
		fqname++;
	}
	target = aa_label_parse(label, fqname, GFP_KERNEL, true, false);
	if (IS_ERR(target)) {
		struct aa_profile *tprofile;

		info = "label not found";
		error = PTR_ERR(target);
		target = NULL;
		/*
		 * TODO: fixme using labels_profile is not right - do profile
		 * per complain profile
		 */
		if ((flags & AA_CHANGE_TEST) ||
		    !COMPLAIN_MODE(labels_profile(label)))
			goto audit;
		/* released below */
		tprofile = aa_new_learning_profile(labels_profile(label), false,
						   fqname, GFP_KERNEL);
		if (!tprofile) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			info = "failed null profile create";
			error = -ENOMEM;
			goto audit;
		}
<<<<<<< HEAD
	}

	/* check if tracing task is allowed to trace target domain */
	error = may_change_ptraced_domain(current, target);
	if (error) {
		info = "ptrace prevents transition";
		goto audit;
	}

	if (permtest)
		goto audit;

	if (onexec)
		error = aa_set_current_onexec(target);
	else
		error = aa_replace_current_profile(target);

audit:
	if (!permtest)
		error = aa_audit_file(profile, &perms, GFP_KERNEL, op, request,
				      name, hname, 0, info, error);

	aa_put_namespace(ns);
	aa_put_profile(target);
	put_cred(cred);
=======
		target = &tprofile->label;
		goto check;
	}

	/*
	 * self directed transitions only apply to current policy ns
	 * TODO: currently requiring perms for stacking and straight change
	 *       stacking doesn't strictly need this. Determine how much
	 *       we want to loosen this restriction for stacking
	 *
	 * if (!stack) {
	 */
	error = fn_for_each_in_ns(label, profile,
			change_profile_perms_wrapper(op, auditname,
						     subj_cred,
						     profile, target, stack,
						     request, &perms));
	if (error)
		/* auditing done in change_profile_perms_wrapper */
		goto out;

	/* } */

check:
	/* check if tracing task is allowed to trace target domain */
	error = may_change_ptraced_domain(subj_cred, target, &info);
	if (error && !fn_for_each_in_ns(label, profile,
					COMPLAIN_MODE(profile)))
		goto audit;

	/* TODO: add permission check to allow this
	 * if ((flags & AA_CHANGE_ONEXEC) && !current_is_single_threaded()) {
	 *      info = "not a single threaded task";
	 *      error = -EACCES;
	 *      goto audit;
	 * }
	 */
	if (flags & AA_CHANGE_TEST)
		goto out;

	/* stacking is always a subset, so only check the nonstack case */
	if (!stack) {
		new = fn_label_build_in_ns(label, profile, GFP_KERNEL,
					   aa_get_label(target),
					   aa_get_label(&profile->label));
		/*
		 * no new privs prevents domain transitions that would
		 * reduce restrictions.
		 */
		if (task_no_new_privs(current) && !unconfined(label) &&
		    !aa_label_is_unconfined_subset(new, ctx->nnp)) {
			/* not an apparmor denial per se, so don't log it */
			AA_DEBUG("no_new_privs - change_hat denied");
			error = -EPERM;
			goto out;
		}
	}

	if (!(flags & AA_CHANGE_ONEXEC)) {
		/* only transition profiles in the current ns */
		if (stack)
			new = aa_label_merge(label, target, GFP_KERNEL);
		if (IS_ERR_OR_NULL(new)) {
			info = "failed to build target label";
			if (!new)
				error = -ENOMEM;
			else
				error = PTR_ERR(new);
			new = NULL;
			perms.allow = 0;
			goto audit;
		}
		error = aa_replace_current_label(new);
	} else {
		if (new) {
			aa_put_label(new);
			new = NULL;
		}

		/* full transition will be built in exec path */
		aa_set_current_onexec(target, stack);
	}

audit:
	error = fn_for_each_in_ns(label, profile,
			aa_audit_file(subj_cred,
				      profile, &perms, op, request, auditname,
				      NULL, new ? new : target,
				      GLOBAL_ROOT_UID, info, error));

out:
	aa_put_label(new);
	aa_put_label(target);
	aa_put_label(label);
	put_cred(subj_cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
}
