<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef XFS_DISCARD_H
#define XFS_DISCARD_H 1

struct fstrim_range;
<<<<<<< HEAD
struct list_head;

extern int	xfs_ioc_trim(struct xfs_mount *, struct fstrim_range __user *);
extern int	xfs_discard_extents(struct xfs_mount *, struct list_head *);
=======
struct xfs_mount;
struct xfs_busy_extents;

int xfs_discard_extents(struct xfs_mount *mp, struct xfs_busy_extents *busy);
int xfs_ioc_trim(struct xfs_mount *mp, struct fstrim_range __user *fstrim);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* XFS_DISCARD_H */
