<<<<<<< HEAD
#ifndef _LINUX_FS_H
#define _LINUX_FS_H

/*
 * This file has definitions for some important file table
 * structures etc.
 */

#include <linux/limits.h>
#include <linux/ioctl.h>
#include <linux/blk_types.h>
#include <linux/types.h>

/*
 * It's silly to have NR_OPEN bigger than NR_FILE, but you can change
 * the file limit at runtime and only root can increase the per-process
 * nr_file rlimit, so it's safe to set up a ridiculously high absolute
 * upper limit on files-per-process.
 *
 * Some programs (notably those using select()) may have to be 
 * recompiled to take full advantage of the new limits..  
 */

/* Fixed constants first: */
#undef NR_OPEN
#define INR_OPEN_CUR 1024	/* Initial setting for nfile rlimits */
#define INR_OPEN_MAX 4096	/* Hard limit for nfile rlimits */

#define BLOCK_SIZE_BITS 10
#define BLOCK_SIZE (1<<BLOCK_SIZE_BITS)

#define SEEK_SET	0	/* seek relative to beginning of file */
#define SEEK_CUR	1	/* seek relative to current file position */
#define SEEK_END	2	/* seek relative to end of file */
#define SEEK_DATA	3	/* seek to the next data */
#define SEEK_HOLE	4	/* seek to the next hole */
#define SEEK_MAX	SEEK_HOLE

struct fstrim_range {
	__u64 start;
	__u64 len;
	__u64 minlen;
};

/* And dynamically-tunable limits and defaults: */
struct files_stat_struct {
	unsigned long nr_files;		/* read only */
	unsigned long nr_free_files;	/* read only */
	unsigned long max_files;		/* tunable */
};

struct inodes_stat_t {
	int nr_inodes;
	int nr_unused;
	int dummy[5];		/* padding for sysctl ABI compatibility */
};


#define NR_FILE  8192	/* this can well be larger on a larger system */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_H
#define _LINUX_FS_H

#include <linux/linkage.h>
#include <linux/wait_bit.h>
#include <linux/kdev_t.h>
#include <linux/dcache.h>
#include <linux/path.h>
#include <linux/stat.h>
#include <linux/cache.h>
#include <linux/list.h>
#include <linux/list_lru.h>
#include <linux/llist.h>
#include <linux/radix-tree.h>
#include <linux/xarray.h>
#include <linux/rbtree.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/bug.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/mm_types.h>
#include <linux/capability.h>
#include <linux/semaphore.h>
#include <linux/fcntl.h>
#include <linux/rculist_bl.h>
#include <linux/atomic.h>
#include <linux/shrinker.h>
#include <linux/migrate_mode.h>
#include <linux/uidgid.h>
#include <linux/lockdep.h>
#include <linux/percpu-rwsem.h>
#include <linux/workqueue.h>
#include <linux/delayed_call.h>
#include <linux/uuid.h>
#include <linux/errseq.h>
#include <linux/ioprio.h>
#include <linux/fs_types.h>
#include <linux/build_bug.h>
#include <linux/stddef.h>
#include <linux/mount.h>
#include <linux/cred.h>
#include <linux/mnt_idmapping.h>
#include <linux/slab.h>
#include <linux/maple_tree.h>
#include <linux/rw_hint.h>

#include <asm/byteorder.h>
#include <uapi/linux/fs.h>

struct backing_dev_info;
struct bdi_writeback;
struct bio;
struct io_comp_batch;
struct export_operations;
struct fiemap_extent_info;
struct hd_geometry;
struct iovec;
struct kiocb;
struct kobject;
struct pipe_inode_info;
struct poll_table_struct;
struct kstatfs;
struct vm_area_struct;
struct vfsmount;
struct cred;
struct swap_info_struct;
struct seq_file;
struct workqueue_struct;
struct iov_iter;
struct fscrypt_inode_info;
struct fscrypt_operations;
struct fsverity_info;
struct fsverity_operations;
struct fs_context;
struct fs_parameter_spec;
struct fileattr;
struct iomap_ops;

extern void __init inode_init(void);
extern void __init inode_init_early(void);
extern void __init files_init(void);
extern void __init files_maxfiles_init(void);

extern unsigned long get_max_files(void);
extern unsigned int sysctl_nr_open;

typedef __kernel_rwf_t rwf_t;

struct buffer_head;
typedef int (get_block_t)(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create);
typedef int (dio_iodone_t)(struct kiocb *iocb, loff_t offset,
			ssize_t bytes, void *private);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MAY_EXEC		0x00000001
#define MAY_WRITE		0x00000002
#define MAY_READ		0x00000004
#define MAY_APPEND		0x00000008
#define MAY_ACCESS		0x00000010
#define MAY_OPEN		0x00000020
#define MAY_CHDIR		0x00000040
/* called from RCU mode, don't block */
#define MAY_NOT_BLOCK		0x00000080

/*
 * flags in file.f_mode.  Note that FMODE_READ and FMODE_WRITE must correspond
<<<<<<< HEAD
 * to O_WRONLY and O_RDWR via the strange trick in __dentry_open()
=======
 * to O_WRONLY and O_RDWR via the strange trick in do_dentry_open()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/* file is open for reading */
#define FMODE_READ		((__force fmode_t)0x1)
/* file is open for writing */
#define FMODE_WRITE		((__force fmode_t)0x2)
/* file is seekable */
#define FMODE_LSEEK		((__force fmode_t)0x4)
/* file can be accessed using pread */
#define FMODE_PREAD		((__force fmode_t)0x8)
/* file can be accessed using pwrite */
#define FMODE_PWRITE		((__force fmode_t)0x10)
/* File is opened for execution with sys_execve / sys_uselib */
#define FMODE_EXEC		((__force fmode_t)0x20)
<<<<<<< HEAD
/* File is opened with O_NDELAY (only set for block devices) */
#define FMODE_NDELAY		((__force fmode_t)0x40)
/* File is opened with O_EXCL (only set for block devices) */
#define FMODE_EXCL		((__force fmode_t)0x80)
/* File is opened using open(.., 3, ..) and is writeable only for ioctls
   (specialy hack for floppy.c) */
#define FMODE_WRITE_IOCTL	((__force fmode_t)0x100)
=======
/* File writes are restricted (block device specific) */
#define FMODE_WRITE_RESTRICTED  ((__force fmode_t)0x40)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* 32bit hashes as llseek() offset (for directories) */
#define FMODE_32BITHASH         ((__force fmode_t)0x200)
/* 64bit hashes as llseek() offset (for directories) */
#define FMODE_64BITHASH         ((__force fmode_t)0x400)

/*
 * Don't update ctime and mtime.
 *
 * Currently a special hack for the XFS open_by_handle ioctl, but we'll
 * hopefully graduate it to a proper O_CMTIME flag supported by open(2) soon.
 */
#define FMODE_NOCMTIME		((__force fmode_t)0x800)

/* Expect random access pattern */
#define FMODE_RANDOM		((__force fmode_t)0x1000)

<<<<<<< HEAD
/* File is huge (eg. /dev/kmem): treat loff_t as unsigned */
=======
/* File is huge (eg. /dev/mem): treat loff_t as unsigned */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define FMODE_UNSIGNED_OFFSET	((__force fmode_t)0x2000)

/* File is opened with O_PATH; almost nothing can be done with it */
#define FMODE_PATH		((__force fmode_t)0x4000)

<<<<<<< HEAD
/* File hasn't page cache and can't be mmaped, for stackable filesystem */
#define FMODE_NONMAPPABLE       ((__force fmode_t)0x400000)

/* File was opened by fanotify and shouldn't generate fanotify events */
#define FMODE_NONOTIFY		((__force fmode_t)0x1000000)

/* File can be read using splice */
#define FMODE_SPLICE_READ       ((__force fmode_t)0x8000000)
/* File can be written using splice */
#define FMODE_SPLICE_WRITE      ((__force fmode_t)0x10000000)

/*
 * The below are the various read and write types that we support. Some of
 * them include behavioral modifiers that send information down to the
 * block layer and IO scheduler. Terminology:
 *
 *	The block layer uses device plugging to defer IO a little bit, in
 *	the hope that we will see more IO very shortly. This increases
 *	coalescing of adjacent IO and thus reduces the number of IOs we
 *	have to send to the device. It also allows for better queuing,
 *	if the IO isn't mergeable. If the caller is going to be waiting
 *	for the IO, then he must ensure that the device is unplugged so
 *	that the IO is dispatched to the driver.
 *
 *	All IO is handled async in Linux. This is fine for background
 *	writes, but for reads or writes that someone waits for completion
 *	on, we want to notify the block layer and IO scheduler so that they
 *	know about it. That allows them to make better scheduling
 *	decisions. So when the below references 'sync' and 'async', it
 *	is referencing this priority hint.
 *
 * With that in mind, the available types are:
 *
 * READ			A normal read operation. Device will be plugged.
 * READ_SYNC		A synchronous read. Device is not plugged, caller can
 *			immediately wait on this read without caring about
 *			unplugging.
 * READA		Used for read-ahead operations. Lower priority, and the
 *			block layer could (in theory) choose to ignore this
 *			request if it runs into resource problems.
 * WRITE		A normal async write. Device will be plugged.
 * WRITE_SYNC		Synchronous write. Identical to WRITE, but passes down
 *			the hint that someone will be waiting on this IO
 *			shortly. The write equivalent of READ_SYNC.
 * WRITE_ODIRECT	Special case write for O_DIRECT only.
 * WRITE_FLUSH		Like WRITE_SYNC but with preceding cache flush.
 * WRITE_FUA		Like WRITE_SYNC but data is guaranteed to be on
 *			non-volatile media on completion.
 * WRITE_FLUSH_FUA	Combination of WRITE_FLUSH and FUA. The IO is preceded
 *			by a cache flush and data is guaranteed to be on
 *			non-volatile media on completion.
 *
 */
#define RW_MASK			REQ_WRITE
#define RWA_MASK		REQ_RAHEAD

#define READ			0
#define WRITE			RW_MASK
#define READA			RWA_MASK

#define READ_SYNC		(READ | REQ_SYNC)
#define WRITE_SYNC		(WRITE | REQ_SYNC | REQ_NOIDLE)
#define WRITE_ODIRECT		(WRITE | REQ_SYNC)
#define WRITE_FLUSH		(WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FLUSH)
#define WRITE_FUA		(WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FUA)
#define WRITE_FLUSH_FUA		(WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FLUSH | REQ_FUA)

#define SEL_IN		1
#define SEL_OUT		2
#define SEL_EX		4

/* public flags for file_system_type */
#define FS_REQUIRES_DEV 1 
#define FS_BINARY_MOUNTDATA 2
#define FS_HAS_SUBTYPE 4
#define FS_REVAL_DOT	16384	/* Check the paths ".", ".." for staleness */
#define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move()
					 * during rename() internally.
					 */

/*
 * These are the fs-independent mount-flags: up to 32 flags are supported
 */
#define MS_RDONLY	 1	/* Mount read-only */
#define MS_NOSUID	 2	/* Ignore suid and sgid bits */
#define MS_NODEV	 4	/* Disallow access to device special files */
#define MS_NOEXEC	 8	/* Disallow program execution */
#define MS_SYNCHRONOUS	16	/* Writes are synced at once */
#define MS_REMOUNT	32	/* Alter flags of a mounted FS */
#define MS_MANDLOCK	64	/* Allow mandatory locks on an FS */
#define MS_DIRSYNC	128	/* Directory modifications are synchronous */
#define MS_NOATIME	1024	/* Do not update access times. */
#define MS_NODIRATIME	2048	/* Do not update directory access times */
#define MS_BIND		4096
#define MS_MOVE		8192
#define MS_REC		16384
#define MS_VERBOSE	32768	/* War is peace. Verbosity is silence.
				   MS_VERBOSE is deprecated. */
#define MS_SILENT	32768
#define MS_POSIXACL	(1<<16)	/* VFS does not apply the umask */
#define MS_UNBINDABLE	(1<<17)	/* change to unbindable */
#define MS_PRIVATE	(1<<18)	/* change to private */
#define MS_SLAVE	(1<<19)	/* change to slave */
#define MS_SHARED	(1<<20)	/* change to shared */
#define MS_RELATIME	(1<<21)	/* Update atime relative to mtime/ctime. */
#define MS_KERNMOUNT	(1<<22) /* this is a kern_mount call */
#define MS_I_VERSION	(1<<23) /* Update inode I_version field */
#define MS_STRICTATIME	(1<<24) /* Always perform atime updates */
#define MS_NOSEC	(1<<28)
#define MS_BORN		(1<<29)
#define MS_ACTIVE	(1<<30)
#define MS_NOUSER	(1<<31)

/*
 * Superblock flags that can be altered by MS_REMOUNT
 */
#define MS_RMT_MASK	(MS_RDONLY|MS_SYNCHRONOUS|MS_MANDLOCK|MS_I_VERSION)

/*
 * Old magic mount flag and mask
 */
#define MS_MGC_VAL 0xC0ED0000
#define MS_MGC_MSK 0xffff0000

/* Inode flags - they have nothing to superblock flags now */

#define S_SYNC		1	/* Writes are synced at once */
#define S_NOATIME	2	/* Do not update access times */
#define S_APPEND	4	/* Append-only file */
#define S_IMMUTABLE	8	/* Immutable file */
#define S_DEAD		16	/* removed, but still open directory */
#define S_NOQUOTA	32	/* Inode is not counted to quota */
#define S_DIRSYNC	64	/* Directory modifications are synchronous */
#define S_NOCMTIME	128	/* Do not update file c/mtime */
#define S_SWAPFILE	256	/* Do not truncate: swapon got its bmaps */
#define S_PRIVATE	512	/* Inode is fs-internal */
#define S_IMA		1024	/* Inode has an associated IMA struct */
#define S_AUTOMOUNT	2048	/* Automount/referral quasi-directory */
#define S_NOSEC		4096	/* no suid or xattr security attributes */

/*
 * Note that nosuid etc flags are inode-specific: setting some file-system
 * flags just means all the inodes inherit those flags by default. It might be
 * possible to override it selectively if you really wanted to with some
 * ioctl() that is not currently implemented.
 *
 * Exception: MS_RDONLY is always applied to the entire file system.
 *
 * Unfortunately, it is possible to change a filesystems flags with it mounted
 * with files in use.  This means that all of the inodes will not have their
 * i_flags updated.  Hence, i_flags no longer inherit the superblock mount
 * flags, so these have to be checked separately. -- rmk@arm.uk.linux.org
 */
#define __IS_FLG(inode,flg) ((inode)->i_sb->s_flags & (flg))

#define IS_RDONLY(inode) ((inode)->i_sb->s_flags & MS_RDONLY)
#define IS_SYNC(inode)		(__IS_FLG(inode, MS_SYNCHRONOUS) || \
					((inode)->i_flags & S_SYNC))
#define IS_DIRSYNC(inode)	(__IS_FLG(inode, MS_SYNCHRONOUS|MS_DIRSYNC) || \
					((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
#define IS_MANDLOCK(inode)	__IS_FLG(inode, MS_MANDLOCK)
#define IS_NOATIME(inode)   __IS_FLG(inode, MS_RDONLY|MS_NOATIME)
#define IS_I_VERSION(inode)   __IS_FLG(inode, MS_I_VERSION)

#define IS_NOQUOTA(inode)	((inode)->i_flags & S_NOQUOTA)
#define IS_APPEND(inode)	((inode)->i_flags & S_APPEND)
#define IS_IMMUTABLE(inode)	((inode)->i_flags & S_IMMUTABLE)
#define IS_POSIXACL(inode)	__IS_FLG(inode, MS_POSIXACL)

#define IS_DEADDIR(inode)	((inode)->i_flags & S_DEAD)
#define IS_NOCMTIME(inode)	((inode)->i_flags & S_NOCMTIME)
#define IS_SWAPFILE(inode)	((inode)->i_flags & S_SWAPFILE)
#define IS_PRIVATE(inode)	((inode)->i_flags & S_PRIVATE)
#define IS_IMA(inode)		((inode)->i_flags & S_IMA)
#define IS_AUTOMOUNT(inode)	((inode)->i_flags & S_AUTOMOUNT)
#define IS_NOSEC(inode)		((inode)->i_flags & S_NOSEC)

/* the read-only stuff doesn't really belong here, but any other place is
   probably as bad and I don't want to create yet another include file. */

#define BLKROSET   _IO(0x12,93)	/* set device read-only (0 = read-write) */
#define BLKROGET   _IO(0x12,94)	/* get read-only status (0 = read_write) */
#define BLKRRPART  _IO(0x12,95)	/* re-read partition table */
#define BLKGETSIZE _IO(0x12,96)	/* return device size /512 (long *arg) */
#define BLKFLSBUF  _IO(0x12,97)	/* flush buffer cache */
#define BLKRASET   _IO(0x12,98)	/* set read ahead for block device */
#define BLKRAGET   _IO(0x12,99)	/* get current read ahead setting */
#define BLKFRASET  _IO(0x12,100)/* set filesystem (mm/filemap.c) read-ahead */
#define BLKFRAGET  _IO(0x12,101)/* get filesystem (mm/filemap.c) read-ahead */
#define BLKSECTSET _IO(0x12,102)/* set max sectors per request (ll_rw_blk.c) */
#define BLKSECTGET _IO(0x12,103)/* get max sectors per request (ll_rw_blk.c) */
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#if 0
#define BLKPG      _IO(0x12,105)/* See blkpg.h */

/* Some people are morons.  Do not use sizeof! */

#define BLKELVGET  _IOR(0x12,106,size_t)/* elevator get */
#define BLKELVSET  _IOW(0x12,107,size_t)/* elevator set */
/* This was here just to show that the number is taken -
   probably all these _IO(0x12,*) ioctls should be moved to blkpg.h. */
#endif
/* A jump here: 108-111 have been used for various private purposes. */
#define BLKBSZGET  _IOR(0x12,112,size_t)
#define BLKBSZSET  _IOW(0x12,113,size_t)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#define BLKTRACESETUP _IOWR(0x12,115,struct blk_user_trace_setup)
#define BLKTRACESTART _IO(0x12,116)
#define BLKTRACESTOP _IO(0x12,117)
#define BLKTRACETEARDOWN _IO(0x12,118)
#define BLKDISCARD _IO(0x12,119)
#define BLKIOMIN _IO(0x12,120)
#define BLKIOOPT _IO(0x12,121)
#define BLKALIGNOFF _IO(0x12,122)
#define BLKPBSZGET _IO(0x12,123)
#define BLKDISCARDZEROES _IO(0x12,124)
#define BLKSECDISCARD _IO(0x12,125)
#define BLKROTATIONAL _IO(0x12,126)
#define BLKSANITIZE _IO(0x12, 127)

#define BMAP_IOCTL 1		/* obsolete - kept for compatibility */
#define FIBMAP	   _IO(0x00,1)	/* bmap access */
#define FIGETBSZ   _IO(0x00,2)	/* get the block size used for bmap */
#define FIFREEZE	_IOWR('X', 119, int)	/* Freeze */
#define FITHAW		_IOWR('X', 120, int)	/* Thaw */
#define FITRIM		_IOWR('X', 121, struct fstrim_range)	/* Trim */

#define	FS_IOC_GETFLAGS			_IOR('f', 1, long)
#define	FS_IOC_SETFLAGS			_IOW('f', 2, long)
#define	FS_IOC_GETVERSION		_IOR('v', 1, long)
#define	FS_IOC_SETVERSION		_IOW('v', 2, long)
#define FS_IOC_FIEMAP			_IOWR('f', 11, struct fiemap)
#define FS_IOC32_GETFLAGS		_IOR('f', 1, int)
#define FS_IOC32_SETFLAGS		_IOW('f', 2, int)
#define FS_IOC32_GETVERSION		_IOR('v', 1, int)
#define FS_IOC32_SETVERSION		_IOW('v', 2, int)

/*
 * File system encryption support
 */
/* Policy provided via an ioctl on the topmost directory */
#define FS_KEY_DESCRIPTOR_SIZE 8

struct fscrypt_policy {
	__u8 version;
	__u8 contents_encryption_mode;
	__u8 filenames_encryption_mode;
	__u8 flags;
	__u8 master_key_descriptor[FS_KEY_DESCRIPTOR_SIZE];
} __packed;

#define FS_IOC_SET_ENCRYPTION_POLICY	_IOR('f', 19, struct fscrypt_policy)
#define FS_IOC_GET_ENCRYPTION_PWSALT	_IOW('f', 20, __u8[16])
#define FS_IOC_GET_ENCRYPTION_POLICY	_IOW('f', 21, struct fscrypt_policy)

/*
 * Inode flags (FS_IOC_GETFLAGS / FS_IOC_SETFLAGS)
 */
#define	FS_SECRM_FL			0x00000001 /* Secure deletion */
#define	FS_UNRM_FL			0x00000002 /* Undelete */
#define	FS_COMPR_FL			0x00000004 /* Compress file */
#define FS_SYNC_FL			0x00000008 /* Synchronous updates */
#define FS_IMMUTABLE_FL			0x00000010 /* Immutable file */
#define FS_APPEND_FL			0x00000020 /* writes to file may only append */
#define FS_NODUMP_FL			0x00000040 /* do not dump file */
#define FS_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define FS_DIRTY_FL			0x00000100
#define FS_COMPRBLK_FL			0x00000200 /* One or more compressed clusters */
#define FS_NOCOMP_FL			0x00000400 /* Don't compress */
#define FS_ECOMPR_FL			0x00000800 /* Compression error */
/* End compression flags --- maybe not all used */
#define FS_BTREE_FL			0x00001000 /* btree format dir */
#define FS_INDEX_FL			0x00001000 /* hash-indexed directory */
#define FS_IMAGIC_FL			0x00002000 /* AFS directory */
#define FS_JOURNAL_DATA_FL		0x00004000 /* Reserved for ext3 */
#define FS_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#define FS_DIRSYNC_FL			0x00010000 /* dirsync behaviour (directories only) */
#define FS_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#define FS_EXTENT_FL			0x00080000 /* Extents */
#define FS_DIRECTIO_FL			0x00100000 /* Use direct i/o */
#define FS_NOCOW_FL			0x00800000 /* Do not cow file */
#define FS_RESERVED_FL			0x80000000 /* reserved for ext2 lib */

#define FS_FL_USER_VISIBLE		0x0003DFFF /* User visible flags */
#define FS_FL_USER_MODIFIABLE		0x000380FF /* User modifiable flags */


#define SYNC_FILE_RANGE_WAIT_BEFORE	1
#define SYNC_FILE_RANGE_WRITE		2
#define SYNC_FILE_RANGE_WAIT_AFTER	4

#ifdef __KERNEL__

#include <linux/linkage.h>
#include <linux/wait.h>
#include <linux/kdev_t.h>
#include <linux/dcache.h>
#include <linux/path.h>
#include <linux/stat.h>
#include <linux/cache.h>
#include <linux/list.h>
#include <linux/radix-tree.h>
#include <linux/prio_tree.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/bug.h>
#include <linux/mutex.h>
#include <linux/capability.h>
#include <linux/semaphore.h>
#include <linux/fiemap.h>
#include <linux/rculist_bl.h>
#include <linux/atomic.h>
#include <linux/shrinker.h>
#include <linux/migrate_mode.h>

#include <asm/byteorder.h>

struct export_operations;
struct hd_geometry;
struct iovec;
struct nameidata;
struct kiocb;
struct kobject;
struct pipe_inode_info;
struct poll_table_struct;
struct kstatfs;
struct vm_area_struct;
struct vfsmount;
struct cred;
struct fscrypt_info;
struct fscrypt_operations;

extern void __init inode_init(void);
extern void __init inode_init_early(void);
extern void __init files_init(unsigned long);

extern struct files_stat_struct files_stat;
extern unsigned long get_max_files(void);
extern int sysctl_nr_open;
extern struct inodes_stat_t inodes_stat;
extern int leases_enable, lease_break_time;
extern int sysctl_protected_symlinks;
extern int sysctl_protected_hardlinks;

struct buffer_head;
typedef int (get_block_t)(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create);
typedef void (dio_iodone_t)(struct kiocb *iocb, loff_t offset,
			ssize_t bytes, void *private, int ret,
			bool is_async);
=======
/* File needs atomic accesses to f_pos */
#define FMODE_ATOMIC_POS	((__force fmode_t)0x8000)
/* Write access to underlying fs */
#define FMODE_WRITER		((__force fmode_t)0x10000)
/* Has read method(s) */
#define FMODE_CAN_READ          ((__force fmode_t)0x20000)
/* Has write method(s) */
#define FMODE_CAN_WRITE         ((__force fmode_t)0x40000)

#define FMODE_OPENED		((__force fmode_t)0x80000)
#define FMODE_CREATED		((__force fmode_t)0x100000)

/* File is stream-like */
#define FMODE_STREAM		((__force fmode_t)0x200000)

/* File supports DIRECT IO */
#define	FMODE_CAN_ODIRECT	((__force fmode_t)0x400000)

#define	FMODE_NOREUSE		((__force fmode_t)0x800000)

/* File supports non-exclusive O_DIRECT writes from multiple threads */
#define FMODE_DIO_PARALLEL_WRITE	((__force fmode_t)0x1000000)

/* File is embedded in backing_file object */
#define FMODE_BACKING		((__force fmode_t)0x2000000)

/* File was opened by fanotify and shouldn't generate fanotify events */
#define FMODE_NONOTIFY		((__force fmode_t)0x4000000)

/* File is capable of returning -EAGAIN if I/O will block */
#define FMODE_NOWAIT		((__force fmode_t)0x8000000)

/* File represents mount that needs unmounting */
#define FMODE_NEED_UNMOUNT	((__force fmode_t)0x10000000)

/* File does not contribute to nr_files count */
#define FMODE_NOACCOUNT		((__force fmode_t)0x20000000)

/* File supports async buffered reads */
#define FMODE_BUF_RASYNC	((__force fmode_t)0x40000000)

/* File supports async nowait buffered writes */
#define FMODE_BUF_WASYNC	((__force fmode_t)0x80000000)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Attribute flags.  These should be or-ed together to figure out what
 * has been changed!
 */
#define ATTR_MODE	(1 << 0)
#define ATTR_UID	(1 << 1)
#define ATTR_GID	(1 << 2)
#define ATTR_SIZE	(1 << 3)
#define ATTR_ATIME	(1 << 4)
#define ATTR_MTIME	(1 << 5)
#define ATTR_CTIME	(1 << 6)
#define ATTR_ATIME_SET	(1 << 7)
#define ATTR_MTIME_SET	(1 << 8)
#define ATTR_FORCE	(1 << 9) /* Not a change, but a change it */
<<<<<<< HEAD
#define ATTR_ATTR_FLAG	(1 << 10)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ATTR_KILL_SUID	(1 << 11)
#define ATTR_KILL_SGID	(1 << 12)
#define ATTR_FILE	(1 << 13)
#define ATTR_KILL_PRIV	(1 << 14)
#define ATTR_OPEN	(1 << 15) /* Truncating from open(O_TRUNC) */
#define ATTR_TIMES_SET	(1 << 16)
<<<<<<< HEAD
=======
#define ATTR_TOUCH	(1 << 17)

/*
 * Whiteout is represented by a char device.  The following constants define the
 * mode and device number to use.
 */
#define WHITEOUT_MODE 0
#define WHITEOUT_DEV 0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the Inode Attributes structure, used for notify_change().  It
 * uses the above definitions as flags, to know which values have changed.
 * Also, in this manner, a Filesystem can look at only the values it cares
 * about.  Basically, these are the attributes that the VFS layer can
 * request to change from the FS layer.
 *
 * Derek Atkins <warlord@MIT.EDU> 94-10-20
 */
struct iattr {
	unsigned int	ia_valid;
	umode_t		ia_mode;
<<<<<<< HEAD
	uid_t		ia_uid;
	gid_t		ia_gid;
	loff_t		ia_size;
	struct timespec	ia_atime;
	struct timespec	ia_mtime;
	struct timespec	ia_ctime;
=======
	/*
	 * The two anonymous unions wrap structures with the same member.
	 *
	 * Filesystems raising FS_ALLOW_IDMAP need to use ia_vfs{g,u}id which
	 * are a dedicated type requiring the filesystem to use the dedicated
	 * helpers. Other filesystem can continue to use ia_{g,u}id until they
	 * have been ported.
	 *
	 * They always contain the same value. In other words FS_ALLOW_IDMAP
	 * pass down the same value on idmapped mounts as they would on regular
	 * mounts.
	 */
	union {
		kuid_t		ia_uid;
		vfsuid_t	ia_vfsuid;
	};
	union {
		kgid_t		ia_gid;
		vfsgid_t	ia_vfsgid;
	};
	loff_t		ia_size;
	struct timespec64 ia_atime;
	struct timespec64 ia_mtime;
	struct timespec64 ia_ctime;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Not an attribute, but an auxiliary info for filesystems wanting to
	 * implement an ftruncate() like method.  NOTE: filesystem should
	 * check for (ia_valid & ATTR_FILE), and not for (ia_file != NULL).
	 */
	struct file	*ia_file;
};

/*
 * Includes for diskquotas.
 */
#include <linux/quota.h>

/*
 * Maximum number of layers of fs stack.  Needs to be limited to
 * prevent kernel stack overflow
 */
#define FILESYSTEM_MAX_STACK_DEPTH 2

/** 
 * enum positive_aop_returns - aop return codes with specific semantics
 *
 * @AOP_WRITEPAGE_ACTIVATE: Informs the caller that page writeback has
 * 			    completed, that the page is still locked, and
 * 			    should be considered active.  The VM uses this hint
 * 			    to return the page to the active list -- it won't
 * 			    be a candidate for writeback again in the near
 * 			    future.  Other callers must be careful to unlock
 * 			    the page if they get this return.  Returned by
 * 			    writepage(); 
 *
 * @AOP_TRUNCATED_PAGE: The AOP method that was handed a locked page has
 *  			unlocked it and the page might have been truncated.
 *  			The caller should back up to acquiring a new page and
 *  			trying again.  The aop will be taking reasonable
 *  			precautions not to livelock.  If the caller held a page
 *  			reference, it should drop it before retrying.  Returned
<<<<<<< HEAD
 *  			by readpage().
=======
 *  			by read_folio().
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * address_space_operation functions return these large constants to indicate
 * special semantics to the caller.  These are much larger than the bytes in a
 * page to allow for functions that return the number of bytes operated on in a
 * given page.
 */

enum positive_aop_returns {
	AOP_WRITEPAGE_ACTIVATE	= 0x80000,
	AOP_TRUNCATED_PAGE	= 0x80001,
};

<<<<<<< HEAD
#define AOP_FLAG_UNINTERRUPTIBLE	0x0001 /* will not do a short write */
#define AOP_FLAG_CONT_EXPAND		0x0002 /* called from cont_expand */
#define AOP_FLAG_NOFS			0x0004 /* used by filesystem to direct
						* helper code (eg buffer layer)
						* to clear GFP_FS from alloc */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * oh the beauties of C type declarations.
 */
struct page;
struct address_space;
struct writeback_control;
<<<<<<< HEAD

struct iov_iter {
	const struct iovec *iov;
	unsigned long nr_segs;
	size_t iov_offset;
	size_t count;
};

size_t iov_iter_copy_from_user_atomic(struct page *page,
		struct iov_iter *i, unsigned long offset, size_t bytes);
size_t iov_iter_copy_from_user(struct page *page,
		struct iov_iter *i, unsigned long offset, size_t bytes);
void iov_iter_advance(struct iov_iter *i, size_t bytes);
int iov_iter_fault_in_readable(struct iov_iter *i, size_t bytes);
size_t iov_iter_single_seg_count(struct iov_iter *i);

static inline void iov_iter_init(struct iov_iter *i,
			const struct iovec *iov, unsigned long nr_segs,
			size_t count, size_t written)
{
	i->iov = iov;
	i->nr_segs = nr_segs;
	i->iov_offset = 0;
	i->count = count + written;

	iov_iter_advance(i, written);
}

static inline size_t iov_iter_count(struct iov_iter *i)
{
	return i->count;
}

/*
 * "descriptor" for what we're up to with a read.
 * This allows us to use the same read code yet
 * have multiple different users of the data that
 * we read from a file.
 *
 * The simplest case just copies the data to user
 * mode.
 */
typedef struct {
	size_t written;
	size_t count;
	union {
		char __user *buf;
		void *data;
	} arg;
	int error;
} read_descriptor_t;

typedef int (*read_actor_t)(read_descriptor_t *, struct page *,
		unsigned long, unsigned long);

struct address_space_operations {
	int (*writepage)(struct page *page, struct writeback_control *wbc);
	int (*readpage)(struct file *, struct page *);
=======
struct readahead_control;

/* Match RWF_* bits to IOCB bits */
#define IOCB_HIPRI		(__force int) RWF_HIPRI
#define IOCB_DSYNC		(__force int) RWF_DSYNC
#define IOCB_SYNC		(__force int) RWF_SYNC
#define IOCB_NOWAIT		(__force int) RWF_NOWAIT
#define IOCB_APPEND		(__force int) RWF_APPEND

/* non-RWF related bits - start at 16 */
#define IOCB_EVENTFD		(1 << 16)
#define IOCB_DIRECT		(1 << 17)
#define IOCB_WRITE		(1 << 18)
/* iocb->ki_waitq is valid */
#define IOCB_WAITQ		(1 << 19)
#define IOCB_NOIO		(1 << 20)
/* can use bio alloc cache */
#define IOCB_ALLOC_CACHE	(1 << 21)
/*
 * IOCB_DIO_CALLER_COMP can be set by the iocb owner, to indicate that the
 * iocb completion can be passed back to the owner for execution from a safe
 * context rather than needing to be punted through a workqueue. If this
 * flag is set, the bio completion handling may set iocb->dio_complete to a
 * handler function and iocb->private to context information for that handler.
 * The issuer should call the handler with that context information from task
 * context to complete the processing of the iocb. Note that while this
 * provides a task context for the dio_complete() callback, it should only be
 * used on the completion side for non-IO generating completions. It's fine to
 * call blocking functions from this callback, but they should not wait for
 * unrelated IO (like cache flushing, new IO generation, etc).
 */
#define IOCB_DIO_CALLER_COMP	(1 << 22)
/* kiocb is a read or write operation submitted by fs/aio.c. */
#define IOCB_AIO_RW		(1 << 23)

/* for use in trace events */
#define TRACE_IOCB_STRINGS \
	{ IOCB_HIPRI,		"HIPRI" }, \
	{ IOCB_DSYNC,		"DSYNC" }, \
	{ IOCB_SYNC,		"SYNC" }, \
	{ IOCB_NOWAIT,		"NOWAIT" }, \
	{ IOCB_APPEND,		"APPEND" }, \
	{ IOCB_EVENTFD,		"EVENTFD"}, \
	{ IOCB_DIRECT,		"DIRECT" }, \
	{ IOCB_WRITE,		"WRITE" }, \
	{ IOCB_WAITQ,		"WAITQ" }, \
	{ IOCB_NOIO,		"NOIO" }, \
	{ IOCB_ALLOC_CACHE,	"ALLOC_CACHE" }, \
	{ IOCB_DIO_CALLER_COMP,	"CALLER_COMP" }

struct kiocb {
	struct file		*ki_filp;
	loff_t			ki_pos;
	void (*ki_complete)(struct kiocb *iocb, long ret);
	void			*private;
	int			ki_flags;
	u16			ki_ioprio; /* See linux/ioprio.h */
	union {
		/*
		 * Only used for async buffered reads, where it denotes the
		 * page waitqueue associated with completing the read. Valid
		 * IFF IOCB_WAITQ is set.
		 */
		struct wait_page_queue	*ki_waitq;
		/*
		 * Can be used for O_DIRECT IO, where the completion handling
		 * is punted back to the issuer of the IO. May only be set
		 * if IOCB_DIO_CALLER_COMP is set by the issuer, and the issuer
		 * must then check for presence of this handler when ki_complete
		 * is invoked. The data passed in to this handler must be
		 * assigned to ->private when dio_complete is assigned.
		 */
		ssize_t (*dio_complete)(void *data);
	};
};

static inline bool is_sync_kiocb(struct kiocb *kiocb)
{
	return kiocb->ki_complete == NULL;
}

struct address_space_operations {
	int (*writepage)(struct page *page, struct writeback_control *wbc);
	int (*read_folio)(struct file *, struct folio *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Write back some dirty pages from this mapping. */
	int (*writepages)(struct address_space *, struct writeback_control *);

<<<<<<< HEAD
	/* Set a page dirty.  Return true if this dirtied it */
	int (*set_page_dirty)(struct page *page);

	int (*readpages)(struct file *filp, struct address_space *mapping,
			struct list_head *pages, unsigned nr_pages);

	int (*write_begin)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
=======
	/* Mark a folio dirty.  Return true if this dirtied it */
	bool (*dirty_folio)(struct address_space *, struct folio *);

	void (*readahead)(struct readahead_control *);

	int (*write_begin)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct page **pagep, void **fsdata);
	int (*write_end)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata);

	/* Unfortunately this kludge is needed for FIBMAP. Don't use it */
	sector_t (*bmap)(struct address_space *, sector_t);
<<<<<<< HEAD
	void (*invalidatepage) (struct page *, unsigned long);
	int (*releasepage) (struct page *, gfp_t);
	void (*freepage)(struct page *);
	ssize_t (*direct_IO)(int, struct kiocb *, const struct iovec *iov,
			loff_t offset, unsigned long nr_segs);
	int (*get_xip_mem)(struct address_space *, pgoff_t, int,
						void **, unsigned long *);
	/*
	 * migrate the contents of a page to the specified target. If sync
	 * is false, it must not block.
	 */
	int (*migratepage) (struct address_space *,
			struct page *, struct page *, enum migrate_mode);
	int (*launder_page) (struct page *);
	int (*is_partially_uptodate) (struct page *, read_descriptor_t *,
					unsigned long);
	int (*error_remove_page)(struct address_space *, struct page *);
=======
	void (*invalidate_folio) (struct folio *, size_t offset, size_t len);
	bool (*release_folio)(struct folio *, gfp_t);
	void (*free_folio)(struct folio *folio);
	ssize_t (*direct_IO)(struct kiocb *, struct iov_iter *iter);
	/*
	 * migrate the contents of a folio to the specified target. If
	 * migrate_mode is MIGRATE_ASYNC, it must not block.
	 */
	int (*migrate_folio)(struct address_space *, struct folio *dst,
			struct folio *src, enum migrate_mode);
	int (*launder_folio)(struct folio *);
	bool (*is_partially_uptodate) (struct folio *, size_t from,
			size_t count);
	void (*is_dirty_writeback) (struct folio *, bool *dirty, bool *wb);
	int (*error_remove_folio)(struct address_space *, struct folio *);

	/* swapfile support */
	int (*swap_activate)(struct swap_info_struct *sis, struct file *file,
				sector_t *span);
	void (*swap_deactivate)(struct file *file);
	int (*swap_rw)(struct kiocb *iocb, struct iov_iter *iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

extern const struct address_space_operations empty_aops;

<<<<<<< HEAD
/*
 * pagecache_write_begin/pagecache_write_end must be used by general code
 * to write into the pagecache.
 */
int pagecache_write_begin(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
				struct page **pagep, void **fsdata);

int pagecache_write_end(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata);

struct backing_dev_info;
struct address_space {
	struct inode		*host;		/* owner: inode, block_device */
	struct radix_tree_root	page_tree;	/* radix tree of all pages */
	spinlock_t		tree_lock;	/* and lock protecting it */
	unsigned int		i_mmap_writable;/* count VM_SHARED mappings */
	struct prio_tree_root	i_mmap;		/* tree of private and shared mappings */
	struct list_head	i_mmap_nonlinear;/*list VM_NONLINEAR mappings */
	struct mutex		i_mmap_mutex;	/* protect tree, count, list */
	/* Protected by tree_lock together with the radix tree */
	unsigned long		nrpages;	/* number of total pages */
	pgoff_t			writeback_index;/* writeback starts here */
	const struct address_space_operations *a_ops;	/* methods */
	unsigned long		flags;		/* error bits/gfp mask */
	struct backing_dev_info *backing_dev_info; /* device readahead, etc */
	spinlock_t		private_lock;	/* for use by the address_space */
	struct list_head	private_list;	/* ditto */
	struct address_space	*assoc_mapping;	/* ditto */
} __attribute__((aligned(sizeof(long))));
=======
/**
 * struct address_space - Contents of a cacheable, mappable object.
 * @host: Owner, either the inode or the block_device.
 * @i_pages: Cached pages.
 * @invalidate_lock: Guards coherency between page cache contents and
 *   file offset->disk block mappings in the filesystem during invalidates.
 *   It is also used to block modification of page cache contents through
 *   memory mappings.
 * @gfp_mask: Memory allocation flags to use for allocating pages.
 * @i_mmap_writable: Number of VM_SHARED, VM_MAYWRITE mappings.
 * @nr_thps: Number of THPs in the pagecache (non-shmem only).
 * @i_mmap: Tree of private and shared mappings.
 * @i_mmap_rwsem: Protects @i_mmap and @i_mmap_writable.
 * @nrpages: Number of page entries, protected by the i_pages lock.
 * @writeback_index: Writeback starts here.
 * @a_ops: Methods.
 * @flags: Error bits and flags (AS_*).
 * @wb_err: The most recent error which has occurred.
 * @i_private_lock: For use by the owner of the address_space.
 * @i_private_list: For use by the owner of the address_space.
 * @i_private_data: For use by the owner of the address_space.
 */
struct address_space {
	struct inode		*host;
	struct xarray		i_pages;
	struct rw_semaphore	invalidate_lock;
	gfp_t			gfp_mask;
	atomic_t		i_mmap_writable;
#ifdef CONFIG_READ_ONLY_THP_FOR_FS
	/* number of thp, only for non-shmem files */
	atomic_t		nr_thps;
#endif
	struct rb_root_cached	i_mmap;
	unsigned long		nrpages;
	pgoff_t			writeback_index;
	const struct address_space_operations *a_ops;
	unsigned long		flags;
	errseq_t		wb_err;
	spinlock_t		i_private_lock;
	struct list_head	i_private_list;
	struct rw_semaphore	i_mmap_rwsem;
	void *			i_private_data;
} __attribute__((aligned(sizeof(long)))) __randomize_layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * On most architectures that alignment is already the case; but
	 * must be enforced here for CRIS, to let the least significant bit
	 * of struct page's "mapping" pointer be used for PAGE_MAPPING_ANON.
	 */
<<<<<<< HEAD
struct request_queue;

struct block_device {
	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
	int			bd_openers;
	struct inode *		bd_inode;	/* will die */
	struct super_block *	bd_super;
	struct mutex		bd_mutex;	/* open/close mutex */
	struct list_head	bd_inodes;
	void *			bd_claiming;
	void *			bd_holder;
	int			bd_holders;
	bool			bd_write_holder;
#ifdef CONFIG_SYSFS
	struct list_head	bd_holder_disks;
#endif
	struct block_device *	bd_contains;
	unsigned		bd_block_size;
	struct hd_struct *	bd_part;
	/* number of times partitions within this device have been opened. */
	unsigned		bd_part_count;
	int			bd_invalidated;
	struct gendisk *	bd_disk;
	struct request_queue *  bd_queue;
	struct list_head	bd_list;
	/*
	 * Private data.  You must have bd_claim'ed the block_device
	 * to use this.  NOTE:  bd_claim allows an owner to claim
	 * the same device multiple times, the owner must take special
	 * care to not mess up bd_private for that case.
	 */
	unsigned long		bd_private;

	/* The counter of freeze processes */
	int			bd_fsfreeze_count;
	/* Mutex for freeze */
	struct mutex		bd_fsfreeze_mutex;
};

/*
 * Radix-tree tags, for tagging dirty and writeback pages within the pagecache
 * radix trees
 */
#define PAGECACHE_TAG_DIRTY	0
#define PAGECACHE_TAG_WRITEBACK	1
#define PAGECACHE_TAG_TOWRITE	2

int mapping_tagged(struct address_space *mapping, int tag);
=======

/* XArray tags, for tagging dirty and writeback pages in the pagecache. */
#define PAGECACHE_TAG_DIRTY	XA_MARK_0
#define PAGECACHE_TAG_WRITEBACK	XA_MARK_1
#define PAGECACHE_TAG_TOWRITE	XA_MARK_2

/*
 * Returns true if any of the pages in the mapping are marked with the tag.
 */
static inline bool mapping_tagged(struct address_space *mapping, xa_mark_t tag)
{
	return xa_marked(&mapping->i_pages, tag);
}

static inline void i_mmap_lock_write(struct address_space *mapping)
{
	down_write(&mapping->i_mmap_rwsem);
}

static inline int i_mmap_trylock_write(struct address_space *mapping)
{
	return down_write_trylock(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_unlock_write(struct address_space *mapping)
{
	up_write(&mapping->i_mmap_rwsem);
}

static inline int i_mmap_trylock_read(struct address_space *mapping)
{
	return down_read_trylock(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_lock_read(struct address_space *mapping)
{
	down_read(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_unlock_read(struct address_space *mapping)
{
	up_read(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_assert_locked(struct address_space *mapping)
{
	lockdep_assert_held(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_assert_write_locked(struct address_space *mapping)
{
	lockdep_assert_held_write(&mapping->i_mmap_rwsem);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Might pages of this file be mapped into userspace?
 */
static inline int mapping_mapped(struct address_space *mapping)
{
<<<<<<< HEAD
	return	!prio_tree_empty(&mapping->i_mmap) ||
		!list_empty(&mapping->i_mmap_nonlinear);
=======
	return	!RB_EMPTY_ROOT(&mapping->i_mmap.rb_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Might pages of this file have been modified in userspace?
<<<<<<< HEAD
 * Note that i_mmap_writable counts all VM_SHARED vmas: do_mmap_pgoff
 * marks vma as VM_SHARED if it is shared, and the file was opened for
 * writing i.e. vma may be mprotected writable even if now readonly.
 */
static inline int mapping_writably_mapped(struct address_space *mapping)
{
	return mapping->i_mmap_writable != 0;
=======
 * Note that i_mmap_writable counts all VM_SHARED, VM_MAYWRITE vmas: do_mmap
 * marks vma as VM_SHARED if it is shared, and the file was opened for
 * writing i.e. vma may be mprotected writable even if now readonly.
 *
 * If i_mmap_writable is negative, no new writable mappings are allowed. You
 * can only deny writable mappings, if none exists right now.
 */
static inline int mapping_writably_mapped(struct address_space *mapping)
{
	return atomic_read(&mapping->i_mmap_writable) > 0;
}

static inline int mapping_map_writable(struct address_space *mapping)
{
	return atomic_inc_unless_negative(&mapping->i_mmap_writable) ?
		0 : -EPERM;
}

static inline void mapping_unmap_writable(struct address_space *mapping)
{
	atomic_dec(&mapping->i_mmap_writable);
}

static inline int mapping_deny_writable(struct address_space *mapping)
{
	return atomic_dec_unless_positive(&mapping->i_mmap_writable) ?
		0 : -EBUSY;
}

static inline void mapping_allow_writable(struct address_space *mapping)
{
	atomic_inc(&mapping->i_mmap_writable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Use sequence counter to get consistent i_size on 32-bit processors.
 */
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
#include <linux/seqlock.h>
#define __NEED_I_SIZE_ORDERED
#define i_size_ordered_init(inode) seqcount_init(&inode->i_size_seqcount)
#else
#define i_size_ordered_init(inode) do { } while (0)
#endif

struct posix_acl;
#define ACL_NOT_CACHED ((void *)(-1))
<<<<<<< HEAD
=======
/*
 * ACL_DONT_CACHE is for stacked filesystems, that rely on underlying fs to
 * cache the ACL.  This also means that ->get_inode_acl() can be called in RCU
 * mode with the LOOKUP_RCU flag.
 */
#define ACL_DONT_CACHE ((void *)(-3))

static inline struct posix_acl *
uncached_acl_sentinel(struct task_struct *task)
{
	return (void *)task + 1;
}

static inline bool
is_uncached_acl(struct posix_acl *acl)
{
	return (long)acl & 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define IOP_FASTPERM	0x0001
#define IOP_LOOKUP	0x0002
#define IOP_NOFOLLOW	0x0004
<<<<<<< HEAD
=======
#define IOP_XATTR	0x0008
#define IOP_DEFAULT_READLINK	0x0010

struct fsnotify_mark_connector;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Keep mostly read-only and often accessed (especially for
 * the RCU path lookup and 'stat' data) fields at the beginning
 * of the 'struct inode'
 */
struct inode {
	umode_t			i_mode;
	unsigned short		i_opflags;
<<<<<<< HEAD
	uid_t			i_uid;
	gid_t			i_gid;
=======
	kuid_t			i_uid;
	kgid_t			i_gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int		i_flags;

#ifdef CONFIG_FS_POSIX_ACL
	struct posix_acl	*i_acl;
	struct posix_acl	*i_default_acl;
#endif

	const struct inode_operations	*i_op;
	struct super_block	*i_sb;
	struct address_space	*i_mapping;

#ifdef CONFIG_SECURITY
	void			*i_security;
#endif

	/* Stat data, not accessed from path walking */
	unsigned long		i_ino;
	/*
	 * Filesystems may only read i_nlink directly.  They shall use the
	 * following functions for modification:
	 *
	 *    (set|clear|inc|drop)_nlink
	 *    inode_(inc|dec)_link_count
	 */
	union {
		const unsigned int i_nlink;
		unsigned int __i_nlink;
	};
	dev_t			i_rdev;
<<<<<<< HEAD
	struct timespec		i_atime;
	struct timespec		i_mtime;
	struct timespec		i_ctime;
	spinlock_t		i_lock;	/* i_blocks, i_bytes, maybe i_size */
	unsigned short          i_bytes;
	blkcnt_t		i_blocks;
	loff_t			i_size;
=======
	loff_t			i_size;
	struct timespec64	__i_atime;
	struct timespec64	__i_mtime;
	struct timespec64	__i_ctime; /* use inode_*_ctime accessors! */
	spinlock_t		i_lock;	/* i_blocks, i_bytes, maybe i_size */
	unsigned short          i_bytes;
	u8			i_blkbits;
	enum rw_hint		i_write_hint;
	blkcnt_t		i_blocks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef __NEED_I_SIZE_ORDERED
	seqcount_t		i_size_seqcount;
#endif

	/* Misc */
	unsigned long		i_state;
<<<<<<< HEAD
	struct mutex		i_mutex;

	unsigned long		dirtied_when;	/* jiffies of first dirtying */

	struct hlist_node	i_hash;
	struct list_head	i_wb_list;	/* backing dev IO list */
	struct list_head	i_lru;		/* inode LRU list */
	struct list_head	i_sb_list;
	union {
		struct list_head	i_dentry;
		struct rcu_head		i_rcu;
	};
	atomic_t		i_count;
	unsigned int		i_blkbits;
	u64			i_version;
	atomic_t		i_dio_count;
	atomic_t		i_writecount;
	const struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
	struct file_lock	*i_flock;
	struct address_space	i_data;
#ifdef CONFIG_QUOTA
	struct dquot		*i_dquot[MAXQUOTAS];
#endif
	struct list_head	i_devices;
	union {
		struct pipe_inode_info	*i_pipe;
		struct block_device	*i_bdev;
		struct cdev		*i_cdev;
=======
	struct rw_semaphore	i_rwsem;

	unsigned long		dirtied_when;	/* jiffies of first dirtying */
	unsigned long		dirtied_time_when;

	struct hlist_node	i_hash;
	struct list_head	i_io_list;	/* backing dev IO list */
#ifdef CONFIG_CGROUP_WRITEBACK
	struct bdi_writeback	*i_wb;		/* the associated cgroup wb */

	/* foreign inode detection, see wbc_detach_inode() */
	int			i_wb_frn_winner;
	u16			i_wb_frn_avg_time;
	u16			i_wb_frn_history;
#endif
	struct list_head	i_lru;		/* inode LRU list */
	struct list_head	i_sb_list;
	struct list_head	i_wb_list;	/* backing dev writeback list */
	union {
		struct hlist_head	i_dentry;
		struct rcu_head		i_rcu;
	};
	atomic64_t		i_version;
	atomic64_t		i_sequence; /* see futex */
	atomic_t		i_count;
	atomic_t		i_dio_count;
	atomic_t		i_writecount;
#if defined(CONFIG_IMA) || defined(CONFIG_FILE_LOCKING)
	atomic_t		i_readcount; /* struct files open RO */
#endif
	union {
		const struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
		void (*free_inode)(struct inode *);
	};
	struct file_lock_context	*i_flctx;
	struct address_space	i_data;
	struct list_head	i_devices;
	union {
		struct pipe_inode_info	*i_pipe;
		struct cdev		*i_cdev;
		char			*i_link;
		unsigned		i_dir_seq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};

	__u32			i_generation;

#ifdef CONFIG_FSNOTIFY
	__u32			i_fsnotify_mask; /* all events this inode cares about */
<<<<<<< HEAD
	struct hlist_head	i_fsnotify_marks;
#endif

#ifdef CONFIG_IMA
	atomic_t		i_readcount; /* struct files open RO */
#endif

#if IS_ENABLED(CONFIG_FS_ENCRYPTION)
	struct fscrypt_info	*i_crypt_info;
#endif

	void			*i_private; /* fs or device private pointer */
};
=======
	struct fsnotify_mark_connector __rcu	*i_fsnotify_marks;
#endif

#ifdef CONFIG_FS_ENCRYPTION
	struct fscrypt_inode_info	*i_crypt_info;
#endif

#ifdef CONFIG_FS_VERITY
	struct fsverity_info	*i_verity_info;
#endif

	void			*i_private; /* fs or device private pointer */
} __randomize_layout;

struct timespec64 timestamp_truncate(struct timespec64 t, struct inode *inode);

static inline unsigned int i_blocksize(const struct inode *node)
{
	return (1 << node->i_blkbits);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int inode_unhashed(struct inode *inode)
{
	return hlist_unhashed(&inode->i_hash);
}

/*
<<<<<<< HEAD
=======
 * __mark_inode_dirty expects inodes to be hashed.  Since we don't
 * want special inodes in the fileset inode space, we make them
 * appear hashed, but do not put on any lists.  hlist_del()
 * will work fine and require no locking.
 */
static inline void inode_fake_hash(struct inode *inode)
{
	hlist_add_fake(&inode->i_hash);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * inode->i_mutex nesting subclasses for the lock validator:
 *
 * 0: the object of the current VFS operation
 * 1: parent
 * 2: child/target
<<<<<<< HEAD
 * 3: quota file
 *
 * The locking order between these classes is
 * parent -> child -> normal -> xattr -> quota
=======
 * 3: xattr
 * 4: second non-directory
 * 5: second parent (when locking independent directories in rename)
 *
 * I_MUTEX_NONDIR2 is for certain operations (such as rename) which lock two
 * non-directories at once.
 *
 * The locking order between these classes is
 * parent[2] -> child -> grandchild -> normal -> xattr -> second non-directory
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
enum inode_i_mutex_lock_class
{
	I_MUTEX_NORMAL,
	I_MUTEX_PARENT,
	I_MUTEX_CHILD,
	I_MUTEX_XATTR,
<<<<<<< HEAD
	I_MUTEX_QUOTA
};

=======
	I_MUTEX_NONDIR2,
	I_MUTEX_PARENT2,
};

static inline void inode_lock(struct inode *inode)
{
	down_write(&inode->i_rwsem);
}

static inline void inode_unlock(struct inode *inode)
{
	up_write(&inode->i_rwsem);
}

static inline void inode_lock_shared(struct inode *inode)
{
	down_read(&inode->i_rwsem);
}

static inline void inode_unlock_shared(struct inode *inode)
{
	up_read(&inode->i_rwsem);
}

static inline int inode_trylock(struct inode *inode)
{
	return down_write_trylock(&inode->i_rwsem);
}

static inline int inode_trylock_shared(struct inode *inode)
{
	return down_read_trylock(&inode->i_rwsem);
}

static inline int inode_is_locked(struct inode *inode)
{
	return rwsem_is_locked(&inode->i_rwsem);
}

static inline void inode_lock_nested(struct inode *inode, unsigned subclass)
{
	down_write_nested(&inode->i_rwsem, subclass);
}

static inline void inode_lock_shared_nested(struct inode *inode, unsigned subclass)
{
	down_read_nested(&inode->i_rwsem, subclass);
}

static inline void filemap_invalidate_lock(struct address_space *mapping)
{
	down_write(&mapping->invalidate_lock);
}

static inline void filemap_invalidate_unlock(struct address_space *mapping)
{
	up_write(&mapping->invalidate_lock);
}

static inline void filemap_invalidate_lock_shared(struct address_space *mapping)
{
	down_read(&mapping->invalidate_lock);
}

static inline int filemap_invalidate_trylock_shared(
					struct address_space *mapping)
{
	return down_read_trylock(&mapping->invalidate_lock);
}

static inline void filemap_invalidate_unlock_shared(
					struct address_space *mapping)
{
	up_read(&mapping->invalidate_lock);
}

void lock_two_nondirectories(struct inode *, struct inode*);
void unlock_two_nondirectories(struct inode *, struct inode*);

void filemap_invalidate_lock_two(struct address_space *mapping1,
				 struct address_space *mapping2);
void filemap_invalidate_unlock_two(struct address_space *mapping1,
				   struct address_space *mapping2);


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * NOTE: in a 32bit arch with a preemptable kernel and
 * an UP compile the i_size_read/write must be atomic
 * with respect to the local cpu (unlike with preempt disabled),
 * but they don't need to be atomic with respect to other cpus like in
 * true SMP (so they need either to either locally disable irq around
 * the read or for example on x86 they can be still implemented as a
 * cmpxchg8b without the need of the lock prefix). For SMP compiles
 * and 64bit archs it makes no difference if preempt is enabled or not.
 */
static inline loff_t i_size_read(const struct inode *inode)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	loff_t i_size;
	unsigned int seq;

	do {
		seq = read_seqcount_begin(&inode->i_size_seqcount);
		i_size = inode->i_size;
	} while (read_seqcount_retry(&inode->i_size_seqcount, seq));
	return i_size;
<<<<<<< HEAD
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
=======
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPTION)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	loff_t i_size;

	preempt_disable();
	i_size = inode->i_size;
	preempt_enable();
	return i_size;
#else
<<<<<<< HEAD
	return inode->i_size;
=======
	/* Pairs with smp_store_release() in i_size_write() */
	return smp_load_acquire(&inode->i_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

/*
 * NOTE: unlike i_size_read(), i_size_write() does need locking around it
 * (normally i_mutex), otherwise on 32bit/SMP an update of i_size_seqcount
 * can be lost, resulting in subsequent i_size_read() calls spinning forever.
 */
static inline void i_size_write(struct inode *inode, loff_t i_size)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	preempt_disable();
	write_seqcount_begin(&inode->i_size_seqcount);
	inode->i_size = i_size;
	write_seqcount_end(&inode->i_size_seqcount);
	preempt_enable();
<<<<<<< HEAD
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
=======
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPTION)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	preempt_disable();
	inode->i_size = i_size;
	preempt_enable();
#else
<<<<<<< HEAD
	inode->i_size = i_size;
=======
	/*
	 * Pairs with smp_load_acquire() in i_size_read() to ensure
	 * changes related to inode size (such as page contents) are
	 * visible before we see the changed inode size.
	 */
	smp_store_release(&inode->i_size, i_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static inline unsigned iminor(const struct inode *inode)
{
	return MINOR(inode->i_rdev);
}

static inline unsigned imajor(const struct inode *inode)
{
	return MAJOR(inode->i_rdev);
}

<<<<<<< HEAD
extern struct block_device *I_BDEV(struct inode *inode);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct fown_struct {
	rwlock_t lock;          /* protects pid, uid, euid fields */
	struct pid *pid;	/* pid or -pgrp where SIGIO should be sent */
	enum pid_type pid_type;	/* Kind of process group SIGIO should be sent to */
<<<<<<< HEAD
	uid_t uid, euid;	/* uid/euid of process setting the owner */
	int signum;		/* posix.1b rt signal to be delivered on IO */
};

/*
 * Track a single file's readahead state
 */
struct file_ra_state {
	pgoff_t start;			/* where readahead started */
	unsigned int size;		/* # of readahead pages */
	unsigned int async_size;	/* do asynchronous readahead when
					   there are only # of pages ahead */

	unsigned int ra_pages;		/* Maximum readahead window */
	unsigned int mmap_miss;		/* Cache miss stat for mmap accesses */
	loff_t prev_pos;		/* Cache last read() position */
=======
	kuid_t uid, euid;	/* uid/euid of process setting the owner */
	int signum;		/* posix.1b rt signal to be delivered on IO */
};

/**
 * struct file_ra_state - Track a file's readahead state.
 * @start: Where the most recent readahead started.
 * @size: Number of pages read in the most recent readahead.
 * @async_size: Numer of pages that were/are not needed immediately
 *      and so were/are genuinely "ahead".  Start next readahead when
 *      the first of these pages is accessed.
 * @ra_pages: Maximum size of a readahead request, copied from the bdi.
 * @mmap_miss: How many mmap accesses missed in the page cache.
 * @prev_pos: The last byte in the most recent read request.
 *
 * When this structure is passed to ->readahead(), the "most recent"
 * readahead means the current readahead.
 */
struct file_ra_state {
	pgoff_t start;
	unsigned int size;
	unsigned int async_size;
	unsigned int ra_pages;
	unsigned int mmap_miss;
	loff_t prev_pos;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Check if @index falls in the readahead windows.
 */
static inline int ra_has_index(struct file_ra_state *ra, pgoff_t index)
{
	return (index >= ra->start &&
		index <  ra->start + ra->size);
}

<<<<<<< HEAD
#define FILE_MNT_WRITE_TAKEN	1
#define FILE_MNT_WRITE_RELEASED	2

struct file {
	union {
		struct rcu_head 	fu_rcuhead;
	} f_u;
	struct path		f_path;
#define f_dentry	f_path.dentry
#define f_vfsmnt	f_path.mnt
	const struct file_operations	*f_op;

	/*
	 * Protects f_ep_links, f_flags, f_pos vs i_size in lseek SEEK_CUR.
	 * Must not be taken from IRQ context.
	 */
	spinlock_t		f_lock;
	atomic_long_t		f_count;
	unsigned int 		f_flags;
	fmode_t			f_mode;
	loff_t			f_pos;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;
=======
/*
 * f_{lock,count,pos_lock} members can be highly contended and share
 * the same cacheline. f_{lock,mode} are very frequently used together
 * and so share the same cacheline as well. The read-mostly
 * f_{path,inode,op} are kept on a separate cacheline.
 */
struct file {
	union {
		/* fput() uses task work when closing and freeing file (default). */
		struct callback_head 	f_task_work;
		/* fput() must use workqueue (most kernel threads). */
		struct llist_node	f_llist;
		unsigned int 		f_iocb_flags;
	};

	/*
	 * Protects f_ep, f_flags.
	 * Must not be taken from IRQ context.
	 */
	spinlock_t		f_lock;
	fmode_t			f_mode;
	atomic_long_t		f_count;
	struct mutex		f_pos_lock;
	loff_t			f_pos;
	unsigned int		f_flags;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;
	struct path		f_path;
	struct inode		*f_inode;	/* cached value */
	const struct file_operations	*f_op;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	u64			f_version;
#ifdef CONFIG_SECURITY
	void			*f_security;
#endif
	/* needed for tty driver, and maybe others */
	void			*private_data;

#ifdef CONFIG_EPOLL
	/* Used by fs/eventpoll.c to link all the hooks to this file */
<<<<<<< HEAD
	struct list_head	f_ep_links;
	struct list_head	f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
	struct address_space	*f_mapping;
#ifdef CONFIG_DEBUG_WRITECOUNT
	unsigned long f_mnt_write_state;
#endif
};
=======
	struct hlist_head	*f_ep;
#endif /* #ifdef CONFIG_EPOLL */
	struct address_space	*f_mapping;
	errseq_t		f_wb_err;
	errseq_t		f_sb_err; /* for syncfs */
} __randomize_layout
  __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct file_handle {
	__u32 handle_bytes;
	int handle_type;
	/* file identifier */
<<<<<<< HEAD
	unsigned char f_handle[0];
};

#define get_file(x)	atomic_long_inc(&(x)->f_count)
#define fput_atomic(x)	atomic_long_add_unless(&(x)->f_count, -1, 1)
#define file_count(x)	atomic_long_read(&(x)->f_count)

#ifdef CONFIG_DEBUG_WRITECOUNT
static inline void file_take_write(struct file *f)
{
	WARN_ON(f->f_mnt_write_state != 0);
	f->f_mnt_write_state = FILE_MNT_WRITE_TAKEN;
}
static inline void file_release_write(struct file *f)
{
	f->f_mnt_write_state |= FILE_MNT_WRITE_RELEASED;
}
static inline void file_reset_write(struct file *f)
{
	f->f_mnt_write_state = 0;
}
static inline void file_check_state(struct file *f)
{
	/*
	 * At this point, either both or neither of these bits
	 * should be set.
	 */
	WARN_ON(f->f_mnt_write_state == FILE_MNT_WRITE_TAKEN);
	WARN_ON(f->f_mnt_write_state == FILE_MNT_WRITE_RELEASED);
}
static inline int file_check_writeable(struct file *f)
{
	if (f->f_mnt_write_state == FILE_MNT_WRITE_TAKEN)
		return 0;
	printk(KERN_WARNING "writeable file with no "
			    "mnt_want_write()\n");
	WARN_ON(1);
	return -EINVAL;
}
#else /* !CONFIG_DEBUG_WRITECOUNT */
static inline void file_take_write(struct file *filp) {}
static inline void file_release_write(struct file *filp) {}
static inline void file_reset_write(struct file *filp) {}
static inline void file_check_state(struct file *filp) {}
static inline int file_check_writeable(struct file *filp)
{
	return 0;
}
#endif /* CONFIG_DEBUG_WRITECOUNT */
=======
	unsigned char f_handle[];
};

static inline struct file *get_file(struct file *f)
{
	atomic_long_inc(&f->f_count);
	return f;
}

struct file *get_file_rcu(struct file __rcu **f);
struct file *get_file_active(struct file **f);

#define file_count(x)	atomic_long_read(&(x)->f_count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define	MAX_NON_LFS	((1UL<<31) - 1)

/* Page cache limit. The filesystems should put that into their s_maxbytes 
   limits, otherwise bad things can happen in VM. */ 
#if BITS_PER_LONG==32
<<<<<<< HEAD
#define MAX_LFS_FILESIZE	(((u64)PAGE_CACHE_SIZE << (BITS_PER_LONG-1))-1) 
#elif BITS_PER_LONG==64
#define MAX_LFS_FILESIZE 	0x7fffffffffffffffUL
#endif

#define FL_POSIX	1
#define FL_FLOCK	2
#define FL_ACCESS	8	/* not trying to lock, just looking */
#define FL_EXISTS	16	/* when unlocking, test for existence */
#define FL_LEASE	32	/* lease held on this file */
#define FL_CLOSE	64	/* unlock on close */
#define FL_SLEEP	128	/* A blocking lock */
#define FL_DOWNGRADE_PENDING	256 /* Lease is being downgraded */
#define FL_UNLOCK_PENDING	512 /* Lease is being broken */
#define FL_OFDLCK	1024	/* lock is "owned" by struct file */

/*
 * Special return value from posix_lock_file() and vfs_lock_file() for
 * asynchronous locking.
 */
#define FILE_LOCK_DEFERRED 1

/*
 * The POSIX file lock owner is determined by
 * the "struct files_struct" in the thread group
 * (or NULL for no owner - BSD locks).
 *
 * Lockd stuffs a "host" pointer into this.
 */
typedef struct files_struct *fl_owner_t;

struct file_lock_operations {
	void (*fl_copy_lock)(struct file_lock *, struct file_lock *);
	void (*fl_release_private)(struct file_lock *);
};

struct lock_manager_operations {
	int (*lm_compare_owner)(struct file_lock *, struct file_lock *);
	void (*lm_notify)(struct file_lock *);	/* unblock callback */
	int (*lm_grant)(struct file_lock *, struct file_lock *, int);
	void (*lm_release_private)(struct file_lock *);
	void (*lm_break)(struct file_lock *);
	int (*lm_change)(struct file_lock **, int);
};

struct lock_manager {
	struct list_head list;
};

void locks_start_grace(struct lock_manager *);
void locks_end_grace(struct lock_manager *);
int locks_in_grace(void);

/* that will die - we need it for nfs_lock_info */
#include <linux/nfs_fs_i.h>

struct file_lock {
	struct file_lock *fl_next;	/* singly linked list for this inode  */
	struct list_head fl_link;	/* doubly linked list of all locks */
	struct list_head fl_block;	/* circular list of blocked processes */
	fl_owner_t fl_owner;
	unsigned int fl_flags;
	unsigned char fl_type;
	unsigned int fl_pid;
	struct pid *fl_nspid;
	wait_queue_head_t fl_wait;
	struct file *fl_file;
	loff_t fl_start;
	loff_t fl_end;

	struct fasync_struct *	fl_fasync; /* for lease break notifications */
	/* for lease breaks: */
	unsigned long fl_break_time;
	unsigned long fl_downgrade_time;

	const struct file_lock_operations *fl_ops;	/* Callbacks for filesystems */
	const struct lock_manager_operations *fl_lmops;	/* Callbacks for lockmanagers */
	union {
		struct nfs_lock_info	nfs_fl;
		struct nfs4_lock_info	nfs4_fl;
		struct {
			struct list_head link;	/* link in AFS vnode's pending_locks list */
			int state;		/* state of grant or error if -ve */
		} afs;
	} fl_u;
};

/* The following constant reflects the upper bound of the file/locking space */
#ifndef OFFSET_MAX
#define INT_LIMIT(x)	(~((x)1 << (sizeof(x)*8 - 1)))
#define OFFSET_MAX	INT_LIMIT(loff_t)
#define OFFT_OFFSET_MAX	INT_LIMIT(off_t)
#endif

#include <linux/fcntl.h>

extern void send_sigio(struct fown_struct *fown, int fd, int band);

#ifdef CONFIG_FILE_LOCKING
extern int fcntl_getlk(struct file *, unsigned int, struct flock __user *);
extern int fcntl_setlk(unsigned int, struct file *, unsigned int,
			struct flock __user *);

#if BITS_PER_LONG == 32
extern int fcntl_getlk64(struct file *, unsigned int, struct flock64 __user *);
extern int fcntl_setlk64(unsigned int, struct file *, unsigned int,
			struct flock64 __user *);
#endif

extern int fcntl_setlease(unsigned int fd, struct file *filp, long arg);
extern int fcntl_getlease(struct file *filp);

/* fs/locks.c */
void locks_free_lock(struct file_lock *fl);
extern void locks_init_lock(struct file_lock *);
extern struct file_lock * locks_alloc_lock(void);
extern void locks_copy_lock(struct file_lock *, struct file_lock *);
extern void __locks_copy_lock(struct file_lock *, const struct file_lock *);
extern void locks_remove_posix(struct file *, fl_owner_t);
extern void locks_remove_file(struct file *);
extern void locks_release_private(struct file_lock *);
extern void posix_test_lock(struct file *, struct file_lock *);
extern int posix_lock_file(struct file *, struct file_lock *, struct file_lock *);
extern int posix_lock_file_wait(struct file *, struct file_lock *);
extern int posix_unblock_lock(struct file *, struct file_lock *);
extern int vfs_test_lock(struct file *, struct file_lock *);
extern int vfs_lock_file(struct file *, unsigned int, struct file_lock *, struct file_lock *);
extern int vfs_cancel_lock(struct file *filp, struct file_lock *fl);
extern int flock_lock_file_wait(struct file *filp, struct file_lock *fl);
extern int __break_lease(struct inode *inode, unsigned int flags);
extern void lease_get_mtime(struct inode *, struct timespec *time);
extern int generic_setlease(struct file *, long, struct file_lock **);
extern int vfs_setlease(struct file *, long, struct file_lock **);
extern int lease_modify(struct file_lock **, int);
extern int lock_may_read(struct inode *, loff_t start, unsigned long count);
extern int lock_may_write(struct inode *, loff_t start, unsigned long count);
extern void locks_delete_block(struct file_lock *waiter);
extern void lock_flocks(void);
extern void unlock_flocks(void);
#else /* !CONFIG_FILE_LOCKING */
static inline int fcntl_getlk(struct file *file, unsigned int cmd,
			      struct flock __user *user)
{
	return -EINVAL;
}

static inline int fcntl_setlk(unsigned int fd, struct file *file,
			      unsigned int cmd, struct flock __user *user)
{
	return -EACCES;
}

#if BITS_PER_LONG == 32
static inline int fcntl_getlk64(struct file *file, unsigned int cmd,
				struct flock64 __user *user)
{
	return -EINVAL;
}

static inline int fcntl_setlk64(unsigned int fd, struct file *file,
				unsigned int cmd, struct flock64 __user *user)
{
	return -EACCES;
}
#endif
static inline int fcntl_setlease(unsigned int fd, struct file *filp, long arg)
{
	return 0;
}

static inline int fcntl_getlease(struct file *filp)
{
	return 0;
}

static inline void locks_init_lock(struct file_lock *fl)
{
	return;
}

static inline void __locks_copy_lock(struct file_lock *new, struct file_lock *fl)
{
	return;
}

static inline void locks_copy_lock(struct file_lock *new, struct file_lock *fl)
{
	return;
}

static inline void locks_remove_posix(struct file *filp, fl_owner_t owner)
{
	return;
}

static inline void locks_remove_file(struct file *filp)
{
	return;
}

static inline void posix_test_lock(struct file *filp, struct file_lock *fl)
{
	return;
}

static inline int posix_lock_file(struct file *filp, struct file_lock *fl,
				  struct file_lock *conflock)
{
	return -ENOLCK;
}

static inline int posix_lock_file_wait(struct file *filp, struct file_lock *fl)
{
	return -ENOLCK;
}

static inline int posix_unblock_lock(struct file *filp,
				     struct file_lock *waiter)
{
	return -ENOENT;
}

static inline int vfs_test_lock(struct file *filp, struct file_lock *fl)
{
	return 0;
}

static inline int vfs_lock_file(struct file *filp, unsigned int cmd,
				struct file_lock *fl, struct file_lock *conf)
{
	return -ENOLCK;
}

static inline int vfs_cancel_lock(struct file *filp, struct file_lock *fl)
{
	return 0;
}

static inline int flock_lock_file_wait(struct file *filp,
				       struct file_lock *request)
{
	return -ENOLCK;
}

static inline int __break_lease(struct inode *inode, unsigned int mode)
{
	return 0;
}

static inline void lease_get_mtime(struct inode *inode, struct timespec *time)
{
	return;
}

static inline int generic_setlease(struct file *filp, long arg,
				    struct file_lock **flp)
{
	return -EINVAL;
}

static inline int vfs_setlease(struct file *filp, long arg,
			       struct file_lock **lease)
{
	return -EINVAL;
}

static inline int lease_modify(struct file_lock **before, int arg)
{
	return -EINVAL;
}

static inline int lock_may_read(struct inode *inode, loff_t start,
				unsigned long len)
{
	return 1;
}

static inline int lock_may_write(struct inode *inode, loff_t start,
				 unsigned long len)
{
	return 1;
}

static inline void locks_delete_block(struct file_lock *waiter)
{
}

static inline void lock_flocks(void)
{
}

static inline void unlock_flocks(void)
{
}

#endif /* !CONFIG_FILE_LOCKING */


struct fasync_struct {
	spinlock_t		fa_lock;
=======
#define MAX_LFS_FILESIZE	((loff_t)ULONG_MAX << PAGE_SHIFT)
#elif BITS_PER_LONG==64
#define MAX_LFS_FILESIZE 	((loff_t)LLONG_MAX)
#endif

/* legacy typedef, should eventually be removed */
typedef void *fl_owner_t;

struct file_lock;
struct file_lease;

/* The following constant reflects the upper bound of the file/locking space */
#ifndef OFFSET_MAX
#define OFFSET_MAX	type_max(loff_t)
#define OFFT_OFFSET_MAX	type_max(off_t)
#endif

extern void send_sigio(struct fown_struct *fown, int fd, int band);

static inline struct inode *file_inode(const struct file *f)
{
	return f->f_inode;
}

/*
 * file_dentry() is a relic from the days that overlayfs was using files with a
 * "fake" path, meaning, f_path on overlayfs and f_inode on underlying fs.
 * In those days, file_dentry() was needed to get the underlying fs dentry that
 * matches f_inode.
 * Files with "fake" path should not exist nowadays, so use an assertion to make
 * sure that file_dentry() was not papering over filesystem bugs.
 */
static inline struct dentry *file_dentry(const struct file *file)
{
	struct dentry *dentry = file->f_path.dentry;

	WARN_ON_ONCE(d_inode(dentry) != file_inode(file));
	return dentry;
}

struct fasync_struct {
	rwlock_t		fa_lock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			magic;
	int			fa_fd;
	struct fasync_struct	*fa_next; /* singly linked list */
	struct file		*fa_file;
	struct rcu_head		fa_rcu;
};

#define FASYNC_MAGIC 0x4601

/* SMP safe fasync helpers: */
extern int fasync_helper(int, struct file *, int, struct fasync_struct **);
extern struct fasync_struct *fasync_insert_entry(int, struct file *, struct fasync_struct **, struct fasync_struct *);
extern int fasync_remove_entry(struct file *, struct fasync_struct **);
extern struct fasync_struct *fasync_alloc(void);
extern void fasync_free(struct fasync_struct *);

/* can be called from interrupts */
extern void kill_fasync(struct fasync_struct **, int, int);

<<<<<<< HEAD
extern int __f_setown(struct file *filp, struct pid *, enum pid_type, int force);
extern int f_setown(struct file *filp, unsigned long arg, int force);
=======
extern void __f_setown(struct file *filp, struct pid *, enum pid_type, int force);
extern int f_setown(struct file *filp, int who, int force);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void f_delown(struct file *filp);
extern pid_t f_getown(struct file *filp);
extern int send_sigurg(struct fown_struct *fown);

/*
<<<<<<< HEAD
=======
 * sb->s_flags.  Note that these mirror the equivalent MS_* flags where
 * represented in both.
 */
#define SB_RDONLY       BIT(0)	/* Mount read-only */
#define SB_NOSUID       BIT(1)	/* Ignore suid and sgid bits */
#define SB_NODEV        BIT(2)	/* Disallow access to device special files */
#define SB_NOEXEC       BIT(3)	/* Disallow program execution */
#define SB_SYNCHRONOUS  BIT(4)	/* Writes are synced at once */
#define SB_MANDLOCK     BIT(6)	/* Allow mandatory locks on an FS */
#define SB_DIRSYNC      BIT(7)	/* Directory modifications are synchronous */
#define SB_NOATIME      BIT(10)	/* Do not update access times. */
#define SB_NODIRATIME   BIT(11)	/* Do not update directory access times */
#define SB_SILENT       BIT(15)
#define SB_POSIXACL     BIT(16)	/* Supports POSIX ACLs */
#define SB_INLINECRYPT  BIT(17)	/* Use blk-crypto for encrypted files */
#define SB_KERNMOUNT    BIT(22)	/* this is a kern_mount call */
#define SB_I_VERSION    BIT(23)	/* Update inode I_version field */
#define SB_LAZYTIME     BIT(25)	/* Update the on-disk [acm]times lazily */

/* These sb flags are internal to the kernel */
#define SB_DEAD         BIT(21)
#define SB_DYING        BIT(24)
#define SB_SUBMOUNT     BIT(26)
#define SB_FORCE        BIT(27)
#define SB_NOSEC        BIT(28)
#define SB_BORN         BIT(29)
#define SB_ACTIVE       BIT(30)
#define SB_NOUSER       BIT(31)

/* These flags relate to encoding and casefolding */
#define SB_ENC_STRICT_MODE_FL	(1 << 0)

#define sb_has_strict_encoding(sb) \
	(sb->s_encoding_flags & SB_ENC_STRICT_MODE_FL)

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Umount options
 */

#define MNT_FORCE	0x00000001	/* Attempt to forcibily umount */
#define MNT_DETACH	0x00000002	/* Just detach from the tree */
#define MNT_EXPIRE	0x00000004	/* Mark for expiry */
#define UMOUNT_NOFOLLOW	0x00000008	/* Don't follow symlink on umount */
#define UMOUNT_UNUSED	0x80000000	/* Flag guaranteed to be unused */

<<<<<<< HEAD
extern struct list_head super_blocks;
extern spinlock_t sb_lock;
=======
/* sb->s_iflags */
#define SB_I_CGROUPWB	0x00000001	/* cgroup-aware writeback enabled */
#define SB_I_NOEXEC	0x00000002	/* Ignore executables on this fs */
#define SB_I_NODEV	0x00000004	/* Ignore devices on this fs */
#define SB_I_STABLE_WRITES 0x00000008	/* don't modify blks until WB is done */

/* sb->s_iflags to limit user namespace mounts */
#define SB_I_USERNS_VISIBLE		0x00000010 /* fstype already mounted */
#define SB_I_IMA_UNVERIFIABLE_SIGNATURE	0x00000020
#define SB_I_UNTRUSTED_MOUNTER		0x00000040
#define SB_I_EVM_UNSUPPORTED		0x00000080

#define SB_I_SKIP_SYNC	0x00000100	/* Skip superblock at global sync */
#define SB_I_PERSB_BDI	0x00000200	/* has a per-sb bdi */
#define SB_I_TS_EXPIRY_WARNED 0x00000400 /* warned about timestamp range expiry */
#define SB_I_RETIRED	0x00000800	/* superblock shouldn't be reused */
#define SB_I_NOUMASK	0x00001000	/* VFS does not apply umask */

/* Possible states of 'frozen' field */
enum {
	SB_UNFROZEN = 0,		/* FS is unfrozen */
	SB_FREEZE_WRITE	= 1,		/* Writes, dir ops, ioctls frozen */
	SB_FREEZE_PAGEFAULT = 2,	/* Page faults stopped as well */
	SB_FREEZE_FS = 3,		/* For internal FS use (e.g. to stop
					 * internal threads if needed) */
	SB_FREEZE_COMPLETE = 4,		/* ->freeze_fs finished successfully */
};

#define SB_FREEZE_LEVELS (SB_FREEZE_COMPLETE - 1)

struct sb_writers {
	unsigned short			frozen;		/* Is sb frozen? */
	int				freeze_kcount;	/* How many kernel freeze requests? */
	int				freeze_ucount;	/* How many userspace freeze requests? */
	struct percpu_rw_semaphore	rw_sem[SB_FREEZE_LEVELS];
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct super_block {
	struct list_head	s_list;		/* Keep this first */
	dev_t			s_dev;		/* search index; _not_ kdev_t */
<<<<<<< HEAD
	unsigned char		s_dirt;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char		s_blocksize_bits;
	unsigned long		s_blocksize;
	loff_t			s_maxbytes;	/* Max file size */
	struct file_system_type	*s_type;
	const struct super_operations	*s_op;
	const struct dquot_operations	*dq_op;
	const struct quotactl_ops	*s_qcop;
	const struct export_operations *s_export_op;
	unsigned long		s_flags;
<<<<<<< HEAD
	unsigned long		s_magic;
	struct dentry		*s_root;
	struct rw_semaphore	s_umount;
	struct mutex		s_lock;
=======
	unsigned long		s_iflags;	/* internal SB_I_* flags */
	unsigned long		s_magic;
	struct dentry		*s_root;
	struct rw_semaphore	s_umount;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			s_count;
	atomic_t		s_active;
#ifdef CONFIG_SECURITY
	void                    *s_security;
#endif
<<<<<<< HEAD
	const struct xattr_handler **s_xattr;

	struct list_head	s_inodes;	/* all inodes */

	const struct fscrypt_operations *s_cop;

	struct hlist_bl_head	s_anon;		/* anonymous dentries for (nfs) exporting */
	struct list_head	s_mounts;	/* list of mounts; _not_ for fs use */
	/* s_dentry_lru, s_nr_dentry_unused protected by dcache.c lru locks */
	struct list_head	s_dentry_lru;	/* unused dentry lru */
	int			s_nr_dentry_unused;	/* # of dentry on lru */

	/* s_inode_lru_lock protects s_inode_lru and s_nr_inodes_unused */
	spinlock_t		s_inode_lru_lock ____cacheline_aligned_in_smp;
	struct list_head	s_inode_lru;		/* unused inode lru */
	int			s_nr_inodes_unused;	/* # of inodes on lru */

	struct block_device	*s_bdev;
	struct backing_dev_info *s_bdi;
	struct mtd_info		*s_mtd;
	struct hlist_node	s_instances;
	struct quota_info	s_dquot;	/* Diskquota specific options */

	int			s_frozen;
	wait_queue_head_t	s_wait_unfrozen;

	char s_id[32];				/* Informational name */
	u8 s_uuid[16];				/* UUID */

	void 			*s_fs_info;	/* Filesystem private info */
	unsigned int		s_max_links;
	fmode_t			s_mode;

	/* Granularity of c/m/atime in ns.
	   Cannot be worse than a second */
	u32		   s_time_gran;
=======
	const struct xattr_handler * const *s_xattr;
#ifdef CONFIG_FS_ENCRYPTION
	const struct fscrypt_operations	*s_cop;
	struct fscrypt_keyring	*s_master_keys; /* master crypto keys in use */
#endif
#ifdef CONFIG_FS_VERITY
	const struct fsverity_operations *s_vop;
#endif
#if IS_ENABLED(CONFIG_UNICODE)
	struct unicode_map *s_encoding;
	__u16 s_encoding_flags;
#endif
	struct hlist_bl_head	s_roots;	/* alternate root dentries for NFS */
	struct list_head	s_mounts;	/* list of mounts; _not_ for fs use */
	struct block_device	*s_bdev;	/* can go away once we use an accessor for @s_bdev_file */
	struct file		*s_bdev_file;
	struct backing_dev_info *s_bdi;
	struct mtd_info		*s_mtd;
	struct hlist_node	s_instances;
	unsigned int		s_quota_types;	/* Bitmask of supported quota types */
	struct quota_info	s_dquot;	/* Diskquota specific options */

	struct sb_writers	s_writers;

	/*
	 * Keep s_fs_info, s_time_gran, s_fsnotify_mask, and
	 * s_fsnotify_marks together for cache efficiency. They are frequently
	 * accessed and rarely modified.
	 */
	void			*s_fs_info;	/* Filesystem private info */

	/* Granularity of c/m/atime in ns (cannot be worse than a second) */
	u32			s_time_gran;
	/* Time limits for c/m/atime in seconds */
	time64_t		   s_time_min;
	time64_t		   s_time_max;
#ifdef CONFIG_FSNOTIFY
	__u32			s_fsnotify_mask;
	struct fsnotify_mark_connector __rcu	*s_fsnotify_marks;
#endif

	/*
	 * q: why are s_id and s_sysfs_name not the same? both are human
	 * readable strings that identify the filesystem
	 * a: s_id is allowed to change at runtime; it's used in log messages,
	 * and we want to when a device starts out as single device (s_id is dev
	 * name) but then a device is hot added and we have to switch to
	 * identifying it by UUID
	 * but s_sysfs_name is a handle for programmatic access, and can't
	 * change at runtime
	 */
	char			s_id[32];	/* Informational name */
	uuid_t			s_uuid;		/* UUID */
	u8			s_uuid_len;	/* Default 16, possibly smaller for weird filesystems */

	/* if set, fs shows up under sysfs at /sys/fs/$FSTYP/s_sysfs_name */
	char			s_sysfs_name[UUID_STRING_LEN + 1];

	unsigned int		s_max_links;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The next field is for VFS *only*. No filesystems have any business
	 * even looking at it. You had been warned.
	 */
	struct mutex s_vfs_rename_mutex;	/* Kludge */

	/*
	 * Filesystem subtype.  If non-empty the filesystem type field
	 * in /proc/mounts will be "type.subtype"
	 */
<<<<<<< HEAD
	char *s_subtype;

	/*
	 * Saved mount options for lazy filesystems using
	 * generic_show_options()
	 */
	char __rcu *s_options;
	const struct dentry_operations *s_d_op; /* default d_op for dentries */

	/*
	 * Saved pool identifier for cleancache (-1 means none)
	 */
	int cleancache_poolid;

	struct shrinker s_shrink;	/* per-sb shrinker handle */
=======
	const char *s_subtype;

	const struct dentry_operations *s_d_op; /* default d_op for dentries */

	struct shrinker *s_shrink;	/* per-sb shrinker handle */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Number of inodes with nlink == 0 but still referenced */
	atomic_long_t s_remove_count;

<<<<<<< HEAD
	/* Being remounted read-only */
	int s_readonly_remount;

=======
	/*
	 * Number of inode/mount/sb objects that are being watched, note that
	 * inodes objects are currently double-accounted.
	 */
	atomic_long_t s_fsnotify_connectors;

	/* Read-only state of the superblock is being changed */
	int s_readonly_remount;

	/* per-sb errseq_t for reporting writeback errors via syncfs */
	errseq_t s_wb_err;

	/* AIO completions deferred from interrupt context */
	struct workqueue_struct *s_dio_done_wq;
	struct hlist_head s_pins;

	/*
	 * Owning user namespace and default context in which to
	 * interpret filesystem uids, gids, quotas, device nodes,
	 * xattrs and security labels.
	 */
	struct user_namespace *s_user_ns;

	/*
	 * The list_lru structure is essentially just a pointer to a table
	 * of per-node lru lists, each of which has its own spinlock.
	 * There is no need to put them into separate cachelines.
	 */
	struct list_lru		s_dentry_lru;
	struct list_lru		s_inode_lru;
	struct rcu_head		rcu;
	struct work_struct	destroy_work;

	struct mutex		s_sync_lock;	/* sync serialisation lock */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Indicates how deep in a filesystem stack this SB is
	 */
	int s_stack_depth;
<<<<<<< HEAD
};

/* superblock cache pruning functions */
extern void prune_icache_sb(struct super_block *sb, int nr_to_scan);
extern void prune_dcache_sb(struct super_block *sb, int nr_to_scan);

extern struct timespec current_fs_time(struct super_block *sb);
=======

	/* s_inode_list_lock protects s_inodes */
	spinlock_t		s_inode_list_lock ____cacheline_aligned_in_smp;
	struct list_head	s_inodes;	/* all inodes */

	spinlock_t		s_inode_wblist_lock;
	struct list_head	s_inodes_wb;	/* writeback inodes */
} __randomize_layout;

static inline struct user_namespace *i_user_ns(const struct inode *inode)
{
	return inode->i_sb->s_user_ns;
}

/* Helper functions so that in most cases filesystems will
 * not need to deal directly with kuid_t and kgid_t and can
 * instead deal with the raw numeric values that are stored
 * in the filesystem.
 */
static inline uid_t i_uid_read(const struct inode *inode)
{
	return from_kuid(i_user_ns(inode), inode->i_uid);
}

static inline gid_t i_gid_read(const struct inode *inode)
{
	return from_kgid(i_user_ns(inode), inode->i_gid);
}

static inline void i_uid_write(struct inode *inode, uid_t uid)
{
	inode->i_uid = make_kuid(i_user_ns(inode), uid);
}

static inline void i_gid_write(struct inode *inode, gid_t gid)
{
	inode->i_gid = make_kgid(i_user_ns(inode), gid);
}

/**
 * i_uid_into_vfsuid - map an inode's i_uid down according to an idmapping
 * @idmap: idmap of the mount the inode was found from
 * @inode: inode to map
 *
 * Return: whe inode's i_uid mapped down according to @idmap.
 * If the inode's i_uid has no mapping INVALID_VFSUID is returned.
 */
static inline vfsuid_t i_uid_into_vfsuid(struct mnt_idmap *idmap,
					 const struct inode *inode)
{
	return make_vfsuid(idmap, i_user_ns(inode), inode->i_uid);
}

/**
 * i_uid_needs_update - check whether inode's i_uid needs to be updated
 * @idmap: idmap of the mount the inode was found from
 * @attr: the new attributes of @inode
 * @inode: the inode to update
 *
 * Check whether the $inode's i_uid field needs to be updated taking idmapped
 * mounts into account if the filesystem supports it.
 *
 * Return: true if @inode's i_uid field needs to be updated, false if not.
 */
static inline bool i_uid_needs_update(struct mnt_idmap *idmap,
				      const struct iattr *attr,
				      const struct inode *inode)
{
	return ((attr->ia_valid & ATTR_UID) &&
		!vfsuid_eq(attr->ia_vfsuid,
			   i_uid_into_vfsuid(idmap, inode)));
}

/**
 * i_uid_update - update @inode's i_uid field
 * @idmap: idmap of the mount the inode was found from
 * @attr: the new attributes of @inode
 * @inode: the inode to update
 *
 * Safely update @inode's i_uid field translating the vfsuid of any idmapped
 * mount into the filesystem kuid.
 */
static inline void i_uid_update(struct mnt_idmap *idmap,
				const struct iattr *attr,
				struct inode *inode)
{
	if (attr->ia_valid & ATTR_UID)
		inode->i_uid = from_vfsuid(idmap, i_user_ns(inode),
					   attr->ia_vfsuid);
}

/**
 * i_gid_into_vfsgid - map an inode's i_gid down according to an idmapping
 * @idmap: idmap of the mount the inode was found from
 * @inode: inode to map
 *
 * Return: the inode's i_gid mapped down according to @idmap.
 * If the inode's i_gid has no mapping INVALID_VFSGID is returned.
 */
static inline vfsgid_t i_gid_into_vfsgid(struct mnt_idmap *idmap,
					 const struct inode *inode)
{
	return make_vfsgid(idmap, i_user_ns(inode), inode->i_gid);
}

/**
 * i_gid_needs_update - check whether inode's i_gid needs to be updated
 * @idmap: idmap of the mount the inode was found from
 * @attr: the new attributes of @inode
 * @inode: the inode to update
 *
 * Check whether the $inode's i_gid field needs to be updated taking idmapped
 * mounts into account if the filesystem supports it.
 *
 * Return: true if @inode's i_gid field needs to be updated, false if not.
 */
static inline bool i_gid_needs_update(struct mnt_idmap *idmap,
				      const struct iattr *attr,
				      const struct inode *inode)
{
	return ((attr->ia_valid & ATTR_GID) &&
		!vfsgid_eq(attr->ia_vfsgid,
			   i_gid_into_vfsgid(idmap, inode)));
}

/**
 * i_gid_update - update @inode's i_gid field
 * @idmap: idmap of the mount the inode was found from
 * @attr: the new attributes of @inode
 * @inode: the inode to update
 *
 * Safely update @inode's i_gid field translating the vfsgid of any idmapped
 * mount into the filesystem kgid.
 */
static inline void i_gid_update(struct mnt_idmap *idmap,
				const struct iattr *attr,
				struct inode *inode)
{
	if (attr->ia_valid & ATTR_GID)
		inode->i_gid = from_vfsgid(idmap, i_user_ns(inode),
					   attr->ia_vfsgid);
}

/**
 * inode_fsuid_set - initialize inode's i_uid field with callers fsuid
 * @inode: inode to initialize
 * @idmap: idmap of the mount the inode was found from
 *
 * Initialize the i_uid field of @inode. If the inode was found/created via
 * an idmapped mount map the caller's fsuid according to @idmap.
 */
static inline void inode_fsuid_set(struct inode *inode,
				   struct mnt_idmap *idmap)
{
	inode->i_uid = mapped_fsuid(idmap, i_user_ns(inode));
}

/**
 * inode_fsgid_set - initialize inode's i_gid field with callers fsgid
 * @inode: inode to initialize
 * @idmap: idmap of the mount the inode was found from
 *
 * Initialize the i_gid field of @inode. If the inode was found/created via
 * an idmapped mount map the caller's fsgid according to @idmap.
 */
static inline void inode_fsgid_set(struct inode *inode,
				   struct mnt_idmap *idmap)
{
	inode->i_gid = mapped_fsgid(idmap, i_user_ns(inode));
}

/**
 * fsuidgid_has_mapping() - check whether caller's fsuid/fsgid is mapped
 * @sb: the superblock we want a mapping in
 * @idmap: idmap of the relevant mount
 *
 * Check whether the caller's fsuid and fsgid have a valid mapping in the
 * s_user_ns of the superblock @sb. If the caller is on an idmapped mount map
 * the caller's fsuid and fsgid according to the @idmap first.
 *
 * Return: true if fsuid and fsgid is mapped, false if not.
 */
static inline bool fsuidgid_has_mapping(struct super_block *sb,
					struct mnt_idmap *idmap)
{
	struct user_namespace *fs_userns = sb->s_user_ns;
	kuid_t kuid;
	kgid_t kgid;

	kuid = mapped_fsuid(idmap, fs_userns);
	if (!uid_valid(kuid))
		return false;
	kgid = mapped_fsgid(idmap, fs_userns);
	if (!gid_valid(kgid))
		return false;
	return kuid_has_mapping(fs_userns, kuid) &&
	       kgid_has_mapping(fs_userns, kgid);
}

struct timespec64 current_time(struct inode *inode);
struct timespec64 inode_set_ctime_current(struct inode *inode);

static inline time64_t inode_get_atime_sec(const struct inode *inode)
{
	return inode->__i_atime.tv_sec;
}

static inline long inode_get_atime_nsec(const struct inode *inode)
{
	return inode->__i_atime.tv_nsec;
}

static inline struct timespec64 inode_get_atime(const struct inode *inode)
{
	return inode->__i_atime;
}

static inline struct timespec64 inode_set_atime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
	inode->__i_atime = ts;
	return ts;
}

static inline struct timespec64 inode_set_atime(struct inode *inode,
						time64_t sec, long nsec)
{
	struct timespec64 ts = { .tv_sec  = sec,
				 .tv_nsec = nsec };
	return inode_set_atime_to_ts(inode, ts);
}

static inline time64_t inode_get_mtime_sec(const struct inode *inode)
{
	return inode->__i_mtime.tv_sec;
}

static inline long inode_get_mtime_nsec(const struct inode *inode)
{
	return inode->__i_mtime.tv_nsec;
}

static inline struct timespec64 inode_get_mtime(const struct inode *inode)
{
	return inode->__i_mtime;
}

static inline struct timespec64 inode_set_mtime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
	inode->__i_mtime = ts;
	return ts;
}

static inline struct timespec64 inode_set_mtime(struct inode *inode,
						time64_t sec, long nsec)
{
	struct timespec64 ts = { .tv_sec  = sec,
				 .tv_nsec = nsec };
	return inode_set_mtime_to_ts(inode, ts);
}

static inline time64_t inode_get_ctime_sec(const struct inode *inode)
{
	return inode->__i_ctime.tv_sec;
}

static inline long inode_get_ctime_nsec(const struct inode *inode)
{
	return inode->__i_ctime.tv_nsec;
}

static inline struct timespec64 inode_get_ctime(const struct inode *inode)
{
	return inode->__i_ctime;
}

static inline struct timespec64 inode_set_ctime_to_ts(struct inode *inode,
						      struct timespec64 ts)
{
	inode->__i_ctime = ts;
	return ts;
}

/**
 * inode_set_ctime - set the ctime in the inode
 * @inode: inode in which to set the ctime
 * @sec: tv_sec value to set
 * @nsec: tv_nsec value to set
 *
 * Set the ctime in @inode to { @sec, @nsec }
 */
static inline struct timespec64 inode_set_ctime(struct inode *inode,
						time64_t sec, long nsec)
{
	struct timespec64 ts = { .tv_sec  = sec,
				 .tv_nsec = nsec };

	return inode_set_ctime_to_ts(inode, ts);
}

struct timespec64 simple_inode_init_ts(struct inode *inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Snapshotting support.
 */
<<<<<<< HEAD
enum {
	SB_UNFROZEN = 0,
	SB_FREEZE_WRITE	= 1,
	SB_FREEZE_TRANS = 2,
};

#define vfs_check_frozen(sb, level) \
	wait_event((sb)->s_wait_unfrozen, ((sb)->s_frozen < (level)))

/*
 * until VFS tracks user namespaces for inodes, just make all files
 * belong to init_user_ns
 */
extern struct user_namespace init_user_ns;
#define inode_userns(inode) (&init_user_ns)
extern bool inode_owner_or_capable(const struct inode *inode);

/* not quite ready to be deprecated, but... */
extern void lock_super(struct super_block *);
extern void unlock_super(struct super_block *);
=======

/*
 * These are internal functions, please use sb_start_{write,pagefault,intwrite}
 * instead.
 */
static inline void __sb_end_write(struct super_block *sb, int level)
{
	percpu_up_read(sb->s_writers.rw_sem + level-1);
}

static inline void __sb_start_write(struct super_block *sb, int level)
{
	percpu_down_read(sb->s_writers.rw_sem + level - 1);
}

static inline bool __sb_start_write_trylock(struct super_block *sb, int level)
{
	return percpu_down_read_trylock(sb->s_writers.rw_sem + level - 1);
}

#define __sb_writers_acquired(sb, lev)	\
	percpu_rwsem_acquire(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)
#define __sb_writers_release(sb, lev)	\
	percpu_rwsem_release(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)

/**
 * __sb_write_started - check if sb freeze level is held
 * @sb: the super we write to
 * @level: the freeze level
 *
 * * > 0 - sb freeze level is held
 * *   0 - sb freeze level is not held
 * * < 0 - !CONFIG_LOCKDEP/LOCK_STATE_UNKNOWN
 */
static inline int __sb_write_started(const struct super_block *sb, int level)
{
	return lockdep_is_held_type(sb->s_writers.rw_sem + level - 1, 1);
}

/**
 * sb_write_started - check if SB_FREEZE_WRITE is held
 * @sb: the super we write to
 *
 * May be false positive with !CONFIG_LOCKDEP/LOCK_STATE_UNKNOWN.
 */
static inline bool sb_write_started(const struct super_block *sb)
{
	return __sb_write_started(sb, SB_FREEZE_WRITE);
}

/**
 * sb_write_not_started - check if SB_FREEZE_WRITE is not held
 * @sb: the super we write to
 *
 * May be false positive with !CONFIG_LOCKDEP/LOCK_STATE_UNKNOWN.
 */
static inline bool sb_write_not_started(const struct super_block *sb)
{
	return __sb_write_started(sb, SB_FREEZE_WRITE) <= 0;
}

/**
 * file_write_started - check if SB_FREEZE_WRITE is held
 * @file: the file we write to
 *
 * May be false positive with !CONFIG_LOCKDEP/LOCK_STATE_UNKNOWN.
 * May be false positive with !S_ISREG, because file_start_write() has
 * no effect on !S_ISREG.
 */
static inline bool file_write_started(const struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return true;
	return sb_write_started(file_inode(file)->i_sb);
}

/**
 * file_write_not_started - check if SB_FREEZE_WRITE is not held
 * @file: the file we write to
 *
 * May be false positive with !CONFIG_LOCKDEP/LOCK_STATE_UNKNOWN.
 * May be false positive with !S_ISREG, because file_start_write() has
 * no effect on !S_ISREG.
 */
static inline bool file_write_not_started(const struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return true;
	return sb_write_not_started(file_inode(file)->i_sb);
}

/**
 * sb_end_write - drop write access to a superblock
 * @sb: the super we wrote to
 *
 * Decrement number of writers to the filesystem. Wake up possible waiters
 * wanting to freeze the filesystem.
 */
static inline void sb_end_write(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_WRITE);
}

/**
 * sb_end_pagefault - drop write access to a superblock from a page fault
 * @sb: the super we wrote to
 *
 * Decrement number of processes handling write page fault to the filesystem.
 * Wake up possible waiters wanting to freeze the filesystem.
 */
static inline void sb_end_pagefault(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_PAGEFAULT);
}

/**
 * sb_end_intwrite - drop write access to a superblock for internal fs purposes
 * @sb: the super we wrote to
 *
 * Decrement fs-internal number of writers to the filesystem.  Wake up possible
 * waiters wanting to freeze the filesystem.
 */
static inline void sb_end_intwrite(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_FS);
}

/**
 * sb_start_write - get write access to a superblock
 * @sb: the super we write to
 *
 * When a process wants to write data or metadata to a file system (i.e. dirty
 * a page or an inode), it should embed the operation in a sb_start_write() -
 * sb_end_write() pair to get exclusion against file system freezing. This
 * function increments number of writers preventing freezing. If the file
 * system is already frozen, the function waits until the file system is
 * thawed.
 *
 * Since freeze protection behaves as a lock, users have to preserve
 * ordering of freeze protection and other filesystem locks. Generally,
 * freeze protection should be the outermost lock. In particular, we have:
 *
 * sb_start_write
 *   -> i_mutex			(write path, truncate, directory ops, ...)
 *   -> s_umount		(freeze_super, thaw_super)
 */
static inline void sb_start_write(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_WRITE);
}

static inline bool sb_start_write_trylock(struct super_block *sb)
{
	return __sb_start_write_trylock(sb, SB_FREEZE_WRITE);
}

/**
 * sb_start_pagefault - get write access to a superblock from a page fault
 * @sb: the super we write to
 *
 * When a process starts handling write page fault, it should embed the
 * operation into sb_start_pagefault() - sb_end_pagefault() pair to get
 * exclusion against file system freezing. This is needed since the page fault
 * is going to dirty a page. This function increments number of running page
 * faults preventing freezing. If the file system is already frozen, the
 * function waits until the file system is thawed.
 *
 * Since page fault freeze protection behaves as a lock, users have to preserve
 * ordering of freeze protection and other filesystem locks. It is advised to
 * put sb_start_pagefault() close to mmap_lock in lock ordering. Page fault
 * handling code implies lock dependency:
 *
 * mmap_lock
 *   -> sb_start_pagefault
 */
static inline void sb_start_pagefault(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_PAGEFAULT);
}

/**
 * sb_start_intwrite - get write access to a superblock for internal fs purposes
 * @sb: the super we write to
 *
 * This is the third level of protection against filesystem freezing. It is
 * free for use by a filesystem. The only requirement is that it must rank
 * below sb_start_pagefault.
 *
 * For example filesystem can call sb_start_intwrite() when starting a
 * transaction which somewhat eases handling of freezing for internal sources
 * of filesystem changes (internal fs threads, discarding preallocation on file
 * close, etc.).
 */
static inline void sb_start_intwrite(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_FS);
}

static inline bool sb_start_intwrite_trylock(struct super_block *sb)
{
	return __sb_start_write_trylock(sb, SB_FREEZE_FS);
}

bool inode_owner_or_capable(struct mnt_idmap *idmap,
			    const struct inode *inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * VFS helper functions..
 */
<<<<<<< HEAD
extern int vfs_create(struct inode *, struct dentry *, umode_t, bool);
extern int vfs_create2(struct vfsmount *, struct inode *, struct dentry *, umode_t, bool);
extern int vfs_mkdir(struct inode *, struct dentry *, umode_t);
extern int vfs_mkdir2(struct vfsmount *, struct inode *, struct dentry *, umode_t);
extern int vfs_mknod(struct inode *, struct dentry *, umode_t, dev_t);
extern int vfs_mknod2(struct vfsmount *, struct inode *, struct dentry *, umode_t, dev_t);
extern int vfs_symlink(struct inode *, struct dentry *, const char *);
extern int vfs_link(struct dentry *, struct inode *, struct dentry *);
extern int vfs_link2(struct vfsmount *, struct dentry *, struct inode *, struct dentry *);
extern int vfs_rmdir(struct inode *, struct dentry *);
extern int vfs_rmdir2(struct vfsmount *, struct inode *, struct dentry *);
extern int vfs_unlink(struct inode *, struct dentry *);
extern int vfs_unlink2(struct vfsmount *, struct inode *, struct dentry *);
extern int vfs_rename(struct inode *, struct dentry *, struct inode *, struct dentry *);
extern int vfs_rename2(struct vfsmount *, struct inode *, struct dentry *, struct inode *, struct dentry *);

/*
 * VFS dentry helper functions.
 */
extern void dentry_unhash(struct dentry *dentry);
=======
int vfs_create(struct mnt_idmap *, struct inode *,
	       struct dentry *, umode_t, bool);
int vfs_mkdir(struct mnt_idmap *, struct inode *,
	      struct dentry *, umode_t);
int vfs_mknod(struct mnt_idmap *, struct inode *, struct dentry *,
              umode_t, dev_t);
int vfs_symlink(struct mnt_idmap *, struct inode *,
		struct dentry *, const char *);
int vfs_link(struct dentry *, struct mnt_idmap *, struct inode *,
	     struct dentry *, struct inode **);
int vfs_rmdir(struct mnt_idmap *, struct inode *, struct dentry *);
int vfs_unlink(struct mnt_idmap *, struct inode *, struct dentry *,
	       struct inode **);

/**
 * struct renamedata - contains all information required for renaming
 * @old_mnt_idmap:     idmap of the old mount the inode was found from
 * @old_dir:           parent of source
 * @old_dentry:                source
 * @new_mnt_idmap:     idmap of the new mount the inode was found from
 * @new_dir:           parent of destination
 * @new_dentry:                destination
 * @delegated_inode:   returns an inode needing a delegation break
 * @flags:             rename flags
 */
struct renamedata {
	struct mnt_idmap *old_mnt_idmap;
	struct inode *old_dir;
	struct dentry *old_dentry;
	struct mnt_idmap *new_mnt_idmap;
	struct inode *new_dir;
	struct dentry *new_dentry;
	struct inode **delegated_inode;
	unsigned int flags;
} __randomize_layout;

int vfs_rename(struct renamedata *);

static inline int vfs_whiteout(struct mnt_idmap *idmap,
			       struct inode *dir, struct dentry *dentry)
{
	return vfs_mknod(idmap, dir, dentry, S_IFCHR | WHITEOUT_MODE,
			 WHITEOUT_DEV);
}

struct file *kernel_tmpfile_open(struct mnt_idmap *idmap,
				 const struct path *parentpath,
				 umode_t mode, int open_flag,
				 const struct cred *cred);
struct file *kernel_file_open(const struct path *path, int flags,
			      struct inode *inode, const struct cred *cred);

int vfs_mkobj(struct dentry *, umode_t,
		int (*f)(struct dentry *, umode_t, void *),
		void *);

int vfs_fchown(struct file *file, uid_t user, gid_t group);
int vfs_fchmod(struct file *file, umode_t mode);
int vfs_utimes(const struct path *path, struct timespec64 *times);

extern long vfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#ifdef CONFIG_COMPAT
extern long compat_ptr_ioctl(struct file *file, unsigned int cmd,
					unsigned long arg);
#else
#define compat_ptr_ioctl NULL
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * VFS file helper functions.
 */
<<<<<<< HEAD
extern void inode_init_owner(struct inode *inode, const struct inode *dir,
			umode_t mode);
/*
 * VFS FS_IOC_FIEMAP helper definitions.
 */
struct fiemap_extent_info {
	unsigned int fi_flags;		/* Flags as passed from user */
	unsigned int fi_extents_mapped;	/* Number of mapped extents */
	unsigned int fi_extents_max;	/* Size of fiemap_extent array */
	struct fiemap_extent __user *fi_extents_start; /* Start of
							fiemap_extent array */
};
int fiemap_fill_next_extent(struct fiemap_extent_info *info, u64 logical,
			    u64 phys, u64 len, u32 flags);
int fiemap_check_flags(struct fiemap_extent_info *fieinfo, u32 fs_flags);

/*
 * File types
 *
 * NOTE! These match bits 12..15 of stat.st_mode
 * (ie "(i_mode >> 12) & 15").
 */
#define DT_UNKNOWN	0
#define DT_FIFO		1
#define DT_CHR		2
#define DT_DIR		4
#define DT_BLK		6
#define DT_REG		8
#define DT_LNK		10
#define DT_SOCK		12
#define DT_WHT		14
=======
void inode_init_owner(struct mnt_idmap *idmap, struct inode *inode,
		      const struct inode *dir, umode_t mode);
extern bool may_open_dev(const struct path *path);
umode_t mode_strip_sgid(struct mnt_idmap *idmap,
			const struct inode *dir, umode_t mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the "filldir" function type, used by readdir() to let
 * the kernel specify what kind of dirent layout it wants to have.
 * This allows the kernel to read directories into kernel space or
 * to have different dirent layouts depending on the binary type.
<<<<<<< HEAD
 */
typedef int (*filldir_t)(void *, const char *, int, loff_t, u64, unsigned);
struct block_device_operations;

/* These macros are for out of kernel modules to test that
 * the kernel supports the unlocked_ioctl and compat_ioctl
 * fields in struct file_operations. */
#define HAVE_COMPAT_IOCTL 1
#define HAVE_UNLOCKED_IOCTL 1
=======
 * Return 'true' to keep going and 'false' if there are no more entries.
 */
struct dir_context;
typedef bool (*filldir_t)(struct dir_context *, const char *, int, loff_t, u64,
			 unsigned);

struct dir_context {
	filldir_t actor;
	loff_t pos;
};

/*
 * These flags let !MMU mmap() govern direct device mapping vs immediate
 * copying more easily for MAP_PRIVATE, especially for ROM filesystems.
 *
 * NOMMU_MAP_COPY:	Copy can be mapped (MAP_PRIVATE)
 * NOMMU_MAP_DIRECT:	Can be mapped directly (MAP_SHARED)
 * NOMMU_MAP_READ:	Can be mapped for reading
 * NOMMU_MAP_WRITE:	Can be mapped for writing
 * NOMMU_MAP_EXEC:	Can be mapped for execution
 */
#define NOMMU_MAP_COPY		0x00000001
#define NOMMU_MAP_DIRECT	0x00000008
#define NOMMU_MAP_READ		VM_MAYREAD
#define NOMMU_MAP_WRITE		VM_MAYWRITE
#define NOMMU_MAP_EXEC		VM_MAYEXEC

#define NOMMU_VMFLAGS \
	(NOMMU_MAP_READ | NOMMU_MAP_WRITE | NOMMU_MAP_EXEC)

/*
 * These flags control the behavior of the remap_file_range function pointer.
 * If it is called with len == 0 that means "remap to end of source file".
 * See Documentation/filesystems/vfs.rst for more details about this call.
 *
 * REMAP_FILE_DEDUP: only remap if contents identical (i.e. deduplicate)
 * REMAP_FILE_CAN_SHORTEN: caller can handle a shortened request
 */
#define REMAP_FILE_DEDUP		(1 << 0)
#define REMAP_FILE_CAN_SHORTEN		(1 << 1)

/*
 * These flags signal that the caller is ok with altering various aspects of
 * the behavior of the remap operation.  The changes must be made by the
 * implementation; the vfs remap helper functions can take advantage of them.
 * Flags in this category exist to preserve the quirky behavior of the hoisted
 * btrfs clone/dedupe ioctls.
 */
#define REMAP_FILE_ADVISORY		(REMAP_FILE_CAN_SHORTEN)

/*
 * These flags control the behavior of vfs_copy_file_range().
 * They are not available to the user via syscall.
 *
 * COPY_FILE_SPLICE: call splice direct instead of fs clone/copy ops
 */
#define COPY_FILE_SPLICE		(1 << 0)

struct iov_iter;
struct io_uring_cmd;
struct offset_ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
<<<<<<< HEAD
	ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	int (*readdir) (struct file *, void *, filldir_t);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
=======
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	int (*iopoll)(struct kiocb *kiocb, struct io_comp_batch *,
			unsigned int flags);
	int (*iterate_shared) (struct file *, struct dir_context *);
	__poll_t (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	unsigned long mmap_supported_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
<<<<<<< HEAD
	int (*aio_fsync) (struct kiocb *, int datasync);
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
=======
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
<<<<<<< HEAD
	int (*setlease)(struct file *, long, struct file_lock **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
	struct file* (*get_lower_file)(struct file *f);
};

struct inode_operations {
	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
	void * (*follow_link) (struct dentry *, struct nameidata *);
	int (*permission) (struct inode *, int);
	int (*permission2) (struct vfsmount *, struct inode *, int);
	struct posix_acl * (*get_acl)(struct inode *, int);

	int (*readlink) (struct dentry *, char __user *,int);
	void (*put_link) (struct dentry *, struct nameidata *, void *);

	int (*create) (struct inode *,struct dentry *, umode_t, bool);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct inode *,struct dentry *,const char *);
	int (*mkdir) (struct inode *,struct dentry *,umode_t);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);
	int (*rename) (struct inode *, struct dentry *,
			struct inode *, struct dentry *);
	void (*truncate) (struct inode *);
	int (*setattr) (struct dentry *, struct iattr *);
	int (*setattr2) (struct vfsmount *, struct dentry *, struct iattr *);
	int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
	int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);
	ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
	ssize_t (*listxattr) (struct dentry *, char *, size_t);
	int (*removexattr) (struct dentry *, const char *);
	void (*truncate_range)(struct inode *, loff_t, loff_t);
	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
		      u64 len);
	int (*update_time)(struct inode *, struct timespec *, int);
	int (*atomic_open)(struct inode *, struct dentry *,
			   struct file *, unsigned open_flag,
			   umode_t create_mode, int *opened);
	int (*tmpfile) (struct inode *, struct dentry *, umode_t);
} ____cacheline_aligned;

struct seq_file;

ssize_t rw_copy_check_uvector(int type, const struct iovec __user * uvector,
			      unsigned long nr_segs, unsigned long fast_segs,
			      struct iovec *fast_pointer,
			      struct iovec **ret_pointer,
			      int check_access);

extern ssize_t vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t vfs_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t vfs_readv(struct file *, const struct iovec __user *,
		unsigned long, loff_t *);
extern ssize_t vfs_writev(struct file *, const struct iovec __user *,
		unsigned long, loff_t *);
=======
	void (*splice_eof)(struct file *file);
	int (*setlease)(struct file *, int, struct file_lease **, void **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
	void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
	unsigned (*mmap_capabilities)(struct file *);
#endif
	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
			loff_t, size_t, unsigned int);
	loff_t (*remap_file_range)(struct file *file_in, loff_t pos_in,
				   struct file *file_out, loff_t pos_out,
				   loff_t len, unsigned int remap_flags);
	int (*fadvise)(struct file *, loff_t, loff_t, int);
	int (*uring_cmd)(struct io_uring_cmd *ioucmd, unsigned int issue_flags);
	int (*uring_cmd_iopoll)(struct io_uring_cmd *, struct io_comp_batch *,
				unsigned int poll_flags);
} __randomize_layout;

/* Wrap a directory iterator that needs exclusive inode access */
int wrap_directory_iterator(struct file *, struct dir_context *,
			    int (*) (struct file *, struct dir_context *));
#define WRAP_DIR_ITER(x) \
	static int shared_##x(struct file *file , struct dir_context *ctx) \
	{ return wrap_directory_iterator(file, ctx, x); }

struct inode_operations {
	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
	const char * (*get_link) (struct dentry *, struct inode *, struct delayed_call *);
	int (*permission) (struct mnt_idmap *, struct inode *, int);
	struct posix_acl * (*get_inode_acl)(struct inode *, int, bool);

	int (*readlink) (struct dentry *, char __user *,int);

	int (*create) (struct mnt_idmap *, struct inode *,struct dentry *,
		       umode_t, bool);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct mnt_idmap *, struct inode *,struct dentry *,
			const char *);
	int (*mkdir) (struct mnt_idmap *, struct inode *,struct dentry *,
		      umode_t);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct mnt_idmap *, struct inode *,struct dentry *,
		      umode_t,dev_t);
	int (*rename) (struct mnt_idmap *, struct inode *, struct dentry *,
			struct inode *, struct dentry *, unsigned int);
	int (*setattr) (struct mnt_idmap *, struct dentry *, struct iattr *);
	int (*getattr) (struct mnt_idmap *, const struct path *,
			struct kstat *, u32, unsigned int);
	ssize_t (*listxattr) (struct dentry *, char *, size_t);
	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
		      u64 len);
	int (*update_time)(struct inode *, int);
	int (*atomic_open)(struct inode *, struct dentry *,
			   struct file *, unsigned open_flag,
			   umode_t create_mode);
	int (*tmpfile) (struct mnt_idmap *, struct inode *,
			struct file *, umode_t);
	struct posix_acl *(*get_acl)(struct mnt_idmap *, struct dentry *,
				     int);
	int (*set_acl)(struct mnt_idmap *, struct dentry *,
		       struct posix_acl *, int);
	int (*fileattr_set)(struct mnt_idmap *idmap,
			    struct dentry *dentry, struct fileattr *fa);
	int (*fileattr_get)(struct dentry *dentry, struct fileattr *fa);
	struct offset_ctx *(*get_offset_ctx)(struct inode *inode);
} ____cacheline_aligned;

static inline ssize_t call_read_iter(struct file *file, struct kiocb *kio,
				     struct iov_iter *iter)
{
	return file->f_op->read_iter(kio, iter);
}

static inline ssize_t call_write_iter(struct file *file, struct kiocb *kio,
				      struct iov_iter *iter)
{
	return file->f_op->write_iter(kio, iter);
}

static inline int call_mmap(struct file *file, struct vm_area_struct *vma)
{
	return file->f_op->mmap(file, vma);
}

extern ssize_t vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t vfs_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t vfs_copy_file_range(struct file *, loff_t , struct file *,
				   loff_t, size_t, unsigned int);
int __generic_remap_file_range_prep(struct file *file_in, loff_t pos_in,
				    struct file *file_out, loff_t pos_out,
				    loff_t *len, unsigned int remap_flags,
				    const struct iomap_ops *dax_read_ops);
int generic_remap_file_range_prep(struct file *file_in, loff_t pos_in,
				  struct file *file_out, loff_t pos_out,
				  loff_t *count, unsigned int remap_flags);
extern loff_t vfs_clone_file_range(struct file *file_in, loff_t pos_in,
				   struct file *file_out, loff_t pos_out,
				   loff_t len, unsigned int remap_flags);
extern int vfs_dedupe_file_range(struct file *file,
				 struct file_dedupe_range *same);
extern loff_t vfs_dedupe_file_range_one(struct file *src_file, loff_t src_pos,
					struct file *dst_file, loff_t dst_pos,
					loff_t len, unsigned int remap_flags);

/**
 * enum freeze_holder - holder of the freeze
 * @FREEZE_HOLDER_KERNEL: kernel wants to freeze or thaw filesystem
 * @FREEZE_HOLDER_USERSPACE: userspace wants to freeze or thaw filesystem
 * @FREEZE_MAY_NEST: whether nesting freeze and thaw requests is allowed
 *
 * Indicate who the owner of the freeze or thaw request is and whether
 * the freeze needs to be exclusive or can nest.
 * Without @FREEZE_MAY_NEST, multiple freeze and thaw requests from the
 * same holder aren't allowed. It is however allowed to hold a single
 * @FREEZE_HOLDER_USERSPACE and a single @FREEZE_HOLDER_KERNEL freeze at
 * the same time. This is relied upon by some filesystems during online
 * repair or similar.
 */
enum freeze_holder {
	FREEZE_HOLDER_KERNEL	= (1U << 0),
	FREEZE_HOLDER_USERSPACE	= (1U << 1),
	FREEZE_MAY_NEST		= (1U << 2),
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct super_operations {
   	struct inode *(*alloc_inode)(struct super_block *sb);
	void (*destroy_inode)(struct inode *);
<<<<<<< HEAD
=======
	void (*free_inode)(struct inode *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

   	void (*dirty_inode) (struct inode *, int flags);
	int (*write_inode) (struct inode *, struct writeback_control *wbc);
	int (*drop_inode) (struct inode *);
	void (*evict_inode) (struct inode *);
	void (*put_super) (struct super_block *);
<<<<<<< HEAD
	void (*write_super) (struct super_block *);
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_fs) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	int (*remount_fs2) (struct vfsmount *, struct super_block *, int *, char *);
	void *(*clone_mnt_data) (void *);
	void (*copy_mnt_data) (void *, void *);
	void (*umount_begin) (struct super_block *);

	int (*show_options)(struct seq_file *, struct dentry *);
	int (*show_options2)(struct vfsmount *,struct seq_file *, struct dentry *);
=======
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_super) (struct super_block *, enum freeze_holder who);
	int (*freeze_fs) (struct super_block *);
	int (*thaw_super) (struct super_block *, enum freeze_holder who);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*umount_begin) (struct super_block *);

	int (*show_options)(struct seq_file *, struct dentry *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int (*show_devname)(struct seq_file *, struct dentry *);
	int (*show_path)(struct seq_file *, struct dentry *);
	int (*show_stats)(struct seq_file *, struct dentry *);
#ifdef CONFIG_QUOTA
	ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
	ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
<<<<<<< HEAD
#endif
	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
	int (*nr_cached_objects)(struct super_block *);
	void (*free_cached_objects)(struct super_block *, int);
};

/*
 * Inode state bits.  Protected by inode->i_lock
 *
 * Three bits determine the dirty state of the inode, I_DIRTY_SYNC,
 * I_DIRTY_DATASYNC and I_DIRTY_PAGES.
=======
	struct dquot __rcu **(*get_dquots)(struct inode *);
#endif
	long (*nr_cached_objects)(struct super_block *,
				  struct shrink_control *);
	long (*free_cached_objects)(struct super_block *,
				    struct shrink_control *);
	void (*shutdown)(struct super_block *sb);
};

/*
 * Inode flags - they have no relation to superblock flags now
 */
#define S_SYNC		(1 << 0)  /* Writes are synced at once */
#define S_NOATIME	(1 << 1)  /* Do not update access times */
#define S_APPEND	(1 << 2)  /* Append-only file */
#define S_IMMUTABLE	(1 << 3)  /* Immutable file */
#define S_DEAD		(1 << 4)  /* removed, but still open directory */
#define S_NOQUOTA	(1 << 5)  /* Inode is not counted to quota */
#define S_DIRSYNC	(1 << 6)  /* Directory modifications are synchronous */
#define S_NOCMTIME	(1 << 7)  /* Do not update file c/mtime */
#define S_SWAPFILE	(1 << 8)  /* Do not truncate: swapon got its bmaps */
#define S_PRIVATE	(1 << 9)  /* Inode is fs-internal */
#define S_IMA		(1 << 10) /* Inode has an associated IMA struct */
#define S_AUTOMOUNT	(1 << 11) /* Automount/referral quasi-directory */
#define S_NOSEC		(1 << 12) /* no suid or xattr security attributes */
#ifdef CONFIG_FS_DAX
#define S_DAX		(1 << 13) /* Direct Access, avoiding the page cache */
#else
#define S_DAX		0	  /* Make all the DAX code disappear */
#endif
#define S_ENCRYPTED	(1 << 14) /* Encrypted file (using fs/crypto/) */
#define S_CASEFOLD	(1 << 15) /* Casefolded file */
#define S_VERITY	(1 << 16) /* Verity file (using fs/verity/) */
#define S_KERNEL_FILE	(1 << 17) /* File is in use by the kernel (eg. fs/cachefiles) */

/*
 * Note that nosuid etc flags are inode-specific: setting some file-system
 * flags just means all the inodes inherit those flags by default. It might be
 * possible to override it selectively if you really wanted to with some
 * ioctl() that is not currently implemented.
 *
 * Exception: SB_RDONLY is always applied to the entire file system.
 *
 * Unfortunately, it is possible to change a filesystems flags with it mounted
 * with files in use.  This means that all of the inodes will not have their
 * i_flags updated.  Hence, i_flags no longer inherit the superblock mount
 * flags, so these have to be checked separately. -- rmk@arm.uk.linux.org
 */
#define __IS_FLG(inode, flg)	((inode)->i_sb->s_flags & (flg))

static inline bool sb_rdonly(const struct super_block *sb) { return sb->s_flags & SB_RDONLY; }
#define IS_RDONLY(inode)	sb_rdonly((inode)->i_sb)
#define IS_SYNC(inode)		(__IS_FLG(inode, SB_SYNCHRONOUS) || \
					((inode)->i_flags & S_SYNC))
#define IS_DIRSYNC(inode)	(__IS_FLG(inode, SB_SYNCHRONOUS|SB_DIRSYNC) || \
					((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
#define IS_MANDLOCK(inode)	__IS_FLG(inode, SB_MANDLOCK)
#define IS_NOATIME(inode)	__IS_FLG(inode, SB_RDONLY|SB_NOATIME)
#define IS_I_VERSION(inode)	__IS_FLG(inode, SB_I_VERSION)

#define IS_NOQUOTA(inode)	((inode)->i_flags & S_NOQUOTA)
#define IS_APPEND(inode)	((inode)->i_flags & S_APPEND)
#define IS_IMMUTABLE(inode)	((inode)->i_flags & S_IMMUTABLE)

#ifdef CONFIG_FS_POSIX_ACL
#define IS_POSIXACL(inode)	__IS_FLG(inode, SB_POSIXACL)
#else
#define IS_POSIXACL(inode)	0
#endif

#define IS_DEADDIR(inode)	((inode)->i_flags & S_DEAD)
#define IS_NOCMTIME(inode)	((inode)->i_flags & S_NOCMTIME)
#define IS_SWAPFILE(inode)	((inode)->i_flags & S_SWAPFILE)
#define IS_PRIVATE(inode)	((inode)->i_flags & S_PRIVATE)
#define IS_IMA(inode)		((inode)->i_flags & S_IMA)
#define IS_AUTOMOUNT(inode)	((inode)->i_flags & S_AUTOMOUNT)
#define IS_NOSEC(inode)		((inode)->i_flags & S_NOSEC)
#define IS_DAX(inode)		((inode)->i_flags & S_DAX)
#define IS_ENCRYPTED(inode)	((inode)->i_flags & S_ENCRYPTED)
#define IS_CASEFOLDED(inode)	((inode)->i_flags & S_CASEFOLD)
#define IS_VERITY(inode)	((inode)->i_flags & S_VERITY)

#define IS_WHITEOUT(inode)	(S_ISCHR(inode->i_mode) && \
				 (inode)->i_rdev == WHITEOUT_DEV)

static inline bool HAS_UNMAPPED_ID(struct mnt_idmap *idmap,
				   struct inode *inode)
{
	return !vfsuid_valid(i_uid_into_vfsuid(idmap, inode)) ||
	       !vfsgid_valid(i_gid_into_vfsgid(idmap, inode));
}

static inline void init_sync_kiocb(struct kiocb *kiocb, struct file *filp)
{
	*kiocb = (struct kiocb) {
		.ki_filp = filp,
		.ki_flags = filp->f_iocb_flags,
		.ki_ioprio = get_current_ioprio(),
	};
}

static inline void kiocb_clone(struct kiocb *kiocb, struct kiocb *kiocb_src,
			       struct file *filp)
{
	*kiocb = (struct kiocb) {
		.ki_filp = filp,
		.ki_flags = kiocb_src->ki_flags,
		.ki_ioprio = kiocb_src->ki_ioprio,
		.ki_pos = kiocb_src->ki_pos,
	};
}

/*
 * Inode state bits.  Protected by inode->i_lock
 *
 * Four bits determine the dirty state of the inode: I_DIRTY_SYNC,
 * I_DIRTY_DATASYNC, I_DIRTY_PAGES, and I_DIRTY_TIME.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Four bits define the lifetime of an inode.  Initially, inodes are I_NEW,
 * until that flag is cleared.  I_WILL_FREE, I_FREEING and I_CLEAR are set at
 * various stages of removing an inode.
 *
 * Two bits are used for locking and completion notification, I_NEW and I_SYNC.
 *
 * I_DIRTY_SYNC		Inode is dirty, but doesn't have to be written on
<<<<<<< HEAD
 *			fdatasync().  i_atime is the usual cause.
 * I_DIRTY_DATASYNC	Data-related inode changes pending. We keep track of
 *			these changes separately from I_DIRTY_SYNC so that we
 *			don't have to write inode on fdatasync() when only
 *			mtime has changed in it.
 * I_DIRTY_PAGES	Inode has dirty pages.  Inode itself may be clean.
=======
 *			fdatasync() (unless I_DIRTY_DATASYNC is also set).
 *			Timestamp updates are the usual cause.
 * I_DIRTY_DATASYNC	Data-related inode changes pending.  We keep track of
 *			these changes separately from I_DIRTY_SYNC so that we
 *			don't have to write inode on fdatasync() when only
 *			e.g. the timestamps have changed.
 * I_DIRTY_PAGES	Inode has dirty pages.  Inode itself may be clean.
 * I_DIRTY_TIME		The inode itself has dirty timestamps, and the
 *			lazytime mount option is enabled.  We keep track of this
 *			separately from I_DIRTY_SYNC in order to implement
 *			lazytime.  This gets cleared if I_DIRTY_INODE
 *			(I_DIRTY_SYNC and/or I_DIRTY_DATASYNC) gets set. But
 *			I_DIRTY_TIME can still be set if I_DIRTY_SYNC is already
 *			in place because writeback might already be in progress
 *			and we don't want to lose the time update
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * I_NEW		Serves as both a mutex and completion notification.
 *			New inodes set I_NEW.  If two processes both create
 *			the same inode, one of them will release its inode and
 *			wait for I_NEW to be released before returning.
 *			Inodes in I_WILL_FREE, I_FREEING or I_CLEAR state can
 *			also cause waiting on I_NEW, without I_NEW actually
 *			being set.  find_inode() uses this to prevent returning
 *			nearly-dead inodes.
 * I_WILL_FREE		Must be set when calling write_inode_now() if i_count
 *			is zero.  I_FREEING must be set when I_WILL_FREE is
 *			cleared.
 * I_FREEING		Set when inode is about to be freed but still has dirty
 *			pages or buffers attached or the inode itself is still
 *			dirty.
<<<<<<< HEAD
 * I_CLEAR		Added by end_writeback().  In this state the inode is clean
 *			and can be destroyed.  Inode keeps I_FREEING.
=======
 * I_CLEAR		Added by clear_inode().  In this state the inode is
 *			clean and can be destroyed.  Inode keeps I_FREEING.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *			Inodes that are I_WILL_FREE, I_FREEING or I_CLEAR are
 *			prohibited for many purposes.  iget() must wait for
 *			the inode to be completely released, then create it
 *			anew.  Other functions will just ignore such inodes,
 *			if appropriate.  I_NEW is used for waiting.
 *
<<<<<<< HEAD
 * I_SYNC		Synchonized write of dirty inode data.  The bits is
 *			set during data writeback, and cleared with a wakeup
 *			on the bit address once it is done.
=======
 * I_SYNC		Writeback of inode is running. The bit is set during
 *			data writeback, and cleared with a wakeup on the bit
 *			address once it is done. The bit is also used to pin
 *			the inode in memory for flusher thread.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * I_REFERENCED		Marks the inode as recently references on the LRU list.
 *
 * I_DIO_WAKEUP		Never set.  Only used as a key for wait_on_bit().
 *
<<<<<<< HEAD
=======
 * I_WB_SWITCH		Cgroup bdi_writeback switching in progress.  Used to
 *			synchronize competing switching instances and to tell
 *			wb stat updates to grab the i_pages lock.  See
 *			inode_switch_wbs_work_fn() for details.
 *
 * I_OVL_INUSE		Used by overlayfs to get exclusive ownership on upper
 *			and work dirs among overlayfs mounts.
 *
 * I_CREATING		New object's inode in the middle of setting up.
 *
 * I_DONTCACHE		Evict inode as soon as it is not used anymore.
 *
 * I_SYNC_QUEUED	Inode is queued in b_io or b_more_io writeback lists.
 *			Used to detect that mark_inode_dirty() should not move
 * 			inode between dirty lists.
 *
 * I_PINNING_FSCACHE_WB	Inode is pinning an fscache object for writeback.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Q: What is the difference between I_WILL_FREE and I_FREEING?
 */
#define I_DIRTY_SYNC		(1 << 0)
#define I_DIRTY_DATASYNC	(1 << 1)
#define I_DIRTY_PAGES		(1 << 2)
#define __I_NEW			3
#define I_NEW			(1 << __I_NEW)
#define I_WILL_FREE		(1 << 4)
#define I_FREEING		(1 << 5)
#define I_CLEAR			(1 << 6)
#define __I_SYNC		7
#define I_SYNC			(1 << __I_SYNC)
#define I_REFERENCED		(1 << 8)
#define __I_DIO_WAKEUP		9
<<<<<<< HEAD
#define I_DIO_WAKEUP		(1 << I_DIO_WAKEUP)
#define I_LINKABLE		(1 << 10)

#define I_DIRTY (I_DIRTY_SYNC | I_DIRTY_DATASYNC | I_DIRTY_PAGES)
=======
#define I_DIO_WAKEUP		(1 << __I_DIO_WAKEUP)
#define I_LINKABLE		(1 << 10)
#define I_DIRTY_TIME		(1 << 11)
#define I_WB_SWITCH		(1 << 13)
#define I_OVL_INUSE		(1 << 14)
#define I_CREATING		(1 << 15)
#define I_DONTCACHE		(1 << 16)
#define I_SYNC_QUEUED		(1 << 17)
#define I_PINNING_NETFS_WB	(1 << 18)

#define I_DIRTY_INODE (I_DIRTY_SYNC | I_DIRTY_DATASYNC)
#define I_DIRTY (I_DIRTY_INODE | I_DIRTY_PAGES)
#define I_DIRTY_ALL (I_DIRTY | I_DIRTY_TIME)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void __mark_inode_dirty(struct inode *, int);
static inline void mark_inode_dirty(struct inode *inode)
{
	__mark_inode_dirty(inode, I_DIRTY);
}

static inline void mark_inode_dirty_sync(struct inode *inode)
{
	__mark_inode_dirty(inode, I_DIRTY_SYNC);
}

<<<<<<< HEAD
=======
/*
 * Returns true if the given inode itself only has dirty timestamps (its pages
 * may still be dirty) and isn't currently being allocated or freed.
 * Filesystems should call this if when writing an inode when lazytime is
 * enabled, they want to opportunistically write the timestamps of other inodes
 * located very nearby on-disk, e.g. in the same inode block.  This returns true
 * if the given inode is in need of such an opportunistic update.  Requires
 * i_lock, or at least later re-checking under i_lock.
 */
static inline bool inode_is_dirtytime_only(struct inode *inode)
{
	return (inode->i_state & (I_DIRTY_TIME | I_NEW |
				  I_FREEING | I_WILL_FREE)) == I_DIRTY_TIME;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void inc_nlink(struct inode *inode);
extern void drop_nlink(struct inode *inode);
extern void clear_nlink(struct inode *inode);
extern void set_nlink(struct inode *inode, unsigned int nlink);

static inline void inode_inc_link_count(struct inode *inode)
{
	inc_nlink(inode);
	mark_inode_dirty(inode);
}

static inline void inode_dec_link_count(struct inode *inode)
{
	drop_nlink(inode);
	mark_inode_dirty(inode);
}

<<<<<<< HEAD
/**
 * inode_inc_iversion - increments i_version
 * @inode: inode that need to be updated
 *
 * Every time the inode is modified, the i_version field will be incremented.
 * The filesystem has to be mounted with i_version flag
 */

static inline void inode_inc_iversion(struct inode *inode)
{
       spin_lock(&inode->i_lock);
       inode->i_version++;
       spin_unlock(&inode->i_lock);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum file_time_flags {
	S_ATIME = 1,
	S_MTIME = 2,
	S_CTIME = 4,
	S_VERSION = 8,
};

<<<<<<< HEAD
extern void touch_atime(struct path *);
=======
extern bool atime_needs_update(const struct path *, struct inode *);
extern void touch_atime(const struct path *);
int inode_update_time(struct inode *inode, int flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void file_accessed(struct file *file)
{
	if (!(file->f_flags & O_NOATIME))
		touch_atime(&file->f_path);
}

<<<<<<< HEAD
int sync_inode(struct inode *inode, struct writeback_control *wbc);
=======
extern int file_modified(struct file *file);
int kiocb_modified(struct kiocb *iocb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int sync_inode_metadata(struct inode *inode, int wait);

struct file_system_type {
	const char *name;
	int fs_flags;
#define FS_REQUIRES_DEV		1 
#define FS_BINARY_MOUNTDATA	2
#define FS_HAS_SUBTYPE		4
#define FS_USERNS_MOUNT		8	/* Can be mounted by userns root */
<<<<<<< HEAD
#define FS_USERNS_DEV_MOUNT	16 /* A userns mount does not imply MNT_NODEV */
#define FS_REVAL_DOT		16384	/* Check the paths ".", ".." for staleness */
#define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move() during rename() internally. */
	struct dentry *(*mount) (struct file_system_type *, int,
		       const char *, void *);
	struct dentry *(*mount2) (struct vfsmount *, struct file_system_type *, int,
			       const char *, void *);
	void *(*alloc_mnt_data) (void);
=======
#define FS_DISALLOW_NOTIFY_PERM	16	/* Disable fanotify permission events */
#define FS_ALLOW_IDMAP         32      /* FS has been updated to handle vfs idmappings. */
#define FS_RENAME_DOES_D_MOVE	32768	/* FS will handle d_move() during rename() internally. */
	int (*init_fs_context)(struct fs_context *);
	const struct fs_parameter_spec *parameters;
	struct dentry *(*mount) (struct file_system_type *, int,
		       const char *, void *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void (*kill_sb) (struct super_block *);
	struct module *owner;
	struct file_system_type * next;
	struct hlist_head fs_supers;

	struct lock_class_key s_lock_key;
	struct lock_class_key s_umount_key;
	struct lock_class_key s_vfs_rename_key;
<<<<<<< HEAD

	struct lock_class_key i_lock_key;
	struct lock_class_key i_mutex_key;
=======
	struct lock_class_key s_writers_key[SB_FREEZE_LEVELS];

	struct lock_class_key i_lock_key;
	struct lock_class_key i_mutex_key;
	struct lock_class_key invalidate_lock_key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct lock_class_key i_mutex_dir_key;
};

#define MODULE_ALIAS_FS(NAME) MODULE_ALIAS("fs-" NAME)

<<<<<<< HEAD
extern struct dentry *mount_ns(struct file_system_type *fs_type, int flags,
	void *data, int (*fill_super)(struct super_block *, void *, int));
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_single(struct file_system_type *fs_type,
	int flags, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_nodev(struct file_system_type *fs_type,
	int flags, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_subtree(struct vfsmount *mnt, const char *path);
<<<<<<< HEAD
=======
void retire_super(struct super_block *sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void generic_shutdown_super(struct super_block *sb);
void kill_block_super(struct super_block *sb);
void kill_anon_super(struct super_block *sb);
void kill_litter_super(struct super_block *sb);
void deactivate_super(struct super_block *sb);
void deactivate_locked_super(struct super_block *sb);
int set_anon_super(struct super_block *s, void *data);
<<<<<<< HEAD
int get_anon_bdev(dev_t *);
void free_anon_bdev(dev_t);
struct super_block *sget(struct file_system_type *type,
			int (*test)(struct super_block *,void *),
			int (*set)(struct super_block *,void *),
			void *data);
extern struct dentry *mount_pseudo(struct file_system_type *, char *,
	const struct super_operations *ops,
	const struct dentry_operations *dops,
	unsigned long);
=======
int set_anon_super_fc(struct super_block *s, struct fs_context *fc);
int get_anon_bdev(dev_t *);
void free_anon_bdev(dev_t);
struct super_block *sget_fc(struct fs_context *fc,
			    int (*test)(struct super_block *, struct fs_context *),
			    int (*set)(struct super_block *, struct fs_context *));
struct super_block *sget(struct file_system_type *type,
			int (*test)(struct super_block *,void *),
			int (*set)(struct super_block *,void *),
			int flags, void *data);
struct super_block *sget_dev(struct fs_context *fc, dev_t dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Alas, no aliases. Too much hassle with bringing module.h everywhere */
#define fops_get(fops) \
	(((fops) && try_module_get((fops)->owner) ? (fops) : NULL))
#define fops_put(fops) \
	do { if (fops) module_put((fops)->owner); } while(0)
<<<<<<< HEAD

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);
extern struct vfsmount *kern_mount_data(struct file_system_type *, void *data);
#define kern_mount(type) kern_mount_data(type, NULL)
extern void kern_unmount(struct vfsmount *mnt);
extern int may_umount_tree(struct vfsmount *);
extern int may_umount(struct vfsmount *);
extern long do_mount(const char *, const char *, const char *, unsigned long, void *);
extern struct vfsmount *collect_mounts(struct path *);
extern void drop_collected_mounts(struct vfsmount *);
extern int iterate_mounts(int (*)(struct vfsmount *, void *), void *,
			  struct vfsmount *);
extern int vfs_statfs(struct path *, struct kstatfs *);
extern int user_statfs(const char __user *, struct kstatfs *);
extern int fd_statfs(int, struct kstatfs *);
extern int vfs_ustat(dev_t, struct kstatfs *);
extern int freeze_super(struct super_block *super);
extern int thaw_super(struct super_block *super);
extern bool our_mnt(struct vfsmount *mnt);

extern int current_umask(void);

/* /sys/fs */
extern struct kobject *fs_kobj;

#define MAX_RW_COUNT (INT_MAX & PAGE_CACHE_MASK)
extern int rw_verify_area(int, struct file *, loff_t *, size_t);

#define FLOCK_VERIFY_READ  1
#define FLOCK_VERIFY_WRITE 2

#ifdef CONFIG_FILE_LOCKING
extern int locks_mandatory_locked(struct inode *);
extern int locks_mandatory_area(int, struct inode *, struct file *, loff_t, size_t);

/*
 * Candidates for mandatory locking have the setgid bit set
 * but no group execute bit -  an otherwise meaningless combination.
 */

static inline int __mandatory_lock(struct inode *ino)
{
	return (ino->i_mode & (S_ISGID | S_IXGRP)) == S_ISGID;
}

/*
 * ... and these candidates should be on MS_MANDLOCK mounted fs,
 * otherwise these will be advisory locks
 */

static inline int mandatory_lock(struct inode *ino)
{
	return IS_MANDLOCK(ino) && __mandatory_lock(ino);
}

static inline int locks_verify_locked(struct inode *inode)
{
	if (mandatory_lock(inode))
		return locks_mandatory_locked(inode);
	return 0;
}

static inline int locks_verify_truncate(struct inode *inode,
				    struct file *filp,
				    loff_t size)
{
	if (inode->i_flock && mandatory_lock(inode))
		return locks_mandatory_area(
			FLOCK_VERIFY_WRITE, inode, filp,
			size < inode->i_size ? size : inode->i_size,
			(size < inode->i_size ? inode->i_size - size
			 : size - inode->i_size)
		);
	return 0;
}

static inline int break_lease(struct inode *inode, unsigned int mode)
{
	if (inode->i_flock)
		return __break_lease(inode, mode);
	return 0;
}
#else /* !CONFIG_FILE_LOCKING */
static inline int locks_mandatory_locked(struct inode *inode)
{
	return 0;
}

static inline int locks_mandatory_area(int rw, struct inode *inode,
				       struct file *filp, loff_t offset,
				       size_t count)
{
	return 0;
}

static inline int __mandatory_lock(struct inode *inode)
{
	return 0;
}

static inline int mandatory_lock(struct inode *inode)
{
	return 0;
}

static inline int locks_verify_locked(struct inode *inode)
{
	return 0;
}

static inline int locks_verify_truncate(struct inode *inode, struct file *filp,
					size_t size)
{
	return 0;
}

static inline int break_lease(struct inode *inode, unsigned int mode)
{
	return 0;
}

#endif /* CONFIG_FILE_LOCKING */

/* fs/open.c */
struct filename {
	const char		*name;	/* pointer to actual string */
	const __user char	*uptr;	/* original userland pointer */
	bool			separate; /* should "name" be freed? */
};

extern int do_truncate(struct dentry *, loff_t start, unsigned int time_attrs,
		       struct file *filp);
extern int do_truncate2(struct vfsmount *, struct dentry *, loff_t start,
			unsigned int time_attrs, struct file *filp);
extern int do_fallocate(struct file *file, int mode, loff_t offset,
=======
/*
 * This one is to be used *ONLY* from ->open() instances.
 * fops must be non-NULL, pinned down *and* module dependencies
 * should be sufficient to pin the caller down as well.
 */
#define replace_fops(f, fops) \
	do {	\
		struct file *__file = (f); \
		fops_put(__file->f_op); \
		BUG_ON(!(__file->f_op = (fops))); \
	} while(0)

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);
extern int vfs_statfs(const struct path *, struct kstatfs *);
extern int user_statfs(const char __user *, struct kstatfs *);
extern int fd_statfs(int, struct kstatfs *);
int freeze_super(struct super_block *super, enum freeze_holder who);
int thaw_super(struct super_block *super, enum freeze_holder who);
extern __printf(2, 3)
int super_setup_bdi_name(struct super_block *sb, char *fmt, ...);
extern int super_setup_bdi(struct super_block *sb);

static inline void super_set_uuid(struct super_block *sb, const u8 *uuid, unsigned len)
{
	if (WARN_ON(len > sizeof(sb->s_uuid)))
		len = sizeof(sb->s_uuid);
	sb->s_uuid_len = len;
	memcpy(&sb->s_uuid, uuid, len);
}

/* set sb sysfs name based on sb->s_bdev */
static inline void super_set_sysfs_name_bdev(struct super_block *sb)
{
	snprintf(sb->s_sysfs_name, sizeof(sb->s_sysfs_name), "%pg", sb->s_bdev);
}

/* set sb sysfs name based on sb->s_uuid */
static inline void super_set_sysfs_name_uuid(struct super_block *sb)
{
	WARN_ON(sb->s_uuid_len != sizeof(sb->s_uuid));
	snprintf(sb->s_sysfs_name, sizeof(sb->s_sysfs_name), "%pU", sb->s_uuid.b);
}

/* set sb sysfs name based on sb->s_id */
static inline void super_set_sysfs_name_id(struct super_block *sb)
{
	strscpy(sb->s_sysfs_name, sb->s_id, sizeof(sb->s_sysfs_name));
}

/* try to use something standard before you use this */
__printf(2, 3)
static inline void super_set_sysfs_name_generic(struct super_block *sb, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(sb->s_sysfs_name, sizeof(sb->s_sysfs_name), fmt, args);
	va_end(args);
}

extern int current_umask(void);

extern void ihold(struct inode * inode);
extern void iput(struct inode *);
int inode_update_timestamps(struct inode *inode, int flags);
int generic_update_time(struct inode *, int);

/* /sys/fs */
extern struct kobject *fs_kobj;

#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)

/* fs/open.c */
struct audit_names;
struct filename {
	const char		*name;	/* pointer to actual string */
	const __user char	*uptr;	/* original userland pointer */
	atomic_t		refcnt;
	struct audit_names	*aname;
	const char		iname[];
};
static_assert(offsetof(struct filename, iname) % sizeof(long) == 0);

static inline struct mnt_idmap *file_mnt_idmap(const struct file *file)
{
	return mnt_idmap(file->f_path.mnt);
}

/**
 * is_idmapped_mnt - check whether a mount is mapped
 * @mnt: the mount to check
 *
 * If @mnt has an non @nop_mnt_idmap attached to it then @mnt is mapped.
 *
 * Return: true if mount is mapped, false if not.
 */
static inline bool is_idmapped_mnt(const struct vfsmount *mnt)
{
	return mnt_idmap(mnt) != &nop_mnt_idmap;
}

extern long vfs_truncate(const struct path *, loff_t);
int do_truncate(struct mnt_idmap *, struct dentry *, loff_t start,
		unsigned int time_attrs, struct file *filp);
extern int vfs_fallocate(struct file *file, int mode, loff_t offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			loff_t len);
extern long do_sys_open(int dfd, const char __user *filename, int flags,
			umode_t mode);
extern struct file *file_open_name(struct filename *, int, umode_t);
extern struct file *filp_open(const char *, int, umode_t);
<<<<<<< HEAD
extern struct file *file_open_root(struct dentry *, struct vfsmount *,
				   const char *, int);
extern struct file * dentry_open(struct dentry *, struct vfsmount *, int,
				 const struct cred *);
extern int filp_close(struct file *, fl_owner_t id);

extern struct filename *getname(const char __user *);

enum {
	FILE_CREATED = 1,
	FILE_OPENED = 2
};
extern int finish_open(struct file *file, struct dentry *dentry,
			int (*open)(struct inode *, struct file *),
			int *opened);
extern int finish_no_open(struct file *file, struct dentry *dentry);

/* fs/ioctl.c */

extern int ioctl_preallocate(struct file *filp, void __user *argp);

/* fs/dcache.c */
extern void __init vfs_caches_init_early(void);
extern void __init vfs_caches_init(unsigned long);

extern struct kmem_cache *names_cachep;

extern void final_putname(struct filename *name);

#define __getname()		kmem_cache_alloc(names_cachep, GFP_KERNEL)
#define __putname(name)		kmem_cache_free(names_cachep, (void *)(name))
#ifndef CONFIG_AUDITSYSCALL
#define putname(name)		final_putname(name)
#else
extern void putname(struct filename *name);
#endif

#ifdef CONFIG_BLOCK
extern int register_blkdev(unsigned int, const char *);
extern void unregister_blkdev(unsigned int, const char *);
extern struct block_device *bdget(dev_t);
extern struct block_device *bdgrab(struct block_device *bdev);
extern void bd_set_size(struct block_device *, loff_t size);
extern sector_t blkdev_max_block(struct block_device *bdev);
extern void bd_forget(struct inode *inode);
extern void bdput(struct block_device *);
extern void invalidate_bdev(struct block_device *);
extern int sync_blockdev(struct block_device *bdev);
extern void kill_bdev(struct block_device *);
extern struct super_block *freeze_bdev(struct block_device *);
extern void emergency_thaw_all(void);
extern int thaw_bdev(struct block_device *bdev, struct super_block *sb);
extern int fsync_bdev(struct block_device *);
#else
static inline void bd_forget(struct inode *inode) {}
static inline int sync_blockdev(struct block_device *bdev) { return 0; }
static inline void kill_bdev(struct block_device *bdev) {}
static inline void invalidate_bdev(struct block_device *bdev) {}

static inline struct super_block *freeze_bdev(struct block_device *sb)
{
	return NULL;
}

static inline int thaw_bdev(struct block_device *bdev, struct super_block *sb)
{
	return 0;
}
#endif
extern int sync_filesystem(struct super_block *);
extern const struct file_operations def_blk_fops;
extern const struct file_operations def_chr_fops;
extern const struct file_operations bad_sock_fops;
extern const struct file_operations def_fifo_fops;
#ifdef CONFIG_BLOCK
extern int ioctl_by_bdev(struct block_device *, unsigned, unsigned long);
extern int blkdev_ioctl(struct block_device *, fmode_t, unsigned, unsigned long);
extern long compat_blkdev_ioctl(struct file *, unsigned, unsigned long);
extern int blkdev_get(struct block_device *bdev, fmode_t mode, void *holder);
extern struct block_device *blkdev_get_by_path(const char *path, fmode_t mode,
					       void *holder);
extern struct block_device *blkdev_get_by_dev(dev_t dev, fmode_t mode,
					      void *holder);
extern int blkdev_put(struct block_device *bdev, fmode_t mode);
#ifdef CONFIG_SYSFS
extern int bd_link_disk_holder(struct block_device *bdev, struct gendisk *disk);
extern void bd_unlink_disk_holder(struct block_device *bdev,
				  struct gendisk *disk);
#else
static inline int bd_link_disk_holder(struct block_device *bdev,
				      struct gendisk *disk)
{
	return 0;
}
static inline void bd_unlink_disk_holder(struct block_device *bdev,
					 struct gendisk *disk)
{
}
#endif
#endif

/* fs/char_dev.c */
#define CHRDEV_MAJOR_HASH_SIZE	255
=======
extern struct file *file_open_root(const struct path *,
				   const char *, int, umode_t);
static inline struct file *file_open_root_mnt(struct vfsmount *mnt,
				   const char *name, int flags, umode_t mode)
{
	return file_open_root(&(struct path){.mnt = mnt, .dentry = mnt->mnt_root},
			      name, flags, mode);
}
struct file *dentry_open(const struct path *path, int flags,
			 const struct cred *creds);
struct file *dentry_create(const struct path *path, int flags, umode_t mode,
			   const struct cred *cred);
struct path *backing_file_user_path(struct file *f);

/*
 * When mmapping a file on a stackable filesystem (e.g., overlayfs), the file
 * stored in ->vm_file is a backing file whose f_inode is on the underlying
 * filesystem.  When the mapped file path and inode number are displayed to
 * user (e.g. via /proc/<pid>/maps), these helpers should be used to get the
 * path and inode number to display to the user, which is the path of the fd
 * that user has requested to map and the inode number that would be returned
 * by fstat() on that same fd.
 */
/* Get the path to display in /proc/<pid>/maps */
static inline const struct path *file_user_path(struct file *f)
{
	if (unlikely(f->f_mode & FMODE_BACKING))
		return backing_file_user_path(f);
	return &f->f_path;
}
/* Get the inode whose inode number to display in /proc/<pid>/maps */
static inline const struct inode *file_user_inode(struct file *f)
{
	if (unlikely(f->f_mode & FMODE_BACKING))
		return d_inode(backing_file_user_path(f)->dentry);
	return file_inode(f);
}

static inline struct file *file_clone_open(struct file *file)
{
	return dentry_open(&file->f_path, file->f_flags, file->f_cred);
}
extern int filp_close(struct file *, fl_owner_t id);

extern struct filename *getname_flags(const char __user *, int, int *);
extern struct filename *getname_uflags(const char __user *, int);
extern struct filename *getname(const char __user *);
extern struct filename *getname_kernel(const char *);
extern void putname(struct filename *name);

extern int finish_open(struct file *file, struct dentry *dentry,
			int (*open)(struct inode *, struct file *));
extern int finish_no_open(struct file *file, struct dentry *dentry);

/* Helper for the simple case when original dentry is used */
static inline int finish_open_simple(struct file *file, int error)
{
	if (error)
		return error;

	return finish_open(file, file->f_path.dentry, NULL);
}

/* fs/dcache.c */
extern void __init vfs_caches_init_early(void);
extern void __init vfs_caches_init(void);

extern struct kmem_cache *names_cachep;

#define __getname()		kmem_cache_alloc(names_cachep, GFP_KERNEL)
#define __putname(name)		kmem_cache_free(names_cachep, (void *)(name))

extern struct super_block *blockdev_superblock;
static inline bool sb_is_blkdev_sb(struct super_block *sb)
{
	return IS_ENABLED(CONFIG_BLOCK) && sb == blockdev_superblock;
}

void emergency_thaw_all(void);
extern int sync_filesystem(struct super_block *);
extern const struct file_operations def_blk_fops;
extern const struct file_operations def_chr_fops;

/* fs/char_dev.c */
#define CHRDEV_MAJOR_MAX 512
/* Marks the bottom of the first segment of free char majors */
#define CHRDEV_MAJOR_DYN_END 234
/* Marks the top and bottom of the second segment of free char majors */
#define CHRDEV_MAJOR_DYN_EXT_START 511
#define CHRDEV_MAJOR_DYN_EXT_END 384

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);
extern int register_chrdev_region(dev_t, unsigned, const char *);
extern int __register_chrdev(unsigned int major, unsigned int baseminor,
			     unsigned int count, const char *name,
			     const struct file_operations *fops);
extern void __unregister_chrdev(unsigned int major, unsigned int baseminor,
				unsigned int count, const char *name);
extern void unregister_chrdev_region(dev_t, unsigned);
extern void chrdev_show(struct seq_file *,off_t);

static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops)
{
	return __register_chrdev(major, 0, 256, name, fops);
}

static inline void unregister_chrdev(unsigned int major, const char *name)
{
	__unregister_chrdev(major, 0, 256, name);
}

<<<<<<< HEAD
/* fs/block_dev.c */
#define BDEVNAME_SIZE	32	/* Largest string for a blockdev identifier */
#define BDEVT_SIZE	10	/* Largest string for MAJ:MIN for blkdev */

#ifdef CONFIG_BLOCK
#define BLKDEV_MAJOR_HASH_SIZE	255
extern const char *__bdevname(dev_t, char *buffer);
extern const char *bdevname(struct block_device *bdev, char *buffer);
extern struct block_device *lookup_bdev(const char *);
extern void blkdev_show(struct seq_file *,off_t);

#else
#define BLKDEV_MAJOR_HASH_SIZE	0
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void init_special_inode(struct inode *, umode_t, dev_t);

/* Invalid inode operations -- fs/bad_inode.c */
extern void make_bad_inode(struct inode *);
<<<<<<< HEAD
extern int is_bad_inode(struct inode *);

extern const struct file_operations read_pipefifo_fops;
extern const struct file_operations write_pipefifo_fops;
extern const struct file_operations rdwr_pipefifo_fops;

#ifdef CONFIG_BLOCK
/*
 * return READ, READA, or WRITE
 */
#define bio_rw(bio)		((bio)->bi_rw & (RW_MASK | RWA_MASK))

/*
 * return data direction, READ or WRITE
 */
#define bio_data_dir(bio)	((bio)->bi_rw & 1)

extern void check_disk_size_change(struct gendisk *disk,
				   struct block_device *bdev);
extern int revalidate_disk(struct gendisk *);
extern int check_disk_change(struct block_device *);
extern int __invalidate_device(struct block_device *, bool);
extern int invalidate_partition(struct gendisk *, int);
#endif
unsigned long invalidate_mapping_pages(struct address_space *mapping,
					pgoff_t start, pgoff_t end);

static inline void invalidate_remote_inode(struct inode *inode)
{
	if (S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
	    S_ISLNK(inode->i_mode))
		invalidate_mapping_pages(inode->i_mapping, 0, -1);
}
extern int invalidate_inode_pages2(struct address_space *mapping);
extern int invalidate_inode_pages2_range(struct address_space *mapping,
					 pgoff_t start, pgoff_t end);
extern int write_inode_now(struct inode *, int);
extern int filemap_fdatawrite(struct address_space *);
extern int filemap_flush(struct address_space *);
extern int filemap_fdatawait(struct address_space *);
extern int filemap_fdatawait_range(struct address_space *, loff_t lstart,
				   loff_t lend);
extern int filemap_write_and_wait(struct address_space *mapping);
extern int filemap_write_and_wait_range(struct address_space *mapping,
				        loff_t lstart, loff_t lend);
extern int __filemap_fdatawrite_range(struct address_space *mapping,
				loff_t start, loff_t end, int sync_mode);
extern int filemap_fdatawrite_range(struct address_space *mapping,
				loff_t start, loff_t end);
=======
extern bool is_bad_inode(struct inode *);

extern int __must_check file_fdatawait_range(struct file *file, loff_t lstart,
						loff_t lend);
extern int __must_check file_check_and_advance_wb_err(struct file *file);
extern int __must_check file_write_and_wait_range(struct file *file,
						loff_t start, loff_t end);

static inline int file_write_and_wait(struct file *file)
{
	return file_write_and_wait_range(file, 0, LLONG_MAX);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int vfs_fsync_range(struct file *file, loff_t start, loff_t end,
			   int datasync);
extern int vfs_fsync(struct file *file, int datasync);
<<<<<<< HEAD
extern int generic_write_sync(struct file *file, loff_t pos, loff_t count);
extern void sync_supers(void);
extern void emergency_sync(void);
extern void emergency_remount(void);
#ifdef CONFIG_BLOCK
extern sector_t bmap(struct inode *, sector_t);
#endif
extern int notify_change(struct dentry *, struct iattr *);
extern int notify_change2(struct vfsmount *, struct dentry *, struct iattr *);
extern int inode_permission(struct inode *, int);
extern int inode_permission2(struct vfsmount *, struct inode *, int);
extern int generic_permission(struct inode *, int);
=======

extern int sync_file_range(struct file *file, loff_t offset, loff_t nbytes,
				unsigned int flags);

static inline bool iocb_is_dsync(const struct kiocb *iocb)
{
	return (iocb->ki_flags & IOCB_DSYNC) ||
		IS_SYNC(iocb->ki_filp->f_mapping->host);
}

/*
 * Sync the bytes written if this was a synchronous write.  Expect ki_pos
 * to already be updated for the write, and will return either the amount
 * of bytes passed in, or an error if syncing the file failed.
 */
static inline ssize_t generic_write_sync(struct kiocb *iocb, ssize_t count)
{
	if (iocb_is_dsync(iocb)) {
		int ret = vfs_fsync_range(iocb->ki_filp,
				iocb->ki_pos - count, iocb->ki_pos - 1,
				(iocb->ki_flags & IOCB_SYNC) ? 0 : 1);
		if (ret)
			return ret;
	}

	return count;
}

extern void emergency_sync(void);
extern void emergency_remount(void);

#ifdef CONFIG_BLOCK
extern int bmap(struct inode *inode, sector_t *block);
#else
static inline int bmap(struct inode *inode,  sector_t *block)
{
	return -EINVAL;
}
#endif

int notify_change(struct mnt_idmap *, struct dentry *,
		  struct iattr *, struct inode **);
int inode_permission(struct mnt_idmap *, struct inode *, int);
int generic_permission(struct mnt_idmap *, struct inode *, int);
static inline int file_permission(struct file *file, int mask)
{
	return inode_permission(file_mnt_idmap(file),
				file_inode(file), mask);
}
static inline int path_permission(const struct path *path, int mask)
{
	return inode_permission(mnt_idmap(path->mnt),
				d_inode(path->dentry), mask);
}
int __check_sticky(struct mnt_idmap *idmap, struct inode *dir,
		   struct inode *inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline bool execute_ok(struct inode *inode)
{
	return (inode->i_mode & S_IXUGO) || S_ISDIR(inode->i_mode);
}

<<<<<<< HEAD
/*
 * get_write_access() gets write permission for a file.
 * put_write_access() releases this write permission.
 * This is used for regular files.
 * We cannot support write (and maybe mmap read-write shared) accesses and
 * MAP_DENYWRITE mmappings simultaneously. The i_writecount field of an inode
 * can have the following values:
 * 0: no writers, no VM_DENYWRITE mappings
 * < 0: (-i_writecount) vm_area_structs with VM_DENYWRITE set exist
 * > 0: (i_writecount) users are writing to the file.
=======
static inline bool inode_wrong_type(const struct inode *inode, umode_t mode)
{
	return (inode->i_mode ^ mode) & S_IFMT;
}

/**
 * file_start_write - get write access to a superblock for regular file io
 * @file: the file we want to write to
 *
 * This is a variant of sb_start_write() which is a noop on non-regualr file.
 * Should be matched with a call to file_end_write().
 */
static inline void file_start_write(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return;
	sb_start_write(file_inode(file)->i_sb);
}

static inline bool file_start_write_trylock(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return true;
	return sb_start_write_trylock(file_inode(file)->i_sb);
}

/**
 * file_end_write - drop write access to a superblock of a regular file
 * @file: the file we wrote to
 *
 * Should be matched with a call to file_start_write().
 */
static inline void file_end_write(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return;
	sb_end_write(file_inode(file)->i_sb);
}

/**
 * kiocb_start_write - get write access to a superblock for async file io
 * @iocb: the io context we want to submit the write with
 *
 * This is a variant of sb_start_write() for async io submission.
 * Should be matched with a call to kiocb_end_write().
 */
static inline void kiocb_start_write(struct kiocb *iocb)
{
	struct inode *inode = file_inode(iocb->ki_filp);

	sb_start_write(inode->i_sb);
	/*
	 * Fool lockdep by telling it the lock got released so that it
	 * doesn't complain about the held lock when we return to userspace.
	 */
	__sb_writers_release(inode->i_sb, SB_FREEZE_WRITE);
}

/**
 * kiocb_end_write - drop write access to a superblock after async file io
 * @iocb: the io context we sumbitted the write with
 *
 * Should be matched with a call to kiocb_start_write().
 */
static inline void kiocb_end_write(struct kiocb *iocb)
{
	struct inode *inode = file_inode(iocb->ki_filp);

	/*
	 * Tell lockdep we inherited freeze protection from submission thread.
	 */
	__sb_writers_acquired(inode->i_sb, SB_FREEZE_WRITE);
	sb_end_write(inode->i_sb);
}

/*
 * This is used for regular files where some users -- especially the
 * currently executed binary in a process, previously handled via
 * VM_DENYWRITE -- cannot handle concurrent write (and maybe mmap
 * read-write shared) accesses.
 *
 * get_write_access() gets write permission for a file.
 * put_write_access() releases this write permission.
 * deny_write_access() denies write access to a file.
 * allow_write_access() re-enables write access to a file.
 *
 * The i_writecount field of an inode can have the following values:
 * 0: no write access, no denied write access
 * < 0: (-i_writecount) users that denied write access to the file.
 * > 0: (i_writecount) users that have write access to the file.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Normally we operate on that counter with atomic_{inc,dec} and it's safe
 * except for the cases where we don't hold i_writecount yet. Then we need to
 * use {get,deny}_write_access() - these functions check the sign and refuse
 * to do the change if sign is wrong.
 */
static inline int get_write_access(struct inode *inode)
{
	return atomic_inc_unless_negative(&inode->i_writecount) ? 0 : -ETXTBSY;
}
static inline int deny_write_access(struct file *file)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return atomic_dec_unless_positive(&inode->i_writecount) ? 0 : -ETXTBSY;
}
static inline void put_write_access(struct inode * inode)
{
	atomic_dec(&inode->i_writecount);
}
static inline void allow_write_access(struct file *file)
{
	if (file)
<<<<<<< HEAD
		atomic_inc(&file->f_path.dentry->d_inode->i_writecount);
}
#ifdef CONFIG_IMA
static inline void i_readcount_dec(struct inode *inode)
{
	BUG_ON(!atomic_read(&inode->i_readcount));
	atomic_dec(&inode->i_readcount);
=======
		atomic_inc(&file_inode(file)->i_writecount);
}
static inline bool inode_is_open_for_write(const struct inode *inode)
{
	return atomic_read(&inode->i_writecount) > 0;
}

#if defined(CONFIG_IMA) || defined(CONFIG_FILE_LOCKING)
static inline void i_readcount_dec(struct inode *inode)
{
	BUG_ON(atomic_dec_return(&inode->i_readcount) < 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static inline void i_readcount_inc(struct inode *inode)
{
	atomic_inc(&inode->i_readcount);
}
#else
static inline void i_readcount_dec(struct inode *inode)
{
	return;
}
static inline void i_readcount_inc(struct inode *inode)
{
	return;
}
#endif
extern int do_pipe_flags(int *, int);
<<<<<<< HEAD
extern struct file *create_read_pipe(struct file *f, int flags);
extern struct file *create_write_pipe(int flags);
extern void free_write_pipe(struct file *);

extern int kernel_read(struct file *, loff_t, char *, unsigned long);
extern struct file * open_exec(const char *);
 
/* fs/dcache.c -- generic fs support functions */
extern int is_subdir(struct dentry *, struct dentry *);
extern int path_is_under(struct path *, struct path *);
extern ino_t find_inode_number(struct dentry *, struct qstr *);
=======

extern ssize_t kernel_read(struct file *, void *, size_t, loff_t *);
ssize_t __kernel_read(struct file *file, void *buf, size_t count, loff_t *pos);
extern ssize_t kernel_write(struct file *, const void *, size_t, loff_t *);
extern ssize_t __kernel_write(struct file *, const void *, size_t, loff_t *);
extern struct file * open_exec(const char *);
 
/* fs/dcache.c -- generic fs support functions */
extern bool is_subdir(struct dentry *, struct dentry *);
extern bool path_is_under(const struct path *, const struct path *);

extern char *file_path(struct file *, char *, int);

/**
 * is_dot_dotdot - returns true only if @name is "." or ".."
 * @name: file name to check
 * @len: length of file name, in bytes
 */
static inline bool is_dot_dotdot(const char *name, size_t len)
{
	return len && unlikely(name[0] == '.') &&
		(len == 1 || (len == 2 && name[1] == '.'));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/err.h>

/* needed for stackable file system support */
<<<<<<< HEAD
extern loff_t default_llseek(struct file *file, loff_t offset, int origin);

extern loff_t vfs_llseek(struct file *file, loff_t offset, int origin);
=======
extern loff_t default_llseek(struct file *file, loff_t offset, int whence);

extern loff_t vfs_llseek(struct file *file, loff_t offset, int whence);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int inode_init_always(struct super_block *, struct inode *);
extern void inode_init_once(struct inode *);
extern void address_space_init_once(struct address_space *mapping);
<<<<<<< HEAD
extern void ihold(struct inode * inode);
extern void iput(struct inode *);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct inode * igrab(struct inode *);
extern ino_t iunique(struct super_block *, ino_t);
extern int inode_needs_sync(struct inode *inode);
extern int generic_delete_inode(struct inode *inode);
static inline int generic_drop_inode(struct inode *inode)
{
	return !inode->i_nlink || inode_unhashed(inode);
}
<<<<<<< HEAD
=======
extern void d_mark_dontcache(struct inode *inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct inode *ilookup5_nowait(struct super_block *sb,
		unsigned long hashval, int (*test)(struct inode *, void *),
		void *data);
extern struct inode *ilookup5(struct super_block *sb, unsigned long hashval,
		int (*test)(struct inode *, void *), void *data);
extern struct inode *ilookup(struct super_block *sb, unsigned long ino);

<<<<<<< HEAD
extern struct inode * iget5_locked(struct super_block *, unsigned long, int (*test)(struct inode *, void *), int (*set)(struct inode *, void *), void *);
extern struct inode * iget_locked(struct super_block *, unsigned long);
=======
extern struct inode *inode_insert5(struct inode *inode, unsigned long hashval,
		int (*test)(struct inode *, void *),
		int (*set)(struct inode *, void *),
		void *data);
extern struct inode * iget5_locked(struct super_block *, unsigned long, int (*test)(struct inode *, void *), int (*set)(struct inode *, void *), void *);
extern struct inode * iget_locked(struct super_block *, unsigned long);
extern struct inode *find_inode_nowait(struct super_block *,
				       unsigned long,
				       int (*match)(struct inode *,
						    unsigned long, void *),
				       void *data);
extern struct inode *find_inode_rcu(struct super_block *, unsigned long,
				    int (*)(struct inode *, void *), void *);
extern struct inode *find_inode_by_ino_rcu(struct super_block *, unsigned long);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int insert_inode_locked4(struct inode *, unsigned long, int (*test)(struct inode *, void *), void *);
extern int insert_inode_locked(struct inode *);
#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern void lockdep_annotate_inode_mutex_key(struct inode *inode);
#else
static inline void lockdep_annotate_inode_mutex_key(struct inode *inode) { };
#endif
extern void unlock_new_inode(struct inode *);
<<<<<<< HEAD
extern unsigned int get_next_ino(void);

extern void __iget(struct inode * inode);
extern void iget_failed(struct inode *);
extern void end_writeback(struct inode *);
=======
extern void discard_new_inode(struct inode *);
extern unsigned int get_next_ino(void);
extern void evict_inodes(struct super_block *sb);
void dump_mapping(const struct address_space *);

/*
 * Userspace may rely on the inode number being non-zero. For example, glibc
 * simply ignores files with zero i_ino in unlink() and other places.
 *
 * As an additional complication, if userspace was compiled with
 * _FILE_OFFSET_BITS=32 on a 64-bit kernel we'll only end up reading out the
 * lower 32 bits, so we need to check that those aren't zero explicitly. With
 * _FILE_OFFSET_BITS=64, this may cause some harmless false-negatives, but
 * better safe than sorry.
 */
static inline bool is_zero_ino(ino_t ino)
{
	return (u32)ino == 0;
}

extern void __iget(struct inode * inode);
extern void iget_failed(struct inode *);
extern void clear_inode(struct inode *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void __destroy_inode(struct inode *);
extern struct inode *new_inode_pseudo(struct super_block *sb);
extern struct inode *new_inode(struct super_block *sb);
extern void free_inode_nonrcu(struct inode *inode);
<<<<<<< HEAD
extern int should_remove_suid(struct dentry *);
extern int file_remove_suid(struct file *);
=======
extern int setattr_should_drop_suidgid(struct mnt_idmap *, struct inode *);
extern int file_remove_privs_flags(struct file *file, unsigned int flags);
extern int file_remove_privs(struct file *);
int setattr_should_drop_sgid(struct mnt_idmap *idmap,
			     const struct inode *inode);

/*
 * This must be used for allocating filesystems specific inodes to set
 * up the inode reclaim context correctly.
 */
static inline void *
alloc_inode_sb(struct super_block *sb, struct kmem_cache *cache, gfp_t gfp)
{
	return kmem_cache_alloc_lru(cache, &sb->s_inode_lru, gfp);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void __insert_inode_hash(struct inode *, unsigned long hashval);
static inline void insert_inode_hash(struct inode *inode)
{
	__insert_inode_hash(inode, inode->i_ino);
}

extern void __remove_inode_hash(struct inode *);
static inline void remove_inode_hash(struct inode *inode)
{
<<<<<<< HEAD
	if (!inode_unhashed(inode))
=======
	if (!inode_unhashed(inode) && !hlist_fake(&inode->i_hash))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__remove_inode_hash(inode);
}

extern void inode_sb_list_add(struct inode *inode);
<<<<<<< HEAD

#ifdef CONFIG_BLOCK
extern void submit_bio(int, struct bio *);
extern int bdev_read_only(struct block_device *);
#endif
extern int set_blocksize(struct block_device *, int);
=======
extern void inode_add_lru(struct inode *inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int sb_set_blocksize(struct super_block *, int);
extern int sb_min_blocksize(struct super_block *, int);

extern int generic_file_mmap(struct file *, struct vm_area_struct *);
extern int generic_file_readonly_mmap(struct file *, struct vm_area_struct *);
<<<<<<< HEAD
extern int file_read_actor(read_descriptor_t * desc, struct page *page, unsigned long offset, unsigned long size);
int generic_write_checks(struct file *file, loff_t *pos, size_t *count, int isblk);
extern ssize_t generic_file_aio_read(struct kiocb *, const struct iovec *, unsigned long, loff_t);
extern ssize_t __generic_file_aio_write(struct kiocb *, const struct iovec *, unsigned long,
		loff_t *);
extern ssize_t generic_file_aio_write(struct kiocb *, const struct iovec *, unsigned long, loff_t);
extern ssize_t generic_file_direct_write(struct kiocb *, const struct iovec *,
		unsigned long *, loff_t, loff_t *, size_t, size_t);
extern ssize_t generic_file_buffered_write(struct kiocb *, const struct iovec *,
		unsigned long, loff_t, loff_t *, size_t, ssize_t);
extern ssize_t do_sync_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos);
extern ssize_t do_sync_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos);
extern int generic_segment_checks(const struct iovec *iov,
		unsigned long *nr_segs, size_t *count, int access_flags);

/* fs/block_dev.c */
extern ssize_t blkdev_aio_write(struct kiocb *iocb, const struct iovec *iov,
				unsigned long nr_segs, loff_t pos);
extern int blkdev_fsync(struct file *filp, loff_t start, loff_t end,
			int datasync);
extern void block_sync_page(struct page *page);

/* fs/splice.c */
extern ssize_t generic_file_splice_read(struct file *, loff_t *,
		struct pipe_inode_info *, size_t, unsigned int);
extern ssize_t default_file_splice_read(struct file *, loff_t *,
		struct pipe_inode_info *, size_t, unsigned int);
extern ssize_t generic_file_splice_write(struct pipe_inode_info *,
		struct file *, loff_t *, size_t, unsigned int);
extern ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe,
		struct file *out, loff_t *, size_t len, unsigned int flags);
extern long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
		size_t len, unsigned int flags);

extern void
file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping);
extern loff_t noop_llseek(struct file *file, loff_t offset, int origin);
extern loff_t no_llseek(struct file *file, loff_t offset, int origin);
extern loff_t generic_file_llseek(struct file *file, loff_t offset, int origin);
extern loff_t generic_file_llseek_size(struct file *file, loff_t offset,
		int origin, loff_t maxsize);
extern int generic_file_open(struct inode * inode, struct file * filp);
extern int nonseekable_open(struct inode * inode, struct file * filp);

#ifdef CONFIG_FS_XIP
extern ssize_t xip_file_read(struct file *filp, char __user *buf, size_t len,
			     loff_t *ppos);
extern int xip_file_mmap(struct file * file, struct vm_area_struct * vma);
extern ssize_t xip_file_write(struct file *filp, const char __user *buf,
			      size_t len, loff_t *ppos);
extern int xip_truncate_page(struct address_space *mapping, loff_t from);
#else
static inline int xip_truncate_page(struct address_space *mapping, loff_t from)
{
	return 0;
}
#endif

#ifdef CONFIG_BLOCK
typedef void (dio_submit_t)(int rw, struct bio *bio, struct inode *inode,
=======
extern ssize_t generic_write_checks(struct kiocb *, struct iov_iter *);
int generic_write_checks_count(struct kiocb *iocb, loff_t *count);
extern int generic_write_check_limits(struct file *file, loff_t pos,
		loff_t *count);
extern int generic_file_rw_checks(struct file *file_in, struct file *file_out);
ssize_t filemap_read(struct kiocb *iocb, struct iov_iter *to,
		ssize_t already_read);
extern ssize_t generic_file_read_iter(struct kiocb *, struct iov_iter *);
extern ssize_t __generic_file_write_iter(struct kiocb *, struct iov_iter *);
extern ssize_t generic_file_write_iter(struct kiocb *, struct iov_iter *);
extern ssize_t generic_file_direct_write(struct kiocb *, struct iov_iter *);
ssize_t generic_perform_write(struct kiocb *, struct iov_iter *);
ssize_t direct_write_fallback(struct kiocb *iocb, struct iov_iter *iter,
		ssize_t direct_written, ssize_t buffered_written);

ssize_t vfs_iter_read(struct file *file, struct iov_iter *iter, loff_t *ppos,
		rwf_t flags);
ssize_t vfs_iter_write(struct file *file, struct iov_iter *iter, loff_t *ppos,
		rwf_t flags);
ssize_t vfs_iocb_iter_read(struct file *file, struct kiocb *iocb,
			   struct iov_iter *iter);
ssize_t vfs_iocb_iter_write(struct file *file, struct kiocb *iocb,
			    struct iov_iter *iter);

/* fs/splice.c */
ssize_t filemap_splice_read(struct file *in, loff_t *ppos,
			    struct pipe_inode_info *pipe,
			    size_t len, unsigned int flags);
ssize_t copy_splice_read(struct file *in, loff_t *ppos,
			 struct pipe_inode_info *pipe,
			 size_t len, unsigned int flags);
extern ssize_t iter_file_splice_write(struct pipe_inode_info *,
		struct file *, loff_t *, size_t, unsigned int);


extern void
file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping);
extern loff_t noop_llseek(struct file *file, loff_t offset, int whence);
#define no_llseek NULL
extern loff_t vfs_setpos(struct file *file, loff_t offset, loff_t maxsize);
extern loff_t generic_file_llseek(struct file *file, loff_t offset, int whence);
extern loff_t generic_file_llseek_size(struct file *file, loff_t offset,
		int whence, loff_t maxsize, loff_t eof);
extern loff_t fixed_size_llseek(struct file *file, loff_t offset,
		int whence, loff_t size);
extern loff_t no_seek_end_llseek_size(struct file *, loff_t, int, loff_t);
extern loff_t no_seek_end_llseek(struct file *, loff_t, int);
int rw_verify_area(int, struct file *, const loff_t *, size_t);
extern int generic_file_open(struct inode * inode, struct file * filp);
extern int nonseekable_open(struct inode * inode, struct file * filp);
extern int stream_open(struct inode * inode, struct file * filp);

#ifdef CONFIG_BLOCK
typedef void (dio_submit_t)(struct bio *bio, struct inode *inode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    loff_t file_offset);

enum {
	/* need locking between buffered and direct access */
	DIO_LOCKING	= 0x01,

	/* filesystem does not support filling holes */
	DIO_SKIP_HOLES	= 0x02,
};

<<<<<<< HEAD
void dio_end_io(struct bio *bio, int error);
void inode_dio_wait(struct inode *inode);
void inode_dio_done(struct inode *inode);
struct inode *dio_bio_get_inode(struct bio *bio);

ssize_t __blockdev_direct_IO(int rw, struct kiocb *iocb, struct inode *inode,
	struct block_device *bdev, const struct iovec *iov, loff_t offset,
	unsigned long nr_segs, get_block_t get_block, dio_iodone_t end_io,
	dio_submit_t submit_io,	int flags);

static inline ssize_t blockdev_direct_IO(int rw, struct kiocb *iocb,
		struct inode *inode, const struct iovec *iov, loff_t offset,
		unsigned long nr_segs, get_block_t get_block)
{
	return __blockdev_direct_IO(rw, iocb, inode, inode->i_sb->s_bdev, iov,
				    offset, nr_segs, get_block, NULL, NULL,
				    DIO_LOCKING | DIO_SKIP_HOLES);
}
#else
static inline void inode_dio_wait(struct inode *inode)
{
}
#endif

=======
ssize_t __blockdev_direct_IO(struct kiocb *iocb, struct inode *inode,
			     struct block_device *bdev, struct iov_iter *iter,
			     get_block_t get_block,
			     dio_iodone_t end_io,
			     int flags);

static inline ssize_t blockdev_direct_IO(struct kiocb *iocb,
					 struct inode *inode,
					 struct iov_iter *iter,
					 get_block_t get_block)
{
	return __blockdev_direct_IO(iocb, inode, inode->i_sb->s_bdev, iter,
			get_block, NULL, DIO_LOCKING | DIO_SKIP_HOLES);
}
#endif

void inode_dio_wait(struct inode *inode);

/**
 * inode_dio_begin - signal start of a direct I/O requests
 * @inode: inode the direct I/O happens on
 *
 * This is called once we've finished processing a direct I/O request,
 * and is used to wake up callers waiting for direct I/O to be quiesced.
 */
static inline void inode_dio_begin(struct inode *inode)
{
	atomic_inc(&inode->i_dio_count);
}

/**
 * inode_dio_end - signal finish of a direct I/O requests
 * @inode: inode the direct I/O happens on
 *
 * This is called once we've finished processing a direct I/O request,
 * and is used to wake up callers waiting for direct I/O to be quiesced.
 */
static inline void inode_dio_end(struct inode *inode)
{
	if (atomic_dec_and_test(&inode->i_dio_count))
		wake_up_bit(&inode->i_state, __I_DIO_WAKEUP);
}

extern void inode_set_flags(struct inode *inode, unsigned int flags,
			    unsigned int mask);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern const struct file_operations generic_ro_fops;

#define special_file(m) (S_ISCHR(m)||S_ISBLK(m)||S_ISFIFO(m)||S_ISSOCK(m))

<<<<<<< HEAD
extern int vfs_readlink(struct dentry *, char __user *, int, const char *);
extern int vfs_follow_link(struct nameidata *, const char *);
extern int page_readlink(struct dentry *, char __user *, int);
extern void *page_follow_link_light(struct dentry *, struct nameidata *);
extern void page_put_link(struct dentry *, struct nameidata *, void *);
extern int __page_symlink(struct inode *inode, const char *symname, int len,
		int nofs);
extern int page_symlink(struct inode *inode, const char *symname, int len);
extern const struct inode_operations page_symlink_inode_operations;
extern int generic_readlink(struct dentry *, char __user *, int);
extern void generic_fillattr(struct inode *, struct kstat *);
extern int vfs_getattr(struct vfsmount *, struct dentry *, struct kstat *);
void __inode_add_bytes(struct inode *inode, loff_t bytes);
void inode_add_bytes(struct inode *inode, loff_t bytes);
void inode_sub_bytes(struct inode *inode, loff_t bytes);
loff_t inode_get_bytes(struct inode *inode);
void inode_set_bytes(struct inode *inode, loff_t bytes);

extern int vfs_readdir(struct file *, filldir_t, void *);

extern int vfs_stat(const char __user *, struct kstat *);
extern int vfs_lstat(const char __user *, struct kstat *);
extern int vfs_fstat(unsigned int, struct kstat *);
extern int vfs_fstatat(int , const char __user *, struct kstat *, int);

extern int do_vfs_ioctl(struct file *filp, unsigned int fd, unsigned int cmd,
		    unsigned long arg);
extern int __generic_block_fiemap(struct inode *inode,
				  struct fiemap_extent_info *fieinfo,
				  loff_t start, loff_t len,
				  get_block_t *get_block);
extern int generic_block_fiemap(struct inode *inode,
				struct fiemap_extent_info *fieinfo, u64 start,
				u64 len, get_block_t *get_block);

extern void get_filesystem(struct file_system_type *fs);
extern void put_filesystem(struct file_system_type *fs);
extern struct file_system_type *get_fs_type(const char *name);
extern struct super_block *get_super(struct block_device *);
extern struct super_block *get_super_thawed(struct block_device *);
extern struct super_block *get_active_super(struct block_device *bdev);
extern void drop_super(struct super_block *sb);
=======
extern int readlink_copy(char __user *, int, const char *);
extern int page_readlink(struct dentry *, char __user *, int);
extern const char *page_get_link(struct dentry *, struct inode *,
				 struct delayed_call *);
extern void page_put_link(void *);
extern int page_symlink(struct inode *inode, const char *symname, int len);
extern const struct inode_operations page_symlink_inode_operations;
extern void kfree_link(void *);
void generic_fillattr(struct mnt_idmap *, u32, struct inode *, struct kstat *);
void generic_fill_statx_attr(struct inode *inode, struct kstat *stat);
extern int vfs_getattr_nosec(const struct path *, struct kstat *, u32, unsigned int);
extern int vfs_getattr(const struct path *, struct kstat *, u32, unsigned int);
void __inode_add_bytes(struct inode *inode, loff_t bytes);
void inode_add_bytes(struct inode *inode, loff_t bytes);
void __inode_sub_bytes(struct inode *inode, loff_t bytes);
void inode_sub_bytes(struct inode *inode, loff_t bytes);
static inline loff_t __inode_get_bytes(struct inode *inode)
{
	return (((loff_t)inode->i_blocks) << 9) + inode->i_bytes;
}
loff_t inode_get_bytes(struct inode *inode);
void inode_set_bytes(struct inode *inode, loff_t bytes);
const char *simple_get_link(struct dentry *, struct inode *,
			    struct delayed_call *);
extern const struct inode_operations simple_symlink_inode_operations;

extern int iterate_dir(struct file *, struct dir_context *);

int vfs_fstatat(int dfd, const char __user *filename, struct kstat *stat,
		int flags);
int vfs_fstat(int fd, struct kstat *stat);

static inline int vfs_stat(const char __user *filename, struct kstat *stat)
{
	return vfs_fstatat(AT_FDCWD, filename, stat, 0);
}
static inline int vfs_lstat(const char __user *name, struct kstat *stat)
{
	return vfs_fstatat(AT_FDCWD, name, stat, AT_SYMLINK_NOFOLLOW);
}

extern const char *vfs_get_link(struct dentry *, struct delayed_call *);
extern int vfs_readlink(struct dentry *, char __user *, int);

extern struct file_system_type *get_filesystem(struct file_system_type *fs);
extern void put_filesystem(struct file_system_type *fs);
extern struct file_system_type *get_fs_type(const char *name);
extern void drop_super(struct super_block *sb);
extern void drop_super_exclusive(struct super_block *sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void iterate_supers(void (*)(struct super_block *, void *), void *);
extern void iterate_supers_type(struct file_system_type *,
			        void (*)(struct super_block *, void *), void *);

extern int dcache_dir_open(struct inode *, struct file *);
extern int dcache_dir_close(struct inode *, struct file *);
extern loff_t dcache_dir_lseek(struct file *, loff_t, int);
<<<<<<< HEAD
extern int dcache_readdir(struct file *, void *, filldir_t);
extern int simple_setattr(struct dentry *, struct iattr *);
extern int simple_getattr(struct vfsmount *, struct dentry *, struct kstat *);
=======
extern int dcache_readdir(struct file *, struct dir_context *);
extern int simple_setattr(struct mnt_idmap *, struct dentry *,
			  struct iattr *);
extern int simple_getattr(struct mnt_idmap *, const struct path *,
			  struct kstat *, u32, unsigned int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int simple_statfs(struct dentry *, struct kstatfs *);
extern int simple_open(struct inode *inode, struct file *file);
extern int simple_link(struct dentry *, struct inode *, struct dentry *);
extern int simple_unlink(struct inode *, struct dentry *);
extern int simple_rmdir(struct inode *, struct dentry *);
<<<<<<< HEAD
extern int simple_rename(struct inode *, struct dentry *, struct inode *, struct dentry *);
extern int noop_fsync(struct file *, loff_t, loff_t, int);
extern int simple_empty(struct dentry *);
extern int simple_readpage(struct file *file, struct page *page);
extern int simple_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata);
extern int simple_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata);
=======
void simple_rename_timestamp(struct inode *old_dir, struct dentry *old_dentry,
			     struct inode *new_dir, struct dentry *new_dentry);
extern int simple_rename_exchange(struct inode *old_dir, struct dentry *old_dentry,
				  struct inode *new_dir, struct dentry *new_dentry);
extern int simple_rename(struct mnt_idmap *, struct inode *,
			 struct dentry *, struct inode *, struct dentry *,
			 unsigned int);
extern void simple_recursive_removal(struct dentry *,
                              void (*callback)(struct dentry *));
extern int noop_fsync(struct file *, loff_t, loff_t, int);
extern ssize_t noop_direct_IO(struct kiocb *iocb, struct iov_iter *iter);
extern int simple_empty(struct dentry *);
extern int simple_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len,
			struct page **pagep, void **fsdata);
extern const struct address_space_operations ram_aops;
extern int always_delete_dentry(const struct dentry *);
extern struct inode *alloc_anon_inode(struct super_block *);
extern int simple_nosetlease(struct file *, int, struct file_lease **, void **);
extern const struct dentry_operations simple_dentry_operations;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct dentry *simple_lookup(struct inode *, struct dentry *, unsigned int flags);
extern ssize_t generic_read_dir(struct file *, char __user *, size_t, loff_t *);
extern const struct file_operations simple_dir_operations;
extern const struct inode_operations simple_dir_inode_operations;
<<<<<<< HEAD
struct tree_descr { char *name; const struct file_operations *ops; int mode; };
struct dentry *d_alloc_name(struct dentry *, const char *);
extern int simple_fill_super(struct super_block *, unsigned long, struct tree_descr *);
=======
extern void make_empty_dir_inode(struct inode *inode);
extern bool is_empty_dir_inode(struct inode *inode);
struct tree_descr { const char *name; const struct file_operations *ops; int mode; };
struct dentry *d_alloc_name(struct dentry *, const char *);
extern int simple_fill_super(struct super_block *, unsigned long,
			     const struct tree_descr *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int simple_pin_fs(struct file_system_type *, struct vfsmount **mount, int *count);
extern void simple_release_fs(struct vfsmount **mount, int *count);

extern ssize_t simple_read_from_buffer(void __user *to, size_t count,
			loff_t *ppos, const void *from, size_t available);
extern ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
		const void __user *from, size_t count);

<<<<<<< HEAD
=======
struct offset_ctx {
	struct maple_tree	mt;
	unsigned long		next_offset;
};

void simple_offset_init(struct offset_ctx *octx);
int simple_offset_add(struct offset_ctx *octx, struct dentry *dentry);
void simple_offset_remove(struct offset_ctx *octx, struct dentry *dentry);
int simple_offset_empty(struct dentry *dentry);
int simple_offset_rename_exchange(struct inode *old_dir,
				  struct dentry *old_dentry,
				  struct inode *new_dir,
				  struct dentry *new_dentry);
void simple_offset_destroy(struct offset_ctx *octx);

extern const struct file_operations simple_offset_dir_operations;

extern int __generic_file_fsync(struct file *, loff_t, loff_t, int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int generic_file_fsync(struct file *, loff_t, loff_t, int);

extern int generic_check_addressable(unsigned, u64);

<<<<<<< HEAD
#ifdef CONFIG_MIGRATION
extern int buffer_migrate_page(struct address_space *,
				struct page *, struct page *,
				enum migrate_mode);
#else
#define buffer_migrate_page NULL
#endif

extern int inode_change_ok(const struct inode *, struct iattr *);
extern int inode_newsize_ok(const struct inode *, loff_t offset);
extern void setattr_copy(struct inode *inode, const struct iattr *attr);

extern int file_update_time(struct file *file);

extern int generic_show_options(struct seq_file *m, struct dentry *root);
extern void save_mount_options(struct super_block *sb, char *options);
extern void replace_mount_options(struct super_block *sb, char *options);
=======
extern void generic_set_sb_d_ops(struct super_block *sb);

static inline bool sb_has_encoding(const struct super_block *sb)
{
#if IS_ENABLED(CONFIG_UNICODE)
	return !!sb->s_encoding;
#else
	return false;
#endif
}

int may_setattr(struct mnt_idmap *idmap, struct inode *inode,
		unsigned int ia_valid);
int setattr_prepare(struct mnt_idmap *, struct dentry *, struct iattr *);
extern int inode_newsize_ok(const struct inode *, loff_t offset);
void setattr_copy(struct mnt_idmap *, struct inode *inode,
		  const struct iattr *attr);

extern int file_update_time(struct file *file);

static inline bool vma_is_dax(const struct vm_area_struct *vma)
{
	return vma->vm_file && IS_DAX(vma->vm_file->f_mapping->host);
}

static inline bool vma_is_fsdax(struct vm_area_struct *vma)
{
	struct inode *inode;

	if (!IS_ENABLED(CONFIG_FS_DAX) || !vma->vm_file)
		return false;
	if (!vma_is_dax(vma))
		return false;
	inode = file_inode(vma->vm_file);
	if (S_ISCHR(inode->i_mode))
		return false; /* device-dax */
	return true;
}

static inline int iocb_flags(struct file *file)
{
	int res = 0;
	if (file->f_flags & O_APPEND)
		res |= IOCB_APPEND;
	if (file->f_flags & O_DIRECT)
		res |= IOCB_DIRECT;
	if (file->f_flags & O_DSYNC)
		res |= IOCB_DSYNC;
	if (file->f_flags & __O_SYNC)
		res |= IOCB_SYNC;
	return res;
}

static inline int kiocb_set_rw_flags(struct kiocb *ki, rwf_t flags)
{
	int kiocb_flags = 0;

	/* make sure there's no overlap between RWF and private IOCB flags */
	BUILD_BUG_ON((__force int) RWF_SUPPORTED & IOCB_EVENTFD);

	if (!flags)
		return 0;
	if (unlikely(flags & ~RWF_SUPPORTED))
		return -EOPNOTSUPP;
	if (unlikely((flags & RWF_APPEND) && (flags & RWF_NOAPPEND)))
		return -EINVAL;

	if (flags & RWF_NOWAIT) {
		if (!(ki->ki_filp->f_mode & FMODE_NOWAIT))
			return -EOPNOTSUPP;
		kiocb_flags |= IOCB_NOIO;
	}
	kiocb_flags |= (__force int) (flags & RWF_SUPPORTED);
	if (flags & RWF_SYNC)
		kiocb_flags |= IOCB_DSYNC;

	if ((flags & RWF_NOAPPEND) && (ki->ki_flags & IOCB_APPEND)) {
		if (IS_APPEND(file_inode(ki->ki_filp)))
			return -EPERM;
		ki->ki_flags &= ~IOCB_APPEND;
	}

	ki->ki_flags |= kiocb_flags;
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline ino_t parent_ino(struct dentry *dentry)
{
	ino_t res;

	/*
	 * Don't strictly need d_lock here? If the parent ino could change
	 * then surely we'd have a deeper race in the caller?
	 */
	spin_lock(&dentry->d_lock);
	res = dentry->d_parent->d_inode->i_ino;
	spin_unlock(&dentry->d_lock);
	return res;
}

/* Transaction based IO helpers */

/*
 * An argresp is stored in an allocated page and holds the
 * size of the argument or response, along with its content
 */
struct simple_transaction_argresp {
	ssize_t size;
<<<<<<< HEAD
	char data[0];
=======
	char data[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define SIMPLE_TRANSACTION_LIMIT (PAGE_SIZE - sizeof(struct simple_transaction_argresp))

char *simple_transaction_get(struct file *file, const char __user *buf,
				size_t size);
ssize_t simple_transaction_read(struct file *file, char __user *buf,
				size_t size, loff_t *pos);
int simple_transaction_release(struct inode *inode, struct file *file);

void simple_transaction_set(struct file *file, size_t n);

/*
 * simple attribute files
 *
 * These attributes behave similar to those in sysfs:
 *
 * Writing to an attribute immediately sets a value, an open file can be
 * written to multiple times.
 *
 * Reading from an attribute creates a buffer from the value that might get
 * read with multiple read calls. When the attribute has been read
 * completely, no further read calls are possible until the file is opened
 * again.
 *
 * All attributes contain a text representation of a numeric value
 * that are accessed with the get() and set() functions.
 */
<<<<<<< HEAD
#define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)		\
=======
#define DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, __is_signed)	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __fops ## _open(struct inode *inode, struct file *file)	\
{									\
	__simple_attr_check_format(__fmt, 0ull);			\
	return simple_attr_open(inode, file, __get, __set, __fmt);	\
}									\
static const struct file_operations __fops = {				\
	.owner	 = THIS_MODULE,						\
	.open	 = __fops ## _open,					\
	.release = simple_attr_release,					\
	.read	 = simple_attr_read,					\
<<<<<<< HEAD
	.write	 = simple_attr_write,					\
	.llseek	 = generic_file_llseek,					\
};
=======
	.write	 = (__is_signed) ? simple_attr_write_signed : simple_attr_write,	\
	.llseek	 = generic_file_llseek,					\
}

#define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)		\
	DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, false)

#define DEFINE_SIMPLE_ATTRIBUTE_SIGNED(__fops, __get, __set, __fmt)	\
	DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, true)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline __printf(1, 2)
void __simple_attr_check_format(const char *fmt, ...)
{
	/* don't do anything, just let the compiler check the arguments; */
}

int simple_attr_open(struct inode *inode, struct file *file,
		     int (*get)(void *, u64 *), int (*set)(void *, u64),
		     const char *fmt);
int simple_attr_release(struct inode *inode, struct file *file);
ssize_t simple_attr_read(struct file *file, char __user *buf,
			 size_t len, loff_t *ppos);
ssize_t simple_attr_write(struct file *file, const char __user *buf,
			  size_t len, loff_t *ppos);
<<<<<<< HEAD

struct ctl_table;
int proc_nr_files(struct ctl_table *table, int write,
		  void __user *buffer, size_t *lenp, loff_t *ppos);
int proc_nr_dentry(struct ctl_table *table, int write,
		  void __user *buffer, size_t *lenp, loff_t *ppos);
int proc_nr_inodes(struct ctl_table *table, int write,
		   void __user *buffer, size_t *lenp, loff_t *ppos);
int __init get_filesystem_list(char *buf);
=======
ssize_t simple_attr_write_signed(struct file *file, const char __user *buf,
				 size_t len, loff_t *ppos);

struct ctl_table;
int __init list_bdev_fs_names(char *buf, size_t size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __FMODE_EXEC		((__force int) FMODE_EXEC)
#define __FMODE_NONOTIFY	((__force int) FMODE_NONOTIFY)

#define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
#define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
					    (flag & __FMODE_NONOTIFY)))

<<<<<<< HEAD
static inline int is_sxid(umode_t mode)
{
	return (mode & S_ISUID) || ((mode & S_ISGID) && (mode & S_IXGRP));
=======
static inline bool is_sxid(umode_t mode)
{
	return mode & (S_ISUID | S_ISGID);
}

static inline int check_sticky(struct mnt_idmap *idmap,
			       struct inode *dir, struct inode *inode)
{
	if (!(dir->i_mode & S_ISVTX))
		return 0;

	return __check_sticky(idmap, dir, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void inode_has_no_xattr(struct inode *inode)
{
<<<<<<< HEAD
	if (!is_sxid(inode->i_mode) && (inode->i_sb->s_flags & MS_NOSEC))
		inode->i_flags |= S_NOSEC;
}

#endif /* __KERNEL__ */
=======
	if (!is_sxid(inode->i_mode) && (inode->i_sb->s_flags & SB_NOSEC))
		inode->i_flags |= S_NOSEC;
}

static inline bool is_root_inode(struct inode *inode)
{
	return inode == inode->i_sb->s_root->d_inode;
}

static inline bool dir_emit(struct dir_context *ctx,
			    const char *name, int namelen,
			    u64 ino, unsigned type)
{
	return ctx->actor(ctx, name, namelen, ctx->pos, ino, type);
}
static inline bool dir_emit_dot(struct file *file, struct dir_context *ctx)
{
	return ctx->actor(ctx, ".", 1, ctx->pos,
			  file->f_path.dentry->d_inode->i_ino, DT_DIR);
}
static inline bool dir_emit_dotdot(struct file *file, struct dir_context *ctx)
{
	return ctx->actor(ctx, "..", 2, ctx->pos,
			  parent_ino(file->f_path.dentry), DT_DIR);
}
static inline bool dir_emit_dots(struct file *file, struct dir_context *ctx)
{
	if (ctx->pos == 0) {
		if (!dir_emit_dot(file, ctx))
			return false;
		ctx->pos = 1;
	}
	if (ctx->pos == 1) {
		if (!dir_emit_dotdot(file, ctx))
			return false;
		ctx->pos = 2;
	}
	return true;
}
static inline bool dir_relax(struct inode *inode)
{
	inode_unlock(inode);
	inode_lock(inode);
	return !IS_DEADDIR(inode);
}

static inline bool dir_relax_shared(struct inode *inode)
{
	inode_unlock_shared(inode);
	inode_lock_shared(inode);
	return !IS_DEADDIR(inode);
}

extern bool path_noexec(const struct path *path);
extern void inode_nohighmem(struct inode *inode);

/* mm/fadvise.c */
extern int vfs_fadvise(struct file *file, loff_t offset, loff_t len,
		       int advice);
extern int generic_fadvise(struct file *file, loff_t offset, loff_t len,
			   int advice);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_FS_H */
