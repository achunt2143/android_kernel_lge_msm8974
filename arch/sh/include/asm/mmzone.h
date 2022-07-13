<<<<<<< HEAD
#ifndef __ASM_SH_MMZONE_H
#define __ASM_SH_MMZONE_H

#ifdef __KERNEL__

#ifdef CONFIG_NEED_MULTIPLE_NODES
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_MMZONE_H
#define __ASM_SH_MMZONE_H

#ifdef CONFIG_NUMA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/numa.h>

extern struct pglist_data *node_data[];
#define NODE_DATA(nid)		(node_data[nid])

static inline int pfn_to_nid(unsigned long pfn)
{
	int nid;

	for (nid = 0; nid < MAX_NUMNODES; nid++)
		if (pfn >= node_start_pfn(nid) && pfn <= node_end_pfn(nid))
			break;

	return nid;
}

static inline struct pglist_data *pfn_to_pgdat(unsigned long pfn)
{
	return NODE_DATA(pfn_to_nid(pfn));
}

/* arch/sh/mm/numa.c */
void __init setup_bootmem_node(int nid, unsigned long start, unsigned long end);
#else
static inline void
setup_bootmem_node(int nid, unsigned long start, unsigned long end)
{
}
<<<<<<< HEAD
#endif /* CONFIG_NEED_MULTIPLE_NODES */
=======
#endif /* CONFIG_NUMA */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Platform specific mem init */
void __init plat_mem_setup(void);

/* arch/sh/kernel/setup.c */
void __init __add_active_range(unsigned int nid, unsigned long start_pfn,
			       unsigned long end_pfn);
/* arch/sh/mm/init.c */
void __init allocate_pgdat(unsigned int nid);

<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_MMZONE_H */
