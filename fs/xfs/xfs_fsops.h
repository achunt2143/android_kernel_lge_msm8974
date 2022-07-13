<<<<<<< HEAD
/*
 * Copyright (c) 2000-2001,2005 Silicon Graphics, Inc.
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
 * Copyright (c) 2000-2001,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __XFS_FSOPS_H__
#define	__XFS_FSOPS_H__

<<<<<<< HEAD
extern int xfs_fs_geometry(xfs_mount_t *mp, xfs_fsop_geom_t *geo, int nversion);
extern int xfs_growfs_data(xfs_mount_t *mp, xfs_growfs_data_t *in);
extern int xfs_growfs_log(xfs_mount_t *mp, xfs_growfs_log_t *in);
extern int xfs_fs_counts(xfs_mount_t *mp, xfs_fsop_counts_t *cnt);
extern int xfs_reserve_blocks(xfs_mount_t *mp, __uint64_t *inval,
				xfs_fsop_resblks_t *outval);
extern int xfs_fs_goingdown(xfs_mount_t *mp, __uint32_t inflags);
extern int xfs_fs_log_dummy(struct xfs_mount *mp);
=======
int xfs_growfs_data(struct xfs_mount *mp, struct xfs_growfs_data *in);
int xfs_growfs_log(struct xfs_mount *mp, struct xfs_growfs_log *in);
int xfs_reserve_blocks(struct xfs_mount *mp, uint64_t request);
int xfs_fs_goingdown(struct xfs_mount *mp, uint32_t inflags);

int xfs_fs_reserve_ag_blocks(struct xfs_mount *mp);
int xfs_fs_unreserve_ag_blocks(struct xfs_mount *mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __XFS_FSOPS_H__ */
