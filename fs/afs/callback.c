/*
 * Copyright (c) 2002, 2007 Red Hat, Inc. All rights reserved.
 *
 * This software may be freely redistributed under the terms of the
 * GNU General Public License.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Authors: David Woodhouse <dwmw2@infradead.org>
 *          David Howells <dhowells@redhat.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/circ_buf.h>
#include <linux/sched.h>
#include "internal.h"

<<<<<<< HEAD
#if 0
unsigned afs_vnode_update_timeout = 10;
#endif  /*  0  */

#define afs_breakring_space(server) \
	CIRC_SPACE((server)->cb_break_head, (server)->cb_break_tail,	\
		   ARRAY_SIZE((server)->cb_break))

//static void afs_callback_updater(struct work_struct *);

static struct workqueue_struct *afs_callback_update_worker;

/*
 * allow the fileserver to request callback state (re-)initialisation
 */
void afs_init_callback_state(struct afs_server *server)
{
	struct afs_vnode *vnode;

	_enter("{%p}", server);

	spin_lock(&server->cb_lock);

	/* kill all the promises on record from this server */
	while (!RB_EMPTY_ROOT(&server->cb_promises)) {
		vnode = rb_entry(server->cb_promises.rb_node,
				 struct afs_vnode, cb_promise);
		_debug("UNPROMISE { vid=%x:%u uq=%u}",
		       vnode->fid.vid, vnode->fid.vnode, vnode->fid.unique);
		rb_erase(&vnode->cb_promise, &server->cb_promises);
		vnode->cb_promised = false;
	}

	spin_unlock(&server->cb_lock);
	_leave("");
}

/*
 * handle the data invalidation side of a callback being broken
 */
void afs_broken_callback_work(struct work_struct *work)
{
	struct afs_vnode *vnode =
		container_of(work, struct afs_vnode, cb_broken_work);

	_enter("");

	if (test_bit(AFS_VNODE_DELETED, &vnode->flags))
		return;

	/* we're only interested in dealing with a broken callback on *this*
	 * vnode and only if no-one else has dealt with it yet */
	if (!mutex_trylock(&vnode->validate_lock))
		return; /* someone else is dealing with it */

	if (test_bit(AFS_VNODE_CB_BROKEN, &vnode->flags)) {
		if (S_ISDIR(vnode->vfs_inode.i_mode))
			afs_clear_permits(vnode);

		if (afs_vnode_fetch_status(vnode, NULL, NULL) < 0)
			goto out;

		if (test_bit(AFS_VNODE_DELETED, &vnode->flags))
			goto out;

		/* if the vnode's data version number changed then its contents
		 * are different */
		if (test_and_clear_bit(AFS_VNODE_ZAP_DATA, &vnode->flags))
			afs_zap_data(vnode);
	}

out:
	mutex_unlock(&vnode->validate_lock);

	/* avoid the potential race whereby the mutex_trylock() in this
	 * function happens again between the clear_bit() and the
	 * mutex_unlock() */
	if (test_bit(AFS_VNODE_CB_BROKEN, &vnode->flags)) {
		_debug("requeue");
		queue_work(afs_callback_update_worker, &vnode->cb_broken_work);
	}
	_leave("");
=======
/*
 * Handle invalidation of an mmap'd file.  We invalidate all the PTEs referring
 * to the pages in this file's pagecache, forcing the kernel to go through
 * ->fault() or ->page_mkwrite() - at which point we can handle invalidation
 * more fully.
 */
void afs_invalidate_mmap_work(struct work_struct *work)
{
	struct afs_vnode *vnode = container_of(work, struct afs_vnode, cb_work);

	unmap_mapping_pages(vnode->netfs.inode.i_mapping, 0, 0, false);
}

static void afs_volume_init_callback(struct afs_volume *volume)
{
	struct afs_vnode *vnode;

	down_read(&volume->open_mmaps_lock);

	list_for_each_entry(vnode, &volume->open_mmaps, cb_mmap_link) {
		if (vnode->cb_v_check != atomic_read(&volume->cb_v_break)) {
			atomic64_set(&vnode->cb_expires_at, AFS_NO_CB_PROMISE);
			queue_work(system_unbound_wq, &vnode->cb_work);
		}
	}

	up_read(&volume->open_mmaps_lock);
}

/*
 * Allow the fileserver to request callback state (re-)initialisation.
 * Unfortunately, UUIDs are not guaranteed unique.
 */
void afs_init_callback_state(struct afs_server *server)
{
	struct afs_server_entry *se;

	down_read(&server->cell->vs_lock);

	list_for_each_entry(se, &server->volumes, slink) {
		se->cb_expires_at = AFS_NO_CB_PROMISE;
		se->volume->cb_expires_at = AFS_NO_CB_PROMISE;
		trace_afs_cb_v_break(se->volume->vid, atomic_read(&se->volume->cb_v_break),
				     afs_cb_break_for_s_reinit);
		if (!list_empty(&se->volume->open_mmaps))
			afs_volume_init_callback(se->volume);
	}

	up_read(&server->cell->vs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * actually break a callback
 */
<<<<<<< HEAD
static void afs_break_callback(struct afs_server *server,
			       struct afs_vnode *vnode)
{
	_enter("");

	set_bit(AFS_VNODE_CB_BROKEN, &vnode->flags);

	if (vnode->cb_promised) {
		spin_lock(&vnode->lock);

		_debug("break callback");

		spin_lock(&server->cb_lock);
		if (vnode->cb_promised) {
			rb_erase(&vnode->cb_promise, &server->cb_promises);
			vnode->cb_promised = false;
		}
		spin_unlock(&server->cb_lock);

		queue_work(afs_callback_update_worker, &vnode->cb_broken_work);
		if (list_empty(&vnode->granted_locks) &&
		    !list_empty(&vnode->pending_locks))
			afs_lock_may_be_available(vnode);
		spin_unlock(&vnode->lock);
	}
}

=======
void __afs_break_callback(struct afs_vnode *vnode, enum afs_cb_break_reason reason)
{
	_enter("");

	clear_bit(AFS_VNODE_NEW_CONTENT, &vnode->flags);
	if (atomic64_xchg(&vnode->cb_expires_at, AFS_NO_CB_PROMISE) != AFS_NO_CB_PROMISE) {
		vnode->cb_break++;
		vnode->cb_v_check = atomic_read(&vnode->volume->cb_v_break);
		afs_clear_permits(vnode);

		if (vnode->lock_state == AFS_VNODE_LOCK_WAITING_FOR_CB)
			afs_lock_may_be_available(vnode);

		if (reason != afs_cb_break_for_deleted &&
		    vnode->status.type == AFS_FTYPE_FILE &&
		    atomic_read(&vnode->cb_nr_mmap))
			queue_work(system_unbound_wq, &vnode->cb_work);

		trace_afs_cb_break(&vnode->fid, vnode->cb_break, reason, true);
	} else {
		trace_afs_cb_break(&vnode->fid, vnode->cb_break, reason, false);
	}
}

void afs_break_callback(struct afs_vnode *vnode, enum afs_cb_break_reason reason)
{
	write_seqlock(&vnode->cb_lock);
	__afs_break_callback(vnode, reason);
	write_sequnlock(&vnode->cb_lock);
}

/*
 * Look up a volume by volume ID under RCU conditions.
 */
static struct afs_volume *afs_lookup_volume_rcu(struct afs_cell *cell,
						afs_volid_t vid)
{
	struct afs_volume *volume = NULL;
	struct rb_node *p;
	int seq = 1;

	for (;;) {
		/* Unfortunately, rbtree walking doesn't give reliable results
		 * under just the RCU read lock, so we have to check for
		 * changes.
		 */
		seq++; /* 2 on the 1st/lockless path, otherwise odd */
		read_seqbegin_or_lock(&cell->volume_lock, &seq);

		p = rcu_dereference_raw(cell->volumes.rb_node);
		while (p) {
			volume = rb_entry(p, struct afs_volume, cell_node);

			if (volume->vid < vid)
				p = rcu_dereference_raw(p->rb_left);
			else if (volume->vid > vid)
				p = rcu_dereference_raw(p->rb_right);
			else
				break;
			volume = NULL;
		}

		if (volume && afs_try_get_volume(volume, afs_volume_trace_get_callback))
			break;
		if (!need_seqretry(&cell->volume_lock, seq))
			break;
		seq |= 1; /* Want a lock next time */
	}

	done_seqretry(&cell->volume_lock, seq);
	return volume;
}

/*
 * Allow the fileserver to break callbacks at the volume-level.  This is
 * typically done when, for example, a R/W volume is snapshotted to a R/O
 * volume (the only way to change an R/O volume).  It may also, however, happen
 * when a volserver takes control of a volume (offlining it, moving it, etc.).
 *
 * Every file in that volume will need to be reevaluated.
 */
static void afs_break_volume_callback(struct afs_server *server,
				      struct afs_volume *volume)
	__releases(RCU)
{
	struct afs_server_list *slist = rcu_dereference(volume->servers);
	unsigned int i, cb_v_break;

	write_lock(&volume->cb_v_break_lock);

	for (i = 0; i < slist->nr_servers; i++)
		if (slist->servers[i].server == server)
			slist->servers[i].cb_expires_at = AFS_NO_CB_PROMISE;
	volume->cb_expires_at = AFS_NO_CB_PROMISE;

	cb_v_break = atomic_inc_return_release(&volume->cb_v_break);
	trace_afs_cb_v_break(volume->vid, cb_v_break, afs_cb_break_for_volume_callback);

	write_unlock(&volume->cb_v_break_lock);
	rcu_read_unlock();

	if (!list_empty(&volume->open_mmaps))
		afs_volume_init_callback(volume);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * allow the fileserver to explicitly break one callback
 * - happens when
 *   - the backing file is changed
 *   - a lock is released
 */
static void afs_break_one_callback(struct afs_server *server,
<<<<<<< HEAD
				   struct afs_fid *fid)
{
	struct afs_vnode *vnode;
	struct rb_node *p;

	_debug("find");
	spin_lock(&server->fs_lock);
	p = server->fs_vnodes.rb_node;
	while (p) {
		vnode = rb_entry(p, struct afs_vnode, server_rb);
		if (fid->vid < vnode->fid.vid)
			p = p->rb_left;
		else if (fid->vid > vnode->fid.vid)
			p = p->rb_right;
		else if (fid->vnode < vnode->fid.vnode)
			p = p->rb_left;
		else if (fid->vnode > vnode->fid.vnode)
			p = p->rb_right;
		else if (fid->unique < vnode->fid.unique)
			p = p->rb_left;
		else if (fid->unique > vnode->fid.unique)
			p = p->rb_right;
		else
			goto found;
	}

	/* not found so we just ignore it (it may have moved to another
	 * server) */
not_available:
	_debug("not avail");
	spin_unlock(&server->fs_lock);
	_leave("");
	return;

found:
	_debug("found");
	ASSERTCMP(server, ==, vnode->server);

	if (!igrab(AFS_VNODE_TO_I(vnode)))
		goto not_available;
	spin_unlock(&server->fs_lock);

	afs_break_callback(server, vnode);
	iput(&vnode->vfs_inode);
	_leave("");
=======
				   struct afs_volume *volume,
				   struct afs_fid *fid)
{
	struct super_block *sb;
	struct afs_vnode *vnode;
	struct inode *inode;

	/* See if we can find a matching inode - even an I_NEW inode needs to
	 * be marked as it can have its callback broken before we finish
	 * setting up the local inode.
	 */
	sb = rcu_dereference(volume->sb);
	if (!sb)
		return;

	inode = find_inode_rcu(sb, fid->vnode, afs_ilookup5_test_by_fid, fid);
	if (inode) {
		vnode = AFS_FS_I(inode);
		afs_break_callback(vnode, afs_cb_break_for_callback);
	} else {
		trace_afs_cb_miss(fid, afs_cb_break_for_callback);
	}
}

static void afs_break_some_callbacks(struct afs_server *server,
				     struct afs_callback_break *cbb,
				     size_t *_count)
{
	struct afs_callback_break *residue = cbb;
	struct afs_volume *volume;
	afs_volid_t vid = cbb->fid.vid;
	size_t i;

	rcu_read_lock();
	volume = afs_lookup_volume_rcu(server->cell, vid);
	if (cbb->fid.vnode == 0 && cbb->fid.unique == 0) {
		afs_break_volume_callback(server, volume);
		*_count -= 1;
		if (*_count)
			memmove(cbb, cbb + 1, sizeof(*cbb) * *_count);
	} else {
		/* TODO: Find all matching volumes if we couldn't match the server and
		 * break them anyway.
		 */

		for (i = *_count; i > 0; cbb++, i--) {
			if (cbb->fid.vid == vid) {
				_debug("- Fid { vl=%08llx n=%llu u=%u }",
				       cbb->fid.vid,
				       cbb->fid.vnode,
				       cbb->fid.unique);
				--*_count;
				if (volume)
					afs_break_one_callback(server, volume, &cbb->fid);
			} else {
				*residue++ = *cbb;
			}
		}
		rcu_read_unlock();
	}

	afs_put_volume(volume, afs_volume_trace_put_callback);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * allow the fileserver to break callback promises
 */
void afs_break_callbacks(struct afs_server *server, size_t count,
<<<<<<< HEAD
			 struct afs_callback callbacks[])
=======
			 struct afs_callback_break *callbacks)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	_enter("%p,%zu,", server, count);

	ASSERT(server != NULL);
<<<<<<< HEAD
	ASSERTCMP(count, <=, AFSCBMAX);

	for (; count > 0; callbacks++, count--) {
		_debug("- Fid { vl=%08x n=%u u=%u }  CB { v=%u x=%u t=%u }",
		       callbacks->fid.vid,
		       callbacks->fid.vnode,
		       callbacks->fid.unique,
		       callbacks->version,
		       callbacks->expiry,
		       callbacks->type
		       );
		afs_break_one_callback(server, &callbacks->fid);
	}

	_leave("");
	return;
}

/*
 * record the callback for breaking
 * - the caller must hold server->cb_lock
 */
static void afs_do_give_up_callback(struct afs_server *server,
				    struct afs_vnode *vnode)
{
	struct afs_callback *cb;

	_enter("%p,%p", server, vnode);

	cb = &server->cb_break[server->cb_break_head];
	cb->fid		= vnode->fid;
	cb->version	= vnode->cb_version;
	cb->expiry	= vnode->cb_expiry;
	cb->type	= vnode->cb_type;
	smp_wmb();
	server->cb_break_head =
		(server->cb_break_head + 1) &
		(ARRAY_SIZE(server->cb_break) - 1);

	/* defer the breaking of callbacks to try and collect as many as
	 * possible to ship in one operation */
	switch (atomic_inc_return(&server->cb_break_n)) {
	case 1 ... AFSCBMAX - 1:
		queue_delayed_work(afs_callback_update_worker,
				   &server->cb_break_work, HZ * 2);
		break;
	case AFSCBMAX:
		afs_flush_callback_breaks(server);
		break;
	default:
		break;
	}

	ASSERT(server->cb_promises.rb_node != NULL);
	rb_erase(&vnode->cb_promise, &server->cb_promises);
	vnode->cb_promised = false;
	_leave("");
}

/*
 * discard the callback on a deleted item
 */
void afs_discard_callback_on_delete(struct afs_vnode *vnode)
{
	struct afs_server *server = vnode->server;

	_enter("%d", vnode->cb_promised);

	if (!vnode->cb_promised) {
		_leave(" [not promised]");
		return;
	}

	ASSERT(server != NULL);

	spin_lock(&server->cb_lock);
	if (vnode->cb_promised) {
		ASSERT(server->cb_promises.rb_node != NULL);
		rb_erase(&vnode->cb_promise, &server->cb_promises);
		vnode->cb_promised = false;
	}
	spin_unlock(&server->cb_lock);
	_leave("");
}

/*
 * give up the callback registered for a vnode on the file server when the
 * inode is being cleared
 */
void afs_give_up_callback(struct afs_vnode *vnode)
{
	struct afs_server *server = vnode->server;

	DECLARE_WAITQUEUE(myself, current);

	_enter("%d", vnode->cb_promised);

	_debug("GIVE UP INODE %p", &vnode->vfs_inode);

	if (!vnode->cb_promised) {
		_leave(" [not promised]");
		return;
	}

	ASSERT(server != NULL);

	spin_lock(&server->cb_lock);
	if (vnode->cb_promised && afs_breakring_space(server) == 0) {
		add_wait_queue(&server->cb_break_waitq, &myself);
		for (;;) {
			set_current_state(TASK_UNINTERRUPTIBLE);
			if (!vnode->cb_promised ||
			    afs_breakring_space(server) != 0)
				break;
			spin_unlock(&server->cb_lock);
			schedule();
			spin_lock(&server->cb_lock);
		}
		remove_wait_queue(&server->cb_break_waitq, &myself);
		__set_current_state(TASK_RUNNING);
	}

	/* of course, it's always possible for the server to break this vnode's
	 * callback first... */
	if (vnode->cb_promised)
		afs_do_give_up_callback(server, vnode);

	spin_unlock(&server->cb_lock);
	_leave("");
}

/*
 * dispatch a deferred give up callbacks operation
 */
void afs_dispatch_give_up_callbacks(struct work_struct *work)
{
	struct afs_server *server =
		container_of(work, struct afs_server, cb_break_work.work);

	_enter("");

	/* tell the fileserver to discard the callback promises it has
	 * - in the event of ENOMEM or some other error, we just forget that we
	 *   had callbacks entirely, and the server will call us later to break
	 *   them
	 */
	afs_fs_give_up_callbacks(server, &afs_async_call);
}

/*
 * flush the outstanding callback breaks on a server
 */
void afs_flush_callback_breaks(struct afs_server *server)
{
	cancel_delayed_work(&server->cb_break_work);
	queue_delayed_work(afs_callback_update_worker,
			   &server->cb_break_work, 0);
}

#if 0
/*
 * update a bunch of callbacks
 */
static void afs_callback_updater(struct work_struct *work)
{
	struct afs_server *server;
	struct afs_vnode *vnode, *xvnode;
	time_t now;
	long timeout;
	int ret;

	server = container_of(work, struct afs_server, updater);

	_enter("");

	now = get_seconds();

	/* find the first vnode to update */
	spin_lock(&server->cb_lock);
	for (;;) {
		if (RB_EMPTY_ROOT(&server->cb_promises)) {
			spin_unlock(&server->cb_lock);
			_leave(" [nothing]");
			return;
		}

		vnode = rb_entry(rb_first(&server->cb_promises),
				 struct afs_vnode, cb_promise);
		if (atomic_read(&vnode->usage) > 0)
			break;
		rb_erase(&vnode->cb_promise, &server->cb_promises);
		vnode->cb_promised = false;
	}

	timeout = vnode->update_at - now;
	if (timeout > 0) {
		queue_delayed_work(afs_vnode_update_worker,
				   &afs_vnode_update, timeout * HZ);
		spin_unlock(&server->cb_lock);
		_leave(" [nothing]");
		return;
	}

	list_del_init(&vnode->update);
	atomic_inc(&vnode->usage);
	spin_unlock(&server->cb_lock);

	/* we can now perform the update */
	_debug("update %s", vnode->vldb.name);
	vnode->state = AFS_VL_UPDATING;
	vnode->upd_rej_cnt = 0;
	vnode->upd_busy_cnt = 0;

	ret = afs_vnode_update_record(vl, &vldb);
	switch (ret) {
	case 0:
		afs_vnode_apply_update(vl, &vldb);
		vnode->state = AFS_VL_UPDATING;
		break;
	case -ENOMEDIUM:
		vnode->state = AFS_VL_VOLUME_DELETED;
		break;
	default:
		vnode->state = AFS_VL_UNCERTAIN;
		break;
	}

	/* and then reschedule */
	_debug("reschedule");
	vnode->update_at = get_seconds() + afs_vnode_update_timeout;

	spin_lock(&server->cb_lock);

	if (!list_empty(&server->cb_promises)) {
		/* next update in 10 minutes, but wait at least 1 second more
		 * than the newest record already queued so that we don't spam
		 * the VL server suddenly with lots of requests
		 */
		xvnode = list_entry(server->cb_promises.prev,
				    struct afs_vnode, update);
		if (vnode->update_at <= xvnode->update_at)
			vnode->update_at = xvnode->update_at + 1;
		xvnode = list_entry(server->cb_promises.next,
				    struct afs_vnode, update);
		timeout = xvnode->update_at - now;
		if (timeout < 0)
			timeout = 0;
	} else {
		timeout = afs_vnode_update_timeout;
	}

	list_add_tail(&vnode->update, &server->cb_promises);

	_debug("timeout %ld", timeout);
	queue_delayed_work(afs_vnode_update_worker,
			   &afs_vnode_update, timeout * HZ);
	spin_unlock(&server->cb_lock);
	afs_put_vnode(vl);
}
#endif

/*
 * initialise the callback update process
 */
int __init afs_callback_update_init(void)
{
	afs_callback_update_worker =
		create_singlethread_workqueue("kafs_callbackd");
	return afs_callback_update_worker ? 0 : -ENOMEM;
}

/*
 * shut down the callback update process
 */
void afs_callback_update_kill(void)
{
	destroy_workqueue(afs_callback_update_worker);
=======

	while (count > 0)
		afs_break_some_callbacks(server, callbacks, &count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
