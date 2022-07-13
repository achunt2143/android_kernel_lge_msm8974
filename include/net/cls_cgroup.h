<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cls_cgroup.h			Control Group Classifier
 *
 * Authors:	Thomas Graf <tgraf@suug.ch>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _NET_CLS_CGROUP_H
#define _NET_CLS_CGROUP_H

#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD

#ifdef CONFIG_CGROUPS
struct cgroup_cls_state
{
=======
#include <net/sock.h>
#include <net/inet_sock.h>

#ifdef CONFIG_CGROUP_NET_CLASSID
struct cgroup_cls_state {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cgroup_subsys_state css;
	u32 classid;
};

<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_CGROUP
static inline u32 task_cls_classid(struct task_struct *p)
{
	int classid;
=======
struct cgroup_cls_state *task_cls_state(struct task_struct *p);

static inline u32 task_cls_classid(struct task_struct *p)
{
	u32 classid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (in_interrupt())
		return 0;

	rcu_read_lock();
<<<<<<< HEAD
	classid = container_of(task_subsys_state(p, net_cls_subsys_id),
=======
	classid = container_of(task_css(p, net_cls_cgrp_id),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}
<<<<<<< HEAD
#else
extern int net_cls_subsys_id;

static inline u32 task_cls_classid(struct task_struct *p)
{
	int id;
	u32 classid = 0;

	if (in_interrupt())
		return 0;

	rcu_read_lock();
	id = rcu_dereference_index_check(net_cls_subsys_id,
					 rcu_read_lock_held());
	if (id >= 0)
		classid = container_of(task_subsys_state(p, id),
				       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}
#endif
#else
static inline u32 task_cls_classid(struct task_struct *p)
{
	return 0;
}
#endif
=======

static inline void sock_update_classid(struct sock_cgroup_data *skcd)
{
	u32 classid;

	classid = task_cls_classid(current);
	sock_cgroup_set_classid(skcd, classid);
}

static inline u32 __task_get_classid(struct task_struct *task)
{
	return task_cls_state(task)->classid;
}

static inline u32 task_get_classid(const struct sk_buff *skb)
{
	u32 classid = __task_get_classid(current);

	/* Due to the nature of the classifier it is required to ignore all
	 * packets originating from softirq context as accessing `current'
	 * would lead to false results.
	 *
	 * This test assumes that all callers of dev_queue_xmit() explicitly
	 * disable bh. Knowing this, it is possible to detect softirq based
	 * calls by looking at the number of nested bh disable calls because
	 * softirqs always disables bh.
	 */
	if (in_serving_softirq()) {
		struct sock *sk = skb_to_full_sk(skb);

		/* If there is an sock_cgroup_classid we'll use that. */
		if (!sk || !sk_fullsock(sk))
			return 0;

		classid = sock_cgroup_classid(&sk->sk_cgrp_data);
	}

	return classid;
}
#else /* !CONFIG_CGROUP_NET_CLASSID */
static inline void sock_update_classid(struct sock_cgroup_data *skcd)
{
}

static inline u32 task_get_classid(const struct sk_buff *skb)
{
	return 0;
}
#endif /* CONFIG_CGROUP_NET_CLASSID */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif  /* _NET_CLS_CGROUP_H */
