<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#include <linux/stringify.h>

<<<<<<< HEAD
#define __ALIGN .align 4, 0x07
=======
#define __ALIGN .balign CONFIG_FUNCTION_ALIGNMENT, 0x07
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __ALIGN_STR __stringify(__ALIGN)

#endif
