<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
  File: linux/posix_acl_xattr.h

  Extended attribute system call representation of Access Control Lists.

  Copyright (C) 2000 by Andreas Gruenbacher <a.gruenbacher@computer.org>
  Copyright (C) 2002 SGI - Silicon Graphics, Inc <linux-xfs@oss.sgi.com>
 */
#ifndef _POSIX_ACL_XATTR_H
#define _POSIX_ACL_XATTR_H

<<<<<<< HEAD
#include <linux/posix_acl.h>

/* Extended attribute names */
#define POSIX_ACL_XATTR_ACCESS	"system.posix_acl_access"
#define POSIX_ACL_XATTR_DEFAULT	"system.posix_acl_default"

/* Supported ACL a_version fields */
#define POSIX_ACL_XATTR_VERSION	0x0002


/* An undefined entry e_id value */
#define ACL_UNDEFINED_ID	(-1)

typedef struct {
	__le16			e_tag;
	__le16			e_perm;
	__le32			e_id;
} posix_acl_xattr_entry;

typedef struct {
	__le32			a_version;
	posix_acl_xattr_entry	a_entries[0];
} posix_acl_xattr_header;


static inline size_t
posix_acl_xattr_size(int count)
{
	return (sizeof(posix_acl_xattr_header) +
		(count * sizeof(posix_acl_xattr_entry)));
=======
#include <uapi/linux/xattr.h>
#include <uapi/linux/posix_acl_xattr.h>
#include <linux/posix_acl.h>

static inline size_t
posix_acl_xattr_size(int count)
{
	return (sizeof(struct posix_acl_xattr_header) +
		(count * sizeof(struct posix_acl_xattr_entry)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int
posix_acl_xattr_count(size_t size)
{
<<<<<<< HEAD
	if (size < sizeof(posix_acl_xattr_header))
		return -1;
	size -= sizeof(posix_acl_xattr_header);
	if (size % sizeof(posix_acl_xattr_entry))
		return -1;
	return size / sizeof(posix_acl_xattr_entry);
}

struct posix_acl *posix_acl_from_xattr(const void *value, size_t size);
int posix_acl_to_xattr(const struct posix_acl *acl, void *buffer, size_t size);
=======
	if (size < sizeof(struct posix_acl_xattr_header))
		return -1;
	size -= sizeof(struct posix_acl_xattr_header);
	if (size % sizeof(struct posix_acl_xattr_entry))
		return -1;
	return size / sizeof(struct posix_acl_xattr_entry);
}

#ifdef CONFIG_FS_POSIX_ACL
struct posix_acl *posix_acl_from_xattr(struct user_namespace *user_ns,
				       const void *value, size_t size);
#else
static inline struct posix_acl *
posix_acl_from_xattr(struct user_namespace *user_ns, const void *value,
		     size_t size)
{
	return ERR_PTR(-EOPNOTSUPP);
}
#endif

int posix_acl_to_xattr(struct user_namespace *user_ns,
		       const struct posix_acl *acl, void *buffer, size_t size);
static inline const char *posix_acl_xattr_name(int type)
{
	switch (type) {
	case ACL_TYPE_ACCESS:
		return XATTR_NAME_POSIX_ACL_ACCESS;
	case ACL_TYPE_DEFAULT:
		return XATTR_NAME_POSIX_ACL_DEFAULT;
	}

	return "";
}

static inline int posix_acl_type(const char *name)
{
	if (strcmp(name, XATTR_NAME_POSIX_ACL_ACCESS) == 0)
		return ACL_TYPE_ACCESS;
	else if (strcmp(name, XATTR_NAME_POSIX_ACL_DEFAULT) == 0)
		return ACL_TYPE_DEFAULT;

	return -1;
}

/* These are legacy handlers. Don't use them for new code. */
extern const struct xattr_handler nop_posix_acl_access;
extern const struct xattr_handler nop_posix_acl_default;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* _POSIX_ACL_XATTR_H */
