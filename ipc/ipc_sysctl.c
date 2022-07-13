<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (C) 2007
 *
 *  Author: Eric Biederman <ebiederm@xmision.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/ipc.h>
#include <linux/nsproxy.h>
#include <linux/sysctl.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/ipc_namespace.h>
#include <linux/msg.h>
#include "util.h"

static void *get_ipc(ctl_table *table)
{
	char *which = table->data;
	struct ipc_namespace *ipc_ns = current->nsproxy->ipc_ns;
	which = (which - (char *)&init_ipc_ns) + (char *)ipc_ns;
	return which;
}

#ifdef CONFIG_PROC_SYSCTL
static int proc_ipc_dointvec(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_dointvec(&ipc_table, write, buffer, lenp, ppos);
}

static int proc_ipc_dointvec_minmax(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_dointvec_minmax(&ipc_table, write, buffer, lenp, ppos);
}

static int proc_ipc_dointvec_minmax_orphans(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ipc_namespace *ns = current->nsproxy->ipc_ns;
	int err = proc_ipc_dointvec_minmax(table, write, buffer, lenp, ppos);
=======
#include <linux/capability.h>
#include <linux/ipc_namespace.h>
#include <linux/msg.h>
#include <linux/slab.h>
#include <linux/cred.h>
#include "util.h"

static int proc_ipc_dointvec_minmax_orphans(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
{
	struct ipc_namespace *ns =
		container_of(table->data, struct ipc_namespace, shm_rmid_forced);
	int err;

	err = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (err < 0)
		return err;
	if (ns->shm_rmid_forced)
		shm_destroy_orphaned(ns);
	return err;
}

<<<<<<< HEAD
static int proc_ipc_callback_dointvec(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;
	size_t lenp_bef = *lenp;
	int rc;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	rc = proc_dointvec(&ipc_table, write, buffer, lenp, ppos);

	if (write && !rc && lenp_bef == *lenp)
		/*
		 * Tunable has successfully been changed by hand. Disable its
		 * automatic adjustment. This simply requires unregistering
		 * the notifiers that trigger recalculation.
		 */
		unregister_ipcns_notifier(current->nsproxy->ipc_ns);

	return rc;
}

static int proc_ipc_doulongvec_minmax(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;
	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);

	return proc_doulongvec_minmax(&ipc_table, write, buffer,
					lenp, ppos);
}

/*
 * Routine that is called when the file "auto_msgmni" has successfully been
 * written.
 * Two values are allowed:
 * 0: unregister msgmni's callback routine from the ipc namespace notifier
 *    chain. This means that msgmni won't be recomputed anymore upon memory
 *    add/remove or ipc namespace creation/removal.
 * 1: register back the callback routine.
 */
static void ipc_auto_callback(int val)
{
	if (!val)
		unregister_ipcns_notifier(current->nsproxy->ipc_ns);
	else {
		/*
		 * Re-enable automatic recomputing only if not already
		 * enabled.
		 */
		recompute_msgmni(current->nsproxy->ipc_ns);
		cond_register_ipcns_notifier(current->nsproxy->ipc_ns);
	}
}

static int proc_ipcauto_dointvec_minmax(ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;
	size_t lenp_bef = *lenp;
	int oldval;
	int rc;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = get_ipc(table);
	oldval = *((int *)(ipc_table.data));

	rc = proc_dointvec_minmax(&ipc_table, write, buffer, lenp, ppos);

	if (write && !rc && lenp_bef == *lenp) {
		int newval = *((int *)(ipc_table.data));
		/*
		 * The file "auto_msgmni" has correctly been set.
		 * React by (un)registering the corresponding tunable, if the
		 * value has changed.
		 */
		if (newval != oldval)
			ipc_auto_callback(newval);
	}

	return rc;
}

#else
#define proc_ipc_doulongvec_minmax NULL
#define proc_ipc_dointvec	   NULL
#define proc_ipc_dointvec_minmax   NULL
#define proc_ipc_dointvec_minmax_orphans   NULL
#define proc_ipc_callback_dointvec NULL
#define proc_ipcauto_dointvec_minmax NULL
#endif

static int zero;
static int one = 1;

static struct ctl_table ipc_kern_table[] = {
	{
		.procname	= "shmmax",
		.data		= &init_ipc_ns.shm_ctlmax,
		.maxlen		= sizeof (init_ipc_ns.shm_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_ipc_doulongvec_minmax,
=======
static int proc_ipc_auto_msgmni(struct ctl_table *table, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table ipc_table;
	int dummy = 0;

	memcpy(&ipc_table, table, sizeof(ipc_table));
	ipc_table.data = &dummy;

	if (write)
		pr_info_once("writing to auto_msgmni has no effect");

	return proc_dointvec_minmax(&ipc_table, write, buffer, lenp, ppos);
}

static int proc_ipc_sem_dointvec(struct ctl_table *table, int write,
	void *buffer, size_t *lenp, loff_t *ppos)
{
	struct ipc_namespace *ns =
		container_of(table->data, struct ipc_namespace, sem_ctls);
	int ret, semmni;

	semmni = ns->sem_ctls[3];
	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (!ret)
		ret = sem_check_semmni(ns);

	/*
	 * Reset the semmni value if an error happens.
	 */
	if (ret)
		ns->sem_ctls[3] = semmni;
	return ret;
}

int ipc_mni = IPCMNI;
int ipc_mni_shift = IPCMNI_SHIFT;
int ipc_min_cycle = RADIX_TREE_MAP_SIZE;

static struct ctl_table ipc_sysctls[] = {
	{
		.procname	= "shmmax",
		.data		= &init_ipc_ns.shm_ctlmax,
		.maxlen		= sizeof(init_ipc_ns.shm_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "shmall",
		.data		= &init_ipc_ns.shm_ctlall,
<<<<<<< HEAD
		.maxlen		= sizeof (init_ipc_ns.shm_ctlall),
		.mode		= 0644,
		.proc_handler	= proc_ipc_doulongvec_minmax,
=======
		.maxlen		= sizeof(init_ipc_ns.shm_ctlall),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "shmmni",
		.data		= &init_ipc_ns.shm_ctlmni,
<<<<<<< HEAD
		.maxlen		= sizeof (init_ipc_ns.shm_ctlmni),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec,
=======
		.maxlen		= sizeof(init_ipc_ns.shm_ctlmni),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= &ipc_mni,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "shm_rmid_forced",
		.data		= &init_ipc_ns.shm_rmid_forced,
		.maxlen		= sizeof(init_ipc_ns.shm_rmid_forced),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec_minmax_orphans,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &one,
=======
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "msgmax",
		.data		= &init_ipc_ns.msg_ctlmax,
<<<<<<< HEAD
		.maxlen		= sizeof (init_ipc_ns.msg_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec,
=======
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmax),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "msgmni",
		.data		= &init_ipc_ns.msg_ctlmni,
<<<<<<< HEAD
		.maxlen		= sizeof (init_ipc_ns.msg_ctlmni),
		.mode		= 0644,
		.proc_handler	= proc_ipc_callback_dointvec,
=======
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmni),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= &ipc_mni,
	},
	{
		.procname	= "auto_msgmni",
		.data		= NULL,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_ipc_auto_msgmni,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	=  "msgmnb",
		.data		= &init_ipc_ns.msg_ctlmnb,
<<<<<<< HEAD
		.maxlen		= sizeof (init_ipc_ns.msg_ctlmnb),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec,
=======
		.maxlen		= sizeof(init_ipc_ns.msg_ctlmnb),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.procname	= "sem",
		.data		= &init_ipc_ns.sem_ctls,
<<<<<<< HEAD
		.maxlen		= 4*sizeof (int),
		.mode		= 0644,
		.proc_handler	= proc_ipc_dointvec,
	},
	{
		.procname	= "auto_msgmni",
		.data		= &init_ipc_ns.auto_msgmni,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_ipcauto_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &one,
	},
	{}
};

static struct ctl_table ipc_root_table[] = {
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= ipc_kern_table,
	},
	{}
};

static int __init ipc_sysctl_init(void)
{
	register_sysctl_table(ipc_root_table);
	return 0;
}

__initcall(ipc_sysctl_init);
=======
		.maxlen		= 4*sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_ipc_sem_dointvec,
	},
#ifdef CONFIG_CHECKPOINT_RESTORE
	{
		.procname	= "sem_next_id",
		.data		= &init_ipc_ns.ids[IPC_SEM_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_SEM_IDS].next_id),
		.mode		= 0444,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
	},
	{
		.procname	= "msg_next_id",
		.data		= &init_ipc_ns.ids[IPC_MSG_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_MSG_IDS].next_id),
		.mode		= 0444,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
	},
	{
		.procname	= "shm_next_id",
		.data		= &init_ipc_ns.ids[IPC_SHM_IDS].next_id,
		.maxlen		= sizeof(init_ipc_ns.ids[IPC_SHM_IDS].next_id),
		.mode		= 0444,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_INT_MAX,
	},
#endif
	{}
};

static struct ctl_table_set *set_lookup(struct ctl_table_root *root)
{
	return &current->nsproxy->ipc_ns->ipc_set;
}

static int set_is_seen(struct ctl_table_set *set)
{
	return &current->nsproxy->ipc_ns->ipc_set == set;
}

static void ipc_set_ownership(struct ctl_table_header *head,
			      struct ctl_table *table,
			      kuid_t *uid, kgid_t *gid)
{
	struct ipc_namespace *ns =
		container_of(head->set, struct ipc_namespace, ipc_set);

	kuid_t ns_root_uid = make_kuid(ns->user_ns, 0);
	kgid_t ns_root_gid = make_kgid(ns->user_ns, 0);

	*uid = uid_valid(ns_root_uid) ? ns_root_uid : GLOBAL_ROOT_UID;
	*gid = gid_valid(ns_root_gid) ? ns_root_gid : GLOBAL_ROOT_GID;
}

static int ipc_permissions(struct ctl_table_header *head, struct ctl_table *table)
{
	int mode = table->mode;

#ifdef CONFIG_CHECKPOINT_RESTORE
	struct ipc_namespace *ns =
		container_of(head->set, struct ipc_namespace, ipc_set);

	if (((table->data == &ns->ids[IPC_SEM_IDS].next_id) ||
	     (table->data == &ns->ids[IPC_MSG_IDS].next_id) ||
	     (table->data == &ns->ids[IPC_SHM_IDS].next_id)) &&
	    checkpoint_restore_ns_capable(ns->user_ns))
		mode = 0666;
	else
#endif
	{
		kuid_t ns_root_uid;
		kgid_t ns_root_gid;

		ipc_set_ownership(head, table, &ns_root_uid, &ns_root_gid);

		if (uid_eq(current_euid(), ns_root_uid))
			mode >>= 6;

		else if (in_egroup_p(ns_root_gid))
			mode >>= 3;
	}

	mode &= 7;

	return (mode << 6) | (mode << 3) | mode;
}

static struct ctl_table_root set_root = {
	.lookup = set_lookup,
	.permissions = ipc_permissions,
	.set_ownership = ipc_set_ownership,
};

bool setup_ipc_sysctls(struct ipc_namespace *ns)
{
	struct ctl_table *tbl;

	setup_sysctl_set(&ns->ipc_set, &set_root, set_is_seen);

	tbl = kmemdup(ipc_sysctls, sizeof(ipc_sysctls), GFP_KERNEL);
	if (tbl) {
		int i;

		for (i = 0; i < ARRAY_SIZE(ipc_sysctls); i++) {
			if (tbl[i].data == &init_ipc_ns.shm_ctlmax)
				tbl[i].data = &ns->shm_ctlmax;

			else if (tbl[i].data == &init_ipc_ns.shm_ctlall)
				tbl[i].data = &ns->shm_ctlall;

			else if (tbl[i].data == &init_ipc_ns.shm_ctlmni)
				tbl[i].data = &ns->shm_ctlmni;

			else if (tbl[i].data == &init_ipc_ns.shm_rmid_forced)
				tbl[i].data = &ns->shm_rmid_forced;

			else if (tbl[i].data == &init_ipc_ns.msg_ctlmax)
				tbl[i].data = &ns->msg_ctlmax;

			else if (tbl[i].data == &init_ipc_ns.msg_ctlmni)
				tbl[i].data = &ns->msg_ctlmni;

			else if (tbl[i].data == &init_ipc_ns.msg_ctlmnb)
				tbl[i].data = &ns->msg_ctlmnb;

			else if (tbl[i].data == &init_ipc_ns.sem_ctls)
				tbl[i].data = &ns->sem_ctls;
#ifdef CONFIG_CHECKPOINT_RESTORE
			else if (tbl[i].data == &init_ipc_ns.ids[IPC_SEM_IDS].next_id)
				tbl[i].data = &ns->ids[IPC_SEM_IDS].next_id;

			else if (tbl[i].data == &init_ipc_ns.ids[IPC_MSG_IDS].next_id)
				tbl[i].data = &ns->ids[IPC_MSG_IDS].next_id;

			else if (tbl[i].data == &init_ipc_ns.ids[IPC_SHM_IDS].next_id)
				tbl[i].data = &ns->ids[IPC_SHM_IDS].next_id;
#endif
			else
				tbl[i].data = NULL;
		}

		ns->ipc_sysctls = __register_sysctl_table(&ns->ipc_set, "kernel", tbl,
							  ARRAY_SIZE(ipc_sysctls));
	}
	if (!ns->ipc_sysctls) {
		kfree(tbl);
		retire_sysctl_set(&ns->ipc_set);
		return false;
	}

	return true;
}

void retire_ipc_sysctls(struct ipc_namespace *ns)
{
	struct ctl_table *tbl;

	tbl = ns->ipc_sysctls->ctl_table_arg;
	unregister_sysctl_table(ns->ipc_sysctls);
	retire_sysctl_set(&ns->ipc_set);
	kfree(tbl);
}

static int __init ipc_sysctl_init(void)
{
	if (!setup_ipc_sysctls(&init_ipc_ns)) {
		pr_warn("ipc sysctl registration failed\n");
		return -ENOMEM;
	}
	return 0;
}

device_initcall(ipc_sysctl_init);

static int __init ipc_mni_extend(char *str)
{
	ipc_mni = IPCMNI_EXTEND;
	ipc_mni_shift = IPCMNI_EXTEND_SHIFT;
	ipc_min_cycle = IPCMNI_EXTEND_MIN_CYCLE;
	pr_info("IPCMNI extended to %d.\n", ipc_mni);
	return 0;
}
early_param("ipcmni_extend", ipc_mni_extend);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
