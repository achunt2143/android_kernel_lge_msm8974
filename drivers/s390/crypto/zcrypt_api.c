<<<<<<< HEAD
/*
 *  linux/drivers/s390/crypto/zcrypt_api.c
 *
 *  zcrypt 2.1.0
 *
 *  Copyright (C)  2001, 2006 IBM Corporation
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 *  Copyright IBM Corp. 2001, 2018
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  Author(s): Robert Burroughs
 *	       Eric Rossman (edrossma@us.ibm.com)
 *	       Cornelia Huck <cornelia.huck@de.ibm.com>
 *
 *  Hotplug & misc device support: Jochen Roehrig (roehrig@de.ibm.com)
 *  Major cleanup & driver split: Martin Schwidefsky <schwidefsky@de.ibm.com>
 *				  Ralph Wuerthner <rwuerthn@de.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

=======
 *  MSGTYPE restruct:		  Holger Dengler <hd@linux.vnet.ibm.com>
 *  Multiple device nodes: Harald Freudenberger <freude@linux.ibm.com>
 */

#define KMSG_COMPONENT "zcrypt"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/compat.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <asm/uaccess.h>
#include <linux/hw_random.h>

#include "zcrypt_api.h"
=======
#include <linux/compat.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <linux/uaccess.h>
#include <linux/hw_random.h>
#include <linux/debugfs.h>
#include <linux/cdev.h>
#include <linux/ctype.h>
#include <linux/capability.h>
#include <asm/debug.h>

#define CREATE_TRACE_POINTS
#include <asm/trace/zcrypt.h>

#include "zcrypt_api.h"
#include "zcrypt_debug.h"

#include "zcrypt_msgtype6.h"
#include "zcrypt_msgtype50.h"
#include "zcrypt_ccamisc.h"
#include "zcrypt_ep11misc.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Module description.
 */
MODULE_AUTHOR("IBM Corporation");
<<<<<<< HEAD
MODULE_DESCRIPTION("Cryptographic Coprocessor interface, "
		   "Copyright 2001, 2006 IBM Corporation");
MODULE_LICENSE("GPL");

static DEFINE_SPINLOCK(zcrypt_device_lock);
static LIST_HEAD(zcrypt_device_list);
static int zcrypt_device_count = 0;
static atomic_t zcrypt_open_count = ATOMIC_INIT(0);

static int zcrypt_rng_device_add(void);
static void zcrypt_rng_device_remove(void);

/*
 * Device attributes common for all crypto devices.
 */
static ssize_t zcrypt_type_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct zcrypt_device *zdev = to_ap_dev(dev)->private;
	return snprintf(buf, PAGE_SIZE, "%s\n", zdev->type_string);
}

static DEVICE_ATTR(type, 0444, zcrypt_type_show, NULL);

static ssize_t zcrypt_online_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	struct zcrypt_device *zdev = to_ap_dev(dev)->private;
	return snprintf(buf, PAGE_SIZE, "%d\n", zdev->online);
}

static ssize_t zcrypt_online_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct zcrypt_device *zdev = to_ap_dev(dev)->private;
	int online;

	if (sscanf(buf, "%d\n", &online) != 1 || online < 0 || online > 1)
		return -EINVAL;
	zdev->online = online;
	if (!online)
		ap_flush_queue(zdev->ap_dev);
	return count;
}

static DEVICE_ATTR(online, 0644, zcrypt_online_show, zcrypt_online_store);

static struct attribute * zcrypt_device_attrs[] = {
	&dev_attr_type.attr,
	&dev_attr_online.attr,
	NULL,
};

static struct attribute_group zcrypt_device_attr_group = {
	.attrs = zcrypt_device_attrs,
};

/**
 * __zcrypt_increase_preference(): Increase preference of a crypto device.
 * @zdev: Pointer the crypto device
 *
 * Move the device towards the head of the device list.
 * Need to be called while holding the zcrypt device list lock.
 * Note: cards with speed_rating of 0 are kept at the end of the list.
 */
static void __zcrypt_increase_preference(struct zcrypt_device *zdev)
{
	struct zcrypt_device *tmp;
	struct list_head *l;

	if (zdev->speed_rating == 0)
		return;
	for (l = zdev->list.prev; l != &zcrypt_device_list; l = l->prev) {
		tmp = list_entry(l, struct zcrypt_device, list);
		if ((tmp->request_count + 1) * tmp->speed_rating <=
		    (zdev->request_count + 1) * zdev->speed_rating &&
		    tmp->speed_rating != 0)
			break;
	}
	if (l == zdev->list.prev)
		return;
	/* Move zdev behind l */
	list_move(&zdev->list, l);
}

/**
 * __zcrypt_decrease_preference(): Decrease preference of a crypto device.
 * @zdev: Pointer to a crypto device.
 *
 * Move the device towards the tail of the device list.
 * Need to be called while holding the zcrypt device list lock.
 * Note: cards with speed_rating of 0 are kept at the end of the list.
 */
static void __zcrypt_decrease_preference(struct zcrypt_device *zdev)
{
	struct zcrypt_device *tmp;
	struct list_head *l;

	if (zdev->speed_rating == 0)
		return;
	for (l = zdev->list.next; l != &zcrypt_device_list; l = l->next) {
		tmp = list_entry(l, struct zcrypt_device, list);
		if ((tmp->request_count + 1) * tmp->speed_rating >
		    (zdev->request_count + 1) * zdev->speed_rating ||
		    tmp->speed_rating == 0)
			break;
	}
	if (l == zdev->list.next)
		return;
	/* Move zdev before l */
	list_move_tail(&zdev->list, l);
}

static void zcrypt_device_release(struct kref *kref)
{
	struct zcrypt_device *zdev =
		container_of(kref, struct zcrypt_device, refcount);
	zcrypt_device_free(zdev);
}

void zcrypt_device_get(struct zcrypt_device *zdev)
{
	kref_get(&zdev->refcount);
}
EXPORT_SYMBOL(zcrypt_device_get);

int zcrypt_device_put(struct zcrypt_device *zdev)
{
	return kref_put(&zdev->refcount, zcrypt_device_release);
}
EXPORT_SYMBOL(zcrypt_device_put);

struct zcrypt_device *zcrypt_device_alloc(size_t max_response_size)
{
	struct zcrypt_device *zdev;

	zdev = kzalloc(sizeof(struct zcrypt_device), GFP_KERNEL);
	if (!zdev)
		return NULL;
	zdev->reply.message = kmalloc(max_response_size, GFP_KERNEL);
	if (!zdev->reply.message)
		goto out_free;
	zdev->reply.length = max_response_size;
	spin_lock_init(&zdev->lock);
	INIT_LIST_HEAD(&zdev->list);
	return zdev;

out_free:
	kfree(zdev);
	return NULL;
}
EXPORT_SYMBOL(zcrypt_device_alloc);

void zcrypt_device_free(struct zcrypt_device *zdev)
{
	kfree(zdev->reply.message);
	kfree(zdev);
}
EXPORT_SYMBOL(zcrypt_device_free);

/**
 * zcrypt_device_register() - Register a crypto device.
 * @zdev: Pointer to a crypto device
 *
 * Register a crypto device. Returns 0 if successful.
 */
int zcrypt_device_register(struct zcrypt_device *zdev)
{
	int rc;

	rc = sysfs_create_group(&zdev->ap_dev->device.kobj,
				&zcrypt_device_attr_group);
	if (rc)
		goto out;
	get_device(&zdev->ap_dev->device);
	kref_init(&zdev->refcount);
	spin_lock_bh(&zcrypt_device_lock);
	zdev->online = 1;	/* New devices are online by default. */
	list_add_tail(&zdev->list, &zcrypt_device_list);
	__zcrypt_increase_preference(zdev);
	zcrypt_device_count++;
	spin_unlock_bh(&zcrypt_device_lock);
	if (zdev->ops->rng) {
		rc = zcrypt_rng_device_add();
		if (rc)
			goto out_unregister;
	}
	return 0;

out_unregister:
	spin_lock_bh(&zcrypt_device_lock);
	zcrypt_device_count--;
	list_del_init(&zdev->list);
	spin_unlock_bh(&zcrypt_device_lock);
	sysfs_remove_group(&zdev->ap_dev->device.kobj,
			   &zcrypt_device_attr_group);
	put_device(&zdev->ap_dev->device);
	zcrypt_device_put(zdev);
out:
	return rc;
}
EXPORT_SYMBOL(zcrypt_device_register);

/**
 * zcrypt_device_unregister(): Unregister a crypto device.
 * @zdev: Pointer to crypto device
 *
 * Unregister a crypto device.
 */
void zcrypt_device_unregister(struct zcrypt_device *zdev)
{
	if (zdev->ops->rng)
		zcrypt_rng_device_remove();
	spin_lock_bh(&zcrypt_device_lock);
	zcrypt_device_count--;
	list_del_init(&zdev->list);
	spin_unlock_bh(&zcrypt_device_lock);
	sysfs_remove_group(&zdev->ap_dev->device.kobj,
			   &zcrypt_device_attr_group);
	put_device(&zdev->ap_dev->device);
	zcrypt_device_put(zdev);
}
EXPORT_SYMBOL(zcrypt_device_unregister);

/**
=======
MODULE_DESCRIPTION("Cryptographic Coprocessor interface, " \
		   "Copyright IBM Corp. 2001, 2012");
MODULE_LICENSE("GPL");

/*
 * zcrypt tracepoint functions
 */
EXPORT_TRACEPOINT_SYMBOL(s390_zcrypt_req);
EXPORT_TRACEPOINT_SYMBOL(s390_zcrypt_rep);

DEFINE_SPINLOCK(zcrypt_list_lock);
LIST_HEAD(zcrypt_card_list);

static atomic_t zcrypt_open_count = ATOMIC_INIT(0);

static LIST_HEAD(zcrypt_ops_list);

/* Zcrypt related debug feature stuff. */
debug_info_t *zcrypt_dbf_info;

/*
 * Process a rescan of the transport layer.
 * Runs a synchronous AP bus rescan.
 * Returns true if something has changed (for example the
 * bus scan has found and build up new devices) and it is
 * worth to do a retry. Otherwise false is returned meaning
 * no changes on the AP bus level.
 */
static inline bool zcrypt_process_rescan(void)
{
	return ap_bus_force_rescan();
}

void zcrypt_msgtype_register(struct zcrypt_ops *zops)
{
	list_add_tail(&zops->list, &zcrypt_ops_list);
}

void zcrypt_msgtype_unregister(struct zcrypt_ops *zops)
{
	list_del_init(&zops->list);
}

struct zcrypt_ops *zcrypt_msgtype(unsigned char *name, int variant)
{
	struct zcrypt_ops *zops;

	list_for_each_entry(zops, &zcrypt_ops_list, list)
		if (zops->variant == variant &&
		    (!strncmp(zops->name, name, sizeof(zops->name))))
			return zops;
	return NULL;
}
EXPORT_SYMBOL(zcrypt_msgtype);

/*
 * Multi device nodes extension functions.
 */

struct zcdn_device;

static void zcdn_device_release(struct device *dev);
static const struct class zcrypt_class = {
	.name = ZCRYPT_NAME,
	.dev_release = zcdn_device_release,
};
static dev_t zcrypt_devt;
static struct cdev zcrypt_cdev;

struct zcdn_device {
	struct device device;
	struct ap_perms perms;
};

#define to_zcdn_dev(x) container_of((x), struct zcdn_device, device)

#define ZCDN_MAX_NAME 32

static int zcdn_create(const char *name);
static int zcdn_destroy(const char *name);

/*
 * Find zcdn device by name.
 * Returns reference to the zcdn device which needs to be released
 * with put_device() after use.
 */
static inline struct zcdn_device *find_zcdndev_by_name(const char *name)
{
	struct device *dev = class_find_device_by_name(&zcrypt_class, name);

	return dev ? to_zcdn_dev(dev) : NULL;
}

/*
 * Find zcdn device by devt value.
 * Returns reference to the zcdn device which needs to be released
 * with put_device() after use.
 */
static inline struct zcdn_device *find_zcdndev_by_devt(dev_t devt)
{
	struct device *dev = class_find_device_by_devt(&zcrypt_class, devt);

	return dev ? to_zcdn_dev(dev) : NULL;
}

static ssize_t ioctlmask_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);
	int i, n;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	n = sysfs_emit(buf, "0x");
	for (i = 0; i < sizeof(zcdndev->perms.ioctlm) / sizeof(long); i++)
		n += sysfs_emit_at(buf, n, "%016lx", zcdndev->perms.ioctlm[i]);
	n += sysfs_emit_at(buf, n, "\n");

	mutex_unlock(&ap_perms_mutex);

	return n;
}

static ssize_t ioctlmask_store(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	int rc;
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);

	rc = ap_parse_mask_str(buf, zcdndev->perms.ioctlm,
			       AP_IOCTLS, &ap_perms_mutex);
	if (rc)
		return rc;

	return count;
}

static DEVICE_ATTR_RW(ioctlmask);

static ssize_t apmask_show(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);
	int i, n;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	n = sysfs_emit(buf, "0x");
	for (i = 0; i < sizeof(zcdndev->perms.apm) / sizeof(long); i++)
		n += sysfs_emit_at(buf, n, "%016lx", zcdndev->perms.apm[i]);
	n += sysfs_emit_at(buf, n, "\n");

	mutex_unlock(&ap_perms_mutex);

	return n;
}

static ssize_t apmask_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
{
	int rc;
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);

	rc = ap_parse_mask_str(buf, zcdndev->perms.apm,
			       AP_DEVICES, &ap_perms_mutex);
	if (rc)
		return rc;

	return count;
}

static DEVICE_ATTR_RW(apmask);

static ssize_t aqmask_show(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);
	int i, n;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	n = sysfs_emit(buf, "0x");
	for (i = 0; i < sizeof(zcdndev->perms.aqm) / sizeof(long); i++)
		n += sysfs_emit_at(buf, n, "%016lx", zcdndev->perms.aqm[i]);
	n += sysfs_emit_at(buf, n, "\n");

	mutex_unlock(&ap_perms_mutex);

	return n;
}

static ssize_t aqmask_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
{
	int rc;
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);

	rc = ap_parse_mask_str(buf, zcdndev->perms.aqm,
			       AP_DOMAINS, &ap_perms_mutex);
	if (rc)
		return rc;

	return count;
}

static DEVICE_ATTR_RW(aqmask);

static ssize_t admask_show(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);
	int i, n;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	n = sysfs_emit(buf, "0x");
	for (i = 0; i < sizeof(zcdndev->perms.adm) / sizeof(long); i++)
		n += sysfs_emit_at(buf, n, "%016lx", zcdndev->perms.adm[i]);
	n += sysfs_emit_at(buf, n, "\n");

	mutex_unlock(&ap_perms_mutex);

	return n;
}

static ssize_t admask_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
{
	int rc;
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);

	rc = ap_parse_mask_str(buf, zcdndev->perms.adm,
			       AP_DOMAINS, &ap_perms_mutex);
	if (rc)
		return rc;

	return count;
}

static DEVICE_ATTR_RW(admask);

static struct attribute *zcdn_dev_attrs[] = {
	&dev_attr_ioctlmask.attr,
	&dev_attr_apmask.attr,
	&dev_attr_aqmask.attr,
	&dev_attr_admask.attr,
	NULL
};

static struct attribute_group zcdn_dev_attr_group = {
	.attrs = zcdn_dev_attrs
};

static const struct attribute_group *zcdn_dev_attr_groups[] = {
	&zcdn_dev_attr_group,
	NULL
};

static ssize_t zcdn_create_store(const struct class *class,
				 const struct class_attribute *attr,
				 const char *buf, size_t count)
{
	int rc;
	char name[ZCDN_MAX_NAME];

	strscpy(name, skip_spaces(buf), sizeof(name));

	rc = zcdn_create(strim(name));

	return rc ? rc : count;
}

static const struct class_attribute class_attr_zcdn_create =
	__ATTR(create, 0600, NULL, zcdn_create_store);

static ssize_t zcdn_destroy_store(const struct class *class,
				  const struct class_attribute *attr,
				  const char *buf, size_t count)
{
	int rc;
	char name[ZCDN_MAX_NAME];

	strscpy(name, skip_spaces(buf), sizeof(name));

	rc = zcdn_destroy(strim(name));

	return rc ? rc : count;
}

static const struct class_attribute class_attr_zcdn_destroy =
	__ATTR(destroy, 0600, NULL, zcdn_destroy_store);

static void zcdn_device_release(struct device *dev)
{
	struct zcdn_device *zcdndev = to_zcdn_dev(dev);

	ZCRYPT_DBF_INFO("%s releasing zcdn device %d:%d\n",
			__func__, MAJOR(dev->devt), MINOR(dev->devt));

	kfree(zcdndev);
}

static int zcdn_create(const char *name)
{
	dev_t devt;
	int i, rc = 0;
	struct zcdn_device *zcdndev;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	/* check if device node with this name already exists */
	if (name[0]) {
		zcdndev = find_zcdndev_by_name(name);
		if (zcdndev) {
			put_device(&zcdndev->device);
			rc = -EEXIST;
			goto unlockout;
		}
	}

	/* find an unused minor number */
	for (i = 0; i < ZCRYPT_MAX_MINOR_NODES; i++) {
		devt = MKDEV(MAJOR(zcrypt_devt), MINOR(zcrypt_devt) + i);
		zcdndev = find_zcdndev_by_devt(devt);
		if (zcdndev)
			put_device(&zcdndev->device);
		else
			break;
	}
	if (i == ZCRYPT_MAX_MINOR_NODES) {
		rc = -ENOSPC;
		goto unlockout;
	}

	/* alloc and prepare a new zcdn device */
	zcdndev = kzalloc(sizeof(*zcdndev), GFP_KERNEL);
	if (!zcdndev) {
		rc = -ENOMEM;
		goto unlockout;
	}
	zcdndev->device.release = zcdn_device_release;
	zcdndev->device.class = &zcrypt_class;
	zcdndev->device.devt = devt;
	zcdndev->device.groups = zcdn_dev_attr_groups;
	if (name[0])
		rc = dev_set_name(&zcdndev->device, "%s", name);
	else
		rc = dev_set_name(&zcdndev->device, ZCRYPT_NAME "_%d", (int)MINOR(devt));
	if (rc) {
		kfree(zcdndev);
		goto unlockout;
	}
	rc = device_register(&zcdndev->device);
	if (rc) {
		put_device(&zcdndev->device);
		goto unlockout;
	}

	ZCRYPT_DBF_INFO("%s created zcdn device %d:%d\n",
			__func__, MAJOR(devt), MINOR(devt));

unlockout:
	mutex_unlock(&ap_perms_mutex);
	return rc;
}

static int zcdn_destroy(const char *name)
{
	int rc = 0;
	struct zcdn_device *zcdndev;

	if (mutex_lock_interruptible(&ap_perms_mutex))
		return -ERESTARTSYS;

	/* try to find this zcdn device */
	zcdndev = find_zcdndev_by_name(name);
	if (!zcdndev) {
		rc = -ENOENT;
		goto unlockout;
	}

	/*
	 * The zcdn device is not hard destroyed. It is subject to
	 * reference counting and thus just needs to be unregistered.
	 */
	put_device(&zcdndev->device);
	device_unregister(&zcdndev->device);

unlockout:
	mutex_unlock(&ap_perms_mutex);
	return rc;
}

static void zcdn_destroy_all(void)
{
	int i;
	dev_t devt;
	struct zcdn_device *zcdndev;

	mutex_lock(&ap_perms_mutex);
	for (i = 0; i < ZCRYPT_MAX_MINOR_NODES; i++) {
		devt = MKDEV(MAJOR(zcrypt_devt), MINOR(zcrypt_devt) + i);
		zcdndev = find_zcdndev_by_devt(devt);
		if (zcdndev) {
			put_device(&zcdndev->device);
			device_unregister(&zcdndev->device);
		}
	}
	mutex_unlock(&ap_perms_mutex);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * zcrypt_read (): Not supported beyond zcrypt 1.3.1.
 *
 * This function is not supported beyond zcrypt 1.3.1.
 */
static ssize_t zcrypt_read(struct file *filp, char __user *buf,
			   size_t count, loff_t *f_pos)
{
	return -EPERM;
}

<<<<<<< HEAD
/**
 * zcrypt_write(): Not allowed.
 *
 * Write is is not allowed
=======
/*
 * zcrypt_write(): Not allowed.
 *
 * Write is not allowed
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static ssize_t zcrypt_write(struct file *filp, const char __user *buf,
			    size_t count, loff_t *f_pos)
{
	return -EPERM;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * zcrypt_open(): Count number of users.
 *
 * Device open function to count number of users.
 */
static int zcrypt_open(struct inode *inode, struct file *filp)
{
<<<<<<< HEAD
	atomic_inc(&zcrypt_open_count);
	return nonseekable_open(inode, filp);
}

/**
=======
	struct ap_perms *perms = &ap_perms;

	if (filp->f_inode->i_cdev == &zcrypt_cdev) {
		struct zcdn_device *zcdndev;

		if (mutex_lock_interruptible(&ap_perms_mutex))
			return -ERESTARTSYS;
		zcdndev = find_zcdndev_by_devt(filp->f_inode->i_rdev);
		/* find returns a reference, no get_device() needed */
		mutex_unlock(&ap_perms_mutex);
		if (zcdndev)
			perms = &zcdndev->perms;
	}
	filp->private_data = (void *)perms;

	atomic_inc(&zcrypt_open_count);
	return stream_open(inode, filp);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * zcrypt_release(): Count number of users.
 *
 * Device close function to count number of users.
 */
static int zcrypt_release(struct inode *inode, struct file *filp)
{
<<<<<<< HEAD
=======
	if (filp->f_inode->i_cdev == &zcrypt_cdev) {
		struct zcdn_device *zcdndev;

		mutex_lock(&ap_perms_mutex);
		zcdndev = find_zcdndev_by_devt(filp->f_inode->i_rdev);
		mutex_unlock(&ap_perms_mutex);
		if (zcdndev) {
			/* 2 puts here: one for find, one for open */
			put_device(&zcdndev->device);
			put_device(&zcdndev->device);
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_dec(&zcrypt_open_count);
	return 0;
}

<<<<<<< HEAD
/*
 * zcrypt ioctls.
 */
static long zcrypt_rsa_modexpo(struct ica_rsa_modexpo *mex)
{
	struct zcrypt_device *zdev;
	int rc;

	if (mex->outputdatalength < mex->inputdatalength)
		return -EINVAL;
=======
static inline int zcrypt_check_ioctl(struct ap_perms *perms,
				     unsigned int cmd)
{
	int rc = -EPERM;
	int ioctlnr = (cmd & _IOC_NRMASK) >> _IOC_NRSHIFT;

	if (ioctlnr > 0 && ioctlnr < AP_IOCTLS) {
		if (test_bit_inv(ioctlnr, perms->ioctlm))
			rc = 0;
	}

	if (rc)
		ZCRYPT_DBF_WARN("%s ioctl check failed: ioctlnr=0x%04x rc=%d\n",
				__func__, ioctlnr, rc);

	return rc;
}

static inline bool zcrypt_check_card(struct ap_perms *perms, int card)
{
	return test_bit_inv(card, perms->apm) ? true : false;
}

static inline bool zcrypt_check_queue(struct ap_perms *perms, int queue)
{
	return test_bit_inv(queue, perms->aqm) ? true : false;
}

static inline struct zcrypt_queue *zcrypt_pick_queue(struct zcrypt_card *zc,
						     struct zcrypt_queue *zq,
						     struct module **pmod,
						     unsigned int weight)
{
	if (!zq || !try_module_get(zq->queue->ap_dev.device.driver->owner))
		return NULL;
	zcrypt_card_get(zc);
	zcrypt_queue_get(zq);
	get_device(&zq->queue->ap_dev.device);
	atomic_add(weight, &zc->load);
	atomic_add(weight, &zq->load);
	zq->request_count++;
	*pmod = zq->queue->ap_dev.device.driver->owner;
	return zq;
}

static inline void zcrypt_drop_queue(struct zcrypt_card *zc,
				     struct zcrypt_queue *zq,
				     struct module *mod,
				     unsigned int weight)
{
	zq->request_count--;
	atomic_sub(weight, &zc->load);
	atomic_sub(weight, &zq->load);
	put_device(&zq->queue->ap_dev.device);
	zcrypt_queue_put(zq);
	zcrypt_card_put(zc);
	module_put(mod);
}

static inline bool zcrypt_card_compare(struct zcrypt_card *zc,
				       struct zcrypt_card *pref_zc,
				       unsigned int weight,
				       unsigned int pref_weight)
{
	if (!pref_zc)
		return true;
	weight += atomic_read(&zc->load);
	pref_weight += atomic_read(&pref_zc->load);
	if (weight == pref_weight)
		return atomic64_read(&zc->card->total_request_count) <
			atomic64_read(&pref_zc->card->total_request_count);
	return weight < pref_weight;
}

static inline bool zcrypt_queue_compare(struct zcrypt_queue *zq,
					struct zcrypt_queue *pref_zq,
					unsigned int weight,
					unsigned int pref_weight)
{
	if (!pref_zq)
		return true;
	weight += atomic_read(&zq->load);
	pref_weight += atomic_read(&pref_zq->load);
	if (weight == pref_weight)
		return zq->queue->total_request_count <
			pref_zq->queue->total_request_count;
	return weight < pref_weight;
}

/*
 * zcrypt ioctls.
 */
static long zcrypt_rsa_modexpo(struct ap_perms *perms,
			       struct zcrypt_track *tr,
			       struct ica_rsa_modexpo *mex)
{
	struct zcrypt_card *zc, *pref_zc;
	struct zcrypt_queue *zq, *pref_zq;
	struct ap_message ap_msg;
	unsigned int wgt = 0, pref_wgt = 0;
	unsigned int func_code;
	int cpen, qpen, qid = 0, rc = -ENODEV;
	struct module *mod;

	trace_s390_zcrypt_req(mex, TP_ICARSAMODEXPO);

	ap_init_message(&ap_msg);

	if (mex->outputdatalength < mex->inputdatalength) {
		func_code = 0;
		rc = -EINVAL;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * As long as outputdatalength is big enough, we can set the
	 * outputdatalength equal to the inputdatalength, since that is the
	 * number of bytes we will copy in any case
	 */
	mex->outputdatalength = mex->inputdatalength;

<<<<<<< HEAD
	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		if (!zdev->online ||
		    !zdev->ops->rsa_modexpo ||
		    zdev->min_mod_size > mex->inputdatalength ||
		    zdev->max_mod_size < mex->inputdatalength)
			continue;
		zcrypt_device_get(zdev);
		get_device(&zdev->ap_dev->device);
		zdev->request_count++;
		__zcrypt_decrease_preference(zdev);
		if (try_module_get(zdev->ap_dev->drv->driver.owner)) {
			spin_unlock_bh(&zcrypt_device_lock);
			rc = zdev->ops->rsa_modexpo(zdev, mex);
			spin_lock_bh(&zcrypt_device_lock);
			module_put(zdev->ap_dev->drv->driver.owner);
		}
		else
			rc = -EAGAIN;
		zdev->request_count--;
		__zcrypt_increase_preference(zdev);
		put_device(&zdev->ap_dev->device);
		zcrypt_device_put(zdev);
		spin_unlock_bh(&zcrypt_device_lock);
		return rc;
	}
	spin_unlock_bh(&zcrypt_device_lock);
	return -ENODEV;
}

static long zcrypt_rsa_crt(struct ica_rsa_modexpo_crt *crt)
{
	struct zcrypt_device *zdev;
	unsigned long long z1, z2, z3;
	int rc, copied;

	if (crt->outputdatalength < crt->inputdatalength ||
	    (crt->inputdatalength & 1))
		return -EINVAL;
=======
	rc = get_rsa_modex_fc(mex, &func_code);
	if (rc)
		goto out;

	pref_zc = NULL;
	pref_zq = NULL;
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		/* Check for usable accelerator or CCA card */
		if (!zc->online || !zc->card->config || zc->card->chkstop ||
		    !(zc->card->hwinfo.accel || zc->card->hwinfo.cca))
			continue;
		/* Check for size limits */
		if (zc->min_mod_size > mex->inputdatalength ||
		    zc->max_mod_size < mex->inputdatalength)
			continue;
		/* check if device node has admission for this card */
		if (!zcrypt_check_card(perms, zc->card->id))
			continue;
		/* get weight index of the card device	*/
		wgt = zc->speed_rating[func_code];
		/* penalty if this msg was previously sent via this card */
		cpen = (tr && tr->again_counter && tr->last_qid &&
			AP_QID_CARD(tr->last_qid) == zc->card->id) ?
			TRACK_AGAIN_CARD_WEIGHT_PENALTY : 0;
		if (!zcrypt_card_compare(zc, pref_zc, wgt + cpen, pref_wgt))
			continue;
		for_each_zcrypt_queue(zq, zc) {
			/* check if device is usable and eligible */
			if (!zq->online || !zq->ops->rsa_modexpo ||
			    !ap_queue_usable(zq->queue))
				continue;
			/* check if device node has admission for this queue */
			if (!zcrypt_check_queue(perms,
						AP_QID_QUEUE(zq->queue->qid)))
				continue;
			/* penalty if the msg was previously sent at this qid */
			qpen = (tr && tr->again_counter && tr->last_qid &&
				tr->last_qid == zq->queue->qid) ?
				TRACK_AGAIN_QUEUE_WEIGHT_PENALTY : 0;
			if (!zcrypt_queue_compare(zq, pref_zq,
						  wgt + cpen + qpen, pref_wgt))
				continue;
			pref_zc = zc;
			pref_zq = zq;
			pref_wgt = wgt + cpen + qpen;
		}
	}
	pref_zq = zcrypt_pick_queue(pref_zc, pref_zq, &mod, wgt);
	spin_unlock(&zcrypt_list_lock);

	if (!pref_zq) {
		pr_debug("%s no matching queue found => ENODEV\n", __func__);
		rc = -ENODEV;
		goto out;
	}

	qid = pref_zq->queue->qid;
	rc = pref_zq->ops->rsa_modexpo(pref_zq, mex, &ap_msg);

	spin_lock(&zcrypt_list_lock);
	zcrypt_drop_queue(pref_zc, pref_zq, mod, wgt);
	spin_unlock(&zcrypt_list_lock);

out:
	ap_release_message(&ap_msg);
	if (tr) {
		tr->last_rc = rc;
		tr->last_qid = qid;
	}
	trace_s390_zcrypt_rep(mex, func_code, rc,
			      AP_QID_CARD(qid), AP_QID_QUEUE(qid));
	return rc;
}

static long zcrypt_rsa_crt(struct ap_perms *perms,
			   struct zcrypt_track *tr,
			   struct ica_rsa_modexpo_crt *crt)
{
	struct zcrypt_card *zc, *pref_zc;
	struct zcrypt_queue *zq, *pref_zq;
	struct ap_message ap_msg;
	unsigned int wgt = 0, pref_wgt = 0;
	unsigned int func_code;
	int cpen, qpen, qid = 0, rc = -ENODEV;
	struct module *mod;

	trace_s390_zcrypt_req(crt, TP_ICARSACRT);

	ap_init_message(&ap_msg);

	if (crt->outputdatalength < crt->inputdatalength) {
		func_code = 0;
		rc = -EINVAL;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * As long as outputdatalength is big enough, we can set the
	 * outputdatalength equal to the inputdatalength, since that is the
	 * number of bytes we will copy in any case
	 */
	crt->outputdatalength = crt->inputdatalength;

<<<<<<< HEAD
	copied = 0;
 restart:
	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		if (!zdev->online ||
		    !zdev->ops->rsa_modexpo_crt ||
		    zdev->min_mod_size > crt->inputdatalength ||
		    zdev->max_mod_size < crt->inputdatalength)
			continue;
		if (zdev->short_crt && crt->inputdatalength > 240) {
			/*
			 * Check inputdata for leading zeros for cards
			 * that can't handle np_prime, bp_key, or
			 * u_mult_inv > 128 bytes.
			 */
			if (copied == 0) {
				unsigned int len;
				spin_unlock_bh(&zcrypt_device_lock);
				/* len is max 256 / 2 - 120 = 8
				 * For bigger device just assume len of leading
				 * 0s is 8 as stated in the requirements for
				 * ica_rsa_modexpo_crt struct in zcrypt.h.
				 */
				if (crt->inputdatalength <= 256)
					len = crt->inputdatalength / 2 - 120;
				else
					len = 8;
				if (len > sizeof(z1))
					return -EFAULT;
				z1 = z2 = z3 = 0;
				if (copy_from_user(&z1, crt->np_prime, len) ||
				    copy_from_user(&z2, crt->bp_key, len) ||
				    copy_from_user(&z3, crt->u_mult_inv, len))
					return -EFAULT;
				z1 = z2 = z3 = 0;
				copied = 1;
				/*
				 * We have to restart device lookup -
				 * the device list may have changed by now.
				 */
				goto restart;
			}
			if (z1 != 0ULL || z2 != 0ULL || z3 != 0ULL)
				/* The device can't handle this request. */
				continue;
		}
		zcrypt_device_get(zdev);
		get_device(&zdev->ap_dev->device);
		zdev->request_count++;
		__zcrypt_decrease_preference(zdev);
		if (try_module_get(zdev->ap_dev->drv->driver.owner)) {
			spin_unlock_bh(&zcrypt_device_lock);
			rc = zdev->ops->rsa_modexpo_crt(zdev, crt);
			spin_lock_bh(&zcrypt_device_lock);
			module_put(zdev->ap_dev->drv->driver.owner);
		}
		else
			rc = -EAGAIN;
		zdev->request_count--;
		__zcrypt_increase_preference(zdev);
		put_device(&zdev->ap_dev->device);
		zcrypt_device_put(zdev);
		spin_unlock_bh(&zcrypt_device_lock);
		return rc;
	}
	spin_unlock_bh(&zcrypt_device_lock);
	return -ENODEV;
}

static long zcrypt_send_cprb(struct ica_xcRB *xcRB)
{
	struct zcrypt_device *zdev;
	int rc;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		if (!zdev->online || !zdev->ops->send_cprb ||
		    (xcRB->user_defined != AUTOSELECT &&
			AP_QID_DEVICE(zdev->ap_dev->qid) != xcRB->user_defined)
		    )
			continue;
		zcrypt_device_get(zdev);
		get_device(&zdev->ap_dev->device);
		zdev->request_count++;
		__zcrypt_decrease_preference(zdev);
		if (try_module_get(zdev->ap_dev->drv->driver.owner)) {
			spin_unlock_bh(&zcrypt_device_lock);
			rc = zdev->ops->send_cprb(zdev, xcRB);
			spin_lock_bh(&zcrypt_device_lock);
			module_put(zdev->ap_dev->drv->driver.owner);
		}
		else
			rc = -EAGAIN;
		zdev->request_count--;
		__zcrypt_increase_preference(zdev);
		put_device(&zdev->ap_dev->device);
		zcrypt_device_put(zdev);
		spin_unlock_bh(&zcrypt_device_lock);
		return rc;
	}
	spin_unlock_bh(&zcrypt_device_lock);
	return -ENODEV;
}

static long zcrypt_rng(char *buffer)
{
	struct zcrypt_device *zdev;
	int rc;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		if (!zdev->online || !zdev->ops->rng)
			continue;
		zcrypt_device_get(zdev);
		get_device(&zdev->ap_dev->device);
		zdev->request_count++;
		__zcrypt_decrease_preference(zdev);
		if (try_module_get(zdev->ap_dev->drv->driver.owner)) {
			spin_unlock_bh(&zcrypt_device_lock);
			rc = zdev->ops->rng(zdev, buffer);
			spin_lock_bh(&zcrypt_device_lock);
			module_put(zdev->ap_dev->drv->driver.owner);
		} else
			rc = -EAGAIN;
		zdev->request_count--;
		__zcrypt_increase_preference(zdev);
		put_device(&zdev->ap_dev->device);
		zcrypt_device_put(zdev);
		spin_unlock_bh(&zcrypt_device_lock);
		return rc;
	}
	spin_unlock_bh(&zcrypt_device_lock);
	return -ENODEV;
}

static void zcrypt_status_mask(char status[AP_DEVICES])
{
	struct zcrypt_device *zdev;

	memset(status, 0, sizeof(char) * AP_DEVICES);
	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list)
		status[AP_QID_DEVICE(zdev->ap_dev->qid)] =
			zdev->online ? zdev->user_space_type : 0x0d;
	spin_unlock_bh(&zcrypt_device_lock);
}

static void zcrypt_qdepth_mask(char qdepth[AP_DEVICES])
{
	struct zcrypt_device *zdev;

	memset(qdepth, 0, sizeof(char)	* AP_DEVICES);
	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		spin_lock(&zdev->ap_dev->lock);
		qdepth[AP_QID_DEVICE(zdev->ap_dev->qid)] =
			zdev->ap_dev->pendingq_count +
			zdev->ap_dev->requestq_count;
		spin_unlock(&zdev->ap_dev->lock);
	}
	spin_unlock_bh(&zcrypt_device_lock);
}

static void zcrypt_perdev_reqcnt(int reqcnt[AP_DEVICES])
{
	struct zcrypt_device *zdev;

	memset(reqcnt, 0, sizeof(int) * AP_DEVICES);
	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		spin_lock(&zdev->ap_dev->lock);
		reqcnt[AP_QID_DEVICE(zdev->ap_dev->qid)] =
			zdev->ap_dev->total_request_count;
		spin_unlock(&zdev->ap_dev->lock);
	}
	spin_unlock_bh(&zcrypt_device_lock);
=======
	rc = get_rsa_crt_fc(crt, &func_code);
	if (rc)
		goto out;

	pref_zc = NULL;
	pref_zq = NULL;
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		/* Check for usable accelerator or CCA card */
		if (!zc->online || !zc->card->config || zc->card->chkstop ||
		    !(zc->card->hwinfo.accel || zc->card->hwinfo.cca))
			continue;
		/* Check for size limits */
		if (zc->min_mod_size > crt->inputdatalength ||
		    zc->max_mod_size < crt->inputdatalength)
			continue;
		/* check if device node has admission for this card */
		if (!zcrypt_check_card(perms, zc->card->id))
			continue;
		/* get weight index of the card device	*/
		wgt = zc->speed_rating[func_code];
		/* penalty if this msg was previously sent via this card */
		cpen = (tr && tr->again_counter && tr->last_qid &&
			AP_QID_CARD(tr->last_qid) == zc->card->id) ?
			TRACK_AGAIN_CARD_WEIGHT_PENALTY : 0;
		if (!zcrypt_card_compare(zc, pref_zc, wgt + cpen, pref_wgt))
			continue;
		for_each_zcrypt_queue(zq, zc) {
			/* check if device is usable and eligible */
			if (!zq->online || !zq->ops->rsa_modexpo_crt ||
			    !ap_queue_usable(zq->queue))
				continue;
			/* check if device node has admission for this queue */
			if (!zcrypt_check_queue(perms,
						AP_QID_QUEUE(zq->queue->qid)))
				continue;
			/* penalty if the msg was previously sent at this qid */
			qpen = (tr && tr->again_counter && tr->last_qid &&
				tr->last_qid == zq->queue->qid) ?
				TRACK_AGAIN_QUEUE_WEIGHT_PENALTY : 0;
			if (!zcrypt_queue_compare(zq, pref_zq,
						  wgt + cpen + qpen, pref_wgt))
				continue;
			pref_zc = zc;
			pref_zq = zq;
			pref_wgt = wgt + cpen + qpen;
		}
	}
	pref_zq = zcrypt_pick_queue(pref_zc, pref_zq, &mod, wgt);
	spin_unlock(&zcrypt_list_lock);

	if (!pref_zq) {
		pr_debug("%s no matching queue found => ENODEV\n", __func__);
		rc = -ENODEV;
		goto out;
	}

	qid = pref_zq->queue->qid;
	rc = pref_zq->ops->rsa_modexpo_crt(pref_zq, crt, &ap_msg);

	spin_lock(&zcrypt_list_lock);
	zcrypt_drop_queue(pref_zc, pref_zq, mod, wgt);
	spin_unlock(&zcrypt_list_lock);

out:
	ap_release_message(&ap_msg);
	if (tr) {
		tr->last_rc = rc;
		tr->last_qid = qid;
	}
	trace_s390_zcrypt_rep(crt, func_code, rc,
			      AP_QID_CARD(qid), AP_QID_QUEUE(qid));
	return rc;
}

static long _zcrypt_send_cprb(bool userspace, struct ap_perms *perms,
			      struct zcrypt_track *tr,
			      struct ica_xcRB *xcrb)
{
	struct zcrypt_card *zc, *pref_zc;
	struct zcrypt_queue *zq, *pref_zq;
	struct ap_message ap_msg;
	unsigned int wgt = 0, pref_wgt = 0;
	unsigned int func_code;
	unsigned short *domain, tdom;
	int cpen, qpen, qid = 0, rc = -ENODEV;
	struct module *mod;

	trace_s390_zcrypt_req(xcrb, TB_ZSECSENDCPRB);

	xcrb->status = 0;
	ap_init_message(&ap_msg);

	rc = prep_cca_ap_msg(userspace, xcrb, &ap_msg, &func_code, &domain);
	if (rc)
		goto out;
	print_hex_dump_debug("ccareq: ", DUMP_PREFIX_ADDRESS, 16, 1,
			     ap_msg.msg, ap_msg.len, false);

	tdom = *domain;
	if (perms != &ap_perms && tdom < AP_DOMAINS) {
		if (ap_msg.flags & AP_MSG_FLAG_ADMIN) {
			if (!test_bit_inv(tdom, perms->adm)) {
				rc = -ENODEV;
				goto out;
			}
		} else if ((ap_msg.flags & AP_MSG_FLAG_USAGE) == 0) {
			rc = -EOPNOTSUPP;
			goto out;
		}
	}
	/*
	 * If a valid target domain is set and this domain is NOT a usage
	 * domain but a control only domain, autoselect target domain.
	 */
	if (tdom < AP_DOMAINS &&
	    !ap_test_config_usage_domain(tdom) &&
	    ap_test_config_ctrl_domain(tdom))
		tdom = AUTOSEL_DOM;

	pref_zc = NULL;
	pref_zq = NULL;
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		/* Check for usable CCA card */
		if (!zc->online || !zc->card->config || zc->card->chkstop ||
		    !zc->card->hwinfo.cca)
			continue;
		/* Check for user selected CCA card */
		if (xcrb->user_defined != AUTOSELECT &&
		    xcrb->user_defined != zc->card->id)
			continue;
		/* check if request size exceeds card max msg size */
		if (ap_msg.len > zc->card->maxmsgsize)
			continue;
		/* check if device node has admission for this card */
		if (!zcrypt_check_card(perms, zc->card->id))
			continue;
		/* get weight index of the card device	*/
		wgt = speed_idx_cca(func_code) * zc->speed_rating[SECKEY];
		/* penalty if this msg was previously sent via this card */
		cpen = (tr && tr->again_counter && tr->last_qid &&
			AP_QID_CARD(tr->last_qid) == zc->card->id) ?
			TRACK_AGAIN_CARD_WEIGHT_PENALTY : 0;
		if (!zcrypt_card_compare(zc, pref_zc, wgt + cpen, pref_wgt))
			continue;
		for_each_zcrypt_queue(zq, zc) {
			/* check for device usable and eligible */
			if (!zq->online || !zq->ops->send_cprb ||
			    !ap_queue_usable(zq->queue) ||
			    (tdom != AUTOSEL_DOM &&
			     tdom != AP_QID_QUEUE(zq->queue->qid)))
				continue;
			/* check if device node has admission for this queue */
			if (!zcrypt_check_queue(perms,
						AP_QID_QUEUE(zq->queue->qid)))
				continue;
			/* penalty if the msg was previously sent at this qid */
			qpen = (tr && tr->again_counter && tr->last_qid &&
				tr->last_qid == zq->queue->qid) ?
				TRACK_AGAIN_QUEUE_WEIGHT_PENALTY : 0;
			if (!zcrypt_queue_compare(zq, pref_zq,
						  wgt + cpen + qpen, pref_wgt))
				continue;
			pref_zc = zc;
			pref_zq = zq;
			pref_wgt = wgt + cpen + qpen;
		}
	}
	pref_zq = zcrypt_pick_queue(pref_zc, pref_zq, &mod, wgt);
	spin_unlock(&zcrypt_list_lock);

	if (!pref_zq) {
		pr_debug("%s no match for address %02x.%04x => ENODEV\n",
			 __func__, xcrb->user_defined, *domain);
		rc = -ENODEV;
		goto out;
	}

	/* in case of auto select, provide the correct domain */
	qid = pref_zq->queue->qid;
	if (*domain == AUTOSEL_DOM)
		*domain = AP_QID_QUEUE(qid);

	rc = pref_zq->ops->send_cprb(userspace, pref_zq, xcrb, &ap_msg);
	if (!rc) {
		print_hex_dump_debug("ccarpl: ", DUMP_PREFIX_ADDRESS, 16, 1,
				     ap_msg.msg, ap_msg.len, false);
	}

	spin_lock(&zcrypt_list_lock);
	zcrypt_drop_queue(pref_zc, pref_zq, mod, wgt);
	spin_unlock(&zcrypt_list_lock);

out:
	ap_release_message(&ap_msg);
	if (tr) {
		tr->last_rc = rc;
		tr->last_qid = qid;
	}
	trace_s390_zcrypt_rep(xcrb, func_code, rc,
			      AP_QID_CARD(qid), AP_QID_QUEUE(qid));
	return rc;
}

long zcrypt_send_cprb(struct ica_xcRB *xcrb)
{
	struct zcrypt_track tr;
	int rc;

	memset(&tr, 0, sizeof(tr));

	do {
		rc = _zcrypt_send_cprb(false, &ap_perms, &tr, xcrb);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = _zcrypt_send_cprb(false, &ap_perms, &tr, xcrb);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		pr_debug("%s rc=%d\n", __func__, rc);

	return rc;
}
EXPORT_SYMBOL(zcrypt_send_cprb);

static bool is_desired_ep11_card(unsigned int dev_id,
				 unsigned short target_num,
				 struct ep11_target_dev *targets)
{
	while (target_num-- > 0) {
		if (targets->ap_id == dev_id || targets->ap_id == AUTOSEL_AP)
			return true;
		targets++;
	}
	return false;
}

static bool is_desired_ep11_queue(unsigned int dev_qid,
				  unsigned short target_num,
				  struct ep11_target_dev *targets)
{
	int card = AP_QID_CARD(dev_qid), dom = AP_QID_QUEUE(dev_qid);

	while (target_num-- > 0) {
		if ((targets->ap_id == card || targets->ap_id == AUTOSEL_AP) &&
		    (targets->dom_id == dom || targets->dom_id == AUTOSEL_DOM))
			return true;
		targets++;
	}
	return false;
}

static long _zcrypt_send_ep11_cprb(bool userspace, struct ap_perms *perms,
				   struct zcrypt_track *tr,
				   struct ep11_urb *xcrb)
{
	struct zcrypt_card *zc, *pref_zc;
	struct zcrypt_queue *zq, *pref_zq;
	struct ep11_target_dev *targets;
	unsigned short target_num;
	unsigned int wgt = 0, pref_wgt = 0;
	unsigned int func_code, domain;
	struct ap_message ap_msg;
	int cpen, qpen, qid = 0, rc = -ENODEV;
	struct module *mod;

	trace_s390_zcrypt_req(xcrb, TP_ZSENDEP11CPRB);

	ap_init_message(&ap_msg);

	target_num = (unsigned short)xcrb->targets_num;

	/* empty list indicates autoselect (all available targets) */
	targets = NULL;
	if (target_num != 0) {
		struct ep11_target_dev __user *uptr;

		targets = kcalloc(target_num, sizeof(*targets), GFP_KERNEL);
		if (!targets) {
			func_code = 0;
			rc = -ENOMEM;
			goto out;
		}

		uptr = (struct ep11_target_dev __force __user *)xcrb->targets;
		if (z_copy_from_user(userspace, targets, uptr,
				     target_num * sizeof(*targets))) {
			func_code = 0;
			rc = -EFAULT;
			goto out_free;
		}
	}

	rc = prep_ep11_ap_msg(userspace, xcrb, &ap_msg, &func_code, &domain);
	if (rc)
		goto out_free;
	print_hex_dump_debug("ep11req: ", DUMP_PREFIX_ADDRESS, 16, 1,
			     ap_msg.msg, ap_msg.len, false);

	if (perms != &ap_perms && domain < AUTOSEL_DOM) {
		if (ap_msg.flags & AP_MSG_FLAG_ADMIN) {
			if (!test_bit_inv(domain, perms->adm)) {
				rc = -ENODEV;
				goto out_free;
			}
		} else if ((ap_msg.flags & AP_MSG_FLAG_USAGE) == 0) {
			rc = -EOPNOTSUPP;
			goto out_free;
		}
	}

	pref_zc = NULL;
	pref_zq = NULL;
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		/* Check for usable EP11 card */
		if (!zc->online || !zc->card->config || zc->card->chkstop ||
		    !zc->card->hwinfo.ep11)
			continue;
		/* Check for user selected EP11 card */
		if (targets &&
		    !is_desired_ep11_card(zc->card->id, target_num, targets))
			continue;
		/* check if request size exceeds card max msg size */
		if (ap_msg.len > zc->card->maxmsgsize)
			continue;
		/* check if device node has admission for this card */
		if (!zcrypt_check_card(perms, zc->card->id))
			continue;
		/* get weight index of the card device	*/
		wgt = speed_idx_ep11(func_code) * zc->speed_rating[SECKEY];
		/* penalty if this msg was previously sent via this card */
		cpen = (tr && tr->again_counter && tr->last_qid &&
			AP_QID_CARD(tr->last_qid) == zc->card->id) ?
			TRACK_AGAIN_CARD_WEIGHT_PENALTY : 0;
		if (!zcrypt_card_compare(zc, pref_zc, wgt + cpen, pref_wgt))
			continue;
		for_each_zcrypt_queue(zq, zc) {
			/* check if device is usable and eligible */
			if (!zq->online || !zq->ops->send_ep11_cprb ||
			    !ap_queue_usable(zq->queue) ||
			    (targets &&
			     !is_desired_ep11_queue(zq->queue->qid,
						    target_num, targets)))
				continue;
			/* check if device node has admission for this queue */
			if (!zcrypt_check_queue(perms,
						AP_QID_QUEUE(zq->queue->qid)))
				continue;
			/* penalty if the msg was previously sent at this qid */
			qpen = (tr && tr->again_counter && tr->last_qid &&
				tr->last_qid == zq->queue->qid) ?
				TRACK_AGAIN_QUEUE_WEIGHT_PENALTY : 0;
			if (!zcrypt_queue_compare(zq, pref_zq,
						  wgt + cpen + qpen, pref_wgt))
				continue;
			pref_zc = zc;
			pref_zq = zq;
			pref_wgt = wgt + cpen + qpen;
		}
	}
	pref_zq = zcrypt_pick_queue(pref_zc, pref_zq, &mod, wgt);
	spin_unlock(&zcrypt_list_lock);

	if (!pref_zq) {
		if (targets && target_num == 1) {
			pr_debug("%s no match for address %02x.%04x => ENODEV\n",
				 __func__, (int)targets->ap_id,
				 (int)targets->dom_id);
		} else if (targets) {
			pr_debug("%s no match for %d target addrs => ENODEV\n",
				 __func__, (int)target_num);
		} else {
			pr_debug("%s no match for address ff.ffff => ENODEV\n",
				 __func__);
		}
		rc = -ENODEV;
		goto out_free;
	}

	qid = pref_zq->queue->qid;
	rc = pref_zq->ops->send_ep11_cprb(userspace, pref_zq, xcrb, &ap_msg);
	if (!rc) {
		print_hex_dump_debug("ep11rpl: ", DUMP_PREFIX_ADDRESS, 16, 1,
				     ap_msg.msg, ap_msg.len, false);
	}

	spin_lock(&zcrypt_list_lock);
	zcrypt_drop_queue(pref_zc, pref_zq, mod, wgt);
	spin_unlock(&zcrypt_list_lock);

out_free:
	kfree(targets);
out:
	ap_release_message(&ap_msg);
	if (tr) {
		tr->last_rc = rc;
		tr->last_qid = qid;
	}
	trace_s390_zcrypt_rep(xcrb, func_code, rc,
			      AP_QID_CARD(qid), AP_QID_QUEUE(qid));
	return rc;
}

long zcrypt_send_ep11_cprb(struct ep11_urb *xcrb)
{
	struct zcrypt_track tr;
	int rc;

	memset(&tr, 0, sizeof(tr));

	do {
		rc = _zcrypt_send_ep11_cprb(false, &ap_perms, &tr, xcrb);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = _zcrypt_send_ep11_cprb(false, &ap_perms, &tr, xcrb);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		pr_debug("%s rc=%d\n", __func__, rc);

	return rc;
}
EXPORT_SYMBOL(zcrypt_send_ep11_cprb);

static long zcrypt_rng(char *buffer)
{
	struct zcrypt_card *zc, *pref_zc;
	struct zcrypt_queue *zq, *pref_zq;
	unsigned int wgt = 0, pref_wgt = 0;
	unsigned int func_code;
	struct ap_message ap_msg;
	unsigned int domain;
	int qid = 0, rc = -ENODEV;
	struct module *mod;

	trace_s390_zcrypt_req(buffer, TP_HWRNGCPRB);

	ap_init_message(&ap_msg);
	rc = prep_rng_ap_msg(&ap_msg, &func_code, &domain);
	if (rc)
		goto out;

	pref_zc = NULL;
	pref_zq = NULL;
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		/* Check for usable CCA card */
		if (!zc->online || !zc->card->config || zc->card->chkstop ||
		    !zc->card->hwinfo.cca)
			continue;
		/* get weight index of the card device	*/
		wgt = zc->speed_rating[func_code];
		if (!zcrypt_card_compare(zc, pref_zc, wgt, pref_wgt))
			continue;
		for_each_zcrypt_queue(zq, zc) {
			/* check if device is usable and eligible */
			if (!zq->online || !zq->ops->rng ||
			    !ap_queue_usable(zq->queue))
				continue;
			if (!zcrypt_queue_compare(zq, pref_zq, wgt, pref_wgt))
				continue;
			pref_zc = zc;
			pref_zq = zq;
			pref_wgt = wgt;
		}
	}
	pref_zq = zcrypt_pick_queue(pref_zc, pref_zq, &mod, wgt);
	spin_unlock(&zcrypt_list_lock);

	if (!pref_zq) {
		pr_debug("%s no matching queue found => ENODEV\n", __func__);
		rc = -ENODEV;
		goto out;
	}

	qid = pref_zq->queue->qid;
	rc = pref_zq->ops->rng(pref_zq, buffer, &ap_msg);

	spin_lock(&zcrypt_list_lock);
	zcrypt_drop_queue(pref_zc, pref_zq, mod, wgt);
	spin_unlock(&zcrypt_list_lock);

out:
	ap_release_message(&ap_msg);
	trace_s390_zcrypt_rep(buffer, func_code, rc,
			      AP_QID_CARD(qid), AP_QID_QUEUE(qid));
	return rc;
}

static void zcrypt_device_status_mask(struct zcrypt_device_status *devstatus)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	struct zcrypt_device_status *stat;
	int card, queue;

	memset(devstatus, 0, MAX_ZDEV_ENTRIES
	       * sizeof(struct zcrypt_device_status));

	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			card = AP_QID_CARD(zq->queue->qid);
			if (card >= MAX_ZDEV_CARDIDS)
				continue;
			queue = AP_QID_QUEUE(zq->queue->qid);
			stat = &devstatus[card * AP_DOMAINS + queue];
			stat->hwtype = zc->card->ap_dev.device_type;
			stat->functions = zc->card->hwinfo.fac >> 26;
			stat->qid = zq->queue->qid;
			stat->online = zq->online ? 0x01 : 0x00;
		}
	}
	spin_unlock(&zcrypt_list_lock);
}

void zcrypt_device_status_mask_ext(struct zcrypt_device_status_ext *devstatus)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	struct zcrypt_device_status_ext *stat;
	int card, queue;

	memset(devstatus, 0, MAX_ZDEV_ENTRIES_EXT
	       * sizeof(struct zcrypt_device_status_ext));

	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			card = AP_QID_CARD(zq->queue->qid);
			queue = AP_QID_QUEUE(zq->queue->qid);
			stat = &devstatus[card * AP_DOMAINS + queue];
			stat->hwtype = zc->card->ap_dev.device_type;
			stat->functions = zc->card->hwinfo.fac >> 26;
			stat->qid = zq->queue->qid;
			stat->online = zq->online ? 0x01 : 0x00;
		}
	}
	spin_unlock(&zcrypt_list_lock);
}
EXPORT_SYMBOL(zcrypt_device_status_mask_ext);

int zcrypt_device_status_ext(int card, int queue,
			     struct zcrypt_device_status_ext *devstat)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;

	memset(devstat, 0, sizeof(*devstat));

	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			if (card == AP_QID_CARD(zq->queue->qid) &&
			    queue == AP_QID_QUEUE(zq->queue->qid)) {
				devstat->hwtype = zc->card->ap_dev.device_type;
				devstat->functions = zc->card->hwinfo.fac >> 26;
				devstat->qid = zq->queue->qid;
				devstat->online = zq->online ? 0x01 : 0x00;
				spin_unlock(&zcrypt_list_lock);
				return 0;
			}
		}
	}
	spin_unlock(&zcrypt_list_lock);

	return -ENODEV;
}
EXPORT_SYMBOL(zcrypt_device_status_ext);

static void zcrypt_status_mask(char status[], size_t max_adapters)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	int card;

	memset(status, 0, max_adapters);
	spin_lock(&zcrypt_list_lock);
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			card = AP_QID_CARD(zq->queue->qid);
			if (AP_QID_QUEUE(zq->queue->qid) != ap_domain_index ||
			    card >= max_adapters)
				continue;
			status[card] = zc->online ? zc->user_space_type : 0x0d;
		}
	}
	spin_unlock(&zcrypt_list_lock);
}

static void zcrypt_qdepth_mask(char qdepth[], size_t max_adapters)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	int card;

	memset(qdepth, 0, max_adapters);
	spin_lock(&zcrypt_list_lock);
	local_bh_disable();
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			card = AP_QID_CARD(zq->queue->qid);
			if (AP_QID_QUEUE(zq->queue->qid) != ap_domain_index ||
			    card >= max_adapters)
				continue;
			spin_lock(&zq->queue->lock);
			qdepth[card] =
				zq->queue->pendingq_count +
				zq->queue->requestq_count;
			spin_unlock(&zq->queue->lock);
		}
	}
	local_bh_enable();
	spin_unlock(&zcrypt_list_lock);
}

static void zcrypt_perdev_reqcnt(u32 reqcnt[], size_t max_adapters)
{
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	int card;
	u64 cnt;

	memset(reqcnt, 0, sizeof(int) * max_adapters);
	spin_lock(&zcrypt_list_lock);
	local_bh_disable();
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			card = AP_QID_CARD(zq->queue->qid);
			if (AP_QID_QUEUE(zq->queue->qid) != ap_domain_index ||
			    card >= max_adapters)
				continue;
			spin_lock(&zq->queue->lock);
			cnt = zq->queue->total_request_count;
			spin_unlock(&zq->queue->lock);
			reqcnt[card] = (cnt < UINT_MAX) ? (u32)cnt : UINT_MAX;
		}
	}
	local_bh_enable();
	spin_unlock(&zcrypt_list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int zcrypt_pendingq_count(void)
{
<<<<<<< HEAD
	struct zcrypt_device *zdev;
	int pendingq_count = 0;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		spin_lock(&zdev->ap_dev->lock);
		pendingq_count += zdev->ap_dev->pendingq_count;
		spin_unlock(&zdev->ap_dev->lock);
	}
	spin_unlock_bh(&zcrypt_device_lock);
=======
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	int pendingq_count;

	pendingq_count = 0;
	spin_lock(&zcrypt_list_lock);
	local_bh_disable();
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			if (AP_QID_QUEUE(zq->queue->qid) != ap_domain_index)
				continue;
			spin_lock(&zq->queue->lock);
			pendingq_count += zq->queue->pendingq_count;
			spin_unlock(&zq->queue->lock);
		}
	}
	local_bh_enable();
	spin_unlock(&zcrypt_list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pendingq_count;
}

static int zcrypt_requestq_count(void)
{
<<<<<<< HEAD
	struct zcrypt_device *zdev;
	int requestq_count = 0;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list) {
		spin_lock(&zdev->ap_dev->lock);
		requestq_count += zdev->ap_dev->requestq_count;
		spin_unlock(&zdev->ap_dev->lock);
	}
	spin_unlock_bh(&zcrypt_device_lock);
	return requestq_count;
}

static int zcrypt_count_type(int type)
{
	struct zcrypt_device *zdev;
	int device_count = 0;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list)
		if (zdev->user_space_type == type)
			device_count++;
	spin_unlock_bh(&zcrypt_device_lock);
	return device_count;
}

/**
 * zcrypt_ica_status(): Old, depracted combi status call.
 *
 * Old, deprecated combi status call.
 */
static long zcrypt_ica_status(struct file *filp, unsigned long arg)
{
	struct ica_z90_status *pstat;
	int ret;

	pstat = kzalloc(sizeof(*pstat), GFP_KERNEL);
	if (!pstat)
		return -ENOMEM;
	pstat->totalcount = zcrypt_device_count;
	pstat->leedslitecount = zcrypt_count_type(ZCRYPT_PCICA);
	pstat->leeds2count = zcrypt_count_type(ZCRYPT_PCICC);
	pstat->requestqWaitCount = zcrypt_requestq_count();
	pstat->pendingqWaitCount = zcrypt_pendingq_count();
	pstat->totalOpenCount = atomic_read(&zcrypt_open_count);
	pstat->cryptoDomain = ap_domain_index;
	zcrypt_status_mask(pstat->status);
	zcrypt_qdepth_mask(pstat->qdepth);
	ret = 0;
	if (copy_to_user((void __user *) arg, pstat, sizeof(*pstat)))
		ret = -EFAULT;
	kfree(pstat);
	return ret;
=======
	struct zcrypt_card *zc;
	struct zcrypt_queue *zq;
	int requestq_count;

	requestq_count = 0;
	spin_lock(&zcrypt_list_lock);
	local_bh_disable();
	for_each_zcrypt_card(zc) {
		for_each_zcrypt_queue(zq, zc) {
			if (AP_QID_QUEUE(zq->queue->qid) != ap_domain_index)
				continue;
			spin_lock(&zq->queue->lock);
			requestq_count += zq->queue->requestq_count;
			spin_unlock(&zq->queue->lock);
		}
	}
	local_bh_enable();
	spin_unlock(&zcrypt_list_lock);
	return requestq_count;
}

static int icarsamodexpo_ioctl(struct ap_perms *perms, unsigned long arg)
{
	int rc;
	struct zcrypt_track tr;
	struct ica_rsa_modexpo mex;
	struct ica_rsa_modexpo __user *umex = (void __user *)arg;

	memset(&tr, 0, sizeof(tr));
	if (copy_from_user(&mex, umex, sizeof(mex)))
		return -EFAULT;

	do {
		rc = zcrypt_rsa_modexpo(perms, &tr, &mex);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = zcrypt_rsa_modexpo(perms, &tr, &mex);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc) {
		pr_debug("ioctl ICARSAMODEXPO rc=%d\n", rc);
		return rc;
	}
	return put_user(mex.outputdatalength, &umex->outputdatalength);
}

static int icarsacrt_ioctl(struct ap_perms *perms, unsigned long arg)
{
	int rc;
	struct zcrypt_track tr;
	struct ica_rsa_modexpo_crt crt;
	struct ica_rsa_modexpo_crt __user *ucrt = (void __user *)arg;

	memset(&tr, 0, sizeof(tr));
	if (copy_from_user(&crt, ucrt, sizeof(crt)))
		return -EFAULT;

	do {
		rc = zcrypt_rsa_crt(perms, &tr, &crt);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = zcrypt_rsa_crt(perms, &tr, &crt);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc) {
		pr_debug("ioctl ICARSACRT rc=%d\n", rc);
		return rc;
	}
	return put_user(crt.outputdatalength, &ucrt->outputdatalength);
}

static int zsecsendcprb_ioctl(struct ap_perms *perms, unsigned long arg)
{
	int rc;
	struct ica_xcRB xcrb;
	struct zcrypt_track tr;
	struct ica_xcRB __user *uxcrb = (void __user *)arg;

	memset(&tr, 0, sizeof(tr));
	if (copy_from_user(&xcrb, uxcrb, sizeof(xcrb)))
		return -EFAULT;

	do {
		rc = _zcrypt_send_cprb(true, perms, &tr, &xcrb);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = _zcrypt_send_cprb(true, perms, &tr, &xcrb);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		pr_debug("ioctl ZSENDCPRB rc=%d status=0x%x\n",
			 rc, xcrb.status);
	if (copy_to_user(uxcrb, &xcrb, sizeof(xcrb)))
		return -EFAULT;
	return rc;
}

static int zsendep11cprb_ioctl(struct ap_perms *perms, unsigned long arg)
{
	int rc;
	struct ep11_urb xcrb;
	struct zcrypt_track tr;
	struct ep11_urb __user *uxcrb = (void __user *)arg;

	memset(&tr, 0, sizeof(tr));
	if (copy_from_user(&xcrb, uxcrb, sizeof(xcrb)))
		return -EFAULT;

	do {
		rc = _zcrypt_send_ep11_cprb(true, perms, &tr, &xcrb);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = _zcrypt_send_ep11_cprb(true, perms, &tr, &xcrb);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		pr_debug("ioctl ZSENDEP11CPRB rc=%d\n", rc);
	if (copy_to_user(uxcrb, &xcrb, sizeof(xcrb)))
		return -EFAULT;
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static long zcrypt_unlocked_ioctl(struct file *filp, unsigned int cmd,
				  unsigned long arg)
{
	int rc;
<<<<<<< HEAD

	switch (cmd) {
	case ICARSAMODEXPO: {
		struct ica_rsa_modexpo __user *umex = (void __user *) arg;
		struct ica_rsa_modexpo mex;
		if (copy_from_user(&mex, umex, sizeof(mex)))
			return -EFAULT;
		do {
			rc = zcrypt_rsa_modexpo(&mex);
		} while (rc == -EAGAIN);
		if (rc)
			return rc;
		return put_user(mex.outputdatalength, &umex->outputdatalength);
	}
	case ICARSACRT: {
		struct ica_rsa_modexpo_crt __user *ucrt = (void __user *) arg;
		struct ica_rsa_modexpo_crt crt;
		if (copy_from_user(&crt, ucrt, sizeof(crt)))
			return -EFAULT;
		do {
			rc = zcrypt_rsa_crt(&crt);
		} while (rc == -EAGAIN);
		if (rc)
			return rc;
		return put_user(crt.outputdatalength, &ucrt->outputdatalength);
	}
	case ZSECSENDCPRB: {
		struct ica_xcRB __user *uxcRB = (void __user *) arg;
		struct ica_xcRB xcRB;
		if (copy_from_user(&xcRB, uxcRB, sizeof(xcRB)))
			return -EFAULT;
		do {
			rc = zcrypt_send_cprb(&xcRB);
		} while (rc == -EAGAIN);
		if (copy_to_user(uxcRB, &xcRB, sizeof(xcRB)))
			return -EFAULT;
		return rc;
	}
	case Z90STAT_STATUS_MASK: {
		char status[AP_DEVICES];
		zcrypt_status_mask(status);
		if (copy_to_user((char __user *) arg, status,
				 sizeof(char) * AP_DEVICES))
=======
	struct ap_perms *perms =
		(struct ap_perms *)filp->private_data;

	rc = zcrypt_check_ioctl(perms, cmd);
	if (rc)
		return rc;

	switch (cmd) {
	case ICARSAMODEXPO:
		return icarsamodexpo_ioctl(perms, arg);
	case ICARSACRT:
		return icarsacrt_ioctl(perms, arg);
	case ZSECSENDCPRB:
		return zsecsendcprb_ioctl(perms, arg);
	case ZSENDEP11CPRB:
		return zsendep11cprb_ioctl(perms, arg);
	case ZCRYPT_DEVICE_STATUS: {
		struct zcrypt_device_status_ext *device_status;
		size_t total_size = MAX_ZDEV_ENTRIES_EXT
			* sizeof(struct zcrypt_device_status_ext);

		device_status = kvmalloc_array(MAX_ZDEV_ENTRIES_EXT,
					       sizeof(struct zcrypt_device_status_ext),
					       GFP_KERNEL);
		if (!device_status)
			return -ENOMEM;
		zcrypt_device_status_mask_ext(device_status);
		if (copy_to_user((char __user *)arg, device_status,
				 total_size))
			rc = -EFAULT;
		kvfree(device_status);
		return rc;
	}
	case ZCRYPT_STATUS_MASK: {
		char status[AP_DEVICES];

		zcrypt_status_mask(status, AP_DEVICES);
		if (copy_to_user((char __user *)arg, status, sizeof(status)))
			return -EFAULT;
		return 0;
	}
	case ZCRYPT_QDEPTH_MASK: {
		char qdepth[AP_DEVICES];

		zcrypt_qdepth_mask(qdepth, AP_DEVICES);
		if (copy_to_user((char __user *)arg, qdepth, sizeof(qdepth)))
			return -EFAULT;
		return 0;
	}
	case ZCRYPT_PERDEV_REQCNT: {
		u32 *reqcnt;

		reqcnt = kcalloc(AP_DEVICES, sizeof(u32), GFP_KERNEL);
		if (!reqcnt)
			return -ENOMEM;
		zcrypt_perdev_reqcnt(reqcnt, AP_DEVICES);
		if (copy_to_user((int __user *)arg, reqcnt,
				 sizeof(u32) * AP_DEVICES))
			rc = -EFAULT;
		kfree(reqcnt);
		return rc;
	}
	case Z90STAT_REQUESTQ_COUNT:
		return put_user(zcrypt_requestq_count(), (int __user *)arg);
	case Z90STAT_PENDINGQ_COUNT:
		return put_user(zcrypt_pendingq_count(), (int __user *)arg);
	case Z90STAT_TOTALOPEN_COUNT:
		return put_user(atomic_read(&zcrypt_open_count),
				(int __user *)arg);
	case Z90STAT_DOMAIN_INDEX:
		return put_user(ap_domain_index, (int __user *)arg);
	/*
	 * Deprecated ioctls
	 */
	case ZDEVICESTATUS: {
		/* the old ioctl supports only 64 adapters */
		struct zcrypt_device_status *device_status;
		size_t total_size = MAX_ZDEV_ENTRIES
			* sizeof(struct zcrypt_device_status);

		device_status = kzalloc(total_size, GFP_KERNEL);
		if (!device_status)
			return -ENOMEM;
		zcrypt_device_status_mask(device_status);
		if (copy_to_user((char __user *)arg, device_status,
				 total_size))
			rc = -EFAULT;
		kfree(device_status);
		return rc;
	}
	case Z90STAT_STATUS_MASK: {
		/* the old ioctl supports only 64 adapters */
		char status[MAX_ZDEV_CARDIDS];

		zcrypt_status_mask(status, MAX_ZDEV_CARDIDS);
		if (copy_to_user((char __user *)arg, status, sizeof(status)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		return 0;
	}
	case Z90STAT_QDEPTH_MASK: {
<<<<<<< HEAD
		char qdepth[AP_DEVICES];
		zcrypt_qdepth_mask(qdepth);
		if (copy_to_user((char __user *) arg, qdepth,
				 sizeof(char) * AP_DEVICES))
=======
		/* the old ioctl supports only 64 adapters */
		char qdepth[MAX_ZDEV_CARDIDS];

		zcrypt_qdepth_mask(qdepth, MAX_ZDEV_CARDIDS);
		if (copy_to_user((char __user *)arg, qdepth, sizeof(qdepth)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;
		return 0;
	}
	case Z90STAT_PERDEV_REQCNT: {
<<<<<<< HEAD
		int reqcnt[AP_DEVICES];
		zcrypt_perdev_reqcnt(reqcnt);
		if (copy_to_user((int __user *) arg, reqcnt,
				 sizeof(int) * AP_DEVICES))
			return -EFAULT;
		return 0;
	}
	case Z90STAT_REQUESTQ_COUNT:
		return put_user(zcrypt_requestq_count(), (int __user *) arg);
	case Z90STAT_PENDINGQ_COUNT:
		return put_user(zcrypt_pendingq_count(), (int __user *) arg);
	case Z90STAT_TOTALOPEN_COUNT:
		return put_user(atomic_read(&zcrypt_open_count),
				(int __user *) arg);
	case Z90STAT_DOMAIN_INDEX:
		return put_user(ap_domain_index, (int __user *) arg);
	/*
	 * Deprecated ioctls. Don't add another device count ioctl,
	 * you can count them yourself in the user space with the
	 * output of the Z90STAT_STATUS_MASK ioctl.
	 */
	case ICAZ90STATUS:
		return zcrypt_ica_status(filp, arg);
	case Z90STAT_TOTALCOUNT:
		return put_user(zcrypt_device_count, (int __user *) arg);
	case Z90STAT_PCICACOUNT:
		return put_user(zcrypt_count_type(ZCRYPT_PCICA),
				(int __user *) arg);
	case Z90STAT_PCICCCOUNT:
		return put_user(zcrypt_count_type(ZCRYPT_PCICC),
				(int __user *) arg);
	case Z90STAT_PCIXCCMCL2COUNT:
		return put_user(zcrypt_count_type(ZCRYPT_PCIXCC_MCL2),
				(int __user *) arg);
	case Z90STAT_PCIXCCMCL3COUNT:
		return put_user(zcrypt_count_type(ZCRYPT_PCIXCC_MCL3),
				(int __user *) arg);
	case Z90STAT_PCIXCCCOUNT:
		return put_user(zcrypt_count_type(ZCRYPT_PCIXCC_MCL2) +
				zcrypt_count_type(ZCRYPT_PCIXCC_MCL3),
				(int __user *) arg);
	case Z90STAT_CEX2CCOUNT:
		return put_user(zcrypt_count_type(ZCRYPT_CEX2C),
				(int __user *) arg);
	case Z90STAT_CEX2ACOUNT:
		return put_user(zcrypt_count_type(ZCRYPT_CEX2A),
				(int __user *) arg);
	default:
		/* unknown ioctl number */
=======
		/* the old ioctl supports only 64 adapters */
		u32 reqcnt[MAX_ZDEV_CARDIDS];

		zcrypt_perdev_reqcnt(reqcnt, MAX_ZDEV_CARDIDS);
		if (copy_to_user((int __user *)arg, reqcnt, sizeof(reqcnt)))
			return -EFAULT;
		return 0;
	}
	/* unknown ioctl number */
	default:
		pr_debug("unknown ioctl 0x%08x\n", cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOIOCTLCMD;
	}
}

#ifdef CONFIG_COMPAT
/*
 * ioctl32 conversion routines
 */
struct compat_ica_rsa_modexpo {
	compat_uptr_t	inputdata;
	unsigned int	inputdatalength;
	compat_uptr_t	outputdata;
	unsigned int	outputdatalength;
	compat_uptr_t	b_key;
	compat_uptr_t	n_modulus;
};

<<<<<<< HEAD
static long trans_modexpo32(struct file *filp, unsigned int cmd,
			    unsigned long arg)
=======
static long trans_modexpo32(struct ap_perms *perms, struct file *filp,
			    unsigned int cmd, unsigned long arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct compat_ica_rsa_modexpo __user *umex32 = compat_ptr(arg);
	struct compat_ica_rsa_modexpo mex32;
	struct ica_rsa_modexpo mex64;
<<<<<<< HEAD
	long rc;

=======
	struct zcrypt_track tr;
	long rc;

	memset(&tr, 0, sizeof(tr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_from_user(&mex32, umex32, sizeof(mex32)))
		return -EFAULT;
	mex64.inputdata = compat_ptr(mex32.inputdata);
	mex64.inputdatalength = mex32.inputdatalength;
	mex64.outputdata = compat_ptr(mex32.outputdata);
	mex64.outputdatalength = mex32.outputdatalength;
	mex64.b_key = compat_ptr(mex32.b_key);
	mex64.n_modulus = compat_ptr(mex32.n_modulus);
	do {
<<<<<<< HEAD
		rc = zcrypt_rsa_modexpo(&mex64);
	} while (rc == -EAGAIN);
	if (!rc)
		rc = put_user(mex64.outputdatalength,
			      &umex32->outputdatalength);
	return rc;
=======
		rc = zcrypt_rsa_modexpo(perms, &tr, &mex64);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = zcrypt_rsa_modexpo(perms, &tr, &mex64);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		return rc;
	return put_user(mex64.outputdatalength,
			&umex32->outputdatalength);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct compat_ica_rsa_modexpo_crt {
	compat_uptr_t	inputdata;
	unsigned int	inputdatalength;
	compat_uptr_t	outputdata;
	unsigned int	outputdatalength;
	compat_uptr_t	bp_key;
	compat_uptr_t	bq_key;
	compat_uptr_t	np_prime;
	compat_uptr_t	nq_prime;
	compat_uptr_t	u_mult_inv;
};

<<<<<<< HEAD
static long trans_modexpo_crt32(struct file *filp, unsigned int cmd,
				unsigned long arg)
=======
static long trans_modexpo_crt32(struct ap_perms *perms, struct file *filp,
				unsigned int cmd, unsigned long arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct compat_ica_rsa_modexpo_crt __user *ucrt32 = compat_ptr(arg);
	struct compat_ica_rsa_modexpo_crt crt32;
	struct ica_rsa_modexpo_crt crt64;
<<<<<<< HEAD
	long rc;

=======
	struct zcrypt_track tr;
	long rc;

	memset(&tr, 0, sizeof(tr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (copy_from_user(&crt32, ucrt32, sizeof(crt32)))
		return -EFAULT;
	crt64.inputdata = compat_ptr(crt32.inputdata);
	crt64.inputdatalength = crt32.inputdatalength;
<<<<<<< HEAD
	crt64.outputdata=  compat_ptr(crt32.outputdata);
=======
	crt64.outputdata = compat_ptr(crt32.outputdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	crt64.outputdatalength = crt32.outputdatalength;
	crt64.bp_key = compat_ptr(crt32.bp_key);
	crt64.bq_key = compat_ptr(crt32.bq_key);
	crt64.np_prime = compat_ptr(crt32.np_prime);
	crt64.nq_prime = compat_ptr(crt32.nq_prime);
	crt64.u_mult_inv = compat_ptr(crt32.u_mult_inv);
	do {
<<<<<<< HEAD
		rc = zcrypt_rsa_crt(&crt64);
	} while (rc == -EAGAIN);
	if (!rc)
		rc = put_user(crt64.outputdatalength,
			      &ucrt32->outputdatalength);
	return rc;
}

struct compat_ica_xcRB {
=======
		rc = zcrypt_rsa_crt(perms, &tr, &crt64);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = zcrypt_rsa_crt(perms, &tr, &crt64);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	if (rc)
		return rc;
	return put_user(crt64.outputdatalength,
			&ucrt32->outputdatalength);
}

struct compat_ica_xcrb {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned short	agent_ID;
	unsigned int	user_defined;
	unsigned short	request_ID;
	unsigned int	request_control_blk_length;
<<<<<<< HEAD
	unsigned char	padding1[16 - sizeof (compat_uptr_t)];
	compat_uptr_t	request_control_blk_addr;
	unsigned int	request_data_length;
	char		padding2[16 - sizeof (compat_uptr_t)];
	compat_uptr_t	request_data_address;
	unsigned int	reply_control_blk_length;
	char		padding3[16 - sizeof (compat_uptr_t)];
	compat_uptr_t	reply_control_blk_addr;
	unsigned int	reply_data_length;
	char		padding4[16 - sizeof (compat_uptr_t)];
	compat_uptr_t	reply_data_addr;
	unsigned short	priority_window;
	unsigned int	status;
} __attribute__((packed));

static long trans_xcRB32(struct file *filp, unsigned int cmd,
			 unsigned long arg)
{
	struct compat_ica_xcRB __user *uxcRB32 = compat_ptr(arg);
	struct compat_ica_xcRB xcRB32;
	struct ica_xcRB xcRB64;
	long rc;

	if (copy_from_user(&xcRB32, uxcRB32, sizeof(xcRB32)))
		return -EFAULT;
	xcRB64.agent_ID = xcRB32.agent_ID;
	xcRB64.user_defined = xcRB32.user_defined;
	xcRB64.request_ID = xcRB32.request_ID;
	xcRB64.request_control_blk_length =
		xcRB32.request_control_blk_length;
	xcRB64.request_control_blk_addr =
		compat_ptr(xcRB32.request_control_blk_addr);
	xcRB64.request_data_length =
		xcRB32.request_data_length;
	xcRB64.request_data_address =
		compat_ptr(xcRB32.request_data_address);
	xcRB64.reply_control_blk_length =
		xcRB32.reply_control_blk_length;
	xcRB64.reply_control_blk_addr =
		compat_ptr(xcRB32.reply_control_blk_addr);
	xcRB64.reply_data_length = xcRB32.reply_data_length;
	xcRB64.reply_data_addr =
		compat_ptr(xcRB32.reply_data_addr);
	xcRB64.priority_window = xcRB32.priority_window;
	xcRB64.status = xcRB32.status;
	do {
		rc = zcrypt_send_cprb(&xcRB64);
	} while (rc == -EAGAIN);
	xcRB32.reply_control_blk_length = xcRB64.reply_control_blk_length;
	xcRB32.reply_data_length = xcRB64.reply_data_length;
	xcRB32.status = xcRB64.status;
	if (copy_to_user(uxcRB32, &xcRB32, sizeof(xcRB32)))
			return -EFAULT;
=======
	unsigned char	padding1[16 - sizeof(compat_uptr_t)];
	compat_uptr_t	request_control_blk_addr;
	unsigned int	request_data_length;
	char		padding2[16 - sizeof(compat_uptr_t)];
	compat_uptr_t	request_data_address;
	unsigned int	reply_control_blk_length;
	char		padding3[16 - sizeof(compat_uptr_t)];
	compat_uptr_t	reply_control_blk_addr;
	unsigned int	reply_data_length;
	char		padding4[16 - sizeof(compat_uptr_t)];
	compat_uptr_t	reply_data_addr;
	unsigned short	priority_window;
	unsigned int	status;
} __packed;

static long trans_xcrb32(struct ap_perms *perms, struct file *filp,
			 unsigned int cmd, unsigned long arg)
{
	struct compat_ica_xcrb __user *uxcrb32 = compat_ptr(arg);
	struct compat_ica_xcrb xcrb32;
	struct zcrypt_track tr;
	struct ica_xcRB xcrb64;
	long rc;

	memset(&tr, 0, sizeof(tr));
	if (copy_from_user(&xcrb32, uxcrb32, sizeof(xcrb32)))
		return -EFAULT;
	xcrb64.agent_ID = xcrb32.agent_ID;
	xcrb64.user_defined = xcrb32.user_defined;
	xcrb64.request_ID = xcrb32.request_ID;
	xcrb64.request_control_blk_length =
		xcrb32.request_control_blk_length;
	xcrb64.request_control_blk_addr =
		compat_ptr(xcrb32.request_control_blk_addr);
	xcrb64.request_data_length =
		xcrb32.request_data_length;
	xcrb64.request_data_address =
		compat_ptr(xcrb32.request_data_address);
	xcrb64.reply_control_blk_length =
		xcrb32.reply_control_blk_length;
	xcrb64.reply_control_blk_addr =
		compat_ptr(xcrb32.reply_control_blk_addr);
	xcrb64.reply_data_length = xcrb32.reply_data_length;
	xcrb64.reply_data_addr =
		compat_ptr(xcrb32.reply_data_addr);
	xcrb64.priority_window = xcrb32.priority_window;
	xcrb64.status = xcrb32.status;
	do {
		rc = _zcrypt_send_cprb(true, perms, &tr, &xcrb64);
	} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);

	/* on ENODEV failure: retry once again after a requested rescan */
	if (rc == -ENODEV && zcrypt_process_rescan())
		do {
			rc = _zcrypt_send_cprb(true, perms, &tr, &xcrb64);
		} while (rc == -EAGAIN && ++tr.again_counter < TRACK_AGAIN_MAX);
	if (rc == -EAGAIN && tr.again_counter >= TRACK_AGAIN_MAX)
		rc = -EIO;
	xcrb32.reply_control_blk_length = xcrb64.reply_control_blk_length;
	xcrb32.reply_data_length = xcrb64.reply_data_length;
	xcrb32.status = xcrb64.status;
	if (copy_to_user(uxcrb32, &xcrb32, sizeof(xcrb32)))
		return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static long zcrypt_compat_ioctl(struct file *filp, unsigned int cmd,
<<<<<<< HEAD
			 unsigned long arg)
{
	if (cmd == ICARSAMODEXPO)
		return trans_modexpo32(filp, cmd, arg);
	if (cmd == ICARSACRT)
		return trans_modexpo_crt32(filp, cmd, arg);
	if (cmd == ZSECSENDCPRB)
		return trans_xcRB32(filp, cmd, arg);
=======
				unsigned long arg)
{
	int rc;
	struct ap_perms *perms =
		(struct ap_perms *)filp->private_data;

	rc = zcrypt_check_ioctl(perms, cmd);
	if (rc)
		return rc;

	if (cmd == ICARSAMODEXPO)
		return trans_modexpo32(perms, filp, cmd, arg);
	if (cmd == ICARSACRT)
		return trans_modexpo_crt32(perms, filp, cmd, arg);
	if (cmd == ZSECSENDCPRB)
		return trans_xcrb32(perms, filp, cmd, arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return zcrypt_unlocked_ioctl(filp, cmd, arg);
}
#endif

/*
 * Misc device file operations.
 */
static const struct file_operations zcrypt_fops = {
	.owner		= THIS_MODULE,
	.read		= zcrypt_read,
	.write		= zcrypt_write,
	.unlocked_ioctl	= zcrypt_unlocked_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= zcrypt_compat_ioctl,
#endif
	.open		= zcrypt_open,
	.release	= zcrypt_release,
	.llseek		= no_llseek,
};

/*
 * Misc device.
 */
static struct miscdevice zcrypt_misc_device = {
	.minor	    = MISC_DYNAMIC_MINOR,
	.name	    = "z90crypt",
	.fops	    = &zcrypt_fops,
};

<<<<<<< HEAD
/*
 * Deprecated /proc entry support.
 */
static struct proc_dir_entry *zcrypt_entry;

static void sprintcl(struct seq_file *m, unsigned char *addr, unsigned int len)
{
	int i;

	for (i = 0; i < len; i++)
		seq_printf(m, "%01x", (unsigned int) addr[i]);
	seq_putc(m, ' ');
}

static void sprintrw(struct seq_file *m, unsigned char *addr, unsigned int len)
{
	int inl, c, cx;

	seq_printf(m, "	   ");
	inl = 0;
	for (c = 0; c < (len / 16); c++) {
		sprintcl(m, addr+inl, 16);
		inl += 16;
	}
	cx = len%16;
	if (cx) {
		sprintcl(m, addr+inl, cx);
		inl += cx;
	}
	seq_putc(m, '\n');
}

static void sprinthx(unsigned char *title, struct seq_file *m,
		     unsigned char *addr, unsigned int len)
{
	int inl, r, rx;

	seq_printf(m, "\n%s\n", title);
	inl = 0;
	for (r = 0; r < (len / 64); r++) {
		sprintrw(m, addr+inl, 64);
		inl += 64;
	}
	rx = len % 64;
	if (rx) {
		sprintrw(m, addr+inl, rx);
		inl += rx;
	}
	seq_putc(m, '\n');
}

static void sprinthx4(unsigned char *title, struct seq_file *m,
		      unsigned int *array, unsigned int len)
{
	int r;

	seq_printf(m, "\n%s\n", title);
	for (r = 0; r < len; r++) {
		if ((r % 8) == 0)
			seq_printf(m, "    ");
		seq_printf(m, "%08X ", array[r]);
		if ((r % 8) == 7)
			seq_putc(m, '\n');
	}
	seq_putc(m, '\n');
}

static int zcrypt_proc_show(struct seq_file *m, void *v)
{
	char workarea[sizeof(int) * AP_DEVICES];

	seq_printf(m, "\nzcrypt version: %d.%d.%d\n",
		   ZCRYPT_VERSION, ZCRYPT_RELEASE, ZCRYPT_VARIANT);
	seq_printf(m, "Cryptographic domain: %d\n", ap_domain_index);
	seq_printf(m, "Total device count: %d\n", zcrypt_device_count);
	seq_printf(m, "PCICA count: %d\n", zcrypt_count_type(ZCRYPT_PCICA));
	seq_printf(m, "PCICC count: %d\n", zcrypt_count_type(ZCRYPT_PCICC));
	seq_printf(m, "PCIXCC MCL2 count: %d\n",
		   zcrypt_count_type(ZCRYPT_PCIXCC_MCL2));
	seq_printf(m, "PCIXCC MCL3 count: %d\n",
		   zcrypt_count_type(ZCRYPT_PCIXCC_MCL3));
	seq_printf(m, "CEX2C count: %d\n", zcrypt_count_type(ZCRYPT_CEX2C));
	seq_printf(m, "CEX2A count: %d\n", zcrypt_count_type(ZCRYPT_CEX2A));
	seq_printf(m, "CEX3C count: %d\n", zcrypt_count_type(ZCRYPT_CEX3C));
	seq_printf(m, "CEX3A count: %d\n", zcrypt_count_type(ZCRYPT_CEX3A));
	seq_printf(m, "requestq count: %d\n", zcrypt_requestq_count());
	seq_printf(m, "pendingq count: %d\n", zcrypt_pendingq_count());
	seq_printf(m, "Total open handles: %d\n\n",
		   atomic_read(&zcrypt_open_count));
	zcrypt_status_mask(workarea);
	sprinthx("Online devices: 1=PCICA 2=PCICC 3=PCIXCC(MCL2) "
		 "4=PCIXCC(MCL3) 5=CEX2C 6=CEX2A 7=CEX3C 8=CEX3A",
		 m, workarea, AP_DEVICES);
	zcrypt_qdepth_mask(workarea);
	sprinthx("Waiting work element counts", m, workarea, AP_DEVICES);
	zcrypt_perdev_reqcnt((int *) workarea);
	sprinthx4("Per-device successfully completed request counts",
		  m, (unsigned int *) workarea, AP_DEVICES);
	return 0;
}

static int zcrypt_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, zcrypt_proc_show, NULL);
}

static void zcrypt_disable_card(int index)
{
	struct zcrypt_device *zdev;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list)
		if (AP_QID_DEVICE(zdev->ap_dev->qid) == index) {
			zdev->online = 0;
			ap_flush_queue(zdev->ap_dev);
			break;
		}
	spin_unlock_bh(&zcrypt_device_lock);
}

static void zcrypt_enable_card(int index)
{
	struct zcrypt_device *zdev;

	spin_lock_bh(&zcrypt_device_lock);
	list_for_each_entry(zdev, &zcrypt_device_list, list)
		if (AP_QID_DEVICE(zdev->ap_dev->qid) == index) {
			zdev->online = 1;
			break;
		}
	spin_unlock_bh(&zcrypt_device_lock);
}

static ssize_t zcrypt_proc_write(struct file *file, const char __user *buffer,
				 size_t count, loff_t *pos)
{
	unsigned char *lbuf, *ptr;
	size_t local_count;
	int j;

	if (count <= 0)
		return 0;

#define LBUFSIZE 1200UL
	lbuf = kmalloc(LBUFSIZE, GFP_KERNEL);
	if (!lbuf)
		return 0;

	local_count = min(LBUFSIZE - 1, count);
	if (copy_from_user(lbuf, buffer, local_count) != 0) {
		kfree(lbuf);
		return -EFAULT;
	}
	lbuf[local_count] = '\0';

	ptr = strstr(lbuf, "Online devices");
	if (!ptr)
		goto out;
	ptr = strstr(ptr, "\n");
	if (!ptr)
		goto out;
	ptr++;

	if (strstr(ptr, "Waiting work element counts") == NULL)
		goto out;

	for (j = 0; j < 64 && *ptr; ptr++) {
		/*
		 * '0' for no device, '1' for PCICA, '2' for PCICC,
		 * '3' for PCIXCC_MCL2, '4' for PCIXCC_MCL3,
		 * '5' for CEX2C and '6' for CEX2A'
		 * '7' for CEX3C and '8' for CEX3A
		 */
		if (*ptr >= '0' && *ptr <= '8')
			j++;
		else if (*ptr == 'd' || *ptr == 'D')
			zcrypt_disable_card(j++);
		else if (*ptr == 'e' || *ptr == 'E')
			zcrypt_enable_card(j++);
		else if (*ptr != ' ' && *ptr != '\t')
			break;
	}
out:
	kfree(lbuf);
	return count;
}

static const struct file_operations zcrypt_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= zcrypt_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= zcrypt_proc_write,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int zcrypt_rng_device_count;
static u32 *zcrypt_rng_buffer;
static int zcrypt_rng_buffer_index;
static DEFINE_MUTEX(zcrypt_rng_mutex);

static int zcrypt_rng_data_read(struct hwrng *rng, u32 *data)
{
	int rc;

	/*
	 * We don't need locking here because the RNG API guarantees serialized
	 * read method calls.
	 */
	if (zcrypt_rng_buffer_index == 0) {
<<<<<<< HEAD
		rc = zcrypt_rng((char *) zcrypt_rng_buffer);
		if (rc < 0)
			return -EIO;
		zcrypt_rng_buffer_index = rc / sizeof *data;
	}
	*data = zcrypt_rng_buffer[--zcrypt_rng_buffer_index];
	return sizeof *data;
=======
		rc = zcrypt_rng((char *)zcrypt_rng_buffer);
		/* on ENODEV failure: retry once again after an AP bus rescan */
		if (rc == -ENODEV && zcrypt_process_rescan())
			rc = zcrypt_rng((char *)zcrypt_rng_buffer);
		if (rc < 0)
			return -EIO;
		zcrypt_rng_buffer_index = rc / sizeof(*data);
	}
	*data = zcrypt_rng_buffer[--zcrypt_rng_buffer_index];
	return sizeof(*data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct hwrng zcrypt_rng_dev = {
	.name		= "zcrypt",
	.data_read	= zcrypt_rng_data_read,
<<<<<<< HEAD
};

static int zcrypt_rng_device_add(void)
=======
	.quality	= 990,
};

int zcrypt_rng_device_add(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int rc = 0;

	mutex_lock(&zcrypt_rng_mutex);
	if (zcrypt_rng_device_count == 0) {
<<<<<<< HEAD
		zcrypt_rng_buffer = (u32 *) get_zeroed_page(GFP_KERNEL);
=======
		zcrypt_rng_buffer = (u32 *)get_zeroed_page(GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!zcrypt_rng_buffer) {
			rc = -ENOMEM;
			goto out;
		}
		zcrypt_rng_buffer_index = 0;
		rc = hwrng_register(&zcrypt_rng_dev);
		if (rc)
			goto out_free;
		zcrypt_rng_device_count = 1;
<<<<<<< HEAD
	} else
		zcrypt_rng_device_count++;
=======
	} else {
		zcrypt_rng_device_count++;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&zcrypt_rng_mutex);
	return 0;

out_free:
<<<<<<< HEAD
	free_page((unsigned long) zcrypt_rng_buffer);
=======
	free_page((unsigned long)zcrypt_rng_buffer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	mutex_unlock(&zcrypt_rng_mutex);
	return rc;
}

<<<<<<< HEAD
static void zcrypt_rng_device_remove(void)
=======
void zcrypt_rng_device_remove(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mutex_lock(&zcrypt_rng_mutex);
	zcrypt_rng_device_count--;
	if (zcrypt_rng_device_count == 0) {
		hwrng_unregister(&zcrypt_rng_dev);
<<<<<<< HEAD
		free_page((unsigned long) zcrypt_rng_buffer);
=======
		free_page((unsigned long)zcrypt_rng_buffer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&zcrypt_rng_mutex);
}

<<<<<<< HEAD
/**
=======
/*
 * Wait until the zcrypt api is operational.
 * The AP bus scan and the binding of ap devices to device drivers is
 * an asynchronous job. This function waits until these initial jobs
 * are done and so the zcrypt api should be ready to serve crypto
 * requests - if there are resources available. The function uses an
 * internal timeout of 30s. The very first caller will either wait for
 * ap bus bindings complete or the timeout happens. This state will be
 * remembered for further callers which will only be blocked until a
 * decision is made (timeout or bindings complete).
 * On timeout -ETIME is returned, on success the return value is 0.
 */
int zcrypt_wait_api_operational(void)
{
	static DEFINE_MUTEX(zcrypt_wait_api_lock);
	static int zcrypt_wait_api_state;
	int rc;

	rc = mutex_lock_interruptible(&zcrypt_wait_api_lock);
	if (rc)
		return rc;

	switch (zcrypt_wait_api_state) {
	case 0:
		/* initial state, invoke wait for the ap bus complete */
		rc = ap_wait_apqn_bindings_complete(
			msecs_to_jiffies(ZCRYPT_WAIT_BINDINGS_COMPLETE_MS));
		switch (rc) {
		case 0:
			/* ap bus bindings are complete */
			zcrypt_wait_api_state = 1;
			break;
		case -EINTR:
			/* interrupted, go back to caller */
			break;
		case -ETIME:
			/* timeout */
			ZCRYPT_DBF_WARN("%s ap_wait_init_apqn_bindings_complete()=ETIME\n",
					__func__);
			zcrypt_wait_api_state = -ETIME;
			break;
		default:
			/* other failure */
			pr_debug("%s ap_wait_init_apqn_bindings_complete()=%d\n",
				 __func__, rc);
			break;
		}
		break;
	case 1:
		/* a previous caller already found ap bus bindings complete */
		rc = 0;
		break;
	default:
		/* a previous caller had timeout or other failure */
		rc = zcrypt_wait_api_state;
		break;
	}

	mutex_unlock(&zcrypt_wait_api_lock);

	return rc;
}
EXPORT_SYMBOL(zcrypt_wait_api_operational);

int __init zcrypt_debug_init(void)
{
	zcrypt_dbf_info = debug_register("zcrypt", 2, 1,
					 ZCRYPT_DBF_MAX_SPRINTF_ARGS * sizeof(long));
	debug_register_view(zcrypt_dbf_info, &debug_sprintf_view);
	debug_set_level(zcrypt_dbf_info, DBF_ERR);

	return 0;
}

void zcrypt_debug_exit(void)
{
	debug_unregister(zcrypt_dbf_info);
}

static int __init zcdn_init(void)
{
	int rc;

	/* create a new class 'zcrypt' */
	rc = class_register(&zcrypt_class);
	if (rc)
		goto out_class_register_failed;

	/* alloc device minor range */
	rc = alloc_chrdev_region(&zcrypt_devt,
				 0, ZCRYPT_MAX_MINOR_NODES,
				 ZCRYPT_NAME);
	if (rc)
		goto out_alloc_chrdev_failed;

	cdev_init(&zcrypt_cdev, &zcrypt_fops);
	zcrypt_cdev.owner = THIS_MODULE;
	rc = cdev_add(&zcrypt_cdev, zcrypt_devt, ZCRYPT_MAX_MINOR_NODES);
	if (rc)
		goto out_cdev_add_failed;

	/* need some class specific sysfs attributes */
	rc = class_create_file(&zcrypt_class, &class_attr_zcdn_create);
	if (rc)
		goto out_class_create_file_1_failed;
	rc = class_create_file(&zcrypt_class, &class_attr_zcdn_destroy);
	if (rc)
		goto out_class_create_file_2_failed;

	return 0;

out_class_create_file_2_failed:
	class_remove_file(&zcrypt_class, &class_attr_zcdn_create);
out_class_create_file_1_failed:
	cdev_del(&zcrypt_cdev);
out_cdev_add_failed:
	unregister_chrdev_region(zcrypt_devt, ZCRYPT_MAX_MINOR_NODES);
out_alloc_chrdev_failed:
	class_unregister(&zcrypt_class);
out_class_register_failed:
	return rc;
}

static void zcdn_exit(void)
{
	class_remove_file(&zcrypt_class, &class_attr_zcdn_create);
	class_remove_file(&zcrypt_class, &class_attr_zcdn_destroy);
	zcdn_destroy_all();
	cdev_del(&zcrypt_cdev);
	unregister_chrdev_region(zcrypt_devt, ZCRYPT_MAX_MINOR_NODES);
	class_unregister(&zcrypt_class);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * zcrypt_api_init(): Module initialization.
 *
 * The module initialization code.
 */
int __init zcrypt_api_init(void)
{
	int rc;

<<<<<<< HEAD
	/* Register the request sprayer. */
	rc = misc_register(&zcrypt_misc_device);
	if (rc < 0)
		goto out;

	/* Set up the proc file system */
	zcrypt_entry = proc_create("driver/z90crypt", 0644, NULL, &zcrypt_proc_fops);
	if (!zcrypt_entry) {
		rc = -ENOMEM;
		goto out_misc;
	}

	return 0;

out_misc:
	misc_deregister(&zcrypt_misc_device);
=======
	rc = zcrypt_debug_init();
	if (rc)
		goto out;

	rc = zcdn_init();
	if (rc)
		goto out;

	/* Register the request sprayer. */
	rc = misc_register(&zcrypt_misc_device);
	if (rc < 0)
		goto out_misc_register_failed;

	zcrypt_msgtype6_init();
	zcrypt_msgtype50_init();

	return 0;

out_misc_register_failed:
	zcdn_exit();
	zcrypt_debug_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return rc;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * zcrypt_api_exit(): Module termination.
 *
 * The module termination code.
 */
<<<<<<< HEAD
void zcrypt_api_exit(void)
{
	remove_proc_entry("driver/z90crypt", NULL);
	misc_deregister(&zcrypt_misc_device);
=======
void __exit zcrypt_api_exit(void)
{
	zcdn_exit();
	misc_deregister(&zcrypt_misc_device);
	zcrypt_msgtype6_exit();
	zcrypt_msgtype50_exit();
	zcrypt_ccamisc_exit();
	zcrypt_ep11misc_exit();
	zcrypt_debug_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(zcrypt_api_init);
module_exit(zcrypt_api_exit);
