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
#ifndef __XFS_DQUOT_H__
#define __XFS_DQUOT_H__

/*
 * Dquots are structures that hold quota information about a user or a group,
 * much like inodes are for files. In fact, dquots share many characteristics
 * with inodes. However, dquots can also be a centralized resource, relative
 * to a collection of inodes. In this respect, dquots share some characteristics
 * of the superblock.
 * XFS dquots exploit both those in its algorithms. They make every attempt
 * to not be a bottleneck when quotas are on and have minimal impact, if any,
 * when quotas are off.
 */

struct xfs_mount;
struct xfs_trans;

<<<<<<< HEAD
/*
 * The incore dquot structure
 */
typedef struct xfs_dquot {
	uint		 dq_flags;	/* various flags (XFS_DQ_*) */
	struct list_head q_lru;		/* global free list of dquots */
	struct xfs_mount*q_mount;	/* filesystem this relates to */
	struct xfs_trans*q_transp;	/* trans this belongs to currently */
	uint		 q_nrefs;	/* # active refs from inodes */
	xfs_daddr_t	 q_blkno;	/* blkno of dquot buffer */
	int		 q_bufoffset;	/* off of dq in buffer (# dquots) */
	xfs_fileoff_t	 q_fileoffset;	/* offset in quotas file */

	struct xfs_dquot*q_gdquot;	/* group dquot, hint only */
	xfs_disk_dquot_t q_core;	/* actual usage & quotas */
	xfs_dq_logitem_t q_logitem;	/* dquot log item */
	xfs_qcnt_t	 q_res_bcount;	/* total regular nblks used+reserved */
	xfs_qcnt_t	 q_res_icount;	/* total inos allocd+reserved */
	xfs_qcnt_t	 q_res_rtbcount;/* total realtime blks used+reserved */
	struct mutex	 q_qlock;	/* quota lock */
	struct completion q_flush;	/* flush completion queue */
	atomic_t          q_pincount;	/* dquot pin count */
	wait_queue_head_t q_pinwait;	/* dquot pinning wait queue */
} xfs_dquot_t;
=======
enum {
	XFS_QLOWSP_1_PCNT = 0,
	XFS_QLOWSP_3_PCNT,
	XFS_QLOWSP_5_PCNT,
	XFS_QLOWSP_MAX
};

struct xfs_dquot_res {
	/* Total resources allocated and reserved. */
	xfs_qcnt_t		reserved;

	/* Total resources allocated. */
	xfs_qcnt_t		count;

	/* Absolute and preferred limits. */
	xfs_qcnt_t		hardlimit;
	xfs_qcnt_t		softlimit;

	/*
	 * For root dquots, this is the default grace period, in seconds.
	 * Otherwise, this is when the quota grace period expires,
	 * in seconds since the Unix epoch.
	 */
	time64_t		timer;
};

static inline bool
xfs_dquot_res_over_limits(
	const struct xfs_dquot_res	*qres)
{
	if ((qres->softlimit && qres->softlimit < qres->reserved) ||
	    (qres->hardlimit && qres->hardlimit < qres->reserved))
		return true;
	return false;
}

/*
 * The incore dquot structure
 */
struct xfs_dquot {
	struct list_head	q_lru;
	struct xfs_mount	*q_mount;
	xfs_dqtype_t		q_type;
	uint16_t		q_flags;
	xfs_dqid_t		q_id;
	uint			q_nrefs;
	int			q_bufoffset;
	xfs_daddr_t		q_blkno;
	xfs_fileoff_t		q_fileoffset;

	struct xfs_dquot_res	q_blk;	/* regular blocks */
	struct xfs_dquot_res	q_ino;	/* inodes */
	struct xfs_dquot_res	q_rtb;	/* realtime blocks */

	struct xfs_dq_logitem	q_logitem;

	xfs_qcnt_t		q_prealloc_lo_wmark;
	xfs_qcnt_t		q_prealloc_hi_wmark;
	int64_t			q_low_space[XFS_QLOWSP_MAX];
	struct mutex		q_qlock;
	struct completion	q_flush;
	atomic_t		q_pincount;
	struct wait_queue_head	q_pinwait;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Lock hierarchy for q_qlock:
 *	XFS_QLOCK_NORMAL is the implicit default,
<<<<<<< HEAD
 * 	XFS_QLOCK_NESTED is the dquot with the higher id in xfs_dqlock2
=======
 *	XFS_QLOCK_NESTED is the dquot with the higher id in xfs_dqlock2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
enum {
	XFS_QLOCK_NORMAL = 0,
	XFS_QLOCK_NESTED,
};

/*
<<<<<<< HEAD
 * Manage the q_flush completion queue embedded in the dquot.  This completion
 * queue synchronizes processes attempting to flush the in-core dquot back to
 * disk.
 */
static inline void xfs_dqflock(xfs_dquot_t *dqp)
=======
 * Manage the q_flush completion queue embedded in the dquot. This completion
 * queue synchronizes processes attempting to flush the in-core dquot back to
 * disk.
 */
static inline void xfs_dqflock(struct xfs_dquot *dqp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	wait_for_completion(&dqp->q_flush);
}

<<<<<<< HEAD
static inline int xfs_dqflock_nowait(xfs_dquot_t *dqp)
=======
static inline bool xfs_dqflock_nowait(struct xfs_dquot *dqp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return try_wait_for_completion(&dqp->q_flush);
}

<<<<<<< HEAD
static inline void xfs_dqfunlock(xfs_dquot_t *dqp)
=======
static inline void xfs_dqfunlock(struct xfs_dquot *dqp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	complete(&dqp->q_flush);
}

static inline int xfs_dqlock_nowait(struct xfs_dquot *dqp)
{
	return mutex_trylock(&dqp->q_qlock);
}

static inline void xfs_dqlock(struct xfs_dquot *dqp)
{
	mutex_lock(&dqp->q_qlock);
}

static inline void xfs_dqunlock(struct xfs_dquot *dqp)
{
	mutex_unlock(&dqp->q_qlock);
}

<<<<<<< HEAD
static inline int xfs_this_quota_on(struct xfs_mount *mp, int type)
{
	switch (type & XFS_DQ_ALLTYPES) {
	case XFS_DQ_USER:
		return XFS_IS_UQUOTA_ON(mp);
	case XFS_DQ_GROUP:
	case XFS_DQ_PROJ:
		return XFS_IS_OQUOTA_ON(mp);
=======
static inline int
xfs_dquot_type(const struct xfs_dquot *dqp)
{
	return dqp->q_type & XFS_DQTYPE_REC_MASK;
}

static inline int xfs_this_quota_on(struct xfs_mount *mp, xfs_dqtype_t type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return XFS_IS_UQUOTA_ON(mp);
	case XFS_DQTYPE_GROUP:
		return XFS_IS_GQUOTA_ON(mp);
	case XFS_DQTYPE_PROJ:
		return XFS_IS_PQUOTA_ON(mp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return 0;
	}
}

<<<<<<< HEAD
static inline xfs_dquot_t *xfs_inode_dquot(struct xfs_inode *ip, int type)
{
	switch (type & XFS_DQ_ALLTYPES) {
	case XFS_DQ_USER:
		return ip->i_udquot;
	case XFS_DQ_GROUP:
	case XFS_DQ_PROJ:
		return ip->i_gdquot;
=======
static inline struct xfs_dquot *xfs_inode_dquot(
	struct xfs_inode	*ip,
	xfs_dqtype_t		type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return ip->i_udquot;
	case XFS_DQTYPE_GROUP:
		return ip->i_gdquot;
	case XFS_DQTYPE_PROJ:
		return ip->i_pdquot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return NULL;
	}
}

<<<<<<< HEAD
#define XFS_DQ_IS_LOCKED(dqp)	(mutex_is_locked(&((dqp)->q_qlock)))
#define XFS_DQ_IS_DIRTY(dqp)	((dqp)->dq_flags & XFS_DQ_DIRTY)
#define XFS_QM_ISUDQ(dqp)	((dqp)->dq_flags & XFS_DQ_USER)
#define XFS_QM_ISPDQ(dqp)	((dqp)->dq_flags & XFS_DQ_PROJ)
#define XFS_QM_ISGDQ(dqp)	((dqp)->dq_flags & XFS_DQ_GROUP)
#define XFS_DQ_TO_QINF(dqp)	((dqp)->q_mount->m_quotainfo)
#define XFS_DQ_TO_QIP(dqp)	(XFS_QM_ISUDQ(dqp) ? \
				 XFS_DQ_TO_QINF(dqp)->qi_uquotaip : \
				 XFS_DQ_TO_QINF(dqp)->qi_gquotaip)

extern int		xfs_qm_dqread(struct xfs_mount *, xfs_dqid_t, uint,
					uint, struct xfs_dquot	**);
extern void		xfs_qm_dqdestroy(xfs_dquot_t *);
extern int		xfs_qm_dqflush(xfs_dquot_t *, uint);
extern void		xfs_qm_dqunpin_wait(xfs_dquot_t *);
extern void		xfs_qm_adjust_dqtimers(xfs_mount_t *,
					xfs_disk_dquot_t *);
extern void		xfs_qm_adjust_dqlimits(xfs_mount_t *,
					xfs_disk_dquot_t *);
extern int		xfs_qm_dqget(xfs_mount_t *, xfs_inode_t *,
					xfs_dqid_t, uint, uint, xfs_dquot_t **);
extern void		xfs_qm_dqput(xfs_dquot_t *);

extern void		xfs_dqlock2(struct xfs_dquot *, struct xfs_dquot *);
extern void		xfs_dqflock_pushbuf_wait(struct xfs_dquot *dqp);
=======
/* Decide if the dquot's limits are actually being enforced. */
static inline bool
xfs_dquot_is_enforced(
	const struct xfs_dquot	*dqp)
{
	switch (xfs_dquot_type(dqp)) {
	case XFS_DQTYPE_USER:
		return XFS_IS_UQUOTA_ENFORCED(dqp->q_mount);
	case XFS_DQTYPE_GROUP:
		return XFS_IS_GQUOTA_ENFORCED(dqp->q_mount);
	case XFS_DQTYPE_PROJ:
		return XFS_IS_PQUOTA_ENFORCED(dqp->q_mount);
	}
	ASSERT(0);
	return false;
}

/*
 * Check whether a dquot is under low free space conditions. We assume the quota
 * is enabled and enforced.
 */
static inline bool xfs_dquot_lowsp(struct xfs_dquot *dqp)
{
	int64_t freesp;

	freesp = dqp->q_blk.hardlimit - dqp->q_blk.reserved;
	if (freesp < dqp->q_low_space[XFS_QLOWSP_1_PCNT])
		return true;

	return false;
}

void xfs_dquot_to_disk(struct xfs_disk_dquot *ddqp, struct xfs_dquot *dqp);

#define XFS_DQ_IS_LOCKED(dqp)	(mutex_is_locked(&((dqp)->q_qlock)))
#define XFS_DQ_IS_DIRTY(dqp)	((dqp)->q_flags & XFS_DQFLAG_DIRTY)

void		xfs_qm_dqdestroy(struct xfs_dquot *dqp);
int		xfs_qm_dqflush(struct xfs_dquot *dqp, struct xfs_buf **bpp);
void		xfs_qm_dqunpin_wait(struct xfs_dquot *dqp);
void		xfs_qm_adjust_dqtimers(struct xfs_dquot *d);
void		xfs_qm_adjust_dqlimits(struct xfs_dquot *d);
xfs_dqid_t	xfs_qm_id_for_quotatype(struct xfs_inode *ip,
				xfs_dqtype_t type);
int		xfs_qm_dqget(struct xfs_mount *mp, xfs_dqid_t id,
				xfs_dqtype_t type, bool can_alloc,
				struct xfs_dquot **dqpp);
int		xfs_qm_dqget_inode(struct xfs_inode *ip, xfs_dqtype_t type,
				bool can_alloc, struct xfs_dquot **dqpp);
int		xfs_qm_dqget_next(struct xfs_mount *mp, xfs_dqid_t id,
				xfs_dqtype_t type, struct xfs_dquot **dqpp);
int		xfs_qm_dqget_uncached(struct xfs_mount *mp,
				xfs_dqid_t id, xfs_dqtype_t type,
				struct xfs_dquot **dqpp);
void		xfs_qm_dqput(struct xfs_dquot *dqp);

void		xfs_dqlock2(struct xfs_dquot *, struct xfs_dquot *);

void		xfs_dquot_set_prealloc_limits(struct xfs_dquot *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct xfs_dquot *xfs_qm_dqhold(struct xfs_dquot *dqp)
{
	xfs_dqlock(dqp);
	dqp->q_nrefs++;
	xfs_dqunlock(dqp);
	return dqp;
}

<<<<<<< HEAD
=======
time64_t xfs_dquot_set_timeout(struct xfs_mount *mp, time64_t timeout);
time64_t xfs_dquot_set_grace_period(time64_t grace);

void xfs_qm_init_dquot_blk(struct xfs_trans *tp, xfs_dqid_t id, xfs_dqtype_t
		type, struct xfs_buf *bp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __XFS_DQUOT_H__ */
