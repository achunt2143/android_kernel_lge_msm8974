<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/ipc/msg.c
 * Copyright (C) 1992 Krishna Balasubramanian
 *
 * Removed all the remaining kerneld mess
 * Catch the -EFAULT stuff properly
 * Use GFP_KERNEL for messages as in 1.2
 * Fixed up the unchecked user space derefs
 * Copyright (C) 1998 Alan Cox & Andi Kleen
 *
 * /proc/sysvipc/msg support (c) 1999 Dragos Acostachioaie <dragos@iname.com>
 *
 * mostly rewritten, threaded and wake-one semantics added
 * MSGMAX limit removed, sysctl's added
 * (c) 1999 Manfred Spraul <manfred@colorfullife.com>
 *
 * support for audit of ipc object properties and permission changes
 * Dustin Kirkland <dustin.kirkland@us.ibm.com>
 *
 * namespaces support
 * OpenVZ, SWsoft Inc.
 * Pavel Emelianov <xemul@openvz.org>
 */

#include <linux/capability.h>
#include <linux/msg.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/wake_q.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/syscalls.h>
#include <linux/audit.h>
#include <linux/seq_file.h>
#include <linux/rwsem.h>
#include <linux/nsproxy.h>
#include <linux/ipc_namespace.h>
<<<<<<< HEAD

#include <asm/current.h>
#include <asm/uaccess.h>
#include "util.h"

/*
 * one msg_receiver structure for each sleeping receiver:
 */
=======
#include <linux/rhashtable.h>
#include <linux/percpu_counter.h>

#include <asm/current.h>
#include <linux/uaccess.h>
#include "util.h"

/* one msq_queue structure for each present queue on the system */
struct msg_queue {
	struct kern_ipc_perm q_perm;
	time64_t q_stime;		/* last msgsnd time */
	time64_t q_rtime;		/* last msgrcv time */
	time64_t q_ctime;		/* last change time */
	unsigned long q_cbytes;		/* current number of bytes on queue */
	unsigned long q_qnum;		/* number of messages in queue */
	unsigned long q_qbytes;		/* max number of bytes on queue */
	struct pid *q_lspid;		/* pid of last msgsnd */
	struct pid *q_lrpid;		/* last receive pid */

	struct list_head q_messages;
	struct list_head q_receivers;
	struct list_head q_senders;
} __randomize_layout;

/*
 * MSG_BARRIER Locking:
 *
 * Similar to the optimization used in ipc/mqueue.c, one syscall return path
 * does not acquire any locks when it sees that a message exists in
 * msg_receiver.r_msg. Therefore r_msg is set using smp_store_release()
 * and accessed using READ_ONCE()+smp_acquire__after_ctrl_dep(). In addition,
 * wake_q_add_safe() is used. See ipc/mqueue.c for more details
 */

/* one msg_receiver structure for each sleeping receiver */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct msg_receiver {
	struct list_head	r_list;
	struct task_struct	*r_tsk;

	int			r_mode;
	long			r_msgtype;
	long			r_maxsize;

<<<<<<< HEAD
	struct msg_msg		*volatile r_msg;
=======
	struct msg_msg		*r_msg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* one msg_sender for each sleeping sender */
struct msg_sender {
	struct list_head	list;
	struct task_struct	*tsk;
<<<<<<< HEAD
=======
	size_t                  msgsz;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define SEARCH_ANY		1
#define SEARCH_EQUAL		2
#define SEARCH_NOTEQUAL		3
#define SEARCH_LESSEQUAL	4
<<<<<<< HEAD

#define msg_ids(ns)	((ns)->ids[IPC_MSG_IDS])

#define msg_unlock(msq)		ipc_unlock(&(msq)->q_perm)

static void freeque(struct ipc_namespace *, struct kern_ipc_perm *);
static int newque(struct ipc_namespace *, struct ipc_params *);
#ifdef CONFIG_PROC_FS
static int sysvipc_msg_proc_show(struct seq_file *s, void *it);
#endif

/*
 * Scale msgmni with the available lowmem size: the memory dedicated to msg
 * queues should occupy at most 1/MSG_MEM_SCALE of lowmem.
 * Also take into account the number of nsproxies created so far.
 * This should be done staying within the (MSGMNI , IPCMNI/nr_ipc_ns) range.
 */
void recompute_msgmni(struct ipc_namespace *ns)
{
	struct sysinfo i;
	unsigned long allowed;
	int nb_ns;

	si_meminfo(&i);
	allowed = (((i.totalram - i.totalhigh) / MSG_MEM_SCALE) * i.mem_unit)
		/ MSGMNB;
	nb_ns = atomic_read(&nr_ipc_ns);
	allowed /= nb_ns;

	if (allowed < MSGMNI) {
		ns->msg_ctlmni = MSGMNI;
		return;
	}

	if (allowed > IPCMNI / nb_ns) {
		ns->msg_ctlmni = IPCMNI / nb_ns;
		return;
	}

	ns->msg_ctlmni = allowed;
}

void msg_init_ns(struct ipc_namespace *ns)
{
	ns->msg_ctlmax = MSGMAX;
	ns->msg_ctlmnb = MSGMNB;

	recompute_msgmni(ns);

	atomic_set(&ns->msg_bytes, 0);
	atomic_set(&ns->msg_hdrs, 0);
	ipc_init_ids(&ns->ids[IPC_MSG_IDS]);
}

#ifdef CONFIG_IPC_NS
void msg_exit_ns(struct ipc_namespace *ns)
{
	free_ipcs(ns, &msg_ids(ns), freeque);
	idr_destroy(&ns->ids[IPC_MSG_IDS].ipcs_idr);
}
#endif

void __init msg_init(void)
{
	msg_init_ns(&init_ipc_ns);

	printk(KERN_INFO "msgmni has been set to %d\n",
		init_ipc_ns.msg_ctlmni);

	ipc_init_proc_interface("sysvipc/msg",
				"       key      msqid perms      cbytes       qnum lspid lrpid   uid   gid  cuid  cgid      stime      rtime      ctime\n",
				IPC_MSG_IDS, sysvipc_msg_proc_show);
}

/*
 * msg_lock_(check_) routines are called in the paths where the rw_mutex
 * is not held.
 */
static inline struct msg_queue *msg_lock(struct ipc_namespace *ns, int id)
{
	struct kern_ipc_perm *ipcp = ipc_lock(&msg_ids(ns), id);

	if (IS_ERR(ipcp))
		return (struct msg_queue *)ipcp;
=======
#define SEARCH_NUMBER		5

#define msg_ids(ns)	((ns)->ids[IPC_MSG_IDS])

static inline struct msg_queue *msq_obtain_object(struct ipc_namespace *ns, int id)
{
	struct kern_ipc_perm *ipcp = ipc_obtain_object_idr(&msg_ids(ns), id);

	if (IS_ERR(ipcp))
		return ERR_CAST(ipcp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return container_of(ipcp, struct msg_queue, q_perm);
}

<<<<<<< HEAD
static inline struct msg_queue *msg_lock_check(struct ipc_namespace *ns,
						int id)
{
	struct kern_ipc_perm *ipcp = ipc_lock_check(&msg_ids(ns), id);

	if (IS_ERR(ipcp))
		return (struct msg_queue *)ipcp;
=======
static inline struct msg_queue *msq_obtain_object_check(struct ipc_namespace *ns,
							int id)
{
	struct kern_ipc_perm *ipcp = ipc_obtain_object_check(&msg_ids(ns), id);

	if (IS_ERR(ipcp))
		return ERR_CAST(ipcp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return container_of(ipcp, struct msg_queue, q_perm);
}

static inline void msg_rmid(struct ipc_namespace *ns, struct msg_queue *s)
{
	ipc_rmid(&msg_ids(ns), &s->q_perm);
}

<<<<<<< HEAD
=======
static void msg_rcu_free(struct rcu_head *head)
{
	struct kern_ipc_perm *p = container_of(head, struct kern_ipc_perm, rcu);
	struct msg_queue *msq = container_of(p, struct msg_queue, q_perm);

	security_msg_queue_free(&msq->q_perm);
	kfree(msq);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * newque - Create a new msg queue
 * @ns: namespace
 * @params: ptr to the structure that contains the key and msgflg
 *
<<<<<<< HEAD
 * Called with msg_ids.rw_mutex held (writer)
=======
 * Called with msg_ids.rwsem held (writer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int newque(struct ipc_namespace *ns, struct ipc_params *params)
{
	struct msg_queue *msq;
<<<<<<< HEAD
	int id, retval;
	key_t key = params->key;
	int msgflg = params->flg;

	msq = ipc_rcu_alloc(sizeof(*msq));
	if (!msq)
=======
	int retval;
	key_t key = params->key;
	int msgflg = params->flg;

	msq = kmalloc(sizeof(*msq), GFP_KERNEL_ACCOUNT);
	if (unlikely(!msq))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;

	msq->q_perm.mode = msgflg & S_IRWXUGO;
	msq->q_perm.key = key;

	msq->q_perm.security = NULL;
<<<<<<< HEAD
	retval = security_msg_queue_alloc(msq);
	if (retval) {
		ipc_rcu_putref(msq);
=======
	retval = security_msg_queue_alloc(&msq->q_perm);
	if (retval) {
		kfree(msq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return retval;
	}

	msq->q_stime = msq->q_rtime = 0;
<<<<<<< HEAD
	msq->q_ctime = get_seconds();
	msq->q_cbytes = msq->q_qnum = 0;
	msq->q_qbytes = ns->msg_ctlmnb;
	msq->q_lspid = msq->q_lrpid = 0;
=======
	msq->q_ctime = ktime_get_real_seconds();
	msq->q_cbytes = msq->q_qnum = 0;
	msq->q_qbytes = ns->msg_ctlmnb;
	msq->q_lspid = msq->q_lrpid = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&msq->q_messages);
	INIT_LIST_HEAD(&msq->q_receivers);
	INIT_LIST_HEAD(&msq->q_senders);

<<<<<<< HEAD
	/*
	 * ipc_addid() locks msq
	 */
	id = ipc_addid(&msg_ids(ns), &msq->q_perm, ns->msg_ctlmni);
	if (id < 0) {
		security_msg_queue_free(msq);
		ipc_rcu_putref(msq);
		return id;
	}

	msg_unlock(msq);
=======
	/* ipc_addid() locks msq upon success. */
	retval = ipc_addid(&msg_ids(ns), &msq->q_perm, ns->msg_ctlmni);
	if (retval < 0) {
		ipc_rcu_putref(&msq->q_perm, msg_rcu_free);
		return retval;
	}

	ipc_unlock_object(&msq->q_perm);
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return msq->q_perm.id;
}

<<<<<<< HEAD
static inline void ss_add(struct msg_queue *msq, struct msg_sender *mss)
{
	mss->tsk = current;
	current->state = TASK_INTERRUPTIBLE;
=======
static inline bool msg_fits_inqueue(struct msg_queue *msq, size_t msgsz)
{
	return msgsz + msq->q_cbytes <= msq->q_qbytes &&
		1 + msq->q_qnum <= msq->q_qbytes;
}

static inline void ss_add(struct msg_queue *msq,
			  struct msg_sender *mss, size_t msgsz)
{
	mss->tsk = current;
	mss->msgsz = msgsz;
	/*
	 * No memory barrier required: we did ipc_lock_object(),
	 * and the waker obtains that lock before calling wake_q_add().
	 */
	__set_current_state(TASK_INTERRUPTIBLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_add_tail(&mss->list, &msq->q_senders);
}

static inline void ss_del(struct msg_sender *mss)
{
<<<<<<< HEAD
	if (mss->list.next != NULL)
		list_del(&mss->list);
}

static void ss_wakeup(struct list_head *h, int kill)
{
	struct list_head *tmp;

	tmp = h->next;
	while (tmp != h) {
		struct msg_sender *mss;

		mss = list_entry(tmp, struct msg_sender, list);
		tmp = tmp->next;
		if (kill)
			mss->list.next = NULL;
		wake_up_process(mss->tsk);
	}
}

static void expunge_all(struct msg_queue *msq, int res)
{
	struct list_head *tmp;

	tmp = msq->q_receivers.next;
	while (tmp != &msq->q_receivers) {
		struct msg_receiver *msr;

		msr = list_entry(tmp, struct msg_receiver, r_list);
		tmp = tmp->next;
		msr->r_msg = NULL;
		wake_up_process(msr->r_tsk);
		smp_mb();
		msr->r_msg = ERR_PTR(res);
=======
	if (mss->list.next)
		list_del(&mss->list);
}

static void ss_wakeup(struct msg_queue *msq,
		      struct wake_q_head *wake_q, bool kill)
{
	struct msg_sender *mss, *t;
	struct task_struct *stop_tsk = NULL;
	struct list_head *h = &msq->q_senders;

	list_for_each_entry_safe(mss, t, h, list) {
		if (kill)
			mss->list.next = NULL;

		/*
		 * Stop at the first task we don't wakeup,
		 * we've already iterated the original
		 * sender queue.
		 */
		else if (stop_tsk == mss->tsk)
			break;
		/*
		 * We are not in an EIDRM scenario here, therefore
		 * verify that we really need to wakeup the task.
		 * To maintain current semantics and wakeup order,
		 * move the sender to the tail on behalf of the
		 * blocked task.
		 */
		else if (!msg_fits_inqueue(msq, mss->msgsz)) {
			if (!stop_tsk)
				stop_tsk = mss->tsk;

			list_move_tail(&mss->list, &msq->q_senders);
			continue;
		}

		wake_q_add(wake_q, mss->tsk);
	}
}

static void expunge_all(struct msg_queue *msq, int res,
			struct wake_q_head *wake_q)
{
	struct msg_receiver *msr, *t;

	list_for_each_entry_safe(msr, t, &msq->q_receivers, r_list) {
		struct task_struct *r_tsk;

		r_tsk = get_task_struct(msr->r_tsk);

		/* see MSG_BARRIER for purpose/pairing */
		smp_store_release(&msr->r_msg, ERR_PTR(res));
		wake_q_add_safe(wake_q, r_tsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * freeque() wakes up waiters on the sender and receiver waiting queue,
 * removes the message queue from message queue ID IDR, and cleans up all the
 * messages associated with this queue.
 *
<<<<<<< HEAD
 * msg_ids.rw_mutex (writer) and the spinlock for this message queue are held
 * before freeque() is called. msg_ids.rw_mutex remains locked on exit.
 */
static void freeque(struct ipc_namespace *ns, struct kern_ipc_perm *ipcp)
{
	struct list_head *tmp;
	struct msg_queue *msq = container_of(ipcp, struct msg_queue, q_perm);

	expunge_all(msq, -EIDRM);
	ss_wakeup(&msq->q_senders, 1);
	msg_rmid(ns, msq);
	msg_unlock(msq);

	tmp = msq->q_messages.next;
	while (tmp != &msq->q_messages) {
		struct msg_msg *msg = list_entry(tmp, struct msg_msg, m_list);

		tmp = tmp->next;
		atomic_dec(&ns->msg_hdrs);
		free_msg(msg);
	}
	atomic_sub(msq->q_cbytes, &ns->msg_bytes);
	security_msg_queue_free(msq);
	ipc_lock_by_ptr(&msq->q_perm);
	ipc_rcu_putref(msq);
	ipc_unlock(&msq->q_perm);
}

/*
 * Called with msg_ids.rw_mutex and ipcp locked.
 */
static inline int msg_security(struct kern_ipc_perm *ipcp, int msgflg)
{
	struct msg_queue *msq = container_of(ipcp, struct msg_queue, q_perm);

	return security_msg_queue_associate(msq, msgflg);
}

SYSCALL_DEFINE2(msgget, key_t, key, int, msgflg)
{
	struct ipc_namespace *ns;
	struct ipc_ops msg_ops;
=======
 * msg_ids.rwsem (writer) and the spinlock for this message queue are held
 * before freeque() is called. msg_ids.rwsem remains locked on exit.
 */
static void freeque(struct ipc_namespace *ns, struct kern_ipc_perm *ipcp)
	__releases(RCU)
	__releases(&msq->q_perm)
{
	struct msg_msg *msg, *t;
	struct msg_queue *msq = container_of(ipcp, struct msg_queue, q_perm);
	DEFINE_WAKE_Q(wake_q);

	expunge_all(msq, -EIDRM, &wake_q);
	ss_wakeup(msq, &wake_q, true);
	msg_rmid(ns, msq);
	ipc_unlock_object(&msq->q_perm);
	wake_up_q(&wake_q);
	rcu_read_unlock();

	list_for_each_entry_safe(msg, t, &msq->q_messages, m_list) {
		percpu_counter_sub_local(&ns->percpu_msg_hdrs, 1);
		free_msg(msg);
	}
	percpu_counter_sub_local(&ns->percpu_msg_bytes, msq->q_cbytes);
	ipc_update_pid(&msq->q_lspid, NULL);
	ipc_update_pid(&msq->q_lrpid, NULL);
	ipc_rcu_putref(&msq->q_perm, msg_rcu_free);
}

long ksys_msgget(key_t key, int msgflg)
{
	struct ipc_namespace *ns;
	static const struct ipc_ops msg_ops = {
		.getnew = newque,
		.associate = security_msg_queue_associate,
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ipc_params msg_params;

	ns = current->nsproxy->ipc_ns;

<<<<<<< HEAD
	msg_ops.getnew = newque;
	msg_ops.associate = msg_security;
	msg_ops.more_checks = NULL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msg_params.key = key;
	msg_params.flg = msgflg;

	return ipcget(ns, &msg_ids(ns), &msg_ops, &msg_params);
}

<<<<<<< HEAD
static inline unsigned long
copy_msqid_to_user(void __user *buf, struct msqid64_ds *in, int version)
{
	switch(version) {
=======
SYSCALL_DEFINE2(msgget, key_t, key, int, msgflg)
{
	return ksys_msgget(key, msgflg);
}

static inline unsigned long
copy_msqid_to_user(void __user *buf, struct msqid64_ds *in, int version)
{
	switch (version) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPC_64:
		return copy_to_user(buf, in, sizeof(*in));
	case IPC_OLD:
	{
		struct msqid_ds out;

		memset(&out, 0, sizeof(out));

		ipc64_perm_to_ipc_perm(&in->msg_perm, &out.msg_perm);

		out.msg_stime		= in->msg_stime;
		out.msg_rtime		= in->msg_rtime;
		out.msg_ctime		= in->msg_ctime;

		if (in->msg_cbytes > USHRT_MAX)
			out.msg_cbytes	= USHRT_MAX;
		else
			out.msg_cbytes	= in->msg_cbytes;
		out.msg_lcbytes		= in->msg_cbytes;

		if (in->msg_qnum > USHRT_MAX)
			out.msg_qnum	= USHRT_MAX;
		else
			out.msg_qnum	= in->msg_qnum;

		if (in->msg_qbytes > USHRT_MAX)
			out.msg_qbytes	= USHRT_MAX;
		else
			out.msg_qbytes	= in->msg_qbytes;
		out.msg_lqbytes		= in->msg_qbytes;

		out.msg_lspid		= in->msg_lspid;
		out.msg_lrpid		= in->msg_lrpid;

		return copy_to_user(buf, &out, sizeof(out));
	}
	default:
		return -EINVAL;
	}
}

static inline unsigned long
copy_msqid_from_user(struct msqid64_ds *out, void __user *buf, int version)
{
<<<<<<< HEAD
	switch(version) {
=======
	switch (version) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPC_64:
		if (copy_from_user(out, buf, sizeof(*out)))
			return -EFAULT;
		return 0;
	case IPC_OLD:
	{
		struct msqid_ds tbuf_old;

		if (copy_from_user(&tbuf_old, buf, sizeof(tbuf_old)))
			return -EFAULT;

<<<<<<< HEAD
		out->msg_perm.uid      	= tbuf_old.msg_perm.uid;
		out->msg_perm.gid      	= tbuf_old.msg_perm.gid;
		out->msg_perm.mode     	= tbuf_old.msg_perm.mode;
=======
		out->msg_perm.uid	= tbuf_old.msg_perm.uid;
		out->msg_perm.gid	= tbuf_old.msg_perm.gid;
		out->msg_perm.mode	= tbuf_old.msg_perm.mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (tbuf_old.msg_qbytes == 0)
			out->msg_qbytes	= tbuf_old.msg_lqbytes;
		else
			out->msg_qbytes	= tbuf_old.msg_qbytes;

		return 0;
	}
	default:
		return -EINVAL;
	}
}

/*
<<<<<<< HEAD
 * This function handles some msgctl commands which require the rw_mutex
 * to be held in write mode.
 * NOTE: no locks must be held, the rw_mutex is taken inside this function.
 */
static int msgctl_down(struct ipc_namespace *ns, int msqid, int cmd,
		       struct msqid_ds __user *buf, int version)
{
	struct kern_ipc_perm *ipcp;
	struct msqid64_ds uninitialized_var(msqid64);
	struct msg_queue *msq;
	int err;

	if (cmd == IPC_SET) {
		if (copy_msqid_from_user(&msqid64, buf, version))
			return -EFAULT;
	}

	ipcp = ipcctl_pre_down(ns, &msg_ids(ns), msqid, cmd,
			       &msqid64.msg_perm, msqid64.msg_qbytes);
	if (IS_ERR(ipcp))
		return PTR_ERR(ipcp);

	msq = container_of(ipcp, struct msg_queue, q_perm);

	err = security_msg_queue_msgctl(msq, cmd);
	if (err)
		goto out_unlock;

	switch (cmd) {
	case IPC_RMID:
		freeque(ns, ipcp);
		goto out_up;
	case IPC_SET:
		if (msqid64.msg_qbytes > ns->msg_ctlmnb &&
		    !capable(CAP_SYS_RESOURCE)) {
			err = -EPERM;
			goto out_unlock;
		}

		msq->q_qbytes = msqid64.msg_qbytes;

		ipc_update_perm(&msqid64.msg_perm, ipcp);
		msq->q_ctime = get_seconds();
		/* sleeping receivers might be excluded by
		 * stricter permissions.
		 */
		expunge_all(msq, -EAGAIN);
		/* sleeping senders might be able to send
		 * due to a larger queue size.
		 */
		ss_wakeup(&msq->q_senders, 0);
		break;
	default:
		err = -EINVAL;
	}
out_unlock:
	msg_unlock(msq);
out_up:
	up_write(&msg_ids(ns).rw_mutex);
	return err;
}

SYSCALL_DEFINE3(msgctl, int, msqid, int, cmd, struct msqid_ds __user *, buf)
{
	struct msg_queue *msq;
	int err, version;
	struct ipc_namespace *ns;
=======
 * This function handles some msgctl commands which require the rwsem
 * to be held in write mode.
 * NOTE: no locks must be held, the rwsem is taken inside this function.
 */
static int msgctl_down(struct ipc_namespace *ns, int msqid, int cmd,
			struct ipc64_perm *perm, int msg_qbytes)
{
	struct kern_ipc_perm *ipcp;
	struct msg_queue *msq;
	int err;

	down_write(&msg_ids(ns).rwsem);
	rcu_read_lock();

	ipcp = ipcctl_obtain_check(ns, &msg_ids(ns), msqid, cmd,
				      perm, msg_qbytes);
	if (IS_ERR(ipcp)) {
		err = PTR_ERR(ipcp);
		goto out_unlock1;
	}

	msq = container_of(ipcp, struct msg_queue, q_perm);

	err = security_msg_queue_msgctl(&msq->q_perm, cmd);
	if (err)
		goto out_unlock1;

	switch (cmd) {
	case IPC_RMID:
		ipc_lock_object(&msq->q_perm);
		/* freeque unlocks the ipc object and rcu */
		freeque(ns, ipcp);
		goto out_up;
	case IPC_SET:
	{
		DEFINE_WAKE_Q(wake_q);

		if (msg_qbytes > ns->msg_ctlmnb &&
		    !capable(CAP_SYS_RESOURCE)) {
			err = -EPERM;
			goto out_unlock1;
		}

		ipc_lock_object(&msq->q_perm);
		err = ipc_update_perm(perm, ipcp);
		if (err)
			goto out_unlock0;

		msq->q_qbytes = msg_qbytes;

		msq->q_ctime = ktime_get_real_seconds();
		/*
		 * Sleeping receivers might be excluded by
		 * stricter permissions.
		 */
		expunge_all(msq, -EAGAIN, &wake_q);
		/*
		 * Sleeping senders might be able to send
		 * due to a larger queue size.
		 */
		ss_wakeup(msq, &wake_q, false);
		ipc_unlock_object(&msq->q_perm);
		wake_up_q(&wake_q);

		goto out_unlock1;
	}
	default:
		err = -EINVAL;
		goto out_unlock1;
	}

out_unlock0:
	ipc_unlock_object(&msq->q_perm);
out_unlock1:
	rcu_read_unlock();
out_up:
	up_write(&msg_ids(ns).rwsem);
	return err;
}

static int msgctl_info(struct ipc_namespace *ns, int msqid,
			 int cmd, struct msginfo *msginfo)
{
	int err;
	int max_idx;

	/*
	 * We must not return kernel stack data.
	 * due to padding, it's not enough
	 * to set all member fields.
	 */
	err = security_msg_queue_msgctl(NULL, cmd);
	if (err)
		return err;

	memset(msginfo, 0, sizeof(*msginfo));
	msginfo->msgmni = ns->msg_ctlmni;
	msginfo->msgmax = ns->msg_ctlmax;
	msginfo->msgmnb = ns->msg_ctlmnb;
	msginfo->msgssz = MSGSSZ;
	msginfo->msgseg = MSGSEG;
	down_read(&msg_ids(ns).rwsem);
	if (cmd == MSG_INFO)
		msginfo->msgpool = msg_ids(ns).in_use;
	max_idx = ipc_get_maxidx(&msg_ids(ns));
	up_read(&msg_ids(ns).rwsem);
	if (cmd == MSG_INFO) {
		msginfo->msgmap = min_t(int,
				     percpu_counter_sum(&ns->percpu_msg_hdrs),
				     INT_MAX);
		msginfo->msgtql = min_t(int,
		                     percpu_counter_sum(&ns->percpu_msg_bytes),
				     INT_MAX);
	} else {
		msginfo->msgmap = MSGMAP;
		msginfo->msgpool = MSGPOOL;
		msginfo->msgtql = MSGTQL;
	}
	return (max_idx < 0) ? 0 : max_idx;
}

static int msgctl_stat(struct ipc_namespace *ns, int msqid,
			 int cmd, struct msqid64_ds *p)
{
	struct msg_queue *msq;
	int err;

	memset(p, 0, sizeof(*p));

	rcu_read_lock();
	if (cmd == MSG_STAT || cmd == MSG_STAT_ANY) {
		msq = msq_obtain_object(ns, msqid);
		if (IS_ERR(msq)) {
			err = PTR_ERR(msq);
			goto out_unlock;
		}
	} else { /* IPC_STAT */
		msq = msq_obtain_object_check(ns, msqid);
		if (IS_ERR(msq)) {
			err = PTR_ERR(msq);
			goto out_unlock;
		}
	}

	/* see comment for SHM_STAT_ANY */
	if (cmd == MSG_STAT_ANY)
		audit_ipc_obj(&msq->q_perm);
	else {
		err = -EACCES;
		if (ipcperms(ns, &msq->q_perm, S_IRUGO))
			goto out_unlock;
	}

	err = security_msg_queue_msgctl(&msq->q_perm, cmd);
	if (err)
		goto out_unlock;

	ipc_lock_object(&msq->q_perm);

	if (!ipc_valid_object(&msq->q_perm)) {
		ipc_unlock_object(&msq->q_perm);
		err = -EIDRM;
		goto out_unlock;
	}

	kernel_to_ipc64_perm(&msq->q_perm, &p->msg_perm);
	p->msg_stime  = msq->q_stime;
	p->msg_rtime  = msq->q_rtime;
	p->msg_ctime  = msq->q_ctime;
#ifndef CONFIG_64BIT
	p->msg_stime_high = msq->q_stime >> 32;
	p->msg_rtime_high = msq->q_rtime >> 32;
	p->msg_ctime_high = msq->q_ctime >> 32;
#endif
	p->msg_cbytes = msq->q_cbytes;
	p->msg_qnum   = msq->q_qnum;
	p->msg_qbytes = msq->q_qbytes;
	p->msg_lspid  = pid_vnr(msq->q_lspid);
	p->msg_lrpid  = pid_vnr(msq->q_lrpid);

	if (cmd == IPC_STAT) {
		/*
		 * As defined in SUS:
		 * Return 0 on success
		 */
		err = 0;
	} else {
		/*
		 * MSG_STAT and MSG_STAT_ANY (both Linux specific)
		 * Return the full id, including the sequence number
		 */
		err = msq->q_perm.id;
	}

	ipc_unlock_object(&msq->q_perm);
out_unlock:
	rcu_read_unlock();
	return err;
}

static long ksys_msgctl(int msqid, int cmd, struct msqid_ds __user *buf, int version)
{
	struct ipc_namespace *ns;
	struct msqid64_ds msqid64;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (msqid < 0 || cmd < 0)
		return -EINVAL;

<<<<<<< HEAD
	version = ipc_parse_version(&cmd);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ns = current->nsproxy->ipc_ns;

	switch (cmd) {
	case IPC_INFO:
<<<<<<< HEAD
	case MSG_INFO:
	{
		struct msginfo msginfo;
		int max_id;

		if (!buf)
			return -EFAULT;
		/*
		 * We must not return kernel stack data.
		 * due to padding, it's not enough
		 * to set all member fields.
		 */
		err = security_msg_queue_msgctl(NULL, cmd);
		if (err)
			return err;

		memset(&msginfo, 0, sizeof(msginfo));
		msginfo.msgmni = ns->msg_ctlmni;
		msginfo.msgmax = ns->msg_ctlmax;
		msginfo.msgmnb = ns->msg_ctlmnb;
		msginfo.msgssz = MSGSSZ;
		msginfo.msgseg = MSGSEG;
		down_read(&msg_ids(ns).rw_mutex);
		if (cmd == MSG_INFO) {
			msginfo.msgpool = msg_ids(ns).in_use;
			msginfo.msgmap = atomic_read(&ns->msg_hdrs);
			msginfo.msgtql = atomic_read(&ns->msg_bytes);
		} else {
			msginfo.msgmap = MSGMAP;
			msginfo.msgpool = MSGPOOL;
			msginfo.msgtql = MSGTQL;
		}
		max_id = ipc_get_maxid(&msg_ids(ns));
		up_read(&msg_ids(ns).rw_mutex);
		if (copy_to_user(buf, &msginfo, sizeof(struct msginfo)))
			return -EFAULT;
		return (max_id < 0) ? 0 : max_id;
	}
	case MSG_STAT:	/* msqid is an index rather than a msg queue id */
	case IPC_STAT:
	{
		struct msqid64_ds tbuf;
		int success_return;

		if (!buf)
			return -EFAULT;

		if (cmd == MSG_STAT) {
			msq = msg_lock(ns, msqid);
			if (IS_ERR(msq))
				return PTR_ERR(msq);
			success_return = msq->q_perm.id;
		} else {
			msq = msg_lock_check(ns, msqid);
			if (IS_ERR(msq))
				return PTR_ERR(msq);
			success_return = 0;
		}
		err = -EACCES;
		if (ipcperms(ns, &msq->q_perm, S_IRUGO))
			goto out_unlock;

		err = security_msg_queue_msgctl(msq, cmd);
		if (err)
			goto out_unlock;

		memset(&tbuf, 0, sizeof(tbuf));

		kernel_to_ipc64_perm(&msq->q_perm, &tbuf.msg_perm);
		tbuf.msg_stime  = msq->q_stime;
		tbuf.msg_rtime  = msq->q_rtime;
		tbuf.msg_ctime  = msq->q_ctime;
		tbuf.msg_cbytes = msq->q_cbytes;
		tbuf.msg_qnum   = msq->q_qnum;
		tbuf.msg_qbytes = msq->q_qbytes;
		tbuf.msg_lspid  = msq->q_lspid;
		tbuf.msg_lrpid  = msq->q_lrpid;
		msg_unlock(msq);
		if (copy_msqid_to_user(buf, &tbuf, version))
			return -EFAULT;
		return success_return;
	}
	case IPC_SET:
	case IPC_RMID:
		err = msgctl_down(ns, msqid, cmd, buf, version);
		return err;
	default:
		return  -EINVAL;
	}

out_unlock:
	msg_unlock(msq);
	return err;
}

static int testmsg(struct msg_msg *msg, long type, int mode)
{
	switch(mode)
	{
		case SEARCH_ANY:
			return 1;
		case SEARCH_LESSEQUAL:
			if (msg->m_type <=type)
				return 1;
			break;
		case SEARCH_EQUAL:
			if (msg->m_type == type)
				return 1;
			break;
		case SEARCH_NOTEQUAL:
			if (msg->m_type != type)
				return 1;
			break;
=======
	case MSG_INFO: {
		struct msginfo msginfo;
		err = msgctl_info(ns, msqid, cmd, &msginfo);
		if (err < 0)
			return err;
		if (copy_to_user(buf, &msginfo, sizeof(struct msginfo)))
			err = -EFAULT;
		return err;
	}
	case MSG_STAT:	/* msqid is an index rather than a msg queue id */
	case MSG_STAT_ANY:
	case IPC_STAT:
		err = msgctl_stat(ns, msqid, cmd, &msqid64);
		if (err < 0)
			return err;
		if (copy_msqid_to_user(buf, &msqid64, version))
			err = -EFAULT;
		return err;
	case IPC_SET:
		if (copy_msqid_from_user(&msqid64, buf, version))
			return -EFAULT;
		return msgctl_down(ns, msqid, cmd, &msqid64.msg_perm,
				   msqid64.msg_qbytes);
	case IPC_RMID:
		return msgctl_down(ns, msqid, cmd, NULL, 0);
	default:
		return  -EINVAL;
	}
}

SYSCALL_DEFINE3(msgctl, int, msqid, int, cmd, struct msqid_ds __user *, buf)
{
	return ksys_msgctl(msqid, cmd, buf, IPC_64);
}

#ifdef CONFIG_ARCH_WANT_IPC_PARSE_VERSION
long ksys_old_msgctl(int msqid, int cmd, struct msqid_ds __user *buf)
{
	int version = ipc_parse_version(&cmd);

	return ksys_msgctl(msqid, cmd, buf, version);
}

SYSCALL_DEFINE3(old_msgctl, int, msqid, int, cmd, struct msqid_ds __user *, buf)
{
	return ksys_old_msgctl(msqid, cmd, buf);
}
#endif

#ifdef CONFIG_COMPAT

struct compat_msqid_ds {
	struct compat_ipc_perm msg_perm;
	compat_uptr_t msg_first;
	compat_uptr_t msg_last;
	old_time32_t msg_stime;
	old_time32_t msg_rtime;
	old_time32_t msg_ctime;
	compat_ulong_t msg_lcbytes;
	compat_ulong_t msg_lqbytes;
	unsigned short msg_cbytes;
	unsigned short msg_qnum;
	unsigned short msg_qbytes;
	compat_ipc_pid_t msg_lspid;
	compat_ipc_pid_t msg_lrpid;
};

static int copy_compat_msqid_from_user(struct msqid64_ds *out, void __user *buf,
					int version)
{
	memset(out, 0, sizeof(*out));
	if (version == IPC_64) {
		struct compat_msqid64_ds __user *p = buf;
		if (get_compat_ipc64_perm(&out->msg_perm, &p->msg_perm))
			return -EFAULT;
		if (get_user(out->msg_qbytes, &p->msg_qbytes))
			return -EFAULT;
	} else {
		struct compat_msqid_ds __user *p = buf;
		if (get_compat_ipc_perm(&out->msg_perm, &p->msg_perm))
			return -EFAULT;
		if (get_user(out->msg_qbytes, &p->msg_qbytes))
			return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
static inline int pipelined_send(struct msg_queue *msq, struct msg_msg *msg)
{
	struct list_head *tmp;

	tmp = msq->q_receivers.next;
	while (tmp != &msq->q_receivers) {
		struct msg_receiver *msr;

		msr = list_entry(tmp, struct msg_receiver, r_list);
		tmp = tmp->next;
		if (testmsg(msg, msr->r_msgtype, msr->r_mode) &&
		    !security_msg_queue_msgrcv(msq, msg, msr->r_tsk,
=======
static int copy_compat_msqid_to_user(void __user *buf, struct msqid64_ds *in,
					int version)
{
	if (version == IPC_64) {
		struct compat_msqid64_ds v;
		memset(&v, 0, sizeof(v));
		to_compat_ipc64_perm(&v.msg_perm, &in->msg_perm);
		v.msg_stime	 = lower_32_bits(in->msg_stime);
		v.msg_stime_high = upper_32_bits(in->msg_stime);
		v.msg_rtime	 = lower_32_bits(in->msg_rtime);
		v.msg_rtime_high = upper_32_bits(in->msg_rtime);
		v.msg_ctime	 = lower_32_bits(in->msg_ctime);
		v.msg_ctime_high = upper_32_bits(in->msg_ctime);
		v.msg_cbytes = in->msg_cbytes;
		v.msg_qnum = in->msg_qnum;
		v.msg_qbytes = in->msg_qbytes;
		v.msg_lspid = in->msg_lspid;
		v.msg_lrpid = in->msg_lrpid;
		return copy_to_user(buf, &v, sizeof(v));
	} else {
		struct compat_msqid_ds v;
		memset(&v, 0, sizeof(v));
		to_compat_ipc_perm(&v.msg_perm, &in->msg_perm);
		v.msg_stime = in->msg_stime;
		v.msg_rtime = in->msg_rtime;
		v.msg_ctime = in->msg_ctime;
		v.msg_cbytes = in->msg_cbytes;
		v.msg_qnum = in->msg_qnum;
		v.msg_qbytes = in->msg_qbytes;
		v.msg_lspid = in->msg_lspid;
		v.msg_lrpid = in->msg_lrpid;
		return copy_to_user(buf, &v, sizeof(v));
	}
}

static long compat_ksys_msgctl(int msqid, int cmd, void __user *uptr, int version)
{
	struct ipc_namespace *ns;
	int err;
	struct msqid64_ds msqid64;

	ns = current->nsproxy->ipc_ns;

	if (msqid < 0 || cmd < 0)
		return -EINVAL;

	switch (cmd & (~IPC_64)) {
	case IPC_INFO:
	case MSG_INFO: {
		struct msginfo msginfo;
		err = msgctl_info(ns, msqid, cmd, &msginfo);
		if (err < 0)
			return err;
		if (copy_to_user(uptr, &msginfo, sizeof(struct msginfo)))
			err = -EFAULT;
		return err;
	}
	case IPC_STAT:
	case MSG_STAT:
	case MSG_STAT_ANY:
		err = msgctl_stat(ns, msqid, cmd, &msqid64);
		if (err < 0)
			return err;
		if (copy_compat_msqid_to_user(uptr, &msqid64, version))
			err = -EFAULT;
		return err;
	case IPC_SET:
		if (copy_compat_msqid_from_user(&msqid64, uptr, version))
			return -EFAULT;
		return msgctl_down(ns, msqid, cmd, &msqid64.msg_perm, msqid64.msg_qbytes);
	case IPC_RMID:
		return msgctl_down(ns, msqid, cmd, NULL, 0);
	default:
		return -EINVAL;
	}
}

COMPAT_SYSCALL_DEFINE3(msgctl, int, msqid, int, cmd, void __user *, uptr)
{
	return compat_ksys_msgctl(msqid, cmd, uptr, IPC_64);
}

#ifdef CONFIG_ARCH_WANT_COMPAT_IPC_PARSE_VERSION
long compat_ksys_old_msgctl(int msqid, int cmd, void __user *uptr)
{
	int version = compat_ipc_parse_version(&cmd);

	return compat_ksys_msgctl(msqid, cmd, uptr, version);
}

COMPAT_SYSCALL_DEFINE3(old_msgctl, int, msqid, int, cmd, void __user *, uptr)
{
	return compat_ksys_old_msgctl(msqid, cmd, uptr);
}
#endif
#endif

static int testmsg(struct msg_msg *msg, long type, int mode)
{
	switch (mode) {
	case SEARCH_ANY:
	case SEARCH_NUMBER:
		return 1;
	case SEARCH_LESSEQUAL:
		if (msg->m_type <= type)
			return 1;
		break;
	case SEARCH_EQUAL:
		if (msg->m_type == type)
			return 1;
		break;
	case SEARCH_NOTEQUAL:
		if (msg->m_type != type)
			return 1;
		break;
	}
	return 0;
}

static inline int pipelined_send(struct msg_queue *msq, struct msg_msg *msg,
				 struct wake_q_head *wake_q)
{
	struct msg_receiver *msr, *t;

	list_for_each_entry_safe(msr, t, &msq->q_receivers, r_list) {
		if (testmsg(msg, msr->r_msgtype, msr->r_mode) &&
		    !security_msg_queue_msgrcv(&msq->q_perm, msg, msr->r_tsk,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					       msr->r_msgtype, msr->r_mode)) {

			list_del(&msr->r_list);
			if (msr->r_maxsize < msg->m_ts) {
<<<<<<< HEAD
				msr->r_msg = NULL;
				wake_up_process(msr->r_tsk);
				smp_mb();
				msr->r_msg = ERR_PTR(-E2BIG);
			} else {
				msr->r_msg = NULL;
				msq->q_lrpid = task_pid_vnr(msr->r_tsk);
				msq->q_rtime = get_seconds();
				wake_up_process(msr->r_tsk);
				smp_mb();
				msr->r_msg = msg;

=======
				wake_q_add(wake_q, msr->r_tsk);

				/* See expunge_all regarding memory barrier */
				smp_store_release(&msr->r_msg, ERR_PTR(-E2BIG));
			} else {
				ipc_update_pid(&msq->q_lrpid, task_pid(msr->r_tsk));
				msq->q_rtime = ktime_get_real_seconds();

				wake_q_add(wake_q, msr->r_tsk);

				/* See expunge_all regarding memory barrier */
				smp_store_release(&msr->r_msg, msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return 1;
			}
		}
	}
<<<<<<< HEAD
	return 0;
}

long do_msgsnd(int msqid, long mtype, void __user *mtext,
=======

	return 0;
}

static long do_msgsnd(int msqid, long mtype, void __user *mtext,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		size_t msgsz, int msgflg)
{
	struct msg_queue *msq;
	struct msg_msg *msg;
	int err;
	struct ipc_namespace *ns;
<<<<<<< HEAD
=======
	DEFINE_WAKE_Q(wake_q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ns = current->nsproxy->ipc_ns;

	if (msgsz > ns->msg_ctlmax || (long) msgsz < 0 || msqid < 0)
		return -EINVAL;
	if (mtype < 1)
		return -EINVAL;

	msg = load_msg(mtext, msgsz);
	if (IS_ERR(msg))
		return PTR_ERR(msg);

	msg->m_type = mtype;
	msg->m_ts = msgsz;

<<<<<<< HEAD
	msq = msg_lock_check(ns, msqid);
	if (IS_ERR(msq)) {
		err = PTR_ERR(msq);
		goto out_free;
	}

=======
	rcu_read_lock();
	msq = msq_obtain_object_check(ns, msqid);
	if (IS_ERR(msq)) {
		err = PTR_ERR(msq);
		goto out_unlock1;
	}

	ipc_lock_object(&msq->q_perm);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (;;) {
		struct msg_sender s;

		err = -EACCES;
		if (ipcperms(ns, &msq->q_perm, S_IWUGO))
<<<<<<< HEAD
			goto out_unlock_free;

		err = security_msg_queue_msgsnd(msq, msg, msgflg);
		if (err)
			goto out_unlock_free;

		if (msgsz + msq->q_cbytes <= msq->q_qbytes &&
				1 + msq->q_qnum <= msq->q_qbytes) {
			break;
		}

		/* queue full, wait: */
		if (msgflg & IPC_NOWAIT) {
			err = -EAGAIN;
			goto out_unlock_free;
		}
		ss_add(msq, &s);
		ipc_rcu_getref(msq);
		msg_unlock(msq);
		schedule();

		ipc_lock_by_ptr(&msq->q_perm);
		ipc_rcu_putref(msq);
		if (msq->q_perm.deleted) {
			err = -EIDRM;
			goto out_unlock_free;
=======
			goto out_unlock0;

		/* raced with RMID? */
		if (!ipc_valid_object(&msq->q_perm)) {
			err = -EIDRM;
			goto out_unlock0;
		}

		err = security_msg_queue_msgsnd(&msq->q_perm, msg, msgflg);
		if (err)
			goto out_unlock0;

		if (msg_fits_inqueue(msq, msgsz))
			break;

		/* queue full, wait: */
		if (msgflg & IPC_NOWAIT) {
			err = -EAGAIN;
			goto out_unlock0;
		}

		/* enqueue the sender and prepare to block */
		ss_add(msq, &s, msgsz);

		if (!ipc_rcu_getref(&msq->q_perm)) {
			err = -EIDRM;
			goto out_unlock0;
		}

		ipc_unlock_object(&msq->q_perm);
		rcu_read_unlock();
		schedule();

		rcu_read_lock();
		ipc_lock_object(&msq->q_perm);

		ipc_rcu_putref(&msq->q_perm, msg_rcu_free);
		/* raced with RMID? */
		if (!ipc_valid_object(&msq->q_perm)) {
			err = -EIDRM;
			goto out_unlock0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		ss_del(&s);

		if (signal_pending(current)) {
			err = -ERESTARTNOHAND;
<<<<<<< HEAD
			goto out_unlock_free;
		}
	}

	msq->q_lspid = task_tgid_vnr(current);
	msq->q_stime = get_seconds();

	if (!pipelined_send(msq, msg)) {
=======
			goto out_unlock0;
		}

	}

	ipc_update_pid(&msq->q_lspid, task_tgid(current));
	msq->q_stime = ktime_get_real_seconds();

	if (!pipelined_send(msq, msg, &wake_q)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* no one is waiting for this message, enqueue it */
		list_add_tail(&msg->m_list, &msq->q_messages);
		msq->q_cbytes += msgsz;
		msq->q_qnum++;
<<<<<<< HEAD
		atomic_add(msgsz, &ns->msg_bytes);
		atomic_inc(&ns->msg_hdrs);
=======
		percpu_counter_add_local(&ns->percpu_msg_bytes, msgsz);
		percpu_counter_add_local(&ns->percpu_msg_hdrs, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = 0;
	msg = NULL;

<<<<<<< HEAD
out_unlock_free:
	msg_unlock(msq);
out_free:
=======
out_unlock0:
	ipc_unlock_object(&msq->q_perm);
	wake_up_q(&wake_q);
out_unlock1:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (msg != NULL)
		free_msg(msg);
	return err;
}

<<<<<<< HEAD
SYSCALL_DEFINE4(msgsnd, int, msqid, struct msgbuf __user *, msgp, size_t, msgsz,
		int, msgflg)
=======
long ksys_msgsnd(int msqid, struct msgbuf __user *msgp, size_t msgsz,
		 int msgflg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	long mtype;

	if (get_user(mtype, &msgp->mtype))
		return -EFAULT;
	return do_msgsnd(msqid, mtype, msgp->mtext, msgsz, msgflg);
}

<<<<<<< HEAD
static inline int convert_mode(long *msgtyp, int msgflg)
{
=======
SYSCALL_DEFINE4(msgsnd, int, msqid, struct msgbuf __user *, msgp, size_t, msgsz,
		int, msgflg)
{
	return ksys_msgsnd(msqid, msgp, msgsz, msgflg);
}

#ifdef CONFIG_COMPAT

struct compat_msgbuf {
	compat_long_t mtype;
	char mtext[1];
};

long compat_ksys_msgsnd(int msqid, compat_uptr_t msgp,
		       compat_ssize_t msgsz, int msgflg)
{
	struct compat_msgbuf __user *up = compat_ptr(msgp);
	compat_long_t mtype;

	if (get_user(mtype, &up->mtype))
		return -EFAULT;
	return do_msgsnd(msqid, mtype, up->mtext, (ssize_t)msgsz, msgflg);
}

COMPAT_SYSCALL_DEFINE4(msgsnd, int, msqid, compat_uptr_t, msgp,
		       compat_ssize_t, msgsz, int, msgflg)
{
	return compat_ksys_msgsnd(msqid, msgp, msgsz, msgflg);
}
#endif

static inline int convert_mode(long *msgtyp, int msgflg)
{
	if (msgflg & MSG_COPY)
		return SEARCH_NUMBER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *  find message of correct type.
	 *  msgtyp = 0 => get first.
	 *  msgtyp > 0 => get first message of matching type.
	 *  msgtyp < 0 => get message with least type must be < abs(msgtype).
	 */
	if (*msgtyp == 0)
		return SEARCH_ANY;
	if (*msgtyp < 0) {
<<<<<<< HEAD
		*msgtyp = -*msgtyp;
=======
		if (*msgtyp == LONG_MIN) /* -LONG_MIN is undefined */
			*msgtyp = LONG_MAX;
		else
			*msgtyp = -*msgtyp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return SEARCH_LESSEQUAL;
	}
	if (msgflg & MSG_EXCEPT)
		return SEARCH_NOTEQUAL;
	return SEARCH_EQUAL;
}

<<<<<<< HEAD
long do_msgrcv(int msqid, long *pmtype, void __user *mtext,
		size_t msgsz, long msgtyp, int msgflg)
{
	struct msg_queue *msq;
	struct msg_msg *msg;
	int mode;
	struct ipc_namespace *ns;

	if (msqid < 0 || (long) msgsz < 0)
		return -EINVAL;
	mode = convert_mode(&msgtyp, msgflg);
	ns = current->nsproxy->ipc_ns;

	msq = msg_lock_check(ns, msqid);
	if (IS_ERR(msq))
		return PTR_ERR(msq);

	for (;;) {
		struct msg_receiver msr_d;
		struct list_head *tmp;

		msg = ERR_PTR(-EACCES);
		if (ipcperms(ns, &msq->q_perm, S_IRUGO))
			goto out_unlock;

		msg = ERR_PTR(-EAGAIN);
		tmp = msq->q_messages.next;
		while (tmp != &msq->q_messages) {
			struct msg_msg *walk_msg;

			walk_msg = list_entry(tmp, struct msg_msg, m_list);
			if (testmsg(walk_msg, msgtyp, mode) &&
			    !security_msg_queue_msgrcv(msq, walk_msg, current,
						       msgtyp, mode)) {

				msg = walk_msg;
				if (mode == SEARCH_LESSEQUAL &&
						walk_msg->m_type != 1) {
					msg = walk_msg;
					msgtyp = walk_msg->m_type - 1;
				} else {
					msg = walk_msg;
					break;
				}
			}
			tmp = tmp->next;
		}
=======
static long do_msg_fill(void __user *dest, struct msg_msg *msg, size_t bufsz)
{
	struct msgbuf __user *msgp = dest;
	size_t msgsz;

	if (put_user(msg->m_type, &msgp->mtype))
		return -EFAULT;

	msgsz = (bufsz > msg->m_ts) ? msg->m_ts : bufsz;
	if (store_msg(msgp->mtext, msg, msgsz))
		return -EFAULT;
	return msgsz;
}

#ifdef CONFIG_CHECKPOINT_RESTORE
/*
 * This function creates new kernel message structure, large enough to store
 * bufsz message bytes.
 */
static inline struct msg_msg *prepare_copy(void __user *buf, size_t bufsz)
{
	struct msg_msg *copy;

	/*
	 * Create dummy message to copy real message to.
	 */
	copy = load_msg(buf, bufsz);
	if (!IS_ERR(copy))
		copy->m_ts = bufsz;
	return copy;
}

static inline void free_copy(struct msg_msg *copy)
{
	if (copy)
		free_msg(copy);
}
#else
static inline struct msg_msg *prepare_copy(void __user *buf, size_t bufsz)
{
	return ERR_PTR(-ENOSYS);
}

static inline void free_copy(struct msg_msg *copy)
{
}
#endif

static struct msg_msg *find_msg(struct msg_queue *msq, long *msgtyp, int mode)
{
	struct msg_msg *msg, *found = NULL;
	long count = 0;

	list_for_each_entry(msg, &msq->q_messages, m_list) {
		if (testmsg(msg, *msgtyp, mode) &&
		    !security_msg_queue_msgrcv(&msq->q_perm, msg, current,
					       *msgtyp, mode)) {
			if (mode == SEARCH_LESSEQUAL && msg->m_type != 1) {
				*msgtyp = msg->m_type - 1;
				found = msg;
			} else if (mode == SEARCH_NUMBER) {
				if (*msgtyp == count)
					return msg;
			} else
				return msg;
			count++;
		}
	}

	return found ?: ERR_PTR(-EAGAIN);
}

static long do_msgrcv(int msqid, void __user *buf, size_t bufsz, long msgtyp, int msgflg,
	       long (*msg_handler)(void __user *, struct msg_msg *, size_t))
{
	int mode;
	struct msg_queue *msq;
	struct ipc_namespace *ns;
	struct msg_msg *msg, *copy = NULL;
	DEFINE_WAKE_Q(wake_q);

	ns = current->nsproxy->ipc_ns;

	if (msqid < 0 || (long) bufsz < 0)
		return -EINVAL;

	if (msgflg & MSG_COPY) {
		if ((msgflg & MSG_EXCEPT) || !(msgflg & IPC_NOWAIT))
			return -EINVAL;
		copy = prepare_copy(buf, min_t(size_t, bufsz, ns->msg_ctlmax));
		if (IS_ERR(copy))
			return PTR_ERR(copy);
	}
	mode = convert_mode(&msgtyp, msgflg);

	rcu_read_lock();
	msq = msq_obtain_object_check(ns, msqid);
	if (IS_ERR(msq)) {
		rcu_read_unlock();
		free_copy(copy);
		return PTR_ERR(msq);
	}

	for (;;) {
		struct msg_receiver msr_d;

		msg = ERR_PTR(-EACCES);
		if (ipcperms(ns, &msq->q_perm, S_IRUGO))
			goto out_unlock1;

		ipc_lock_object(&msq->q_perm);

		/* raced with RMID? */
		if (!ipc_valid_object(&msq->q_perm)) {
			msg = ERR_PTR(-EIDRM);
			goto out_unlock0;
		}

		msg = find_msg(msq, &msgtyp, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!IS_ERR(msg)) {
			/*
			 * Found a suitable message.
			 * Unlink it from the queue.
			 */
<<<<<<< HEAD
			if ((msgsz < msg->m_ts) && !(msgflg & MSG_NOERROR)) {
				msg = ERR_PTR(-E2BIG);
				goto out_unlock;
			}
			list_del(&msg->m_list);
			msq->q_qnum--;
			msq->q_rtime = get_seconds();
			msq->q_lrpid = task_tgid_vnr(current);
			msq->q_cbytes -= msg->m_ts;
			atomic_sub(msg->m_ts, &ns->msg_bytes);
			atomic_dec(&ns->msg_hdrs);
			ss_wakeup(&msq->q_senders, 0);
			msg_unlock(msq);
			break;
		}
		/* No message waiting. Wait for a message */
		if (msgflg & IPC_NOWAIT) {
			msg = ERR_PTR(-ENOMSG);
			goto out_unlock;
		}
=======
			if ((bufsz < msg->m_ts) && !(msgflg & MSG_NOERROR)) {
				msg = ERR_PTR(-E2BIG);
				goto out_unlock0;
			}
			/*
			 * If we are copying, then do not unlink message and do
			 * not update queue parameters.
			 */
			if (msgflg & MSG_COPY) {
				msg = copy_msg(msg, copy);
				goto out_unlock0;
			}

			list_del(&msg->m_list);
			msq->q_qnum--;
			msq->q_rtime = ktime_get_real_seconds();
			ipc_update_pid(&msq->q_lrpid, task_tgid(current));
			msq->q_cbytes -= msg->m_ts;
			percpu_counter_sub_local(&ns->percpu_msg_bytes, msg->m_ts);
			percpu_counter_sub_local(&ns->percpu_msg_hdrs, 1);
			ss_wakeup(msq, &wake_q, false);

			goto out_unlock0;
		}

		/* No message waiting. Wait for a message */
		if (msgflg & IPC_NOWAIT) {
			msg = ERR_PTR(-ENOMSG);
			goto out_unlock0;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		list_add_tail(&msr_d.r_list, &msq->q_receivers);
		msr_d.r_tsk = current;
		msr_d.r_msgtype = msgtyp;
		msr_d.r_mode = mode;
		if (msgflg & MSG_NOERROR)
			msr_d.r_maxsize = INT_MAX;
		else
<<<<<<< HEAD
			msr_d.r_maxsize = msgsz;
		msr_d.r_msg = ERR_PTR(-EAGAIN);
		current->state = TASK_INTERRUPTIBLE;
		msg_unlock(msq);

		schedule();

		/* Lockless receive, part 1:
		 * Disable preemption.  We don't hold a reference to the queue
		 * and getting a reference would defeat the idea of a lockless
		 * operation, thus the code relies on rcu to guarantee the
		 * existence of msq:
		 * Prior to destruction, expunge_all(-EIRDM) changes r_msg.
		 * Thus if r_msg is -EAGAIN, then the queue not yet destroyed.
		 * rcu_read_lock() prevents preemption between reading r_msg
		 * and the spin_lock() inside ipc_lock_by_ptr().
		 */
		rcu_read_lock();

		/* Lockless receive, part 2:
		 * Wait until pipelined_send or expunge_all are outside of
		 * wake_up_process(). There is a race with exit(), see
		 * ipc/mqueue.c for the details.
		 */
		msg = (struct msg_msg*)msr_d.r_msg;
		while (msg == NULL) {
			cpu_relax();
			msg = (struct msg_msg *)msr_d.r_msg;
		}

		/* Lockless receive, part 3:
		 * If there is a message or an error then accept it without
		 * locking.
		 */
		if (msg != ERR_PTR(-EAGAIN)) {
			rcu_read_unlock();
			break;
		}

		/* Lockless receive, part 3:
		 * Acquire the queue spinlock.
		 */
		ipc_lock_by_ptr(&msq->q_perm);
		rcu_read_unlock();

		/* Lockless receive, part 4:
		 * Repeat test after acquiring the spinlock.
		 */
		msg = (struct msg_msg*)msr_d.r_msg;
		if (msg != ERR_PTR(-EAGAIN))
			goto out_unlock;
=======
			msr_d.r_maxsize = bufsz;

		/* memory barrier not require due to ipc_lock_object() */
		WRITE_ONCE(msr_d.r_msg, ERR_PTR(-EAGAIN));

		/* memory barrier not required, we own ipc_lock_object() */
		__set_current_state(TASK_INTERRUPTIBLE);

		ipc_unlock_object(&msq->q_perm);
		rcu_read_unlock();
		schedule();

		/*
		 * Lockless receive, part 1:
		 * We don't hold a reference to the queue and getting a
		 * reference would defeat the idea of a lockless operation,
		 * thus the code relies on rcu to guarantee the existence of
		 * msq:
		 * Prior to destruction, expunge_all(-EIRDM) changes r_msg.
		 * Thus if r_msg is -EAGAIN, then the queue not yet destroyed.
		 */
		rcu_read_lock();

		/*
		 * Lockless receive, part 2:
		 * The work in pipelined_send() and expunge_all():
		 * - Set pointer to message
		 * - Queue the receiver task for later wakeup
		 * - Wake up the process after the lock is dropped.
		 *
		 * Should the process wake up before this wakeup (due to a
		 * signal) it will either see the message and continue ...
		 */
		msg = READ_ONCE(msr_d.r_msg);
		if (msg != ERR_PTR(-EAGAIN)) {
			/* see MSG_BARRIER for purpose/pairing */
			smp_acquire__after_ctrl_dep();

			goto out_unlock1;
		}

		 /*
		  * ... or see -EAGAIN, acquire the lock to check the message
		  * again.
		  */
		ipc_lock_object(&msq->q_perm);

		msg = READ_ONCE(msr_d.r_msg);
		if (msg != ERR_PTR(-EAGAIN))
			goto out_unlock0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		list_del(&msr_d.r_list);
		if (signal_pending(current)) {
			msg = ERR_PTR(-ERESTARTNOHAND);
<<<<<<< HEAD
out_unlock:
			msg_unlock(msq);
			break;
		}
	}
	if (IS_ERR(msg))
		return PTR_ERR(msg);

	msgsz = (msgsz > msg->m_ts) ? msg->m_ts : msgsz;
	*pmtype = msg->m_type;
	if (store_msg(mtext, msg, msgsz))
		msgsz = -EFAULT;

	free_msg(msg);

	return msgsz;
=======
			goto out_unlock0;
		}

		ipc_unlock_object(&msq->q_perm);
	}

out_unlock0:
	ipc_unlock_object(&msq->q_perm);
	wake_up_q(&wake_q);
out_unlock1:
	rcu_read_unlock();
	if (IS_ERR(msg)) {
		free_copy(copy);
		return PTR_ERR(msg);
	}

	bufsz = msg_handler(buf, msg, bufsz);
	free_msg(msg);

	return bufsz;
}

long ksys_msgrcv(int msqid, struct msgbuf __user *msgp, size_t msgsz,
		 long msgtyp, int msgflg)
{
	return do_msgrcv(msqid, msgp, msgsz, msgtyp, msgflg, do_msg_fill);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

SYSCALL_DEFINE5(msgrcv, int, msqid, struct msgbuf __user *, msgp, size_t, msgsz,
		long, msgtyp, int, msgflg)
{
<<<<<<< HEAD
	long err, mtype;

	err =  do_msgrcv(msqid, &mtype, msgp->mtext, msgsz, msgtyp, msgflg);
	if (err < 0)
		goto out;

	if (put_user(mtype, &msgp->mtype))
		err = -EFAULT;
out:
	return err;
}

#ifdef CONFIG_PROC_FS
static int sysvipc_msg_proc_show(struct seq_file *s, void *it)
{
	struct msg_queue *msq = it;

	return seq_printf(s,
			"%10d %10d  %4o  %10lu %10lu %5u %5u %5u %5u %5u %5u %10lu %10lu %10lu\n",
			msq->q_perm.key,
			msq->q_perm.id,
			msq->q_perm.mode,
			msq->q_cbytes,
			msq->q_qnum,
			msq->q_lspid,
			msq->q_lrpid,
			msq->q_perm.uid,
			msq->q_perm.gid,
			msq->q_perm.cuid,
			msq->q_perm.cgid,
			msq->q_stime,
			msq->q_rtime,
			msq->q_ctime);
}
#endif
=======
	return ksys_msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
}

#ifdef CONFIG_COMPAT
static long compat_do_msg_fill(void __user *dest, struct msg_msg *msg, size_t bufsz)
{
	struct compat_msgbuf __user *msgp = dest;
	size_t msgsz;

	if (put_user(msg->m_type, &msgp->mtype))
		return -EFAULT;

	msgsz = (bufsz > msg->m_ts) ? msg->m_ts : bufsz;
	if (store_msg(msgp->mtext, msg, msgsz))
		return -EFAULT;
	return msgsz;
}

long compat_ksys_msgrcv(int msqid, compat_uptr_t msgp, compat_ssize_t msgsz,
			compat_long_t msgtyp, int msgflg)
{
	return do_msgrcv(msqid, compat_ptr(msgp), (ssize_t)msgsz, (long)msgtyp,
			 msgflg, compat_do_msg_fill);
}

COMPAT_SYSCALL_DEFINE5(msgrcv, int, msqid, compat_uptr_t, msgp,
		       compat_ssize_t, msgsz, compat_long_t, msgtyp,
		       int, msgflg)
{
	return compat_ksys_msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
}
#endif

int msg_init_ns(struct ipc_namespace *ns)
{
	int ret;

	ns->msg_ctlmax = MSGMAX;
	ns->msg_ctlmnb = MSGMNB;
	ns->msg_ctlmni = MSGMNI;

	ret = percpu_counter_init(&ns->percpu_msg_bytes, 0, GFP_KERNEL);
	if (ret)
		goto fail_msg_bytes;
	ret = percpu_counter_init(&ns->percpu_msg_hdrs, 0, GFP_KERNEL);
	if (ret)
		goto fail_msg_hdrs;
	ipc_init_ids(&ns->ids[IPC_MSG_IDS]);
	return 0;

fail_msg_hdrs:
	percpu_counter_destroy(&ns->percpu_msg_bytes);
fail_msg_bytes:
	return ret;
}

#ifdef CONFIG_IPC_NS
void msg_exit_ns(struct ipc_namespace *ns)
{
	free_ipcs(ns, &msg_ids(ns), freeque);
	idr_destroy(&ns->ids[IPC_MSG_IDS].ipcs_idr);
	rhashtable_destroy(&ns->ids[IPC_MSG_IDS].key_ht);
	percpu_counter_destroy(&ns->percpu_msg_bytes);
	percpu_counter_destroy(&ns->percpu_msg_hdrs);
}
#endif

#ifdef CONFIG_PROC_FS
static int sysvipc_msg_proc_show(struct seq_file *s, void *it)
{
	struct pid_namespace *pid_ns = ipc_seq_pid_ns(s);
	struct user_namespace *user_ns = seq_user_ns(s);
	struct kern_ipc_perm *ipcp = it;
	struct msg_queue *msq = container_of(ipcp, struct msg_queue, q_perm);

	seq_printf(s,
		   "%10d %10d  %4o  %10lu %10lu %5u %5u %5u %5u %5u %5u %10llu %10llu %10llu\n",
		   msq->q_perm.key,
		   msq->q_perm.id,
		   msq->q_perm.mode,
		   msq->q_cbytes,
		   msq->q_qnum,
		   pid_nr_ns(msq->q_lspid, pid_ns),
		   pid_nr_ns(msq->q_lrpid, pid_ns),
		   from_kuid_munged(user_ns, msq->q_perm.uid),
		   from_kgid_munged(user_ns, msq->q_perm.gid),
		   from_kuid_munged(user_ns, msq->q_perm.cuid),
		   from_kgid_munged(user_ns, msq->q_perm.cgid),
		   msq->q_stime,
		   msq->q_rtime,
		   msq->q_ctime);

	return 0;
}
#endif

void __init msg_init(void)
{
	msg_init_ns(&init_ipc_ns);

	ipc_init_proc_interface("sysvipc/msg",
				"       key      msqid perms      cbytes       qnum lspid lrpid   uid   gid  cuid  cgid      stime      rtime      ctime\n",
				IPC_MSG_IDS, sysvipc_msg_proc_show);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
