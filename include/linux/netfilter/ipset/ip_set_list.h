<<<<<<< HEAD
#ifndef __IP_SET_LIST_H
#define __IP_SET_LIST_H

/* List type specific error codes */
enum {
	/* Set name to be added/deleted/tested does not exist. */
	IPSET_ERR_NAME = IPSET_ERR_TYPE_SPECIFIC,
	/* list:set type is not permitted to add */
	IPSET_ERR_LOOP,
	/* Missing reference set */
	IPSET_ERR_BEFORE,
	/* Reference set does not exist */
	IPSET_ERR_NAMEREF,
	/* Set is full */
	IPSET_ERR_LIST_FULL,
	/* Reference set is not added to the set */
	IPSET_ERR_REF_EXIST,
};

#ifdef __KERNEL__

#define IP_SET_LIST_DEFAULT_SIZE	8
#define IP_SET_LIST_MIN_SIZE		4

#endif /* __KERNEL__ */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __IP_SET_LIST_H
#define __IP_SET_LIST_H

#include <uapi/linux/netfilter/ipset/ip_set_list.h>


#define IP_SET_LIST_DEFAULT_SIZE	8
#define IP_SET_LIST_MIN_SIZE		4
#define IP_SET_LIST_MAX_SIZE		65536
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __IP_SET_LIST_H */
