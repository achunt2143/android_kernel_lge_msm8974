<<<<<<< HEAD
/* CacheFiles path walking and related routines
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fsnotify.h>
#include <linux/quotaops.h>
#include <linux/xattr.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/security.h>
#include <linux/slab.h>
#include "internal.h"

#define CACHEFILES_KEYBUF_SIZE 512

/*
 * dump debugging info about an object
 */
static noinline
void __cachefiles_printk_object(struct cachefiles_object *object,
				const char *prefix,
				u8 *keybuf)
{
	struct fscache_cookie *cookie;
	unsigned keylen, loop;

	printk(KERN_ERR "%sobject: OBJ%x\n",
	       prefix, object->fscache.debug_id);
	printk(KERN_ERR "%sobjstate=%s fl=%lx wbusy=%x ev=%lx[%lx]\n",
	       prefix, fscache_object_states[object->fscache.state],
	       object->fscache.flags, work_busy(&object->fscache.work),
	       object->fscache.events,
	       object->fscache.event_mask & FSCACHE_OBJECT_EVENTS_MASK);
	printk(KERN_ERR "%sops=%u inp=%u exc=%u\n",
	       prefix, object->fscache.n_ops, object->fscache.n_in_progress,
	       object->fscache.n_exclusive);
	printk(KERN_ERR "%sparent=%p\n",
	       prefix, object->fscache.parent);

	spin_lock(&object->fscache.lock);
	cookie = object->fscache.cookie;
	if (cookie) {
		printk(KERN_ERR "%scookie=%p [pr=%p nd=%p fl=%lx]\n",
		       prefix,
		       object->fscache.cookie,
		       object->fscache.cookie->parent,
		       object->fscache.cookie->netfs_data,
		       object->fscache.cookie->flags);
		if (keybuf)
			keylen = cookie->def->get_key(cookie->netfs_data, keybuf,
						      CACHEFILES_KEYBUF_SIZE);
		else
			keylen = 0;
	} else {
		printk(KERN_ERR "%scookie=NULL\n", prefix);
		keylen = 0;
	}
	spin_unlock(&object->fscache.lock);

	if (keylen) {
		printk(KERN_ERR "%skey=[%u] '", prefix, keylen);
		for (loop = 0; loop < keylen; loop++)
			printk("%02x", keybuf[loop]);
		printk("'\n");
	}
}

/*
 * dump debugging info about a pair of objects
 */
static noinline void cachefiles_printk_object(struct cachefiles_object *object,
					      struct cachefiles_object *xobject)
{
	u8 *keybuf;

	keybuf = kmalloc(CACHEFILES_KEYBUF_SIZE, GFP_NOIO);
	if (object)
		__cachefiles_printk_object(object, "", keybuf);
	if (xobject)
		__cachefiles_printk_object(xobject, "x", keybuf);
	kfree(keybuf);
}

/*
 * mark the owner of a dentry, if there is one, to indicate that that dentry
 * has been preemptively deleted
 * - the caller must hold the i_mutex on the dentry's parent as required to
 *   call vfs_unlink(), vfs_rmdir() or vfs_rename()
 */
static void cachefiles_mark_object_buried(struct cachefiles_cache *cache,
					  struct dentry *dentry)
{
	struct cachefiles_object *object;
	struct rb_node *p;

	_enter(",'%*.*s'",
	       dentry->d_name.len, dentry->d_name.len, dentry->d_name.name);

	write_lock(&cache->active_lock);

	p = cache->active_nodes.rb_node;
	while (p) {
		object = rb_entry(p, struct cachefiles_object, active_node);
		if (object->dentry > dentry)
			p = p->rb_left;
		else if (object->dentry < dentry)
			p = p->rb_right;
		else
			goto found_dentry;
	}

	write_unlock(&cache->active_lock);
	_leave(" [no owner]");
	return;

	/* found the dentry for  */
found_dentry:
	kdebug("preemptive burial: OBJ%x [%s] %p",
	       object->fscache.debug_id,
	       fscache_object_states[object->fscache.state],
	       dentry);

	if (object->fscache.state < FSCACHE_OBJECT_DYING) {
		printk(KERN_ERR "\n");
		printk(KERN_ERR "CacheFiles: Error:"
		       " Can't preemptively bury live object\n");
		cachefiles_printk_object(object, NULL);
	} else if (test_and_set_bit(CACHEFILES_OBJECT_BURIED, &object->flags)) {
		printk(KERN_ERR "CacheFiles: Error:"
		       " Object already preemptively buried\n");
	}

	write_unlock(&cache->active_lock);
	_leave(" [owner marked]");
}

/*
 * record the fact that an object is now active
 */
static int cachefiles_mark_object_active(struct cachefiles_cache *cache,
					 struct cachefiles_object *object)
{
	struct cachefiles_object *xobject;
	struct rb_node **_p, *_parent = NULL;
	struct dentry *dentry;

	_enter(",%p", object);

try_again:
	write_lock(&cache->active_lock);

	if (test_and_set_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags)) {
		printk(KERN_ERR "CacheFiles: Error: Object already active\n");
		cachefiles_printk_object(object, NULL);
		BUG();
	}

	dentry = object->dentry;
	_p = &cache->active_nodes.rb_node;
	while (*_p) {
		_parent = *_p;
		xobject = rb_entry(_parent,
				   struct cachefiles_object, active_node);

		ASSERT(xobject != object);

		if (xobject->dentry > dentry)
			_p = &(*_p)->rb_left;
		else if (xobject->dentry < dentry)
			_p = &(*_p)->rb_right;
		else
			goto wait_for_old_object;
	}

	rb_link_node(&object->active_node, _parent, _p);
	rb_insert_color(&object->active_node, &cache->active_nodes);

	write_unlock(&cache->active_lock);
	_leave(" = 0");
	return 0;

	/* an old object from a previous incarnation is hogging the slot - we
	 * need to wait for it to be destroyed */
wait_for_old_object:
	if (xobject->fscache.state < FSCACHE_OBJECT_DYING) {
		printk(KERN_ERR "\n");
		printk(KERN_ERR "CacheFiles: Error:"
		       " Unexpected object collision\n");
		cachefiles_printk_object(object, xobject);
		BUG();
	}
	atomic_inc(&xobject->usage);
	write_unlock(&cache->active_lock);

	if (test_bit(CACHEFILES_OBJECT_ACTIVE, &xobject->flags)) {
		wait_queue_head_t *wq;

		signed long timeout = 60 * HZ;
		wait_queue_t wait;
		bool requeue;

		/* if the object we're waiting for is queued for processing,
		 * then just put ourselves on the queue behind it */
		if (work_pending(&xobject->fscache.work)) {
			_debug("queue OBJ%x behind OBJ%x immediately",
			       object->fscache.debug_id,
			       xobject->fscache.debug_id);
			goto requeue;
		}

		/* otherwise we sleep until either the object we're waiting for
		 * is done, or the fscache_object is congested */
		wq = bit_waitqueue(&xobject->flags, CACHEFILES_OBJECT_ACTIVE);
		init_wait(&wait);
		requeue = false;
		do {
			prepare_to_wait(wq, &wait, TASK_UNINTERRUPTIBLE);
			if (!test_bit(CACHEFILES_OBJECT_ACTIVE, &xobject->flags))
				break;

			requeue = fscache_object_sleep_till_congested(&timeout);
		} while (timeout > 0 && !requeue);
		finish_wait(wq, &wait);

		if (requeue &&
		    test_bit(CACHEFILES_OBJECT_ACTIVE, &xobject->flags)) {
			_debug("queue OBJ%x behind OBJ%x after wait",
			       object->fscache.debug_id,
			       xobject->fscache.debug_id);
			goto requeue;
		}

		if (timeout <= 0) {
			printk(KERN_ERR "\n");
			printk(KERN_ERR "CacheFiles: Error: Overlong"
			       " wait for old active object to go away\n");
			cachefiles_printk_object(object, xobject);
			goto requeue;
		}
	}

	ASSERT(!test_bit(CACHEFILES_OBJECT_ACTIVE, &xobject->flags));

	cache->cache.ops->put_object(&xobject->fscache);
	goto try_again;

requeue:
	clear_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags);
	cache->cache.ops->put_object(&xobject->fscache);
	_leave(" = -ETIMEDOUT");
	return -ETIMEDOUT;
}

/*
 * delete an object representation from the cache
 * - file backed objects are unlinked
 * - directory backed objects are stuffed into the graveyard for userspace to
 *   delete
 * - unlocks the directory mutex
 */
static int cachefiles_bury_object(struct cachefiles_cache *cache,
				  struct dentry *dir,
				  struct dentry *rep,
				  bool preemptive)
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* CacheFiles path walking and related routines
 *
 * Copyright (C) 2021 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 */

#include <linux/fs.h>
#include <linux/namei.h>
#include "internal.h"

/*
 * Mark the backing file as being a cache file if it's not already in use.  The
 * mark tells the culling request command that it's not allowed to cull the
 * file or directory.  The caller must hold the inode lock.
 */
static bool __cachefiles_mark_inode_in_use(struct cachefiles_object *object,
					   struct inode *inode)
{
	bool can_use = false;

	if (!(inode->i_flags & S_KERNEL_FILE)) {
		inode->i_flags |= S_KERNEL_FILE;
		trace_cachefiles_mark_active(object, inode);
		can_use = true;
	} else {
		trace_cachefiles_mark_failed(object, inode);
	}

	return can_use;
}

static bool cachefiles_mark_inode_in_use(struct cachefiles_object *object,
					 struct inode *inode)
{
	bool can_use;

	inode_lock(inode);
	can_use = __cachefiles_mark_inode_in_use(object, inode);
	inode_unlock(inode);
	return can_use;
}

/*
 * Unmark a backing inode.  The caller must hold the inode lock.
 */
static void __cachefiles_unmark_inode_in_use(struct cachefiles_object *object,
					     struct inode *inode)
{
	inode->i_flags &= ~S_KERNEL_FILE;
	trace_cachefiles_mark_inactive(object, inode);
}

static void cachefiles_do_unmark_inode_in_use(struct cachefiles_object *object,
					      struct inode *inode)
{
	inode_lock(inode);
	__cachefiles_unmark_inode_in_use(object, inode);
	inode_unlock(inode);
}

/*
 * Unmark a backing inode and tell cachefilesd that there's something that can
 * be culled.
 */
void cachefiles_unmark_inode_in_use(struct cachefiles_object *object,
				    struct file *file)
{
	struct cachefiles_cache *cache = object->volume->cache;
	struct inode *inode = file_inode(file);

	cachefiles_do_unmark_inode_in_use(object, inode);

	if (!test_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags)) {
		atomic_long_add(inode->i_blocks, &cache->b_released);
		if (atomic_inc_return(&cache->f_released))
			cachefiles_state_changed(cache);
	}
}

/*
 * get a subdirectory
 */
struct dentry *cachefiles_get_directory(struct cachefiles_cache *cache,
					struct dentry *dir,
					const char *dirname,
					bool *_is_new)
{
	struct dentry *subdir;
	struct path path;
	int ret;

	_enter(",,%s", dirname);

	/* search the current directory for the element name */
	inode_lock_nested(d_inode(dir), I_MUTEX_PARENT);

retry:
	ret = cachefiles_inject_read_error();
	if (ret == 0)
		subdir = lookup_one_len(dirname, dir, strlen(dirname));
	else
		subdir = ERR_PTR(ret);
	trace_cachefiles_lookup(NULL, dir, subdir);
	if (IS_ERR(subdir)) {
		trace_cachefiles_vfs_error(NULL, d_backing_inode(dir),
					   PTR_ERR(subdir),
					   cachefiles_trace_lookup_error);
		if (PTR_ERR(subdir) == -ENOMEM)
			goto nomem_d_alloc;
		goto lookup_error;
	}

	_debug("subdir -> %pd %s",
	       subdir, d_backing_inode(subdir) ? "positive" : "negative");

	/* we need to create the subdir if it doesn't exist yet */
	if (d_is_negative(subdir)) {
		ret = cachefiles_has_space(cache, 1, 0,
					   cachefiles_has_space_for_create);
		if (ret < 0)
			goto mkdir_error;

		_debug("attempt mkdir");

		path.mnt = cache->mnt;
		path.dentry = dir;
		ret = security_path_mkdir(&path, subdir, 0700);
		if (ret < 0)
			goto mkdir_error;
		ret = cachefiles_inject_write_error();
		if (ret == 0)
			ret = vfs_mkdir(&nop_mnt_idmap, d_inode(dir), subdir, 0700);
		if (ret < 0) {
			trace_cachefiles_vfs_error(NULL, d_inode(dir), ret,
						   cachefiles_trace_mkdir_error);
			goto mkdir_error;
		}
		trace_cachefiles_mkdir(dir, subdir);

		if (unlikely(d_unhashed(subdir))) {
			cachefiles_put_directory(subdir);
			goto retry;
		}
		ASSERT(d_backing_inode(subdir));

		_debug("mkdir -> %pd{ino=%lu}",
		       subdir, d_backing_inode(subdir)->i_ino);
		if (_is_new)
			*_is_new = true;
	}

	/* Tell rmdir() it's not allowed to delete the subdir */
	inode_lock(d_inode(subdir));
	inode_unlock(d_inode(dir));

	if (!__cachefiles_mark_inode_in_use(NULL, d_inode(subdir))) {
		pr_notice("cachefiles: Inode already in use: %pd (B=%lx)\n",
			  subdir, d_inode(subdir)->i_ino);
		goto mark_error;
	}

	inode_unlock(d_inode(subdir));

	/* we need to make sure the subdir is a directory */
	ASSERT(d_backing_inode(subdir));

	if (!d_can_lookup(subdir)) {
		pr_err("%s is not a directory\n", dirname);
		ret = -EIO;
		goto check_error;
	}

	ret = -EPERM;
	if (!(d_backing_inode(subdir)->i_opflags & IOP_XATTR) ||
	    !d_backing_inode(subdir)->i_op->lookup ||
	    !d_backing_inode(subdir)->i_op->mkdir ||
	    !d_backing_inode(subdir)->i_op->rename ||
	    !d_backing_inode(subdir)->i_op->rmdir ||
	    !d_backing_inode(subdir)->i_op->unlink)
		goto check_error;

	_leave(" = [%lu]", d_backing_inode(subdir)->i_ino);
	return subdir;

check_error:
	cachefiles_put_directory(subdir);
	_leave(" = %d [check]", ret);
	return ERR_PTR(ret);

mark_error:
	inode_unlock(d_inode(subdir));
	dput(subdir);
	return ERR_PTR(-EBUSY);

mkdir_error:
	inode_unlock(d_inode(dir));
	dput(subdir);
	pr_err("mkdir %s failed with error %d\n", dirname, ret);
	return ERR_PTR(ret);

lookup_error:
	inode_unlock(d_inode(dir));
	ret = PTR_ERR(subdir);
	pr_err("Lookup %s failed with error %d\n", dirname, ret);
	return ERR_PTR(ret);

nomem_d_alloc:
	inode_unlock(d_inode(dir));
	_leave(" = -ENOMEM");
	return ERR_PTR(-ENOMEM);
}

/*
 * Put a subdirectory.
 */
void cachefiles_put_directory(struct dentry *dir)
{
	if (dir) {
		cachefiles_do_unmark_inode_in_use(NULL, d_inode(dir));
		dput(dir);
	}
}

/*
 * Remove a regular file from the cache.
 */
static int cachefiles_unlink(struct cachefiles_cache *cache,
			     struct cachefiles_object *object,
			     struct dentry *dir, struct dentry *dentry,
			     enum fscache_why_object_killed why)
{
	struct path path = {
		.mnt	= cache->mnt,
		.dentry	= dir,
	};
	int ret;

	trace_cachefiles_unlink(object, d_inode(dentry)->i_ino, why);
	ret = security_path_unlink(&path, dentry);
	if (ret < 0) {
		cachefiles_io_error(cache, "Unlink security error");
		return ret;
	}

	ret = cachefiles_inject_remove_error();
	if (ret == 0) {
		ret = vfs_unlink(&nop_mnt_idmap, d_backing_inode(dir), dentry, NULL);
		if (ret == -EIO)
			cachefiles_io_error(cache, "Unlink failed");
	}
	if (ret != 0)
		trace_cachefiles_vfs_error(object, d_backing_inode(dir), ret,
					   cachefiles_trace_unlink_error);
	return ret;
}

/*
 * Delete an object representation from the cache
 * - File backed objects are unlinked
 * - Directory backed objects are stuffed into the graveyard for userspace to
 *   delete
 */
int cachefiles_bury_object(struct cachefiles_cache *cache,
			   struct cachefiles_object *object,
			   struct dentry *dir,
			   struct dentry *rep,
			   enum fscache_why_object_killed why)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct dentry *grave, *trap;
	struct path path, path_to_graveyard;
	char nbuffer[8 + 8 + 1];
	int ret;

<<<<<<< HEAD
	_enter(",'%*.*s','%*.*s'",
	       dir->d_name.len, dir->d_name.len, dir->d_name.name,
	       rep->d_name.len, rep->d_name.len, rep->d_name.name);

	_debug("remove %p from %p", rep, dir);

	/* non-directories can just be unlinked */
	if (!S_ISDIR(rep->d_inode->i_mode)) {
		_debug("unlink stale object");

		path.mnt = cache->mnt;
		path.dentry = dir;
		ret = security_path_unlink(&path, rep);
		if (ret < 0) {
			cachefiles_io_error(cache, "Unlink security error");
		} else {
			ret = vfs_unlink(dir->d_inode, rep);

			if (preemptive)
				cachefiles_mark_object_buried(cache, rep);
		}

		mutex_unlock(&dir->d_inode->i_mutex);

		if (ret == -EIO)
			cachefiles_io_error(cache, "Unlink failed");

=======
	_enter(",'%pd','%pd'", dir, rep);

	if (rep->d_parent != dir) {
		inode_unlock(d_inode(dir));
		_leave(" = -ESTALE");
		return -ESTALE;
	}

	/* non-directories can just be unlinked */
	if (!d_is_dir(rep)) {
		dget(rep); /* Stop the dentry being negated if it's only pinned
			    * by a file struct.
			    */
		ret = cachefiles_unlink(cache, object, dir, rep, why);
		dput(rep);

		inode_unlock(d_inode(dir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		_leave(" = %d", ret);
		return ret;
	}

	/* directories have to be moved to the graveyard */
	_debug("move stale object to graveyard");
<<<<<<< HEAD
	mutex_unlock(&dir->d_inode->i_mutex);
=======
	inode_unlock(d_inode(dir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

try_again:
	/* first step is to make up a grave dentry in the graveyard */
	sprintf(nbuffer, "%08x%08x",
<<<<<<< HEAD
		(uint32_t) get_seconds(),
=======
		(uint32_t) ktime_get_real_seconds(),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		(uint32_t) atomic_inc_return(&cache->gravecounter));

	/* do the multiway lock magic */
	trap = lock_rename(cache->graveyard, dir);
<<<<<<< HEAD

	/* do some checks before getting the grave dentry */
	if (rep->d_parent != dir) {
=======
	if (IS_ERR(trap))
		return PTR_ERR(trap);

	/* do some checks before getting the grave dentry */
	if (rep->d_parent != dir || IS_DEADDIR(d_inode(rep))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* the entry was probably culled when we dropped the parent dir
		 * lock */
		unlock_rename(cache->graveyard, dir);
		_leave(" = 0 [culled?]");
		return 0;
	}

<<<<<<< HEAD
	if (!S_ISDIR(cache->graveyard->d_inode->i_mode)) {
=======
	if (!d_can_lookup(cache->graveyard)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unlock_rename(cache->graveyard, dir);
		cachefiles_io_error(cache, "Graveyard no longer a directory");
		return -EIO;
	}

	if (trap == rep) {
		unlock_rename(cache->graveyard, dir);
		cachefiles_io_error(cache, "May not make directory loop");
		return -EIO;
	}

	if (d_mountpoint(rep)) {
		unlock_rename(cache->graveyard, dir);
		cachefiles_io_error(cache, "Mountpoint in cache");
		return -EIO;
	}

	grave = lookup_one_len(nbuffer, cache->graveyard, strlen(nbuffer));
	if (IS_ERR(grave)) {
		unlock_rename(cache->graveyard, dir);
<<<<<<< HEAD
=======
		trace_cachefiles_vfs_error(object, d_inode(cache->graveyard),
					   PTR_ERR(grave),
					   cachefiles_trace_lookup_error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (PTR_ERR(grave) == -ENOMEM) {
			_leave(" = -ENOMEM");
			return -ENOMEM;
		}

<<<<<<< HEAD
		cachefiles_io_error(cache, "Lookup error %ld",
				    PTR_ERR(grave));
		return -EIO;
	}

	if (grave->d_inode) {
=======
		cachefiles_io_error(cache, "Lookup error %ld", PTR_ERR(grave));
		return -EIO;
	}

	if (d_is_positive(grave)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unlock_rename(cache->graveyard, dir);
		dput(grave);
		grave = NULL;
		cond_resched();
		goto try_again;
	}

	if (d_mountpoint(grave)) {
		unlock_rename(cache->graveyard, dir);
		dput(grave);
		cachefiles_io_error(cache, "Mountpoint in graveyard");
		return -EIO;
	}

	/* target should not be an ancestor of source */
	if (trap == grave) {
		unlock_rename(cache->graveyard, dir);
		dput(grave);
		cachefiles_io_error(cache, "May not make directory loop");
		return -EIO;
	}

	/* attempt the rename */
	path.mnt = cache->mnt;
	path.dentry = dir;
	path_to_graveyard.mnt = cache->mnt;
	path_to_graveyard.dentry = cache->graveyard;
<<<<<<< HEAD
	ret = security_path_rename(&path, rep, &path_to_graveyard, grave);
	if (ret < 0) {
		cachefiles_io_error(cache, "Rename security error %d", ret);
	} else {
		ret = vfs_rename(dir->d_inode, rep,
				 cache->graveyard->d_inode, grave);
		if (ret != 0 && ret != -ENOMEM)
			cachefiles_io_error(cache,
					    "Rename failed with error %d", ret);

		if (preemptive)
			cachefiles_mark_object_buried(cache, rep);
	}

=======
	ret = security_path_rename(&path, rep, &path_to_graveyard, grave, 0);
	if (ret < 0) {
		cachefiles_io_error(cache, "Rename security error %d", ret);
	} else {
		struct renamedata rd = {
			.old_mnt_idmap	= &nop_mnt_idmap,
			.old_dir	= d_inode(dir),
			.old_dentry	= rep,
			.new_mnt_idmap	= &nop_mnt_idmap,
			.new_dir	= d_inode(cache->graveyard),
			.new_dentry	= grave,
		};
		trace_cachefiles_rename(object, d_inode(rep)->i_ino, why);
		ret = cachefiles_inject_read_error();
		if (ret == 0)
			ret = vfs_rename(&rd);
		if (ret != 0)
			trace_cachefiles_vfs_error(object, d_inode(dir), ret,
						   cachefiles_trace_rename_error);
		if (ret != 0 && ret != -ENOMEM)
			cachefiles_io_error(cache,
					    "Rename failed with error %d", ret);
	}

	__cachefiles_unmark_inode_in_use(object, d_inode(rep));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unlock_rename(cache->graveyard, dir);
	dput(grave);
	_leave(" = 0");
	return 0;
}

/*
<<<<<<< HEAD
 * delete an object representation from the cache
 */
int cachefiles_delete_object(struct cachefiles_cache *cache,
			     struct cachefiles_object *object)
{
	struct dentry *dir;
	int ret;

	_enter(",OBJ%x{%p}", object->fscache.debug_id, object->dentry);

	ASSERT(object->dentry);
	ASSERT(object->dentry->d_inode);
	ASSERT(object->dentry->d_parent);

	dir = dget_parent(object->dentry);

	mutex_lock_nested(&dir->d_inode->i_mutex, I_MUTEX_PARENT);

	if (test_bit(CACHEFILES_OBJECT_BURIED, &object->flags)) {
		/* object allocation for the same key preemptively deleted this
		 * object's file so that it could create its own file */
		_debug("object preemptively buried");
		mutex_unlock(&dir->d_inode->i_mutex);
		ret = 0;
	} else {
		/* we need to check that our parent is _still_ our parent - it
		 * may have been renamed */
		if (dir == object->dentry->d_parent) {
			ret = cachefiles_bury_object(cache, dir,
						     object->dentry, false);
		} else {
			/* it got moved, presumably by cachefilesd culling it,
			 * so it's no longer in the key path and we can ignore
			 * it */
			mutex_unlock(&dir->d_inode->i_mutex);
			ret = 0;
		}
	}

	dput(dir);
	_leave(" = %d", ret);
	return ret;
=======
 * Delete a cache file.
 */
int cachefiles_delete_object(struct cachefiles_object *object,
			     enum fscache_why_object_killed why)
{
	struct cachefiles_volume *volume = object->volume;
	struct dentry *dentry = object->file->f_path.dentry;
	struct dentry *fan = volume->fanout[(u8)object->cookie->key_hash];
	int ret;

	_enter(",OBJ%x{%pD}", object->debug_id, object->file);

	/* Stop the dentry being negated if it's only pinned by a file struct. */
	dget(dentry);

	inode_lock_nested(d_backing_inode(fan), I_MUTEX_PARENT);
	ret = cachefiles_unlink(volume->cache, object, fan, dentry, why);
	inode_unlock(d_backing_inode(fan));
	dput(dentry);
	return ret;
}

/*
 * Create a temporary file and leave it unattached and un-xattr'd until the
 * time comes to discard the object from memory.
 */
struct file *cachefiles_create_tmpfile(struct cachefiles_object *object)
{
	struct cachefiles_volume *volume = object->volume;
	struct cachefiles_cache *cache = volume->cache;
	const struct cred *saved_cred;
	struct dentry *fan = volume->fanout[(u8)object->cookie->key_hash];
	struct file *file;
	const struct path parentpath = { .mnt = cache->mnt, .dentry = fan };
	uint64_t ni_size;
	long ret;


	cachefiles_begin_secure(cache, &saved_cred);

	ret = cachefiles_inject_write_error();
	if (ret == 0) {
		file = kernel_tmpfile_open(&nop_mnt_idmap, &parentpath,
					   S_IFREG | 0600,
					   O_RDWR | O_LARGEFILE | O_DIRECT,
					   cache->cache_cred);
		ret = PTR_ERR_OR_ZERO(file);
	}
	if (ret) {
		trace_cachefiles_vfs_error(object, d_inode(fan), ret,
					   cachefiles_trace_tmpfile_error);
		if (ret == -EIO)
			cachefiles_io_error_obj(object, "Failed to create tmpfile");
		goto err;
	}

	trace_cachefiles_tmpfile(object, file_inode(file));

	/* This is a newly created file with no other possible user */
	if (!cachefiles_mark_inode_in_use(object, file_inode(file)))
		WARN_ON(1);

	ret = cachefiles_ondemand_init_object(object);
	if (ret < 0)
		goto err_unuse;

	ni_size = object->cookie->object_size;
	ni_size = round_up(ni_size, CACHEFILES_DIO_BLOCK_SIZE);

	if (ni_size > 0) {
		trace_cachefiles_trunc(object, file_inode(file), 0, ni_size,
				       cachefiles_trunc_expand_tmpfile);
		ret = cachefiles_inject_write_error();
		if (ret == 0)
			ret = vfs_truncate(&file->f_path, ni_size);
		if (ret < 0) {
			trace_cachefiles_vfs_error(
				object, file_inode(file), ret,
				cachefiles_trace_trunc_error);
			goto err_unuse;
		}
	}

	ret = -EINVAL;
	if (unlikely(!file->f_op->read_iter) ||
	    unlikely(!file->f_op->write_iter)) {
		fput(file);
		pr_notice("Cache does not support read_iter and write_iter\n");
		goto err_unuse;
	}
out:
	cachefiles_end_secure(cache, saved_cred);
	return file;

err_unuse:
	cachefiles_do_unmark_inode_in_use(object, file_inode(file));
	fput(file);
err:
	file = ERR_PTR(ret);
	goto out;
}

/*
 * Create a new file.
 */
static bool cachefiles_create_file(struct cachefiles_object *object)
{
	struct file *file;
	int ret;

	ret = cachefiles_has_space(object->volume->cache, 1, 0,
				   cachefiles_has_space_for_create);
	if (ret < 0)
		return false;

	file = cachefiles_create_tmpfile(object);
	if (IS_ERR(file))
		return false;

	set_bit(FSCACHE_COOKIE_NEEDS_UPDATE, &object->cookie->flags);
	set_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags);
	_debug("create -> %pD{ino=%lu}", file, file_inode(file)->i_ino);
	object->file = file;
	return true;
}

/*
 * Open an existing file, checking its attributes and replacing it if it is
 * stale.
 */
static bool cachefiles_open_file(struct cachefiles_object *object,
				 struct dentry *dentry)
{
	struct cachefiles_cache *cache = object->volume->cache;
	struct file *file;
	struct path path;
	int ret;

	_enter("%pd", dentry);

	if (!cachefiles_mark_inode_in_use(object, d_inode(dentry))) {
		pr_notice("cachefiles: Inode already in use: %pd (B=%lx)\n",
			  dentry, d_inode(dentry)->i_ino);
		return false;
	}

	/* We need to open a file interface onto a data file now as we can't do
	 * it on demand because writeback called from do_exit() sees
	 * current->fs == NULL - which breaks d_path() called from ext4 open.
	 */
	path.mnt = cache->mnt;
	path.dentry = dentry;
	file = kernel_file_open(&path, O_RDWR | O_LARGEFILE | O_DIRECT,
				d_backing_inode(dentry), cache->cache_cred);
	if (IS_ERR(file)) {
		trace_cachefiles_vfs_error(object, d_backing_inode(dentry),
					   PTR_ERR(file),
					   cachefiles_trace_open_error);
		goto error;
	}

	if (unlikely(!file->f_op->read_iter) ||
	    unlikely(!file->f_op->write_iter)) {
		pr_notice("Cache does not support read_iter and write_iter\n");
		goto error_fput;
	}
	_debug("file -> %pd positive", dentry);

	ret = cachefiles_ondemand_init_object(object);
	if (ret < 0)
		goto error_fput;

	ret = cachefiles_check_auxdata(object, file);
	if (ret < 0)
		goto check_failed;

	clear_bit(FSCACHE_COOKIE_NO_DATA_TO_READ, &object->cookie->flags);

	object->file = file;

	/* Always update the atime on an object we've just looked up (this is
	 * used to keep track of culling, and atimes are only updated by read,
	 * write and readdir but not lookup or open).
	 */
	touch_atime(&file->f_path);
	dput(dentry);
	return true;

check_failed:
	fscache_cookie_lookup_negative(object->cookie);
	cachefiles_unmark_inode_in_use(object, file);
	fput(file);
	dput(dentry);
	if (ret == -ESTALE)
		return cachefiles_create_file(object);
	return false;

error_fput:
	fput(file);
error:
	cachefiles_do_unmark_inode_in_use(object, d_inode(dentry));
	dput(dentry);
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * walk from the parent object to the child object through the backing
 * filesystem, creating directories as we go
 */
<<<<<<< HEAD
int cachefiles_walk_to_object(struct cachefiles_object *parent,
			      struct cachefiles_object *object,
			      const char *key,
			      struct cachefiles_xattr *auxdata)
{
	struct cachefiles_cache *cache;
	struct dentry *dir, *next = NULL;
	struct path path;
	unsigned long start;
	const char *name;
	int ret, nlen;

	_enter("OBJ%x{%p},OBJ%x,%s,",
	       parent->fscache.debug_id, parent->dentry,
	       object->fscache.debug_id, key);

	cache = container_of(parent->fscache.cache,
			     struct cachefiles_cache, cache);
	path.mnt = cache->mnt;

	ASSERT(parent->dentry);
	ASSERT(parent->dentry->d_inode);

	if (!(S_ISDIR(parent->dentry->d_inode->i_mode))) {
		// TODO: convert file to dir
		_leave("looking up in none directory");
		return -ENOBUFS;
	}

	dir = dget(parent->dentry);

advance:
	/* attempt to transit the first directory component */
	name = key;
	nlen = strlen(key);

	/* key ends in a double NUL */
	key = key + nlen + 1;
	if (!*key)
		key = NULL;

lookup_again:
	/* search the current directory for the element name */
	_debug("lookup '%s'", name);

	mutex_lock_nested(&dir->d_inode->i_mutex, I_MUTEX_PARENT);

	start = jiffies;
	next = lookup_one_len(name, dir, nlen);
	cachefiles_hist(cachefiles_lookup_histogram, start);
	if (IS_ERR(next))
		goto lookup_error;

	_debug("next -> %p %s", next, next->d_inode ? "positive" : "negative");

	if (!key)
		object->new = !next->d_inode;

	/* if this element of the path doesn't exist, then the lookup phase
	 * failed, and we can release any readers in the certain knowledge that
	 * there's nothing for them to actually read */
	if (!next->d_inode)
		fscache_object_lookup_negative(&object->fscache);

	/* we need to create the object if it's negative */
	if (key || object->type == FSCACHE_COOKIE_TYPE_INDEX) {
		/* index objects and intervening tree levels must be subdirs */
		if (!next->d_inode) {
			ret = cachefiles_has_space(cache, 1, 0);
			if (ret < 0)
				goto create_error;

			path.dentry = dir;
			ret = security_path_mkdir(&path, next, 0);
			if (ret < 0)
				goto create_error;
			start = jiffies;
			ret = vfs_mkdir(dir->d_inode, next, 0);
			cachefiles_hist(cachefiles_mkdir_histogram, start);
			if (ret < 0)
				goto create_error;

			ASSERT(next->d_inode);

			_debug("mkdir -> %p{%p{ino=%lu}}",
			       next, next->d_inode, next->d_inode->i_ino);

		} else if (!S_ISDIR(next->d_inode->i_mode)) {
			kerror("inode %lu is not a directory",
			       next->d_inode->i_ino);
			ret = -ENOBUFS;
			goto error;
		}

	} else {
		/* non-index objects start out life as files */
		if (!next->d_inode) {
			ret = cachefiles_has_space(cache, 1, 0);
			if (ret < 0)
				goto create_error;

			path.dentry = dir;
			ret = security_path_mknod(&path, next, S_IFREG, 0);
			if (ret < 0)
				goto create_error;
			start = jiffies;
			ret = vfs_create(dir->d_inode, next, S_IFREG, true);
			cachefiles_hist(cachefiles_create_histogram, start);
			if (ret < 0)
				goto create_error;

			ASSERT(next->d_inode);

			_debug("create -> %p{%p{ino=%lu}}",
			       next, next->d_inode, next->d_inode->i_ino);

		} else if (!S_ISDIR(next->d_inode->i_mode) &&
			   !S_ISREG(next->d_inode->i_mode)
			   ) {
			kerror("inode %lu is not a file or directory",
			       next->d_inode->i_ino);
			ret = -ENOBUFS;
			goto error;
		}
	}

	/* process the next component */
	if (key) {
		_debug("advance");
		mutex_unlock(&dir->d_inode->i_mutex);
		dput(dir);
		dir = next;
		next = NULL;
		goto advance;
	}

	/* we've found the object we were looking for */
	object->dentry = next;

	/* if we've found that the terminal object exists, then we need to
	 * check its attributes and delete it if it's out of date */
	if (!object->new) {
		_debug("validate '%*.*s'",
		       next->d_name.len, next->d_name.len, next->d_name.name);

		ret = cachefiles_check_object_xattr(object, auxdata);
		if (ret == -ESTALE) {
			/* delete the object (the deleter drops the directory
			 * mutex) */
			object->dentry = NULL;

			ret = cachefiles_bury_object(cache, dir, next, true);
			dput(next);
			next = NULL;

			if (ret < 0)
				goto delete_error;

			_debug("redo lookup");
			goto lookup_again;
		}
	}

	/* note that we're now using this object */
	ret = cachefiles_mark_object_active(cache, object);

	mutex_unlock(&dir->d_inode->i_mutex);
	dput(dir);
	dir = NULL;

	if (ret == -ETIMEDOUT)
		goto mark_active_timed_out;

	_debug("=== OBTAINED_OBJECT ===");

	if (object->new) {
		/* attach data to a newly constructed terminal object */
		ret = cachefiles_set_object_xattr(object, auxdata);
		if (ret < 0)
			goto check_error;
	} else {
		/* always update the atime on an object we've just looked up
		 * (this is used to keep track of culling, and atimes are only
		 * updated by read, write and readdir but not lookup or
		 * open) */
		path.dentry = next;
		touch_atime(&path);
	}

	/* open a file interface onto a data file */
	if (object->type != FSCACHE_COOKIE_TYPE_INDEX) {
		if (S_ISREG(object->dentry->d_inode->i_mode)) {
			const struct address_space_operations *aops;

			ret = -EPERM;
			aops = object->dentry->d_inode->i_mapping->a_ops;
			if (!aops->bmap)
				goto check_error;

			object->backer = object->dentry;
		} else {
			BUG(); // TODO: open file in data-class subdir
		}
	}

	object->new = 0;
	fscache_obtained_object(&object->fscache);

	_leave(" = 0 [%lu]", object->dentry->d_inode->i_ino);
	return 0;

create_error:
	_debug("create error %d", ret);
	if (ret == -EIO)
		cachefiles_io_error(cache, "Create/mkdir failed");
	goto error;

mark_active_timed_out:
	_debug("mark active timed out");
	goto release_dentry;

check_error:
	_debug("check error %d", ret);
	write_lock(&cache->active_lock);
	rb_erase(&object->active_node, &cache->active_nodes);
	clear_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags);
	wake_up_bit(&object->flags, CACHEFILES_OBJECT_ACTIVE);
	write_unlock(&cache->active_lock);
release_dentry:
	dput(object->dentry);
	object->dentry = NULL;
	goto error_out;

delete_error:
	_debug("delete error %d", ret);
	goto error_out2;

lookup_error:
	_debug("lookup error %ld", PTR_ERR(next));
	ret = PTR_ERR(next);
	if (ret == -EIO)
		cachefiles_io_error(cache, "Lookup failed");
	next = NULL;
error:
	mutex_unlock(&dir->d_inode->i_mutex);
	dput(next);
error_out2:
	dput(dir);
error_out:
	_leave(" = error %d", -ret);
	return ret;
}

/*
 * get a subdirectory
 */
struct dentry *cachefiles_get_directory(struct cachefiles_cache *cache,
					struct dentry *dir,
					const char *dirname)
{
	struct dentry *subdir;
	unsigned long start;
	struct path path;
	int ret;

	_enter(",,%s", dirname);

	/* search the current directory for the element name */
	mutex_lock(&dir->d_inode->i_mutex);

	start = jiffies;
	subdir = lookup_one_len(dirname, dir, strlen(dirname));
	cachefiles_hist(cachefiles_lookup_histogram, start);
	if (IS_ERR(subdir)) {
		if (PTR_ERR(subdir) == -ENOMEM)
			goto nomem_d_alloc;
		goto lookup_error;
	}

	_debug("subdir -> %p %s",
	       subdir, subdir->d_inode ? "positive" : "negative");

	/* we need to create the subdir if it doesn't exist yet */
	if (!subdir->d_inode) {
		ret = cachefiles_has_space(cache, 1, 0);
		if (ret < 0)
			goto mkdir_error;

		_debug("attempt mkdir");

		path.mnt = cache->mnt;
		path.dentry = dir;
		ret = security_path_mkdir(&path, subdir, 0700);
		if (ret < 0)
			goto mkdir_error;
		ret = vfs_mkdir(dir->d_inode, subdir, 0700);
		if (ret < 0)
			goto mkdir_error;

		ASSERT(subdir->d_inode);

		_debug("mkdir -> %p{%p{ino=%lu}}",
		       subdir,
		       subdir->d_inode,
		       subdir->d_inode->i_ino);
	}

	mutex_unlock(&dir->d_inode->i_mutex);

	/* we need to make sure the subdir is a directory */
	ASSERT(subdir->d_inode);

	if (!S_ISDIR(subdir->d_inode->i_mode)) {
		kerror("%s is not a directory", dirname);
		ret = -EIO;
		goto check_error;
	}

	ret = -EPERM;
	if (!subdir->d_inode->i_op ||
	    !subdir->d_inode->i_op->setxattr ||
	    !subdir->d_inode->i_op->getxattr ||
	    !subdir->d_inode->i_op->lookup ||
	    !subdir->d_inode->i_op->mkdir ||
	    !subdir->d_inode->i_op->create ||
	    !subdir->d_inode->i_op->rename ||
	    !subdir->d_inode->i_op->rmdir ||
	    !subdir->d_inode->i_op->unlink)
		goto check_error;

	_leave(" = [%lu]", subdir->d_inode->i_ino);
	return subdir;

check_error:
	dput(subdir);
	_leave(" = %d [check]", ret);
	return ERR_PTR(ret);

mkdir_error:
	mutex_unlock(&dir->d_inode->i_mutex);
	dput(subdir);
	kerror("mkdir %s failed with error %d", dirname, ret);
	return ERR_PTR(ret);

lookup_error:
	mutex_unlock(&dir->d_inode->i_mutex);
	ret = PTR_ERR(subdir);
	kerror("Lookup %s failed with error %d", dirname, ret);
	return ERR_PTR(ret);

nomem_d_alloc:
	mutex_unlock(&dir->d_inode->i_mutex);
	_leave(" = -ENOMEM");
	return ERR_PTR(-ENOMEM);
}

/*
 * find out if an object is in use or not
 * - if finds object and it's not in use:
 *   - returns a pointer to the object and a reference on it
 *   - returns with the directory locked
 */
static struct dentry *cachefiles_check_active(struct cachefiles_cache *cache,
					      struct dentry *dir,
					      char *filename)
{
	struct cachefiles_object *object;
	struct rb_node *_n;
	struct dentry *victim;
	unsigned long start;
	int ret;

	//_enter(",%*.*s/,%s",
	//       dir->d_name.len, dir->d_name.len, dir->d_name.name, filename);

	/* look up the victim */
	mutex_lock_nested(&dir->d_inode->i_mutex, 1);

	start = jiffies;
	victim = lookup_one_len(filename, dir, strlen(filename));
	cachefiles_hist(cachefiles_lookup_histogram, start);
	if (IS_ERR(victim))
		goto lookup_error;

	//_debug("victim -> %p %s",
	//       victim, victim->d_inode ? "positive" : "negative");

	/* if the object is no longer there then we probably retired the object
	 * at the netfs's request whilst the cull was in progress
	 */
	if (!victim->d_inode) {
		mutex_unlock(&dir->d_inode->i_mutex);
		dput(victim);
		_leave(" = -ENOENT [absent]");
		return ERR_PTR(-ENOENT);
	}

	/* check to see if we're using this object */
	read_lock(&cache->active_lock);

	_n = cache->active_nodes.rb_node;

	while (_n) {
		object = rb_entry(_n, struct cachefiles_object, active_node);

		if (object->dentry > victim)
			_n = _n->rb_left;
		else if (object->dentry < victim)
			_n = _n->rb_right;
		else
			goto object_in_use;
	}

	read_unlock(&cache->active_lock);

	//_leave(" = %p", victim);
	return victim;

object_in_use:
	read_unlock(&cache->active_lock);
	mutex_unlock(&dir->d_inode->i_mutex);
	dput(victim);
	//_leave(" = -EBUSY [in use]");
	return ERR_PTR(-EBUSY);

lookup_error:
	mutex_unlock(&dir->d_inode->i_mutex);
	ret = PTR_ERR(victim);
	if (ret == -ENOENT) {
		/* file or dir now absent - probably retired by netfs */
		_leave(" = -ESTALE [absent]");
		return ERR_PTR(-ESTALE);
	}
=======
bool cachefiles_look_up_object(struct cachefiles_object *object)
{
	struct cachefiles_volume *volume = object->volume;
	struct dentry *dentry, *fan = volume->fanout[(u8)object->cookie->key_hash];
	int ret;

	_enter("OBJ%x,%s,", object->debug_id, object->d_name);

	/* Look up path "cache/vol/fanout/file". */
	ret = cachefiles_inject_read_error();
	if (ret == 0)
		dentry = lookup_positive_unlocked(object->d_name, fan,
						  object->d_name_len);
	else
		dentry = ERR_PTR(ret);
	trace_cachefiles_lookup(object, fan, dentry);
	if (IS_ERR(dentry)) {
		if (dentry == ERR_PTR(-ENOENT))
			goto new_file;
		if (dentry == ERR_PTR(-EIO))
			cachefiles_io_error_obj(object, "Lookup failed");
		return false;
	}

	if (!d_is_reg(dentry)) {
		pr_err("%pd is not a file\n", dentry);
		inode_lock_nested(d_inode(fan), I_MUTEX_PARENT);
		ret = cachefiles_bury_object(volume->cache, object, fan, dentry,
					     FSCACHE_OBJECT_IS_WEIRD);
		dput(dentry);
		if (ret < 0)
			return false;
		goto new_file;
	}

	if (!cachefiles_open_file(object, dentry))
		return false;

	_leave(" = t [%lu]", file_inode(object->file)->i_ino);
	return true;

new_file:
	fscache_cookie_lookup_negative(object->cookie);
	return cachefiles_create_file(object);
}

/*
 * Attempt to link a temporary file into its rightful place in the cache.
 */
bool cachefiles_commit_tmpfile(struct cachefiles_cache *cache,
			       struct cachefiles_object *object)
{
	struct cachefiles_volume *volume = object->volume;
	struct dentry *dentry, *fan = volume->fanout[(u8)object->cookie->key_hash];
	bool success = false;
	int ret;

	_enter(",%pD", object->file);

	inode_lock_nested(d_inode(fan), I_MUTEX_PARENT);
	ret = cachefiles_inject_read_error();
	if (ret == 0)
		dentry = lookup_one_len(object->d_name, fan, object->d_name_len);
	else
		dentry = ERR_PTR(ret);
	if (IS_ERR(dentry)) {
		trace_cachefiles_vfs_error(object, d_inode(fan), PTR_ERR(dentry),
					   cachefiles_trace_lookup_error);
		_debug("lookup fail %ld", PTR_ERR(dentry));
		goto out_unlock;
	}

	if (!d_is_negative(dentry)) {
		if (d_backing_inode(dentry) == file_inode(object->file)) {
			success = true;
			goto out_dput;
		}

		ret = cachefiles_unlink(volume->cache, object, fan, dentry,
					FSCACHE_OBJECT_IS_STALE);
		if (ret < 0)
			goto out_dput;

		dput(dentry);
		ret = cachefiles_inject_read_error();
		if (ret == 0)
			dentry = lookup_one_len(object->d_name, fan, object->d_name_len);
		else
			dentry = ERR_PTR(ret);
		if (IS_ERR(dentry)) {
			trace_cachefiles_vfs_error(object, d_inode(fan), PTR_ERR(dentry),
						   cachefiles_trace_lookup_error);
			_debug("lookup fail %ld", PTR_ERR(dentry));
			goto out_unlock;
		}
	}

	ret = cachefiles_inject_read_error();
	if (ret == 0)
		ret = vfs_link(object->file->f_path.dentry, &nop_mnt_idmap,
			       d_inode(fan), dentry, NULL);
	if (ret < 0) {
		trace_cachefiles_vfs_error(object, d_inode(fan), ret,
					   cachefiles_trace_link_error);
		_debug("link fail %d", ret);
	} else {
		trace_cachefiles_link(object, file_inode(object->file));
		spin_lock(&object->lock);
		/* TODO: Do we want to switch the file pointer to the new dentry? */
		clear_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags);
		spin_unlock(&object->lock);
		success = true;
	}

out_dput:
	dput(dentry);
out_unlock:
	inode_unlock(d_inode(fan));
	_leave(" = %u", success);
	return success;
}

/*
 * Look up an inode to be checked or culled.  Return -EBUSY if the inode is
 * marked in use.
 */
static struct dentry *cachefiles_lookup_for_cull(struct cachefiles_cache *cache,
						 struct dentry *dir,
						 char *filename)
{
	struct dentry *victim;
	int ret = -ENOENT;

	inode_lock_nested(d_inode(dir), I_MUTEX_PARENT);

	victim = lookup_one_len(filename, dir, strlen(filename));
	if (IS_ERR(victim))
		goto lookup_error;
	if (d_is_negative(victim))
		goto lookup_put;
	if (d_inode(victim)->i_flags & S_KERNEL_FILE)
		goto lookup_busy;
	return victim;

lookup_busy:
	ret = -EBUSY;
lookup_put:
	inode_unlock(d_inode(dir));
	dput(victim);
	return ERR_PTR(ret);

lookup_error:
	inode_unlock(d_inode(dir));
	ret = PTR_ERR(victim);
	if (ret == -ENOENT)
		return ERR_PTR(-ESTALE); /* Probably got retired by the netfs */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ret == -EIO) {
		cachefiles_io_error(cache, "Lookup failed");
	} else if (ret != -ENOMEM) {
<<<<<<< HEAD
		kerror("Internal error: %d", ret);
		ret = -EIO;
	}

	_leave(" = %d", ret);
=======
		pr_err("Internal error: %d\n", ret);
		ret = -EIO;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(ret);
}

/*
<<<<<<< HEAD
 * cull an object if it's not in use
=======
 * Cull an object if it's not in use
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * - called only by cache manager daemon
 */
int cachefiles_cull(struct cachefiles_cache *cache, struct dentry *dir,
		    char *filename)
{
	struct dentry *victim;
<<<<<<< HEAD
	int ret;

	_enter(",%*.*s/,%s",
	       dir->d_name.len, dir->d_name.len, dir->d_name.name, filename);

	victim = cachefiles_check_active(cache, dir, filename);
	if (IS_ERR(victim))
		return PTR_ERR(victim);

	_debug("victim -> %p %s",
	       victim, victim->d_inode ? "positive" : "negative");

	/* okay... the victim is not being used so we can cull it
	 * - start by marking it as stale
	 */
	_debug("victim is cullable");

	ret = cachefiles_remove_object_xattr(cache, victim);
	if (ret < 0)
		goto error_unlock;

	/*  actually remove the victim (drops the dir mutex) */
	_debug("bury");

	ret = cachefiles_bury_object(cache, dir, victim, false);
	if (ret < 0)
		goto error;

=======
	struct inode *inode;
	int ret;

	_enter(",%pd/,%s", dir, filename);

	victim = cachefiles_lookup_for_cull(cache, dir, filename);
	if (IS_ERR(victim))
		return PTR_ERR(victim);

	/* check to see if someone is using this object */
	inode = d_inode(victim);
	inode_lock(inode);
	if (inode->i_flags & S_KERNEL_FILE) {
		ret = -EBUSY;
	} else {
		/* Stop the cache from picking it back up */
		inode->i_flags |= S_KERNEL_FILE;
		ret = 0;
	}
	inode_unlock(inode);
	if (ret < 0)
		goto error_unlock;

	ret = cachefiles_bury_object(cache, NULL, dir, victim,
				     FSCACHE_OBJECT_WAS_CULLED);
	if (ret < 0)
		goto error;

	fscache_count_culled();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dput(victim);
	_leave(" = 0");
	return 0;

error_unlock:
<<<<<<< HEAD
	mutex_unlock(&dir->d_inode->i_mutex);
error:
	dput(victim);
	if (ret == -ENOENT) {
		/* file or dir now absent - probably retired by netfs */
		_leave(" = -ESTALE [absent]");
		return -ESTALE;
	}

	if (ret != -ENOMEM) {
		kerror("Internal error: %d", ret);
=======
	inode_unlock(d_inode(dir));
error:
	dput(victim);
	if (ret == -ENOENT)
		return -ESTALE; /* Probably got retired by the netfs */

	if (ret != -ENOMEM) {
		pr_err("Internal error: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EIO;
	}

	_leave(" = %d", ret);
	return ret;
}

/*
<<<<<<< HEAD
 * find out if an object is in use or not
=======
 * Find out if an object is in use or not
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * - called only by cache manager daemon
 * - returns -EBUSY or 0 to indicate whether an object is in use or not
 */
int cachefiles_check_in_use(struct cachefiles_cache *cache, struct dentry *dir,
			    char *filename)
{
	struct dentry *victim;
<<<<<<< HEAD

	//_enter(",%*.*s/,%s",
	//       dir->d_name.len, dir->d_name.len, dir->d_name.name, filename);

	victim = cachefiles_check_active(cache, dir, filename);
	if (IS_ERR(victim))
		return PTR_ERR(victim);

	mutex_unlock(&dir->d_inode->i_mutex);
	dput(victim);
	//_leave(" = 0");
	return 0;
=======
	int ret = 0;

	victim = cachefiles_lookup_for_cull(cache, dir, filename);
	if (IS_ERR(victim))
		return PTR_ERR(victim);

	inode_unlock(d_inode(dir));
	dput(victim);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
