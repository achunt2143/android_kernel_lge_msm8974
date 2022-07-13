<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/alpha/lib/strcpy.S
 * Contributed by Richard Henderson (rth@tamu.edu)
 *
 * Copy a null-terminated string from SRC to DST.  Return a pointer
 * to the null-terminator in the source.
 */
<<<<<<< HEAD

=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.text

	.align 3
	.globl strcpy
	.ent strcpy
strcpy:
	.frame $30, 0, $26
	.prologue 0

	mov	$16, $0		# set up return value
	mov	$26, $23	# set up return address
	unop
	br	__stxcpy	# do the copy

	.end strcpy
<<<<<<< HEAD
=======
	EXPORT_SYMBOL(strcpy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
