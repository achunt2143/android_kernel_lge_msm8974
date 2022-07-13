<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
  File: linux/xattr.h

  Extended attributes handling.

  Copyright (C) 2001 by Andreas Gruenbacher <a.gruenbacher@computer.org>
  Copyright (c) 2001-2002 Silicon Graphics, Inc.  All Rights Reserved.
  Copyright (c) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
*/
#ifndef _LINUX_XATTR_H
#define _LINUX_XATTR_H

<<<<<<< HEAD
#define XATTR_CREATE	0x1	/* set value, fail if attr already exists */
#define XATTR_REPLACE	0x2	/* set value, fail if attr does not exist */

/* Namespaces */
#define XATTR_OS2_PREFIX "os2."
#define XATTR_OS2_PREFIX_LEN (sizeof (XATTR_OS2_PREFIX) - 1)

#define XATTR_SECURITY_PREFIX	"security."
#define XATTR_SECURITY_PREFIX_LEN (sizeof (XATTR_SECURITY_PREFIX) - 1)

#define XATTR_SYSTEM_PREFIX "system."
#define XATTR_SYSTEM_PREFIX_LEN (sizeof (XATTR_SYSTEM_PREFIX) - 1)

#define XATTR_TRUSTED_PREFIX "trusted."
#define XATTR_TRUSTED_PREFIX_LEN (sizeof (XATTR_TRUSTED_PREFIX) - 1)

#define XATTR_USER_PREFIX "user."
#define XATTR_USER_PREFIX_LEN (sizeof (XATTR_USER_PREFIX) - 1)

/* Security namespace */
#define XATTR_EVM_SUFFIX "evm"
#define XATTR_NAME_EVM XATTR_SECURITY_PREFIX XATTR_EVM_SUFFIX

#define XATTR_SELINUX_SUFFIX "selinux"
#define XATTR_NAME_SELINUX XATTR_SECURITY_PREFIX XATTR_SELINUX_SUFFIX

#define XATTR_SMACK_SUFFIX "SMACK64"
#define XATTR_SMACK_IPIN "SMACK64IPIN"
#define XATTR_SMACK_IPOUT "SMACK64IPOUT"
#define XATTR_SMACK_EXEC "SMACK64EXEC"
#define XATTR_SMACK_TRANSMUTE "SMACK64TRANSMUTE"
#define XATTR_SMACK_MMAP "SMACK64MMAP"
#define XATTR_NAME_SMACK XATTR_SECURITY_PREFIX XATTR_SMACK_SUFFIX
#define XATTR_NAME_SMACKIPIN	XATTR_SECURITY_PREFIX XATTR_SMACK_IPIN
#define XATTR_NAME_SMACKIPOUT	XATTR_SECURITY_PREFIX XATTR_SMACK_IPOUT
#define XATTR_NAME_SMACKEXEC	XATTR_SECURITY_PREFIX XATTR_SMACK_EXEC
#define XATTR_NAME_SMACKTRANSMUTE XATTR_SECURITY_PREFIX XATTR_SMACK_TRANSMUTE
#define XATTR_NAME_SMACKMMAP XATTR_SECURITY_PREFIX XATTR_SMACK_MMAP

#define XATTR_CAPS_SUFFIX "capability"
#define XATTR_NAME_CAPS XATTR_SECURITY_PREFIX XATTR_CAPS_SUFFIX

#define XATTR_POSIX_ACL_ACCESS  "posix_acl_access"
#define XATTR_NAME_POSIX_ACL_ACCESS XATTR_SYSTEM_PREFIX XATTR_POSIX_ACL_ACCESS
#define XATTR_POSIX_ACL_DEFAULT  "posix_acl_default"
#define XATTR_NAME_POSIX_ACL_DEFAULT XATTR_SYSTEM_PREFIX XATTR_POSIX_ACL_DEFAULT

#ifdef  __KERNEL__

#include <linux/types.h>
=======

#include <linux/slab.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/user_namespace.h>
#include <uapi/linux/xattr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct inode;
struct dentry;

<<<<<<< HEAD
struct xattr_handler {
	const char *prefix;
	int flags;	/* fs private flags passed back to the handlers */
	size_t (*list)(struct dentry *dentry, char *list, size_t list_size,
		       const char *name, size_t name_len, int handler_flags);
	int (*get)(struct dentry *dentry, const char *name, void *buffer,
		   size_t size, int handler_flags);
	int (*set)(struct dentry *dentry, const char *name, const void *buffer,
		   size_t size, int flags, int handler_flags);
};

struct xattr {
	char *name;
=======
static inline bool is_posix_acl_xattr(const char *name)
{
	return (strcmp(name, XATTR_NAME_POSIX_ACL_ACCESS) == 0) ||
	       (strcmp(name, XATTR_NAME_POSIX_ACL_DEFAULT) == 0);
}

/*
 * struct xattr_handler: When @name is set, match attributes with exactly that
 * name.  When @prefix is set instead, match attributes with that prefix and
 * with a non-empty suffix.
 */
struct xattr_handler {
	const char *name;
	const char *prefix;
	int flags;      /* fs private flags */
	bool (*list)(struct dentry *dentry);
	int (*get)(const struct xattr_handler *, struct dentry *dentry,
		   struct inode *inode, const char *name, void *buffer,
		   size_t size);
	int (*set)(const struct xattr_handler *,
		   struct mnt_idmap *idmap, struct dentry *dentry,
		   struct inode *inode, const char *name, const void *buffer,
		   size_t size, int flags);
};

/**
 * xattr_handler_can_list - check whether xattr can be listed
 * @handler: handler for this type of xattr
 * @dentry: dentry whose inode xattr to list
 *
 * Determine whether the xattr associated with @dentry can be listed given
 * @handler.
 *
 * Return: true if xattr can be listed, false if not.
 */
static inline bool xattr_handler_can_list(const struct xattr_handler *handler,
					  struct dentry *dentry)
{
	return handler && (!handler->list || handler->list(dentry));
}

const char *xattr_full_name(const struct xattr_handler *, const char *);

struct xattr {
	const char *name;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void *value;
	size_t value_len;
};

<<<<<<< HEAD
ssize_t xattr_getsecurity(struct inode *, const char *, void *, size_t);
ssize_t vfs_getxattr(struct dentry *, const char *, void *, size_t);
ssize_t vfs_listxattr(struct dentry *d, char *list, size_t size);
int __vfs_setxattr_noperm(struct dentry *, const char *, const void *, size_t, int);
int vfs_setxattr(struct dentry *, const char *, const void *, size_t, int);
int vfs_removexattr(struct dentry *, const char *);

ssize_t generic_getxattr(struct dentry *dentry, const char *name, void *buffer, size_t size);
ssize_t generic_listxattr(struct dentry *dentry, char *buffer, size_t buffer_size);
int generic_setxattr(struct dentry *dentry, const char *name, const void *value, size_t size, int flags);
int generic_removexattr(struct dentry *dentry, const char *name);
ssize_t vfs_getxattr_alloc(struct dentry *dentry, const char *name,
			   char **xattr_value, size_t size, gfp_t flags);
int vfs_xattr_cmp(struct dentry *dentry, const char *xattr_name,
		  const char *value, size_t size, gfp_t flags);
#endif  /*  __KERNEL__  */
=======
ssize_t __vfs_getxattr(struct dentry *, struct inode *, const char *, void *, size_t);
ssize_t vfs_getxattr(struct mnt_idmap *, struct dentry *, const char *,
		     void *, size_t);
ssize_t vfs_listxattr(struct dentry *d, char *list, size_t size);
int __vfs_setxattr(struct mnt_idmap *, struct dentry *, struct inode *,
		   const char *, const void *, size_t, int);
int __vfs_setxattr_noperm(struct mnt_idmap *, struct dentry *,
			  const char *, const void *, size_t, int);
int __vfs_setxattr_locked(struct mnt_idmap *, struct dentry *,
			  const char *, const void *, size_t, int,
			  struct inode **);
int vfs_setxattr(struct mnt_idmap *, struct dentry *, const char *,
		 const void *, size_t, int);
int __vfs_removexattr(struct mnt_idmap *, struct dentry *, const char *);
int __vfs_removexattr_locked(struct mnt_idmap *, struct dentry *,
			     const char *, struct inode **);
int vfs_removexattr(struct mnt_idmap *, struct dentry *, const char *);

ssize_t generic_listxattr(struct dentry *dentry, char *buffer, size_t buffer_size);
int vfs_getxattr_alloc(struct mnt_idmap *idmap,
		       struct dentry *dentry, const char *name,
		       char **xattr_value, size_t size, gfp_t flags);

int xattr_supports_user_prefix(struct inode *inode);

static inline const char *xattr_prefix(const struct xattr_handler *handler)
{
	return handler->prefix ?: handler->name;
}

struct simple_xattrs {
	struct rb_root rb_root;
	rwlock_t lock;
};

struct simple_xattr {
	struct rb_node rb_node;
	char *name;
	size_t size;
	char value[];
};

void simple_xattrs_init(struct simple_xattrs *xattrs);
void simple_xattrs_free(struct simple_xattrs *xattrs, size_t *freed_space);
size_t simple_xattr_space(const char *name, size_t size);
struct simple_xattr *simple_xattr_alloc(const void *value, size_t size);
void simple_xattr_free(struct simple_xattr *xattr);
int simple_xattr_get(struct simple_xattrs *xattrs, const char *name,
		     void *buffer, size_t size);
struct simple_xattr *simple_xattr_set(struct simple_xattrs *xattrs,
				      const char *name, const void *value,
				      size_t size, int flags);
ssize_t simple_xattr_list(struct inode *inode, struct simple_xattrs *xattrs,
			  char *buffer, size_t size);
void simple_xattr_add(struct simple_xattrs *xattrs,
		      struct simple_xattr *new_xattr);
int xattr_list_one(char **buffer, ssize_t *remaining_size, const char *name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* _LINUX_XATTR_H */
