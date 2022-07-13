<<<<<<< HEAD
/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
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
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __XFS_ITABLE_H__
#define	__XFS_ITABLE_H__

<<<<<<< HEAD
/*
 * xfs_bulkstat() is used to fill in xfs_bstat structures as well as dm_stat
 * structures (by the dmi library). This is a pointer to a formatter function
 * that will iget the inode and fill in the appropriate structure.
 * see xfs_bulkstat_one() and xfs_dm_bulkstat_one() in dmapi_xfs.c
 */
typedef int (*bulkstat_one_pf)(struct xfs_mount	*mp,
			       xfs_ino_t	ino,
			       void		__user *buffer,
			       int		ubsize,
			       int		*ubused,
			       int		*stat);

/*
 * Values for stat return value.
 */
#define BULKSTAT_RV_NOTHING	0
#define BULKSTAT_RV_DIDONE	1
#define BULKSTAT_RV_GIVEUP	2
=======
/* In-memory representation of a userspace request for batch inode data. */
struct xfs_ibulk {
	struct xfs_mount	*mp;
	struct mnt_idmap	*idmap;
	void __user		*ubuffer; /* user output buffer */
	xfs_ino_t		startino; /* start with this inode */
	unsigned int		icount;   /* number of elements in ubuffer */
	unsigned int		ocount;   /* number of records returned */
	unsigned int		flags;    /* see XFS_IBULK_FLAG_* */
};

/* Only iterate within the same AG as startino */
#define XFS_IBULK_SAME_AG	(1U << 0)

/* Fill out the bs_extents64 field if set. */
#define XFS_IBULK_NREXT64	(1U << 1)

/*
 * Advance the user buffer pointer by one record of the given size.  If the
 * buffer is now full, return the appropriate error code.
 */
static inline int
xfs_ibulk_advance(
	struct xfs_ibulk	*breq,
	size_t			bytes)
{
	char __user		*b = breq->ubuffer;

	breq->ubuffer = b + bytes;
	breq->ocount++;
	return breq->ocount == breq->icount ? -ECANCELED : 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 */
<<<<<<< HEAD
int					/* error status */
xfs_bulkstat(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	*lastino,	/* last inode returned */
	int		*count,		/* size of buffer/count returned */
	bulkstat_one_pf formatter,	/* func that'd fill a single buf */
	size_t		statstruct_size,/* sizeof struct that we're filling */
	char		__user *ubuffer,/* buffer with inode stats */
	int		*done);		/* 1 if there are more stats to get */

int
xfs_bulkstat_single(
	xfs_mount_t		*mp,
	xfs_ino_t		*lastinop,
	char			__user *buffer,
	int			*done);

typedef int (*bulkstat_one_fmt_pf)(  /* used size in bytes or negative error */
	void			__user *ubuffer, /* buffer to write to */
	int			ubsize,		 /* remaining user buffer sz */
	int			*ubused,	 /* bytes used by formatter */
	const xfs_bstat_t	*buffer);        /* buffer to read from */

int
xfs_bulkstat_one_int(
	xfs_mount_t		*mp,
	xfs_ino_t		ino,
	void			__user *buffer,
	int			ubsize,
	bulkstat_one_fmt_pf	formatter,
	int			*ubused,
	int			*stat);

int
xfs_bulkstat_one(
	xfs_mount_t		*mp,
	xfs_ino_t		ino,
	void			__user *buffer,
	int			ubsize,
	int			*ubused,
	int			*stat);

typedef int (*inumbers_fmt_pf)(
	void			__user *ubuffer, /* buffer to write to */
	const xfs_inogrp_t	*buffer,	/* buffer to read from */
	long			count,		/* # of elements to read */
	long			*written);	/* # of bytes written */

int
xfs_inumbers_fmt(
	void			__user *ubuffer, /* buffer to write to */
	const xfs_inogrp_t	*buffer,	/* buffer to read from */
	long			count,		/* # of elements to read */
	long			*written);	/* # of bytes written */

int					/* error status */
xfs_inumbers(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*last,	/* last inode returned */
	int			*count,	/* size of buffer/count returned */
	void			__user *buffer, /* buffer with inode info */
	inumbers_fmt_pf		formatter);
=======

/*
 * Return codes for the formatter function are 0 to continue iterating, and
 * non-zero to stop iterating.  Any non-zero value will be passed up to the
 * bulkstat/inumbers caller.  The special value -ECANCELED can be used to stop
 * iteration, as neither bulkstat nor inumbers will ever generate that error
 * code on their own.
 */

typedef int (*bulkstat_one_fmt_pf)(struct xfs_ibulk *breq,
		const struct xfs_bulkstat *bstat);

int xfs_bulkstat_one(struct xfs_ibulk *breq, bulkstat_one_fmt_pf formatter);
int xfs_bulkstat(struct xfs_ibulk *breq, bulkstat_one_fmt_pf formatter);
void xfs_bulkstat_to_bstat(struct xfs_mount *mp, struct xfs_bstat *bs1,
		const struct xfs_bulkstat *bstat);

typedef int (*inumbers_fmt_pf)(struct xfs_ibulk *breq,
		const struct xfs_inumbers *igrp);

int xfs_inumbers(struct xfs_ibulk *breq, inumbers_fmt_pf formatter);
void xfs_inumbers_to_inogrp(struct xfs_inogrp *ig1,
		const struct xfs_inumbers *ig);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __XFS_ITABLE_H__ */
