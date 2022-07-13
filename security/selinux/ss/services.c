<<<<<<< HEAD
/*
 * Implementation of the security services.
 *
 * Authors : Stephen Smalley, <sds@epoch.ncsc.mil>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Implementation of the security services.
 *
 * Authors : Stephen Smalley, <stephen.smalley.work@gmail.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	     James Morris <jmorris@redhat.com>
 *
 * Updated: Trusted Computer Solutions, Inc. <dgoeddel@trustedcs.com>
 *
 *	Support for enhanced MLS infrastructure.
 *	Support for context based audit filters.
 *
 * Updated: Frank Mayer <mayerf@tresys.com> and Karl MacMillan <kmacmillan@tresys.com>
 *
 *	Added conditional policy language extensions
 *
 * Updated: Hewlett-Packard <paul@paul-moore.com>
 *
 *      Added support for NetLabel
 *      Added support for the policy capability bitmap
 *
 * Updated: Chad Sellers <csellers@tresys.com>
 *
 *  Added validation of kernel classes and permissions
 *
 * Updated: KaiGai Kohei <kaigai@ak.jp.nec.com>
 *
 *  Added support for bounds domain and audit messaged on masked permissions
 *
 * Updated: Guido Trentalancia <guido@trentalancia.com>
 *
 *  Added support for runtime switching of the policy type
 *
 * Copyright (C) 2008, 2009 NEC Corporation
 * Copyright (C) 2006, 2007 Hewlett-Packard Development Company, L.P.
 * Copyright (C) 2004-2006 Trusted Computer Solutions, Inc.
 * Copyright (C) 2003 - 2004, 2006 Tresys Technology, LLC
 * Copyright (C) 2003 Red Hat, Inc., James Morris <jmorris@redhat.com>
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/rcupdate.h>
#include <linux/errno.h>
#include <linux/in.h>
#include <linux/sched.h>
#include <linux/audit.h>
<<<<<<< HEAD
#include <linux/mutex.h>
#include <linux/selinux.h>
#include <linux/flex_array.h>
#include <linux/vmalloc.h>
=======
#include <linux/vmalloc.h>
#include <linux/lsm_hooks.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netlabel.h>

#include "flask.h"
#include "avc.h"
#include "avc_ss.h"
#include "security.h"
#include "context.h"
#include "policydb.h"
#include "sidtab.h"
#include "services.h"
#include "conditional.h"
#include "mls.h"
#include "objsec.h"
#include "netlabel.h"
#include "xfrm.h"
#include "ebitmap.h"
#include "audit.h"
<<<<<<< HEAD

int selinux_policycap_netpeer;
int selinux_policycap_openperm;

static DEFINE_RWLOCK(policy_rwlock);

static struct sidtab sidtab;
struct policydb policydb;
int ss_initialized;

/*
 * The largest sequence number that has been used when
 * providing an access decision to the access vector cache.
 * The sequence number only changes when a policy change
 * occurs.
 */
static u32 latest_granting;

/* Forward declaration. */
static int context_struct_to_string(struct context *context, char **scontext,
				    u32 *scontext_len);

static void context_struct_compute_av(struct context *scontext,
					struct context *tcontext,
					u16 tclass,
					struct av_decision *avd,
					struct extended_perms *xperms);

struct selinux_mapping {
	u16 value; /* policy value */
	unsigned num_perms;
	u32 perms[sizeof(u32) * 8];
};

static struct selinux_mapping *current_mapping;
static u16 current_mapping_size;

static int selinux_set_mapping(struct policydb *pol,
			       struct security_class_mapping *map,
			       struct selinux_mapping **out_map_p,
			       u16 *out_map_size)
{
	struct selinux_mapping *out_map = NULL;
	size_t size = sizeof(struct selinux_mapping);
	u16 i, j;
	unsigned k;
=======
#include "policycap_names.h"
#include "ima.h"

struct selinux_policy_convert_data {
	struct convert_context_args args;
	struct sidtab_convert_params sidtab_params;
};

/* Forward declaration. */
static int context_struct_to_string(struct policydb *policydb,
				    struct context *context,
				    char **scontext,
				    u32 *scontext_len);

static int sidtab_entry_to_string(struct policydb *policydb,
				  struct sidtab *sidtab,
				  struct sidtab_entry *entry,
				  char **scontext,
				  u32 *scontext_len);

static void context_struct_compute_av(struct policydb *policydb,
				      struct context *scontext,
				      struct context *tcontext,
				      u16 tclass,
				      struct av_decision *avd,
				      struct extended_perms *xperms);

static int selinux_set_mapping(struct policydb *pol,
			       const struct security_class_mapping *map,
			       struct selinux_map *out_map)
{
	u16 i, j;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool print_unknown_handle = false;

	/* Find number of classes in the input mapping */
	if (!map)
		return -EINVAL;
	i = 0;
	while (map[i].name)
		i++;

	/* Allocate space for the class records, plus one for class zero */
<<<<<<< HEAD
	out_map = kcalloc(++i, size, GFP_ATOMIC);
	if (!out_map)
=======
	out_map->mapping = kcalloc(++i, sizeof(*out_map->mapping), GFP_ATOMIC);
	if (!out_map->mapping)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	/* Store the raw class and permission values */
	j = 0;
	while (map[j].name) {
<<<<<<< HEAD
		struct security_class_mapping *p_in = map + (j++);
		struct selinux_mapping *p_out = out_map + j;
=======
		const struct security_class_mapping *p_in = map + (j++);
		struct selinux_mapping *p_out = out_map->mapping + j;
		u16 k;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* An empty class string skips ahead */
		if (!strcmp(p_in->name, "")) {
			p_out->num_perms = 0;
			continue;
		}

		p_out->value = string_to_security_class(pol, p_in->name);
		if (!p_out->value) {
<<<<<<< HEAD
			printk(KERN_INFO
			       "SELinux:  Class %s not defined in policy.\n",
=======
			pr_info("SELinux:  Class %s not defined in policy.\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       p_in->name);
			if (pol->reject_unknown)
				goto err;
			p_out->num_perms = 0;
			print_unknown_handle = true;
			continue;
		}

		k = 0;
<<<<<<< HEAD
		while (p_in->perms && p_in->perms[k]) {
=======
		while (p_in->perms[k]) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* An empty permission string skips ahead */
			if (!*p_in->perms[k]) {
				k++;
				continue;
			}
			p_out->perms[k] = string_to_av_perm(pol, p_out->value,
							    p_in->perms[k]);
			if (!p_out->perms[k]) {
<<<<<<< HEAD
				printk(KERN_INFO
				       "SELinux:  Permission %s in class %s not defined in policy.\n",
=======
				pr_info("SELinux:  Permission %s in class %s not defined in policy.\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       p_in->perms[k], p_in->name);
				if (pol->reject_unknown)
					goto err;
				print_unknown_handle = true;
			}

			k++;
		}
		p_out->num_perms = k;
	}

	if (print_unknown_handle)
<<<<<<< HEAD
		printk(KERN_INFO "SELinux: the above unknown classes and permissions will be %s\n",
		       pol->allow_unknown ? "allowed" : "denied");

	*out_map_p = out_map;
	*out_map_size = i;
	return 0;
err:
	kfree(out_map);
=======
		pr_info("SELinux: the above unknown classes and permissions will be %s\n",
		       pol->allow_unknown ? "allowed" : "denied");

	out_map->size = i;
	return 0;
err:
	kfree(out_map->mapping);
	out_map->mapping = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EINVAL;
}

/*
 * Get real, policy values from mapped values
 */

<<<<<<< HEAD
static u16 unmap_class(u16 tclass)
{
	if (tclass < current_mapping_size)
		return current_mapping[tclass].value;
=======
static u16 unmap_class(struct selinux_map *map, u16 tclass)
{
	if (tclass < map->size)
		return map->mapping[tclass].value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return tclass;
}

/*
 * Get kernel value for class from its policy value
 */
<<<<<<< HEAD
static u16 map_class(u16 pol_value)
{
	u16 i;

	for (i = 1; i < current_mapping_size; i++) {
		if (current_mapping[i].value == pol_value)
=======
static u16 map_class(struct selinux_map *map, u16 pol_value)
{
	u16 i;

	for (i = 1; i < map->size; i++) {
		if (map->mapping[i].value == pol_value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return i;
	}

	return SECCLASS_NULL;
}

<<<<<<< HEAD
static void map_decision(u16 tclass, struct av_decision *avd,
			 int allow_unknown)
{
	if (tclass < current_mapping_size) {
		unsigned i, n = current_mapping[tclass].num_perms;
		u32 result;

		for (i = 0, result = 0; i < n; i++) {
			if (avd->allowed & current_mapping[tclass].perms[i])
				result |= 1<<i;
			if (allow_unknown && !current_mapping[tclass].perms[i])
				result |= 1<<i;
=======
static void map_decision(struct selinux_map *map,
			 u16 tclass, struct av_decision *avd,
			 int allow_unknown)
{
	if (tclass < map->size) {
		struct selinux_mapping *mapping = &map->mapping[tclass];
		unsigned int i, n = mapping->num_perms;
		u32 result;

		for (i = 0, result = 0; i < n; i++) {
			if (avd->allowed & mapping->perms[i])
				result |= (u32)1<<i;
			if (allow_unknown && !mapping->perms[i])
				result |= (u32)1<<i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		avd->allowed = result;

		for (i = 0, result = 0; i < n; i++)
<<<<<<< HEAD
			if (avd->auditallow & current_mapping[tclass].perms[i])
				result |= 1<<i;
		avd->auditallow = result;

		for (i = 0, result = 0; i < n; i++) {
			if (avd->auditdeny & current_mapping[tclass].perms[i])
				result |= 1<<i;
			if (!allow_unknown && !current_mapping[tclass].perms[i])
				result |= 1<<i;
=======
			if (avd->auditallow & mapping->perms[i])
				result |= (u32)1<<i;
		avd->auditallow = result;

		for (i = 0, result = 0; i < n; i++) {
			if (avd->auditdeny & mapping->perms[i])
				result |= (u32)1<<i;
			if (!allow_unknown && !mapping->perms[i])
				result |= (u32)1<<i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		/*
		 * In case the kernel has a bug and requests a permission
		 * between num_perms and the maximum permission number, we
		 * should audit that denial
		 */
		for (; i < (sizeof(u32)*8); i++)
<<<<<<< HEAD
			result |= 1<<i;
=======
			result |= (u32)1<<i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		avd->auditdeny = result;
	}
}

int security_mls_enabled(void)
{
<<<<<<< HEAD
	return policydb.mls_enabled;
=======
	int mls_enabled;
	struct selinux_policy *policy;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	mls_enabled = policy->policydb.mls_enabled;
	rcu_read_unlock();
	return mls_enabled;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return the boolean value of a constraint expression
 * when it is applied to the specified source and target
 * security contexts.
 *
 * xcontext is a special beast...  It is used by the validatetrans rules
 * only.  For these rules, scontext is the context before the transition,
 * tcontext is the context after the transition, and xcontext is the context
 * of the process performing the transition.  All other callers of
 * constraint_expr_eval should pass in NULL for xcontext.
 */
<<<<<<< HEAD
static int constraint_expr_eval(struct context *scontext,
=======
static int constraint_expr_eval(struct policydb *policydb,
				struct context *scontext,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct context *tcontext,
				struct context *xcontext,
				struct constraint_expr *cexpr)
{
	u32 val1, val2;
	struct context *c;
	struct role_datum *r1, *r2;
	struct mls_level *l1, *l2;
	struct constraint_expr *e;
	int s[CEXPR_MAXDEPTH];
	int sp = -1;

	for (e = cexpr; e; e = e->next) {
		switch (e->expr_type) {
		case CEXPR_NOT:
			BUG_ON(sp < 0);
			s[sp] = !s[sp];
			break;
		case CEXPR_AND:
			BUG_ON(sp < 1);
			sp--;
			s[sp] &= s[sp + 1];
			break;
		case CEXPR_OR:
			BUG_ON(sp < 1);
			sp--;
			s[sp] |= s[sp + 1];
			break;
		case CEXPR_ATTR:
			if (sp == (CEXPR_MAXDEPTH - 1))
				return 0;
			switch (e->attr) {
			case CEXPR_USER:
				val1 = scontext->user;
				val2 = tcontext->user;
				break;
			case CEXPR_TYPE:
				val1 = scontext->type;
				val2 = tcontext->type;
				break;
			case CEXPR_ROLE:
				val1 = scontext->role;
				val2 = tcontext->role;
<<<<<<< HEAD
				r1 = policydb.role_val_to_struct[val1 - 1];
				r2 = policydb.role_val_to_struct[val2 - 1];
=======
				r1 = policydb->role_val_to_struct[val1 - 1];
				r2 = policydb->role_val_to_struct[val2 - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				switch (e->op) {
				case CEXPR_DOM:
					s[++sp] = ebitmap_get_bit(&r1->dominates,
								  val2 - 1);
					continue;
				case CEXPR_DOMBY:
					s[++sp] = ebitmap_get_bit(&r2->dominates,
								  val1 - 1);
					continue;
				case CEXPR_INCOMP:
					s[++sp] = (!ebitmap_get_bit(&r1->dominates,
								    val2 - 1) &&
						   !ebitmap_get_bit(&r2->dominates,
								    val1 - 1));
					continue;
				default:
					break;
				}
				break;
			case CEXPR_L1L2:
				l1 = &(scontext->range.level[0]);
				l2 = &(tcontext->range.level[0]);
				goto mls_ops;
			case CEXPR_L1H2:
				l1 = &(scontext->range.level[0]);
				l2 = &(tcontext->range.level[1]);
				goto mls_ops;
			case CEXPR_H1L2:
				l1 = &(scontext->range.level[1]);
				l2 = &(tcontext->range.level[0]);
				goto mls_ops;
			case CEXPR_H1H2:
				l1 = &(scontext->range.level[1]);
				l2 = &(tcontext->range.level[1]);
				goto mls_ops;
			case CEXPR_L1H1:
				l1 = &(scontext->range.level[0]);
				l2 = &(scontext->range.level[1]);
				goto mls_ops;
			case CEXPR_L2H2:
				l1 = &(tcontext->range.level[0]);
				l2 = &(tcontext->range.level[1]);
				goto mls_ops;
mls_ops:
<<<<<<< HEAD
			switch (e->op) {
			case CEXPR_EQ:
				s[++sp] = mls_level_eq(l1, l2);
				continue;
			case CEXPR_NEQ:
				s[++sp] = !mls_level_eq(l1, l2);
				continue;
			case CEXPR_DOM:
				s[++sp] = mls_level_dom(l1, l2);
				continue;
			case CEXPR_DOMBY:
				s[++sp] = mls_level_dom(l2, l1);
				continue;
			case CEXPR_INCOMP:
				s[++sp] = mls_level_incomp(l2, l1);
				continue;
			default:
				BUG();
				return 0;
			}
			break;
=======
				switch (e->op) {
				case CEXPR_EQ:
					s[++sp] = mls_level_eq(l1, l2);
					continue;
				case CEXPR_NEQ:
					s[++sp] = !mls_level_eq(l1, l2);
					continue;
				case CEXPR_DOM:
					s[++sp] = mls_level_dom(l1, l2);
					continue;
				case CEXPR_DOMBY:
					s[++sp] = mls_level_dom(l2, l1);
					continue;
				case CEXPR_INCOMP:
					s[++sp] = mls_level_incomp(l2, l1);
					continue;
				default:
					BUG();
					return 0;
				}
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			default:
				BUG();
				return 0;
			}

			switch (e->op) {
			case CEXPR_EQ:
				s[++sp] = (val1 == val2);
				break;
			case CEXPR_NEQ:
				s[++sp] = (val1 != val2);
				break;
			default:
				BUG();
				return 0;
			}
			break;
		case CEXPR_NAMES:
			if (sp == (CEXPR_MAXDEPTH-1))
				return 0;
			c = scontext;
			if (e->attr & CEXPR_TARGET)
				c = tcontext;
			else if (e->attr & CEXPR_XTARGET) {
				c = xcontext;
				if (!c) {
					BUG();
					return 0;
				}
			}
			if (e->attr & CEXPR_USER)
				val1 = c->user;
			else if (e->attr & CEXPR_ROLE)
				val1 = c->role;
			else if (e->attr & CEXPR_TYPE)
				val1 = c->type;
			else {
				BUG();
				return 0;
			}

			switch (e->op) {
			case CEXPR_EQ:
				s[++sp] = ebitmap_get_bit(&e->names, val1 - 1);
				break;
			case CEXPR_NEQ:
				s[++sp] = !ebitmap_get_bit(&e->names, val1 - 1);
				break;
			default:
				BUG();
				return 0;
			}
			break;
		default:
			BUG();
			return 0;
		}
	}

	BUG_ON(sp != 0);
	return s[0];
}

/*
 * security_dump_masked_av - dumps masked permissions during
 * security_compute_av due to RBAC, MLS/Constraint and Type bounds.
 */
static int dump_masked_av_helper(void *k, void *d, void *args)
{
	struct perm_datum *pdatum = d;
	char **permission_names = args;

	BUG_ON(pdatum->value < 1 || pdatum->value > 32);

	permission_names[pdatum->value - 1] = (char *)k;

	return 0;
}

<<<<<<< HEAD
static void security_dump_masked_av(struct context *scontext,
=======
static void security_dump_masked_av(struct policydb *policydb,
				    struct context *scontext,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct context *tcontext,
				    u16 tclass,
				    u32 permissions,
				    const char *reason)
{
	struct common_datum *common_dat;
	struct class_datum *tclass_dat;
	struct audit_buffer *ab;
	char *tclass_name;
	char *scontext_name = NULL;
	char *tcontext_name = NULL;
	char *permission_names[32];
	int index;
	u32 length;
	bool need_comma = false;

	if (!permissions)
		return;

<<<<<<< HEAD
	tclass_name = sym_name(&policydb, SYM_CLASSES, tclass - 1);
	tclass_dat = policydb.class_val_to_struct[tclass - 1];
=======
	tclass_name = sym_name(policydb, SYM_CLASSES, tclass - 1);
	tclass_dat = policydb->class_val_to_struct[tclass - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	common_dat = tclass_dat->comdatum;

	/* init permission_names */
	if (common_dat &&
<<<<<<< HEAD
	    hashtab_map(common_dat->permissions.table,
			dump_masked_av_helper, permission_names) < 0)
		goto out;

	if (hashtab_map(tclass_dat->permissions.table,
=======
	    hashtab_map(&common_dat->permissions.table,
			dump_masked_av_helper, permission_names) < 0)
		goto out;

	if (hashtab_map(&tclass_dat->permissions.table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dump_masked_av_helper, permission_names) < 0)
		goto out;

	/* get scontext/tcontext in text form */
<<<<<<< HEAD
	if (context_struct_to_string(scontext,
				     &scontext_name, &length) < 0)
		goto out;

	if (context_struct_to_string(tcontext,
=======
	if (context_struct_to_string(policydb, scontext,
				     &scontext_name, &length) < 0)
		goto out;

	if (context_struct_to_string(policydb, tcontext,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     &tcontext_name, &length) < 0)
		goto out;

	/* audit a message */
<<<<<<< HEAD
	ab = audit_log_start(current->audit_context,
=======
	ab = audit_log_start(audit_context(),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     GFP_ATOMIC, AUDIT_SELINUX_ERR);
	if (!ab)
		goto out;

	audit_log_format(ab, "op=security_compute_av reason=%s "
			 "scontext=%s tcontext=%s tclass=%s perms=",
			 reason, scontext_name, tcontext_name, tclass_name);

	for (index = 0; index < 32; index++) {
		u32 mask = (1 << index);

		if ((mask & permissions) == 0)
			continue;

		audit_log_format(ab, "%s%s",
				 need_comma ? "," : "",
				 permission_names[index]
				 ? permission_names[index] : "????");
		need_comma = true;
	}
	audit_log_end(ab);
out:
	/* release scontext/tcontext */
	kfree(tcontext_name);
	kfree(scontext_name);
<<<<<<< HEAD

	return;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * security_boundary_permission - drops violated permissions
 * on boundary constraint.
 */
<<<<<<< HEAD
static void type_attribute_bounds_av(struct context *scontext,
=======
static void type_attribute_bounds_av(struct policydb *policydb,
				     struct context *scontext,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     struct context *tcontext,
				     u16 tclass,
				     struct av_decision *avd)
{
	struct context lo_scontext;
<<<<<<< HEAD
	struct context lo_tcontext;
=======
	struct context lo_tcontext, *tcontextp = tcontext;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct av_decision lo_avd;
	struct type_datum *source;
	struct type_datum *target;
	u32 masked = 0;

<<<<<<< HEAD
	source = flex_array_get_ptr(policydb.type_val_to_struct_array,
				    scontext->type - 1);
	BUG_ON(!source);

	target = flex_array_get_ptr(policydb.type_val_to_struct_array,
				    tcontext->type - 1);
	BUG_ON(!target);

	if (source->bounds) {
		memset(&lo_avd, 0, sizeof(lo_avd));

		memcpy(&lo_scontext, scontext, sizeof(lo_scontext));
		lo_scontext.type = source->bounds;

		context_struct_compute_av(&lo_scontext,
					  tcontext,
					  tclass,
					  &lo_avd,
					  NULL);
		if ((lo_avd.allowed & avd->allowed) == avd->allowed)
			return;		/* no masked permission */
		masked = ~lo_avd.allowed & avd->allowed;
	}

	if (target->bounds) {
		memset(&lo_avd, 0, sizeof(lo_avd));

		memcpy(&lo_tcontext, tcontext, sizeof(lo_tcontext));
		lo_tcontext.type = target->bounds;

		context_struct_compute_av(scontext,
					  &lo_tcontext,
					  tclass,
					  &lo_avd,
					  NULL);
		if ((lo_avd.allowed & avd->allowed) == avd->allowed)
			return;		/* no masked permission */
		masked = ~lo_avd.allowed & avd->allowed;
	}

	if (source->bounds && target->bounds) {
		memset(&lo_avd, 0, sizeof(lo_avd));
		/*
		 * lo_scontext and lo_tcontext are already
		 * set up.
		 */

		context_struct_compute_av(&lo_scontext,
					  &lo_tcontext,
					  tclass,
					  &lo_avd,
					  NULL);
		if ((lo_avd.allowed & avd->allowed) == avd->allowed)
			return;		/* no masked permission */
		masked = ~lo_avd.allowed & avd->allowed;
	}

	if (masked) {
		/* mask violated permissions */
		avd->allowed &= ~masked;

		/* audit masked permissions */
		security_dump_masked_av(scontext, tcontext,
					tclass, masked, "bounds");
	}
=======
	source = policydb->type_val_to_struct[scontext->type - 1];
	BUG_ON(!source);

	if (!source->bounds)
		return;

	target = policydb->type_val_to_struct[tcontext->type - 1];
	BUG_ON(!target);

	memset(&lo_avd, 0, sizeof(lo_avd));

	memcpy(&lo_scontext, scontext, sizeof(lo_scontext));
	lo_scontext.type = source->bounds;

	if (target->bounds) {
		memcpy(&lo_tcontext, tcontext, sizeof(lo_tcontext));
		lo_tcontext.type = target->bounds;
		tcontextp = &lo_tcontext;
	}

	context_struct_compute_av(policydb, &lo_scontext,
				  tcontextp,
				  tclass,
				  &lo_avd,
				  NULL);

	masked = ~lo_avd.allowed & avd->allowed;

	if (likely(!masked))
		return;		/* no masked permission */

	/* mask violated permissions */
	avd->allowed &= ~masked;

	/* audit masked permissions */
	security_dump_masked_av(policydb, scontext, tcontext,
				tclass, masked, "bounds");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * flag which drivers have permissions
<<<<<<< HEAD
 * only looking for ioctl based extended permssions
=======
 * only looking for ioctl based extended permissions
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void services_compute_xperms_drivers(
		struct extended_perms *xperms,
		struct avtab_node *node)
{
	unsigned int i;

	if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLDRIVER) {
		/* if one or more driver has all permissions allowed */
		for (i = 0; i < ARRAY_SIZE(xperms->drivers.p); i++)
			xperms->drivers.p[i] |= node->datum.u.xperms->perms.p[i];
	} else if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLFUNCTION) {
		/* if allowing permissions within a driver */
		security_xperm_set(xperms->drivers.p,
					node->datum.u.xperms->driver);
	}

<<<<<<< HEAD
	/* If no ioctl commands are allowed, ignore auditallow and auditdeny */
	if (node->key.specified & AVTAB_XPERMS_ALLOWED)
		xperms->len = 1;
=======
	xperms->len = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Compute access vectors and extended permissions based on a context
 * structure pair for the permissions in a particular class.
 */
<<<<<<< HEAD
static void context_struct_compute_av(struct context *scontext,
					struct context *tcontext,
					u16 tclass,
					struct av_decision *avd,
					struct extended_perms *xperms)
=======
static void context_struct_compute_av(struct policydb *policydb,
				      struct context *scontext,
				      struct context *tcontext,
				      u16 tclass,
				      struct av_decision *avd,
				      struct extended_perms *xperms)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct constraint_node *constraint;
	struct role_allow *ra;
	struct avtab_key avkey;
	struct avtab_node *node;
	struct class_datum *tclass_datum;
	struct ebitmap *sattr, *tattr;
	struct ebitmap_node *snode, *tnode;
	unsigned int i, j;

	avd->allowed = 0;
	avd->auditallow = 0;
	avd->auditdeny = 0xffffffff;
	if (xperms) {
		memset(&xperms->drivers, 0, sizeof(xperms->drivers));
		xperms->len = 0;
	}

<<<<<<< HEAD
	if (unlikely(!tclass || tclass > policydb.p_classes.nprim)) {
		if (printk_ratelimit())
			printk(KERN_WARNING "SELinux:  Invalid class %hu\n", tclass);
		return;
	}

	tclass_datum = policydb.class_val_to_struct[tclass - 1];
=======
	if (unlikely(!tclass || tclass > policydb->p_classes.nprim)) {
		if (printk_ratelimit())
			pr_warn("SELinux:  Invalid class %hu\n", tclass);
		return;
	}

	tclass_datum = policydb->class_val_to_struct[tclass - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If a specific type enforcement rule was defined for
	 * this permission check, then use it.
	 */
	avkey.target_class = tclass;
	avkey.specified = AVTAB_AV | AVTAB_XPERMS;
<<<<<<< HEAD
	sattr = flex_array_get(policydb.type_attr_map_array, scontext->type - 1);
	BUG_ON(!sattr);
	tattr = flex_array_get(policydb.type_attr_map_array, tcontext->type - 1);
	BUG_ON(!tattr);
=======
	sattr = &policydb->type_attr_map_array[scontext->type - 1];
	tattr = &policydb->type_attr_map_array[tcontext->type - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ebitmap_for_each_positive_bit(sattr, snode, i) {
		ebitmap_for_each_positive_bit(tattr, tnode, j) {
			avkey.source_type = i + 1;
			avkey.target_type = j + 1;
<<<<<<< HEAD
			for (node = avtab_search_node(&policydb.te_avtab, &avkey);
=======
			for (node = avtab_search_node(&policydb->te_avtab,
						      &avkey);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     node;
			     node = avtab_search_node_next(node, avkey.specified)) {
				if (node->key.specified == AVTAB_ALLOWED)
					avd->allowed |= node->datum.u.data;
				else if (node->key.specified == AVTAB_AUDITALLOW)
					avd->auditallow |= node->datum.u.data;
				else if (node->key.specified == AVTAB_AUDITDENY)
					avd->auditdeny &= node->datum.u.data;
				else if (xperms && (node->key.specified & AVTAB_XPERMS))
					services_compute_xperms_drivers(xperms, node);
			}

			/* Check conditional av table for additional permissions */
<<<<<<< HEAD
			cond_compute_av(&policydb.te_cond_avtab, &avkey,
=======
			cond_compute_av(&policydb->te_cond_avtab, &avkey,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					avd, xperms);

		}
	}

	/*
	 * Remove any permissions prohibited by a constraint (this includes
	 * the MLS policy).
	 */
	constraint = tclass_datum->constraints;
	while (constraint) {
		if ((constraint->permissions & (avd->allowed)) &&
<<<<<<< HEAD
		    !constraint_expr_eval(scontext, tcontext, NULL,
=======
		    !constraint_expr_eval(policydb, scontext, tcontext, NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  constraint->expr)) {
			avd->allowed &= ~(constraint->permissions);
		}
		constraint = constraint->next;
	}

	/*
	 * If checking process transition permission and the
	 * role is changing, then check the (current_role, new_role)
	 * pair.
	 */
<<<<<<< HEAD
	if (tclass == policydb.process_class &&
	    (avd->allowed & policydb.process_trans_perms) &&
	    scontext->role != tcontext->role) {
		for (ra = policydb.role_allow; ra; ra = ra->next) {
=======
	if (tclass == policydb->process_class &&
	    (avd->allowed & policydb->process_trans_perms) &&
	    scontext->role != tcontext->role) {
		for (ra = policydb->role_allow; ra; ra = ra->next) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (scontext->role == ra->role &&
			    tcontext->role == ra->new_role)
				break;
		}
		if (!ra)
<<<<<<< HEAD
			avd->allowed &= ~policydb.process_trans_perms;
=======
			avd->allowed &= ~policydb->process_trans_perms;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * If the given source and target types have boundary
	 * constraint, lazy checks have to mask any violated
	 * permission and notice it to userspace via audit.
	 */
<<<<<<< HEAD
	type_attribute_bounds_av(scontext, tcontext,
				 tclass, avd);
}

static int security_validtrans_handle_fail(struct context *ocontext,
					   struct context *ncontext,
					   struct context *tcontext,
					   u16 tclass)
{
	char *o = NULL, *n = NULL, *t = NULL;
	u32 olen, nlen, tlen;

	if (context_struct_to_string(ocontext, &o, &olen))
		goto out;
	if (context_struct_to_string(ncontext, &n, &nlen))
		goto out;
	if (context_struct_to_string(tcontext, &t, &tlen))
		goto out;
	audit_log(current->audit_context, GFP_ATOMIC, AUDIT_SELINUX_ERR,
		  "security_validate_transition:  denied for"
		  " oldcontext=%s newcontext=%s taskcontext=%s tclass=%s",
		  o, n, t, sym_name(&policydb, SYM_CLASSES, tclass-1));
=======
	type_attribute_bounds_av(policydb, scontext, tcontext,
				 tclass, avd);
}

static int security_validtrans_handle_fail(struct selinux_policy *policy,
					struct sidtab_entry *oentry,
					struct sidtab_entry *nentry,
					struct sidtab_entry *tentry,
					u16 tclass)
{
	struct policydb *p = &policy->policydb;
	struct sidtab *sidtab = policy->sidtab;
	char *o = NULL, *n = NULL, *t = NULL;
	u32 olen, nlen, tlen;

	if (sidtab_entry_to_string(p, sidtab, oentry, &o, &olen))
		goto out;
	if (sidtab_entry_to_string(p, sidtab, nentry, &n, &nlen))
		goto out;
	if (sidtab_entry_to_string(p, sidtab, tentry, &t, &tlen))
		goto out;
	audit_log(audit_context(), GFP_ATOMIC, AUDIT_SELINUX_ERR,
		  "op=security_validate_transition seresult=denied"
		  " oldcontext=%s newcontext=%s taskcontext=%s tclass=%s",
		  o, n, t, sym_name(p, SYM_CLASSES, tclass-1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(o);
	kfree(n);
	kfree(t);

<<<<<<< HEAD
	if (!selinux_enforcing)
=======
	if (!enforcing_enabled())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	return -EPERM;
}

<<<<<<< HEAD
int security_validate_transition(u32 oldsid, u32 newsid, u32 tasksid,
				 u16 orig_tclass)
{
	struct context *ocontext;
	struct context *ncontext;
	struct context *tcontext;
=======
static int security_compute_validatetrans(u32 oldsid, u32 newsid, u32 tasksid,
					  u16 orig_tclass, bool user)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct sidtab_entry *oentry;
	struct sidtab_entry *nentry;
	struct sidtab_entry *tentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct class_datum *tclass_datum;
	struct constraint_node *constraint;
	u16 tclass;
	int rc = 0;

<<<<<<< HEAD
	if (!ss_initialized)
		return 0;

	read_lock(&policy_rwlock);

	tclass = unmap_class(orig_tclass);

	if (!tclass || tclass > policydb.p_classes.nprim) {
		printk(KERN_ERR "SELinux: %s:  unrecognized class %d\n",
			__func__, tclass);
		rc = -EINVAL;
		goto out;
	}
	tclass_datum = policydb.class_val_to_struct[tclass - 1];

	ocontext = sidtab_search(&sidtab, oldsid);
	if (!ocontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();

	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	if (!user)
		tclass = unmap_class(&policy->map, orig_tclass);
	else
		tclass = orig_tclass;

	if (!tclass || tclass > policydb->p_classes.nprim) {
		rc = -EINVAL;
		goto out;
	}
	tclass_datum = policydb->class_val_to_struct[tclass - 1];

	oentry = sidtab_search_entry(sidtab, oldsid);
	if (!oentry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, oldsid);
		rc = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	ncontext = sidtab_search(&sidtab, newsid);
	if (!ncontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	nentry = sidtab_search_entry(sidtab, newsid);
	if (!nentry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, newsid);
		rc = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	tcontext = sidtab_search(&sidtab, tasksid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	tentry = sidtab_search_entry(sidtab, tasksid);
	if (!tentry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, tasksid);
		rc = -EINVAL;
		goto out;
	}

	constraint = tclass_datum->validatetrans;
	while (constraint) {
<<<<<<< HEAD
		if (!constraint_expr_eval(ocontext, ncontext, tcontext,
					  constraint->expr)) {
			rc = security_validtrans_handle_fail(ocontext, ncontext,
							     tcontext, tclass);
=======
		if (!constraint_expr_eval(policydb, &oentry->context,
					  &nentry->context, &tentry->context,
					  constraint->expr)) {
			if (user)
				rc = -EPERM;
			else
				rc = security_validtrans_handle_fail(policy,
								oentry,
								nentry,
								tentry,
								tclass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}
		constraint = constraint->next;
	}

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
	return rc;
}

=======
	rcu_read_unlock();
	return rc;
}

int security_validate_transition_user(u32 oldsid, u32 newsid, u32 tasksid,
				      u16 tclass)
{
	return security_compute_validatetrans(oldsid, newsid, tasksid,
					      tclass, true);
}

int security_validate_transition(u32 oldsid, u32 newsid, u32 tasksid,
				 u16 orig_tclass)
{
	return security_compute_validatetrans(oldsid, newsid, tasksid,
					      orig_tclass, false);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * security_bounded_transition - check whether the given
 * transition is directed to bounded, or not.
 * It returns 0, if @newsid is bounded by @oldsid.
 * Otherwise, it returns error code.
 *
 * @oldsid : current security identifier
 * @newsid : destinated security identifier
 */
int security_bounded_transition(u32 old_sid, u32 new_sid)
{
<<<<<<< HEAD
	struct context *old_context, *new_context;
	struct type_datum *type;
	int index;
	int rc;

	read_lock(&policy_rwlock);

	rc = -EINVAL;
	old_context = sidtab_search(&sidtab, old_sid);
	if (!old_context) {
		printk(KERN_ERR "SELinux: %s: unrecognized SID %u\n",
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct sidtab_entry *old_entry, *new_entry;
	struct type_datum *type;
	u32 index;
	int rc;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	rc = -EINVAL;
	old_entry = sidtab_search_entry(sidtab, old_sid);
	if (!old_entry) {
		pr_err("SELinux: %s: unrecognized SID %u\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, old_sid);
		goto out;
	}

	rc = -EINVAL;
<<<<<<< HEAD
	new_context = sidtab_search(&sidtab, new_sid);
	if (!new_context) {
		printk(KERN_ERR "SELinux: %s: unrecognized SID %u\n",
=======
	new_entry = sidtab_search_entry(sidtab, new_sid);
	if (!new_entry) {
		pr_err("SELinux: %s: unrecognized SID %u\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, new_sid);
		goto out;
	}

	rc = 0;
	/* type/domain unchanged */
<<<<<<< HEAD
	if (old_context->type == new_context->type)
		goto out;

	index = new_context->type;
	while (true) {
		type = flex_array_get_ptr(policydb.type_val_to_struct_array,
					  index - 1);
=======
	if (old_entry->context.type == new_entry->context.type)
		goto out;

	index = new_entry->context.type;
	while (true) {
		type = policydb->type_val_to_struct[index - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG_ON(!type);

		/* not bounded anymore */
		rc = -EPERM;
		if (!type->bounds)
			break;

		/* @newsid is bounded by @oldsid */
		rc = 0;
<<<<<<< HEAD
		if (type->bounds == old_context->type)
=======
		if (type->bounds == old_entry->context.type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		index = type->bounds;
	}

	if (rc) {
		char *old_name = NULL;
		char *new_name = NULL;
		u32 length;

<<<<<<< HEAD
		if (!context_struct_to_string(old_context,
					      &old_name, &length) &&
		    !context_struct_to_string(new_context,
					      &new_name, &length)) {
			audit_log(current->audit_context,
				  GFP_ATOMIC, AUDIT_SELINUX_ERR,
				  "op=security_bounded_transition "
				  "result=denied "
=======
		if (!sidtab_entry_to_string(policydb, sidtab, old_entry,
					    &old_name, &length) &&
		    !sidtab_entry_to_string(policydb, sidtab, new_entry,
					    &new_name, &length)) {
			audit_log(audit_context(),
				  GFP_ATOMIC, AUDIT_SELINUX_ERR,
				  "op=security_bounded_transition "
				  "seresult=denied "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  "oldcontext=%s newcontext=%s",
				  old_name, new_name);
		}
		kfree(new_name);
		kfree(old_name);
	}
out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rc;
}

<<<<<<< HEAD
static void avd_init(struct av_decision *avd)
=======
static void avd_init(struct selinux_policy *policy, struct av_decision *avd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	avd->allowed = 0;
	avd->auditallow = 0;
	avd->auditdeny = 0xffffffff;
<<<<<<< HEAD
	avd->seqno = latest_granting;
=======
	if (policy)
		avd->seqno = policy->latest_granting;
	else
		avd->seqno = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	avd->flags = 0;
}

void services_compute_xperms_decision(struct extended_perms_decision *xpermd,
					struct avtab_node *node)
{
	unsigned int i;

	if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLFUNCTION) {
		if (xpermd->driver != node->datum.u.xperms->driver)
			return;
	} else if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLDRIVER) {
		if (!security_xperm_test(node->datum.u.xperms->perms.p,
					xpermd->driver))
			return;
	} else {
		BUG();
	}

	if (node->key.specified == AVTAB_XPERMS_ALLOWED) {
		xpermd->used |= XPERMS_ALLOWED;
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLDRIVER) {
			memset(xpermd->allowed->p, 0xff,
					sizeof(xpermd->allowed->p));
		}
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLFUNCTION) {
			for (i = 0; i < ARRAY_SIZE(xpermd->allowed->p); i++)
				xpermd->allowed->p[i] |=
					node->datum.u.xperms->perms.p[i];
		}
	} else if (node->key.specified == AVTAB_XPERMS_AUDITALLOW) {
		xpermd->used |= XPERMS_AUDITALLOW;
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLDRIVER) {
			memset(xpermd->auditallow->p, 0xff,
					sizeof(xpermd->auditallow->p));
		}
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLFUNCTION) {
			for (i = 0; i < ARRAY_SIZE(xpermd->auditallow->p); i++)
				xpermd->auditallow->p[i] |=
					node->datum.u.xperms->perms.p[i];
		}
	} else if (node->key.specified == AVTAB_XPERMS_DONTAUDIT) {
		xpermd->used |= XPERMS_DONTAUDIT;
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLDRIVER) {
			memset(xpermd->dontaudit->p, 0xff,
					sizeof(xpermd->dontaudit->p));
		}
		if (node->datum.u.xperms->specified == AVTAB_XPERMS_IOCTLFUNCTION) {
			for (i = 0; i < ARRAY_SIZE(xpermd->dontaudit->p); i++)
				xpermd->dontaudit->p[i] |=
					node->datum.u.xperms->perms.p[i];
		}
	} else {
		BUG();
	}
}

void security_compute_xperms_decision(u32 ssid,
<<<<<<< HEAD
				u32 tsid,
				u16 orig_tclass,
				u8 driver,
				struct extended_perms_decision *xpermd)
{
=======
				      u32 tsid,
				      u16 orig_tclass,
				      u8 driver,
				      struct extended_perms_decision *xpermd)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 tclass;
	struct context *scontext, *tcontext;
	struct avtab_key avkey;
	struct avtab_node *node;
	struct ebitmap *sattr, *tattr;
	struct ebitmap_node *snode, *tnode;
	unsigned int i, j;

	xpermd->driver = driver;
	xpermd->used = 0;
	memset(xpermd->allowed->p, 0, sizeof(xpermd->allowed->p));
	memset(xpermd->auditallow->p, 0, sizeof(xpermd->auditallow->p));
	memset(xpermd->dontaudit->p, 0, sizeof(xpermd->dontaudit->p));

<<<<<<< HEAD
	read_lock(&policy_rwlock);
	if (!ss_initialized)
		goto allow;

	scontext = sidtab_search(&sidtab, ssid);
	if (!scontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	rcu_read_lock();
	if (!selinux_initialized())
		goto allow;

	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	scontext = sidtab_search(sidtab, ssid);
	if (!scontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, ssid);
		goto out;
	}

<<<<<<< HEAD
	tcontext = sidtab_search(&sidtab, tsid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	tcontext = sidtab_search(sidtab, tsid);
	if (!tcontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, tsid);
		goto out;
	}

<<<<<<< HEAD
	tclass = unmap_class(orig_tclass);
	if (unlikely(orig_tclass && !tclass)) {
		if (policydb.allow_unknown)
=======
	tclass = unmap_class(&policy->map, orig_tclass);
	if (unlikely(orig_tclass && !tclass)) {
		if (policydb->allow_unknown)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto allow;
		goto out;
	}


<<<<<<< HEAD
	if (unlikely(!tclass || tclass > policydb.p_classes.nprim)) {
		if (printk_ratelimit())
                	printk(KERN_WARNING "SELinux:  Invalid class %hu\n", tclass);
=======
	if (unlikely(!tclass || tclass > policydb->p_classes.nprim)) {
		pr_warn_ratelimited("SELinux:  Invalid class %hu\n", tclass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	avkey.target_class = tclass;
	avkey.specified = AVTAB_XPERMS;
<<<<<<< HEAD
	sattr = flex_array_get(policydb.type_attr_map_array,
				scontext->type - 1);
	BUG_ON(!sattr);
	tattr = flex_array_get(policydb.type_attr_map_array,
				tcontext->type - 1);
	BUG_ON(!tattr);
=======
	sattr = &policydb->type_attr_map_array[scontext->type - 1];
	tattr = &policydb->type_attr_map_array[tcontext->type - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ebitmap_for_each_positive_bit(sattr, snode, i) {
		ebitmap_for_each_positive_bit(tattr, tnode, j) {
			avkey.source_type = i + 1;
			avkey.target_type = j + 1;
<<<<<<< HEAD
			for (node = avtab_search_node(&policydb.te_avtab, &avkey);
=======
			for (node = avtab_search_node(&policydb->te_avtab,
						      &avkey);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     node;
			     node = avtab_search_node_next(node, avkey.specified))
				services_compute_xperms_decision(xpermd, node);

<<<<<<< HEAD
			cond_compute_xperms(&policydb.te_cond_avtab,
=======
			cond_compute_xperms(&policydb->te_cond_avtab,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						&avkey, xpermd);
		}
	}
out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
allow:
	memset(xpermd->allowed->p, 0xff, sizeof(xpermd->allowed->p));
	goto out;
}

/**
 * security_compute_av - Compute access vector decisions.
 * @ssid: source security identifier
 * @tsid: target security identifier
<<<<<<< HEAD
 * @tclass: target security class
=======
 * @orig_tclass: target security class
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @avd: access vector decisions
 * @xperms: extended permissions
 *
 * Compute a set of access vector decisions based on the
 * SID pair (@ssid, @tsid) for the permissions in @tclass.
 */
void security_compute_av(u32 ssid,
			 u32 tsid,
			 u16 orig_tclass,
			 struct av_decision *avd,
			 struct extended_perms *xperms)
{
<<<<<<< HEAD
	u16 tclass;
	struct context *scontext = NULL, *tcontext = NULL;

	read_lock(&policy_rwlock);
	avd_init(avd);
	xperms->len = 0;
	if (!ss_initialized)
		goto allow;

	scontext = sidtab_search(&sidtab, ssid);
	if (!scontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	u16 tclass;
	struct context *scontext = NULL, *tcontext = NULL;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	avd_init(policy, avd);
	xperms->len = 0;
	if (!selinux_initialized())
		goto allow;

	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	scontext = sidtab_search(sidtab, ssid);
	if (!scontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, ssid);
		goto out;
	}

	/* permissive domain? */
<<<<<<< HEAD
	if (ebitmap_get_bit(&policydb.permissive_map, scontext->type))
		avd->flags |= AVD_FLAGS_PERMISSIVE;

	tcontext = sidtab_search(&sidtab, tsid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	if (ebitmap_get_bit(&policydb->permissive_map, scontext->type))
		avd->flags |= AVD_FLAGS_PERMISSIVE;

	tcontext = sidtab_search(sidtab, tsid);
	if (!tcontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, tsid);
		goto out;
	}

<<<<<<< HEAD
	tclass = unmap_class(orig_tclass);
	if (unlikely(orig_tclass && !tclass)) {
		if (policydb.allow_unknown)
			goto allow;
		goto out;
	}
	context_struct_compute_av(scontext, tcontext, tclass, avd, xperms);
	map_decision(orig_tclass, avd, policydb.allow_unknown);
out:
	read_unlock(&policy_rwlock);
=======
	tclass = unmap_class(&policy->map, orig_tclass);
	if (unlikely(orig_tclass && !tclass)) {
		if (policydb->allow_unknown)
			goto allow;
		goto out;
	}
	context_struct_compute_av(policydb, scontext, tcontext, tclass, avd,
				  xperms);
	map_decision(&policy->map, orig_tclass, avd,
		     policydb->allow_unknown);
out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
allow:
	avd->allowed = 0xffffffff;
	goto out;
}

void security_compute_av_user(u32 ssid,
			      u32 tsid,
			      u16 tclass,
			      struct av_decision *avd)
{
<<<<<<< HEAD
	struct context *scontext = NULL, *tcontext = NULL;

	read_lock(&policy_rwlock);
	avd_init(avd);
	if (!ss_initialized)
		goto allow;

	scontext = sidtab_search(&sidtab, ssid);
	if (!scontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct context *scontext = NULL, *tcontext = NULL;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	avd_init(policy, avd);
	if (!selinux_initialized())
		goto allow;

	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	scontext = sidtab_search(sidtab, ssid);
	if (!scontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, ssid);
		goto out;
	}

	/* permissive domain? */
<<<<<<< HEAD
	if (ebitmap_get_bit(&policydb.permissive_map, scontext->type))
		avd->flags |= AVD_FLAGS_PERMISSIVE;

	tcontext = sidtab_search(&sidtab, tsid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	if (ebitmap_get_bit(&policydb->permissive_map, scontext->type))
		avd->flags |= AVD_FLAGS_PERMISSIVE;

	tcontext = sidtab_search(sidtab, tsid);
	if (!tcontext) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, tsid);
		goto out;
	}

	if (unlikely(!tclass)) {
<<<<<<< HEAD
		if (policydb.allow_unknown)
=======
		if (policydb->allow_unknown)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto allow;
		goto out;
	}

<<<<<<< HEAD
	context_struct_compute_av(scontext, tcontext, tclass, avd, NULL);
 out:
	read_unlock(&policy_rwlock);
=======
	context_struct_compute_av(policydb, scontext, tcontext, tclass, avd,
				  NULL);
 out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
allow:
	avd->allowed = 0xffffffff;
	goto out;
}

/*
 * Write the security context string representation of
 * the context structure `context' into a dynamically
 * allocated string of the correct size.  Set `*scontext'
 * to point to this string and set `*scontext_len' to
 * the length of the string.
 */
<<<<<<< HEAD
static int context_struct_to_string(struct context *context, char **scontext, u32 *scontext_len)
=======
static int context_struct_to_string(struct policydb *p,
				    struct context *context,
				    char **scontext, u32 *scontext_len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *scontextp;

	if (scontext)
		*scontext = NULL;
	*scontext_len = 0;

	if (context->len) {
		*scontext_len = context->len;
<<<<<<< HEAD
		*scontext = kstrdup(context->str, GFP_ATOMIC);
		if (!(*scontext))
			return -ENOMEM;
=======
		if (scontext) {
			*scontext = kstrdup(context->str, GFP_ATOMIC);
			if (!(*scontext))
				return -ENOMEM;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	/* Compute the size of the context. */
<<<<<<< HEAD
	*scontext_len += strlen(sym_name(&policydb, SYM_USERS, context->user - 1)) + 1;
	*scontext_len += strlen(sym_name(&policydb, SYM_ROLES, context->role - 1)) + 1;
	*scontext_len += strlen(sym_name(&policydb, SYM_TYPES, context->type - 1)) + 1;
	*scontext_len += mls_compute_context_len(context);
=======
	*scontext_len += strlen(sym_name(p, SYM_USERS, context->user - 1)) + 1;
	*scontext_len += strlen(sym_name(p, SYM_ROLES, context->role - 1)) + 1;
	*scontext_len += strlen(sym_name(p, SYM_TYPES, context->type - 1)) + 1;
	*scontext_len += mls_compute_context_len(p, context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!scontext)
		return 0;

	/* Allocate space for the context; caller must free this space. */
	scontextp = kmalloc(*scontext_len, GFP_ATOMIC);
	if (!scontextp)
		return -ENOMEM;
	*scontext = scontextp;

	/*
	 * Copy the user name, role name and type name into the context.
	 */
<<<<<<< HEAD
	sprintf(scontextp, "%s:%s:%s",
		sym_name(&policydb, SYM_USERS, context->user - 1),
		sym_name(&policydb, SYM_ROLES, context->role - 1),
		sym_name(&policydb, SYM_TYPES, context->type - 1));
	scontextp += strlen(sym_name(&policydb, SYM_USERS, context->user - 1)) +
		     1 + strlen(sym_name(&policydb, SYM_ROLES, context->role - 1)) +
		     1 + strlen(sym_name(&policydb, SYM_TYPES, context->type - 1));

	mls_sid_to_context(context, &scontextp);
=======
	scontextp += sprintf(scontextp, "%s:%s:%s",
		sym_name(p, SYM_USERS, context->user - 1),
		sym_name(p, SYM_ROLES, context->role - 1),
		sym_name(p, SYM_TYPES, context->type - 1));

	mls_sid_to_context(p, context, &scontextp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*scontextp = 0;

	return 0;
}

<<<<<<< HEAD
#include "initial_sid_to_string.h"

=======
static int sidtab_entry_to_string(struct policydb *p,
				  struct sidtab *sidtab,
				  struct sidtab_entry *entry,
				  char **scontext, u32 *scontext_len)
{
	int rc = sidtab_sid2str_get(sidtab, entry, scontext, scontext_len);

	if (rc != -ENOENT)
		return rc;

	rc = context_struct_to_string(p, &entry->context, scontext,
				      scontext_len);
	if (!rc && scontext)
		sidtab_sid2str_put(sidtab, entry, *scontext, *scontext_len);
	return rc;
}

#include "initial_sid_to_string.h"

int security_sidtab_hash_stats(char *page)
{
	struct selinux_policy *policy;
	int rc;

	if (!selinux_initialized()) {
		pr_err("SELinux: %s:  called before initial load_policy\n",
		       __func__);
		return -EINVAL;
	}

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	rc = sidtab_hash_stats(policy->sidtab, page);
	rcu_read_unlock();

	return rc;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const char *security_get_initial_sid_context(u32 sid)
{
	if (unlikely(sid > SECINITSID_NUM))
		return NULL;
	return initial_sid_to_string[sid];
}

static int security_sid_to_context_core(u32 sid, char **scontext,
<<<<<<< HEAD
					u32 *scontext_len, int force)
{
	struct context *context;
=======
					u32 *scontext_len, int force,
					int only_invalid)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct sidtab_entry *entry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc = 0;

	if (scontext)
		*scontext = NULL;
	*scontext_len  = 0;

<<<<<<< HEAD
	if (!ss_initialized) {
		if (sid <= SECINITSID_NUM) {
			char *scontextp;

			*scontext_len = strlen(initial_sid_to_string[sid]) + 1;
			if (!scontext)
				goto out;
			scontextp = kmalloc(*scontext_len, GFP_ATOMIC);
			if (!scontextp) {
				rc = -ENOMEM;
				goto out;
			}
			strcpy(scontextp, initial_sid_to_string[sid]);
			*scontext = scontextp;
			goto out;
		}
		printk(KERN_ERR "SELinux: %s:  called before initial "
		       "load_policy on unknown SID %d\n", __func__, sid);
		rc = -EINVAL;
		goto out;
	}
	read_lock(&policy_rwlock);
	if (force)
		context = sidtab_search_force(&sidtab, sid);
	else
		context = sidtab_search(&sidtab, sid);
	if (!context) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	if (!selinux_initialized()) {
		if (sid <= SECINITSID_NUM) {
			char *scontextp;
			const char *s;

			/*
			 * Before the policy is loaded, translate
			 * SECINITSID_INIT to "kernel", because systemd and
			 * libselinux < 2.6 take a getcon_raw() result that is
			 * both non-null and not "kernel" to mean that a policy
			 * is already loaded.
			 */
			if (sid == SECINITSID_INIT)
				sid = SECINITSID_KERNEL;

			s = initial_sid_to_string[sid];
			if (!s)
				return -EINVAL;
			*scontext_len = strlen(s) + 1;
			if (!scontext)
				return 0;
			scontextp = kmemdup(s, *scontext_len, GFP_ATOMIC);
			if (!scontextp)
				return -ENOMEM;
			*scontext = scontextp;
			return 0;
		}
		pr_err("SELinux: %s:  called before initial "
		       "load_policy on unknown SID %d\n", __func__, sid);
		return -EINVAL;
	}
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	if (force)
		entry = sidtab_search_entry_force(sidtab, sid);
	else
		entry = sidtab_search_entry(sidtab, sid);
	if (!entry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, sid);
		rc = -EINVAL;
		goto out_unlock;
	}
<<<<<<< HEAD
	rc = context_struct_to_string(context, scontext, scontext_len);
out_unlock:
	read_unlock(&policy_rwlock);
out:
=======
	if (only_invalid && !entry->context.len)
		goto out_unlock;

	rc = sidtab_entry_to_string(policydb, sidtab, entry, scontext,
				    scontext_len);

out_unlock:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;

}

/**
 * security_sid_to_context - Obtain a context for a given SID.
 * @sid: security identifier, SID
 * @scontext: security context
 * @scontext_len: length in bytes
 *
 * Write the string representation of the context associated with @sid
 * into a dynamically allocated string of the correct size.  Set @scontext
 * to point to this string and set @scontext_len to the length of the string.
 */
int security_sid_to_context(u32 sid, char **scontext, u32 *scontext_len)
{
<<<<<<< HEAD
	return security_sid_to_context_core(sid, scontext, scontext_len, 0);
}

int security_sid_to_context_force(u32 sid, char **scontext, u32 *scontext_len)
{
	return security_sid_to_context_core(sid, scontext, scontext_len, 1);
=======
	return security_sid_to_context_core(sid, scontext,
					    scontext_len, 0, 0);
}

int security_sid_to_context_force(u32 sid,
				  char **scontext, u32 *scontext_len)
{
	return security_sid_to_context_core(sid, scontext,
					    scontext_len, 1, 0);
}

/**
 * security_sid_to_context_inval - Obtain a context for a given SID if it
 *                                 is invalid.
 * @sid: security identifier, SID
 * @scontext: security context
 * @scontext_len: length in bytes
 *
 * Write the string representation of the context associated with @sid
 * into a dynamically allocated string of the correct size, but only if the
 * context is invalid in the current policy.  Set @scontext to point to
 * this string (or NULL if the context is valid) and set @scontext_len to
 * the length of the string (or 0 if the context is valid).
 */
int security_sid_to_context_inval(u32 sid,
				  char **scontext, u32 *scontext_len)
{
	return security_sid_to_context_core(sid, scontext,
					    scontext_len, 1, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Caveat:  Mutates scontext.
 */
static int string_to_context_struct(struct policydb *pol,
				    struct sidtab *sidtabp,
				    char *scontext,
<<<<<<< HEAD
				    u32 scontext_len,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct context *ctx,
				    u32 def_sid)
{
	struct role_datum *role;
	struct type_datum *typdatum;
	struct user_datum *usrdatum;
	char *scontextp, *p, oldc;
	int rc = 0;

	context_init(ctx);

	/* Parse the security context. */

	rc = -EINVAL;
<<<<<<< HEAD
	scontextp = (char *) scontext;
=======
	scontextp = scontext;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Extract the user. */
	p = scontextp;
	while (*p && *p != ':')
		p++;

	if (*p == 0)
		goto out;

	*p++ = 0;

<<<<<<< HEAD
	usrdatum = hashtab_search(pol->p_users.table, scontextp);
=======
	usrdatum = symtab_search(&pol->p_users, scontextp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!usrdatum)
		goto out;

	ctx->user = usrdatum->value;

	/* Extract role. */
	scontextp = p;
	while (*p && *p != ':')
		p++;

	if (*p == 0)
		goto out;

	*p++ = 0;

<<<<<<< HEAD
	role = hashtab_search(pol->p_roles.table, scontextp);
=======
	role = symtab_search(&pol->p_roles, scontextp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!role)
		goto out;
	ctx->role = role->value;

	/* Extract type. */
	scontextp = p;
	while (*p && *p != ':')
		p++;
	oldc = *p;
	*p++ = 0;

<<<<<<< HEAD
	typdatum = hashtab_search(pol->p_types.table, scontextp);
=======
	typdatum = symtab_search(&pol->p_types, scontextp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!typdatum || typdatum->attribute)
		goto out;

	ctx->type = typdatum->value;

<<<<<<< HEAD
	rc = mls_context_to_sid(pol, oldc, &p, ctx, sidtabp, def_sid);
	if (rc)
		goto out;

	rc = -EINVAL;
	if ((p - scontext) < scontext_len)
		goto out;

	/* Check the validity of the new context. */
=======
	rc = mls_context_to_sid(pol, oldc, p, ctx, sidtabp, def_sid);
	if (rc)
		goto out;

	/* Check the validity of the new context. */
	rc = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!policydb_context_isvalid(pol, ctx))
		goto out;
	rc = 0;
out:
	if (rc)
		context_destroy(ctx);
	return rc;
}

static int security_context_to_sid_core(const char *scontext, u32 scontext_len,
					u32 *sid, u32 def_sid, gfp_t gfp_flags,
					int force)
{
<<<<<<< HEAD
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *scontext2, *str = NULL;
	struct context context;
	int rc = 0;

	/* An empty security context is never valid. */
	if (!scontext_len)
		return -EINVAL;

<<<<<<< HEAD
	if (!ss_initialized) {
		int i;

		for (i = 1; i < SECINITSID_NUM; i++) {
			if (!strcmp(initial_sid_to_string[i], scontext)) {
				*sid = i;
				return 0;
			}
		}
		*sid = SECINITSID_KERNEL;
		return 0;
	}
	*sid = SECSID_NULL;

	/* Copy the string so that we can modify the copy as we parse it. */
	scontext2 = kmalloc(scontext_len + 1, gfp_flags);
	if (!scontext2)
		return -ENOMEM;
	memcpy(scontext2, scontext, scontext_len);
	scontext2[scontext_len] = 0;

=======
	/* Copy the string to allow changes and ensure a NUL terminator */
	scontext2 = kmemdup_nul(scontext, scontext_len, gfp_flags);
	if (!scontext2)
		return -ENOMEM;

	if (!selinux_initialized()) {
		u32 i;

		for (i = 1; i < SECINITSID_NUM; i++) {
			const char *s = initial_sid_to_string[i];

			if (s && !strcmp(s, scontext2)) {
				*sid = i;
				goto out;
			}
		}
		*sid = SECINITSID_KERNEL;
		goto out;
	}
	*sid = SECSID_NULL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (force) {
		/* Save another copy for storing in uninterpreted form */
		rc = -ENOMEM;
		str = kstrdup(scontext2, gfp_flags);
		if (!str)
			goto out;
	}
<<<<<<< HEAD

	read_lock(&policy_rwlock);
	rc = string_to_context_struct(&policydb, &sidtab, scontext2,
				      scontext_len, &context, def_sid);
	if (rc == -EINVAL && force) {
		context.str = str;
		context.len = scontext_len;
		str = NULL;
	} else if (rc)
		goto out_unlock;
	rc = sidtab_context_to_sid(&sidtab, &context, sid);
	context_destroy(&context);
out_unlock:
	read_unlock(&policy_rwlock);
=======
retry:
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;
	rc = string_to_context_struct(policydb, sidtab, scontext2,
				      &context, def_sid);
	if (rc == -EINVAL && force) {
		context.str = str;
		context.len = strlen(str) + 1;
		str = NULL;
	} else if (rc)
		goto out_unlock;
	rc = sidtab_context_to_sid(sidtab, &context, sid);
	if (rc == -ESTALE) {
		rcu_read_unlock();
		if (context.str) {
			str = context.str;
			context.str = NULL;
		}
		context_destroy(&context);
		goto retry;
	}
	context_destroy(&context);
out_unlock:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(scontext2);
	kfree(str);
	return rc;
}

/**
 * security_context_to_sid - Obtain a SID for a given security context.
 * @scontext: security context
 * @scontext_len: length in bytes
 * @sid: security identifier, SID
<<<<<<< HEAD
=======
 * @gfp: context for the allocation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Obtains a SID associated with the security context that
 * has the string representation specified by @scontext.
 * Returns -%EINVAL if the context is invalid, -%ENOMEM if insufficient
 * memory is available, or 0 on success.
 */
<<<<<<< HEAD
int security_context_to_sid(const char *scontext, u32 scontext_len, u32 *sid)
{
	return security_context_to_sid_core(scontext, scontext_len,
					    sid, SECSID_NULL, GFP_KERNEL, 0);
=======
int security_context_to_sid(const char *scontext, u32 scontext_len, u32 *sid,
			    gfp_t gfp)
{
	return security_context_to_sid_core(scontext, scontext_len,
					    sid, SECSID_NULL, gfp, 0);
}

int security_context_str_to_sid(const char *scontext, u32 *sid, gfp_t gfp)
{
	return security_context_to_sid(scontext, strlen(scontext),
				       sid, gfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * security_context_to_sid_default - Obtain a SID for a given security context,
 * falling back to specified default if needed.
 *
 * @scontext: security context
 * @scontext_len: length in bytes
 * @sid: security identifier, SID
 * @def_sid: default SID to assign on error
<<<<<<< HEAD
=======
 * @gfp_flags: the allocator get-free-page (GFP) flags
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Obtains a SID associated with the security context that
 * has the string representation specified by @scontext.
 * The default SID is passed to the MLS layer to be used to allow
 * kernel labeling of the MLS field if the MLS field is not present
 * (for upgrading to MLS without full relabel).
 * Implicitly forces adding of the context even if it cannot be mapped yet.
 * Returns -%EINVAL if the context is invalid, -%ENOMEM if insufficient
 * memory is available, or 0 on success.
 */
int security_context_to_sid_default(const char *scontext, u32 scontext_len,
				    u32 *sid, u32 def_sid, gfp_t gfp_flags)
{
	return security_context_to_sid_core(scontext, scontext_len,
					    sid, def_sid, gfp_flags, 1);
}

int security_context_to_sid_force(const char *scontext, u32 scontext_len,
				  u32 *sid)
{
	return security_context_to_sid_core(scontext, scontext_len,
					    sid, SECSID_NULL, GFP_KERNEL, 1);
}

static int compute_sid_handle_invalid_context(
<<<<<<< HEAD
	struct context *scontext,
	struct context *tcontext,
	u16 tclass,
	struct context *newcontext)
{
	char *s = NULL, *t = NULL, *n = NULL;
	u32 slen, tlen, nlen;

	if (context_struct_to_string(scontext, &s, &slen))
		goto out;
	if (context_struct_to_string(tcontext, &t, &tlen))
		goto out;
	if (context_struct_to_string(newcontext, &n, &nlen))
		goto out;
	audit_log(current->audit_context, GFP_ATOMIC, AUDIT_SELINUX_ERR,
		  "security_compute_sid:  invalid context %s"
		  " for scontext=%s"
		  " tcontext=%s"
		  " tclass=%s",
		  n, s, t, sym_name(&policydb, SYM_CLASSES, tclass-1));
=======
	struct selinux_policy *policy,
	struct sidtab_entry *sentry,
	struct sidtab_entry *tentry,
	u16 tclass,
	struct context *newcontext)
{
	struct policydb *policydb = &policy->policydb;
	struct sidtab *sidtab = policy->sidtab;
	char *s = NULL, *t = NULL, *n = NULL;
	u32 slen, tlen, nlen;
	struct audit_buffer *ab;

	if (sidtab_entry_to_string(policydb, sidtab, sentry, &s, &slen))
		goto out;
	if (sidtab_entry_to_string(policydb, sidtab, tentry, &t, &tlen))
		goto out;
	if (context_struct_to_string(policydb, newcontext, &n, &nlen))
		goto out;
	ab = audit_log_start(audit_context(), GFP_ATOMIC, AUDIT_SELINUX_ERR);
	if (!ab)
		goto out;
	audit_log_format(ab,
			 "op=security_compute_sid invalid_context=");
	/* no need to record the NUL with untrusted strings */
	audit_log_n_untrustedstring(ab, n, nlen - 1);
	audit_log_format(ab, " scontext=%s tcontext=%s tclass=%s",
			 s, t, sym_name(policydb, SYM_CLASSES, tclass-1));
	audit_log_end(ab);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(s);
	kfree(t);
	kfree(n);
<<<<<<< HEAD
	if (!selinux_enforcing)
=======
	if (!enforcing_enabled())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	return -EACCES;
}

<<<<<<< HEAD
static void filename_compute_type(struct policydb *p, struct context *newcontext,
				  u32 stype, u32 ttype, u16 tclass,
				  const char *objname)
{
	struct filename_trans ft;
	struct filename_trans_datum *otype;
=======
static void filename_compute_type(struct policydb *policydb,
				  struct context *newcontext,
				  u32 stype, u32 ttype, u16 tclass,
				  const char *objname)
{
	struct filename_trans_key ft;
	struct filename_trans_datum *datum;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Most filename trans rules are going to live in specific directories
	 * like /dev or /var/run.  This bitmap will quickly skip rule searches
	 * if the ttype does not contain any rules.
	 */
<<<<<<< HEAD
	if (!ebitmap_get_bit(&p->filename_trans_ttypes, ttype))
		return;

	ft.stype = stype;
=======
	if (!ebitmap_get_bit(&policydb->filename_trans_ttypes, ttype))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ft.ttype = ttype;
	ft.tclass = tclass;
	ft.name = objname;

<<<<<<< HEAD
	otype = hashtab_search(p->filename_trans, &ft);
	if (otype)
		newcontext->type = otype->otype;
=======
	datum = policydb_filenametr_search(policydb, &ft);
	while (datum) {
		if (ebitmap_get_bit(&datum->stypes, stype - 1)) {
			newcontext->type = datum->otype;
			return;
		}
		datum = datum->next;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int security_compute_sid(u32 ssid,
				u32 tsid,
				u16 orig_tclass,
<<<<<<< HEAD
				u32 specified,
=======
				u16 specified,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				const char *objname,
				u32 *out_sid,
				bool kern)
{
<<<<<<< HEAD
	struct class_datum *cladatum = NULL;
	struct context *scontext = NULL, *tcontext = NULL, newcontext;
	struct role_trans *roletr = NULL;
	struct avtab_key avkey;
	struct avtab_datum *avdatum;
	struct avtab_node *node;
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct class_datum *cladatum;
	struct context *scontext, *tcontext, newcontext;
	struct sidtab_entry *sentry, *tentry;
	struct avtab_key avkey;
	struct avtab_node *avnode, *node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 tclass;
	int rc = 0;
	bool sock;

<<<<<<< HEAD
	if (!ss_initialized) {
=======
	if (!selinux_initialized()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (orig_tclass) {
		case SECCLASS_PROCESS: /* kernel value */
			*out_sid = ssid;
			break;
		default:
			*out_sid = tsid;
			break;
		}
		goto out;
	}

<<<<<<< HEAD
	context_init(&newcontext);

	read_lock(&policy_rwlock);

	if (kern) {
		tclass = unmap_class(orig_tclass);
		sock = security_is_socket_class(orig_tclass);
	} else {
		tclass = orig_tclass;
		sock = security_is_socket_class(map_class(tclass));
	}

	scontext = sidtab_search(&sidtab, ssid);
	if (!scontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
retry:
	cladatum = NULL;
	context_init(&newcontext);

	rcu_read_lock();

	policy = rcu_dereference(selinux_state.policy);

	if (kern) {
		tclass = unmap_class(&policy->map, orig_tclass);
		sock = security_is_socket_class(orig_tclass);
	} else {
		tclass = orig_tclass;
		sock = security_is_socket_class(map_class(&policy->map,
							  tclass));
	}

	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	sentry = sidtab_search_entry(sidtab, ssid);
	if (!sentry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, ssid);
		rc = -EINVAL;
		goto out_unlock;
	}
<<<<<<< HEAD
	tcontext = sidtab_search(&sidtab, tsid);
	if (!tcontext) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	tentry = sidtab_search_entry(sidtab, tsid);
	if (!tentry) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, tsid);
		rc = -EINVAL;
		goto out_unlock;
	}

<<<<<<< HEAD
	if (tclass && tclass <= policydb.p_classes.nprim)
		cladatum = policydb.class_val_to_struct[tclass - 1];
=======
	scontext = &sentry->context;
	tcontext = &tentry->context;

	if (tclass && tclass <= policydb->p_classes.nprim)
		cladatum = policydb->class_val_to_struct[tclass - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set the user identity. */
	switch (specified) {
	case AVTAB_TRANSITION:
	case AVTAB_CHANGE:
		if (cladatum && cladatum->default_user == DEFAULT_TARGET) {
			newcontext.user = tcontext->user;
		} else {
			/* notice this gets both DEFAULT_SOURCE and unset */
			/* Use the process user identity. */
			newcontext.user = scontext->user;
		}
		break;
	case AVTAB_MEMBER:
		/* Use the related object owner. */
		newcontext.user = tcontext->user;
		break;
	}

	/* Set the role to default values. */
	if (cladatum && cladatum->default_role == DEFAULT_SOURCE) {
		newcontext.role = scontext->role;
	} else if (cladatum && cladatum->default_role == DEFAULT_TARGET) {
		newcontext.role = tcontext->role;
	} else {
<<<<<<< HEAD
		if ((tclass == policydb.process_class) || (sock == true))
=======
		if ((tclass == policydb->process_class) || sock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			newcontext.role = scontext->role;
		else
			newcontext.role = OBJECT_R_VAL;
	}

	/* Set the type to default values. */
	if (cladatum && cladatum->default_type == DEFAULT_SOURCE) {
		newcontext.type = scontext->type;
	} else if (cladatum && cladatum->default_type == DEFAULT_TARGET) {
		newcontext.type = tcontext->type;
	} else {
<<<<<<< HEAD
		if ((tclass == policydb.process_class) || (sock == true)) {
=======
		if ((tclass == policydb->process_class) || sock) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Use the type of process. */
			newcontext.type = scontext->type;
		} else {
			/* Use the type of the related object. */
			newcontext.type = tcontext->type;
		}
	}

	/* Look for a type transition/member/change rule. */
	avkey.source_type = scontext->type;
	avkey.target_type = tcontext->type;
	avkey.target_class = tclass;
	avkey.specified = specified;
<<<<<<< HEAD
	avdatum = avtab_search(&policydb.te_avtab, &avkey);

	/* If no permanent rule, also check for enabled conditional rules */
	if (!avdatum) {
		node = avtab_search_node(&policydb.te_cond_avtab, &avkey);
		for (; node; node = avtab_search_node_next(node, specified)) {
			if (node->key.specified & AVTAB_ENABLED) {
				avdatum = &node->datum;
=======
	avnode = avtab_search_node(&policydb->te_avtab, &avkey);

	/* If no permanent rule, also check for enabled conditional rules */
	if (!avnode) {
		node = avtab_search_node(&policydb->te_cond_avtab, &avkey);
		for (; node; node = avtab_search_node_next(node, specified)) {
			if (node->key.specified & AVTAB_ENABLED) {
				avnode = node;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			}
		}
	}

<<<<<<< HEAD
	if (avdatum) {
		/* Use the type from the type transition/member/change rule. */
		newcontext.type = avdatum->u.data;
=======
	if (avnode) {
		/* Use the type from the type transition/member/change rule. */
		newcontext.type = avnode->datum.u.data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* if we have a objname this is a file trans check so check those rules */
	if (objname)
<<<<<<< HEAD
		filename_compute_type(&policydb, &newcontext, scontext->type,
=======
		filename_compute_type(policydb, &newcontext, scontext->type,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      tcontext->type, tclass, objname);

	/* Check for class-specific changes. */
	if (specified & AVTAB_TRANSITION) {
		/* Look for a role transition rule. */
<<<<<<< HEAD
		for (roletr = policydb.role_tr; roletr; roletr = roletr->next) {
			if ((roletr->role == scontext->role) &&
			    (roletr->type == tcontext->type) &&
			    (roletr->tclass == tclass)) {
				/* Use the role transition rule. */
				newcontext.role = roletr->new_role;
				break;
			}
		}
=======
		struct role_trans_datum *rtd;
		struct role_trans_key rtk = {
			.role = scontext->role,
			.type = tcontext->type,
			.tclass = tclass,
		};

		rtd = policydb_roletr_search(policydb, &rtk);
		if (rtd)
			newcontext.role = rtd->new_role;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Set the MLS attributes.
	   This is done last because it may allocate memory. */
<<<<<<< HEAD
	rc = mls_compute_sid(scontext, tcontext, tclass, specified,
=======
	rc = mls_compute_sid(policydb, scontext, tcontext, tclass, specified,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     &newcontext, sock);
	if (rc)
		goto out_unlock;

	/* Check the validity of the context. */
<<<<<<< HEAD
	if (!policydb_context_isvalid(&policydb, &newcontext)) {
		rc = compute_sid_handle_invalid_context(scontext,
							tcontext,
							tclass,
=======
	if (!policydb_context_isvalid(policydb, &newcontext)) {
		rc = compute_sid_handle_invalid_context(policy, sentry,
							tentry, tclass,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							&newcontext);
		if (rc)
			goto out_unlock;
	}
	/* Obtain the sid for the context. */
<<<<<<< HEAD
	rc = sidtab_context_to_sid(&sidtab, &newcontext, out_sid);
out_unlock:
	read_unlock(&policy_rwlock);
=======
	rc = sidtab_context_to_sid(sidtab, &newcontext, out_sid);
	if (rc == -ESTALE) {
		rcu_read_unlock();
		context_destroy(&newcontext);
		goto retry;
	}
out_unlock:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	context_destroy(&newcontext);
out:
	return rc;
}

/**
 * security_transition_sid - Compute the SID for a new subject/object.
 * @ssid: source security identifier
 * @tsid: target security identifier
 * @tclass: target security class
<<<<<<< HEAD
=======
 * @qstr: object name
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @out_sid: security identifier for new subject/object
 *
 * Compute a SID to use for labeling a new subject or object in the
 * class @tclass based on a SID pair (@ssid, @tsid).
 * Return -%EINVAL if any of the parameters are invalid, -%ENOMEM
 * if insufficient memory is available, or %0 if the new SID was
 * computed successfully.
 */
int security_transition_sid(u32 ssid, u32 tsid, u16 tclass,
			    const struct qstr *qstr, u32 *out_sid)
{
<<<<<<< HEAD
	return security_compute_sid(ssid, tsid, tclass, AVTAB_TRANSITION,
=======
	return security_compute_sid(ssid, tsid, tclass,
				    AVTAB_TRANSITION,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    qstr ? qstr->name : NULL, out_sid, true);
}

int security_transition_sid_user(u32 ssid, u32 tsid, u16 tclass,
				 const char *objname, u32 *out_sid)
{
<<<<<<< HEAD
	return security_compute_sid(ssid, tsid, tclass, AVTAB_TRANSITION,
=======
	return security_compute_sid(ssid, tsid, tclass,
				    AVTAB_TRANSITION,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    objname, out_sid, false);
}

/**
 * security_member_sid - Compute the SID for member selection.
 * @ssid: source security identifier
 * @tsid: target security identifier
 * @tclass: target security class
 * @out_sid: security identifier for selected member
 *
 * Compute a SID to use when selecting a member of a polyinstantiated
 * object of class @tclass based on a SID pair (@ssid, @tsid).
 * Return -%EINVAL if any of the parameters are invalid, -%ENOMEM
 * if insufficient memory is available, or %0 if the SID was
 * computed successfully.
 */
int security_member_sid(u32 ssid,
			u32 tsid,
			u16 tclass,
			u32 *out_sid)
{
<<<<<<< HEAD
	return security_compute_sid(ssid, tsid, tclass, AVTAB_MEMBER, NULL,
=======
	return security_compute_sid(ssid, tsid, tclass,
				    AVTAB_MEMBER, NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    out_sid, false);
}

/**
 * security_change_sid - Compute the SID for object relabeling.
 * @ssid: source security identifier
 * @tsid: target security identifier
 * @tclass: target security class
 * @out_sid: security identifier for selected member
 *
 * Compute a SID to use for relabeling an object of class @tclass
 * based on a SID pair (@ssid, @tsid).
 * Return -%EINVAL if any of the parameters are invalid, -%ENOMEM
 * if insufficient memory is available, or %0 if the SID was
 * computed successfully.
 */
int security_change_sid(u32 ssid,
			u32 tsid,
			u16 tclass,
			u32 *out_sid)
{
	return security_compute_sid(ssid, tsid, tclass, AVTAB_CHANGE, NULL,
				    out_sid, false);
}

<<<<<<< HEAD
/* Clone the SID into the new SID table. */
static int clone_sid(u32 sid,
		     struct context *context,
		     void *arg)
{
	struct sidtab *s = arg;

	if (sid > SECINITSID_NUM)
		return sidtab_insert(s, sid, context);
	else
		return 0;
}

static inline int convert_context_handle_invalid_context(struct context *context)
=======
static inline int convert_context_handle_invalid_context(
	struct policydb *policydb,
	struct context *context)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *s;
	u32 len;

<<<<<<< HEAD
	if (selinux_enforcing)
		return -EINVAL;

	if (!context_struct_to_string(context, &s, &len)) {
		printk(KERN_WARNING "SELinux:  Context %s would be invalid if enforcing\n", s);
=======
	if (enforcing_enabled())
		return -EINVAL;

	if (!context_struct_to_string(policydb, context, &s, &len)) {
		pr_warn("SELinux:  Context %s would be invalid if enforcing\n",
			s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(s);
	}
	return 0;
}

<<<<<<< HEAD
struct convert_context_args {
	struct policydb *oldp;
	struct policydb *newp;
};

/*
 * Convert the values in the security context
 * structure `c' from the values specified
 * in the policy `p->oldp' to the values specified
 * in the policy `p->newp'.  Verify that the
 * context is valid under the new policy.
 */
static int convert_context(u32 key,
			   struct context *c,
			   void *p)
{
	struct convert_context_args *args;
	struct context oldc;
	struct ocontext *oc;
	struct mls_range *range;
=======
/**
 * services_convert_context - Convert a security context across policies.
 * @args: populated convert_context_args struct
 * @oldc: original context
 * @newc: converted context
 * @gfp_flags: allocation flags
 *
 * Convert the values in the security context structure @oldc from the values
 * specified in the policy @args->oldp to the values specified in the policy
 * @args->newp, storing the new context in @newc, and verifying that the
 * context is valid under the new policy.
 */
int services_convert_context(struct convert_context_args *args,
			     struct context *oldc, struct context *newc,
			     gfp_t gfp_flags)
{
	struct ocontext *oc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct role_datum *role;
	struct type_datum *typdatum;
	struct user_datum *usrdatum;
	char *s;
	u32 len;
<<<<<<< HEAD
	int rc = 0;

	if (key <= SECINITSID_NUM)
		goto out;

	args = p;

	if (c->str) {
		struct context ctx;

		rc = -ENOMEM;
		s = kstrdup(c->str, GFP_KERNEL);
		if (!s)
			goto out;

		rc = string_to_context_struct(args->newp, NULL, s,
					      c->len, &ctx, SECSID_NULL);
		kfree(s);
		if (!rc) {
			printk(KERN_INFO "SELinux:  Context %s became valid (mapped).\n",
			       c->str);
			/* Replace string with mapped representation. */
			kfree(c->str);
			memcpy(c, &ctx, sizeof(*c));
			goto out;
		} else if (rc == -EINVAL) {
			/* Retain string representation for later mapping. */
			rc = 0;
			goto out;
		} else {
			/* Other error condition, e.g. ENOMEM. */
			printk(KERN_ERR "SELinux:   Unable to map context %s, rc = %d.\n",
			       c->str, -rc);
			goto out;
		}
	}

	rc = context_cpy(&oldc, c);
	if (rc)
		goto out;

	/* Convert the user. */
	rc = -EINVAL;
	usrdatum = hashtab_search(args->newp->p_users.table,
				  sym_name(args->oldp, SYM_USERS, c->user - 1));
	if (!usrdatum)
		goto bad;
	c->user = usrdatum->value;

	/* Convert the role. */
	rc = -EINVAL;
	role = hashtab_search(args->newp->p_roles.table,
			      sym_name(args->oldp, SYM_ROLES, c->role - 1));
	if (!role)
		goto bad;
	c->role = role->value;

	/* Convert the type. */
	rc = -EINVAL;
	typdatum = hashtab_search(args->newp->p_types.table,
				  sym_name(args->oldp, SYM_TYPES, c->type - 1));
	if (!typdatum)
		goto bad;
	c->type = typdatum->value;

	/* Convert the MLS fields if dealing with MLS policies */
	if (args->oldp->mls_enabled && args->newp->mls_enabled) {
		rc = mls_convert_context(args->oldp, args->newp, c);
		if (rc)
			goto bad;
	} else if (args->oldp->mls_enabled && !args->newp->mls_enabled) {
		/*
		 * Switching between MLS and non-MLS policy:
		 * free any storage used by the MLS fields in the
		 * context for all existing entries in the sidtab.
		 */
		mls_context_destroy(c);
=======
	int rc;

	if (oldc->str) {
		s = kstrdup(oldc->str, gfp_flags);
		if (!s)
			return -ENOMEM;

		rc = string_to_context_struct(args->newp, NULL, s, newc, SECSID_NULL);
		if (rc == -EINVAL) {
			/*
			 * Retain string representation for later mapping.
			 *
			 * IMPORTANT: We need to copy the contents of oldc->str
			 * back into s again because string_to_context_struct()
			 * may have garbled it.
			 */
			memcpy(s, oldc->str, oldc->len);
			context_init(newc);
			newc->str = s;
			newc->len = oldc->len;
			return 0;
		}
		kfree(s);
		if (rc) {
			/* Other error condition, e.g. ENOMEM. */
			pr_err("SELinux:   Unable to map context %s, rc = %d.\n",
			       oldc->str, -rc);
			return rc;
		}
		pr_info("SELinux:  Context %s became valid (mapped).\n",
			oldc->str);
		return 0;
	}

	context_init(newc);

	/* Convert the user. */
	usrdatum = symtab_search(&args->newp->p_users,
				 sym_name(args->oldp, SYM_USERS, oldc->user - 1));
	if (!usrdatum)
		goto bad;
	newc->user = usrdatum->value;

	/* Convert the role. */
	role = symtab_search(&args->newp->p_roles,
			     sym_name(args->oldp, SYM_ROLES, oldc->role - 1));
	if (!role)
		goto bad;
	newc->role = role->value;

	/* Convert the type. */
	typdatum = symtab_search(&args->newp->p_types,
				 sym_name(args->oldp, SYM_TYPES, oldc->type - 1));
	if (!typdatum)
		goto bad;
	newc->type = typdatum->value;

	/* Convert the MLS fields if dealing with MLS policies */
	if (args->oldp->mls_enabled && args->newp->mls_enabled) {
		rc = mls_convert_context(args->oldp, args->newp, oldc, newc);
		if (rc)
			goto bad;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (!args->oldp->mls_enabled && args->newp->mls_enabled) {
		/*
		 * Switching between non-MLS and MLS policy:
		 * ensure that the MLS fields of the context for all
		 * existing entries in the sidtab are filled in with a
		 * suitable default value, likely taken from one of the
		 * initial SIDs.
		 */
		oc = args->newp->ocontexts[OCON_ISID];
		while (oc && oc->sid[0] != SECINITSID_UNLABELED)
			oc = oc->next;
<<<<<<< HEAD
		rc = -EINVAL;
		if (!oc) {
			printk(KERN_ERR "SELinux:  unable to look up"
				" the initial SIDs list\n");
			goto bad;
		}
		range = &oc->context[0].range;
		rc = mls_range_set(c, range);
=======
		if (!oc) {
			pr_err("SELinux:  unable to look up"
				" the initial SIDs list\n");
			goto bad;
		}
		rc = mls_range_set(newc, &oc->context[0].range);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			goto bad;
	}

	/* Check the validity of the new context. */
<<<<<<< HEAD
	if (!policydb_context_isvalid(args->newp, c)) {
		rc = convert_context_handle_invalid_context(&oldc);
=======
	if (!policydb_context_isvalid(args->newp, newc)) {
		rc = convert_context_handle_invalid_context(args->oldp, oldc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			goto bad;
	}

<<<<<<< HEAD
	context_destroy(&oldc);

	rc = 0;
out:
	return rc;
bad:
	/* Map old representation to string and save it. */
	rc = context_struct_to_string(&oldc, &s, &len);
	if (rc)
		return rc;
	context_destroy(&oldc);
	context_destroy(c);
	c->str = s;
	c->len = len;
	printk(KERN_INFO "SELinux:  Context %s became invalid (unmapped).\n",
	       c->str);
	rc = 0;
	goto out;
}

static void security_load_policycaps(void)
{
	selinux_policycap_netpeer = ebitmap_get_bit(&policydb.policycaps,
						  POLICYDB_CAPABILITY_NETPEER);
	selinux_policycap_openperm = ebitmap_get_bit(&policydb.policycaps,
						  POLICYDB_CAPABILITY_OPENPERM);
}

static int security_preserve_bools(struct policydb *p);
=======
	return 0;
bad:
	/* Map old representation to string and save it. */
	rc = context_struct_to_string(args->oldp, oldc, &s, &len);
	if (rc)
		return rc;
	context_destroy(newc);
	newc->str = s;
	newc->len = len;
	pr_info("SELinux:  Context %s became invalid (unmapped).\n",
		newc->str);
	return 0;
}

static void security_load_policycaps(struct selinux_policy *policy)
{
	struct policydb *p;
	unsigned int i;
	struct ebitmap_node *node;

	p = &policy->policydb;

	for (i = 0; i < ARRAY_SIZE(selinux_state.policycap); i++)
		WRITE_ONCE(selinux_state.policycap[i],
			ebitmap_get_bit(&p->policycaps, i));

	for (i = 0; i < ARRAY_SIZE(selinux_policycap_names); i++)
		pr_info("SELinux:  policy capability %s=%d\n",
			selinux_policycap_names[i],
			ebitmap_get_bit(&p->policycaps, i));

	ebitmap_for_each_positive_bit(&p->policycaps, node, i) {
		if (i >= ARRAY_SIZE(selinux_policycap_names))
			pr_info("SELinux:  unknown policy capability %u\n",
				i);
	}
}

static int security_preserve_bools(struct selinux_policy *oldpolicy,
				struct selinux_policy *newpolicy);

static void selinux_policy_free(struct selinux_policy *policy)
{
	if (!policy)
		return;

	sidtab_destroy(policy->sidtab);
	kfree(policy->map.mapping);
	policydb_destroy(&policy->policydb);
	kfree(policy->sidtab);
	kfree(policy);
}

static void selinux_policy_cond_free(struct selinux_policy *policy)
{
	cond_policydb_destroy_dup(&policy->policydb);
	kfree(policy);
}

void selinux_policy_cancel(struct selinux_load_state *load_state)
{
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *oldpolicy;

	oldpolicy = rcu_dereference_protected(state->policy,
					lockdep_is_held(&state->policy_mutex));

	sidtab_cancel_convert(oldpolicy->sidtab);
	selinux_policy_free(load_state->policy);
	kfree(load_state->convert_data);
}

static void selinux_notify_policy_change(u32 seqno)
{
	/* Flush external caches and notify userspace of policy load */
	avc_ss_reset(seqno);
	selnl_notify_policyload(seqno);
	selinux_status_update_policyload(seqno);
	selinux_netlbl_cache_invalidate();
	selinux_xfrm_notify_policyload();
	selinux_ima_measure_state_locked();
}

void selinux_policy_commit(struct selinux_load_state *load_state)
{
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *oldpolicy, *newpolicy = load_state->policy;
	unsigned long flags;
	u32 seqno;

	oldpolicy = rcu_dereference_protected(state->policy,
					lockdep_is_held(&state->policy_mutex));

	/* If switching between different policy types, log MLS status */
	if (oldpolicy) {
		if (oldpolicy->policydb.mls_enabled && !newpolicy->policydb.mls_enabled)
			pr_info("SELinux: Disabling MLS support...\n");
		else if (!oldpolicy->policydb.mls_enabled && newpolicy->policydb.mls_enabled)
			pr_info("SELinux: Enabling MLS support...\n");
	}

	/* Set latest granting seqno for new policy. */
	if (oldpolicy)
		newpolicy->latest_granting = oldpolicy->latest_granting + 1;
	else
		newpolicy->latest_granting = 1;
	seqno = newpolicy->latest_granting;

	/* Install the new policy. */
	if (oldpolicy) {
		sidtab_freeze_begin(oldpolicy->sidtab, &flags);
		rcu_assign_pointer(state->policy, newpolicy);
		sidtab_freeze_end(oldpolicy->sidtab, &flags);
	} else {
		rcu_assign_pointer(state->policy, newpolicy);
	}

	/* Load the policycaps from the new policy */
	security_load_policycaps(newpolicy);

	if (!selinux_initialized()) {
		/*
		 * After first policy load, the security server is
		 * marked as initialized and ready to handle requests and
		 * any objects created prior to policy load are then labeled.
		 */
		selinux_mark_initialized();
		selinux_complete_init();
	}

	/* Free the old policy */
	synchronize_rcu();
	selinux_policy_free(oldpolicy);
	kfree(load_state->convert_data);

	/* Notify others of the policy change */
	selinux_notify_policy_change(seqno);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * security_load_policy - Load a security policy configuration.
 * @data: binary policy data
 * @len: length of data in bytes
<<<<<<< HEAD
=======
 * @load_state: policy load state
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Load a new set of security policy configuration data,
 * validate it and convert the SID table as necessary.
 * This function will flush the access vector cache after
 * loading the new policy.
 */
<<<<<<< HEAD
int security_load_policy(void *data, size_t len)
{
	struct policydb oldpolicydb, newpolicydb;
	struct sidtab oldsidtab, newsidtab;
	struct selinux_mapping *oldmap, *map = NULL;
	struct convert_context_args args;
	u32 seqno;
	u16 map_size;
	int rc = 0;
	struct policy_file file = { data, len }, *fp = &file;

	if (!ss_initialized) {
		avtab_cache_init();
		rc = policydb_read(&policydb, fp);
		if (rc) {
			avtab_cache_destroy();
			return rc;
		}

		policydb.len = len;
		rc = selinux_set_mapping(&policydb, secclass_map,
					 &current_mapping,
					 &current_mapping_size);
		if (rc) {
			policydb_destroy(&policydb);
			avtab_cache_destroy();
			return rc;
		}

		rc = policydb_load_isids(&policydb, &sidtab);
		if (rc) {
			policydb_destroy(&policydb);
			avtab_cache_destroy();
			return rc;
		}

		security_load_policycaps();
		ss_initialized = 1;
		seqno = ++latest_granting;
		selinux_complete_init();
		avc_ss_reset(seqno);
		selnl_notify_policyload(seqno);
		selinux_status_update_policyload(seqno);
		selinux_netlbl_cache_invalidate();
		selinux_xfrm_notify_policyload();
		return 0;
	}

#if 0
	sidtab_hash_eval(&sidtab, "sids");
#endif

	rc = policydb_read(&newpolicydb, fp);
	if (rc)
		return rc;

	newpolicydb.len = len;
	/* If switching between different policy types, log MLS status */
	if (policydb.mls_enabled && !newpolicydb.mls_enabled)
		printk(KERN_INFO "SELinux: Disabling MLS support...\n");
	else if (!policydb.mls_enabled && newpolicydb.mls_enabled)
		printk(KERN_INFO "SELinux: Enabling MLS support...\n");

	rc = policydb_load_isids(&newpolicydb, &newsidtab);
	if (rc) {
		printk(KERN_ERR "SELinux:  unable to load the initial SIDs\n");
		policydb_destroy(&newpolicydb);
		return rc;
	}

	rc = selinux_set_mapping(&newpolicydb, secclass_map, &map, &map_size);
	if (rc)
		goto err;

	rc = security_preserve_bools(&newpolicydb);
	if (rc) {
		printk(KERN_ERR "SELinux:  unable to preserve booleans\n");
		goto err;
	}

	/* Clone the SID table. */
	sidtab_shutdown(&sidtab);

	rc = sidtab_map(&sidtab, clone_sid, &newsidtab);
	if (rc)
		goto err;

=======
int security_load_policy(void *data, size_t len,
			 struct selinux_load_state *load_state)
{
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *newpolicy, *oldpolicy;
	struct selinux_policy_convert_data *convert_data;
	int rc = 0;
	struct policy_file file = { data, len }, *fp = &file;

	newpolicy = kzalloc(sizeof(*newpolicy), GFP_KERNEL);
	if (!newpolicy)
		return -ENOMEM;

	newpolicy->sidtab = kzalloc(sizeof(*newpolicy->sidtab), GFP_KERNEL);
	if (!newpolicy->sidtab) {
		rc = -ENOMEM;
		goto err_policy;
	}

	rc = policydb_read(&newpolicy->policydb, fp);
	if (rc)
		goto err_sidtab;

	newpolicy->policydb.len = len;
	rc = selinux_set_mapping(&newpolicy->policydb, secclass_map,
				&newpolicy->map);
	if (rc)
		goto err_policydb;

	rc = policydb_load_isids(&newpolicy->policydb, newpolicy->sidtab);
	if (rc) {
		pr_err("SELinux:  unable to load the initial SIDs\n");
		goto err_mapping;
	}

	if (!selinux_initialized()) {
		/* First policy load, so no need to preserve state from old policy */
		load_state->policy = newpolicy;
		load_state->convert_data = NULL;
		return 0;
	}

	oldpolicy = rcu_dereference_protected(state->policy,
					lockdep_is_held(&state->policy_mutex));

	/* Preserve active boolean values from the old policy */
	rc = security_preserve_bools(oldpolicy, newpolicy);
	if (rc) {
		pr_err("SELinux:  unable to preserve booleans\n");
		goto err_free_isids;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Convert the internal representations of contexts
	 * in the new SID table.
	 */
<<<<<<< HEAD
	args.oldp = &policydb;
	args.newp = &newpolicydb;
	rc = sidtab_map(&newsidtab, convert_context, &args);
	if (rc) {
		printk(KERN_ERR "SELinux:  unable to convert the internal"
			" representation of contexts in the new SID"
			" table\n");
		goto err;
	}

	/* Save the old policydb and SID table to free later. */
	memcpy(&oldpolicydb, &policydb, sizeof policydb);
	sidtab_set(&oldsidtab, &sidtab);

	/* Install the new policydb and SID table. */
	write_lock_irq(&policy_rwlock);
	memcpy(&policydb, &newpolicydb, sizeof policydb);
	sidtab_set(&sidtab, &newsidtab);
	security_load_policycaps();
	oldmap = current_mapping;
	current_mapping = map;
	current_mapping_size = map_size;
	seqno = ++latest_granting;
	write_unlock_irq(&policy_rwlock);

	/* Free the old policydb and SID table. */
	policydb_destroy(&oldpolicydb);
	sidtab_destroy(&oldsidtab);
	kfree(oldmap);

	avc_ss_reset(seqno);
	selnl_notify_policyload(seqno);
	selinux_status_update_policyload(seqno);
	selinux_netlbl_cache_invalidate();
	selinux_xfrm_notify_policyload();

	return 0;

err:
	kfree(map);
	sidtab_destroy(&newsidtab);
	policydb_destroy(&newpolicydb);
	return rc;

}

size_t security_policydb_len(void)
{
	size_t len;

	read_lock(&policy_rwlock);
	len = policydb.len;
	read_unlock(&policy_rwlock);

	return len;
=======

	convert_data = kmalloc(sizeof(*convert_data), GFP_KERNEL);
	if (!convert_data) {
		rc = -ENOMEM;
		goto err_free_isids;
	}

	convert_data->args.oldp = &oldpolicy->policydb;
	convert_data->args.newp = &newpolicy->policydb;

	convert_data->sidtab_params.args = &convert_data->args;
	convert_data->sidtab_params.target = newpolicy->sidtab;

	rc = sidtab_convert(oldpolicy->sidtab, &convert_data->sidtab_params);
	if (rc) {
		pr_err("SELinux:  unable to convert the internal"
			" representation of contexts in the new SID"
			" table\n");
		goto err_free_convert_data;
	}

	load_state->policy = newpolicy;
	load_state->convert_data = convert_data;
	return 0;

err_free_convert_data:
	kfree(convert_data);
err_free_isids:
	sidtab_destroy(newpolicy->sidtab);
err_mapping:
	kfree(newpolicy->map.mapping);
err_policydb:
	policydb_destroy(&newpolicy->policydb);
err_sidtab:
	kfree(newpolicy->sidtab);
err_policy:
	kfree(newpolicy);

	return rc;
}

/**
 * ocontext_to_sid - Helper to safely get sid for an ocontext
 * @sidtab: SID table
 * @c: ocontext structure
 * @index: index of the context entry (0 or 1)
 * @out_sid: pointer to the resulting SID value
 *
 * For all ocontexts except OCON_ISID the SID fields are populated
 * on-demand when needed. Since updating the SID value is an SMP-sensitive
 * operation, this helper must be used to do that safely.
 *
 * WARNING: This function may return -ESTALE, indicating that the caller
 * must retry the operation after re-acquiring the policy pointer!
 */
static int ocontext_to_sid(struct sidtab *sidtab, struct ocontext *c,
			   size_t index, u32 *out_sid)
{
	int rc;
	u32 sid;

	/* Ensure the associated sidtab entry is visible to this thread. */
	sid = smp_load_acquire(&c->sid[index]);
	if (!sid) {
		rc = sidtab_context_to_sid(sidtab, &c->context[index], &sid);
		if (rc)
			return rc;

		/*
		 * Ensure the new sidtab entry is visible to other threads
		 * when they see the SID.
		 */
		smp_store_release(&c->sid[index], sid);
	}
	*out_sid = sid;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * security_port_sid - Obtain the SID for a port.
 * @protocol: protocol number
 * @port: port number
 * @out_sid: security identifier
 */
int security_port_sid(u8 protocol, u16 port, u32 *out_sid)
{
<<<<<<< HEAD
	struct ocontext *c;
	int rc = 0;

	read_lock(&policy_rwlock);

	c = policydb.ocontexts[OCON_PORT];
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct ocontext *c;
	int rc;

	if (!selinux_initialized()) {
		*out_sid = SECINITSID_PORT;
		return 0;
	}

retry:
	rc = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	c = policydb->ocontexts[OCON_PORT];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (c) {
		if (c->u.port.protocol == protocol &&
		    c->u.port.low_port <= port &&
		    c->u.port.high_port >= port)
			break;
		c = c->next;
	}

	if (c) {
<<<<<<< HEAD
		if (!c->sid[0]) {
			rc = sidtab_context_to_sid(&sidtab,
						   &c->context[0],
						   &c->sid[0]);
			if (rc)
				goto out;
		}
		*out_sid = c->sid[0];
=======
		rc = ocontext_to_sid(sidtab, c, 0, out_sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		*out_sid = SECINITSID_PORT;
	}

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
	return rc;
}

/**
 * security_ib_pkey_sid - Obtain the SID for a pkey.
 * @subnet_prefix: Subnet Prefix
 * @pkey_num: pkey number
 * @out_sid: security identifier
 */
int security_ib_pkey_sid(u64 subnet_prefix, u16 pkey_num, u32 *out_sid)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct ocontext *c;
	int rc;

	if (!selinux_initialized()) {
		*out_sid = SECINITSID_UNLABELED;
		return 0;
	}

retry:
	rc = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	c = policydb->ocontexts[OCON_IBPKEY];
	while (c) {
		if (c->u.ibpkey.low_pkey <= pkey_num &&
		    c->u.ibpkey.high_pkey >= pkey_num &&
		    c->u.ibpkey.subnet_prefix == subnet_prefix)
			break;

		c = c->next;
	}

	if (c) {
		rc = ocontext_to_sid(sidtab, c, 0, out_sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
	} else
		*out_sid = SECINITSID_UNLABELED;

out:
	rcu_read_unlock();
	return rc;
}

/**
 * security_ib_endport_sid - Obtain the SID for a subnet management interface.
 * @dev_name: device name
 * @port_num: port number
 * @out_sid: security identifier
 */
int security_ib_endport_sid(const char *dev_name, u8 port_num, u32 *out_sid)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct ocontext *c;
	int rc;

	if (!selinux_initialized()) {
		*out_sid = SECINITSID_UNLABELED;
		return 0;
	}

retry:
	rc = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	c = policydb->ocontexts[OCON_IBENDPORT];
	while (c) {
		if (c->u.ibendport.port == port_num &&
		    !strncmp(c->u.ibendport.dev_name,
			     dev_name,
			     IB_DEVICE_NAME_MAX))
			break;

		c = c->next;
	}

	if (c) {
		rc = ocontext_to_sid(sidtab, c, 0, out_sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
	} else
		*out_sid = SECINITSID_UNLABELED;

out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

/**
 * security_netif_sid - Obtain the SID for a network interface.
 * @name: interface name
 * @if_sid: interface SID
 */
int security_netif_sid(char *name, u32 *if_sid)
{
<<<<<<< HEAD
	int rc = 0;
	struct ocontext *c;

	read_lock(&policy_rwlock);

	c = policydb.ocontexts[OCON_NETIF];
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	int rc;
	struct ocontext *c;

	if (!selinux_initialized()) {
		*if_sid = SECINITSID_NETIF;
		return 0;
	}

retry:
	rc = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	c = policydb->ocontexts[OCON_NETIF];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (c) {
		if (strcmp(name, c->u.name) == 0)
			break;
		c = c->next;
	}

	if (c) {
<<<<<<< HEAD
		if (!c->sid[0] || !c->sid[1]) {
			rc = sidtab_context_to_sid(&sidtab,
						  &c->context[0],
						  &c->sid[0]);
			if (rc)
				goto out;
			rc = sidtab_context_to_sid(&sidtab,
						   &c->context[1],
						   &c->sid[1]);
			if (rc)
				goto out;
		}
		*if_sid = c->sid[0];
=======
		rc = ocontext_to_sid(sidtab, c, 0, if_sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		*if_sid = SECINITSID_NETIF;

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int match_ipv6_addrmask(u32 *input, u32 *addr, u32 *mask)
{
	int i, fail = 0;

	for (i = 0; i < 4; i++)
		if (addr[i] != (input[i] & mask[i])) {
			fail = 1;
			break;
		}

	return !fail;
}

/**
 * security_node_sid - Obtain the SID for a node (host).
 * @domain: communication domain aka address family
 * @addrp: address
 * @addrlen: address length in bytes
 * @out_sid: security identifier
 */
int security_node_sid(u16 domain,
		      void *addrp,
		      u32 addrlen,
		      u32 *out_sid)
{
<<<<<<< HEAD
	int rc;
	struct ocontext *c;

	read_lock(&policy_rwlock);
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	int rc;
	struct ocontext *c;

	if (!selinux_initialized()) {
		*out_sid = SECINITSID_NODE;
		return 0;
	}

retry:
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (domain) {
	case AF_INET: {
		u32 addr;

		rc = -EINVAL;
		if (addrlen != sizeof(u32))
			goto out;

		addr = *((u32 *)addrp);

<<<<<<< HEAD
		c = policydb.ocontexts[OCON_NODE];
=======
		c = policydb->ocontexts[OCON_NODE];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (c) {
			if (c->u.node.addr == (addr & c->u.node.mask))
				break;
			c = c->next;
		}
		break;
	}

	case AF_INET6:
		rc = -EINVAL;
		if (addrlen != sizeof(u64) * 2)
			goto out;
<<<<<<< HEAD
		c = policydb.ocontexts[OCON_NODE6];
=======
		c = policydb->ocontexts[OCON_NODE6];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		while (c) {
			if (match_ipv6_addrmask(addrp, c->u.node6.addr,
						c->u.node6.mask))
				break;
			c = c->next;
		}
		break;

	default:
		rc = 0;
		*out_sid = SECINITSID_NODE;
		goto out;
	}

	if (c) {
<<<<<<< HEAD
		if (!c->sid[0]) {
			rc = sidtab_context_to_sid(&sidtab,
						   &c->context[0],
						   &c->sid[0]);
			if (rc)
				goto out;
		}
		*out_sid = c->sid[0];
=======
		rc = ocontext_to_sid(sidtab, c, 0, out_sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		*out_sid = SECINITSID_NODE;
	}

	rc = 0;
out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

#define SIDS_NEL 25

/**
 * security_get_user_sids - Obtain reachable SIDs for a user.
 * @fromsid: starting SID
 * @username: username
 * @sids: array of reachable SIDs for user
 * @nel: number of elements in @sids
 *
 * Generate the set of SIDs for legal security contexts
 * for a given user that can be reached by @fromsid.
 * Set *@sids to point to a dynamically allocated
 * array containing the set of SIDs.  Set *@nel to the
 * number of elements in the array.
 */

int security_get_user_sids(u32 fromsid,
			   char *username,
			   u32 **sids,
			   u32 *nel)
{
<<<<<<< HEAD
	struct context *fromcon, usercon;
	u32 *mysids = NULL, *mysids2, sid;
	u32 mynel = 0, maxnel = SIDS_NEL;
	struct user_datum *user;
	struct role_datum *role;
	struct ebitmap_node *rnode, *tnode;
	int rc = 0, i, j;
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	struct context *fromcon, usercon;
	u32 *mysids = NULL, *mysids2, sid;
	u32 i, j, mynel, maxnel = SIDS_NEL;
	struct user_datum *user;
	struct role_datum *role;
	struct ebitmap_node *rnode, *tnode;
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*sids = NULL;
	*nel = 0;

<<<<<<< HEAD
	if (!ss_initialized)
		goto out;

	read_lock(&policy_rwlock);
=======
	if (!selinux_initialized())
		return 0;

	mysids = kcalloc(maxnel, sizeof(*mysids), GFP_KERNEL);
	if (!mysids)
		return -ENOMEM;

retry:
	mynel = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	context_init(&usercon);

	rc = -EINVAL;
<<<<<<< HEAD
	fromcon = sidtab_search(&sidtab, fromsid);
=======
	fromcon = sidtab_search(sidtab, fromsid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!fromcon)
		goto out_unlock;

	rc = -EINVAL;
<<<<<<< HEAD
	user = hashtab_search(policydb.p_users.table, username);
=======
	user = symtab_search(&policydb->p_users, username);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!user)
		goto out_unlock;

	usercon.user = user->value;

<<<<<<< HEAD
	rc = -ENOMEM;
	mysids = kcalloc(maxnel, sizeof(*mysids), GFP_ATOMIC);
	if (!mysids)
		goto out_unlock;

	ebitmap_for_each_positive_bit(&user->roles, rnode, i) {
		role = policydb.role_val_to_struct[i];
=======
	ebitmap_for_each_positive_bit(&user->roles, rnode, i) {
		role = policydb->role_val_to_struct[i];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		usercon.role = i + 1;
		ebitmap_for_each_positive_bit(&role->types, tnode, j) {
			usercon.type = j + 1;

<<<<<<< HEAD
			if (mls_setup_user_range(fromcon, user, &usercon))
				continue;

			rc = sidtab_context_to_sid(&sidtab, &usercon, &sid);
=======
			if (mls_setup_user_range(policydb, fromcon, user,
						 &usercon))
				continue;

			rc = sidtab_context_to_sid(sidtab, &usercon, &sid);
			if (rc == -ESTALE) {
				rcu_read_unlock();
				goto retry;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (rc)
				goto out_unlock;
			if (mynel < maxnel) {
				mysids[mynel++] = sid;
			} else {
				rc = -ENOMEM;
				maxnel += SIDS_NEL;
				mysids2 = kcalloc(maxnel, sizeof(*mysids2), GFP_ATOMIC);
				if (!mysids2)
					goto out_unlock;
				memcpy(mysids2, mysids, mynel * sizeof(*mysids2));
				kfree(mysids);
				mysids = mysids2;
				mysids[mynel++] = sid;
			}
		}
	}
	rc = 0;
out_unlock:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
	if (rc || !mynel) {
		kfree(mysids);
		goto out;
=======
	rcu_read_unlock();
	if (rc || !mynel) {
		kfree(mysids);
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rc = -ENOMEM;
	mysids2 = kcalloc(mynel, sizeof(*mysids2), GFP_KERNEL);
	if (!mysids2) {
		kfree(mysids);
<<<<<<< HEAD
		goto out;
=======
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	for (i = 0, j = 0; i < mynel; i++) {
		struct av_decision dummy_avd;
		rc = avc_has_perm_noaudit(fromsid, mysids[i],
					  SECCLASS_PROCESS, /* kernel value */
					  PROCESS__TRANSITION, AVC_STRICT,
					  &dummy_avd);
		if (!rc)
			mysids2[j++] = mysids[i];
		cond_resched();
	}
<<<<<<< HEAD
	rc = 0;
	kfree(mysids);
	*sids = mysids2;
	*nel = j;
out:
	return rc;
=======
	kfree(mysids);
	*sids = mysids2;
	*nel = j;
	return 0;
}

/**
 * __security_genfs_sid - Helper to obtain a SID for a file in a filesystem
 * @policy: policy
 * @fstype: filesystem type
 * @path: path from root of mount
 * @orig_sclass: file security class
 * @sid: SID for path
 *
 * Obtain a SID to use for a file in a filesystem that
 * cannot support xattr or use a fixed labeling behavior like
 * transition SIDs or task SIDs.
 *
 * WARNING: This function may return -ESTALE, indicating that the caller
 * must retry the operation after re-acquiring the policy pointer!
 */
static inline int __security_genfs_sid(struct selinux_policy *policy,
				       const char *fstype,
				       const char *path,
				       u16 orig_sclass,
				       u32 *sid)
{
	struct policydb *policydb = &policy->policydb;
	struct sidtab *sidtab = policy->sidtab;
	u16 sclass;
	struct genfs *genfs;
	struct ocontext *c;
	int cmp = 0;

	while (path[0] == '/' && path[1] == '/')
		path++;

	sclass = unmap_class(&policy->map, orig_sclass);
	*sid = SECINITSID_UNLABELED;

	for (genfs = policydb->genfs; genfs; genfs = genfs->next) {
		cmp = strcmp(fstype, genfs->fstype);
		if (cmp <= 0)
			break;
	}

	if (!genfs || cmp)
		return -ENOENT;

	for (c = genfs->head; c; c = c->next) {
		size_t len = strlen(c->u.name);
		if ((!c->v.sclass || sclass == c->v.sclass) &&
		    (strncmp(c->u.name, path, len) == 0))
			break;
	}

	if (!c)
		return -ENOENT;

	return ocontext_to_sid(sidtab, c, 0, sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * security_genfs_sid - Obtain a SID for a file in a filesystem
 * @fstype: filesystem type
 * @path: path from root of mount
<<<<<<< HEAD
 * @sclass: file security class
 * @sid: SID for path
 *
 * Obtain a SID to use for a file in a filesystem that
 * cannot support xattr or use a fixed labeling behavior like
 * transition SIDs or task SIDs.
 */
int security_genfs_sid(const char *fstype,
		       char *path,
		       u16 orig_sclass,
		       u32 *sid)
{
	int len;
	u16 sclass;
	struct genfs *genfs;
	struct ocontext *c;
	int rc, cmp = 0;

	while (path[0] == '/' && path[1] == '/')
		path++;

	read_lock(&policy_rwlock);

	sclass = unmap_class(orig_sclass);
	*sid = SECINITSID_UNLABELED;

	for (genfs = policydb.genfs; genfs; genfs = genfs->next) {
		cmp = strcmp(fstype, genfs->fstype);
		if (cmp <= 0)
			break;
	}

	rc = -ENOENT;
	if (!genfs || cmp)
		goto out;

	for (c = genfs->head; c; c = c->next) {
		len = strlen(c->u.name);
		if ((!c->v.sclass || sclass == c->v.sclass) &&
		    (strncmp(c->u.name, path, len) == 0))
			break;
	}

	rc = -ENOENT;
	if (!c)
		goto out;

	if (!c->sid[0]) {
		rc = sidtab_context_to_sid(&sidtab, &c->context[0], &c->sid[0]);
		if (rc)
			goto out;
	}

	*sid = c->sid[0];
	rc = 0;
out:
	read_unlock(&policy_rwlock);
	return rc;
=======
 * @orig_sclass: file security class
 * @sid: SID for path
 *
 * Acquire policy_rwlock before calling __security_genfs_sid() and release
 * it afterward.
 */
int security_genfs_sid(const char *fstype,
		       const char *path,
		       u16 orig_sclass,
		       u32 *sid)
{
	struct selinux_policy *policy;
	int retval;

	if (!selinux_initialized()) {
		*sid = SECINITSID_UNLABELED;
		return 0;
	}

	do {
		rcu_read_lock();
		policy = rcu_dereference(selinux_state.policy);
		retval = __security_genfs_sid(policy, fstype, path,
					      orig_sclass, sid);
		rcu_read_unlock();
	} while (retval == -ESTALE);
	return retval;
}

int selinux_policy_genfs_sid(struct selinux_policy *policy,
			const char *fstype,
			const char *path,
			u16 orig_sclass,
			u32 *sid)
{
	/* no lock required, policy is not yet accessible by other threads */
	return __security_genfs_sid(policy, fstype, path, orig_sclass, sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * security_fs_use - Determine how to handle labeling for a filesystem.
<<<<<<< HEAD
 * @fstype: filesystem type
 * @behavior: labeling behavior
 * @sid: SID for filesystem (superblock)
 */
int security_fs_use(
	const char *fstype,
	unsigned int *behavior,
	u32 *sid)
{
	int rc = 0;
	struct ocontext *c;

	read_lock(&policy_rwlock);

	c = policydb.ocontexts[OCON_FSUSE];
=======
 * @sb: superblock in question
 */
int security_fs_use(struct super_block *sb)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
	int rc;
	struct ocontext *c;
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
	const char *fstype = sb->s_type->name;

	if (!selinux_initialized()) {
		sbsec->behavior = SECURITY_FS_USE_NONE;
		sbsec->sid = SECINITSID_UNLABELED;
		return 0;
	}

retry:
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	c = policydb->ocontexts[OCON_FSUSE];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (c) {
		if (strcmp(fstype, c->u.name) == 0)
			break;
		c = c->next;
	}

	if (c) {
<<<<<<< HEAD
		*behavior = c->v.behavior;
		if (!c->sid[0]) {
			rc = sidtab_context_to_sid(&sidtab, &c->context[0],
						   &c->sid[0]);
			if (rc)
				goto out;
		}
		*sid = c->sid[0];
	} else {
		rc = security_genfs_sid(fstype, "/", SECCLASS_DIR, sid);
		if (rc) {
			*behavior = SECURITY_FS_USE_NONE;
			rc = 0;
		} else {
			*behavior = SECURITY_FS_USE_GENFS;
=======
		sbsec->behavior = c->v.behavior;
		rc = ocontext_to_sid(sidtab, c, 0, &sbsec->sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;
	} else {
		rc = __security_genfs_sid(policy, fstype, "/",
					SECCLASS_DIR, &sbsec->sid);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc) {
			sbsec->behavior = SECURITY_FS_USE_NONE;
			rc = 0;
		} else {
			sbsec->behavior = SECURITY_FS_USE_GENFS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
	return rc;
}

int security_get_bools(int *len, char ***names, int **values)
{
	int i, rc;

	read_lock(&policy_rwlock);
=======
	rcu_read_unlock();
	return rc;
}

int security_get_bools(struct selinux_policy *policy,
		       u32 *len, char ***names, int **values)
{
	struct policydb *policydb;
	u32 i;
	int rc;

	policydb = &policy->policydb;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*names = NULL;
	*values = NULL;

	rc = 0;
<<<<<<< HEAD
	*len = policydb.p_bools.nprim;
=======
	*len = policydb->p_bools.nprim;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!*len)
		goto out;

	rc = -ENOMEM;
	*names = kcalloc(*len, sizeof(char *), GFP_ATOMIC);
	if (!*names)
		goto err;

	rc = -ENOMEM;
	*values = kcalloc(*len, sizeof(int), GFP_ATOMIC);
	if (!*values)
		goto err;

	for (i = 0; i < *len; i++) {
<<<<<<< HEAD
		size_t name_len;

		(*values)[i] = policydb.bool_val_to_struct[i]->state;
		name_len = strlen(sym_name(&policydb, SYM_BOOLS, i)) + 1;

		rc = -ENOMEM;
		(*names)[i] = kmalloc(sizeof(char) * name_len, GFP_ATOMIC);
		if (!(*names)[i])
			goto err;

		strncpy((*names)[i], sym_name(&policydb, SYM_BOOLS, i), name_len);
		(*names)[i][name_len - 1] = 0;
	}
	rc = 0;
out:
	read_unlock(&policy_rwlock);
=======
		(*values)[i] = policydb->bool_val_to_struct[i]->state;

		rc = -ENOMEM;
		(*names)[i] = kstrdup(sym_name(policydb, SYM_BOOLS, i),
				      GFP_ATOMIC);
		if (!(*names)[i])
			goto err;
	}
	rc = 0;
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
err:
	if (*names) {
		for (i = 0; i < *len; i++)
			kfree((*names)[i]);
<<<<<<< HEAD
	}
	kfree(*values);
=======
		kfree(*names);
	}
	kfree(*values);
	*len = 0;
	*names = NULL;
	*values = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}


<<<<<<< HEAD
int security_set_bools(int len, int *values)
{
	int i, rc;
	int lenp, seqno = 0;
	struct cond_node *cur;

	write_lock_irq(&policy_rwlock);

	rc = -EFAULT;
	lenp = policydb.p_bools.nprim;
	if (len != lenp)
		goto out;

	for (i = 0; i < len; i++) {
		if (!!values[i] != policydb.bool_val_to_struct[i]->state) {
			audit_log(current->audit_context, GFP_ATOMIC,
				AUDIT_MAC_CONFIG_CHANGE,
				"bool=%s val=%d old_val=%d auid=%u ses=%u",
				sym_name(&policydb, SYM_BOOLS, i),
				!!values[i],
				policydb.bool_val_to_struct[i]->state,
				audit_get_loginuid(current),
				audit_get_sessionid(current));
		}
		if (values[i])
			policydb.bool_val_to_struct[i]->state = 1;
		else
			policydb.bool_val_to_struct[i]->state = 0;
	}

	for (cur = policydb.cond_list; cur; cur = cur->next) {
		rc = evaluate_cond_node(&policydb, cur);
		if (rc)
			goto out;
	}

	seqno = ++latest_granting;
	rc = 0;
out:
	write_unlock_irq(&policy_rwlock);
	if (!rc) {
		avc_ss_reset(seqno);
		selnl_notify_policyload(seqno);
		selinux_status_update_policyload(seqno);
		selinux_xfrm_notify_policyload();
	}
	return rc;
}

int security_get_bool_value(int bool)
{
	int rc;
	int len;

	read_lock(&policy_rwlock);

	rc = -EFAULT;
	len = policydb.p_bools.nprim;
	if (bool >= len)
		goto out;

	rc = policydb.bool_val_to_struct[bool]->state;
out:
	read_unlock(&policy_rwlock);
	return rc;
}

static int security_preserve_bools(struct policydb *p)
{
	int rc, nbools = 0, *bvalues = NULL, i;
	char **bnames = NULL;
	struct cond_bool_datum *booldatum;
	struct cond_node *cur;

	rc = security_get_bools(&nbools, &bnames, &bvalues);
	if (rc)
		goto out;
	for (i = 0; i < nbools; i++) {
		booldatum = hashtab_search(p->p_bools.table, bnames[i]);
		if (booldatum)
			booldatum->state = bvalues[i];
	}
	for (cur = p->cond_list; cur; cur = cur->next) {
		rc = evaluate_cond_node(p, cur);
		if (rc)
			goto out;
	}
=======
int security_set_bools(u32 len, int *values)
{
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *newpolicy, *oldpolicy;
	int rc;
	u32 i, seqno = 0;

	if (!selinux_initialized())
		return -EINVAL;

	oldpolicy = rcu_dereference_protected(state->policy,
					lockdep_is_held(&state->policy_mutex));

	/* Consistency check on number of booleans, should never fail */
	if (WARN_ON(len != oldpolicy->policydb.p_bools.nprim))
		return -EINVAL;

	newpolicy = kmemdup(oldpolicy, sizeof(*newpolicy), GFP_KERNEL);
	if (!newpolicy)
		return -ENOMEM;

	/*
	 * Deep copy only the parts of the policydb that might be
	 * modified as a result of changing booleans.
	 */
	rc = cond_policydb_dup(&newpolicy->policydb, &oldpolicy->policydb);
	if (rc) {
		kfree(newpolicy);
		return -ENOMEM;
	}

	/* Update the boolean states in the copy */
	for (i = 0; i < len; i++) {
		int new_state = !!values[i];
		int old_state = newpolicy->policydb.bool_val_to_struct[i]->state;

		if (new_state != old_state) {
			audit_log(audit_context(), GFP_ATOMIC,
				AUDIT_MAC_CONFIG_CHANGE,
				"bool=%s val=%d old_val=%d auid=%u ses=%u",
				sym_name(&newpolicy->policydb, SYM_BOOLS, i),
				new_state,
				old_state,
				from_kuid(&init_user_ns, audit_get_loginuid(current)),
				audit_get_sessionid(current));
			newpolicy->policydb.bool_val_to_struct[i]->state = new_state;
		}
	}

	/* Re-evaluate the conditional rules in the copy */
	evaluate_cond_nodes(&newpolicy->policydb);

	/* Set latest granting seqno for new policy */
	newpolicy->latest_granting = oldpolicy->latest_granting + 1;
	seqno = newpolicy->latest_granting;

	/* Install the new policy */
	rcu_assign_pointer(state->policy, newpolicy);

	/*
	 * Free the conditional portions of the old policydb
	 * that were copied for the new policy, and the oldpolicy
	 * structure itself but not what it references.
	 */
	synchronize_rcu();
	selinux_policy_cond_free(oldpolicy);

	/* Notify others of the policy change */
	selinux_notify_policy_change(seqno);
	return 0;
}

int security_get_bool_value(u32 index)
{
	struct selinux_policy *policy;
	struct policydb *policydb;
	int rc;
	u32 len;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;

	rc = -EFAULT;
	len = policydb->p_bools.nprim;
	if (index >= len)
		goto out;

	rc = policydb->bool_val_to_struct[index]->state;
out:
	rcu_read_unlock();
	return rc;
}

static int security_preserve_bools(struct selinux_policy *oldpolicy,
				struct selinux_policy *newpolicy)
{
	int rc, *bvalues = NULL;
	char **bnames = NULL;
	struct cond_bool_datum *booldatum;
	u32 i, nbools = 0;

	rc = security_get_bools(oldpolicy, &nbools, &bnames, &bvalues);
	if (rc)
		goto out;
	for (i = 0; i < nbools; i++) {
		booldatum = symtab_search(&newpolicy->policydb.p_bools,
					bnames[i]);
		if (booldatum)
			booldatum->state = bvalues[i];
	}
	evaluate_cond_nodes(&newpolicy->policydb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	if (bnames) {
		for (i = 0; i < nbools; i++)
			kfree(bnames[i]);
	}
	kfree(bnames);
	kfree(bvalues);
	return rc;
}

/*
 * security_sid_mls_copy() - computes a new sid based on the given
 * sid and the mls portion of mls_sid.
 */
int security_sid_mls_copy(u32 sid, u32 mls_sid, u32 *new_sid)
{
<<<<<<< HEAD
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct context *context1;
	struct context *context2;
	struct context newcon;
	char *s;
	u32 len;
	int rc;

<<<<<<< HEAD
	rc = 0;
	if (!ss_initialized || !policydb.mls_enabled) {
		*new_sid = sid;
		goto out;
	}

	context_init(&newcon);

	read_lock(&policy_rwlock);

	rc = -EINVAL;
	context1 = sidtab_search(&sidtab, sid);
	if (!context1) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	if (!selinux_initialized()) {
		*new_sid = sid;
		return 0;
	}

retry:
	rc = 0;
	context_init(&newcon);

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	if (!policydb->mls_enabled) {
		*new_sid = sid;
		goto out_unlock;
	}

	rc = -EINVAL;
	context1 = sidtab_search(sidtab, sid);
	if (!context1) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, sid);
		goto out_unlock;
	}

	rc = -EINVAL;
<<<<<<< HEAD
	context2 = sidtab_search(&sidtab, mls_sid);
	if (!context2) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	context2 = sidtab_search(sidtab, mls_sid);
	if (!context2) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, mls_sid);
		goto out_unlock;
	}

	newcon.user = context1->user;
	newcon.role = context1->role;
	newcon.type = context1->type;
	rc = mls_context_cpy(&newcon, context2);
	if (rc)
		goto out_unlock;

	/* Check the validity of the new context. */
<<<<<<< HEAD
	if (!policydb_context_isvalid(&policydb, &newcon)) {
		rc = convert_context_handle_invalid_context(&newcon);
		if (rc) {
			if (!context_struct_to_string(&newcon, &s, &len)) {
				audit_log(current->audit_context, GFP_ATOMIC, AUDIT_SELINUX_ERR,
					  "security_sid_mls_copy: invalid context %s", s);
=======
	if (!policydb_context_isvalid(policydb, &newcon)) {
		rc = convert_context_handle_invalid_context(policydb,
							&newcon);
		if (rc) {
			if (!context_struct_to_string(policydb, &newcon, &s,
						      &len)) {
				struct audit_buffer *ab;

				ab = audit_log_start(audit_context(),
						     GFP_ATOMIC,
						     AUDIT_SELINUX_ERR);
				audit_log_format(ab,
						 "op=security_sid_mls_copy invalid_context=");
				/* don't record NUL with untrusted strings */
				audit_log_n_untrustedstring(ab, s, len - 1);
				audit_log_end(ab);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				kfree(s);
			}
			goto out_unlock;
		}
	}
<<<<<<< HEAD

	rc = sidtab_context_to_sid(&sidtab, &newcon, new_sid);
out_unlock:
	read_unlock(&policy_rwlock);
	context_destroy(&newcon);
out:
=======
	rc = sidtab_context_to_sid(sidtab, &newcon, new_sid);
	if (rc == -ESTALE) {
		rcu_read_unlock();
		context_destroy(&newcon);
		goto retry;
	}
out_unlock:
	rcu_read_unlock();
	context_destroy(&newcon);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

/**
 * security_net_peersid_resolve - Compare and resolve two network peer SIDs
 * @nlbl_sid: NetLabel SID
 * @nlbl_type: NetLabel labeling protocol type
 * @xfrm_sid: XFRM SID
<<<<<<< HEAD
=======
 * @peer_sid: network peer sid
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Description:
 * Compare the @nlbl_sid and @xfrm_sid values and if the two SIDs can be
 * resolved into a single SID it is returned via @peer_sid and the function
 * returns zero.  Otherwise @peer_sid is set to SECSID_NULL and the function
 * returns a negative value.  A table summarizing the behavior is below:
 *
 *                                 | function return |      @sid
 *   ------------------------------+-----------------+-----------------
 *   no peer labels                |        0        |    SECSID_NULL
 *   single peer label             |        0        |    <peer_label>
 *   multiple, consistent labels   |        0        |    <peer_label>
 *   multiple, inconsistent labels |    -<errno>     |    SECSID_NULL
 *
 */
int security_net_peersid_resolve(u32 nlbl_sid, u32 nlbl_type,
				 u32 xfrm_sid,
				 u32 *peer_sid)
{
<<<<<<< HEAD
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	struct context *nlbl_ctx;
	struct context *xfrm_ctx;

	*peer_sid = SECSID_NULL;

	/* handle the common (which also happens to be the set of easy) cases
	 * right away, these two if statements catch everything involving a
	 * single or absent peer SID/label */
	if (xfrm_sid == SECSID_NULL) {
		*peer_sid = nlbl_sid;
		return 0;
	}
	/* NOTE: an nlbl_type == NETLBL_NLTYPE_UNLABELED is a "fallback" label
	 * and is treated as if nlbl_sid == SECSID_NULL when a XFRM SID/label
	 * is present */
	if (nlbl_sid == SECSID_NULL || nlbl_type == NETLBL_NLTYPE_UNLABELED) {
		*peer_sid = xfrm_sid;
		return 0;
	}

<<<<<<< HEAD
	/* we don't need to check ss_initialized here since the only way both
	 * nlbl_sid and xfrm_sid are not equal to SECSID_NULL would be if the
	 * security server was initialized and ss_initialized was true */
	if (!policydb.mls_enabled)
		return 0;

	read_lock(&policy_rwlock);

	rc = -EINVAL;
	nlbl_ctx = sidtab_search(&sidtab, nlbl_sid);
	if (!nlbl_ctx) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;

	/*
	 * We don't need to check initialized here since the only way both
	 * nlbl_sid and xfrm_sid are not equal to SECSID_NULL would be if the
	 * security server was initialized and state->initialized was true.
	 */
	if (!policydb->mls_enabled) {
		rc = 0;
		goto out;
	}

	rc = -EINVAL;
	nlbl_ctx = sidtab_search(sidtab, nlbl_sid);
	if (!nlbl_ctx) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, nlbl_sid);
		goto out;
	}
	rc = -EINVAL;
<<<<<<< HEAD
	xfrm_ctx = sidtab_search(&sidtab, xfrm_sid);
	if (!xfrm_ctx) {
		printk(KERN_ERR "SELinux: %s:  unrecognized SID %d\n",
=======
	xfrm_ctx = sidtab_search(sidtab, xfrm_sid);
	if (!xfrm_ctx) {
		pr_err("SELinux: %s:  unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       __func__, xfrm_sid);
		goto out;
	}
	rc = (mls_context_cmp(nlbl_ctx, xfrm_ctx) ? 0 : -EACCES);
	if (rc)
		goto out;

	/* at present NetLabel SIDs/labels really only carry MLS
	 * information so if the MLS portion of the NetLabel SID
	 * matches the MLS portion of the labeled XFRM SID/label
	 * then pass along the XFRM SID as it is the most
	 * expressive */
	*peer_sid = xfrm_sid;
out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int get_classes_callback(void *k, void *d, void *args)
{
	struct class_datum *datum = d;
	char *name = k, **classes = args;
<<<<<<< HEAD
	int value = datum->value - 1;
=======
	u32 value = datum->value - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	classes[value] = kstrdup(name, GFP_ATOMIC);
	if (!classes[value])
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
int security_get_classes(char ***classes, int *nclasses)
{
	int rc;

	read_lock(&policy_rwlock);

	rc = -ENOMEM;
	*nclasses = policydb.p_classes.nprim;
=======
int security_get_classes(struct selinux_policy *policy,
			 char ***classes, u32 *nclasses)
{
	struct policydb *policydb;
	int rc;

	policydb = &policy->policydb;

	rc = -ENOMEM;
	*nclasses = policydb->p_classes.nprim;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*classes = kcalloc(*nclasses, sizeof(**classes), GFP_ATOMIC);
	if (!*classes)
		goto out;

<<<<<<< HEAD
	rc = hashtab_map(policydb.p_classes.table, get_classes_callback,
			*classes);
	if (rc) {
		int i;
=======
	rc = hashtab_map(&policydb->p_classes.table, get_classes_callback,
			 *classes);
	if (rc) {
		u32 i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < *nclasses; i++)
			kfree((*classes)[i]);
		kfree(*classes);
	}

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int get_permissions_callback(void *k, void *d, void *args)
{
	struct perm_datum *datum = d;
	char *name = k, **perms = args;
<<<<<<< HEAD
	int value = datum->value - 1;
=======
	u32 value = datum->value - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	perms[value] = kstrdup(name, GFP_ATOMIC);
	if (!perms[value])
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
int security_get_permissions(char *class, char ***perms, int *nperms)
{
	int rc, i;
	struct class_datum *match;

	read_lock(&policy_rwlock);

	rc = -EINVAL;
	match = hashtab_search(policydb.p_classes.table, class);
	if (!match) {
		printk(KERN_ERR "SELinux: %s:  unrecognized class %s\n",
=======
int security_get_permissions(struct selinux_policy *policy,
			     const char *class, char ***perms, u32 *nperms)
{
	struct policydb *policydb;
	u32 i;
	int rc;
	struct class_datum *match;

	policydb = &policy->policydb;

	rc = -EINVAL;
	match = symtab_search(&policydb->p_classes, class);
	if (!match) {
		pr_err("SELinux: %s:  unrecognized class %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, class);
		goto out;
	}

	rc = -ENOMEM;
	*nperms = match->permissions.nprim;
	*perms = kcalloc(*nperms, sizeof(**perms), GFP_ATOMIC);
	if (!*perms)
		goto out;

	if (match->comdatum) {
<<<<<<< HEAD
		rc = hashtab_map(match->comdatum->permissions.table,
				get_permissions_callback, *perms);
=======
		rc = hashtab_map(&match->comdatum->permissions.table,
				 get_permissions_callback, *perms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			goto err;
	}

<<<<<<< HEAD
	rc = hashtab_map(match->permissions.table, get_permissions_callback,
			*perms);
=======
	rc = hashtab_map(&match->permissions.table, get_permissions_callback,
			 *perms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		goto err;

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
	return rc;

err:
	read_unlock(&policy_rwlock);
=======
	return rc;

err:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < *nperms; i++)
		kfree((*perms)[i]);
	kfree(*perms);
	return rc;
}

int security_get_reject_unknown(void)
{
<<<<<<< HEAD
	return policydb.reject_unknown;
=======
	struct selinux_policy *policy;
	int value;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	value = policy->policydb.reject_unknown;
	rcu_read_unlock();
	return value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int security_get_allow_unknown(void)
{
<<<<<<< HEAD
	return policydb.allow_unknown;
=======
	struct selinux_policy *policy;
	int value;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	value = policy->policydb.allow_unknown;
	rcu_read_unlock();
	return value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * security_policycap_supported - Check for a specific policy capability
 * @req_cap: capability
 *
 * Description:
 * This function queries the currently loaded policy to see if it supports the
 * capability specified by @req_cap.  Returns true (1) if the capability is
 * supported, false (0) if it isn't supported.
 *
 */
int security_policycap_supported(unsigned int req_cap)
{
<<<<<<< HEAD
	int rc;

	read_lock(&policy_rwlock);
	rc = ebitmap_get_bit(&policydb.policycaps, req_cap);
	read_unlock(&policy_rwlock);
=======
	struct selinux_policy *policy;
	int rc;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	rc = ebitmap_get_bit(&policy->policydb.policycaps, req_cap);
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rc;
}

struct selinux_audit_rule {
	u32 au_seqno;
	struct context au_ctxt;
};

void selinux_audit_rule_free(void *vrule)
{
	struct selinux_audit_rule *rule = vrule;

	if (rule) {
		context_destroy(&rule->au_ctxt);
		kfree(rule);
	}
}

int selinux_audit_rule_init(u32 field, u32 op, char *rulestr, void **vrule)
{
<<<<<<< HEAD
=======
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *policy;
	struct policydb *policydb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct selinux_audit_rule *tmprule;
	struct role_datum *roledatum;
	struct type_datum *typedatum;
	struct user_datum *userdatum;
	struct selinux_audit_rule **rule = (struct selinux_audit_rule **)vrule;
	int rc = 0;

	*rule = NULL;

<<<<<<< HEAD
	if (!ss_initialized)
=======
	if (!selinux_initialized())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EOPNOTSUPP;

	switch (field) {
	case AUDIT_SUBJ_USER:
	case AUDIT_SUBJ_ROLE:
	case AUDIT_SUBJ_TYPE:
	case AUDIT_OBJ_USER:
	case AUDIT_OBJ_ROLE:
	case AUDIT_OBJ_TYPE:
		/* only 'equals' and 'not equals' fit user, role, and type */
		if (op != Audit_equal && op != Audit_not_equal)
			return -EINVAL;
		break;
	case AUDIT_SUBJ_SEN:
	case AUDIT_SUBJ_CLR:
	case AUDIT_OBJ_LEV_LOW:
	case AUDIT_OBJ_LEV_HIGH:
		/* we do not allow a range, indicated by the presence of '-' */
		if (strchr(rulestr, '-'))
			return -EINVAL;
		break;
	default:
		/* only the above fields are valid */
		return -EINVAL;
	}

	tmprule = kzalloc(sizeof(struct selinux_audit_rule), GFP_KERNEL);
	if (!tmprule)
		return -ENOMEM;
<<<<<<< HEAD

	context_init(&tmprule->au_ctxt);

	read_lock(&policy_rwlock);

	tmprule->au_seqno = latest_granting;

	switch (field) {
	case AUDIT_SUBJ_USER:
	case AUDIT_OBJ_USER:
		rc = -EINVAL;
		userdatum = hashtab_search(policydb.p_users.table, rulestr);
		if (!userdatum)
			goto out;
=======
	context_init(&tmprule->au_ctxt);

	rcu_read_lock();
	policy = rcu_dereference(state->policy);
	policydb = &policy->policydb;
	tmprule->au_seqno = policy->latest_granting;
	switch (field) {
	case AUDIT_SUBJ_USER:
	case AUDIT_OBJ_USER:
		userdatum = symtab_search(&policydb->p_users, rulestr);
		if (!userdatum) {
			rc = -EINVAL;
			goto err;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmprule->au_ctxt.user = userdatum->value;
		break;
	case AUDIT_SUBJ_ROLE:
	case AUDIT_OBJ_ROLE:
<<<<<<< HEAD
		rc = -EINVAL;
		roledatum = hashtab_search(policydb.p_roles.table, rulestr);
		if (!roledatum)
			goto out;
=======
		roledatum = symtab_search(&policydb->p_roles, rulestr);
		if (!roledatum) {
			rc = -EINVAL;
			goto err;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmprule->au_ctxt.role = roledatum->value;
		break;
	case AUDIT_SUBJ_TYPE:
	case AUDIT_OBJ_TYPE:
<<<<<<< HEAD
		rc = -EINVAL;
		typedatum = hashtab_search(policydb.p_types.table, rulestr);
		if (!typedatum)
			goto out;
=======
		typedatum = symtab_search(&policydb->p_types, rulestr);
		if (!typedatum) {
			rc = -EINVAL;
			goto err;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmprule->au_ctxt.type = typedatum->value;
		break;
	case AUDIT_SUBJ_SEN:
	case AUDIT_SUBJ_CLR:
	case AUDIT_OBJ_LEV_LOW:
	case AUDIT_OBJ_LEV_HIGH:
<<<<<<< HEAD
		rc = mls_from_string(rulestr, &tmprule->au_ctxt, GFP_ATOMIC);
		if (rc)
			goto out;
		break;
	}
	rc = 0;
out:
	read_unlock(&policy_rwlock);

	if (rc) {
		selinux_audit_rule_free(tmprule);
		tmprule = NULL;
	}

	*rule = tmprule;

=======
		rc = mls_from_string(policydb, rulestr, &tmprule->au_ctxt,
				     GFP_ATOMIC);
		if (rc)
			goto err;
		break;
	}
	rcu_read_unlock();

	*rule = tmprule;
	return 0;

err:
	rcu_read_unlock();
	selinux_audit_rule_free(tmprule);
	*rule = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

/* Check to see if the rule contains any selinux fields */
int selinux_audit_rule_known(struct audit_krule *rule)
{
<<<<<<< HEAD
	int i;
=======
	u32 i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < rule->field_count; i++) {
		struct audit_field *f = &rule->fields[i];
		switch (f->type) {
		case AUDIT_SUBJ_USER:
		case AUDIT_SUBJ_ROLE:
		case AUDIT_SUBJ_TYPE:
		case AUDIT_SUBJ_SEN:
		case AUDIT_SUBJ_CLR:
		case AUDIT_OBJ_USER:
		case AUDIT_OBJ_ROLE:
		case AUDIT_OBJ_TYPE:
		case AUDIT_OBJ_LEV_LOW:
		case AUDIT_OBJ_LEV_HIGH:
			return 1;
		}
	}

	return 0;
}

<<<<<<< HEAD
int selinux_audit_rule_match(u32 sid, u32 field, u32 op, void *vrule,
			     struct audit_context *actx)
{
=======
int selinux_audit_rule_match(u32 sid, u32 field, u32 op, void *vrule)
{
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *policy;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct context *ctxt;
	struct mls_level *level;
	struct selinux_audit_rule *rule = vrule;
	int match = 0;

<<<<<<< HEAD
	if (!rule) {
		audit_log(actx, GFP_ATOMIC, AUDIT_SELINUX_ERR,
			  "selinux_audit_rule_match: missing rule\n");
		return -ENOENT;
	}

	read_lock(&policy_rwlock);

	if (rule->au_seqno < latest_granting) {
		audit_log(actx, GFP_ATOMIC, AUDIT_SELINUX_ERR,
			  "selinux_audit_rule_match: stale rule\n");
=======
	if (unlikely(!rule)) {
		WARN_ONCE(1, "selinux_audit_rule_match: missing rule\n");
		return -ENOENT;
	}

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();

	policy = rcu_dereference(state->policy);

	if (rule->au_seqno < policy->latest_granting) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		match = -ESTALE;
		goto out;
	}

<<<<<<< HEAD
	ctxt = sidtab_search(&sidtab, sid);
	if (!ctxt) {
		audit_log(actx, GFP_ATOMIC, AUDIT_SELINUX_ERR,
			  "selinux_audit_rule_match: unrecognized SID %d\n",
=======
	ctxt = sidtab_search(policy->sidtab, sid);
	if (unlikely(!ctxt)) {
		WARN_ONCE(1, "selinux_audit_rule_match: unrecognized SID %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  sid);
		match = -ENOENT;
		goto out;
	}

	/* a field/op pair that is not caught here will simply fall through
	   without a match */
	switch (field) {
	case AUDIT_SUBJ_USER:
	case AUDIT_OBJ_USER:
		switch (op) {
		case Audit_equal:
			match = (ctxt->user == rule->au_ctxt.user);
			break;
		case Audit_not_equal:
			match = (ctxt->user != rule->au_ctxt.user);
			break;
		}
		break;
	case AUDIT_SUBJ_ROLE:
	case AUDIT_OBJ_ROLE:
		switch (op) {
		case Audit_equal:
			match = (ctxt->role == rule->au_ctxt.role);
			break;
		case Audit_not_equal:
			match = (ctxt->role != rule->au_ctxt.role);
			break;
		}
		break;
	case AUDIT_SUBJ_TYPE:
	case AUDIT_OBJ_TYPE:
		switch (op) {
		case Audit_equal:
			match = (ctxt->type == rule->au_ctxt.type);
			break;
		case Audit_not_equal:
			match = (ctxt->type != rule->au_ctxt.type);
			break;
		}
		break;
	case AUDIT_SUBJ_SEN:
	case AUDIT_SUBJ_CLR:
	case AUDIT_OBJ_LEV_LOW:
	case AUDIT_OBJ_LEV_HIGH:
		level = ((field == AUDIT_SUBJ_SEN ||
			  field == AUDIT_OBJ_LEV_LOW) ?
			 &ctxt->range.level[0] : &ctxt->range.level[1]);
		switch (op) {
		case Audit_equal:
			match = mls_level_eq(&rule->au_ctxt.range.level[0],
					     level);
			break;
		case Audit_not_equal:
			match = !mls_level_eq(&rule->au_ctxt.range.level[0],
					      level);
			break;
		case Audit_lt:
			match = (mls_level_dom(&rule->au_ctxt.range.level[0],
					       level) &&
				 !mls_level_eq(&rule->au_ctxt.range.level[0],
					       level));
			break;
		case Audit_le:
			match = mls_level_dom(&rule->au_ctxt.range.level[0],
					      level);
			break;
		case Audit_gt:
			match = (mls_level_dom(level,
					      &rule->au_ctxt.range.level[0]) &&
				 !mls_level_eq(level,
					       &rule->au_ctxt.range.level[0]));
			break;
		case Audit_ge:
			match = mls_level_dom(level,
					      &rule->au_ctxt.range.level[0]);
			break;
		}
	}

out:
<<<<<<< HEAD
	read_unlock(&policy_rwlock);
	return match;
}

static int (*aurule_callback)(void) = audit_update_lsm_rules;

static int aurule_avc_callback(u32 event, u32 ssid, u32 tsid,
			       u16 class, u32 perms, u32 *retained)
{
	int err = 0;

	if (event == AVC_CALLBACK_RESET && aurule_callback)
		err = aurule_callback();
	return err;
=======
	rcu_read_unlock();
	return match;
}

static int aurule_avc_callback(u32 event)
{
	if (event == AVC_CALLBACK_RESET)
		return audit_update_lsm_rules();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init aurule_init(void)
{
	int err;

<<<<<<< HEAD
	err = avc_add_callback(aurule_avc_callback, AVC_CALLBACK_RESET,
			       SECSID_NULL, SECSID_NULL, SECCLASS_NULL, 0);
=======
	err = avc_add_callback(aurule_avc_callback, AVC_CALLBACK_RESET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		panic("avc_add_callback() failed, error %d\n", err);

	return err;
}
__initcall(aurule_init);

#ifdef CONFIG_NETLABEL
/**
 * security_netlbl_cache_add - Add an entry to the NetLabel cache
 * @secattr: the NetLabel packet security attributes
 * @sid: the SELinux SID
 *
 * Description:
 * Attempt to cache the context in @ctx, which was derived from the packet in
 * @skb, in the NetLabel subsystem cache.  This function assumes @secattr has
 * already been initialized.
 *
 */
static void security_netlbl_cache_add(struct netlbl_lsm_secattr *secattr,
				      u32 sid)
{
	u32 *sid_cache;

	sid_cache = kmalloc(sizeof(*sid_cache), GFP_ATOMIC);
	if (sid_cache == NULL)
		return;
	secattr->cache = netlbl_secattr_cache_alloc(GFP_ATOMIC);
	if (secattr->cache == NULL) {
		kfree(sid_cache);
		return;
	}

	*sid_cache = sid;
	secattr->cache->free = kfree;
	secattr->cache->data = sid_cache;
	secattr->flags |= NETLBL_SECATTR_CACHE;
}

/**
 * security_netlbl_secattr_to_sid - Convert a NetLabel secattr to a SELinux SID
 * @secattr: the NetLabel packet security attributes
 * @sid: the SELinux SID
 *
 * Description:
 * Convert the given NetLabel security attributes in @secattr into a
 * SELinux SID.  If the @secattr field does not contain a full SELinux
 * SID/context then use SECINITSID_NETMSG as the foundation.  If possible the
 * 'cache' field of @secattr is set and the CACHE flag is set; this is to
 * allow the @secattr to be used by NetLabel to cache the secattr to SID
 * conversion for future lookups.  Returns zero on success, negative values on
 * failure.
 *
 */
int security_netlbl_secattr_to_sid(struct netlbl_lsm_secattr *secattr,
				   u32 *sid)
{
<<<<<<< HEAD
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	struct sidtab *sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	struct context *ctx;
	struct context ctx_new;

<<<<<<< HEAD
	if (!ss_initialized) {
=======
	if (!selinux_initialized()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*sid = SECSID_NULL;
		return 0;
	}

<<<<<<< HEAD
	read_lock(&policy_rwlock);
=======
retry:
	rc = 0;
	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;
	sidtab = policy->sidtab;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (secattr->flags & NETLBL_SECATTR_CACHE)
		*sid = *(u32 *)secattr->cache->data;
	else if (secattr->flags & NETLBL_SECATTR_SECID)
		*sid = secattr->attr.secid;
	else if (secattr->flags & NETLBL_SECATTR_MLS_LVL) {
		rc = -EIDRM;
<<<<<<< HEAD
		ctx = sidtab_search(&sidtab, SECINITSID_NETMSG);
=======
		ctx = sidtab_search(sidtab, SECINITSID_NETMSG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ctx == NULL)
			goto out;

		context_init(&ctx_new);
		ctx_new.user = ctx->user;
		ctx_new.role = ctx->role;
		ctx_new.type = ctx->type;
<<<<<<< HEAD
		mls_import_netlbl_lvl(&ctx_new, secattr);
		if (secattr->flags & NETLBL_SECATTR_MLS_CAT) {
			rc = ebitmap_netlbl_import(&ctx_new.range.level[0].cat,
						   secattr->attr.mls.cat);
			if (rc)
				goto out;
			memcpy(&ctx_new.range.level[1].cat,
			       &ctx_new.range.level[0].cat,
			       sizeof(ctx_new.range.level[0].cat));
		}
		rc = -EIDRM;
		if (!mls_context_isvalid(&policydb, &ctx_new))
			goto out_free;

		rc = sidtab_context_to_sid(&sidtab, &ctx_new, sid);
		if (rc)
			goto out_free;

		security_netlbl_cache_add(secattr, *sid);

		ebitmap_destroy(&ctx_new.range.level[0].cat);
	} else
		*sid = SECSID_NULL;

	read_unlock(&policy_rwlock);
	return 0;
out_free:
	ebitmap_destroy(&ctx_new.range.level[0].cat);
out:
	read_unlock(&policy_rwlock);
=======
		mls_import_netlbl_lvl(policydb, &ctx_new, secattr);
		if (secattr->flags & NETLBL_SECATTR_MLS_CAT) {
			rc = mls_import_netlbl_cat(policydb, &ctx_new, secattr);
			if (rc)
				goto out;
		}
		rc = -EIDRM;
		if (!mls_context_isvalid(policydb, &ctx_new)) {
			ebitmap_destroy(&ctx_new.range.level[0].cat);
			goto out;
		}

		rc = sidtab_context_to_sid(sidtab, &ctx_new, sid);
		ebitmap_destroy(&ctx_new.range.level[0].cat);
		if (rc == -ESTALE) {
			rcu_read_unlock();
			goto retry;
		}
		if (rc)
			goto out;

		security_netlbl_cache_add(secattr, *sid);
	} else
		*sid = SECSID_NULL;

out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

/**
 * security_netlbl_sid_to_secattr - Convert a SELinux SID to a NetLabel secattr
 * @sid: the SELinux SID
 * @secattr: the NetLabel packet security attributes
 *
 * Description:
 * Convert the given SELinux SID in @sid into a NetLabel security attribute.
 * Returns zero on success, negative values on failure.
 *
 */
int security_netlbl_sid_to_secattr(u32 sid, struct netlbl_lsm_secattr *secattr)
{
<<<<<<< HEAD
	int rc;
	struct context *ctx;

	if (!ss_initialized)
		return 0;

	read_lock(&policy_rwlock);

	rc = -ENOENT;
	ctx = sidtab_search(&sidtab, sid);
=======
	struct selinux_policy *policy;
	struct policydb *policydb;
	int rc;
	struct context *ctx;

	if (!selinux_initialized())
		return 0;

	rcu_read_lock();
	policy = rcu_dereference(selinux_state.policy);
	policydb = &policy->policydb;

	rc = -ENOENT;
	ctx = sidtab_search(policy->sidtab, sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ctx == NULL)
		goto out;

	rc = -ENOMEM;
<<<<<<< HEAD
	secattr->domain = kstrdup(sym_name(&policydb, SYM_TYPES, ctx->type - 1),
=======
	secattr->domain = kstrdup(sym_name(policydb, SYM_TYPES, ctx->type - 1),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  GFP_ATOMIC);
	if (secattr->domain == NULL)
		goto out;

	secattr->attr.secid = sid;
	secattr->flags |= NETLBL_SECATTR_DOMAIN_CPY | NETLBL_SECATTR_SECID;
<<<<<<< HEAD
	mls_export_netlbl_lvl(ctx, secattr);
	rc = mls_export_netlbl_cat(ctx, secattr);
out:
	read_unlock(&policy_rwlock);
=======
	mls_export_netlbl_lvl(policydb, ctx, secattr);
	rc = mls_export_netlbl_cat(policydb, ctx, secattr);
out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}
#endif /* CONFIG_NETLABEL */

/**
<<<<<<< HEAD
=======
 * __security_read_policy - read the policy.
 * @policy: SELinux policy
 * @data: binary policy data
 * @len: length of data in bytes
 *
 */
static int __security_read_policy(struct selinux_policy *policy,
				  void *data, size_t *len)
{
	int rc;
	struct policy_file fp;

	fp.data = data;
	fp.len = *len;

	rc = policydb_write(&policy->policydb, &fp);
	if (rc)
		return rc;

	*len = (unsigned long)fp.data - (unsigned long)data;
	return 0;
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * security_read_policy - read the policy.
 * @data: binary policy data
 * @len: length of data in bytes
 *
 */
int security_read_policy(void **data, size_t *len)
{
<<<<<<< HEAD
	int rc;
	struct policy_file fp;

	if (!ss_initialized)
		return -EINVAL;

	*len = security_policydb_len();

=======
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *policy;

	policy = rcu_dereference_protected(
			state->policy, lockdep_is_held(&state->policy_mutex));
	if (!policy)
		return -EINVAL;

	*len = policy->policydb.len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*data = vmalloc_user(*len);
	if (!*data)
		return -ENOMEM;

<<<<<<< HEAD
	fp.data = *data;
	fp.len = *len;

	read_lock(&policy_rwlock);
	rc = policydb_write(&policydb, &fp);
	read_unlock(&policy_rwlock);

	if (rc)
		return rc;

	*len = (unsigned long)fp.data - (unsigned long)*data;
	return 0;

=======
	return __security_read_policy(policy, *data, len);
}

/**
 * security_read_state_kernel - read the policy.
 * @data: binary policy data
 * @len: length of data in bytes
 *
 * Allocates kernel memory for reading SELinux policy.
 * This function is for internal use only and should not
 * be used for returning data to user space.
 *
 * This function must be called with policy_mutex held.
 */
int security_read_state_kernel(void **data, size_t *len)
{
	int err;
	struct selinux_state *state = &selinux_state;
	struct selinux_policy *policy;

	policy = rcu_dereference_protected(
			state->policy, lockdep_is_held(&state->policy_mutex));
	if (!policy)
		return -EINVAL;

	*len = policy->policydb.len;
	*data = vmalloc(*len);
	if (!*data)
		return -ENOMEM;

	err = __security_read_policy(policy, *data, len);
	if (err) {
		vfree(*data);
		*data = NULL;
		*len = 0;
	}
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
