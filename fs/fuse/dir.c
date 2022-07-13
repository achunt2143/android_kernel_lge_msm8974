/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2008  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/

#include "fuse_i.h"

#include <linux/pagemap.h>
#include <linux/file.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/slab.h>

#if BITS_PER_LONG >= 64
static inline void fuse_dentry_settime(struct dentry *entry, u64 time)
{
	entry->d_time = time;
}

static inline u64 fuse_dentry_time(struct dentry *entry)
{
	return entry->d_time;
}
#else
/*
 * On 32 bit archs store the high 32 bits of time in d_fsdata
 */
static void fuse_dentry_settime(struct dentry *entry, u64 time)
{
	entry->d_time = time;
	entry->d_fsdata = (void *) (unsigned long) (time >> 32);
}

static u64 fuse_dentry_time(struct dentry *entry)
{
	return (u64) entry->d_time +
		((u64) (unsigned long) entry->d_fsdata << 32);
}
#endif

/*
 * FUSE caches dentries and attributes with separate timeout.  The
 * time in jiffies until the dentry/attributes are valid is stored in
 * dentry->d_time and fuse_inode->i_time respectively.
=======
#include <linux/fs_context.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/xattr.h>
#include <linux/iversion.h>
#include <linux/posix_acl.h>
#include <linux/security.h>
#include <linux/types.h>
#include <linux/kernel.h>

static bool __read_mostly allow_sys_admin_access;
module_param(allow_sys_admin_access, bool, 0644);
MODULE_PARM_DESC(allow_sys_admin_access,
		 "Allow users with CAP_SYS_ADMIN in initial userns to bypass allow_other access check");

static void fuse_advise_use_readdirplus(struct inode *dir)
{
	struct fuse_inode *fi = get_fuse_inode(dir);

	set_bit(FUSE_I_ADVISE_RDPLUS, &fi->state);
}

#if BITS_PER_LONG >= 64
static inline void __fuse_dentry_settime(struct dentry *entry, u64 time)
{
	entry->d_fsdata = (void *) time;
}

static inline u64 fuse_dentry_time(const struct dentry *entry)
{
	return (u64)entry->d_fsdata;
}

#else
union fuse_dentry {
	u64 time;
	struct rcu_head rcu;
};

static inline void __fuse_dentry_settime(struct dentry *dentry, u64 time)
{
	((union fuse_dentry *) dentry->d_fsdata)->time = time;
}

static inline u64 fuse_dentry_time(const struct dentry *entry)
{
	return ((union fuse_dentry *) entry->d_fsdata)->time;
}
#endif

static void fuse_dentry_settime(struct dentry *dentry, u64 time)
{
	struct fuse_conn *fc = get_fuse_conn_super(dentry->d_sb);
	bool delete = !time && fc->delete_stale;
	/*
	 * Mess with DCACHE_OP_DELETE because dput() will be faster without it.
	 * Don't care about races, either way it's just an optimization
	 */
	if ((!delete && (dentry->d_flags & DCACHE_OP_DELETE)) ||
	    (delete && !(dentry->d_flags & DCACHE_OP_DELETE))) {
		spin_lock(&dentry->d_lock);
		if (!delete)
			dentry->d_flags &= ~DCACHE_OP_DELETE;
		else
			dentry->d_flags |= DCACHE_OP_DELETE;
		spin_unlock(&dentry->d_lock);
	}

	__fuse_dentry_settime(dentry, time);
}

/*
 * FUSE caches dentries and attributes with separate timeout.  The
 * time in jiffies until the dentry/attributes are valid is stored in
 * dentry->d_fsdata and fuse_inode->i_time respectively.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * Calculate the time in jiffies until a dentry/attributes are valid
 */
<<<<<<< HEAD
static u64 time_to_jiffies(unsigned long sec, unsigned long nsec)
{
	if (sec || nsec) {
		struct timespec ts = {sec, nsec};
		return get_jiffies_64() + timespec_to_jiffies(&ts);
=======
u64 fuse_time_to_jiffies(u64 sec, u32 nsec)
{
	if (sec || nsec) {
		struct timespec64 ts = {
			sec,
			min_t(u32, nsec, NSEC_PER_SEC - 1)
		};

		return get_jiffies_64() + timespec64_to_jiffies(&ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		return 0;
}

/*
 * Set dentry and possibly attribute timeouts from the lookup/mk*
 * replies
 */
<<<<<<< HEAD
static void fuse_change_entry_timeout(struct dentry *entry,
				      struct fuse_entry_out *o)
{
	fuse_dentry_settime(entry,
		time_to_jiffies(o->entry_valid, o->entry_valid_nsec));
}

static u64 attr_timeout(struct fuse_attr_out *o)
{
	return time_to_jiffies(o->attr_valid, o->attr_valid_nsec);
}

static u64 entry_attr_timeout(struct fuse_entry_out *o)
{
	return time_to_jiffies(o->attr_valid, o->attr_valid_nsec);
=======
void fuse_change_entry_timeout(struct dentry *entry, struct fuse_entry_out *o)
{
	fuse_dentry_settime(entry,
		fuse_time_to_jiffies(o->entry_valid, o->entry_valid_nsec));
}

void fuse_invalidate_attr_mask(struct inode *inode, u32 mask)
{
	set_mask_bits(&get_fuse_inode(inode)->inval_mask, 0, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Mark the attributes as stale, so that at the next call to
 * ->getattr() they will be fetched from userspace
 */
void fuse_invalidate_attr(struct inode *inode)
{
<<<<<<< HEAD
	get_fuse_inode(inode)->i_time = 0;
=======
	fuse_invalidate_attr_mask(inode, STATX_BASIC_STATS);
}

static void fuse_dir_changed(struct inode *dir)
{
	fuse_invalidate_attr(dir);
	inode_maybe_inc_iversion(dir, false);
}

/*
 * Mark the attributes as stale due to an atime change.  Avoid the invalidate if
 * atime is not used.
 */
void fuse_invalidate_atime(struct inode *inode)
{
	if (!IS_RDONLY(inode))
		fuse_invalidate_attr_mask(inode, STATX_ATIME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Just mark the entry as stale, so that a next attempt to look it up
 * will result in a new lookup call to userspace
 *
 * This is called when a dentry is about to become negative and the
 * timeout is unknown (unlink, rmdir, rename and in some cases
 * lookup)
 */
void fuse_invalidate_entry_cache(struct dentry *entry)
{
	fuse_dentry_settime(entry, 0);
}

/*
 * Same as fuse_invalidate_entry_cache(), but also try to remove the
 * dentry from the hash
 */
static void fuse_invalidate_entry(struct dentry *entry)
{
	d_invalidate(entry);
	fuse_invalidate_entry_cache(entry);
}

<<<<<<< HEAD
static void fuse_lookup_init(struct fuse_conn *fc, struct fuse_req *req,
			     u64 nodeid, struct qstr *name,
			     struct fuse_entry_out *outarg)
{
	memset(outarg, 0, sizeof(struct fuse_entry_out));
	req->in.h.opcode = FUSE_LOOKUP;
	req->in.h.nodeid = nodeid;
	req->in.numargs = 1;
	req->in.args[0].size = name->len + 1;
	req->in.args[0].value = name->name;
	req->out.numargs = 1;
	if (fc->minor < 9)
		req->out.args[0].size = FUSE_COMPAT_ENTRY_OUT_SIZE;
	else
		req->out.args[0].size = sizeof(struct fuse_entry_out);
	req->out.args[0].value = outarg;
}

u64 fuse_get_attr_version(struct fuse_conn *fc)
{
	u64 curr_version;

	/*
	 * The spin lock isn't actually needed on 64bit archs, but we
	 * don't yet care too much about such optimizations.
	 */
	spin_lock(&fc->lock);
	curr_version = fc->attr_version;
	spin_unlock(&fc->lock);

	return curr_version;
=======
static void fuse_lookup_init(struct fuse_conn *fc, struct fuse_args *args,
			     u64 nodeid, const struct qstr *name,
			     struct fuse_entry_out *outarg)
{
	memset(outarg, 0, sizeof(struct fuse_entry_out));
	args->opcode = FUSE_LOOKUP;
	args->nodeid = nodeid;
	args->in_numargs = 1;
	args->in_args[0].size = name->len + 1;
	args->in_args[0].value = name->name;
	args->out_numargs = 1;
	args->out_args[0].size = sizeof(struct fuse_entry_out);
	args->out_args[0].value = outarg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check whether the dentry is still valid
 *
 * If the entry validity timeout has expired and the dentry is
 * positive, try to redo the lookup.  If the lookup results in a
 * different inode, then let the VFS invalidate the dentry and redo
 * the lookup once more.  If the lookup results in the same inode,
 * then refresh the attributes, timeouts and mark the dentry valid.
 */
static int fuse_dentry_revalidate(struct dentry *entry, unsigned int flags)
{
	struct inode *inode;
<<<<<<< HEAD

	inode = ACCESS_ONCE(entry->d_inode);
	if (inode && is_bad_inode(inode))
		return 0;
	else if (fuse_dentry_time(entry) < get_jiffies_64()) {
		int err;
		struct fuse_entry_out outarg;
		struct fuse_conn *fc;
		struct fuse_req *req;
		struct fuse_forget_link *forget;
		struct dentry *parent;
=======
	struct dentry *parent;
	struct fuse_mount *fm;
	struct fuse_inode *fi;
	int ret;

	inode = d_inode_rcu(entry);
	if (inode && fuse_is_bad(inode))
		goto invalid;
	else if (time_before64(fuse_dentry_time(entry), get_jiffies_64()) ||
		 (flags & (LOOKUP_EXCL | LOOKUP_REVAL | LOOKUP_RENAME_TARGET))) {
		struct fuse_entry_out outarg;
		FUSE_ARGS(args);
		struct fuse_forget_link *forget;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u64 attr_version;

		/* For negative dentries, always do a fresh lookup */
		if (!inode)
<<<<<<< HEAD
			return 0;

		if (flags & LOOKUP_RCU)
			return -ECHILD;

		fc = get_fuse_conn(inode);
		req = fuse_get_req(fc);
		if (IS_ERR(req))
			return 0;

		forget = fuse_alloc_forget();
		if (!forget) {
			fuse_put_request(fc, req);
			return 0;
		}

		attr_version = fuse_get_attr_version(fc);

		parent = dget_parent(entry);
		fuse_lookup_init(fc, req, get_node_id(parent->d_inode),
				 &entry->d_name, &outarg);
		fuse_request_send(fc, req);
		dput(parent);
		err = req->out.h.error;
		fuse_put_request(fc, req);
		/* Zero nodeid is same as -ENOENT */
		if (!err && !outarg.nodeid)
			err = -ENOENT;
		if (!err) {
			struct fuse_inode *fi = get_fuse_inode(inode);
			if (outarg.nodeid != get_node_id(inode)) {
				fuse_queue_forget(fc, forget, outarg.nodeid, 1);
				return 0;
			}
			spin_lock(&fc->lock);
			fi->nlookup++;
			spin_unlock(&fc->lock);
		}
		kfree(forget);
		if (err || (outarg.attr.mode ^ inode->i_mode) & S_IFMT)
			return 0;

		fuse_change_attributes(inode, &outarg.attr,
				       entry_attr_timeout(&outarg),
				       attr_version);
		fuse_change_entry_timeout(entry, &outarg);
	}
	return 1;
}

/*
 * Get the canonical path. Since we must translate to a path, this must be done
 * in the context of the userspace daemon, however, the userspace daemon cannot
 * look up paths on its own. Instead, we handle the lookup as a special case
 * inside of the write request.
 */
static void fuse_dentry_canonical_path(const struct path *path, struct path *canonical_path) {
	struct inode *inode = path->dentry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	int err;
	char *path_name;

	req = fuse_get_req(fc);
	err = PTR_ERR(req);
	if (IS_ERR(req))
		goto default_path;

	path_name = (char*)__get_free_page(GFP_KERNEL);
	if (!path_name) {
		fuse_put_request(fc, req);
		goto default_path;
	}

	req->in.h.opcode = FUSE_CANONICAL_PATH;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 0;
	req->out.numargs = 1;
	req->out.args[0].size = PATH_MAX;
	req->out.args[0].value = path_name;
	req->canonical_path = canonical_path;
	req->out.argvar = 1;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	free_page((unsigned long)path_name);
	if (!err)
		return;
default_path:
	canonical_path->dentry = path->dentry;
	canonical_path->mnt = path->mnt;
	path_get(canonical_path);
}

static int invalid_nodeid(u64 nodeid)
{
	return !nodeid || nodeid == FUSE_ROOT_ID;
=======
			goto invalid;

		ret = -ECHILD;
		if (flags & LOOKUP_RCU)
			goto out;

		fm = get_fuse_mount(inode);

		forget = fuse_alloc_forget();
		ret = -ENOMEM;
		if (!forget)
			goto out;

		attr_version = fuse_get_attr_version(fm->fc);

		parent = dget_parent(entry);
		fuse_lookup_init(fm->fc, &args, get_node_id(d_inode(parent)),
				 &entry->d_name, &outarg);
		ret = fuse_simple_request(fm, &args);
		dput(parent);
		/* Zero nodeid is same as -ENOENT */
		if (!ret && !outarg.nodeid)
			ret = -ENOENT;
		if (!ret) {
			fi = get_fuse_inode(inode);
			if (outarg.nodeid != get_node_id(inode) ||
			    (bool) IS_AUTOMOUNT(inode) != (bool) (outarg.attr.flags & FUSE_ATTR_SUBMOUNT)) {
				fuse_queue_forget(fm->fc, forget,
						  outarg.nodeid, 1);
				goto invalid;
			}
			spin_lock(&fi->lock);
			fi->nlookup++;
			spin_unlock(&fi->lock);
		}
		kfree(forget);
		if (ret == -ENOMEM || ret == -EINTR)
			goto out;
		if (ret || fuse_invalid_attr(&outarg.attr) ||
		    fuse_stale_inode(inode, outarg.generation, &outarg.attr))
			goto invalid;

		forget_all_cached_acls(inode);
		fuse_change_attributes(inode, &outarg.attr, NULL,
				       ATTR_TIMEOUT(&outarg),
				       attr_version);
		fuse_change_entry_timeout(entry, &outarg);
	} else if (inode) {
		fi = get_fuse_inode(inode);
		if (flags & LOOKUP_RCU) {
			if (test_bit(FUSE_I_INIT_RDPLUS, &fi->state))
				return -ECHILD;
		} else if (test_and_clear_bit(FUSE_I_INIT_RDPLUS, &fi->state)) {
			parent = dget_parent(entry);
			fuse_advise_use_readdirplus(d_inode(parent));
			dput(parent);
		}
	}
	ret = 1;
out:
	return ret;

invalid:
	ret = 0;
	goto out;
}

#if BITS_PER_LONG < 64
static int fuse_dentry_init(struct dentry *dentry)
{
	dentry->d_fsdata = kzalloc(sizeof(union fuse_dentry),
				   GFP_KERNEL_ACCOUNT | __GFP_RECLAIMABLE);

	return dentry->d_fsdata ? 0 : -ENOMEM;
}
static void fuse_dentry_release(struct dentry *dentry)
{
	union fuse_dentry *fd = dentry->d_fsdata;

	kfree_rcu(fd, rcu);
}
#endif

static int fuse_dentry_delete(const struct dentry *dentry)
{
	return time_before64(fuse_dentry_time(dentry), get_jiffies_64());
}

/*
 * Create a fuse_mount object with a new superblock (with path->dentry
 * as the root), and return that mount so it can be auto-mounted on
 * @path.
 */
static struct vfsmount *fuse_dentry_automount(struct path *path)
{
	struct fs_context *fsc;
	struct vfsmount *mnt;
	struct fuse_inode *mp_fi = get_fuse_inode(d_inode(path->dentry));

	fsc = fs_context_for_submount(path->mnt->mnt_sb->s_type, path->dentry);
	if (IS_ERR(fsc))
		return ERR_CAST(fsc);

	/* Pass the FUSE inode of the mount for fuse_get_tree_submount() */
	fsc->fs_private = mp_fi;

	/* Create the submount */
	mnt = fc_mount(fsc);
	if (!IS_ERR(mnt))
		mntget(mnt);

	put_fs_context(fsc);
	return mnt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct dentry_operations fuse_dentry_operations = {
	.d_revalidate	= fuse_dentry_revalidate,
<<<<<<< HEAD
	.d_canonical_path = fuse_dentry_canonical_path,
=======
	.d_delete	= fuse_dentry_delete,
#if BITS_PER_LONG < 64
	.d_init		= fuse_dentry_init,
	.d_release	= fuse_dentry_release,
#endif
	.d_automount	= fuse_dentry_automount,
};

const struct dentry_operations fuse_root_dentry_operations = {
#if BITS_PER_LONG < 64
	.d_init		= fuse_dentry_init,
	.d_release	= fuse_dentry_release,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int fuse_valid_type(int m)
{
	return S_ISREG(m) || S_ISDIR(m) || S_ISLNK(m) || S_ISCHR(m) ||
		S_ISBLK(m) || S_ISFIFO(m) || S_ISSOCK(m);
}

<<<<<<< HEAD
/*
 * Add a directory inode to a dentry, ensuring that no other dentry
 * refers to this inode.  Called with fc->inst_mutex.
 */
static struct dentry *fuse_d_add_directory(struct dentry *entry,
					   struct inode *inode)
{
	struct dentry *alias = d_find_alias(inode);
	if (alias && !(alias->d_flags & DCACHE_DISCONNECTED)) {
		/* This tries to shrink the subtree below alias */
		fuse_invalidate_entry(alias);
		dput(alias);
		if (!list_empty(&inode->i_dentry))
			return ERR_PTR(-EBUSY);
	} else {
		dput(alias);
	}
	return d_splice_alias(inode, entry);
}

int fuse_lookup_name(struct super_block *sb, u64 nodeid, struct qstr *name,
		     struct fuse_entry_out *outarg, struct inode **inode)
{
	struct fuse_conn *fc = get_fuse_conn_super(sb);
	struct fuse_req *req;
=======
static bool fuse_valid_size(u64 size)
{
	return size <= LLONG_MAX;
}

bool fuse_invalid_attr(struct fuse_attr *attr)
{
	return !fuse_valid_type(attr->mode) || !fuse_valid_size(attr->size);
}

int fuse_lookup_name(struct super_block *sb, u64 nodeid, const struct qstr *name,
		     struct fuse_entry_out *outarg, struct inode **inode)
{
	struct fuse_mount *fm = get_fuse_mount_super(sb);
	FUSE_ARGS(args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct fuse_forget_link *forget;
	u64 attr_version;
	int err;

	*inode = NULL;
	err = -ENAMETOOLONG;
	if (name->len > FUSE_NAME_MAX)
		goto out;

<<<<<<< HEAD
	req = fuse_get_req(fc);
	err = PTR_ERR(req);
	if (IS_ERR(req))
		goto out;

	forget = fuse_alloc_forget();
	err = -ENOMEM;
	if (!forget) {
		fuse_put_request(fc, req);
		goto out;
	}

	attr_version = fuse_get_attr_version(fc);

	fuse_lookup_init(fc, req, nodeid, name, outarg);
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
=======

	forget = fuse_alloc_forget();
	err = -ENOMEM;
	if (!forget)
		goto out;

	attr_version = fuse_get_attr_version(fm->fc);

	fuse_lookup_init(fm->fc, &args, nodeid, name, outarg);
	err = fuse_simple_request(fm, &args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Zero nodeid is same as -ENOENT, but with valid timeout */
	if (err || !outarg->nodeid)
		goto out_put_forget;

	err = -EIO;
<<<<<<< HEAD
	if (!outarg->nodeid)
		goto out_put_forget;
	if (!fuse_valid_type(outarg->attr.mode))
		goto out_put_forget;

	*inode = fuse_iget(sb, outarg->nodeid, outarg->generation,
			   &outarg->attr, entry_attr_timeout(outarg),
			   attr_version);
	err = -ENOMEM;
	if (!*inode) {
		fuse_queue_forget(fc, forget, outarg->nodeid, 1);
=======
	if (fuse_invalid_attr(&outarg->attr))
		goto out_put_forget;
	if (outarg->nodeid == FUSE_ROOT_ID && outarg->generation != 0) {
		pr_warn_once("root generation should be zero\n");
		outarg->generation = 0;
	}

	*inode = fuse_iget(sb, outarg->nodeid, outarg->generation,
			   &outarg->attr, ATTR_TIMEOUT(outarg),
			   attr_version);
	err = -ENOMEM;
	if (!*inode) {
		fuse_queue_forget(fm->fc, forget, outarg->nodeid, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}
	err = 0;

 out_put_forget:
	kfree(forget);
 out:
	return err;
}

static struct dentry *fuse_lookup(struct inode *dir, struct dentry *entry,
				  unsigned int flags)
{
	int err;
	struct fuse_entry_out outarg;
	struct inode *inode;
	struct dentry *newent;
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(dir);
	bool outarg_valid = true;

	err = fuse_lookup_name(dir->i_sb, get_node_id(dir), &entry->d_name,
			       &outarg, &inode);
=======
	bool outarg_valid = true;
	bool locked;

	if (fuse_is_bad(dir))
		return ERR_PTR(-EIO);

	locked = fuse_lock_inode(dir);
	err = fuse_lookup_name(dir->i_sb, get_node_id(dir), &entry->d_name,
			       &outarg, &inode);
	fuse_unlock_inode(dir, locked);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err == -ENOENT) {
		outarg_valid = false;
		err = 0;
	}
	if (err)
		goto out_err;

	err = -EIO;
	if (inode && get_node_id(inode) == FUSE_ROOT_ID)
		goto out_iput;

<<<<<<< HEAD
	if (inode && S_ISDIR(inode->i_mode)) {
		mutex_lock(&fc->inst_mutex);
		newent = fuse_d_add_directory(entry, inode);
		mutex_unlock(&fc->inst_mutex);
		err = PTR_ERR(newent);
		if (IS_ERR(newent))
			goto out_iput;
	} else {
		newent = d_splice_alias(inode, entry);
	}
=======
	newent = d_splice_alias(inode, entry);
	err = PTR_ERR(newent);
	if (IS_ERR(newent))
		goto out_err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	entry = newent ? newent : entry;
	if (outarg_valid)
		fuse_change_entry_timeout(entry, &outarg);
	else
		fuse_invalidate_entry_cache(entry);

<<<<<<< HEAD
=======
	if (inode)
		fuse_advise_use_readdirplus(dir);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return newent;

 out_iput:
	iput(inode);
 out_err:
	return ERR_PTR(err);
}

<<<<<<< HEAD
=======
static int get_security_context(struct dentry *entry, umode_t mode,
				struct fuse_in_arg *ext)
{
	struct fuse_secctx *fctx;
	struct fuse_secctx_header *header;
	void *ctx = NULL, *ptr;
	u32 ctxlen, total_len = sizeof(*header);
	int err, nr_ctx = 0;
	const char *name;
	size_t namelen;

	err = security_dentry_init_security(entry, mode, &entry->d_name,
					    &name, &ctx, &ctxlen);
	if (err) {
		if (err != -EOPNOTSUPP)
			goto out_err;
		/* No LSM is supporting this security hook. Ignore error */
		ctxlen = 0;
		ctx = NULL;
	}

	if (ctxlen) {
		nr_ctx = 1;
		namelen = strlen(name) + 1;
		err = -EIO;
		if (WARN_ON(namelen > XATTR_NAME_MAX + 1 || ctxlen > S32_MAX))
			goto out_err;
		total_len += FUSE_REC_ALIGN(sizeof(*fctx) + namelen + ctxlen);
	}

	err = -ENOMEM;
	header = ptr = kzalloc(total_len, GFP_KERNEL);
	if (!ptr)
		goto out_err;

	header->nr_secctx = nr_ctx;
	header->size = total_len;
	ptr += sizeof(*header);
	if (nr_ctx) {
		fctx = ptr;
		fctx->size = ctxlen;
		ptr += sizeof(*fctx);

		strcpy(ptr, name);
		ptr += namelen;

		memcpy(ptr, ctx, ctxlen);
	}
	ext->size = total_len;
	ext->value = header;
	err = 0;
out_err:
	kfree(ctx);
	return err;
}

static void *extend_arg(struct fuse_in_arg *buf, u32 bytes)
{
	void *p;
	u32 newlen = buf->size + bytes;

	p = krealloc(buf->value, newlen, GFP_KERNEL);
	if (!p) {
		kfree(buf->value);
		buf->size = 0;
		buf->value = NULL;
		return NULL;
	}

	memset(p + buf->size, 0, bytes);
	buf->value = p;
	buf->size = newlen;

	return p + newlen - bytes;
}

static u32 fuse_ext_size(size_t size)
{
	return FUSE_REC_ALIGN(sizeof(struct fuse_ext_header) + size);
}

/*
 * This adds just a single supplementary group that matches the parent's group.
 */
static int get_create_supp_group(struct inode *dir, struct fuse_in_arg *ext)
{
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_ext_header *xh;
	struct fuse_supp_groups *sg;
	kgid_t kgid = dir->i_gid;
	gid_t parent_gid = from_kgid(fc->user_ns, kgid);
	u32 sg_len = fuse_ext_size(sizeof(*sg) + sizeof(sg->groups[0]));

	if (parent_gid == (gid_t) -1 || gid_eq(kgid, current_fsgid()) ||
	    !in_group_p(kgid))
		return 0;

	xh = extend_arg(ext, sg_len);
	if (!xh)
		return -ENOMEM;

	xh->size = sg_len;
	xh->type = FUSE_EXT_GROUPS;

	sg = (struct fuse_supp_groups *) &xh[1];
	sg->nr_groups = 1;
	sg->groups[0] = parent_gid;

	return 0;
}

static int get_create_ext(struct fuse_args *args,
			  struct inode *dir, struct dentry *dentry,
			  umode_t mode)
{
	struct fuse_conn *fc = get_fuse_conn_super(dentry->d_sb);
	struct fuse_in_arg ext = { .size = 0, .value = NULL };
	int err = 0;

	if (fc->init_security)
		err = get_security_context(dentry, mode, &ext);
	if (!err && fc->create_supp_group)
		err = get_create_supp_group(dir, &ext);

	if (!err && ext.size) {
		WARN_ON(args->in_numargs >= ARRAY_SIZE(args->in_args));
		args->is_ext = true;
		args->ext_idx = args->in_numargs++;
		args->in_args[args->ext_idx] = ext;
	} else {
		kfree(ext.value);
	}

	return err;
}

static void free_ext_value(struct fuse_args *args)
{
	if (args->is_ext)
		kfree(args->in_args[args->ext_idx].value);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Atomic create+open operation
 *
 * If the filesystem doesn't support this, then fall back to separate
 * 'mknod' + 'open' requests.
 */
static int fuse_create_open(struct inode *dir, struct dentry *entry,
<<<<<<< HEAD
			    struct file *file, unsigned flags,
			    umode_t mode, int *opened)
{
	int err;
	struct inode *inode;
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_req *req;
	struct fuse_forget_link *forget;
	struct fuse_create_in inarg;
	struct fuse_open_out outopen;
	struct fuse_entry_out outentry;
	struct fuse_file *ff;
=======
			    struct file *file, unsigned int flags,
			    umode_t mode, u32 opcode)
{
	int err;
	struct inode *inode;
	struct fuse_mount *fm = get_fuse_mount(dir);
	FUSE_ARGS(args);
	struct fuse_forget_link *forget;
	struct fuse_create_in inarg;
	struct fuse_open_out *outopenp;
	struct fuse_entry_out outentry;
	struct fuse_inode *fi;
	struct fuse_file *ff;
	bool trunc = flags & O_TRUNC;

	/* Userspace expects S_IFREG in create mode */
	BUG_ON((mode & S_IFMT) != S_IFREG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	forget = fuse_alloc_forget();
	err = -ENOMEM;
	if (!forget)
		goto out_err;

<<<<<<< HEAD
	req = fuse_get_req(fc);
	err = PTR_ERR(req);
	if (IS_ERR(req))
		goto out_put_forget_req;

	err = -ENOMEM;
	ff = fuse_file_alloc(fc);
	if (!ff)
		goto out_put_request;

	if (!fc->dont_mask)
=======
	err = -ENOMEM;
	ff = fuse_file_alloc(fm, true);
	if (!ff)
		goto out_put_forget_req;

	if (!fm->fc->dont_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode &= ~current_umask();

	flags &= ~O_NOCTTY;
	memset(&inarg, 0, sizeof(inarg));
	memset(&outentry, 0, sizeof(outentry));
	inarg.flags = flags;
	inarg.mode = mode;
	inarg.umask = current_umask();
<<<<<<< HEAD
	req->in.h.opcode = FUSE_CREATE;
	req->in.h.nodeid = get_node_id(dir);
	req->in.numargs = 2;
	req->in.args[0].size = fc->minor < 12 ? sizeof(struct fuse_open_in) :
						sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = entry->d_name.len + 1;
	req->in.args[1].value = entry->d_name.name;
	req->out.numargs = 2;
	if (fc->minor < 9)
		req->out.args[0].size = FUSE_COMPAT_ENTRY_OUT_SIZE;
	else
		req->out.args[0].size = sizeof(outentry);
	req->out.args[0].value = &outentry;
	req->out.args[1].size = sizeof(outopen);
	req->out.args[1].value = &outopen;
	fuse_request_send(fc, req);
	err = req->out.h.error;
=======

	if (fm->fc->handle_killpriv_v2 && trunc &&
	    !(flags & O_EXCL) && !capable(CAP_FSETID)) {
		inarg.open_flags |= FUSE_OPEN_KILL_SUIDGID;
	}

	args.opcode = opcode;
	args.nodeid = get_node_id(dir);
	args.in_numargs = 2;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.in_args[1].size = entry->d_name.len + 1;
	args.in_args[1].value = entry->d_name.name;
	args.out_numargs = 2;
	args.out_args[0].size = sizeof(outentry);
	args.out_args[0].value = &outentry;
	/* Store outarg for fuse_finish_open() */
	outopenp = &ff->args->open_outarg;
	args.out_args[1].size = sizeof(*outopenp);
	args.out_args[1].value = outopenp;

	err = get_create_ext(&args, dir, entry, mode);
	if (err)
		goto out_put_forget_req;

	err = fuse_simple_request(fm, &args);
	free_ext_value(&args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_free_ff;

	err = -EIO;
<<<<<<< HEAD
	if (!S_ISREG(outentry.attr.mode) || invalid_nodeid(outentry.nodeid))
		goto out_free_ff;

	fuse_put_request(fc, req);
	ff->fh = outopen.fh;
	ff->nodeid = outentry.nodeid;
	ff->open_flags = outopen.open_flags;
	inode = fuse_iget(dir->i_sb, outentry.nodeid, outentry.generation,
			  &outentry.attr, entry_attr_timeout(&outentry), 0);
	if (!inode) {
		flags &= ~(O_CREAT | O_EXCL | O_TRUNC);
		fuse_sync_release(ff, flags);
		fuse_queue_forget(fc, forget, outentry.nodeid, 1);
=======
	if (!S_ISREG(outentry.attr.mode) || invalid_nodeid(outentry.nodeid) ||
	    fuse_invalid_attr(&outentry.attr))
		goto out_free_ff;

	ff->fh = outopenp->fh;
	ff->nodeid = outentry.nodeid;
	ff->open_flags = outopenp->open_flags;
	inode = fuse_iget(dir->i_sb, outentry.nodeid, outentry.generation,
			  &outentry.attr, ATTR_TIMEOUT(&outentry), 0);
	if (!inode) {
		flags &= ~(O_CREAT | O_EXCL | O_TRUNC);
		fuse_sync_release(NULL, ff, flags);
		fuse_queue_forget(fm->fc, forget, outentry.nodeid, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOMEM;
		goto out_err;
	}
	kfree(forget);
	d_instantiate(entry, inode);
	fuse_change_entry_timeout(entry, &outentry);
<<<<<<< HEAD
	fuse_invalidate_attr(dir);
	err = finish_open(file, entry, generic_file_open, opened);
	if (err) {
		fuse_sync_release(ff, flags);
	} else {
		file->private_data = fuse_file_get(ff);
		fuse_finish_open(inode, file);
=======
	fuse_dir_changed(dir);
	err = generic_file_open(inode, file);
	if (!err) {
		file->private_data = ff;
		err = finish_open(file, entry, fuse_finish_open);
	}
	if (err) {
		fi = get_fuse_inode(inode);
		fuse_sync_release(fi, ff, flags);
	} else {
		if (fm->fc->atomic_o_trunc && trunc)
			truncate_pagecache(inode, 0);
		else if (!(ff->open_flags & FOPEN_KEEP_CACHE))
			invalidate_inode_pages2(inode->i_mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;

out_free_ff:
	fuse_file_free(ff);
<<<<<<< HEAD
out_put_request:
	fuse_put_request(fc, req);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_put_forget_req:
	kfree(forget);
out_err:
	return err;
}

<<<<<<< HEAD
static int fuse_mknod(struct inode *, struct dentry *, umode_t, dev_t);
static int fuse_atomic_open(struct inode *dir, struct dentry *entry,
			    struct file *file, unsigned flags,
			    umode_t mode, int *opened)
=======
static int fuse_mknod(struct mnt_idmap *, struct inode *, struct dentry *,
		      umode_t, dev_t);
static int fuse_atomic_open(struct inode *dir, struct dentry *entry,
			    struct file *file, unsigned flags,
			    umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct dentry *res = NULL;

<<<<<<< HEAD
	if (d_unhashed(entry)) {
=======
	if (fuse_is_bad(dir))
		return -EIO;

	if (d_in_lookup(entry)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		res = fuse_lookup(dir, entry, 0);
		if (IS_ERR(res))
			return PTR_ERR(res);

		if (res)
			entry = res;
	}

<<<<<<< HEAD
	if (!(flags & O_CREAT) || entry->d_inode)
		goto no_open;

	/* Only creates */
	*opened |= FILE_CREATED;
=======
	if (!(flags & O_CREAT) || d_really_is_positive(entry))
		goto no_open;

	/* Only creates */
	file->f_mode |= FMODE_CREATED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (fc->no_create)
		goto mknod;

<<<<<<< HEAD
	err = fuse_create_open(dir, entry, file, flags, mode, opened);
	if (err == -ENOSYS) {
		fc->no_create = 1;
		goto mknod;
	}
=======
	err = fuse_create_open(dir, entry, file, flags, mode, FUSE_CREATE);
	if (err == -ENOSYS) {
		fc->no_create = 1;
		goto mknod;
	} else if (err == -EEXIST)
		fuse_invalidate_entry(entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_dput:
	dput(res);
	return err;

mknod:
<<<<<<< HEAD
	err = fuse_mknod(dir, entry, mode, 0);
=======
	err = fuse_mknod(&nop_mnt_idmap, dir, entry, mode, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_dput;
no_open:
	return finish_no_open(file, res);
}

/*
 * Code shared between mknod, mkdir, symlink and link
 */
<<<<<<< HEAD
static int create_new_entry(struct fuse_conn *fc, struct fuse_req *req,
=======
static int create_new_entry(struct fuse_mount *fm, struct fuse_args *args,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    struct inode *dir, struct dentry *entry,
			    umode_t mode)
{
	struct fuse_entry_out outarg;
	struct inode *inode;
<<<<<<< HEAD
	int err;
	struct fuse_forget_link *forget;

	forget = fuse_alloc_forget();
	if (!forget) {
		fuse_put_request(fc, req);
		return -ENOMEM;
	}

	memset(&outarg, 0, sizeof(outarg));
	req->in.h.nodeid = get_node_id(dir);
	req->out.numargs = 1;
	if (fc->minor < 9)
		req->out.args[0].size = FUSE_COMPAT_ENTRY_OUT_SIZE;
	else
		req->out.args[0].size = sizeof(outarg);
	req->out.args[0].value = &outarg;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
=======
	struct dentry *d;
	int err;
	struct fuse_forget_link *forget;

	if (fuse_is_bad(dir))
		return -EIO;

	forget = fuse_alloc_forget();
	if (!forget)
		return -ENOMEM;

	memset(&outarg, 0, sizeof(outarg));
	args->nodeid = get_node_id(dir);
	args->out_numargs = 1;
	args->out_args[0].size = sizeof(outarg);
	args->out_args[0].value = &outarg;

	if (args->opcode != FUSE_LINK) {
		err = get_create_ext(args, dir, entry, mode);
		if (err)
			goto out_put_forget_req;
	}

	err = fuse_simple_request(fm, args);
	free_ext_value(args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out_put_forget_req;

	err = -EIO;
<<<<<<< HEAD
	if (invalid_nodeid(outarg.nodeid))
=======
	if (invalid_nodeid(outarg.nodeid) || fuse_invalid_attr(&outarg.attr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_put_forget_req;

	if ((outarg.attr.mode ^ mode) & S_IFMT)
		goto out_put_forget_req;

	inode = fuse_iget(dir->i_sb, outarg.nodeid, outarg.generation,
<<<<<<< HEAD
			  &outarg.attr, entry_attr_timeout(&outarg), 0);
	if (!inode) {
		fuse_queue_forget(fc, forget, outarg.nodeid, 1);
=======
			  &outarg.attr, ATTR_TIMEOUT(&outarg), 0);
	if (!inode) {
		fuse_queue_forget(fm->fc, forget, outarg.nodeid, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	}
	kfree(forget);

<<<<<<< HEAD
	if (S_ISDIR(inode->i_mode)) {
		struct dentry *alias;
		mutex_lock(&fc->inst_mutex);
		alias = d_find_alias(inode);
		if (alias) {
			/* New directory must have moved since mkdir */
			mutex_unlock(&fc->inst_mutex);
			dput(alias);
			iput(inode);
			return -EBUSY;
		}
		d_instantiate(entry, inode);
		mutex_unlock(&fc->inst_mutex);
	} else
		d_instantiate(entry, inode);

	fuse_change_entry_timeout(entry, &outarg);
	fuse_invalidate_attr(dir);
	return 0;

 out_put_forget_req:
=======
	d_drop(entry);
	d = d_splice_alias(inode, entry);
	if (IS_ERR(d))
		return PTR_ERR(d);

	if (d) {
		fuse_change_entry_timeout(d, &outarg);
		dput(d);
	} else {
		fuse_change_entry_timeout(entry, &outarg);
	}
	fuse_dir_changed(dir);
	return 0;

 out_put_forget_req:
	if (err == -EEXIST)
		fuse_invalidate_entry(entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(forget);
	return err;
}

<<<<<<< HEAD
static int fuse_mknod(struct inode *dir, struct dentry *entry, umode_t mode,
		      dev_t rdev)
{
	struct fuse_mknod_in inarg;
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	if (!fc->dont_mask)
=======
static int fuse_mknod(struct mnt_idmap *idmap, struct inode *dir,
		      struct dentry *entry, umode_t mode, dev_t rdev)
{
	struct fuse_mknod_in inarg;
	struct fuse_mount *fm = get_fuse_mount(dir);
	FUSE_ARGS(args);

	if (!fm->fc->dont_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode &= ~current_umask();

	memset(&inarg, 0, sizeof(inarg));
	inarg.mode = mode;
	inarg.rdev = new_encode_dev(rdev);
	inarg.umask = current_umask();
<<<<<<< HEAD
	req->in.h.opcode = FUSE_MKNOD;
	req->in.numargs = 2;
	req->in.args[0].size = fc->minor < 12 ? FUSE_COMPAT_MKNOD_IN_SIZE :
						sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = entry->d_name.len + 1;
	req->in.args[1].value = entry->d_name.name;
	return create_new_entry(fc, req, dir, entry, mode);
}

static int fuse_create(struct inode *dir, struct dentry *entry, umode_t mode,
		       bool excl)
{
	return fuse_mknod(dir, entry, mode, 0);
}

static int fuse_mkdir(struct inode *dir, struct dentry *entry, umode_t mode)
{
	struct fuse_mkdir_in inarg;
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	if (!fc->dont_mask)
=======
	args.opcode = FUSE_MKNOD;
	args.in_numargs = 2;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.in_args[1].size = entry->d_name.len + 1;
	args.in_args[1].value = entry->d_name.name;
	return create_new_entry(fm, &args, dir, entry, mode);
}

static int fuse_create(struct mnt_idmap *idmap, struct inode *dir,
		       struct dentry *entry, umode_t mode, bool excl)
{
	return fuse_mknod(&nop_mnt_idmap, dir, entry, mode, 0);
}

static int fuse_tmpfile(struct mnt_idmap *idmap, struct inode *dir,
			struct file *file, umode_t mode)
{
	struct fuse_conn *fc = get_fuse_conn(dir);
	int err;

	if (fc->no_tmpfile)
		return -EOPNOTSUPP;

	err = fuse_create_open(dir, file->f_path.dentry, file, file->f_flags, mode, FUSE_TMPFILE);
	if (err == -ENOSYS) {
		fc->no_tmpfile = 1;
		err = -EOPNOTSUPP;
	}
	return err;
}

static int fuse_mkdir(struct mnt_idmap *idmap, struct inode *dir,
		      struct dentry *entry, umode_t mode)
{
	struct fuse_mkdir_in inarg;
	struct fuse_mount *fm = get_fuse_mount(dir);
	FUSE_ARGS(args);

	if (!fm->fc->dont_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode &= ~current_umask();

	memset(&inarg, 0, sizeof(inarg));
	inarg.mode = mode;
	inarg.umask = current_umask();
<<<<<<< HEAD
	req->in.h.opcode = FUSE_MKDIR;
	req->in.numargs = 2;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = entry->d_name.len + 1;
	req->in.args[1].value = entry->d_name.name;
	return create_new_entry(fc, req, dir, entry, S_IFDIR);
}

static int fuse_symlink(struct inode *dir, struct dentry *entry,
			const char *link)
{
	struct fuse_conn *fc = get_fuse_conn(dir);
	unsigned len = strlen(link) + 1;
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	req->in.h.opcode = FUSE_SYMLINK;
	req->in.numargs = 2;
	req->in.args[0].size = entry->d_name.len + 1;
	req->in.args[0].value = entry->d_name.name;
	req->in.args[1].size = len;
	req->in.args[1].value = link;
	return create_new_entry(fc, req, dir, entry, S_IFLNK);
=======
	args.opcode = FUSE_MKDIR;
	args.in_numargs = 2;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.in_args[1].size = entry->d_name.len + 1;
	args.in_args[1].value = entry->d_name.name;
	return create_new_entry(fm, &args, dir, entry, S_IFDIR);
}

static int fuse_symlink(struct mnt_idmap *idmap, struct inode *dir,
			struct dentry *entry, const char *link)
{
	struct fuse_mount *fm = get_fuse_mount(dir);
	unsigned len = strlen(link) + 1;
	FUSE_ARGS(args);

	args.opcode = FUSE_SYMLINK;
	args.in_numargs = 2;
	args.in_args[0].size = entry->d_name.len + 1;
	args.in_args[0].value = entry->d_name.name;
	args.in_args[1].size = len;
	args.in_args[1].value = link;
	return create_new_entry(fm, &args, dir, entry, S_IFLNK);
}

void fuse_flush_time_update(struct inode *inode)
{
	int err = sync_inode_metadata(inode, 1);

	mapping_set_error(inode->i_mapping, err);
}

static void fuse_update_ctime_in_cache(struct inode *inode)
{
	if (!IS_NOCMTIME(inode)) {
		inode_set_ctime_current(inode);
		mark_inode_dirty_sync(inode);
		fuse_flush_time_update(inode);
	}
}

void fuse_update_ctime(struct inode *inode)
{
	fuse_invalidate_attr_mask(inode, STATX_CTIME);
	fuse_update_ctime_in_cache(inode);
}

static void fuse_entry_unlinked(struct dentry *entry)
{
	struct inode *inode = d_inode(entry);
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);

	spin_lock(&fi->lock);
	fi->attr_version = atomic64_inc_return(&fc->attr_version);
	/*
	 * If i_nlink == 0 then unlink doesn't make sense, yet this can
	 * happen if userspace filesystem is careless.  It would be
	 * difficult to enforce correct nlink usage so just ignore this
	 * condition here
	 */
	if (S_ISDIR(inode->i_mode))
		clear_nlink(inode);
	else if (inode->i_nlink > 0)
		drop_nlink(inode);
	spin_unlock(&fi->lock);
	fuse_invalidate_entry_cache(entry);
	fuse_update_ctime(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fuse_unlink(struct inode *dir, struct dentry *entry)
{
	int err;
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	req->in.h.opcode = FUSE_UNLINK;
	req->in.h.nodeid = get_node_id(dir);
	req->in.numargs = 1;
	req->in.args[0].size = entry->d_name.len + 1;
	req->in.args[0].value = entry->d_name.name;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (!err) {
		struct inode *inode = entry->d_inode;
		struct fuse_inode *fi = get_fuse_inode(inode);

		spin_lock(&fc->lock);
		fi->attr_version = ++fc->attr_version;
		/*
		 * If i_nlink == 0 then unlink doesn't make sense, yet this can
		 * happen if userspace filesystem is careless.  It would be
		 * difficult to enforce correct nlink usage so just ignore this
		 * condition here
		 */
		if (inode->i_nlink > 0)
			drop_nlink(inode);
		spin_unlock(&fc->lock);
		fuse_invalidate_attr(inode);
		fuse_invalidate_attr(dir);
		fuse_invalidate_entry_cache(entry);
	} else if (err == -EINTR)
=======
	struct fuse_mount *fm = get_fuse_mount(dir);
	FUSE_ARGS(args);

	if (fuse_is_bad(dir))
		return -EIO;

	args.opcode = FUSE_UNLINK;
	args.nodeid = get_node_id(dir);
	args.in_numargs = 1;
	args.in_args[0].size = entry->d_name.len + 1;
	args.in_args[0].value = entry->d_name.name;
	err = fuse_simple_request(fm, &args);
	if (!err) {
		fuse_dir_changed(dir);
		fuse_entry_unlinked(entry);
	} else if (err == -EINTR || err == -ENOENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fuse_invalidate_entry(entry);
	return err;
}

static int fuse_rmdir(struct inode *dir, struct dentry *entry)
{
	int err;
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(dir);
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	req->in.h.opcode = FUSE_RMDIR;
	req->in.h.nodeid = get_node_id(dir);
	req->in.numargs = 1;
	req->in.args[0].size = entry->d_name.len + 1;
	req->in.args[0].value = entry->d_name.name;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (!err) {
		clear_nlink(entry->d_inode);
		fuse_invalidate_attr(dir);
		fuse_invalidate_entry_cache(entry);
	} else if (err == -EINTR)
=======
	struct fuse_mount *fm = get_fuse_mount(dir);
	FUSE_ARGS(args);

	if (fuse_is_bad(dir))
		return -EIO;

	args.opcode = FUSE_RMDIR;
	args.nodeid = get_node_id(dir);
	args.in_numargs = 1;
	args.in_args[0].size = entry->d_name.len + 1;
	args.in_args[0].value = entry->d_name.name;
	err = fuse_simple_request(fm, &args);
	if (!err) {
		fuse_dir_changed(dir);
		fuse_entry_unlinked(entry);
	} else if (err == -EINTR || err == -ENOENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fuse_invalidate_entry(entry);
	return err;
}

<<<<<<< HEAD
static int fuse_rename(struct inode *olddir, struct dentry *oldent,
		       struct inode *newdir, struct dentry *newent)
{
	int err;
	struct fuse_rename_in inarg;
	struct fuse_conn *fc = get_fuse_conn(olddir);
	struct fuse_req *req = fuse_get_req(fc);

	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.newdir = get_node_id(newdir);
	req->in.h.opcode = FUSE_RENAME;
	req->in.h.nodeid = get_node_id(olddir);
	req->in.numargs = 3;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = oldent->d_name.len + 1;
	req->in.args[1].value = oldent->d_name.name;
	req->in.args[2].size = newent->d_name.len + 1;
	req->in.args[2].value = newent->d_name.name;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (!err) {
		/* ctime changes */
		fuse_invalidate_attr(oldent->d_inode);

		fuse_invalidate_attr(olddir);
		if (olddir != newdir)
			fuse_invalidate_attr(newdir);

		/* newent will end up negative */
		if (newent->d_inode) {
			fuse_invalidate_attr(newent->d_inode);
			fuse_invalidate_entry_cache(newent);
		}
	} else if (err == -EINTR) {
=======
static int fuse_rename_common(struct inode *olddir, struct dentry *oldent,
			      struct inode *newdir, struct dentry *newent,
			      unsigned int flags, int opcode, size_t argsize)
{
	int err;
	struct fuse_rename2_in inarg;
	struct fuse_mount *fm = get_fuse_mount(olddir);
	FUSE_ARGS(args);

	memset(&inarg, 0, argsize);
	inarg.newdir = get_node_id(newdir);
	inarg.flags = flags;
	args.opcode = opcode;
	args.nodeid = get_node_id(olddir);
	args.in_numargs = 3;
	args.in_args[0].size = argsize;
	args.in_args[0].value = &inarg;
	args.in_args[1].size = oldent->d_name.len + 1;
	args.in_args[1].value = oldent->d_name.name;
	args.in_args[2].size = newent->d_name.len + 1;
	args.in_args[2].value = newent->d_name.name;
	err = fuse_simple_request(fm, &args);
	if (!err) {
		/* ctime changes */
		fuse_update_ctime(d_inode(oldent));

		if (flags & RENAME_EXCHANGE)
			fuse_update_ctime(d_inode(newent));

		fuse_dir_changed(olddir);
		if (olddir != newdir)
			fuse_dir_changed(newdir);

		/* newent will end up negative */
		if (!(flags & RENAME_EXCHANGE) && d_really_is_positive(newent))
			fuse_entry_unlinked(newent);
	} else if (err == -EINTR || err == -ENOENT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* If request was interrupted, DEITY only knows if the
		   rename actually took place.  If the invalidation
		   fails (e.g. some process has CWD under the renamed
		   directory), then there can be inconsistency between
		   the dcache and the real filesystem.  Tough luck. */
		fuse_invalidate_entry(oldent);
<<<<<<< HEAD
		if (newent->d_inode)
=======
		if (d_really_is_positive(newent))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			fuse_invalidate_entry(newent);
	}

	return err;
}

<<<<<<< HEAD
=======
static int fuse_rename2(struct mnt_idmap *idmap, struct inode *olddir,
			struct dentry *oldent, struct inode *newdir,
			struct dentry *newent, unsigned int flags)
{
	struct fuse_conn *fc = get_fuse_conn(olddir);
	int err;

	if (fuse_is_bad(olddir))
		return -EIO;

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE | RENAME_WHITEOUT))
		return -EINVAL;

	if (flags) {
		if (fc->no_rename2 || fc->minor < 23)
			return -EINVAL;

		err = fuse_rename_common(olddir, oldent, newdir, newent, flags,
					 FUSE_RENAME2,
					 sizeof(struct fuse_rename2_in));
		if (err == -ENOSYS) {
			fc->no_rename2 = 1;
			err = -EINVAL;
		}
	} else {
		err = fuse_rename_common(olddir, oldent, newdir, newent, 0,
					 FUSE_RENAME,
					 sizeof(struct fuse_rename_in));
	}

	return err;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int fuse_link(struct dentry *entry, struct inode *newdir,
		     struct dentry *newent)
{
	int err;
	struct fuse_link_in inarg;
<<<<<<< HEAD
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.oldnodeid = get_node_id(inode);
	req->in.h.opcode = FUSE_LINK;
	req->in.numargs = 2;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = newent->d_name.len + 1;
	req->in.args[1].value = newent->d_name.name;
	err = create_new_entry(fc, req, newdir, newent, inode->i_mode);
	/* Contrary to "normal" filesystems it can happen that link
	   makes two "logical" inodes point to the same "physical"
	   inode.  We invalidate the attributes of the old one, so it
	   will reflect changes in the backing inode (link count,
	   etc.)
	*/
	if (!err) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		spin_lock(&fc->lock);
		fi->attr_version = ++fc->attr_version;
		inc_nlink(inode);
		spin_unlock(&fc->lock);
		fuse_invalidate_attr(inode);
	} else if (err == -EINTR) {
		fuse_invalidate_attr(inode);
	}
=======
	struct inode *inode = d_inode(entry);
	struct fuse_mount *fm = get_fuse_mount(inode);
	FUSE_ARGS(args);

	memset(&inarg, 0, sizeof(inarg));
	inarg.oldnodeid = get_node_id(inode);
	args.opcode = FUSE_LINK;
	args.in_numargs = 2;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.in_args[1].size = newent->d_name.len + 1;
	args.in_args[1].value = newent->d_name.name;
	err = create_new_entry(fm, &args, newdir, newent, inode->i_mode);
	if (!err)
		fuse_update_ctime_in_cache(inode);
	else if (err == -EINTR)
		fuse_invalidate_attr(inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void fuse_fillattr(struct inode *inode, struct fuse_attr *attr,
			  struct kstat *stat)
{
<<<<<<< HEAD
=======
	unsigned int blkbits;
	struct fuse_conn *fc = get_fuse_conn(inode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stat->dev = inode->i_sb->s_dev;
	stat->ino = attr->ino;
	stat->mode = (inode->i_mode & S_IFMT) | (attr->mode & 07777);
	stat->nlink = attr->nlink;
<<<<<<< HEAD
	stat->uid = attr->uid;
	stat->gid = attr->gid;
=======
	stat->uid = make_kuid(fc->user_ns, attr->uid);
	stat->gid = make_kgid(fc->user_ns, attr->gid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stat->rdev = inode->i_rdev;
	stat->atime.tv_sec = attr->atime;
	stat->atime.tv_nsec = attr->atimensec;
	stat->mtime.tv_sec = attr->mtime;
	stat->mtime.tv_nsec = attr->mtimensec;
	stat->ctime.tv_sec = attr->ctime;
	stat->ctime.tv_nsec = attr->ctimensec;
	stat->size = attr->size;
	stat->blocks = attr->blocks;
<<<<<<< HEAD
	stat->blksize = (1 << inode->i_blkbits);
}

static int fuse_do_getattr(struct inode *inode, struct kstat *stat,
			   struct file *file)
{
	int err;
	struct fuse_getattr_in inarg;
	struct fuse_attr_out outarg;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	u64 attr_version;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	attr_version = fuse_get_attr_version(fc);
=======

	if (attr->blksize != 0)
		blkbits = ilog2(attr->blksize);
	else
		blkbits = inode->i_sb->s_blocksize_bits;

	stat->blksize = 1 << blkbits;
}

static void fuse_statx_to_attr(struct fuse_statx *sx, struct fuse_attr *attr)
{
	memset(attr, 0, sizeof(*attr));
	attr->ino = sx->ino;
	attr->size = sx->size;
	attr->blocks = sx->blocks;
	attr->atime = sx->atime.tv_sec;
	attr->mtime = sx->mtime.tv_sec;
	attr->ctime = sx->ctime.tv_sec;
	attr->atimensec = sx->atime.tv_nsec;
	attr->mtimensec = sx->mtime.tv_nsec;
	attr->ctimensec = sx->ctime.tv_nsec;
	attr->mode = sx->mode;
	attr->nlink = sx->nlink;
	attr->uid = sx->uid;
	attr->gid = sx->gid;
	attr->rdev = new_encode_dev(MKDEV(sx->rdev_major, sx->rdev_minor));
	attr->blksize = sx->blksize;
}

static int fuse_do_statx(struct inode *inode, struct file *file,
			 struct kstat *stat)
{
	int err;
	struct fuse_attr attr;
	struct fuse_statx *sx;
	struct fuse_statx_in inarg;
	struct fuse_statx_out outarg;
	struct fuse_mount *fm = get_fuse_mount(inode);
	u64 attr_version = fuse_get_attr_version(fm->fc);
	FUSE_ARGS(args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memset(&inarg, 0, sizeof(inarg));
	memset(&outarg, 0, sizeof(outarg));
	/* Directories have separate file-handle space */
	if (file && S_ISREG(inode->i_mode)) {
		struct fuse_file *ff = file->private_data;

		inarg.getattr_flags |= FUSE_GETATTR_FH;
		inarg.fh = ff->fh;
	}
<<<<<<< HEAD
	req->in.h.opcode = FUSE_GETATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->out.numargs = 1;
	if (fc->minor < 9)
		req->out.args[0].size = FUSE_COMPAT_ATTR_OUT_SIZE;
	else
		req->out.args[0].size = sizeof(outarg);
	req->out.args[0].value = &outarg;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (!err) {
		if ((inode->i_mode ^ outarg.attr.mode) & S_IFMT) {
			make_bad_inode(inode);
			err = -EIO;
		} else {
			fuse_change_attributes(inode, &outarg.attr,
					       attr_timeout(&outarg),
=======
	/* For now leave sync hints as the default, request all stats. */
	inarg.sx_flags = 0;
	inarg.sx_mask = STATX_BASIC_STATS | STATX_BTIME;
	args.opcode = FUSE_STATX;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (err)
		return err;

	sx = &outarg.stat;
	if (((sx->mask & STATX_SIZE) && !fuse_valid_size(sx->size)) ||
	    ((sx->mask & STATX_TYPE) && (!fuse_valid_type(sx->mode) ||
					 inode_wrong_type(inode, sx->mode)))) {
		fuse_make_bad(inode);
		return -EIO;
	}

	fuse_statx_to_attr(&outarg.stat, &attr);
	if ((sx->mask & STATX_BASIC_STATS) == STATX_BASIC_STATS) {
		fuse_change_attributes(inode, &attr, &outarg.stat,
				       ATTR_TIMEOUT(&outarg), attr_version);
	}

	if (stat) {
		stat->result_mask = sx->mask & (STATX_BASIC_STATS | STATX_BTIME);
		stat->btime.tv_sec = sx->btime.tv_sec;
		stat->btime.tv_nsec = min_t(u32, sx->btime.tv_nsec, NSEC_PER_SEC - 1);
		fuse_fillattr(inode, &attr, stat);
		stat->result_mask |= STATX_TYPE;
	}

	return 0;
}

static int fuse_do_getattr(struct inode *inode, struct kstat *stat,
			   struct file *file)
{
	int err;
	struct fuse_getattr_in inarg;
	struct fuse_attr_out outarg;
	struct fuse_mount *fm = get_fuse_mount(inode);
	FUSE_ARGS(args);
	u64 attr_version;

	attr_version = fuse_get_attr_version(fm->fc);

	memset(&inarg, 0, sizeof(inarg));
	memset(&outarg, 0, sizeof(outarg));
	/* Directories have separate file-handle space */
	if (file && S_ISREG(inode->i_mode)) {
		struct fuse_file *ff = file->private_data;

		inarg.getattr_flags |= FUSE_GETATTR_FH;
		inarg.fh = ff->fh;
	}
	args.opcode = FUSE_GETATTR;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	args.out_numargs = 1;
	args.out_args[0].size = sizeof(outarg);
	args.out_args[0].value = &outarg;
	err = fuse_simple_request(fm, &args);
	if (!err) {
		if (fuse_invalid_attr(&outarg.attr) ||
		    inode_wrong_type(inode, outarg.attr.mode)) {
			fuse_make_bad(inode);
			err = -EIO;
		} else {
			fuse_change_attributes(inode, &outarg.attr, NULL,
					       ATTR_TIMEOUT(&outarg),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					       attr_version);
			if (stat)
				fuse_fillattr(inode, &outarg.attr, stat);
		}
	}
	return err;
}

<<<<<<< HEAD
int fuse_update_attributes(struct inode *inode, struct kstat *stat,
			   struct file *file, bool *refreshed)
{
	struct fuse_inode *fi = get_fuse_inode(inode);
	int err;
	bool r;

	if (fi->i_time < get_jiffies_64()) {
		r = true;
		err = fuse_do_getattr(inode, stat, file);
	} else {
		r = false;
		err = 0;
		if (stat) {
			generic_fillattr(inode, stat);
			stat->mode = fi->orig_i_mode;
			stat->ino = fi->orig_ino;
		}
	}

	if (refreshed != NULL)
		*refreshed = r;

	return err;
}

int fuse_reverse_inval_entry(struct super_block *sb, u64 parent_nodeid,
			     u64 child_nodeid, struct qstr *name)
=======
static int fuse_update_get_attr(struct inode *inode, struct file *file,
				struct kstat *stat, u32 request_mask,
				unsigned int flags)
{
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_conn *fc = get_fuse_conn(inode);
	int err = 0;
	bool sync;
	u32 inval_mask = READ_ONCE(fi->inval_mask);
	u32 cache_mask = fuse_get_cache_mask(inode);


	/* FUSE only supports basic stats and possibly btime */
	request_mask &= STATX_BASIC_STATS | STATX_BTIME;
retry:
	if (fc->no_statx)
		request_mask &= STATX_BASIC_STATS;

	if (!request_mask)
		sync = false;
	else if (flags & AT_STATX_FORCE_SYNC)
		sync = true;
	else if (flags & AT_STATX_DONT_SYNC)
		sync = false;
	else if (request_mask & inval_mask & ~cache_mask)
		sync = true;
	else
		sync = time_before64(fi->i_time, get_jiffies_64());

	if (sync) {
		forget_all_cached_acls(inode);
		/* Try statx if BTIME is requested */
		if (!fc->no_statx && (request_mask & ~STATX_BASIC_STATS)) {
			err = fuse_do_statx(inode, file, stat);
			if (err == -ENOSYS) {
				fc->no_statx = 1;
				err = 0;
				goto retry;
			}
		} else {
			err = fuse_do_getattr(inode, stat, file);
		}
	} else if (stat) {
		generic_fillattr(&nop_mnt_idmap, request_mask, inode, stat);
		stat->mode = fi->orig_i_mode;
		stat->ino = fi->orig_ino;
		if (test_bit(FUSE_I_BTIME, &fi->state)) {
			stat->btime = fi->i_btime;
			stat->result_mask |= STATX_BTIME;
		}
	}

	return err;
}

int fuse_update_attributes(struct inode *inode, struct file *file, u32 mask)
{
	return fuse_update_get_attr(inode, file, NULL, mask, 0);
}

int fuse_reverse_inval_entry(struct fuse_conn *fc, u64 parent_nodeid,
			     u64 child_nodeid, struct qstr *name, u32 flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = -ENOTDIR;
	struct inode *parent;
	struct dentry *dir;
	struct dentry *entry;

<<<<<<< HEAD
	parent = ilookup5(sb, parent_nodeid, fuse_inode_eq, &parent_nodeid);
	if (!parent)
		return -ENOENT;

	mutex_lock(&parent->i_mutex);
=======
	parent = fuse_ilookup(fc, parent_nodeid, NULL);
	if (!parent)
		return -ENOENT;

	inode_lock_nested(parent, I_MUTEX_PARENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!S_ISDIR(parent->i_mode))
		goto unlock;

	err = -ENOENT;
	dir = d_find_alias(parent);
	if (!dir)
		goto unlock;

<<<<<<< HEAD
=======
	name->hash = full_name_hash(dir, name->name, name->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	entry = d_lookup(dir, name);
	dput(dir);
	if (!entry)
		goto unlock;

<<<<<<< HEAD
	fuse_invalidate_attr(parent);
	fuse_invalidate_entry(entry);

	if (child_nodeid != 0 && entry->d_inode) {
		mutex_lock(&entry->d_inode->i_mutex);
		if (get_node_id(entry->d_inode) != child_nodeid) {
=======
	fuse_dir_changed(parent);
	if (!(flags & FUSE_EXPIRE_ONLY))
		d_invalidate(entry);
	fuse_invalidate_entry_cache(entry);

	if (child_nodeid != 0 && d_really_is_positive(entry)) {
		inode_lock(d_inode(entry));
		if (get_node_id(d_inode(entry)) != child_nodeid) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENOENT;
			goto badentry;
		}
		if (d_mountpoint(entry)) {
			err = -EBUSY;
			goto badentry;
		}
<<<<<<< HEAD
		if (S_ISDIR(entry->d_inode->i_mode)) {
=======
		if (d_is_dir(entry)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			shrink_dcache_parent(entry);
			if (!simple_empty(entry)) {
				err = -ENOTEMPTY;
				goto badentry;
			}
<<<<<<< HEAD
			entry->d_inode->i_flags |= S_DEAD;
		}
		dont_mount(entry);
		clear_nlink(entry->d_inode);
		err = 0;
 badentry:
		mutex_unlock(&entry->d_inode->i_mutex);
=======
			d_inode(entry)->i_flags |= S_DEAD;
		}
		dont_mount(entry);
		clear_nlink(d_inode(entry));
		err = 0;
 badentry:
		inode_unlock(d_inode(entry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!err)
			d_delete(entry);
	} else {
		err = 0;
	}
	dput(entry);

 unlock:
<<<<<<< HEAD
	mutex_unlock(&parent->i_mutex);
=======
	inode_unlock(parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iput(parent);
	return err;
}

<<<<<<< HEAD
/*
 * Calling into a user-controlled filesystem gives the filesystem
 * daemon ptrace-like capabilities over the requester process.  This
=======
static inline bool fuse_permissible_uidgid(struct fuse_conn *fc)
{
	const struct cred *cred = current_cred();

	return (uid_eq(cred->euid, fc->user_id) &&
		uid_eq(cred->suid, fc->user_id) &&
		uid_eq(cred->uid,  fc->user_id) &&
		gid_eq(cred->egid, fc->group_id) &&
		gid_eq(cred->sgid, fc->group_id) &&
		gid_eq(cred->gid,  fc->group_id));
}

/*
 * Calling into a user-controlled filesystem gives the filesystem
 * daemon ptrace-like capabilities over the current process.  This
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * means, that the filesystem daemon is able to record the exact
 * filesystem operations performed, and can also control the behavior
 * of the requester process in otherwise impossible ways.  For example
 * it can delay the operation for arbitrary length of time allowing
 * DoS against the requester.
 *
 * For this reason only those processes can call into the filesystem,
 * for which the owner of the mount has ptrace privilege.  This
 * excludes processes started by other users, suid or sgid processes.
 */
<<<<<<< HEAD
int fuse_allow_task(struct fuse_conn *fc, struct task_struct *task)
{
	const struct cred *cred;
	int ret;

	if (fc->flags & FUSE_ALLOW_OTHER)
		return 1;

	rcu_read_lock();
	ret = 0;
	cred = __task_cred(task);
	if (cred->euid == fc->user_id &&
	    cred->suid == fc->user_id &&
	    cred->uid  == fc->user_id &&
	    cred->egid == fc->group_id &&
	    cred->sgid == fc->group_id &&
	    cred->gid  == fc->group_id)
		ret = 1;
	rcu_read_unlock();

	return ret;
=======
bool fuse_allow_current_process(struct fuse_conn *fc)
{
	bool allow;

	if (fc->allow_other)
		allow = current_in_userns(fc->user_ns);
	else
		allow = fuse_permissible_uidgid(fc);

	if (!allow && allow_sys_admin_access && capable(CAP_SYS_ADMIN))
		allow = true;

	return allow;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fuse_access(struct inode *inode, int mask)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	struct fuse_access_in inarg;
	int err;

	if (fc->no_access)
		return 0;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.mask = mask & (MAY_READ | MAY_WRITE | MAY_EXEC);
	req->in.h.opcode = FUSE_ACCESS;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (err == -ENOSYS) {
		fc->no_access = 1;
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
	FUSE_ARGS(args);
	struct fuse_access_in inarg;
	int err;

	BUG_ON(mask & MAY_NOT_BLOCK);

	if (fm->fc->no_access)
		return 0;

	memset(&inarg, 0, sizeof(inarg));
	inarg.mask = mask & (MAY_READ | MAY_WRITE | MAY_EXEC);
	args.opcode = FUSE_ACCESS;
	args.nodeid = get_node_id(inode);
	args.in_numargs = 1;
	args.in_args[0].size = sizeof(inarg);
	args.in_args[0].value = &inarg;
	err = fuse_simple_request(fm, &args);
	if (err == -ENOSYS) {
		fm->fc->no_access = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 0;
	}
	return err;
}

static int fuse_perm_getattr(struct inode *inode, int mask)
{
	if (mask & MAY_NOT_BLOCK)
		return -ECHILD;

<<<<<<< HEAD
=======
	forget_all_cached_acls(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return fuse_do_getattr(inode, NULL, NULL);
}

/*
 * Check permission.  The two basic access models of FUSE are:
 *
 * 1) Local access checking ('default_permissions' mount option) based
 * on file mode.  This is the plain old disk filesystem permission
<<<<<<< HEAD
 * modell.
=======
 * model.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * 2) "Remote" access checking, where server is responsible for
 * checking permission in each inode operation.  An exception to this
 * is if ->permission() was invoked from sys_access() in which case an
 * access request is sent.  Execute permission is still checked
 * locally based on file mode.
 */
<<<<<<< HEAD
static int fuse_permission(struct inode *inode, int mask)
=======
static int fuse_permission(struct mnt_idmap *idmap,
			   struct inode *inode, int mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct fuse_conn *fc = get_fuse_conn(inode);
	bool refreshed = false;
	int err = 0;

<<<<<<< HEAD
	if (!fuse_allow_task(fc, current))
=======
	if (fuse_is_bad(inode))
		return -EIO;

	if (!fuse_allow_current_process(fc))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EACCES;

	/*
	 * If attributes are needed, refresh them before proceeding
	 */
<<<<<<< HEAD
	if ((fc->flags & FUSE_DEFAULT_PERMISSIONS) ||
	    ((mask & MAY_EXEC) && S_ISREG(inode->i_mode))) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		if (fi->i_time < get_jiffies_64()) {
=======
	if (fc->default_permissions ||
	    ((mask & MAY_EXEC) && S_ISREG(inode->i_mode))) {
		struct fuse_inode *fi = get_fuse_inode(inode);
		u32 perm_mask = STATX_MODE | STATX_UID | STATX_GID;

		if (perm_mask & READ_ONCE(fi->inval_mask) ||
		    time_before64(fi->i_time, get_jiffies_64())) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			refreshed = true;

			err = fuse_perm_getattr(inode, mask);
			if (err)
				return err;
		}
	}

<<<<<<< HEAD
	if (fc->flags & FUSE_DEFAULT_PERMISSIONS) {
		err = generic_permission(inode, mask);
=======
	if (fc->default_permissions) {
		err = generic_permission(&nop_mnt_idmap, inode, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* If permission is denied, try to refresh file
		   attributes.  This is also needed, because the root
		   node will at first have no permissions */
		if (err == -EACCES && !refreshed) {
			err = fuse_perm_getattr(inode, mask);
			if (!err)
<<<<<<< HEAD
				err = generic_permission(inode, mask);
=======
				err = generic_permission(&nop_mnt_idmap,
							 inode, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		/* Note: the opposite of the above test does not
		   exist.  So if permissions are revoked this won't be
		   noticed immediately, only after the attribute
		   timeout has expired */
	} else if (mask & (MAY_ACCESS | MAY_CHDIR)) {
<<<<<<< HEAD
		if (mask & MAY_NOT_BLOCK)
			return -ECHILD;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = fuse_access(inode, mask);
	} else if ((mask & MAY_EXEC) && S_ISREG(inode->i_mode)) {
		if (!(inode->i_mode & S_IXUGO)) {
			if (refreshed)
				return -EACCES;

			err = fuse_perm_getattr(inode, mask);
			if (!err && !(inode->i_mode & S_IXUGO))
				return -EACCES;
		}
	}
	return err;
}

<<<<<<< HEAD
static int parse_dirfile(char *buf, size_t nbytes, struct file *file,
			 void *dstbuf, filldir_t filldir)
{
	while (nbytes >= FUSE_NAME_OFFSET) {
		struct fuse_dirent *dirent = (struct fuse_dirent *) buf;
		size_t reclen = FUSE_DIRENT_SIZE(dirent);
		int over;
		if (!dirent->namelen || dirent->namelen > FUSE_NAME_MAX)
			return -EIO;
		if (reclen > nbytes)
			break;
		if (memchr(dirent->name, '/', dirent->namelen) != NULL)
			return -EIO;

		over = filldir(dstbuf, dirent->name, dirent->namelen,
			       file->f_pos, dirent->ino, dirent->type);
		if (over)
			break;

		buf += reclen;
		nbytes -= reclen;
		file->f_pos = dirent->off;
	}
=======
static int fuse_readlink_page(struct inode *inode, struct page *page)
{
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_page_desc desc = { .length = PAGE_SIZE - 1 };
	struct fuse_args_pages ap = {
		.num_pages = 1,
		.pages = &page,
		.descs = &desc,
	};
	char *link;
	ssize_t res;

	ap.args.opcode = FUSE_READLINK;
	ap.args.nodeid = get_node_id(inode);
	ap.args.out_pages = true;
	ap.args.out_argvar = true;
	ap.args.page_zeroing = true;
	ap.args.out_numargs = 1;
	ap.args.out_args[0].size = desc.length;
	res = fuse_simple_request(fm, &ap.args);

	fuse_invalidate_atime(inode);

	if (res < 0)
		return res;

	if (WARN_ON(res >= PAGE_SIZE))
		return -EIO;

	link = page_address(page);
	link[res] = '\0';
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int fuse_readdir(struct file *file, void *dstbuf, filldir_t filldir)
{
	int err;
	size_t nbytes;
	struct page *page;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;

	if (is_bad_inode(inode))
		return -EIO;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	page = alloc_page(GFP_KERNEL);
	if (!page) {
		fuse_put_request(fc, req);
		return -ENOMEM;
	}
	req->out.argpages = 1;
	req->num_pages = 1;
	req->pages[0] = page;
	fuse_read_fill(req, file, file->f_pos, PAGE_SIZE, FUSE_READDIR);
	fuse_request_send(fc, req);
	nbytes = req->out.args[0].size;
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (!err)
		err = parse_dirfile(page_address(page), nbytes, file, dstbuf,
				    filldir);

	__free_page(page);
	fuse_invalidate_attr(inode); /* atime changed */
	return err;
}

static char *read_link(struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req = fuse_get_req(fc);
	char *link;

	if (IS_ERR(req))
		return ERR_CAST(req);

	link = (char *) __get_free_page(GFP_KERNEL);
	if (!link) {
		link = ERR_PTR(-ENOMEM);
		goto out;
	}
	req->in.h.opcode = FUSE_READLINK;
	req->in.h.nodeid = get_node_id(inode);
	req->out.argvar = 1;
	req->out.numargs = 1;
	req->out.args[0].size = PAGE_SIZE - 1;
	req->out.args[0].value = link;
	fuse_request_send(fc, req);
	if (req->out.h.error) {
		free_page((unsigned long) link);
		link = ERR_PTR(req->out.h.error);
	} else
		link[req->out.args[0].size] = '\0';
 out:
	fuse_put_request(fc, req);
	fuse_invalidate_attr(inode); /* atime changed */
	return link;
}

static void free_link(char *link)
{
	if (!IS_ERR(link))
		free_page((unsigned long) link);
}

static void *fuse_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	nd_set_link(nd, read_link(dentry));
	return NULL;
}

static void fuse_put_link(struct dentry *dentry, struct nameidata *nd, void *c)
{
	free_link(nd_get_link(nd));
=======
static const char *fuse_get_link(struct dentry *dentry, struct inode *inode,
				 struct delayed_call *callback)
{
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct page *page;
	int err;

	err = -EIO;
	if (fuse_is_bad(inode))
		goto out_err;

	if (fc->cache_symlinks)
		return page_get_link(dentry, inode, callback);

	err = -ECHILD;
	if (!dentry)
		goto out_err;

	page = alloc_page(GFP_KERNEL);
	err = -ENOMEM;
	if (!page)
		goto out_err;

	err = fuse_readlink_page(inode, page);
	if (err) {
		__free_page(page);
		goto out_err;
	}

	set_delayed_call(callback, page_put_link, page);

	return page_address(page);

out_err:
	return ERR_PTR(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fuse_dir_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	return fuse_open_common(inode, file, true);
=======
	struct fuse_mount *fm = get_fuse_mount(inode);
	int err;

	if (fuse_is_bad(inode))
		return -EIO;

	err = generic_file_open(inode, file);
	if (err)
		return err;

	err = fuse_do_open(fm, get_node_id(inode), file, true);
	if (!err) {
		struct fuse_file *ff = file->private_data;

		/*
		 * Keep handling FOPEN_STREAM and FOPEN_NONSEEKABLE for
		 * directories for backward compatibility, though it's unlikely
		 * to be useful.
		 */
		if (ff->open_flags & (FOPEN_STREAM | FOPEN_NONSEEKABLE))
			nonseekable_open(inode, file);
	}

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fuse_dir_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	fuse_release_common(file, FUSE_RELEASEDIR);
=======
	fuse_release_common(file, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int fuse_dir_fsync(struct file *file, loff_t start, loff_t end,
			  int datasync)
{
<<<<<<< HEAD
	return fuse_fsync_common(file, start, end, datasync, 1);
=======
	struct inode *inode = file->f_mapping->host;
	struct fuse_conn *fc = get_fuse_conn(inode);
	int err;

	if (fuse_is_bad(inode))
		return -EIO;

	if (fc->no_fsyncdir)
		return 0;

	inode_lock(inode);
	err = fuse_fsync_common(file, start, end, datasync, FUSE_FSYNCDIR);
	if (err == -ENOSYS) {
		fc->no_fsyncdir = 1;
		err = 0;
	}
	inode_unlock(inode);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static long fuse_dir_ioctl(struct file *file, unsigned int cmd,
			    unsigned long arg)
{
	struct fuse_conn *fc = get_fuse_conn(file->f_mapping->host);

	/* FUSE_IOCTL_DIR only supported for API version >= 7.18 */
	if (fc->minor < 18)
		return -ENOTTY;

	return fuse_ioctl_common(file, cmd, arg, FUSE_IOCTL_DIR);
}

static long fuse_dir_compat_ioctl(struct file *file, unsigned int cmd,
				   unsigned long arg)
{
	struct fuse_conn *fc = get_fuse_conn(file->f_mapping->host);

	if (fc->minor < 18)
		return -ENOTTY;

	return fuse_ioctl_common(file, cmd, arg,
				 FUSE_IOCTL_COMPAT | FUSE_IOCTL_DIR);
}

<<<<<<< HEAD
static bool update_mtime(unsigned ivalid)
=======
static bool update_mtime(unsigned ivalid, bool trust_local_mtime)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Always update if mtime is explicitly set  */
	if (ivalid & ATTR_MTIME_SET)
		return true;

<<<<<<< HEAD
=======
	/* Or if kernel i_mtime is the official one */
	if (trust_local_mtime)
		return true;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If it's an open(O_TRUNC) or an ftruncate(), don't update */
	if ((ivalid & ATTR_SIZE) && (ivalid & (ATTR_OPEN | ATTR_FILE)))
		return false;

	/* In all other cases update */
	return true;
}

<<<<<<< HEAD
static void iattr_to_fattr(struct iattr *iattr, struct fuse_setattr_in *arg)
=======
static void iattr_to_fattr(struct fuse_conn *fc, struct iattr *iattr,
			   struct fuse_setattr_in *arg, bool trust_local_cmtime)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned ivalid = iattr->ia_valid;

	if (ivalid & ATTR_MODE)
		arg->valid |= FATTR_MODE,   arg->mode = iattr->ia_mode;
	if (ivalid & ATTR_UID)
<<<<<<< HEAD
		arg->valid |= FATTR_UID,    arg->uid = iattr->ia_uid;
	if (ivalid & ATTR_GID)
		arg->valid |= FATTR_GID,    arg->gid = iattr->ia_gid;
=======
		arg->valid |= FATTR_UID,    arg->uid = from_kuid(fc->user_ns, iattr->ia_uid);
	if (ivalid & ATTR_GID)
		arg->valid |= FATTR_GID,    arg->gid = from_kgid(fc->user_ns, iattr->ia_gid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ivalid & ATTR_SIZE)
		arg->valid |= FATTR_SIZE,   arg->size = iattr->ia_size;
	if (ivalid & ATTR_ATIME) {
		arg->valid |= FATTR_ATIME;
		arg->atime = iattr->ia_atime.tv_sec;
		arg->atimensec = iattr->ia_atime.tv_nsec;
		if (!(ivalid & ATTR_ATIME_SET))
			arg->valid |= FATTR_ATIME_NOW;
	}
<<<<<<< HEAD
	if ((ivalid & ATTR_MTIME) && update_mtime(ivalid)) {
		arg->valid |= FATTR_MTIME;
		arg->mtime = iattr->ia_mtime.tv_sec;
		arg->mtimensec = iattr->ia_mtime.tv_nsec;
		if (!(ivalid & ATTR_MTIME_SET))
			arg->valid |= FATTR_MTIME_NOW;
	}
=======
	if ((ivalid & ATTR_MTIME) && update_mtime(ivalid, trust_local_cmtime)) {
		arg->valid |= FATTR_MTIME;
		arg->mtime = iattr->ia_mtime.tv_sec;
		arg->mtimensec = iattr->ia_mtime.tv_nsec;
		if (!(ivalid & ATTR_MTIME_SET) && !trust_local_cmtime)
			arg->valid |= FATTR_MTIME_NOW;
	}
	if ((ivalid & ATTR_CTIME) && trust_local_cmtime) {
		arg->valid |= FATTR_CTIME;
		arg->ctime = iattr->ia_ctime.tv_sec;
		arg->ctimensec = iattr->ia_ctime.tv_nsec;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Prevent concurrent writepages on inode
 *
 * This is done by adding a negative bias to the inode write counter
 * and waiting for all pending writes to finish.
 */
void fuse_set_nowrite(struct inode *inode)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);

	BUG_ON(!mutex_is_locked(&inode->i_mutex));

	spin_lock(&fc->lock);
	BUG_ON(fi->writectr < 0);
	fi->writectr += FUSE_NOWRITE;
	spin_unlock(&fc->lock);
=======
	struct fuse_inode *fi = get_fuse_inode(inode);

	BUG_ON(!inode_is_locked(inode));

	spin_lock(&fi->lock);
	BUG_ON(fi->writectr < 0);
	fi->writectr += FUSE_NOWRITE;
	spin_unlock(&fi->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wait_event(fi->page_waitq, fi->writectr == FUSE_NOWRITE);
}

/*
 * Allow writepages on inode
 *
 * Remove the bias from the writecounter and send any queued
 * writepages.
 */
static void __fuse_release_nowrite(struct inode *inode)
{
	struct fuse_inode *fi = get_fuse_inode(inode);

	BUG_ON(fi->writectr != FUSE_NOWRITE);
	fi->writectr = 0;
	fuse_flush_writepages(inode);
}

void fuse_release_nowrite(struct inode *inode)
{
<<<<<<< HEAD
	struct fuse_conn *fc = get_fuse_conn(inode);

	spin_lock(&fc->lock);
	__fuse_release_nowrite(inode);
	spin_unlock(&fc->lock);
=======
	struct fuse_inode *fi = get_fuse_inode(inode);

	spin_lock(&fi->lock);
	__fuse_release_nowrite(inode);
	spin_unlock(&fi->lock);
}

static void fuse_setattr_fill(struct fuse_conn *fc, struct fuse_args *args,
			      struct inode *inode,
			      struct fuse_setattr_in *inarg_p,
			      struct fuse_attr_out *outarg_p)
{
	args->opcode = FUSE_SETATTR;
	args->nodeid = get_node_id(inode);
	args->in_numargs = 1;
	args->in_args[0].size = sizeof(*inarg_p);
	args->in_args[0].value = inarg_p;
	args->out_numargs = 1;
	args->out_args[0].size = sizeof(*outarg_p);
	args->out_args[0].value = outarg_p;
}

/*
 * Flush inode->i_mtime to the server
 */
int fuse_flush_times(struct inode *inode, struct fuse_file *ff)
{
	struct fuse_mount *fm = get_fuse_mount(inode);
	FUSE_ARGS(args);
	struct fuse_setattr_in inarg;
	struct fuse_attr_out outarg;

	memset(&inarg, 0, sizeof(inarg));
	memset(&outarg, 0, sizeof(outarg));

	inarg.valid = FATTR_MTIME;
	inarg.mtime = inode_get_mtime_sec(inode);
	inarg.mtimensec = inode_get_mtime_nsec(inode);
	if (fm->fc->minor >= 23) {
		inarg.valid |= FATTR_CTIME;
		inarg.ctime = inode_get_ctime_sec(inode);
		inarg.ctimensec = inode_get_ctime_nsec(inode);
	}
	if (ff) {
		inarg.valid |= FATTR_FH;
		inarg.fh = ff->fh;
	}
	fuse_setattr_fill(fm->fc, &args, inode, &inarg, &outarg);

	return fuse_simple_request(fm, &args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set attributes, and at the same time refresh them.
 *
 * Truncation is slightly complicated, because the 'truncate' request
 * may fail, in which case we don't want to touch the mapping.
 * vmtruncate() doesn't allow for this case, so do the rlimit checking
 * and the actual truncation by hand.
 */
<<<<<<< HEAD
static int fuse_do_setattr(struct dentry *entry, struct iattr *attr,
			   struct file *file)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct fuse_req *req;
	struct fuse_setattr_in inarg;
	struct fuse_attr_out outarg;
	bool is_truncate = false;
	loff_t oldsize;
	int err;

	if (!fuse_allow_task(fc, current))
		return -EACCES;

	if (!(fc->flags & FUSE_DEFAULT_PERMISSIONS))
		attr->ia_valid |= ATTR_FORCE;

	err = inode_change_ok(inode, attr);
	if (err)
		return err;

	if (attr->ia_valid & ATTR_OPEN) {
		if (fc->atomic_o_trunc)
			return 0;
		file = NULL;
	}

	if (attr->ia_valid & ATTR_SIZE)
		is_truncate = true;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);
=======
int fuse_do_setattr(struct dentry *dentry, struct iattr *attr,
		    struct file *file)
{
	struct inode *inode = d_inode(dentry);
	struct fuse_mount *fm = get_fuse_mount(inode);
	struct fuse_conn *fc = fm->fc;
	struct fuse_inode *fi = get_fuse_inode(inode);
	struct address_space *mapping = inode->i_mapping;
	FUSE_ARGS(args);
	struct fuse_setattr_in inarg;
	struct fuse_attr_out outarg;
	bool is_truncate = false;
	bool is_wb = fc->writeback_cache && S_ISREG(inode->i_mode);
	loff_t oldsize;
	int err;
	bool trust_local_cmtime = is_wb;
	bool fault_blocked = false;

	if (!fc->default_permissions)
		attr->ia_valid |= ATTR_FORCE;

	err = setattr_prepare(&nop_mnt_idmap, dentry, attr);
	if (err)
		return err;

	if (attr->ia_valid & ATTR_SIZE) {
		if (WARN_ON(!S_ISREG(inode->i_mode)))
			return -EIO;
		is_truncate = true;
	}

	if (FUSE_IS_DAX(inode) && is_truncate) {
		filemap_invalidate_lock(mapping);
		fault_blocked = true;
		err = fuse_dax_break_layouts(inode, 0, 0);
		if (err) {
			filemap_invalidate_unlock(mapping);
			return err;
		}
	}

	if (attr->ia_valid & ATTR_OPEN) {
		/* This is coming from open(..., ... | O_TRUNC); */
		WARN_ON(!(attr->ia_valid & ATTR_SIZE));
		WARN_ON(attr->ia_size != 0);
		if (fc->atomic_o_trunc) {
			/*
			 * No need to send request to userspace, since actual
			 * truncation has already been done by OPEN.  But still
			 * need to truncate page cache.
			 */
			i_size_write(inode, 0);
			truncate_pagecache(inode, 0);
			goto out;
		}
		file = NULL;
	}

	/* Flush dirty data/metadata before non-truncate SETATTR */
	if (is_wb &&
	    attr->ia_valid &
			(ATTR_MODE | ATTR_UID | ATTR_GID | ATTR_MTIME_SET |
			 ATTR_TIMES_SET)) {
		err = write_inode_now(inode, true);
		if (err)
			return err;

		fuse_set_nowrite(inode);
		fuse_release_nowrite(inode);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (is_truncate) {
		fuse_set_nowrite(inode);
		set_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);
<<<<<<< HEAD
=======
		if (trust_local_cmtime && attr->ia_size != inode->i_size)
			attr->ia_valid |= ATTR_MTIME | ATTR_CTIME;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	memset(&inarg, 0, sizeof(inarg));
	memset(&outarg, 0, sizeof(outarg));
<<<<<<< HEAD
	iattr_to_fattr(attr, &inarg);
=======
	iattr_to_fattr(fc, attr, &inarg, trust_local_cmtime);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (file) {
		struct fuse_file *ff = file->private_data;
		inarg.valid |= FATTR_FH;
		inarg.fh = ff->fh;
	}
<<<<<<< HEAD
=======

	/* Kill suid/sgid for non-directory chown unconditionally */
	if (fc->handle_killpriv_v2 && !S_ISDIR(inode->i_mode) &&
	    attr->ia_valid & (ATTR_UID | ATTR_GID))
		inarg.valid |= FATTR_KILL_SUIDGID;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (attr->ia_valid & ATTR_SIZE) {
		/* For mandatory locking in truncate */
		inarg.valid |= FATTR_LOCKOWNER;
		inarg.lock_owner = fuse_lock_owner_id(fc, current->files);
<<<<<<< HEAD
	}
	req->in.h.opcode = FUSE_SETATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->out.numargs = 1;
	if (fc->minor < 9)
		req->out.args[0].size = FUSE_COMPAT_ATTR_OUT_SIZE;
	else
		req->out.args[0].size = sizeof(outarg);
	req->out.args[0].value = &outarg;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
=======

		/* Kill suid/sgid for truncate only if no CAP_FSETID */
		if (fc->handle_killpriv_v2 && !capable(CAP_FSETID))
			inarg.valid |= FATTR_KILL_SUIDGID;
	}
	fuse_setattr_fill(fc, &args, inode, &inarg, &outarg);
	err = fuse_simple_request(fm, &args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		if (err == -EINTR)
			fuse_invalidate_attr(inode);
		goto error;
	}

<<<<<<< HEAD
	if ((inode->i_mode ^ outarg.attr.mode) & S_IFMT) {
		make_bad_inode(inode);
=======
	if (fuse_invalid_attr(&outarg.attr) ||
	    inode_wrong_type(inode, outarg.attr.mode)) {
		fuse_make_bad(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EIO;
		goto error;
	}

<<<<<<< HEAD
	spin_lock(&fc->lock);
	fuse_change_attributes_common(inode, &outarg.attr,
				      attr_timeout(&outarg));
	oldsize = inode->i_size;
	i_size_write(inode, outarg.attr.size);

	if (is_truncate) {
		/* NOTE: this may release/reacquire fc->lock */
		__fuse_release_nowrite(inode);
	}
	spin_unlock(&fc->lock);

	/*
	 * Only call invalidate_inode_pages2() after removing
	 * FUSE_NOWRITE, otherwise fuse_launder_page() would deadlock.
	 */
	if (S_ISREG(inode->i_mode) && oldsize != outarg.attr.size) {
		truncate_pagecache(inode, oldsize, outarg.attr.size);
		invalidate_inode_pages2(inode->i_mapping);
	}

	clear_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);
=======
	spin_lock(&fi->lock);
	/* the kernel maintains i_mtime locally */
	if (trust_local_cmtime) {
		if (attr->ia_valid & ATTR_MTIME)
			inode_set_mtime_to_ts(inode, attr->ia_mtime);
		if (attr->ia_valid & ATTR_CTIME)
			inode_set_ctime_to_ts(inode, attr->ia_ctime);
		/* FIXME: clear I_DIRTY_SYNC? */
	}

	fuse_change_attributes_common(inode, &outarg.attr, NULL,
				      ATTR_TIMEOUT(&outarg),
				      fuse_get_cache_mask(inode));
	oldsize = inode->i_size;
	/* see the comment in fuse_change_attributes() */
	if (!is_wb || is_truncate)
		i_size_write(inode, outarg.attr.size);

	if (is_truncate) {
		/* NOTE: this may release/reacquire fi->lock */
		__fuse_release_nowrite(inode);
	}
	spin_unlock(&fi->lock);

	/*
	 * Only call invalidate_inode_pages2() after removing
	 * FUSE_NOWRITE, otherwise fuse_launder_folio() would deadlock.
	 */
	if ((is_truncate || !is_wb) &&
	    S_ISREG(inode->i_mode) && oldsize != outarg.attr.size) {
		truncate_pagecache(inode, outarg.attr.size);
		invalidate_inode_pages2(mapping);
	}

	clear_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);
out:
	if (fault_blocked)
		filemap_invalidate_unlock(mapping);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

error:
	if (is_truncate)
		fuse_release_nowrite(inode);

	clear_bit(FUSE_I_SIZE_UNSTABLE, &fi->state);
<<<<<<< HEAD
	return err;
}

static int fuse_setattr(struct dentry *entry, struct iattr *attr)
{
	if (attr->ia_valid & ATTR_FILE)
		return fuse_do_setattr(entry, attr, attr->ia_file);
	else
		return fuse_do_setattr(entry, attr, NULL);
}

static int fuse_getattr(struct vfsmount *mnt, struct dentry *entry,
			struct kstat *stat)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);

	if (!fuse_allow_task(fc, current))
		return -EACCES;

	return fuse_update_attributes(inode, stat, NULL, NULL);
}

static int fuse_setxattr(struct dentry *entry, const char *name,
			 const void *value, size_t size, int flags)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	struct fuse_setxattr_in inarg;
	int err;

	if (fc->no_setxattr)
		return -EOPNOTSUPP;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.size = size;
	inarg.flags = flags;
	req->in.h.opcode = FUSE_SETXATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 3;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = strlen(name) + 1;
	req->in.args[1].value = name;
	req->in.args[2].size = size;
	req->in.args[2].value = value;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (err == -ENOSYS) {
		fc->no_setxattr = 1;
		err = -EOPNOTSUPP;
	}
	if (!err)
		fuse_invalidate_attr(inode);
	return err;
}

static ssize_t fuse_getxattr(struct dentry *entry, const char *name,
			     void *value, size_t size)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	struct fuse_getxattr_in inarg;
	struct fuse_getxattr_out outarg;
	ssize_t ret;

	if (fc->no_getxattr)
		return -EOPNOTSUPP;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.size = size;
	req->in.h.opcode = FUSE_GETXATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 2;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	req->in.args[1].size = strlen(name) + 1;
	req->in.args[1].value = name;
	/* This is really two different operations rolled into one */
	req->out.numargs = 1;
	if (size) {
		req->out.argvar = 1;
		req->out.args[0].size = size;
		req->out.args[0].value = value;
	} else {
		req->out.args[0].size = sizeof(outarg);
		req->out.args[0].value = &outarg;
	}
	fuse_request_send(fc, req);
	ret = req->out.h.error;
	if (!ret)
		ret = size ? req->out.args[0].size : outarg.size;
	else {
		if (ret == -ENOSYS) {
			fc->no_getxattr = 1;
			ret = -EOPNOTSUPP;
		}
	}
	fuse_put_request(fc, req);
	return ret;
}

static ssize_t fuse_listxattr(struct dentry *entry, char *list, size_t size)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	struct fuse_getxattr_in inarg;
	struct fuse_getxattr_out outarg;
	ssize_t ret;

	if (!fuse_allow_task(fc, current))
		return -EACCES;

	if (fc->no_listxattr)
		return -EOPNOTSUPP;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	memset(&inarg, 0, sizeof(inarg));
	inarg.size = size;
	req->in.h.opcode = FUSE_LISTXATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = sizeof(inarg);
	req->in.args[0].value = &inarg;
	/* This is really two different operations rolled into one */
	req->out.numargs = 1;
	if (size) {
		req->out.argvar = 1;
		req->out.args[0].size = size;
		req->out.args[0].value = list;
	} else {
		req->out.args[0].size = sizeof(outarg);
		req->out.args[0].value = &outarg;
	}
	fuse_request_send(fc, req);
	ret = req->out.h.error;
	if (!ret)
		ret = size ? req->out.args[0].size : outarg.size;
	else {
		if (ret == -ENOSYS) {
			fc->no_listxattr = 1;
			ret = -EOPNOTSUPP;
		}
	}
	fuse_put_request(fc, req);
	return ret;
}

static int fuse_removexattr(struct dentry *entry, const char *name)
{
	struct inode *inode = entry->d_inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct fuse_req *req;
	int err;

	if (fc->no_removexattr)
		return -EOPNOTSUPP;

	req = fuse_get_req(fc);
	if (IS_ERR(req))
		return PTR_ERR(req);

	req->in.h.opcode = FUSE_REMOVEXATTR;
	req->in.h.nodeid = get_node_id(inode);
	req->in.numargs = 1;
	req->in.args[0].size = strlen(name) + 1;
	req->in.args[0].value = name;
	fuse_request_send(fc, req);
	err = req->out.h.error;
	fuse_put_request(fc, req);
	if (err == -ENOSYS) {
		fc->no_removexattr = 1;
		err = -EOPNOTSUPP;
	}
	if (!err)
		fuse_invalidate_attr(inode);
	return err;
=======

	if (fault_blocked)
		filemap_invalidate_unlock(mapping);
	return err;
}

static int fuse_setattr(struct mnt_idmap *idmap, struct dentry *entry,
			struct iattr *attr)
{
	struct inode *inode = d_inode(entry);
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct file *file = (attr->ia_valid & ATTR_FILE) ? attr->ia_file : NULL;
	int ret;

	if (fuse_is_bad(inode))
		return -EIO;

	if (!fuse_allow_current_process(get_fuse_conn(inode)))
		return -EACCES;

	if (attr->ia_valid & (ATTR_KILL_SUID | ATTR_KILL_SGID)) {
		attr->ia_valid &= ~(ATTR_KILL_SUID | ATTR_KILL_SGID |
				    ATTR_MODE);

		/*
		 * The only sane way to reliably kill suid/sgid is to do it in
		 * the userspace filesystem
		 *
		 * This should be done on write(), truncate() and chown().
		 */
		if (!fc->handle_killpriv && !fc->handle_killpriv_v2) {
			/*
			 * ia_mode calculation may have used stale i_mode.
			 * Refresh and recalculate.
			 */
			ret = fuse_do_getattr(inode, NULL, file);
			if (ret)
				return ret;

			attr->ia_mode = inode->i_mode;
			if (inode->i_mode & S_ISUID) {
				attr->ia_valid |= ATTR_MODE;
				attr->ia_mode &= ~S_ISUID;
			}
			if ((inode->i_mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
				attr->ia_valid |= ATTR_MODE;
				attr->ia_mode &= ~S_ISGID;
			}
		}
	}
	if (!attr->ia_valid)
		return 0;

	ret = fuse_do_setattr(entry, attr, file);
	if (!ret) {
		/*
		 * If filesystem supports acls it may have updated acl xattrs in
		 * the filesystem, so forget cached acls for the inode.
		 */
		if (fc->posix_acl)
			forget_all_cached_acls(inode);

		/* Directory mode changed, may need to revalidate access */
		if (d_is_dir(entry) && (attr->ia_valid & ATTR_MODE))
			fuse_invalidate_entry_cache(entry);
	}
	return ret;
}

static int fuse_getattr(struct mnt_idmap *idmap,
			const struct path *path, struct kstat *stat,
			u32 request_mask, unsigned int flags)
{
	struct inode *inode = d_inode(path->dentry);
	struct fuse_conn *fc = get_fuse_conn(inode);

	if (fuse_is_bad(inode))
		return -EIO;

	if (!fuse_allow_current_process(fc)) {
		if (!request_mask) {
			/*
			 * If user explicitly requested *nothing* then don't
			 * error out, but return st_dev only.
			 */
			stat->result_mask = 0;
			stat->dev = inode->i_sb->s_dev;
			return 0;
		}
		return -EACCES;
	}

	return fuse_update_get_attr(inode, NULL, stat, request_mask, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct inode_operations fuse_dir_inode_operations = {
	.lookup		= fuse_lookup,
	.mkdir		= fuse_mkdir,
	.symlink	= fuse_symlink,
	.unlink		= fuse_unlink,
	.rmdir		= fuse_rmdir,
<<<<<<< HEAD
	.rename		= fuse_rename,
=======
	.rename		= fuse_rename2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.link		= fuse_link,
	.setattr	= fuse_setattr,
	.create		= fuse_create,
	.atomic_open	= fuse_atomic_open,
<<<<<<< HEAD
	.mknod		= fuse_mknod,
	.permission	= fuse_permission,
	.getattr	= fuse_getattr,
	.setxattr	= fuse_setxattr,
	.getxattr	= fuse_getxattr,
	.listxattr	= fuse_listxattr,
	.removexattr	= fuse_removexattr,
=======
	.tmpfile	= fuse_tmpfile,
	.mknod		= fuse_mknod,
	.permission	= fuse_permission,
	.getattr	= fuse_getattr,
	.listxattr	= fuse_listxattr,
	.get_inode_acl	= fuse_get_inode_acl,
	.get_acl	= fuse_get_acl,
	.set_acl	= fuse_set_acl,
	.fileattr_get	= fuse_fileattr_get,
	.fileattr_set	= fuse_fileattr_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct file_operations fuse_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= fuse_readdir,
=======
	.iterate_shared	= fuse_readdir,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open		= fuse_dir_open,
	.release	= fuse_dir_release,
	.fsync		= fuse_dir_fsync,
	.unlocked_ioctl	= fuse_dir_ioctl,
	.compat_ioctl	= fuse_dir_compat_ioctl,
};

static const struct inode_operations fuse_common_inode_operations = {
	.setattr	= fuse_setattr,
	.permission	= fuse_permission,
	.getattr	= fuse_getattr,
<<<<<<< HEAD
	.setxattr	= fuse_setxattr,
	.getxattr	= fuse_getxattr,
	.listxattr	= fuse_listxattr,
	.removexattr	= fuse_removexattr,
=======
	.listxattr	= fuse_listxattr,
	.get_inode_acl	= fuse_get_inode_acl,
	.get_acl	= fuse_get_acl,
	.set_acl	= fuse_set_acl,
	.fileattr_get	= fuse_fileattr_get,
	.fileattr_set	= fuse_fileattr_set,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct inode_operations fuse_symlink_inode_operations = {
	.setattr	= fuse_setattr,
<<<<<<< HEAD
	.follow_link	= fuse_follow_link,
	.put_link	= fuse_put_link,
	.readlink	= generic_readlink,
	.getattr	= fuse_getattr,
	.setxattr	= fuse_setxattr,
	.getxattr	= fuse_getxattr,
	.listxattr	= fuse_listxattr,
	.removexattr	= fuse_removexattr,
=======
	.get_link	= fuse_get_link,
	.getattr	= fuse_getattr,
	.listxattr	= fuse_listxattr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

void fuse_init_common(struct inode *inode)
{
	inode->i_op = &fuse_common_inode_operations;
}

void fuse_init_dir(struct inode *inode)
{
<<<<<<< HEAD
	inode->i_op = &fuse_dir_inode_operations;
	inode->i_fop = &fuse_dir_operations;
}

void fuse_init_symlink(struct inode *inode)
{
	inode->i_op = &fuse_symlink_inode_operations;
=======
	struct fuse_inode *fi = get_fuse_inode(inode);

	inode->i_op = &fuse_dir_inode_operations;
	inode->i_fop = &fuse_dir_operations;

	spin_lock_init(&fi->rdc.lock);
	fi->rdc.cached = false;
	fi->rdc.size = 0;
	fi->rdc.pos = 0;
	fi->rdc.version = 0;
}

static int fuse_symlink_read_folio(struct file *null, struct folio *folio)
{
	int err = fuse_readlink_page(folio->mapping->host, &folio->page);

	if (!err)
		folio_mark_uptodate(folio);

	folio_unlock(folio);

	return err;
}

static const struct address_space_operations fuse_symlink_aops = {
	.read_folio	= fuse_symlink_read_folio,
};

void fuse_init_symlink(struct inode *inode)
{
	inode->i_op = &fuse_symlink_inode_operations;
	inode->i_data.a_ops = &fuse_symlink_aops;
	inode_nohighmem(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
