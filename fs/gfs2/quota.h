<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __QUOTA_DOT_H__
#define __QUOTA_DOT_H__

<<<<<<< HEAD
struct gfs2_inode;
struct gfs2_sbd;
struct shrink_control;

#define NO_QUOTA_CHANGE ((u32)-1)

extern int gfs2_quota_hold(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_unhold(struct gfs2_inode *ip);

extern int gfs2_quota_lock(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_unlock(struct gfs2_inode *ip);

extern int gfs2_quota_check(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_change(struct gfs2_inode *ip, s64 change,
			      u32 uid, u32 gid);

extern int gfs2_quota_sync(struct super_block *sb, int type, int wait);
extern int gfs2_quota_refresh(struct gfs2_sbd *sdp, int user, u32 id);

extern int gfs2_quota_init(struct gfs2_sbd *sdp);
extern void gfs2_quota_cleanup(struct gfs2_sbd *sdp);
extern int gfs2_quotad(void *data);

extern void gfs2_wake_up_statfs(struct gfs2_sbd *sdp);

static inline int gfs2_quota_lock_check(struct gfs2_inode *ip)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	int ret;
	if (sdp->sd_args.ar_quota == GFS2_QUOTA_OFF)
		return 0;
	ret = gfs2_quota_lock(ip, NO_QUOTA_CHANGE, NO_QUOTA_CHANGE);
	if (ret)
		return ret;
	if (sdp->sd_args.ar_quota != GFS2_QUOTA_ON)
		return 0;
	ret = gfs2_quota_check(ip, ip->i_inode.i_uid, ip->i_inode.i_gid);
=======
#include <linux/list_lru.h>

struct gfs2_inode;
struct gfs2_sbd;

#define NO_UID_QUOTA_CHANGE INVALID_UID
#define NO_GID_QUOTA_CHANGE INVALID_GID

int gfs2_qa_get(struct gfs2_inode *ip);
void gfs2_qa_put(struct gfs2_inode *ip);
int gfs2_quota_hold(struct gfs2_inode *ip, kuid_t uid, kgid_t gid);
void gfs2_quota_unhold(struct gfs2_inode *ip);

int gfs2_quota_lock(struct gfs2_inode *ip, kuid_t uid, kgid_t gid);
void gfs2_quota_unlock(struct gfs2_inode *ip);

int gfs2_quota_check(struct gfs2_inode *ip, kuid_t uid, kgid_t gid,
		     struct gfs2_alloc_parms *ap);
void gfs2_quota_change(struct gfs2_inode *ip, s64 change,
		       kuid_t uid, kgid_t gid);

int gfs2_quota_sync(struct super_block *sb, int type);
int gfs2_quota_refresh(struct gfs2_sbd *sdp, struct kqid qid);

int gfs2_quota_init(struct gfs2_sbd *sdp);
void gfs2_quota_cleanup(struct gfs2_sbd *sdp);
int gfs2_quotad(void *data);

void gfs2_wake_up_statfs(struct gfs2_sbd *sdp);

static inline int gfs2_quota_lock_check(struct gfs2_inode *ip,
					struct gfs2_alloc_parms *ap)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	int ret;

	ap->allowed = UINT_MAX; /* Assume we are permitted a whole lot */
	if (capable(CAP_SYS_RESOURCE) ||
	    sdp->sd_args.ar_quota == GFS2_QUOTA_OFF)
		return 0;
	ret = gfs2_quota_lock(ip, NO_UID_QUOTA_CHANGE, NO_GID_QUOTA_CHANGE);
	if (ret)
		return ret;
	if (sdp->sd_args.ar_quota == GFS2_QUOTA_ACCOUNT)
		return 0;
	ret = gfs2_quota_check(ip, ip->i_inode.i_uid, ip->i_inode.i_gid, ap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret)
		gfs2_quota_unlock(ip);
	return ret;
}

<<<<<<< HEAD
extern int gfs2_shrink_qd_memory(struct shrinker *shrink,
				 struct shrink_control *sc);
extern const struct quotactl_ops gfs2_quotactl_ops;
=======
extern const struct quotactl_ops gfs2_quotactl_ops;
int __init gfs2_qd_shrinker_init(void);
void gfs2_qd_shrinker_exit(void);
extern struct list_lru gfs2_qd_lru;

void __init gfs2_quota_hash_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __QUOTA_DOT_H__ */
