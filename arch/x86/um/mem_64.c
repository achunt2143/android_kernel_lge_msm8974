<<<<<<< HEAD
#include "linux/mm.h"
#include "asm/page.h"
#include "asm/mman.h"
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/mm.h>
#include <asm/elf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && vma->vm_start == um_vdso_addr)
		return "[vdso]";

	return NULL;
}
<<<<<<< HEAD

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return NULL;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return 0;
}

int in_gate_area_no_mm(unsigned long addr)
{
	return 0;
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
