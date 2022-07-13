<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/befs/io.c
 *
 * Copyright (C) 2001 Will Dyson <will_dyson@pobox.com
 *
<<<<<<< HEAD
 * Based on portions of file.c and inode.c 
=======
 * Based on portions of file.c and inode.c
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * by Makoto Kato (m_kato@ga2.so-net.ne.jp)
 *
 * Many thanks to Dominic Giampaolo, author of Practical File System
 * Design with the Be File System, for such a helpful book.
 *
 */

#include <linux/buffer_head.h>

#include "befs.h"
#include "io.h"

/*
 * Converts befs notion of disk addr to a disk offset and uses
 * linux kernel function sb_bread() to get the buffer containing
<<<<<<< HEAD
 * the offset. -Will Dyson
 *
=======
 * the offset.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

struct buffer_head *
befs_bread_iaddr(struct super_block *sb, befs_inode_addr iaddr)
{
<<<<<<< HEAD
	struct buffer_head *bh = NULL;
	befs_blocknr_t block = 0;
	befs_sb_info *befs_sb = BEFS_SB(sb);

	befs_debug(sb, "---> Enter befs_read_iaddr() "
		   "[%u, %hu, %hu]",
		   iaddr.allocation_group, iaddr.start, iaddr.len);
=======
	struct buffer_head *bh;
	befs_blocknr_t block;
	struct befs_sb_info *befs_sb = BEFS_SB(sb);

	befs_debug(sb, "---> Enter %s "
		   "[%u, %hu, %hu]", __func__, iaddr.allocation_group,
		   iaddr.start, iaddr.len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (iaddr.allocation_group > befs_sb->num_ags) {
		befs_error(sb, "BEFS: Invalid allocation group %u, max is %u",
			   iaddr.allocation_group, befs_sb->num_ags);
		goto error;
	}

	block = iaddr2blockno(sb, &iaddr);

<<<<<<< HEAD
	befs_debug(sb, "befs_read_iaddr: offset = %lu", block);
=======
	befs_debug(sb, "%s: offset = %lu", __func__, (unsigned long)block);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bh = sb_bread(sb, block);

	if (bh == NULL) {
<<<<<<< HEAD
		befs_error(sb, "Failed to read block %lu", block);
		goto error;
	}

	befs_debug(sb, "<--- befs_read_iaddr()");
	return bh;

      error:
	befs_debug(sb, "<--- befs_read_iaddr() ERROR");
	return NULL;
}

struct buffer_head *
befs_bread(struct super_block *sb, befs_blocknr_t block)
{
	struct buffer_head *bh = NULL;

	befs_debug(sb, "---> Enter befs_read() %Lu", block);

	bh = sb_bread(sb, block);

	if (bh == NULL) {
		befs_error(sb, "Failed to read block %lu", block);
		goto error;
	}

	befs_debug(sb, "<--- befs_read()");

	return bh;

      error:
	befs_debug(sb, "<--- befs_read() ERROR");
=======
		befs_error(sb, "Failed to read block %lu",
			   (unsigned long)block);
		goto error;
	}

	befs_debug(sb, "<--- %s", __func__);
	return bh;

error:
	befs_debug(sb, "<--- %s ERROR", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}
