<<<<<<< HEAD
#ifndef __LINUX_LTALK_H
#define __LINUX_LTALK_H

#define LTALK_HLEN		1
#define LTALK_MTU		600
#define LTALK_ALEN		1

#ifdef __KERNEL__
extern struct net_device *alloc_ltalkdev(int sizeof_priv);
#endif

#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_LTALK_H
#define __LINUX_LTALK_H

#include <uapi/linux/if_ltalk.h>

extern struct net_device *alloc_ltalkdev(int sizeof_priv);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
