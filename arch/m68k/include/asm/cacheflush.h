<<<<<<< HEAD
#ifdef __uClinux__
#include "cacheflush_no.h"
#else
#include "cacheflush_mm.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifdef __uClinux__
#include <asm/cacheflush_no.h>
#else
#include <asm/cacheflush_mm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
