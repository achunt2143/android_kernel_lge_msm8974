<<<<<<< HEAD
/*
 *  drivers/s390/net/qeth_l3.h
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Copyright IBM Corp. 2007
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#ifndef __QETH_L3_H__
#define __QETH_L3_H__

#include "qeth_core.h"
<<<<<<< HEAD

#define QETH_SNIFF_AVAIL	0x0008

struct qeth_ipaddr {
	struct list_head entry;
	enum qeth_ip_types type;
	enum qeth_ipa_setdelip_flags set_flags;
	enum qeth_ipa_setdelip_flags del_flags;
	int is_multicast;
	int users;
	enum qeth_prot_versions proto;
	unsigned char mac[OSA_ADDR_LEN];
	union {
		struct {
			unsigned int addr;
			unsigned int mask;
=======
#include <linux/hashtable.h>

enum qeth_ip_types {
	QETH_IP_TYPE_NORMAL,
	QETH_IP_TYPE_VIPA,
	QETH_IP_TYPE_RXIP,
};

struct qeth_ipaddr {
	struct hlist_node hnode;
	enum qeth_ip_types type;
	u8 is_multicast:1;
	u8 disp_flag:2;
	u8 ipato:1;			/* ucast only */

	/* is changed only for normal ip addresses
	 * for non-normal addresses it always is  1
	 */
	int  ref_counter;
	enum qeth_prot_versions proto;
	union {
		struct {
			__be32 addr;
			__be32 mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} a4;
		struct {
			struct in6_addr addr;
			unsigned int pfxlen;
		} a6;
	} u;
};

<<<<<<< HEAD
=======
static inline void qeth_l3_init_ipaddr(struct qeth_ipaddr *addr,
				       enum qeth_ip_types type,
				       enum qeth_prot_versions proto)
{
	memset(addr, 0, sizeof(*addr));
	addr->type = type;
	addr->proto = proto;
	addr->disp_flag = QETH_DISP_ADDR_DO_NOTHING;
	addr->ref_counter = 1;
}

static inline bool qeth_l3_addr_match_ip(struct qeth_ipaddr *a1,
					 struct qeth_ipaddr *a2)
{
	if (a1->proto != a2->proto)
		return false;
	if (a1->proto == QETH_PROT_IPV6)
		return ipv6_addr_equal(&a1->u.a6.addr, &a2->u.a6.addr);
	return a1->u.a4.addr == a2->u.a4.addr;
}

static inline bool qeth_l3_addr_match_all(struct qeth_ipaddr *a1,
					  struct qeth_ipaddr *a2)
{
	/* Assumes that the pair was obtained via qeth_l3_addr_find_by_ip(),
	 * so 'proto' and 'addr' match for sure.
	 *
	 * For ucast:
	 * -	'mask'/'pfxlen' for RXIP/VIPA is always 0. For NORMAL, matching
	 *	values are required to avoid mixups in takeover eligibility.
	 *
	 * For mcast,
	 * -	'mask'/'pfxlen' is always 0.
	 */
	if (a1->type != a2->type)
		return false;
	if (a1->proto == QETH_PROT_IPV6)
		return a1->u.a6.pfxlen == a2->u.a6.pfxlen;
	return a1->u.a4.mask == a2->u.a4.mask;
}

static inline u32 qeth_l3_ipaddr_hash(struct qeth_ipaddr *addr)
{
	if (addr->proto == QETH_PROT_IPV6)
		return ipv6_addr_hash(&addr->u.a6.addr);
	else
		return ipv4_addr_hash(addr->u.a4.addr);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct qeth_ipato_entry {
	struct list_head entry;
	enum qeth_prot_versions proto;
	char addr[16];
<<<<<<< HEAD
	int mask_bits;
};


void qeth_l3_ipaddr4_to_string(const __u8 *, char *);
int qeth_l3_string_to_ipaddr4(const char *, __u8 *);
void qeth_l3_ipaddr6_to_string(const __u8 *, char *);
int qeth_l3_string_to_ipaddr6(const char *, __u8 *);
void qeth_l3_ipaddr_to_string(enum qeth_prot_versions, const __u8 *, char *);
int qeth_l3_string_to_ipaddr(const char *, enum qeth_prot_versions, __u8 *);
int qeth_l3_create_device_attributes(struct device *);
void qeth_l3_remove_device_attributes(struct device *);
int qeth_l3_setrouting_v4(struct qeth_card *);
int qeth_l3_setrouting_v6(struct qeth_card *);
int qeth_l3_add_ipato_entry(struct qeth_card *, struct qeth_ipato_entry *);
void qeth_l3_del_ipato_entry(struct qeth_card *, enum qeth_prot_versions,
			u8 *, int);
int qeth_l3_add_vipa(struct qeth_card *, enum qeth_prot_versions, const u8 *);
void qeth_l3_del_vipa(struct qeth_card *, enum qeth_prot_versions, const u8 *);
int qeth_l3_add_rxip(struct qeth_card *, enum qeth_prot_versions, const u8 *);
void qeth_l3_del_rxip(struct qeth_card *card, enum qeth_prot_versions,
			const u8 *);
int qeth_l3_is_addr_covered_by_ipato(struct qeth_card *, struct qeth_ipaddr *);
struct qeth_ipaddr *qeth_l3_get_addr_buffer(enum qeth_prot_versions);
int qeth_l3_add_ip(struct qeth_card *, struct qeth_ipaddr *);
int qeth_l3_delete_ip(struct qeth_card *, struct qeth_ipaddr *);
void qeth_l3_set_ip_addr_list(struct qeth_card *);
=======
	unsigned int mask_bits;
};

extern const struct attribute_group *qeth_l3_attr_groups[];

int qeth_l3_ipaddr_to_string(enum qeth_prot_versions proto, const u8 *addr,
			     char *buf);
int qeth_l3_setrouting_v4(struct qeth_card *);
int qeth_l3_setrouting_v6(struct qeth_card *);
int qeth_l3_add_ipato_entry(struct qeth_card *, struct qeth_ipato_entry *);
int qeth_l3_del_ipato_entry(struct qeth_card *card,
			    enum qeth_prot_versions proto, u8 *addr,
			    unsigned int mask_bits);
void qeth_l3_update_ipato(struct qeth_card *card);
int qeth_l3_modify_hsuid(struct qeth_card *card, bool add);
int qeth_l3_modify_rxip_vipa(struct qeth_card *card, bool add, const u8 *ip,
			     enum qeth_ip_types type,
			     enum qeth_prot_versions proto);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __QETH_L3_H__ */
