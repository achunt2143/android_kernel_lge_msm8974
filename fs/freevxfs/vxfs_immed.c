<<<<<<< HEAD
/*
 * Copyright (c) 2000-2001 Christoph Hellwig.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2000-2001 Christoph Hellwig.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Veritas filesystem driver - support for 'immed' inodes.
 */
#include <linux/fs.h>
#include <linux/pagemap.h>
<<<<<<< HEAD
#include <linux/namei.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "vxfs.h"
#include "vxfs_extern.h"
#include "vxfs_inode.h"

<<<<<<< HEAD

static void *	vxfs_immed_follow_link(struct dentry *, struct nameidata *);

static int	vxfs_immed_readpage(struct file *, struct page *);

/*
 * Inode operations for immed symlinks.
 *
 * Unliked all other operations we do not go through the pagecache,
 * but do all work directly on the inode.
 */
const struct inode_operations vxfs_immed_symlink_iops = {
	.readlink =		generic_readlink,
	.follow_link =		vxfs_immed_follow_link,
};

/*
 * Address space operations for immed files and directories.
 */
const struct address_space_operations vxfs_immed_aops = {
	.readpage =		vxfs_immed_readpage,
};

/**
 * vxfs_immed_follow_link - follow immed symlink
 * @dp:		dentry for the link
 * @np:		pathname lookup data for the current path walk
 *
 * Description:
 *   vxfs_immed_follow_link restarts the pathname lookup with
 *   the data obtained from @dp.
 *
 * Returns:
 *   Zero on success, else a negative error code.
 */
static void *
vxfs_immed_follow_link(struct dentry *dp, struct nameidata *np)
{
	struct vxfs_inode_info		*vip = VXFS_INO(dp->d_inode);
	nd_set_link(np, vip->vii_immed.vi_immed);
	return NULL;
}

/**
 * vxfs_immed_readpage - read part of an immed inode into pagecache
 * @file:	file context (unused)
 * @page:	page frame to fill in.
 *
 * Description:
 *   vxfs_immed_readpage reads a part of the immed area of the
 *   file that hosts @pp into the pagecache.
=======
/**
 * vxfs_immed_read_folio - read part of an immed inode into pagecache
 * @fp:		file context (unused)
 * @folio:	folio to fill in.
 *
 * Description:
 *   vxfs_immed_read_folio reads a part of the immed area of the
 *   file that hosts @folio into the pagecache.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns:
 *   Zero on success, else a negative error code.
 *
 * Locking status:
<<<<<<< HEAD
 *   @page is locked and will be unlocked.
 */
static int
vxfs_immed_readpage(struct file *fp, struct page *pp)
{
	struct vxfs_inode_info	*vip = VXFS_INO(pp->mapping->host);
	u_int64_t	offset = (u_int64_t)pp->index << PAGE_CACHE_SHIFT;
	caddr_t		kaddr;

	kaddr = kmap(pp);
	memcpy(kaddr, vip->vii_immed.vi_immed + offset, PAGE_CACHE_SIZE);
	kunmap(pp);
	
	flush_dcache_page(pp);
	SetPageUptodate(pp);
        unlock_page(pp);

	return 0;
}
=======
 *   @folio is locked and will be unlocked.
 */
static int vxfs_immed_read_folio(struct file *fp, struct folio *folio)
{
	struct vxfs_inode_info *vip = VXFS_INO(folio->mapping->host);
	void *src = vip->vii_immed.vi_immed + folio_pos(folio);
	unsigned long i;

	for (i = 0; i < folio_nr_pages(folio); i++) {
		memcpy_to_page(folio_page(folio, i), 0, src, PAGE_SIZE);
		src += PAGE_SIZE;
	}

	folio_mark_uptodate(folio);
	folio_unlock(folio);

	return 0;
}

/*
 * Address space operations for immed files and directories.
 */
const struct address_space_operations vxfs_immed_aops = {
	.read_folio =	vxfs_immed_read_folio,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
