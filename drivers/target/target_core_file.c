<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
 * Filename:  target_core_file.c
 *
 * This file contains the Storage Engine <-> FILEIO transport specific functions
 *
<<<<<<< HEAD
 * Copyright (c) 2005 PyX Technologies, Inc.
 * Copyright (c) 2005-2006 SBE, Inc.  All Rights Reserved.
 * Copyright (c) 2007-2010 Rising Tide Systems
 * Copyright (c) 2008-2010 Linux-iSCSI.org
 *
 * Nicholas A. Bellinger <nab@kernel.org>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
 * (c) Copyright 2005-2013 Datera, Inc.
 *
 * Nicholas A. Bellinger <nab@kernel.org>
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ******************************************************************************/

#include <linux/string.h>
#include <linux/parser.h>
#include <linux/timer.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <scsi/scsi.h>
#include <scsi/scsi_host.h>
=======
#include <linux/vmalloc.h>
#include <linux/falloc.h>
#include <linux/uio.h>
#include <linux/scatterlist.h>
#include <scsi/scsi_proto.h>
#include <asm/unaligned.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <target/target_core_base.h>
#include <target/target_core_backend.h>

#include "target_core_file.h"

<<<<<<< HEAD
static struct se_subsystem_api fileio_template;

/*	fd_attach_hba(): (Part of se_subsystem_api_t template)
 *
 *
 */
=======
static inline struct fd_dev *FD_DEV(struct se_device *dev)
{
	return container_of(dev, struct fd_dev, dev);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int fd_attach_hba(struct se_hba *hba, u32 host_id)
{
	struct fd_host *fd_host;

	fd_host = kzalloc(sizeof(struct fd_host), GFP_KERNEL);
	if (!fd_host) {
		pr_err("Unable to allocate memory for struct fd_host\n");
		return -ENOMEM;
	}

	fd_host->fd_host_id = host_id;

	hba->hba_ptr = fd_host;

	pr_debug("CORE_HBA[%d] - TCM FILEIO HBA Driver %s on Generic"
		" Target Core Stack %s\n", hba->hba_id, FD_VERSION,
<<<<<<< HEAD
		TARGET_CORE_MOD_VERSION);
	pr_debug("CORE_HBA[%d] - Attached FILEIO HBA: %u to Generic"
		" MaxSectors: %u\n",
		hba->hba_id, fd_host->fd_host_id, FD_MAX_SECTORS);
=======
		TARGET_CORE_VERSION);
	pr_debug("CORE_HBA[%d] - Attached FILEIO HBA: %u to Generic\n",
		hba->hba_id, fd_host->fd_host_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void fd_detach_hba(struct se_hba *hba)
{
	struct fd_host *fd_host = hba->hba_ptr;

	pr_debug("CORE_HBA[%d] - Detached FILEIO HBA: %u from Generic"
		" Target Core\n", hba->hba_id, fd_host->fd_host_id);

	kfree(fd_host);
	hba->hba_ptr = NULL;
}

<<<<<<< HEAD
static void *fd_allocate_virtdevice(struct se_hba *hba, const char *name)
=======
static struct se_device *fd_alloc_device(struct se_hba *hba, const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fd_dev *fd_dev;
	struct fd_host *fd_host = hba->hba_ptr;

	fd_dev = kzalloc(sizeof(struct fd_dev), GFP_KERNEL);
	if (!fd_dev) {
		pr_err("Unable to allocate memory for struct fd_dev\n");
		return NULL;
	}

	fd_dev->fd_host = fd_host;

	pr_debug("FILEIO: Allocated fd_dev for %p\n", name);

<<<<<<< HEAD
	return fd_dev;
}

/*	fd_create_virtdevice(): (Part of se_subsystem_api_t template)
 *
 *
 */
static struct se_device *fd_create_virtdevice(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	void *p)
{
	char *dev_p = NULL;
	struct se_device *dev;
	struct se_dev_limits dev_limits;
	struct queue_limits *limits;
	struct fd_dev *fd_dev = p;
	struct fd_host *fd_host = hba->hba_ptr;
	mm_segment_t old_fs;
	struct file *file;
	struct inode *inode = NULL;
	int dev_flags = 0, flags, ret = -EINVAL;

	memset(&dev_limits, 0, sizeof(struct se_dev_limits));

	old_fs = get_fs();
	set_fs(get_ds());
	dev_p = getname(fd_dev->fd_dev_name);
	set_fs(old_fs);

	if (IS_ERR(dev_p)) {
		pr_err("getname(%s) failed: %lu\n",
			fd_dev->fd_dev_name, IS_ERR(dev_p));
		ret = PTR_ERR(dev_p);
		goto fail;
	}
=======
	return &fd_dev->dev;
}

static bool fd_configure_unmap(struct se_device *dev)
{
	struct file *file = FD_DEV(dev)->fd_file;
	struct inode *inode = file->f_mapping->host;

	if (S_ISBLK(inode->i_mode))
		return target_configure_unmap_from_queue(&dev->dev_attrib,
							 I_BDEV(inode));

	/* Limit UNMAP emulation to 8k Number of LBAs (NoLB) */
	dev->dev_attrib.max_unmap_lba_count = 0x2000;
	/* Currently hardcoded to 1 in Linux/SCSI code. */
	dev->dev_attrib.max_unmap_block_desc_count = 1;
	dev->dev_attrib.unmap_granularity = 1;
	dev->dev_attrib.unmap_granularity_alignment = 0;
	return true;
}

static int fd_configure_device(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
	struct fd_host *fd_host = dev->se_hba->hba_ptr;
	struct file *file;
	struct inode *inode = NULL;
	int flags, ret = -EINVAL;

	if (!(fd_dev->fbd_flags & FBDF_HAS_PATH)) {
		pr_err("Missing fd_dev_name=\n");
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Use O_DSYNC by default instead of O_SYNC to forgo syncing
	 * of pure timestamp updates.
	 */
	flags = O_RDWR | O_CREAT | O_LARGEFILE | O_DSYNC;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Optionally allow fd_buffered_io=1 to be enabled for people
	 * who want use the fs buffer cache as an WriteCache mechanism.
	 *
	 * This means that in event of a hard failure, there is a risk
	 * of silent data-loss if the SCSI client has *not* performed a
	 * forced unit access (FUA) write, or issued SYNCHRONIZE_CACHE
	 * to write-out the entire device cache.
	 */
	if (fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) {
		pr_debug("FILEIO: Disabling O_DSYNC, using buffered FILEIO\n");
		flags &= ~O_DSYNC;
	}

<<<<<<< HEAD
	file = filp_open(dev_p, flags, 0600);
	if (IS_ERR(file)) {
		pr_err("filp_open(%s) failed\n", dev_p);
		ret = PTR_ERR(file);
		goto fail;
	}
	if (!file || !file->f_dentry) {
		pr_err("filp_open(%s) failed\n", dev_p);
		goto fail;
	}
=======
	file = filp_open(fd_dev->fd_dev_name, flags, 0600);
	if (IS_ERR(file)) {
		pr_err("filp_open(%s) failed\n", fd_dev->fd_dev_name);
		ret = PTR_ERR(file);
		goto fail;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fd_dev->fd_file = file;
	/*
	 * If using a block backend with this struct file, we extract
	 * fd_dev->fd_[block,dev]_size from struct block_device.
	 *
	 * Otherwise, we use the passed fd_size= from configfs
	 */
	inode = file->f_mapping->host;
	if (S_ISBLK(inode->i_mode)) {
<<<<<<< HEAD
		struct request_queue *q;
		unsigned long long dev_size;
		/*
		 * Setup the local scope queue_limits from struct request_queue->limits
		 * to pass into transport_add_device_to_core_hba() as struct se_dev_limits.
		 */
		q = bdev_get_queue(inode->i_bdev);
		limits = &dev_limits.limits;
		limits->logical_block_size = bdev_logical_block_size(inode->i_bdev);
		limits->max_hw_sectors = queue_max_hw_sectors(q);
		limits->max_sectors = queue_max_sectors(q);
=======
		struct block_device *bdev = I_BDEV(inode);
		unsigned long long dev_size;

		fd_dev->fd_block_size = bdev_logical_block_size(bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Determine the number of bytes from i_size_read() minus
		 * one (1) logical sector from underlying struct block_device
		 */
<<<<<<< HEAD
		fd_dev->fd_block_size = bdev_logical_block_size(inode->i_bdev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_size = (i_size_read(file->f_mapping->host) -
				       fd_dev->fd_block_size);

		pr_debug("FILEIO: Using size: %llu bytes from struct"
			" block_device blocks: %llu logical_block_size: %d\n",
			dev_size, div_u64(dev_size, fd_dev->fd_block_size),
			fd_dev->fd_block_size);
<<<<<<< HEAD
=======
		/*
		 * Enable write same emulation for IBLOCK and use 0xFFFF as
		 * the smaller WRITE_SAME(10) only has a two-byte block count.
		 */
		dev->dev_attrib.max_write_same_len = 0xFFFF;

		if (bdev_nonrot(bdev))
			dev->dev_attrib.is_nonrot = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (!(fd_dev->fbd_flags & FBDF_HAS_SIZE)) {
			pr_err("FILEIO: Missing fd_dev_size="
				" parameter, and no backing struct"
				" block_device\n");
			goto fail;
		}

<<<<<<< HEAD
		limits = &dev_limits.limits;
		limits->logical_block_size = FD_BLOCKSIZE;
		limits->max_hw_sectors = FD_MAX_SECTORS;
		limits->max_sectors = FD_MAX_SECTORS;
		fd_dev->fd_block_size = FD_BLOCKSIZE;
	}

	dev_limits.hw_queue_depth = FD_MAX_DEVICE_QUEUE_DEPTH;
	dev_limits.queue_depth = FD_DEVICE_QUEUE_DEPTH;

	dev = transport_add_device_to_core_hba(hba, &fileio_template,
				se_dev, dev_flags, fd_dev,
				&dev_limits, "FILEIO", FD_VERSION);
	if (!dev)
		goto fail;
=======
		fd_dev->fd_block_size = FD_BLOCKSIZE;

		/*
		 * Limit WRITE_SAME w/ UNMAP=0 emulation to 8k Number of LBAs (NoLB)
		 * based upon struct iovec limit for vfs_writev()
		 */
		dev->dev_attrib.max_write_same_len = 0x1000;
	}

	dev->dev_attrib.hw_block_size = fd_dev->fd_block_size;
	dev->dev_attrib.hw_max_sectors = FD_MAX_BYTES / fd_dev->fd_block_size;
	dev->dev_attrib.hw_queue_depth = FD_MAX_DEVICE_QUEUE_DEPTH;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) {
		pr_debug("FILEIO: Forcing setting of emulate_write_cache=1"
			" with FDBD_HAS_BUFFERED_IO_WCE\n");
<<<<<<< HEAD
		dev->se_sub_dev->se_dev_attrib.emulate_write_cache = 1;
=======
		dev->dev_attrib.emulate_write_cache = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	fd_dev->fd_dev_id = fd_host->fd_host_dev_id_count++;
	fd_dev->fd_queue_depth = dev->queue_depth;

	pr_debug("CORE_FILE[%u] - Added TCM FILEIO Device ID: %u at %s,"
		" %llu total bytes\n", fd_host->fd_host_id, fd_dev->fd_dev_id,
			fd_dev->fd_dev_name, fd_dev->fd_dev_size);

<<<<<<< HEAD
	putname(dev_p);
	return dev;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
fail:
	if (fd_dev->fd_file) {
		filp_close(fd_dev->fd_file, NULL);
		fd_dev->fd_file = NULL;
	}
<<<<<<< HEAD
	putname(dev_p);
	return ERR_PTR(ret);
}

/*	fd_free_device(): (Part of se_subsystem_api_t template)
 *
 *
 */
static void fd_free_device(void *p)
{
	struct fd_dev *fd_dev = p;
=======
	return ret;
}

static void fd_dev_call_rcu(struct rcu_head *p)
{
	struct se_device *dev = container_of(p, struct se_device, rcu_head);
	struct fd_dev *fd_dev = FD_DEV(dev);

	kfree(fd_dev);
}

static void fd_free_device(struct se_device *dev)
{
	call_rcu(&dev->rcu_head, fd_dev_call_rcu);
}

static void fd_destroy_device(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (fd_dev->fd_file) {
		filp_close(fd_dev->fd_file, NULL);
		fd_dev->fd_file = NULL;
	}
<<<<<<< HEAD

	kfree(fd_dev);
}

static inline struct fd_request *FILE_REQ(struct se_task *task)
{
	return container_of(task, struct fd_request, fd_task);
}


static struct se_task *
fd_alloc_task(unsigned char *cdb)
{
	struct fd_request *fd_req;

	fd_req = kzalloc(sizeof(struct fd_request), GFP_KERNEL);
	if (!fd_req) {
		pr_err("Unable to allocate struct fd_request\n");
		return NULL;
	}

	return &fd_req->fd_task;
}

static int fd_do_readv(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);
	struct se_device *se_dev = req->fd_task.task_se_cmd->se_dev;
	struct fd_dev *dev = se_dev->dev_ptr;
	struct file *fd = dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba *
		      se_dev->se_sub_dev->se_dev_attrib.block_size);
	int ret = 0, i;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_nents, GFP_KERNEL);
	if (!iov) {
=======
}

struct target_core_file_cmd {
	unsigned long	len;
	struct se_cmd	*cmd;
	struct kiocb	iocb;
	struct bio_vec	bvecs[];
};

static void cmd_rw_aio_complete(struct kiocb *iocb, long ret)
{
	struct target_core_file_cmd *cmd;

	cmd = container_of(iocb, struct target_core_file_cmd, iocb);

	if (ret != cmd->len)
		target_complete_cmd(cmd->cmd, SAM_STAT_CHECK_CONDITION);
	else
		target_complete_cmd(cmd->cmd, SAM_STAT_GOOD);

	kfree(cmd);
}

static sense_reason_t
fd_execute_rw_aio(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
	      enum dma_data_direction data_direction)
{
	int is_write = !(data_direction == DMA_FROM_DEVICE);
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(dev);
	struct file *file = fd_dev->fd_file;
	struct target_core_file_cmd *aio_cmd;
	struct iov_iter iter;
	struct scatterlist *sg;
	ssize_t len = 0;
	int ret = 0, i;

	aio_cmd = kmalloc(struct_size(aio_cmd, bvecs, sgl_nents), GFP_KERNEL);
	if (!aio_cmd)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	for_each_sg(sgl, sg, sgl_nents, i) {
		bvec_set_page(&aio_cmd->bvecs[i], sg_page(sg), sg->length,
			      sg->offset);
		len += sg->length;
	}

	iov_iter_bvec(&iter, is_write, aio_cmd->bvecs, sgl_nents, len);

	aio_cmd->cmd = cmd;
	aio_cmd->len = len;
	aio_cmd->iocb.ki_pos = cmd->t_task_lba * dev->dev_attrib.block_size;
	aio_cmd->iocb.ki_filp = file;
	aio_cmd->iocb.ki_complete = cmd_rw_aio_complete;
	aio_cmd->iocb.ki_flags = IOCB_DIRECT;

	if (is_write && (cmd->se_cmd_flags & SCF_FUA))
		aio_cmd->iocb.ki_flags |= IOCB_DSYNC;

	if (is_write)
		ret = call_write_iter(file, &aio_cmd->iocb, &iter);
	else
		ret = call_read_iter(file, &aio_cmd->iocb, &iter);

	if (ret != -EIOCBQUEUED)
		cmd_rw_aio_complete(&aio_cmd->iocb, ret);

	return 0;
}

static int fd_do_rw(struct se_cmd *cmd, struct file *fd,
		    u32 block_size, struct scatterlist *sgl,
		    u32 sgl_nents, u32 data_length, int is_write)
{
	struct scatterlist *sg;
	struct iov_iter iter;
	struct bio_vec *bvec;
	ssize_t len = 0;
	loff_t pos = (cmd->t_task_lba * block_size);
	int ret = 0, i;

	bvec = kcalloc(sgl_nents, sizeof(struct bio_vec), GFP_KERNEL);
	if (!bvec) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_err("Unable to allocate fd_do_readv iov[]\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	for_each_sg(task->task_sg, sg, task->task_sg_nents, i) {
		iov[i].iov_len = sg->length;
		iov[i].iov_base = kmap(sg_page(sg)) + sg->offset;
	}

	old_fs = get_fs();
	set_fs(get_ds());
	ret = vfs_readv(fd, &iov[0], task->task_sg_nents, &pos);
	set_fs(old_fs);

	for_each_sg(task->task_sg, sg, task->task_sg_nents, i)
		kunmap(sg_page(sg));
	kfree(iov);
	/*
	 * Return zeros and GOOD status even if the READ did not return
	 * the expected virt_size for struct file w/o a backing struct
	 * block_device.
	 */
	if (S_ISBLK(fd->f_dentry->d_inode->i_mode)) {
		if (ret < 0 || ret != task->task_size) {
			pr_err("vfs_readv() returned %d,"
				" expecting %d for S_ISBLK\n", ret,
				(int)task->task_size);
			return (ret < 0 ? ret : -EINVAL);
		}
	} else {
		if (ret < 0) {
			pr_err("vfs_readv() returned %d for non"
				" S_ISBLK\n", ret);
			return ret;
		}
	}

	return 1;
}

static int fd_do_writev(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);
	struct se_device *se_dev = req->fd_task.task_se_cmd->se_dev;
	struct fd_dev *dev = se_dev->dev_ptr;
	struct file *fd = dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba *
		      se_dev->se_sub_dev->se_dev_attrib.block_size);
	int ret, i = 0;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_nents, GFP_KERNEL);
	if (!iov) {
		pr_err("Unable to allocate fd_do_writev iov[]\n");
		return -ENOMEM;
	}

	for_each_sg(task->task_sg, sg, task->task_sg_nents, i) {
		iov[i].iov_len = sg->length;
		iov[i].iov_base = kmap(sg_page(sg)) + sg->offset;
	}

	old_fs = get_fs();
	set_fs(get_ds());
	ret = vfs_writev(fd, &iov[0], task->task_sg_nents, &pos);
	set_fs(old_fs);

	for_each_sg(task->task_sg, sg, task->task_sg_nents, i)
		kunmap(sg_page(sg));

	kfree(iov);

	if (ret < 0 || ret != task->task_size) {
		pr_err("vfs_writev() returned %d\n", ret);
		return (ret < 0 ? ret : -EINVAL);
	}

	return 1;
}

static void fd_emulate_sync_cache(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = dev->dev_ptr;
=======
	for_each_sg(sgl, sg, sgl_nents, i) {
		bvec_set_page(&bvec[i], sg_page(sg), sg->length, sg->offset);
		len += sg->length;
	}

	iov_iter_bvec(&iter, is_write, bvec, sgl_nents, len);
	if (is_write)
		ret = vfs_iter_write(fd, &iter, &pos, 0);
	else
		ret = vfs_iter_read(fd, &iter, &pos, 0);

	if (is_write) {
		if (ret < 0 || ret != data_length) {
			pr_err("%s() write returned %d\n", __func__, ret);
			if (ret >= 0)
				ret = -EINVAL;
		}
	} else {
		/*
		 * Return zeros and GOOD status even if the READ did not return
		 * the expected virt_size for struct file w/o a backing struct
		 * block_device.
		 */
		if (S_ISBLK(file_inode(fd)->i_mode)) {
			if (ret < 0 || ret != data_length) {
				pr_err("%s() returned %d, expecting %u for "
						"S_ISBLK\n", __func__, ret,
						data_length);
				if (ret >= 0)
					ret = -EINVAL;
			}
		} else {
			if (ret < 0) {
				pr_err("%s() returned %d for non S_ISBLK\n",
						__func__, ret);
			} else if (ret != data_length) {
				/*
				 * Short read case:
				 * Probably some one truncate file under us.
				 * We must explicitly zero sg-pages to prevent
				 * expose uninizialized pages to userspace.
				 */
				if (ret < data_length)
					ret += iov_iter_zero(data_length - ret, &iter);
				else
					ret = -EINVAL;
			}
		}
	}
	kfree(bvec);
	return ret;
}

static sense_reason_t
fd_execute_sync_cache(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int immed = (cmd->t_task_cdb[1] & 0x2);
	loff_t start, end;
	int ret;

	/*
	 * If the Immediate bit is set, queue up the GOOD response
	 * for this SYNCHRONIZE_CACHE op
	 */
	if (immed)
<<<<<<< HEAD
		transport_complete_sync_cache(cmd, 1);
=======
		target_complete_cmd(cmd, SAM_STAT_GOOD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Determine if we will be flushing the entire device.
	 */
	if (cmd->t_task_lba == 0 && cmd->data_length == 0) {
		start = 0;
		end = LLONG_MAX;
	} else {
<<<<<<< HEAD
		start = cmd->t_task_lba * dev->se_sub_dev->se_dev_attrib.block_size;
		if (cmd->data_length)
			end = start + cmd->data_length;
=======
		start = cmd->t_task_lba * dev->dev_attrib.block_size;
		if (cmd->data_length)
			end = start + cmd->data_length - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			end = LLONG_MAX;
	}

	ret = vfs_fsync_range(fd_dev->fd_file, start, end, 1);
	if (ret != 0)
		pr_err("FILEIO: vfs_fsync_range() failed: %d\n", ret);

<<<<<<< HEAD
	if (!immed)
		transport_complete_sync_cache(cmd, ret == 0);
}

static int fd_do_task(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	int ret = 0;

=======
	if (immed)
		return 0;

	if (ret)
		target_complete_cmd(cmd, SAM_STAT_CHECK_CONDITION);
	else
		target_complete_cmd(cmd, SAM_STAT_GOOD);

	return 0;
}

static sense_reason_t
fd_execute_write_same(struct se_cmd *cmd)
{
	struct se_device *se_dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(se_dev);
	loff_t pos = cmd->t_task_lba * se_dev->dev_attrib.block_size;
	sector_t nolb = sbc_get_write_same_sectors(cmd);
	struct iov_iter iter;
	struct bio_vec *bvec;
	unsigned int len = 0, i;
	ssize_t ret;

	if (cmd->prot_op) {
		pr_err("WRITE_SAME: Protection information with FILEIO"
		       " backends not supported\n");
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	if (!cmd->t_data_nents)
		return TCM_INVALID_CDB_FIELD;

	if (cmd->t_data_nents > 1 ||
	    cmd->t_data_sg[0].length != cmd->se_dev->dev_attrib.block_size) {
		pr_err("WRITE_SAME: Illegal SGL t_data_nents: %u length: %u"
			" block_size: %u\n",
			cmd->t_data_nents,
			cmd->t_data_sg[0].length,
			cmd->se_dev->dev_attrib.block_size);
		return TCM_INVALID_CDB_FIELD;
	}

	bvec = kcalloc(nolb, sizeof(struct bio_vec), GFP_KERNEL);
	if (!bvec)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	for (i = 0; i < nolb; i++) {
		bvec_set_page(&bvec[i], sg_page(&cmd->t_data_sg[0]),
			      cmd->t_data_sg[0].length,
			      cmd->t_data_sg[0].offset);
		len += se_dev->dev_attrib.block_size;
	}

	iov_iter_bvec(&iter, ITER_SOURCE, bvec, nolb, len);
	ret = vfs_iter_write(fd_dev->fd_file, &iter, &pos, 0);

	kfree(bvec);
	if (ret < 0 || ret != len) {
		pr_err("vfs_iter_write() returned %zd for write same\n", ret);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	target_complete_cmd(cmd, SAM_STAT_GOOD);
	return 0;
}

static int
fd_do_prot_fill(struct se_device *se_dev, sector_t lba, sector_t nolb,
		void *buf, size_t bufsize)
{
	struct fd_dev *fd_dev = FD_DEV(se_dev);
	struct file *prot_fd = fd_dev->fd_prot_file;
	sector_t prot_length, prot;
	loff_t pos = lba * se_dev->prot_length;

	if (!prot_fd) {
		pr_err("Unable to locate fd_dev->fd_prot_file\n");
		return -ENODEV;
	}

	prot_length = nolb * se_dev->prot_length;

	memset(buf, 0xff, bufsize);
	for (prot = 0; prot < prot_length;) {
		sector_t len = min_t(sector_t, bufsize, prot_length - prot);
		ssize_t ret = kernel_write(prot_fd, buf, len, &pos);

		if (ret != len) {
			pr_err("vfs_write to prot file failed: %zd\n", ret);
			return ret < 0 ? ret : -ENODEV;
		}
		prot += ret;
	}

	return 0;
}

static int
fd_do_prot_unmap(struct se_cmd *cmd, sector_t lba, sector_t nolb)
{
	void *buf;
	int rc;

	buf = (void *)__get_free_page(GFP_KERNEL);
	if (!buf) {
		pr_err("Unable to allocate FILEIO prot buf\n");
		return -ENOMEM;
	}

	rc = fd_do_prot_fill(cmd->se_dev, lba, nolb, buf, PAGE_SIZE);

	free_page((unsigned long)buf);

	return rc;
}

static sense_reason_t
fd_execute_unmap(struct se_cmd *cmd, sector_t lba, sector_t nolb)
{
	struct file *file = FD_DEV(cmd->se_dev)->fd_file;
	struct inode *inode = file->f_mapping->host;
	int ret;

	if (!nolb) {
		return 0;
	}

	if (cmd->se_dev->dev_attrib.pi_prot_type) {
		ret = fd_do_prot_unmap(cmd, lba, nolb);
		if (ret)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	if (S_ISBLK(inode->i_mode)) {
		/* The backend is block device, use discard */
		struct block_device *bdev = I_BDEV(inode);
		struct se_device *dev = cmd->se_dev;

		ret = blkdev_issue_discard(bdev,
					   target_to_linux_sector(dev, lba),
					   target_to_linux_sector(dev,  nolb),
					   GFP_KERNEL);
		if (ret < 0) {
			pr_warn("FILEIO: blkdev_issue_discard() failed: %d\n",
				ret);
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}
	} else {
		/* The backend is normal file, use fallocate */
		struct se_device *se_dev = cmd->se_dev;
		loff_t pos = lba * se_dev->dev_attrib.block_size;
		unsigned int len = nolb * se_dev->dev_attrib.block_size;
		int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;

		if (!file->f_op->fallocate)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

		ret = file->f_op->fallocate(file, mode, pos, len);
		if (ret < 0) {
			pr_warn("FILEIO: fallocate() failed: %d\n", ret);
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}
	}

	return 0;
}

static sense_reason_t
fd_execute_rw_buffered(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
	      enum dma_data_direction data_direction)
{
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(dev);
	struct file *file = fd_dev->fd_file;
	struct file *pfile = fd_dev->fd_prot_file;
	sense_reason_t rc;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Call vectorized fileio functions to map struct scatterlist
	 * physical memory addresses to struct iovec virtual memory.
	 */
<<<<<<< HEAD
	if (task->task_data_direction == DMA_FROM_DEVICE) {
		ret = fd_do_readv(task);
	} else {
		ret = fd_do_writev(task);
		/*
		 * Perform implict vfs_fsync_range() for fd_do_writev() ops
		 * for SCSI WRITEs with Forced Unit Access (FUA) set.
		 * Allow this to happen independent of WCE=0 setting.
		 */
		if (ret > 0 &&
		    dev->se_sub_dev->se_dev_attrib.emulate_fua_write > 0 &&
		    (cmd->se_cmd_flags & SCF_FUA)) {
			struct fd_dev *fd_dev = dev->dev_ptr;
			loff_t start = task->task_lba *
				dev->se_sub_dev->se_dev_attrib.block_size;
			loff_t end = start + task->task_size;

			vfs_fsync_range(fd_dev->fd_file, start, end, 1);
		}
	}

	if (ret < 0) {
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		return ret;
	}
	if (ret) {
		task->task_scsi_status = GOOD;
		transport_complete_task(task, 1);
	}
	return 0;
}

/*	fd_free_task(): (Part of se_subsystem_api_t template)
 *
 *
 */
static void fd_free_task(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);

	kfree(req);
}

enum {
	Opt_fd_dev_name, Opt_fd_dev_size, Opt_fd_buffered_io, Opt_err
=======
	if (data_direction == DMA_FROM_DEVICE) {
		if (cmd->prot_type && dev->dev_attrib.pi_prot_type) {
			ret = fd_do_rw(cmd, pfile, dev->prot_length,
				       cmd->t_prot_sg, cmd->t_prot_nents,
				       cmd->prot_length, 0);
			if (ret < 0)
				return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}

		ret = fd_do_rw(cmd, file, dev->dev_attrib.block_size,
			       sgl, sgl_nents, cmd->data_length, 0);

		if (ret > 0 && cmd->prot_type && dev->dev_attrib.pi_prot_type &&
		    dev->dev_attrib.pi_prot_verify) {
			u32 sectors = cmd->data_length >>
					ilog2(dev->dev_attrib.block_size);

			rc = sbc_dif_verify(cmd, cmd->t_task_lba, sectors,
					    0, cmd->t_prot_sg, 0);
			if (rc)
				return rc;
		}
	} else {
		if (cmd->prot_type && dev->dev_attrib.pi_prot_type &&
		    dev->dev_attrib.pi_prot_verify) {
			u32 sectors = cmd->data_length >>
					ilog2(dev->dev_attrib.block_size);

			rc = sbc_dif_verify(cmd, cmd->t_task_lba, sectors,
					    0, cmd->t_prot_sg, 0);
			if (rc)
				return rc;
		}

		ret = fd_do_rw(cmd, file, dev->dev_attrib.block_size,
			       sgl, sgl_nents, cmd->data_length, 1);
		/*
		 * Perform implicit vfs_fsync_range() for fd_do_writev() ops
		 * for SCSI WRITEs with Forced Unit Access (FUA) set.
		 * Allow this to happen independent of WCE=0 setting.
		 */
		if (ret > 0 && (cmd->se_cmd_flags & SCF_FUA)) {
			loff_t start = cmd->t_task_lba *
				dev->dev_attrib.block_size;
			loff_t end;

			if (cmd->data_length)
				end = start + cmd->data_length - 1;
			else
				end = LLONG_MAX;

			vfs_fsync_range(fd_dev->fd_file, start, end, 1);
		}

		if (ret > 0 && cmd->prot_type && dev->dev_attrib.pi_prot_type) {
			ret = fd_do_rw(cmd, pfile, dev->prot_length,
				       cmd->t_prot_sg, cmd->t_prot_nents,
				       cmd->prot_length, 1);
			if (ret < 0)
				return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}
	}

	if (ret < 0)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	target_complete_cmd(cmd, SAM_STAT_GOOD);
	return 0;
}

static sense_reason_t
fd_execute_rw(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
	      enum dma_data_direction data_direction)
{
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(dev);

	/*
	 * We are currently limited by the number of iovecs (2048) per
	 * single vfs_[writev,readv] call.
	 */
	if (cmd->data_length > FD_MAX_BYTES) {
		pr_err("FILEIO: Not able to process I/O of %u bytes due to"
		       "FD_MAX_BYTES: %u iovec count limitation\n",
			cmd->data_length, FD_MAX_BYTES);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	if (fd_dev->fbd_flags & FDBD_HAS_ASYNC_IO)
		return fd_execute_rw_aio(cmd, sgl, sgl_nents, data_direction);
	return fd_execute_rw_buffered(cmd, sgl, sgl_nents, data_direction);
}

enum {
	Opt_fd_dev_name, Opt_fd_dev_size, Opt_fd_buffered_io,
	Opt_fd_async_io, Opt_err
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static match_table_t tokens = {
	{Opt_fd_dev_name, "fd_dev_name=%s"},
	{Opt_fd_dev_size, "fd_dev_size=%s"},
	{Opt_fd_buffered_io, "fd_buffered_io=%d"},
<<<<<<< HEAD
	{Opt_err, NULL}
};

static ssize_t fd_set_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	const char *page, ssize_t count)
{
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;
=======
	{Opt_fd_async_io, "fd_async_io=%d"},
	{Opt_err, NULL}
};

static ssize_t fd_set_configfs_dev_params(struct se_device *dev,
		const char *page, ssize_t count)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *orig, *ptr, *arg_p, *opts;
	substring_t args[MAX_OPT_ARGS];
	int ret = 0, arg, token;

	opts = kstrdup(page, GFP_KERNEL);
	if (!opts)
		return -ENOMEM;

	orig = opts;

	while ((ptr = strsep(&opts, ",\n")) != NULL) {
		if (!*ptr)
			continue;

		token = match_token(ptr, tokens, args);
		switch (token) {
		case Opt_fd_dev_name:
<<<<<<< HEAD
			arg_p = match_strdup(&args[0]);
			if (!arg_p) {
				ret = -ENOMEM;
				break;
			}
			snprintf(fd_dev->fd_dev_name, FD_MAX_DEV_NAME,
					"%s", arg_p);
			kfree(arg_p);
=======
			if (match_strlcpy(fd_dev->fd_dev_name, &args[0],
				FD_MAX_DEV_NAME) == 0) {
				ret = -EINVAL;
				break;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			pr_debug("FILEIO: Referencing Path: %s\n",
					fd_dev->fd_dev_name);
			fd_dev->fbd_flags |= FBDF_HAS_PATH;
			break;
		case Opt_fd_dev_size:
			arg_p = match_strdup(&args[0]);
			if (!arg_p) {
				ret = -ENOMEM;
				break;
			}
<<<<<<< HEAD
			ret = strict_strtoull(arg_p, 0, &fd_dev->fd_dev_size);
			kfree(arg_p);
			if (ret < 0) {
				pr_err("strict_strtoull() failed for"
=======
			ret = kstrtoull(arg_p, 0, &fd_dev->fd_dev_size);
			kfree(arg_p);
			if (ret < 0) {
				pr_err("kstrtoull() failed for"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						" fd_dev_size=\n");
				goto out;
			}
			pr_debug("FILEIO: Referencing Size: %llu"
					" bytes\n", fd_dev->fd_dev_size);
			fd_dev->fbd_flags |= FBDF_HAS_SIZE;
			break;
		case Opt_fd_buffered_io:
<<<<<<< HEAD
			match_int(args, &arg);
=======
			ret = match_int(args, &arg);
			if (ret)
				goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (arg != 1) {
				pr_err("bogus fd_buffered_io=%d value\n", arg);
				ret = -EINVAL;
				goto out;
			}

			pr_debug("FILEIO: Using buffered I/O"
				" operations for struct fd_dev\n");

			fd_dev->fbd_flags |= FDBD_HAS_BUFFERED_IO_WCE;
			break;
<<<<<<< HEAD
=======
		case Opt_fd_async_io:
			ret = match_int(args, &arg);
			if (ret)
				goto out;
			if (arg != 1) {
				pr_err("bogus fd_async_io=%d value\n", arg);
				ret = -EINVAL;
				goto out;
			}

			pr_debug("FILEIO: Using async I/O"
				" operations for struct fd_dev\n");

			fd_dev->fbd_flags |= FDBD_HAS_ASYNC_IO;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			break;
		}
	}

out:
	kfree(orig);
	return (!ret) ? count : ret;
}

<<<<<<< HEAD
static ssize_t fd_check_configfs_dev_params(struct se_hba *hba, struct se_subsystem_dev *se_dev)
{
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;

	if (!(fd_dev->fbd_flags & FBDF_HAS_PATH)) {
		pr_err("Missing fd_dev_name=\n");
		return -EINVAL;
	}

	return 0;
}

static ssize_t fd_show_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	char *b)
{
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;
	ssize_t bl = 0;

	bl = sprintf(b + bl, "TCM FILEIO ID: %u", fd_dev->fd_dev_id);
	bl += sprintf(b + bl, "        File: %s  Size: %llu  Mode: %s\n",
		fd_dev->fd_dev_name, fd_dev->fd_dev_size,
		(fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) ?
		"Buffered-WCE" : "O_DSYNC");
	return bl;
}

/*	fd_get_device_rev(): (Part of se_subsystem_api_t template)
 *
 *
 */
static u32 fd_get_device_rev(struct se_device *dev)
{
	return SCSI_SPC_2; /* Returns SPC-3 in Initiator Data */
}

/*	fd_get_device_type(): (Part of se_subsystem_api_t template)
 *
 *
 */
static u32 fd_get_device_type(struct se_device *dev)
{
	return TYPE_DISK;
}

static sector_t fd_get_blocks(struct se_device *dev)
{
	struct fd_dev *fd_dev = dev->dev_ptr;
=======
static ssize_t fd_show_configfs_dev_params(struct se_device *dev, char *b)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
	ssize_t bl = 0;

	bl = sprintf(b + bl, "TCM FILEIO ID: %u", fd_dev->fd_dev_id);
	bl += sprintf(b + bl, "        File: %s  Size: %llu  Mode: %s Async: %d\n",
		fd_dev->fd_dev_name, fd_dev->fd_dev_size,
		(fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) ?
		"Buffered-WCE" : "O_DSYNC",
		!!(fd_dev->fbd_flags & FDBD_HAS_ASYNC_IO));
	return bl;
}

static sector_t fd_get_blocks(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct file *f = fd_dev->fd_file;
	struct inode *i = f->f_mapping->host;
	unsigned long long dev_size;
	/*
	 * When using a file that references an underlying struct block_device,
	 * ensure dev_size is always based on the current inode size in order
	 * to handle underlying block_device resize operations.
	 */
	if (S_ISBLK(i->i_mode))
<<<<<<< HEAD
		dev_size = (i_size_read(i) - fd_dev->fd_block_size);
	else
		dev_size = fd_dev->fd_dev_size;

	return div_u64(dev_size, dev->se_sub_dev->se_dev_attrib.block_size);
}

static struct se_subsystem_api fileio_template = {
	.name			= "fileio",
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_VHBA_PDEV,
	.write_cache_emulated	= 1,
	.fua_write_emulated	= 1,
	.attach_hba		= fd_attach_hba,
	.detach_hba		= fd_detach_hba,
	.allocate_virtdevice	= fd_allocate_virtdevice,
	.create_virtdevice	= fd_create_virtdevice,
	.free_device		= fd_free_device,
	.alloc_task		= fd_alloc_task,
	.do_task		= fd_do_task,
	.do_sync_cache		= fd_emulate_sync_cache,
	.free_task		= fd_free_task,
	.check_configfs_dev_params = fd_check_configfs_dev_params,
	.set_configfs_dev_params = fd_set_configfs_dev_params,
	.show_configfs_dev_params = fd_show_configfs_dev_params,
	.get_device_rev		= fd_get_device_rev,
	.get_device_type	= fd_get_device_type,
	.get_blocks		= fd_get_blocks,
=======
		dev_size = i_size_read(i);
	else
		dev_size = fd_dev->fd_dev_size;

	return div_u64(dev_size - dev->dev_attrib.block_size,
		       dev->dev_attrib.block_size);
}

static int fd_init_prot(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
	struct file *prot_file, *file = fd_dev->fd_file;
	struct inode *inode;
	int ret, flags = O_RDWR | O_CREAT | O_LARGEFILE | O_DSYNC;
	char buf[FD_MAX_DEV_PROT_NAME];

	if (!file) {
		pr_err("Unable to locate fd_dev->fd_file\n");
		return -ENODEV;
	}

	inode = file->f_mapping->host;
	if (S_ISBLK(inode->i_mode)) {
		pr_err("FILEIO Protection emulation only supported on"
		       " !S_ISBLK\n");
		return -ENOSYS;
	}

	if (fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE)
		flags &= ~O_DSYNC;

	snprintf(buf, FD_MAX_DEV_PROT_NAME, "%s.protection",
		 fd_dev->fd_dev_name);

	prot_file = filp_open(buf, flags, 0600);
	if (IS_ERR(prot_file)) {
		pr_err("filp_open(%s) failed\n", buf);
		ret = PTR_ERR(prot_file);
		return ret;
	}
	fd_dev->fd_prot_file = prot_file;

	return 0;
}

static int fd_format_prot(struct se_device *dev)
{
	unsigned char *buf;
	int unit_size = FDBD_FORMAT_UNIT_SIZE * dev->dev_attrib.block_size;
	int ret;

	if (!dev->dev_attrib.pi_prot_type) {
		pr_err("Unable to format_prot while pi_prot_type == 0\n");
		return -ENODEV;
	}

	buf = vzalloc(unit_size);
	if (!buf) {
		pr_err("Unable to allocate FILEIO prot buf\n");
		return -ENOMEM;
	}

	pr_debug("Using FILEIO prot_length: %llu\n",
		 (unsigned long long)(dev->transport->get_blocks(dev) + 1) *
					dev->prot_length);

	ret = fd_do_prot_fill(dev, 0, dev->transport->get_blocks(dev) + 1,
			      buf, unit_size);
	vfree(buf);
	return ret;
}

static void fd_free_prot(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);

	if (!fd_dev->fd_prot_file)
		return;

	filp_close(fd_dev->fd_prot_file, NULL);
	fd_dev->fd_prot_file = NULL;
}

static struct exec_cmd_ops fd_exec_cmd_ops = {
	.execute_rw		= fd_execute_rw,
	.execute_sync_cache	= fd_execute_sync_cache,
	.execute_write_same	= fd_execute_write_same,
	.execute_unmap		= fd_execute_unmap,
};

static sense_reason_t
fd_parse_cdb(struct se_cmd *cmd)
{
	return sbc_parse_cdb(cmd, &fd_exec_cmd_ops);
}

static const struct target_backend_ops fileio_ops = {
	.name			= "fileio",
	.inquiry_prod		= "FILEIO",
	.inquiry_rev		= FD_VERSION,
	.owner			= THIS_MODULE,
	.attach_hba		= fd_attach_hba,
	.detach_hba		= fd_detach_hba,
	.alloc_device		= fd_alloc_device,
	.configure_device	= fd_configure_device,
	.destroy_device		= fd_destroy_device,
	.free_device		= fd_free_device,
	.configure_unmap	= fd_configure_unmap,
	.parse_cdb		= fd_parse_cdb,
	.set_configfs_dev_params = fd_set_configfs_dev_params,
	.show_configfs_dev_params = fd_show_configfs_dev_params,
	.get_device_type	= sbc_get_device_type,
	.get_blocks		= fd_get_blocks,
	.init_prot		= fd_init_prot,
	.format_prot		= fd_format_prot,
	.free_prot		= fd_free_prot,
	.tb_dev_attrib_attrs	= sbc_attrib_attrs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init fileio_module_init(void)
{
<<<<<<< HEAD
	return transport_subsystem_register(&fileio_template);
}

static void fileio_module_exit(void)
{
	transport_subsystem_release(&fileio_template);
=======
	return transport_backend_register(&fileio_ops);
}

static void __exit fileio_module_exit(void)
{
	target_backend_unregister(&fileio_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

MODULE_DESCRIPTION("TCM FILEIO subsystem plugin");
MODULE_AUTHOR("nab@Linux-iSCSI.org");
MODULE_LICENSE("GPL");

module_init(fileio_module_init);
module_exit(fileio_module_exit);
