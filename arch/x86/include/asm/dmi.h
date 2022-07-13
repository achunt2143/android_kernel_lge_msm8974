<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_DMI_H
#define _ASM_X86_DMI_H

#include <linux/compiler.h>
#include <linux/init.h>
<<<<<<< HEAD

#include <asm/io.h>
=======
#include <linux/io.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/setup.h>

static __always_inline __init void *dmi_alloc(unsigned len)
{
	return extend_brk(len, sizeof(int));
}

/* Use early IO mappings for DMI because it's initialized early */
<<<<<<< HEAD
#define dmi_ioremap early_ioremap
#define dmi_iounmap early_iounmap
=======
#define dmi_early_remap		early_memremap
#define dmi_early_unmap		early_memunmap
#define dmi_remap(_x, _l)	memremap(_x, _l, MEMREMAP_WB)
#define dmi_unmap(_x)		memunmap(_x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_DMI_H */
