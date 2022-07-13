<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/adfs/dir_fplus.c
 *
 *  Copyright (C) 1997-1999 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include "adfs.h"
#include "dir_fplus.h"

static int
adfs_fplus_read(struct super_block *sb, unsigned int id, unsigned int sz, struct adfs_dir *dir)
{
	struct adfs_bigdirheader *h;
	struct adfs_bigdirtail *t;
	unsigned long block;
	unsigned int blk, size;
	int i, ret = -EIO;

	dir->nr_buffers = 0;

	/* start off using fixed bh set - only alloc for big dirs */
	dir->bh_fplus = &dir->bh[0];

	block = __adfs_block_map(sb, id, 0);
	if (!block) {
		adfs_error(sb, "dir object %X has a hole at offset 0", id);
		goto out;
	}

	dir->bh_fplus[0] = sb_bread(sb, block);
	if (!dir->bh_fplus[0])
		goto out;
	dir->nr_buffers += 1;

	h = (struct adfs_bigdirheader *)dir->bh_fplus[0]->b_data;
	size = le32_to_cpu(h->bigdirsize);
	if (size != sz) {
		printk(KERN_WARNING "adfs: adfs_fplus_read:"
					" directory header size %X\n"
					" does not match directory size %X\n",
					size, sz);
	}

	if (h->bigdirversion[0] != 0 || h->bigdirversion[1] != 0 ||
	    h->bigdirversion[2] != 0 || size & 2047 ||
	    h->bigdirstartname != cpu_to_le32(BIGDIRSTARTNAME)) {
		printk(KERN_WARNING "adfs: dir object %X has"
					" malformed dir header\n", id);
		goto out;
	}

	size >>= sb->s_blocksize_bits;
	if (size > sizeof(dir->bh)/sizeof(dir->bh[0])) {
		/* this directory is too big for fixed bh set, must allocate */
		struct buffer_head **bh_fplus =
			kzalloc(size * sizeof(struct buffer_head *),
				GFP_KERNEL);
		if (!bh_fplus) {
			adfs_error(sb, "not enough memory for"
					" dir object %X (%d blocks)", id, size);
			goto out;
		}
		dir->bh_fplus = bh_fplus;
		/* copy over the pointer to the block that we've already read */
		dir->bh_fplus[0] = dir->bh[0];
	}

	for (blk = 1; blk < size; blk++) {
		block = __adfs_block_map(sb, id, blk);
		if (!block) {
			adfs_error(sb, "dir object %X has a hole at offset %d", id, blk);
			goto out;
		}

		dir->bh_fplus[blk] = sb_bread(sb, block);
		if (!dir->bh_fplus[blk]) {
			adfs_error(sb,	"dir object %X failed read for"
					" offset %d, mapped block %X",
					id, blk, block);
			goto out;
		}

		dir->nr_buffers += 1;
	}

	t = (struct adfs_bigdirtail *)
		(dir->bh_fplus[size - 1]->b_data + (sb->s_blocksize - 8));

	if (t->bigdirendname != cpu_to_le32(BIGDIRENDNAME) ||
	    t->bigdirendmasseq != h->startmasseq ||
	    t->reserved[0] != 0 || t->reserved[1] != 0) {
		printk(KERN_WARNING "adfs: dir object %X has "
					"malformed dir end\n", id);
=======
 */
#include "adfs.h"
#include "dir_fplus.h"

/* Return the byte offset to directory entry pos */
static unsigned int adfs_fplus_offset(const struct adfs_bigdirheader *h,
				      unsigned int pos)
{
	return offsetof(struct adfs_bigdirheader, bigdirname) +
	       ALIGN(le32_to_cpu(h->bigdirnamelen), 4) +
	       pos * sizeof(struct adfs_bigdirentry);
}

static int adfs_fplus_validate_header(const struct adfs_bigdirheader *h)
{
	unsigned int size = le32_to_cpu(h->bigdirsize);
	unsigned int len;

	if (h->bigdirversion[0] != 0 || h->bigdirversion[1] != 0 ||
	    h->bigdirversion[2] != 0 ||
	    h->bigdirstartname != cpu_to_le32(BIGDIRSTARTNAME) ||
	    !size || size & 2047 || size > SZ_4M)
		return -EIO;

	size -= sizeof(struct adfs_bigdirtail) +
		offsetof(struct adfs_bigdirheader, bigdirname);

	/* Check that bigdirnamelen fits within the directory */
	len = ALIGN(le32_to_cpu(h->bigdirnamelen), 4);
	if (len > size)
		return -EIO;

	size -= len;

	/* Check that bigdirnamesize fits within the directory */
	len = le32_to_cpu(h->bigdirnamesize);
	if (len > size)
		return -EIO;

	size -= len;

	/*
	 * Avoid division, we know that absolute maximum number of entries
	 * can not be so large to cause overflow of the multiplication below.
	 */
	len = le32_to_cpu(h->bigdirentries);
	if (len > SZ_4M / sizeof(struct adfs_bigdirentry) ||
	    len * sizeof(struct adfs_bigdirentry) > size)
		return -EIO;

	return 0;
}

static int adfs_fplus_validate_tail(const struct adfs_bigdirheader *h,
				    const struct adfs_bigdirtail *t)
{
	if (t->bigdirendname != cpu_to_le32(BIGDIRENDNAME) ||
	    t->bigdirendmasseq != h->startmasseq ||
	    t->reserved[0] != 0 || t->reserved[1] != 0)
		return -EIO;

	return 0;
}

static u8 adfs_fplus_checkbyte(struct adfs_dir *dir)
{
	struct adfs_bigdirheader *h = dir->bighead;
	struct adfs_bigdirtail *t = dir->bigtail;
	unsigned int end, bs, bi, i;
	__le32 *bp;
	u32 dircheck;

	end = adfs_fplus_offset(h, le32_to_cpu(h->bigdirentries)) +
		le32_to_cpu(h->bigdirnamesize);

	/* Accumulate the contents of the header, entries and names */
	for (dircheck = 0, bi = 0; end; bi++) {
		bp = (void *)dir->bhs[bi]->b_data;
		bs = dir->bhs[bi]->b_size;
		if (bs > end)
			bs = end;

		for (i = 0; i < bs; i += sizeof(u32))
			dircheck = ror32(dircheck, 13) ^ le32_to_cpup(bp++);

		end -= bs;
	}

	/* Accumulate the contents of the tail except for the check byte */
	dircheck = ror32(dircheck, 13) ^ le32_to_cpu(t->bigdirendname);
	dircheck = ror32(dircheck, 13) ^ t->bigdirendmasseq;
	dircheck = ror32(dircheck, 13) ^ t->reserved[0];
	dircheck = ror32(dircheck, 13) ^ t->reserved[1];

	return dircheck ^ dircheck >> 8 ^ dircheck >> 16 ^ dircheck >> 24;
}

static int adfs_fplus_read(struct super_block *sb, u32 indaddr,
			   unsigned int size, struct adfs_dir *dir)
{
	struct adfs_bigdirheader *h;
	struct adfs_bigdirtail *t;
	unsigned int dirsize;
	int ret;

	/* Read first buffer */
	ret = adfs_dir_read_buffers(sb, indaddr, sb->s_blocksize, dir);
	if (ret)
		return ret;

	dir->bighead = h = (void *)dir->bhs[0]->b_data;
	ret = adfs_fplus_validate_header(h);
	if (ret) {
		adfs_error(sb, "dir %06x has malformed header", indaddr);
		goto out;
	}

	dirsize = le32_to_cpu(h->bigdirsize);
	if (size && dirsize != size) {
		adfs_msg(sb, KERN_WARNING,
			 "dir %06x header size %X does not match directory size %X",
			 indaddr, dirsize, size);
	}

	/* Read remaining buffers */
	ret = adfs_dir_read_buffers(sb, indaddr, dirsize, dir);
	if (ret)
		return ret;

	dir->bigtail = t = (struct adfs_bigdirtail *)
		(dir->bhs[dir->nr_buffers - 1]->b_data + (sb->s_blocksize - 8));

	ret = adfs_fplus_validate_tail(h, t);
	if (ret) {
		adfs_error(sb, "dir %06x has malformed tail", indaddr);
		goto out;
	}

	if (adfs_fplus_checkbyte(dir) != t->bigdircheckbyte) {
		adfs_error(sb, "dir %06x checkbyte mismatch\n", indaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	dir->parent_id = le32_to_cpu(h->bigdirparent);
<<<<<<< HEAD
	dir->sb = sb;
	return 0;

out:
	if (dir->bh_fplus) {
		for (i = 0; i < dir->nr_buffers; i++)
			brelse(dir->bh_fplus[i]);

		if (&dir->bh[0] != dir->bh_fplus)
			kfree(dir->bh_fplus);

		dir->bh_fplus = NULL;
	}

	dir->nr_buffers = 0;
	dir->sb = NULL;
=======
	return 0;

out:
	adfs_dir_relse(dir);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int
adfs_fplus_setpos(struct adfs_dir *dir, unsigned int fpos)
{
<<<<<<< HEAD
	struct adfs_bigdirheader *h =
		(struct adfs_bigdirheader *) dir->bh_fplus[0]->b_data;
	int ret = -ENOENT;

	if (fpos <= le32_to_cpu(h->bigdirentries)) {
=======
	int ret = -ENOENT;

	if (fpos <= le32_to_cpu(dir->bighead->bigdirentries)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dir->pos = fpos;
		ret = 0;
	}

	return ret;
}

<<<<<<< HEAD
static void
dir_memcpy(struct adfs_dir *dir, unsigned int offset, void *to, int len)
{
	struct super_block *sb = dir->sb;
	unsigned int buffer, partial, remainder;

	buffer = offset >> sb->s_blocksize_bits;
	offset &= sb->s_blocksize - 1;

	partial = sb->s_blocksize - offset;

	if (partial >= len)
		memcpy(to, dir->bh_fplus[buffer]->b_data + offset, len);
	else {
		char *c = (char *)to;

		remainder = len - partial;

		memcpy(c,
			dir->bh_fplus[buffer]->b_data + offset,
			partial);

		memcpy(c + partial,
			dir->bh_fplus[buffer + 1]->b_data,
			remainder);
	}
}

static int
adfs_fplus_getnext(struct adfs_dir *dir, struct object_info *obj)
{
	struct adfs_bigdirheader *h =
		(struct adfs_bigdirheader *) dir->bh_fplus[0]->b_data;
	struct adfs_bigdirentry bde;
	unsigned int offset;
	int i, ret = -ENOENT;

	if (dir->pos >= le32_to_cpu(h->bigdirentries))
		goto out;

	offset = offsetof(struct adfs_bigdirheader, bigdirname);
	offset += ((le32_to_cpu(h->bigdirnamelen) + 4) & ~3);
	offset += dir->pos * sizeof(struct adfs_bigdirentry);

	dir_memcpy(dir, offset, &bde, sizeof(struct adfs_bigdirentry));
=======
static int
adfs_fplus_getnext(struct adfs_dir *dir, struct object_info *obj)
{
	struct adfs_bigdirheader *h = dir->bighead;
	struct adfs_bigdirentry bde;
	unsigned int offset;
	int ret;

	if (dir->pos >= le32_to_cpu(h->bigdirentries))
		return -ENOENT;

	offset = adfs_fplus_offset(h, dir->pos);

	ret = adfs_dir_copyfrom(&bde, dir, offset,
				sizeof(struct adfs_bigdirentry));
	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	obj->loadaddr = le32_to_cpu(bde.bigdirload);
	obj->execaddr = le32_to_cpu(bde.bigdirexec);
	obj->size     = le32_to_cpu(bde.bigdirlen);
<<<<<<< HEAD
	obj->file_id  = le32_to_cpu(bde.bigdirindaddr);
	obj->attr     = le32_to_cpu(bde.bigdirattr);
	obj->name_len = le32_to_cpu(bde.bigdirobnamelen);

	offset = offsetof(struct adfs_bigdirheader, bigdirname);
	offset += ((le32_to_cpu(h->bigdirnamelen) + 4) & ~3);
	offset += le32_to_cpu(h->bigdirentries) * sizeof(struct adfs_bigdirentry);
	offset += le32_to_cpu(bde.bigdirobnameptr);

	dir_memcpy(dir, offset, obj->name, obj->name_len);
	for (i = 0; i < obj->name_len; i++)
		if (obj->name[i] == '/')
			obj->name[i] = '.';

	obj->filetype = -1;

	/*
	 * object is a file and is filetyped and timestamped?
	 * RISC OS 12-bit filetype is stored in load_address[19:8]
	 */
	if ((0 == (obj->attr & ADFS_NDA_DIRECTORY)) &&
		(0xfff00000 == (0xfff00000 & obj->loadaddr))) {
		obj->filetype = (__u16) ((0x000fff00 & obj->loadaddr) >> 8);

		/* optionally append the ,xyz hex filetype suffix */
		if (ADFS_SB(dir->sb)->s_ftsuffix)
			obj->name_len +=
				append_filetype_suffix(
					&obj->name[obj->name_len],
					obj->filetype);
	}

	dir->pos += 1;
	ret = 0;
out:
	return ret;
}

static int
adfs_fplus_sync(struct adfs_dir *dir)
{
	int err = 0;
	int i;

	for (i = dir->nr_buffers - 1; i >= 0; i--) {
		struct buffer_head *bh = dir->bh_fplus[i];
		sync_dirty_buffer(bh);
		if (buffer_req(bh) && !buffer_uptodate(bh))
			err = -EIO;
	}

	return err;
}

static void
adfs_fplus_free(struct adfs_dir *dir)
{
	int i;

	if (dir->bh_fplus) {
		for (i = 0; i < dir->nr_buffers; i++)
			brelse(dir->bh_fplus[i]);

		if (&dir->bh[0] != dir->bh_fplus)
			kfree(dir->bh_fplus);

		dir->bh_fplus = NULL;
	}

	dir->nr_buffers = 0;
	dir->sb = NULL;
}

struct adfs_dir_ops adfs_fplus_dir_ops = {
	.read		= adfs_fplus_read,
	.setpos		= adfs_fplus_setpos,
	.getnext	= adfs_fplus_getnext,
	.sync		= adfs_fplus_sync,
	.free		= adfs_fplus_free
=======
	obj->indaddr  = le32_to_cpu(bde.bigdirindaddr);
	obj->attr     = le32_to_cpu(bde.bigdirattr);
	obj->name_len = le32_to_cpu(bde.bigdirobnamelen);

	offset = adfs_fplus_offset(h, le32_to_cpu(h->bigdirentries));
	offset += le32_to_cpu(bde.bigdirobnameptr);

	ret = adfs_dir_copyfrom(obj->name, dir, offset, obj->name_len);
	if (ret)
		return ret;

	adfs_object_fixup(dir, obj);

	dir->pos += 1;

	return 0;
}

static int adfs_fplus_iterate(struct adfs_dir *dir, struct dir_context *ctx)
{
	struct object_info obj;

	if ((ctx->pos - 2) >> 32)
		return 0;

	if (adfs_fplus_setpos(dir, ctx->pos - 2))
		return 0;

	while (!adfs_fplus_getnext(dir, &obj)) {
		if (!dir_emit(ctx, obj.name, obj.name_len,
			      obj.indaddr, DT_UNKNOWN))
			break;
		ctx->pos++;
	}

	return 0;
}

static int adfs_fplus_update(struct adfs_dir *dir, struct object_info *obj)
{
	struct adfs_bigdirheader *h = dir->bighead;
	struct adfs_bigdirentry bde;
	int offset, end, ret;

	offset = adfs_fplus_offset(h, 0) - sizeof(bde);
	end = adfs_fplus_offset(h, le32_to_cpu(h->bigdirentries));

	do {
		offset += sizeof(bde);
		if (offset >= end) {
			adfs_error(dir->sb, "unable to locate entry to update");
			return -ENOENT;
		}
		ret = adfs_dir_copyfrom(&bde, dir, offset, sizeof(bde));
		if (ret) {
			adfs_error(dir->sb, "error reading directory entry");
			return -ENOENT;
		}
	} while (le32_to_cpu(bde.bigdirindaddr) != obj->indaddr);

	bde.bigdirload    = cpu_to_le32(obj->loadaddr);
	bde.bigdirexec    = cpu_to_le32(obj->execaddr);
	bde.bigdirlen     = cpu_to_le32(obj->size);
	bde.bigdirindaddr = cpu_to_le32(obj->indaddr);
	bde.bigdirattr    = cpu_to_le32(obj->attr);

	return adfs_dir_copyto(dir, offset, &bde, sizeof(bde));
}

static int adfs_fplus_commit(struct adfs_dir *dir)
{
	int ret;

	/* Increment directory sequence number */
	dir->bighead->startmasseq += 1;
	dir->bigtail->bigdirendmasseq += 1;

	/* Update directory check byte */
	dir->bigtail->bigdircheckbyte = adfs_fplus_checkbyte(dir);

	/* Make sure the directory still validates correctly */
	ret = adfs_fplus_validate_header(dir->bighead);
	if (ret == 0)
		ret = adfs_fplus_validate_tail(dir->bighead, dir->bigtail);

	return ret;
}

const struct adfs_dir_ops adfs_fplus_dir_ops = {
	.read		= adfs_fplus_read,
	.iterate	= adfs_fplus_iterate,
	.setpos		= adfs_fplus_setpos,
	.getnext	= adfs_fplus_getnext,
	.update		= adfs_fplus_update,
	.commit		= adfs_fplus_commit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
