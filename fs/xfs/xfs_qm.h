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
#ifndef __XFS_QM_H__
#define __XFS_QM_H__

#include "xfs_dquot_item.h"
#include "xfs_dquot.h"
<<<<<<< HEAD
#include "xfs_quota_priv.h"

struct xfs_inode;

extern struct kmem_zone	*xfs_qm_dqtrxzone;

/*
 * This defines the unit of allocation of dquots.
 * Currently, it is just one file system block, and a 4K blk contains 30
 * (136 * 30 = 4080) dquots. It's probably not worth trying to make
 * this more dynamic.
 * XXXsup However, if this number is changed, we have to make sure that we don't
 * implicitly assume that we do allocations in chunks of a single filesystem
 * block in the dquot/xqm code.
 */
#define XFS_DQUOT_CLUSTER_SIZE_FSB	(xfs_filblks_t)1
=======

struct xfs_inode;

extern struct kmem_cache	*xfs_dqtrx_cache;

/*
 * Number of bmaps that we ask from bmapi when doing a quotacheck.
 * We make this restriction to keep the memory usage to a minimum.
 */
#define XFS_DQITER_MAP_SIZE	10

#define XFS_IS_DQUOT_UNINITIALIZED(dqp) ( \
	!dqp->q_blk.hardlimit && \
	!dqp->q_blk.softlimit && \
	!dqp->q_rtb.hardlimit && \
	!dqp->q_rtb.softlimit && \
	!dqp->q_ino.hardlimit && \
	!dqp->q_ino.softlimit && \
	!dqp->q_blk.count && \
	!dqp->q_rtb.count && \
	!dqp->q_ino.count)

struct xfs_quota_limits {
	xfs_qcnt_t		hard;	/* default hard limit */
	xfs_qcnt_t		soft;	/* default soft limit */
	time64_t		time;	/* limit for timers */
};

/* Defaults for each quota type: time limits, warn limits, usage limits */
struct xfs_def_quota {
	struct xfs_quota_limits	blk;
	struct xfs_quota_limits	ino;
	struct xfs_quota_limits	rtb;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Various quota information for individual filesystems.
 * The mount structure keeps a pointer to this.
 */
<<<<<<< HEAD
typedef struct xfs_quotainfo {
	struct radix_tree_root qi_uquota_tree;
	struct radix_tree_root qi_gquota_tree;
	struct mutex qi_tree_lock;
	xfs_inode_t	*qi_uquotaip;	 /* user quota inode */
	xfs_inode_t	*qi_gquotaip;	 /* group quota inode */
	struct list_head qi_lru_list;
	struct mutex	 qi_lru_lock;
	int		 qi_lru_count;
	int		 qi_dquots;
	time_t		 qi_btimelimit;	 /* limit for blks timer */
	time_t		 qi_itimelimit;	 /* limit for inodes timer */
	time_t		 qi_rtbtimelimit;/* limit for rt blks timer */
	xfs_qwarncnt_t	 qi_bwarnlimit;	 /* limit for blks warnings */
	xfs_qwarncnt_t	 qi_iwarnlimit;	 /* limit for inodes warnings */
	xfs_qwarncnt_t	 qi_rtbwarnlimit;/* limit for rt blks warnings */
	struct mutex	 qi_quotaofflock;/* to serialize quotaoff */
	xfs_filblks_t	 qi_dqchunklen;	 /* # BBs in a chunk of dqs */
	uint		 qi_dqperchunk;	 /* # ondisk dqs in above chunk */
	xfs_qcnt_t	 qi_bhardlimit;	 /* default data blk hard limit */
	xfs_qcnt_t	 qi_bsoftlimit;	 /* default data blk soft limit */
	xfs_qcnt_t	 qi_ihardlimit;	 /* default inode count hard limit */
	xfs_qcnt_t	 qi_isoftlimit;	 /* default inode count soft limit */
	xfs_qcnt_t	 qi_rtbhardlimit;/* default realtime blk hard limit */
	xfs_qcnt_t	 qi_rtbsoftlimit;/* default realtime blk soft limit */
	struct shrinker  qi_shrinker;
} xfs_quotainfo_t;

#define XFS_DQUOT_TREE(qi, type) \
	((type & XFS_DQ_USER) ? \
	 &((qi)->qi_uquota_tree) : \
	 &((qi)->qi_gquota_tree))


extern void	xfs_trans_mod_dquot(xfs_trans_t *, xfs_dquot_t *, uint, long);
extern int	xfs_trans_reserve_quota_bydquots(xfs_trans_t *, xfs_mount_t *,
			xfs_dquot_t *, xfs_dquot_t *, long, long, uint);
extern void	xfs_trans_dqjoin(xfs_trans_t *, xfs_dquot_t *);
extern void	xfs_trans_log_dquot(xfs_trans_t *, xfs_dquot_t *);

/*
 * We keep the usr and grp dquots separately so that locking will be easier
 * to do at commit time. All transactions that we know of at this point
 * affect no more than two dquots of one type. Hence, the TRANS_MAXDQS value.
 */
#define XFS_QM_TRANS_MAXDQS		2
typedef struct xfs_dquot_acct {
	xfs_dqtrx_t	dqa_usrdquots[XFS_QM_TRANS_MAXDQS];
	xfs_dqtrx_t	dqa_grpdquots[XFS_QM_TRANS_MAXDQS];
} xfs_dquot_acct_t;
=======
struct xfs_quotainfo {
	struct radix_tree_root	qi_uquota_tree;
	struct radix_tree_root	qi_gquota_tree;
	struct radix_tree_root	qi_pquota_tree;
	struct mutex		qi_tree_lock;
	struct xfs_inode	*qi_uquotaip;	/* user quota inode */
	struct xfs_inode	*qi_gquotaip;	/* group quota inode */
	struct xfs_inode	*qi_pquotaip;	/* project quota inode */
	struct list_lru		qi_lru;
	int			qi_dquots;
	struct mutex		qi_quotaofflock;/* to serialize quotaoff */
	xfs_filblks_t		qi_dqchunklen;	/* # BBs in a chunk of dqs */
	uint			qi_dqperchunk;	/* # ondisk dq in above chunk */
	struct xfs_def_quota	qi_usr_default;
	struct xfs_def_quota	qi_grp_default;
	struct xfs_def_quota	qi_prj_default;
	struct shrinker		*qi_shrinker;

	/* Minimum and maximum quota expiration timestamp values. */
	time64_t		qi_expiry_min;
	time64_t		qi_expiry_max;

	/* Hook to feed quota counter updates to an active online repair. */
	struct xfs_hooks	qi_mod_ino_dqtrx_hooks;
	struct xfs_hooks	qi_apply_dqtrx_hooks;
};

static inline struct radix_tree_root *
xfs_dquot_tree(
	struct xfs_quotainfo	*qi,
	xfs_dqtype_t		type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return &qi->qi_uquota_tree;
	case XFS_DQTYPE_GROUP:
		return &qi->qi_gquota_tree;
	case XFS_DQTYPE_PROJ:
		return &qi->qi_pquota_tree;
	default:
		ASSERT(0);
	}
	return NULL;
}

static inline struct xfs_inode *
xfs_quota_inode(struct xfs_mount *mp, xfs_dqtype_t type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return mp->m_quotainfo->qi_uquotaip;
	case XFS_DQTYPE_GROUP:
		return mp->m_quotainfo->qi_gquotaip;
	case XFS_DQTYPE_PROJ:
		return mp->m_quotainfo->qi_pquotaip;
	default:
		ASSERT(0);
	}
	return NULL;
}

/*
 * Parameters for tracking dqtrx changes on behalf of an inode.  The hook
 * function arg parameter is the field being updated.
 */
struct xfs_mod_ino_dqtrx_params {
	uintptr_t		tx_id;
	xfs_ino_t		ino;
	xfs_dqtype_t		q_type;
	xfs_dqid_t		q_id;
	int64_t			delta;
};

extern void	xfs_trans_mod_dquot(struct xfs_trans *tp, struct xfs_dquot *dqp,
				    uint field, int64_t delta);
extern void	xfs_trans_dqjoin(struct xfs_trans *, struct xfs_dquot *);
extern void	xfs_trans_log_dquot(struct xfs_trans *, struct xfs_dquot *);

/*
 * We keep the usr, grp, and prj dquots separately so that locking will be
 * easier to do at commit time. All transactions that we know of at this point
 * affect no more than two dquots of one type. Hence, the TRANS_MAXDQS value.
 */
enum {
	XFS_QM_TRANS_USR = 0,
	XFS_QM_TRANS_GRP,
	XFS_QM_TRANS_PRJ,
	XFS_QM_TRANS_DQTYPES
};
#define XFS_QM_TRANS_MAXDQS		2
struct xfs_dquot_acct {
	struct xfs_dqtrx	dqs[XFS_QM_TRANS_DQTYPES][XFS_QM_TRANS_MAXDQS];
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Users are allowed to have a usage exceeding their softlimit for
 * a period this long.
 */
#define XFS_QM_BTIMELIMIT	(7 * 24*60*60)          /* 1 week */
#define XFS_QM_RTBTIMELIMIT	(7 * 24*60*60)          /* 1 week */
#define XFS_QM_ITIMELIMIT	(7 * 24*60*60)          /* 1 week */

<<<<<<< HEAD
#define XFS_QM_BWARNLIMIT	5
#define XFS_QM_IWARNLIMIT	5
#define XFS_QM_RTBWARNLIMIT	5

extern void		xfs_qm_destroy_quotainfo(xfs_mount_t *);
extern int		xfs_qm_quotacheck(xfs_mount_t *);
extern int		xfs_qm_write_sb_changes(xfs_mount_t *, __int64_t);

/* dquot stuff */
extern void		xfs_qm_dqpurge_all(xfs_mount_t *, uint);
extern void		xfs_qm_dqrele_all_inodes(xfs_mount_t *, uint);

/* quota ops */
extern int		xfs_qm_scall_trunc_qfiles(xfs_mount_t *, uint);
extern int		xfs_qm_scall_getquota(xfs_mount_t *, xfs_dqid_t, uint,
					fs_disk_quota_t *);
extern int		xfs_qm_scall_setqlim(xfs_mount_t *, xfs_dqid_t, uint,
					fs_disk_quota_t *);
extern int		xfs_qm_scall_getqstat(xfs_mount_t *, fs_quota_stat_t *);
extern int		xfs_qm_scall_quotaon(xfs_mount_t *, uint);
extern int		xfs_qm_scall_quotaoff(xfs_mount_t *, uint);
=======
extern void		xfs_qm_destroy_quotainfo(struct xfs_mount *);

/* quota ops */
extern int		xfs_qm_scall_trunc_qfiles(struct xfs_mount *, uint);
extern int		xfs_qm_scall_getquota(struct xfs_mount *mp,
					xfs_dqid_t id,
					xfs_dqtype_t type,
					struct qc_dqblk *dst);
extern int		xfs_qm_scall_getquota_next(struct xfs_mount *mp,
					xfs_dqid_t *id,
					xfs_dqtype_t type,
					struct qc_dqblk *dst);
extern int		xfs_qm_scall_setqlim(struct xfs_mount *mp,
					xfs_dqid_t id,
					xfs_dqtype_t type,
					struct qc_dqblk *newlim);
extern int		xfs_qm_scall_quotaon(struct xfs_mount *, uint);
extern int		xfs_qm_scall_quotaoff(struct xfs_mount *, uint);

static inline struct xfs_def_quota *
xfs_get_defquota(struct xfs_quotainfo *qi, xfs_dqtype_t type)
{
	switch (type) {
	case XFS_DQTYPE_USER:
		return &qi->qi_usr_default;
	case XFS_DQTYPE_GROUP:
		return &qi->qi_grp_default;
	case XFS_DQTYPE_PROJ:
		return &qi->qi_prj_default;
	default:
		ASSERT(0);
		return NULL;
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __XFS_QM_H__ */
