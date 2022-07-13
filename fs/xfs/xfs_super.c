<<<<<<< HEAD
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
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
 */

#include "xfs.h"
#include "xfs_bit.h"
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_dir2.h"
#include "xfs_alloc.h"
#include "xfs_quota.h"
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_ialloc.h"
#include "xfs_bmap.h"
#include "xfs_rtalloc.h"
#include "xfs_error.h"
#include "xfs_itable.h"
#include "xfs_fsops.h"
#include "xfs_attr.h"
#include "xfs_buf_item.h"
#include "xfs_utils.h"
#include "xfs_vnodeops.h"
#include "xfs_log_priv.h"
#include "xfs_trans_priv.h"
#include "xfs_filestream.h"
#include "xfs_da_btree.h"
#include "xfs_extfree_item.h"
#include "xfs_mru_cache.h"
#include "xfs_inode_item.h"
#include "xfs_sync.h"
#include "xfs_trace.h"

#include <linux/namei.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mount.h>
#include <linux/mempool.h>
#include <linux/writeback.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/parser.h>

static const struct super_operations xfs_super_operations;
static kmem_zone_t *xfs_ioend_zone;
mempool_t *xfs_ioend_pool;

#define MNTOPT_LOGBUFS	"logbufs"	/* number of XFS log buffers */
#define MNTOPT_LOGBSIZE	"logbsize"	/* size of XFS log buffers */
#define MNTOPT_LOGDEV	"logdev"	/* log device */
#define MNTOPT_RTDEV	"rtdev"		/* realtime I/O device */
#define MNTOPT_BIOSIZE	"biosize"	/* log2 of preferred buffered io size */
#define MNTOPT_WSYNC	"wsync"		/* safe-mode nfs compatible mount */
#define MNTOPT_NOALIGN	"noalign"	/* turn off stripe alignment */
#define MNTOPT_SWALLOC	"swalloc"	/* turn on stripe width allocation */
#define MNTOPT_SUNIT	"sunit"		/* data volume stripe unit */
#define MNTOPT_SWIDTH	"swidth"	/* data volume stripe width */
#define MNTOPT_NOUUID	"nouuid"	/* ignore filesystem UUID */
#define MNTOPT_MTPT	"mtpt"		/* filesystem mount point */
#define MNTOPT_GRPID	"grpid"		/* group-ID from parent directory */
#define MNTOPT_NOGRPID	"nogrpid"	/* group-ID from current process */
#define MNTOPT_BSDGROUPS    "bsdgroups"    /* group-ID from parent directory */
#define MNTOPT_SYSVGROUPS   "sysvgroups"   /* group-ID from current process */
#define MNTOPT_ALLOCSIZE    "allocsize"    /* preferred allocation size */
#define MNTOPT_NORECOVERY   "norecovery"   /* don't run XFS recovery */
#define MNTOPT_BARRIER	"barrier"	/* use writer barriers for log write and
					 * unwritten extent conversion */
#define MNTOPT_NOBARRIER "nobarrier"	/* .. disable */
#define MNTOPT_64BITINODE   "inode64"	/* inodes can be allocated anywhere */
#define MNTOPT_IKEEP	"ikeep"		/* do not free empty inode clusters */
#define MNTOPT_NOIKEEP	"noikeep"	/* free empty inode clusters */
#define MNTOPT_LARGEIO	   "largeio"	/* report large I/O sizes in stat() */
#define MNTOPT_NOLARGEIO   "nolargeio"	/* do not report large I/O sizes
					 * in stat(). */
#define MNTOPT_ATTR2	"attr2"		/* do use attr2 attribute format */
#define MNTOPT_NOATTR2	"noattr2"	/* do not use attr2 attribute format */
#define MNTOPT_FILESTREAM  "filestreams" /* use filestreams allocator */
#define MNTOPT_QUOTA	"quota"		/* disk quotas (user) */
#define MNTOPT_NOQUOTA	"noquota"	/* no quotas */
#define MNTOPT_USRQUOTA	"usrquota"	/* user quota enabled */
#define MNTOPT_GRPQUOTA	"grpquota"	/* group quota enabled */
#define MNTOPT_PRJQUOTA	"prjquota"	/* project quota enabled */
#define MNTOPT_UQUOTA	"uquota"	/* user quota (IRIX variant) */
#define MNTOPT_GQUOTA	"gquota"	/* group quota (IRIX variant) */
#define MNTOPT_PQUOTA	"pquota"	/* project quota (IRIX variant) */
#define MNTOPT_UQUOTANOENF "uqnoenforce"/* user quota limit enforcement */
#define MNTOPT_GQUOTANOENF "gqnoenforce"/* group quota limit enforcement */
#define MNTOPT_PQUOTANOENF "pqnoenforce"/* project quota limit enforcement */
#define MNTOPT_QUOTANOENF  "qnoenforce"	/* same as uqnoenforce */
#define MNTOPT_DELAYLOG    "delaylog"	/* Delayed logging enabled */
#define MNTOPT_NODELAYLOG  "nodelaylog"	/* Delayed logging disabled */
#define MNTOPT_DISCARD	   "discard"	/* Discard unused blocks */
#define MNTOPT_NODISCARD   "nodiscard"	/* Do not discard unused blocks */

/*
 * Table driven mount option parser.
 *
 * Currently only used for remount, but it will be used for mount
 * in the future, too.
 */
enum {
	Opt_barrier, Opt_nobarrier, Opt_err
};

static const match_table_t tokens = {
	{Opt_barrier, "barrier"},
	{Opt_nobarrier, "nobarrier"},
	{Opt_err, NULL}
};


STATIC unsigned long
suffix_strtoul(char *s, char **endp, unsigned int base)
{
	int	last, shift_left_factor = 0;
	char	*value = s;

	last = strlen(value) - 1;
	if (value[last] == 'K' || value[last] == 'k') {
		shift_left_factor = 10;
		value[last] = '\0';
	}
	if (value[last] == 'M' || value[last] == 'm') {
		shift_left_factor = 20;
		value[last] = '\0';
	}
	if (value[last] == 'G' || value[last] == 'g') {
		shift_left_factor = 30;
		value[last] = '\0';
	}

	return simple_strtoul((const char *)s, endp, base) << shift_left_factor;
}

/*
 * This function fills in xfs_mount_t fields based on mount args.
 * Note: the superblock has _not_ yet been read in.
 *
 * Note that this function leaks the various device name allocations on
 * failure.  The caller takes care of them.
 */
STATIC int
xfs_parseargs(
	struct xfs_mount	*mp,
	char			*options)
{
	struct super_block	*sb = mp->m_super;
	char			*this_char, *value, *eov;
	int			dsunit = 0;
	int			dswidth = 0;
	int			iosize = 0;
	__uint8_t		iosizelog = 0;

	/*
	 * set up the mount name first so all the errors will refer to the
	 * correct device.
	 */
	mp->m_fsname = kstrndup(sb->s_id, MAXNAMELEN, GFP_KERNEL);
	if (!mp->m_fsname)
		return ENOMEM;
	mp->m_fsname_len = strlen(mp->m_fsname) + 1;

	/*
	 * Copy binary VFS mount flags we are interested in.
	 */
	if (sb->s_flags & MS_RDONLY)
		mp->m_flags |= XFS_MOUNT_RDONLY;
	if (sb->s_flags & MS_DIRSYNC)
		mp->m_flags |= XFS_MOUNT_DIRSYNC;
	if (sb->s_flags & MS_SYNCHRONOUS)
		mp->m_flags |= XFS_MOUNT_WSYNC;

	/*
	 * Set some default flags that could be cleared by the mount option
	 * parsing.
	 */
	mp->m_flags |= XFS_MOUNT_BARRIER;
	mp->m_flags |= XFS_MOUNT_COMPAT_IOSIZE;
	mp->m_flags |= XFS_MOUNT_SMALL_INUMS;

	/*
	 * These can be overridden by the mount option parsing.
	 */
	mp->m_logbufs = -1;
	mp->m_logbsize = -1;

	if (!options)
		goto done;

	while ((this_char = strsep(&options, ",")) != NULL) {
		if (!*this_char)
			continue;
		if ((value = strchr(this_char, '=')) != NULL)
			*value++ = 0;

		if (!strcmp(this_char, MNTOPT_LOGBUFS)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			mp->m_logbufs = simple_strtoul(value, &eov, 10);
		} else if (!strcmp(this_char, MNTOPT_LOGBSIZE)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			mp->m_logbsize = suffix_strtoul(value, &eov, 10);
		} else if (!strcmp(this_char, MNTOPT_LOGDEV)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			mp->m_logname = kstrndup(value, MAXNAMELEN, GFP_KERNEL);
			if (!mp->m_logname)
				return ENOMEM;
		} else if (!strcmp(this_char, MNTOPT_MTPT)) {
			xfs_warn(mp, "%s option not allowed on this system",
				this_char);
			return EINVAL;
		} else if (!strcmp(this_char, MNTOPT_RTDEV)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			mp->m_rtname = kstrndup(value, MAXNAMELEN, GFP_KERNEL);
			if (!mp->m_rtname)
				return ENOMEM;
		} else if (!strcmp(this_char, MNTOPT_BIOSIZE)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			iosize = simple_strtoul(value, &eov, 10);
			iosizelog = ffs(iosize) - 1;
		} else if (!strcmp(this_char, MNTOPT_ALLOCSIZE)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			iosize = suffix_strtoul(value, &eov, 10);
			iosizelog = ffs(iosize) - 1;
		} else if (!strcmp(this_char, MNTOPT_GRPID) ||
			   !strcmp(this_char, MNTOPT_BSDGROUPS)) {
			mp->m_flags |= XFS_MOUNT_GRPID;
		} else if (!strcmp(this_char, MNTOPT_NOGRPID) ||
			   !strcmp(this_char, MNTOPT_SYSVGROUPS)) {
			mp->m_flags &= ~XFS_MOUNT_GRPID;
		} else if (!strcmp(this_char, MNTOPT_WSYNC)) {
			mp->m_flags |= XFS_MOUNT_WSYNC;
		} else if (!strcmp(this_char, MNTOPT_NORECOVERY)) {
			mp->m_flags |= XFS_MOUNT_NORECOVERY;
		} else if (!strcmp(this_char, MNTOPT_NOALIGN)) {
			mp->m_flags |= XFS_MOUNT_NOALIGN;
		} else if (!strcmp(this_char, MNTOPT_SWALLOC)) {
			mp->m_flags |= XFS_MOUNT_SWALLOC;
		} else if (!strcmp(this_char, MNTOPT_SUNIT)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			dsunit = simple_strtoul(value, &eov, 10);
		} else if (!strcmp(this_char, MNTOPT_SWIDTH)) {
			if (!value || !*value) {
				xfs_warn(mp, "%s option requires an argument",
					this_char);
				return EINVAL;
			}
			dswidth = simple_strtoul(value, &eov, 10);
		} else if (!strcmp(this_char, MNTOPT_64BITINODE)) {
			mp->m_flags &= ~XFS_MOUNT_SMALL_INUMS;
#if !XFS_BIG_INUMS
			xfs_warn(mp, "%s option not allowed on this system",
				this_char);
			return EINVAL;
#endif
		} else if (!strcmp(this_char, MNTOPT_NOUUID)) {
			mp->m_flags |= XFS_MOUNT_NOUUID;
		} else if (!strcmp(this_char, MNTOPT_BARRIER)) {
			mp->m_flags |= XFS_MOUNT_BARRIER;
		} else if (!strcmp(this_char, MNTOPT_NOBARRIER)) {
			mp->m_flags &= ~XFS_MOUNT_BARRIER;
		} else if (!strcmp(this_char, MNTOPT_IKEEP)) {
			mp->m_flags |= XFS_MOUNT_IKEEP;
		} else if (!strcmp(this_char, MNTOPT_NOIKEEP)) {
			mp->m_flags &= ~XFS_MOUNT_IKEEP;
		} else if (!strcmp(this_char, MNTOPT_LARGEIO)) {
			mp->m_flags &= ~XFS_MOUNT_COMPAT_IOSIZE;
		} else if (!strcmp(this_char, MNTOPT_NOLARGEIO)) {
			mp->m_flags |= XFS_MOUNT_COMPAT_IOSIZE;
		} else if (!strcmp(this_char, MNTOPT_ATTR2)) {
			mp->m_flags |= XFS_MOUNT_ATTR2;
		} else if (!strcmp(this_char, MNTOPT_NOATTR2)) {
			mp->m_flags &= ~XFS_MOUNT_ATTR2;
			mp->m_flags |= XFS_MOUNT_NOATTR2;
		} else if (!strcmp(this_char, MNTOPT_FILESTREAM)) {
			mp->m_flags |= XFS_MOUNT_FILESTREAMS;
		} else if (!strcmp(this_char, MNTOPT_NOQUOTA)) {
			mp->m_qflags &= ~XFS_ALL_QUOTA_ACCT;
			mp->m_qflags &= ~XFS_ALL_QUOTA_ENFD;
			mp->m_qflags &= ~XFS_ALL_QUOTA_ACTIVE;
		} else if (!strcmp(this_char, MNTOPT_QUOTA) ||
			   !strcmp(this_char, MNTOPT_UQUOTA) ||
			   !strcmp(this_char, MNTOPT_USRQUOTA)) {
			mp->m_qflags |= (XFS_UQUOTA_ACCT | XFS_UQUOTA_ACTIVE |
					 XFS_UQUOTA_ENFD);
		} else if (!strcmp(this_char, MNTOPT_QUOTANOENF) ||
			   !strcmp(this_char, MNTOPT_UQUOTANOENF)) {
			mp->m_qflags |= (XFS_UQUOTA_ACCT | XFS_UQUOTA_ACTIVE);
			mp->m_qflags &= ~XFS_UQUOTA_ENFD;
		} else if (!strcmp(this_char, MNTOPT_PQUOTA) ||
			   !strcmp(this_char, MNTOPT_PRJQUOTA)) {
			mp->m_qflags |= (XFS_PQUOTA_ACCT | XFS_PQUOTA_ACTIVE |
					 XFS_OQUOTA_ENFD);
		} else if (!strcmp(this_char, MNTOPT_PQUOTANOENF)) {
			mp->m_qflags |= (XFS_PQUOTA_ACCT | XFS_PQUOTA_ACTIVE);
			mp->m_qflags &= ~XFS_OQUOTA_ENFD;
		} else if (!strcmp(this_char, MNTOPT_GQUOTA) ||
			   !strcmp(this_char, MNTOPT_GRPQUOTA)) {
			mp->m_qflags |= (XFS_GQUOTA_ACCT | XFS_GQUOTA_ACTIVE |
					 XFS_OQUOTA_ENFD);
		} else if (!strcmp(this_char, MNTOPT_GQUOTANOENF)) {
			mp->m_qflags |= (XFS_GQUOTA_ACCT | XFS_GQUOTA_ACTIVE);
			mp->m_qflags &= ~XFS_OQUOTA_ENFD;
		} else if (!strcmp(this_char, MNTOPT_DELAYLOG)) {
			xfs_warn(mp,
	"delaylog is the default now, option is deprecated.");
		} else if (!strcmp(this_char, MNTOPT_NODELAYLOG)) {
			xfs_warn(mp,
	"nodelaylog support has been removed, option is deprecated.");
		} else if (!strcmp(this_char, MNTOPT_DISCARD)) {
			mp->m_flags |= XFS_MOUNT_DISCARD;
		} else if (!strcmp(this_char, MNTOPT_NODISCARD)) {
			mp->m_flags &= ~XFS_MOUNT_DISCARD;
		} else if (!strcmp(this_char, "ihashsize")) {
			xfs_warn(mp,
	"ihashsize no longer used, option is deprecated.");
		} else if (!strcmp(this_char, "osyncisdsync")) {
			xfs_warn(mp,
	"osyncisdsync has no effect, option is deprecated.");
		} else if (!strcmp(this_char, "osyncisosync")) {
			xfs_warn(mp,
	"osyncisosync has no effect, option is deprecated.");
		} else if (!strcmp(this_char, "irixsgid")) {
			xfs_warn(mp,
	"irixsgid is now a sysctl(2) variable, option is deprecated.");
		} else {
			xfs_warn(mp, "unknown mount option [%s].", this_char);
			return EINVAL;
		}
	}

	/*
	 * no recovery flag requires a read-only mount
	 */
	if ((mp->m_flags & XFS_MOUNT_NORECOVERY) &&
	    !(mp->m_flags & XFS_MOUNT_RDONLY)) {
		xfs_warn(mp, "no-recovery mounts must be read-only.");
		return EINVAL;
	}

	if ((mp->m_flags & XFS_MOUNT_NOALIGN) && (dsunit || dswidth)) {
		xfs_warn(mp,
	"sunit and swidth options incompatible with the noalign option");
		return EINVAL;
	}

#ifndef CONFIG_XFS_QUOTA
	if (XFS_IS_QUOTA_RUNNING(mp)) {
		xfs_warn(mp, "quota support not available in this kernel.");
		return EINVAL;
	}
#endif

	if ((mp->m_qflags & (XFS_GQUOTA_ACCT | XFS_GQUOTA_ACTIVE)) &&
	    (mp->m_qflags & (XFS_PQUOTA_ACCT | XFS_PQUOTA_ACTIVE))) {
		xfs_warn(mp, "cannot mount with both project and group quota");
		return EINVAL;
	}

	if ((dsunit && !dswidth) || (!dsunit && dswidth)) {
		xfs_warn(mp, "sunit and swidth must be specified together");
		return EINVAL;
	}

	if (dsunit && (dswidth % dsunit != 0)) {
		xfs_warn(mp,
	"stripe width (%d) must be a multiple of the stripe unit (%d)",
			dswidth, dsunit);
		return EINVAL;
	}

done:
	if (!(mp->m_flags & XFS_MOUNT_NOALIGN)) {
		/*
		 * At this point the superblock has not been read
		 * in, therefore we do not know the block size.
		 * Before the mount call ends we will convert
		 * these to FSBs.
		 */
		if (dsunit) {
			mp->m_dalign = dsunit;
			mp->m_flags |= XFS_MOUNT_RETERR;
		}

		if (dswidth)
			mp->m_swidth = dswidth;
	}

	if (mp->m_logbufs != -1 &&
	    mp->m_logbufs != 0 &&
	    (mp->m_logbufs < XLOG_MIN_ICLOGS ||
	     mp->m_logbufs > XLOG_MAX_ICLOGS)) {
		xfs_warn(mp, "invalid logbufs value: %d [not %d-%d]",
			mp->m_logbufs, XLOG_MIN_ICLOGS, XLOG_MAX_ICLOGS);
		return XFS_ERROR(EINVAL);
	}
	if (mp->m_logbsize != -1 &&
	    mp->m_logbsize !=  0 &&
	    (mp->m_logbsize < XLOG_MIN_RECORD_BSIZE ||
	     mp->m_logbsize > XLOG_MAX_RECORD_BSIZE ||
	     !is_power_of_2(mp->m_logbsize))) {
		xfs_warn(mp,
			"invalid logbufsize: %d [not 16k,32k,64k,128k or 256k]",
			mp->m_logbsize);
		return XFS_ERROR(EINVAL);
	}

	if (iosizelog) {
		if (iosizelog > XFS_MAX_IO_LOG ||
		    iosizelog < XFS_MIN_IO_LOG) {
			xfs_warn(mp, "invalid log iosize: %d [not %d-%d]",
				iosizelog, XFS_MIN_IO_LOG,
				XFS_MAX_IO_LOG);
			return XFS_ERROR(EINVAL);
		}

		mp->m_flags |= XFS_MOUNT_DFLT_IOSIZE;
		mp->m_readio_log = iosizelog;
		mp->m_writeio_log = iosizelog;
	}

	return 0;
}

struct proc_xfs_info {
	int	flag;
	char	*str;
};

STATIC int
xfs_showargs(
	struct xfs_mount	*mp,
	struct seq_file		*m)
{
	static struct proc_xfs_info xfs_info_set[] = {
		/* the few simple ones we can get from the mount struct */
		{ XFS_MOUNT_IKEEP,		"," MNTOPT_IKEEP },
		{ XFS_MOUNT_WSYNC,		"," MNTOPT_WSYNC },
		{ XFS_MOUNT_NOALIGN,		"," MNTOPT_NOALIGN },
		{ XFS_MOUNT_SWALLOC,		"," MNTOPT_SWALLOC },
		{ XFS_MOUNT_NOUUID,		"," MNTOPT_NOUUID },
		{ XFS_MOUNT_NORECOVERY,		"," MNTOPT_NORECOVERY },
		{ XFS_MOUNT_ATTR2,		"," MNTOPT_ATTR2 },
		{ XFS_MOUNT_FILESTREAMS,	"," MNTOPT_FILESTREAM },
		{ XFS_MOUNT_GRPID,		"," MNTOPT_GRPID },
		{ XFS_MOUNT_DISCARD,		"," MNTOPT_DISCARD },
		{ 0, NULL }
	};
	static struct proc_xfs_info xfs_info_unset[] = {
		/* the few simple ones we can get from the mount struct */
		{ XFS_MOUNT_COMPAT_IOSIZE,	"," MNTOPT_LARGEIO },
		{ XFS_MOUNT_BARRIER,		"," MNTOPT_NOBARRIER },
		{ XFS_MOUNT_SMALL_INUMS,	"," MNTOPT_64BITINODE },
		{ 0, NULL }
	};
	struct proc_xfs_info	*xfs_infop;

	for (xfs_infop = xfs_info_set; xfs_infop->flag; xfs_infop++) {
		if (mp->m_flags & xfs_infop->flag)
			seq_puts(m, xfs_infop->str);
	}
	for (xfs_infop = xfs_info_unset; xfs_infop->flag; xfs_infop++) {
		if (!(mp->m_flags & xfs_infop->flag))
			seq_puts(m, xfs_infop->str);
	}

	if (mp->m_flags & XFS_MOUNT_DFLT_IOSIZE)
		seq_printf(m, "," MNTOPT_ALLOCSIZE "=%dk",
				(int)(1 << mp->m_writeio_log) >> 10);

	if (mp->m_logbufs > 0)
		seq_printf(m, "," MNTOPT_LOGBUFS "=%d", mp->m_logbufs);
	if (mp->m_logbsize > 0)
		seq_printf(m, "," MNTOPT_LOGBSIZE "=%dk", mp->m_logbsize >> 10);

	if (mp->m_logname)
		seq_show_option(m, MNTOPT_LOGDEV, mp->m_logname);
	if (mp->m_rtname)
		seq_show_option(m, MNTOPT_RTDEV, mp->m_rtname);

	if (mp->m_dalign > 0)
		seq_printf(m, "," MNTOPT_SUNIT "=%d",
				(int)XFS_FSB_TO_BB(mp, mp->m_dalign));
	if (mp->m_swidth > 0)
		seq_printf(m, "," MNTOPT_SWIDTH "=%d",
				(int)XFS_FSB_TO_BB(mp, mp->m_swidth));

	if (mp->m_qflags & (XFS_UQUOTA_ACCT|XFS_UQUOTA_ENFD))
		seq_puts(m, "," MNTOPT_USRQUOTA);
	else if (mp->m_qflags & XFS_UQUOTA_ACCT)
		seq_puts(m, "," MNTOPT_UQUOTANOENF);

	/* Either project or group quotas can be active, not both */

	if (mp->m_qflags & XFS_PQUOTA_ACCT) {
		if (mp->m_qflags & XFS_OQUOTA_ENFD)
			seq_puts(m, "," MNTOPT_PRJQUOTA);
		else
			seq_puts(m, "," MNTOPT_PQUOTANOENF);
	} else if (mp->m_qflags & XFS_GQUOTA_ACCT) {
		if (mp->m_qflags & XFS_OQUOTA_ENFD)
			seq_puts(m, "," MNTOPT_GRPQUOTA);
		else
			seq_puts(m, "," MNTOPT_GQUOTANOENF);
	}

	if (!(mp->m_qflags & XFS_ALL_QUOTA_ACCT))
		seq_puts(m, "," MNTOPT_NOQUOTA);

	return 0;
}
__uint64_t
xfs_max_file_offset(
	unsigned int		blockshift)
{
	unsigned int		pagefactor = 1;
	unsigned int		bitshift = BITS_PER_LONG - 1;

	/* Figure out maximum filesize, on Linux this can depend on
	 * the filesystem blocksize (on 32 bit platforms).
	 * __block_write_begin does this in an [unsigned] long...
	 *      page->index << (PAGE_CACHE_SHIFT - bbits)
	 * So, for page sized blocks (4K on 32 bit platforms),
	 * this wraps at around 8Tb (hence MAX_LFS_FILESIZE which is
	 *      (((u64)PAGE_CACHE_SIZE << (BITS_PER_LONG-1))-1)
	 * but for smaller blocksizes it is less (bbits = log2 bsize).
	 * Note1: get_block_t takes a long (implicit cast from above)
	 * Note2: The Large Block Device (LBD and HAVE_SECTOR_T) patch
	 * can optionally convert the [unsigned] long from above into
	 * an [unsigned] long long.
	 */

#if BITS_PER_LONG == 32
# if defined(CONFIG_LBDAF)
	ASSERT(sizeof(sector_t) == 8);
	pagefactor = PAGE_CACHE_SIZE;
	bitshift = BITS_PER_LONG;
# else
	pagefactor = PAGE_CACHE_SIZE >> (PAGE_CACHE_SHIFT - blockshift);
# endif
#endif

	return (((__uint64_t)pagefactor) << bitshift) - 1;
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
 */

#include "xfs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_sb.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_bmap.h"
#include "xfs_alloc.h"
#include "xfs_fsops.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"
#include "xfs_log.h"
#include "xfs_log_priv.h"
#include "xfs_dir2.h"
#include "xfs_extfree_item.h"
#include "xfs_mru_cache.h"
#include "xfs_inode_item.h"
#include "xfs_icache.h"
#include "xfs_trace.h"
#include "xfs_icreate_item.h"
#include "xfs_filestream.h"
#include "xfs_quota.h"
#include "xfs_sysfs.h"
#include "xfs_ondisk.h"
#include "xfs_rmap_item.h"
#include "xfs_refcount_item.h"
#include "xfs_bmap_item.h"
#include "xfs_reflink.h"
#include "xfs_pwork.h"
#include "xfs_ag.h"
#include "xfs_defer.h"
#include "xfs_attr_item.h"
#include "xfs_xattr.h"
#include "xfs_iunlink_item.h"
#include "xfs_dahash_test.h"
#include "xfs_rtbitmap.h"
#include "scrub/stats.h"
#include "scrub/rcbag_btree.h"

#include <linux/magic.h>
#include <linux/fs_context.h>
#include <linux/fs_parser.h>

static const struct super_operations xfs_super_operations;

static struct dentry *xfs_debugfs;	/* top-level xfs debugfs dir */
static struct kset *xfs_kset;		/* top-level xfs sysfs dir */
#ifdef DEBUG
static struct xfs_kobj xfs_dbg_kobj;	/* global debug sysfs attrs */
#endif

enum xfs_dax_mode {
	XFS_DAX_INODE = 0,
	XFS_DAX_ALWAYS = 1,
	XFS_DAX_NEVER = 2,
};

static void
xfs_mount_set_dax_mode(
	struct xfs_mount	*mp,
	enum xfs_dax_mode	mode)
{
	switch (mode) {
	case XFS_DAX_INODE:
		mp->m_features &= ~(XFS_FEAT_DAX_ALWAYS | XFS_FEAT_DAX_NEVER);
		break;
	case XFS_DAX_ALWAYS:
		mp->m_features |= XFS_FEAT_DAX_ALWAYS;
		mp->m_features &= ~XFS_FEAT_DAX_NEVER;
		break;
	case XFS_DAX_NEVER:
		mp->m_features |= XFS_FEAT_DAX_NEVER;
		mp->m_features &= ~XFS_FEAT_DAX_ALWAYS;
		break;
	}
}

static const struct constant_table dax_param_enums[] = {
	{"inode",	XFS_DAX_INODE },
	{"always",	XFS_DAX_ALWAYS },
	{"never",	XFS_DAX_NEVER },
	{}
};

/*
 * Table driven mount option parser.
 */
enum {
	Opt_logbufs, Opt_logbsize, Opt_logdev, Opt_rtdev,
	Opt_wsync, Opt_noalign, Opt_swalloc, Opt_sunit, Opt_swidth, Opt_nouuid,
	Opt_grpid, Opt_nogrpid, Opt_bsdgroups, Opt_sysvgroups,
	Opt_allocsize, Opt_norecovery, Opt_inode64, Opt_inode32, Opt_ikeep,
	Opt_noikeep, Opt_largeio, Opt_nolargeio, Opt_attr2, Opt_noattr2,
	Opt_filestreams, Opt_quota, Opt_noquota, Opt_usrquota, Opt_grpquota,
	Opt_prjquota, Opt_uquota, Opt_gquota, Opt_pquota,
	Opt_uqnoenforce, Opt_gqnoenforce, Opt_pqnoenforce, Opt_qnoenforce,
	Opt_discard, Opt_nodiscard, Opt_dax, Opt_dax_enum,
};

static const struct fs_parameter_spec xfs_fs_parameters[] = {
	fsparam_u32("logbufs",		Opt_logbufs),
	fsparam_string("logbsize",	Opt_logbsize),
	fsparam_string("logdev",	Opt_logdev),
	fsparam_string("rtdev",		Opt_rtdev),
	fsparam_flag("wsync",		Opt_wsync),
	fsparam_flag("noalign",		Opt_noalign),
	fsparam_flag("swalloc",		Opt_swalloc),
	fsparam_u32("sunit",		Opt_sunit),
	fsparam_u32("swidth",		Opt_swidth),
	fsparam_flag("nouuid",		Opt_nouuid),
	fsparam_flag("grpid",		Opt_grpid),
	fsparam_flag("nogrpid",		Opt_nogrpid),
	fsparam_flag("bsdgroups",	Opt_bsdgroups),
	fsparam_flag("sysvgroups",	Opt_sysvgroups),
	fsparam_string("allocsize",	Opt_allocsize),
	fsparam_flag("norecovery",	Opt_norecovery),
	fsparam_flag("inode64",		Opt_inode64),
	fsparam_flag("inode32",		Opt_inode32),
	fsparam_flag("ikeep",		Opt_ikeep),
	fsparam_flag("noikeep",		Opt_noikeep),
	fsparam_flag("largeio",		Opt_largeio),
	fsparam_flag("nolargeio",	Opt_nolargeio),
	fsparam_flag("attr2",		Opt_attr2),
	fsparam_flag("noattr2",		Opt_noattr2),
	fsparam_flag("filestreams",	Opt_filestreams),
	fsparam_flag("quota",		Opt_quota),
	fsparam_flag("noquota",		Opt_noquota),
	fsparam_flag("usrquota",	Opt_usrquota),
	fsparam_flag("grpquota",	Opt_grpquota),
	fsparam_flag("prjquota",	Opt_prjquota),
	fsparam_flag("uquota",		Opt_uquota),
	fsparam_flag("gquota",		Opt_gquota),
	fsparam_flag("pquota",		Opt_pquota),
	fsparam_flag("uqnoenforce",	Opt_uqnoenforce),
	fsparam_flag("gqnoenforce",	Opt_gqnoenforce),
	fsparam_flag("pqnoenforce",	Opt_pqnoenforce),
	fsparam_flag("qnoenforce",	Opt_qnoenforce),
	fsparam_flag("discard",		Opt_discard),
	fsparam_flag("nodiscard",	Opt_nodiscard),
	fsparam_flag("dax",		Opt_dax),
	fsparam_enum("dax",		Opt_dax_enum, dax_param_enums),
	{}
};

struct proc_xfs_info {
	uint64_t	flag;
	char		*str;
};

static int
xfs_fs_show_options(
	struct seq_file		*m,
	struct dentry		*root)
{
	static struct proc_xfs_info xfs_info_set[] = {
		/* the few simple ones we can get from the mount struct */
		{ XFS_FEAT_IKEEP,		",ikeep" },
		{ XFS_FEAT_WSYNC,		",wsync" },
		{ XFS_FEAT_NOALIGN,		",noalign" },
		{ XFS_FEAT_SWALLOC,		",swalloc" },
		{ XFS_FEAT_NOUUID,		",nouuid" },
		{ XFS_FEAT_NORECOVERY,		",norecovery" },
		{ XFS_FEAT_ATTR2,		",attr2" },
		{ XFS_FEAT_FILESTREAMS,		",filestreams" },
		{ XFS_FEAT_GRPID,		",grpid" },
		{ XFS_FEAT_DISCARD,		",discard" },
		{ XFS_FEAT_LARGE_IOSIZE,	",largeio" },
		{ XFS_FEAT_DAX_ALWAYS,		",dax=always" },
		{ XFS_FEAT_DAX_NEVER,		",dax=never" },
		{ 0, NULL }
	};
	struct xfs_mount	*mp = XFS_M(root->d_sb);
	struct proc_xfs_info	*xfs_infop;

	for (xfs_infop = xfs_info_set; xfs_infop->flag; xfs_infop++) {
		if (mp->m_features & xfs_infop->flag)
			seq_puts(m, xfs_infop->str);
	}

	seq_printf(m, ",inode%d", xfs_has_small_inums(mp) ? 32 : 64);

	if (xfs_has_allocsize(mp))
		seq_printf(m, ",allocsize=%dk",
			   (1 << mp->m_allocsize_log) >> 10);

	if (mp->m_logbufs > 0)
		seq_printf(m, ",logbufs=%d", mp->m_logbufs);
	if (mp->m_logbsize > 0)
		seq_printf(m, ",logbsize=%dk", mp->m_logbsize >> 10);

	if (mp->m_logname)
		seq_show_option(m, "logdev", mp->m_logname);
	if (mp->m_rtname)
		seq_show_option(m, "rtdev", mp->m_rtname);

	if (mp->m_dalign > 0)
		seq_printf(m, ",sunit=%d",
				(int)XFS_FSB_TO_BB(mp, mp->m_dalign));
	if (mp->m_swidth > 0)
		seq_printf(m, ",swidth=%d",
				(int)XFS_FSB_TO_BB(mp, mp->m_swidth));

	if (mp->m_qflags & XFS_UQUOTA_ENFD)
		seq_puts(m, ",usrquota");
	else if (mp->m_qflags & XFS_UQUOTA_ACCT)
		seq_puts(m, ",uqnoenforce");

	if (mp->m_qflags & XFS_PQUOTA_ENFD)
		seq_puts(m, ",prjquota");
	else if (mp->m_qflags & XFS_PQUOTA_ACCT)
		seq_puts(m, ",pqnoenforce");

	if (mp->m_qflags & XFS_GQUOTA_ENFD)
		seq_puts(m, ",grpquota");
	else if (mp->m_qflags & XFS_GQUOTA_ACCT)
		seq_puts(m, ",gqnoenforce");

	if (!(mp->m_qflags & XFS_ALL_QUOTA_ACCT))
		seq_puts(m, ",noquota");

	return 0;
}

static bool
xfs_set_inode_alloc_perag(
	struct xfs_perag	*pag,
	xfs_ino_t		ino,
	xfs_agnumber_t		max_metadata)
{
	if (!xfs_is_inode32(pag->pag_mount)) {
		set_bit(XFS_AGSTATE_ALLOWS_INODES, &pag->pag_opstate);
		clear_bit(XFS_AGSTATE_PREFERS_METADATA, &pag->pag_opstate);
		return false;
	}

	if (ino > XFS_MAXINUMBER_32) {
		clear_bit(XFS_AGSTATE_ALLOWS_INODES, &pag->pag_opstate);
		clear_bit(XFS_AGSTATE_PREFERS_METADATA, &pag->pag_opstate);
		return false;
	}

	set_bit(XFS_AGSTATE_ALLOWS_INODES, &pag->pag_opstate);
	if (pag->pag_agno < max_metadata)
		set_bit(XFS_AGSTATE_PREFERS_METADATA, &pag->pag_opstate);
	else
		clear_bit(XFS_AGSTATE_PREFERS_METADATA, &pag->pag_opstate);
	return true;
}

/*
 * Set parameters for inode allocation heuristics, taking into account
 * filesystem size and inode32/inode64 mount options; i.e. specifically
 * whether or not XFS_FEAT_SMALL_INUMS is set.
 *
 * Inode allocation patterns are altered only if inode32 is requested
 * (XFS_FEAT_SMALL_INUMS), and the filesystem is sufficiently large.
 * If altered, XFS_OPSTATE_INODE32 is set as well.
 *
 * An agcount independent of that in the mount structure is provided
 * because in the growfs case, mp->m_sb.sb_agcount is not yet updated
 * to the potentially higher ag count.
 *
 * Returns the maximum AG index which may contain inodes.
 */
xfs_agnumber_t
xfs_set_inode_alloc(
	struct xfs_mount *mp,
	xfs_agnumber_t	agcount)
{
	xfs_agnumber_t	index;
	xfs_agnumber_t	maxagi = 0;
	xfs_sb_t	*sbp = &mp->m_sb;
	xfs_agnumber_t	max_metadata;
	xfs_agino_t	agino;
	xfs_ino_t	ino;

	/*
	 * Calculate how much should be reserved for inodes to meet
	 * the max inode percentage.  Used only for inode32.
	 */
	if (M_IGEO(mp)->maxicount) {
		uint64_t	icount;

		icount = sbp->sb_dblocks * sbp->sb_imax_pct;
		do_div(icount, 100);
		icount += sbp->sb_agblocks - 1;
		do_div(icount, sbp->sb_agblocks);
		max_metadata = icount;
	} else {
		max_metadata = agcount;
	}

	/* Get the last possible inode in the filesystem */
	agino =	XFS_AGB_TO_AGINO(mp, sbp->sb_agblocks - 1);
	ino = XFS_AGINO_TO_INO(mp, agcount - 1, agino);

	/*
	 * If user asked for no more than 32-bit inodes, and the fs is
	 * sufficiently large, set XFS_OPSTATE_INODE32 if we must alter
	 * the allocator to accommodate the request.
	 */
	if (xfs_has_small_inums(mp) && ino > XFS_MAXINUMBER_32)
		set_bit(XFS_OPSTATE_INODE32, &mp->m_opstate);
	else
		clear_bit(XFS_OPSTATE_INODE32, &mp->m_opstate);

	for (index = 0; index < agcount; index++) {
		struct xfs_perag	*pag;

		ino = XFS_AGINO_TO_INO(mp, index, agino);

		pag = xfs_perag_get(mp, index);
		if (xfs_set_inode_alloc_perag(pag, ino, max_metadata))
			maxagi++;
		xfs_perag_put(pag);
	}

	return xfs_is_inode32(mp) ? maxagi : agcount;
}

static int
xfs_setup_dax_always(
	struct xfs_mount	*mp)
{
	if (!mp->m_ddev_targp->bt_daxdev &&
	    (!mp->m_rtdev_targp || !mp->m_rtdev_targp->bt_daxdev)) {
		xfs_alert(mp,
			"DAX unsupported by block device. Turning off DAX.");
		goto disable_dax;
	}

	if (mp->m_super->s_blocksize != PAGE_SIZE) {
		xfs_alert(mp,
			"DAX not supported for blocksize. Turning off DAX.");
		goto disable_dax;
	}

	if (xfs_has_reflink(mp) &&
	    bdev_is_partition(mp->m_ddev_targp->bt_bdev)) {
		xfs_alert(mp,
			"DAX and reflink cannot work with multi-partitions!");
		return -EINVAL;
	}

	return 0;

disable_dax:
	xfs_mount_set_dax_mode(mp, XFS_DAX_NEVER);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_blkdev_get(
	xfs_mount_t		*mp,
	const char		*name,
<<<<<<< HEAD
	struct block_device	**bdevp)
{
	int			error = 0;

	*bdevp = blkdev_get_by_path(name, FMODE_READ|FMODE_WRITE|FMODE_EXCL,
				    mp);
	if (IS_ERR(*bdevp)) {
		error = PTR_ERR(*bdevp);
		xfs_warn(mp, "Invalid device [%s], error=%d\n", name, error);
	}

	return -error;
}

STATIC void
xfs_blkdev_put(
	struct block_device	*bdev)
{
	if (bdev)
		blkdev_put(bdev, FMODE_READ|FMODE_WRITE|FMODE_EXCL);
}

void
xfs_blkdev_issue_flush(
	xfs_buftarg_t		*buftarg)
{
	blkdev_issue_flush(buftarg->bt_bdev, GFP_KERNEL, NULL);
}

STATIC void
xfs_close_devices(
	struct xfs_mount	*mp)
{
	if (mp->m_logdev_targp && mp->m_logdev_targp != mp->m_ddev_targp) {
		struct block_device *logdev = mp->m_logdev_targp->bt_bdev;
		xfs_free_buftarg(mp, mp->m_logdev_targp);
		xfs_blkdev_put(logdev);
	}
	if (mp->m_rtdev_targp) {
		struct block_device *rtdev = mp->m_rtdev_targp->bt_bdev;
		xfs_free_buftarg(mp, mp->m_rtdev_targp);
		xfs_blkdev_put(rtdev);
	}
	xfs_free_buftarg(mp, mp->m_ddev_targp);
=======
	struct file		**bdev_filep)
{
	int			error = 0;

	*bdev_filep = bdev_file_open_by_path(name,
		BLK_OPEN_READ | BLK_OPEN_WRITE | BLK_OPEN_RESTRICT_WRITES,
		mp->m_super, &fs_holder_ops);
	if (IS_ERR(*bdev_filep)) {
		error = PTR_ERR(*bdev_filep);
		*bdev_filep = NULL;
		xfs_warn(mp, "Invalid device [%s], error=%d", name, error);
	}

	return error;
}

STATIC void
xfs_shutdown_devices(
	struct xfs_mount	*mp)
{
	/*
	 * Udev is triggered whenever anyone closes a block device or unmounts
	 * a file systemm on a block device.
	 * The default udev rules invoke blkid to read the fs super and create
	 * symlinks to the bdev under /dev/disk.  For this, it uses buffered
	 * reads through the page cache.
	 *
	 * xfs_db also uses buffered reads to examine metadata.  There is no
	 * coordination between xfs_db and udev, which means that they can run
	 * concurrently.  Note there is no coordination between the kernel and
	 * blkid either.
	 *
	 * On a system with 64k pages, the page cache can cache the superblock
	 * and the root inode (and hence the root directory) with the same 64k
	 * page.  If udev spawns blkid after the mkfs and the system is busy
	 * enough that it is still running when xfs_db starts up, they'll both
	 * read from the same page in the pagecache.
	 *
	 * The unmount writes updated inode metadata to disk directly.  The XFS
	 * buffer cache does not use the bdev pagecache, so it needs to
	 * invalidate that pagecache on unmount.  If the above scenario occurs,
	 * the pagecache no longer reflects what's on disk, xfs_db reads the
	 * stale metadata, and fails to find /a.  Most of the time this succeeds
	 * because closing a bdev invalidates the page cache, but when processes
	 * race, everyone loses.
	 */
	if (mp->m_logdev_targp && mp->m_logdev_targp != mp->m_ddev_targp) {
		blkdev_issue_flush(mp->m_logdev_targp->bt_bdev);
		invalidate_bdev(mp->m_logdev_targp->bt_bdev);
	}
	if (mp->m_rtdev_targp) {
		blkdev_issue_flush(mp->m_rtdev_targp->bt_bdev);
		invalidate_bdev(mp->m_rtdev_targp->bt_bdev);
	}
	blkdev_issue_flush(mp->m_ddev_targp->bt_bdev);
	invalidate_bdev(mp->m_ddev_targp->bt_bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The file system configurations are:
 *	(1) device (partition) with data and internal log
 *	(2) logical volume with data and log subvolumes.
 *	(3) logical volume with data, log, and realtime subvolumes.
 *
 * We only have to handle opening the log and realtime volumes here if
 * they are present.  The data subvolume has already been opened by
 * get_sb_bdev() and is stored in sb->s_bdev.
 */
STATIC int
xfs_open_devices(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	struct block_device	*ddev = mp->m_super->s_bdev;
	struct block_device	*logdev = NULL, *rtdev = NULL;
=======
	struct super_block	*sb = mp->m_super;
	struct block_device	*ddev = sb->s_bdev;
	struct file		*logdev_file = NULL, *rtdev_file = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			error;

	/*
	 * Open real time and log devices - order is important.
	 */
	if (mp->m_logname) {
<<<<<<< HEAD
		error = xfs_blkdev_get(mp, mp->m_logname, &logdev);
		if (error)
			goto out;
	}

	if (mp->m_rtname) {
		error = xfs_blkdev_get(mp, mp->m_rtname, &rtdev);
		if (error)
			goto out_close_logdev;

		if (rtdev == ddev || rtdev == logdev) {
			xfs_warn(mp,
	"Cannot mount filesystem with identical rtdev and ddev/logdev.");
			error = EINVAL;
=======
		error = xfs_blkdev_get(mp, mp->m_logname, &logdev_file);
		if (error)
			return error;
	}

	if (mp->m_rtname) {
		error = xfs_blkdev_get(mp, mp->m_rtname, &rtdev_file);
		if (error)
			goto out_close_logdev;

		if (file_bdev(rtdev_file) == ddev ||
		    (logdev_file &&
		     file_bdev(rtdev_file) == file_bdev(logdev_file))) {
			xfs_warn(mp,
	"Cannot mount filesystem with identical rtdev and ddev/logdev.");
			error = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_close_rtdev;
		}
	}

	/*
	 * Setup xfs_mount buffer target pointers
	 */
<<<<<<< HEAD
	error = ENOMEM;
	mp->m_ddev_targp = xfs_alloc_buftarg(mp, ddev, 0, mp->m_fsname);
	if (!mp->m_ddev_targp)
		goto out_close_rtdev;

	if (rtdev) {
		mp->m_rtdev_targp = xfs_alloc_buftarg(mp, rtdev, 1,
							mp->m_fsname);
=======
	error = -ENOMEM;
	mp->m_ddev_targp = xfs_alloc_buftarg(mp, sb->s_bdev_file);
	if (!mp->m_ddev_targp)
		goto out_close_rtdev;

	if (rtdev_file) {
		mp->m_rtdev_targp = xfs_alloc_buftarg(mp, rtdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!mp->m_rtdev_targp)
			goto out_free_ddev_targ;
	}

<<<<<<< HEAD
	if (logdev && logdev != ddev) {
		mp->m_logdev_targp = xfs_alloc_buftarg(mp, logdev, 1,
							mp->m_fsname);
=======
	if (logdev_file && file_bdev(logdev_file) != ddev) {
		mp->m_logdev_targp = xfs_alloc_buftarg(mp, logdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!mp->m_logdev_targp)
			goto out_free_rtdev_targ;
	} else {
		mp->m_logdev_targp = mp->m_ddev_targp;
<<<<<<< HEAD
=======
		/* Handle won't be used, drop it */
		if (logdev_file)
			bdev_fput(logdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

 out_free_rtdev_targ:
	if (mp->m_rtdev_targp)
<<<<<<< HEAD
		xfs_free_buftarg(mp, mp->m_rtdev_targp);
 out_free_ddev_targ:
	xfs_free_buftarg(mp, mp->m_ddev_targp);
 out_close_rtdev:
	if (rtdev)
		xfs_blkdev_put(rtdev);
 out_close_logdev:
	if (logdev && logdev != ddev)
		xfs_blkdev_put(logdev);
 out:
=======
		xfs_free_buftarg(mp->m_rtdev_targp);
 out_free_ddev_targ:
	xfs_free_buftarg(mp->m_ddev_targp);
 out_close_rtdev:
	 if (rtdev_file)
		bdev_fput(rtdev_file);
 out_close_logdev:
	if (logdev_file)
		bdev_fput(logdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Setup xfs_mount buffer target pointers based on superblock
 */
STATIC int
xfs_setup_devices(
	struct xfs_mount	*mp)
{
	int			error;

<<<<<<< HEAD
	error = xfs_setsize_buftarg(mp->m_ddev_targp, mp->m_sb.sb_blocksize,
				    mp->m_sb.sb_sectsize);
=======
	error = xfs_setsize_buftarg(mp->m_ddev_targp, mp->m_sb.sb_sectsize);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;

	if (mp->m_logdev_targp && mp->m_logdev_targp != mp->m_ddev_targp) {
		unsigned int	log_sector_size = BBSIZE;

<<<<<<< HEAD
		if (xfs_sb_version_hassector(&mp->m_sb))
			log_sector_size = mp->m_sb.sb_logsectsize;
		error = xfs_setsize_buftarg(mp->m_logdev_targp,
					    mp->m_sb.sb_blocksize,
=======
		if (xfs_has_sector(mp))
			log_sector_size = mp->m_sb.sb_logsectsize;
		error = xfs_setsize_buftarg(mp->m_logdev_targp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    log_sector_size);
		if (error)
			return error;
	}
	if (mp->m_rtdev_targp) {
		error = xfs_setsize_buftarg(mp->m_rtdev_targp,
<<<<<<< HEAD
					    mp->m_sb.sb_blocksize,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					    mp->m_sb.sb_sectsize);
		if (error)
			return error;
	}

	return 0;
}

STATIC int
xfs_init_mount_workqueues(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	mp->m_data_workqueue = alloc_workqueue("xfs-data/%s",
			WQ_MEM_RECLAIM, 0, mp->m_fsname);
	if (!mp->m_data_workqueue)
		goto out;

	mp->m_unwritten_workqueue = alloc_workqueue("xfs-conv/%s",
			WQ_MEM_RECLAIM, 0, mp->m_fsname);
	if (!mp->m_unwritten_workqueue)
		goto out_destroy_data_iodone_queue;

	return 0;

out_destroy_data_iodone_queue:
	destroy_workqueue(mp->m_data_workqueue);
=======
	mp->m_buf_workqueue = alloc_workqueue("xfs-buf/%s",
			XFS_WQFLAGS(WQ_FREEZABLE | WQ_MEM_RECLAIM),
			1, mp->m_super->s_id);
	if (!mp->m_buf_workqueue)
		goto out;

	mp->m_unwritten_workqueue = alloc_workqueue("xfs-conv/%s",
			XFS_WQFLAGS(WQ_FREEZABLE | WQ_MEM_RECLAIM),
			0, mp->m_super->s_id);
	if (!mp->m_unwritten_workqueue)
		goto out_destroy_buf;

	mp->m_reclaim_workqueue = alloc_workqueue("xfs-reclaim/%s",
			XFS_WQFLAGS(WQ_FREEZABLE | WQ_MEM_RECLAIM),
			0, mp->m_super->s_id);
	if (!mp->m_reclaim_workqueue)
		goto out_destroy_unwritten;

	mp->m_blockgc_wq = alloc_workqueue("xfs-blockgc/%s",
			XFS_WQFLAGS(WQ_UNBOUND | WQ_FREEZABLE | WQ_MEM_RECLAIM),
			0, mp->m_super->s_id);
	if (!mp->m_blockgc_wq)
		goto out_destroy_reclaim;

	mp->m_inodegc_wq = alloc_workqueue("xfs-inodegc/%s",
			XFS_WQFLAGS(WQ_FREEZABLE | WQ_MEM_RECLAIM),
			1, mp->m_super->s_id);
	if (!mp->m_inodegc_wq)
		goto out_destroy_blockgc;

	mp->m_sync_workqueue = alloc_workqueue("xfs-sync/%s",
			XFS_WQFLAGS(WQ_FREEZABLE), 0, mp->m_super->s_id);
	if (!mp->m_sync_workqueue)
		goto out_destroy_inodegc;

	return 0;

out_destroy_inodegc:
	destroy_workqueue(mp->m_inodegc_wq);
out_destroy_blockgc:
	destroy_workqueue(mp->m_blockgc_wq);
out_destroy_reclaim:
	destroy_workqueue(mp->m_reclaim_workqueue);
out_destroy_unwritten:
	destroy_workqueue(mp->m_unwritten_workqueue);
out_destroy_buf:
	destroy_workqueue(mp->m_buf_workqueue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return -ENOMEM;
}

STATIC void
xfs_destroy_mount_workqueues(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	destroy_workqueue(mp->m_data_workqueue);
	destroy_workqueue(mp->m_unwritten_workqueue);
=======
	destroy_workqueue(mp->m_sync_workqueue);
	destroy_workqueue(mp->m_blockgc_wq);
	destroy_workqueue(mp->m_inodegc_wq);
	destroy_workqueue(mp->m_reclaim_workqueue);
	destroy_workqueue(mp->m_unwritten_workqueue);
	destroy_workqueue(mp->m_buf_workqueue);
}

static void
xfs_flush_inodes_worker(
	struct work_struct	*work)
{
	struct xfs_mount	*mp = container_of(work, struct xfs_mount,
						   m_flush_inodes_work);
	struct super_block	*sb = mp->m_super;

	if (down_read_trylock(&sb->s_umount)) {
		sync_inodes_sb(sb);
		up_read(&sb->s_umount);
	}
}

/*
 * Flush all dirty data to disk. Must not be called while holding an XFS_ILOCK
 * or a page lock. We use sync_inodes_sb() here to ensure we block while waiting
 * for IO to complete so that we effectively throttle multiple callers to the
 * rate at which IO is completing.
 */
void
xfs_flush_inodes(
	struct xfs_mount	*mp)
{
	/*
	 * If flush_work() returns true then that means we waited for a flush
	 * which was already in progress.  Don't bother running another scan.
	 */
	if (flush_work(&mp->m_flush_inodes_work))
		return;

	queue_work(mp->m_sync_workqueue, &mp->m_flush_inodes_work);
	flush_work(&mp->m_flush_inodes_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Catch misguided souls that try to use this interface on XFS */
STATIC struct inode *
xfs_fs_alloc_inode(
	struct super_block	*sb)
{
	BUG();
	return NULL;
}

/*
 * Now that the generic code is guaranteed not to be accessing
<<<<<<< HEAD
 * the linux inode, we can reclaim the inode.
=======
 * the linux inode, we can inactivate and reclaim the inode.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
STATIC void
xfs_fs_destroy_inode(
	struct inode		*inode)
{
	struct xfs_inode	*ip = XFS_I(inode);

	trace_xfs_destroy_inode(ip);

<<<<<<< HEAD
	XFS_STATS_INC(vn_reclaim);

	/* bad inode, get out here ASAP */
	if (is_bad_inode(inode))
		goto out_reclaim;

	ASSERT(XFS_FORCED_SHUTDOWN(ip->i_mount) || ip->i_delayed_blks == 0);

	/*
	 * We should never get here with one of the reclaim flags already set.
	 */
	ASSERT_ALWAYS(!xfs_iflags_test(ip, XFS_IRECLAIMABLE));
	ASSERT_ALWAYS(!xfs_iflags_test(ip, XFS_IRECLAIM));

	/*
	 * We always use background reclaim here because even if the
	 * inode is clean, it still may be under IO and hence we have
	 * to take the flush lock. The background reclaim path handles
	 * this more efficiently than we can here, so simply let background
	 * reclaim tear down all inodes.
	 */
out_reclaim:
	xfs_inode_set_reclaim_tag(ip);
=======
	ASSERT(!rwsem_is_locked(&inode->i_rwsem));
	XFS_STATS_INC(ip->i_mount, vn_rele);
	XFS_STATS_INC(ip->i_mount, vn_remove);
	xfs_inode_mark_reclaimable(ip);
}

static void
xfs_fs_dirty_inode(
	struct inode			*inode,
	int				flags)
{
	struct xfs_inode		*ip = XFS_I(inode);
	struct xfs_mount		*mp = ip->i_mount;
	struct xfs_trans		*tp;

	if (!(inode->i_sb->s_flags & SB_LAZYTIME))
		return;

	/*
	 * Only do the timestamp update if the inode is dirty (I_DIRTY_SYNC)
	 * and has dirty timestamp (I_DIRTY_TIME). I_DIRTY_TIME can be passed
	 * in flags possibly together with I_DIRTY_SYNC.
	 */
	if ((flags & ~I_DIRTY_TIME) != I_DIRTY_SYNC || !(flags & I_DIRTY_TIME))
		return;

	if (xfs_trans_alloc(mp, &M_RES(mp)->tr_fsyncts, 0, 0, 0, &tp))
		return;
	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_TIMESTAMP);
	xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Slab object creation initialisation for the XFS inode.
 * This covers only the idempotent fields in the XFS inode;
 * all other fields need to be initialised on allocation
 * from the slab. This avoids the need to repeatedly initialise
 * fields in the xfs inode that left in the initialise state
 * when freeing the inode.
 */
STATIC void
xfs_fs_inode_init_once(
	void			*inode)
{
	struct xfs_inode	*ip = inode;

	memset(ip, 0, sizeof(struct xfs_inode));

	/* vfs inode */
	inode_init_once(VFS_I(ip));

	/* xfs inode */
	atomic_set(&ip->i_pincount, 0);
	spin_lock_init(&ip->i_flags_lock);
<<<<<<< HEAD

	mrlock_init(&ip->i_lock, MRLOCK_ALLOW_EQUAL_PRI|MRLOCK_BARRIER,
		     "xfsino", ip->i_ino);
}

/*
 * This is called by the VFS when dirtying inode metadata.  This can happen
 * for a few reasons, but we only care about timestamp updates, given that
 * we handled the rest ourselves.  In theory no other calls should happen,
 * but for example generic_write_end() keeps dirtying the inode after
 * updating i_size.  Thus we check that the flags are exactly I_DIRTY_SYNC,
 * and skip this call otherwise.
 *
 * We'll hopefull get a different method just for updating timestamps soon,
 * at which point this hack can go away, and maybe we'll also get real
 * error handling here.
 */
STATIC void
xfs_fs_dirty_inode(
	struct inode		*inode,
	int			flags)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	int			error;

	if (flags != I_DIRTY_SYNC)
		return;

	trace_xfs_dirty_inode(ip);

	tp = xfs_trans_alloc(mp, XFS_TRANS_FSYNC_TS);
	error = xfs_trans_reserve(tp, 0, XFS_FSYNC_TS_LOG_RES(mp), 0, 0, 0);
	if (error) {
		xfs_trans_cancel(tp, 0);
		goto trouble;
	}
	xfs_ilock(ip, XFS_ILOCK_EXCL);
	/*
	 * Grab all the latest timestamps from the Linux inode.
	 */
	ip->i_d.di_atime.t_sec = (__int32_t)inode->i_atime.tv_sec;
	ip->i_d.di_atime.t_nsec = (__int32_t)inode->i_atime.tv_nsec;
	ip->i_d.di_ctime.t_sec = (__int32_t)inode->i_ctime.tv_sec;
	ip->i_d.di_ctime.t_nsec = (__int32_t)inode->i_ctime.tv_nsec;
	ip->i_d.di_mtime.t_sec = (__int32_t)inode->i_mtime.tv_sec;
	ip->i_d.di_mtime.t_nsec = (__int32_t)inode->i_mtime.tv_nsec;

	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_TIMESTAMP);
	error = xfs_trans_commit(tp, 0);
	if (error)
		goto trouble;
	return;

trouble:
	xfs_warn(mp, "failed to update timestamps for inode 0x%llx", ip->i_ino);
}

STATIC void
xfs_fs_evict_inode(
	struct inode		*inode)
{
	xfs_inode_t		*ip = XFS_I(inode);

	trace_xfs_evict_inode(ip);

	truncate_inode_pages(&inode->i_data, 0);
	end_writeback(inode);
	XFS_STATS_INC(vn_rele);
	XFS_STATS_INC(vn_remove);
	XFS_STATS_DEC(vn_active);

	/*
	 * The iolock is used by the file system to coordinate reads,
	 * writes, and block truncates.  Up to this point the lock
	 * protected concurrent accesses by users of the inode.  But
	 * from here forward we're doing some final processing of the
	 * inode because we're done with it, and although we reuse the
	 * iolock for protection it is really a distinct lock class
	 * (in the lockdep sense) from before.  To keep lockdep happy
	 * (and basically indicate what we are doing), we explicitly
	 * re-init the iolock here.
	 */
	ASSERT(!rwsem_is_locked(&ip->i_iolock.mr_lock));
	mrlock_init(&ip->i_iolock, MRLOCK_BARRIER, "xfsio", ip->i_ino);
	lockdep_set_class_and_name(&ip->i_iolock.mr_lock,
			&xfs_iolock_reclaimable, "xfs_iolock_reclaimable");

	xfs_inactive(ip);
=======
	init_rwsem(&ip->i_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We do an unlocked check for XFS_IDONTCACHE here because we are already
 * serialised against cache hits here via the inode->i_lock and igrab() in
 * xfs_iget_cache_hit(). Hence a lookup that might clear this flag will not be
 * racing with us, and it avoids needing to grab a spinlock here for every inode
 * we drop the final reference on.
 */
STATIC int
xfs_fs_drop_inode(
	struct inode		*inode)
{
	struct xfs_inode	*ip = XFS_I(inode);

<<<<<<< HEAD
	return generic_drop_inode(inode) || (ip->i_flags & XFS_IDONTCACHE);
}

STATIC void
xfs_free_fsname(
	struct xfs_mount	*mp)
{
	kfree(mp->m_fsname);
	kfree(mp->m_rtname);
	kfree(mp->m_logname);
}

STATIC void
xfs_fs_put_super(
	struct super_block	*sb)
{
	struct xfs_mount	*mp = XFS_M(sb);

	xfs_syncd_stop(mp);

	/*
	 * Blow away any referenced inode in the filestreams cache.
	 * This can and will cause log traffic as inodes go inactive
	 * here.
	 */
	xfs_filestream_unmount(mp);

	xfs_flush_buftarg(mp->m_ddev_targp, 1);

	xfs_unmountfs(mp);
	xfs_freesb(mp);
	xfs_icsb_destroy_counters(mp);
	xfs_destroy_mount_workqueues(mp);
	xfs_close_devices(mp);
	xfs_free_fsname(mp);
=======
	/*
	 * If this unlinked inode is in the middle of recovery, don't
	 * drop the inode just yet; log recovery will take care of
	 * that.  See the comment for this inode flag.
	 */
	if (ip->i_flags & XFS_IRECOVERY) {
		ASSERT(xlog_recovery_needed(ip->i_mount->m_log));
		return 0;
	}

	return generic_drop_inode(inode);
}

static void
xfs_mount_free(
	struct xfs_mount	*mp)
{
	if (mp->m_logdev_targp && mp->m_logdev_targp != mp->m_ddev_targp)
		xfs_free_buftarg(mp->m_logdev_targp);
	if (mp->m_rtdev_targp)
		xfs_free_buftarg(mp->m_rtdev_targp);
	if (mp->m_ddev_targp)
		xfs_free_buftarg(mp->m_ddev_targp);

	debugfs_remove(mp->m_debugfs);
	kfree(mp->m_rtname);
	kfree(mp->m_logname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(mp);
}

STATIC int
xfs_fs_sync_fs(
	struct super_block	*sb,
	int			wait)
{
	struct xfs_mount	*mp = XFS_M(sb);
	int			error;

<<<<<<< HEAD
=======
	trace_xfs_fs_sync_fs(mp, __return_address);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Doing anything during the async pass would be counterproductive.
	 */
	if (!wait)
		return 0;

<<<<<<< HEAD
	error = xfs_quiesce_data(mp);
	if (error)
		return -error;
=======
	error = xfs_log_force(mp, XFS_LOG_SYNC);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (laptop_mode) {
		/*
		 * The disk must be active because we're syncing.
<<<<<<< HEAD
		 * We schedule xfssyncd now (now that the disk is
		 * active) instead of later (when it might not be).
		 */
		flush_delayed_work_sync(&mp->m_sync_work);
=======
		 * We schedule log work now (now that the disk is
		 * active) instead of later (when it might not be).
		 */
		flush_delayed_work(&mp->m_log->l_work);
	}

	/*
	 * If we are called with page faults frozen out, it means we are about
	 * to freeze the transaction subsystem. Take the opportunity to shut
	 * down inodegc because once SB_FREEZE_FS is set it's too late to
	 * prevent inactivation races with freeze. The fs doesn't get called
	 * again by the freezing process until after SB_FREEZE_FS has been set,
	 * so it's now or never.  Same logic applies to speculative allocation
	 * garbage collection.
	 *
	 * We don't care if this is a normal syncfs call that does this or
	 * freeze that does this - we can run this multiple times without issue
	 * and we won't race with a restart because a restart can only occur
	 * when the state is either SB_FREEZE_FS or SB_FREEZE_COMPLETE.
	 */
	if (sb->s_writers.frozen == SB_FREEZE_PAGEFAULT) {
		xfs_inodegc_stop(mp);
		xfs_blockgc_stop(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

STATIC int
xfs_fs_statfs(
	struct dentry		*dentry,
	struct kstatfs		*statp)
{
	struct xfs_mount	*mp = XFS_M(dentry->d_sb);
	xfs_sb_t		*sbp = &mp->m_sb;
<<<<<<< HEAD
	struct xfs_inode	*ip = XFS_I(dentry->d_inode);
	__uint64_t		fakeinos, id;
	xfs_extlen_t		lsize;
	__int64_t		ffree;

	statp->f_type = XFS_SB_MAGIC;
	statp->f_namelen = MAXNAMELEN - 1;

	id = huge_encode_dev(mp->m_ddev_targp->bt_dev);
	statp->f_fsid.val[0] = (u32)id;
	statp->f_fsid.val[1] = (u32)(id >> 32);

	xfs_icsb_sync_counters(mp, XFS_ICSB_LAZY_COUNT);
=======
	struct xfs_inode	*ip = XFS_I(d_inode(dentry));
	uint64_t		fakeinos, id;
	uint64_t		icount;
	uint64_t		ifree;
	uint64_t		fdblocks;
	xfs_extlen_t		lsize;
	int64_t			ffree;

	/*
	 * Expedite background inodegc but don't wait. We do not want to block
	 * here waiting hours for a billion extent file to be truncated.
	 */
	xfs_inodegc_push(mp);

	statp->f_type = XFS_SUPER_MAGIC;
	statp->f_namelen = MAXNAMELEN - 1;

	id = huge_encode_dev(mp->m_ddev_targp->bt_dev);
	statp->f_fsid = u64_to_fsid(id);

	icount = percpu_counter_sum(&mp->m_icount);
	ifree = percpu_counter_sum(&mp->m_ifree);
	fdblocks = percpu_counter_sum(&mp->m_fdblocks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&mp->m_sb_lock);
	statp->f_bsize = sbp->sb_blocksize;
	lsize = sbp->sb_logstart ? sbp->sb_logblocks : 0;
	statp->f_blocks = sbp->sb_dblocks - lsize;
<<<<<<< HEAD
	statp->f_bfree = statp->f_bavail =
				sbp->sb_fdblocks - XFS_ALLOC_SET_ASIDE(mp);
	fakeinos = statp->f_bfree << sbp->sb_inopblog;
	statp->f_files =
	    MIN(sbp->sb_icount + fakeinos, (__uint64_t)XFS_MAXINUMBER);
	if (mp->m_maxicount)
		statp->f_files = min_t(typeof(statp->f_files),
					statp->f_files,
					mp->m_maxicount);

	/* make sure statp->f_ffree does not underflow */
	ffree = statp->f_files - (sbp->sb_icount - sbp->sb_ifree);
	statp->f_ffree = max_t(__int64_t, ffree, 0);

	spin_unlock(&mp->m_sb_lock);

	if ((ip->i_d.di_flags & XFS_DIFLAG_PROJINHERIT) ||
	    ((mp->m_qflags & (XFS_PQUOTA_ACCT|XFS_OQUOTA_ENFD))) ==
			      (XFS_PQUOTA_ACCT|XFS_OQUOTA_ENFD))
		xfs_qm_statvfs(ip, statp);
=======
	spin_unlock(&mp->m_sb_lock);

	/* make sure statp->f_bfree does not underflow */
	statp->f_bfree = max_t(int64_t, 0,
				fdblocks - xfs_fdblocks_unavailable(mp));
	statp->f_bavail = statp->f_bfree;

	fakeinos = XFS_FSB_TO_INO(mp, statp->f_bfree);
	statp->f_files = min(icount + fakeinos, (uint64_t)XFS_MAXINUMBER);
	if (M_IGEO(mp)->maxicount)
		statp->f_files = min_t(typeof(statp->f_files),
					statp->f_files,
					M_IGEO(mp)->maxicount);

	/* If sb_icount overshot maxicount, report actual allocation */
	statp->f_files = max_t(typeof(statp->f_files),
					statp->f_files,
					sbp->sb_icount);

	/* make sure statp->f_ffree does not underflow */
	ffree = statp->f_files - (icount - ifree);
	statp->f_ffree = max_t(int64_t, ffree, 0);


	if ((ip->i_diflags & XFS_DIFLAG_PROJINHERIT) &&
	    ((mp->m_qflags & (XFS_PQUOTA_ACCT|XFS_PQUOTA_ENFD))) ==
			      (XFS_PQUOTA_ACCT|XFS_PQUOTA_ENFD))
		xfs_qm_statvfs(ip, statp);

	if (XFS_IS_REALTIME_MOUNT(mp) &&
	    (ip->i_diflags & (XFS_DIFLAG_RTINHERIT | XFS_DIFLAG_REALTIME))) {
		s64	freertx;

		statp->f_blocks = sbp->sb_rblocks;
		freertx = percpu_counter_sum_positive(&mp->m_frextents);
		statp->f_bavail = statp->f_bfree = xfs_rtx_to_rtb(mp, freertx);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

STATIC void
xfs_save_resvblks(struct xfs_mount *mp)
{
<<<<<<< HEAD
	__uint64_t resblks = 0;

	mp->m_resblks_save = mp->m_resblks;
	xfs_reserve_blocks(mp, &resblks, NULL);
=======
	mp->m_resblks_save = mp->m_resblks;
	xfs_reserve_blocks(mp, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC void
xfs_restore_resvblks(struct xfs_mount *mp)
{
<<<<<<< HEAD
	__uint64_t resblks;
=======
	uint64_t resblks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mp->m_resblks_save) {
		resblks = mp->m_resblks_save;
		mp->m_resblks_save = 0;
	} else
		resblks = xfs_default_resblks(mp);

<<<<<<< HEAD
	xfs_reserve_blocks(mp, &resblks, NULL);
}

STATIC int
xfs_fs_remount(
	struct super_block	*sb,
	int			*flags,
	char			*options)
{
	struct xfs_mount	*mp = XFS_M(sb);
	substring_t		args[MAX_OPT_ARGS];
	char			*p;
	int			error;

	while ((p = strsep(&options, ",")) != NULL) {
		int token;

		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_barrier:
			mp->m_flags |= XFS_MOUNT_BARRIER;
			break;
		case Opt_nobarrier:
			mp->m_flags &= ~XFS_MOUNT_BARRIER;
			break;
		default:
			/*
			 * Logically we would return an error here to prevent
			 * users from believing they might have changed
			 * mount options using remount which can't be changed.
			 *
			 * But unfortunately mount(8) adds all options from
			 * mtab and fstab to the mount arguments in some cases
			 * so we can't blindly reject options, but have to
			 * check for each specified option if it actually
			 * differs from the currently set option and only
			 * reject it if that's the case.
			 *
			 * Until that is implemented we return success for
			 * every remount request, and silently ignore all
			 * options that we can't actually change.
			 */
#if 0
			xfs_info(mp,
		"mount option \"%s\" not supported for remount\n", p);
			return -EINVAL;
#else
			break;
#endif
		}
	}

	/* ro -> rw */
	if ((mp->m_flags & XFS_MOUNT_RDONLY) && !(*flags & MS_RDONLY)) {
		mp->m_flags &= ~XFS_MOUNT_RDONLY;

		/*
		 * If this is the first remount to writeable state we
		 * might have some superblock changes to update.
		 */
		if (mp->m_update_flags) {
			error = xfs_mount_log_sb(mp, mp->m_update_flags);
			if (error) {
				xfs_warn(mp, "failed to write sb changes");
				return error;
			}
			mp->m_update_flags = 0;
		}

		/*
		 * Fill out the reserve pool if it is empty. Use the stashed
		 * value if it is non-zero, otherwise go with the default.
		 */
		xfs_restore_resvblks(mp);
	}

	/* rw -> ro */
	if (!(mp->m_flags & XFS_MOUNT_RDONLY) && (*flags & MS_RDONLY)) {
		/*
		 * After we have synced the data but before we sync the
		 * metadata, we need to free up the reserve block pool so that
		 * the used block count in the superblock on disk is correct at
		 * the end of the remount. Stash the current reserve pool size
		 * so that if we get remounted rw, we can return it to the same
		 * size.
		 */

		xfs_quiesce_data(mp);
		xfs_save_resvblks(mp);
		xfs_quiesce_attr(mp);
		mp->m_flags |= XFS_MOUNT_RDONLY;
	}

	return 0;
=======
	xfs_reserve_blocks(mp, resblks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Second stage of a freeze. The data is already frozen so we only
<<<<<<< HEAD
 * need to take care of the metadata. Once that's done write a dummy
 * record to dirty the log in case of a crash while frozen.
=======
 * need to take care of the metadata. Once that's done sync the superblock
 * to the log to dirty it in case of a crash while frozen. This ensures that we
 * will recover the unlinked inode lists on the next mount.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
STATIC int
xfs_fs_freeze(
	struct super_block	*sb)
{
	struct xfs_mount	*mp = XFS_M(sb);
<<<<<<< HEAD

	xfs_save_resvblks(mp);
	xfs_quiesce_attr(mp);
	return -xfs_fs_log_dummy(mp);
=======
	unsigned int		flags;
	int			ret;

	/*
	 * The filesystem is now frozen far enough that memory reclaim
	 * cannot safely operate on the filesystem. Hence we need to
	 * set a GFP_NOFS context here to avoid recursion deadlocks.
	 */
	flags = memalloc_nofs_save();
	xfs_save_resvblks(mp);
	ret = xfs_log_quiesce(mp);
	memalloc_nofs_restore(flags);

	/*
	 * For read-write filesystems, we need to restart the inodegc on error
	 * because we stopped it at SB_FREEZE_PAGEFAULT level and a thaw is not
	 * going to be run to restart it now.  We are at SB_FREEZE_FS level
	 * here, so we can restart safely without racing with a stop in
	 * xfs_fs_sync_fs().
	 */
	if (ret && !xfs_is_readonly(mp)) {
		xfs_blockgc_start(mp);
		xfs_inodegc_start(mp);
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_fs_unfreeze(
	struct super_block	*sb)
{
	struct xfs_mount	*mp = XFS_M(sb);

	xfs_restore_resvblks(mp);
<<<<<<< HEAD
	return 0;
}

STATIC int
xfs_fs_show_options(
	struct seq_file		*m,
	struct dentry		*root)
{
	return -xfs_showargs(XFS_M(root->d_sb), m);
=======
	xfs_log_work_queue(mp);

	/*
	 * Don't reactivate the inodegc worker on a readonly filesystem because
	 * inodes are sent directly to reclaim.  Don't reactivate the blockgc
	 * worker because there are no speculative preallocations on a readonly
	 * filesystem.
	 */
	if (!xfs_is_readonly(mp)) {
		xfs_blockgc_start(mp);
		xfs_inodegc_start(mp);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This function fills in xfs_mount_t fields based on mount args.
 * Note: the superblock _has_ now been read in.
 */
STATIC int
xfs_finish_flags(
	struct xfs_mount	*mp)
{
<<<<<<< HEAD
	int			ronly = (mp->m_flags & XFS_MOUNT_RDONLY);

	/* Fail a mount where the logbuf is smaller than the log stripe */
	if (xfs_sb_version_haslogv2(&mp->m_sb)) {
=======
	/* Fail a mount where the logbuf is smaller than the log stripe */
	if (xfs_has_logv2(mp)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (mp->m_logbsize <= 0 &&
		    mp->m_sb.sb_logsunit > XLOG_BIG_RECORD_BSIZE) {
			mp->m_logbsize = mp->m_sb.sb_logsunit;
		} else if (mp->m_logbsize > 0 &&
			   mp->m_logbsize < mp->m_sb.sb_logsunit) {
			xfs_warn(mp,
		"logbuf size must be greater than or equal to log stripe size");
<<<<<<< HEAD
			return XFS_ERROR(EINVAL);
=======
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		/* Fail a mount if the logbuf is larger than 32K */
		if (mp->m_logbsize > XLOG_BIG_RECORD_BSIZE) {
			xfs_warn(mp,
		"logbuf size for version 1 logs must be 16K or 32K");
<<<<<<< HEAD
			return XFS_ERROR(EINVAL);
=======
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
<<<<<<< HEAD
	 * mkfs'ed attr2 will turn on attr2 mount unless explicitly
	 * told by noattr2 to turn it off
	 */
	if (xfs_sb_version_hasattr2(&mp->m_sb) &&
	    !(mp->m_flags & XFS_MOUNT_NOATTR2))
		mp->m_flags |= XFS_MOUNT_ATTR2;
=======
	 * V5 filesystems always use attr2 format for attributes.
	 */
	if (xfs_has_crc(mp) && xfs_has_noattr2(mp)) {
		xfs_warn(mp, "Cannot mount a V5 filesystem as noattr2. "
			     "attr2 is always enabled for V5 filesystems.");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * prohibit r/w mounts of read-only filesystems
	 */
<<<<<<< HEAD
	if ((mp->m_sb.sb_flags & XFS_SBF_READONLY) && !ronly) {
		xfs_warn(mp,
			"cannot mount a read-only filesystem as read-write");
		return XFS_ERROR(EROFS);
=======
	if ((mp->m_sb.sb_flags & XFS_SBF_READONLY) && !xfs_is_readonly(mp)) {
		xfs_warn(mp,
			"cannot mount a read-only filesystem as read-write");
		return -EROFS;
	}

	if ((mp->m_qflags & XFS_GQUOTA_ACCT) &&
	    (mp->m_qflags & XFS_PQUOTA_ACCT) &&
	    !xfs_has_pquotino(mp)) {
		xfs_warn(mp,
		  "Super block does not support project and group quota together");
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
STATIC int
xfs_fs_fill_super(
	struct super_block	*sb,
	void			*data,
	int			silent)
{
	struct inode		*root;
	struct xfs_mount	*mp = NULL;
	int			flags = 0, error = ENOMEM;

	mp = kzalloc(sizeof(struct xfs_mount), GFP_KERNEL);
	if (!mp)
		goto out;

	spin_lock_init(&mp->m_sb_lock);
	mutex_init(&mp->m_growlock);
	atomic_set(&mp->m_active_trans, 0);

	mp->m_super = sb;
	sb->s_fs_info = mp;

	error = xfs_parseargs(mp, (char *)data);
	if (error)
		goto out_free_fsname;
=======
static int
xfs_init_percpu_counters(
	struct xfs_mount	*mp)
{
	int		error;

	error = percpu_counter_init(&mp->m_icount, 0, GFP_KERNEL);
	if (error)
		return -ENOMEM;

	error = percpu_counter_init(&mp->m_ifree, 0, GFP_KERNEL);
	if (error)
		goto free_icount;

	error = percpu_counter_init(&mp->m_fdblocks, 0, GFP_KERNEL);
	if (error)
		goto free_ifree;

	error = percpu_counter_init(&mp->m_delalloc_blks, 0, GFP_KERNEL);
	if (error)
		goto free_fdblocks;

	error = percpu_counter_init(&mp->m_frextents, 0, GFP_KERNEL);
	if (error)
		goto free_delalloc;

	return 0;

free_delalloc:
	percpu_counter_destroy(&mp->m_delalloc_blks);
free_fdblocks:
	percpu_counter_destroy(&mp->m_fdblocks);
free_ifree:
	percpu_counter_destroy(&mp->m_ifree);
free_icount:
	percpu_counter_destroy(&mp->m_icount);
	return -ENOMEM;
}

void
xfs_reinit_percpu_counters(
	struct xfs_mount	*mp)
{
	percpu_counter_set(&mp->m_icount, mp->m_sb.sb_icount);
	percpu_counter_set(&mp->m_ifree, mp->m_sb.sb_ifree);
	percpu_counter_set(&mp->m_fdblocks, mp->m_sb.sb_fdblocks);
	percpu_counter_set(&mp->m_frextents, mp->m_sb.sb_frextents);
}

static void
xfs_destroy_percpu_counters(
	struct xfs_mount	*mp)
{
	percpu_counter_destroy(&mp->m_icount);
	percpu_counter_destroy(&mp->m_ifree);
	percpu_counter_destroy(&mp->m_fdblocks);
	ASSERT(xfs_is_shutdown(mp) ||
	       percpu_counter_sum(&mp->m_delalloc_blks) == 0);
	percpu_counter_destroy(&mp->m_delalloc_blks);
	percpu_counter_destroy(&mp->m_frextents);
}

static int
xfs_inodegc_init_percpu(
	struct xfs_mount	*mp)
{
	struct xfs_inodegc	*gc;
	int			cpu;

	mp->m_inodegc = alloc_percpu(struct xfs_inodegc);
	if (!mp->m_inodegc)
		return -ENOMEM;

	for_each_possible_cpu(cpu) {
		gc = per_cpu_ptr(mp->m_inodegc, cpu);
		gc->cpu = cpu;
		gc->mp = mp;
		init_llist_head(&gc->list);
		gc->items = 0;
		gc->error = 0;
		INIT_DELAYED_WORK(&gc->work, xfs_inodegc_worker);
	}
	return 0;
}

static void
xfs_inodegc_free_percpu(
	struct xfs_mount	*mp)
{
	if (!mp->m_inodegc)
		return;
	free_percpu(mp->m_inodegc);
}

static void
xfs_fs_put_super(
	struct super_block	*sb)
{
	struct xfs_mount	*mp = XFS_M(sb);

	xfs_notice(mp, "Unmounting Filesystem %pU", &mp->m_sb.sb_uuid);
	xfs_filestream_unmount(mp);
	xfs_unmountfs(mp);

	xfs_freesb(mp);
	xchk_mount_stats_free(mp);
	free_percpu(mp->m_stats.xs_stats);
	xfs_inodegc_free_percpu(mp);
	xfs_destroy_percpu_counters(mp);
	xfs_destroy_mount_workqueues(mp);
	xfs_shutdown_devices(mp);
}

static long
xfs_fs_nr_cached_objects(
	struct super_block	*sb,
	struct shrink_control	*sc)
{
	/* Paranoia: catch incorrect calls during mount setup or teardown */
	if (WARN_ON_ONCE(!sb->s_fs_info))
		return 0;
	return xfs_reclaim_inodes_count(XFS_M(sb));
}

static long
xfs_fs_free_cached_objects(
	struct super_block	*sb,
	struct shrink_control	*sc)
{
	return xfs_reclaim_inodes_nr(XFS_M(sb), sc->nr_to_scan);
}

static void
xfs_fs_shutdown(
	struct super_block	*sb)
{
	xfs_force_shutdown(XFS_M(sb), SHUTDOWN_DEVICE_REMOVED);
}

static const struct super_operations xfs_super_operations = {
	.alloc_inode		= xfs_fs_alloc_inode,
	.destroy_inode		= xfs_fs_destroy_inode,
	.dirty_inode		= xfs_fs_dirty_inode,
	.drop_inode		= xfs_fs_drop_inode,
	.put_super		= xfs_fs_put_super,
	.sync_fs		= xfs_fs_sync_fs,
	.freeze_fs		= xfs_fs_freeze,
	.unfreeze_fs		= xfs_fs_unfreeze,
	.statfs			= xfs_fs_statfs,
	.show_options		= xfs_fs_show_options,
	.nr_cached_objects	= xfs_fs_nr_cached_objects,
	.free_cached_objects	= xfs_fs_free_cached_objects,
	.shutdown		= xfs_fs_shutdown,
};

static int
suffix_kstrtoint(
	const char	*s,
	unsigned int	base,
	int		*res)
{
	int		last, shift_left_factor = 0, _res;
	char		*value;
	int		ret = 0;

	value = kstrdup(s, GFP_KERNEL);
	if (!value)
		return -ENOMEM;

	last = strlen(value) - 1;
	if (value[last] == 'K' || value[last] == 'k') {
		shift_left_factor = 10;
		value[last] = '\0';
	}
	if (value[last] == 'M' || value[last] == 'm') {
		shift_left_factor = 20;
		value[last] = '\0';
	}
	if (value[last] == 'G' || value[last] == 'g') {
		shift_left_factor = 30;
		value[last] = '\0';
	}

	if (kstrtoint(value, base, &_res))
		ret = -EINVAL;
	kfree(value);
	*res = _res << shift_left_factor;
	return ret;
}

static inline void
xfs_fs_warn_deprecated(
	struct fs_context	*fc,
	struct fs_parameter	*param,
	uint64_t		flag,
	bool			value)
{
	/* Don't print the warning if reconfiguring and current mount point
	 * already had the flag set
	 */
	if ((fc->purpose & FS_CONTEXT_FOR_RECONFIGURE) &&
            !!(XFS_M(fc->root->d_sb)->m_features & flag) == value)
		return;
	xfs_warn(fc->s_fs_info, "%s mount option is deprecated.", param->key);
}

/*
 * Set mount state from a mount option.
 *
 * NOTE: mp->m_super is NULL here!
 */
static int
xfs_fs_parse_param(
	struct fs_context	*fc,
	struct fs_parameter	*param)
{
	struct xfs_mount	*parsing_mp = fc->s_fs_info;
	struct fs_parse_result	result;
	int			size = 0;
	int			opt;

	opt = fs_parse(fc, xfs_fs_parameters, param, &result);
	if (opt < 0)
		return opt;

	switch (opt) {
	case Opt_logbufs:
		parsing_mp->m_logbufs = result.uint_32;
		return 0;
	case Opt_logbsize:
		if (suffix_kstrtoint(param->string, 10, &parsing_mp->m_logbsize))
			return -EINVAL;
		return 0;
	case Opt_logdev:
		kfree(parsing_mp->m_logname);
		parsing_mp->m_logname = kstrdup(param->string, GFP_KERNEL);
		if (!parsing_mp->m_logname)
			return -ENOMEM;
		return 0;
	case Opt_rtdev:
		kfree(parsing_mp->m_rtname);
		parsing_mp->m_rtname = kstrdup(param->string, GFP_KERNEL);
		if (!parsing_mp->m_rtname)
			return -ENOMEM;
		return 0;
	case Opt_allocsize:
		if (suffix_kstrtoint(param->string, 10, &size))
			return -EINVAL;
		parsing_mp->m_allocsize_log = ffs(size) - 1;
		parsing_mp->m_features |= XFS_FEAT_ALLOCSIZE;
		return 0;
	case Opt_grpid:
	case Opt_bsdgroups:
		parsing_mp->m_features |= XFS_FEAT_GRPID;
		return 0;
	case Opt_nogrpid:
	case Opt_sysvgroups:
		parsing_mp->m_features &= ~XFS_FEAT_GRPID;
		return 0;
	case Opt_wsync:
		parsing_mp->m_features |= XFS_FEAT_WSYNC;
		return 0;
	case Opt_norecovery:
		parsing_mp->m_features |= XFS_FEAT_NORECOVERY;
		return 0;
	case Opt_noalign:
		parsing_mp->m_features |= XFS_FEAT_NOALIGN;
		return 0;
	case Opt_swalloc:
		parsing_mp->m_features |= XFS_FEAT_SWALLOC;
		return 0;
	case Opt_sunit:
		parsing_mp->m_dalign = result.uint_32;
		return 0;
	case Opt_swidth:
		parsing_mp->m_swidth = result.uint_32;
		return 0;
	case Opt_inode32:
		parsing_mp->m_features |= XFS_FEAT_SMALL_INUMS;
		return 0;
	case Opt_inode64:
		parsing_mp->m_features &= ~XFS_FEAT_SMALL_INUMS;
		return 0;
	case Opt_nouuid:
		parsing_mp->m_features |= XFS_FEAT_NOUUID;
		return 0;
	case Opt_largeio:
		parsing_mp->m_features |= XFS_FEAT_LARGE_IOSIZE;
		return 0;
	case Opt_nolargeio:
		parsing_mp->m_features &= ~XFS_FEAT_LARGE_IOSIZE;
		return 0;
	case Opt_filestreams:
		parsing_mp->m_features |= XFS_FEAT_FILESTREAMS;
		return 0;
	case Opt_noquota:
		parsing_mp->m_qflags &= ~XFS_ALL_QUOTA_ACCT;
		parsing_mp->m_qflags &= ~XFS_ALL_QUOTA_ENFD;
		return 0;
	case Opt_quota:
	case Opt_uquota:
	case Opt_usrquota:
		parsing_mp->m_qflags |= (XFS_UQUOTA_ACCT | XFS_UQUOTA_ENFD);
		return 0;
	case Opt_qnoenforce:
	case Opt_uqnoenforce:
		parsing_mp->m_qflags |= XFS_UQUOTA_ACCT;
		parsing_mp->m_qflags &= ~XFS_UQUOTA_ENFD;
		return 0;
	case Opt_pquota:
	case Opt_prjquota:
		parsing_mp->m_qflags |= (XFS_PQUOTA_ACCT | XFS_PQUOTA_ENFD);
		return 0;
	case Opt_pqnoenforce:
		parsing_mp->m_qflags |= XFS_PQUOTA_ACCT;
		parsing_mp->m_qflags &= ~XFS_PQUOTA_ENFD;
		return 0;
	case Opt_gquota:
	case Opt_grpquota:
		parsing_mp->m_qflags |= (XFS_GQUOTA_ACCT | XFS_GQUOTA_ENFD);
		return 0;
	case Opt_gqnoenforce:
		parsing_mp->m_qflags |= XFS_GQUOTA_ACCT;
		parsing_mp->m_qflags &= ~XFS_GQUOTA_ENFD;
		return 0;
	case Opt_discard:
		parsing_mp->m_features |= XFS_FEAT_DISCARD;
		return 0;
	case Opt_nodiscard:
		parsing_mp->m_features &= ~XFS_FEAT_DISCARD;
		return 0;
#ifdef CONFIG_FS_DAX
	case Opt_dax:
		xfs_mount_set_dax_mode(parsing_mp, XFS_DAX_ALWAYS);
		return 0;
	case Opt_dax_enum:
		xfs_mount_set_dax_mode(parsing_mp, result.uint_32);
		return 0;
#endif
	/* Following mount options will be removed in September 2025 */
	case Opt_ikeep:
		xfs_fs_warn_deprecated(fc, param, XFS_FEAT_IKEEP, true);
		parsing_mp->m_features |= XFS_FEAT_IKEEP;
		return 0;
	case Opt_noikeep:
		xfs_fs_warn_deprecated(fc, param, XFS_FEAT_IKEEP, false);
		parsing_mp->m_features &= ~XFS_FEAT_IKEEP;
		return 0;
	case Opt_attr2:
		xfs_fs_warn_deprecated(fc, param, XFS_FEAT_ATTR2, true);
		parsing_mp->m_features |= XFS_FEAT_ATTR2;
		return 0;
	case Opt_noattr2:
		xfs_fs_warn_deprecated(fc, param, XFS_FEAT_NOATTR2, true);
		parsing_mp->m_features |= XFS_FEAT_NOATTR2;
		return 0;
	default:
		xfs_warn(parsing_mp, "unknown mount option [%s].", param->key);
		return -EINVAL;
	}

	return 0;
}

static int
xfs_fs_validate_params(
	struct xfs_mount	*mp)
{
	/* No recovery flag requires a read-only mount */
	if (xfs_has_norecovery(mp) && !xfs_is_readonly(mp)) {
		xfs_warn(mp, "no-recovery mounts must be read-only.");
		return -EINVAL;
	}

	/*
	 * We have not read the superblock at this point, so only the attr2
	 * mount option can set the attr2 feature by this stage.
	 */
	if (xfs_has_attr2(mp) && xfs_has_noattr2(mp)) {
		xfs_warn(mp, "attr2 and noattr2 cannot both be specified.");
		return -EINVAL;
	}


	if (xfs_has_noalign(mp) && (mp->m_dalign || mp->m_swidth)) {
		xfs_warn(mp,
	"sunit and swidth options incompatible with the noalign option");
		return -EINVAL;
	}

	if (!IS_ENABLED(CONFIG_XFS_QUOTA) && mp->m_qflags != 0) {
		xfs_warn(mp, "quota support not available in this kernel.");
		return -EINVAL;
	}

	if ((mp->m_dalign && !mp->m_swidth) ||
	    (!mp->m_dalign && mp->m_swidth)) {
		xfs_warn(mp, "sunit and swidth must be specified together");
		return -EINVAL;
	}

	if (mp->m_dalign && (mp->m_swidth % mp->m_dalign != 0)) {
		xfs_warn(mp,
	"stripe width (%d) must be a multiple of the stripe unit (%d)",
			mp->m_swidth, mp->m_dalign);
		return -EINVAL;
	}

	if (mp->m_logbufs != -1 &&
	    mp->m_logbufs != 0 &&
	    (mp->m_logbufs < XLOG_MIN_ICLOGS ||
	     mp->m_logbufs > XLOG_MAX_ICLOGS)) {
		xfs_warn(mp, "invalid logbufs value: %d [not %d-%d]",
			mp->m_logbufs, XLOG_MIN_ICLOGS, XLOG_MAX_ICLOGS);
		return -EINVAL;
	}

	if (mp->m_logbsize != -1 &&
	    mp->m_logbsize !=  0 &&
	    (mp->m_logbsize < XLOG_MIN_RECORD_BSIZE ||
	     mp->m_logbsize > XLOG_MAX_RECORD_BSIZE ||
	     !is_power_of_2(mp->m_logbsize))) {
		xfs_warn(mp,
			"invalid logbufsize: %d [not 16k,32k,64k,128k or 256k]",
			mp->m_logbsize);
		return -EINVAL;
	}

	if (xfs_has_allocsize(mp) &&
	    (mp->m_allocsize_log > XFS_MAX_IO_LOG ||
	     mp->m_allocsize_log < XFS_MIN_IO_LOG)) {
		xfs_warn(mp, "invalid log iosize: %d [not %d-%d]",
			mp->m_allocsize_log, XFS_MIN_IO_LOG, XFS_MAX_IO_LOG);
		return -EINVAL;
	}

	return 0;
}

struct dentry *
xfs_debugfs_mkdir(
	const char	*name,
	struct dentry	*parent)
{
	struct dentry	*child;

	/* Apparently we're expected to ignore error returns?? */
	child = debugfs_create_dir(name, parent);
	if (IS_ERR(child))
		return NULL;

	return child;
}

static int
xfs_fs_fill_super(
	struct super_block	*sb,
	struct fs_context	*fc)
{
	struct xfs_mount	*mp = sb->s_fs_info;
	struct inode		*root;
	int			flags = 0, error;

	mp->m_super = sb;

	/*
	 * Copy VFS mount flags from the context now that all parameter parsing
	 * is guaranteed to have been completed by either the old mount API or
	 * the newer fsopen/fsconfig API.
	 */
	if (fc->sb_flags & SB_RDONLY)
		set_bit(XFS_OPSTATE_READONLY, &mp->m_opstate);
	if (fc->sb_flags & SB_DIRSYNC)
		mp->m_features |= XFS_FEAT_DIRSYNC;
	if (fc->sb_flags & SB_SYNCHRONOUS)
		mp->m_features |= XFS_FEAT_WSYNC;

	error = xfs_fs_validate_params(mp);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	sb_min_blocksize(sb, BBSIZE);
	sb->s_xattr = xfs_xattr_handlers;
	sb->s_export_op = &xfs_export_operations;
#ifdef CONFIG_XFS_QUOTA
	sb->s_qcop = &xfs_quotactl_operations;
<<<<<<< HEAD
#endif
	sb->s_op = &xfs_super_operations;

	if (silent)
=======
	sb->s_quota_types = QTYPE_MASK_USR | QTYPE_MASK_GRP | QTYPE_MASK_PRJ;
#endif
	sb->s_op = &xfs_super_operations;

	/*
	 * Delay mount work if the debug hook is set. This is debug
	 * instrumention to coordinate simulation of xfs mount failures with
	 * VFS superblock operations
	 */
	if (xfs_globals.mount_delay) {
		xfs_notice(mp, "Delaying mount for %d seconds.",
			xfs_globals.mount_delay);
		msleep(xfs_globals.mount_delay * 1000);
	}

	if (fc->sb_flags & SB_SILENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags |= XFS_MFSI_QUIET;

	error = xfs_open_devices(mp);
	if (error)
<<<<<<< HEAD
		goto out_free_fsname;

	error = xfs_init_mount_workqueues(mp);
	if (error)
		goto out_close_devices;

	error = xfs_icsb_init_counters(mp);
	if (error)
		goto out_destroy_workqueues;

	error = xfs_readsb(mp, flags);
	if (error)
		goto out_destroy_counters;

=======
		return error;

	if (xfs_debugfs) {
		mp->m_debugfs = xfs_debugfs_mkdir(mp->m_super->s_id,
						  xfs_debugfs);
	} else {
		mp->m_debugfs = NULL;
	}

	error = xfs_init_mount_workqueues(mp);
	if (error)
		goto out_shutdown_devices;

	error = xfs_init_percpu_counters(mp);
	if (error)
		goto out_destroy_workqueues;

	error = xfs_inodegc_init_percpu(mp);
	if (error)
		goto out_destroy_counters;

	/* Allocate stats memory before we do operations that might use it */
	mp->m_stats.xs_stats = alloc_percpu(struct xfsstats);
	if (!mp->m_stats.xs_stats) {
		error = -ENOMEM;
		goto out_destroy_inodegc;
	}

	error = xchk_mount_stats_alloc(mp);
	if (error)
		goto out_free_stats;

	error = xfs_readsb(mp, flags);
	if (error)
		goto out_free_scrub_stats;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = xfs_finish_flags(mp);
	if (error)
		goto out_free_sb;

	error = xfs_setup_devices(mp);
	if (error)
		goto out_free_sb;

<<<<<<< HEAD
=======
	/* V4 support is undergoing deprecation. */
	if (!xfs_has_crc(mp)) {
#ifdef CONFIG_XFS_SUPPORT_V4
		xfs_warn_once(mp,
	"Deprecated V4 format (crc=0) will not be supported after September 2030.");
#else
		xfs_warn(mp,
	"Deprecated V4 format (crc=0) not supported by kernel.");
		error = -EINVAL;
		goto out_free_sb;
#endif
	}

	/* ASCII case insensitivity is undergoing deprecation. */
	if (xfs_has_asciici(mp)) {
#ifdef CONFIG_XFS_SUPPORT_ASCII_CI
		xfs_warn_once(mp,
	"Deprecated ASCII case-insensitivity feature (ascii-ci=1) will not be supported after September 2030.");
#else
		xfs_warn(mp,
	"Deprecated ASCII case-insensitivity feature (ascii-ci=1) not supported by kernel.");
		error = -EINVAL;
		goto out_free_sb;
#endif
	}

	/* Filesystem claims it needs repair, so refuse the mount. */
	if (xfs_has_needsrepair(mp)) {
		xfs_warn(mp, "Filesystem needs repair.  Please run xfs_repair.");
		error = -EFSCORRUPTED;
		goto out_free_sb;
	}

	/*
	 * Don't touch the filesystem if a user tool thinks it owns the primary
	 * superblock.  mkfs doesn't clear the flag from secondary supers, so
	 * we don't check them at all.
	 */
	if (mp->m_sb.sb_inprogress) {
		xfs_warn(mp, "Offline file system operation in progress!");
		error = -EFSCORRUPTED;
		goto out_free_sb;
	}

	/*
	 * Until this is fixed only page-sized or smaller data blocks work.
	 */
	if (mp->m_sb.sb_blocksize > PAGE_SIZE) {
		xfs_warn(mp,
		"File system with blocksize %d bytes. "
		"Only pagesize (%ld) or less will currently work.",
				mp->m_sb.sb_blocksize, PAGE_SIZE);
		error = -ENOSYS;
		goto out_free_sb;
	}

	/* Ensure this filesystem fits in the page cache limits */
	if (xfs_sb_validate_fsb_count(&mp->m_sb, mp->m_sb.sb_dblocks) ||
	    xfs_sb_validate_fsb_count(&mp->m_sb, mp->m_sb.sb_rblocks)) {
		xfs_warn(mp,
		"file system too large to be mounted on this system.");
		error = -EFBIG;
		goto out_free_sb;
	}

	/*
	 * XFS block mappings use 54 bits to store the logical block offset.
	 * This should suffice to handle the maximum file size that the VFS
	 * supports (currently 2^63 bytes on 64-bit and ULONG_MAX << PAGE_SHIFT
	 * bytes on 32-bit), but as XFS and VFS have gotten the s_maxbytes
	 * calculation wrong on 32-bit kernels in the past, we'll add a WARN_ON
	 * to check this assertion.
	 *
	 * Avoid integer overflow by comparing the maximum bmbt offset to the
	 * maximum pagecache offset in units of fs blocks.
	 */
	if (!xfs_verify_fileoff(mp, XFS_B_TO_FSBT(mp, MAX_LFS_FILESIZE))) {
		xfs_warn(mp,
"MAX_LFS_FILESIZE block offset (%llu) exceeds extent map maximum (%llu)!",
			 XFS_B_TO_FSBT(mp, MAX_LFS_FILESIZE),
			 XFS_MAX_FILEOFF);
		error = -EINVAL;
		goto out_free_sb;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = xfs_filestream_mount(mp);
	if (error)
		goto out_free_sb;

	/*
	 * we must configure the block size in the superblock before we run the
	 * full mount process as the mount process can lookup and cache inodes.
<<<<<<< HEAD
	 * For the same reason we must also initialise the syncd and register
	 * the inode cache shrinker so that inodes can be reclaimed during
	 * operations like a quotacheck that iterate all inodes in the
	 * filesystem.
	 */
	sb->s_magic = XFS_SB_MAGIC;
	sb->s_blocksize = mp->m_sb.sb_blocksize;
	sb->s_blocksize_bits = ffs(sb->s_blocksize) - 1;
	sb->s_maxbytes = xfs_max_file_offset(sb->s_blocksize_bits);
	sb->s_max_links = XFS_MAXLINK;
	sb->s_time_gran = 1;
	set_posix_acl_flag(sb);

=======
	 */
	sb->s_magic = XFS_SUPER_MAGIC;
	sb->s_blocksize = mp->m_sb.sb_blocksize;
	sb->s_blocksize_bits = ffs(sb->s_blocksize) - 1;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_max_links = XFS_MAXLINK;
	sb->s_time_gran = 1;
	if (xfs_has_bigtime(mp)) {
		sb->s_time_min = xfs_bigtime_to_unix(XFS_BIGTIME_TIME_MIN);
		sb->s_time_max = xfs_bigtime_to_unix(XFS_BIGTIME_TIME_MAX);
	} else {
		sb->s_time_min = XFS_LEGACY_TIME_MIN;
		sb->s_time_max = XFS_LEGACY_TIME_MAX;
	}
	trace_xfs_inode_timestamp_range(mp, sb->s_time_min, sb->s_time_max);
	sb->s_iflags |= SB_I_CGROUPWB;

	set_posix_acl_flag(sb);

	/* version 5 superblocks support inode version counters. */
	if (xfs_has_crc(mp))
		sb->s_flags |= SB_I_VERSION;

	if (xfs_has_dax_always(mp)) {
		error = xfs_setup_dax_always(mp);
		if (error)
			goto out_filestream_unmount;
	}

	if (xfs_has_discard(mp) && !bdev_max_discard_sectors(sb->s_bdev)) {
		xfs_warn(mp,
	"mounting with \"discard\" option, but the device does not support discard");
		mp->m_features &= ~XFS_FEAT_DISCARD;
	}

	if (xfs_has_reflink(mp)) {
		if (mp->m_sb.sb_rblocks) {
			xfs_alert(mp,
	"reflink not compatible with realtime device!");
			error = -EINVAL;
			goto out_filestream_unmount;
		}

		if (xfs_globals.always_cow) {
			xfs_info(mp, "using DEBUG-only always_cow mode.");
			mp->m_always_cow = true;
		}
	}

	if (xfs_has_rmapbt(mp) && mp->m_sb.sb_rblocks) {
		xfs_alert(mp,
	"reverse mapping btree not compatible with realtime device!");
		error = -EINVAL;
		goto out_filestream_unmount;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	error = xfs_mountfs(mp);
	if (error)
		goto out_filestream_unmount;

<<<<<<< HEAD
	error = xfs_syncd_init(mp);
	if (error)
		goto out_unmount;

	root = igrab(VFS_I(mp->m_rootip));
	if (!root) {
		error = ENOENT;
		goto out_syncd_stop;
	}
	if (is_bad_inode(root)) {
		error = EINVAL;
		goto out_syncd_stop;
	}
	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		error = ENOMEM;
		goto out_syncd_stop;
=======
	root = igrab(VFS_I(mp->m_rootip));
	if (!root) {
		error = -ENOENT;
		goto out_unmount;
	}
	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		error = -ENOMEM;
		goto out_unmount;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

 out_filestream_unmount:
	xfs_filestream_unmount(mp);
 out_free_sb:
	xfs_freesb(mp);
<<<<<<< HEAD
 out_destroy_counters:
	xfs_icsb_destroy_counters(mp);
out_destroy_workqueues:
	xfs_destroy_mount_workqueues(mp);
 out_close_devices:
	xfs_close_devices(mp);
 out_free_fsname:
	xfs_free_fsname(mp);
	kfree(mp);
 out:
	return -error;

 out_syncd_stop:
	xfs_syncd_stop(mp);
 out_unmount:
	/*
	 * Blow away any referenced inode in the filestreams cache.
	 * This can and will cause log traffic as inodes go inactive
	 * here.
	 */
	xfs_filestream_unmount(mp);

	xfs_flush_buftarg(mp->m_ddev_targp, 1);

=======
 out_free_scrub_stats:
	xchk_mount_stats_free(mp);
 out_free_stats:
	free_percpu(mp->m_stats.xs_stats);
 out_destroy_inodegc:
	xfs_inodegc_free_percpu(mp);
 out_destroy_counters:
	xfs_destroy_percpu_counters(mp);
 out_destroy_workqueues:
	xfs_destroy_mount_workqueues(mp);
 out_shutdown_devices:
	xfs_shutdown_devices(mp);
	return error;

 out_unmount:
	xfs_filestream_unmount(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_unmountfs(mp);
	goto out_free_sb;
}

<<<<<<< HEAD
STATIC struct dentry *
xfs_fs_mount(
	struct file_system_type	*fs_type,
	int			flags,
	const char		*dev_name,
	void			*data)
{
	return mount_bdev(fs_type, flags, dev_name, data, xfs_fs_fill_super);
}

static int
xfs_fs_nr_cached_objects(
	struct super_block	*sb)
{
	return xfs_reclaim_inodes_count(XFS_M(sb));
}

static void
xfs_fs_free_cached_objects(
	struct super_block	*sb,
	int			nr_to_scan)
{
	xfs_reclaim_inodes_nr(XFS_M(sb), nr_to_scan);
}

static const struct super_operations xfs_super_operations = {
	.alloc_inode		= xfs_fs_alloc_inode,
	.destroy_inode		= xfs_fs_destroy_inode,
	.dirty_inode		= xfs_fs_dirty_inode,
	.evict_inode		= xfs_fs_evict_inode,
	.drop_inode		= xfs_fs_drop_inode,
	.put_super		= xfs_fs_put_super,
	.sync_fs		= xfs_fs_sync_fs,
	.freeze_fs		= xfs_fs_freeze,
	.unfreeze_fs		= xfs_fs_unfreeze,
	.statfs			= xfs_fs_statfs,
	.remount_fs		= xfs_fs_remount,
	.show_options		= xfs_fs_show_options,
	.nr_cached_objects	= xfs_fs_nr_cached_objects,
	.free_cached_objects	= xfs_fs_free_cached_objects,
};

static struct file_system_type xfs_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "xfs",
	.mount			= xfs_fs_mount,
	.kill_sb		= kill_block_super,
	.fs_flags		= FS_REQUIRES_DEV,
=======
static int
xfs_fs_get_tree(
	struct fs_context	*fc)
{
	return get_tree_bdev(fc, xfs_fs_fill_super);
}

static int
xfs_remount_rw(
	struct xfs_mount	*mp)
{
	struct xfs_sb		*sbp = &mp->m_sb;
	int error;

	if (xfs_has_norecovery(mp)) {
		xfs_warn(mp,
			"ro->rw transition prohibited on norecovery mount");
		return -EINVAL;
	}

	if (xfs_sb_is_v5(sbp) &&
	    xfs_sb_has_ro_compat_feature(sbp, XFS_SB_FEAT_RO_COMPAT_UNKNOWN)) {
		xfs_warn(mp,
	"ro->rw transition prohibited on unknown (0x%x) ro-compat filesystem",
			(sbp->sb_features_ro_compat &
				XFS_SB_FEAT_RO_COMPAT_UNKNOWN));
		return -EINVAL;
	}

	clear_bit(XFS_OPSTATE_READONLY, &mp->m_opstate);

	/*
	 * If this is the first remount to writeable state we might have some
	 * superblock changes to update.
	 */
	if (mp->m_update_sb) {
		error = xfs_sync_sb(mp, false);
		if (error) {
			xfs_warn(mp, "failed to write sb changes");
			return error;
		}
		mp->m_update_sb = false;
	}

	/*
	 * Fill out the reserve pool if it is empty. Use the stashed value if
	 * it is non-zero, otherwise go with the default.
	 */
	xfs_restore_resvblks(mp);
	xfs_log_work_queue(mp);
	xfs_blockgc_start(mp);

	/* Create the per-AG metadata reservation pool .*/
	error = xfs_fs_reserve_ag_blocks(mp);
	if (error && error != -ENOSPC)
		return error;

	/* Re-enable the background inode inactivation worker. */
	xfs_inodegc_start(mp);

	return 0;
}

static int
xfs_remount_ro(
	struct xfs_mount	*mp)
{
	struct xfs_icwalk	icw = {
		.icw_flags	= XFS_ICWALK_FLAG_SYNC,
	};
	int			error;

	/* Flush all the dirty data to disk. */
	error = sync_filesystem(mp->m_super);
	if (error)
		return error;

	/*
	 * Cancel background eofb scanning so it cannot race with the final
	 * log force+buftarg wait and deadlock the remount.
	 */
	xfs_blockgc_stop(mp);

	/*
	 * Clear out all remaining COW staging extents and speculative post-EOF
	 * preallocations so that we don't leave inodes requiring inactivation
	 * cleanups during reclaim on a read-only mount.  We must process every
	 * cached inode, so this requires a synchronous cache scan.
	 */
	error = xfs_blockgc_free_space(mp, &icw);
	if (error) {
		xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
		return error;
	}

	/*
	 * Stop the inodegc background worker.  xfs_fs_reconfigure already
	 * flushed all pending inodegc work when it sync'd the filesystem.
	 * The VFS holds s_umount, so we know that inodes cannot enter
	 * xfs_fs_destroy_inode during a remount operation.  In readonly mode
	 * we send inodes straight to reclaim, so no inodes will be queued.
	 */
	xfs_inodegc_stop(mp);

	/* Free the per-AG metadata reservation pool. */
	error = xfs_fs_unreserve_ag_blocks(mp);
	if (error) {
		xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
		return error;
	}

	/*
	 * Before we sync the metadata, we need to free up the reserve block
	 * pool so that the used block count in the superblock on disk is
	 * correct at the end of the remount. Stash the current* reserve pool
	 * size so that if we get remounted rw, we can return it to the same
	 * size.
	 */
	xfs_save_resvblks(mp);

	xfs_log_clean(mp);
	set_bit(XFS_OPSTATE_READONLY, &mp->m_opstate);

	return 0;
}

/*
 * Logically we would return an error here to prevent users from believing
 * they might have changed mount options using remount which can't be changed.
 *
 * But unfortunately mount(8) adds all options from mtab and fstab to the mount
 * arguments in some cases so we can't blindly reject options, but have to
 * check for each specified option if it actually differs from the currently
 * set option and only reject it if that's the case.
 *
 * Until that is implemented we return success for every remount request, and
 * silently ignore all options that we can't actually change.
 */
static int
xfs_fs_reconfigure(
	struct fs_context *fc)
{
	struct xfs_mount	*mp = XFS_M(fc->root->d_sb);
	struct xfs_mount        *new_mp = fc->s_fs_info;
	int			flags = fc->sb_flags;
	int			error;

	/* version 5 superblocks always support version counters. */
	if (xfs_has_crc(mp))
		fc->sb_flags |= SB_I_VERSION;

	error = xfs_fs_validate_params(new_mp);
	if (error)
		return error;

	/* inode32 -> inode64 */
	if (xfs_has_small_inums(mp) && !xfs_has_small_inums(new_mp)) {
		mp->m_features &= ~XFS_FEAT_SMALL_INUMS;
		mp->m_maxagi = xfs_set_inode_alloc(mp, mp->m_sb.sb_agcount);
	}

	/* inode64 -> inode32 */
	if (!xfs_has_small_inums(mp) && xfs_has_small_inums(new_mp)) {
		mp->m_features |= XFS_FEAT_SMALL_INUMS;
		mp->m_maxagi = xfs_set_inode_alloc(mp, mp->m_sb.sb_agcount);
	}

	/* ro -> rw */
	if (xfs_is_readonly(mp) && !(flags & SB_RDONLY)) {
		error = xfs_remount_rw(mp);
		if (error)
			return error;
	}

	/* rw -> ro */
	if (!xfs_is_readonly(mp) && (flags & SB_RDONLY)) {
		error = xfs_remount_ro(mp);
		if (error)
			return error;
	}

	return 0;
}

static void
xfs_fs_free(
	struct fs_context	*fc)
{
	struct xfs_mount	*mp = fc->s_fs_info;

	/*
	 * mp is stored in the fs_context when it is initialized.
	 * mp is transferred to the superblock on a successful mount,
	 * but if an error occurs before the transfer we have to free
	 * it here.
	 */
	if (mp)
		xfs_mount_free(mp);
}

static const struct fs_context_operations xfs_context_ops = {
	.parse_param = xfs_fs_parse_param,
	.get_tree    = xfs_fs_get_tree,
	.reconfigure = xfs_fs_reconfigure,
	.free        = xfs_fs_free,
};

/*
 * WARNING: do not initialise any parameters in this function that depend on
 * mount option parsing having already been performed as this can be called from
 * fsopen() before any parameters have been set.
 */
static int xfs_init_fs_context(
	struct fs_context	*fc)
{
	struct xfs_mount	*mp;

	mp = kzalloc(sizeof(struct xfs_mount), GFP_KERNEL | __GFP_NOFAIL);
	if (!mp)
		return -ENOMEM;

	spin_lock_init(&mp->m_sb_lock);
	INIT_RADIX_TREE(&mp->m_perag_tree, GFP_ATOMIC);
	spin_lock_init(&mp->m_perag_lock);
	mutex_init(&mp->m_growlock);
	INIT_WORK(&mp->m_flush_inodes_work, xfs_flush_inodes_worker);
	INIT_DELAYED_WORK(&mp->m_reclaim_work, xfs_reclaim_worker);
	mp->m_kobj.kobject.kset = xfs_kset;
	/*
	 * We don't create the finobt per-ag space reservation until after log
	 * recovery, so we must set this to true so that an ifree transaction
	 * started during log recovery will not depend on space reservations
	 * for finobt expansion.
	 */
	mp->m_finobt_nores = true;

	/*
	 * These can be overridden by the mount option parsing.
	 */
	mp->m_logbufs = -1;
	mp->m_logbsize = -1;
	mp->m_allocsize_log = 16; /* 64k */

	xfs_hooks_init(&mp->m_dir_update_hooks);

	fc->s_fs_info = mp;
	fc->ops = &xfs_context_ops;

	return 0;
}

static void
xfs_kill_sb(
	struct super_block		*sb)
{
	kill_block_super(sb);
	xfs_mount_free(XFS_M(sb));
}

static struct file_system_type xfs_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "xfs",
	.init_fs_context	= xfs_init_fs_context,
	.parameters		= xfs_fs_parameters,
	.kill_sb		= xfs_kill_sb,
	.fs_flags		= FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
MODULE_ALIAS_FS("xfs");

STATIC int __init
<<<<<<< HEAD
xfs_init_zones(void)
{

	xfs_ioend_zone = kmem_zone_init(sizeof(xfs_ioend_t), "xfs_ioend");
	if (!xfs_ioend_zone)
		goto out;

	xfs_ioend_pool = mempool_create_slab_pool(4 * MAX_BUF_PER_PAGE,
						  xfs_ioend_zone);
	if (!xfs_ioend_pool)
		goto out_destroy_ioend_zone;

	xfs_log_ticket_zone = kmem_zone_init(sizeof(xlog_ticket_t),
						"xfs_log_ticket");
	if (!xfs_log_ticket_zone)
		goto out_destroy_ioend_pool;

	xfs_bmap_free_item_zone = kmem_zone_init(sizeof(xfs_bmap_free_item_t),
						"xfs_bmap_free_item");
	if (!xfs_bmap_free_item_zone)
		goto out_destroy_log_ticket_zone;

	xfs_btree_cur_zone = kmem_zone_init(sizeof(xfs_btree_cur_t),
						"xfs_btree_cur");
	if (!xfs_btree_cur_zone)
		goto out_destroy_bmap_free_item_zone;

	xfs_da_state_zone = kmem_zone_init(sizeof(xfs_da_state_t),
						"xfs_da_state");
	if (!xfs_da_state_zone)
		goto out_destroy_btree_cur_zone;

	xfs_dabuf_zone = kmem_zone_init(sizeof(xfs_dabuf_t), "xfs_dabuf");
	if (!xfs_dabuf_zone)
		goto out_destroy_da_state_zone;

	xfs_ifork_zone = kmem_zone_init(sizeof(xfs_ifork_t), "xfs_ifork");
	if (!xfs_ifork_zone)
		goto out_destroy_dabuf_zone;

	xfs_trans_zone = kmem_zone_init(sizeof(xfs_trans_t), "xfs_trans");
	if (!xfs_trans_zone)
		goto out_destroy_ifork_zone;

	xfs_log_item_desc_zone =
		kmem_zone_init(sizeof(struct xfs_log_item_desc),
			       "xfs_log_item_desc");
	if (!xfs_log_item_desc_zone)
		goto out_destroy_trans_zone;

	/*
	 * The size of the zone allocated buf log item is the maximum
	 * size possible under XFS.  This wastes a little bit of memory,
	 * but it is much faster.
	 */
	xfs_buf_item_zone = kmem_zone_init((sizeof(xfs_buf_log_item_t) +
				(((XFS_MAX_BLOCKSIZE / XFS_BLF_CHUNK) /
				  NBWORD) * sizeof(int))), "xfs_buf_item");
	if (!xfs_buf_item_zone)
		goto out_destroy_log_item_desc_zone;

	xfs_efd_zone = kmem_zone_init((sizeof(xfs_efd_log_item_t) +
			((XFS_EFD_MAX_FAST_EXTENTS - 1) *
				 sizeof(xfs_extent_t))), "xfs_efd_item");
	if (!xfs_efd_zone)
		goto out_destroy_buf_item_zone;

	xfs_efi_zone = kmem_zone_init((sizeof(xfs_efi_log_item_t) +
			((XFS_EFI_MAX_FAST_EXTENTS - 1) *
				sizeof(xfs_extent_t))), "xfs_efi_item");
	if (!xfs_efi_zone)
		goto out_destroy_efd_zone;

	xfs_inode_zone =
		kmem_zone_init_flags(sizeof(xfs_inode_t), "xfs_inode",
			KM_ZONE_HWALIGN | KM_ZONE_RECLAIM | KM_ZONE_SPREAD,
			xfs_fs_inode_init_once);
	if (!xfs_inode_zone)
		goto out_destroy_efi_zone;

	xfs_ili_zone =
		kmem_zone_init_flags(sizeof(xfs_inode_log_item_t), "xfs_ili",
					KM_ZONE_SPREAD, NULL);
	if (!xfs_ili_zone)
		goto out_destroy_inode_zone;

	return 0;

 out_destroy_inode_zone:
	kmem_zone_destroy(xfs_inode_zone);
 out_destroy_efi_zone:
	kmem_zone_destroy(xfs_efi_zone);
 out_destroy_efd_zone:
	kmem_zone_destroy(xfs_efd_zone);
 out_destroy_buf_item_zone:
	kmem_zone_destroy(xfs_buf_item_zone);
 out_destroy_log_item_desc_zone:
	kmem_zone_destroy(xfs_log_item_desc_zone);
 out_destroy_trans_zone:
	kmem_zone_destroy(xfs_trans_zone);
 out_destroy_ifork_zone:
	kmem_zone_destroy(xfs_ifork_zone);
 out_destroy_dabuf_zone:
	kmem_zone_destroy(xfs_dabuf_zone);
 out_destroy_da_state_zone:
	kmem_zone_destroy(xfs_da_state_zone);
 out_destroy_btree_cur_zone:
	kmem_zone_destroy(xfs_btree_cur_zone);
 out_destroy_bmap_free_item_zone:
	kmem_zone_destroy(xfs_bmap_free_item_zone);
 out_destroy_log_ticket_zone:
	kmem_zone_destroy(xfs_log_ticket_zone);
 out_destroy_ioend_pool:
	mempool_destroy(xfs_ioend_pool);
 out_destroy_ioend_zone:
	kmem_zone_destroy(xfs_ioend_zone);
=======
xfs_init_caches(void)
{
	int		error;

	xfs_buf_cache = kmem_cache_create("xfs_buf", sizeof(struct xfs_buf), 0,
					 SLAB_HWCACHE_ALIGN |
					 SLAB_RECLAIM_ACCOUNT,
					 NULL);
	if (!xfs_buf_cache)
		goto out;

	xfs_log_ticket_cache = kmem_cache_create("xfs_log_ticket",
						sizeof(struct xlog_ticket),
						0, 0, NULL);
	if (!xfs_log_ticket_cache)
		goto out_destroy_buf_cache;

	error = xfs_btree_init_cur_caches();
	if (error)
		goto out_destroy_log_ticket_cache;

	error = rcbagbt_init_cur_cache();
	if (error)
		goto out_destroy_btree_cur_cache;

	error = xfs_defer_init_item_caches();
	if (error)
		goto out_destroy_rcbagbt_cur_cache;

	xfs_da_state_cache = kmem_cache_create("xfs_da_state",
					      sizeof(struct xfs_da_state),
					      0, 0, NULL);
	if (!xfs_da_state_cache)
		goto out_destroy_defer_item_cache;

	xfs_ifork_cache = kmem_cache_create("xfs_ifork",
					   sizeof(struct xfs_ifork),
					   0, 0, NULL);
	if (!xfs_ifork_cache)
		goto out_destroy_da_state_cache;

	xfs_trans_cache = kmem_cache_create("xfs_trans",
					   sizeof(struct xfs_trans),
					   0, 0, NULL);
	if (!xfs_trans_cache)
		goto out_destroy_ifork_cache;


	/*
	 * The size of the cache-allocated buf log item is the maximum
	 * size possible under XFS.  This wastes a little bit of memory,
	 * but it is much faster.
	 */
	xfs_buf_item_cache = kmem_cache_create("xfs_buf_item",
					      sizeof(struct xfs_buf_log_item),
					      0, 0, NULL);
	if (!xfs_buf_item_cache)
		goto out_destroy_trans_cache;

	xfs_efd_cache = kmem_cache_create("xfs_efd_item",
			xfs_efd_log_item_sizeof(XFS_EFD_MAX_FAST_EXTENTS),
			0, 0, NULL);
	if (!xfs_efd_cache)
		goto out_destroy_buf_item_cache;

	xfs_efi_cache = kmem_cache_create("xfs_efi_item",
			xfs_efi_log_item_sizeof(XFS_EFI_MAX_FAST_EXTENTS),
			0, 0, NULL);
	if (!xfs_efi_cache)
		goto out_destroy_efd_cache;

	xfs_inode_cache = kmem_cache_create("xfs_inode",
					   sizeof(struct xfs_inode), 0,
					   (SLAB_HWCACHE_ALIGN |
					    SLAB_RECLAIM_ACCOUNT |
					    SLAB_ACCOUNT),
					   xfs_fs_inode_init_once);
	if (!xfs_inode_cache)
		goto out_destroy_efi_cache;

	xfs_ili_cache = kmem_cache_create("xfs_ili",
					 sizeof(struct xfs_inode_log_item), 0,
					 SLAB_RECLAIM_ACCOUNT,
					 NULL);
	if (!xfs_ili_cache)
		goto out_destroy_inode_cache;

	xfs_icreate_cache = kmem_cache_create("xfs_icr",
					     sizeof(struct xfs_icreate_item),
					     0, 0, NULL);
	if (!xfs_icreate_cache)
		goto out_destroy_ili_cache;

	xfs_rud_cache = kmem_cache_create("xfs_rud_item",
					 sizeof(struct xfs_rud_log_item),
					 0, 0, NULL);
	if (!xfs_rud_cache)
		goto out_destroy_icreate_cache;

	xfs_rui_cache = kmem_cache_create("xfs_rui_item",
			xfs_rui_log_item_sizeof(XFS_RUI_MAX_FAST_EXTENTS),
			0, 0, NULL);
	if (!xfs_rui_cache)
		goto out_destroy_rud_cache;

	xfs_cud_cache = kmem_cache_create("xfs_cud_item",
					 sizeof(struct xfs_cud_log_item),
					 0, 0, NULL);
	if (!xfs_cud_cache)
		goto out_destroy_rui_cache;

	xfs_cui_cache = kmem_cache_create("xfs_cui_item",
			xfs_cui_log_item_sizeof(XFS_CUI_MAX_FAST_EXTENTS),
			0, 0, NULL);
	if (!xfs_cui_cache)
		goto out_destroy_cud_cache;

	xfs_bud_cache = kmem_cache_create("xfs_bud_item",
					 sizeof(struct xfs_bud_log_item),
					 0, 0, NULL);
	if (!xfs_bud_cache)
		goto out_destroy_cui_cache;

	xfs_bui_cache = kmem_cache_create("xfs_bui_item",
			xfs_bui_log_item_sizeof(XFS_BUI_MAX_FAST_EXTENTS),
			0, 0, NULL);
	if (!xfs_bui_cache)
		goto out_destroy_bud_cache;

	xfs_attrd_cache = kmem_cache_create("xfs_attrd_item",
					    sizeof(struct xfs_attrd_log_item),
					    0, 0, NULL);
	if (!xfs_attrd_cache)
		goto out_destroy_bui_cache;

	xfs_attri_cache = kmem_cache_create("xfs_attri_item",
					    sizeof(struct xfs_attri_log_item),
					    0, 0, NULL);
	if (!xfs_attri_cache)
		goto out_destroy_attrd_cache;

	xfs_iunlink_cache = kmem_cache_create("xfs_iul_item",
					     sizeof(struct xfs_iunlink_item),
					     0, 0, NULL);
	if (!xfs_iunlink_cache)
		goto out_destroy_attri_cache;

	return 0;

 out_destroy_attri_cache:
	kmem_cache_destroy(xfs_attri_cache);
 out_destroy_attrd_cache:
	kmem_cache_destroy(xfs_attrd_cache);
 out_destroy_bui_cache:
	kmem_cache_destroy(xfs_bui_cache);
 out_destroy_bud_cache:
	kmem_cache_destroy(xfs_bud_cache);
 out_destroy_cui_cache:
	kmem_cache_destroy(xfs_cui_cache);
 out_destroy_cud_cache:
	kmem_cache_destroy(xfs_cud_cache);
 out_destroy_rui_cache:
	kmem_cache_destroy(xfs_rui_cache);
 out_destroy_rud_cache:
	kmem_cache_destroy(xfs_rud_cache);
 out_destroy_icreate_cache:
	kmem_cache_destroy(xfs_icreate_cache);
 out_destroy_ili_cache:
	kmem_cache_destroy(xfs_ili_cache);
 out_destroy_inode_cache:
	kmem_cache_destroy(xfs_inode_cache);
 out_destroy_efi_cache:
	kmem_cache_destroy(xfs_efi_cache);
 out_destroy_efd_cache:
	kmem_cache_destroy(xfs_efd_cache);
 out_destroy_buf_item_cache:
	kmem_cache_destroy(xfs_buf_item_cache);
 out_destroy_trans_cache:
	kmem_cache_destroy(xfs_trans_cache);
 out_destroy_ifork_cache:
	kmem_cache_destroy(xfs_ifork_cache);
 out_destroy_da_state_cache:
	kmem_cache_destroy(xfs_da_state_cache);
 out_destroy_defer_item_cache:
	xfs_defer_destroy_item_caches();
 out_destroy_rcbagbt_cur_cache:
	rcbagbt_destroy_cur_cache();
 out_destroy_btree_cur_cache:
	xfs_btree_destroy_cur_caches();
 out_destroy_log_ticket_cache:
	kmem_cache_destroy(xfs_log_ticket_cache);
 out_destroy_buf_cache:
	kmem_cache_destroy(xfs_buf_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 out:
	return -ENOMEM;
}

STATIC void
<<<<<<< HEAD
xfs_destroy_zones(void)
=======
xfs_destroy_caches(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * Make sure all delayed rcu free are flushed before we
	 * destroy caches.
	 */
	rcu_barrier();
<<<<<<< HEAD
	kmem_zone_destroy(xfs_ili_zone);
	kmem_zone_destroy(xfs_inode_zone);
	kmem_zone_destroy(xfs_efi_zone);
	kmem_zone_destroy(xfs_efd_zone);
	kmem_zone_destroy(xfs_buf_item_zone);
	kmem_zone_destroy(xfs_log_item_desc_zone);
	kmem_zone_destroy(xfs_trans_zone);
	kmem_zone_destroy(xfs_ifork_zone);
	kmem_zone_destroy(xfs_dabuf_zone);
	kmem_zone_destroy(xfs_da_state_zone);
	kmem_zone_destroy(xfs_btree_cur_zone);
	kmem_zone_destroy(xfs_bmap_free_item_zone);
	kmem_zone_destroy(xfs_log_ticket_zone);
	mempool_destroy(xfs_ioend_pool);
	kmem_zone_destroy(xfs_ioend_zone);

=======
	kmem_cache_destroy(xfs_iunlink_cache);
	kmem_cache_destroy(xfs_attri_cache);
	kmem_cache_destroy(xfs_attrd_cache);
	kmem_cache_destroy(xfs_bui_cache);
	kmem_cache_destroy(xfs_bud_cache);
	kmem_cache_destroy(xfs_cui_cache);
	kmem_cache_destroy(xfs_cud_cache);
	kmem_cache_destroy(xfs_rui_cache);
	kmem_cache_destroy(xfs_rud_cache);
	kmem_cache_destroy(xfs_icreate_cache);
	kmem_cache_destroy(xfs_ili_cache);
	kmem_cache_destroy(xfs_inode_cache);
	kmem_cache_destroy(xfs_efi_cache);
	kmem_cache_destroy(xfs_efd_cache);
	kmem_cache_destroy(xfs_buf_item_cache);
	kmem_cache_destroy(xfs_trans_cache);
	kmem_cache_destroy(xfs_ifork_cache);
	kmem_cache_destroy(xfs_da_state_cache);
	xfs_defer_destroy_item_caches();
	rcbagbt_destroy_cur_cache();
	xfs_btree_destroy_cur_caches();
	kmem_cache_destroy(xfs_log_ticket_cache);
	kmem_cache_destroy(xfs_buf_cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int __init
xfs_init_workqueues(void)
{
	/*
<<<<<<< HEAD
	 * We never want to the same work item to run twice, reclaiming inodes
	 * or idling the log is not going to get any faster by multiple CPUs
	 * competing for ressources.  Use the default large max_active value
	 * so that even lots of filesystems can perform these task in parallel.
	 */
	xfs_syncd_wq = alloc_workqueue("xfssyncd", WQ_NON_REENTRANT, 0);
	if (!xfs_syncd_wq)
		return -ENOMEM;

	/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * The allocation workqueue can be used in memory reclaim situations
	 * (writepage path), and parallelism is only limited by the number of
	 * AGs in all the filesystems mounted. Hence use the default large
	 * max_active value for this workqueue.
	 */
<<<<<<< HEAD
	xfs_alloc_wq = alloc_workqueue("xfsalloc", WQ_MEM_RECLAIM, 0);
	if (!xfs_alloc_wq)
		goto out_destroy_syncd;

	return 0;

out_destroy_syncd:
	destroy_workqueue(xfs_syncd_wq);
=======
	xfs_alloc_wq = alloc_workqueue("xfsalloc",
			XFS_WQFLAGS(WQ_MEM_RECLAIM | WQ_FREEZABLE), 0);
	if (!xfs_alloc_wq)
		return -ENOMEM;

	xfs_discard_wq = alloc_workqueue("xfsdiscard", XFS_WQFLAGS(WQ_UNBOUND),
			0);
	if (!xfs_discard_wq)
		goto out_free_alloc_wq;

	return 0;
out_free_alloc_wq:
	destroy_workqueue(xfs_alloc_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

STATIC void
xfs_destroy_workqueues(void)
{
<<<<<<< HEAD
	destroy_workqueue(xfs_alloc_wq);
	destroy_workqueue(xfs_syncd_wq);
=======
	destroy_workqueue(xfs_discard_wq);
	destroy_workqueue(xfs_alloc_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int __init
init_xfs_fs(void)
{
	int			error;

<<<<<<< HEAD
=======
	xfs_check_ondisk_structs();

	error = xfs_dahash_test();
	if (error)
		return error;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk(KERN_INFO XFS_VERSION_STRING " with "
			 XFS_BUILD_OPTIONS " enabled\n");

	xfs_dir_startup();

<<<<<<< HEAD
	error = xfs_init_zones();
=======
	error = xfs_init_caches();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		goto out;

	error = xfs_init_workqueues();
	if (error)
<<<<<<< HEAD
		goto out_destroy_zones;
=======
		goto out_destroy_caches;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = xfs_mru_cache_init();
	if (error)
		goto out_destroy_wq;

<<<<<<< HEAD
	error = xfs_filestream_init();
	if (error)
		goto out_mru_cache_uninit;

	error = xfs_buf_init();
	if (error)
		goto out_filestream_uninit;

	error = xfs_init_procfs();
	if (error)
		goto out_buf_terminate;
=======
	error = xfs_init_procfs();
	if (error)
		goto out_mru_cache_uninit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = xfs_sysctl_register();
	if (error)
		goto out_cleanup_procfs;

<<<<<<< HEAD
	error = xfs_qm_init();
	if (error)
		goto out_sysctl_unregister;
=======
	xfs_debugfs = xfs_debugfs_mkdir("xfs", NULL);

	xfs_kset = kset_create_and_add("xfs", NULL, fs_kobj);
	if (!xfs_kset) {
		error = -ENOMEM;
		goto out_debugfs_unregister;
	}

	xfsstats.xs_kobj.kobject.kset = xfs_kset;

	xfsstats.xs_stats = alloc_percpu(struct xfsstats);
	if (!xfsstats.xs_stats) {
		error = -ENOMEM;
		goto out_kset_unregister;
	}

	error = xfs_sysfs_init(&xfsstats.xs_kobj, &xfs_stats_ktype, NULL,
			       "stats");
	if (error)
		goto out_free_stats;

	error = xchk_global_stats_setup(xfs_debugfs);
	if (error)
		goto out_remove_stats_kobj;

#ifdef DEBUG
	xfs_dbg_kobj.kobject.kset = xfs_kset;
	error = xfs_sysfs_init(&xfs_dbg_kobj, &xfs_dbg_ktype, NULL, "debug");
	if (error)
		goto out_remove_scrub_stats;
#endif

	error = xfs_qm_init();
	if (error)
		goto out_remove_dbg_kobj;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	error = register_filesystem(&xfs_fs_type);
	if (error)
		goto out_qm_exit;
	return 0;

 out_qm_exit:
	xfs_qm_exit();
<<<<<<< HEAD
 out_sysctl_unregister:
	xfs_sysctl_unregister();
 out_cleanup_procfs:
	xfs_cleanup_procfs();
 out_buf_terminate:
	xfs_buf_terminate();
 out_filestream_uninit:
	xfs_filestream_uninit();
=======
 out_remove_dbg_kobj:
#ifdef DEBUG
	xfs_sysfs_del(&xfs_dbg_kobj);
 out_remove_scrub_stats:
#endif
	xchk_global_stats_teardown();
 out_remove_stats_kobj:
	xfs_sysfs_del(&xfsstats.xs_kobj);
 out_free_stats:
	free_percpu(xfsstats.xs_stats);
 out_kset_unregister:
	kset_unregister(xfs_kset);
 out_debugfs_unregister:
	debugfs_remove(xfs_debugfs);
	xfs_sysctl_unregister();
 out_cleanup_procfs:
	xfs_cleanup_procfs();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 out_mru_cache_uninit:
	xfs_mru_cache_uninit();
 out_destroy_wq:
	xfs_destroy_workqueues();
<<<<<<< HEAD
 out_destroy_zones:
	xfs_destroy_zones();
=======
 out_destroy_caches:
	xfs_destroy_caches();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 out:
	return error;
}

STATIC void __exit
exit_xfs_fs(void)
{
	xfs_qm_exit();
	unregister_filesystem(&xfs_fs_type);
<<<<<<< HEAD
	xfs_sysctl_unregister();
	xfs_cleanup_procfs();
	xfs_buf_terminate();
	xfs_filestream_uninit();
	xfs_mru_cache_uninit();
	xfs_destroy_workqueues();
	xfs_destroy_zones();
=======
#ifdef DEBUG
	xfs_sysfs_del(&xfs_dbg_kobj);
#endif
	xchk_global_stats_teardown();
	xfs_sysfs_del(&xfsstats.xs_kobj);
	free_percpu(xfsstats.xs_stats);
	kset_unregister(xfs_kset);
	debugfs_remove(xfs_debugfs);
	xfs_sysctl_unregister();
	xfs_cleanup_procfs();
	xfs_mru_cache_uninit();
	xfs_destroy_workqueues();
	xfs_destroy_caches();
	xfs_uuid_table_free();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(init_xfs_fs);
module_exit(exit_xfs_fs);

MODULE_AUTHOR("Silicon Graphics, Inc.");
MODULE_DESCRIPTION(XFS_VERSION_STRING " with " XFS_BUILD_OPTIONS " enabled");
MODULE_LICENSE("GPL");
