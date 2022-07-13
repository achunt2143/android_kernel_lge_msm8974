/*
 * net/tipc/core.c: TIPC module code
 *
<<<<<<< HEAD
 * Copyright (c) 2003-2006, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2003-2006, 2013, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2013, Wind River Systems
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

<<<<<<< HEAD
#include <linux/module.h>

#include "core.h"
#include "ref.h"
#include "name_table.h"
#include "subscr.h"
#include "config.h"


#ifndef CONFIG_TIPC_PORTS
#define CONFIG_TIPC_PORTS 8191
#endif

#ifndef CONFIG_TIPC_LOG
#define CONFIG_TIPC_LOG 0
#endif

/* global variables used by multiple sub-systems within TIPC */

int tipc_random;

const char tipc_alphabet[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.";

/* configurable TIPC parameters */

u32 tipc_own_addr;
int tipc_max_ports;
int tipc_max_subscriptions;
int tipc_max_publications;
int tipc_net_id;
int tipc_remote_management;


/**
 * tipc_buf_acquire - creates a TIPC message buffer
 * @size: message size (including TIPC header)
 *
 * Returns a new buffer with data pointers set to the specified size.
 *
 * NOTE: Headroom is reserved to allow prepending of a data link header.
 *       There may also be unrequested tailroom present at the buffer's end.
 */

struct sk_buff *tipc_buf_acquire(u32 size)
{
	struct sk_buff *skb;
	unsigned int buf_size = (BUF_HEADROOM + size + 3) & ~3u;

	skb = alloc_skb_fclone(buf_size, GFP_ATOMIC);
	if (skb) {
		skb_reserve(skb, BUF_HEADROOM);
		skb_put(skb, size);
		skb->next = NULL;
	}
	return skb;
}

/**
 * tipc_core_stop_net - shut down TIPC networking sub-systems
 */

static void tipc_core_stop_net(void)
{
	tipc_net_stop();
	tipc_eth_media_stop();
}

/**
 * start_net - start TIPC networking sub-systems
 */

int tipc_core_start_net(unsigned long addr)
{
	int res;

	res = tipc_net_start(addr);
	if (!res)
		res = tipc_eth_media_start();
	if (res)
		tipc_core_stop_net();
	return res;
}

/**
 * tipc_core_stop - switch TIPC from SINGLE NODE to NOT RUNNING mode
 */

static void tipc_core_stop(void)
{
	tipc_netlink_stop();
	tipc_handler_stop();
	tipc_cfg_stop();
	tipc_subscr_stop();
	tipc_nametbl_stop();
	tipc_ref_table_stop();
	tipc_socket_stop();
	tipc_log_resize(0);
}

/**
 * tipc_core_start - switch TIPC from NOT RUNNING to SINGLE NODE mode
 */

static int tipc_core_start(void)
{
	int res;

	get_random_bytes(&tipc_random, sizeof(tipc_random));

	res = tipc_handler_start();
	if (!res)
		res = tipc_ref_table_init(tipc_max_ports, tipc_random);
	if (!res)
		res = tipc_nametbl_init();
	if (!res)
		res = tipc_k_signal((Handler)tipc_subscr_start, 0);
	if (!res)
		res = tipc_k_signal((Handler)tipc_cfg_init, 0);
	if (!res)
		res = tipc_netlink_start();
	if (!res)
		res = tipc_socket_init();
	if (res)
		tipc_core_stop();

	return res;
}


static int __init tipc_init(void)
{
	int res;

	if (tipc_log_resize(CONFIG_TIPC_LOG) != 0)
		warn("Unable to create log buffer\n");

	info("Activated (version " TIPC_MOD_VER ")\n");

	tipc_own_addr = 0;
	tipc_remote_management = 1;
	tipc_max_publications = 10000;
	tipc_max_subscriptions = 2000;
	tipc_max_ports = CONFIG_TIPC_PORTS;
	tipc_net_id = 4711;

	res = tipc_core_start();
	if (res)
		err("Unable to start in single node mode\n");
	else
		info("Started in single node mode\n");
	return res;
=======
#include "core.h"
#include "name_table.h"
#include "subscr.h"
#include "bearer.h"
#include "net.h"
#include "socket.h"
#include "bcast.h"
#include "node.h"
#include "crypto.h"

#include <linux/module.h>

/* configurable TIPC parameters */
unsigned int tipc_net_id __read_mostly;
int sysctl_tipc_rmem[3] __read_mostly;	/* min/default/max */

static int __net_init tipc_init_net(struct net *net)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	int err;

	tn->net_id = 4711;
	tn->node_addr = 0;
	tn->trial_addr = 0;
	tn->addr_trial_end = 0;
	tn->capabilities = TIPC_NODE_CAPABILITIES;
	INIT_WORK(&tn->work, tipc_net_finalize_work);
	memset(tn->node_id, 0, sizeof(tn->node_id));
	memset(tn->node_id_string, 0, sizeof(tn->node_id_string));
	tn->mon_threshold = TIPC_DEF_MON_THRESHOLD;
	get_random_bytes(&tn->random, sizeof(int));
	INIT_LIST_HEAD(&tn->node_list);
	spin_lock_init(&tn->node_list_lock);

#ifdef CONFIG_TIPC_CRYPTO
	err = tipc_crypto_start(&tn->crypto_tx, net, NULL);
	if (err)
		goto out_crypto;
#endif
	err = tipc_sk_rht_init(net);
	if (err)
		goto out_sk_rht;

	err = tipc_nametbl_init(net);
	if (err)
		goto out_nametbl;

	err = tipc_bcast_init(net);
	if (err)
		goto out_bclink;

	err = tipc_attach_loopback(net);
	if (err)
		goto out_bclink;

	return 0;

out_bclink:
	tipc_nametbl_stop(net);
out_nametbl:
	tipc_sk_rht_destroy(net);
out_sk_rht:

#ifdef CONFIG_TIPC_CRYPTO
	tipc_crypto_stop(&tn->crypto_tx);
out_crypto:
#endif
	return err;
}

static void __net_exit tipc_exit_net(struct net *net)
{
	struct tipc_net *tn = tipc_net(net);

	tipc_detach_loopback(net);
	tipc_net_stop(net);
	/* Make sure the tipc_net_finalize_work() finished */
	cancel_work_sync(&tn->work);
	tipc_bcast_stop(net);
	tipc_nametbl_stop(net);
	tipc_sk_rht_destroy(net);
#ifdef CONFIG_TIPC_CRYPTO
	tipc_crypto_stop(&tipc_net(net)->crypto_tx);
#endif
	while (atomic_read(&tn->wq_count))
		cond_resched();
}

static void __net_exit tipc_pernet_pre_exit(struct net *net)
{
	tipc_node_pre_cleanup_net(net);
}

static struct pernet_operations tipc_pernet_pre_exit_ops = {
	.pre_exit = tipc_pernet_pre_exit,
};

static struct pernet_operations tipc_net_ops = {
	.init = tipc_init_net,
	.exit = tipc_exit_net,
	.id   = &tipc_net_id,
	.size = sizeof(struct tipc_net),
};

static struct pernet_operations tipc_topsrv_net_ops = {
	.init = tipc_topsrv_init_net,
	.exit = tipc_topsrv_exit_net,
};

static int __init tipc_init(void)
{
	int err;

	pr_info("Activated (version " TIPC_MOD_VER ")\n");

	sysctl_tipc_rmem[0] = RCVBUF_MIN;
	sysctl_tipc_rmem[1] = RCVBUF_DEF;
	sysctl_tipc_rmem[2] = RCVBUF_MAX;

	err = tipc_register_sysctl();
	if (err)
		goto out_sysctl;

	err = register_pernet_device(&tipc_net_ops);
	if (err)
		goto out_pernet;

	err = tipc_socket_init();
	if (err)
		goto out_socket;

	err = register_pernet_device(&tipc_topsrv_net_ops);
	if (err)
		goto out_pernet_topsrv;

	err = register_pernet_subsys(&tipc_pernet_pre_exit_ops);
	if (err)
		goto out_register_pernet_subsys;

	err = tipc_bearer_setup();
	if (err)
		goto out_bearer;

	err = tipc_netlink_start();
	if (err)
		goto out_netlink;

	err = tipc_netlink_compat_start();
	if (err)
		goto out_netlink_compat;

	pr_info("Started in single node mode\n");
	return 0;

out_netlink_compat:
	tipc_netlink_stop();
out_netlink:
	tipc_bearer_cleanup();
out_bearer:
	unregister_pernet_subsys(&tipc_pernet_pre_exit_ops);
out_register_pernet_subsys:
	unregister_pernet_device(&tipc_topsrv_net_ops);
out_pernet_topsrv:
	tipc_socket_stop();
out_socket:
	unregister_pernet_device(&tipc_net_ops);
out_pernet:
	tipc_unregister_sysctl();
out_sysctl:
	pr_err("Unable to start in single node mode\n");
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit tipc_exit(void)
{
<<<<<<< HEAD
	tipc_core_stop_net();
	tipc_core_stop();
	info("Deactivated\n");
=======
	tipc_netlink_compat_stop();
	tipc_netlink_stop();
	tipc_bearer_cleanup();
	unregister_pernet_subsys(&tipc_pernet_pre_exit_ops);
	unregister_pernet_device(&tipc_topsrv_net_ops);
	tipc_socket_stop();
	unregister_pernet_device(&tipc_net_ops);
	tipc_unregister_sysctl();

	pr_info("Deactivated\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(tipc_init);
module_exit(tipc_exit);

MODULE_DESCRIPTION("TIPC: Transparent Inter Process Communication");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION(TIPC_MOD_VER);
