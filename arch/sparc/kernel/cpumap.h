<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _CPUMAP_H
#define _CPUMAP_H

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void cpu_map_rebuild(void);
extern int  map_to_cpu(unsigned int index);
=======
void cpu_map_rebuild(void);
int map_to_cpu(unsigned int index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define cpu_map_init() cpu_map_rebuild()
#else
#define cpu_map_init() do {} while (0)
static inline int map_to_cpu(unsigned int index)
{
	return raw_smp_processor_id();
}
#endif

#endif
