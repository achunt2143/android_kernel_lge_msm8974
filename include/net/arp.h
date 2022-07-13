<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* linux/net/inet/arp.h */
#ifndef _ARP_H
#define _ARP_H

#include <linux/if_arp.h>
<<<<<<< HEAD
=======
#include <linux/hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/neighbour.h>


extern struct neigh_table arp_tbl;

<<<<<<< HEAD
static inline u32 arp_hashfn(u32 key, const struct net_device *dev, u32 hash_rnd)
{
	u32 val = key ^ dev->ifindex;

	return val * hash_rnd;
}

static inline struct neighbour *__ipv4_neigh_lookup(struct net_device *dev, u32 key)
{
	struct neigh_hash_table *nht;
	struct neighbour *n;
	u32 hash_val;

	rcu_read_lock_bh();
	nht = rcu_dereference_bh(arp_tbl.nht);
	hash_val = arp_hashfn(key, dev, nht->hash_rnd[0]) >> (32 - nht->hash_shift);
	for (n = rcu_dereference_bh(nht->hash_buckets[hash_val]);
	     n != NULL;
	     n = rcu_dereference_bh(n->next)) {
		if (n->dev == dev && *(u32 *)n->primary_key == key) {
			if (!atomic_inc_not_zero(&n->refcnt))
				n = NULL;
			break;
		}
	}
	rcu_read_unlock_bh();
=======
static inline u32 arp_hashfn(const void *pkey, const struct net_device *dev, u32 *hash_rnd)
{
	u32 key = *(const u32 *)pkey;
	u32 val = key ^ hash32_ptr(dev);

	return val * hash_rnd[0];
}

#ifdef CONFIG_INET
static inline struct neighbour *__ipv4_neigh_lookup_noref(struct net_device *dev, u32 key)
{
	if (dev->flags & (IFF_LOOPBACK | IFF_POINTOPOINT))
		key = INADDR_ANY;

	return ___neigh_lookup_noref(&arp_tbl, neigh_key_eq32, arp_hashfn, &key, dev);
}
#else
static inline
struct neighbour *__ipv4_neigh_lookup_noref(struct net_device *dev, u32 key)
{
	return NULL;
}
#endif

static inline struct neighbour *__ipv4_neigh_lookup(struct net_device *dev, u32 key)
{
	struct neighbour *n;

	rcu_read_lock();
	n = __ipv4_neigh_lookup_noref(dev, key);
	if (n && !refcount_inc_not_zero(&n->refcnt))
		n = NULL;
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return n;
}

<<<<<<< HEAD
extern void	arp_init(void);
extern int	arp_find(unsigned char *haddr, struct sk_buff *skb);
extern int	arp_ioctl(struct net *net, unsigned int cmd, void __user *arg);
extern void     arp_send(int type, int ptype, __be32 dest_ip,
			 struct net_device *dev, __be32 src_ip,
			 const unsigned char *dest_hw,
			 const unsigned char *src_hw, const unsigned char *th);
extern int	arp_mc_map(__be32 addr, u8 *haddr, struct net_device *dev, int dir);
extern void	arp_ifdown(struct net_device *dev);

extern struct sk_buff *arp_create(int type, int ptype, __be32 dest_ip,
				  struct net_device *dev, __be32 src_ip,
				  const unsigned char *dest_hw,
				  const unsigned char *src_hw,
				  const unsigned char *target_hw);
extern void arp_xmit(struct sk_buff *skb);
int arp_invalidate(struct net_device *dev, __be32 ip);
=======
static inline void __ipv4_confirm_neigh(struct net_device *dev, u32 key)
{
	struct neighbour *n;

	rcu_read_lock();
	n = __ipv4_neigh_lookup_noref(dev, key);
	neigh_confirm(n);
	rcu_read_unlock();
}

void arp_init(void);
int arp_ioctl(struct net *net, unsigned int cmd, void __user *arg);
void arp_send(int type, int ptype, __be32 dest_ip,
	      struct net_device *dev, __be32 src_ip,
	      const unsigned char *dest_hw,
	      const unsigned char *src_hw, const unsigned char *th);
int arp_mc_map(__be32 addr, u8 *haddr, struct net_device *dev, int dir);
void arp_ifdown(struct net_device *dev);
int arp_invalidate(struct net_device *dev, __be32 ip, bool force);

struct sk_buff *arp_create(int type, int ptype, __be32 dest_ip,
			   struct net_device *dev, __be32 src_ip,
			   const unsigned char *dest_hw,
			   const unsigned char *src_hw,
			   const unsigned char *target_hw);
void arp_xmit(struct sk_buff *skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* _ARP_H */
