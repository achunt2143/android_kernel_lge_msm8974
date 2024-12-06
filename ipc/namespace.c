<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/ipc/namespace.c
 * Copyright (C) 2006 Pavel Emelyanov <xemul@openvz.org> OpenVZ, SWsoft Inc.
 */

#include <linux/ipc.h>
#include <linux/msg.h>
#include <linux/ipc_namespace.h>
#include <linux/rcupdate.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/user_namespace.h>
#include <linux/proc_fs.h>

#include "util.h"

static struct ipc_namespace *create_ipc_ns(struct task_struct *tsk,
					   struct ipc_namespace *old_ns)
{
	struct ipc_namespace *ns;
	int err;

	ns = kmalloc(sizeof(struct ipc_namespace), GFP_KERNEL);
	if (ns == NULL)
		return ERR_PTR(-ENOMEM);

	err = proc_alloc_inum(&ns->proc_inum);
	if (err) {
		kfree(ns);
		return ERR_PTR(err);
	}

	atomic_set(&ns->count, 1);
	err = mq_init_ns(ns);
	if (err) {
		proc_free_inum(ns->proc_inum);
		kfree(ns);
		return ERR_PTR(err);
	}
	atomic_inc(&nr_ipc_ns);

	sem_init_ns(ns);
	msg_init_ns(ns);
	shm_init_ns(ns);

	/*
	 * msgmni has already been computed for the new ipc ns.
	 * Thus, do the ipcns creation notification before registering that
	 * new ipcns in the chain.
	 */
	ipcns_notify(IPCNS_CREATED);
	register_ipcns_notifier(ns);

	ns->user_ns = get_user_ns(task_cred_xxx(tsk, user)->user_ns);

	return ns;
}

struct ipc_namespace *copy_ipcs(unsigned long flags,
				struct task_struct *tsk)
{
	struct ipc_namespace *ns = tsk->nsproxy->ipc_ns;

	if (!(flags & CLONE_NEWIPC))
		return get_ipc_ns(ns);
	return create_ipc_ns(tsk, ns);
=======
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/user_namespace.h>
#include <linux/proc_ns.h>
#include <linux/sched/task.h>

#include "util.h"

/*
 * The work queue is used to avoid the cost of synchronize_rcu in kern_unmount.
 */
static void free_ipc(struct work_struct *unused);
static DECLARE_WORK(free_ipc_work, free_ipc);

static struct ucounts *inc_ipc_namespaces(struct user_namespace *ns)
{
	return inc_ucount(ns, current_euid(), UCOUNT_IPC_NAMESPACES);
}

static void dec_ipc_namespaces(struct ucounts *ucounts)
{
	dec_ucount(ucounts, UCOUNT_IPC_NAMESPACES);
}

static struct ipc_namespace *create_ipc_ns(struct user_namespace *user_ns,
					   struct ipc_namespace *old_ns)
{
	struct ipc_namespace *ns;
	struct ucounts *ucounts;
	int err;

	err = -ENOSPC;
 again:
	ucounts = inc_ipc_namespaces(user_ns);
	if (!ucounts) {
		/*
		 * IPC namespaces are freed asynchronously, by free_ipc_work.
		 * If frees were pending, flush_work will wait, and
		 * return true. Fail the allocation if no frees are pending.
		 */
		if (flush_work(&free_ipc_work))
			goto again;
		goto fail;
	}

	err = -ENOMEM;
	ns = kzalloc(sizeof(struct ipc_namespace), GFP_KERNEL_ACCOUNT);
	if (ns == NULL)
		goto fail_dec;

	err = ns_alloc_inum(&ns->ns);
	if (err)
		goto fail_free;
	ns->ns.ops = &ipcns_operations;

	refcount_set(&ns->ns.count, 1);
	ns->user_ns = get_user_ns(user_ns);
	ns->ucounts = ucounts;

	err = mq_init_ns(ns);
	if (err)
		goto fail_put;

	err = -ENOMEM;
	if (!setup_mq_sysctls(ns))
		goto fail_put;

	if (!setup_ipc_sysctls(ns))
		goto fail_mq;

	err = msg_init_ns(ns);
	if (err)
		goto fail_put;

	sem_init_ns(ns);
	shm_init_ns(ns);

	return ns;

fail_mq:
	retire_mq_sysctls(ns);

fail_put:
	put_user_ns(ns->user_ns);
	ns_free_inum(&ns->ns);
fail_free:
	kfree(ns);
fail_dec:
	dec_ipc_namespaces(ucounts);
fail:
	return ERR_PTR(err);
}

struct ipc_namespace *copy_ipcs(unsigned long flags,
	struct user_namespace *user_ns, struct ipc_namespace *ns)
{
	if (!(flags & CLONE_NEWIPC))
		return get_ipc_ns(ns);
	return create_ipc_ns(user_ns, ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * free_ipcs - free all ipcs of one type
 * @ns:   the namespace to remove the ipcs from
 * @ids:  the table of ipcs to free
 * @free: the function called to free each individual ipc
 *
 * Called for each kind of ipc when an ipc_namespace exits.
 */
void free_ipcs(struct ipc_namespace *ns, struct ipc_ids *ids,
	       void (*free)(struct ipc_namespace *, struct kern_ipc_perm *))
{
	struct kern_ipc_perm *perm;
	int next_id;
	int total, in_use;

<<<<<<< HEAD
	down_write(&ids->rw_mutex);
=======
	down_write(&ids->rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	in_use = ids->in_use;

	for (total = 0, next_id = 0; total < in_use; next_id++) {
		perm = idr_find(&ids->ipcs_idr, next_id);
		if (perm == NULL)
			continue;
<<<<<<< HEAD
		ipc_lock_by_ptr(perm);
		free(ns, perm);
		total++;
	}
	up_write(&ids->rw_mutex);
=======
		rcu_read_lock();
		ipc_lock_object(perm);
		free(ns, perm);
		total++;
	}
	up_write(&ids->rwsem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void free_ipc_ns(struct ipc_namespace *ns)
{
	/*
<<<<<<< HEAD
	 * Unregistering the hotplug notifier at the beginning guarantees
	 * that the ipc namespace won't be freed while we are inside the
	 * callback routine. Since the blocking_notifier_chain_XXX routines
	 * hold a rw lock on the notifier list, unregister_ipcns_notifier()
	 * won't take the rw lock before blocking_notifier_call_chain() has
	 * released the rd lock.
	 */
	unregister_ipcns_notifier(ns);
	sem_exit_ns(ns);
	msg_exit_ns(ns);
	shm_exit_ns(ns);
	atomic_dec(&nr_ipc_ns);

	/*
	 * Do the ipcns removal notification after decrementing nr_ipc_ns in
	 * order to have a correct value when recomputing msgmni.
	 */
	ipcns_notify(IPCNS_REMOVED);
	put_user_ns(ns->user_ns);
	proc_free_inum(ns->proc_inum);
	kfree(ns);
}

=======
	 * Caller needs to wait for an RCU grace period to have passed
	 * after making the mount point inaccessible to new accesses.
	 */
	mntput(ns->mq_mnt);
	sem_exit_ns(ns);
	msg_exit_ns(ns);
	shm_exit_ns(ns);

	retire_mq_sysctls(ns);
	retire_ipc_sysctls(ns);

	dec_ipc_namespaces(ns->ucounts);
	put_user_ns(ns->user_ns);
	ns_free_inum(&ns->ns);
	kfree(ns);
}

static LLIST_HEAD(free_ipc_list);
static void free_ipc(struct work_struct *unused)
{
	struct llist_node *node = llist_del_all(&free_ipc_list);
	struct ipc_namespace *n, *t;

	llist_for_each_entry_safe(n, t, node, mnt_llist)
		mnt_make_shortterm(n->mq_mnt);

	/* Wait for any last users to have gone away. */
	synchronize_rcu();

	llist_for_each_entry_safe(n, t, node, mnt_llist)
		free_ipc_ns(n);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * put_ipc_ns - drop a reference to an ipc namespace.
 * @ns: the namespace to put
 *
 * If this is the last task in the namespace exiting, and
 * it is dropping the refcount to 0, then it can race with
 * a task in another ipc namespace but in a mounts namespace
 * which has this ipcns's mqueuefs mounted, doing some action
 * with one of the mqueuefs files.  That can raise the refcount.
 * So dropping the refcount, and raising the refcount when
 * accessing it through the VFS, are protected with mq_lock.
 *
 * (Clearly, a task raising the refcount on its own ipc_ns
 * needn't take mq_lock since it can't race with the last task
 * in the ipcns exiting).
 */
void put_ipc_ns(struct ipc_namespace *ns)
{
<<<<<<< HEAD
	if (atomic_dec_and_lock(&ns->count, &mq_lock)) {
		mq_clear_sbinfo(ns);
		spin_unlock(&mq_lock);
		mq_put_mnt(ns);
		free_ipc_ns(ns);
	}
}

static void *ipcns_get(struct task_struct *task)
=======
	if (refcount_dec_and_lock(&ns->ns.count, &mq_lock)) {
		mq_clear_sbinfo(ns);
		spin_unlock(&mq_lock);

		if (llist_add(&ns->mnt_llist, &free_ipc_list))
			schedule_work(&free_ipc_work);
	}
}

static inline struct ipc_namespace *to_ipc_ns(struct ns_common *ns)
{
	return container_of(ns, struct ipc_namespace, ns);
}

static struct ns_common *ipcns_get(struct task_struct *task)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipc_namespace *ns = NULL;
	struct nsproxy *nsproxy;

<<<<<<< HEAD
	rcu_read_lock();
	nsproxy = task_nsproxy(task);
	if (nsproxy)
		ns = get_ipc_ns(nsproxy->ipc_ns);
	rcu_read_unlock();

	return ns;
}

static void ipcns_put(void *ns)
{
	return put_ipc_ns(ns);
}

static int ipcns_install(struct nsproxy *nsproxy, void *ns)
{
	/* Ditch state from the old ipc namespace */
	exit_sem(current);
=======
	task_lock(task);
	nsproxy = task->nsproxy;
	if (nsproxy)
		ns = get_ipc_ns(nsproxy->ipc_ns);
	task_unlock(task);

	return ns ? &ns->ns : NULL;
}

static void ipcns_put(struct ns_common *ns)
{
	return put_ipc_ns(to_ipc_ns(ns));
}

static int ipcns_install(struct nsset *nsset, struct ns_common *new)
{
	struct nsproxy *nsproxy = nsset->nsproxy;
	struct ipc_namespace *ns = to_ipc_ns(new);
	if (!ns_capable(ns->user_ns, CAP_SYS_ADMIN) ||
	    !ns_capable(nsset->cred->user_ns, CAP_SYS_ADMIN))
		return -EPERM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_ipc_ns(nsproxy->ipc_ns);
	nsproxy->ipc_ns = get_ipc_ns(ns);
	return 0;
}

<<<<<<< HEAD
static unsigned int ipcns_inum(void *vp)
{
	struct ipc_namespace *ns = vp;

	return ns->proc_inum;
=======
static struct user_namespace *ipcns_owner(struct ns_common *ns)
{
	return to_ipc_ns(ns)->user_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct proc_ns_operations ipcns_operations = {
	.name		= "ipc",
	.type		= CLONE_NEWIPC,
	.get		= ipcns_get,
	.put		= ipcns_put,
	.install	= ipcns_install,
<<<<<<< HEAD
	.inum		= ipcns_inum,
=======
	.owner		= ipcns_owner,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
