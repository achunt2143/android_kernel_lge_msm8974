<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_IDMAP_H
#define __ASM_IDMAP_H

#include <linux/compiler.h>
<<<<<<< HEAD
#include <asm/pgtable.h>

/* Tag a function as requiring to be executed via an identity mapping. */
#define __idmap __section(.idmap.text) noinline notrace
=======
#include <linux/pgtable.h>

/* Tag a function as requiring to be executed via an identity mapping. */
#define __idmap __section(".idmap.text") noinline notrace
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern pgd_t *idmap_pgd;

void setup_mm_for_reboot(void);

#endif	/* __ASM_IDMAP_H */
