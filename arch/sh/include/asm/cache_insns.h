<<<<<<< HEAD
#ifndef __ASM_SH_CACHE_INSNS_H
#define __ASM_SH_CACHE_INSNS_H


#ifdef CONFIG_SUPERH32
# include "cache_insns_32.h"
#else
# include "cache_insns_64.h"
#endif

#endif /* __ASM_SH_CACHE_INSNS_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <asm/cache_insns_32.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
