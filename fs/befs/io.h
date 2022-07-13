/*
 * io.h
 */

struct buffer_head *befs_bread_iaddr(struct super_block *sb,
				     befs_inode_addr iaddr);
<<<<<<< HEAD

struct buffer_head *befs_bread(struct super_block *sb, befs_blocknr_t block);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
