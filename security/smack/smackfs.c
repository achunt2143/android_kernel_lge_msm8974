<<<<<<< HEAD
/*
 * Copyright (C) 2007 Casey Schaufler <casey@schaufler-ca.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *  	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2007 Casey Schaufler <casey@schaufler-ca.com>
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors:
 * 	Casey Schaufler <casey@schaufler-ca.com>
 * 	Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Special thanks to the authors of selinuxfs.
 *
 *	Karl MacMillan <kmacmillan@tresys.com>
 *	James Morris <jmorris@redhat.com>
<<<<<<< HEAD
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/security.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
<<<<<<< HEAD
#include <net/netlabel.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/cipso_ipv4.h>
#include <linux/seq_file.h>
#include <linux/ctype.h>
#include <linux/audit.h>
<<<<<<< HEAD
#include "smack.h"

=======
#include <linux/magic.h>
#include <linux/mount.h>
#include <linux/fs_context.h>
#include "smack.h"

#define BEBITS	(sizeof(__be32) * 8)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * smackfs pseudo filesystem.
 */

enum smk_inos {
	SMK_ROOT_INO	= 2,
	SMK_LOAD	= 3,	/* load policy */
	SMK_CIPSO	= 4,	/* load label -> CIPSO mapping */
	SMK_DOI		= 5,	/* CIPSO DOI */
	SMK_DIRECT	= 6,	/* CIPSO level indicating direct label */
	SMK_AMBIENT	= 7,	/* internet ambient label */
<<<<<<< HEAD
	SMK_NETLBLADDR	= 8,	/* single label hosts */
=======
	SMK_NET4ADDR	= 8,	/* single label hosts */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SMK_ONLYCAP	= 9,	/* the only "capable" label */
	SMK_LOGGING	= 10,	/* logging */
	SMK_LOAD_SELF	= 11,	/* task specific rules */
	SMK_ACCESSES	= 12,	/* access policy */
<<<<<<< HEAD
=======
	SMK_MAPPED	= 13,	/* CIPSO level indicating mapped label */
	SMK_LOAD2	= 14,	/* load policy with long labels */
	SMK_LOAD_SELF2	= 15,	/* load task specific rules with long labels */
	SMK_ACCESS2	= 16,	/* make an access check with long labels */
	SMK_CIPSO2	= 17,	/* load long label -> CIPSO mapping */
	SMK_REVOKE_SUBJ	= 18,	/* set rules with subject label to '-' */
	SMK_CHANGE_RULE	= 19,	/* change or add rules (long labels) */
	SMK_SYSLOG	= 20,	/* change syslog label) */
	SMK_PTRACE	= 21,	/* set ptrace rule */
#ifdef CONFIG_SECURITY_SMACK_BRINGUP
	SMK_UNCONFINED	= 22,	/* define an unconfined label */
#endif
#if IS_ENABLED(CONFIG_IPV6)
	SMK_NET6ADDR	= 23,	/* single label IPv6 hosts */
#endif /* CONFIG_IPV6 */
	SMK_RELABEL_SELF = 24, /* relabel possible without CAP_MAC_ADMIN */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * List locks
 */
<<<<<<< HEAD
static DEFINE_MUTEX(smack_list_lock);
static DEFINE_MUTEX(smack_cipso_lock);
static DEFINE_MUTEX(smack_ambient_lock);
static DEFINE_MUTEX(smk_netlbladdr_lock);
=======
static DEFINE_MUTEX(smack_cipso_lock);
static DEFINE_MUTEX(smack_ambient_lock);
static DEFINE_MUTEX(smk_net4addr_lock);
#if IS_ENABLED(CONFIG_IPV6)
static DEFINE_MUTEX(smk_net6addr_lock);
#endif /* CONFIG_IPV6 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the "ambient" label for network traffic.
 * If it isn't somehow marked, use this.
 * It can be reset via smackfs/ambient
 */
<<<<<<< HEAD
char *smack_net_ambient = smack_known_floor.smk_known;
=======
struct smack_known *smack_net_ambient;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the level in a CIPSO header that indicates a
 * smack label is contained directly in the category set.
 * It can be reset via smackfs/direct
 */
int smack_cipso_direct = SMACK_CIPSO_DIRECT_DEFAULT;

/*
<<<<<<< HEAD
 * Unless a process is running with this label even
 * having CAP_MAC_OVERRIDE isn't enough to grant
 * privilege to violate MAC policy. If no label is
 * designated (the NULL case) capabilities apply to
 * everyone. It is expected that the hat (^) label
 * will be used if any label is used.
 */
char *smack_onlycap;
=======
 * This is the level in a CIPSO header that indicates a
 * secid is contained directly in the category set.
 * It can be reset via smackfs/mapped
 */
int smack_cipso_mapped = SMACK_CIPSO_MAPPED_DEFAULT;

#ifdef CONFIG_SECURITY_SMACK_BRINGUP
/*
 * Allow one label to be unconfined. This is for
 * debugging and application bring-up purposes only.
 * It is bad and wrong, but everyone seems to expect
 * to have it.
 */
struct smack_known *smack_unconfined;
#endif

/*
 * If this value is set restrict syslog use to the label specified.
 * It can be reset via smackfs/syslog
 */
struct smack_known *smack_syslog_label;

/*
 * Ptrace current rule
 * SMACK_PTRACE_DEFAULT    regular smack ptrace rules (/proc based)
 * SMACK_PTRACE_EXACT      labels must match, but can be overriden with
 *			   CAP_SYS_PTRACE
 * SMACK_PTRACE_DRACONIAN  labels must match, CAP_SYS_PTRACE has no effect
 */
int smack_ptrace_rule = SMACK_PTRACE_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Certain IP addresses may be designated as single label hosts.
 * Packets are sent there unlabeled, but only from tasks that
 * can write to the specified label.
 */

<<<<<<< HEAD
LIST_HEAD(smk_netlbladdr_list);

/*
 * Rule lists are maintained for each label.
 * This master list is just for reading /smack/load.
 */
struct smack_master_list {
	struct list_head	list;
	struct smack_rule	*smk_rule;
};

LIST_HEAD(smack_rule_list);

static int smk_cipso_doi_value = SMACK_CIPSO_DOI_DEFAULT;

const char *smack_cipso_option = SMACK_CIPSO_OPTION;

=======
LIST_HEAD(smk_net4addr_list);
#if IS_ENABLED(CONFIG_IPV6)
LIST_HEAD(smk_net6addr_list);
#endif /* CONFIG_IPV6 */

/*
 * Rule lists are maintained for each label.
 */
struct smack_parsed_rule {
	struct smack_known	*smk_subject;
	struct smack_known	*smk_object;
	int			smk_access1;
	int			smk_access2;
};

static int smk_cipso_doi_value = SMACK_CIPSO_DOI_DEFAULT;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Values for parsing cipso rules
 * SMK_DIGITLEN: Length of a digit field in a rule.
 * SMK_CIPSOMIN: Minimum possible cipso rule length.
 * SMK_CIPSOMAX: Maximum possible cipso rule length.
 */
#define SMK_DIGITLEN 4
#define SMK_CIPSOMIN (SMK_LABELLEN + 2 * SMK_DIGITLEN)
#define SMK_CIPSOMAX (SMK_CIPSOMIN + SMACK_CIPSO_MAXCATNUM * SMK_DIGITLEN)

/*
 * Values for parsing MAC rules
 * SMK_ACCESS: Maximum possible combination of access permissions
 * SMK_ACCESSLEN: Maximum length for a rule access field
 * SMK_LOADLEN: Smack rule length
 */
#define SMK_OACCESS	"rwxa"
<<<<<<< HEAD
#define SMK_ACCESS	"rwxat"
=======
#define SMK_ACCESS	"rwxatl"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SMK_OACCESSLEN	(sizeof(SMK_OACCESS) - 1)
#define SMK_ACCESSLEN	(sizeof(SMK_ACCESS) - 1)
#define SMK_OLOADLEN	(SMK_LABELLEN + SMK_LABELLEN + SMK_OACCESSLEN)
#define SMK_LOADLEN	(SMK_LABELLEN + SMK_LABELLEN + SMK_ACCESSLEN)

<<<<<<< HEAD
=======
/*
 * Stricly for CIPSO level manipulation.
 * Set the category bit number in a smack label sized buffer.
 */
static inline void smack_catset_bit(unsigned int cat, char *catsetp)
{
	if (cat == 0 || cat > (SMK_CIPSOLEN * 8))
		return;

	catsetp[(cat - 1) / 8] |= 0x80 >> ((cat - 1) % 8);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * smk_netlabel_audit_set - fill a netlbl_audit struct
 * @nap: structure to fill
 */
static void smk_netlabel_audit_set(struct netlbl_audit *nap)
{
<<<<<<< HEAD
	nap->loginuid = audit_get_loginuid(current);
	nap->sessionid = audit_get_sessionid(current);
	nap->secid = smack_to_secid(smk_of_current());
}

/*
 * Values for parsing single label host rules
 * "1.2.3.4 X"
 * "192.168.138.129/32 abcdefghijklmnopqrstuvw"
 */
#define SMK_NETLBLADDRMIN	9
#define SMK_NETLBLADDRMAX	42

/**
 * smk_set_access - add a rule to the rule list
 * @srp: the new rule to add
=======
	struct smack_known *skp = smk_of_current();

	nap->loginuid = audit_get_loginuid(current);
	nap->sessionid = audit_get_sessionid(current);
	nap->secid = skp->smk_secid;
}

/*
 * Value for parsing single label host rules
 * "1.2.3.4 X"
 */
#define SMK_NETLBLADDRMIN	9

/**
 * smk_set_access - add a rule to the rule list or replace an old rule
 * @srp: the rule to add or replace
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @rule_list: the list of rules
 * @rule_lock: the rule list lock
 *
 * Looks through the current subject/object/access list for
 * the subject/object pair and replaces the access that was
 * there. If the pair isn't found add it with the specified
 * access.
 *
<<<<<<< HEAD
 * Returns 1 if a rule was found to exist already, 0 if it is new
 * Returns 0 if nothing goes wrong or -ENOMEM if it fails
 * during the allocation of the new pair to add.
 */
static int smk_set_access(struct smack_rule *srp, struct list_head *rule_list,
=======
 * Returns 0 if nothing goes wrong or -ENOMEM if it fails
 * during the allocation of the new pair to add.
 */
static int smk_set_access(struct smack_parsed_rule *srp,
				struct list_head *rule_list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct mutex *rule_lock)
{
	struct smack_rule *sp;
	int found = 0;
<<<<<<< HEAD
=======
	int rc = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(rule_lock);

	/*
	 * Because the object label is less likely to match
	 * than the subject label check it first
	 */
	list_for_each_entry_rcu(sp, rule_list, list) {
		if (sp->smk_object == srp->smk_object &&
		    sp->smk_subject == srp->smk_subject) {
			found = 1;
<<<<<<< HEAD
			sp->smk_access = srp->smk_access;
			break;
		}
	}
	if (found == 0)
		list_add_rcu(&srp->list, rule_list);

	mutex_unlock(rule_lock);

	return found;
=======
			sp->smk_access |= srp->smk_access1;
			sp->smk_access &= ~srp->smk_access2;
			break;
		}
	}

	if (found == 0) {
		sp = kmem_cache_zalloc(smack_rule_cache, GFP_KERNEL);
		if (sp == NULL) {
			rc = -ENOMEM;
			goto out;
		}

		sp->smk_subject = srp->smk_subject;
		sp->smk_object = srp->smk_object;
		sp->smk_access = srp->smk_access1 & ~srp->smk_access2;

		list_add_rcu(&sp->list, rule_list);
	}

out:
	mutex_unlock(rule_lock);
	return rc;
}

/**
 * smk_perm_from_str - parse smack accesses from a text string
 * @string: a text string that contains a Smack accesses code
 *
 * Returns an integer with respective bits set for specified accesses.
 */
static int smk_perm_from_str(const char *string)
{
	int perm = 0;
	const char *cp;

	for (cp = string; ; cp++)
		switch (*cp) {
		case '-':
			break;
		case 'r':
		case 'R':
			perm |= MAY_READ;
			break;
		case 'w':
		case 'W':
			perm |= MAY_WRITE;
			break;
		case 'x':
		case 'X':
			perm |= MAY_EXEC;
			break;
		case 'a':
		case 'A':
			perm |= MAY_APPEND;
			break;
		case 't':
		case 'T':
			perm |= MAY_TRANSMUTE;
			break;
		case 'l':
		case 'L':
			perm |= MAY_LOCK;
			break;
		case 'b':
		case 'B':
			perm |= MAY_BRINGUP;
			break;
		default:
			return perm;
		}
}

/**
 * smk_fill_rule - Fill Smack rule from strings
 * @subject: subject label string
 * @object: object label string
 * @access1: access string
 * @access2: string with permissions to be removed
 * @rule: Smack rule
 * @import: if non-zero, import labels
 * @len: label length limit
 *
 * Returns 0 on success, appropriate error code on failure.
 */
static int smk_fill_rule(const char *subject, const char *object,
				const char *access1, const char *access2,
				struct smack_parsed_rule *rule, int import,
				int len)
{
	const char *cp;
	struct smack_known *skp;

	if (import) {
		rule->smk_subject = smk_import_entry(subject, len);
		if (IS_ERR(rule->smk_subject))
			return PTR_ERR(rule->smk_subject);

		rule->smk_object = smk_import_entry(object, len);
		if (IS_ERR(rule->smk_object))
			return PTR_ERR(rule->smk_object);
	} else {
		cp = smk_parse_smack(subject, len);
		if (IS_ERR(cp))
			return PTR_ERR(cp);
		skp = smk_find_entry(cp);
		kfree(cp);
		if (skp == NULL)
			return -ENOENT;
		rule->smk_subject = skp;

		cp = smk_parse_smack(object, len);
		if (IS_ERR(cp))
			return PTR_ERR(cp);
		skp = smk_find_entry(cp);
		kfree(cp);
		if (skp == NULL)
			return -ENOENT;
		rule->smk_object = skp;
	}

	rule->smk_access1 = smk_perm_from_str(access1);
	if (access2)
		rule->smk_access2 = smk_perm_from_str(access2);
	else
		rule->smk_access2 = ~rule->smk_access1;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * smk_parse_rule - parse Smack rule from load string
 * @data: string to be parsed whose size is SMK_LOADLEN
 * @rule: Smack rule
 * @import: if non-zero, import labels
<<<<<<< HEAD
 */
static int smk_parse_rule(const char *data, struct smack_rule *rule, int import)
{
	char smack[SMK_LABELLEN];
	struct smack_known *skp;

	if (import) {
		rule->smk_subject = smk_import(data, 0);
		if (rule->smk_subject == NULL)
			return -1;

		rule->smk_object = smk_import(data + SMK_LABELLEN, 0);
		if (rule->smk_object == NULL)
			return -1;
	} else {
		smk_parse_smack(data, 0, smack);
		skp = smk_find_entry(smack);
		if (skp == NULL)
			return -1;
		rule->smk_subject = skp->smk_known;

		smk_parse_smack(data + SMK_LABELLEN, 0, smack);
		skp = smk_find_entry(smack);
		if (skp == NULL)
			return -1;
		rule->smk_object = skp->smk_known;
	}

	rule->smk_access = 0;

	switch (data[SMK_LABELLEN + SMK_LABELLEN]) {
	case '-':
		break;
	case 'r':
	case 'R':
		rule->smk_access |= MAY_READ;
		break;
	default:
		return -1;
	}

	switch (data[SMK_LABELLEN + SMK_LABELLEN + 1]) {
	case '-':
		break;
	case 'w':
	case 'W':
		rule->smk_access |= MAY_WRITE;
		break;
	default:
		return -1;
	}

	switch (data[SMK_LABELLEN + SMK_LABELLEN + 2]) {
	case '-':
		break;
	case 'x':
	case 'X':
		rule->smk_access |= MAY_EXEC;
		break;
	default:
		return -1;
	}

	switch (data[SMK_LABELLEN + SMK_LABELLEN + 3]) {
	case '-':
		break;
	case 'a':
	case 'A':
		rule->smk_access |= MAY_APPEND;
		break;
	default:
		return -1;
	}

	switch (data[SMK_LABELLEN + SMK_LABELLEN + 4]) {
	case '-':
		break;
	case 't':
	case 'T':
		rule->smk_access |= MAY_TRANSMUTE;
		break;
	default:
		return -1;
	}

	return 0;
}

/**
 * smk_write_load_list - write() for any /smack/load
=======
 *
 * Returns 0 on success, -1 on errors.
 */
static int smk_parse_rule(const char *data, struct smack_parsed_rule *rule,
				int import)
{
	int rc;

	rc = smk_fill_rule(data, data + SMK_LABELLEN,
			   data + SMK_LABELLEN + SMK_LABELLEN, NULL, rule,
			   import, SMK_LABELLEN);
	return rc;
}

/**
 * smk_parse_long_rule - parse Smack rule from rule string
 * @data: string to be parsed, null terminated
 * @rule: Will be filled with Smack parsed rule
 * @import: if non-zero, import labels
 * @tokens: number of substrings expected in data
 *
 * Returns number of processed bytes on success, -ERRNO on failure.
 */
static ssize_t smk_parse_long_rule(char *data, struct smack_parsed_rule *rule,
				int import, int tokens)
{
	ssize_t cnt = 0;
	char *tok[4];
	int rc;
	int i;

	/*
	 * Parsing the rule in-place, filling all white-spaces with '\0'
	 */
	for (i = 0; i < tokens; ++i) {
		while (isspace(data[cnt]))
			data[cnt++] = '\0';

		if (data[cnt] == '\0')
			/* Unexpected end of data */
			return -EINVAL;

		tok[i] = data + cnt;

		while (data[cnt] && !isspace(data[cnt]))
			++cnt;
	}
	while (isspace(data[cnt]))
		data[cnt++] = '\0';

	while (i < 4)
		tok[i++] = NULL;

	rc = smk_fill_rule(tok[0], tok[1], tok[2], tok[3], rule, import, 0);
	return rc == 0 ? cnt : rc;
}

#define SMK_FIXED24_FMT	0	/* Fixed 24byte label format */
#define SMK_LONG_FMT	1	/* Variable long label format */
#define SMK_CHANGE_FMT	2	/* Rule modification format */
/**
 * smk_write_rules_list - write() for any /smack rule file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 * @rule_list: the list of rules to write to
 * @rule_lock: lock for the rule list
<<<<<<< HEAD
 *
 * Get one smack access rule from above.
 * The format is exactly:
 *     char subject[SMK_LABELLEN]
 *     char object[SMK_LABELLEN]
 *     char access[SMK_ACCESSLEN]
 *
 * writes must be SMK_LABELLEN+SMK_LABELLEN+SMK_ACCESSLEN bytes.
 */
static ssize_t smk_write_load_list(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos,
				struct list_head *rule_list,
				struct mutex *rule_lock)
{
	struct smack_master_list *smlp;
	struct smack_known *skp;
	struct smack_rule *rule;
	char *data;
	int rc = -EINVAL;
	int load = 0;
=======
 * @format: /smack/load or /smack/load2 or /smack/change-rule format.
 *
 * Get one smack access rule from above.
 * The format for SMK_LONG_FMT is:
 *	"subject<whitespace>object<whitespace>access[<whitespace>...]"
 * The format for SMK_FIXED24_FMT is exactly:
 *	"subject                 object                  rwxat"
 * The format for SMK_CHANGE_FMT is:
 *	"subject<whitespace>object<whitespace>
 *	 acc_enable<whitespace>acc_disable[<whitespace>...]"
 */
static ssize_t smk_write_rules_list(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos,
					struct list_head *rule_list,
					struct mutex *rule_lock, int format)
{
	struct smack_parsed_rule rule;
	char *data;
	int rc;
	int trunc = 0;
	int tokens;
	ssize_t cnt = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * No partial writes.
	 * Enough data must be present.
	 */
	if (*ppos != 0)
		return -EINVAL;
<<<<<<< HEAD
	/*
	 * Minor hack for backward compatibility
	 */
	if (count < (SMK_OLOADLEN) || count > SMK_LOADLEN)
		return -EINVAL;

	data = kzalloc(SMK_LOADLEN, GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	if (copy_from_user(data, buf, count) != 0) {
		rc = -EFAULT;
		goto out;
	}

	/*
	 * More on the minor hack for backward compatibility
	 */
	if (count == (SMK_OLOADLEN))
		data[SMK_OLOADLEN] = '-';

	rule = kzalloc(sizeof(*rule), GFP_KERNEL);
	if (rule == NULL) {
		rc = -ENOMEM;
		goto out;
	}

	if (smk_parse_rule(data, rule, 1))
		goto out_free_rule;

	if (rule_list == NULL) {
		load = 1;
		skp = smk_find_entry(rule->smk_subject);
		rule_list = &skp->smk_rules;
		rule_lock = &skp->smk_rules_lock;
	}

	rc = count;
	/*
	 * If this is "load" as opposed to "load-self" and a new rule
	 * it needs to get added for reporting.
	 * smk_set_access returns true if there was already a rule
	 * for the subject/object pair, and false if it was new.
	 */
	if (load && !smk_set_access(rule, rule_list, rule_lock)) {
		smlp = kzalloc(sizeof(*smlp), GFP_KERNEL);
		if (smlp != NULL) {
			smlp->smk_rule = rule;
			list_add_rcu(&smlp->list, &smack_rule_list);
		} else
			rc = -ENOMEM;
		goto out;
	}

out_free_rule:
	kfree(rule);
=======

	if (format == SMK_FIXED24_FMT) {
		/*
		 * Minor hack for backward compatibility
		 */
		if (count < SMK_OLOADLEN || count > SMK_LOADLEN)
			return -EINVAL;
	} else {
		if (count >= PAGE_SIZE) {
			count = PAGE_SIZE - 1;
			trunc = 1;
		}
	}

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	/*
	 * In case of parsing only part of user buf,
	 * avoid having partial rule at the data buffer
	 */
	if (trunc) {
		while (count > 0 && (data[count - 1] != '\n'))
			--count;
		if (count == 0) {
			rc = -EINVAL;
			goto out;
		}
	}

	data[count] = '\0';
	tokens = (format == SMK_CHANGE_FMT ? 4 : 3);
	while (cnt < count) {
		if (format == SMK_FIXED24_FMT) {
			rc = smk_parse_rule(data, &rule, 1);
			if (rc < 0)
				goto out;
			cnt = count;
		} else {
			rc = smk_parse_long_rule(data + cnt, &rule, 1, tokens);
			if (rc < 0)
				goto out;
			if (rc == 0) {
				rc = -EINVAL;
				goto out;
			}
			cnt += rc;
		}

		if (rule_list == NULL)
			rc = smk_set_access(&rule, &rule.smk_subject->smk_rules,
				&rule.smk_subject->smk_rules_lock);
		else
			rc = smk_set_access(&rule, rule_list, rule_lock);

		if (rc)
			goto out;
	}

	rc = cnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(data);
	return rc;
}

/*
 * Core logic for smackfs seq list operations.
 */

static void *smk_seq_start(struct seq_file *s, loff_t *pos,
				struct list_head *head)
{
	struct list_head *list;
<<<<<<< HEAD

	/*
	 * This is 0 the first time through.
	 */
	if (s->index == 0)
		s->private = head;

	if (s->private == NULL)
		return NULL;

	list = s->private;
	if (list_empty(list))
		return NULL;

	if (s->index == 0)
		return list->next;
	return list;
=======
	int i = *pos;

	rcu_read_lock();
	for (list = rcu_dereference(list_next_rcu(head));
		list != head;
		list = rcu_dereference(list_next_rcu(list))) {
		if (i-- == 0)
			return list;
	}

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *smk_seq_next(struct seq_file *s, void *v, loff_t *pos,
				struct list_head *head)
{
	struct list_head *list = v;

<<<<<<< HEAD
	if (list_is_last(list, head)) {
		s->private = NULL;
		return NULL;
	}
	s->private = list->next;
	return list->next;
=======
	++*pos;
	list = rcu_dereference(list_next_rcu(list));

	return (list == head) ? NULL : list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void smk_seq_stop(struct seq_file *s, void *v)
{
<<<<<<< HEAD
	/* No-op */
}

/*
 * Seq_file read operations for /smack/load
 */

static void *load_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smack_rule_list);
}

static void *load_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smack_rule_list);
}

static int load_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_master_list *smlp =
		 list_entry(list, struct smack_master_list, list);
	struct smack_rule *srp = smlp->smk_rule;

	seq_printf(s, "%s %s", (char *)srp->smk_subject,
		   (char *)srp->smk_object);
=======
	rcu_read_unlock();
}

static void smk_rule_show(struct seq_file *s, struct smack_rule *srp, int max)
{
	/*
	 * Don't show any rules with label names too long for
	 * interface file (/smack/load or /smack/load2)
	 * because you should expect to be able to write
	 * anything you read back.
	 */
	if (strlen(srp->smk_subject->smk_known) >= max ||
	    strlen(srp->smk_object->smk_known) >= max)
		return;

	if (srp->smk_access == 0)
		return;

	seq_printf(s, "%s %s",
		   srp->smk_subject->smk_known,
		   srp->smk_object->smk_known);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	seq_putc(s, ' ');

	if (srp->smk_access & MAY_READ)
		seq_putc(s, 'r');
	if (srp->smk_access & MAY_WRITE)
		seq_putc(s, 'w');
	if (srp->smk_access & MAY_EXEC)
		seq_putc(s, 'x');
	if (srp->smk_access & MAY_APPEND)
		seq_putc(s, 'a');
	if (srp->smk_access & MAY_TRANSMUTE)
		seq_putc(s, 't');
<<<<<<< HEAD
	if (srp->smk_access == 0)
		seq_putc(s, '-');

	seq_putc(s, '\n');
=======
	if (srp->smk_access & MAY_LOCK)
		seq_putc(s, 'l');
	if (srp->smk_access & MAY_BRINGUP)
		seq_putc(s, 'b');

	seq_putc(s, '\n');
}

/*
 * Seq_file read operations for /smack/load
 */

static void *load2_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smack_known_list);
}

static void *load2_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smack_known_list);
}

static int load_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_rule *srp;
	struct smack_known *skp =
		list_entry_rcu(list, struct smack_known, list);

	list_for_each_entry_rcu(srp, &skp->smk_rules, list)
		smk_rule_show(s, srp, SMK_LABELLEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct seq_operations load_seq_ops = {
<<<<<<< HEAD
	.start = load_seq_start,
	.next  = load_seq_next,
=======
	.start = load2_seq_start,
	.next  = load2_seq_next,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.show  = load_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_load - open() for /smack/load
 * @inode: inode structure representing file
 * @file: "load" file pointer
 *
 * For reading, use load_seq_* seq_file reading operations.
 */
static int smk_open_load(struct inode *inode, struct file *file)
{
	return seq_open(file, &load_seq_ops);
}

/**
 * smk_write_load - write() for /smack/load
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 *
 */
static ssize_t smk_write_load(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Must have privilege.
	 * No partial writes.
	 * Enough data must be present.
	 */
<<<<<<< HEAD
	if (!capable(CAP_MAC_ADMIN))
		return -EPERM;

	return smk_write_load_list(file, buf, count, ppos, NULL, NULL);
=======
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	return smk_write_rules_list(file, buf, count, ppos, NULL, NULL,
				    SMK_FIXED24_FMT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations smk_load_ops = {
	.open           = smk_open_load,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_load,
	.release        = seq_release,
};

/**
 * smk_cipso_doi - initialize the CIPSO domain
 */
static void smk_cipso_doi(void)
{
	int rc;
	struct cipso_v4_doi *doip;
	struct netlbl_audit nai;

	smk_netlabel_audit_set(&nai);

	rc = netlbl_cfg_map_del(NULL, PF_INET, NULL, NULL, &nai);
	if (rc != 0)
		printk(KERN_WARNING "%s:%d remove rc = %d\n",
		       __func__, __LINE__, rc);

<<<<<<< HEAD
	doip = kmalloc(sizeof(struct cipso_v4_doi), GFP_KERNEL);
	if (doip == NULL)
		panic("smack:  Failed to initialize cipso DOI.\n");
=======
	doip = kmalloc(sizeof(struct cipso_v4_doi), GFP_KERNEL | __GFP_NOFAIL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	doip->map.std = NULL;
	doip->doi = smk_cipso_doi_value;
	doip->type = CIPSO_V4_MAP_PASS;
	doip->tags[0] = CIPSO_V4_TAG_RBITMAP;
	for (rc = 1; rc < CIPSO_V4_TAG_MAXCNT; rc++)
		doip->tags[rc] = CIPSO_V4_TAG_INVALID;

	rc = netlbl_cfg_cipsov4_add(doip, &nai);
	if (rc != 0) {
		printk(KERN_WARNING "%s:%d cipso add rc = %d\n",
		       __func__, __LINE__, rc);
		kfree(doip);
		return;
	}
	rc = netlbl_cfg_cipsov4_map_add(doip->doi, NULL, NULL, NULL, &nai);
	if (rc != 0) {
		printk(KERN_WARNING "%s:%d map add rc = %d\n",
		       __func__, __LINE__, rc);
<<<<<<< HEAD
		kfree(doip);
=======
		netlbl_cfg_cipsov4_del(doip->doi, &nai);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}
}

/**
 * smk_unlbl_ambient - initialize the unlabeled domain
 * @oldambient: previous domain string
 */
static void smk_unlbl_ambient(char *oldambient)
{
	int rc;
	struct netlbl_audit nai;

	smk_netlabel_audit_set(&nai);

	if (oldambient != NULL) {
		rc = netlbl_cfg_map_del(oldambient, PF_INET, NULL, NULL, &nai);
		if (rc != 0)
			printk(KERN_WARNING "%s:%d remove rc = %d\n",
			       __func__, __LINE__, rc);
	}
<<<<<<< HEAD

	rc = netlbl_cfg_unlbl_map_add(smack_net_ambient, PF_INET,
=======
	if (smack_net_ambient == NULL)
		smack_net_ambient = &smack_known_floor;

	rc = netlbl_cfg_unlbl_map_add(smack_net_ambient->smk_known, PF_INET,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      NULL, NULL, &nai);
	if (rc != 0)
		printk(KERN_WARNING "%s:%d add rc = %d\n",
		       __func__, __LINE__, rc);
}

/*
 * Seq_file read operations for /smack/cipso
 */

static void *cipso_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smack_known_list);
}

static void *cipso_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smack_known_list);
}

/*
 * Print cipso labels in format:
 * label level[/cat[,cat]]
 */
static int cipso_seq_show(struct seq_file *s, void *v)
{
	struct list_head  *list = v;
	struct smack_known *skp =
<<<<<<< HEAD
		 list_entry(list, struct smack_known, list);
	struct smack_cipso *scp = skp->smk_cipso;
	char *cbp;
	char sep = '/';
	int cat = 1;
	int i;
	unsigned char m;

	if (scp == NULL)
		return 0;

	seq_printf(s, "%s %3d", (char *)&skp->smk_known, scp->smk_level);

	cbp = scp->smk_catset;
	for (i = 0; i < SMK_LABELLEN; i++)
		for (m = 0x80; m != 0; m >>= 1) {
			if (m & cbp[i]) {
				seq_printf(s, "%c%d", sep, cat);
				sep = ',';
			}
			cat++;
		}
=======
		list_entry_rcu(list, struct smack_known, list);
	struct netlbl_lsm_catmap *cmp = skp->smk_netlabel.attr.mls.cat;
	char sep = '/';
	int i;

	/*
	 * Don't show a label that could not have been set using
	 * /smack/cipso. This is in support of the notion that
	 * anything read from /smack/cipso ought to be writeable
	 * to /smack/cipso.
	 *
	 * /smack/cipso2 should be used instead.
	 */
	if (strlen(skp->smk_known) >= SMK_LABELLEN)
		return 0;

	seq_printf(s, "%s %3d", skp->smk_known, skp->smk_netlabel.attr.mls.lvl);

	for (i = netlbl_catmap_walk(cmp, 0); i >= 0;
	     i = netlbl_catmap_walk(cmp, i + 1)) {
		seq_printf(s, "%c%d", sep, i);
		sep = ',';
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	seq_putc(s, '\n');

	return 0;
}

static const struct seq_operations cipso_seq_ops = {
	.start = cipso_seq_start,
	.next  = cipso_seq_next,
	.show  = cipso_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_cipso - open() for /smack/cipso
 * @inode: inode structure representing file
 * @file: "cipso" file pointer
 *
 * Connect our cipso_seq_* operations with /smack/cipso
 * file_operations
 */
static int smk_open_cipso(struct inode *inode, struct file *file)
{
	return seq_open(file, &cipso_seq_ops);
}

/**
<<<<<<< HEAD
 * smk_write_cipso - write() for /smack/cipso
=======
 * smk_set_cipso - do the work for write() for cipso and cipso2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
<<<<<<< HEAD
=======
 * @format: /smack/cipso or /smack/cipso2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Accepts only one cipso rule per write call.
 * Returns number of bytes written or error code, as appropriate
 */
<<<<<<< HEAD
static ssize_t smk_write_cipso(struct file *file, const char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct smack_known *skp;
	struct smack_cipso *scp = NULL;
	char mapcatset[SMK_LABELLEN];
	int maplevel;
	int cat;
=======
static ssize_t smk_set_cipso(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos, int format)
{
	struct netlbl_lsm_catmap *old_cat, *new_cat = NULL;
	struct smack_known *skp;
	struct netlbl_lsm_secattr ncats;
	char mapcatset[SMK_CIPSOLEN];
	int maplevel;
	unsigned int cat;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int catlen;
	ssize_t rc = -EINVAL;
	char *data = NULL;
	char *rule;
	int ret;
	int i;

	/*
	 * Must have privilege.
	 * No partial writes.
	 * Enough data must be present.
	 */
<<<<<<< HEAD
	if (!capable(CAP_MAC_ADMIN))
		return -EPERM;
	if (*ppos != 0)
		return -EINVAL;
	if (count < SMK_CIPSOMIN || count > SMK_CIPSOMAX)
		return -EINVAL;

	data = kzalloc(count + 1, GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	if (copy_from_user(data, buf, count) != 0) {
		rc = -EFAULT;
		goto unlockedout;
	}

	/* labels cannot begin with a '-' */
	if (data[0] == '-') {
		rc = -EINVAL;
		goto unlockedout;
	}
	data[count] = '\0';
=======
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;
	if (*ppos != 0)
		return -EINVAL;
	if (format == SMK_FIXED24_FMT &&
	    (count < SMK_CIPSOMIN || count > SMK_CIPSOMAX))
		return -EINVAL;
	if (count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rule = data;
	/*
	 * Only allow one writer at a time. Writes should be
	 * quite rare and small in any case.
	 */
	mutex_lock(&smack_cipso_lock);

	skp = smk_import_entry(rule, 0);
<<<<<<< HEAD
	if (skp == NULL)
		goto out;

	rule += SMK_LABELLEN;
	ret = sscanf(rule, "%d", &maplevel);
	if (ret != 1 || maplevel > SMACK_CIPSO_MAXLEVEL)
		goto out;

	rule += SMK_DIGITLEN;
	ret = sscanf(rule, "%d", &catlen);
	if (ret != 1 || catlen > SMACK_CIPSO_MAXCATNUM)
		goto out;

	if (count != (SMK_CIPSOMIN + catlen * SMK_DIGITLEN))
=======
	if (IS_ERR(skp)) {
		rc = PTR_ERR(skp);
		goto out;
	}

	if (format == SMK_FIXED24_FMT)
		rule += SMK_LABELLEN;
	else
		rule += strlen(skp->smk_known) + 1;

	if (rule > data + count) {
		rc = -EOVERFLOW;
		goto out;
	}

	ret = sscanf(rule, "%d", &maplevel);
	if (ret != 1 || maplevel < 0 || maplevel > SMACK_CIPSO_MAXLEVEL)
		goto out;

	rule += SMK_DIGITLEN;
	if (rule > data + count) {
		rc = -EOVERFLOW;
		goto out;
	}

	ret = sscanf(rule, "%d", &catlen);
	if (ret != 1 || catlen < 0 || catlen > SMACK_CIPSO_MAXCATNUM)
		goto out;

	if (format == SMK_FIXED24_FMT &&
	    count != (SMK_CIPSOMIN + catlen * SMK_DIGITLEN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	memset(mapcatset, 0, sizeof(mapcatset));

	for (i = 0; i < catlen; i++) {
		rule += SMK_DIGITLEN;
<<<<<<< HEAD
		ret = sscanf(rule, "%d", &cat);
		if (ret != 1 || cat > SMACK_CIPSO_MAXCATVAL)
=======
		if (rule > data + count) {
			rc = -EOVERFLOW;
			goto out;
		}
		ret = sscanf(rule, "%u", &cat);
		if (ret != 1 || cat > SMACK_CIPSO_MAXCATNUM)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;

		smack_catset_bit(cat, mapcatset);
	}
<<<<<<< HEAD

	if (skp->smk_cipso == NULL) {
		scp = kzalloc(sizeof(struct smack_cipso), GFP_KERNEL);
		if (scp == NULL) {
			rc = -ENOMEM;
			goto out;
		}
	}

	spin_lock_bh(&skp->smk_cipsolock);

	if (scp == NULL)
		scp = skp->smk_cipso;
	else
		skp->smk_cipso = scp;

	scp->smk_level = maplevel;
	memcpy(scp->smk_catset, mapcatset, sizeof(mapcatset));

	spin_unlock_bh(&skp->smk_cipsolock);

	rc = count;
out:
	mutex_unlock(&smack_cipso_lock);
unlockedout:
=======
	ncats.flags = 0;
	if (catlen == 0) {
		ncats.attr.mls.cat = NULL;
		ncats.attr.mls.lvl = maplevel;
		new_cat = netlbl_catmap_alloc(GFP_ATOMIC);
		if (new_cat)
			new_cat->next = ncats.attr.mls.cat;
		ncats.attr.mls.cat = new_cat;
		skp->smk_netlabel.flags &= ~(1U << 3);
		rc = 0;
	} else {
		rc = smk_netlbl_mls(maplevel, mapcatset, &ncats, SMK_CIPSOLEN);
	}
	if (rc >= 0) {
		old_cat = skp->smk_netlabel.attr.mls.cat;
		skp->smk_netlabel.attr.mls.cat = ncats.attr.mls.cat;
		skp->smk_netlabel.attr.mls.lvl = ncats.attr.mls.lvl;
		synchronize_rcu();
		netlbl_catmap_free(old_cat);
		rc = count;
		/*
		 * This mapping may have been cached, so clear the cache.
		 */
		netlbl_cache_invalidate();
	}

out:
	mutex_unlock(&smack_cipso_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(data);
	return rc;
}

<<<<<<< HEAD
=======
/**
 * smk_write_cipso - write() for /smack/cipso
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Accepts only one cipso rule per write call.
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_cipso(struct file *file, const char __user *buf,
			       size_t count, loff_t *ppos)
{
	return smk_set_cipso(file, buf, count, ppos, SMK_FIXED24_FMT);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct file_operations smk_cipso_ops = {
	.open           = smk_open_cipso,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_cipso,
	.release        = seq_release,
};

/*
<<<<<<< HEAD
 * Seq_file read operations for /smack/netlabel
 */

static void *netlbladdr_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smk_netlbladdr_list);
}

static void *netlbladdr_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smk_netlbladdr_list);
}
#define BEBITS	(sizeof(__be32) * 8)

/*
 * Print host/label pairs
 */
static int netlbladdr_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smk_netlbladdr *skp =
			 list_entry(list, struct smk_netlbladdr, list);
	unsigned char *hp = (char *) &skp->smk_host.sin_addr.s_addr;
	int maskn;
	u32 temp_mask = be32_to_cpu(skp->smk_mask.s_addr);

	for (maskn = 0; temp_mask; temp_mask <<= 1, maskn++);

	seq_printf(s, "%u.%u.%u.%u/%d %s\n",
		hp[0], hp[1], hp[2], hp[3], maskn, skp->smk_label);
=======
 * Seq_file read operations for /smack/cipso2
 */

/*
 * Print cipso labels in format:
 * label level[/cat[,cat]]
 */
static int cipso2_seq_show(struct seq_file *s, void *v)
{
	struct list_head  *list = v;
	struct smack_known *skp =
		list_entry_rcu(list, struct smack_known, list);
	struct netlbl_lsm_catmap *cmp = skp->smk_netlabel.attr.mls.cat;
	char sep = '/';
	int i;

	seq_printf(s, "%s %3d", skp->smk_known, skp->smk_netlabel.attr.mls.lvl);

	for (i = netlbl_catmap_walk(cmp, 0); i >= 0;
	     i = netlbl_catmap_walk(cmp, i + 1)) {
		seq_printf(s, "%c%d", sep, i);
		sep = ',';
	}

	seq_putc(s, '\n');
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct seq_operations netlbladdr_seq_ops = {
	.start = netlbladdr_seq_start,
	.next  = netlbladdr_seq_next,
	.show  = netlbladdr_seq_show,
=======
static const struct seq_operations cipso2_seq_ops = {
	.start = cipso_seq_start,
	.next  = cipso_seq_next,
	.show  = cipso2_seq_show,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.stop  = smk_seq_stop,
};

/**
<<<<<<< HEAD
 * smk_open_netlbladdr - open() for /smack/netlabel
 * @inode: inode structure representing file
 * @file: "netlabel" file pointer
 *
 * Connect our netlbladdr_seq_* operations with /smack/netlabel
 * file_operations
 */
static int smk_open_netlbladdr(struct inode *inode, struct file *file)
{
	return seq_open(file, &netlbladdr_seq_ops);
}

/**
 * smk_netlbladdr_insert
 * @new : netlabel to insert
 *
 * This helper insert netlabel in the smack_netlbladdrs list
 * sorted by netmask length (longest to smallest)
 * locked by &smk_netlbladdr_lock in smk_write_netlbladdr
 *
 */
static void smk_netlbladdr_insert(struct smk_netlbladdr *new)
{
	struct smk_netlbladdr *m, *m_next;

	if (list_empty(&smk_netlbladdr_list)) {
		list_add_rcu(&new->list, &smk_netlbladdr_list);
		return;
	}

	m = list_entry_rcu(smk_netlbladdr_list.next,
			   struct smk_netlbladdr, list);

	/* the comparison '>' is a bit hacky, but works */
	if (new->smk_mask.s_addr > m->smk_mask.s_addr) {
		list_add_rcu(&new->list, &smk_netlbladdr_list);
		return;
	}

	list_for_each_entry_rcu(m, &smk_netlbladdr_list, list) {
		if (list_is_last(&m->list, &smk_netlbladdr_list)) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
		m_next = list_entry_rcu(m->list.next,
					struct smk_netlbladdr, list);
		if (new->smk_mask.s_addr > m_next->smk_mask.s_addr) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
	}
}


/**
 * smk_write_netlbladdr - write() for /smack/netlabel
=======
 * smk_open_cipso2 - open() for /smack/cipso2
 * @inode: inode structure representing file
 * @file: "cipso2" file pointer
 *
 * Connect our cipso_seq_* operations with /smack/cipso2
 * file_operations
 */
static int smk_open_cipso2(struct inode *inode, struct file *file)
{
	return seq_open(file, &cipso2_seq_ops);
}

/**
 * smk_write_cipso2 - write() for /smack/cipso2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
<<<<<<< HEAD
 * Accepts only one netlbladdr per write call.
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_netlbladdr(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct smk_netlbladdr *skp;
	struct sockaddr_in newname;
	char smack[SMK_LABELLEN];
	char *sp;
	char data[SMK_NETLBLADDRMAX + 1];
=======
 * Accepts only one cipso rule per write call.
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_cipso2(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	return smk_set_cipso(file, buf, count, ppos, SMK_LONG_FMT);
}

static const struct file_operations smk_cipso2_ops = {
	.open           = smk_open_cipso2,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_cipso2,
	.release        = seq_release,
};

/*
 * Seq_file read operations for /smack/netlabel
 */

static void *net4addr_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smk_net4addr_list);
}

static void *net4addr_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smk_net4addr_list);
}

/*
 * Print host/label pairs
 */
static int net4addr_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smk_net4addr *skp =
			list_entry_rcu(list, struct smk_net4addr, list);
	char *kp = SMACK_CIPSO_OPTION;

	if (skp->smk_label != NULL)
		kp = skp->smk_label->smk_known;
	seq_printf(s, "%pI4/%d %s\n", &skp->smk_host.s_addr,
			skp->smk_masks, kp);

	return 0;
}

static const struct seq_operations net4addr_seq_ops = {
	.start = net4addr_seq_start,
	.next  = net4addr_seq_next,
	.show  = net4addr_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_net4addr - open() for /smack/netlabel
 * @inode: inode structure representing file
 * @file: "netlabel" file pointer
 *
 * Connect our net4addr_seq_* operations with /smack/netlabel
 * file_operations
 */
static int smk_open_net4addr(struct inode *inode, struct file *file)
{
	return seq_open(file, &net4addr_seq_ops);
}

/**
 * smk_net4addr_insert
 * @new : netlabel to insert
 *
 * This helper insert netlabel in the smack_net4addrs list
 * sorted by netmask length (longest to smallest)
 * locked by &smk_net4addr_lock in smk_write_net4addr
 *
 */
static void smk_net4addr_insert(struct smk_net4addr *new)
{
	struct smk_net4addr *m;
	struct smk_net4addr *m_next;

	if (list_empty(&smk_net4addr_list)) {
		list_add_rcu(&new->list, &smk_net4addr_list);
		return;
	}

	m = list_entry_rcu(smk_net4addr_list.next,
			   struct smk_net4addr, list);

	/* the comparison '>' is a bit hacky, but works */
	if (new->smk_masks > m->smk_masks) {
		list_add_rcu(&new->list, &smk_net4addr_list);
		return;
	}

	list_for_each_entry_rcu(m, &smk_net4addr_list, list) {
		if (list_is_last(&m->list, &smk_net4addr_list)) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
		m_next = list_entry_rcu(m->list.next,
					struct smk_net4addr, list);
		if (new->smk_masks > m_next->smk_masks) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
	}
}


/**
 * smk_write_net4addr - write() for /smack/netlabel
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Accepts only one net4addr per write call.
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_net4addr(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct smk_net4addr *snp;
	struct sockaddr_in newname;
	char *smack;
	struct smack_known *skp = NULL;
	char *data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *host = (char *)&newname.sin_addr.s_addr;
	int rc;
	struct netlbl_audit audit_info;
	struct in_addr mask;
	unsigned int m;
<<<<<<< HEAD
=======
	unsigned int masks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int found;
	u32 mask_bits = (1<<31);
	__be32 nsa;
	u32 temp_mask;

	/*
	 * Must have privilege.
	 * No partial writes.
	 * Enough data must be present.
	 * "<addr/mask, as a.b.c.d/e><space><label>"
	 * "<addr, as a.b.c.d><space><label>"
	 */
<<<<<<< HEAD
	if (!capable(CAP_MAC_ADMIN))
		return -EPERM;
	if (*ppos != 0)
		return -EINVAL;
	if (count < SMK_NETLBLADDRMIN || count > SMK_NETLBLADDRMAX)
		return -EINVAL;
	if (copy_from_user(data, buf, count) != 0)
		return -EFAULT;

	data[count] = '\0';

	rc = sscanf(data, "%hhd.%hhd.%hhd.%hhd/%d %s",
		&host[0], &host[1], &host[2], &host[3], &m, smack);
	if (rc != 6) {
		rc = sscanf(data, "%hhd.%hhd.%hhd.%hhd %s",
			&host[0], &host[1], &host[2], &host[3], smack);
		if (rc != 5)
			return -EINVAL;
		m = BEBITS;
	}
	if (m > BEBITS)
		return -EINVAL;

	/* if smack begins with '-', its an option, don't import it */
	if (smack[0] != '-') {
		sp = smk_import(smack, 0);
		if (sp == NULL)
			return -EINVAL;
	} else {
		/* check known options */
		if (strcmp(smack, smack_cipso_option) == 0)
			sp = (char *)smack_cipso_option;
		else
			return -EINVAL;
	}

	for (temp_mask = 0; m > 0; m--) {
=======
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;
	if (*ppos != 0)
		return -EINVAL;
	if (count < SMK_NETLBLADDRMIN || count > PAGE_SIZE - 1)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	smack = kzalloc(count + 1, GFP_KERNEL);
	if (smack == NULL) {
		rc = -ENOMEM;
		goto free_data_out;
	}

	rc = sscanf(data, "%hhd.%hhd.%hhd.%hhd/%u %s",
		&host[0], &host[1], &host[2], &host[3], &masks, smack);
	if (rc != 6) {
		rc = sscanf(data, "%hhd.%hhd.%hhd.%hhd %s",
			&host[0], &host[1], &host[2], &host[3], smack);
		if (rc != 5) {
			rc = -EINVAL;
			goto free_out;
		}
		masks = 32;
	}
	if (masks > BEBITS) {
		rc = -EINVAL;
		goto free_out;
	}

	/*
	 * If smack begins with '-', it is an option, don't import it
	 */
	if (smack[0] != '-') {
		skp = smk_import_entry(smack, 0);
		if (IS_ERR(skp)) {
			rc = PTR_ERR(skp);
			goto free_out;
		}
	} else {
		/*
		 * Only the -CIPSO option is supported for IPv4
		 */
		if (strcmp(smack, SMACK_CIPSO_OPTION) != 0) {
			rc = -EINVAL;
			goto free_out;
		}
	}

	for (m = masks, temp_mask = 0; m > 0; m--) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		temp_mask |= mask_bits;
		mask_bits >>= 1;
	}
	mask.s_addr = cpu_to_be32(temp_mask);

	newname.sin_addr.s_addr &= mask.s_addr;
	/*
	 * Only allow one writer at a time. Writes should be
	 * quite rare and small in any case.
	 */
<<<<<<< HEAD
	mutex_lock(&smk_netlbladdr_lock);
=======
	mutex_lock(&smk_net4addr_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nsa = newname.sin_addr.s_addr;
	/* try to find if the prefix is already in the list */
	found = 0;
<<<<<<< HEAD
	list_for_each_entry_rcu(skp, &smk_netlbladdr_list, list) {
		if (skp->smk_host.sin_addr.s_addr == nsa &&
		    skp->smk_mask.s_addr == mask.s_addr) {
=======
	list_for_each_entry_rcu(snp, &smk_net4addr_list, list) {
		if (snp->smk_host.s_addr == nsa && snp->smk_masks == masks) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			found = 1;
			break;
		}
	}
	smk_netlabel_audit_set(&audit_info);

	if (found == 0) {
<<<<<<< HEAD
		skp = kzalloc(sizeof(*skp), GFP_KERNEL);
		if (skp == NULL)
			rc = -ENOMEM;
		else {
			rc = 0;
			skp->smk_host.sin_addr.s_addr = newname.sin_addr.s_addr;
			skp->smk_mask.s_addr = mask.s_addr;
			skp->smk_label = sp;
			smk_netlbladdr_insert(skp);
		}
	} else {
		/* we delete the unlabeled entry, only if the previous label
		 * wasn't the special CIPSO option */
		if (skp->smk_label != smack_cipso_option)
			rc = netlbl_cfg_unlbl_static_del(&init_net, NULL,
					&skp->smk_host.sin_addr, &skp->smk_mask,
					PF_INET, &audit_info);
		else
			rc = 0;
		skp->smk_label = sp;
=======
		snp = kzalloc(sizeof(*snp), GFP_KERNEL);
		if (snp == NULL)
			rc = -ENOMEM;
		else {
			rc = 0;
			snp->smk_host.s_addr = newname.sin_addr.s_addr;
			snp->smk_mask.s_addr = mask.s_addr;
			snp->smk_label = skp;
			snp->smk_masks = masks;
			smk_net4addr_insert(snp);
		}
	} else {
		/*
		 * Delete the unlabeled entry, only if the previous label
		 * wasn't the special CIPSO option
		 */
		if (snp->smk_label != NULL)
			rc = netlbl_cfg_unlbl_static_del(&init_net, NULL,
					&snp->smk_host, &snp->smk_mask,
					PF_INET, &audit_info);
		else
			rc = 0;
		snp->smk_label = skp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Now tell netlabel about the single label nature of
	 * this host so that incoming packets get labeled.
	 * but only if we didn't get the special CIPSO option
	 */
<<<<<<< HEAD
	if (rc == 0 && sp != smack_cipso_option)
		rc = netlbl_cfg_unlbl_static_add(&init_net, NULL,
			&skp->smk_host.sin_addr, &skp->smk_mask, PF_INET,
			smack_to_secid(skp->smk_label), &audit_info);
=======
	if (rc == 0 && skp != NULL)
		rc = netlbl_cfg_unlbl_static_add(&init_net, NULL,
			&snp->smk_host, &snp->smk_mask, PF_INET,
			snp->smk_label->smk_secid, &audit_info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rc == 0)
		rc = count;

<<<<<<< HEAD
	mutex_unlock(&smk_netlbladdr_lock);
=======
	mutex_unlock(&smk_net4addr_lock);

free_out:
	kfree(smack);
free_data_out:
	kfree(data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rc;
}

<<<<<<< HEAD
static const struct file_operations smk_netlbladdr_ops = {
	.open           = smk_open_netlbladdr,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_netlbladdr,
	.release        = seq_release,
};

=======
static const struct file_operations smk_net4addr_ops = {
	.open           = smk_open_net4addr,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_net4addr,
	.release        = seq_release,
};

#if IS_ENABLED(CONFIG_IPV6)
/*
 * Seq_file read operations for /smack/netlabel6
 */

static void *net6addr_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smk_net6addr_list);
}

static void *net6addr_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smk_net6addr_list);
}

/*
 * Print host/label pairs
 */
static int net6addr_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smk_net6addr *skp =
			 list_entry(list, struct smk_net6addr, list);

	if (skp->smk_label != NULL)
		seq_printf(s, "%pI6/%d %s\n", &skp->smk_host, skp->smk_masks,
				skp->smk_label->smk_known);

	return 0;
}

static const struct seq_operations net6addr_seq_ops = {
	.start = net6addr_seq_start,
	.next  = net6addr_seq_next,
	.show  = net6addr_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_net6addr - open() for /smack/netlabel
 * @inode: inode structure representing file
 * @file: "netlabel" file pointer
 *
 * Connect our net6addr_seq_* operations with /smack/netlabel
 * file_operations
 */
static int smk_open_net6addr(struct inode *inode, struct file *file)
{
	return seq_open(file, &net6addr_seq_ops);
}

/**
 * smk_net6addr_insert
 * @new : entry to insert
 *
 * This inserts an entry in the smack_net6addrs list
 * sorted by netmask length (longest to smallest)
 * locked by &smk_net6addr_lock in smk_write_net6addr
 *
 */
static void smk_net6addr_insert(struct smk_net6addr *new)
{
	struct smk_net6addr *m_next;
	struct smk_net6addr *m;

	if (list_empty(&smk_net6addr_list)) {
		list_add_rcu(&new->list, &smk_net6addr_list);
		return;
	}

	m = list_entry_rcu(smk_net6addr_list.next,
			   struct smk_net6addr, list);

	if (new->smk_masks > m->smk_masks) {
		list_add_rcu(&new->list, &smk_net6addr_list);
		return;
	}

	list_for_each_entry_rcu(m, &smk_net6addr_list, list) {
		if (list_is_last(&m->list, &smk_net6addr_list)) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
		m_next = list_entry_rcu(m->list.next,
					struct smk_net6addr, list);
		if (new->smk_masks > m_next->smk_masks) {
			list_add_rcu(&new->list, &m->list);
			return;
		}
	}
}


/**
 * smk_write_net6addr - write() for /smack/netlabel
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Accepts only one net6addr per write call.
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_net6addr(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct smk_net6addr *snp;
	struct in6_addr newname;
	struct in6_addr fullmask;
	struct smack_known *skp = NULL;
	char *smack;
	char *data;
	int rc = 0;
	int found = 0;
	int i;
	unsigned int scanned[8];
	unsigned int m;
	unsigned int mask = 128;

	/*
	 * Must have privilege.
	 * No partial writes.
	 * Enough data must be present.
	 * "<addr/mask, as a:b:c:d:e:f:g:h/e><space><label>"
	 * "<addr, as a:b:c:d:e:f:g:h><space><label>"
	 */
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;
	if (*ppos != 0)
		return -EINVAL;
	if (count < SMK_NETLBLADDRMIN || count > PAGE_SIZE - 1)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	smack = kzalloc(count + 1, GFP_KERNEL);
	if (smack == NULL) {
		rc = -ENOMEM;
		goto free_data_out;
	}

	i = sscanf(data, "%x:%x:%x:%x:%x:%x:%x:%x/%u %s",
			&scanned[0], &scanned[1], &scanned[2], &scanned[3],
			&scanned[4], &scanned[5], &scanned[6], &scanned[7],
			&mask, smack);
	if (i != 10) {
		i = sscanf(data, "%x:%x:%x:%x:%x:%x:%x:%x %s",
				&scanned[0], &scanned[1], &scanned[2],
				&scanned[3], &scanned[4], &scanned[5],
				&scanned[6], &scanned[7], smack);
		if (i != 9) {
			rc = -EINVAL;
			goto free_out;
		}
	}
	if (mask > 128) {
		rc = -EINVAL;
		goto free_out;
	}
	for (i = 0; i < 8; i++) {
		if (scanned[i] > 0xffff) {
			rc = -EINVAL;
			goto free_out;
		}
		newname.s6_addr16[i] = htons(scanned[i]);
	}

	/*
	 * If smack begins with '-', it is an option, don't import it
	 */
	if (smack[0] != '-') {
		skp = smk_import_entry(smack, 0);
		if (IS_ERR(skp)) {
			rc = PTR_ERR(skp);
			goto free_out;
		}
	} else {
		/*
		 * Only -DELETE is supported for IPv6
		 */
		if (strcmp(smack, SMACK_DELETE_OPTION) != 0) {
			rc = -EINVAL;
			goto free_out;
		}
	}

	for (i = 0, m = mask; i < 8; i++) {
		if (m >= 16) {
			fullmask.s6_addr16[i] = 0xffff;
			m -= 16;
		} else if (m > 0) {
			fullmask.s6_addr16[i] = (1 << m) - 1;
			m = 0;
		} else
			fullmask.s6_addr16[i] = 0;
		newname.s6_addr16[i] &= fullmask.s6_addr16[i];
	}

	/*
	 * Only allow one writer at a time. Writes should be
	 * quite rare and small in any case.
	 */
	mutex_lock(&smk_net6addr_lock);
	/*
	 * Try to find the prefix in the list
	 */
	list_for_each_entry_rcu(snp, &smk_net6addr_list, list) {
		if (mask != snp->smk_masks)
			continue;
		for (found = 1, i = 0; i < 8; i++) {
			if (newname.s6_addr16[i] !=
			    snp->smk_host.s6_addr16[i]) {
				found = 0;
				break;
			}
		}
		if (found == 1)
			break;
	}
	if (found == 0) {
		snp = kzalloc(sizeof(*snp), GFP_KERNEL);
		if (snp == NULL)
			rc = -ENOMEM;
		else {
			snp->smk_host = newname;
			snp->smk_mask = fullmask;
			snp->smk_masks = mask;
			snp->smk_label = skp;
			smk_net6addr_insert(snp);
		}
	} else {
		snp->smk_label = skp;
	}

	if (rc == 0)
		rc = count;

	mutex_unlock(&smk_net6addr_lock);

free_out:
	kfree(smack);
free_data_out:
	kfree(data);

	return rc;
}

static const struct file_operations smk_net6addr_ops = {
	.open           = smk_open_net6addr,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_net6addr,
	.release        = seq_release,
};
#endif /* CONFIG_IPV6 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * smk_read_doi - read() for /smack/doi
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @count: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_doi(struct file *filp, char __user *buf,
			    size_t count, loff_t *ppos)
{
	char temp[80];
	ssize_t rc;

	if (*ppos != 0)
		return 0;

	sprintf(temp, "%d", smk_cipso_doi_value);
	rc = simple_read_from_buffer(buf, count, ppos, temp, strlen(temp));

	return rc;
}

/**
 * smk_write_doi - write() for /smack/doi
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_doi(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	char temp[80];
	int i;

<<<<<<< HEAD
	if (!capable(CAP_MAC_ADMIN))
=======
	if (!smack_privileged(CAP_MAC_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (count >= sizeof(temp) || count == 0)
		return -EINVAL;

	if (copy_from_user(temp, buf, count) != 0)
		return -EFAULT;

	temp[count] = '\0';

	if (sscanf(temp, "%d", &i) != 1)
		return -EINVAL;

	smk_cipso_doi_value = i;

	smk_cipso_doi();

	return count;
}

static const struct file_operations smk_doi_ops = {
	.read		= smk_read_doi,
	.write		= smk_write_doi,
	.llseek		= default_llseek,
};

/**
 * smk_read_direct - read() for /smack/direct
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @count: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_direct(struct file *filp, char __user *buf,
			       size_t count, loff_t *ppos)
{
	char temp[80];
	ssize_t rc;

	if (*ppos != 0)
		return 0;

	sprintf(temp, "%d", smack_cipso_direct);
	rc = simple_read_from_buffer(buf, count, ppos, temp, strlen(temp));

	return rc;
}

/**
 * smk_write_direct - write() for /smack/direct
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_direct(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	char temp[80];
	int i;

	if (!capable(CAP_MAC_ADMIN))
=======
	struct smack_known *skp;
	char temp[80];
	int i;

	if (!smack_privileged(CAP_MAC_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (count >= sizeof(temp) || count == 0)
		return -EINVAL;

	if (copy_from_user(temp, buf, count) != 0)
		return -EFAULT;

	temp[count] = '\0';

	if (sscanf(temp, "%d", &i) != 1)
		return -EINVAL;

<<<<<<< HEAD
	smack_cipso_direct = i;
=======
	/*
	 * Don't do anything if the value hasn't actually changed.
	 * If it is changing reset the level on entries that were
	 * set up to be direct when they were created.
	 */
	if (smack_cipso_direct != i) {
		mutex_lock(&smack_known_lock);
		list_for_each_entry_rcu(skp, &smack_known_list, list)
			if (skp->smk_netlabel.attr.mls.lvl ==
			    smack_cipso_direct)
				skp->smk_netlabel.attr.mls.lvl = i;
		smack_cipso_direct = i;
		mutex_unlock(&smack_known_lock);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return count;
}

static const struct file_operations smk_direct_ops = {
	.read		= smk_read_direct,
	.write		= smk_write_direct,
	.llseek		= default_llseek,
};

/**
<<<<<<< HEAD
=======
 * smk_read_mapped - read() for /smack/mapped
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @count: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_mapped(struct file *filp, char __user *buf,
			       size_t count, loff_t *ppos)
{
	char temp[80];
	ssize_t rc;

	if (*ppos != 0)
		return 0;

	sprintf(temp, "%d", smack_cipso_mapped);
	rc = simple_read_from_buffer(buf, count, ppos, temp, strlen(temp));

	return rc;
}

/**
 * smk_write_mapped - write() for /smack/mapped
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_mapped(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct smack_known *skp;
	char temp[80];
	int i;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	if (count >= sizeof(temp) || count == 0)
		return -EINVAL;

	if (copy_from_user(temp, buf, count) != 0)
		return -EFAULT;

	temp[count] = '\0';

	if (sscanf(temp, "%d", &i) != 1)
		return -EINVAL;

	/*
	 * Don't do anything if the value hasn't actually changed.
	 * If it is changing reset the level on entries that were
	 * set up to be mapped when they were created.
	 */
	if (smack_cipso_mapped != i) {
		mutex_lock(&smack_known_lock);
		list_for_each_entry_rcu(skp, &smack_known_list, list)
			if (skp->smk_netlabel.attr.mls.lvl ==
			    smack_cipso_mapped)
				skp->smk_netlabel.attr.mls.lvl = i;
		smack_cipso_mapped = i;
		mutex_unlock(&smack_known_lock);
	}

	return count;
}

static const struct file_operations smk_mapped_ops = {
	.read		= smk_read_mapped,
	.write		= smk_write_mapped,
	.llseek		= default_llseek,
};

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * smk_read_ambient - read() for /smack/ambient
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @cn: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_ambient(struct file *filp, char __user *buf,
				size_t cn, loff_t *ppos)
{
	ssize_t rc;
	int asize;

	if (*ppos != 0)
		return 0;
	/*
	 * Being careful to avoid a problem in the case where
	 * smack_net_ambient gets changed in midstream.
	 */
	mutex_lock(&smack_ambient_lock);

<<<<<<< HEAD
	asize = strlen(smack_net_ambient) + 1;

	if (cn >= asize)
		rc = simple_read_from_buffer(buf, cn, ppos,
					     smack_net_ambient, asize);
=======
	asize = strlen(smack_net_ambient->smk_known) + 1;

	if (cn >= asize)
		rc = simple_read_from_buffer(buf, cn, ppos,
					     smack_net_ambient->smk_known,
					     asize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		rc = -EINVAL;

	mutex_unlock(&smack_ambient_lock);

	return rc;
}

/**
 * smk_write_ambient - write() for /smack/ambient
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_ambient(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	char in[SMK_LABELLEN];
	char *oldambient;
	char *smack;

	if (!capable(CAP_MAC_ADMIN))
		return -EPERM;

	if (count >= SMK_LABELLEN)
		return -EINVAL;

	if (copy_from_user(in, buf, count) != 0)
		return -EFAULT;

	smack = smk_import(in, count);
	if (smack == NULL)
		return -EINVAL;

	mutex_lock(&smack_ambient_lock);

	oldambient = smack_net_ambient;
	smack_net_ambient = smack;
=======
	struct smack_known *skp;
	char *oldambient;
	char *data;
	int rc = count;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	/* Enough data must be present */
	if (count == 0 || count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	skp = smk_import_entry(data, count);
	if (IS_ERR(skp)) {
		rc = PTR_ERR(skp);
		goto out;
	}

	mutex_lock(&smack_ambient_lock);

	oldambient = smack_net_ambient->smk_known;
	smack_net_ambient = skp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	smk_unlbl_ambient(oldambient);

	mutex_unlock(&smack_ambient_lock);

<<<<<<< HEAD
	return count;
=======
out:
	kfree(data);
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations smk_ambient_ops = {
	.read		= smk_read_ambient,
	.write		= smk_write_ambient,
	.llseek		= default_llseek,
};

<<<<<<< HEAD
/**
 * smk_read_onlycap - read() for /smack/onlycap
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @cn: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_onlycap(struct file *filp, char __user *buf,
				size_t cn, loff_t *ppos)
{
	char *smack = "";
	ssize_t rc = -EINVAL;
	int asize;

	if (*ppos != 0)
		return 0;

	if (smack_onlycap != NULL)
		smack = smack_onlycap;

	asize = strlen(smack) + 1;

	if (cn >= asize)
		rc = simple_read_from_buffer(buf, cn, ppos, smack, asize);

	return rc;
}

/**
 * smk_write_onlycap - write() for /smack/onlycap
=======
/*
 * Seq_file operations for /smack/onlycap
 */
static void *onlycap_seq_start(struct seq_file *s, loff_t *pos)
{
	return smk_seq_start(s, pos, &smack_onlycap_list);
}

static void *onlycap_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	return smk_seq_next(s, v, pos, &smack_onlycap_list);
}

static int onlycap_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_known_list_elem *sklep =
		list_entry_rcu(list, struct smack_known_list_elem, list);

	seq_puts(s, sklep->smk_label->smk_known);
	seq_putc(s, ' ');

	return 0;
}

static const struct seq_operations onlycap_seq_ops = {
	.start = onlycap_seq_start,
	.next  = onlycap_seq_next,
	.show  = onlycap_seq_show,
	.stop  = smk_seq_stop,
};

static int smk_open_onlycap(struct inode *inode, struct file *file)
{
	return seq_open(file, &onlycap_seq_ops);
}

/**
 * smk_list_swap_rcu - swap public list with a private one in RCU-safe way
 * The caller must hold appropriate mutex to prevent concurrent modifications
 * to the public list.
 * Private list is assumed to be not accessible to other threads yet.
 *
 * @public: public list
 * @private: private list
 */
static void smk_list_swap_rcu(struct list_head *public,
			      struct list_head *private)
{
	struct list_head *first, *last;

	if (list_empty(public)) {
		list_splice_init_rcu(private, public, synchronize_rcu);
	} else {
		/* Remember public list before replacing it */
		first = public->next;
		last = public->prev;

		/* Publish private list in place of public in RCU-safe way */
		private->prev->next = public;
		private->next->prev = public;
		rcu_assign_pointer(public->next, private->next);
		public->prev = private->prev;

		synchronize_rcu();

		/* When all readers are done with the old public list,
		 * attach it in place of private */
		private->next = first;
		private->prev = last;
		first->prev = private;
		last->next = private;
	}
}

/**
 * smk_parse_label_list - parse list of Smack labels, separated by spaces
 *
 * @data: the string to parse
 * @list: destination list
 *
 * Returns zero on success or error code, as appropriate
 */
static int smk_parse_label_list(char *data, struct list_head *list)
{
	char *tok;
	struct smack_known *skp;
	struct smack_known_list_elem *sklep;

	while ((tok = strsep(&data, " ")) != NULL) {
		if (!*tok)
			continue;

		skp = smk_import_entry(tok, 0);
		if (IS_ERR(skp))
			return PTR_ERR(skp);

		sklep = kzalloc(sizeof(*sklep), GFP_KERNEL);
		if (sklep == NULL)
			return -ENOMEM;

		sklep->smk_label = skp;
		list_add(&sklep->list, list);
	}

	return 0;
}

/**
 * smk_destroy_label_list - destroy a list of smack_known_list_elem
 * @list: header pointer of the list to destroy
 */
void smk_destroy_label_list(struct list_head *list)
{
	struct smack_known_list_elem *sklep;
	struct smack_known_list_elem *sklep2;

	list_for_each_entry_safe(sklep, sklep2, list, list)
		kfree(sklep);

	INIT_LIST_HEAD(list);
}

/**
 * smk_write_onlycap - write() for smackfs/onlycap
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_onlycap(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	char in[SMK_LABELLEN];
	char *sp = smk_of_task(current->cred->security);

	if (!capable(CAP_MAC_ADMIN))
		return -EPERM;

	/*
	 * This can be done using smk_access() but is done
	 * explicitly for clarity. The smk_access() implementation
	 * would use smk_access(smack_onlycap, MAY_WRITE)
	 */
	if (smack_onlycap != NULL && smack_onlycap != sp)
		return -EPERM;

	if (count >= SMK_LABELLEN)
		return -EINVAL;

	if (copy_from_user(in, buf, count) != 0)
		return -EFAULT;

	/*
	 * Should the null string be passed in unset the onlycap value.
	 * This seems like something to be careful with as usually
	 * smk_import only expects to return NULL for errors. It
	 * is usually the case that a nullstring or "\n" would be
	 * bad to pass to smk_import but in fact this is useful here.
	 */
	smack_onlycap = smk_import(in, count);

	return count;
}

static const struct file_operations smk_onlycap_ops = {
	.read		= smk_read_onlycap,
	.write		= smk_write_onlycap,
	.llseek		= default_llseek,
};
=======
	char *data;
	LIST_HEAD(list_tmp);
	int rc;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	if (count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	rc = smk_parse_label_list(data, &list_tmp);
	kfree(data);

	/*
	 * Clear the smack_onlycap on invalid label errors. This means
	 * that we can pass a null string to unset the onlycap value.
	 *
	 * Importing will also reject a label beginning with '-',
	 * so "-usecapabilities" will also work.
	 *
	 * But do so only on invalid label, not on system errors.
	 * The invalid label must be first to count as clearing attempt.
	 */
	if (!rc || (rc == -EINVAL && list_empty(&list_tmp))) {
		mutex_lock(&smack_onlycap_lock);
		smk_list_swap_rcu(&smack_onlycap_list, &list_tmp);
		mutex_unlock(&smack_onlycap_lock);
		rc = count;
	}

	smk_destroy_label_list(&list_tmp);

	return rc;
}

static const struct file_operations smk_onlycap_ops = {
	.open		= smk_open_onlycap,
	.read		= seq_read,
	.write		= smk_write_onlycap,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

#ifdef CONFIG_SECURITY_SMACK_BRINGUP
/**
 * smk_read_unconfined - read() for smackfs/unconfined
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @cn: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_unconfined(struct file *filp, char __user *buf,
					size_t cn, loff_t *ppos)
{
	char *smack = "";
	ssize_t rc = -EINVAL;
	int asize;

	if (*ppos != 0)
		return 0;

	if (smack_unconfined != NULL)
		smack = smack_unconfined->smk_known;

	asize = strlen(smack) + 1;

	if (cn >= asize)
		rc = simple_read_from_buffer(buf, cn, ppos, smack, asize);

	return rc;
}

/**
 * smk_write_unconfined - write() for smackfs/unconfined
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_unconfined(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char *data;
	struct smack_known *skp;
	int rc = count;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	if (count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	/*
	 * Clear the smack_unconfined on invalid label errors. This means
	 * that we can pass a null string to unset the unconfined value.
	 *
	 * Importing will also reject a label beginning with '-',
	 * so "-confine" will also work.
	 *
	 * But do so only on invalid label, not on system errors.
	 */
	skp = smk_import_entry(data, count);
	if (PTR_ERR(skp) == -EINVAL)
		skp = NULL;
	else if (IS_ERR(skp)) {
		rc = PTR_ERR(skp);
		goto freeout;
	}

	smack_unconfined = skp;

freeout:
	kfree(data);
	return rc;
}

static const struct file_operations smk_unconfined_ops = {
	.read		= smk_read_unconfined,
	.write		= smk_write_unconfined,
	.llseek		= default_llseek,
};
#endif /* CONFIG_SECURITY_SMACK_BRINGUP */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * smk_read_logging - read() for /smack/logging
 * @filp: file pointer, not actually used
 * @buf: where to put the result
<<<<<<< HEAD
 * @cn: maximum to send along
=======
 * @count: maximum to send along
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_logging(struct file *filp, char __user *buf,
				size_t count, loff_t *ppos)
{
	char temp[32];
	ssize_t rc;

	if (*ppos != 0)
		return 0;

	sprintf(temp, "%d\n", log_policy);
	rc = simple_read_from_buffer(buf, count, ppos, temp, strlen(temp));
	return rc;
}

/**
 * smk_write_logging - write() for /smack/logging
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_logging(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char temp[32];
	int i;

<<<<<<< HEAD
	if (!capable(CAP_MAC_ADMIN))
=======
	if (!smack_privileged(CAP_MAC_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (count >= sizeof(temp) || count == 0)
		return -EINVAL;

	if (copy_from_user(temp, buf, count) != 0)
		return -EFAULT;

	temp[count] = '\0';

	if (sscanf(temp, "%d", &i) != 1)
		return -EINVAL;
	if (i < 0 || i > 3)
		return -EINVAL;
	log_policy = i;
	return count;
}



static const struct file_operations smk_logging_ops = {
	.read		= smk_read_logging,
	.write		= smk_write_logging,
	.llseek		= default_llseek,
};

/*
 * Seq_file read operations for /smack/load-self
 */

static void *load_self_seq_start(struct seq_file *s, loff_t *pos)
{
<<<<<<< HEAD
	struct task_smack *tsp = current_security();
=======
	struct task_smack *tsp = smack_cred(current_cred());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return smk_seq_start(s, pos, &tsp->smk_rules);
}

static void *load_self_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct task_smack *tsp = current_security();
=======
	struct task_smack *tsp = smack_cred(current_cred());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return smk_seq_next(s, v, pos, &tsp->smk_rules);
}

static int load_self_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_rule *srp =
<<<<<<< HEAD
		 list_entry(list, struct smack_rule, list);

	seq_printf(s, "%s %s", (char *)srp->smk_subject,
		   (char *)srp->smk_object);

	seq_putc(s, ' ');

	if (srp->smk_access & MAY_READ)
		seq_putc(s, 'r');
	if (srp->smk_access & MAY_WRITE)
		seq_putc(s, 'w');
	if (srp->smk_access & MAY_EXEC)
		seq_putc(s, 'x');
	if (srp->smk_access & MAY_APPEND)
		seq_putc(s, 'a');
	if (srp->smk_access & MAY_TRANSMUTE)
		seq_putc(s, 't');
	if (srp->smk_access == 0)
		seq_putc(s, '-');

	seq_putc(s, '\n');
=======
		list_entry_rcu(list, struct smack_rule, list);

	smk_rule_show(s, srp, SMK_LABELLEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct seq_operations load_self_seq_ops = {
	.start = load_self_seq_start,
	.next  = load_self_seq_next,
	.show  = load_self_seq_show,
	.stop  = smk_seq_stop,
};


/**
<<<<<<< HEAD
 * smk_open_load_self - open() for /smack/load-self
=======
 * smk_open_load_self - open() for /smack/load-self2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @inode: inode structure representing file
 * @file: "load" file pointer
 *
 * For reading, use load_seq_* seq_file reading operations.
 */
static int smk_open_load_self(struct inode *inode, struct file *file)
{
	return seq_open(file, &load_self_seq_ops);
}

/**
 * smk_write_load_self - write() for /smack/load-self
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 *
 */
static ssize_t smk_write_load_self(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct task_smack *tsp = current_security();

	return smk_write_load_list(file, buf, count, ppos, &tsp->smk_rules,
					&tsp->smk_rules_lock);
=======
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_write_rules_list(file, buf, count, ppos, &tsp->smk_rules,
				    &tsp->smk_rules_lock, SMK_FIXED24_FMT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations smk_load_self_ops = {
	.open           = smk_open_load_self,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_load_self,
	.release        = seq_release,
};

/**
<<<<<<< HEAD
=======
 * smk_user_access - handle access check transaction
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 * @format: /smack/load or /smack/load2 or /smack/change-rule format.
 */
static ssize_t smk_user_access(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos, int format)
{
	struct smack_parsed_rule rule;
	char *data;
	int res;

	data = simple_transaction_get(file, buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	if (format == SMK_FIXED24_FMT) {
		if (count < SMK_LOADLEN)
			return -EINVAL;
		res = smk_parse_rule(data, &rule, 0);
	} else {
		/*
		 * simple_transaction_get() returns null-terminated data
		 */
		res = smk_parse_long_rule(data, &rule, 0, 3);
	}

	if (res >= 0)
		res = smk_access(rule.smk_subject, rule.smk_object,
				 rule.smk_access1, NULL);
	else if (res != -ENOENT)
		return res;

	/*
	 * smk_access() can return a value > 0 in the "bringup" case.
	 */
	data[0] = res >= 0 ? '1' : '0';
	data[1] = '\0';

	simple_transaction_set(file, 2);

	if (format == SMK_FIXED24_FMT)
		return SMK_LOADLEN;
	return count;
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * smk_write_access - handle access check transaction
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 */
static ssize_t smk_write_access(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct smack_rule rule;
	char *data;
	int res;

	data = simple_transaction_get(file, buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	if (count < SMK_LOADLEN || smk_parse_rule(data, &rule, 0))
		return -EINVAL;

	res = smk_access(rule.smk_subject, rule.smk_object, rule.smk_access,
			  NULL);
	data[0] = res == 0 ? '1' : '0';
	data[1] = '\0';

	simple_transaction_set(file, 2);
	return SMK_LOADLEN;
=======
	return smk_user_access(file, buf, count, ppos, SMK_FIXED24_FMT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations smk_access_ops = {
	.write		= smk_write_access,
	.read		= simple_transaction_read,
	.release	= simple_transaction_release,
	.llseek		= generic_file_llseek,
};

<<<<<<< HEAD
/**
 * smk_fill_super - fill the /smackfs superblock
 * @sb: the empty superblock
 * @data: unused
 * @silent: unused
 *
 * Fill in the well known entries for /smack
 *
 * Returns 0 on success, an error code on failure
 */
static int smk_fill_super(struct super_block *sb, void *data, int silent)
{
	int rc;
	struct inode *root_inode;

	static struct tree_descr smack_files[] = {
=======

/*
 * Seq_file read operations for /smack/load2
 */

static int load2_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_rule *srp;
	struct smack_known *skp =
		list_entry_rcu(list, struct smack_known, list);

	list_for_each_entry_rcu(srp, &skp->smk_rules, list)
		smk_rule_show(s, srp, SMK_LONGLABEL);

	return 0;
}

static const struct seq_operations load2_seq_ops = {
	.start = load2_seq_start,
	.next  = load2_seq_next,
	.show  = load2_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_load2 - open() for /smack/load2
 * @inode: inode structure representing file
 * @file: "load2" file pointer
 *
 * For reading, use load2_seq_* seq_file reading operations.
 */
static int smk_open_load2(struct inode *inode, struct file *file)
{
	return seq_open(file, &load2_seq_ops);
}

/**
 * smk_write_load2 - write() for /smack/load2
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 *
 */
static ssize_t smk_write_load2(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	/*
	 * Must have privilege.
	 */
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	return smk_write_rules_list(file, buf, count, ppos, NULL, NULL,
				    SMK_LONG_FMT);
}

static const struct file_operations smk_load2_ops = {
	.open           = smk_open_load2,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_load2,
	.release        = seq_release,
};

/*
 * Seq_file read operations for /smack/load-self2
 */

static void *load_self2_seq_start(struct seq_file *s, loff_t *pos)
{
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_seq_start(s, pos, &tsp->smk_rules);
}

static void *load_self2_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_seq_next(s, v, pos, &tsp->smk_rules);
}

static int load_self2_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_rule *srp =
		list_entry_rcu(list, struct smack_rule, list);

	smk_rule_show(s, srp, SMK_LONGLABEL);

	return 0;
}

static const struct seq_operations load_self2_seq_ops = {
	.start = load_self2_seq_start,
	.next  = load_self2_seq_next,
	.show  = load_self2_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_load_self2 - open() for /smack/load-self2
 * @inode: inode structure representing file
 * @file: "load" file pointer
 *
 * For reading, use load_seq_* seq_file reading operations.
 */
static int smk_open_load_self2(struct inode *inode, struct file *file)
{
	return seq_open(file, &load_self2_seq_ops);
}

/**
 * smk_write_load_self2 - write() for /smack/load-self2
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 *
 */
static ssize_t smk_write_load_self2(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_write_rules_list(file, buf, count, ppos, &tsp->smk_rules,
				    &tsp->smk_rules_lock, SMK_LONG_FMT);
}

static const struct file_operations smk_load_self2_ops = {
	.open           = smk_open_load_self2,
	.read		= seq_read,
	.llseek         = seq_lseek,
	.write		= smk_write_load_self2,
	.release        = seq_release,
};

/**
 * smk_write_access2 - handle access check transaction
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 */
static ssize_t smk_write_access2(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	return smk_user_access(file, buf, count, ppos, SMK_LONG_FMT);
}

static const struct file_operations smk_access2_ops = {
	.write		= smk_write_access2,
	.read		= simple_transaction_read,
	.release	= simple_transaction_release,
	.llseek		= generic_file_llseek,
};

/**
 * smk_write_revoke_subj - write() for /smack/revoke-subject
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 */
static ssize_t smk_write_revoke_subj(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char *data;
	const char *cp;
	struct smack_known *skp;
	struct smack_rule *sp;
	struct list_head *rule_list;
	struct mutex *rule_lock;
	int rc = count;

	if (*ppos != 0)
		return -EINVAL;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	if (count == 0 || count > SMK_LONGLABEL)
		return -EINVAL;

	data = memdup_user(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	cp = smk_parse_smack(data, count);
	if (IS_ERR(cp)) {
		rc = PTR_ERR(cp);
		goto out_data;
	}

	skp = smk_find_entry(cp);
	if (skp == NULL)
		goto out_cp;

	rule_list = &skp->smk_rules;
	rule_lock = &skp->smk_rules_lock;

	mutex_lock(rule_lock);

	list_for_each_entry_rcu(sp, rule_list, list)
		sp->smk_access = 0;

	mutex_unlock(rule_lock);

out_cp:
	kfree(cp);
out_data:
	kfree(data);

	return rc;
}

static const struct file_operations smk_revoke_subj_ops = {
	.write		= smk_write_revoke_subj,
	.read		= simple_transaction_read,
	.release	= simple_transaction_release,
	.llseek		= generic_file_llseek,
};

/**
 * smk_init_sysfs - initialize /sys/fs/smackfs
 *
 */
static int smk_init_sysfs(void)
{
	return sysfs_create_mount_point(fs_kobj, "smackfs");
}

/**
 * smk_write_change_rule - write() for /smack/change-rule
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 */
static ssize_t smk_write_change_rule(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	/*
	 * Must have privilege.
	 */
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	return smk_write_rules_list(file, buf, count, ppos, NULL, NULL,
				    SMK_CHANGE_FMT);
}

static const struct file_operations smk_change_rule_ops = {
	.write		= smk_write_change_rule,
	.read		= simple_transaction_read,
	.release	= simple_transaction_release,
	.llseek		= generic_file_llseek,
};

/**
 * smk_read_syslog - read() for smackfs/syslog
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @cn: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_syslog(struct file *filp, char __user *buf,
				size_t cn, loff_t *ppos)
{
	struct smack_known *skp;
	ssize_t rc = -EINVAL;
	int asize;

	if (*ppos != 0)
		return 0;

	if (smack_syslog_label == NULL)
		skp = &smack_known_star;
	else
		skp = smack_syslog_label;

	asize = strlen(skp->smk_known) + 1;

	if (cn >= asize)
		rc = simple_read_from_buffer(buf, cn, ppos, skp->smk_known,
						asize);

	return rc;
}

/**
 * smk_write_syslog - write() for smackfs/syslog
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start
 *
 * Returns number of bytes written or error code, as appropriate
 */
static ssize_t smk_write_syslog(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char *data;
	struct smack_known *skp;
	int rc = count;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	/* Enough data must be present */
	if (count == 0 || count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	skp = smk_import_entry(data, count);
	if (IS_ERR(skp))
		rc = PTR_ERR(skp);
	else
		smack_syslog_label = skp;

	kfree(data);
	return rc;
}

static const struct file_operations smk_syslog_ops = {
	.read		= smk_read_syslog,
	.write		= smk_write_syslog,
	.llseek		= default_llseek,
};

/*
 * Seq_file read operations for /smack/relabel-self
 */

static void *relabel_self_seq_start(struct seq_file *s, loff_t *pos)
{
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_seq_start(s, pos, &tsp->smk_relabel);
}

static void *relabel_self_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct task_smack *tsp = smack_cred(current_cred());

	return smk_seq_next(s, v, pos, &tsp->smk_relabel);
}

static int relabel_self_seq_show(struct seq_file *s, void *v)
{
	struct list_head *list = v;
	struct smack_known_list_elem *sklep =
		list_entry(list, struct smack_known_list_elem, list);

	seq_puts(s, sklep->smk_label->smk_known);
	seq_putc(s, ' ');

	return 0;
}

static const struct seq_operations relabel_self_seq_ops = {
	.start = relabel_self_seq_start,
	.next  = relabel_self_seq_next,
	.show  = relabel_self_seq_show,
	.stop  = smk_seq_stop,
};

/**
 * smk_open_relabel_self - open() for /smack/relabel-self
 * @inode: inode structure representing file
 * @file: "relabel-self" file pointer
 *
 * Connect our relabel_self_seq_* operations with /smack/relabel-self
 * file_operations
 */
static int smk_open_relabel_self(struct inode *inode, struct file *file)
{
	return seq_open(file, &relabel_self_seq_ops);
}

/**
 * smk_write_relabel_self - write() for /smack/relabel-self
 * @file: file pointer, not actually used
 * @buf: where to get the data from
 * @count: bytes sent
 * @ppos: where to start - must be 0
 *
 */
static ssize_t smk_write_relabel_self(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char *data;
	int rc;
	LIST_HEAD(list_tmp);

	/*
	 * Must have privilege.
	 */
	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	/*
	 * No partial write.
	 * Enough data must be present.
	 */
	if (*ppos != 0)
		return -EINVAL;
	if (count == 0 || count > PAGE_SIZE)
		return -EINVAL;

	data = memdup_user_nul(buf, count);
	if (IS_ERR(data))
		return PTR_ERR(data);

	rc = smk_parse_label_list(data, &list_tmp);
	kfree(data);

	if (!rc || (rc == -EINVAL && list_empty(&list_tmp))) {
		struct cred *new;
		struct task_smack *tsp;

		new = prepare_creds();
		if (!new) {
			rc = -ENOMEM;
			goto out;
		}
		tsp = smack_cred(new);
		smk_destroy_label_list(&tsp->smk_relabel);
		list_splice(&list_tmp, &tsp->smk_relabel);
		commit_creds(new);
		return count;
	}
out:
	smk_destroy_label_list(&list_tmp);
	return rc;
}

static const struct file_operations smk_relabel_self_ops = {
	.open		= smk_open_relabel_self,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= smk_write_relabel_self,
	.release	= seq_release,
};

/**
 * smk_read_ptrace - read() for /smack/ptrace
 * @filp: file pointer, not actually used
 * @buf: where to put the result
 * @count: maximum to send along
 * @ppos: where to start
 *
 * Returns number of bytes read or error code, as appropriate
 */
static ssize_t smk_read_ptrace(struct file *filp, char __user *buf,
			       size_t count, loff_t *ppos)
{
	char temp[32];
	ssize_t rc;

	if (*ppos != 0)
		return 0;

	sprintf(temp, "%d\n", smack_ptrace_rule);
	rc = simple_read_from_buffer(buf, count, ppos, temp, strlen(temp));
	return rc;
}

/**
 * smk_write_ptrace - write() for /smack/ptrace
 * @file: file pointer
 * @buf: data from user space
 * @count: bytes sent
 * @ppos: where to start - must be 0
 */
static ssize_t smk_write_ptrace(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	char temp[32];
	int i;

	if (!smack_privileged(CAP_MAC_ADMIN))
		return -EPERM;

	if (*ppos != 0 || count >= sizeof(temp) || count == 0)
		return -EINVAL;

	if (copy_from_user(temp, buf, count) != 0)
		return -EFAULT;

	temp[count] = '\0';

	if (sscanf(temp, "%d", &i) != 1)
		return -EINVAL;
	if (i < SMACK_PTRACE_DEFAULT || i > SMACK_PTRACE_MAX)
		return -EINVAL;
	smack_ptrace_rule = i;

	return count;
}

static const struct file_operations smk_ptrace_ops = {
	.write		= smk_write_ptrace,
	.read		= smk_read_ptrace,
	.llseek		= default_llseek,
};

/**
 * smk_fill_super - fill the smackfs superblock
 * @sb: the empty superblock
 * @fc: unused
 *
 * Fill in the well known entries for the smack filesystem
 *
 * Returns 0 on success, an error code on failure
 */
static int smk_fill_super(struct super_block *sb, struct fs_context *fc)
{
	int rc;

	static const struct tree_descr smack_files[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		[SMK_LOAD] = {
			"load", &smk_load_ops, S_IRUGO|S_IWUSR},
		[SMK_CIPSO] = {
			"cipso", &smk_cipso_ops, S_IRUGO|S_IWUSR},
		[SMK_DOI] = {
			"doi", &smk_doi_ops, S_IRUGO|S_IWUSR},
		[SMK_DIRECT] = {
			"direct", &smk_direct_ops, S_IRUGO|S_IWUSR},
		[SMK_AMBIENT] = {
			"ambient", &smk_ambient_ops, S_IRUGO|S_IWUSR},
<<<<<<< HEAD
		[SMK_NETLBLADDR] = {
			"netlabel", &smk_netlbladdr_ops, S_IRUGO|S_IWUSR},
=======
		[SMK_NET4ADDR] = {
			"netlabel", &smk_net4addr_ops, S_IRUGO|S_IWUSR},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		[SMK_ONLYCAP] = {
			"onlycap", &smk_onlycap_ops, S_IRUGO|S_IWUSR},
		[SMK_LOGGING] = {
			"logging", &smk_logging_ops, S_IRUGO|S_IWUSR},
		[SMK_LOAD_SELF] = {
			"load-self", &smk_load_self_ops, S_IRUGO|S_IWUGO},
		[SMK_ACCESSES] = {
			"access", &smk_access_ops, S_IRUGO|S_IWUGO},
<<<<<<< HEAD
=======
		[SMK_MAPPED] = {
			"mapped", &smk_mapped_ops, S_IRUGO|S_IWUSR},
		[SMK_LOAD2] = {
			"load2", &smk_load2_ops, S_IRUGO|S_IWUSR},
		[SMK_LOAD_SELF2] = {
			"load-self2", &smk_load_self2_ops, S_IRUGO|S_IWUGO},
		[SMK_ACCESS2] = {
			"access2", &smk_access2_ops, S_IRUGO|S_IWUGO},
		[SMK_CIPSO2] = {
			"cipso2", &smk_cipso2_ops, S_IRUGO|S_IWUSR},
		[SMK_REVOKE_SUBJ] = {
			"revoke-subject", &smk_revoke_subj_ops,
			S_IRUGO|S_IWUSR},
		[SMK_CHANGE_RULE] = {
			"change-rule", &smk_change_rule_ops, S_IRUGO|S_IWUSR},
		[SMK_SYSLOG] = {
			"syslog", &smk_syslog_ops, S_IRUGO|S_IWUSR},
		[SMK_PTRACE] = {
			"ptrace", &smk_ptrace_ops, S_IRUGO|S_IWUSR},
#ifdef CONFIG_SECURITY_SMACK_BRINGUP
		[SMK_UNCONFINED] = {
			"unconfined", &smk_unconfined_ops, S_IRUGO|S_IWUSR},
#endif
#if IS_ENABLED(CONFIG_IPV6)
		[SMK_NET6ADDR] = {
			"ipv6host", &smk_net6addr_ops, S_IRUGO|S_IWUSR},
#endif /* CONFIG_IPV6 */
		[SMK_RELABEL_SELF] = {
			"relabel-self", &smk_relabel_self_ops,
				S_IRUGO|S_IWUGO},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* last one */
			{""}
	};

	rc = simple_fill_super(sb, SMACK_MAGIC, smack_files);
	if (rc != 0) {
		printk(KERN_ERR "%s failed %d while creating inodes\n",
			__func__, rc);
		return rc;
	}

<<<<<<< HEAD
	root_inode = sb->s_root->d_inode;
	root_inode->i_security = new_inode_smack(smack_known_floor.smk_known);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
 * smk_mount - get the smackfs superblock
 * @fs_type: passed along without comment
 * @flags: passed along without comment
 * @dev_name: passed along without comment
 * @data: passed along without comment
=======
 * smk_get_tree - get the smackfs superblock
 * @fc: The mount context, including any options
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Just passes everything along.
 *
 * Returns what the lower level code does.
 */
<<<<<<< HEAD
static struct dentry *smk_mount(struct file_system_type *fs_type,
		      int flags, const char *dev_name, void *data)
{
	return mount_single(fs_type, flags, data, smk_fill_super);
=======
static int smk_get_tree(struct fs_context *fc)
{
	return get_tree_single(fc, smk_fill_super);
}

static const struct fs_context_operations smk_context_ops = {
	.get_tree	= smk_get_tree,
};

/**
 * smk_init_fs_context - Initialise a filesystem context for smackfs
 * @fc: The blank mount context
 */
static int smk_init_fs_context(struct fs_context *fc)
{
	fc->ops = &smk_context_ops;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct file_system_type smk_fs_type = {
	.name		= "smackfs",
<<<<<<< HEAD
	.mount		= smk_mount,
=======
	.init_fs_context = smk_init_fs_context,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.kill_sb	= kill_litter_super,
};

static struct vfsmount *smackfs_mount;

/**
 * init_smk_fs - get the smackfs superblock
 *
 * register the smackfs
 *
 * Do not register smackfs if Smack wasn't enabled
 * on boot. We can not put this method normally under the
 * smack_init() code path since the security subsystem get
 * initialized before the vfs caches.
 *
 * Returns true if we were not chosen on boot or if
 * we were chosen and filesystem registration succeeded.
 */
static int __init init_smk_fs(void)
{
	int err;
<<<<<<< HEAD

	if (!security_module_enable(&smack_ops))
		return 0;

=======
	int rc;

	if (smack_enabled == 0)
		return 0;

	err = smk_init_sysfs();
	if (err)
		printk(KERN_ERR "smackfs: sysfs mountpoint problem.\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = register_filesystem(&smk_fs_type);
	if (!err) {
		smackfs_mount = kern_mount(&smk_fs_type);
		if (IS_ERR(smackfs_mount)) {
			printk(KERN_ERR "smackfs:  could not mount!\n");
			err = PTR_ERR(smackfs_mount);
			smackfs_mount = NULL;
		}
	}

	smk_cipso_doi();
	smk_unlbl_ambient(NULL);

<<<<<<< HEAD
=======
	rc = smack_populate_secattr(&smack_known_floor);
	if (err == 0 && rc < 0)
		err = rc;
	rc = smack_populate_secattr(&smack_known_hat);
	if (err == 0 && rc < 0)
		err = rc;
	rc = smack_populate_secattr(&smack_known_huh);
	if (err == 0 && rc < 0)
		err = rc;
	rc = smack_populate_secattr(&smack_known_star);
	if (err == 0 && rc < 0)
		err = rc;
	rc = smack_populate_secattr(&smack_known_web);
	if (err == 0 && rc < 0)
		err = rc;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

__initcall(init_smk_fs);
