<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __BEN_VLAN_802_1Q_INC__
#define __BEN_VLAN_802_1Q_INC__

#include <linux/if_vlan.h>
#include <linux/u64_stats_sync.h>
#include <linux/list.h>

<<<<<<< HEAD

/**
 *	struct vlan_priority_tci_mapping - vlan egress priority mappings
 *	@priority: skb priority
 *	@vlan_qos: vlan priority: (skb->priority << 13) & 0xE000
 *	@next: pointer to next struct
 */
struct vlan_priority_tci_mapping {
	u32					priority;
	u16					vlan_qos;
	struct vlan_priority_tci_mapping	*next;
};


/**
 *	struct vlan_pcpu_stats - VLAN percpu rx/tx stats
 *	@rx_packets: number of received packets
 *	@rx_bytes: number of received bytes
 *	@rx_multicast: number of received multicast packets
 *	@tx_packets: number of transmitted packets
 *	@tx_bytes: number of transmitted bytes
 *	@syncp: synchronization point for 64bit counters
 *	@rx_errors: number of rx errors
 *	@tx_dropped: number of tx drops
 */
struct vlan_pcpu_stats {
	u64			rx_packets;
	u64			rx_bytes;
	u64			rx_multicast;
	u64			tx_packets;
	u64			tx_bytes;
	struct u64_stats_sync	syncp;
	u32			rx_errors;
	u32			tx_dropped;
};

struct netpoll;

/**
 *	struct vlan_dev_priv - VLAN private device data
 *	@nr_ingress_mappings: number of ingress priority mappings
 *	@ingress_priority_map: ingress priority mappings
 *	@nr_egress_mappings: number of egress priority mappings
 *	@egress_priority_map: hash of egress priority mappings
 *	@vlan_id: VLAN identifier
 *	@flags: device flags
 *	@real_dev: underlying netdevice
 *	@real_dev_addr: address of underlying netdevice
 *	@dent: proc dir entry
 *	@vlan_pcpu_stats: ptr to percpu rx stats
 */
struct vlan_dev_priv {
	unsigned int				nr_ingress_mappings;
	u32					ingress_priority_map[8];
	unsigned int				nr_egress_mappings;
	struct vlan_priority_tci_mapping	*egress_priority_map[16];

	u16					vlan_id;
	u16					flags;

	struct net_device			*real_dev;
	unsigned char				real_dev_addr[ETH_ALEN];

	struct proc_dir_entry			*dent;
	struct vlan_pcpu_stats __percpu		*vlan_pcpu_stats;
#ifdef CONFIG_NET_POLL_CONTROLLER
	struct netpoll				*netpoll;
#endif
};

static inline struct vlan_dev_priv *vlan_dev_priv(const struct net_device *dev)
{
	return netdev_priv(dev);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* if this changes, algorithm will have to be reworked because this
 * depends on completely exhausting the VLAN identifier space.  Thus
 * it gives constant time look-up, but in many cases it wastes memory.
 */
#define VLAN_GROUP_ARRAY_SPLIT_PARTS  8
#define VLAN_GROUP_ARRAY_PART_LEN     (VLAN_N_VID/VLAN_GROUP_ARRAY_SPLIT_PARTS)

<<<<<<< HEAD
struct vlan_group {
	unsigned int		nr_vlan_devs;
	struct hlist_node	hlist;	/* linked list */
	struct net_device **vlan_devices_arrays[VLAN_GROUP_ARRAY_SPLIT_PARTS];
=======
enum vlan_protos {
	VLAN_PROTO_8021Q	= 0,
	VLAN_PROTO_8021AD,
	VLAN_PROTO_NUM,
};

struct vlan_group {
	unsigned int		nr_vlan_devs;
	struct hlist_node	hlist;	/* linked list */
	struct net_device **vlan_devices_arrays[VLAN_PROTO_NUM]
					       [VLAN_GROUP_ARRAY_SPLIT_PARTS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct vlan_info {
	struct net_device	*real_dev; /* The ethernet(like) device
					    * the vlan is attached to.
					    */
	struct vlan_group	grp;
	struct list_head	vid_list;
	unsigned int		nr_vids;
	struct rcu_head		rcu;
};

<<<<<<< HEAD
static inline struct net_device *vlan_group_get_device(struct vlan_group *vg,
						       u16 vlan_id)
{
	struct net_device **array;
	array = vg->vlan_devices_arrays[vlan_id / VLAN_GROUP_ARRAY_PART_LEN];
	return array ? array[vlan_id % VLAN_GROUP_ARRAY_PART_LEN] : NULL;
}

static inline void vlan_group_set_device(struct vlan_group *vg,
					 u16 vlan_id,
					 struct net_device *dev)
{
	struct net_device **array;
	if (!vg)
		return;
	array = vg->vlan_devices_arrays[vlan_id / VLAN_GROUP_ARRAY_PART_LEN];
=======
static inline int vlan_proto_idx(__be16 proto)
{
	switch (proto) {
	case htons(ETH_P_8021Q):
		return VLAN_PROTO_8021Q;
	case htons(ETH_P_8021AD):
		return VLAN_PROTO_8021AD;
	default:
		WARN(1, "invalid VLAN protocol: 0x%04x\n", ntohs(proto));
		return -EINVAL;
	}
}

static inline struct net_device *__vlan_group_get_device(struct vlan_group *vg,
							 unsigned int pidx,
							 u16 vlan_id)
{
	struct net_device **array;

	array = vg->vlan_devices_arrays[pidx]
				       [vlan_id / VLAN_GROUP_ARRAY_PART_LEN];

	/* paired with smp_wmb() in vlan_group_prealloc_vid() */
	smp_rmb();

	return array ? array[vlan_id % VLAN_GROUP_ARRAY_PART_LEN] : NULL;
}

static inline struct net_device *vlan_group_get_device(struct vlan_group *vg,
						       __be16 vlan_proto,
						       u16 vlan_id)
{
	int pidx = vlan_proto_idx(vlan_proto);

	if (pidx < 0)
		return NULL;

	return __vlan_group_get_device(vg, pidx, vlan_id);
}

static inline void vlan_group_set_device(struct vlan_group *vg,
					 __be16 vlan_proto, u16 vlan_id,
					 struct net_device *dev)
{
	int pidx = vlan_proto_idx(vlan_proto);
	struct net_device **array;

	if (!vg || pidx < 0)
		return;
	array = vg->vlan_devices_arrays[pidx]
				       [vlan_id / VLAN_GROUP_ARRAY_PART_LEN];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	array[vlan_id % VLAN_GROUP_ARRAY_PART_LEN] = dev;
}

/* Must be invoked with rcu_read_lock or with RTNL. */
static inline struct net_device *vlan_find_dev(struct net_device *real_dev,
<<<<<<< HEAD
					       u16 vlan_id)
=======
					       __be16 vlan_proto, u16 vlan_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vlan_info *vlan_info = rcu_dereference_rtnl(real_dev->vlan_info);

	if (vlan_info)
<<<<<<< HEAD
		return vlan_group_get_device(&vlan_info->grp, vlan_id);
=======
		return vlan_group_get_device(&vlan_info->grp,
					     vlan_proto, vlan_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NULL;
}

<<<<<<< HEAD
=======
static inline netdev_features_t vlan_tnl_features(struct net_device *real_dev)
{
	netdev_features_t ret;

	ret = real_dev->hw_enc_features &
	      (NETIF_F_CSUM_MASK | NETIF_F_GSO_SOFTWARE |
	       NETIF_F_GSO_ENCAP_ALL);

	if ((ret & NETIF_F_GSO_ENCAP_ALL) && (ret & NETIF_F_CSUM_MASK))
		return (ret & ~NETIF_F_CSUM_MASK) | NETIF_F_HW_CSUM;
	return 0;
}

#define vlan_group_for_each_dev(grp, i, dev) \
	for ((i) = 0; i < VLAN_PROTO_NUM * VLAN_N_VID; i++) \
		if (((dev) = __vlan_group_get_device((grp), (i) / VLAN_N_VID, \
							    (i) % VLAN_N_VID)))

int vlan_filter_push_vids(struct vlan_info *vlan_info, __be16 proto);
void vlan_filter_drop_vids(struct vlan_info *vlan_info, __be16 proto);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* found in vlan_dev.c */
void vlan_dev_set_ingress_priority(const struct net_device *dev,
				   u32 skb_prio, u16 vlan_prio);
int vlan_dev_set_egress_priority(const struct net_device *dev,
				 u32 skb_prio, u16 vlan_prio);
<<<<<<< HEAD
int vlan_dev_change_flags(const struct net_device *dev, u32 flag, u32 mask);
void vlan_dev_get_realdev_name(const struct net_device *dev, char *result);

int vlan_check_real_dev(struct net_device *real_dev, u16 vlan_id);
void vlan_setup(struct net_device *dev);
int register_vlan_dev(struct net_device *dev);
void unregister_vlan_dev(struct net_device *dev, struct list_head *head);
=======
void vlan_dev_free_egress_priority(const struct net_device *dev);
int vlan_dev_change_flags(const struct net_device *dev, u32 flag, u32 mask);
void vlan_dev_get_realdev_name(const struct net_device *dev, char *result,
			       size_t size);

int vlan_check_real_dev(struct net_device *real_dev,
			__be16 protocol, u16 vlan_id,
			struct netlink_ext_ack *extack);
void vlan_setup(struct net_device *dev);
int register_vlan_dev(struct net_device *dev, struct netlink_ext_ack *extack);
void unregister_vlan_dev(struct net_device *dev, struct list_head *head);
bool vlan_dev_inherit_address(struct net_device *dev,
			      struct net_device *real_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline u32 vlan_get_ingress_priority(struct net_device *dev,
					    u16 vlan_tci)
{
	struct vlan_dev_priv *vip = vlan_dev_priv(dev);

	return vip->ingress_priority_map[(vlan_tci >> VLAN_PRIO_SHIFT) & 0x7];
}

#ifdef CONFIG_VLAN_8021Q_GVRP
<<<<<<< HEAD
extern int vlan_gvrp_request_join(const struct net_device *dev);
extern void vlan_gvrp_request_leave(const struct net_device *dev);
extern int vlan_gvrp_init_applicant(struct net_device *dev);
extern void vlan_gvrp_uninit_applicant(struct net_device *dev);
extern int vlan_gvrp_init(void);
extern void vlan_gvrp_uninit(void);
=======
int vlan_gvrp_request_join(const struct net_device *dev);
void vlan_gvrp_request_leave(const struct net_device *dev);
int vlan_gvrp_init_applicant(struct net_device *dev);
void vlan_gvrp_uninit_applicant(struct net_device *dev);
int vlan_gvrp_init(void);
void vlan_gvrp_uninit(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline int vlan_gvrp_request_join(const struct net_device *dev) { return 0; }
static inline void vlan_gvrp_request_leave(const struct net_device *dev) {}
static inline int vlan_gvrp_init_applicant(struct net_device *dev) { return 0; }
static inline void vlan_gvrp_uninit_applicant(struct net_device *dev) {}
static inline int vlan_gvrp_init(void) { return 0; }
static inline void vlan_gvrp_uninit(void) {}
#endif

<<<<<<< HEAD
extern const char vlan_fullname[];
extern const char vlan_version[];
extern int vlan_netlink_init(void);
extern void vlan_netlink_fini(void);

extern struct rtnl_link_ops vlan_link_ops;

extern int vlan_net_id;
=======
#ifdef CONFIG_VLAN_8021Q_MVRP
int vlan_mvrp_request_join(const struct net_device *dev);
void vlan_mvrp_request_leave(const struct net_device *dev);
int vlan_mvrp_init_applicant(struct net_device *dev);
void vlan_mvrp_uninit_applicant(struct net_device *dev);
int vlan_mvrp_init(void);
void vlan_mvrp_uninit(void);
#else
static inline int vlan_mvrp_request_join(const struct net_device *dev) { return 0; }
static inline void vlan_mvrp_request_leave(const struct net_device *dev) {}
static inline int vlan_mvrp_init_applicant(struct net_device *dev) { return 0; }
static inline void vlan_mvrp_uninit_applicant(struct net_device *dev) {}
static inline int vlan_mvrp_init(void) { return 0; }
static inline void vlan_mvrp_uninit(void) {}
#endif

extern const char vlan_fullname[];
extern const char vlan_version[];
int vlan_netlink_init(void);
void vlan_netlink_fini(void);

extern struct rtnl_link_ops vlan_link_ops;

extern unsigned int vlan_net_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct proc_dir_entry;

struct vlan_net {
	/* /proc/net/vlan */
	struct proc_dir_entry *proc_vlan_dir;
	/* /proc/net/vlan/config */
	struct proc_dir_entry *proc_vlan_conf;
	/* Determines interface naming scheme. */
	unsigned short name_type;
};

#endif /* !(__BEN_VLAN_802_1Q_INC__) */
