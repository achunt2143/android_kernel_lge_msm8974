<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * datastream.h
 *
 */

struct buffer_head *befs_read_datastream(struct super_block *sb,
<<<<<<< HEAD
					 befs_data_stream * ds, befs_off_t pos,
					 uint * off);

int befs_fblock2brun(struct super_block *sb, befs_data_stream * data,
		     befs_blocknr_t fblock, befs_block_run * run);

size_t befs_read_lsymlink(struct super_block *sb, befs_data_stream * data,
			  void *buff, befs_off_t len);

befs_blocknr_t befs_count_blocks(struct super_block *sb, befs_data_stream * ds);

extern const befs_inode_addr BAD_IADDR;

=======
					 const befs_data_stream *ds,
					 befs_off_t pos, uint *off);

int befs_fblock2brun(struct super_block *sb, const befs_data_stream *data,
		     befs_blocknr_t fblock, befs_block_run *run);

size_t befs_read_lsymlink(struct super_block *sb, const befs_data_stream *data,
			  void *buff, befs_off_t len);

befs_blocknr_t befs_count_blocks(struct super_block *sb,
			  const befs_data_stream *ds);

extern const befs_inode_addr BAD_IADDR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
