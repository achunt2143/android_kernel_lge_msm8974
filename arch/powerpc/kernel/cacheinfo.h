<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _PPC_CACHEINFO_H
#define _PPC_CACHEINFO_H

/* These are just hooks for sysfs.c to use. */
extern void cacheinfo_cpu_online(unsigned int cpu_id);
extern void cacheinfo_cpu_offline(unsigned int cpu_id);

<<<<<<< HEAD
=======
/* Allow migration/suspend to tear down and rebuild the hierarchy. */
extern void cacheinfo_teardown(void);
extern void cacheinfo_rebuild(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _PPC_CACHEINFO_H */
