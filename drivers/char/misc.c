<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/drivers/char/misc.c
 *
 * Generic misc open routine by Johan Myreen
 *
 * Based on code from Linus
 *
 * Teemu Rantanen's Microsoft Busmouse support and Derrick Cole's
 *   changes incorporated into 0.97pl4
 *   by Peter Cervasio (pete%q106fm.uucp@wupost.wustl.edu) (08SEP92)
 *   See busmouse.c for particulars.
 *
 * Made things a lot mode modular - easy to compile in just one or two
 * of the misc drivers, as they are now completely independent. Linus.
 *
 * Support for loadable modules. 8-Sep-95 Philip Blundell <pjb27@cam.ac.uk>
 *
 * Fixed a failing symbol register to free the device registration
 *		Alan Cox <alan@lxorguk.ukuu.org.uk> 21-Jan-96
 *
 * Dynamic minors and /proc/mice by Alessandro Rubini. 26-Mar-96
 *
 * Renamed to misc and miscdevice to be more accurate. Alan Cox 26-Mar-96
 *
 * Handling of mouse minor numbers for kerneld:
 *  Idea by Jacques Gelinas <jack@solucorp.qc.ca>,
 *  adapted by Bjorn Ekwall <bj0rn@blox.se>
 *  corrected by Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Changes for kmod (from kerneld):
 *	Cyrus Durgin <cider@speakeasy.org>
 *
 * Added devfs support. Richard Gooch <rgooch@atnf.csiro.au>  10-Jan-1998
 */

#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

/*
 * Head entry for the doubly linked miscdevice list
 */
static LIST_HEAD(misc_list);
static DEFINE_MUTEX(misc_mtx);

/*
 * Assigned numbers, used for dynamic minors
 */
<<<<<<< HEAD
#define DYNAMIC_MINORS 96 /* like dynamic majors */
static DECLARE_BITMAP(misc_minors, DYNAMIC_MINORS);
=======
#define DYNAMIC_MINORS 128 /* like dynamic majors */
static DEFINE_IDA(misc_minors_ida);

static int misc_minor_alloc(void)
{
	int ret;

	ret = ida_alloc_max(&misc_minors_ida, DYNAMIC_MINORS - 1, GFP_KERNEL);
	if (ret >= 0) {
		ret = DYNAMIC_MINORS - ret - 1;
	} else {
		ret = ida_alloc_range(&misc_minors_ida, MISC_DYNAMIC_MINOR + 1,
				      MINORMASK, GFP_KERNEL);
	}
	return ret;
}

static void misc_minor_free(int minor)
{
	if (minor < DYNAMIC_MINORS)
		ida_free(&misc_minors_ida, DYNAMIC_MINORS - minor - 1);
	else if (minor > MISC_DYNAMIC_MINOR)
		ida_free(&misc_minors_ida, minor);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PROC_FS
static void *misc_seq_start(struct seq_file *seq, loff_t *pos)
{
	mutex_lock(&misc_mtx);
	return seq_list_start(&misc_list, *pos);
}

static void *misc_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	return seq_list_next(v, &misc_list, pos);
}

static void misc_seq_stop(struct seq_file *seq, void *v)
{
	mutex_unlock(&misc_mtx);
}

static int misc_seq_show(struct seq_file *seq, void *v)
{
	const struct miscdevice *p = list_entry(v, struct miscdevice, list);

	seq_printf(seq, "%3i %s\n", p->minor, p->name ? p->name : "");
	return 0;
}


static const struct seq_operations misc_seq_ops = {
	.start = misc_seq_start,
	.next  = misc_seq_next,
	.stop  = misc_seq_stop,
	.show  = misc_seq_show,
};
<<<<<<< HEAD

static int misc_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &misc_seq_ops);
}

static const struct file_operations misc_proc_fops = {
	.owner	 = THIS_MODULE,
	.open    = misc_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};
#endif

static int misc_open(struct inode * inode, struct file * file)
{
	int minor = iminor(inode);
	struct miscdevice *c;
	int err = -ENODEV;
	const struct file_operations *old_fops, *new_fops = NULL;

	mutex_lock(&misc_mtx);
	
	list_for_each_entry(c, &misc_list, list) {
		if (c->minor == minor) {
			new_fops = fops_get(c->fops);		
			break;
		}
	}
		
=======
#endif

static int misc_open(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);
	struct miscdevice *c = NULL, *iter;
	int err = -ENODEV;
	const struct file_operations *new_fops = NULL;

	mutex_lock(&misc_mtx);

	list_for_each_entry(iter, &misc_list, list) {
		if (iter->minor != minor)
			continue;
		c = iter;
		new_fops = fops_get(iter->fops);
		break;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!new_fops) {
		mutex_unlock(&misc_mtx);
		request_module("char-major-%d-%d", MISC_MAJOR, minor);
		mutex_lock(&misc_mtx);

<<<<<<< HEAD
		list_for_each_entry(c, &misc_list, list) {
			if (c->minor == minor) {
				new_fops = fops_get(c->fops);
				break;
			}
=======
		list_for_each_entry(iter, &misc_list, list) {
			if (iter->minor != minor)
				continue;
			c = iter;
			new_fops = fops_get(iter->fops);
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (!new_fops)
			goto fail;
	}

<<<<<<< HEAD
	err = 0;
	old_fops = file->f_op;
	file->f_op = new_fops;
	if (file->f_op->open) {
		file->private_data = c;
		err=file->f_op->open(inode,file);
		if (err) {
			fops_put(file->f_op);
			file->f_op = fops_get(old_fops);
		}
	}
	fops_put(old_fops);
=======
	/*
	 * Place the miscdevice in the file's
	 * private_data so it can be used by the
	 * file operations, including f_op->open below
	 */
	file->private_data = c;

	err = 0;
	replace_fops(file, new_fops);
	if (file->f_op->open)
		err = file->f_op->open(inode, file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
fail:
	mutex_unlock(&misc_mtx);
	return err;
}

<<<<<<< HEAD
static struct class *misc_class;
=======
static char *misc_devnode(const struct device *dev, umode_t *mode)
{
	const struct miscdevice *c = dev_get_drvdata(dev);

	if (mode && c->mode)
		*mode = c->mode;
	if (c->nodename)
		return kstrdup(c->nodename, GFP_KERNEL);
	return NULL;
}

static const struct class misc_class = {
	.name		= "misc",
	.devnode	= misc_devnode,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct file_operations misc_fops = {
	.owner		= THIS_MODULE,
	.open		= misc_open,
	.llseek		= noop_llseek,
};

/**
 *	misc_register	-	register a miscellaneous device
 *	@misc: device structure
<<<<<<< HEAD
 *	
=======
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Register a miscellaneous device with the kernel. If the minor
 *	number is set to %MISC_DYNAMIC_MINOR a minor number is assigned
 *	and placed in the minor field of the structure. For other cases
 *	the minor number requested is used.
 *
 *	The structure passed is linked into the kernel and may not be
<<<<<<< HEAD
 *	destroyed until it has been unregistered.
=======
 *	destroyed until it has been unregistered. By default, an open()
 *	syscall to the device sets file->private_data to point to the
 *	structure. Drivers don't need open in fops for this.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *	A zero is returned on success and a negative errno code for
 *	failure.
 */
<<<<<<< HEAD
 
int misc_register(struct miscdevice * misc)
{
	struct miscdevice *c;
	dev_t dev;
	int err = 0;
=======

int misc_register(struct miscdevice *misc)
{
	dev_t dev;
	int err = 0;
	bool is_dynamic = (misc->minor == MISC_DYNAMIC_MINOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	INIT_LIST_HEAD(&misc->list);

	mutex_lock(&misc_mtx);
<<<<<<< HEAD
	list_for_each_entry(c, &misc_list, list) {
		if (c->minor == misc->minor) {
			mutex_unlock(&misc_mtx);
			return -EBUSY;
		}
	}

	if (misc->minor == MISC_DYNAMIC_MINOR) {
		int i = find_first_zero_bit(misc_minors, DYNAMIC_MINORS);
		if (i >= DYNAMIC_MINORS) {
			mutex_unlock(&misc_mtx);
			return -EBUSY;
		}
		misc->minor = DYNAMIC_MINORS - i - 1;
		set_bit(i, misc_minors);
=======

	if (is_dynamic) {
		int i = misc_minor_alloc();

		if (i < 0) {
			err = -EBUSY;
			goto out;
		}
		misc->minor = i;
	} else {
		struct miscdevice *c;

		list_for_each_entry(c, &misc_list, list) {
			if (c->minor == misc->minor) {
				err = -EBUSY;
				goto out;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	dev = MKDEV(MISC_MAJOR, misc->minor);

<<<<<<< HEAD
	misc->this_device = device_create(misc_class, misc->parent, dev,
					  misc, "%s", misc->name);
	if (IS_ERR(misc->this_device)) {
		int i = DYNAMIC_MINORS - misc->minor - 1;
		if (i < DYNAMIC_MINORS && i >= 0)
			clear_bit(i, misc_minors);
=======
	misc->this_device =
		device_create_with_groups(&misc_class, misc->parent, dev,
					  misc, misc->groups, "%s", misc->name);
	if (IS_ERR(misc->this_device)) {
		if (is_dynamic) {
			misc_minor_free(misc->minor);
			misc->minor = MISC_DYNAMIC_MINOR;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = PTR_ERR(misc->this_device);
		goto out;
	}

	/*
	 * Add it to the front, so that later devices can "override"
	 * earlier defaults
	 */
	list_add(&misc->list, &misc_list);
 out:
	mutex_unlock(&misc_mtx);
	return err;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(misc_register);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 *	misc_deregister - unregister a miscellaneous device
 *	@misc: device to unregister
 *
 *	Unregister a miscellaneous device that was previously
<<<<<<< HEAD
 *	successfully registered with misc_register(). Success
 *	is indicated by a zero return, a negative errno code
 *	indicates an error.
 */

int misc_deregister(struct miscdevice *misc)
{
	int i = DYNAMIC_MINORS - misc->minor - 1;

	if (WARN_ON(list_empty(&misc->list)))
		return -EINVAL;

	mutex_lock(&misc_mtx);
	list_del(&misc->list);
	device_destroy(misc_class, MKDEV(MISC_MAJOR, misc->minor));
	if (i < DYNAMIC_MINORS && i >= 0)
		clear_bit(i, misc_minors);
	mutex_unlock(&misc_mtx);
	return 0;
}

EXPORT_SYMBOL(misc_register);
EXPORT_SYMBOL(misc_deregister);

static char *misc_devnode(struct device *dev, umode_t *mode)
{
	struct miscdevice *c = dev_get_drvdata(dev);

	if (mode && c->mode)
		*mode = c->mode;
	if (c->nodename)
		return kstrdup(c->nodename, GFP_KERNEL);
	return NULL;
}

static int __init misc_init(void)
{
	int err;

#ifdef CONFIG_PROC_FS
	proc_create("misc", 0, NULL, &misc_proc_fops);
#endif
	misc_class = class_create(THIS_MODULE, "misc");
	err = PTR_ERR(misc_class);
	if (IS_ERR(misc_class))
		goto fail_remove;

	err = -EIO;
	if (register_chrdev(MISC_MAJOR,"misc",&misc_fops))
		goto fail_printk;
	misc_class->devnode = misc_devnode;
	return 0;

fail_printk:
	printk("unable to get major %d for misc devices\n", MISC_MAJOR);
	class_destroy(misc_class);
fail_remove:
	remove_proc_entry("misc", NULL);
=======
 *	successfully registered with misc_register().
 */

void misc_deregister(struct miscdevice *misc)
{
	if (WARN_ON(list_empty(&misc->list)))
		return;

	mutex_lock(&misc_mtx);
	list_del(&misc->list);
	device_destroy(&misc_class, MKDEV(MISC_MAJOR, misc->minor));
	misc_minor_free(misc->minor);
	mutex_unlock(&misc_mtx);
}
EXPORT_SYMBOL(misc_deregister);

static int __init misc_init(void)
{
	int err;
	struct proc_dir_entry *ret;

	ret = proc_create_seq("misc", 0, NULL, &misc_seq_ops);
	err = class_register(&misc_class);
	if (err)
		goto fail_remove;

	err = -EIO;
	if (register_chrdev(MISC_MAJOR, "misc", &misc_fops))
		goto fail_printk;
	return 0;

fail_printk:
	pr_err("unable to get major %d for misc devices\n", MISC_MAJOR);
	class_unregister(&misc_class);
fail_remove:
	if (ret)
		remove_proc_entry("misc", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
subsys_initcall(misc_init);
