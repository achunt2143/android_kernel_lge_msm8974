<<<<<<< HEAD
#include "as-layout.h"

	.globl syscall_stub
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <as-layout.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
.section .__syscall_stub, "ax"

	.globl batch_syscall_stub
batch_syscall_stub:
<<<<<<< HEAD
	/* load pointer to first operation */
	mov	$(STUB_DATA+8), %esp

=======
	/* %esp comes in as "top of page" */
	mov %esp, %ecx
	/* %esp has pointer to first operation */
	add $8, %esp
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
again:
	/* load length of additional data */
	mov	0x0(%esp), %eax

	/* if(length == 0) : end of list */
	/* write possible 0 to header */
<<<<<<< HEAD
	mov	%eax, STUB_DATA+4
=======
	mov	%eax, 0x4(%ecx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmpl	$0, %eax
	jz	done

	/* save current pointer */
<<<<<<< HEAD
	mov	%esp, STUB_DATA+4
=======
	mov	%esp, 0x4(%ecx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* skip additional data */
	add	%eax, %esp

	/* load syscall-# */
	pop	%eax

	/* load syscall params */
	pop	%ebx
	pop	%ecx
	pop	%edx
	pop	%esi
 	pop	%edi
	pop	%ebp

	/* execute syscall */
	int	$0x80

<<<<<<< HEAD
=======
	/* restore top of page pointer in %ecx */
	mov	%esp, %ecx
	andl	$(~UM_KERN_PAGE_SIZE) + 1, %ecx

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* check return value */
	pop	%ebx
	cmp	%ebx, %eax
	je	again

done:
	/* save return value */
<<<<<<< HEAD
	mov	%eax, STUB_DATA
=======
	mov	%eax, (%ecx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* stop */
	int3
