<<<<<<< HEAD
#ifndef __NETNS_CORE_H__
#define __NETNS_CORE_H__

struct ctl_table_header;
struct prot_inuse;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NETNS_CORE_H__
#define __NETNS_CORE_H__

#include <linux/types.h>

struct ctl_table_header;
struct prot_inuse;
struct cpumask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct netns_core {
	/* core sysctls */
	struct ctl_table_header	*sysctl_hdr;

	int	sysctl_somaxconn;
<<<<<<< HEAD

	struct prot_inuse __percpu *inuse;
=======
	int	sysctl_optmem_max;
	u8	sysctl_txrehash;

#ifdef CONFIG_PROC_FS
	struct prot_inuse __percpu *prot_inuse;
#endif

#if IS_ENABLED(CONFIG_RPS) && IS_ENABLED(CONFIG_SYSCTL)
	struct cpumask *rps_default_mask;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
