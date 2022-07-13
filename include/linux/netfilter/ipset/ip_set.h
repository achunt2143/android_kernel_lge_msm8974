<<<<<<< HEAD
#ifndef _IP_SET_H
#define _IP_SET_H

/* Copyright (C) 2000-2002 Joakim Axelsson <gozem@linux.nu>
 *                         Patrick Schaaf <bof@bof.de>
 *                         Martin Josefsson <gandalf@wlug.westbo.se>
 * Copyright (C) 2003-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>

/* The protocol version */
#define IPSET_PROTOCOL		6

/* The max length of strings including NUL: set and type identifiers */
#define IPSET_MAXNAMELEN	32

/* Message types and commands */
enum ipset_cmd {
	IPSET_CMD_NONE,
	IPSET_CMD_PROTOCOL,	/* 1: Return protocol version */
	IPSET_CMD_CREATE,	/* 2: Create a new (empty) set */
	IPSET_CMD_DESTROY,	/* 3: Destroy a (empty) set */
	IPSET_CMD_FLUSH,	/* 4: Remove all elements from a set */
	IPSET_CMD_RENAME,	/* 5: Rename a set */
	IPSET_CMD_SWAP,		/* 6: Swap two sets */
	IPSET_CMD_LIST,		/* 7: List sets */
	IPSET_CMD_SAVE,		/* 8: Save sets */
	IPSET_CMD_ADD,		/* 9: Add an element to a set */
	IPSET_CMD_DEL,		/* 10: Delete an element from a set */
	IPSET_CMD_TEST,		/* 11: Test an element in a set */
	IPSET_CMD_HEADER,	/* 12: Get set header data only */
	IPSET_CMD_TYPE,		/* 13: Get set type */
	IPSET_MSG_MAX,		/* Netlink message commands */

	/* Commands in userspace: */
	IPSET_CMD_RESTORE = IPSET_MSG_MAX, /* 14: Enter restore mode */
	IPSET_CMD_HELP,		/* 15: Get help */
	IPSET_CMD_VERSION,	/* 16: Get program version */
	IPSET_CMD_QUIT,		/* 17: Quit from interactive mode */

	IPSET_CMD_MAX,

	IPSET_CMD_COMMIT = IPSET_CMD_MAX, /* 18: Commit buffered commands */
};

/* Attributes at command level */
enum {
	IPSET_ATTR_UNSPEC,
	IPSET_ATTR_PROTOCOL,	/* 1: Protocol version */
	IPSET_ATTR_SETNAME,	/* 2: Name of the set */
	IPSET_ATTR_TYPENAME,	/* 3: Typename */
	IPSET_ATTR_SETNAME2 = IPSET_ATTR_TYPENAME, /* Setname at rename/swap */
	IPSET_ATTR_REVISION,	/* 4: Settype revision */
	IPSET_ATTR_FAMILY,	/* 5: Settype family */
	IPSET_ATTR_FLAGS,	/* 6: Flags at command level */
	IPSET_ATTR_DATA,	/* 7: Nested attributes */
	IPSET_ATTR_ADT,		/* 8: Multiple data containers */
	IPSET_ATTR_LINENO,	/* 9: Restore lineno */
	IPSET_ATTR_PROTOCOL_MIN, /* 10: Minimal supported version number */
	IPSET_ATTR_REVISION_MIN	= IPSET_ATTR_PROTOCOL_MIN, /* type rev min */
	__IPSET_ATTR_CMD_MAX,
};
#define IPSET_ATTR_CMD_MAX	(__IPSET_ATTR_CMD_MAX - 1)

/* CADT specific attributes */
enum {
	IPSET_ATTR_IP = IPSET_ATTR_UNSPEC + 1,
	IPSET_ATTR_IP_FROM = IPSET_ATTR_IP,
	IPSET_ATTR_IP_TO,	/* 2 */
	IPSET_ATTR_CIDR,	/* 3 */
	IPSET_ATTR_PORT,	/* 4 */
	IPSET_ATTR_PORT_FROM = IPSET_ATTR_PORT,
	IPSET_ATTR_PORT_TO,	/* 5 */
	IPSET_ATTR_TIMEOUT,	/* 6 */
	IPSET_ATTR_PROTO,	/* 7 */
	IPSET_ATTR_CADT_FLAGS,	/* 8 */
	IPSET_ATTR_CADT_LINENO = IPSET_ATTR_LINENO,	/* 9 */
	/* Reserve empty slots */
	IPSET_ATTR_CADT_MAX = 16,
	/* Create-only specific attributes */
	IPSET_ATTR_GC,
	IPSET_ATTR_HASHSIZE,
	IPSET_ATTR_MAXELEM,
	IPSET_ATTR_NETMASK,
	IPSET_ATTR_PROBES,
	IPSET_ATTR_RESIZE,
	IPSET_ATTR_SIZE,
	/* Kernel-only */
	IPSET_ATTR_ELEMENTS,
	IPSET_ATTR_REFERENCES,
	IPSET_ATTR_MEMSIZE,

	__IPSET_ATTR_CREATE_MAX,
};
#define IPSET_ATTR_CREATE_MAX	(__IPSET_ATTR_CREATE_MAX - 1)

/* ADT specific attributes */
enum {
	IPSET_ATTR_ETHER = IPSET_ATTR_CADT_MAX + 1,
	IPSET_ATTR_NAME,
	IPSET_ATTR_NAMEREF,
	IPSET_ATTR_IP2,
	IPSET_ATTR_CIDR2,
	IPSET_ATTR_IP2_TO,
	IPSET_ATTR_IFACE,
	__IPSET_ATTR_ADT_MAX,
};
#define IPSET_ATTR_ADT_MAX	(__IPSET_ATTR_ADT_MAX - 1)

/* IP specific attributes */
enum {
	IPSET_ATTR_IPADDR_IPV4 = IPSET_ATTR_UNSPEC + 1,
	IPSET_ATTR_IPADDR_IPV6,
	__IPSET_ATTR_IPADDR_MAX,
};
#define IPSET_ATTR_IPADDR_MAX	(__IPSET_ATTR_IPADDR_MAX - 1)

/* Error codes */
enum ipset_errno {
	IPSET_ERR_PRIVATE = 4096,
	IPSET_ERR_PROTOCOL,
	IPSET_ERR_FIND_TYPE,
	IPSET_ERR_MAX_SETS,
	IPSET_ERR_BUSY,
	IPSET_ERR_EXIST_SETNAME2,
	IPSET_ERR_TYPE_MISMATCH,
	IPSET_ERR_EXIST,
	IPSET_ERR_INVALID_CIDR,
	IPSET_ERR_INVALID_NETMASK,
	IPSET_ERR_INVALID_FAMILY,
	IPSET_ERR_TIMEOUT,
	IPSET_ERR_REFERENCED,
	IPSET_ERR_IPADDR_IPV4,
	IPSET_ERR_IPADDR_IPV6,

	/* Type specific error codes */
	IPSET_ERR_TYPE_SPECIFIC = 4352,
};

/* Flags at command level */
enum ipset_cmd_flags {
	IPSET_FLAG_BIT_EXIST	= 0,
	IPSET_FLAG_EXIST	= (1 << IPSET_FLAG_BIT_EXIST),
	IPSET_FLAG_BIT_LIST_SETNAME = 1,
	IPSET_FLAG_LIST_SETNAME	= (1 << IPSET_FLAG_BIT_LIST_SETNAME),
	IPSET_FLAG_BIT_LIST_HEADER = 2,
	IPSET_FLAG_LIST_HEADER	= (1 << IPSET_FLAG_BIT_LIST_HEADER),
	IPSET_FLAG_CMD_MAX = 15,	/* Lower half */
};

/* Flags at CADT attribute level */
enum ipset_cadt_flags {
	IPSET_FLAG_BIT_BEFORE	= 0,
	IPSET_FLAG_BEFORE	= (1 << IPSET_FLAG_BIT_BEFORE),
	IPSET_FLAG_BIT_PHYSDEV	= 1,
	IPSET_FLAG_PHYSDEV	= (1 << IPSET_FLAG_BIT_PHYSDEV),
	IPSET_FLAG_BIT_NOMATCH	= 2,
	IPSET_FLAG_NOMATCH	= (1 << IPSET_FLAG_BIT_NOMATCH),
	IPSET_FLAG_CADT_MAX	= 15,	/* Upper half */
};

/* Commands with settype-specific attributes */
enum ipset_adt {
	IPSET_ADD,
	IPSET_DEL,
	IPSET_TEST,
	IPSET_ADT_MAX,
	IPSET_CREATE = IPSET_ADT_MAX,
	IPSET_CADT_MAX,
};

/* Sets are identified by an index in kernel space. Tweak with ip_set_id_t
 * and IPSET_INVALID_ID if you want to increase the max number of sets.
 */
typedef __u16 ip_set_id_t;

#define IPSET_INVALID_ID		65535

enum ip_set_dim {
	IPSET_DIM_ZERO = 0,
	IPSET_DIM_ONE,
	IPSET_DIM_TWO,
	IPSET_DIM_THREE,
	/* Max dimension in elements.
	 * If changed, new revision of iptables match/target is required.
	 */
	IPSET_DIM_MAX = 6,
};

/* Option flags for kernel operations */
enum ip_set_kopt {
	IPSET_INV_MATCH = (1 << IPSET_DIM_ZERO),
	IPSET_DIM_ONE_SRC = (1 << IPSET_DIM_ONE),
	IPSET_DIM_TWO_SRC = (1 << IPSET_DIM_TWO),
	IPSET_DIM_THREE_SRC = (1 << IPSET_DIM_THREE),
};

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2000-2002 Joakim Axelsson <gozem@linux.nu>
 *                         Patrick Schaaf <bof@bof.de>
 *                         Martin Josefsson <gandalf@wlug.westbo.se>
 * Copyright (C) 2003-2013 Jozsef Kadlecsik <kadlec@netfilter.org>
 */
#ifndef _IP_SET_H
#define _IP_SET_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>
#include <net/netlink.h>
=======
#include <linux/stringify.h>
#include <linux/vmalloc.h>
#include <net/netlink.h>
#include <uapi/linux/netfilter/ipset/ip_set.h>

#define _IP_SET_MODULE_DESC(a, b, c)		\
	MODULE_DESCRIPTION(a " type of IP sets, revisions " b "-" c)
#define IP_SET_MODULE_DESC(a, b, c)		\
	_IP_SET_MODULE_DESC(a, __stringify(b), __stringify(c))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Set features */
enum ip_set_feature {
	IPSET_TYPE_IP_FLAG = 0,
	IPSET_TYPE_IP = (1 << IPSET_TYPE_IP_FLAG),
	IPSET_TYPE_PORT_FLAG = 1,
	IPSET_TYPE_PORT = (1 << IPSET_TYPE_PORT_FLAG),
	IPSET_TYPE_MAC_FLAG = 2,
	IPSET_TYPE_MAC = (1 << IPSET_TYPE_MAC_FLAG),
	IPSET_TYPE_IP2_FLAG = 3,
	IPSET_TYPE_IP2 = (1 << IPSET_TYPE_IP2_FLAG),
	IPSET_TYPE_NAME_FLAG = 4,
	IPSET_TYPE_NAME = (1 << IPSET_TYPE_NAME_FLAG),
	IPSET_TYPE_IFACE_FLAG = 5,
	IPSET_TYPE_IFACE = (1 << IPSET_TYPE_IFACE_FLAG),
<<<<<<< HEAD
	/* Strictly speaking not a feature, but a flag for dumping:
	 * this settype must be dumped last */
	IPSET_DUMP_LAST_FLAG = 7,
	IPSET_DUMP_LAST = (1 << IPSET_DUMP_LAST_FLAG),
};

struct ip_set;

typedef int (*ipset_adtfn)(struct ip_set *set, void *value,
			   u32 timeout, u32 flags);
=======
	IPSET_TYPE_MARK_FLAG = 6,
	IPSET_TYPE_MARK = (1 << IPSET_TYPE_MARK_FLAG),
	IPSET_TYPE_NOMATCH_FLAG = 7,
	IPSET_TYPE_NOMATCH = (1 << IPSET_TYPE_NOMATCH_FLAG),
	/* Strictly speaking not a feature, but a flag for dumping:
	 * this settype must be dumped last */
	IPSET_DUMP_LAST_FLAG = 8,
	IPSET_DUMP_LAST = (1 << IPSET_DUMP_LAST_FLAG),
};

/* Set extensions */
enum ip_set_extension {
	IPSET_EXT_BIT_TIMEOUT = 0,
	IPSET_EXT_TIMEOUT = (1 << IPSET_EXT_BIT_TIMEOUT),
	IPSET_EXT_BIT_COUNTER = 1,
	IPSET_EXT_COUNTER = (1 << IPSET_EXT_BIT_COUNTER),
	IPSET_EXT_BIT_COMMENT = 2,
	IPSET_EXT_COMMENT = (1 << IPSET_EXT_BIT_COMMENT),
	IPSET_EXT_BIT_SKBINFO = 3,
	IPSET_EXT_SKBINFO = (1 << IPSET_EXT_BIT_SKBINFO),
	/* Mark set with an extension which needs to call destroy */
	IPSET_EXT_BIT_DESTROY = 7,
	IPSET_EXT_DESTROY = (1 << IPSET_EXT_BIT_DESTROY),
};

#define SET_WITH_TIMEOUT(s)	((s)->extensions & IPSET_EXT_TIMEOUT)
#define SET_WITH_COUNTER(s)	((s)->extensions & IPSET_EXT_COUNTER)
#define SET_WITH_COMMENT(s)	((s)->extensions & IPSET_EXT_COMMENT)
#define SET_WITH_SKBINFO(s)	((s)->extensions & IPSET_EXT_SKBINFO)
#define SET_WITH_FORCEADD(s)	((s)->flags & IPSET_CREATE_FLAG_FORCEADD)

/* Extension id, in size order */
enum ip_set_ext_id {
	IPSET_EXT_ID_COUNTER = 0,
	IPSET_EXT_ID_TIMEOUT,
	IPSET_EXT_ID_SKBINFO,
	IPSET_EXT_ID_COMMENT,
	IPSET_EXT_ID_MAX,
};

struct ip_set;

/* Extension type */
struct ip_set_ext_type {
	/* Destroy extension private data (can be NULL) */
	void (*destroy)(struct ip_set *set, void *ext);
	enum ip_set_extension type;
	enum ipset_cadt_flags flag;
	/* Size and minimal alignment */
	u8 len;
	u8 align;
};

extern const struct ip_set_ext_type ip_set_extensions[];

struct ip_set_counter {
	atomic64_t bytes;
	atomic64_t packets;
};

struct ip_set_comment_rcu {
	struct rcu_head rcu;
	char str[];
};

struct ip_set_comment {
	struct ip_set_comment_rcu __rcu *c;
};

struct ip_set_skbinfo {
	u32 skbmark;
	u32 skbmarkmask;
	u32 skbprio;
	u16 skbqueue;
	u16 __pad;
};

struct ip_set_ext {
	struct ip_set_skbinfo skbinfo;
	u64 packets;
	u64 bytes;
	char *comment;
	u32 timeout;
	u8 packets_op;
	u8 bytes_op;
	bool target;
};

#define ext_timeout(e, s)	\
((unsigned long *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_TIMEOUT]))
#define ext_counter(e, s)	\
((struct ip_set_counter *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_COUNTER]))
#define ext_comment(e, s)	\
((struct ip_set_comment *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_COMMENT]))
#define ext_skbinfo(e, s)	\
((struct ip_set_skbinfo *)(((void *)(e)) + (s)->offset[IPSET_EXT_ID_SKBINFO]))

typedef int (*ipset_adtfn)(struct ip_set *set, void *value,
			   const struct ip_set_ext *ext,
			   struct ip_set_ext *mext, u32 cmdflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Kernel API function options */
struct ip_set_adt_opt {
	u8 family;		/* Actual protocol family */
	u8 dim;			/* Dimension of match/target */
	u8 flags;		/* Direction and negation flags */
	u32 cmdflags;		/* Command-like flags */
<<<<<<< HEAD
	u32 timeout;		/* Timeout value */
=======
	struct ip_set_ext ext;	/* Extensions */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Set type, variant-specific part */
struct ip_set_type_variant {
	/* Kernelspace: test/add/del entries
	 *		returns negative error code,
	 *			zero for no match/success to add/delete
	 *			positive for matching element */
<<<<<<< HEAD
	int (*kadt)(struct ip_set *set, const struct sk_buff * skb,
		    const struct xt_action_param *par,
		    enum ipset_adt adt, const struct ip_set_adt_opt *opt);
=======
	int (*kadt)(struct ip_set *set, const struct sk_buff *skb,
		    const struct xt_action_param *par,
		    enum ipset_adt adt, struct ip_set_adt_opt *opt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Userspace: test/add/del entries
	 *		returns negative error code,
	 *			zero for no match/success to add/delete
	 *			positive for matching element */
	int (*uadt)(struct ip_set *set, struct nlattr *tb[],
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried);

	/* Low level add/del/test functions */
	ipset_adtfn adt[IPSET_ADT_MAX];

	/* When adding entries and set is full, try to resize the set */
	int (*resize)(struct ip_set *set, bool retried);
	/* Destroy the set */
	void (*destroy)(struct ip_set *set);
	/* Flush the elements */
	void (*flush)(struct ip_set *set);
	/* Expire entries before listing */
	void (*expire)(struct ip_set *set);
	/* List set header data */
	int (*head)(struct ip_set *set, struct sk_buff *skb);
	/* List elements */
	int (*list)(const struct ip_set *set, struct sk_buff *skb,
		    struct netlink_callback *cb);
<<<<<<< HEAD
=======
	/* Keep listing private when resizing runs parallel */
	void (*uref)(struct ip_set *set, struct netlink_callback *cb,
		     bool start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Return true if "b" set is the same as "a"
	 * according to the create set parameters */
	bool (*same_set)(const struct ip_set *a, const struct ip_set *b);
<<<<<<< HEAD
};

=======
	/* Cancel ongoing garbage collectors before destroying the set*/
	void (*cancel_gc)(struct ip_set *set);
	/* Region-locking is used */
	bool region_lock;
};

struct ip_set_region {
	spinlock_t lock;	/* Region lock */
	size_t ext_size;	/* Size of the dynamic extensions */
	u32 elements;		/* Number of elements vs timeout */
};

/* Max range where every element is added/deleted in one step */
#define IPSET_MAX_RANGE		(1<<14)

/* The max revision number supported by any set type + 1 */
#define IPSET_REVISION_MAX	9

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* The core set type structure */
struct ip_set_type {
	struct list_head list;

	/* Typename */
	char name[IPSET_MAXNAMELEN];
	/* Protocol version */
	u8 protocol;
<<<<<<< HEAD
	/* Set features to control swapping */
	u8 features;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Set type dimension */
	u8 dimension;
	/*
	 * Supported family: may be NFPROTO_UNSPEC for both
	 * NFPROTO_IPV4/NFPROTO_IPV6.
	 */
	u8 family;
	/* Type revisions */
	u8 revision_min, revision_max;
<<<<<<< HEAD

	/* Create set */
	int (*create)(struct ip_set *set, struct nlattr *tb[], u32 flags);
=======
	/* Revision-specific supported (create) flags */
	u8 create_flags[IPSET_REVISION_MAX+1];
	/* Set features to control swapping */
	u16 features;

	/* Create set */
	int (*create)(struct net *net, struct ip_set *set,
		      struct nlattr *tb[], u32 flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Attribute policies */
	const struct nla_policy create_policy[IPSET_ATTR_CREATE_MAX + 1];
	const struct nla_policy adt_policy[IPSET_ATTR_ADT_MAX + 1];

	/* Set this to THIS_MODULE if you are a module, otherwise NULL */
	struct module *me;
};

/* register and unregister set type */
extern int ip_set_type_register(struct ip_set_type *set_type);
extern void ip_set_type_unregister(struct ip_set_type *set_type);

/* A generic IP set */
struct ip_set {
<<<<<<< HEAD
	/* The name of the set */
	char name[IPSET_MAXNAMELEN];
	/* Lock protecting the set data */
	rwlock_t lock;
	/* References to the set */
	u32 ref;
=======
	/* For call_cru in destroy */
	struct rcu_head rcu;
	/* The name of the set */
	char name[IPSET_MAXNAMELEN];
	/* Lock protecting the set data */
	spinlock_t lock;
	/* References to the set */
	u32 ref;
	/* References to the set for netlink events like dump,
	 * ref can be swapped out by ip_set_swap
	 */
	u32 ref_netlink;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The core set type */
	struct ip_set_type *type;
	/* The type variant doing the real job */
	const struct ip_set_type_variant *variant;
	/* The actual INET family of the set */
	u8 family;
	/* The type revision */
	u8 revision;
<<<<<<< HEAD
=======
	/* Extensions */
	u8 extensions;
	/* Create flags */
	u8 flags;
	/* Default timeout value, if enabled */
	u32 timeout;
	/* Number of elements (vs timeout) */
	u32 elements;
	/* Size of the dynamic extensions (vs timeout) */
	size_t ext_size;
	/* Element data size */
	size_t dsize;
	/* Offsets to extensions in elements */
	size_t offset[IPSET_EXT_ID_MAX];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The type specific data */
	void *data;
};

<<<<<<< HEAD
/* register and unregister set references */
extern ip_set_id_t ip_set_get_byname(const char *name, struct ip_set **set);
extern void ip_set_put_byindex(ip_set_id_t index);
extern const char *ip_set_name_byindex(ip_set_id_t index);
extern ip_set_id_t ip_set_nfnl_get(const char *name);
extern ip_set_id_t ip_set_nfnl_get_byindex(ip_set_id_t index);
extern void ip_set_nfnl_put(ip_set_id_t index);
=======
static inline void
ip_set_ext_destroy(struct ip_set *set, void *data)
{
	/* Check that the extension is enabled for the set and
	 * call it's destroy function for its extension part in data.
	 */
	if (SET_WITH_COMMENT(set)) {
		struct ip_set_comment *c = ext_comment(data, set);

		ip_set_extensions[IPSET_EXT_ID_COMMENT].destroy(set, c);
	}
}

int ip_set_put_flags(struct sk_buff *skb, struct ip_set *set);

/* Netlink CB args */
enum {
	IPSET_CB_NET = 0,	/* net namespace */
	IPSET_CB_PROTO,		/* ipset protocol */
	IPSET_CB_DUMP,		/* dump single set/all sets */
	IPSET_CB_INDEX,		/* set index */
	IPSET_CB_PRIVATE,	/* set private data */
	IPSET_CB_ARG0,		/* type specific */
};

/* register and unregister set references */
extern ip_set_id_t ip_set_get_byname(struct net *net,
				     const char *name, struct ip_set **set);
extern void ip_set_put_byindex(struct net *net, ip_set_id_t index);
extern void ip_set_name_byindex(struct net *net, ip_set_id_t index, char *name);
extern ip_set_id_t ip_set_nfnl_get_byindex(struct net *net, ip_set_id_t index);
extern void ip_set_nfnl_put(struct net *net, ip_set_id_t index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* API for iptables set match, and SET target */

extern int ip_set_add(ip_set_id_t id, const struct sk_buff *skb,
		      const struct xt_action_param *par,
<<<<<<< HEAD
		      const struct ip_set_adt_opt *opt);
extern int ip_set_del(ip_set_id_t id, const struct sk_buff *skb,
		      const struct xt_action_param *par,
		      const struct ip_set_adt_opt *opt);
extern int ip_set_test(ip_set_id_t id, const struct sk_buff *skb,
		       const struct xt_action_param *par,
		       const struct ip_set_adt_opt *opt);
=======
		      struct ip_set_adt_opt *opt);
extern int ip_set_del(ip_set_id_t id, const struct sk_buff *skb,
		      const struct xt_action_param *par,
		      struct ip_set_adt_opt *opt);
extern int ip_set_test(ip_set_id_t id, const struct sk_buff *skb,
		       const struct xt_action_param *par,
		       struct ip_set_adt_opt *opt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Utility functions */
extern void *ip_set_alloc(size_t size);
extern void ip_set_free(void *members);
extern int ip_set_get_ipaddr4(struct nlattr *nla,  __be32 *ipaddr);
extern int ip_set_get_ipaddr6(struct nlattr *nla, union nf_inet_addr *ipaddr);
<<<<<<< HEAD
=======
extern size_t ip_set_elem_len(struct ip_set *set, struct nlattr *tb[],
			      size_t len, size_t align);
extern int ip_set_get_extensions(struct ip_set *set, struct nlattr *tb[],
				 struct ip_set_ext *ext);
extern int ip_set_put_extensions(struct sk_buff *skb, const struct ip_set *set,
				 const void *e, bool active);
extern bool ip_set_match_extensions(struct ip_set *set,
				    const struct ip_set_ext *ext,
				    struct ip_set_ext *mext,
				    u32 flags, void *data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int
ip_set_get_hostipaddr4(struct nlattr *nla, u32 *ipaddr)
{
	__be32 ip;
	int ret = ip_set_get_ipaddr4(nla, &ip);

	if (ret)
		return ret;
	*ipaddr = ntohl(ip);
	return 0;
}

/* Ignore IPSET_ERR_EXIST errors if asked to do so? */
static inline bool
ip_set_eexist(int ret, u32 flags)
{
	return ret == -IPSET_ERR_EXIST && (flags & IPSET_FLAG_EXIST);
}

<<<<<<< HEAD
=======
/* Match elements marked with nomatch */
static inline bool
ip_set_enomatch(int ret, u32 flags, enum ipset_adt adt, struct ip_set *set)
{
	return adt == IPSET_TEST &&
	       (set->type->features & IPSET_TYPE_NOMATCH) &&
	       ((flags >> 16) & IPSET_FLAG_NOMATCH) &&
	       (ret > 0 || ret == -ENOTEMPTY);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Check the NLA_F_NET_BYTEORDER flag */
static inline bool
ip_set_attr_netorder(struct nlattr *tb[], int type)
{
	return tb[type] && (tb[type]->nla_type & NLA_F_NET_BYTEORDER);
}

static inline bool
ip_set_optattr_netorder(struct nlattr *tb[], int type)
{
	return !tb[type] || (tb[type]->nla_type & NLA_F_NET_BYTEORDER);
}

/* Useful converters */
static inline u32
ip_set_get_h32(const struct nlattr *attr)
{
	return ntohl(nla_get_be32(attr));
}

static inline u16
ip_set_get_h16(const struct nlattr *attr)
{
	return ntohs(nla_get_be16(attr));
}

<<<<<<< HEAD
#define ipset_nest_start(skb, attr) nla_nest_start(skb, attr | NLA_F_NESTED)
#define ipset_nest_end(skb, start)  nla_nest_end(skb, start)

#define NLA_PUT_IPADDR4(skb, type, ipaddr)			\
do {								\
	struct nlattr *__nested = ipset_nest_start(skb, type);	\
								\
	if (!__nested)						\
		goto nla_put_failure;				\
	NLA_PUT_NET32(skb, IPSET_ATTR_IPADDR_IPV4, ipaddr);	\
	ipset_nest_end(skb, __nested);				\
} while (0)

#define NLA_PUT_IPADDR6(skb, type, ipaddrptr)			\
do {								\
	struct nlattr *__nested = ipset_nest_start(skb, type);	\
								\
	if (!__nested)						\
		goto nla_put_failure;				\
	NLA_PUT(skb, IPSET_ATTR_IPADDR_IPV6,			\
		sizeof(struct in6_addr), ipaddrptr);		\
	ipset_nest_end(skb, __nested);				\
} while (0)
=======
static inline int nla_put_ipaddr4(struct sk_buff *skb, int type, __be32 ipaddr)
{
	struct nlattr *__nested = nla_nest_start(skb, type);
	int ret;

	if (!__nested)
		return -EMSGSIZE;
	ret = nla_put_in_addr(skb, IPSET_ATTR_IPADDR_IPV4, ipaddr);
	if (!ret)
		nla_nest_end(skb, __nested);
	return ret;
}

static inline int nla_put_ipaddr6(struct sk_buff *skb, int type,
				  const struct in6_addr *ipaddrptr)
{
	struct nlattr *__nested = nla_nest_start(skb, type);
	int ret;

	if (!__nested)
		return -EMSGSIZE;
	ret = nla_put_in6_addr(skb, IPSET_ATTR_IPADDR_IPV6, ipaddrptr);
	if (!ret)
		nla_nest_end(skb, __nested);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Get address from skbuff */
static inline __be32
ip4addr(const struct sk_buff *skb, bool src)
{
	return src ? ip_hdr(skb)->saddr : ip_hdr(skb)->daddr;
}

static inline void
ip4addrptr(const struct sk_buff *skb, bool src, __be32 *addr)
{
	*addr = src ? ip_hdr(skb)->saddr : ip_hdr(skb)->daddr;
}

static inline void
ip6addrptr(const struct sk_buff *skb, bool src, struct in6_addr *addr)
{
	memcpy(addr, src ? &ipv6_hdr(skb)->saddr : &ipv6_hdr(skb)->daddr,
	       sizeof(*addr));
}

<<<<<<< HEAD
/* Calculate the bytes required to store the inclusive range of a-b */
static inline int
bitmap_bytes(u32 a, u32 b)
{
	return 4 * ((((b - a + 8) / 8) + 3) / 4);
}

#endif /* __KERNEL__ */

/* Interface to iptables/ip6tables */

#define SO_IP_SET		83

union ip_set_name_index {
	char name[IPSET_MAXNAMELEN];
	ip_set_id_t index;
};

#define IP_SET_OP_GET_BYNAME	0x00000006	/* Get set index by name */
struct ip_set_req_get_set {
	unsigned op;
	unsigned version;
	union ip_set_name_index set;
};

#define IP_SET_OP_GET_BYINDEX	0x00000007	/* Get set name by index */
/* Uses ip_set_req_get_set */

#define IP_SET_OP_VERSION	0x00000100	/* Ask kernel version */
struct ip_set_req_version {
	unsigned op;
	unsigned version;
};
=======
/* How often should the gc be run by default */
#define IPSET_GC_TIME			(3 * 60)

/* Timeout period depending on the timeout value of the given set */
#define IPSET_GC_PERIOD(timeout) \
	((timeout/3) ? min_t(u32, (timeout)/3, IPSET_GC_TIME) : 1)

/* Entry is set with no timeout value */
#define IPSET_ELEM_PERMANENT	0

/* Set is defined with timeout support: timeout value may be 0 */
#define IPSET_NO_TIMEOUT	UINT_MAX

/* Max timeout value, see msecs_to_jiffies() in jiffies.h */
#define IPSET_MAX_TIMEOUT	(UINT_MAX >> 1)/MSEC_PER_SEC

#define ip_set_adt_opt_timeout(opt, set)	\
((opt)->ext.timeout != IPSET_NO_TIMEOUT ? (opt)->ext.timeout : (set)->timeout)

static inline unsigned int
ip_set_timeout_uget(struct nlattr *tb)
{
	unsigned int timeout = ip_set_get_h32(tb);

	/* Normalize to fit into jiffies */
	if (timeout > IPSET_MAX_TIMEOUT)
		timeout = IPSET_MAX_TIMEOUT;

	return timeout;
}

static inline bool
ip_set_timeout_expired(const unsigned long *t)
{
	return *t != IPSET_ELEM_PERMANENT && time_is_before_jiffies(*t);
}

static inline void
ip_set_timeout_set(unsigned long *timeout, u32 value)
{
	unsigned long t;

	if (!value) {
		*timeout = IPSET_ELEM_PERMANENT;
		return;
	}

	t = msecs_to_jiffies(value * MSEC_PER_SEC) + jiffies;
	if (t == IPSET_ELEM_PERMANENT)
		/* Bingo! :-) */
		t--;
	*timeout = t;
}

void ip_set_init_comment(struct ip_set *set, struct ip_set_comment *comment,
			 const struct ip_set_ext *ext);

static inline void
ip_set_init_counter(struct ip_set_counter *counter,
		    const struct ip_set_ext *ext)
{
	if (ext->bytes != ULLONG_MAX)
		atomic64_set(&(counter)->bytes, (long long)(ext->bytes));
	if (ext->packets != ULLONG_MAX)
		atomic64_set(&(counter)->packets, (long long)(ext->packets));
}

static inline void
ip_set_init_skbinfo(struct ip_set_skbinfo *skbinfo,
		    const struct ip_set_ext *ext)
{
	*skbinfo = ext->skbinfo;
}

static inline void
nf_inet_addr_mask_inplace(union nf_inet_addr *a1,
			  const union nf_inet_addr *mask)
{
	a1->all[0] &= mask->all[0];
	a1->all[1] &= mask->all[1];
	a1->all[2] &= mask->all[2];
	a1->all[3] &= mask->all[3];
}

#define IP_SET_INIT_KEXT(skb, opt, set)			\
	{ .bytes = (skb)->len, .packets = 1, .target = true,\
	  .timeout = ip_set_adt_opt_timeout(opt, set) }

#define IP_SET_INIT_UEXT(set)				\
	{ .bytes = ULLONG_MAX, .packets = ULLONG_MAX,	\
	  .timeout = (set)->timeout }

#define IPSET_CONCAT(a, b)		a##b
#define IPSET_TOKEN(a, b)		IPSET_CONCAT(a, b)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /*_IP_SET_H */
