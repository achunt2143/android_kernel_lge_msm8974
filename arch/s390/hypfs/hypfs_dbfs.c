<<<<<<< HEAD
/*
 * Hypervisor filesystem for Linux on s390 - debugfs interface
 *
 * Copyright (C) IBM Corp. 2010
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Hypervisor filesystem for Linux on s390 - debugfs interface
 *
 * Copyright IBM Corp. 2010
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author(s): Michael Holzheu <holzheu@linux.vnet.ibm.com>
 */

#include <linux/slab.h>
#include "hypfs.h"

static struct dentry *dbfs_dir;

static struct hypfs_dbfs_data *hypfs_dbfs_data_alloc(struct hypfs_dbfs_file *f)
{
	struct hypfs_dbfs_data *data;

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;
<<<<<<< HEAD
	kref_init(&data->kref);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data->dbfs_file = f;
	return data;
}

<<<<<<< HEAD
static void hypfs_dbfs_data_free(struct kref *kref)
{
	struct hypfs_dbfs_data *data;

	data = container_of(kref, struct hypfs_dbfs_data, kref);
=======
static void hypfs_dbfs_data_free(struct hypfs_dbfs_data *data)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data->dbfs_file->data_free(data->buf_free_ptr);
	kfree(data);
}

<<<<<<< HEAD
static void data_free_delayed(struct work_struct *work)
{
	struct hypfs_dbfs_data *data;
	struct hypfs_dbfs_file *df;

	df = container_of(work, struct hypfs_dbfs_file, data_free_work.work);
	mutex_lock(&df->lock);
	data = df->data;
	df->data = NULL;
	mutex_unlock(&df->lock);
	kref_put(&data->kref, hypfs_dbfs_data_free);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t dbfs_read(struct file *file, char __user *buf,
			 size_t size, loff_t *ppos)
{
	struct hypfs_dbfs_data *data;
	struct hypfs_dbfs_file *df;
	ssize_t rc;

	if (*ppos != 0)
		return 0;

<<<<<<< HEAD
	df = file->f_path.dentry->d_inode->i_private;
	mutex_lock(&df->lock);
	if (!df->data) {
		data = hypfs_dbfs_data_alloc(df);
		if (!data) {
			mutex_unlock(&df->lock);
			return -ENOMEM;
		}
		rc = df->data_create(&data->buf, &data->buf_free_ptr,
				     &data->size);
		if (rc) {
			mutex_unlock(&df->lock);
			kfree(data);
			return rc;
		}
		df->data = data;
		schedule_delayed_work(&df->data_free_work, HZ);
	}
	data = df->data;
	kref_get(&data->kref);
	mutex_unlock(&df->lock);

	rc = simple_read_from_buffer(buf, size, ppos, data->buf, data->size);
	kref_put(&data->kref, hypfs_dbfs_data_free);
=======
	df = file_inode(file)->i_private;
	mutex_lock(&df->lock);
	data = hypfs_dbfs_data_alloc(df);
	if (!data) {
		mutex_unlock(&df->lock);
		return -ENOMEM;
	}
	rc = df->data_create(&data->buf, &data->buf_free_ptr, &data->size);
	if (rc) {
		mutex_unlock(&df->lock);
		kfree(data);
		return rc;
	}
	mutex_unlock(&df->lock);

	rc = simple_read_from_buffer(buf, size, ppos, data->buf, data->size);
	hypfs_dbfs_data_free(data);
	return rc;
}

static long dbfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct hypfs_dbfs_file *df = file_inode(file)->i_private;
	long rc;

	mutex_lock(&df->lock);
	if (df->unlocked_ioctl)
		rc = df->unlocked_ioctl(file, cmd, arg);
	else
		rc = -ENOTTY;
	mutex_unlock(&df->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static const struct file_operations dbfs_ops = {
	.read		= dbfs_read,
	.llseek		= no_llseek,
<<<<<<< HEAD
};

int hypfs_dbfs_create_file(struct hypfs_dbfs_file *df)
{
	df->dentry = debugfs_create_file(df->name, 0400, dbfs_dir, df,
					 &dbfs_ops);
	if (IS_ERR(df->dentry))
		return PTR_ERR(df->dentry);
	mutex_init(&df->lock);
	INIT_DELAYED_WORK(&df->data_free_work, data_free_delayed);
	return 0;
=======
	.unlocked_ioctl = dbfs_ioctl,
};

void hypfs_dbfs_create_file(struct hypfs_dbfs_file *df)
{
	df->dentry = debugfs_create_file(df->name, 0400, dbfs_dir, df,
					 &dbfs_ops);
	mutex_init(&df->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void hypfs_dbfs_remove_file(struct hypfs_dbfs_file *df)
{
	debugfs_remove(df->dentry);
}

<<<<<<< HEAD
int hypfs_dbfs_init(void)
{
	dbfs_dir = debugfs_create_dir("s390_hypfs", NULL);
	if (IS_ERR(dbfs_dir))
		return PTR_ERR(dbfs_dir);
	return 0;
}

void hypfs_dbfs_exit(void)
{
	debugfs_remove(dbfs_dir);
}
=======
static int __init hypfs_dbfs_init(void)
{
	int rc = -ENODATA;

	dbfs_dir = debugfs_create_dir("s390_hypfs", NULL);
	if (hypfs_diag_init())
		goto fail_dbfs_exit;
	if (hypfs_vm_init())
		goto fail_hypfs_diag_exit;
	hypfs_sprp_init();
	if (hypfs_diag0c_init())
		goto fail_hypfs_sprp_exit;
	rc = hypfs_fs_init();
	if (rc)
		goto fail_hypfs_diag0c_exit;
	return 0;

fail_hypfs_diag0c_exit:
	hypfs_diag0c_exit();
fail_hypfs_sprp_exit:
	hypfs_sprp_exit();
	hypfs_vm_exit();
fail_hypfs_diag_exit:
	hypfs_diag_exit();
	pr_err("Initialization of hypfs failed with rc=%i\n", rc);
fail_dbfs_exit:
	debugfs_remove(dbfs_dir);
	return rc;
}
device_initcall(hypfs_dbfs_init)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
