<<<<<<< HEAD

=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/minix_fs.h>
#include <linux/ext2_fs.h>
#include <linux/romfs_fs.h>
<<<<<<< HEAD
#include <linux/cramfs_fs.h>
=======
#include <uapi/linux/cramfs_fs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/initrd.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "do_mounts.h"
#include "../fs/squashfs/squashfs_fs.h"

#include <linux/decompress/generic.h>

<<<<<<< HEAD

int __initdata rd_prompt = 1;/* 1 = prompt for RAM disk, 0 = don't prompt */

static int __init prompt_ramdisk(char *str)
{
	rd_prompt = simple_strtol(str,NULL,0) & 1;
=======
static struct file *in_file, *out_file;
static loff_t in_pos, out_pos;

static int __init prompt_ramdisk(char *str)
{
	pr_warn("ignoring the deprecated prompt_ramdisk= option\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}
__setup("prompt_ramdisk=", prompt_ramdisk);

int __initdata rd_image_start;		/* starting block # of image */

static int __init ramdisk_start_setup(char *str)
{
	rd_image_start = simple_strtol(str,NULL,0);
	return 1;
}
__setup("ramdisk_start=", ramdisk_start_setup);

<<<<<<< HEAD
static int __init crd_load(int in_fd, int out_fd, decompress_fn deco);
=======
static int __init crd_load(decompress_fn deco);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This routine tries to find a RAM disk image to load, and returns the
 * number of blocks to read for a non-compressed image, 0 if the image
 * is a compressed image, and -1 if an image with the right magic
 * numbers could not be found.
 *
 * We currently check for the following magic numbers:
 *	minix
 *	ext2
 *	romfs
 *	cramfs
 *	squashfs
 *	gzip
<<<<<<< HEAD
 */
static int __init
identify_ramdisk_image(int fd, int start_block, decompress_fn *decompressor)
=======
 *	bzip2
 *	lzma
 *	xz
 *	lzo
 *	lz4
 */
static int __init
identify_ramdisk_image(struct file *file, loff_t pos,
		decompress_fn *decompressor)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const int size = 512;
	struct minix_super_block *minixsb;
	struct romfs_super_block *romfsb;
	struct cramfs_super *cramfsb;
	struct squashfs_super_block *squashfsb;
	int nblocks = -1;
	unsigned char *buf;
	const char *compress_name;
	unsigned long n;
<<<<<<< HEAD
=======
	int start_block = rd_image_start;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf = kmalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	minixsb = (struct minix_super_block *) buf;
	romfsb = (struct romfs_super_block *) buf;
	cramfsb = (struct cramfs_super *) buf;
	squashfsb = (struct squashfs_super_block *) buf;
	memset(buf, 0xe5, size);

	/*
	 * Read block 0 to test for compressed kernel
	 */
<<<<<<< HEAD
	sys_lseek(fd, start_block * BLOCK_SIZE, 0);
	sys_read(fd, buf, size);

	*decompressor = decompress_method(buf, size, &compress_name);
	printk(KERN_ERR
		   "RAMDISK: decompressor name is %s!\n",
		   compress_name);

=======
	pos = start_block * BLOCK_SIZE;
	kernel_read(file, buf, size, &pos);

	*decompressor = decompress_method(buf, size, &compress_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (compress_name) {
		printk(KERN_NOTICE "RAMDISK: %s image found at block %d\n",
		       compress_name, start_block);
		if (!*decompressor)
			printk(KERN_EMERG
			       "RAMDISK: %s decompressor not configured!\n",
			       compress_name);
		nblocks = 0;
		goto done;
	}

	/* romfs is at block zero too */
	if (romfsb->word0 == ROMSB_WORD0 &&
	    romfsb->word1 == ROMSB_WORD1) {
		printk(KERN_NOTICE
		       "RAMDISK: romfs filesystem found at block %d\n",
		       start_block);
		nblocks = (ntohl(romfsb->size)+BLOCK_SIZE-1)>>BLOCK_SIZE_BITS;
		goto done;
	}

	if (cramfsb->magic == CRAMFS_MAGIC) {
		printk(KERN_NOTICE
		       "RAMDISK: cramfs filesystem found at block %d\n",
		       start_block);
		nblocks = (cramfsb->size + BLOCK_SIZE - 1) >> BLOCK_SIZE_BITS;
		goto done;
	}

	/* squashfs is at block zero too */
	if (le32_to_cpu(squashfsb->s_magic) == SQUASHFS_MAGIC) {
		printk(KERN_NOTICE
		       "RAMDISK: squashfs filesystem found at block %d\n",
		       start_block);
		nblocks = (le64_to_cpu(squashfsb->bytes_used) + BLOCK_SIZE - 1)
			 >> BLOCK_SIZE_BITS;
		goto done;
	}

	/*
	 * Read 512 bytes further to check if cramfs is padded
	 */
<<<<<<< HEAD
	sys_lseek(fd, start_block * BLOCK_SIZE + 0x200, 0);
	sys_read(fd, buf, size);
=======
	pos = start_block * BLOCK_SIZE + 0x200;
	kernel_read(file, buf, size, &pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (cramfsb->magic == CRAMFS_MAGIC) {
		printk(KERN_NOTICE
		       "RAMDISK: cramfs filesystem found at block %d\n",
		       start_block);
		nblocks = (cramfsb->size + BLOCK_SIZE - 1) >> BLOCK_SIZE_BITS;
		goto done;
	}

	/*
	 * Read block 1 to test for minix and ext2 superblock
	 */
<<<<<<< HEAD
	sys_lseek(fd, (start_block+1) * BLOCK_SIZE, 0);
	sys_read(fd, buf, size);
=======
	pos = (start_block + 1) * BLOCK_SIZE;
	kernel_read(file, buf, size, &pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Try minix */
	if (minixsb->s_magic == MINIX_SUPER_MAGIC ||
	    minixsb->s_magic == MINIX_SUPER_MAGIC2) {
		printk(KERN_NOTICE
		       "RAMDISK: Minix filesystem found at block %d\n",
		       start_block);
		nblocks = minixsb->s_nzones << minixsb->s_log_zone_size;
		goto done;
	}

	/* Try ext2 */
	n = ext2_image_size(buf);
	if (n) {
		printk(KERN_NOTICE
		       "RAMDISK: ext2 filesystem found at block %d\n",
		       start_block);
		nblocks = n;
		goto done;
	}

	printk(KERN_NOTICE
	       "RAMDISK: Couldn't find valid RAM disk image starting at %d.\n",
	       start_block);

done:
<<<<<<< HEAD
	sys_lseek(fd, start_block * BLOCK_SIZE, 0);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(buf);
	return nblocks;
}

<<<<<<< HEAD
int __init rd_load_image(char *from)
{
	int res = 0;
	int in_fd, out_fd;
	unsigned long rd_blocks, devblocks;
	int nblocks, i, disk;
=======
static unsigned long nr_blocks(struct file *file)
{
	struct inode *inode = file->f_mapping->host;

	if (!S_ISBLK(inode->i_mode))
		return 0;
	return i_size_read(inode) >> 10;
}

int __init rd_load_image(char *from)
{
	int res = 0;
	unsigned long rd_blocks, devblocks;
	int nblocks, i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *buf = NULL;
	unsigned short rotate = 0;
	decompress_fn decompressor = NULL;
#if !defined(CONFIG_S390)
	char rotator[4] = { '|' , '/' , '-' , '\\' };
#endif

<<<<<<< HEAD
	out_fd = sys_open((const char __user __force *) "/dev/ram", O_RDWR, 0);
	if (out_fd < 0)
		goto out;

	in_fd = sys_open(from, O_RDONLY, 0);
	if (in_fd < 0)
		goto noclose_input;

	nblocks = identify_ramdisk_image(in_fd, rd_image_start, &decompressor);
=======
	out_file = filp_open("/dev/ram", O_RDWR, 0);
	if (IS_ERR(out_file))
		goto out;

	in_file = filp_open(from, O_RDONLY, 0);
	if (IS_ERR(in_file))
		goto noclose_input;

	in_pos = rd_image_start * BLOCK_SIZE;
	nblocks = identify_ramdisk_image(in_file, in_pos, &decompressor);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nblocks < 0)
		goto done;

	if (nblocks == 0) {
<<<<<<< HEAD
		if (crd_load(in_fd, out_fd, decompressor) == 0)
=======
		if (crd_load(decompressor) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto successful_load;
		goto done;
	}

	/*
	 * NOTE NOTE: nblocks is not actually blocks but
	 * the number of kibibytes of data to load into a ramdisk.
<<<<<<< HEAD
	 * So any ramdisk block size that is a multiple of 1KiB should
	 * work when the appropriate ramdisk_blocksize is specified
	 * on the command line.
	 *
	 * The default ramdisk_blocksize is 1KiB and it is generally
	 * silly to use anything else, so make sure to use 1KiB
	 * blocksize while generating ext2fs ramdisk-images.
	 */
	if (sys_ioctl(out_fd, BLKGETSIZE, (unsigned long)&rd_blocks) < 0)
		rd_blocks = 0;
	else
		rd_blocks >>= 1;

=======
	 */
	rd_blocks = nr_blocks(out_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (nblocks > rd_blocks) {
		printk("RAMDISK: image too big! (%dKiB/%ldKiB)\n",
		       nblocks, rd_blocks);
		goto done;
	}

	/*
	 * OK, time to copy in the data
	 */
<<<<<<< HEAD
	if (sys_ioctl(in_fd, BLKGETSIZE, (unsigned long)&devblocks) < 0)
		devblocks = 0;
	else
		devblocks >>= 1;

	if (strcmp(from, "/initrd.image") == 0)
		devblocks = nblocks;
=======
	if (strcmp(from, "/initrd.image") == 0)
		devblocks = nblocks;
	else
		devblocks = nr_blocks(in_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (devblocks == 0) {
		printk(KERN_ERR "RAMDISK: could not determine device size\n");
		goto done;
	}

	buf = kmalloc(BLOCK_SIZE, GFP_KERNEL);
	if (!buf) {
		printk(KERN_ERR "RAMDISK: could not allocate buffer\n");
		goto done;
	}

	printk(KERN_NOTICE "RAMDISK: Loading %dKiB [%ld disk%s] into ram disk... ",
		nblocks, ((nblocks-1)/devblocks)+1, nblocks>devblocks ? "s" : "");
<<<<<<< HEAD
	for (i = 0, disk = 1; i < nblocks; i++) {
		if (i && (i % devblocks == 0)) {
			printk("done disk #%d.\n", disk++);
			rotate = 0;
			if (sys_close(in_fd)) {
				printk("Error closing the disk.\n");
				goto noclose_input;
			}
			change_floppy("disk #%d", disk);
			in_fd = sys_open(from, O_RDONLY, 0);
			if (in_fd < 0)  {
				printk("Error opening disk.\n");
				goto noclose_input;
			}
			printk("Loading disk #%d... ", disk);
		}
		sys_read(in_fd, buf, BLOCK_SIZE);
		sys_write(out_fd, buf, BLOCK_SIZE);
#if !defined(CONFIG_S390)
		if (!(i % 16)) {
			printk("%c\b", rotator[rotate & 0x3]);
=======
	for (i = 0; i < nblocks; i++) {
		if (i && (i % devblocks == 0)) {
			pr_cont("done disk #1.\n");
			rotate = 0;
			fput(in_file);
			break;
		}
		kernel_read(in_file, buf, BLOCK_SIZE, &in_pos);
		kernel_write(out_file, buf, BLOCK_SIZE, &out_pos);
#if !defined(CONFIG_S390)
		if (!(i % 16)) {
			pr_cont("%c\b", rotator[rotate & 0x3]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rotate++;
		}
#endif
	}
<<<<<<< HEAD
	printk("done.\n");
=======
	pr_cont("done.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

successful_load:
	res = 1;
done:
<<<<<<< HEAD
	sys_close(in_fd);
noclose_input:
	sys_close(out_fd);
out:
	kfree(buf);
	sys_unlink((const char __user __force *) "/dev/ram");
=======
	fput(in_file);
noclose_input:
	fput(out_file);
out:
	kfree(buf);
	init_unlink("/dev/ram");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return res;
}

int __init rd_load_disk(int n)
{
<<<<<<< HEAD
	if (rd_prompt)
		change_floppy("root floppy disk to be loaded into RAM disk");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	create_dev("/dev/root", ROOT_DEV);
	create_dev("/dev/ram", MKDEV(RAMDISK_MAJOR, n));
	return rd_load_image("/dev/root");
}

static int exit_code;
static int decompress_error;
<<<<<<< HEAD
static int crd_infd, crd_outfd;

static int __init compr_fill(void *buf, unsigned int len)
{
	int r = sys_read(crd_infd, buf, len);
=======

static long __init compr_fill(void *buf, unsigned long len)
{
	long r = kernel_read(in_file, buf, len, &in_pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r < 0)
		printk(KERN_ERR "RAMDISK: error while reading compressed data");
	else if (r == 0)
		printk(KERN_ERR "RAMDISK: EOF while reading compressed data");
	return r;
}

<<<<<<< HEAD
static int __init compr_flush(void *window, unsigned int outcnt)
{
	int written = sys_write(crd_outfd, window, outcnt);
	if (written != outcnt) {
		if (decompress_error == 0)
			printk(KERN_ERR
			       "RAMDISK: incomplete write (%d != %d)\n",
=======
static long __init compr_flush(void *window, unsigned long outcnt)
{
	long written = kernel_write(out_file, window, outcnt, &out_pos);
	if (written != outcnt) {
		if (decompress_error == 0)
			printk(KERN_ERR
			       "RAMDISK: incomplete write (%ld != %ld)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       written, outcnt);
		decompress_error = 1;
		return -1;
	}
	return outcnt;
}

static void __init error(char *x)
{
	printk(KERN_ERR "%s\n", x);
	exit_code = 1;
	decompress_error = 1;
}

<<<<<<< HEAD
static int __init crd_load(int in_fd, int out_fd, decompress_fn deco)
{
	int result;
	crd_infd = in_fd;
	crd_outfd = out_fd;
=======
static int __init crd_load(decompress_fn deco)
{
	int result;

	if (!deco) {
		pr_emerg("Invalid ramdisk decompression routine.  "
			 "Select appropriate config option.\n");
		panic("Could not decompress initial ramdisk image.");
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	result = deco(NULL, 0, compr_fill, compr_flush, NULL, NULL, error);
	if (decompress_error)
		result = 1;
	return result;
}
