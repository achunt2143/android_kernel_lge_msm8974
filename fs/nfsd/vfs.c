<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * File operations used by nfsd. Some of these have been ripped from
 * other parts of the kernel because they weren't exported, others
 * are partial duplicates with added or changed functionality.
 *
 * Note that several functions dget() the dentry upon which they want
 * to act, most notably those that create directory entries. Response
 * dentry's are dput()'d if necessary in the release callback.
 * So if you notice code paths that apparently fail to dput() the
 * dentry, don't worry--they have been taken care of.
 *
 * Copyright (C) 1995-1999 Olaf Kirch <okir@monad.swb.de>
 * Zerocpy NFS support (C) 2002 Hirokazu Takahashi <taka@valinux.co.jp>
 */

#include <linux/fs.h>
#include <linux/file.h>
#include <linux/splice.h>
<<<<<<< HEAD
=======
#include <linux/falloc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fcntl.h>
#include <linux/namei.h>
#include <linux/delay.h>
#include <linux/fsnotify.h>
#include <linux/posix_acl_xattr.h>
#include <linux/xattr.h>
#include <linux/jhash.h>
<<<<<<< HEAD
#include <linux/ima.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/exportfs.h>
#include <linux/writeback.h>

#ifdef CONFIG_NFSD_V3
#include "xdr3.h"
#endif /* CONFIG_NFSD_V3 */

#ifdef CONFIG_NFSD_V4
#include "acl.h"
#include "idmap.h"
=======
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/exportfs.h>
#include <linux/writeback.h>
#include <linux/security.h>

#include "xdr3.h"

#ifdef CONFIG_NFSD_V4
#include "../internal.h"
#include "acl.h"
#include "idmap.h"
#include "xdr4.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_NFSD_V4 */

#include "nfsd.h"
#include "vfs.h"
<<<<<<< HEAD

#define NFSDDBG_FACILITY		NFSDDBG_FILEOP


/*
 * This is a cache of readahead params that help us choose the proper
 * readahead strategy. Initially, we set all readahead parameters to 0
 * and let the VFS handle things.
 * If you increase the number of cached files very much, you'll need to
 * add a hash table here.
 */
struct raparms {
	struct raparms		*p_next;
	unsigned int		p_count;
	ino_t			p_ino;
	dev_t			p_dev;
	int			p_set;
	struct file_ra_state	p_ra;
	unsigned int		p_hindex;
};

struct raparm_hbucket {
	struct raparms		*pb_head;
	spinlock_t		pb_lock;
} ____cacheline_aligned_in_smp;

#define RAPARM_HASH_BITS	4
#define RAPARM_HASH_SIZE	(1<<RAPARM_HASH_BITS)
#define RAPARM_HASH_MASK	(RAPARM_HASH_SIZE-1)
static struct raparm_hbucket	raparm_hash[RAPARM_HASH_SIZE];
=======
#include "filecache.h"
#include "trace.h"

#define NFSDDBG_FACILITY		NFSDDBG_FILEOP

/**
 * nfserrno - Map Linux errnos to NFS errnos
 * @errno: POSIX(-ish) error code to be mapped
 *
 * Returns the appropriate (net-endian) nfserr_* (or nfs_ok if errno is 0). If
 * it's an error we don't expect, log it once and return nfserr_io.
 */
__be32
nfserrno (int errno)
{
	static struct {
		__be32	nfserr;
		int	syserr;
	} nfs_errtbl[] = {
		{ nfs_ok, 0 },
		{ nfserr_perm, -EPERM },
		{ nfserr_noent, -ENOENT },
		{ nfserr_io, -EIO },
		{ nfserr_nxio, -ENXIO },
		{ nfserr_fbig, -E2BIG },
		{ nfserr_stale, -EBADF },
		{ nfserr_acces, -EACCES },
		{ nfserr_exist, -EEXIST },
		{ nfserr_xdev, -EXDEV },
		{ nfserr_mlink, -EMLINK },
		{ nfserr_nodev, -ENODEV },
		{ nfserr_notdir, -ENOTDIR },
		{ nfserr_isdir, -EISDIR },
		{ nfserr_inval, -EINVAL },
		{ nfserr_fbig, -EFBIG },
		{ nfserr_nospc, -ENOSPC },
		{ nfserr_rofs, -EROFS },
		{ nfserr_mlink, -EMLINK },
		{ nfserr_nametoolong, -ENAMETOOLONG },
		{ nfserr_notempty, -ENOTEMPTY },
		{ nfserr_dquot, -EDQUOT },
		{ nfserr_stale, -ESTALE },
		{ nfserr_jukebox, -ETIMEDOUT },
		{ nfserr_jukebox, -ERESTARTSYS },
		{ nfserr_jukebox, -EAGAIN },
		{ nfserr_jukebox, -EWOULDBLOCK },
		{ nfserr_jukebox, -ENOMEM },
		{ nfserr_io, -ETXTBSY },
		{ nfserr_notsupp, -EOPNOTSUPP },
		{ nfserr_toosmall, -ETOOSMALL },
		{ nfserr_serverfault, -ESERVERFAULT },
		{ nfserr_serverfault, -ENFILE },
		{ nfserr_io, -EREMOTEIO },
		{ nfserr_stale, -EOPENSTALE },
		{ nfserr_io, -EUCLEAN },
		{ nfserr_perm, -ENOKEY },
		{ nfserr_no_grace, -ENOGRACE},
	};
	int	i;

	for (i = 0; i < ARRAY_SIZE(nfs_errtbl); i++) {
		if (nfs_errtbl[i].syserr == errno)
			return nfs_errtbl[i].nfserr;
	}
	WARN_ONCE(1, "nfsd: non-standard errno: %d\n", errno);
	return nfserr_io;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* 
 * Called from nfsd_lookup and encode_dirent. Check if we have crossed 
 * a mount point.
 * Returns -EAGAIN or -ETIMEDOUT leaving *dpp and *expp unchanged,
 *  or nfs_ok having possibly changed *dpp and *expp
 */
int
nfsd_cross_mnt(struct svc_rqst *rqstp, struct dentry **dpp, 
		        struct svc_export **expp)
{
	struct svc_export *exp = *expp, *exp2 = NULL;
	struct dentry *dentry = *dpp;
	struct path path = {.mnt = mntget(exp->ex_path.mnt),
			    .dentry = dget(dentry)};
<<<<<<< HEAD
	int err = 0;

	err = follow_down(&path);
	if (err < 0)
		goto out;
=======
	unsigned int follow_flags = 0;
	int err = 0;

	if (exp->ex_flags & NFSEXP_CROSSMOUNT)
		follow_flags = LOOKUP_AUTOMOUNT;

	err = follow_down(&path, follow_flags);
	if (err < 0)
		goto out;
	if (path.mnt == exp->ex_path.mnt && path.dentry == dentry &&
	    nfsd_mountpoint(dentry, exp) == 2) {
		/* This is only a mountpoint in some other namespace */
		path_put(&path);
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	exp2 = rqst_exp_get_by_name(rqstp, &path);
	if (IS_ERR(exp2)) {
		err = PTR_ERR(exp2);
		/*
		 * We normally allow NFS clients to continue
		 * "underneath" a mountpoint that is not exported.
		 * The exception is V4ROOT, where no traversal is ever
		 * allowed without an explicit export of the new
		 * directory.
		 */
		if (err == -ENOENT && !(exp->ex_flags & NFSEXP_V4ROOT))
			err = 0;
		path_put(&path);
		goto out;
	}
	if (nfsd_v4client(rqstp) ||
		(exp->ex_flags & NFSEXP_CROSSMOUNT) || EX_NOHIDE(exp2)) {
		/* successfully crossed mount point */
		/*
		 * This is subtle: path.dentry is *not* on path.mnt
		 * at this point.  The only reason we are safe is that
		 * original mnt is pinned down by exp, so we should
		 * put path *before* putting exp
		 */
		*dpp = path.dentry;
		path.dentry = dentry;
		*expp = exp2;
		exp2 = exp;
	}
	path_put(&path);
	exp_put(exp2);
out:
	return err;
}

static void follow_to_parent(struct path *path)
{
	struct dentry *dp;

	while (path->dentry == path->mnt->mnt_root && follow_up(path))
		;
	dp = dget_parent(path->dentry);
	dput(path->dentry);
	path->dentry = dp;
}

static int nfsd_lookup_parent(struct svc_rqst *rqstp, struct dentry *dparent, struct svc_export **exp, struct dentry **dentryp)
{
	struct svc_export *exp2;
	struct path path = {.mnt = mntget((*exp)->ex_path.mnt),
			    .dentry = dget(dparent)};

	follow_to_parent(&path);

	exp2 = rqst_exp_parent(rqstp, &path);
	if (PTR_ERR(exp2) == -ENOENT) {
		*dentryp = dget(dparent);
	} else if (IS_ERR(exp2)) {
		path_put(&path);
		return PTR_ERR(exp2);
	} else {
		*dentryp = dget(path.dentry);
		exp_put(*exp);
		*exp = exp2;
	}
	path_put(&path);
	return 0;
}

/*
 * For nfsd purposes, we treat V4ROOT exports as though there was an
 * export at *every* directory.
<<<<<<< HEAD
 */
int nfsd_mountpoint(struct dentry *dentry, struct svc_export *exp)
{
	if (d_mountpoint(dentry))
		return 1;
	if (nfsd4_is_junction(dentry))
		return 1;
	if (!(exp->ex_flags & NFSEXP_V4ROOT))
		return 0;
	return dentry->d_inode != NULL;
=======
 * We return:
 * '1' if this dentry *must* be an export point,
 * '2' if it might be, if there is really a mount here, and
 * '0' if there is no chance of an export point here.
 */
int nfsd_mountpoint(struct dentry *dentry, struct svc_export *exp)
{
	if (!d_inode(dentry))
		return 0;
	if (exp->ex_flags & NFSEXP_V4ROOT)
		return 1;
	if (nfsd4_is_junction(dentry))
		return 1;
	if (d_managed(dentry))
		/*
		 * Might only be a mountpoint in a different namespace,
		 * but we need to check.
		 */
		return 2;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

__be32
nfsd_lookup_dentry(struct svc_rqst *rqstp, struct svc_fh *fhp,
		   const char *name, unsigned int len,
		   struct svc_export **exp_ret, struct dentry **dentry_ret)
{
	struct svc_export	*exp;
	struct dentry		*dparent;
	struct dentry		*dentry;
	int			host_err;

	dprintk("nfsd: nfsd_lookup(fh %s, %.*s)\n", SVCFH_fmt(fhp), len,name);

	dparent = fhp->fh_dentry;
<<<<<<< HEAD
	exp  = fhp->fh_export;
	exp_get(exp);
=======
	exp = exp_get(fhp->fh_export);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Lookup the name, but don't follow links */
	if (isdotent(name, len)) {
		if (len==1)
			dentry = dget(dparent);
		else if (dparent != exp->ex_path.dentry)
			dentry = dget_parent(dparent);
		else if (!EX_NOHIDE(exp) && !nfsd_v4client(rqstp))
			dentry = dget(dparent); /* .. == . just like at / */
		else {
			/* checking mountpoint crossing is very different when stepping up */
			host_err = nfsd_lookup_parent(rqstp, dparent, &exp, &dentry);
			if (host_err)
				goto out_nfserr;
		}
	} else {
<<<<<<< HEAD
		fh_lock(fhp);
		dentry = lookup_one_len(name, dparent, len);
		host_err = PTR_ERR(dentry);
		if (IS_ERR(dentry))
			goto out_nfserr;
		/*
		 * check if we have crossed a mount point ...
		 */
		if (nfsd_mountpoint(dentry, exp)) {
			if ((host_err = nfsd_cross_mnt(rqstp, &dentry, &exp))) {
=======
		dentry = lookup_one_len_unlocked(name, dparent, len);
		host_err = PTR_ERR(dentry);
		if (IS_ERR(dentry))
			goto out_nfserr;
		if (nfsd_mountpoint(dentry, exp)) {
			host_err = nfsd_cross_mnt(rqstp, &dentry, &exp);
			if (host_err) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dput(dentry);
				goto out_nfserr;
			}
		}
	}
	*dentry_ret = dentry;
	*exp_ret = exp;
	return 0;

out_nfserr:
	exp_put(exp);
	return nfserrno(host_err);
}

<<<<<<< HEAD
/*
=======
/**
 * nfsd_lookup - look up a single path component for nfsd
 *
 * @rqstp:   the request context
 * @fhp:     the file handle of the directory
 * @name:    the component name, or %NULL to look up parent
 * @len:     length of name to examine
 * @resfh:   pointer to pre-initialised filehandle to hold result.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Look up one component of a pathname.
 * N.B. After this call _both_ fhp and resfh need an fh_put
 *
 * If the lookup would cross a mountpoint, and the mounted filesystem
 * is exported to the client with NFSEXP_NOHIDE, then the lookup is
 * accepted as it stands and the mounted directory is
 * returned. Otherwise the covered directory is returned.
 * NOTE: this mountpoint crossing is not supported properly by all
 *   clients and is explicitly disallowed for NFSv3
<<<<<<< HEAD
 *      NeilBrown <neilb@cse.unsw.edu.au>
 */
__be32
nfsd_lookup(struct svc_rqst *rqstp, struct svc_fh *fhp, const char *name,
				unsigned int len, struct svc_fh *resfh)
=======
 *
 */
__be32
nfsd_lookup(struct svc_rqst *rqstp, struct svc_fh *fhp, const char *name,
	    unsigned int len, struct svc_fh *resfh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct svc_export	*exp;
	struct dentry		*dentry;
	__be32 err;

	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_EXEC);
	if (err)
		return err;
	err = nfsd_lookup_dentry(rqstp, fhp, name, len, &exp, &dentry);
	if (err)
		return err;
	err = check_nfsd_access(exp, rqstp);
	if (err)
		goto out;
	/*
	 * Note: we compose the file handle now, but as the
	 * dentry may be negative, it may need to be updated.
	 */
	err = fh_compose(resfh, exp, dentry, fhp);
<<<<<<< HEAD
	if (!err && !dentry->d_inode)
=======
	if (!err && d_really_is_negative(dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = nfserr_noent;
out:
	dput(dentry);
	exp_put(exp);
	return err;
}

<<<<<<< HEAD
static int nfsd_break_lease(struct inode *inode)
{
	if (!S_ISREG(inode->i_mode))
		return 0;
	return break_lease(inode, O_WRONLY | O_NONBLOCK);
=======
static void
commit_reset_write_verifier(struct nfsd_net *nn, struct svc_rqst *rqstp,
			    int err)
{
	switch (err) {
	case -EAGAIN:
	case -ESTALE:
		/*
		 * Neither of these are the result of a problem with
		 * durable storage, so avoid a write verifier reset.
		 */
		break;
	default:
		nfsd_reset_write_verifier(nn);
		trace_nfsd_writeverf_reset(nn, rqstp, err);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Commit metadata changes to stable storage.
 */
static int
<<<<<<< HEAD
commit_metadata(struct svc_fh *fhp)
{
	struct inode *inode = fhp->fh_dentry->d_inode;
	const struct export_operations *export_ops = inode->i_sb->s_export_op;

	if (!EX_ISSYNC(fhp->fh_export))
		return 0;

=======
commit_inode_metadata(struct inode *inode)
{
	const struct export_operations *export_ops = inode->i_sb->s_export_op;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (export_ops->commit_metadata)
		return export_ops->commit_metadata(inode);
	return sync_inode_metadata(inode, 1);
}

<<<<<<< HEAD
=======
static int
commit_metadata(struct svc_fh *fhp)
{
	struct inode *inode = d_inode(fhp->fh_dentry);

	if (!EX_ISSYNC(fhp->fh_export))
		return 0;
	return commit_inode_metadata(inode);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Go over the attributes and take care of the small differences between
 * NFS semantics and what Linux expects.
 */
static void
nfsd_sanitize_attrs(struct inode *inode, struct iattr *iap)
{
<<<<<<< HEAD
	/*
	 * NFSv2 does not differentiate between "set-[ac]time-to-now"
	 * which only requires access, and "set-[ac]time-to-X" which
	 * requires ownership.
	 * So if it looks like it might be "set both to the same time which
	 * is close to now", and if inode_change_ok fails, then we
	 * convert to "set to now" instead of "set to explicit time"
	 *
	 * We only call inode_change_ok as the last test as technically
	 * it is not an interface that we should be using.
	 */
#define BOTH_TIME_SET (ATTR_ATIME_SET | ATTR_MTIME_SET)
#define	MAX_TOUCH_TIME_ERROR (30*60)
	if ((iap->ia_valid & BOTH_TIME_SET) == BOTH_TIME_SET &&
	    iap->ia_mtime.tv_sec == iap->ia_atime.tv_sec) {
		/*
		 * Looks probable.
		 *
		 * Now just make sure time is in the right ballpark.
		 * Solaris, at least, doesn't seem to care what the time
		 * request is.  We require it be within 30 minutes of now.
		 */
		time_t delta = iap->ia_atime.tv_sec - get_seconds();
		if (delta < 0)
			delta = -delta;
		if (delta < MAX_TOUCH_TIME_ERROR &&
		    inode_change_ok(inode, iap) != 0) {
			/*
			 * Turn off ATTR_[AM]TIME_SET but leave ATTR_[AM]TIME.
			 * This will cause notify_change to set these times
			 * to "now"
			 */
			iap->ia_valid &= ~BOTH_TIME_SET;
		}
	}
=======
	/* Ignore mode updates on symlinks */
	if (S_ISLNK(inode->i_mode))
		iap->ia_valid &= ~ATTR_MODE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* sanitize the mode change */
	if (iap->ia_valid & ATTR_MODE) {
		iap->ia_mode &= S_IALLUGO;
		iap->ia_mode |= (inode->i_mode & ~S_IALLUGO);
	}

	/* Revoke setuid/setgid on chown */
	if (!S_ISDIR(inode->i_mode) &&
<<<<<<< HEAD
	    (((iap->ia_valid & ATTR_UID) && iap->ia_uid != inode->i_uid) ||
	     ((iap->ia_valid & ATTR_GID) && iap->ia_gid != inode->i_gid))) {
=======
	    ((iap->ia_valid & ATTR_UID) || (iap->ia_valid & ATTR_GID))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		iap->ia_valid |= ATTR_KILL_PRIV;
		if (iap->ia_valid & ATTR_MODE) {
			/* we're setting mode too, just clear the s*id bits */
			iap->ia_mode &= ~S_ISUID;
			if (iap->ia_mode & S_IXGRP)
				iap->ia_mode &= ~S_ISGID;
		} else {
			/* set ATTR_KILL_* bits and let VFS handle it */
<<<<<<< HEAD
			iap->ia_valid |= (ATTR_KILL_SUID | ATTR_KILL_SGID);
=======
			iap->ia_valid |= ATTR_KILL_SUID;
			iap->ia_valid |=
				setattr_should_drop_sgid(&nop_mnt_idmap, inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

static __be32
nfsd_get_write_access(struct svc_rqst *rqstp, struct svc_fh *fhp,
		struct iattr *iap)
{
<<<<<<< HEAD
	struct inode *inode = fhp->fh_dentry->d_inode;
	int host_err;
=======
	struct inode *inode = d_inode(fhp->fh_dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (iap->ia_size < inode->i_size) {
		__be32 err;

		err = nfsd_permission(rqstp, fhp->fh_export, fhp->fh_dentry,
				NFSD_MAY_TRUNC | NFSD_MAY_OWNER_OVERRIDE);
		if (err)
			return err;
	}
<<<<<<< HEAD

	host_err = get_write_access(inode);
	if (host_err)
		goto out_nfserrno;

	host_err = locks_verify_truncate(inode, NULL, iap->ia_size);
	if (host_err)
		goto out_put_write_access;
	return 0;

out_put_write_access:
	put_write_access(inode);
out_nfserrno:
	return nfserrno(host_err);
}

/*
 * Set various file attributes.  After this call fhp needs an fh_put.
 */
__be32
nfsd_setattr(struct svc_rqst *rqstp, struct svc_fh *fhp, struct iattr *iap,
	     int check_guard, time_t guardtime)
{
	struct dentry	*dentry;
	struct inode	*inode;
	int		accmode = NFSD_MAY_SATTR;
	umode_t		ftype = 0;
	__be32		err;
	int		host_err;
	bool		get_write_count;
	int		size_change = 0;

	if (iap->ia_valid & (ATTR_ATIME | ATTR_MTIME | ATTR_SIZE))
		accmode |= NFSD_MAY_WRITE|NFSD_MAY_OWNER_OVERRIDE;
	if (iap->ia_valid & ATTR_SIZE)
		ftype = S_IFREG;
=======
	return nfserrno(get_write_access(inode));
}

static int __nfsd_setattr(struct dentry *dentry, struct iattr *iap)
{
	int host_err;

	if (iap->ia_valid & ATTR_SIZE) {
		/*
		 * RFC5661, Section 18.30.4:
		 *   Changing the size of a file with SETATTR indirectly
		 *   changes the time_modify and change attributes.
		 *
		 * (and similar for the older RFCs)
		 */
		struct iattr size_attr = {
			.ia_valid	= ATTR_SIZE | ATTR_CTIME | ATTR_MTIME,
			.ia_size	= iap->ia_size,
		};

		if (iap->ia_size < 0)
			return -EFBIG;

		host_err = notify_change(&nop_mnt_idmap, dentry, &size_attr, NULL);
		if (host_err)
			return host_err;
		iap->ia_valid &= ~ATTR_SIZE;

		/*
		 * Avoid the additional setattr call below if the only other
		 * attribute that the client sends is the mtime, as we update
		 * it as part of the size change above.
		 */
		if ((iap->ia_valid & ~ATTR_MTIME) == 0)
			return 0;
	}

	if (!iap->ia_valid)
		return 0;

	iap->ia_valid |= ATTR_CTIME;
	return notify_change(&nop_mnt_idmap, dentry, iap, NULL);
}

/**
 * nfsd_setattr - Set various file attributes.
 * @rqstp: controlling RPC transaction
 * @fhp: filehandle of target
 * @attr: attributes to set
 * @guardtime: do not act if ctime.tv_sec does not match this timestamp
 *
 * This call may adjust the contents of @attr (in particular, this
 * call may change the bits in the na_iattr.ia_valid field).
 *
 * Returns nfs_ok on success, otherwise an NFS status code is
 * returned. Caller must release @fhp by calling fh_put in either
 * case.
 */
__be32
nfsd_setattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
	     struct nfsd_attrs *attr, const struct timespec64 *guardtime)
{
	struct dentry	*dentry;
	struct inode	*inode;
	struct iattr	*iap = attr->na_iattr;
	int		accmode = NFSD_MAY_SATTR;
	umode_t		ftype = 0;
	__be32		err;
	int		host_err = 0;
	bool		get_write_count;
	bool		size_change = (iap->ia_valid & ATTR_SIZE);
	int		retries;

	if (iap->ia_valid & ATTR_SIZE) {
		accmode |= NFSD_MAY_WRITE|NFSD_MAY_OWNER_OVERRIDE;
		ftype = S_IFREG;
	}

	/*
	 * If utimes(2) and friends are called with times not NULL, we should
	 * not set NFSD_MAY_WRITE bit. Otherwise fh_verify->nfsd_permission
	 * will return EACCES, when the caller's effective UID does not match
	 * the owner of the file, and the caller is not privileged. In this
	 * situation, we should return EPERM(notify_change will return this).
	 */
	if (iap->ia_valid & (ATTR_ATIME | ATTR_MTIME)) {
		accmode |= NFSD_MAY_OWNER_OVERRIDE;
		if (!(iap->ia_valid & (ATTR_ATIME_SET | ATTR_MTIME_SET)))
			accmode |= NFSD_MAY_WRITE;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Callers that do fh_verify should do the fh_want_write: */
	get_write_count = !fhp->fh_dentry;

	/* Get inode */
	err = fh_verify(rqstp, fhp, ftype, accmode);
	if (err)
<<<<<<< HEAD
		goto out;
	if (get_write_count) {
		host_err = fh_want_write(fhp);
		if (host_err)
			return nfserrno(host_err);
	}

	dentry = fhp->fh_dentry;
	inode = dentry->d_inode;

	/* Ignore any mode updates on symlinks */
	if (S_ISLNK(inode->i_mode))
		iap->ia_valid &= ~ATTR_MODE;

	if (!iap->ia_valid)
		goto out;
=======
		return err;
	if (get_write_count) {
		host_err = fh_want_write(fhp);
		if (host_err)
			goto out;
	}

	dentry = fhp->fh_dentry;
	inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	nfsd_sanitize_attrs(inode, iap);

	/*
	 * The size case is special, it changes the file in addition to the
<<<<<<< HEAD
	 * attributes.
	 */
	if (iap->ia_valid & ATTR_SIZE) {
		err = nfsd_get_write_access(rqstp, fhp, iap);
		if (err)
			goto out;
		size_change = 1;
	}

	iap->ia_valid |= ATTR_CTIME;

	if (check_guard && guardtime != inode->i_ctime.tv_sec) {
		err = nfserr_notsync;
		goto out_put_write_access;
	}

	host_err = nfsd_break_lease(inode);
	if (host_err)
		goto out_put_write_access_nfserror;

	fh_lock(fhp);
	host_err = notify_change(dentry, iap);
	fh_unlock(fhp);

out_put_write_access_nfserror:
	err = nfserrno(host_err);
out_put_write_access:
	if (size_change)
		put_write_access(inode);
	if (!err)
		commit_metadata(fhp);
out:
	return err;
}

#if defined(CONFIG_NFSD_V2_ACL) || \
    defined(CONFIG_NFSD_V3_ACL) || \
    defined(CONFIG_NFSD_V4)
static ssize_t nfsd_getxattr(struct dentry *dentry, char *key, void **buf)
{
	ssize_t buflen;
	ssize_t ret;

	buflen = vfs_getxattr(dentry, key, NULL, 0);
	if (buflen <= 0)
		return buflen;

	*buf = kmalloc(buflen, GFP_KERNEL);
	if (!*buf)
		return -ENOMEM;

	ret = vfs_getxattr(dentry, key, *buf, buflen);
	if (ret < 0)
		kfree(*buf);
	return ret;
}
#endif

#if defined(CONFIG_NFSD_V4)
static int
set_nfsv4_acl_one(struct dentry *dentry, struct posix_acl *pacl, char *key)
{
	int len;
	size_t buflen;
	char *buf = NULL;
	int error = 0;

	if (!pacl)
		return vfs_setxattr(dentry, key, NULL, 0, 0);

	buflen = posix_acl_xattr_size(pacl->a_count);
	buf = kmalloc(buflen, GFP_KERNEL);
	error = -ENOMEM;
	if (buf == NULL)
		goto out;

	len = posix_acl_to_xattr(pacl, buf, buflen);
	if (len < 0) {
		error = len;
		goto out;
	}

	error = vfs_setxattr(dentry, key, buf, len, 0);
out:
	kfree(buf);
	return error;
}

__be32
nfsd4_set_nfs4_acl(struct svc_rqst *rqstp, struct svc_fh *fhp,
    struct nfs4_acl *acl)
{
	__be32 error;
	int host_error;
	struct dentry *dentry;
	struct inode *inode;
	struct posix_acl *pacl = NULL, *dpacl = NULL;
	unsigned int flags = 0;

	/* Get inode */
	error = fh_verify(rqstp, fhp, 0, NFSD_MAY_SATTR);
	if (error)
		return error;

	dentry = fhp->fh_dentry;
	inode = dentry->d_inode;
	if (S_ISDIR(inode->i_mode))
		flags = NFS4_ACL_DIR;

	host_error = nfs4_acl_nfsv4_to_posix(acl, &pacl, &dpacl, flags);
	if (host_error == -EINVAL) {
		return nfserr_attrnotsupp;
	} else if (host_error < 0)
		goto out_nfserr;

	host_error = set_nfsv4_acl_one(dentry, pacl, POSIX_ACL_XATTR_ACCESS);
	if (host_error < 0)
		goto out_release;

	if (S_ISDIR(inode->i_mode))
		host_error = set_nfsv4_acl_one(dentry, dpacl, POSIX_ACL_XATTR_DEFAULT);

out_release:
	posix_acl_release(pacl);
	posix_acl_release(dpacl);
out_nfserr:
	if (host_error == -EOPNOTSUPP)
		return nfserr_attrnotsupp;
	else
		return nfserrno(host_error);
}

static struct posix_acl *
_get_posix_acl(struct dentry *dentry, char *key)
{
	void *buf = NULL;
	struct posix_acl *pacl = NULL;
	int buflen;

	buflen = nfsd_getxattr(dentry, key, &buf);
	if (!buflen)
		buflen = -ENODATA;
	if (buflen <= 0)
		return ERR_PTR(buflen);

	pacl = posix_acl_from_xattr(buf, buflen);
	kfree(buf);
	return pacl;
}

int
nfsd4_get_nfs4_acl(struct svc_rqst *rqstp, struct dentry *dentry, struct nfs4_acl **acl)
{
	struct inode *inode = dentry->d_inode;
	int error = 0;
	struct posix_acl *pacl = NULL, *dpacl = NULL;
	unsigned int flags = 0;

	pacl = _get_posix_acl(dentry, POSIX_ACL_XATTR_ACCESS);
	if (IS_ERR(pacl) && PTR_ERR(pacl) == -ENODATA)
		pacl = posix_acl_from_mode(inode->i_mode, GFP_KERNEL);
	if (IS_ERR(pacl)) {
		error = PTR_ERR(pacl);
		pacl = NULL;
		goto out;
	}

	if (S_ISDIR(inode->i_mode)) {
		dpacl = _get_posix_acl(dentry, POSIX_ACL_XATTR_DEFAULT);
		if (IS_ERR(dpacl) && PTR_ERR(dpacl) == -ENODATA)
			dpacl = NULL;
		else if (IS_ERR(dpacl)) {
			error = PTR_ERR(dpacl);
			dpacl = NULL;
			goto out;
		}
		flags = NFS4_ACL_DIR;
	}

	*acl = nfs4_acl_posix_to_nfsv4(pacl, dpacl, flags);
	if (IS_ERR(*acl)) {
		error = PTR_ERR(*acl);
		*acl = NULL;
	}
 out:
	posix_acl_release(pacl);
	posix_acl_release(dpacl);
	return error;
}

=======
	 * attributes, and file systems don't expect it to be mixed with
	 * "random" attribute changes.  We thus split out the size change
	 * into a separate call to ->setattr, and do the rest as a separate
	 * setattr call.
	 */
	if (size_change) {
		err = nfsd_get_write_access(rqstp, fhp, iap);
		if (err)
			return err;
	}

	inode_lock(inode);
	err = fh_fill_pre_attrs(fhp);
	if (err)
		goto out_unlock;

	if (guardtime) {
		struct timespec64 ctime = inode_get_ctime(inode);
		if ((u32)guardtime->tv_sec != (u32)ctime.tv_sec ||
		    guardtime->tv_nsec != ctime.tv_nsec) {
			err = nfserr_notsync;
			goto out_fill_attrs;
		}
	}

	for (retries = 1;;) {
		struct iattr attrs;

		/*
		 * notify_change() can alter its iattr argument, making
		 * @iap unsuitable for submission multiple times. Make a
		 * copy for every loop iteration.
		 */
		attrs = *iap;
		host_err = __nfsd_setattr(dentry, &attrs);
		if (host_err != -EAGAIN || !retries--)
			break;
		if (!nfsd_wait_for_delegreturn(rqstp, inode))
			break;
	}
	if (attr->na_seclabel && attr->na_seclabel->len)
		attr->na_labelerr = security_inode_setsecctx(dentry,
			attr->na_seclabel->data, attr->na_seclabel->len);
	if (IS_ENABLED(CONFIG_FS_POSIX_ACL) && attr->na_pacl)
		attr->na_aclerr = set_posix_acl(&nop_mnt_idmap,
						dentry, ACL_TYPE_ACCESS,
						attr->na_pacl);
	if (IS_ENABLED(CONFIG_FS_POSIX_ACL) &&
	    !attr->na_aclerr && attr->na_dpacl && S_ISDIR(inode->i_mode))
		attr->na_aclerr = set_posix_acl(&nop_mnt_idmap,
						dentry, ACL_TYPE_DEFAULT,
						attr->na_dpacl);
out_fill_attrs:
	/*
	 * RFC 1813 Section 3.3.2 does not mandate that an NFS server
	 * returns wcc_data for SETATTR. Some client implementations
	 * depend on receiving wcc_data, however, to sort out partial
	 * updates (eg., the client requested that size and mode be
	 * modified, but the server changed only the file mode).
	 */
	fh_fill_post_attrs(fhp);
out_unlock:
	inode_unlock(inode);
	if (size_change)
		put_write_access(inode);
out:
	if (!host_err)
		host_err = commit_metadata(fhp);
	return err != 0 ? err : nfserrno(host_err);
}

#if defined(CONFIG_NFSD_V4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * NFS junction information is stored in an extended attribute.
 */
#define NFSD_JUNCTION_XATTR_NAME	XATTR_TRUSTED_PREFIX "junction.nfs"

/**
 * nfsd4_is_junction - Test if an object could be an NFS junction
 *
 * @dentry: object to test
 *
 * Returns 1 if "dentry" appears to contain NFS junction information.
 * Otherwise 0 is returned.
 */
int nfsd4_is_junction(struct dentry *dentry)
{
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
=======
	struct inode *inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (inode == NULL)
		return 0;
	if (inode->i_mode & S_IXUGO)
		return 0;
	if (!(inode->i_mode & S_ISVTX))
		return 0;
<<<<<<< HEAD
	if (vfs_getxattr(dentry, NFSD_JUNCTION_XATTR_NAME, NULL, 0) <= 0)
		return 0;
	return 1;
}
#endif /* defined(CONFIG_NFSD_V4) */

#ifdef CONFIG_NFSD_V3
=======
	if (vfs_getxattr(&nop_mnt_idmap, dentry, NFSD_JUNCTION_XATTR_NAME,
			 NULL, 0) <= 0)
		return 0;
	return 1;
}

static struct nfsd4_compound_state *nfsd4_get_cstate(struct svc_rqst *rqstp)
{
	return &((struct nfsd4_compoundres *)rqstp->rq_resp)->cstate;
}

__be32 nfsd4_clone_file_range(struct svc_rqst *rqstp,
		struct nfsd_file *nf_src, u64 src_pos,
		struct nfsd_file *nf_dst, u64 dst_pos,
		u64 count, bool sync)
{
	struct file *src = nf_src->nf_file;
	struct file *dst = nf_dst->nf_file;
	errseq_t since;
	loff_t cloned;
	__be32 ret = 0;

	since = READ_ONCE(dst->f_wb_err);
	cloned = vfs_clone_file_range(src, src_pos, dst, dst_pos, count, 0);
	if (cloned < 0) {
		ret = nfserrno(cloned);
		goto out_err;
	}
	if (count && cloned != count) {
		ret = nfserrno(-EINVAL);
		goto out_err;
	}
	if (sync) {
		loff_t dst_end = count ? dst_pos + count - 1 : LLONG_MAX;
		int status = vfs_fsync_range(dst, dst_pos, dst_end, 0);

		if (!status)
			status = filemap_check_wb_err(dst->f_mapping, since);
		if (!status)
			status = commit_inode_metadata(file_inode(src));
		if (status < 0) {
			struct nfsd_net *nn = net_generic(nf_dst->nf_net,
							  nfsd_net_id);

			trace_nfsd_clone_file_range_err(rqstp,
					&nfsd4_get_cstate(rqstp)->save_fh,
					src_pos,
					&nfsd4_get_cstate(rqstp)->current_fh,
					dst_pos,
					count, status);
			commit_reset_write_verifier(nn, rqstp, status);
			ret = nfserrno(status);
		}
	}
out_err:
	return ret;
}

ssize_t nfsd_copy_file_range(struct file *src, u64 src_pos, struct file *dst,
			     u64 dst_pos, u64 count)
{
	ssize_t ret;

	/*
	 * Limit copy to 4MB to prevent indefinitely blocking an nfsd
	 * thread and client rpc slot.  The choice of 4MB is somewhat
	 * arbitrary.  We might instead base this on r/wsize, or make it
	 * tunable, or use a time instead of a byte limit, or implement
	 * asynchronous copy.  In theory a client could also recognize a
	 * limit like this and pipeline multiple COPY requests.
	 */
	count = min_t(u64, count, 1 << 22);
	ret = vfs_copy_file_range(src, src_pos, dst, dst_pos, count, 0);

	if (ret == -EOPNOTSUPP || ret == -EXDEV)
		ret = vfs_copy_file_range(src, src_pos, dst, dst_pos, count,
					  COPY_FILE_SPLICE);
	return ret;
}

__be32 nfsd4_vfs_fallocate(struct svc_rqst *rqstp, struct svc_fh *fhp,
			   struct file *file, loff_t offset, loff_t len,
			   int flags)
{
	int error;

	if (!S_ISREG(file_inode(file)->i_mode))
		return nfserr_inval;

	error = vfs_fallocate(file, flags, offset, len);
	if (!error)
		error = commit_metadata(fhp);

	return nfserrno(error);
}
#endif /* defined(CONFIG_NFSD_V4) */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Check server access rights to a file system object
 */
struct accessmap {
	u32		access;
	int		how;
};
static struct accessmap	nfs3_regaccess[] = {
    {	NFS3_ACCESS_READ,	NFSD_MAY_READ			},
    {	NFS3_ACCESS_EXECUTE,	NFSD_MAY_EXEC			},
    {	NFS3_ACCESS_MODIFY,	NFSD_MAY_WRITE|NFSD_MAY_TRUNC	},
    {	NFS3_ACCESS_EXTEND,	NFSD_MAY_WRITE			},

<<<<<<< HEAD
=======
#ifdef CONFIG_NFSD_V4
    {	NFS4_ACCESS_XAREAD,	NFSD_MAY_READ			},
    {	NFS4_ACCESS_XAWRITE,	NFSD_MAY_WRITE			},
    {	NFS4_ACCESS_XALIST,	NFSD_MAY_READ			},
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    {	0,			0				}
};

static struct accessmap	nfs3_diraccess[] = {
    {	NFS3_ACCESS_READ,	NFSD_MAY_READ			},
    {	NFS3_ACCESS_LOOKUP,	NFSD_MAY_EXEC			},
    {	NFS3_ACCESS_MODIFY,	NFSD_MAY_EXEC|NFSD_MAY_WRITE|NFSD_MAY_TRUNC},
    {	NFS3_ACCESS_EXTEND,	NFSD_MAY_EXEC|NFSD_MAY_WRITE	},
    {	NFS3_ACCESS_DELETE,	NFSD_MAY_REMOVE			},

<<<<<<< HEAD
=======
#ifdef CONFIG_NFSD_V4
    {	NFS4_ACCESS_XAREAD,	NFSD_MAY_READ			},
    {	NFS4_ACCESS_XAWRITE,	NFSD_MAY_WRITE			},
    {	NFS4_ACCESS_XALIST,	NFSD_MAY_READ			},
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    {	0,			0				}
};

static struct accessmap	nfs3_anyaccess[] = {
	/* Some clients - Solaris 2.6 at least, make an access call
	 * to the server to check for access for things like /dev/null
	 * (which really, the server doesn't care about).  So
	 * We provide simple access checking for them, looking
	 * mainly at mode bits, and we make sure to ignore read-only
	 * filesystem checks
	 */
    {	NFS3_ACCESS_READ,	NFSD_MAY_READ			},
    {	NFS3_ACCESS_EXECUTE,	NFSD_MAY_EXEC			},
    {	NFS3_ACCESS_MODIFY,	NFSD_MAY_WRITE|NFSD_MAY_LOCAL_ACCESS	},
    {	NFS3_ACCESS_EXTEND,	NFSD_MAY_WRITE|NFSD_MAY_LOCAL_ACCESS	},

    {	0,			0				}
};

__be32
nfsd_access(struct svc_rqst *rqstp, struct svc_fh *fhp, u32 *access, u32 *supported)
{
	struct accessmap	*map;
	struct svc_export	*export;
	struct dentry		*dentry;
	u32			query, result = 0, sresult = 0;
	__be32			error;

	error = fh_verify(rqstp, fhp, 0, NFSD_MAY_NOP);
	if (error)
		goto out;

	export = fhp->fh_export;
	dentry = fhp->fh_dentry;

<<<<<<< HEAD
	if (S_ISREG(dentry->d_inode->i_mode))
		map = nfs3_regaccess;
	else if (S_ISDIR(dentry->d_inode->i_mode))
=======
	if (d_is_reg(dentry))
		map = nfs3_regaccess;
	else if (d_is_dir(dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		map = nfs3_diraccess;
	else
		map = nfs3_anyaccess;


	query = *access;
	for  (; map->access; map++) {
		if (map->access & query) {
			__be32 err2;

			sresult |= map->access;

			err2 = nfsd_permission(rqstp, export, dentry, map->how);
			switch (err2) {
			case nfs_ok:
				result |= map->access;
				break;
				
			/* the following error codes just mean the access was not allowed,
			 * rather than an error occurred */
			case nfserr_rofs:
			case nfserr_acces:
			case nfserr_perm:
				/* simply don't "or" in the access bit. */
				break;
			default:
				error = err2;
				goto out;
			}
		}
	}
	*access = result;
	if (supported)
		*supported = sresult;

 out:
	return error;
}
<<<<<<< HEAD
#endif /* CONFIG_NFSD_V3 */

static int nfsd_open_break_lease(struct inode *inode, int access)
=======

int nfsd_open_break_lease(struct inode *inode, int access)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int mode;

	if (access & NFSD_MAY_NOT_BREAK_LEASE)
		return 0;
	mode = (access & NFSD_MAY_WRITE) ? O_WRONLY : O_RDONLY;
	return break_lease(inode, mode | O_NONBLOCK);
}

/*
 * Open an existing file or directory.
 * The may_flags argument indicates the type of open (read/write/lock)
 * and additional flags.
 * N.B. After this call fhp needs an fh_put
 */
<<<<<<< HEAD
__be32
nfsd_open(struct svc_rqst *rqstp, struct svc_fh *fhp, umode_t type,
			int may_flags, struct file **filp)
{
	struct dentry	*dentry;
	struct inode	*inode;
	int		flags = O_RDONLY|O_LARGEFILE;
	__be32		err;
	int		host_err = 0;

	validate_process_creds();

	/*
	 * If we get here, then the client has already done an "open",
	 * and (hopefully) checked permission - so allow OWNER_OVERRIDE
	 * in case a chmod has now revoked permission.
	 */
	err = fh_verify(rqstp, fhp, type, may_flags | NFSD_MAY_OWNER_OVERRIDE);
	if (err)
		goto out;

	dentry = fhp->fh_dentry;
	inode = dentry->d_inode;

	/* Disallow write access to files with the append-only bit set
	 * or any access when mandatory locking enabled
	 */
	err = nfserr_perm;
	if (IS_APPEND(inode) && (may_flags & NFSD_MAY_WRITE))
		goto out;
	/*
	 * We must ignore files (but only files) which might have mandatory
	 * locks on them because there is no way to know if the accesser has
	 * the lock.
	 */
	if (S_ISREG((inode)->i_mode) && mandatory_lock(inode))
		goto out;
=======
static int
__nfsd_open(struct svc_rqst *rqstp, struct svc_fh *fhp, umode_t type,
			int may_flags, struct file **filp)
{
	struct path	path;
	struct inode	*inode;
	struct file	*file;
	int		flags = O_RDONLY|O_LARGEFILE;
	int		host_err = -EPERM;

	path.mnt = fhp->fh_export->ex_path.mnt;
	path.dentry = fhp->fh_dentry;
	inode = d_inode(path.dentry);

	if (IS_APPEND(inode) && (may_flags & NFSD_MAY_WRITE))
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!inode->i_fop)
		goto out;

	host_err = nfsd_open_break_lease(inode, may_flags);
	if (host_err) /* NOMEM or WOULDBLOCK */
<<<<<<< HEAD
		goto out_nfserr;
=======
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (may_flags & NFSD_MAY_WRITE) {
		if (may_flags & NFSD_MAY_READ)
			flags = O_RDWR|O_LARGEFILE;
		else
			flags = O_WRONLY|O_LARGEFILE;
	}
<<<<<<< HEAD
	*filp = dentry_open(dget(dentry), mntget(fhp->fh_export->ex_path.mnt),
			    flags, current_cred());
	if (IS_ERR(*filp)) {
		host_err = PTR_ERR(*filp);
		*filp = NULL;
	} else {
		host_err = ima_file_check(*filp, may_flags);

		if (may_flags & NFSD_MAY_64BIT_COOKIE)
			(*filp)->f_mode |= FMODE_64BITHASH;
		else
			(*filp)->f_mode |= FMODE_32BITHASH;
	}

out_nfserr:
	err = nfserrno(host_err);
out:
	validate_process_creds();
	return err;
}

/*
 * Close a file.
 */
void
nfsd_close(struct file *filp)
{
	fput(filp);
}

/*
 * Obtain the readahead parameters for the file
 * specified by (dev, ino).
 */

static inline struct raparms *
nfsd_get_raparms(dev_t dev, ino_t ino)
{
	struct raparms	*ra, **rap, **frap = NULL;
	int depth = 0;
	unsigned int hash;
	struct raparm_hbucket *rab;

	hash = jhash_2words(dev, ino, 0xfeedbeef) & RAPARM_HASH_MASK;
	rab = &raparm_hash[hash];

	spin_lock(&rab->pb_lock);
	for (rap = &rab->pb_head; (ra = *rap); rap = &ra->p_next) {
		if (ra->p_ino == ino && ra->p_dev == dev)
			goto found;
		depth++;
		if (ra->p_count == 0)
			frap = rap;
	}
	depth = nfsdstats.ra_size;
	if (!frap) {	
		spin_unlock(&rab->pb_lock);
		return NULL;
	}
	rap = frap;
	ra = *frap;
	ra->p_dev = dev;
	ra->p_ino = ino;
	ra->p_set = 0;
	ra->p_hindex = hash;
found:
	if (rap != &rab->pb_head) {
		*rap = ra->p_next;
		ra->p_next   = rab->pb_head;
		rab->pb_head = ra;
	}
	ra->p_count++;
	nfsdstats.ra_depth[depth*10/nfsdstats.ra_size]++;
	spin_unlock(&rab->pb_lock);
	return ra;
=======

	file = dentry_open(&path, flags, current_cred());
	if (IS_ERR(file)) {
		host_err = PTR_ERR(file);
		goto out;
	}

	host_err = security_file_post_open(file, may_flags);
	if (host_err) {
		fput(file);
		goto out;
	}

	if (may_flags & NFSD_MAY_64BIT_COOKIE)
		file->f_mode |= FMODE_64BITHASH;
	else
		file->f_mode |= FMODE_32BITHASH;

	*filp = file;
out:
	return host_err;
}

__be32
nfsd_open(struct svc_rqst *rqstp, struct svc_fh *fhp, umode_t type,
		int may_flags, struct file **filp)
{
	__be32 err;
	int host_err;
	bool retried = false;

	/*
	 * If we get here, then the client has already done an "open",
	 * and (hopefully) checked permission - so allow OWNER_OVERRIDE
	 * in case a chmod has now revoked permission.
	 *
	 * Arguably we should also allow the owner override for
	 * directories, but we never have and it doesn't seem to have
	 * caused anyone a problem.  If we were to change this, note
	 * also that our filldir callbacks would need a variant of
	 * lookup_one_len that doesn't check permissions.
	 */
	if (type == S_IFREG)
		may_flags |= NFSD_MAY_OWNER_OVERRIDE;
retry:
	err = fh_verify(rqstp, fhp, type, may_flags);
	if (!err) {
		host_err = __nfsd_open(rqstp, fhp, type, may_flags, filp);
		if (host_err == -EOPENSTALE && !retried) {
			retried = true;
			fh_put(fhp);
			goto retry;
		}
		err = nfserrno(host_err);
	}
	return err;
}

/**
 * nfsd_open_verified - Open a regular file for the filecache
 * @rqstp: RPC request
 * @fhp: NFS filehandle of the file to open
 * @may_flags: internal permission flags
 * @filp: OUT: open "struct file *"
 *
 * Returns zero on success, or a negative errno value.
 */
int
nfsd_open_verified(struct svc_rqst *rqstp, struct svc_fh *fhp, int may_flags,
		   struct file **filp)
{
	return __nfsd_open(rqstp, fhp, S_IFREG, may_flags, filp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Grab and keep cached pages associated with a file in the svc_rqst
<<<<<<< HEAD
 * so that they can be passed to the network sendmsg/sendpage routines
 * directly. They will be released after the sending has completed.
=======
 * so that they can be passed to the network sendmsg routines
 * directly. They will be released after the sending has completed.
 *
 * Return values: Number of bytes consumed, or -EIO if there are no
 * remaining pages in rqstp->rq_pages.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int
nfsd_splice_actor(struct pipe_inode_info *pipe, struct pipe_buffer *buf,
		  struct splice_desc *sd)
{
	struct svc_rqst *rqstp = sd->u.data;
<<<<<<< HEAD
	struct page **pp = rqstp->rq_respages + rqstp->rq_resused;
	struct page *page = buf->page;
	size_t size;

	size = sd->len;

	if (rqstp->rq_res.page_len == 0) {
		get_page(page);
		put_page(*pp);
		*pp = page;
		rqstp->rq_resused++;
		rqstp->rq_res.page_base = buf->offset;
		rqstp->rq_res.page_len = size;
	} else if (page != pp[-1]) {
		get_page(page);
		if (*pp)
			put_page(*pp);
		*pp = page;
		rqstp->rq_resused++;
		rqstp->rq_res.page_len += size;
	} else
		rqstp->rq_res.page_len += size;

	return size;
=======
	struct page *page = buf->page;	// may be a compound one
	unsigned offset = buf->offset;
	struct page *last_page;

	last_page = page + (offset + sd->len - 1) / PAGE_SIZE;
	for (page += offset / PAGE_SIZE; page <= last_page; page++) {
		/*
		 * Skip page replacement when extending the contents of the
		 * current page.  But note that we may get two zero_pages in a
		 * row from shmem.
		 */
		if (page == *(rqstp->rq_next_page - 1) &&
		    offset_in_page(rqstp->rq_res.page_base +
				   rqstp->rq_res.page_len))
			continue;
		if (unlikely(!svc_rqst_replace_page(rqstp, page)))
			return -EIO;
	}
	if (rqstp->rq_res.page_len == 0)	// first call
		rqstp->rq_res.page_base = offset % PAGE_SIZE;
	rqstp->rq_res.page_len += sd->len;
	return sd->len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int nfsd_direct_splice_actor(struct pipe_inode_info *pipe,
				    struct splice_desc *sd)
{
	return __splice_from_pipe(pipe, sd, nfsd_splice_actor);
}

<<<<<<< HEAD
static __be32
nfsd_vfs_read(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file *file,
              loff_t offset, struct kvec *vec, int vlen, unsigned long *count)
{
	mm_segment_t	oldfs;
	__be32		err;
	int		host_err;

	err = nfserr_perm;

	if (file->f_op->splice_read && rqstp->rq_splice_ok) {
		struct splice_desc sd = {
			.len		= 0,
			.total_len	= *count,
			.pos		= offset,
			.u.data		= rqstp,
		};

		rqstp->rq_resused = 1;
		host_err = splice_direct_to_actor(file, &sd, nfsd_direct_splice_actor);
	} else {
		oldfs = get_fs();
		set_fs(KERNEL_DS);
		host_err = vfs_readv(file, (struct iovec __user *)vec, vlen, &offset);
		set_fs(oldfs);
	}

	if (host_err >= 0) {
		nfsdstats.io_read += host_err;
		*count = host_err;
		err = 0;
		fsnotify_access(file);
	} else 
		err = nfserrno(host_err);
	return err;
}

static void kill_suid(struct dentry *dentry)
{
	struct iattr	ia;
	ia.ia_valid = ATTR_KILL_SUID | ATTR_KILL_SGID | ATTR_KILL_PRIV;

	mutex_lock(&dentry->d_inode->i_mutex);
	notify_change(dentry, &ia);
	mutex_unlock(&dentry->d_inode->i_mutex);
=======
static u32 nfsd_eof_on_read(struct file *file, loff_t offset, ssize_t len,
		size_t expected)
{
	if (expected != 0 && len == 0)
		return 1;
	if (offset+len >= i_size_read(file_inode(file)))
		return 1;
	return 0;
}

static __be32 nfsd_finish_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
			       struct file *file, loff_t offset,
			       unsigned long *count, u32 *eof, ssize_t host_err)
{
	if (host_err >= 0) {
		struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

		nfsd_stats_io_read_add(nn, fhp->fh_export, host_err);
		*eof = nfsd_eof_on_read(file, offset, host_err, *count);
		*count = host_err;
		fsnotify_access(file);
		trace_nfsd_read_io_done(rqstp, fhp, offset, *count);
		return 0;
	} else {
		trace_nfsd_read_err(rqstp, fhp, offset, host_err);
		return nfserrno(host_err);
	}
}

/**
 * nfsd_splice_read - Perform a VFS read using a splice pipe
 * @rqstp: RPC transaction context
 * @fhp: file handle of file to be read
 * @file: opened struct file of file to be read
 * @offset: starting byte offset
 * @count: IN: requested number of bytes; OUT: number of bytes read
 * @eof: OUT: set non-zero if operation reached the end of the file
 *
 * Returns nfs_ok on success, otherwise an nfserr stat value is
 * returned.
 */
__be32 nfsd_splice_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
			struct file *file, loff_t offset, unsigned long *count,
			u32 *eof)
{
	struct splice_desc sd = {
		.len		= 0,
		.total_len	= *count,
		.pos		= offset,
		.u.data		= rqstp,
	};
	ssize_t host_err;

	trace_nfsd_read_splice(rqstp, fhp, offset, *count);
	host_err = rw_verify_area(READ, file, &offset, *count);
	if (!host_err)
		host_err = splice_direct_to_actor(file, &sd,
						  nfsd_direct_splice_actor);
	return nfsd_finish_read(rqstp, fhp, file, offset, count, eof, host_err);
}

/**
 * nfsd_iter_read - Perform a VFS read using an iterator
 * @rqstp: RPC transaction context
 * @fhp: file handle of file to be read
 * @file: opened struct file of file to be read
 * @offset: starting byte offset
 * @count: IN: requested number of bytes; OUT: number of bytes read
 * @base: offset in first page of read buffer
 * @eof: OUT: set non-zero if operation reached the end of the file
 *
 * Some filesystems or situations cannot use nfsd_splice_read. This
 * function is the slightly less-performant fallback for those cases.
 *
 * Returns nfs_ok on success, otherwise an nfserr stat value is
 * returned.
 */
__be32 nfsd_iter_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
		      struct file *file, loff_t offset, unsigned long *count,
		      unsigned int base, u32 *eof)
{
	unsigned long v, total;
	struct iov_iter iter;
	loff_t ppos = offset;
	struct page *page;
	ssize_t host_err;

	v = 0;
	total = *count;
	while (total) {
		page = *(rqstp->rq_next_page++);
		rqstp->rq_vec[v].iov_base = page_address(page) + base;
		rqstp->rq_vec[v].iov_len = min_t(size_t, total, PAGE_SIZE - base);
		total -= rqstp->rq_vec[v].iov_len;
		++v;
		base = 0;
	}
	WARN_ON_ONCE(v > ARRAY_SIZE(rqstp->rq_vec));

	trace_nfsd_read_vector(rqstp, fhp, offset, *count);
	iov_iter_kvec(&iter, ITER_DEST, rqstp->rq_vec, v, *count);
	host_err = vfs_iter_read(file, &iter, &ppos, 0);
	return nfsd_finish_read(rqstp, fhp, file, offset, count, eof, host_err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Gathered writes: If another process is currently writing to the file,
 * there's a high chance this is another nfsd (triggered by a bulk write
 * from a client's biod). Rather than syncing the file with each write
 * request, we sleep for 10 msec.
 *
 * I don't know if this roughly approximates C. Juszak's idea of
 * gathered writes, but it's a nice and simple solution (IMHO), and it
 * seems to work:-)
 *
 * Note: we do this only in the NFSv2 case, since v3 and higher have a
 * better tool (separate unstable writes and commits) for solving this
 * problem.
 */
static int wait_for_concurrent_writes(struct file *file)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	static ino_t last_ino;
	static dev_t last_dev;
	int err = 0;

	if (atomic_read(&inode->i_writecount) > 1
	    || (last_ino == inode->i_ino && last_dev == inode->i_sb->s_dev)) {
		dprintk("nfsd: write defer %d\n", task_pid_nr(current));
		msleep(10);
		dprintk("nfsd: write resume %d\n", task_pid_nr(current));
	}

	if (inode->i_state & I_DIRTY) {
		dprintk("nfsd: write sync %d\n", task_pid_nr(current));
		err = vfs_fsync(file, 0);
	}
	last_ino = inode->i_ino;
	last_dev = inode->i_sb->s_dev;
	return err;
}

<<<<<<< HEAD
static __be32
nfsd_vfs_write(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file *file,
				loff_t offset, struct kvec *vec, int vlen,
				unsigned long *cnt, int *stablep)
{
	struct svc_export	*exp;
	struct dentry		*dentry;
	struct inode		*inode;
	mm_segment_t		oldfs;
	__be32			err = 0;
	int			host_err;
	int			stable = *stablep;
	int			use_wgather;

	dentry = file->f_path.dentry;
	inode = dentry->d_inode;
	exp   = fhp->fh_export;

	/*
	 * Request sync writes if
	 *  -	the sync export option has been set, or
	 *  -	the client requested O_SYNC behavior (NFSv3 feature).
	 *  -   The file system doesn't support fsync().
	 * When NFSv2 gathered writes have been configured for this volume,
	 * flushing the data to disk is handled separately below.
	 */
	use_wgather = (rqstp->rq_vers == 2) && EX_WGATHER(exp);

	if (!file->f_op->fsync) {/* COMMIT3 cannot work */
	       stable = 2;
	       *stablep = 2; /* FILE_SYNC */
	}

	if (!EX_ISSYNC(exp))
		stable = 0;
	if (stable && !use_wgather) {
		spin_lock(&file->f_lock);
		file->f_flags |= O_SYNC;
		spin_unlock(&file->f_lock);
	}

	/* Write the data. */
	oldfs = get_fs(); set_fs(KERNEL_DS);
	host_err = vfs_writev(file, (struct iovec __user *)vec, vlen, &offset);
	set_fs(oldfs);
	if (host_err < 0)
		goto out_nfserr;
	*cnt = host_err;
	nfsdstats.io_write += host_err;
	fsnotify_modify(file);

	/* clear setuid/setgid flag after write */
	if (inode->i_mode & (S_ISUID | S_ISGID))
		kill_suid(dentry);

	if (stable && use_wgather)
		host_err = wait_for_concurrent_writes(file);

out_nfserr:
	dprintk("nfsd: write complete host_err=%d\n", host_err);
	if (host_err >= 0)
		err = 0;
	else
		err = nfserrno(host_err);
	return err;
}

/*
 * Read data from a file. count must contain the requested read count
 * on entry. On return, *count contains the number of bytes actually read.
 * N.B. After this call fhp needs an fh_put
 */
__be32 nfsd_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
	loff_t offset, struct kvec *vec, int vlen, unsigned long *count)
{
	struct file *file;
	struct inode *inode;
	struct raparms	*ra;
	__be32 err;

	err = nfsd_open(rqstp, fhp, S_IFREG, NFSD_MAY_READ, &file);
	if (err)
		return err;

	inode = file->f_path.dentry->d_inode;

	/* Get readahead parameters */
	ra = nfsd_get_raparms(inode->i_sb->s_dev, inode->i_ino);

	if (ra && ra->p_set)
		file->f_ra = ra->p_ra;

	err = nfsd_vfs_read(rqstp, fhp, file, offset, vec, vlen, count);

	/* Write back readahead params */
	if (ra) {
		struct raparm_hbucket *rab = &raparm_hash[ra->p_hindex];
		spin_lock(&rab->pb_lock);
		ra->p_ra = file->f_ra;
		ra->p_set = 1;
		ra->p_count--;
		spin_unlock(&rab->pb_lock);
	}

	nfsd_close(file);
	return err;
}

/* As above, but use the provided file descriptor. */
__be32
nfsd_read_file(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file *file,
		loff_t offset, struct kvec *vec, int vlen,
		unsigned long *count)
{
	__be32		err;

	if (file) {
		err = nfsd_permission(rqstp, fhp->fh_export, fhp->fh_dentry,
				NFSD_MAY_READ|NFSD_MAY_OWNER_OVERRIDE);
		if (err)
			goto out;
		err = nfsd_vfs_read(rqstp, fhp, file, offset, vec, vlen, count);
	} else /* Note file may still be NULL in NFSv4 special stateid case: */
		err = nfsd_read(rqstp, fhp, offset, vec, vlen, count);
out:
=======
__be32
nfsd_vfs_write(struct svc_rqst *rqstp, struct svc_fh *fhp, struct nfsd_file *nf,
				loff_t offset, struct kvec *vec, int vlen,
				unsigned long *cnt, int stable,
				__be32 *verf)
{
	struct nfsd_net		*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
	struct file		*file = nf->nf_file;
	struct super_block	*sb = file_inode(file)->i_sb;
	struct svc_export	*exp;
	struct iov_iter		iter;
	errseq_t		since;
	__be32			nfserr;
	int			host_err;
	int			use_wgather;
	loff_t			pos = offset;
	unsigned long		exp_op_flags = 0;
	unsigned int		pflags = current->flags;
	rwf_t			flags = 0;
	bool			restore_flags = false;

	trace_nfsd_write_opened(rqstp, fhp, offset, *cnt);

	if (sb->s_export_op)
		exp_op_flags = sb->s_export_op->flags;

	if (test_bit(RQ_LOCAL, &rqstp->rq_flags) &&
	    !(exp_op_flags & EXPORT_OP_REMOTE_FS)) {
		/*
		 * We want throttling in balance_dirty_pages()
		 * and shrink_inactive_list() to only consider
		 * the backingdev we are writing to, so that nfs to
		 * localhost doesn't cause nfsd to lock up due to all
		 * the client's dirty pages or its congested queue.
		 */
		current->flags |= PF_LOCAL_THROTTLE;
		restore_flags = true;
	}

	exp = fhp->fh_export;
	use_wgather = (rqstp->rq_vers == 2) && EX_WGATHER(exp);

	if (!EX_ISSYNC(exp))
		stable = NFS_UNSTABLE;

	if (stable && !use_wgather)
		flags |= RWF_SYNC;

	iov_iter_kvec(&iter, ITER_SOURCE, vec, vlen, *cnt);
	since = READ_ONCE(file->f_wb_err);
	if (verf)
		nfsd_copy_write_verifier(verf, nn);
	host_err = vfs_iter_write(file, &iter, &pos, flags);
	if (host_err < 0) {
		commit_reset_write_verifier(nn, rqstp, host_err);
		goto out_nfserr;
	}
	*cnt = host_err;
	nfsd_stats_io_write_add(nn, exp, *cnt);
	fsnotify_modify(file);
	host_err = filemap_check_wb_err(file->f_mapping, since);
	if (host_err < 0)
		goto out_nfserr;

	if (stable && use_wgather) {
		host_err = wait_for_concurrent_writes(file);
		if (host_err < 0)
			commit_reset_write_verifier(nn, rqstp, host_err);
	}

out_nfserr:
	if (host_err >= 0) {
		trace_nfsd_write_io_done(rqstp, fhp, offset, *cnt);
		nfserr = nfs_ok;
	} else {
		trace_nfsd_write_err(rqstp, fhp, offset, host_err);
		nfserr = nfserrno(host_err);
	}
	if (restore_flags)
		current_restore_flags(pflags, PF_LOCAL_THROTTLE);
	return nfserr;
}

/**
 * nfsd_read_splice_ok - check if spliced reading is supported
 * @rqstp: RPC transaction context
 *
 * Return values:
 *   %true: nfsd_splice_read() may be used
 *   %false: nfsd_splice_read() must not be used
 *
 * NFS READ normally uses splice to send data in-place. However the
 * data in cache can change after the reply's MIC is computed but
 * before the RPC reply is sent. To prevent the client from
 * rejecting the server-computed MIC in this somewhat rare case, do
 * not use splice with the GSS integrity and privacy services.
 */
bool nfsd_read_splice_ok(struct svc_rqst *rqstp)
{
	switch (svc_auth_flavor(rqstp)) {
	case RPC_AUTH_GSS_KRB5I:
	case RPC_AUTH_GSS_KRB5P:
		return false;
	}
	return true;
}

/**
 * nfsd_read - Read data from a file
 * @rqstp: RPC transaction context
 * @fhp: file handle of file to be read
 * @offset: starting byte offset
 * @count: IN: requested number of bytes; OUT: number of bytes read
 * @eof: OUT: set non-zero if operation reached the end of the file
 *
 * The caller must verify that there is enough space in @rqstp.rq_res
 * to perform this operation.
 *
 * N.B. After this call fhp needs an fh_put
 *
 * Returns nfs_ok on success, otherwise an nfserr stat value is
 * returned.
 */
__be32 nfsd_read(struct svc_rqst *rqstp, struct svc_fh *fhp,
		 loff_t offset, unsigned long *count, u32 *eof)
{
	struct nfsd_file	*nf;
	struct file *file;
	__be32 err;

	trace_nfsd_read_start(rqstp, fhp, offset, *count);
	err = nfsd_file_acquire_gc(rqstp, fhp, NFSD_MAY_READ, &nf);
	if (err)
		return err;

	file = nf->nf_file;
	if (file->f_op->splice_read && nfsd_read_splice_ok(rqstp))
		err = nfsd_splice_read(rqstp, fhp, file, offset, count, eof);
	else
		err = nfsd_iter_read(rqstp, fhp, file, offset, count, 0, eof);

	nfsd_file_put(nf);
	trace_nfsd_read_done(rqstp, fhp, offset, *count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/*
 * Write data to a file.
 * The stable flag requests synchronous writes.
 * N.B. After this call fhp needs an fh_put
 */
__be32
<<<<<<< HEAD
nfsd_write(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file *file,
		loff_t offset, struct kvec *vec, int vlen, unsigned long *cnt,
		int *stablep)
{
	__be32			err = 0;

	if (file) {
		err = nfsd_permission(rqstp, fhp->fh_export, fhp->fh_dentry,
				NFSD_MAY_WRITE|NFSD_MAY_OWNER_OVERRIDE);
		if (err)
			goto out;
		err = nfsd_vfs_write(rqstp, fhp, file, offset, vec, vlen, cnt,
				stablep);
	} else {
		err = nfsd_open(rqstp, fhp, S_IFREG, NFSD_MAY_WRITE, &file);
		if (err)
			goto out;

		if (cnt)
			err = nfsd_vfs_write(rqstp, fhp, file, offset, vec, vlen,
					     cnt, stablep);
		nfsd_close(file);
	}
out:
	return err;
}

#ifdef CONFIG_NFSD_V3
/*
 * Commit all pending writes to stable storage.
 *
 * Note: we only guarantee that data that lies within the range specified
 * by the 'offset' and 'count' parameters will be synced.
 *
 * Unfortunately we cannot lock the file to make sure we return full WCC
 * data to the client, as locking happens lower down in the filesystem.
 */
__be32
nfsd_commit(struct svc_rqst *rqstp, struct svc_fh *fhp,
               loff_t offset, unsigned long count)
{
	struct file	*file;
	loff_t		end = LLONG_MAX;
	__be32		err = nfserr_inval;

	if (offset < 0)
		goto out;
	if (count != 0) {
		end = offset + (loff_t)count - 1;
		if (end < offset)
			goto out;
	}

	err = nfsd_open(rqstp, fhp, S_IFREG,
			NFSD_MAY_WRITE|NFSD_MAY_NOT_BREAK_LEASE, &file);
	if (err)
		goto out;
	if (EX_ISSYNC(fhp->fh_export)) {
		int err2 = vfs_fsync_range(file, offset, end, 0);

		if (err2 != -EINVAL)
			err = nfserrno(err2);
		else
			err = nfserr_notsupp;
	}

	nfsd_close(file);
out:
	return err;
}
#endif /* CONFIG_NFSD_V3 */

static __be32
nfsd_create_setattr(struct svc_rqst *rqstp, struct svc_fh *resfhp,
			struct iattr *iap)
{
	/*
	 * Mode has already been set earlier in create:
	 */
	iap->ia_valid &= ~ATTR_MODE;
=======
nfsd_write(struct svc_rqst *rqstp, struct svc_fh *fhp, loff_t offset,
	   struct kvec *vec, int vlen, unsigned long *cnt, int stable,
	   __be32 *verf)
{
	struct nfsd_file *nf;
	__be32 err;

	trace_nfsd_write_start(rqstp, fhp, offset, *cnt);

	err = nfsd_file_acquire_gc(rqstp, fhp, NFSD_MAY_WRITE, &nf);
	if (err)
		goto out;

	err = nfsd_vfs_write(rqstp, fhp, nf, offset, vec,
			vlen, cnt, stable, verf);
	nfsd_file_put(nf);
out:
	trace_nfsd_write_done(rqstp, fhp, offset, *cnt);
	return err;
}

/**
 * nfsd_commit - Commit pending writes to stable storage
 * @rqstp: RPC request being processed
 * @fhp: NFS filehandle
 * @nf: target file
 * @offset: raw offset from beginning of file
 * @count: raw count of bytes to sync
 * @verf: filled in with the server's current write verifier
 *
 * Note: we guarantee that data that lies within the range specified
 * by the 'offset' and 'count' parameters will be synced. The server
 * is permitted to sync data that lies outside this range at the
 * same time.
 *
 * Unfortunately we cannot lock the file to make sure we return full WCC
 * data to the client, as locking happens lower down in the filesystem.
 *
 * Return values:
 *   An nfsstat value in network byte order.
 */
__be32
nfsd_commit(struct svc_rqst *rqstp, struct svc_fh *fhp, struct nfsd_file *nf,
	    u64 offset, u32 count, __be32 *verf)
{
	__be32			err = nfs_ok;
	u64			maxbytes;
	loff_t			start, end;
	struct nfsd_net		*nn;

	/*
	 * Convert the client-provided (offset, count) range to a
	 * (start, end) range. If the client-provided range falls
	 * outside the maximum file size of the underlying FS,
	 * clamp the sync range appropriately.
	 */
	start = 0;
	end = LLONG_MAX;
	maxbytes = (u64)fhp->fh_dentry->d_sb->s_maxbytes;
	if (offset < maxbytes) {
		start = offset;
		if (count && (offset + count - 1 < maxbytes))
			end = offset + count - 1;
	}

	nn = net_generic(nf->nf_net, nfsd_net_id);
	if (EX_ISSYNC(fhp->fh_export)) {
		errseq_t since = READ_ONCE(nf->nf_file->f_wb_err);
		int err2;

		err2 = vfs_fsync_range(nf->nf_file, start, end, 0);
		switch (err2) {
		case 0:
			nfsd_copy_write_verifier(verf, nn);
			err2 = filemap_check_wb_err(nf->nf_file->f_mapping,
						    since);
			err = nfserrno(err2);
			break;
		case -EINVAL:
			err = nfserr_notsupp;
			break;
		default:
			commit_reset_write_verifier(nn, rqstp, err2);
			err = nfserrno(err2);
		}
	} else
		nfsd_copy_write_verifier(verf, nn);

	return err;
}

/**
 * nfsd_create_setattr - Set a created file's attributes
 * @rqstp: RPC transaction being executed
 * @fhp: NFS filehandle of parent directory
 * @resfhp: NFS filehandle of new object
 * @attrs: requested attributes of new object
 *
 * Returns nfs_ok on success, or an nfsstat in network byte order.
 */
__be32
nfsd_create_setattr(struct svc_rqst *rqstp, struct svc_fh *fhp,
		    struct svc_fh *resfhp, struct nfsd_attrs *attrs)
{
	struct iattr *iap = attrs->na_iattr;
	__be32 status;

	/*
	 * Mode has already been set by file creation.
	 */
	iap->ia_valid &= ~ATTR_MODE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Setting uid/gid works only for root.  Irix appears to
	 * send along the gid on create when it tries to implement
	 * setgid directories via NFS:
	 */
<<<<<<< HEAD
	if (current_fsuid() != 0)
		iap->ia_valid &= ~(ATTR_UID|ATTR_GID);
	if (iap->ia_valid)
		return nfsd_setattr(rqstp, resfhp, iap, 0, (time_t)0);
	return 0;
=======
	if (!uid_eq(current_fsuid(), GLOBAL_ROOT_UID))
		iap->ia_valid &= ~(ATTR_UID|ATTR_GID);

	/*
	 * Callers expect new file metadata to be committed even
	 * if the attributes have not changed.
	 */
	if (iap->ia_valid)
		status = nfsd_setattr(rqstp, resfhp, attrs, NULL);
	else
		status = nfserrno(commit_metadata(resfhp));

	/*
	 * Transactional filesystems had a chance to commit changes
	 * for both parent and child simultaneously making the
	 * following commit_metadata a noop in many cases.
	 */
	if (!status)
		status = nfserrno(commit_metadata(fhp));

	/*
	 * Update the new filehandle to pick up the new attributes.
	 */
	if (!status)
		status = fh_update(resfhp);

	return status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* HPUX client sometimes creates a file in mode 000, and sets size to 0.
 * setting size to 0 may fail for some specific file systems by the permission
 * checking which requires WRITE permission but the mode is 000.
 * we ignore the resizing(to 0) on the just new created file, since the size is
 * 0 after file created.
 *
 * call this only after vfs_create() is called.
 * */
static void
nfsd_check_ignore_resizing(struct iattr *iap)
{
	if ((iap->ia_valid & ATTR_SIZE) && (iap->ia_size == 0))
		iap->ia_valid &= ~ATTR_SIZE;
}

<<<<<<< HEAD
/*
 * Create a file (regular, directory, device, fifo); UNIX sockets 
 * not yet implemented.
 * If the response fh has been verified, the parent directory should
 * already be locked. Note that the parent directory is left locked.
 *
 * N.B. Every call to nfsd_create needs an fh_put for _both_ fhp and resfhp
 */
__be32
nfsd_create(struct svc_rqst *rqstp, struct svc_fh *fhp,
		char *fname, int flen, struct iattr *iap,
		int type, dev_t rdev, struct svc_fh *resfhp)
{
	struct dentry	*dentry, *dchild = NULL;
	struct inode	*dirp;
	__be32		err;
	__be32		err2;
	int		host_err;

	err = nfserr_perm;
	if (!flen)
		goto out;
	err = nfserr_exist;
	if (isdotent(fname, flen))
		goto out;

	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_CREATE);
	if (err)
		goto out;

	dentry = fhp->fh_dentry;
	dirp = dentry->d_inode;

	err = nfserr_notdir;
	if (!dirp->i_op->lookup)
		goto out;
	/*
	 * Check whether the response file handle has been verified yet.
	 * If it has, the parent directory should already be locked.
	 */
	if (!resfhp->fh_dentry) {
		/* called from nfsd_proc_mkdir, or possibly nfsd3_proc_create */
		fh_lock_nested(fhp, I_MUTEX_PARENT);
		dchild = lookup_one_len(fname, dentry, flen);
		host_err = PTR_ERR(dchild);
		if (IS_ERR(dchild))
			goto out_nfserr;
		err = fh_compose(resfhp, fhp->fh_export, dchild, fhp);
		if (err)
			goto out;
	} else {
		/* called from nfsd_proc_create */
		dchild = dget(resfhp->fh_dentry);
		if (!fhp->fh_locked) {
			/* not actually possible */
			printk(KERN_ERR
				"nfsd_create: parent %s/%s not locked!\n",
				dentry->d_parent->d_name.name,
				dentry->d_name.name);
			err = nfserr_io;
			goto out;
		}
	}
	/*
	 * Make sure the child dentry is still negative ...
	 */
	err = nfserr_exist;
	if (dchild->d_inode) {
		dprintk("nfsd_create: dentry %s/%s not negative!\n",
			dentry->d_name.name, dchild->d_name.name);
		goto out; 
	}

=======
/* The parent directory should already be locked: */
__be32
nfsd_create_locked(struct svc_rqst *rqstp, struct svc_fh *fhp,
		   struct nfsd_attrs *attrs,
		   int type, dev_t rdev, struct svc_fh *resfhp)
{
	struct dentry	*dentry, *dchild;
	struct inode	*dirp;
	struct iattr	*iap = attrs->na_iattr;
	__be32		err;
	int		host_err;

	dentry = fhp->fh_dentry;
	dirp = d_inode(dentry);

	dchild = dget(resfhp->fh_dentry);
	err = nfsd_permission(rqstp, fhp->fh_export, dentry, NFSD_MAY_CREATE);
	if (err)
		goto out;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(iap->ia_valid & ATTR_MODE))
		iap->ia_mode = 0;
	iap->ia_mode = (iap->ia_mode & S_IALLUGO) | type;

<<<<<<< HEAD
	err = nfserr_inval;
	if (!S_ISREG(type) && !S_ISDIR(type) && !special_file(type)) {
		printk(KERN_WARNING "nfsd: bad file type %o in nfsd_create\n",
		       type);
		goto out;
	}

	host_err = fh_want_write(fhp);
	if (host_err)
		goto out_nfserr;

	/*
	 * Get the dir op function pointer.
	 */
	err = 0;
	switch (type) {
	case S_IFREG:
		host_err = vfs_create(dirp, dchild, iap->ia_mode, true);
=======
	if (!IS_POSIXACL(dirp))
		iap->ia_mode &= ~current_umask();

	err = 0;
	switch (type) {
	case S_IFREG:
		host_err = vfs_create(&nop_mnt_idmap, dirp, dchild,
				      iap->ia_mode, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!host_err)
			nfsd_check_ignore_resizing(iap);
		break;
	case S_IFDIR:
<<<<<<< HEAD
		host_err = vfs_mkdir(dirp, dchild, iap->ia_mode);
=======
		host_err = vfs_mkdir(&nop_mnt_idmap, dirp, dchild, iap->ia_mode);
		if (!host_err && unlikely(d_unhashed(dchild))) {
			struct dentry *d;
			d = lookup_one_len(dchild->d_name.name,
					   dchild->d_parent,
					   dchild->d_name.len);
			if (IS_ERR(d)) {
				host_err = PTR_ERR(d);
				break;
			}
			if (unlikely(d_is_negative(d))) {
				dput(d);
				err = nfserr_serverfault;
				goto out;
			}
			dput(resfhp->fh_dentry);
			resfhp->fh_dentry = dget(d);
			err = fh_update(resfhp);
			dput(dchild);
			dchild = d;
			if (err)
				goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case S_IFCHR:
	case S_IFBLK:
	case S_IFIFO:
	case S_IFSOCK:
<<<<<<< HEAD
		host_err = vfs_mknod(dirp, dchild, iap->ia_mode, rdev);
		break;
	}
	if (host_err < 0) {
		fh_drop_write(fhp);
		goto out_nfserr;
	}

	err = nfsd_create_setattr(rqstp, resfhp, iap);

	/*
	 * nfsd_setattr already committed the child.  Transactional filesystems
	 * had a chance to commit changes for both parent and child
	 * simultaneously making the following commit_metadata a noop.
	 */
	err2 = nfserrno(commit_metadata(fhp));
	if (err2)
		err = err2;
	fh_drop_write(fhp);
	/*
	 * Update the file handle to get the new inode info.
	 */
	if (!err)
		err = fh_update(resfhp);
out:
	if (dchild && !IS_ERR(dchild))
		dput(dchild);
=======
		host_err = vfs_mknod(&nop_mnt_idmap, dirp, dchild,
				     iap->ia_mode, rdev);
		break;
	default:
		printk(KERN_WARNING "nfsd: bad file type %o in nfsd_create\n",
		       type);
		host_err = -EINVAL;
	}
	if (host_err < 0)
		goto out_nfserr;

	err = nfsd_create_setattr(rqstp, fhp, resfhp, attrs);

out:
	dput(dchild);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;

out_nfserr:
	err = nfserrno(host_err);
	goto out;
}

<<<<<<< HEAD
#ifdef CONFIG_NFSD_V3

static inline int nfsd_create_is_exclusive(int createmode)
{
	return createmode == NFS3_CREATE_EXCLUSIVE
	       || createmode == NFS4_CREATE_EXCLUSIVE4_1;
}

/*
 * NFSv3 and NFSv4 version of nfsd_create
 */
__be32
do_nfsd_create(struct svc_rqst *rqstp, struct svc_fh *fhp,
		char *fname, int flen, struct iattr *iap,
		struct svc_fh *resfhp, int createmode, u32 *verifier,
	        bool *truncp, bool *created)
{
	struct dentry	*dentry, *dchild = NULL;
	struct inode	*dirp;
	__be32		err;
	int		host_err;
	__u32		v_mtime=0, v_atime=0;

	err = nfserr_perm;
	if (!flen)
		goto out;
	err = nfserr_exist;
	if (isdotent(fname, flen))
		goto out;
	if (!(iap->ia_valid & ATTR_MODE))
		iap->ia_mode = 0;
	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_EXEC);
	if (err)
		goto out;

	dentry = fhp->fh_dentry;
	dirp = dentry->d_inode;

	/* Get all the sanity checks out of the way before
	 * we lock the parent. */
	err = nfserr_notdir;
	if (!dirp->i_op->lookup)
		goto out;
	fh_lock_nested(fhp, I_MUTEX_PARENT);

	/*
	 * Compose the response file handle.
	 */
	dchild = lookup_one_len(fname, dentry, flen);
	host_err = PTR_ERR(dchild);
	if (IS_ERR(dchild))
		goto out_nfserr;

	/* If file doesn't exist, check for permissions to create one */
	if (!dchild->d_inode) {
		err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_CREATE);
		if (err)
			goto out;
	}

	err = fh_compose(resfhp, fhp->fh_export, dchild, fhp);
	if (err)
		goto out;

	if (nfsd_create_is_exclusive(createmode)) {
		/* solaris7 gets confused (bugid 4218508) if these have
		 * the high bit set, so just clear the high bits. If this is
		 * ever changed to use different attrs for storing the
		 * verifier, then do_open_lookup() will also need to be fixed
		 * accordingly.
		 */
		v_mtime = verifier[0]&0x7fffffff;
		v_atime = verifier[1]&0x7fffffff;
	}
	
	host_err = fh_want_write(fhp);
	if (host_err)
		goto out_nfserr;
	if (dchild->d_inode) {
		err = 0;

		switch (createmode) {
		case NFS3_CREATE_UNCHECKED:
			if (! S_ISREG(dchild->d_inode->i_mode))
				goto out;
			else if (truncp) {
				/* in nfsv4, we need to treat this case a little
				 * differently.  we don't want to truncate the
				 * file now; this would be wrong if the OPEN
				 * fails for some other reason.  furthermore,
				 * if the size is nonzero, we should ignore it
				 * according to spec!
				 */
				*truncp = (iap->ia_valid & ATTR_SIZE) && !iap->ia_size;
			}
			else {
				iap->ia_valid &= ATTR_SIZE;
				goto set_attr;
			}
			break;
		case NFS3_CREATE_EXCLUSIVE:
			if (   dchild->d_inode->i_mtime.tv_sec == v_mtime
			    && dchild->d_inode->i_atime.tv_sec == v_atime
			    && dchild->d_inode->i_size  == 0 ) {
				if (created)
					*created = 1;
				break;
			}
		case NFS4_CREATE_EXCLUSIVE4_1:
			if (   dchild->d_inode->i_mtime.tv_sec == v_mtime
			    && dchild->d_inode->i_atime.tv_sec == v_atime
			    && dchild->d_inode->i_size  == 0 ) {
				if (created)
					*created = 1;
				goto set_attr;
			}
			 /* fallthru */
		case NFS3_CREATE_GUARDED:
			err = nfserr_exist;
		}
		fh_drop_write(fhp);
		goto out;
	}

	host_err = vfs_create(dirp, dchild, iap->ia_mode, true);
	if (host_err < 0) {
		fh_drop_write(fhp);
		goto out_nfserr;
	}
	if (created)
		*created = 1;

	nfsd_check_ignore_resizing(iap);

	if (nfsd_create_is_exclusive(createmode)) {
		/* Cram the verifier into atime/mtime */
		iap->ia_valid = ATTR_MTIME|ATTR_ATIME
			| ATTR_MTIME_SET|ATTR_ATIME_SET;
		/* XXX someone who knows this better please fix it for nsec */ 
		iap->ia_mtime.tv_sec = v_mtime;
		iap->ia_atime.tv_sec = v_atime;
		iap->ia_mtime.tv_nsec = 0;
		iap->ia_atime.tv_nsec = 0;
	}

 set_attr:
	err = nfsd_create_setattr(rqstp, resfhp, iap);

	/*
	 * nfsd_setattr already committed the child (and possibly also the parent).
	 */
	if (!err)
		err = nfserrno(commit_metadata(fhp));

	fh_drop_write(fhp);
	/*
	 * Update the filehandle to get the new inode info.
	 */
	if (!err)
		err = fh_update(resfhp);

 out:
	fh_unlock(fhp);
	if (dchild && !IS_ERR(dchild))
		dput(dchild);
 	return err;
 
 out_nfserr:
	err = nfserrno(host_err);
	goto out;
}
#endif /* CONFIG_NFSD_V3 */
=======
/*
 * Create a filesystem object (regular, directory, special).
 * Note that the parent directory is left locked.
 *
 * N.B. Every call to nfsd_create needs an fh_put for _both_ fhp and resfhp
 */
__be32
nfsd_create(struct svc_rqst *rqstp, struct svc_fh *fhp,
	    char *fname, int flen, struct nfsd_attrs *attrs,
	    int type, dev_t rdev, struct svc_fh *resfhp)
{
	struct dentry	*dentry, *dchild = NULL;
	__be32		err;
	int		host_err;

	if (isdotent(fname, flen))
		return nfserr_exist;

	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_NOP);
	if (err)
		return err;

	dentry = fhp->fh_dentry;

	host_err = fh_want_write(fhp);
	if (host_err)
		return nfserrno(host_err);

	inode_lock_nested(dentry->d_inode, I_MUTEX_PARENT);
	dchild = lookup_one_len(fname, dentry, flen);
	host_err = PTR_ERR(dchild);
	if (IS_ERR(dchild)) {
		err = nfserrno(host_err);
		goto out_unlock;
	}
	err = fh_compose(resfhp, fhp->fh_export, dchild, fhp);
	/*
	 * We unconditionally drop our ref to dchild as fh_compose will have
	 * already grabbed its own ref for it.
	 */
	dput(dchild);
	if (err)
		goto out_unlock;
	err = fh_fill_pre_attrs(fhp);
	if (err != nfs_ok)
		goto out_unlock;
	err = nfsd_create_locked(rqstp, fhp, attrs, type, rdev, resfhp);
	fh_fill_post_attrs(fhp);
out_unlock:
	inode_unlock(dentry->d_inode);
	return err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Read a symlink. On entry, *lenp must contain the maximum path length that
 * fits into the buffer. On return, it contains the true length.
 * N.B. After this call fhp needs an fh_put
 */
__be32
nfsd_readlink(struct svc_rqst *rqstp, struct svc_fh *fhp, char *buf, int *lenp)
{
<<<<<<< HEAD
	struct inode	*inode;
	mm_segment_t	oldfs;
	__be32		err;
	int		host_err;
	struct path path;

	err = fh_verify(rqstp, fhp, S_IFLNK, NFSD_MAY_NOP);
	if (err)
		goto out;

	path.mnt = fhp->fh_export->ex_path.mnt;
	path.dentry = fhp->fh_dentry;
	inode = path.dentry->d_inode;

	err = nfserr_inval;
	if (!inode->i_op->readlink)
		goto out;

	touch_atime(&path);
	/* N.B. Why does this call need a get_fs()??
	 * Remove the set_fs and watch the fireworks:-) --okir
	 */

	oldfs = get_fs(); set_fs(KERNEL_DS);
	host_err = inode->i_op->readlink(path.dentry, buf, *lenp);
	set_fs(oldfs);

	if (host_err < 0)
		goto out_nfserr;
	*lenp = host_err;
	err = 0;
out:
	return err;

out_nfserr:
	err = nfserrno(host_err);
	goto out;
}

/*
 * Create a symlink and look up its inode
 * N.B. After this call _both_ fhp and resfhp need an fh_put
 */
__be32
nfsd_symlink(struct svc_rqst *rqstp, struct svc_fh *fhp,
				char *fname, int flen,
				char *path,  int plen,
				struct svc_fh *resfhp,
				struct iattr *iap)
=======
	__be32		err;
	const char *link;
	struct path path;
	DEFINE_DELAYED_CALL(done);
	int len;

	err = fh_verify(rqstp, fhp, S_IFLNK, NFSD_MAY_NOP);
	if (unlikely(err))
		return err;

	path.mnt = fhp->fh_export->ex_path.mnt;
	path.dentry = fhp->fh_dentry;

	if (unlikely(!d_is_symlink(path.dentry)))
		return nfserr_inval;

	touch_atime(&path);

	link = vfs_get_link(path.dentry, &done);
	if (IS_ERR(link))
		return nfserrno(PTR_ERR(link));

	len = strlen(link);
	if (len < *lenp)
		*lenp = len;
	memcpy(buf, link, *lenp);
	do_delayed_call(&done);
	return 0;
}

/**
 * nfsd_symlink - Create a symlink and look up its inode
 * @rqstp: RPC transaction being executed
 * @fhp: NFS filehandle of parent directory
 * @fname: filename of the new symlink
 * @flen: length of @fname
 * @path: content of the new symlink (NUL-terminated)
 * @attrs: requested attributes of new object
 * @resfhp: NFS filehandle of new object
 *
 * N.B. After this call _both_ fhp and resfhp need an fh_put
 *
 * Returns nfs_ok on success, or an nfsstat in network byte order.
 */
__be32
nfsd_symlink(struct svc_rqst *rqstp, struct svc_fh *fhp,
	     char *fname, int flen,
	     char *path, struct nfsd_attrs *attrs,
	     struct svc_fh *resfhp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dentry	*dentry, *dnew;
	__be32		err, cerr;
	int		host_err;

	err = nfserr_noent;
<<<<<<< HEAD
	if (!flen || !plen)
=======
	if (!flen || path[0] == '\0')
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	err = nfserr_exist;
	if (isdotent(fname, flen))
		goto out;

	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_CREATE);
	if (err)
		goto out;
<<<<<<< HEAD
	fh_lock(fhp);
	dentry = fhp->fh_dentry;
	dnew = lookup_one_len(fname, dentry, flen);
	host_err = PTR_ERR(dnew);
	if (IS_ERR(dnew))
		goto out_nfserr;

	host_err = fh_want_write(fhp);
	if (host_err)
		goto out_nfserr;

	if (unlikely(path[plen] != 0)) {
		char *path_alloced = kmalloc(plen+1, GFP_KERNEL);
		if (path_alloced == NULL)
			host_err = -ENOMEM;
		else {
			strncpy(path_alloced, path, plen);
			path_alloced[plen] = 0;
			host_err = vfs_symlink(dentry->d_inode, dnew, path_alloced);
			kfree(path_alloced);
		}
	} else
		host_err = vfs_symlink(dentry->d_inode, dnew, path);
	err = nfserrno(host_err);
	if (!err)
		err = nfserrno(commit_metadata(fhp));
	fh_unlock(fhp);

	fh_drop_write(fhp);

	cerr = fh_compose(resfhp, fhp->fh_export, dnew, fhp);
	dput(dnew);
	if (err==0) err = cerr;
out:
	return err;

out_nfserr:
	err = nfserrno(host_err);
	goto out;
=======

	host_err = fh_want_write(fhp);
	if (host_err) {
		err = nfserrno(host_err);
		goto out;
	}

	dentry = fhp->fh_dentry;
	inode_lock_nested(dentry->d_inode, I_MUTEX_PARENT);
	dnew = lookup_one_len(fname, dentry, flen);
	if (IS_ERR(dnew)) {
		err = nfserrno(PTR_ERR(dnew));
		inode_unlock(dentry->d_inode);
		goto out_drop_write;
	}
	err = fh_fill_pre_attrs(fhp);
	if (err != nfs_ok)
		goto out_unlock;
	host_err = vfs_symlink(&nop_mnt_idmap, d_inode(dentry), dnew, path);
	err = nfserrno(host_err);
	cerr = fh_compose(resfhp, fhp->fh_export, dnew, fhp);
	if (!err)
		nfsd_create_setattr(rqstp, fhp, resfhp, attrs);
	fh_fill_post_attrs(fhp);
out_unlock:
	inode_unlock(dentry->d_inode);
	if (!err)
		err = nfserrno(commit_metadata(fhp));
	dput(dnew);
	if (err==0) err = cerr;
out_drop_write:
	fh_drop_write(fhp);
out:
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Create a hardlink
 * N.B. After this call _both_ ffhp and tfhp need an fh_put
 */
__be32
nfsd_link(struct svc_rqst *rqstp, struct svc_fh *ffhp,
				char *name, int len, struct svc_fh *tfhp)
{
	struct dentry	*ddir, *dnew, *dold;
	struct inode	*dirp;
	__be32		err;
	int		host_err;

	err = fh_verify(rqstp, ffhp, S_IFDIR, NFSD_MAY_CREATE);
	if (err)
		goto out;
	err = fh_verify(rqstp, tfhp, 0, NFSD_MAY_NOP);
	if (err)
		goto out;
	err = nfserr_isdir;
<<<<<<< HEAD
	if (S_ISDIR(tfhp->fh_dentry->d_inode->i_mode))
=======
	if (d_is_dir(tfhp->fh_dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	err = nfserr_perm;
	if (!len)
		goto out;
	err = nfserr_exist;
	if (isdotent(name, len))
		goto out;

<<<<<<< HEAD
	fh_lock_nested(ffhp, I_MUTEX_PARENT);
	ddir = ffhp->fh_dentry;
	dirp = ddir->d_inode;

	dnew = lookup_one_len(name, ddir, len);
	host_err = PTR_ERR(dnew);
	if (IS_ERR(dnew))
		goto out_nfserr;

	dold = tfhp->fh_dentry;

	host_err = fh_want_write(tfhp);
	if (host_err) {
		err = nfserrno(host_err);
		goto out_dput;
	}
	err = nfserr_noent;
	if (!dold->d_inode)
		goto out_drop_write;
	host_err = nfsd_break_lease(dold->d_inode);
	if (host_err) {
		err = nfserrno(host_err);
		goto out_drop_write;
	}
	host_err = vfs_link(dold, dirp, dnew);
=======
	host_err = fh_want_write(tfhp);
	if (host_err) {
		err = nfserrno(host_err);
		goto out;
	}

	ddir = ffhp->fh_dentry;
	dirp = d_inode(ddir);
	inode_lock_nested(dirp, I_MUTEX_PARENT);

	dnew = lookup_one_len(name, ddir, len);
	if (IS_ERR(dnew)) {
		err = nfserrno(PTR_ERR(dnew));
		goto out_unlock;
	}

	dold = tfhp->fh_dentry;

	err = nfserr_noent;
	if (d_really_is_negative(dold))
		goto out_dput;
	err = fh_fill_pre_attrs(ffhp);
	if (err != nfs_ok)
		goto out_dput;
	host_err = vfs_link(dold, &nop_mnt_idmap, dirp, dnew, NULL);
	fh_fill_post_attrs(ffhp);
	inode_unlock(dirp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!host_err) {
		err = nfserrno(commit_metadata(ffhp));
		if (!err)
			err = nfserrno(commit_metadata(tfhp));
	} else {
		if (host_err == -EXDEV && rqstp->rq_vers == 2)
			err = nfserr_acces;
		else
			err = nfserrno(host_err);
	}
<<<<<<< HEAD
out_drop_write:
	fh_drop_write(tfhp);
out_dput:
	dput(dnew);
out_unlock:
	fh_unlock(ffhp);
out:
	return err;

out_nfserr:
	err = nfserrno(host_err);
	goto out_unlock;
=======
	dput(dnew);
out_drop_write:
	fh_drop_write(tfhp);
out:
	return err;

out_dput:
	dput(dnew);
out_unlock:
	inode_unlock(dirp);
	goto out_drop_write;
}

static void
nfsd_close_cached_files(struct dentry *dentry)
{
	struct inode *inode = d_inode(dentry);

	if (inode && S_ISREG(inode->i_mode))
		nfsd_file_close_inode_sync(inode);
}

static bool
nfsd_has_cached_files(struct dentry *dentry)
{
	bool		ret = false;
	struct inode *inode = d_inode(dentry);

	if (inode && S_ISREG(inode->i_mode))
		ret = nfsd_file_is_cached(inode);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Rename a file
 * N.B. After this call _both_ ffhp and tfhp need an fh_put
 */
__be32
nfsd_rename(struct svc_rqst *rqstp, struct svc_fh *ffhp, char *fname, int flen,
			    struct svc_fh *tfhp, char *tname, int tlen)
{
	struct dentry	*fdentry, *tdentry, *odentry, *ndentry, *trap;
	struct inode	*fdir, *tdir;
	__be32		err;
	int		host_err;
<<<<<<< HEAD
=======
	bool		close_cached = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = fh_verify(rqstp, ffhp, S_IFDIR, NFSD_MAY_REMOVE);
	if (err)
		goto out;
	err = fh_verify(rqstp, tfhp, S_IFDIR, NFSD_MAY_CREATE);
	if (err)
		goto out;

	fdentry = ffhp->fh_dentry;
<<<<<<< HEAD
	fdir = fdentry->d_inode;

	tdentry = tfhp->fh_dentry;
	tdir = tdentry->d_inode;

	err = (rqstp->rq_vers == 2) ? nfserr_acces : nfserr_xdev;
	if (ffhp->fh_export != tfhp->fh_export)
		goto out;
=======
	fdir = d_inode(fdentry);

	tdentry = tfhp->fh_dentry;
	tdir = d_inode(tdentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = nfserr_perm;
	if (!flen || isdotent(fname, flen) || !tlen || isdotent(tname, tlen))
		goto out;

<<<<<<< HEAD
	/* cannot use fh_lock as we need deadlock protective ordering
	 * so do it by hand */
	trap = lock_rename(tdentry, fdentry);
	ffhp->fh_locked = tfhp->fh_locked = 1;
	fill_pre_wcc(ffhp);
	fill_pre_wcc(tfhp);
=======
	err = (rqstp->rq_vers == 2) ? nfserr_acces : nfserr_xdev;
	if (ffhp->fh_export->ex_path.mnt != tfhp->fh_export->ex_path.mnt)
		goto out;
	if (ffhp->fh_export->ex_path.dentry != tfhp->fh_export->ex_path.dentry)
		goto out;

retry:
	host_err = fh_want_write(ffhp);
	if (host_err) {
		err = nfserrno(host_err);
		goto out;
	}

	trap = lock_rename(tdentry, fdentry);
	if (IS_ERR(trap)) {
		err = (rqstp->rq_vers == 2) ? nfserr_acces : nfserr_xdev;
		goto out_want_write;
	}
	err = fh_fill_pre_attrs(ffhp);
	if (err != nfs_ok)
		goto out_unlock;
	err = fh_fill_pre_attrs(tfhp);
	if (err != nfs_ok)
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	odentry = lookup_one_len(fname, fdentry, flen);
	host_err = PTR_ERR(odentry);
	if (IS_ERR(odentry))
		goto out_nfserr;

	host_err = -ENOENT;
<<<<<<< HEAD
	if (!odentry->d_inode)
=======
	if (d_really_is_negative(odentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_dput_old;
	host_err = -EINVAL;
	if (odentry == trap)
		goto out_dput_old;

	ndentry = lookup_one_len(tname, tdentry, tlen);
	host_err = PTR_ERR(ndentry);
	if (IS_ERR(ndentry))
		goto out_dput_old;
	host_err = -ENOTEMPTY;
	if (ndentry == trap)
		goto out_dput_new;

<<<<<<< HEAD
	host_err = -EXDEV;
	if (ffhp->fh_export->ex_path.mnt != tfhp->fh_export->ex_path.mnt)
		goto out_dput_new;
	host_err = fh_want_write(ffhp);
	if (host_err)
		goto out_dput_new;

	host_err = nfsd_break_lease(odentry->d_inode);
	if (host_err)
		goto out_drop_write;
	if (ndentry->d_inode) {
		host_err = nfsd_break_lease(ndentry->d_inode);
		if (host_err)
			goto out_drop_write;
	}
	host_err = vfs_rename(fdir, odentry, tdir, ndentry);
	if (!host_err) {
		host_err = commit_metadata(tfhp);
		if (!host_err)
			host_err = commit_metadata(ffhp);
	}
out_drop_write:
	fh_drop_write(ffhp);
=======
	if ((ndentry->d_sb->s_export_op->flags & EXPORT_OP_CLOSE_BEFORE_UNLINK) &&
	    nfsd_has_cached_files(ndentry)) {
		close_cached = true;
		goto out_dput_old;
	} else {
		struct renamedata rd = {
			.old_mnt_idmap	= &nop_mnt_idmap,
			.old_dir	= fdir,
			.old_dentry	= odentry,
			.new_mnt_idmap	= &nop_mnt_idmap,
			.new_dir	= tdir,
			.new_dentry	= ndentry,
		};
		int retries;

		for (retries = 1;;) {
			host_err = vfs_rename(&rd);
			if (host_err != -EAGAIN || !retries--)
				break;
			if (!nfsd_wait_for_delegreturn(rqstp, d_inode(odentry)))
				break;
		}
		if (!host_err) {
			host_err = commit_metadata(tfhp);
			if (!host_err)
				host_err = commit_metadata(ffhp);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 out_dput_new:
	dput(ndentry);
 out_dput_old:
	dput(odentry);
 out_nfserr:
	err = nfserrno(host_err);

<<<<<<< HEAD
	/* we cannot reply on fh_unlock on the two filehandles,
	 * as that would do the wrong thing if the two directories
	 * were the same, so again we do it by hand
	 */
	fill_post_wcc(ffhp);
	fill_post_wcc(tfhp);
	unlock_rename(tdentry, fdentry);
	ffhp->fh_locked = tfhp->fh_locked = 0;

=======
	if (!close_cached) {
		fh_fill_post_attrs(ffhp);
		fh_fill_post_attrs(tfhp);
	}
out_unlock:
	unlock_rename(tdentry, fdentry);
out_want_write:
	fh_drop_write(ffhp);

	/*
	 * If the target dentry has cached open files, then we need to
	 * try to close them prior to doing the rename.  Final fput
	 * shouldn't be done with locks held however, so we delay it
	 * until this point and then reattempt the whole shebang.
	 */
	if (close_cached) {
		close_cached = false;
		nfsd_close_cached_files(ndentry);
		dput(ndentry);
		goto retry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

/*
 * Unlink a file or directory
 * N.B. After this call fhp needs an fh_put
 */
__be32
nfsd_unlink(struct svc_rqst *rqstp, struct svc_fh *fhp, int type,
				char *fname, int flen)
{
	struct dentry	*dentry, *rdentry;
	struct inode	*dirp;
<<<<<<< HEAD
=======
	struct inode	*rinode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be32		err;
	int		host_err;

	err = nfserr_acces;
	if (!flen || isdotent(fname, flen))
		goto out;
	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_REMOVE);
	if (err)
		goto out;

<<<<<<< HEAD
	fh_lock_nested(fhp, I_MUTEX_PARENT);
	dentry = fhp->fh_dentry;
	dirp = dentry->d_inode;
=======
	host_err = fh_want_write(fhp);
	if (host_err)
		goto out_nfserr;

	dentry = fhp->fh_dentry;
	dirp = d_inode(dentry);
	inode_lock_nested(dirp, I_MUTEX_PARENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdentry = lookup_one_len(fname, dentry, flen);
	host_err = PTR_ERR(rdentry);
	if (IS_ERR(rdentry))
<<<<<<< HEAD
		goto out_nfserr;

	if (!rdentry->d_inode) {
		dput(rdentry);
		err = nfserr_noent;
		goto out;
	}

	if (!type)
		type = rdentry->d_inode->i_mode & S_IFMT;

	host_err = fh_want_write(fhp);
	if (host_err)
		goto out_put;

	host_err = nfsd_break_lease(rdentry->d_inode);
	if (host_err)
		goto out_drop_write;
	if (type != S_IFDIR)
		host_err = vfs_unlink(dirp, rdentry);
	else
		host_err = vfs_rmdir(dirp, rdentry);
	if (!host_err)
		host_err = commit_metadata(fhp);
out_drop_write:
	fh_drop_write(fhp);
out_put:
	dput(rdentry);

out_nfserr:
	err = nfserrno(host_err);
out:
	return err;
=======
		goto out_unlock;

	if (d_really_is_negative(rdentry)) {
		dput(rdentry);
		host_err = -ENOENT;
		goto out_unlock;
	}
	rinode = d_inode(rdentry);
	err = fh_fill_pre_attrs(fhp);
	if (err != nfs_ok)
		goto out_unlock;

	ihold(rinode);
	if (!type)
		type = d_inode(rdentry)->i_mode & S_IFMT;

	if (type != S_IFDIR) {
		int retries;

		if (rdentry->d_sb->s_export_op->flags & EXPORT_OP_CLOSE_BEFORE_UNLINK)
			nfsd_close_cached_files(rdentry);

		for (retries = 1;;) {
			host_err = vfs_unlink(&nop_mnt_idmap, dirp, rdentry, NULL);
			if (host_err != -EAGAIN || !retries--)
				break;
			if (!nfsd_wait_for_delegreturn(rqstp, rinode))
				break;
		}
	} else {
		host_err = vfs_rmdir(&nop_mnt_idmap, dirp, rdentry);
	}
	fh_fill_post_attrs(fhp);

	inode_unlock(dirp);
	if (!host_err)
		host_err = commit_metadata(fhp);
	dput(rdentry);
	iput(rinode);    /* truncate the inode here */

out_drop_write:
	fh_drop_write(fhp);
out_nfserr:
	if (host_err == -EBUSY) {
		/* name is mounted-on. There is no perfect
		 * error status.
		 */
		if (nfsd_v4client(rqstp))
			err = nfserr_file_open;
		else
			err = nfserr_acces;
	} else {
		err = nfserrno(host_err);
	}
out:
	return err;
out_unlock:
	inode_unlock(dirp);
	goto out_drop_write;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We do this buffering because we must not call back into the file
 * system's ->lookup() method from the filldir callback. That may well
 * deadlock a number of file systems.
 *
 * This is based heavily on the implementation of same in XFS.
 */
struct buffered_dirent {
	u64		ino;
	loff_t		offset;
	int		namlen;
	unsigned int	d_type;
	char		name[];
};

struct readdir_data {
<<<<<<< HEAD
=======
	struct dir_context ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char		*dirent;
	size_t		used;
	int		full;
};

<<<<<<< HEAD
static int nfsd_buffered_filldir(void *__buf, const char *name, int namlen,
				 loff_t offset, u64 ino, unsigned int d_type)
{
	struct readdir_data *buf = __buf;
=======
static bool nfsd_buffered_filldir(struct dir_context *ctx, const char *name,
				 int namlen, loff_t offset, u64 ino,
				 unsigned int d_type)
{
	struct readdir_data *buf =
		container_of(ctx, struct readdir_data, ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct buffered_dirent *de = (void *)(buf->dirent + buf->used);
	unsigned int reclen;

	reclen = ALIGN(sizeof(struct buffered_dirent) + namlen, sizeof(u64));
	if (buf->used + reclen > PAGE_SIZE) {
		buf->full = 1;
<<<<<<< HEAD
		return -EINVAL;
=======
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	de->namlen = namlen;
	de->offset = offset;
	de->ino = ino;
	de->d_type = d_type;
	memcpy(de->name, name, namlen);
	buf->used += reclen;

<<<<<<< HEAD
	return 0;
}

static __be32 nfsd_buffered_readdir(struct file *file, filldir_t func,
				    struct readdir_cd *cdp, loff_t *offsetp)
{
	struct readdir_data buf;
=======
	return true;
}

static __be32 nfsd_buffered_readdir(struct file *file, struct svc_fh *fhp,
				    nfsd_filldir_t func, struct readdir_cd *cdp,
				    loff_t *offsetp)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct buffered_dirent *de;
	int host_err;
	int size;
	loff_t offset;
<<<<<<< HEAD

	buf.dirent = (void *)__get_free_page(GFP_KERNEL);
=======
	struct readdir_data buf = {
		.ctx.actor = nfsd_buffered_filldir,
		.dirent = (void *)__get_free_page(GFP_KERNEL)
	};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!buf.dirent)
		return nfserrno(-ENOMEM);

	offset = *offsetp;

	while (1) {
<<<<<<< HEAD
		struct inode *dir_inode = file->f_path.dentry->d_inode;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unsigned int reclen;

		cdp->err = nfserr_eof; /* will be cleared on successful read */
		buf.used = 0;
		buf.full = 0;

<<<<<<< HEAD
		host_err = vfs_readdir(file, nfsd_buffered_filldir, &buf);
=======
		host_err = iterate_dir(file, &buf.ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (buf.full)
			host_err = 0;

		if (host_err < 0)
			break;

		size = buf.used;

		if (!size)
			break;

<<<<<<< HEAD
		/*
		 * Various filldir functions may end up calling back into
		 * lookup_one_len() and the file system's ->lookup() method.
		 * These expect i_mutex to be held, as it would within readdir.
		 */
		host_err = mutex_lock_killable(&dir_inode->i_mutex);
		if (host_err)
			break;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		de = (struct buffered_dirent *)buf.dirent;
		while (size > 0) {
			offset = de->offset;

			if (func(cdp, de->name, de->namlen, de->offset,
				 de->ino, de->d_type))
				break;

			if (cdp->err != nfs_ok)
				break;

<<<<<<< HEAD
=======
			trace_nfsd_dirent(fhp, de->ino, de->name, de->namlen);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			reclen = ALIGN(sizeof(*de) + de->namlen,
				       sizeof(u64));
			size -= reclen;
			de = (struct buffered_dirent *)((char *)de + reclen);
		}
<<<<<<< HEAD
		mutex_unlock(&dir_inode->i_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (size > 0) /* We bailed out early */
			break;

		offset = vfs_llseek(file, 0, SEEK_CUR);
	}

	free_page((unsigned long)(buf.dirent));

	if (host_err)
		return nfserrno(host_err);

	*offsetp = offset;
	return cdp->err;
}

<<<<<<< HEAD
/*
 * Read entries from a directory.
 * The  NFSv3/4 verifier we ignore for now.
 */
__be32
nfsd_readdir(struct svc_rqst *rqstp, struct svc_fh *fhp, loff_t *offsetp, 
	     struct readdir_cd *cdp, filldir_t func)
=======
/**
 * nfsd_readdir - Read entries from a directory
 * @rqstp: RPC transaction context
 * @fhp: NFS file handle of directory to be read
 * @offsetp: OUT: seek offset of final entry that was read
 * @cdp: OUT: an eof error value
 * @func: entry filler actor
 *
 * This implementation ignores the NFSv3/4 verifier cookie.
 *
 * NB: normal system calls hold file->f_pos_lock when calling
 * ->iterate_shared and ->llseek, but nfsd_readdir() does not.
 * Because the struct file acquired here is not visible to other
 * threads, it's internal state does not need mutex protection.
 *
 * Returns nfs_ok on success, otherwise an nfsstat code is
 * returned.
 */
__be32
nfsd_readdir(struct svc_rqst *rqstp, struct svc_fh *fhp, loff_t *offsetp, 
	     struct readdir_cd *cdp, nfsd_filldir_t func)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__be32		err;
	struct file	*file;
	loff_t		offset = *offsetp;
	int             may_flags = NFSD_MAY_READ;

	/* NFSv2 only supports 32 bit cookies */
	if (rqstp->rq_vers > 2)
		may_flags |= NFSD_MAY_64BIT_COOKIE;

	err = nfsd_open(rqstp, fhp, S_IFDIR, may_flags, &file);
	if (err)
		goto out;

<<<<<<< HEAD
	offset = vfs_llseek(file, offset, 0);
=======
	offset = vfs_llseek(file, offset, SEEK_SET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (offset < 0) {
		err = nfserrno((int)offset);
		goto out_close;
	}

<<<<<<< HEAD
	err = nfsd_buffered_readdir(file, func, cdp, offsetp);
=======
	err = nfsd_buffered_readdir(file, fhp, func, cdp, offsetp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (err == nfserr_eof || err == nfserr_toosmall)
		err = nfs_ok; /* can still be found in ->err */
out_close:
<<<<<<< HEAD
	nfsd_close(file);
=======
	nfsd_filp_close(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
}

<<<<<<< HEAD
=======
/**
 * nfsd_filp_close: close a file synchronously
 * @fp: the file to close
 *
 * nfsd_filp_close() is similar in behaviour to filp_close().
 * The difference is that if this is the final close on the
 * file, the that finalisation happens immediately, rather then
 * being handed over to a work_queue, as it the case for
 * filp_close().
 * When a user-space process closes a file (even when using
 * filp_close() the finalisation happens before returning to
 * userspace, so it is effectively synchronous.  When a kernel thread
 * uses file_close(), on the other hand, the handling is completely
 * asynchronous.  This means that any cost imposed by that finalisation
 * is not imposed on the nfsd thread, and nfsd could potentually
 * close files more quickly than the work queue finalises the close,
 * which would lead to unbounded growth in the queue.
 *
 * In some contexts is it not safe to synchronously wait for
 * close finalisation (see comment for __fput_sync()), but nfsd
 * does not match those contexts.  In partcilarly it does not, at the
 * time that this function is called, hold and locks and no finalisation
 * of any file, socket, or device driver would have any cause to wait
 * for nfsd to make progress.
 */
void nfsd_filp_close(struct file *fp)
{
	get_file(fp);
	filp_close(fp, NULL);
	__fput_sync(fp);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Get file system stats
 * N.B. After this call fhp needs an fh_put
 */
__be32
nfsd_statfs(struct svc_rqst *rqstp, struct svc_fh *fhp, struct kstatfs *stat, int access)
{
	__be32 err;

	err = fh_verify(rqstp, fhp, 0, NFSD_MAY_NOP | access);
	if (!err) {
		struct path path = {
			.mnt	= fhp->fh_export->ex_path.mnt,
			.dentry	= fhp->fh_dentry,
		};
		if (vfs_statfs(&path, stat))
			err = nfserr_io;
	}
	return err;
}

static int exp_rdonly(struct svc_rqst *rqstp, struct svc_export *exp)
{
	return nfsexp_flags(rqstp, exp) & NFSEXP_READONLY;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_NFSD_V4
/*
 * Helper function to translate error numbers. In the case of xattr operations,
 * some error codes need to be translated outside of the standard translations.
 *
 * ENODATA needs to be translated to nfserr_noxattr.
 * E2BIG to nfserr_xattr2big.
 *
 * Additionally, vfs_listxattr can return -ERANGE. This means that the
 * file has too many extended attributes to retrieve inside an
 * XATTR_LIST_MAX sized buffer. This is a bug in the xattr implementation:
 * filesystems will allow the adding of extended attributes until they hit
 * their own internal limit. This limit may be larger than XATTR_LIST_MAX.
 * So, at that point, the attributes are present and valid, but can't
 * be retrieved using listxattr, since the upper level xattr code enforces
 * the XATTR_LIST_MAX limit.
 *
 * This bug means that we need to deal with listxattr returning -ERANGE. The
 * best mapping is to return TOOSMALL.
 */
static __be32
nfsd_xattr_errno(int err)
{
	switch (err) {
	case -ENODATA:
		return nfserr_noxattr;
	case -E2BIG:
		return nfserr_xattr2big;
	case -ERANGE:
		return nfserr_toosmall;
	}
	return nfserrno(err);
}

/*
 * Retrieve the specified user extended attribute. To avoid always
 * having to allocate the maximum size (since we are not getting
 * a maximum size from the RPC), do a probe + alloc. Hold a reader
 * lock on i_rwsem to prevent the extended attribute from changing
 * size while we're doing this.
 */
__be32
nfsd_getxattr(struct svc_rqst *rqstp, struct svc_fh *fhp, char *name,
	      void **bufp, int *lenp)
{
	ssize_t len;
	__be32 err;
	char *buf;
	struct inode *inode;
	struct dentry *dentry;

	err = fh_verify(rqstp, fhp, 0, NFSD_MAY_READ);
	if (err)
		return err;

	err = nfs_ok;
	dentry = fhp->fh_dentry;
	inode = d_inode(dentry);

	inode_lock_shared(inode);

	len = vfs_getxattr(&nop_mnt_idmap, dentry, name, NULL, 0);

	/*
	 * Zero-length attribute, just return.
	 */
	if (len == 0) {
		*bufp = NULL;
		*lenp = 0;
		goto out;
	}

	if (len < 0) {
		err = nfsd_xattr_errno(len);
		goto out;
	}

	if (len > *lenp) {
		err = nfserr_toosmall;
		goto out;
	}

	buf = kvmalloc(len, GFP_KERNEL);
	if (buf == NULL) {
		err = nfserr_jukebox;
		goto out;
	}

	len = vfs_getxattr(&nop_mnt_idmap, dentry, name, buf, len);
	if (len <= 0) {
		kvfree(buf);
		buf = NULL;
		err = nfsd_xattr_errno(len);
	}

	*lenp = len;
	*bufp = buf;

out:
	inode_unlock_shared(inode);

	return err;
}

/*
 * Retrieve the xattr names. Since we can't know how many are
 * user extended attributes, we must get all attributes here,
 * and have the XDR encode filter out the "user." ones.
 *
 * While this could always just allocate an XATTR_LIST_MAX
 * buffer, that's a waste, so do a probe + allocate. To
 * avoid any changes between the probe and allocate, wrap
 * this in inode_lock.
 */
__be32
nfsd_listxattr(struct svc_rqst *rqstp, struct svc_fh *fhp, char **bufp,
	       int *lenp)
{
	ssize_t len;
	__be32 err;
	char *buf;
	struct inode *inode;
	struct dentry *dentry;

	err = fh_verify(rqstp, fhp, 0, NFSD_MAY_READ);
	if (err)
		return err;

	dentry = fhp->fh_dentry;
	inode = d_inode(dentry);
	*lenp = 0;

	inode_lock_shared(inode);

	len = vfs_listxattr(dentry, NULL, 0);
	if (len <= 0) {
		err = nfsd_xattr_errno(len);
		goto out;
	}

	if (len > XATTR_LIST_MAX) {
		err = nfserr_xattr2big;
		goto out;
	}

	buf = kvmalloc(len, GFP_KERNEL);
	if (buf == NULL) {
		err = nfserr_jukebox;
		goto out;
	}

	len = vfs_listxattr(dentry, buf, len);
	if (len <= 0) {
		kvfree(buf);
		err = nfsd_xattr_errno(len);
		goto out;
	}

	*lenp = len;
	*bufp = buf;

	err = nfs_ok;
out:
	inode_unlock_shared(inode);

	return err;
}

/**
 * nfsd_removexattr - Remove an extended attribute
 * @rqstp: RPC transaction being executed
 * @fhp: NFS filehandle of object with xattr to remove
 * @name: name of xattr to remove (NUL-terminate)
 *
 * Pass in a NULL pointer for delegated_inode, and let the client deal
 * with NFS4ERR_DELAY (same as with e.g. setattr and remove).
 *
 * Returns nfs_ok on success, or an nfsstat in network byte order.
 */
__be32
nfsd_removexattr(struct svc_rqst *rqstp, struct svc_fh *fhp, char *name)
{
	__be32 err;
	int ret;

	err = fh_verify(rqstp, fhp, 0, NFSD_MAY_WRITE);
	if (err)
		return err;

	ret = fh_want_write(fhp);
	if (ret)
		return nfserrno(ret);

	inode_lock(fhp->fh_dentry->d_inode);
	err = fh_fill_pre_attrs(fhp);
	if (err != nfs_ok)
		goto out_unlock;
	ret = __vfs_removexattr_locked(&nop_mnt_idmap, fhp->fh_dentry,
				       name, NULL);
	err = nfsd_xattr_errno(ret);
	fh_fill_post_attrs(fhp);
out_unlock:
	inode_unlock(fhp->fh_dentry->d_inode);
	fh_drop_write(fhp);

	return err;
}

__be32
nfsd_setxattr(struct svc_rqst *rqstp, struct svc_fh *fhp, char *name,
	      void *buf, u32 len, u32 flags)
{
	__be32 err;
	int ret;

	err = fh_verify(rqstp, fhp, 0, NFSD_MAY_WRITE);
	if (err)
		return err;

	ret = fh_want_write(fhp);
	if (ret)
		return nfserrno(ret);
	inode_lock(fhp->fh_dentry->d_inode);
	err = fh_fill_pre_attrs(fhp);
	if (err != nfs_ok)
		goto out_unlock;
	ret = __vfs_setxattr_locked(&nop_mnt_idmap, fhp->fh_dentry,
				    name, buf, len, flags, NULL);
	fh_fill_post_attrs(fhp);
	err = nfsd_xattr_errno(ret);
out_unlock:
	inode_unlock(fhp->fh_dentry->d_inode);
	fh_drop_write(fhp);
	return err;
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Check for a user's access permissions to this inode.
 */
__be32
nfsd_permission(struct svc_rqst *rqstp, struct svc_export *exp,
					struct dentry *dentry, int acc)
{
<<<<<<< HEAD
	struct inode	*inode = dentry->d_inode;
=======
	struct inode	*inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int		err;

	if ((acc & NFSD_MAY_MASK) == NFSD_MAY_NOP)
		return 0;
#if 0
	dprintk("nfsd: permission 0x%x%s%s%s%s%s%s%s mode 0%o%s%s%s\n",
		acc,
		(acc & NFSD_MAY_READ)?	" read"  : "",
		(acc & NFSD_MAY_WRITE)?	" write" : "",
		(acc & NFSD_MAY_EXEC)?	" exec"  : "",
		(acc & NFSD_MAY_SATTR)?	" sattr" : "",
		(acc & NFSD_MAY_TRUNC)?	" trunc" : "",
		(acc & NFSD_MAY_LOCK)?	" lock"  : "",
		(acc & NFSD_MAY_OWNER_OVERRIDE)? " owneroverride" : "",
		inode->i_mode,
		IS_IMMUTABLE(inode)?	" immut" : "",
		IS_APPEND(inode)?	" append" : "",
		__mnt_is_readonly(exp->ex_path.mnt)?	" ro" : "");
	dprintk("      owner %d/%d user %d/%d\n",
		inode->i_uid, inode->i_gid, current_fsuid(), current_fsgid());
#endif

	/* Normally we reject any write/sattr etc access on a read-only file
	 * system.  But if it is IRIX doing check on write-access for a 
	 * device special file, we ignore rofs.
	 */
	if (!(acc & NFSD_MAY_LOCAL_ACCESS))
		if (acc & (NFSD_MAY_WRITE | NFSD_MAY_SATTR | NFSD_MAY_TRUNC)) {
			if (exp_rdonly(rqstp, exp) ||
			    __mnt_is_readonly(exp->ex_path.mnt))
				return nfserr_rofs;
			if (/* (acc & NFSD_MAY_WRITE) && */ IS_IMMUTABLE(inode))
				return nfserr_perm;
		}
	if ((acc & NFSD_MAY_TRUNC) && IS_APPEND(inode))
		return nfserr_perm;

	if (acc & NFSD_MAY_LOCK) {
		/* If we cannot rely on authentication in NLM requests,
		 * just allow locks, otherwise require read permission, or
		 * ownership
		 */
		if (exp->ex_flags & NFSEXP_NOAUTHNLM)
			return 0;
		else
			acc = NFSD_MAY_READ | NFSD_MAY_OWNER_OVERRIDE;
	}
	/*
	 * The file owner always gets access permission for accesses that
	 * would normally be checked at open time. This is to make
	 * file access work even when the client has done a fchmod(fd, 0).
	 *
	 * However, `cp foo bar' should fail nevertheless when bar is
	 * readonly. A sensible way to do this might be to reject all
	 * attempts to truncate a read-only file, because a creat() call
	 * always implies file truncation.
	 * ... but this isn't really fair.  A process may reasonably call
	 * ftruncate on an open file descriptor on a file with perm 000.
	 * We must trust the client to do permission checking - using "ACCESS"
	 * with NFSv3.
	 */
	if ((acc & NFSD_MAY_OWNER_OVERRIDE) &&
<<<<<<< HEAD
	    inode->i_uid == current_fsuid())
		return 0;

	/* This assumes  NFSD_MAY_{READ,WRITE,EXEC} == MAY_{READ,WRITE,EXEC} */
	err = inode_permission(inode, acc & (MAY_READ|MAY_WRITE|MAY_EXEC));
=======
	    uid_eq(inode->i_uid, current_fsuid()))
		return 0;

	/* This assumes  NFSD_MAY_{READ,WRITE,EXEC} == MAY_{READ,WRITE,EXEC} */
	err = inode_permission(&nop_mnt_idmap, inode,
			       acc & (MAY_READ | MAY_WRITE | MAY_EXEC));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Allow read access to binaries even when mode 111 */
	if (err == -EACCES && S_ISREG(inode->i_mode) &&
	     (acc == (NFSD_MAY_READ | NFSD_MAY_OWNER_OVERRIDE) ||
	      acc == (NFSD_MAY_READ | NFSD_MAY_READ_IF_EXEC)))
<<<<<<< HEAD
		err = inode_permission(inode, MAY_EXEC);

	return err? nfserrno(err) : 0;
}

void
nfsd_racache_shutdown(void)
{
	struct raparms *raparm, *last_raparm;
	unsigned int i;

	dprintk("nfsd: freeing readahead buffers.\n");

	for (i = 0; i < RAPARM_HASH_SIZE; i++) {
		raparm = raparm_hash[i].pb_head;
		while(raparm) {
			last_raparm = raparm;
			raparm = raparm->p_next;
			kfree(last_raparm);
		}
		raparm_hash[i].pb_head = NULL;
	}
}
/*
 * Initialize readahead param cache
 */
int
nfsd_racache_init(int cache_size)
{
	int	i;
	int	j = 0;
	int	nperbucket;
	struct raparms **raparm = NULL;


	if (raparm_hash[0].pb_head)
		return 0;
	nperbucket = DIV_ROUND_UP(cache_size, RAPARM_HASH_SIZE);
	if (nperbucket < 2)
		nperbucket = 2;
	cache_size = nperbucket * RAPARM_HASH_SIZE;

	dprintk("nfsd: allocating %d readahead buffers.\n", cache_size);

	for (i = 0; i < RAPARM_HASH_SIZE; i++) {
		spin_lock_init(&raparm_hash[i].pb_lock);

		raparm = &raparm_hash[i].pb_head;
		for (j = 0; j < nperbucket; j++) {
			*raparm = kzalloc(sizeof(struct raparms), GFP_KERNEL);
			if (!*raparm)
				goto out_nomem;
			raparm = &(*raparm)->p_next;
		}
		*raparm = NULL;
	}

	nfsdstats.ra_size = cache_size;
	return 0;

out_nomem:
	dprintk("nfsd: kmalloc failed, freeing readahead buffers\n");
	nfsd_racache_shutdown();
	return -ENOMEM;
}

#if defined(CONFIG_NFSD_V2_ACL) || defined(CONFIG_NFSD_V3_ACL)
struct posix_acl *
nfsd_get_posix_acl(struct svc_fh *fhp, int type)
{
	struct inode *inode = fhp->fh_dentry->d_inode;
	char *name;
	void *value = NULL;
	ssize_t size;
	struct posix_acl *acl;

	if (!IS_POSIXACL(inode))
		return ERR_PTR(-EOPNOTSUPP);

	switch (type) {
	case ACL_TYPE_ACCESS:
		name = POSIX_ACL_XATTR_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = POSIX_ACL_XATTR_DEFAULT;
		break;
	default:
		return ERR_PTR(-EOPNOTSUPP);
	}

	size = nfsd_getxattr(fhp->fh_dentry, name, &value);
	if (size < 0)
		return ERR_PTR(size);

	acl = posix_acl_from_xattr(value, size);
	kfree(value);
	return acl;
}

int
nfsd_set_posix_acl(struct svc_fh *fhp, int type, struct posix_acl *acl)
{
	struct inode *inode = fhp->fh_dentry->d_inode;
	char *name;
	void *value = NULL;
	size_t size;
	int error;

	if (!IS_POSIXACL(inode) ||
	    !inode->i_op->setxattr || !inode->i_op->removexattr)
		return -EOPNOTSUPP;
	switch(type) {
		case ACL_TYPE_ACCESS:
			name = POSIX_ACL_XATTR_ACCESS;
			break;
		case ACL_TYPE_DEFAULT:
			name = POSIX_ACL_XATTR_DEFAULT;
			break;
		default:
			return -EOPNOTSUPP;
	}

	if (acl && acl->a_count) {
		size = posix_acl_xattr_size(acl->a_count);
		value = kmalloc(size, GFP_KERNEL);
		if (!value)
			return -ENOMEM;
		error = posix_acl_to_xattr(acl, value, size);
		if (error < 0)
			goto getout;
		size = error;
	} else
		size = 0;

	error = fh_want_write(fhp);
	if (error)
		goto getout;
	if (size)
		error = vfs_setxattr(fhp->fh_dentry, name, value, size, 0);
	else {
		if (!S_ISDIR(inode->i_mode) && type == ACL_TYPE_DEFAULT)
			error = 0;
		else {
			error = vfs_removexattr(fhp->fh_dentry, name);
			if (error == -ENODATA)
				error = 0;
		}
	}
	fh_drop_write(fhp);

getout:
	kfree(value);
	return error;
}
#endif  /* defined(CONFIG_NFSD_V2_ACL) || defined(CONFIG_NFSD_V3_ACL) */
=======
		err = inode_permission(&nop_mnt_idmap, inode, MAY_EXEC);

	return err? nfserrno(err) : 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
