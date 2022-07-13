<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
  File: fs/xattr.c

  Extended attribute handling.

  Copyright (C) 2001 by Andreas Gruenbacher <a.gruenbacher@computer.org>
  Copyright (C) 2001 SGI - Silicon Graphics, Inc <linux-xfs@oss.sgi.com>
  Copyright (c) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
 */
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/filelock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/xattr.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/evm.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/syscalls.h>
#include <linux/export.h>
#include <linux/fsnotify.h>
#include <linux/audit.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD

#include <asm/uaccess.h>
=======
#include <linux/posix_acl_xattr.h>

#include <linux/uaccess.h>

#include "internal.h"

static const char *
strcmp_prefix(const char *a, const char *a_prefix)
{
	while (*a_prefix && *a == *a_prefix) {
		a++;
		a_prefix++;
	}
	return *a_prefix ? NULL : a;
}

/*
 * In order to implement different sets of xattr operations for each xattr
 * prefix, a filesystem should create a null-terminated array of struct
 * xattr_handler (one for each prefix) and hang a pointer to it off of the
 * s_xattr field of the superblock.
 */
#define for_each_xattr_handler(handlers, handler)		\
	if (handlers)						\
		for ((handler) = *(handlers)++;			\
			(handler) != NULL;			\
			(handler) = *(handlers)++)

/*
 * Find the xattr_handler with the matching prefix.
 */
static const struct xattr_handler *
xattr_resolve_name(struct inode *inode, const char **name)
{
	const struct xattr_handler * const *handlers = inode->i_sb->s_xattr;
	const struct xattr_handler *handler;

	if (!(inode->i_opflags & IOP_XATTR)) {
		if (unlikely(is_bad_inode(inode)))
			return ERR_PTR(-EIO);
		return ERR_PTR(-EOPNOTSUPP);
	}
	for_each_xattr_handler(handlers, handler) {
		const char *n;

		n = strcmp_prefix(*name, xattr_prefix(handler));
		if (n) {
			if (!handler->prefix ^ !*n) {
				if (*n)
					continue;
				return ERR_PTR(-EINVAL);
			}
			*name = n;
			return handler;
		}
	}
	return ERR_PTR(-EOPNOTSUPP);
}

/**
 * may_write_xattr - check whether inode allows writing xattr
 * @idmap: idmap of the mount the inode was found from
 * @inode: the inode on which to set an xattr
 *
 * Check whether the inode allows writing xattrs. Specifically, we can never
 * set or remove an extended attribute on a read-only filesystem  or on an
 * immutable / append-only inode.
 *
 * We also need to ensure that the inode has a mapping in the mount to
 * not risk writing back invalid i_{g,u}id values.
 *
 * Return: On success zero is returned. On error a negative errno is returned.
 */
int may_write_xattr(struct mnt_idmap *idmap, struct inode *inode)
{
	if (IS_IMMUTABLE(inode))
		return -EPERM;
	if (IS_APPEND(inode))
		return -EPERM;
	if (HAS_UNMAPPED_ID(idmap, inode))
		return -EPERM;
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Check permissions for extended attribute access.  This is a bit complicated
 * because different namespaces have very different rules.
 */
static int
<<<<<<< HEAD
xattr_permission(struct inode *inode, const char *name, int mask)
{
	/*
	 * We can never set or remove an extended attribute on a read-only
	 * filesystem  or on an immutable / append-only inode.
	 */
	if (mask & MAY_WRITE) {
		if (IS_IMMUTABLE(inode) || IS_APPEND(inode))
			return -EPERM;
=======
xattr_permission(struct mnt_idmap *idmap, struct inode *inode,
		 const char *name, int mask)
{
	if (mask & MAY_WRITE) {
		int ret;

		ret = may_write_xattr(idmap, inode);
		if (ret)
			return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * No restriction for security.* and system.* from the VFS.  Decision
	 * on these is left to the underlying filesystem / security module.
	 */
	if (!strncmp(name, XATTR_SECURITY_PREFIX, XATTR_SECURITY_PREFIX_LEN) ||
	    !strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return 0;

	/*
	 * The trusted.* namespace can only be accessed by privileged users.
	 */
	if (!strncmp(name, XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN)) {
		if (!capable(CAP_SYS_ADMIN))
			return (mask & MAY_WRITE) ? -EPERM : -ENODATA;
		return 0;
	}

	/*
	 * In the user.* namespace, only regular files and directories can have
	 * extended attributes. For sticky directories, only the owner and
	 * privileged users can write attributes.
	 */
	if (!strncmp(name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)) {
		if (!S_ISREG(inode->i_mode) && !S_ISDIR(inode->i_mode))
			return (mask & MAY_WRITE) ? -EPERM : -ENODATA;
		if (S_ISDIR(inode->i_mode) && (inode->i_mode & S_ISVTX) &&
<<<<<<< HEAD
		    (mask & MAY_WRITE) && !inode_owner_or_capable(inode))
			return -EPERM;
	}

	return inode_permission2(ERR_PTR(-EOPNOTSUPP), inode, mask);
}

=======
		    (mask & MAY_WRITE) &&
		    !inode_owner_or_capable(idmap, inode))
			return -EPERM;
	}

	return inode_permission(idmap, inode, mask);
}

/*
 * Look for any handler that deals with the specified namespace.
 */
int
xattr_supports_user_prefix(struct inode *inode)
{
	const struct xattr_handler * const *handlers = inode->i_sb->s_xattr;
	const struct xattr_handler *handler;

	if (!(inode->i_opflags & IOP_XATTR)) {
		if (unlikely(is_bad_inode(inode)))
			return -EIO;
		return -EOPNOTSUPP;
	}

	for_each_xattr_handler(handlers, handler) {
		if (!strncmp(xattr_prefix(handler), XATTR_USER_PREFIX,
			     XATTR_USER_PREFIX_LEN))
			return 0;
	}

	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(xattr_supports_user_prefix);

int
__vfs_setxattr(struct mnt_idmap *idmap, struct dentry *dentry,
	       struct inode *inode, const char *name, const void *value,
	       size_t size, int flags)
{
	const struct xattr_handler *handler;

	if (is_posix_acl_xattr(name))
		return -EOPNOTSUPP;

	handler = xattr_resolve_name(inode, &name);
	if (IS_ERR(handler))
		return PTR_ERR(handler);
	if (!handler->set)
		return -EOPNOTSUPP;
	if (size == 0)
		value = "";  /* empty EA, do not remove */
	return handler->set(handler, idmap, dentry, inode, name, value,
			    size, flags);
}
EXPORT_SYMBOL(__vfs_setxattr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 *  __vfs_setxattr_noperm - perform setxattr operation without performing
 *  permission checks.
 *
<<<<<<< HEAD
 *  @dentry - object to perform setxattr on
 *  @name - xattr name to set
 *  @value - value to set @name to
 *  @size - size of @value
 *  @flags - flags to pass into filesystem operations
=======
 *  @idmap: idmap of the mount the inode was found from
 *  @dentry: object to perform setxattr on
 *  @name: xattr name to set
 *  @value: value to set @name to
 *  @size: size of @value
 *  @flags: flags to pass into filesystem operations
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  returns the result of the internal setxattr or setsecurity operations.
 *
 *  This function requires the caller to lock the inode's i_mutex before it
 *  is executed. It also assumes that the caller will make the appropriate
 *  permission checks.
 */
<<<<<<< HEAD
int __vfs_setxattr_noperm(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	int error = -EOPNOTSUPP;
=======
int __vfs_setxattr_noperm(struct mnt_idmap *idmap,
			  struct dentry *dentry, const char *name,
			  const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	int error = -EAGAIN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int issec = !strncmp(name, XATTR_SECURITY_PREFIX,
				   XATTR_SECURITY_PREFIX_LEN);

	if (issec)
		inode->i_flags &= ~S_NOSEC;
<<<<<<< HEAD
	if (inode->i_op->setxattr) {
		error = inode->i_op->setxattr(dentry, name, value, size, flags);
=======
	if (inode->i_opflags & IOP_XATTR) {
		error = __vfs_setxattr(idmap, dentry, inode, name, value,
				       size, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!error) {
			fsnotify_xattr(dentry);
			security_inode_post_setxattr(dentry, name, value,
						     size, flags);
		}
<<<<<<< HEAD
	} else if (issec) {
		const char *suffix = name + XATTR_SECURITY_PREFIX_LEN;
		error = security_inode_setsecurity(inode, suffix, value,
						   size, flags);
		if (!error)
			fsnotify_xattr(dentry);
=======
	} else {
		if (unlikely(is_bad_inode(inode)))
			return -EIO;
	}
	if (error == -EAGAIN) {
		error = -EOPNOTSUPP;

		if (issec) {
			const char *suffix = name + XATTR_SECURITY_PREFIX_LEN;

			error = security_inode_setsecurity(inode, suffix, value,
							   size, flags);
			if (!error)
				fsnotify_xattr(dentry);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return error;
}

<<<<<<< HEAD

int
vfs_setxattr(struct dentry *dentry, const char *name, const void *value,
		size_t size, int flags)
=======
/**
 * __vfs_setxattr_locked - set an extended attribute while holding the inode
 * lock
 *
 *  @idmap: idmap of the mount of the target inode
 *  @dentry: object to perform setxattr on
 *  @name: xattr name to set
 *  @value: value to set @name to
 *  @size: size of @value
 *  @flags: flags to pass into filesystem operations
 *  @delegated_inode: on return, will contain an inode pointer that
 *  a delegation was broken on, NULL if none.
 */
int
__vfs_setxattr_locked(struct mnt_idmap *idmap, struct dentry *dentry,
		      const char *name, const void *value, size_t size,
		      int flags, struct inode **delegated_inode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = dentry->d_inode;
	int error;

<<<<<<< HEAD
	error = xattr_permission(inode, name, MAY_WRITE);
	if (error)
		return error;

	mutex_lock(&inode->i_mutex);
	error = security_inode_setxattr(dentry, name, value, size, flags);
	if (error)
		goto out;

	error = __vfs_setxattr_noperm(dentry, name, value, size, flags);

out:
	mutex_unlock(&inode->i_mutex);
=======
	error = xattr_permission(idmap, inode, name, MAY_WRITE);
	if (error)
		return error;

	error = security_inode_setxattr(idmap, dentry, name, value, size,
					flags);
	if (error)
		goto out;

	error = try_break_deleg(inode, delegated_inode);
	if (error)
		goto out;

	error = __vfs_setxattr_noperm(idmap, dentry, name, value,
				      size, flags);

out:
	return error;
}
EXPORT_SYMBOL_GPL(__vfs_setxattr_locked);

int
vfs_setxattr(struct mnt_idmap *idmap, struct dentry *dentry,
	     const char *name, const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode *delegated_inode = NULL;
	const void  *orig_value = value;
	int error;

	if (size && strcmp(name, XATTR_NAME_CAPS) == 0) {
		error = cap_convert_nscap(idmap, dentry, &value, size);
		if (error < 0)
			return error;
		size = error;
	}

retry_deleg:
	inode_lock(inode);
	error = __vfs_setxattr_locked(idmap, dentry, name, value, size,
				      flags, &delegated_inode);
	inode_unlock(inode);

	if (delegated_inode) {
		error = break_deleg_wait(&delegated_inode);
		if (!error)
			goto retry_deleg;
	}
	if (value != orig_value)
		kfree(value);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}
EXPORT_SYMBOL_GPL(vfs_setxattr);

<<<<<<< HEAD
ssize_t
xattr_getsecurity(struct inode *inode, const char *name, void *value,
			size_t size)
=======
static ssize_t
xattr_getsecurity(struct mnt_idmap *idmap, struct inode *inode,
		  const char *name, void *value, size_t size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void *buffer = NULL;
	ssize_t len;

	if (!value || !size) {
<<<<<<< HEAD
		len = security_inode_getsecurity(inode, name, &buffer, false);
		goto out_noalloc;
	}

	len = security_inode_getsecurity(inode, name, &buffer, true);
=======
		len = security_inode_getsecurity(idmap, inode, name,
						 &buffer, false);
		goto out_noalloc;
	}

	len = security_inode_getsecurity(idmap, inode, name, &buffer,
					 true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len < 0)
		return len;
	if (size < len) {
		len = -ERANGE;
		goto out;
	}
	memcpy(value, buffer, len);
out:
<<<<<<< HEAD
	security_release_secctx(buffer, len);
out_noalloc:
	return len;
}
EXPORT_SYMBOL_GPL(xattr_getsecurity);
=======
	kfree(buffer);
out_noalloc:
	return len;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * vfs_getxattr_alloc - allocate memory, if necessary, before calling getxattr
 *
 * Allocate memory, if not already allocated, or re-allocate correct size,
<<<<<<< HEAD
 * before retrieving the extended attribute.
 *
 * Returns the result of alloc, if failed, or the getxattr operation.
 */
ssize_t
vfs_getxattr_alloc(struct dentry *dentry, const char *name, char **xattr_value,
		   size_t xattr_size, gfp_t flags)
{
=======
 * before retrieving the extended attribute.  The xattr value buffer should
 * always be freed by the caller, even on error.
 *
 * Returns the result of alloc, if failed, or the getxattr operation.
 */
int
vfs_getxattr_alloc(struct mnt_idmap *idmap, struct dentry *dentry,
		   const char *name, char **xattr_value, size_t xattr_size,
		   gfp_t flags)
{
	const struct xattr_handler *handler;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode = dentry->d_inode;
	char *value = *xattr_value;
	int error;

<<<<<<< HEAD
	error = xattr_permission(inode, name, MAY_READ);
	if (error)
		return error;

	if (!inode->i_op->getxattr)
		return -EOPNOTSUPP;

	error = inode->i_op->getxattr(dentry, name, NULL, 0);
=======
	error = xattr_permission(idmap, inode, name, MAY_READ);
	if (error)
		return error;

	handler = xattr_resolve_name(inode, &name);
	if (IS_ERR(handler))
		return PTR_ERR(handler);
	if (!handler->get)
		return -EOPNOTSUPP;
	error = handler->get(handler, dentry, inode, name, NULL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error < 0)
		return error;

	if (!value || (error > xattr_size)) {
		value = krealloc(*xattr_value, error + 1, flags);
		if (!value)
			return -ENOMEM;
		memset(value, 0, error + 1);
	}

<<<<<<< HEAD
	error = inode->i_op->getxattr(dentry, name, value, error);
=======
	error = handler->get(handler, dentry, inode, name, value, error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*xattr_value = value;
	return error;
}

<<<<<<< HEAD
/* Compare an extended attribute value with the given value */
int vfs_xattr_cmp(struct dentry *dentry, const char *xattr_name,
		  const char *value, size_t size, gfp_t flags)
{
	char *xattr_value = NULL;
	int rc;

	rc = vfs_getxattr_alloc(dentry, xattr_name, &xattr_value, 0, flags);
	if (rc < 0)
		return rc;

	if ((rc != size) || (memcmp(xattr_value, value, rc) != 0))
		rc = -EINVAL;
	else
		rc = 0;
	kfree(xattr_value);
	return rc;
}

ssize_t
vfs_getxattr(struct dentry *dentry, const char *name, void *value, size_t size)
=======
ssize_t
__vfs_getxattr(struct dentry *dentry, struct inode *inode, const char *name,
	       void *value, size_t size)
{
	const struct xattr_handler *handler;

	if (is_posix_acl_xattr(name))
		return -EOPNOTSUPP;

	handler = xattr_resolve_name(inode, &name);
	if (IS_ERR(handler))
		return PTR_ERR(handler);
	if (!handler->get)
		return -EOPNOTSUPP;
	return handler->get(handler, dentry, inode, name, value, size);
}
EXPORT_SYMBOL(__vfs_getxattr);

ssize_t
vfs_getxattr(struct mnt_idmap *idmap, struct dentry *dentry,
	     const char *name, void *value, size_t size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = dentry->d_inode;
	int error;

<<<<<<< HEAD
	error = xattr_permission(inode, name, MAY_READ);
=======
	error = xattr_permission(idmap, inode, name, MAY_READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;

	error = security_inode_getxattr(dentry, name);
	if (error)
		return error;

	if (!strncmp(name, XATTR_SECURITY_PREFIX,
				XATTR_SECURITY_PREFIX_LEN)) {
		const char *suffix = name + XATTR_SECURITY_PREFIX_LEN;
<<<<<<< HEAD
		int ret = xattr_getsecurity(inode, suffix, value, size);
=======
		int ret = xattr_getsecurity(idmap, inode, suffix, value,
					    size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Only overwrite the return value if a security module
		 * is actually active.
		 */
		if (ret == -EOPNOTSUPP)
			goto nolsm;
		return ret;
	}
nolsm:
<<<<<<< HEAD
	if (inode->i_op->getxattr)
		error = inode->i_op->getxattr(dentry, name, value, size);
	else
		error = -EOPNOTSUPP;

	return error;
}
EXPORT_SYMBOL_GPL(vfs_getxattr);

ssize_t
vfs_listxattr(struct dentry *d, char *list, size_t size)
{
	ssize_t error;

	error = security_inode_listxattr(d);
	if (error)
		return error;
	error = -EOPNOTSUPP;
	if (d->d_inode->i_op->listxattr) {
		error = d->d_inode->i_op->listxattr(d, list, size);
	} else {
		error = security_inode_listsecurity(d->d_inode, list, size);
=======
	return __vfs_getxattr(dentry, inode, name, value, size);
}
EXPORT_SYMBOL_GPL(vfs_getxattr);

/**
 * vfs_listxattr - retrieve \0 separated list of xattr names
 * @dentry: the dentry from whose inode the xattr names are retrieved
 * @list: buffer to store xattr names into
 * @size: size of the buffer
 *
 * This function returns the names of all xattrs associated with the
 * inode of @dentry.
 *
 * Note, for legacy reasons the vfs_listxattr() function lists POSIX
 * ACLs as well. Since POSIX ACLs are decoupled from IOP_XATTR the
 * vfs_listxattr() function doesn't check for this flag since a
 * filesystem could implement POSIX ACLs without implementing any other
 * xattrs.
 *
 * However, since all codepaths that remove IOP_XATTR also assign of
 * inode operations that either don't implement or implement a stub
 * ->listxattr() operation.
 *
 * Return: On success, the size of the buffer that was used. On error a
 *         negative error code.
 */
ssize_t
vfs_listxattr(struct dentry *dentry, char *list, size_t size)
{
	struct inode *inode = d_inode(dentry);
	ssize_t error;

	error = security_inode_listxattr(dentry);
	if (error)
		return error;

	if (inode->i_op->listxattr) {
		error = inode->i_op->listxattr(dentry, list, size);
	} else {
		error = security_inode_listsecurity(inode, list, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (size && error > size)
			error = -ERANGE;
	}
	return error;
}
EXPORT_SYMBOL_GPL(vfs_listxattr);

int
<<<<<<< HEAD
vfs_removexattr(struct dentry *dentry, const char *name)
=======
__vfs_removexattr(struct mnt_idmap *idmap, struct dentry *dentry,
		  const char *name)
{
	struct inode *inode = d_inode(dentry);
	const struct xattr_handler *handler;

	if (is_posix_acl_xattr(name))
		return -EOPNOTSUPP;

	handler = xattr_resolve_name(inode, &name);
	if (IS_ERR(handler))
		return PTR_ERR(handler);
	if (!handler->set)
		return -EOPNOTSUPP;
	return handler->set(handler, idmap, dentry, inode, name, NULL, 0,
			    XATTR_REPLACE);
}
EXPORT_SYMBOL(__vfs_removexattr);

/**
 * __vfs_removexattr_locked - set an extended attribute while holding the inode
 * lock
 *
 *  @idmap: idmap of the mount of the target inode
 *  @dentry: object to perform setxattr on
 *  @name: name of xattr to remove
 *  @delegated_inode: on return, will contain an inode pointer that
 *  a delegation was broken on, NULL if none.
 */
int
__vfs_removexattr_locked(struct mnt_idmap *idmap,
			 struct dentry *dentry, const char *name,
			 struct inode **delegated_inode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = dentry->d_inode;
	int error;

<<<<<<< HEAD
	if (!inode->i_op->removexattr)
		return -EOPNOTSUPP;

	error = xattr_permission(inode, name, MAY_WRITE);
	if (error)
		return error;

	error = security_inode_removexattr(dentry, name);
	if (error)
		return error;

	mutex_lock(&inode->i_mutex);
	error = inode->i_op->removexattr(dentry, name);
	mutex_unlock(&inode->i_mutex);

	if (!error) {
		fsnotify_xattr(dentry);
		evm_inode_post_removexattr(dentry, name);
	}
=======
	error = xattr_permission(idmap, inode, name, MAY_WRITE);
	if (error)
		return error;

	error = security_inode_removexattr(idmap, dentry, name);
	if (error)
		goto out;

	error = try_break_deleg(inode, delegated_inode);
	if (error)
		goto out;

	error = __vfs_removexattr(idmap, dentry, name);
	if (error)
		return error;

	fsnotify_xattr(dentry);
	security_inode_post_removexattr(dentry, name);

out:
	return error;
}
EXPORT_SYMBOL_GPL(__vfs_removexattr_locked);

int
vfs_removexattr(struct mnt_idmap *idmap, struct dentry *dentry,
		const char *name)
{
	struct inode *inode = dentry->d_inode;
	struct inode *delegated_inode = NULL;
	int error;

retry_deleg:
	inode_lock(inode);
	error = __vfs_removexattr_locked(idmap, dentry,
					 name, &delegated_inode);
	inode_unlock(inode);

	if (delegated_inode) {
		error = break_deleg_wait(&delegated_inode);
		if (!error)
			goto retry_deleg;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}
EXPORT_SYMBOL_GPL(vfs_removexattr);

<<<<<<< HEAD

/*
 * Extended attribute SET operations
 */
static long
setxattr(struct dentry *d, const char __user *name, const void __user *value,
	 size_t size, int flags)
{
	int error;
	void *kvalue = NULL;
	void *vvalue = NULL;	/* If non-NULL, we used vmalloc() */
	char kname[XATTR_NAME_MAX + 1];

	if (flags & ~(XATTR_CREATE|XATTR_REPLACE))
		return -EINVAL;

	error = strncpy_from_user(kname, name, sizeof(kname));
	if (error == 0 || error == sizeof(kname))
		error = -ERANGE;
	if (error < 0)
		return error;

	if (size) {
		if (size > XATTR_SIZE_MAX)
			return -E2BIG;
		kvalue = kmalloc(size, GFP_KERNEL | __GFP_NOWARN);
		if (!kvalue) {
			vvalue = vmalloc(size);
			if (!vvalue)
				return -ENOMEM;
			kvalue = vvalue;
		}
		if (copy_from_user(kvalue, value, size)) {
			error = -EFAULT;
			goto out;
		}
	}

	error = vfs_setxattr(d, kname, kvalue, size, flags);
out:
	if (vvalue)
		vfree(vvalue);
	else
		kfree(kvalue);
=======
/*
 * Extended attribute SET operations
 */

int setxattr_copy(const char __user *name, struct xattr_ctx *ctx)
{
	int error;

	if (ctx->flags & ~(XATTR_CREATE|XATTR_REPLACE))
		return -EINVAL;

	error = strncpy_from_user(ctx->kname->name, name,
				sizeof(ctx->kname->name));
	if (error == 0 || error == sizeof(ctx->kname->name))
		return  -ERANGE;
	if (error < 0)
		return error;

	error = 0;
	if (ctx->size) {
		if (ctx->size > XATTR_SIZE_MAX)
			return -E2BIG;

		ctx->kvalue = vmemdup_user(ctx->cvalue, ctx->size);
		if (IS_ERR(ctx->kvalue)) {
			error = PTR_ERR(ctx->kvalue);
			ctx->kvalue = NULL;
		}
	}

	return error;
}

int do_setxattr(struct mnt_idmap *idmap, struct dentry *dentry,
		struct xattr_ctx *ctx)
{
	if (is_posix_acl_xattr(ctx->kname->name))
		return do_set_acl(idmap, dentry, ctx->kname->name,
				  ctx->kvalue, ctx->size);

	return vfs_setxattr(idmap, dentry, ctx->kname->name,
			ctx->kvalue, ctx->size, ctx->flags);
}

static long
setxattr(struct mnt_idmap *idmap, struct dentry *d,
	const char __user *name, const void __user *value, size_t size,
	int flags)
{
	struct xattr_name kname;
	struct xattr_ctx ctx = {
		.cvalue   = value,
		.kvalue   = NULL,
		.size     = size,
		.kname    = &kname,
		.flags    = flags,
	};
	int error;

	error = setxattr_copy(name, &ctx);
	if (error)
		return error;

	error = do_setxattr(idmap, d, &ctx);

	kvfree(ctx.kvalue);
	return error;
}

static int path_setxattr(const char __user *pathname,
			 const char __user *name, const void __user *value,
			 size_t size, int flags, unsigned int lookup_flags)
{
	struct path path;
	int error;

retry:
	error = user_path_at(AT_FDCWD, pathname, lookup_flags, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = setxattr(mnt_idmap(path.mnt), path.dentry, name,
				 value, size, flags);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

SYSCALL_DEFINE5(setxattr, const char __user *, pathname,
		const char __user *, name, const void __user *, value,
		size_t, size, int, flags)
{
<<<<<<< HEAD
	struct path path;
	int error;

	error = user_path(pathname, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = setxattr(path.dentry, name, value, size, flags);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	return error;
=======
	return path_setxattr(pathname, name, value, size, flags, LOOKUP_FOLLOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE5(lsetxattr, const char __user *, pathname,
		const char __user *, name, const void __user *, value,
		size_t, size, int, flags)
{
<<<<<<< HEAD
	struct path path;
	int error;

	error = user_lpath(pathname, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = setxattr(path.dentry, name, value, size, flags);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	return error;
=======
	return path_setxattr(pathname, name, value, size, flags, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE5(fsetxattr, int, fd, const char __user *, name,
		const void __user *,value, size_t, size, int, flags)
{
<<<<<<< HEAD
	struct file *f;
	struct dentry *dentry;
	int error = -EBADF;

	f = fget(fd);
	if (!f)
		return error;
	dentry = f->f_path.dentry;
	audit_inode(NULL, dentry);
	error = mnt_want_write_file(f);
	if (!error) {
		error = setxattr(dentry, name, value, size, flags);
		mnt_drop_write_file(f);
	}
	fput(f);
=======
	struct fd f = fdget(fd);
	int error = -EBADF;

	if (!f.file)
		return error;
	audit_file(f.file);
	error = mnt_want_write_file(f.file);
	if (!error) {
		error = setxattr(file_mnt_idmap(f.file),
				 f.file->f_path.dentry, name,
				 value, size, flags);
		mnt_drop_write_file(f.file);
	}
	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Extended attribute GET operations
 */
<<<<<<< HEAD
static ssize_t
getxattr(struct dentry *d, const char __user *name, void __user *value,
	 size_t size)
{
	ssize_t error;
	void *kvalue = NULL;
	char kname[XATTR_NAME_MAX + 1];

	error = strncpy_from_user(kname, name, sizeof(kname));
	if (error == 0 || error == sizeof(kname))
		error = -ERANGE;
	if (error < 0)
		return error;

	if (size) {
		if (size > XATTR_SIZE_MAX)
			size = XATTR_SIZE_MAX;
		kvalue = kzalloc(size, GFP_KERNEL);
		if (!kvalue)
			return -ENOMEM;
	}

	error = vfs_getxattr(d, kname, kvalue, size);
	if (error > 0) {
		if (size && copy_to_user(value, kvalue, error))
			error = -EFAULT;
	} else if (error == -ERANGE && size >= XATTR_SIZE_MAX) {
=======
ssize_t
do_getxattr(struct mnt_idmap *idmap, struct dentry *d,
	struct xattr_ctx *ctx)
{
	ssize_t error;
	char *kname = ctx->kname->name;

	if (ctx->size) {
		if (ctx->size > XATTR_SIZE_MAX)
			ctx->size = XATTR_SIZE_MAX;
		ctx->kvalue = kvzalloc(ctx->size, GFP_KERNEL);
		if (!ctx->kvalue)
			return -ENOMEM;
	}

	if (is_posix_acl_xattr(ctx->kname->name))
		error = do_get_acl(idmap, d, kname, ctx->kvalue, ctx->size);
	else
		error = vfs_getxattr(idmap, d, kname, ctx->kvalue, ctx->size);
	if (error > 0) {
		if (ctx->size && copy_to_user(ctx->value, ctx->kvalue, error))
			error = -EFAULT;
	} else if (error == -ERANGE && ctx->size >= XATTR_SIZE_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* The file system tried to returned a value bigger
		   than XATTR_SIZE_MAX bytes. Not possible. */
		error = -E2BIG;
	}
<<<<<<< HEAD
	kfree(kvalue);
=======

	return error;
}

static ssize_t
getxattr(struct mnt_idmap *idmap, struct dentry *d,
	 const char __user *name, void __user *value, size_t size)
{
	ssize_t error;
	struct xattr_name kname;
	struct xattr_ctx ctx = {
		.value    = value,
		.kvalue   = NULL,
		.size     = size,
		.kname    = &kname,
		.flags    = 0,
	};

	error = strncpy_from_user(kname.name, name, sizeof(kname.name));
	if (error == 0 || error == sizeof(kname.name))
		error = -ERANGE;
	if (error < 0)
		return error;

	error =  do_getxattr(idmap, d, &ctx);

	kvfree(ctx.kvalue);
	return error;
}

static ssize_t path_getxattr(const char __user *pathname,
			     const char __user *name, void __user *value,
			     size_t size, unsigned int lookup_flags)
{
	struct path path;
	ssize_t error;
retry:
	error = user_path_at(AT_FDCWD, pathname, lookup_flags, &path);
	if (error)
		return error;
	error = getxattr(mnt_idmap(path.mnt), path.dentry, name, value, size);
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

SYSCALL_DEFINE4(getxattr, const char __user *, pathname,
		const char __user *, name, void __user *, value, size_t, size)
{
<<<<<<< HEAD
	struct path path;
	ssize_t error;

	error = user_path(pathname, &path);
	if (error)
		return error;
	error = getxattr(path.dentry, name, value, size);
	path_put(&path);
	return error;
=======
	return path_getxattr(pathname, name, value, size, LOOKUP_FOLLOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE4(lgetxattr, const char __user *, pathname,
		const char __user *, name, void __user *, value, size_t, size)
{
<<<<<<< HEAD
	struct path path;
	ssize_t error;

	error = user_lpath(pathname, &path);
	if (error)
		return error;
	error = getxattr(path.dentry, name, value, size);
	path_put(&path);
	return error;
=======
	return path_getxattr(pathname, name, value, size, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE4(fgetxattr, int, fd, const char __user *, name,
		void __user *, value, size_t, size)
{
<<<<<<< HEAD
	struct file *f;
	ssize_t error = -EBADF;

	f = fget(fd);
	if (!f)
		return error;
	audit_inode(NULL, f->f_path.dentry);
	error = getxattr(f->f_path.dentry, name, value, size);
	fput(f);
=======
	struct fd f = fdget(fd);
	ssize_t error = -EBADF;

	if (!f.file)
		return error;
	audit_file(f.file);
	error = getxattr(file_mnt_idmap(f.file), f.file->f_path.dentry,
			 name, value, size);
	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Extended attribute LIST operations
 */
static ssize_t
listxattr(struct dentry *d, char __user *list, size_t size)
{
	ssize_t error;
	char *klist = NULL;
<<<<<<< HEAD
	char *vlist = NULL;	/* If non-NULL, we used vmalloc() */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (size) {
		if (size > XATTR_LIST_MAX)
			size = XATTR_LIST_MAX;
<<<<<<< HEAD
		klist = kmalloc(size, __GFP_NOWARN | GFP_KERNEL);
		if (!klist) {
			vlist = vmalloc(size);
			if (!vlist)
				return -ENOMEM;
			klist = vlist;
		}
=======
		klist = kvmalloc(size, GFP_KERNEL);
		if (!klist)
			return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	error = vfs_listxattr(d, klist, size);
	if (error > 0) {
		if (size && copy_to_user(list, klist, error))
			error = -EFAULT;
	} else if (error == -ERANGE && size >= XATTR_LIST_MAX) {
		/* The file system tried to returned a list bigger
		   than XATTR_LIST_MAX bytes. Not possible. */
		error = -E2BIG;
	}
<<<<<<< HEAD
	if (vlist)
		vfree(vlist);
	else
		kfree(klist);
=======

	kvfree(klist);

	return error;
}

static ssize_t path_listxattr(const char __user *pathname, char __user *list,
			      size_t size, unsigned int lookup_flags)
{
	struct path path;
	ssize_t error;
retry:
	error = user_path_at(AT_FDCWD, pathname, lookup_flags, &path);
	if (error)
		return error;
	error = listxattr(path.dentry, list, size);
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

SYSCALL_DEFINE3(listxattr, const char __user *, pathname, char __user *, list,
		size_t, size)
{
<<<<<<< HEAD
	struct path path;
	ssize_t error;

	error = user_path(pathname, &path);
	if (error)
		return error;
	error = listxattr(path.dentry, list, size);
	path_put(&path);
	return error;
=======
	return path_listxattr(pathname, list, size, LOOKUP_FOLLOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE3(llistxattr, const char __user *, pathname, char __user *, list,
		size_t, size)
{
<<<<<<< HEAD
	struct path path;
	ssize_t error;

	error = user_lpath(pathname, &path);
	if (error)
		return error;
	error = listxattr(path.dentry, list, size);
	path_put(&path);
	return error;
=======
	return path_listxattr(pathname, list, size, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE3(flistxattr, int, fd, char __user *, list, size_t, size)
{
<<<<<<< HEAD
	struct file *f;
	ssize_t error = -EBADF;

	f = fget(fd);
	if (!f)
		return error;
	audit_inode(NULL, f->f_path.dentry);
	error = listxattr(f->f_path.dentry, list, size);
	fput(f);
=======
	struct fd f = fdget(fd);
	ssize_t error = -EBADF;

	if (!f.file)
		return error;
	audit_file(f.file);
	error = listxattr(f.file->f_path.dentry, list, size);
	fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return error;
}

/*
 * Extended attribute REMOVE operations
 */
static long
<<<<<<< HEAD
removexattr(struct dentry *d, const char __user *name)
=======
removexattr(struct mnt_idmap *idmap, struct dentry *d,
	    const char __user *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int error;
	char kname[XATTR_NAME_MAX + 1];

	error = strncpy_from_user(kname, name, sizeof(kname));
	if (error == 0 || error == sizeof(kname))
		error = -ERANGE;
	if (error < 0)
		return error;

<<<<<<< HEAD
	return vfs_removexattr(d, kname);
=======
	if (is_posix_acl_xattr(kname))
		return vfs_remove_acl(idmap, d, kname);

	return vfs_removexattr(idmap, d, kname);
}

static int path_removexattr(const char __user *pathname,
			    const char __user *name, unsigned int lookup_flags)
{
	struct path path;
	int error;
retry:
	error = user_path_at(AT_FDCWD, pathname, lookup_flags, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = removexattr(mnt_idmap(path.mnt), path.dentry, name);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE2(removexattr, const char __user *, pathname,
		const char __user *, name)
{
<<<<<<< HEAD
	struct path path;
	int error;

	error = user_path(pathname, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = removexattr(path.dentry, name);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	return error;
=======
	return path_removexattr(pathname, name, LOOKUP_FOLLOW);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE2(lremovexattr, const char __user *, pathname,
		const char __user *, name)
{
<<<<<<< HEAD
	struct path path;
	int error;

	error = user_lpath(pathname, &path);
	if (error)
		return error;
	error = mnt_want_write(path.mnt);
	if (!error) {
		error = removexattr(path.dentry, name);
		mnt_drop_write(path.mnt);
	}
	path_put(&path);
	return error;
=======
	return path_removexattr(pathname, name, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE2(fremovexattr, int, fd, const char __user *, name)
{
<<<<<<< HEAD
	struct file *f;
	struct dentry *dentry;
	int error = -EBADF;

	f = fget(fd);
	if (!f)
		return error;
	dentry = f->f_path.dentry;
	audit_inode(NULL, dentry);
	error = mnt_want_write_file(f);
	if (!error) {
		error = removexattr(dentry, name);
		mnt_drop_write_file(f);
	}
	fput(f);
	return error;
}


static const char *
strcmp_prefix(const char *a, const char *a_prefix)
{
	while (*a_prefix && *a == *a_prefix) {
		a++;
		a_prefix++;
	}
	return *a_prefix ? NULL : a;
}

/*
 * In order to implement different sets of xattr operations for each xattr
 * prefix with the generic xattr API, a filesystem should create a
 * null-terminated array of struct xattr_handler (one for each prefix) and
 * hang a pointer to it off of the s_xattr field of the superblock.
 *
 * The generic_fooxattr() functions will use this list to dispatch xattr
 * operations to the correct xattr_handler.
 */
#define for_each_xattr_handler(handlers, handler)		\
		for ((handler) = *(handlers)++;			\
			(handler) != NULL;			\
			(handler) = *(handlers)++)

/*
 * Find the xattr_handler with the matching prefix.
 */
static const struct xattr_handler *
xattr_resolve_name(const struct xattr_handler **handlers, const char **name)
{
	const struct xattr_handler *handler;

	if (!*name)
		return NULL;

	for_each_xattr_handler(handlers, handler) {
		const char *n = strcmp_prefix(*name, handler->prefix);
		if (n) {
			*name = n;
			break;
		}
	}
	return handler;
}

/*
 * Find the handler for the prefix and dispatch its get() operation.
 */
ssize_t
generic_getxattr(struct dentry *dentry, const char *name, void *buffer, size_t size)
{
	const struct xattr_handler *handler;

	handler = xattr_resolve_name(dentry->d_sb->s_xattr, &name);
	if (!handler)
		return -EOPNOTSUPP;
	return handler->get(dentry, name, buffer, size, handler->flags);
}

/*
 * Combine the results of the list() operation from every xattr_handler in the
 * list.
=======
	struct fd f = fdget(fd);
	int error = -EBADF;

	if (!f.file)
		return error;
	audit_file(f.file);
	error = mnt_want_write_file(f.file);
	if (!error) {
		error = removexattr(file_mnt_idmap(f.file),
				    f.file->f_path.dentry, name);
		mnt_drop_write_file(f.file);
	}
	fdput(f);
	return error;
}

int xattr_list_one(char **buffer, ssize_t *remaining_size, const char *name)
{
	size_t len;

	len = strlen(name) + 1;
	if (*buffer) {
		if (*remaining_size < len)
			return -ERANGE;
		memcpy(*buffer, name, len);
		*buffer += len;
	}
	*remaining_size -= len;
	return 0;
}

/**
 * generic_listxattr - run through a dentry's xattr list() operations
 * @dentry: dentry to list the xattrs
 * @buffer: result buffer
 * @buffer_size: size of @buffer
 *
 * Combine the results of the list() operation from every xattr_handler in the
 * xattr_handler stack.
 *
 * Note that this will not include the entries for POSIX ACLs.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
ssize_t
generic_listxattr(struct dentry *dentry, char *buffer, size_t buffer_size)
{
<<<<<<< HEAD
	const struct xattr_handler *handler, **handlers = dentry->d_sb->s_xattr;
	unsigned int size = 0;

	if (!buffer) {
		for_each_xattr_handler(handlers, handler) {
			size += handler->list(dentry, NULL, 0, NULL, 0,
					      handler->flags);
		}
	} else {
		char *buf = buffer;

		for_each_xattr_handler(handlers, handler) {
			size = handler->list(dentry, buf, buffer_size,
					     NULL, 0, handler->flags);
			if (size > buffer_size)
				return -ERANGE;
			buf += size;
			buffer_size -= size;
		}
		size = buf - buffer;
	}
	return size;
}

/*
 * Find the handler for the prefix and dispatch its set() operation.
 */
int
generic_setxattr(struct dentry *dentry, const char *name, const void *value, size_t size, int flags)
{
	const struct xattr_handler *handler;

	if (size == 0)
		value = "";  /* empty EA, do not remove */
	handler = xattr_resolve_name(dentry->d_sb->s_xattr, &name);
	if (!handler)
		return -EOPNOTSUPP;
	return handler->set(dentry, name, value, size, flags, handler->flags);
}

/*
 * Find the handler for the prefix and dispatch its set() operation to remove
 * any associated extended attribute.
 */
int
generic_removexattr(struct dentry *dentry, const char *name)
{
	const struct xattr_handler *handler;

	handler = xattr_resolve_name(dentry->d_sb->s_xattr, &name);
	if (!handler)
		return -EOPNOTSUPP;
	return handler->set(dentry, name, NULL, 0,
			    XATTR_REPLACE, handler->flags);
}

EXPORT_SYMBOL(generic_getxattr);
EXPORT_SYMBOL(generic_listxattr);
EXPORT_SYMBOL(generic_setxattr);
EXPORT_SYMBOL(generic_removexattr);
=======
	const struct xattr_handler *handler, * const *handlers = dentry->d_sb->s_xattr;
	ssize_t remaining_size = buffer_size;
	int err = 0;

	for_each_xattr_handler(handlers, handler) {
		if (!handler->name || (handler->list && !handler->list(dentry)))
			continue;
		err = xattr_list_one(&buffer, &remaining_size, handler->name);
		if (err)
			return err;
	}

	return err ? err : buffer_size - remaining_size;
}
EXPORT_SYMBOL(generic_listxattr);

/**
 * xattr_full_name  -  Compute full attribute name from suffix
 *
 * @handler:	handler of the xattr_handler operation
 * @name:	name passed to the xattr_handler operation
 *
 * The get and set xattr handler operations are called with the remainder of
 * the attribute name after skipping the handler's prefix: for example, "foo"
 * is passed to the get operation of a handler with prefix "user." to get
 * attribute "user.foo".  The full name is still "there" in the name though.
 *
 * Note: the list xattr handler operation when called from the vfs is passed a
 * NULL name; some file systems use this operation internally, with varying
 * semantics.
 */
const char *xattr_full_name(const struct xattr_handler *handler,
			    const char *name)
{
	size_t prefix_len = strlen(xattr_prefix(handler));

	return name - prefix_len;
}
EXPORT_SYMBOL(xattr_full_name);

/**
 * simple_xattr_space - estimate the memory used by a simple xattr
 * @name: the full name of the xattr
 * @size: the size of its value
 *
 * This takes no account of how much larger the two slab objects actually are:
 * that would depend on the slab implementation, when what is required is a
 * deterministic number, which grows with name length and size and quantity.
 *
 * Return: The approximate number of bytes of memory used by such an xattr.
 */
size_t simple_xattr_space(const char *name, size_t size)
{
	/*
	 * Use "40" instead of sizeof(struct simple_xattr), to return the
	 * same result on 32-bit and 64-bit, and even if simple_xattr grows.
	 */
	return 40 + size + strlen(name);
}

/**
 * simple_xattr_free - free an xattr object
 * @xattr: the xattr object
 *
 * Free the xattr object. Can handle @xattr being NULL.
 */
void simple_xattr_free(struct simple_xattr *xattr)
{
	if (xattr)
		kfree(xattr->name);
	kvfree(xattr);
}

/**
 * simple_xattr_alloc - allocate new xattr object
 * @value: value of the xattr object
 * @size: size of @value
 *
 * Allocate a new xattr object and initialize respective members. The caller is
 * responsible for handling the name of the xattr.
 *
 * Return: On success a new xattr object is returned. On failure NULL is
 * returned.
 */
struct simple_xattr *simple_xattr_alloc(const void *value, size_t size)
{
	struct simple_xattr *new_xattr;
	size_t len;

	/* wrap around? */
	len = sizeof(*new_xattr) + size;
	if (len < sizeof(*new_xattr))
		return NULL;

	new_xattr = kvmalloc(len, GFP_KERNEL_ACCOUNT);
	if (!new_xattr)
		return NULL;

	new_xattr->size = size;
	memcpy(new_xattr->value, value, size);
	return new_xattr;
}

/**
 * rbtree_simple_xattr_cmp - compare xattr name with current rbtree xattr entry
 * @key: xattr name
 * @node: current node
 *
 * Compare the xattr name with the xattr name attached to @node in the rbtree.
 *
 * Return: Negative value if continuing left, positive if continuing right, 0
 * if the xattr attached to @node matches @key.
 */
static int rbtree_simple_xattr_cmp(const void *key, const struct rb_node *node)
{
	const char *xattr_name = key;
	const struct simple_xattr *xattr;

	xattr = rb_entry(node, struct simple_xattr, rb_node);
	return strcmp(xattr->name, xattr_name);
}

/**
 * rbtree_simple_xattr_node_cmp - compare two xattr rbtree nodes
 * @new_node: new node
 * @node: current node
 *
 * Compare the xattr attached to @new_node with the xattr attached to @node.
 *
 * Return: Negative value if continuing left, positive if continuing right, 0
 * if the xattr attached to @new_node matches the xattr attached to @node.
 */
static int rbtree_simple_xattr_node_cmp(struct rb_node *new_node,
					const struct rb_node *node)
{
	struct simple_xattr *xattr;
	xattr = rb_entry(new_node, struct simple_xattr, rb_node);
	return rbtree_simple_xattr_cmp(xattr->name, node);
}

/**
 * simple_xattr_get - get an xattr object
 * @xattrs: the header of the xattr object
 * @name: the name of the xattr to retrieve
 * @buffer: the buffer to store the value into
 * @size: the size of @buffer
 *
 * Try to find and retrieve the xattr object associated with @name.
 * If @buffer is provided store the value of @xattr in @buffer
 * otherwise just return the length. The size of @buffer is limited
 * to XATTR_SIZE_MAX which currently is 65536.
 *
 * Return: On success the length of the xattr value is returned. On error a
 * negative error code is returned.
 */
int simple_xattr_get(struct simple_xattrs *xattrs, const char *name,
		     void *buffer, size_t size)
{
	struct simple_xattr *xattr = NULL;
	struct rb_node *rbp;
	int ret = -ENODATA;

	read_lock(&xattrs->lock);
	rbp = rb_find(name, &xattrs->rb_root, rbtree_simple_xattr_cmp);
	if (rbp) {
		xattr = rb_entry(rbp, struct simple_xattr, rb_node);
		ret = xattr->size;
		if (buffer) {
			if (size < xattr->size)
				ret = -ERANGE;
			else
				memcpy(buffer, xattr->value, xattr->size);
		}
	}
	read_unlock(&xattrs->lock);
	return ret;
}

/**
 * simple_xattr_set - set an xattr object
 * @xattrs: the header of the xattr object
 * @name: the name of the xattr to retrieve
 * @value: the value to store along the xattr
 * @size: the size of @value
 * @flags: the flags determining how to set the xattr
 *
 * Set a new xattr object.
 * If @value is passed a new xattr object will be allocated. If XATTR_REPLACE
 * is specified in @flags a matching xattr object for @name must already exist.
 * If it does it will be replaced with the new xattr object. If it doesn't we
 * fail. If XATTR_CREATE is specified and a matching xattr does already exist
 * we fail. If it doesn't we create a new xattr. If @flags is zero we simply
 * insert the new xattr replacing any existing one.
 *
 * If @value is empty and a matching xattr object is found we delete it if
 * XATTR_REPLACE is specified in @flags or @flags is zero.
 *
 * If @value is empty and no matching xattr object for @name is found we do
 * nothing if XATTR_CREATE is specified in @flags or @flags is zero. For
 * XATTR_REPLACE we fail as mentioned above.
 *
 * Return: On success, the removed or replaced xattr is returned, to be freed
 * by the caller; or NULL if none. On failure a negative error code is returned.
 */
struct simple_xattr *simple_xattr_set(struct simple_xattrs *xattrs,
				      const char *name, const void *value,
				      size_t size, int flags)
{
	struct simple_xattr *old_xattr = NULL, *new_xattr = NULL;
	struct rb_node *parent = NULL, **rbp;
	int err = 0, ret;

	/* value == NULL means remove */
	if (value) {
		new_xattr = simple_xattr_alloc(value, size);
		if (!new_xattr)
			return ERR_PTR(-ENOMEM);

		new_xattr->name = kstrdup(name, GFP_KERNEL_ACCOUNT);
		if (!new_xattr->name) {
			simple_xattr_free(new_xattr);
			return ERR_PTR(-ENOMEM);
		}
	}

	write_lock(&xattrs->lock);
	rbp = &xattrs->rb_root.rb_node;
	while (*rbp) {
		parent = *rbp;
		ret = rbtree_simple_xattr_cmp(name, *rbp);
		if (ret < 0)
			rbp = &(*rbp)->rb_left;
		else if (ret > 0)
			rbp = &(*rbp)->rb_right;
		else
			old_xattr = rb_entry(*rbp, struct simple_xattr, rb_node);
		if (old_xattr)
			break;
	}

	if (old_xattr) {
		/* Fail if XATTR_CREATE is requested and the xattr exists. */
		if (flags & XATTR_CREATE) {
			err = -EEXIST;
			goto out_unlock;
		}

		if (new_xattr)
			rb_replace_node(&old_xattr->rb_node,
					&new_xattr->rb_node, &xattrs->rb_root);
		else
			rb_erase(&old_xattr->rb_node, &xattrs->rb_root);
	} else {
		/* Fail if XATTR_REPLACE is requested but no xattr is found. */
		if (flags & XATTR_REPLACE) {
			err = -ENODATA;
			goto out_unlock;
		}

		/*
		 * If XATTR_CREATE or no flags are specified together with a
		 * new value simply insert it.
		 */
		if (new_xattr) {
			rb_link_node(&new_xattr->rb_node, parent, rbp);
			rb_insert_color(&new_xattr->rb_node, &xattrs->rb_root);
		}

		/*
		 * If XATTR_CREATE or no flags are specified and neither an
		 * old or new xattr exist then we don't need to do anything.
		 */
	}

out_unlock:
	write_unlock(&xattrs->lock);
	if (!err)
		return old_xattr;
	simple_xattr_free(new_xattr);
	return ERR_PTR(err);
}

static bool xattr_is_trusted(const char *name)
{
	return !strncmp(name, XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN);
}

/**
 * simple_xattr_list - list all xattr objects
 * @inode: inode from which to get the xattrs
 * @xattrs: the header of the xattr object
 * @buffer: the buffer to store all xattrs into
 * @size: the size of @buffer
 *
 * List all xattrs associated with @inode. If @buffer is NULL we returned
 * the required size of the buffer. If @buffer is provided we store the
 * xattrs value into it provided it is big enough.
 *
 * Note, the number of xattr names that can be listed with listxattr(2) is
 * limited to XATTR_LIST_MAX aka 65536 bytes. If a larger buffer is passed
 * then vfs_listxattr() caps it to XATTR_LIST_MAX and if more xattr names
 * are found it will return -E2BIG.
 *
 * Return: On success the required size or the size of the copied xattrs is
 * returned. On error a negative error code is returned.
 */
ssize_t simple_xattr_list(struct inode *inode, struct simple_xattrs *xattrs,
			  char *buffer, size_t size)
{
	bool trusted = ns_capable_noaudit(&init_user_ns, CAP_SYS_ADMIN);
	struct simple_xattr *xattr;
	struct rb_node *rbp;
	ssize_t remaining_size = size;
	int err = 0;

	err = posix_acl_listxattr(inode, &buffer, &remaining_size);
	if (err)
		return err;

	read_lock(&xattrs->lock);
	for (rbp = rb_first(&xattrs->rb_root); rbp; rbp = rb_next(rbp)) {
		xattr = rb_entry(rbp, struct simple_xattr, rb_node);

		/* skip "trusted." attributes for unprivileged callers */
		if (!trusted && xattr_is_trusted(xattr->name))
			continue;

		err = xattr_list_one(&buffer, &remaining_size, xattr->name);
		if (err)
			break;
	}
	read_unlock(&xattrs->lock);

	return err ? err : size - remaining_size;
}

/**
 * rbtree_simple_xattr_less - compare two xattr rbtree nodes
 * @new_node: new node
 * @node: current node
 *
 * Compare the xattr attached to @new_node with the xattr attached to @node.
 * Note that this function technically tolerates duplicate entries.
 *
 * Return: True if insertion point in the rbtree is found.
 */
static bool rbtree_simple_xattr_less(struct rb_node *new_node,
				     const struct rb_node *node)
{
	return rbtree_simple_xattr_node_cmp(new_node, node) < 0;
}

/**
 * simple_xattr_add - add xattr objects
 * @xattrs: the header of the xattr object
 * @new_xattr: the xattr object to add
 *
 * Add an xattr object to @xattrs. This assumes no replacement or removal
 * of matching xattrs is wanted. Should only be called during inode
 * initialization when a few distinct initial xattrs are supposed to be set.
 */
void simple_xattr_add(struct simple_xattrs *xattrs,
		      struct simple_xattr *new_xattr)
{
	write_lock(&xattrs->lock);
	rb_add(&new_xattr->rb_node, &xattrs->rb_root, rbtree_simple_xattr_less);
	write_unlock(&xattrs->lock);
}

/**
 * simple_xattrs_init - initialize new xattr header
 * @xattrs: header to initialize
 *
 * Initialize relevant fields of a an xattr header.
 */
void simple_xattrs_init(struct simple_xattrs *xattrs)
{
	xattrs->rb_root = RB_ROOT;
	rwlock_init(&xattrs->lock);
}

/**
 * simple_xattrs_free - free xattrs
 * @xattrs: xattr header whose xattrs to destroy
 * @freed_space: approximate number of bytes of memory freed from @xattrs
 *
 * Destroy all xattrs in @xattr. When this is called no one can hold a
 * reference to any of the xattrs anymore.
 */
void simple_xattrs_free(struct simple_xattrs *xattrs, size_t *freed_space)
{
	struct rb_node *rbp;

	if (freed_space)
		*freed_space = 0;
	rbp = rb_first(&xattrs->rb_root);
	while (rbp) {
		struct simple_xattr *xattr;
		struct rb_node *rbp_next;

		rbp_next = rb_next(rbp);
		xattr = rb_entry(rbp, struct simple_xattr, rb_node);
		rb_erase(&xattr->rb_node, &xattrs->rb_root);
		if (freed_space)
			*freed_space += simple_xattr_space(xattr->name,
							   xattr->size);
		simple_xattr_free(xattr);
		rbp = rbp_next;
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
