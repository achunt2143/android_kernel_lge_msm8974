<<<<<<< HEAD
/* FS-Cache interface to CacheFiles
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* FS-Cache interface to CacheFiles
 *
 * Copyright (C) 2021 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/slab.h>
#include <linux/mount.h>
<<<<<<< HEAD
#include "internal.h"

#define list_to_page(head) (list_entry((head)->prev, struct page, lru))

struct cachefiles_lookup_data {
	struct cachefiles_xattr	*auxdata;	/* auxiliary data */
	char			*key;		/* key path */
};

static int cachefiles_attr_changed(struct fscache_object *_object);

/*
 * allocate an object record for a cookie lookup and prepare the lookup data
 */
static struct fscache_object *cachefiles_alloc_object(
	struct fscache_cache *_cache,
	struct fscache_cookie *cookie)
{
	struct cachefiles_lookup_data *lookup_data;
	struct cachefiles_object *object;
	struct cachefiles_cache *cache;
	struct cachefiles_xattr *auxdata;
	unsigned keylen, auxlen;
	void *buffer;
	char *key;

	cache = container_of(_cache, struct cachefiles_cache, cache);

	_enter("{%s},%p,", cache->cache.identifier, cookie);

	lookup_data = kmalloc(sizeof(*lookup_data), GFP_KERNEL);
	if (!lookup_data)
		goto nomem_lookup_data;

	/* create a new object record and a temporary leaf image */
	object = kmem_cache_alloc(cachefiles_object_jar, GFP_KERNEL);
	if (!object)
		goto nomem_object;

	ASSERTCMP(object->backer, ==, NULL);

	BUG_ON(test_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags));
	atomic_set(&object->usage, 1);

	fscache_object_init(&object->fscache, cookie, &cache->cache);

	object->type = cookie->def->type;

	/* get hold of the raw key
	 * - stick the length on the front and leave space on the back for the
	 *   encoder
	 */
	buffer = kmalloc((2 + 512) + 3, GFP_KERNEL);
	if (!buffer)
		goto nomem_buffer;

	keylen = cookie->def->get_key(cookie->netfs_data, buffer + 2, 512);
	ASSERTCMP(keylen, <, 512);

	*(uint16_t *)buffer = keylen;
	((char *)buffer)[keylen + 2] = 0;
	((char *)buffer)[keylen + 3] = 0;
	((char *)buffer)[keylen + 4] = 0;

	/* turn the raw key into something that can work with as a filename */
	key = cachefiles_cook_key(buffer, keylen + 2, object->type);
	if (!key)
		goto nomem_key;

	/* get hold of the auxiliary data and prepend the object type */
	auxdata = buffer;
	auxlen = 0;
	if (cookie->def->get_aux) {
		auxlen = cookie->def->get_aux(cookie->netfs_data,
					      auxdata->data, 511);
		ASSERTCMP(auxlen, <, 511);
	}

	auxdata->len = auxlen + 1;
	auxdata->type = cookie->def->type;

	lookup_data->auxdata = auxdata;
	lookup_data->key = key;
	object->lookup_data = lookup_data;

	_leave(" = %p [%p]", &object->fscache, lookup_data);
	return &object->fscache;

nomem_key:
	kfree(buffer);
nomem_buffer:
	BUG_ON(test_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags));
	kmem_cache_free(cachefiles_object_jar, object);
	fscache_object_destroyed(&cache->cache);
nomem_object:
	kfree(lookup_data);
nomem_lookup_data:
	_leave(" = -ENOMEM");
	return ERR_PTR(-ENOMEM);
}

/*
 * attempt to look up the nominated node in this cache
 * - return -ETIMEDOUT to be scheduled again
 */
static int cachefiles_lookup_object(struct fscache_object *_object)
{
	struct cachefiles_lookup_data *lookup_data;
	struct cachefiles_object *parent, *object;
	struct cachefiles_cache *cache;
	const struct cred *saved_cred;
	int ret;

	_enter("{OBJ%x}", _object->debug_id);

	cache = container_of(_object->cache, struct cachefiles_cache, cache);
	parent = container_of(_object->parent,
			      struct cachefiles_object, fscache);
	object = container_of(_object, struct cachefiles_object, fscache);
	lookup_data = object->lookup_data;

	ASSERTCMP(lookup_data, !=, NULL);

	/* look up the key, creating any missing bits */
	cachefiles_begin_secure(cache, &saved_cred);
	ret = cachefiles_walk_to_object(parent, object,
					lookup_data->key,
					lookup_data->auxdata);
	cachefiles_end_secure(cache, saved_cred);

	/* polish off by setting the attributes of non-index files */
	if (ret == 0 &&
	    object->fscache.cookie->def->type != FSCACHE_COOKIE_TYPE_INDEX)
		cachefiles_attr_changed(&object->fscache);

	if (ret < 0 && ret != -ETIMEDOUT) {
		if (ret != -ENOBUFS)
			printk(KERN_WARNING
			       "CacheFiles: Lookup failed error %d\n", ret);
		fscache_object_lookup_error(&object->fscache);
	}

	_leave(" [%d]", ret);
	return ret;
}

/*
 * indication of lookup completion
 */
static void cachefiles_lookup_complete(struct fscache_object *_object)
{
	struct cachefiles_object *object;

	object = container_of(_object, struct cachefiles_object, fscache);

	_enter("{OBJ%x,%p}", object->fscache.debug_id, object->lookup_data);

	if (object->lookup_data) {
		kfree(object->lookup_data->key);
		kfree(object->lookup_data->auxdata);
		kfree(object->lookup_data);
		object->lookup_data = NULL;
	}
}

/*
 * increment the usage count on an inode object (may fail if unmounting)
 */
static
struct fscache_object *cachefiles_grab_object(struct fscache_object *_object)
{
	struct cachefiles_object *object =
		container_of(_object, struct cachefiles_object, fscache);

	_enter("{OBJ%x,%d}", _object->debug_id, atomic_read(&object->usage));

#ifdef CACHEFILES_DEBUG_SLAB
	ASSERT((atomic_read(&object->usage) & 0xffff0000) != 0x6b6b0000);
#endif

	atomic_inc(&object->usage);
	return &object->fscache;
}

/*
 * update the auxiliary data for an object object on disk
 */
static void cachefiles_update_object(struct fscache_object *_object)
{
	struct cachefiles_object *object;
	struct cachefiles_xattr *auxdata;
	struct cachefiles_cache *cache;
	struct fscache_cookie *cookie;
	const struct cred *saved_cred;
	unsigned auxlen;

	_enter("{OBJ%x}", _object->debug_id);

	object = container_of(_object, struct cachefiles_object, fscache);
	cache = container_of(object->fscache.cache, struct cachefiles_cache,
			     cache);
	cookie = object->fscache.cookie;

	if (!cookie->def->get_aux) {
		_leave(" [no aux]");
		return;
	}

	auxdata = kmalloc(2 + 512 + 3, GFP_KERNEL);
	if (!auxdata) {
		_leave(" [nomem]");
		return;
	}

	auxlen = cookie->def->get_aux(cookie->netfs_data, auxdata->data, 511);
	ASSERTCMP(auxlen, <, 511);

	auxdata->len = auxlen + 1;
	auxdata->type = cookie->def->type;

	cachefiles_begin_secure(cache, &saved_cred);
	cachefiles_update_object_xattr(object, auxdata);
	cachefiles_end_secure(cache, saved_cred);
	kfree(auxdata);
	_leave("");
}

/*
 * discard the resources pinned by an object and effect retirement if
 * requested
 */
static void cachefiles_drop_object(struct fscache_object *_object)
{
	struct cachefiles_object *object;
	struct cachefiles_cache *cache;
	const struct cred *saved_cred;

	ASSERT(_object);

	object = container_of(_object, struct cachefiles_object, fscache);

	_enter("{OBJ%x,%d}",
	       object->fscache.debug_id, atomic_read(&object->usage));

	cache = container_of(object->fscache.cache,
			     struct cachefiles_cache, cache);

#ifdef CACHEFILES_DEBUG_SLAB
	ASSERT((atomic_read(&object->usage) & 0xffff0000) != 0x6b6b0000);
#endif

	/* delete retired objects */
	if (object->fscache.state == FSCACHE_OBJECT_RECYCLING &&
	    _object != cache->cache.fsdef
	    ) {
		_debug("- retire object OBJ%x", object->fscache.debug_id);
		cachefiles_begin_secure(cache, &saved_cred);
		cachefiles_delete_object(cache, object);
		cachefiles_end_secure(cache, saved_cred);
	}

	/* close the filesystem stuff attached to the object */
	if (object->backer != object->dentry)
		dput(object->backer);
	object->backer = NULL;

	/* note that the object is now inactive */
	if (test_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags)) {
		write_lock(&cache->active_lock);
		if (!test_and_clear_bit(CACHEFILES_OBJECT_ACTIVE,
					&object->flags))
			BUG();
		rb_erase(&object->active_node, &cache->active_nodes);
		wake_up_bit(&object->flags, CACHEFILES_OBJECT_ACTIVE);
		write_unlock(&cache->active_lock);
	}

	dput(object->dentry);
	object->dentry = NULL;

	_leave("");
=======
#include <linux/xattr.h>
#include <linux/file.h>
#include <linux/falloc.h>
#include <trace/events/fscache.h>
#include "internal.h"

static atomic_t cachefiles_object_debug_id;

/*
 * Allocate a cache object record.
 */
static
struct cachefiles_object *cachefiles_alloc_object(struct fscache_cookie *cookie)
{
	struct fscache_volume *vcookie = cookie->volume;
	struct cachefiles_volume *volume = vcookie->cache_priv;
	struct cachefiles_object *object;

	_enter("{%s},%x,", vcookie->key, cookie->debug_id);

	object = kmem_cache_zalloc(cachefiles_object_jar, GFP_KERNEL);
	if (!object)
		return NULL;

	if (cachefiles_ondemand_init_obj_info(object, volume)) {
		kmem_cache_free(cachefiles_object_jar, object);
		return NULL;
	}

	refcount_set(&object->ref, 1);

	spin_lock_init(&object->lock);
	INIT_LIST_HEAD(&object->cache_link);
	object->volume = volume;
	object->debug_id = atomic_inc_return(&cachefiles_object_debug_id);
	object->cookie = fscache_get_cookie(cookie, fscache_cookie_get_attach_object);

	fscache_count_object(vcookie->cache);
	trace_cachefiles_ref(object->debug_id, cookie->debug_id, 1,
			     cachefiles_obj_new);
	return object;
}

/*
 * Note that an object has been seen.
 */
void cachefiles_see_object(struct cachefiles_object *object,
			   enum cachefiles_obj_ref_trace why)
{
	trace_cachefiles_ref(object->debug_id, object->cookie->debug_id,
			     refcount_read(&object->ref), why);
}

/*
 * Increment the usage count on an object;
 */
struct cachefiles_object *cachefiles_grab_object(struct cachefiles_object *object,
						 enum cachefiles_obj_ref_trace why)
{
	int r;

	__refcount_inc(&object->ref, &r);
	trace_cachefiles_ref(object->debug_id, object->cookie->debug_id, r, why);
	return object;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * dispose of a reference to an object
 */
<<<<<<< HEAD
static void cachefiles_put_object(struct fscache_object *_object)
{
	struct cachefiles_object *object;
	struct fscache_cache *cache;

	ASSERT(_object);

	object = container_of(_object, struct cachefiles_object, fscache);

	_enter("{OBJ%x,%d}",
	       object->fscache.debug_id, atomic_read(&object->usage));

#ifdef CACHEFILES_DEBUG_SLAB
	ASSERT((atomic_read(&object->usage) & 0xffff0000) != 0x6b6b0000);
#endif

	ASSERTIFCMP(object->fscache.parent,
		    object->fscache.parent->n_children, >, 0);

	if (atomic_dec_and_test(&object->usage)) {
		_debug("- kill object OBJ%x", object->fscache.debug_id);

		ASSERT(!test_bit(CACHEFILES_OBJECT_ACTIVE, &object->flags));
		ASSERTCMP(object->fscache.parent, ==, NULL);
		ASSERTCMP(object->backer, ==, NULL);
		ASSERTCMP(object->dentry, ==, NULL);
		ASSERTCMP(object->fscache.n_ops, ==, 0);
		ASSERTCMP(object->fscache.n_children, ==, 0);

		if (object->lookup_data) {
			kfree(object->lookup_data->key);
			kfree(object->lookup_data->auxdata);
			kfree(object->lookup_data);
			object->lookup_data = NULL;
		}

		cache = object->fscache.cache;
		fscache_object_destroy(&object->fscache);
		kmem_cache_free(cachefiles_object_jar, object);
		fscache_object_destroyed(cache);
=======
void cachefiles_put_object(struct cachefiles_object *object,
			   enum cachefiles_obj_ref_trace why)
{
	unsigned int object_debug_id = object->debug_id;
	unsigned int cookie_debug_id = object->cookie->debug_id;
	struct fscache_cache *cache;
	bool done;
	int r;

	done = __refcount_dec_and_test(&object->ref, &r);
	trace_cachefiles_ref(object_debug_id, cookie_debug_id, r, why);
	if (done) {
		_debug("- kill object OBJ%x", object_debug_id);

		ASSERTCMP(object->file, ==, NULL);

		kfree(object->d_name);
		cachefiles_ondemand_deinit_obj_info(object);
		cache = object->volume->cache->cache;
		fscache_put_cookie(object->cookie, fscache_cookie_put_object);
		object->cookie = NULL;
		kmem_cache_free(cachefiles_object_jar, object);
		fscache_uncount_object(cache);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	_leave("");
}

/*
<<<<<<< HEAD
 * sync a cache
 */
static void cachefiles_sync_cache(struct fscache_cache *_cache)
{
	struct cachefiles_cache *cache;
	const struct cred *saved_cred;
	int ret;

	_enter("%p", _cache);

	cache = container_of(_cache, struct cachefiles_cache, cache);

	/* make sure all pages pinned by operations on behalf of the netfs are
	 * written to disc */
	cachefiles_begin_secure(cache, &saved_cred);
	down_read(&cache->mnt->mnt_sb->s_umount);
	ret = sync_filesystem(cache->mnt->mnt_sb);
	up_read(&cache->mnt->mnt_sb->s_umount);
	cachefiles_end_secure(cache, saved_cred);

	if (ret == -EIO)
		cachefiles_io_error(cache,
				    "Attempt to sync backing fs superblock"
				    " returned error %d",
				    ret);
}

/*
 * notification the attributes on an object have changed
 * - called with reads/writes excluded by FS-Cache
 */
static int cachefiles_attr_changed(struct fscache_object *_object)
{
	struct cachefiles_object *object;
	struct cachefiles_cache *cache;
	const struct cred *saved_cred;
	struct iattr newattrs;
=======
 * Adjust the size of a cache file if necessary to match the DIO size.  We keep
 * the EOF marker a multiple of DIO blocks so that we don't fall back to doing
 * non-DIO for a partial block straddling the EOF, but we also have to be
 * careful of someone expanding the file and accidentally accreting the
 * padding.
 */
static int cachefiles_adjust_size(struct cachefiles_object *object)
{
	struct iattr newattrs;
	struct file *file = object->file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	uint64_t ni_size;
	loff_t oi_size;
	int ret;

<<<<<<< HEAD
	_object->cookie->def->get_attr(_object->cookie->netfs_data, &ni_size);

	_enter("{OBJ%x},[%llu]",
	       _object->debug_id, (unsigned long long) ni_size);

	object = container_of(_object, struct cachefiles_object, fscache);
	cache = container_of(object->fscache.cache,
			     struct cachefiles_cache, cache);

	if (ni_size == object->i_size)
		return 0;

	if (!object->backer)
		return -ENOBUFS;

	ASSERT(S_ISREG(object->backer->d_inode->i_mode));

	fscache_set_store_limit(&object->fscache, ni_size);

	oi_size = i_size_read(object->backer->d_inode);
	if (oi_size == ni_size)
		return 0;

	cachefiles_begin_secure(cache, &saved_cred);
	mutex_lock(&object->backer->d_inode->i_mutex);
=======
	ni_size = object->cookie->object_size;
	ni_size = round_up(ni_size, CACHEFILES_DIO_BLOCK_SIZE);

	_enter("{OBJ%x},[%llu]",
	       object->debug_id, (unsigned long long) ni_size);

	if (!file)
		return -ENOBUFS;

	oi_size = i_size_read(file_inode(file));
	if (oi_size == ni_size)
		return 0;

	inode_lock(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* if there's an extension to a partial page at the end of the backing
	 * file, we need to discard the partial page so that we pick up new
	 * data after it */
	if (oi_size & ~PAGE_MASK && ni_size > oi_size) {
		_debug("discard tail %llx", oi_size);
		newattrs.ia_valid = ATTR_SIZE;
		newattrs.ia_size = oi_size & PAGE_MASK;
<<<<<<< HEAD
		ret = notify_change(object->backer, &newattrs);
=======
		ret = cachefiles_inject_remove_error();
		if (ret == 0)
			ret = notify_change(&nop_mnt_idmap, file->f_path.dentry,
					    &newattrs, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			goto truncate_failed;
	}

	newattrs.ia_valid = ATTR_SIZE;
	newattrs.ia_size = ni_size;
<<<<<<< HEAD
	ret = notify_change(object->backer, &newattrs);

truncate_failed:
	mutex_unlock(&object->backer->d_inode->i_mutex);
	cachefiles_end_secure(cache, saved_cred);

	if (ret == -EIO) {
		fscache_set_store_limit(&object->fscache, 0);
=======
	ret = cachefiles_inject_write_error();
	if (ret == 0)
		ret = notify_change(&nop_mnt_idmap, file->f_path.dentry,
				    &newattrs, NULL);

truncate_failed:
	inode_unlock(file_inode(file));

	if (ret < 0)
		trace_cachefiles_io_error(NULL, file_inode(file), ret,
					  cachefiles_trace_notify_change_error);
	if (ret == -EIO) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cachefiles_io_error_obj(object, "Size set failed");
		ret = -ENOBUFS;
	}

	_leave(" = %d", ret);
	return ret;
}

/*
<<<<<<< HEAD
 * dissociate a cache from all the pages it was backing
 */
static void cachefiles_dissociate_pages(struct fscache_cache *cache)
{
	_enter("");
=======
 * Attempt to look up the nominated node in this cache
 */
static bool cachefiles_lookup_cookie(struct fscache_cookie *cookie)
{
	struct cachefiles_object *object;
	struct cachefiles_cache *cache = cookie->volume->cache->cache_priv;
	const struct cred *saved_cred;
	bool success;

	object = cachefiles_alloc_object(cookie);
	if (!object)
		goto fail;

	_enter("{OBJ%x}", object->debug_id);

	if (!cachefiles_cook_key(object))
		goto fail_put;

	cookie->cache_priv = object;

	cachefiles_begin_secure(cache, &saved_cred);

	success = cachefiles_look_up_object(object);
	if (!success)
		goto fail_withdraw;

	cachefiles_see_object(object, cachefiles_obj_see_lookup_cookie);

	spin_lock(&cache->object_list_lock);
	list_add(&object->cache_link, &cache->object_list);
	spin_unlock(&cache->object_list_lock);
	cachefiles_adjust_size(object);

	cachefiles_end_secure(cache, saved_cred);
	_leave(" = t");
	return true;

fail_withdraw:
	cachefiles_end_secure(cache, saved_cred);
	cachefiles_see_object(object, cachefiles_obj_see_lookup_failed);
	fscache_caching_failed(cookie);
	_debug("failed c=%08x o=%08x", cookie->debug_id, object->debug_id);
	/* The caller holds an access count on the cookie, so we need them to
	 * drop it before we can withdraw the object.
	 */
	return false;

fail_put:
	cachefiles_put_object(object, cachefiles_obj_put_alloc_fail);
fail:
	return false;
}

/*
 * Shorten the backing object to discard any dirty data and free up
 * any unused granules.
 */
static bool cachefiles_shorten_object(struct cachefiles_object *object,
				      struct file *file, loff_t new_size)
{
	struct cachefiles_cache *cache = object->volume->cache;
	struct inode *inode = file_inode(file);
	loff_t i_size, dio_size;
	int ret;

	dio_size = round_up(new_size, CACHEFILES_DIO_BLOCK_SIZE);
	i_size = i_size_read(inode);

	trace_cachefiles_trunc(object, inode, i_size, dio_size,
			       cachefiles_trunc_shrink);
	ret = cachefiles_inject_remove_error();
	if (ret == 0)
		ret = vfs_truncate(&file->f_path, dio_size);
	if (ret < 0) {
		trace_cachefiles_io_error(object, file_inode(file), ret,
					  cachefiles_trace_trunc_error);
		cachefiles_io_error_obj(object, "Trunc-to-size failed %d", ret);
		cachefiles_remove_object_xattr(cache, object, file->f_path.dentry);
		return false;
	}

	if (new_size < dio_size) {
		trace_cachefiles_trunc(object, inode, dio_size, new_size,
				       cachefiles_trunc_dio_adjust);
		ret = cachefiles_inject_write_error();
		if (ret == 0)
			ret = vfs_fallocate(file, FALLOC_FL_ZERO_RANGE,
					    new_size, dio_size - new_size);
		if (ret < 0) {
			trace_cachefiles_io_error(object, file_inode(file), ret,
						  cachefiles_trace_fallocate_error);
			cachefiles_io_error_obj(object, "Trunc-to-dio-size failed %d", ret);
			cachefiles_remove_object_xattr(cache, object, file->f_path.dentry);
			return false;
		}
	}

	return true;
}

/*
 * Resize the backing object.
 */
static void cachefiles_resize_cookie(struct netfs_cache_resources *cres,
				     loff_t new_size)
{
	struct cachefiles_object *object = cachefiles_cres_object(cres);
	struct cachefiles_cache *cache = object->volume->cache;
	struct fscache_cookie *cookie = object->cookie;
	const struct cred *saved_cred;
	struct file *file = cachefiles_cres_file(cres);
	loff_t old_size = cookie->object_size;

	_enter("%llu->%llu", old_size, new_size);

	if (new_size < old_size) {
		cachefiles_begin_secure(cache, &saved_cred);
		cachefiles_shorten_object(object, file, new_size);
		cachefiles_end_secure(cache, saved_cred);
		object->cookie->object_size = new_size;
		return;
	}

	/* The file is being expanded.  We don't need to do anything
	 * particularly.  cookie->initial_size doesn't change and so the point
	 * at which we have to download before doesn't change.
	 */
	cookie->object_size = new_size;
}

/*
 * Commit changes to the object as we drop it.
 */
static void cachefiles_commit_object(struct cachefiles_object *object,
				     struct cachefiles_cache *cache)
{
	bool update = false;

	if (test_and_clear_bit(FSCACHE_COOKIE_LOCAL_WRITE, &object->cookie->flags))
		update = true;
	if (test_and_clear_bit(FSCACHE_COOKIE_NEEDS_UPDATE, &object->cookie->flags))
		update = true;
	if (update)
		cachefiles_set_object_xattr(object);

	if (test_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags))
		cachefiles_commit_tmpfile(cache, object);
}

/*
 * Finalise and object and close the VFS structs that we have.
 */
static void cachefiles_clean_up_object(struct cachefiles_object *object,
				       struct cachefiles_cache *cache)
{
	if (test_bit(FSCACHE_COOKIE_RETIRED, &object->cookie->flags)) {
		if (!test_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags)) {
			cachefiles_see_object(object, cachefiles_obj_see_clean_delete);
			_debug("- inval object OBJ%x", object->debug_id);
			cachefiles_delete_object(object, FSCACHE_OBJECT_WAS_RETIRED);
		} else {
			cachefiles_see_object(object, cachefiles_obj_see_clean_drop_tmp);
			_debug("- inval object OBJ%x tmpfile", object->debug_id);
		}
	} else {
		cachefiles_see_object(object, cachefiles_obj_see_clean_commit);
		cachefiles_commit_object(object, cache);
	}

	cachefiles_unmark_inode_in_use(object, object->file);
	if (object->file) {
		fput(object->file);
		object->file = NULL;
	}
}

/*
 * Withdraw caching for a cookie.
 */
static void cachefiles_withdraw_cookie(struct fscache_cookie *cookie)
{
	struct cachefiles_object *object = cookie->cache_priv;
	struct cachefiles_cache *cache = object->volume->cache;
	const struct cred *saved_cred;

	_enter("o=%x", object->debug_id);
	cachefiles_see_object(object, cachefiles_obj_see_withdraw_cookie);

	if (!list_empty(&object->cache_link)) {
		spin_lock(&cache->object_list_lock);
		cachefiles_see_object(object, cachefiles_obj_see_withdrawal);
		list_del_init(&object->cache_link);
		spin_unlock(&cache->object_list_lock);
	}

	cachefiles_ondemand_clean_object(object);

	if (object->file) {
		cachefiles_begin_secure(cache, &saved_cred);
		cachefiles_clean_up_object(object, cache);
		cachefiles_end_secure(cache, saved_cred);
	}

	cookie->cache_priv = NULL;
	cachefiles_put_object(object, cachefiles_obj_put_detach);
}

/*
 * Invalidate the storage associated with a cookie.
 */
static bool cachefiles_invalidate_cookie(struct fscache_cookie *cookie)
{
	struct cachefiles_object *object = cookie->cache_priv;
	struct file *new_file, *old_file;
	bool old_tmpfile;

	_enter("o=%x,[%llu]", object->debug_id, object->cookie->object_size);

	old_tmpfile = test_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags);

	if (!object->file) {
		fscache_resume_after_invalidation(cookie);
		_leave(" = t [light]");
		return true;
	}

	new_file = cachefiles_create_tmpfile(object);
	if (IS_ERR(new_file))
		goto failed;

	/* Substitute the VFS target */
	_debug("sub");
	spin_lock(&object->lock);

	old_file = object->file;
	object->file = new_file;
	object->content_info = CACHEFILES_CONTENT_NO_DATA;
	set_bit(CACHEFILES_OBJECT_USING_TMPFILE, &object->flags);
	set_bit(FSCACHE_COOKIE_NEEDS_UPDATE, &object->cookie->flags);

	spin_unlock(&object->lock);
	_debug("subbed");

	/* Allow I/O to take place again */
	fscache_resume_after_invalidation(cookie);

	if (old_file) {
		if (!old_tmpfile) {
			struct cachefiles_volume *volume = object->volume;
			struct dentry *fan = volume->fanout[(u8)cookie->key_hash];

			inode_lock_nested(d_inode(fan), I_MUTEX_PARENT);
			cachefiles_bury_object(volume->cache, object, fan,
					       old_file->f_path.dentry,
					       FSCACHE_OBJECT_INVALIDATED);
		}
		fput(old_file);
	}

	_leave(" = t");
	return true;

failed:
	_leave(" = f");
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct fscache_cache_ops cachefiles_cache_ops = {
	.name			= "cachefiles",
<<<<<<< HEAD
	.alloc_object		= cachefiles_alloc_object,
	.lookup_object		= cachefiles_lookup_object,
	.lookup_complete	= cachefiles_lookup_complete,
	.grab_object		= cachefiles_grab_object,
	.update_object		= cachefiles_update_object,
	.drop_object		= cachefiles_drop_object,
	.put_object		= cachefiles_put_object,
	.sync_cache		= cachefiles_sync_cache,
	.attr_changed		= cachefiles_attr_changed,
	.read_or_alloc_page	= cachefiles_read_or_alloc_page,
	.read_or_alloc_pages	= cachefiles_read_or_alloc_pages,
	.allocate_page		= cachefiles_allocate_page,
	.allocate_pages		= cachefiles_allocate_pages,
	.write_page		= cachefiles_write_page,
	.uncache_page		= cachefiles_uncache_page,
	.dissociate_pages	= cachefiles_dissociate_pages,
=======
	.acquire_volume		= cachefiles_acquire_volume,
	.free_volume		= cachefiles_free_volume,
	.lookup_cookie		= cachefiles_lookup_cookie,
	.withdraw_cookie	= cachefiles_withdraw_cookie,
	.invalidate_cookie	= cachefiles_invalidate_cookie,
	.begin_operation	= cachefiles_begin_operation,
	.resize_cookie		= cachefiles_resize_cookie,
	.prepare_to_write	= cachefiles_prepare_to_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
