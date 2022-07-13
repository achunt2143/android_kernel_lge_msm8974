<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Simple read-only (writable only for RAM) mtdblock driver
 *
 * Copyright © 2001-2010 David Woodhouse <dwmw2@infradead.org>
<<<<<<< HEAD
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/blktrans.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/major.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int mtdblock_readsect(struct mtd_blktrans_dev *dev,
			      unsigned long block, char *buf)
{
	size_t retlen;
<<<<<<< HEAD

	if (mtd_read(dev->mtd, (block * 512), 512, &retlen, buf))
=======
	int err;

	err = mtd_read(dev->mtd, (block * 512), 512, &retlen, buf);
	if (err && !mtd_is_bitflip(err))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	return 0;
}

static int mtdblock_writesect(struct mtd_blktrans_dev *dev,
			      unsigned long block, char *buf)
{
	size_t retlen;

	if (mtd_write(dev->mtd, (block * 512), 512, &retlen, buf))
		return 1;
	return 0;
}

static void mtdblock_add_mtd(struct mtd_blktrans_ops *tr, struct mtd_info *mtd)
{
	struct mtd_blktrans_dev *dev = kzalloc(sizeof(*dev), GFP_KERNEL);

	if (!dev)
		return;

	dev->mtd = mtd;
	dev->devnum = mtd->index;

	dev->size = mtd->size >> 9;
	dev->tr = tr;
	dev->readonly = 1;

<<<<<<< HEAD
=======
	if (mtd_type_is_nand(mtd))
		pr_warn_ratelimited("%s: MTD device '%s' is NAND, please consider using UBI block devices instead.\n",
			tr->name, mtd->name);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (add_mtd_blktrans_dev(dev))
		kfree(dev);
}

static void mtdblock_remove_dev(struct mtd_blktrans_dev *dev)
{
	del_mtd_blktrans_dev(dev);
}

static struct mtd_blktrans_ops mtdblock_tr = {
	.name		= "mtdblock",
<<<<<<< HEAD
	.major		= 31,
=======
	.major		= MTD_BLOCK_MAJOR,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.part_bits	= 0,
	.blksize 	= 512,
	.readsect	= mtdblock_readsect,
	.writesect	= mtdblock_writesect,
	.add_mtd	= mtdblock_add_mtd,
	.remove_dev	= mtdblock_remove_dev,
	.owner		= THIS_MODULE,
};

<<<<<<< HEAD
static int __init mtdblock_init(void)
{
	return register_mtd_blktrans(&mtdblock_tr);
}

static void __exit mtdblock_exit(void)
{
	deregister_mtd_blktrans(&mtdblock_tr);
}

module_init(mtdblock_init);
module_exit(mtdblock_exit);
=======
module_mtd_blktrans(mtdblock_tr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("Simple read-only block device emulation access to MTD devices");
