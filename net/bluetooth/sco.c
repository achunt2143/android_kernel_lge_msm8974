/*
   BlueZ - Bluetooth protocol stack for Linux
   Copyright (C) 2000-2001 Qualcomm Incorporated
<<<<<<< HEAD
   Copyright (c) 2011, The Linux Foundation. All rights reserved.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

   Written 2000,2001 by Maxim Krasnyansky <maxk@qualcomm.com>

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

/* Bluetooth SCO sockets. */

<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/module.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/device.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <net/sock.h>

#include <asm/system.h>
#include <linux/uaccess.h>
=======
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <net/bluetooth/sco.h>

static bool disable_esco;

static const struct proto_ops sco_sock_ops;

static struct bt_sock_list sco_sk_list = {
	.lock = __RW_LOCK_UNLOCKED(sco_sk_list.lock)
};

<<<<<<< HEAD
static void __sco_chan_add(struct sco_conn *conn, struct sock *sk, struct sock *parent);
static void sco_chan_del(struct sock *sk, int err);

static int  sco_conn_del(struct hci_conn *conn, int err, u8 is_process);
=======
/* ---- SCO connections ---- */
struct sco_conn {
	struct hci_conn	*hcon;

	spinlock_t	lock;
	struct sock	*sk;

	struct delayed_work	timeout_work;

	unsigned int    mtu;
};

#define sco_conn_lock(c)	spin_lock(&c->lock)
#define sco_conn_unlock(c)	spin_unlock(&c->lock)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void sco_sock_close(struct sock *sk);
static void sco_sock_kill(struct sock *sk);

<<<<<<< HEAD
/* ---- SCO timers ---- */
static void sco_sock_timeout(unsigned long arg)
{
	struct sock *sk = (struct sock *) arg;

	BT_DBG("sock %p state %d", sk, sk->sk_state);

	bh_lock_sock(sk);
	sk->sk_err = ETIMEDOUT;
	sk->sk_state_change(sk);
	bh_unlock_sock(sk);

	sco_sock_kill(sk);
=======
/* ----- SCO socket info ----- */
#define sco_pi(sk) ((struct sco_pinfo *) sk)

struct sco_pinfo {
	struct bt_sock	bt;
	bdaddr_t	src;
	bdaddr_t	dst;
	__u32		flags;
	__u16		setting;
	struct bt_codec codec;
	struct sco_conn	*conn;
};

/* ---- SCO timers ---- */
#define SCO_CONN_TIMEOUT	(HZ * 40)
#define SCO_DISCONN_TIMEOUT	(HZ * 2)

static void sco_sock_timeout(struct work_struct *work)
{
	struct sco_conn *conn = container_of(work, struct sco_conn,
					     timeout_work.work);
	struct sock *sk;

	sco_conn_lock(conn);
	if (!conn->hcon) {
		sco_conn_unlock(conn);
		return;
	}
	sk = conn->sk;
	if (sk)
		sock_hold(sk);
	sco_conn_unlock(conn);

	if (!sk)
		return;

	BT_DBG("sock %p state %d", sk, sk->sk_state);

	lock_sock(sk);
	sk->sk_err = ETIMEDOUT;
	sk->sk_state_change(sk);
	release_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_put(sk);
}

static void sco_sock_set_timer(struct sock *sk, long timeout)
{
<<<<<<< HEAD
	BT_DBG("sock %p state %d timeout %ld", sk, sk->sk_state, timeout);
	sk_reset_timer(sk, &sk->sk_timer, jiffies + timeout);
=======
	if (!sco_pi(sk)->conn)
		return;

	BT_DBG("sock %p state %d timeout %ld", sk, sk->sk_state, timeout);
	cancel_delayed_work(&sco_pi(sk)->conn->timeout_work);
	schedule_delayed_work(&sco_pi(sk)->conn->timeout_work, timeout);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void sco_sock_clear_timer(struct sock *sk)
{
<<<<<<< HEAD
	BT_DBG("sock %p state %d", sk, sk->sk_state);
	sk_stop_timer(sk, &sk->sk_timer);
}

/* ---- SCO connections ---- */
static struct sco_conn *sco_conn_add(struct hci_conn *hcon, __u8 status)
=======
	if (!sco_pi(sk)->conn)
		return;

	BT_DBG("sock %p state %d", sk, sk->sk_state);
	cancel_delayed_work(&sco_pi(sk)->conn->timeout_work);
}

/* ---- SCO connections ---- */
static struct sco_conn *sco_conn_add(struct hci_conn *hcon)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct hci_dev *hdev = hcon->hdev;
	struct sco_conn *conn = hcon->sco_data;

<<<<<<< HEAD
	if (conn || status)
		return conn;

	conn = kzalloc(sizeof(struct sco_conn), GFP_ATOMIC);
=======
	if (conn) {
		if (!conn->hcon)
			conn->hcon = hcon;
		return conn;
	}

	conn = kzalloc(sizeof(struct sco_conn), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!conn)
		return NULL;

	spin_lock_init(&conn->lock);
<<<<<<< HEAD
=======
	INIT_DELAYED_WORK(&conn->timeout_work, sco_sock_timeout);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hcon->sco_data = conn;
	conn->hcon = hcon;

<<<<<<< HEAD
	conn->src = &hdev->bdaddr;
	conn->dst = &hcon->dst;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hdev->sco_mtu > 0)
		conn->mtu = hdev->sco_mtu;
	else
		conn->mtu = 60;

	BT_DBG("hcon %p conn %p", hcon, conn);

	return conn;
}

<<<<<<< HEAD
static inline struct sock *sco_chan_get(struct sco_conn *conn)
{
	struct sock *sk = NULL;
	sco_conn_lock(conn);
	sk = conn->sk;
	sco_conn_unlock(conn);
	return sk;
}

static int sco_conn_del(struct hci_conn *hcon, int err, u8 is_process)
=======
/* Delete channel.
 * Must be called on the locked socket. */
static void sco_chan_del(struct sock *sk, int err)
{
	struct sco_conn *conn;

	conn = sco_pi(sk)->conn;

	BT_DBG("sk %p, conn %p, err %d", sk, conn, err);

	if (conn) {
		sco_conn_lock(conn);
		conn->sk = NULL;
		sco_pi(sk)->conn = NULL;
		sco_conn_unlock(conn);

		if (conn->hcon)
			hci_conn_drop(conn->hcon);
	}

	sk->sk_state = BT_CLOSED;
	sk->sk_err   = err;
	sk->sk_state_change(sk);

	sock_set_flag(sk, SOCK_ZAPPED);
}

static void sco_conn_del(struct hci_conn *hcon, int err)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sco_conn *conn = hcon->sco_data;
	struct sock *sk;

	if (!conn)
<<<<<<< HEAD
		return 0;
=======
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BT_DBG("hcon %p conn %p, err %d", hcon, conn, err);

	/* Kill socket */
<<<<<<< HEAD
	sk = sco_chan_get(conn);
	if (sk) {
		if (is_process)
			lock_sock(sk);
		else
			bh_lock_sock(sk);
		sco_sock_clear_timer(sk);
		sco_chan_del(sk, err);
		if (is_process)
			release_sock(sk);
		else
			bh_unlock_sock(sk);
		sco_sock_kill(sk);
	}

	hcon->sco_data = NULL;
	kfree(conn);
	return 0;
}

static inline int sco_chan_add(struct sco_conn *conn, struct sock *sk, struct sock *parent)
=======
	sco_conn_lock(conn);
	sk = conn->sk;
	if (sk)
		sock_hold(sk);
	sco_conn_unlock(conn);

	if (sk) {
		lock_sock(sk);
		sco_sock_clear_timer(sk);
		sco_chan_del(sk, err);
		release_sock(sk);
		sock_put(sk);
	}

	/* Ensure no more work items will run before freeing conn. */
	cancel_delayed_work_sync(&conn->timeout_work);

	hcon->sco_data = NULL;
	kfree(conn);
}

static void __sco_chan_add(struct sco_conn *conn, struct sock *sk,
			   struct sock *parent)
{
	BT_DBG("conn %p", conn);

	sco_pi(sk)->conn = conn;
	conn->sk = sk;

	if (parent)
		bt_accept_enqueue(parent, sk, true);
}

static int sco_chan_add(struct sco_conn *conn, struct sock *sk,
			struct sock *parent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;

	sco_conn_lock(conn);
	if (conn->sk)
		err = -EBUSY;
	else
		__sco_chan_add(conn, sk, parent);

	sco_conn_unlock(conn);
	return err;
}

<<<<<<< HEAD
static int sco_connect(struct sock *sk, __s8 is_wbs)
{
	bdaddr_t *src = &bt_sk(sk)->src;
	bdaddr_t *dst = &bt_sk(sk)->dst;
	__u16 pkt_type = sco_pi(sk)->pkt_type;
=======
static int sco_connect(struct sock *sk)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sco_conn *conn;
	struct hci_conn *hcon;
	struct hci_dev  *hdev;
	int err, type;

<<<<<<< HEAD
	BT_DBG("%s -> %s", batostr(src), batostr(dst));

	hdev = hci_get_route(dst, src);
	if (!hdev)
		return -EHOSTUNREACH;

	hci_dev_lock_bh(hdev);

	hdev->is_wbs = is_wbs;

	if (lmp_esco_capable(hdev) && !disable_esco) {
		type = ESCO_LINK;
	} else if (is_wbs) {
		return -ENAVAIL;
	} else {
		type = SCO_LINK;
		pkt_type &= SCO_ESCO_MASK;
	}

	BT_DBG("type: %d, pkt_type: 0x%x", type, pkt_type);

	hcon = hci_connect(hdev, type, pkt_type, dst,
					BT_SECURITY_LOW, HCI_AT_NO_BONDING);
	if (IS_ERR(hcon)) {
		err = PTR_ERR(hcon);
		goto done;
	}

	if (is_wbs && (hcon->type != ESCO_LINK)) {
		BT_ERR("WBS [ hcon->type: 0x%x, hcon->pkt_type: 0x%x ]",
				hcon->type, hcon->pkt_type);
		err = -EREMOTEIO;
		goto done;
	}

	conn = sco_conn_add(hcon, 0);
	if (!conn) {
		hci_conn_put(hcon);
		err = -ENOMEM;
		goto done;
	}

	/* Update source addr of the socket */
	bacpy(src, conn->src);

	err = sco_chan_add(conn, sk, NULL);
	if (err)
		goto done;
=======
	BT_DBG("%pMR -> %pMR", &sco_pi(sk)->src, &sco_pi(sk)->dst);

	hdev = hci_get_route(&sco_pi(sk)->dst, &sco_pi(sk)->src, BDADDR_BREDR);
	if (!hdev)
		return -EHOSTUNREACH;

	hci_dev_lock(hdev);

	if (lmp_esco_capable(hdev) && !disable_esco)
		type = ESCO_LINK;
	else
		type = SCO_LINK;

	if (sco_pi(sk)->setting == BT_VOICE_TRANSPARENT &&
	    (!lmp_transp_capable(hdev) || !lmp_esco_capable(hdev))) {
		err = -EOPNOTSUPP;
		goto unlock;
	}

	hcon = hci_connect_sco(hdev, type, &sco_pi(sk)->dst,
			       sco_pi(sk)->setting, &sco_pi(sk)->codec,
			       sk->sk_sndtimeo);
	if (IS_ERR(hcon)) {
		err = PTR_ERR(hcon);
		goto unlock;
	}

	conn = sco_conn_add(hcon);
	if (!conn) {
		hci_conn_drop(hcon);
		err = -ENOMEM;
		goto unlock;
	}

	lock_sock(sk);

	err = sco_chan_add(conn, sk, NULL);
	if (err) {
		release_sock(sk);
		goto unlock;
	}

	/* Update source addr of the socket */
	bacpy(&sco_pi(sk)->src, &hcon->src);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hcon->state == BT_CONNECTED) {
		sco_sock_clear_timer(sk);
		sk->sk_state = BT_CONNECTED;
	} else {
		sk->sk_state = BT_CONNECT;
		sco_sock_set_timer(sk, sk->sk_sndtimeo);
	}

<<<<<<< HEAD
done:
	hci_dev_unlock_bh(hdev);
=======
	release_sock(sk);

unlock:
	hci_dev_unlock(hdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hci_dev_put(hdev);
	return err;
}

<<<<<<< HEAD
static inline int sco_send_frame(struct sock *sk, struct msghdr *msg, int len)
{
	struct sco_conn *conn = sco_pi(sk)->conn;
	struct sk_buff *skb;
	int err, count;
=======
static int sco_send_frame(struct sock *sk, struct sk_buff *skb)
{
	struct sco_conn *conn = sco_pi(sk)->conn;
	int len = skb->len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check outgoing MTU */
	if (len > conn->mtu)
		return -EINVAL;

	BT_DBG("sk %p len %d", sk, len);

<<<<<<< HEAD
	count = min_t(unsigned int, conn->mtu, len);
	skb = bt_skb_send_alloc(sk, count,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return err;

	if (memcpy_fromiovec(skb_put(skb, count), msg->msg_iov, count)) {
		kfree_skb(skb);
		return -EFAULT;
	}

	hci_send_sco(conn->hcon, skb);

	return count;
}

static inline void sco_recv_frame(struct sco_conn *conn, struct sk_buff *skb)
{
	struct sock *sk = sco_chan_get(conn);
=======
	hci_send_sco(conn->hcon, skb);

	return len;
}

static void sco_recv_frame(struct sco_conn *conn, struct sk_buff *skb)
{
	struct sock *sk;

	sco_conn_lock(conn);
	sk = conn->sk;
	sco_conn_unlock(conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!sk)
		goto drop;

<<<<<<< HEAD
	BT_DBG("sk %p len %d", sk, skb->len);
=======
	BT_DBG("sk %p len %u", sk, skb->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sk->sk_state != BT_CONNECTED)
		goto drop;

	if (!sock_queue_rcv_skb(sk, skb))
		return;

drop:
	kfree_skb(skb);
}

/* -------- Socket interface ---------- */
<<<<<<< HEAD
static struct sock *__sco_get_sock_by_addr(bdaddr_t *ba)
{
	struct sock *sk;
	struct hlist_node *node;

	sk_for_each(sk, node, &sco_sk_list.head)
		if (!bacmp(&bt_sk(sk)->src, ba))
			goto found;
	sk = NULL;
found:
	return sk;
=======
static struct sock *__sco_get_sock_listen_by_addr(bdaddr_t *ba)
{
	struct sock *sk;

	sk_for_each(sk, &sco_sk_list.head) {
		if (sk->sk_state != BT_LISTEN)
			continue;

		if (!bacmp(&sco_pi(sk)->src, ba))
			return sk;
	}

	return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Find socket listening on source bdaddr.
 * Returns closest match.
 */
static struct sock *sco_get_sock_listen(bdaddr_t *src)
{
	struct sock *sk = NULL, *sk1 = NULL;
<<<<<<< HEAD
	struct hlist_node *node;

	read_lock(&sco_sk_list.lock);

	sk_for_each(sk, node, &sco_sk_list.head) {
=======

	read_lock(&sco_sk_list.lock);

	sk_for_each(sk, &sco_sk_list.head) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (sk->sk_state != BT_LISTEN)
			continue;

		/* Exact match. */
<<<<<<< HEAD
		if (!bacmp(&bt_sk(sk)->src, src))
			break;

		/* Closest match */
		if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
=======
		if (!bacmp(&sco_pi(sk)->src, src))
			break;

		/* Closest match */
		if (!bacmp(&sco_pi(sk)->src, BDADDR_ANY))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			sk1 = sk;
	}

	read_unlock(&sco_sk_list.lock);

<<<<<<< HEAD
	return node ? sk : sk1;
=======
	return sk ? sk : sk1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void sco_sock_destruct(struct sock *sk)
{
	BT_DBG("sk %p", sk);

	skb_queue_purge(&sk->sk_receive_queue);
	skb_queue_purge(&sk->sk_write_queue);
}

static void sco_sock_cleanup_listen(struct sock *parent)
{
	struct sock *sk;

	BT_DBG("parent %p", parent);

	/* Close not yet accepted channels */
	while ((sk = bt_accept_dequeue(parent, NULL))) {
		sco_sock_close(sk);
		sco_sock_kill(sk);
	}

	parent->sk_state  = BT_CLOSED;
	sock_set_flag(parent, SOCK_ZAPPED);
}

/* Kill socket (only if zapped and orphan)
 * Must be called on unlocked socket.
 */
static void sco_sock_kill(struct sock *sk)
{
	if (!sock_flag(sk, SOCK_ZAPPED) || sk->sk_socket)
		return;

	BT_DBG("sk %p state %d", sk, sk->sk_state);

	/* Kill poor orphan */
	bt_sock_unlink(&sco_sk_list, sk);
	sock_set_flag(sk, SOCK_DEAD);
	sock_put(sk);
}

static void __sco_sock_close(struct sock *sk)
{
	BT_DBG("sk %p state %d socket %p", sk, sk->sk_state, sk->sk_socket);

	switch (sk->sk_state) {
	case BT_LISTEN:
		sco_sock_cleanup_listen(sk);
		break;

	case BT_CONNECTED:
	case BT_CONFIG:
<<<<<<< HEAD
		if (sco_pi(sk)->conn) {
			sk->sk_state = BT_DISCONN;
			sco_sock_set_timer(sk, SCO_DISCONN_TIMEOUT);
			if (sco_pi(sk)->conn->hcon != NULL) {
				hci_conn_put(sco_pi(sk)->conn->hcon);
				sco_pi(sk)->conn->hcon = NULL;
			}
=======
		if (sco_pi(sk)->conn->hcon) {
			sk->sk_state = BT_DISCONN;
			sco_sock_set_timer(sk, SCO_DISCONN_TIMEOUT);
			sco_conn_lock(sco_pi(sk)->conn);
			hci_conn_drop(sco_pi(sk)->conn->hcon);
			sco_pi(sk)->conn->hcon = NULL;
			sco_conn_unlock(sco_pi(sk)->conn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else
			sco_chan_del(sk, ECONNRESET);
		break;

<<<<<<< HEAD
=======
	case BT_CONNECT2:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case BT_CONNECT:
	case BT_DISCONN:
		sco_chan_del(sk, ECONNRESET);
		break;

	default:
		sock_set_flag(sk, SOCK_ZAPPED);
		break;
	}
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Must be called on unlocked socket. */
static void sco_sock_close(struct sock *sk)
{
<<<<<<< HEAD
	sco_sock_clear_timer(sk);
	lock_sock(sk);
	__sco_sock_close(sk);
	release_sock(sk);
	sco_sock_kill(sk);
=======
	lock_sock(sk);
	sco_sock_clear_timer(sk);
	__sco_sock_close(sk);
	release_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void sco_sock_init(struct sock *sk, struct sock *parent)
{
	BT_DBG("sk %p", sk);

<<<<<<< HEAD
	if (parent)
		sk->sk_type = parent->sk_type;
=======
	if (parent) {
		sk->sk_type = parent->sk_type;
		bt_sk(sk)->flags = bt_sk(parent)->flags;
		security_sk_clone(parent, sk);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct proto sco_proto = {
	.name		= "SCO",
	.owner		= THIS_MODULE,
	.obj_size	= sizeof(struct sco_pinfo)
};

<<<<<<< HEAD
static struct sock *sco_sock_alloc(struct net *net, struct socket *sock, int proto, gfp_t prio)
{
	struct sock *sk;

	sk = sk_alloc(net, PF_BLUETOOTH, prio, &sco_proto);
	if (!sk)
		return NULL;

	sock_init_data(sock, sk);
	INIT_LIST_HEAD(&bt_sk(sk)->accept_q);

	sk->sk_destruct = sco_sock_destruct;
	sk->sk_sndtimeo = SCO_CONN_TIMEOUT;

	sock_reset_flag(sk, SOCK_ZAPPED);

	sk->sk_protocol = proto;
	sk->sk_state    = BT_OPEN;

	setup_timer(&sk->sk_timer, sco_sock_timeout, (unsigned long)sk);
=======
static struct sock *sco_sock_alloc(struct net *net, struct socket *sock,
				   int proto, gfp_t prio, int kern)
{
	struct sock *sk;

	sk = bt_sock_alloc(net, sock, &sco_proto, proto, prio, kern);
	if (!sk)
		return NULL;

	sk->sk_destruct = sco_sock_destruct;
	sk->sk_sndtimeo = SCO_CONN_TIMEOUT;

	sco_pi(sk)->setting = BT_VOICE_CVSD_16BIT;
	sco_pi(sk)->codec.id = BT_CODEC_CVSD;
	sco_pi(sk)->codec.cid = 0xffff;
	sco_pi(sk)->codec.vid = 0xffff;
	sco_pi(sk)->codec.data_path = 0x00;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bt_sock_link(&sco_sk_list, sk);
	return sk;
}

static int sco_sock_create(struct net *net, struct socket *sock, int protocol,
			   int kern)
{
	struct sock *sk;

	BT_DBG("sock %p", sock);

	sock->state = SS_UNCONNECTED;

	if (sock->type != SOCK_SEQPACKET)
		return -ESOCKTNOSUPPORT;

	sock->ops = &sco_sock_ops;

<<<<<<< HEAD
	sk = sco_sock_alloc(net, sock, protocol, GFP_ATOMIC);
=======
	sk = sco_sock_alloc(net, sock, protocol, GFP_ATOMIC, kern);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sk)
		return -ENOMEM;

	sco_sock_init(sk, NULL);
	return 0;
}

<<<<<<< HEAD
static int sco_sock_bind(struct socket *sock, struct sockaddr *addr, int alen)
{
	struct sockaddr_sco sa;
	struct sock *sk = sock->sk;
	bdaddr_t *src = &sa.sco_bdaddr;
	int len, err = 0;

	BT_DBG("sk %p %s", sk, batostr(&sa.sco_bdaddr));

	if (!addr || addr->sa_family != AF_BLUETOOTH)
		return -EINVAL;

	if (alen < sizeof(struct sockaddr_sco))
		return -EINVAL;

	memset(&sa, 0, sizeof(sa));
	len = min_t(unsigned int, sizeof(sa), alen);
	memcpy(&sa, addr, len);
=======
static int sco_sock_bind(struct socket *sock, struct sockaddr *addr,
			 int addr_len)
{
	struct sockaddr_sco *sa = (struct sockaddr_sco *) addr;
	struct sock *sk = sock->sk;
	int err = 0;

	if (!addr || addr_len < sizeof(struct sockaddr_sco) ||
	    addr->sa_family != AF_BLUETOOTH)
		return -EINVAL;

	BT_DBG("sk %p %pMR", sk, &sa->sco_bdaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	lock_sock(sk);

	if (sk->sk_state != BT_OPEN) {
		err = -EBADFD;
		goto done;
	}

<<<<<<< HEAD
	write_lock_bh(&sco_sk_list.lock);

	if (bacmp(src, BDADDR_ANY) && __sco_get_sock_by_addr(src)) {
		err = -EADDRINUSE;
	} else {
		/* Save source address */
		bacpy(&bt_sk(sk)->src, &sa.sco_bdaddr);
		sco_pi(sk)->pkt_type = sa.sco_pkt_type;
		sk->sk_state = BT_BOUND;
	}

	write_unlock_bh(&sco_sk_list.lock);
=======
	if (sk->sk_type != SOCK_SEQPACKET) {
		err = -EINVAL;
		goto done;
	}

	bacpy(&sco_pi(sk)->src, &sa->sco_bdaddr);

	sk->sk_state = BT_BOUND;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

done:
	release_sock(sk);
	return err;
}

static int sco_sock_connect(struct socket *sock, struct sockaddr *addr, int alen, int flags)
{
<<<<<<< HEAD
	struct sock *sk = sock->sk;
	struct sockaddr_sco sa;
	int len, err = 0;

	BT_DBG("sk %p", sk);

	if (!addr || addr->sa_family != AF_BLUETOOTH)
		return -EINVAL;

	memset(&sa, 0, sizeof(sa));
	len = min_t(unsigned int, sizeof(sa), alen);
	memcpy(&sa, addr, len);

	lock_sock(sk);

	if (sk->sk_type != SOCK_SEQPACKET) {
		err = -EINVAL;
		goto done;
	}

	if (sk->sk_state != BT_OPEN && sk->sk_state != BT_BOUND) {
		err = -EBADFD;
		goto done;
	}

	/* Set destination address and psm */
	bacpy(&bt_sk(sk)->dst, &sa.sco_bdaddr);
	sco_pi(sk)->pkt_type = sa.sco_pkt_type;

	err = sco_connect(sk, sa.is_wbs);
	if (err)
		goto done;

	err = bt_sock_wait_state(sk, BT_CONNECTED,
			sock_sndtimeo(sk, flags & O_NONBLOCK));

done:
=======
	struct sockaddr_sco *sa = (struct sockaddr_sco *) addr;
	struct sock *sk = sock->sk;
	int err;

	BT_DBG("sk %p", sk);

	if (alen < sizeof(struct sockaddr_sco) ||
	    addr->sa_family != AF_BLUETOOTH)
		return -EINVAL;

	if (sk->sk_state != BT_OPEN && sk->sk_state != BT_BOUND)
		return -EBADFD;

	if (sk->sk_type != SOCK_SEQPACKET)
		err = -EINVAL;

	lock_sock(sk);
	/* Set destination address and psm */
	bacpy(&sco_pi(sk)->dst, &sa->sco_bdaddr);
	release_sock(sk);

	err = sco_connect(sk);
	if (err)
		return err;

	lock_sock(sk);

	err = bt_sock_wait_state(sk, BT_CONNECTED,
				 sock_sndtimeo(sk, flags & O_NONBLOCK));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	release_sock(sk);
	return err;
}

static int sco_sock_listen(struct socket *sock, int backlog)
{
	struct sock *sk = sock->sk;
<<<<<<< HEAD
=======
	bdaddr_t *src = &sco_pi(sk)->src;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	BT_DBG("sk %p backlog %d", sk, backlog);

	lock_sock(sk);

<<<<<<< HEAD
	if (sk->sk_state != BT_BOUND || sock->type != SOCK_SEQPACKET) {
=======
	if (sk->sk_state != BT_BOUND) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EBADFD;
		goto done;
	}

<<<<<<< HEAD
	sk->sk_max_ack_backlog = backlog;
	sk->sk_ack_backlog = 0;
	sk->sk_state = BT_LISTEN;

=======
	if (sk->sk_type != SOCK_SEQPACKET) {
		err = -EINVAL;
		goto done;
	}

	write_lock(&sco_sk_list.lock);

	if (__sco_get_sock_listen_by_addr(src)) {
		err = -EADDRINUSE;
		goto unlock;
	}

	sk->sk_max_ack_backlog = backlog;
	sk->sk_ack_backlog = 0;

	sk->sk_state = BT_LISTEN;

unlock:
	write_unlock(&sco_sk_list.lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
done:
	release_sock(sk);
	return err;
}

<<<<<<< HEAD
static int sco_sock_accept(struct socket *sock, struct socket *newsock, int flags)
{
	DECLARE_WAITQUEUE(wait, current);
=======
static int sco_sock_accept(struct socket *sock, struct socket *newsock,
			   int flags, bool kern)
{
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sock *sk = sock->sk, *ch;
	long timeo;
	int err = 0;

	lock_sock(sk);

<<<<<<< HEAD
	if (sk->sk_state != BT_LISTEN) {
		err = -EBADFD;
		goto done;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	timeo = sock_rcvtimeo(sk, flags & O_NONBLOCK);

	BT_DBG("sk %p timeo %ld", sk, timeo);

	/* Wait for an incoming connection. (wake-one). */
	add_wait_queue_exclusive(sk_sleep(sk), &wait);
<<<<<<< HEAD
	while (!(ch = bt_accept_dequeue(sk, newsock))) {
		set_current_state(TASK_INTERRUPTIBLE);
		if (!timeo) {
			err = -EAGAIN;
			break;
		}

		release_sock(sk);
		timeo = schedule_timeout(timeo);
		lock_sock(sk);

		if (sk->sk_state != BT_LISTEN) {
			err = -EBADFD;
=======
	while (1) {
		if (sk->sk_state != BT_LISTEN) {
			err = -EBADFD;
			break;
		}

		ch = bt_accept_dequeue(sk, newsock);
		if (ch)
			break;

		if (!timeo) {
			err = -EAGAIN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}

		if (signal_pending(current)) {
			err = sock_intr_errno(timeo);
			break;
		}
<<<<<<< HEAD
	}
	set_current_state(TASK_RUNNING);
=======

		release_sock(sk);

		timeo = wait_woken(&wait, TASK_INTERRUPTIBLE, timeo);
		lock_sock(sk);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	remove_wait_queue(sk_sleep(sk), &wait);

	if (err)
		goto done;

	newsock->state = SS_CONNECTED;

	BT_DBG("new socket %p", ch);

done:
	release_sock(sk);
	return err;
}

<<<<<<< HEAD
static int sco_sock_getname(struct socket *sock, struct sockaddr *addr, int *len, int peer)
=======
static int sco_sock_getname(struct socket *sock, struct sockaddr *addr,
			    int peer)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sockaddr_sco *sa = (struct sockaddr_sco *) addr;
	struct sock *sk = sock->sk;

	BT_DBG("sock %p, sk %p", sock, sk);

	addr->sa_family = AF_BLUETOOTH;
<<<<<<< HEAD
	*len = sizeof(struct sockaddr_sco);

	if (peer)
		bacpy(&sa->sco_bdaddr, &bt_sk(sk)->dst);
	else
		bacpy(&sa->sco_bdaddr, &bt_sk(sk)->src);
	sa->sco_pkt_type = sco_pi(sk)->pkt_type;

	return 0;
}

static int sco_sock_sendmsg(struct kiocb *iocb, struct socket *sock,
			    struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
=======

	if (peer)
		bacpy(&sa->sco_bdaddr, &sco_pi(sk)->dst);
	else
		bacpy(&sa->sco_bdaddr, &sco_pi(sk)->src);

	return sizeof(struct sockaddr_sco);
}

static int sco_sock_sendmsg(struct socket *sock, struct msghdr *msg,
			    size_t len)
{
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	BT_DBG("sock %p, sk %p", sock, sk);

	err = sock_error(sk);
	if (err)
		return err;

	if (msg->msg_flags & MSG_OOB)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	lock_sock(sk);

	if (sk->sk_state == BT_CONNECTED)
		err = sco_send_frame(sk, msg, len);
=======
	skb = bt_skb_sendmsg(sk, msg, len, len, 0, 0);
	if (IS_ERR(skb))
		return PTR_ERR(skb);

	lock_sock(sk);

	if (sk->sk_state == BT_CONNECTED)
		err = sco_send_frame(sk, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		err = -ENOTCONN;

	release_sock(sk);
<<<<<<< HEAD
	return err;
}

static int sco_sock_setsockopt(struct socket *sock, int level, int optname, char __user *optval, unsigned int optlen)
{
	struct sock *sk = sock->sk;
	int err = 0;
=======

	if (err < 0)
		kfree_skb(skb);
	return err;
}

static void sco_conn_defer_accept(struct hci_conn *conn, u16 setting)
{
	struct hci_dev *hdev = conn->hdev;

	BT_DBG("conn %p", conn);

	conn->state = BT_CONFIG;

	if (!lmp_esco_capable(hdev)) {
		struct hci_cp_accept_conn_req cp;

		bacpy(&cp.bdaddr, &conn->dst);
		cp.role = 0x00; /* Ignored */

		hci_send_cmd(hdev, HCI_OP_ACCEPT_CONN_REQ, sizeof(cp), &cp);
	} else {
		struct hci_cp_accept_sync_conn_req cp;

		bacpy(&cp.bdaddr, &conn->dst);
		cp.pkt_type = cpu_to_le16(conn->pkt_type);

		cp.tx_bandwidth   = cpu_to_le32(0x00001f40);
		cp.rx_bandwidth   = cpu_to_le32(0x00001f40);
		cp.content_format = cpu_to_le16(setting);

		switch (setting & SCO_AIRMODE_MASK) {
		case SCO_AIRMODE_TRANSP:
			if (conn->pkt_type & ESCO_2EV3)
				cp.max_latency = cpu_to_le16(0x0008);
			else
				cp.max_latency = cpu_to_le16(0x000D);
			cp.retrans_effort = 0x02;
			break;
		case SCO_AIRMODE_CVSD:
			cp.max_latency = cpu_to_le16(0xffff);
			cp.retrans_effort = 0xff;
			break;
		default:
			/* use CVSD settings as fallback */
			cp.max_latency = cpu_to_le16(0xffff);
			cp.retrans_effort = 0xff;
			break;
		}

		hci_send_cmd(hdev, HCI_OP_ACCEPT_SYNC_CONN_REQ,
			     sizeof(cp), &cp);
	}
}

static int sco_sock_recvmsg(struct socket *sock, struct msghdr *msg,
			    size_t len, int flags)
{
	struct sock *sk = sock->sk;
	struct sco_pinfo *pi = sco_pi(sk);

	lock_sock(sk);

	if (sk->sk_state == BT_CONNECT2 &&
	    test_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags)) {
		sco_conn_defer_accept(pi->conn->hcon, pi->setting);
		sk->sk_state = BT_CONFIG;

		release_sock(sk);
		return 0;
	}

	release_sock(sk);

	return bt_sock_recvmsg(sock, msg, len, flags);
}

static int sco_sock_setsockopt(struct socket *sock, int level, int optname,
			       sockptr_t optval, unsigned int optlen)
{
	struct sock *sk = sock->sk;
	int err = 0;
	struct bt_voice voice;
	u32 opt;
	struct bt_codecs *codecs;
	struct hci_dev *hdev;
	__u8 buffer[255];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BT_DBG("sk %p", sk);

	lock_sock(sk);

	switch (optname) {
<<<<<<< HEAD
=======

	case BT_DEFER_SETUP:
		if (sk->sk_state != BT_BOUND && sk->sk_state != BT_LISTEN) {
			err = -EINVAL;
			break;
		}

		err = bt_copy_from_sockptr(&opt, sizeof(opt), optval, optlen);
		if (err)
			break;

		if (opt)
			set_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags);
		else
			clear_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags);
		break;

	case BT_VOICE:
		if (sk->sk_state != BT_OPEN && sk->sk_state != BT_BOUND &&
		    sk->sk_state != BT_CONNECT2) {
			err = -EINVAL;
			break;
		}

		voice.setting = sco_pi(sk)->setting;

		err = bt_copy_from_sockptr(&voice, sizeof(voice), optval,
					   optlen);
		if (err)
			break;

		/* Explicitly check for these values */
		if (voice.setting != BT_VOICE_TRANSPARENT &&
		    voice.setting != BT_VOICE_CVSD_16BIT) {
			err = -EINVAL;
			break;
		}

		sco_pi(sk)->setting = voice.setting;
		hdev = hci_get_route(&sco_pi(sk)->dst, &sco_pi(sk)->src,
				     BDADDR_BREDR);
		if (!hdev) {
			err = -EBADFD;
			break;
		}
		if (enhanced_sync_conn_capable(hdev) &&
		    voice.setting == BT_VOICE_TRANSPARENT)
			sco_pi(sk)->codec.id = BT_CODEC_TRANSPARENT;
		hci_dev_put(hdev);
		break;

	case BT_PKT_STATUS:
		err = bt_copy_from_sockptr(&opt, sizeof(opt), optval, optlen);
		if (err)
			break;

		if (opt)
			set_bit(BT_SK_PKT_STATUS, &bt_sk(sk)->flags);
		else
			clear_bit(BT_SK_PKT_STATUS, &bt_sk(sk)->flags);
		break;

	case BT_CODEC:
		if (sk->sk_state != BT_OPEN && sk->sk_state != BT_BOUND &&
		    sk->sk_state != BT_CONNECT2) {
			err = -EINVAL;
			break;
		}

		hdev = hci_get_route(&sco_pi(sk)->dst, &sco_pi(sk)->src,
				     BDADDR_BREDR);
		if (!hdev) {
			err = -EBADFD;
			break;
		}

		if (!hci_dev_test_flag(hdev, HCI_OFFLOAD_CODECS_ENABLED)) {
			hci_dev_put(hdev);
			err = -EOPNOTSUPP;
			break;
		}

		if (!hdev->get_data_path_id) {
			hci_dev_put(hdev);
			err = -EOPNOTSUPP;
			break;
		}

		if (optlen < sizeof(struct bt_codecs) ||
		    optlen > sizeof(buffer)) {
			hci_dev_put(hdev);
			err = -EINVAL;
			break;
		}

		err = bt_copy_from_sockptr(buffer, optlen, optval, optlen);
		if (err) {
			hci_dev_put(hdev);
			break;
		}

		codecs = (void *)buffer;

		if (codecs->num_codecs > 1) {
			hci_dev_put(hdev);
			err = -EINVAL;
			break;
		}

		sco_pi(sk)->codec = codecs->codecs[0];
		hci_dev_put(hdev);
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		err = -ENOPROTOOPT;
		break;
	}

	release_sock(sk);
	return err;
}

<<<<<<< HEAD
static int sco_sock_getsockopt_old(struct socket *sock, int optname, char __user *optval, int __user *optlen)
=======
static int sco_sock_getsockopt_old(struct socket *sock, int optname,
				   char __user *optval, int __user *optlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct sco_options opts;
	struct sco_conninfo cinfo;
<<<<<<< HEAD
	int len, err = 0;
=======
	int err = 0;
	size_t len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BT_DBG("sk %p", sk);

	if (get_user(len, optlen))
		return -EFAULT;

	lock_sock(sk);

	switch (optname) {
	case SCO_OPTIONS:
<<<<<<< HEAD
		if (sk->sk_state != BT_CONNECTED) {
=======
		if (sk->sk_state != BT_CONNECTED &&
		    !(sk->sk_state == BT_CONNECT2 &&
		      test_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENOTCONN;
			break;
		}

		opts.mtu = sco_pi(sk)->conn->mtu;

<<<<<<< HEAD
		BT_DBG("mtu %d", opts.mtu);

		len = min_t(unsigned int, len, sizeof(opts));
=======
		BT_DBG("mtu %u", opts.mtu);

		len = min(len, sizeof(opts));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (copy_to_user(optval, (char *)&opts, len))
			err = -EFAULT;

		break;

	case SCO_CONNINFO:
<<<<<<< HEAD
		if (sk->sk_state != BT_CONNECTED) {
=======
		if (sk->sk_state != BT_CONNECTED &&
		    !(sk->sk_state == BT_CONNECT2 &&
		      test_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -ENOTCONN;
			break;
		}

		memset(&cinfo, 0, sizeof(cinfo));
		cinfo.hci_handle = sco_pi(sk)->conn->hcon->handle;
		memcpy(cinfo.dev_class, sco_pi(sk)->conn->hcon->dev_class, 3);

<<<<<<< HEAD
		len = min_t(unsigned int, len, sizeof(cinfo));
=======
		len = min(len, sizeof(cinfo));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (copy_to_user(optval, (char *)&cinfo, len))
			err = -EFAULT;

		break;

	default:
		err = -ENOPROTOOPT;
		break;
	}

	release_sock(sk);
	return err;
}

<<<<<<< HEAD
static int sco_sock_getsockopt(struct socket *sock, int level, int optname, char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	int len, err = 0;
=======
static int sco_sock_getsockopt(struct socket *sock, int level, int optname,
			       char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	int len, err = 0;
	struct bt_voice voice;
	u32 phys;
	int buf_len;
	struct codec_list *c;
	u8 num_codecs, i, __user *ptr;
	struct hci_dev *hdev;
	struct hci_codec_caps *caps;
	struct bt_codec codec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BT_DBG("sk %p", sk);

	if (level == SOL_SCO)
		return sco_sock_getsockopt_old(sock, optname, optval, optlen);

	if (get_user(len, optlen))
		return -EFAULT;

	lock_sock(sk);

	switch (optname) {
<<<<<<< HEAD
=======

	case BT_DEFER_SETUP:
		if (sk->sk_state != BT_BOUND && sk->sk_state != BT_LISTEN) {
			err = -EINVAL;
			break;
		}

		if (put_user(test_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags),
			     (u32 __user *)optval))
			err = -EFAULT;

		break;

	case BT_VOICE:
		voice.setting = sco_pi(sk)->setting;

		len = min_t(unsigned int, len, sizeof(voice));
		if (copy_to_user(optval, (char *)&voice, len))
			err = -EFAULT;

		break;

	case BT_PHY:
		if (sk->sk_state != BT_CONNECTED) {
			err = -ENOTCONN;
			break;
		}

		phys = hci_conn_get_phy(sco_pi(sk)->conn->hcon);

		if (put_user(phys, (u32 __user *) optval))
			err = -EFAULT;
		break;

	case BT_PKT_STATUS:
		if (put_user(test_bit(BT_SK_PKT_STATUS, &bt_sk(sk)->flags),
			     (int __user *)optval))
			err = -EFAULT;
		break;

	case BT_SNDMTU:
	case BT_RCVMTU:
		if (sk->sk_state != BT_CONNECTED) {
			err = -ENOTCONN;
			break;
		}

		if (put_user(sco_pi(sk)->conn->mtu, (u32 __user *)optval))
			err = -EFAULT;
		break;

	case BT_CODEC:
		num_codecs = 0;
		buf_len = 0;

		hdev = hci_get_route(&sco_pi(sk)->dst, &sco_pi(sk)->src, BDADDR_BREDR);
		if (!hdev) {
			err = -EBADFD;
			break;
		}

		if (!hci_dev_test_flag(hdev, HCI_OFFLOAD_CODECS_ENABLED)) {
			hci_dev_put(hdev);
			err = -EOPNOTSUPP;
			break;
		}

		if (!hdev->get_data_path_id) {
			hci_dev_put(hdev);
			err = -EOPNOTSUPP;
			break;
		}

		release_sock(sk);

		/* find total buffer size required to copy codec + caps */
		hci_dev_lock(hdev);
		list_for_each_entry(c, &hdev->local_codecs, list) {
			if (c->transport != HCI_TRANSPORT_SCO_ESCO)
				continue;
			num_codecs++;
			for (i = 0, caps = c->caps; i < c->num_caps; i++) {
				buf_len += 1 + caps->len;
				caps = (void *)&caps->data[caps->len];
			}
			buf_len += sizeof(struct bt_codec);
		}
		hci_dev_unlock(hdev);

		buf_len += sizeof(struct bt_codecs);
		if (buf_len > len) {
			hci_dev_put(hdev);
			return -ENOBUFS;
		}
		ptr = optval;

		if (put_user(num_codecs, ptr)) {
			hci_dev_put(hdev);
			return -EFAULT;
		}
		ptr += sizeof(num_codecs);

		/* Iterate all the codecs supported over SCO and populate
		 * codec data
		 */
		hci_dev_lock(hdev);
		list_for_each_entry(c, &hdev->local_codecs, list) {
			if (c->transport != HCI_TRANSPORT_SCO_ESCO)
				continue;

			codec.id = c->id;
			codec.cid = c->cid;
			codec.vid = c->vid;
			err = hdev->get_data_path_id(hdev, &codec.data_path);
			if (err < 0)
				break;
			codec.num_caps = c->num_caps;
			if (copy_to_user(ptr, &codec, sizeof(codec))) {
				err = -EFAULT;
				break;
			}
			ptr += sizeof(codec);

			/* find codec capabilities data length */
			len = 0;
			for (i = 0, caps = c->caps; i < c->num_caps; i++) {
				len += 1 + caps->len;
				caps = (void *)&caps->data[caps->len];
			}

			/* copy codec capabilities data */
			if (len && copy_to_user(ptr, c->caps, len)) {
				err = -EFAULT;
				break;
			}
			ptr += len;
		}

		hci_dev_unlock(hdev);
		hci_dev_put(hdev);

		lock_sock(sk);

		if (!err && put_user(buf_len, optlen))
			err = -EFAULT;

		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		err = -ENOPROTOOPT;
		break;
	}

	release_sock(sk);
	return err;
}

static int sco_sock_shutdown(struct socket *sock, int how)
{
	struct sock *sk = sock->sk;
	int err = 0;

	BT_DBG("sock %p, sk %p", sock, sk);

	if (!sk)
		return 0;

<<<<<<< HEAD
	lock_sock(sk);
=======
	sock_hold(sk);
	lock_sock(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!sk->sk_shutdown) {
		sk->sk_shutdown = SHUTDOWN_MASK;
		sco_sock_clear_timer(sk);
		__sco_sock_close(sk);

<<<<<<< HEAD
		if (sock_flag(sk, SOCK_LINGER) && sk->sk_lingertime)
			err = bt_sock_wait_state(sk, BT_CLOSED,
							sk->sk_lingertime);
		else
			err = bt_sock_wait_state(sk, BT_CLOSED,
							SCO_DISCONN_TIMEOUT);
	}
	release_sock(sk);
=======
		if (sock_flag(sk, SOCK_LINGER) && sk->sk_lingertime &&
		    !(current->flags & PF_EXITING))
			err = bt_sock_wait_state(sk, BT_CLOSED,
						 sk->sk_lingertime);
	}

	release_sock(sk);
	sock_put(sk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int sco_sock_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	int err = 0;

	BT_DBG("sock %p, sk %p", sock, sk);

	if (!sk)
		return 0;

	sco_sock_close(sk);

<<<<<<< HEAD
	if (sock_flag(sk, SOCK_LINGER) && sk->sk_lingertime) {
		lock_sock(sk);
		err = bt_sock_wait_state(sk, BT_CLOSED, sk->sk_lingertime);
		release_sock(sk);
	} else {
		lock_sock(sk);
		err = bt_sock_wait_state(sk, BT_CLOSED,
							SCO_DISCONN_TIMEOUT);
		release_sock(sk);
=======
	if (sock_flag(sk, SOCK_LINGER) && READ_ONCE(sk->sk_lingertime) &&
	    !(current->flags & PF_EXITING)) {
		lock_sock(sk);
		err = bt_sock_wait_state(sk, BT_CLOSED, sk->sk_lingertime);
		release_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	sock_orphan(sk);
	sco_sock_kill(sk);
	return err;
}

<<<<<<< HEAD
static void __sco_chan_add(struct sco_conn *conn, struct sock *sk, struct sock *parent)
{
	BT_DBG("conn %p", conn);

	sco_pi(sk)->conn = conn;
	conn->sk = sk;

	if (parent)
		bt_accept_enqueue(parent, sk);
}

/* Delete channel.
 * Must be called on the locked socket. */
static void sco_chan_del(struct sock *sk, int err)
{
	struct sco_conn *conn;

	conn = sco_pi(sk)->conn;

	BT_DBG("sk %p, conn %p, err %d", sk, conn, err);

	if (conn) {
		sco_conn_lock(conn);
		conn->sk = NULL;
		sco_pi(sk)->conn = NULL;
		sco_conn_unlock(conn);

		if (conn->hcon)
			hci_conn_put(conn->hcon);
	}

	sk->sk_state = BT_CLOSED;
	sk->sk_err   = err;
	sk->sk_state_change(sk);

	sock_set_flag(sk, SOCK_ZAPPED);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void sco_conn_ready(struct sco_conn *conn)
{
	struct sock *parent;
	struct sock *sk = conn->sk;

	BT_DBG("conn %p", conn);

<<<<<<< HEAD
	sco_conn_lock(conn);

	if (sk) {
		sco_sock_clear_timer(sk);
		bh_lock_sock(sk);
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
		bh_unlock_sock(sk);
	} else {
		parent = sco_get_sock_listen(conn->src);
		if (!parent)
			goto done;

		bh_lock_sock(parent);

		sk = sco_sock_alloc(sock_net(parent), NULL,
				BTPROTO_SCO, GFP_ATOMIC);
		if (!sk) {
			bh_unlock_sock(parent);
			goto done;
=======
	if (sk) {
		lock_sock(sk);
		sco_sock_clear_timer(sk);
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
		release_sock(sk);
	} else {
		sco_conn_lock(conn);

		if (!conn->hcon) {
			sco_conn_unlock(conn);
			return;
		}

		parent = sco_get_sock_listen(&conn->hcon->src);
		if (!parent) {
			sco_conn_unlock(conn);
			return;
		}

		lock_sock(parent);

		sk = sco_sock_alloc(sock_net(parent), NULL,
				    BTPROTO_SCO, GFP_ATOMIC, 0);
		if (!sk) {
			release_sock(parent);
			sco_conn_unlock(conn);
			return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		sco_sock_init(sk, parent);

<<<<<<< HEAD
		bacpy(&bt_sk(sk)->src, conn->src);
		bacpy(&bt_sk(sk)->dst, conn->dst);
=======
		bacpy(&sco_pi(sk)->src, &conn->hcon->src);
		bacpy(&sco_pi(sk)->dst, &conn->hcon->dst);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		hci_conn_hold(conn->hcon);
		__sco_chan_add(conn, sk, parent);

<<<<<<< HEAD
		sk->sk_state = BT_CONNECTED;

		/* Wake up parent */
		parent->sk_data_ready(parent, 1);

		bh_unlock_sock(parent);
	}

done:
	sco_conn_unlock(conn);
}

/* ----- SCO interface with lower layer (HCI) ----- */
static int sco_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 type)
{
	register struct sock *sk;
	struct hlist_node *node;
	int lm = 0;

	if (type != SCO_LINK && type != ESCO_LINK)
		return 0;

	BT_DBG("hdev %s, bdaddr %s", hdev->name, batostr(bdaddr));

	/* Find listening sockets */
	read_lock(&sco_sk_list.lock);
	sk_for_each(sk, node, &sco_sk_list.head) {
		if (sk->sk_state != BT_LISTEN)
			continue;

		if (!bacmp(&bt_sk(sk)->src, &hdev->bdaddr) ||
				!bacmp(&bt_sk(sk)->src, BDADDR_ANY)) {
			lm |= HCI_LM_ACCEPT;
=======
		if (test_bit(BT_SK_DEFER_SETUP, &bt_sk(parent)->flags))
			sk->sk_state = BT_CONNECT2;
		else
			sk->sk_state = BT_CONNECTED;

		/* Wake up parent */
		parent->sk_data_ready(parent);

		release_sock(parent);

		sco_conn_unlock(conn);
	}
}

/* ----- SCO interface with lower layer (HCI) ----- */
int sco_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, __u8 *flags)
{
	struct sock *sk;
	int lm = 0;

	BT_DBG("hdev %s, bdaddr %pMR", hdev->name, bdaddr);

	/* Find listening sockets */
	read_lock(&sco_sk_list.lock);
	sk_for_each(sk, &sco_sk_list.head) {
		if (sk->sk_state != BT_LISTEN)
			continue;

		if (!bacmp(&sco_pi(sk)->src, &hdev->bdaddr) ||
		    !bacmp(&sco_pi(sk)->src, BDADDR_ANY)) {
			lm |= HCI_LM_ACCEPT;

			if (test_bit(BT_SK_DEFER_SETUP, &bt_sk(sk)->flags))
				*flags |= HCI_PROTO_DEFER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}
	}
	read_unlock(&sco_sk_list.lock);

	return lm;
}

<<<<<<< HEAD
static int sco_connect_cfm(struct hci_conn *hcon, __u8 status)
{
	BT_DBG("hcon %p bdaddr %s status %d", hcon, batostr(&hcon->dst), status);

	if (hcon->type != SCO_LINK && hcon->type != ESCO_LINK)
		return -EINVAL;
=======
static void sco_connect_cfm(struct hci_conn *hcon, __u8 status)
{
	if (hcon->type != SCO_LINK && hcon->type != ESCO_LINK)
		return;

	BT_DBG("hcon %p bdaddr %pMR status %u", hcon, &hcon->dst, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!status) {
		struct sco_conn *conn;

<<<<<<< HEAD
		conn = sco_conn_add(hcon, status);
		if (conn)
			sco_conn_ready(conn);
	} else
		sco_conn_del(hcon, bt_err(status), 0);

	return 0;
}

static int sco_disconn_cfm(struct hci_conn *hcon, __u8 reason, __u8 is_process)
{
	BT_DBG("hcon %p reason %d", hcon, reason);

	if (hcon->type != SCO_LINK && hcon->type != ESCO_LINK)
		return -EINVAL;

	sco_conn_del(hcon, bt_err(reason), is_process);

	return 0;
}

static int sco_recv_scodata(struct hci_conn *hcon, struct sk_buff *skb)
=======
		conn = sco_conn_add(hcon);
		if (conn)
			sco_conn_ready(conn);
	} else
		sco_conn_del(hcon, bt_to_errno(status));
}

static void sco_disconn_cfm(struct hci_conn *hcon, __u8 reason)
{
	if (hcon->type != SCO_LINK && hcon->type != ESCO_LINK)
		return;

	BT_DBG("hcon %p reason %d", hcon, reason);

	sco_conn_del(hcon, bt_to_errno(reason));
}

void sco_recv_scodata(struct hci_conn *hcon, struct sk_buff *skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sco_conn *conn = hcon->sco_data;

	if (!conn)
		goto drop;

<<<<<<< HEAD
	BT_DBG("conn %p len %d", conn, skb->len);

	if (skb->len) {
		sco_recv_frame(conn, skb);
		return 0;
=======
	BT_DBG("conn %p len %u", conn, skb->len);

	if (skb->len) {
		sco_recv_frame(conn, skb);
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

drop:
	kfree_skb(skb);
<<<<<<< HEAD
	return 0;
}

static int sco_debugfs_show(struct seq_file *f, void *p)
{
	struct sock *sk;
	struct hlist_node *node;

	read_lock_bh(&sco_sk_list.lock);

	sk_for_each(sk, node, &sco_sk_list.head) {
		seq_printf(f, "%s %s %d\n", batostr(&bt_sk(sk)->src),
				batostr(&bt_sk(sk)->dst), sk->sk_state);
	}

	read_unlock_bh(&sco_sk_list.lock);
=======
}

static struct hci_cb sco_cb = {
	.name		= "SCO",
	.connect_cfm	= sco_connect_cfm,
	.disconn_cfm	= sco_disconn_cfm,
};

static int sco_debugfs_show(struct seq_file *f, void *p)
{
	struct sock *sk;

	read_lock(&sco_sk_list.lock);

	sk_for_each(sk, &sco_sk_list.head) {
		seq_printf(f, "%pMR %pMR %d\n", &sco_pi(sk)->src,
			   &sco_pi(sk)->dst, sk->sk_state);
	}

	read_unlock(&sco_sk_list.lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int sco_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, sco_debugfs_show, inode->i_private);
}

static const struct file_operations sco_debugfs_fops = {
	.open		= sco_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
=======
DEFINE_SHOW_ATTRIBUTE(sco_debugfs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct dentry *sco_debugfs;

static const struct proto_ops sco_sock_ops = {
	.family		= PF_BLUETOOTH,
	.owner		= THIS_MODULE,
	.release	= sco_sock_release,
	.bind		= sco_sock_bind,
	.connect	= sco_sock_connect,
	.listen		= sco_sock_listen,
	.accept		= sco_sock_accept,
	.getname	= sco_sock_getname,
	.sendmsg	= sco_sock_sendmsg,
<<<<<<< HEAD
	.recvmsg	= bt_sock_recvmsg,
	.poll		= bt_sock_poll,
	.ioctl		= bt_sock_ioctl,
=======
	.recvmsg	= sco_sock_recvmsg,
	.poll		= bt_sock_poll,
	.ioctl		= bt_sock_ioctl,
	.gettstamp	= sock_gettstamp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.mmap		= sock_no_mmap,
	.socketpair	= sock_no_socketpair,
	.shutdown	= sco_sock_shutdown,
	.setsockopt	= sco_sock_setsockopt,
	.getsockopt	= sco_sock_getsockopt
};

static const struct net_proto_family sco_sock_family_ops = {
	.family	= PF_BLUETOOTH,
	.owner	= THIS_MODULE,
	.create	= sco_sock_create,
};

<<<<<<< HEAD
static struct hci_proto sco_hci_proto = {
	.name		= "SCO",
	.id		= HCI_PROTO_SCO,
	.connect_ind	= sco_connect_ind,
	.connect_cfm	= sco_connect_cfm,
	.disconn_cfm	= sco_disconn_cfm,
	.recv_scodata	= sco_recv_scodata
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init sco_init(void)
{
	int err;

<<<<<<< HEAD
=======
	BUILD_BUG_ON(sizeof(struct sockaddr_sco) > sizeof(struct sockaddr));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = proto_register(&sco_proto, 0);
	if (err < 0)
		return err;

	err = bt_sock_register(BTPROTO_SCO, &sco_sock_family_ops);
	if (err < 0) {
		BT_ERR("SCO socket registration failed");
		goto error;
	}

<<<<<<< HEAD
	err = hci_register_proto(&sco_hci_proto);
	if (err < 0) {
		BT_ERR("SCO protocol registration failed");
=======
	err = bt_procfs_init(&init_net, "sco", &sco_sk_list, NULL);
	if (err < 0) {
		BT_ERR("Failed to create SCO proc file");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bt_sock_unregister(BTPROTO_SCO);
		goto error;
	}

<<<<<<< HEAD
	if (bt_debugfs) {
		sco_debugfs = debugfs_create_file("sco", 0444,
					bt_debugfs, NULL, &sco_debugfs_fops);
		if (!sco_debugfs)
			BT_ERR("Failed to create SCO debug file");
	}

	BT_INFO("SCO socket layer initialized");

=======
	BT_INFO("SCO socket layer initialized");

	hci_register_cb(&sco_cb);

	if (IS_ERR_OR_NULL(bt_debugfs))
		return 0;

	sco_debugfs = debugfs_create_file("sco", 0444, bt_debugfs,
					  NULL, &sco_debugfs_fops);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

error:
	proto_unregister(&sco_proto);
	return err;
}

<<<<<<< HEAD
void __exit sco_exit(void)
{
	debugfs_remove(sco_debugfs);

	if (bt_sock_unregister(BTPROTO_SCO) < 0)
		BT_ERR("SCO socket unregistration failed");

	if (hci_unregister_proto(&sco_hci_proto) < 0)
		BT_ERR("SCO protocol unregistration failed");
=======
void sco_exit(void)
{
	bt_procfs_cleanup(&init_net, "sco");

	debugfs_remove(sco_debugfs);

	hci_unregister_cb(&sco_cb);

	bt_sock_unregister(BTPROTO_SCO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	proto_unregister(&sco_proto);
}

module_param(disable_esco, bool, 0644);
MODULE_PARM_DESC(disable_esco, "Disable eSCO connection creation");
