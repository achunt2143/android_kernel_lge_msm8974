<<<<<<< HEAD
#ifndef __ASM_GENERIC_CURRENT_H
#define __ASM_GENERIC_CURRENT_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_CURRENT_H
#define __ASM_GENERIC_CURRENT_H

#ifndef __ASSEMBLY__
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/thread_info.h>

#define get_current() (current_thread_info()->task)
#define current get_current()
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_GENERIC_CURRENT_H */
