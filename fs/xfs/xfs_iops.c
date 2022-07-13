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
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_acl.h"
#include "xfs_bit.h"
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_alloc.h"
#include "xfs_quota.h"
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_bmap.h"
#include "xfs_rtalloc.h"
#include "xfs_error.h"
#include "xfs_itable.h"
#include "xfs_rw.h"
#include "xfs_attr.h"
#include "xfs_buf_item.h"
#include "xfs_utils.h"
#include "xfs_vnodeops.h"
#include "xfs_inode_item.h"
#include "xfs_trace.h"

#include <linux/capability.h>
#include <linux/xattr.h>
#include <linux/namei.h>
#include <linux/posix_acl.h>
#include <linux/security.h>
#include <linux/fiemap.h>
#include <linux/slab.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_acl.h"
#include "xfs_quota.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_attr.h"
#include "xfs_trans.h"
#include "xfs_trace.h"
#include "xfs_icache.h"
#include "xfs_symlink.h"
#include "xfs_dir2.h"
#include "xfs_iomap.h"
#include "xfs_error.h"
#include "xfs_ioctl.h"
#include "xfs_xattr.h"

#include <linux/posix_acl.h>
#include <linux/security.h>
#include <linux/iversion.h>
#include <linux/fiemap.h>

/*
 * Directories have different lock order w.r.t. mmap_lock compared to regular
 * files. This is due to readdir potentially triggering page faults on a user
 * buffer inside filldir(), and this happens with the ilock on the directory
 * held. For regular files, the lock order is the other way around - the
 * mmap_lock is taken during the page fault, and then we lock the ilock to do
 * block mapping. Hence we need a different class for the directory ilock so
 * that lockdep can tell them apart.
 */
static struct lock_class_key xfs_nondir_ilock_class;
static struct lock_class_key xfs_dir_ilock_class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int
xfs_initxattrs(
	struct inode		*inode,
	const struct xattr	*xattr_array,
	void			*fs_info)
{
	const struct xattr	*xattr;
	struct xfs_inode	*ip = XFS_I(inode);
	int			error = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
<<<<<<< HEAD
		error = xfs_attr_set(ip, xattr->name, xattr->value,
				     xattr->value_len, ATTR_SECURE);
=======
		struct xfs_da_args	args = {
			.dp		= ip,
			.attr_filter	= XFS_ATTR_SECURE,
			.name		= xattr->name,
			.namelen	= strlen(xattr->name),
			.value		= xattr->value,
			.valuelen	= xattr->value_len,
		};
		error = xfs_attr_change(&args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error < 0)
			break;
	}
	return error;
}

/*
 * Hook in SELinux.  This is not quite correct yet, what we really need
 * here (as we do for default ACLs) is a mechanism by which creation of
 * these attrs can be journalled at inode creation time (along with the
 * inode, of course, such that log replay can't cause these to be lost).
 */
<<<<<<< HEAD

STATIC int
xfs_init_security(
=======
int
xfs_inode_init_security(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode	*inode,
	struct inode	*dir,
	const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
<<<<<<< HEAD
					    &xfs_initxattrs, NULL);
=======
					     &xfs_initxattrs, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
xfs_dentry_to_name(
	struct xfs_name	*namep,
	struct dentry	*dentry)
{
	namep->name = dentry->d_name.name;
	namep->len = dentry->d_name.len;
<<<<<<< HEAD
=======
	namep->type = XFS_DIR3_FT_UNKNOWN;
}

static int
xfs_dentry_mode_to_name(
	struct xfs_name	*namep,
	struct dentry	*dentry,
	int		mode)
{
	namep->name = dentry->d_name.name;
	namep->len = dentry->d_name.len;
	namep->type = xfs_mode_to_ftype(mode);

	if (unlikely(namep->type == XFS_DIR3_FT_UNKNOWN))
		return -EFSCORRUPTED;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC void
xfs_cleanup_inode(
	struct inode	*dir,
	struct inode	*inode,
	struct dentry	*dentry)
{
	struct xfs_name	teardown;

	/* Oh, the horror.
	 * If we can't add the ACL or we fail in
<<<<<<< HEAD
	 * xfs_init_security we must back out.
=======
	 * xfs_inode_init_security we must back out.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * ENOSPC can hit here, among other things.
	 */
	xfs_dentry_to_name(&teardown, dentry);

	xfs_remove(XFS_I(dir), &teardown, XFS_I(inode));
<<<<<<< HEAD
	iput(inode);
}

STATIC int
xfs_vn_mknod(
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode,
	dev_t		rdev)
{
	struct inode	*inode;
	struct xfs_inode *ip = NULL;
	struct posix_acl *default_acl = NULL;
=======
}

/*
 * Check to see if we are likely to need an extended attribute to be added to
 * the inode we are about to allocate. This allows the attribute fork to be
 * created during the inode allocation, reducing the number of transactions we
 * need to do in this fast path.
 *
 * The security checks are optimistic, but not guaranteed. The two LSMs that
 * require xattrs to be added here (selinux and smack) are also the only two
 * LSMs that add a sb->s_security structure to the superblock. Hence if security
 * is enabled and sb->s_security is set, we have a pretty good idea that we are
 * going to be asked to add a security xattr immediately after allocating the
 * xfs inode and instantiating the VFS inode.
 */
static inline bool
xfs_create_need_xattr(
	struct inode	*dir,
	struct posix_acl *default_acl,
	struct posix_acl *acl)
{
	if (acl)
		return true;
	if (default_acl)
		return true;
#if IS_ENABLED(CONFIG_SECURITY)
	if (dir->i_sb->s_security)
		return true;
#endif
	return false;
}


STATIC int
xfs_generic_create(
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct dentry		*dentry,
	umode_t			mode,
	dev_t			rdev,
	struct file		*tmpfile)	/* unnamed file */
{
	struct inode	*inode;
	struct xfs_inode *ip = NULL;
	struct posix_acl *default_acl, *acl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xfs_name	name;
	int		error;

	/*
	 * Irix uses Missed'em'V split, but doesn't want to see
	 * the upper 5 bits of (14bit) major.
	 */
	if (S_ISCHR(mode) || S_ISBLK(mode)) {
		if (unlikely(!sysv_valid_dev(rdev) || MAJOR(rdev) & ~0x1ff))
			return -EINVAL;
<<<<<<< HEAD
		rdev = sysv_encode_dev(rdev);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		rdev = 0;
	}

<<<<<<< HEAD
	if (IS_POSIXACL(dir)) {
		default_acl = xfs_get_acl(dir, ACL_TYPE_DEFAULT);
		if (IS_ERR(default_acl))
			return PTR_ERR(default_acl);

		if (!default_acl)
			mode &= ~current_umask();
	}

	xfs_dentry_to_name(&name, dentry);
	error = xfs_create(XFS_I(dir), &name, mode, rdev, &ip);
=======
	error = posix_acl_create(dir, &mode, &default_acl, &acl);
	if (error)
		return error;

	/* Verify mode is valid also for tmpfile case */
	error = xfs_dentry_mode_to_name(&name, dentry, mode);
	if (unlikely(error))
		goto out_free_acl;

	if (!tmpfile) {
		error = xfs_create(idmap, XFS_I(dir), &name, mode, rdev,
				xfs_create_need_xattr(dir, default_acl, acl),
				&ip);
	} else {
		error = xfs_create_tmpfile(idmap, XFS_I(dir), mode, &ip);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(error))
		goto out_free_acl;

	inode = VFS_I(ip);

<<<<<<< HEAD
	error = xfs_init_security(inode, dir, &dentry->d_name);
=======
	error = xfs_inode_init_security(inode, dir, &dentry->d_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(error))
		goto out_cleanup_inode;

	if (default_acl) {
<<<<<<< HEAD
		error = -xfs_inherit_acl(inode, default_acl);
		default_acl = NULL;
		if (unlikely(error))
			goto out_cleanup_inode;
	}


	d_instantiate(dentry, inode);
	return -error;

 out_cleanup_inode:
	xfs_cleanup_inode(dir, inode, dentry);
 out_free_acl:
	posix_acl_release(default_acl);
	return -error;
=======
		error = __xfs_set_acl(inode, default_acl, ACL_TYPE_DEFAULT);
		if (error)
			goto out_cleanup_inode;
	}
	if (acl) {
		error = __xfs_set_acl(inode, acl, ACL_TYPE_ACCESS);
		if (error)
			goto out_cleanup_inode;
	}

	xfs_setup_iops(ip);

	if (tmpfile) {
		/*
		 * The VFS requires that any inode fed to d_tmpfile must have
		 * nlink == 1 so that it can decrement the nlink in d_tmpfile.
		 * However, we created the temp file with nlink == 0 because
		 * we're not allowed to put an inode with nlink > 0 on the
		 * unlinked list.  Therefore we have to set nlink to 1 so that
		 * d_tmpfile can immediately set it back to zero.
		 */
		set_nlink(inode, 1);
		d_tmpfile(tmpfile, inode);
	} else
		d_instantiate(dentry, inode);

	xfs_finish_inode_setup(ip);

 out_free_acl:
	posix_acl_release(default_acl);
	posix_acl_release(acl);
	return error;

 out_cleanup_inode:
	xfs_finish_inode_setup(ip);
	if (!tmpfile)
		xfs_cleanup_inode(dir, inode, dentry);
	xfs_irele(ip);
	goto out_free_acl;
}

STATIC int
xfs_vn_mknod(
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct dentry		*dentry,
	umode_t			mode,
	dev_t			rdev)
{
	return xfs_generic_create(idmap, dir, dentry, mode, rdev, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_vn_create(
<<<<<<< HEAD
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode,
	bool		flags)
{
	return xfs_vn_mknod(dir, dentry, mode, 0);
=======
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct dentry		*dentry,
	umode_t			mode,
	bool			flags)
{
	return xfs_generic_create(idmap, dir, dentry, mode, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_vn_mkdir(
<<<<<<< HEAD
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode)
{
	return xfs_vn_mknod(dir, dentry, mode|S_IFDIR, 0);
=======
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct dentry		*dentry,
	umode_t			mode)
{
	return xfs_generic_create(idmap, dir, dentry, mode | S_IFDIR, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC struct dentry *
xfs_vn_lookup(
	struct inode	*dir,
	struct dentry	*dentry,
	unsigned int flags)
{
<<<<<<< HEAD
=======
	struct inode *inode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct xfs_inode *cip;
	struct xfs_name	name;
	int		error;

	if (dentry->d_name.len >= MAXNAMELEN)
		return ERR_PTR(-ENAMETOOLONG);

	xfs_dentry_to_name(&name, dentry);
	error = xfs_lookup(XFS_I(dir), &name, &cip, NULL);
<<<<<<< HEAD
	if (unlikely(error)) {
		if (unlikely(error != ENOENT))
			return ERR_PTR(-error);
		d_add(dentry, NULL);
		return NULL;
	}

	return d_splice_alias(VFS_I(cip), dentry);
=======
	if (likely(!error))
		inode = VFS_I(cip);
	else if (likely(error == -ENOENT))
		inode = NULL;
	else
		inode = ERR_PTR(error);
	return d_splice_alias(inode, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC struct dentry *
xfs_vn_ci_lookup(
	struct inode	*dir,
	struct dentry	*dentry,
	unsigned int flags)
{
	struct xfs_inode *ip;
	struct xfs_name	xname;
	struct xfs_name ci_name;
	struct qstr	dname;
	int		error;

	if (dentry->d_name.len >= MAXNAMELEN)
		return ERR_PTR(-ENAMETOOLONG);

	xfs_dentry_to_name(&xname, dentry);
	error = xfs_lookup(XFS_I(dir), &xname, &ip, &ci_name);
	if (unlikely(error)) {
<<<<<<< HEAD
		if (unlikely(error != ENOENT))
			return ERR_PTR(-error);
=======
		if (unlikely(error != -ENOENT))
			return ERR_PTR(error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * call d_add(dentry, NULL) here when d_drop_negative_children
		 * is called in xfs_vn_mknod (ie. allow negative dentries
		 * with CI filesystems).
		 */
		return NULL;
	}

	/* if exact match, just splice and exit */
	if (!ci_name.name)
		return d_splice_alias(VFS_I(ip), dentry);

	/* else case-insensitive match... */
	dname.name = ci_name.name;
	dname.len = ci_name.len;
	dentry = d_add_ci(dentry, VFS_I(ip), &dname);
<<<<<<< HEAD
	kmem_free(ci_name.name);
=======
	kfree(ci_name.name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return dentry;
}

STATIC int
xfs_vn_link(
	struct dentry	*old_dentry,
	struct inode	*dir,
	struct dentry	*dentry)
{
<<<<<<< HEAD
	struct inode	*inode = old_dentry->d_inode;
	struct xfs_name	name;
	int		error;

	xfs_dentry_to_name(&name, dentry);

	error = xfs_link(XFS_I(dir), XFS_I(inode), &name);
	if (unlikely(error))
		return -error;
=======
	struct inode	*inode = d_inode(old_dentry);
	struct xfs_name	name;
	int		error;

	error = xfs_dentry_mode_to_name(&name, dentry, inode->i_mode);
	if (unlikely(error))
		return error;

	error = xfs_link(XFS_I(dir), XFS_I(inode), &name);
	if (unlikely(error))
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ihold(inode);
	d_instantiate(dentry, inode);
	return 0;
}

STATIC int
xfs_vn_unlink(
	struct inode	*dir,
	struct dentry	*dentry)
{
	struct xfs_name	name;
	int		error;

	xfs_dentry_to_name(&name, dentry);

<<<<<<< HEAD
	error = -xfs_remove(XFS_I(dir), &name, XFS_I(dentry->d_inode));
=======
	error = xfs_remove(XFS_I(dir), &name, XFS_I(d_inode(dentry)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;

	/*
	 * With unlink, the VFS makes the dentry "negative": no inode,
	 * but still hashed. This is incompatible with case-insensitive
	 * mode, so invalidate (unhash) the dentry in CI-mode.
	 */
<<<<<<< HEAD
	if (xfs_sb_version_hasasciici(&XFS_M(dir->i_sb)->m_sb))
=======
	if (xfs_has_asciici(XFS_M(dir->i_sb)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		d_invalidate(dentry);
	return 0;
}

STATIC int
xfs_vn_symlink(
<<<<<<< HEAD
	struct inode	*dir,
	struct dentry	*dentry,
	const char	*symname)
=======
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct dentry		*dentry,
	const char		*symname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode	*inode;
	struct xfs_inode *cip = NULL;
	struct xfs_name	name;
	int		error;
	umode_t		mode;

	mode = S_IFLNK |
		(irix_symlink_mode ? 0777 & ~current_umask() : S_IRWXUGO);
<<<<<<< HEAD
	xfs_dentry_to_name(&name, dentry);

	error = xfs_symlink(XFS_I(dir), &name, symname, mode, &cip);
=======
	error = xfs_dentry_mode_to_name(&name, dentry, mode);
	if (unlikely(error))
		goto out;

	error = xfs_symlink(idmap, XFS_I(dir), &name, symname, mode, &cip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(error))
		goto out;

	inode = VFS_I(cip);

<<<<<<< HEAD
	error = xfs_init_security(inode, dir, &dentry->d_name);
	if (unlikely(error))
		goto out_cleanup_inode;

	d_instantiate(dentry, inode);
	return 0;

 out_cleanup_inode:
	xfs_cleanup_inode(dir, inode, dentry);
 out:
	return -error;
=======
	error = xfs_inode_init_security(inode, dir, &dentry->d_name);
	if (unlikely(error))
		goto out_cleanup_inode;

	xfs_setup_iops(cip);

	d_instantiate(dentry, inode);
	xfs_finish_inode_setup(cip);
	return 0;

 out_cleanup_inode:
	xfs_finish_inode_setup(cip);
	xfs_cleanup_inode(dir, inode, dentry);
	xfs_irele(cip);
 out:
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_vn_rename(
<<<<<<< HEAD
	struct inode	*odir,
	struct dentry	*odentry,
	struct inode	*ndir,
	struct dentry	*ndentry)
{
	struct inode	*new_inode = ndentry->d_inode;
	struct xfs_name	oname;
	struct xfs_name	nname;

	xfs_dentry_to_name(&oname, odentry);
	xfs_dentry_to_name(&nname, ndentry);

	return -xfs_rename(XFS_I(odir), &oname, XFS_I(odentry->d_inode),
			   XFS_I(ndir), &nname, new_inode ?
			   			XFS_I(new_inode) : NULL);
=======
	struct mnt_idmap	*idmap,
	struct inode		*odir,
	struct dentry		*odentry,
	struct inode		*ndir,
	struct dentry		*ndentry,
	unsigned int		flags)
{
	struct inode	*new_inode = d_inode(ndentry);
	int		omode = 0;
	int		error;
	struct xfs_name	oname;
	struct xfs_name	nname;

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE | RENAME_WHITEOUT))
		return -EINVAL;

	/* if we are exchanging files, we need to set i_mode of both files */
	if (flags & RENAME_EXCHANGE)
		omode = d_inode(ndentry)->i_mode;

	error = xfs_dentry_mode_to_name(&oname, odentry, omode);
	if (omode && unlikely(error))
		return error;

	error = xfs_dentry_mode_to_name(&nname, ndentry,
					d_inode(odentry)->i_mode);
	if (unlikely(error))
		return error;

	return xfs_rename(idmap, XFS_I(odir), &oname,
			  XFS_I(d_inode(odentry)), XFS_I(ndir), &nname,
			  new_inode ? XFS_I(new_inode) : NULL, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * careful here - this function can get called recursively, so
 * we need to be very careful about how much stack we use.
 * uio is kmalloced for this reason...
 */
<<<<<<< HEAD
STATIC void *
xfs_vn_follow_link(
	struct dentry		*dentry,
	struct nameidata	*nd)
=======
STATIC const char *
xfs_vn_get_link(
	struct dentry		*dentry,
	struct inode		*inode,
	struct delayed_call	*done)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char			*link;
	int			error = -ENOMEM;

<<<<<<< HEAD
	link = kmalloc(MAXPATHLEN+1, GFP_KERNEL);
	if (!link)
		goto out_err;

	error = -xfs_readlink(XFS_I(dentry->d_inode), link);
	if (unlikely(error))
		goto out_kfree;

	nd_set_link(nd, link);
	return NULL;
=======
	if (!dentry)
		return ERR_PTR(-ECHILD);

	link = kmalloc(XFS_SYMLINK_MAXLEN+1, GFP_KERNEL);
	if (!link)
		goto out_err;

	error = xfs_readlink(XFS_I(d_inode(dentry)), link);
	if (unlikely(error))
		goto out_kfree;

	set_delayed_call(done, kfree_link, link);
	return link;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 out_kfree:
	kfree(link);
 out_err:
<<<<<<< HEAD
	nd_set_link(nd, ERR_PTR(error));
	return NULL;
}

STATIC void
xfs_vn_put_link(
	struct dentry	*dentry,
	struct nameidata *nd,
	void		*p)
{
	char		*s = nd_get_link(nd);

	if (!IS_ERR(s))
		kfree(s);
=======
	return ERR_PTR(error);
}

static uint32_t
xfs_stat_blksize(
	struct xfs_inode	*ip)
{
	struct xfs_mount	*mp = ip->i_mount;

	/*
	 * If the file blocks are being allocated from a realtime volume, then
	 * always return the realtime extent size.
	 */
	if (XFS_IS_REALTIME_INODE(ip))
		return XFS_FSB_TO_B(mp, xfs_get_extsz_hint(ip));

	/*
	 * Allow large block sizes to be reported to userspace programs if the
	 * "largeio" mount option is used.
	 *
	 * If compatibility mode is specified, simply return the basic unit of
	 * caching so that we don't get inefficient read/modify/write I/O from
	 * user apps. Otherwise....
	 *
	 * If the underlying volume is a stripe, then return the stripe width in
	 * bytes as the recommended I/O size. It is not a stripe and we've set a
	 * default buffered I/O size, return that, otherwise return the compat
	 * default.
	 */
	if (xfs_has_large_iosize(mp)) {
		if (mp->m_swidth)
			return XFS_FSB_TO_B(mp, mp->m_swidth);
		if (xfs_has_allocsize(mp))
			return 1U << mp->m_allocsize_log;
	}

	return PAGE_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_vn_getattr(
<<<<<<< HEAD
	struct vfsmount		*mnt,
	struct dentry		*dentry,
	struct kstat		*stat)
{
	struct inode		*inode = dentry->d_inode;
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;

	trace_xfs_getattr(ip);

	if (XFS_FORCED_SHUTDOWN(mp))
		return -XFS_ERROR(EIO);

	stat->size = XFS_ISIZE(ip);
	stat->dev = inode->i_sb->s_dev;
	stat->mode = ip->i_d.di_mode;
	stat->nlink = ip->i_d.di_nlink;
	stat->uid = ip->i_d.di_uid;
	stat->gid = ip->i_d.di_gid;
	stat->ino = ip->i_ino;
	stat->atime = inode->i_atime;
	stat->mtime = inode->i_mtime;
	stat->ctime = inode->i_ctime;
	stat->blocks =
		XFS_FSB_TO_BB(mp, ip->i_d.di_nblocks + ip->i_delayed_blks);

=======
	struct mnt_idmap	*idmap,
	const struct path	*path,
	struct kstat		*stat,
	u32			request_mask,
	unsigned int		query_flags)
{
	struct inode		*inode = d_inode(path->dentry);
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	vfsuid_t		vfsuid = i_uid_into_vfsuid(idmap, inode);
	vfsgid_t		vfsgid = i_gid_into_vfsgid(idmap, inode);

	trace_xfs_getattr(ip);

	if (xfs_is_shutdown(mp))
		return -EIO;

	stat->size = XFS_ISIZE(ip);
	stat->dev = inode->i_sb->s_dev;
	stat->mode = inode->i_mode;
	stat->nlink = inode->i_nlink;
	stat->uid = vfsuid_into_kuid(vfsuid);
	stat->gid = vfsgid_into_kgid(vfsgid);
	stat->ino = ip->i_ino;
	stat->atime = inode_get_atime(inode);
	stat->mtime = inode_get_mtime(inode);
	stat->ctime = inode_get_ctime(inode);
	stat->blocks = XFS_FSB_TO_BB(mp, ip->i_nblocks + ip->i_delayed_blks);

	if (xfs_has_v3inodes(mp)) {
		if (request_mask & STATX_BTIME) {
			stat->result_mask |= STATX_BTIME;
			stat->btime = ip->i_crtime;
		}
	}

	if ((request_mask & STATX_CHANGE_COOKIE) && IS_I_VERSION(inode)) {
		stat->change_cookie = inode_query_iversion(inode);
		stat->result_mask |= STATX_CHANGE_COOKIE;
	}

	/*
	 * Note: If you add another clause to set an attribute flag, please
	 * update attributes_mask below.
	 */
	if (ip->i_diflags & XFS_DIFLAG_IMMUTABLE)
		stat->attributes |= STATX_ATTR_IMMUTABLE;
	if (ip->i_diflags & XFS_DIFLAG_APPEND)
		stat->attributes |= STATX_ATTR_APPEND;
	if (ip->i_diflags & XFS_DIFLAG_NODUMP)
		stat->attributes |= STATX_ATTR_NODUMP;

	stat->attributes_mask |= (STATX_ATTR_IMMUTABLE |
				  STATX_ATTR_APPEND |
				  STATX_ATTR_NODUMP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (inode->i_mode & S_IFMT) {
	case S_IFBLK:
	case S_IFCHR:
		stat->blksize = BLKDEV_IOSIZE;
<<<<<<< HEAD
		stat->rdev = MKDEV(sysv_major(ip->i_df.if_u2.if_rdev) & 0x1ff,
				   sysv_minor(ip->i_df.if_u2.if_rdev));
		break;
	default:
		if (XFS_IS_REALTIME_INODE(ip)) {
			/*
			 * If the file blocks are being allocated from a
			 * realtime volume, then return the inode's realtime
			 * extent size or the realtime volume's extent size.
			 */
			stat->blksize =
				xfs_get_extsz_hint(ip) << mp->m_sb.sb_blocklog;
		} else
			stat->blksize = xfs_preferred_iosize(mp);
=======
		stat->rdev = inode->i_rdev;
		break;
	case S_IFREG:
		if (request_mask & STATX_DIOALIGN) {
			struct xfs_buftarg	*target = xfs_inode_buftarg(ip);
			struct block_device	*bdev = target->bt_bdev;

			stat->result_mask |= STATX_DIOALIGN;
			stat->dio_mem_align = bdev_dma_alignment(bdev) + 1;
			stat->dio_offset_align = bdev_logical_block_size(bdev);
		}
		fallthrough;
	default:
		stat->blksize = xfs_stat_blksize(ip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		stat->rdev = 0;
		break;
	}

	return 0;
}

<<<<<<< HEAD
static void
xfs_setattr_mode(
	struct xfs_trans	*tp,
	struct xfs_inode	*ip,
	struct iattr		*iattr)
{
	struct inode	*inode = VFS_I(ip);
	umode_t		mode = iattr->ia_mode;

	ASSERT(tp);
	ASSERT(xfs_isilocked(ip, XFS_ILOCK_EXCL));

	if (!in_group_p(inode->i_gid) && !capable(CAP_FSETID))
		mode &= ~S_ISGID;

	ip->i_d.di_mode &= S_IFMT;
	ip->i_d.di_mode |= mode & ~S_IFMT;

	inode->i_mode &= S_IFMT;
	inode->i_mode |= mode & ~S_IFMT;
}

int
xfs_setattr_nonsize(
	struct xfs_inode	*ip,
	struct iattr		*iattr,
	int			flags)
=======
static int
xfs_vn_change_ok(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct iattr		*iattr)
{
	struct xfs_mount	*mp = XFS_I(d_inode(dentry))->i_mount;

	if (xfs_is_readonly(mp))
		return -EROFS;

	if (xfs_is_shutdown(mp))
		return -EIO;

	return setattr_prepare(idmap, dentry, iattr);
}

/*
 * Set non-size attributes of an inode.
 *
 * Caution: The caller of this function is responsible for calling
 * setattr_prepare() or otherwise verifying the change is fine.
 */
static int
xfs_setattr_nonsize(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct xfs_inode	*ip,
	struct iattr		*iattr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	xfs_mount_t		*mp = ip->i_mount;
	struct inode		*inode = VFS_I(ip);
	int			mask = iattr->ia_valid;
	xfs_trans_t		*tp;
	int			error;
<<<<<<< HEAD
	uid_t			uid = 0, iuid = 0;
	gid_t			gid = 0, igid = 0;
	struct xfs_dquot	*udqp = NULL, *gdqp = NULL;
	struct xfs_dquot	*olddquot1 = NULL, *olddquot2 = NULL;

	trace_xfs_setattr(ip);

	if (mp->m_flags & XFS_MOUNT_RDONLY)
		return XFS_ERROR(EROFS);

	if (XFS_FORCED_SHUTDOWN(mp))
		return XFS_ERROR(EIO);

	error = -inode_change_ok(inode, iattr);
	if (error)
		return XFS_ERROR(error);
=======
	kuid_t			uid = GLOBAL_ROOT_UID;
	kgid_t			gid = GLOBAL_ROOT_GID;
	struct xfs_dquot	*udqp = NULL, *gdqp = NULL;
	struct xfs_dquot	*old_udqp = NULL, *old_gdqp = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT((mask & ATTR_SIZE) == 0);

	/*
	 * If disk quotas is on, we make sure that the dquots do exist on disk,
	 * before we start any other transactions. Trying to do this later
	 * is messy. We don't care to take a readlock to look at the ids
	 * in inode here, because we can't hold it across the trans_reserve.
	 * If the IDs do change before we take the ilock, we're covered
	 * because the i_*dquot fields will get updated anyway.
	 */
	if (XFS_IS_QUOTA_ON(mp) && (mask & (ATTR_UID|ATTR_GID))) {
		uint	qflags = 0;

		if ((mask & ATTR_UID) && XFS_IS_UQUOTA_ON(mp)) {
<<<<<<< HEAD
			uid = iattr->ia_uid;
			qflags |= XFS_QMOPT_UQUOTA;
		} else {
			uid = ip->i_d.di_uid;
		}
		if ((mask & ATTR_GID) && XFS_IS_GQUOTA_ON(mp)) {
			gid = iattr->ia_gid;
			qflags |= XFS_QMOPT_GQUOTA;
		}  else {
			gid = ip->i_d.di_gid;
=======
			uid = from_vfsuid(idmap, i_user_ns(inode),
					  iattr->ia_vfsuid);
			qflags |= XFS_QMOPT_UQUOTA;
		} else {
			uid = inode->i_uid;
		}
		if ((mask & ATTR_GID) && XFS_IS_GQUOTA_ON(mp)) {
			gid = from_vfsgid(idmap, i_user_ns(inode),
					  iattr->ia_vfsgid);
			qflags |= XFS_QMOPT_GQUOTA;
		}  else {
			gid = inode->i_gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/*
		 * We take a reference when we initialize udqp and gdqp,
		 * so it is important that we never blindly double trip on
		 * the same variable. See xfs_create() for an example.
		 */
		ASSERT(udqp == NULL);
		ASSERT(gdqp == NULL);
<<<<<<< HEAD
		error = xfs_qm_vop_dqalloc(ip, uid, gid, xfs_get_projid(ip),
					 qflags, &udqp, &gdqp);
=======
		error = xfs_qm_vop_dqalloc(ip, uid, gid, ip->i_projid,
					   qflags, &udqp, &gdqp, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (error)
			return error;
	}

<<<<<<< HEAD
	tp = xfs_trans_alloc(mp, XFS_TRANS_SETATTR_NOT_SIZE);
	error = xfs_trans_reserve(tp, 0, XFS_ICHANGE_LOG_RES(mp), 0, 0, 0);
	if (error)
		goto out_dqrele;

	xfs_ilock(ip, XFS_ILOCK_EXCL);

	/*
	 * Change file ownership.  Must be the owner or privileged.
	 */
	if (mask & (ATTR_UID|ATTR_GID)) {
		/*
		 * These IDs could have changed since we last looked at them.
		 * But, we're assured that if the ownership did change
		 * while we didn't have the inode locked, inode's dquot(s)
		 * would have changed also.
		 */
		iuid = ip->i_d.di_uid;
		igid = ip->i_d.di_gid;
		gid = (mask & ATTR_GID) ? iattr->ia_gid : igid;
		uid = (mask & ATTR_UID) ? iattr->ia_uid : iuid;

		/*
		 * Do a quota reservation only if uid/gid is actually
		 * going to change.
		 */
		if (XFS_IS_QUOTA_RUNNING(mp) &&
		    ((XFS_IS_UQUOTA_ON(mp) && iuid != uid) ||
		     (XFS_IS_GQUOTA_ON(mp) && igid != gid))) {
			ASSERT(tp);
			error = xfs_qm_vop_chown_reserve(tp, ip, udqp, gdqp,
						capable(CAP_FOWNER) ?
						XFS_QMOPT_FORCE_RES : 0);
			if (error)	/* out of quota */
				goto out_trans_cancel;
		}
	}

	xfs_trans_ijoin(tp, ip, 0);

	/*
	 * Change file ownership.  Must be the owner or privileged.
	 */
	if (mask & (ATTR_UID|ATTR_GID)) {
		/*
		 * CAP_FSETID overrides the following restrictions:
		 *
		 * The set-user-ID and set-group-ID bits of a file will be
		 * cleared upon successful return from chown()
		 */
		if ((ip->i_d.di_mode & (S_ISUID|S_ISGID)) &&
		    !capable(CAP_FSETID))
			ip->i_d.di_mode &= ~(S_ISUID|S_ISGID);

		/*
		 * Change the ownerships and register quota modifications
		 * in the transaction.
		 */
		if (iuid != uid) {
			if (XFS_IS_QUOTA_RUNNING(mp) && XFS_IS_UQUOTA_ON(mp)) {
				ASSERT(mask & ATTR_UID);
				ASSERT(udqp);
				olddquot1 = xfs_qm_vop_chown(tp, ip,
							&ip->i_udquot, udqp);
			}
			ip->i_d.di_uid = uid;
			inode->i_uid = uid;
		}
		if (igid != gid) {
			if (XFS_IS_QUOTA_RUNNING(mp) && XFS_IS_GQUOTA_ON(mp)) {
				ASSERT(!XFS_IS_PQUOTA_ON(mp));
				ASSERT(mask & ATTR_GID);
				ASSERT(gdqp);
				olddquot2 = xfs_qm_vop_chown(tp, ip,
							&ip->i_gdquot, gdqp);
			}
			ip->i_d.di_gid = gid;
			inode->i_gid = gid;
		}
	}

	/*
	 * Change file access modes.
	 */
	if (mask & ATTR_MODE)
		xfs_setattr_mode(tp, ip, iattr);

	/*
	 * Change file access or modified times.
	 */
	if (mask & ATTR_ATIME) {
		inode->i_atime = iattr->ia_atime;
		ip->i_d.di_atime.t_sec = iattr->ia_atime.tv_sec;
		ip->i_d.di_atime.t_nsec = iattr->ia_atime.tv_nsec;
	}
	if (mask & ATTR_CTIME) {
		inode->i_ctime = iattr->ia_ctime;
		ip->i_d.di_ctime.t_sec = iattr->ia_ctime.tv_sec;
		ip->i_d.di_ctime.t_nsec = iattr->ia_ctime.tv_nsec;
	}
	if (mask & ATTR_MTIME) {
		inode->i_mtime = iattr->ia_mtime;
		ip->i_d.di_mtime.t_sec = iattr->ia_mtime.tv_sec;
		ip->i_d.di_mtime.t_nsec = iattr->ia_mtime.tv_nsec;
	}

	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	XFS_STATS_INC(xs_ig_attrchg);

	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(tp);
	error = xfs_trans_commit(tp, 0);

	xfs_iunlock(ip, XFS_ILOCK_EXCL);
=======
	error = xfs_trans_alloc_ichange(ip, udqp, gdqp, NULL,
			has_capability_noaudit(current, CAP_FOWNER), &tp);
	if (error)
		goto out_dqrele;

	/*
	 * Register quota modifications in the transaction.  Must be the owner
	 * or privileged.  These IDs could have changed since we last looked at
	 * them.  But, we're assured that if the ownership did change while we
	 * didn't have the inode locked, inode's dquot(s) would have changed
	 * also.
	 */
	if (XFS_IS_UQUOTA_ON(mp) &&
	    i_uid_needs_update(idmap, iattr, inode)) {
		ASSERT(udqp);
		old_udqp = xfs_qm_vop_chown(tp, ip, &ip->i_udquot, udqp);
	}
	if (XFS_IS_GQUOTA_ON(mp) &&
	    i_gid_needs_update(idmap, iattr, inode)) {
		ASSERT(xfs_has_pquotino(mp) || !XFS_IS_PQUOTA_ON(mp));
		ASSERT(gdqp);
		old_gdqp = xfs_qm_vop_chown(tp, ip, &ip->i_gdquot, gdqp);
	}

	setattr_copy(idmap, inode, iattr);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	XFS_STATS_INC(mp, xs_ig_attrchg);

	if (xfs_has_wsync(mp))
		xfs_trans_set_sync(tp);
	error = xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Release any dquot(s) the inode had kept before chown.
	 */
<<<<<<< HEAD
	xfs_qm_dqrele(olddquot1);
	xfs_qm_dqrele(olddquot2);
=======
	xfs_qm_dqrele(old_udqp);
	xfs_qm_dqrele(old_gdqp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_qm_dqrele(udqp);
	xfs_qm_dqrele(gdqp);

	if (error)
<<<<<<< HEAD
		return XFS_ERROR(error);
=======
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * XXX(hch): Updating the ACL entries is not atomic vs the i_mode
	 * 	     update.  We could avoid this with linked transactions
	 * 	     and passing down the transaction pointer all the way
	 *	     to attr_set.  No previous user of the generic
	 * 	     Posix ACL code seems to care about this issue either.
	 */
<<<<<<< HEAD
	if ((mask & ATTR_MODE) && !(flags & XFS_ATTR_NOACL)) {
		error = -xfs_acl_chmod(inode);
		if (error)
			return XFS_ERROR(error);
=======
	if (mask & ATTR_MODE) {
		error = posix_acl_chmod(idmap, dentry, inode->i_mode);
		if (error)
			return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;

<<<<<<< HEAD
out_trans_cancel:
	xfs_trans_cancel(tp, 0);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_dqrele:
	xfs_qm_dqrele(udqp);
	xfs_qm_dqrele(gdqp);
	return error;
}

/*
 * Truncate file.  Must have write permission and not be a directory.
<<<<<<< HEAD
 */
int
xfs_setattr_size(
	struct xfs_inode	*ip,
	struct iattr		*iattr,
	int			flags)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct inode		*inode = VFS_I(ip);
	int			mask = iattr->ia_valid;
	xfs_off_t		oldsize, newsize;
	struct xfs_trans	*tp;
	int			error;
	uint			lock_flags;
	uint			commit_flags = 0;

	trace_xfs_setattr(ip);

	if (mp->m_flags & XFS_MOUNT_RDONLY)
		return XFS_ERROR(EROFS);

	if (XFS_FORCED_SHUTDOWN(mp))
		return XFS_ERROR(EIO);

	error = -inode_change_ok(inode, iattr);
	if (error)
		return XFS_ERROR(error);

	ASSERT(S_ISREG(ip->i_d.di_mode));
	ASSERT((mask & (ATTR_UID|ATTR_GID|ATTR_ATIME|ATTR_ATIME_SET|
			ATTR_MTIME_SET|ATTR_KILL_PRIV|ATTR_TIMES_SET)) == 0);

	lock_flags = XFS_ILOCK_EXCL;
	if (!(flags & XFS_ATTR_NOLOCK))
		lock_flags |= XFS_IOLOCK_EXCL;
	xfs_ilock(ip, lock_flags);
=======
 *
 * Caution: The caller of this function is responsible for calling
 * setattr_prepare() or otherwise verifying the change is fine.
 */
STATIC int
xfs_setattr_size(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct xfs_inode	*ip,
	struct iattr		*iattr)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct inode		*inode = VFS_I(ip);
	xfs_off_t		oldsize, newsize;
	struct xfs_trans	*tp;
	int			error;
	uint			lock_flags = 0;
	bool			did_zeroing = false;

	xfs_assert_ilocked(ip, XFS_IOLOCK_EXCL | XFS_MMAPLOCK_EXCL);
	ASSERT(S_ISREG(inode->i_mode));
	ASSERT((iattr->ia_valid & (ATTR_UID|ATTR_GID|ATTR_ATIME|ATTR_ATIME_SET|
		ATTR_MTIME_SET|ATTR_TIMES_SET)) == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	oldsize = inode->i_size;
	newsize = iattr->ia_size;

	/*
	 * Short circuit the truncate case for zero length files.
	 */
<<<<<<< HEAD
	if (newsize == 0 && oldsize == 0 && ip->i_d.di_nextents == 0) {
		if (!(mask & (ATTR_CTIME|ATTR_MTIME)))
			goto out_unlock;
=======
	if (newsize == 0 && oldsize == 0 && ip->i_df.if_nextents == 0) {
		if (!(iattr->ia_valid & (ATTR_CTIME|ATTR_MTIME)))
			return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Use the regular setattr path to update the timestamps.
		 */
<<<<<<< HEAD
		xfs_iunlock(ip, lock_flags);
		iattr->ia_valid &= ~ATTR_SIZE;
		return xfs_setattr_nonsize(ip, iattr, 0);
=======
		iattr->ia_valid &= ~ATTR_SIZE;
		return xfs_setattr_nonsize(idmap, dentry, ip, iattr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Make sure that the dquots are attached to the inode.
	 */
<<<<<<< HEAD
	error = xfs_qm_dqattach_locked(ip, 0);
	if (error)
		goto out_unlock;

	/*
	 * Now we can make the changes.  Before we join the inode to the
	 * transaction, take care of the part of the truncation that must be
	 * done without the inode lock.  This needs to be done before joining
	 * the inode to the transaction, because the inode cannot be unlocked
	 * once it is a part of the transaction.
	 */
	if (newsize > oldsize) {
		/*
		 * Do the first part of growing a file: zero any data in the
		 * last block that is beyond the old EOF.  We need to do this
		 * before the inode is joined to the transaction to modify
		 * i_size.
		 */
		error = xfs_zero_eof(ip, newsize, oldsize);
		if (error)
			goto out_unlock;
	}
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	lock_flags &= ~XFS_ILOCK_EXCL;

	/*
	 * We are going to log the inode size change in this transaction so
	 * any previous writes that are beyond the on disk EOF and the new
	 * EOF that have not been written out need to be written here.  If we
	 * do not write the data out, we expose ourselves to the null files
	 * problem.
	 *
	 * Only flush from the on disk size to the smaller of the in memory
	 * file size or the new size as that's the range we really care about
	 * here and prevents waiting for other data not within the range we
	 * care about here.
	 */
	if (oldsize != ip->i_d.di_size && newsize > ip->i_d.di_size) {
		error = xfs_flush_pages(ip, ip->i_d.di_size, newsize, 0,
					FI_NONE);
		if (error)
			goto out_unlock;
	}
=======
	error = xfs_qm_dqattach(ip);
	if (error)
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Wait for all direct I/O to complete.
	 */
	inode_dio_wait(inode);

<<<<<<< HEAD
	error = -block_truncate_page(inode->i_mapping, newsize, xfs_get_blocks);
	if (error)
		goto out_unlock;

	tp = xfs_trans_alloc(mp, XFS_TRANS_SETATTR_SIZE);
	error = xfs_trans_reserve(tp, 0, XFS_ITRUNCATE_LOG_RES(mp), 0,
				 XFS_TRANS_PERM_LOG_RES,
				 XFS_ITRUNCATE_LOG_COUNT);
	if (error)
		goto out_trans_cancel;

	truncate_setsize(inode, newsize);

	commit_flags = XFS_TRANS_RELEASE_LOG_RES;
	lock_flags |= XFS_ILOCK_EXCL;

	xfs_ilock(ip, XFS_ILOCK_EXCL);

=======
	/*
	 * File data changes must be complete before we start the transaction to
	 * modify the inode.  This needs to be done before joining the inode to
	 * the transaction because the inode cannot be unlocked once it is a
	 * part of the transaction.
	 *
	 * Start with zeroing any data beyond EOF that we may expose on file
	 * extension, or zeroing out the rest of the block on a downward
	 * truncate.
	 */
	if (newsize > oldsize) {
		trace_xfs_zero_eof(ip, oldsize, newsize - oldsize);
		error = xfs_zero_range(ip, oldsize, newsize - oldsize,
				&did_zeroing);
	} else {
		/*
		 * iomap won't detect a dirty page over an unwritten block (or a
		 * cow block over a hole) and subsequently skips zeroing the
		 * newly post-EOF portion of the page. Flush the new EOF to
		 * convert the block before the pagecache truncate.
		 */
		error = filemap_write_and_wait_range(inode->i_mapping, newsize,
						     newsize);
		if (error)
			return error;
		error = xfs_truncate_page(ip, newsize, &did_zeroing);
	}

	if (error)
		return error;

	/*
	 * We've already locked out new page faults, so now we can safely remove
	 * pages from the page cache knowing they won't get refaulted until we
	 * drop the XFS_MMAP_EXCL lock after the extent manipulations are
	 * complete. The truncate_setsize() call also cleans partial EOF page
	 * PTEs on extending truncates and hence ensures sub-page block size
	 * filesystems are correctly handled, too.
	 *
	 * We have to do all the page cache truncate work outside the
	 * transaction context as the "lock" order is page lock->log space
	 * reservation as defined by extent allocation in the writeback path.
	 * Hence a truncate can fail with ENOMEM from xfs_trans_alloc(), but
	 * having already truncated the in-memory version of the file (i.e. made
	 * user visible changes). There's not much we can do about this, except
	 * to hope that the caller sees ENOMEM and retries the truncate
	 * operation.
	 *
	 * And we update in-core i_size and truncate page cache beyond newsize
	 * before writeback the [i_disk_size, newsize] range, so we're
	 * guaranteed not to write stale data past the new EOF on truncate down.
	 */
	truncate_setsize(inode, newsize);

	/*
	 * We are going to log the inode size change in this transaction so
	 * any previous writes that are beyond the on disk EOF and the new
	 * EOF that have not been written out need to be written here.  If we
	 * do not write the data out, we expose ourselves to the null files
	 * problem. Note that this includes any block zeroing we did above;
	 * otherwise those blocks may not be zeroed after a crash.
	 */
	if (did_zeroing ||
	    (newsize > ip->i_disk_size && oldsize != ip->i_disk_size)) {
		error = filemap_write_and_wait_range(VFS_I(ip)->i_mapping,
						ip->i_disk_size, newsize - 1);
		if (error)
			return error;
	}

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_itruncate, 0, 0, 0, &tp);
	if (error)
		return error;

	lock_flags |= XFS_ILOCK_EXCL;
	xfs_ilock(ip, XFS_ILOCK_EXCL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_trans_ijoin(tp, ip, 0);

	/*
	 * Only change the c/mtime if we are changing the size or we are
	 * explicitly asked to change it.  This handles the semantic difference
	 * between truncate() and ftruncate() as implemented in the VFS.
	 *
	 * The regular truncate() case without ATTR_CTIME and ATTR_MTIME is a
	 * special case where we need to update the times despite not having
	 * these flags set.  For all other operations the VFS set these flags
	 * explicitly if it wants a timestamp update.
	 */
<<<<<<< HEAD
	if (newsize != oldsize && (!(mask & (ATTR_CTIME | ATTR_MTIME)))) {
		iattr->ia_ctime = iattr->ia_mtime =
			current_fs_time(inode->i_sb);
		mask |= ATTR_CTIME | ATTR_MTIME;
=======
	if (newsize != oldsize &&
	    !(iattr->ia_valid & (ATTR_CTIME | ATTR_MTIME))) {
		iattr->ia_ctime = iattr->ia_mtime =
			current_time(inode);
		iattr->ia_valid |= ATTR_CTIME | ATTR_MTIME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * The first thing we do is set the size to new_size permanently on
	 * disk.  This way we don't have to worry about anyone ever being able
	 * to look at the data being freed even in the face of a crash.
	 * What we're getting around here is the case where we free a block, it
	 * is allocated to another file, it is written to, and then we crash.
	 * If the new data gets written to the file but the log buffers
	 * containing the free and reallocation don't, then we'd end up with
	 * garbage in the blocks being freed.  As long as we make the new size
	 * permanent before actually freeing any blocks it doesn't matter if
	 * they get written to.
	 */
<<<<<<< HEAD
	ip->i_d.di_size = newsize;
=======
	ip->i_disk_size = newsize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	if (newsize <= oldsize) {
		error = xfs_itruncate_extents(&tp, ip, XFS_DATA_FORK, newsize);
		if (error)
<<<<<<< HEAD
			goto out_trans_abort;
=======
			goto out_trans_cancel;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Truncated "down", so we're removing references to old data
		 * here - if we delay flushing for a long time, we expose
		 * ourselves unduly to the notorious NULL files problem.  So,
		 * we mark this inode and flush it when the file is closed,
		 * and do not wait the usual (long) time for writeout.
		 */
		xfs_iflags_set(ip, XFS_ITRUNCATED);
<<<<<<< HEAD
	}

	/*
	 * Change file access modes.
	 */
	if (mask & ATTR_MODE)
		xfs_setattr_mode(tp, ip, iattr);

	if (mask & ATTR_CTIME) {
		inode->i_ctime = iattr->ia_ctime;
		ip->i_d.di_ctime.t_sec = iattr->ia_ctime.tv_sec;
		ip->i_d.di_ctime.t_nsec = iattr->ia_ctime.tv_nsec;
	}
	if (mask & ATTR_MTIME) {
		inode->i_mtime = iattr->ia_mtime;
		ip->i_d.di_mtime.t_sec = iattr->ia_mtime.tv_sec;
		ip->i_d.di_mtime.t_nsec = iattr->ia_mtime.tv_nsec;
	}

	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	XFS_STATS_INC(xs_ig_attrchg);

	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(tp);

	error = xfs_trans_commit(tp, XFS_TRANS_RELEASE_LOG_RES);
=======

		/* A truncate down always removes post-EOF blocks. */
		xfs_inode_clear_eofblocks_tag(ip);
	}

	ASSERT(!(iattr->ia_valid & (ATTR_UID | ATTR_GID)));
	setattr_copy(idmap, inode, iattr);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	XFS_STATS_INC(mp, xs_ig_attrchg);

	if (xfs_has_wsync(mp))
		xfs_trans_set_sync(tp);

	error = xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_unlock:
	if (lock_flags)
		xfs_iunlock(ip, lock_flags);
	return error;

<<<<<<< HEAD
out_trans_abort:
	commit_flags |= XFS_TRANS_ABORT;
out_trans_cancel:
	xfs_trans_cancel(tp, commit_flags);
	goto out_unlock;
}

STATIC int
xfs_vn_setattr(
	struct dentry	*dentry,
	struct iattr	*iattr)
{
	if (iattr->ia_valid & ATTR_SIZE)
		return -xfs_setattr_size(XFS_I(dentry->d_inode), iattr, 0);
	return -xfs_setattr_nonsize(XFS_I(dentry->d_inode), iattr, 0);
}

#define XFS_FIEMAP_FLAGS	(FIEMAP_FLAG_SYNC|FIEMAP_FLAG_XATTR)

/*
 * Call fiemap helper to fill in user data.
 * Returns positive errors to xfs_getbmap.
 */
STATIC int
xfs_fiemap_format(
	void			**arg,
	struct getbmapx		*bmv,
	int			*full)
{
	int			error;
	struct fiemap_extent_info *fieinfo = *arg;
	u32			fiemap_flags = 0;
	u64			logical, physical, length;

	/* Do nothing for a hole */
	if (bmv->bmv_block == -1LL)
		return 0;

	logical = BBTOB(bmv->bmv_offset);
	physical = BBTOB(bmv->bmv_block);
	length = BBTOB(bmv->bmv_length);

	if (bmv->bmv_oflags & BMV_OF_PREALLOC)
		fiemap_flags |= FIEMAP_EXTENT_UNWRITTEN;
	else if (bmv->bmv_oflags & BMV_OF_DELALLOC) {
		fiemap_flags |= FIEMAP_EXTENT_DELALLOC;
		physical = 0;   /* no block yet */
	}
	if (bmv->bmv_oflags & BMV_OF_LAST)
		fiemap_flags |= FIEMAP_EXTENT_LAST;

	error = fiemap_fill_next_extent(fieinfo, logical, physical,
					length, fiemap_flags);
	if (error > 0) {
		error = 0;
		*full = 1;	/* user array now full */
	}

	return -error;
=======
out_trans_cancel:
	xfs_trans_cancel(tp);
	goto out_unlock;
}

int
xfs_vn_setattr_size(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct iattr		*iattr)
{
	struct xfs_inode	*ip = XFS_I(d_inode(dentry));
	int error;

	trace_xfs_setattr(ip);

	error = xfs_vn_change_ok(idmap, dentry, iattr);
	if (error)
		return error;
	return xfs_setattr_size(idmap, dentry, ip, iattr);
}

STATIC int
xfs_vn_setattr(
	struct mnt_idmap	*idmap,
	struct dentry		*dentry,
	struct iattr		*iattr)
{
	struct inode		*inode = d_inode(dentry);
	struct xfs_inode	*ip = XFS_I(inode);
	int			error;

	if (iattr->ia_valid & ATTR_SIZE) {
		uint			iolock;

		xfs_ilock(ip, XFS_MMAPLOCK_EXCL);
		iolock = XFS_IOLOCK_EXCL | XFS_MMAPLOCK_EXCL;

		error = xfs_break_layouts(inode, &iolock, BREAK_UNMAP);
		if (error) {
			xfs_iunlock(ip, XFS_MMAPLOCK_EXCL);
			return error;
		}

		error = xfs_vn_setattr_size(idmap, dentry, iattr);
		xfs_iunlock(ip, XFS_MMAPLOCK_EXCL);
	} else {
		trace_xfs_setattr(ip);

		error = xfs_vn_change_ok(idmap, dentry, iattr);
		if (!error)
			error = xfs_setattr_nonsize(idmap, dentry, ip, iattr);
	}

	return error;
}

STATIC int
xfs_vn_update_time(
	struct inode		*inode,
	int			flags)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	int			log_flags = XFS_ILOG_TIMESTAMP;
	struct xfs_trans	*tp;
	int			error;
	struct timespec64	now;

	trace_xfs_update_time(ip);

	if (inode->i_sb->s_flags & SB_LAZYTIME) {
		if (!((flags & S_VERSION) &&
		      inode_maybe_inc_iversion(inode, false))) {
			generic_update_time(inode, flags);
			return 0;
		}

		/* Capture the iversion update that just occurred */
		log_flags |= XFS_ILOG_CORE;
	}

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_fsyncts, 0, 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	if (flags & (S_CTIME|S_MTIME))
		now = inode_set_ctime_current(inode);
	else
		now = current_time(inode);

	if (flags & S_MTIME)
		inode_set_mtime_to_ts(inode, now);
	if (flags & S_ATIME)
		inode_set_atime_to_ts(inode, now);

	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	xfs_trans_log_inode(tp, ip, log_flags);
	return xfs_trans_commit(tp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

STATIC int
xfs_vn_fiemap(
	struct inode		*inode,
	struct fiemap_extent_info *fieinfo,
	u64			start,
	u64			length)
{
<<<<<<< HEAD
	xfs_inode_t		*ip = XFS_I(inode);
	struct getbmapx		bm;
	int			error;

	error = fiemap_check_flags(fieinfo, XFS_FIEMAP_FLAGS);
	if (error)
		return error;

	/* Set up bmap header for xfs internal routine */
	bm.bmv_offset = BTOBB(start);
	/* Special case for whole file */
	if (length == FIEMAP_MAX_OFFSET)
		bm.bmv_length = -1LL;
	else
		bm.bmv_length = BTOBB(length);

	/* We add one because in getbmap world count includes the header */
	bm.bmv_count = !fieinfo->fi_extents_max ? MAXEXTNUM :
					fieinfo->fi_extents_max + 1;
	bm.bmv_count = min_t(__s32, bm.bmv_count,
			     (PAGE_SIZE * 16 / sizeof(struct getbmapx)));
	bm.bmv_iflags = BMV_IF_PREALLOC | BMV_IF_NO_HOLES;
	if (fieinfo->fi_flags & FIEMAP_FLAG_XATTR)
		bm.bmv_iflags |= BMV_IF_ATTRFORK;
	if (!(fieinfo->fi_flags & FIEMAP_FLAG_SYNC))
		bm.bmv_iflags |= BMV_IF_DELALLOC;

	error = xfs_getbmap(ip, &bm, xfs_fiemap_format, fieinfo);
	if (error)
		return -error;

	return 0;
}

static const struct inode_operations xfs_inode_operations = {
	.get_acl		= xfs_get_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.setxattr		= generic_setxattr,
	.getxattr		= generic_getxattr,
	.removexattr		= generic_removexattr,
	.listxattr		= xfs_vn_listxattr,
	.fiemap			= xfs_vn_fiemap,
=======
	int			error;

	xfs_ilock(XFS_I(inode), XFS_IOLOCK_SHARED);
	if (fieinfo->fi_flags & FIEMAP_FLAG_XATTR) {
		fieinfo->fi_flags &= ~FIEMAP_FLAG_XATTR;
		error = iomap_fiemap(inode, fieinfo, start, length,
				&xfs_xattr_iomap_ops);
	} else {
		error = iomap_fiemap(inode, fieinfo, start, length,
				&xfs_read_iomap_ops);
	}
	xfs_iunlock(XFS_I(inode), XFS_IOLOCK_SHARED);

	return error;
}

STATIC int
xfs_vn_tmpfile(
	struct mnt_idmap	*idmap,
	struct inode		*dir,
	struct file		*file,
	umode_t			mode)
{
	int err = xfs_generic_create(idmap, dir, file->f_path.dentry, mode, 0, file);

	return finish_open_simple(file, err);
}

static const struct inode_operations xfs_inode_operations = {
	.get_inode_acl		= xfs_get_acl,
	.set_acl		= xfs_set_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.listxattr		= xfs_vn_listxattr,
	.fiemap			= xfs_vn_fiemap,
	.update_time		= xfs_vn_update_time,
	.fileattr_get		= xfs_fileattr_get,
	.fileattr_set		= xfs_fileattr_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct inode_operations xfs_dir_inode_operations = {
	.create			= xfs_vn_create,
	.lookup			= xfs_vn_lookup,
	.link			= xfs_vn_link,
	.unlink			= xfs_vn_unlink,
	.symlink		= xfs_vn_symlink,
	.mkdir			= xfs_vn_mkdir,
	/*
	 * Yes, XFS uses the same method for rmdir and unlink.
	 *
	 * There are some subtile differences deeper in the code,
	 * but we use S_ISDIR to check for those.
	 */
	.rmdir			= xfs_vn_unlink,
	.mknod			= xfs_vn_mknod,
	.rename			= xfs_vn_rename,
<<<<<<< HEAD
	.get_acl		= xfs_get_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.setxattr		= generic_setxattr,
	.getxattr		= generic_getxattr,
	.removexattr		= generic_removexattr,
	.listxattr		= xfs_vn_listxattr,
=======
	.get_inode_acl		= xfs_get_acl,
	.set_acl		= xfs_set_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.listxattr		= xfs_vn_listxattr,
	.update_time		= xfs_vn_update_time,
	.tmpfile		= xfs_vn_tmpfile,
	.fileattr_get		= xfs_fileattr_get,
	.fileattr_set		= xfs_fileattr_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct inode_operations xfs_dir_ci_inode_operations = {
	.create			= xfs_vn_create,
	.lookup			= xfs_vn_ci_lookup,
	.link			= xfs_vn_link,
	.unlink			= xfs_vn_unlink,
	.symlink		= xfs_vn_symlink,
	.mkdir			= xfs_vn_mkdir,
	/*
	 * Yes, XFS uses the same method for rmdir and unlink.
	 *
	 * There are some subtile differences deeper in the code,
	 * but we use S_ISDIR to check for those.
	 */
	.rmdir			= xfs_vn_unlink,
	.mknod			= xfs_vn_mknod,
	.rename			= xfs_vn_rename,
<<<<<<< HEAD
	.get_acl		= xfs_get_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.setxattr		= generic_setxattr,
	.getxattr		= generic_getxattr,
	.removexattr		= generic_removexattr,
	.listxattr		= xfs_vn_listxattr,
};

static const struct inode_operations xfs_symlink_inode_operations = {
	.readlink		= generic_readlink,
	.follow_link		= xfs_vn_follow_link,
	.put_link		= xfs_vn_put_link,
	.get_acl		= xfs_get_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.setxattr		= generic_setxattr,
	.getxattr		= generic_getxattr,
	.removexattr		= generic_removexattr,
	.listxattr		= xfs_vn_listxattr,
};

STATIC void
xfs_diflags_to_iflags(
	struct inode		*inode,
	struct xfs_inode	*ip)
{
	if (ip->i_d.di_flags & XFS_DIFLAG_IMMUTABLE)
		inode->i_flags |= S_IMMUTABLE;
	else
		inode->i_flags &= ~S_IMMUTABLE;
	if (ip->i_d.di_flags & XFS_DIFLAG_APPEND)
		inode->i_flags |= S_APPEND;
	else
		inode->i_flags &= ~S_APPEND;
	if (ip->i_d.di_flags & XFS_DIFLAG_SYNC)
		inode->i_flags |= S_SYNC;
	else
		inode->i_flags &= ~S_SYNC;
	if (ip->i_d.di_flags & XFS_DIFLAG_NOATIME)
		inode->i_flags |= S_NOATIME;
	else
		inode->i_flags &= ~S_NOATIME;
}

/*
 * Initialize the Linux inode, set up the operation vectors and
 * unlock the inode.
 *
 * When reading existing inodes from disk this is called directly
 * from xfs_iget, when creating a new inode it is called from
 * xfs_ialloc after setting up the inode.
 *
 * We are always called with an uninitialised linux inode here.
 * We need to initialise the necessary fields and take a reference
 * on it.
=======
	.get_inode_acl		= xfs_get_acl,
	.set_acl		= xfs_set_acl,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.listxattr		= xfs_vn_listxattr,
	.update_time		= xfs_vn_update_time,
	.tmpfile		= xfs_vn_tmpfile,
	.fileattr_get		= xfs_fileattr_get,
	.fileattr_set		= xfs_fileattr_set,
};

static const struct inode_operations xfs_symlink_inode_operations = {
	.get_link		= xfs_vn_get_link,
	.getattr		= xfs_vn_getattr,
	.setattr		= xfs_vn_setattr,
	.listxattr		= xfs_vn_listxattr,
	.update_time		= xfs_vn_update_time,
};

/* Figure out if this file actually supports DAX. */
static bool
xfs_inode_supports_dax(
	struct xfs_inode	*ip)
{
	struct xfs_mount	*mp = ip->i_mount;

	/* Only supported on regular files. */
	if (!S_ISREG(VFS_I(ip)->i_mode))
		return false;

	/* Block size must match page size */
	if (mp->m_sb.sb_blocksize != PAGE_SIZE)
		return false;

	/* Device has to support DAX too. */
	return xfs_inode_buftarg(ip)->bt_daxdev != NULL;
}

static bool
xfs_inode_should_enable_dax(
	struct xfs_inode *ip)
{
	if (!IS_ENABLED(CONFIG_FS_DAX))
		return false;
	if (xfs_has_dax_never(ip->i_mount))
		return false;
	if (!xfs_inode_supports_dax(ip))
		return false;
	if (xfs_has_dax_always(ip->i_mount))
		return true;
	if (ip->i_diflags2 & XFS_DIFLAG2_DAX)
		return true;
	return false;
}

void
xfs_diflags_to_iflags(
	struct xfs_inode	*ip,
	bool init)
{
	struct inode            *inode = VFS_I(ip);
	unsigned int            xflags = xfs_ip2xflags(ip);
	unsigned int            flags = 0;

	ASSERT(!(IS_DAX(inode) && init));

	if (xflags & FS_XFLAG_IMMUTABLE)
		flags |= S_IMMUTABLE;
	if (xflags & FS_XFLAG_APPEND)
		flags |= S_APPEND;
	if (xflags & FS_XFLAG_SYNC)
		flags |= S_SYNC;
	if (xflags & FS_XFLAG_NOATIME)
		flags |= S_NOATIME;
	if (init && xfs_inode_should_enable_dax(ip))
		flags |= S_DAX;

	/*
	 * S_DAX can only be set during inode initialization and is never set by
	 * the VFS, so we cannot mask off S_DAX in i_flags.
	 */
	inode->i_flags &= ~(S_IMMUTABLE | S_APPEND | S_SYNC | S_NOATIME);
	inode->i_flags |= flags;
}

/*
 * Initialize the Linux inode.
 *
 * When reading existing inodes from disk this is called directly from xfs_iget,
 * when creating a new inode it is called from xfs_init_new_inode after setting
 * up the inode. These callers have different criteria for clearing XFS_INEW, so
 * leave it up to the caller to deal with unlocking the inode appropriately.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void
xfs_setup_inode(
	struct xfs_inode	*ip)
{
	struct inode		*inode = &ip->i_vnode;
<<<<<<< HEAD

	inode->i_ino = ip->i_ino;
	inode->i_state = I_NEW;

	inode_sb_list_add(inode);
	/* make the inode look hashed for the writeback code */
	hlist_add_fake(&inode->i_hash);

	inode->i_mode	= ip->i_d.di_mode;
	set_nlink(inode, ip->i_d.di_nlink);
	inode->i_uid	= ip->i_d.di_uid;
	inode->i_gid	= ip->i_d.di_gid;

	switch (inode->i_mode & S_IFMT) {
	case S_IFBLK:
	case S_IFCHR:
		inode->i_rdev =
			MKDEV(sysv_major(ip->i_df.if_u2.if_rdev) & 0x1ff,
			      sysv_minor(ip->i_df.if_u2.if_rdev));
		break;
	default:
		inode->i_rdev = 0;
		break;
	}

	inode->i_generation = ip->i_d.di_gen;
	i_size_write(inode, ip->i_d.di_size);
	inode->i_atime.tv_sec	= ip->i_d.di_atime.t_sec;
	inode->i_atime.tv_nsec	= ip->i_d.di_atime.t_nsec;
	inode->i_mtime.tv_sec	= ip->i_d.di_mtime.t_sec;
	inode->i_mtime.tv_nsec	= ip->i_d.di_mtime.t_nsec;
	inode->i_ctime.tv_sec	= ip->i_d.di_ctime.t_sec;
	inode->i_ctime.tv_nsec	= ip->i_d.di_ctime.t_nsec;
	xfs_diflags_to_iflags(inode, ip);
=======
	gfp_t			gfp_mask;

	inode->i_ino = ip->i_ino;
	inode->i_state |= I_NEW;

	inode_sb_list_add(inode);
	/* make the inode look hashed for the writeback code */
	inode_fake_hash(inode);

	i_size_write(inode, ip->i_disk_size);
	xfs_diflags_to_iflags(ip, true);

	if (S_ISDIR(inode->i_mode)) {
		/*
		 * We set the i_rwsem class here to avoid potential races with
		 * lockdep_annotate_inode_mutex_key() reinitialising the lock
		 * after a filehandle lookup has already found the inode in
		 * cache before it has been unlocked via unlock_new_inode().
		 */
		lockdep_set_class(&inode->i_rwsem,
				  &inode->i_sb->s_type->i_mutex_dir_key);
		lockdep_set_class(&ip->i_lock, &xfs_dir_ilock_class);
	} else {
		lockdep_set_class(&ip->i_lock, &xfs_nondir_ilock_class);
	}

	/*
	 * Ensure all page cache allocations are done from GFP_NOFS context to
	 * prevent direct reclaim recursion back into the filesystem and blowing
	 * stacks or deadlocking.
	 */
	gfp_mask = mapping_gfp_mask(inode->i_mapping);
	mapping_set_gfp_mask(inode->i_mapping, (gfp_mask & ~(__GFP_FS)));

	/*
	 * For real-time inodes update the stable write flags to that of the RT
	 * device instead of the data device.
	 */
	if (S_ISREG(inode->i_mode) && XFS_IS_REALTIME_INODE(ip))
		xfs_update_stable_writes(ip);

	/*
	 * If there is no attribute fork no ACL can exist on this inode,
	 * and it can't have any file capabilities attached to it either.
	 */
	if (!xfs_inode_has_attr_fork(ip)) {
		inode_has_no_xattr(inode);
		cache_no_acl(inode);
	}
}

void
xfs_setup_iops(
	struct xfs_inode	*ip)
{
	struct inode		*inode = &ip->i_vnode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (inode->i_mode & S_IFMT) {
	case S_IFREG:
		inode->i_op = &xfs_inode_operations;
		inode->i_fop = &xfs_file_operations;
<<<<<<< HEAD
		inode->i_mapping->a_ops = &xfs_address_space_operations;
		break;
	case S_IFDIR:
		if (xfs_sb_version_hasasciici(&XFS_M(inode->i_sb)->m_sb))
=======
		if (IS_DAX(inode))
			inode->i_mapping->a_ops = &xfs_dax_aops;
		else
			inode->i_mapping->a_ops = &xfs_address_space_operations;
		break;
	case S_IFDIR:
		if (xfs_has_asciici(XFS_M(inode->i_sb)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			inode->i_op = &xfs_dir_ci_inode_operations;
		else
			inode->i_op = &xfs_dir_inode_operations;
		inode->i_fop = &xfs_dir_file_operations;
		break;
	case S_IFLNK:
		inode->i_op = &xfs_symlink_inode_operations;
<<<<<<< HEAD
		if (!(ip->i_df.if_flags & XFS_IFINLINE))
			inode->i_mapping->a_ops = &xfs_address_space_operations;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		inode->i_op = &xfs_inode_operations;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
		break;
	}
<<<<<<< HEAD

	/*
	 * If there is no attribute fork no ACL can exist on this inode,
	 * and it can't have any file capabilities attached to it either.
	 */
	if (!XFS_IFORK_Q(ip)) {
		inode_has_no_xattr(inode);
		cache_no_acl(inode);
	}

	xfs_iflags_clear(ip, XFS_INEW);
	barrier();

	unlock_new_inode(inode);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
