<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_SH_VMLINUX_LDS_H
#define __ASM_SH_VMLINUX_LDS_H

#include <asm-generic/vmlinux.lds.h>

#ifdef CONFIG_DWARF_UNWINDER
#define DWARF_EH_FRAME							\
	.eh_frame : AT(ADDR(.eh_frame) - LOAD_OFFSET) {			\
<<<<<<< HEAD
		  VMLINUX_SYMBOL(__start_eh_frame) = .;			\
		  *(.eh_frame)						\
		  VMLINUX_SYMBOL(__stop_eh_frame) = .;			\
=======
		  __start_eh_frame = .;					\
		  *(.eh_frame)						\
		  __stop_eh_frame = .;					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#else
#define DWARF_EH_FRAME
#endif

<<<<<<< HEAD
#ifdef CONFIG_SUPERH64
#define EXTRA_TEXT		\
	*(.text64)		\
	*(.text..SHmedia32)
#else
#define EXTRA_TEXT
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_VMLINUX_LDS_H */
