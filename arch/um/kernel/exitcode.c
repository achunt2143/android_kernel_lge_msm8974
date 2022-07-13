<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * If read and write race, the read will still atomically read a valid
 * value.
 */
int uml_exitcode = 0;

static int exitcode_proc_show(struct seq_file *m, void *v)
{
	int val;

	/*
	 * Save uml_exitcode in a local so that we don't need to guarantee
	 * that sprintf accesses it atomically.
	 */
	val = uml_exitcode;
	seq_printf(m, "%d\n", val);
	return 0;
}

static int exitcode_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, exitcode_proc_show, NULL);
}

static ssize_t exitcode_proc_write(struct file *file,
		const char __user *buffer, size_t count, loff_t *pos)
{
	char *end, buf[sizeof("nnnnn\0")];
	size_t size;
	int tmp;

	size = min(count, sizeof(buf));
	if (copy_from_user(buf, buffer, size))
		return -EFAULT;

	tmp = simple_strtol(buf, &end, 0);
	if ((*end != '\0') && !isspace(*end))
		return -EINVAL;

	uml_exitcode = tmp;
	return count;
}

<<<<<<< HEAD
static const struct file_operations exitcode_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= exitcode_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= exitcode_proc_write,
=======
static const struct proc_ops exitcode_proc_ops = {
	.proc_open	= exitcode_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
	.proc_write	= exitcode_proc_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int make_proc_exitcode(void)
{
	struct proc_dir_entry *ent;

<<<<<<< HEAD
	ent = proc_create("exitcode", 0600, NULL, &exitcode_proc_fops);
=======
	ent = proc_create("exitcode", 0600, NULL, &exitcode_proc_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ent == NULL) {
		printk(KERN_WARNING "make_proc_exitcode : Failed to register "
		       "/proc/exitcode\n");
		return 0;
	}
	return 0;
}

__initcall(make_proc_exitcode);
