/*
   BNEP implementation for Linux Bluetooth stack (BlueZ).
   Copyright (C) 2001-2002 Inventel Systemes
   Written 2001-2002 by
	David Libault  <david.libault@inventel.fr>

   Copyright (C) 2002 Maxim Krasnyansky <maxk@qualcomm.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

<<<<<<< HEAD
#include <linux/module.h>

#include <linux/types.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/ioctl.h>
#include <linux/file.h>
#include <linux/init.h>
#include <linux/compat.h>
#include <linux/gfp.h>
#include <linux/uaccess.h>
#include <net/sock.h>

#include <asm/system.h>

#include "bnep.h"

=======
#include <linux/compat.h>
#include <linux/export.h>
#include <linux/file.h>

#include "bnep.h"

static struct bt_sock_list bnep_sk_list = {
	.lock = __RW_LOCK_UNLOCKED(bnep_sk_list.lock)
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int bnep_sock_release(struct socket *sock)
{
	struct sock *sk = sock->sk;

	BT_DBG("sock %p sk %p", sock, sk);

	if (!sk)
		return 0;

<<<<<<< HEAD
=======
	bt_sock_unlink(&bnep_sk_list, sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_orphan(sk);
	sock_put(sk);
	return 0;
}

<<<<<<< HEAD
static int bnep_sock_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
=======
static int do_bnep_sock_ioctl(struct socket *sock, unsigned int cmd, void __user *argp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct bnep_connlist_req cl;
	struct bnep_connadd_req  ca;
	struct bnep_conndel_req  cd;
	struct bnep_conninfo ci;
	struct socket *nsock;
<<<<<<< HEAD
	void __user *argp = (void __user *)arg;
	int err;

	BT_DBG("cmd %x arg %lx", cmd, arg);
=======
	__u32 supp_feat = BIT(BNEP_SETUP_RESPONSE);
	int err;

	BT_DBG("cmd %x arg %p", cmd, argp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (cmd) {
	case BNEPCONNADD:
		if (!capable(CAP_NET_ADMIN))
<<<<<<< HEAD
			return -EACCES;
=======
			return -EPERM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (copy_from_user(&ca, argp, sizeof(ca)))
			return -EFAULT;

		nsock = sockfd_lookup(ca.sock, &err);
		if (!nsock)
			return err;

		if (nsock->sk->sk_state != BT_CONNECTED) {
			sockfd_put(nsock);
			return -EBADFD;
		}
		ca.device[sizeof(ca.device)-1] = 0;

		err = bnep_add_connection(&ca, nsock);
		if (!err) {
			if (copy_to_user(argp, &ca, sizeof(ca)))
				err = -EFAULT;
		} else
			sockfd_put(nsock);

		return err;

	case BNEPCONNDEL:
		if (!capable(CAP_NET_ADMIN))
<<<<<<< HEAD
			return -EACCES;
=======
			return -EPERM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (copy_from_user(&cd, argp, sizeof(cd)))
			return -EFAULT;

		return bnep_del_connection(&cd);

	case BNEPGETCONNLIST:
		if (copy_from_user(&cl, argp, sizeof(cl)))
			return -EFAULT;

		if (cl.cnum <= 0)
			return -EINVAL;

		err = bnep_get_connlist(&cl);
		if (!err && copy_to_user(argp, &cl, sizeof(cl)))
			return -EFAULT;

		return err;

	case BNEPGETCONNINFO:
		if (copy_from_user(&ci, argp, sizeof(ci)))
			return -EFAULT;

		err = bnep_get_conninfo(&ci);
		if (!err && copy_to_user(argp, &ci, sizeof(ci)))
			return -EFAULT;

		return err;

<<<<<<< HEAD
=======
	case BNEPGETSUPPFEAT:
		if (copy_to_user(argp, &supp_feat, sizeof(supp_feat)))
			return -EFAULT;

		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
static int bnep_sock_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	if (cmd == BNEPGETCONNLIST) {
		struct bnep_connlist_req cl;
		uint32_t uci;
		int err;

		if (get_user(cl.cnum, (uint32_t __user *) arg) ||
				get_user(uci, (u32 __user *) (arg + 4)))
=======
static int bnep_sock_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	return do_bnep_sock_ioctl(sock, cmd, (void __user *)arg);
}

#ifdef CONFIG_COMPAT
static int bnep_sock_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	void __user *argp = compat_ptr(arg);
	if (cmd == BNEPGETCONNLIST) {
		struct bnep_connlist_req cl;
		unsigned __user *p = argp;
		u32 uci;
		int err;

		if (get_user(cl.cnum, p) || get_user(uci, p + 1))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EFAULT;

		cl.ci = compat_ptr(uci);

		if (cl.cnum <= 0)
			return -EINVAL;

		err = bnep_get_connlist(&cl);

<<<<<<< HEAD
		if (!err && put_user(cl.cnum, (uint32_t __user *) arg))
=======
		if (!err && put_user(cl.cnum, p))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EFAULT;

		return err;
	}

<<<<<<< HEAD
	return bnep_sock_ioctl(sock, cmd, arg);
=======
	return do_bnep_sock_ioctl(sock, cmd, argp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

static const struct proto_ops bnep_sock_ops = {
	.family		= PF_BLUETOOTH,
	.owner		= THIS_MODULE,
	.release	= bnep_sock_release,
	.ioctl		= bnep_sock_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= bnep_sock_compat_ioctl,
#endif
	.bind		= sock_no_bind,
	.getname	= sock_no_getname,
	.sendmsg	= sock_no_sendmsg,
	.recvmsg	= sock_no_recvmsg,
<<<<<<< HEAD
	.poll		= sock_no_poll,
	.listen		= sock_no_listen,
	.shutdown	= sock_no_shutdown,
	.setsockopt	= sock_no_setsockopt,
	.getsockopt	= sock_no_getsockopt,
=======
	.listen		= sock_no_listen,
	.shutdown	= sock_no_shutdown,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.connect	= sock_no_connect,
	.socketpair	= sock_no_socketpair,
	.accept		= sock_no_accept,
	.mmap		= sock_no_mmap
};

static struct proto bnep_proto = {
	.name		= "BNEP",
	.owner		= THIS_MODULE,
	.obj_size	= sizeof(struct bt_sock)
};

static int bnep_sock_create(struct net *net, struct socket *sock, int protocol,
			    int kern)
{
	struct sock *sk;

	BT_DBG("sock %p", sock);

	if (sock->type != SOCK_RAW)
		return -ESOCKTNOSUPPORT;

<<<<<<< HEAD
	sk = sk_alloc(net, PF_BLUETOOTH, GFP_ATOMIC, &bnep_proto);
	if (!sk)
		return -ENOMEM;

	sock_init_data(sock, sk);

	sock->ops = &bnep_sock_ops;

	sock->state = SS_UNCONNECTED;

	sock_reset_flag(sk, SOCK_ZAPPED);

	sk->sk_protocol = protocol;
	sk->sk_state	= BT_OPEN;

=======
	sk = bt_sock_alloc(net, sock, &bnep_proto, protocol, GFP_ATOMIC, kern);
	if (!sk)
		return -ENOMEM;

	sock->ops = &bnep_sock_ops;
	sock->state = SS_UNCONNECTED;

	bt_sock_link(&bnep_sk_list, sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct net_proto_family bnep_sock_family_ops = {
	.family = PF_BLUETOOTH,
	.owner	= THIS_MODULE,
	.create = bnep_sock_create
};

int __init bnep_sock_init(void)
{
	int err;

	err = proto_register(&bnep_proto, 0);
	if (err < 0)
		return err;

	err = bt_sock_register(BTPROTO_BNEP, &bnep_sock_family_ops);
<<<<<<< HEAD
	if (err < 0)
		goto error;
=======
	if (err < 0) {
		BT_ERR("Can't register BNEP socket");
		goto error;
	}

	err = bt_procfs_init(&init_net, "bnep", &bnep_sk_list, NULL);
	if (err < 0) {
		BT_ERR("Failed to create BNEP proc file");
		bt_sock_unregister(BTPROTO_BNEP);
		goto error;
	}

	BT_INFO("BNEP socket layer initialized");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

error:
<<<<<<< HEAD
	BT_ERR("Can't register BNEP socket");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	proto_unregister(&bnep_proto);
	return err;
}

void __exit bnep_sock_cleanup(void)
{
<<<<<<< HEAD
	if (bt_sock_unregister(BTPROTO_BNEP) < 0)
		BT_ERR("Can't unregister BNEP socket");

=======
	bt_procfs_cleanup(&init_net, "bnep");
	bt_sock_unregister(BTPROTO_BNEP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	proto_unregister(&bnep_proto);
}
