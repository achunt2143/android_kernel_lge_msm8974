<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
=======
#include <linux/blkdev.h>
#include "internal.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int devinfo_show(struct seq_file *f, void *v)
{
	int i = *(loff_t *) v;

<<<<<<< HEAD
	if (i < CHRDEV_MAJOR_HASH_SIZE) {
=======
	if (i < CHRDEV_MAJOR_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (i == 0)
			seq_puts(f, "Character devices:\n");
		chrdev_show(f, i);
	}
#ifdef CONFIG_BLOCK
	else {
<<<<<<< HEAD
		i -= CHRDEV_MAJOR_HASH_SIZE;
=======
		i -= CHRDEV_MAJOR_MAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (i == 0)
			seq_puts(f, "\nBlock devices:\n");
		blkdev_show(f, i);
	}
#endif
	return 0;
}

static void *devinfo_start(struct seq_file *f, loff_t *pos)
{
<<<<<<< HEAD
	if (*pos < (BLKDEV_MAJOR_HASH_SIZE + CHRDEV_MAJOR_HASH_SIZE))
=======
	if (*pos < (BLKDEV_MAJOR_MAX + CHRDEV_MAJOR_MAX))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return pos;
	return NULL;
}

static void *devinfo_next(struct seq_file *f, void *v, loff_t *pos)
{
	(*pos)++;
<<<<<<< HEAD
	if (*pos >= (BLKDEV_MAJOR_HASH_SIZE + CHRDEV_MAJOR_HASH_SIZE))
=======
	if (*pos >= (BLKDEV_MAJOR_MAX + CHRDEV_MAJOR_MAX))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	return pos;
}

static void devinfo_stop(struct seq_file *f, void *v)
{
	/* Nothing to do */
}

static const struct seq_operations devinfo_ops = {
	.start = devinfo_start,
	.next  = devinfo_next,
	.stop  = devinfo_stop,
	.show  = devinfo_show
};

<<<<<<< HEAD
static int devinfo_open(struct inode *inode, struct file *filp)
{
	return seq_open(filp, &devinfo_ops);
}

static const struct file_operations proc_devinfo_operations = {
	.open		= devinfo_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_devices_init(void)
{
	proc_create("devices", 0, NULL, &proc_devinfo_operations);
	return 0;
}
module_init(proc_devices_init);
=======
static int __init proc_devices_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_seq("devices", 0, NULL, &devinfo_ops);
	pde_make_permanent(pde);
	return 0;
}
fs_initcall(proc_devices_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
