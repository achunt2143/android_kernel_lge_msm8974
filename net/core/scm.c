<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* scm.c - Socket level control messages processing.
 *
 * Author:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *              Alignment and value checking mods by Craig Metz
<<<<<<< HEAD
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/signal.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <linux/sched/user.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/socket.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/net.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <linux/pid.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>

#include <asm/uaccess.h>
=======
#include <linux/pid_namespace.h>
#include <linux/pid.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>
#include <linux/errqueue.h>
#include <linux/io_uring.h>

#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/compat.h>
#include <net/scm.h>
<<<<<<< HEAD
=======
#include <net/cls_cgroup.h>
#include <net/af_unix.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/*
 *	Only allow a user to send credentials, that they could set with
 *	setu(g)id.
 */

static __inline__ int scm_check_creds(struct ucred *creds)
{
	const struct cred *cred = current_cred();
<<<<<<< HEAD

	if ((creds->pid == task_tgid_vnr(current) || capable(CAP_SYS_ADMIN)) &&
	    ((creds->uid == cred->uid   || creds->uid == cred->euid ||
	      creds->uid == cred->suid) || capable(CAP_SETUID)) &&
	    ((creds->gid == cred->gid   || creds->gid == cred->egid ||
	      creds->gid == cred->sgid) || capable(CAP_SETGID))) {
=======
	kuid_t uid = make_kuid(cred->user_ns, creds->uid);
	kgid_t gid = make_kgid(cred->user_ns, creds->gid);

	if (!uid_valid(uid) || !gid_valid(gid))
		return -EINVAL;

	if ((creds->pid == task_tgid_vnr(current) ||
	     ns_capable(task_active_pid_ns(current)->user_ns, CAP_SYS_ADMIN)) &&
	    ((uid_eq(uid, cred->uid)   || uid_eq(uid, cred->euid) ||
	      uid_eq(uid, cred->suid)) || ns_capable(cred->user_ns, CAP_SETUID)) &&
	    ((gid_eq(gid, cred->gid)   || gid_eq(gid, cred->egid) ||
	      gid_eq(gid, cred->sgid)) || ns_capable(cred->user_ns, CAP_SETGID))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       return 0;
	}
	return -EPERM;
}

static int scm_fp_copy(struct cmsghdr *cmsg, struct scm_fp_list **fplp)
{
	int *fdp = (int*)CMSG_DATA(cmsg);
	struct scm_fp_list *fpl = *fplp;
	struct file **fpp;
	int i, num;

<<<<<<< HEAD
	num = (cmsg->cmsg_len - CMSG_ALIGN(sizeof(struct cmsghdr)))/sizeof(int);
=======
	num = (cmsg->cmsg_len - sizeof(struct cmsghdr))/sizeof(int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (num <= 0)
		return 0;

	if (num > SCM_MAX_FD)
		return -EINVAL;

	if (!fpl)
	{
<<<<<<< HEAD
		fpl = kmalloc(sizeof(struct scm_fp_list), GFP_KERNEL);
=======
		fpl = kmalloc(sizeof(struct scm_fp_list), GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!fpl)
			return -ENOMEM;
		*fplp = fpl;
		fpl->count = 0;
<<<<<<< HEAD
=======
		fpl->count_unix = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fpl->max = SCM_MAX_FD;
		fpl->user = NULL;
	}
	fpp = &fpl->fp[fpl->count];

	if (fpl->count + num > fpl->max)
		return -EINVAL;

	/*
	 *	Verify the descriptors and increment the usage count.
	 */

	for (i=0; i< num; i++)
	{
		int fd = fdp[i];
		struct file *file;

		if (fd < 0 || !(file = fget_raw(fd)))
			return -EBADF;
<<<<<<< HEAD
=======
		/* don't allow io_uring files */
		if (io_is_uring_fops(file)) {
			fput(file);
			return -EINVAL;
		}
		if (unix_get_socket(file))
			fpl->count_unix++;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*fpp++ = file;
		fpl->count++;
	}

	if (!fpl->user)
		fpl->user = get_uid(current_user());

	return num;
}

void __scm_destroy(struct scm_cookie *scm)
{
	struct scm_fp_list *fpl = scm->fp;
	int i;

	if (fpl) {
		scm->fp = NULL;
<<<<<<< HEAD
		if (current->scm_work_list) {
			list_add_tail(&fpl->list, current->scm_work_list);
		} else {
			LIST_HEAD(work_list);

			current->scm_work_list = &work_list;

			list_add(&fpl->list, &work_list);
			while (!list_empty(&work_list)) {
				fpl = list_first_entry(&work_list, struct scm_fp_list, list);

				list_del(&fpl->list);
				for (i=fpl->count-1; i>=0; i--)
					fput(fpl->fp[i]);
				free_uid(fpl->user);
				kfree(fpl);
			}

			current->scm_work_list = NULL;
		}
=======
		for (i=fpl->count-1; i>=0; i--)
			fput(fpl->fp[i]);
		free_uid(fpl->user);
		kfree(fpl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
EXPORT_SYMBOL(__scm_destroy);

int __scm_send(struct socket *sock, struct msghdr *msg, struct scm_cookie *p)
{
<<<<<<< HEAD
	struct cmsghdr *cmsg;
	int err;

	for (cmsg = CMSG_FIRSTHDR(msg); cmsg; cmsg = CMSG_NXTHDR(msg, cmsg))
	{
=======
	const struct proto_ops *ops = READ_ONCE(sock->ops);
	struct cmsghdr *cmsg;
	int err;

	for_each_cmsghdr(cmsg, msg) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;

		/* Verify that cmsg_len is at least sizeof(struct cmsghdr) */
		/* The first check was omitted in <= 2.2.5. The reasoning was
		   that parser checks cmsg_len in any case, so that
		   additional check would be work duplication.
		   But if cmsg_level is not SOL_SOCKET, we do not check
		   for too short ancillary data object at all! Oops.
		   OK, let's add it...
		 */
		if (!CMSG_OK(msg, cmsg))
			goto error;

		if (cmsg->cmsg_level != SOL_SOCKET)
			continue;

		switch (cmsg->cmsg_type)
		{
		case SCM_RIGHTS:
<<<<<<< HEAD
			if (!sock->ops || sock->ops->family != PF_UNIX)
=======
			if (!ops || ops->family != PF_UNIX)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto error;
			err=scm_fp_copy(cmsg, &p->fp);
			if (err<0)
				goto error;
			break;
		case SCM_CREDENTIALS:
<<<<<<< HEAD
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct ucred)))
				goto error;
			memcpy(&p->creds, CMSG_DATA(cmsg), sizeof(struct ucred));
			err = scm_check_creds(&p->creds);
			if (err)
				goto error;

			if (!p->pid || pid_vnr(p->pid) != p->creds.pid) {
				struct pid *pid;
				err = -ESRCH;
				pid = find_get_pid(p->creds.pid);
=======
		{
			struct ucred creds;
			kuid_t uid;
			kgid_t gid;
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct ucred)))
				goto error;
			memcpy(&creds, CMSG_DATA(cmsg), sizeof(struct ucred));
			err = scm_check_creds(&creds);
			if (err)
				goto error;

			p->creds.pid = creds.pid;
			if (!p->pid || pid_vnr(p->pid) != creds.pid) {
				struct pid *pid;
				err = -ESRCH;
				pid = find_get_pid(creds.pid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (!pid)
					goto error;
				put_pid(p->pid);
				p->pid = pid;
			}

<<<<<<< HEAD
			if (!p->cred ||
			    (p->cred->euid != p->creds.uid) ||
			    (p->cred->egid != p->creds.gid)) {
				struct cred *cred;
				err = -ENOMEM;
				cred = prepare_creds();
				if (!cred)
					goto error;

				cred->uid = cred->euid = p->creds.uid;
				cred->gid = cred->egid = p->creds.gid;
				if (p->cred)
					put_cred(p->cred);
				p->cred = cred;
			}
			break;
=======
			err = -EINVAL;
			uid = make_kuid(current_user_ns(), creds.uid);
			gid = make_kgid(current_user_ns(), creds.gid);
			if (!uid_valid(uid) || !gid_valid(gid))
				goto error;

			p->creds.uid = uid;
			p->creds.gid = gid;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			goto error;
		}
	}

	if (p->fp && !p->fp->count)
	{
		kfree(p->fp);
		p->fp = NULL;
	}
	return 0;

error:
	scm_destroy(p);
	return err;
}
EXPORT_SYMBOL(__scm_send);

int put_cmsg(struct msghdr * msg, int level, int type, int len, void *data)
{
<<<<<<< HEAD
	struct cmsghdr __user *cm
		= (__force struct cmsghdr __user *)msg->msg_control;
	struct cmsghdr cmhdr;
	int cmlen = CMSG_LEN(len);
	int err;

	if (MSG_CMSG_COMPAT & msg->msg_flags)
		return put_cmsg_compat(msg, level, type, len, data);

	if (cm==NULL || msg->msg_controllen < sizeof(*cm)) {
=======
	int cmlen = CMSG_LEN(len);

	if (msg->msg_flags & MSG_CMSG_COMPAT)
		return put_cmsg_compat(msg, level, type, len, data);

	if (!msg->msg_control || msg->msg_controllen < sizeof(struct cmsghdr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		msg->msg_flags |= MSG_CTRUNC;
		return 0; /* XXX: return error? check spec. */
	}
	if (msg->msg_controllen < cmlen) {
		msg->msg_flags |= MSG_CTRUNC;
		cmlen = msg->msg_controllen;
	}
<<<<<<< HEAD
	cmhdr.cmsg_level = level;
	cmhdr.cmsg_type = type;
	cmhdr.cmsg_len = cmlen;

	err = -EFAULT;
	if (copy_to_user(cm, &cmhdr, sizeof cmhdr))
		goto out;
	if (copy_to_user(CMSG_DATA(cm), data, cmlen - sizeof(struct cmsghdr)))
		goto out;
	cmlen = CMSG_SPACE(len);
	if (msg->msg_controllen < cmlen)
		cmlen = msg->msg_controllen;
	msg->msg_control += cmlen;
	msg->msg_controllen -= cmlen;
	err = 0;
out:
	return err;
}
EXPORT_SYMBOL(put_cmsg);

void scm_detach_fds(struct msghdr *msg, struct scm_cookie *scm)
{
	struct cmsghdr __user *cm
		= (__force struct cmsghdr __user*)msg->msg_control;

	int fdmax = 0;
	int fdnum = scm->fp->count;
	struct file **fp = scm->fp->fp;
	int __user *cmfptr;
	int err = 0, i;

	if (MSG_CMSG_COMPAT & msg->msg_flags) {
=======

	if (msg->msg_control_is_user) {
		struct cmsghdr __user *cm = msg->msg_control_user;

		check_object_size(data, cmlen - sizeof(*cm), true);

		if (!user_write_access_begin(cm, cmlen))
			goto efault;

		unsafe_put_user(cmlen, &cm->cmsg_len, efault_end);
		unsafe_put_user(level, &cm->cmsg_level, efault_end);
		unsafe_put_user(type, &cm->cmsg_type, efault_end);
		unsafe_copy_to_user(CMSG_USER_DATA(cm), data,
				    cmlen - sizeof(*cm), efault_end);
		user_write_access_end();
	} else {
		struct cmsghdr *cm = msg->msg_control;

		cm->cmsg_level = level;
		cm->cmsg_type = type;
		cm->cmsg_len = cmlen;
		memcpy(CMSG_DATA(cm), data, cmlen - sizeof(*cm));
	}

	cmlen = min(CMSG_SPACE(len), msg->msg_controllen);
	if (msg->msg_control_is_user)
		msg->msg_control_user += cmlen;
	else
		msg->msg_control += cmlen;
	msg->msg_controllen -= cmlen;
	return 0;

efault_end:
	user_write_access_end();
efault:
	return -EFAULT;
}
EXPORT_SYMBOL(put_cmsg);

void put_cmsg_scm_timestamping64(struct msghdr *msg, struct scm_timestamping_internal *tss_internal)
{
	struct scm_timestamping64 tss;
	int i;

	for (i = 0; i < ARRAY_SIZE(tss.ts); i++) {
		tss.ts[i].tv_sec = tss_internal->ts[i].tv_sec;
		tss.ts[i].tv_nsec = tss_internal->ts[i].tv_nsec;
	}

	put_cmsg(msg, SOL_SOCKET, SO_TIMESTAMPING_NEW, sizeof(tss), &tss);
}
EXPORT_SYMBOL(put_cmsg_scm_timestamping64);

void put_cmsg_scm_timestamping(struct msghdr *msg, struct scm_timestamping_internal *tss_internal)
{
	struct scm_timestamping tss;
	int i;

	for (i = 0; i < ARRAY_SIZE(tss.ts); i++) {
		tss.ts[i].tv_sec = tss_internal->ts[i].tv_sec;
		tss.ts[i].tv_nsec = tss_internal->ts[i].tv_nsec;
	}

	put_cmsg(msg, SOL_SOCKET, SO_TIMESTAMPING_OLD, sizeof(tss), &tss);
}
EXPORT_SYMBOL(put_cmsg_scm_timestamping);

static int scm_max_fds(struct msghdr *msg)
{
	if (msg->msg_controllen <= sizeof(struct cmsghdr))
		return 0;
	return (msg->msg_controllen - sizeof(struct cmsghdr)) / sizeof(int);
}

void scm_detach_fds(struct msghdr *msg, struct scm_cookie *scm)
{
	struct cmsghdr __user *cm =
		(__force struct cmsghdr __user *)msg->msg_control_user;
	unsigned int o_flags = (msg->msg_flags & MSG_CMSG_CLOEXEC) ? O_CLOEXEC : 0;
	int fdmax = min_t(int, scm_max_fds(msg), scm->fp->count);
	int __user *cmsg_data = CMSG_USER_DATA(cm);
	int err = 0, i;

	/* no use for FD passing from kernel space callers */
	if (WARN_ON_ONCE(!msg->msg_control_is_user))
		return;

	if (msg->msg_flags & MSG_CMSG_COMPAT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		scm_detach_fds_compat(msg, scm);
		return;
	}

<<<<<<< HEAD
	if (msg->msg_controllen > sizeof(struct cmsghdr))
		fdmax = ((msg->msg_controllen - sizeof(struct cmsghdr))
			 / sizeof(int));

	if (fdnum < fdmax)
		fdmax = fdnum;

	for (i=0, cmfptr=(__force int __user *)CMSG_DATA(cm); i<fdmax;
	     i++, cmfptr++)
	{
		int new_fd;
		err = security_file_receive(fp[i]);
		if (err)
			break;
		err = get_unused_fd_flags(MSG_CMSG_CLOEXEC & msg->msg_flags
					  ? O_CLOEXEC : 0);
		if (err < 0)
			break;
		new_fd = err;
		err = put_user(new_fd, cmfptr);
		if (err) {
			put_unused_fd(new_fd);
			break;
		}
		/* Bump the usage count and install the file. */
		get_file(fp[i]);
		fd_install(new_fd, fp[i]);
	}

	if (i > 0)
	{
		int cmlen = CMSG_LEN(i*sizeof(int));
=======
	for (i = 0; i < fdmax; i++) {
		err = scm_recv_one_fd(scm->fp->fp[i], cmsg_data + i, o_flags);
		if (err < 0)
			break;
	}

	if (i > 0) {
		int cmlen = CMSG_LEN(i * sizeof(int));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = put_user(SOL_SOCKET, &cm->cmsg_level);
		if (!err)
			err = put_user(SCM_RIGHTS, &cm->cmsg_type);
		if (!err)
			err = put_user(cmlen, &cm->cmsg_len);
		if (!err) {
<<<<<<< HEAD
			cmlen = CMSG_SPACE(i*sizeof(int));
			msg->msg_control += cmlen;
			msg->msg_controllen -= cmlen;
		}
	}
	if (i < fdnum || (fdnum && fdmax <= 0))
		msg->msg_flags |= MSG_CTRUNC;

	/*
	 * All of the files that fit in the message have had their
	 * usage counts incremented, so we just free the list.
=======
			cmlen = CMSG_SPACE(i * sizeof(int));
			if (msg->msg_controllen < cmlen)
				cmlen = msg->msg_controllen;
			msg->msg_control_user += cmlen;
			msg->msg_controllen -= cmlen;
		}
	}

	if (i < scm->fp->count || (scm->fp->count && fdmax <= 0))
		msg->msg_flags |= MSG_CTRUNC;

	/*
	 * All of the files that fit in the message have had their usage counts
	 * incremented, so we just free the list.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	__scm_destroy(scm);
}
EXPORT_SYMBOL(scm_detach_fds);

struct scm_fp_list *scm_fp_dup(struct scm_fp_list *fpl)
{
	struct scm_fp_list *new_fpl;
	int i;

	if (!fpl)
		return NULL;

	new_fpl = kmemdup(fpl, offsetof(struct scm_fp_list, fp[fpl->count]),
<<<<<<< HEAD
			  GFP_KERNEL);
=======
			  GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (new_fpl) {
		for (i = 0; i < fpl->count; i++)
			get_file(fpl->fp[i]);
		new_fpl->max = new_fpl->count;
		new_fpl->user = get_uid(fpl->user);
	}
	return new_fpl;
}
EXPORT_SYMBOL(scm_fp_dup);
