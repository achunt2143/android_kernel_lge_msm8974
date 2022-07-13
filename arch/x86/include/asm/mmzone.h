<<<<<<< HEAD
#ifdef CONFIG_X86_32
# include "mmzone_32.h"
#else
# include "mmzone_64.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifdef CONFIG_X86_32
# include <asm/mmzone_32.h>
#else
# include <asm/mmzone_64.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
