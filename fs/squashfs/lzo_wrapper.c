<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2010 LG Electronics
 * Chan Jeong <chan.jeong@lge.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * lzo_wrapper.c
 */

#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
=======
#include <linux/bio.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/lzo.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"
#include "decompressor.h"
<<<<<<< HEAD
=======
#include "page_actor.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct squashfs_lzo {
	void	*input;
	void	*output;
};

<<<<<<< HEAD
static void *lzo_init(struct squashfs_sb_info *msblk, void *buff, int len)
=======
static void *lzo_init(struct squashfs_sb_info *msblk, void *buff)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int block_size = max_t(int, msblk->block_size, SQUASHFS_METADATA_SIZE);

	struct squashfs_lzo *stream = kzalloc(sizeof(*stream), GFP_KERNEL);
	if (stream == NULL)
		goto failed;
	stream->input = vmalloc(block_size);
	if (stream->input == NULL)
		goto failed;
	stream->output = vmalloc(block_size);
	if (stream->output == NULL)
		goto failed2;

	return stream;

failed2:
	vfree(stream->input);
failed:
	ERROR("Failed to allocate lzo workspace\n");
	kfree(stream);
	return ERR_PTR(-ENOMEM);
}


static void lzo_free(void *strm)
{
	struct squashfs_lzo *stream = strm;

	if (stream) {
		vfree(stream->input);
		vfree(stream->output);
	}
	kfree(stream);
}


<<<<<<< HEAD
static int lzo_uncompress(struct squashfs_sb_info *msblk, void **buffer,
	struct buffer_head **bh, int b, int offset, int length, int srclength,
	int pages)
{
	struct squashfs_lzo *stream = msblk->stream;
	void *buff = stream->input;
	int avail, i, bytes = length, res;
	size_t out_len = srclength;

	mutex_lock(&msblk->read_data_mutex);

	for (i = 0; i < b; i++) {
		wait_on_buffer(bh[i]);
		if (!buffer_uptodate(bh[i]))
			goto block_release;

		avail = min(bytes, msblk->devblksize - offset);
		memcpy(buff, bh[i]->b_data + offset, avail);
		buff += avail;
		bytes -= avail;
		offset = 0;
		put_bh(bh[i]);
=======
static int lzo_uncompress(struct squashfs_sb_info *msblk, void *strm,
	struct bio *bio, int offset, int length,
	struct squashfs_page_actor *output)
{
	struct bvec_iter_all iter_all = {};
	struct bio_vec *bvec = bvec_init_iter_all(&iter_all);
	struct squashfs_lzo *stream = strm;
	void *buff = stream->input, *data;
	int bytes = length, res;
	size_t out_len = output->length;

	while (bio_next_segment(bio, &iter_all)) {
		int avail = min(bytes, ((int)bvec->bv_len) - offset);

		data = bvec_virt(bvec);
		memcpy(buff, data + offset, avail);
		buff += avail;
		bytes -= avail;
		offset = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	res = lzo1x_decompress_safe(stream->input, (size_t)length,
					stream->output, &out_len);
	if (res != LZO_E_OK)
		goto failed;

	res = bytes = (int)out_len;
<<<<<<< HEAD
	for (i = 0, buff = stream->output; bytes && i < pages; i++) {
		avail = min_t(int, bytes, PAGE_CACHE_SIZE);
		memcpy(buffer[i], buff, avail);
		buff += avail;
		bytes -= avail;
	}

	mutex_unlock(&msblk->read_data_mutex);
	return res;

block_release:
	for (; i < b; i++)
		put_bh(bh[i]);

failed:
	mutex_unlock(&msblk->read_data_mutex);

	ERROR("lzo decompression failed, data probably corrupt\n");
=======
	data = squashfs_first_page(output);
	buff = stream->output;
	while (data) {
		if (bytes <= PAGE_SIZE) {
			if (!IS_ERR(data))
				memcpy(data, buff, bytes);
			break;
		} else {
			if (!IS_ERR(data))
				memcpy(data, buff, PAGE_SIZE);
			buff += PAGE_SIZE;
			bytes -= PAGE_SIZE;
			data = squashfs_next_page(output);
		}
	}
	squashfs_finish_page(output);

	return res;

failed:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EIO;
}

const struct squashfs_decompressor squashfs_lzo_comp_ops = {
	.init = lzo_init,
	.free = lzo_free,
	.decompress = lzo_uncompress,
	.id = LZO_COMPRESSION,
	.name = "lzo",
<<<<<<< HEAD
=======
	.alloc_buffer = 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.supported = 1
};
