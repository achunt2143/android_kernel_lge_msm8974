<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * namei.c
 *
 * PURPOSE
 *      Inode name handling routines for the OSTA-UDF(tm) filesystem.
 *
 * COPYRIGHT
<<<<<<< HEAD
 *      This file is distributed under the terms of the GNU General Public
 *      License (GPL). Copies of the GPL can be obtained from:
 *              ftp://prep.ai.mit.edu/pub/gnu/GPL
 *      Each contributing author retains all rights to their own work.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  (C) 1998-2004 Ben Fennema
 *  (C) 1999-2000 Stelias Computing Inc
 *
 * HISTORY
 *
 *  12/12/98 blf  Created. Split out the lookup code from dir.c
 *  04/19/99 blf  link, mknod, symlink support
 */

#include "udfdecl.h"

#include "udf_i.h"
#include "udf_sb.h"
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
#include <linux/sched.h>
#include <linux/crc-itu-t.h>
#include <linux/exportfs.h>
=======
#include <linux/sched.h>
#include <linux/crc-itu-t.h>
#include <linux/exportfs.h>
#include <linux/iversion.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int udf_match(int len1, const unsigned char *name1, int len2,
			    const unsigned char *name2)
{
	if (len1 != len2)
		return 0;

	return !memcmp(name1, name2, len1);
}

<<<<<<< HEAD
int udf_write_fi(struct inode *inode, struct fileIdentDesc *cfi,
		 struct fileIdentDesc *sfi, struct udf_fileident_bh *fibh,
		 uint8_t *impuse, uint8_t *fileident)
{
	uint16_t crclen = fibh->eoffset - fibh->soffset - sizeof(struct tag);
	uint16_t crc;
	int offset;
	uint16_t liu = le16_to_cpu(cfi->lengthOfImpUse);
	uint8_t lfi = cfi->lengthFileIdent;
	int padlen = fibh->eoffset - fibh->soffset - liu - lfi -
		sizeof(struct fileIdentDesc);
	int adinicb = 0;

	if (UDF_I(inode)->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		adinicb = 1;

	offset = fibh->soffset + sizeof(struct fileIdentDesc);

	if (impuse) {
		if (adinicb || (offset + liu < 0)) {
			memcpy((uint8_t *)sfi->impUse, impuse, liu);
		} else if (offset >= 0) {
			memcpy(fibh->ebh->b_data + offset, impuse, liu);
		} else {
			memcpy((uint8_t *)sfi->impUse, impuse, -offset);
			memcpy(fibh->ebh->b_data, impuse - offset,
				liu + offset);
		}
	}

	offset += liu;

	if (fileident) {
		if (adinicb || (offset + lfi < 0)) {
			memcpy((uint8_t *)sfi->fileIdent + liu, fileident, lfi);
		} else if (offset >= 0) {
			memcpy(fibh->ebh->b_data + offset, fileident, lfi);
		} else {
			memcpy((uint8_t *)sfi->fileIdent + liu, fileident,
				-offset);
			memcpy(fibh->ebh->b_data, fileident - offset,
				lfi + offset);
		}
	}

	offset += lfi;

	if (adinicb || (offset + padlen < 0)) {
		memset((uint8_t *)sfi->padding + liu + lfi, 0x00, padlen);
	} else if (offset >= 0) {
		memset(fibh->ebh->b_data + offset, 0x00, padlen);
	} else {
		memset((uint8_t *)sfi->padding + liu + lfi, 0x00, -offset);
		memset(fibh->ebh->b_data, 0x00, padlen + offset);
	}

	crc = crc_itu_t(0, (uint8_t *)cfi + sizeof(struct tag),
		      sizeof(struct fileIdentDesc) - sizeof(struct tag));

	if (fibh->sbh == fibh->ebh) {
		crc = crc_itu_t(crc, (uint8_t *)sfi->impUse,
			      crclen + sizeof(struct tag) -
			      sizeof(struct fileIdentDesc));
	} else if (sizeof(struct fileIdentDesc) >= -fibh->soffset) {
		crc = crc_itu_t(crc, fibh->ebh->b_data +
					sizeof(struct fileIdentDesc) +
					fibh->soffset,
			      crclen + sizeof(struct tag) -
					sizeof(struct fileIdentDesc));
	} else {
		crc = crc_itu_t(crc, (uint8_t *)sfi->impUse,
			      -fibh->soffset - sizeof(struct fileIdentDesc));
		crc = crc_itu_t(crc, fibh->ebh->b_data, fibh->eoffset);
	}

	cfi->descTag.descCRC = cpu_to_le16(crc);
	cfi->descTag.descCRCLength = cpu_to_le16(crclen);
	cfi->descTag.tagChecksum = udf_tag_checksum(&cfi->descTag);

	if (adinicb || (sizeof(struct fileIdentDesc) <= -fibh->soffset)) {
		memcpy((uint8_t *)sfi, (uint8_t *)cfi,
			sizeof(struct fileIdentDesc));
	} else {
		memcpy((uint8_t *)sfi, (uint8_t *)cfi, -fibh->soffset);
		memcpy(fibh->ebh->b_data, (uint8_t *)cfi - fibh->soffset,
		       sizeof(struct fileIdentDesc) + fibh->soffset);
	}

	if (adinicb) {
		mark_inode_dirty(inode);
	} else {
		if (fibh->sbh != fibh->ebh)
			mark_buffer_dirty_inode(fibh->ebh, inode);
		mark_buffer_dirty_inode(fibh->sbh, inode);
	}
	return 0;
}

static struct fileIdentDesc *udf_find_entry(struct inode *dir,
					    const struct qstr *child,
					    struct udf_fileident_bh *fibh,
					    struct fileIdentDesc *cfi)
{
	struct fileIdentDesc *fi = NULL;
	loff_t f_pos;
	int block, flen;
	unsigned char *fname = NULL;
	unsigned char *nameptr;
	uint8_t lfi;
	uint16_t liu;
	loff_t size;
	struct kernel_lb_addr eloc;
	uint32_t elen;
	sector_t offset;
	struct extent_position epos = {};
	struct udf_inode_info *dinfo = UDF_I(dir);
	int isdotdot = child->len == 2 &&
		child->name[0] == '.' && child->name[1] == '.';

	size = udf_ext0_offset(dir) + dir->i_size;
	f_pos = udf_ext0_offset(dir);

	fibh->sbh = fibh->ebh = NULL;
	fibh->soffset = fibh->eoffset = f_pos & (dir->i_sb->s_blocksize - 1);
	if (dinfo->i_alloc_type != ICBTAG_FLAG_AD_IN_ICB) {
		if (inode_bmap(dir, f_pos >> dir->i_sb->s_blocksize_bits, &epos,
		    &eloc, &elen, &offset) != (EXT_RECORDED_ALLOCATED >> 30))
			goto out_err;
		block = udf_get_lb_pblock(dir->i_sb, &eloc, offset);
		if ((++offset << dir->i_sb->s_blocksize_bits) < elen) {
			if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_SHORT)
				epos.offset -= sizeof(struct short_ad);
			else if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_LONG)
				epos.offset -= sizeof(struct long_ad);
		} else
			offset = 0;

		fibh->sbh = fibh->ebh = udf_tread(dir->i_sb, block);
		if (!fibh->sbh)
			goto out_err;
	}

	fname = kmalloc(UDF_NAME_LEN, GFP_NOFS);
	if (!fname)
		goto out_err;

	while (f_pos < size) {
		fi = udf_fileident_read(dir, &f_pos, fibh, cfi, &epos, &eloc,
					&elen, &offset);
		if (!fi)
			goto out_err;

		liu = le16_to_cpu(cfi->lengthOfImpUse);
		lfi = cfi->lengthFileIdent;

		if (fibh->sbh == fibh->ebh) {
			nameptr = fi->fileIdent + liu;
		} else {
			int poffset;	/* Unpaded ending offset */

			poffset = fibh->soffset + sizeof(struct fileIdentDesc) +
					liu + lfi;

			if (poffset >= lfi)
				nameptr = (uint8_t *)(fibh->ebh->b_data +
						      poffset - lfi);
			else {
				nameptr = fname;
				memcpy(nameptr, fi->fileIdent + liu,
					lfi - poffset);
				memcpy(nameptr + lfi - poffset,
					fibh->ebh->b_data, poffset);
			}
		}

		if ((cfi->fileCharacteristics & FID_FILE_CHAR_DELETED) != 0) {
			if (!UDF_QUERY_FLAG(dir->i_sb, UDF_FLAG_UNDELETE))
				continue;
		}

		if ((cfi->fileCharacteristics & FID_FILE_CHAR_HIDDEN) != 0) {
			if (!UDF_QUERY_FLAG(dir->i_sb, UDF_FLAG_UNHIDE))
				continue;
		}

		if ((cfi->fileCharacteristics & FID_FILE_CHAR_PARENT) &&
		    isdotdot)
			goto out_ok;

		if (!lfi)
			continue;

		flen = udf_get_filename(dir->i_sb, nameptr, lfi, fname,
					UDF_NAME_LEN);
		if (flen && udf_match(flen, fname, child->len, child->name))
			goto out_ok;
	}

out_err:
	fi = NULL;
	if (fibh->sbh != fibh->ebh)
		brelse(fibh->ebh);
	brelse(fibh->sbh);
out_ok:
	brelse(epos.bh);
	kfree(fname);

	return fi;
=======
/**
 * udf_fiiter_find_entry - find entry in given directory.
 *
 * @dir:	directory inode to search in
 * @child:	qstr of the name
 * @iter:	iter to use for searching
 *
 * This function searches in the directory @dir for a file name @child. When
 * found, @iter points to the position in the directory with given entry.
 *
 * Returns 0 on success, < 0 on error (including -ENOENT).
 */
static int udf_fiiter_find_entry(struct inode *dir, const struct qstr *child,
				 struct udf_fileident_iter *iter)
{
	int flen;
	unsigned char *fname = NULL;
	struct super_block *sb = dir->i_sb;
	int isdotdot = child->len == 2 &&
		child->name[0] == '.' && child->name[1] == '.';
	int ret;

	fname = kmalloc(UDF_NAME_LEN, GFP_KERNEL);
	if (!fname)
		return -ENOMEM;

	for (ret = udf_fiiter_init(iter, dir, 0);
	     !ret && iter->pos < dir->i_size;
	     ret = udf_fiiter_advance(iter)) {
		if (iter->fi.fileCharacteristics & FID_FILE_CHAR_DELETED) {
			if (!UDF_QUERY_FLAG(sb, UDF_FLAG_UNDELETE))
				continue;
		}

		if (iter->fi.fileCharacteristics & FID_FILE_CHAR_HIDDEN) {
			if (!UDF_QUERY_FLAG(sb, UDF_FLAG_UNHIDE))
				continue;
		}

		if ((iter->fi.fileCharacteristics & FID_FILE_CHAR_PARENT) &&
		    isdotdot)
			goto out_ok;

		if (!iter->fi.lengthFileIdent)
			continue;

		flen = udf_get_filename(sb, iter->name,
				iter->fi.lengthFileIdent, fname, UDF_NAME_LEN);
		if (flen < 0) {
			ret = flen;
			goto out_err;
		}

		if (udf_match(flen, fname, child->len, child->name))
			goto out_ok;
	}
	if (!ret)
		ret = -ENOENT;

out_err:
	udf_fiiter_release(iter);
out_ok:
	kfree(fname);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *udf_lookup(struct inode *dir, struct dentry *dentry,
				 unsigned int flags)
{
	struct inode *inode = NULL;
<<<<<<< HEAD
	struct fileIdentDesc cfi;
	struct udf_fileident_bh fibh;

	if (dentry->d_name.len > UDF_NAME_LEN - 2)
		return ERR_PTR(-ENAMETOOLONG);

#ifdef UDF_RECOVERY
	/* temporary shorthand for specifying files by inode number */
	if (!strncmp(dentry->d_name.name, ".B=", 3)) {
		struct kernel_lb_addr lb = {
			.logicalBlockNum = 0,
			.partitionReferenceNum =
				simple_strtoul(dentry->d_name.name + 3,
						NULL, 0),
		};
		inode = udf_iget(dir->i_sb, lb);
		if (!inode) {
			return ERR_PTR(-EACCES);
		}
	} else
#endif /* UDF_RECOVERY */

	if (udf_find_entry(dir, &dentry->d_name, &fibh, &cfi)) {
		struct kernel_lb_addr loc;

		if (fibh.sbh != fibh.ebh)
			brelse(fibh.ebh);
		brelse(fibh.sbh);

		loc = lelb_to_cpu(cfi.icb.extLocation);
		inode = udf_iget(dir->i_sb, &loc);
		if (!inode) {
			return ERR_PTR(-EACCES);
		}
=======
	struct udf_fileident_iter iter;
	int err;

	if (dentry->d_name.len > UDF_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	err = udf_fiiter_find_entry(dir, &dentry->d_name, &iter);
	if (err < 0 && err != -ENOENT)
		return ERR_PTR(err);

	if (err == 0) {
		struct kernel_lb_addr loc;

		loc = lelb_to_cpu(iter.fi.icb.extLocation);
		udf_fiiter_release(&iter);

		inode = udf_iget(dir->i_sb, &loc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return d_splice_alias(inode, dentry);
}

<<<<<<< HEAD
static struct fileIdentDesc *udf_add_entry(struct inode *dir,
					   struct dentry *dentry,
					   struct udf_fileident_bh *fibh,
					   struct fileIdentDesc *cfi, int *err)
{
	struct super_block *sb = dir->i_sb;
	struct fileIdentDesc *fi = NULL;
	unsigned char *name = NULL;
	int namelen;
	loff_t f_pos;
	loff_t size = udf_ext0_offset(dir) + dir->i_size;
	int nfidlen;
	uint8_t lfi;
	uint16_t liu;
	int block;
	struct kernel_lb_addr eloc;
	uint32_t elen = 0;
	sector_t offset;
	struct extent_position epos = {};
	struct udf_inode_info *dinfo;

	fibh->sbh = fibh->ebh = NULL;
	name = kmalloc(UDF_NAME_LEN, GFP_NOFS);
	if (!name) {
		*err = -ENOMEM;
		goto out_err;
	}

	if (dentry) {
		if (!dentry->d_name.len) {
			*err = -EINVAL;
			goto out_err;
		}
		namelen = udf_put_filename(sb, dentry->d_name.name, name,
						 dentry->d_name.len);
		if (!namelen) {
			*err = -ENAMETOOLONG;
			goto out_err;
		}
	} else {
		namelen = 0;
	}

	nfidlen = (sizeof(struct fileIdentDesc) + namelen + 3) & ~3;

	f_pos = udf_ext0_offset(dir);

	fibh->soffset = fibh->eoffset = f_pos & (dir->i_sb->s_blocksize - 1);
	dinfo = UDF_I(dir);
	if (dinfo->i_alloc_type != ICBTAG_FLAG_AD_IN_ICB) {
		if (inode_bmap(dir, f_pos >> dir->i_sb->s_blocksize_bits, &epos,
		    &eloc, &elen, &offset) != (EXT_RECORDED_ALLOCATED >> 30)) {
			block = udf_get_lb_pblock(dir->i_sb,
					&dinfo->i_location, 0);
			fibh->soffset = fibh->eoffset = sb->s_blocksize;
			goto add;
		}
		block = udf_get_lb_pblock(dir->i_sb, &eloc, offset);
		if ((++offset << dir->i_sb->s_blocksize_bits) < elen) {
			if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_SHORT)
				epos.offset -= sizeof(struct short_ad);
			else if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_LONG)
				epos.offset -= sizeof(struct long_ad);
		} else
			offset = 0;

		fibh->sbh = fibh->ebh = udf_tread(dir->i_sb, block);
		if (!fibh->sbh) {
			*err = -EIO;
			goto out_err;
		}

		block = dinfo->i_location.logicalBlockNum;
	}

	while (f_pos < size) {
		fi = udf_fileident_read(dir, &f_pos, fibh, cfi, &epos, &eloc,
					&elen, &offset);

		if (!fi) {
			*err = -EIO;
			goto out_err;
		}

		liu = le16_to_cpu(cfi->lengthOfImpUse);
		lfi = cfi->lengthFileIdent;

		if ((cfi->fileCharacteristics & FID_FILE_CHAR_DELETED) != 0) {
			if (((sizeof(struct fileIdentDesc) +
					liu + lfi + 3) & ~3) == nfidlen) {
				cfi->descTag.tagSerialNum = cpu_to_le16(1);
				cfi->fileVersionNum = cpu_to_le16(1);
				cfi->fileCharacteristics = 0;
				cfi->lengthFileIdent = namelen;
				cfi->lengthOfImpUse = cpu_to_le16(0);
				if (!udf_write_fi(dir, cfi, fi, fibh, NULL,
						  name))
					goto out_ok;
				else {
					*err = -EIO;
					goto out_err;
				}
			}
		}
	}

add:
	f_pos += nfidlen;

	if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB &&
	    sb->s_blocksize - fibh->eoffset < nfidlen) {
		brelse(epos.bh);
		epos.bh = NULL;
		fibh->soffset -= udf_ext0_offset(dir);
		fibh->eoffset -= udf_ext0_offset(dir);
		f_pos -= udf_ext0_offset(dir);
		if (fibh->sbh != fibh->ebh)
			brelse(fibh->ebh);
		brelse(fibh->sbh);
		fibh->sbh = fibh->ebh =
				udf_expand_dir_adinicb(dir, &block, err);
		if (!fibh->sbh)
			goto out_err;
		epos.block = dinfo->i_location;
		epos.offset = udf_file_entry_alloc_offset(dir);
		/* Load extent udf_expand_dir_adinicb() has created */
		udf_current_aext(dir, &epos, &eloc, &elen, 1);
	}

	/* Entry fits into current block? */
	if (sb->s_blocksize - fibh->eoffset >= nfidlen) {
		fibh->soffset = fibh->eoffset;
		fibh->eoffset += nfidlen;
		if (fibh->sbh != fibh->ebh) {
			brelse(fibh->sbh);
			fibh->sbh = fibh->ebh;
		}

		if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
			block = dinfo->i_location.logicalBlockNum;
			fi = (struct fileIdentDesc *)
					(dinfo->i_ext.i_data +
					 fibh->soffset -
					 udf_ext0_offset(dir) +
					 dinfo->i_lenEAttr);
		} else {
			block = eloc.logicalBlockNum +
					((elen - 1) >>
						dir->i_sb->s_blocksize_bits);
			fi = (struct fileIdentDesc *)
				(fibh->sbh->b_data + fibh->soffset);
		}
	} else {
		/* Round up last extent in the file */
		elen = (elen + sb->s_blocksize - 1) & ~(sb->s_blocksize - 1);
		if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_SHORT)
			epos.offset -= sizeof(struct short_ad);
		else if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_LONG)
			epos.offset -= sizeof(struct long_ad);
		udf_write_aext(dir, &epos, &eloc, elen, 1);
		dinfo->i_lenExtents = (dinfo->i_lenExtents + sb->s_blocksize
					- 1) & ~(sb->s_blocksize - 1);

		fibh->soffset = fibh->eoffset - sb->s_blocksize;
		fibh->eoffset += nfidlen - sb->s_blocksize;
		if (fibh->sbh != fibh->ebh) {
			brelse(fibh->sbh);
			fibh->sbh = fibh->ebh;
		}

		block = eloc.logicalBlockNum + ((elen - 1) >>
						dir->i_sb->s_blocksize_bits);
		fibh->ebh = udf_bread(dir,
				f_pos >> dir->i_sb->s_blocksize_bits, 1, err);
		if (!fibh->ebh)
			goto out_err;
		/* Extents could have been merged, invalidate our position */
		brelse(epos.bh);
		epos.bh = NULL;
		epos.block = dinfo->i_location;
		epos.offset = udf_file_entry_alloc_offset(dir);

		if (!fibh->soffset) {
			/* Find the freshly allocated block */
			while (udf_next_aext(dir, &epos, &eloc, &elen, 1) ==
				(EXT_RECORDED_ALLOCATED >> 30))
				;
			block = eloc.logicalBlockNum + ((elen - 1) >>
					dir->i_sb->s_blocksize_bits);
			brelse(fibh->sbh);
			fibh->sbh = fibh->ebh;
			fi = (struct fileIdentDesc *)(fibh->sbh->b_data);
		} else {
			fi = (struct fileIdentDesc *)
				(fibh->sbh->b_data + sb->s_blocksize +
					fibh->soffset);
		}
	}

	memset(cfi, 0, sizeof(struct fileIdentDesc));
	if (UDF_SB(sb)->s_udfrev >= 0x0200)
		udf_new_tag((char *)cfi, TAG_IDENT_FID, 3, 1, block,
			    sizeof(struct tag));
	else
		udf_new_tag((char *)cfi, TAG_IDENT_FID, 2, 1, block,
			    sizeof(struct tag));
	cfi->fileVersionNum = cpu_to_le16(1);
	cfi->lengthFileIdent = namelen;
	cfi->lengthOfImpUse = cpu_to_le16(0);
	if (!udf_write_fi(dir, cfi, fi, fibh, NULL, name)) {
		dir->i_size += nfidlen;
		if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
			dinfo->i_lenAlloc += nfidlen;
		else {
			/* Find the last extent and truncate it to proper size */
			while (udf_next_aext(dir, &epos, &eloc, &elen, 1) ==
				(EXT_RECORDED_ALLOCATED >> 30))
				;
			elen -= dinfo->i_lenExtents - dir->i_size;
			if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_SHORT)
				epos.offset -= sizeof(struct short_ad);
			else if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_LONG)
				epos.offset -= sizeof(struct long_ad);
			udf_write_aext(dir, &epos, &eloc, elen, 1);
			dinfo->i_lenExtents = dir->i_size;
		}

		mark_inode_dirty(dir);
		goto out_ok;
	} else {
		*err = -EIO;
		goto out_err;
	}

out_err:
	fi = NULL;
	if (fibh->sbh != fibh->ebh)
		brelse(fibh->ebh);
	brelse(fibh->sbh);
out_ok:
	brelse(epos.bh);
	kfree(name);
	return fi;
}

static int udf_delete_entry(struct inode *inode, struct fileIdentDesc *fi,
			    struct udf_fileident_bh *fibh,
			    struct fileIdentDesc *cfi)
{
	cfi->fileCharacteristics |= FID_FILE_CHAR_DELETED;

	if (UDF_QUERY_FLAG(inode->i_sb, UDF_FLAG_STRICT))
		memset(&(cfi->icb), 0x00, sizeof(struct long_ad));

	return udf_write_fi(inode, cfi, fi, fibh, NULL, NULL);
}

static int udf_create(struct inode *dir, struct dentry *dentry, umode_t mode,
		      bool excl)
{
	struct udf_fileident_bh fibh;
	struct inode *inode;
	struct fileIdentDesc cfi, *fi;
	int err;
	struct udf_inode_info *iinfo;

	inode = udf_new_inode(dir, mode, &err);
	if (!inode) {
		return err;
	}

	iinfo = UDF_I(inode);
	if (iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		inode->i_data.a_ops = &udf_adinicb_aops;
	else
		inode->i_data.a_ops = &udf_aops;
	inode->i_op = &udf_file_inode_operations;
	inode->i_fop = &udf_file_operations;
	mark_inode_dirty(inode);

	fi = udf_add_entry(dir, dentry, &fibh, &cfi, &err);
	if (!fi) {
		inode_dec_link_count(inode);
		iput(inode);
		return err;
	}
	cfi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
		cpu_to_le32(iinfo->i_unique & 0x00000000FFFFFFFFUL);
	udf_write_fi(dir, &cfi, fi, &fibh, NULL, NULL);
	if (UDF_I(dir)->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		mark_inode_dirty(dir);
	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	d_instantiate(dentry, inode);
=======
static int udf_expand_dir_adinicb(struct inode *inode, udf_pblk_t *block)
{
	udf_pblk_t newblock;
	struct buffer_head *dbh = NULL;
	struct kernel_lb_addr eloc;
	struct extent_position epos;
	uint8_t alloctype;
	struct udf_inode_info *iinfo = UDF_I(inode);
	struct udf_fileident_iter iter;
	uint8_t *impuse;
	int ret;

	if (UDF_QUERY_FLAG(inode->i_sb, UDF_FLAG_USE_SHORT_AD))
		alloctype = ICBTAG_FLAG_AD_SHORT;
	else
		alloctype = ICBTAG_FLAG_AD_LONG;

	if (!inode->i_size) {
		iinfo->i_alloc_type = alloctype;
		mark_inode_dirty(inode);
		return 0;
	}

	/* alloc block, and copy data to it */
	*block = udf_new_block(inode->i_sb, inode,
			       iinfo->i_location.partitionReferenceNum,
			       iinfo->i_location.logicalBlockNum, &ret);
	if (!(*block))
		return ret;
	newblock = udf_get_pblock(inode->i_sb, *block,
				  iinfo->i_location.partitionReferenceNum,
				0);
	if (newblock == 0xffffffff)
		return -EFSCORRUPTED;
	dbh = sb_getblk(inode->i_sb, newblock);
	if (!dbh)
		return -ENOMEM;
	lock_buffer(dbh);
	memcpy(dbh->b_data, iinfo->i_data, inode->i_size);
	memset(dbh->b_data + inode->i_size, 0,
	       inode->i_sb->s_blocksize - inode->i_size);
	set_buffer_uptodate(dbh);
	unlock_buffer(dbh);

	/* Drop inline data, add block instead */
	iinfo->i_alloc_type = alloctype;
	memset(iinfo->i_data + iinfo->i_lenEAttr, 0, iinfo->i_lenAlloc);
	iinfo->i_lenAlloc = 0;
	eloc.logicalBlockNum = *block;
	eloc.partitionReferenceNum =
				iinfo->i_location.partitionReferenceNum;
	iinfo->i_lenExtents = inode->i_size;
	epos.bh = NULL;
	epos.block = iinfo->i_location;
	epos.offset = udf_file_entry_alloc_offset(inode);
	ret = udf_add_aext(inode, &epos, &eloc, inode->i_size, 0);
	brelse(epos.bh);
	if (ret < 0) {
		brelse(dbh);
		udf_free_blocks(inode->i_sb, inode, &eloc, 0, 1);
		return ret;
	}
	mark_inode_dirty(inode);

	/* Now fixup tags in moved directory entries */
	for (ret = udf_fiiter_init(&iter, inode, 0);
	     !ret && iter.pos < inode->i_size;
	     ret = udf_fiiter_advance(&iter)) {
		iter.fi.descTag.tagLocation = cpu_to_le32(*block);
		if (iter.fi.lengthOfImpUse != cpu_to_le16(0))
			impuse = dbh->b_data + iter.pos +
						sizeof(struct fileIdentDesc);
		else
			impuse = NULL;
		udf_fiiter_write_fi(&iter, impuse);
	}
	brelse(dbh);
	/*
	 * We don't expect the iteration to fail as the directory has been
	 * already verified to be correct
	 */
	WARN_ON_ONCE(ret);
	udf_fiiter_release(&iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int udf_mknod(struct inode *dir, struct dentry *dentry, umode_t mode,
		     dev_t rdev)
{
	struct inode *inode;
	struct udf_fileident_bh fibh;
	struct fileIdentDesc cfi, *fi;
	int err;
	struct udf_inode_info *iinfo;
=======
static int udf_fiiter_add_entry(struct inode *dir, struct dentry *dentry,
				struct udf_fileident_iter *iter)
{
	struct udf_inode_info *dinfo = UDF_I(dir);
	int nfidlen, namelen = 0;
	int ret;
	int off, blksize = 1 << dir->i_blkbits;
	udf_pblk_t block;
	char name[UDF_NAME_LEN_CS0];

	if (dentry) {
		namelen = udf_put_filename(dir->i_sb, dentry->d_name.name,
					   dentry->d_name.len,
					   name, UDF_NAME_LEN_CS0);
		if (!namelen)
			return -ENAMETOOLONG;
	}
	nfidlen = ALIGN(sizeof(struct fileIdentDesc) + namelen, UDF_NAME_PAD);

	for (ret = udf_fiiter_init(iter, dir, 0);
	     !ret && iter->pos < dir->i_size;
	     ret = udf_fiiter_advance(iter)) {
		if (iter->fi.fileCharacteristics & FID_FILE_CHAR_DELETED) {
			if (udf_dir_entry_len(&iter->fi) == nfidlen) {
				iter->fi.descTag.tagSerialNum = cpu_to_le16(1);
				iter->fi.fileVersionNum = cpu_to_le16(1);
				iter->fi.fileCharacteristics = 0;
				iter->fi.lengthFileIdent = namelen;
				iter->fi.lengthOfImpUse = cpu_to_le16(0);
				memcpy(iter->namebuf, name, namelen);
				iter->name = iter->namebuf;
				return 0;
			}
		}
	}
	if (ret) {
		udf_fiiter_release(iter);
		return ret;
	}
	if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB &&
	    blksize - udf_ext0_offset(dir) - iter->pos < nfidlen) {
		udf_fiiter_release(iter);
		ret = udf_expand_dir_adinicb(dir, &block);
		if (ret)
			return ret;
		ret = udf_fiiter_init(iter, dir, dir->i_size);
		if (ret < 0)
			return ret;
	}

	/* Get blocknumber to use for entry tag */
	if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
		block = dinfo->i_location.logicalBlockNum;
	} else {
		block = iter->eloc.logicalBlockNum +
				((iter->elen - 1) >> dir->i_blkbits);
	}
	off = iter->pos & (blksize - 1);
	if (!off)
		off = blksize;
	/* Entry fits into current block? */
	if (blksize - udf_ext0_offset(dir) - off >= nfidlen)
		goto store_fi;

	ret = udf_fiiter_append_blk(iter);
	if (ret) {
		udf_fiiter_release(iter);
		return ret;
	}

	/* Entry will be completely in the new block? Update tag location... */
	if (!(iter->pos & (blksize - 1)))
		block = iter->eloc.logicalBlockNum +
				((iter->elen - 1) >> dir->i_blkbits);
store_fi:
	memset(&iter->fi, 0, sizeof(struct fileIdentDesc));
	if (UDF_SB(dir->i_sb)->s_udfrev >= 0x0200)
		udf_new_tag((char *)(&iter->fi), TAG_IDENT_FID, 3, 1, block,
			    sizeof(struct tag));
	else
		udf_new_tag((char *)(&iter->fi), TAG_IDENT_FID, 2, 1, block,
			    sizeof(struct tag));
	iter->fi.fileVersionNum = cpu_to_le16(1);
	iter->fi.lengthFileIdent = namelen;
	iter->fi.lengthOfImpUse = cpu_to_le16(0);
	memcpy(iter->namebuf, name, namelen);
	iter->name = iter->namebuf;

	dir->i_size += nfidlen;
	if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
		dinfo->i_lenAlloc += nfidlen;
	} else {
		/* Truncate last extent to proper size */
		udf_fiiter_update_elen(iter, iter->elen -
					(dinfo->i_lenExtents - dir->i_size));
	}
	mark_inode_dirty(dir);

	return 0;
}

static void udf_fiiter_delete_entry(struct udf_fileident_iter *iter)
{
	iter->fi.fileCharacteristics |= FID_FILE_CHAR_DELETED;

	if (UDF_QUERY_FLAG(iter->dir->i_sb, UDF_FLAG_STRICT))
		memset(&iter->fi.icb, 0x00, sizeof(struct long_ad));

	udf_fiiter_write_fi(iter, NULL);
}

static void udf_add_fid_counter(struct super_block *sb, bool dir, int val)
{
	struct logicalVolIntegrityDescImpUse *lvidiu = udf_sb_lvidiu(sb);

	if (!lvidiu)
		return;
	mutex_lock(&UDF_SB(sb)->s_alloc_mutex);
	if (dir)
		le32_add_cpu(&lvidiu->numDirs, val);
	else
		le32_add_cpu(&lvidiu->numFiles, val);
	udf_updated_lvid(sb);
	mutex_unlock(&UDF_SB(sb)->s_alloc_mutex);
}

static int udf_add_nondir(struct dentry *dentry, struct inode *inode)
{
	struct udf_inode_info *iinfo = UDF_I(inode);
	struct inode *dir = d_inode(dentry->d_parent);
	struct udf_fileident_iter iter;
	int err;

	err = udf_fiiter_add_entry(dir, dentry, &iter);
	if (err) {
		inode_dec_link_count(inode);
		discard_new_inode(inode);
		return err;
	}
	iter.fi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	iter.fi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)iter.fi.icb.impUse)->impUse =
		cpu_to_le32(iinfo->i_unique & 0x00000000FFFFFFFFUL);
	udf_fiiter_write_fi(&iter, NULL);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	udf_fiiter_release(&iter);
	udf_add_fid_counter(dir->i_sb, false, 1);
	d_instantiate_new(dentry, inode);

	return 0;
}

static int udf_create(struct mnt_idmap *idmap, struct inode *dir,
		      struct dentry *dentry, umode_t mode, bool excl)
{
	struct inode *inode = udf_new_inode(dir, mode);

	if (IS_ERR(inode))
		return PTR_ERR(inode);

	inode->i_data.a_ops = &udf_aops;
	inode->i_op = &udf_file_inode_operations;
	inode->i_fop = &udf_file_operations;
	mark_inode_dirty(inode);

	return udf_add_nondir(dentry, inode);
}

static int udf_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
		       struct file *file, umode_t mode)
{
	struct inode *inode = udf_new_inode(dir, mode);

	if (IS_ERR(inode))
		return PTR_ERR(inode);

	inode->i_data.a_ops = &udf_aops;
	inode->i_op = &udf_file_inode_operations;
	inode->i_fop = &udf_file_operations;
	mark_inode_dirty(inode);
	d_tmpfile(file, inode);
	unlock_new_inode(inode);
	return finish_open_simple(file, 0);
}

static int udf_mknod(struct mnt_idmap *idmap, struct inode *dir,
		     struct dentry *dentry, umode_t mode, dev_t rdev)
{
	struct inode *inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!old_valid_dev(rdev))
		return -EINVAL;

<<<<<<< HEAD
	err = -EIO;
	inode = udf_new_inode(dir, mode, &err);
	if (!inode)
		goto out;

	iinfo = UDF_I(inode);
	init_special_inode(inode, mode, rdev);
	fi = udf_add_entry(dir, dentry, &fibh, &cfi, &err);
	if (!fi) {
		inode_dec_link_count(inode);
		iput(inode);
		return err;
	}
	cfi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
		cpu_to_le32(iinfo->i_unique & 0x00000000FFFFFFFFUL);
	udf_write_fi(dir, &cfi, fi, &fibh, NULL, NULL);
	if (UDF_I(dir)->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		mark_inode_dirty(dir);
	mark_inode_dirty(inode);

	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	d_instantiate(dentry, inode);
	err = 0;

out:
	return err;
}

static int udf_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	struct inode *inode;
	struct udf_fileident_bh fibh;
	struct fileIdentDesc cfi, *fi;
=======
	inode = udf_new_inode(dir, mode);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	init_special_inode(inode, mode, rdev);
	return udf_add_nondir(dentry, inode);
}

static int udf_mkdir(struct mnt_idmap *idmap, struct inode *dir,
		     struct dentry *dentry, umode_t mode)
{
	struct inode *inode;
	struct udf_fileident_iter iter;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;
	struct udf_inode_info *dinfo = UDF_I(dir);
	struct udf_inode_info *iinfo;

<<<<<<< HEAD
	err = -EIO;
	inode = udf_new_inode(dir, S_IFDIR | mode, &err);
	if (!inode)
		goto out;
=======
	inode = udf_new_inode(dir, S_IFDIR | mode);
	if (IS_ERR(inode))
		return PTR_ERR(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	iinfo = UDF_I(inode);
	inode->i_op = &udf_dir_inode_operations;
	inode->i_fop = &udf_dir_operations;
<<<<<<< HEAD
	fi = udf_add_entry(inode, NULL, &fibh, &cfi, &err);
	if (!fi) {
		inode_dec_link_count(inode);
		iput(inode);
		goto out;
	}
	set_nlink(inode, 2);
	cfi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(dinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
		cpu_to_le32(dinfo->i_unique & 0x00000000FFFFFFFFUL);
	cfi.fileCharacteristics =
			FID_FILE_CHAR_DIRECTORY | FID_FILE_CHAR_PARENT;
	udf_write_fi(inode, &cfi, fi, &fibh, NULL, NULL);
	brelse(fibh.sbh);
	mark_inode_dirty(inode);

	fi = udf_add_entry(dir, dentry, &fibh, &cfi, &err);
	if (!fi) {
		clear_nlink(inode);
		mark_inode_dirty(inode);
		iput(inode);
		goto out;
	}
	cfi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
		cpu_to_le32(iinfo->i_unique & 0x00000000FFFFFFFFUL);
	cfi.fileCharacteristics |= FID_FILE_CHAR_DIRECTORY;
	udf_write_fi(dir, &cfi, fi, &fibh, NULL, NULL);
	inc_nlink(dir);
	mark_inode_dirty(dir);
	d_instantiate(dentry, inode);
	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	err = 0;

out:
	return err;
=======
	err = udf_fiiter_add_entry(inode, NULL, &iter);
	if (err) {
		clear_nlink(inode);
		discard_new_inode(inode);
		return err;
	}
	set_nlink(inode, 2);
	iter.fi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	iter.fi.icb.extLocation = cpu_to_lelb(dinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)iter.fi.icb.impUse)->impUse =
		cpu_to_le32(dinfo->i_unique & 0x00000000FFFFFFFFUL);
	iter.fi.fileCharacteristics =
			FID_FILE_CHAR_DIRECTORY | FID_FILE_CHAR_PARENT;
	udf_fiiter_write_fi(&iter, NULL);
	udf_fiiter_release(&iter);
	mark_inode_dirty(inode);

	err = udf_fiiter_add_entry(dir, dentry, &iter);
	if (err) {
		clear_nlink(inode);
		discard_new_inode(inode);
		return err;
	}
	iter.fi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	iter.fi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	*(__le32 *)((struct allocDescImpUse *)iter.fi.icb.impUse)->impUse =
		cpu_to_le32(iinfo->i_unique & 0x00000000FFFFFFFFUL);
	iter.fi.fileCharacteristics |= FID_FILE_CHAR_DIRECTORY;
	udf_fiiter_write_fi(&iter, NULL);
	udf_fiiter_release(&iter);
	udf_add_fid_counter(dir->i_sb, true, 1);
	inc_nlink(dir);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	d_instantiate_new(dentry, inode);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int empty_dir(struct inode *dir)
{
<<<<<<< HEAD
	struct fileIdentDesc *fi, cfi;
	struct udf_fileident_bh fibh;
	loff_t f_pos;
	loff_t size = udf_ext0_offset(dir) + dir->i_size;
	int block;
	struct kernel_lb_addr eloc;
	uint32_t elen;
	sector_t offset;
	struct extent_position epos = {};
	struct udf_inode_info *dinfo = UDF_I(dir);

	f_pos = udf_ext0_offset(dir);
	fibh.soffset = fibh.eoffset = f_pos & (dir->i_sb->s_blocksize - 1);

	if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		fibh.sbh = fibh.ebh = NULL;
	else if (inode_bmap(dir, f_pos >> dir->i_sb->s_blocksize_bits,
			      &epos, &eloc, &elen, &offset) ==
					(EXT_RECORDED_ALLOCATED >> 30)) {
		block = udf_get_lb_pblock(dir->i_sb, &eloc, offset);
		if ((++offset << dir->i_sb->s_blocksize_bits) < elen) {
			if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_SHORT)
				epos.offset -= sizeof(struct short_ad);
			else if (dinfo->i_alloc_type == ICBTAG_FLAG_AD_LONG)
				epos.offset -= sizeof(struct long_ad);
		} else
			offset = 0;

		fibh.sbh = fibh.ebh = udf_tread(dir->i_sb, block);
		if (!fibh.sbh) {
			brelse(epos.bh);
			return 0;
		}
	} else {
		brelse(epos.bh);
		return 0;
	}

	while (f_pos < size) {
		fi = udf_fileident_read(dir, &f_pos, &fibh, &cfi, &epos, &eloc,
					&elen, &offset);
		if (!fi) {
			if (fibh.sbh != fibh.ebh)
				brelse(fibh.ebh);
			brelse(fibh.sbh);
			brelse(epos.bh);
			return 0;
		}

		if (cfi.lengthFileIdent &&
		    (cfi.fileCharacteristics & FID_FILE_CHAR_DELETED) == 0) {
			if (fibh.sbh != fibh.ebh)
				brelse(fibh.ebh);
			brelse(fibh.sbh);
			brelse(epos.bh);
			return 0;
		}
	}

	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	brelse(epos.bh);
=======
	struct udf_fileident_iter iter;
	int ret;

	for (ret = udf_fiiter_init(&iter, dir, 0);
	     !ret && iter.pos < dir->i_size;
	     ret = udf_fiiter_advance(&iter)) {
		if (iter.fi.lengthFileIdent &&
		    !(iter.fi.fileCharacteristics & FID_FILE_CHAR_DELETED)) {
			udf_fiiter_release(&iter);
			return 0;
		}
	}
	udf_fiiter_release(&iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

static int udf_rmdir(struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	int retval;
	struct inode *inode = dentry->d_inode;
	struct udf_fileident_bh fibh;
	struct fileIdentDesc *fi, cfi;
	struct kernel_lb_addr tloc;

	retval = -ENOENT;
	fi = udf_find_entry(dir, &dentry->d_name, &fibh, &cfi);
	if (!fi)
		goto out;

	retval = -EIO;
	tloc = lelb_to_cpu(cfi.icb.extLocation);
	if (udf_get_lb_pblock(dir->i_sb, &tloc, 0) != inode->i_ino)
		goto end_rmdir;
	retval = -ENOTEMPTY;
	if (!empty_dir(inode))
		goto end_rmdir;
	retval = udf_delete_entry(dir, fi, &fibh, &cfi);
	if (retval)
		goto end_rmdir;
	if (inode->i_nlink != 2)
		udf_warn(inode->i_sb, "empty directory has nlink != 2 (%d)\n",
=======
	int ret;
	struct inode *inode = d_inode(dentry);
	struct udf_fileident_iter iter;
	struct kernel_lb_addr tloc;

	ret = udf_fiiter_find_entry(dir, &dentry->d_name, &iter);
	if (ret)
		goto out;

	ret = -EFSCORRUPTED;
	tloc = lelb_to_cpu(iter.fi.icb.extLocation);
	if (udf_get_lb_pblock(dir->i_sb, &tloc, 0) != inode->i_ino)
		goto end_rmdir;
	ret = -ENOTEMPTY;
	if (!empty_dir(inode))
		goto end_rmdir;
	udf_fiiter_delete_entry(&iter);
	if (inode->i_nlink != 2)
		udf_warn(inode->i_sb, "empty directory has nlink != 2 (%u)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 inode->i_nlink);
	clear_nlink(inode);
	inode->i_size = 0;
	inode_dec_link_count(dir);
<<<<<<< HEAD
	inode->i_ctime = dir->i_ctime = dir->i_mtime =
						current_fs_time(dir->i_sb);
	mark_inode_dirty(dir);

end_rmdir:
	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);

out:
	return retval;
=======
	udf_add_fid_counter(dir->i_sb, true, -1);
	inode_set_mtime_to_ts(dir,
			      inode_set_ctime_to_ts(dir, inode_set_ctime_current(inode)));
	mark_inode_dirty(dir);
	ret = 0;
end_rmdir:
	udf_fiiter_release(&iter);
out:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int udf_unlink(struct inode *dir, struct dentry *dentry)
{
<<<<<<< HEAD
	int retval;
	struct inode *inode = dentry->d_inode;
	struct udf_fileident_bh fibh;
	struct fileIdentDesc *fi;
	struct fileIdentDesc cfi;
	struct kernel_lb_addr tloc;

	retval = -ENOENT;
	fi = udf_find_entry(dir, &dentry->d_name, &fibh, &cfi);
	if (!fi)
		goto out;

	retval = -EIO;
	tloc = lelb_to_cpu(cfi.icb.extLocation);
=======
	int ret;
	struct inode *inode = d_inode(dentry);
	struct udf_fileident_iter iter;
	struct kernel_lb_addr tloc;

	ret = udf_fiiter_find_entry(dir, &dentry->d_name, &iter);
	if (ret)
		goto out;

	ret = -EFSCORRUPTED;
	tloc = lelb_to_cpu(iter.fi.icb.extLocation);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (udf_get_lb_pblock(dir->i_sb, &tloc, 0) != inode->i_ino)
		goto end_unlink;

	if (!inode->i_nlink) {
<<<<<<< HEAD
		udf_debug("Deleting nonexistent file (%lu), %d\n",
			  inode->i_ino, inode->i_nlink);
		set_nlink(inode, 1);
	}
	retval = udf_delete_entry(dir, fi, &fibh, &cfi);
	if (retval)
		goto end_unlink;
	dir->i_ctime = dir->i_mtime = current_fs_time(dir->i_sb);
	mark_inode_dirty(dir);
	inode_dec_link_count(inode);
	inode->i_ctime = dir->i_ctime;
	retval = 0;

end_unlink:
	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);

out:
	return retval;
}

static int udf_symlink(struct inode *dir, struct dentry *dentry,
		       const char *symname)
=======
		udf_debug("Deleting nonexistent file (%lu), %u\n",
			  inode->i_ino, inode->i_nlink);
		set_nlink(inode, 1);
	}
	udf_fiiter_delete_entry(&iter);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	inode_dec_link_count(inode);
	udf_add_fid_counter(dir->i_sb, false, -1);
	inode_set_ctime_to_ts(inode, inode_get_ctime(dir));
	ret = 0;
end_unlink:
	udf_fiiter_release(&iter);
out:
	return ret;
}

static int udf_symlink(struct mnt_idmap *idmap, struct inode *dir,
		       struct dentry *dentry, const char *symname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode;
	struct pathComponent *pc;
	const char *compstart;
<<<<<<< HEAD
	struct udf_fileident_bh fibh;
	struct extent_position epos = {};
	int eoffset, elen = 0;
	struct fileIdentDesc *fi;
	struct fileIdentDesc cfi;
	uint8_t *ea;
	int err;
	int block;
=======
	struct extent_position epos = {};
	int eoffset, elen = 0;
	uint8_t *ea;
	int err;
	udf_pblk_t block;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char *name = NULL;
	int namelen;
	struct udf_inode_info *iinfo;
	struct super_block *sb = dir->i_sb;

<<<<<<< HEAD
	inode = udf_new_inode(dir, S_IFLNK | S_IRWXUGO, &err);
	if (!inode)
		goto out;

	iinfo = UDF_I(inode);
	down_write(&iinfo->i_data_sem);
	name = kmalloc(UDF_NAME_LEN, GFP_NOFS);
	if (!name) {
		err = -ENOMEM;
		goto out_no_entry;
	}

	inode->i_data.a_ops = &udf_symlink_aops;
	inode->i_op = &udf_symlink_inode_operations;
=======
	name = kmalloc(UDF_NAME_LEN_CS0, GFP_KERNEL);
	if (!name) {
		err = -ENOMEM;
		goto out;
	}

	inode = udf_new_inode(dir, S_IFLNK | 0777);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}

	iinfo = UDF_I(inode);
	down_write(&iinfo->i_data_sem);
	inode->i_data.a_ops = &udf_symlink_aops;
	inode->i_op = &udf_symlink_inode_operations;
	inode_nohighmem(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (iinfo->i_alloc_type != ICBTAG_FLAG_AD_IN_ICB) {
		struct kernel_lb_addr eloc;
		uint32_t bsize;

		block = udf_new_block(sb, inode,
				iinfo->i_location.partitionReferenceNum,
				iinfo->i_location.logicalBlockNum, &err);
		if (!block)
			goto out_no_entry;
		epos.block = iinfo->i_location;
		epos.offset = udf_file_entry_alloc_offset(inode);
		epos.bh = NULL;
		eloc.logicalBlockNum = block;
		eloc.partitionReferenceNum =
				iinfo->i_location.partitionReferenceNum;
		bsize = sb->s_blocksize;
		iinfo->i_lenExtents = bsize;
<<<<<<< HEAD
		udf_add_aext(inode, &epos, &eloc, bsize, 0);
		brelse(epos.bh);
=======
		err = udf_add_aext(inode, &epos, &eloc, bsize, 0);
		brelse(epos.bh);
		if (err < 0) {
			udf_free_blocks(sb, inode, &eloc, 0, 1);
			goto out_no_entry;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		block = udf_get_pblock(sb, block,
				iinfo->i_location.partitionReferenceNum,
				0);
<<<<<<< HEAD
		epos.bh = udf_tgetblk(sb, block);
=======
		epos.bh = sb_getblk(sb, block);
		if (unlikely(!epos.bh)) {
			err = -ENOMEM;
			udf_free_blocks(sb, inode, &eloc, 0, 1);
			goto out_no_entry;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		lock_buffer(epos.bh);
		memset(epos.bh->b_data, 0x00, bsize);
		set_buffer_uptodate(epos.bh);
		unlock_buffer(epos.bh);
		mark_buffer_dirty_inode(epos.bh, inode);
		ea = epos.bh->b_data + udf_ext0_offset(inode);
	} else
<<<<<<< HEAD
		ea = iinfo->i_ext.i_data + iinfo->i_lenEAttr;
=======
		ea = iinfo->i_data + iinfo->i_lenEAttr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	eoffset = sb->s_blocksize - udf_ext0_offset(inode);
	pc = (struct pathComponent *)ea;

	if (*symname == '/') {
		do {
			symname++;
		} while (*symname == '/');

		pc->componentType = 1;
		pc->lengthComponentIdent = 0;
		pc->componentFileVersionNum = 0;
		elen += sizeof(struct pathComponent);
	}

	err = -ENAMETOOLONG;

	while (*symname) {
		if (elen + sizeof(struct pathComponent) > eoffset)
			goto out_no_entry;

		pc = (struct pathComponent *)(ea + elen);

		compstart = symname;

		do {
			symname++;
		} while (*symname && *symname != '/');

		pc->componentType = 5;
		pc->lengthComponentIdent = 0;
		pc->componentFileVersionNum = 0;
		if (compstart[0] == '.') {
			if ((symname - compstart) == 1)
				pc->componentType = 4;
			else if ((symname - compstart) == 2 &&
					compstart[1] == '.')
				pc->componentType = 3;
		}

		if (pc->componentType == 5) {
<<<<<<< HEAD
			namelen = udf_put_filename(sb, compstart, name,
						   symname - compstart);
=======
			namelen = udf_put_filename(sb, compstart,
						   symname - compstart,
						   name, UDF_NAME_LEN_CS0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!namelen)
				goto out_no_entry;

			if (elen + sizeof(struct pathComponent) + namelen >
					eoffset)
				goto out_no_entry;
			else
				pc->lengthComponentIdent = namelen;

			memcpy(pc->componentIdent, name, namelen);
		}

		elen += sizeof(struct pathComponent) + pc->lengthComponentIdent;

		if (*symname) {
			do {
				symname++;
			} while (*symname == '/');
		}
	}

	brelse(epos.bh);
	inode->i_size = elen;
	if (iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		iinfo->i_lenAlloc = inode->i_size;
	else
		udf_truncate_tail_extent(inode);
	mark_inode_dirty(inode);
<<<<<<< HEAD

	fi = udf_add_entry(dir, dentry, &fibh, &cfi, &err);
	if (!fi)
		goto out_no_entry;
	cfi.icb.extLength = cpu_to_le32(sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(iinfo->i_location);
	if (UDF_SB(inode->i_sb)->s_lvid_bh) {
		*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
			cpu_to_le32(lvid_get_unique_id(sb));
	}
	udf_write_fi(dir, &cfi, fi, &fibh, NULL, NULL);
	if (UDF_I(dir)->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		mark_inode_dirty(dir);
	up_write(&iinfo->i_data_sem);
	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	d_instantiate(dentry, inode);
	err = 0;

=======
	up_write(&iinfo->i_data_sem);

	err = udf_add_nondir(dentry, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(name);
	return err;

out_no_entry:
	up_write(&iinfo->i_data_sem);
	inode_dec_link_count(inode);
<<<<<<< HEAD
	iput(inode);
=======
	discard_new_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	goto out;
}

static int udf_link(struct dentry *old_dentry, struct inode *dir,
		    struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = old_dentry->d_inode;
	struct udf_fileident_bh fibh;
	struct fileIdentDesc cfi, *fi;
	int err;

	fi = udf_add_entry(dir, dentry, &fibh, &cfi, &err);
	if (!fi) {
		return err;
	}
	cfi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	cfi.icb.extLocation = cpu_to_lelb(UDF_I(inode)->i_location);
	if (UDF_SB(inode->i_sb)->s_lvid_bh) {
		*(__le32 *)((struct allocDescImpUse *)cfi.icb.impUse)->impUse =
			cpu_to_le32(lvid_get_unique_id(inode->i_sb));
	}
	udf_write_fi(dir, &cfi, fi, &fibh, NULL, NULL);
	if (UDF_I(dir)->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
		mark_inode_dirty(dir);

	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);
	inc_nlink(inode);
	inode->i_ctime = current_fs_time(inode->i_sb);
	mark_inode_dirty(inode);
=======
	struct inode *inode = d_inode(old_dentry);
	struct udf_fileident_iter iter;
	int err;

	err = udf_fiiter_add_entry(dir, dentry, &iter);
	if (err)
		return err;
	iter.fi.icb.extLength = cpu_to_le32(inode->i_sb->s_blocksize);
	iter.fi.icb.extLocation = cpu_to_lelb(UDF_I(inode)->i_location);
	if (UDF_SB(inode->i_sb)->s_lvid_bh) {
		*(__le32 *)((struct allocDescImpUse *)iter.fi.icb.impUse)->impUse =
			cpu_to_le32(lvid_get_unique_id(inode->i_sb));
	}
	udf_fiiter_write_fi(&iter, NULL);
	udf_fiiter_release(&iter);

	inc_nlink(inode);
	udf_add_fid_counter(dir->i_sb, false, 1);
	inode_set_ctime_current(inode);
	mark_inode_dirty(inode);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ihold(inode);
	d_instantiate(dentry, inode);

	return 0;
}

/* Anybody can rename anything with this: the permission checks are left to the
 * higher-level routines.
 */
<<<<<<< HEAD
static int udf_rename(struct inode *old_dir, struct dentry *old_dentry,
		      struct inode *new_dir, struct dentry *new_dentry)
{
	struct inode *old_inode = old_dentry->d_inode;
	struct inode *new_inode = new_dentry->d_inode;
	struct udf_fileident_bh ofibh, nfibh;
	struct fileIdentDesc *ofi = NULL, *nfi = NULL, *dir_fi = NULL;
	struct fileIdentDesc ocfi, ncfi;
	struct buffer_head *dir_bh = NULL;
	int retval = -ENOENT;
	struct kernel_lb_addr tloc;
	struct udf_inode_info *old_iinfo = UDF_I(old_inode);

	ofi = udf_find_entry(old_dir, &old_dentry->d_name, &ofibh, &ocfi);
	if (ofi) {
		if (ofibh.sbh != ofibh.ebh)
			brelse(ofibh.ebh);
		brelse(ofibh.sbh);
	}
	tloc = lelb_to_cpu(ocfi.icb.extLocation);
	if (!ofi || udf_get_lb_pblock(old_dir->i_sb, &tloc, 0)
	    != old_inode->i_ino)
		goto end_rename;

	nfi = udf_find_entry(new_dir, &new_dentry->d_name, &nfibh, &ncfi);
	if (nfi) {
		if (!new_inode) {
			if (nfibh.sbh != nfibh.ebh)
				brelse(nfibh.ebh);
			brelse(nfibh.sbh);
			nfi = NULL;
		}
	}
	if (S_ISDIR(old_inode->i_mode)) {
		int offset = udf_ext0_offset(old_inode);

		if (new_inode) {
			retval = -ENOTEMPTY;
			if (!empty_dir(new_inode))
				goto end_rename;
		}
		retval = -EIO;
		if (old_iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB) {
			dir_fi = udf_get_fileident(
					old_iinfo->i_ext.i_data -
					  (old_iinfo->i_efe ?
					   sizeof(struct extendedFileEntry) :
					   sizeof(struct fileEntry)),
					old_inode->i_sb->s_blocksize, &offset);
		} else {
			dir_bh = udf_bread(old_inode, 0, 0, &retval);
			if (!dir_bh)
				goto end_rename;
			dir_fi = udf_get_fileident(dir_bh->b_data,
					old_inode->i_sb->s_blocksize, &offset);
		}
		if (!dir_fi)
			goto end_rename;
		tloc = lelb_to_cpu(dir_fi->icb.extLocation);
		if (udf_get_lb_pblock(old_inode->i_sb, &tloc, 0) !=
				old_dir->i_ino)
			goto end_rename;
	}
	if (!nfi) {
		nfi = udf_add_entry(new_dir, new_dentry, &nfibh, &ncfi,
				    &retval);
		if (!nfi)
			goto end_rename;
=======
static int udf_rename(struct mnt_idmap *idmap, struct inode *old_dir,
		      struct dentry *old_dentry, struct inode *new_dir,
		      struct dentry *new_dentry, unsigned int flags)
{
	struct inode *old_inode = d_inode(old_dentry);
	struct inode *new_inode = d_inode(new_dentry);
	struct udf_fileident_iter oiter, niter, diriter;
	bool has_diriter = false, is_dir = false;
	int retval;
	struct kernel_lb_addr tloc;

	if (flags & ~RENAME_NOREPLACE)
		return -EINVAL;

	retval = udf_fiiter_find_entry(old_dir, &old_dentry->d_name, &oiter);
	if (retval)
		return retval;

	tloc = lelb_to_cpu(oiter.fi.icb.extLocation);
	if (udf_get_lb_pblock(old_dir->i_sb, &tloc, 0) != old_inode->i_ino) {
		retval = -ENOENT;
		goto out_oiter;
	}

	if (S_ISDIR(old_inode->i_mode)) {
		if (new_inode) {
			retval = -ENOTEMPTY;
			if (!empty_dir(new_inode))
				goto out_oiter;
		}
		is_dir = true;
	}
	if (is_dir && old_dir != new_dir) {
		retval = udf_fiiter_find_entry(old_inode, &dotdot_name,
					       &diriter);
		if (retval == -ENOENT) {
			udf_err(old_inode->i_sb,
				"directory (ino %lu) has no '..' entry\n",
				old_inode->i_ino);
			retval = -EFSCORRUPTED;
		}
		if (retval)
			goto out_oiter;
		has_diriter = true;
		tloc = lelb_to_cpu(diriter.fi.icb.extLocation);
		if (udf_get_lb_pblock(old_inode->i_sb, &tloc, 0) !=
				old_dir->i_ino) {
			retval = -EFSCORRUPTED;
			udf_err(old_inode->i_sb,
				"directory (ino %lu) has parent entry pointing to another inode (%lu != %u)\n",
				old_inode->i_ino, old_dir->i_ino,
				udf_get_lb_pblock(old_inode->i_sb, &tloc, 0));
			goto out_oiter;
		}
	}

	retval = udf_fiiter_find_entry(new_dir, &new_dentry->d_name, &niter);
	if (retval && retval != -ENOENT)
		goto out_oiter;
	/* Entry found but not passed by VFS? */
	if (!retval && !new_inode) {
		retval = -EFSCORRUPTED;
		udf_fiiter_release(&niter);
		goto out_oiter;
	}
	/* Entry not found? Need to add one... */
	if (retval) {
		udf_fiiter_release(&niter);
		retval = udf_fiiter_add_entry(new_dir, new_dentry, &niter);
		if (retval)
			goto out_oiter;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Like most other Unix systems, set the ctime for inodes on a
	 * rename.
	 */
<<<<<<< HEAD
	old_inode->i_ctime = current_fs_time(old_inode->i_sb);
=======
	inode_set_ctime_current(old_inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mark_inode_dirty(old_inode);

	/*
	 * ok, that's it
	 */
<<<<<<< HEAD
	ncfi.fileVersionNum = ocfi.fileVersionNum;
	ncfi.fileCharacteristics = ocfi.fileCharacteristics;
	memcpy(&(ncfi.icb), &(ocfi.icb), sizeof(struct long_ad));
	udf_write_fi(new_dir, &ncfi, nfi, &nfibh, NULL, NULL);

	/* The old fid may have moved - find it again */
	ofi = udf_find_entry(old_dir, &old_dentry->d_name, &ofibh, &ocfi);
	udf_delete_entry(old_dir, ofi, &ofibh, &ocfi);

	if (new_inode) {
		new_inode->i_ctime = current_fs_time(new_inode->i_sb);
		inode_dec_link_count(new_inode);
	}
	old_dir->i_ctime = old_dir->i_mtime = current_fs_time(old_dir->i_sb);
	mark_inode_dirty(old_dir);

	if (dir_fi) {
		dir_fi->icb.extLocation = cpu_to_lelb(UDF_I(new_dir)->i_location);
		udf_update_tag((char *)dir_fi,
				(sizeof(struct fileIdentDesc) +
				le16_to_cpu(dir_fi->lengthOfImpUse) + 3) & ~3);
		if (old_iinfo->i_alloc_type == ICBTAG_FLAG_AD_IN_ICB)
			mark_inode_dirty(old_inode);
		else
			mark_buffer_dirty_inode(dir_bh, old_inode);

=======
	niter.fi.fileVersionNum = oiter.fi.fileVersionNum;
	niter.fi.fileCharacteristics = oiter.fi.fileCharacteristics;
	memcpy(&(niter.fi.icb), &(oiter.fi.icb), sizeof(oiter.fi.icb));
	udf_fiiter_write_fi(&niter, NULL);
	udf_fiiter_release(&niter);

	/*
	 * The old entry may have moved due to new entry allocation. Find it
	 * again.
	 */
	udf_fiiter_release(&oiter);
	retval = udf_fiiter_find_entry(old_dir, &old_dentry->d_name, &oiter);
	if (retval) {
		udf_err(old_dir->i_sb,
			"failed to find renamed entry again in directory (ino %lu)\n",
			old_dir->i_ino);
	} else {
		udf_fiiter_delete_entry(&oiter);
		udf_fiiter_release(&oiter);
	}

	if (new_inode) {
		inode_set_ctime_current(new_inode);
		inode_dec_link_count(new_inode);
		udf_add_fid_counter(old_dir->i_sb, S_ISDIR(new_inode->i_mode),
				    -1);
	}
	inode_set_mtime_to_ts(old_dir, inode_set_ctime_current(old_dir));
	inode_set_mtime_to_ts(new_dir, inode_set_ctime_current(new_dir));
	mark_inode_dirty(old_dir);
	mark_inode_dirty(new_dir);

	if (has_diriter) {
		diriter.fi.icb.extLocation =
					cpu_to_lelb(UDF_I(new_dir)->i_location);
		udf_update_tag((char *)&diriter.fi,
			       udf_dir_entry_len(&diriter.fi));
		udf_fiiter_write_fi(&diriter, NULL);
		udf_fiiter_release(&diriter);
	}

	if (is_dir) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inode_dec_link_count(old_dir);
		if (new_inode)
			inode_dec_link_count(new_inode);
		else {
			inc_nlink(new_dir);
			mark_inode_dirty(new_dir);
		}
	}
<<<<<<< HEAD

	if (ofi) {
		if (ofibh.sbh != ofibh.ebh)
			brelse(ofibh.ebh);
		brelse(ofibh.sbh);
	}

	retval = 0;

end_rename:
	brelse(dir_bh);
	if (nfi) {
		if (nfibh.sbh != nfibh.ebh)
			brelse(nfibh.ebh);
		brelse(nfibh.sbh);
	}
=======
	return 0;
out_oiter:
	if (has_diriter)
		udf_fiiter_release(&diriter);
	udf_fiiter_release(&oiter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return retval;
}

static struct dentry *udf_get_parent(struct dentry *child)
{
	struct kernel_lb_addr tloc;
<<<<<<< HEAD
	struct inode *inode = NULL;
	struct qstr dotdot = QSTR_INIT("..", 2);
	struct fileIdentDesc cfi;
	struct udf_fileident_bh fibh;

	if (!udf_find_entry(child->d_inode, &dotdot, &fibh, &cfi))
		goto out_unlock;

	if (fibh.sbh != fibh.ebh)
		brelse(fibh.ebh);
	brelse(fibh.sbh);

	tloc = lelb_to_cpu(cfi.icb.extLocation);
	inode = udf_iget(child->d_inode->i_sb, &tloc);
	if (!inode)
		goto out_unlock;

	return d_obtain_alias(inode);
out_unlock:
	return ERR_PTR(-EACCES);
=======
	struct udf_fileident_iter iter;
	int err;

	err = udf_fiiter_find_entry(d_inode(child), &dotdot_name, &iter);
	if (err)
		return ERR_PTR(err);

	tloc = lelb_to_cpu(iter.fi.icb.extLocation);
	udf_fiiter_release(&iter);
	return d_obtain_alias(udf_iget(child->d_sb, &tloc));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static struct dentry *udf_nfs_get_inode(struct super_block *sb, u32 block,
					u16 partref, __u32 generation)
{
	struct inode *inode;
	struct kernel_lb_addr loc;

	if (block == 0)
		return ERR_PTR(-ESTALE);

	loc.logicalBlockNum = block;
	loc.partitionReferenceNum = partref;
	inode = udf_iget(sb, &loc);

<<<<<<< HEAD
	if (inode == NULL)
		return ERR_PTR(-ENOMEM);
=======
	if (IS_ERR(inode))
		return ERR_CAST(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (generation && inode->i_generation != generation) {
		iput(inode);
		return ERR_PTR(-ESTALE);
	}
	return d_obtain_alias(inode);
}

static struct dentry *udf_fh_to_dentry(struct super_block *sb,
				       struct fid *fid, int fh_len, int fh_type)
{
<<<<<<< HEAD
	if ((fh_len != 3 && fh_len != 5) ||
=======
	if (fh_len < 3 ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (fh_type != FILEID_UDF_WITH_PARENT &&
	     fh_type != FILEID_UDF_WITHOUT_PARENT))
		return NULL;

	return udf_nfs_get_inode(sb, fid->udf.block, fid->udf.partref,
			fid->udf.generation);
}

static struct dentry *udf_fh_to_parent(struct super_block *sb,
				       struct fid *fid, int fh_len, int fh_type)
{
<<<<<<< HEAD
	if (fh_len != 5 || fh_type != FILEID_UDF_WITH_PARENT)
=======
	if (fh_len < 5 || fh_type != FILEID_UDF_WITH_PARENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;

	return udf_nfs_get_inode(sb, fid->udf.parent_block,
				 fid->udf.parent_partref,
				 fid->udf.parent_generation);
}
<<<<<<< HEAD
static int udf_encode_fh(struct dentry *de, __u32 *fh, int *lenp,
			 int connectable)
{
	int len = *lenp;
	struct inode *inode =  de->d_inode;
=======
static int udf_encode_fh(struct inode *inode, __u32 *fh, int *lenp,
			 struct inode *parent)
{
	int len = *lenp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct kernel_lb_addr location = UDF_I(inode)->i_location;
	struct fid *fid = (struct fid *)fh;
	int type = FILEID_UDF_WITHOUT_PARENT;

<<<<<<< HEAD
	if (connectable && (len < 5)) {
		*lenp = 5;
		return 255;
	} else if (len < 3) {
		*lenp = 3;
		return 255;
=======
	if (parent && (len < 5)) {
		*lenp = 5;
		return FILEID_INVALID;
	} else if (len < 3) {
		*lenp = 3;
		return FILEID_INVALID;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	*lenp = 3;
	fid->udf.block = location.logicalBlockNum;
	fid->udf.partref = location.partitionReferenceNum;
	fid->udf.parent_partref = 0;
	fid->udf.generation = inode->i_generation;

<<<<<<< HEAD
	if (connectable && !S_ISDIR(inode->i_mode)) {
		spin_lock(&de->d_lock);
		inode = de->d_parent->d_inode;
		location = UDF_I(inode)->i_location;
		fid->udf.parent_block = location.logicalBlockNum;
		fid->udf.parent_partref = location.partitionReferenceNum;
		fid->udf.parent_generation = inode->i_generation;
		spin_unlock(&de->d_lock);
=======
	if (parent) {
		location = UDF_I(parent)->i_location;
		fid->udf.parent_block = location.logicalBlockNum;
		fid->udf.parent_partref = location.partitionReferenceNum;
		fid->udf.parent_generation = inode->i_generation;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*lenp = 5;
		type = FILEID_UDF_WITH_PARENT;
	}

	return type;
}

const struct export_operations udf_export_ops = {
	.encode_fh	= udf_encode_fh,
	.fh_to_dentry   = udf_fh_to_dentry,
	.fh_to_parent   = udf_fh_to_parent,
	.get_parent     = udf_get_parent,
};

const struct inode_operations udf_dir_inode_operations = {
	.lookup				= udf_lookup,
	.create				= udf_create,
	.link				= udf_link,
	.unlink				= udf_unlink,
	.symlink			= udf_symlink,
	.mkdir				= udf_mkdir,
	.rmdir				= udf_rmdir,
	.mknod				= udf_mknod,
	.rename				= udf_rename,
<<<<<<< HEAD
};
const struct inode_operations udf_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.follow_link	= page_follow_link_light,
	.put_link	= page_put_link,
=======
	.tmpfile			= udf_tmpfile,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
