<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_SH_SECTIONS_H
#define __ASM_SH_SECTIONS_H

#include <asm-generic/sections.h>

<<<<<<< HEAD
extern long __nosave_begin, __nosave_end;
extern long __machvec_start, __machvec_end;
extern char __uncached_start, __uncached_end;
extern char _ebss[];
=======
extern char __machvec_start[], __machvec_end[];
extern char __uncached_start, __uncached_end;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern char __start_eh_frame[], __stop_eh_frame[];

#endif /* __ASM_SH_SECTIONS_H */

