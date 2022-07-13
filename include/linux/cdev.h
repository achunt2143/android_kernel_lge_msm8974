<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_CDEV_H
#define _LINUX_CDEV_H

#include <linux/kobject.h>
#include <linux/kdev_t.h>
#include <linux/list.h>
<<<<<<< HEAD
=======
#include <linux/device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct file_operations;
struct inode;
struct module;

struct cdev {
	struct kobject kobj;
	struct module *owner;
	const struct file_operations *ops;
	struct list_head list;
	dev_t dev;
	unsigned int count;
<<<<<<< HEAD
};
=======
} __randomize_layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void cdev_init(struct cdev *, const struct file_operations *);

struct cdev *cdev_alloc(void);

void cdev_put(struct cdev *p);

int cdev_add(struct cdev *, dev_t, unsigned);

<<<<<<< HEAD
=======
void cdev_set_parent(struct cdev *p, struct kobject *kobj);
int cdev_device_add(struct cdev *cdev, struct device *dev);
void cdev_device_del(struct cdev *cdev, struct device *dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void cdev_del(struct cdev *);

void cd_forget(struct inode *);

<<<<<<< HEAD
extern struct backing_dev_info directly_mappable_cdev_bdi;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
