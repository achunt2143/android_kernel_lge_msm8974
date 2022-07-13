<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2005,2006,2007,2008 IBM Corporation
 *
 * Authors:
 * Kylene Hall <kjhall@us.ibm.com>
 * Reiner Sailer <sailer@us.ibm.com>
 * Mimi Zohar <zohar@us.ibm.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * File: ima_fs.c
 *	implemenents security file system for reporting
 *	current measurement list and IMA statistics
 */
<<<<<<< HEAD
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/module.h>
=======

#include <linux/fcntl.h>
#include <linux/kernel_read_file.h>
#include <linux/slab.h>
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/seq_file.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
#include <linux/parser.h>
<<<<<<< HEAD

#include "ima.h"

static int valid_policy = 1;
#define TMPBUFLEN 12
static ssize_t ima_show_htable_value(char __user *buf, size_t count,
				     loff_t *ppos, atomic_long_t *val)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t len;

	len = scnprintf(tmpbuf, TMPBUFLEN, "%li\n", atomic_long_read(val));
=======
#include <linux/vmalloc.h>

#include "ima.h"

static DEFINE_MUTEX(ima_write_mutex);

bool ima_canonical_fmt;
static int __init default_canonical_fmt_setup(char *str)
{
#ifdef __BIG_ENDIAN
	ima_canonical_fmt = true;
#endif
	return 1;
}
__setup("ima_canonical_fmt", default_canonical_fmt_setup);

static int valid_policy = 1;

static ssize_t ima_show_htable_value(char __user *buf, size_t count,
				     loff_t *ppos, atomic_long_t *val)
{
	char tmpbuf[32];	/* greater than largest 'long' string value */
	ssize_t len;

	len = scnprintf(tmpbuf, sizeof(tmpbuf), "%li\n", atomic_long_read(val));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, len);
}

static ssize_t ima_show_htable_violations(struct file *filp,
					  char __user *buf,
					  size_t count, loff_t *ppos)
{
	return ima_show_htable_value(buf, count, ppos, &ima_htable.violations);
}

static const struct file_operations ima_htable_violations_ops = {
	.read = ima_show_htable_violations,
	.llseek = generic_file_llseek,
};

static ssize_t ima_show_measurements_count(struct file *filp,
					   char __user *buf,
					   size_t count, loff_t *ppos)
{
	return ima_show_htable_value(buf, count, ppos, &ima_htable.len);

}

static const struct file_operations ima_measurements_count_ops = {
	.read = ima_show_measurements_count,
	.llseek = generic_file_llseek,
};

/* returns pointer to hlist_node */
static void *ima_measurements_start(struct seq_file *m, loff_t *pos)
{
	loff_t l = *pos;
	struct ima_queue_entry *qe;

	/* we need a lock since pos could point beyond last element */
	rcu_read_lock();
	list_for_each_entry_rcu(qe, &ima_measurements, later) {
		if (!l--) {
			rcu_read_unlock();
			return qe;
		}
	}
	rcu_read_unlock();
	return NULL;
}

static void *ima_measurements_next(struct seq_file *m, void *v, loff_t *pos)
{
	struct ima_queue_entry *qe = v;

	/* lock protects when reading beyond last element
	 * against concurrent list-extension
	 */
	rcu_read_lock();
<<<<<<< HEAD
	qe = list_entry_rcu(qe->later.next,
			    struct ima_queue_entry, later);
=======
	qe = list_entry_rcu(qe->later.next, struct ima_queue_entry, later);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	(*pos)++;

	return (&qe->later == &ima_measurements) ? NULL : qe;
}

static void ima_measurements_stop(struct seq_file *m, void *v)
{
}

<<<<<<< HEAD
static void ima_putc(struct seq_file *m, void *data, int datalen)
=======
void ima_putc(struct seq_file *m, void *data, int datalen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	while (datalen--)
		seq_putc(m, *(char *)data++);
}

/* print format:
 *       32bit-le=pcr#
 *       char[20]=template digest
 *       32bit-le=template name size
 *       char[n]=template name
<<<<<<< HEAD
 *       eventdata[n]=template specific data
 */
static int ima_measurements_show(struct seq_file *m, void *v)
=======
 *       [eventdata length]
 *       eventdata[n]=template specific data
 */
int ima_measurements_show(struct seq_file *m, void *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* the list never shrinks, so we don't need a lock here */
	struct ima_queue_entry *qe = v;
	struct ima_template_entry *e;
<<<<<<< HEAD
	int namelen;
	u32 pcr = CONFIG_IMA_MEASURE_PCR_IDX;
=======
	char *template_name;
	u32 pcr, namelen, template_data_len; /* temporary fields */
	bool is_ima_template = false;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* get entry */
	e = qe->entry;
	if (e == NULL)
		return -1;

<<<<<<< HEAD
	/*
	 * 1st: PCRIndex
	 * PCR used is always the same (config option) in
	 * little-endian format
	 */
	ima_putc(m, &pcr, sizeof pcr);

	/* 2nd: template digest */
	ima_putc(m, e->digest, IMA_DIGEST_SIZE);

	/* 3rd: template name size */
	namelen = strlen(e->template_name);
	ima_putc(m, &namelen, sizeof namelen);

	/* 4th:  template name */
	ima_putc(m, (void *)e->template_name, namelen);

	/* 5th:  template specific data */
	ima_template_show(m, (struct ima_template_data *)&e->template,
			  IMA_SHOW_BINARY);
=======
	template_name = (e->template_desc->name[0] != '\0') ?
	    e->template_desc->name : e->template_desc->fmt;

	/*
	 * 1st: PCRIndex
	 * PCR used defaults to the same (config option) in
	 * little-endian format, unless set in policy
	 */
	pcr = !ima_canonical_fmt ? e->pcr : (__force u32)cpu_to_le32(e->pcr);
	ima_putc(m, &pcr, sizeof(e->pcr));

	/* 2nd: template digest */
	ima_putc(m, e->digests[ima_sha1_idx].digest, TPM_DIGEST_SIZE);

	/* 3rd: template name size */
	namelen = !ima_canonical_fmt ? strlen(template_name) :
		(__force u32)cpu_to_le32(strlen(template_name));
	ima_putc(m, &namelen, sizeof(namelen));

	/* 4th:  template name */
	ima_putc(m, template_name, strlen(template_name));

	/* 5th:  template length (except for 'ima' template) */
	if (strcmp(template_name, IMA_TEMPLATE_IMA_NAME) == 0)
		is_ima_template = true;

	if (!is_ima_template) {
		template_data_len = !ima_canonical_fmt ? e->template_data_len :
			(__force u32)cpu_to_le32(e->template_data_len);
		ima_putc(m, &template_data_len, sizeof(e->template_data_len));
	}

	/* 6th:  template specific data */
	for (i = 0; i < e->template_desc->num_fields; i++) {
		enum ima_show_type show = IMA_SHOW_BINARY;
		const struct ima_template_field *field =
			e->template_desc->fields[i];

		if (is_ima_template && strcmp(field->field_id, "d") == 0)
			show = IMA_SHOW_BINARY_NO_FIELD_LEN;
		if (is_ima_template && strcmp(field->field_id, "n") == 0)
			show = IMA_SHOW_BINARY_OLD_STRING_FMT;
		field->field_show(m, show, &e->template_data[i]);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct seq_operations ima_measurments_seqops = {
	.start = ima_measurements_start,
	.next = ima_measurements_next,
	.stop = ima_measurements_stop,
	.show = ima_measurements_show
};

static int ima_measurements_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &ima_measurments_seqops);
}

static const struct file_operations ima_measurements_ops = {
	.open = ima_measurements_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

<<<<<<< HEAD
static void ima_print_digest(struct seq_file *m, u8 *digest)
{
	int i;

	for (i = 0; i < IMA_DIGEST_SIZE; i++)
		seq_printf(m, "%02x", *(digest + i));
}

void ima_template_show(struct seq_file *m, void *e, enum ima_show_type show)
{
	struct ima_template_data *entry = e;
	int namelen;

	switch (show) {
	case IMA_SHOW_ASCII:
		ima_print_digest(m, entry->digest);
		seq_printf(m, " %s\n", entry->file_name);
		break;
	case IMA_SHOW_BINARY:
		ima_putc(m, entry->digest, IMA_DIGEST_SIZE);

		namelen = strlen(entry->file_name);
		ima_putc(m, &namelen, sizeof namelen);
		ima_putc(m, entry->file_name, namelen);
	default:
		break;
	}
}

=======
void ima_print_digest(struct seq_file *m, u8 *digest, u32 size)
{
	u32 i;

	for (i = 0; i < size; i++)
		seq_printf(m, "%02x", *(digest + i));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* print in ascii */
static int ima_ascii_measurements_show(struct seq_file *m, void *v)
{
	/* the list never shrinks, so we don't need a lock here */
	struct ima_queue_entry *qe = v;
	struct ima_template_entry *e;
<<<<<<< HEAD
=======
	char *template_name;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* get entry */
	e = qe->entry;
	if (e == NULL)
		return -1;

<<<<<<< HEAD
	/* 1st: PCR used (config option) */
	seq_printf(m, "%2d ", CONFIG_IMA_MEASURE_PCR_IDX);

	/* 2nd: SHA1 template hash */
	ima_print_digest(m, e->digest);

	/* 3th:  template name */
	seq_printf(m, " %s ", e->template_name);

	/* 4th:  template specific data */
	ima_template_show(m, (struct ima_template_data *)&e->template,
			  IMA_SHOW_ASCII);
=======
	template_name = (e->template_desc->name[0] != '\0') ?
	    e->template_desc->name : e->template_desc->fmt;

	/* 1st: PCR used (config option) */
	seq_printf(m, "%2d ", e->pcr);

	/* 2nd: SHA1 template hash */
	ima_print_digest(m, e->digests[ima_sha1_idx].digest, TPM_DIGEST_SIZE);

	/* 3th:  template name */
	seq_printf(m, " %s", template_name);

	/* 4th:  template specific data */
	for (i = 0; i < e->template_desc->num_fields; i++) {
		seq_puts(m, " ");
		if (e->template_data[i].len == 0)
			continue;

		e->template_desc->fields[i]->field_show(m, IMA_SHOW_ASCII,
							&e->template_data[i]);
	}
	seq_puts(m, "\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct seq_operations ima_ascii_measurements_seqops = {
	.start = ima_measurements_start,
	.next = ima_measurements_next,
	.stop = ima_measurements_stop,
	.show = ima_ascii_measurements_show
};

static int ima_ascii_measurements_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &ima_ascii_measurements_seqops);
}

static const struct file_operations ima_ascii_measurements_ops = {
	.open = ima_ascii_measurements_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

<<<<<<< HEAD
static ssize_t ima_write_policy(struct file *file, const char __user *buf,
				size_t datalen, loff_t *ppos)
{
	char *data = NULL;
=======
static ssize_t ima_read_policy(char *path)
{
	void *data = NULL;
	char *datap;
	size_t size;
	int rc, pathlen = strlen(path);

	char *p;

	/* remove \n */
	datap = path;
	strsep(&datap, "\n");

	rc = kernel_read_file_from_path(path, 0, &data, INT_MAX, NULL,
					READING_POLICY);
	if (rc < 0) {
		pr_err("Unable to open file: %s (%d)", path, rc);
		return rc;
	}
	size = rc;
	rc = 0;

	datap = data;
	while (size > 0 && (p = strsep(&datap, "\n"))) {
		pr_debug("rule: %s\n", p);
		rc = ima_parse_add_rule(p);
		if (rc < 0)
			break;
		size -= rc;
	}

	vfree(data);
	if (rc < 0)
		return rc;
	else if (size)
		return -EINVAL;
	else
		return pathlen;
}

static ssize_t ima_write_policy(struct file *file, const char __user *buf,
				size_t datalen, loff_t *ppos)
{
	char *data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ssize_t result;

	if (datalen >= PAGE_SIZE)
		datalen = PAGE_SIZE - 1;

	/* No partial writes. */
	result = -EINVAL;
	if (*ppos != 0)
		goto out;

<<<<<<< HEAD
	result = -ENOMEM;
	data = kmalloc(datalen + 1, GFP_KERNEL);
	if (!data)
		goto out;

	*(data + datalen) = '\0';

	result = -EFAULT;
	if (copy_from_user(data, buf, datalen))
		goto out;

	result = ima_parse_add_rule(data);
out:
	if (result < 0)
		valid_policy = 0;
	kfree(data);
=======
	data = memdup_user_nul(buf, datalen);
	if (IS_ERR(data)) {
		result = PTR_ERR(data);
		goto out;
	}

	result = mutex_lock_interruptible(&ima_write_mutex);
	if (result < 0)
		goto out_free;

	if (data[0] == '/') {
		result = ima_read_policy(data);
	} else if (ima_appraise & IMA_APPRAISE_POLICY) {
		pr_err("signed policy file (specified as an absolute pathname) required\n");
		integrity_audit_msg(AUDIT_INTEGRITY_STATUS, NULL, NULL,
				    "policy_update", "signed policy required",
				    1, 0);
		result = -EACCES;
	} else {
		result = ima_parse_add_rule(data);
	}
	mutex_unlock(&ima_write_mutex);
out_free:
	kfree(data);
out:
	if (result < 0)
		valid_policy = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}

static struct dentry *ima_dir;
<<<<<<< HEAD
=======
static struct dentry *ima_symlink;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct dentry *binary_runtime_measurements;
static struct dentry *ascii_runtime_measurements;
static struct dentry *runtime_measurements_count;
static struct dentry *violations;
static struct dentry *ima_policy;

<<<<<<< HEAD
static atomic_t policy_opencount = ATOMIC_INIT(1);
/*
 * ima_open_policy: sequentialize access to the policy file
 */
static int ima_open_policy(struct inode * inode, struct file * filp)
{
	/* No point in being allowed to open it if you aren't going to write */
	if (!(filp->f_flags & O_WRONLY))
		return -EACCES;
	if (atomic_dec_and_test(&policy_opencount))
		return 0;
	return -EBUSY;
=======
enum ima_fs_flags {
	IMA_FS_BUSY,
};

static unsigned long ima_fs_flags;

#ifdef	CONFIG_IMA_READ_POLICY
static const struct seq_operations ima_policy_seqops = {
		.start = ima_policy_start,
		.next = ima_policy_next,
		.stop = ima_policy_stop,
		.show = ima_policy_show,
};
#endif

/*
 * ima_open_policy: sequentialize access to the policy file
 */
static int ima_open_policy(struct inode *inode, struct file *filp)
{
	if (!(filp->f_flags & O_WRONLY)) {
#ifndef	CONFIG_IMA_READ_POLICY
		return -EACCES;
#else
		if ((filp->f_flags & O_ACCMODE) != O_RDONLY)
			return -EACCES;
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		return seq_open(filp, &ima_policy_seqops);
#endif
	}
	if (test_and_set_bit(IMA_FS_BUSY, &ima_fs_flags))
		return -EBUSY;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * ima_release_policy - start using the new measure policy rules.
 *
 * Initially, ima_measure points to the default policy rules, now
 * point to the new policy rules, and remove the securityfs policy file,
 * assuming a valid policy.
 */
static int ima_release_policy(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	if (!valid_policy) {
		ima_delete_rules();
		valid_policy = 1;
		atomic_set(&policy_opencount, 1);
		return 0;
	}
	ima_update_policy();
	securityfs_remove(ima_policy);
	ima_policy = NULL;
=======
	const char *cause = valid_policy ? "completed" : "failed";

	if ((file->f_flags & O_ACCMODE) == O_RDONLY)
		return seq_release(inode, file);

	if (valid_policy && ima_check_policy() < 0) {
		cause = "failed";
		valid_policy = 0;
	}

	pr_info("policy update %s\n", cause);
	integrity_audit_msg(AUDIT_INTEGRITY_STATUS, NULL, NULL,
			    "policy_update", cause, !valid_policy, 0);

	if (!valid_policy) {
		ima_delete_rules();
		valid_policy = 1;
		clear_bit(IMA_FS_BUSY, &ima_fs_flags);
		return 0;
	}

	ima_update_policy();
#if !defined(CONFIG_IMA_WRITE_POLICY) && !defined(CONFIG_IMA_READ_POLICY)
	securityfs_remove(ima_policy);
	ima_policy = NULL;
#elif defined(CONFIG_IMA_WRITE_POLICY)
	clear_bit(IMA_FS_BUSY, &ima_fs_flags);
#elif defined(CONFIG_IMA_READ_POLICY)
	inode->i_mode &= ~S_IWUSR;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct file_operations ima_measure_policy_ops = {
	.open = ima_open_policy,
	.write = ima_write_policy,
<<<<<<< HEAD
=======
	.read = seq_read,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.release = ima_release_policy,
	.llseek = generic_file_llseek,
};

int __init ima_fs_init(void)
{
<<<<<<< HEAD
	ima_dir = securityfs_create_dir("ima", NULL);
	if (IS_ERR(ima_dir))
		return -1;
=======
	int ret;

	ima_dir = securityfs_create_dir("ima", integrity_dir);
	if (IS_ERR(ima_dir))
		return PTR_ERR(ima_dir);

	ima_symlink = securityfs_create_symlink("ima", NULL, "integrity/ima",
						NULL);
	if (IS_ERR(ima_symlink)) {
		ret = PTR_ERR(ima_symlink);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	binary_runtime_measurements =
	    securityfs_create_file("binary_runtime_measurements",
				   S_IRUSR | S_IRGRP, ima_dir, NULL,
				   &ima_measurements_ops);
<<<<<<< HEAD
	if (IS_ERR(binary_runtime_measurements))
		goto out;
=======
	if (IS_ERR(binary_runtime_measurements)) {
		ret = PTR_ERR(binary_runtime_measurements);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ascii_runtime_measurements =
	    securityfs_create_file("ascii_runtime_measurements",
				   S_IRUSR | S_IRGRP, ima_dir, NULL,
				   &ima_ascii_measurements_ops);
<<<<<<< HEAD
	if (IS_ERR(ascii_runtime_measurements))
		goto out;
=======
	if (IS_ERR(ascii_runtime_measurements)) {
		ret = PTR_ERR(ascii_runtime_measurements);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	runtime_measurements_count =
	    securityfs_create_file("runtime_measurements_count",
				   S_IRUSR | S_IRGRP, ima_dir, NULL,
				   &ima_measurements_count_ops);
<<<<<<< HEAD
	if (IS_ERR(runtime_measurements_count))
		goto out;
=======
	if (IS_ERR(runtime_measurements_count)) {
		ret = PTR_ERR(runtime_measurements_count);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	violations =
	    securityfs_create_file("violations", S_IRUSR | S_IRGRP,
				   ima_dir, NULL, &ima_htable_violations_ops);
<<<<<<< HEAD
	if (IS_ERR(violations))
		goto out;

	ima_policy = securityfs_create_file("policy",
					    S_IWUSR,
					    ima_dir, NULL,
					    &ima_measure_policy_ops);
	if (IS_ERR(ima_policy))
		goto out;

	return 0;
out:
	securityfs_remove(runtime_measurements_count);
	securityfs_remove(ascii_runtime_measurements);
	securityfs_remove(binary_runtime_measurements);
	securityfs_remove(ima_dir);
	securityfs_remove(ima_policy);
	return -1;
}

void __exit ima_fs_cleanup(void)
{
=======
	if (IS_ERR(violations)) {
		ret = PTR_ERR(violations);
		goto out;
	}

	ima_policy = securityfs_create_file("policy", POLICY_FILE_FLAGS,
					    ima_dir, NULL,
					    &ima_measure_policy_ops);
	if (IS_ERR(ima_policy)) {
		ret = PTR_ERR(ima_policy);
		goto out;
	}

	return 0;
out:
	securityfs_remove(ima_policy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	securityfs_remove(violations);
	securityfs_remove(runtime_measurements_count);
	securityfs_remove(ascii_runtime_measurements);
	securityfs_remove(binary_runtime_measurements);
<<<<<<< HEAD
	securityfs_remove(ima_dir);
	securityfs_remove(ima_policy);
=======
	securityfs_remove(ima_symlink);
	securityfs_remove(ima_dir);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
