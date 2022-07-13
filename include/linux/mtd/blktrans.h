<<<<<<< HEAD
/*
 * Copyright © 2003-2010 David Woodhouse <dwmw2@infradead.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright © 2003-2010 David Woodhouse <dwmw2@infradead.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __MTD_TRANS_H__
#define __MTD_TRANS_H__

#include <linux/mutex.h>
#include <linux/kref.h>
#include <linux/sysfs.h>

struct hd_geometry;
struct mtd_info;
struct mtd_blktrans_ops;
struct file;
struct inode;

struct mtd_blktrans_dev {
	struct mtd_blktrans_ops *tr;
	struct list_head list;
	struct mtd_info *mtd;
	struct mutex lock;
	int devnum;
	bool bg_stop;
	unsigned long size;
	int readonly;
	int open;
	struct kref ref;
	struct gendisk *disk;
	struct attribute_group *disk_attributes;
<<<<<<< HEAD
	struct task_struct *thread;
	struct request_queue *rq;
	spinlock_t queue_lock;
	void *priv;
	fmode_t file_mode;
=======
	struct request_queue *rq;
	struct list_head rq_list;
	struct blk_mq_tag_set *tag_set;
	spinlock_t queue_lock;
	void *priv;
	bool writable;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct mtd_blktrans_ops {
	char *name;
	int major;
	int part_bits;
	int blksize;
	int blkshift;

	/* Access functions */
	int (*readsect)(struct mtd_blktrans_dev *dev,
		    unsigned long block, char *buffer);
	int (*writesect)(struct mtd_blktrans_dev *dev,
		     unsigned long block, char *buffer);
	int (*discard)(struct mtd_blktrans_dev *dev,
		       unsigned long block, unsigned nr_blocks);
	void (*background)(struct mtd_blktrans_dev *dev);

	/* Block layer ioctls */
	int (*getgeo)(struct mtd_blktrans_dev *dev, struct hd_geometry *geo);
	int (*flush)(struct mtd_blktrans_dev *dev);

	/* Called with mtd_table_mutex held; no race with add/remove */
	int (*open)(struct mtd_blktrans_dev *dev);
<<<<<<< HEAD
	int (*release)(struct mtd_blktrans_dev *dev);
=======
	void (*release)(struct mtd_blktrans_dev *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Called on {de,}registration and on subsequent addition/removal
	   of devices, with mtd_table_mutex held. */
	void (*add_mtd)(struct mtd_blktrans_ops *tr, struct mtd_info *mtd);
	void (*remove_dev)(struct mtd_blktrans_dev *dev);

	struct list_head devs;
	struct list_head list;
	struct module *owner;
};

extern int register_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int deregister_mtd_blktrans(struct mtd_blktrans_ops *tr);
extern int add_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);
extern int del_mtd_blktrans_dev(struct mtd_blktrans_dev *dev);
extern int mtd_blktrans_cease_background(struct mtd_blktrans_dev *dev);

<<<<<<< HEAD
=======
/**
 * module_mtd_blktrans() - Helper macro for registering a mtd blktrans driver
 * @__mtd_blktrans: mtd_blktrans_ops struct
 *
 * Helper macro for mtd blktrans drivers which do not do anything special in
 * module init/exit. This eliminates a lot of boilerplate. Each module may only
 * use this macro once, and calling it replaces module_init() and module_exit()
 */
#define module_mtd_blktrans(__mtd_blktrans) \
	module_driver(__mtd_blktrans, register_mtd_blktrans, \
					deregister_mtd_blktrans)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __MTD_TRANS_H__ */
