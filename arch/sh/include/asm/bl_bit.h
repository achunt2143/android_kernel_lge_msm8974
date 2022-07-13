<<<<<<< HEAD
#ifndef __ASM_SH_BL_BIT_H
#define __ASM_SH_BL_BIT_H

#ifdef CONFIG_SUPERH32
# include "bl_bit_32.h"
#else
# include "bl_bit_64.h"
#endif

#endif /* __ASM_SH_BL_BIT_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <asm/bl_bit_32.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
