<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/alpha/lib/clear_page.S
 *
 * Zero an entire page.
 */
<<<<<<< HEAD

=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.text
	.align 4
	.global clear_page
	.ent clear_page
clear_page:
	.prologue 0

	lda	$0,128
	nop
	unop
	nop

1:	stq	$31,0($16)
	stq	$31,8($16)
	stq	$31,16($16)
	stq	$31,24($16)

	stq	$31,32($16)
	stq	$31,40($16)
	stq	$31,48($16)
	subq	$0,1,$0

	stq	$31,56($16)
	addq	$16,64,$16
	unop
	bne	$0,1b

	ret
	nop
	unop
	nop

	.end clear_page
<<<<<<< HEAD
=======
	EXPORT_SYMBOL(clear_page)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
