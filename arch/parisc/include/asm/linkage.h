<<<<<<< HEAD
#ifndef __ASM_PARISC_LINKAGE_H
#define __ASM_PARISC_LINKAGE_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_PARISC_LINKAGE_H
#define __ASM_PARISC_LINKAGE_H

#include <asm/dwarf.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ALIGN
#define __ALIGN         .align 4
#define __ALIGN_STR     ".align 4"
#endif

/*
 * In parisc assembly a semicolon marks a comment while a
 * exclamation mark is used to separate independent lines.
 */
<<<<<<< HEAD
#ifdef __ASSEMBLY__

#define ENTRY(name) \
	.export name !\
	ALIGN !\
name:

#ifdef CONFIG_64BIT
#define ENDPROC(name) \
	END(name)
#else
#define ENDPROC(name) \
	.type name, @function !\
	END(name)
#endif
=======
#define ASM_NL	!

#ifdef __ASSEMBLY__

#define ENTRY(name) \
	ALIGN	!\
name:		ASM_NL\
	.export name

#define ENTRY_CFI(name, ...) \
	ENTRY(name)	ASM_NL\
	.proc		ASM_NL\
	.callinfo __VA_ARGS__	ASM_NL\
	.entry		ASM_NL\
	CFI_STARTPROC

#define ENDPROC_CFI(name) \
	CFI_ENDPROC	ASM_NL\
	.exit		ASM_NL\
	.procend	ASM_NL\
	ENDPROC(name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASSEMBLY__ */

#endif  /* __ASM_PARISC_LINKAGE_H */
