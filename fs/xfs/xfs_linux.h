<<<<<<< HEAD
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __XFS_LINUX__
#define __XFS_LINUX__

#include <linux/types.h>
<<<<<<< HEAD

/*
 * XFS_BIG_BLKNOS needs block layer disk addresses to be 64 bits.
 * XFS_BIG_INUMS requires XFS_BIG_BLKNOS to be set.
 */
#if defined(CONFIG_LBDAF) || (BITS_PER_LONG == 64)
# define XFS_BIG_BLKNOS	1
# define XFS_BIG_INUMS	1
#else
# define XFS_BIG_BLKNOS	0
# define XFS_BIG_INUMS	0
#endif

#include "xfs_types.h"

#include "kmem.h"
#include "mrlock.h"
#include "time.h"
#include "uuid.h"

#include <linux/semaphore.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/file.h>
#include <linux/swap.h>
#include <linux/errno.h>
#include <linux/sched.h>
=======
#include <linux/uuid.h>

/*
 * Kernel specific type declarations for XFS
 */

typedef __s64			xfs_off_t;	/* <file offset> type */
typedef unsigned long long	xfs_ino_t;	/* <inode> type */
typedef __s64			xfs_daddr_t;	/* <disk address> type */
typedef __u32			xfs_dev_t;
typedef __u32			xfs_nlink_t;

#include "xfs_types.h"

#include <linux/semaphore.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/crc32c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/file.h>
#include <linux/filelock.h>
#include <linux/swap.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/bitops.h>
#include <linux/major.h>
#include <linux/pagemap.h>
#include <linux/vfs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/sort.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/delay.h>
#include <linux/log2.h>
<<<<<<< HEAD
=======
#include <linux/rwsem.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/ctype.h>
#include <linux/writeback.h>
#include <linux/capability.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/list_sort.h>
<<<<<<< HEAD
=======
#include <linux/ratelimit.h>
#include <linux/rhashtable.h>
#include <linux/xattr.h>
#include <linux/mnt_idmapping.h>
#include <linux/debugfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/page.h>
#include <asm/div64.h>
#include <asm/param.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>

#include "xfs_vnode.h"
=======
#include <linux/uaccess.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>

#include "xfs_fs.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "xfs_stats.h"
#include "xfs_sysctl.h"
#include "xfs_iops.h"
#include "xfs_aops.h"
#include "xfs_super.h"
<<<<<<< HEAD
#include "xfs_buf.h"
#include "xfs_message.h"
=======
#include "xfs_cksum.h"
#include "xfs_buf.h"
#include "xfs_message.h"
#include "xfs_drain.h"
#include "xfs_hooks.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef __BIG_ENDIAN
#define XFS_NATIVE_HOST 1
#else
#undef XFS_NATIVE_HOST
#endif

<<<<<<< HEAD
/*
 * Feature macros (disable/enable)
 */
#ifdef CONFIG_SMP
#define HAVE_PERCPU_SB	/* per cpu superblock counters are a 2.6 feature */
#else
#undef  HAVE_PERCPU_SB	/* per cpu superblock counters are a 2.6 feature */
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define irix_sgid_inherit	xfs_params.sgid_inherit.val
#define irix_symlink_mode	xfs_params.symlink_mode.val
#define xfs_panic_mask		xfs_params.panic_mask.val
#define xfs_error_level		xfs_params.error_level.val
#define xfs_syncd_centisecs	xfs_params.syncd_timer.val
#define xfs_stats_clear		xfs_params.stats_clear.val
#define xfs_inherit_sync	xfs_params.inherit_sync.val
#define xfs_inherit_nodump	xfs_params.inherit_nodump.val
#define xfs_inherit_noatime	xfs_params.inherit_noatim.val
<<<<<<< HEAD
#define xfs_buf_timer_centisecs	xfs_params.xfs_buf_timer.val
#define xfs_buf_age_centisecs	xfs_params.xfs_buf_age.val
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define xfs_inherit_nosymlinks	xfs_params.inherit_nosym.val
#define xfs_rotorstep		xfs_params.rotorstep.val
#define xfs_inherit_nodefrag	xfs_params.inherit_nodfrg.val
#define xfs_fstrm_centisecs	xfs_params.fstrm_timer.val
<<<<<<< HEAD

#define current_cpu()		(raw_smp_processor_id())
#define current_pid()		(current->pid)
#define current_test_flags(f)	(current->flags & (f))
#define current_set_flags_nested(sp, f)		\
		(*(sp) = current->flags, current->flags |= (f))
#define current_clear_flags_nested(sp, f)	\
		(*(sp) = current->flags, current->flags &= ~(f))
#define current_restore_flags_nested(sp, f)	\
		(current->flags = ((current->flags & ~(f)) | (*(sp) & (f))))

#define spinlock_destroy(lock)

=======
#define xfs_blockgc_secs	xfs_params.blockgc_timer.val

#define current_cpu()		(raw_smp_processor_id())
#define current_set_flags_nested(sp, f)		\
		(*(sp) = current->flags, current->flags |= (f))
#define current_restore_flags_nested(sp, f)	\
		(current->flags = ((current->flags & ~(f)) | (*(sp) & (f))))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NBBY		8		/* number of bits per byte */

/*
 * Size of block device i/o is parameterized here.
 * Currently the system supports page-sized i/o.
 */
<<<<<<< HEAD
#define	BLKDEV_IOSHIFT		PAGE_CACHE_SHIFT
=======
#define	BLKDEV_IOSHIFT		PAGE_SHIFT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	BLKDEV_IOSIZE		(1<<BLKDEV_IOSHIFT)
/* number of BB's per block device block */
#define	BLKDEV_BB		BTOBB(BLKDEV_IOSIZE)

#define ENOATTR		ENODATA		/* Attribute not found */
#define EWRONGFS	EINVAL		/* Mount with wrong filesystem type */
#define EFSCORRUPTED	EUCLEAN		/* Filesystem is corrupted */
<<<<<<< HEAD

#define SYNCHRONIZE()	barrier()
#define __return_address __builtin_return_address(0)

#define XFS_PROJID_DEFAULT	0
#define MAXPATHLEN	1024

#define MIN(a,b)	(min(a,b))
#define MAX(a,b)	(max(a,b))
#define howmany(x, y)	(((x)+((y)-1))/(y))

=======
#define EFSBADCRC	EBADMSG		/* Bad CRC detected */

#define __return_address __builtin_return_address(0)

/*
 * Return the address of a label.  Use barrier() so that the optimizer
 * won't reorder code to refactor the error jumpouts into a single
 * return, which throws off the reported address.
 */
#define __this_address	({ __label__ __here; __here: barrier(); &&__here; })

#define XFS_PROJID_DEFAULT	0

#define howmany(x, y)	(((x)+((y)-1))/(y))

static inline void delay(long ticks)
{
	schedule_timeout_uninterruptible(ticks);
}

/*
 * XFS wrapper structure for sysfs support. It depends on external data
 * structures and is embedded in various internal data structures to implement
 * the XFS sysfs object heirarchy. Define it here for broad access throughout
 * the codebase.
 */
struct xfs_kobj {
	struct kobject		kobject;
	struct completion	complete;
};

struct xstats {
	struct xfsstats __percpu	*xs_stats;
	struct xfs_kobj			xs_kobj;
};

extern struct xstats xfsstats;

static inline dev_t xfs_to_linux_dev_t(xfs_dev_t dev)
{
	return MKDEV(sysv_major(dev) & 0x1ff, sysv_minor(dev));
}

static inline xfs_dev_t linux_to_xfs_dev_t(dev_t dev)
{
	return sysv_encode_dev(dev);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Various platform dependent calls that don't fit anywhere else
 */
#define xfs_sort(a,n,s,fn)	sort(a,n,s,fn,NULL)
#define xfs_stack_trace()	dump_stack()

<<<<<<< HEAD

/* Move the kernel do_div definition off to one side */

#if defined __i386__
/* For ia32 we need to pull some tricks to get past various versions
 * of the compiler which do not like us using do_div in the middle
 * of large functions.
 */
static inline __u32 xfs_do_div(void *a, __u32 b, int n)
{
	__u32	mod;

	switch (n) {
		case 4:
			mod = *(__u32 *)a % b;
			*(__u32 *)a = *(__u32 *)a / b;
			return mod;
		case 8:
			{
			unsigned long __upper, __low, __high, __mod;
			__u64	c = *(__u64 *)a;
			__upper = __high = c >> 32;
			__low = c;
			if (__high) {
				__upper = __high % (b);
				__high = __high / (b);
			}
			asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (b), "0" (__low), "1" (__upper));
			asm("":"=A" (c):"a" (__low),"d" (__high));
			*(__u64 *)a = c;
			return __mod;
			}
	}

	/* NOTREACHED */
	return 0;
}

/* Side effect free 64 bit mod operation */
static inline __u32 xfs_do_mod(void *a, __u32 b, int n)
{
	switch (n) {
		case 4:
			return *(__u32 *)a % b;
		case 8:
			{
			unsigned long __upper, __low, __high, __mod;
			__u64	c = *(__u64 *)a;
			__upper = __high = c >> 32;
			__low = c;
			if (__high) {
				__upper = __high % (b);
				__high = __high / (b);
			}
			asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (b), "0" (__low), "1" (__upper));
			asm("":"=A" (c):"a" (__low),"d" (__high));
			return __mod;
			}
	}

	/* NOTREACHED */
	return 0;
}
#else
static inline __u32 xfs_do_div(void *a, __u32 b, int n)
{
	__u32	mod;

	switch (n) {
		case 4:
			mod = *(__u32 *)a % b;
			*(__u32 *)a = *(__u32 *)a / b;
			return mod;
		case 8:
			mod = do_div(*(__u64 *)a, b);
			return mod;
	}

	/* NOTREACHED */
	return 0;
}

/* Side effect free 64 bit mod operation */
static inline __u32 xfs_do_mod(void *a, __u32 b, int n)
{
	switch (n) {
		case 4:
			return *(__u32 *)a % b;
		case 8:
			{
			__u64	c = *(__u64 *)a;
			return do_div(c, b);
			}
	}

	/* NOTREACHED */
	return 0;
}
#endif

#undef do_div
#define do_div(a, b)	xfs_do_div(&(a), (b), sizeof(a))
#define do_mod(a, b)	xfs_do_mod(&(a), (b), sizeof(a))

static inline __uint64_t roundup_64(__uint64_t x, __uint32_t y)
{
	x += y - 1;
	do_div(x, y);
	return(x * y);
}

static inline __uint64_t howmany_64(__uint64_t x, __uint32_t y)
=======
static inline uint64_t rounddown_64(uint64_t x, uint32_t y)
{
	do_div(x, y);
	return x * y;
}

static inline uint64_t roundup_64(uint64_t x, uint32_t y)
{
	x += y - 1;
	do_div(x, y);
	return x * y;
}

static inline uint64_t howmany_64(uint64_t x, uint32_t y)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	x += y - 1;
	do_div(x, y);
	return x;
}

<<<<<<< HEAD
/* ARM old ABI has some weird alignment/padding */
#if defined(__arm__) && !defined(__ARM_EABI__)
#define __arch_pack __attribute__((packed))
#else
#define __arch_pack
#endif

#define ASSERT_ALWAYS(expr)	\
	(unlikely(expr) ? (void)0 : assfail(#expr, __FILE__, __LINE__))

#ifndef DEBUG
#define ASSERT(expr)	((void)0)

#ifndef STATIC
# define STATIC static noinline
#endif

#else /* DEBUG */

#define ASSERT(expr)	\
	(unlikely(expr) ? (void)0 : assfail(#expr, __FILE__, __LINE__))

#ifndef STATIC
# define STATIC noinline
#endif

#endif /* DEBUG */

=======
/* If @b is a power of 2, return log2(b).  Else return -1. */
static inline int8_t log2_if_power2(unsigned long b)
{
	return is_power_of_2(b) ? ilog2(b) : -1;
}

/* If @b is a power of 2, return a mask of the lower bits, else return zero. */
static inline unsigned long long mask64_if_power2(unsigned long b)
{
	return is_power_of_2(b) ? b - 1 : 0;
}

int xfs_rw_bdev(struct block_device *bdev, sector_t sector, unsigned int count,
		char *data, enum req_op op);

#define ASSERT_ALWAYS(expr)	\
	(likely(expr) ? (void)0 : assfail(NULL, #expr, __FILE__, __LINE__))

#ifdef DEBUG
#define ASSERT(expr)	\
	(likely(expr) ? (void)0 : assfail(NULL, #expr, __FILE__, __LINE__))

#else	/* !DEBUG */

#ifdef XFS_WARN

#define ASSERT(expr)	\
	(likely(expr) ? (void)0 : asswarn(NULL, #expr, __FILE__, __LINE__))

#else	/* !DEBUG && !XFS_WARN */

#define ASSERT(expr)		((void)0)

#endif /* XFS_WARN */
#endif /* DEBUG */

#define XFS_IS_CORRUPT(mp, expr)	\
	(unlikely(expr) ? xfs_corruption_error(#expr, XFS_ERRLEVEL_LOW, (mp), \
					       NULL, 0, __FILE__, __LINE__, \
					       __this_address), \
			  true : false)

#define STATIC static noinline

#ifdef CONFIG_XFS_RT

/*
 * make sure we ignore the inode flag if the filesystem doesn't have a
 * configured realtime device.
 */
#define XFS_IS_REALTIME_INODE(ip)			\
	(((ip)->i_diflags & XFS_DIFLAG_REALTIME) &&	\
	 (ip)->i_mount->m_rtdev_targp)
#define XFS_IS_REALTIME_MOUNT(mp) ((mp)->m_rtdev_targp ? 1 : 0)
#else
#define XFS_IS_REALTIME_INODE(ip) (0)
#define XFS_IS_REALTIME_MOUNT(mp) (0)
#endif

/*
 * Starting in Linux 4.15, the %p (raw pointer value) printk modifier
 * prints a hashed version of the pointer to avoid leaking kernel
 * pointers into dmesg.  If we're trying to debug the kernel we want the
 * raw values, so override this behavior as best we can.
 */
#ifdef DEBUG
# define PTR_FMT "%px"
#else
# define PTR_FMT "%p"
#endif

/*
 * Helper for IO routines to grab backing pages from allocated kernel memory.
 */
static inline struct page *
kmem_to_page(void *addr)
{
	if (is_vmalloc_addr(addr))
		return vmalloc_to_page(addr);
	return virt_to_page(addr);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __XFS_LINUX__ */
