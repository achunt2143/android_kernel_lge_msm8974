<<<<<<< HEAD
/*
 * Copyright (C) 2002 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2002 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __UM_NET_KERN_H
#define __UM_NET_KERN_H

#include <linux/netdevice.h>
#include <linux/platform_device.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/list.h>
#include <linux/workqueue.h>

struct uml_net {
	struct list_head list;
	struct net_device *dev;
	struct platform_device pdev;
	int index;
<<<<<<< HEAD
	unsigned char mac[ETH_ALEN];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct uml_net_private {
	struct list_head list;
	spinlock_t lock;
	struct net_device *dev;
	struct timer_list tl;

	struct work_struct work;
	int fd;
	unsigned char mac[ETH_ALEN];
	int max_packet;
	unsigned short (*protocol)(struct sk_buff *);
	int (*open)(void *);
	void (*close)(int, void *);
	void (*remove)(void *);
	int (*read)(int, struct sk_buff *skb, struct uml_net_private *);
	int (*write)(int, struct sk_buff *skb, struct uml_net_private *);

	void (*add_address)(unsigned char *, unsigned char *, void *);
	void (*delete_address)(unsigned char *, unsigned char *, void *);
<<<<<<< HEAD
	char user[0];
=======
	char user[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct net_kern_info {
	void (*init)(struct net_device *, void *);
	unsigned short (*protocol)(struct sk_buff *);
	int (*read)(int, struct sk_buff *skb, struct uml_net_private *);
	int (*write)(int, struct sk_buff *skb, struct uml_net_private *);
};

struct transport {
	struct list_head list;
	const char *name;
	int (* const setup)(char *, char **, void *);
	const struct net_user_info *user;
	const struct net_kern_info *kern;
	const int private_size;
	const int setup_size;
};

<<<<<<< HEAD
extern struct net_device *ether_init(int);
extern unsigned short ether_protocol(struct sk_buff *);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int tap_setup_common(char *str, char *type, char **dev_name,
			    char **mac_out, char **gate_addr);
extern void register_transport(struct transport *new);
extern unsigned short eth_protocol(struct sk_buff *skb);
<<<<<<< HEAD
=======
extern void uml_net_setup_etheraddr(struct net_device *dev, char *str);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
