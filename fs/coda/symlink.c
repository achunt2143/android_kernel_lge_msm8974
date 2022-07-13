<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Symlink inode operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/pagemap.h>

#include <linux/coda.h>
<<<<<<< HEAD
#include <linux/coda_psdev.h>

#include "coda_linux.h"

static int coda_symlink_filler(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	int error;
	struct coda_inode_info *cii;
	unsigned int len = PAGE_SIZE;
	char *p = kmap(page);
=======
#include "coda_psdev.h"
#include "coda_linux.h"

static int coda_symlink_filler(struct file *file, struct folio *folio)
{
	struct inode *inode = folio->mapping->host;
	int error;
	struct coda_inode_info *cii;
	unsigned int len = PAGE_SIZE;
	char *p = folio_address(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cii = ITOC(inode);

	error = venus_readlink(inode->i_sb, &cii->c_fid, p, &len);
	if (error)
		goto fail;
<<<<<<< HEAD
	SetPageUptodate(page);
	kunmap(page);
	unlock_page(page);
	return 0;

fail:
	SetPageError(page);
	kunmap(page);
	unlock_page(page);
=======
	folio_mark_uptodate(folio);
	folio_unlock(folio);
	return 0;

fail:
	folio_set_error(folio);
	folio_unlock(folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

const struct address_space_operations coda_symlink_aops = {
<<<<<<< HEAD
	.readpage	= coda_symlink_filler,
=======
	.read_folio	= coda_symlink_filler,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
