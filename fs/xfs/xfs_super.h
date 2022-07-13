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
#ifndef __XFS_SUPER_H__
#define __XFS_SUPER_H__

#include <linux/exportfs.h>

#ifdef CONFIG_XFS_QUOTA
extern int xfs_qm_init(void);
extern void xfs_qm_exit(void);
<<<<<<< HEAD
#else
# define xfs_qm_init()	(0)
# define xfs_qm_exit()	do { } while (0)
=======
# define XFS_QUOTA_STRING	"quota, "
#else
# define xfs_qm_init()	(0)
# define xfs_qm_exit()	do { } while (0)
# define XFS_QUOTA_STRING
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_XFS_POSIX_ACL
# define XFS_ACL_STRING		"ACLs, "
<<<<<<< HEAD
# define set_posix_acl_flag(sb)	((sb)->s_flags |= MS_POSIXACL)
=======
# define set_posix_acl_flag(sb)	((sb)->s_flags |= SB_POSIXACL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
# define XFS_ACL_STRING
# define set_posix_acl_flag(sb)	do { } while (0)
#endif

#define XFS_SECURITY_STRING	"security attributes, "

#ifdef CONFIG_XFS_RT
# define XFS_REALTIME_STRING	"realtime, "
#else
# define XFS_REALTIME_STRING
#endif

<<<<<<< HEAD
#if XFS_BIG_BLKNOS
# if XFS_BIG_INUMS
#  define XFS_BIGFS_STRING	"large block/inode numbers, "
# else
#  define XFS_BIGFS_STRING	"large block numbers, "
# endif
#else
# define XFS_BIGFS_STRING
=======
#ifdef CONFIG_XFS_ONLINE_SCRUB
# define XFS_SCRUB_STRING	"scrub, "
#else
# define XFS_SCRUB_STRING
#endif

#ifdef CONFIG_XFS_ONLINE_REPAIR
# define XFS_REPAIR_STRING	"repair, "
#else
# define XFS_REPAIR_STRING
#endif

#ifdef CONFIG_XFS_WARN
# define XFS_WARN_STRING	"verbose warnings, "
#else
# define XFS_WARN_STRING
#endif

#ifdef CONFIG_XFS_ASSERT_FATAL
# define XFS_ASSERT_FATAL_STRING	"fatal assert, "
#else
# define XFS_ASSERT_FATAL_STRING
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef DEBUG
# define XFS_DBG_STRING		"debug"
#else
# define XFS_DBG_STRING		"no debug"
#endif

#define XFS_VERSION_STRING	"SGI XFS"
#define XFS_BUILD_OPTIONS	XFS_ACL_STRING \
				XFS_SECURITY_STRING \
				XFS_REALTIME_STRING \
<<<<<<< HEAD
				XFS_BIGFS_STRING \
				XFS_DBG_STRING /* DBG must be last */

=======
				XFS_SCRUB_STRING \
				XFS_REPAIR_STRING \
				XFS_WARN_STRING \
				XFS_QUOTA_STRING \
				XFS_ASSERT_FATAL_STRING \
				XFS_DBG_STRING /* DBG must be last */

#ifdef DEBUG
# define XFS_WQFLAGS(wqflags)	(WQ_SYSFS | (wqflags))
#else
# define XFS_WQFLAGS(wqflags)	(wqflags)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct xfs_inode;
struct xfs_mount;
struct xfs_buftarg;
struct block_device;

<<<<<<< HEAD
extern __uint64_t xfs_max_file_offset(unsigned int);

extern void xfs_blkdev_issue_flush(struct xfs_buftarg *);

extern const struct export_operations xfs_export_operations;
extern const struct xattr_handler *xfs_xattr_handlers[];
extern const struct quotactl_ops xfs_quotactl_operations;

#define XFS_M(sb)		((struct xfs_mount *)((sb)->s_fs_info))

=======
extern void xfs_flush_inodes(struct xfs_mount *mp);
extern xfs_agnumber_t xfs_set_inode_alloc(struct xfs_mount *,
					   xfs_agnumber_t agcount);

extern const struct export_operations xfs_export_operations;
extern const struct quotactl_ops xfs_quotactl_operations;
extern const struct dax_holder_operations xfs_dax_holder_operations;

extern void xfs_reinit_percpu_counters(struct xfs_mount *mp);

extern struct workqueue_struct *xfs_discard_wq;

#define XFS_M(sb)		((struct xfs_mount *)((sb)->s_fs_info))

struct dentry *xfs_debugfs_mkdir(const char *name, struct dentry *parent);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* __XFS_SUPER_H__ */
