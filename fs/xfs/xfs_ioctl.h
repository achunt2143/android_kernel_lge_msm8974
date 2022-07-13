<<<<<<< HEAD
/*
 * Copyright (c) 2008 Silicon Graphics, Inc.
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
 * Copyright (c) 2008 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __XFS_IOCTL_H__
#define __XFS_IOCTL_H__

<<<<<<< HEAD
extern int
xfs_ioc_space(
	struct xfs_inode	*ip,
	struct inode		*inode,
	struct file		*filp,
	int			ioflags,
	unsigned int		cmd,
	xfs_flock64_t		*bf);
=======
struct xfs_bstat;
struct xfs_ibulk;
struct xfs_inogrp;

int
xfs_ioc_swapext(
	xfs_swapext_t	*sxp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int
xfs_find_handle(
	unsigned int		cmd,
	xfs_fsop_handlereq_t	*hreq);

extern int
xfs_open_by_handle(
	struct file		*parfilp,
	xfs_fsop_handlereq_t	*hreq);

extern int
xfs_readlink_by_handle(
	struct file		*parfilp,
	xfs_fsop_handlereq_t	*hreq);

<<<<<<< HEAD
extern int
xfs_attrmulti_attr_get(
	struct inode		*inode,
	unsigned char		*name,
	unsigned char		__user *ubuf,
	__uint32_t		*len,
	__uint32_t		flags);

extern int
xfs_attrmulti_attr_set(
	struct inode		*inode,
	unsigned char		*name,
	const unsigned char	__user *ubuf,
	__uint32_t		len,
	__uint32_t		flags);

extern int
xfs_attrmulti_attr_remove(
	struct inode		*inode,
	unsigned char		*name,
	__uint32_t		flags);
=======
int xfs_ioc_attrmulti_one(struct file *parfilp, struct inode *inode,
		uint32_t opcode, void __user *uname, void __user *value,
		uint32_t *len, uint32_t flags);
int xfs_ioc_attr_list(struct xfs_inode *dp, void __user *ubuf,
		      size_t bufsize, int flags,
		      struct xfs_attrlist_cursor __user *ucursor);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct dentry *
xfs_handle_to_dentry(
	struct file		*parfilp,
	void __user		*uhandle,
	u32			hlen);

<<<<<<< HEAD
=======
extern int
xfs_fileattr_get(
	struct dentry		*dentry,
	struct fileattr		*fa);

extern int
xfs_fileattr_set(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct fileattr		*fa);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern long
xfs_file_ioctl(
	struct file		*filp,
	unsigned int		cmd,
	unsigned long		p);

extern long
xfs_file_compat_ioctl(
	struct file		*file,
	unsigned int		cmd,
	unsigned long		arg);

<<<<<<< HEAD
=======
int xfs_fsbulkstat_one_fmt(struct xfs_ibulk *breq,
			   const struct xfs_bulkstat *bstat);
int xfs_fsinumbers_fmt(struct xfs_ibulk *breq, const struct xfs_inumbers *igrp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
