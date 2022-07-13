<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * lowlevel.c
 *
 * PURPOSE
 *  Low Level Device Routines for the UDF filesystem
 *
 * COPYRIGHT
<<<<<<< HEAD
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  (C) 1999-2001 Ben Fennema
 *
 * HISTORY
 *
 *  03/26/99 blf  Created.
 */

#include "udfdecl.h"

#include <linux/blkdev.h>
#include <linux/cdrom.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "udf_sb.h"

unsigned int udf_get_last_session(struct super_block *sb)
{
<<<<<<< HEAD
	struct cdrom_multisession ms_info;
	unsigned int vol_desc_start;
	struct block_device *bdev = sb->s_bdev;
	int i;

	vol_desc_start = 0;
	ms_info.addr_format = CDROM_LBA;
	i = ioctl_by_bdev(bdev, CDROMMULTISESSION, (unsigned long)&ms_info);

	if (i == 0) {
		udf_debug("XA disk: %s, vol_desc_start=%d\n",
			  ms_info.xa_flag ? "yes" : "no", ms_info.addr.lba);
		if (ms_info.xa_flag) /* necessary for a valid ms_info.addr */
			vol_desc_start = ms_info.addr.lba;
	} else {
		udf_debug("CDROMMULTISESSION not supported: rc=%d\n", i);
	}
	return vol_desc_start;
}

unsigned long udf_get_last_block(struct super_block *sb)
{
	struct block_device *bdev = sb->s_bdev;
	unsigned long lblock = 0;

	/*
	 * ioctl failed or returned obviously bogus value?
	 * Try using the device size...
	 */
	if (ioctl_by_bdev(bdev, CDROM_LAST_WRITTEN, (unsigned long) &lblock) ||
	    lblock == 0)
		lblock = bdev->bd_inode->i_size >> sb->s_blocksize_bits;

	if (lblock)
		return lblock - 1;
	else
		return 0;
=======
	struct cdrom_device_info *cdi = disk_to_cdi(sb->s_bdev->bd_disk);
	struct cdrom_multisession ms_info;

	if (!cdi) {
		udf_debug("CDROMMULTISESSION not supported.\n");
		return 0;
	}

	ms_info.addr_format = CDROM_LBA;
	if (cdrom_multisession(cdi, &ms_info) == 0) {
		udf_debug("XA disk: %s, vol_desc_start=%d\n",
			  ms_info.xa_flag ? "yes" : "no", ms_info.addr.lba);
		if (ms_info.xa_flag) /* necessary for a valid ms_info.addr */
			return ms_info.addr.lba;
	}
	return 0;
}

udf_pblk_t udf_get_last_block(struct super_block *sb)
{
	struct cdrom_device_info *cdi = disk_to_cdi(sb->s_bdev->bd_disk);
	unsigned long lblock = 0;

	/*
	 * The cdrom layer call failed or returned obviously bogus value?
	 * Try using the device size...
	 */
	if (!cdi || cdrom_get_last_written(cdi, &lblock) || lblock == 0) {
		if (sb_bdev_nr_blocks(sb) > ~(udf_pblk_t)0)
			return 0;
		lblock = sb_bdev_nr_blocks(sb);
	}

	if (lblock)
		return lblock - 1;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
