<<<<<<< HEAD
#ifndef _LINUX_NSPROXY_H
#define _LINUX_NSPROXY_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NSPROXY_H
#define _LINUX_NSPROXY_H

#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/spinlock.h>
#include <linux/sched.h>

struct mnt_namespace;
struct uts_namespace;
struct ipc_namespace;
struct pid_namespace;
<<<<<<< HEAD
=======
struct cgroup_namespace;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct fs_struct;

/*
 * A structure to contain pointers to all per-process
 * namespaces - fs (mount), uts, network, sysvipc, etc.
 *
<<<<<<< HEAD
=======
 * The pid namespace is an exception -- it's accessed using
 * task_active_pid_ns.  The pid namespace here is the
 * namespace that children will use.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * 'count' is the number of tasks holding a reference.
 * The count for each namespace, then, will be the number
 * of nsproxies pointing to it, not the number of tasks.
 *
 * The nsproxy is shared by tasks which share all namespaces.
 * As soon as a single namespace is cloned or unshared, the
 * nsproxy is copied.
 */
struct nsproxy {
<<<<<<< HEAD
	atomic_t count;
	struct uts_namespace *uts_ns;
	struct ipc_namespace *ipc_ns;
	struct mnt_namespace *mnt_ns;
	struct pid_namespace *pid_ns;
	struct net 	     *net_ns;
=======
	refcount_t count;
	struct uts_namespace *uts_ns;
	struct ipc_namespace *ipc_ns;
	struct mnt_namespace *mnt_ns;
	struct pid_namespace *pid_ns_for_children;
	struct net 	     *net_ns;
	struct time_namespace *time_ns;
	struct time_namespace *time_ns_for_children;
	struct cgroup_namespace *cgroup_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
extern struct nsproxy init_nsproxy;

/*
<<<<<<< HEAD
 * the namespaces access rules are:
 *
 *  1. only current task is allowed to change tsk->nsproxy pointer or
 *     any pointer on the nsproxy itself
=======
 * A structure to encompass all bits needed to install
 * a partial or complete new set of namespaces.
 *
 * If a new user namespace is requested cred will
 * point to a modifiable set of credentials. If a pointer
 * to a modifiable set is needed nsset_cred() must be
 * used and tested.
 */
struct nsset {
	unsigned flags;
	struct nsproxy *nsproxy;
	struct fs_struct *fs;
	const struct cred *cred;
};

static inline struct cred *nsset_cred(struct nsset *set)
{
	if (set->flags & CLONE_NEWUSER)
		return (struct cred *)set->cred;

	return NULL;
}

/*
 * the namespaces access rules are:
 *
 *  1. only current task is allowed to change tsk->nsproxy pointer or
 *     any pointer on the nsproxy itself.  Current must hold the task_lock
 *     when changing tsk->nsproxy.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  2. when accessing (i.e. reading) current task's namespaces - no
 *     precautions should be taken - just dereference the pointers
 *
 *  3. the access to other task namespaces is performed like this
<<<<<<< HEAD
 *     rcu_read_lock();
 *     nsproxy = task_nsproxy(tsk);
=======
 *     task_lock(task);
 *     nsproxy = task->nsproxy;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *     if (nsproxy != NULL) {
 *             / *
 *               * work with the namespaces here
 *               * e.g. get the reference on one of them
 *               * /
 *     } / *
<<<<<<< HEAD
 *         * NULL task_nsproxy() means that this task is
 *         * almost dead (zombie)
 *         * /
 *     rcu_read_unlock();
 *
 */

static inline struct nsproxy *task_nsproxy(struct task_struct *tsk)
{
	return rcu_dereference(tsk->nsproxy);
}

int copy_namespaces(unsigned long flags, struct task_struct *tsk);
void exit_task_namespaces(struct task_struct *tsk);
void switch_task_namespaces(struct task_struct *tsk, struct nsproxy *new);
void free_nsproxy(struct nsproxy *ns);
int unshare_nsproxy_namespaces(unsigned long, struct nsproxy **,
	struct fs_struct *);
=======
 *         * NULL task->nsproxy means that this task is
 *         * almost dead (zombie)
 *         * /
 *     task_unlock(task);
 *
 */

int copy_namespaces(unsigned long flags, struct task_struct *tsk);
void exit_task_namespaces(struct task_struct *tsk);
void switch_task_namespaces(struct task_struct *tsk, struct nsproxy *new);
int exec_task_namespaces(void);
void free_nsproxy(struct nsproxy *ns);
int unshare_nsproxy_namespaces(unsigned long, struct nsproxy **,
	struct cred *, struct fs_struct *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init nsproxy_cache_init(void);

static inline void put_nsproxy(struct nsproxy *ns)
{
<<<<<<< HEAD
	if (atomic_dec_and_test(&ns->count)) {
		free_nsproxy(ns);
	}
=======
	if (refcount_dec_and_test(&ns->count))
		free_nsproxy(ns);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void get_nsproxy(struct nsproxy *ns)
{
<<<<<<< HEAD
	atomic_inc(&ns->count);
=======
	refcount_inc(&ns->count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif
