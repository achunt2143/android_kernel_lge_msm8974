<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * symlink.c
 *
 * Copyright (c) 1999 Al Smith
 *
 * Portions derived from work (c) 1995,1996 Christian Vogelgsang.
 */

#include <linux/string.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include "efs.h"

<<<<<<< HEAD
static int efs_symlink_readpage(struct file *file, struct page *page)
{
	char *link = kmap(page);
=======
static int efs_symlink_read_folio(struct file *file, struct folio *folio)
{
	struct page *page = &folio->page;
	char *link = page_address(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct buffer_head * bh;
	struct inode * inode = page->mapping->host;
	efs_block_t size = inode->i_size;
	int err;
  
	err = -ENAMETOOLONG;
	if (size > 2 * EFS_BLOCKSIZE)
		goto fail;
  
	/* read first 512 bytes of link target */
	err = -EIO;
	bh = sb_bread(inode->i_sb, efs_bmap(inode, 0));
	if (!bh)
		goto fail;
	memcpy(link, bh->b_data, (size > EFS_BLOCKSIZE) ? EFS_BLOCKSIZE : size);
	brelse(bh);
	if (size > EFS_BLOCKSIZE) {
		bh = sb_bread(inode->i_sb, efs_bmap(inode, 1));
		if (!bh)
			goto fail;
		memcpy(link + EFS_BLOCKSIZE, bh->b_data, size - EFS_BLOCKSIZE);
		brelse(bh);
	}
	link[size] = '\0';
	SetPageUptodate(page);
<<<<<<< HEAD
	kunmap(page);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_page(page);
	return 0;
fail:
	SetPageError(page);
<<<<<<< HEAD
	kunmap(page);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_page(page);
	return err;
}

const struct address_space_operations efs_symlink_aops = {
<<<<<<< HEAD
	.readpage	= efs_symlink_readpage
=======
	.read_folio	= efs_symlink_read_folio
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
