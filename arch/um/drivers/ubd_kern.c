<<<<<<< HEAD
/*
 * Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 Cambridge Greys Ltd
 * Copyright (C) 2015-2016 Anton Ivanov (aivanov@brocade.com)
 * Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/* 2001-09-28...2002-04-17
 * Partition stuff by James_McMechan@hotmail.com
 * old style ubd by setting UBD_SHIFT to 0
 * 2002-09-27...2002-10-18 massive tinkering for 2.5
 * partitions have changed in 2.5
 * 2003-01-29 more tinkering for 2.5.59-1
 * This should now address the sysfs problems and has
 * the symlink for devfs to allow for booting with
 * the common /dev/ubd/discX/... names rather than
 * only /dev/ubdN/discN this version also has lots of
 * clean ups preparing for ubd-many.
 * James McMechan
 */

#define UBD_SHIFT 4

#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
#include <linux/ata.h>
#include <linux/hdreg.h>
=======
#include <linux/blk-mq.h>
#include <linux/ata.h>
#include <linux/hdreg.h>
#include <linux/major.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/cdrom.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <asm/tlbflush.h>
<<<<<<< HEAD
#include "kern_util.h"
#include "mconsole_kern.h"
#include "init.h"
#include "irq_kern.h"
#include "ubd.h"
#include "os.h"
#include "cow.h"

enum ubd_req { UBD_READ, UBD_WRITE };

struct io_thread_req {
	struct request *req;
	enum ubd_req op;
	int fds[2];
	unsigned long offsets[2];
	unsigned long long offset;
	unsigned long length;
	char *buffer;
	int sectorsize;
	unsigned long sector_mask;
	unsigned long long cow_offset;
	unsigned long bitmap_words[2];
	int error;
};

=======
#include <kern_util.h>
#include "mconsole_kern.h"
#include <init.h>
#include <irq_kern.h>
#include "ubd.h"
#include <os.h>
#include "cow.h"

/* Max request size is determined by sector mask - 32K */
#define UBD_MAX_REQUEST (8 * sizeof(long))

struct io_desc {
	char *buffer;
	unsigned long length;
	unsigned long sector_mask;
	unsigned long long cow_offset;
	unsigned long bitmap_words[2];
};

struct io_thread_req {
	struct request *req;
	int fds[2];
	unsigned long offsets[2];
	unsigned long long offset;
	int sectorsize;
	int error;

	int desc_cnt;
	/* io_desc has to be the last element of the struct */
	struct io_desc io_desc[];
};


static struct io_thread_req * (*irq_req_buffer)[];
static struct io_thread_req *irq_remainder;
static int irq_remainder_size;

static struct io_thread_req * (*io_req_buffer)[];
static struct io_thread_req *io_remainder;
static int io_remainder_size;



>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int ubd_test_bit(__u64 bit, unsigned char *data)
{
	__u64 n;
	int bits, off;

	bits = sizeof(data[0]) * 8;
	n = bit / bits;
	off = bit % bits;
	return (data[n] & (1 << off)) != 0;
}

static inline void ubd_set_bit(__u64 bit, unsigned char *data)
{
	__u64 n;
	int bits, off;

	bits = sizeof(data[0]) * 8;
	n = bit / bits;
	off = bit % bits;
	data[n] |= (1 << off);
}
/*End stuff from ubd_user.h*/

#define DRIVER_NAME "uml-blkdev"

static DEFINE_MUTEX(ubd_lock);
static DEFINE_MUTEX(ubd_mutex); /* replaces BKL, might not be needed */

<<<<<<< HEAD
static int ubd_open(struct block_device *bdev, fmode_t mode);
static int ubd_release(struct gendisk *disk, fmode_t mode);
static int ubd_ioctl(struct block_device *bdev, fmode_t mode,
=======
static int ubd_ioctl(struct block_device *bdev, blk_mode_t mode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     unsigned int cmd, unsigned long arg);
static int ubd_getgeo(struct block_device *bdev, struct hd_geometry *geo);

#define MAX_DEV (16)

static const struct block_device_operations ubd_blops = {
        .owner		= THIS_MODULE,
<<<<<<< HEAD
        .open		= ubd_open,
        .release	= ubd_release,
        .ioctl		= ubd_ioctl,
	.getgeo		= ubd_getgeo,
};

/* Protected by ubd_lock */
static int fake_major = UBD_MAJOR;
static struct gendisk *ubd_gendisk[MAX_DEV];
static struct gendisk *fake_gendisk[MAX_DEV];

=======
        .ioctl		= ubd_ioctl,
        .compat_ioctl	= blkdev_compat_ptr_ioctl,
	.getgeo		= ubd_getgeo,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_BLK_DEV_UBD_SYNC
#define OPEN_FLAGS ((struct openflags) { .r = 1, .w = 1, .s = 1, .c = 0, \
					 .cl = 1 })
#else
#define OPEN_FLAGS ((struct openflags) { .r = 1, .w = 1, .s = 0, .c = 0, \
					 .cl = 1 })
#endif
static struct openflags global_openflags = OPEN_FLAGS;

struct cow {
	/* backing file name */
	char *file;
	/* backing file fd */
	int fd;
	unsigned long *bitmap;
	unsigned long bitmap_len;
	int bitmap_offset;
	int data_offset;
};

#define MAX_SG 64

struct ubd {
<<<<<<< HEAD
	struct list_head restart;
	/* name (and fd, below) of the file opened for writing, either the
	 * backing or the cow file. */
	char *file;
	int count;
=======
	/* name (and fd, below) of the file opened for writing, either the
	 * backing or the cow file. */
	char *file;
	char *serial;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int fd;
	__u64 size;
	struct openflags boot_openflags;
	struct openflags openflags;
	unsigned shared:1;
	unsigned no_cow:1;
<<<<<<< HEAD
	struct cow cow;
	struct platform_device pdev;
	struct request_queue *queue;
	spinlock_t lock;
	struct scatterlist sg[MAX_SG];
	struct request *request;
	int start_sg, end_sg;
	sector_t rq_pos;
=======
	unsigned no_trim:1;
	struct cow cow;
	struct platform_device pdev;
	struct gendisk *disk;
	struct blk_mq_tag_set tag_set;
	spinlock_t lock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define DEFAULT_COW { \
	.file =			NULL, \
	.fd =			-1,	\
	.bitmap =		NULL, \
	.bitmap_offset =	0, \
	.data_offset =		0, \
}

#define DEFAULT_UBD { \
	.file = 		NULL, \
<<<<<<< HEAD
	.count =		0, \
=======
	.serial =		NULL, \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.fd =			-1, \
	.size =			-1, \
	.boot_openflags =	OPEN_FLAGS, \
	.openflags =		OPEN_FLAGS, \
	.no_cow =               0, \
<<<<<<< HEAD
	.shared =		0, \
	.cow =			DEFAULT_COW, \
	.lock =			__SPIN_LOCK_UNLOCKED(ubd_devs.lock), \
	.request =		NULL, \
	.start_sg =		0, \
	.end_sg =		0, \
	.rq_pos =		0, \
=======
	.no_trim =		0, \
	.shared =		0, \
	.cow =			DEFAULT_COW, \
	.lock =			__SPIN_LOCK_UNLOCKED(ubd_devs.lock), \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Protected by ubd_lock */
static struct ubd ubd_devs[MAX_DEV] = { [0 ... MAX_DEV - 1] = DEFAULT_UBD };

<<<<<<< HEAD
/* Only changed by fake_ide_setup which is a setup */
static int fake_ide = 0;
static struct proc_dir_entry *proc_ide_root = NULL;
static struct proc_dir_entry *proc_ide = NULL;

static void make_proc_ide(void)
{
	proc_ide_root = proc_mkdir("ide", NULL);
	proc_ide = proc_mkdir("ide0", proc_ide_root);
}

static int fake_ide_media_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, "disk\n");
	return 0;
}

static int fake_ide_media_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fake_ide_media_proc_show, NULL);
}

static const struct file_operations fake_ide_media_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= fake_ide_media_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void make_ide_entries(const char *dev_name)
{
	struct proc_dir_entry *dir, *ent;
	char name[64];

	if(proc_ide_root == NULL) make_proc_ide();

	dir = proc_mkdir(dev_name, proc_ide);
	if(!dir) return;

	ent = proc_create("media", S_IRUGO, dir, &fake_ide_media_proc_fops);
	if(!ent) return;
	snprintf(name, sizeof(name), "ide0/%s", dev_name);
	proc_symlink(dev_name, proc_ide_root, name);
}

static int fake_ide_setup(char *str)
{
	fake_ide = 1;
	return 1;
}

=======
static blk_status_t ubd_queue_rq(struct blk_mq_hw_ctx *hctx,
				 const struct blk_mq_queue_data *bd);

static int fake_ide_setup(char *str)
{
	pr_warn("The fake_ide option has been removed\n");
	return 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
__setup("fake_ide", fake_ide_setup);

__uml_help(fake_ide_setup,
"fake_ide\n"
<<<<<<< HEAD
"    Create ide0 entries that map onto ubd devices.\n\n"
=======
"    Obsolete stub.\n\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

static int parse_unit(char **ptr)
{
	char *str = *ptr, *end;
	int n = -1;

	if(isdigit(*str)) {
		n = simple_strtoul(str, &end, 0);
		if(end == str)
			return -1;
		*ptr = end;
	}
	else if (('a' <= *str) && (*str <= 'z')) {
		n = *str - 'a';
		str++;
		*ptr = str;
	}
	return n;
}

/* If *index_out == -1 at exit, the passed option was a general one;
 * otherwise, the str pointer is used (and owned) inside ubd_devs array, so it
 * should not be freed on exit.
 */
static int ubd_setup_common(char *str, int *index_out, char **error_out)
{
	struct ubd *ubd_dev;
	struct openflags flags = global_openflags;
<<<<<<< HEAD
	char *backing_file;
=======
	char *file, *backing_file, *serial;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int n, err = 0, i;

	if(index_out) *index_out = -1;
	n = *str;
	if(n == '='){
<<<<<<< HEAD
		char *end;
		int major;

		str++;
		if(!strcmp(str, "sync")){
			global_openflags = of_sync(global_openflags);
			goto out1;
		}

		err = -EINVAL;
		major = simple_strtoul(str, &end, 0);
		if((*end != '\0') || (end == str)){
			*error_out = "Didn't parse major number";
			goto out1;
		}

		mutex_lock(&ubd_lock);
		if (fake_major != UBD_MAJOR) {
			*error_out = "Can't assign a fake major twice";
			goto out1;
		}

		fake_major = major;

		printk(KERN_INFO "Setting extra ubd major number to %d\n",
		       major);
		err = 0;
	out1:
		mutex_unlock(&ubd_lock);
		return err;
=======
		str++;
		if(!strcmp(str, "sync")){
			global_openflags = of_sync(global_openflags);
			return err;
		}

		pr_warn("fake major not supported any more\n");
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	n = parse_unit(&str);
	if(n < 0){
		*error_out = "Couldn't parse device number";
		return -EINVAL;
	}
	if(n >= MAX_DEV){
		*error_out = "Device number out of range";
		return 1;
	}

	err = -EBUSY;
	mutex_lock(&ubd_lock);

	ubd_dev = &ubd_devs[n];
	if(ubd_dev->file != NULL){
		*error_out = "Device is already configured";
		goto out;
	}

	if (index_out)
		*index_out = n;

	err = -EINVAL;
<<<<<<< HEAD
	for (i = 0; i < sizeof("rscd="); i++) {
=======
	for (i = 0; i < sizeof("rscdt="); i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		switch (*str) {
		case 'r':
			flags.w = 0;
			break;
		case 's':
			flags.s = 1;
			break;
		case 'd':
			ubd_dev->no_cow = 1;
			break;
		case 'c':
			ubd_dev->shared = 1;
			break;
<<<<<<< HEAD
=======
		case 't':
			ubd_dev->no_trim = 1;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case '=':
			str++;
			goto break_loop;
		default:
			*error_out = "Expected '=' or flag letter "
<<<<<<< HEAD
				"(r, s, c, or d)";
=======
				"(r, s, c, t or d)";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}
		str++;
	}

	if (*str == '=')
		*error_out = "Too many flags specified";
	else
		*error_out = "Missing '='";
	goto out;

break_loop:
<<<<<<< HEAD
	backing_file = strchr(str, ',');

	if (backing_file == NULL)
		backing_file = strchr(str, ':');

	if(backing_file != NULL){
		if(ubd_dev->no_cow){
			*error_out = "Can't specify both 'd' and a cow file";
			goto out;
		}
		else {
			*backing_file = '\0';
			backing_file++;
		}
	}
	err = 0;
	ubd_dev->file = str;
	ubd_dev->cow.file = backing_file;
=======
	file = strsep(&str, ",:");
	if (*file == '\0')
		file = NULL;

	backing_file = strsep(&str, ",:");
	if (backing_file && *backing_file == '\0')
		backing_file = NULL;

	serial = strsep(&str, ",:");
	if (serial && *serial == '\0')
		serial = NULL;

	if (backing_file && ubd_dev->no_cow) {
		*error_out = "Can't specify both 'd' and a cow file";
		goto out;
	}

	err = 0;
	ubd_dev->file = file;
	ubd_dev->cow.file = backing_file;
	ubd_dev->serial = serial;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ubd_dev->boot_openflags = flags;
out:
	mutex_unlock(&ubd_lock);
	return err;
}

static int ubd_setup(char *str)
{
	char *error;
	int err;

	err = ubd_setup_common(str, NULL, &error);
	if(err)
		printk(KERN_ERR "Failed to initialize device with \"%s\" : "
		       "%s\n", str, error);
	return 1;
}

__setup("ubd", ubd_setup);
__uml_help(ubd_setup,
<<<<<<< HEAD
"ubd<n><flags>=<filename>[(:|,)<filename2>]\n"
=======
"ubd<n><flags>=<filename>[(:|,)<filename2>][(:|,)<serial>]\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
"    This is used to associate a device with a file in the underlying\n"
"    filesystem. When specifying two filenames, the first one is the\n"
"    COW name and the second is the backing file name. As separator you can\n"
"    use either a ':' or a ',': the first one allows writing things like;\n"
"	ubd0=~/Uml/root_cow:~/Uml/root_backing_file\n"
"    while with a ',' the shell would not expand the 2nd '~'.\n"
"    When using only one filename, UML will detect whether to treat it like\n"
"    a COW file or a backing file. To override this detection, add the 'd'\n"
"    flag:\n"
"	ubd0d=BackingFile\n"
"    Usually, there is a filesystem in the file, but \n"
"    that's not required. Swap devices containing swap files can be\n"
"    specified like this. Also, a file which doesn't contain a\n"
"    filesystem can have its contents read in the virtual \n"
"    machine by running 'dd' on the device. <n> must be in the range\n"
"    0 to 7. Appending an 'r' to the number will cause that device\n"
"    to be mounted read-only. For example ubd1r=./ext_fs. Appending\n"
"    an 's' will cause data to be written to disk on the host immediately.\n"
"    'c' will cause the device to be treated as being shared between multiple\n"
"    UMLs and file locking will be turned off - this is appropriate for a\n"
"    cluster filesystem and inappropriate at almost all other times.\n\n"
<<<<<<< HEAD
=======
"    't' will disable trim/discard support on the device (enabled by default).\n\n"
"    An optional device serial number can be exposed using the serial parameter\n"
"    on the cmdline which is exposed as a sysfs entry. This is particularly\n"
"    useful when a unique number should be given to the device. Note when\n"
"    specifying a label, the filename2 must be also presented. It can be\n"
"    an empty string, in which case the backing file is not used:\n"
"       ubd0=File,,Serial\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

static int udb_setup(char *str)
{
	printk("udb%s specified on command line is almost certainly a ubd -> "
	       "udb TYPO\n", str);
	return 1;
}

__setup("udb", udb_setup);
__uml_help(udb_setup,
"udb\n"
"    This option is here solely to catch ubd -> udb typos, which can be\n"
"    to impossible to catch visually unless you specifically look for\n"
"    them.  The only result of any option starting with 'udb' is an error\n"
"    in the boot output.\n\n"
);

<<<<<<< HEAD
static void do_ubd_request(struct request_queue * q);

/* Only changed by ubd_init, which is an initcall. */
static int thread_fd = -1;
static LIST_HEAD(restart);

/* XXX - move this inside ubd_intr. */
/* Called without dev->lock held, and only in interrupt context. */
static void ubd_handler(void)
{
	struct io_thread_req *req;
	struct ubd *ubd;
	struct list_head *list, *next_ele;
	unsigned long flags;
	int n;

	while(1){
		n = os_read_file(thread_fd, &req,
				 sizeof(struct io_thread_req *));
		if(n != sizeof(req)){
=======
/* Only changed by ubd_init, which is an initcall. */
static int thread_fd = -1;

/* Function to read several request pointers at a time
* handling fractional reads if (and as) needed
*/

static int bulk_req_safe_read(
	int fd,
	struct io_thread_req * (*request_buffer)[],
	struct io_thread_req **remainder,
	int *remainder_size,
	int max_recs
	)
{
	int n = 0;
	int res = 0;

	if (*remainder_size > 0) {
		memmove(
			(char *) request_buffer,
			(char *) remainder, *remainder_size
		);
		n = *remainder_size;
	}

	res = os_read_file(
			fd,
			((char *) request_buffer) + *remainder_size,
			sizeof(struct io_thread_req *)*max_recs
				- *remainder_size
		);
	if (res > 0) {
		n += res;
		if ((n % sizeof(struct io_thread_req *)) > 0) {
			/*
			* Read somehow returned not a multiple of dword
			* theoretically possible, but never observed in the
			* wild, so read routine must be able to handle it
			*/
			*remainder_size = n % sizeof(struct io_thread_req *);
			WARN(*remainder_size > 0, "UBD IPC read returned a partial result");
			memmove(
				remainder,
				((char *) request_buffer) +
					(n/sizeof(struct io_thread_req *))*sizeof(struct io_thread_req *),
				*remainder_size
			);
			n = n - *remainder_size;
		}
	} else {
		n = res;
	}
	return n;
}

/* Called without dev->lock held, and only in interrupt context. */
static void ubd_handler(void)
{
	int n;
	int count;

	while(1){
		n = bulk_req_safe_read(
			thread_fd,
			irq_req_buffer,
			&irq_remainder,
			&irq_remainder_size,
			UBD_REQ_BUFFER_SIZE
		);
		if (n < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if(n == -EAGAIN)
				break;
			printk(KERN_ERR "spurious interrupt in ubd_handler, "
			       "err = %d\n", -n);
			return;
		}
<<<<<<< HEAD

		blk_end_request(req->req, 0, req->length);
		kfree(req);
	}
	reactivate_fd(thread_fd, UBD_IRQ);

	list_for_each_safe(list, next_ele, &restart){
		ubd = container_of(list, struct ubd, restart);
		list_del_init(&ubd->restart);
		spin_lock_irqsave(&ubd->lock, flags);
		do_ubd_request(ubd->queue);
		spin_unlock_irqrestore(&ubd->lock, flags);
=======
		for (count = 0; count < n/sizeof(struct io_thread_req *); count++) {
			struct io_thread_req *io_req = (*irq_req_buffer)[count];

			if ((io_req->error == BLK_STS_NOTSUPP) && (req_op(io_req->req) == REQ_OP_DISCARD)) {
				blk_queue_max_discard_sectors(io_req->req->q, 0);
				blk_queue_max_write_zeroes_sectors(io_req->req->q, 0);
			}
			blk_mq_end_request(io_req->req, io_req->error);
			kfree(io_req);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static irqreturn_t ubd_intr(int irq, void *dev)
{
	ubd_handler();
	return IRQ_HANDLED;
}

/* Only changed by ubd_init, which is an initcall. */
static int io_pid = -1;

static void kill_io_thread(void)
{
	if(io_pid != -1)
		os_kill_process(io_pid, 1);
}

__uml_exitcall(kill_io_thread);

static inline int ubd_file_size(struct ubd *ubd_dev, __u64 *size_out)
{
	char *file;
	int fd;
	int err;

	__u32 version;
	__u32 align;
	char *backing_file;
<<<<<<< HEAD
	time_t mtime;
=======
	time64_t mtime;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long long size;
	int sector_size;
	int bitmap_offset;

	if (ubd_dev->file && ubd_dev->cow.file) {
		file = ubd_dev->cow.file;

		goto out;
	}

<<<<<<< HEAD
	fd = os_open_file(ubd_dev->file, global_openflags, 0);
=======
	fd = os_open_file(ubd_dev->file, of_read(OPENFLAGS()), 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (fd < 0)
		return fd;

	err = read_cow_header(file_reader, &fd, &version, &backing_file, \
		&mtime, &size, &sector_size, &align, &bitmap_offset);
	os_close_file(fd);

	if(err == -EINVAL)
		file = ubd_dev->file;
	else
		file = backing_file;

out:
	return os_file_size(file, size_out);
}

static int read_cow_bitmap(int fd, void *buf, int offset, int len)
{
	int err;

<<<<<<< HEAD
	err = os_seek_file(fd, offset);
	if (err < 0)
		return err;

	err = os_read_file(fd, buf, len);
=======
	err = os_pread_file(fd, buf, len, offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	return 0;
}

<<<<<<< HEAD
static int backing_file_mismatch(char *file, __u64 size, time_t mtime)
{
	unsigned long modtime;
=======
static int backing_file_mismatch(char *file, __u64 size, time64_t mtime)
{
	time64_t modtime;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long long actual;
	int err;

	err = os_file_modtime(file, &modtime);
	if (err < 0) {
		printk(KERN_ERR "Failed to get modification time of backing "
		       "file \"%s\", err = %d\n", file, -err);
		return err;
	}

	err = os_file_size(file, &actual);
	if (err < 0) {
		printk(KERN_ERR "Failed to get size of backing file \"%s\", "
		       "err = %d\n", file, -err);
		return err;
	}

	if (actual != size) {
		/*__u64 can be a long on AMD64 and with %lu GCC complains; so
		 * the typecast.*/
		printk(KERN_ERR "Size mismatch (%llu vs %llu) of COW header "
		       "vs backing file\n", (unsigned long long) size, actual);
		return -EINVAL;
	}
	if (modtime != mtime) {
<<<<<<< HEAD
		printk(KERN_ERR "mtime mismatch (%ld vs %ld) of COW header vs "
=======
		printk(KERN_ERR "mtime mismatch (%lld vs %lld) of COW header vs "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       "backing file\n", mtime, modtime);
		return -EINVAL;
	}
	return 0;
}

static int path_requires_switch(char *from_cmdline, char *from_cow, char *cow)
{
	struct uml_stat buf1, buf2;
	int err;

	if (from_cmdline == NULL)
		return 0;
	if (!strcmp(from_cmdline, from_cow))
		return 0;

	err = os_stat_file(from_cmdline, &buf1);
	if (err < 0) {
		printk(KERN_ERR "Couldn't stat '%s', err = %d\n", from_cmdline,
		       -err);
		return 0;
	}
	err = os_stat_file(from_cow, &buf2);
	if (err < 0) {
		printk(KERN_ERR "Couldn't stat '%s', err = %d\n", from_cow,
		       -err);
		return 1;
	}
	if ((buf1.ust_dev == buf2.ust_dev) && (buf1.ust_ino == buf2.ust_ino))
		return 0;

	printk(KERN_ERR "Backing file mismatch - \"%s\" requested, "
	       "\"%s\" specified in COW header of \"%s\"\n",
	       from_cmdline, from_cow, cow);
	return 1;
}

static int open_ubd_file(char *file, struct openflags *openflags, int shared,
		  char **backing_file_out, int *bitmap_offset_out,
		  unsigned long *bitmap_len_out, int *data_offset_out,
		  int *create_cow_out)
{
<<<<<<< HEAD
	time_t mtime;
=======
	time64_t mtime;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long long size;
	__u32 version, align;
	char *backing_file;
	int fd, err, sectorsize, asked_switch, mode = 0644;

	fd = os_open_file(file, *openflags, mode);
	if (fd < 0) {
		if ((fd == -ENOENT) && (create_cow_out != NULL))
			*create_cow_out = 1;
		if (!openflags->w ||
		    ((fd != -EROFS) && (fd != -EACCES)))
			return fd;
		openflags->w = 0;
		fd = os_open_file(file, *openflags, mode);
		if (fd < 0)
			return fd;
	}

	if (shared)
		printk(KERN_INFO "Not locking \"%s\" on the host\n", file);
	else {
		err = os_lock_file(fd, openflags->w);
		if (err < 0) {
			printk(KERN_ERR "Failed to lock '%s', err = %d\n",
			       file, -err);
			goto out_close;
		}
	}

	/* Successful return case! */
	if (backing_file_out == NULL)
		return fd;

	err = read_cow_header(file_reader, &fd, &version, &backing_file, &mtime,
			      &size, &sectorsize, &align, bitmap_offset_out);
	if (err && (*backing_file_out != NULL)) {
		printk(KERN_ERR "Failed to read COW header from COW file "
		       "\"%s\", errno = %d\n", file, -err);
		goto out_close;
	}
	if (err)
		return fd;

	asked_switch = path_requires_switch(*backing_file_out, backing_file,
					    file);

	/* Allow switching only if no mismatch. */
	if (asked_switch && !backing_file_mismatch(*backing_file_out, size,
						   mtime)) {
		printk(KERN_ERR "Switching backing file to '%s'\n",
		       *backing_file_out);
		err = write_cow_header(file, fd, *backing_file_out,
				       sectorsize, align, &size);
		if (err) {
			printk(KERN_ERR "Switch failed, errno = %d\n", -err);
			goto out_close;
		}
	} else {
		*backing_file_out = backing_file;
		err = backing_file_mismatch(*backing_file_out, size, mtime);
		if (err)
			goto out_close;
	}

	cow_sizes(version, size, sectorsize, align, *bitmap_offset_out,
		  bitmap_len_out, data_offset_out);

	return fd;
 out_close:
	os_close_file(fd);
	return err;
}

static int create_cow_file(char *cow_file, char *backing_file,
		    struct openflags flags,
		    int sectorsize, int alignment, int *bitmap_offset_out,
		    unsigned long *bitmap_len_out, int *data_offset_out)
{
	int err, fd;

	flags.c = 1;
	fd = open_ubd_file(cow_file, &flags, 0, NULL, NULL, NULL, NULL, NULL);
	if (fd < 0) {
		err = fd;
		printk(KERN_ERR "Open of COW file '%s' failed, errno = %d\n",
		       cow_file, -err);
		goto out;
	}

	err = init_cow_file(fd, cow_file, backing_file, sectorsize, alignment,
			    bitmap_offset_out, bitmap_len_out,
			    data_offset_out);
	if (!err)
		return fd;
	os_close_file(fd);
 out:
	return err;
}

static void ubd_close_dev(struct ubd *ubd_dev)
{
	os_close_file(ubd_dev->fd);
	if(ubd_dev->cow.file == NULL)
		return;

	os_close_file(ubd_dev->cow.fd);
	vfree(ubd_dev->cow.bitmap);
	ubd_dev->cow.bitmap = NULL;
}

static int ubd_open_dev(struct ubd *ubd_dev)
{
	struct openflags flags;
	char **back_ptr;
	int err, create_cow, *create_ptr;
	int fd;

	ubd_dev->openflags = ubd_dev->boot_openflags;
	create_cow = 0;
	create_ptr = (ubd_dev->cow.file != NULL) ? &create_cow : NULL;
	back_ptr = ubd_dev->no_cow ? NULL : &ubd_dev->cow.file;

	fd = open_ubd_file(ubd_dev->file, &ubd_dev->openflags, ubd_dev->shared,
				back_ptr, &ubd_dev->cow.bitmap_offset,
				&ubd_dev->cow.bitmap_len, &ubd_dev->cow.data_offset,
				create_ptr);

	if((fd == -ENOENT) && create_cow){
		fd = create_cow_file(ubd_dev->file, ubd_dev->cow.file,
<<<<<<< HEAD
					  ubd_dev->openflags, 1 << 9, PAGE_SIZE,
=======
					  ubd_dev->openflags, SECTOR_SIZE, PAGE_SIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					  &ubd_dev->cow.bitmap_offset,
					  &ubd_dev->cow.bitmap_len,
					  &ubd_dev->cow.data_offset);
		if(fd >= 0){
			printk(KERN_INFO "Creating \"%s\" as COW file for "
			       "\"%s\"\n", ubd_dev->file, ubd_dev->cow.file);
		}
	}

	if(fd < 0){
		printk("Failed to open '%s', errno = %d\n", ubd_dev->file,
		       -fd);
		return fd;
	}
	ubd_dev->fd = fd;

	if(ubd_dev->cow.file != NULL){
<<<<<<< HEAD
		blk_queue_max_hw_sectors(ubd_dev->queue, 8 * sizeof(long));

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOMEM;
		ubd_dev->cow.bitmap = vmalloc(ubd_dev->cow.bitmap_len);
		if(ubd_dev->cow.bitmap == NULL){
			printk(KERN_ERR "Failed to vmalloc COW bitmap\n");
			goto error;
		}
		flush_tlb_kernel_vm();

		err = read_cow_bitmap(ubd_dev->fd, ubd_dev->cow.bitmap,
				      ubd_dev->cow.bitmap_offset,
				      ubd_dev->cow.bitmap_len);
		if(err < 0)
			goto error;

		flags = ubd_dev->openflags;
		flags.w = 0;
		err = open_ubd_file(ubd_dev->cow.file, &flags, ubd_dev->shared, NULL,
				    NULL, NULL, NULL, NULL);
		if(err < 0) goto error;
		ubd_dev->cow.fd = err;
	}
	return 0;
 error:
	os_close_file(ubd_dev->fd);
	return err;
}

static void ubd_device_release(struct device *dev)
{
	struct ubd *ubd_dev = dev_get_drvdata(dev);

<<<<<<< HEAD
	blk_cleanup_queue(ubd_dev->queue);
	*ubd_dev = ((struct ubd) DEFAULT_UBD);
}

static int ubd_disk_register(int major, u64 size, int unit,
			     struct gendisk **disk_out)
{
	struct gendisk *disk;

	disk = alloc_disk(1 << UBD_SHIFT);
	if(disk == NULL)
		return -ENOMEM;

	disk->major = major;
	disk->first_minor = unit << UBD_SHIFT;
	disk->fops = &ubd_blops;
	set_capacity(disk, size / 512);
	if (major == UBD_MAJOR)
		sprintf(disk->disk_name, "ubd%c", 'a' + unit);
	else
		sprintf(disk->disk_name, "ubd_fake%d", unit);

	/* sysfs register (not for ide fake devices) */
	if (major == UBD_MAJOR) {
		ubd_devs[unit].pdev.id   = unit;
		ubd_devs[unit].pdev.name = DRIVER_NAME;
		ubd_devs[unit].pdev.dev.release = ubd_device_release;
		dev_set_drvdata(&ubd_devs[unit].pdev.dev, &ubd_devs[unit]);
		platform_device_register(&ubd_devs[unit].pdev);
		disk->driverfs_dev = &ubd_devs[unit].pdev.dev;
	}

	disk->private_data = &ubd_devs[unit];
	disk->queue = ubd_devs[unit].queue;
	add_disk(disk);

	*disk_out = disk;
	return 0;
}

#define ROUND_BLOCK(n) ((n + ((1 << 9) - 1)) & (-1 << 9))
=======
	blk_mq_free_tag_set(&ubd_dev->tag_set);
	*ubd_dev = ((struct ubd) DEFAULT_UBD);
}

static ssize_t serial_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct ubd *ubd_dev = disk->private_data;

	if (!ubd_dev)
		return 0;

	return sprintf(buf, "%s", ubd_dev->serial);
}

static DEVICE_ATTR_RO(serial);

static struct attribute *ubd_attrs[] = {
	&dev_attr_serial.attr,
	NULL,
};

static umode_t ubd_attrs_are_visible(struct kobject *kobj,
				     struct attribute *a, int n)
{
	return a->mode;
}

static const struct attribute_group ubd_attr_group = {
	.attrs = ubd_attrs,
	.is_visible = ubd_attrs_are_visible,
};

static const struct attribute_group *ubd_attr_groups[] = {
	&ubd_attr_group,
	NULL,
};

#define ROUND_BLOCK(n) ((n + (SECTOR_SIZE - 1)) & (-SECTOR_SIZE))

static const struct blk_mq_ops ubd_mq_ops = {
	.queue_rq = ubd_queue_rq,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int ubd_add(int n, char **error_out)
{
	struct ubd *ubd_dev = &ubd_devs[n];
<<<<<<< HEAD
=======
	struct queue_limits lim = {
		.max_segments		= MAX_SG,
		.seg_boundary_mask	= PAGE_SIZE - 1,
	};
	struct gendisk *disk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	if(ubd_dev->file == NULL)
		goto out;

<<<<<<< HEAD
=======
	if (ubd_dev->cow.file)
		lim.max_hw_sectors = 8 * sizeof(long);
	if (!ubd_dev->no_trim) {
		lim.max_hw_discard_sectors = UBD_MAX_REQUEST;
		lim.max_write_zeroes_sectors = UBD_MAX_REQUEST;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ubd_file_size(ubd_dev, &ubd_dev->size);
	if(err < 0){
		*error_out = "Couldn't determine size of device's file";
		goto out;
	}

<<<<<<< HEAD
	ubd_dev->size = ROUND_BLOCK(ubd_dev->size);

	INIT_LIST_HEAD(&ubd_dev->restart);
	sg_init_table(ubd_dev->sg, MAX_SG);

	err = -ENOMEM;
	ubd_dev->queue = blk_init_queue(do_ubd_request, &ubd_dev->lock);
	if (ubd_dev->queue == NULL) {
		*error_out = "Failed to initialize device queue";
		goto out;
	}
	ubd_dev->queue->queuedata = ubd_dev;

	blk_queue_max_segments(ubd_dev->queue, MAX_SG);
	err = ubd_disk_register(UBD_MAJOR, ubd_dev->size, n, &ubd_gendisk[n]);
	if(err){
		*error_out = "Failed to register device";
		goto out_cleanup;
	}

	if (fake_major != UBD_MAJOR)
		ubd_disk_register(fake_major, ubd_dev->size, n,
				  &fake_gendisk[n]);

	/*
	 * Perhaps this should also be under the "if (fake_major)" above
	 * using the fake_disk->disk_name
	 */
	if (fake_ide)
		make_ide_entries(ubd_gendisk[n]->disk_name);

	err = 0;
out:
	return err;

out_cleanup:
	blk_cleanup_queue(ubd_dev->queue);
	goto out;
=======
	err = ubd_open_dev(ubd_dev);
	if (err) {
		pr_err("ubd%c: Can't open \"%s\": errno = %d\n",
			'a' + n, ubd_dev->file, -err);
		goto out;
	}

	ubd_dev->size = ROUND_BLOCK(ubd_dev->size);

	ubd_dev->tag_set.ops = &ubd_mq_ops;
	ubd_dev->tag_set.queue_depth = 64;
	ubd_dev->tag_set.numa_node = NUMA_NO_NODE;
	ubd_dev->tag_set.flags = BLK_MQ_F_SHOULD_MERGE;
	ubd_dev->tag_set.driver_data = ubd_dev;
	ubd_dev->tag_set.nr_hw_queues = 1;

	err = blk_mq_alloc_tag_set(&ubd_dev->tag_set);
	if (err)
		goto out_close;

	disk = blk_mq_alloc_disk(&ubd_dev->tag_set, &lim, ubd_dev);
	if (IS_ERR(disk)) {
		err = PTR_ERR(disk);
		goto out_cleanup_tags;
	}

	blk_queue_flag_set(QUEUE_FLAG_NONROT, disk->queue);
	blk_queue_write_cache(disk->queue, true, false);
	disk->major = UBD_MAJOR;
	disk->first_minor = n << UBD_SHIFT;
	disk->minors = 1 << UBD_SHIFT;
	disk->fops = &ubd_blops;
	set_capacity(disk, ubd_dev->size / 512);
	sprintf(disk->disk_name, "ubd%c", 'a' + n);
	disk->private_data = ubd_dev;
	set_disk_ro(disk, !ubd_dev->openflags.w);

	ubd_dev->pdev.id = n;
	ubd_dev->pdev.name = DRIVER_NAME;
	ubd_dev->pdev.dev.release = ubd_device_release;
	dev_set_drvdata(&ubd_dev->pdev.dev, ubd_dev);
	platform_device_register(&ubd_dev->pdev);

	err = device_add_disk(&ubd_dev->pdev.dev, disk, ubd_attr_groups);
	if (err)
		goto out_cleanup_disk;

	return 0;

out_cleanup_disk:
	put_disk(disk);
out_cleanup_tags:
	blk_mq_free_tag_set(&ubd_dev->tag_set);
out_close:
	ubd_close_dev(ubd_dev);
out:
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ubd_config(char *str, char **error_out)
{
	int n, ret;

	/* This string is possibly broken up and stored, so it's only
	 * freed if ubd_setup_common fails, or if only general options
	 * were set.
	 */
	str = kstrdup(str, GFP_KERNEL);
	if (str == NULL) {
		*error_out = "Failed to allocate memory";
		return -ENOMEM;
	}

	ret = ubd_setup_common(str, &n, error_out);
	if (ret)
		goto err_free;

	if (n == -1) {
		ret = 0;
		goto err_free;
	}

	mutex_lock(&ubd_lock);
	ret = ubd_add(n, error_out);
	if (ret)
		ubd_devs[n].file = NULL;
	mutex_unlock(&ubd_lock);

out:
	return ret;

err_free:
	kfree(str);
	goto out;
}

static int ubd_get_config(char *name, char *str, int size, char **error_out)
{
	struct ubd *ubd_dev;
	int n, len = 0;

	n = parse_unit(&name);
	if((n >= MAX_DEV) || (n < 0)){
		*error_out = "ubd_get_config : device number out of range";
		return -1;
	}

	ubd_dev = &ubd_devs[n];
	mutex_lock(&ubd_lock);

	if(ubd_dev->file == NULL){
		CONFIG_CHUNK(str, size, len, "", 1);
		goto out;
	}

	CONFIG_CHUNK(str, size, len, ubd_dev->file, 0);

	if(ubd_dev->cow.file != NULL){
		CONFIG_CHUNK(str, size, len, ",", 0);
		CONFIG_CHUNK(str, size, len, ubd_dev->cow.file, 1);
	}
	else CONFIG_CHUNK(str, size, len, "", 1);

 out:
	mutex_unlock(&ubd_lock);
	return len;
}

static int ubd_id(char **str, int *start_out, int *end_out)
{
	int n;

	n = parse_unit(str);
	*start_out = 0;
	*end_out = MAX_DEV - 1;
	return n;
}

static int ubd_remove(int n, char **error_out)
{
<<<<<<< HEAD
	struct gendisk *disk = ubd_gendisk[n];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ubd *ubd_dev;
	int err = -ENODEV;

	mutex_lock(&ubd_lock);

	ubd_dev = &ubd_devs[n];

	if(ubd_dev->file == NULL)
		goto out;

<<<<<<< HEAD
	/* you cannot remove a open disk */
	err = -EBUSY;
	if(ubd_dev->count > 0)
		goto out;

	ubd_gendisk[n] = NULL;
	if(disk != NULL){
		del_gendisk(disk);
		put_disk(disk);
	}

	if(fake_gendisk[n] != NULL){
		del_gendisk(fake_gendisk[n]);
		put_disk(fake_gendisk[n]);
		fake_gendisk[n] = NULL;
=======
	if (ubd_dev->disk) {
		/* you cannot remove a open disk */
		err = -EBUSY;
		if (disk_openers(ubd_dev->disk))
			goto out;

		del_gendisk(ubd_dev->disk);
		ubd_close_dev(ubd_dev);
		put_disk(ubd_dev->disk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = 0;
	platform_device_unregister(&ubd_dev->pdev);
out:
	mutex_unlock(&ubd_lock);
	return err;
}

/* All these are called by mconsole in process context and without
 * ubd-specific locks.  The structure itself is const except for .list.
 */
static struct mc_device ubd_mc = {
	.list		= LIST_HEAD_INIT(ubd_mc.list),
	.name		= "ubd",
	.config		= ubd_config,
	.get_config	= ubd_get_config,
	.id		= ubd_id,
	.remove		= ubd_remove,
};

static int __init ubd_mc_init(void)
{
	mconsole_register_dev(&ubd_mc);
	return 0;
}

__initcall(ubd_mc_init);

static int __init ubd0_init(void)
{
	struct ubd *ubd_dev = &ubd_devs[0];

	mutex_lock(&ubd_lock);
	if(ubd_dev->file == NULL)
		ubd_dev->file = "root_fs";
	mutex_unlock(&ubd_lock);

	return 0;
}

__initcall(ubd0_init);

/* Used in ubd_init, which is an initcall */
static struct platform_driver ubd_driver = {
	.driver = {
		.name  = DRIVER_NAME,
	},
};

static int __init ubd_init(void)
{
	char *error;
	int i, err;

	if (register_blkdev(UBD_MAJOR, "ubd"))
		return -1;

<<<<<<< HEAD
	if (fake_major != UBD_MAJOR) {
		char name[sizeof("ubd_nnn\0")];

		snprintf(name, sizeof(name), "ubd_%d", fake_major);
		if (register_blkdev(fake_major, "ubd"))
			return -1;
=======
	irq_req_buffer = kmalloc_array(UBD_REQ_BUFFER_SIZE,
				       sizeof(struct io_thread_req *),
				       GFP_KERNEL
		);
	irq_remainder = 0;

	if (irq_req_buffer == NULL) {
		printk(KERN_ERR "Failed to initialize ubd buffering\n");
		return -1;
	}
	io_req_buffer = kmalloc_array(UBD_REQ_BUFFER_SIZE,
				      sizeof(struct io_thread_req *),
				      GFP_KERNEL
		);

	io_remainder = 0;

	if (io_req_buffer == NULL) {
		printk(KERN_ERR "Failed to initialize ubd buffering\n");
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	platform_driver_register(&ubd_driver);
	mutex_lock(&ubd_lock);
	for (i = 0; i < MAX_DEV; i++){
		err = ubd_add(i, &error);
		if(err)
			printk(KERN_ERR "Failed to initialize ubd device %d :"
			       "%s\n", i, error);
	}
	mutex_unlock(&ubd_lock);
	return 0;
}

late_initcall(ubd_init);

static int __init ubd_driver_init(void){
	unsigned long stack;
	int err;

	/* Set by CONFIG_BLK_DEV_UBD_SYNC or ubd=sync.*/
	if(global_openflags.s){
		printk(KERN_INFO "ubd: Synchronous mode\n");
		/* Letting ubd=sync be like using ubd#s= instead of ubd#= is
		 * enough. So use anyway the io thread. */
	}
	stack = alloc_stack(0, 0);
<<<<<<< HEAD
	io_pid = start_io_thread(stack + PAGE_SIZE - sizeof(void *),
				 &thread_fd);
=======
	io_pid = start_io_thread(stack + PAGE_SIZE, &thread_fd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if(io_pid < 0){
		printk(KERN_ERR
		       "ubd : Failed to start I/O thread (errno = %d) - "
		       "falling back to synchronous I/O\n", -io_pid);
		io_pid = -1;
		return 0;
	}
	err = um_request_irq(UBD_IRQ, thread_fd, IRQ_READ, ubd_intr,
			     0, "ubd", ubd_devs);
<<<<<<< HEAD
	if(err != 0)
=======
	if(err < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		printk(KERN_ERR "um_request_irq failed - errno = %d\n", -err);
	return 0;
}

device_initcall(ubd_driver_init);

<<<<<<< HEAD
static int ubd_open(struct block_device *bdev, fmode_t mode)
{
	struct gendisk *disk = bdev->bd_disk;
	struct ubd *ubd_dev = disk->private_data;
	int err = 0;

	mutex_lock(&ubd_mutex);
	if(ubd_dev->count == 0){
		err = ubd_open_dev(ubd_dev);
		if(err){
			printk(KERN_ERR "%s: Can't open \"%s\": errno = %d\n",
			       disk->disk_name, ubd_dev->file, -err);
			goto out;
		}
	}
	ubd_dev->count++;
	set_disk_ro(disk, !ubd_dev->openflags.w);

	/* This should no more be needed. And it didn't work anyway to exclude
	 * read-write remounting of filesystems.*/
	/*if((mode & FMODE_WRITE) && !ubd_dev->openflags.w){
	        if(--ubd_dev->count == 0) ubd_close_dev(ubd_dev);
	        err = -EROFS;
	}*/
out:
	mutex_unlock(&ubd_mutex);
	return err;
}

static int ubd_release(struct gendisk *disk, fmode_t mode)
{
	struct ubd *ubd_dev = disk->private_data;

	mutex_lock(&ubd_mutex);
	if(--ubd_dev->count == 0)
		ubd_close_dev(ubd_dev);
	mutex_unlock(&ubd_mutex);
	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void cowify_bitmap(__u64 io_offset, int length, unsigned long *cow_mask,
			  __u64 *cow_offset, unsigned long *bitmap,
			  __u64 bitmap_offset, unsigned long *bitmap_words,
			  __u64 bitmap_len)
{
<<<<<<< HEAD
	__u64 sector = io_offset >> 9;
	int i, update_bitmap = 0;

	for(i = 0; i < length >> 9; i++){
=======
	__u64 sector = io_offset >> SECTOR_SHIFT;
	int i, update_bitmap = 0;

	for (i = 0; i < length >> SECTOR_SHIFT; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if(cow_mask != NULL)
			ubd_set_bit(i, (unsigned char *) cow_mask);
		if(ubd_test_bit(sector + i, (unsigned char *) bitmap))
			continue;

		update_bitmap = 1;
		ubd_set_bit(sector + i, (unsigned char *) bitmap);
	}

	if(!update_bitmap)
		return;

	*cow_offset = sector / (sizeof(unsigned long) * 8);

	/* This takes care of the case where we're exactly at the end of the
	 * device, and *cow_offset + 1 is off the end.  So, just back it up
	 * by one word.  Thanks to Lynn Kerby for the fix and James McMechan
	 * for the original diagnosis.
	 */
	if (*cow_offset == (DIV_ROUND_UP(bitmap_len,
					 sizeof(unsigned long)) - 1))
		(*cow_offset)--;

	bitmap_words[0] = bitmap[*cow_offset];
	bitmap_words[1] = bitmap[*cow_offset + 1];

	*cow_offset *= sizeof(unsigned long);
	*cow_offset += bitmap_offset;
}

<<<<<<< HEAD
static void cowify_req(struct io_thread_req *req, unsigned long *bitmap,
		       __u64 bitmap_offset, __u64 bitmap_len)
{
	__u64 sector = req->offset >> 9;
	int i;

	if(req->length > (sizeof(req->sector_mask) * 8) << 9)
		panic("Operation too long");

	if(req->op == UBD_READ) {
		for(i = 0; i < req->length >> 9; i++){
			if(ubd_test_bit(sector + i, (unsigned char *) bitmap))
				ubd_set_bit(i, (unsigned char *)
					    &req->sector_mask);
		}
	}
	else cowify_bitmap(req->offset, req->length, &req->sector_mask,
			   &req->cow_offset, bitmap, bitmap_offset,
			   req->bitmap_words, bitmap_len);
}

/* Called with dev->lock held */
static void prepare_request(struct request *req, struct io_thread_req *io_req,
			    unsigned long long offset, int page_offset,
			    int len, struct page *page)
{
	struct gendisk *disk = req->rq_disk;
	struct ubd *ubd_dev = disk->private_data;

	io_req->req = req;
	io_req->fds[0] = (ubd_dev->cow.file != NULL) ? ubd_dev->cow.fd :
		ubd_dev->fd;
	io_req->fds[1] = ubd_dev->fd;
	io_req->cow_offset = -1;
	io_req->offset = offset;
	io_req->length = len;
	io_req->error = 0;
	io_req->sector_mask = 0;

	io_req->op = (rq_data_dir(req) == READ) ? UBD_READ : UBD_WRITE;
	io_req->offsets[0] = 0;
	io_req->offsets[1] = ubd_dev->cow.data_offset;
	io_req->buffer = page_address(page) + page_offset;
	io_req->sectorsize = 1 << 9;

	if(ubd_dev->cow.file != NULL)
		cowify_req(io_req, ubd_dev->cow.bitmap,
			   ubd_dev->cow.bitmap_offset, ubd_dev->cow.bitmap_len);

}

/* Called with dev->lock held */
static void do_ubd_request(struct request_queue *q)
{
	struct io_thread_req *io_req;
	struct request *req;
	int n;

	while(1){
		struct ubd *dev = q->queuedata;
		if(dev->end_sg == 0){
			struct request *req = blk_fetch_request(q);
			if(req == NULL)
				return;

			dev->request = req;
			dev->rq_pos = blk_rq_pos(req);
			dev->start_sg = 0;
			dev->end_sg = blk_rq_map_sg(q, req, dev->sg);
		}

		req = dev->request;
		while(dev->start_sg < dev->end_sg){
			struct scatterlist *sg = &dev->sg[dev->start_sg];

			io_req = kmalloc(sizeof(struct io_thread_req),
					 GFP_ATOMIC);
			if(io_req == NULL){
				if(list_empty(&dev->restart))
					list_add(&dev->restart, &restart);
				return;
			}
			prepare_request(req, io_req,
					(unsigned long long)dev->rq_pos << 9,
					sg->offset, sg->length, sg_page(sg));

			n = os_write_file(thread_fd, &io_req,
					  sizeof(struct io_thread_req *));
			if(n != sizeof(struct io_thread_req *)){
				if(n != -EAGAIN)
					printk("write to io thread failed, "
					       "errno = %d\n", -n);
				else if(list_empty(&dev->restart))
					list_add(&dev->restart, &restart);
				kfree(io_req);
				return;
			}

			dev->rq_pos += sg->length >> 9;
			dev->start_sg++;
		}
		dev->end_sg = 0;
		dev->request = NULL;
	}
=======
static void cowify_req(struct io_thread_req *req, struct io_desc *segment,
		       unsigned long offset, unsigned long *bitmap,
		       __u64 bitmap_offset, __u64 bitmap_len)
{
	__u64 sector = offset >> SECTOR_SHIFT;
	int i;

	if (segment->length > (sizeof(segment->sector_mask) * 8) << SECTOR_SHIFT)
		panic("Operation too long");

	if (req_op(req->req) == REQ_OP_READ) {
		for (i = 0; i < segment->length >> SECTOR_SHIFT; i++) {
			if(ubd_test_bit(sector + i, (unsigned char *) bitmap))
				ubd_set_bit(i, (unsigned char *)
					    &segment->sector_mask);
		}
	} else {
		cowify_bitmap(offset, segment->length, &segment->sector_mask,
			      &segment->cow_offset, bitmap, bitmap_offset,
			      segment->bitmap_words, bitmap_len);
	}
}

static void ubd_map_req(struct ubd *dev, struct io_thread_req *io_req,
			struct request *req)
{
	struct bio_vec bvec;
	struct req_iterator iter;
	int i = 0;
	unsigned long byte_offset = io_req->offset;
	enum req_op op = req_op(req);

	if (op == REQ_OP_WRITE_ZEROES || op == REQ_OP_DISCARD) {
		io_req->io_desc[0].buffer = NULL;
		io_req->io_desc[0].length = blk_rq_bytes(req);
	} else {
		rq_for_each_segment(bvec, req, iter) {
			BUG_ON(i >= io_req->desc_cnt);

			io_req->io_desc[i].buffer = bvec_virt(&bvec);
			io_req->io_desc[i].length = bvec.bv_len;
			i++;
		}
	}

	if (dev->cow.file) {
		for (i = 0; i < io_req->desc_cnt; i++) {
			cowify_req(io_req, &io_req->io_desc[i], byte_offset,
				   dev->cow.bitmap, dev->cow.bitmap_offset,
				   dev->cow.bitmap_len);
			byte_offset += io_req->io_desc[i].length;
		}

	}
}

static struct io_thread_req *ubd_alloc_req(struct ubd *dev, struct request *req,
					   int desc_cnt)
{
	struct io_thread_req *io_req;
	int i;

	io_req = kmalloc(sizeof(*io_req) +
			 (desc_cnt * sizeof(struct io_desc)),
			 GFP_ATOMIC);
	if (!io_req)
		return NULL;

	io_req->req = req;
	if (dev->cow.file)
		io_req->fds[0] = dev->cow.fd;
	else
		io_req->fds[0] = dev->fd;
	io_req->error = 0;
	io_req->sectorsize = SECTOR_SIZE;
	io_req->fds[1] = dev->fd;
	io_req->offset = (u64) blk_rq_pos(req) << SECTOR_SHIFT;
	io_req->offsets[0] = 0;
	io_req->offsets[1] = dev->cow.data_offset;

	for (i = 0 ; i < desc_cnt; i++) {
		io_req->io_desc[i].sector_mask = 0;
		io_req->io_desc[i].cow_offset = -1;
	}

	return io_req;
}

static int ubd_submit_request(struct ubd *dev, struct request *req)
{
	int segs = 0;
	struct io_thread_req *io_req;
	int ret;
	enum req_op op = req_op(req);

	if (op == REQ_OP_FLUSH)
		segs = 0;
	else if (op == REQ_OP_WRITE_ZEROES || op == REQ_OP_DISCARD)
		segs = 1;
	else
		segs = blk_rq_nr_phys_segments(req);

	io_req = ubd_alloc_req(dev, req, segs);
	if (!io_req)
		return -ENOMEM;

	io_req->desc_cnt = segs;
	if (segs)
		ubd_map_req(dev, io_req, req);

	ret = os_write_file(thread_fd, &io_req, sizeof(io_req));
	if (ret != sizeof(io_req)) {
		if (ret != -EAGAIN)
			pr_err("write to io thread failed: %d\n", -ret);
		kfree(io_req);
	}
	return ret;
}

static blk_status_t ubd_queue_rq(struct blk_mq_hw_ctx *hctx,
				 const struct blk_mq_queue_data *bd)
{
	struct ubd *ubd_dev = hctx->queue->queuedata;
	struct request *req = bd->rq;
	int ret = 0, res = BLK_STS_OK;

	blk_mq_start_request(req);

	spin_lock_irq(&ubd_dev->lock);

	switch (req_op(req)) {
	case REQ_OP_FLUSH:
	case REQ_OP_READ:
	case REQ_OP_WRITE:
	case REQ_OP_DISCARD:
	case REQ_OP_WRITE_ZEROES:
		ret = ubd_submit_request(ubd_dev, req);
		break;
	default:
		WARN_ON_ONCE(1);
		res = BLK_STS_NOTSUPP;
	}

	spin_unlock_irq(&ubd_dev->lock);

	if (ret < 0) {
		if (ret == -ENOMEM)
			res = BLK_STS_RESOURCE;
		else
			res = BLK_STS_DEV_RESOURCE;
	}

	return res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ubd_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	struct ubd *ubd_dev = bdev->bd_disk->private_data;

	geo->heads = 128;
	geo->sectors = 32;
	geo->cylinders = ubd_dev->size / (128 * 32 * 512);
	return 0;
}

<<<<<<< HEAD
static int ubd_ioctl(struct block_device *bdev, fmode_t mode,
=======
static int ubd_ioctl(struct block_device *bdev, blk_mode_t mode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     unsigned int cmd, unsigned long arg)
{
	struct ubd *ubd_dev = bdev->bd_disk->private_data;
	u16 ubd_id[ATA_ID_WORDS];

	switch (cmd) {
		struct cdrom_volctrl volume;
	case HDIO_GET_IDENTITY:
		memset(&ubd_id, 0, ATA_ID_WORDS * 2);
		ubd_id[ATA_ID_CYLS]	= ubd_dev->size / (128 * 32 * 512);
		ubd_id[ATA_ID_HEADS]	= 128;
		ubd_id[ATA_ID_SECTORS]	= 32;
		if(copy_to_user((char __user *) arg, (char *) &ubd_id,
				 sizeof(ubd_id)))
			return -EFAULT;
		return 0;

	case CDROMVOLREAD:
		if(copy_from_user(&volume, (char __user *) arg, sizeof(volume)))
			return -EFAULT;
		volume.channel0 = 255;
		volume.channel1 = 255;
		volume.channel2 = 255;
		volume.channel3 = 255;
		if(copy_to_user((char __user *) arg, &volume, sizeof(volume)))
			return -EFAULT;
		return 0;
	}
	return -EINVAL;
}

<<<<<<< HEAD
static int update_bitmap(struct io_thread_req *req)
{
	int n;

	if(req->cow_offset == -1)
		return 0;

	n = os_seek_file(req->fds[1], req->cow_offset);
	if(n < 0){
		printk("do_io - bitmap lseek failed : err = %d\n", -n);
		return 1;
	}

	n = os_write_file(req->fds[1], &req->bitmap_words,
			  sizeof(req->bitmap_words));
	if(n != sizeof(req->bitmap_words)){
		printk("do_io - bitmap update failed, err = %d fd = %d\n", -n,
		       req->fds[1]);
		return 1;
	}

	return 0;
}

static void do_io(struct io_thread_req *req)
{
	char *buf;
	unsigned long len;
	int n, nsectors, start, end, bit;
	int err;
	__u64 off;

	nsectors = req->length / req->sectorsize;
	start = 0;
	do {
		bit = ubd_test_bit(start, (unsigned char *) &req->sector_mask);
		end = start;
		while((end < nsectors) &&
		      (ubd_test_bit(end, (unsigned char *)
				    &req->sector_mask) == bit))
=======
static int map_error(int error_code)
{
	switch (error_code) {
	case 0:
		return BLK_STS_OK;
	case ENOSYS:
	case EOPNOTSUPP:
		return BLK_STS_NOTSUPP;
	case ENOSPC:
		return BLK_STS_NOSPC;
	}
	return BLK_STS_IOERR;
}

/*
 * Everything from here onwards *IS NOT PART OF THE KERNEL*
 *
 * The following functions are part of UML hypervisor code.
 * All functions from here onwards are executed as a helper
 * thread and are not allowed to execute any kernel functions.
 *
 * Any communication must occur strictly via shared memory and IPC.
 *
 * Do not add printks, locks, kernel memory operations, etc - it
 * will result in unpredictable behaviour and/or crashes.
 */

static int update_bitmap(struct io_thread_req *req, struct io_desc *segment)
{
	int n;

	if (segment->cow_offset == -1)
		return map_error(0);

	n = os_pwrite_file(req->fds[1], &segment->bitmap_words,
			  sizeof(segment->bitmap_words), segment->cow_offset);
	if (n != sizeof(segment->bitmap_words))
		return map_error(-n);

	return map_error(0);
}

static void do_io(struct io_thread_req *req, struct io_desc *desc)
{
	char *buf = NULL;
	unsigned long len;
	int n, nsectors, start, end, bit;
	__u64 off;

	/* FLUSH is really a special case, we cannot "case" it with others */

	if (req_op(req->req) == REQ_OP_FLUSH) {
		/* fds[0] is always either the rw image or our cow file */
		req->error = map_error(-os_sync_file(req->fds[0]));
		return;
	}

	nsectors = desc->length / req->sectorsize;
	start = 0;
	do {
		bit = ubd_test_bit(start, (unsigned char *) &desc->sector_mask);
		end = start;
		while((end < nsectors) &&
		      (ubd_test_bit(end, (unsigned char *) &desc->sector_mask) == bit))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			end++;

		off = req->offset + req->offsets[bit] +
			start * req->sectorsize;
		len = (end - start) * req->sectorsize;
<<<<<<< HEAD
		buf = &req->buffer[start * req->sectorsize];

		err = os_seek_file(req->fds[bit], off);
		if(err < 0){
			printk("do_io - lseek failed : err = %d\n", -err);
			req->error = 1;
			return;
		}
		if(req->op == UBD_READ){
=======
		if (desc->buffer != NULL)
			buf = &desc->buffer[start * req->sectorsize];

		switch (req_op(req->req)) {
		case REQ_OP_READ:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			n = 0;
			do {
				buf = &buf[n];
				len -= n;
<<<<<<< HEAD
				n = os_read_file(req->fds[bit], buf, len);
				if (n < 0) {
					printk("do_io - read failed, err = %d "
					       "fd = %d\n", -n, req->fds[bit]);
					req->error = 1;
=======
				n = os_pread_file(req->fds[bit], buf, len, off);
				if (n < 0) {
					req->error = map_error(-n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					return;
				}
			} while((n < len) && (n != 0));
			if (n < len) memset(&buf[n], 0, len - n);
<<<<<<< HEAD
		} else {
			n = os_write_file(req->fds[bit], buf, len);
			if(n != len){
				printk("do_io - write failed err = %d "
				       "fd = %d\n", -n, req->fds[bit]);
				req->error = 1;
				return;
			}
=======
			break;
		case REQ_OP_WRITE:
			n = os_pwrite_file(req->fds[bit], buf, len, off);
			if(n != len){
				req->error = map_error(-n);
				return;
			}
			break;
		case REQ_OP_DISCARD:
			n = os_falloc_punch(req->fds[bit], off, len);
			if (n) {
				req->error = map_error(-n);
				return;
			}
			break;
		case REQ_OP_WRITE_ZEROES:
			n = os_falloc_zeroes(req->fds[bit], off, len);
			if (n) {
				req->error = map_error(-n);
				return;
			}
			break;
		default:
			WARN_ON_ONCE(1);
			req->error = BLK_STS_NOTSUPP;
			return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		start = end;
	} while(start < nsectors);

<<<<<<< HEAD
	req->error = update_bitmap(req);
=======
	req->offset += len;
	req->error = update_bitmap(req, desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Changed in start_io_thread, which is serialized by being called only
 * from ubd_init, which is an initcall.
 */
int kernel_fd = -1;

/* Only changed by the io thread. XXX: currently unused. */
<<<<<<< HEAD
static int io_count = 0;

int io_thread(void *arg)
{
	struct io_thread_req *req;
	int n;

	ignore_sigwinch_sig();
	while(1){
		n = os_read_file(kernel_fd, &req,
				 sizeof(struct io_thread_req *));
		if(n != sizeof(struct io_thread_req *)){
			if(n < 0)
				printk("io_thread - read failed, fd = %d, "
				       "err = %d\n", kernel_fd, -n);
			else {
				printk("io_thread - short read, fd = %d, "
				       "length = %d\n", kernel_fd, n);
			}
			continue;
		}
		io_count++;
		do_io(req);
		n = os_write_file(kernel_fd, &req,
				  sizeof(struct io_thread_req *));
		if(n != sizeof(struct io_thread_req *))
			printk("io_thread - write failed, fd = %d, err = %d\n",
			       kernel_fd, -n);
=======
static int io_count;

int io_thread(void *arg)
{
	int n, count, written, res;

	os_fix_helper_signals();

	while(1){
		n = bulk_req_safe_read(
			kernel_fd,
			io_req_buffer,
			&io_remainder,
			&io_remainder_size,
			UBD_REQ_BUFFER_SIZE
		);
		if (n <= 0) {
			if (n == -EAGAIN)
				ubd_read_poll(-1);

			continue;
		}

		for (count = 0; count < n/sizeof(struct io_thread_req *); count++) {
			struct io_thread_req *req = (*io_req_buffer)[count];
			int i;

			io_count++;
			for (i = 0; !req->error && i < req->desc_cnt; i++)
				do_io(req, &(req->io_desc[i]));

		}

		written = 0;

		do {
			res = os_write_file(kernel_fd,
					    ((char *) io_req_buffer) + written,
					    n - written);
			if (res >= 0) {
				written += res;
			}
			if (written < n) {
				ubd_write_poll(-1);
			}
		} while (written < n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
