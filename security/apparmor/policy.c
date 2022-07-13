<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor policy manipulation functions
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * AppArmor policy is based around profiles, which contain the rules a
 * task is confined by.  Every task in the system has a profile attached
 * to it determined either by matching "unconfined" tasks against the
 * visible set of profiles or by following a profiles attachment rules.
 *
 * Each profile exists in a profile namespace which is a container of
 * visible profiles.  Each namespace contains a special "unconfined" profile,
 * which doesn't enforce any confinement on a task beyond DAC.
 *
 * Namespace and profile names can be written together in either
 * of two syntaxes.
 *	:namespace:profile - used by kernel interfaces for easy detection
 *	namespace://profile - used by policy
 *
 * Profile names can not start with : or @ or ^ and may not contain \0
 *
 * Reserved profile names
 *	unconfined - special automatically generated unconfined profile
 *	inherit - special name to indicate profile inheritance
 *	null-XXXX-YYYY - special automatically generated learning profiles
 *
 * Namespace names may not start with / or @ and may not contain \0 or :
 * Reserved namespace names
 *	user-XXXX - user defined profiles
 *
 * a // in a profile or namespace name indicates a hierarchical name with the
 * name before the // being the parent and the name after the child.
 *
 * Profile and namespace hierarchies serve two different but similar purposes.
 * The namespace contains the set of visible profiles that are considered
 * for attachment.  The hierarchy of namespaces allows for virtualizing
 * the namespace so that for example a chroot can have its own set of profiles
 * which may define some local user namespaces.
 * The profile hierarchy severs two distinct purposes,
 * -  it allows for sub profiles or hats, which allows an application to run
 *    subprograms under its own profile with different restriction than it
 *    self, and not have it use the system profile.
 *    eg. if a mail program starts an editor, the policy might make the
 *        restrictions tighter on the editor tighter than the mail program,
 *        and definitely different than general editor restrictions
 * - it allows for binary hierarchy of profiles, so that execution history
 *   is preserved.  This feature isn't exploited by AppArmor reference policy
 *   but is allowed.  NOTE: this is currently suboptimal because profile
 *   aliasing is not currently implemented so that a profile for each
 *   level must be defined.
 *   eg. /bin/bash///bin/ls as a name would indicate /bin/ls was started
 *       from /bin/bash
 *
 *   A profile or namespace name that can contain one or more // separators
 *   is referred to as an hname (hierarchical).
 *   eg.  /bin/bash//bin/ls
 *
 *   An fqname is a name that may contain both namespace and profile hnames.
 *   eg. :ns:/bin/bash//bin/ls
 *
 * NOTES:
 *   - locking of profile lists is currently fairly coarse.  All profile
 *     lists within a namespace use the namespace lock.
 * FIXME: move profile lists to using rcu_lists
 */

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
<<<<<<< HEAD

#include "include/apparmor.h"
#include "include/capability.h"
#include "include/context.h"
=======
#include <linux/cred.h>
#include <linux/rculist.h>
#include <linux/user_namespace.h>

#include "include/apparmor.h"
#include "include/capability.h"
#include "include/cred.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "include/file.h"
#include "include/ipc.h"
#include "include/match.h"
#include "include/path.h"
#include "include/policy.h"
<<<<<<< HEAD
#include "include/policy_unpack.h"
#include "include/resource.h"
#include "include/sid.h"


/* root profile namespace */
struct aa_namespace *root_ns;

const char *const profile_mode_names[] = {
	"enforce",
	"complain",
	"kill",
};

/**
 * hname_tail - find the last component of an hname
 * @name: hname to find the base profile name component of  (NOT NULL)
 *
 * Returns: the tail (base profile name) name component of an hname
 */
static const char *hname_tail(const char *hname)
{
	char *split;
	hname = strim((char *)hname);
	for (split = strstr(hname, "//"); split; split = strstr(hname, "//"))
		hname = split + 2;

	return hname;
}

/**
 * policy_init - initialize a policy structure
 * @policy: policy to initialize  (NOT NULL)
 * @prefix: prefix name if any is required.  (MAYBE NULL)
 * @name: name of the policy, init will make a copy of it  (NOT NULL)
 *
 * Note: this fn creates a copy of strings passed in
 *
 * Returns: true if policy init successful
 */
static bool policy_init(struct aa_policy *policy, const char *prefix,
			const char *name)
{
	/* freed by policy_free */
	if (prefix) {
		policy->hname = kmalloc(strlen(prefix) + strlen(name) + 3,
					GFP_KERNEL);
		if (policy->hname)
			sprintf(policy->hname, "%s//%s", prefix, name);
	} else
		policy->hname = kstrdup(name, GFP_KERNEL);
	if (!policy->hname)
		return 0;
	/* base.name is a substring of fqname */
	policy->name = (char *)hname_tail(policy->hname);
	INIT_LIST_HEAD(&policy->list);
	INIT_LIST_HEAD(&policy->profiles);
	kref_init(&policy->count);

	return 1;
}

/**
 * policy_destroy - free the elements referenced by @policy
 * @policy: policy that is to have its elements freed  (NOT NULL)
 */
static void policy_destroy(struct aa_policy *policy)
{
	/* still contains profiles -- invalid */
	if (!list_empty(&policy->profiles)) {
		AA_ERROR("%s: internal error, "
			 "policy '%s' still contains profiles\n",
			 __func__, policy->name);
		BUG();
	}
	if (!list_empty(&policy->list)) {
		AA_ERROR("%s: internal error, policy '%s' still on list\n",
			 __func__, policy->name);
		BUG();
	}

	/* don't free name as its a subset of hname */
	kzfree(policy->hname);
}

/**
 * __policy_find - find a policy by @name on a policy list
 * @head: list to search  (NOT NULL)
 * @name: name to search for  (NOT NULL)
 *
 * Requires: correct locks for the @head list be held
 *
 * Returns: unrefcounted policy that match @name or NULL if not found
 */
static struct aa_policy *__policy_find(struct list_head *head, const char *name)
{
	struct aa_policy *policy;

	list_for_each_entry(policy, head, list) {
		if (!strcmp(policy->name, name))
			return policy;
	}
	return NULL;
}

/**
 * __policy_strn_find - find a policy that's name matches @len chars of @str
 * @head: list to search  (NOT NULL)
 * @str: string to search for  (NOT NULL)
 * @len: length of match required
 *
 * Requires: correct locks for the @head list be held
 *
 * Returns: unrefcounted policy that match @str or NULL if not found
 *
 * if @len == strlen(@strlen) then this is equiv to __policy_find
 * other wise it allows searching for policy by a partial match of name
 */
static struct aa_policy *__policy_strn_find(struct list_head *head,
					    const char *str, int len)
{
	struct aa_policy *policy;

	list_for_each_entry(policy, head, list) {
		if (aa_strneq(policy->name, str, len))
			return policy;
	}

	return NULL;
}

/*
 * Routines for AppArmor namespaces
 */

static const char *hidden_ns_name = "---";
/**
 * aa_ns_visible - test if @view is visible from @curr
 * @curr: namespace to treat as the parent (NOT NULL)
 * @view:  namespace to test if visible from @curr (NOT NULL)
 *
 * Returns: true if @view is visible from @curr else false
 */
bool aa_ns_visible(struct aa_namespace *curr, struct aa_namespace *view)
{
	if (curr == view)
		return true;

	for ( ; view; view = view->parent) {
		if (view->parent == curr)
			return true;
	}
	return false;
}

/**
 * aa_na_name - Find the ns name to display for @view from @curr
 * @curr - current namespace (NOT NULL)
 * @view - namespace attempting to view (NOT NULL)
 *
 * Returns: name of @view visible from @curr
 */
const char *aa_ns_name(struct aa_namespace *curr, struct aa_namespace *view)
{
	/* if view == curr then the namespace name isn't displayed */
	if (curr == view)
		return "";

	if (aa_ns_visible(curr, view)) {
		/* at this point if a ns is visible it is in a view ns
		 * thus the curr ns.hname is a prefix of its name.
		 * Only output the virtualized portion of the name
		 * Add + 2 to skip over // separating curr hname prefix
		 * from the visible tail of the views hname
		 */
		return view->base.hname + strlen(curr->base.hname) + 2;
	} else
		return hidden_ns_name;
}

/**
 * alloc_namespace - allocate, initialize and return a new namespace
 * @prefix: parent namespace name (MAYBE NULL)
 * @name: a preallocated name  (NOT NULL)
 *
 * Returns: refcounted namespace or NULL on failure.
 */
static struct aa_namespace *alloc_namespace(const char *prefix,
					    const char *name)
{
	struct aa_namespace *ns;

	ns = kzalloc(sizeof(*ns), GFP_KERNEL);
	AA_DEBUG("%s(%p)\n", __func__, ns);
	if (!ns)
		return NULL;
	if (!policy_init(&ns->base, prefix, name))
		goto fail_ns;

	INIT_LIST_HEAD(&ns->sub_ns);
	rwlock_init(&ns->lock);

	/* released by free_namespace */
	ns->unconfined = aa_alloc_profile("unconfined");
	if (!ns->unconfined)
		goto fail_unconfined;

	ns->unconfined->sid = aa_alloc_sid();
	ns->unconfined->flags = PFLAG_UNCONFINED | PFLAG_IX_ON_NAME_ERROR |
	    PFLAG_IMMUTABLE;

	/*
	 * released by free_namespace, however __remove_namespace breaks
	 * the cyclic references (ns->unconfined, and unconfined->ns) and
	 * replaces with refs to parent namespace unconfined
	 */
	ns->unconfined->ns = aa_get_namespace(ns);

	return ns;

fail_unconfined:
	kzfree(ns->base.hname);
fail_ns:
	kzfree(ns);
	return NULL;
}

/**
 * free_namespace - free a profile namespace
 * @ns: the namespace to free  (MAYBE NULL)
 *
 * Requires: All references to the namespace must have been put, if the
 *           namespace was referenced by a profile confining a task,
 */
static void free_namespace(struct aa_namespace *ns)
{
	if (!ns)
		return;

	policy_destroy(&ns->base);
	aa_put_namespace(ns->parent);

	if (ns->unconfined && ns->unconfined->ns == ns)
		ns->unconfined->ns = NULL;

	aa_put_profile(ns->unconfined);
	kzfree(ns);
}

/**
 * aa_free_namespace_kref - free aa_namespace by kref (see aa_put_namespace)
 * @kr: kref callback for freeing of a namespace  (NOT NULL)
 */
void aa_free_namespace_kref(struct kref *kref)
{
	free_namespace(container_of(kref, struct aa_namespace, base.count));
}

/**
 * __aa_find_namespace - find a namespace on a list by @name
 * @head: list to search for namespace on  (NOT NULL)
 * @name: name of namespace to look for  (NOT NULL)
 *
 * Returns: unrefcounted namespace
 *
 * Requires: ns lock be held
 */
static struct aa_namespace *__aa_find_namespace(struct list_head *head,
						const char *name)
{
	return (struct aa_namespace *)__policy_find(head, name);
}

/**
 * aa_find_namespace  -  look up a profile namespace on the namespace list
 * @root: namespace to search in  (NOT NULL)
 * @name: name of namespace to find  (NOT NULL)
 *
 * Returns: a refcounted namespace on the list, or NULL if no namespace
 *          called @name exists.
 *
 * refcount released by caller
 */
struct aa_namespace *aa_find_namespace(struct aa_namespace *root,
				       const char *name)
{
	struct aa_namespace *ns = NULL;

	read_lock(&root->lock);
	ns = aa_get_namespace(__aa_find_namespace(&root->sub_ns, name));
	read_unlock(&root->lock);

	return ns;
}

/**
 * aa_prepare_namespace - find an existing or create a new namespace of @name
 * @name: the namespace to find or add  (MAYBE NULL)
 *
 * Returns: refcounted namespace or NULL if failed to create one
 */
static struct aa_namespace *aa_prepare_namespace(const char *name)
{
	struct aa_namespace *ns, *root;

	root = aa_current_profile()->ns;

	write_lock(&root->lock);

	/* if name isn't specified the profile is loaded to the current ns */
	if (!name) {
		/* released by caller */
		ns = aa_get_namespace(root);
		goto out;
	}

	/* try and find the specified ns and if it doesn't exist create it */
	/* released by caller */
	ns = aa_get_namespace(__aa_find_namespace(&root->sub_ns, name));
	if (!ns) {
		/* namespace not found */
		struct aa_namespace *new_ns;
		write_unlock(&root->lock);
		new_ns = alloc_namespace(root->base.hname, name);
		if (!new_ns)
			return NULL;
		write_lock(&root->lock);
		/* test for race when new_ns was allocated */
		ns = __aa_find_namespace(&root->sub_ns, name);
		if (!ns) {
			/* add parent ref */
			new_ns->parent = aa_get_namespace(root);

			list_add(&new_ns->base.list, &root->sub_ns);
			/* add list ref */
			ns = aa_get_namespace(new_ns);
		} else {
			/* raced so free the new one */
			free_namespace(new_ns);
			/* get reference on namespace */
			aa_get_namespace(ns);
		}
	}
out:
	write_unlock(&root->lock);

	/* return ref */
	return ns;
}

/**
 * __list_add_profile - add a profile to a list
=======
#include "include/policy_ns.h"
#include "include/policy_unpack.h"
#include "include/resource.h"

int unprivileged_userns_apparmor_policy = 1;
int aa_unprivileged_unconfined_restricted;

const char *const aa_profile_mode_names[] = {
	"enforce",
	"complain",
	"kill",
	"unconfined",
	"user",
};


static void aa_free_pdb(struct aa_policydb *pdb)
{
	if (pdb) {
		aa_put_dfa(pdb->dfa);
		if (pdb->perms)
			kvfree(pdb->perms);
		aa_free_str_table(&pdb->trans);
		kfree(pdb);
	}
}

/**
 * aa_pdb_free_kref - free aa_policydb by kref (called by aa_put_pdb)
 * @kref: kref callback for freeing of a dfa  (NOT NULL)
 */
void aa_pdb_free_kref(struct kref *kref)
{
	struct aa_policydb *pdb = container_of(kref, struct aa_policydb, count);

	aa_free_pdb(pdb);
}


struct aa_policydb *aa_alloc_pdb(gfp_t gfp)
{
	struct aa_policydb *pdb = kzalloc(sizeof(struct aa_policydb), gfp);

	if (!pdb)
		return NULL;

	kref_init(&pdb->count);

	return pdb;
}


/**
 * __add_profile - add a profiles to list and label tree
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @list: list to add it to  (NOT NULL)
 * @profile: the profile to add  (NOT NULL)
 *
 * refcount @profile, should be put by __list_remove_profile
 *
 * Requires: namespace lock be held, or list not be shared
 */
<<<<<<< HEAD
static void __list_add_profile(struct list_head *list,
			       struct aa_profile *profile)
{
	list_add(&profile->base.list, list);
	/* get list reference */
	aa_get_profile(profile);
=======
static void __add_profile(struct list_head *list, struct aa_profile *profile)
{
	struct aa_label *l;

	AA_BUG(!list);
	AA_BUG(!profile);
	AA_BUG(!profile->ns);
	AA_BUG(!mutex_is_locked(&profile->ns->lock));

	list_add_rcu(&profile->base.list, list);
	/* get list reference */
	aa_get_profile(profile);
	l = aa_label_insert(&profile->ns->labels, &profile->label);
	AA_BUG(l != &profile->label);
	aa_put_label(l);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * __list_remove_profile - remove a profile from the list it is on
 * @profile: the profile to remove  (NOT NULL)
 *
 * remove a profile from the list, warning generally removal should
 * be done with __replace_profile as most profile removals are
 * replacements to the unconfined profile.
 *
 * put @profile list refcount
 *
 * Requires: namespace lock be held, or list not have been live
 */
static void __list_remove_profile(struct aa_profile *profile)
{
<<<<<<< HEAD
	list_del_init(&profile->base.list);
	if (!(profile->flags & PFLAG_NO_LIST_REF))
		/* release list reference */
		aa_put_profile(profile);
}

/**
 * __replace_profile - replace @old with @new on a list
 * @old: profile to be replaced  (NOT NULL)
 * @new: profile to replace @old with  (NOT NULL)
 *
 * Will duplicate and refcount elements that @new inherits from @old
 * and will inherit @old children.
 *
 * refcount @new for list, put @old list refcount
 *
 * Requires: namespace list lock be held, or list not be shared
 */
static void __replace_profile(struct aa_profile *old, struct aa_profile *new)
{
	struct aa_policy *policy;
	struct aa_profile *child, *tmp;

	if (old->parent)
		policy = &old->parent->base;
	else
		policy = &old->ns->base;

	/* released when @new is freed */
	new->parent = aa_get_profile(old->parent);
	new->ns = aa_get_namespace(old->ns);
	new->sid = old->sid;
	__list_add_profile(&policy->profiles, new);
	/* inherit children */
	list_for_each_entry_safe(child, tmp, &old->base.profiles, base.list) {
		aa_put_profile(child->parent);
		child->parent = aa_get_profile(new);
		/* list refcount transferred to @new*/
		list_move(&child->base.list, &new->base.profiles);
	}

	/* released by free_profile */
	old->replacedby = aa_get_profile(new);
	__list_remove_profile(old);
}

static void __profile_list_release(struct list_head *head);

/**
=======
	AA_BUG(!profile);
	AA_BUG(!profile->ns);
	AA_BUG(!mutex_is_locked(&profile->ns->lock));

	list_del_rcu(&profile->base.list);
	aa_put_profile(profile);
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * __remove_profile - remove old profile, and children
 * @profile: profile to be replaced  (NOT NULL)
 *
 * Requires: namespace list lock be held, or list not be shared
 */
static void __remove_profile(struct aa_profile *profile)
{
<<<<<<< HEAD
	/* release any children lists first */
	__profile_list_release(&profile->base.profiles);
	/* released by free_profile */
	profile->replacedby = aa_get_profile(profile->ns->unconfined);
=======
	AA_BUG(!profile);
	AA_BUG(!profile->ns);
	AA_BUG(!mutex_is_locked(&profile->ns->lock));

	/* release any children lists first */
	__aa_profile_list_release(&profile->base.profiles);
	/* released by free_profile */
	aa_label_remove(&profile->label);
	__aafs_profile_rmdir(profile);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__list_remove_profile(profile);
}

/**
<<<<<<< HEAD
 * __profile_list_release - remove all profiles on the list and put refs
=======
 * __aa_profile_list_release - remove all profiles on the list and put refs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @head: list of profiles  (NOT NULL)
 *
 * Requires: namespace lock be held
 */
<<<<<<< HEAD
static void __profile_list_release(struct list_head *head)
=======
void __aa_profile_list_release(struct list_head *head)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct aa_profile *profile, *tmp;
	list_for_each_entry_safe(profile, tmp, head, base.list)
		__remove_profile(profile);
}

<<<<<<< HEAD
static void __ns_list_release(struct list_head *head);

/**
 * destroy_namespace - remove everything contained by @ns
 * @ns: namespace to have it contents removed  (NOT NULL)
 */
static void destroy_namespace(struct aa_namespace *ns)
{
	if (!ns)
		return;

	write_lock(&ns->lock);
	/* release all profiles in this namespace */
	__profile_list_release(&ns->base.profiles);

	/* release all sub namespaces */
	__ns_list_release(&ns->sub_ns);

	write_unlock(&ns->lock);
}

/**
 * __remove_namespace - remove a namespace and all its children
 * @ns: namespace to be removed  (NOT NULL)
 *
 * Requires: ns->parent->lock be held and ns removed from parent.
 */
static void __remove_namespace(struct aa_namespace *ns)
{
	struct aa_profile *unconfined = ns->unconfined;

	/* remove ns from namespace list */
	list_del_init(&ns->base.list);

	/*
	 * break the ns, unconfined profile cyclic reference and forward
	 * all new unconfined profiles requests to the parent namespace
	 * This will result in all confined tasks that have a profile
	 * being removed, inheriting the parent->unconfined profile.
	 */
	if (ns->parent)
		ns->unconfined = aa_get_profile(ns->parent->unconfined);

	destroy_namespace(ns);

	/* release original ns->unconfined ref */
	aa_put_profile(unconfined);
	/* release ns->base.list ref, from removal above */
	aa_put_namespace(ns);
}

/**
 * __ns_list_release - remove all profile namespaces on the list put refs
 * @head: list of profile namespaces  (NOT NULL)
 *
 * Requires: namespace lock be held
 */
static void __ns_list_release(struct list_head *head)
{
	struct aa_namespace *ns, *tmp;
	list_for_each_entry_safe(ns, tmp, head, base.list)
		__remove_namespace(ns);

}

/**
 * aa_alloc_root_ns - allocate the root profile namespace
 *
 * Returns: %0 on success else error
 *
 */
int __init aa_alloc_root_ns(void)
{
	/* released by aa_free_root_ns - used as list ref*/
	root_ns = alloc_namespace(NULL, "root");
	if (!root_ns)
		return -ENOMEM;

	return 0;
}

 /**
  * aa_free_root_ns - free the root profile namespace
  */
void __init aa_free_root_ns(void)
 {
	 struct aa_namespace *ns = root_ns;
	 root_ns = NULL;

	 destroy_namespace(ns);
	 aa_put_namespace(ns);
}

/**
 * aa_alloc_profile - allocate, initialize and return a new profile
 * @hname: name of the profile  (NOT NULL)
 *
 * Returns: refcount profile or NULL on failure
 */
struct aa_profile *aa_alloc_profile(const char *hname)
{
	struct aa_profile *profile;

	/* freed by free_profile - usually through aa_put_profile */
	profile = kzalloc(sizeof(*profile), GFP_KERNEL);
	if (!profile)
		return NULL;

	if (!policy_init(&profile->base, NULL, hname)) {
		kzfree(profile);
		return NULL;
	}

	/* refcount released by caller */
	return profile;
}

/**
 * aa_new_null_profile - create a new null-X learning profile
 * @parent: profile that caused this profile to be created (NOT NULL)
 * @hat: true if the null- learning profile is a hat
 *
 * Create a null- complain mode profile used in learning mode.  The name of
 * the profile is unique and follows the format of parent//null-sid.
 *
 * null profiles are added to the profile list but the list does not
 * hold a count on them so that they are automatically released when
 * not in use.
 *
 * Returns: new refcounted profile else NULL on failure
 */
struct aa_profile *aa_new_null_profile(struct aa_profile *parent, int hat)
{
	struct aa_profile *profile = NULL;
	char *name;
	u32 sid = aa_alloc_sid();

	/* freed below */
	name = kmalloc(strlen(parent->base.hname) + 2 + 7 + 8, GFP_KERNEL);
	if (!name)
		goto fail;
	sprintf(name, "%s//null-%x", parent->base.hname, sid);

	profile = aa_alloc_profile(name);
	kfree(name);
	if (!profile)
		goto fail;

	profile->sid = sid;
	profile->mode = APPARMOR_COMPLAIN;
	profile->flags = PFLAG_NULL;
	if (hat)
		profile->flags |= PFLAG_HAT;

	/* released on free_profile */
	profile->parent = aa_get_profile(parent);
	profile->ns = aa_get_namespace(parent->ns);

	write_lock(&profile->ns->lock);
	__list_add_profile(&parent->base.profiles, profile);
	write_unlock(&profile->ns->lock);

	/* refcount released by caller */
	return profile;

fail:
	aa_free_sid(sid);
	return NULL;
}

/**
 * free_profile - free a profile
=======
/**
 * aa_free_data - free a data blob
 * @ptr: data to free
 * @arg: unused
 */
static void aa_free_data(void *ptr, void *arg)
{
	struct aa_data *data = ptr;

	kfree_sensitive(data->data);
	kfree_sensitive(data->key);
	kfree_sensitive(data);
}

static void free_attachment(struct aa_attachment *attach)
{
	int i;

	for (i = 0; i < attach->xattr_count; i++)
		kfree_sensitive(attach->xattrs[i]);
	kfree_sensitive(attach->xattrs);
	aa_put_pdb(attach->xmatch);
}

static void free_ruleset(struct aa_ruleset *rules)
{
	int i;

	aa_put_pdb(rules->file);
	aa_put_pdb(rules->policy);
	aa_free_cap_rules(&rules->caps);
	aa_free_rlimit_rules(&rules->rlimits);

	for (i = 0; i < rules->secmark_count; i++)
		kfree_sensitive(rules->secmark[i].label);
	kfree_sensitive(rules->secmark);
	kfree_sensitive(rules);
}

struct aa_ruleset *aa_alloc_ruleset(gfp_t gfp)
{
	struct aa_ruleset *rules;

	rules = kzalloc(sizeof(*rules), gfp);
	if (rules)
		INIT_LIST_HEAD(&rules->list);

	return rules;
}

/**
 * aa_free_profile - free a profile
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @profile: the profile to free  (MAYBE NULL)
 *
 * Free a profile, its hats and null_profile. All references to the profile,
 * its hats and null_profile must have been put.
 *
 * If the profile was referenced from a task context, free_profile() will
 * be called from an rcu callback routine, so we must not sleep here.
 */
<<<<<<< HEAD
static void free_profile(struct aa_profile *profile)
{
=======
void aa_free_profile(struct aa_profile *profile)
{
	struct aa_ruleset *rule, *tmp;
	struct rhashtable *rht;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	AA_DEBUG("%s(%p)\n", __func__, profile);

	if (!profile)
		return;

<<<<<<< HEAD
	if (!list_empty(&profile->base.list)) {
		AA_ERROR("%s: internal error, "
			 "profile '%s' still on ns list\n",
			 __func__, profile->base.name);
		BUG();
	}

	/* free children profiles */
	policy_destroy(&profile->base);
	aa_put_profile(profile->parent);

	aa_put_namespace(profile->ns);
	kzfree(profile->rename);

	aa_free_file_rules(&profile->file);
	aa_free_cap_rules(&profile->caps);
	aa_free_rlimit_rules(&profile->rlimits);

	aa_free_sid(profile->sid);
	aa_put_dfa(profile->xmatch);
	aa_put_dfa(profile->policy.dfa);

	aa_put_profile(profile->replacedby);

	kzfree(profile);
}

/**
 * aa_free_profile_kref - free aa_profile by kref (called by aa_put_profile)
 * @kr: kref callback for freeing of a profile  (NOT NULL)
 */
void aa_free_profile_kref(struct kref *kref)
{
	struct aa_profile *p = container_of(kref, struct aa_profile,
					    base.count);

	free_profile(p);
=======
	/* free children profiles */
	aa_policy_destroy(&profile->base);
	aa_put_profile(rcu_access_pointer(profile->parent));

	aa_put_ns(profile->ns);
	kfree_sensitive(profile->rename);
	kfree_sensitive(profile->disconnected);

	free_attachment(&profile->attach);

	/*
	 * at this point there are no tasks that can have a reference
	 * to rules
	 */
	list_for_each_entry_safe(rule, tmp, &profile->rules, list) {
		list_del_init(&rule->list);
		free_ruleset(rule);
	}
	kfree_sensitive(profile->dirname);

	if (profile->data) {
		rht = profile->data;
		profile->data = NULL;
		rhashtable_free_and_destroy(rht, aa_free_data, NULL);
		kfree_sensitive(rht);
	}

	kfree_sensitive(profile->hash);
	aa_put_loaddata(profile->rawdata);
	aa_label_destroy(&profile->label);

	kfree_sensitive(profile);
}

/**
 * aa_alloc_profile - allocate, initialize and return a new profile
 * @hname: name of the profile  (NOT NULL)
 * @proxy: proxy to use OR null if to allocate a new one
 * @gfp: allocation type
 *
 * Returns: refcount profile or NULL on failure
 */
struct aa_profile *aa_alloc_profile(const char *hname, struct aa_proxy *proxy,
				    gfp_t gfp)
{
	struct aa_profile *profile;
	struct aa_ruleset *rules;

	/* freed by free_profile - usually through aa_put_profile */
	profile = kzalloc(struct_size(profile, label.vec, 2), gfp);
	if (!profile)
		return NULL;

	if (!aa_policy_init(&profile->base, NULL, hname, gfp))
		goto fail;
	if (!aa_label_init(&profile->label, 1, gfp))
		goto fail;

	INIT_LIST_HEAD(&profile->rules);

	/* allocate the first ruleset, but leave it empty */
	rules = aa_alloc_ruleset(gfp);
	if (!rules)
		goto fail;
	list_add(&rules->list, &profile->rules);

	/* update being set needed by fs interface */
	if (!proxy) {
		proxy = aa_alloc_proxy(&profile->label, gfp);
		if (!proxy)
			goto fail;
	} else
		aa_get_proxy(proxy);
	profile->label.proxy = proxy;

	profile->label.hname = profile->base.hname;
	profile->label.flags |= FLAG_PROFILE;
	profile->label.vec[0] = profile;

	/* refcount released by caller */
	return profile;

fail:
	aa_free_profile(profile);

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* TODO: profile accounting - setup in remove */

/**
<<<<<<< HEAD
 * __find_child - find a profile on @head list with a name matching @name
 * @head: list to search  (NOT NULL)
 * @name: name of profile (NOT NULL)
 *
 * Requires: ns lock protecting list be held
 *
 * Returns: unrefcounted profile ptr, or NULL if not found
 */
static struct aa_profile *__find_child(struct list_head *head, const char *name)
{
	return (struct aa_profile *)__policy_find(head, name);
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * __strn_find_child - find a profile on @head list using substring of @name
 * @head: list to search  (NOT NULL)
 * @name: name of profile (NOT NULL)
 * @len: length of @name substring to match
 *
<<<<<<< HEAD
 * Requires: ns lock protecting list be held
=======
 * Requires: rcu_read_lock be held
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns: unrefcounted profile ptr, or NULL if not found
 */
static struct aa_profile *__strn_find_child(struct list_head *head,
					    const char *name, int len)
{
	return (struct aa_profile *)__policy_strn_find(head, name, len);
}

/**
<<<<<<< HEAD
=======
 * __find_child - find a profile on @head list with a name matching @name
 * @head: list to search  (NOT NULL)
 * @name: name of profile (NOT NULL)
 *
 * Requires: rcu_read_lock be held
 *
 * Returns: unrefcounted profile ptr, or NULL if not found
 */
static struct aa_profile *__find_child(struct list_head *head, const char *name)
{
	return __strn_find_child(head, name, strlen(name));
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * aa_find_child - find a profile by @name in @parent
 * @parent: profile to search  (NOT NULL)
 * @name: profile name to search for  (NOT NULL)
 *
 * Returns: a refcounted profile or NULL if not found
 */
struct aa_profile *aa_find_child(struct aa_profile *parent, const char *name)
{
	struct aa_profile *profile;

<<<<<<< HEAD
	read_lock(&parent->ns->lock);
	profile = aa_get_profile(__find_child(&parent->base.profiles, name));
	read_unlock(&parent->ns->lock);
=======
	rcu_read_lock();
	do {
		profile = __find_child(&parent->base.profiles, name);
	} while (profile && !aa_get_profile_not0(profile));
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* refcount released by caller */
	return profile;
}

/**
 * __lookup_parent - lookup the parent of a profile of name @hname
 * @ns: namespace to lookup profile in  (NOT NULL)
 * @hname: hierarchical profile name to find parent of  (NOT NULL)
 *
 * Lookups up the parent of a fully qualified profile name, the profile
 * that matches hname does not need to exist, in general this
 * is used to load a new profile.
 *
<<<<<<< HEAD
 * Requires: ns->lock be held
 *
 * Returns: unrefcounted policy or NULL if not found
 */
static struct aa_policy *__lookup_parent(struct aa_namespace *ns,
=======
 * Requires: rcu_read_lock be held
 *
 * Returns: unrefcounted policy or NULL if not found
 */
static struct aa_policy *__lookup_parent(struct aa_ns *ns,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					 const char *hname)
{
	struct aa_policy *policy;
	struct aa_profile *profile = NULL;
	char *split;

	policy = &ns->base;

	for (split = strstr(hname, "//"); split;) {
		profile = __strn_find_child(&policy->profiles, hname,
					    split - hname);
		if (!profile)
			return NULL;
		policy = &profile->base;
		hname = split + 2;
		split = strstr(hname, "//");
	}
	if (!profile)
		return &ns->base;
	return &profile->base;
}

/**
<<<<<<< HEAD
 * __lookup_profile - lookup the profile matching @hname
 * @base: base list to start looking up profile name from  (NOT NULL)
 * @hname: hierarchical profile name  (NOT NULL)
 *
 * Requires: ns->lock be held
=======
 * __create_missing_ancestors - create place holders for missing ancestores
 * @ns: namespace to lookup profile in (NOT NULL)
 * @hname: hierarchical profile name to find parent of (NOT NULL)
 * @gfp: type of allocation.
 *
 * Requires: ns mutex lock held
 *
 * Return: unrefcounted parent policy on success or %NULL if error creating
 *          place holder profiles.
 */
static struct aa_policy *__create_missing_ancestors(struct aa_ns *ns,
						    const char *hname,
						    gfp_t gfp)
{
	struct aa_policy *policy;
	struct aa_profile *parent, *profile = NULL;
	char *split;

	AA_BUG(!ns);
	AA_BUG(!hname);

	policy = &ns->base;

	for (split = strstr(hname, "//"); split;) {
		parent = profile;
		profile = __strn_find_child(&policy->profiles, hname,
					    split - hname);
		if (!profile) {
			const char *name = kstrndup(hname, split - hname,
						    gfp);
			if (!name)
				return NULL;
			profile = aa_alloc_null(parent, name, gfp);
			kfree(name);
			if (!profile)
				return NULL;
			if (!parent)
				profile->ns = aa_get_ns(ns);
		}
		policy = &profile->base;
		hname = split + 2;
		split = strstr(hname, "//");
	}
	if (!profile)
		return &ns->base;
	return &profile->base;
}

/**
 * __lookupn_profile - lookup the profile matching @hname
 * @base: base list to start looking up profile name from  (NOT NULL)
 * @hname: hierarchical profile name  (NOT NULL)
 * @n: length of @hname
 *
 * Requires: rcu_read_lock be held
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns: unrefcounted profile pointer or NULL if not found
 *
 * Do a relative name lookup, recursing through profile tree.
 */
<<<<<<< HEAD
static struct aa_profile *__lookup_profile(struct aa_policy *base,
					   const char *hname)
{
	struct aa_profile *profile = NULL;
	char *split;

	for (split = strstr(hname, "//"); split;) {
=======
static struct aa_profile *__lookupn_profile(struct aa_policy *base,
					    const char *hname, size_t n)
{
	struct aa_profile *profile = NULL;
	const char *split;

	for (split = strnstr(hname, "//", n); split;
	     split = strnstr(hname, "//", n)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		profile = __strn_find_child(&base->profiles, hname,
					    split - hname);
		if (!profile)
			return NULL;

		base = &profile->base;
<<<<<<< HEAD
		hname = split + 2;
		split = strstr(hname, "//");
	}

	profile = __find_child(&base->profiles, hname);
=======
		n -= split + 2 - hname;
		hname = split + 2;
	}

	if (n)
		return __strn_find_child(&base->profiles, hname, n);
	return NULL;
}

static struct aa_profile *__lookup_profile(struct aa_policy *base,
					   const char *hname)
{
	return __lookupn_profile(base, hname, strlen(hname));
}

/**
 * aa_lookupn_profile - find a profile by its full or partial name
 * @ns: the namespace to start from (NOT NULL)
 * @hname: name to do lookup on.  Does not contain namespace prefix (NOT NULL)
 * @n: size of @hname
 *
 * Returns: refcounted profile or NULL if not found
 */
struct aa_profile *aa_lookupn_profile(struct aa_ns *ns, const char *hname,
				      size_t n)
{
	struct aa_profile *profile;

	rcu_read_lock();
	do {
		profile = __lookupn_profile(&ns->base, hname, n);
	} while (profile && !aa_get_profile_not0(profile));
	rcu_read_unlock();

	/* the unconfined profile is not in the regular profile list */
	if (!profile && strncmp(hname, "unconfined", n) == 0)
		profile = aa_get_newest_profile(ns->unconfined);

	/* refcount released by caller */
	return profile;
}

struct aa_profile *aa_lookup_profile(struct aa_ns *ns, const char *hname)
{
	return aa_lookupn_profile(ns, hname, strlen(hname));
}

struct aa_profile *aa_fqlookupn_profile(struct aa_label *base,
					const char *fqname, size_t n)
{
	struct aa_profile *profile;
	struct aa_ns *ns;
	const char *name, *ns_name;
	size_t ns_len;

	name = aa_splitn_fqname(fqname, n, &ns_name, &ns_len);
	if (ns_name) {
		ns = aa_lookupn_ns(labels_ns(base), ns_name, ns_len);
		if (!ns)
			return NULL;
	} else
		ns = aa_get_ns(labels_ns(base));

	if (name)
		profile = aa_lookupn_profile(ns, name, n - (name - fqname));
	else if (ns)
		/* default profile for ns, currently unconfined */
		profile = aa_get_newest_profile(ns->unconfined);
	else
		profile = NULL;
	aa_put_ns(ns);

	return profile;
}


struct aa_profile *aa_alloc_null(struct aa_profile *parent, const char *name,
				 gfp_t gfp)
{
	struct aa_profile *profile;
	struct aa_ruleset *rules;

	profile = aa_alloc_profile(name, NULL, gfp);
	if (!profile)
		return NULL;

	/* TODO: ideally we should inherit abi from parent */
	profile->label.flags |= FLAG_NULL;
	rules = list_first_entry(&profile->rules, typeof(*rules), list);
	rules->file = aa_get_pdb(nullpdb);
	rules->policy = aa_get_pdb(nullpdb);

	if (parent) {
		profile->path_flags = parent->path_flags;

		/* released on free_profile */
		rcu_assign_pointer(profile->parent, aa_get_profile(parent));
		profile->ns = aa_get_ns(parent->ns);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return profile;
}

/**
<<<<<<< HEAD
 * aa_lookup_profile - find a profile by its full or partial name
 * @ns: the namespace to start from (NOT NULL)
 * @hname: name to do lookup on.  Does not contain namespace prefix (NOT NULL)
 *
 * Returns: refcounted profile or NULL if not found
 */
struct aa_profile *aa_lookup_profile(struct aa_namespace *ns, const char *hname)
{
	struct aa_profile *profile;

	read_lock(&ns->lock);
	profile = aa_get_profile(__lookup_profile(&ns->base, hname));
	read_unlock(&ns->lock);

	/* refcount released by caller */
	return profile;
=======
 * aa_new_learning_profile - create or find a null-X learning profile
 * @parent: profile that caused this profile to be created (NOT NULL)
 * @hat: true if the null- learning profile is a hat
 * @base: name to base the null profile off of
 * @gfp: type of allocation
 *
 * Find/Create a null- complain mode profile used in learning mode.  The
 * name of the profile is unique and follows the format of parent//null-XXX.
 * where XXX is based on the @name or if that fails or is not supplied
 * a unique number
 *
 * null profiles are added to the profile list but the list does not
 * hold a count on them so that they are automatically released when
 * not in use.
 *
 * Returns: new refcounted profile else NULL on failure
 */
struct aa_profile *aa_new_learning_profile(struct aa_profile *parent, bool hat,
					   const char *base, gfp_t gfp)
{
	struct aa_profile *p, *profile;
	const char *bname;
	char *name = NULL;

	AA_BUG(!parent);

	if (base) {
		name = kmalloc(strlen(parent->base.hname) + 8 + strlen(base),
			       gfp);
		if (name) {
			sprintf(name, "%s//null-%s", parent->base.hname, base);
			goto name;
		}
		/* fall through to try shorter uniq */
	}

	name = kmalloc(strlen(parent->base.hname) + 2 + 7 + 8, gfp);
	if (!name)
		return NULL;
	sprintf(name, "%s//null-%x", parent->base.hname,
		atomic_inc_return(&parent->ns->uniq_null));

name:
	/* lookup to see if this is a dup creation */
	bname = basename(name);
	profile = aa_find_child(parent, bname);
	if (profile)
		goto out;

	profile = aa_alloc_null(parent, name, gfp);
	if (!profile)
		goto fail;
	profile->mode = APPARMOR_COMPLAIN;
	if (hat)
		profile->label.flags |= FLAG_HAT;

	mutex_lock_nested(&profile->ns->lock, profile->ns->level);
	p = __find_child(&parent->base.profiles, bname);
	if (p) {
		aa_free_profile(profile);
		profile = aa_get_profile(p);
	} else {
		__add_profile(&parent->base.profiles, profile);
	}
	mutex_unlock(&profile->ns->lock);

	/* refcount released by caller */
out:
	kfree(name);

	return profile;

fail:
	kfree(name);
	aa_free_profile(profile);
	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * replacement_allowed - test to see if replacement is allowed
 * @profile: profile to test if it can be replaced  (MAYBE NULL)
 * @noreplace: true if replacement shouldn't be allowed but addition is okay
 * @info: Returns - info about why replacement failed (NOT NULL)
 *
 * Returns: %0 if replacement allowed else error code
 */
static int replacement_allowed(struct aa_profile *profile, int noreplace,
			       const char **info)
{
	if (profile) {
<<<<<<< HEAD
		if (profile->flags & PFLAG_IMMUTABLE) {
			*info = "cannot replace immutible profile";
=======
		if (profile->label.flags & FLAG_IMMUTIBLE) {
			*info = "cannot replace immutable profile";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EPERM;
		} else if (noreplace) {
			*info = "profile already exists";
			return -EEXIST;
		}
	}
	return 0;
}

<<<<<<< HEAD
/**
 * __add_new_profile - simple wrapper around __list_add_profile
 * @ns: namespace that profile is being added to  (NOT NULL)
 * @policy: the policy container to add the profile to  (NOT NULL)
 * @profile: profile to add  (NOT NULL)
 *
 * add a profile to a list and do other required basic allocations
 */
static void __add_new_profile(struct aa_namespace *ns, struct aa_policy *policy,
			      struct aa_profile *profile)
{
	if (policy != &ns->base)
		/* released on profile replacement or free_profile */
		profile->parent = aa_get_profile((struct aa_profile *) policy);
	__list_add_profile(&policy->profiles, profile);
	/* released on free_profile */
	profile->sid = aa_alloc_sid();
	profile->ns = aa_get_namespace(ns);
}

/**
 * aa_audit_policy - Do auditing of policy changes
 * @op: policy operation being performed
 * @gfp: memory allocation flags
=======
/* audit callback for net specific fields */
static void audit_cb(struct audit_buffer *ab, void *va)
{
	struct common_audit_data *sa = va;
	struct apparmor_audit_data *ad = aad(sa);

	if (ad->iface.ns) {
		audit_log_format(ab, " ns=");
		audit_log_untrustedstring(ab, ad->iface.ns);
	}
}

/**
 * audit_policy - Do auditing of policy changes
 * @subj_label: label to check if it can manage policy
 * @op: policy operation being performed
 * @ns_name: name of namespace being manipulated
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @name: name of profile being manipulated (NOT NULL)
 * @info: any extra information to be audited (MAYBE NULL)
 * @error: error code
 *
 * Returns: the error to be returned after audit is done
 */
<<<<<<< HEAD
static int audit_policy(int op, gfp_t gfp, const char *name, const char *info,
			int error)
{
	struct common_audit_data sa;
	struct apparmor_audit_data aad = {0,};
	COMMON_AUDIT_DATA_INIT(&sa, NONE);
	sa.aad = &aad;
	aad.op = op;
	aad.name = name;
	aad.info = info;
	aad.error = error;

	return aa_audit(AUDIT_APPARMOR_STATUS, __aa_current_profile(), gfp,
			&sa, NULL);
=======
static int audit_policy(struct aa_label *subj_label, const char *op,
			const char *ns_name, const char *name,
			const char *info, int error)
{
	DEFINE_AUDIT_DATA(ad, LSM_AUDIT_DATA_NONE, AA_CLASS_NONE, op);

	ad.iface.ns = ns_name;
	ad.name = name;
	ad.info = info;
	ad.error = error;
	ad.subj_label = subj_label;

	aa_audit_msg(AUDIT_APPARMOR_STATUS, &ad, audit_cb);

	return error;
}

/* don't call out to other LSMs in the stack for apparmor policy admin
 * permissions
 */
static int policy_ns_capable(const struct cred *subj_cred,
			     struct aa_label *label,
			     struct user_namespace *userns, int cap)
{
	int err;

	/* check for MAC_ADMIN cap in cred */
	err = cap_capable(subj_cred, userns, cap, CAP_OPT_NONE);
	if (!err)
		err = aa_capable(subj_cred, label, cap, CAP_OPT_NONE);

	return err;
}

/**
 * aa_policy_view_capable - check if viewing policy in at @ns is allowed
 * @subj_cred: cred of subject
 * @label: label that is trying to view policy in ns
 * @ns: namespace being viewed by @label (may be NULL if @label's ns)
 *
 * Returns: true if viewing policy is allowed
 *
 * If @ns is NULL then the namespace being viewed is assumed to be the
 * tasks current namespace.
 */
bool aa_policy_view_capable(const struct cred *subj_cred,
			     struct aa_label *label, struct aa_ns *ns)
{
	struct user_namespace *user_ns = subj_cred->user_ns;
	struct aa_ns *view_ns = labels_view(label);
	bool root_in_user_ns = uid_eq(current_euid(), make_kuid(user_ns, 0)) ||
			       in_egroup_p(make_kgid(user_ns, 0));
	bool response = false;
	if (!ns)
		ns = view_ns;

	if (root_in_user_ns && aa_ns_visible(view_ns, ns, true) &&
	    (user_ns == &init_user_ns ||
	     (unprivileged_userns_apparmor_policy != 0 &&
	      user_ns->level == view_ns->level)))
		response = true;

	return response;
}

bool aa_policy_admin_capable(const struct cred *subj_cred,
			     struct aa_label *label, struct aa_ns *ns)
{
	struct user_namespace *user_ns = subj_cred->user_ns;
	bool capable = policy_ns_capable(subj_cred, label, user_ns,
					 CAP_MAC_ADMIN) == 0;

	AA_DEBUG("cap_mac_admin? %d\n", capable);
	AA_DEBUG("policy locked? %d\n", aa_g_lock_policy);

	return aa_policy_view_capable(subj_cred, label, ns) && capable &&
		!aa_g_lock_policy;
}

bool aa_current_policy_view_capable(struct aa_ns *ns)
{
	struct aa_label *label;
	bool res;

	label = __begin_current_label_crit_section();
	res = aa_policy_view_capable(current_cred(), label, ns);
	__end_current_label_crit_section(label);

	return res;
}

bool aa_current_policy_admin_capable(struct aa_ns *ns)
{
	struct aa_label *label;
	bool res;

	label = __begin_current_label_crit_section();
	res = aa_policy_admin_capable(current_cred(), label, ns);
	__end_current_label_crit_section(label);

	return res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * aa_may_manage_policy - can the current task manage policy
<<<<<<< HEAD
 * @op: the policy manipulation operation being done
 *
 * Returns: true if the task is allowed to manipulate policy
 */
bool aa_may_manage_policy(int op)
{
	/* check if loading policy is locked out */
	if (aa_g_lock_policy) {
		audit_policy(op, GFP_KERNEL, NULL, "policy_locked", -EACCES);
		return 0;
	}

	if (!capable(CAP_MAC_ADMIN)) {
		audit_policy(op, GFP_KERNEL, NULL, "not policy admin", -EACCES);
		return 0;
	}

	return 1;
=======
 * @subj_cred: subjects cred
 * @label: label to check if it can manage policy
 * @ns: namespace being managed by @label (may be NULL if @label's ns)
 * @mask: contains the policy manipulation operation being done
 *
 * Returns: 0 if the task is allowed to manipulate policy else error
 */
int aa_may_manage_policy(const struct cred *subj_cred, struct aa_label *label,
			 struct aa_ns *ns, u32 mask)
{
	const char *op;

	if (mask & AA_MAY_REMOVE_POLICY)
		op = OP_PROF_RM;
	else if (mask & AA_MAY_REPLACE_POLICY)
		op = OP_PROF_REPL;
	else
		op = OP_PROF_LOAD;

	/* check if loading policy is locked out */
	if (aa_g_lock_policy)
		return audit_policy(label, op, NULL, NULL, "policy_locked",
				    -EACCES);

	if (!aa_policy_admin_capable(subj_cred, label, ns))
		return audit_policy(label, op, NULL, NULL, "not policy admin",
				    -EACCES);

	/* TODO: add fine grained mediation of policy loads */
	return 0;
}

static struct aa_profile *__list_lookup_parent(struct list_head *lh,
					       struct aa_profile *profile)
{
	const char *base = basename(profile->base.hname);
	long len = base - profile->base.hname;
	struct aa_load_ent *ent;

	/* parent won't have trailing // so remove from len */
	if (len <= 2)
		return NULL;
	len -= 2;

	list_for_each_entry(ent, lh, list) {
		if (ent->new == profile)
			continue;
		if (strncmp(ent->new->base.hname, profile->base.hname, len) ==
		    0 && ent->new->base.hname[len] == 0)
			return ent->new;
	}

	return NULL;
}

/**
 * __replace_profile - replace @old with @new on a list
 * @old: profile to be replaced  (NOT NULL)
 * @new: profile to replace @old with  (NOT NULL)
 *
 * Will duplicate and refcount elements that @new inherits from @old
 * and will inherit @old children.
 *
 * refcount @new for list, put @old list refcount
 *
 * Requires: namespace list lock be held, or list not be shared
 */
static void __replace_profile(struct aa_profile *old, struct aa_profile *new)
{
	struct aa_profile *child, *tmp;

	if (!list_empty(&old->base.profiles)) {
		LIST_HEAD(lh);
		list_splice_init_rcu(&old->base.profiles, &lh, synchronize_rcu);

		list_for_each_entry_safe(child, tmp, &lh, base.list) {
			struct aa_profile *p;

			list_del_init(&child->base.list);
			p = __find_child(&new->base.profiles, child->base.name);
			if (p) {
				/* @p replaces @child  */
				__replace_profile(child, p);
				continue;
			}

			/* inherit @child and its children */
			/* TODO: update hname of inherited children */
			/* list refcount transferred to @new */
			p = aa_deref_parent(child);
			rcu_assign_pointer(child->parent, aa_get_profile(new));
			list_add_rcu(&child->base.list, &new->base.profiles);
			aa_put_profile(p);
		}
	}

	if (!rcu_access_pointer(new->parent)) {
		struct aa_profile *parent = aa_deref_parent(old);
		rcu_assign_pointer(new->parent, aa_get_profile(parent));
	}
	aa_label_replace(&old->label, &new->label);
	/* migrate dents must come after label replacement b/c update */
	__aafs_profile_migrate_dents(old, new);

	if (list_empty(&new->base.list)) {
		/* new is not on a list already */
		list_replace_rcu(&old->base.list, &new->base.list);
		aa_get_profile(new);
		aa_put_profile(old);
	} else
		__list_remove_profile(old);
}

/**
 * __lookup_replace - lookup replacement information for a profile
 * @ns: namespace the lookup occurs in
 * @hname: name of profile to lookup
 * @noreplace: true if not replacing an existing profile
 * @p: Returns - profile to be replaced
 * @info: Returns - info string on why lookup failed
 *
 * Returns: profile to replace (no ref) on success else ptr error
 */
static int __lookup_replace(struct aa_ns *ns, const char *hname,
			    bool noreplace, struct aa_profile **p,
			    const char **info)
{
	*p = aa_get_profile(__lookup_profile(&ns->base, hname));
	if (*p) {
		int error = replacement_allowed(*p, noreplace, info);
		if (error) {
			*info = "profile can not be replaced";
			return error;
		}
	}

	return 0;
}

static void share_name(struct aa_profile *old, struct aa_profile *new)
{
	aa_put_str(new->base.hname);
	aa_get_str(old->base.hname);
	new->base.hname = old->base.hname;
	new->base.name = old->base.name;
	new->label.hname = old->label.hname;
}

/* Update to newest version of parent after previous replacements
 * Returns: unrefcount newest version of parent
 */
static struct aa_profile *update_to_newest_parent(struct aa_profile *new)
{
	struct aa_profile *parent, *newest;

	parent = rcu_dereference_protected(new->parent,
					   mutex_is_locked(&new->ns->lock));
	newest = aa_get_newest_profile(parent);

	/* parent replaced in this atomic set? */
	if (newest != parent) {
		aa_put_profile(parent);
		rcu_assign_pointer(new->parent, newest);
	} else
		aa_put_profile(newest);

	return newest;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * aa_replace_profiles - replace profile(s) on the profile list
<<<<<<< HEAD
 * @udata: serialized data stream  (NOT NULL)
 * @size: size of the serialized data stream
 * @noreplace: true if only doing addition, no replacement allowed
 *
 * unpack and replace a profile on the profile list and uses of that profile
 * by any aa_task_cxt.  If the profile does not exist on the profile list
 * it is added.
 *
 * Returns: size of data consumed else error code on failure.
 */
ssize_t aa_replace_profiles(void *udata, size_t size, bool noreplace)
{
	struct aa_policy *policy;
	struct aa_profile *old_profile = NULL, *new_profile = NULL;
	struct aa_profile *rename_profile = NULL;
	struct aa_namespace *ns = NULL;
	const char *ns_name, *name = NULL, *info = NULL;
	int op = OP_PROF_REPL;
	ssize_t error;

	/* released below */
	new_profile = aa_unpack(udata, size, &ns_name);
	if (IS_ERR(new_profile)) {
		error = PTR_ERR(new_profile);
		new_profile = NULL;
		goto fail;
	}

	/* released below */
	ns = aa_prepare_namespace(ns_name);
	if (!ns) {
		info = "failed to prepare namespace";
		error = -ENOMEM;
		name = ns_name;
		goto fail;
	}

	name = new_profile->base.hname;

	write_lock(&ns->lock);
	/* no ref on policy only use inside lock */
	policy = __lookup_parent(ns, new_profile->base.hname);

	if (!policy) {
		info = "parent does not exist";
		error = -ENOENT;
		goto audit;
	}

	old_profile = __find_child(&policy->profiles, new_profile->base.name);
	/* released below */
	aa_get_profile(old_profile);

	if (new_profile->rename) {
		rename_profile = __lookup_profile(&ns->base,
						  new_profile->rename);
		/* released below */
		aa_get_profile(rename_profile);

		if (!rename_profile) {
			info = "profile to rename does not exist";
			name = new_profile->rename;
			error = -ENOENT;
			goto audit;
		}
	}

	error = replacement_allowed(old_profile, noreplace, &info);
	if (error)
		goto audit;

	error = replacement_allowed(rename_profile, noreplace, &info);
	if (error)
		goto audit;

audit:
	if (!old_profile && !rename_profile)
		op = OP_PROF_LOAD;

	error = audit_policy(op, GFP_ATOMIC, name, info, error);

	if (!error) {
		if (rename_profile)
			__replace_profile(rename_profile, new_profile);
		if (old_profile) {
			/* when there are both rename and old profiles
			 * inherit old profiles sid
			 */
			if (rename_profile)
				aa_free_sid(new_profile->sid);
			__replace_profile(old_profile, new_profile);
		}
		if (!(old_profile || rename_profile))
			__add_new_profile(ns, policy, new_profile);
	}
	write_unlock(&ns->lock);

out:
	aa_put_namespace(ns);
	aa_put_profile(rename_profile);
	aa_put_profile(old_profile);
	aa_put_profile(new_profile);
	if (error)
		return error;
	return size;

fail:
	error = audit_policy(op, GFP_KERNEL, name, info, error);
=======
 * @policy_ns: namespace load is occurring on
 * @label: label that is attempting to load/replace policy
 * @mask: permission mask
 * @udata: serialized data stream  (NOT NULL)
 *
 * unpack and replace a profile on the profile list and uses of that profile
 * by any task creds via invalidating the old version of the profile, which
 * tasks will notice to update their own cred.  If the profile does not exist
 * on the profile list it is added.
 *
 * Returns: size of data consumed else error code on failure.
 */
ssize_t aa_replace_profiles(struct aa_ns *policy_ns, struct aa_label *label,
			    u32 mask, struct aa_loaddata *udata)
{
	const char *ns_name = NULL, *info = NULL;
	struct aa_ns *ns = NULL;
	struct aa_load_ent *ent, *tmp;
	struct aa_loaddata *rawdata_ent;
	const char *op;
	ssize_t count, error;
	LIST_HEAD(lh);

	op = mask & AA_MAY_REPLACE_POLICY ? OP_PROF_REPL : OP_PROF_LOAD;
	aa_get_loaddata(udata);
	/* released below */
	error = aa_unpack(udata, &lh, &ns_name);
	if (error)
		goto out;

	/* ensure that profiles are all for the same ns
	 * TODO: update locking to remove this constaint. All profiles in
	 *       the load set must succeed as a set or the load will
	 *       fail. Sort ent list and take ns locks in hierarchy order
	 */
	count = 0;
	list_for_each_entry(ent, &lh, list) {
		if (ns_name) {
			if (ent->ns_name &&
			    strcmp(ent->ns_name, ns_name) != 0) {
				info = "policy load has mixed namespaces";
				error = -EACCES;
				goto fail;
			}
		} else if (ent->ns_name) {
			if (count) {
				info = "policy load has mixed namespaces";
				error = -EACCES;
				goto fail;
			}
			ns_name = ent->ns_name;
		} else
			count++;
	}
	if (ns_name) {
		ns = aa_prepare_ns(policy_ns ? policy_ns : labels_ns(label),
				   ns_name);
		if (IS_ERR(ns)) {
			op = OP_PROF_LOAD;
			info = "failed to prepare namespace";
			error = PTR_ERR(ns);
			ns = NULL;
			ent = NULL;
			goto fail;
		}
	} else
		ns = aa_get_ns(policy_ns ? policy_ns : labels_ns(label));

	mutex_lock_nested(&ns->lock, ns->level);
	/* check for duplicate rawdata blobs: space and file dedup */
	if (!list_empty(&ns->rawdata_list)) {
		list_for_each_entry(rawdata_ent, &ns->rawdata_list, list) {
			if (aa_rawdata_eq(rawdata_ent, udata)) {
				struct aa_loaddata *tmp;

				tmp = __aa_get_loaddata(rawdata_ent);
				/* check we didn't fail the race */
				if (tmp) {
					aa_put_loaddata(udata);
					udata = tmp;
					break;
				}
			}
		}
	}
	/* setup parent and ns info */
	list_for_each_entry(ent, &lh, list) {
		struct aa_policy *policy;
		struct aa_profile *p;

		if (aa_g_export_binary)
			ent->new->rawdata = aa_get_loaddata(udata);
		error = __lookup_replace(ns, ent->new->base.hname,
					 !(mask & AA_MAY_REPLACE_POLICY),
					 &ent->old, &info);
		if (error)
			goto fail_lock;

		if (ent->new->rename) {
			error = __lookup_replace(ns, ent->new->rename,
						!(mask & AA_MAY_REPLACE_POLICY),
						&ent->rename, &info);
			if (error)
				goto fail_lock;
		}

		/* released when @new is freed */
		ent->new->ns = aa_get_ns(ns);

		if (ent->old || ent->rename)
			continue;

		/* no ref on policy only use inside lock */
		p = NULL;
		policy = __lookup_parent(ns, ent->new->base.hname);
		if (!policy) {
			/* first check for parent in the load set */
			p = __list_lookup_parent(&lh, ent->new);
			if (!p) {
				/*
				 * fill in missing parent with null
				 * profile that doesn't have
				 * permissions. This allows for
				 * individual profile loading where
				 * the child is loaded before the
				 * parent, and outside of the current
				 * atomic set. This unfortunately can
				 * happen with some userspaces.  The
				 * null profile will be replaced once
				 * the parent is loaded.
				 */
				policy = __create_missing_ancestors(ns,
							ent->new->base.hname,
							GFP_KERNEL);
				if (!policy) {
					error = -ENOENT;
					info = "parent does not exist";
					goto fail_lock;
				}
			}
		}
		if (!p && policy != &ns->base)
			/* released on profile replacement or free_profile */
			p = (struct aa_profile *) policy;
		rcu_assign_pointer(ent->new->parent, aa_get_profile(p));
	}

	/* create new fs entries for introspection if needed */
	if (!udata->dents[AAFS_LOADDATA_DIR] && aa_g_export_binary) {
		error = __aa_fs_create_rawdata(ns, udata);
		if (error) {
			info = "failed to create raw_data dir and files";
			ent = NULL;
			goto fail_lock;
		}
	}
	list_for_each_entry(ent, &lh, list) {
		if (!ent->old) {
			struct dentry *parent;
			if (rcu_access_pointer(ent->new->parent)) {
				struct aa_profile *p;
				p = aa_deref_parent(ent->new);
				parent = prof_child_dir(p);
			} else
				parent = ns_subprofs_dir(ent->new->ns);
			error = __aafs_profile_mkdir(ent->new, parent);
		}

		if (error) {
			info = "failed to create";
			goto fail_lock;
		}
	}

	/* Done with checks that may fail - do actual replacement */
	__aa_bump_ns_revision(ns);
	if (aa_g_export_binary)
		__aa_loaddata_update(udata, ns->revision);
	list_for_each_entry_safe(ent, tmp, &lh, list) {
		list_del_init(&ent->list);
		op = (!ent->old && !ent->rename) ? OP_PROF_LOAD : OP_PROF_REPL;

		if (ent->old && ent->old->rawdata == ent->new->rawdata &&
		    ent->new->rawdata) {
			/* dedup actual profile replacement */
			audit_policy(label, op, ns_name, ent->new->base.hname,
				     "same as current profile, skipping",
				     error);
			/* break refcount cycle with proxy. */
			aa_put_proxy(ent->new->label.proxy);
			ent->new->label.proxy = NULL;
			goto skip;
		}

		/*
		 * TODO: finer dedup based on profile range in data. Load set
		 * can differ but profile may remain unchanged
		 */
		audit_policy(label, op, ns_name, ent->new->base.hname, NULL,
			     error);

		if (ent->old) {
			share_name(ent->old, ent->new);
			__replace_profile(ent->old, ent->new);
		} else {
			struct list_head *lh;

			if (rcu_access_pointer(ent->new->parent)) {
				struct aa_profile *parent;

				parent = update_to_newest_parent(ent->new);
				lh = &parent->base.profiles;
			} else
				lh = &ns->base.profiles;
			__add_profile(lh, ent->new);
		}
	skip:
		aa_load_ent_free(ent);
	}
	__aa_labelset_update_subtree(ns);
	mutex_unlock(&ns->lock);

out:
	aa_put_ns(ns);
	aa_put_loaddata(udata);
	kfree(ns_name);

	if (error)
		return error;
	return udata->size;

fail_lock:
	mutex_unlock(&ns->lock);

	/* audit cause of failure */
	op = (ent && !ent->old) ? OP_PROF_LOAD : OP_PROF_REPL;
fail:
	  audit_policy(label, op, ns_name, ent ? ent->new->base.hname : NULL,
		       info, error);
	/* audit status that rest of profiles in the atomic set failed too */
	info = "valid profile in failed atomic policy load";
	list_for_each_entry(tmp, &lh, list) {
		if (tmp == ent) {
			info = "unchecked profile in failed atomic policy load";
			/* skip entry that caused failure */
			continue;
		}
		op = (!tmp->old) ? OP_PROF_LOAD : OP_PROF_REPL;
		audit_policy(label, op, ns_name, tmp->new->base.hname, info,
			     error);
	}
	list_for_each_entry_safe(ent, tmp, &lh, list) {
		list_del_init(&ent->list);
		aa_load_ent_free(ent);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}

/**
 * aa_remove_profiles - remove profile(s) from the system
<<<<<<< HEAD
=======
 * @policy_ns: namespace the remove is being done from
 * @subj: label attempting to remove policy
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @fqname: name of the profile or namespace to remove  (NOT NULL)
 * @size: size of the name
 *
 * Remove a profile or sub namespace from the current namespace, so that
 * they can not be found anymore and mark them as replaced by unconfined
 *
<<<<<<< HEAD
 * NOTE: removing confinement does not restore rlimits to preconfinemnet values
 *
 * Returns: size of data consume else error code if fails
 */
ssize_t aa_remove_profiles(char *fqname, size_t size)
{
	struct aa_namespace *root, *ns = NULL;
	struct aa_profile *profile = NULL;
	const char *name = fqname, *info = NULL;
=======
 * NOTE: removing confinement does not restore rlimits to preconfinement values
 *
 * Returns: size of data consume else error code if fails
 */
ssize_t aa_remove_profiles(struct aa_ns *policy_ns, struct aa_label *subj,
			   char *fqname, size_t size)
{
	struct aa_ns *ns = NULL;
	struct aa_profile *profile = NULL;
	const char *name = fqname, *info = NULL;
	const char *ns_name = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ssize_t error = 0;

	if (*fqname == 0) {
		info = "no profile specified";
		error = -ENOENT;
		goto fail;
	}

<<<<<<< HEAD
	root = aa_current_profile()->ns;

	if (fqname[0] == ':') {
		char *ns_name;
		name = aa_split_fqname(fqname, &ns_name);
		if (ns_name) {
			/* released below */
			ns = aa_find_namespace(root, ns_name);
			if (!ns) {
				info = "namespace does not exist";
				error = -ENOENT;
				goto fail;
			}
		}
	} else
		/* released below */
		ns = aa_get_namespace(root);

	if (!name) {
		/* remove namespace - can only happen if fqname[0] == ':' */
		write_lock(&ns->parent->lock);
		__remove_namespace(ns);
		write_unlock(&ns->parent->lock);
	} else {
		/* remove profile */
		write_lock(&ns->lock);
=======
	if (fqname[0] == ':') {
		size_t ns_len;

		name = aa_splitn_fqname(fqname, size, &ns_name, &ns_len);
		/* released below */
		ns = aa_lookupn_ns(policy_ns ? policy_ns : labels_ns(subj),
				   ns_name, ns_len);
		if (!ns) {
			info = "namespace does not exist";
			error = -ENOENT;
			goto fail;
		}
	} else
		/* released below */
		ns = aa_get_ns(policy_ns ? policy_ns : labels_ns(subj));

	if (!name) {
		/* remove namespace - can only happen if fqname[0] == ':' */
		mutex_lock_nested(&ns->parent->lock, ns->parent->level);
		__aa_bump_ns_revision(ns);
		__aa_remove_ns(ns);
		mutex_unlock(&ns->parent->lock);
	} else {
		/* remove profile */
		mutex_lock_nested(&ns->lock, ns->level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		profile = aa_get_profile(__lookup_profile(&ns->base, name));
		if (!profile) {
			error = -ENOENT;
			info = "profile does not exist";
			goto fail_ns_lock;
		}
		name = profile->base.hname;
<<<<<<< HEAD
		__remove_profile(profile);
		write_unlock(&ns->lock);
	}

	/* don't fail removal if audit fails */
	(void) audit_policy(OP_PROF_RM, GFP_KERNEL, name, info, error);
	aa_put_namespace(ns);
=======
		__aa_bump_ns_revision(ns);
		__remove_profile(profile);
		__aa_labelset_update_subtree(ns);
		mutex_unlock(&ns->lock);
	}

	/* don't fail removal if audit fails */
	(void) audit_policy(subj, OP_PROF_RM, ns_name, name, info,
			    error);
	aa_put_ns(ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	aa_put_profile(profile);
	return size;

fail_ns_lock:
<<<<<<< HEAD
	write_unlock(&ns->lock);
	aa_put_namespace(ns);

fail:
	(void) audit_policy(OP_PROF_RM, GFP_KERNEL, name, info, error);
=======
	mutex_unlock(&ns->lock);
	aa_put_ns(ns);

fail:
	(void) audit_policy(subj, OP_PROF_RM, ns_name, name, info,
			    error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}
