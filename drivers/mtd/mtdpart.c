<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Simple MTD partitioning layer
 *
 * Copyright © 2000 Nicolas Pitre <nico@fluxnic.net>
 * Copyright © 2002 Thomas Gleixner <gleixner@linutronix.de>
 * Copyright © 2000-2010 David Woodhouse <dwmw2@infradead.org>
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

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/kmod.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/err.h>
<<<<<<< HEAD

#include "mtdcore.h"

/* Our partition linked list */
static LIST_HEAD(mtd_partitions);
static DEFINE_MUTEX(mtd_partitions_mutex);

/* Our partition node structure */
struct mtd_part {
	struct mtd_info mtd;
	struct mtd_info *master;
	uint64_t offset;
	struct list_head list;
};

/*
 * Given a pointer to the MTD object in the mtd_part structure, we can retrieve
 * the pointer to that structure with this macro.
 */
#define PART(x)  ((struct mtd_part *)(x))


=======
#include <linux/of.h>
#include <linux/of_platform.h>

#include "mtdcore.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MTD methods which simply translate the effective address and pass through
 * to the _real_ device.
 */

<<<<<<< HEAD
static int part_read(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	struct mtd_ecc_stats stats;
	int res;

	stats = part->master->ecc_stats;
	res = part->master->_read(part->master, from + part->offset, len,
				  retlen, buf);
	if (unlikely(res)) {
		if (mtd_is_bitflip(res))
			mtd->ecc_stats.corrected += part->master->ecc_stats.corrected - stats.corrected;
		if (mtd_is_eccerr(res))
			mtd->ecc_stats.failed += part->master->ecc_stats.failed - stats.failed;
	}
	return res;
}

static int part_point(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, void **virt, resource_size_t *phys)
{
	struct mtd_part *part = PART(mtd);

	return part->master->_point(part->master, from + part->offset, len,
				    retlen, virt, phys);
}

static int part_unpoint(struct mtd_info *mtd, loff_t from, size_t len)
{
	struct mtd_part *part = PART(mtd);

	return part->master->_unpoint(part->master, from + part->offset, len);
}

static unsigned long part_get_unmapped_area(struct mtd_info *mtd,
					    unsigned long len,
					    unsigned long offset,
					    unsigned long flags)
{
	struct mtd_part *part = PART(mtd);

	offset += part->offset;
	return part->master->_get_unmapped_area(part->master, len, offset,
						flags);
}

static int part_read_oob(struct mtd_info *mtd, loff_t from,
		struct mtd_oob_ops *ops)
{
	struct mtd_part *part = PART(mtd);
	int res;

	if (from >= mtd->size)
		return -EINVAL;
	if (ops->datbuf && from + ops->len > mtd->size)
		return -EINVAL;

	/*
	 * If OOB is also requested, make sure that we do not read past the end
	 * of this partition.
	 */
	if (ops->oobbuf) {
		size_t len, pages;

		if (ops->mode == MTD_OPS_AUTO_OOB)
			len = mtd->oobavail;
		else
			len = mtd->oobsize;
		pages = mtd_div_by_ws(mtd->size, mtd);
		pages -= mtd_div_by_ws(from, mtd);
		if (ops->ooboffs + ops->ooblen > pages * len)
			return -EINVAL;
	}

	res = part->master->_read_oob(part->master, from + part->offset, ops);
	if (unlikely(res)) {
		if (mtd_is_bitflip(res))
			mtd->ecc_stats.corrected++;
		if (mtd_is_eccerr(res))
			mtd->ecc_stats.failed++;
	}
	return res;
}

static int part_read_user_prot_reg(struct mtd_info *mtd, loff_t from,
		size_t len, size_t *retlen, u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_read_user_prot_reg(part->master, from, len,
						 retlen, buf);
}

static int part_get_user_prot_info(struct mtd_info *mtd,
		struct otp_info *buf, size_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_get_user_prot_info(part->master, buf, len);
}

static int part_read_fact_prot_reg(struct mtd_info *mtd, loff_t from,
		size_t len, size_t *retlen, u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_read_fact_prot_reg(part->master, from, len,
						 retlen, buf);
}

static int part_get_fact_prot_info(struct mtd_info *mtd, struct otp_info *buf,
		size_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_get_fact_prot_info(part->master, buf, len);
}

static int part_write(struct mtd_info *mtd, loff_t to, size_t len,
		size_t *retlen, const u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_write(part->master, to + part->offset, len,
				    retlen, buf);
}

static int part_panic_write(struct mtd_info *mtd, loff_t to, size_t len,
		size_t *retlen, const u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_panic_write(part->master, to + part->offset, len,
					  retlen, buf);
}

static int part_write_oob(struct mtd_info *mtd, loff_t to,
		struct mtd_oob_ops *ops)
{
	struct mtd_part *part = PART(mtd);

	if (to >= mtd->size)
		return -EINVAL;
	if (ops->datbuf && to + ops->len > mtd->size)
		return -EINVAL;
	return part->master->_write_oob(part->master, to + part->offset, ops);
}

static int part_write_user_prot_reg(struct mtd_info *mtd, loff_t from,
		size_t len, size_t *retlen, u_char *buf)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_write_user_prot_reg(part->master, from, len,
						  retlen, buf);
}

static int part_lock_user_prot_reg(struct mtd_info *mtd, loff_t from,
		size_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_lock_user_prot_reg(part->master, from, len);
}

static int part_writev(struct mtd_info *mtd, const struct kvec *vecs,
		unsigned long count, loff_t to, size_t *retlen)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_writev(part->master, vecs, count,
				     to + part->offset, retlen);
}

static int part_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct mtd_part *part = PART(mtd);
	int ret;

	instr->addr += part->offset;
	ret = part->master->_erase(part->master, instr);
	if (ret) {
		if (instr->fail_addr != MTD_FAIL_ADDR_UNKNOWN)
			instr->fail_addr -= part->offset;
		instr->addr -= part->offset;
	}
	return ret;
}

void mtd_erase_callback(struct erase_info *instr)
{
	if (instr->mtd->_erase == part_erase) {
		struct mtd_part *part = PART(instr->mtd);

		if (instr->fail_addr != MTD_FAIL_ADDR_UNKNOWN)
			instr->fail_addr -= part->offset;
		instr->addr -= part->offset;
	}
	if (instr->callback)
		instr->callback(instr);
}
EXPORT_SYMBOL_GPL(mtd_erase_callback);

static int part_lock(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_lock(part->master, ofs + part->offset, len);
}

static int part_unlock(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_unlock(part->master, ofs + part->offset, len);
}

static int part_is_locked(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_is_locked(part->master, ofs + part->offset, len);
}

static void part_sync(struct mtd_info *mtd)
{
	struct mtd_part *part = PART(mtd);
	part->master->_sync(part->master);
}

static int part_suspend(struct mtd_info *mtd)
{
	struct mtd_part *part = PART(mtd);
	return part->master->_suspend(part->master);
}

static void part_resume(struct mtd_info *mtd)
{
	struct mtd_part *part = PART(mtd);
	part->master->_resume(part->master);
}

static int part_block_isbad(struct mtd_info *mtd, loff_t ofs)
{
	struct mtd_part *part = PART(mtd);
	ofs += part->offset;
	return part->master->_block_isbad(part->master, ofs);
}

static int part_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct mtd_part *part = PART(mtd);
	int res;

	ofs += part->offset;
	res = part->master->_block_markbad(part->master, ofs);
	if (!res)
		mtd->ecc_stats.badblocks++;
	return res;
}

static inline void free_partition(struct mtd_part *p)
{
	kfree(p->mtd.name);
	kfree(p);
}

void part_fill_badblockstats(struct mtd_info *mtd)
{
	struct mtd_part *part = PART(mtd);
	if (part->master->_block_isbad) {
		uint64_t offs = 0;
		mtd->ecc_stats.badblocks = 0;
		while (offs < mtd->size) {
			if (mtd_block_isbad(part->master,
						offs + part->offset))
				mtd->ecc_stats.badblocks++;
			offs += mtd->erasesize;
		}
	}
}

/*
 * This function unregisters and destroy all slave MTD objects which are
 * attached to the given master MTD object.
 */

int del_mtd_partitions(struct mtd_info *master)
{
	struct mtd_part *slave, *next;
	int ret, err = 0;

	mutex_lock(&mtd_partitions_mutex);
	list_for_each_entry_safe(slave, next, &mtd_partitions, list)
		if (slave->master == master) {
			ret = del_mtd_device(&slave->mtd);
			if (ret < 0) {
				err = ret;
				continue;
			}
			list_del(&slave->list);
			free_partition(slave);
		}
	mutex_unlock(&mtd_partitions_mutex);

	return err;
}

static struct mtd_part *allocate_partition(struct mtd_info *master,
			const struct mtd_partition *part, int partno,
			uint64_t cur_offset)
{
	struct mtd_part *slave;
	char *name;

	/* allocate the partition structure */
	slave = kzalloc(sizeof(*slave), GFP_KERNEL);
	name = kstrdup(part->name, GFP_KERNEL);
	if (!name || !slave) {
		printk(KERN_ERR"memory allocation error while creating partitions for \"%s\"\n",
		       master->name);
		kfree(name);
		kfree(slave);
=======
static inline void free_partition(struct mtd_info *mtd)
{
	kfree(mtd->name);
	kfree(mtd);
}

void release_mtd_partition(struct mtd_info *mtd)
{
	WARN_ON(!list_empty(&mtd->part.node));
	free_partition(mtd);
}

static struct mtd_info *allocate_partition(struct mtd_info *parent,
					   const struct mtd_partition *part,
					   int partno, uint64_t cur_offset)
{
	struct mtd_info *master = mtd_get_master(parent);
	int wr_alignment = (parent->flags & MTD_NO_ERASE) ?
			   master->writesize : master->erasesize;
	u64 parent_size = mtd_is_partition(parent) ?
			  parent->part.size : parent->size;
	struct mtd_info *child;
	u32 remainder;
	char *name;
	u64 tmp;

	/* allocate the partition structure */
	child = kzalloc(sizeof(*child), GFP_KERNEL);
	name = kstrdup(part->name, GFP_KERNEL);
	if (!name || !child) {
		printk(KERN_ERR"memory allocation error while creating partitions for \"%s\"\n",
		       parent->name);
		kfree(name);
		kfree(child);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ERR_PTR(-ENOMEM);
	}

	/* set up the MTD object for this partition */
<<<<<<< HEAD
	slave->mtd.type = master->type;
	slave->mtd.flags = master->flags & ~part->mask_flags;
	slave->mtd.size = part->size;
	slave->mtd.writesize = master->writesize;
	slave->mtd.writebufsize = master->writebufsize;
	slave->mtd.oobsize = master->oobsize;
	slave->mtd.oobavail = master->oobavail;
	slave->mtd.subpage_sft = master->subpage_sft;

	slave->mtd.name = name;
	slave->mtd.owner = master->owner;
	slave->mtd.backing_dev_info = master->backing_dev_info;

	/* NOTE:  we don't arrange MTDs as a tree; it'd be error-prone
	 * to have the same data be in two different partitions.
	 */
	slave->mtd.dev.parent = master->dev.parent;

	slave->mtd._read = part_read;
	slave->mtd._write = part_write;

	if (master->_panic_write)
		slave->mtd._panic_write = part_panic_write;

	if (master->_point && master->_unpoint) {
		slave->mtd._point = part_point;
		slave->mtd._unpoint = part_unpoint;
	}

	if (master->_get_unmapped_area)
		slave->mtd._get_unmapped_area = part_get_unmapped_area;
	if (master->_read_oob)
		slave->mtd._read_oob = part_read_oob;
	if (master->_write_oob)
		slave->mtd._write_oob = part_write_oob;
	if (master->_read_user_prot_reg)
		slave->mtd._read_user_prot_reg = part_read_user_prot_reg;
	if (master->_read_fact_prot_reg)
		slave->mtd._read_fact_prot_reg = part_read_fact_prot_reg;
	if (master->_write_user_prot_reg)
		slave->mtd._write_user_prot_reg = part_write_user_prot_reg;
	if (master->_lock_user_prot_reg)
		slave->mtd._lock_user_prot_reg = part_lock_user_prot_reg;
	if (master->_get_user_prot_info)
		slave->mtd._get_user_prot_info = part_get_user_prot_info;
	if (master->_get_fact_prot_info)
		slave->mtd._get_fact_prot_info = part_get_fact_prot_info;
	if (master->_sync)
		slave->mtd._sync = part_sync;
	if (!partno && !master->dev.class && master->_suspend &&
	    master->_resume) {
			slave->mtd._suspend = part_suspend;
			slave->mtd._resume = part_resume;
	}
	if (master->_writev)
		slave->mtd._writev = part_writev;
	if (master->_lock)
		slave->mtd._lock = part_lock;
	if (master->_unlock)
		slave->mtd._unlock = part_unlock;
	if (master->_is_locked)
		slave->mtd._is_locked = part_is_locked;
	if (master->_block_isbad)
		slave->mtd._block_isbad = part_block_isbad;
	if (master->_block_markbad)
		slave->mtd._block_markbad = part_block_markbad;
	slave->mtd._erase = part_erase;
	slave->master = master;
	slave->offset = part->offset;

	if (slave->offset == MTDPART_OFS_APPEND)
		slave->offset = cur_offset;
	if (slave->offset == MTDPART_OFS_NXTBLK) {
		slave->offset = cur_offset;
		if (mtd_mod_by_eb(cur_offset, master) != 0) {
			/* Round up to next erasesize */
			slave->offset = (mtd_div_by_eb(cur_offset, master) + 1) * master->erasesize;
			printk(KERN_NOTICE "Moving partition %d: "
			       "0x%012llx -> 0x%012llx\n", partno,
			       (unsigned long long)cur_offset, (unsigned long long)slave->offset);
		}
	}
	if (slave->offset == MTDPART_OFS_RETAIN) {
		slave->offset = cur_offset;
		if (master->size - slave->offset >= slave->mtd.size) {
			slave->mtd.size = master->size - slave->offset
							- slave->mtd.size;
		} else {
			printk(KERN_ERR "mtd partition \"%s\" doesn't have enough space: %#llx < %#llx, disabled\n",
				part->name, master->size - slave->offset,
				slave->mtd.size);
=======
	child->type = parent->type;
	child->part.flags = parent->flags & ~part->mask_flags;
	child->part.flags |= part->add_flags;
	child->flags = child->part.flags;
	child->part.size = part->size;
	child->writesize = parent->writesize;
	child->writebufsize = parent->writebufsize;
	child->oobsize = parent->oobsize;
	child->oobavail = parent->oobavail;
	child->subpage_sft = parent->subpage_sft;

	child->name = name;
	child->owner = parent->owner;

	/* NOTE: Historically, we didn't arrange MTDs as a tree out of
	 * concern for showing the same data in multiple partitions.
	 * However, it is very useful to have the master node present,
	 * so the MTD_PARTITIONED_MASTER option allows that. The master
	 * will have device nodes etc only if this is set, so make the
	 * parent conditional on that option. Note, this is a way to
	 * distinguish between the parent and its partitions in sysfs.
	 */
	child->dev.parent = IS_ENABLED(CONFIG_MTD_PARTITIONED_MASTER) || mtd_is_partition(parent) ?
			    &parent->dev : parent->dev.parent;
	child->dev.of_node = part->of_node;
	child->parent = parent;
	child->part.offset = part->offset;
	INIT_LIST_HEAD(&child->partitions);

	if (child->part.offset == MTDPART_OFS_APPEND)
		child->part.offset = cur_offset;
	if (child->part.offset == MTDPART_OFS_NXTBLK) {
		tmp = cur_offset;
		child->part.offset = cur_offset;
		remainder = do_div(tmp, wr_alignment);
		if (remainder) {
			child->part.offset += wr_alignment - remainder;
			printk(KERN_NOTICE "Moving partition %d: "
			       "0x%012llx -> 0x%012llx\n", partno,
			       (unsigned long long)cur_offset,
			       child->part.offset);
		}
	}
	if (child->part.offset == MTDPART_OFS_RETAIN) {
		child->part.offset = cur_offset;
		if (parent_size - child->part.offset >= child->part.size) {
			child->part.size = parent_size - child->part.offset -
					   child->part.size;
		} else {
			printk(KERN_ERR "mtd partition \"%s\" doesn't have enough space: %#llx < %#llx, disabled\n",
				part->name, parent_size - child->part.offset,
				child->part.size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* register to preserve ordering */
			goto out_register;
		}
	}
<<<<<<< HEAD
	if (slave->mtd.size == MTDPART_SIZ_FULL)
		slave->mtd.size = master->size - slave->offset;

	printk(KERN_NOTICE "0x%012llx-0x%012llx : \"%s\"\n", (unsigned long long)slave->offset,
		(unsigned long long)(slave->offset + slave->mtd.size), slave->mtd.name);

	/* let's do some sanity checks */
	if (slave->offset >= master->size) {
		/* let's register it anyway to preserve ordering */
		slave->offset = 0;
		slave->mtd.size = 0;
=======
	if (child->part.size == MTDPART_SIZ_FULL)
		child->part.size = parent_size - child->part.offset;

	printk(KERN_NOTICE "0x%012llx-0x%012llx : \"%s\"\n",
	       child->part.offset, child->part.offset + child->part.size,
	       child->name);

	/* let's do some sanity checks */
	if (child->part.offset >= parent_size) {
		/* let's register it anyway to preserve ordering */
		child->part.offset = 0;
		child->part.size = 0;

		/* Initialize ->erasesize to make add_mtd_device() happy. */
		child->erasesize = parent->erasesize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		printk(KERN_ERR"mtd: partition \"%s\" is out of reach -- disabled\n",
			part->name);
		goto out_register;
	}
<<<<<<< HEAD
	if (slave->offset + slave->mtd.size > master->size) {
		slave->mtd.size = master->size - slave->offset;
		printk(KERN_WARNING"mtd: partition \"%s\" extends beyond the end of device \"%s\" -- size truncated to %#llx\n",
			part->name, master->name, (unsigned long long)slave->mtd.size);
	}
	if (master->numeraseregions > 1) {
		/* Deal with variable erase size stuff */
		int i, max = master->numeraseregions;
		u64 end = slave->offset + slave->mtd.size;
		struct mtd_erase_region_info *regions = master->eraseregions;

		/* Find the first erase regions which is part of this
		 * partition. */
		for (i = 0; i < max && regions[i].offset <= slave->offset; i++)
=======
	if (child->part.offset + child->part.size > parent->size) {
		child->part.size = parent_size - child->part.offset;
		printk(KERN_WARNING"mtd: partition \"%s\" extends beyond the end of device \"%s\" -- size truncated to %#llx\n",
			part->name, parent->name, child->part.size);
	}

	if (parent->numeraseregions > 1) {
		/* Deal with variable erase size stuff */
		int i, max = parent->numeraseregions;
		u64 end = child->part.offset + child->part.size;
		struct mtd_erase_region_info *regions = parent->eraseregions;

		/* Find the first erase regions which is part of this
		 * partition. */
		for (i = 0; i < max && regions[i].offset <= child->part.offset;
		     i++)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			;
		/* The loop searched for the region _behind_ the first one */
		if (i > 0)
			i--;

		/* Pick biggest erasesize */
		for (; i < max && regions[i].offset < end; i++) {
<<<<<<< HEAD
			if (slave->mtd.erasesize < regions[i].erasesize) {
				slave->mtd.erasesize = regions[i].erasesize;
			}
		}
		BUG_ON(slave->mtd.erasesize == 0);
	} else {
		/* Single erase size */
		slave->mtd.erasesize = master->erasesize;
	}

	if ((slave->mtd.flags & MTD_WRITEABLE) &&
	    mtd_mod_by_eb(slave->offset, &slave->mtd)) {
		/* Doesn't start on a boundary of major erase size */
		/* FIXME: Let it be writable if it is on a boundary of
		 * _minor_ erase size though */
		slave->mtd.flags &= ~MTD_WRITEABLE;
		printk(KERN_WARNING"mtd: partition \"%s\" doesn't start on an erase block boundary -- force read-only\n",
			part->name);
	}
	if ((slave->mtd.flags & MTD_WRITEABLE) &&
	    mtd_mod_by_eb(slave->mtd.size, &slave->mtd)) {
		slave->mtd.flags &= ~MTD_WRITEABLE;
		printk(KERN_WARNING"mtd: partition \"%s\" doesn't end on an erase block -- force read-only\n",
			part->name);
	}

	slave->mtd.ecclayout = master->ecclayout;
	slave->mtd.ecc_strength = master->ecc_strength;

#ifndef CONFIG_MTD_LAZYECCSTATS
	part_fill_badblockstats(&(slave->mtd));
#endif

out_register:
	return slave;
}

int mtd_add_partition(struct mtd_info *master, char *name,
		      long long offset, long long length)
{
	struct mtd_partition part;
	struct mtd_part *p, *new;
	uint64_t start, end;
=======
			if (child->erasesize < regions[i].erasesize)
				child->erasesize = regions[i].erasesize;
		}
		BUG_ON(child->erasesize == 0);
	} else {
		/* Single erase size */
		child->erasesize = master->erasesize;
	}

	/*
	 * Child erasesize might differ from the parent one if the parent
	 * exposes several regions with different erasesize. Adjust
	 * wr_alignment accordingly.
	 */
	if (!(child->flags & MTD_NO_ERASE))
		wr_alignment = child->erasesize;

	tmp = mtd_get_master_ofs(child, 0);
	remainder = do_div(tmp, wr_alignment);
	if ((child->flags & MTD_WRITEABLE) && remainder) {
		/* Doesn't start on a boundary of major erase size */
		/* FIXME: Let it be writable if it is on a boundary of
		 * _minor_ erase size though */
		child->flags &= ~MTD_WRITEABLE;
		printk(KERN_WARNING"mtd: partition \"%s\" doesn't start on an erase/write block boundary -- force read-only\n",
			part->name);
	}

	tmp = mtd_get_master_ofs(child, 0) + child->part.size;
	remainder = do_div(tmp, wr_alignment);
	if ((child->flags & MTD_WRITEABLE) && remainder) {
		child->flags &= ~MTD_WRITEABLE;
		printk(KERN_WARNING"mtd: partition \"%s\" doesn't end on an erase/write block -- force read-only\n",
			part->name);
	}

	child->size = child->part.size;
	child->ecc_step_size = parent->ecc_step_size;
	child->ecc_strength = parent->ecc_strength;
	child->bitflip_threshold = parent->bitflip_threshold;

	if (master->_block_isbad) {
		uint64_t offs = 0;

		while (offs < child->part.size) {
			if (mtd_block_isreserved(child, offs))
				child->ecc_stats.bbtblocks++;
			else if (mtd_block_isbad(child, offs))
				child->ecc_stats.badblocks++;
			offs += child->erasesize;
		}
	}

out_register:
	return child;
}

static ssize_t offset_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct mtd_info *mtd = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%lld\n", mtd->part.offset);
}
static DEVICE_ATTR_RO(offset);	/* mtd partition offset */

static const struct attribute *mtd_partition_attrs[] = {
	&dev_attr_offset.attr,
	NULL
};

static int mtd_add_partition_attrs(struct mtd_info *new)
{
	int ret = sysfs_create_files(&new->dev.kobj, mtd_partition_attrs);
	if (ret)
		printk(KERN_WARNING
		       "mtd: failed to create partition attrs, err=%d\n", ret);
	return ret;
}

int mtd_add_partition(struct mtd_info *parent, const char *name,
		      long long offset, long long length)
{
	struct mtd_info *master = mtd_get_master(parent);
	u64 parent_size = mtd_is_partition(parent) ?
			  parent->part.size : parent->size;
	struct mtd_partition part;
	struct mtd_info *child;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret = 0;

	/* the direct offset is expected */
	if (offset == MTDPART_OFS_APPEND ||
	    offset == MTDPART_OFS_NXTBLK)
		return -EINVAL;

	if (length == MTDPART_SIZ_FULL)
<<<<<<< HEAD
		length = master->size - offset;
=======
		length = parent_size - offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (length <= 0)
		return -EINVAL;

<<<<<<< HEAD
	part.name = name;
	part.size = length;
	part.offset = offset;
	part.mask_flags = 0;
	part.ecclayout = NULL;

	new = allocate_partition(master, &part, -1, offset);
	if (IS_ERR(new))
		return PTR_ERR(new);

	start = offset;
	end = offset + length;

	mutex_lock(&mtd_partitions_mutex);
	list_for_each_entry(p, &mtd_partitions, list)
		if (p->master == master) {
			if ((start >= p->offset) &&
			    (start < (p->offset + p->mtd.size)))
				goto err_inv;

			if ((end >= p->offset) &&
			    (end < (p->offset + p->mtd.size)))
				goto err_inv;
		}

	list_add(&new->list, &mtd_partitions);
	mutex_unlock(&mtd_partitions_mutex);

	add_mtd_device(&new->mtd);

	return ret;
err_inv:
	mutex_unlock(&mtd_partitions_mutex);
	free_partition(new);
	return -EINVAL;
}
EXPORT_SYMBOL_GPL(mtd_add_partition);

int mtd_del_partition(struct mtd_info *master, int partno)
{
	struct mtd_part *slave, *next;
	int ret = -EINVAL;

	mutex_lock(&mtd_partitions_mutex);
	list_for_each_entry_safe(slave, next, &mtd_partitions, list)
		if ((slave->master == master) &&
		    (slave->mtd.index == partno)) {
			ret = del_mtd_device(&slave->mtd);
			if (ret < 0)
				break;

			list_del(&slave->list);
			free_partition(slave);
			break;
		}
	mutex_unlock(&mtd_partitions_mutex);
=======
	memset(&part, 0, sizeof(part));
	part.name = name;
	part.size = length;
	part.offset = offset;

	child = allocate_partition(parent, &part, -1, offset);
	if (IS_ERR(child))
		return PTR_ERR(child);

	mutex_lock(&master->master.partitions_lock);
	list_add_tail(&child->part.node, &parent->partitions);
	mutex_unlock(&master->master.partitions_lock);

	ret = add_mtd_device(child);
	if (ret)
		goto err_remove_part;

	mtd_add_partition_attrs(child);

	return 0;

err_remove_part:
	mutex_lock(&master->master.partitions_lock);
	list_del(&child->part.node);
	mutex_unlock(&master->master.partitions_lock);

	free_partition(child);

	return ret;
}
EXPORT_SYMBOL_GPL(mtd_add_partition);

/**
 * __mtd_del_partition - delete MTD partition
 *
 * @mtd: MTD structure to be deleted
 *
 * This function must be called with the partitions mutex locked.
 */
static int __mtd_del_partition(struct mtd_info *mtd)
{
	struct mtd_info *child, *next;
	int err;

	list_for_each_entry_safe(child, next, &mtd->partitions, part.node) {
		err = __mtd_del_partition(child);
		if (err)
			return err;
	}

	sysfs_remove_files(&mtd->dev.kobj, mtd_partition_attrs);

	list_del_init(&mtd->part.node);
	err = del_mtd_device(mtd);
	if (err)
		return err;

	return 0;
}

/*
 * This function unregisters and destroy all slave MTD objects which are
 * attached to the given MTD object, recursively.
 */
static int __del_mtd_partitions(struct mtd_info *mtd)
{
	struct mtd_info *child, *next;
	int ret, err = 0;

	list_for_each_entry_safe(child, next, &mtd->partitions, part.node) {
		if (mtd_has_partitions(child))
			__del_mtd_partitions(child);

		pr_info("Deleting %s MTD partition\n", child->name);
		list_del_init(&child->part.node);
		ret = del_mtd_device(child);
		if (ret < 0) {
			pr_err("Error when deleting partition \"%s\" (%d)\n",
			       child->name, ret);
			err = ret;
			continue;
		}
	}

	return err;
}

int del_mtd_partitions(struct mtd_info *mtd)
{
	struct mtd_info *master = mtd_get_master(mtd);
	int ret;

	pr_info("Deleting MTD partitions on \"%s\":\n", mtd->name);

	mutex_lock(&master->master.partitions_lock);
	ret = __del_mtd_partitions(mtd);
	mutex_unlock(&master->master.partitions_lock);

	return ret;
}

int mtd_del_partition(struct mtd_info *mtd, int partno)
{
	struct mtd_info *child, *master = mtd_get_master(mtd);
	int ret = -EINVAL;

	mutex_lock(&master->master.partitions_lock);
	list_for_each_entry(child, &mtd->partitions, part.node) {
		if (child->index == partno) {
			ret = __mtd_del_partition(child);
			break;
		}
	}
	mutex_unlock(&master->master.partitions_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}
EXPORT_SYMBOL_GPL(mtd_del_partition);

/*
<<<<<<< HEAD
 * This function, given a master MTD object and a partition table, creates
 * and registers slave MTD objects which are bound to the master according to
 * the partition definitions.
 *
 * We don't register the master, or expect the caller to have done so,
 * for reasons of data integrity.
 */

int add_mtd_partitions(struct mtd_info *master,
		       const struct mtd_partition *parts,
		       int nbparts)
{
	struct mtd_part *slave;
	uint64_t cur_offset = 0;
	int i;

	printk(KERN_NOTICE "Creating %d MTD partitions on \"%s\":\n", nbparts, master->name);

	for (i = 0; i < nbparts; i++) {
		slave = allocate_partition(master, parts + i, i, cur_offset);
		if (IS_ERR(slave)) {
			del_mtd_partitions(master);
			return PTR_ERR(slave);
		}

		mutex_lock(&mtd_partitions_mutex);
		list_add(&slave->list, &mtd_partitions);
		mutex_unlock(&mtd_partitions_mutex);

		add_mtd_device(&slave->mtd);

		cur_offset = slave->offset + slave->mtd.size;
	}

	return 0;
=======
 * This function, given a parent MTD object and a partition table, creates
 * and registers the child MTD objects which are bound to the parent according
 * to the partition definitions.
 *
 * For historical reasons, this function's caller only registers the parent
 * if the MTD_PARTITIONED_MASTER config option is set.
 */

int add_mtd_partitions(struct mtd_info *parent,
		       const struct mtd_partition *parts,
		       int nbparts)
{
	struct mtd_info *child, *master = mtd_get_master(parent);
	uint64_t cur_offset = 0;
	int i, ret;

	printk(KERN_NOTICE "Creating %d MTD partitions on \"%s\":\n",
	       nbparts, parent->name);

	for (i = 0; i < nbparts; i++) {
		child = allocate_partition(parent, parts + i, i, cur_offset);
		if (IS_ERR(child)) {
			ret = PTR_ERR(child);
			goto err_del_partitions;
		}

		mutex_lock(&master->master.partitions_lock);
		list_add_tail(&child->part.node, &parent->partitions);
		mutex_unlock(&master->master.partitions_lock);

		ret = add_mtd_device(child);
		if (ret) {
			mutex_lock(&master->master.partitions_lock);
			list_del(&child->part.node);
			mutex_unlock(&master->master.partitions_lock);

			free_partition(child);
			goto err_del_partitions;
		}

		mtd_add_partition_attrs(child);

		/* Look for subpartitions */
		ret = parse_mtd_partitions(child, parts[i].types, NULL);
		if (ret < 0) {
			pr_err("Failed to parse subpartitions: %d\n", ret);
			goto err_del_partitions;
		}

		cur_offset = child->part.offset + child->part.size;
	}

	return 0;

err_del_partitions:
	del_mtd_partitions(master);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEFINE_SPINLOCK(part_parser_lock);
static LIST_HEAD(part_parsers);

<<<<<<< HEAD
static struct mtd_part_parser *get_partition_parser(const char *name)
=======
static struct mtd_part_parser *mtd_part_parser_get(const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mtd_part_parser *p, *ret = NULL;

	spin_lock(&part_parser_lock);

	list_for_each_entry(p, &part_parsers, list)
		if (!strcmp(p->name, name) && try_module_get(p->owner)) {
			ret = p;
			break;
		}

	spin_unlock(&part_parser_lock);

	return ret;
}

<<<<<<< HEAD
#define put_partition_parser(p) do { module_put((p)->owner); } while (0)

int register_mtd_parser(struct mtd_part_parser *p)
{
=======
static inline void mtd_part_parser_put(const struct mtd_part_parser *p)
{
	module_put(p->owner);
}

/*
 * Many partition parsers just expected the core to kfree() all their data in
 * one chunk. Do that by default.
 */
static void mtd_part_parser_cleanup_default(const struct mtd_partition *pparts,
					    int nr_parts)
{
	kfree(pparts);
}

int __register_mtd_parser(struct mtd_part_parser *p, struct module *owner)
{
	p->owner = owner;

	if (!p->cleanup)
		p->cleanup = &mtd_part_parser_cleanup_default;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&part_parser_lock);
	list_add(&p->list, &part_parsers);
	spin_unlock(&part_parser_lock);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(register_mtd_parser);

int deregister_mtd_parser(struct mtd_part_parser *p)
=======
EXPORT_SYMBOL_GPL(__register_mtd_parser);

void deregister_mtd_parser(struct mtd_part_parser *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	spin_lock(&part_parser_lock);
	list_del(&p->list);
	spin_unlock(&part_parser_lock);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(deregister_mtd_parser);

/*
 * Do not forget to update 'parse_mtd_partitions()' kerneldoc comment if you
 * are changing this array!
 */
<<<<<<< HEAD
static const char *default_mtd_part_types[] = {
=======
static const char * const default_mtd_part_types[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"cmdlinepart",
	"ofpart",
	NULL
};

<<<<<<< HEAD
/**
 * parse_mtd_partitions - parse MTD partitions
 * @master: the master partition (describes whole MTD device)
 * @types: names of partition parsers to try or %NULL
 * @pparts: array of partitions found is returned here
 * @data: MTD partition parser-specific data
 *
 * This function tries to find partition on MTD device @master. It uses MTD
 * partition parsers, specified in @types. However, if @types is %NULL, then
 * the default list of parsers is used. The default list contains only the
=======
/* Check DT only when looking for subpartitions. */
static const char * const default_subpartition_types[] = {
	"ofpart",
	NULL
};

static int mtd_part_do_parse(struct mtd_part_parser *parser,
			     struct mtd_info *master,
			     struct mtd_partitions *pparts,
			     struct mtd_part_parser_data *data)
{
	int ret;

	ret = (*parser->parse_fn)(master, &pparts->parts, data);
	pr_debug("%s: parser %s: %i\n", master->name, parser->name, ret);
	if (ret <= 0)
		return ret;

	pr_notice("%d %s partitions found on MTD device %s\n", ret,
		  parser->name, master->name);

	pparts->nr_parts = ret;
	pparts->parser = parser;

	return ret;
}

/**
 * mtd_part_get_compatible_parser - find MTD parser by a compatible string
 *
 * @compat: compatible string describing partitions in a device tree
 *
 * MTD parsers can specify supported partitions by providing a table of
 * compatibility strings. This function finds a parser that advertises support
 * for a passed value of "compatible".
 */
static struct mtd_part_parser *mtd_part_get_compatible_parser(const char *compat)
{
	struct mtd_part_parser *p, *ret = NULL;

	spin_lock(&part_parser_lock);

	list_for_each_entry(p, &part_parsers, list) {
		const struct of_device_id *matches;

		matches = p->of_match_table;
		if (!matches)
			continue;

		for (; matches->compatible[0]; matches++) {
			if (!strcmp(matches->compatible, compat) &&
			    try_module_get(p->owner)) {
				ret = p;
				break;
			}
		}

		if (ret)
			break;
	}

	spin_unlock(&part_parser_lock);

	return ret;
}

static int mtd_part_of_parse(struct mtd_info *master,
			     struct mtd_partitions *pparts)
{
	struct mtd_part_parser *parser;
	struct device_node *np;
	struct device_node *child;
	struct property *prop;
	struct device *dev;
	const char *compat;
	const char *fixed = "fixed-partitions";
	int ret, err = 0;

	dev = &master->dev;
	/* Use parent device (controller) if the top level MTD is not registered */
	if (!IS_ENABLED(CONFIG_MTD_PARTITIONED_MASTER) && !mtd_is_partition(master))
		dev = master->dev.parent;

	np = mtd_get_of_node(master);
	if (mtd_is_partition(master))
		of_node_get(np);
	else
		np = of_get_child_by_name(np, "partitions");

	/*
	 * Don't create devices that are added to a bus but will never get
	 * probed. That'll cause fw_devlink to block probing of consumers of
	 * this partition until the partition device is probed.
	 */
	for_each_child_of_node(np, child)
		if (of_device_is_compatible(child, "nvmem-cells"))
			of_node_set_flag(child, OF_POPULATED);

	of_property_for_each_string(np, "compatible", prop, compat) {
		parser = mtd_part_get_compatible_parser(compat);
		if (!parser)
			continue;
		ret = mtd_part_do_parse(parser, master, pparts, NULL);
		if (ret > 0) {
			of_platform_populate(np, NULL, NULL, dev);
			of_node_put(np);
			return ret;
		}
		mtd_part_parser_put(parser);
		if (ret < 0 && !err)
			err = ret;
	}
	of_platform_populate(np, NULL, NULL, dev);
	of_node_put(np);

	/*
	 * For backward compatibility we have to try the "fixed-partitions"
	 * parser. It supports old DT format with partitions specified as a
	 * direct subnodes of a flash device DT node without any compatibility
	 * specified we could match.
	 */
	parser = mtd_part_parser_get(fixed);
	if (!parser && !request_module("%s", fixed))
		parser = mtd_part_parser_get(fixed);
	if (parser) {
		ret = mtd_part_do_parse(parser, master, pparts, NULL);
		if (ret > 0)
			return ret;
		mtd_part_parser_put(parser);
		if (ret < 0 && !err)
			err = ret;
	}

	return err;
}

/**
 * parse_mtd_partitions - parse and register MTD partitions
 *
 * @master: the master partition (describes whole MTD device)
 * @types: names of partition parsers to try or %NULL
 * @data: MTD partition parser-specific data
 *
 * This function tries to find & register partitions on MTD device @master. It
 * uses MTD partition parsers, specified in @types. However, if @types is %NULL,
 * then the default list of parsers is used. The default list contains only the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * "cmdlinepart" and "ofpart" parsers ATM.
 * Note: If there are more then one parser in @types, the kernel only takes the
 * partitions parsed out by the first parser.
 *
 * This function may return:
 * o a negative error code in case of failure
<<<<<<< HEAD
 * o zero if no partitions were found
 * o a positive number of found partitions, in which case on exit @pparts will
 *   point to an array containing this number of &struct mtd_info objects.
 */
int parse_mtd_partitions(struct mtd_info *master, const char **types,
			 struct mtd_partition **pparts,
			 struct mtd_part_parser_data *data)
{
	struct mtd_part_parser *parser;
	int ret = 0;

	if (!types)
		types = default_mtd_part_types;

	for ( ; ret <= 0 && *types; types++) {
		parser = get_partition_parser(*types);
		if (!parser && !request_module("%s", *types))
			parser = get_partition_parser(*types);
		if (!parser)
			continue;
		ret = (*parser->parse_fn)(master, pparts, data);
		put_partition_parser(parser);
		if (ret > 0) {
			printk(KERN_NOTICE "%d %s partitions found on MTD device %s\n",
			       ret, parser->name, master->name);
			break;
		}
	}
	return ret;
}

int mtd_is_partition(struct mtd_info *mtd)
{
	struct mtd_part *part;
	int ispart = 0;

	mutex_lock(&mtd_partitions_mutex);
	list_for_each_entry(part, &mtd_partitions, list)
		if (&part->mtd == mtd) {
			ispart = 1;
			break;
		}
	mutex_unlock(&mtd_partitions_mutex);

	return ispart;
}
EXPORT_SYMBOL_GPL(mtd_is_partition);
=======
 * o number of found partitions otherwise
 */
int parse_mtd_partitions(struct mtd_info *master, const char *const *types,
			 struct mtd_part_parser_data *data)
{
	struct mtd_partitions pparts = { };
	struct mtd_part_parser *parser;
	int ret, err = 0;

	if (!types)
		types = mtd_is_partition(master) ? default_subpartition_types :
			default_mtd_part_types;

	for ( ; *types; types++) {
		/*
		 * ofpart is a special type that means OF partitioning info
		 * should be used. It requires a bit different logic so it is
		 * handled in a separated function.
		 */
		if (!strcmp(*types, "ofpart")) {
			ret = mtd_part_of_parse(master, &pparts);
		} else {
			pr_debug("%s: parsing partitions %s\n", master->name,
				 *types);
			parser = mtd_part_parser_get(*types);
			if (!parser && !request_module("%s", *types))
				parser = mtd_part_parser_get(*types);
			pr_debug("%s: got parser %s\n", master->name,
				parser ? parser->name : NULL);
			if (!parser)
				continue;
			ret = mtd_part_do_parse(parser, master, &pparts, data);
			if (ret <= 0)
				mtd_part_parser_put(parser);
		}
		/* Found partitions! */
		if (ret > 0) {
			err = add_mtd_partitions(master, pparts.parts,
						 pparts.nr_parts);
			mtd_part_parser_cleanup(&pparts);
			return err ? err : pparts.nr_parts;
		}
		/*
		 * Stash the first error we see; only report it if no parser
		 * succeeds
		 */
		if (ret < 0 && !err)
			err = ret;
	}
	return err;
}

void mtd_part_parser_cleanup(struct mtd_partitions *parts)
{
	const struct mtd_part_parser *parser;

	if (!parts)
		return;

	parser = parts->parser;
	if (parser) {
		if (parser->cleanup)
			parser->cleanup(parts->parts, parts->nr_parts);

		mtd_part_parser_put(parser);
	}
}

/* Returns the size of the entire flash chip */
uint64_t mtd_get_device_size(const struct mtd_info *mtd)
{
	struct mtd_info *master = mtd_get_master((struct mtd_info *)mtd);

	return master->size;
}
EXPORT_SYMBOL_GPL(mtd_get_device_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
