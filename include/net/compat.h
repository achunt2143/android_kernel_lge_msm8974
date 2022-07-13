<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef NET_COMPAT_H
#define NET_COMPAT_H


struct sock;

<<<<<<< HEAD
#if defined(CONFIG_COMPAT)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/compat.h>

struct compat_msghdr {
	compat_uptr_t	msg_name;	/* void * */
	compat_int_t	msg_namelen;
	compat_uptr_t	msg_iov;	/* struct compat_iovec * */
	compat_size_t	msg_iovlen;
	compat_uptr_t	msg_control;	/* void * */
	compat_size_t	msg_controllen;
	compat_uint_t	msg_flags;
};

struct compat_mmsghdr {
	struct compat_msghdr msg_hdr;
	compat_uint_t        msg_len;
};

struct compat_cmsghdr {
	compat_size_t	cmsg_len;
	compat_int_t	cmsg_level;
	compat_int_t	cmsg_type;
};

<<<<<<< HEAD
extern int compat_sock_get_timestamp(struct sock *, struct timeval __user *);
extern int compat_sock_get_timestampns(struct sock *, struct timespec __user *);

#else /* defined(CONFIG_COMPAT) */
/*
 * To avoid compiler warnings:
 */
#define compat_msghdr	msghdr
#define compat_mmsghdr	mmsghdr
#endif /* defined(CONFIG_COMPAT) */

extern int get_compat_msghdr(struct msghdr *, struct compat_msghdr __user *);
extern int verify_compat_iovec(struct msghdr *, struct iovec *, struct sockaddr_storage *, int);
extern asmlinkage long compat_sys_sendmsg(int,struct compat_msghdr __user *,unsigned);
extern asmlinkage long compat_sys_sendmmsg(int, struct compat_mmsghdr __user *,
					   unsigned, unsigned);
extern asmlinkage long compat_sys_recvmsg(int,struct compat_msghdr __user *,unsigned);
extern asmlinkage long compat_sys_recvmmsg(int, struct compat_mmsghdr __user *,
					   unsigned, unsigned,
					   struct compat_timespec __user *);
extern asmlinkage long compat_sys_getsockopt(int, int, int, char __user *, int __user *);
extern int put_cmsg_compat(struct msghdr*, int, int, int, void *);

extern int cmsghdr_from_user_compat_to_kern(struct msghdr *, struct sock *, unsigned char *, int);

extern int compat_mc_setsockopt(struct sock *, int, int, char __user *, unsigned int,
	int (*)(struct sock *, int, int, char __user *, unsigned int));
extern int compat_mc_getsockopt(struct sock *, int, int, char __user *,
	int __user *, int (*)(struct sock *, int, int, char __user *,
				int __user *));
=======
struct compat_rtentry {
	u32		rt_pad1;
	struct sockaddr rt_dst;         /* target address               */
	struct sockaddr rt_gateway;     /* gateway addr (RTF_GATEWAY)   */
	struct sockaddr rt_genmask;     /* target network mask (IP)     */
	unsigned short	rt_flags;
	short		rt_pad2;
	u32		rt_pad3;
	unsigned char	rt_tos;
	unsigned char	rt_class;
	short		rt_pad4;
	short		rt_metric;      /* +1 for binary compatibility! */
	compat_uptr_t	rt_dev;         /* forcing the device at add    */
	u32		rt_mtu;         /* per route MTU/Window         */
	u32		rt_window;      /* Window clamping              */
	unsigned short  rt_irtt;        /* Initial RTT                  */
};

int __get_compat_msghdr(struct msghdr *kmsg, struct compat_msghdr *msg,
			struct sockaddr __user **save_addr);
int get_compat_msghdr(struct msghdr *, struct compat_msghdr __user *,
		      struct sockaddr __user **, struct iovec **);
int put_cmsg_compat(struct msghdr*, int, int, int, void *);

int cmsghdr_from_user_compat_to_kern(struct msghdr *, struct sock *,
				     unsigned char *, int);

struct compat_group_req {
	__u32				 gr_interface;
	struct __kernel_sockaddr_storage gr_group
		__aligned(4);
} __packed;

struct compat_group_source_req {
	__u32				 gsr_interface;
	struct __kernel_sockaddr_storage gsr_group
		__aligned(4);
	struct __kernel_sockaddr_storage gsr_source
		__aligned(4);
} __packed;

struct compat_group_filter {
	union {
		struct {
			__u32				 gf_interface_aux;
			struct __kernel_sockaddr_storage gf_group_aux
				__aligned(4);
			__u32				 gf_fmode_aux;
			__u32				 gf_numsrc_aux;
			struct __kernel_sockaddr_storage gf_slist[1]
				__aligned(4);
		} __packed;
		struct {
			__u32				 gf_interface;
			struct __kernel_sockaddr_storage gf_group
				__aligned(4);
			__u32				 gf_fmode;
			__u32				 gf_numsrc;
			struct __kernel_sockaddr_storage gf_slist_flex[]
				__aligned(4);
		} __packed;
	};
} __packed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* NET_COMPAT_H */
