<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* AFS volume management
 *
 * Copyright (C) 2002, 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/sched.h>
#include "internal.h"

static const char *afs_voltypes[] = { "R/W", "R/O", "BAK" };

/*
 * lookup a volume by name
 * - this can be one of the following:
=======
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include "internal.h"

static unsigned __read_mostly afs_volume_record_life = 60 * 60;

static void afs_destroy_volume(struct work_struct *work);

/*
 * Insert a volume into a cell.  If there's an existing volume record, that is
 * returned instead with a ref held.
 */
static struct afs_volume *afs_insert_volume_into_cell(struct afs_cell *cell,
						      struct afs_volume *volume)
{
	struct afs_volume *p;
	struct rb_node *parent = NULL, **pp;

	write_seqlock(&cell->volume_lock);

	pp = &cell->volumes.rb_node;
	while (*pp) {
		parent = *pp;
		p = rb_entry(parent, struct afs_volume, cell_node);
		if (p->vid < volume->vid) {
			pp = &(*pp)->rb_left;
		} else if (p->vid > volume->vid) {
			pp = &(*pp)->rb_right;
		} else {
			if (afs_try_get_volume(p, afs_volume_trace_get_cell_insert)) {
				volume = p;
				goto found;
			}

			set_bit(AFS_VOLUME_RM_TREE, &volume->flags);
			rb_replace_node_rcu(&p->cell_node, &volume->cell_node, &cell->volumes);
		}
	}

	rb_link_node_rcu(&volume->cell_node, parent, pp);
	rb_insert_color(&volume->cell_node, &cell->volumes);
	hlist_add_head_rcu(&volume->proc_link, &cell->proc_volumes);

found:
	write_sequnlock(&cell->volume_lock);
	return volume;

}

static void afs_remove_volume_from_cell(struct afs_volume *volume)
{
	struct afs_cell *cell = volume->cell;

	if (!hlist_unhashed(&volume->proc_link)) {
		trace_afs_volume(volume->vid, refcount_read(&cell->ref),
				 afs_volume_trace_remove);
		write_seqlock(&cell->volume_lock);
		hlist_del_rcu(&volume->proc_link);
		if (!test_and_set_bit(AFS_VOLUME_RM_TREE, &volume->flags))
			rb_erase(&volume->cell_node, &cell->volumes);
		write_sequnlock(&cell->volume_lock);
	}
}

/*
 * Allocate a volume record and load it up from a vldb record.
 */
static struct afs_volume *afs_alloc_volume(struct afs_fs_context *params,
					   struct afs_vldb_entry *vldb,
					   struct afs_server_list **_slist)
{
	struct afs_server_list *slist;
	struct afs_volume *volume;
	int ret = -ENOMEM, i;

	volume = kzalloc(sizeof(struct afs_volume), GFP_KERNEL);
	if (!volume)
		goto error_0;

	volume->vid		= vldb->vid[params->type];
	volume->update_at	= ktime_get_real_seconds() + afs_volume_record_life;
	volume->cell		= afs_get_cell(params->cell, afs_cell_trace_get_vol);
	volume->type		= params->type;
	volume->type_force	= params->force;
	volume->name_len	= vldb->name_len;
	volume->creation_time	= TIME64_MIN;
	volume->update_time	= TIME64_MIN;

	refcount_set(&volume->ref, 1);
	INIT_HLIST_NODE(&volume->proc_link);
	INIT_WORK(&volume->destructor, afs_destroy_volume);
	rwlock_init(&volume->servers_lock);
	mutex_init(&volume->volsync_lock);
	mutex_init(&volume->cb_check_lock);
	rwlock_init(&volume->cb_v_break_lock);
	INIT_LIST_HEAD(&volume->open_mmaps);
	init_rwsem(&volume->open_mmaps_lock);
	memcpy(volume->name, vldb->name, vldb->name_len + 1);

	for (i = 0; i < AFS_MAXTYPES; i++)
		volume->vids[i] = vldb->vid[i];

	slist = afs_alloc_server_list(volume, params->key, vldb);
	if (IS_ERR(slist)) {
		ret = PTR_ERR(slist);
		goto error_1;
	}

	*_slist = slist;
	rcu_assign_pointer(volume->servers, slist);
	trace_afs_volume(volume->vid, 1, afs_volume_trace_alloc);
	return volume;

error_1:
	afs_put_cell(volume->cell, afs_cell_trace_put_vol);
	kfree(volume);
error_0:
	return ERR_PTR(ret);
}

/*
 * Look up or allocate a volume record.
 */
static struct afs_volume *afs_lookup_volume(struct afs_fs_context *params,
					    struct afs_vldb_entry *vldb)
{
	struct afs_server_list *slist;
	struct afs_volume *candidate, *volume;

	candidate = afs_alloc_volume(params, vldb, &slist);
	if (IS_ERR(candidate))
		return candidate;

	volume = afs_insert_volume_into_cell(params->cell, candidate);
	if (volume == candidate)
		afs_attach_volume_to_servers(volume, slist);
	else
		afs_put_volume(candidate, afs_volume_trace_put_cell_dup);
	return volume;
}

/*
 * Look up a VLDB record for a volume.
 */
static struct afs_vldb_entry *afs_vl_lookup_vldb(struct afs_cell *cell,
						 struct key *key,
						 const char *volname,
						 size_t volnamesz)
{
	struct afs_vldb_entry *vldb = ERR_PTR(-EDESTADDRREQ);
	struct afs_vl_cursor vc;
	int ret;

	if (!afs_begin_vlserver_operation(&vc, cell, key))
		return ERR_PTR(-ERESTARTSYS);

	while (afs_select_vlserver(&vc)) {
		vldb = afs_vl_get_entry_by_name_u(&vc, volname, volnamesz);
	}

	ret = afs_end_vlserver_operation(&vc);
	return ret < 0 ? ERR_PTR(ret) : vldb;
}

/*
 * Look up a volume in the VL server and create a candidate volume record for
 * it.
 *
 * The volume name can be one of the following:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	"%[cell:]volume[.]"		R/W volume
 *	"#[cell:]volume[.]"		R/O or R/W volume (rwparent=0),
 *					 or R/W (rwparent=1) volume
 *	"%[cell:]volume.readonly"	R/O volume
 *	"#[cell:]volume.readonly"	R/O volume
 *	"%[cell:]volume.backup"		Backup volume
 *	"#[cell:]volume.backup"		Backup volume
 *
 * The cell name is optional, and defaults to the current cell.
 *
 * See "The Rules of Mount Point Traversal" in Chapter 5 of the AFS SysAdmin
 * Guide
 * - Rule 1: Explicit type suffix forces access of that type or nothing
 *           (no suffix, then use Rule 2 & 3)
 * - Rule 2: If parent volume is R/O, then mount R/O volume by preference, R/W
 *           if not available
 * - Rule 3: If parent volume is R/W, then only mount R/W volume unless
 *           explicitly told otherwise
 */
<<<<<<< HEAD
struct afs_volume *afs_volume_lookup(struct afs_mount_params *params)
{
	struct afs_vlocation *vlocation = NULL;
	struct afs_volume *volume = NULL;
	struct afs_server *server = NULL;
	char srvtmask;
	int ret, loop;

	_enter("{%*.*s,%d}",
	       params->volnamesz, params->volnamesz, params->volname, params->rwpath);

	/* lookup the volume location record */
	vlocation = afs_vlocation_lookup(params->cell, params->key,
					 params->volname, params->volnamesz);
	if (IS_ERR(vlocation)) {
		ret = PTR_ERR(vlocation);
		vlocation = NULL;
		goto error;
	}

	/* make the final decision on the type we want */
	ret = -ENOMEDIUM;
	if (params->force && !(vlocation->vldb.vidmask & (1 << params->type)))
		goto error;

	srvtmask = 0;
	for (loop = 0; loop < vlocation->vldb.nservers; loop++)
		srvtmask |= vlocation->vldb.srvtmask[loop];

	if (params->force) {
		if (!(srvtmask & (1 << params->type)))
			goto error;
	} else if (srvtmask & AFS_VOL_VTM_RO) {
		params->type = AFSVL_ROVOL;
	} else if (srvtmask & AFS_VOL_VTM_RW) {
=======
struct afs_volume *afs_create_volume(struct afs_fs_context *params)
{
	struct afs_vldb_entry *vldb;
	struct afs_volume *volume;
	unsigned long type_mask = 1UL << params->type;

	vldb = afs_vl_lookup_vldb(params->cell, params->key,
				  params->volname, params->volnamesz);
	if (IS_ERR(vldb))
		return ERR_CAST(vldb);

	if (test_bit(AFS_VLDB_QUERY_ERROR, &vldb->flags)) {
		volume = ERR_PTR(vldb->error);
		goto error;
	}

	/* Make the final decision on the type we want */
	volume = ERR_PTR(-ENOMEDIUM);
	if (params->force) {
		if (!(vldb->flags & type_mask))
			goto error;
	} else if (test_bit(AFS_VLDB_HAS_RO, &vldb->flags)) {
		params->type = AFSVL_ROVOL;
	} else if (test_bit(AFS_VLDB_HAS_RW, &vldb->flags)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		params->type = AFSVL_RWVOL;
	} else {
		goto error;
	}

<<<<<<< HEAD
	down_write(&params->cell->vl_sem);

	/* is the volume already active? */
	if (vlocation->vols[params->type]) {
		/* yes - re-use it */
		volume = vlocation->vols[params->type];
		afs_get_volume(volume);
		goto success;
	}

	/* create a new volume record */
	_debug("creating new volume record");

	ret = -ENOMEM;
	volume = kzalloc(sizeof(struct afs_volume), GFP_KERNEL);
	if (!volume)
		goto error_up;

	atomic_set(&volume->usage, 1);
	volume->type		= params->type;
	volume->type_force	= params->force;
	volume->cell		= params->cell;
	volume->vid		= vlocation->vldb.vid[params->type];

	ret = bdi_setup_and_register(&volume->bdi, "afs", BDI_CAP_MAP_COPY);
	if (ret)
		goto error_bdi;

	init_rwsem(&volume->server_sem);

	/* look up all the applicable server records */
	for (loop = 0; loop < 8; loop++) {
		if (vlocation->vldb.srvtmask[loop] & (1 << volume->type)) {
			server = afs_lookup_server(
			       volume->cell, &vlocation->vldb.servers[loop]);
			if (IS_ERR(server)) {
				ret = PTR_ERR(server);
				goto error_discard;
			}

			volume->servers[volume->nservers] = server;
			volume->nservers++;
		}
	}

	/* attach the cache and volume location */
#ifdef CONFIG_AFS_FSCACHE
	volume->cache = fscache_acquire_cookie(vlocation->cache,
					       &afs_volume_cache_index_def,
					       volume);
#endif
	afs_get_vlocation(vlocation);
	volume->vlocation = vlocation;

	vlocation->vols[volume->type] = volume;

success:
	_debug("kAFS selected %s volume %08x",
	       afs_voltypes[volume->type], volume->vid);
	up_write(&params->cell->vl_sem);
	afs_put_vlocation(vlocation);
	_leave(" = %p", volume);
	return volume;

	/* clean up */
error_up:
	up_write(&params->cell->vl_sem);
error:
	afs_put_vlocation(vlocation);
	_leave(" = %d", ret);
	return ERR_PTR(ret);

error_discard:
	bdi_destroy(&volume->bdi);
error_bdi:
	up_write(&params->cell->vl_sem);

	for (loop = volume->nservers - 1; loop >= 0; loop--)
		afs_put_server(volume->servers[loop]);

	kfree(volume);
	goto error;
}

/*
 * destroy a volume record
 */
void afs_put_volume(struct afs_volume *volume)
{
	struct afs_vlocation *vlocation;
	int loop;

	if (!volume)
		return;

	_enter("%p", volume);

	ASSERTCMP(atomic_read(&volume->usage), >, 0);

	vlocation = volume->vlocation;

	/* to prevent a race, the decrement and the dequeue must be effectively
	 * atomic */
	down_write(&vlocation->cell->vl_sem);

	if (likely(!atomic_dec_and_test(&volume->usage))) {
		up_write(&vlocation->cell->vl_sem);
		_leave("");
		return;
	}

	vlocation->vols[volume->type] = NULL;

	up_write(&vlocation->cell->vl_sem);

	/* finish cleaning up the volume */
#ifdef CONFIG_AFS_FSCACHE
	fscache_relinquish_cookie(volume->cache, 0);
#endif
	afs_put_vlocation(vlocation);

	for (loop = volume->nservers - 1; loop >= 0; loop--)
		afs_put_server(volume->servers[loop]);

	bdi_destroy(&volume->bdi);
	kfree(volume);
=======
	volume = afs_lookup_volume(params, vldb);

error:
	kfree(vldb);
	return volume;
}

/*
 * Destroy a volume record
 */
static void afs_destroy_volume(struct work_struct *work)
{
	struct afs_volume *volume = container_of(work, struct afs_volume, destructor);
	struct afs_server_list *slist = rcu_access_pointer(volume->servers);

	_enter("%p", volume);

#ifdef CONFIG_AFS_FSCACHE
	ASSERTCMP(volume->cache, ==, NULL);
#endif

	afs_detach_volume_from_servers(volume, slist);
	afs_remove_volume_from_cell(volume);
	afs_put_serverlist(volume->cell->net, slist);
	afs_put_cell(volume->cell, afs_cell_trace_put_vol);
	trace_afs_volume(volume->vid, refcount_read(&volume->ref),
			 afs_volume_trace_free);
	kfree_rcu(volume, rcu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	_leave(" [destroyed]");
}

/*
<<<<<<< HEAD
 * pick a server to use to try accessing this volume
 * - returns with an elevated usage count on the server chosen
 */
struct afs_server *afs_volume_pick_fileserver(struct afs_vnode *vnode)
{
	struct afs_volume *volume = vnode->volume;
	struct afs_server *server;
	int ret, state, loop;

	_enter("%s", volume->vlocation->vldb.name);

	/* stick with the server we're already using if we can */
	if (vnode->server && vnode->server->fs_state == 0) {
		afs_get_server(vnode->server);
		_leave(" = %p [current]", vnode->server);
		return vnode->server;
	}

	down_read(&volume->server_sem);

	/* handle the no-server case */
	if (volume->nservers == 0) {
		ret = volume->rjservers ? -ENOMEDIUM : -ESTALE;
		up_read(&volume->server_sem);
		_leave(" = %d [no servers]", ret);
		return ERR_PTR(ret);
	}

	/* basically, just search the list for the first live server and use
	 * that */
	ret = 0;
	for (loop = 0; loop < volume->nservers; loop++) {
		server = volume->servers[loop];
		state = server->fs_state;

		_debug("consider %d [%d]", loop, state);

		switch (state) {
			/* found an apparently healthy server */
		case 0:
			afs_get_server(server);
			up_read(&volume->server_sem);
			_leave(" = %p (picked %08x)",
			       server, ntohl(server->addr.s_addr));
			return server;

		case -ENETUNREACH:
			if (ret == 0)
				ret = state;
			break;

		case -EHOSTUNREACH:
			if (ret == 0 ||
			    ret == -ENETUNREACH)
				ret = state;
			break;

		case -ECONNREFUSED:
			if (ret == 0 ||
			    ret == -ENETUNREACH ||
			    ret == -EHOSTUNREACH)
				ret = state;
			break;

		default:
		case -EREMOTEIO:
			if (ret == 0 ||
			    ret == -ENETUNREACH ||
			    ret == -EHOSTUNREACH ||
			    ret == -ECONNREFUSED)
				ret = state;
			break;
		}
	}

	/* no available servers
	 * - TODO: handle the no active servers case better
	 */
	up_read(&volume->server_sem);
	_leave(" = %d", ret);
	return ERR_PTR(ret);
}

/*
 * release a server after use
 * - releases the ref on the server struct that was acquired by picking
 * - records result of using a particular server to access a volume
 * - return 0 to try again, 1 if okay or to issue error
 * - the caller must release the server struct if result was 0
 */
int afs_volume_release_fileserver(struct afs_vnode *vnode,
				  struct afs_server *server,
				  int result)
{
	struct afs_volume *volume = vnode->volume;
	unsigned loop;

	_enter("%s,%08x,%d",
	       volume->vlocation->vldb.name, ntohl(server->addr.s_addr),
	       result);

	switch (result) {
		/* success */
	case 0:
		server->fs_act_jif = jiffies;
		server->fs_state = 0;
		_leave("");
		return 1;

		/* the fileserver denied all knowledge of the volume */
	case -ENOMEDIUM:
		server->fs_act_jif = jiffies;
		down_write(&volume->server_sem);

		/* firstly, find where the server is in the active list (if it
		 * is) */
		for (loop = 0; loop < volume->nservers; loop++)
			if (volume->servers[loop] == server)
				goto present;

		/* no longer there - may have been discarded by another op */
		goto try_next_server_upw;

	present:
		volume->nservers--;
		memmove(&volume->servers[loop],
			&volume->servers[loop + 1],
			sizeof(volume->servers[loop]) *
			(volume->nservers - loop));
		volume->servers[volume->nservers] = NULL;
		afs_put_server(server);
		volume->rjservers++;

		if (volume->nservers > 0)
			/* another server might acknowledge its existence */
			goto try_next_server_upw;

		/* handle the case where all the fileservers have rejected the
		 * volume
		 * - TODO: try asking the fileservers for volume information
		 * - TODO: contact the VL server again to see if the volume is
		 *         no longer registered
		 */
		up_write(&volume->server_sem);
		afs_put_server(server);
		_leave(" [completely rejected]");
		return 1;

		/* problem reaching the server */
	case -ENETUNREACH:
	case -EHOSTUNREACH:
	case -ECONNREFUSED:
	case -ETIME:
	case -ETIMEDOUT:
	case -EREMOTEIO:
		/* mark the server as dead
		 * TODO: vary dead timeout depending on error
		 */
		spin_lock(&server->fs_lock);
		if (!server->fs_state) {
			server->fs_dead_jif = jiffies + HZ * 10;
			server->fs_state = result;
			printk("kAFS: SERVER DEAD state=%d\n", result);
		}
		spin_unlock(&server->fs_lock);
		goto try_next_server;

		/* miscellaneous error */
	default:
		server->fs_act_jif = jiffies;
	case -ENOMEM:
	case -ENONET:
		/* tell the caller to accept the result */
		afs_put_server(server);
		_leave(" [local failure]");
		return 1;
	}

	/* tell the caller to loop around and try the next server */
try_next_server_upw:
	up_write(&volume->server_sem);
try_next_server:
	afs_put_server(server);
	_leave(" [try next server]");
	return 0;
}
=======
 * Try to get a reference on a volume record.
 */
bool afs_try_get_volume(struct afs_volume *volume, enum afs_volume_trace reason)
{
	int r;

	if (__refcount_inc_not_zero(&volume->ref, &r)) {
		trace_afs_volume(volume->vid, r + 1, reason);
		return true;
	}
	return false;
}

/*
 * Get a reference on a volume record.
 */
struct afs_volume *afs_get_volume(struct afs_volume *volume,
				  enum afs_volume_trace reason)
{
	if (volume) {
		int r;

		__refcount_inc(&volume->ref, &r);
		trace_afs_volume(volume->vid, r + 1, reason);
	}
	return volume;
}


/*
 * Drop a reference on a volume record.
 */
void afs_put_volume(struct afs_volume *volume, enum afs_volume_trace reason)
{
	if (volume) {
		afs_volid_t vid = volume->vid;
		bool zero;
		int r;

		zero = __refcount_dec_and_test(&volume->ref, &r);
		trace_afs_volume(vid, r - 1, reason);
		if (zero)
			schedule_work(&volume->destructor);
	}
}

/*
 * Activate a volume.
 */
int afs_activate_volume(struct afs_volume *volume)
{
#ifdef CONFIG_AFS_FSCACHE
	struct fscache_volume *vcookie;
	char *name;

	name = kasprintf(GFP_KERNEL, "afs,%s,%llx",
			 volume->cell->name, volume->vid);
	if (!name)
		return -ENOMEM;

	vcookie = fscache_acquire_volume(name, NULL, NULL, 0);
	if (IS_ERR(vcookie)) {
		if (vcookie != ERR_PTR(-EBUSY)) {
			kfree(name);
			return PTR_ERR(vcookie);
		}
		pr_err("AFS: Cache volume key already in use (%s)\n", name);
		vcookie = NULL;
	}
	volume->cache = vcookie;
	kfree(name);
#endif
	return 0;
}

/*
 * Deactivate a volume.
 */
void afs_deactivate_volume(struct afs_volume *volume)
{
	_enter("%s", volume->name);

#ifdef CONFIG_AFS_FSCACHE
	fscache_relinquish_volume(volume->cache, NULL,
				  test_bit(AFS_VOLUME_DELETED, &volume->flags));
	volume->cache = NULL;
#endif

	_leave("");
}

/*
 * Query the VL service to update the volume status.
 */
static int afs_update_volume_status(struct afs_volume *volume, struct key *key)
{
	struct afs_server_list *new, *old, *discard;
	struct afs_vldb_entry *vldb;
	char idbuf[24];
	int ret, idsz;

	_enter("");

	/* We look up an ID by passing it as a decimal string in the
	 * operation's name parameter.
	 */
	idsz = snprintf(idbuf, sizeof(idbuf), "%llu", volume->vid);

	vldb = afs_vl_lookup_vldb(volume->cell, key, idbuf, idsz);
	if (IS_ERR(vldb)) {
		ret = PTR_ERR(vldb);
		goto error;
	}

	/* See if the volume got renamed. */
	if (vldb->name_len != volume->name_len ||
	    memcmp(vldb->name, volume->name, vldb->name_len) != 0) {
		/* TODO: Use RCU'd string. */
		memcpy(volume->name, vldb->name, AFS_MAXVOLNAME);
		volume->name_len = vldb->name_len;
	}

	/* See if the volume's server list got updated. */
	new = afs_alloc_server_list(volume, key, vldb);
	if (IS_ERR(new)) {
		ret = PTR_ERR(new);
		goto error_vldb;
	}

	write_lock(&volume->servers_lock);

	discard = new;
	old = rcu_dereference_protected(volume->servers,
					lockdep_is_held(&volume->servers_lock));
	if (afs_annotate_server_list(new, old)) {
		new->seq = volume->servers_seq + 1;
		rcu_assign_pointer(volume->servers, new);
		smp_wmb();
		volume->servers_seq++;
		discard = old;
	}

	/* Check more often if replication is ongoing. */
	if (new->ro_replicating)
		volume->update_at = ktime_get_real_seconds() + 10 * 60;
	else
		volume->update_at = ktime_get_real_seconds() + afs_volume_record_life;
	write_unlock(&volume->servers_lock);

	if (discard == old)
		afs_reattach_volume_to_servers(volume, new, old);
	afs_put_serverlist(volume->cell->net, discard);
	ret = 0;
error_vldb:
	kfree(vldb);
error:
	_leave(" = %d", ret);
	return ret;
}

/*
 * Make sure the volume record is up to date.
 */
int afs_check_volume_status(struct afs_volume *volume, struct afs_operation *op)
{
	int ret, retries = 0;

	_enter("");

retry:
	if (test_bit(AFS_VOLUME_WAIT, &volume->flags))
		goto wait;
	if (volume->update_at <= ktime_get_real_seconds() ||
	    test_bit(AFS_VOLUME_NEEDS_UPDATE, &volume->flags))
		goto update;
	_leave(" = 0");
	return 0;

update:
	if (!test_and_set_bit_lock(AFS_VOLUME_UPDATING, &volume->flags)) {
		clear_bit(AFS_VOLUME_NEEDS_UPDATE, &volume->flags);
		ret = afs_update_volume_status(volume, op->key);
		if (ret < 0)
			set_bit(AFS_VOLUME_NEEDS_UPDATE, &volume->flags);
		clear_bit_unlock(AFS_VOLUME_WAIT, &volume->flags);
		clear_bit_unlock(AFS_VOLUME_UPDATING, &volume->flags);
		wake_up_bit(&volume->flags, AFS_VOLUME_WAIT);
		_leave(" = %d", ret);
		return ret;
	}

wait:
	if (!test_bit(AFS_VOLUME_WAIT, &volume->flags)) {
		_leave(" = 0 [no wait]");
		return 0;
	}

	ret = wait_on_bit(&volume->flags, AFS_VOLUME_WAIT,
			  (op->flags & AFS_OPERATION_UNINTR) ?
			  TASK_UNINTERRUPTIBLE : TASK_INTERRUPTIBLE);
	if (ret == -ERESTARTSYS) {
		_leave(" = %d", ret);
		return ret;
	}

	retries++;
	if (retries == 4) {
		_leave(" = -ESTALE");
		return -ESTALE;
	}
	goto retry;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
