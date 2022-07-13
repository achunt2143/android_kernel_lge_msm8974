<<<<<<< HEAD
#ifndef _RDMA_NETLINK_H
#define _RDMA_NETLINK_H

#include <linux/types.h>

enum {
	RDMA_NL_RDMA_CM = 1
};

#define RDMA_NL_GET_CLIENT(type) ((type & (((1 << 6) - 1) << 10)) >> 10)
#define RDMA_NL_GET_OP(type) (type & ((1 << 10) - 1))
#define RDMA_NL_GET_TYPE(client, op) ((client << 10) + op)

enum {
	RDMA_NL_RDMA_CM_ID_STATS = 0,
	RDMA_NL_RDMA_CM_NUM_OPS
};

enum {
	RDMA_NL_RDMA_CM_ATTR_SRC_ADDR = 1,
	RDMA_NL_RDMA_CM_ATTR_DST_ADDR,
	RDMA_NL_RDMA_CM_NUM_ATTR,
};

struct rdma_cm_id_stats {
	__u32	qp_num;
	__u32	bound_dev_if;
	__u32	port_space;
	__s32	pid;
	__u8	cm_state;
	__u8	node_type;
	__u8	port_num;
	__u8	qp_type;
};

#ifdef __KERNEL__

#include <linux/netlink.h>

struct ibnl_client_cbs {
	int (*dump)(struct sk_buff *skb, struct netlink_callback *nlcb);
	struct module *module;
};

int ibnl_init(void);
void ibnl_cleanup(void);

/**
 * Add a a client to the list of IB netlink exporters.
 * @index: Index of the added client
 * @nops: Number of supported ops by the added client.
 * @cb_table: A table for op->callback
 *
 * Returns 0 on success or a negative error code.
 */
int ibnl_add_client(int index, int nops,
		    const struct ibnl_client_cbs cb_table[]);
=======
/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _RDMA_NETLINK_H
#define _RDMA_NETLINK_H

#include <linux/netlink.h>
#include <uapi/rdma/rdma_netlink.h>

enum {
	RDMA_NLDEV_ATTR_EMPTY_STRING = 1,
	RDMA_NLDEV_ATTR_ENTRY_STRLEN = 16,
	RDMA_NLDEV_ATTR_CHARDEV_TYPE_SIZE = 32,
};

struct rdma_nl_cbs {
	int (*doit)(struct sk_buff *skb, struct nlmsghdr *nlh,
		    struct netlink_ext_ack *extack);
	int (*dump)(struct sk_buff *skb, struct netlink_callback *nlcb);
	u8 flags;
};

enum rdma_nl_flags {
	/* Require CAP_NET_ADMIN */
	RDMA_NL_ADMIN_PERM	= 1 << 0,
};

/* Define this module as providing netlink services for NETLINK_RDMA, with
 * index _index.  Since the client indexes were setup in a uapi header as an
 * enum and we do no want to change that, the user must supply the expanded
 * constant as well and the compiler checks they are the same.
 */
#define MODULE_ALIAS_RDMA_NETLINK(_index, _val)                                \
	static inline void __maybe_unused __chk_##_index(void)                 \
	{                                                                      \
		BUILD_BUG_ON(_index != _val);                                  \
	}                                                                      \
	MODULE_ALIAS("rdma-netlink-subsys-" __stringify(_val))

/**
 * Register client in RDMA netlink.
 * @index: Index of the added client
 * @cb_table: A table for op->callback
 */
void rdma_nl_register(unsigned int index,
		      const struct rdma_nl_cbs cb_table[]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Remove a client from IB netlink.
 * @index: Index of the removed IB client.
<<<<<<< HEAD
 *
 * Returns 0 on success or a negative error code.
 */
int ibnl_remove_client(int index);
=======
 */
void rdma_nl_unregister(unsigned int index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * Put a new message in a supplied skb.
 * @skb: The netlink skb.
 * @nlh: Pointer to put the header of the new netlink message.
 * @seq: The message sequence number.
 * @len: The requested message length to allocate.
 * @client: Calling IB netlink client.
 * @op: message content op.
 * Returns the allocated buffer on success and NULL on failure.
 */
void *ibnl_put_msg(struct sk_buff *skb, struct nlmsghdr **nlh, int seq,
<<<<<<< HEAD
		   int len, int client, int op);
=======
		   int len, int client, int op, int flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * Put a new attribute in a supplied skb.
 * @skb: The netlink skb.
 * @nlh: Header of the netlink message to append the attribute to.
 * @len: The length of the attribute data.
 * @data: The attribute data to put.
 * @type: The attribute type.
 * Returns the 0 and a negative error code on failure.
 */
int ibnl_put_attr(struct sk_buff *skb, struct nlmsghdr *nlh,
		  int len, void *data, int type);

<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
/**
 * Send the supplied skb to a specific userspace PID.
 * @net: Net namespace in which to send the skb
 * @skb: The netlink skb
 * @pid: Userspace netlink process ID
 * Returns 0 on success or a negative error code.
 */
int rdma_nl_unicast(struct net *net, struct sk_buff *skb, u32 pid);

/**
 * Send, with wait/1 retry, the supplied skb to a specific userspace PID.
 * @net: Net namespace in which to send the skb
 * @skb: The netlink skb
 * @pid: Userspace netlink process ID
 * Returns 0 on success or a negative error code.
 */
int rdma_nl_unicast_wait(struct net *net, struct sk_buff *skb, __u32 pid);

/**
 * Send the supplied skb to a netlink group.
 * @net: Net namespace in which to send the skb
 * @skb: The netlink skb
 * @group: Netlink group ID
 * @flags: allocation flags
 * Returns 0 on success or a negative error code.
 */
int rdma_nl_multicast(struct net *net, struct sk_buff *skb,
		      unsigned int group, gfp_t flags);

/**
 * Check if there are any listeners to the netlink group
 * @group: the netlink group ID
 * Returns true on success or false if no listeners.
 */
bool rdma_nl_chk_listeners(unsigned int group);

struct rdma_link_ops {
	struct list_head list;
	const char *type;
	int (*newlink)(const char *ibdev_name, struct net_device *ndev);
};

void rdma_link_register(struct rdma_link_ops *ops);
void rdma_link_unregister(struct rdma_link_ops *ops);

#define MODULE_ALIAS_RDMA_LINK(type) MODULE_ALIAS("rdma-link-" type)
#define MODULE_ALIAS_RDMA_CLIENT(type) MODULE_ALIAS("rdma-client-" type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _RDMA_NETLINK_H */
