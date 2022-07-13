<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
  File: linux/posix_acl.h

  (C) 2002 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/


#ifndef __LINUX_POSIX_ACL_H
#define __LINUX_POSIX_ACL_H

#include <linux/bug.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD

#define ACL_UNDEFINED_ID	(-1)

/* a_type field in acl_user_posix_entry_t */
#define ACL_TYPE_ACCESS		(0x8000)
#define ACL_TYPE_DEFAULT	(0x4000)

/* e_tag entry in struct posix_acl_entry */
#define ACL_USER_OBJ		(0x01)
#define ACL_USER		(0x02)
#define ACL_GROUP_OBJ		(0x04)
#define ACL_GROUP		(0x08)
#define ACL_MASK		(0x10)
#define ACL_OTHER		(0x20)

/* permissions in the e_perm field */
#define ACL_READ		(0x04)
#define ACL_WRITE		(0x02)
#define ACL_EXECUTE		(0x01)
//#define ACL_ADD		(0x08)
//#define ACL_DELETE		(0x10)
=======
#include <linux/refcount.h>
#include <uapi/linux/posix_acl.h>

struct user_namespace;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct posix_acl_entry {
	short			e_tag;
	unsigned short		e_perm;
<<<<<<< HEAD
	unsigned int		e_id;
};

struct posix_acl {
	union {
		atomic_t		a_refcount;
		struct rcu_head		a_rcu;
	};
	unsigned int		a_count;
	struct posix_acl_entry	a_entries[0];
=======
	union {
		kuid_t		e_uid;
		kgid_t		e_gid;
	};
};

struct posix_acl {
	refcount_t		a_refcount;
	struct rcu_head		a_rcu;
	unsigned int		a_count;
	struct posix_acl_entry	a_entries[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define FOREACH_ACL_ENTRY(pa, acl, pe) \
	for(pa=(acl)->a_entries, pe=pa+(acl)->a_count; pa<pe; pa++)


/*
 * Duplicate an ACL handle.
 */
static inline struct posix_acl *
posix_acl_dup(struct posix_acl *acl)
{
	if (acl)
<<<<<<< HEAD
		atomic_inc(&acl->a_refcount);
=======
		refcount_inc(&acl->a_refcount);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return acl;
}

/*
 * Free an ACL handle.
 */
static inline void
posix_acl_release(struct posix_acl *acl)
{
<<<<<<< HEAD
	if (acl && atomic_dec_and_test(&acl->a_refcount))
=======
	if (acl && refcount_dec_and_test(&acl->a_refcount))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree_rcu(acl, a_rcu);
}


/* posix_acl.c */

extern void posix_acl_init(struct posix_acl *, int);
extern struct posix_acl *posix_acl_alloc(int, gfp_t);
<<<<<<< HEAD
extern int posix_acl_valid(const struct posix_acl *);
extern int posix_acl_permission(struct inode *, const struct posix_acl *, int);
extern struct posix_acl *posix_acl_from_mode(umode_t, gfp_t);
extern int posix_acl_equiv_mode(const struct posix_acl *, umode_t *);
extern int posix_acl_create(struct posix_acl **, gfp_t, umode_t *);
extern int posix_acl_chmod(struct posix_acl **, gfp_t, umode_t);
extern int posix_acl_update_mode(struct inode *, umode_t *, struct posix_acl **);

extern struct posix_acl *get_posix_acl(struct inode *, int);
extern int set_posix_acl(struct inode *, int, struct posix_acl *);

#ifdef CONFIG_FS_POSIX_ACL
static inline struct posix_acl **acl_by_type(struct inode *inode, int type)
{
	switch (type) {
	case ACL_TYPE_ACCESS:
		return &inode->i_acl;
	case ACL_TYPE_DEFAULT:
		return &inode->i_default_acl;
	default:
		BUG();
	}
}

static inline struct posix_acl *get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *acl = ACCESS_ONCE(*p);
	if (acl) {
		spin_lock(&inode->i_lock);
		acl = *p;
		if (acl != ACL_NOT_CACHED)
			acl = posix_acl_dup(acl);
		spin_unlock(&inode->i_lock);
	}
	return acl;
}

static inline struct posix_acl *get_cached_acl_rcu(struct inode *inode, int type)
{
	return rcu_dereference(*acl_by_type(inode, type));
}

static inline void set_cached_acl(struct inode *inode,
				  int type,
				  struct posix_acl *acl)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	rcu_assign_pointer(*p, posix_acl_dup(acl));
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
}

static inline void forget_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	*p = ACL_NOT_CACHED;
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
=======
extern struct posix_acl *posix_acl_from_mode(umode_t, gfp_t);
extern int posix_acl_equiv_mode(const struct posix_acl *, umode_t *);
extern int __posix_acl_create(struct posix_acl **, gfp_t, umode_t *);
extern int __posix_acl_chmod(struct posix_acl **, gfp_t, umode_t);

extern struct posix_acl *get_posix_acl(struct inode *, int);
int set_posix_acl(struct mnt_idmap *, struct dentry *, int,
		  struct posix_acl *);

struct posix_acl *get_cached_acl_rcu(struct inode *inode, int type);
struct posix_acl *posix_acl_clone(const struct posix_acl *acl, gfp_t flags);

#ifdef CONFIG_FS_POSIX_ACL
int posix_acl_chmod(struct mnt_idmap *, struct dentry *, umode_t);
extern int posix_acl_create(struct inode *, umode_t *, struct posix_acl **,
		struct posix_acl **);
int posix_acl_update_mode(struct mnt_idmap *, struct inode *, umode_t *,
			  struct posix_acl **);

int simple_set_acl(struct mnt_idmap *, struct dentry *,
		   struct posix_acl *, int);
extern int simple_acl_create(struct inode *, struct inode *);

struct posix_acl *get_cached_acl(struct inode *inode, int type);
void set_cached_acl(struct inode *inode, int type, struct posix_acl *acl);
void forget_cached_acl(struct inode *inode, int type);
void forget_all_cached_acls(struct inode *inode);
int posix_acl_valid(struct user_namespace *, const struct posix_acl *);
int posix_acl_permission(struct mnt_idmap *, struct inode *,
			 const struct posix_acl *, int);

static inline void cache_no_acl(struct inode *inode)
{
	inode->i_acl = NULL;
	inode->i_default_acl = NULL;
}

int vfs_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		const char *acl_name, struct posix_acl *kacl);
struct posix_acl *vfs_get_acl(struct mnt_idmap *idmap,
			      struct dentry *dentry, const char *acl_name);
int vfs_remove_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		   const char *acl_name);
int posix_acl_listxattr(struct inode *inode, char **buffer,
			ssize_t *remaining_size);
#else
static inline int posix_acl_chmod(struct mnt_idmap *idmap,
				  struct dentry *dentry, umode_t mode)
{
	return 0;
}

#define simple_set_acl		NULL

static inline int simple_acl_create(struct inode *dir, struct inode *inode)
{
	return 0;
}
static inline void cache_no_acl(struct inode *inode)
{
}

static inline int posix_acl_create(struct inode *inode, umode_t *mode,
		struct posix_acl **default_acl, struct posix_acl **acl)
{
	*default_acl = *acl = NULL;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void forget_all_cached_acls(struct inode *inode)
{
<<<<<<< HEAD
	struct posix_acl *old_access, *old_default;
	spin_lock(&inode->i_lock);
	old_access = inode->i_acl;
	old_default = inode->i_default_acl;
	inode->i_acl = inode->i_default_acl = ACL_NOT_CACHED;
	spin_unlock(&inode->i_lock);
	if (old_access != ACL_NOT_CACHED)
		posix_acl_release(old_access);
	if (old_default != ACL_NOT_CACHED)
		posix_acl_release(old_default);
}
#endif

static inline void cache_no_acl(struct inode *inode)
{
#ifdef CONFIG_FS_POSIX_ACL
	inode->i_acl = NULL;
	inode->i_default_acl = NULL;
#endif
}

=======
}

static inline int vfs_set_acl(struct mnt_idmap *idmap,
			      struct dentry *dentry, const char *name,
			      struct posix_acl *acl)
{
	return -EOPNOTSUPP;
}

static inline struct posix_acl *vfs_get_acl(struct mnt_idmap *idmap,
					    struct dentry *dentry,
					    const char *acl_name)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int vfs_remove_acl(struct mnt_idmap *idmap,
				 struct dentry *dentry, const char *acl_name)
{
	return -EOPNOTSUPP;
}
static inline int posix_acl_listxattr(struct inode *inode, char **buffer,
				      ssize_t *remaining_size)
{
	return 0;
}
#endif /* CONFIG_FS_POSIX_ACL */

struct posix_acl *get_inode_acl(struct inode *inode, int type);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif  /* __LINUX_POSIX_ACL_H */
