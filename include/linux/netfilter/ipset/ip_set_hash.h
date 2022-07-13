<<<<<<< HEAD
#ifndef __IP_SET_HASH_H
#define __IP_SET_HASH_H

/* Hash type specific error codes */
enum {
	/* Hash is full */
	IPSET_ERR_HASH_FULL = IPSET_ERR_TYPE_SPECIFIC,
	/* Null-valued element */
	IPSET_ERR_HASH_ELEM,
	/* Invalid protocol */
	IPSET_ERR_INVALID_PROTO,
	/* Protocol missing but must be specified */
	IPSET_ERR_MISSING_PROTO,
	/* Range not supported */
	IPSET_ERR_HASH_RANGE_UNSUPPORTED,
	/* Invalid range */
	IPSET_ERR_HASH_RANGE,
};

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __IP_SET_HASH_H
#define __IP_SET_HASH_H

#include <uapi/linux/netfilter/ipset/ip_set_hash.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define IPSET_DEFAULT_HASHSIZE		1024
#define IPSET_MIMINAL_HASHSIZE		64
#define IPSET_DEFAULT_MAXELEM		65536
#define IPSET_DEFAULT_PROBES		4
#define IPSET_DEFAULT_RESIZE		100

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __IP_SET_HASH_H */
